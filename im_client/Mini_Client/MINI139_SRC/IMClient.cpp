// IMClient.cpp : ����Ӧ�ó��������Ϊ��
//

#include "stdafx.h"
#include ".\imclient.h"
#include "im\logindlg.h"
#include "im\messagehandlewnd.h"
#include "im\magicfacedlg.h"
#include "im\webprocess.h"
#include "MainFrame.h"
#include "im/DialogShowHelp.h"
#include "urlop.h"
#include "CharacterConvert.h"
#include "..\..\DuiLib\UIlib.h"
#include "..\..\Common\WndShadow.h"
#include "im\DuiGuidePage.h"
#include "CefWebUI.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#include "DumpFile.h"

// ��ʱ����
//#define _TEMP_DEBUG

#ifdef _TEMP_DEBUG
#include "DlgTest.h"
#include "im\UserRegDlg.h"
#include "im\ClusterCreateDlg.h"
#endif
// CIMClientApp

#include <initguid.h>
#include "Mini139_src_i.c"


class CMini139_srcModule :
	public ATL::CAtlMfcModule
{
public:
	DECLARE_LIBID(LIBID_Mini139_srcLib);
	DECLARE_REGISTRY_APPID_RESOURCEID(IDR_MINI139_SRC, "{E2A303DE-6D28-46D6-8146-CBF3A545F412}");};

CMini139_srcModule _AtlModule;

BEGIN_MESSAGE_MAP(CIMClientApp, CWinApp)
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()


UINT  InitServerList(LPVOID lpParam)
{
	CString strPath = (LPCSTR)lpParam;

	CString strServerListConfigFile = strPath + "serverlist.config";
	CString strServerListTempFile   = strPath + "serverlist.temp";

	CString strServerList = UrlGetStr("http://downloads.feiniu.com/iecim/serverlist.xml");
	strServerList.Trim();


	if ( strServerList.Find("<serverList>")>-1 && strServerList.Find("</serverList>")>-1 )
	{
		CStdioFile objFile;

		if ( objFile.Open(strServerListTempFile,CFile::modeCreate|CFile::modeWrite|CFile::typeText) )
		{
			objFile.WriteString(strServerList);
			objFile.Close();

			DeleteFile(strServerListConfigFile);
			rename((LPCSTR)strServerListTempFile,(LPCSTR)strServerListConfigFile);
		}
	}

	return 0;
}

// �޸ĳ�����ʹ�õ�ie�ں�
void WINAPI WriteWebBrowserRegKey(LPCTSTR lpKey,DWORD dwValue)
{
	HKEY hk;
	CString str = "Software\\Microsoft\\Internet Explorer\\Main\\FeatureControl\\";
	str += lpKey;
	if (RegCreateKey(HKEY_CURRENT_USER,str,&hk)!=0)
	{
		//AfxMessageBox("��ע���ʧ��!");
		return;
	}
	if (RegSetValueEx(hk,"moumou.exe",NULL,REG_DWORD,(const byte*)&dwValue,4)!=0)
	{
		RegCloseKey(hk);
		//AfxMessageBox("дע���ʧ��!");
		return;
		
		/*ExitProcess(-1)*/;
	}
	RegCloseKey(hk);
}


//��ȡIE�汾�� ��ȡ����mshtml.dll�ļ��İ汾�ţ���Ϊmshtml.dll�汾�ź�ie�ĺ���dll�汾����ͬ��
#pragma comment(lib,"Version.lib")
DWORD GetIeMajorVersion()
{
	const TCHAR szFilename[] = _T("mshtml.dll");  
	DWORD dwMajorVersion =0, dwMinorVersion = 0;  
	DWORD dwBuildNumber =0, dwRevisionNumber = 0;  
	DWORD dwHandle = 0;  
	DWORD dwVerInfoSize = GetFileVersionInfoSize(szFilename, &dwHandle);  
	if (dwVerInfoSize)  
	{  
		LPVOID lpBuffer = LocalAlloc(LPTR, dwVerInfoSize);  
		if (lpBuffer)  
		{  
			if (GetFileVersionInfo(szFilename, dwHandle, dwVerInfoSize, lpBuffer))  
			{  
				VS_FIXEDFILEINFO * lpFixedFileInfo = NULL;  
				UINT nFixedFileInfoSize = 0;  
				if (VerQueryValue(lpBuffer,TEXT("\\"),(LPVOID*)&lpFixedFileInfo, &nFixedFileInfoSize) &&(nFixedFileInfoSize))  
				{  
					dwMajorVersion = HIWORD(lpFixedFileInfo->dwFileVersionMS);  
					dwMinorVersion = LOWORD(lpFixedFileInfo->dwFileVersionMS);  
					dwBuildNumber = HIWORD(lpFixedFileInfo->dwFileVersionLS);  
					dwRevisionNumber = LOWORD(lpFixedFileInfo->dwFileVersionLS);  
				}  
			}  
			LocalFree(lpBuffer);  
		}  
	}  

	return dwMajorVersion;
}

// CIMClientApp ����

