// im\SendMsgActionDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "SendMsgActionDlg.h"
#include ".\sendmsgactiondlg.h"
#include "localdata.h"

#define IDC_MYTIP	1001

// CSendMsgActionToolBar �Ի���

IMPLEMENT_DYNAMIC(CSendMsgActionToolBar, CDialog)
CSendMsgActionToolBar::CSendMsgActionToolBar(CWnd* pParent /*=NULL*/)
	: CDialog(CSendMsgActionToolBar::IDD, pParent)
{

}

CSendMsgActionToolBar::~CSendMsgActionToolBar()
{

}

void CSendMsgActionToolBar::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CMB_OBJECT, m_cmbObject);
	DDX_Control(pDX, IDC_CMB_ACTIONS, m_cmbAction);
	DDX_Control(pDX, IDC_STATIC_LBL, m_lblMsg);
	DDX_Control(pDX, IDC_BTN_EDITACTION, m_btnEditAction);
	DDX_Control(pDX, IDC_BTN_LOADACTION, m_btnLoadAction);
}


BEGIN_MESSAGE_MAP(CSendMsgActionToolBar, CDialog)
	ON_WM_ERASEBKGND()
	ON_BN_CLICKED(IDC_BTN_EDITACTION, OnBnClickedBtnEditaction)
	ON_BN_CLICKED(IDC_BTN_LOADACTION, OnBnClickedBtnLoadaction)
	ON_CBN_SELCHANGE(IDC_CMB_ACTIONS, OnCbnSelchangeCmbActions)
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()


// CSendMsgActionToolBar ��Ϣ�������

BOOL CSendMsgActionToolBar::OnEraseBkgnd(CDC* pDC)
{
	CRect rc;
	GetClientRect(rc);
	pDC->FillSolidRect(&rc,RGB(226,240,253));

	return FALSE;
}

BOOL CSendMsgActionToolBar::OnInitDialog()
{
	CDialog::OnInitDialog();
	int nPos;
	m_btnEditAction.SetBitmap(IDB_BTN_MODIFY,RGB(0,255,0));
	m_btnEditAction.SetFlatStyle(CFlatButton::FS_OFFICE);
	//m_btnLoadAction.SetBitmap(IDB_BTN_CFG,RGB(0,255,0));
	m_btnLoadAction.SetFlatStyle(CFlatButton::FS_OFFICE);
	//m_btnEditAction.SetBitmap(IDB_BTN_MODIFY,IDB_BTN_MODIFY,IDB_BTN_MODIFY);
	m_btnEditAction.SetToolTip("�༭�����ű�");
	m_btnLoadAction.SetToolTip("���ض����ű�");
	nPos=m_cmbObject.AddString("������");
	m_cmbObject.SetItemData(nPos,0);

	m_cmbObject.SetCurSel(nPos);
	// ��ʼ�������б�
	InitActions();
	
	return TRUE; 
}

void CSendMsgActionToolBar::InitActions()
{
	ACTION_ITEM test;
	int nPos;
	CString sPath= g_LocalData.GetPath();
	sPath+="data\\actions.txt";
	
	if(m_reader.LoadActionText(sPath))
	{
		m_actionList=m_reader.GetActionList();
	}
	else
	{
		m_actionList.clear();
	};
	
	// ��������б�
	m_cmbAction.ResetContent();

	nPos=m_cmbAction.AddString("��");
	m_cmbAction.SetItemData(nPos,-1);
	m_cmbAction.SetCurSel(nPos);
	for(int i=0;i<m_actionList.size();i++)
	{
		nPos=m_cmbAction.InsertString(-1,m_actionList[i].sName);
		m_cmbAction.SetItemData(nPos,i);
	}

}


BOOL CSendMsgActionToolBar::GetActionData(ACTION_DATA& ad)
{
	ad.Clear();
	int nPos;

	nPos = m_cmbObject.GetCurSel();
	if(nPos != CB_ERR)
	{
		ad.id = m_cmbObject.GetItemData(nPos);
	}
	else
	{
		ad.id = 0;
	}

	nPos = m_cmbAction.GetCurSel();
	if(nPos == CB_ERR)
		return FALSE;
	int n=(int)m_cmbAction.GetItemData(nPos);
	if(n==-1)
	{
		return FALSE;
	}
	ASSERT(n<m_actionList.size());
	
	m_cmbObject.GetWindowText(ad.sObject);
	ad.sAction0 = m_actionList[n].sAction0;
	ad.sAction1 = m_actionList[n].sAction1;
	ad.sAction2 = m_actionList[n].sAction2;
	ad.sMsg = m_actionList[n].sMsg;
	

	return TRUE;
}

