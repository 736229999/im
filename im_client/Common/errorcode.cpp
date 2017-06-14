// errorcode.cpp
// 错误处理
#include <stdafx.h>
#include "errorcode.h"
#include "PublicType.h"
#include <time.h>

/*================================================================ 
*
* 函 数 名：DB_Error 
** 输出、获取数据库错误描述
* 参 数： 
* unsigned int nErrCode [IN] : 错误ID 
* char * szErrFrom	[IN,OUT] : 错误描述
* bool bGetError [[IN]] : true 获取错误描述，false 错误写入LOG
* 返 回 值：uint32 错误ID
* 
================================================================*/
uint32 CError::DB_Error(unsigned int nErrCode, char * szError, bool bGetError)
{
	
	switch(nErrCode)
	{
	case DB_ERROR:
		if (bGetError)
            sprintf(szError, "操作失败\n");
		else
			CLog::Log(szError, CLog::TYPE_IMPORTANT, "操作失败");
		return nErrCode;

	case DB_NOUSER:
		if (bGetError)
			sprintf(szError, "该用户不存在\n");
		else
			CLog::Log(szError, CLog::TYPE_IMPORTANT, "该用户不存在");
		return nErrCode;
	case DB_NOFAMILY:
		if (bGetError)
			sprintf(szError, "该家族不存在\n");
		else
			CLog::Log(szError, CLog::TYPE_IMPORTANT, "该家族不存在");
		return nErrCode;
	case DB_PARAM:
		if (bGetError)
			sprintf(szError, "信息错误\n");
		else
			CLog::Log(szError, CLog::TYPE_IMPORTANT, "传入参数错误，数据库操作失败");
		return nErrCode;
	case DB_FAMILYNAME_EXIST:
		if (bGetError)
			sprintf(szError, "该家族名称已经存在\n");
		else
			CLog::Log(szError, CLog::TYPE_IMPORTANT, "该家族名称已经存在");
		return nErrCode;
	case DB_FAMILYDOMAIN_EXIST:
		if (bGetError)
			sprintf(szError, "该家族域名已经存在\n");
		else
			CLog::Log(szError, CLog::TYPE_IMPORTANT, "该家族域名已经存在");
		return nErrCode;
	case DB_CREATE_FAMILY_FULL:
		if (bGetError)
			sprintf(szError, "用户最多创建%d个固定家族\n", MAX_CREATEFAMILY_NUM);
		else
			CLog::Log(szError, CLog::TYPE_IMPORTANT, "用户最多创建的家族个数已满");
		return nErrCode;
	case DB_FAMILYMEM_FULL:
		if (bGetError)
			sprintf(szError, "家族成员数已满");
		else
			CLog::Log(szError, CLog::TYPE_IMPORTANT, "家族成员数已满");
		return nErrCode;
	case DB_GROUP_EXIST:
		if (bGetError)
			sprintf(szError, "该组名已经存在");
		else
			CLog::Log(szError, CLog::TYPE_NORMAL, "该组名已经存在");
		return nErrCode;
	case DB_GROUT_FULL:
		if (bGetError)
			sprintf(szError, "您所创建的分组数已满");
		else
			CLog::Log(szError, CLog::TYPE_NORMAL, "您所创建的分组数已满");
		return nErrCode;
	case DB_FRIENDS_FULL:
		if (bGetError)
			sprintf(szError, "您的好友数已满，不能再填加好友了");
		else
			CLog::Log(szError, CLog::TYPE_NORMAL, "您的好友数已满，不能再填加好友了");
		return nErrCode;
	case DB_NOFRIEND:
		if (bGetError)
			sprintf(szError, "该用户不是您的好友");
		else
			CLog::Log(szError, CLog::TYPE_NORMAL,  "该用户不是您的好友");
		return nErrCode;
	default:
		return DB_SUCCESS;
	}
	return DB_SUCCESS;
}
