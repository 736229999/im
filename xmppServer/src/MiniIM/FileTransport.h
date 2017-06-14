#pragma once

#include "Utility.h"
#include "ZObjectPool.h"
#include "SyncCritical.h"
#include "ClientProtocol.h"


//文件，图片传输管理

//把文件拆分后的包
class CFilePacket
{
public:
	BYTE data[CPIC_PACKET_LEN];
	int	 size;

};

struct FID{
	UINT32 sid;
	UINT32 fid;
	bool operator < (FID r)
	{
		return (sid < r.sid || ((sid == r.sid) && fid < r.fid));
	}

	bool  operator < ( const FID r) const
	{
		return (sid < r.sid || ((sid == r.sid) && fid < r.fid));
	} 

}; 


#define MAX_PACKETS_IN_ONEFILE		2000

//传输的文件缓存
class  CTransFileBuf
{
public:
	CTransFileBuf();
	~CTransFileBuf();
	void Init(FID fid,int nFileSize,uint32 nFamilyId,int UserCount);

	bool AddPacket(const char* pPacket,int nSize,int nPos );

	void ReleasePacket();

	CFilePacket* GetPacket(int nPos);

	void ReleaseUser() { --m_nUserCount;	}
	bool IsNoneUser() { return m_nUserCount <= 0 ;	}

	bool IsOverTime();

	FID GetFid() { return m_fid;	}

	uint32 GetFamilyId() {return m_FamilyId;	}


private:
	FID				m_fid;				//文件ID信息
	uint32			m_FamilyId;			//文件所属家族id

	int				m_nUserCount;		//需要该文件缓存的用户数
	CFilePacket*	m_PacketArray[MAX_PACKETS_IN_ONEFILE];		//文件包缓存
	int				m_nFileSize;		//文件的真实大小

	int				m_PacketSize;		//当前Packet的数量

	int				m_TimeTick;			//文件缓存生成的时间

	static  CZObjectPool<CFilePacket>		m_PacketPool;

public:
	CTransFileBuf*	m_pre;
	CTransFileBuf*	m_next;

};


class CTransFileBufManager
{
public:
	CTransFileBufManager();
	~CTransFileBufManager();

	 CTransFileBuf* AddTransFile(FID* pFid);
	 CTransFileBuf* Find(FID* pFid);
	 bool DelTransFile(FID* pFid);


private:
	map<FID,CTransFileBuf*>		m_mapTransFile;

	CZObjectPool<CTransFileBuf>	m_FileBufPool;

	CTransFileBuf*					m_pTransFileHead;
	CTransFileBuf*					m_pTransFileTail;
	CSyncCritical					m_FileListLock;

};

