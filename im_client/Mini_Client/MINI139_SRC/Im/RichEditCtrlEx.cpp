#include "stdafx.h"
#include "ImageDataObject.h"
#include "..\ImClient.h"
#include "..\resource.h"
#include ".\richeditctrlex.h"
#include "localdata.h"
#include "addemotiondlg.h"
#include "customemotioninfodlg.h"
#include "../OleImage.h"
#include "..\FriendInfoDlg.h"
#include "ClusterInfoDlg.h"
#include "PictureCtrl.h"
#include "WebProcess.h"
#include "MessageHandleWnd.h"
#include "DuiPicture.h"
#include "..\..\HmgCrtLib\hmgcrtlib.h"

 #ifdef _DEBUG
 #include "../Debug/olelinkinfo.tlh"
 #else
 #include "../Release/olelinkinfo.tlh"
 #endif

#define FRM_TIMER_ID 16

#define MIN_FRM_DELAY 100

#define ID_XT_MENU_CUT				0x0A01
#define ID_XT_MENU_COPY				0x0A02
#define ID_XT_MENU_PASTE			0x0A03
#define ID_XT_MENU_SAVEAS			0x0A04
#define ID_XT_ADD2EMOTION			0x0A05
#define ID_XT_CANCEL_ZYF			0x0A06
#define ID_XT_SEL_ALL				0x0A07			// 选择所有
#define ID_XT_CLEAR				0x0A08			// 清屏	

#define TIMER_CAPTURE_USERNAME  1001
#define TIME_CAPTURE_USERNAME	200


extern "C" const GUID __declspec(selectany) IID_IXPicture =
{0xAA750B0F,0x67E7,0x4288,{0xBA,0x90,0xB5,0x6D,0xC7,0x6B,0x5B,0xFF}};

/////////////////////////////////////////////////////////////////////////////
// CRichEditCtrlEx
DWORD CRichEditCtrlEx::m_dwFileSessionId=0;

IMPLEMENT_DYNAMIC(CRichEditCtrlEx,CRichEditCtrl)
CRichEditCtrlEx::CRichEditCtrlEx()
{
	m_pXFont = NULL;
	m_pImageEmotionList = NULL;
	srand(time(0));
	m_dwFileSessionId = ((rand()<<15)|(rand()<<2))|(rand()>>13);

	for ( int i=0; i<100; i++ )
	{
		m_nArrayGifId[i]=i;
	}

	m_dwBarIndex = 0;
	

	m_bZYF = true;
	m_bFocused = false;
	m_bPromptOffline = true;
	m_nPreEmotion = -1;
	m_nLimitWordSize = 0;
	m_nEditType = 0;
	m_strEditOld = "";
	m_nStartCharOld = 0;
	m_nEndCharOld = 0;
	m_pPictureCtrl = NULL;
	m_bIsShowFastWordMenu = FALSE;
	m_bIsShowLinkInformation = FALSE;
	m_bIsPointInLinkRect = FALSE;
}

CRichEditCtrlEx::~CRichEditCtrlEx()
{
	for ( int i=0; i<m_vecImageFileStruct.size(); i++ )
	{
		delete m_vecImageFileStruct[i];
	}

	m_vecImageFileStruct.clear();

	for ( int i=0; i<m_vecStaticFace.size(); i++ )
	{
		DeleteObject(m_vecStaticFace[i]);
	}
	
	m_vecStaticFace.clear();

	delete m_pXFont;
	m_pXFont=NULL;

	///*表情delete*/
	//for(int i=0; i<m_vecOleImage.size();i++)
	//{
	//	COleImage* spOleImage = m_vecOleImage[i];
	//	if(spOleImage)
	//		delete spOleImage;
	//	spOleImage = NULL;
	//}
	//m_vecOleImage.clear();

	if(m_pPictureCtrl != NULL)
	{
		delete m_pPictureCtrl;
		m_pPictureCtrl = NULL;
	}

	//if(this->m_hWnd)
	//{
	//	this->DestroyWindow();
	//}
}


BEGIN_MESSAGE_MAP(CRichEditCtrlEx, CRichEditCtrl)
	ON_NOTIFY_REFLECT(EN_LINK, OnLink)
	ON_WM_RBUTTONDOWN()
	ON_COMMAND(ID_XT_MENU_COPY, OnCopy)
	ON_COMMAND(ID_XT_MENU_CUT, OnCut)
	ON_COMMAND(ID_XT_MENU_PASTE, OnPaste)
	ON_COMMAND(ID_XT_MENU_SAVEAS, OnSaveAs)
	ON_COMMAND(ID_XT_ADD2EMOTION, OnAdd2Emotion)
	ON_COMMAND(ID_XT_CANCEL_ZYF,OnCancelZYF)
	ON_WM_DROPFILES()
	ON_WM_MOUSEWHEEL()
	ON_WM_CHAR()
	ON_WM_KEYDOWN()
	ON_WM_SETFOCUS()
	ON_WM_KILLFOCUS()
	ON_WM_TIMER()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_MOUSEMOVE()
	ON_MESSAGE(WM_IME_CHAR,OnIMEChar)
	ON_CONTROL_REFLECT(EN_CHANGE, OnEnChange)
	ON_COMMAND(ID_XT_SEL_ALL, OnSelAll)
	ON_COMMAND(ID_XT_CLEAR, OnClear)
	ON_WM_CREATE()
	ON_MESSAGE(WM_WEBFILE_NOTIFY,OnWebFileNotify)
	ON_WM_SETCURSOR()
	ON_WM_NCDESTROY()
END_MESSAGE_MAP()



LPCTSTR CRichEditCtrlEx::emotion_tips[] =
{
	"哈哈","寒","嗯","呵呵","靠","5555...","啊!!!",
	"哦?","真的吗?","饶了我吧,我投降!","啵~","zzzz...",
	"玫瑰,送你!","生病了","我很酷的","风车","shit",
	"太阳出来了",
};

LPCTSTR CRichEditCtrlEx::emotions[] = 
{    
	";D","/han","/en",":)",":<",";-|","/aa","/?",
	":-{",":]","/bo","/zz","/rs",":[","8)","9)",
	"/sht","/sun",
};

const int CRichEditCtrlEx::emotion_count = sizeof(CRichEditCtrlEx::emotions)/sizeof(LPCTSTR);

//LPCTSTR CRichEditCtrlEx::emotion_tips[] =
//{
//	"可爱","苦","色迷迷","怒","呲牙","微笑","欢呼",
//	"喜欢","大哭","眨眼","疑问","犯傻","吃惊","臭",
//	"气愤","晕","无聊","可怜","坏笑","大笑","惊讶",
//	"恶心","恐怖","装无辜","思考","媚眼","欠揍",
//	"惊吓","流泪","意外","害羞","鬼脸","棒","高兴",
//	"鬼","爱心","朝气","叹气","崇拜","鬼脸2","吻",
//	"钻石","玫瑰","蛋糕","电话","咖啡","太阳","月亮",
//	"爱","心碎","礼物","金币","钞票","猪头","大便",
//	"快餐","炸弹","真棒","刀子","下雨",
//};
//
//LPCTSTR CRichEditCtrlEx::emotions[] = 
//{    
//	"/keai","/ku","/se","/nu","/cy","/wx","/hh","/xh",
//	"/dk","/zy","/yw","/fs","/cj","/chou","/qf","/yun","/wl","/kl",
//	"/hx","/dx","/jy","/ex","/kb","/zwg","/sk","/my","/qz","/jx",
//	"/ll","/yw","/haix","/gl","/bang","/gx","/g","/ax","/zq","/tq",
//	"/cb","/gl2","/kiss","/zs","/mg","/dg","/dh","/kf","/ty","/yl",
//	"/love","/xs","/lw","/jb","/cp","/zt","/db","/kc","/zd",
//	"/zb","/dz","/xy",
//
//};

BOOL CRichEditCtrlEx::Create( DWORD in_dwStyle, const RECT& in_rcRect, CWnd* in_pParentWnd, UINT in_nID )
{
	CWnd* l_pWnd = this ;

	return l_pWnd->Create( _T( "RichEdit20A" ), NULL, in_dwStyle, in_rcRect, in_pParentWnd, in_nID ) ;

	//SetUndoLimit(100);
}



void CRichEditCtrlEx::OnLink(NMHDR *pNMHDR, LRESULT *result)
{
	ENLINK *link = (ENLINK *) pNMHDR;

	*result = 0;

	if (link->msg == WM_LBUTTONDOWN) {
		CHARRANGE cr;
		GetSel(cr);
		SetSel(link->chrg);

		static CString url = "";
		static CString urlOrigin = "";
		
		url = GetSelText();
		SetSel(cr);

		urlOrigin = url;

		url.MakeLower();

		if ( url.Find("://")>-1 || url.Find("www")>-1 || url.Find(".")>-1 )
		{
			if ( url.Find("feiniu.com")>-1 )
			{
				MiniOpenUrl(urlOrigin);
			}
			else
			{
				OpenHtmlPage(urlOrigin);
			}
		}
		else
		{
			url.Trim();

			if ( url.Find("“")==0 && url.Find("”")>0 )
			{
				url.Replace("“","");
				url.Replace("”","");
				url.Trim();


				if ( url!="")
					GetParent()->PostMessage(WM_MAGICFACE_CLICK,0,(LPARAM)(LPCSTR)url);
			}
			else
			{
				//文件接收等操作命令
				OLE_DATA_TYPE odtType;
				IMiniFileInfoBar *pFileBar = NULL; 
				IMiniMMInfoBar *pMMBar     = NULL; 

				int nMMBarPos, nFileBarPos;

				pFileBar = FindNearestFileInfoBar(link->chrg.cpMin,odtType,nFileBarPos);
				pMMBar   = FindNearestMMInfoBar(link->chrg.cpMin,odtType,nMMBarPos);

				if ( pMMBar && pFileBar )
				{
					if ( nMMBarPos>nFileBarPos )
					{
						pFileBar=NULL;
					}
					else
					{
						pMMBar = NULL;
					}
				}

				if ( pFileBar )
				{
					if ( odtType==ODT_RECV_FILE )
					{
						if ( url=="接收" )
						{
							GetParent()->PostMessage(WM_RICHEDIT_NOTIFY,RN_FILE_ACCEPT,pFileBar->OwnerData);
						}
						else
						if ( url=="拒绝" )
						{
							GetParent()->PostMessage(WM_RICHEDIT_NOTIFY,RN_FILE_REFUSE,pFileBar->OwnerData);
						}
						else
						if ( url=="另存为" )
						{
							GetParent()->PostMessage(WM_RICHEDIT_NOTIFY,RN_FILE_SAVEAS,pFileBar->OwnerData);
						}						
					}
					else
					if ( odtType==ODT_SEND_FILE )
					{
						if ( url=="取消" )
						{
							GetParent()->PostMessage(WM_RICHEDIT_NOTIFY,RN_FILE_CANCEL,pFileBar->OwnerData);
						}
					}
				}
				
				if ( pMMBar )
				{
					if ( odtType==ODT_SEND_VEDIO )
					{
						if ( url=="取消" )
						{
							GetParent()->PostMessage(WM_RICHEDIT_NOTIFY,RN_VEDIO_CANCEL,pMMBar->OwnerData);
						}
					}
					else
					if ( odtType==ODT_SEND_AUDIO )
					{
						if ( url=="取消" )
						{
							GetParent()->PostMessage(WM_RICHEDIT_NOTIFY,RN_AUDIO_CANCEL,pMMBar->OwnerData);
						}
					}
					else
					if ( odtType==ODT_RECV_VEDIO )
					{
						if ( url=="同意" )
						{
							GetParent()->PostMessage(WM_RICHEDIT_NOTIFY,RN_VEDIO_ACCEPT,pMMBar->OwnerData);
						}
						else
						if ( url=="拒绝" )
						{
							GetParent()->PostMessage(WM_RICHEDIT_NOTIFY,RN_VEDIO_REFUSE,pMMBar->OwnerData);
						}
					}
					else
					if ( odtType==ODT_RECV_AUDIO )
					{
						if ( url=="同意" )
						{
							GetParent()->PostMessage(WM_RICHEDIT_NOTIFY,RN_AUDIO_ACCEPT,pMMBar->OwnerData);
						}
						else
						if ( url=="拒绝" )
						{
							GetParent()->PostMessage(WM_RICHEDIT_NOTIFY,RN_AUDIO_REFUSE,pMMBar->OwnerData);
						}
					}

					// SetTimer(0x01,10,NULL);
				}
				else
				{
					if ( url=="设置摄像头" )
					{
						GetParent()->PostMessage(WM_RICHEDIT_NOTIFY,RN_VEDIO_SETTING,0);
					}
					if (url == "更新设置")
					{
						GetParent()->PostMessage(WM_RICHEDIT_NOTIFY,RN_FILETRANS_SETTING,0);
					}
					if( url == "打开文件" ||  url == "打开文件夹")
					{
						SetSel(0, link->chrg.cpMin);
						CString str = GetSelText();
						CString strBegin = "文件 \"";
						int nBegin = strBegin.GetLength();
						while ( str.Find("文件 \"") != -1)
						{
							str = str.Right(str.GetLength() - nBegin + 1);
						}

						nBegin = str.Find("\"");
						str = str.Right(str.GetLength() - nBegin - 1);
						int nEnd = str.Find("\" 接收完成。");
						str = str.Left(nEnd);

						CString str1,str2,strExt;
						ExtractFilePath(str,str1,str2,strExt);
						if(!FileExist(str))
						{
							CMessageBox::Model(NULL, "文件不存在！", NULL, MB_OK);
						}
						else
						{
							if(url == "打开文件")
								ShellExecute(NULL,"open",str, NULL,NULL,SW_SHOWNORMAL);
							else if(url == "打开文件夹")
								ShellExecute(NULL,"open",str1, NULL,NULL,SW_SHOWNORMAL);
						}
					}
				}
			}
		}


		*result = 0;
	}
}

bool CRichEditCtrlEx::FindFirstCustomEmotion(CString str,int & nGroup,int &iEmotion, int &iPos)
{
	int nStart = 1024;
	int nCGroup = 0;
	int nC = 0;
	int i,j;
	int type;
	CString strShortCut;

	iEmotion = -1;
	nCGroup = g_customEmotionDB.size();
	for ( i = 0; i < nCGroup; i++ )
	{
		CUSTOM_EMOTION_GROUP & group = g_customEmotionDB[i];
		nC = group.size();
		for(j = 0; j < nC; j++)
		{
			type = group[j].type;
			if(type == CUSTOM_EMOTION::SYS || type == CUSTOM_EMOTION::SYS_GIF)
				continue;
			strShortCut = group[j].szShortCut;
			strShortCut.Trim();
			if ( strShortCut != "" )
			{
				int n = str.Find(strShortCut,0);
				if ( n>-1 )
				{
					if ( n < nStart )
					{
						nStart = n;
						iEmotion = j;
						nGroup = i;
						iPos = n;
					}
				}
			}
		}
		
	}

	if ( iEmotion==-1 )
	{
		return false;
	}
	else
	{
		return true;
	}
}

bool CRichEditCtrlEx::FindFirstImage(CString str,CString &strImageFileName,int &iImageStart,int &iImageEnd)
{
	int n=str.Find("<image:",0);

	if ( n>-1 )
	{
		iImageStart=n;

		n=str.Find(">",iImageStart);

		if ( n>iImageStart )
		{
			iImageEnd=n+1;
			strImageFileName = str.Mid(iImageStart+7,iImageEnd-iImageStart-8);
			return true;
		}		
	}

	return false;
}

bool CRichEditCtrlEx::FindFirstInputSysEmo(CString str , int &iEmotion, int & nEmBeg,int & nEmEnd )
{
	int count = sizeof(emotions)/sizeof(LPCSTR);
	iEmotion = -1;
	nEmBeg = -1;
	nEmEnd = -1;

	bool bRet=false;
	int i,j;
	int nLen = str.GetLength();
	const char * pStr = (LPCTSTR)str;
	if(nLen < 2)
		return false;
	
	for(i = 0;i < nLen;i++)
	{
		for (j = 0; j < count; ++j) 
		{
			if(strcmp(pStr,emotions[j]) == 0)
			{
				nEmBeg = i;
				nEmEnd = i + strlen(emotions[j]);
				iEmotion = j;
				return true;
			}		
		}
		pStr++;
	}
	return bRet;
}

bool CRichEditCtrlEx::FindFirstEmotion(CString str , int &iEmotion, int & nEmBeg,int & nEmEnd )
{
	int count = sizeof(emotions)/sizeof(LPCSTR);

	iEmotion = -1;
	nEmBeg = -1;
	nEmEnd = -1;

	bool bRet=false;
	int i;
	int nBeg = str.Find("<image|",0);
	int nEnd;
	CString sEmotion;

	if(nBeg > -1)
	{
		nEnd = str.Find("/>",nBeg);
		if(nEnd <= nBeg)
		{
			return false;
		}
		sEmotion = str.Mid(nBeg + 7,nEnd - nBeg - 7);
	}
	for (i = 0; i < count; ++i) 
	{
		if(sEmotion == emotions[i])
		{
			nEmBeg = nBeg;
			nEmEnd = nEnd + 2;
			iEmotion = i;
			bRet = true;
			break;
		}		
	} 
	return bRet;
}

void CRichEditCtrlEx::OnAbortImageFile(const char * file_name)
{
	IRichEditOle *ole = GetIRichEditOle();
	int n = ole->GetObjectCount();

	REOBJECT reo;
	reo.cbStruct = sizeof(reo);

	for (int i = 0; i < n; ++i)
	{
		ole->GetObject(i, &reo, REO_GETOBJ_ALL_INTERFACES);

		if ( reo.dwUser==ODT_WAITICON)//表示当前OLE对象是IShowImage
		{
#ifndef _XY_PICTURE
			IShowImage *pImage = NULL;
			reo.poleobj->QueryInterface(IID_IShowImage,(void **)&pImage);	
#else
			IXPicture *pImage = NULL;
			reo.poleobj->QueryInterface(IID_IXPicture,(void **)&pImage);	
#endif



			//新的图片接口
			CComPtr<COleImage> spOleImage;
			reo.poleobj->QueryInterface(IID_IOleImage, (void **)&spOleImage);

			if(spOleImage)
			{
				IMAGE_FILE_STRUCT *pImage = (IMAGE_FILE_STRUCT *)(spOleImage->m_lOwnData);
				if ( strcmp(pImage->file_name,file_name)==0 )
				{
					CHARRANGE cr;
					GetSel(cr);
					SetSel(reo.cp,reo.cp+1);
					ReplaceSel("");
					
					CResBitmap bmp; 
					bmp.LoadBitmap(IDB_NO_IMAGE);
					InsertBitmap((HBITMAP)bmp.m_hObject);	

					SetSel(cr);

					ScrollToBottom();
				}
			}

			if ( pImage )
			{
				DWORD dwOwnData ;
#ifndef _XY_PICTURE
				dwOwnData = (DWORD)pImage->GetOwnData();
#else
				pImage->GetUserData((LONG*)&dwOwnData);
#endif
				if (dwOwnData!=0 )
				{
					IMAGE_FILE_STRUCT *pImage = (IMAGE_FILE_STRUCT *)dwOwnData;

					if ( strcmp(pImage->file_name,file_name)==0 )
					{
						CHARRANGE cr;
						GetSel(cr);
						SetSel(reo.cp,reo.cp+1);
						ReplaceSel("");
						//CString strPic=	theApp.GetModulePath()+"Image\\noImage.gif";
				
						CResBitmap bmp; 
						bmp.LoadBitmap(IDB_NO_IMAGE);
						InsertBitmap((HBITMAP)bmp.m_hObject);	

						SetSel(cr);

						ScrollToBottom();
					}
				}				
			}	
		}
	}
}

