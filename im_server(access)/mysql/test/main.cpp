/*
 * main.cpp
 *
 *  Created on: Dec 5, 2014
 *      Author: yaoxing
 */

#include <mysql++/mysql++.h>
#include <mysql++/ssqls.h>
#include <thread>
#include <vector>
#include "MysqlConnPool.h"
#include "QueryUser.h"

using namespace mysqlpp;
using namespace std;

int main()
{
//	using users_t = vector<mall_m_member>;
//	SetCharsetNameOption *charOP = new SetCharsetNameOption("utf8");
//	Connection conn;
//	conn.set_option(charOP);
//	conn.connect("member", "wh-mysql03.fn.com", "zhouxiang", "zhouxiang.123");
//
//	Query query = conn.query("select * from mall_m_member");
//	users_t res;
//	query.storein(res);
//	for (auto& i: res) {
//		cout << i.id << "\t" << i.uuid << "\t" << i.username << "\t" << i.truename << endl;
//	}

	QueryUser qu;
	MysqlConnPool pool;
	pool.init(10, bind(&QueryUser::init, &qu));

	vector<thread> threads;
	for (int i = 0; i < 1; ++i) {
		threads.emplace_back(thread(bind(&QueryUser::work, &qu, &pool)));
	}

	for (auto& i: threads) {
		i.join();
	}

	return 0;
}

