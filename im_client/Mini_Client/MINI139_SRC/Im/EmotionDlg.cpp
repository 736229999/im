// DialogEmotion.cpp : 实现文件
//

#include "stdafx.h"
#include "EmotionDlg.h"
#include "LocalData.h"
#include "emotionmanagedlg.h"
#include "AddEmotionDlg.h"
#include "ImageFileDialog.h"

#define _SIMPLE_EMOTION_

#define			ID_MODIFY_EMOTION		10001
#define			ID_DEL_EMOTION			10002
#define			ID_NEW_FACE				10003			
#define			ID_NEW_GROUP			10004
#define			ID_MOD_GROUPNAME		10005
#define			ID_DEL_GROUP			10006
#define			ID_MOVE_FACE_BEG		10100		
#define			ID_MOVE_FACE_END		10200
#define			ID_CMD_ENTER			10400

#define			IDC_LABEL_GROUP			20001


#define			EMOTION_WIDTH			32
#define			EMOTION_HEIGHT			32
#define			EMOTION_PERLINE			8
#define			EMOTION_ROW				7
#define			EMOTION_SIZE			28//25
#define			EMOTION_TITLE_HEIGHT	18
#define			BUTTON_HEIGHT			26
#define			LEFT_MARGIN				1
#define			RIGHT_MARGIN			2
#define			BOTTOM_MARGIN			4
#define			TOP_MARGIN				6
#define			LABEL_SPAN				2
#define			GROUP_LABEL_BUTTON_WIDTH 15
#define			EMOTION_GRID_COLOR		RGB(200,200,200)
const int		EMOTION_PAGECOUNT	=	EMOTION_ROW*EMOTION_PERLINE;
const int		EMOTION_TOTAL_WIDTH =	EMOTION_PERLINE * EMOTION_WIDTH + LEFT_MARGIN + RIGHT_MARGIN;
const int		EMOTION_TOTAL_HEIGHT = EMOTION_ROW*EMOTION_HEIGHT + BUTTON_HEIGHT + BOTTOM_MARGIN + EMOTION_TITLE_HEIGHT + TOP_MARGIN;
const COLORREF	CLR_FRAME  = RGB(0,0,0);//RGB(107,134,222);
const COLORREF	CLR_BORDER = RGB(150,150,150);

const UINT		TIMER_PREVIEW = 2001;

const char SZ_CUSTOM_FACE[] = "自定义表情";
const char SZ_UPDATE[]		= "从网站更新";
const char SZ_PRE[]			= "<<上一页";
const char SZ_NEXT[]		= "下一页>>";

CUSTOM_EMOTION_DB  g_customEmotionDB;

void WriteCustomEmotionConfig(CStdioFile& file)
{
	file.WriteString("<?xml version=\"1.0\" encoding=\"gb2312\" ?>\n");
	file.WriteString("<config>\n");
	char szType[2]={0};
	int nGroup = g_customEmotionDB.size();
	int nItems;
	int i,j;
	int type;
	CString sGroup;
	for ( i = 0; i < nGroup; i++ )
	{
		if(i == 1)
			continue;	// 过滤其他(系统gif）
		if(i == 0)
			sGroup.Format("<default>\n");
		else
			sGroup.Format("<group name=\"%s\">\n",g_customEmotionDB[i].sName);
		file.WriteString(sGroup);
		nItems = g_customEmotionDB[i].size();
		for(j = 0; j < nItems; j++)
		{
			type = g_customEmotionDB[i][j].type;
			if(type == CUSTOM_EMOTION::SYS || type == CUSTOM_EMOTION::SYS_GIF)
				continue;
			file.WriteString("<face>\n");
			file.WriteString("<shortcut>");
			file.WriteString(g_customEmotionDB[i][j].szShortCut);
			file.WriteString("</shortcut>");
			file.WriteString("<tip>");
			file.WriteString(g_customEmotionDB[i][j].szTip);
			file.WriteString("</tip>");
			file.WriteString("<file>");
			file.WriteString(g_customEmotionDB[i][j].szFile);
			file.WriteString("</file>");
			file.WriteString("<type>");
			szType[0]=g_customEmotionDB[i][j].type+'0';
			file.WriteString(szType);
			file.WriteString("</type>");
			file.WriteString("<id>");
			file.WriteString(g_customEmotionDB[i][j].szID);
			file.WriteString("</id>");
			file.WriteString("<url>");
			file.WriteString(g_customEmotionDB[i][j].szUrl);
			file.WriteString("</url>");
			file.WriteString("</face>\n");
		}
		if(i == 0)
			file.WriteString("</default>\n");
		else
			file.WriteString("</group>\n");
	}
	file.WriteString("</config>");
}

void SaveCustomEmotionConfig()
{
	// 首先获得自定义表情配置文件
	CString strCustomFacePath = g_LocalData.GetCustomFacePath();
	CString strCustomConfigFile = strCustomFacePath+"customface.cfg.xml";
	CStdioFile file;
	bool bOpenFlag;
	// 重建配置文件
	bOpenFlag = file.Open(strCustomConfigFile,CFile::modeCreate|CFile::modeWrite|CFile::typeText);
	if(bOpenFlag)
	{
		WriteCustomEmotionConfig(file);
		file.Close();
	}
}

// 从xml配置文件加载某组表情文件
void GetGroupCustomFace(MSXML2::IXMLDOMNodePtr nodeGroupPtr,CUSTOM_EMOTION_GROUP & group)
{
	MSXML2::IXMLDOMNodePtr			nodePtr;
	MSXML2::IXMLDOMNodeListPtr		faceListPtr,infoListPtr;
	MSXML2::IXMLDOMNamedNodeMapPtr	attrsPtr;

	int i;
	int nC;
	int nCInfo;
	CUSTOM_EMOTION face;

	// 获取组名
	attrsPtr = nodeGroupPtr->attributes;
	nodePtr = attrsPtr->getNamedItem("name");
	if(nodePtr)
	{
		group.sName = (const char *)nodePtr->text;
	}
	else
	{
		group.sName = "";
	}
	// 获取表情项
	faceListPtr = nodeGroupPtr->selectNodes("face");
	nC = faceListPtr->length;
	for(i = 0;i < nC; i++)
	{
		nodePtr = faceListPtr->item[i];
		infoListPtr = nodePtr->childNodes;
		nCInfo = infoListPtr->length;
		// 至少需要3个配置项
		if ( nCInfo > 2 )
		{
			strcpy(face.szShortCut,(const char *)infoListPtr->item[0]->text);
			strcpy(face.szTip,(const char *)infoListPtr->item[1]->text);
			strcpy(face.szFile,(const char *)infoListPtr->item[2]->text);// 文件路径	
		}
		if( nCInfo > 3)
		{
			// 类型
			char c = *(const char *)infoListPtr->item[3]->text;
			if(c != 0)
				face.type = c - '0';
			else
				face.type = 0;
		}
		if( nCInfo > 4)
		{
			// ID
			strncpy(face.szID,(const char *)infoListPtr->item[4]->text,sizeof(face.szID)-1);
		}
		if( nCInfo > 5)
		{
			// URL
			strncpy(face.szUrl,(const char *)infoListPtr->item[5]->text,sizeof(face.szUrl)-1);
		}
		group.push_back(face);
	}

}

