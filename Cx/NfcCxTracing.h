/*++

Copyright (c) 2012  Microsoft Corporation

Module Name:
    NfcCxTracing.h

Abstract:
    This module defines the WPP tracing macros for the NfcCx driver

Environment:
    User mode.

--*/

#pragma once


/////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Custom Type definitions
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////

//
// Enums
//
// begin_wpp config
// CUSTOM_TYPE(NFC_CX_STATE, ItemEnum(_NFC_CX_STATE) );
// CUSTOM_TYPE(NFC_CX_BC_CODE, ItemEnum(_NFC_CX_BC_CODE) );
// CUSTOM_TYPE(NFC_CX_TRANSPORT_TYPE, ItemEnum(_NFC_CX_TRANSPORT_TYPE) );
// CUSTOM_TYPE(NFC_CX_HOST_ACTION, ItemEnum(_NFC_CX_HOST_ACTION) );
// CUSTOM_TYPE(NFC_CX_SEQUENCE, ItemEnum(_NFC_CX_SEQUENCE) );
// CUSTOM_TYPE(SECURE_ELEMENT_TYPE, ItemEnum(_SECURE_ELEMENT_TYPE) );
// CUSTOM_TYPE(SECURE_ELEMENT_EVENT_TYPE, ItemEnum(_SECURE_ELEMENT_EVENT_TYPE) );
// CUSTOM_TYPE(SECURE_ELEMENT_CARD_EMULATION_MODE, ItemEnum(_SECURE_ELEMENT_CARD_EMULATION_MODE) );
// CUSTOM_TYPE(CARD_EMULATION_MODE_INTERNAL, ItemEnum(_CARD_EMULATION_MODE_INTERNAL) );
// CUSTOM_TYPE(SECURE_ELEMENT_ROUTING_TYPE, ItemEnum(_SECURE_ELEMENT_ROUTING_TYPE) );
// CUSTOM_TYPE(NFC_LLCP_SOCKET_TYPE, ItemEnum(_NFC_LLCP_SOCKET_INFO) );
// CUSTOM_TYPE(NFC_SNEP_SERVER_TYPE, ItemEnum(_NFC_SNEP_SERVER_TYPE) );
// CUSTOM_TYPE(NFC_RF_DISCOVERY_MODE, ItemEnum(_NFC_RF_DISCOVERY_MODE) );
// CUSTOM_TYPE(FILE_OBJECT_ROLE, ItemEnum(_FILE_OBJECT_ROLE) );
// CUSTOM_TYPE(TRANSLATION_TYPE_PROTOCOL, ItemEnum(_TRANSLATION_TYPE_PROTOCOL) );
// CUSTOM_TYPE(NFCCX_LIBNFC_MESSAGE, ItemEnum(_NFCCX_LIBNFC_MESSAGE) );
// CUSTOM_TYPE(NFCCX_CX_STATE, ItemEnum(_NFCCX_CX_STATE) );
// CUSTOM_TYPE(NFCCX_CX_EVENT, ItemEnum(_NFCCX_CX_EVENT) );
// CUSTOM_TYPE(NFCCX_CX_TRANSITION_FLAG, ItemEnum(_NFCCX_CX_TRANSITION_FLAG) );
// CUSTOM_TYPE(NFCCX_DTA_MESSAGE, ItemEnum(_NFCCX_DTA_MESSAGE) );
// end_wpp
//