void CRichEditCtrlEx::OnRecvImageFile(const char * file_name )
{
	IRichEditOle *ole = GetIRichEditOle();
	int n = ole->GetObjectCount();
	REOBJECT reo;
	reo.cbStruct = sizeof(reo);

	for (int i = 0; i < n; ++i)
	{
		ole->GetObject(i, &reo, REO_GETOBJ_ALL_INTERFACES);

		if ( reo.dwUser == ODT_WAITICON )
		{
#ifndef _XY_PICTURE
			IShowImage *pImage = NULL;	
			reo.poleobj->QueryInterface(IID_IShowImage,(void **)&pImage);
#else
			IXPicture * pImage = NULL;
			reo.poleobj->QueryInterface(IID_IXPicture,(void **)&pImage);			
#endif

			//新的图片接口
			CComPtr<COleImage> spOleImage;
			reo.poleobj->QueryInterface(IID_IOleImage, (void **)&spOleImage);

			if(spOleImage)
			{
				IMAGE_FILE_STRUCT *pImage = (IMAGE_FILE_STRUCT *)(spOleImage->m_lOwnData);
				if ( strcmp(pImage->file_name,file_name)==0 )
				{
					CHARRANGE cr;
					GetSel(cr);
					SetSel(reo.cp,reo.cp+1);
					ReplaceSel("");
					CString strPic;
					strPic.Format("%s%s",g_LocalData.GetImageFileCachePath(),file_name);
					InsertImage((LPCTSTR)strPic);
					SetSel(cr);

					ScrollToBottom();
				}
				//是否需要删除?
				//delete spOleImage;
				//spOleImage = NULL;
				
			}

			if ( pImage )
			{
				DWORD dwOwnData;
#ifndef _XY_PICTURE
				dwOwnData = (LONG)pImage->GetOwnData();
#else
				pImage->GetUserData((LONG*)&dwOwnData);
#endif
				if (dwOwnData!=0 )
				{
					IMAGE_FILE_STRUCT *pImage = (IMAGE_FILE_STRUCT *)dwOwnData;

					if ( strcmp(pImage->file_name,file_name)==0 )
					{
						CHARRANGE cr;
						GetSel(cr);
						SetSel(reo.cp,reo.cp+1);
						ReplaceSel("");
						CString strPic;
						strPic.Format("%s%s",g_LocalData.GetImageFileCachePath(),file_name);

						//InsertImage(strPic,EMOTION_PIC_DATA());
						InsertImage((LPCTSTR)strPic);
						
						SetSel(cr);

						ScrollToBottom();
					}
				}				
			}	
		}
	}
}

HBITMAP CRichEditCtrlEx::GetStaticFace(int num)
{	
	if ( num>=0 && num<m_vecStaticFace.size() )
		return m_vecStaticFace[num];
	else
		return NULL;
}



//
// 插入图像
//
void CRichEditCtrlEx::InsertWaitIcon(LPCTSTR lpszPathName, IMAGE_FILE_STRUCT *pStruct)
{
	// 全部使用智能指针
	CComPtr<IStorage> spStorage;
	CComPtr<ILockBytes> spLockBytes;
	CComPtr<IOleClientSite> spOleClientSite;	
	COleImage* spOleImage;   //这不能用智能指针
	CComPtr<IOleObject> spOleObject;
	CLSID clsid;
	REOBJECT reobject;
	HRESULT hr = E_FAIL;

	do {

		// 创建LockBytes
		hr = CreateILockBytesOnHGlobal(NULL, TRUE, &spLockBytes);
		if (hr != S_OK) {
			break;
		}

		ASSERT(spLockBytes != NULL);

		// 创建Storage
		hr = StgCreateDocfileOnILockBytes(spLockBytes,
			STGM_SHARE_EXCLUSIVE | STGM_CREATE | STGM_READWRITE, 0, &spStorage);
		if (hr != S_OK) {
			break;
		}

		// 获取ClientSite
		hr = m_pRichEditOle->GetClientSite(&spOleClientSite);
		if (hr != S_OK) {
			break;
		}

		// 创建COleImage实例
		hr = CoCreateInstance(CLSID_OleImage, NULL, CLSCTX_INPROC, IID_IOleImage, (LPVOID*) &spOleImage);
		if (hr != S_OK) {
			break;
		}

		spOleImage->m_strImageName = (CString)lpszPathName;
		spOleImage->m_lOwnData = (long)pStruct;

		// 加载图像
		hr = spOleImage->LoadFromFile(_bstr_t(lpszPathName), this, 400);
		if (hr != S_OK) {
			break;
		}

		// 获取IOleObject接口
		hr = spOleImage->QueryInterface(IID_IOleObject, (LPVOID *) &spOleObject);
		if (hr != S_OK) {
			break;
		}
		spOleImage->Release();	/*QueryInterface会增加引用计数，调用后恢复一下*/	

		// 获取IOleObject的用户CLSID
		hr = spOleObject->GetUserClassID(&clsid);
		if (hr != S_OK) {
			break;
		}

		// 填充OLE对象属性
		ZeroMemory(&reobject, sizeof(REOBJECT));		
		reobject.cbStruct	= sizeof(REOBJECT);
		reobject.clsid		= clsid;
		reobject.cp			= REO_CP_SELECTION;
		reobject.dvaspect	= DVASPECT_CONTENT;
		reobject.dwFlags	= REO_BELOWBASELINE;
		reobject.poleobj	= spOleObject;
		reobject.polesite	= spOleClientSite;
		reobject.pstg		= spStorage;
		SIZEL sizel = {0};
		reobject.sizel = sizel;
		reobject.dwUser = ODT_WAITICON;

		// 插入OLE对象
		hr = m_pRichEditOle->InsertObject(&reobject);
		if (hr != S_OK) {
			break;
		}

		// 通知OLE容器保证OLE对象被正确引用
		hr = OleSetContainedObject(spOleObject, TRUE);

		if(spOleObject)
			spOleObject.Release();	

		if(spLockBytes)
			spLockBytes.Release();

		if(spOleClientSite)
			spOleClientSite.Release();

		/*if(m_pRichEditOle)
			m_pRichEditOle.Release();*/

		//加入vec,为了释放清空
		//m_vecOleImage.push_back(spOleImage);

	} while (0);

}

void CRichEditCtrlEx::InsertWaitIcon(IMAGE_FILE_STRUCT *pStruct)
{
	LPLOCKBYTES lpLockBytes = NULL;

	SCODE   sc ;
	HRESULT hr ;

	LPOLECLIENTSITE lpClientSite ;
#ifndef _XY_PICTURE
	IShowImagePtr	lpAnimator   ;
#else
	IXPicturePtr	lpAnimator   ;
#endif
	LPSTORAGE		lpStorage    ;
	LPOLEOBJECT		lpObject     ;

	sc = ::CreateILockBytesOnHGlobal(NULL, TRUE, &lpLockBytes);
	if (sc != S_OK)
		AfxThrowOleException(sc);
	ASSERT(lpLockBytes != NULL);

	sc = ::StgCreateDocfileOnILockBytes(lpLockBytes,
		STGM_SHARE_EXCLUSIVE|STGM_CREATE|STGM_READWRITE, 0, &lpStorage);
	if (sc != S_OK)
	{
		VERIFY(lpLockBytes->Release() == 0);
		lpLockBytes = NULL;
		AfxThrowOleException(sc);
	}
	ASSERT(lpStorage != NULL);

	IRichEditOle *lpRichEditOle = GetIRichEditOle();

	lpRichEditOle->GetClientSite(&lpClientSite);

	ASSERT(lpClientSite != NULL);

	CString strPicPath=GetModulePath()+"Image\\waitImage.gif";
	BSTR sPic;
	try
	{
#ifndef _XY_PICTURE
		hr = lpAnimator.CreateInstance(CLSID_ShowImage);
#else
		hr = lpAnimator.CreateInstance("XPictureMain.XPicture.1");
#endif
		if( FAILED(hr) )
				_com_issue_error(hr);
		m_arrImages.push_back(lpAnimator);
		sPic = strPicPath.AllocSysString();
#ifndef _XY_PICTURE
		hr = lpAnimator->LoadImageFile(sPic);
#else
		hr = lpAnimator->LoadImage(sPic);
#endif
		SysFreeString(sPic);
		if( FAILED(hr) )
			_com_issue_error(hr);

		hr = lpAnimator.QueryInterface(IID_IOleObject, (void**)&lpObject);
		if( FAILED(hr) )
			_com_issue_error(hr);
#ifndef _XY_PICTURE
		lpAnimator->PutOwnData((LONG*)pStruct);
#else
		lpAnimator->SetUserData((LONG)pStruct);
#endif
		OleSetContainedObject(lpObject, TRUE);

		//2 insert in 2 richedit, you need a struct of REOBJECT
		REOBJECT reobject;
		ZeroMemory(&reobject, sizeof(REOBJECT));

		reobject.cbStruct = sizeof(REOBJECT);
		CLSID clsid;
		sc = lpObject->GetUserClassID(&clsid);
		if (sc != S_OK)
			AfxThrowOleException(sc);

		reobject.clsid	= clsid;
		reobject.cp		= REO_CP_SELECTION;
		reobject.dvaspect	= DVASPECT_CONTENT;
		reobject.dwFlags	= REO_BELOWBASELINE | REO_INPLACEACTIVE ;
		reobject.dwUser		= ODT_WAITICON;
		reobject.poleobj	= lpObject;
		reobject.polesite	= lpClientSite;
		reobject.pstg		= lpStorage;
	
		SIZEL sizel={0,0};
		reobject.sizel = sizel;

		lpRichEditOle->InsertObject(&reobject);


		if(lpObject)
			lpObject->Release();	

		if(lpLockBytes)
			lpLockBytes->Release();

		if(lpClientSite)
			lpClientSite->Release();

		if(lpRichEditOle)
			lpRichEditOle->Release();

	}
	catch(...)
	{
	}
}

void CRichEditCtrlEx::InsertGifEmotion(int i)
{
	CString   strGif = g_LocalData.GetGifFacePath(i);

	if ( FileExist(strGif)==FALSE )
	{
		return ;
	}
	InsertImage((LPCTSTR)strGif);
	return;

	LPLOCKBYTES lpLockBytes = NULL;

	SCODE   sc ;
	HRESULT hr ;

	LPOLECLIENTSITE lpClientSite ;
#ifndef _XY_PICTURE
	IShowImagePtr	lpAnimator   ;
#else
	IXPicturePtr	lpAnimator   ;
#endif
	LPSTORAGE		lpStorage    ;
	LPOLEOBJECT		lpObject     ;

	sc = ::CreateILockBytesOnHGlobal(NULL, TRUE, &lpLockBytes);
	if (sc != S_OK)
		AfxThrowOleException(sc);
	ASSERT(lpLockBytes != NULL);

	sc = ::StgCreateDocfileOnILockBytes(lpLockBytes,
		STGM_SHARE_EXCLUSIVE|STGM_CREATE|STGM_READWRITE, 0, &lpStorage);
	if (sc != S_OK)
	{
		VERIFY(lpLockBytes->Release() == 0);
		lpLockBytes = NULL;
		AfxThrowOleException(sc);
	}
	ASSERT(lpStorage != NULL);

	IRichEditOle *lpRichEditOle = GetIRichEditOle();

	lpRichEditOle->GetClientSite(&lpClientSite);

	ASSERT(lpClientSite != NULL);

	try
	{
#ifndef _XY_PICTURE
		hr = lpAnimator.CreateInstance(CLSID_ShowImage);
#else
		hr = lpAnimator.CreateInstance("XPictureMain.XPicture.1");
#endif
		if( FAILED(hr) )
				_com_issue_error(hr);
		//m_arrImageData.push_back(lpAnimator);

		BSTR bstr = strGif.AllocSysString();
#ifndef _XY_PICTURE
		hr = lpAnimator->LoadImageFile(bstr);
#else
        hr = lpAnimator->LoadImage(bstr);
#endif
		::SysFreeString(bstr);
	
		if( FAILED(hr) )
			_com_issue_error(hr);

		hr = lpAnimator.QueryInterface(IID_IOleObject, (void**)&lpObject);
		if( FAILED(hr) )
			_com_issue_error(hr);
#ifndef _XY_PICTURE
		lpAnimator->PutOwnData((LONG*)m_nArrayGifId[i]);
#else
		lpAnimator->SetUserData((LONG)m_nArrayGifId[i]);
#endif
		OleSetContainedObject(lpObject, TRUE);

		//2 insert in 2 richedit, you need a struct of REOBJECT
		REOBJECT reobject;
		ZeroMemory(&reobject, sizeof(REOBJECT));

		reobject.cbStruct = sizeof(REOBJECT);
		CLSID clsid;
		sc = lpObject->GetUserClassID(&clsid);
		if (sc != S_OK)
			AfxThrowOleException(sc);

		reobject.clsid	= clsid;
		reobject.cp		= REO_CP_SELECTION;
		reobject.dvaspect	= DVASPECT_CONTENT;
		reobject.dwFlags	= REO_BELOWBASELINE | REO_INPLACEACTIVE ;
		reobject.dwUser		= ODT_GIF;
		reobject.poleobj	= lpObject;
		reobject.polesite	= lpClientSite;
		reobject.pstg		= lpStorage;
	
		SIZEL sizel={0,0};
		reobject.sizel = sizel;

		lpRichEditOle->InsertObject(&reobject);


		if(lpObject)
			lpObject->Release();	

		if(lpLockBytes)
			lpLockBytes->Release();

		if(lpClientSite)
			lpClientSite->Release();

		if(lpRichEditOle)
			lpRichEditOle->Release();

		if(lpStorage)
			lpStorage->Release();


	}
	catch(...)
	{
	}
}

void CRichEditCtrlEx::InsertImage(CString strPicPath,EMOTION_PIC_DATA& data)
{
	try
	{
	if(data.nType == EPT_WEB)
	{
		//data.sData.Replace("pub.feiniu.com/mini139","downloads.feiniu.com/iecim");
		//data.sData.Replace("fs3.feiniu.com/mini","downloads.feiniu.com/iecim");
	}
	
	SCODE			sc ;
	HRESULT			hr ;
	LPLOCKBYTES		lpLockBytes = NULL;
	LPOLECLIENTSITE lpClientSite ;
#ifndef _XY_PICTURE
	IShowImagePtr	lpAnimator   ;
#else
	IXPicturePtr	lpAnimator   ;
#endif
	LPSTORAGE		lpStorage    ;
	LPOLEOBJECT		lpObject     ;

	sc = ::CreateILockBytesOnHGlobal(NULL, TRUE, &lpLockBytes);
	if (sc != S_OK)
		AfxThrowOleException(sc);
	ASSERT(lpLockBytes != NULL);

	sc = ::StgCreateDocfileOnILockBytes(lpLockBytes,
		STGM_SHARE_EXCLUSIVE|STGM_CREATE|STGM_READWRITE, 0, &lpStorage);
	if (sc != S_OK)
	{
		VERIFY(lpLockBytes->Release() == 0);
		lpLockBytes = NULL;
		AfxThrowOleException(sc);
	}
	ASSERT(lpStorage != NULL);
	IRichEditOle *lpRichEditOle = GetIRichEditOle();
	lpRichEditOle->GetClientSite(&lpClientSite);
	ASSERT(lpClientSite != NULL);

#ifndef _XY_PICTURE
		hr = lpAnimator.CreateInstance(CLSID_ShowImage);
#else
		hr = lpAnimator.CreateInstance("XPictureMain.XPicture.1");
#endif
		if( FAILED(hr) )
				_com_issue_error(hr);
		m_arrImages.push_back(lpAnimator);
		BSTR bstr = strPicPath.AllocSysString();
#ifndef _XY_PICTURE
		hr = lpAnimator->LoadImageFile(bstr);
#else
		hr = lpAnimator->LoadImage(bstr);
#endif
		SysFreeString(bstr);

		int nW = ::GetSystemMetrics(SM_CXSCREEN);
		int nH = ::GetSystemMetrics(SM_CYSCREEN);
		/*
		自动缩放后性能变差了
		if(lpAnimator->Width > nW)
			lpAnimator->Width = nW;
		if(lpAnimator->Height > nH)
			lpAnimator->Height = nH;*/
		//hr = S_OK;//lpAnimator->LoadImageFile(bstr);

		
		if( FAILED(hr) )
			_com_issue_error(hr);
		// 获取被嵌入的对象
		hr = lpAnimator.QueryInterface(IID_IOleObject, (void**)&lpObject);
		lpObject->SetClientSite(lpClientSite);
		if( FAILED(hr) )
			_com_issue_error(hr);
		if(data.nType == EPT_WEB)
		{
			// 保存该图片的额外信息（url）
			m_arrPicData.push_back(data);
#ifndef _XY_PICTURE
			lpAnimator->PutOwnData((LONG*)(m_arrPicData.size()-1));
#else
			lpAnimator->SetUserData((LONG)(m_arrPicData.size()-1));
#endif
		}
		else
		{
#ifndef _XY_PICTURE
			lpAnimator->PutOwnData((LONG*)m_nArrayGifId[0]);
#else
			lpAnimator->SetUserData(m_nArrayGifId[0]);
#endif
		}	
		// 通知一个对象被嵌入容器中
		hr = OleSetContainedObject(lpObject, TRUE);

		//2 insert in 2 richedit, you need a struct of REOBJECT
		REOBJECT reobject;
		ZeroMemory(&reobject, sizeof(REOBJECT));
		reobject.cbStruct = sizeof(REOBJECT);
		CLSID clsid;
		// 获取被嵌入对象的类id
		sc = lpObject->GetUserClassID(&clsid);
		if (sc != S_OK)
			AfxThrowOleException(sc);

		reobject.clsid	= clsid;
		// 能被选择
		reobject.cp		= REO_CP_SELECTION;
		// 内容
		reobject.dvaspect	= DVASPECT_CONTENT;
		// 和文字同一水平线
		reobject.dwFlags	= REO_BELOWBASELINE| REO_INPLACEACTIVE ;
		if(data.nType == EPT_WEB)
			reobject.dwUser	= ODT_WEB_EMOTION;	// web定义图片
		else
			reobject.dwUser = ODT_GIF;
		// 嵌入对象
		reobject.poleobj	= lpObject;
		// 
		reobject.polesite	= lpClientSite;
		// 存储数据
		reobject.pstg		= lpStorage;
	
		SIZEL sizel={0,0};
		reobject.sizel = sizel;
		lpRichEditOle->InsertObject(&reobject);

		if(lpObject)
			lpObject->Release();	
		if(lpLockBytes)
			lpLockBytes->Release();
		if(lpClientSite)
			lpClientSite->Release();
		if(lpRichEditOle)
			lpRichEditOle->Release();
		if(lpStorage)
			lpStorage->Release();
		/*if(lpAnimator)
			lpAnimator->Release();*/

	}
	catch(...)
	{
		TRACE("insert image插入图片出现异常!\n");
	}
}

void CRichEditCtrlEx::InsertEmotion(int i)
{
	CImageDataObject::InsertBitmap(GetIRichEditOle(),
		GetStaticFace( i), i);
}

void CRichEditCtrlEx::InsertLine()
{
	CResBitmap bmpLine;
	bmpLine.LoadBitmap(IDB_MSG_LINE);
	InsertBitmap((HBITMAP)bmpLine.GetSafeHandle());
}

void CRichEditCtrlEx::InsertBitmap(HBITMAP hBitmap)
{
	try
	{
		CImageDataObject::InsertBitmap(GetIRichEditOle(),
			hBitmap,ODT_BITMAP);
	}
	catch(...)
	{
		TRACE("\ninsertbitmap error\n");
	}
}

