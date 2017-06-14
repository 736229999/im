#pragma once
#include "DataBaseLib.h"
#include "ClientProtocol.h"
#include "SyncCritical.h"

#include <iostream>
#include <string>
#include <cstdlib>
#include <cstdio>
#include <csignal>
#include <cstring>

/*
 * Typically include path in a real application would be
 * #include <librdkafka/rdkafkacpp.h>
 */
#include "../include/kafka/rdkafkacpp.h"


class ExampleDeliveryReportCb : public RdKafka::DeliveryReportCb {
 public:
  void dr_cb (RdKafka::Message &message) {
    std::cout << "Message delivery for (" << message.len() << " bytes): " <<
        message.errstr() << std::endl;
  }
};


class ExampleEventCb : public RdKafka::EventCb {
 public:
  void event_cb (RdKafka::Event &event) {
    switch (event.type())
    {
      case RdKafka::Event::EVENT_ERROR:
        std::cerr << "ERROR (" << RdKafka::err2str(event.err()) << "): " <<
            event.str() << std::endl;
        if (event.err() == RdKafka::ERR__ALL_BROKERS_DOWN)
         break;

      case RdKafka::Event::EVENT_STATS:
        std::cerr << "\"STATS\": " << event.str() << std::endl;
        break;

      case RdKafka::Event::EVENT_LOG:
        fprintf(stderr, "LOG-%i-%s: %s\n",
                event.severity(), event.fac().c_str(), event.str().c_str());
        break;

      default:
        std::cerr << "EVENT " << event.type() <<
            " (" << RdKafka::err2str(event.err()) << "): " <<
            event.str() << std::endl;
        break;
    }
  }
};
 

class KafKa_Producer
{
public:
	KafKa_Producer();
	~KafKa_Producer();
	void KafKa_init();
	void Start_KafKa_Producer();	
	int StartWork();
	static void* KafKa_ProducerPorc( LPVOID lpParam );
private:
	  std::string brokers;
	  std::string errstr;
	  std::string topic_str;
	  std::string mode;
	  RdKafka::Conf *conf;
	  RdKafka::Conf *tconf;
	  int32_t partition;
	  int64_t start_offset;
	  ExampleEventCb ex_event_cb;	  
	  ExampleDeliveryReportCb ex_dr_cb;
	  RdKafka::Producer *producer;
	  RdKafka::Topic *topic;
	  CDataBaseMini m_pDataBaseMini;
	  
};

