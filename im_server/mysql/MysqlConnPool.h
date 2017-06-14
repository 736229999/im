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

	//初始化数据库
	void init(uint32_t cnt, const SqlInitCallback& func);

	//执行SQL操作
	void exec(const SqlExecCallback& func);

private:
	//从连接池中获取一个连接
	MysqlConn getConn();

	//将一个连接放回连接池
	void releaseConn(MysqlConn&&);

private:
	std::mutex 				pool_mutex_;
	std::condition_variable	pool_condition_;
	ConnPool				conn_pool_;

	static const std::chrono::duration<int> timeout;
};

#endif /* MYSQLCONNPOOL_H_ */