CIMClientApp::CIMClientApp()
{
	m_pMainFrame = NULL;
}

CIMClientApp::~CIMClientApp()
{

}

// Ψһ��һ�� CIMClientApp ����

CIMClientApp theApp;


void CIMClientApp::InitGolbalGdiObject()
{
	CResBitmap bmp;

	bmp.LoadBitmap(IDB_EMOTION2);
	g_imgEmotionList.Create(20,20,ILC_COLOR24|ILC_MASK,0,1);
	g_imgEmotionList.Add(&bmp,RGB(0,255,0));
	g_imgEmotionList.SetBkColor(RGB(255,255,255));
	bmp.DeleteObject();

	bmp.LoadBitmap(IDB_IM_STATUS24);
	g_imgStatus24.Create(24,24,ILC_COLOR8|ILC_MASK,0,1);
	g_imgStatus24.Add(&bmp,RGB(0,255,0));
	bmp.DeleteObject();


	bmp.LoadBitmap(IDB_IM_STATUS30);
	g_imgStatus30.Create(30,30,ILC_COLOR8|ILC_MASK,0,1);
	g_imgStatus30.Add(&bmp,RGB(0,255,0));
	bmp.DeleteObject();

	bmp.LoadBitmap(IDB_IM_STATUS35);
	g_imgStatus35.Create(35,35,ILC_COLOR24|ILC_MASK,0,1);
	g_imgStatus35.Add(&bmp,RGB(0,0xFF,0));
	bmp.DeleteObject();

	bmp.LoadBitmap(IDB_IM_STATUS47);
	g_imgStatus47.Create(47,47,ILC_COLOR24|ILC_MASK,0,1);
	g_imgStatus47.Add(&bmp,RGB(0,0,255));
	bmp.DeleteObject();

	bmp.LoadBitmap(IDB_IM_STATUS52);
	g_imgStatus52.Create(52,52,ILC_COLOR24|ILC_MASK,0,1);
	g_imgStatus52.Add(&bmp,RGB(255,255,255));
	bmp.DeleteObject();

	bmp.LoadBitmap(IDB_GROUP_STATUS30);
	g_imgGroupStatus30.Create(30,30,ILC_COLOR24|ILC_MASK,0,1);
	g_imgGroupStatus30.Add(&bmp,RGB(0,0,255));
	bmp.DeleteObject();

	bmp.LoadBitmap(IDB_GROUP_STATUS35);
	g_imgGroupStatus35.Create(35,35,ILC_COLOR24|ILC_MASK,0,1);
	g_imgGroupStatus35.Add(&bmp,RGB(0,0,255));
	bmp.DeleteObject();

	bmp.LoadBitmap(IDB_WIN2K_ICON);
	g_imgAppIcon2k.Create(16,16,ILC_COLOR24|ILC_MASK,0,1);
	g_imgAppIcon2k.Add(&bmp,RGB(0,255,0));
	bmp.DeleteObject();

	bmp.LoadBitmap(IDB_WINXP_ICON);
	g_imgAppIconXp.Create(16,16,ILC_COLOR24|ILC_MASK,0,1);
	g_imgAppIconXp.Add(&bmp,RGB(255,255,255));

	g_fontText.CreateFont(
		12,                        // nHeight
		0,                         // nWidth
		0,                         // nEscapement
		0,                         // nOrientation
		FW_NORMAL,                  // nWeight
		FALSE,                     // bItalic
		FALSE,                     // bUnderline
		0,                         // cStrikeOut
		ANSI_CHARSET,              // nCharSet
		OUT_DEFAULT_PRECIS,        // nOutPrecision
		CLIP_DEFAULT_PRECIS,       // nClipPrecision
		DEFAULT_QUALITY,           // nQuality
		DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
		"΢���ź�");

	

	g_fontText2.CreateFont(
		10,                        // nHeight
		0,                         // nWidth
		0,                         // nEscapement
		0,                         // nOrientation
		FW_NORMAL,                  // nWeight
		FALSE,                     // bItalic
		FALSE,                     // bUnderline
		0,                         // cStrikeOut
		ANSI_CHARSET,              // nCharSet
		OUT_DEFAULT_PRECIS,        // nOutPrecision
		CLIP_DEFAULT_PRECIS,       // nClipPrecision
		DEFAULT_QUALITY,           // nQuality
		DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
		"Microsoft Sans Serif");

	//16 ����
	g_font16.CreateFont(
		22,                        // nHeight
		0,                         // nWidth
		0,                         // nEscapement
		0,                         // nOrientation
		FW_NORMAL,                  // nWeight
		FALSE,                     // bItalic
		FALSE,                     // bUnderline
		0,                         // cStrikeOut
		ANSI_CHARSET,              // nCharSet
		OUT_DEFAULT_PRECIS,        // nOutPrecision
		CLIP_DEFAULT_PRECIS,       // nClipPrecision
		DEFAULT_QUALITY,           // nQuality
		DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
		"΢���ź�");
	
	//16 ��
	g_font16B.CreateFont(
		22,                        // nHeight
		0,                         // nWidth
		0,                         // nEscapement
		0,                         // nOrientation
		FW_BOLD,                  // nWeight
		FALSE,                     // bItalic
		FALSE,                     // bUnderline
		0,                         // cStrikeOut
		ANSI_CHARSET,              // nCharSet
		OUT_DEFAULT_PRECIS,        // nOutPrecision
		CLIP_DEFAULT_PRECIS,       // nClipPrecision
		DEFAULT_QUALITY,           // nQuality
		DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
		"΢���ź�");
	
	//14 ����
	g_font14.CreateFont(
		22,                        // nHeight
		0,                         // nWidth
		0,                         // nEscapement
		0,                         // nOrientation
		FW_NORMAL,                  // nWeight
		FALSE,                     // bItalic
		FALSE,                     // bUnderline
		0,                         // cStrikeOut
		ANSI_CHARSET,              // nCharSet
		OUT_DEFAULT_PRECIS,        // nOutPrecision
		CLIP_DEFAULT_PRECIS,       // nClipPrecision
		DEFAULT_QUALITY,           // nQuality
		DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
		"΢���ź�");
	
	//14  ��
	g_font14B.CreateFont(
		22,                        // nHeight
		0,                         // nWidth
		0,                         // nEscapement
		0,                         // nOrientation
		FW_BOLD,                  // nWeight
		FALSE,                     // bItalic
		FALSE,                     // bUnderline
		0,                         // cStrikeOut
		ANSI_CHARSET,              // nCharSet
		OUT_DEFAULT_PRECIS,        // nOutPrecision
		CLIP_DEFAULT_PRECIS,       // nClipPrecision
		DEFAULT_QUALITY,           // nQuality
		DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
		"΢���ź�");

	//13  ����
	g_font13.CreateFont(
		20,                        // nHeight
		0,                         // nWidth
		0,                         // nEscapement
		0,                         // nOrientation
		FW_NORMAL,                  // nWeight
		FALSE,                     // bItalic
		FALSE,                     // bUnderline
		0,                         // cStrikeOut
		ANSI_CHARSET,              // nCharSet
		OUT_DEFAULT_PRECIS,        // nOutPrecision
		CLIP_DEFAULT_PRECIS,       // nClipPrecision
		DEFAULT_QUALITY,           // nQuality
		DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
		"΢���ź�");


	//13  ��
	g_font13B.CreateFont(
		20,                        // nHeight
		0,                         // nWidth
		0,                         // nEscapement
		0,                         // nOrientation
		FW_BOLD,                  // nWeight
		FALSE,                     // bItalic
		FALSE,                     // bUnderline
		0,                         // cStrikeOut
		ANSI_CHARSET,              // nCharSet
		OUT_DEFAULT_PRECIS,        // nOutPrecision
		CLIP_DEFAULT_PRECIS,       // nClipPrecision
		DEFAULT_QUALITY,           // nQuality
		DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
		"΢���ź�");
	
	//12  ����
	g_font12.CreateFont(
		18,                        // nHeight
		0,                         // nWidth
		0,                         // nEscapement
		0,                         // nOrientation
		FW_NORMAL,                  // nWeight
		FALSE,                     // bItalic
		FALSE,                     // bUnderline
		0,                         // cStrikeOut
		ANSI_CHARSET,              // nCharSet
		OUT_DEFAULT_PRECIS,        // nOutPrecision
		CLIP_DEFAULT_PRECIS,       // nClipPrecision
		DEFAULT_QUALITY,           // nQuality
		DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
		"΢���ź�");

	//12  ����
	g_font8.CreateFont(
		13,                        // nHeight
		0,                         // nWidth
		0,                         // nEscapement
		0,                         // nOrientation
		FW_NORMAL,                  // nWeight
		FALSE,                     // bItalic
		FALSE,                     // bUnderline
		0,                         // cStrikeOut
		ANSI_CHARSET,              // nCharSet
		OUT_DEFAULT_PRECIS,        // nOutPrecision
		CLIP_DEFAULT_PRECIS,       // nClipPrecision
		DEFAULT_QUALITY,           // nQuality
		DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
		"΢���ź�");
	
	//12  ��
	g_font12B.CreateFont(
		18,                        // nHeight
		0,                         // nWidth
		0,                         // nEscapement
		0,                         // nOrientation
		FW_BOLD,                  // nWeight
		FALSE,                     // bItalic
		FALSE,                     // bUnderline
		0,                         // cStrikeOut
		ANSI_CHARSET,              // nCharSet
		OUT_DEFAULT_PRECIS,        // nOutPrecision
		CLIP_DEFAULT_PRECIS,       // nClipPrecision
		DEFAULT_QUALITY,           // nQuality
		DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
		"΢���ź�");


	//g_ImgIcon.Load( _T("C:\\Users\\zhangmin\\Desktop\\im\\im_client\\Mini_Client\\Mini139_res\\res\\register.bmp"));   
	LoadImageFromResource(&g_ImgIcon, IDB_ICON_ON, _T("PNG"));
	
	LoadImageFromResource(&g_ImgIconGray, IDB_ICON_OFF, _T("PNG"));
	
	LoadImageFromResource(&g_ImgIconGroup, IDB_ICON_GROUP, _T("PNG"));
	
	LoadImageFromResource(&g_ImgFaceFrame, IDB_FACE_FRAME, _T("PNG"));

	
	LoadImageFromResource(&g_ImgStatusBusy, IDB_PNG_STATUS_BUSY_SMALL, _T("PNG"));
	LoadImageFromResource(&g_ImgStatusLeave, IDB_PNG_STATUS_LEAVE_SMALL, _T("PNG"));
	LoadImageFromResource(&g_ImgStatusHide, IDB_PNG_STATUS_HIDE_SMALL, _T("PNG"));



	ImageFromIDResource(g_ImgIcon2,IDB_ICON_ON);

	
	//MakeBlackAndwhite(&g_ImgIcon);

	//ImageToGray(g_ImgIcon, g_ImgIconGray);


}

