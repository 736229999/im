#include "stdafx.h"
#include "..\IMClient.h"
#include "impublic.h"
#include "xtpacket.h"
#include "hintdlg.h"
#include "ClusterAgreeDlg.h"
#include "messagehandlewnd.h"
#include "webprocess.h"
#include "DuiCounsel.h"
// 密钥
//BYTE g_Key[16];g_ImUserInfo.GetPassword().c_str()

// 系统字体
static vector<string> g_vecSysFontName;

IWebConfigXPtr    g_pWebConfig	  ;
IMyPicturesXPtr   g_pMyPictures   ;
IWriteArticleXPtr g_pWriteArticle ;
IMyArticlesXPtr   g_pMyArticles   ;
IHeadPhotoXPtr    g_pHeadPhoto	  ;
//IIMiniPictureBroadCastXPtr g_pMiniPictureBroadCast;

CSpaceData        g_spaceData     ;

XT_FONT			g_ImFont			; //当前聊天框使用的字体
USER_INFO       g_ImUserInfo		; //当前登录用户状态,信息
FRIEND_LIST     g_lsFriend			; //好友列表
FRIEND_LIST		g_lsFriendTemp		; // 临时好友信息
FRIEND_LIST     g_lsWebVounchFriends; // 网站推荐好友列表
GROUP_INFO	    g_GroupInfo			;
vector<CLUSTER_INFO *> g_vecCluster ;    //群列表
vector<CLUSTER_INFO *> g_vecClusterExit; //退出的群列表
CImageList		g_imgEmotionList	;
CImageList      g_imgStatus24       ;
CImageList      g_imgStatus30       ;
CImageList      g_imgStatus35       ;
CImageList      g_imgStatus47       ;
CImageList      g_imgStatus52       ;

CImageList      g_imgGroupStatus30  ;
CImageList      g_imgGroupStatus35  ;


CImageList      g_imgAppIconXp      ;
CImageList      g_imgAppIcon2k      ;

bool            g_bOs2K           = false;

HWND            g_hMessageWnd     = NULL;
HWND            g_hMainWnd        = NULL;
HANDLE          g_hUserMutex      = NULL;
HBRUSH          g_hBackBrush      = ::CreateSolidBrush(BACKGND_COLOR);
HBRUSH          g_hWhiteBrush     = ::CreateSolidBrush(RGB(255,255,255));

int	MAX_USER_HEAD_COUNT   = 0;

CString         g_strUpdateInfo ;
CDuiCounsel *g_pDuiCounsel = NULL;//正在咨询框


// 解析出群聊天信息中的包含的动作信息
// 动作定义表达式{\action0[]action1[\object]action2\}
BOOL ParseMsg(const char* szMsg,
			  CString& sMsg,
			  CString& sObj,
			  CString& sAction0,
			  CString& sAction1,
			  CString& sAction2)
{
	int nLen=0;
	const char* p=szMsg;
	const char* pWord;
	char* pData;

	if(*p==0)
		return FALSE;
	// 动作定义必须以符号"{\"开始,以"\}"结束
	if(*p=='{'&& *(p+1)=='\\')
	{
		// 有动作信息
		p+=2;
		if(*p==0)
		{
			return FALSE;
		}
		// action0的字符串开始
		pWord=p;
		while(*p)
		{
			// 获得发话人位置
			if(*p=='[' && *(p+1)==']')
			{
				// action0字符串结束
				nLen=p-pWord;
				pData=new char[nLen+1];
				strncpy(pData,pWord,nLen);
				*(pData+nLen)=0;
				sAction0=pData;
				p+=2;
				// action1字符串开始
				pWord=p;
				break;
			}
			else
			{
				p++;
			}
		}
		// 获得action1
		while(*p)
		{
			if(*p=='[' && *(p+1)=='\\')
			{
				nLen=p-pWord;
				pData=new char[nLen+1];
				strncpy(pData,pWord,nLen);
				*(pData+nLen)=0;
				sAction1=pData;
				// action1字符串结束
				p+=2;
				// obj字符串开始
				pWord=p;
				break;
			}
			else
			{
				p++;
			}
		}
		// 获取object
		while(*p)
		{
			if(*p==']')
			{
				// obj字符串结束
				nLen=p-pWord;
				pData=new char[nLen+1];
				strncpy(pData,pWord,nLen);
				*(pData+nLen)=0;
				sObj=pData;
				p+=1;
				// action2字符串开始
				pWord=p;
				break;
			}
			else
			{
				p++;
			}
		}
		// 获取action2
		while(*p)
		{
			if(*p=='\\' && *(p+1)=='}')
			{
				nLen=p-pWord;
				pData=new char[nLen+1];
				strncpy(pData,pWord,nLen);
				*(pData+nLen)=0;
				sAction2=pData;
				p+=2;
				// 消息字符串正式开始
				sMsg=p;
				break;
			}
			else
			{
				p++;
			}
		}
		return TRUE;
	}
	else
	{
		// 没有包含动作信息
		return FALSE;
	}
}

#ifdef _DEBUG
void testParseMsg()
{
	char * szMsgs[]={
		"{\\[]aabb\\}",
			"{\\可爱的[]飞起一脚踢到[\\小明]的头上\\}哈哈",
			"abc",

	};
	CString sMsg;
	CString sObj;
	CString sAction0;
	CString sAction1;
	CString sAction2;

	for(int i=0;i<sizeof(szMsgs)/sizeof(char*);i++)
	{
		if(ParseMsg(szMsgs[i],
			sMsg,
			sObj,
			sAction0,
			sAction1,
			sAction2)
			)
		{
#ifdef _std_out	
			cout<<(LPCTSTR)sAction0
				<<"[]"
				<<(LPCTSTR)sAction1
				<<"["
				<<(LPCTSTR)sObj
				<<"]"
				<<(LPCTSTR)sAction2
				<<endl
				<<"msg="
				<<(LPCTSTR)sMsg
				<<endl;

		}
		else
		{
			cout<<szMsgs[i]<<endl;
#endif
		}

		sMsg="";
		sObj="";
		sAction0="";
		sAction1="";
		sAction2="";
	}


}
#endif


// 区位码范围
struct QWM_RANGE
{
	unsigned short sBeg;
	unsigned short sEnd;
	// 判断是否为有效的范围
	bool IsValid() const
	{
		return (sBeg <= sEnd ) && (sBeg != -1) && (sEnd != -1);
	}
	// 返回值=0表示在范围内，<0小于范围，>0大于范围
	int RangeIn(unsigned short code) const
	{
		if(code >= sBeg )
		{
			if(code <= sEnd)
				return 0;
			else
				return 1;
		}
		else
		{
			return -1;
		}
	};
};

// 区位码字母分布表
QWM_RANGE QWM_TABLE[26]=
{
	{0xb0a1,0xb0c4},	// a
	{0xb0c5,0xb2c0},	// b
	{0xb2c1,0xb4ed},	// c
	{0xb4ee,0xb6e9},	// d
	{0xb6ea,0xb7a1},	// e
	{0xb7a2,0xb8c0},	// f
	{0xb8c1,0xb9fd},	// g
	{0xb9fe,0xbbf6},	// h
	{-1,-1},			// i
	{0xbbf7,0xbfa5},	// j
	{0xbfa6,0xc0ab},	// k
	{0xc0ac,0xc2e7},	// l
	{0xc2e8,0xc4c2},	// m
	{0xc4c3,0xc5b5},	// n
	{0xc5b6,0xc5bd},	// o
	{0xc5be,0xc6d9},	// p
	{0xc6da,0xc8ba},	// q
	{0xc8bb,0xc8f5},	// r
	{0xc8f6,0xcbfa},	// s
	{0xcbfb,0xcdd9},	// t
	{-1,-1},			// u
	{-1,-1},			// v
	{0xcdda,0xcef3},	// w
	{0xcef4,0xd1b8},	// x
	{0xd1b9,0xd4d0},	// y
	{0xd4d1,0xd7f9},	// z
};

// 获得汉字的首个拼音字母
char GetFirstPinyin(const char* szGb2312)
{
	if(szGb2312==NULL)
		return 0;
	const unsigned char *p=(const unsigned char*)szGb2312;
	unsigned short w=0;
	char *pB=(char*)&w;
	int i=0;
	if(*p<128)
	{
		// ascii码
		return *p;
	}
	else
	{
		// gb2312中区号在低地址字节,位号在高地址字节,所以要颠倒过来
		*pB=*(szGb2312+1);	// 获得位号
		*(pB+1)=*(szGb2312);	// 获得区号
		for(i=0;i<26;i++)
		{
			if(QWM_TABLE[i].sBeg!=-1)
			{
				// 判断是否在该区内
				if(QWM_TABLE[i].RangeIn(w)==0)
					return i+'a';
			}
		}
	}
	return 0;
}

CString  SexToStr(unsigned char sex)
{
	if ( sex==0 )
	{
		return "女";
	}
	else
	if (sex==1)
	{
		return "男";
	}
	else
	{
		return "女";
	}
}

void ShowMessage(CString strMessage,UINT nType)
{
	char *msg = new char[strMessage.GetLength()+1];
	strcpy(msg,strMessage);
	PostMessage(g_hMessageWnd,WM_SHOWMESSAGE,(WPARAM)msg,nType);
}

