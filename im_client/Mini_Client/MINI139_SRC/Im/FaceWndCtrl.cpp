#include "stdafx.h"
#include "facewndctrl.h"

#include "WebProcess.h"
#include "improtocol.h"
#include "localdata.h"

bool ParaseFaceList(CString strWebResult, map<int,CString> &mapFace)
{
	HRESULT hr_com = ::CoInitialize(0);

	CComPtr<IXMLDOMDocument> pDoc;
	pDoc.CoCreateInstance(__uuidof(DOMDocument));

	VARIANT_BOOL bSuccess=VARIANT_FALSE;

	BSTR bstrXml = strWebResult.AllocSysString();
	pDoc->loadXML(bstrXml,&bSuccess);
	SysFreeString(bstrXml);

	int  nFace = 0;
	char szItem[255];

	USES_CONVERSION;

	while ( bSuccess==VARIANT_TRUE )
	{
		CComPtr<IXMLDOMNodeList> pNodes = NULL;

		sprintf(szItem,"faceList/face[%d]/*",nFace++);
		//pDoc->selectSingleNode(A2W(szItem),&pNode);
		pDoc->selectNodes(A2W(szItem),&pNodes);

		long nLen ;
		pNodes->get_length(&nLen);

		if ( pNodes &&  nLen>=2 )
		{			
			CComPtr<IXMLDOMNode> pNode = NULL;
			BSTR text=L"";

			int			nFaceId;
			CString		strUrl;

			USES_CONVERSION;

			pNodes->get_item(0,&pNode);
			pNode->get_text(&text);

			nFaceId = atoi(WideStrToStr(text));
			pNode.Release();

			pNodes->get_item(1,&pNode);
			pNode->get_text(&text);
			strUrl = WideStrToStr(text);

			map<int,CString>::iterator it = mapFace.find(nFaceId);

			if ( it!=mapFace.end() )
			{
				mapFace[nFaceId]=strUrl;
			}
			else
			{
				mapFace.insert(map<int,CString>::value_type(nFaceId,strUrl));
			}
		}
		else
		{
			break;
		}
	}

	if ( hr_com==S_OK )
		::CoUninitialize();

	return true;
}

// CFaceWndCtrl
CString HistoryFaceUrl2WebUrl(CString strHistoryFaceUrl)
{
	CString strUrl;

	CString strFileName = GetWebFileName(strHistoryFaceUrl);
	int pos=strFileName.Find(".");
	CString strNewFileName;
	strNewFileName.Format("%s_%d%s",strFileName.Left(pos),FACE_SZ,strFileName.Mid(pos,strFileName.GetLength()-pos));
	strUrl = strHistoryFaceUrl;
	strUrl.Replace(strFileName,strNewFileName);

	return strUrl;
}

IMPLEMENT_DYNAMIC(CFaceWndCtrl, CWnd)
CFaceWndCtrl::CFaceWndCtrl()
{
	m_OriginY = 0;
	m_CurrentSel.nFacyType=-1;
	m_CurrentSel.nSel=-1;
	m_HotSel.nFacyType=-1;
	m_HotSel.nSel=-1;
	m_bMouseOver = false;
	
	m_hWokuheadProc = NULL;
}

CFaceWndCtrl::~CFaceWndCtrl()
{
	if ( m_hWokuheadProc )
	{
		try
		{
			TerminateThread(m_hWokuheadProc,0);
			CloseHandle(m_hWokuheadProc);
			m_hWokuheadProc = NULL ;
		}
		catch(...)
		{
		}		
	}

	ClearData();
}

BEGIN_MESSAGE_MAP(CFaceWndCtrl, CWnd)
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	ON_WM_VSCROLL()
	ON_WM_SIZE()
	ON_MESSAGE(WM_WEBFILE_NOTIFY,OnWebFileNotify)
	ON_MESSAGE(WM_THREAD_NOTIFY,OnThreadNotify)
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_TIMER()
END_MESSAGE_MAP()

