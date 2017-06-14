#include "SSDBConn.h"
#include "../common/Log.h"
#include <sstream>

SSDBLinks::SSDBLinks()
{

}

SSDBLinks::~SSDBLinks()
{

}

SSDBLinks& SSDBLinks::GetInstance()
{
	static SSDBLinks SingleInstance;
	return SingleInstance;
}

int SSDBLinks::init_links(unsigned int num,const char *ip,int port)
{
	max_links = num;
	
	for(unsigned int i = 0; i < num; i ++)
	{
		ssdb::Client *link = ssdb::Client::connect(ip, port);
		if(link == NULL)
		{
			CLog::Log("SSDBLinks::init_links",CLog::TYPE_ERROR,"connect to ssdb server error");
			return -1;
		}
		else
		{
			try
			{
				ssdbClient* client = new ssdbClient;
				client->l = link;
				Links.insert(make_pair(i,client));
			}
			catch(...)
			{
				CLog::Log("SSDBLinks::init_links",CLog::TYPE_ERROR,"links insert error");
			}
		}
	}
	return 0;
}

ssdbClient* SSDBLinks::getLink()
{
	CAutoLock mylock(&Links.m_mapLock);
	//ssdb::Client* link = NULL;
	ssdbLink_Map::iterator iter;
	
	for(iter = Links.begin(); iter != Links.end(); ++ iter)
	{
		if(iter->second->s == LINK_IDLE && iter->second->l != NULL)
		{
			iter->second->s = LINK_USE;
			CLog::Log("SSDBLinks::getLink",CLog::TYPE_IMPORTANT,"ssdb get link[%d]",iter->first);

			break;
		}	
	}

	if(iter == Links.end())
	{
		CLog::Log("SSDBLinks::getLink",CLog::TYPE_ERROR,"ssdb pool has no link for use");
		return NULL;
	}

	return iter->second;
}

int SSDBLinks::releaseLink(ssdbClient* client)
{
	CAutoLock mylock(&Links.m_mapLock);
	ssdbLink_Map::iterator iter;

	if(client != NULL)
	{
		if(client->s != LINK_USE)
		{
			CLog::Log("SSDBLinks::releaseLink",CLog::TYPE_ERROR,"link is in use");
			return -1;
		}
		else
		{
			for(iter = Links.begin(); iter != Links.end(); ++ iter)
			{
				if(client == iter->second)
				{
					iter->second->s = LINK_IDLE;
					break;
				}
			}
			if(iter == Links.end())
			{
				CLog::Log("SSDBLinks::releaseLink",CLog::TYPE_ERROR,"link is in not in pool");
				return -1;
			}	
		}
	}

	return 0;
}

void SSDBLinks::genhName(uint32 id,string& hName)
{
	stringstream stream;

	stream<<id;

	hName = stream.str();

}

void SSDBLinks::genhKey(uint32 key,string& hkey)
{
	stringstream stream;

	stream<<key;

	hkey = stream.str();

}

void SSDBLinks::genhValue(XT_TALK* msg,string& hValue)
{
	stringstream fid,did,type,ver,time,flag,fsize,fcolor,fstyle;

	fid<<msg->from_id;
	did<<msg->to_id;
	type<<(uint32)msg->data_type;
	ver<<msg->ver;
	time<<msg->send_time;
	flag<<(uint32)msg->recv_flag;
	fsize<<(uint32)msg->fontSize;
	fcolor<<msg->fontColor;
	fstyle<<msg->fontStyle;

	hValue = fid.str() + symbol + did.str() + symbol + type.str() + symbol + ver.str() + symbol + time.str() + symbol + flag.str() 
			+ symbol + fsize.str() + symbol + fcolor.str() + symbol + fstyle.str() + symbol + msg->fontName + symbol + msg->data + symbol + msg->from_nickname;
}

void SSDBLinks::genhValue(uint32 fromid,uint32 toid,string& hValue)
{
	stringstream fid,did;

	fid<<fromid;
	did<<toid;

	hValue = fid.str() + did.str();
}

void SSDBLinks::genzName(uint32 fid,string& z_name)
{
	stringstream stream;

	stream<<fid;

	z_name = stream.str();

}

void SSDBLinks::genzKey(uint32 did,string& key)
{
	stringstream stream;

	stream<<did;

	key = stream.str();
}

void SSDBLinks::genzValue(uint32 fid,uint32 did,string& value)
{
	stringstream f_id,d_id;

	f_id<<fid;
	d_id<<did;

	value = f_id.str() + d_id.str();
}

ssdb::Status SSDBLinks::hSetData(ssdb::Client* link, string& hash_name, string& key, string& value)
{
	ssdb::Status ret;

	ret = link->hset(hash_name,key,value);

	return ret;		
}

ssdb::Status SSDBLinks::hGetData(ssdb::Client* link, string& hash_name, string& key, string& value)
{
	ssdb::Status ret;

	ret = link->hget(hash_name,key,&value);

	return ret;		
}

ssdb::Status SSDBLinks::hScanData(ssdb::Client* link, string& hash_name, string& key_start, string& key_end, 
	uint32 limit, vector<string>* value)
{
	ssdb::Status ret;

	if(key_end.empty())
		key_end = "";

	ret = link->hscan(hash_name,key_start,key_end,limit,value);

	return ret;
}

int SSDBLinks::split(string& str, string& pattern, vector<string>* ret)
{
	string::size_type pos;
	size_t size;
	
	str += pattern;
	size = str.size();

	for(size_t i = 0; i < size; i ++)
	{
		pos = str.find(pattern,i);
		if(pos < size)
		{
			string s = str.substr(i, pos-i);
			ret->push_back(s);
			i = pos + pattern.size() - 1;
		}
	}
	return 0;
}

#define ELEMENT_NUM 12
int SSDBLinks::genMsg(string& str, vector<string>& sub_str, XT_TALK* msg)
{
	stringstream fid,did,type,ver,time,flag,fsize,fcolor,fstyle;
	uint32 len;

	split(str,symbol,&sub_str);

	if(sub_str.size() == ELEMENT_NUM)
	{
		fid<<sub_str[FID];
		did<<sub_str[DID];
		type<<sub_str[TYPE];
		ver<<sub_str[VER];
		time<<sub_str[TIME];
		flag<<sub_str[FLAG];
		fsize<<sub_str[FSIZE];
		fcolor<<sub_str[FCOLOR];
		fstyle<<sub_str[FSTYLE];

		fid>>msg->from_id;
		did>>msg->to_id;
		type>>msg->data_type;
		ver>>msg->ver;
		time>>msg->send_time;
		flag>>msg->recv_flag;
		fsize>>msg->fontSize;
		fcolor>>msg->fontColor;
		fstyle>>msg->fontStyle;

		len = sub_str[FNAME].length();
		if(len >= 50)
			len = 49;
		strncpy(msg->fontName, sub_str[FNAME].c_str(), len);
		msg->fontName[len] = 0;

		len = sub_str[DATA].length();
		if(len >= 1315)
			len = 1314;
		strncpy(msg->data, sub_str[DATA].c_str(), len);
		msg->data[len] = 0;

		len = sub_str[NICK].length();
		if(len >= 32)
			len = 31;
		strncpy(msg->from_nickname, sub_str[NICK].c_str(), len);
		msg->data[len] = 0;
	}	
	else
		CLog::Log("SSDBLinks::genMsg",CLog::TYPE_IMPORTANT,"Element num error!");
	
	return RESULT_SUCCESS;
}

