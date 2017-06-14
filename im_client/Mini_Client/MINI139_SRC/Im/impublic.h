#ifndef _IM_PUBLIC_
#define _IM_PUBLIC_

#include "..\..\include\ClientProtocol.h" 

#ifdef _DEBUG
#import "..\..\binDebug\UserInfo.dll" named_guids no_namespace
#import "..\..\binDebug\WriteArticle.dll" named_guids no_namespace
#import "..\..\binDebug\MyArticles.dll" named_guids no_namespace
#import "..\..\binDebug\UserInfo.dll" named_guids no_namespace
#import "..\..\binDebug\MyPictures.dll" named_guids no_namespace
#import "..\..\binDebug\HeadPhoto.dll" named_guids no_namespace
//#import "...\..\binDebug\PictureLoadX.dll" named_guids no_namespace
#else
#import "..\..\binRelease\UserInfo.dll" named_guids no_namespace
#import "..\..\binRelease\WriteArticle.dll" named_guids no_namespace
#import "..\..\binRelease\MyArticles.dll" named_guids no_namespace
#import "..\..\binRelease\UserInfo.dll" named_guids no_namespace
#import "..\..\binRelease\MyPictures.dll" named_guids no_namespace
#import "..\..\binRelease\HeadPhoto.dll" named_guids no_namespace
//#import "...\..\binRelease\PictureLoadX.dll" named_guids no_namespace
#endif


#import "..\..\..\SpaceManagement\WebConfig\Debug\WebConfig.tlb" named_guids no_namespace
/*
#import "..\..\..\SpaceManagement\bin\WriteArticle.dll" named_guids no_namespace
#import "..\..\..\SpaceManagement\bin\MyArticles.dll" named_guids no_namespace
#import "..\..\..\SpaceManagement\bin\UserInfo.dll" named_guids no_namespace
#import "..\..\..\SpaceManagement\bin\MyPictures.dll" named_guids no_namespace
#import "..\..\..\SpaceManagement\bin\HeadPhoto.dll" named_guids no_namespace
#import "..\..\..\SpaceManagement\bin\PictureLoadX.dll" named_guids no_namespace
*/
#include "spacetypedef.h"
#include "SpaceData.h"
#include "..\utils\function.h"
#include "resgdiobj.h"
#include "messageBox.h"

#include "WebDataProcess.h"
#include "WebPictureProcess.h"
#include "WebInterfaceWrapper.h"
/********************************
**用于定义 IM 模块所用的全局变量
**和类型
**
*********************************/

#define SERVER_ACK_TIMEOUT			5000   // 报文等待ack超时，20秒
#define P2P_ACK_TIMEOUT			    5000

#define XTREAM_SERVER_PORT			PORT_IM_BUSINESS
//#define XTREAM_LOCAL_PORT			3330
#define REG_ACCOUNT_MAX				100
//Leading 7 bytes of Data package

#define HELPER_ID                   260953

//字体定义
#define XTF_ITALIC				0x01
#define XTF_UNDERLINE			0x02
#define XTF_BOLD				0x04

//字符串最长
#define MAX_STR_LEN				255
#define MAX_INT					65535

#define IM_SPACE_COLOR			RGB(0x4d,0x95,0xc2)
#define IM_LINE_COLOR			RGB(255,255,255)

#ifdef _CS_PLAT_EX_   ////飞牛客服工作台
   #define IM_CAPTION           "飞牛客服工作台"
#else
   #define IM_CAPTION			"哞哞"
#endif

#define IDS_NOPASS				"对不起，您的密码错误。提示：合法密码应由字符A-Z、a-z、0-9及下划线组成，长度为6-15Bytes。"
#define IDS_WRONGNO				"请输入您的用户名！"

#define MAX_CLUSTER_MEMBERS      300//最大群成员个数

#define HEAD_SZ					 35//头像Icon尺寸

#define FILE_PACKET_LEN			 1240 // 图片文件传输包中数据字节数

//图片放映缩略图尺寸
#define PICTURE_CX				80
#define PICTURE_CY				80
#define FRAME_SZ				85
#define PICTURE_SPACE			10


//api 接口



#if defined _VER_PREVIEW_
	#define BUYER_LOGIN_API 		"stage-member-api01.idc1.fn"								/*买家登录api*/
	#define SELLER_LOGIN_API		"zhaoshang-api.idc1.fn"										/*卖家登录api*/
	#define MERCHANT_API			"http://zhaoshang-api.idc1.fn/api/call.do"              /*获取商家ID的接口*/

	#define BUYER_REGISTER_URL		"https://member.feiniu.com/getaway/register"				/*买家注册*/
	#define SELLER_REGISTER_URL 	"http://sjmember.feiniu.com/static/html/register.html"		/*卖家注册*/

	#define BUYER_FORGET_PSW_URL	"https://member.feiniu.com/securityView/lostPwd"			/*买家忘记密码*/
	#define SELLER_FORGET_PSW_URL	"http://sjmember.feiniu.com/static/html/wjml.html"			/*卖家忘记密码*/
	#define WORK_FORGET_PSW_URL		"http://backend.idc1.fn/index.jsp"						    /*工作人员修改密码*/

	#define HEADICON_URL			"http://merchant.feiniu.com/webim/static/images/moumoupc/"	/*系统头像*/
	#define PICTURE_URL_OLD         ".fn-mart.com/"                                             /*旧图片空间地址*/	
	#define PICTURE_URL             "http://img16.fn-mart.com"                    /*图片空间*/
	#define PICTURE_URL2            "http://img17.fn-mart.com"                    /*图片空间*/
	#define PICTURE_URL3            "http://img18.fn-mart.com"                    /*图片空间*/
	#define RIGHT_URL				"http://merchant.feiniu.com/webim/right.html"				/*右侧窗口*/

	#define UPLOAD_DUMP_URL			"http://imindex.idc1.fn/dump-tracker/upload-for-pc-merchant.do" /*dump文件上传地址*/

	#define ORDER_TRADE_URL          "http://merchant.feiniu.com/order/detail/orderDetail.do?" /*新订单和已付款的地址*/
	#define ORDER_COMMENT_URL        "http://merchant.beta.fn.com/portal/index.do#/s5_6"       /*评论管理的地址*/
	#define ORDER_RETURN_URL         "http://merchant.feiniu.com/order/orderReturn/order_refund.do?"        /*退换货管理的地址*/
