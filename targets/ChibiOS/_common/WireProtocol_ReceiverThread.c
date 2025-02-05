//
// Copyright (c) .NET Foundation and Contributors
// See LICENSE file in the project root for full license information.
//

#include <ch.h>
#include <hal.h>
#include <cmsis_os.h>
#include <WireProtocol_Message.h>

#if (HAL_USE_SERIAL_USB == TRUE)
extern SerialUSBDriver SDU1;
#endif

// This thread needs to be implemented at ChibiOS level because it has to include a call to chThdShouldTerminateX()
// in case the thread is requested to terminate by the CMSIS call osThreadTerminate()

__attribute__((noreturn)) void ReceiverThread(void const *argument)
{
    (void)argument;

    osDelay(500);

    WP_Message_PrepareReception();

    // loop until thread receives a request to terminate
    while (1)
    {

#if (HAL_USE_SERIAL_USB == TRUE)
        // only bother to wait for WP message is USB is connected
        if (SDU1.config->usbp->state == USB_ACTIVE)
        {
#endif

            WP_Message_Process();

#if (HAL_USE_SERIAL_USB == TRUE)
            // pass control to the OS
            osThreadYield();
        }
        else
        {
            osDelay(1000);
        }

#elif (HAL_USE_SERIAL == TRUE)
        // delay here to give other threads a chance to run
        osThreadYield();
#endif
    }

    // this function never returns
}

void WP_Message_PrepareReception_Platform()
{
    // empty on purpose, nothing to configure
}
