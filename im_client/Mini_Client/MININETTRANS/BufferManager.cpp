/******************************************************************
** 文件名:	BufferManager.cpp
** 版  权:	
** 创建人:	Tony
** 日  期:	2007-6-6
** 描  述:  这个类主要用用来对收发的数据缓冲管理

**************************** 修改记录 ******************************
** 修改人: 
** 日  期: 
** 描  述: 
*******************************************************************/

#include "stdafx.h"
#include "BufferManager.h"
#include "PublicType.h"

CSndBuffer::CSndBuffer(uint32 size)
{
	m_windowSize = size;
	m_beginPos = 0;
	m_endPos   = 0;
	threadMode::scope_lock(m_bufferQueue.m_mutex);
	for(uint32 i=0;i<=size;++i)
	{
		m_bufferQueue.push_back(new Block);
	}
}
CSndBuffer::~CSndBuffer()
{
	while(!m_bufferQueue.empty())
	{
		delete m_bufferQueue.front();
		m_bufferQueue.pop_front();
	}
}
uint32 CSndBuffer::getSndSpace()
{ ///滑动窗口的空余大小
	return m_windowSize - (m_endPos - m_beginPos);
}

bool CSndBuffer::addBlock(const char* buf,uint32& size,uint32& seq,uint32& msgNo)
{
	threadMode::scope_lock guard(m_mutex); //同步锁

	if(m_endPos > msgNo)
		fclAssert(false);
	if(getSndSpace() == 0) return false;
    Block* pBlock = NULL;
    if(m_sndBuf.find(msgNo) != m_sndBuf.end())
	{
		//fclAssert(false);  //添加了重复的消息号
		Error("send 函数添加了重复的消息号!\n");
		return false;
	}
	else
	{
		if(m_bufferQueue.empty())
		{
			fclAssert(false);
			return false;
		}
		pBlock = m_bufferQueue.front();
		m_bufferQueue.pop_front();
		memcpy(pBlock->m_pcData,buf,size);
		pBlock->m_iMsgNo = msgNo;
		pBlock->m_isn = seq;
		pBlock->m_iLength = size;
		m_sndBuf[msgNo] = pBlock;
		m_endPos = msgNo;
	}
	return true;
}
bool CSndBuffer::getBlock(uint32 msgNo,uint32& seq,char*& buf,uint16& size)
{

	threadMode::scope_lock guard(m_mutex);

	if(!isInWnd(msgNo)) return false;
    Block* pBlock = NULL;
	fclHash_map<uint32,Block*>::iterator itr = m_sndBuf.find(msgNo);
	if(itr != m_sndBuf.end())
	{
		pBlock = (*itr).second;
		seq = pBlock->m_isn;
		buf = pBlock->m_pcData;
		size = pBlock->m_iLength;
		return true;
	}

	return false;
}
//msgNo以前的数据包都被确认
bool CSndBuffer::ackBlock(uint32& msgNo)
{
	threadMode::scope_lock guard(m_mutex);

	if(isEmpty())
		return false;
	Block* pBlock = NULL;
	if(msgNo < m_beginPos || msgNo > m_endPos + 1)
	{
		Trace("not in snd Window\n");
		return false;
	}
	fclHash_map<uint32,Block*>::iterator itr;
	while(m_beginPos < msgNo)
	{
		itr = m_sndBuf.find(m_beginPos);
		pBlock = (*itr).second;
		m_bufferQueue.push_back(pBlock); //回收缓冲区块
		m_sndBuf.erase(itr);
		++m_beginPos;
	}

	return true;
}
//msgNo是否在滑动窗口内
bool CSndBuffer::isInWnd(uint32& msgNo)
{
	if(isEmpty()) 
		return false;

	return msgNo >= m_beginPos && msgNo <= m_endPos;
}
uint32 CSndBuffer::getCurMsgNo()
{
	if(isEmpty())
		return 0;

	return m_endPos;
}
bool CSndBuffer::isEmpty()
{
	return m_sndBuf.size() == 0;
}
void CSndBuffer::getWndRange(uint32& beginPos,uint32& endPos)
{
	beginPos = m_beginPos;
	endPos   = m_endPos;
}