//设置段落,粗点作为段落的开头
void CRichEditCtrlEx::SetParagraphBulleted()
{
/*	PARAFORMAT paraformat ;
	GetParaFormat(paraformat);

	if ( (paraformat.dwMask & PFM_NUMBERING) && (paraformat.wNumbering == PFN_BULLET) )
	{
		paraformat.wNumbering = 0;
		paraformat.dxOffset = 0;
		paraformat.dxStartIndent = 10;
		paraformat.dwMask = PFM_NUMBERING | PFM_STARTINDENT | PFM_OFFSET;
	}
	else
	{
		paraformat.wNumbering = PFN_BULLET;
		paraformat.dwMask = PFM_NUMBERING;
		if (paraformat.dxOffset == 0)
		{
			paraformat.dxOffset = 0;
			paraformat.dwMask = PFM_NUMBERING | PFM_STARTINDENT | PFM_OFFSET;
		}
	}
	
	SetParaFormat(paraformat); */
}

bool CRichEditCtrlEx::ParagraphIsBulleted()
{
	/*PARAFORMAT pf ;
	GetParaFormat(pf);

	if (pf.wNumbering == PFN_BULLET)
		return true;
	else
		return false;*/
	return false;
}

bool CRichEditCtrlEx::ReplaceFileRecvImage(LPCTSTR sPathName,int nFileSessionId)
{
	/*GUID guid;
	if(!FileNameToGUID(sPathName, guid)) return FALSE;

	IRichEditOle* pReO = GetIRichEditOle();
	int nCount = pReO->GetObjectCount();

	REOBJECT robj;
	robj.cbStruct = sizeof(REOBJECT);
	for(int i = 0; i < nCount; i++)
	{
		pReO->GetObject(i, &robj, REO_GETOBJ_POLEOBJ | REO_GETOBJ_PSTG);
		IMAGESTRUCT is;
		if(m_aryImage.GetAt(robj.dwUser, is)) 
		{		
			if(0 == memcmp(&is.uniName, &guid, sizeof(GUID)))
			{
				
				SetSel(robj.cp, robj.cp + 1);
				Clear();				
				InsertImageObject(sPathName);
			}
		}
	}*/
	return false;
}

bool CRichEditCtrlEx::IsGifEmotion(CString str, int & id)
{
	//id = atoi((LPCSTR)str);
	id = GetGifIndex(str) + 1;
	if ( id > 0 && id < 60 )
		return true;
	else
		return false;
}

void CRichEditCtrlEx::InsertText(CString text,const XT_FONT& xf)
{
	int iEmotion;
	int iImageStart,iImageEnd;
	int nEmBeg;
	int nEmEnd;
	CString strImageFileName;
	bool bSysEmotion;
	bool bFindImage;
	CHARRANGE cr;
	GetSel(cr);

	CHARRANGE crBefore;
	SetSel(-1,-1);
	GetSel(crBefore);
	SetSel(cr);

	while ( 1 )
	{
		bSysEmotion = FindFirstEmotion(text,iEmotion,nEmBeg,nEmEnd) ;
		bFindImage   = FindFirstImage(text,strImageFileName,iImageStart,iImageEnd);

		// 判断最先出现的是系统表情还是自定义表情
		if ( bSysEmotion && bFindImage )
		{
			if ( nEmBeg < iImageStart )
			{
				bFindImage   = false;
			}
			else
			{
				bSysEmotion = false;
			}
		}

		if ( bFindImage )
		{
			ReplaceSel(text.Left(iImageStart));

			CString strFullImageFilePathName ;

			CString strTemp = strImageFileName;
			strTemp.MakeLower();

			int id;
			bool bGifFace=false;

			if ( strTemp == "noimage.gif" )
			{
				strFullImageFilePathName = GetModulePath()+"Image\\"+strImageFileName;
			}
			else if ( IsGifEmotion(strTemp,id) )
			{
				bGifFace = true;
				strFullImageFilePathName = g_LocalData.GetGifFacePath(id);
			}
			else
			{
				strFullImageFilePathName = g_LocalData.GetImageFileCachePath()+strImageFileName;
			}

			if ( _access(strFullImageFilePathName,0)!=-1 )
			{
				if ( bGifFace )
					this->InsertGifEmotion(id);
				else
					InsertImage(strFullImageFilePathName,EMOTION_PIC_DATA());
			}
			else
			{
				CResBitmap bmp; 
				bmp.LoadBitmap(IDB_NO_IMAGE);
				InsertBitmap((HBITMAP)bmp.m_hObject);		
			}

			text = text.Mid(iImageEnd);
		}
		// 如果是系统表情
		else if ( bSysEmotion )
		{
			ReplaceSel(text.Left(nEmBeg));
			InsertEmotion(iEmotion);
			text = text.Mid(nEmEnd);
		}
		else
		{
			break;
		}		
	}

	ReplaceSel(text);

	SetSel(-1,-1);
	CHARRANGE crEnd;
	GetSel(crEnd);

	cr.cpMax = cr.cpMax+crEnd.cpMax-crBefore.cpMax;
	SetSel(cr);
	this->SetSelectionCharFormat(XFont2CharFormat(xf));
	
	SetSel(cr.cpMax,cr.cpMax);
}

CString CRichEditCtrlEx::Utf82Char(CString strUtf8)
{
	char *pStr = strUtf8.GetBuffer(strUtf8.GetLength()); //取得str对象的原始字符串
	int nBufferSize = MultiByteToWideChar(CP_UTF8, 0, pStr, -1, NULL, 0); //取得所需缓存的多少
	wchar_t *pBuffer = (wchar_t*)malloc(nBufferSize * sizeof(wchar_t));//申请缓存空间
	MultiByteToWideChar(CP_UTF8, 0, pStr, -1 , pBuffer, nBufferSize*sizeof(wchar_t));//转码
	char* pStr2 = (LPSTR)(LPCSTR)strUtf8;
	WideCharToMultiByte(CP_ACP,0,pBuffer,-1,pStr2,512,NULL,NULL);
	free(pBuffer);
	return pStr2;
}
void CRichEditCtrlEx::InsertText(CString text, bool bWaitImageFile, CStringList *pslWaitImageFile)
{
	int iEmotion;
	int iImageStart,iImageEnd;
	int nEmotionBeg;
	int nEmotionEnd;

	CString strImageFileName;

	int nLinkStoreStart;
	int nLinkStoreEnd;
	int nLinkItemStart;
	int nLinkItemEnd;
	bool bFindEmotion, bFindImage, bIsLinkStore, bIsLinkItem;

	CHARRANGE cr;
	GetSel(cr);

	while ( 1 )
	{
		bFindEmotion = false;
		bFindImage = false;
		bIsLinkStore = false;
		bIsLinkItem = false;

		nLinkStoreStart = text.Find(STORE_URL);
		if(nLinkStoreStart != -1)
			bIsLinkStore = true;

		nLinkItemStart = text.Find(ITEM_URL);
		if(nLinkItemStart != -1)
			bIsLinkItem = true;

		bFindEmotion = FindFirstEmotion(text,iEmotion,nEmotionBeg,nEmotionEnd);
		bFindImage   = FindFirstImage(text,strImageFileName,iImageStart,iImageEnd);

		if(!bFindEmotion && !bFindImage && !bIsLinkStore && !bIsLinkItem)
			break;

		if(nLinkStoreStart != -1)
		{
			nLinkStoreEnd = text.Find(".html") + 5;
			//CString strUrl = text.Mid(nLinkStoreStart, nLinkStoreEnd - nLinkStoreStart);
		}

		if(nLinkItemStart != -1)
		{
			CString strUrlSize = ITEM_URL;
			CString strItemRight = text.Right(text.GetLength() - strUrlSize.GetLength() - nLinkItemStart);
			CString strItem = "";
			while(strItemRight.GetAt(0) >= 0x30 && strItemRight.GetAt(0) <= 0x39)
			{
				strItem += strItemRight.GetAt(0);
				strItemRight.Delete(0);
			}
			nLinkItemEnd = nLinkItemStart + strUrlSize.GetLength() + strItem.GetLength();
		}
		


		if(max(nLinkStoreStart, nLinkItemStart) < max(nEmotionBeg, iImageStart) && (bIsLinkItem || bIsLinkStore))
		{
			bFindImage = false;
			bFindEmotion = false;
			if(nLinkStoreStart < nLinkItemStart && bIsLinkStore)
				bIsLinkItem = false;
			else if(nLinkStoreStart > nLinkItemStart && bIsLinkItem)
				bIsLinkStore = false;
		}
		else if(max(nLinkStoreStart, nLinkItemStart) > max(nEmotionBeg, iImageStart) && (bFindImage || bFindEmotion))
		{
			bIsLinkItem = false;
			bIsLinkStore = false;
			if(nEmotionBeg < iImageStart && bFindEmotion)
				bFindImage = false;
			else if(nEmotionBeg > iImageStart && bFindImage)
				bFindEmotion = false;
		}

		if(bIsLinkStore)
		{
			ReplaceSel(text.Left(nLinkStoreEnd));
			CString strUrlStore = text.Mid(nLinkStoreStart, nLinkStoreEnd - nLinkStoreStart);

			text = text.Mid(nLinkStoreEnd);

			CString strStoreFind = STORE_URL;
			int nFindStore = strUrlStore.Find(strStoreFind);
			if( nFindStore != -1)
			{
				int nFindLastDot = strUrlStore.ReverseFind('.');
				if(nFindLastDot != -1)
				{
					CString strStoreID = strUrlStore.Mid(nFindStore + strStoreFind.GetLength(), nFindLastDot - strStoreFind.GetLength() - nFindStore);
					if(m_bIsShowLinkInformation)
					{
						CString strHttpConnectionStore = WEBIM_URL;
						CString strHttpFileStore = WEBIM_LINKINFO_STORE + strStoreID;
						CString strStoreResult = GetStoreInformation(strHttpConnectionStore, strHttpFileStore);

						Json::Reader jReader;
						Json::Value jvSellerInfo;

						std::string strStore = LPCSTR(strStoreResult);
						jReader.parse(strStore,jvSellerInfo);
						std::string storeSuccess = jvSellerInfo["success"].asString();	

						if(storeSuccess == "true")
						{
							std::string strStoreData = LPCSTR("data");
							Json::Value valueData = jvSellerInfo.get(strStoreData, jvSellerInfo);

							std::string strStoreInfo = LPCSTR("storeInfo");
							Json::Value valueInfo = valueData.get(strStoreInfo, valueData);

							std::string storeLogoUrl = valueInfo["storeLogoUrl"].asString();
							std::string storeName = valueInfo["storeName"].asString();
							std::string storeDesc = valueInfo["storeDesc"].asString();

							CString strStoreLogoUrl = storeLogoUrl.c_str();
							CString strStoreName = storeName.c_str();
							CString strStoreDesc = storeDesc.c_str();

							string strGbkStoreDesc = UTF8_Convert(strStoreDesc, FALSE);
							strStoreDesc = strGbkStoreDesc.c_str();
							string strGbkStoreName = UTF8_Convert(strStoreName, FALSE);
							strStoreName = strGbkStoreName.c_str();

							//去掉字体样式
							strStoreDesc.Replace("&nbsp;", "");
							strStoreDesc.Replace("<br />", "");
							strStoreDesc.Replace("&gt;", "");
							strStoreDesc.Replace("&lt;", "");
							strStoreDesc.Replace("\r", "");
							strStoreDesc.Replace("\n", "");
							strStoreDesc.Replace("</span>", "");
							strStoreDesc.Replace("</em>", "");
							strStoreDesc.Replace("</u>", "");
							strStoreDesc.Replace("</strong>", "");

							while( strStoreDesc.Find("<span") != -1)
							{
								CString str = strStoreDesc.Right(strStoreDesc.GetLength() - strStoreDesc.Find("<span"));
								if(str.Left(str.Find(">")) != -1)
								{
									str = str.Left(str.Find(">") + 1);
									strStoreDesc.Replace(str, "");
								}
								else
									strStoreDesc.Delete(strStoreDesc.Find("<span"));
							}
							while( strStoreDesc.Find("<em") != -1)
							{
								CString str = strStoreDesc.Right(strStoreDesc.GetLength() - strStoreDesc.Find("<em"));
								if(str.Left(str.Find(">")) != -1)
								{
									str = str.Left(str.Find(">") + 1);
									strStoreDesc.Replace(str, "");
								}
								else
									strStoreDesc.Delete(strStoreDesc.Find("<em"));
							}
							while( strStoreDesc.Find("<u") != -1)
							{
								CString str = strStoreDesc.Right(strStoreDesc.GetLength() - strStoreDesc.Find("<u"));
								if(str.Left(str.Find(">")) != -1)
								{
									str = str.Left(str.Find(">") + 1);
									strStoreDesc.Replace(str, "");
								}
								else
									strStoreDesc.Delete(strStoreDesc.Find("<u"));
							}
							while( strStoreDesc.Find("<strong") != -1)
							{
								CString str = strStoreDesc.Right(strStoreDesc.GetLength() - strStoreDesc.Find("<strong"));
								if(str.Left(str.Find(">")) != -1)
								{
									str = str.Left(str.Find(">") + 1);
									strStoreDesc.Replace(str, "");
								}
								else
									strStoreDesc.Delete(strStoreDesc.Find("<strong"));
							}
							while (strStoreDesc.Find("/>") != -1)
							{
								CString str = strStoreDesc.Left(strStoreDesc.Find("/>") + 2);
								if(str.ReverseFind('<') != -1)
								{
									str = str.Right(str.GetLength() - str.ReverseFind('<'));
									strStoreDesc.Replace(str, "");
								}
								else
								{
									strStoreDesc.Delete(strStoreDesc.Find("/>"), 2);
								}
							}

							ReplaceSel("\r");
							InsertLinkInfo(strUrlStore, strStoreLogoUrl, strStoreName, strStoreDesc);
							if(text != "")
								ReplaceSel("\r");
						}
					}
				}
			}
		}
		else if(bIsLinkItem)
		{
			ReplaceSel(text.Left(nLinkItemEnd));
			CString strUrlItem = text.Mid(nLinkItemStart, nLinkItemEnd - nLinkItemStart);

			text = text.Mid(nLinkItemEnd);

			CString strItemFind = ITEM_URL;
			int nFindItem = strUrlItem.Find(strItemFind);
			if( nFindItem != -1)
			{
				CString strItemID = strUrlItem.Right(strUrlItem.GetLength() - strItemFind.GetLength());
				if(m_bIsShowLinkInformation)
				{
					CString strHttpConnectionStore = WEBIM_URL;
					CString strHttpFileStore = WEBIM_LINKINFO_ITEM + strItemID;
					CString strItemResult = GetStoreInformation(strHttpConnectionStore, strHttpFileStore);

					Json::Reader jReader;
					Json::Value jvSellerInfo;

					std::string strItem = LPCSTR(strItemResult);
					jReader.parse(strItem,jvSellerInfo);
					std::string storeSuccess = jvSellerInfo["success"].asString();	

					if(storeSuccess == "true")
					{
						std::string strItemData = LPCSTR("data");
						Json::Value valueData = jvSellerInfo.get(strItemData, jvSellerInfo);

						std::string strProductInfo = LPCSTR("goodsAsking");
						Json::Value valueInfo = valueData.get(strProductInfo, valueData);

						double dPrice = valueInfo["salePrice"].asDouble();
						std::string itemTitle = valueInfo["title"].asString();
						std::string itemPic = valueInfo["pic01"].asString();

						CString strItemPrice = "";
						strItemPrice.Format("价格：￥%lg元", dPrice);
						CString strItemTitle = itemTitle.c_str();
						CString strItemPic = itemPic.c_str();

						int nFindStyle = strItemTitle.Find("</");
						if(nFindStyle != -1)
						{
							CString strDescLeft = strItemTitle.Left(nFindStyle);
							CString strDescRight = strDescLeft.Right(strDescLeft.GetLength() - strDescLeft.ReverseFind('>') - 1);
							strItemTitle = strDescRight;
						}
						strItemTitle = Utf82Char(strItemTitle);

						ReplaceSel("\r");
						InsertLinkInfo(strUrlItem, strItemPic, strItemTitle, strItemPrice);
						if(text != "")
							ReplaceSel("\r");
					}
				}
			}
		}
		
		// 将自定义表情替换为插入图片
		else if ( bFindImage )
		{
			ReplaceSel(text.Left(iImageStart));

			CString strFullImageFilePathName ;

			CString strTemp = strImageFileName;
			strTemp.MakeLower();

			int id = -1;
			bool bGifFace=false;

			if( strTemp == "noimage" )//如果为noimage则显示叉叉图片
			{
				CResBitmap bmp; 
				bmp.LoadBitmap(IDB_NO_IMAGE);
				InsertBitmap((HBITMAP)bmp.m_hObject);	
			}
			else
			{
				if ( strTemp == "noimage.gif" )
				{
					strFullImageFilePathName = GetModulePath()+"Image\\"+strImageFileName;
				}
				else if ( IsGifEmotion(strTemp,id) )
				{
					bGifFace = true;
					strFullImageFilePathName = g_LocalData.GetGifFacePath(id);
				}
				else
				{
					strFullImageFilePathName = g_LocalData.GetImageFileCachePath()+strImageFileName;
				}
				if ( _access(strFullImageFilePathName,0)!=-1 )
				{
					if ( bGifFace )
						this->InsertGifEmotion(id);
					else
					{
						//InsertImage(strFullImageFilePathName,EMOTION_PIC_DATA());
						InsertImage((LPCTSTR)strFullImageFilePathName);
					}
				}
				else
				{
					if ( bWaitImageFile )
					{
						IMAGE_FILE_STRUCT *pStruct = new IMAGE_FILE_STRUCT();
						sprintf(pStruct->file_name,strImageFileName);
						string strFile = pStruct->file_name;
							
						// 判断是否为网络图片
						if(strFile[0] != '{')
						{
							pStruct->image_type = 2;
							CString strHttpImg = pStruct->file_name;
							if (strHttpImg.Find("http://") == -1)
							{
								strHttpImg = GetPicRandomUrl() + strHttpImg;
							}
							sprintf(pStruct->file_name,strHttpImg);
						}
						/*if(strFile.find("http") != string::npos)
						{
						pStruct->image_type = 2;
						}*/
						//InsertWaitIcon(pStruct);	
						InsertWaitIcon(g_LocalData.GetProgramPath() + "\\Emotion\\waitImage.gif", pStruct);
						m_vecImageFileStruct.push_back(pStruct);
						if(pStruct->image_type == 2)
						{
							// 下载图片
							g_webFileProcess.GetWebFile((DWORD)pStruct, pStruct->file_name, GetSafeHwnd());
						}

						if ( pslWaitImageFile )
						{
							pslWaitImageFile->AddTail(strImageFileName);
						}
					}
					else
					{
						IMAGE_FILE_STRUCT *pStruct = new IMAGE_FILE_STRUCT();
						sprintf(pStruct->file_name,strImageFileName);
						string strFile = pStruct->file_name;
						
						if(strFile[0] != '{')
						//if(strFile.find("http") != string::npos)
						{
							pStruct->image_type = 2;
							CString strHttpImg = pStruct->file_name;
							if (strHttpImg.Find("http://") == -1)
							{
								strHttpImg = GetPicRandomUrl() + strHttpImg;
							}
							sprintf(pStruct->file_name,strHttpImg);

							InsertWaitIcon(g_LocalData.GetProgramPath() + "\\Emotion\\waitImage.gif", pStruct);
							m_vecImageFileStruct.push_back(pStruct);
							// 下载图片
							g_webFileProcess.GetWebFile((DWORD)pStruct, pStruct->file_name, GetSafeHwnd());

							if ( pslWaitImageFile )
							{
								pslWaitImageFile->AddTail(strImageFileName);
							}
						}
						else
						{
							delete pStruct;
							pStruct = NULL;

							CResBitmap bmp; 
							bmp.LoadBitmap(IDB_NO_IMAGE);
							InsertBitmap((HBITMAP)bmp.m_hObject);		
						}
					}
				}
			}

			text = text.Mid(iImageEnd);
		}
		// 将系统表情替换为插入表情
		else if ( bFindEmotion )
		{
			ReplaceSel(text.Left(nEmotionBeg));
			InsertEmotion(iEmotion);
			text = text.Mid(nEmotionEnd);
		}
		else
		{
			break;
		}		
	}
	ReplaceSel(text);
}

