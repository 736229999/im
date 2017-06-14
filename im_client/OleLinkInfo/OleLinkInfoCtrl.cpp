// OleLinkInfoCtrl.cpp : COleLinkInfoCtrl ActiveX 控件类的实现。

#include "stdafx.h"
#include "OleLinkInfo.h"
#include "OleLinkInfoCtrl.h"
#include "OleLinkInfoPropPage.h"
#include "afxdialogex.h"
#include "GraphicsRoundRectPath.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


IMPLEMENT_DYNCREATE(COleLinkInfoCtrl, COleControl)



// 消息映射

BEGIN_MESSAGE_MAP(COleLinkInfoCtrl, COleControl)
	ON_OLEVERB(AFX_IDS_VERB_PROPERTIES, OnProperties)
END_MESSAGE_MAP()



// 调度映射

BEGIN_DISPATCH_MAP(COleLinkInfoCtrl, COleControl)
	DISP_FUNCTION_ID(COleLinkInfoCtrl, "SetPicFilePath", dispidSetPicFilePath, SetPicFilePath, VT_EMPTY, VTS_BSTR)
	DISP_FUNCTION_ID(COleLinkInfoCtrl, "SetHeadText", dispidSetHeadText, SetHeadText, VT_EMPTY, VTS_BSTR)
	DISP_FUNCTION_ID(COleLinkInfoCtrl, "SetContentText", dispidSetContentText, SetContentText, VT_EMPTY, VTS_BSTR)
	DISP_FUNCTION_ID(COleLinkInfoCtrl, "IsPointInLinkRect", dispidIsPointInLinkRect, IsPointInLinkRect, VT_I4, VTS_I4 VTS_I4 VTS_I4 VTS_I4)
	DISP_FUNCTION_ID(COleLinkInfoCtrl, "SetUrlText", dispidSetUrlText, SetUrlText, VT_EMPTY, VTS_BSTR)
	DISP_FUNCTION_ID(COleLinkInfoCtrl, "GetUrlText", dispidGetUrlText, GetUrlText, VT_BSTR, VTS_NONE)
END_DISPATCH_MAP()



// 事件映射

BEGIN_EVENT_MAP(COleLinkInfoCtrl, COleControl)
END_EVENT_MAP()



// 属性页

// TODO: 按需要添加更多属性页。请记住增加计数!
BEGIN_PROPPAGEIDS(COleLinkInfoCtrl, 1)
	PROPPAGEID(COleLinkInfoPropPage::guid)
END_PROPPAGEIDS(COleLinkInfoCtrl)



// 初始化类工厂和 guid

IMPLEMENT_OLECREATE_EX(COleLinkInfoCtrl, "OLELINKINFO.OleLinkInfoCtrl.1",
	0x7eedabb0, 0xa9d2, 0x4187, 0x86, 0x4a, 0xa4, 0x8e, 0x48, 0xea, 0x93, 0xe1)



// 键入库 ID 和版本

IMPLEMENT_OLETYPELIB(COleLinkInfoCtrl, _tlid, _wVerMajor, _wVerMinor)



// 接口 ID

const IID IID_DOleLinkInfo = { 0x5C8FA553, 0x98DF, 0x4A73, { 0x92, 0x8, 0x7F, 0xF1, 0xBD, 0xF0, 0xC5, 0x5B } };
const IID IID_DOleLinkInfoEvents = { 0x99D5104D, 0x2A9C, 0x4F64, { 0xB3, 0xD0, 0xAE, 0x29, 0x53, 0xEE, 0xBD, 0x1C } };


// 控件类型信息

static const DWORD _dwOleLinkInfoOleMisc =
	OLEMISC_ACTIVATEWHENVISIBLE |
	OLEMISC_SETCLIENTSITEFIRST |
	OLEMISC_INSIDEOUT |
	OLEMISC_CANTLINKINSIDE |
	OLEMISC_RECOMPOSEONRESIZE;

IMPLEMENT_OLECTLTYPE(COleLinkInfoCtrl, IDS_OLELINKINFO, _dwOleLinkInfoOleMisc)



// COleLinkInfoCtrl::COleLinkInfoCtrlFactory::UpdateRegistry -
// 添加或移除 COleLinkInfoCtrl 的系统注册表项

