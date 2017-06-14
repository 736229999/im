/******************************************************************
** �ļ���:	BufferManager.h
** ��  Ȩ:	
** ������:	Tony
** ��  ��:	2007-6-6
** ��  ��:  �������Ҫ���������շ������ݻ������

**************************** �޸ļ�¼ ******************************
** �޸���: 
** ��  ��: 
** ��  ��: 
*******************************************************************/
#ifndef __P2P_BUFFER__
#define __P2P_BUFFER__
#include "fcl.h"
#include "PublicType.h"
using namespace fcl;

//------------------------------------------------------
// ���ݷ��ͳ�,ʵ�����ݰ����ͺͳ�ʱ����
//------------------------------------------------------




class CSndBuffer
{
	//friend class CNetUnited;
	struct Block
	{
		char m_pcData[MAX_NETWORK_PACKET];                   // ���ݿ�ָ��
		uint32 m_iLength;                    // ���ݿ鳤��
		uint32 m_iMsgNo;                 // ������Ϣ��
		uint32 m_isn;                 // �������к�
	};
public:
	//�趨�������ڵĴ�С����λΪ��
	CSndBuffer(uint32 size);
	~CSndBuffer();
	///��û������ڻ��ж��ٸ����Ŀ��࣬���Լ������͵Ĵ�С
	uint32 getSndSpace();
	/**�򻬶��������һ�������͵����ݿ�
	 @param buf:
	 @param size:
     @param seq:
	 @param msgNo
	*/
	bool addBlock(const char* buf,uint32& size,uint32& seq,uint32& msgNo);
	/**��û���������ָ����Ϣ�ŵ����ݿ鼰����
	* @param msgNo:  ���������Ҫ��ȡ���ݿ�����
	* @param seq  :  ������������ݿ��isn���
	* @param buf  :  ������������ݿ�ָ��
	* @param size :  ������������ݿ鳤��
	* @return  bool: �Ƿ��ҵ���Ӧ�����ݿ�
	*/
	bool getBlock(const uint32 msgNo,uint32& seq,char*& buf,uint16& size);
	/**  ȷ��һ����֮ǰ�����а����Ѿ��յ�
	  @param msgNo:  ������Ϣ��
	  @return bool: ȷ������Ƿ��ڻ������ڷ�Χ��
	*/
	bool ackBlock(uint32& msgNo);
	/**
	  @param msgNo: ���ݰ�����Ϣ��
	  @return bool:  ��Ϣ���Ƿ��ڻ������ڷ�Χ��
	*/
	bool isInWnd(uint32& msgNo);
	/**
	*  @note : ��õ�ǰ���͵�����Ϣ��
	*/
	uint32 getCurMsgNo();
	/**���������Ƿ�Ϊ��
	*/
	bool isEmpty();
	//���ػ������ڵĿ�ʼ�ͽ�βֵ
	void getWndRange(uint32& beginPos,uint32& endPos);

private:
	typedef multiThread threadMode;
	threadMode::mutex     m_mutex;    //addBlock ��ackBlockҪͬ��
	uint32 m_windowSize;  ///�������ڵĴ�С
	uint32 m_beginPos;  ///�������ڵĿ�ʼֵ������Ϣ��
	uint32 m_endPos;    ///�������ڵĽ���λ�ã�����Ϣ��
	Block* m_pWnd;   ///��������
	bool*   m_ackWnd;   ///ȷ�ϴ���
	fclHash_map<uint32,Block*>  m_sndBuf;  //���ͷ���������ʵ��
	fclList<Block*>   m_bufferQueue;      //����������
};

class CRcvBuffer
{
//	friend class CNetUnited;
	struct Block
	{
		char m_pcData[MAX_NETWORK_PACKET];                   // ���ݿ�ָ��
		uint32 m_iLength;                    // ���ݿ鳤��
		uint32 m_iMsgNo;                 // ������Ϣ��
		uint32 m_isn;                 // �������к�
	};
public:
	CRcvBuffer(uint32 size);
	~CRcvBuffer();
	///��ý��ջ������ڻ����Խ��յİ���
	uint32 getRecvSpace()const;
	/**�򻬶��������һ���յ������ݰ�
	@param buf:
	@param size:
	@param seq:
	@param msgNo
	*/
	bool addBlock(const char* buf,uint16& size,uint32& seq,uint32& msgNo);
	/**���ص�ǰ�Ķ�ʧ������Ϣ������
	 @param buf :������Ŷ�ʧ�İ�����Ϣ��
	 @param len : �����С
	 @return   :  ��ʧ�İ��ĸ���
	*/
    //uint32 getLosssBlockMsgNo(uint32* buf,uint32 len);
    ///��ý��ն��еĳ��� 
	uint32 getRecvQueueLen();
	/**��ȡ��ʽ����
	  @param buf: ��ȡ���ݴ�ŵĻ�����
	  @param size: �������Ĵ�С
	  @return :    ʵ�ʶ���������
	*/
	uint32 readData(char* buf, uint32 size);
	//���ػ������ڵĿ�ʼ�ͽ�βֵ
	void getWndRange(uint32& beginPos,uint32& endPos)const;
	//���������Ƿ�Ϊ��
	bool isEmpty()const;
private:
	uint32 m_windowSize;  ///�������ڵĴ�С,�԰�Ϊ��λ
	fclHash_map<uint32,Block*>   m_rcvBuf;//�������ݰ�����ʵ�ֻ�������
	uint32 m_beginPos;  ///�������ڵĿ�ʼֵ,����Ϣ��
	uint32 m_endPos;    ///�������ڵĵ�ǰλ�ã���Ϣ��
	Block* m_pWnd;   ///��������
	bool*  m_dataFlag;   ///����ĳ��λ���Ƿ����յ�����
	//int32 m_lastMsgno;  ///�Ѿ�ȷ���յ�����С����Ϣ��
	fclList<Block*> m_recvDataQueue;  ///�Ѿ�ȷ���յ�����ʽ���ݰ�����
	typedef fclList<Block*>::const_iterator recvDataQueueItr;
	fclList<Block*> m_bufferQueue;  //���ݿ黺�����
	typedef multiThread threadMode;
	threadMode::mutex    m_mutex;    //readData()��addBlock()Ҫ����ͬ��
};



#endif/*__P2P_BUFFER__*/