BOOL CFaceWndCtrl::Create(CWnd *pParentWnd,const CRect &rect, DWORD dwStyle/* =WS_VISIBLE|WS_BORDER|WS_CHILD */,UINT nID/* =0 */)
{
	return CWnd::Create( AfxRegisterWndClass(CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW | CS_GLOBALCLASS),
		"selffacewndctrl",
		dwStyle,
		rect,pParentWnd,
		nID);
}

// CFaceWndCtrl 消息处理程序
BOOL CFaceWndCtrl::OnEraseBkgnd(CDC* pDC)
{
	CRect rc;
	GetClientRect(&rc);
	pDC->FillSolidRect(&rc,0xFFFFFF);
	return FALSE;
}

void CFaceWndCtrl::OnPaint()
{
	CPaintDC dc(this);
	
	Draw(&dc);
}

void CFaceWndCtrl::Draw(CDC *pDC)
{
	CRect rcClient;
	GetClientRect(&rcClient);

	int  cx = rcClient.Width();
	int  cy = rcClient.Height();

	int  nFaceDrawSz     = FACE_SZ+FACE_SPACE;
	int  faces_per_line  = (cx-FACE_SPACE) /(FACE_SZ+FACE_SPACE); 

	int n1 = m_vecDefaultFace.size();
	int n2 = m_vecHistoryFace.size();

	int l1 = n1/faces_per_line ;
	if ( (n1 % faces_per_line)!=0 )
		l1++;

	if ( l1==0 ) 
		l1=1;

	int l2 = n2/faces_per_line ;
	if ( (n2 % faces_per_line)!=0 )
		l2++;

	if (l2==0)
		l2=1;

	CDC dcMem;
	dcMem.CreateCompatibleDC(pDC);

	CBitmap bmMem;
	bmMem.CreateCompatibleBitmap(pDC,cx,this->m_nClientRealHeigh);

	int nSavedMemDC = dcMem.SaveDC();

	dcMem.SelectObject(&bmMem);

	dcMem.FillSolidRect(0,0,cx,m_nClientRealHeigh,0xFFFFFF);
	dcMem.SetBkMode(TRANSPARENT);

	int nTop = 2;
	CString str;

	int nDefaultFaces = m_vecDefaultFace.size();
	if ( nDefaultFaces==0 )
	{
		str = "正在获取头像数据,稍候...";
	}
	else
	{
		str.Format("推荐形象 (共%d个)",nDefaultFaces);
	}

	dcMem.SelectObject(&::g_fontText);
	dcMem.TextOut(3,nTop,str);

	int line,col;

	for ( line=0; line<l1; line++ )
	{
		for ( col=0; col<faces_per_line; col++ )
		{
			int item = line*faces_per_line+col;

			if ( item<n1 )
			{
				CRect & rcFace  = m_vecDefaultFace[item]->rcDraw;
				rcFace.left  = col*nFaceDrawSz+FACE_SPACE;
				rcFace.right = rcFace.left+FACE_SZ;
				rcFace.top   = nTop + line*nFaceDrawSz+TEXT_CY;
				rcFace.bottom=rcFace.top+FACE_SZ;

				rcFace.InflateRect(2,2);
				dcMem.FillSolidRect(&rcFace,RGB(233,222,244));
				if ( m_HotSel.nFacyType==FACE_DEFAULT && m_HotSel.nSel==item )
				{
					dcMem.Draw3dRect(&rcFace,RGB(0,0,255),RGB(0,0,255));
				}

				if ( m_CurrentSel.nFacyType==FACE_DEFAULT && m_CurrentSel.nSel==item )
				{
					dcMem.Draw3dRect(&rcFace,RGB(255,0,0),RGB(255,0,0));
				}

				rcFace.InflateRect(-2,-2);

				if ( m_vecDefaultFace[item]->pBitmap )
				{
					DrawBitmap(&dcMem,rcFace.left,rcFace.top,m_vecDefaultFace[item]->pBitmap,0);
				}				
			}
		}
	}

	nTop += (TEXT_CY+l1*nFaceDrawSz+FACE_SPACE);

	dcMem.Draw3dRect(0,nTop,cx,1,RGB(167,212,231),RGB(212,234,243));

	nTop+=FACE_SPACE;

	str.Format("您已经上传过的形象 (共%d个)",m_vecHistoryFace.size());

	dcMem.TextOut(3,nTop,str);

	for ( line=0; line<l2; line++ )
	{
		for ( col=0; col<faces_per_line; col++ )
		{
			int item = line*faces_per_line+col;

			if ( item<n2 )
			{
				CRect & rcFace = m_vecHistoryFace[item]->rcDraw;
				rcFace.left  = col*nFaceDrawSz+FACE_SPACE;
				rcFace.right = rcFace.left+FACE_SZ;
				rcFace.top   = nTop + line*nFaceDrawSz+TEXT_CY;
				rcFace.bottom=rcFace.top+FACE_SZ;

				rcFace.InflateRect(2,2);
				dcMem.FillSolidRect(&rcFace,RGB(233,222,244));

				if ( m_HotSel.nFacyType==FACE_HISTORY && m_HotSel.nSel==item )
				{
					dcMem.Draw3dRect(&rcFace,RGB(0,0,255),RGB(0,0,255));
				}

				if ( m_CurrentSel.nFacyType==FACE_HISTORY && m_CurrentSel.nSel==item )
				{
					dcMem.Draw3dRect(&rcFace,RGB(255,0,0),RGB(255,0,0));
				}

				rcFace.InflateRect(-2,-2);

				if ( m_vecHistoryFace[item]->pBitmap )
				{
					DrawBitmap(&dcMem,rcFace.left,rcFace.top,m_vecHistoryFace[item]->pBitmap,0);
				}
			}
		}
	}

	int nHeight = m_nClientRealHeigh-m_OriginY;
	pDC->BitBlt(0,0,cx,nHeight,&dcMem,0,m_OriginY,SRCCOPY);


	dcMem.RestoreDC(nSavedMemDC);
}

