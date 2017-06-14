#include "KafkaConsumer.h"
#include "AppManager.h"
#include <iostream>
#include <string>
#include <cstdlib>
#include <cstdio>
#include <csignal>
#include <cstring>

#include <getopt.h>
#include <unistd.h>
#include "Log.h"
#include "Function.h"

using namespace std;


#define __STDC_FORMAT_MACROS
#include <ctype.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <syslog.h>
#include <sys/time.h>
#include <errno.h>

/* Typical include path would be <librdkafka/rdkafka.h>, but this program
 * is builtin from within the librdkafka source tree and thus differs. */



#include <jansson.h>


#define BROKER_PATH "/brokers/ids"
/*
static  enum
{
    OUTPUT_HEXDUMP,
    OUTPUT_RAW,
} output = OUTPUT_HEXDUMP;
*/
/*
static void hexdump (FILE *fp, const char *name, const void *ptr, size_t len)
{
    const char *p = (const char *)ptr;
    int of = 0;


    if (name)
        fprintf(fp, "%s hexdump (%zd bytes):\n", name, len);

    for (of = 0 ; of < (int)len ; of += 16)
    {
        char hexen[16*3+1];
        char charen[16+1];
        int hof = 0;

        int cof = 0;
        int i;

        for (i = of ; i < (int)of + 16 && i < (int)len ; i++)
        {
            hof += sprintf(hexen+hof, "%02x ", p[i] & 0xff);
            cof += sprintf(charen+cof, "%c",
                           isprint((int)p[i]) ? p[i] : '.');
        }
        fprintf(fp, "%08x: %-48s %-16s\n",
                of, hexen, charen);
    }
}
*/
/**
 * Kafka logger callback (optional)
 */
static void logger (const rd_kafka_t *rk, int level,
                    const char *fac, const char *buf)
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    
    CLog::Log("logger",CLog::TYPE_IMPORTANT, "%u.%03u RDKAFKA-%i-%s: %s: %s",
              (int)tv.tv_sec, (int)(tv.tv_usec / 1000),
              level, fac, rd_kafka_name(rk), buf);
}

/**
 * Message delivery report callback.
 * Called once for each message.
 * See rdkafka.h for more information.
 */
 /*
static void msg_delivered (rd_kafka_t *rk,
                           void *payload, size_t len,
                           rd_kafka_resp_err_t error_code,
                           void *opaque, void *msg_opaque)
{

    if (error_code)
        CLog::Log("logger",CLog::TYPE_ERROR,"%% Message delivery failed: %s",
                  rd_kafka_err2str((rd_kafka_resp_err_t)error_code));
    //else if (!quiet)
    //    CLog::Log("logger",CLog::TYPE_ERROR, "%% Message delivered (%zd bytes)\n", len);
}
*/

static void msg_consume (rd_kafka_message_t *rkmessage,
                         CKafKaConsumer *pThis)
{
    if (rkmessage->err)
    {
        if (rkmessage->err == RD_KAFKA_RESP_ERR__PARTITION_EOF)
        {
            CLog::Log("msg_consume",CLog::TYPE_IMPORTANT,
                      "%% Consumer reached end of %s [%d] message queue at offset %lld",
                      rd_kafka_topic_name(rkmessage->rkt),
                      rkmessage->partition, rkmessage->offset);

            /*if (exit_eof)
                run = 0;*/

            return;
        }

        CLog::Log("msg_consume",CLog::TYPE_ERROR, "%% Consume error for topic \"%s\" [%d] offset %lld: %s",
                  rd_kafka_topic_name(rkmessage->rkt),
                  rkmessage->partition,
                  rkmessage->offset,
                  rd_kafka_message_errstr(rkmessage));
        return;
    }

    /*if (!quiet)*/
    CLog::Log("msg_consume",CLog::TYPE_IMPORTANT, "%% Message (offset %lld, %zd bytes):",
              rkmessage->offset, rkmessage->len);

    if (rkmessage->key_len)
    {
        /*if (output == OUTPUT_HEXDUMP)
            hexdump(stdout, "Message Key",
                rkmessage->key, rkmessage->key_len);
        else*/
        CLog::Log("msg_consume",CLog::TYPE_IMPORTANT,"Key: %.*s",
                  (int)rkmessage->key_len, (char *)rkmessage->key);
    }


    /*if (output == OUTPUT_HEXDUMP)
        CLog::Log("msg_consume",CLog::TYPE_IMPORTANT, "Message Payload",
            rkmessage->payload, rkmessage->len);
    else*/
    CLog::Log("msg_consume",CLog::TYPE_IMPORTANT, "%.*s",
              (int)rkmessage->len, (char *)rkmessage->payload);

    // utf8转GBK
    int inlen = static_cast<int>(rkmessage->len);
    char szRecord[2048] = {0};
    int length = 2048;
    utf2GB18030(static_cast<char *>(rkmessage->payload),inlen,szRecord,length);
    pThis->GetNewRecord(szRecord);
}

