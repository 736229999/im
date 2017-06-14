/*
 * MysqlMini.cpp
 *
 *  Created on: Dec 9, 2014
 *      Author: yaoxing
 */

#include "stdafx.h"
#include <functional>
#include <mysql++/mysql++.h>
#include <mysql++/ssqls.h>
#include <vector>
#include <iconv.h>
#include "Log.h"
#include "ErrorCode.h"
#include "ClientProtocol.h"
#include "MysqlLib.h"
#include <sstream>

std::once_flag MysqlLib::s_instance_flag;
MysqlLib* MysqlLib::s_mysql_lib = nullptr;

using namespace mysqlpp;

//查找密码
sql_create_4(user2password,
		1, 0,
		sql_int, 			id,
		sql_varchar,		password,
		sql_int,		merchant_id,
		sql_int,			state)

//获取用户信息
sql_create_22(user_info,
		1, 0,
		sql_int, 			id,
		Null<sql_varchar>,	uuid,
		sql_varchar,		username,
		sql_varchar,		password,
		sql_int,			type,
		sql_int,			state,
		Null<sql_varchar>,	email,
		Null<sql_varchar>,	mobile,
		Null<sql_varchar>,	truename,
		Null<sql_int>,		merchant_id,
		Null<sql_varchar>,	merchant_code,
		Null<sql_int>,		higherup_id,
		Null<sql_int>,		role_id,
		Null<sql_int>,		department_id,
		Null<sql_varchar>,	office_address,
		Null<sql_varchar>,	job_number,
		Null<sql_timestamp>,entry_time,
		Null<sql_varchar>, 	id_no,
		sql_timestamp,		create_time,
		Null<sql_timestamp>,update_time,
		Null<sql_varchar>,	job,
		Null<sql_varchar>,	random_key)

//获取子账号
sql_create_1(sub_user,
		1, 0,
		sql_int,			id)

//判断是否有子账号
sql_create_1(has_sub_account,
		1, 0,
		sql_int,			higherup_id)
		
//获取子账号的商家账号
sql_create_2(sub_and_merchant_user,
		1, 0,
		sql_int,			id,
		sql_int,			merchant_id)
		

MysqlLib::MysqlLib()
{

}

MysqlLib::~MysqlLib()
{

}

MysqlLib* MysqlLib::getInstance()
{
	//创建mysql连接单件
	std::call_once(s_instance_flag, []() {assert(s_mysql_lib == nullptr); s_mysql_lib = new MysqlLib; });

	return s_mysql_lib;
}

bool MysqlLib::connect(const char* db, const char* server, const char* user,
		const char* password, uint32_t port, uint32_t count)
{
	user2password::table("mall_m_member");
	user_info::table("mall_m_member");
	sub_user::table("mall_m_member");
	sub_and_merchant_user::table("mall_m_member");

	try {
		//连接mysql
		mysql_conn_pool_.reset(new MysqlConnPool);
		mysql_conn_pool_->init(count,
				std::bind(&MysqlLib::initMysqlConn, this, db, server, user, password, port));

		return true;

	} catch(const mysqlpp::BadOption& e) {	//option设置错误
		CLog::Log("MysqlLib", CLog::TYPE_ERROR, "set option:%s", e.what());
		return false;
	} catch(const mysqlpp::Exception& e) {	//mysql连接错误
		CLog::Log("MysqlLib", CLog::TYPE_ERROR, "connect to mysql:%s", e.what());
		return false;
	}
}

mysqlpp::Connection* MysqlLib::initMysqlConn(const char* db, const char* server, const char* user,
		const char* password, uint32_t port)
{
	mysqlpp::Connection* conn = new mysqlpp::Connection;

	//设置字符集
	mysqlpp::SetCharsetNameOption *charOP = new mysqlpp::SetCharsetNameOption("utf8");
	conn->set_option(charOP);

	//设置自动重连
	mysqlpp::ReconnectOption *reOP = new mysqlpp::ReconnectOption(true);
	conn->set_option(reOP);
	conn->connect(db, server, user, password, port);
	return conn;
}

