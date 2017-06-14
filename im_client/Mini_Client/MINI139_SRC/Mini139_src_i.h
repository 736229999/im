

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


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


/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 475
#endif

#include "rpc.h"
#include "rpcndr.h"

#ifndef __RPCNDR_H_VERSION__
#error this stub requires an updated version of <rpcndr.h>
#endif // __RPCNDR_H_VERSION__

#ifndef COM_NO_WINDOWS_H
#include "windows.h"
#include "ole2.h"
#endif /*COM_NO_WINDOWS_H*/

#ifndef __Mini139_src_i_h__
#define __Mini139_src_i_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __ICOleImage_FWD_DEFINED__
#define __ICOleImage_FWD_DEFINED__
typedef interface ICOleImage ICOleImage;
#endif 	/* __ICOleImage_FWD_DEFINED__ */


#ifndef __IOleImage_FWD_DEFINED__
#define __IOleImage_FWD_DEFINED__
typedef interface IOleImage IOleImage;
#endif 	/* __IOleImage_FWD_DEFINED__ */


#ifndef ___ICOleImageEvents_FWD_DEFINED__
#define ___ICOleImageEvents_FWD_DEFINED__
typedef interface _ICOleImageEvents _ICOleImageEvents;
#endif 	/* ___ICOleImageEvents_FWD_DEFINED__ */


#ifndef __COleImage_FWD_DEFINED__
#define __COleImage_FWD_DEFINED__

#ifdef __cplusplus
typedef class COleImage COleImage;
#else
typedef struct COleImage COleImage;
#endif /* __cplusplus */

#endif 	/* __COleImage_FWD_DEFINED__ */


#ifndef ___IOleImageEvents_FWD_DEFINED__
#define ___IOleImageEvents_FWD_DEFINED__
typedef interface _IOleImageEvents _IOleImageEvents;
#endif 	/* ___IOleImageEvents_FWD_DEFINED__ */


#ifndef __OleImage_FWD_DEFINED__
#define __OleImage_FWD_DEFINED__

#ifdef __cplusplus
typedef class OleImage OleImage;
#else
typedef struct OleImage OleImage;
#endif /* __cplusplus */

#endif 	/* __OleImage_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"

#ifdef __cplusplus
extern "C"{
#endif 


#ifndef __ICOleImage_INTERFACE_DEFINED__
#define __ICOleImage_INTERFACE_DEFINED__

/* interface ICOleImage */
/* [unique][nonextensible][dual][uuid][object] */ 


EXTERN_C const IID IID_ICOleImage;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("B12A6538-CE42-4C80-A096-97BC261DBEB5")
    ICOleImage : public IDispatch
    {
    public:
    };
    
#else 	/* C style interface */

    typedef struct ICOleImageVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            ICOleImage * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            ICOleImage * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            ICOleImage * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            ICOleImage * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            ICOleImage * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            ICOleImage * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            ICOleImage * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        END_INTERFACE
    } ICOleImageVtbl;

    interface ICOleImage
    {
        CONST_VTBL struct ICOleImageVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ICOleImage_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define ICOleImage_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define ICOleImage_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define ICOleImage_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define ICOleImage_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define ICOleImage_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define ICOleImage_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __ICOleImage_INTERFACE_DEFINED__ */


#ifndef __IOleImage_INTERFACE_DEFINED__
#define __IOleImage_INTERFACE_DEFINED__

/* interface IOleImage */
/* [unique][nonextensible][dual][uuid][object] */ 


EXTERN_C const IID IID_IOleImage;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("3204084F-52AB-4071-A7F8-09B7E257D03A")
    IOleImage : public IDispatch
    {
    public:
    };
    
#else 	/* C style interface */

    typedef struct IOleImageVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IOleImage * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IOleImage * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IOleImage * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IOleImage * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IOleImage * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IOleImage * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IOleImage * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        END_INTERFACE
    } IOleImageVtbl;

    interface IOleImage
    {
        CONST_VTBL struct IOleImageVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IOleImage_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IOleImage_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IOleImage_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IOleImage_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IOleImage_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IOleImage_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IOleImage_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IOleImage_INTERFACE_DEFINED__ */



#ifndef __Mini139_srcLib_LIBRARY_DEFINED__
#define __Mini139_srcLib_LIBRARY_DEFINED__

/* library Mini139_srcLib */
/* [version][uuid] */ 


EXTERN_C const IID LIBID_Mini139_srcLib;

#ifndef ___ICOleImageEvents_DISPINTERFACE_DEFINED__
#define ___ICOleImageEvents_DISPINTERFACE_DEFINED__

/* dispinterface _ICOleImageEvents */
/* [uuid] */ 


EXTERN_C const IID DIID__ICOleImageEvents;

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("B37E7D9C-8DF9-4046-8145-8E17E1907709")
    _ICOleImageEvents : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct _ICOleImageEventsVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            _ICOleImageEvents * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            _ICOleImageEvents * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            _ICOleImageEvents * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            _ICOleImageEvents * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            _ICOleImageEvents * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            _ICOleImageEvents * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            _ICOleImageEvents * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        END_INTERFACE
    } _ICOleImageEventsVtbl;

    interface _ICOleImageEvents
    {
        CONST_VTBL struct _ICOleImageEventsVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define _ICOleImageEvents_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define _ICOleImageEvents_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define _ICOleImageEvents_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define _ICOleImageEvents_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define _ICOleImageEvents_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define _ICOleImageEvents_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define _ICOleImageEvents_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* ___ICOleImageEvents_DISPINTERFACE_DEFINED__ */


EXTERN_C const CLSID CLSID_COleImage;

#ifdef __cplusplus

class DECLSPEC_UUID("0C217B0F-D4AA-4B2D-9934-F51E671681CB")
COleImage;
#endif

#ifndef ___IOleImageEvents_DISPINTERFACE_DEFINED__
#define ___IOleImageEvents_DISPINTERFACE_DEFINED__

/* dispinterface _IOleImageEvents */
/* [uuid] */ 


EXTERN_C const IID DIID__IOleImageEvents;

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("A512FAA3-B7B0-4719-A655-E80B74976ECC")
    _IOleImageEvents : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct _IOleImageEventsVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            _IOleImageEvents * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            _IOleImageEvents * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            _IOleImageEvents * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            _IOleImageEvents * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            _IOleImageEvents * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            _IOleImageEvents * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            _IOleImageEvents * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        END_INTERFACE
    } _IOleImageEventsVtbl;

    interface _IOleImageEvents
    {
        CONST_VTBL struct _IOleImageEventsVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define _IOleImageEvents_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define _IOleImageEvents_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define _IOleImageEvents_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define _IOleImageEvents_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define _IOleImageEvents_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define _IOleImageEvents_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define _IOleImageEvents_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* ___IOleImageEvents_DISPINTERFACE_DEFINED__ */


EXTERN_C const CLSID CLSID_OleImage;

#ifdef __cplusplus

class DECLSPEC_UUID("515AF5D3-6031-4B49-A9FC-8728EFE2E9C3")
OleImage;
#endif
#endif /* __Mini139_srcLib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


