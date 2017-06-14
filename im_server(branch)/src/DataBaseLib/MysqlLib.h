/*
 * MysqlMini.h
 *
 *  Created on: Dec 9, 2014
 *      Author: yaoxing
 */

#ifndef MYSQLMINI_H_
#define MYSQLMINI_H_

#include "MysqlConnPool.h"
#include <memory>
#include <mutex>
#include <map>

#define MYSQL_LIB MysqlLib::getInstance()

namespace mysqlpp {
class Connection;
}

class XT_SERVER_USERINFO_GET_ACK;

using AccountArray = std::vector<uint32_t>;

//´øÊ±¼ä±êÇ©µÄ×ÓÕÊºÅ
class AccoutArrayWithTime
{
public:
	AccountArray accountArray;
	time_t readTime;
};

class MainAccoutWithTime
{
public:
	uint32_t mainAccount;
	time_t readTime;
};


class SingleAccount
{
	public:
		SingleAccount(){};
		~SingleAccount(){};

		uint32 id;
		std::string username;
		std::string password;
		std::string truename;
};

using MyGroupAccount = std::vector<SingleAccount>;

struct HasSubAccount
{
	HasSubAccount(uint32_t _id, bool _b): id(_id), b(_b) {}
	uint32_t	id = 0;
	bool		b = false;		//是否有子账号
};
using HasSubAccountArray = std::vector<HasSubAccount>;

struct MerchantandSubAccount
{
	uint32_t merchantId;
	uint32_t subAccountId;
};


class MysqlLib
{
public:
	MysqlLib();
	virtual ~MysqlLib();

	//单件实例
	static MysqlLib* getInstance();

	//连接数据库
	bool connect(const char* db = "member", const char* server = "stage-fnscdb.idc1.fn",
			const char* user = "usr_sc", const char* password = "m3E9B7er",
			uint32_t port = 0, uint32_t count = 3);

	//查询用户的密码
	uint32_t getPwdByName(const char* name, uint32_t& userid, char* pwd, uint32_t& merchant_id, uint32_t& state);

	//获取用户信息
	uint32_t getUserInfo(uint32_t iUserID, XT_MERCHANT_INFO_GET_ACK &lpAck);

	//获取全部子账号
	uint32_t getAllSubAccount(uint32_t p_id, uint32_t c_id, MyGroupAccount& GroupAccount);
	uint32_t getAllSubAccount(uint32_t parent_id, AccountArray& account_array);

	//获取最小负载的子账号
	uint32_t getBestSubAccount(uint32_t parent_id, uint32_t& sub_id);

	//判断是否有子账号
	uint32_t hasSubAccount(const AccountArray& account_array, HasSubAccountArray& has_sub_account_array);
	
	uint32_t getMainAccount(uint32_t parent_id, uint32_t &main_id);
	
	uint32_t getMainMerchant(uint32_t subaccount, uint32_t &self_id);

	uint32_t getMainMerchants(vector<uint32_t> subaccount, vector<MerchantandSubAccount> &self_id);

private:
	//初始化mysql连接的回调函数
	mysqlpp::Connection* initMysqlConn(const char* db, const char* server, const char* user,
			const char* password, uint32_t port);

private:
	static std::once_flag s_instance_flag;
	static MysqlLib* s_mysql_lib;

private:
	std::unique_ptr<MysqlConnPool> mysql_conn_pool_;

	using LastAccountMap = std::map<uint32_t, uint32_t>;
	LastAccountMap account_load_map_;
	std::mutex account_load_mutex_;

	std::map<uint32_t, AccoutArrayWithTime> mapSubAccount;
	std::map<uint32_t, MainAccoutWithTime> mapMainAccount;
	std::map<uint32_t, MainAccoutWithTime> mapMainMerchant;

};

#endif /* MYSQLMINI_H_ */