uint32_t MysqlLib::getPwdByName(const char* name, uint32_t& userid, char* pwd, uint32_t& merchant_id, uint32_t& state)
{
	std::vector<user2password> res;

	try {
//		my_init();
		//通过用户名获取账号id和密码
		std::string exec("select id, password, merchant_id, state from mall_m_member where username='");
		exec += name;
		exec += "'";
		mysql_conn_pool_->exec([&] (mysqlpp::Connection* conn) {
			Query query = conn->query(exec);
			query.storein(res);
		});

		if (res.empty()) {
			return DB_NOUSER;
		}

		strncpy(pwd, res[0].password.c_str(), res[0].password.length());
		userid = res[0].id;
		merchant_id = res[0].merchant_id;
		state = res[0].state;

		#ifdef DEBUG
		CLog::Log("MysqlLib", CLog::TYPE_IMPORTANT, "get %d %s's password: %s", userid, name, res[0].password.c_str());
		#endif

	} 	catch (const mysqlpp::BadQuery& er) {	//查询不到数据
		CLog::Log("MysqlLib", CLog::TYPE_ERROR, er.what());
		return DB_ERROR;
	} catch (const mysqlpp::BadConversion& er) {	//数据类型错误
		CLog::Log("MysqlLib", CLog::TYPE_ERROR, "Conversion error: %s\tretrieved data size: %d"
				", actual size: %d", er.what(), er.retrieved, er.actual_size);
		return DB_ERROR;
	} catch (const mysqlpp::Exception& er) {	//其他错误
		CLog::Log("MysqlLib", CLog::TYPE_ERROR, "get password Error: %s", er.what());
		return DB_ERROR;
	}

	return DB_SUCCESS;
}

uint32_t MysqlLib::getUserInfo(uint32_t iUserID, XT_MERCHANT_INFO_GET_ACK &lpAck)
{
	std::vector<user_info> res;

	try {
		//通过用户id查询用户信息
		mysql_conn_pool_->exec([&] (mysqlpp::Connection* conn) {
			Query query = conn->query();
			query << "select * from mall_m_member where id=";
			query << iUserID;
			query.storein(res);
		});

		if (res.empty()) {
			CLog::Log("MysqlLib", CLog::TYPE_IMPORTANT, "no user id %d", lpAck.id);
			return DB_NOUSER;
		}

		user_info &ui = res[0];


//		char inStr[128];
//		char outStr[128];
//		char **in = &inStr;
//		char **out = &outStr;
//		iconv_t  conv;
//		conv = iconv_open("GBK", "UTF-8");
//
//		strncpy(inStr, ui.truename.data.c_str(), ui.truename.data.size());
//		uint64_t inBytes = ui.truename.data.size();
//		uint64_t outBytes = 128;
//		iconv(conv, in, &inBytes, out, &outBytes);
//		lpAck.truename = outStr;
//		iconv_close(conv);

		lpAck.create_time = ui.create_time.str();
		lpAck.department_id = ui.department_id.data;
		lpAck.email = ui.email.data;
		lpAck.entry_time = ui.email.data;
		lpAck.higherup_id = ui.higherup_id.data;
		lpAck.id = ui.id;
		lpAck.id_no = ui.id_no.data;
		lpAck.job = ui.job.data;
		lpAck.job_number = ui.job_number.data;
		lpAck.merchant_code = ui.merchant_code.data;
		lpAck.merchant_id = ui.merchant_id.data;
		lpAck.mobile = ui.mobile.data;
		lpAck.office_address = ui.mobile.data;
		lpAck.password = ui.password;
		lpAck.random_key = ui.random_key.data;
		lpAck.role_id = ui.role_id.data;
		lpAck.state = ui.state;
		lpAck.truename = ui.truename.data;
		lpAck.type = ui.type;
		lpAck.update_time = ui.update_time.data.str();
		lpAck.username = ui.username;
		lpAck.uuid = ui.uuid.data;


		CLog::Log("MysqlLib", CLog::TYPE_IMPORTANT, "get %s' info username:%s, truename:%s, email:%d",
				lpAck.username.c_str(), lpAck.truename.c_str(), lpAck.email.c_str(), lpAck.id);
	

	} 	catch (const mysqlpp::BadQuery& er) {	//查询不到数据
		CLog::Log("MysqlLib", CLog::TYPE_ERROR, er.what());
		return DB_ERROR;
	} catch (const mysqlpp::BadConversion& er) {	//数据类型错误
		CLog::Log("MysqlLib", CLog::TYPE_ERROR, "Conversion error: %s\tretrieved data size: %d"
				", actual size: %d", er.what(), er.retrieved, er.actual_size);
		return DB_ERROR;
	} catch (const mysqlpp::Exception& er) {	//其他错误
		CLog::Log("MysqlLib", CLog::TYPE_ERROR, "get password Error: %s", er.what());
		return DB_ERROR;
	}

	return DB_SUCCESS;
}

