// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���Ǿ���ʹ�õ��������ĵ�
// ��Ŀ�ض��İ����ļ�

#pragma once

#define _ATL_APARTMENT_THREADED 
#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN		// �� Windows ��ͷ���ų�����ʹ�õ�����
#endif

// ���������ʹ��������ָ����ƽ̨֮ǰ��ƽ̨�����޸�����Ķ��塣
// �йز�ͬƽ̨����Ӧֵ��������Ϣ����ο� MSDN��
#ifndef WINVER				// ����ʹ���ض��� Windows 95 �� Windows NT 4 ����߰汾�Ĺ��ܡ�
//#define WINVER 0x0400		// ���˸���Ϊ����� Windows 98 �� Windows 2000 ����߰汾�ĺ��ʵ�ֵ��
#define WINVER 0x0501		// ���˸���Ϊ����� Windows 98 �� Windows 2000 ����߰汾�ĺ��ʵ�ֵ��
#endif

#ifndef _WIN32_WINNT		// ����ʹ���ض��� Windows NT 4 ����߰汾�Ĺ��ܡ�
//#define _WIN32_WINNT 0x0400	// ���˸���Ϊ����� Windows 2000 ����߰汾�ĺ��ʵ�ֵ��
#define _WIN32_WINNT 0x0501	// ���˸���Ϊ����� Windows 2000 ����߰汾�ĺ��ʵ�ֵ��
#endif						

#ifndef _WIN32_WINDOWS		// ����ʹ���ض��� Windows 98 ����߰汾�Ĺ��ܡ�
//#define _WIN32_WINDOWS 0x0410 // ���˸���Ϊ����� Windows Me ����߰汾�ĺ��ʵ�ֵ��
#define _WIN32_WINDOWS 0x0501 // ���˸���Ϊ����� Windows Me ����߰汾�ĺ��ʵ�ֵ��
#endif

#ifndef _WIN32_IE			// ����ʹ���ض��� IE 4.0 ����߰汾�Ĺ��ܡ�
//#define _WIN32_IE 0x0400	// ���˸���Ϊ����� IE 5.0 ����߰汾�ĺ��ʵ�ֵ��
#define _WIN32_IE 0x0500	// ���˸���Ϊ����� IE 5.0 ����߰汾�ĺ��ʵ�ֵ��
#endif

// MFC
#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions


// Common Controls
#include <afxdtctl.h>			// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>				// MFC support for Windows Common Controls
#endif		

#include <afxtempl.h>
#include <afxmt.h>			// MFC Threading
#include <afxinet.h>		// MFC Internet
#include <afxsock.h>		// MFC Socketss
#include <afxpriv.h>

#pragma warning(disable:4786)
#pragma warning(disable:4018) // Disable un-signed/signed comparison warnings
#pragma warning(disable:4244) // Disable type conversion warnings
#pragma warning(disable:4800)
#pragma warning(disable:4311)
#pragma warning(disable:4267)
#pragma warning(disable:4312)
#pragma warning(disable:4355)
#pragma warning(disable:4996 )



#include <io.h>

#include <assert.h>
// STL includes
#include <vector>
#include <list>
#include <map>
#include <algorithm>
#include <functional>
using namespace std;

#include <gdiplus.h>
using namespace Gdiplus;

//#include "C:\Program Files\MSXML 4.0\inc\msxml2.h" 


#include <hmgskincontrol.h>
#include "..\hmgcrtlib\hmgcrtlib.h"
#include "buttonext.h"
#include <afxcontrolbars.h>

#include "../json/json/json.h"


//#define _MINI_CLIENT_
//�Զ�����Ϣ

#define WM_TRAYICON					WM_USER+0x01	 //���������Tray icon��Ϣ

#define WM_TRAYICON_UPDATE			WM_USER+0x02     //�����壬״̬����

#define WM_MSGSEL_NOTIFY			WM_USER+0x03	 //�����ʷ��¼�����仯֪ͨ

#define WM_P2PSENDMSGDLG_NOTIFY		WM_USER+0x04	 //������������巢��֪ͨ   

#define WM_CLUSTERSENDMSGDLG_NOTIFY	WM_USER+0x05

#define WM_P2P_TO_GROUP             WM_USER+0x06     //һ��һ�����ת�ɶ��������

#define WM_HINTDLG_CLICK			WM_USER+0x07     //��Ϣ�������巢�Ϳ�������¼�֪ͨ

#define WM_FACESEL_NOTIFY			WM_USER+0x08	 //ͷ��ѡ�������,�򸸴���֪ͨ