void CFaceWndCtrl::ClearData()
{
	int n;
	n=m_vecDefaultFace.size();
	for ( int i=0; i<n; i++ )
	{
		tagDefaultFace *pd = m_vecDefaultFace[i];
		delete pd->pBitmap;
		delete pd;
	}
	m_vecDefaultFace.clear();

	n=m_vecHistoryFace.size();
	for ( int i=0; i<n; i++ )
	{
		tagHistoryFace *ph = m_vecHistoryFace[i];
		delete ph->pBitmap;
		delete ph;
	}
	m_vecHistoryFace.clear();

}

void CFaceWndCtrl::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);

	if (m_hWnd)
		CalcScrollInfo();
}


void CFaceWndCtrl::HitTest(const CPoint & point,int &iFaceType, int &iSel)
{
	int n;
	iSel = -1;
	iFaceType =-1;

	CPoint pt = point;
	pt.y += m_OriginY;

	n = m_vecDefaultFace.size();

	for ( int i=0; i<n; i++ )
	{
		tagDefaultFace * td = m_vecDefaultFace[i];

		if ( td->rcDraw.PtInRect(pt) )
		{	
			iSel = i;
			iFaceType= FACE_DEFAULT;
			
			break;
		}
	}

	n = m_vecHistoryFace.size();

	for ( int i=0; i<n; i++ )
	{
		tagHistoryFace * th = m_vecHistoryFace[i];

		if ( th->rcDraw.PtInRect(pt) )
		{
			iSel = i;
			iFaceType= FACE_HISTORY;

			break;
		}
	}
}

void CFaceWndCtrl::GetSel(int &iType, CString &strUrl)
{
	iType=m_CurrentSel.nFacyType;

	if ( iType==FACE_DEFAULT )
	{
		strUrl = m_vecDefaultFace[m_CurrentSel.nSel]->szUrl;
	}
	else
	if ( iType==FACE_HISTORY )
	{
		strUrl = m_vecHistoryFace[m_CurrentSel.nSel]->szUrl;
	}
}