BOOL COleLinkInfoCtrl::COleLinkInfoCtrlFactory::UpdateRegistry(BOOL bRegister)
{
	// TODO: 验证您的控件是否符合单元模型线程处理规则。
	// 有关更多信息，请参考 MFC 技术说明 64。
	// 如果您的控件不符合单元模型规则，则
	// 必须修改如下代码，将第六个参数从
	// afxRegApartmentThreading 改为 0。

	if (bRegister)
		return AfxOleRegisterControlClass(
			AfxGetInstanceHandle(),
			m_clsid,
			m_lpszProgID,
			IDS_OLELINKINFO,
			IDB_OLELINKINFO,
			afxRegApartmentThreading,
			_dwOleLinkInfoOleMisc,
			_tlid,
			_wVerMajor,
			_wVerMinor);
	else
		return AfxOleUnregisterClass(m_clsid, m_lpszProgID);
}



// COleLinkInfoCtrl::COleLinkInfoCtrl - 构造函数

COleLinkInfoCtrl::COleLinkInfoCtrl()
{
	InitializeIIDs(&IID_DOleLinkInfo, &IID_DOleLinkInfoEvents);
	// TODO: 在此初始化控件的实例数据。
	GdiplusStartupInput m_gdiplusStartupInput;
	GdiplusStartup(&m_pGdiToken,&m_gdiplusStartupInput,NULL);
}



// COleLinkInfoCtrl::~COleLinkInfoCtrl - 析构函数

COleLinkInfoCtrl::~COleLinkInfoCtrl()
{
	// TODO: 在此清理控件的实例数据。
	GdiplusShutdown(m_pGdiToken);
}



// COleLinkInfoCtrl::OnDraw - 绘图函数