#define WM_EMOTIONSEL_NOTIFY		WM_USER+0x09     //����ѡ�������,�ȸ�����֪ͨ


#define WM_XT_CMD_NOTIFY			WM_USER+0x0B	 //Э�����UI���ϱ�����֪ͨ

#define WM_HISTORY_REDRAW			WM_USER+0x0C	 //ͼ�Ļ��ű༭�ؼ��Ի�

#define WM_IMFRIEND_LIST_NOTIFY		WM_USER+0x0D     //�����б��ȸ����巢����Ϣ֪ͨ

#define WM_CLUSTER_LIST_NOTIFY		WM_USER+0x0E     //�����б��ȸ����巢����Ϣ֪ͨ

#define WM_GROUPNAME_EDIT			WM_USER+0x0F	 //�޸������Ʊ༭�ؼ��¼�

#define WM_CHECKVERSION				WM_USER+0x10	 //�ͻ��˰汾����֪ͨ

#define WM_SHOWMESSAGE              WM_USER+0x11

#define WM_NETWORK_ERROR            WM_USER+0x12

#define WM_LOGIN_FAILED             WM_USER+0x13  

#define WM_QUERY_PEERONLINE			WM_USER+0x14     //rich edit ѯ��������Ƿ����ת���Զ������

#define WM_IM_OK                    WM_USER+0x15     //֪ͨ������IM��ʼ��OK��

#define WM_USER_HEAD_NOTIFY         WM_USER+0x16     //����ͷ��֪ͨ

#define WM_FAMILY_HEAD_NOTIFY       WM_USER+0x17	 //�������֪ͨ

#define WM_MAGICFACE_CLICK          WM_USER+0x18

#define WM_139WEB_NOTIFY            WM_USER+0x19

#define WM_THREAD_NOTIFY            WM_USER+0x1A //ͨ��thread notify

#define WM_HEART_TIMEOUT            WM_USER+0x1B

#define WM_DISMISS_CLUSTER			WM_USER+0x1C  //��FloorPageCluster���ͽ�ɢȺ����Ϣ

#define WM_CHANGE_FRIENDGROUP		WM_USER+0x1E //��FloorPageIM ���ͺ��������֪ͨ

#define WM_SAVECARD_NOTIFY          WM_USER+0x1E  //��FloorPageAddbook ���ͱ�����Ƭ��֪ͨ

#define WM_MAGICFACE_NOTIFY         WM_USER+0x1F  

#define WM_WEBFACE_NOTIFY           WM_USER+0x20  

#define WM_WEBPIC_NOTIFY            WM_USER+0x21   //д��־,������������ѡ��ͼƬ֪ͨ

#define WM_XT_RESEND_NOTIFY			WM_USER+0x22	

#define WM_SHOW_HISTORY_FIND_TOOL	WM_USER+0x23	//��ʾ/������ʷ��¼���ҹ�����

#define WM_HISTORY_PAGE				WM_USER+0x24	//

#define WM_MOD_BACKNAME				WM_USER+0x25	//

#define WM_MAINTAB_SEL_CHANGE	    WM_USER+32		

#define WM_SYSTEM_SETTING_MSG	    WM_USER+33		// ϵͳ���ô��ڵ��ȷ��

#define WM_DOWNLOADFACE				WM_USER+0x1001	// 

#define WM_DOWNLOADFACE_OK			WM_USER+0x1002	// 

#define WM_DOWNLOADPROCESS			WM_USER+0x1003	// 

#define WM_INSERTMSG				WM_USER+0x1004	

#define WM_WEBDATA_NOTIFY			WM_USER+0x1005  //

#define WM_WEBFLASH_NOTIFY			WM_USER+0x1006	// 

#define WM_WEBFILE_NOTIFY			WM_USER+0x1007

#define	WM_WEBPICTURE_NOTIFY		WM_USER+0x1008


//��վ��call me ��Ϣ
#define WM_WEB_CALLME				WM_USER+0x1009

// ��im���淢�Ͳ�������
#define WM_IMOPERATION				WM_USER+0x1100

#define WM_USER_DROPFILE			WM_USER+0x1101

#define WM_RICHEDIT_NOTIFY			WM_USER+0x1102  //Richedit��֪ͨ��Ϣ

#define WM_P2PNET_NOTIFY            WM_USER+0x1103
#define WM_MODULE_NOTIFY            WM_USER+0x2001  //ģ��֮�����Ϣ֪ͨ