uint32_t MysqlLib::getAllSubAccount(uint32_t p_id, uint32_t c_id, MyGroupAccount& GroupAccount)
{
	SingleAccount Account;
	GroupAccount.clear();

	try {
		//通过商家id查询所有其子账户的id、用户名、密码、真名
		StoreQueryResult res;
		mysql_conn_pool_->exec([&] (mysqlpp::Connection* conn) {
		Query query = conn->query();
			//query << "select id,username,password,truename from mall_m_member where state=1 and id<>%0:c_id and merchant_id=%1:p_id";
			query << "select id,username,password,truename from mall_m_member where state=1 and merchant_id=%1:p_id";
			query.parse();
			res = query.store(c_id,p_id);
		});
		//cout<<"The result is:"<<res.size()<<endl;
		StoreQueryResult::const_iterator iter;
		for(iter = res.begin(); iter != res.end(); ++iter)
		{

			Row row = *iter;
			Account.id = row[0];
			Account.username = row[1].c_str();
			Account.password = row[2].c_str();
			Account.truename = row[3].c_str();
			//cout<<Account.id<<";"<<Account.username<<";"<<Account.password<<";"<<Account.truename<<endl;
			GroupAccount.push_back(Account);
		}

		
		CLog::Log("MysqlLib", CLog::TYPE_DEBUG, "get all %d's sub account, size: %d", p_id, GroupAccount.size());
		

	} 	catch (const mysqlpp::BadQuery& er) {	//查询不到数据
		CLog::Log("MysqlLib", CLog::TYPE_ERROR, er.what());
		return DB_ERROR;
	} catch (const mysqlpp::BadConversion& er) {	//数据类型错误
		CLog::Log("MysqlLib", CLog::TYPE_ERROR, "Conversion error: %s\tretrieved data size: %d"
				", actual size: %d", er.what(), er.retrieved, er.actual_size);
		return DB_ERROR;
	} catch (const mysqlpp::Exception& er) {	//其他错误
		CLog::Log("MysqlLib", CLog::TYPE_ERROR, "get password Error: %s", er.what());
		return DB_ERROR;
	}

	return DB_SUCCESS;
}

uint32_t MysqlLib::getAllSubAccount(uint32_t parent_id, AccountArray& account_array)
{
	//获取当前时间
	time_t timeNow;
	time(&timeNow);
	bool isTimeOut = false;

	std::map<uint32_t, AccoutArrayWithTime>::iterator it = mapSubAccount.find(parent_id);
	if(it != mapSubAccount.end())
	{
		AccoutArrayWithTime accountArrayWithTime = it->second;
		//CLog::Log(__FUNCTION__, CLog::TYPE_IMPORTANT, "timeNow:%d, readTime:%d, timeDif:%d",
			//timeNow, accountArrayWithTime.readTime, timeNow - accountArrayWithTime.readTime);

		//判断子帐号内存是否过期
		if(timeNow < accountArrayWithTime.readTime + 30 * 60)
		{
			account_array = accountArrayWithTime.accountArray;
		
			stringstream streamLog;
			streamLog << "memory, " << parent_id << ":[";
			for(size_t j = 0; j<account_array.size(); j++)
			{
				streamLog <<" "<< account_array[j];
			}		
			streamLog << "]";
			CLog::Log(__FUNCTION__, CLog::TYPE_IMPORTANT, "%s", streamLog.str().c_str());
			return DB_SUCCESS;
		}
		else
		{
			isTimeOut = true;

		}

	}

	std::vector<sub_user> res;
	account_array.clear();

	try {
		//通过商家id查询所有子账号id
		mysql_conn_pool_->exec([&] (mysqlpp::Connection* conn) {
			Query query = conn->query();
			query << "select id from mall_m_member where state=1 and merchant_id=";
			query << parent_id;
			query.storein(res);
		});

		for (auto& i: res) {
			account_array.push_back(i.id);
		}

		stringstream streamLog;
		streamLog << "mysql, " << parent_id << ":[";
		for(size_t j = 0; j<account_array.size(); j++)
		{
			streamLog << " " << account_array[j];
		}		
		streamLog << "]";
		CLog::Log(__FUNCTION__,CLog::TYPE_IMPORTANT, "%s", streamLog.str().c_str());

		//更新过期时间
		AccoutArrayWithTime accountArrayWithTime;
		accountArrayWithTime.accountArray = account_array;
		accountArrayWithTime.readTime = timeNow;
		if(isTimeOut)
		{
			CLog::Log(__FUNCTION__, CLog::TYPE_IMPORTANT, "merchantId:%d, oldReadTime:%d, newReadTime:%d, timeDif:%d",
				parent_id, mapSubAccount[parent_id].readTime, timeNow, timeNow - mapSubAccount[parent_id].readTime);
			mapSubAccount[parent_id] = accountArrayWithTime;
		}
		else
		{
			mapSubAccount.insert(map<uint32_t, AccoutArrayWithTime>::value_type(parent_id, accountArrayWithTime));
		}


	} 	
	catch (const mysqlpp::BadQuery& er) {
		CLog::Log("MysqlLib", CLog::TYPE_ERROR, er.what());
		return DB_ERROR;
	}
	catch (const mysqlpp::BadConversion& er) {
		CLog::Log("MysqlLib", CLog::TYPE_ERROR, "Conversion error: %s\tretrieved data size: %d, actual size: %d", er.what(), er.retrieved, er.actual_size);
		return DB_ERROR;
	}
	catch (const mysqlpp::Exception& er){
		CLog::Log("MysqlLib", CLog::TYPE_ERROR, "get password Error: %s", er.what());
		return DB_ERROR;
	}

	return DB_SUCCESS;
}

