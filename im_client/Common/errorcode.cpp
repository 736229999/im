// errorcode.cpp
// ������
#include <stdafx.h>
#include "errorcode.h"
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
            sprintf(szError, "����ʧ��\n");
		else
			CLog::Log(szError, CLog::TYPE_IMPORTANT, "����ʧ��");
		return nErrCode;

	case DB_NOUSER:
		if (bGetError)
			sprintf(szError, "���û�������\n");
		else
			CLog::Log(szError, CLog::TYPE_IMPORTANT, "���û�������");
		return nErrCode;
	case DB_NOFAMILY:
		if (bGetError)
			sprintf(szError, "�ü��岻����\n");
		else
			CLog::Log(szError, CLog::TYPE_IMPORTANT, "�ü��岻����");
		return nErrCode;
	case DB_PARAM:
		if (bGetError)
			sprintf(szError, "��Ϣ����\n");
		else
			CLog::Log(szError, CLog::TYPE_IMPORTANT, "��������������ݿ����ʧ��");
		return nErrCode;
	case DB_FAMILYNAME_EXIST:
		if (bGetError)
			sprintf(szError, "�ü��������Ѿ�����\n");
		else
			CLog::Log(szError, CLog::TYPE_IMPORTANT, "�ü��������Ѿ�����");
		return nErrCode;
	case DB_FAMILYDOMAIN_EXIST:
		if (bGetError)
			sprintf(szError, "�ü��������Ѿ�����\n");
		else
			CLog::Log(szError, CLog::TYPE_IMPORTANT, "�ü��������Ѿ�����");
		return nErrCode;
	case DB_CREATE_FAMILY_FULL:
		if (bGetError)
			sprintf(szError, "�û���ഴ��%d���̶�����\n", MAX_CREATEFAMILY_NUM);
		else
			CLog::Log(szError, CLog::TYPE_IMPORTANT, "�û���ഴ���ļ����������");
		return nErrCode;
	case DB_FAMILYMEM_FULL:
		if (bGetError)
			sprintf(szError, "�����Ա������");
		else
			CLog::Log(szError, CLog::TYPE_IMPORTANT, "�����Ա������");
		return nErrCode;
	case DB_GROUP_EXIST:
		if (bGetError)
			sprintf(szError, "�������Ѿ�����");
		else
			CLog::Log(szError, CLog::TYPE_NORMAL, "�������Ѿ�����");
		return nErrCode;
	case DB_GROUT_FULL:
		if (bGetError)
			sprintf(szError, "���������ķ���������");
		else
			CLog::Log(szError, CLog::TYPE_NORMAL, "���������ķ���������");
		return nErrCode;
	case DB_FRIENDS_FULL:
		if (bGetError)
			sprintf(szError, "���ĺ�������������������Ӻ�����");
		else
			CLog::Log(szError, CLog::TYPE_NORMAL, "���ĺ�������������������Ӻ�����");
		return nErrCode;
	case DB_NOFRIEND:
		if (bGetError)
			sprintf(szError, "���û��������ĺ���");
		else
			CLog::Log(szError, CLog::TYPE_NORMAL,  "���û��������ĺ���");
		return nErrCode;
	default:
		return DB_SUCCESS;
	}
	return DB_SUCCESS;
}
