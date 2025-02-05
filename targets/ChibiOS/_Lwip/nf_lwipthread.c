//
// Copyright (c) .NET Foundation and Contributors
// Portions Copyright (c) 2001-2004 Swedish Institute of Computer Science.  All rights reserved.
// Portions Copyright (c) 2006..2015 Giovanni Di Sirio. All rights reserved.
// See LICENSE file in the project root for full license information.
//

#include "hal.h"
#include "evtimer.h"

#include "nf_lwipthread.h"

#include <lwip/opt.h>
#include <lwip/def.h>
#include <lwip/mem.h>
#include <lwip/pbuf.h>
#include <lwip/sys.h>
#include <lwip/stats.h>
#include <lwip/snmp.h>
#include <lwip/tcpip.h>
#include <netif/etharp.h>
#include <lwip/netifapi.h>
#include <apps/sntp.h>

#if LWIP_DHCP
#include <lwip/dhcp.h>
#endif

#if LWIP_AUTOIP
#include <lwip/autoip.h>
#endif

#define PERIODIC_TIMER_ID 1
#define FRAME_RECEIVED_ID 2

/*
 * Suspension point for initialization procedure.
 */
thread_reference_t lwip_trp = NULL;

/*
 * Stack area for the LWIP-MAC thread.
 */
static THD_WORKING_AREA(wa_lwip_thread, LWIP_THREAD_STACK_SIZE);

/*
 * Initialization.
 */
static void low_level_init(struct netif *netif)
{
    /* set MAC hardware address length */
    netif->hwaddr_len = ETHARP_HWADDR_LEN;

    /* maximum transfer unit */
    netif->mtu = LWIP_NETIF_MTU;

    /* device capabilities */
    /* don't set NETIF_FLAG_ETHARP if this device is not an Ethernet one */
    netif->flags = NETIF_FLAG_BROADCAST | NETIF_FLAG_ETHARP;

    /* Do whatever else is needed to initialize interface. */
}

/*
 * This function does the actual transmission of the packet. The packet is
 * contained in the pbuf that is passed to the function. This pbuf
 * might be chained.
 *
 * @param netif the lwip network interface structure for this ethernetif
 * @param p the MAC packet to send (e.g. IP packet including MAC addresses and type)
 * @return ERR_OK if the packet could be sent
 *         an err_t value if the packet couldn't be sent
 *
 * @note Returning ERR_MEM here if a DMA queue of your MAC is full can lead to
 *       strange results. You might consider waiting for space in the DMA queue
 *       to become available since the stack doesn't retry to send a packet
 *       dropped because of memory failure (except for the TCP timers).
 */
static err_t low_level_output(struct netif *netif, struct pbuf *p)
{
    struct pbuf *q;
    MACTransmitDescriptor td;

    (void)netif;
    if (macWaitTransmitDescriptor(&ETHD1, &td, TIME_MS2I(LWIP_SEND_TIMEOUT)) != MSG_OK)
        return ERR_TIMEOUT;

#if ETH_PAD_SIZE
    pbuf_header(p, -ETH_PAD_SIZE); /* drop the padding word */
#endif

    /* Iterates through the pbuf chain. */
    for (q = p; q != NULL; q = q->next)
        macWriteTransmitDescriptor(&td, (uint8_t *)q->payload, (size_t)q->len);
    macReleaseTransmitDescriptorX(&td);

    MIB2_STATS_NETIF_ADD(netif, ifoutoctets, p->tot_len);
    if (((u8_t *)p->payload)[0] & 1)
    {
        /* broadcast or multicast packet*/
        MIB2_STATS_NETIF_INC(netif, ifoutnucastpkts);
    }
    else
    {
        /* unicast packet */
        MIB2_STATS_NETIF_INC(netif, ifoutucastpkts);
    }
    /* increase ifoutdiscards or ifouterrors on error */

#if ETH_PAD_SIZE
    pbuf_header(p, ETH_PAD_SIZE); /* reclaim the padding word */
#endif

    LINK_STATS_INC(link.xmit);

    return ERR_OK;
}

/*
 * Receives a frame.
 * Allocates a pbuf and transfers the bytes of the incoming
 * packet from the interface into the pbuf.
 *
 * @param netif the lwip network interface structure for this ethernetif
 * @return a pbuf filled with the received packet (including MAC header)
 *         NULL on memory error
 */