UINT CFaceWndCtrl::LoadWokuheadProc(LPVOID lpParam)
{
	CFaceWndCtrl *pCtrl = (CFaceWndCtrl *)lpParam;

	CString strFile     = g_LocalData.GetPath()+"wokuhead.xml";
	CString strConfig   = UrlGetStr("http://api.feiniu.com/api/wokuhead.xml");

	strConfig.Trim();

	if ( strConfig!="" )
	{
		DeleteFile(strFile);
		CStdioFile f;
		if ( f.Open(strFile,CFile::modeCreate|CFile::modeWrite|CFile::typeText) )
		{
			f.WriteString(strConfig);
			f.Close();
		}
	}
		
	map<int,CString> mapDefaultFace;
	ParaseFaceList(strConfig,mapDefaultFace);

	pCtrl->SendMessage(WM_THREAD_NOTIFY,0,(LPARAM)&mapDefaultFace);

	return 0;
}

void CFaceWndCtrl::InitData()
{
	m_HotSel.nFacyType = m_HotSel.nSel = m_CurrentSel.nFacyType = m_CurrentSel.nSel = -1;
	m_nClientRealHeigh = 0;

	//初始化本地上传过的头像
	CStringList slHistoryFace;
	g_LocalData.GetHistoryFaceList(slHistoryFace);
	
	for ( int i=0; i<slHistoryFace.GetCount(); i++ )
	{
		tagHistoryFace *ph = new tagHistoryFace;
		strcpy(ph->szUrl,(char *)(LPCSTR)slHistoryFace.GetAt( slHistoryFace.FindIndex(i) ));
		m_vecHistoryFace.push_back(ph);

		CString strUrl = HistoryFaceUrl2WebUrl(ph->szUrl);

		g_webFileProcess.GetWebFile(MAKELPARAM(i,FACE_HISTORY), 
					(char *)(LPCSTR)strUrl,
					m_hWnd);
	}


	CString strFaceFile = g_LocalData.GetPath()+"wokuhead.xml";

	CStdioFile  f;
	CString		strTemp;

	if ( f.Open(strFaceFile,CFile::modeRead|CFile::typeText) )
	{
		CString s;
		while ( f.ReadString(strTemp) )
		{
			strTemp = strTemp+s;
		}
		f.Close();
	}
	
	map<int,CString> mapDefaultFace;
	ParaseFaceList(strTemp,mapDefaultFace);

	map<int,CString>::iterator it;

	for ( it=mapDefaultFace.begin(); it!=mapDefaultFace.end(); it++ )
	{
		tagDefaultFace *pd = new tagDefaultFace;

		pd->nFaceId = it->first;
		strcpy(pd->szUrl,(char *)(LPCSTR)it->second);
		m_vecDefaultFace.push_back(pd);
		g_webFileProcess.GetWebFile(MAKELPARAM(pd->nFaceId,FACE_DEFAULT), 
						pd->szUrl,
						m_hWnd);
	}

	DWORD dwThreadId;

	if ( m_hWokuheadProc==NULL )
		m_hWokuheadProc = CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)LoadWokuheadProc,this,0,&dwThreadId);

	CalcScrollInfo();
}

void CFaceWndCtrl::CalcScrollInfo()
{
	if (IsWindow(m_hWnd)) 
	{
		CRect rcClient;
		GetClientRect(&rcClient);

		int cx = rcClient.Width();
		int cy = rcClient.Height();

		int nPreRealHeigh   = m_nClientRealHeigh;
		int faces_per_line  = (cx-FACE_SPACE) /(FACE_SZ+FACE_SPACE);

		int n1 = m_vecDefaultFace.size();
		int n2 = m_vecHistoryFace.size();

		int l1 = n1/faces_per_line ;
		if ( (n1 % faces_per_line)!=0 )
			l1++;

		if ( l1==0 ) 
			l1=1;

		int l2 = n2/faces_per_line ;
		if ( (n2 % faces_per_line)!=0 )
			l2++;

		if (l2==0)
			l2=1;

		m_nClientRealHeigh = 2+2*TEXT_CY+2*FACE_SPACE+(l1+l2)*(FACE_SZ+FACE_SPACE);

		SCROLLINFO si;
		memset(&si, 0, sizeof(si));
		si.cbSize = sizeof(si);
		si.fMask  = SIF_PAGE | SIF_RANGE;
		si.nPage  = rcClient.Height();
		si.nMin = 0;
		si.nMax = m_nClientRealHeigh - 1;
		if (si.nPage >= si.nMax) 
		{
			ShowScrollBar(SB_VERT, FALSE);
		}
		else
		if ( nPreRealHeigh!=m_nClientRealHeigh )
		{
			CWnd::SetScrollInfo(SB_VERT, &si);
		}
	}
}

