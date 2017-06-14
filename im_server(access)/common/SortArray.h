#pragma once

#include "ZObjectPool.h"

/************************************************************************ 
文件描述：排列数组类的声明文件 

创建人：hechang
时间：	2006-7-5 
************************************************************************/

//排列数组,里面的元素按照ID排序,内部采用二分法实现快速查找,同时支持数组的遍历

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


	//添加一个元素,如果已经存在,返回NULL
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
				//已经存在该元素
				return NULL;
			}
		}


		//找到要插入的位置了，检查是插当前位置还是后面
		nMidPos = (nHeadPos + nTailPos) / 2;
		if(m_pArrayData[nMidPos].id == nID )
		{
			//已经存在该元素
			return NULL;
		}


		int nPushBackPos = 0;
		int nPushLength = 0 ;
		nPushBackPos = m_pArrayData[nMidPos].id < nID ? 
			nMidPos + 1 : nMidPos;
		nPushLength	 = m_nArraySize - nPushBackPos;

		if( nPushLength > 0 )
		{
			//把插入点的元素往后推
			for(int i = nPushBackPos + nPushLength ; i > nPushBackPos ; --i )
			{
				m_pArrayData[i].id		= m_pArrayData[i-1].id;
				m_pArrayData[i].pData	= m_pArrayData[i-1].pData;
			}
		}

		//最终插入新元素
		m_pArrayData[nPushBackPos].id = nID;
		m_pArrayData[nPushBackPos].pData = m_ObjectPool.AllocateObj();;
		++m_nArraySize;

		return m_pArrayData[nPushBackPos].pData;

	}


	//删除一个元素
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
				//找到该元素
				RemoveByPos(nMidPos);
				return true;
			}
		}

		//最后检查是否是该元素
		nMidPos = (nHeadPos + nTailPos) / 2;
		if(m_pArrayData[nMidPos].id == nID )
		{
			//已经存在该元素
			RemoveByPos(nMidPos);
			return true;
		}

		return false;

	}


	//得到元素数据
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
				//找到该元素
				return m_pArrayData[nMidPos].pData;
			}
		}

		nMidPos = (nHeadPos + nTailPos) / 2;
		if(m_pArrayData[nMidPos].id == nID )
		{
			//找到该元素
			return m_pArrayData[nMidPos].pData;
		}

		return NULL;
	}

	void RemoveByPos(int nPos)
	{
		m_ObjectPool.ReleaseObj(m_pArrayData[nPos].pData);
		//把删除点的元素往前推从而抹掉旧元素
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
	int		m_nArraySize;		//当前数组大小
	int		m_nMaxArraySize;	//数组的内部缓冲最大尺寸	


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