//获取商家主账号
uint32_t MysqlLib::getMainAccount(uint32_t parent_id, uint32_t &main_id)
{
	//获取当前时间
	time_t timeNow;
	time(&timeNow);
	bool isTimeOut = false;

	std::map<uint32_t, MainAccoutWithTime>::iterator it = mapMainAccount.find(parent_id);
	if(it != mapMainAccount.end())
	{
		MainAccoutWithTime mainAccountWithTime = it->second;
		//CLog::Log(__FUNCTION__, CLog::TYPE_IMPORTANT, "timeNow:%d, readTime:%d, timeDif:%d",
			//timeNow, accountArrayWithTime.readTime, timeNow - accountArrayWithTime.readTime);

		//判断子帐号内存是否过期
		if(timeNow < mainAccountWithTime.readTime + 30 * 60)
		{
			main_id = mainAccountWithTime.mainAccount;
		
			stringstream streamLog;
			streamLog << "memory-mainAccount, " << parent_id << ":[" << main_id << "]";
			CLog::Log(__FUNCTION__, CLog::TYPE_IMPORTANT, "%s", streamLog.str().c_str());
			return DB_SUCCESS;
		}
		else
		{
			isTimeOut = true;

		}

	}

	std::vector<sub_user> res;
	main_id = 0;

	try {
		//通过商家id查询所有子账号id
		mysql_conn_pool_->exec([&] (mysqlpp::Connection* conn) {
			Query query = conn->query();
			query << "select id from mall_m_member where state=1 and type=1 and merchant_id=";
			query << parent_id;
			query.storein(res);
		});
		if(res.size() == 0)
		{			
			CLog::Log(__FUNCTION__,CLog::TYPE_IMPORTANT, "没有商家 %u 的主账号",parent_id);
			main_id = 0;
		}
		else
		{
			main_id = res[0].id;
		}
		stringstream streamLog;
		streamLog << "mysql-mainAccount, " << parent_id << ":[" << main_id << "]";

		CLog::Log(__FUNCTION__,CLog::TYPE_IMPORTANT, "%s", streamLog.str().c_str());

		MainAccoutWithTime mainAccountWithTime;
		if(isTimeOut)
		{
			CLog::Log(__FUNCTION__, CLog::TYPE_IMPORTANT, "merchantId:%d, oldReadTime:%d, newReadTime:%d, timeDif:%d",
				parent_id, mapMainAccount[parent_id].readTime, timeNow, timeNow - mapMainAccount[parent_id].readTime);
			mainAccountWithTime.mainAccount = main_id;
			mainAccountWithTime.readTime = timeNow;
			mapMainAccount[parent_id] = mainAccountWithTime;
			
		}
		else
		{
			//更新过期时间
			mainAccountWithTime.mainAccount= main_id;
			mainAccountWithTime.readTime = timeNow;
			mapMainAccount.insert(map<uint32_t, MainAccoutWithTime>::value_type(parent_id, mainAccountWithTime));
		}


	} 	
	catch (const mysqlpp::BadQuery& er) {
		CLog::Log("MysqlLib", CLog::TYPE_ERROR, er.what());
		return DB_ERROR;
	}
	catch (const mysqlpp::BadConversion& er) {
		CLog::Log("MysqlLib", CLog::TYPE_ERROR, "Conversion error: %s\tretrieved data size: %d, actual size: %d", er.what(), er.retrieved, er.actual_size);
		return DB_ERROR;
	}
	catch (const mysqlpp::Exception& er){
		CLog::Log("MysqlLib", CLog::TYPE_ERROR, "get password Error: %s", er.what());
		return DB_ERROR;
	}

	return DB_SUCCESS;
}

