// SysCommonAudio.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "SysCommonAudio.h"
#include "afxdialogex.h"
#include "mmsystem.h"
#include <windows.h>
#include <stdio.h>
#include <setupapi.h>
#include <devguid.h>
#include <regstr.h>
#include <shlwapi.h>
#include "Im/LocalData.h"
#pragma comment(lib, "setupapi.lib")
#define BUFSIZE    1024
#define SM_SERVERR    289


using namespace DuiLib;


// CSysCommonAudio �Ի���
// ��ȡ��Ƶ�豸��
int GetImageDevice(vector<string> &vecImgDevName)
{
    HDEVINFO hDevInfo;
    SP_DEVINFO_DATA DeviceInfoData;
    DWORD i;
    hDevInfo = SetupDiGetClassDevs((LPGUID) &/*GUID_DEVCLASS_MEDIA*/GUID_DEVCLASS_IMAGE, 0, 0,DIGCF_PRESENT);
    /*
    GUID_DEVCLASS_FDC���̿�����
    GUID_DEVCLASS_DISPLAY��ʾ��
    GUID_DEVCLASS_CDROM����
    GUID_DEVCLASS_KEYBOARD����
    GUID_DEVCLASS_COMPUTER�����
    GUID_DEVCLASS_SYSTEMϵͳ
    GUID_DEVCLASS_DISKDRIVE����������
    GUID_DEVCLASS_MEDIA��������Ƶ����Ϸ������
    GUID_DEVCLASS_MODEMMODEM
    GUID_DEVCLASS_MOUSE��������ָ���豸
    GUID_DEVCLASS_NET�����豸��
    GUID_DEVCLASS_USBͨ�ô������߿�����
    GUID_DEVCLASS_FLOPPYDISK����������
    GUID_DEVCLASS_UNKNOWNδ֪�豸
    GUID_DEVCLASS_SCSIADAPTERSCSI �� RAID ������
    GUID_DEVCLASS_HDCIDE ATA/ATAPI ������
    GUID_DEVCLASS_PORTS�˿ڣ�COM �� LPT��
    GUID_DEVCLASS_MONITOR������
    */
    if (hDevInfo == INVALID_HANDLE_VALUE)
    {
        DWORD dwError = GetLastError();
        return 1;
    }
    // Enumerate through all devices in Set.
    DeviceInfoData.cbSize = sizeof(SP_DEVINFO_DATA);
    for (i = 0; SetupDiEnumDeviceInfo(hDevInfo, i,  &DeviceInfoData);  i++)
    {
        DWORD DataT;
		LPTSTR buffer = NULL;
		DWORD buffersize = 0;
        while (!SetupDiGetDeviceRegistryProperty(
            hDevInfo,
            &DeviceInfoData,
            SPDRP_DEVICEDESC,
            &DataT,
            (PBYTE)buffer,
            buffersize,
            &buffersize))
        {
			if (GetLastError() == ERROR_INSUFFICIENT_BUFFER)
			{
				// Change the buffer size.
				if (buffer)
				{
					LocalFree(buffer);
					buffer = NULL;
				}
				buffer = (char*)LocalAlloc(LPTR,buffersize);
			}
			else
			{
				break;
			}
        }

		if (buffer != NULL)
		{
			vecImgDevName.push_back(buffer);
			LocalFree(buffer);
			buffer = NULL;
		}
    }
    if ( GetLastError() != NO_ERROR &&   GetLastError() != ERROR_NO_MORE_ITEMS )
    {
        return 1;
    }
    // Cleanup
    SetupDiDestroyDeviceInfoList(hDevInfo);
    return 0;
}

