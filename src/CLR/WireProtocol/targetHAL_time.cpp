
//
// Copyright (c) .NET Foundation and Contributors
// Portions Copyright (c) Microsoft Corporation.  All rights reserved.
// See LICENSE file in the project root for full license information.
//

#include <nanoCLR_Headers.h>

extern "C"
{

    uint64_t HAL_Time_CurrentSysTicks()
    {
        // TODO need to check if using the Win32 100ns ticks works
        return 0; // UNDONE: FIXME: EmulatorNative::GetITimeDriver()->CurrentTicks();
    }
}
