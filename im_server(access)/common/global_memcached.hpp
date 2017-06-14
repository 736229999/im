/*
    libmemcached component
    used for c++ interface

    Created: Du Yong
    Date: 2015-06-09


*/

/*
#define REDIS_REPLY_STRING 1
#define REDIS_REPLY_ARRAY 2
#define REDIS_REPLY_INTEGER 3
#define REDIS_REPLY_NIL 4
#define REDIS_REPLY_STATUS 5
#define REDIS_REPLY_ERROR 6
*/
#define MAX_REDIS_LENGTH 1024


#ifndef GLOBAL_MEMCACHED
#define GLOBAL_MEMCACHED
#pragma once

#include <libmemcached-1.0/memcached.h>
#if 0
#include <libmemcached/exception.hpp>
#endif

#include <string.h>

#include <sstream>
#include <string>
#include <vector>
#include <map>

#ifdef IM_USE_REDIS
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <ctype.h>

#include "../include/hiredis-vip/hircluster.h"
//#include "../include/hiredis-vip/hiutil.h"
//#include "../include/hiredis-vip/hiarray.h"
//#include "../include/hiredis-vip/command.h"
//#include "../include/hiredis-vip/dict.h"
//#include "../include/hiredis-vip/async.h"

namespace global_memcache
{

    /**
     * This is the core memcached library (if later, other objects
     * are needed, they will be created from this class).
     */
    class global_Memcache
    {
    public:

        global_Memcache()
        {
        	struct timeval redis_tv;			
		    redis_tv.tv_sec = 10;
		    redis_tv.tv_usec = 0;
            cc = redisClusterConnectWithTimeout("10.202.249.175:20634,10.202.249.176:20634,10.202.249.177:20634",
                                     redis_tv,HIRCLUSTER_FLAG_NULL);
            if(cc == NULL || cc->err)
            {
                printf("connect error : %s\n", cc == NULL ? "NULL" : cc->errstr);
            }
        };

		~global_Memcache()
		{
			redisClusterFree(cc);		
		};
		
        global_Memcache(char *hostname, int port)
        {
        	struct timeval redis_tv;			
		    redis_tv.tv_sec = 10;
		    redis_tv.tv_usec = 0;
			cc = redisClusterConnectWithTimeout(hostname,redis_tv, HIRCLUSTER_FLAG_NULL);
			if(cc == NULL || cc->err)
			{
				printf("connect error : %s\n", cc == NULL ? "NULL" : cc->errstr);
			}

		};
		
        bool get(const std::string &key, std::vector<char> &ret_val)
        {
            size_t value_length= 0;
            redisReply *reply = NULL;
            reply = (redisReply *)redisClusterCommand(cc, "get %s", key.c_str());
            if(reply == NULL)
            {
                printf("reply is null[%s]\n", cc->errstr);
                //redisClusterFree(cc);
                return false;
            }
            printf("GET reply->type:%d key  %s  value %s\r\n", reply->type,key.c_str(),reply->str);

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
				sprintf(integer,"%lld",reply->integer);					
            	value_length = strlen(integer);					
				ret_val.reserve(value_length +1); // Always provide null
                ret_val.assign(reply->str, reply->str +value_length +1);
                ret_val.resize(value_length);
			}
			else if(reply->type == REDIS_REPLY_NIL)
			{
				freeReplyObject(reply);
            	return true;
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
      //      size_t value_length = 0;
            redisReply *reply = NULL;
            reply = (redisReply *)redisClusterCommand(cc, "get %s", key.c_str());
            if(reply == NULL)
            {
                printf("reply is null[%s]\n", cc->errstr);
                //redisClusterFree(cc);
                return false;
            }
            printf("GET reply->type:%d key  %s  value %s\r\n", reply->type,key.c_str(),reply->str);
            
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
				sprintf(integer,"%lld",reply->integer);					
          //  	value_length = strlen(integer);					
					ret_val = integer;
			}
			else if(reply->type == REDIS_REPLY_NIL)
			{
				freeReplyObject(reply);
            	return true;
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
                //redisClusterFree(cc);
                return false;
            }
            printf("GET reply->type:%d key  %s  value %s\r\n", reply->type,key,reply->str);
            
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
				sprintf(ret_val,"%lld",reply->integer);					
            	strcpy(ret_val,integer);
			}
			else if(reply->type == REDIS_REPLY_NIL)
			{
				freeReplyObject(reply);
            	return true;
			}
			else
			{
				freeReplyObject(reply);
				return false;
			}
			
