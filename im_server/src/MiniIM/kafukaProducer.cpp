
#include "kafukaProducer.h"
#include "AppManager.h"
#include "Function.h"


KafKa_Producer::KafKa_Producer()
{
    /*
        static config
        ./rdkafka_example_cpp -P -b kafka01.dev1.fn:9092,kafka02.dev1.fn:9092,kafka03.dev1.fn:9092 -t moumou_topic_flow
        ./rdkafka_example_cpp -C -b kafka01.dev1.fn:9092,kafka02.dev1.fn:9092,kafka03.dev1.fn:9092 -t moumou_topic_flow -p 0
    */
    topic = NULL;
	producer = NULL;
}

KafKa_Producer::~KafKa_Producer()
{
    if(topic != NULL)delete topic;
    if(producer != NULL)delete producer;
}


void KafKa_Producer::KafKa_init()
{
	CConfigInfo m_Config;
    mode = 'P';
    brokers = m_Config.GetKafuKaBroker();
    topic_str = m_Config.GetKafuKaTopic();

	CLog::Log("KafKa_Producer", CLog::TYPE_DEBUG,"Create producer: brokers :%s topics: %s" ,brokers.c_str(),topic_str.c_str());

//	brokers = "kafka01.dev1.fn:9092,kafka02.dev1.fn:9092,kafka03.dev1.fn:9092";
//	topic_str = "moumou_topic_flow";
    partition = RdKafka::Topic::PARTITION_UA;
    start_offset = RdKafka::Topic::OFFSET_BEGINNING;
    conf = RdKafka::Conf::create(RdKafka::Conf::CONF_GLOBAL);
    tconf = RdKafka::Conf::create(RdKafka::Conf::CONF_TOPIC);

    conf->set("metadata.broker.list", brokers, errstr);
    conf->set("event_cb", &ex_event_cb, errstr);


    /* Set delivery report callback */
    conf->set("dr_cb", &ex_dr_cb, errstr);
    /*
     * Create producer using accumulated global configuration.
     */
    producer = RdKafka::Producer::create(conf, errstr);

    if (!producer)
    {    	
		CLog::Log("KafKa_Producer", CLog::TYPE_DEBUG,"Failed to create producer: %s",errstr.c_str());
        exit(1);
    }
    /*
     * Create topic handle.
     */
    topic = RdKafka::Topic::create(producer, topic_str,
                                   tconf, errstr);
    if (!topic)
    {
    	CLog::Log("KafKa_Producer", CLog::TYPE_DEBUG,"Failed to create topic: %s",errstr.c_str());
        exit(1);
    }

}

void *KafKa_Producer::KafKa_ProducerPorc( LPVOID lpParam )
{
    KafKa_Producer* pKafKa_Producer = (KafKa_Producer*)lpParam;
    while(true)
    {
        //XT_JSON_STRING xtJsonStr;
        char xtJsonStr[8192] = {0};
		
        char xtUtfJsonStr[8192] = {0};
        int nMsgLen = 0;
        CAppManager::GetInstance().GetKafKaMsgBuffer()->Pop((BYTE*)xtJsonStr, nMsgLen);
        CLog::Log("KafKa_Producer", CLog::TYPE_DEBUG, "KafKaBuffer %u len %d",nMsgLen,CAppManager::GetInstance().GetKafKaMsgBuffer()->GetSize());
		if(nMsgLen <= 0 || nMsgLen > 8190)
		{
			continue;
		}
		gbk2utf8(xtUtfJsonStr,xtJsonStr);

        /*
               * Produce message
               */
//		static int seq = 0;
//		sprintf(xtJsonStr,"{\"test\":\"duyong_test\",\"seq\":%d}",seq++);
        RdKafka::ErrorCode resp =
            pKafKa_Producer->producer->produce(pKafKa_Producer->topic, pKafKa_Producer->partition,
                              RdKafka::Producer::RK_MSG_COPY /* Copy payload */,
                              const_cast<char *>(xtUtfJsonStr), strlen(xtUtfJsonStr) ,
                              NULL, NULL);
        if (resp != RdKafka::ERR_NO_ERROR)
            CLog::Log("KafKa_Producer", CLog::TYPE_DEBUG,"Produce failed %s", RdKafka::err2str(resp).c_str());
        else
            CLog::Log("KafKa_Producer", CLog::TYPE_DEBUG,"Produce  %s len %d", xtJsonStr,strlen(xtJsonStr));

        pKafKa_Producer->producer->poll(0);
//		sleep(5);

    }

#if 0
    /*
     * Read messages from stdin and produce to broker.
     */
    for (std::string line; run && std::getline(std::cin, line);)
    {
        if (line.empty())
        {
            producer->poll(0);
            continue;
        }

        /*
         * Produce message
         */
        RdKafka::ErrorCode resp =
            producer->produce(topic, partition,
                              RdKafka::Producer::RK_MSG_COPY /* Copy payload */,
                              const_cast<char *>(line.c_str()), line.size(),
                              NULL, NULL);
        if (resp != RdKafka::ERR_NO_ERROR)
            std::cerr << "% Produce failed: " <<
                      RdKafka::err2str(resp) << std::endl;
        else
            std::cerr << "% Produced message (" << line.size() << " bytes)" <<
                      std::endl;

        producer->poll(0);
    }
    run = true;

    while (run && producer->outq_len() > 0)
    {
        std::cerr << "Waiting for " << producer->outq_len() << std::endl;
        producer->poll(1000);
    }
#endif
	return NULL;
}

int KafKa_Producer::StartWork()
{
    //初始化数据库
    CConfigInfo m_Config;
    char miniuid[32],minipwd[32],minisid[32],webuid[32],webpwd[32],websid[32],familyuid[32],familypwd[32],familysid[32];
    m_Config.GetDataBaseConfig(miniuid, minipwd, minisid,webuid,webpwd,websid,familyuid,familypwd,familysid);
    m_pDataBaseMini.SetConnectDB(miniuid,minipwd,minisid);


    if( !m_pDataBaseMini.ConnectDB() )
    {
        CLog::Log("KafKa_Producer",CLog::TYPE_ERROR,"DatabaseMini.ConnectDB() failed!");
        return RESULT_FAILED;
    }
	KafKa_init();

    CreateThread(
        NULL,                       // default security attributes
        0,                          // use default stack size
        KafKa_ProducerPorc,                 // thread function
        this,                       // argument to thread function
        0,                          // use default creation flags
        NULL);                      // returns the thread identifier

//  StartEraseTimer();
    return RESULT_SUCCESS;
}



