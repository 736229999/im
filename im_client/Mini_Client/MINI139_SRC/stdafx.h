// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 项目特定的包含文件

#pragma once

#define _ATL_APARTMENT_THREADED 
#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN		// 从 Windows 标头中排除不常使用的资料
#endif

// 如果您必须使用下列所指定的平台之前的平台，则修改下面的定义。
// 有关不同平台的相应值的最新信息，请参考 MSDN。
#ifndef WINVER				// 允许使用特定于 Windows 95 和 Windows NT 4 或更高版本的功能。
//#define WINVER 0x0400		// 将此更改为针对于 Windows 98 和 Windows 2000 或更高版本的合适的值。
#define WINVER 0x0501		// 将此更改为针对于 Windows 98 和 Windows 2000 或更高版本的合适的值。
#endif

#ifndef _WIN32_WINNT		// 允许使用特定于 Windows NT 4 或更高版本的功能。
//#define _WIN32_WINNT 0x0400	// 将此更改为针对于 Windows 2000 或更高版本的合适的值。
#define _WIN32_WINNT 0x0501	// 将此更改为针对于 Windows 2000 或更高版本的合适的值。
#endif						

#ifndef _WIN32_WINDOWS		// 允许使用特定于 Windows 98 或更高版本的功能。
//#define _WIN32_WINDOWS 0x0410 // 将此更改为针对于 Windows Me 或更高版本的合适的值。
#define _WIN32_WINDOWS 0x0501 // 将此更改为针对于 Windows Me 或更高版本的合适的值。
#endif

#ifndef _WIN32_IE			// 允许使用特定于 IE 4.0 或更高版本的功能。
//#define _WIN32_IE 0x0400	// 将此更改为针对于 IE 5.0 或更高版本的合适的值。
#define _WIN32_IE 0x0500	// 将此更改为针对于 IE 5.0 或更高版本的合适的值。
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
//自定义消息

#define WM_TRAYICON					WM_USER+0x01	 //主窗体接收Tray icon消息

#define WM_TRAYICON_UPDATE			WM_USER+0x02     //主窗体，状态更改

#define WM_MSGSEL_NOTIFY			WM_USER+0x03	 //浏览历史记录索引变化通知

#define WM_P2PSENDMSGDLG_NOTIFY		WM_USER+0x04	 //聊天框，向主窗体发送通知   

#define WM_CLUSTERSENDMSGDLG_NOTIFY	WM_USER+0x05

#define WM_P2P_TO_GROUP             WM_USER+0x06     //一对一聊天框转成多人聊天框

#define WM_HINTDLG_CLICK			WM_USER+0x07     //消息向主窗体发送框鼠标点击事件通知

#define WM_FACESEL_NOTIFY			WM_USER+0x08	 //头像选择结束后,向父窗体通知

#define WM_EMOTIONSEL_NOTIFY		WM_USER+0x09     //表情选择结束后,先父窗体通知


#define WM_XT_CMD_NOTIFY			WM_USER+0x0B	 //协议层向UI层上报数据通知

#define WM_HISTORY_REDRAW			WM_USER+0x0C	 //图文混排编辑控件自绘

#define WM_IMFRIEND_LIST_NOTIFY		WM_USER+0x0D     //好友列表先父窗体发送消息通知

#define WM_CLUSTER_LIST_NOTIFY		WM_USER+0x0E     //好友列表先父窗体发送消息通知

#define WM_GROUPNAME_EDIT			WM_USER+0x0F	 //修改组名称编辑控件事件

#define WM_CHECKVERSION				WM_USER+0x10	 //客户端版本更新通知

#define WM_SHOWMESSAGE              WM_USER+0x11

#define WM_NETWORK_ERROR            WM_USER+0x12

#define WM_LOGIN_FAILED             WM_USER+0x13  

#define WM_QUERY_PEERONLINE			WM_USER+0x14     //rich edit 询问聊天框是否可以转义自定义表情

#define WM_IM_OK                    WM_USER+0x15     //通知主窗体IM初始化OK了

#define WM_USER_HEAD_NOTIFY         WM_USER+0x16     //下载头像通知

#define WM_FAMILY_HEAD_NOTIFY       WM_USER+0x17	 //下载族徽通知

#define WM_MAGICFACE_CLICK          WM_USER+0x18

#define WM_139WEB_NOTIFY            WM_USER+0x19

#define WM_THREAD_NOTIFY            WM_USER+0x1A //通用thread notify

#define WM_HEART_TIMEOUT            WM_USER+0x1B

#define WM_DISMISS_CLUSTER			WM_USER+0x1C  //向FloorPageCluster发送解散群的消息

#define WM_CHANGE_FRIENDGROUP		WM_USER+0x1E //向FloorPageIM 发送好友组更改通知

#define WM_SAVECARD_NOTIFY          WM_USER+0x1E  //向FloorPageAddbook 发送保存名片的通知