static bool low_level_input(struct netif *netif, struct pbuf **pbuf)
{
    MACReceiveDescriptor rd;
    struct pbuf *q;
    u16_t len;

    (void)netif;

    osalDbgAssert(pbuf != NULL, "invalid null pointer");

    if (macWaitReceiveDescriptor(&ETHD1, &rd, TIME_IMMEDIATE) != MSG_OK)
        return false;

    len = (u16_t)rd.size;

#if ETH_PAD_SIZE
    len += ETH_PAD_SIZE; /* allow room for Ethernet padding */
#endif

    /* We allocate a pbuf chain of pbufs from the pool. */
    *pbuf = pbuf_alloc(PBUF_RAW, len, PBUF_POOL);

    if (*pbuf != NULL)
    {
#if ETH_PAD_SIZE
        pbuf_header(*pbuf, -ETH_PAD_SIZE); /* drop the padding word */
#endif

        /* Iterates through the pbuf chain. */
        for (q = *pbuf; q != NULL; q = q->next)
            macReadReceiveDescriptor(&rd, (uint8_t *)q->payload, (size_t)q->len);
        macReleaseReceiveDescriptorX(&rd);

        MIB2_STATS_NETIF_ADD(netif, ifinoctets, (*pbuf)->tot_len);

        if (*(uint8_t *)((*pbuf)->payload) & 1)
        {
            /* broadcast or multicast packet*/
            MIB2_STATS_NETIF_INC(netif, ifinnucastpkts);
        }
        else
        {
            /* unicast packet*/
            MIB2_STATS_NETIF_INC(netif, ifinucastpkts);
        }

#if ETH_PAD_SIZE
        pbuf_header(*pbuf, ETH_PAD_SIZE); /* reclaim the padding word */
#endif

        LINK_STATS_INC(link.recv);
    }
    else
    {
        macReleaseReceiveDescriptorX(&rd); // Drop packet
        LINK_STATS_INC(link.memerr);
        LINK_STATS_INC(link.drop);
        MIB2_STATS_NETIF_INC(netif, ifindiscards);
    }

    return true;
}

/*
 * Called at the beginning of the program to set up the
 * network interface. It calls the function low_level_init() to do the
 * actual setup of the hardware.
 *
 * This function should be passed as a parameter to netifapi_netif_add().
 *
 * @param netif the lwip network interface structure for this ethernetif
 * @return ERR_OK if the loopif is initialised
 *         ERR_MEM if private data couldn't be allocated
 *         any other err_t on error
 */
static err_t ethernetif_init(struct netif *netif)
{
    osalDbgAssert((netif != NULL), "netif != NULL");

    /*
     * Initialize the snmp variables and counters inside the struct netif.
     * The last argument should be replaced with your link speed, in units
     * of bits per second.
     */
    MIB2_INIT_NETIF(netif, snmp_ifType_ethernet_csmacd, LWIP_LINK_SPEED);

    netif->state = NULL;
    netif->name[0] = LWIP_IFNAME0;
    netif->name[1] = LWIP_IFNAME1;
    /* We directly use etharp_output() here to save a function call.
     * You can instead declare your own function an call etharp_output()
     * from it if you have to do some checks before sending (e.g. if link
     * is available...) */
    netif->output = etharp_output;
    netif->linkoutput = low_level_output;

    /* initialize the hardware */
    low_level_init(netif);

    return ERR_OK;
}

static struct netif thisif = {0};
static net_addr_mode_t addressMode;
static ip4_addr_t ip, gateway, netmask;

struct netif *nf_getNetif()
{
    return &thisif;
}

void lwipDefaultLinkUpCB(void *p)
{
    struct netif *ifc = (struct netif *)p;
    (void)ifc;

#if LWIP_AUTOIP
    if (addressMode == NET_ADDRESS_AUTO)
    {
        autoip_start(ifc);
    }
#endif

#if LWIP_DHCP
    if (addressMode == NET_ADDRESS_DHCP)
    {
        dhcp_start(ifc);
    }
#endif

#if SNTP_SERVER_DNS
    sntp_init();
#endif
}

void lwipDefaultLinkDownCB(void *p)
{
    struct netif *ifc = (struct netif *)p;
    (void)ifc;

#if LWIP_AUTOIP
    if (addressMode == NET_ADDRESS_AUTO)
    {
        autoip_stop(ifc);
    }
#endif

#if LWIP_DHCP
    if (addressMode == NET_ADDRESS_DHCP)
    {
        dhcp_stop(ifc);
    }
#endif

#if SNTP_SERVER_DNS
    sntp_stop();
#endif
}