// 从xml配置文件加载表情文件
void LoadCustomFace()
{
	//MSXML2::IXMLDOMDocumentPtr docPtr;	
	//docPtr.CreateInstance("msxml2.domdocument");
	//VARIANT_BOOL bSuccess = false;

	//CString strCustomConfigFile = g_LocalData.GetCustomFacePath()+"customface.cfg.xml";
	////CString strCustomConfigFile = "customface.cfg.xml";

	//VARIANT var;
	//var.vt=VT_BSTR;
	//var.bstrVal = strCustomConfigFile.AllocSysString();
	//docPtr->load(var);
	//SysFreeString(var.bstrVal);

	int nFaceId = 0;
	int nGroup = 0;
	int nC;
	int i;
	CUSTOM_EMOTION_GROUP group;
	// 初始化系统自带表情
	CUSTOM_EMOTION face;
	/*
	face.type = CUSTOM_EMOTION::SYS;
	nC = CRichEditCtrlEx::emotion_count;
	for(i = 0; i < nC;i++)
	{
		strcpy(face.szShortCut,CRichEditCtrlEx::emotions[i]);
		strcpy(face.szTip,CRichEditCtrlEx::emotion_tips[i]);
		face.id = i;
		group.push_back(face);
	}
	*/

	// 清空
	g_customEmotionDB.clear();
	// 先添加默认
	//不需要之前的静态的表情，不好看；zhangmin
	//g_customEmotionDB.push_back(group);
	//

	group.clear();
	group.sName = "系统";

	static LPCSTR  szEmotionTip[] ={
		"好的", "悲伤", "鄙视", "大笑", "撒钱", "恭喜", "鼓掌", "呼唤","奋斗", "胜利",
		"惊讶", "开心", "可怜", "祈祷", "礼物", "阴险", "流汗", "大哭", "示爱","玫瑰",
		"凋谢", "赞", "吻", "发怒","酷", "太阳", "困", "月亮", "偷笑", "狂吐", 
		"调皮", "握手", "发呆", "思考", "色", "下雨", "心碎", "晕", "疑问", "喝彩", 
		"谢谢", "拜拜", "无聊", "唱歌", "奔跑", "大怒", "祝福", "加油", "惊喜", "无奈",
		"扔番茄", "讨厌", "吐舌头", "伤心", "晕"
	}; 

	face.szShortCut[0] = 0;
	face.szTip[0] = 0;
	face.type = CUSTOM_EMOTION::SYS_GIF;
	for ( i = 1; i <= GIF_EMOTION_COUNT; i++ )
	{
		face.id = i;
		strcpy(face.szFile, (LPCTSTR)g_LocalData.GetGifFacePath(i));
		strcpy(face.szTip, szEmotionTip[i-1]); 
		group.push_back(face);
	}
	g_customEmotionDB.push_back(group);


	//MSXML2::IXMLDOMNodeListPtr nodesPtr;
	//MSXML2::IXMLDOMNodePtr nodeCfgPtr;
	//nodesPtr = docPtr->selectNodes("config");
	//if(nodesPtr->length < 1)
	//{
	//	return;
	//}
	//// 解析config
	//nodeCfgPtr = nodesPtr->item[0];
	//ASSERT(nodeCfgPtr != NULL);
	//if(nodeCfgPtr)
	//{
	//	nodesPtr = nodeCfgPtr->selectNodes("default");
	//	if(nodesPtr->length > 0)
	//	{
	//		GetGroupCustomFace(nodesPtr->item[0],g_customEmotionDB[0]);
	//	}
	//}
	//nodesPtr = nodeCfgPtr->selectNodes("group");
	//nC = nodesPtr->length;
	//for(i = 0;i < nC;i++)
	//{
	//	group.clear();
	//	GetGroupCustomFace(nodesPtr->item[i],group);
	//	g_customEmotionDB.push_back(group);
	//}
}
//void LoadCustomFace()
//{
//	
//	CComPtr<IXMLDOMDocument> pDoc;	
//	pDoc.CoCreateInstance(__uuidof(DOMDocument));
//	const char* p=NULL;
//
//	VARIANT_BOOL bSuccess = false;
//
//	CString strCustomConfigFile = g_LocalData.GetCustomFacePath()+"customface.cfg.xml";
//
//	VARIANT var;
//	var.vt=VT_BSTR;
//	var.bstrVal = strCustomConfigFile.AllocSysString();
//	pDoc->load(var,&bSuccess);
//	SysFreeString(var.bstrVal);
//
//	int nFaceId=0;
//
//	g_customEmotionDB.clear();
//
//	while (1)
//	{oti
//		char szItem[255];
//		sprintf(szItem,"config/face[%d]/*",nFaceId++);
//
//		CComBSTR bstr(szItem); 
//		CComPtr<IXMLDOMNodeList> pNodes; 
//
//		pDoc->selectNodes(bstr,&pNodes);
//
//		if ( pNodes )
//		{
//			long nNodes; pNodes->get_length(&nNodes);
//
//			if ( nNodes>=3 )
//			{
//				CUSTOM_EMOTION cFace;
//
//				CComPtr<IXMLDOMNode> pNode = NULL;
//
//				pNodes->get_item(0, &pNode);
//
//				USES_CONVERSION;
//
//				pNode->get_text(&bstr);//shortcut
//				sprintf(cFace.szShortCut,W2A(bstr));
//				pNode.Release();
//
//				pNodes->get_item(1, &pNode);//tip
//				pNode->get_text(&bstr);
//				sprintf(cFace.szTip,W2A(bstr));
//				pNode.Release();
//
//				pNodes->get_item(2, &pNode);	// 文件路径
//				pNode->get_text(&bstr);
//				sprintf(cFace.szFile,W2A(bstr));
//				pNode.Release();
//
//				pNodes->get_item(3,&pNode);	// 类型
//				if(pNode)
//				{
//					pNode->get_text(&bstr);
//					p=W2A(bstr);
//					if(p && ((*p)!=0))
//					{
//						cFace.type=*p-'0';
//					}
//					else
//						cFace.type=0;
//					pNode.Release();
//				}
//
//				pNodes->get_item(4,&pNode);	// ID
//				if(pNode)
//				{
//					pNode->get_text(&bstr);
//					strncpy(cFace.szID,W2A(bstr),sizeof(cFace.szID)-1);
//					//sprintf(cFace.szID,W2A(bstr));
//					pNode.Release();
//				}
//				pNodes->get_item(5,&pNode);	// URL
//				if(pNode)
//				{
//					pNode->get_text(&bstr);
//					strncpy(cFace.szUrl,W2A(bstr),sizeof(cFace.szUrl)-1);
//					pNode.Release();
//				}
//				g_customEmotionDB.push_back(cFace);
//			}
//			else
//			{
//				break;
//			}
//		}
//	}
//}
// CEmotionDlg 对话框
#include <ocidl.h> 
#include <olectl.h> 
#include ".\emotiondlg.h"
//
//
//HRESULT DrawPicture(char *lpstrFile,CDC *pDC,int nScrWidth,int nScrHeight) 
//{ 
//IPicture *pPic; 
//IStream *pStm; 
//
//BOOL bResult; 
//
//HANDLE hFile=NULL; 
//DWORD dwFileSize,dwByteRead; 
//
////打开硬盘中的图形文件 
//hFile=CreateFile(lpstrFile,GENERIC_READ, 
//FILE_SHARE_READ,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL); 
//
//if (hFile!=INVALID_HANDLE_VALUE) 
//{ 
//	dwFileSize=GetFileSize(hFile,NULL);//获取文件字节数 
//
//	if (dwFileSize==0xFFFFFFFF) 
//	return E_FAIL; 
//} 
//else 
//{ 
//	return E_FAIL; 
//}
//} 
//
////分配全局存储空间 
//HGLOBAL hGlobal = GlobalAlloc(GMEM_MOVEABLE, dwFileSize); 
//LPVOID pvData = NULL; 
//
//if (hGlobal == NULL) 
//return E_FAIL; 
//
//if ((pvData = GlobalLock(hGlobal)) == NULL)//锁定分配内存块 
//return E_FAIL; 
//
//ReadFile(hFile,pvData,dwFileSize,&dwByteRead,NULL);//把文件读入内存缓冲区 
//
//GlobalUnlock(hGlobal); 
//
//CreateStreamOnHGlobal(hGlobal, TRUE, &pStm); 
//
////装入图形文件 
//bResult=OleLoadPicture(pStm,dwFileSize,TRUE,IID_IPicture,(LPVOID*)&pPic); 
//
//if(FAILED(bResult)) 
//return E_FAIL; 
//
//OLE_XSIZE_HIMETRIC hmWidth;//图片的真实宽度 
//OLE_YSIZE_HIMETRIC hmHeight;//图片的真实高度 
//pPic->get_Width(&hmWidth); 
//pPic->get_Height(&hmHeight); 
//
//
////将图形输出到屏幕上（有点像BitBlt） 
//bResult=pPic->Render(pDC->m_hDC,0,0,nScrWidth,nScrHeight, 
//0,hmHeight,hmWidth,-hmHeight,NULL); 
//
//pPic->Release(); 
//
//CloseHandle(hFile);//关闭打开的文件 
//
//if (SUCCEEDED(bResult)) 
//{ 
//return S_OK; 
//} 
//else 
//{ 
//return E_FAIL; 
//} 
//} 
 
IMPLEMENT_DYNAMIC(CEmotionDlg, CDialog)
CEmotionDlg::CEmotionDlg(int posx, int posy, CImageList *pImgList, CWnd* pParent /*=NULL*/)
	: CDialog(CEmotionDlg::IDD, pParent)
{
	m_posx = posx;
	m_posy = posy;
	m_pImgList = pImgList;
	m_hEmotionSelWnd = NULL;

	m_nImgHotOver = -1 ;
	m_nImgPre     = -1 ;
	m_iBtn        = -1 ;
	m_nEditGroup = -1;
	m_nLoaded = 0;

	m_nCurPageCount  = 0  ;
	m_nCurGroup = 0;
	m_nCurPage = 0;
	m_pWndPreview = NULL;
	m_pImage = NULL;
	m_pPropertyItem = NULL;
	m_bShowPreview = false;
	m_pGroupLbl = NULL;
	m_bEditLabel = false;
	

	Create(IDD_DIALOG_EMOTION,pParent);
	
}

CEmotionDlg::~CEmotionDlg()
{
	if(m_pGroupLbl)
	{
		delete m_pGroupLbl;
		m_pGroupLbl = NULL;
	}
	if(m_pImage)
	{
		delete m_pImage;
	}
	if(m_pWndPreview)
	{
		delete m_pWndPreview;
	}
	if(m_pPropertyItem)
	{
		delete m_pPropertyItem;
	}

	CustomFaceMapIt it;

	for ( it=m_mapCustomFace.begin(); it!=m_mapCustomFace.end(); it++ )
	{
		if(it->second)
			delete (CBitmap *)it->second;
	}

	map<int,CBitmap *>::iterator it2;

	for ( it2 = m_mapGifEmotion.begin(); it2!=m_mapGifEmotion.end(); it2++ )
	{
		if(it2->second)
			delete (CBitmap *)it2->second;
	}

	m_mapCustomFace.clear();
	m_mapGifEmotion.clear();
}

void CEmotionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
//	DDX_Control(pDX,IDC_STATIC_NEXT,m_btnNext);
//	DDX_Control(pDX,IDC_STATIC_PREV,m_btnPre);
//	DDX_Control(pDX,IDC_STATIC_ADD,m_btnAdd);
}


BEGIN_MESSAGE_MAP(CEmotionDlg, CDialog)
	ON_WM_NCPAINT()
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_KILLFOCUS()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_NCDESTROY()
	ON_WM_TIMER()
	ON_WM_RBUTTONUP()

	ON_MESSAGE(DM_GETDEFID,OnGetDefID)

	ON_EN_CHANGE(IDC_LABEL_GROUP,OnEditChange)
	ON_EN_KILLFOCUS(IDC_LABEL_GROUP, OnEnKillfocusEditLabel)

	ON_COMMAND(ID_CMD_ENTER,OnEnter)
	ON_COMMAND(ID_MODIFY_EMOTION,OnModifyEmotion)
	ON_COMMAND(ID_DEL_EMOTION,OnDelEmotion)
	ON_COMMAND(ID_DEL_GROUP,OnDelGroup)
	ON_COMMAND(ID_NEW_GROUP,OnNewGroup)
	ON_COMMAND(ID_MOD_GROUPNAME,OnModGroupName)
	ON_COMMAND(ID_NEW_FACE,OnNewFace)
	ON_COMMAND_RANGE(ID_MOVE_FACE_BEG,ID_MOVE_FACE_END,OnMoveFace)
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()

LRESULT CEmotionDlg::OnGetDefID(WPARAM wp, LPARAM lp)
{
	return MAKELONG(ID_CMD_ENTER,DC_HASDEFID);
}

