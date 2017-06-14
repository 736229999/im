/******************************************************************
** 文件名:	dataTrans.h
** 版  权:	深圳华亚和讯
** 创建人:	Tony
** 日  期:	2007-7-2 15:38
** 描  述:  对NetTrans中流式传输进一步封装,加入服务器中转方式，对外提供统一接口

**************************** 修改记录 ******************************
** 修改人: 
** 日  期: 
** 描  述: 
*******************************************************************/
#ifndef __DATA_TRANS__
#define __DATA_TRANS__
#include "MiniNetTrans.h"

typedef enum
{
	CONN_P2P,       //直接建立连接
	CONN_SRV_TRANS  //通过服务器中转建立连接
}ConnType;
class MININETTRANS_API CDataTrans
{
public:
	/**datatrans构造函数
	*  @param id:   标识本连接的id，一般为好友id
	*  @param udpNetTrans: DataTrans使用的网络层对象指针
	*/
	CDataTrans(const uint32 id,CUdpNetTrans* udpNetTrans);
	/**生成一个新的socket标识符
	*  @param wndSize:  socket滑动窗口的大小
	*  @note:  注意，滑动窗口可根据需要调整
	*/
	NETSOCKET socket(uint32 wndSize = 50);
	/** 建立连接
	*  @param u:           socket标识符
	*  @param fid:         要连接到的好友id
	*  @param frirendIp :  好友ip
	*  @param friendPort:  好友的端口
	*  @param nTranIP   :  中转服务器ip
	*  @param nPort     :  中转服务器端口
	*/
	int      connect(NETSOCKET u,uint32 fid,uint32 friendIp,uint16 friendPort,uint32 nTranIP,uint16 nPort);
	/** 接收连接
	*  @param fid  :      要接收的好友的id
	*  @param friendIp:   好友ip
	*  @param friendPort: 好友的端口
	*  @param nTranIP   : 中转服务器ip
	*  @paran nPort     : 中转服务器端口
	*/
	NETSOCKET      accept(uint32 fid, uint32 friendIp,uint16 friendPort,uint32 nTranIP,uint16 nPort);
	/**发送数据块
	* @param u:    socket ID
	* @param buf:   要发送的数据缓冲区
	* @param len:   缓冲区长度
	* @return   int: 实际发出的数据长度
	* @note  : 注意，这里采用异步发送
	*/
	int      send(const NETSOCKET u, char* buf,const uint32 len);
	/**接收数据
	*  @param u  : socket id
	*  @param buf : 接收数据的缓冲区
	*  @param len :  缓冲区长度
	*/
	int      recv(const NETSOCKET u, char* buf,const int len);
	/**关闭一个连接
	*  @param  u:  连接标识符
	*/
	void      close( NETSOCKET u);
	/**获得实际连接的类型
	*  @return : 
	*/
	ConnType getConnType()const;
private:
	CUdpNetTrans* m_pNetTrans;
	uint32       m_selfId;     //自己的id
	ConnType m_connType;   //连接类型，直接连接或服务器中转连接
	//sockaddr_in  m_transServerAddr;  //中转服务器地址
	sockaddr_in  m_peerAddr;         //对方地址
};

#endif/*__DATA_TRANS__*/