#pragma once
#include <string>
#include <vector>

#include <sstream>
#include <string.h>

#include <map>


using namespace std;
/*

ui
#define REDIS_REPLY_STRING 1
#define REDIS_REPLY_ARRAY 2
#define REDIS_REPLY_INTEGER 3
#define REDIS_REPLY_NIL 4
#define REDIS_REPLY_STATUS 5
#define REDIS_REPLY_ERROR 6


*/

namespace global_Redis
{
    class global_redis
    {
    public:

        global_redis()
        {
            cc = redisClusterConnect("10.202.249.175:20634,10.202.249.176:20634,10.202.249.177:20634",
                                     HIRCLUSTER_FLAG_NULL);
            if(cc == NULL || cc->err)
            {
                printf("connect error : %s\n", cc == NULL ? "NULL" : cc->errstr);
            }
        };

		~global_redis()
		{
			redisClusterFree(cc);		
		};
		
        bool get(const std::string &key, std::vector<char> &ret_val)
        {
            size_t value_length= 0;
            redisReply *reply = NULL;
            reply = (redisReply *)redisClusterCommand(cc, "get %s", key.c_str());
            if(reply == NULL)
            {
                printf("reply is null[%s]\n", cc->errstr);
                redisClusterFree(cc);
                return false;
            }
            printf(" GET reply->type:%d key  %s  value %s\r\n", reply->type,key.c_str(),reply->str);

            if(reply->type == REDIS_REPLY_STRING)
            {
            	if (reply->str != NULL && (value_length = strlen(reply->str)) > 0)
        		{
                    ret_val.reserve(value_length +1); // Always provide null
                    ret_val.assign(reply->str, reply->str +value_length +1);
                    ret_val.resize(value_length);
            	}
				else
				{
					freeReplyObject(reply);
            		return false;
				}
            }
            else if(reply->type == REDIS_REPLY_INTEGER)
            {
            	char integer[64] = {0};
				sprintf(integer,"%lu",reply->integer);					
            	value_length = strlen(integer);					
				ret_val.reserve(value_length +1); // Always provide null
                ret_val.assign(reply->str, reply->str +value_length +1);
                ret_val.resize(value_length);
			}
			else
			{
				freeReplyObject(reply);
				return false;
			}
            freeReplyObject(reply);
            return true;
            
            
        }

        bool get(const std::string &key, string &ret_val)
        {
            size_t value_length= 0;
            redisReply *reply = NULL;
            reply = (redisReply *)redisClusterCommand(cc, "get %s", key.c_str());
            if(reply == NULL)
            {
                printf("reply is null[%s]\n", cc->errstr);
                redisClusterFree(cc);
                return false;
            }
            printf(" GET reply->type:%d key  %s  value %s\r\n", reply->type,key.c_str(),reply->str);
            
            if(reply->type == REDIS_REPLY_STRING)
            {                	
				if (reply->str != NULL && strlen(reply->str) > 0)
				{
					ret_val = reply->str;
				}
				else
				{
					freeReplyObject(reply);
					return false;
				}
            }
            else if(reply->type == REDIS_REPLY_INTEGER)
            {
            	char integer[64] = {0};
				sprintf(integer,"%lu",reply->integer);					
            	value_length = strlen(integer);					
					ret_val = integer;
			}
			else
			{
				freeReplyObject(reply);
				return false;
			}
            freeReplyObject(reply);
            return true;
        }

        bool get(const char *key, char *ret_val)
        {
            size_t value_length= 0;
            redisReply *reply = NULL;
            reply = (redisReply *)redisClusterCommand(cc, "get %s", key);
            if(reply == NULL)
            {
                printf("reply is null[%s]\n", cc->errstr);
                redisClusterFree(cc);
                return false;
            }
            printf(" GET reply->type:%d key  %s  value %s\r\n", reply->type,key,reply->str);
            
            if(reply->type == REDIS_REPLY_STRING)
            {
            	value_length = strlen(reply->str);
            	if (reply->str != NULL && value_length > 0 && value_length < MAX_GET_LENGTH)
        		{
					strcpy(ret_val,reply->str);
            	}
				else
				{
					freeReplyObject(reply);
            		return false;
				}
            }
            else if(reply->type == REDIS_REPLY_INTEGER)
            {
            	char integer[64] = {0};
				sprintf(ret_val,"%lu",reply->integer);					
            	strcpy(ret_val,integer);
			}
			else
			{
				freeReplyObject(reply);
				return false;
			}
            freeReplyObject(reply);
            return true;
        }
        