void CSysCommonAudio::InitWindow()
{
	WindowImplBase::InitWindow();

	m_comboVoiceInput = static_cast<CComboUI*>(m_PaintManager.FindControl("ComboVoiceInput")) ;     ////��������
	m_comboVoiceOutput =  static_cast<CComboUI*>(m_PaintManager.FindControl("ComboVoiceOutput")) ;  ////�������
	m_pSliderMicro  = static_cast<CSliderUI*>(m_PaintManager.FindControl("AudioOutputSlider")) ;      ////��Ƶ���
	m_pSliderVoice  = static_cast<CSliderUI*>(m_PaintManager.FindControl("voiceOutputSlider")) ;      ////�������
    m_comboCameraSel = static_cast<CComboUI*>(m_PaintManager.FindControl("CameraOutput")) ;          ////����豸

	//////////����Ĭ������
	WAVEINCAPS incap;  
	int innum = waveInGetNumDevs();    //��ȡ������Ƶ�����豸��Ŀ  
	for(int i=0;i<innum;i++)  
	{  
		MMRESULT rs = waveInGetDevCaps(i,&incap,sizeof(incap)); //��ȡ�豸���е�ǰ�����豸��Ϣ  

		DuiLib::CListLabelElementUI* pListElement = new DuiLib::CListLabelElementUI;
		pListElement->SetAttribute("text", incap.szPname);
		pListElement->SetTag(i);
		m_comboVoiceInput->Add(pListElement);
	}  
	m_comboVoiceInput->SelectItem(0);
	
	WAVEOUTCAPS outcap;  
	int outnum = waveOutGetNumDevs();    //��ȡ������Ƶ�����豸��Ŀ  
	for(int i=0;i<outnum;i++)  
	{  
		MMRESULT rs = waveOutGetDevCaps(i,&outcap,sizeof(outcap)); //��ȡ�豸���е�ǰ�����豸��Ϣ  
		DuiLib::CListLabelElementUI* pListElement = new DuiLib::CListLabelElementUI;
		pListElement->SetAttribute("text", outcap.szPname);
		pListElement->SetTag(i);
		m_comboVoiceOutput->Add(pListElement);
	}  
	m_comboVoiceOutput->SelectItem(0);
	
	vector<string> vecImgDev;
	GetImageDevice(vecImgDev);
	for(int i = 0; i < vecImgDev.size(); ++i)
	{
		DuiLib::CListLabelElementUI* pListElement = new DuiLib::CListLabelElementUI;
		pListElement->SetAttribute("text", vecImgDev[i].c_str());
		pListElement->SetTag(i);
		m_comboCameraSel->Add(pListElement);
	}
	m_comboCameraSel->SelectItem(0);
	
	//// ���ݱ����ļ�����ѡ��
	MEDIA_DEV_SETTING* setting = g_LocalData.GetMediaDevSetting();
	if(setting->nIdxVoiceInSel >= 0)
	{
		m_comboVoiceInput->SelectItem(setting->nIdxVoiceInSel);
	}
	if(setting->nIdxVoiceOutSel >= 0)
	{
		m_comboVoiceOutput->SelectItem(setting->nIdxVoiceOutSel);
	}
	if(setting->nIdxVedioSel >= 0)
	{
		m_comboCameraSel->SelectItem(setting->nIdxVedioSel);
	}
	if(setting->nVoiceInSize >= 0)
	{
		m_pSliderMicro->SetValue(setting->nVoiceInSize);
	}
	if(setting->nVoiceOutSize >= 0)
	{
		m_pSliderVoice->SetValue(setting->nVoiceOutSize);
	}
}


// ��Ϣ
void CSysCommonAudio::Notify(DuiLib::TNotifyUI& msg)
{
	DuiLib::WindowImplBase::Notify(msg);
	//if (_tcsicmp(msg.sType, _T("click")) == 0)
	//{
	//	
	//}
}


// ��������
void CSysCommonAudio::Save()
{
	// ���ݱ����ļ�����ѡ��
	MEDIA_DEV_SETTING* setting = g_LocalData.GetMediaDevSetting();
	if(m_comboVoiceInput->GetCurSel() >= 0)
	{
		setting->nIdxVoiceInSel = m_comboVoiceInput->GetCurSel();
	}
	if(m_comboVoiceOutput->GetCurSel() >= 0)
	{
		setting->nIdxVoiceOutSel = m_comboVoiceOutput->GetCurSel();
	}
	if(m_comboCameraSel->GetCurSel() >= 0)
	{
		setting->nIdxVedioSel = m_comboCameraSel->GetCurSel();
	}
	setting->nVoiceInSize = m_pSliderMicro->GetValue();
	setting->nVoiceOutSize = m_pSliderVoice->GetValue();
	g_LocalData.SetMediaDevSetting(setting);
}