/*用字符串描述一个好友信息(id,name,...)*/
CString UserInfoToStr(XT_USER_INFO *user)
{
	char szTemp[20];
	if(user->comment_name[0] == 0)
	{
		if(user->profile_name[0] == 0)///////好友群名片
		{
			if(user->nickname[0] == 0)
			{
				return itoa(user->GetId(),szTemp,10);
			}
			else
			{
				if(CMainFrame::IsCSer(user->GetId()))
				{
					CString strNewName;
					strNewName.Format("%s(%s)", (LPCTSTR)user->nickname, user->GetUserName().c_str());
					return strNewName;
				}
				else
				{
					CString strNickName = user->nickname;
					if(strNickName.Left(1) == "1" && strNickName.GetLength() == 11)
					{
						BOOL bIsPhoneNum = TRUE;
						for(int nNum = 0; nNum < strNickName.GetLength(); nNum++)
						{
							if(!(strNickName[nNum] >= '0' && strNickName[nNum] <= '9'))
							{
								bIsPhoneNum = FALSE;
								break;
							}
						}
						if(bIsPhoneNum)
						{
							strNickName.Format("%s****%s", strNickName.Left(3), strNickName.Right(4));
							return strNickName;
						}
					}
					return user->nickname;
				}
				
			}
		}
		else////本人的群名片不为空
		{
			return user->profile_name;
		}
	}
	else
	{
		return user->comment_name;
	}
}

/*用字符串描述一个好友信息(id,name,...)*/
CString UserInfoToStrEx(XT_USER_INFO *user)
{
	CString strRlt = UserInfoToStr(user);

	if(CMainFrame::IsBuyer(user->GetId()))
	{	// 买家，修改显示昵称
		string strUserName = user->GetUserName();
		if(strUserName != "")
		{
			CString strNewName;
			strNewName.Format("%s(%s)", (LPCTSTR)strRlt, strUserName.c_str());
			strRlt = strNewName;
		}
	}

	return strRlt;
}


/*用字符串描述一个群成员的信息*/
CString CMemberInfoToStr(XT_USER_INFO *user)
{
	char szTemp[20];
	if(user->profile_name[0] == 0)///////群名片
	{
		if(user->comment_name[0] == 0)
		{

			if(user->nickname[0] == 0)
			{
				return itoa(user->GetId(),szTemp,10);
			}
			else
			{
				return user->nickname;
			}
		}
		else////本人的群名片不为空
		{
			return user->comment_name;
		}
	}
	else
	{
		return user->profile_name;
	}
}



/*用字符串描述一个群成员的账号信息*/
CString CountInfoToStr(XT_USER_INFO *user)
{
	char szTemp[20];

	if(user->comment_name[0] == 0)///////群名片
	{

		if(user->nickname[0] == 0)
		{
			return itoa(user->GetId(),szTemp,10);
		}
		else
		{
			return user->nickname;
		}
	}
	else////本人的群名片不为空
	{
		return user->comment_name;
	}

}

/*将好友状态对应相应颜色*/
COLORREF StatusToColor(int status)
{
	if ( status==XTREAM_ONLINE )
	{
		return  RGB(0,0,0);
	}
	else
	if ( status==XTREAM_OFFLINE )
	{
		return RGB(111,111,111);
	}
	else
	if ( status==XTREAM_HIDE )
	{
		return RGB(111,111,111);
	}
	else
	if ( status==XTREAM_LEAVE )
	{
		return RGB(200,111,111);
	}
	else
	{
		return RGB(111,111,111);
	}
}

/*用文字描述对应的状态*/
CString StatusToStr(int status)
{
	if ( status==XTREAM_ONLINE )
	{
		return "在线";
	}
	else if ( status==XTREAM_OFFLINE )
	{
		return "离线";
	}
	else if ( status==XTREAM_HIDE )
	{
		return "隐身";
	}
	else if ( status==XTREAM_LEAVE )
	{
		return "离开";
	}
	else if( status==XTREAM_BUSY)
	{
		return "忙碌";
	}
	else if( status==XTREAM_HANGUP )
	{
		return "挂起";
	}
	else
	{
		return "状态不确定";
	}
}

/*根据id,从好友链表中查找节点*/
FRIEND_INFO * FindFriend(uint32 fid)
{
	FRIEND_LIST::iterator it;
	for ( it=g_lsFriend.begin(); it!=g_lsFriend.end(); it++)
	{
		if ( (*it)->GetId()==fid )
		{
			return (*it);
		}
	}

	return NULL;
}

FRIEND_INFO * GetFriendInfo(uint32 fid)
{
	//
	// 顺序不能乱
	//

	// 首先判断是否是自己
	if(fid == g_ImUserInfo.GetId())
		return &g_ImUserInfo;
	// 然后查找好友列表
	FRIEND_INFO * pFriend = FindFriend(fid);
	if(pFriend != NULL)
		return pFriend;
	// 最后查找群列表
	size_t i,c;
	c = g_vecCluster.size();
	for( i = 0; i < c; i++)
	{
		pFriend = g_vecCluster[i]->GetFriendInfo(fid);
		if(pFriend)
		{
			return pFriend;
		}
	}
	// 补充，从临时好友中查询
	FRIEND_LIST::iterator it,itE;
	itE = g_lsFriendTemp.end();
	for ( it = g_lsFriendTemp.begin(); it != itE; it++)
	{
		if ( (*it)->GetId() == fid )
		{
			return (*it);
		}
	}
	return NULL;
}

INT CALLBACK EnumFontNameProc(LOGFONT *plf, TEXTMETRIC* /*ptm*/, INT /*nFontType*/, LPARAM lParam)
{
	vector<string> *pvecFontName = (vector<string> *) lParam;
	pvecFontName->push_back(plf->lfFaceName);

	return TRUE;
}

void IM_LOG(CString str)
{
#ifdef _DEBUG
	try
	{
	CString strText;
	strText.Format("[%s]:%s\n",CTime::GetCurrentTime().Format("%Y-%m-%d %H:%M:%S"),str);
	
	char sExePath[MAX_PATH];
	char drive[MAX_PATH], dir[MAX_PATH], fname[MAX_PATH], ext[MAX_PATH];
	CString strPath;
	GetModuleFileName(NULL,sExePath,MAX_PATH);
	_splitpath( sExePath, drive, dir, fname, ext );
	_makepath(sExePath, drive,dir,"","");

	CString strLogFile;
	strLogFile.Format("%sdata\\%s\\im_client_log.txt", sExePath,g_ImUserInfo./*GetUserName*/GetUserFileName().c_str());

	CStdioFile file;

	bool bOpenFlag = false;

	if ( _access((LPCSTR)strLogFile,0)==-1 )
	{
		bOpenFlag = file.Open(strLogFile,CFile::typeText|CFile::modeCreate|CFile::modeWrite);
	}
	else
	{
		bOpenFlag = file.Open(strLogFile,CFile::typeText|CFile::modeWrite);
	}

	if ( bOpenFlag )
	{
		file.SeekToEnd();
		file.WriteString(strText);
		file.Close();
	}

	}
	catch(...)
	{
	}
#endif
}

void ClearList(FRIEND_LIST &l)
{
	FRIEND_LIST::iterator it;	

	for (it=l.begin(); it!=l.end(); it++ )
	{
		try
		{
			FRIEND_INFO *p = (*it);
			if ( p->pUserWebPictureInfo )
				delete p->pUserWebPictureInfo;

			if ( p->pUserWebArticleInfo )
				delete p->pUserWebArticleInfo;
			delete p;
		}
		catch(...)
		{
		}
	}
	
	l.clear();
}

void ClearFriendList()
{
	ClearList(g_lsFriend);
}

void ClearClusterList()
{
	for ( int i=0; i<g_vecCluster.size(); i++ )
	{
		delete g_vecCluster[i];
	}
	g_vecCluster.clear();
}

void ClearClusterExitList()
{
	//for ( int i=0; i<g_vecClusterExit.size(); i++ )
	//{
	//	delete g_vecClusterExit[i];
	//}
	//g_vecClusterExit.clear();
}

CLUSTER_INFO * FindClusterInfo(uint32 cid)
{
	int n=g_vecCluster.size();
	for ( int i=0; i<n; i++ )
	{
		if (g_vecCluster[i]->nFamilyID==cid )
		{
			CLUSTER_INFO *p = g_vecCluster[i];

			try
			{
				p->szFamilyName;
				return p;
			}
			catch(...)
			{
				return NULL;
			}

			break;
		}
	}

	return NULL;
}

inline BYTE toHex(const BYTE &x)
{
	return x > 9 ? x + 55: x + 48;
}

bool GetFileDir(const std::string & sPath, std::string & sDir )
{
	sDir = "";
	if(sPath.size() < 2)
		return false;

	const char * pB = sPath.c_str();
	const char * p = pB ;

	// 判断路径是否合法
	if( (*p >= 'a' && *p <= 'z') ||
		(*p>= 'A' && *p <= 'Z'))
	{
		if(*(p+1) == ':')
		{
			if(*(p+2) == '\0')
			{
				// 如果是根目录,直接返回
				sDir = sPath;
				return true;
			}
			else if(*(p+2) != '\\')
			{
				// 目录名有误
				return false;
			}
		}
		else
		{
			// 非法路径表达式
			return false;
		}
	}
	p = pB + sPath.size();
	if(*(p-1) == '\\')
	{
		// 去掉末尾的'\'
		--p;
	}
	while((--p) >= pB)
	{
		if(*p == '\\' )
		{
			if(p - pB < 2)
			{
				return false;
			}
			// 注意，这里调试信息可以能显示为空串，这是正常的。可参考basic_string的_Myptr()成员函数
			sDir.assign(pB,p - pB);
			return true;
		}
	}
	return false;
}