        bool get(const char *key, string &ret_val)
        {
            size_t value_length= 0;
            redisReply *reply = NULL;
            reply = (redisReply *)redisClusterCommand(cc, "get %s", key);
            if(reply == NULL)
            {
                printf("reply is null[%s]\n", cc->errstr);
                redisClusterFree(cc);
                return false;
            }
            printf(" GET reply->type:%d key  %s  value %s\r\n", reply->type,key,reply->str);
            
            if(reply->type == REDIS_REPLY_STRING)
            {                	
				if (reply->str != NULL && strlen(reply->str) > 0)
				{
					ret_val = reply->str;
				}
				else
				{
					freeReplyObject(reply);
					return false;
				}
            }
            else if(reply->type == REDIS_REPLY_INTEGER)
            {
            	char integer[64] = {0};
				sprintf(integer,"%lu",reply->integer);					
            	value_length = strlen(integer);					
					ret_val = integer;
			}
			else
			{
				freeReplyObject(reply);
				return false;
			}
            freeReplyObject(reply);
            return true;
        }

	bool set(const std::string &key,
			 const std::vector<char> &value,
			 time_t expiration,
			 uint32_t flags)
	{	
		redisReply *reply = NULL;
		reply = (redisReply *)redisClusterCommand(cc, "set %s %s", key.c_str(), &value[0]);
		if(reply == NULL)
		{
			printf("reply is null[%s]\n", cc->errstr);
			redisClusterFree(cc);
			return false;
		}
		printf("set %s reply->type:%d\r\n",key.c_str(),reply->type);
		freeReplyObject(reply);
		return true;

	}
	
	bool set(const std::string &key,
			 const char* value, const size_t value_length,
			 time_t expiration,
			 uint32_t flags)
	{	
		redisReply *reply = NULL;
		reply = (redisReply *)redisClusterCommand(cc, "set %s %s", key.c_str(),value);
		if(reply == NULL)
		{
			printf("reply is null[%s]\n", cc->errstr);
			redisClusterFree(cc);
			return false;
		}
		printf("set %s value %s reply->type:%d\r\n",key.c_str(),value,reply->type);
		freeReplyObject(reply);
		return true;

	}
	
	
	
	bool set(const char *key,
			 const char* value)
	{	
		redisReply *reply = NULL;
		reply = (redisReply *)redisClusterCommand(cc, "set %s %s", key,value);
		if(reply == NULL)
		{
			printf("reply is null[%s]\n", cc->errstr);
			redisClusterFree(cc);
			return false;
		}
		printf("set %s value %s reply->type:%d\r\n",key,value,reply->type);
		freeReplyObject(reply);
		return true;

	};
	
	
	bool set(const std::string &key,
			 const char* value)
	{	
		redisReply *reply = NULL;
		reply = (redisReply *)redisClusterCommand(cc, "set %s %s", key.c_str(),value);
		if(reply == NULL)
		{
			printf("reply is null[%s]\n", cc->errstr);
			redisClusterFree(cc);
			return false;
		}
		printf("set %s value %s reply->type:%d\r\n",key.c_str(),value,reply->type);
		freeReplyObject(reply);
		return true;

	};
	
	bool set(const std::string &key,
			 const std::string &value)
	{	
		redisReply *reply = NULL;
		reply = (redisReply *)redisClusterCommand(cc, "set %s %s", key.c_str(),value.c_str());
		if(reply == NULL)
		{
			printf("reply is null[%s]\n", cc->errstr);
			redisClusterFree(cc);
			return false;
		}
		printf("set %s value %s reply->type:%d\r\n",key.c_str(),value.c_str(),reply->type);
		freeReplyObject(reply);
		return true;

	};

