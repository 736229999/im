#include "KafkaGoodsComment.h"
#include "Log.h"
#include "json/json.h"
#include "msgnotifyinner.pb.h"
#include "AppManager.h"
#include "PacketBuffer.h"


CKafkaGoodsComment::CKafkaGoodsComment(const char *pBrokers, const char *pTopic)
	: CKafKaConsumer(pBrokers, pTopic)
{

}

void CKafkaGoodsComment::GetNewRecord(const char *pRecord)
{
	CLog::Log("CKafkaGoodsComment",CLog::TYPE_DEBUG, "Topic:%s, Record: %s", GetTopicStr(), pRecord);

    Json::Reader jReader(Json::Features::strictMode());
    Json::Value jvValue;
	/*
	{
		"order_id": "201601CP15036971",
		"buyer_id": "F1962196-3DB1-4449-8897-9A716E30A193",
		"buyer_name": "sunding3",
		"evaluated_goods_name": "商城首页测试商品（勿动）4",
		"evaluate_level": 4,
		"evaluate_context": "的发的所发生的范德萨发单四方达发送到发送到",
		"evaluated_goods_sku_color": "",
		"evaluated_goods_sku_size": "",
		"goodsId": "1302",
		"skuId": "100000955"
	}*/

    KfkGoodsComment goodscomment;
    if((pRecord[0] == '{') && (jReader.parse(pRecord, jvValue)))
    {
        if(jvValue.isMember("buyer_id"))
        {
            goodscomment.set_memguid(jvValue["buyer_id"].asCString());
        }
        if(jvValue.isMember("order_id"))
        {
            goodscomment.set_ogno(jvValue["order_id"].asCString());
        }
        if(jvValue.isMember("buyer_name"))
        {
            goodscomment.set_bugername(jvValue["buyer_name"].asCString());
        }
        if(jvValue.isMember("evaluate_context"))
        {
            goodscomment.set_evaluatecontext(jvValue["evaluate_context"].asCString());
        }
        if(jvValue.isMember("skuId"))
        {
            goodscomment.set_skuid(jvValue["skuId"].asCString());
        }
        if(jvValue.isMember("evaluate_level"))
        {
            goodscomment.set_evaluatelevel(jvValue["evaluate_level"].asDouble());
        }

        uint32_t msg_size = goodscomment.ByteSize();
        char* szData = new char[msg_size];
        goodscomment.SerializeToArray(szData, msg_size);
		CLog::Log("CKafkaGoodsComment",CLog::TYPE_DEBUG,"kfk 商品ID%s 评价等级%f 评价内容%s",
                  goodscomment.skuid().c_str(),
                  goodscomment.evaluatelevel(),
                  goodscomment.evaluatecontext().c_str());


		MsgPushInner msgpush;
		msgpush.set_mptype(MP_GOODSCOMMENT);
		msgpush.set_mpdata(szData, msg_size);

		delete[] szData;
		szData = NULL;

		msg_size = msgpush.ByteSize();
        szData = new char[msg_size];
        msgpush.SerializeToArray(szData, msg_size);
		CAppManager::GetInstance().GetFifoKafkaMsg()->Push((BYTE*)szData,msg_size);
        delete[] szData;
        szData = NULL;
    }
}