size_t len_wchar2ansi(LPCTSTR pszText, size_t pnwcharLen)
{
	if (pnwcharLen == 0) 
	{
		return 0;
	}
	
	char* szText = 0;
	wchar_t* wszText = new wchar_t[pnwcharLen + 1];
	int nszLen;

	MultiByteToWideChar(CP_ACP, 0, pszText, -1, wszText, pnwcharLen);
	wszText[pnwcharLen] = 0;

	nszLen = WideCharToMultiByte(CP_ACP, 0, wszText, -1, szText, 0, NULL, NULL);
	szText = new char[nszLen + 1];
	WideCharToMultiByte(CP_ACP, 0, wszText, -1, szText, nszLen + 1, NULL, NULL);
	
	size_t nLen = strlen(szText);

	delete[] szText;
	delete[] wszText;

	return nLen;
}

CString CRichEditCtrlEx::GetText(CString &text,IMAGE_FILE_LIST * ifl)
{
	CString str;
	GetWindowText(str);

	char szOld[3] ={ 13, 10, 0 };
	char szNew[2] ={ 13, 0 };

	str.Replace(szOld,szNew);

	IRichEditOle *ole = GetIRichEditOle();

	if(ole == NULL)
		return "";

	int n = ole->GetObjectCount();
	REOBJECT reo;
	reo.cbStruct = sizeof(reo);

	text.Empty();
	
	CString str2;
	str2 = str;
	int postext = 0, posObjects = 0;

	if ( ifl )
		ifl->clear();

	USES_CONVERSION;
	for (int i = 0; i < n; ++i)
	{
		// 获得RichEdit中的所有对象
		ole->GetObject(i, &reo, REO_GETOBJ_ALL_INTERFACES);
		text += str.Mid(postext, len_wchar2ansi(str2, reo.cp - posObjects));

		if ( (reo.dwUser >= 0) && (reo.dwUser < sizeof(emotions)/sizeof(LPCTSTR)) )
		{
			text += "<image|";
			text += emotions[reo.dwUser];
			text += "/>";
		}
		else
		{
#ifndef _XY_PICTURE
			IShowImage *pImage = NULL;	
			reo.poleobj->QueryInterface(IID_IShowImage,(void **)&pImage);
#else
			IXPicture * pImage = NULL;
			reo.poleobj->QueryInterface(IID_IXPicture,(void **)&pImage);
#endif
			CComPtr<COleImage> spOleImage;
			reo.poleobj->QueryInterface(IID_IOleImage, (void **)&spOleImage);

			CString strImageFile; 
			if(spOleImage)
			{
				strImageFile =	spOleImage->m_strImageName;
			}

			if( pImage)
			{
#ifndef _XY_PICTURE
				strImageFile = W2A(pImage->GetImage());
#else
				BSTR bs;
				pImage->GetFileName(&bs);
				strImageFile = W2A(bs);
#endif
			}

			if ( !strImageFile.IsEmpty() )
			{
				CString strTemp;
				
				// 自定义图片，系统gif图片等
				if (reo.dwUser == ODT_GIF )
				{
					long nOwnData = 0;
					if(pImage)
					{
#ifndef _XY_PICTURE
					nOwnData = (LONG)pImage->GetOwnData();
#else
					pImage->GetUserData(&nOwnData);
#endif
					}

					if ( nOwnData==0 )
					{
						IMAGE_FILE_STRUCT image;

						CString strPath,strFileName,strExt;
						ExtractFilePath(strImageFile,strPath,strFileName,strExt);

						image.file_session_id   = m_dwFileSessionId  ;
						image.file_size         = GetFileSize(strImageFile);
						ASSERT(image.file_size > 0);
						sprintf(image.file_name, "%s%s", strFileName, strExt ); 

						if ( ifl )
						{
							bool find=false;

							IMAGE_FILE_LIST::iterator it;
							for ( it=ifl->begin(); it!=ifl->end(); it++ )
							{
								if (strcmp( (*it).file_name, image.file_name)==0 )
								{
									find=true;
								}
							}

							if ( find==false )
							{
								int nGifId = 0;
								if(!IsGifEmotion(image.file_name, nGifId))	// 排除系统表情
								{	
									m_dwFileSessionId++;
									ifl->push_back(image);
								}
							}
						}

						strTemp.Format("<image:%s%s>",strFileName,strExt);
					}
					else
					{
						strTemp.Format("<image:%d>",nOwnData);
					}									
				}
				// web图片
				else if(reo.dwUser == ODT_WEB_EMOTION )
				{
					long nOwnData = 0;
					if(pImage)
					{
#ifndef _XY_PICTURE
					nOwnData = (LONG)pImage->GetOwnData();
#else
					pImage->GetUserData((LONG*)&nOwnData);
#endif
					}
					if(nOwnData>=0 && nOwnData<m_arrPicData.size())
					{
						strTemp = "[img]"+m_arrPicData[nOwnData].sData+"[/img]";
					}
				}
				
				text += strTemp;
			}		

		}

		posObjects = reo.cp+1;
		postext = len_wchar2ansi(str, reo.cp)+1;

		str2=str.Mid(postext);
	}

	text += str.Mid(postext);

	return text;
}

CString CRichEditCtrlEx::GetSelTextEx(BOOL& bIsOleLinkInfo)
{
	CString str = GetSelText();

	CString text ;//return value

	CHARRANGE cr;
	this->GetSel(cr);

	IRichEditOle *ole = GetIRichEditOle();
	int n = ole->GetObjectCount();
	REOBJECT reo;
	reo.cbStruct = sizeof(reo);

	CString str2;
	str2 = str;
	int postext = 0, posObjects = 0;


	for (int i = 0; i < n; ++i)
	{
		ole->GetObject(i, &reo, REO_GETOBJ_ALL_INTERFACES);
	//	text += str.Mid(postext,reo.cp-posObjects);

		if ( reo.cp<cr.cpMin || reo.cp>cr.cpMax )
		{
			continue;
		}

		if(reo.dwUser == ODT_LINK_INFO)
		{
			bIsOleLinkInfo = TRUE;
			return "";
		}
		text += str.Mid(postext, len_wchar2ansi(str2, reo.cp - posObjects - cr.cpMin));

		USES_CONVERSION;
		if ( reo.dwUser>=0 && reo.dwUser<sizeof(emotions)/sizeof(LPCTSTR) )
		{
			text += emotions[reo.dwUser];
		}
		else
		{
#ifndef _XY_PICTURE
			IShowImage *pImage = NULL;		
			reo.poleobj->QueryInterface(IID_IShowImage,(void **)&pImage);
#else
			IXPicture * pImage = NULL;
			reo.poleobj->QueryInterface(IID_IXPicture,(void **)&pImage);
#endif
			
			CComPtr<COleImage> spOleImage;
			reo.poleobj->QueryInterface(IID_IOleImage, (void **)&spOleImage);


			CString strImageFile; 
			if(spOleImage)
			{
				strImageFile =	spOleImage->m_strImageName;
			}
			
			if( pImage)
			{
#ifndef _XY_PICTURE
				strImageFile = W2A(pImage->GetImage());
#else
				BSTR bs;
				pImage->GetFileName(&bs);
				strImageFile = W2A(bs);
#endif
			}

			if ( !strImageFile.IsEmpty() ) 
			{
				CString strTemp;

				if (reo.dwUser == ODT_GIF )
				{
					IMAGE_FILE_STRUCT image;

					CString strPath,strFileName,strExt;
					ExtractFilePath(strImageFile,strPath,strFileName,strExt);

					image.file_session_id   = m_dwFileSessionId  ;
					image.file_size         = GetFileSize(strImageFile);

					sprintf(image.file_name, "%s%s", strFileName, strExt );      ;

					strTemp.Format("<image:%s%s>",strFileName,strExt);	
					//strTemp.Format("<image:%s>",strImageFile);
				}
			
				text += strTemp;
			}		

		}

		posObjects = reo.cp+1-cr.cpMin;
		postext = len_wchar2ansi(str, reo.cp-cr.cpMin)+1;

		str2=str.Mid(postext);
		break;
	}

	text += str.Mid(postext);

	return text;
}

XT_FONT CRichEditCtrlEx::GetXFont()
{
	return *m_pXFont;
}

void CRichEditCtrlEx::SetXFont(const XT_FONT &xf)
{
	CHARFORMAT2 cf=XFont2CharFormat(xf);

	CHARRANGE crPrev;
	GetSel(crPrev);
	SetSel(0,-1);
	SetSelectionCharFormat(cf);
	SetSel(crPrev);

	if ( m_pXFont!=NULL )
		delete m_pXFont;

	m_pXFont = new XT_FONT;
	memcpy(m_pXFont,&xf,sizeof(XT_FONT));

	SetDefaultCharFormat(cf);
}

CHARFORMAT2 CRichEditCtrlEx::XFont2CharFormat(const XT_FONT &xf)
{
	CHARFORMAT2 cf;
	memset(&cf,0,sizeof(cf));
	cf.cbSize = sizeof(cf);

	cf.dwMask = CFM_BOLD | CFM_ITALIC | CFM_UNDERLINE | 
				CFM_FACE | CFM_SIZE | CFM_COLOR ;
				//CFM_STRIKEOUT | CFM_PROTECTED; ;

	int nSize = strlen(xf.fontName);
	for ( int i=0; i< nSize; i++ )
	{
		if ( xf.fontName[i]<0 || xf.fontName[i]>127 )
		{
			cf.dwMask |= CFM_CHARSET;
			cf.bCharSet = GB2312_CHARSET;
			break;
		}
	}
	
	cf.dwEffects |= ( xf.flags & XTF_BOLD ? CFE_BOLD : 0 );
	cf.dwEffects |= ( xf.flags & XTF_ITALIC ? CFE_ITALIC : 0 );
	cf.dwEffects |= ( xf.flags & XTF_UNDERLINE ? CFE_UNDERLINE : 0 );
//	cf.dwEffects |= CFE_AUTOCOLOR;
	
	sprintf(cf.szFaceName,xf.fontName);
	cf.crTextColor = xf.fontColor;
	cf.yHeight = xf.fontSize * 20;

	return cf;
}

XT_FONT CRichEditCtrlEx::CharFormat2XCFont(const CHARFORMAT2 &cf)
{
	XT_FONT xf;
	memset(&xf,0,sizeof(xf));

	xf.flags |= ( cf.dwEffects & CFE_BOLD ? XTF_BOLD : 0 );
	xf.flags |= ( cf.dwEffects & CFE_ITALIC ? XTF_ITALIC : 0 );
	xf.flags |= ( cf.dwEffects & CFE_UNDERLINE ? XTF_UNDERLINE : 0 );

	xf.fontSize = cf.yHeight / 20;
	sprintf(xf.fontName,cf.szFaceName);
	xf.fontColor = cf.crTextColor;

	return xf;
}

void CRichEditCtrlEx::SetXFont(const XT_FONT &xf, long nStart, long nEnd)
{
	CHARRANGE crPrev;
	GetSel(crPrev);
	SetSel(nStart,nEnd);
	SetSelectionCharFormat(XFont2CharFormat(xf));
	SetSel(crPrev);
}


void CRichEditCtrlEx::SetText(LPCTSTR text)
{
	SetSel(0, -1);
	Clear();
	InsertText(text);
}

void CRichEditCtrlEx::ScrollToBottom()
{
	if ( m_hWnd )
	{
		CRect rc;
		GetClientRect(&rc);

		rc.left=rc.right-20;

		this->InvalidateRect(&rc);

		PostMessage(WM_VSCROLL,SB_LINEUP,0);
		PostMessage(WM_VSCROLL,SB_BOTTOM,0);
	}
}

void CRichEditCtrlEx::GetMsgFormat(XT_FONT &mft)
{
	CHARFORMAT cf;
	ZeroMemory(&cf, sizeof(cf));
	cf.cbSize = sizeof(cf);
	cf.dwMask = CFM_BOLD | CFM_ITALIC | CFM_UNDERLINE | CFM_FACE | CFM_SIZE | CFM_COLOR;
	GetSelectionCharFormat(cf);

	strcpy( mft.fontName , cf.szFaceName ) ;
	mft.fontSize = (cf.yHeight + 5) / 20;
	mft.fontColor = cf.crTextColor;

	BYTE flags = 0;
	if (cf.dwEffects & CFE_BOLD)
		flags |= XTF_BOLD;
	if (cf.dwEffects & CFE_ITALIC)
		flags |= XTF_ITALIC;
	if (cf.dwEffects & CFE_UNDERLINE)
		flags |= XTF_UNDERLINE;
	mft.flags = flags;

}

//设置文本块缩进
void CRichEditCtrlEx::SetStartIndent(int nIndent)
{
	PARAFORMAT2 pf;

	GetParaFormat(pf);

	pf.dwMask = 0;
	pf.dwMask |= PFM_ALIGNMENT;
	pf.wAlignment = PFA_LEFT;
	pf.dwMask |= PFM_RIGHTINDENT;
	pf.dwMask |= PFM_STARTINDENT;
	pf.dwMask |= PFM_OFFSET;

	int nRight=0;
	int nFirst=0;
	int nLeft=nIndent;

	pf.dxRightIndent = nRight;
	pf.dxOffset = -nFirst;
	pf.dxStartIndent = nLeft + nFirst;

	this->SetParaFormat(pf);
}

void CRichEditCtrlEx::ClearMsg()
{
	SetSel( 0 , -1 ) ;
	InsertText("_");//如果有超级链接的话,直接清空会有问题,鼠标还是手形状

	SetSel( 0 , -1 ) ;
	ReplaceSel("");	
	// 清空图片数据
	size_t i,c;
	c = m_arrImages.size();
	for(i = 0; i < c; i++)
	{
#ifndef _XY_PICTURE
		m_arrImages[i].Release();
#else
		m_arrImages[i]->DeleteImage();
#endif
	}
	m_arrImages.clear();
	
	/*for(i=0; i<m_vecOleImage.size();i++)
	{

	COleImage* spOleImage = m_vecOleImage[i];
	if(spOleImage)
	delete spOleImage;
	spOleImage = NULL;
	}
	m_vecOleImage.clear();*/

	/*

	IRichEditOle *ole = GetIRichEditOle();
	int n = ole->GetObjectCount();
	REOBJECT reo;
	reo.cbStruct = sizeof(reo);
	for ( i = 0; i < n; ++i)
	{
		// 获得RichEdit中的所有对象
		ole->GetObject(i, &reo, REO_GETOBJ_POLEOBJ);
		if ( reo.dwUser>=0 && reo.dwUser<sizeof(emotions)/sizeof(LPCTSTR) )
		{
			;
		}
		else
		{
			IShowImage *pImage = NULL;			
			reo.poleobj->QueryInterface(IID_IShowImage,(void **)&pImage);
			if ( pImage )
			{
		
			}								
		}

	}

	*/
}

CHARRANGE CRichEditCtrlEx::InsertLink(CString strTextLink)
{
	CHARFORMAT cfPrev;
	cfPrev.dwMask=0xFFFFFFFF;
	GetDefaultCharFormat(cfPrev);

	SetSel(-1, -1);

	InsertText(" ");

	CHARRANGE cr;
	GetSel(cr);


	InsertText(strTextLink);
	CHARFORMAT2 cf;
	memset(&cf, 0, sizeof(cf));
	cf.cbSize = sizeof(cf);
	cf.dwMask = CFM_LINK | CFM_PROTECTED;
	cf.dwEffects = CFE_LINK | CFE_PROTECTED;

	int ansi_len = strTextLink.GetLength();

	WCHAR wszText[255];
	MultiByteToWideChar(CP_ACP,0,(LPCSTR)strTextLink,-1, wszText,255);

	int w_len =
	wcslen(wszText);

	cr.cpMin = cr.cpMax ;
	cr.cpMax = cr.cpMin + w_len ;

	InsertText(" ");

	SetSel(cr.cpMin,cr.cpMax);
	SetSelectionCharFormat(cf);

	SetSel(cr.cpMax+1, -1);
	cf.dwMask=0xFFFFFFFF;
	SetSelectionCharFormat(cfPrev);

	return cr ;
}

void CRichEditCtrlEx::ClearFriendClusterLink()
{
	m_vecFriendClusterLink.clear();
}

void CRichEditCtrlEx::InsertFriendClusterLink(FRIEND_CLUSTER_LINK friendClusterLinkInfo)
{
	CString strTextLink = "";
	if(friendClusterLinkInfo.nType == 1)//好友
	{
		FRIEND_INFO* pFriend = friendClusterLinkInfo.friendInfo;
		strTextLink.Format("【%s】", friendClusterLinkInfo.szName);
	}
	else if(friendClusterLinkInfo.nType == 2)//群
	{
		XT_CLUSTER_INFO* pCluster = friendClusterLinkInfo.clusterInfo;
		strTextLink.Format("【%s】", friendClusterLinkInfo.szName);
	}

	CHARFORMAT cfPrev;
	cfPrev.dwMask=0xFFFFFFFF;
	GetDefaultCharFormat(cfPrev);

	SetSel(-1, -1);

	InsertText(" ");

	CHARRANGE cr;
	GetSel(cr);
	InsertText(strTextLink);
	CHARFORMAT2 cf;
	memset(&cf, 0, sizeof(cf));
	cf.cbSize = sizeof(cf);
	cf.dwMask = CFM_LINK | CFM_PROTECTED;
	cf.dwEffects = CFE_LINK | CFE_PROTECTED;

	int ansi_len = strTextLink.GetLength();

	WCHAR wszText[255];
	MultiByteToWideChar(CP_ACP,0,(LPCSTR)strTextLink,-1, wszText,255);

	int w_len =
		wcslen(wszText);

	cr.cpMin = cr.cpMax ;
	cr.cpMax = cr.cpMin + w_len ;

	friendClusterLinkInfo.range = cr;
	InsertText(" ");

	SetSel(cr.cpMin,cr.cpMax);
	SetSelectionCharFormat(cf);

	SetSel(cr.cpMax+1, -1);
	cf.dwMask=0xFFFFFFFF;
	SetSelectionCharFormat(cfPrev);
	m_vecFriendClusterLink.push_back(friendClusterLinkInfo);
}

