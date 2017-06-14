#ifndef _SPACE_TYPE_DEF_H_
#define _SPACE_TYPE_DEF_H_

#include <afxtempl.h>
#include <afxmt.h>
#include <vector>
#include <map>
#include <algorithm>
using namespace std;

#pragma warning (disable:4267)

//图片放映缩略图尺寸
#define PICTURE_CX				80
#define PICTURE_CY				80
#define FRAME_SZ				85
#define PICTURE_SPACE			10


#define MAX_FOCUSED_ARTICLES    250  //关注文章最大数目
#define MAX_FOCUSED_PICTURES	150  //最多关注图片
#define	MAX_ARTICLE_TITLE_LEN	101

#define MAX_USERNAME_LEN			120				//用户名
#define MAX_USERNAME_LEN_LAST       40              //用户名原长度40，用于兼容上一个版本读取用户名文件
#define MAX_NICKNAME_LEN			40				//呢称
#define MAX_HEADURL_LEN				100				//自定义头像图片URL
#define MAX_EMAIL_LEN				60				//Email

#define WM_MODULE_NOTIFY   WM_USER+0x2001 
//模块ID定义
#define MODULE_MainProgram			0x00
#define MODULE_MyArticles			0x01
#define MODULE_WriteArticle			0x02
#define MODULE_HeadPhoto			0x03
#define MODULE_MyPictures           0x04
#define MODULE_UserPictures		    0x05
#define MODULE_UserArticles         0x06
#define MODULE_MiniPictureBroadCast 0x07

typedef unsigned char    uint8;
typedef unsigned short   uint16;
typedef unsigned int     uint32;
typedef unsigned __int64 uint64;

struct TTagInfo
{
	long nTagId;
	char szTagName[128];
};

struct TBookInfo
{
	long nBookId;
	int  nProp;//0不公开，3公开
	char szBookName[128];
	char szCreateDate[45];
};

struct TAlbumInfo
{
	long nAlbumId;
	char szAlbumName[255];
	char szCreateDate[45];
};


struct TWebNotificationInfo
{
	int        type;
	int		   sub_type;
	uint32	   notification_id;
	char	   szUserName[64];
	char	   szNickName[64];
	uint32     user_id;
	uint32     log_id;//博客内容ID(文章、图片、视频ID)
	char	   szUrl[128];
	char	   szTitle[256];
	char       szContent[1024];
	char	   szCreateDate[45];
	int        is_read;//0 not read, 1 read
	DWORD	   dwCreateDate;

	TWebNotificationInfo()
	{
		memset(this,0,sizeof(TWebNotificationInfo));
	}
};


struct TUserBookInfo
{
	uint32 uID;

	vector<TBookInfo> vecBook;

	TUserBookInfo()
	{
		uID=0;
	}
};

struct TUserAlbumInfo
{
	uint32 uID;

	vector<TAlbumInfo> vecAlbum;

	TUserAlbumInfo()
	{
		uID=0;
	}
};

struct TArticleBookInfo
{
	uint32 uArticleId;

	vector<TBookInfo> vecBook;

	TArticleBookInfo()
	{
		uArticleId=0;
	}
};

struct TPicturelbumInfo
{
	uint32 uPictureId;

	vector<TAlbumInfo> vecAlbum;

	TPicturelbumInfo()
	{
		uPictureId=0;
	}
};


struct TLocalSavedArticleInfo
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
} ;

struct TSavedArticleItem
{
	char    szTitle[MAX_ARTICLE_TITLE_LEN+1];
	DWORD   dwSavedID;
};

// 个人空间 - 文章设置
struct TZoneSetting
{
	char    szBackupPath[255];//备份目录

};


struct TWebPictureInfo
{
	uint32       uID;//属于用户的ID
	DWORD        dwPictureID;
	DWORD        dwFrom;/*0=web,1=wap,2=彩信或彩E,3=邮箱，4＝Mini发表*/
	DWORD        dwViewType;/*0－自己（私有），1－好友，2－注册用户(保留，但不用)，3－所有（公开）4－密码访问（保留，但不用）*/
	int          iWidth;//图片像素
	int          iHeight;
	long		 nAlbumId;
	char         szTitle[128];
	char         szCreateDate[45];
	char         szUrl[255];
	DWORD		 dwCreateDate;
	DWORD		 dwData;

	static 	bool ComparePictureDate(const TWebPictureInfo *p1, const TWebPictureInfo *p2)
	{
		if (p1->dwCreateDate>p2->dwCreateDate)
			return true;
		else
			return false;
	}
} ;

struct TUserWebPictureInfo
{
	uint32     uID;
	char       szLastUpdateDate[45];
	int        isRead;//1 read, 0 not read
	//DWORD      dwData ;

	vector<TWebPictureInfo *>  vecPics;

	void Lock() { m_sec.Lock();
	}

	void Unlock() { m_sec.Unlock();
	}

