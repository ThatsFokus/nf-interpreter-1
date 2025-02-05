//
// Copyright (c) .NET Foundation and Contributors
// See LICENSE file in the project root for full license information.
//

#include <hal.h>
#include <hal_nf_community.h>

///////////////////////////////////////////////////////////////////////////////
// Driver local definitions.                                                 //
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// Driver exported variables.                                                //
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// Driver local variables and types.                                         //
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// Driver local functions.                                                   //
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// Driver exported functions.                                                //
///////////////////////////////////////////////////////////////////////////////

// HAL initialization (community part).
void halCommunityInit(void) {

///////////////////////////////
// from ChibiOS-Contrib repo //
///////////////////////////////

#if (HAL_USE_USBH == TRUE)
  usbhInit();
#endif

///////////////////////////
// nanoFramework overlay //
///////////////////////////

#if (HAL_NF_USE_STM32_FLASH == TRUE)
  stm32FlashInit();
#endif

#if (HAL_NF_USE_STM32_CRC == TRUE)
  crcInit();
#endif

#if (HAL_NF_USE_STM32_RNG == TRUE)
  rngInit();
#endif

#if HAL_NF_USE_FSMC
  stm32FsmcInit();
#endif

#if (HAL_NF_USE_STM32_ONEWIRE == TRUE)
  oneWireInit();
#endif

#if (HAL_NF_USE_STM32_QSPI == TRUE)
  qspiInit();
#endif

}