bool WebLogin(char * username, char * password)
{
	return true;
}

void ShowWebRegisterPage()
{
	OpenHtmlPage("www.feiniu.com");
}

void ShowAlbum(char *username)
{
	if(g_pWebConfig)
	{
		CString strUrl = (LPCSTR)g_pWebConfig->GetWebPageUrl(USER_PHOTO,StrToWideStr(username));
		MiniOpenUrl(strUrl);
	}

}

void ShowBlogSite(char *username)
{
	if(g_pWebConfig)
	{
		CString strUrl = (LPCSTR)g_pWebConfig->GetWebPageUrl(USER_HOME,StrToWideStr(username));
		MiniOpenUrl(strUrl);
	}

}

void ShowArticle(char *username)
{
	if(g_pWebConfig)
	{
		CString strUrl = (LPCSTR)g_pWebConfig->GetWebPageUrl(USER_ARTICLE,StrToWideStr(username));
		MiniOpenUrl(strUrl);
	}

}

void ShowBlogFamilySite(char* clusterName)
{
	if(g_pWebConfig)
	{
		CString sUrl = (LPCSTR)g_pWebConfig->GetWebPageUrl(FAMILY_HOME,StrToWideStr(clusterName));
		MiniOpenUrl(sUrl);
	}

}

CString GetMiniUrl(const CString & strUrl)
{
	return strUrl;
	//CString strOpen;
	//CString strParamInfo = "";

	//strParamInfo = strParamInfo + UIntToStr(g_ImUserInfo.GetId())+"&";
	//strParamInfo = strParamInfo + CString(g_ImUserInfo.GetUserName().c_str())+"&";
	//strParamInfo = strParamInfo + UrlEncode(g_ImUserInfo.nickname)+"&";
	//strParamInfo = strParamInfo + UIntToStr(g_ImUserInfo.sex)+"&";
	//strParamInfo = strParamInfo + UrlEncode(g_ImUserInfo.email)+"&";
	//strParamInfo = strParamInfo + UIntToStr((int)g_ImUserInfo.gold_money)+"&";
	//strParamInfo = strParamInfo + UrlEncode(g_ImUserInfo.face_url)+"&";
	//strParamInfo = strParamInfo + IntToStr(g_ImUserInfo.message_count);

	//strParamInfo = Base64Encode(strParamInfo);


	//CString strKey    = "DJFU#(R#URJ#@Od#@$#@09809";
	//CString strUserId = UIntToStr(g_ImUserInfo.GetId());
	//CString strParamKey;

	//int l = strUserId.GetLength();
	//for ( int i=0; i<l; i++ )
	//{
	//	strParamKey+=strUserId[i];
	//	strParamKey+=strKey[i];
	//}

	//strParamKey+=strKey.Right(strKey.GetLength()-l);

	//unsigned char digest[16];

	//MD5_CTX md5T;
	//md5T.MD5Update ((unsigned char*)(LPCSTR)strParamKey,(uint32)strParamKey.GetLength() );
	//md5T.MD5Final (digest);

	//strParamKey = ToHexString(digest,16);

	////strOpen.Format("http://passport.feiniu.com/MiniOpen.aspx?info=%s&key=%s&url=%s",strParamInfo,strParamKey,strUrl);
	////strOpen.Format("http://www.feiniu.com");

	//return strOpen;
}

void MiniOpenUrl(const CString & strUrl)
{
	OpenHtmlPage(GetMiniUrl(strUrl));
}

int GetDefaultImage(int sex, int status)
{
	int uIcon;

	switch(status)
	{
	case XTREAM_LEAVE:
		uIcon = 0;//IDI_LEAVE;
		break;
	case XTREAM_OFFLINE:
		uIcon = 1;//IDI_OFFLINE;
		break;
	case XTREAM_HIDE:
		uIcon = 3;//IDI_HIDE;
		break;
	case XTREAM_ONLINE:
		uIcon = 0;//IDI_ONLINE;
		break;
	case XTREAM_BUSY:
		uIcon = 0;//IDI_ONLINE;
		break;
	default:
		uIcon = 0;//IDI_OFFLINE;
		break;
	}

	//if ( sex==0 )//女
	//	uIcon+=4;

	if(status == XTREAM_ONLINE||status == XTREAM_BUSY||status ==XTREAM_LEAVE)
		return 1;
	else
		return 0;
	return uIcon;
}

//bool SaveBitmapFile(const CBitmap* pBmp,int width,int height)
//{
//	assert(pBmp);
//	assert(width > 0);
//	assert(height > 0);
//
//	CWnd wnd;
//	CDC * pDC;
//	CDC dcMem;
//	CBitmap bitmap;
//	Bitmap img;
//	wnd.Attach(GetDesktopWindow());
//	pDC = wnd.GetDC();
//	dcMem.CreateCompatibleDC(pDC);
//	bitmap.CreateCompatibleBitmap(dcMem);
//	Graphics g(dcMem.m_hDC);
//	
//	g.DrawImage()
//	wnd.ReleaseDC(pDC);
//	wnd.Detach();
//	return true;
//}
CBitmap * CBitmapFromStream(char *pData, int dataLen, int nWidth, int nHeight)
{
	IStream *pStm = NULL;

//	BOOL bResult; 

	//分配全局存储空间 
	HGLOBAL hGlobal = GlobalAlloc(GMEM_MOVEABLE, dataLen ); 
	LPVOID pvData = NULL; 

	if (hGlobal == NULL) 
		return NULL; 

	if ((pvData = GlobalLock(hGlobal)) == NULL)//锁定分配内存块 
		return NULL; 

	memcpy(pvData,pData,dataLen);

	GlobalUnlock(hGlobal); 

	CreateStreamOnHGlobal(hGlobal, TRUE, &pStm); 

	Bitmap * bitmap   = Bitmap::FromStream(pStm);
	CBitmap * pBitmap = NULL;

	if ( bitmap )
	{
		float cx = bitmap->GetWidth();
		float cy = bitmap->GetHeight();

		if ( nWidth==0 )
			nWidth  = cx;
		if ( nHeight==0 )
			nHeight = cy;

		CWnd * pWnd = CWnd::GetDesktopWindow();//AfxGetMainWnd();

		CDC *pDC = 0;
		
		try
		{
			pDC=pWnd->GetDC();
		}
		catch(...)
		{
			pDC=0;
			pWnd=NULL;
		}

		if(pDC)
		{
			pBitmap = new CBitmap();
			pBitmap->CreateCompatibleBitmap(pDC,nWidth,nHeight);

			CDC dcTemp;
			dcTemp.CreateCompatibleDC(pDC);
	
			CBitmap *pOldbmp = dcTemp.SelectObject(pBitmap);

			dcTemp.FillSolidRect(0,0,nWidth,nHeight,0xffffff);

			Graphics g(dcTemp.m_hDC);
			Unit  units;

			float d1 = cx/nWidth;
			float d2 = cy/nHeight;


			//		float d  = d1>d2? d1:d2;//按比例缩放
			float d  = d1<d2? d1:d2;

			cx=cx/d; cy=cy/d;

			POINT pt;

			pt.x = (nWidth-cx)/2;
			pt.y = (nHeight-cy)/2;

			RectF destRect(REAL(pt.x),	REAL(pt.y),	REAL(cx),	REAL(cy)),
				srcRect;
			bitmap->GetBounds(&srcRect, &units);
			g.DrawImage(bitmap, destRect, srcRect.X, srcRect.Y, srcRect.Width,	srcRect.Height,	UnitPixel, NULL);

			dcTemp.SelectObject(pOldbmp);
		}

		if ( pDC )
		{
			pWnd->ReleaseDC(pDC);
		}

		delete bitmap;
	}

	GlobalFree(hGlobal);

	return pBitmap;
}

bool GetFileName(const std::string & sPath, std::string & sFileName )
{
	sFileName = "";
	if(sPath.size() < 2)
		return false;
	const char * pB = sPath.c_str();
	const char * p = pB + sPath.size();
	const char * pE = p;
	while((--p) >= pB)
	{
		if(*p == '\\')
		{
			if(pE - p < 2)
			{
				return false;
			}
			// 注意，这里调试信息可以能显示为空串，这是正常的。可参考basic_string的_Myptr()成员函数
			sFileName.assign(p + 1,pE - p);
			return true;
		}
	}
	sFileName = sPath;
	return false;
}

