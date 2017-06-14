// WebConfigX.h : CWebConfigX 的声明

#pragma once
#include "resource.h"       // 主符号
#include "WebConfig.h"
#include "atlsync.h"

class ATL_NO_VTABLE CWebConfigX : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CWebConfigX, &CLSID_WebConfigX>,
	public IDispatchImpl<IWebConfigX, &IID_IWebConfigX, &LIBID_WebConfigLib, /*wMajor =*/ 1, /*wMinor =*/ 0>
{
public:
	CWebConfigX()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_WEBCONFIGX)


BEGIN_COM_MAP(CWebConfigX)
	COM_INTERFACE_ENTRY(IWebConfigX)
	COM_INTERFACE_ENTRY(IDispatch)
END_COM_MAP()


	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{	
		if ( m_bInit==false )
		{
			strPhpAPI     = "http://api.woku.com/api/index.php";
			strMiniOnline = "http://api.woku.com/api/online.php";

			strUploadPictureASPX  = "http://fs10.woku.com/upload.aspx";
			strFixUserNameASPX    = "http://passport.woku.com/convert/convert.aspx";

			strMagicFaceASPX  = "http://shop.woku.com/IM_Magic/magicface.aspx";
			strPhotoFrameASPX = "http://shop.woku.com/IM_Magic/magicphoto.aspx ";

			strUserHomeURL	  = "http://[username].woku.com"; 
			strZoneURL        = "http://[username].woku.com/myzone/";
			strAccountURL     = "http://[username].woku.com/myzone/account/account.php";
			strUserPhotoURL   = "http://[username].woku.com/album/0.html";
			strUserArticleURL = "http://[username].woku.com/article/list.php";
			strArticleEmotionURL = "http://[username].woku.com/FCKeditor/editor/dialog/new_smiley.html";
			strArticlePictureURL = "http://[username].woku.com/photo/pictureShow.html";

			strFamilyHomeURL    = "http://[familydomain].fami.woku.com/index.php";
			strFamilyDiscussURL = "http://[familydomain].fami.woku.com/forum/index.php";
			strFamilyAlbumURL   = "http://[familydomain].fami.woku.com/album/index.html";
			strFamilySettingURL = "http://[familydomain].fami.woku.com/manage/index.php";
			strWebFaceURL       = "http://downloads.woku.com/mini/face/";

			strWebChanelURL     = "http://ent.woku.com/index.html";
			strWokuShopURL      = "http://shop.woku.com/";
			strAdvertiseURL     = "http://downloads.woku.com/mini/ad.htm";
			strJifenURL         = "http://jifen.woku.com";
			strUploadMediaUrl   = "http://[username].woku.com/media/upload.php";

			DWORD  dwID;
			HANDLE hThread  = CreateThread(NULL,0,
								(LPTHREAD_START_ROUTINE)InitProc,
								(LPVOID)this,
								0,
								&dwID);

			CloseHandle(hThread);
			m_bInit=true;
		}

		return S_OK;
	}
	
	void FinalRelease() 
	{
	}

protected:
	static UINT InitProc(LPVOID param);
	void DoInit();
	void ParaseUrl(CString strXml);

protected:
	static CString strPhpAPI,strMiniOnline;//获取文章列表
	static CString strUploadPictureASPX;
	static CString strFixUserNameASPX;
	static CString strMagicFaceASPX,strPhotoFrameASPX;
	static CString strZoneURL,strAccountURL,strUserHomeURL,strUserPhotoURL,strUserArticleURL;
	static CString strArticleEmotionURL,strArticlePictureURL;
	static CString strFamilyHomeURL,strFamilyDiscussURL,strFamilyAlbumURL,strFamilySettingURL, strWebFaceURL, strWebChanelURL;
	static CString strPicLoginUrl;
	static CString strWokuShopURL;
	static CString strAdvertiseURL;//广告链接
	static CString strJifenURL ;
	static CString strUploadMediaUrl;
	
	static bool		m_bInit;
	static CCriticalSection  m_secLock;
	
public:
	STDMETHOD(GetWebInterfaceUrl)(WebInterfaceType type, BSTR param, BSTR* pVal);
	STDMETHOD(GetWebPageUrl)(WebPageType type, BSTR param, BSTR* pVal);
};

OBJECT_ENTRY_AUTO(__uuidof(WebConfigX), CWebConfigX)