/**
 * @brief LWIP handling thread.
 *
 * @param[in] p pointer to a @p lwipthread_opts structure or @p NULL
 * @return The function does not return.
 */
static THD_FUNCTION(lwip_thread, p)
{
    event_timer_t evt;
    event_listener_t el0, el1;
    static const MACConfig mac_config = {thisif.hwaddr};
    err_t result;
    tcpip_callback_fn link_up_cb = NULL;
    tcpip_callback_fn link_down_cb = NULL;

    chRegSetThreadName(LWIP_THREAD_NAME);

    /* Initializes the thing.*/
    tcpip_init(NULL, NULL);

    /* TCP/IP parameters, runtime or compile time.*/
    if (p)
    {
        lwipthread_opts_t *opts = p;
        unsigned i;

        for (i = 0; i < 6; i++)
        {
            thisif.hwaddr[i] = opts->macaddress[i];
        }
#if LWIP_IPV6
        // FIXME IPV6
        // ip.u_addr = opts->address;
        // gateway.u_addr = opts->gateway;
        // netmask.u_addr = opts->netmask;
#else
        ip.addr = opts->address;
        gateway.addr = opts->gateway;
        netmask.addr = opts->netmask;
#endif
        addressMode = opts->addrMode;

#if LWIP_NETIF_HOSTNAME
        thisif.hostname = opts->ourHostName;
#endif

        link_up_cb = opts->link_up_cb;
        link_down_cb = opts->link_down_cb;
    }
    else
    {
        thisif.hwaddr[0] = LWIP_ETHADDR_0;
        thisif.hwaddr[1] = LWIP_ETHADDR_1;
        thisif.hwaddr[2] = LWIP_ETHADDR_2;
        thisif.hwaddr[3] = LWIP_ETHADDR_3;
        thisif.hwaddr[4] = LWIP_ETHADDR_4;
        thisif.hwaddr[5] = LWIP_ETHADDR_5;
#if LWIP_IPV6
        // FIXME IPV6
        // LWIP_IPADDR(&ip);
        // LWIP_GATEWAY(&gateway);
        // LWIP_NETMASK(&netmask);
#else
        LWIP_IPADDR(&ip);
        LWIP_GATEWAY(&gateway);
        LWIP_NETMASK(&netmask);
#endif
    }

#if LWIP_NETIF_HOSTNAME
    if (thisif.hostname == NULL)
    {
        thisif.hostname = LWIP_NETIF_HOSTNAME_STRING;
    }
#endif

    if (!link_up_cb)
    {
        link_up_cb = lwipDefaultLinkUpCB;
    }

    if (!link_down_cb)
    {
        link_down_cb = lwipDefaultLinkDownCB;
    }

    macStart(&ETHD1, &mac_config);

    MIB2_INIT_NETIF(&thisif, snmp_ifType_ethernet_csmacd, 0);

    /* Add interface. */
    result = netifapi_netif_add(&thisif, &ip, &netmask, &gateway, NULL, ethernetif_init, tcpip_input);
    if (result != ERR_OK)
    {
        chThdSleepMilliseconds(1000);   // Give some time to print any other diagnostics.
        osalSysHalt("netif_add error"); // Not sure what else we can do if an error occurs here.
    };

    netifapi_netif_set_default(&thisif);
    netifapi_netif_set_up(&thisif);

    /* Setup event sources.*/
    evtObjectInit(&evt, LWIP_LINK_POLL_INTERVAL);
    evtStart(&evt);
    chEvtRegisterMask(&evt.et_es, &el0, PERIODIC_TIMER_ID);
    chEvtRegisterMaskWithFlags(macGetEventSource(&ETHD1), &el1, FRAME_RECEIVED_ID, MAC_FLAGS_RX);
    chEvtAddEvents(PERIODIC_TIMER_ID | FRAME_RECEIVED_ID);

    /* Resumes the caller and goes to the final priority.*/
    chThdResume(&lwip_trp, MSG_OK);
    chThdSetPriority(LWIP_THREAD_PRIORITY);

// setup SNTP
#if SNTP_SERVER_DNS
    sntp_setoperatingmode(SNTP_OPMODE_POLL);
    sntp_setservername(0, SNTP_SERVER0_DEFAULT_ADDRESS);
    sntp_setservername(1, SNTP_SERVER1_DEFAULT_ADDRESS);
    sntp_init();
#endif

    while (true)
    {
        eventmask_t mask = chEvtWaitAny(ALL_EVENTS);
        if (mask & PERIODIC_TIMER_ID)
        {
            bool current_link_status = macPollLinkStatus(&ETHD1);
            if (current_link_status != netif_is_link_up(&thisif))
            {
                if (current_link_status)
                {
                    tcpip_callback_with_block((tcpip_callback_fn)netif_set_link_up, &thisif, 0);
                    tcpip_callback_with_block(link_up_cb, &thisif, 0);
                }
                else
                {
                    tcpip_callback_with_block((tcpip_callback_fn)netif_set_link_down, &thisif, 0);
                    tcpip_callback_with_block(link_down_cb, &thisif, 0);
                }
            }
        }

        if (mask & FRAME_RECEIVED_ID)
        {
            struct pbuf *workBuffer;
            while (low_level_input(&thisif, &workBuffer))
            {
                if (workBuffer != NULL)
                {
                    struct eth_hdr *ethhdr = workBuffer->payload;
                    switch (htons(ethhdr->type))
                    {
                        /* IP or ARP packet? */
                        case ETHTYPE_IP:
                        case ETHTYPE_ARP:
                            /* full packet send to tcpip_thread to process */
                            if (thisif.input(workBuffer, &thisif) == ERR_OK)
                                break;
                            LWIP_DEBUGF(NETIF_DEBUG, ("ethernetif_input: IP input error\n"));
                            /* Falls through */
                        default:
                            pbuf_free(workBuffer);
                    }
                }
            }
        }
    }
}

