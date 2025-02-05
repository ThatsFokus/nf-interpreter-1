//
// Copyright (c) .NET Foundation and Contributors
// Portions Copyright (c) Microsoft Corporation.  All rights reserved.
// See LICENSE file in the project root for full license information.
//
#ifndef NANO_WEAK_H
#define NANO_WEAK_H

// definition of WEAK attribute for stub functions
#if defined(_MSC_VER)
// because VC++ doesn't support this attribute it's definition end up being an empty one
#define __nfweak 

#elif defined(__GNUC__)
#define __nfweak __attribute__((weak))

#else
#error "Unknown platform. Please add definition for weak attribute."
#endif

#endif // NANO_WEAK_H