int GetEncoderClsid(const WCHAR* format, CLSID* pClsid)
{
	UINT  num = 0;          // number of image encoders
	UINT  size = 0;         // size of the image encoder array in bytes

	ImageCodecInfo* pImageCodecInfo = NULL;

	GetImageEncodersSize(&num, &size);
	if(size == 0)
		return -1;  // Failure

	pImageCodecInfo = (ImageCodecInfo*)(malloc(size));
	if(pImageCodecInfo == NULL)
		return -1;  // Failure

	GetImageEncoders(num, size, pImageCodecInfo);

	for(UINT j = 0; j < num; ++j)
	{
		if( wcscmp(pImageCodecInfo[j].MimeType, format) == 0 )
		{
			*pClsid = pImageCodecInfo[j].Clsid;
			free(pImageCodecInfo);
			return j;  // Success
		}    
	}

	free(pImageCodecInfo);
	return -1;  // Failure
}


//bool SaveAsBitmap(const CString & sImageFile,int nWidth,int nHeight)
//{
//	string sFileName;
//	GetFileName((LPCSTR)sImageFile,sFileName);
//	Bitmap * pBitmap = Bitmap::FromFile(sImageFile.AllocSysString());
//	Bitmap * pNewBmp;
//	if(pBitmap)
//	{
//		CLSID bmpClsid;
//		GetEncoderClsid(L"image/bmp", &bmpClsid);
//		pNewBmp = pBitmap->Clone();
//		delete pBitmap;
//		return true;
//	}
//	else
//	{
//		return false;
//	}
//}

// 获取表情的26x26的缩略图
CBitmap * GetFaceBmpFromFile(const CString & sImageFile)
{
	string sFileName;
	string sFileDir;
	GetFileName((LPCTSTR)sImageFile,sFileName);
	// 确定缩略图的文件名
	CString sSmallName;
	sSmallName.Format("_%s.bmp",sFileName.c_str());
	if(GetFileDir((LPCTSTR)sImageFile,sFileDir))
	{
		sFileDir += "\\";
		sSmallName = sFileDir.c_str() + sSmallName;
	}
	CBitmap * pBmpRt = NULL;

	CWnd * pWnd = CWnd::GetDesktopWindow();
	CDC * pDC;
	// 判断是否已经有缩略图，如果没有则生成
	if(_access(sSmallName,0) == -1)
	{
		
		if(pWnd)
		{
			pDC = pWnd->GetDC();
			Graphics g(pDC->m_hDC);
			BSTR bsName = sImageFile.AllocSysString();
			Bitmap * pBmp = Bitmap::FromFile(bsName);
			::SysFreeString(bsName);
			Bitmap bmpMem(20,20,&g);
			Rect destRect(0,0,20,20);
			Graphics gM(&bmpMem);
			SolidBrush brush(Color(255,255,255));
			gM.FillRectangle(&brush,destRect);
			gM.DrawImage(pBmp,destRect);
			if(pBmp == NULL)
			{
				pWnd->ReleaseDC(pDC);
				return NULL;
			}
			else
			{
				delete pBmp;
			}
			CLSID bmpClsid;
			GetEncoderClsid(L"image/bmp", &bmpClsid);
			BSTR bsSmallName = sSmallName.AllocSysString();
			bmpMem.Save(bsSmallName,&bmpClsid);
			::SysFreeString(bsSmallName);
			HBITMAP hBmp = NULL;
			pBmpRt = new CBitmap();
			bmpMem.GetHBITMAP(Color(255,255,255,255),&hBmp);
			pBmpRt->Attach(hBmp);
			pWnd->ReleaseDC(pDC);
			return pBmpRt;
		}
		else
		{
			return NULL;
		}
	}
	else
	{
		// 直接加载缩略位图
		pBmpRt = new CBitmap();
		HBITMAP hBitmap = (HBITMAP)LoadImage(NULL,sSmallName,IMAGE_BITMAP,0,0,LR_LOADFROMFILE|LR_DEFAULTSIZE);
		pBmpRt->Attach(hBitmap);
		return pBmpRt;
	}
}

BOOL DownloadMini139(CString strVersionUrl, CString &strDownloadFile)
{
	CString strXml = UrlGetStr(strVersionUrl);

    CComPtr<IXMLDOMDocument> pDoc ;
	pDoc.CoCreateInstance(__uuidof(DOMDocument));

	VARIANT_BOOL bSuccess         = false;
	bool         bGetDownloadInfo = false;

	CString strIssueName, strDate, strSize, strVersion, strDownloadUrl;

	strDownloadFile = "";
	BSTR szXml = strXml.AllocSysString();
	pDoc->loadXML(szXml,&bSuccess);
	SysFreeString(szXml);
	CComBSTR bstr(L"mm_hmg/update_list/update_item[0]/*"); 
	CComPtr<IXMLDOMNodeList> pNodes; 

	pDoc->selectNodes(bstr,&pNodes);

	if ( pNodes )
	{
		long nNodes; pNodes->get_length(&nNodes);

		if ( nNodes==5 )
		{
			CComPtr<IXMLDOMNode> pNode = NULL;

			pNodes->get_item(0, &pNode);

			USES_CONVERSION;

			pNode->get_text(&bstr);
			strVersion = W2A(bstr);

			pNode.Release();

			pNodes->get_item(1, &pNode);

			pNode->get_text(&bstr);
			strDate = W2A(bstr);

			pNode.Release();

			pNodes->get_item(2, &pNode);

			pNode->get_text(&bstr);
			strIssueName = W2A(bstr);

			pNode.Release();

			pNodes->get_item(3, &pNode);

			pNode->get_text(&bstr);
			strSize = W2A(bstr);

			pNode.Release();

			pNodes->get_item(4, &pNode);

			pNode->get_text(&bstr);
			strDownloadUrl = W2A(bstr);

			pNode.Release();

			bGetDownloadInfo = true;
		}
	}

	pDoc.Release();

	if ( bGetDownloadInfo==false )
	{
		return FALSE;
	}
	else
	{
		char temp[255];
		::GetTempPath(sizeof(temp),temp);

		CreateDirectory(temp,NULL);

		strDownloadFile.Format("%s\\%s",temp,strIssueName);

		return DownLoadWebFile(strDownloadUrl,strDownloadFile);
	}
}

void ShowUserHintDlg(FRIEND_INFO *pFriend,CString strTitle,CString strHint,HWND hCmdWnd)
{
	if ( g_ImUserInfo.bShowHint )
	{
		if(pFriend->GetId() != g_ImUserInfo.GetId())
			CHintDlg::Show(pFriend,strTitle,strHint,hCmdWnd);
	}
}

void ShowClusterHintDlg(CLUSTER_INFO*pCluster,CString sTitle,CString sHint,HWND hCmdWnd)
{
	if(g_ImUserInfo.bShowHint)
	{
		CHintDlg::Show(pCluster,sTitle,sHint,hCmdWnd);
	}
}

void ShowClusterAgreeDlg(CLUSTER_INFO*pCluster,FRIEND_INFO*pFriend,CString sTitle,CString sHint,HWND hCmdWnd)
{
	if(g_ImUserInfo.bShowHint)
	{
		CClusterAgreeDlg::Show(pCluster,pFriend,sTitle,sHint,hCmdWnd);
	}
}

void ShowUserHintDlg(CString strHint)
{
	if ( g_ImUserInfo.bShowHint )
	{
		CHintDlg::Show(strHint);
	}
}

bool FindForbiddenWords(CString str)
{
	static LPCSTR  forbidden_words[] ={"尻","操你","干死你","叉你","叉死","插你","插死","干你","干死","草你","屄","赑","妣","肏","尻","屌","fuck","江泽民","李鹏","胡锦涛","吴邦国","温家宝","贾庆林","曾庆红","黄菊","吴官正","李长春","罗干","杜世成","王丹","法轮","李洪志","夏耕","大法","FALUNDAFA","FALUN","falundafa","falun","DAFA",
		"我操","插舔","舔你","舔我","插我","阴蒂",
		"139.0033.cn","035667",
	}; 

	int count = sizeof(forbidden_words)/sizeof(LPCSTR);

	for ( int i=0; i<count; i++ )
	{
		if ( str.Find(forbidden_words[i],0)>-1 )
			return true;
	}

	return false;
}

void InitClusterType(CComboBox *pComboBox )
{

	static LPCSTR c_type [] = { 
			"食品","生鲜","数码","家电","日常生活",
			"母婴","图书音像","美妆","服饰箱包",
			"保健","兴趣爱好","工作交流","分享",
			"其他"

								};

	static int    c_id   [] = {
								101, 102, 103, 104,
								201, 202, 203, 204,
							   	301, 302, 303, 304,
							   	401	};

	int n = sizeof(c_id)/sizeof(int);

	pComboBox->ResetContent();

	for ( int i=0; i<n; i++ )
	{
		pComboBox->AddString(c_type[i]);
		pComboBox->SetItemData(i,c_id[i]);
	}

	pComboBox->SetCurSel(2);
}

LPCSTR GetClusterClass(int nClass )
{

	static LPCSTR c_type [] = { 
		"食品","生鲜","数码","家电","日常生活",
		"母婴","图书音像","美妆","服饰箱包",
		"保健","兴趣爱好","工作交流","分享",
		"其他"

	};

	static int    c_id   [] = {
		101, 102, 103, 104,
		201, 202, 203, 204,
		301, 302, 303, 304,
		401	};

		int n = sizeof(c_id)/sizeof(int);

		for ( int i=0; i<n; i++ )
		{
			if(c_id[i] == nClass)
				return c_type[i];
		}
		return "其他";
}

