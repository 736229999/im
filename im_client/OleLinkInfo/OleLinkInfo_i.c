

/* this ALWAYS GENERATED file contains the IIDs and CLSIDs */

/* link this file in with the server and any clients */


 /* File created by MIDL compiler version 7.00.0555 */
/* at Fri Feb 19 11:26:50 2016
 */
/* Compiler settings for OleLinkInfo.idl:
    Oicf, W1, Zp8, env=Win32 (32b run), target_arch=X86 7.00.0555 
    protocol : dce , ms_ext, c_ext, robust
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
/* @@MIDL_FILE_HEADING(  ) */

#pragma warning( disable: 4049 )  /* more than 64k source lines */


#ifdef __cplusplus
extern "C"{
#endif 


#include <rpc.h>
#include <rpcndr.h>

#ifdef _MIDL_USE_GUIDDEF_

#ifndef INITGUID
#define INITGUID
#include <guiddef.h>
#undef INITGUID
#else
#include <guiddef.h>
#endif

#define MIDL_DEFINE_GUID(type,name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8) \
        DEFINE_GUID(name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8)

#else // !_MIDL_USE_GUIDDEF_

#ifndef __IID_DEFINED__
#define __IID_DEFINED__

typedef struct _IID
{
    unsigned long x;
    unsigned short s1;
    unsigned short s2;
    unsigned char  c[8];
} IID;

#endif // __IID_DEFINED__

#ifndef CLSID_DEFINED
#define CLSID_DEFINED
typedef IID CLSID;
#endif // CLSID_DEFINED

#define MIDL_DEFINE_GUID(type,name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8) \
        const type name = {l,w1,w2,{b1,b2,b3,b4,b5,b6,b7,b8}}

#endif !_MIDL_USE_GUIDDEF_

MIDL_DEFINE_GUID(IID, LIBID_OleLinkInfoLib,0x21DFE841,0xC6D0,0x42AF,0x8E,0xF2,0x14,0x79,0x55,0xFF,0x87,0x91);


MIDL_DEFINE_GUID(IID, DIID__DOleLinkInfo,0x5C8FA553,0x98DF,0x4A73,0x92,0x08,0x7F,0xF1,0xBD,0xF0,0xC5,0x5B);


MIDL_DEFINE_GUID(IID, DIID__DOleLinkInfoEvents,0x99D5104D,0x2A9C,0x4F64,0xB3,0xD0,0xAE,0x29,0x53,0xEE,0xBD,0x1C);


MIDL_DEFINE_GUID(CLSID, CLSID_OleLinkInfo,0x7EEDABB0,0xA9D2,0x4187,0x86,0x4A,0xA4,0x8E,0x48,0xEA,0x93,0xE1);

#undef MIDL_DEFINE_GUID

#ifdef __cplusplus
}
#endif