void CIMClientApp::InitVideoDriver()
{
	for ( WORD wIndex=0; wIndex<10; wIndex++ )
	{
		char szName[255], szDesc[255];
		if ( capGetDriverDescription(wIndex,szName,sizeof(szName),szDesc,sizeof(szDesc)) )
		{
			if ( strcmp(szName,"")!=0 )
			{
				g_mapVideoDriverIndex[wIndex] = szName;
			}
		}
		else
		{
			break;
		}
	}
}

void CIMClientApp::InitSpaceObjects()
{
//	HRESULT hr=S_OK;

	try
	{
		g_pWebConfig.CreateInstance(CLSID_WebConfigX);
		g_pWriteArticle.CreateInstance(CLSID_WriteArticleX);
		g_pMyArticles.CreateInstance(CLSID_MyArticlesX);
		g_pHeadPhoto.CreateInstance(CLSID_HeadPhotoX);
		g_pMyPictures.CreateInstance(CLSID_MyPicturesX);
		//g_pMiniPictureBroadCast.CreateInstance(CLSID_IMiniPictureBroadCastX);

		g_pMyArticles->SelectWriteArticleX((long)(IWriteArticleX *)g_pWriteArticle);
		g_pWriteArticle->SelectMyArticlesX((long)(IMyArticlesX *)g_pMyArticles);
		g_pMyPictures->SelectHeadPhotoX((long)(IHeadPhotoX *)g_pHeadPhoto);
	}
	catch(...)
	{
		CMessageBox::Model(NULL,"�����ʼ��ʧ�ܣ������°�װ����","��ʾ",MB_OK|MB_ICONINFORMATION);
		exit(0);
	}
}



