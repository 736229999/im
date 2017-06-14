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
**���ڶ��� IM ģ�����õ�ȫ�ֱ���
**������
**
*********************************/

#define SERVER_ACK_TIMEOUT			5000   // ���ĵȴ�ack��ʱ��20��
#define P2P_ACK_TIMEOUT			    5000

#define XTREAM_SERVER_PORT			PORT_IM_BUSINESS
//#define XTREAM_LOCAL_PORT			3330
#define REG_ACCOUNT_MAX				100
//Leading 7 bytes of Data package

#define HELPER_ID                   260953

//���嶨��
#define XTF_ITALIC				0x01
#define XTF_UNDERLINE			0x02
#define XTF_BOLD				0x04

//�ַ����
#define MAX_STR_LEN				255
#define MAX_INT					65535

#define IM_SPACE_COLOR			RGB(0x4d,0x95,0xc2)
#define IM_LINE_COLOR			RGB(255,255,255)

#ifdef _CS_PLAT_EX_   ////��ţ�ͷ�����̨
   #define IM_CAPTION           "��ţ�ͷ�����̨"
#else
   #define IM_CAPTION			"����"
#endif

#define IDS_NOPASS				"�Բ����������������ʾ���Ϸ�����Ӧ���ַ�A-Z��a-z��0-9���»�����ɣ�����Ϊ6-15Bytes��"
#define IDS_WRONGNO				"�����������û�����"

#define MAX_CLUSTER_MEMBERS      300//���Ⱥ��Ա����

#define HEAD_SZ					 35//ͷ��Icon�ߴ�

#define FILE_PACKET_LEN			 1240 // ͼƬ�ļ�������������ֽ���

//ͼƬ��ӳ����ͼ�ߴ�
#define PICTURE_CX				80
#define PICTURE_CY				80
#define FRAME_SZ				85
#define PICTURE_SPACE			10


//api �ӿ�



#if defined _VER_PREVIEW_
	#define BUYER_LOGIN_API 		"stage-member-api01.idc1.fn"								/*��ҵ�¼api*/
	#define SELLER_LOGIN_API		"zhaoshang-api.idc1.fn"										/*���ҵ�¼api*/
	#define MERCHANT_API			"http://zhaoshang-api.idc1.fn/api/call.do"              /*��ȡ�̼�ID�Ľӿ�*/

	#define BUYER_REGISTER_URL		"https://member.feiniu.com/getaway/register"				/*���ע��*/
	#define SELLER_REGISTER_URL 	"http://sjmember.feiniu.com/static/html/register.html"		/*����ע��*/

	#define BUYER_FORGET_PSW_URL	"https://member.feiniu.com/securityView/lostPwd"			/*�����������*/
	#define SELLER_FORGET_PSW_URL	"http://sjmember.feiniu.com/static/html/wjml.html"			/*������������*/
	#define WORK_FORGET_PSW_URL		"http://backend.idc1.fn/index.jsp"						    /*������Ա�޸�����*/

	#define HEADICON_URL			"http://merchant.feiniu.com/webim/static/images/moumoupc/"	/*ϵͳͷ��*/
	#define PICTURE_URL_OLD         ".fn-mart.com/"                                             /*��ͼƬ�ռ��ַ*/	
	#define PICTURE_URL             "http://img16.fn-mart.com"                    /*ͼƬ�ռ�*/
	#define PICTURE_URL2            "http://img17.fn-mart.com"                    /*ͼƬ�ռ�*/
	#define PICTURE_URL3            "http://img18.fn-mart.com"                    /*ͼƬ�ռ�*/
	#define RIGHT_URL				"http://merchant.feiniu.com/webim/right.html"				/*�Ҳര��*/

	#define UPLOAD_DUMP_URL			"http://imindex.idc1.fn/dump-tracker/upload-for-pc-merchant.do" /*dump�ļ��ϴ���ַ*/

	#define ORDER_TRADE_URL          "http://merchant.feiniu.com/order/detail/orderDetail.do?" /*�¶������Ѹ���ĵ�ַ*/
	#define ORDER_COMMENT_URL        "http://merchant.beta.fn.com/portal/index.do#/s5_6"       /*���۹���ĵ�ַ*/
	#define ORDER_RETURN_URL         "http://merchant.feiniu.com/order/orderReturn/order_refund.do?"        /*�˻�������ĵ�ַ*/
#ifdef _CS_PLAT_EX_
	#define RIGHT_URL_SELLER		"http://webim-cs.feiniu.com/webRight/service-log.htm?"      /*Preview�ͷ�����̨�ͷ�������Ҳ�ҳ*/
	#define EMPLOYEE_RIGHT_URL	    "http://webim-cs.feiniu.com/serviceProfile/right_profile.html?" /*Preview�����ͷ�����̨�ͷ��Կͷ��Ҳ�ҳ*/
	#define QUICK_PHRASE_URL	    "http://webim-cs.feiniu.com/serviceProfile/quick-phrase?" /*Preview������ȡ��ݶ����ַ*/