#ifdef _CS_PLAT_EX_
	#define RIGHT_URL_SELLER		"http://webim-cs.feiniu.com/webRight/service-log.htm?"      /*Preview客服工作台客服对买家右侧页*/
	#define EMPLOYEE_RIGHT_URL	    "http://webim-cs.feiniu.com/serviceProfile/right_profile.html?" /*Preview环境客服工作台客服对客服右侧页*/
	#define QUICK_PHRASE_URL	    "http://webim-cs.feiniu.com/serviceProfile/quick-phrase?" /*Preview环境获取快捷短语地址*/
#elif defined _WORK_EX_
	#define RIGHT_URL_WORK			"http://merchant.feiniu.com/webim/right_in.html"          /*工作版右侧窗口对面是工作人员*/
	#define RIGHT_URL_SELLER		"http://merchant.feiniu.com/webim/right_bm.html"          /*工作版右侧窗口对面是商家*/
#else
	#define RIGHT_URL_WORK			"http://merchant.feiniu.com/webim/right_in.html"          /*商家版右侧窗口对面是工作人员*/
	#define RIGHT_URL_SELLER		"http://merchant.feiniu.com/webim/seller_right.html"      /*卖家版右侧窗口*/
#endif
    #define WEBIM_URL               "merchant.feiniu.com"                                       /*Webim根地址*/
	#define COOKIE_URL              ".feiniu.com"                                               /*设置cookie的根地址*/
    #define WEBIM_LINKINFO_STORE    "webim/vender?VENDER_INFO_TYPE=GET_STORE_INFO&merchant_id=" /*获取店铺信息接口*/
    #define WEBIM_LINKINFO_ITEM     "webim/vender?VENDER_INFO_TYPE=GET_PRODUCT_INFO&product_id="/*获取商品信息接口*/
	#define STORE_URL               "http://store.feiniu.com/index-"                            /*店铺地址*/
	#define ITEM_URL                "http://item.feiniu.com/"                                   /*商品地址*/

	#define SERVER_IP				/*"10.201.201.24"*/"10.212.129.181"							/*服务器ip*/
	#define	SERVER_PORT             10002														/*服务器ip*/

	#define LOAD_PIC_URL			"http://merchant.feiniu.com/webim/upload-photo.do?isFromIM=1"	/*上传图片到图片空间的地址*/

	#define RESET_PWD_URL			"https://sjmember.feiniu.com/static/html/wjml.html?backUrl=http%3A%2F%2Fmerchant.feiniu.com%2Fportal%2Findex.do"			/*重置密码*/
	#define BIND_MOBIL_URL			"https://member.beta.fn.com/static/html/accountSet/checkMobile.html"	/*绑定手机*/
	
	#define PROXY_URL               "proxy2.fn.com:8080"                                          /*preview商家后台和运营后台的代理*/
#elif defined _VER_ONLINE_
	#define BUYER_LOGIN_API 		"stage-member-api01.idc1.fn"								/*买家登录api*/
	#define SELLER_LOGIN_API		"zhaoshang-api.idc1.fn"										/*卖家登录api*/
	#define MERCHANT_API			"http://zhaoshang-api.idc1.fn/api/call.do"              /*获取商家ID的接口*/

	#define BUYER_REGISTER_URL		"https://member.feiniu.com/getaway/register"				/*买家注册*/
	#define SELLER_REGISTER_URL 	"http://sjmember.feiniu.com/static/html/register.html"		/*卖家注册*/

	#define BUYER_FORGET_PSW_URL	"https://member.feiniu.com/securityView/lostPwd"			/*买家忘记密码*/
	#define SELLER_FORGET_PSW_URL	"http://sjmember.feiniu.com/static/html/wjml.html"			/*卖家忘记密码*/
	#define WORK_FORGET_PSW_URL		"http://backend.idc1.fn/index.jsp"						    /*工作人员修改密码*/

	#define HEADICON_URL			"http://merchant.feiniu.com/webim/static/images/moumoupc/"	/*系统头像*/
	#define PICTURE_URL_OLD         ".fn-mart.com/"                                             /*旧图片空间地址*/
	#define PICTURE_URL             "http://img16.fn-mart.com"                    /*图片空间*/
	#define PICTURE_URL2            "http://img17.fn-mart.com"                    /*图片空间*/
	#define PICTURE_URL3            "http://img18.fn-mart.com"                    /*图片空间*/
	#define RIGHT_URL				"http://merchant.feiniu.com/webim/right.html"				/*右侧窗口*/	

	#define UPLOAD_DUMP_URL			"http://im.feiniu.com/dump-tracker/upload-for-pc-merchant.do" /*dump文件上传地址*/

	#define ORDER_TRADE_URL          "http://merchant.feiniu.com/order/detail/orderDetail.do?" /*新订单和已付款的地址*/
	#define ORDER_COMMENT_URL        "http://merchant.beta.fn.com/portal/index.do#/s5_6"       /*评论管理的地址*/
	#define ORDER_RETURN_URL         "http://merchant.feiniu.com/order/orderReturn/order_refund.do?"        /*退换货管理的地址*/
#ifdef _CS_PLAT_EX_
	#define RIGHT_URL_SELLER		"http://webim-cs.feiniu.com/webRight/service-log.htm?"      /*Online客服工作台客服对买家右侧页*/
	#define EMPLOYEE_RIGHT_URL	    "http://webim-cs.feiniu.com/serviceProfile/right_profile.html?" /*Online环境客服工作台客服对客服右侧页*/
	#define QUICK_PHRASE_URL	    "http://webim-cs.feiniu.com/serviceProfile/quick-phrase?" /*Online环境获取快捷短语地址*/
#elif defined _WORK_EX_
	#define RIGHT_URL_WORK			"http://merchant.feiniu.com/webim/right_in.html"          /*工作版右侧窗口对面是工作人员*/
	#define RIGHT_URL_SELLER		"http://merchant.feiniu.com/webim/right_bm.html"          /*工作版右侧窗口对面是商家*/
#else
	#define RIGHT_URL_WORK			"http://merchant.feiniu.com/webim/right_in.html"          /*商家版右侧窗口对面是工作人员*/
	#define RIGHT_URL_SELLER		"http://merchant.feiniu.com/webim/seller_right.html"          /*卖家版右侧窗口*/
