/*
 * MysqlConnPool.h
 *
 *  Created on: Dec 5, 2014
 *      Author: yaoxing
 */

#ifndef MYSQLCONNPOOL_H_
#define MYSQLCONNPOOL_H_

#include <mutex>
#include <condition_variable>
#include <vector>
#include <functional>
#include <memory>
#include <chrono>

namespace mysqlpp {
class Connection;
}

using MysqlConn = std::unique_ptr<mysqlpp::Connection>;
using ConnPool = std::vector<MysqlConn>;
using SqlInitCallback = std::function<mysqlpp::Connection*()>;
using SqlExecCallback = std::function<void(mysqlpp::Connection*)>;

class MysqlConnPool
{
public:

	MysqlConnPool();
	virtual ~MysqlConnPool();

	//��ʼ�����ݿ�
	void init(uint32_t cnt, const SqlInitCallback& func);

	//ִ��SQL����
	void exec(const SqlExecCallback& func);

private:
	//�����ӳ��л�ȡһ������
	MysqlConn getConn();

	//��һ�����ӷŻ����ӳ�
	void releaseConn(MysqlConn&&);

private:
	std::mutex 				pool_mutex_;
	std::condition_variable	pool_condition_;
	ConnPool				conn_pool_;

	static const std::chrono::duration<int> timeout;
};

#endif /* MYSQLCONNPOOL_H_ */