/*
static void metadata_print (const char *topic,
                            const struct rd_kafka_metadata *metadata)
{
    int i, j, k;

    CLog::Log("metadata_print",CLog::TYPE_IMPORTANT,"Metadata for %s (from broker %d: %s):",
              topic ? : "all topics",
              metadata->orig_broker_id,
              metadata->orig_broker_name);


    CLog::Log("metadata_print",CLog::TYPE_IMPORTANT," %i brokers:", metadata->broker_cnt);
    for (i = 0 ; i < metadata->broker_cnt ; i++)
        CLog::Log("metadata_print",CLog::TYPE_IMPORTANT,"  broker %d at %s:%i",
                  metadata->brokers[i].id,
                  metadata->brokers[i].host,
                  metadata->brokers[i].port);

    CLog::Log("metadata_print",CLog::TYPE_IMPORTANT," %i topics:", metadata->topic_cnt);
    for (i = 0 ; i < metadata->topic_cnt ; i++)
    {
        const struct rd_kafka_metadata_topic *t = &metadata->topics[i];
        CLog::Log("metadata_print",CLog::TYPE_IMPORTANT,"  topic \"%s\" with %i partitions:",
                  t->topic,
                  t->partition_cnt);
        if (t->err)
        {
            CLog::Log("metadata_print",CLog::TYPE_IMPORTANT," %s", rd_kafka_err2str(t->err));
            if (t->err == RD_KAFKA_RESP_ERR_LEADER_NOT_AVAILABLE)
                CLog::Log("metadata_print",CLog::TYPE_IMPORTANT," (try again)");
        }

        for (j = 0 ; j < t->partition_cnt ; j++)
        {
            const struct rd_kafka_metadata_partition *p;
            p = &t->partitions[j];
            CLog::Log("metadata_print",CLog::TYPE_IMPORTANT,"    partition %d, leader %d, replicas: ",
                      p->id, p->leader);

            for (k = 0 ; k < p->replica_cnt ; k++)
                CLog::Log("metadata_print",CLog::TYPE_IMPORTANT,"%s%d",
                          k > 0 ? ",":"", p->replicas[k]);

            CLog::Log("metadata_print",CLog::TYPE_IMPORTANT,", isrs: ");
            for (k = 0 ; k < p->isr_cnt ; k++)
                CLog::Log("metadata_print",CLog::TYPE_IMPORTANT,"%s%d",
                          k > 0 ? ",":"", p->isrs[k]);
            if (p->err)
                CLog::Log("metadata_print",CLog::TYPE_IMPORTANT,", %s", rd_kafka_err2str(p->err));
        }
    }
}
*/

static void set_brokerlist_from_zookeeper(zhandle_t *zzh, char *brokers)
{
    if (zzh)
    {
        struct String_vector brokerlist;
        if (zoo_get_children(zzh, BROKER_PATH, 1, &brokerlist) != ZOK)
        {
            CLog::Log("set_brokerlist_from_zookeeper",CLog::TYPE_ERROR, "No brokers found on path %s", BROKER_PATH);
            return;
        }

        int i;
        char *brokerptr = brokers;
        for (i = 0; i < brokerlist.count; i++)
        {
            char path[255], cfg[1024];
            sprintf(path, "/brokers/ids/%s", brokerlist.data[i]);
            int len = sizeof(cfg);
            zoo_get(zzh, path, 0, cfg, &len, NULL);

            if (len > 0)
            {
                cfg[len] = '\0';
                json_error_t jerror;
                json_t *jobj = json_loads(cfg, 0, &jerror);
                if (jobj)
                {
                    json_t *jhost = json_object_get(jobj, "host");
                    json_t *jport = json_object_get(jobj, "port");

                    if (jhost && jport)
                    {
                        const char *host = json_string_value(jhost);
                        const int   port = json_integer_value(jport);
                        sprintf(brokerptr, "%s:%d", host, port);

                        brokerptr += strlen(brokerptr);
                        if (i < brokerlist.count - 1)
                        {
                            *brokerptr++ = ',';
                        }
                    }
                    json_decref(jobj);
                }
            }
        }
        deallocate_String_vector(&brokerlist);
        CLog::Log("set_brokerlist_from_zookeeper",CLog::TYPE_IMPORTANT,"Found brokers %s", brokers);
    }
}