	int FindPicture(int nPictureID)
	{
		int n= vecPics.size();
		for (int i=0; i<n; i++ )
		{
			if ( vecPics[i]->dwPictureID==nPictureID )
				return i;
		}

		return -1;
	}

	bool Push(TWebPictureInfo *p,bool bLock=true)
	{
		if ( bLock )
			m_sec.Lock();

		int n = vecPics.size();
		n--;
		while (n>=0)
		{
			if (vecPics[n]->dwPictureID==p->dwPictureID )
			{
				if ( vecPics[n]->dwCreateDate==p->dwCreateDate  )
				{
					if ( bLock ) 
						m_sec.Unlock();
					return false;
				}
				else
				{
					memcpy(vecPics[n],p,sizeof(TWebPictureInfo));
					if ( bLock ) 
						m_sec.Unlock();
					return true;
				}

			}
			n--;
		}

		vecPics.push_back(p);

		if ( bLock )
			m_sec.Unlock();

		return true;
	}

	TUserWebPictureInfo(uint32 userID)
	{
		uID=userID;
		//	dwData = 0;
		isRead = 1;
		memset(szLastUpdateDate,0,sizeof(szLastUpdateDate));
	}

	~TUserWebPictureInfo()
	{
		int n=vecPics.size();

		for ( int i=0; i<n; i++ )
		{
			delete vecPics[i];
		}

		vecPics.clear();
	}

private:
	TUserWebPictureInfo();
	CCriticalSection m_sec;
};


struct TWebArticleInfo
{
	uint32     uID;
	DWORD      dwArticleID;
	DWORD      dwFrom;
	DWORD      dwViewType;
	DWORD      dwTopFlag;
	DWORD      dwDirID   ;
	int        iState    ;//0 new, 1 read
	char	   szOutlines[1025];
	char       szCreateDate[45];
	char       szModifyDate[45];
	char       szUrl[256];
	char       szTitle[MAX_ARTICLE_TITLE_LEN];
	DWORD	   dwModifyDate;//不写入xml
	DWORD      dwCreateDate;//不写入xml
	DWORD      dwData;

	static 	bool CompareArticleDate(const TWebArticleInfo *p1, const TWebArticleInfo *p2)
	{
		if (p1->dwCreateDate>p2->dwCreateDate)
			return true;
		else
			return false;
	}
};


struct TUserWebArticleInfo
{
	uint32    uID;
	int       isRead;//1 read ,0 not read
	char      szLastUpdateDate[45];

	vector<TWebArticleInfo *> vecArticles;

	void Lock() { m_sec.Lock();
	}

	void Unlock() { m_sec.Unlock();
	}

	int FindArticle(int nArticleID)
	{
		int n= vecArticles.size();
		for (int i=0; i<n; i++ )
		{
			if ( vecArticles[i]->dwArticleID==nArticleID )
				return i;
		}

		return -1;
	}

	bool Push(TWebArticleInfo *p, bool bLock=true)
	{
		if ( bLock )
			m_sec.Lock();

		int n = vecArticles.size();
		n--;
		while(n>=0)
		{
			if ( p->dwArticleID==vecArticles[n]->dwArticleID )
			{
				if ( p->dwModifyDate==vecArticles[n]->dwModifyDate )
				{
					if ( bLock ) m_sec.Unlock();
					return false;
				}
				else
				{
					memcpy(vecArticles[n],p,sizeof(TWebArticleInfo));
					if ( bLock ) m_sec.Unlock();
					return true;
				}

			}

			n--;
		}
		vecArticles.push_back(p);
		if ( bLock ) m_sec.Unlock();
		return true;
	}

	TUserWebArticleInfo(uint32 userID)
	{
		uID		= userID;
		isRead  = 1;
		memset(szLastUpdateDate,0,sizeof(szLastUpdateDate));
	}

	~TUserWebArticleInfo()
	{
		int n=vecArticles.size();
		for ( int i=0; i<n; i++ )
			delete vecArticles[i];
		vecArticles.clear();
	}

private:
	TUserWebArticleInfo();
	CCriticalSection m_sec;
};

struct TWebAudioInfo
{
	uint32     uID;
	DWORD      dwAudioID;
	char	   szLyric[1025];
	char       szSpecial[45];
	char       szSinger[45];
	char       szName[45];
	char       szCreateDate[45];
	char       szUrl[256];
	DWORD	   dwViewType;
	DWORD      dwCreateDate;//不写入xml

	static 	bool CompareAudioDate(const TWebAudioInfo *p1, const TWebAudioInfo *p2)
	{
		if (p1->dwCreateDate>p2->dwCreateDate)
			return true;
		else
			return false;
	}
};

struct TUserInfo
{
	uint32  id           ;	// 0 用户id
	char    username[MAX_USERNAME_LEN+1] ;	// 3 用户名
	char    nickname[MAX_NICKNAME_LEN+1] ;
};

typedef CList<TUserInfo,TUserInfo> TUserList;
typedef CList<TSavedArticleItem,TSavedArticleItem> TSavedArticleList;//未发表的文章列表

#endif