bool CEmotionDlg::IsAnimatePic(Image* pImage,int & nFrameCount)
{
	if(pImage == NULL)
	{
		nFrameCount = 0;
		return false;
	}
	UINT count = 0;
	// 获得图片的维数
	count = pImage->GetFrameDimensionsCount();
	GUID* pDimensionIDs = new GUID[count];
	// 获得图元列表
	pImage->GetFrameDimensionsList(pDimensionIDs, count);
	// 首层的帧数
	nFrameCount = pImage->GetFrameCount(&pDimensionIDs[0]);

	// 获取属性(PropertyTagFrameDelay)的大小
	int nSize = pImage->GetPropertyItemSize(PropertyTagFrameDelay);
	// 为属性分配空间
	if(m_pPropertyItem)
	{
		delete m_pPropertyItem;
	}
	m_pPropertyItem = NULL;
	m_pPropertyItem = (PropertyItem*) new char[nSize];
	// 获取属性
	pImage->GetPropertyItem(PropertyTagFrameDelay, nSize, m_pPropertyItem);

	delete  pDimensionIDs;
	return (nFrameCount > 1);
}

void CEmotionDlg::InitAnimation()
{
	m_nFramePosition = 0;

	long nTime = ((long*) m_pPropertyItem->value)[m_nFramePosition] * 12;
	if(nTime < 10)
	{
		nTime = 100;
	};
	SetTimer(TIMER_PREVIEW,nTime,NULL);

}

void CEmotionDlg::ShowPreview(bool bShow /* = true */)
{
	m_bShowPreview = bShow;
}

void CEmotionDlg::LoadPreviewImage(const CString & sImage)
{
	m_nFramePosition = 0;
	m_nFrameCount = 0;
	m_bAnimate = false;

	BSTR bName = sImage.AllocSysString();
	if(m_pImage)
	{
		delete m_pImage;
	}
	m_pImage = Image::FromFile(bName);
	::SysFreeString(bName);
	m_bAnimate = IsAnimatePic(m_pImage,m_nFrameCount);
	if(m_bAnimate)
	{
		InitAnimation();
	}
	
}

void CEmotionDlg::DrawPreview(CDC & dc)
{
	Graphics g(dc.m_hDC);
	Rect rc(m_rcPreview.left + 3,m_rcPreview.top + 3,m_rcPreview.Width() - 4,m_rcPreview.Height() - 4);
	Rect rcImage = rc;
	Pen pen(Color(0,0,255),1);
	SolidBrush brush(Color(255,255,255));
	g.FillRectangle(&brush,rc);
	if(m_pImage)
	{
		// 将图片中间对齐
		int nX = 0,nY = 0;
		int nW,nH;
		float fW,fH;
		fW = m_pImage->GetWidth();
		fH = m_pImage->GetHeight();
		float fWn,fHn;
		fWn = fW / rc.Width;
		fHn = fH / (rc.Height-22);

		//
		// 先判断是否需要缩放
		//

		// 如果图片大于显示区域
		if(fWn > 1 || fHn > 1)
		{
			// 宽的缩放系数比高大
			if(fWn > fHn)
			{
				nW = rc.Width;			// 以预览区宽度作宽
				nH = fH / fWn;			// 高缩短
			}
			// 宽的缩放系数比高小
			else
			{
				nH = rc.Height-22;			// 以预览区高度作高
				nW = fW / fHn;			// 宽缩小
			}
		}
		else
		{
			nW = m_pImage->GetWidth();
			nH = m_pImage->GetHeight();
		}

		//
		// 然后居中
		//

		nX = (rc.Width - nW) / 2;
		nY = (rc.Height - 22 - nH) / 2;
		rcImage.X += nX;
		rcImage.Y += nY;
		rcImage.Width = nW;
		rcImage.Height = nH;
		g.DrawImage(m_pImage,rcImage,0,0,m_pImage->GetWidth(),m_pImage->GetHeight(),UnitPixel);

		// 绘制文本背景色
		RectF textRect(rc.GetLeft()+1, rc.GetBottom()-22, (REAL)rc.GetRight()-rc.GetLeft()-2, (REAL)22);
		Color colorBk(0xee, 0xee, 0xee);
		SolidBrush brushBk(colorBk); 
		g.FillRectangle(&brushBk, textRect);

		// 绘制文本
		USES_CONVERSION;
		wstring mTextValue = A2W(g_customEmotionDB[m_nCurGroup][m_nImgHotOver].szTip);
		LOGFONT LogFont;
		GetFont()->GetLogFont(&LogFont);
		Font nFont(dc.m_hDC, &LogFont);
		StringFormat format;
		format.SetAlignment(StringAlignmentCenter);
		format.SetLineAlignment(StringAlignmentCenter);
		Color color(0x2e, 0x2e, 0x2e);
		SolidBrush brush(color); 
		g.DrawString(mTextValue.c_str(), mTextValue.length(), &nFont, textRect, &format, &brush);
	}
	g.DrawRectangle(&pen,rc);
}

void CEmotionDlg::OnPaint()
{
	CPaintDC dc(this); 

	CDC dcBack;
	CBitmap bmpBack;
	CPen penGrid(PS_SOLID,1,EMOTION_GRID_COLOR);
	CPen penBorder(PS_SOLID,1,CLR_BORDER);

	CRect rcClient;
	GetClientRect(&rcClient);

	dcBack.CreateCompatibleDC(&dc);
	bmpBack.CreateCompatibleBitmap(&dc,rcClient.Width(),rcClient.Height());

	CBitmap *	pOldbmp = dcBack.SelectObject(&bmpBack);
	CFont *	pOldfont = dcBack.SelectObject(GetFont());
	CPen *	pOldPen = dcBack.SelectObject(&penBorder);
	dcBack.FillSolidRect(rcClient, RGB(255,255,255));
	dcBack.SetBkMode(TRANSPARENT);
	
	int i;

	// 绘制组标签
	int nC = m_arrGroup.size();
	int nTop;
	int nLeft = 0;
	CRect rc;
	CPoint ptRound(5,5);
	int nXDelta = 0;
	int nX1 = LEFT_MARGIN ,nX2 = LEFT_MARGIN;
	rc = m_arrGroup[m_arrGroup.m_nFirstGroup].rc;
	// 相对差额
	nXDelta = rc.left - m_arrGroup[0].rc.left;
	for(i = m_arrGroup.m_nFirstGroup; i < nC; i++ )
	{
		rc = m_arrGroup[i].rc;
		rc.left = rc.left - nXDelta;
		rc.right = rc.right - nXDelta;
		// 选中的标签需要放大
		if(i == m_nCurGroup)
		{ 
			rc.top -= 3;
			if(i == 0)
				rc.left = LEFT_MARGIN + 1;	// 选中的标签贴近身边的标签
			else
				rc.left = rc.left - LABEL_SPAN - 1;
			rc.right += LABEL_SPAN + 1;
			nTop = rc.top + 6;
			nX1 = rc.left;
			nX2 = rc.right;
			nLeft = rc.left + 5 + LABEL_SPAN;
		}
		else
		{
			nTop = rc.top + 3;
			nLeft = rc.left + 5;
		}
		if(nLeft > rcClient.right - RIGHT_MARGIN - GROUP_LABEL_BUTTON_WIDTH)
		{
			break;
		}
		rc.bottom += 4;
		// 绘制标签
		dcBack.RoundRect(rc,ptRound);
		if(i == m_nCurGroup && m_bEditLabel)
		{
			
		}
		else
		{
			if(m_arrGroup[i].sName != "")
				dcBack.TextOut(nLeft,nTop,m_arrGroup[i].sName);
			else
				dcBack.TextOut(nLeft,nTop,"<空>");
		}
		
	}

	if ( nC==2 )
	{
		COLORREF clr=dcBack.GetTextColor();
		dcBack.SetTextColor(RGB(122,122,122));
		char szText[]="温馨提示：鼠标右键有更多设置";
		CSize size=dcBack.GetTextExtent(szText);		
		dcBack.TextOut(rcClient.right-size.cx-3,nTop,szText);
		dcBack.SetTextColor(clr);
	}

	rc = m_arrGroup[m_arrGroup.m_nFirstGroup].rc;
	rc.bottom += 4;
	rc.top = rc.bottom - 2;
	rc.left = rcClient.left;
	rc.right = rcClient.right;
	dcBack.FillSolidRect(rc,RGB(255,255,255));


	dcBack.SelectObject(&penGrid); 

	// 绘制边框和窗体边框
	CRect rcBorder;
	rcBorder.top = rcClient.top + TOP_MARGIN + EMOTION_TITLE_HEIGHT - 1;
	rcBorder.left = rcClient.left + LEFT_MARGIN + 1;
	rcBorder.bottom = rcBorder.top + EMOTION_ROW * EMOTION_HEIGHT;
	rcBorder.right = rcBorder.left + EMOTION_PERLINE * EMOTION_WIDTH - 2;
	dcBack.Draw3dRect(&rcClient,CLR_FRAME,CLR_FRAME);
	
	// 绘制当前页中的表情图片
	EMOTION_ID id;
	id.nGroup = m_nCurGroup;
	int nCount = g_customEmotionDB[m_nCurGroup].size();
	int nBeg = m_nCurPage * EMOTION_PAGECOUNT;
	int nEnd = (m_nCurPage + 1) * EMOTION_PAGECOUNT;
	if(nEnd > nCount)
		nEnd = nCount;
	for(i = nBeg; i < nEnd;i++)
	{
		CUSTOM_EMOTION & emotion = g_customEmotionDB[m_nCurGroup][i];
		if(emotion.type == CUSTOM_EMOTION::SYS)
		{
			DrawEmotion(emotion.id, ILD_NORMAL, &dcBack);
		}
		else if(emotion.type == CUSTOM_EMOTION::SYS_GIF)
		{
			DrawGifEmotion(emotion.id ,&dcBack);
		}
		else
		{
			id.nItem = i;
			DrawCustomFace(id,&dcBack);
		}
		if(i == m_nImgHotOver)
		{
			CRect rcTemp;	
			GetEmotionRect(m_nImgHotOver,rcTemp);
			dcBack.Draw3dRect(rcTemp,RGB(0,0,255),RGB(0,0,255));
		}
	}

	// 绘制行线
	for (i = 1; i < EMOTION_ROW; i++ )
	{
		dcBack.MoveTo(LEFT_MARGIN + 1 ,i*EMOTION_HEIGHT + EMOTION_TITLE_HEIGHT + TOP_MARGIN);
		dcBack.LineTo(rcClient.right - RIGHT_MARGIN - 1,i*EMOTION_HEIGHT + EMOTION_TITLE_HEIGHT + TOP_MARGIN);
	}
	// 绘制列线
	for (i = 1; i< EMOTION_PERLINE; i++ )
	{
		dcBack.MoveTo(LEFT_MARGIN + i*EMOTION_WIDTH,EMOTION_TITLE_HEIGHT + TOP_MARGIN + 2);
		dcBack.LineTo(LEFT_MARGIN + i*EMOTION_WIDTH,EMOTION_ROW*EMOTION_HEIGHT + EMOTION_TITLE_HEIGHT + TOP_MARGIN - 2);
	}
	

	// 开始绘制表情框
	dcBack.SelectObject(&penBorder);
	// 左竖线
	dcBack.MoveTo(rcBorder.left,rcBorder.top);
	dcBack.LineTo(rcBorder.left,rcBorder.bottom);
	// 右竖线
	dcBack.MoveTo(rcBorder.right,rcBorder.top);
	dcBack.LineTo(rcBorder.right,rcBorder.bottom);
	// 下横线
	dcBack.MoveTo(rcBorder.left,rcBorder.bottom);
	dcBack.LineTo(rcBorder.right,rcBorder.bottom);

	// 绘制上横行
	dcBack.MoveTo(rcBorder.left,rcBorder.top);	// (左到选中标签)
	dcBack.LineTo(nX1,rcBorder.top);
	dcBack.MoveTo(nX2,rcBorder.top);
	dcBack.LineTo(rcBorder.right,rcBorder.top);	// (选中标签到右边)

	// 绘制预览图片
	if(m_bShowPreview)
	{
		if(m_typePreview == CUSTOM_EMOTION::SYS)
		{
			DrawSysPreview(m_nImgHotOver,&dcBack);
		}
		else if(m_typePreview == CUSTOM_EMOTION::SYS_GIF)
		{
			//DrawSysGifPreview(m_nImgHotOver + 1,&dcBack);
			DrawPreview(dcBack);
		}
		else
		{
			DrawPreview(dcBack);
		}
	}
	
#ifndef _SIMPLE_EMOTION_

	// 绘制自定义表情按钮
	if ( m_iBtn==IDC_LINK_ADD )
		dcBack.SetTextColor(RGB(0,0,255));
	else
		dcBack.SetTextColor(RGB(0,80,122));
	
	dcBack.DrawText(SZ_CUSTOM_FACE, strlen(SZ_CUSTOM_FACE), &m_rcBtnAdd, DT_VCENTER);
	
	// 绘制从网站更新按钮
	if ( m_iBtn==IDC_LINK_UPDATE )
		dcBack.SetTextColor(RGB(0,0,255));
	else
		dcBack.SetTextColor(RGB(0,80,122));
	dcBack.DrawText(SZ_UPDATE,strlen(SZ_UPDATE),&m_rcBtnUpdate,DT_VCENTER);
	
	// 绘制上一页按钮
	if ( m_iBtn==IDC_LINK_PRE )
		dcBack.SetTextColor(RGB(0,0,255));
	else
		dcBack.SetTextColor(RGB(0,80,122));
	if ( m_nCurPageCount < 2 || m_nCurPage == 0 )
		dcBack.SetTextColor(RGB(133,133,133));
	dcBack.DrawText(SZ_PRE, strlen(SZ_PRE), &m_rcBtnPrev, DT_VCENTER);
	// 绘制下一页按钮
	if ( m_iBtn==IDC_LINK_NEXT )
		dcBack.SetTextColor(RGB(0,0,255));
	else
		dcBack.SetTextColor(RGB(0,80,122));
	if ( m_nCurPageCount < 2 || m_nCurPage == (m_nCurPageCount-1) )
		dcBack.SetTextColor(RGB(133,133,133));
	dcBack.DrawText(SZ_NEXT, strlen(SZ_NEXT), &m_rcBtnNext, DT_VCENTER);
	// 绘制当前页数
	dcBack.SetTextColor(RGB(188,80,100));
	dcBack.SetTextAlign(TA_RIGHT);
	CString strPageInfo;
	strPageInfo.Format("第%d页/共%d页 ",m_nCurPage + 1,m_nCurPageCount);
	dcBack.TextOut(m_rcBtnPrev.left,m_rcBtnAdd.top,strPageInfo);


#endif

	dc.BitBlt(0,0,rcClient.Width(),rcClient.Height(),
		&dcBack,0,0,
		SRCCOPY);

	dcBack.SelectObject(pOldbmp);
	dcBack.SelectObject(pOldfont);
	dcBack.SelectObject(pOldPen);
}

