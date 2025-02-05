//
// Copyright (c) .NET Foundation and Contributors
// Portions Copyright (c) Microsoft Corporation.  All rights reserved.
// See LICENSE file in the project root for full license information.
//

#include "sys_net_native.h"

HRESULT Library_sys_net_native_System_Security_Cryptography_X509Certificates_X509Certificate::
    ParseCertificate___STATIC__VOID__SZARRAY_U1__BYREF_STRING__BYREF_STRING__BYREF_SystemDateTime__BYREF_SystemDateTime(
        CLR_RT_StackFrame &stack)
{
    NATIVE_PROFILE_CLR_NETWORK();
    NANOCLR_HEADER();

    CLR_RT_HeapBlock_Array *arrData = stack.Arg0().DereferenceArray();
    CLR_UINT8 *certBytes;
    CLR_RT_HeapBlock hbIssuer;
    CLR_RT_HeapBlock hbSubject;
    CLR_RT_ProtectFromGC gc1(hbIssuer);
    CLR_RT_ProtectFromGC gc2(hbSubject);
    X509CertData cert;
    CLR_INT64 *val;
    SYSTEMTIME st;

    CLR_RT_Memory::ZeroFill(&cert, sizeof(cert));

    FAULT_ON_NULL(arrData);

    certBytes = arrData->GetFirstElement();

    if (!SSL_ParseCertificate((const char *)certBytes, arrData->m_numOfElements, &cert))
        NANOCLR_SET_AND_LEAVE(CLR_E_INVALID_PARAMETER);

    // fill in the various fields of the certificate class
    NANOCLR_CHECK_HRESULT(CLR_RT_HeapBlock_String::CreateInstance(hbIssuer, cert.Issuer));
    NANOCLR_CHECK_HRESULT(hbIssuer.StoreToReference(stack.Arg1(), 0));

    NANOCLR_CHECK_HRESULT(CLR_RT_HeapBlock_String::CreateInstance(hbSubject, cert.Subject));
    NANOCLR_CHECK_HRESULT(hbSubject.StoreToReference(stack.Arg2(), 0));

    st.wYear = cert.EffectiveDate.year;
    st.wMonth = cert.EffectiveDate.month;
    st.wDay = cert.EffectiveDate.day;
    st.wHour = cert.EffectiveDate.hour;
    st.wMinute = cert.EffectiveDate.minute;
    st.wSecond = cert.EffectiveDate.second;
    st.wMilliseconds = cert.EffectiveDate.msec;

    val = Library_corlib_native_System_DateTime::GetValuePtr(stack.Arg3());
    *val = HAL_Time_ConvertFromSystemTime(&st);

    st.wYear = cert.ExpirationDate.year;
    st.wMonth = cert.ExpirationDate.month;
    st.wDay = cert.ExpirationDate.day;
    st.wHour = cert.ExpirationDate.hour;
    st.wMinute = cert.ExpirationDate.minute;
    st.wSecond = cert.ExpirationDate.second;
    st.wMilliseconds = cert.ExpirationDate.msec;

    val = Library_corlib_native_System_DateTime::GetValuePtr(stack.Arg4());
    *val = HAL_Time_ConvertFromSystemTime(&st);

    NANOCLR_NOCLEANUP();
}
