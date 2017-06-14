/*  errorcode.h

  错误编码
  1。0 -- 10000 表示数据库错误码
  2。10001 -- 20000  服务器内部错误码
  3。20001 -- 30000	 ……

*/
#ifndef _ERRORCODE_H_
#define _ERRORCODE_H_

/********************************服务器内部错误码定义***************************/
#define  BUSIERR_INTERNAL   10001//服务内部错误
#define  BUSIERR_GREGISTER  10002//向全局注册用户错误

/************************************数据库错误码*******************************/
//需要在CError::DB_Error中定义错误描述

#define DB_SUCCESS						0	//处理成功
#define DB_ERROR						1	//数据库本身错误
#define DB_NOUSER						2	//用户不存在
#define DB_NOFAMILY						3	//家族不存在
#define DB_PARAM						4	//传入的参数错误
#define DB_FAMILYNAME_EXIST				5	//家族名称已经存在
#define DB_FAMILYDOMAIN_EXIST			6	//家族域名已经存在
#define DB_CREATE_FAMILY_FULL			7	//用户创建的家族数已到限制
#define DB_FAMILYMEM_FULL				8	//家族成员数已满，无法再添加

#define DB_GROUP_EXIST					9	//用户分组的组名已经存在
#define DB_GROUT_FULL					10	//用户分组已达到限制
#define DB_FRIENDS_FULL					11	//用户所加的好友已满
#define DB_NOFRIEND						12  //该用户不是您的好友

/************************************数据库错误码*******************************/

////////////////////////////////////////////////////////////////////////////

class CError
{
public:
	CError();
	~CError();
	static uint32 DB_Error(unsigned int nErrCode, char * szError = NULL, bool bGetError = false);

};




#endif