//获取商家自己账号
uint32_t MysqlLib::getMainMerchant(uint32_t subaccount, uint32_t &self_id)
{
	//获取当前时间
	time_t timeNow;
	time(&timeNow);
	bool isTimeOut = false;

	std::map<uint32_t, MainAccoutWithTime>::iterator it = mapMainMerchant.find(subaccount);
	if(it != mapMainMerchant.end())
	{
		MainAccoutWithTime mainAccountWithTime = it->second;
		//CLog::Log(__FUNCTION__, CLog::TYPE_IMPORTANT, "timeNow:%d, readTime:%d, timeDif:%d",
			//timeNow, accountArrayWithTime.readTime, timeNow - accountArrayWithTime.readTime);

		//判断子帐号内存是否过期
		if(timeNow < mainAccountWithTime.readTime + 30 * 60)
		{
			self_id = mainAccountWithTime.mainAccount;
		
			stringstream streamLog;
			streamLog << "memory-mainAccount, " << self_id << ":[" << subaccount << "]";
			CLog::Log(__FUNCTION__, CLog::TYPE_IMPORTANT, "%s", streamLog.str().c_str());
			return DB_SUCCESS;
		}
		else
		{
			isTimeOut = true;

		}

	}

	std::vector<sub_and_merchant_user> res;
	self_id = 0;

	try {
		//通过商家id查询所有子账号id
		mysql_conn_pool_->exec([&] (mysqlpp::Connection* conn) {
			Query query = conn->query();
			query << "select id,merchant_id from mall_m_member where id=";
			query << subaccount;
			query.storein(res);
		});

		if(res.size() == 0)
		{			
			CLog::Log(__FUNCTION__,CLog::TYPE_IMPORTANT, "没有商家 %u 的账号",subaccount);
			self_id = 0;
		}
		else
		{
			self_id = res[0].merchant_id;
		}
		stringstream streamLog;
		streamLog << "mysql-mainAccount, " << self_id << ":[" << subaccount << "]";

		CLog::Log(__FUNCTION__,CLog::TYPE_IMPORTANT, "%s", streamLog.str().c_str());

		MainAccoutWithTime mainAccountWithTime;
		if(isTimeOut)
		{
			CLog::Log(__FUNCTION__, CLog::TYPE_IMPORTANT, "merchantId:%d, oldReadTime:%d, newReadTime:%d, timeDif:%d",
				subaccount, mapMainMerchant[subaccount].readTime, timeNow, timeNow - mapMainMerchant[subaccount].readTime);
			mainAccountWithTime.mainAccount = self_id;
			mainAccountWithTime.readTime = timeNow;
			mapMainMerchant[subaccount] = mainAccountWithTime;
			
		}
		else
		{
			//更新过期时间
			mainAccountWithTime.mainAccount= self_id;
			mainAccountWithTime.readTime = timeNow;
			mapMainMerchant.insert(map<uint32_t, MainAccoutWithTime>::value_type(subaccount, mainAccountWithTime));
		}


	} 	
	catch (const mysqlpp::BadQuery& er) {
		CLog::Log("MysqlLib", CLog::TYPE_ERROR, er.what());
		return DB_ERROR;
	}
	catch (const mysqlpp::BadConversion& er) {
		CLog::Log("MysqlLib", CLog::TYPE_ERROR, "Conversion error: %s\tretrieved data size: %d, actual size: %d", er.what(), er.retrieved, er.actual_size);
		return DB_ERROR;
	}
	catch (const mysqlpp::Exception& er){
		CLog::Log("MysqlLib", CLog::TYPE_ERROR, "get password Error: %s", er.what());
		return DB_ERROR;
	}

	return DB_SUCCESS;
}


