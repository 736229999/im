

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


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


/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 475
#endif

#include "rpc.h"
#include "rpcndr.h"

#ifndef __RPCNDR_H_VERSION__
#error this stub requires an updated version of <rpcndr.h>
#endif // __RPCNDR_H_VERSION__


#ifndef __OleLinkInfoidl_h__
#define __OleLinkInfoidl_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef ___DOleLinkInfo_FWD_DEFINED__
#define ___DOleLinkInfo_FWD_DEFINED__
typedef interface _DOleLinkInfo _DOleLinkInfo;
#endif 	/* ___DOleLinkInfo_FWD_DEFINED__ */


#ifndef ___DOleLinkInfoEvents_FWD_DEFINED__
#define ___DOleLinkInfoEvents_FWD_DEFINED__
typedef interface _DOleLinkInfoEvents _DOleLinkInfoEvents;
#endif 	/* ___DOleLinkInfoEvents_FWD_DEFINED__ */


#ifndef __OleLinkInfo_FWD_DEFINED__
#define __OleLinkInfo_FWD_DEFINED__

#ifdef __cplusplus
typedef class OleLinkInfo OleLinkInfo;
#else
typedef struct OleLinkInfo OleLinkInfo;
#endif /* __cplusplus */

#endif 	/* __OleLinkInfo_FWD_DEFINED__ */


#ifdef __cplusplus
extern "C"{
#endif 



#ifndef __OleLinkInfoLib_LIBRARY_DEFINED__
#define __OleLinkInfoLib_LIBRARY_DEFINED__

/* library OleLinkInfoLib */
/* [control][version][uuid] */ 


EXTERN_C const IID LIBID_OleLinkInfoLib;

#ifndef ___DOleLinkInfo_DISPINTERFACE_DEFINED__
#define ___DOleLinkInfo_DISPINTERFACE_DEFINED__

/* dispinterface _DOleLinkInfo */
/* [uuid] */ 


EXTERN_C const IID DIID__DOleLinkInfo;

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("5C8FA553-98DF-4A73-9208-7FF1BDF0C55B")
    _DOleLinkInfo : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct _DOleLinkInfoVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            _DOleLinkInfo * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            _DOleLinkInfo * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            _DOleLinkInfo * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            _DOleLinkInfo * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            _DOleLinkInfo * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            _DOleLinkInfo * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            _DOleLinkInfo * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        END_INTERFACE
    } _DOleLinkInfoVtbl;

    interface _DOleLinkInfo
    {
        CONST_VTBL struct _DOleLinkInfoVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define _DOleLinkInfo_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define _DOleLinkInfo_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define _DOleLinkInfo_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define _DOleLinkInfo_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define _DOleLinkInfo_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define _DOleLinkInfo_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define _DOleLinkInfo_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* ___DOleLinkInfo_DISPINTERFACE_DEFINED__ */


#ifndef ___DOleLinkInfoEvents_DISPINTERFACE_DEFINED__
#define ___DOleLinkInfoEvents_DISPINTERFACE_DEFINED__

/* dispinterface _DOleLinkInfoEvents */
/* [uuid] */ 


EXTERN_C const IID DIID__DOleLinkInfoEvents;

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("99D5104D-2A9C-4F64-B3D0-AE2953EEBD1C")
    _DOleLinkInfoEvents : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct _DOleLinkInfoEventsVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            _DOleLinkInfoEvents * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            _DOleLinkInfoEvents * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            _DOleLinkInfoEvents * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            _DOleLinkInfoEvents * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            _DOleLinkInfoEvents * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            _DOleLinkInfoEvents * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            _DOleLinkInfoEvents * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        END_INTERFACE
    } _DOleLinkInfoEventsVtbl;

    interface _DOleLinkInfoEvents
    {
        CONST_VTBL struct _DOleLinkInfoEventsVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define _DOleLinkInfoEvents_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define _DOleLinkInfoEvents_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define _DOleLinkInfoEvents_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define _DOleLinkInfoEvents_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define _DOleLinkInfoEvents_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define _DOleLinkInfoEvents_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define _DOleLinkInfoEvents_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* ___DOleLinkInfoEvents_DISPINTERFACE_DEFINED__ */


EXTERN_C const CLSID CLSID_OleLinkInfo;

#ifdef __cplusplus

class DECLSPEC_UUID("7EEDABB0-A9D2-4187-864A-A48E48EA93E1")
OleLinkInfo;
#endif
#endif /* __OleLinkInfoLib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


