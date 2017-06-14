#pragma once

#include "KafkaConsumer.h"

class CKafkaGoodsComment : public CKafKaConsumer
{
public:
	CKafkaGoodsComment(const char *pBrokers, const char *pTopic);

	void GetNewRecord(const char *pRecord);
};