#endif
    #define WEBIM_URL               "merchant.feiniu.com"                                       /*Webim根地址*/
	#define COOKIE_URL              ".feiniu.com"                                               /*设置cookie的根地址*/
    #define WEBIM_LINKINFO_STORE    "webim/vender?VENDER_INFO_TYPE=GET_STORE_INFO&merchant_id=" /*获取店铺信息接口*/
    #define WEBIM_LINKINFO_ITEM     "webim/vender?VENDER_INFO_TYPE=GET_PRODUCT_INFO&product_id="/*获取商品信息接口*/
	#define STORE_URL               "http://store.feiniu.com/index-"                            /*店铺地址*/
	#define ITEM_URL                "http://item.feiniu.com/"                                   /*商品地址*/

	#define SERVER_IP				"221.133.245.172"										    /*服务器ip*/
	#define	SERVER_PORT             10002														/*服务器端口*/

	#define LOAD_PIC_URL			"http://merchant.feiniu.com/webim/upload-photo.do?isFromIM=1"	/*上传图片到图片空间的地址*/

	#define RESET_PWD_URL			"https://sjmember.feiniu.com/static/html/wjml.html?backUrl=http%3A%2F%2Fmerchant.feiniu.com%2Fportal%2Findex.do"			/*重置密码*/
	#define BIND_MOBIL_URL			"https://member.beta.fn.com/static/html/accountSet/checkMobile.html"	/*绑定手机*/

	#define PROXY_URL               "proxy-api.fn.com:8080"                                      /*online商家后台和运营后台的代理*/
#else  /*beta环境*/
	#define BUYER_LOGIN_API			"member-api.beta1.fn"										/*买家登录api*/
	#define SELLER_LOGIN_API		"zhaoshang-api.beta.fn.com"									/*卖家登录api*/
	#define MERCHANT_API			"http://zhaoshang-api.beta.fn.com/api/call.do"              /*获取商家ID的接口*/
	
	#define BUYER_REGISTER_URL  	"https://member.beta1.fn/sh/getaway/register"				/*买家注册*/
	#define SELLER_REGISTER_URL 	"http://member.beta.fn.com/static/html/register.html"		/*卖家注册*/
	
	#define BUYER_FORGET_PSW_URL   	"https://member.beta1.fn/sh/securityView/lostPwd"			/*买家忘记密码*/
	#define SELLER_FORGET_PSW_URL   "http://member.beta.fn.com/static/html/wjml.html"			/*卖家忘记密码*/
	#define WORK_FORGET_PSW_URL		"http://backend.beta.fn.com/index.jsp"						/*工作人员修改密码*/

	#define HEADICON_URL			"http://webim.beta.fn.com/webim/static/images/moumoupc/"	/*系统头像*/	
	#define PICTURE_URL_OLD		    "http://wh-image01.fn.com/"                                 /*商城图片空间*/     
	#define PICTURE_URL_OLD2		"http://img.beta1.fn/"                                      /*自营图片空间*/
	#define PICTURE_URL             "http://10.202.249.218"                       /*图片空间*/
	#define RIGHT_URL				"http://webim.beta.fn.com/webim/right.html"					/*右侧窗口*/

	#define UPLOAD_DUMP_URL          "http://messagecenter.beta.fn.com/imindex/dump-tracker/upload-for-pc-merchant.do" /*dump文件上传地址*/

	#define ORDER_TRADE_URL          "http://order.beta.fn.com/detail/orderDetail.do?" /*新订单和已付款的地址*/
	#define ORDER_COMMENT_URL        "http://merchant.beta.fn.com/portal/index.do#/s5_6"       /*评论管理的地址*/
	#define ORDER_RETURN_URL         "http://order.beta.fn.com/orderReturn/order_refund.do?"        /*退换货管理的地址*/

#ifdef _CS_PLAT_EX_
    #define RIGHT_URL_SELLER		"http://webim.beta1.fn/webim-cs/webRight/service-log.htm?"      /*BETA客服工作台客服对买家右侧页*/
    #define EMPLOYEE_RIGHT_URL	    "http://webim.beta1.fn/webim-cs/serviceProfile/right_profile.html?" /*BETA环境客服工作台客服对客服右侧页*/
	#define QUICK_PHRASE_URL	    "http://webim.beta1.fn/webim-cs/serviceProfile/quick-phrase?" /*BETA环境获取快捷短语地址*/
#elif defined _WORK_EX_
	#define RIGHT_URL_WORK			"http://webim.beta1.fn/webim/right_in.html"          /*beta工作版右侧窗口对面是工作人员*/
	#define RIGHT_URL_SELLER		"http://webim.beta1.fn/webim/right_bm.html"          /*beta工作版右侧窗口对面是商家*/
#else
	#define RIGHT_URL_WORK			"http://webim.beta1.fn/webim/right_in.html"          /*beta商家版右侧窗口对面是工作人员*/
	#define RIGHT_URL_SELLER		"http://webim.beta1.fn/webim/seller_right.html"          /*卖家版右侧窗口*/
#endif
	//#define RIGHT_URL_SELLER		"10.211.240.128/webim/seller_right.html"
    #define WEBIM_URL               "webim.beta1.fn"                                            /*Webim根地址*/
	#define COOKIE_URL              ".beta1.fn"                                               /*设置cookie的根地址*/
    #define WEBIM_LINKINFO_STORE    "webim/vender?VENDER_INFO_TYPE=GET_STORE_INFO&merchant_id=" /*获取店铺信息接口*/
    #define WEBIM_LINKINFO_ITEM     "webim/vender?VENDER_INFO_TYPE=GET_PRODUCT_INFO&product_id="/*获取商品信息接口*/
    #define STORE_URL               "http://storefront.beta1.fn/index-"                         /*店铺地址*/
    #define ITEM_URL                "http://item.beta1.fn/"                                     /*商品地址*/

	#define SERVER_IP				"10.211.64.73" 												/*服务器ip*/
	#define	SERVER_PORT             10002														/*服务器端口*/

	#define LOAD_PIC_URL			"http://webim.beta1.fn/webim/upload-photo.do?isFromIM=1"	/*上传图片到图片空间的地址*/

	#define RESET_PWD_URL			"https://member.beta.fn.com/static/html/wjml.html"			/*重置密码*/	
	#define BIND_MOBIL_URL			"https://member.beta.fn.com/static/html/accountSet/checkMobile.html"	/*绑定手机*/
#endif