bool CIMClientApp::IsBuyerLogin()
{
	CString strMutex;
	strMutex.Format("xid_%s",m_strCustomerName.c_str());

	g_hUserMutex=::CreateMutex(NULL,FALSE,strMutex);

	if ( GetLastError()==ERROR_ALREADY_EXISTS )
	{
		CloseHandle(g_hUserMutex);
		g_hUserMutex = NULL;
		
		HWND hwd=::FindWindow(NULL,CLIENT_NAME_BUYER);
		Json::FastWriter jFastWrite;
		Json::Value jvSellerInfo;
		jvSellerInfo["customerName"] = m_strCustomerName;
		jvSellerInfo["subMerchantName"] = m_strSubMerchantName;
		jvSellerInfo["merchantID"] = m_nSubMerchantID;
		jvSellerInfo["subMerchantID"] = m_nSubMerchantID;
		jvSellerInfo["productID"] = m_nProductID;
		
		std::string strSellerInfo = jFastWrite.write(jvSellerInfo);
		char xxx[256];
		strcpy(xxx,strSellerInfo.c_str());
		FILE *fp;

		CString strModulePath;
		TCHAR szUserPath[MAX_PATH];
		SHGetFolderPath(NULL, CSIDL_MYDOCUMENTS, NULL, SHGFP_TYPE_CURRENT, szUserPath);
		if( strlen(szUserPath) > 0 )
			strModulePath = CString(szUserPath)+"\\";
#ifdef _VER_INTERNAL_
		strModulePath = strModulePath + "MouMou_Internal";
#elif defined _CS_PLAT_
		strModulePath = strModulePath + "MouMou_Cs";
#elif defined _WORK_
		strModulePath = strModulePath + "MouMou_Work";
#elif defined _BUSINESS_VESION
		strModulePath = strModulePath + "MouMou_Business";
#else
		strModulePath = strModulePath + "MouMou";
#endif

		fp = fopen(GetModulePath()+"\\xxxzx.txt","wt");
		if(fp != NULL)
		{
			fwrite(xxx,strlen(xxx),1,fp);
			fclose(fp);
		}
		::PostMessage(hwd,WM_OPEN_SELLER_WIN,m_nSubMerchantID,1);	
		return true;
	}
	return false;
}

