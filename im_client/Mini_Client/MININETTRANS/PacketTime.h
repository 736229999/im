
/******************************************************************
** �ļ���:	PacketTime.h
** ��  Ȩ:	���ڻ��Ǻ�Ѷ
** ������:	Tony
** ��  ��:	2007-6-11 14:27
** ��  ��:  udp���ݰ���ʱ������࣬��������rtt����������������

**************************** �޸ļ�¼ ******************************
** �޸���: 
** ��  ��: 
** ��  ��: 
*******************************************************************/
#ifndef __NETTRANS_PACKET_TIME__
#define __NETTRANS_PACKET_TIME__

#pragma once

#include "fcl.h"


/** ���ݰ�ʱ�������
*/
class CPacketTime
{
public:
	CPacketTime(uint32 wndSize = 16);
	~CPacketTime();


    /** �����С�����ݰ����ͼ��ʱ��
	 *  @return ΢��
	*/
	int getMinPktSndInt() const;

	/**���ÿ���յ��İ���
	*/
	int getPktRcvSpeed() const;

	/**
	* @brief  ������ӵĴ���ÿ��İ�����
	*/
	int getBandwidth() const;

    /**
	* @brief   ��¼������ʱ���
	* @param currtime:  ��ǰ��ʱ��
	*/
	void onPktSent(const int& currtime);


	/**
	* @brief  ��¼�յ��յ�һ����
	*/
	void onPktArrival(uint32& msgNo);

	/**
	*  @brief   ��¼ack2�е�rttֵ
	*/
	void ack2Arrival(const int& rtt);

protected:
	/**
	* @brief  ��¼��һ���԰���ʱ���ÿ16����Ϊһ���԰���
	* ֻ��¼ÿ16�����ĵ�һ������ʱ���
	*/
	void probe1Arrival();


	/**
	* brief  ��¼ÿ16�����г���һ������������ʱ��������Щ���ڹ��ƴ���ʹ��
	*/
	void probe2Arrival();
private:
	NewTimer<>   m_timer;    ///���cpuʱ�����,΢�뼶
	int m_iAWSize;           //�ܹ����Լ�¼���ĸ���
	int* m_piPktWindow;          // ��¼���ļ��ʱ��
	int m_iPktWindowPtr;         // ��ǰʹ�õ�������λ��

	int m_iRWSize;            //rtt��¼���ڴ�С
	int* m_piRTTWindow;        // RTT ��ʷ����
	int* m_piPCTWindow;          // ��¼�԰���ʷ����
	int* m_piPDTWindow;          //�԰������ʷ����
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