            printf(" 32424 fdv GET reply->type:%d key  %s  value %s\r\n", reply->type,key,reply->str);
            freeReplyObject(reply);
            return true;
        }
        


        bool get(const char *key, string &ret_val)
        {
     //       size_t value_length= 0;
            redisReply *reply = NULL;
            reply = (redisReply *)redisClusterCommand(cc, "get %s", key);
            if(reply == NULL)
            {
                printf("reply is null[%s]\n", cc->errstr);
                //redisClusterFree(cc);
                return false;
            }
            printf("GET reply->type:%d key  %s  value %s\r\n", reply->type,key,reply->str);
            
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
				sprintf(integer,"%lld",reply->integer);					
//            	value_length = strlen(integer);					
					ret_val = integer;
			}
			else if(reply->type == REDIS_REPLY_NIL)
			{
				freeReplyObject(reply);
            	return true;
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
			//redisClusterFree(cc);
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
			//redisClusterFree(cc);
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
			//redisClusterFree(cc);
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
			//redisClusterFree(cc);
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
			//redisClusterFree(cc);
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
			//redisClusterFree(cc);
			return false;
		}
		printf("setnx %s value %s reply->type:%d integer %lld\r\n",key,value,reply->type,reply->integer);
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
			expire(key,expiration);
			return true;
		}
		return false;
	};

	bool add(const char *key,
			 const char* value) //返回true表示写入成功，且之前没有该key
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
			//redisClusterFree(cc);
			return false;
		}
		printf("expire %s expiration %ld reply->type:%d\r\n",key,expiration,reply->type);
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
			//redisClusterFree(cc);
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
			//redisClusterFree(cc);
			return false;
		}
		printf("del %s reply->type:%d\r\n",key,reply->type);
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
//        size_t value_length= 0;
        redisReply *reply = NULL;
        reply = (redisReply *)redisClusterCommand(cc, "incr %s", key.c_str());
        if(reply == NULL)
        {
            printf("reply is null[%s]\n", cc->errstr);
            //redisClusterFree(cc);
            return false;
        }
        
        printf(" INCR reply->type:%d key  %s  value %lld\r\n", reply->type,key.c_str(),reply->integer);
        if(reply->type == REDIS_REPLY_INTEGER)
        {
			*value =(uint64_t)reply->integer;			
			printf(" ---INCR reply->type:%d key  %s  value %lld *value %ld\r\n", reply->type,key.c_str(),reply->integer,*value);
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
        //size_t value_length= 0;
        redisReply *reply = NULL;
        reply = (redisReply *)redisClusterCommand(cc, "decr %s", key.c_str());
        if(reply == NULL)
        {
            printf("reply is null[%s]\n", cc->errstr);
            //redisClusterFree(cc);
            return false;
        }
        
        printf(" INCR reply->type:%d key  %s  value %lld\r\n", reply->type,key.c_str(),reply->integer);
        if(reply->type == REDIS_REPLY_INTEGER)
        {
			*value =(uint64_t)reply->integer;
			printf(" ---INCR reply->type:%d key  %s  value %lld *value %ld\r\n", reply->type,key.c_str(),reply->integer,*value);
		}
		else
		{
			freeReplyObject(reply);
			return false;
		}
        freeReplyObject(reply);
        return true;
    }


	bool lpush(const char *key, const char* value)
	{
        //size_t value_length= 0;
        redisReply *reply = NULL;
        reply = (redisReply *)redisClusterCommand(cc, "lpush %s %s", key,value);
        if(reply == NULL)
        {
            printf("reply is null[%s]\n", cc->errstr);
            //redisClusterFree(cc);
            return false;
        }        
        printf(" LPUSH reply->type:%d key  %s  value %lld\r\n", reply->type,key,reply->integer);
        if(reply->type == REDIS_REPLY_INTEGER)
        {
	        freeReplyObject(reply);
	        return true;		
		}
		else
		{
			freeReplyObject(reply);
			return false;
		}
    }

	bool lrem(const char *key, uint32 count, const char *value)
	{
		//size_t value_length= 0;
		redisReply *reply = NULL;
		reply = (redisReply *)redisClusterCommand(cc, "lrem %s %u %s", key,count,value);
		if(reply == NULL)
		{
			printf("reply is null[%s]\n", cc->errstr);
			//redisClusterFree(cc);
			return false;
		}
		
		printf("LREM reply->type:%d key  %s count %u value %s,return %lld\r\n", reply->type,key,count,value,reply->integer);
		if(reply->type == REDIS_REPLY_INTEGER)
		{
			freeReplyObject(reply);
			return true;
		}
		else
		{
			freeReplyObject(reply);
			return false;
		}

	}

	bool llen(const char *key,uint64_t *length)
	{
        //size_t value_length= 0;
        redisReply *reply = NULL;
        reply = (redisReply *)redisClusterCommand(cc, "llen %s", key);
        if(reply == NULL)
        {
            printf("reply is null[%s]\n", cc->errstr);
            //redisClusterFree(cc);
            return false;
        }
        
        printf(" LLEN reply->type:%d key  %s  length %lld\r\n", reply->type,key,reply->integer);
        if(reply->type == REDIS_REPLY_INTEGER)
        {
			*length =(uint64_t)reply->integer;
			printf(" ---LLEN reply->type:%d key  %s  value %lld *value %ld\r\n", reply->type,key,reply->integer,*length);
		}
		else
		{
			freeReplyObject(reply);
			return false;
		}
        freeReplyObject(reply);
        return true;
    }

	bool lindex(const char *key, uint32 offset, char *value)
	{
        size_t value_length= 0;
        redisReply *reply = NULL;
        reply = (redisReply *)redisClusterCommand(cc, "lindex %s %d", key,offset);
        if(reply == NULL)
        {
            printf("reply is null[%s]\n", cc->errstr);
            //redisClusterFree(cc);
            return false;
        }
        
        printf(" LINDEX reply->type:%d key  %s  value %lld\r\n", reply->type,key,reply->integer);
        if(reply->type == REDIS_REPLY_STRING)
        {
        	value_length = strlen(reply->str);
        	if (reply->str != NULL && value_length > 0 && value_length < MAX_GET_LENGTH)
    		{
				strcpy(value,reply->str);					
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
			sprintf(value,"%lld",reply->integer);					
        	strcpy(value,integer);
		}
		else if(reply->type == REDIS_REPLY_NIL)
		{
			freeReplyObject(reply);
        	return true;
		}
		else
		{
			freeReplyObject(reply);
			return false;
		}
        freeReplyObject(reply);
        return true;
    };

#if 0
	 bool hmset(const char *key,int count,char (*field)[MAX_REDIS_LENGTH] ,char (*value)[MAX_REDIS_LENGTH])
	 {	 
		 redisReply *reply = NULL;
		 switch(count)
		 {
			case 1:
				reply = (redisReply *)redisClusterCommand(cc, "hmset %s %s %s",key,field[0],value[0]);
				break;
			case 2:				
				reply = (redisReply *)redisClusterCommand(cc, "hmset %s %s %s %s %s",key,field[0],value[0],field[1],value[1]);
				break;
			case 3:				
				reply = (redisReply *)redisClusterCommand(cc, "hmset %s %s %s %s %s %s %s",
					key,field[0],value[0],field[1],value[1],field[2],value[2]);
				break;
			case 4:				
				reply = (redisReply *)redisClusterCommand(cc, "hmset %s %s %s %s %s %s %s %s %s",
					key,field[0],value[0],field[1],value[1],field[2],value[2],field[3],value[3]);
				break;
			default: break;
		 }
		 if(reply == NULL)
		 {
			 printf("reply is null[%s]\n", cc->errstr);
			 //redisClusterFree(cc);
			 return false;
		 }
		 printf("hmset %s %s %s reply->type:%d\r\n",key,field,value,reply->type);
		 freeReplyObject(reply);
		 return true;
	 };
#endif

	 bool hget(const char *key,const char *field,char *ret_val)
	{
		size_t value_length= 0;
		redisReply *reply = NULL;
		reply = (redisReply *)redisClusterCommand(cc, "hget %s %s",key,field);
		if(reply == NULL)
		{
			printf("reply is null[%s]\n", cc->errstr);
			//redisClusterFree(cc);
			return false;
		}
		printf("HGET reply->type:%d key	%s	value %s\r\n", reply->type,key,reply->str);
		
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
			sprintf(ret_val,"%lld",reply->integer); 				
			strcpy(ret_val,integer);
		}
		else if(reply->type == REDIS_REPLY_NIL)
		{
			freeReplyObject(reply);
			return true;
		}
		else
		{
			freeReplyObject(reply);
			return false;
		}
		freeReplyObject(reply);
		return true;
	};
	
	 bool hmget(const char *key,int count, char (*field)[MAX_REDIS_LENGTH],char (*ret_val)[MAX_REDIS_LENGTH])
	 {
		 size_t value_length= 0;
		 redisReply *reply = NULL;
		 switch(count)
		 {
			case 1:				
				reply = (redisReply *)redisClusterCommand(cc, "hmget %s %s",key,field[0]);
				break;
			case 2:
				reply = (redisReply *)redisClusterCommand(cc, "hmget %s %s %s",key,field[0],field[1]);
				break;	
			case 3:
				reply = (redisReply *)redisClusterCommand(cc, "hmget %s %s %s %s"
					,key,field[0],field[1],field[2]);
				break;
			default:
				break;
		 }
		 if(reply == NULL)
		 {
			 printf("reply is null[%s]\n", cc->errstr);
			 //redisClusterFree(cc);
			 return false;
		 }
		 printf("HMGET reply->type:%d key  %s  value %s\r\n", reply->type,key,reply->str);
		 if (reply->type == REDIS_REPLY_ARRAY) 
		 {
			 size_t redis_len = 0;
			 for (size_t j = 0; j < reply->elements; j++) 
			 {	
			 	if(reply->element[j]->type != REDIS_REPLY_NIL )
				{
					 redis_len = strlen(reply->element[j]->str);
					 printf("key %s field[%ld]: %s value %s len %lu\n", key,j,field[j],reply->element[j]->str,redis_len);
					 
					 if(redis_len > 0 && redis_len < MAX_GET_LENGTH)
					 {
						 strcpy(ret_val[j],reply->element[j]->str);
					 }
			 	}
			 }
		 }
		 else if(reply->type == REDIS_REPLY_STRING)
		 {
			 value_length = strlen(reply->str);
			 if (reply->str != NULL && value_length > 0 && value_length < MAX_GET_LENGTH)
			 {
				 strcpy(ret_val[0],reply->str);				 
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
			 sprintf(ret_val[0],"%lld",reply->integer);				 
			 strcpy(ret_val[0],integer);
		 }
		 else if(reply->type == REDIS_REPLY_NIL)
		 {
			 freeReplyObject(reply);
			 return true;
		 }
		 else
		 {
			 freeReplyObject(reply);
			 return false;
		 }
		 freeReplyObject(reply);
		 return true;
	 };
	
	 bool hdel(char *key_field)
	 {	 
		 redisReply *reply = NULL;
		 reply = (redisReply *)redisClusterCommand(cc, "hdel %s", key_field);
		 if(reply == NULL)
		 {
			 printf("reply is null[%s]\n", cc->errstr);
			 //redisClusterFree(cc);
			 return false;
		 }
		 printf("hdel %s reply->type:%d\r\n",key_field,reply->type);
		 freeReplyObject(reply);
		 return true;
	 };

	bool redisClusterCommand_set(const char *format, ...)
	{		
		va_list ap;
		redisReply *reply = NULL;
		
		va_start(ap,format);
		reply = (redisReply *)redisClustervCommand(cc, format, ap);
		va_end(ap);
		if(reply == NULL)
		 {
			 printf("reply is null[%s]\n", cc->errstr);
			 //redisClusterFree(cc);
			 return false;
		 }		 
		 freeReplyObject(reply);
		 return true;
	}

	bool redisClusterCommand_get(char (*ret_val)[MAX_REDIS_LENGTH],const char *format, ...)
	{		
		size_t value_length= 0;
		va_list ap;
		redisReply *reply = NULL;
		
		va_start(ap,format);
		reply =(redisReply *) redisClustervCommand(cc, format, ap);
		va_end(ap);
		if(reply == NULL)
		 {
			 printf("reply is null[%s]\n", cc->errstr);
			 //redisClusterFree(cc);
			 return false;
		 }
		 printf("redisClusterCommand_get reply->type:%d\r\n", reply->type);
		 if (reply->type == REDIS_REPLY_ARRAY) 
		 {
			 size_t redis_len = 0;
			 for (size_t j = 0; j < reply->elements; j++) 
			 {	
			 	if(reply->element[j]->type != REDIS_REPLY_NIL )
				{
					 redis_len = strlen(reply->element[j]->str);
					 printf("type %d value %s len %ld\n",reply->type,reply->element[j]->str,redis_len);
					 
					 if(redis_len > 0 && redis_len < MAX_GET_LENGTH)
					 {
						 strcpy(ret_val[j],reply->element[j]->str);
					 }
			 	}
			 }
		 }
		 else if(reply->type == REDIS_REPLY_STRING)
		 {
			 value_length = strlen(reply->str);
			 if (reply->str != NULL && value_length > 0 && value_length < MAX_GET_LENGTH)
			 {
				 strcpy(ret_val[0],reply->str);				 
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
			 sprintf(ret_val[0],"%lld",reply->integer);				 
			 strcpy(ret_val[0],integer);
		 }
		 else if(reply->type == REDIS_REPLY_NIL)
		 {
			 freeReplyObject(reply);
			 return true;
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

#else


namespace global_memcache
{

    /**
     * This is the core memcached library (if later, other objects
     * are needed, they will be created from this class).
     */
    class global_Memcache
    {
    public:

        global_Memcache()
        {
            memc_= memcached(NULL, 0);
        }

        global_Memcache(const std::string &config)
        {
            memc_= memcached(config.c_str(), config.size());
        }

        global_Memcache(const std::string &hostname, in_port_t port)
        {
            memc_= memcached(NULL, 0);
            if (memc_)
            {
                memcached_server_add(memc_, hostname.c_str(), port);
            }
        }

        global_Memcache(memcached_st *clone)
        {
            memc_= memcached_clone(NULL, clone);
        }

        global_Memcache(const global_Memcache &rhs)
        {
            memc_= memcached_clone(NULL, rhs.getImpl());
        }

        global_Memcache &operator=(const global_Memcache &rhs)
        {
            if (this != &rhs)
            {
                memcached_free(memc_);
                memc_= memcached_clone(NULL, rhs.getImpl());
            }

            return *this;
        }

        ~global_Memcache()
        {
            memcached_free(memc_);
        }

        /**
         * Get the internal memcached_st *
         */
        const memcached_st *getImpl() const
        {
            return memc_;
        }

        /**
         * Return an error string for the given return structure.
         *
         * @param[in] rc a memcached_return_t structure
         * @return error string corresponding to given return code in the library.
         */
        const std::string getError(memcached_return_t rc) const
        {
            /* first parameter to strerror is unused */
            return memcached_strerror(NULL, rc);
        }

        bool error(std::string& error_message) const
        {
            if (memcached_failed(memcached_last_error(memc_)))
            {
                error_message+= memcached_last_error_message(memc_);
                return true;
            }

            return false;
        }

        bool error() const
        {
            if (memcached_failed(memcached_last_error(memc_)))
            {
                return true;
            }

            return false;
        }

        bool error(memcached_return_t& arg) const
        {
            arg= memcached_last_error(memc_);
            return memcached_failed(arg);
        }

        bool setBehavior(memcached_behavior_t flag, uint64_t data)
        {
            return (memcached_success(memcached_behavior_set(memc_, flag, data)));
        }

        uint64_t getBehavior(memcached_behavior_t flag)
        {
            return memcached_behavior_get(memc_, flag);
        }

        /**
         * Configure the memcache object
         *
         * @param[in] in_config configuration
         * @return true on success; false otherwise
         */
        bool configure(const std::string &configuration)
        {
            memcached_st *new_memc= memcached(configuration.c_str(), configuration.size());

            if (new_memc)
            {
                memcached_free(memc_);
                memc_= new_memc;

                return true;
            }

            return false;
        }

        /**
         * Add a server to the list of memcached servers to use.
         *
         * @param[in] server_name name of the server to add
         * @param[in] port port number of server to add
         * @return true on success; false otherwise
         */
        bool addServer(const std::string &server_name, in_port_t port)
        {
            return memcached_success(memcached_server_add(memc_, server_name.c_str(), port));
        }

        /**
         * Remove a server from the list of memcached servers to use.
         *
         * @param[in] server_name name of the server to remove
         * @param[in] port port number of server to remove
         * @return true on success; false otherwise
         */
        bool removeServer(const std::string &server_name, in_port_t port)
        {
            std::string tmp_str;
            std::ostringstream strstm;
            tmp_str.append(",");
            tmp_str.append(server_name);
            tmp_str.append(":");
            strstm << port;
            tmp_str.append(strstm.str());

            //memcached_return_t rc= memcached_server_remove(server);

            return false;
        }

        /**
         * Fetches an individual value from the server. mget() must always
         * be called before using this method.
         *
         * @param[in] key key of object to fetch
         * @param[out] ret_val store returned object in this vector
         * @return a memcached return structure
         */
        memcached_return_t fetch(std::string &key,
                                 std::vector<char> &ret_val,
                                 uint32_t &flags,
                                 uint64_t &cas_value)
        {
            memcached_return_t rc;

            memcached_result_st *result;
            if ((result= memcached_fetch_result(memc_, NULL, &rc)))
            {
                // Key
                key.assign(memcached_result_key_value(result), memcached_result_key_length(result));

                // Actual value, null terminated
                ret_val.reserve(memcached_result_length(result) +1);
                ret_val.assign(memcached_result_value(result),
                               memcached_result_value(result) +memcached_result_length(result) +1);
                ret_val.resize(memcached_result_length(result));

                // Misc
                flags= memcached_result_flags(result);
                cas_value= memcached_result_cas(result);
            }
            memcached_result_free(result);

            return rc;
        }

        memcached_return_t fetch(std::string &key,
                                 std::vector<char> &ret_val)
        {
            uint32_t flags= 0;
            uint64_t cas_value= 0;

            return fetch(key, ret_val, flags, cas_value);
        }

        /**
         * Fetches an individual value from the server. mget() must always
         * be called before using this method.
         *
         * @param[in] key key of object to fetch
         * @param[out] ret_val store returned object in this vector
         * @return a memcached return structure
         */

        bool fetch(std::string &key,
                   std::string &ret_val,
                   uint32_t &flags,
                   uint64_t &cas_value)
        {
            memcached_return_t rc;

            memcached_result_st *result;
            if ((result= memcached_fetch_result(memc_, NULL, &rc)))
            {

                // Key
                key.assign(memcached_result_key_value(result), memcached_result_key_length(result));

                // Actual value, null terminated
                ret_val.assign(memcached_result_value(result),memcached_result_length(result));
                // Misc
                flags= memcached_result_flags(result);
                cas_value= memcached_result_cas(result);
                //cout << "we have fetched : " << key  <<" : " << ret_val << "   return " << rc << endl;
            }
            memcached_result_free(result);

            return (rc==MEMCACHED_SUCCESS);
        }

        bool fetch(std::string &key,
                   std::string &ret_val)
        {
            uint32_t flags= 0;
            uint64_t cas_value= 0;

            return fetch(key, ret_val, flags, cas_value);
        }


        /**
         * Fetches an individual value from the server.
         *
         * @param[in] key key of object whose value to get
         * @param[out] ret_val object that is retrieved is stored in
         *                     this vector
         * @return true on success; false otherwise
         */
        bool get(const std::string &key, std::vector<char> &ret_val)
        {
            uint32_t flags= 0;
            memcached_return_t rc;
            size_t value_length= 0;

            char *value= memcached_get(memc_, key.c_str(), key.length(),
                                       &value_length, &flags, &rc);
            if (value != NULL && ret_val.empty())
            {
                ret_val.reserve(value_length +1); // Always provide null
                ret_val.assign(value, value +value_length +1);
                ret_val.resize(value_length);
                free(value);

                return true;
            }

            return false;
        }

        bool get(const std::string &key, string &ret_val)
        {
            uint32_t flags= 0;
            memcached_return_t rc;
            size_t value_length= 0;

            char *value= memcached_get((memcached_st *)memc_, key.c_str(), key.length(),
                                       &value_length, &flags, &rc);
            if (value != NULL)
            {
                ret_val = value;
                free(value);

                return true;
            }

            return false;
        }

        bool get(const char *key, char *ret_val)
        {
            uint32_t flags= 0;
            memcached_return_t rc;
            size_t value_length= 0;

            char *value= memcached_get((memcached_st *)memc_, key, strlen(key),
                                       &value_length, &flags, &rc);
            if (value != NULL && ret_val != NULL)
            {
                if(value_length < MAX_GET_LENGTH)
                {
                    strcpy(ret_val,value);
                    free(value);
                    return true;
                }
                free(value);
                return false;
            }
            return false;
        }


        bool get(const char *key, string &ret_val)
        {
            uint32_t flags= 0;
            memcached_return_t rc;
            size_t value_length= 0;

            char *value= memcached_get((memcached_st *)memc_, key,strlen(key),
                                       &value_length, &flags, &rc);
            if (value != NULL && ret_val.empty())
            {
                ret_val = value;
                free(value);

                return true;
            }

            return false;
        }



        /**
         * Fetches an individual from a server which is specified by
         * the master_key parameter that is used for determining which
         * server an object was stored in if key partitioning was
         * used for storage.
         *
         * @param[in] master_key key that specifies server object is stored on
         * @param[in] key key of object whose value to get
         * @param[out] ret_val object that is retrieved is stored in
         *                     this vector
         * @return true on success; false otherwise
         */
        bool getByKey(const std::string &master_key,
                      const std::string &key,
                      std::vector<char> &ret_val)
        {
            uint32_t flags= 0;
            memcached_return_t rc;
            size_t value_length= 0;

            char *value= memcached_get_by_key(memc_,
                                              master_key.c_str(), master_key.length(),
                                              key.c_str(), key.length(),
                                              &value_length, &flags, &rc);
            if (value)
            {
                ret_val.reserve(value_length +1); // Always provide null
                ret_val.assign(value, value +value_length +1);
                ret_val.resize(value_length);
                free(value);

                return true;
            }
            return false;
        }

        /**
         * Selects multiple keys at once. This method always
         * works asynchronously.
         *
         * @param[in] keys vector of keys to select
         * @return true if all keys are found
         */
        bool mget(const std::vector<std::string>& keys)
        {
            std::vector<const char *> real_keys;
            std::vector<size_t> key_len;
            /*
             * Construct an array which will contain the length
             * of each of the strings in the input vector. Also, to
             * interface with the memcached C API, we need to convert
             * the vector of std::string's to a vector of char *.
             */
            real_keys.reserve(keys.size());
            key_len.reserve(keys.size());

            std::vector<std::string>::const_iterator it= keys.begin();

            while (it != keys.end())
            {
                real_keys.push_back(const_cast<char *>((*it).c_str()));
                key_len.push_back((*it).length());
                ++it;
            }

            /*
             * If the std::vector of keys is empty then we cannot
             * call memcached_mget as we will get undefined behavior.
             */
            if (not real_keys.empty())
            {
                return memcached_success(memcached_mget(memc_, &real_keys[0], &key_len[0], real_keys.size()));
            }

            return false;
        }



        bool mget(const std::vector<int>& keys)
        {
            std::vector<const char *> real_keys;
            std::vector<size_t> key_len;
            /*
             * Construct an array which will contain the length
             * of each of the strings in the input vector. Also, to
             * interface with the memcached C API, we need to convert
             * the vector of std::string's to a vector of char *.
             */
            real_keys.reserve(keys.size());
            key_len.reserve(keys.size());

            vector<int>::const_iterator it= keys.begin();

            while (it != keys.end())
            {
                char *tmp_char = new char[1024];
                sprintf(tmp_char,"%d",(*it));
                real_keys.push_back(tmp_char);
                key_len.push_back(strlen(tmp_char));
                ++it;
            }

            /*
             * If the std::vector of keys is empty then we cannot
             * call memcached_mget as we will get undefined behavior.
             */
            if (not real_keys.empty())
            {
                return memcached_success(memcached_mget(memc_, &real_keys[0], &key_len[0], real_keys.size()));
            }

            std::vector<const char *>::const_iterator it2= real_keys.begin();
            while (it2 != real_keys.end())
            {
                //cout << "turn into    " << *it2 << endl;
                delete (*it2);
                ++it2;
            }
            return false;
        }

        /**
         * Writes an object to the server. If the object already exists, it will
         * overwrite the existing object. This method always returns true
         * when using non-blocking mode unless a network error occurs.
         *
         * @param[in] key key of object to write to server
         * @param[in] value value of object to write to server
         * @param[in] expiration time to keep the object stored in the server for
         * @param[in] flags flags to store with the object
         * @return true on succcess; false otherwise
         */
        bool set(const std::string &key,
                 const std::vector<char> &value,
                 time_t expiration,
                 uint32_t flags)
        {
            memcached_return_t rc= memcached_set(memc_,
                                                 key.c_str(), key.length(),
                                                 &value[0], value.size(),
                                                 0, 0);
            return memcached_success(rc);
        }

        bool set(const std::string &key,
                 const char* value, const size_t value_length,
                 time_t expiration,
                 uint32_t flags)
        {
            memcached_return_t rc= memcached_set(memc_,
                                                 key.c_str(), key.length(),
                                                 value, value_length,
                                                 0, 0);
            return memcached_success(rc);
        }



        bool set(const char *key,
                 const char* value)
        {
            memcached_return_t rc= memcached_set((memcached_st *)memc_,
                                                 key, strlen(key),
                                                 value, strlen(value),
                                                 0, 0);
            return memcached_success(rc);
        };


        bool set(const std::string &key,
                 const char* value)
        {
            memcached_return_t rc= memcached_set((memcached_st *)memc_,
                                                 key.c_str(), key.length(),
                                                 value, strlen(value),
                                                 0, 0);
            return memcached_success(rc);
        };

        bool set(const std::string &key,
                 const std::string &value)
        {
            memcached_return_t rc= memcached_set((memcached_st *)memc_,
                                                 key.c_str(), key.length(),
                                                 value.c_str(), value.length(),
                                                 0, 0);
            return memcached_success(rc);
        };

        /**
         * Writes an object to a server specified by the master_key parameter.
         * If the object already exists, it will overwrite the existing object.
         *
         * @param[in] master_key key that specifies server to write to
         * @param[in] key key of object to write to server
         * @param[in] value value of object to write to server
         * @param[in] expiration time to keep the object stored in the server for
         * @param[in] flags flags to store with the object
         * @return true on succcess; false otherwise
         */
        bool setByKey(const std::string& master_key,
                      const std::string& key,
                      const std::vector<char> &value,
                      time_t expiration,
                      uint32_t flags)
        {
            return memcached_success(memcached_set_by_key(memc_, master_key.c_str(),
                                     master_key.length(),
                                     key.c_str(), key.length(),
                                     &value[0], value.size(),
                                     expiration,
                                     flags));
        }

        /**
         * Writes a list of objects to the server. Objects are specified by
         * 2 vectors - 1 vector of keys and 1 vector of values.
         *
         * @param[in] keys vector of keys of objects to write to server
         * @param[in] values vector of values of objects to write to server
         * @param[in] expiration time to keep the objects stored in server for
         * @param[in] flags flags to store with the objects
         * @return true on success; false otherwise
         */
        bool setAll(const std::vector<std::string>& keys,
                    const std::vector< std::vector<char> *>& values,
                    time_t expiration,
                    uint32_t flags)
        {
            bool retval= true;
            std::vector<std::string>::const_iterator key_it= keys.begin();
            std::vector< std::vector<char> *>::const_iterator val_it= values.begin();
            while (key_it != keys.end())
            {
                retval= set((*key_it), *(*val_it), expiration, flags);
                if (retval == false)
                {
                    return retval;
                }
                ++key_it;
                ++val_it;
            }
            return retval;
        }

        /**
         * Writes a list of objects to the server. Objects are specified by
         * a map of keys to values.
         *
         * @param[in] key_value_map map of keys and values to store in server
         * @param[in] expiration time to keep the objects stored in server for
         * @param[in] flags flags to store with the objects
         * @return true on success; false otherwise
         */
        bool setAll(const std::map<const std::string, std::vector<char> >& key_value_map,
                    time_t expiration,
                    uint32_t flags)
        {
            bool retval= true;
            std::map<const std::string, std::vector<char> >::const_iterator it= key_value_map.begin();

            while (it != key_value_map.end())
            {
                retval= set(it->first, it->second, expiration, flags);
                if (retval == false)
                {
                    // We should tell the user what the key that failed was
                    return false;
                }
                ++it;
            }

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
            return memcached_success(memcached_increment(memc_, key.c_str(), key.length(), offset, value));
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
            return memcached_success(memcached_decrement(memc_, key.c_str(),
                                     key.length(),
                                     offset, value));
        }


        /**
         * Add an object with the specified key and value to the server. This
         * function returns false if the object already exists on the server.
         *
         * @param[in] key key of object to add
         * @param[in] value of object to add
         * @return true on success; false otherwise
         */
        bool add(const std::string& key, const std::vector<char>& value)
        {
            return memcached_success(memcached_add(memc_, key.c_str(), key.length(),
                                                   &value[0], value.size(), 0, 0));
        }
/* added  */
		bool add(char * key, char* value,time_t expiration)
        {
            return memcached_success(memcached_add(memc_, key, strlen(key),
                                                   value, strlen(value), expiration, 0));
        }

        bool add(const std::string& key, const std::vector<char>& value,time_t expiration)
        {
            return memcached_success(memcached_add(memc_, key.c_str(), key.length(),
                                                   &value[0], value.size(), expiration, 0));
        }

        /**
         * Add an object with the specified key and value to the server. This
         * function returns false if the object already exists on the server. The
         * server to add the object to is specified by the master_key parameter.
         *
         * @param[in[ master_key key of server to add object to
         * @param[in] key key of object to add
         * @param[in] value of object to add
         * @return true on success; false otherwise
         */
        bool addByKey(const std::string& master_key,
                      const std::string& key,
                      const std::vector<char>& value)
        {
            return memcached_success(memcached_add_by_key(memc_,
                                     master_key.c_str(),
                                     master_key.length(),
                                     key.c_str(),
                                     key.length(),
                                     &value[0],
                                     value.size(),
                                     0, 0));
        }

        /**
         * Replaces an object on the server. This method only succeeds
         * if the object is already present on the server.
         *
         * @param[in] key key of object to replace
         * @param[in[ value value to replace object with
         * @return true on success; false otherwise
         */
        bool replace(const std::string& key, const std::vector<char>& value)
        {
            return memcached_success(memcached_replace(memc_, key.c_str(), key.length(),
                                     &value[0], value.size(),
                                     0, 0));
        }

        /**
         * Replaces an object on the server. This method only succeeds
         * if the object is already present on the server. The server
         * to replace the object on is specified by the master_key param.
         *
         * @param[in] master_key key of server to replace object on
         * @param[in] key key of object to replace
         * @param[in[ value value to replace object with
         * @return true on success; false otherwise
         */
        bool replaceByKey(const std::string& master_key,
                          const std::string& key,
                          const std::vector<char>& value)
        {
            return memcached_success(memcached_replace_by_key(memc_,
                                     master_key.c_str(),
                                     master_key.length(),
                                     key.c_str(),
                                     key.length(),
                                     &value[0],
                                     value.size(),
                                     0, 0));
        }

        /**
         * Places a segment of data before the last piece of data stored.
         *
         * @param[in] key key of object whose value we will prepend data to
         * @param[in] value data to prepend to object's value
         * @return true on success; false otherwise
         */
        bool prepend(const std::string& key, const std::vector<char>& value)
        {
            return memcached_success(memcached_prepend(memc_, key.c_str(), key.length(),
                                     &value[0], value.size(), 0, 0));
        }

        /**
         * Places a segment of data before the last piece of data stored. The
         * server on which the object where we will be prepending data is stored
         * on is specified by the master_key parameter.
         *
         * @param[in] master_key key of server where object is stored
         * @param[in] key key of object whose value we will prepend data to
         * @param[in] value data to prepend to object's value
         * @return true on success; false otherwise
         */
        bool prependByKey(const std::string& master_key,
                          const std::string& key,
                          const std::vector<char>& value)
        {
            return memcached_success(memcached_prepend_by_key(memc_,
                                     master_key.c_str(),
                                     master_key.length(),
                                     key.c_str(),
                                     key.length(),
                                     &value[0],
                                     value.size(),
                                     0,
                                     0));
        }

        /**
         * Places a segment of data at the end of the last piece of data stored.
         *
         * @param[in] key key of object whose value we will append data to
         * @param[in] value data to append to object's value
         * @return true on success; false otherwise
         */
        bool append(const std::string& key, const std::vector<char>& value)
        {
            return memcached_success(memcached_append(memc_,
                                     key.c_str(),
                                     key.length(),
                                     &value[0],
                                     value.size(),
                                     0, 0));
        }

        /**
         * Places a segment of data at the end of the last piece of data stored. The
         * server on which the object where we will be appending data is stored
         * on is specified by the master_key parameter.
         *
         * @param[in] master_key key of server where object is stored
         * @param[in] key key of object whose value we will append data to
         * @param[in] value data to append to object's value
         * @return true on success; false otherwise
         */
        bool appendByKey(const std::string& master_key,
                         const std::string& key,
                         const std::vector<char> &value)
        {
            return memcached_success(memcached_append_by_key(memc_,
                                     master_key.c_str(),
                                     master_key.length(),
                                     key.c_str(),
                                     key.length(),
                                     &value[0],
                                     value.size(),
                                     0, 0));
        }

        /**
         * Overwrite data in the server as long as the cas_arg value
         * is still the same in the server.
         *
         * @param[in] key key of object in server
         * @param[in] value value to store for object in server
         * @param[in] cas_arg "cas" value
         */
        bool cas(const std::string& key,
                 const std::vector<char>& value,
                 uint64_t cas_arg)
        {
            return memcached_success(memcached_cas(memc_, key.c_str(), key.length(),
                                                   &value[0], value.size(),
                                                   0, 0, cas_arg));
        }

        /**
         * Overwrite data in the server as long as the cas_arg value
         * is still the same in the server. The server to use is
         * specified by the master_key parameter.
         *
         * @param[in] master_key specifies server to operate on
         * @param[in] key key of object in server
         * @param[in] value value to store for object in server
         * @param[in] cas_arg "cas" value
         */
        bool casByKey(const std::string& master_key,
                      const std::string& key,
                      const std::vector<char> &value,
                      uint64_t cas_arg)
        {
            return memcached_success(memcached_cas_by_key(memc_,
                                     master_key.c_str(),
                                     master_key.length(),
                                     key.c_str(),
                                     key.length(),
                                     &value[0],
                                     value.size(),
                                     0, 0, cas_arg));
        }

        /**
         * Delete an object from the server specified by the key given.
         *
         * @param[in] key key of object to delete
         * @return true on success; false otherwise
         */
        bool remove(const std::string& key)
        {
            return memcached_success(memcached_delete(memc_, key.c_str(), key.length(), 0));
        }
/* added */
        bool remove(char *key)
        {
            return memcached_success(memcached_delete(memc_, key, strlen(key), 0));
        }

        /**
         * Delete an object from the server specified by the key given.
         *
         * @param[in] key key of object to delete
         * @param[in] expiration time to delete the object after
         * @return true on success; false otherwise
         */
        bool remove(const std::string& key, time_t expiration)
        {
            return memcached_success(memcached_delete(memc_,
                                     key.c_str(),
                                     key.length(),
                                     expiration));
        }

        /**
         * Delete an object from the server specified by the key given.
         *
         * @param[in] master_key specifies server to remove object from
         * @param[in] key key of object to delete
         * @return true on success; false otherwise
         */
        bool removeByKey(const std::string& master_key,
                         const std::string& key)
        {
            return memcached_success(memcached_delete_by_key(memc_,
                                     master_key.c_str(),
                                     master_key.length(),
                                     key.c_str(),
                                     key.length(),
                                     0));
        }

        /**
         * Delete an object from the server specified by the key given.
         *
         * @param[in] master_key specifies server to remove object from
         * @param[in] key key of object to delete
         * @param[in] expiration time to delete the object after
         * @return true on success; false otherwise
         */
        bool removeByKey(const std::string& master_key,
                         const std::string& key,
                         time_t expiration)
        {
            return memcached_success(memcached_delete_by_key(memc_,
                                     master_key.c_str(),
                                     master_key.length(),
                                     key.c_str(),
                                     key.length(),
                                     expiration));
        }

        /**
         * Wipe the contents of memcached servers.
         *
         * @param[in] expiration time to wait until wiping contents of
         *                       memcached servers
         * @return true on success; false otherwise
         */
        bool flush(time_t expiration= 0)
        {
            return memcached_success(memcached_flush(memc_, expiration));
        }

        /**
         * Get the library version string.
         * @return std::string containing a copy of the library version string.
         */
        const std::string libVersion() const
        {
            const char *ver= memcached_lib_version();
            const std::string version(ver);
            return version;
        }

        /**
         * Retrieve memcached statistics. Populate a std::map with the retrieved
         * stats. Each server will map to another std::map of the key:value stats.
         *
         * @param[out] stats_map a std::map to be populated with the memcached
         *                       stats
         * @return true on success; false otherwise
         */
        bool getStats(std::map< std::string, std::map<std::string, std::string> >& stats_map)
        {
            memcached_return_t rc;
            memcached_stat_st *stats= memcached_stat(memc_, NULL, &rc);

            if (rc != MEMCACHED_SUCCESS &&
                rc != MEMCACHED_SOME_ERRORS)
            {
                return false;
            }

            uint32_t server_count= memcached_server_count(memc_);

            /*
             * For each memcached server, construct a std::map for its stats and add
             * it to the std::map of overall stats.
             */
            for (uint32_t x= 0; x < server_count; x++)
            {
                const memcached_instance_st * instance= memcached_server_instance_by_position(memc_, x);
                std::ostringstream strstm;
                std::string server_name(memcached_server_name(instance));
                server_name.append(":");
                strstm << memcached_server_port(instance);
                server_name.append(strstm.str());

                std::map<std::string, std::string> server_stats;
                char **list= memcached_stat_get_keys(memc_, &stats[x], &rc);
                for (char** ptr= list; *ptr; ptr++)
                {
                    char *value= memcached_stat_get_value(memc_, &stats[x], *ptr, &rc);
                    server_stats[*ptr]= value;
                    free(value);
                }

                stats_map[server_name]= server_stats;
                free(list);
            }

            memcached_stat_free(memc_, stats);
            return true;
        }

    private:
        enum {MAX_GET_LENGTH=1024};
        memcached_st *memc_;
    };

}
#endif

#endif