//
// IOCTLs
// Note: The names of the below IOCTLs have been shortened due to a limitation in the number of
// characters that WPP can parse for custom types. If you add more IOCTLs and see that some of are
// not being decoded, it could be because of this limitation.
//
// begin_wpp config
// CUSTOM_TYPE(NFC_IOCTL, ItemListLong(SMARTCARD_POWER=0x310004, SMARTCARD_GET_ATTRIBUTE=0x310008, SMARTCARD_SET_ATTRIBUTE=0x31000C, SMARTCARD_CONFISCATE=0x310010, SMARTCARD_TRANSMIT=0x310014, SMARTCARD_EJECT=0x310018, SMARTCARD_SWALLOW=0x31001C, SMARTCARD_IS_PRESENT=0x310028, SMARTCARD_IS_ABSENT=0x31002C, SMARTCARD_SET_PROTOCOL=0x310030, SMARTCARD_GET_STATE=0x310038, SMARTCARD_GET_LAST_ERROR=0x31003C, SMARTCARD_GET_PERF_CNTR=0x310040, NFP_GET_NEXT_SUBSCRIBED_MESSAGE=0x510040, NFP_SET_PAYLOAD=0x510044, NFP_GET_NEXT_TRANSMITTED_MESSAGE=0x510048, NFP_DISABLE=0x51004C, NFP_ENABLE=0x510050, NFP_GET_MAX_MESSAGE_BYTES=0x510080, NFP_GET_KILO_BYTES_PER_SECOND=0x510084, NFCSE_ENUM_ENDPOINTS=0x220800, NFCSE_SUBSCRIBE_FOR_EVENT=0x220804, NFCSE_GET_NEXT_EVENT=0x220808, NFCSE_SET_CARD_EMULATION_MODE=0x22080C, NFCSE_GET_NFCC_CAPABILITIES=0x220810, NFCSE_GET_ROUTING_TABLE=0x220814, NFCSE_SET_ROUTING_TABLE=0x220818, NFCSE_HCE_REMOTE_RECV=0x220940, NFCSE_HCE_REMOTE_SEND=0x220944, NFCRM_SET_RADIO_STATE=0x510184, NFCRM_QUERY_RADIO_STATE=0x510188, NFCSERM_SET_RADIO_STATE=0x51018C, NFCSERM_QUERY_RADIO_STATE=0x510190) );
// CUSTOM_TYPE(NFCCX_IOCTL, ItemListLong(NFCCX_WRITE_PACKET=0x514000) );
// end_wpp
//

/////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Class method entry/exit tracing macros.  They trace the "this" pointer on entry and exit.
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////

//
// Define shorter versions of the ETW trace levels
//
#define LEVEL_CRITICAL  TRACE_LEVEL_CRITICAL
#define LEVEL_ERROR     TRACE_LEVEL_ERROR
#define LEVEL_WARNING   TRACE_LEVEL_WARNING
#define LEVEL_INFO      TRACE_LEVEL_INFORMATION
#define LEVEL_VERBOSE   TRACE_LEVEL_VERBOSE
#define LEVEL_FATAL     TRACE_LEVEL_FATAL

//
// This is a special LEVEL that changes the trace macro level from ERROR to VERBOSE
// depending on whether the return value passed to the macro was non-zero or zero,
// respectively.
//
#define LEVEL_COND 0xFFFF

//
// Override the default LOGGER and ENABLED macros to support true ETW level control
// instead of the WPP flag control.
//
#define WPP_LEVEL_LOGGER(LEVEL) (WPP_CONTROL(WPP_BIT_ ## DUMMY).Logger),
#define WPP_LEVEL_ENABLED(LEVEL) (WPP_CONTROL(WPP_BIT_ ## DUMMY).Level >= LEVEL)

//
// This macro is to be used by the WPP custom macros below that want to do conditional
// logging based on return value. If LEVEL_VERBOSE is specified when calling a macro that
// uses this, the level will be set to LEVEL_INFO if return code is 0 or
// LEVEL_ERROR if the return code is not 0. This can be called in any PRE macro.
//
// The "LEVEL == LEVEL_COND" check generates a compiler warning that the "conditional
// expression is constant" so we explicitly disable that.
#define WPP_CONDITIONAL_LEVEL_OVERRIDE(LEVEL, RET)          \
    BOOL bEnabled = WPP_LEVEL_ENABLED(LEVEL);               \
    __pragma(warning(push))                                 \
    __pragma(warning(disable: 4127))                        \
    if( LEVEL == LEVEL_COND )                               \
    {                                                       \
        if( S_OK == RET )                                   \
            bEnabled = WPP_LEVEL_ENABLED(LEVEL_VERBOSE);    \
        else                                                \
            bEnabled = WPP_LEVEL_ENABLED(LEVEL_ERROR);      \
    }                                                       \
    __pragma(warning(pop))


//*********************************************************
// MACRO: TRACE_METHOD_ENTRY
//
// begin_wpp config
// USEPREFIX (TRACE_METHOD_ENTRY, "%!STDPREFIX!%!FUNC!:%s", " ");
// FUNC TRACE_METHOD_ENTRY(TRACE_METHOD_ENTRY_EXP);
// USESUFFIX (TRACE_METHOD_ENTRY, "Enter, this=0x%p", this);
// end_wpp

