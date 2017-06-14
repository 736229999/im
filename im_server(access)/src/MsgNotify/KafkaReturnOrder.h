#pragma once

#include "KafkaConsumer.h"

class CKafkaReturnOrder : public CKafKaConsumer
{
public:
	CKafkaReturnOrder(const char *pBrokers, const char *pTopic);

	void GetNewRecord(const char *pRecord);
};

