#pragma once

#include "SyncCritical.h"

/************************************************************************ 
文件描述：自动ID分配类的声明文件 

创建人：hechang
时间：	2006-7-4 
************************************************************************/


//用于分配一个唯一ID

class CAutoID
{
public:
	CAutoID(void);
	~CAutoID(void);

	//得到一个新的ID
	int GetNewID();

	//取消一个使用的ID
	void DeleteID(int nOldID);

private:
	int		  m_nMaxIDNum;			//使用的ID数
	list<int> m_listBackup;			//回收的ID列表

	CSyncCritical	m_SyncLock;			//同步锁
};