BOOL CEmotionDlg::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
}

void CEmotionDlg::OnKillFocus(CWnd* pNewWnd)
{
	CDialog::OnKillFocus(pNewWnd);
	m_bShowPreview = false;
	RememberPostion();
	// 防止gid资源泄露
	if(m_nImgHotOver != -1)
	{
		m_nImgHotOver = -1;
		OnCancel();
	}
	else
	{
		DestroyWindow();
	}
}

BOOL CEmotionDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	
	

	// 获取上次选择的表情分组和分页位置
	CString sCfg;
	sCfg.Format("%sdata\\%s\\customface.ini",(LPCTSTR)g_LocalData.GetPath(),g_ImUserInfo./*GetUserName*/GetUserFileName().c_str());
	m_nCurGroup = ::GetPrivateProfileInt("LastSelected","group",0,sCfg);
	m_nCurPage = ::GetPrivateProfileInt("LastSelected","page",0,sCfg);
	
	int screen_cx = GetSystemMetrics(SM_CXSCREEN)-25;
	int screen_cy = GetSystemMetrics(SM_CYSCREEN)-25;

	// 以指定弹出位置为下中角或上中角
	CRect rcWnd;
	// 优先弹出位置为下中角；假如上方空间不够的情况下才设为上中角
	if(m_posy < EMOTION_TOTAL_HEIGHT)
	{
		// 上中角
		rcWnd.top = m_posy;
	}
	else
	{
		// 下中角
		rcWnd.top = m_posy - EMOTION_TOTAL_HEIGHT;
	}

	rcWnd.left = m_posx - EMOTION_TOTAL_WIDTH / 2;
	if(rcWnd.left < 0)
		rcWnd.left = 0;
	// 窗口宽度
	rcWnd.right = rcWnd.left + EMOTION_TOTAL_WIDTH;
	// 窗口高度
	rcWnd.bottom = rcWnd.top + EMOTION_TOTAL_HEIGHT;

	CDC *pDC = GetDC();
    CFont *pFont = GetFont();

	CFont *pOldFont = pDC->SelectObject(pFont);

	CSize sz = pDC->GetTextExtent(SZ_CUSTOM_FACE);
	int topMargin = (BUTTON_HEIGHT - sz.cy)/2;

	// 添加按钮的区域
	m_rcBtnAdd.left = LEFT_MARGIN;
	m_rcBtnAdd.right = sz.cx+m_rcBtnAdd.left;
	m_rcBtnAdd.top = EMOTION_ROW * EMOTION_HEIGHT + EMOTION_TITLE_HEIGHT + TOP_MARGIN + topMargin;
	m_rcBtnAdd.bottom = m_rcBtnAdd.top + BUTTON_HEIGHT - topMargin;
    // 从网站更新按钮的区域
	sz = pDC->GetTextExtent(SZ_UPDATE);
	m_rcBtnUpdate.left = m_rcBtnAdd.right+10;
	m_rcBtnUpdate.right=sz.cx+m_rcBtnUpdate.left;
	m_rcBtnUpdate.top=m_rcBtnAdd.top;
	m_rcBtnUpdate.bottom=m_rcBtnAdd.bottom;

	int cx1 = pDC->GetTextExtent(SZ_NEXT).cx;

	m_rcBtnNext.left = rcWnd.Width()- cx1 - RIGHT_MARGIN;
	m_rcBtnNext.right = cx1+m_rcBtnNext.left;
	m_rcBtnNext.top = m_rcBtnAdd.top;
	m_rcBtnNext.bottom = m_rcBtnAdd.bottom;
 
	int cx2 = pDC->GetTextExtent(SZ_PRE).cx;

	m_rcBtnPrev.left = m_rcBtnNext.left - cx2 - 5;
	m_rcBtnPrev.right = m_rcBtnPrev.left + cx2;
	m_rcBtnPrev.top = m_rcBtnAdd.top;
	m_rcBtnPrev.bottom = m_rcBtnAdd.bottom;
 
	pDC->SelectObject(pOldFont);
	ReleaseDC(pDC);

	

	int nTemp;
	if ( rcWnd.right > screen_cx )
	{
		nTemp = rcWnd.right - screen_cx + 10;
		rcWnd.left -= nTemp;
		rcWnd.right -= nTemp;
	}

	if ( rcWnd.bottom > screen_cy )
	{
		nTemp = rcWnd.bottom-screen_cy + 10;
		rcWnd.top -= nTemp;
		rcWnd.bottom -= nTemp;
	}

	MoveWindow(rcWnd);
	//m_nEmotionCount =  m_pImgList->GetImageCount();
	int nCurFaceCount = 0;
	if(m_nCurGroup >= g_customEmotionDB.size())
		m_nCurGroup = 0;
	nCurFaceCount = g_customEmotionDB[m_nCurGroup].size();
	m_nCurPageCount = (nCurFaceCount  / EMOTION_PAGECOUNT );
	if ( (nCurFaceCount % EMOTION_PAGECOUNT ) != 0 )
		m_nCurPageCount++;
	
	//// 如果是默认分组就需要加上系统带的gif表情
	//if(m_nCurGroup == 0)
	//{
	//	m_nGifFacePageCount  = GIF_EMOTION_COUNT / EMOTION_PAGECOUNT;
	//	if ( (GIF_EMOTION_COUNT % EMOTION_PAGECOUNT)!=0 )
	//	{
	//		m_nGifFacePageCount++;
	//	}
	//	m_nCurPageCount += m_nGifFacePageCount;
	//}
	
	if(m_nCurPage >= m_nCurPageCount)
		m_nCurPage = 0;

	// 初始化自定义表情
	InitCustomFace();
	InitGifEmotion();
	InitFaceGroup(g_customEmotionDB);


	//CPoint pt(LEFT_MARGIN,TOP_MARGIN + EMOTION_TITLE_HEIGHT);
	//CSize size(50,50);
	//CRect rcWndPreview(pt,size);
	//m_pWndPreview = new CPictureEx();
	//m_pWndPreview->Create("",WS_CHILD|SS_BLACKFRAME|SS_NOTIFY,rcWndPreview,this,40001);
	//if(m_pWndPreview->Load("f:\\t.gif"))
	//	m_pWndPreview->Draw();
	//m_pWndPreview->ShowWindow(SW_SHOW);

	m_pGroupLbl = new CEdit();
	m_pGroupLbl->Create(WS_CHILD|WS_VISIBLE|WS_TABSTOP|ES_AUTOHSCROLL,CRect(-1000,-1000, 0,0), this, IDC_LABEL_GROUP);
	m_pGroupLbl->SetLimitText(50);

	m_pGroupLbl->SetFont(GetFont());
	//m_pStatic = new CStatic();
	//m_pStatic->Create("",WS_CHILD|WS_VISIBLE|WS_TABSTOP,CRect(0,0,1,1),this);
	return TRUE; 

}

void CEmotionDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	CString strTip = "";
	m_iBtn = -1;
	// 获取鼠标所在的项位置索引
	int nImgHotOver = PtCheck(point);
	int nLeft;
	int nTop;
	int nLines;
	int nColumns;

	if(nImgHotOver != -1 )
	{
		nImgHotOver += m_nCurPage * EMOTION_PERLINE * EMOTION_ROW;
		
		//
		// 设置提示信息
		//

		if(nImgHotOver < g_customEmotionDB[m_nCurGroup].size())
		{
			/*
			strTip.Format("%s / %s",
				g_customEmotionDB[m_nCurGroup][nImgHotOver].szShortCut,
				g_customEmotionDB[m_nCurGroup][nImgHotOver].szTip);
			*/
			strTip.Format("%s", g_customEmotionDB[m_nCurGroup][nImgHotOver].szTip);
			m_typePreview = g_customEmotionDB[m_nCurGroup][nImgHotOver].type;
		}

		if(nImgHotOver != m_nImgHotOver)
		{
			CRect rcLastSel ;
			GetEmotionRect(m_nImgHotOver,rcLastSel);
			m_nImgHotOver = nImgHotOver;
			
			// 
			// 确定预览框的位置
			//

			nLines = nImgHotOver / EMOTION_PERLINE;
			nColumns = nImgHotOver % EMOTION_PERLINE;
			if(nLines < EMOTION_ROW / 2)
			{
				// 被选图标在上部--〉预览就在下部
				nTop =  EMOTION_ROW * EMOTION_HEIGHT + TOP_MARGIN + EMOTION_TITLE_HEIGHT - EMOTION_HEIGHT * 3;
			}
			else
			{
				// 下部 --〉上部
				nTop = TOP_MARGIN + EMOTION_TITLE_HEIGHT;
			}
			if(nColumns < EMOTION_PERLINE / 2)
			{
				// 左部 --〉右部
				nLeft = LEFT_MARGIN + EMOTION_PERLINE * EMOTION_WIDTH - EMOTION_WIDTH * 3;
			}
			else
			{
				// 右部 --〉左部
				nLeft = LEFT_MARGIN ;
			}
			CRect rcOld = m_rcPreview;
			rcOld.InflateRect(2,2,2,2);
			m_rcPreview.left = nLeft;
			m_rcPreview.right = nLeft + EMOTION_WIDTH * 3;
			m_rcPreview.top = nTop;
			m_rcPreview.bottom = nTop + EMOTION_HEIGHT * 3;

			// 
			// 刷新界面预览区和选择表情区
			//

			if(nImgHotOver < g_customEmotionDB[m_nCurGroup].size())
			{	
				CString sFile;
				//sFile.Format("%s%s",(LPCTSTR)g_LocalData.GetCustomFacePath(),g_customEmotionDB[m_nCurGroup][m_nImgHotOver].szFile);
				sFile.Format("%s",g_customEmotionDB[m_nCurGroup][m_nImgHotOver].szFile);
				LoadPreviewImage(sFile);
				ShowPreview(true);	
				InvalidateRect(&rcOld);
				rcOld = m_rcPreview;
				rcOld.InflateRect(2,2,2,2);
				InvalidateRect(&rcOld);
				CRect rcSel;	
				GetEmotionRect(m_nImgHotOver,rcSel);
				InvalidateRect( &rcLastSel);
				InvalidateRect( &rcSel );
			}
			else
			{
				m_nImgHotOver = -1;
				ShowPreview(false);
				InvalidateRect(&rcOld);
				InvalidateRect( &rcLastSel);
			}
		}
	}
	else
	{
		if(m_nImgHotOver != nImgHotOver)
		{
			ShowPreview(false);
			CRect rcLastSel ;
			GetEmotionRect(m_nImgHotOver,rcLastSel);
			InvalidateRect(&rcLastSel);
			InvalidateRect(&m_rcPreview);

			m_nImgHotOver = nImgHotOver;
		}
#ifndef _SIMPLE_EMOTION_
		if ( m_rcBtnAdd.PtInRect(point) )
		{
			m_iBtn = IDC_LINK_ADD;
			strTip = "添加、修改、删除您的表情";
		}

		if( m_rcBtnUpdate.PtInRect(point))
		{
			m_iBtn = IDC_LINK_UPDATE;
			strTip = "从网站下载最新的表情图片";
		}

		if ( m_rcBtnPrev.PtInRect(point) )
		{
			m_iBtn = IDC_LINK_PRE;
		}

		if ( m_rcBtnNext.PtInRect(point) )
		{
			m_iBtn = IDC_LINK_NEXT;
		}
#endif
	}

	// 显示提示信息
	//ShowToolTip(strTip);

	if ( m_iBtn != -1 )
	{
		::SetCursor(::LoadCursor(NULL,MAKEINTRESOURCE(32649)));//IDC HAND
		Invalidate();
	}
//#endif	
	CDialog::OnMouseMove(nFlags, point);
}

int CEmotionDlg::PtCheck(CPoint pt)
{
	if (pt.x < 1) 
		pt.x = 0;
	if (pt.y < 1) 
		pt.y = 0;

	int x = (pt.x - LEFT_MARGIN);
	if(x < 0)
		return -1;
	x /= EMOTION_WIDTH ;
	int y = (pt.y - (TOP_MARGIN + EMOTION_TITLE_HEIGHT));
	if(y < 0)
	{
		return -1;
	}
	y /= EMOTION_HEIGHT;

	

	if( x >= EMOTION_PERLINE)
	{
		return -1;
	}
	else if( x < 0)
	{
		return -1;
	}

	if ( y >= EMOTION_ROW )
	{
		return -1;
	}
	else if( y < 0 )
	{
		return -1;
	}

	return (y * EMOTION_PERLINE + x);

}

void CEmotionDlg::GetEmotionRect(int nEmotion, CRect &rcEmotion)
{
	if(nEmotion < 0)
	{
		rcEmotion = CRect(0,0,0,0);
		return;
	}
	nEmotion = nEmotion % EMOTION_PAGECOUNT;
	POINT pt;
	pt.x = (nEmotion%EMOTION_PERLINE) * EMOTION_WIDTH + LEFT_MARGIN + 2; 
	pt.y = (nEmotion/EMOTION_PERLINE) * EMOTION_HEIGHT + TOP_MARGIN + EMOTION_TITLE_HEIGHT + 2;

	rcEmotion.left = pt.x ;
	rcEmotion.top = pt.y ;
	rcEmotion.bottom = pt.y + EMOTION_HEIGHT -3;
	rcEmotion.right = pt.x + EMOTION_WIDTH -3;
}

void CEmotionDlg::DrawCustomFace(EMOTION_ID id, CDC *pDC )
{
	CustomFaceMapIt it = m_mapCustomFace.find(id);
	CBitmap * pCustomFaceBmp = NULL;
	if ( it == m_mapCustomFace.end() || it->second == NULL )
	{
		CString strCustomFaceFile;
		CString strCustomFacePath = g_LocalData.GetCustomFacePath();
		strCustomFaceFile.Format("%s%s",strCustomFacePath,g_customEmotionDB[id.nGroup][id.nItem].szFile);
		pCustomFaceBmp = GetFaceBmpFromFile(strCustomFaceFile);
		if(pCustomFaceBmp)
		{
			m_mapCustomFace[id] = pCustomFaceBmp;
		}
		else
		{
			return;
		}
	}
	else
	{
		pCustomFaceBmp = it->second;
	}


	int nEmotion;
	nEmotion = id.nItem % EMOTION_PAGECOUNT;

	POINT pt;
	pt.x = (nEmotion%EMOTION_PERLINE) * EMOTION_WIDTH + 3 + LEFT_MARGIN; 
	pt.y = (nEmotion/EMOTION_PERLINE) * EMOTION_HEIGHT + 3 + TOP_MARGIN + EMOTION_TITLE_HEIGHT;

	CRect rcEdge;
	rcEdge.left = pt.x ;
	rcEdge.top = pt.y+1 ;
	rcEdge.bottom = pt.y + EMOTION_SIZE+1 ;
	rcEdge.right = pt.x + EMOTION_SIZE+1 ;

	pDC->FillSolidRect(rcEdge, RGB(255,255,255));

	CDC mDC;
	mDC.CreateCompatibleDC(pDC);
	CBitmap *pOldbmp = mDC.SelectObject(pCustomFaceBmp);
	pDC->BitBlt(pt.x,pt.y,EMOTION_SIZE,EMOTION_SIZE,&mDC,0,0,SRCCOPY);
	mDC.SelectObject(pOldbmp);
}

