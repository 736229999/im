#ifndef COMMONDATABASE_MYSQLPOOL_H
#define COMMONDATABASE_MYSQLPOOL_H

#ifdef _WIN32
#  include <winsock2.h>
#endif


#include <CommonDatabase/MySqlPoolF.h>
#include <CommonUtil/Time.h>
#include <CommonUtil/Mutex.h>
#include <CommonUtil/LoggerUtil.h>
#include <CommonNet/BasicStream.h>
#include <map>
#include <mysql.h>

namespace CommonDatabase
{

class MySqlData : public CommonUtil::Shared
{
public:
	MySqlData()
	{
		m_pMySql = NULL;
		m_bUsed  = false;
	}
	virtual ~MySqlData() {
	
		if(m_bUsed)
		{
			LOG_TRACE("~MySqlData","in used");
			try
			{
				if( m_pMySql)
				{
					LOG_TRACE("~MySqlData","mysql_close");
					mysql_close(m_pMySql);
					m_pMySql = NULL;
				}

			}
			catch (...)
			{
				LOG_TRACE("~MySqlData","error expection");
			}

		}
		else
		{
			LOG_TRACE("~MySqlData","not in used");
			if( m_pMySql )
			{
				LOG_TRACE("~MySqlData","mysql_close");
				mysql_close(m_pMySql);
				m_pMySql = NULL;
			}
		}


	}
public:
	MYSQL* m_pMySql;
	bool m_bUsed;
	CommonUtil::Time m_tmLastActive;
};


class MySqlQueryData : public CommonUtil::Shared
{
public:
	MySqlQueryData(const MySqlPoolPtr& ptrMySqlPool, const MySqlDataPtr& ptrMySqlData, MYSQL_RES* pMySqlRes);
	virtual ~MySqlQueryData();
public:
	bool FetchRow(MYSQL_ROW& row);
private:
	MySqlPoolPtr m_ptrMySqlPool;
	MySqlDataPtr m_ptrMySqlData;
	MYSQL_RES* m_pMySqlRes;
};

class MySqlPool : public CommonUtil::Shared
{
public:
	MySqlPool();
	virtual ~MySqlPool();
public:
	inline void Init(const std::string& strHost, const std::string& strUser, const std::string& strPassword, const std::string& strCharsetName)
	{
		m_strHost = strHost;
		m_strUser = strUser;
		m_strPassword = strPassword;
		m_strCharsetName = strCharsetName;
	}
public:
	bool Execute(const char* pszSql);
	bool Query(const char* pszQuerySql, MySqlQueryDataPtr& ptrMySqlQueryData);
	bool ExecuteProcedure(const char* pszSql,CommonNet::BasicStreamPtr& pBasicStream);
private:
	MySqlDataPtr Get();
	void Release(const MySqlDataPtr& ptrMySqlData);
	void Drop(const MySqlDataPtr& ptrMySqlData);
private:
	MySqlDataPtr NewConnection();
private:
	std::map<MySqlData*, MySqlDataPtr> m_mapMySqlPtr;
	CommonUtil::Mutex m_lockMySqlPtr;
private:
	std::string m_strHost;
	std::string m_strUser;
	std::string m_strPassword;
	std::string m_strCharsetName;
private:
	int m_nConnectionGetCount;
	int m_nConnectionReleaseCount;

	friend class MySqlQueryData;
};

}

#endif