void CRichEditCtrlEx::OnRButtonDown(UINT nFlags, CPoint point)
{
	int nIdx = PointToUserLink(point);
	if(nIdx >= 0)
	{
		CWnd * pParent;
		pParent = GetParent();
		if(pParent)
		{
			pParent->SendMessage(WM_USERLINK_RBCLICK,(WPARAM)m_arrName[nIdx].id,(LPARAM)(LPCTSTR)m_arrName[nIdx].sName);
		}
		CRichEditCtrl::OnRButtonDown(nFlags, point);
		return;
	}

	CHARRANGE cr;
	this->GetSel(cr);

	SetFocus();

	if ( cr.cpMax==cr.cpMin )
	{
		SendMessage(WM_LBUTTONDOWN,nFlags,MAKELPARAM(point.x,point.y));
		SendMessage(WM_LBUTTONUP,nFlags,MAKELPARAM(point.x,point.y));
	}

	CMenu popupMen;
	popupMen.CreatePopupMenu();


	popupMen.AppendMenu(0, ID_XT_MENU_CUT,  "剪切(&X)" ) ;
    popupMen.AppendMenu(0, ID_XT_MENU_COPY, "复制(&C)" );
	popupMen.AppendMenu(0, ID_XT_MENU_PASTE,"粘贴(&V)" );
	popupMen.AppendMenu(0, ID_XT_SEL_ALL,"全部选择(&A)" );
	popupMen.AppendMenu(0, ID_XT_CLEAR,"清屏" );
	if ( (GetOptions() & ECO_READONLY ) == false)
	{
		if(m_bZYF)
			popupMen.AppendMenu(0,ID_XT_CANCEL_ZYF,"取消转义符(&Z)" );
		else
			popupMen.AppendMenu(0,ID_XT_CANCEL_ZYF,"恢复转义符(&Z)" );
	};

	//int status =  CanPaste() ? MF_ENABLED:MF_GRAYED;
	int status = (GetStyle() & ES_READONLY) ? MF_GRAYED:MF_ENABLED;
	popupMen.EnableMenuItem(ID_XT_MENU_PASTE, status|MF_BYCOMMAND);
	
	BOOL bHaveSel = !(cr.cpMin == cr.cpMax );
	status = bHaveSel ? MF_ENABLED:MF_GRAYED;
	popupMen.EnableMenuItem(ID_XT_MENU_COPY, status|MF_BYCOMMAND);

	status = (CanPaste() && bHaveSel) ? MF_ENABLED:MF_GRAYED;
	popupMen.EnableMenuItem(ID_XT_MENU_CUT, status|MF_BYCOMMAND);

	CString strMsg;
	GetWindowText(strMsg);
	status = (!strMsg.IsEmpty()) ? MF_ENABLED:MF_GRAYED;
	popupMen.EnableMenuItem(ID_XT_SEL_ALL, status|MF_BYCOMMAND);
	popupMen.EnableMenuItem(ID_XT_CLEAR, status|MF_BYCOMMAND);
	
	WORD seltype = GetSelectionType();
	if((seltype & SEL_OBJECT)  && !(seltype & SEL_TEXT) && !(seltype & SEL_MULTIOBJECT))
	{
		IRichEditOle *ole = GetIRichEditOle();

		REOBJECT reo;
		reo.cbStruct = sizeof(reo);

		ole->GetObject(REO_IOB_SELECTION,&reo,REO_GETOBJ_ALL_INTERFACES);
		
		if ( reo.dwUser==ODT_GIF )
		{
			/*暂时不要这两个，prd没有要求，免得有bug*/
			//popupMen.AppendMenu(0, ID_XT_MENU_SAVEAS, "图片另存为(&S)");
			//popupMen.AppendMenu(MF_SEPARATOR,0,"");
			//popupMen.AppendMenu(0, ID_XT_ADD2EMOTION, "添加到自定义表情" );
		}
	}

	POINT pt;
	::GetCursorPos(&pt);
	popupMen.TrackPopupMenu(0, pt.x, pt.y, this);

	CRichEditCtrl::OnRButtonDown(nFlags, point);
}

void CRichEditCtrlEx::SetHintText(CString &strTitle)
{
	int n=strTitle.Find("<image:",0);
	if ( n>-1 )
	{
		int iImageStart=n+7;////跳过<image：

		n=strTitle.Find(">",iImageStart);

		if ( n>iImageStart )
		{
			strTitle = strTitle.Mid(iImageStart,n-iImageStart);
		}
	}
}

void CRichEditCtrlEx::OnLButtonDblClk(UINT nFlags, CPoint point)
{
 	CString strPicPath = g_LocalData.GetImageFileCachePath();
	BOOL bIsOleLinkInfo = FALSE;
 	CString strPic= GetSelTextEx(bIsOleLinkInfo);
	if(bIsOleLinkInfo)
		return;
	if(!strPic.IsEmpty())
	{
		SetHintText(strPic);
		strPicPath += strPic;
		//ShellExecute(NULL, _T("open"),_T("explorer.exe"),strPicPath, NULL, SW_SHOW);
		//ShellExecute( NULL, _T("open"), strPicPath, NULL, NULL, SW_SHOWNORMAL );
		if(PathFileExists(strPicPath))//判断是否为截屏图片并且图片是否存在
		{
			//if(m_pPictureCtrl == NULL)
			//{
			//	m_pPictureCtrl = new CPictureCtrl(this);
			//	m_pPictureCtrl->Create(CPictureCtrl::IDD, this);
			//}
			//m_pPictureCtrl->SetFile(strPicPath);
			//m_pPictureCtrl->SetPos();
			//m_pPictureCtrl->ShowWindow(SW_SHOW);
			//m_pPictureCtrl->SetFocus();
			std::string strFilePath = strPicPath;
			CDuiPicture::Show(strFilePath);
		}
	}
 	CRichEditCtrl::OnLButtonDblClk(nFlags, point);
}
	
void CRichEditCtrlEx::OnCopy(void)
{
	BOOL bIsOleLinkInfo = FALSE;
	CString text= GetSelTextEx(bIsOleLinkInfo);
	if(bIsOleLinkInfo)
		return;

	if(OpenClipboard())
	{
		if (g_bOs2K)
		{
			HGLOBAL clipbuffer;
			char * buffer;
			char * pstr = (char *)(LPCSTR)text;

			EmptyClipboard();
			clipbuffer = GlobalAlloc(GMEM_DDESHARE, (strlen(pstr)+1)*sizeof(WCHAR));
			buffer = (char *)GlobalLock(clipbuffer);

			strcpy(buffer,pstr);

			GlobalUnlock(clipbuffer);
			SetClipboardData(CF_TEXT,clipbuffer);
			CloseClipboard();
		}
		else
		{
			USES_CONVERSION;

			HGLOBAL clipbuffer;
			WCHAR * buffer;
			WCHAR * pstr = A2W((char *)(LPCSTR)text);

			EmptyClipboard();
			clipbuffer = GlobalAlloc(GMEM_DDESHARE, (wcslen(pstr)+1)*sizeof(WCHAR));
			buffer = (WCHAR *)GlobalLock(clipbuffer);

			wcscpy(buffer,pstr);

			GlobalUnlock(clipbuffer);
			SetClipboardData(CF_UNICODETEXT,clipbuffer);
			CloseClipboard();
		}
	}


	//Copy();
}

void CRichEditCtrlEx::OnCut(void)
{
	BOOL bIsOleLinkInfo = FALSE;
	CString text= GetSelTextEx(bIsOleLinkInfo);
	if(bIsOleLinkInfo)
		return;

	if(OpenClipboard() && !(GetStyle()&ES_READONLY))   //只读的不能剪切
	{
		if (g_bOs2K )
		{
			HGLOBAL clipbuffer;
			char * buffer;
			char * pstr = (char *)(LPCSTR)text;

			EmptyClipboard();
			clipbuffer = GlobalAlloc(GMEM_DDESHARE, (strlen(pstr)+1)*sizeof(WCHAR));
			buffer = (char *)GlobalLock(clipbuffer);

			strcpy(buffer,pstr);

			GlobalUnlock(clipbuffer);
			SetClipboardData(CF_TEXT,clipbuffer);
			CloseClipboard();
		}
		else
		{
			USES_CONVERSION;

			HGLOBAL clipbuffer;
			WCHAR * buffer;
			WCHAR * pstr = A2W((char *)(LPCSTR)text);

			EmptyClipboard();
			clipbuffer = GlobalAlloc(GMEM_DDESHARE, (wcslen(pstr)+1)*sizeof(WCHAR));
			buffer = (WCHAR *)GlobalLock(clipbuffer);

			wcscpy(buffer,pstr);

			GlobalUnlock(clipbuffer);
			SetClipboardData(CF_UNICODETEXT,clipbuffer);
			CloseClipboard();
		}
	}

	if(CanPaste())
		this->ReplaceSel("");
	//Cut();
}

void CRichEditCtrlEx::OnPaste(void)
{
	if (CanPaste())
	{
		//Paste();
		if(m_nEditType == EDIT_TYPE_SENDMSG)
		{
			if ( g_bOs2K )
			{
				char * buffer;
				if(OpenClipboard())
				{
					buffer = (char *)GetClipboardData(CF_TEXT);
					this->InsertText(buffer,false);
					//this->SetXFont(g_ImFont);
					CloseClipboard();
				}
			}
			else
			{
				WCHAR * buffer;
				if(OpenClipboard())
				{
					buffer = (WCHAR *)GetClipboardData(CF_UNICODETEXT);
					USES_CONVERSION;
					this->InsertText(W2A(buffer),false);
					//this->SetXFont(g_ImFont);
					CloseClipboard();
				}
			}
		}
		else
		{
			if ( g_bOs2K )
			{
				char * buffer;
				if(OpenClipboard())
				{
					buffer = (char *)GetClipboardData(CF_TEXT);
					this->InsertText(buffer,false);
					XT_FONT xTFont = *m_pXFont;
					this->SetXFont(xTFont);
					CloseClipboard();
				}
			}
			else
			{
				WCHAR * buffer;
				if(OpenClipboard())
				{
					buffer = (WCHAR *)GetClipboardData(CF_UNICODETEXT);
					USES_CONVERSION;

					this->InsertText(W2A(buffer),false);

					XT_FONT xTFont = *m_pXFont;
					this->SetXFont(xTFont);

					CloseClipboard();
				}
			}
		}
	
	}
	else
	{
		if(OpenClipboard())
		{
			CBitmap* pBitmap = (CBitmap*)GetClipboardData(CF_BITMAP);
			if(pBitmap != NULL)
			{
				CImage image;
				image.Attach((HBITMAP)pBitmap);
				CString str;
				str.Format("%stemp%d.jpg",g_LocalData.GetImageFileCachePath(),time(0));
				image.Save(str);
				CString strCacheFileName;
				g_LocalData.SaveImageFileToCache(str,strCacheFileName);
				strCacheFileName = g_LocalData.GetImageFileCachePath() + strCacheFileName;
				image.Save(strCacheFileName);
				image.Detach();

				InsertImage((LPCTSTR)strCacheFileName);
				//删除临时图片
				DeleteFile(str);
			}
			CloseClipboard();
		}
	}
	
	if ( (GetStyle() & ES_READONLY)==FALSE )
	{
		if(OpenClipboard())
		{
			HDROP hDrop=(HDROP)GetClipboardData(CF_HDROP);
	
			if ( hDrop )
			{
				char   szFilePathName[_MAX_PATH+1] = {0};
				UINT  nNumOfFiles = DragQueryFile(hDrop, 0xFFFFFFFF, NULL, 0); //得到文件个数

				for (UINT nIndex=0 ; nIndex< nNumOfFiles; ++nIndex)
				{
					DragQueryFile(hDrop, nIndex, szFilePathName, _MAX_PATH);  //得到文件名
					GetParent()->SendMessage(WM_USER_DROPFILE,(WPARAM)szFilePathName,0);
				}
			}

			CloseClipboard();
		}

		//GetParent()->SendMessage(WM_USER_DROPFILE,(WPARAM)szFilePathName,0);
	}
	//CHARFORMAT2 cf;
	//this->GetDefaultCharFormat(cf);
	//this->SetDefaultCharFormat(cf);
}

void CRichEditCtrlEx::OnAdd2Emotion(void)
{
	IRichEditOle *ole = GetIRichEditOle();

	REOBJECT reo;
	reo.cbStruct = sizeof(reo);

	ole->GetObject(REO_IOB_SELECTION,&reo,REO_GETOBJ_ALL_INTERFACES);
	
	if ( reo.dwUser==ODT_GIF )
	{
#ifndef _XY_PICTURE
		IShowImage *pImage = NULL;
		reo.poleobj->QueryInterface(IID_IShowImage,(void **)&pImage);	
#else
		IXPicture * pImage = NULL;
		reo.poleobj->QueryInterface(IID_IXPicture,(void **)&pImage);	
#endif
		

		USES_CONVERSION;
		CString strImageFile;
		
#ifndef _XY_PICTURE
		strImageFile = W2A(pImage->GetImage());
#else
		BSTR bs;
		pImage->GetFileName(&bs);
		strImageFile = W2A(bs);
#endif
		
		CString strName,strPath,strExt;
		ExtractFilePath(strImageFile,strPath,strName,strExt);
        
		CString strFile =strName+strExt;

		bool bFind = false;
		int i,j;
		int nG,nC;
		nG = g_customEmotionDB.size();
		for ( i = 0; i< nG; i++ )
		{
			nC = g_customEmotionDB[i].size();
			for(j = 0;j < nC; j++)
			{
				if ( strFile == g_customEmotionDB[i][j].szFile )
				{
					bFind = true;
					break;
				}
			}
			if(bFind)
				break;
		}
		if ( bFind )
		{
			CMessageBox::Prompt("该图片已经在您的表情列表中!");
		}
		else
		{
			CAddEmotionDlg dlg(0,strImageFile,this);
			dlg.DoModal();
		}
	}

}

void CRichEditCtrlEx::OnSaveAs()
{
	IRichEditOle *ole = GetIRichEditOle();

	REOBJECT reo;
	reo.cbStruct = sizeof(reo);

	ole->GetObject(REO_IOB_SELECTION,&reo,REO_GETOBJ_ALL_INTERFACES);
	
	if ( reo.dwUser==ODT_GIF )
	{
#ifndef _XY_PICTURE
		IShowImage *pImage = NULL;
		reo.poleobj->QueryInterface(IID_IShowImage,(void **)&pImage);	
#else
		IXPicture *pImage = NULL;
		reo.poleobj->QueryInterface(IID_IXPicture,(void **)&pImage);	
#endif
		
		USES_CONVERSION;
		CString strFile ;
		
#ifndef _XY_PICTURE
		strFile = W2A(pImage->GetImage());
#else
		BSTR bs;
		pImage->GetFileName(&bs);
		strFile = W2A(bs);
#endif
		

		CString strPath,strName,strExt;

		ExtractFilePath(strFile,strPath,strName,strExt);
		CFileDialog dlg(FALSE,(LPCSTR)strExt,strName+strExt,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,"图片文件(*.bmp;*jpg;*jpeg;*.gif)|*.bmp;*jpg;*jpeg;*.gif||",this);

		if ( dlg.DoModal()==IDOK )
		{
			CopyFile(strFile,dlg.GetPathName(),FALSE);
		}
	}

}


BOOL CRichEditCtrlEx::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	if(m_pPictureCtrl != NULL && m_pPictureCtrl->IsWindowVisible())
	{
		//if(GetFocus()->GetSafeHwnd() != m_pPictureCtrl->GetSafeHwnd())
		//{
		//	m_pPictureCtrl->SetFocus();
		//	return TRUE;
		//}
	}
	else
	{
		//Invalidate( ) ;
		::PostMessage( GetParent()->m_hWnd , WM_HISTORY_REDRAW , 0 , 0 ) ;
	}
	//WM_MOUSEWHEEL发送到父窗口，用于快捷短语菜单上下滑动
	::PostMessage( GetParent()->m_hWnd , WM_MOUSEWHEEL , MAKELONG(nFlags, zDelta) , MAKELONG(pt.x, pt.y) ) ;
	return CRichEditCtrl::OnMouseWheel(nFlags, zDelta, pt);
}

void CRichEditCtrlEx::SetEmotionImageList(CImageList * pImageEmotionList)
{
	m_pImageEmotionList = pImageEmotionList;

	CClientDC dc(NULL);

	for ( int i=0; i<pImageEmotionList->GetImageCount(); i++ )
	{
		CBitmap dist, face;

		IMAGEINFO ii;
		pImageEmotionList->GetImageInfo(i, &ii);

		int nWidth = ii.rcImage.right - ii.rcImage.left;
		int nHeight = ii.rcImage.bottom - ii.rcImage.top;

		dist.CreateCompatibleBitmap(&dc, nWidth, nHeight);
		face.CreateCompatibleBitmap(&dc, nWidth, nHeight);

		CDC distDC, faceDC;
		distDC.CreateCompatibleDC(&dc);
		faceDC.CreateCompatibleDC(&dc);
		CBitmap* pOldDist = distDC.SelectObject(&dist);
		CBitmap* pOldFace = faceDC.SelectObject(&face);
		
		COLORREF oldColor = SetBackgroundColor(FALSE, 0);
		distDC.FillSolidRect(0, 0, nWidth, nHeight, oldColor);
		SetBackgroundColor(FALSE, oldColor);

		pImageEmotionList->Draw(&faceDC, i, CPoint(0, 0), ILD_NORMAL);

		TransparentBlt(distDC.m_hDC,0,0,nWidth,nHeight,
					faceDC.m_hDC,0,0,nWidth,nHeight,
					RGB(255,255,255));

		distDC.SelectObject(pOldDist);
		faceDC.SelectObject(pOldFace);

		m_vecStaticFace.push_back((HBITMAP)dist.Detach());
	}
}

void CRichEditCtrlEx::PreSubclassWindow()
{
	CRichEditCtrl::PreSubclassWindow();

	//SetTimer(TIMER_CAPTURE_USERNAME,TIME_CAPTURE_USERNAME,NULL);
	SetAutoURLDetect(TRUE);
	SetEventMask(GetEventMask() | ENM_LINK | ENM_CHANGE);

	// 下面这个可选，去掉RichEdit的双字体设置  
    // 因为说实话，我不知道怎么分别设置中英文字体，如果哪位同学知道请告诉  
    // 我一下  
    //DWORD fontStyle = SendMessage(EM_GETLANGOPTIONS);  
    //fontStyle &= ~IMF_DUALFONT;  
    //SendMessage(EM_SETLANGOPTIONS, 0, fontStyle);  
  
    // 修改为背景透明，避免刷新动画时闪烁，我们自己绘制白色背景  
    //ModifyStyleEx(0, WS_EX_TRANSPARENT);  
  
    // 这个接口指针使用非常频繁，存下来提高效率  
    m_pRichEditOle = GetIRichEditOle();  
  
    // 启动定时器，用于更新动画，MIN_FRM_DELAY为一个宏，值为1，也可定义为其他不常用的值  
    SetTimer(FRM_TIMER_ID, MIN_FRM_DELAY, NULL);  
}



void CRichEditCtrlEx::ProcessEmotion()
{
	long nStart, nEnd;

 	GetSel(nStart, nEnd);
	
	if ( m_pXFont )
	{
		SetXFont(*m_pXFont,nEnd-1,nEnd);
	}
	CString sText;
	sText.GetBufferSetLength(255);

	int iPrePos1=-1, iPrePos2=-1;
	int iPreN1, iPreN2;
	int iEmotion1, iPos1;
	int nGroup2,iEmotion2, iPos2;
	bool bSysEmotion, bCusEmotion;
	int nSysEnd;
	//CHARRANGE crNew;

	// 快捷键最多为10个字节长度
	for ( int n = 1; n < 10; n++ )
	{
		long nTemp = nEnd - n;

		if ( nTemp<0 )
			return ;
		GetTextRange(nTemp,nEnd,sText);

		bSysEmotion = FindFirstInputSysEmo(sText,iEmotion1,iPos1,nSysEnd);
		bCusEmotion = FindFirstCustomEmotion(sText,nGroup2,iEmotion2,iPos2);

		if ( iPrePos1!=-1 && iPos1==(iPrePos1+n-iPreN1) )
		{
			bSysEmotion = false;
		}

		if ( iPrePos2!=-1 && iPos2==(iPrePos2+n-iPreN2) )
		{
			bCusEmotion = false;
		}

		iPrePos1=iPos1;
		iPrePos2=iPos2;
		iPreN1 = iPreN2 = n;

		if ( bCusEmotion )
		{
			bool bPeerOnline=true;

			GetParent()->SendMessage(WM_QUERY_PEERONLINE,0,(LPARAM)&bPeerOnline);

			if ( bPeerOnline == false )
			{
				bCusEmotion = false;
				//static bool bInfoNextTime=true;
				if ( m_bPromptOffline )
				{
					if ( m_nPreEmotion !=iEmotion2 )
					{
						CCustomEmotionInfoDlg dlg( g_customEmotionDB[nGroup2][iEmotion2].szShortCut, GetDesktopWindow() );

						if ( dlg.DoModal()==IDOK )
						{
							m_bPromptOffline = dlg.IsInfoNextTime();
						}
					}

					m_nPreEmotion = iEmotion2;
				}
			}
		}

		if ( bSysEmotion && bCusEmotion )
		{
			if ( iPos1 < iPos2 )
			{
				bSysEmotion = false;
			}
			else
			{
				bCusEmotion = false;
			}
		}

		if (bSysEmotion) 
		{
			SetSel(nTemp,nEnd);
			ReplaceSel("");	
			InsertEmotion(iEmotion1);
			SetSel(nTemp+1,nEnd + 1 - strlen(emotions[iEmotion1]));
		}
		else if (bCusEmotion) 
		{
			SetSel(nTemp,nEnd);
			ReplaceSel("");	
			//GetSel(crNew);
			InsertFace(&g_customEmotionDB[nGroup2][iEmotion2],0);
			//SetSel(nTemp+1,nEnd+1-strlen(g_customEmotionDB[nGroup2][iEmotion2].szShortCut));
			//SetSel(-1,-1);
		}
	}
}

