#pragma once

#include <string>
#include <zookeeper.h>
#include <zookeeper.jute.h>

#include "../include/kafka/rdkafka.h"  /* for Kafka driver */
#include "WinCommonForLinux.h"

class CKafKaConsumer
{
public:
    CKafKaConsumer(const char *pZookeeper, const char *pTopic);
    ~CKafKaConsumer();
    int StartWork();
    const char* GetTopicStr()
    {
        return m_topic_str.c_str();
    }
    bool m_bRun;
    virtual void GetNewRecord(const char *pRecord) = 0; // ���յ��µļ�¼

public:
    rd_kafka_t *rk;

protected:
    bool Init();
    static void* KafkaConsumerProc( LPVOID lpParam );
    // ����̣߳����жϳ��������̶߳�����ʱ�����¿����߳�
    static void* ThreadMonitor( LPVOID lpParam );
    void Release();

    rd_kafka_topic_t *rkt;
    zhandle_t *zh;
    rd_kafka_conf_t *conf;
    rd_kafka_topic_conf_t *topic_conf;

    std::string m_strZookeeper;
    std::string m_topic_str;
    int64_t start_offset;
};