//
//IMPLEMENT_DYNAMIC(CSysCommonAudio, CDialog)
//
//CSysCommonAudio::CSysCommonAudio(CWnd* pParent /*=NULL*/)
//	: CDialog(CSysCommonAudio::IDD, pParent)
//{
//	m_pSliderMicro = new CUISlider();
//	m_pSliderVoice = new CUISlider();
//}
//
//CSysCommonAudio::~CSysCommonAudio()
//{
//	if(::IsWindow(GetSafeHwnd()))
//	{
//		m_pSliderMicro->SendMessage(WM_NCDESTROY);
//		m_pSliderVoice->SendMessage(WM_NCDESTROY);
//		DestroyWindow();
//	}
//
//	if(m_pSliderMicro != NULL)
//	{
//		delete m_pSliderMicro;
//		m_pSliderMicro = NULL;
//	}
//	if(m_pSliderVoice != NULL)
//	{
//		delete m_pSliderVoice;
//		m_pSliderVoice = NULL;
//	}
//}
//
//void CSysCommonAudio::DoDataExchange(CDataExchange* pDX)
//{
//	CDialog::DoDataExchange(pDX);
//	DDX_Control(pDX, IDC_BTN_MICRO, m_btnMicro);
//	DDX_Control(pDX, IDC_BTN_VOICE, m_btnVoice);
//	DDX_Control(pDX, IDC_COMBO_VOICE_INPUT, m_comboVoiceInput);
//	DDX_Control(pDX, IDC_COMBO_VOICE_OUTPUT, m_comboVoiceOutput);
//	DDX_Control(pDX, IDC_COMBO_CAMERA_SEL, m_comboCameraSel);
//}
//
//
//BEGIN_MESSAGE_MAP(CSysCommonAudio, CDialog)
//	ON_WM_CTLCOLOR()
//END_MESSAGE_MAP()
//
//
//// CSysCommonAudio ��Ϣ�������
//BOOL CSysCommonAudio::OnInitDialog()
//{
//	CDialog::OnInitDialog();
//
//	// ���������ӿؼ������ÿؼ�����
//	HWND hWndChild =::GetWindow(m_hWnd,GW_CHILD); 
//	while(hWndChild)  
//	{  
//		::SendMessage(hWndChild, WM_SETFONT, (WPARAM)g_font12.GetSafeHandle(), TRUE);
//		if(GetDlgItem(IDC_STATIC_VOICESETTING)->GetSafeHwnd() == hWndChild 
//			|| GetDlgItem(IDC_STATIC_VIDEO)->GetSafeHwnd() == hWndChild )
//		{
//			::SendMessage(hWndChild, WM_SETFONT, (WPARAM)g_font12B.GetSafeHandle(), TRUE);
//		}
//		hWndChild=::GetWindow(hWndChild,GW_HWNDNEXT); 
//	}
//
//	m_comboVoiceInput.SetBorderColor(0xc8c8c8);
//	m_comboVoiceOutput.SetBorderColor(0xc8c8c8);
//	m_comboCameraSel.SetBorderColor(0xc8c8c8);
//
//	m_btnMicro.SetPng(IDB_PNG_MICRO, IDB_PNG_MICRO, IDB_PNG_MICRO, IDB_PNG_MICRO);
//	m_btnVoice.SetPng(IDB_PNG_VOICE, IDB_PNG_VOICE, IDB_PNG_VOICE, IDB_PNG_VOICE);
//
//	CRect rcBtn;
//	m_btnMicro.GetWindowRect(&rcBtn);
//	ScreenToClient(&rcBtn);
//	m_pSliderMicro->Create(GetSafeHwnd(), _T("DUISysSettingAudioMicro"), UI_WNDSTYLE_CHILD, 0, rcBtn.right+4, rcBtn.top+(rcBtn.Height()-26)/2, 100, 26); 
//	m_pSliderMicro->ShowWindow(TRUE); 
//
//	m_btnVoice.GetWindowRect(&rcBtn);
//	ScreenToClient(&rcBtn);
//	m_pSliderVoice->Create(GetSafeHwnd(), _T("DUISysSettingAudioVoice"), UI_WNDSTYLE_CHILD, 0, rcBtn.right+4, rcBtn.top+(rcBtn.Height()-26)/2, 100, 26); 
//	m_pSliderVoice->ShowWindow(TRUE); 
//
//	WAVEINCAPS incap;  
//	int innum = waveInGetNumDevs();    //��ȡ������Ƶ�����豸��Ŀ  
//	for(int i=0;i<innum;i++)  
//	{  
//		MMRESULT rs = waveInGetDevCaps(i,&incap,sizeof(incap)); //��ȡ�豸���е�ǰ�����豸��Ϣ  
//		m_comboVoiceInput.AddString(incap.szPname);
//	}  
//	m_comboVoiceInput.SetCurSel(0);
//
//	WAVEOUTCAPS outcap;  
//	int outnum = waveOutGetNumDevs();    //��ȡ������Ƶ�����豸��Ŀ  
//	for(int i=0;i<outnum;i++)  
//	{  
//		MMRESULT rs = waveOutGetDevCaps(i,&outcap,sizeof(outcap)); //��ȡ�豸���е�ǰ�����豸��Ϣ  
//		m_comboVoiceOutput.AddString(outcap.szPname);
//	}  
//	m_comboVoiceOutput.SetCurSel(0);
//
//	vector<string> vecImgDev;
//	GetImageDevice(vecImgDev);
//	for(int i = 0; i < vecImgDev.size(); ++i)
//	{
//		m_comboCameraSel.AddString(vecImgDev[i].c_str());
//	}
//	m_comboCameraSel.SetCurSel(0);
//
//	// ���ݱ����ļ�����ѡ��
//	MEDIA_DEV_SETTING* setting = g_LocalData.GetMediaDevSetting();
//	if(setting->nIdxVoiceInSel >= 0)
//	{
//		m_comboVoiceInput.SetCurSel(setting->nIdxVoiceInSel);
//	}
//	if(setting->nIdxVoiceOutSel >= 0)
//	{
//		m_comboVoiceOutput.SetCurSel(setting->nIdxVoiceOutSel);
//	}
//	if(setting->nIdxVedioSel >= 0)
//	{
//		m_comboCameraSel.SetCurSel(setting->nIdxVedioSel);
//	}
//	if(setting->nVoiceInSize >= 0)
//	{
//		m_pSliderMicro->SetThumbPos(setting->nVoiceInSize);
//	}
//	if(setting->nVoiceOutSize >= 0)
//	{
//		m_pSliderVoice->SetThumbPos(setting->nVoiceOutSize);
//	}
//
//	return TRUE;
//}
//
//HBRUSH CSysCommonAudio::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
//{
//	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
//
//	if ( nCtlColor==CTLCOLOR_STATIC )
//	{
//		UINT nId = pWnd->GetDlgCtrlID();
//
//		if ( nId==IDC_EDIT_USERNAME || nId==IDC_EDIT_NICKNAME || nId==IDC_EDIT_EMAIL )
//		{
//
//		}
//		else
//		{
//			pDC->SetBkMode(TRANSPARENT);
//			hbr = g_hWhiteBrush;
//		}
//
//		if ( nId==IDC_STATIC_GROUP )
//		{
//			pDC->SetTextColor(RGB(255,0,0));//��255,0��0�� //(74,123,211)
//		}
//		else
//		{
//			pDC->SetTextColor(0x2e2e2e);
//		}
//	}
//
//	if ( nCtlColor==CTLCOLOR_DLG  )
//	{
//		hbr = g_hWhiteBrush;
//	}
//
//	return hbr;
//}
//
//void CSysCommonAudio::Save()
//{
//	// ���ݱ����ļ�����ѡ��
//	MEDIA_DEV_SETTING* setting = g_LocalData.GetMediaDevSetting();
//	if(m_comboVoiceInput.GetCurSel() >= 0)
//	{
//		setting->nIdxVoiceInSel = m_comboVoiceInput.GetCurSel();
//	}
//	if(m_comboVoiceOutput.GetCurSel() >= 0)
//	{
//		setting->nIdxVoiceOutSel = m_comboVoiceOutput.GetCurSel();
//	}
//	if(m_comboCameraSel.GetCurSel() >= 0)
//	{
//		setting->nIdxVedioSel = m_comboCameraSel.GetCurSel();
//	}
//	setting->nVoiceInSize = m_pSliderMicro->GetThumbPos();
//	setting->nVoiceOutSize = m_pSliderVoice->GetThumbPos();
//	g_LocalData.SetMediaDevSetting(setting);
//}
//
//DuiLib::UILIB_RESOURCETYPE CUISlider::GetResourceType() const
//{
//	return UILIB_ZIPRESOURCE;
//}
//
//LPCTSTR CUISlider::GetResourceID() const
//{
//	return MAKEINTRESOURCE(IDR_ZIPRES1);
//}
//
//void CUISlider::SetThumbPos(int nThumbPos)
//{
//	CSliderUI* pSlider = static_cast<CSliderUI*>(m_PaintManager.FindControl("Slider"));
//	pSlider->SetValue(nThumbPos);
//}
//
//int CUISlider::GetThumbPos()
//{
//	CSliderUI* pSlider = static_cast<CSliderUI*>(m_PaintManager.FindControl("Slider"));
//	return pSlider->GetValue();
//}