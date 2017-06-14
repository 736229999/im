#pragma once

#include "..\resource.h"
#include "buttonext.h"
#include "p2pSendmsgDlg.h"
#include "DuiP2PSendMsgDlg.h"
#include "RichEditCtrlEx.h"

#define IDC_LINK_ADD		0	// �Զ�����鰴ť
#define IDC_LINK_PRE		1	// ��һҳ��ť
#define IDC_LINK_NEXT		2	// ��һҳ��ť
#define IDC_LINK_UPDATE		3	// ����վ���°�ť

class CUSTOM_EMOTION
{
public:
	enum EMOTION_PIC_TYPE
	{
		CUSTOM = 0,	// �Զ������
		WEB,		// web����
		SYS,		// ���ñ���
		SYS_GIF		// �Դ�����
	};
	char type;		// EMOTION_PIC_TYPE
	int	 id;
	char szID[50];
	char szShortCut[50];
	char szTip[50];
	char szFile[255];
	char szUrl[255];
	CUSTOM_EMOTION()
	{
		memset(this,0,sizeof(CUSTOM_EMOTION));
	}
};

class CUSTOM_EMOTION_GROUP:public vector<CUSTOM_EMOTION>
{
public:

	enum TYPE{SYS,CUSTOM};

public:

	CString		sName;

	TYPE		type;

};

typedef vector<CUSTOM_EMOTION_GROUP>	CUSTOM_EMOTION_DB;

struct EMOTION_ID
{
	int		nGroup;

	int		nItem;

	EMOTION_ID():nGroup(-1),nItem(-1){};

	bool operator < (EMOTION_ID id) const
	{
		return (nGroup < id.nGroup) || ((nGroup == id.nGroup) && nItem < id.nItem);
	}
};
typedef map<EMOTION_ID,CBitmap *>			CustomFaceMap;
typedef CustomFaceMap::iterator				CustomFaceMapIt;

struct EMOTION_GROUP_ITEM
{
	CString		sName;
	CRect		rc;
};

class EMOTION_GROUP:public vector<EMOTION_GROUP_ITEM>
{
public:

	EMOTION_GROUP()
	{
		m_nCurLeft = 0;
		m_nFirstGroup = 0;
	}

	int					m_nCurLeft;		// ��ǰ��ʾ�ķ��������ʵ��λ��
	int					m_nFirstGroup;	// ��ǰ��һ������

};

class CEmotionDlg : public CDialog
{
	DECLARE_DYNAMIC(CEmotionDlg)

public:
	CEmotionDlg(int posx, int posy, CImageList *pImgList, CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CEmotionDlg();
	virtual BOOL PreTranslateMessage(MSG* pMsg);


// �Ի�������
	enum { IDD = IDD_DIALOG_EMOTION };
public:
	HWND  m_hEmotionSelWnd;

protected:

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnNcDestroy();
	afx_msg void OnEnKillfocusEditLabel();
	afx_msg void OnEditChange();

	// ����༭
	void		OnDelEmotion();
	void		OnModifyEmotion();
	void		OnDelGroup();
	void		OnNewGroup();
	void		OnModGroupName();
	void		OnNewFace();
	void		OnMoveFace(UINT nGroupID);
	void		OnEnter();

	void		HideEdit();
	void		ShowEdit();

	LRESULT		OnGetDefID(WPARAM wp, LPARAM lp);
	int	PtInGroup(CPoint pt);
	int PtCheck(CPoint pt);
	void DrawCustomFace(EMOTION_ID id,CDC *pDC);
	void DrawEmotion(int nEmotion, UINT nStyle, CDC * pDC = NULL);
	void DrawSysPreview(int nEmotion,CDC * pDC);
	void DrawSysGifPreview(int nEmotion,CDC * pDC);
	void DrawGifEmotion(int nEmotion, CDC *pDC );
	void GetEmotionRect(int nEmotion, CRect &rcEmotion);
	
	DECLARE_MESSAGE_MAP()

private:

	// �����Զ�������ļ�
	void InitCustomFace();
	// ����Gif�����ļ�
	void InitGifEmotion();
	// ���ط���
	void InitFaceGroup(const CUSTOM_EMOTION_DB & emotions);
	// ��ס�û�ѡ���λ��
	void RememberPostion();

	

	int m_nImgHotOver;
	int m_nImgPre;

	//int m_nImgSel;
	int m_posx, m_posy;
	//int					m_nRButtonEmotion;	// �����Ҽ�ʱѡ�еı���
	CImageList *		m_pImgList;		
	CToolTipCtrl		m_Tooltip ;	
	CRect               m_rcBtnAdd, m_rcBtnPrev, m_rcBtnNext;
	CRect				m_rcBtnUpdate;	// ����վ���°�ť��Χ
	int                 m_iBtn;
	
	//int				    m_nGifFacePageCount;
	//int                 m_nEmotionCount ;
	int					m_nLoaded;		// �Ѿ����صı������
	int					m_nCurPageCount;// ��ǰ���ڷ�ҳ����
	int					m_nCurPage;		// ��ǰѡ���ҳ
	int					m_nCurGroup;	// ��ǰѡ�����
	int					m_nEditGroup;	// ��ǰ�༭����
	CustomFaceMap		m_mapCustomFace;
	map<int,CBitmap *>	m_mapGifEmotion;

	CPictureEx *		m_pWndPreview;
	
	EMOTION_GROUP		m_arrGroup;

protected:

	//------------------------------------------------------
	// ��ʼ������
	//------------------------------------------------------
	void		InitAnimation();

	//------------------------------------------------------
	// �Ƿ�Ϊ����֡
	//------------------------------------------------------
	bool		IsAnimatePic(Image* pImage,int & nFrameCount);

	//------------------------------------------------------
	// ����ͼƬԤ��
	//------------------------------------------------------
	void		LoadPreviewImage(const CString & sImage);

	//------------------------------------------------------
	// ��ʾ������ͼƬԤ��
	//------------------------------------------------------
	void		ShowPreview(bool bShow = true);

	//------------------------------------------------------
	// ����Ԥ��ͼ��
	//------------------------------------------------------
	void		DrawPreview(CDC & dc);

	//------------------------------------------------------
	// ��ʾtips
	//------------------------------------------------------
	void		ShowToolTip(const CString& strTip);

protected:

	Image *				m_pImage;
	bool				m_bAnimate;
	bool				m_bShowPreview;
	int					m_nFrameCount;
	int					m_nFramePosition;
	PropertyItem *		m_pPropertyItem;	// ͼƬ֡��ͣʱ������
	CRect				m_rcPreview;
	int					m_typePreview;		// Ԥ������: CUSTOM_EMOTION::TYPE
	CEdit*				m_pGroupLbl;		// �����༭
	bool				m_bEditLabel;		// �Ƿ����ڱ༭����
	CString				m_sLastTip;

public:

	virtual BOOL DestroyWindow();

	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
};

bool			IsWebFaceExist(const char * szFaceID);
void			GetExistWebFiles(vector<string>& sExistFiles);
EMOTION_ID		FindFaceUseUrl(const char * szUrl);
void			LoadCustomFace();
void			WriteCustomEmotionConfig(CStdioFile& file);

extern CUSTOM_EMOTION_DB  g_customEmotionDB;