void SelClusterType(CComboBox *pComboBox, int iType)
{
	for ( int i=0; i<pComboBox->GetCount(); i++ )
	{
		if ( pComboBox->GetItemData(i)==iType )
		{
			pComboBox->SetCurSel(i);
			break;
		}
	}
}

DWORD GetClusterType(CComboBox *pComboBox)
{
	int nSel = pComboBox->GetCurSel();
	if ( nSel>-1 )
		return pComboBox->GetItemData(nSel);
	else
		return (DWORD)-1;
}

static HWND g_hRestoreActiveWnd = NULL;

void SaveActiveWindow()
{
	g_hRestoreActiveWnd = GetActiveWindow();
}

void RestoreActiveWindow()
{
	if (g_hRestoreActiveWnd)
	{
		try
		{
			::SetActiveWindow(g_hRestoreActiveWnd);
			::BringWindowToTop(g_hRestoreActiveWnd);
		}
		catch(...)
		{
		}

		g_hRestoreActiveWnd = NULL;
	}
}

//用户头像位图
	
struct tagBitmapData
{
	CBitmap * bitmap20;
	CBitmap * bitmap40;

	tagBitmapData()
	{
		bitmap20 = NULL;
		bitmap40 = NULL;
	}
};

static map<uint32,tagBitmapData *>  mapUserBitmap;

void ClearUserBitmap()
{
	map<uint32,tagBitmapData *>::iterator it;

	for ( it=mapUserBitmap.begin(); it!=mapUserBitmap.end(); it++ )
	{
		if ( it->second->bitmap20 )
		{
			try
			{
				delete it->second->bitmap20;
			}
			catch(...)
			{
			}

			it->second->bitmap20=NULL;
		}

		if (it->second->bitmap40 )
		{
			try
			{
				delete it->second->bitmap40;
			}
			catch(...)
			{
			}
		}

		delete it->second;
	}

	mapUserBitmap.clear();
}

bool MakeUserBitmap(FRIEND_INFO * pFriend)
{
	// 如果该好友没有定义头像则返回失败
	map<uint32,tagBitmapData *>::iterator it;
	it = mapUserBitmap.find(pFriend->GetId());
	CString sFace = ::g_webUserHeadIconProcess.GetFaceFile(pFriend);

	if ( FileExist(sFace) == FALSE )
	{
		return false;
	}

	if(it == mapUserBitmap.end())
	{
		tagBitmapData * pBitmapData = new tagBitmapData();
		pBitmapData->bitmap20 = CBitmapFromFile( sFace, 26, 26 );
		pBitmapData->bitmap40 = CBitmapFromFile( sFace, 40, 40 );
		mapUserBitmap.insert(map<uint32,tagBitmapData *>::value_type(pFriend->GetId(),pBitmapData));		
	}
	return true;
}

void UpdateUserBitmap(FRIEND_INFO * pFriend)
{
	map<uint32,tagBitmapData *>::iterator it;
	it = mapUserBitmap.find(pFriend->GetId());

	if ( it!=mapUserBitmap.end() )
	{
		if ( it->second->bitmap20 )
		{
			delete it->second->bitmap20;
		}

		it->second->bitmap20 = CBitmapFromFile( ::g_webUserHeadIconProcess.GetFaceFile(pFriend), 20, 20 );	
	
		if ( it->second->bitmap40 )
		{
			delete it->second->bitmap40;
		}

		it->second->bitmap40 = CBitmapFromFile( ::g_webUserHeadIconProcess.GetFaceFile(pFriend), 40, 40 );
	}
	else
	{
		tagBitmapData * pBitmapData = new tagBitmapData();

		pBitmapData->bitmap20 = CBitmapFromFile( ::g_webUserHeadIconProcess.GetFaceFile(pFriend), 20, 20 );
		pBitmapData->bitmap40 = CBitmapFromFile( ::g_webUserHeadIconProcess.GetFaceFile(pFriend), 40, 40 );
		
		mapUserBitmap.insert(map<uint32,tagBitmapData *>::value_type(pFriend->GetId(),pBitmapData));		
	}
}

CBitmap * GetUserBitmap(FRIEND_INFO * pFriend, bool bSmall)
{
	map<uint32,tagBitmapData *>::iterator it;

	it = mapUserBitmap.find(pFriend->GetId());

	CBitmap * bitmap20=NULL;
	CBitmap * bitmap40=NULL;

	if ( it!=mapUserBitmap.end() )
	{
		bitmap20 = it->second->bitmap20;
		bitmap40 = it->second->bitmap40;

		if ( bSmall )
		{
			return bitmap20;
		}
		else
		{
			return bitmap40;
		}
	}

	return NULL;
}

static map<uint32,tagBitmapData *>  mapClusterBitmap;

void ClearClusterBitmap()
{
	map<uint32,tagBitmapData *>::iterator it;

	for ( it=mapClusterBitmap.begin(); it!=mapClusterBitmap.end(); it++ )
	{
		if ( it->second->bitmap20 )
		{
			try
			{
				delete it->second->bitmap20;
			}
			catch(...)
			{
			}

			it->second->bitmap20=NULL;
		}

		if (it->second->bitmap40 )
		{
			try
			{
				delete it->second->bitmap40;
			}
			catch(...)
			{
			}
		}

		delete it->second;
	}

	mapClusterBitmap.clear();
}

bool MakeClusterBitmap(CLUSTER_INFO * pCluster)
{
	// 如果该好友没有定义头像则返回失败
	map<uint32,tagBitmapData *>::iterator it;
	it = mapClusterBitmap.find(pCluster->nFamilyID);

	CString sFace = g_webFamilyHeadIconProcess.GetFaceFile(pCluster);

	if ( FileExist(sFace) == FALSE )
	{
		return false;
	}

	if(it == mapClusterBitmap.end())
	{
		tagBitmapData * pBitmapData = new tagBitmapData();
		pBitmapData->bitmap20 = CBitmapFromFile( sFace, 20, 20 );
		pBitmapData->bitmap40 = CBitmapFromFile( sFace, 40, 40 );
		mapClusterBitmap.insert(map<uint32,tagBitmapData *>::value_type(pCluster->nFamilyID,pBitmapData));		
	}

	return true;
}

void UpdateClusterBitmap(CLUSTER_INFO * pCluster)
{
	map<uint32,tagBitmapData *>::iterator it;
	it = mapClusterBitmap.find(pCluster->nFamilyID);

	if ( it!=mapClusterBitmap.end() )
	{
		if ( it->second->bitmap20 )
		{
			delete it->second->bitmap20;
		}

		it->second->bitmap20 = CBitmapFromFile( ::g_webFamilyHeadIconProcess.GetFaceFile(pCluster), 20, 20 );	
	
		if ( it->second->bitmap40 )
		{
			delete it->second->bitmap40;
		}

		it->second->bitmap40 = CBitmapFromFile( ::g_webFamilyHeadIconProcess.GetFaceFile(pCluster), 40, 40 );
	}
	else
	{
		tagBitmapData * pBitmapData = new tagBitmapData();

		pBitmapData->bitmap20 = CBitmapFromFile( ::g_webFamilyHeadIconProcess.GetFaceFile(pCluster), 20, 20 );
		pBitmapData->bitmap40 = CBitmapFromFile( ::g_webFamilyHeadIconProcess.GetFaceFile(pCluster), 40, 40 );
		
		mapClusterBitmap.insert(map<uint32,tagBitmapData *>::value_type(pCluster->nFamilyID,pBitmapData));		
	}
}


CBitmap * GetClusterBitmap(CLUSTER_INFO * pCluster, bool bSmall)
{
	map<uint32,tagBitmapData *>::iterator it;

	it = mapClusterBitmap.find(pCluster->nFamilyID);

	CBitmap * bitmap20=NULL;
	CBitmap * bitmap40=NULL;

	if ( it!=mapClusterBitmap.end() )
	{
		bitmap20 = it->second->bitmap20;
		bitmap40 = it->second->bitmap40;

		if ( bSmall )
		{
			return bitmap20;
		}
		else
		{
			return bitmap40;
		}
	}

	return NULL;
}

map<int,CString>		g_mapVideoDriverIndex;//视频头驱动
map<uint16,uint16>      g_mapUserOper; //用户行为统计,map<operation id, command count>

void RegUserOperation(uint16 operId)//记录一下用户操作
{
	if ( g_ImUserInfo.GetId()==0 ) 
		return ;

	map<uint16,uint16>::iterator it = g_mapUserOper.find(operId);
	
	if ( it==g_mapUserOper.end() )
	{
		g_mapUserOper.insert(map<uint16,uint16>::value_type(operId,1));
	}
	else
	{
		it->second = it->second+1;
	}
}

void ClearAddressBookList(ADDRESS_BOOK_LIST &l)
{
	ADDRESS_BOOK_LIST::iterator i;
	for ( i=l.begin(); i!=l.end(); i++ )
	{
		delete (*i);
	}

	l.clear();
}

