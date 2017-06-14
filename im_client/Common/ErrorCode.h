/*  errorcode.h

  �������
  1��0 -- 10000 ��ʾ���ݿ������
  2��10001 -- 20000  �������ڲ�������
  3��20001 -- 30000	 ����

*/
#ifndef _ERRORCODE_H_
#define _ERRORCODE_H_

/********************************�������ڲ������붨��***************************/
#define  BUSIERR_INTERNAL   10001//�����ڲ�����
#define  BUSIERR_GREGISTER  10002//��ȫ��ע���û�����

/************************************���ݿ������*******************************/
//��Ҫ��CError::DB_Error�ж����������

#define DB_SUCCESS						0	//����ɹ�
#define DB_ERROR						1	//���ݿⱾ�����
#define DB_NOUSER						2	//�û�������
#define DB_NOFAMILY						3	//���岻����
#define DB_PARAM						4	//����Ĳ�������
#define DB_FAMILYNAME_EXIST				5	//���������Ѿ�����
#define DB_FAMILYDOMAIN_EXIST			6	//���������Ѿ�����
#define DB_CREATE_FAMILY_FULL			7	//�û������ļ������ѵ�����
#define DB_FAMILYMEM_FULL				8	//�����Ա���������޷������

#define DB_GROUP_EXIST					9	//�û�����������Ѿ�����
#define DB_GROUT_FULL					10	//�û������Ѵﵽ����
#define DB_FRIENDS_FULL					11	//�û����ӵĺ�������
#define DB_NOFRIEND						12  //���û��������ĺ���

/************************************���ݿ������*******************************/

////////////////////////////////////////////////////////////////////////////

class CError
{
public:
	CError();
	~CError();
	static uint32 DB_Error(unsigned int nErrCode, char * szError = NULL, bool bGetError = false);

};




#endif
