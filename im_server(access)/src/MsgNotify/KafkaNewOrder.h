#pragma once

#include "KafkaConsumer.h"

class CKafkaNewOrder : public CKafKaConsumer
{
public:
	CKafkaNewOrder(const char *pBrokers, const char *pTopic);
	
	void GetNewRecord(const char *pRecord);
};