/******************************************************************
** �ļ���:	dataTrans.h
** ��  Ȩ:	���ڻ��Ǻ�Ѷ
** ������:	Tony
** ��  ��:	2007-7-2 15:38
** ��  ��:  ��NetTrans����ʽ�����һ����װ,�����������ת��ʽ�������ṩͳһ�ӿ�

**************************** �޸ļ�¼ ******************************
** �޸���: 
** ��  ��: 
** ��  ��: 
*******************************************************************/
#ifndef __DATA_TRANS__
#define __DATA_TRANS__
#include "MiniNetTrans.h"

typedef enum
{
	CONN_P2P,       //ֱ�ӽ�������
	CONN_SRV_TRANS  //ͨ����������ת��������
}ConnType;
class MININETTRANS_API CDataTrans
{
public:
	/**datatrans���캯��
	*  @param id:   ��ʶ�����ӵ�id��һ��Ϊ����id
	*  @param udpNetTrans: DataTransʹ�õ���������ָ��
	*/
	CDataTrans(const uint32 id,CUdpNetTrans* udpNetTrans);
	/**����һ���µ�socket��ʶ��
	*  @param wndSize:  socket�������ڵĴ�С
	*  @note:  ע�⣬�������ڿɸ�����Ҫ����
	*/
	NETSOCKET socket(uint32 wndSize = 50);
	/** ��������
	*  @param u:           socket��ʶ��
	*  @param fid:         Ҫ���ӵ��ĺ���id
	*  @param frirendIp :  ����ip
	*  @param friendPort:  ���ѵĶ˿�
	*  @param nTranIP   :  ��ת������ip
	*  @param nPort     :  ��ת�������˿�
	*/
	int      connect(NETSOCKET u,uint32 fid,uint32 friendIp,uint16 friendPort,uint32 nTranIP,uint16 nPort);
	/** ��������
	*  @param fid  :      Ҫ���յĺ��ѵ�id
	*  @param friendIp:   ����ip
	*  @param friendPort: ���ѵĶ˿�
	*  @param nTranIP   : ��ת������ip
	*  @paran nPort     : ��ת�������˿�
	*/
	NETSOCKET      accept(uint32 fid, uint32 friendIp,uint16 friendPort,uint32 nTranIP,uint16 nPort);
	/**�������ݿ�
	* @param u:    socket ID
	* @param buf:   Ҫ���͵����ݻ�����
	* @param len:   ����������
	* @return   int: ʵ�ʷ��������ݳ���
	* @note  : ע�⣬��������첽����
	*/
	int      send(const NETSOCKET u, char* buf,const uint32 len);
	/**��������
	*  @param u  : socket id
	*  @param buf : �������ݵĻ�����
	*  @param len :  ����������
	*/
	int      recv(const NETSOCKET u, char* buf,const int len);
	/**�ر�һ������
	*  @param  u:  ���ӱ�ʶ��
	*/
	void      close( NETSOCKET u);
	/**���ʵ�����ӵ�����
	*  @return : 
	*/
	ConnType getConnType()const;
private:
	CUdpNetTrans* m_pNetTrans;
	uint32       m_selfId;     //�Լ���id
	ConnType m_connType;   //�������ͣ�ֱ�����ӻ��������ת����
	//sockaddr_in  m_transServerAddr;  //��ת��������ַ
	sockaddr_in  m_peerAddr;         //�Է���ַ
};

#endif/*__DATA_TRANS__*/