#define WM_P2PVEDIO_NOTIFY          WM_USER+0x1104  //��Ƶ�������֪ͨ��Ϣ
#define WM_P2PAUDIO_NOTIFY          WM_USER+0x1105  //�����������֪ͨ��Ϣ

#define WM_ON_SET_ONLINE_MEMBER	    WM_USER+0x1106
//#define WM_SET_HEAD_TITLE	        WM_USER+0x1107

#define WM_UPLOADING_HTHREAD_MSG	WM_USER+0x1108
#define WM_BRINGTOTOP_MSG	        WM_USER+0x1109
#define WM_ADD_FRIEND_TO_LIST	    WM_USER+0x1110
#define WM_SNAPSHOT	                WM_USER+0x1111
#define WM_EXIT_MAINFRM	            WM_USER+0x1112

#define WM_OPEN_SELLER_WIN          WM_USER+0x1113

#define WM_P2P_SEND                 WM_USER+0x1114     //����ȷ��
#define WM_P2P_RECV                 WM_USER+0x1115     //�ش���������
#define WM_P2P_FINISH               WM_USER+0x1116     //�������

#define WM_OPEN_WORK_WIN            WM_USER+0x1117     //������򿪴���
#define WM_OPEN_CS_INSERT_MSG_WIN   WM_USER+0x1118     //����̨����Ự

#define MIN_CLUSTER_MEMBER	3	// ����������������������

//ȫ��ͳһ����
extern CFont        g_fontText		 ;
extern CFont        g_fontText2      ;

//ȫ��ͳһ���壬΢���ź�
extern CFont g_font16,g_font16B;
extern CFont g_font14,g_font14B;
extern CFont g_font13,g_font13B;
extern CFont g_font12,g_font12B;
extern CFont g_font8;

//ȫ��ͷ��
extern CImage g_ImgIcon;
extern CImage g_ImgIconGray;
extern CImage g_ImgIconGroup;
extern CImage g_ImgFaceFrame;
extern CImage g_ImgStatusBusy;
extern CImage g_ImgStatusLeave;
extern CImage g_ImgStatusHide;

extern Image* g_ImgIcon2;
//extern Image g_ImgFaceFrame2;

//����򱳾�ɫ
#define CHAT_BKG_COLOR				RGB(0xDD, 0xE6, 0xED)

//�����ײ���ɫ
#define CHAT_BOTTOM_COLOR			RGB(0xFF, 0xFF, 0xFF)

//������
#define COMBOBOX_BORDER_NORMAL		RGB(0xC8, 0xC8, 0xC8)
#define COMBOBOX_BORDER_HOVER		RGB(0x4D, 0x95, 0xC2)
#define COMBOBOX_BORDER_PRESSED		RGB(0x38, 0x61, 0x7B)
//(�����Ƶ�����������������)
//#define _BUSINESS_VESION		// �������Ұ�ʱ����˺꣬������Ұ�ʱֱ��ע�͵�


#include <atlbase.h>
#include <atlcom.h>
#include <atlctl.h>

#ifdef _DEBUG
#   ifdef _UNICODE
#       pragma comment(lib, "../../libDebug/DuiLib_ud.lib")
#   else
#       pragma comment(lib, "../../libDebug/DuiLib_d.lib")
#   endif
#else
#   ifdef _UNICODE
#       pragma comment(lib, "../../libRelease/DuiLib_u.lib")
#   else
#       pragma comment(lib, "../../libRelease/DuiLib.lib")
#   endif
#endif

#if defined _M_IX86
#pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_IA64
#pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='ia64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif
#ifdef _DEBUG
#include "vld.h"
#endif

#if (defined _CS_PLAT_) && (defined _BUSINESS_VESION)
#define _CS_PLAT_EX_
#endif

#if (defined _WORK_) && (defined _BUSINESS_VESION)
#define _WORK_EX_
#endif

#ifdef _CS_PLAT_EX_
#define RECENTLIST_OP(X) \
{\
	std::tr1::shared_ptr<CDuiCSPlatDlg> pIn = CDuiCSPlatDlg::GetInstance();\
	if(pIn != NULL && ::IsWindow(pIn->GetHWND()))\
{\
	CDuiRecentList *pList = pIn->GetRecentList();\
	if(pList != NULL)\
{\
	pList->X;\
}\
}\
}
#else
#define RECENTLIST_OP(X)
#endif

#define MOBILE_TEST_	// �ƶ��˲���
#define LOGIN_TEST_	// ��¼ѹ��