CRcvBuffer::CRcvBuffer(uint32 size)
{
	m_windowSize = size;
	m_beginPos = 0;
	m_endPos   = 0;
	threadMode::scope_lock guard(m_bufferQueue.m_mutex);
	for(uint32 i=0;i<=size;++i)
		m_bufferQueue.push_back(new Block);
}
CRcvBuffer::~CRcvBuffer()
{
	while(!m_bufferQueue.empty())
	{
		delete m_bufferQueue.front();
		m_bufferQueue.pop_front();
	}
}
uint32 CRcvBuffer::getRecvSpace()const
{
	///滑动窗口的空余大小
	return m_windowSize - (m_endPos - m_beginPos);
}

bool CRcvBuffer::addBlock(const char* buf,uint16& size,uint32& seq,uint32& msgNo)
{
//	threadMode::scope_lock  guard(m_mutex);
	//这里应该也不需要同步
	Block* pBlock = NULL;
	if(m_rcvBuf.size() > m_windowSize) //窗口已满
		return false;

/////判断接收到的消息号是否在滑动窗口内，如不是，则丢掉
	  if(msgNo < m_beginPos || msgNo - m_beginPos > m_windowSize)
	  {
		  //Trace("Not in rcv window:%d\n",msgNo);
		  return false;
	  }
		///计算包要插入到滑动窗口的位置
	  if(m_rcvBuf.find(msgNo) != m_rcvBuf.end())
	  {
		 // Trace("Receive repeat packet!\n");
		  return false;
	  }
	  else
	  {
		  if(msgNo == m_endPos)
			  ++m_endPos;
		  else if(msgNo > m_endPos)
			  m_endPos = msgNo+1;

		  if(m_bufferQueue.empty())
		  {
			  Error("Rcv window BufferQueue full!\n");
			  pBlock = new Block;
		  }
		  else
		  {
			  pBlock = m_bufferQueue.front();
			  m_bufferQueue.pop_front();
		  }

		  pBlock->m_iLength = size;
		  pBlock->m_iMsgNo = msgNo;
		  pBlock->m_isn = seq;

		  memcpy(pBlock->m_pcData,buf,size);
		  m_rcvBuf[msgNo] = pBlock;
	  }

///根据收到的包的连续个数将确认的连续包放入接收链表，窗口向后移动
      uint32 i = m_beginPos;
	  fclHash_map<uint32,Block*>::iterator itr;
	  while(i < m_endPos)
	  {
		  itr = m_rcvBuf.find(i);
		  if(itr != m_rcvBuf.end())
		  {//连续块
			  m_recvDataQueue.push_back((*itr).second);
			  m_rcvBuf.erase(itr);
			  ++i;
			  ++m_beginPos;
		  }
		  else
		  {
			  break;
		  }
	  }
	   return true;
}

uint32 CRcvBuffer::getRecvQueueLen()
{
	return (uint32)m_recvDataQueue.size();
}
uint32 CRcvBuffer::readData(char* buf, uint32 size)
{
	//threadMode::scope_lock   guard(m_mutex); 
    //这里应该不需要同步，m_recvDataQueue自己同步就可以了
	if(m_recvDataQueue.empty())
		return 0;
    uint32 pos = 0;
	Block* pBlock = NULL;
    while(!m_recvDataQueue.empty())
	{
		pBlock = m_recvDataQueue.front();
		if(pBlock->m_iLength + pos <= size)
		{
			memcpy(buf+pos,pBlock->m_pcData,pBlock->m_iLength);
			pos += pBlock->m_iLength;
			m_bufferQueue.push_back(pBlock); //回收缓冲区块
			m_recvDataQueue.pop_front();
		}
		else
		{
			break;
		}
	}
	return pos;
}

void CRcvBuffer::getWndRange(uint32& beginPos,uint32& endPos)const
{
	beginPos = m_beginPos;
	endPos   = m_endPos;
}
bool CRcvBuffer::isEmpty()const
{
	return m_rcvBuf.size() == 0;
}