#define WPP_TRACE_METHOD_ENTRY_EXP_ENABLED(LEVEL) WPP_LEVEL_ENABLED(LEVEL)
#define WPP_TRACE_METHOD_ENTRY_EXP_LOGGER(LEVEL) WPP_LEVEL_LOGGER(LEVEL)

//*********************************************************
// MACRO: TRACE_METHOD_EXIT
//
// begin_wpp config
// USEPREFIX (TRACE_METHOD_EXIT, "%!STDPREFIX!%!FUNC!:%s", " ");
// FUNC TRACE_METHOD_EXIT(TRACE_METHOD_EXIT_EXP);
// USESUFFIX (TRACE_METHOD_EXIT, "Exit, this=0x%p", this);
// end_wpp

#define WPP_TRACE_METHOD_EXIT_EXP_ENABLED(LEVEL) WPP_LEVEL_ENABLED(LEVEL)
#define WPP_TRACE_METHOD_EXIT_EXP_LOGGER(LEVEL) WPP_LEVEL_LOGGER(LEVEL)

//*********************************************************
// MACRO: TRACE_METHOD_EXIT_HR
//
// begin_wpp config
// USEPREFIX (TRACE_METHOD_EXIT_HR, "%!STDPREFIX!%!FUNC!:%s", " ");
// FUNC TRACE_METHOD_EXIT_HR(TRACE_METHOD_EXIT_HR_EXP, HR);
// USESUFFIX (TRACE_METHOD_EXIT_HR, "Exit, this=0x%p, hr=%!HRESULT!", this, HR);
// end_wpp

#define WPP_TRACE_METHOD_EXIT_HR_EXP_HR_PRE(LEVEL, HR) { WPP_CONDITIONAL_LEVEL_OVERRIDE(LEVEL, HR)
#define WPP_TRACE_METHOD_EXIT_HR_EXP_HR_POST(LEVEL, HR) ;}
#define WPP_TRACE_METHOD_EXIT_HR_EXP_HR_ENABLED(LEVEL, HR) bEnabled
#define WPP_TRACE_METHOD_EXIT_HR_EXP_HR_LOGGER(LEVEL, HR) WPP_LEVEL_LOGGER(LEVEL)

//*********************************************************
// MACRO: TRACE_METHOD_EXIT_DWORD
// 
// begin_wpp config
// USEPREFIX (TRACE_METHOD_EXIT_DWORD, "%!STDPREFIX!%!FUNC!:%s", " ");
// FUNC TRACE_METHOD_EXIT_DWORD(TRACE_METHOD_EXIT_DWORD_EXP, RETVAL);
// USESUFFIX (TRACE_METHOD_EXIT_DWORD, "Exit, this=0x%p, ret=0x%08Ix ", this, RETVAL);
// end_wpp

#define WPP_TRACE_METHOD_EXIT_DWORD_EXP_RETVAL_ENABLED(LEVEL, RETVAL) WPP_LEVEL_ENABLED(LEVEL)
#define WPP_TRACE_METHOD_EXIT_DWORD_EXP_RETVAL_LOGGER(LEVEL, RETVAL) WPP_LEVEL_LOGGER(LEVEL)

//*********************************************************
// MACRO: TRACE_METHOD_EXIT_NTSTATUS
// 
// begin_wpp config
// USEPREFIX (TRACE_METHOD_EXIT_NTSTATUS, "%!STDPREFIX!%!FUNC!:%s", " ");
// FUNC TRACE_METHOD_EXIT_NTSTATUS(TRACE_METHOD_EXIT_NTSTATUS_EXP, RETVAL);
// USESUFFIX (TRACE_METHOD_EXIT_NTSTATUS, "Exit, this=0x%p, ret=%!STATUS! ", this, RETVAL);
// end_wpp

#define WPP_TRACE_METHOD_EXIT_NTSTATUS_EXP_RETVAL_ENABLED(LEVEL, RETVAL) WPP_LEVEL_ENABLED(LEVEL)
#define WPP_TRACE_METHOD_EXIT_NTSTATUS_EXP_RETVAL_LOGGER(LEVEL, RETVAL) WPP_LEVEL_LOGGER(LEVEL)

