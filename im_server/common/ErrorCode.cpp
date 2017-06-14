// errorcode.cpp
// ������
#include <stdafx.h>
#include "ErrorCode.h"
#include "PublicType.h"
#include <time.h>

/*================================================================ 
*
* �� �� ����DB_Error 
** �������ȡ���ݿ��������
* �� ���� 
* unsigned int nErrCode [IN] : ����ID 
* char * szErrFrom	[IN,OUT] : ��������
* bool bGetError [[IN]] : true ��ȡ����������false ����д��LOG
* �� �� ֵ��uint32 ����ID
* 
================================================================*/
uint32 CError::DB_Error(unsigned int nErrCode, char * szError, bool bGetError)
{
	
	switch(nErrCode)
	{
	case DB_ERROR:
		if (bGetError)
            strcpy(szError, "����ʧ��,���Ժ�����\n");
		else
			CLog::Log(szError, CLog::TYPE_DEBUG, "����ʧ��,���Ժ�����");
		break;
	case DB_USERNAME_EXIST:
		if (bGetError)
			strcpy(szError, "�û����Ѿ�����");
		else
			CLog::Log(szError, CLog::TYPE_DEBUG, "�û����Ѿ�����");
		break;
	case DB_NICKNAME_EXIST:
		if (bGetError)
			strcpy(szError,"�ǳ��Ѿ�����");
		else
			CLog::Log(szError, CLog::TYPE_DEBUG, "�ǳ��Ѿ�����");
		break;
	case DB_NOUSER:
		if (bGetError)
			strcpy(szError, "���û�������\n");
		else
			CLog::Log(szError, CLog::TYPE_DEBUG, "���û�������");
		break;
	case DB_NOFAMILY:
		if (bGetError)
			strcpy(szError, "�ü��岻����\n");
		else
			CLog::Log(szError, CLog::TYPE_DEBUG, "�ü��岻����");
		break;
	case DB_PARAM:
		if (bGetError)
			strcpy(szError, "��Ϣ����\n");
		else
			CLog::Log(szError, CLog::TYPE_DEBUG, "��������������ݿ����ʧ��");
		break;
	case DB_FAMILYNAME_EXIST:
		if (bGetError)
			strcpy(szError, "�ü��������Ѿ�����\n");
		else
			CLog::Log(szError, CLog::TYPE_DEBUG, "�ü��������Ѿ�����");
		break;
	case DB_FAMILYDOMAIN_EXIST:
		if (bGetError)
			strcpy(szError, "�ü��������Ѿ�����\n");
		else
			CLog::Log(szError, CLog::TYPE_DEBUG, "�ü��������Ѿ�����");
		break;
	case DB_CREATE_FAMILY_FULL:
		if (bGetError)
			sprintf(szError, "�û���ഴ��%d���̶�����\n", MAX_CREATEFAMILY_NUM);
		else
			CLog::Log(szError, CLog::TYPE_DEBUG, "�û���ഴ���ļ����������");
		break;
	case DB_FAMILYMEM_FULL:
		if (bGetError)
			strcpy(szError, "�����Ա������");
		else
			CLog::Log(szError, CLog::TYPE_DEBUG, "�����Ա������");
		break;
	case DB_GROUP_EXIST:
		if (bGetError)
			strcpy(szError, "�������Ѿ�����");
		else
			CLog::Log(szError, CLog::TYPE_DEBUG, "�������Ѿ�����");
		break;
	case DB_GROUP_FULL:
		if (bGetError)
			strcpy(szError, "���������ķ���������");
		else
			CLog::Log(szError, CLog::TYPE_DEBUG, "���������ķ���������");
		break;
	case DB_FRIENDS_FULL:
		if (bGetError)
			strcpy(szError, "���ĺ�������������������Ӻ�����");
		else
			CLog::Log(szError, CLog::TYPE_DEBUG, "���ĺ�������������������Ӻ�����");
		break;
	case DB_NOFRIEND:
		if (bGetError)
			strcpy(szError, "���û��������ĺ���");
		else
			CLog::Log(szError, CLog::TYPE_DEBUG,  "���û��������ĺ���");
		break;
	case DB_NICKNAME_ERROR:
		if (bGetError)
			strcpy(szError, "�ǳư����Ƿ��ַ�");
		else
			CLog::Log(szError, CLog::TYPE_DEBUG, "�ǳư����Ƿ��ַ�");
		break;
	case DB_FAMILYNAME_ERROR:
		if (bGetError)
			strcpy(szError, "�������ư����Ƿ��ַ�");
		else
			CLog::Log(szError, CLog::TYPE_DEBUG, "�������ư����Ƿ��ַ�");
		break;
	default:
		return DB_SUCCESS;
	}
	return nErrCode;
}