void CEmotionDlg::DrawGifEmotion(int nEmotion, CDC *pDC )
{
	map<int,CBitmap *>::iterator it = m_mapGifEmotion.find(nEmotion);
	if ( it==m_mapGifEmotion.end() || it->second==NULL )
	{
		return ;
	}

	POINT pt;
	nEmotion--;
	if (nEmotion>=EMOTION_PAGECOUNT)
	{
		nEmotion=(nEmotion%EMOTION_PAGECOUNT);
	}

	pt.x = (nEmotion%EMOTION_PERLINE) * EMOTION_WIDTH + 1 + LEFT_MARGIN ; 
	pt.y = (nEmotion/EMOTION_PERLINE) * EMOTION_HEIGHT + 1 + TOP_MARGIN + EMOTION_TITLE_HEIGHT;

	CRect rcEdge;
	rcEdge.left = pt.x ;
	rcEdge.top = pt.y+1 ;
	rcEdge.bottom = pt.y + 25 ;
	rcEdge.right = pt.x + 25 ;

	pDC->FillSolidRect(rcEdge, RGB(255,255,255));

	CDC mDC;
	mDC.CreateCompatibleDC(pDC);

	CBitmap *pOldbmp = mDC.SelectObject((CBitmap *)it->second);
	pt.x += 2;
	pt.y += 2;
	//pDC->BitBlt(pt.x,pt.y,EMOTION_SIZE,EMOTION_SIZE,&mDC,0,0,SRCCOPY);
	pDC->StretchBlt(pt.x,pt.y,EMOTION_SIZE,EMOTION_SIZE,&mDC,0,0,25,25,SRCCOPY);
	mDC.SelectObject(pOldbmp);
}

void CEmotionDlg::DrawSysPreview(int nEmotion,CDC * pDC)
{
	Graphics g(pDC->m_hDC);
	Rect rc(m_rcPreview.left + 3,m_rcPreview.top + 3,m_rcPreview.Width() - 4,m_rcPreview.Height() - 4);
	Rect rcImage = rc;
	Pen pen(Color(0,0,255),1);
	SolidBrush brush(Color(255,255,255));
	g.FillRectangle(&brush,rc);

	POINT pt;
	pt.x = m_rcPreview.left + EMOTION_WIDTH + 3;
	pt.y = m_rcPreview.top + EMOTION_HEIGHT + 3;
	m_pImgList->Draw(pDC, nEmotion, pt,ILD_NORMAL);

	g.DrawRectangle(&pen,rc);
	
	
}

void CEmotionDlg::DrawSysGifPreview(int nEmotion,CDC * pDC)
{

	Graphics g(pDC->m_hDC);
	Rect rc(m_rcPreview.left + 3,m_rcPreview.top + 3,m_rcPreview.Width() - 4,m_rcPreview.Height() - 4);
	Rect rcImage = rc;
	Pen pen(Color(0,0,255),1);
	SolidBrush brush(Color(255,255,255));
	g.FillRectangle(&brush,rc);

	map<int,CBitmap *>::iterator it = m_mapGifEmotion.find(nEmotion);
	if ( it == m_mapGifEmotion.end() || it->second == NULL )
	{
	}
	else
	{
		POINT pt;
		CDC mDC;
		mDC.CreateCompatibleDC(pDC);
		CBitmap *pOldbmp = mDC.SelectObject((CBitmap *)it->second);
		pt.x = m_rcPreview.left + EMOTION_WIDTH + 3;
		pt.y = m_rcPreview.top + EMOTION_HEIGHT + 3;
		pDC->BitBlt(pt.x,pt.y,25,25,&mDC,0,0,SRCCOPY);
		mDC.SelectObject(pOldbmp);
	}
	g.DrawRectangle(&pen,rc);

}

void CEmotionDlg::DrawEmotion(int nEmotion, UINT nStyle, CDC * pDC)
{
	POINT pt;

	pt.x = (nEmotion%EMOTION_PERLINE) * EMOTION_WIDTH + 3 + LEFT_MARGIN;
	pt.y = (nEmotion/EMOTION_PERLINE) * EMOTION_HEIGHT + 3 + TOP_MARGIN + EMOTION_TITLE_HEIGHT;

	CRect rcEdge;

	rcEdge.left = pt.x ;
	rcEdge.top = pt.y+1 ;
	rcEdge.bottom = pt.y + EMOTION_SIZE+1 ;
	rcEdge.right = pt.x + EMOTION_SIZE+1 ;

	pDC->FillSolidRect(rcEdge, RGB(255,255,255));
	m_pImgList->Draw(pDC, nEmotion, pt, nStyle);

	//画边框
	if (nStyle != ILD_NORMAL)
	{
		pDC->Draw3dRect(rcEdge, EMOTION_GRID_COLOR, EMOTION_GRID_COLOR);
	}
}

int CEmotionDlg::PtInGroup(CPoint point)
{
	int i;
	int nC;
	// 是否选中分组标签
	if(point.y < TOP_MARGIN + EMOTION_TITLE_HEIGHT)
	{
		nC = m_arrGroup.size();
		point.x = point.x + (m_arrGroup[m_arrGroup.m_nFirstGroup].rc.left - m_arrGroup[0].rc.left);
		for(i = 0; i < nC; i++)
		{
			if(m_arrGroup[i].rc.PtInRect(point))
			{
				return i;
			}
		}
	}
	return -1;
}

void CEmotionDlg::HideEdit()
{
	if(m_pGroupLbl)
	{
		CString sText;
		m_pGroupLbl->GetWindowText(sText);
		if(m_nEditGroup < m_arrGroup.size() && g_customEmotionDB[m_nEditGroup].sName != sText)
		{
			g_customEmotionDB[m_nEditGroup].sName = sText;
			InitFaceGroup(g_customEmotionDB);
			if(m_nCurGroup < g_customEmotionDB.size())
			{
				SaveCustomEmotionConfig();
			}
		}
		CRect rct;
		GetWindowRect(&rct);
		ScreenToClient(&rct);
		rct.bottom = TOP_MARGIN + EMOTION_TITLE_HEIGHT;
		InvalidateRect(&rct);
		rct.MoveToXY(-1000,-1000);
		m_pGroupLbl->MoveWindow(&rct);
	}
}



void CEmotionDlg::ShowEdit()
{
	if(m_pGroupLbl)
	{
		if(m_nEditGroup > g_customEmotionDB.size())
			return;
		CRect rct;
		CRect rctLbl;
		m_pGroupLbl->GetRect(&rct);
		m_pGroupLbl->SetWindowText(g_customEmotionDB[m_nEditGroup].sName);
		rctLbl = m_arrGroup[m_nEditGroup].rc;
		rct.left = rctLbl.left + 3;
		rct.top = rctLbl.top + 2;
		rct.right = rct.left + rctLbl.Width() - 6;
		rct.bottom = rct.top + rctLbl.Height() - 4;
		m_pGroupLbl->MoveWindow(&rct);
		m_pGroupLbl->SetSel(0,-1);
	}
}

void CEmotionDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	int nImgNow  = PtCheck(point);
//	int nC;
	int nSelGroup = -1;
	bool bDestroyWindow = false;

	// 是否选中表情
	if(nImgNow != -1)
	{
		nImgNow += m_nCurPage * EMOTION_PAGECOUNT;
		// 判断选择的表情是否存在
		if(nImgNow >= g_customEmotionDB[m_nCurGroup].size())
		{
			nImgNow = -1;
			if(m_bEditLabel)
			{
				HideEdit();
				m_bEditLabel = false;
			}
			/*if(m_pGroupLbl)
			{
				m_pGroupLbl->SendMessage(WM_KILLFOCUS,(WPARAM)m_pGroupLbl->m_hWnd);
			}*/
			//m_pStatic->SetFocus();
			return;
		}
		else
		{
			bDestroyWindow = true;
		}
	}
	else
	{
		nSelGroup = PtInGroup(point);
		// 是否选中分组标签
		if(nSelGroup != -1)
		{
			if(m_nCurGroup != nSelGroup)
			{
				m_nCurGroup = nSelGroup;
				m_nCurPage = 0;
				int nC = g_customEmotionDB[m_nCurGroup].size();
				m_nCurPageCount = nC / EMOTION_PAGECOUNT;
				if(nC % EMOTION_PAGECOUNT)
				{
					m_nCurPageCount++;
				}
				Invalidate();
			}
		}
		else
		{
			// 是否选中按钮
#ifndef _SIMPLE_EMOTION_
			switch ( m_iBtn )
			{
			case IDC_LINK_ADD:
				{
					if(m_nCurGroup == 1)
					{
						ShowMessage("当前为系统表情组，不能做修改");
					}
					else
					{
						CEmotionManageDlg dlg(m_nCurGroup,GetDesktopWindow());
						dlg.DoModal();
					}
					
					return;
				}
				break;
			case IDC_LINK_PRE:
				m_nCurPage--;
				if ( m_nCurPage < 0)	
				{
					m_nCurPage = 0;
				}
				Invalidate();
				break ;
			case IDC_LINK_NEXT:
				m_nCurPage++;
				if ( m_nCurPage >= m_nCurPageCount )
				{
					m_nCurPage = m_nCurPageCount - 1;
					if(m_nCurPage < 0)
						m_nCurPage = 0;
				}
				Invalidate();
				break ;
			case IDC_LINK_UPDATE:
				{
					/*CP2PSendMsgDlg*/CDuiP2PSendMsgDlg *pDlg=(/*CP2PSendMsgDlg*/CDuiP2PSendMsgDlg*)GetParent();
					pDlg->SendMessage(WM_DOWNLOADFACE,0,0);
					DestroyWindow();
					return;
				}
			default:
				{
					
					;
				}
			
				
			}
#endif	
		}
	}
	
	if(nImgNow == -1)
	{
		bDestroyWindow = false;
	}
	if(bDestroyWindow == false)
	{
		if(m_bEditLabel)
		{
			HideEdit();
			m_bEditLabel = false;
		}
		return;
	}
	
	//m_nImgSel = nImgNow;

	::SendMessage(m_hEmotionSelWnd,WM_EMOTIONSEL_NOTIFY,(WPARAM)&g_customEmotionDB[m_nCurGroup][nImgNow],nImgNow);

	DestroyWindow();

}