static void watcher(zhandle_t *zh, int type, int state, const char *path, void *watcherCtx)
{
    char brokers[1024] = {0};
    CLog::Log("watcher",CLog::TYPE_IMPORTANT,"type [%d] path [%s]", type, path);
    if (type == ZOO_CHILD_EVENT && strncmp(path, BROKER_PATH, sizeof(BROKER_PATH) - 1) == 0)
    {
        CKafKaConsumer *pThis = (CKafKaConsumer *)zoo_get_context(zh);

        brokers[0] = '\0';
        set_brokerlist_from_zookeeper(zh, brokers);
        CLog::Log("watcher",CLog::TYPE_IMPORTANT,"brokers [%s]", brokers);
        if (brokers[0] != '\0' && pThis->rk != NULL)
        {
            rd_kafka_brokers_add(pThis->rk, brokers);
            rd_kafka_poll(pThis->rk, 10);
        }
    }
}


static zhandle_t* initialize_zookeeper(const char * zookeeper, const int debug, CKafKaConsumer *pThis)
{
    zhandle_t *zh = NULL;
    if (debug)
    {
        zoo_set_debug_level(ZOO_LOG_LEVEL_DEBUG);
    }
    zh = zookeeper_init(zookeeper, watcher, 10000, 0, (void*)pThis, 0);
    if (zh == NULL)
    {
        CLog::Log("set_brokerlist_from_zookeeper",CLog::TYPE_ERROR, "Zookeeper connection not established.");
    }
    return zh;
}

/*
static void sig_usr1 (int sig) {
    rd_kafka_dump(stdout, rk);
}
*/


// 线程参数
struct ThreadParam
{
    CKafKaConsumer *pThis;
    int32_t nPartition;
};

CKafKaConsumer::CKafKaConsumer(const char *pZookeeper, const char *pTopic)
{
    m_strZookeeper = pZookeeper;
    m_topic_str = pTopic;
    rk = NULL;
    rkt = NULL;
    zh = NULL;
    conf = NULL;
    topic_conf = NULL;
}

CKafKaConsumer::~CKafKaConsumer()
{
    Release();
}

void CKafKaConsumer::Release()
{
    rd_kafka_topic_destroy(rkt);

    rd_kafka_destroy(rk);

    /* Let background threads clean up and terminate cleanly. */
    rd_kafka_wait_destroyed(2000);

    /** Free the zookeeper data. */
    zookeeper_close(zh);
}

void* CKafKaConsumer::KafkaConsumerProc( LPVOID lpParam )
{
    ThreadParam *pParam = (ThreadParam*)lpParam;
    CKafKaConsumer *pThis = pParam->pThis;
    int32_t partition = pParam->nPartition;

    CLog::Log("CKafKaConsumer",CLog::TYPE_DEBUG, "Topic:%s  开启线程接收数据", pThis->GetTopicStr());

    //int use_ccb = 1;


    /* Start consuming */
    if (rd_kafka_consume_start(pThis->rkt, partition, pThis->start_offset) == -1)
    {
        CLog::Log("CKafKaConsumer",CLog::TYPE_ERROR, "%% Failed to start consuming: %s",
                  rd_kafka_err2str(rd_kafka_errno2err(errno)));
        return NULL;
    }

    while (CAppManager::g_bRun && pThis->m_bRun)
    {
        rd_kafka_message_t *rkmessage;

        /* Consume single message.
         * See rdkafka_performance.c for high speed
         * consuming of messages. */
        rkmessage = rd_kafka_consume(pThis->rkt, partition, 1000);
        if (!rkmessage) /* timeout */
            continue;

        msg_consume(rkmessage, pThis);

        /* Return message to rdkafka */
        rd_kafka_message_destroy(rkmessage);
    }

    /* Stop consuming */
    rd_kafka_consume_stop(pThis->rkt, partition);

    CLog::Log("CKafKaConsumer",CLog::TYPE_ERROR, "Topic:%s CLOSE CAppManager::g_bRun:%d pThis->m_bRun:%d", pThis->GetTopicStr(), CAppManager::g_bRun, pThis->m_bRun);

    return NULL;
}