void COleLinkInfoCtrl::OnDraw(
			CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid)
{
	if (!pdc)
		return;
	// TODO: 用您自己的绘图代码替换下面的代码。
	//pdc->FillRect(rcBounds, CBrush::FromHandle((HBRUSH)GetStockObject(WHITE_BRUSH)));
	//pdc->Ellipse(rcBounds);

	pdc->FillRect(rcBounds, CBrush::FromHandle((HBRUSH)GetStockObject(WHITE_BRUSH)));

 	CGraphicsRoundRectPath objRoundPath(rcBounds.left, rcBounds.top, rcBounds.Width()-1, rcBounds.Height()-1, 8, 8);
 	Graphics graphics( pdc->m_hDC);

	SolidBrush brushPath(Color(255, 245, 245, 245));//颜色 字体
	graphics.FillPath(&brushPath, &objRoundPath);

	Pen pen(Color(255, 221, 221, 221), 1);
	graphics.DrawPath(&pen, &objRoundPath);

	m_rcPic.left = rcBounds.left + 10;
	m_rcPic.top = rcBounds.top + 10;
	m_rcPic.right = m_rcPic.left + 80;
	m_rcPic.bottom = m_rcPic.top + 80;


	if(m_strPicFilePath != "")
	{
		WCHAR wstrPicFilePath[255];
		MultiByteToWideChar(CP_ACP,0,(LPCSTR)m_strPicFilePath,-1, wstrPicFilePath,255);
		Image image(wstrPicFilePath, FALSE);
		graphics.DrawImage(&image, m_rcPic.left, m_rcPic.top, m_rcPic.Width(), m_rcPic.Height());  //绘制图片
	}
	m_rcHead.left = m_rcPic.right + 10;
	m_rcHead.right = m_rcHead.left + 300;
	m_rcHead.top = m_rcPic.top + 5;
	m_rcHead.bottom = m_rcHead.top + 20;

	CFont fontHead;
	fontHead.CreateFont(
		22, // nHeight
		0, // nWidth
		0, // nEscapement
		0, // nOrientation
		FW_BOLD, // nWeight
		FALSE, // bItalic
		FALSE, // bUnderline
		0, // cStrikeOut
		ANSI_CHARSET, // nCharSet
		OUT_DEFAULT_PRECIS, // nOutPrecision
		CLIP_DEFAULT_PRECIS, // nClipPrecision
		DEFAULT_QUALITY, // nQuality
		DEFAULT_PITCH | FF_SWISS,
		_T("微软雅黑") // nPitchAndFamily Arial
		); 
	DWORD dwColorHead = RGB(70, 139, 182);
	pdc->SelectObject(&fontHead);
	pdc->SetTextColor(dwColorHead);
	if(m_strHead != "")
	{
		//pdc->DrawText(m_strHead, &m_rcHead, DT_END_ELLIPSIS|DT_TOP|DT_LEFT|DT_WORDBREAK);
		FontFamily CompFontFamily(L"微软雅黑");
		Gdiplus::Font cFont(&CompFontFamily, 15, FontStyleBold, UnitPixel);
		PointF pt(m_rcHead.left, m_rcHead.top);
		SolidBrush brush(Color(255, 70, 139, 182));//颜色 字体
		PointF ptNew;
		RectF rectNew;

		WCHAR wstrHead[255];
		MultiByteToWideChar(CP_ACP,0,(LPCSTR)m_strHead,-1, wstrHead,255);
		graphics.MeasureString(wstrHead, -1, &cFont, ptNew, &rectNew);
		if(rectNew.Width + m_rcHead.left > rcBounds.right)
		{
			CString strHead1 = _T("");
			CString strHead2 = _T("");
			int nIndex = 0;
			PointF ptNew1;
			RectF rectNew1;
			PointF ptNew2;
			RectF rectNew2;
			for(; nIndex < m_strHead.GetLength(); nIndex++)
			{
				BYTE bCharIndex = m_strHead.GetAt(nIndex);
				if(bCharIndex > 127)
				{
					strHead1 += m_strHead.GetAt(nIndex);
					nIndex++;
				}
				strHead1 += m_strHead.GetAt(nIndex);

				WCHAR wstrHead1[255];
				MultiByteToWideChar(CP_ACP,0,(LPCSTR)strHead1,-1, wstrHead1,255);
				graphics.MeasureString(wstrHead1, -1, &cFont, ptNew1, &rectNew1);
				if(rectNew1.Width + m_rcHead.left > rcBounds.right)
				{
					BYTE bChar = m_strHead.GetAt(nIndex);
					if(bChar > 127)
					{
						strHead1 = strHead1.Left(strHead1.GetLength()-2);
						nIndex--;
					}
					else
						strHead1 = strHead1.Left(strHead1.GetLength()-1);
					break;
				}
			}
			for(; nIndex <  m_strHead.GetLength(); nIndex++)
			{
				BYTE bCharIndex = m_strHead.GetAt(nIndex);
				if(bCharIndex > 127)
				{
					strHead2 += m_strHead.GetAt(nIndex);
					nIndex++;
				}
				strHead2 += m_strHead.GetAt(nIndex);

				WCHAR wstrHead2[255];
				MultiByteToWideChar(CP_ACP,0,(LPCSTR)strHead2,-1, wstrHead2,255);
				graphics.MeasureString(wstrHead2, -1, &cFont, ptNew2, &rectNew2);
				if(rectNew2.Width + m_rcHead.left > rcBounds.right)
				{
					if(bCharIndex > 127)
					{
						strHead2 = strHead2.Left(strHead2.GetLength()-2);
					}
					for(int nLast = 0; nLast < 3; nLast++)
					{
						BYTE bChar = strHead2.GetAt(strHead2.GetLength()-1);
						if(bChar > 127)
						{
							nLast++;
							strHead2 = strHead2.Left(strHead2.GetLength()-2);
						}
						else
							strHead2 = strHead2.Left(strHead2.GetLength()-1);
					}
					strHead2 = strHead2 + _T("...");
					WCHAR wstrHead2[255];
					MultiByteToWideChar(CP_ACP,0,(LPCSTR)strHead2,-1, wstrHead2,255);
					graphics.MeasureString(wstrHead2, -1, &cFont, ptNew2, &rectNew2);
					break;
				}
			}
			if(strHead2 != _T(""))
			{
				m_rcHead.bottom = m_rcHead.bottom + rectNew.Height;
				WCHAR wstrHead1[255];
				MultiByteToWideChar(CP_ACP,0,(LPCSTR)strHead1,-1, wstrHead1,255);
				graphics.DrawString(wstrHead1, -1, &cFont, pt, &brush);
				PointF pt2(m_rcHead.left, m_rcHead.top + rectNew.Height);
				WCHAR wstrHead2[255];
				MultiByteToWideChar(CP_ACP,0,(LPCSTR)strHead2,-1, wstrHead2,255);
				graphics.DrawString(wstrHead2, -1, &cFont, pt2, &brush);
			}
			m_rcHead.right = rectNew1.Width > rectNew2.Width ? m_rcHead.left + rectNew1.Width : m_rcHead.left + rectNew2.Width;
		}
		else
		{
			WCHAR wstrHead[255];
			MultiByteToWideChar(CP_ACP,0,(LPCSTR)m_strHead,-1, wstrHead,255);
			graphics.DrawString(wstrHead, -1, &cFont, pt, &brush);
			m_rcHead.right = m_rcHead.left + rectNew.Width;
		}
	}

	CFont fontContent;
	fontContent.CreateFont(
		18, // nHeight
		0, // nWidth
		0, // nEscapement
		0, // nOrientation
		FW_NORMAL, // nWeight
		FALSE, // bItalic
		FALSE, // bUnderline
		0, // cStrikeOut
		ANSI_CHARSET, // nCharSet
		OUT_DEFAULT_PRECIS, // nOutPrecision
		CLIP_DEFAULT_PRECIS, // nClipPrecision
		DEFAULT_QUALITY, // nQuality
		DEFAULT_PITCH | FF_SWISS,
		_T("微软雅黑") // nPitchAndFamily Arial
		); 
	DWORD dwColorContent = RGB(0, 0, 0);
	pdc->SelectObject(&fontContent);
	pdc->SetTextColor(dwColorContent);

	m_rcContent1.left = m_rcHead.left;
	m_rcContent1.right = m_rcContent1.left + 300;
	m_rcContent1.top = m_rcHead.bottom + 5;
	m_rcContent1.bottom = m_rcContent1.top + 20;
	if(m_strContent != "")
	{
		//pdc->DrawText(m_strContent, &m_rcContent1, DT_END_ELLIPSIS|DT_TOP|DT_LEFT|DT_WORDBREAK);
		FontFamily CompFontFamily(L"微软雅黑");
		Gdiplus::Font cFont(&CompFontFamily, 12, FontStyleRegular, UnitPixel);
		PointF pt(m_rcContent1.left, m_rcContent1.top);
		SolidBrush brush(Color(255, 0, 0, 0));//颜色 字体
		PointF ptNew;
		RectF rectNew;
		WCHAR wstrContent[255];
		MultiByteToWideChar(CP_ACP,0,(LPCSTR)m_strContent,-1, wstrContent,255);
		graphics.MeasureString(wstrContent, -1, &cFont, ptNew, &rectNew);
		if(rectNew.Width + m_rcContent1.left > rcBounds.right)
		{
			CString strContent1 = _T("");
			CString strContent2 = _T("");
			int nIndex = 0;
			for(; nIndex < m_strContent.GetLength(); nIndex++)
			{
				BYTE bCharIndex = m_strContent.GetAt(nIndex);
				if(bCharIndex > 127)
				{
					strContent1 += m_strContent.GetAt(nIndex);
					nIndex++;
				}
				strContent1 += m_strContent.GetAt(nIndex);
				
				PointF ptNew1;
				RectF rectNew1;
				WCHAR wstrContent1[255];
				MultiByteToWideChar(CP_ACP,0,(LPCSTR)strContent1,-1, wstrContent1,255);
				graphics.MeasureString(wstrContent1, -1, &cFont, ptNew1, &rectNew1);
				if(rectNew1.Width + m_rcContent1.left > rcBounds.right)
				{
					BYTE bChar = m_strContent.GetAt(nIndex);
					if(bChar > 127)
					{
						strContent1 = strContent1.Left(strContent1.GetLength()-2);
						nIndex--;
					}
					else
						strContent1 = strContent1.Left(strContent1.GetLength()-1);
					break;
				}
			}
			for(; nIndex <  m_strContent.GetLength(); nIndex++)
			{
				BYTE bCharIndex = m_strContent.GetAt(nIndex);
				if(bCharIndex > 127)
				{
					strContent2 += m_strContent.GetAt(nIndex);
					nIndex++;
				}
				strContent2 += m_strContent.GetAt(nIndex);
				PointF ptNew2;
				RectF rectNew2;
				WCHAR wstrContent2[255];
				MultiByteToWideChar(CP_ACP,0,(LPCSTR)strContent2,-1, wstrContent2,255);
				graphics.MeasureString(wstrContent2, -1, &cFont, ptNew2, &rectNew2);
				if(rectNew2.Width + m_rcContent1.left > rcBounds.right)
				{
					if(bCharIndex > 127)
					{
						strContent2 = strContent2.Left(strContent2.GetLength()-2);
					}
					for(int nLast = 0; nLast < 3; nLast++)
					{
						BYTE bChar = strContent2.GetAt(strContent2.GetLength()-1);
						if(bChar > 127)
						{
							nLast++;
							strContent2 = strContent2.Left(strContent2.GetLength()-2);
						}
						else
							strContent2 = strContent2.Left(strContent2.GetLength()-1);
					}
					strContent2 = strContent2 + _T("...");
					break;
				}
			}
			if(strContent2 != _T(""))
			{
				m_rcContent1.bottom = m_rcContent1.bottom + rectNew.Height;
				WCHAR wstrContent1[255];
				MultiByteToWideChar(CP_ACP,0,(LPCSTR)strContent1,-1, wstrContent1,255);
				graphics.DrawString(wstrContent1, -1, &cFont, pt, &brush);
				PointF pt2(m_rcContent1.left, m_rcContent1.top + rectNew.Height);
				WCHAR wstrContent2[255];
				MultiByteToWideChar(CP_ACP,0,(LPCSTR)strContent2,-1, wstrContent2,255);
				graphics.DrawString(wstrContent2, -1, &cFont, pt2, &brush);
			}
		}
		else
		{
			WCHAR wstrContent[255];
			MultiByteToWideChar(CP_ACP,0,(LPCSTR)m_strContent,-1, wstrContent,255);
			graphics.DrawString(wstrContent, -1, &cFont, pt, &brush);
		}
	}

	/*m_rcContent2.left = m_rcHead.left;
	m_rcContent2.right = m_rcContent2.left + 300;
	m_rcContent2.top = m_rcContent1.bottom;
	m_rcContent2.bottom = m_rcContent2.top + 20;
	if(m_strContent != "")
	pdc->DrawText(m_strContent, &m_rcContent2, DT_END_ELLIPSIS|DT_TOP|DT_LEFT|DT_WORDBREAK);*/
}



