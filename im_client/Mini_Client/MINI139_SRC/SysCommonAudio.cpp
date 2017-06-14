// SysCommonAudio.cpp : 实现文件
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


// CSysCommonAudio 对话框
// 获取视频设备名
int GetImageDevice(vector<string> &vecImgDevName)
{
    HDEVINFO hDevInfo;
    SP_DEVINFO_DATA DeviceInfoData;
    DWORD i;
    hDevInfo = SetupDiGetClassDevs((LPGUID) &/*GUID_DEVCLASS_MEDIA*/GUID_DEVCLASS_IMAGE, 0, 0,DIGCF_PRESENT);
    /*
    GUID_DEVCLASS_FDC软盘控制器
    GUID_DEVCLASS_DISPLAY显示卡
    GUID_DEVCLASS_CDROM光驱
    GUID_DEVCLASS_KEYBOARD键盘
    GUID_DEVCLASS_COMPUTER计算机
    GUID_DEVCLASS_SYSTEM系统
    GUID_DEVCLASS_DISKDRIVE磁盘驱动器
    GUID_DEVCLASS_MEDIA声音、视频和游戏控制器
    GUID_DEVCLASS_MODEMMODEM
    GUID_DEVCLASS_MOUSE鼠标和其他指针设备
    GUID_DEVCLASS_NET网络设备器
    GUID_DEVCLASS_USB通用串行总线控制器
    GUID_DEVCLASS_FLOPPYDISK软盘驱动器
    GUID_DEVCLASS_UNKNOWN未知设备
    GUID_DEVCLASS_SCSIADAPTERSCSI 和 RAID 控制器
    GUID_DEVCLASS_HDCIDE ATA/ATAPI 控制器
    GUID_DEVCLASS_PORTS端口（COM 和 LPT）
    GUID_DEVCLASS_MONITOR监视器
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

	m_comboVoiceInput = static_cast<CComboUI*>(m_PaintManager.FindControl("ComboVoiceInput")) ;     ////输入语音
	m_comboVoiceOutput =  static_cast<CComboUI*>(m_PaintManager.FindControl("ComboVoiceOutput")) ;  ////输出语音
	m_pSliderMicro  = static_cast<CSliderUI*>(m_PaintManager.FindControl("AudioOutputSlider")) ;      ////视频输出
	m_pSliderVoice  = static_cast<CSliderUI*>(m_PaintManager.FindControl("voiceOutputSlider")) ;      ////语音输出
    m_comboCameraSel = static_cast<CComboUI*>(m_PaintManager.FindControl("CameraOutput")) ;          ////输出设备

	//////////加载默认设置
	WAVEINCAPS incap;  
	int innum = waveInGetNumDevs();    //获取波形音频输入设备数目  
	for(int i=0;i<innum;i++)  
	{  
		MMRESULT rs = waveInGetDevCaps(i,&incap,sizeof(incap)); //获取设备表中当前输入设备信息  

		DuiLib::CListLabelElementUI* pListElement = new DuiLib::CListLabelElementUI;
		pListElement->SetAttribute("text", incap.szPname);
		pListElement->SetTag(i);
		m_comboVoiceInput->Add(pListElement);
	}  
	m_comboVoiceInput->SelectItem(0);
	
	WAVEOUTCAPS outcap;  
	int outnum = waveOutGetNumDevs();    //获取波形音频输入设备数目  
	for(int i=0;i<outnum;i++)  
	{  
		MMRESULT rs = waveOutGetDevCaps(i,&outcap,sizeof(outcap)); //获取设备表中当前输入设备信息  
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
	
	//// 根据保存文件设置选项
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


// 消息
void CSysCommonAudio::Notify(DuiLib::TNotifyUI& msg)
{
	DuiLib::WindowImplBase::Notify(msg);
	//if (_tcsicmp(msg.sType, _T("click")) == 0)
	//{
	//	
	//}
}


// 保存配置
void CSysCommonAudio::Save()
{
	// 根据保存文件设置选项
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
//// CSysCommonAudio 消息处理程序
//BOOL CSysCommonAudio::OnInitDialog()
//{
//	CDialog::OnInitDialog();
//
//	// 遍历所有子控件，设置控件字体
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
//	int innum = waveInGetNumDevs();    //获取波形音频输入设备数目  
//	for(int i=0;i<innum;i++)  
//	{  
//		MMRESULT rs = waveInGetDevCaps(i,&incap,sizeof(incap)); //获取设备表中当前输入设备信息  
//		m_comboVoiceInput.AddString(incap.szPname);
//	}  
//	m_comboVoiceInput.SetCurSel(0);
//
//	WAVEOUTCAPS outcap;  
//	int outnum = waveOutGetNumDevs();    //获取波形音频输入设备数目  
//	for(int i=0;i<outnum;i++)  
//	{  
//		MMRESULT rs = waveOutGetDevCaps(i,&outcap,sizeof(outcap)); //获取设备表中当前输入设备信息  
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
//	// 根据保存文件设置选项
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
//			pDC->SetTextColor(RGB(255,0,0));//（255,0，0） //(74,123,211)
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
//	// 根据保存文件设置选项
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