bool CKafKaConsumer::Init()
{
	m_bRun = true;

    /* Kafka configuration */
    conf = rd_kafka_conf_new();

    /* Topic configuration */
    topic_conf = rd_kafka_topic_conf_new();

    start_offset = RD_KAFKA_OFFSET_END;

    /** Initialize zookeeper */
    zh = initialize_zookeeper(m_strZookeeper.c_str(), 1, this);

    /* Add brokers */
    char brokers[2048] = {0};
    char errstr[2048] = {0};
    set_brokerlist_from_zookeeper(zh, brokers);
    if (rd_kafka_conf_set(conf, "metadata.broker.list",
                          brokers, errstr, sizeof(errstr) !=
                          RD_KAFKA_CONF_OK))
    {
        CLog::Log("CKafKaConsumer",CLog::TYPE_ERROR, "%% Failed to set brokers: %s", errstr);
        return false;
    }

    CLog::Log("CKafKaConsumer",CLog::TYPE_IMPORTANT, "Broker list from zookeeper cluster %s: %s", m_strZookeeper.c_str(), brokers);


    /* Create Kafka handle */
    if (!(rk = rd_kafka_new(RD_KAFKA_CONSUMER, conf,
                            errstr, sizeof(errstr))))
    {
        CLog::Log("CKafKaConsumer",CLog::TYPE_ERROR,
                  "%% Failed to create new consumer: %s",
                  errstr);
        return false;
    }

    /* Set logger */
    rd_kafka_conf_set_log_cb(conf, logger);
    rd_kafka_set_log_level(rk, LOG_DEBUG);

    /* Create topic */
    rkt = rd_kafka_topic_new(rk, m_topic_str.c_str(), topic_conf);

    return true;
}

int CKafKaConsumer::StartWork()
{
    if(!Init())
    	return -1;

    int m_partition = 3;

    // 创建线程
    pthread_t *pArrChildThread = new pthread_t[4];
    for(int32_t i = 0; i < m_partition; ++i)
    {
        ThreadParam *pParam = new ThreadParam;
        pParam->pThis = this;
        pParam->nPartition = i;

        pArrChildThread[i] = CreateThread(
                                 NULL,                       // default security attributes
                                 0,                          // use default stack size
                                 KafkaConsumerProc,                 // thread function
                                 pParam,                     // argument to thread function
                                 0,                          // use default creation flags
                                 NULL);                      // returns the thread identifier
    }

    //pArrChildThread[3] = (pthread_t)this;   // 最后一位存this
    //CreateThread(NULL, 0, ThreadMonitor, (void*)pArrChildThread, 0, NULL);

    return 0;
}

// 监控线程，当判断出三个子线程都结束时，重新开启线程
void* CKafKaConsumer::ThreadMonitor( LPVOID lpParam )
{
    pthread_t *pArrChildThread = (pthread_t*)lpParam;
    CKafKaConsumer *pThis = (CKafKaConsumer *)(pArrChildThread[3]);

    CLog::Log("CKafKaConsumer",CLog::TYPE_IMPORTANT, "Topic:%s 监控线程开启", pThis->GetTopicStr());

    // 等待三个线程都结束后，重新开启线程
    WaitForMultipleObjects(3, pArrChildThread, TRUE, -1);
    pThis->m_bRun = true;
    CLog::Log("CKafKaConsumer",CLog::TYPE_IMPORTANT, "Topic:%s 子线程都已结束", pThis->GetTopicStr());

    pThis->StartWork();

    delete[] pArrChildThread;

    return NULL;
}

