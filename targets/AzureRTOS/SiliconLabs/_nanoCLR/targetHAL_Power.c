//
// Copyright (c) .NET Foundation and Contributors
// See LICENSE file in the project root for full license information.
//

#include <nanoHAL_Power.h>
#include <nanoHAL_v2.h>
#include <target_platform.h>
#include <target_common.h>
#include <em_device.h>

#ifdef HAL_RTC_MODULE_ENABLED
extern RTC_HandleTypeDef RtcHandle;
#endif

// uint32_t WakeupReasonStore;

inline void CPU_Reset()
{
    NVIC_SystemReset();
};

inline bool CPU_IsSoftRebootSupported()
{
    return true;
};

// CPU sleep is not currently implemented in this target
inline void CPU_Sleep(SLEEP_LEVEL_type level, uint64_t wakeEvents)
{
    (void)level;
    (void)wakeEvents;
};

void CPU_SetPowerMode(PowerLevel_type powerLevel)
{
    // default to false
    // bool success = false;

    switch (powerLevel)
    {
        case PowerLevel__Off:
            // stop watchdog
            // wdgStop(&WDGD1);

            // gracefully shutdown everything
            nanoHAL_Uninitialize_C(true);

            __disable_irq();

            //             /////////////////////////////////////////////////////////////////////////
            //             // stop the idependent watchdog, for series where the option is available
            // #if defined(STM32L4XX)

            //             (void)success;
            //             // TODO FIXME this code needs to follow the same workflow as the STM32F7
            //             CLEAR_BIT(FLASH->OPTR, FLASH_OPTR_IWDG_STDBY);
            // #elif defined(STM32F7XX)

            //             // only need to change this option bit if not already done
            //             if ((FLASH->OPTCR & FLASH_OPTCR_IWDG_STDBY))
            //             {
            //                 // developer notes:
            //                 // follow workflow recommended @ 3.4.2 Option bytes programming (from programming manual)
            //                 // Authorizes the Option Byte register programming
            //                 FLASH->OPTKEYR = FLASH_OPT_KEY1;
            //                 FLASH->OPTKEYR = FLASH_OPT_KEY2;

            //                 // wait 500ms for any flash operation to be completed
            //                 success = FLASH_WaitForLastOperation(500);

            //                 if (success)
            //                 {
            //                     // write option value (clear the FLASH_OPTCR_IWDG_STDBY)
            //                     CLEAR_BIT(FLASH->OPTCR, FLASH_OPTCR_IWDG_STDBY);

            //                     // set the option start bit
            //                     FLASH->OPTCR |= FLASH_OPTCR_OPTSTRT;

            //                     // Data synchronous Barrier, forcing the CPU to respect the sequence of instruction
            //                     without
            //                     // optimization
            //                     __DSB();

            //                     // wait 100ms for the flash operation to be completed
            //                     success = FLASH_WaitForLastOperation(100);
            //                 }

            //                 // Set the OPTLOCK Bit to lock the FLASH Option Byte Registers access
            //                 FLASH->OPTCR |= FLASH_OPTCR_OPTLOCK;
            //             }

            //             (void)success;

            // #endif

            //             /////////////////////////////////////////////////////
            //             // set alarm interrupt enable
            //             // set power control register to: power down deep sleep
            //             /////////////////////////////////////////////////////

            //             // TODO
            //             //__HAL_RTC_ALARMA_ENABLE(&RtcHandle);
            //             //__HAL_RTC_ALARM_ENABLE_IT(&RtcHandle, RTC_IT_ALRA);

            //             // TODO
            //             // need review here to use ST HAL HAL_PWREx_EnterSTOP2Mode

            // #if defined(STM32F7XX)

            //             //////////////////////////////////////////////////////////////////////////////////////////////////////
            //             // workaround recommended in section 2.2.2 at STM32F77xxx errata document (DM00257543 -
            //             ES0334 Rev 5) // PWR->CSR1 |= PWR_CSR1_EIWUP;
            //             //////////////////////////////////////////////////////////////////////////////////////////////////////

            //             SET_BIT(PWR->CR1, PWR_CR1_PDDS);

            // #endif

            // set SLEEPDEEP bit of Cortex SCR
            SCB->SCR &= SCB_SCR_SLEEPDEEP_Msk;

            // wait for interrupt, and the execution dies here
            __WFI();

            break;

        default:
            // all the other power modes are unsupported here
            break;
    }
}
