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
	////�˴������Զ���ؼ�
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

	int m_nTrackerRectWidth;//Ԥ��ͷ���ȣ�88
	int m_nTrackerRectHeight;//Ԥ��ͷ��߶ȣ�88

	CRectTrackerEx m_rectTracker;//���ƿ�ѡ����ı߿�
	CRect m_rect;
	CRect m_rcNew;
	CBitmap m_bitmap;//����ͼƬ������ˢ�±���
	CString m_strFilePath;
	CString m_strDefHead;
	IMAGE_FILE_STRUCT m_imageFile;
	vector<SEND_FILE_STRUCT *>  m_vecSendFiles;
public:
	ULONG_PTR m_pGdiToken;//gdi��ʼ��
	void OnMouseMove(UINT nFlags, DuiLib::CPoint point);

	int m_nHandle;//�жϵ�ǰ���״̬���������״̬���������ʽ
	CRect m_rectLast;//ѡ������仯ǰ���������ڼ�����Ҫˢ�µ�����
	//����ƶ���λ��,�����϶�ѡ�������ı�ѡ������Ĵ�С
	int m_nDownX;
	int m_nDownY;
	int m_nMoveX;
	int m_nMoveY;

private:
	CString m_strChoseFilePath;
	BOOL m_bGifFlag;
};

