#pragma once

#include "WinCommonForLinux.h"
#include "improtocol.pb.h"
#include "msgnotifyinner.pb.h"


class CThreadResponseKafka
{
public:
    CThreadResponseKafka();
    ~CThreadResponseKafka();
    int StartWork();

protected:
	static void* Proc( LPVOID lpParam );
	// ���¶�������imserver
	void SendNewOrderToImServer(const NewOrder &neworder);
	// �յ�kafka������һ���¶���
	void RecvKafkaNewOrder(const KfkNewOrder &kfkneworder);
	// ���Ѹ��������imserver
	void SendPayOrderToImServer(const PayOrder &payorder);
	// �յ�kafka������һ���Ѹ����
	void RecvKafkaPayOrder(const KfkPayOrder &kfkpayorder);
	// ����������Ʒ����imserver
	void SendGoodsCommentToImServer(const GoodsComment &goodscomment);
	// �յ�kafka������һ����������Ʒ
	void RecvKafkaGoodsComment(const KfkGoodsComment &kfkgoodscomment);
	// ���˻�����������imserver
	void SendReturnOrderToImServer(const ReturnOrder &returnorder);
	// �յ�kafka������һ���˻�����Ʒ
	void RecvKafkaReturnOrder(const KfkReturnOrder &kfkreturnorder);
	// ��ȡ�����Ϣ
	bool GetBuyerInfo(const char* guid, char* szMobileNo, char* szEmail, char* szNickName,char* szHeadImgUrl);
	
	void *m_ctx;			// zmq������
	void *m_sc;				// ���ӵ�ImServer��socket
};

