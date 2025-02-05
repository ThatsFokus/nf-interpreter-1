//
// Copyright (c) .NET Foundation and Contributors
// See LICENSE file in the project root for full license information.
//

#ifndef TARGET_HAL_H
#define TARGET_HAL_H

#include <target_board.h>
#include <Board.h>
#include <gpio.h>
#include <ti/drivers/dpl/ClockP.h>
#include <ti/sysbios/knl/Task.h>

#define GLOBAL_LOCK()   uint32_t taskKey = Task_disable();
#define GLOBAL_UNLOCK() Task_restore(taskKey);

// platform dependent delay
#define PLATFORM_DELAY(milliSecs) ClockP_usleep(milliSecs * 1000);

// Definitions for Sockets/Network
#define GLOBAL_LOCK_SOCKETS(x)

#define PLATFORM_DEPENDENT__SOCKETS_MAX_COUNT 16

#define LPCSTR const char *

#define TRUE  true
#define FALSE false

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

// map TI SimpleLink calls to UART output to nanoFramework API
// this is valid only for debug buils
// release and RTM build don't call these
#if defined(BUILD_RTM)

#define UART_PRINT(x, ...)
#define INFO_PRINT(x, ...)

#else

#if defined(DEBUG) || defined(_DEBUG)

#define UART_PRINT DebuggerPort_WriteProxy
#define INFO_PRINT DebuggerPort_WriteProxy

#else

#define UART_PRINT(x, ...)
#define INFO_PRINT(x, ...)

#endif // DEBUG

#endif // defined(BUILD_RTM)

#define NANOCLR_STOP() HARD_BREAKPOINT()

__attribute__((always_inline)) static inline void __NOP(void)
{
    __asm("  nop");
}

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

extern int HeapBegin;
extern int HeapEnd;

extern uint32_t __nanoImage_start__;
extern uint32_t __nanoImage_end__;
extern uint32_t __nanoConfig_start__;
extern uint32_t __nanoConfig_end__;
extern uint32_t __deployment_start__;
extern uint32_t __deployment_end__;

#endif //TARGET_HAL_H
