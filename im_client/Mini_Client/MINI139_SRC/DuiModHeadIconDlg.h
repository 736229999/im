#pragma once
#include "..\..\..\DuiLib\UIlib.h"
#include "..\..\..\DuiLib\Control\UIButtonGif.h"
#include "Im\IMProtocol.h"
#include "im/RectTrackerEx.h"
class CDuiModHeadIconDlg : public DuiLib::WindowImplBase
{
	DECLARE_XT_MESSAGE_MAP()
public:
	CDuiModHeadIconDlg(void);
	~CDuiModHeadIconDlg(void);

public:
	virtual LPCTSTR    GetWindowClassName() const			{   return _T("ModHeadIconDlg");  }
	virtual DuiLib::CDuiString GetSkinFile()                {   return _T("ModHeadIconDlg.xml");    }
	virtual DuiLib::CDuiString GetSkinFolder()              {   return DuiLib::CPaintManagerUI::GetInstancePath();  }
	DuiLib::UILIB_RESOURCETYPE GetResourceType() const;
	virtual LRESULT HandleMessage(UINT uMsg,WPARAM wParam,LPARAM lParam);
	LRESULT HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual LRESULT OnTimer(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LPCTSTR GetResourceID() const;
	virtual void InitWindow();
	void OnHeadFileUploadAck(const char *data,WORD wRet);
	//void AddGroup(LPCTSTR szText);
	//void AddFriend(LPCTSTR szImgPath, LPCTSTR szText, unsigned int id, DWORD dwTextColor = 0x2E2E2E);
	void Notify(DuiLib::TNotifyUI& msg);
	static void  UpdateBk();
	static std::tr1::shared_ptr<CDuiModHeadIconDlg> Show();
private:
	static std::tr1::shared_ptr<CDuiModHeadIconDlg> pInstance;
	////此处加载自定义控件
    DuiLib::CControlUI* CreateControl(LPCTSTR pstrClass)
	{
		if (_tcsicmp(pstrClass, _T("ButtonGif")) == 0)
		{
			return	new DuiLib::CButtonGifUI();
		}
		return NULL;
	}

public:
	std::map<CString, CString> m_mapDefHeadLocal_Url;
	DuiLib::CButtonGifUI* m_pHLPreviewHead;
	DuiLib::CTabLayoutUI* m_tabSelfInfo;
	DuiLib::COptionUI* m_btnDefaultHead;
	DuiLib::COptionUI* m_btnCustomHead;
	DuiLib::CButtonUI* m_btnUpload;
	DuiLib::CLabelUI* m_labelPicture;
	DuiLib::CButtonUI* m_btnSave;
	DuiLib::CButtonUI* m_btnCancel;

	void ClickedBtnSelectpic();

	int m_nTrackerRectWidth;//预览头像宽度，88
	int m_nTrackerRectHeight;//预览头像高度，88

	CRectTrackerEx m_rectTracker;//绘制框选区域的边框
	CRect m_rect;
	CRect m_rcNew;
	CBitmap m_bitmap;//背景图片，用于刷新背景
	CString m_strFilePath;
	CString m_strDefHead;
	IMAGE_FILE_STRUCT m_imageFile;
	vector<SEND_FILE_STRUCT *>  m_vecSendFiles;
public:
	ULONG_PTR m_pGdiToken;//gdi初始化
	void OnMouseMove(UINT nFlags, DuiLib::CPoint point);

	int m_nHandle;//判断当前鼠标状态，根据鼠标状态绘制鼠标样式
	CRect m_rectLast;//选择区域变化前的区域，用于计算需要刷新的区域
	//鼠标移动的位置,用于拖动选择区域或改变选择区域的大小
	int m_nDownX;
	int m_nDownY;
	int m_nMoveX;
	int m_nMoveY;

private:
	CString m_strChoseFilePath;
	BOOL m_bGifFlag;
};