#elif defined _WORK_EX_
	#define RIGHT_URL_WORK			"http://merchant.feiniu.com/webim/right_in.html"          /*�������Ҳര�ڶ����ǹ�����Ա*/
	#define RIGHT_URL_SELLER		"http://merchant.feiniu.com/webim/right_bm.html"          /*�������Ҳര�ڶ������̼�*/
#else
	#define RIGHT_URL_WORK			"http://merchant.feiniu.com/webim/right_in.html"          /*�̼Ұ��Ҳര�ڶ����ǹ�����Ա*/
	#define RIGHT_URL_SELLER		"http://merchant.feiniu.com/webim/seller_right.html"      /*���Ұ��Ҳര��*/
#endif
    #define WEBIM_URL               "merchant.feiniu.com"                                       /*Webim����ַ*/
	#define COOKIE_URL              ".feiniu.com"                                               /*����cookie�ĸ���ַ*/
    #define WEBIM_LINKINFO_STORE    "webim/vender?VENDER_INFO_TYPE=GET_STORE_INFO&merchant_id=" /*��ȡ������Ϣ�ӿ�*/
    #define WEBIM_LINKINFO_ITEM     "webim/vender?VENDER_INFO_TYPE=GET_PRODUCT_INFO&product_id="/*��ȡ��Ʒ��Ϣ�ӿ�*/
	#define STORE_URL               "http://store.feiniu.com/index-"                            /*���̵�ַ*/
	#define ITEM_URL                "http://item.feiniu.com/"                                   /*��Ʒ��ַ*/

	#define SERVER_IP				/*"10.201.201.24"*/"10.212.129.181"							/*������ip*/
	#define	SERVER_PORT             10002														/*������ip*/

	#define LOAD_PIC_URL			"http://merchant.feiniu.com/webim/upload-photo.do?isFromIM=1"	/*�ϴ�ͼƬ��ͼƬ�ռ�ĵ�ַ*/

	#define RESET_PWD_URL			"https://sjmember.feiniu.com/static/html/wjml.html?backUrl=http%3A%2F%2Fmerchant.feiniu.com%2Fportal%2Findex.do"			/*��������*/
	#define BIND_MOBIL_URL			"https://member.beta.fn.com/static/html/accountSet/checkMobile.html"	/*���ֻ�*/
	
	#define PROXY_URL               "proxy2.fn.com:8080"                                          /*preview�̼Һ�̨����Ӫ��̨�Ĵ���*/
#elif defined _VER_ONLINE_
	#define BUYER_LOGIN_API 		"stage-member-api01.idc1.fn"								/*��ҵ�¼api*/
	#define SELLER_LOGIN_API		"zhaoshang-api.idc1.fn"										/*���ҵ�¼api*/
	#define MERCHANT_API			"http://zhaoshang-api.idc1.fn/api/call.do"              /*��ȡ�̼�ID�Ľӿ�*/

	#define BUYER_REGISTER_URL		"https://member.feiniu.com/getaway/register"				/*���ע��*/
	#define SELLER_REGISTER_URL 	"http://sjmember.feiniu.com/static/html/register.html"		/*����ע��*/

	#define BUYER_FORGET_PSW_URL	"https://member.feiniu.com/securityView/lostPwd"			/*�����������*/
	#define SELLER_FORGET_PSW_URL	"http://sjmember.feiniu.com/static/html/wjml.html"			/*������������*/
	#define WORK_FORGET_PSW_URL		"http://backend.idc1.fn/index.jsp"						    /*������Ա�޸�����*/

	#define HEADICON_URL			"http://merchant.feiniu.com/webim/static/images/moumoupc/"	/*ϵͳͷ��*/
	#define PICTURE_URL_OLD         ".fn-mart.com/"                                             /*��ͼƬ�ռ��ַ*/
	#define PICTURE_URL             "http://img16.fn-mart.com"                    /*ͼƬ�ռ�*/
	#define PICTURE_URL2            "http://img17.fn-mart.com"                    /*ͼƬ�ռ�*/
	#define PICTURE_URL3            "http://img18.fn-mart.com"                    /*ͼƬ�ռ�*/
	#define RIGHT_URL				"http://merchant.feiniu.com/webim/right.html"				/*�Ҳര��*/	

	#define UPLOAD_DUMP_URL			"http://im.feiniu.com/dump-tracker/upload-for-pc-merchant.do" /*dump�ļ��ϴ���ַ*/

	#define ORDER_TRADE_URL          "http://merchant.feiniu.com/order/detail/orderDetail.do?" /*�¶������Ѹ���ĵ�ַ*/
	#define ORDER_COMMENT_URL        "http://merchant.beta.fn.com/portal/index.do#/s5_6"       /*���۹���ĵ�ַ*/
	#define ORDER_RETURN_URL         "http://merchant.feiniu.com/order/orderReturn/order_refund.do?"        /*�˻�������ĵ�ַ*/