void CEmotionDlg::OnNcDestroy()
{
	CDialog::OnNcDestroy();

	delete this;
}

void CEmotionDlg::InitGifEmotion()
{
	for ( int i = 1; i<=GIF_EMOTION_COUNT; i++ )
	{
		CBitmap *pBitmap = CBitmapFromFile(g_LocalData.GetGifFacePath(i),25,25);
		m_mapGifEmotion.insert(map<int,CBitmap*>::value_type(i,pBitmap));
	}
}

void CEmotionDlg::InitCustomFace()
{
	//CString strCustomFacePath = g_LocalData.GetCustomFacePath();
	//CString strCustomFaceFile ;
	//// 仅初始化首页
	//for ( int i=0; i<g_customEmotionDB.size(); i++ )
	//{
	//	// 除去系统自带表情
	//	if(i >= EMOTION_PAGECOUNT - m_nEmotionCount)
	//	{
	//		break;
	//	}
	//	strCustomFaceFile.Format("%s%s",strCustomFacePath,g_customEmotionDB[i].szFile);
	//	CBitmap *pCustomFaceBmp = CBitmapFromFile(strCustomFaceFile,EMOTION_SIZE,EMOTION_SIZE);
	//	m_mapCustomFace.insert(map<int,CBitmap*>::value_type(i,pCustomFaceBmp));
	//	m_nLoaded ++;
	//}
}

bool IsWebFaceExist(const char* szFaceID)
{
	int nGroup = g_customEmotionDB.size();
	int nC;
	int i,j;
	for(i = 0;i < nGroup; i++)
	{
		CUSTOM_EMOTION_GROUP & group = g_customEmotionDB[i];
		nC = group.size();
		for(j = 0; j < nC; j++)
		{
			if(group[j].type == CUSTOM_EMOTION::WEB)
			{
				if(strcmp(group[j].szID,szFaceID) == 0)
					return true;
			}
		}
	}
	return false;
}

void GetExistWebFiles(vector<string> & sExistFiles)
{
	sExistFiles.clear();
	size_t i,j;
	size_t nC;
	size_t nGroup = g_customEmotionDB.size();
	for( i = 0;i < nGroup; i++)
	{
		nC = g_customEmotionDB[i].size();
		for( j = 0; j < nC; j++)
		{
			if(g_customEmotionDB[i][j].type == CUSTOM_EMOTION::WEB)
			{
				sExistFiles.push_back(g_customEmotionDB[i][j].szID);
			}
		}
		
	}
}

EMOTION_ID FindFaceUseUrl(const char * szUrl)
{
	EMOTION_ID id;
	size_t i,j;
	size_t nC;
	size_t nGroup = g_customEmotionDB.size();
	id.nGroup = -1;
	id.nItem = -1;

	for( i = 0 ;i < nGroup; i++)
	{
		nC = g_customEmotionDB[i].size();
		for(j = 0;j < nC; j++)
		{
			if(g_customEmotionDB[i][j].type == CUSTOM_EMOTION::WEB)
			{
				if(strcmp(g_customEmotionDB[i][j].szUrl,szUrl) == 0)
				{
					id.nGroup = i;
					id.nItem = j;
					return id;
				}
			}
		}
	}
	return id;
}

void CEmotionDlg::RememberPostion()
{
	// 记录下当前选择的表情分组和分页位置
	CString sCfg;
	CString sGroup;
	CString sPage;
	sCfg.Format("%sdata\\%s\\customface.ini",(LPCTSTR)g_LocalData.GetPath(),g_ImUserInfo./*GetUserName*/GetUserFileName().c_str());
	sGroup.Format("%d",m_nCurGroup);
	sPage.Format("%d",m_nCurPage);
	::WritePrivateProfileString("LastSelected","group",sGroup,sCfg);
	::WritePrivateProfileString("LastSelected","page",sPage,sCfg);
}

BOOL CEmotionDlg::DestroyWindow()
{
	RememberPostion();
	return CDialog::DestroyWindow();
}


void CEmotionDlg::InitFaceGroup(const CUSTOM_EMOTION_DB & emotions)
{
	m_arrGroup.clear();
	int i;
	int nC;
	EMOTION_GROUP_ITEM item;
	int nLeft = LEFT_MARGIN + LABEL_SPAN * 2;
	item.rc.top = TOP_MARGIN;
	item.rc.bottom = item.rc.top + EMOTION_TITLE_HEIGHT - 2;
	nC = emotions.size();
	CDC * pDC = GetDC();
	if(pDC == NULL)
		return;
	CSize size;
	CFont * pOldFont = pDC->SelectObject(GetFont());
	
	for(i = 0; i < nC; i++)
	{
		if(i == 0)
			item.sName = "默认";
		else if(emotions[i].sName == "")
			item.sName = "<无>";
		else
			item.sName = emotions[i].sName;
		size = pDC->GetTextExtent(item.sName);
		item.rc.left = nLeft;
		item.rc.right = nLeft + 8 + size.cx;
		nLeft = item.rc.right + LABEL_SPAN;
		m_arrGroup.push_back(item);
	}
	pDC->SelectObject(pOldFont);
	ReleaseDC(pDC);
}
void CEmotionDlg::OnTimer(UINT nIDEvent)
{
	if(nIDEvent == TIMER_PREVIEW)
	{
		if(m_bShowPreview && m_bAnimate)
		{
			GUID   pageGuid = FrameDimensionTime;
			m_nFramePosition++;
			if (m_nFramePosition == m_nFrameCount)
				m_nFramePosition = 0;
			m_pImage->SelectActiveFrame(&pageGuid, m_nFramePosition);
			long nTime = ((long*) m_pPropertyItem->value)[m_nFramePosition] * 12;
			if(nTime < 13)
			{
				nTime = 100;
			};
			SetTimer(TIMER_PREVIEW,nTime,NULL);
			InvalidateRect(&m_rcPreview);
			/*CDC * pDC = GetWindowDC();
			if(pDC)
			{
				DrawPreview(*pDC);
				ReleaseDC(pDC);
			}*/
		}
	}

	CDialog::OnTimer(nIDEvent);
}

void CEmotionDlg::OnRButtonUp(UINT nFlags, CPoint point)
{
#ifndef _SIMPLE_EMOTION_
	CMenu menu;
	int nGroup = g_customEmotionDB.size();
	menu.CreatePopupMenu();
	menu.AppendMenu(MF_STRING,ID_NEW_GROUP,"新建分组(&N)");
	if(m_nImgHotOver < 0)
	{
		if(m_nCurGroup != 0 && m_nCurGroup != 1)
		{
			menu.AppendMenu(MF_STRING,ID_DEL_GROUP,"删除分组(&D)");
			menu.AppendMenu(MF_STRING,ID_MOD_GROUPNAME,"修改组名(&R)");
			menu.AppendMenu(MF_SEPARATOR);
		}
	}
	if(m_nCurGroup != 1)
	{
		menu.AppendMenu(MF_STRING,ID_NEW_FACE,"新添表情(&F)");
	}
	if(m_nCurGroup < nGroup && m_nImgHotOver != -1 && m_nImgHotOver < g_customEmotionDB[m_nCurGroup].size())
	{
		int type = g_customEmotionDB[m_nCurGroup][m_nImgHotOver].type;
		if(type == CUSTOM_EMOTION::SYS || type == CUSTOM_EMOTION::SYS_GIF)
			return;
		menu.AppendMenu(MF_STRING,ID_MODIFY_EMOTION,"修改表情(&M)");
		menu.AppendMenu(MF_STRING,ID_DEL_EMOTION,"删除表情(&D)");
		menu.AppendMenu(MF_SEPARATOR);
		CMenu menuGroup;
		menuGroup.CreatePopupMenu(); 
		int i;
		for(i = 0;i < nGroup;i++)
		{
			if(i == 1 || i == m_nCurGroup)
				continue;
			menuGroup.AppendMenu(MF_STRING,ID_MOVE_FACE_BEG + i,m_arrGroup[i].sName);
		}
		if(nGroup > 2)
			menu.AppendMenu(MF_POPUP, (UINT)menuGroup.m_hMenu, "移动到...");
	}

	ClientToScreen(&point);
	menu.TrackPopupMenu(TPM_LEFTBUTTON,point.x,point.y,this);
#endif

}


void CEmotionDlg::OnDelEmotion()
{
	if(m_nImgHotOver < 0 || m_nImgHotOver >= g_customEmotionDB[m_nCurGroup].size())
		return;
	int type = g_customEmotionDB[m_nCurGroup][m_nImgHotOver].type;
	if(type == CUSTOM_EMOTION::SYS || type == CUSTOM_EMOTION::SYS_GIF)
		return;
	
	CString strEmotionFile;
	CString sSmallFile;
	strEmotionFile = g_LocalData.GetCustomFacePath() + g_customEmotionDB[m_nCurGroup][m_nImgHotOver].szFile;
	sSmallFile = g_LocalData.GetCustomFacePath() + "_" + g_customEmotionDB[m_nCurGroup][m_nImgHotOver].szFile +".bmp";
	if(m_pImage)
	{
		delete m_pImage;
		m_pImage = NULL;
	}
	BOOL bRt = DeleteFile(strEmotionFile);
	bRt = DeleteFile(sSmallFile);

	

	g_customEmotionDB[m_nCurGroup].erase(g_customEmotionDB[m_nCurGroup].begin() + m_nImgHotOver);

	// 将自定义表情图片库删除

	CustomFaceMapIt it,itEnd;
	itEnd = m_mapCustomFace.end();
	for(it = m_mapCustomFace.begin();it != itEnd;it++)
	{
		if(it->second)
		{
			delete it->second;
			it->second = NULL;
		}
	}
	/*EMOTION_ID id;
	id.nGroup = m_nCurGroup;
	id.nItem = m_nImgHotOver;
	it = m_mapCustomFace.find(id);
	itEnd = m_mapCustomFace.end();
	for(it)*/
	//// 将位图指针左移
	//if(it != itEnd)
	//{
	//	if(it->second)
	//		delete it->second;
	//	while(it != itEnd)
	//	{
	//		itCur = it;
	//		it++;
	//		if(it == itEnd)
	//		{
	//			itCur->second = NULL;
	//			break;
	//		}
	//		else
	//		{
	//			itCur->second = it->second;
	//		}
	//	}
	//}

	CString strCustomFacePath = g_LocalData.GetCustomFacePath();
	CString strCustomConfigFile = strCustomFacePath+"customface.cfg.xml";
	DeleteFile(strCustomConfigFile);
	
	CStdioFile file;
	bool bOpenFlag;
	if ( _access((LPCSTR)strCustomConfigFile,0) == -1 )
	{
		bOpenFlag = file.Open(strCustomConfigFile,CFile::modeCreate|CFile::modeWrite|CFile::typeText);
	}
	else
	{
		bOpenFlag = file.Open(strCustomConfigFile,CFile::modeWrite|CFile::typeText) ;
		if(bOpenFlag)
			file.SeekToBegin();
	}

	if ( bOpenFlag )
	{
		WriteCustomEmotionConfig(file);
		file.Close();
	}
	Invalidate();
	
}