// COleLinkInfoCtrl::DoPropExchange - 持久性支持

void COleLinkInfoCtrl::DoPropExchange(CPropExchange* pPX)
{
	ExchangeVersion(pPX, MAKELONG(_wVerMinor, _wVerMajor));
	COleControl::DoPropExchange(pPX);

	// TODO: 为每个持久的自定义属性调用 PX_ 函数。
}



// COleLinkInfoCtrl::OnResetState - 将控件重置为默认状态

void COleLinkInfoCtrl::OnResetState()
{
	COleControl::OnResetState();  // 重置 DoPropExchange 中找到的默认值

	// TODO: 在此重置任意其他控件状态。
}



// COleLinkInfoCtrl 消息处理程序

BOOL COleLinkInfoCtrl::IsInvokeAllowed(DISPID dispid)
{
	return TRUE;
}

void COleLinkInfoCtrl::SetPicFilePath(LPCTSTR strFilePath)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加调度处理程序代码
	m_strPicFilePath = strFilePath;
}


void COleLinkInfoCtrl::SetHeadText(LPCTSTR strHeadText)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加调度处理程序代码
	m_strHead = strHeadText;
}


void COleLinkInfoCtrl::SetContentText(LPCTSTR strContentText)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加调度处理程序代码
	m_strContent = strContentText;
}


LONG COleLinkInfoCtrl::IsPointInLinkRect(LONG olex, LONG oley, LONG x, LONG y)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加调度处理程序代码
	CRect rcPic;
	rcPic.left = olex + 10;
	rcPic.top = oley + 10;
	rcPic.right = rcPic.left + 80;
	rcPic.bottom = rcPic.top + 80;

	if(x > rcPic.left && x < rcPic.right && y > rcPic.top && y < rcPic.bottom)
		return 1;

	CRect rcHead;
	rcHead.left = rcPic.right + 10;
	rcHead.right = rcHead.left + m_rcHead.Width();
	rcHead.top = rcPic.top + 5;
	rcHead.bottom = rcHead.top + m_rcHead.Height();

	if(x > rcHead.left && x < rcHead.right && y > rcHead.top && y < rcHead.bottom)
		return 1;

	
	return 0;
}


void COleLinkInfoCtrl::SetUrlText(LPCTSTR strUrlText)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加调度处理程序代码
	m_strUrl = strUrlText;
}


BSTR COleLinkInfoCtrl::GetUrlText(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString strResult;

	// TODO: 在此添加调度处理程序代码
	strResult = m_strUrl;
	return strResult.AllocSysString();
}