#ifdef _CS_PLAT_EX_
	#define RIGHT_URL_SELLER		"http://webim-cs.feiniu.com/webRight/service-log.htm?"      /*Online�ͷ�����̨�ͷ�������Ҳ�ҳ*/
	#define EMPLOYEE_RIGHT_URL	    "http://webim-cs.feiniu.com/serviceProfile/right_profile.html?" /*Online�����ͷ�����̨�ͷ��Կͷ��Ҳ�ҳ*/
	#define QUICK_PHRASE_URL	    "http://webim-cs.feiniu.com/serviceProfile/quick-phrase?" /*Online������ȡ��ݶ����ַ*/
#elif defined _WORK_EX_
	#define RIGHT_URL_WORK			"http://merchant.feiniu.com/webim/right_in.html"          /*�������Ҳര�ڶ����ǹ�����Ա*/
	#define RIGHT_URL_SELLER		"http://merchant.feiniu.com/webim/right_bm.html"          /*�������Ҳര�ڶ������̼�*/
#else
	#define RIGHT_URL_WORK			"http://merchant.feiniu.com/webim/right_in.html"          /*�̼Ұ��Ҳര�ڶ����ǹ�����Ա*/
	#define RIGHT_URL_SELLER		"http://merchant.feiniu.com/webim/seller_right.html"          /*���Ұ��Ҳര��*/
#endif
    #define WEBIM_URL               "merchant.feiniu.com"                                       /*Webim����ַ*/
	#define COOKIE_URL              ".feiniu.com"                                               /*����cookie�ĸ���ַ*/
    #define WEBIM_LINKINFO_STORE    "webim/vender?VENDER_INFO_TYPE=GET_STORE_INFO&merchant_id=" /*��ȡ������Ϣ�ӿ�*/
    #define WEBIM_LINKINFO_ITEM     "webim/vender?VENDER_INFO_TYPE=GET_PRODUCT_INFO&product_id="/*��ȡ��Ʒ��Ϣ�ӿ�*/
	#define STORE_URL               "http://store.feiniu.com/index-"                            /*���̵�ַ*/
	#define ITEM_URL                "http://item.feiniu.com/"                                   /*��Ʒ��ַ*/

	#define SERVER_IP				"221.133.245.172"										    /*������ip*/
	#define	SERVER_PORT             10002														/*�������˿�*/

	#define LOAD_PIC_URL			"http://merchant.feiniu.com/webim/upload-photo.do?isFromIM=1"	/*�ϴ�ͼƬ��ͼƬ�ռ�ĵ�ַ*/

	#define RESET_PWD_URL			"https://sjmember.feiniu.com/static/html/wjml.html?backUrl=http%3A%2F%2Fmerchant.feiniu.com%2Fportal%2Findex.do"			/*��������*/
	#define BIND_MOBIL_URL			"https://member.beta.fn.com/static/html/accountSet/checkMobile.html"	/*���ֻ�*/

	#define PROXY_URL               "proxy-api.fn.com:8080"                                      /*online�̼Һ�̨����Ӫ��̨�Ĵ���*/
