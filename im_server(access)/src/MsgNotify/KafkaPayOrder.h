#pragma once

#include "KafkaConsumer.h"

class CKafkaPayOrder : public CKafKaConsumer
{
public:
	CKafkaPayOrder(const char *pBrokers, const char *pTopic);

	void GetNewRecord(const char *pRecord);
};

