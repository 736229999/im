#include <CommonDatabase/MySqlPool.h>
#include <CommonUtil/LoggerUtil.h>
#include <CommonUtil/Exception.h>
#include <iostream>
#include <errmsg.h>
#include <stdio.h>

namespace
{
const int kMysqlConnectionIdleTimeout = 600;

class DatabaseException : public CommonUtil::Exception
{
public:
	DatabaseException(const char* file, int line)
		: CommonUtil::Exception(file, line)
	{
	}
	
	virtual ~DatabaseException() throw()
	{
	}
	virtual void print(std::ostream& out) const
	{
		CommonUtil::Exception::print(out);
		out << ":\ndatabase exception: ";
	}
public:
	std::string reason;
};
}

CommonDatabase::MySqlQueryData::MySqlQueryData(const MySqlPoolPtr& ptrMySqlPool, const CommonDatabase::MySqlDataPtr& ptrMySqlData, MYSQL_RES* pMySqlRes)
	: m_ptrMySqlPool(ptrMySqlPool)
	, m_ptrMySqlData(ptrMySqlData)
	, m_pMySqlRes(pMySqlRes)
{
	
}

CommonDatabase::MySqlQueryData::~MySqlQueryData()
{
	mysql_free_result(m_pMySqlRes);
	m_ptrMySqlPool->Release(m_ptrMySqlData);
}


bool CommonDatabase::MySqlQueryData::FetchRow(MYSQL_ROW& row)
{
	row = mysql_fetch_row(m_pMySqlRes);
	return (row != NULL);
}

CommonDatabase::MySqlPool::MySqlPool()
	: m_nConnectionGetCount(0)
	, m_nConnectionReleaseCount(0)
{
}

CommonDatabase::MySqlPool::~MySqlPool()
{
}

bool CommonDatabase::MySqlPool::Execute(const char* pszSql)
{
	try
	{
		CommonDatabase::MySqlDataPtr ptrMySqlData = Get();
		int ret = mysql_query(ptrMySqlData->m_pMySql, pszSql);
		if (ret) {
			if (ret==CR_SERVER_GONE_ERROR || ret==CR_SERVER_LOST || ret==CR_COMMANDS_OUT_OF_SYNC || ret==CR_UNKNOWN_ERROR) {
				printf("pszSql:%s with error:%d\n", pszSql,ret);
				throw DatabaseException(__FILE__, __LINE__);
				Drop(ptrMySqlData); 
				return false;
			} else {
				printf("pszSql:%s with error:%d\n", pszSql,ret);
				throw DatabaseException(__FILE__, __LINE__);
				Release(ptrMySqlData);
				return false;
			}
		}

		Release(ptrMySqlData);
		return true;

	}
	catch (DatabaseException e)
	{
		LOG_ERROR("MySqlPool::Execute DatabaseException:" << e.what());
		return false;
	}

	return false;
}

bool CommonDatabase::MySqlPool::Query(const char* pszQuerySql, CommonDatabase::MySqlQueryDataPtr& ptrMySqlQueryData)
{
	try
	{

		CommonDatabase::MySqlDataPtr ptrMySqlData = Get();
		if(ptrMySqlData == NULL)
		{
			printf("MySqlPool::Query get MySqlDataPtr failed\n");
			return false;
		}
		int ret = mysql_query(ptrMySqlData->m_pMySql, pszQuerySql);
		if (ret) {
			std::cout << pszQuerySql << std::endl;
			if (ret==CR_SERVER_GONE_ERROR || ret==CR_SERVER_LOST || ret==CR_COMMANDS_OUT_OF_SYNC || ret==CR_UNKNOWN_ERROR) {
				throw DatabaseException(__FILE__, __LINE__);
				Drop(ptrMySqlData); 
				return false;
			} else {
				throw DatabaseException(__FILE__, __LINE__);
				Release(ptrMySqlData);
				return false;
			}
		}

		MYSQL_RES* res = mysql_store_result(ptrMySqlData->m_pMySql);
		if (res == NULL) {
			Release(ptrMySqlData);
			return false;
		}

		ptrMySqlQueryData = new CommonDatabase::MySqlQueryData(this, ptrMySqlData, res);
		
		return true;
	}
	catch (DatabaseException e)
	{
		LOG_ERROR("MySqlPool::Query DatabaseException:" << e.what());
		return false;
	}

	return false;
}

