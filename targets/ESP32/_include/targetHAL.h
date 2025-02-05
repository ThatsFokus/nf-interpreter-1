//
// Copyright (c) .NET Foundation and Contributors
// See LICENSE file in the project root for full license information.
//

#ifndef TARGET_HAL_H
#define TARGET_HAL_H

#include <target_board.h>
#include <lwipopts.h>
#include <esp32_idf.h>

// global mutex protecting the internal state of the interpreter, including event flags
extern portMUX_TYPE globalLockMutex;
#define GLOBAL_LOCK()   portENTER_CRITICAL(&globalLockMutex);
#define GLOBAL_UNLOCK() portEXIT_CRITICAL(&globalLockMutex);

// platform dependent delay
#define PLATFORM_DELAY(milliSecs) vTaskDelay(pdMS_TO_TICKS(milliSecs));

// Definitions for Sockets/Network
#define GLOBAL_LOCK_SOCKETS(x)

// get number of sockets from lwIP options
#define PLATFORM_DEPENDENT__SOCKETS_MAX_COUNT MEMP_NUM_NETCONN

#define LPCSTR const char *

#if !defined(BUILD_RTM)

#define HARD_BREAKPOINT() HARD_Breakpoint()

// #if defined(_DEBUG)
// #define DEBUG_HARD_BREAKPOINT()     HARD_Breakpoint()
// #else
// #define DEBUG_HARD_BREAKPOINT()
// #endif

// #else

// #define HARD_BREAKPOINT()
// #define DEBUG_HARD_BREAKPOINT()

#endif // !defined(BUILD_RTM)

#define NANOCLR_STOP() HARD_BREAKPOINT()

/////////////////////////////////////////////////////////////////////////////////////////////////////
// DEBUGGER HELPER                                                                                 //
// The line below is meant to be used as helper on checking that the execution engine is running. //
// This can be inferred by checking if Events_WaitForEvents loop is running.                       //
// The implementation should is to be provided by each target at target_common.h.in                //
////////////////////////////////////////////////////////////////////////////////////////////////////
#if defined(BUILD_RTM)
#define EVENTS_HEART_BEAT
#else
#ifndef EVENTS_HEART_BEAT
#define EVENTS_HEART_BEAT __asm__ __volatile__("nop")
#endif // EVENTS_HEART_BEAT
#endif

#endif //TARGET_HAL_H