#else  /*beta����*/
	#define BUYER_LOGIN_API			"member-api.beta1.fn"										/*��ҵ�¼api*/
	#define SELLER_LOGIN_API		"zhaoshang-api.beta.fn.com"									/*���ҵ�¼api*/
	#define MERCHANT_API			"http://zhaoshang-api.beta.fn.com/api/call.do"              /*��ȡ�̼�ID�Ľӿ�*/
	
	#define BUYER_REGISTER_URL  	"https://member.beta1.fn/sh/getaway/register"				/*���ע��*/
	#define SELLER_REGISTER_URL 	"http://member.beta.fn.com/static/html/register.html"		/*����ע��*/
	
	#define BUYER_FORGET_PSW_URL   	"https://member.beta1.fn/sh/securityView/lostPwd"			/*�����������*/
	#define SELLER_FORGET_PSW_URL   "http://member.beta.fn.com/static/html/wjml.html"			/*������������*/
	#define WORK_FORGET_PSW_URL		"http://backend.beta.fn.com/index.jsp"						/*������Ա�޸�����*/

	#define HEADICON_URL			"http://webim.beta.fn.com/webim/static/images/moumoupc/"	/*ϵͳͷ��*/	
	#define PICTURE_URL_OLD		    "http://wh-image01.fn.com/"                                 /*�̳�ͼƬ�ռ�*/     
	#define PICTURE_URL_OLD2		"http://img.beta1.fn/"                                      /*��ӪͼƬ�ռ�*/
	#define PICTURE_URL             "http://10.202.249.218"                       /*ͼƬ�ռ�*/
	#define RIGHT_URL				"http://webim.beta.fn.com/webim/right.html"					/*�Ҳര��*/

	#define UPLOAD_DUMP_URL          "http://messagecenter.beta.fn.com/imindex/dump-tracker/upload-for-pc-merchant.do" /*dump�ļ��ϴ���ַ*/

	#define ORDER_TRADE_URL          "http://order.beta.fn.com/detail/orderDetail.do?" /*�¶������Ѹ���ĵ�ַ*/
	#define ORDER_COMMENT_URL        "http://merchant.beta.fn.com/portal/index.do#/s5_6"       /*���۹���ĵ�ַ*/
	#define ORDER_RETURN_URL         "http://order.beta.fn.com/orderReturn/order_refund.do?"        /*�˻�������ĵ�ַ*/

#ifdef _CS_PLAT_EX_
    #define RIGHT_URL_SELLER		"http://webim.beta1.fn/webim-cs/webRight/service-log.htm?"      /*BETA�ͷ�����̨�ͷ�������Ҳ�ҳ*/
    #define EMPLOYEE_RIGHT_URL	    "http://webim.beta1.fn/webim-cs/serviceProfile/right_profile.html?" /*BETA�����ͷ�����̨�ͷ��Կͷ��Ҳ�ҳ*/
	#define QUICK_PHRASE_URL	    "http://webim.beta1.fn/webim-cs/serviceProfile/quick-phrase?" /*BETA������ȡ��ݶ����ַ*/
#elif defined _WORK_EX_
	#define RIGHT_URL_WORK			"http://webim.beta1.fn/webim/right_in.html"          /*beta�������Ҳര�ڶ����ǹ�����Ա*/
	#define RIGHT_URL_SELLER		"http://webim.beta1.fn/webim/right_bm.html"          /*beta�������Ҳര�ڶ������̼�*/
#else
	#define RIGHT_URL_WORK			"http://webim.beta1.fn/webim/right_in.html"          /*beta�̼Ұ��Ҳര�ڶ����ǹ�����Ա*/
	#define RIGHT_URL_SELLER		"http://webim.beta1.fn/webim/seller_right.html"          /*���Ұ��Ҳര��*/
#endif
	//#define RIGHT_URL_SELLER		"10.211.240.128/webim/seller_right.html"
    #define WEBIM_URL               "webim.beta1.fn"                                            /*Webim����ַ*/
	#define COOKIE_URL              ".beta1.fn"                                               /*����cookie�ĸ���ַ*/
    #define WEBIM_LINKINFO_STORE    "webim/vender?VENDER_INFO_TYPE=GET_STORE_INFO&merchant_id=" /*��ȡ������Ϣ�ӿ�*/
    #define WEBIM_LINKINFO_ITEM     "webim/vender?VENDER_INFO_TYPE=GET_PRODUCT_INFO&product_id="/*��ȡ��Ʒ��Ϣ�ӿ�*/
    #define STORE_URL               "http://storefront.beta1.fn/index-"                         /*���̵�ַ*/
    #define ITEM_URL                "http://item.beta1.fn/"                                     /*��Ʒ��ַ*/

	#define SERVER_IP				"10.211.64.73" 												/*������ip*/
	#define	SERVER_PORT             10002														/*�������˿�*/

	#define LOAD_PIC_URL			"http://webim.beta1.fn/webim/upload-photo.do?isFromIM=1"	/*�ϴ�ͼƬ��ͼƬ�ռ�ĵ�ַ*/

	#define RESET_PWD_URL			"https://member.beta.fn.com/static/html/wjml.html"			/*��������*/	
	#define BIND_MOBIL_URL			"https://member.beta.fn.com/static/html/accountSet/checkMobile.html"	/*���ֻ�*/
#endif

#ifdef _VER_INTERNAL_
#define CLIENT_NAME_BUYER			"�����ڲ���2015"
#else
#define CLIENT_NAME_BUYER			"����2015"
#endif

#define CLIENT_NAME_SELLER			"�����̼Ұ�2015"
#define FEINIU_CS_PLAT			    "��ţ�ͷ�����̨"
#define CLIENT_NAME_WORK			"���蹤����2016"
//��ǰ��½�û�״̬,��Ϣ