uint32_t MysqlLib::getMainMerchants(vector<uint32_t> subaccount, vector<MerchantandSubAccount> &self_id)
{
	//获取当前时间
	time_t timeNow;
	time(&timeNow);
	bool isTimeOut = false;
	vector<uint32_t> needSubAccount;
	needSubAccount.clear();
	MerchantandSubAccount userEach;


	for(uint32 i = 0 ; i < subaccount.size(); i++)
	{	
		std::map<uint32_t, MainAccoutWithTime>::iterator it = mapMainMerchant.find(subaccount[i]);
		if(it != mapMainMerchant.end())
		{
			MainAccoutWithTime mainAccountWithTime = it->second;
			//CLog::Log(__FUNCTION__, CLog::TYPE_IMPORTANT, "timeNow:%d, readTime:%d, timeDif:%d",
				//timeNow, accountArrayWithTime.readTime, timeNow - accountArrayWithTime.readTime);

			//判断子帐号内存是否过期
			if(timeNow < mainAccountWithTime.readTime + 30 * 60)
			{
				stringstream streamLog;
				streamLog << "memory-mainAccount, " <<  mainAccountWithTime.mainAccount << ":[" << subaccount[i] << "]";
				CLog::Log(__FUNCTION__, CLog::TYPE_IMPORTANT, "%s", streamLog.str().c_str());
				//return DB_SUCCESS;
				userEach.merchantId = mainAccountWithTime.mainAccount;
				userEach.subAccountId = subaccount[i];
				self_id.push_back(userEach);
				
			}
			else
			{
				isTimeOut = true;
				needSubAccount.push_back(subaccount[i]);
			}

		}
	
	}
	std::vector<sub_and_merchant_user> res;

	try {

			if(needSubAccount.size() > 0)
			{
				//通过商家id查询所有子账号id
				mysql_conn_pool_->exec([&] (mysqlpp::Connection* conn) {
					Query query = conn->query();
					query << "select id,merchant_id from mall_m_member where id in (";
					for(uint32 i = 0 ; i < needSubAccount.size() - 1; i++)
					{
						query << needSubAccount[i];
						query << ",";
					}
					
					query << needSubAccount[needSubAccount.size() - 1];
					query << ")";

					query.storein(res);
				});
			}
		
		if(res.size() == 0)
		{			
			CLog::Log(__FUNCTION__,CLog::TYPE_IMPORTANT, "没有从mysql查询到商家的账号");
		}
		else
		{
			for(uint32 i = 0 ; i < res.size() ; i++ )
			{
				userEach.merchantId = res[i].merchant_id;
				userEach.subAccountId = res[i].id;
				self_id.push_back(userEach);

				stringstream streamLog;
				streamLog << "mysql-mainAccount, " << userEach.subAccountId << ":[" << userEach.merchantId << "]";

				CLog::Log(__FUNCTION__,CLog::TYPE_IMPORTANT, "%s", streamLog.str().c_str());
			}
			
		}
		
		for(uint32 i = 0 ; i < res.size() ; i++ )
		{
			MainAccoutWithTime mainAccountWithTime;
			if(isTimeOut)
			{
				CLog::Log(__FUNCTION__, CLog::TYPE_IMPORTANT, "merchantId:%d, oldReadTime:%d, newReadTime:%d, timeDif:%d",
					subaccount[i], mapMainMerchant[subaccount[i]].readTime, timeNow, timeNow - mapMainMerchant[subaccount[i]].readTime);
				mainAccountWithTime.mainAccount = res[i].merchant_id;
				mainAccountWithTime.readTime = timeNow;
				mapMainMerchant[res[i].id] = mainAccountWithTime;
				
			}
			else
			{
				//更新过期时间
				mainAccountWithTime.mainAccount= res[i].merchant_id;
				mainAccountWithTime.readTime = timeNow;
				mapMainMerchant.insert(map<uint32_t, MainAccoutWithTime>::value_type(res[i].id, mainAccountWithTime));
			}
		}
	} 	
	catch (const mysqlpp::BadQuery& er) {
		CLog::Log("MysqlLib", CLog::TYPE_ERROR, er.what());
		return DB_ERROR;
	}
	catch (const mysqlpp::BadConversion& er) {
		CLog::Log("MysqlLib", CLog::TYPE_ERROR, "Conversion error: %s\tretrieved data size: %d, actual size: %d", er.what(), er.retrieved, er.actual_size);
		return DB_ERROR;
	}
	catch (const mysqlpp::Exception& er){
		CLog::Log("MysqlLib", CLog::TYPE_ERROR, "get password Error: %s", er.what());
		return DB_ERROR;
	}

	return DB_SUCCESS;
}


