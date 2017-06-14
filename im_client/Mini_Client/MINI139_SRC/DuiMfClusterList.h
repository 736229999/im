#pragma once

#include "..\..\..\..\DuiLib\UIlib.h"
#include "im\impublic.h"
#include <afxmt.h>
#include <vector>
using namespace std;


#include "resgdiobj.h"

/************************************************************************ 
* 
* �ļ�����xtreamgrouplist.h 
* 
* �ļ�������xtream �����б�ؼ�,
* 
* �����ˣ� ѦС��,2005��8��3��
* 
* �汾�ţ�1.0 
* 
* �޸ļ�¼�� 
* 
************************************************************************/ 

#define NM_CL_LBUTTONDOWN         0x01
#define NM_CL_LBUTTONUP           0x02

#define NM_CL_RBUTTONDOWN         0x03
#define NM_CL_RBUTTONUP           0x04

#define NM_CL_HOME_CLICK			0x07
#define NM_CL_REFUSE_CLICK			0x08
#define NM_CL_PARTY_CLICK			0x09
#define NM_CL_SETTING_CLICK         0x0A
#define NM_CL_ALBUM_CLICK           0x0B
#define NM_CL_ARTICLE_CLICK			0x0C
#define NM_CL_WRITE_ARTICLE_CLICK	0x0D
#define NM_CL_UPLOAD_PIC_CLICK		0x0E
#define NM_CL_DISCUSS_CLICK         0x0F  
#define NM_CL_TITLE_CLICK		    0x10

enum CLS_STATE
{
	CLS_EXPAND,
	CLS_CLOSE,
};

typedef struct _CL_GROUP_POS
{
	int nRoot;
	int nGroup;

	_CL_GROUP_POS()
	{
		nRoot  = -1;
		nGroup = -1;
	}

	bool operator ==(_CL_GROUP_POS &g)
	{
		if ( nRoot==g.nRoot && nGroup==g.nGroup )
			return true;
		else
			return false;
	}

	bool IsNull()
	{
		if ( nRoot==-1 || nGroup==-1 )
		{
			return true;
		}
		else
		{
			return false;
		}
	}

} CL_GROUP_POS;

//typedef struct _CL_ITEM_POS
//{
//	int nRoot;
//	int nGroup;
//	int nItem;
//
//	_CL_ITEM_POS() 
//	{
//		nRoot    = -1;
//		nGroup   = -1;
//		nItem    = -1;
//	}
//
//	operator CL_GROUP_POS() const
//	{
//		CL_GROUP_POS pos;
//		pos.nRoot  = nRoot;
//		pos.nGroup = nGroup;
//		return pos;
//	}
//
//	bool operator ==(_CL_ITEM_POS &i)
//	{
//		if ( nRoot==i.nRoot && nGroup==i.nGroup && nItem==i.nItem )
//			return true;
//		else
//			return false;
//	}
//
//	bool IsNull()
//	{
//		if ( nRoot==-1 || nGroup==-1 || nItem==-1 )
//		{
//			return true;
//		}
//		else
//		{
//			return false;
//		}
//	}
//
//} CL_ITEM_POS;

typedef struct _CL_GROUP_INFO
{		
	CLS_STATE		state		;

	char            szTitle[50]	;

	COLORREF		clrText		;

	DWORD			dwData		;

	CRect			rcBar		;// ��ĳ�����Χ��С
	CRect			rcFace		;// ���
	CRect			rcText		;// �ı���Χ

	CRect			rcBtnHome	;// ������ҳ��ť��Χ
	CRect			rcBtnRefuse	;// ��ֹ��ť��Χ
	//CRect			rcBtnAlbum;
	//CRect			rcBtnUploadPic;
	//CRect			rcBtnArticle;
	//CRect			rcBtnWriteArticle;
	//CRect			rcBtnSetting;
	//CRect			rcBtnParty;//������

	CRect			rcBtnExpand ;//+/- button

	CBitmap		*   pBitmap40		; //���
	CBitmap     *   pBitmap20       ; //���

	bool            bLoadingMembers	;
	bool			bFlash			;//�Ƿ���˸
	int				iFlash			;//no use other

	_CL_GROUP_INFO()
	{
		dwData		    = 0;
		state	        = CLS_CLOSE;
		bLoadingMembers = false;
		bFlash          = false;
		iFlash          = 0;
		pBitmap40		= NULL;
		pBitmap20       = NULL;
		clrText         = RGB(0,0,0);
	}

	~_CL_GROUP_INFO()
	{
	}

} CL_GROUP_INFO;