void CEmotionDlg::OnModifyEmotion()
{

	if(m_nImgHotOver < 0 || m_nImgHotOver >= g_customEmotionDB[m_nCurGroup].size())
		return;
	int type = g_customEmotionDB[m_nCurGroup][m_nImgHotOver].type;
	if(type == CUSTOM_EMOTION::SYS || type == CUSTOM_EMOTION::SYS_GIF)
		return;

	CString strEmotionFile = g_LocalData.GetCustomFacePath() + g_customEmotionDB[m_nCurGroup][m_nImgHotOver].szFile;
	CAddEmotionDlg dlg(m_nCurGroup,strEmotionFile,this);
	dlg.SetModifyEmotion(m_nCurGroup,m_nImgHotOver);
	if (dlg.DoModal()==IDOK)
	{
	}

}

void CEmotionDlg::OnDelGroup()
{
	CString sPrompt;
	if(m_nCurGroup < 2 || m_nCurGroup >= g_customEmotionDB.size())
		return;
	sPrompt.Format("确认删除表情组\"%s\"吗？",(LPCTSTR)g_customEmotionDB[m_nCurGroup].sName);
	
	int nCurGroup = m_nCurGroup;
	// 不关闭表情对话框
	m_nImgHotOver = 0;
	if(CMessageBox::Model(NULL,sPrompt,"删除表情组",MB_YESNO) == IDNO)
	{
		return;
	}

	if(m_pImage)
	{
		delete m_pImage;
		m_pImage = NULL;
	}

	CUSTOM_EMOTION_GROUP & group = g_customEmotionDB[nCurGroup];
	int i;
	int j = 0;
	int n;
	CString strEmotionFile;
	CString sSmallFile;
	
	//EMOTION_ID id;
	//id.nGroup = nCurGroup;
	//CustomFaceMapIt it,itE;
	//itE = m_mapCustomFace.end();
	//// 首先删除内存图片
	//n = group.size();
	//for(i = 0; i < n; i++)
	//{
	//	id.nItem = i;
	//	it = m_mapCustomFace.find(id);
	//	if(it != itE)
	//	{
	//		if(it->second)
	//		{
	//			delete it->second;
	//			it->second = NULL;
	//		}
	//	}
	//}
	
	// 再删除图片文件
	n = group.size();
	for(i = 0; i < n; i++)
	{
		strEmotionFile = g_LocalData.GetCustomFacePath() + group[i].szFile;
		sSmallFile = g_LocalData.GetCustomFacePath() + "_" + group[i].szFile +".bmp";
		DeleteFile(strEmotionFile);
		DeleteFile(sSmallFile);

	}

	//// 如果保持当前表情框打开的话，就更新内存图片库信息

	//int nGroupC = g_customEmotionDB.size();
	//for(i = m_nCurGroup + 1; i < nGroupC;i++)
	//{
	//	n = g_customEmotionDB[i].size();
	//	for(j = 0; j < n; j++)
	//	{
	//		id.nGroup = i;
	//		id.nItem = j;
	//		it = m_mapCustomFace.find(id);
	//		if(it != itE)
	//		{
	//			// 移动到前一组
	//			id.nGroup--;
	//			m_mapCustomFace[id] = it->second;
	//			it->second = NULL;
	//		}
	//	}
	//}

	// 最后删除数据
	g_customEmotionDB.erase(g_customEmotionDB.begin() + nCurGroup);
	SaveCustomEmotionConfig();
	m_nCurGroup = 0;
	m_nImgHotOver = -1;
};

void CEmotionDlg::OnNewGroup()
{
	CUSTOM_EMOTION_GROUP group;
	g_customEmotionDB.push_back(group);
	m_nCurGroup = g_customEmotionDB.size() - 1;
	m_nCurPage = 0;
	m_bEditLabel = true;
	InitFaceGroup(g_customEmotionDB);
	SaveCustomEmotionConfig();
	m_nEditGroup = m_nCurGroup;
	ShowEdit();
	Invalidate();
}

void CEmotionDlg::OnModGroupName()
{
	if(m_nCurGroup != -1 && m_nCurGroup != 0 && m_nCurGroup != 1)
	{
		m_bEditLabel = true;
		m_nEditGroup = m_nCurGroup;
		ShowEdit();
	}
}

void CEmotionDlg::OnNewFace()
{
	CImageFileDialog dlg("","",OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,NULL);
	int nGroup = m_nCurGroup;
	if ( IDOK == dlg.DoModal() )
	{
		CAddEmotionDlg dlg(nGroup,dlg.GetPathName(),GetDesktopWindow());
		dlg.SetAddPic();
		dlg.DoModal();
	}
}

void CEmotionDlg::OnMoveFace(UINT nGroupID)
{
	nGroupID -= ID_MOVE_FACE_BEG;
	if(nGroupID >= g_customEmotionDB.size())
	{
		return;
	}
	if(	m_nImgHotOver != -1 
		&& m_nCurGroup < g_customEmotionDB.size()
		&& m_nImgHotOver < g_customEmotionDB[m_nCurGroup].size())
	{
		CUSTOM_EMOTION face;
		EMOTION_ID idOld,idNew;
		CustomFaceMapIt it,itEnd;
		int nNewItem;
		idOld.nGroup = m_nCurGroup;
		idOld.nItem = m_nImgHotOver;
		
		face = g_customEmotionDB[m_nCurGroup][m_nImgHotOver];
		// 移动表情到新组中
		nNewItem = g_customEmotionDB[nGroupID].size();
		g_customEmotionDB[nGroupID].push_back(face);
		
		// 在当前组中删除该表情
		g_customEmotionDB[m_nCurGroup].erase(g_customEmotionDB[m_nCurGroup].begin() + m_nImgHotOver);
		
		// 删除图片库
		itEnd = m_mapCustomFace.end();
		for(it = m_mapCustomFace.begin();it != itEnd;it++)
		{
			if(it->second)
			{
				delete it->second;
				it->second = NULL;
			}
		}
		
		SaveCustomEmotionConfig();
		Invalidate();
	}
}

void CEmotionDlg::OnEditChange()
{
	if(m_pGroupLbl)
	{
		/*CRect rct;
		GetWindowRect(&rct);
		ScreenToClient(&rct);
		rct.bottom = EMOTION_TITLE_HEIGHT + TOP_MARGIN;
		InvalidateRect(&rct);*/
	}
}

void CEmotionDlg::OnEnter()
{
	if(m_bEditLabel)
	{
		HideEdit();
		m_bEditLabel = false;
	}
}

void CEmotionDlg::OnEnKillfocusEditLabel()
{
	if(m_bEditLabel)
	{
		HideEdit();
		m_bEditLabel = false;
	}
	OnKillFocus(NULL);
}


void CEmotionDlg::OnLButtonUp(UINT nFlags, CPoint point)
{

	CDialog::OnLButtonUp(nFlags, point);
}

void CEmotionDlg::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	int nSelGroup = PtInGroup(point);
	if(nSelGroup != -1)
	{
		m_nCurGroup = nSelGroup;
		m_nEditGroup = nSelGroup;
	}
	
	if(nSelGroup != -1 && nSelGroup != 0 && nSelGroup != 1)
	{
		m_bEditLabel = true;
		ShowEdit();
	}
	CDialog::OnLButtonDblClk(nFlags, point);
}

void CEmotionDlg::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if(nChar == VK_RETURN)
	{
		if(m_bEditLabel)
		{
			HideEdit();
			m_bEditLabel = false;
		}
	}

	CDialog::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CEmotionDlg::ShowToolTip(const CString& strTip)
{
	if(strTip.IsEmpty())
	{
		m_sLastTip = "";
		if(::IsWindow(m_Tooltip.m_hWnd))
		{
			m_Tooltip.Activate(FALSE);
		}
		return;
	}

	if(m_sLastTip==strTip)
		return;
	m_sLastTip=strTip;
	if (m_Tooltip.m_hWnd == NULL)
	{
		m_Tooltip.Create(this,TTS_ALWAYSTIP);
		// 设置出现提示需要的时间长度
		m_Tooltip.SetDelayTime(TTDT_INITIAL,100);
		// 设置提示持续时间
		m_Tooltip.SetDelayTime(TTDT_AUTOPOP,10000);
	}
	if (m_Tooltip.GetToolCount() == 0)
	{
		CRect rc;
		GetClientRect(&rc);
		m_Tooltip.AddTool(this, strTip, rc, 1001);
	}
	m_Tooltip.UpdateTipText(strTip ,this,1001);
	m_Tooltip.Activate(TRUE);
}

BOOL CEmotionDlg::PreTranslateMessage(MSG* pMsg)
{
	/*if( pMsg->message== WM_MOUSEMOVE )
	{
		if(m_Tooltip.m_hWnd!=NULL)
			m_Tooltip.RelayEvent(pMsg);
	}*/
	return CDialog::PreTranslateMessage(pMsg);
}
