#include <stdio.h>
#include <hircluster.h>
#include <pthread.h>
#include <unistd.h>
#include <global_redis.hpp>

/*

#define REDIS_REPLY_STRING 1
#define REDIS_REPLY_ARRAY 2
#define REDIS_REPLY_INTEGER 3
#define REDIS_REPLY_NIL 4
#define REDIS_REPLY_STATUS 5
#define REDIS_REPLY_ERROR 6


*/
using namespace global_Redis;


typedef struct redis_context
{
	redisClusterContext *cc;
	int index;
}redis_context_t;

void *pthread_set(void *buffer)
{
#if 0

	redis_context_t *pbuf = buffer;
	char key_test[32];
	pbuf->cc = redisClusterConnect("10.202.249.175:20634,10.202.249.176:20634,10.202.249.177:20634",
			HIRCLUSTER_FLAG_NULL);
	if(pbuf->cc == NULL || pbuf->cc->err)
	{
		printf("connect error : %s\n", pbuf->cc == NULL ? "NULL" : pbuf->cc->errstr);
		return -1;
	}
	
	redisReply *reply = NULL;
	
	
	for(int i = pbuf->index*100 ; i < (pbuf->index+1)*100; i++)
	{
		sprintf(key_test,"key_%d",i);
		
		reply = redisClusterCommand(pbuf->cc, "set %s %s", key_test, key_test);
		if(reply == NULL)
		{
			printf("reply is null[%s]\n", pbuf->cc->errstr);
			redisClusterFree(pbuf->cc);
			return -1;
		}
		printf("set %s reply->type:%d\r\n",key_test,reply->type);
		   freeReplyObject(reply);

	}
#endif

	global_redis test1 = global_redis();
	printf("_____CLEAR KEYS__________________________________\r\n");	

	for(int i = 0 ; i < 2 ; i++)
	{
		char test[32] = {0};
		sprintf(test,"redis_test_%d",i);

		test1.remove((const char*)test);

		test1.get((const char*)test,test);		

	}
	printf("_____END CLEAR KEYS_____::: START SET AND GET_____________________________\r\n");	
	
	for(int i = 0 ; i < 2 ; i++)
	{
		char test[32] = {0};
		sprintf(test,"redis_test_%d",i);		
		test1.set((const char*)test,test);
		
		test1.get((const char*)test,test);
	}
	printf("_____END SET AND GET KEYS_____::: START EXIST SETNX GET_____________________________\r\n");	
	
	for(int i = 0 ; i < 2 ; i++)
	{
		char test[32] = {0};
		sprintf(test,"redis_test_%d",i);	
		test1.setnx((const char*)test,test);		
		test1.remove((const char*)test);
	}
	printf("_____END EXIST SETNX GET KEYS_____::: START NOT EXIST SETNX GET_____________________________\r\n");	

	for(int i = 0 ; i < 2 ; i++)
	{
		char test[32] = {0};
		sprintf(test,"redis_test_%d",i);	
		test1.setnx((const char*)test,test);	
	}
	
	printf("_____START SET EXPIRE__________________________________\r\n");	
	for(int i = 0 ; i < 2 ; i++)
	{
		char test[32] = {0};
		sprintf(test,"redis_test_%d",i);
		test1.set((const char*)test,test);
		test1.expire((const char*)test,1);
		
	}

	sleep(5);
	printf("_____START CHECK EXPIRE  -- GET __________________________________\r\n");	
	for(int i = 0 ; i < 2 ; i++)
	{
		char test[32] = {0};
		sprintf(test,"redis_test_%d",i);		
		test1.get((const char*)test,test);
		test1.setnx((const char*)test,test);
	}

	for(int i = 0 ; i < 10 ; i++)
	{
		string inc = "incrtest";
		uint64_t val;
		test1.increment(inc,1,&val);
	}

	for(int i = 0 ; i < 10 ; i++)
	{
		string inc = "incrtest";
		uint64_t val;
		test1.decrement(inc,1,&val);
	}

}


void *pthread_get(void *buffer)
{
	redis_context_t *pbuf = (redis_context_t* )buffer;
	char key_test[32];	
	redisReply *reply = NULL;	
	pbuf->cc = redisClusterConnect("10.202.249.175:20634,10.202.249.176:20634,10.202.249.177:20634",
			HIRCLUSTER_FLAG_NULL);
	if(pbuf->cc == NULL || pbuf->cc->err)
	{
		printf("connect error : %s\n", pbuf->cc == NULL ? "NULL" : pbuf->cc->errstr);
		//return -1;
	}
	
	for(int i = pbuf->index*100; i < (pbuf->index+1)*100; i++)
	{
		sprintf(key_test,"key_%d",i);
		
		reply = (redisReply *)redisClusterCommand(pbuf->cc, "get %s", key_test);
		if(reply == NULL)
		{
			printf("reply is null[%s]\n", pbuf->cc->errstr);
			redisClusterFree(pbuf->cc);
			//return -1;
		}
		printf(" GET reply->type:%d key  %s  value %s\r\n", reply->type,key_test,reply->str);
		freeReplyObject(reply);

	}

}