#ifdef _VER_INTERNAL_
#define CLIENT_NAME_BUYER			"哞哞内部版2015"
#else
#define CLIENT_NAME_BUYER			"哞哞2015"
#endif

#define CLIENT_NAME_SELLER			"哞哞商家版2015"
#define FEINIU_CS_PLAT			    "飞牛客服工作台"
#define CLIENT_NAME_WORK			"哞哞工作版2016"
//当前登陆用户状态,信息


enum DOWNLOAD_STATE{
	DS_DOWNLOADING = 0,
	DS_SUCCESS,
	DS_FAILED 
};
struct DOWNLOAD_OBJ{
	CString	sUrl;	// 待下载文件
	CString	sSaved;	// 下载文件保存地址
	HANDLE	handle;	// 线程句柄
	UINT	timer;	// 线程运行时间
	UINT	state;	// 0.正在下载;1.下载成功;2.下载失败
};

// 获取所有系统字体名
std::vector<std::string>* GetAllSysFontName();

// “我的团队”groupID
#define  MY_TEAM_GROUP_ID		250
// 内部版和客服工作台版本
#if defined _VER_PREVIEW_
	#define  FEINIU_GROUP_ID		136
#elif defined _VER_ONLINE_
	#define  FEINIU_GROUP_ID		136
#else
	#define  FEINIU_GROUP_ID		177
#endif


//FIFO模板类
#include <list>
using namespace std;
template <class T>
class FIFO : public list<T>
{
public:
	inline bool Pop(T& data)
	{
		if ( size()>0 ) 
		{
			data = front();
			pop_front();
			return true;
		}
		return false;
	}

	inline bool Push(const T& data,bool bTail=true)
	{
		if ( bTail )
			push_back(data);
		else
			push_front(data);

		return true;
	}

	inline long  GetCount()
	{
		return size();
	}

	inline void Clear()
	{
		clear();
	}

	T operator [] (int n)
	{
		ASSERT( n>=0 || n<size() );

		list<T>::iterator it;
		it=begin();
		while (n-->0)
			it++;
		return *it;
	}
};

template <class T>
class FIFO2 : public FIFO<T>
{
public:
	inline void Lock() { ; }
	inline BOOL Unlock() { return TRUE; }
	
private:
	//CCriticalSection m_sec;
};

typedef struct XT_FONT
{
	//字体名称
	char fontName[MAX_STR_LEN];

	//字体大小
	int  fontSize;

	//风格
	BYTE flags; 

	//颜色
	COLORREF fontColor;

	XT_FONT()
	{
		sprintf(fontName ,"宋体");
		fontSize  = 10;
		vector<string> *pvecFontName = NULL;
		pvecFontName = GetAllSysFontName();
		vector<string>::iterator it = ::find(pvecFontName->begin(), pvecFontName->end(), "微软雅黑");
		if(it != pvecFontName->end())
		{
			sprintf(fontName ,"微软雅黑");
			fontSize  = 9;
		}		
		flags     = 0;
		fontColor = 0;
	}

	XT_FONT(const XT_FONT& font)
	{
		this->flags = font.flags;
		this->fontColor = font.fontColor;
		strcpy(this->fontName, font.fontName);
		this->fontSize = font.fontSize;
	}
	XT_FONT& operator=(const XT_FONT& font)
	{
		if(this == &font)
			return *this;
		this->flags = font.flags;
		this->fontColor = font.fontColor;
		strcpy(this->fontName, font.fontName);
		this->fontSize = font.fontSize;
		return *this;
	}
} XT_FONT;

struct RegAccountStruct//for save to local file use
{
	char    cUsername[MAX_USERNAME_LEN+1];
	char	cPass[MAX_PASSWORD_LEN+1];
	BOOL	bHide;		//隐身登录
	int		iLoginFlag; //登录状态
	BOOL	bAutoLogin;	//自动登录
	BOOL	bRemeberPsw; //记住密码
	BOOL    bShowHint;
	DWORD	dwServerIP;	//登录server
	RegAccountStruct()
	{
		memset(this, 0, sizeof(RegAccountStruct));
	}
};


enum IM_OPERATION{
	OPR_DEL_CLUSTER,	// 删除群
	OPR_MODIFY_CLUSTER_NAME   ///更改群签名或者群介绍
};


struct GROUP_ITEM
{
	char name[50];
	int  group_state;//expand or close
	int  group_index;
	int  group_father;
};

struct GROUP_INFO 
{
	DWORD dwUserId;
	int   count;

	GROUP_ITEM group[100];
	GROUP_INFO()
	{
		memset(this,0,sizeof(GROUP_INFO));
	}

#ifdef _BUSINESS_VESION
	//对分组信息排序（卖家版，我的团队排第一位）
	void Sort()
	{
		int i;
		//我的团队在第一位
		for ( i=1; i<count; i++ )
		{
#ifdef _CS_PLAT_EX_
			if (group[i].group_index == FEINIU_GROUP_ID)// 客服工作台
#else
			if (group[i].group_index == MY_TEAM_GROUP_ID)//我的团队
#endif
			
			{
				GROUP_ITEM temp=group[i];
				group[i]=group[0];			
				group[0]=temp;			
				break;
			}
		}

		//我的好友在第二位
		for (i = 2; i < count; ++i)
		{
			if (group[i].group_index == 0)//我的好友
			{
				GROUP_ITEM temp = group[i];
				group[i] = group[1];
				group[1] = temp;
				break;
			}
		}

		// 黑名单再倒数第二位
		for( i=1; i<count-1; i++ )
		{
			if (group[i].group_index == 2)//黑名单
			{
				GROUP_ITEM temp=group[i];
				group[i]=group[count-1];
				group[count-1]=temp;
				break;
			}
		}

		// 陌生人在最后一位
		for ( i=1; i<count-2; i++ )
		{
			if (group[i].group_index == 1)//陌生人
			{
				GROUP_ITEM temp=group[i];
				group[i]=group[count-2];
				group[count-2]=temp;
				break;
			}
		}
	}
#else
	//对分组信息排序
	void Sort()
	{
		int i;
		//我的好友在第一位
		for (i = 1; i < count; i++)
		{
			if (group[i].group_index == 0)//我的好友
			{
				GROUP_ITEM temp = group[i];
				group[i] = group[0];
				group[0] = temp;
				break;
			}
		}

		for (i = 1; i < count - 1; i++)
		{
			if (group[i].group_index == 2)//黑名单
			{
				GROUP_ITEM temp = group[i];
				group[i] = group[count - 1];
				group[count - 1] = temp;
				break;
			}
		}

		for (i = 1; i < count - 2; i++)
		{
			if (group[i].group_index == 1)//陌生人
			{
				GROUP_ITEM temp = group[i];
				group[i] = group[count - 2];
				group[count - 2] = temp;
				break;
			}
		}
	}
#endif

