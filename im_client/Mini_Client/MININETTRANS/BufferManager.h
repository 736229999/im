/******************************************************************
** 文件名:	BufferManager.h
** 版  权:	
** 创建人:	Tony
** 日  期:	2007-6-6
** 描  述:  这个类主要用用来对收发的数据缓冲管理

**************************** 修改记录 ******************************
** 修改人: 
** 日  期: 
** 描  述: 
*******************************************************************/
#ifndef __P2P_BUFFER__
#define __P2P_BUFFER__
#include "fcl.h"
#include "PublicType.h"
using namespace fcl;

//------------------------------------------------------
// 数据发送池,实现数据包发送和超时管理
//------------------------------------------------------




class CSndBuffer
{
	//friend class CNetUnited;
	struct Block
	{
		char m_pcData[MAX_NETWORK_PACKET];                   // 数据块指针
		uint32 m_iLength;                    // 数据块长度
		uint32 m_iMsgNo;                 // 包的消息号
		uint32 m_isn;                 // 包的序列号
	};
public:
	//设定滑动窗口的大小，单位为包
	CSndBuffer(uint32 size);
	~CSndBuffer();
	///获得滑动窗口还有多少个包的空余，可以继续发送的大小
	uint32 getSndSpace();
	/**向滑动窗口添加一个待发送的数据块
	 @param buf:
	 @param size:
     @param seq:
	 @param msgNo
	*/
	bool addBlock(const char* buf,uint32& size,uint32& seq,uint32& msgNo);
	/**获得滑动窗口中指定消息号的数据块及参数
	* @param msgNo:  输入参数，要获取数据块的序号
	* @param seq  :  输出参数，数据块的isn序号
	* @param buf  :  输出参数，数据块指针
	* @param size :  输出参数，数据块长度
	* @return  bool: 是否找到响应的数据块
	*/
	bool getBlock(const uint32 msgNo,uint32& seq,char*& buf,uint16& size);
	/**  确认一个包之前的所有包都已经收到
	  @param msgNo:  包的消息号
	  @return bool: 确认序号是否在滑动窗口范围内
	*/
	bool ackBlock(uint32& msgNo);
	/**
	  @param msgNo: 数据包的消息号
	  @return bool:  消息号是否在滑动窗口范围内
	*/
	bool isInWnd(uint32& msgNo);
	/**
	*  @note : 获得当前发送到的消息号
	*/
	uint32 getCurMsgNo();
	/**滑动窗口是否为空
	*/
	bool isEmpty();
	//返回滑动窗口的开始和结尾值
	void getWndRange(uint32& beginPos,uint32& endPos);

private:
	typedef multiThread threadMode;
	threadMode::mutex     m_mutex;    //addBlock 和ackBlock要同步
	uint32 m_windowSize;  ///滑动窗口的大小
	uint32 m_beginPos;  ///滑动窗口的开始值，及消息号
	uint32 m_endPos;    ///滑动窗口的结束位置，及消息号
	Block* m_pWnd;   ///滑动窗口
	bool*   m_ackWnd;   ///确认窗口
	fclHash_map<uint32,Block*>  m_sndBuf;  //发送方滑动窗口实现
	fclList<Block*>   m_bufferQueue;      //缓冲区队列
};

class CRcvBuffer
{
//	friend class CNetUnited;
	struct Block
	{
		char m_pcData[MAX_NETWORK_PACKET];                   // 数据块指针
		uint32 m_iLength;                    // 数据块长度
		uint32 m_iMsgNo;                 // 包的消息号
		uint32 m_isn;                 // 包的序列号
	};
public:
	CRcvBuffer(uint32 size);
	~CRcvBuffer();
	///获得接收滑动窗口还可以接收的包数
	uint32 getRecvSpace()const;
	/**向滑动窗口添加一个收到的数据包
	@param buf:
	@param size:
	@param seq:
	@param msgNo
	*/
	bool addBlock(const char* buf,uint16& size,uint32& seq,uint32& msgNo);
	/**返回当前的丢失包的消息号数组
	 @param buf :用来存放丢失的包的消息号
	 @param len : 数组大小
	 @return   :  丢失的包的个数
	*/
    //uint32 getLosssBlockMsgNo(uint32* buf,uint32 len);
    ///获得接收队列的长度 
	uint32 getRecvQueueLen();
	/**读取流式数据
	  @param buf: 读取数据存放的缓冲区
	  @param size: 缓冲区的大小
	  @return :    实际读出的数据
	*/
	uint32 readData(char* buf, uint32 size);
	//返回滑动窗口的开始和结尾值
	void getWndRange(uint32& beginPos,uint32& endPos)const;
	//滑动窗口是否为空
	bool isEmpty()const;
private:
	uint32 m_windowSize;  ///滑动窗口的大小,以包为单位
	fclHash_map<uint32,Block*>   m_rcvBuf;//接收数据包管理，实现滑动窗口
	uint32 m_beginPos;  ///滑动窗口的开始值,即消息号
	uint32 m_endPos;    ///滑动窗口的当前位置，消息号
	Block* m_pWnd;   ///滑动窗口
	bool*  m_dataFlag;   ///窗口某个位置是否已收到数据
	//int32 m_lastMsgno;  ///已经确认收到的最小的消息号
	fclList<Block*> m_recvDataQueue;  ///已经确认收到的流式数据包队列
	typedef fclList<Block*>::const_iterator recvDataQueueItr;
	fclList<Block*> m_bufferQueue;  //数据块缓冲队列
	typedef multiThread threadMode;
	threadMode::mutex    m_mutex;    //readData()和addBlock()要进行同步
};



#endif/*__P2P_BUFFER__*/