void CRichEditCtrlEx::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	CRichEditCtrl::OnChar(nChar, nRepCnt, nFlags);
	//this->SetXFont(g_ImFont);
	//如果设置了字数限制则限制输入的字数
	if(m_nLimitWordSize != 0)
	{
		long nStartChar = 0, nEndChar = 0;
		/*DWORD dwSel = */GetSel(nStartChar, nEndChar);//获取当前光标位置
		int maxSize = m_nLimitWordSize;
		CString strName,allName;
		CString tmp;
		GetWindowText(strName);	
		int iLen =0;//文字个数
		for (int i=0;strName[i];i++) {		
			if (strName[i] < 0)		   
				i++;   			
			iLen++;
		}	
		int iLenOld=0;
		for (int i=0;m_strEditOld[i];i++) {		
			if (m_strEditOld[i] < 0)		   
				i++;   			
			iLenOld++;
		}	
		if(iLen >= maxSize && iLenOld == maxSize)
		{
			SetWindowText(m_strEditOld);
			SetSel(m_nStartCharOld, m_nEndCharOld);
			return;
		}
		else
			iLen = 0;

		for (int i=0;strName[i];i++) {		
			if (strName[i] < 0){  			   
				tmp.Format("%c%c",strName[i],strName[i+1]);
				allName = allName +tmp;
				m_strEditOld = allName;
				i++;   			
			}
			else{
				tmp.Format("%c",strName[i]);
				allName = allName +tmp;
			}
			iLen++;
			if (iLen>=maxSize){
 				SetWindowText(allName);
				SetSel(nStartChar, nEndChar);//如果光标在文本结尾则无法回到文本开头
				break;
			}
		}	
		m_strEditOld = allName;
	}
}

void CRichEditCtrlEx::SendFastWordSelectMenu()
{
	//判断如果快捷短语菜单存在并且含有"/"，则父窗口显示快捷短语菜单
	if(m_bIsShowFastWordMenu)
	{
		CString strText;
		GetText(strText);
		int nFastWord = strText.ReverseFind('/');
		if(nFastWord == -1)
			strText = "";
		else
		{
			strText = strText.Right(strText.GetLength() - nFastWord - 1);
		}
		CPoint point = GetCaretPos();
		char* pChar = (LPSTR)(LPCSTR)strText;
		GetParent()->SendMessage(WM_FASTWORD_MENU, MAKELONG(point.x, point.y), (DWORD)pChar);
	}
}
BOOL CRichEditCtrlEx::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	//if ( pMsg->message==WM_KEYDOWN )
	//{
	//	if (pMsg->wParam==13 && !KEYDOWN(VK_CONTROL) )
	//	{
	//		InsertText("\n");
	//	}
	//}
	//回车按下起来都发送一次WM_CHAR限制字数
	if ( pMsg->message==WM_KEYDOWN )
	{
		GetSel(m_nStartCharOld, m_nEndCharOld);
		CString str = "";
		str.Format("%d\n", pMsg->wParam);
		TRACE(str);
		//AfxMessageBox();
		switch (pMsg->wParam)  
		{ 
		case VK_PROCESSKEY:
		case VK_RETURN : 
			{
				if(m_nLimitWordSize > 0)
				{
					CString strName,allName;
					CString tmp;
					GetWindowText(strName);	
					int iLen =0;//文字个数
					for (int i=0;strName[i];i++) 
					{		
						if (strName[i] < 0)		   
							i++;   			
						iLen++;
					}	
					if(iLen >=m_nLimitWordSize)
					{
						return TRUE;
					}
				}
			}
		
			//SendMessage(WM_CHAR);
			break;
		}
	}
	if ( pMsg->message==WM_KEYUP )
	{
		switch (pMsg->wParam)  
		{  
		case VK_RETURN : 
			SendMessage(WM_CHAR);
			break;
		}
	}
	return CRichEditCtrl::PreTranslateMessage(pMsg);
}

void CRichEditCtrlEx::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	bool bControlKeyDown =  KEYDOWN(VK_CONTROL);
	bool bShiftKeyDown   =  KEYDOWN(VK_SHIFT);

	if (  bControlKeyDown )
	{
		if (nChar=='X' || nChar=='x')
		{
			OnCut();
			return ;
		}

		if (nChar=='C' || nChar=='c')
		{
			OnCopy();
			return ;
		}

		if (nChar=='Z' || nChar=='z')//捕获Ctrl+Z
		{
			// char * buffer;

			//if(OpenClipboard())
			//{
			//	buffer = (char*)GetClipboardData(CF_TEXT);

			//	this->InsertText(buffer);
			//	this->SetXFont(g_ImFont);

			//	EmptyClipboard();
			//	CloseClipboard();
			//}
			Undo();
			return ;
		}

		if (nChar=='Y' || nChar=='y' )
		{
			/*char * buffer;
			if(OpenClipboard())
			{
				buffer = (char*)GetClipboardData(CF_TEXT);

				this->InsertText(buffer);
				this->SetXFont(g_ImFont);

				EmptyClipboard();
				CloseClipboard();
			}*/
			Redo();
			return ;
		}

		if (nChar=='V' || nChar=='v' )
		{	
			OnPaste();
			return ;
		}
	}

	if(nChar == VK_RETURN)
	{
		if(GetParent() != NULL)
		{
			if(GetParent()->SendMessage(WM_KEYDOWN_RETURN, 0, 0) == 2)
			{
				GetParent()->SendMessage(WM_KEYDOWN, nChar, MAKELONG(nRepCnt, nFlags));
				return;
			}
		}
	}

	if(nChar == VK_OEM_2)//输入"/"则设置快捷短语菜单显示
	{
		m_bIsShowFastWordMenu = TRUE;
	}

	//获取"/"之后的字符
	CString strText;
	GetText(strText);
	int nFastWord = strText.ReverseFind('/');
	if(nFastWord == -1)
		strText = "";
	else
	{
		strText = strText.Right(strText.GetLength() - nFastWord - 1);
	}

	//如果快捷短语菜单存在则退格键先关闭菜单不退格
	if(nChar == VK_BACK)
	{		
		if(strText == "")
		{
			if(GetParent()->SendMessage(WM_FASTWORD_MENU_HIDE, nChar, MAKELONG(nRepCnt, nFlags)) == 2)
				return;
		}
	}
	//上下左右键都发送至父窗口，用于其他操作，暂时没有任何操作
	if(nChar == VK_UP || nChar == VK_DOWN)
	{
		GetParent()->SendMessage(WM_KEYDOWN_UPDOWN, nChar, MAKELONG(nRepCnt, nFlags));
	}
	if(nChar == VK_LEFT|| nChar == VK_RIGHT)
	{
		GetParent()->SendMessage(WM_FASTWORD_MENU_HIDE, nChar, MAKELONG(nRepCnt, nFlags));
	}

	CRichEditCtrl::OnKeyDown(nChar, nRepCnt, nFlags);


}

void CRichEditCtrlEx::OnSetFocus(CWnd* pOldWnd)
{
	CRichEditCtrl::OnSetFocus(pOldWnd);

	m_bFocused = true;

	SetSel(-1,-1);
}

void CRichEditCtrlEx::OnKillFocus(CWnd* pNewWnd)
{
	CRichEditCtrl::OnKillFocus(pNewWnd);

	m_bFocused = false;
}

DWORD CRichEditCtrlEx::InsertProgressBar()
{
	SCODE			sc ;
	HRESULT			hr ;
	LPLOCKBYTES		lpLockBytes = NULL;
	LPOLECLIENTSITE lpClientSite ;

	IMiniProgressBarPtr	lpProgress = NULL  ;
	LPSTORAGE		lpStorage    ;
	LPOLEOBJECT		lpObject     ;

	DWORD			dwBarIndex = -1;

	sc = ::CreateILockBytesOnHGlobal(NULL, TRUE, &lpLockBytes);
	if (sc != S_OK)
		AfxThrowOleException(sc);
	ASSERT(lpLockBytes != NULL);

	sc = ::StgCreateDocfileOnILockBytes(lpLockBytes,
		STGM_SHARE_EXCLUSIVE|STGM_CREATE|STGM_READWRITE, 0, &lpStorage);
	if (sc != S_OK)
	{
		VERIFY(lpLockBytes->Release() == 0);
		lpLockBytes = NULL;
		AfxThrowOleException(sc);
	}
	ASSERT(lpStorage != NULL);
	IRichEditOle *lpRichEditOle = GetIRichEditOle();
	lpRichEditOle->GetClientSite(&lpClientSite);
	ASSERT(lpClientSite != NULL);
	try
	{
		hr = lpProgress.CreateInstance(CLSID_CMiniProgressBar);
		if( FAILED(hr) )
			_com_issue_error(hr);

		// 获取被嵌入的对象
		hr = lpProgress.QueryInterface(IID_IOleObject, (void**)&lpObject);
		lpObject->SetClientSite(lpClientSite);
		if( FAILED(hr) )
			_com_issue_error(hr);

		dwBarIndex = m_dwBarIndex++;
		lpProgress->put_OwnerData(dwBarIndex);

		// 通知一个对象被嵌入容器中
		hr = OleSetContainedObject(lpObject, TRUE);

		//2 insert in 2 richedit, you need a struct of REOBJECT
		REOBJECT reobject;
		ZeroMemory(&reobject, sizeof(REOBJECT));
		reobject.cbStruct = sizeof(REOBJECT);
		CLSID clsid;
		// 获取被嵌入对象的类id
		sc = lpObject->GetUserClassID(&clsid);
		if (sc != S_OK)
			AfxThrowOleException(sc);

		reobject.clsid	= clsid;
		// 能被选择
		reobject.cp		= REO_CP_SELECTION;
		// 内容
		reobject.dvaspect	= DVASPECT_CONTENT;
		// 和文字同一水平线
		reobject.dwFlags	= REO_BELOWBASELINE| REO_INPLACEACTIVE ;

		reobject.dwUser		= ODT_PROGRESS_BAR;
		// 嵌入对象
		reobject.poleobj	= lpObject;
		// 
		reobject.polesite	= lpClientSite;
		// 存储数据
		reobject.pstg		= lpStorage;

		SIZEL sizel={0,0};
		reobject.sizel = sizel;

		SetSel(-1,-1);
		InsertText("\n");
		SetStartIndent(120);

		lpRichEditOle->InsertObject(&reobject);
		InsertText("\n");

		if(lpObject)
			lpObject->Release();	
		if(lpLockBytes)
			lpLockBytes->Release();
		if(lpClientSite)
			lpClientSite->Release();
		if(lpRichEditOle)
			lpRichEditOle->Release();
		if(lpStorage)
			lpStorage->Release();
		/*if(lpProgress)
		lpProgress->Release();*/

	}
	catch(...)
	{
		TRACE("CRichEditCtrlEx插入图片出现异常!\n");
	}

	return dwBarIndex;
}

DWORD CRichEditCtrlEx::FindProgressBar()
{
	IRichEditOle* pReO = GetIRichEditOle();
	int nCount = pReO->GetObjectCount();

	REOBJECT robj;
	robj.cbStruct = sizeof(REOBJECT);
	for(int i = 0; i < nCount; i++)
	{
		pReO->GetObject(i, &robj, REO_GETOBJ_POLEOBJ | REO_GETOBJ_PSTG);

		if ( robj.dwUser==ODT_PROGRESS_BAR )
		{
			IMiniProgressBarPtr pBar;
			robj.poleobj->QueryInterface(IID_IMiniProgressBar,(void **)&pBar);
			unsigned long dwData;
			pBar->get_OwnerData(&dwData);
			return dwData;
		}
	}

	return (DWORD)-1;
}

void CRichEditCtrlEx::SetProgressBarPos(DWORD dwBar,int nPos)
{
	IRichEditOle* pReO = GetIRichEditOle();
	int nCount = pReO->GetObjectCount();

	REOBJECT robj;
	robj.cbStruct = sizeof(REOBJECT);
	for(int i = 0; i < nCount; i++)
	{
		pReO->GetObject(i, &robj, REO_GETOBJ_POLEOBJ | REO_GETOBJ_PSTG);

		if ( robj.dwUser==ODT_PROGRESS_BAR )
		{
			IMiniProgressBarPtr pBar;
			robj.poleobj->QueryInterface(IID_IMiniProgressBar,(void **)&pBar);
			unsigned long dwData;
			pBar->get_OwnerData(&dwData);
			if ( dwData==dwBar )
			{
				pBar->SetPos(nPos);
			}
		}
	}
}

void CRichEditCtrlEx::SetProgressBarTip(DWORD dwBar,CString strTip)
{
	IRichEditOle* pReO = GetIRichEditOle();
	int nCount = pReO->GetObjectCount();

	REOBJECT robj;
	robj.cbStruct = sizeof(REOBJECT);
	for(int i = 0; i < nCount; i++)
	{
		pReO->GetObject(i, &robj, REO_GETOBJ_POLEOBJ | REO_GETOBJ_PSTG);

		if ( robj.dwUser==ODT_PROGRESS_BAR )
		{
			IMiniProgressBarPtr pBar;
			robj.poleobj->QueryInterface(IID_IMiniProgressBar,(void **)&pBar);
			unsigned long dwData;
			pBar->get_OwnerData(&dwData);
			if ( dwData==dwBar )
			{
				pBar->SetTip(strTip.AllocSysString());
			}
		}
	}
}

void CRichEditCtrlEx::InsertMMInfoBar(DWORD dwMMID, bool bRequest, CString strMMType, CString strFriendNickName)
{
	SCODE			sc ;
	HRESULT			hr ;
	LPLOCKBYTES		lpLockBytes = NULL;
	LPOLECLIENTSITE lpClientSite ;
	IMiniMMInfoBarPtr	lpMMInfo;
	LPSTORAGE		lpStorage    ;
	LPOLEOBJECT		lpObject     ;

	DWORD			dwBarIndex = -1;

	sc = ::CreateILockBytesOnHGlobal(NULL, TRUE, &lpLockBytes);
	if (sc != S_OK)
		AfxThrowOleException(sc);
	ASSERT(lpLockBytes != NULL);

	sc = ::StgCreateDocfileOnILockBytes(lpLockBytes,
		STGM_SHARE_EXCLUSIVE|STGM_CREATE|STGM_READWRITE, 0, &lpStorage);
	if (sc != S_OK)
	{
		VERIFY(lpLockBytes->Release() == 0);
		lpLockBytes = NULL;
		AfxThrowOleException(sc);
	}
	ASSERT(lpStorage != NULL);
	IRichEditOle *lpRichEditOle = GetIRichEditOle();
	lpRichEditOle->GetClientSite(&lpClientSite);
	ASSERT(lpClientSite != NULL);
	try
	{
		hr = lpMMInfo.CreateInstance(CLSID_CMiniMMInfoBar);
		if( FAILED(hr) )
			_com_issue_error(hr);

		// 获取被嵌入的对象
		hr = lpMMInfo.QueryInterface(IID_IOleObject, (void**)&lpObject);
		lpObject->SetClientSite(lpClientSite);
		if( FAILED(hr) )
			_com_issue_error(hr);

		// 通知一个对象被嵌入容器中
		hr = OleSetContainedObject(lpObject, TRUE);

		//2 insert in 2 richedit, you need a struct of REOBJECT
		REOBJECT reobject;
		ZeroMemory(&reobject, sizeof(REOBJECT));
		reobject.cbStruct = sizeof(REOBJECT);
		CLSID clsid;
		// 获取被嵌入对象的类id
		sc = lpObject->GetUserClassID(&clsid);
		if (sc != S_OK)
			AfxThrowOleException(sc);

		reobject.clsid	= clsid;
		// 能被选择
		reobject.cp		= REO_CP_SELECTION;
		// 内容
		reobject.dvaspect	= DVASPECT_CONTENT;
		// 和文字同一水平线
		reobject.dwFlags	= REO_BELOWBASELINE| REO_INPLACEACTIVE ;
	
		strMMType.Trim();
		strMMType.MakeLower();

		bool bVedio;

		if ( strMMType=="vedio" )
		{
			if ( bRequest )
				reobject.dwUser = ODT_SEND_VEDIO;
			else
				reobject.dwUser = ODT_RECV_VEDIO;

			bVedio = true;
		}
		else
		{
			if ( bRequest )
				reobject.dwUser = ODT_SEND_AUDIO;
			else
				reobject.dwUser = ODT_RECV_AUDIO;

			bVedio = false;
		}
		// 嵌入对象
		reobject.poleobj	= lpObject;
		// 
		reobject.polesite	= lpClientSite;
		// 存储数据
		reobject.pstg		= lpStorage;

		SIZEL sizel={0,0};
		reobject.sizel = sizel;

		SetSel(-1,-1);
		InsertText("\n");
		SetStartIndent(120);

		lpRichEditOle->InsertObject(&reobject);
		lpMMInfo->put_MMType(strMMType.AllocSysString());
		lpMMInfo->put_OwnerData(dwMMID);
		InsertText("\n");

		XT_FONT xf;
		xf.fontColor = RGB(122,122,122);
		SetSel(-1,-1);
		SetSelectionCharFormat(CRichEditCtrlEx::XFont2CharFormat(xf));

		if ( bRequest )
		{
			CString strTip;

			if ( bVedio )
			{
				strTip.Format("请求与 %s 视频聊天",strFriendNickName);
			}
			else
			{
				strTip.Format("请求与 %s 语音聊天",strFriendNickName);
			}

			InsertText(strTip);
			InsertText(" ");
			InsertLink("取消");
			InsertText(" ");

			InsertText("\n");
		}
		else
		{			
			CString strTip;

			if ( bVedio )
			{
				strTip.Format("%s 请求与你视频聊天",strFriendNickName);
			}
			else
			{
				strTip.Format("%s 请求与你语音聊天",strFriendNickName);
			}

			InsertText(strTip);
			InsertText(" ");

			SetSel(-1,-1);
			SetSelectionCharFormat(CRichEditCtrlEx::XFont2CharFormat(xf));
			InsertLink("同意");
			InsertText(" / ");

			SetSel(-1,-1);
			SetSelectionCharFormat(CRichEditCtrlEx::XFont2CharFormat(xf));
			InsertLink("拒绝");

			InsertText(" ");
			InsertText("\n");
		}

		if(lpObject)
			lpObject->Release();	
		if(lpLockBytes)
			lpLockBytes->Release();
		if(lpClientSite)
			lpClientSite->Release();
		if(lpRichEditOle)
			lpRichEditOle->Release();
		if(lpStorage)
			lpStorage->Release();
		/*if(lpProgress)
		lpProgress->Release();*/

	}
	catch(...)
	{
		TRACE("CRichEditCtrlEx插入MMInfoBar出现异常!\n");
	}

}