//������򿪲���ѡ���̼Ҵ����촰��
bool CIMClientApp::IsWorkLogin()
{
	CString strMutex;
	strMutex.Format("xid_%s","moumou_work");

	g_hUserMutex=::CreateMutex(NULL,FALSE,strMutex);

	if ( GetLastError()==ERROR_ALREADY_EXISTS )
	{
		
		CloseHandle(g_hUserMutex);
		g_hUserMutex = NULL;

		HWND hwd=::FindWindow(NULL,CLIENT_NAME_WORK);
		Json::FastWriter jFastWrite;
		Json::Value jvSellerInfo;
		jvSellerInfo["merchantID"] = m_nMerchantID;
		std::string strSellerInfo = jFastWrite.write(jvSellerInfo);
		char xxx[256];
		strcpy(xxx,strSellerInfo.c_str());
		FILE *fp;

		CString strModulePath;
		TCHAR szUserPath[MAX_PATH];
		SHGetFolderPath(NULL, CSIDL_MYDOCUMENTS, NULL, SHGFP_TYPE_CURRENT, szUserPath);
		if( strlen(szUserPath) > 0 )
			strModulePath = CString(szUserPath)+"\\";
#ifdef _VER_INTERNAL_
		strModulePath = strModulePath + "MouMou_Internal";
#elif defined _CS_PLAT_
		strModulePath = strModulePath + "MouMou_Cs";
#elif defined _WORK_
		strModulePath = strModulePath + "MouMou_Work";
#elif defined _BUSINESS_VESION
		strModulePath = strModulePath + "MouMou_Business";
#else
		strModulePath = strModulePath + "MouMou";
#endif

		fp = fopen(strModulePath+"\\xxxzx.txt","wt");
		if(fp != NULL)
		{
			fwrite(xxx,strlen(xxx),1,fp);
			fclose(fp);
		}
		::PostMessage(hwd,WM_OPEN_WORK_WIN,m_nMerchantID,1);	
		return true;
	}
	return false;
}

bool CIMClientApp::IsCsLogin()
{
	CString strMutex;
	strMutex.Format("xid_%s","moumou_cs");

	g_hUserMutex=::CreateMutex(NULL,FALSE,strMutex);
	if ( GetLastError()==ERROR_ALREADY_EXISTS )
	{
		CloseHandle(g_hUserMutex);
		g_hUserMutex = NULL;

		HWND hwd=::FindWindow(NULL,FEINIU_CS_PLAT);
		Json::FastWriter jFastWrite;
		Json::Value jvSellerInfo;
		//jvSellerInfo["merchantID"] = m_nMerchantID;
		jvSellerInfo["monitorID"] = m_nMonitorID;            //����صĿͷ�ID
		jvSellerInfo["serviceID"] = m_nServiceID;            //�ͷ�ID
		jvSellerInfo["customerID"] = m_nCustomerID;           //����ID
		std::string strSellerInfo = jFastWrite.write(jvSellerInfo);
		char xxx[256];
		strcpy(xxx,strSellerInfo.c_str());
		FILE *fp;

		CString strModulePath;
		TCHAR szUserPath[MAX_PATH];
		SHGetFolderPath(NULL, CSIDL_MYDOCUMENTS, NULL, SHGFP_TYPE_CURRENT, szUserPath);
		if( strlen(szUserPath) > 0 )
			strModulePath = CString(szUserPath)+"\\";
#ifdef _VER_INTERNAL_
		strModulePath = strModulePath + "MouMou_Internal";
#elif defined _CS_PLAT_
		strModulePath = strModulePath + "MouMou_Cs";
#elif defined _WORK_
		strModulePath = strModulePath + "MouMou_Work";
#elif defined _BUSINESS_VESION
		strModulePath = strModulePath + "MouMou_Business";
#else
		strModulePath = strModulePath + "MouMou";
#endif

		fp = fopen(strModulePath+"\\xxxzy.txt","wt");
		if(fp != NULL)
		{
			fwrite(xxx,strlen(xxx),1,fp);
			fclose(fp);
		}
		::PostMessage(hwd,WM_OPEN_CS_INSERT_MSG_WIN,m_nServiceID,1);	
		return true;
	}
	return false;
}
BOOL CIMClientApp::InitInstance()
{
	// ���һ�������� Windows XP �ϵ�Ӧ�ó����嵥ָ��Ҫ
	// ʹ�� ComCtl32.dll �汾 6 ����߰汾�����ÿ��ӻ���ʽ��
	//����Ҫ InitCommonControls()�����򣬽��޷��������ڡ�

	/////��¼�쳣�˳���dmp�ļ�
	DeclareDumpFile();

	InitCommonControls();
	::CoInitialize(NULL);
	::AfxInitRichEdit2();
	
	AfxEnableControlContainer();
	// ������׼ shell ���DDE�����ļ�������������
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);
	#if !defined(_WIN32_WCE) || defined(_CE_DCOM)
	// ͨ�� CoRegisterClassObject() ע���๤����
	if (FAILED(_AtlModule.RegisterClassObjects(CLSCTX_LOCAL_SERVER, REGCLS_MULTIPLEUSE)))
		return FALSE;
	#endif // !defined(_WIN32_WCE) || defined(_CE_DCOM)
	// Ӧ�ó������� /Embedding �� /Automation ���������ġ�
	// ��Ӧ�ó�����Ϊ�Զ������������С�
	if (cmdInfo.m_bRunEmbedded || cmdInfo.m_bRunAutomated)
	{
		// ����ʾ������
		return TRUE;
	}
	// Ӧ�ó������� /Unregserver �� /Unregister ���������ġ�
	if (cmdInfo.m_nShellCommand == CCommandLineInfo::AppUnregister)
	{
		_AtlModule.UpdateRegistryAppId(FALSE);
		_AtlModule.UnregisterServer(TRUE);
		return FALSE;
	}
	// Ӧ�ó������� /Register �� /Regserver ���������ġ�
	if (cmdInfo.m_nShellCommand == CCommandLineInfo::AppRegister)
	{
		_AtlModule.UpdateRegistryAppId(TRUE);
		_AtlModule.RegisterServer(TRUE);
		return FALSE;
	}

	CWinApp::InitInstance();
	DuiLib::CPaintManagerUI::SetInstance(m_hInstance);
	DuiLib::CPaintManagerUI::SetResourcePath(DuiLib::CPaintManagerUI::GetInstancePath());    // ָ��duilib��Դ��·��������ָ��Ϊ��exeͬĿ¼
	CWndShadow::Initialize(m_hInstance);
	CefInit();
	if (!AfxSocketInit())
	{
		CMessageBox::Model(NULL,"socket init failed!");
		return FALSE;
	}
	
	// �޸ĳ�����ʹ��ie�ں�
	DWORD dwMajorver = GetIeMajorVersion();
	if(dwMajorver > 0)
	{
		if(dwMajorver > 10)
			dwMajorver = 10;	// ���ֻ֧�ֵ�10
		WriteWebBrowserRegKey("FEATURE_BROWSER_EMULATION",dwMajorver*1000);
		//WriteWebBrowserRegKey("FEATURE_ACTIVEX_REPURPOSEDETECTION",1);
		WriteWebBrowserRegKey("FEATURE_BLOCK_LMZ_IMG",1);
		WriteWebBrowserRegKey("FEATURE_BLOCK_LMZ_OBJECT",1);
		WriteWebBrowserRegKey("FEATURE_BLOCK_LMZ_SCRIPT",1);
		WriteWebBrowserRegKey("FEATURE_Cross_Domain_Redirect_Mitigation",1);
		WriteWebBrowserRegKey("FEATURE_ENABLE_SCRIPT_PASTE_URLACTION_IF_PROMPT",1);
		WriteWebBrowserRegKey("FEATURE_LOCALMACHINE_LOCKDOWN",1);
		WriteWebBrowserRegKey("FEATURE_GPU_RENDERING",1);
	}

