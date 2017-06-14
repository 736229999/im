#include "stdafx.h"
#include "FileTransport.h"

//CTransFileBuf超时时间,十分钟
#define TRANSFILE_OVERTIME		(10*60*1000)

CZObjectPool<CFilePacket>		CTransFileBuf::m_PacketPool;

CTransFileBuf::CTransFileBuf()
{
	memset(m_PacketArray,0,sizeof(m_PacketArray));
	m_PacketSize = 0;
	m_TimeTick = 0;
	m_pre = NULL;
	m_next= NULL;
}

CTransFileBuf::~CTransFileBuf()
{
	ReleasePacket();
}

void CTransFileBuf::Init(FID fid,int nFileSize,uint32 nFamilyId,int UserCount)
{
	m_nFileSize = nFileSize;
	m_nUserCount=UserCount;
	memset(m_PacketArray,0,sizeof(m_PacketArray));
	m_PacketSize = 0;
	m_TimeTick = ::timeGetTime();
	m_fid = fid;
	m_FamilyId = nFamilyId;

}


bool CTransFileBuf::IsOverTime()
{
	return (::timeGetTime() - m_TimeTick) >  TRANSFILE_OVERTIME ;
}

bool CTransFileBuf::AddPacket(const char* pPacket,int nSize,int nPos)
{
	if( m_PacketSize >= MAX_PACKETS_IN_ONEFILE )
	{
		return false;
	}

	CFilePacket* pFilePacket = m_PacketPool.AllocateObj();
	pFilePacket->size = nSize;
	memcpy(pFilePacket->data,pPacket,nSize);

	++m_PacketSize;
	m_PacketArray[nPos] = pFilePacket;
	return true;

}


void CTransFileBuf::ReleasePacket()
{
	for(int i = 0; i <MAX_PACKETS_IN_ONEFILE;++i)
	{
		if(m_PacketArray[i])
		{
			m_PacketPool.ReleaseObj(m_PacketArray[i]);
			m_PacketArray[i] = NULL;
		}

	}

	m_PacketSize = 0;
	m_nFileSize	 = 0;

}


CFilePacket* CTransFileBuf::GetPacket(int nPos)
{
	if( nPos <0 || nPos >= m_PacketSize )
	{
		return NULL;
	}

	return m_PacketArray[nPos];
}


//////////////////////////////////////////////////////////////////////////



CTransFileBufManager::CTransFileBufManager()
{

}

CTransFileBufManager::~CTransFileBufManager()
{

}

CTransFileBuf* CTransFileBufManager::AddTransFile(FID* pFid)
{
	CTransFileBuf* pFileBuf = NULL;
	map<FID,CTransFileBuf*>::iterator it = m_mapTransFile.find(*pFid);
	if( it != m_mapTransFile.end())
	{
		pFileBuf = (*it).second;
		if( pFileBuf )
		{
			return NULL;
		}
	}

	pFileBuf = m_FileBufPool.AllocateObj();
	if( NULL == pFileBuf)
	{
		return NULL;
	}

	m_mapTransFile[*pFid] =pFileBuf;


	m_FileListLock.Lock();

	pFileBuf->m_pre = NULL;
	pFileBuf->m_next= NULL;
	if( m_pTransFileHead)
	{
		m_pTransFileHead->m_pre = pFileBuf;
		pFileBuf->m_next = m_pTransFileHead;
		m_pTransFileHead = pFileBuf;

		//顺便检查最后一个filebuf是否过期,避免队列中有过期的filebuf存在
		if(m_pTransFileTail)
		{

			if( m_pTransFileTail->IsOverTime() )
			{
				CTransFileBuf* pNewTail = m_pTransFileTail->m_pre;

				if( m_pTransFileHead == m_pTransFileTail )
				{
					m_pTransFileHead = NULL;
				}

				m_mapTransFile.erase(m_pTransFileTail->GetFid());
				m_pTransFileTail->ReleasePacket();
				m_FileBufPool.ReleaseObj(m_pTransFileTail);

				m_pTransFileTail = pNewTail;

				if( m_pTransFileTail )
				{
					m_pTransFileTail->m_next  = NULL;
				}
			}
		}
	}
	else
	{
		m_pTransFileTail = pFileBuf;
		m_pTransFileHead = pFileBuf;
	}



	m_FileListLock.Unlock();

	return pFileBuf;
}

CTransFileBuf* CTransFileBufManager::Find(FID* pFid)
{
	map<FID,CTransFileBuf*>::iterator it=m_mapTransFile.find(*pFid);

	if(  it == m_mapTransFile.end())
	{
		return NULL;
	}

	return (*it).second;
}


bool CTransFileBufManager::DelTransFile(FID* pFid)
{
	map<FID,CTransFileBuf*>::iterator it = m_mapTransFile.find(*pFid);

	if( it == m_mapTransFile.end())
	{
		return false;
	}

	CTransFileBuf* pFileBuf = (*it).second;

	if( NULL == pFileBuf)
	{
		return false;
	}



	m_FileListLock.Lock();

	if( pFileBuf->m_pre )
	{
		pFileBuf->m_pre = pFileBuf->m_next;
	}

	if( pFileBuf->m_next)
	{
		pFileBuf->m_next = pFileBuf->m_pre;
	}

	if( m_pTransFileHead == pFileBuf)
	{
		m_pTransFileHead = pFileBuf->m_next;
	}

	if( m_pTransFileTail == pFileBuf )
	{
		m_pTransFileTail = pFileBuf->m_pre;
	}

	m_FileListLock.Unlock();

	m_mapTransFile.erase(*pFid);
	pFileBuf->ReleasePacket();
	m_FileBufPool.ReleaseObj(pFileBuf);

	return true;
}