//*********************************************************
// MACRO: TRACE_METHOD_EXIT_PTR
// 
// begin_wpp config
// USEPREFIX (TRACE_METHOD_EXIT_PTR, "%!STDPREFIX!%!FUNC!:%s", " ");
// FUNC TRACE_METHOD_EXIT_PTR(TRACE_METHOD_EXIT_PTR_EXP, RETVAL);
// USESUFFIX (TRACE_METHOD_EXIT_PTR,"Exit, this=0x%p, retptr=0x%p", this, RETVAL);
// end_wpp

#define WPP_TRACE_METHOD_EXIT_PTR_EXP_RETVAL_ENABLED(LEVEL, RETVAL) WPP_LEVEL_ENABLED(LEVEL)
#define WPP_TRACE_METHOD_EXIT_PTR_EXP_RETVAL_LOGGER(LEVEL, RETVAL) WPP_LEVEL_LOGGER(LEVEL)

/////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Function entry/exit tracing macros.  Similar to the method tracing macros, but do not trace "this" pointer.
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////

//*********************************************************
// MACRO: TRACE_FUNCTION_ENTRY
//
// begin_wpp config
// USEPREFIX (TRACE_FUNCTION_ENTRY, "%!STDPREFIX!%!FUNC!:%s", " ");
// FUNC TRACE_FUNCTION_ENTRY(TRACE_FUNCTION_ENTRY_EXP);
// USESUFFIX (TRACE_FUNCTION_ENTRY, "Enter");
// end_wpp

#define WPP_TRACE_FUNCTION_ENTRY_EXP_ENABLED(LEVEL) WPP_LEVEL_ENABLED(LEVEL)
#define WPP_TRACE_FUNCTION_ENTRY_EXP_LOGGER(LEVEL) WPP_LEVEL_LOGGER(LEVEL)

//*********************************************************
// MACRO: TRACE_FUNCTION_EXIT
//
// begin_wpp config
// USEPREFIX (TRACE_FUNCTION_EXIT, "%!STDPREFIX!%!FUNC!:%s", " ");
// FUNC TRACE_FUNCTION_EXIT(TRACE_FUNCTION_EXIT_EXP);
// USESUFFIX (TRACE_FUNCTION_EXIT, "Exit");
// end_wpp

#define WPP_TRACE_FUNCTION_EXIT_EXP_ENABLED(LEVEL) WPP_LEVEL_ENABLED(LEVEL)
#define WPP_TRACE_FUNCTION_EXIT_EXP_LOGGER(LEVEL) WPP_LEVEL_LOGGER(LEVEL)

//*********************************************************
// MACRO: TRACE_FUNCTION_EXIT_HR
// 
// begin_wpp config
// USEPREFIX (TRACE_FUNCTION_EXIT_HR, "%!STDPREFIX!%!FUNC!:%s", " ");
// FUNC TRACE_FUNCTION_EXIT_HR(TRACE_FUNCTION_EXIT_HR_EXP, HR);
// USESUFFIX (TRACE_FUNCTION_EXIT_HR, "Exit, hr=%!HRESULT!", HR);
// end_wpp

#define WPP_TRACE_FUNCTION_EXIT_HR_EXP_HR_PRE(LEVEL, HR) { WPP_CONDITIONAL_LEVEL_OVERRIDE(LEVEL, HR)
#define WPP_TRACE_FUNCTION_EXIT_HR_EXP_HR_POST(LEVEL, HR) ;}
#define WPP_TRACE_FUNCTION_EXIT_HR_EXP_HR_ENABLED(LEVEL, HR) WPP_LEVEL_ENABLED(LEVEL)
#define WPP_TRACE_FUNCTION_EXIT_HR_EXP_HR_LOGGER(LEVEL, HR) WPP_LEVEL_LOGGER(LEVEL)

//*********************************************************
// MACRO: TRACE_FUNCTION_EXIT_DWORD
// 
// begin_wpp config
// USEPREFIX (TRACE_FUNCTION_EXIT_DWORD, "%!STDPREFIX!%!FUNC!:%s", " ");
// FUNC TRACE_FUNCTION_EXIT_DWORD(TRACE_FUNCTION_EXIT_DWORD_EXP, RETVAL);
// USESUFFIX (TRACE_FUNCTION_EXIT_DWORD, "Exit, ret=0x%08Ix", RETVAL);
// end_wpp