void CRichEditCtrlEx::InsertFileInfoBar(DWORD dwFileID, bool bSendFile, CString strFileName, long nFileSize)
{
	SCODE			sc ;
	HRESULT			hr ;
	LPLOCKBYTES		lpLockBytes = NULL;
	LPOLECLIENTSITE lpClientSite ;
	IMiniFileInfoBarPtr	lpFileInfo;
	LPSTORAGE		lpStorage    ;
	LPOLEOBJECT		lpObject     ;

	DWORD			dwBarIndex = -1;

	sc = ::CreateILockBytesOnHGlobal(NULL, TRUE, &lpLockBytes);
	if (sc != S_OK)
		AfxThrowOleException(sc);
	ASSERT(lpLockBytes != NULL);

	sc = ::StgCreateDocfileOnILockBytes(lpLockBytes,
		STGM_SHARE_EXCLUSIVE|STGM_CREATE|STGM_READWRITE, 0, &lpStorage);
	if (sc != S_OK)
	{
		VERIFY(lpLockBytes->Release() == 0);
		lpLockBytes = NULL;
		AfxThrowOleException(sc);
	}
	ASSERT(lpStorage != NULL);
	IRichEditOle *lpRichEditOle = GetIRichEditOle();
	lpRichEditOle->GetClientSite(&lpClientSite);
	ASSERT(lpClientSite != NULL);
	try
	{
		hr = lpFileInfo.CreateInstance(CLSID_CMiniFileInfoBar);
		if( FAILED(hr) )
			_com_issue_error(hr);

		// 获取被嵌入的对象
		hr = lpFileInfo.QueryInterface(IID_IOleObject, (void**)&lpObject);
		lpObject->SetClientSite(lpClientSite);
		if( FAILED(hr) )
			_com_issue_error(hr);

		lpFileInfo->put_OwnerData(dwFileID);

		// 通知一个对象被嵌入容器中
		hr = OleSetContainedObject(lpObject, TRUE);

		//2 insert in 2 richedit, you need a struct of REOBJECT
		REOBJECT reobject;
		ZeroMemory(&reobject, sizeof(REOBJECT));
		reobject.cbStruct = sizeof(REOBJECT);
		CLSID clsid;
		// 获取被嵌入对象的类id
		sc = lpObject->GetUserClassID(&clsid);
		if (sc != S_OK)
			AfxThrowOleException(sc);

		reobject.clsid	= clsid;
		// 能被选择
		reobject.cp		= REO_CP_SELECTION;
		// 内容
		reobject.dvaspect	= DVASPECT_CONTENT;
		// 和文字同一水平线
		reobject.dwFlags	= REO_BELOWBASELINE| REO_INPLACEACTIVE ;

		reobject.dwUser		= bSendFile? ODT_SEND_FILE : ODT_RECV_FILE;
		// 嵌入对象
		reobject.poleobj	= lpObject;
		// 
		reobject.polesite	= lpClientSite;
		// 存储数据
		reobject.pstg		= lpStorage;

		SIZEL sizel={0,0};
		reobject.sizel = sizel;

		SetSel(-1,-1);
		InsertText("\n");
		SetStartIndent(120);

		lpRichEditOle->InsertObject(&reobject);
		lpFileInfo->SetFileInfo(strFileName.AllocSysString(),nFileSize);
		lpFileInfo->put_OwnerData(dwFileID);
		InsertText("\n");

		XT_FONT xf;
		xf.fontColor = RGB(122,122,122);
		SetSel(-1,-1);
		SetSelectionCharFormat(CRichEditCtrlEx::XFont2CharFormat(xf));


		if ( bSendFile )
		{
			InsertLink("取消");
			InsertText(" ");
			InsertText("\n");
		}
		else
		{			
			SetSel(-1,-1);
			SetSelectionCharFormat(CRichEditCtrlEx::XFont2CharFormat(xf));
			InsertLink("接收");
			InsertText(" / ");

			SetSel(-1,-1);
			SetSelectionCharFormat(CRichEditCtrlEx::XFont2CharFormat(xf));
			InsertLink("拒绝");

			InsertText(" / ");

			SetSel(-1,-1);
			SetSelectionCharFormat(CRichEditCtrlEx::XFont2CharFormat(xf));

			InsertLink("另存为");
			InsertText(" ");
			InsertText("\n");
		}

		if(lpObject)
			lpObject->Release();	
		if(lpLockBytes)
			lpLockBytes->Release();
		if(lpClientSite)
			lpClientSite->Release();
		if(lpRichEditOle)
			lpRichEditOle->Release();
		if(lpStorage)
			lpStorage->Release();
		/*if(lpProgress)
		lpProgress->Release();*/

	}
	catch(...)
	{
		TRACE("CRichEditCtrlEx插入文件出现异常!\n");
	}
}

void CRichEditCtrlEx::SetMMBarTip(DWORD dwData, OLE_DATA_TYPE odtType, CString strTip, COLORREF clrTip)
{
	IRichEditOle *ole = GetIRichEditOle();
	int n = ole->GetObjectCount();

	REOBJECT reo;
	reo.cbStruct = sizeof(reo);

	for (int i = 0; i <n; ++i)
	{
		ole->GetObject(i, &reo, REO_GETOBJ_ALL_INTERFACES);

		if ( reo.dwUser==odtType )
		{
			IMiniMMInfoBar *pMM = NULL;
			reo.poleobj->QueryInterface(IID_IMiniMMInfoBar,(void **)&pMM);	
	
			if ( pMM && pMM->OwnerData==dwData )
			{
				CHARRANGE	crSave;
				CString		strSel;

				GetSel(crSave);
				SetSel(reo.cp+1,-1);
				strSel = GetSelText();
				SetSel(crSave);
				int pos1 = strSel.Find("\r",0);
				int pos2 = strSel.Find("\r",pos1+1);

				CString strSub = strSel.Mid(pos1+1,pos2-pos1-1);

				USES_CONVERSION;
				int nLen =wcslen(A2W((LPCSTR)strSub));

				SetSel(reo.cp+1+pos1+1,reo.cp+2+nLen);
				//ReplaceSel("");
			
				CResBitmap bmpLine;
				bmpLine.LoadBitmap(IDB_HINT_FLAG2);
				InsertBitmap((HBITMAP)bmpLine.GetSafeHandle());

				SetStartIndent(230);
				XT_FONT xf;

				xf.fontColor = clrTip;//RGB(233,222,255);

				CString strInsertTip = " "+strTip;

				InsertText(strInsertTip);

				nLen = wcslen(A2W((LPCSTR)strInsertTip));

				SetXFont(xf,reo.cp+1+pos1+1,reo.cp+3+nLen); 

				SetSel(reo.cp+3+nLen,reo.cp+3+nLen);
			}
		}
	}
}

void CRichEditCtrlEx::SetFileBarTip(DWORD dwFileID, OLE_DATA_TYPE odtType, CString strTip, COLORREF clrTip)
{
	IRichEditOle *ole = GetIRichEditOle();
	int n = ole->GetObjectCount();

	REOBJECT reo;
	reo.cbStruct = sizeof(reo);

	for (int i = 0; i <n; ++i)
	{
		ole->GetObject(i, &reo, REO_GETOBJ_ALL_INTERFACES);

		if ( reo.dwUser==odtType )
		{
			IMiniFileInfoBar *pImage = NULL;
			reo.poleobj->QueryInterface(IID_IMiniFileInfoBar,(void **)&pImage);	
			odtType = (OLE_DATA_TYPE)reo.dwUser;
	
			if ( pImage && pImage->OwnerData==dwFileID )
			{
				CHARRANGE	crSave;
				CString		strSel;

				GetSel(crSave);
				SetSel(reo.cp+1,-1);
				strSel = GetSelText();
				SetSel(crSave);
				int pos1 = strSel.Find("\r",0);
				int pos2 = strSel.Find("\r",pos1+1);

				CString strSub = strSel.Mid(pos1+1,pos2-pos1-1);

				USES_CONVERSION;
				int nLen =wcslen(A2W((LPCSTR)strSub));

				SetSel(reo.cp+1+pos1+1,reo.cp+2+nLen);
				//ReplaceSel("");
			
				CResBitmap bmpLine;
				bmpLine.LoadBitmap(IDB_HINT_FLAG2);
				InsertBitmap((HBITMAP)bmpLine.GetSafeHandle());

				SetStartIndent(230);
				XT_FONT xf;

				xf.fontColor = clrTip;//RGB(233,222,255);

				CString strInsertTip = " "+strTip;

				InsertText(strInsertTip);

				nLen = wcslen(A2W((LPCSTR)strInsertTip));

				SetXFont(xf,reo.cp+1+pos1+1,reo.cp+3+nLen); 

				SetSel(reo.cp+3+nLen,reo.cp+3+nLen);
			}
		}
	}
}

IMiniMMInfoBar * CRichEditCtrlEx::FindNearestMMInfoBar(int pos, OLE_DATA_TYPE &odtType, int & nBarPos )
{
	IRichEditOle *ole = GetIRichEditOle();
	int n = ole->GetObjectCount();

	REOBJECT reo;
	reo.cbStruct = sizeof(reo);

	for (int i = n-1; i >=0; --i)
	{
		ole->GetObject(i, &reo, REO_GETOBJ_ALL_INTERFACES);

		if ( reo.cp<pos )
		{
			IMiniMMInfoBar *pMM = NULL;

			if ( reo.dwUser==ODT_RECV_AUDIO || reo.dwUser==ODT_SEND_AUDIO || reo.dwUser==ODT_RECV_VEDIO || reo.dwUser==ODT_SEND_VEDIO )
			{
				reo.poleobj->QueryInterface(IID_IMiniMMInfoBar,(void **)&pMM);	
				odtType = (OLE_DATA_TYPE)reo.dwUser;
			}

			nBarPos = reo.cp;

			return pMM;
		}
	}

	return NULL;
}

IMiniFileInfoBar * CRichEditCtrlEx::FindNearestFileInfoBar(int pos, OLE_DATA_TYPE &odtType , int & nBarPos )
{
	IRichEditOle *ole = GetIRichEditOle();
	int n = ole->GetObjectCount();

	REOBJECT reo;
	reo.cbStruct = sizeof(reo);

	for (int i = n-1; i >=0; --i)
	{
		ole->GetObject(i, &reo, REO_GETOBJ_ALL_INTERFACES);

		if ( reo.cp<pos && (reo.dwUser==ODT_RECV_FILE || reo.dwUser==ODT_SEND_FILE ) )
		{
			IMiniFileInfoBar *pImage = NULL;
			reo.poleobj->QueryInterface(IID_IMiniFileInfoBar,(void **)&pImage);	
			odtType = (OLE_DATA_TYPE)reo.dwUser;

			nBarPos = reo.cp;
			return pImage;
		}
	}

	return NULL;
}

void CRichEditCtrlEx::OnDropFiles(HDROP hDrop)
{
	char   szFilePathName[_MAX_PATH+1] = {0};
	UINT  nNumOfFiles = DragQueryFile(hDrop, 0xFFFFFFFF, NULL, 0); //得到文件个数
	
	for (UINT nIndex=0 ; nIndex< nNumOfFiles; ++nIndex)
	{
		DragQueryFile(hDrop, nIndex, szFilePathName, _MAX_PATH);  //得到文件名
		GetParent()->SendMessage(WM_USER_DROPFILE,(WPARAM)szFilePathName,0);
	}

	DragFinish(hDrop);
}



void CRichEditCtrlEx::OnTimer(UINT nIDEvent)
{
	switch(nIDEvent)
	{
	case 0x01:
		KillTimer(0x01);
		SetSel(-1,-1);
		break;
	case TIMER_CAPTURE_USERNAME:
		//m_bCapture = true;
		break;
	}


  // TODO: 在此添加消息处理程序代码和/或调用默认值  
  
    // 定时器ID必须为我们所设置的定时器ID，不要以为调用SetTimer这里收到的就一定时我们  
    // 设置的定时器，别忘了父类还可能设置，这一点不用怀疑，我测试到过！  
    // 当写入很多行文本，滚动条出现后，再在最后插入动画，然后向下滚动到底部，再继续向下  
    // 滚动，虽然滚动条不动了，但是动画却动得特别快，显然收到了父类的定时器  
    // 因此这个定时器ID最好设置一个不常用的值，这里随便设置了一个，为1  
    if (nIDEvent == FRM_TIMER_ID) {  
  
        // 得到对象总数，大于0时才需要刷新  
		if(m_pRichEditOle!=NULL)
		{
        int nObjectCount = m_pRichEditOle->GetObjectCount();  
        if (nObjectCount > 0) {  
            CRect rc;  
            GetRect(rc); // 得到可视区文本的矩形区域  
            // 分别使用左上角和右下角的点得到最小和最大字符索引  
            // 即可见区域的最小和最大字符索引  
            int cpMin = CharFromPos(rc.TopLeft());  
            int cpMax = CharFromPos(rc.BottomRight());  
  
            // 使用二分查找算法找到第一个字符索引大于或等于cpMin的对象索引  
            int iFirst = FindFirstObject(cpMin, nObjectCount);  
            REOBJECT reo = {0};  
            reo.cbStruct = sizeof(REOBJECT);  
  
            // 从第一个索引开始遍历对象更换帧  
            for (int i = iFirst; i < nObjectCount; i++) {  
                if (m_pRichEditOle->GetObject(i, &reo, REO_GETOBJ_POLEOBJ) == S_OK) {  
                    reo.poleobj->Release();  
                    // 当前对象的字符索引大于最大字符索引，说明对象不在可见区域，停止遍历  
                    if (reo.cp > cpMax) {  
                        break;  
                    }  
  
                    // 是COleImage对象时才能更新  
                    if (InlineIsEqualGUID(reo.clsid, CLSID_OleImage)) {  
                        // 更换帧  
                        COleImage *pOleImage = COleImage::FromOleObject(reo.poleobj);  
                        pOleImage->ChangeFrame();  
                    }  
                }  
            }  
        }  
    }
	}
	CRichEditCtrl::OnTimer(nIDEvent);
}

void CRichEditCtrlEx::InsertUserLink(const char * szUser,uint32 id)
{
	USER_LINK ul;
	LONG lBeg;
	CHARFORMAT cf;
	SetSel(-1,-1);
	GetSel(ul.range);
	lBeg = ul.range.cpMin;
	ReplaceSel(szUser);
	GetSel(ul.range);
	ul.range.cpMin = lBeg;
	ul.sName = szUser;
	ul.id = id;
	SetSel(ul.range);
	GetSelectionCharFormat(cf);
	cf.dwEffects = CFE_UNDERLINE;
	cf.crTextColor = RGB(0,0,255);
	SetSelectionCharFormat(cf);
	SetSel(-1,-1);

	m_arrName.push_back(ul);

}
void CRichEditCtrlEx::OnLButtonUp(UINT nFlags, CPoint point)
{
	int nIdx;
	nIdx = PointToUserLink(point);
	if(nIdx >= 0)
	{
		CWnd* pParent;
		pParent = GetParent();
		if(pParent)
		{
			pParent->SendMessage(WM_USERLINK_CLICK,(WPARAM)m_arrName[nIdx].id,(LPARAM)(LPCTSTR)m_arrName[nIdx].sName);
		}
	}

	if(m_vecFriendClusterLink.size() > 0)
	{
		int n = CharFromPos(point);
		for(int nLink = 0; nLink < m_vecFriendClusterLink.size(); nLink++)
		{
			FRIEND_CLUSTER_LINK LinkInfo = m_vecFriendClusterLink[nLink];

			if(n >= LinkInfo.range.cpMin && n < LinkInfo.range.cpMax)
			{
				if(LinkInfo.nType == 1)
				{
					if(LinkInfo.friendInfo != NULL)
					{
						XT_USERINFO_GET_REQ req;
						req.fid = LinkInfo.friendInfo->GetId();
						//req.uid = g_ImUserInfo.GetId();
						g_pProtocol->SendReq(req);

						CFriendInfoDlg::NewDlg(LinkInfo.friendInfo);
					}
				}
				else if(LinkInfo.nType == 2)
				{
					if(LinkInfo.clusterInfo != NULL)
					{
						std::tr1::shared_ptr<CClusterInfoDlg> pWnd = CClusterInfoDlg::NewDlg(LinkInfo.clusterInfo);
						if(pWnd)
						{
							// 向服务器请求群信息
							XT_GET_CINFO_REQ req;
							req.nFamilyID   = LinkInfo.clusterInfo->nFamilyID;
							req.nFamilyProp = PERM_FAMILY;
							g_pProtocol->SendReq(req);
							// 显示群信息
							pWnd->ShowWindow(SW_SHOW);
							//pWnd->CenterWindow(GetDesktopWindow());
						}
					}
					else
						CMessageBox::Model(GetSafeHwnd(),"该群已被解散！",IM_CAPTION,MB_OK);
				}
			}
		}
	}
	CRichEditCtrl::OnLButtonUp(nFlags, point);
}

BOOL CRichEditCtrlEx::DestroyWindow()
{
	return CRichEditCtrl::DestroyWindow();
}

void CRichEditCtrlEx::OnMouseMove(UINT nFlags, CPoint point)
{
	//if(m_bCapture)
	//{
		int nIdx = PointToUserLink(point);
		HCURSOR hCursor = NULL;
		if(nIdx >= 0)
		{
			HideCaret();
			hCursor = LoadCursor(NULL,IDC_HAND);
			if(hCursor)
			{
				SetCursor(hCursor);
			}
			
		}
		else
		{
			m_bCapture = false;
			ShowCaret();
		}
	//}

	if(m_bIsShowLinkInformation)
	{
		IRichEditOle *ole = GetIRichEditOle();
		int nOleCount = ole->GetObjectCount();

		REOBJECT reo;
		reo.cbStruct = sizeof(reo);

		for(int nOle = 0; nOle < nOleCount; nOle++)
		{
			ole->GetObject(nOle, &reo, REO_GETOBJ_ALL_INTERFACES);

			if(reo.dwUser == ODT_LINK_INFO)
			{
				CPoint pt = GetCharPos( reo.cp );
 				long x = point.x;
 				long y = point.y;
				_DOleLinkInfo* iut = NULL;
				reo.poleobj->QueryInterface(DIID__DOleLinkInfo,(void **)&iut);
				int nResult = iut->IsPointInLinkRect(pt.x, pt.y, x, y);
				if(nResult == 1)
				{
					hCursor = LoadCursor(NULL,IDC_HAND);
					if(hCursor)
					{
						_bstr_t url = iut->GetUrlText();
						m_strLinkInfoUrl = (LPCSTR)url;
						SysFreeString(url);
						m_bIsPointInLinkRect = TRUE;
						SetCursor(hCursor);
						if( iut != NULL )	iut->Release();
						if( reo.poleobj     != NULL )  reo.poleobj->Release();
						if( reo.polesite     != NULL )  reo.polesite->Release();
						if( reo.pstg     != NULL )  reo.pstg->Release();
						return;
					}
				}
				if( iut     != NULL )	iut->Release();
				if( reo.poleobj     != NULL )  reo.poleobj->Release();
				if( reo.polesite     != NULL )  reo.polesite->Release();
				if( reo.pstg     != NULL )  reo.pstg->Release();
			}
		}
	}
	m_bIsPointInLinkRect = FALSE;
	CRichEditCtrl::OnMouseMove(nFlags, point);
}

