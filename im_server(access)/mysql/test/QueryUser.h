/*
 * QueryUser.h
 *
 *  Created on: Dec 5, 2014
 *      Author: yaoxing
 */

#ifndef QUERYUSER_H_
#define QUERYUSER_H_

#include <mysql++/mysql++.h>
#include <vector>
#include "MysqlConnPool.h"

using namespace mysqlpp;
using namespace std;


class QueryUser
{
public:
	QueryUser();
	virtual ~QueryUser();

	Connection* init();

	void work(MysqlConnPool* pool);

	void query(Connection*);
};

#endif /* QUERYUSER_H_ */