enum DOWNLOAD_STATE{
	DS_DOWNLOADING = 0,
	DS_SUCCESS,
	DS_FAILED 
};
struct DOWNLOAD_OBJ{
	CString	sUrl;	// �������ļ�
	CString	sSaved;	// �����ļ������ַ
	HANDLE	handle;	// �߳̾��
	UINT	timer;	// �߳�����ʱ��
	UINT	state;	// 0.��������;1.���سɹ�;2.����ʧ��
};

// ��ȡ����ϵͳ������
std::vector<std::string>* GetAllSysFontName();

// ���ҵ��Ŷӡ�groupID
#define  MY_TEAM_GROUP_ID		250
// �ڲ���Ϳͷ�����̨�汾
#if defined _VER_PREVIEW_
	#define  FEINIU_GROUP_ID		136
#elif defined _VER_ONLINE_
	#define  FEINIU_GROUP_ID		136
#else
	#define  FEINIU_GROUP_ID		177
#endif


//FIFOģ����
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
	//��������
	char fontName[MAX_STR_LEN];

	//�����С
	int  fontSize;

	//���
	BYTE flags; 

	//��ɫ
	COLORREF fontColor;

	XT_FONT()
	{
		sprintf(fontName ,"����");
		fontSize  = 10;
		vector<string> *pvecFontName = NULL;
		pvecFontName = GetAllSysFontName();
		vector<string>::iterator it = ::find(pvecFontName->begin(), pvecFontName->end(), "΢���ź�");
		if(it != pvecFontName->end())
		{
			sprintf(fontName ,"΢���ź�");
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
	BOOL	bHide;		//�����¼
	int		iLoginFlag; //��¼״̬
	BOOL	bAutoLogin;	//�Զ���¼
	BOOL	bRemeberPsw; //��ס����
	BOOL    bShowHint;
	DWORD	dwServerIP;	//��¼server
	RegAccountStruct()
	{
		memset(this, 0, sizeof(RegAccountStruct));
	}
};


enum IM_OPERATION{
	OPR_DEL_CLUSTER,	// ɾ��Ⱥ
	OPR_MODIFY_CLUSTER_NAME   ///����Ⱥǩ������Ⱥ����
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
	//�Է�����Ϣ�������Ұ棬�ҵ��Ŷ��ŵ�һλ��
	void Sort()
	{
		int i;
		//�ҵ��Ŷ��ڵ�һλ
		for ( i=1; i<count; i++ )
		{
#ifdef _CS_PLAT_EX_
			if (group[i].group_index == FEINIU_GROUP_ID)// �ͷ�����̨
#else
			if (group[i].group_index == MY_TEAM_GROUP_ID)//�ҵ��Ŷ�
#endif
			
			{
				GROUP_ITEM temp=group[i];
				group[i]=group[0];			
				group[0]=temp;			
				break;
			}
		}

		//�ҵĺ����ڵڶ�λ
		for (i = 2; i < count; ++i)
		{
			if (group[i].group_index == 0)//�ҵĺ���
			{
				GROUP_ITEM temp = group[i];
				group[i] = group[1];
				group[1] = temp;
				break;
			}
		}

		// �������ٵ����ڶ�λ
		for( i=1; i<count-1; i++ )
		{
			if (group[i].group_index == 2)//������
			{
				GROUP_ITEM temp=group[i];
				group[i]=group[count-1];
				group[count-1]=temp;
				break;
			}
		}

		// İ���������һλ
		for ( i=1; i<count-2; i++ )
		{
			if (group[i].group_index == 1)//İ����
			{
				GROUP_ITEM temp=group[i];
				group[i]=group[count-2];
				group[count-2]=temp;
				break;
			}
		}
	}
#else
	//�Է�����Ϣ����
	void Sort()
	{
		int i;
		//�ҵĺ����ڵ�һλ
		for (i = 1; i < count; i++)
		{
			if (group[i].group_index == 0)//�ҵĺ���
			{
				GROUP_ITEM temp = group[i];
				group[i] = group[0];
				group[0] = temp;
				break;
			}
		}

		for (i = 1; i < count - 1; i++)
		{
			if (group[i].group_index == 2)//������
			{
				GROUP_ITEM temp = group[i];
				group[i] = group[count - 1];
				group[count - 1] = temp;
				break;
			}
		}

		for (i = 1; i < count - 2; i++)
		{
			if (group[i].group_index == 1)//İ����
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

	// ��ȡһ����Ĳ㼶
	int GetLevel(int nGroupID)
	{
		// ����
		int nIndex = FindItemIndex(nGroupID);
		if(nIndex == -1)
			return -1;	// δ���ҵ�����

		int nLevel = 1;
		while(TRUE)
		{
			int nParentGroupID = group[nIndex].group_father;
			if(nParentGroupID == -1)
				break;

			nIndex = FindItemIndex(nParentGroupID);
			if(nIndex == -1)
				return -1;	// ��Ӧ�ó����Ҳ��������

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
	// ע�⣬�ڻ���ṹ�ı�ʱҲҪ���°汾��
	const static uint32 VER = 0x00000001;

	enum STATUS{
		ONLINE = XTREAM_ONLINE,
		OFFLINE = XTREAM_OFFLINE,
		HIDE = XTREAM_HIDE,
		LEAVE = XTREAM_LEAVE,
		OTHER
	};
	enum CONN_FLAG{
		NONE = 0,	// û�г�������
		TRY_LAN,	// ��������������
		LAN,		// �������ӳɹ�
		LAN_FAILED,	// ��������ʧ��
		TRY_P2P,	// ���Ե�Ե�������
		P2P,		// ��Ե����ӳɹ�
		P2P_FAILED,	// ��Ե�����ʧ��
		SERVER		// ��������ת
	};
	uint32		ip           ;
	uint32		localIP		 ;
	uint16		port         ;
	uint16		localPort	 ;
	uint8		connFlag	 ; // ���ӱ�־
	uint8		group_index  ;
	uint16      ver          ; //�ͻ��˰汾��
	char		key[16]      ; //P2P֮���session key
	uint8		level        ; //�û��ȼ�
	uint8       equipment     ; //1:ӵ����Ƶͷ
	uint8       cluster_property ;//1 byte,2:Creator;1:Admin;0:Normal
	bool        cluster_member ;
	uint8       status       ;//����״̬
	char		web_location[100] ;
	bool        bRefreshed   ;//�û������Ƿ���¹�
	bool        bClosePanel  ;//P2p����ʱ��user panel�Ƿ�ر�
	uint8		p2pRetry     ;// p2p���ӵĿ������Դ���	
	bool		bBasicInfo	 ;// ���������Ƿ���Ч
	uint8       isTalkZYF    ;//����ʱ�Ƿ�����ת���(1,yes  0,no)
	uint8       webInfo      ;//���Ƿ����������»�ͼƬ?0��,1ͼƬonly,2����only,3 bothͼƬ����
	
	bool        bInviting    ;//�Ƿ�����������...

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

//�ϵ��û���Ϣ���ֶ�������ṹ����Ϊ�˼����ϰ汾
struct FRIEND_INFO_OLD : public XT_USER_INFO_OLD
{
	// ע�⣬�ڻ���ṹ�ı�ʱҲҪ���°汾��
	const static uint32 VER = 0x00000001;

	enum STATUS{
		ONLINE = XTREAM_ONLINE,
		OFFLINE = XTREAM_OFFLINE,
		HIDE = XTREAM_HIDE,
		LEAVE = XTREAM_LEAVE,
		OTHER
	};
	enum CONN_FLAG{
		NONE = 0,	// û�г�������
		TRY_LAN,	// ��������������
		LAN,		// �������ӳɹ�
		LAN_FAILED,	// ��������ʧ��
		TRY_P2P,	// ���Ե�Ե�������
		P2P,		// ��Ե����ӳɹ�
		P2P_FAILED,	// ��Ե�����ʧ��
		SERVER		// ��������ת
	};
	uint32		ip           ;
	uint32		localIP		 ;
	uint16		port         ;
	uint16		localPort	 ;
	uint8		connFlag	 ; // ���ӱ�־
	uint8		group_index  ;
	uint16      ver          ; //�ͻ��˰汾��
	char		key[16]      ; //P2P֮���session key
	uint8		level        ; //�û��ȼ�
	uint8       equipment     ; //1:ӵ����Ƶͷ
	uint8       cluster_property ;//1 byte,2:Creator;1:Admin;0:Normal
	bool        cluster_member ;
	uint8       status       ;//����״̬
	char		web_location[100] ;
	bool        bRefreshed   ;//�û������Ƿ���¹�
	bool        bClosePanel  ;//P2p����ʱ��user panel�Ƿ�ر�
	uint8		p2pRetry     ;// p2p���ӵĿ������Դ���	
	bool		bBasicInfo	 ;// ���������Ƿ���Ч
	uint8       isTalkZYF    ;//����ʱ�Ƿ�����ת���(1,yes  0,no)
	uint8       webInfo      ;//���Ƿ����������»�ͼƬ?0��,1ͼƬonly,2����only,3 bothͼƬ����

	bool        bInviting    ;//�Ƿ�����������...

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
// ����������
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

typedef CList<SYSTEM_MSG,SYSTEM_MSG> SYSTEM_MSG_LIST;//ϵͳ��Ϣ

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

typedef CList<CLUSTER_SYSTEM_MSG,CLUSTER_SYSTEM_MSG> CLUSTER_SYSTEM_MSG_LIST;//Ⱥϵͳ��Ϣ�����ڲ鿴Ⱥ��Ϣ�ͺ�����Ϣ

struct USER_INFO : public FRIEND_INFO
{
	bool bAutoLogin; //�Ƿ��Զ���½
	bool bHideLogin; //��½��ʼ״̬�Ƿ�Ϊ����
	BOOL bShowHint ; //�Ƿ�������ʾ��Ϣ
	int  iLoginFlag; //��½��ʼ״̬
	BOOL bRemeberPsw;//�Ƿ��ס����
	uint32  merchant_id;
	BOOL bProfileModified;////�Ƿ��޸�Ⱥ��Ƭ
};

//�û�ȫ����Ϣ
extern USER_INFO     g_ImUserInfo;
struct CLUSTER_INFO : public XT_CLUSTER_INFO
{	
	const static uint32 VER = 0x00000001;

	bool bAutoShowMagic;
	bool bRefused      ;	//�Ƿ���ֹ
	bool bRefreshFlag  ;
	bool bMemberInit   ;	//��Ա�Ƿ��ʼ��
	bool bBasicInfo;		//����Ⱥ��Ϣ�Ƿ���Ч 
	uint8 radio;
	enum{RECV_SHOW = 1,RECV_NOSHOW = 2,NORECV = 3,SHOW_NUM = 4};
	uint32 nMsgNum;			// δ����Ϣ������Ⱥ��Ϣ��������ѡ��ֻ��ʾ����ʱ��Ч��

	DWORD dwTickGet    ;
	vector<FRIEND_INFO>  members;
	DWORD dwClusterPageShow    ;    //��ʱ������ʾ1,2,3,4��ҳ���ж�;
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

	//�ж�Ⱥ��Ա���ԣ�2:Creator;1:Admin;0:Normal)
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

struct ContactGroupInfo//��ϵ������Ϣ
{
	struct tagItem
	{
		int  iIndex     ;//��id
		char szName[50] ;//������
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
		sprintf(items[0].szName,"����");

		items[1].iIndex=1;
		sprintf(items[1].szName,"����");

		items[2].iIndex=2;
		sprintf(items[2].szName,"ͬѧ");

		items[3].iIndex=3;
		sprintf(items[3].szName,"ͬ��");

		items[4].iIndex=MAX_INT;
		sprintf(items[4].szName,"����");
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

struct ContactPersonInfo //��ϵ����Ϣ
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
	int				nInvited;//�Ƿ��͹�����1�ǣ�0��
	int             nSex;
	int				nType; //0,mini139 user; 1 windows address book contact 
	int				nGroupIndex;//��Ӧ��ContactGroupInfo ����id

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
	DWORD      dwViewType	;//����Ȩ��  0���Լ���1�����ѣ�2��ע���û���3������
	DWORD      dwTopFlag	;
	DWORD      dwDirID		;
	CString    strTitle		;
	CString    strContent   ;
	CString    strCreateDate;
	CString    strTag		;
	CString    strDirName   ;
};

typedef list<ContactPersonInfo *>   ADDRESS_BOOK_LIST;//��ϵ�˵�ַ�б�

typedef list<IMAGE_FILE_STRUCT>   IMAGE_FILE_LIST;

typedef list<FRIEND_INFO *>       FRIEND_LIST;


#define GIF_EMOTION_COUNT           55


extern  int	MAX_USER_HEAD_COUNT		;

//�������������
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

extern bool         g_bOs2K          ;// �Ƿ���2K��2K���µ�ϵͳ

extern HWND			g_hMessageWnd    ;
extern HWND         g_hMainWnd       ;

//extern map<int,CString>  g_mapFamilyFace ;//map<family id, face url>

#define KEYDOWN(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000) ? TRUE : FALSE )

extern FRIEND_LIST   g_lsWebVounchFriends;//��վ�Ƽ������б�
extern FRIEND_LIST   g_lsFriend		 ;//��XTreamMainWndά���������б�
extern FRIEND_LIST	 g_lsFriendTemp	 ;// ��ʱ������Ϣ	
extern GROUP_INFO	 g_GroupInfo	 ;//��XTreamMainWndά�������ѷ�����Ϣ
extern vector<CLUSTER_INFO *> g_vecCluster ;
extern vector<CLUSTER_INFO *> g_vecClusterExit ;
extern HANDLE        g_hUserMutex    ;//�����û�ͬһ�ʻ���½����
extern HBRUSH        g_hBackBrush    ;
extern HBRUSH        g_hWhiteBrush   ;

extern CString       g_strUpdateInfo ;
extern CString       g_strDefaultExplorePath;


extern map<int,CString>    g_mapVideoDriverIndex;//��Ƶͷ����
extern map<uint16,uint16>  g_mapUserOper ;//�û���Ϊͳ��

extern IWebConfigXPtr    g_pWebConfig;
extern IWriteArticleXPtr g_pWriteArticle;
extern IMyPicturesXPtr   g_pMyPictures;
extern IMyArticlesXPtr   g_pMyArticles;
extern IHeadPhotoXPtr    g_pHeadPhoto ;
//extern IIMiniPictureBroadCastXPtr g_pMiniPictureBroadCast;

extern CSpaceData		 g_spaceData;

CString		SexToStr(unsigned char sex);	//�Ա�ת�����ַ�������
CString		StatusToStr(int status);		//״̬ת�����ַ�������
COLORREF	StatusToColor(int status);		//״̬ת������Ӧ��������ʾ��ɫ

CString		UserInfoToStr(XT_USER_INFO *);    //�û���Ϣת�����ַ�������
/*���ַ�������һ��������Ϣ(id,name,...)*/
CString		UserInfoToStrEx(XT_USER_INFO *user);
CString		CMemberInfoToStr(XT_USER_INFO *); //Ⱥ��Ա��Ϣת�����ַ�������
CString     CountInfoToStr(XT_USER_INFO *user);    //Ⱥ��Ա�˺�����

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
// ���ݺ���id���ӱ������е������в�ѯ
FRIEND_INFO *	GetFriendInfo(uint32 fid);
void		   ClearFriendList();
void		   ClearList(FRIEND_LIST &l);

int			   GetDefaultImage(int sex,int status);



//��ʾ�������ߣ���������
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

BOOL        DownloadMini139(CString strVersionUrl, CString & strDownloadFile/*[out]�������غ�ı����ļ�*/);

void ShowMessage(CString strMessage,UINT nType=MB_OK|MB_ICONINFORMATION);

INT CALLBACK EnumFontNameProc(LOGFONT *plf, TEXTMETRIC* /*ptm*/, INT /*nFontType*/, LPARAM lParam);

// ��ú��ֵ��׸�ƴ����ĸ,��ͨascii��ֱ�ӷ���,û���ҵ��򷵻�0
char		GetFirstPinyin(const char* szGb2312);

// ������Ⱥ������Ϣ�еİ����Ķ�����Ϣ
// ����������ʽ{\action0[]action1[\object]action2\}
BOOL		ParseMsg(const char* szMsg,
			  CString& sMsg,
			  CString& sObj,
			  CString& sAction0,
			  CString& sAction1,
			  CString& sAction2);

bool ValidateString(const CString &s, CString &error );

// ȥ���ַ�����β��ȫ�ǿհ��ַ�
void TrimSpace2(CString& sSrc);

void GetMD5FileName(const CString& sFilePathName,CString& sMD5Name);

void GetMD5WebFileName(const CString & sUrl,CString & sMD5Name);

//��¼һ���û�����
void        RegUserOperation(uint16 operId);

void		ClearAddressBookList(ADDRESS_BOOK_LIST &l);

void		WriteArticlePreviewFile(const TWebArticleInfo &info, CString strAuthor, CString strLocalFile);


void	ShowUploadPicDlg(char *username, char *nickname, char *password, uint32 uid );
void    CloseUploadPicDlg();


void  ClearWebVounchedFriends();//���web�Ƽ������б�
FRIEND_INFO * FindWebVounchedFriend(uint32 fid);//

bool compareP(const TWebPictureInfo * info1, const TWebPictureInfo * info2 );

CString WebDataFrom2Str(int iFrom);
CString WebViewProp2Str(int iViewProp);//�鿴Ȩ��


void   SaveUserWebData();

void NotifyVideoEquipment(FRIEND_INFO * pFriend);

bool LoadImageFromResource(IN CImage* pImage,IN UINT nResID, IN LPCSTR lpTyp);

bool ImageFromIDResource( Image * & pImg, UINT nID, LPCTSTR sTR = _T("PNG")) ;

#ifdef _BUSINESS_VESION
// �ж���ID�Ƿ������ҵ��Ŷӷ���
BOOL CheckSubGroupIsInMyTeam(int nGroupIndex);
#endif

// ���ݺ����û������ҽڵ�
FRIEND_INFO * FindFriend(const char *szUserName);

// ��ȡİ��������
int GetStrangerCount();

const char* SubstrExist(const char* s,int nLen,const char* szSub);

// �Զ��Ӵ�
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
