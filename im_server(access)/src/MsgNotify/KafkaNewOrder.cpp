#include "KafkaNewOrder.h"
#include "Log.h"
#include "json/json.h"
#include "msgnotifyinner.pb.h"
#include "AppManager.h"
#include "PacketBuffer.h"


CKafkaNewOrder::CKafkaNewOrder(const char *pBrokers, const char *pTopic)
    : CKafKaConsumer(pBrokers, pTopic)
{

}

void CKafkaNewOrder::GetNewRecord(const char *pRecord)
{
    CLog::Log("CKafkaNewOrder",CLog::TYPE_DEBUG, "Topic:%s, Record: %s", GetTopicStr(), pRecord);

	//{"environment":"beta","fidType":"1","memguid":"136BCD2D-EBE5-E25B-E821-29AE3E97F798","ogno":"201601CP08035863","ogseq":"201601CP08035863","succ":1,"timestamp":1452243645145}
    Json::Reader jReader(Json::Features::strictMode());
    Json::Value jvValue;

    KfkNewOrder neworder;
    if((pRecord[0] == '{') && (jReader.parse(pRecord, jvValue)))
    {
        if(jvValue.isMember("memguid"))
        {
            neworder.set_memguid(jvValue["memguid"].asCString());
        }
        if(jvValue.isMember("ogno"))
        {
            neworder.set_ogno(jvValue["ogno"].asCString());
        }
        if(jvValue.isMember("ogseq"))
        {
            neworder.set_ogseq(jvValue["ogseq"].asCString());
        }
        /*if(jvValue.isMember("timestamp"))
        {
            neworder.set_timestamp(jvValue["timestamp"].asUInt());
        }
        if(jvValue.isMember("succ"))
        {
            neworder.set_bsuccess(jvValue["succ"].asUInt());
        }*/

        uint32_t msg_size = neworder.ByteSize();
        char* szData = new char[msg_size];
        neworder.SerializeToArray(szData, msg_size);

		MsgPushInner msgpush;
		msgpush.set_mptype(MP_NEWORDER);
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