#ifdef _Test_
	FILE *fp;
	fp = fopen("D:\\m_lpCmdLine.txt","wt");

	if(fp != NULL)
		fprintf(fp,"%s\n",m_lpCmdLine);
#endif
#ifdef _WORK_EX_
	// ���������в���
	if(m_lpCmdLine != NULL && strcmp(m_lpCmdLine, "") != 0)
	{	// �������в���������������
		char szMerchantId[128] = {0};

		int nSellerId = 0;
		int nGoodsId = 0;

		if(m_lpCmdLine[0]=='\"')
		{
			m_lpCmdLine[strlen(m_lpCmdLine)-1] = '\0';
		}

		string strCmdLine = URLDecode(m_lpCmdLine);

		int nPosProtocolEnd = strCmdLine.find_first_of("?",0);
		std::string cmd = strCmdLine.substr(0,nPosProtocolEnd);

		//if(cmd != "\"feiniu:///" && strCmdLine.length() > 0)
		//return FALSE;

		int nPosBegin = nPosProtocolEnd + 1, nPosEnd = 0;
		int nParaCnt = 0;
		Json::FastWriter jFastWrite;
		Json::Value write;
		while(nPosEnd != string::npos && nPosEnd != strCmdLine.length() - 1 && nParaCnt < 5)
		{
			nPosEnd = strCmdLine.find_first_of("&",nPosBegin);
			std::string strPara =  strCmdLine.substr(nPosBegin,nPosEnd-nPosBegin);
			{
				int nPosKeyEnd = strPara.find_first_of("=",0);
				std::string strKey = strPara.substr(0,nPosKeyEnd);
				std::string strValue = strPara.substr(nPosKeyEnd + 1,strPara.length()-1);
				write[strKey] = strValue;
			}
			nPosBegin = nPosEnd + 1;
			nParaCnt++; //���5��

		}

		std::string strSendData = jFastWrite.write(write);

		std::string strMerchantId = write["merchantID"].asString();
		m_nMerchantID = atoi(strMerchantId.c_str());

		if(m_nMerchantID != 0)
		{
			//������Ѿ����ߣ���������������Ϣ֪ͨ�����Ӧ����
			if(IsWorkLogin()) 
				return false;
		}
		else
			//����ʧ�ܲ�����
		{
			return false;
		}
	}