LRESULT CFaceWndCtrl::OnThreadNotify(WPARAM wParam, LPARAM lParam)
{
	map<int,CString> & mapDefaultFace = *(map<int,CString> *)lParam;

	if ( mapDefaultFace.size()>0 )
	{
		int n;
		n=m_vecDefaultFace.size();
		for ( int i=0; i<n; i++ )
		{
			tagDefaultFace *pd = m_vecDefaultFace[i];
			delete pd->pBitmap;
			delete pd;
		}
		m_vecDefaultFace.clear();

		map<int,CString>::iterator it;

		for ( it=mapDefaultFace.begin(); it!=mapDefaultFace.end(); it++ )
		{
			tagDefaultFace *pd = new tagDefaultFace;

			pd->nFaceId = it->first;
			strcpy(pd->szUrl,(char *)(LPCSTR)it->second);
			m_vecDefaultFace.push_back(pd);
			g_webFileProcess.GetWebFile(MAKELPARAM(pd->nFaceId,FACE_DEFAULT), 
							pd->szUrl,
							m_hWnd);


			CalcScrollInfo();
		}
	}


	CloseHandle(m_hWokuheadProc);
	m_hWokuheadProc = NULL;

	return 0;
}

LRESULT CFaceWndCtrl::OnWebFileNotify(WPARAM wParam, LPARAM lParam)
{
	WORD wHi = HIWORD(wParam);
	WORD wLo = LOWORD(wParam);

	char * szFile = (char *)lParam;

	if ( wHi==FACE_DEFAULT )
	{
		int nFaceId = wLo;
		int n = m_vecDefaultFace.size();
		for ( int i=0; i<n; i++ )
		{
			if ( m_vecDefaultFace[i]->nFaceId==nFaceId )
			{
				if ( m_vecDefaultFace[i]->pBitmap )
				{
					delete m_vecDefaultFace[i]->pBitmap;
				}

				m_vecDefaultFace[i]->pBitmap = CBitmapFromFile(szFile,FACE_SZ,FACE_SZ);
				Invalidate();
				break;
			}
		}
	}
	else
	if ( wHi==FACE_HISTORY )
	{
		int item = wLo;
		m_vecHistoryFace[item]->pBitmap = CBitmapFromFile(szFile,FACE_SZ,FACE_SZ);
		Invalidate();
	}

	return 0;
}

void CFaceWndCtrl::OnMouseMove(UINT nFlags, CPoint point)
{
	int iSel , iFaceType;
	HitTest(point,iFaceType,iSel);

	if ( m_HotSel.nSel!=iSel || m_HotSel.nFacyType!=iFaceType )
	{
		m_HotSel.nSel = iSel;
		m_HotSel.nFacyType = iFaceType;
		ReDraw();
	}

	if ( m_bMouseOver==false )
	{
		m_bMouseOver = true;
		SetTimer(0x01,10,NULL);
	}
    
	CWnd::OnMouseMove(nFlags, point);
}

void CFaceWndCtrl::OnLButtonDown(UINT nFlags, CPoint point)
{
	int iSel , iFaceType;
	HitTest(point,iFaceType,iSel);

	if ( m_CurrentSel.nSel!=iSel || m_CurrentSel.nFacyType!=iFaceType )
	{
		m_CurrentSel.nSel = iSel;
		m_CurrentSel.nFacyType = iFaceType;
		ReDraw();
		
		GetParent()->PostMessage(WM_COMMAND,GetDlgCtrlID(),0);
	}

	CWnd::OnLButtonDown(nFlags, point);
}