uint32_t MysqlLib::getBestSubAccount(uint32_t parent_id, uint32_t& sub_id)
{
	std::vector<sub_user> res;

	try {
		//通过商家id查询所有子账号id
		mysql_conn_pool_->exec([&] (mysqlpp::Connection* conn) {
			Query query = conn->query();
			query << "select id from mall_m_member where type=2 and state=1 and merchant_id=";
			query << parent_id;
			query.storein(res);
		});

		if (res.empty()) {
			//没有子账号
			sub_id = parent_id;
		} else {
			//子账号排序
			std::set<uint32_t> id_set;
			id_set.insert(parent_id);

			for (auto& i: res) {
				id_set.insert(i.id);
			}

			//以上次的结果为起点获取下个子账号
			std::unique_lock<std::mutex> lock(account_load_mutex_);
			auto ifind = account_load_map_.find(parent_id);

			if (ifind == account_load_map_.end()) {
				sub_id = *id_set.begin();
			} else {
				auto i = id_set.find(ifind->second);

				if (i == id_set.end() || ++i == id_set.end()) {
					sub_id = *id_set.begin();
				} else {
					sub_id = *i;
				}
			}
		}

		account_load_map_[parent_id] = sub_id;

		
		CLog::Log("MysqlLib", CLog::TYPE_DEBUG, "get %d's sub account: %d", parent_id, sub_id);
	

	} 	catch (const mysqlpp::BadQuery& er) {
		CLog::Log("MysqlLib", CLog::TYPE_ERROR, er.what());
		return DB_ERROR;
	} catch (const mysqlpp::BadConversion& er) {
		CLog::Log("MysqlLib", CLog::TYPE_ERROR, "Conversion error: %s\tretrieved data size: %d"
				", actual size: %d", er.what(), er.retrieved, er.actual_size);
		return DB_ERROR;
	} catch (const mysqlpp::Exception& er) {
		CLog::Log("MysqlLib", CLog::TYPE_ERROR, "get password Error: %s", er.what());
		return DB_ERROR;
	}

	return DB_SUCCESS;
}

uint32_t MysqlLib::hasSubAccount(const AccountArray& account_array, HasSubAccountArray& has_sub_account_array)
{
	assert(account_array.empty());
	std::vector<has_sub_account> res;
	has_sub_account_array.clear();

	try {
		//查询账号数组中的所有子账号，并按父账号分组排列
		mysql_conn_pool_->exec([&] (mysqlpp::Connection* conn) {
			string sql = "SELECT higherup_id FROM mall_m_member WHERE higherup_id in (";
			for (auto id: account_array) {
				sql += std::to_string(id) + ",";
			}
			sql.erase(sql.length() - 1);
			sql += ") GROUP BY higherup_id;";
			Query query = conn->query(sql);
			query.storein(res);
		});

		//判断账号数组中的每项是否获取到了子账号
		for (auto& i: account_array) {
			auto ifind = std::find_if(res.begin(), res.end(), [i] (const has_sub_account& a) ->bool {
							return (a.higherup_id == (int)i);
						});
			if (ifind != res.end()) {
				has_sub_account_array.push_back(HasSubAccount(i, true));
			} else {
				has_sub_account_array.push_back(HasSubAccount(i, false));
			}
		}

		#ifdef DEBUG
		CLog::Log("MysqlLib", CLog::TYPE_IMPORTANT,
				"check sub account, size: %d", account_array.size());
		#endif

	} 	catch (const mysqlpp::BadQuery& er) {
		CLog::Log("MysqlLib", CLog::TYPE_ERROR, er.what());
		return DB_ERROR;
	} catch (const mysqlpp::BadConversion& er) {
		CLog::Log("MysqlLib", CLog::TYPE_ERROR, "Conversion error: %s\tretrieved data size: %d"
				", actual size: %d", er.what(), er.retrieved, er.actual_size);
		return DB_ERROR;
	} catch (const mysqlpp::Exception& er) {
		CLog::Log("MysqlLib", CLog::TYPE_ERROR, "get password Error: %s", er.what());
		return DB_ERROR;
	}

	return DB_SUCCESS;
}