#elif defined _CS_PLAT_EX_
	// ���������в���
	if(m_lpCmdLine != NULL && strcmp(m_lpCmdLine, "") != 0)
	{	// �������в���������������
		char szMerchantId[128] = {0};

		int nSellerId = 0;
		int nGoodsId = 0;

		if(m_lpCmdLine[0]=='\"')
		{
			m_lpCmdLine[strlen(m_lpCmdLine)-1] = '\0';
		}

		string strCmdLine = URLDecode(m_lpCmdLine);

		int nPosProtocolEnd = strCmdLine.find_first_of("?",0);
		std::string cmd = strCmdLine.substr(0,nPosProtocolEnd);

		//if(cmd != "\"feiniu:///" && strCmdLine.length() > 0)
		//return FALSE;

		int nPosBegin = nPosProtocolEnd + 1, nPosEnd = 0;
		int nParaCnt = 0;
		Json::FastWriter jFastWrite;
		Json::Value write;
		while(nPosEnd != string::npos && nPosEnd != strCmdLine.length() - 1 && nParaCnt < 5)
		{
			nPosEnd = strCmdLine.find_first_of("&",nPosBegin);
			std::string strPara =  strCmdLine.substr(nPosBegin,nPosEnd-nPosBegin);
			{
				int nPosKeyEnd = strPara.find_first_of("=",0);
				std::string strKey = strPara.substr(0,nPosKeyEnd);
				std::string strValue = strPara.substr(nPosKeyEnd + 1,strPara.length()-1);
				write[strKey] = strValue;
			}
			nPosBegin = nPosEnd + 1;
			nParaCnt++; //���5��

		}

		std::string strSendData = jFastWrite.write(write);

		std::string strServiceId = write["serviceID"].asString();
		std::string strMonitorId = write["monitorID"].asString();
		std::string strCustomerId = write["customerID"].asString();
		m_nServiceID = atoi(strServiceId.c_str());
		m_nMonitorID = atoi(strMonitorId.c_str());
		m_nCustomerID = atoi(strCustomerId.c_str());

		if(m_nServiceID != 0)
		{
			//���Ѿ����ߣ���������������Ϣ֪ͨ�����Ӧ����
			if(IsCsLogin()) 
				return false;
		}
		else
			//����ʧ�ܲ�����
		{
			return false;
		}
	}
#else
	// ���������в���
	if(m_lpCmdLine != NULL && strcmp(m_lpCmdLine, "") != 0)
	{	// �������в���������������
		char szUserName[128] = {0};
		char szSellerName[128] = {0};
		char szSellerID[128] = {0};
		char szGoodsID[512] = {0};

		int nSellerId = 0;
		int nGoodsId = 0;

		if(m_lpCmdLine[0]=='\"')
		{
			m_lpCmdLine[strlen(m_lpCmdLine)-1] = '\0';
		}

		string strCmdLine = URLDecode(m_lpCmdLine);
		
		int nPosProtocolEnd = strCmdLine.find_first_of("?",0);
		std::string cmd = strCmdLine.substr(0,nPosProtocolEnd);

		//if(cmd != "\"feiniu:///" && strCmdLine.length() > 0)
			//return FALSE;

		int nPosBegin = nPosProtocolEnd + 1, nPosEnd = 0;
		int nParaCnt = 0;
		Json::FastWriter jFastWrite;
		Json::Value write;
		while(nPosEnd != string::npos && nPosEnd != strCmdLine.length() - 1 && nParaCnt < 5)
		{
			nPosEnd = strCmdLine.find_first_of("&",nPosBegin);
			std::string strPara =  strCmdLine.substr(nPosBegin,nPosEnd-nPosBegin);
			{
				int nPosKeyEnd = strPara.find_first_of("=",0);
				std::string strKey = strPara.substr(0,nPosKeyEnd);
				std::string strValue = strPara.substr(nPosKeyEnd + 1,strPara.length()-1);
				write[strKey] = strValue;
			}
			nPosBegin = nPosEnd + 1;
			nParaCnt++; //���5��
			
		}

		std::string strSendData = jFastWrite.write(write);



		m_strCustomerName = write["customerName"].asString();
		m_strSubMerchantName = write["subMerchantName"].asString();
		std::string strMerchantId = write["merchantID"].asString();
		std::string strSubMerchantId = write["subMerchantID"].asString();
		std::string strGoodsId = write["productID"].asString();
		m_nMerchantID = atoi(strMerchantId.c_str());
		m_nSubMerchantID = atoi(strSubMerchantId.c_str());
		m_nProductID = atoi(strGoodsId.c_str());


#ifdef _Test_	
		if(fp != NULL)
		{
			//fwrite(m_lpCmdLine,strlen(m_lpCmdLine),1,fp);
			fprintf(fp,"%s %s %d %d %d \n",m_strCustomerName.c_str(),m_strSubMerchantName.c_str(),m_nMerchantID,m_nSubMerchantID,m_nProductID);
		}
		
		fclose(fp);
#endif
		if(!m_strCustomerName.empty() != 0  && !m_strSubMerchantName.empty()  && m_nMerchantID != 0 && m_nSubMerchantID != 0)
		{
			//������Ѿ����ߣ���������������Ϣ֪ͨ�����Ӧ����
			if(IsBuyerLogin()) 
				return false;
		}
		else
		//����ʧ�ܲ�����
		{
			return false;
		}
	}