void CFaceWndCtrl::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	CWnd::OnLButtonUp(nFlags, point);
}

void CFaceWndCtrl::OnTimer(UINT nIDEvent)
{
	if ( nIDEvent==0x01 )
	{
		CPoint pt;
		GetCursorPos(&pt);
		ScreenToClient(&pt);
		int iFaceType,iSel;
		HitTest(pt,iFaceType,iSel);
			
		if ( m_HotSel.nFacyType!=iFaceType || m_HotSel.nSel!=iSel )
		{
			m_HotSel.nFacyType=iFaceType;
			m_HotSel.nSel=iSel;
			ReDraw();

			m_bMouseOver = false;
			KillTimer(0x01);
		}
	}

	CWnd::OnTimer(nIDEvent);
}

void CFaceWndCtrl::SetSel(int iType, CString strUrl)
{
	if ( iType==FACE_HISTORY )
	{
		int n=m_vecHistoryFace.size();
		for ( int i=0; i<n; i++ )
		{
			tagHistoryFace *th = m_vecHistoryFace[i];
			CString s1=th->szUrl;
			s1.Trim();
			s1.MakeLower();
			strUrl.Trim();
			strUrl.MakeLower();

			if ( s1==strUrl )
			{
				m_CurrentSel.nFacyType = FACE_HISTORY;
				m_CurrentSel.nSel = i;
				ReDraw();

				GetParent()->PostMessage(WM_COMMAND,GetDlgCtrlID(),0);

				break;
			}
		}
	}
}


void CFaceWndCtrl::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	CRect rcClient;
	GetClientRect(&rcClient);

	SetFocus();

	switch (nSBCode) 
	{
	case SB_TOP:
		OnVScroll(SB_LINEUP, nPos, pScrollBar);
		break;
	case  SB_BOTTOM:
		OnVScroll(SB_LINEDOWN, nPos, pScrollBar);
		break;
	case SB_LINEUP:
		if (m_OriginY > 0) 
		{
			CWnd::SetScrollPos(SB_VERT, GetScrollPos(SB_VERT) - LINE_UP_CAP);
			m_OriginY -= LINE_UP_CAP;
			if (m_OriginY < 0) 
			{
				m_OriginY = 0;
			}

			ReDraw();
		}
		break;
	case SB_LINEDOWN:
		if (m_OriginY + rcClient.Height() < m_nClientRealHeigh) 
		{
			CWnd::SetScrollPos(SB_VERT, GetScrollPos(SB_VERT) + LINE_UP_CAP);
			m_OriginY += LINE_UP_CAP;

			OnPaint();
		}
		break;
	case SB_PAGEDOWN:
		if (m_OriginY + rcClient.Height() < m_nClientRealHeigh) 
		{
			CWnd::SetScrollPos(SB_VERT, GetScrollPos(SB_VERT) + rcClient.Height());
			m_OriginY += rcClient.Height();

			ReDraw();
		}
		break;
	case SB_PAGEUP:
		if (m_OriginY > 0) 
		{
			CWnd::SetScrollPos(SB_VERT, GetScrollPos(SB_VERT) - rcClient.Height());
			m_OriginY -= rcClient.Height();
			if (m_OriginY < 0) 
			{
				m_OriginY = 0;
			}

			ReDraw();
		}
		break;
	case SB_THUMBPOSITION:
		CWnd::SetScrollPos(SB_VERT, nPos);
		m_OriginY = m_OriginY / rcClient.Height() + nPos;		
		OnPaint();
		break;
	case SB_THUMBTRACK:
		if (nPos < m_nClientRealHeigh) 
		{
			m_OriginY = nPos;
			//m_OriginY += LINE_UP_CAP;
			ReDraw();
		}

		CWnd::SetScrollPos(SB_VERT, nPos);
		break;
	default:
		;
	}

	//	CWnd::OnVScroll(nSBCode, nPos, pScrollBar);
}

