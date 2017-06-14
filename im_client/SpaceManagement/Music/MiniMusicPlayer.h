// MiniMusicPlayer.h : CMiniMusicPlayer µÄÉùÃ÷

#pragma once
#include "resource.h"       // Ö÷·ûºÅ

#include "Music.h"


// CMiniMusicPlayer

class ATL_NO_VTABLE CMiniMusicPlayer : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CMiniMusicPlayer, &CLSID_MiniMusicPlayer>,
	public IDispatchImpl<IMiniMusicPlayer, &IID_IMiniMusicPlayer, &LIBID_MusicLib, /*wMajor =*/ 1, /*wMinor =*/ 0>
{
public:
	CMiniMusicPlayer()
	{
	}

//DECLARE_REGISTRY_RESOURCEID(IDR_MINIMUSICPLAYER)


BEGIN_COM_MAP(CMiniMusicPlayer)
	COM_INTERFACE_ENTRY(IMiniMusicPlayer)
	COM_INTERFACE_ENTRY(IDispatch)
END_COM_MAP()


	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		return S_OK;
	}
	
	void FinalRelease() 
	{
	}



public:

	//STDMETHOD(Show)(void);

	//STDMETHOD(AddFriend)(long nUserId, BSTR strUserName, BSTR strNickName, long nSex);
	STDMETHOD(Show)(void);
	STDMETHOD(AddFriend)(LONG nUserId, BSTR strUserName, BSTR strNickName, LONG nSex);
};