	int FindItemIndex(int iGroupId)
	{
		for ( int i=0; i<count; i++ )
		{
			if ( group[i].group_index == iGroupId )
			{
				return i;
			}
		}
		return -1;
	}
	int DelGroup(int iGroupId)
	{
		int iGroupIndex = FindItemIndex(iGroupId);
		if(iGroupIndex != -1)
		{
			for (int j=iGroupIndex; j<count-1; j++)
			{
				group[j] = group[j+1];
			}
			memset(&(group[count-1]),0, sizeof(GROUP_ITEM));
			count--;
			return count;
		}
		return -1;
	}

	// 获取一个组的层级
	int GetLevel(int nGroupID)
	{
		// 查找
		int nIndex = FindItemIndex(nGroupID);
		if(nIndex == -1)
			return -1;	// 未查找到此组

		int nLevel = 1;
		while(TRUE)
		{
			int nParentGroupID = group[nIndex].group_father;
			if(nParentGroupID == -1)
				break;

			nIndex = FindItemIndex(nParentGroupID);
			if(nIndex == -1)
				return -1;	// 不应该出现找不到的情况

			nLevel++;
		}

		return nLevel;
	}

	int FindItemIndex(const char *pGroupName)
	{
		for ( int i=0; i<count; i++ )
		{
			if ( strcmp(pGroupName, group[i].name) == 0 )
			{
				return i;
			}
		}
		return -1;
	}
};


struct FRIEND_INFO : public XT_USER_INFO
{
	// 注意，在基类结构改变时也要更新版本号
	const static uint32 VER = 0x00000001;

	enum STATUS{
		ONLINE = XTREAM_ONLINE,
		OFFLINE = XTREAM_OFFLINE,
		HIDE = XTREAM_HIDE,
		LEAVE = XTREAM_LEAVE,
		OTHER
	};
	enum CONN_FLAG{
		NONE = 0,	// 没有尝试连接
		TRY_LAN,	// 尝试内网连接中
		LAN,		// 内网连接成功
		LAN_FAILED,	// 内网连接失败
		TRY_P2P,	// 尝试点对点连接中
		P2P,		// 点对点连接成功
		P2P_FAILED,	// 点对点连接失败
		SERVER		// 服务器中转
	};
	uint32		ip           ;
	uint32		localIP		 ;
	uint16		port         ;
	uint16		localPort	 ;
	uint8		connFlag	 ; // 连接标志
	uint8		group_index  ;
	uint16      ver          ; //客户端版本号
	char		key[16]      ; //P2P之间的session key
	uint8		level        ; //用户等级
	uint8       equipment     ; //1:拥有视频头
	uint8       cluster_property ;//1 byte,2:Creator;1:Admin;0:Normal
	bool        cluster_member ;
	uint8       status       ;//在线状态
	char		web_location[100] ;
	bool        bRefreshed   ;//用户资料是否更新过
	bool        bClosePanel  ;//P2p聊天时，user panel是否关闭
	uint8		p2pRetry     ;// p2p连接的可用重试次数	
	bool		bBasicInfo	 ;// 基本资料是否有效
	uint8       isTalkZYF    ;//聊天时是否启动转义符(1,yes  0,no)
	uint8       webInfo      ;//他是否有最新文章或图片?0无,1图片only,2文章only,3 both图片文章
	
	bool        bInviting    ;//是否正在邀请中...

	TUserWebArticleInfo *pUserWebArticleInfo;
	TUserWebPictureInfo *pUserWebPictureInfo;

	uint32      reserved     ;
	FRIEND_INFO()
	{
		memset(this,0,sizeof(FRIEND_INFO)) ;
		ver  = XTREAM_CLIENT_VERSION       ;
		status = XTREAM_OFFLINE;
		bRefreshed  = false;
		bClosePanel = false;
		bBasicInfo = false;
        bInviting = false;
		isTalkZYF = 0;
		pUserWebArticleInfo = NULL;
		pUserWebPictureInfo = NULL;
		p2pRetry = 3;
	}
};

//老的用户信息，现定义这个结构体是为了兼容老版本
struct FRIEND_INFO_OLD : public XT_USER_INFO_OLD
{
	// 注意，在基类结构改变时也要更新版本号
	const static uint32 VER = 0x00000001;

	enum STATUS{
		ONLINE = XTREAM_ONLINE,
		OFFLINE = XTREAM_OFFLINE,
		HIDE = XTREAM_HIDE,
		LEAVE = XTREAM_LEAVE,
		OTHER
	};
	enum CONN_FLAG{
		NONE = 0,	// 没有尝试连接
		TRY_LAN,	// 尝试内网连接中
		LAN,		// 内网连接成功
		LAN_FAILED,	// 内网连接失败
		TRY_P2P,	// 尝试点对点连接中
		P2P,		// 点对点连接成功
		P2P_FAILED,	// 点对点连接失败
		SERVER		// 服务器中转
	};
	uint32		ip           ;
	uint32		localIP		 ;
	uint16		port         ;
	uint16		localPort	 ;
	uint8		connFlag	 ; // 连接标志
	uint8		group_index  ;
	uint16      ver          ; //客户端版本号
	char		key[16]      ; //P2P之间的session key
	uint8		level        ; //用户等级
	uint8       equipment     ; //1:拥有视频头
	uint8       cluster_property ;//1 byte,2:Creator;1:Admin;0:Normal
	bool        cluster_member ;
	uint8       status       ;//在线状态
	char		web_location[100] ;
	bool        bRefreshed   ;//用户资料是否更新过
	bool        bClosePanel  ;//P2p聊天时，user panel是否关闭
	uint8		p2pRetry     ;// p2p连接的可用重试次数	
	bool		bBasicInfo	 ;// 基本资料是否有效
	uint8       isTalkZYF    ;//聊天时是否启动转义符(1,yes  0,no)
	uint8       webInfo      ;//他是否有最新文章或图片?0无,1图片only,2文章only,3 both图片文章