#define WPP_TRACE_FUNCTION_EXIT_DWORD_EXP_RETVAL_ENABLED(LEVEL, RETVAL) WPP_LEVEL_ENABLED(LEVEL)
#define WPP_TRACE_FUNCTION_EXIT_DWORD_EXP_RETVAL_LOGGER(LEVEL, RETVAL) WPP_LEVEL_LOGGER(LEVEL)


//*********************************************************
// MACRO: TRACE_FUNCTION_EXIT_NTSTATUS
// 
// begin_wpp config
// USEPREFIX (TRACE_FUNCTION_EXIT_NTSTATUS, "%!STDPREFIX!%!FUNC!:%s", " ");
// FUNC TRACE_FUNCTION_EXIT_NTSTATUS(TRACE_FUNCTION_EXIT_NTSTATUS_EXP, RETVAL);
// USESUFFIX (TRACE_FUNCTION_EXIT_NTSTATUS, "Exit, status=%!STATUS!", RETVAL);
// end_wpp

#define WPP_TRACE_FUNCTION_EXIT_NTSTATUS_EXP_RETVAL_ENABLED(LEVEL, RETVAL) WPP_LEVEL_ENABLED(LEVEL)
#define WPP_TRACE_FUNCTION_EXIT_NTSTATUS_EXP_RETVAL_LOGGER(LEVEL, RETVAL) WPP_LEVEL_LOGGER(LEVEL)


//*********************************************************
// MACRO: TRACE_FUNCTION_EXIT_PTR
// 
// begin_wpp config
// USEPREFIX (TRACE_FUNCTION_EXIT_PTR, "%!STDPREFIX!%!FUNC!:%s", " ");
// FUNC TRACE_FUNCTION_EXIT_PTR(TRACE_FUNCTION_EXIT_PTR_EXP, RETVAL);
// USESUFFIX (TRACE_FUNCTION_EXIT_PTR, "Exit, retptr=0x%p", RETVAL);
// end_wpp

#define WPP_TRACE_FUNCTION_EXIT_PTR_EXP_RETVAL_ENABLED(LEVEL, RETVAL) WPP_LEVEL_ENABLED(LEVEL)
#define WPP_TRACE_FUNCTION_EXIT_PTR_EXP_RETVAL_LOGGER(LEVEL, RETVAL) WPP_LEVEL_LOGGER(LEVEL)

/////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Generic tracing macros.
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////

//*********************************************************
// MACRO: TRACE_LINE
//
// begin_wpp config
// USEPREFIX (TRACE_LINE, "%!STDPREFIX!%!FUNC!:%s", " ");
// FUNC TRACE_LINE(TRACE_LINE_EXP, MSG, ...);
// end_wpp

#define WPP_TRACE_LINE_EXP_ENABLED(LEVEL) WPP_LEVEL_ENABLED(LEVEL)
#define WPP_TRACE_LINE_EXP_LOGGER(LEVEL) WPP_LEVEL_LOGGER(LEVEL)

//*********************************************************
// MACRO: TRACE_HRESULT
//
// begin_wpp config
// USEPREFIX (TRACE_HRESULT, "%!STDPREFIX!%!FUNC!:%s", " ");
// FUNC TRACE_HRESULT(TRACE_HRESULT_EXP, HR, MSG, ...);
// USESUFFIX (TRACE_HRESULT, ", ret=%!HRESULT!", HR);
// end_wpp

#define WPP_TRACE_HRESULT_EXP_HR_PRE(LEVEL, HR) { WPP_CONDITIONAL_LEVEL_OVERRIDE(LEVEL, HR)
#define WPP_TRACE_HRESULT_EXP_HR_POST(LEVEL, HR) ;}
#define WPP_TRACE_HRESULT_EXP_HR_ENABLED(LEVEL, HR) bEnabled
#define WPP_TRACE_HRESULT_EXP_HR_LOGGER(LEVEL, HR) WPP_LEVEL_LOGGER(LEVEL)


#define W32
#define WPP_CHECK_FOR_NULL_STRING //to prevent exceptions due to NULL strings
