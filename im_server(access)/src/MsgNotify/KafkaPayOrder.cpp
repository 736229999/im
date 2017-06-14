#include "KafkaPayOrder.h"
#include "Log.h"
#include "json/json.h"
#include "msgnotifyinner.pb.h"
#include "AppManager.h"
#include "PacketBuffer.h"


CKafkaPayOrder::CKafkaPayOrder(const char *pBrokers, const char *pTopic)
	: CKafKaConsumer(pBrokers, pTopic)
{

}

void CKafkaPayOrder::GetNewRecord(const char *pRecord)
{
	CLog::Log("CKafkaPayOrder",CLog::TYPE_DEBUG, "Topic:%s, Record: %s", GetTopicStr(), pRecord);

	// {"AMOUNT":"0","GATEWAY":"","MEM_GUID":"A8A1A7FE-1DE0-3D29-E0C0-849C62815BEA","OG_NO":"201601CP14036753","OG_SEQ":"201601CO14036753","PAY_TYPE":"14","TRANS_NO":"","v":"1.0.0"}
    Json::Reader jReader(Json::Features::strictMode());
    Json::Value jvValue;

    KfkPayOrder payorder;
    if((pRecord[0] == '{') && (jReader.parse(pRecord, jvValue)))
    {
        if(jvValue.isMember("MEM_GUID"))
        {
            payorder.set_memguid(jvValue["MEM_GUID"].asCString());
        }
        if(jvValue.isMember("OG_NO"))
        {
            payorder.set_ogno(jvValue["OG_NO"].asCString());
        }
        if(jvValue.isMember("OG_SEQ"))
        {
            payorder.set_ogseq(jvValue["OG_SEQ"].asCString());
        }
        if(jvValue.isMember("PAY_TYPE"))
        {
            payorder.set_paytype(jvValue["PAY_TYPE"].asCString());
        }
        if(jvValue.isMember("TRANS_NO"))
        {
            payorder.set_transno(jvValue["TRANS_NO"].asCString());
        }

        uint32_t msg_size = payorder.ByteSize();
        char* szData = new char[msg_size];
        payorder.SerializeToArray(szData, msg_size);

		MsgPushInner msgpush;
		msgpush.set_mptype(MP_PAYORDER);
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
