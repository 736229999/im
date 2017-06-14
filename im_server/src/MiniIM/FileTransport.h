#pragma once

#include "Utility.h"
#include "ZObjectPool.h"
#include "SyncCritical.h"
#include "ClientProtocol.h"


//�ļ���ͼƬ�������

//���ļ���ֺ�İ�
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

//������ļ�����
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
	FID				m_fid;				//�ļ�ID��Ϣ
	uint32			m_FamilyId;			//�ļ���������id

	int				m_nUserCount;		//��Ҫ���ļ�������û���
	CFilePacket*	m_PacketArray[MAX_PACKETS_IN_ONEFILE];		//�ļ�������
	int				m_nFileSize;		//�ļ�����ʵ��С

	int				m_PacketSize;		//��ǰPacket������

	int				m_TimeTick;			//�ļ��������ɵ�ʱ��

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