int CRichEditCtrlEx::PointToUserLink(CPoint point)
{
	int n = CharFromPos(point);
	int nSize = (int)m_arrName.size();
	int i;
	for(i = 0;i < nSize;i++)
	{
		if(n >= m_arrName[i].range.cpMin && n < m_arrName[i].range.cpMax)
		{
			return i;
		}
	}

	return -1;

}

void CRichEditCtrlEx::OnCancelZYF()
{
	m_bZYF = !m_bZYF;
}

void CRichEditCtrlEx::InsertFace(const CUSTOM_EMOTION * pFace,int nParam)
{
	switch(pFace->type)
	{
	case CUSTOM_EMOTION::SYS:
		InsertEmotion(nParam);
		break;
	case CUSTOM_EMOTION::SYS_GIF:
		InsertGifEmotion(nParam + 1);
		break;
	case CUSTOM_EMOTION::CUSTOM:
		{
			CString strCustomFaceFile = g_LocalData.GetCustomFacePath() + pFace->szFile;
			CString strCacheFile = g_LocalData.GetImageFileCachePath() + pFace->szFile;
			if ( _access((LPCTSTR)strCacheFile,0)==-1 )
				CopyFile(strCustomFaceFile,strCacheFile,FALSE);
			InsertImage(strCacheFile,EMOTION_PIC_DATA());
		}
		break;
	case CUSTOM_EMOTION::WEB:
		{
			CString strCustomFaceFile = g_LocalData.GetCustomFacePath() + pFace->szFile;
			CString strCacheFile = g_LocalData.GetImageFileCachePath() + pFace->szFile;
			// 先判断缓存文件夹中有没有该表情
			if ( _access((LPCTSTR)strCacheFile,0)==-1 )
				CopyFile(strCustomFaceFile,strCacheFile,FALSE);

			// 如果url为空，则发送本地图片
			if(strcmp(pFace->szUrl,"")==0)
			{		
				// 显示缓存中的图片文件
				InsertImage(strCacheFile,EMOTION_PIC_DATA());
			}
			else
			{
				EMOTION_PIC_DATA data;
				data.nType = EPT_WEB;
				data.sData = pFace->szUrl;
				InsertImage(strCacheFile,data);
			}
		}
	}
}

LRESULT CRichEditCtrlEx::OnIMEChar(WPARAM wParm,LPARAM lParam)
{
	WORD w = wParm;
	TRACE("ime char = %d\n",w);
	return 0;
}


void CRichEditCtrlEx::OnEnChange()
{
	if ( GetOptions() & ECO_READONLY )
		return ;
	if(m_bZYF)
		ProcessEmotion();
}


//  
// 查找第一个字符索引大于或等于cpMin的对象在所有对象中的索引  
//  
int CRichEditCtrlEx::FindFirstObject(int cpMin, int nObjectCount)  
{  
    // 标准的二分查找算法，不用解释了  
    int low = 0;  
    int high = nObjectCount - 1;  
    REOBJECT reoMid = {0};  
    reoMid.cbStruct = sizeof(REOBJECT);  
    while (low <= high) {  
        int mid = (low + high) >> 1;  
        if (m_pRichEditOle->GetObject(mid, &reoMid, REO_GETOBJ_POLEOBJ) != S_OK) {  
            return -1;  
        }  
        reoMid.poleobj->Release();  
        if (reoMid.cp == cpMin) {  
            return mid;  
        } else if (reoMid.cp < cpMin) {  
            low = mid + 1;  
        } else {  
            high = mid - 1;  
        }  
    }  
  
    // 只不过最后没找到时不是返回-1，而是返回low，此时low必然大于high  
    // 刚好满足条件  
    return low;  
}  

//
// 插入图像
//
HRESULT CRichEditCtrlEx::InsertImage(LPCTSTR lpszPathName)
{
	// 全部使用智能指针
	CComPtr<IStorage> spStorage;
	CComPtr<ILockBytes> spLockBytes;
	CComPtr<IOleClientSite> spOleClientSite;	
	COleImage* spOleImage;   //这不能用智能指针
	CComPtr<IOleObject> spOleObject;
	CLSID clsid;
	REOBJECT reobject;
	HRESULT hr = E_FAIL;

	do {

		// 创建LockBytes
		hr = CreateILockBytesOnHGlobal(NULL, TRUE, &spLockBytes);
		if (hr != S_OK) {
			break;
		}

		ASSERT(spLockBytes != NULL);

		// 创建Storage
		hr = StgCreateDocfileOnILockBytes(spLockBytes,
			STGM_SHARE_EXCLUSIVE | STGM_CREATE | STGM_READWRITE, 0, &spStorage);
		if (hr != S_OK) {
			break;
		}

		// 获取ClientSite
		hr = m_pRichEditOle->GetClientSite(&spOleClientSite);
		if (hr != S_OK) {
			break;
		}

		// 创建COleImage实例
		hr = CoCreateInstance(CLSID_OleImage, NULL, CLSCTX_INPROC, IID_IOleImage, (LPVOID*) &spOleImage);
		if (hr != S_OK) {
			break;
		}

		spOleImage->m_strImageName = (CString)lpszPathName;

		// 加载图像
		hr = spOleImage->LoadFromFile(_bstr_t(lpszPathName), this, 400);
		if (hr != S_OK) {
			break;
		}

		// 获取IOleObject接口
		hr = spOleImage->QueryInterface(IID_IOleObject, (LPVOID *) &spOleObject);
		if (hr != S_OK) {
			break;
		}
		spOleImage->Release();	/*QueryInterface会增加引用计数，调用后恢复一下*/	

		// 获取IOleObject的用户CLSID
		hr = spOleObject->GetUserClassID(&clsid);
		if (hr != S_OK) {
			break;
		}

		// 填充OLE对象属性
		ZeroMemory(&reobject, sizeof(REOBJECT));		
		reobject.cbStruct	= sizeof(REOBJECT);
		reobject.clsid		= clsid;
		reobject.cp			= REO_CP_SELECTION;
		reobject.dvaspect	= DVASPECT_CONTENT;
		reobject.dwFlags	= REO_BELOWBASELINE;
		reobject.poleobj	= spOleObject;
		reobject.polesite	= spOleClientSite;
		reobject.pstg		= spStorage;
		SIZEL sizel = {0};
		reobject.sizel = sizel;
		reobject.dwUser = -1;

		// 插入OLE对象
		hr = m_pRichEditOle->InsertObject(&reobject);
		if (hr != S_OK) {
			break;
		}

		// 通知OLE容器保证OLE对象被正确引用
		hr = OleSetContainedObject(spOleObject, TRUE);

		if(spOleObject)
			spOleObject.Release();	

		if(spLockBytes)
			spLockBytes.Release();

		if(spOleClientSite)
			spOleClientSite.Release();

		/*if(m_pRichEditOle)
		m_pRichEditOle.Release();*/

		//加入vec,为了释放清空
		//m_vecOleImage.push_back(spOleImage);

	} while (0);

	return hr;
}

void CRichEditCtrlEx::OnSelAll()
{
	SetSel(0, -1);
}

void CRichEditCtrlEx::OnClear()
{
	SetWindowText("");
}

// 响应WM_CREATE
int CRichEditCtrlEx::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	int nRlt = CRichEditCtrl::OnCreate(lpCreateStruct);

	// 这个接口指针使用非常频繁，存下来提高效率  
	if(m_pRichEditOle == NULL)
		m_pRichEditOle = GetIRichEditOle();  

	return nRlt;
}

void CRichEditCtrlEx::SetLimitWordSize(int nSize)
{
	m_nLimitWordSize = nSize;
}

void CRichEditCtrlEx::SetEditType(int nType)
{
	m_nEditType = nType;
}

LRESULT CRichEditCtrlEx::OnWebFileNotify(WPARAM wParam, LPARAM lParam)
{
	IMAGE_FILE_STRUCT *pStruct = (IMAGE_FILE_STRUCT*)wParam;
	char * szFile = (char *)lParam;
	ReplaceRecvImg(pStruct, szFile);
	return 0;
}

// 替换接收到的图片
void CRichEditCtrlEx::ReplaceRecvImg(IMAGE_FILE_STRUCT *pStruct, const char * file_name)
{
	IRichEditOle *ole = GetIRichEditOle();
	int n = ole->GetObjectCount();
	REOBJECT reo;
	reo.cbStruct = sizeof(reo);
	CString strNewFile = "";
	if(file_name != NULL && strcmp(file_name, "") != 0)
	{
		strNewFile = g_LocalData.GetImageFileCachePath() + GetFileNameFromPath(file_name);
		CopyFile(file_name, strNewFile, FALSE);
	}
	
	for (int i = 0; i < n; ++i)
	{
		ole->GetObject(i, &reo, REO_GETOBJ_ALL_INTERFACES);

		if ( reo.dwUser == ODT_WAITICON )
		{
#ifndef _XY_PICTURE
			IShowImage *pImage = NULL;	
			reo.poleobj->QueryInterface(IID_IShowImage,(void **)&pImage);
#else
			IXPicture * pImage = NULL;
			reo.poleobj->QueryInterface(IID_IXPicture,(void **)&pImage);			
#endif

			//新的图片接口
			CComPtr<COleImage> spOleImage;
			reo.poleobj->QueryInterface(IID_IOleImage, (void **)&spOleImage);

			if(spOleImage)
			{
				IMAGE_FILE_STRUCT *pImage = (IMAGE_FILE_STRUCT *)(spOleImage->m_lOwnData);
				if ( pStruct == pImage )
				{
					CHARRANGE cr;
					GetSel(cr);
					SetSel(reo.cp,reo.cp+1);
					ReplaceSel("");

					if(strNewFile != "")
						InsertImage(strNewFile);
					else
					{	// 图片下载失败时，插入裂图
						CResBitmap bmp; 
						bmp.LoadBitmap(IDB_NO_IMAGE);
						InsertBitmap((HBITMAP)bmp.m_hObject);	
					}					

					SetSel(cr);

					ScrollToBottom();
				}
				//是否需要删除?
				//delete spOleImage;
				//spOleImage = NULL;

			}

			if ( pImage )
			{
				DWORD dwOwnData;
#ifndef _XY_PICTURE
				dwOwnData = (LONG)pImage->GetOwnData();
#else
				pImage->GetUserData((LONG*)&dwOwnData);
#endif
				if (dwOwnData!=0 )
				{
					IMAGE_FILE_STRUCT *pImage = (IMAGE_FILE_STRUCT *)dwOwnData;

					if ( strcmp(pImage->file_name,file_name)==0 )
					{
						CHARRANGE cr;
						GetSel(cr);
						SetSel(reo.cp,reo.cp+1);
						ReplaceSel("");
						InsertImage(strNewFile);

						SetSel(cr);

						ScrollToBottom();
					}
				}				
			}	
		}
	}
}

void CRichEditCtrlEx::SetShowFastWordMenu(BOOL bIsShow)
{
	m_bIsShowFastWordMenu = bIsShow;
}

void CRichEditCtrlEx::SetShowLinkInformation(BOOL bIsShow)
{
	m_bIsShowLinkInformation = bIsShow;
}

CString CRichEditCtrlEx::GetStoreInformation(CString strHttpConnection, CString strHttpFile)
{
	try
	{
		CInternetSession mysession;
		CHttpConnection *myconn=mysession.GetHttpConnection(strHttpConnection);
		CHttpFile *myfile=myconn->OpenRequest("GET",strHttpFile);
		myfile->SendRequest();
		CString mystr;
		CString tmp;
		while(myfile->ReadString(tmp))
		{
			mystr+=tmp;
		}
		myfile->Close();
		myconn->Close();
		mysession.Close();
		delete myfile;
		delete myconn;
		myfile=0;
		myconn=0;
		return mystr;
	}
	catch (CMemoryException* e)
	{
		return "";
	}
	catch (CFileException* e)
	{
		return "";
	}
	catch (CException* e)
	{
		return "";
	}
}

void CRichEditCtrlEx::InsertLinkInfo(CString strStoreUrl, CString strStoreLogoUrl, CString strStoreName, CString strStoreDesc)
{
	IStorage*  lpStorage  = NULL;//存储接口
	IOleObject*  lpOleObject  = NULL;//OLE对象
	LPLOCKBYTES  lpLockBytes  = NULL;//LOCKBYTE
	IOleClientSite* lpOleClientSite = NULL; 
	_DOleLinkInfo* iut = NULL;
	CLSID   clsid;
	REOBJECT  reobject;
	HRESULT   hr;
	IRichEditOle* lpRichEditOle = GetIRichEditOle();
	if(lpRichEditOle == NULL)
		return;
	//创建PolyCtl对象并获取接口HKEY_CLASSES_ROOT\Wow6432Node\CLSID
	//const GUID CDECL tlid = { 0x6289C187, 0x8A21, 0x4C65, { 0xA3, 0x7A, 0x30, 0xC5, 0x0C, 0xCB, 0x7D, 0x77} };
	//IID IID_DOleTest5 = { 0x693cc9bf, 0xb11e, 0x4ce6, { 0x82, 0x85, 0x8c, 0x0d, 0x8a, 0x49, 0x29, 0x8a} };
	//IUnknown* iu = m_o1.GetControllingUnknown(); 
	hr = ::CoCreateInstance(/*_tlid*/CLSID_OleLinkInfo,NULL,CLSCTX_INPROC,DIID__DOleLinkInfo/*IID_DOleTest10*/,(LPVOID*)&/*lpPolyCtl*/iut);
	//m_o1.m_pOuterUnknown = iu;
	if( /*lpPolyCtl*/iut == NULL )
	{
		return;
	}

	// USES_CONVERSION;
	BOOL bRet = TRUE;

	try{
		hr = /*lpPolyCtl->*/iut->QueryInterface(/*IID_DOleTest5/ *IID_DOleTest10* /,*/ /*(LPVOID *)*/&lpOleObject);//获得数据对象接口
		iut->Release();		/*QueryInterface会增加引用计数，调用后恢复一下*/	
		
		if( hr != S_OK ) 
			AfxThrowOleException(hr);
		hr = lpOleObject->GetUserClassID(&clsid);
		if ( hr != S_OK)
			AfxThrowOleException(hr);
		CString strLocalPath = strStoreLogoUrl.Right(strStoreLogoUrl.GetLength() - strStoreLogoUrl.ReverseFind('/') - 1);
		CString strDownloadFile = g_LocalData.GetImageFileCachePath()+strLocalPath;

		//图片改造
		CString strUrl = strStoreLogoUrl;
		if (strUrl.Find("http://") == -1)
		{
			strUrl = GetPicRandomUrl() + strUrl;
		}

		DownLoadWebFile(/*strStoreLogoUrl*/strUrl,strDownloadFile);
		LPCTSTR lpPic = (LPCSTR)strDownloadFile;
		
		iut->SetPicFilePath(lpPic);
		
		CString strHead = strStoreName;
		strHead.Trim();
		LPCTSTR lpHead = (LPCSTR)strHead;
		iut->SetHeadText(lpHead);

		CString strContent = strStoreDesc;
		strContent.Trim();
		LPCTSTR lpContent = (LPCSTR)strContent;
		iut->SetContentText(lpContent);

		LPCTSTR lpUrl = (LPCSTR)strStoreUrl;
		iut->SetUrlText(lpUrl);
		if( hr != S_OK ) 
			AfxThrowOleException(hr);
		
		hr = ::CreateILockBytesOnHGlobal(NULL, TRUE, &lpLockBytes);//创建LOCKBYTE对象
		if (hr != S_OK)
			AfxThrowOleException(hr);
		ASSERT(lpLockBytes != NULL);
		
		hr = ::StgCreateDocfileOnILockBytes(lpLockBytes,//创建复合文档
			STGM_SHARE_EXCLUSIVE|STGM_CREATE|STGM_READWRITE, 0, &lpStorage);
		if (hr != S_OK)
		{
			VERIFY(lpLockBytes->Release() == 0);
			lpLockBytes = NULL;
			AfxThrowOleException(hr);
		}

		lpRichEditOle->GetClientSite(&lpOleClientSite);

		ZeroMemory(&reobject, sizeof(REOBJECT));//初始化一个对象  
		reobject.cbStruct = sizeof(REOBJECT);
		reobject.clsid  = clsid;
		reobject.cp   = REO_CP_SELECTION;
		reobject.dvaspect = DVASPECT_CONTENT;
		reobject.dwFlags = REO_BELOWBASELINE;
		reobject.dwUser   = ODT_LINK_INFO;
		reobject.poleobj = lpOleObject;
		reobject.polesite = lpOleClientSite;
		reobject.pstg  = lpStorage;
		reobject.sizel.cx = 8000;
		reobject.sizel.cy = 2800;

		hr = lpRichEditOle->InsertObject( &reobject );
		if (hr != S_OK)
			AfxThrowOleException(hr);
		OleSetContainedObject(lpOleObject,TRUE);
	}

	catch( COleException* e )
	{
		TRACE(_T("OleException code:%d"),e->m_sc);
		TCHAR szError[1024];
		e->GetErrorMessage(szError, 1024);
		e->Delete();
		bRet = FALSE;
	}
	catch(_com_error& e)
	{
		//CString strMsg;
		//strMsg.Format(_T("错误描述：%s\n错误消息：%s"), 
		//	(LPCTSTR)e.Description(),
		//	(LPCTSTR)e.ErrorMessage());
		bRet = FALSE;
	}

	// release the interface
	if( lpOleObject  != NULL )  lpOleObject->Release();
	if( lpOleClientSite != NULL ) lpOleClientSite->Release();
	if( lpStorage  != NULL ) lpStorage->Release();
}

BOOL CRichEditCtrlEx::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	if(m_bIsPointInLinkRect)
		return FALSE;
	return CRichEditCtrl::OnSetCursor(pWnd, nHitTest, message);
}

void CRichEditCtrlEx::OnLButtonDown(UINT nFlags, CPoint point)
{
	if(m_bIsPointInLinkRect)
	{
		OpenHtmlPage(m_strLinkInfoUrl);
	}
	CRichEditCtrl::OnLButtonDown(nFlags, point);
}

void CRichEditCtrlEx::OnNcDestroy()
{
	if(m_pRichEditOle != NULL)
	{
		int nOleCount = m_pRichEditOle->GetObjectCount();

		REOBJECT reo;
		reo.cbStruct = sizeof(reo);

		for(int nOle = 0; nOle < nOleCount; nOle++)
		{
			m_pRichEditOle->GetObject(nOle, &reo, REO_GETOBJ_ALL_INTERFACES);

			if(reo.dwUser == ODT_LINK_INFO)
			{
				_DOleLinkInfo* iut = NULL;
				reo.poleobj->QueryInterface(DIID__DOleLinkInfo,(void **)&iut);

				if( reo.poleobj     != NULL )  reo.poleobj->Release();
				if( reo.polesite     != NULL )  reo.polesite->Release();
				if( reo.pstg     != NULL )  reo.pstg->Release();

				if( iut != NULL )	
				{	
					int nCount = iut->Release();
					while(nCount > 0)
					{
						nCount = iut->Release();
					}

					iut = NULL;
				}
			}
		}
	}
	

	//KillTimer(TIMER_CAPTURE_USERNAME);

	// 停止定时器，这不是必要的  
	KillTimer(FRM_TIMER_ID);  
	// 清空内容，目的是删除所有插入的COleImage对象  
	SetWindowText(_T(""));  
	// 但是别忘了，RichEdit支持多级撤消，对象虽然删除了，但是撤消缓存  
	// 中还保留有对象的引用，清空撤消缓存才能真正的释放这些对象  
	EmptyUndoBuffer();  

	CRichEditCtrl::OnNcDestroy();
}