	bool        bInviting    ;//是否正在邀请中...

	TUserWebArticleInfo *pUserWebArticleInfo;
	TUserWebPictureInfo *pUserWebPictureInfo;

	uint32      reserved     ;
	FRIEND_INFO_OLD()
	{
		memset(this,0,sizeof(FRIEND_INFO_OLD)) ;
		ver  = XTREAM_CLIENT_VERSION       ;
		bRefreshed  = false;
		bClosePanel = false;
		bBasicInfo = false;
		bInviting = false;
		isTalkZYF = 0;
		pUserWebArticleInfo = NULL;
		pUserWebPictureInfo = NULL;
		p2pRetry = 3;
	}
};

//------------------------------------------------------
// 好友数组类
//------------------------------------------------------

struct FRIENDP_ARR:public vector<FRIEND_INFO*>
{
	~FRIENDP_ARR()
	{
		clear();
	};
	
	void clear()
	{
		size_t i,c;
		c = size();
		for( i = 0; i < c; i++ )
		{
			if(operator [](i) != NULL)
				delete operator [] (i); 
		}
		__super::clear();
	}

	void operator = (const FRIENDP_ARR & arrFriend)
	{
		size_t i,c;
		clear();
		c = arrFriend.size();
		for( i = 0; i < c; i++)
		{
			ASSERT(arrFriend[i]);
			__super::push_back(new FRIEND_INFO(*arrFriend[i]));
		}
	};

	void push_back(const FRIEND_INFO & friendInfo)
	{
		FRIEND_INFO* pFriendInfo = new FRIEND_INFO(friendInfo);
		__super::push_back(pFriendInfo);
	}

protected:

	void push_back(const FRIEND_INFO * pFriendInfo);

	void push_back(FRIEND_INFO * pFriendInfo);

};

typedef struct SYSTEM_MSG
{
	enum MSG_TYPE{
		SYSMSG_FRINED = 0,
		SYSMSG_CLUSTER,
		SYSMSG_MOUMOU,
		SYSMSG_TRADE,
		SYSMSG_GOODS,
		SYSMSG_RETURN
	};
	const static uint32 VER = 0x00000001;
	int		iType;
	DWORD	dwTime	;
	char    szMessage[512];

	SYSTEM_MSG()
	{
		memset(this,0,sizeof(SYSTEM_MSG));
	}

} SYSTEM_MSG, *LPSYSTEM_MSG;

typedef CList<SYSTEM_MSG,SYSTEM_MSG> SYSTEM_MSG_LIST;//系统消息

typedef struct CLUSTER_SYSTEM_MSG
{
	const static uint32 VER = 0x00000001;
	int		iType;
	DWORD	dwTime	;
	char    szMessage[512];
	int     nInviteId;
	char    szNickName[MAX_NICKNAME_LEN+1];
	int     nClusterId;
	char    szClusterName[255];
	CLUSTER_SYSTEM_MSG()
	{
		memset(this,0,sizeof(CLUSTER_SYSTEM_MSG));
	}

} CLUSTER_SYSTEM_MSG, *LPCLUSTER_SYSTEM_MSG;

typedef CList<CLUSTER_SYSTEM_MSG,CLUSTER_SYSTEM_MSG> CLUSTER_SYSTEM_MSG_LIST;//群系统消息，用于查看群信息和好友信息

struct USER_INFO : public FRIEND_INFO
{
	bool bAutoLogin; //是否自动登陆
	bool bHideLogin; //登陆初始状态是否为隐身
	BOOL bShowHint ; //是否允许提示信息
	int  iLoginFlag; //登陆初始状态
	BOOL bRemeberPsw;//是否记住密码
	uint32  merchant_id;
	BOOL bProfileModified;////是否修改群名片
};

//用户全局信息
extern USER_INFO     g_ImUserInfo;
struct CLUSTER_INFO : public XT_CLUSTER_INFO
{	
	const static uint32 VER = 0x00000001;

	bool bAutoShowMagic;
	bool bRefused      ;	//是否被阻止
	bool bRefreshFlag  ;
	bool bMemberInit   ;	//成员是否初始化
	bool bBasicInfo;		//基本群信息是否有效 
	uint8 radio;
	enum{RECV_SHOW = 1,RECV_NOSHOW = 2,NORECV = 3,SHOW_NUM = 4};
	uint32 nMsgNum;			// 未读消息数（当群消息屏蔽类型选择只显示数量时有效）

	DWORD dwTickGet    ;
	vector<FRIEND_INFO>  members;
	DWORD dwClusterPageShow    ;    //暂时用于显示1,2,3,4的页面判断;
	CLUSTER_INFO()
	{
		//memset(members,0,sizeof(members));
		//count=0;
		bAutoShowMagic = true;
		bRefreshFlag   = false;
		bRefused       = false;
		bMemberInit    = false;
		bBasicInfo	   = false;
		dwTickGet      = 0;
		dwClusterPageShow=0;
		radio = RECV_SHOW;
		nMsgNum = 0;
	}

	size_t Count()
	{ return members.size(); };

	int FindMember(uint32 fid)
	{
		size_t i,c;
		c = members.size();
		for(i = 0; i < c; i++)
		{
			if(members[i].GetId() == fid)
				return i;
		}
		return -1;
	}
	
	void DelMember(uint32 fid)
	{
		int nId = -1;
		nId = FindMember(fid);
		if ( nId != -1 )
		{
			members.erase(members.begin() + nId);
		}
	}

	FRIEND_INFO * GetCreator()
	{
		size_t i,c;
		c = members.size();

		if ( nCreatorID==g_ImUserInfo.GetId() )
			return &g_ImUserInfo;

		if ( c>0 )
		{
			for ( i = 0; i < c; i++)
			{
				if ( members[i].cluster_property==2 )
				{
					return &members[i];
				}
			}
		}

		int n= FindMember(this->nCreatorID);
		if ( n!=-1 )
			return &members[n];
		else
			return NULL;
	}

	FRIEND_INFO * GetFriendInfo(uint32 fid)
	{
		size_t i,c;
		c = members.size();
		for ( i = 0; i < c; i++)
		{
			if ( members[i].GetId()==fid )
			{
				return &members[i];
			}
		}
		return NULL;
	}

	//判断群成员属性（2:Creator;1:Admin;0:Normal)
	int GetMemberType(uint32 mid)
	{
		size_t i,c; 
		c = members.size();
		for ( i = 0; i < c; i++ )
		{
			if ( members[i].GetId()==mid )
			{
				return members[i].cluster_property;
			}
		}
		return -1;
	}
};

