

/* this ALWAYS GENERATED file contains the IIDs and CLSIDs */

/* link this file in with the server and any clients */


 /* File created by MIDL compiler version 7.00.0555 */
/* at Tue Mar 29 18:47:49 2016
 */
/* Compiler settings for Mini139_src.idl:
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

MIDL_DEFINE_GUID(IID, IID_ICOleImage,0xB12A6538,0xCE42,0x4C80,0xA0,0x96,0x97,0xBC,0x26,0x1D,0xBE,0xB5);


MIDL_DEFINE_GUID(IID, IID_IOleImage,0x3204084F,0x52AB,0x4071,0xA7,0xF8,0x09,0xB7,0xE2,0x57,0xD0,0x3A);


MIDL_DEFINE_GUID(IID, LIBID_Mini139_srcLib,0x8213BB96,0xE801,0x48F9,0xA6,0xC6,0x96,0x5C,0x1F,0x15,0x20,0xB6);


MIDL_DEFINE_GUID(IID, DIID__ICOleImageEvents,0xB37E7D9C,0x8DF9,0x4046,0x81,0x45,0x8E,0x17,0xE1,0x90,0x77,0x09);


MIDL_DEFINE_GUID(CLSID, CLSID_COleImage,0x0C217B0F,0xD4AA,0x4B2D,0x99,0x34,0xF5,0x1E,0x67,0x16,0x81,0xCB);


MIDL_DEFINE_GUID(IID, DIID__IOleImageEvents,0xA512FAA3,0xB7B0,0x4719,0xA6,0x55,0xE8,0x0B,0x74,0x97,0x6E,0xCC);


MIDL_DEFINE_GUID(CLSID, CLSID_OleImage,0x515AF5D3,0x6031,0x4B49,0xA9,0xFC,0x87,0x28,0xEF,0xE2,0xE9,0xC3);

#undef MIDL_DEFINE_GUID

#ifdef __cplusplus
}
#endif



