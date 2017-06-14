
/******************************************************************
** 文件名:	PacketTime.h
** 版  权:	深圳华亚和讯
** 创建人:	Tony
** 日  期:	2007-6-11 14:27
** 描  述:  udp数据包的时间管理类，用来计算rtt，带宽，数据流量等

**************************** 修改记录 ******************************
** 修改人: 
** 日  期: 
** 描  述: 
*******************************************************************/
#ifndef __NETTRANS_PACKET_TIME__
#define __NETTRANS_PACKET_TIME__

#pragma once

#include "fcl.h"


/** 数据包时间管理类
*/
class CPacketTime
{
public:
	CPacketTime(uint32 wndSize = 16);
	~CPacketTime();


    /** 获得最小的数据包发送间隔时间
	 *  @return 微妙
	*/
	int getMinPktSndInt() const;

	/**获得每秒收到的包数
	*/
	int getPktRcvSpeed() const;

	/**
	* @brief  获得连接的带宽（每秒的包数）
	*/
	int getBandwidth() const;

    /**
	* @brief   记录发包的时间戳
	* @param currtime:  当前的时间
	*/
	void onPktSent(const int& currtime);


	/**
	* @brief  记录收到收到一个包
	*/
	void onPktArrival(uint32& msgNo);

	/**
	*  @brief   记录ack2中的rtt值
	*/
	void ack2Arrival(const int& rtt);

protected:
	/**
	* @brief  记录第一个对包的时间戳每16个包为一个对包，
	* 只记录每16个包的第一个包的时间戳
	*/
	void probe1Arrival();


	/**
	* brief  记录每16个包中除第一个包其他包的时间间隔，这些用于估计带宽使用
	*/
	void probe2Arrival();
private:
	NewTimer<>   m_timer;    ///获得cpu时间戳类,微秒级
	int m_iAWSize;           //总共可以记录包的个数
	int* m_piPktWindow;          // 记录包的间隔时间
	int m_iPktWindowPtr;         // 当前使用到的索引位置

	int m_iRWSize;            //rtt记录窗口大小
	int* m_piRTTWindow;        // RTT 历史窗口
	int* m_piPCTWindow;          // 记录对包历史窗口
	int* m_piPDTWindow;          //对包差别历史窗口
	int m_iRTTWindowPtr;        

	int m_iPWSize;              
	int* m_piProbeWindow;        
	int m_iProbeWindowPtr;       

	int m_iLastSentTime;         
	int m_iMinPktSndInt;         // Minimum packet sending interval

	uint64 m_LastArrTime;      // last packet arrival time
	uint64 m_CurrArrTime;      // current packet arrival time
	uint64 m_ProbeTime;        // arrival time of the first probing packet
};






#endif /*__NETTRANS_PACKET_TIME__*/