#define WM_MAGICFACE_NOTIFY         WM_USER+0x1F  

#define WM_WEBFACE_NOTIFY           WM_USER+0x20  

#define WM_WEBPIC_NOTIFY            WM_USER+0x21   //写日志,从网络的相册中选择图片通知

#define WM_XT_RESEND_NOTIFY			WM_USER+0x22	

#define WM_SHOW_HISTORY_FIND_TOOL	WM_USER+0x23	//显示/隐藏历史记录查找工具栏

#define WM_HISTORY_PAGE				WM_USER+0x24	//

#define WM_MOD_BACKNAME				WM_USER+0x25	//

#define WM_MAINTAB_SEL_CHANGE	    WM_USER+32		

#define WM_SYSTEM_SETTING_MSG	    WM_USER+33		// 系统设置窗口点击确认

#define WM_DOWNLOADFACE				WM_USER+0x1001	// 

#define WM_DOWNLOADFACE_OK			WM_USER+0x1002	// 

#define WM_DOWNLOADPROCESS			WM_USER+0x1003	// 

#define WM_INSERTMSG				WM_USER+0x1004	

#define WM_WEBDATA_NOTIFY			WM_USER+0x1005  //

#define WM_WEBFLASH_NOTIFY			WM_USER+0x1006	// 

#define WM_WEBFILE_NOTIFY			WM_USER+0x1007

#define	WM_WEBPICTURE_NOTIFY		WM_USER+0x1008


//网站的call me 消息
#define WM_WEB_CALLME				WM_USER+0x1009

// 向im界面发送操作命令
#define WM_IMOPERATION				WM_USER+0x1100

#define WM_USER_DROPFILE			WM_USER+0x1101

#define WM_RICHEDIT_NOTIFY			WM_USER+0x1102  //Richedit的通知消息

#define WM_P2PNET_NOTIFY            WM_USER+0x1103
#define WM_MODULE_NOTIFY            WM_USER+0x2001  //模块之间的消息通知

#define WM_P2PVEDIO_NOTIFY          WM_USER+0x1104  //视频聊天面板通知消息
#define WM_P2PAUDIO_NOTIFY          WM_USER+0x1105  //语音聊天面板通知消息

#define WM_ON_SET_ONLINE_MEMBER	    WM_USER+0x1106
//#define WM_SET_HEAD_TITLE	        WM_USER+0x1107

#define WM_UPLOADING_HTHREAD_MSG	WM_USER+0x1108
#define WM_BRINGTOTOP_MSG	        WM_USER+0x1109
#define WM_ADD_FRIEND_TO_LIST	    WM_USER+0x1110
#define WM_SNAPSHOT	                WM_USER+0x1111
#define WM_EXIT_MAINFRM	            WM_USER+0x1112

#define WM_OPEN_SELLER_WIN          WM_USER+0x1113

#define WM_P2P_SEND                 WM_USER+0x1114     //传输确认
#define WM_P2P_RECV                 WM_USER+0x1115     //重传或者续传
#define WM_P2P_FINISH               WM_USER+0x1116     //传输完成

#define WM_OPEN_WORK_WIN            WM_USER+0x1117     //工作版打开窗口
#define WM_OPEN_CS_INSERT_MSG_WIN   WM_USER+0x1118     //工作台插入会话

#define MIN_CLUSTER_MEMBER	3	// 创建组必须满足的最少人数

//全局统一字体
extern CFont        g_fontText		 ;
extern CFont        g_fontText2      ;

//全局统一字体，微软雅黑
extern CFont g_font16,g_font16B;
extern CFont g_font14,g_font14B;
extern CFont g_font13,g_font13B;
extern CFont g_font12,g_font12B;
extern CFont g_font8;

//全局头像
extern CImage g_ImgIcon;
extern CImage g_ImgIconGray;
extern CImage g_ImgIconGroup;
extern CImage g_ImgFaceFrame;
extern CImage g_ImgStatusBusy;
extern CImage g_ImgStatusLeave;
extern CImage g_ImgStatusHide;

extern Image* g_ImgIcon2;
//extern Image g_ImgFaceFrame2;

//聊天框背景色
#define CHAT_BKG_COLOR				RGB(0xDD, 0xE6, 0xED)

//聊天框底部颜色
#define CHAT_BOTTOM_COLOR			RGB(0xFF, 0xFF, 0xFF)

//下拉框
#define COMBOBOX_BORDER_NORMAL		RGB(0xC8, 0xC8, 0xC8)
#define COMBOBOX_BORDER_HOVER		RGB(0x4D, 0x95, 0xC2)
#define COMBOBOX_BORDER_PRESSED		RGB(0x38, 0x61, 0x7B)
//(现在移到工程属性中来定义)
//#define _BUSINESS_VESION		// 编译卖家版时定义此宏，编译买家版时直接注释掉


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

#define MOBILE_TEST_	// 移动端测试
#define LOGIN_TEST_	// 登录压测