/**
 * @brief   Initializes the lwIP subsystem.
 * @note    The function exits after the initialization is finished.
 *
 * @param[in] opts      pointer to the configuration structure, if @p NULL
 *                      then the static configuration is used.
 */
void lwipInit(const lwipthread_opts_t *opts)
{

    /* Creating the lwIP thread (it changes priority internally).*/
    chThdCreateStatic(wa_lwip_thread, sizeof(wa_lwip_thread), chThdGetPriorityX() - 1, lwip_thread, (void *)opts);

    /* Waiting for the lwIP thread complete initialization. Note,
       this thread reaches the thread reference object first because
       the relative priorities.*/
    chSysLock();
    chThdSuspendS(&lwip_trp);
    chSysUnlock();
}

typedef struct lwip_reconf_params
{
    const lwipreconf_opts_t *opts;
    semaphore_t completion;
} lwip_reconf_params_t;

static void do_reconfigure(void *p)
{
    lwip_reconf_params_t *reconf = (lwip_reconf_params_t *)p;

    switch (addressMode)
    {
#if LWIP_DHCP
        case NET_ADDRESS_DHCP:
        {
            if (netif_is_up(&thisif))
                dhcp_stop(&thisif);
            break;
        }
#endif
        case NET_ADDRESS_STATIC:
        {
            ip4_addr_t zero = {0};
            netif_set_ipaddr(&thisif, &zero);
            netif_set_netmask(&thisif, &zero);
            netif_set_gw(&thisif, &zero);
            break;
        }
#if LWIP_AUTOIP
        case NET_ADDRESS_AUTO:
        {
            if (netif_is_up(&thisif))
                autoip_stop(&thisif);
            break;
        }
#endif
    }

    ip.addr = reconf->opts->address;
    gateway.addr = reconf->opts->gateway;
    netmask.addr = reconf->opts->netmask;
    addressMode = reconf->opts->addrMode;

    switch (addressMode)
    {
#if LWIP_DHCP
        case NET_ADDRESS_DHCP:
        {
            if (netif_is_up(&thisif))
                dhcp_start(&thisif);
            break;
        }
#endif
        case NET_ADDRESS_STATIC:
        {
            netif_set_ipaddr(&thisif, &ip);
            netif_set_netmask(&thisif, &netmask);
            netif_set_gw(&thisif, &gateway);
            break;
        }
#if LWIP_AUTOIP
        case NET_ADDRESS_AUTO:
        {
            if (netif_is_up(&thisif))
                autoip_start(&thisif);
            break;
        }
#endif
    }

    chSemSignal(&reconf->completion);
}

void lwipReconfigure(const lwipreconf_opts_t *opts)
{
    lwip_reconf_params_t params;
    params.opts = opts;
    chSemObjectInit(&params.completion, 0);
    tcpip_callback_with_block(do_reconfigure, &params, 0);
    chSemWait(&params.completion);
}