bool ValidateString(const CString &s, CString &error )
{
	if ( s.Find("\'")>-1 )
	{
		error = "不能有单引号";
		return false;
	}

	if( s.Find(" ")>-1 )
	{
		error = "不能有空格";
		return false;
	}

	if ( s.Find("\"")>-1 )
	{
		error = "不能有双引号";
		return false;
	}

	return true;
}
// 去掉字符串首尾的全角空白字符
void TrimSpace2(CString& sSrc)
{
	int i=0,j=0,nC=0;
	nC=sSrc.GetLength();
	if(nC==0)
		return;
	char* szBuf=new char[nC+1];
	const char* p=sSrc;
	char* pDest;
	const char c = (char)161;

	// 去掉末尾的全角空格

	p+=nC-1;
	for(i=nC-1;i>=0;i--)
	{
		if( ((*p)==c) && ((*(p-1))==c) )
		{
			nC-=2;
			p-=2;
			continue;
		}
		else
		{
			break;
		}
	}

	// 去掉首部的全角空格

	p = sSrc;
	pDest = szBuf;
	for(i=0;i<nC;i++)
	{
		if( ((*p)==c) && ((*(p+1))==c) )
		{
			i++;
			p+=2;
			continue;
		}
		else
		{
			*pDest++ = *p++;
		}
	}
	*pDest=0;
	sSrc = szBuf;
	delete [] szBuf;
}


void GetMD5FileName(const CString & sFilePathName,CString& sMD5Name)
{
	CString str1,str2,strExt;
	ExtractFilePath(sFilePathName,str1,str2,strExt);
	MD5_CTX md5T;
	DWORD dwUid=g_ImUserInfo.GetId();
	unsigned char digest[16];
	char szImageFilePathName[1024];
	sprintf(szImageFilePathName,sFilePathName);
	// 加密
	md5T.MD5Update ((unsigned char*)szImageFilePathName, sFilePathName.GetLength());
	md5T.MD5Final (digest);
	// 同用户id一起合并为文件名
	sMD5Name.Format("{%x-%x-%x-%x}%s",
		(((long *)digest))[0]^dwUid,
		(((long *)digest))[1]^dwUid,
		(((long *)digest))[2]^dwUid,
		(((long *)digest))[3]^dwUid,
		strExt);
}


void ExtractUrlPath(const CString & sUrl,CString & sUrlPath,CString & sFileExt)
{
	sUrlPath = "";
	sFileExt = "";
	int nLen = sUrl.GetLength();
	if(nLen == 0)
		return;
	const char * p = (LPCTSTR)sUrl;
	const char * pBeg = p;
	p += nLen;
	int nLeft;
	while(nLen-- > 0)
	{
		if(*p=='.')
		{
			nLeft = p - pBeg;
			if(nLeft > 0)
			{
				// 左边不为空则有url路径
				sUrlPath = sUrl.Left(nLeft);
			}
			// 右边为文件扩展名
			p++;
			sFileExt = p;
			return;
		}
		p--;
	}
	sUrlPath = sUrl;
}

void GetMD5WebFileName(const CString & sUrl,CString & sMD5Name)
{
	CString sUrlPath;
	CString sExt;
	ExtractUrlPath(sUrl,sUrlPath,sExt);
	MD5_CTX md5T;
	DWORD dwUid=g_ImUserInfo.GetId();
	unsigned char digest[16];
	char szImageFilePathName[1024];
	sprintf(szImageFilePathName,sUrl);
	// 加密
	md5T.MD5Update ((unsigned char*)szImageFilePathName, sUrl.GetLength());
	md5T.MD5Final (digest);
	// 同用户id一起合并为文件名
	sMD5Name.Format("{%x-%x-%x-%x}%s",
		(((long *)digest))[0]^dwUid,
		(((long *)digest))[1]^dwUid,
		(((long *)digest))[2]^dwUid,
		(((long *)digest))[3]^dwUid,
		sExt);
}

void WriteArticlePreviewFile(const TWebArticleInfo &info,CString strAuthor, CString strLocalFile)
{
	CString strHTML = ResLoadHtmlResource(IDR_HTML1);

	CString strDot  = GetModulePath()+"Image\\dot.gif";

	strHTML.Replace("VVVAAA-dot-AAAVVV",strDot);
	strHTML.Replace("VVVAAA-author-AAAVVV",strAuthor);
	strHTML.Replace("VVVAAA-link-AAAVVV",info.szUrl);
	strHTML.Replace("VVVAAA-content-AAAVVV",info.szOutlines);
	strHTML.Replace("VVVAAA-title-AAAVVV",info.szTitle);
	strHTML.Replace("VVVAAA-id-AAAVVV",IntToStr(info.uID));
	CString strDate = info.szModifyDate;
	strDate.Replace(" ","&nbsp;");
	strHTML.Replace("VVVAAA-date-AAAVVV",strDate);

	if ( access((LPCSTR)strLocalFile,0)!=-1 )
	{
		DeleteFile(strLocalFile);
	}

	CStdioFile f;

	if ( f.Open(strLocalFile,CFile::modeCreate|CFile::modeWrite|CFile::typeText) )
	{
		f.WriteString(strHTML);
		f.Close();
	}
}

static PROCESS_INFORMATION piUploadPicture;
static bool bUploadPictureRun=false;

void	ShowUploadPicDlg(char *username, char *nickname, char * password, uint32 uid)
{
	bool bCreatePrococess;

	if(g_pWebConfig==NULL)
	{
		return;
	}
	CString picurl = (LPCSTR)g_pWebConfig->GetWebInterfaceUrl(UPLOAD_PICTURE,L"");

	if ( bUploadPictureRun )//已经启动进程
	{
		CString strWindowCaption;
		strWindowCaption.Format("139uploadpicture_platform:%s",username);
		HWND hWnd = ::FindWindow(NULL,strWindowCaption);

		if ( hWnd==NULL )
		{
			TerminateProcess(piUploadPicture.hProcess,0);

			CloseHandle(piUploadPicture.hProcess);
			CloseHandle(piUploadPicture.hThread);

			piUploadPicture.hProcess=NULL;
			piUploadPicture.hThread=NULL;	
			bCreatePrococess = true;
		}
		else
		{
			::SetActiveWindow(hWnd);
			::BringWindowToTop(hWnd);
			::PostMessage(hWnd,WM_USER+1,0,0);
			bCreatePrococess = false;
		}
	}
	else
	{
		bCreatePrococess = true;
	}

	if ( bCreatePrococess )
	{
		STARTUPINFO si;

		CString strParamInfo = "";

		strParamInfo = strParamInfo + UIntToStr(g_ImUserInfo.GetId())+"&";
		strParamInfo = strParamInfo + CString(g_ImUserInfo.GetUserName().c_str())+"&";
		strParamInfo = strParamInfo + UrlEncode(g_ImUserInfo.nickname)+"&";
		strParamInfo = strParamInfo + UIntToStr(g_ImUserInfo.sex)+"&";
		strParamInfo = strParamInfo + UrlEncode(g_ImUserInfo.email)+"&";
		strParamInfo = strParamInfo + UIntToStr((int)g_ImUserInfo.gold_money)+"&";
		strParamInfo = strParamInfo + UrlEncode(g_ImUserInfo.face_url)+"&";
		strParamInfo = strParamInfo + IntToStr(g_ImUserInfo.message_count);

		strParamInfo = Base64Encode(strParamInfo);


		CString strKey    = "DJFU#(R#URJ#@Od#@$#@09809";
		CString strUserId = UIntToStr(g_ImUserInfo.GetId());
		CString strParamKey;

		int l = strUserId.GetLength();
		for ( int i=0; i<l; i++ )
		{
			strParamKey+=strUserId[i];
			strParamKey+=strKey[i];
		}

		strParamKey+=strKey.Right(strKey.GetLength()-l);

		unsigned char digest[16];

		MD5_CTX md5T;
		md5T.MD5Update ((unsigned char*)(LPCSTR)strParamKey,(uint32)strParamKey.GetLength() );
		md5T.MD5Final (digest);

		strParamKey = ToHexString(digest,16);


		CString strCmd;
		
		strCmd.Format("\"%suploadpicture.exe\" /\"(-<username>%s</username><nickname>%s</nickname><uid>%d</uid><mini_client>0</mini_client><picurl>%s</picurl><paraminfo>%s</paraminfo><paramkey>%s</paramkey>-)\"",
			GetModulePath(),
			username,
			nickname,
			uid,
			(LPCSTR)picurl,
			strParamInfo,
			strParamKey
			);

		memset(&si,0,sizeof(si));
		si.cb = sizeof(si);

		if ( CreateProcess( NULL, // No module name (use command line). 
								(char *)(LPCSTR)strCmd,
								NULL,             // Process handle not inheritable. 
								NULL,             // Thread handle not inheritable. 
								TRUE,            // Set handle inheritance to FALSE. 
								0,                // No creation flags. 
								NULL,             // Use parent's environment block. 
								NULL,             // Use parent's starting directory. 
								&si,              // Pointer to STARTUPINFO structure.
								&piUploadPicture)             // Pointer to PROCESS_INFORMATION structure.
						) 
		{
			bUploadPictureRun = true;
		}
		else
		{
			bUploadPictureRun = false;
		}
	}
}

void    CloseUploadPicDlg()
{	
	if ( bUploadPictureRun )
	{
		TerminateProcess(piUploadPicture.hProcess,0);
		CloseHandle(piUploadPicture.hProcess);
		CloseHandle(piUploadPicture.hThread);

		piUploadPicture.hProcess=NULL;
		piUploadPicture.hThread=NULL;		
	}

	bUploadPictureRun = false;
}

