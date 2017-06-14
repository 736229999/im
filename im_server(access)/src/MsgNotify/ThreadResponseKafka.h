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
	// 将新订单传给imserver
	void SendNewOrderToImServer(const NewOrder &neworder);
	// 收到kafka发来的一个新订单
	void RecvKafkaNewOrder(const KfkNewOrder &kfkneworder);
	// 将已付款订单传给imserver
	void SendPayOrderToImServer(const PayOrder &payorder);
	// 收到kafka发来的一个已付款订单
	void RecvKafkaPayOrder(const KfkPayOrder &kfkpayorder);
	// 将已评价商品传给imserver
	void SendGoodsCommentToImServer(const GoodsComment &goodscomment);
	// 收到kafka发来的一个已评论商品
	void RecvKafkaGoodsComment(const KfkGoodsComment &kfkgoodscomment);
	// 将退换货订单传给imserver
	void SendReturnOrderToImServer(const ReturnOrder &returnorder);
	// 收到kafka发来的一个退换货商品
	void RecvKafkaReturnOrder(const KfkReturnOrder &kfkreturnorder);
	// 获取买家信息
	bool GetBuyerInfo(const char* guid, char* szMobileNo, char* szEmail, char* szNickName,char* szHeadImgUrl);
	
	void *m_ctx;			// zmq上下文
	void *m_sc;				// 连接到ImServer的socket
};

