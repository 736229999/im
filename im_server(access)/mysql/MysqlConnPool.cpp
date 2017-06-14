/*
 * MysqlConnPool.cpp
 *
 *  Created on: Dec 5, 2014
 *      Author: yaoxing
 */

#include <mysql++/mysql++.h>
#include <cassert>
#include "MysqlConnPool.h"
#include <cstdio>

using namespace std;
using std::unique_lock;
using std::mutex;

const std::chrono::duration<int> MysqlConnPool::timeout(5);

MysqlConnPool::MysqlConnPool()
{

}

MysqlConnPool::~MysqlConnPool()
{
	conn_pool_.clear();
}

void MysqlConnPool::init(uint32_t cnt, const SqlInitCallback& func)
{
	assert(cnt != 0);
	for (uint32_t i = 0; i < cnt; ++i) {
		conn_pool_.emplace_back(std::unique_ptr<mysqlpp::Connection>(func()));
	}
}

void MysqlConnPool::exec(const SqlExecCallback& func)
{
	MysqlConn conn = getConn();
	auto releaser = [&conn, this] (void*) {releaseConn(std::move(conn));};
	std::unique_ptr<MysqlConn, decltype(releaser)> guard(&conn, releaser);

	conn->ping();
	func(conn.get());
}

MysqlConn MysqlConnPool::getConn()
{
	unique_lock<mutex> lock(pool_mutex_);
	bool ret = pool_condition_.wait_for(lock, timeout,
			[this] ()->bool {return !conn_pool_.empty();});
	if (!ret) {
		throw mysqlpp::ConnectionFailed("can't get mysql connnection");
	}

	assert(!conn_pool_.empty());
	MysqlConn conn = std::move(conn_pool_.back());
	conn_pool_.pop_back();
	return conn;
}

void MysqlConnPool::releaseConn(MysqlConn&& conn)
{
	unique_lock<mutex> lock(pool_mutex_);
	pool_condition_.notify_one();
	conn_pool_.emplace_back(std::move(conn));
}

