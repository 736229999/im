#ifndef SSDBCONN_H
#define SSDBCONN_H

#include "SSDB.h"
#include "../common/Utility.h"
#include "../include/PublicType.h"
#include "../include/ClientProtocol.h"
#include <string>

using namespace std;

enum linkstat{LINK_IDLE = 0, LINK_USE = 1};

class ssdbClient
{
public:
	ssdbClient(){s = LINK_IDLE;l = NULL;};
	~ssdbClient()
	{
		if(l != NULL)
		{
			delete l;
			l = NULL;
		}	
	};
		
	uint32 s;
	ssdb::Client* l; 
};

typedef SafeMap<uint32,ssdbClient*> ssdbLink_Map;

class SSDBLinks
{
public:
			
	SSDBLinks();
	~SSDBLinks();

	static SSDBLinks& GetInstance();

	ssdbLink_Map& GetLinks(){return Links;};

	int init_links(unsigned int num,const char *ip,int port);

	ssdbClient* getLink();

	int releaseLink(ssdbClient* client);

	//hashmap name && key
	void genhName(uint32 id,string& hName);

	void genhKey(uint32 key,string& hKey);

	//talk message
	void genhValue(XT_TALK* msg,string& hValue);

	void genhValue(uint32 fromid,uint32 toid,string& hValue);
	
	//hash set && get
	ssdb::Status hSetData(ssdb::Client* link, string& hash_name, string& key, string& value);

	ssdb::Status hGetData(ssdb::Client* link, string& hash_name, string& key, string& value);

	ssdb::Status hScanData(ssdb::Client* link, string& hash_name, string& key_start, string& key_end, 
		uint32 limit, vector<string>* value);

	//zset
	void genzName(uint32 fid,string& z_name);
	
	void genzKey(uint32 did,string& key);
	
	void genzValue(uint32 fid,uint32 did,string& value);

	//split
	int split(string& str, string& pattern, vector<string>* ret);
	int genMsg(string& str, vector<string>& sub_str, XT_TALK* msg);

private:

	ssdbLink_Map Links;

	uint32 max_links;
	
	string symbol = ":";

	enum element{FID=0,DID=1,TYPE=2,VER=3,TIME=4,FLAG=5,FSIZE=6,FCOLOR=7,FSTYLE=8,FNAME=9,DATA=10,NICK=11};
};

#endif
