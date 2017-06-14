/*
 * QueryUser.cpp
 *
 *  Created on: Dec 5, 2014
 *      Author: yaoxing
 */

#include "QueryUser.h"
#include "MysqlConnPool.h"
#include <mysql++/ssqls.h>
#include <iostream>
#include <functional>

sql_create_2(multi_select,
		1, 0,
		sql_int,			id,
		Null<sql_varchar>,	merchant_code)

sql_create_21(mall_m_member,
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
		Null<sql_varchar>,	job)

using users_t = vector<mall_m_member>;

QueryUser::QueryUser()
{

}

QueryUser::~QueryUser()
{

}

Connection* QueryUser::init()
{
	Connection* conn = new Connection;
	SetCharsetNameOption *charOP = new SetCharsetNameOption("utf8");
	conn->set_option(charOP);
	conn->connect("member", "stage-fnscdb.idc1.fn", "usr_sc", "m3E9B7er");
	return conn;
}

void QueryUser::work(MysqlConnPool* pool)
{
	pool->exec(bind(&QueryUser::query, this, placeholders::_1));
}

void QueryUser::query(Connection* conn)
{
	Query query = conn->query("select mall_m_member.id as id, mall_m_member.merchant_code from mall_m_member join "
			"mall_m_department on mall_m_member.merchant_code = mall_m_department.merchant_code");
	vector<multi_select> res;
	query.storein(res);
	for (auto& i: res) {
		cout << i.id << "\t" << i.merchant_code << endl;
	}
}

