//
// Copyright (c) .NET Foundation and Contributors
// See LICENSE file in the project root for full license information.
//
#ifndef NANO_PACKSTRUCT_H
#define NANO_PACKSTRUCT_H

// definition of PACK attribute for structs
#if defined(_MSC_VER)
// VC++ doesn't support this attribute the same way so it's definition ends up being an empty one
// for VC++ the equivalent is #pragma pack
#define __nfpack 

#elif defined(__GNUC__)
#define __nfpack __attribute__((__packed__))

#else
#error "Unknow platform. Please add definition for attribute to tell compiler to pack the struct."
#endif

#endif // NANO_PACKSTRUCT_H
