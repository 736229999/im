// errorcode.cpp
// 错误处理
#include <stdafx.h>
#include "ErrorCode.h"
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
            strcpy(szError, "操作失败,请稍后再试\n");
		else
			CLog::Log(szError, CLog::TYPE_DEBUG, "操作失败,请稍后再试");
		break;
	case DB_USERNAME_EXIST:
		if (bGetError)
			strcpy(szError, "用户名已经存在");
		else
			CLog::Log(szError, CLog::TYPE_DEBUG, "用户名已经存在");
		break;
	case DB_NICKNAME_EXIST:
		if (bGetError)
			strcpy(szError,"昵称已经存在");
		else
			CLog::Log(szError, CLog::TYPE_DEBUG, "昵称已经存在");
		break;
	case DB_NOUSER:
		if (bGetError)
			strcpy(szError, "该用户不存在\n");
		else
			CLog::Log(szError, CLog::TYPE_DEBUG, "该用户不存在");
		break;
	case DB_NOFAMILY:
		if (bGetError)
			strcpy(szError, "该家族不存在\n");
		else
			CLog::Log(szError, CLog::TYPE_DEBUG, "该家族不存在");
		break;
	case DB_PARAM:
		if (bGetError)
			strcpy(szError, "信息错误\n");
		else
			CLog::Log(szError, CLog::TYPE_DEBUG, "传入参数错误，数据库操作失败");
		break;
	case DB_FAMILYNAME_EXIST:
		if (bGetError)
			strcpy(szError, "该家族名称已经存在\n");
		else
			CLog::Log(szError, CLog::TYPE_DEBUG, "该家族名称已经存在");
		break;
	case DB_FAMILYDOMAIN_EXIST:
		if (bGetError)
			strcpy(szError, "该家族域名已经存在\n");
		else
			CLog::Log(szError, CLog::TYPE_DEBUG, "该家族域名已经存在");
		break;
	case DB_CREATE_FAMILY_FULL:
		if (bGetError)
			sprintf(szError, "用户最多创建%d个固定家族\n", MAX_CREATEFAMILY_NUM);
		else
			CLog::Log(szError, CLog::TYPE_DEBUG, "用户最多创建的家族个数已满");
		break;
	case DB_FAMILYMEM_FULL:
		if (bGetError)
			strcpy(szError, "家族成员数已满");
		else
			CLog::Log(szError, CLog::TYPE_DEBUG, "家族成员数已满");
		break;
	case DB_GROUP_EXIST:
		if (bGetError)
			strcpy(szError, "该组名已经存在");
		else
			CLog::Log(szError, CLog::TYPE_DEBUG, "该组名已经存在");
		break;
	case DB_GROUP_FULL:
		if (bGetError)
			strcpy(szError, "您所创建的分组数已满");
		else
			CLog::Log(szError, CLog::TYPE_DEBUG, "您所创建的分组数已满");
		break;
	case DB_FRIENDS_FULL:
		if (bGetError)
			strcpy(szError, "您的好友数已满，不能再添加好友了");
		else
			CLog::Log(szError, CLog::TYPE_DEBUG, "您的好友数已满，不能再添加好友了");
		break;
	case DB_NOFRIEND:
		if (bGetError)
			strcpy(szError, "该用户不是您的好友");
		else
			CLog::Log(szError, CLog::TYPE_DEBUG,  "该用户不是您的好友");
		break;
	case DB_NICKNAME_ERROR:
		if (bGetError)
			strcpy(szError, "昵称包含非法字符");
		else
			CLog::Log(szError, CLog::TYPE_DEBUG, "昵称包含非法字符");
		break;
	case DB_FAMILYNAME_ERROR:
		if (bGetError)
			strcpy(szError, "家族名称包含非法字符");
		else
			CLog::Log(szError, CLog::TYPE_DEBUG, "家族名称包含非法字符");
		break;
	default:
		return DB_SUCCESS;
	}
	return nErrCode;
}
