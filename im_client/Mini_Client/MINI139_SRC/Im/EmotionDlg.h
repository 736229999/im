#pragma once

#include "..\resource.h"
#include "buttonext.h"
#include "p2pSendmsgDlg.h"
#include "DuiP2PSendMsgDlg.h"
#include "RichEditCtrlEx.h"

#define IDC_LINK_ADD		0	// 自定义表情按钮
#define IDC_LINK_PRE		1	// 上一页按钮
#define IDC_LINK_NEXT		2	// 下一页按钮
#define IDC_LINK_UPDATE		3	// 从网站更新按钮

class CUSTOM_EMOTION
{
public:
	enum EMOTION_PIC_TYPE
	{
		CUSTOM = 0,	// 自定义表情
		WEB,		// web表情
		SYS,		// 内置表情
		SYS_GIF		// 自带表情
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

	int					m_nCurLeft;		// 当前显示的分组项左边实际位置
	int					m_nFirstGroup;	// 当前第一个分组

};

class CEmotionDlg : public CDialog
{
	DECLARE_DYNAMIC(CEmotionDlg)

public:
	CEmotionDlg(int posx, int posy, CImageList *pImgList, CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CEmotionDlg();
	virtual BOOL PreTranslateMessage(MSG* pMsg);


// 对话框数据
	enum { IDD = IDD_DIALOG_EMOTION };
public:
	HWND  m_hEmotionSelWnd;

protected:

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnNcDestroy();
	afx_msg void OnEnKillfocusEditLabel();
	afx_msg void OnEditChange();

	// 表情编辑
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

	// 加载自定义表情文件
	void InitCustomFace();
	// 加载Gif表情文件
	void InitGifEmotion();
	// 加载分组
	void InitFaceGroup(const CUSTOM_EMOTION_DB & emotions);
	// 记住用户选择的位置
	void RememberPostion();

	

	int m_nImgHotOver;
	int m_nImgPre;

	//int m_nImgSel;
	int m_posx, m_posy;
	//int					m_nRButtonEmotion;	// 按下右键时选中的表情
	CImageList *		m_pImgList;		
	CToolTipCtrl		m_Tooltip ;	
	CRect               m_rcBtnAdd, m_rcBtnPrev, m_rcBtnNext;
	CRect				m_rcBtnUpdate;	// 从网站更新按钮范围
	int                 m_iBtn;
	
	//int				    m_nGifFacePageCount;
	//int                 m_nEmotionCount ;
	int					m_nLoaded;		// 已经加载的表情个数
	int					m_nCurPageCount;// 当前组内分页个数
	int					m_nCurPage;		// 当前选择的页
	int					m_nCurGroup;	// 当前选择的组
	int					m_nEditGroup;	// 当前编辑的组
	CustomFaceMap		m_mapCustomFace;
	map<int,CBitmap *>	m_mapGifEmotion;

	CPictureEx *		m_pWndPreview;
	
	EMOTION_GROUP		m_arrGroup;

protected:

	//------------------------------------------------------
	// 初始化动画
	//------------------------------------------------------
	void		InitAnimation();

	//------------------------------------------------------
	// 是否为动画帧
	//------------------------------------------------------
	bool		IsAnimatePic(Image* pImage,int & nFrameCount);

	//------------------------------------------------------
	// 加载图片预览
	//------------------------------------------------------
	void		LoadPreviewImage(const CString & sImage);

	//------------------------------------------------------
	// 显示或隐藏图片预览
	//------------------------------------------------------
	void		ShowPreview(bool bShow = true);

	//------------------------------------------------------
	// 绘制预览图像
	//------------------------------------------------------
	void		DrawPreview(CDC & dc);

	//------------------------------------------------------
	// 显示tips
	//------------------------------------------------------
	void		ShowToolTip(const CString& strTip);

protected:

	Image *				m_pImage;
	bool				m_bAnimate;
	bool				m_bShowPreview;
	int					m_nFrameCount;
	int					m_nFramePosition;
	PropertyItem *		m_pPropertyItem;	// 图片帧暂停时间属性
	CRect				m_rcPreview;
	int					m_typePreview;		// 预览类型: CUSTOM_EMOTION::TYPE
	CEdit*				m_pGroupLbl;		// 组名编辑
	bool				m_bEditLabel;		// 是否正在编辑组名
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