int main()
{
	#if 0
	redisClusterContext *cc = redisClusterConnect("10.202.249.175:20634,10.202.249.176:20634,10.202.249.177:20634",
			HIRCLUSTER_FLAG_NULL);
	if(cc == NULL || cc->err)
	{
		printf("connect error : %s\n", cc == NULL ? "NULL" : cc->errstr);
		return -1;
	}
	#endif
		redis_context_t info[200];
		

		pthread_t ptr_set[100];
		pthread_t ptr_get[100];	
		
		for(int i = 0 ; i < 1 ; i++)
		{
			//info[i].cc = cc;
			info[i].index = i;
			pthread_create(&ptr_set[i],NULL,pthread_set,(void *)&info[i]);			
		}
		
		printf("#####################################\r\n");
		sleep(10);		

		printf("#####################################\r\n");
#if 0
		for(int i = 0 ; i < 100 ; i++)
		{			
			//info[i].cc = cc;
			info[i].index = i;
			pthread_create(&ptr_set[i],NULL,pthread_get,(void *)&info[i]);			
		}
		
		printf("#####################################\r\n");
#endif
		while(1)
		{
			sleep(1);
		}
}

#if 0
int main()
{

/*

#define REDIS_REPLY_STRING 1
#define REDIS_REPLY_ARRAY 2
#define REDIS_REPLY_INTEGER 3
#define REDIS_REPLY_NIL 4
#define REDIS_REPLY_STATUS 5
#define REDIS_REPLY_ERROR 6


*/
    char *key="key-a";
    char *field="field-1";
    char *key1="key1";
    char *value1="value-1";
    char *key2="key2";
    char *value2="value-2";

	char key_test[32];
	char value_test[32];
    redisClusterContext *cc = redisClusterConnect("10.202.249.175:20634,10.202.249.176:20634,10.202.249.177:20634",
        HIRCLUSTER_FLAG_NULL);
    if(cc == NULL || cc->err)
    {
        printf("connect error : %s\n", cc == NULL ? "NULL" : cc->errstr);
        return -1;
    }
	redisReply *reply = NULL;


	for(int i = 0 ; i < 1000; i++)
	{
		sprintf(key_test,"key_%d",i);
		
 		reply = redisClusterCommand(cc, "set %s %s", key_test, key_test);
		if(reply == NULL)
		{
		    printf("reply is null[%s]\n", cc->errstr);
		    redisClusterFree(cc);
		    return -1;
		}

//	    printf("reply->type:%d\r\n", reply->type);
		   freeReplyObject(reply);

#if 0	
		reply = redisClusterCommand(cc, "get %s", key_test);
		if(reply == NULL)
		{
			printf("reply is null[%s]\n", cc->errstr);
			redisClusterFree(cc);
			return -1;
		}
		printf("get reply->type:%d	key  %s  value %s\r\n", reply->type,key_test,reply->str);
		freeReplyObject(reply);
#endif
	}

	printf("#####################################\r\n");

	for(int i = 0 ; i < 1000; i++)
	{
		sprintf(key_test,"key_%d",i);
		
		
		reply = redisClusterCommand(cc, "get %s",key_test);
		if(reply == NULL)
		{
			printf("reply is null[%s]\n", cc->errstr);
			redisClusterFree(cc);
			return -1;
		}
//		printf(" GET reply->type:%d	key  %s  value %s\r\n", reply->type,key_test,reply->str);
		freeReplyObject(reply);

	}

	printf("#####################################\r\n");

	for(int i = 0 ; i < 10; i++)
	{
		sprintf(key_test,"key_%d",i);
		
		
		reply = redisClusterCommand(cc, "del %s",key_test);
		if(reply == NULL)
		{
			printf("reply is null[%s]\n", cc->errstr);
			redisClusterFree(cc);
			return -1;
		}
		freeReplyObject(reply);

	}
	printf("#####################################\r\n");

	for(int i = 0 ; i < 10; i++)
	{
		sprintf(key_test,"key_%d",i);
		
		
		reply = redisClusterCommand(cc, "get %s",key_test);
		if(reply == NULL)
		{
			printf("reply is null[%s]\n", cc->errstr);
			redisClusterFree(cc);
			return -1;
		}
		printf(" GET reply->type:%d	key  %s  value %s\r\n", reply->type,key_test,reply->str);
		freeReplyObject(reply);

	}

	printf("#####################################\r\n");

	for(int i = 0 ; i < 10; i++)
	{
		
		
		reply = redisClusterCommand(cc, "incr incr_test");
		if(reply == NULL)
		{
			printf("reply is null[%s]\n", cc->errstr);
			redisClusterFree(cc);
			return -1;
		}
		freeReplyObject(reply);

	}

	reply = redisClusterCommand(cc, "get incr_test");
	if(reply == NULL)
	{
		printf("reply is null[%s]\n", cc->errstr);
		redisClusterFree(cc);
		return -1;
	}
	
	printf(" GET reply->type:%d key  incr_test  value %s\r\n", reply->type,reply->str);
	freeReplyObject(reply);

		
    reply = redisClusterCommand(cc, "hmget %s %s", key, field);
    if(reply == NULL)
    {
        printf("reply is null[%s]\n", cc->errstr);
        redisClusterFree(cc);
        return -1;
    }

    printf("reply->type:%d\r\n", reply->type);

    freeReplyObject(reply);
#if 1
    reply = redisClusterCommand(cc, "mset %s %s %s %s", key1, value1, key2, value2);
    if(reply == NULL)
    {
        printf("reply is null[%s]\n", cc->errstr);
        redisClusterFree(cc);
        return -1;
    }

    printf("reply->str:%s\r\n", reply->str);


    freeReplyObject(reply);
#endif
    redisClusterFree(cc);
    return 0;
}
#endif