CString CSendMsgActionToolBar::GetCurSelObject()
{
	CString sRt;
	int nPos;
	nPos = m_cmbObject.GetCurSel();
	if(nPos<0)
		return sRt;
	// ѡ���������ˣ��򷵻ؿ�
	if(nPos==0)
		return "";
	m_cmbObject.GetWindowText(sRt);
	return sRt;

}
void CSendMsgActionToolBar::OnBnClickedBtnEditaction()
{
	CString sFile=g_LocalData.GetPath();
	sFile+="data\\actions.txt";
	if ( _access(sFile,0)==-1 )
	{
		//MessageBox("û���ҵ������ű������ļ�:"+sFile,"����");
		CMessageBox::Prompt("û���ҵ������ű������ļ�:"+sFile,"����");
		return;
	}
	ShellExecute(NULL,"open",sFile,"","",SW_SHOW);
}

void CSendMsgActionToolBar::OnBnClickedBtnLoadaction()
{
	InitActions();
}

void CSendMsgActionToolBar::AddObject(const CString& sObject,int id)
{
	CString sName;

	int n=m_cmbObject.GetCount();
	// ���жϸ���������Ƿ������
	for(int i=0;i<n;i++)
	{
		m_cmbObject.GetLBText(i,sName);
		if(sObject==sName)
		{
			m_cmbObject.SetCurSel(i);
			return;
		}
	}
	int nPos=m_cmbObject.InsertString(-1,sObject);
	if(nPos == CB_ERR)
		return;
	m_cmbObject.SetCurSel(nPos);
	m_cmbObject.SetItemData(nPos,id);
}


BOOL CSendMsgActionToolBar::PreTranslateMessage(MSG* pMsg)
{
	if  (  pMsg->message  ==  WM_KEYDOWN  )  
	{  
		switch(  pMsg->wParam  )  
		{  
		case 0x0D:
		case VK_ESCAPE:
			return TRUE;
		default:
			;
		}  
	}
	if( pMsg->message== WM_LBUTTONDOWN ||
		pMsg->message== WM_LBUTTONUP ||
		pMsg->message== WM_MOUSEMOVE
		)
	{
		if(m_toolTip.m_hWnd!=NULL)
			m_toolTip.RelayEvent(pMsg);
	}
	return CDialog::PreTranslateMessage(pMsg);
}

void CSendMsgActionToolBar::OnCbnSelchangeCmbActions()
{
	CWnd* p=GetParent();
	int nSel=m_cmbAction.GetCurSel();
	if(nSel<0)
		return;
	int nData=(int)m_cmbAction.GetItemData(nSel);
	// û�ж�����Ϣ
	if(nData==-1)
	{
		m_cmbObject.SetCurSel(0);
	}
	static ACTION_DATA ad;
	GetActionData(ad);
	if(p)
		p->SendMessage(WM_INSERTMSG,0,(LPARAM)(LPCTSTR)ad.sMsg);
}

void CSendMsgActionToolBar::OnMouseMove(UINT nFlags, CPoint point)
{
	
	ShowToolTip("��ʾ������ұ߳�Ա����Ӷ�������");
	CDialog::OnMouseMove(nFlags, point);
}

void CSendMsgActionToolBar::ShowToolTip(const CString& strTip)
{
	if(m_sLastTip==strTip)
		return;
	m_sLastTip=strTip;
	if (m_toolTip.m_hWnd == NULL)
	{
		m_toolTip.Create(this,TTS_ALWAYSTIP);
		// ���ó�����ʾ��Ҫ��ʱ�䳤��
		m_toolTip.SetDelayTime(TTDT_INITIAL,100);
		// ������ʾ����ʱ��
		m_toolTip.SetDelayTime(TTDT_AUTOPOP,10000);
	}
	if (m_toolTip.GetToolCount() == 0)
	{
		CRect rc;
		GetClientRect(&rc);
		m_toolTip.AddTool(this, strTip, rc, IDC_MYTIP);
	}
	m_toolTip.UpdateTipText(strTip ,this,IDC_MYTIP);

}

void CSendMsgActionToolBar::ResetAction()
{
	int nIdx = m_cmbAction.GetCount();
	if(nIdx == 0)
		return;
	int nData = (int)m_cmbAction.GetItemData(0);
	if(nData != -1)
		return;
	m_cmbAction.SetCurSel(0);
}

void CSendMsgActionToolBar::ResetObject()
{
	int nIdx = m_cmbObject.GetCount();
	if(nIdx == 0)
		return;
	m_cmbObject.SetCurSel(0);
	int nData = (int)m_cmbObject.GetItemData(0);
	if(nData != -1)
		return;
	
}