//初始化comboBox 查看权限数据
void  ComboViewInitProp(CComboBox *pCombo)
{
	pCombo->ResetContent();

	int i;
	
	i=pCombo->AddString("私有");//0
	pCombo->SetItemData(i,0);

	i=pCombo->AddString("好友可见");//1
	pCombo->SetItemData(i,1);

	//pCombo->AddString("139用户");//2
	i=pCombo->AddString("公开");//3
	pCombo->SetItemData(i,3);
}

void ComboViewSelProp(CComboBox *pCombo,int iViewProp)
{
	int n = pCombo->GetCount();

	for ( int i=0; i<n; i++ )
	{
		if ( pCombo->GetItemData(i)==iViewProp )
		{
			pCombo->SetCurSel(i);
			break;
		}

	}
}

int ComboViewGetProp(CComboBox *pCombo)
{
	int nSel = pCombo->GetCurSel();
	if ( nSel==-1 )
		return 0;
	else
		return pCombo->GetItemData(nSel);
}

void ClearWebVounchedFriends()
{
	ClearList(g_lsWebVounchFriends);
}

FRIEND_INFO * FindWebVounchedFriend(uint32 fid)
{
	FRIEND_LIST::iterator it;

	for ( it=g_lsWebVounchFriends.begin(); 
		it!=g_lsWebVounchFriends.end();
		it++ )
	{
		FRIEND_INFO *p = (*it);

		if ( p->GetId()==fid )
		{
			return p;
		}
	}

	return NULL;
}

bool compareP(const TWebPictureInfo * info1, const TWebPictureInfo * info2 )
{
	if ( info1->dwCreateDate>info2->dwCreateDate )
		return true;
	else
		return false;
}




CString WebDataFrom2Str(int iFrom)
{
	switch ( iFrom )
	{
	case 0:
		return "网站";
	case 1:
		return "WAP";
	case 2:
		return "短信";
	case 3:
		return "邮箱";
	case 4:
		return "哞哞";
	case 5:
		return "博客大赛";
	case 6:
	case 7:
		return "彩信";
	case 8:
		return "批量上传控件";
	case 9:
		return "哞哞 - 大头贴";
	}

	return "";
}

CString WebViewProp2Str(int iViewProp)
{
	switch ( iViewProp )
	{
		//0－自己，1－好友，2－注册用户，3－所有
	case 0:
		return "私有的,只能自己看";
	case 1:
		return "可以给好友看";
	case 2:
		return "139网站用户看";
	case 3:
		return "公开";
	}

	return "";
}

void SaveUserWebData()
{
	FRIEND_LIST::iterator it;

	for ( it=g_lsFriend.begin(); it!=g_lsFriend.end(); it++ )
	{
		FRIEND_INFO *p = (*it);
		/*if ( p->pUserWebArticleInfo )
		g_spaceData.SaveUserWebArticles(*p->pUserWebArticleInfo);

		if ( p->pUserWebPictureInfo )
		g_spaceData.SaveUserWebPictures(*p->pUserWebPictureInfo);*/
	}
}

void NotifyVideoEquipment(FRIEND_INFO * pFriend)
{
	XT_MSG msg;
	memset(&msg,0,sizeof(msg));

	msg.ver         = XTREAM_CLIENT_VERSION;
	msg.from_id     = g_ImUserInfo.GetId();

	msg.send_time   = /*time(0)*/g_LocalData.GetTimeNow();

	msg.to_id		= pFriend->GetId();
	msg.dest_ip		= pFriend->ip;
	msg.dest_port	= pFriend->port;
	msg.msg_type    = 0;

	msg.data_type = XT_MSG::VEDIO_EQUIPMENT;

	CSendPacket sp(msg.data);

	sp<<(uint8)g_mapVideoDriverIndex.size();

	msg.data_len = sp.GetSize();

	g_pProtocol->SendP2PMsg(msg);
}



/////////////////////////////////////////////////////////////////////////
/// 从资源文件中加载图片
/// @param [in] pImage 图片指针
/// @param [in] nResID 资源号
/// @param [in] lpTyp 资源类型
//////////////////////////////////////////////////////////////////////////
bool LoadImageFromResource(IN CImage* pImage,
                           IN UINT nResID, 
                           IN LPCSTR lpTyp)
{
    if ( pImage == NULL) return false;

    pImage->Destroy();

    // 查找资源
    HRSRC hRsrc = ::FindResource(AfxGetResourceHandle(), MAKEINTRESOURCE(nResID), lpTyp);
    if (hRsrc == NULL) return false;

    // 加载资源
    HGLOBAL hImgData = ::LoadResource(AfxGetResourceHandle(), hRsrc);
    if (hImgData == NULL)
    {
        ::FreeResource(hImgData);
        return false;
    }

    // 锁定内存中的指定资源
    LPVOID lpVoid    = ::LockResource(hImgData);

    LPSTREAM pStream = NULL;
    DWORD dwSize    = ::SizeofResource(AfxGetResourceHandle(), hRsrc);
    HGLOBAL hNew    = ::GlobalAlloc(GHND, dwSize);
    LPBYTE lpByte    = (LPBYTE)::GlobalLock(hNew);
    ::memcpy(lpByte, lpVoid, dwSize);

    // 解除内存中的指定资源
    ::GlobalUnlock(hNew);

    // 从指定内存创建流对象
    HRESULT ht = ::CreateStreamOnHGlobal(hNew, TRUE, &pStream);
    if ( ht != S_OK )
    {
        GlobalFree(hNew);
    }
    else
    {
        // 加载图片
        pImage->Load(pStream);
		
		if (pImage->GetBPP() == 32) //确认该图像包含Alpha通道
		{
			for (int i = 0; i < pImage->GetWidth(); i++)
			{
				for (int j = 0; j < pImage->GetHeight(); j++)
				{
					byte *pByte = (byte *)pImage->GetPixelAddress(i, j);
					pByte[0] = pByte[0] * pByte[3] / 255;
					pByte[1] = pByte[1] * pByte[3] / 255;
					pByte[2] = pByte[2] * pByte[3] / 255;
				}
			}
		}

        GlobalFree(hNew);
    }

    // 释放资源
    ::FreeResource(hImgData);

    return true;
}

bool ImageFromIDResource(Image * & pImg, UINT nID, LPCTSTR sTR)  
{  
    HINSTANCE hInst = AfxGetResourceHandle();  
    HRSRC hRsrc = ::FindResource (hInst,MAKEINTRESOURCE(nID),sTR); // type  
    if (!hRsrc)  
        return FALSE;  
    // load resource into memory  
    DWORD len = SizeofResource(hInst, hRsrc);  
    BYTE* lpRsrc = (BYTE*)LoadResource(hInst, hRsrc);  
    if (!lpRsrc)  
        return FALSE;  
    // Allocate global memory on which to create stream  
    HGLOBAL m_hMem = GlobalAlloc(GMEM_FIXED, len);  
    BYTE* pmem = (BYTE*)GlobalLock(m_hMem);  
    memcpy(pmem,lpRsrc,len);  
    IStream* pstm;  
    CreateStreamOnHGlobal(m_hMem,FALSE,&pstm);  
    // load from stream  
    pImg=Gdiplus::Image::FromStream(pstm);  
    // free/release stuff  
    GlobalUnlock(m_hMem);  
    pstm->Release();  
    FreeResource(lpRsrc);  
    return TRUE;  
}  

#ifdef _BUSINESS_VESION
// 判断组ID是否属于我的团队分组
BOOL CheckSubGroupIsInMyTeam(int nGroupIndex)
{
	// 查找“我的团队”分组下所有的子分组
	if (nGroupIndex == MY_TEAM_GROUP_ID)
	{
		return TRUE;
	}

	int nIndex = g_GroupInfo.FindItemIndex(nGroupIndex);
	if (nIndex != -1)
	{
		int nFatherGroupID = g_GroupInfo.group[nIndex].group_father;
		while (nFatherGroupID != -1)
		{
			if (nFatherGroupID == MY_TEAM_GROUP_ID)
			{
				return TRUE;
			}

			nIndex = g_GroupInfo.FindItemIndex(nFatherGroupID);
			if (nIndex == -1)
			{
				break;
			}
			nFatherGroupID = g_GroupInfo.group[nIndex].group_father;
		}
	}

	return FALSE;
}
#endif

// 根据好友用户名查找节点
FRIEND_INFO * FindFriend(const char *szUserName)
{
	FRIEND_LIST::iterator it;
	for (it = g_lsFriend.begin(); it != g_lsFriend.end(); it++)
	{
		if (strcmp((*it)->GetUserName().c_str(), szUserName) == 0)
		{
			return (*it);
		}
	}

	return NULL;
}

// 获取陌生人数量
int GetStrangerCount()
{
	int nCount = 0;
	FRIEND_LIST::iterator it;
	for (it = g_lsFriend.begin(); it != g_lsFriend.end(); it++)
	{	
		if( (*it)->group_index == 1 )
		{
			nCount++;
		}
	}

	return nCount;
}