	int setnx(const char *key,
			 const char* value)
	{	
		redisReply *reply = NULL;
		reply = (redisReply *)redisClusterCommand(cc, "setnx %s %s", key,value);
		if(reply == NULL)
		{
			printf("reply is null[%s]\n", cc->errstr);
			redisClusterFree(cc);
			return false;
		}
		printf("setnx %s value %s reply->type:%d integer %u\r\n",key,value,reply->type,reply->integer);
		if(reply->type == REDIS_REPLY_INTEGER)
		{
			if(reply->integer == 1)	//写成功
			{				
				freeReplyObject(reply);
				return true;
			}			
		}		
		freeReplyObject(reply);
		return false;
	};


/*adpate for the lock in memcached*/
	bool add(const char *key,
			 const char* value,time_t expiration) //返回0 表示成功
	{	
		if(setnx(key,value))
		{
			expire(key,REDIS_TIMEOUT);
			return true;
		}
		return false;
	};

	
	bool expire(const char *key,time_t expiration)
	{	
		redisReply *reply = NULL;
		reply = (redisReply *)redisClusterCommand(cc, "expire %s %u", key,expiration);
		if(reply == NULL)
		{
			printf("reply is null[%s]\n", cc->errstr);
			redisClusterFree(cc);
			return false;
		}
		printf("expire %s expiration %u reply->type:%d\r\n",key,expiration,reply->type);
		freeReplyObject(reply);
		return true;
	}

	bool remove(const std::string& key)
	{	
		redisReply *reply = NULL;
		reply = (redisReply *)redisClusterCommand(cc, "del %s", key.c_str());
		if(reply == NULL)
		{
			printf("reply is null[%s]\n", cc->errstr);
			redisClusterFree(cc);
			return false;
		}
		printf("del %s reply->type:%d\r\n",key.c_str(),reply->type);
		freeReplyObject(reply);
		return true;
	}
	/* added */
	bool remove(char *key)
	{	
		redisReply *reply = NULL;
		reply = (redisReply *)redisClusterCommand(cc, "del %s", key);
		if(reply == NULL)
		{
			printf("reply is null[%s]\n", cc->errstr);
			redisClusterFree(cc);
			return false;
		}
		printf("del %s value %s reply->type:%d\r\n",key,reply->type);
		freeReplyObject(reply);
		return true;
	}
	

	/**
	  * Increment the value of the object associated with the specified
	  * key by the offset given. The resulting value is saved in the value
	  * parameter.
	  *
	  * @param[in] key key of object in server whose value to increment
	  * @param[in] offset amount to increment object's value by
	  * @param[out] value store the result of the increment here
	  * @return true on success; false otherwise
	  */

	bool increment(const std::string& key, uint32_t offset, uint64_t *value)
    {
        size_t value_length= 0;
        redisReply *reply = NULL;
        reply = (redisReply *)redisClusterCommand(cc, "incr %s", key.c_str());
        if(reply == NULL)
        {
            printf("reply is null[%s]\n", cc->errstr);
            redisClusterFree(cc);
            return false;
        }
        
        printf(" INCR reply->type:%d key  %s  value %lu\r\n", reply->type,key.c_str(),reply->integer);
        if(reply->type == REDIS_REPLY_INTEGER)
        {
			*value =(uint64_t)reply->integer;			
			printf(" ---INCR reply->type:%d key  %s  value %lu *value %lu\r\n", reply->type,key.c_str(),reply->integer,*value);
		}
		else
		{
			freeReplyObject(reply);
			return false;
		}
        freeReplyObject(reply);
        return true;
    }
	
	/**
	 * Decrement the value of the object associated with the specified
	 * key by the offset given. The resulting value is saved in the value
	 * parameter.
	 *
	 * @param[in] key key of object in server whose value to decrement
	 * @param[in] offset amount to increment object's value by
	 * @param[out] value store the result of the decrement here
	 * @return true on success; false otherwise
	 */
	bool decrement(const std::string& key, uint32_t offset, uint64_t *value)
	{
        size_t value_length= 0;
        redisReply *reply = NULL;
        reply = (redisReply *)redisClusterCommand(cc, "decr %s", key.c_str());
        if(reply == NULL)
        {
            printf("reply is null[%s]\n", cc->errstr);
            redisClusterFree(cc);
            return false;
        }
        
        printf(" INCR reply->type:%d key  %s  value %lu\r\n", reply->type,key.c_str(),reply->integer);
        if(reply->type == REDIS_REPLY_INTEGER)
        {
			*value =(uint64_t)reply->integer;
			printf(" ---INCR reply->type:%d key  %s  value %lu *value %lu\r\n", reply->type,key.c_str(),reply->integer,*value);
		}
		else
		{
			freeReplyObject(reply);
			return false;
		}
        freeReplyObject(reply);
        return true;
    }

    private:		
        enum {REDIS_TIMEOUT=3,MAX_GET_LENGTH=1024};
        redisClusterContext *cc;
    };
}