struct IMAGE_FILE_STRUCT
{
	char    file_name[255]		;
	uint32  file_session_id     ;
	uint32  file_size           ;
	uint32  image_type          ;//0 image, 1 custom emotion,2 web emotion

	IMAGE_FILE_STRUCT()
	{
		memset(this,0,sizeof(IMAGE_FILE_STRUCT));
	}
};

struct ContactGroupInfo//联系人组信息
{
	struct tagItem
	{
		int  iIndex     ;//组id
		char szName[50] ;//组名称
	};

	void Delete(int iIndex)
	{
		int iDel=-1;

		for ( int i=0; i<count; i++ )
		{
			if ( items[i].iIndex==iIndex )
			{
				iDel=i;
				break;
			}
		}

		if ( iDel!=-1 )
		{
			for ( int i=iDel; i<(count-1); i++ )
			{
				items[i]=items[i+1];
			}

			count--;
		}
	}

	char * GetGroupName(int nGroupId)
	{
		for ( int i=0; i<count; i++ )
		{
			if ( items[i].iIndex==nGroupId )
				return items[i].szName;
		}

		return NULL;
	}

	int GetGroupIndex(CString strName)
	{
		for ( int i=0; i<count; i++ )
		{
			CString strTemp = items[i].szName;
			strTemp.Trim();
			strName.Trim();
			if ( strName==strTemp )
			{
				return items[i].iIndex;
			}
		}

		return -1;
	}

	int MaxIndex()
	{
		int temp=-1;
		for ( int i=0; i<count; i++ )
		{
			if ( items[i].iIndex!=MAX_INT )
			{
				if ( items[i].iIndex>temp )
					temp=items[i].iIndex;
			}
		}

		return temp;
	}

	int Add(CString strName)
	{
		int iIndex = MaxIndex()+1;
		items[count].iIndex = iIndex;
		strcpy(items[count].szName,(LPCSTR)strName);
		count++;
		return iIndex;
	}
	ContactGroupInfo()
	{
		count = 5;

		memset(items,0,sizeof(items));

		items[0].iIndex=0;
		sprintf(items[0].szName,"朋友");

		items[1].iIndex=1;
		sprintf(items[1].szName,"亲戚");

		items[2].iIndex=2;
		sprintf(items[2].szName,"同学");

		items[3].iIndex=3;
		sprintf(items[3].szName,"同事");

		items[4].iIndex=MAX_INT;
		sprintf(items[4].szName,"其他");
	}

	void Sort()
	{
		for ( int i=1; i<count; i++ )
		{
			for ( int j=0; j<i; j++ )
			{
				if ( items[i].iIndex<items[j].iIndex )
				{
					tagItem temp = items[i];
					items[i] = items[j];
					items[j] = temp;
				}
			}
		}
	}

public:
	int		  count;
	tagItem   items[50];

};

struct ContactPersonInfo //联系人信息
{
	CString			strName;
	CString			strCompany;
	CString			strCareer;
	CString			strAddress;
	CString			strEmail;
	CString			strPhone;
	CString			strMSNNum;
	CString			strBlog;
	CString			strNote;
	CString			strQQNum;
	CString			strSchool;
	CString			strBirthday;
	int				nInvited;//是否发送过邀请1是，0否
	int             nSex;
	int				nType; //0,mini139 user; 1 windows address book contact 
	int				nGroupIndex;//对应于ContactGroupInfo 的组id

	ContactPersonInfo()
	{
		nSex  = 0;
		nType = 0;
		nInvited = 0;
		nGroupIndex = 0;
	}
};


struct LocalSavedArticleInfo
{
	DWORD      dwArticleID	;//0 new , other edit
	DWORD      dwViewType	;//访问权限  0－自己，1－好友，2－注册用户，3－所有
	DWORD      dwTopFlag	;
	DWORD      dwDirID		;
	CString    strTitle		;
	CString    strContent   ;
	CString    strCreateDate;
	CString    strTag		;
	CString    strDirName   ;
};

typedef list<ContactPersonInfo *>   ADDRESS_BOOK_LIST;//联系人地址列表

typedef list<IMAGE_FILE_STRUCT>   IMAGE_FILE_LIST;

typedef list<FRIEND_INFO *>       FRIEND_LIST;


#define GIF_EMOTION_COUNT           55


extern  int	MAX_USER_HEAD_COUNT		;

//聊天框字体设置
extern XT_FONT		g_ImFont		 ;
extern CImageList	g_imgEmotionList ;
extern CImageList      g_imgStatus24;
extern CImageList      g_imgStatus30;
extern CImageList      g_imgStatus35;
extern CImageList      g_imgStatus47;
extern CImageList      g_imgStatus52;
extern CImageList      g_imgGroupStatus30;
extern CImageList      g_imgGroupStatus35;
extern CImageList   g_imgAppIconXp   ;
extern CImageList   g_imgAppIcon2k   ;
extern CWnd		  * g_pDesktopWindow ;

extern bool         g_bOs2K          ;// 是否是2K或2K以下的系统

extern HWND			g_hMessageWnd    ;
extern HWND         g_hMainWnd       ;

//extern map<int,CString>  g_mapFamilyFace ;//map<family id, face url>

#define KEYDOWN(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000) ? TRUE : FALSE )

extern FRIEND_LIST   g_lsWebVounchFriends;//网站推荐好友列表
extern FRIEND_LIST   g_lsFriend		 ;//由XTreamMainWnd维护，好友列表
extern FRIEND_LIST	 g_lsFriendTemp	 ;// 临时好友信息	
extern GROUP_INFO	 g_GroupInfo	 ;//由XTreamMainWnd维护，好友分组信息
extern vector<CLUSTER_INFO *> g_vecCluster ;
extern vector<CLUSTER_INFO *> g_vecClusterExit ;
extern HANDLE        g_hUserMutex    ;//本机用户同一帐户登陆互斥
extern HBRUSH        g_hBackBrush    ;
extern HBRUSH        g_hWhiteBrush   ;

extern CString       g_strUpdateInfo ;
extern CString       g_strDefaultExplorePath;


extern map<int,CString>    g_mapVideoDriverIndex;//视频头驱动
extern map<uint16,uint16>  g_mapUserOper ;//用户行为统计