#endif


	//init gdi plus
	GdiplusStartupInput gdiplusStartupInput; 
	GdiplusStartup(&m_gdiplusToken, &gdiplusStartupInput, NULL); 
	InitResourceDLL(GetModulePath()+"mini139_res.dll");
	InitHmgSkinControls(GetModulePath()+"hmgskinres.dll");
	InitGolbalGdiObject();
//	InitSpaceObjects();
	InitVideoDriver();

	g_pProtocol = CIMProtocol::GetInstance();
	g_pProtocol->Start();

	CString strModulePath =  GetModulePath();
	//���ó���Ŀ¼
	g_LocalData.SetProgramPath(strModulePath);
	//AfxBeginThread((AFX_THREADPROC)InitServerList,(LPVOID)(LPCSTR)strModulePath);

	//���ȱ������û�Ŀ¼
	TCHAR szUserPath[MAX_PATH];
	SHGetFolderPath(NULL, CSIDL_MYDOCUMENTS, NULL, SHGFP_TYPE_CURRENT, szUserPath);
	if( strlen(szUserPath) > 0 )
		strModulePath = CString(szUserPath)+"\\";
#ifdef _VER_INTERNAL_
	strModulePath = strModulePath + "MouMou_Internal";
#elif defined _CS_PLAT_
	strModulePath = strModulePath + "MouMou_Cs";
#elif defined _WORK_
	strModulePath = strModulePath + "MouMou_Work";
#elif defined _BUSINESS_VESION
	strModulePath = strModulePath + "MouMou_Business";
#else
	strModulePath = strModulePath + "MouMou";
#endif
	g_spaceData.SetPath(strModulePath);
	g_LocalData.SetPath(strModulePath);
	
	CIMProtocol::s_nCurVer = g_LocalData.GetCurVersion();

	g_magicFaceProcess.SetMagicPlayerPath(strModulePath);
	g_webFileProcess.SetCachePath(strModulePath+"\\WebFiles\\");	
	g_webFlashProcess.SetCachePath(strModulePath+"\\WebFlashs\\");

	g_ImUserInfo.status    = XTREAM_OFFLINE;
	g_ImUserInfo.bShowHint = TRUE;

	g_pMessageHandleWnd = CMessageHandleWnd::GetInstance();

	OSVERSIONINFO osi;

	osi.dwOSVersionInfoSize = sizeof(osi);
	GetVersionEx(&osi);

	if ( osi.dwMajorVersion==5 && osi.dwMinorVersion==0 )
	{
		::g_bOs2K=true;
	}
	else
	{
		::g_bOs2K=false;//OS �汾��2K��2K����
	}

	CLoginDlg::Show();
	DuiLib::CPaintManagerUI::MessageLoop();
	DuiLib::CPaintManagerUI::Term();
	return FALSE;
}

int CIMClientApp::ExitInstance()
{
	CefUninit();
#if !defined(_WIN32_WCE) || defined(_CE_DCOM)
	_AtlModule.RevokeClassObjects();
#endif
	CMagicFaceDlg::Destory();

	DeleteObject(g_hBackBrush);  
	DeleteObject(g_hWhiteBrush);

	if ( g_pMessageHandleWnd )
	{
		try
		{
			g_pMessageHandleWnd->DestroyWindow();
			delete g_pMessageHandleWnd;
		}
		catch(...)
		{
		}

		g_pMessageHandleWnd = NULL;
	}

	if ( m_pMainFrame )
	{
		delete m_pMainFrame;
		m_pMainFrame = NULL;
	}

	if ( g_pProtocol )
	{
		try
		{
			g_pProtocol->Stop();
			delete g_pProtocol;
		}
		catch(...)
		{
		}

		g_pProtocol=NULL;
	}

	DuiLib::CPaintManagerUI::Term();

	GdiplusShutdown(m_gdiplusToken);

	UnInitHmgSkinControls();
	UnInitResourceDLL();

	if(g_pWebConfig)
	g_pWebConfig.Release();
	if(g_pWriteArticle)
	g_pWriteArticle.Release();
	if( g_pMyArticles)
	g_pMyArticles.Release();
	if(g_pHeadPhoto)
	g_pHeadPhoto.Release();
	if(g_pMyPictures)
	g_pMyPictures.Release();
	//g_pMiniPictureBroadCast.Release();


	//ȫ��  cimage �ͷ�
	g_ImgIcon.Destroy();
	g_ImgIconGray.Destroy();
	g_ImgIconGroup.Destroy();
	g_ImgFaceFrame.Destroy();
	g_ImgStatusBusy.Destroy();  
	g_ImgStatusLeave.Destroy();
	g_ImgStatusHide.Destroy();

	::CoUninitialize();
	return CWinApp::ExitInstance();
}