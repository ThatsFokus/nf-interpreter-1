﻿//
// Copyright (c) .NET Foundation and Contributors
// Portions Copyright (c) Microsoft Corporation.  All rights reserved.
// See LICENSE file in the project root for full license information.
//

#include "nf_rt_native.h"

// clang-format off

static const CLR_RT_MethodHandler method_lookup[] =
{
    NULL,
    Library_nf_rt_native_nanoFramework_Runtime_Native_ExecutionConstraint::Install___STATIC__VOID__I4__I4,
    Library_nf_rt_native_nanoFramework_Runtime_Native_GC::Run___STATIC__U4__BOOLEAN,
    Library_nf_rt_native_nanoFramework_Runtime_Native_GC::EnableGCMessages___STATIC__VOID__BOOLEAN,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    Library_nf_rt_native_nanoFramework_Runtime_Native_Power::NativeReboot___STATIC__VOID,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    Library_nf_rt_native_nanoFramework_Runtime_Native_Rtc::Native_RTC_SetSystemTime___STATIC__BOOLEAN__I4__U1__U1__U1__U1__U1__U1,
    Library_nf_rt_native_nanoFramework_Runtime_Native_SystemInfo::GetSystemVersion___STATIC__VOID__BYREF_I4__BYREF_I4__BYREF_I4__BYREF_I4,
    NULL,
    Library_nf_rt_native_nanoFramework_Runtime_Native_SystemInfo::get_OEMString___STATIC__STRING,
    Library_nf_rt_native_nanoFramework_Runtime_Native_SystemInfo::get_OEM___STATIC__U1,
    Library_nf_rt_native_nanoFramework_Runtime_Native_SystemInfo::get_Model___STATIC__U1,
    Library_nf_rt_native_nanoFramework_Runtime_Native_SystemInfo::get_SKU___STATIC__U2,
    Library_nf_rt_native_nanoFramework_Runtime_Native_SystemInfo::get_TargetName___STATIC__STRING,
    Library_nf_rt_native_nanoFramework_Runtime_Native_SystemInfo::get_Platform___STATIC__STRING,
    NULL,
    Library_nf_rt_native_nanoFramework_Runtime_Native_SystemInfo::GetNativeFloatingPointSupport___STATIC__U1,
    Library_nf_rt_native_System_Environment::get_TickCount64___STATIC__I8,
};

const CLR_RT_NativeAssemblyData g_CLR_AssemblyNative_nanoFramework_Runtime_Native =
{
    "nanoFramework.Runtime.Native",
    0x109F6F22,
    method_lookup,
    { 100, 0, 9, 0 }
};

// clang-format on