extern IWebConfigXPtr    g_pWebConfig;
extern IWriteArticleXPtr g_pWriteArticle;
extern IMyPicturesXPtr   g_pMyPictures;
extern IMyArticlesXPtr   g_pMyArticles;
extern IHeadPhotoXPtr    g_pHeadPhoto ;
//extern IIMiniPictureBroadCastXPtr g_pMiniPictureBroadCast;

extern CSpaceData		 g_spaceData;

CString		SexToStr(unsigned char sex);	//性别转换成字符串描述
CString		StatusToStr(int status);		//状态转换成字符串描述
COLORREF	StatusToColor(int status);		//状态转换成相应的文字显示颜色

CString		UserInfoToStr(XT_USER_INFO *);    //用户信息转换成字符串描述
/*用字符串描述一个好友信息(id,name,...)*/
CString		UserInfoToStrEx(XT_USER_INFO *user);
CString		CMemberInfoToStr(XT_USER_INFO *); //群成员信息转换成字符串描述
CString     CountInfoToStr(XT_USER_INFO *user);    //群成员账号描述

bool		WebLogin(char * username, char * password);
void		ShowWebRegisterPage();
void		ShowAlbum(char *username);
void		ShowBlogSite(char *username);
void		ShowArticle(char *username);
void		ShowBlogFamilySite(char *clusterName);
void		MiniOpenUrl(const CString & sUrl);
CString		GetMiniUrl(const CString & strUrl);


void		IM_LOG(CString str);

CLUSTER_INFO * FindClusterInfo(uint32 cid);
void		   ClearClusterList();

void           ClearClusterExitList();

FRIEND_INFO *	FindFriend(uint32 fid); 
// 根据好友id来从本地已有的资料中查询
FRIEND_INFO *	GetFriendInfo(uint32 fid);
void		   ClearFriendList();
void		   ClearList(FRIEND_LIST &l);

int			   GetDefaultImage(int sex,int status);



//显示好友上线，好友聊天
void			ShowUserHintDlg(FRIEND_INFO *pFriend,CString strTitle, CString strHint, HWND hCmdWnd);
void			ShowUserHintDlg(CString strHint);
void			ShowClusterHintDlg(CLUSTER_INFO*pCluster,CString sTitle,CString sHint,HWND hCmdWnd);
void			ShowClusterAgreeDlg(CLUSTER_INFO*pCluster,FRIEND_INFO* pFriend,CString sTitle,CString sHint,HWND hCmdWnd);

bool		   FindForbiddenWords(CString str);

void		SaveActiveWindow();
void		RestoreActiveWindow();

CBitmap *	GetFaceBmpFromFile(const CString & sImageFile);

void		ClearUserBitmap();
void		UpdateUserBitmap(FRIEND_INFO * pFriend);
bool		MakeUserBitmap(FRIEND_INFO * pFriend);
CBitmap *	GetUserBitmap(FRIEND_INFO * pFriend, bool bSmall);

void		ClearClusterBitmap();
void		UpdateClusterBitmap(CLUSTER_INFO *pCluster);
bool		MakeClusterBitmap(CLUSTER_INFO *pCluster);
CBitmap *	GetClusterBitmap(CLUSTER_INFO * pCluster, bool bSmall);

BOOL        DownloadMini139(CString strVersionUrl, CString & strDownloadFile/*[out]返回下载后的本地文件*/);

void ShowMessage(CString strMessage,UINT nType=MB_OK|MB_ICONINFORMATION);

INT CALLBACK EnumFontNameProc(LOGFONT *plf, TEXTMETRIC* /*ptm*/, INT /*nFontType*/, LPARAM lParam);

// 获得汉字的首个拼音字母,普通ascii码直接返回,没有找到则返回0
char		GetFirstPinyin(const char* szGb2312);

// 解析出群聊天信息中的包含的动作信息
// 动作定义表达式{\action0[]action1[\object]action2\}
BOOL		ParseMsg(const char* szMsg,
			  CString& sMsg,
			  CString& sObj,
			  CString& sAction0,
			  CString& sAction1,
			  CString& sAction2);

bool ValidateString(const CString &s, CString &error );

// 去掉字符串首尾的全角空白字符
void TrimSpace2(CString& sSrc);

void GetMD5FileName(const CString& sFilePathName,CString& sMD5Name);

void GetMD5WebFileName(const CString & sUrl,CString & sMD5Name);

//记录一下用户操作
void        RegUserOperation(uint16 operId);

void		ClearAddressBookList(ADDRESS_BOOK_LIST &l);

void		WriteArticlePreviewFile(const TWebArticleInfo &info, CString strAuthor, CString strLocalFile);


void	ShowUploadPicDlg(char *username, char *nickname, char *password, uint32 uid );
void    CloseUploadPicDlg();


void  ClearWebVounchedFriends();//清除web推荐好友列表
FRIEND_INFO * FindWebVounchedFriend(uint32 fid);//

bool compareP(const TWebPictureInfo * info1, const TWebPictureInfo * info2 );

CString WebDataFrom2Str(int iFrom);
CString WebViewProp2Str(int iViewProp);//查看权限


void   SaveUserWebData();

void NotifyVideoEquipment(FRIEND_INFO * pFriend);

bool LoadImageFromResource(IN CImage* pImage,IN UINT nResID, IN LPCSTR lpTyp);

bool ImageFromIDResource( Image * & pImg, UINT nID, LPCTSTR sTR = _T("PNG")) ;

#ifdef _BUSINESS_VESION
// 判断组ID是否属于我的团队分组
BOOL CheckSubGroupIsInMyTeam(int nGroupIndex);
#endif

// 根据好友用户名查找节点
FRIEND_INFO * FindFriend(const char *szUserName);

// 获取陌生人数量
int GetStrangerCount();

const char* SubstrExist(const char* s,int nLen,const char* szSub);

// 自动接待
void AutoAccept(uint32 friendId);
#ifdef _PRESS_TEST
void AutoAcceptPress(XT_MSG stMsg);
#endif

void InitClusterType(CComboBox *pComboBox );
DWORD GetClusterType(CComboBox *pComboBox);
LPCSTR GetClusterClass(int nClass);
void UpdateUserFullInfo(XT_USER_INFO* pUserInfo, XT_USER_INFO* ack );
int CountLevel(int nTime,int&nNextDelta);
CString GetPicRandomUrl();
#endif
