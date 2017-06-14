#ifndef _SPACE_DATA_H_
#define _SPACE_DATA_H_

#include "SpaceTypeDef.h"

class CSpaceData 
{
public:
	CSpaceData();
	virtual ~CSpaceData();

public:
	void      SetUserInfo(CString strUserName,long nUserID);
	void	  SetPath(CString strPath);
	CString & GetPath();

	CString GetUserWebPicturePath(uint32 friend_id);
	void LoadUserWebPictures(uint32 uid,TUserWebPictureInfo &info);
	void SaveUserWebPictures(const TUserWebPictureInfo &info);

	void GetZoneSetting(TZoneSetting &setting);

	CString GetUserWebArticlePath(uint32 friend_id);
	void LoadUserWebArticles(uint32 uid,TUserWebArticleInfo &info);
	void SaveUserWebArticles(const TUserWebArticleInfo &info);

	CString GetMySavedArticlePath();
	void LoadMySavedArticleList(TSavedArticleList &articles);
	void DeleteSavedArticle(DWORD dwSavedID);
	void UpdateSavedArticle(TSavedArticleItem &item);
	bool LoadMySavedArticle(TLocalSavedArticleInfo &info,DWORD dwSavedID);
	void SaveMySavedArticle(const TLocalSavedArticleInfo &info,DWORD dwSaveID);
	void SaveHistoryFaceUrl(CString strUrl);

private:
	CString			m_strPath;			// 本地数据保存的路径
	CString         m_strUserName;
	long			m_nUserID;
};

#endif