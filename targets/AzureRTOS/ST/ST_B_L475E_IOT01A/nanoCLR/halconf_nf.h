//
// Copyright (c) .NET Foundation and Contributors
// See LICENSE file in the project root for full license information.
//

#ifndef _HALCONF_NF_H_
#define _HALCONF_NF_H_

// Enables the ChibiOS community overlay.
#if !defined(HAL_USE_COMMUNITY)
#define HAL_USE_COMMUNITY TRUE
#endif

// enables STM32 Flash driver
#if !defined(HAL_NF_USE_STM32_FLASH)
#define HAL_NF_USE_STM32_FLASH TRUE
#endif

#endif // _HALCONF_NF_H_
