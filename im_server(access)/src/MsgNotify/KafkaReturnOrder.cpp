#include "KafkaReturnOrder.h"
#include "Log.h"
#include "json/json.h"
#include "msgnotifyinner.pb.h"
#include "AppManager.h"
#include "PacketBuffer.h"


CKafkaReturnOrder::CKafkaReturnOrder(const char *pBrokers, const char *pTopic)
	: CKafKaConsumer(pBrokers, pTopic)
{

}

void CKafkaReturnOrder::GetNewRecord(const char *pRecord)
{
	CLog::Log("CKafkaReturnOrder",CLog::TYPE_DEBUG, "Topic:%s, Record: %s", GetTopicStr(), pRecord);

	//{"memGuid":"E114D50E-C8BB-2F8B-FF99-906331382B5D","ogNO":"201601CP20037604","ogSeq":"201601CO20037604","ogsSeq":"201601COGS2010006268","rsSeq":"RS20160120C010008092"}

	Json::Reader jReader(Json::Features::strictMode());
    Json::Value jvValue;

    KfkReturnOrder returnorder;
    if((pRecord[0] == '{') && (jReader.parse(pRecord, jvValue)))
    {
        if(jvValue.isMember("memGuid"))
        {
            returnorder.set_memguid(jvValue["memGuid"].asCString());
        }
        if(jvValue.isMember("ogSeq"))
        {
            returnorder.set_ogseq(jvValue["ogSeq"].asCString());
        }
        if(jvValue.isMember("ogNO"))
        {
            returnorder.set_ogno(jvValue["ogNO"].asCString());
        }
        if(jvValue.isMember("ogsSeq"))
        {
            returnorder.set_ogsseq(jvValue["ogsSeq"].asCString());
        }
        if(jvValue.isMember("rsSeq"))
        {
            returnorder.set_rsseq(jvValue["rsSeq"].asCString());
        }

        uint32_t msg_size = returnorder.ByteSize();
        char* szData = new char[msg_size];
        returnorder.SerializeToArray(szData, msg_size);

		MsgPushInner msgpush;
		msgpush.set_mptype(MP_RETURNORDER);
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