typedef struct _CL_ROOT_INFO
{
	CLS_STATE  state;
	CString    strTitle;

	CRect      rcBar   ;
	CRect      rcButton;
	CRect      rcText;

	vector<CL_GROUP_INFO *> vecGroup;

	_CL_ROOT_INFO()
	{
		state=CLS_EXPAND;
	}

} CL_ROOT_INFO;

class CDuiClusterList : public DuiLib::CTreeViewUI, public DuiLib::IDialogBuilderCallback
{
public:
	CDuiClusterList(HWND hParent, DuiLib::CPaintManagerUI& paint_manager);	
	virtual ~CDuiClusterList();

	// Implementation
public:
	int AddRoot(CString strRoot);

	CL_GROUP_POS FindCluster(int nClusterId);

	void DelGroup(CL_GROUP_POS posGroup);
	int  InsertGroup(CL_GROUP_POS posGroup,CString strTitle,DWORD dwData=0);
	CL_GROUP_POS AddGroup(int nRoot, CLUSTER_INFO *pCluster);
	void SetGroupTextColor(CL_GROUP_POS posGroup, COLORREF clrText);
	void UpdateGroupImage(CL_GROUP_POS posGroup);

	void  SwapGroup(CL_GROUP_POS pos1, CL_GROUP_POS pos2);

	DWORD GetGroupData(CL_GROUP_POS posGroup);
	CL_GROUP_INFO * GetGroupInfo(CL_GROUP_POS posGroup);
	int   GetRootCount();
	int   GetGroupCount();
	int   GetGroupCount(int nRoot);
	CL_GROUP_POS GetCurSel();
	void SetCurSel(CL_GROUP_POS posGroup);
	void ClearCurSel();//��յ�ǰѡ�е�λ��

	DuiLib::CControlUI* CreateControl(LPCTSTR pstrClass);
	void UpdateGroup(CLUSTER_INFO *pCluster);

	// ���ڵ��Ƿ�չ��
	BOOL IsExpand(int nRoot);
	// չ���ڵ�
	void ExpandRoot(int nRoot, BOOL bExpand);

	/*void AddLoadFlag(CL_GROUP_POS posGroup);
	void RemoveLoadFlag(CL_GROUP_POS posGroup);

	void AddFlashFlag(CL_GROUP_POS posGroup);
	void RemoveFlashFlag(CL_GROUP_POS posGroup);*/

	void RemoveAll()
	{
		CTreeViewUI::RemoveAll();
		DelMem();
	}

protected:
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	//afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	//afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	//afx_msg void OnTimer(UINT nIDEvent);

	// ��Ӹ��ڵ�
	BOOL AddRoot_Interface(CString strTitle, int nRoot);
	// ������ɾ����Ӧ��
	void DelCluster_Interface(int nData);
	// ���Һ���node
	DuiLib::CTreeNodeUI* FindCluster_Interface(int nData);
	// ���������ڵ�
	BOOL AddCluster_Interface(int nRoot, int nData);
	// ���½�����ڵ�
	BOOL UpdateCluster_Interface(DuiLib::CTreeNodeUI *pClusterNode, int nData);
	// ���Ҹ��ڵ�
	DuiLib::CTreeNodeUI* FindRoot_Interface(int nRoot);
	// ���ڵ��Ƿ�Ϊ���ڵ�
	BOOL IsRootNote(DuiLib::CTreeNodeUI *pNode);
	bool SelectItem(int iIndex, bool bTakeFocus = false);
	void UpdateRoot();
	void DelMem();

private:
	CL_GROUP_POS m_posGroupSelected;

	vector<CL_ROOT_INFO *>  m_vecRoot;    ;

	DuiLib::CPaintManagerUI& paint_manager_;
	HWND m_hParent;
};