bool CommonDatabase::MySqlPool::ExecuteProcedure(const char* pszSql,CommonNet::BasicStreamPtr& pBasicStream)
{
	try
	{

		CommonDatabase::MySqlDataPtr ptrMySqlData = Get();
		int ret = mysql_query(ptrMySqlData->m_pMySql, pszSql);
		if (ret) {
			printf("\n%s\n",mysql_error(ptrMySqlData->m_pMySql));
			std::cout << pszSql << std::endl;
			if (ret==CR_SERVER_GONE_ERROR || ret==CR_SERVER_LOST || ret==CR_COMMANDS_OUT_OF_SYNC || ret==CR_UNKNOWN_ERROR) {
				//throw DatabaseException(__FILE__, __LINE__);
				Drop(ptrMySqlData); 
				return false;
			} else {
				//throw DatabaseException(__FILE__, __LINE__);
				Release(ptrMySqlData);
				return false;
			}
		}

		MYSQL_RES* res = NULL;
		MYSQL_ROW row;    //返回数据行
		do
		{
			int nFieldNum = 0 ;
			res = mysql_store_result(ptrMySqlData->m_pMySql);
			if (res == NULL) {
				nFieldNum = 0;
				pBasicStream->write(nFieldNum);
				//printf("mysql_store_result failed\n");
				break;
			}

			nFieldNum = (int)mysql_num_fields(res);
				
		//	printf("execute result fieldNum is %d\n",nFieldNum);
			pBasicStream->write(nFieldNum);
			unsigned int rowCount = res->row_count;
		//	printf("execute result rowNum is %d\n",(int)rowCount);
			pBasicStream->write( (CommonNet::ULong)(rowCount+1));
		//	printf("execute result write rowCount finish\n");	

			MYSQL_FIELD * pField = NULL;
		//	int nFieldCount = 0 ;
			while( (pField = mysql_fetch_field(res))!=NULL )
			{
				pBasicStream->write( std::string(pField->name) );
			//	printf("nFieldCount[%d] %s\n",nFieldCount++,pField->name);
			};
		//	printf("execute result write all field finish\n");	

			row = mysql_fetch_row( res );

			while(  row != NULL)
			{
				for(int i = 0 ; i < nFieldNum;++i)
				{
					std::string strValue = row[i]==NULL?"":row[i];
					pBasicStream->write(strValue);
				//	printf("row[%d]=%s\n",i,strValue.c_str());
				}

				row = mysql_fetch_row( res );
		
			}

		//	printf("execute result write all row data finish\n");

			mysql_free_result( res ); 
		}
		while(! mysql_next_result(ptrMySqlData->m_pMySql) );

		Release(ptrMySqlData);
		//printf("execute result write all finish\n");
		return true;

	}
	catch (DatabaseException e)
	{
		LOG_ERROR("MySqlPool::ExecuteProcedure DatabaseException:" << e.what());
		return false;
	}

	return false;
}


CommonDatabase::MySqlDataPtr CommonDatabase::MySqlPool::Get()
{
	CommonUtil::Mutex::Lock autolock(m_lockMySqlPtr);
	++m_nConnectionGetCount;
	for (std::map<MySqlData*, MySqlDataPtr>::iterator i = m_mapMySqlPtr.begin(); i != m_mapMySqlPtr.end();) {
		MySqlDataPtr& ptrMySqlData = i->second;
		if (ptrMySqlData->m_bUsed == false) {
			if ((CommonUtil::Time::now().toSeconds()-ptrMySqlData->m_tmLastActive.toSeconds()) > kMysqlConnectionIdleTimeout) {

				LOG_TRACE("MySqlPool","ready to erase sqlpool");
#ifdef _WIN32
				i = m_mapMySqlPtr.erase(i);
#else
				//m_mapMySqlPtr.erase(i++);
				std::map<MySqlData*, MySqlDataPtr>::iterator iTemp = i;
				iTemp++;
				m_mapMySqlPtr.erase(i);
				i=iTemp;
#endif
				
				continue;
			}
	//		printf("reuse sql pool \n");
			ptrMySqlData->m_tmLastActive = CommonUtil::Time::now();
			ptrMySqlData->m_bUsed = true;

			LOG_TRACE("MySqlPool","reuse old sqlpool,all size="<< m_mapMySqlPtr.size());
			return i->second;
		} else {
			++i;
		}
	}

	CommonDatabase::MySqlDataPtr ptrMySqlData = NewConnection();
	m_mapMySqlPtr[ptrMySqlData.get()] = ptrMySqlData;
	ptrMySqlData->m_bUsed = true;

	return ptrMySqlData;
}

void CommonDatabase::MySqlPool::Release(const CommonDatabase::MySqlDataPtr& ptrMySqlData)
{
	++m_nConnectionReleaseCount;
	ptrMySqlData->m_bUsed = false;

	LOG_TRACE("MySqlPool","Release. count=" << m_nConnectionReleaseCount);
}

void CommonDatabase::MySqlPool::Drop(const MySqlDataPtr& ptrMySqlData)
{
	++m_nConnectionReleaseCount;
	CommonUtil::Mutex::Lock autolock(m_lockMySqlPtr);
	printf("mysql do close connect on drop\n");
	mysql_close(ptrMySqlData->m_pMySql);
	ptrMySqlData->m_pMySql = NULL;
	m_mapMySqlPtr.erase(ptrMySqlData.get());
}

CommonDatabase::MySqlDataPtr CommonDatabase::MySqlPool::NewConnection()
{
	CommonDatabase::MySqlData* pMySqlData = new CommonDatabase::MySqlData();
	pMySqlData->m_bUsed = false;
	pMySqlData->m_pMySql = mysql_init(NULL);
	if(pMySqlData->m_pMySql) {
		int timeout;
		timeout = 15;
		mysql_options(pMySqlData->m_pMySql, MYSQL_OPT_CONNECT_TIMEOUT, (const char *)&timeout);
		mysql_options(pMySqlData->m_pMySql, MYSQL_SET_CHARSET_NAME, m_strCharsetName.c_str());
		if (mysql_real_connect(pMySqlData->m_pMySql, m_strHost.c_str(), m_strUser.c_str(), m_strPassword.c_str(), NULL, 0, NULL, CLIENT_MULTI_RESULTS)) {
			printf("mysql do connect succceed!\n");
			pMySqlData->m_tmLastActive = CommonUtil::Time::now();

			LOG_TRACE("MySqlPool","newConnection ,old size=" << m_mapMySqlPtr.size());

		} else {

			LOG_ERROR("mysql_real_connect connect-num:" << m_nConnectionGetCount << " release-num:" << m_nConnectionReleaseCount 
				<< " error:" << mysql_error(pMySqlData->m_pMySql) );

			mysql_close(pMySqlData->m_pMySql);
			pMySqlData->m_pMySql = NULL;
			delete pMySqlData;
			pMySqlData = NULL;

			throw DatabaseException(__FILE__, __LINE__);
		}
	} else {
		delete pMySqlData;
		pMySqlData = NULL;
	}
	
	return pMySqlData;
}
