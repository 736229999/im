#pragma once

#include "ZObjectPool.h"

/************************************************************************ 
�ļ�����������������������ļ� 

�����ˣ�hechang
ʱ�䣺	2006-7-5 
************************************************************************/

//��������,�����Ԫ�ذ���ID����,�ڲ����ö��ַ�ʵ�ֿ��ٲ���,ͬʱ֧������ı���

template <class T>
class CSortArray
{
public:
	CSortArray(int nSize)
	{
		assert(nSize>0);

		m_nArraySize	= 0;
		m_nMaxArraySize = nSize;
		m_pArrayData	= new SortArrayItem[nSize];
	}



	~CSortArray(void)
	{
		if( m_pArrayData )
		{
			delete[] m_pArrayData;
		}
	}


	//���һ��Ԫ��,����Ѿ�����,����NULL
	T* Add(int nID)
	{
		if(m_nArraySize == m_nMaxArraySize )
		{
			return NULL;
		}

		if(m_nArraySize == 0 )
		{
			m_pArrayData[0].id = nID;
			m_pArrayData[0].pData = m_ObjectPool.AllocateObj();
			++m_nArraySize;
			return m_pArrayData[0].pData;
		}

		int nHeadPos = 0;
		int nTailPos = m_nArraySize-1;
		int nMidPos = 0;

		while(nHeadPos < nTailPos)
		{
			nMidPos = (nHeadPos + nTailPos) / 2;
			if( m_pArrayData[nMidPos].id > nID )
			{
				nTailPos = nMidPos - 1;
			}
			else if( m_pArrayData[nMidPos].id < nID )
			{
				nHeadPos = nMidPos + 1;
			}
			else
			{
				//�Ѿ����ڸ�Ԫ��
				return NULL;
			}
		}


		//�ҵ�Ҫ�����λ���ˣ�����ǲ嵱ǰλ�û��Ǻ���
		nMidPos = (nHeadPos + nTailPos) / 2;
		if(m_pArrayData[nMidPos].id == nID )
		{
			//�Ѿ����ڸ�Ԫ��
			return NULL;
		}


		int nPushBackPos = 0;
		int nPushLength = 0 ;
		nPushBackPos = m_pArrayData[nMidPos].id < nID ? 
			nMidPos + 1 : nMidPos;
		nPushLength	 = m_nArraySize - nPushBackPos;

		if( nPushLength > 0 )
		{
			//�Ѳ�����Ԫ��������
			for(int i = nPushBackPos + nPushLength ; i > nPushBackPos ; --i )
			{
				m_pArrayData[i].id		= m_pArrayData[i-1].id;
				m_pArrayData[i].pData	= m_pArrayData[i-1].pData;
			}
		}

		//���ղ�����Ԫ��
		m_pArrayData[nPushBackPos].id = nID;
		m_pArrayData[nPushBackPos].pData = m_ObjectPool.AllocateObj();;
		++m_nArraySize;

		return m_pArrayData[nPushBackPos].pData;

	}


	//ɾ��һ��Ԫ��
	bool Del(int nID)
	{

		if(m_nArraySize == 0 )
		{	
			return false;
		}

		int nHeadPos = 0;
		int nTailPos = m_nArraySize-1;
		int nMidPos;

		while(nHeadPos < nTailPos)
		{
			nMidPos = (nHeadPos + nTailPos) / 2;
			if( m_pArrayData[nMidPos].id > nID )
			{
				nTailPos = nMidPos - 1;
			}
			else if( m_pArrayData[nMidPos].id < nID )
			{
				nHeadPos = nMidPos + 1;
			}
			else
			{
				//�ҵ���Ԫ��
				RemoveByPos(nMidPos);
				return true;
			}
		}

		//������Ƿ��Ǹ�Ԫ��
		nMidPos = (nHeadPos + nTailPos) / 2;
		if(m_pArrayData[nMidPos].id == nID )
		{
			//�Ѿ����ڸ�Ԫ��
			RemoveByPos(nMidPos);
			return true;
		}

		return false;

	}


	//�õ�Ԫ������
	T* Find(int nID)
	{
		if(m_nArraySize == 0 )
		{	
			return NULL;
		}

		int nHeadPos = 0;
		int nTailPos = m_nArraySize-1;
		int nMidPos;

		while(nHeadPos < nTailPos)
		{
			nMidPos = (nHeadPos + nTailPos) / 2;
			if( m_pArrayData[nMidPos].id > nID )
			{
				nTailPos = nMidPos - 1;
			}
			else if( m_pArrayData[nMidPos].id < nID )
			{
				nHeadPos = nMidPos + 1;
			}
			else
			{
				//�ҵ���Ԫ��
				return m_pArrayData[nMidPos].pData;
			}
		}

		nMidPos = (nHeadPos + nTailPos) / 2;
		if(m_pArrayData[nMidPos].id == nID )
		{
			//�ҵ���Ԫ��
			return m_pArrayData[nMidPos].pData;
		}

		return NULL;
	}

	void RemoveByPos(int nPos)
	{
		m_ObjectPool.ReleaseObj(m_pArrayData[nPos].pData);
		//��ɾ�����Ԫ����ǰ�ƴӶ�Ĩ����Ԫ��
		for(int i = nPos; i < m_nArraySize-1 ; ++i )
		{
			m_pArrayData[i].id		= m_pArrayData[i+1].id;
			m_pArrayData[i].pData	= m_pArrayData[i+1].pData;
		}
		--m_nArraySize;
	}


	T* FindByPos(int nPos) 
	{ 
		//assert( 0<=nPos && nPos < m_nArraySize );
		if( 0<=nPos && nPos < m_nArraySize )
		{
			return m_pArrayData[nPos].pData;
		}

		return NULL;
	}

	int GetSize() { return m_nArraySize;}

	void Clear()
	{
		for( int i = 0 ; i < m_nArraySize; ++i )
		{
			m_ObjectPool.ReleaseObj( m_pArrayData[i].pData );
		}
		m_nArraySize = 0 ;
	}

	CSortArray& operator =(CSortArray& a)
    {
		if( m_pArrayData )
		{
			delete[] m_pArrayData;
			m_pArrayData = NULL;
		}

		m_nArraySize	= a.m_nArraySize;
		m_nMaxArraySize = a.m_nMaxArraySize;
		m_pArrayData	= new SortArrayItem[m_nMaxArraySize];
		memcpy(m_pArrayData, a.m_pArrayData, sizeof(SortArrayItem)*m_nMaxArraySize);
		
        return *this;
    }

	CSortArray(const CSortArray& a)
    {
		m_nArraySize	= a.m_nArraySize;
		m_nMaxArraySize = a.m_nMaxArraySize;
		m_pArrayData	= new SortArrayItem[m_nMaxArraySize];
		memcpy(m_pArrayData, a.m_pArrayData, sizeof(SortArrayItem)*m_nMaxArraySize);
    }

private:
	int		m_nArraySize;		//��ǰ�����С
	int		m_nMaxArraySize;	//������ڲ��������ߴ�	


	struct SortArrayItem
	{
		int id;
		T* pData;
		
		SortArrayItem()
		{
			id = 0;
			pData = NULL;
		}
	}* m_pArrayData;

	CZObjectPool<T>		m_ObjectPool;	
};