// 获取所有系统字体名
std::vector<std::string>* GetAllSysFontName()
{
	if(g_vecSysFontName.empty())
	{
		HDC hdc = GetDC(GetDesktopWindow());
		::EnumFontFamilies(hdc, (LPTSTR) NULL, 
			(FONTENUMPROC)EnumFontNameProc, (LPARAM)&(g_vecSysFontName));

		ReleaseDC(GetDesktopWindow(), hdc);

		// 将常用字体放前面
		BOOL bFindST = FALSE;				// 系统字体中是否有宋体
		BOOL bFindWRYH = FALSE;				// 系统字体中是否有微软雅黑
		BOOL bFindXST = FALSE;				// 系统字体中是否有新宋体
		BOOL bFindYY = FALSE;				// 系统字体中是否有幼圆
		vector<string> vecChiFont;			// 系统中所有中文字体
		vector<string> vecEngFont;			// 系统中所有英文字体
		vector<string> vecHFont;			// 系统中所有横放的字体
		for(size_t i = 0; i != g_vecSysFontName.size(); ++i)
		{
			string strFont = g_vecSysFontName[i];
			if(strFont[0] < 0 || strFont[0] > 127)
			{	// 中文
				if(!bFindST && strFont == "宋体")
				{
					bFindST = TRUE;
				}
				else if(!bFindWRYH && strFont == "微软雅黑")
				{
					bFindWRYH = TRUE;
				}
				else if(!bFindXST && strFont == "新宋体")
				{
					bFindXST = TRUE;
				}
				else if(!bFindYY && strFont == "幼圆")
				{
					bFindYY = TRUE;
				}
				else
				{
					vecChiFont.push_back(strFont);
				}
			}
			else
			{	// 非中文
				if((strFont[0] >= 'a' && strFont[0] <= 'z')
					|| (strFont[0] >= 'A' && strFont[0] <= 'Z'))
				{
					vecEngFont.push_back(strFont);
				}
				else
				{	// 横排字体
					vecHFont.push_back(strFont);
				}
			}
		}

		// 添加排序后的字体
		g_vecSysFontName.clear();

		if(bFindST)
		{
			g_vecSysFontName.push_back("宋体");
		}
		if(bFindWRYH)
		{
			g_vecSysFontName.push_back("微软雅黑");
		}
		if(bFindXST)
		{
			g_vecSysFontName.push_back("新宋体");
		}
		if(bFindYY)
		{
			g_vecSysFontName.push_back("幼圆");
		}
		g_vecSysFontName.insert(g_vecSysFontName.end(), vecChiFont.begin(), vecChiFont.end());
		g_vecSysFontName.insert(g_vecSysFontName.end(), vecEngFont.begin(), vecEngFont.end());
		g_vecSysFontName.insert(g_vecSysFontName.end(), vecHFont.begin(), vecHFont.end());
	}

	return &g_vecSysFontName;
}


/*查找字符字串*/
// szSub为空字符串时恒为真
const char* SubstrExist(const char* s,int nLen,const char* szSub)
{
	if(nLen<0)
		nLen=strlen(s);
	const char* p=s;
	const char* pStrBeg=s;
	const char* pSub=szSub;
	const char* pEnd=s+nLen;
	while(*p)
	{
		if(*pSub==0)
			return pStrBeg;
		if(*p != *pSub)
		{
			// 继续匹配下个字符开始的子串
			p=++pStrBeg;
			pSub=szSub;
			continue;
		}
		p++;
		pSub++;
		if(p>=pEnd)
		{
			break;
		}
	}
	// 如果待查询的子串刚好结束则表明刚好匹配
	if(*pSub==0)
		return pStrBeg;
	return NULL;
}

// 自动接待
unsigned static __stdcall ThreadAutoAccept(void * pParam)
{
	Sleep(200);
	uint32 toId = (uint32)pParam;

	AUTO_REPLY acceptMsg;
	/*if(g_ImUserInfo.status != FRIEND_INFO::ONLINE)
	{
		g_LocalData.GetAutoAccept_Offline(acceptMsg);
	}
	else
	{*/
		g_LocalData.GetAutoAccept_Online(acceptMsg);
	//}

	XT_MSG msg;
	memset(&msg,0,sizeof(msg));
	msg.ver         = XTREAM_CLIENT_VERSION;
	msg.from_id     = g_ImUserInfo.GetId();
	msg.fontColor   = acceptMsg.font.fontColor;
	msg.fontSize    = acceptMsg.font.fontSize;
	msg.fontStyle   = acceptMsg.font.flags;
	strcpy(msg.fontName,acceptMsg.font.fontName);
	strcpy(msg.from_nickname,g_ImUserInfo.nickname);

	msg.send_time   = /*time(0)*/g_LocalData.GetTimeNow();

	msg.to_id		= toId;
	msg.msg_type    = 0;		

	//msg.data_type   = 0;//即时聊天消息
	msg.data_type   = XT_MSG::AUTO_REPLY;	// 自动接待消息

	CString text(acceptMsg.strText);
#ifdef _CS_PLAT_EX_
	if(text.IsEmpty())
	{	
		text.Format("您好，飞牛客服\"%s\"很高兴为您服务。", g_ImUserInfo.GetUserName().c_str());
	}
#else
	if ( text.IsEmpty() )
	{
		return 0;
	}
#endif

	if ( FindForbiddenWords(text) )
	{
		return 0;
	}

	strcpy(msg.data,text);
	msg.data_len=strlen(text)+1;

	int  type = CIMProtocol::SMT_SERVER;
	g_pProtocol->SendP2PMsg(msg,type);

	return 0;
}

// 自动接待
void AutoAccept(uint32 friendId)
{
	unsigned threadID;
	_beginthreadex(NULL, 0, ThreadAutoAccept, (void*)friendId, 0, &threadID);
}
#ifdef _PRESS_TEST
unsigned static __stdcall ThreadAutoAcceptPress(void * pParam)
{
	Sleep(200);
	XT_MSG *pMsg = (XT_MSG*)pParam;
	AUTO_REPLY acceptMsg;
		g_LocalData.GetAutoAccept_Online(acceptMsg);
	XT_MSG msg;
	memset(&msg,0,sizeof(msg));
	msg.ver         = XTREAM_CLIENT_VERSION;
	msg.from_id     = g_ImUserInfo.GetId();
	msg.fontColor   = g_ImFont.fontColor;
	msg.fontSize    = g_ImFont.fontSize;
	msg.fontStyle   = g_ImFont.flags;
	strcpy(msg.fontName, g_ImFont.fontName);
	strcpy(msg.from_nickname,g_ImUserInfo.nickname);
	msg.send_time   = /*time(0)*/g_LocalData.GetTimeNow();
	msg.to_id		= pMsg->from_id;
	msg.msg_type    = 0;		
	msg.data_type   = 0;//即时聊天消息
	CString text(pMsg->data);
	if ( text.IsEmpty() )
	{
		delete pMsg;
		return 0;
	}
	if ( FindForbiddenWords(text) )
	{
		delete pMsg;
		return 0;
	}
	strcpy(msg.data,text);
	msg.data_len=strlen(text)+1;
	int  type = CIMProtocol::SMT_SERVER;
	g_pProtocol->SendP2PMsg(msg,type);
	delete pMsg;
	return 0;
}
void AutoAcceptPress(XT_MSG stMsg)
{
	unsigned threadID;
	XT_MSG *pMsg = new XT_MSG(stMsg);
	_beginthreadex(NULL, 0, ThreadAutoAcceptPress, (void*)pMsg, 0, &threadID);
}

#endif

void UpdateUserFullInfo(XT_USER_INFO* pUserInfo, XT_USER_INFO* ack )
{
	//guid 不被覆盖
	//CString sCommName = pUserInfo->comment_name;
	string strGuid = pUserInfo->GetGuid();
	//memcpy(pUserInfo, (XT_USER_INFO *)ack, sizeof(XT_USER_INFO) - MAX_GUID_LENGTH - 1);
	memcpy(pUserInfo, (XT_USER_INFO *)ack, sizeof(XT_USER_INFO));
	pUserInfo->SetGuid(strGuid.c_str());
	strGuid = pUserInfo->GetGuid();
	//strncpy(pUserInfo->comment_name, sCommName, sizeof(pUserInfo->comment_name) - 1);
}

/*
等级计算公式：
time = (N`2+4*N)*a
a=18
其中N为当前用户等级，time为该等级需要的时间

则当前在线时长time可以计算等级 N = -2 + (√￣(16+time*(4/a)))/2 
*/
int CountLevel(int nTime,int&nNextDelta)
{
	int nLevel;
	float fLevel;
	int nNextTime;	// 到下一等级需要的时间
	fLevel=-2.0+sqrt(16.0+nTime*(4.0/18))/2.0;
	nLevel=fLevel;
	if(nLevel==0)
	{
		nLevel=1;
	}
	if(fLevel >= nLevel)
	{
		nLevel++;
	}
	nNextTime=nLevel*nLevel+4*nLevel;
	nNextTime *= 18;
	nNextDelta=nNextTime-nTime;
	return nLevel;
}

CString GetPicRandomUrl()
{
#if (defined _VER_PREVIEW_) || (defined _VER_ONLINE_)
	int nRand = rand()%2;
	if(nRand == 0)
		return PICTURE_URL;
	if(nRand == 1)
		return PICTURE_URL2;
	if(nRand == 2)
		return PICTURE_URL3;
#endif
	return PICTURE_URL;
}