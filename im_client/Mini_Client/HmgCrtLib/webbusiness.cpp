#include "stdafx.h"
#include "hmgcrtlib.h"
//
// ���ļ�ʵ�ֺ͸��˿ռ�ҵ����صĹ�������
//

/////////////////////////////////////////////////////////////////////////////////////////
//  MakePassportUrl
//  ���ܣ����ɵ�½
//  ���룺strUrl, ���ɵ�½��url
//        nUserID, �û�ID
//		  strUserName, �û���
//        strNickName, �ǳ�
//        nSex,  �Ա�
//        strEmail, email��ַ
//        strFaceUrl, ͷ��url
//		  nGoldMoney, �����
//        nMessageCount, ��Ϣ�� 
//  �����
//  ����ֵ�����ɵ�½url
//  �޸ļ�¼��
/////////////////////////////////////////////////////////////////////////////////////////
CString MakePassportUrl(CString		strUrl,
						  long		nUserID,
						  CString	strUserName,
						  CString   strNickName,
						  int		nSex,
						  CString	strEmail,
						  CString	strFaceUrl,
						  int		nGoldMoney,
						  int		nMessageCount
						)
{
	CString strOpen;
	CString strParamInfo = "";

	strParamInfo = strParamInfo + UIntToStr(nUserID)+"&";
	strParamInfo = strParamInfo + CString(strUserName)+"&";
	strParamInfo = strParamInfo + UrlEncode(strNickName)+"&";
	strParamInfo = strParamInfo + UIntToStr(nSex)+"&";
	strParamInfo = strParamInfo + UrlEncode(strEmail)+"&";
	strParamInfo = strParamInfo + UIntToStr(nGoldMoney)+"&";
	strParamInfo = strParamInfo + UrlEncode(strFaceUrl)+"&";
	strParamInfo = strParamInfo + IntToStr(nMessageCount);

	strParamInfo = Base64Encode(strParamInfo);


	CString strKey    = "DJFU#(R#URJ#@Od#@$#@09809";
	CString strUserId = UIntToStr(nUserID);
	CString strParamKey;

	int l = strUserId.GetLength();
	for ( int i=0; i<l; i++ )
	{
		strParamKey+=strUserId[i];
		strParamKey+=strKey[i];
	}

	strParamKey+=strKey.Right(strKey.GetLength()-l);

	unsigned char digest[16];

	MD5_CTX md5T;
	md5T.MD5Update ((unsigned char*)(LPCSTR)strParamKey,(unsigned int)strParamKey.GetLength() );
	md5T.MD5Final (digest);

	strParamKey = ToHexString(digest,16);

	//strOpen.Format("http://passport.iecmall.com/MiniOpen.aspx?info=%s&key=%s&url=%s",strParamInfo,strParamKey,strUrl);
	strOpen.Format("http://www.iecmall.com");


	return strOpen;
}


/////////////////////////////////////////////////////////////////////////////////////////
//  DateStr2Long
//  ���ܣ���web���ص�ʱ���ַ���ת��ΪDWORD
//  ���룺
//  �����
//  ����ֵ��
//  �޸ļ�¼��
/////////////////////////////////////////////////////////////////////////////////////////
DWORD DateStr2Long(CString strDate)
{
	//2005-1-1 12:0:0
	//2005/1/1
	DWORD year ;
	DWORD month, day;
	DWORD hour, miniute, second;

	year = month = day = hour = miniute = second = 0;

	char *pstr = (char *)(LPCSTR)strDate;

	long ret = 0;
	try
	{
		if ( pstr[4]=='/' )
//			sscanf(pstr,"%d/%d/%d %d:%d:%d",&year,&month,&day,&hour,&miniute,&second);
			sscanf_s(pstr,"%d/%d/%d %d:%d:%d",&year,&month,&day,&hour,&miniute,&second);
		else
		if ( pstr[4]=='-' )
//			sscanf(pstr,"%d-%d-%d %d:%d:%d",&year,&month,&day,&hour,&miniute,&second);
			sscanf_s(pstr,"%d-%d-%d %d:%d:%d",&year,&month,&day,&hour,&miniute,&second);

	}
	catch(...)
	{

	}
	//ret =  ( (DWORD)( year<<4 | month )<<16) ;//  | (DWORD)( (((WORD)(day<<4|hour))<<8)|((WORD)(miniute<<4 | second)) ); 
	//	  ((((WORD)day)<<8|hour)<<12) |(((WORD)miniute)<<8 | (WORD)second ); 

	year = year - 1971;

	if ( ((long)year)<0 )
		year=0;

	ret = second | (miniute<<6) | (hour<<12) | (day<<17) | (month<<22) | (year<<26);

	return ret;
}


/////////////////////////////////////////////////////////////////////////////////////////
//  ComboInitViewProp
//  ���ܣ���ʼ��ComboBoxȨ�޲鿴���ݺ�����
//  ���룺
//  �����
//  ����ֵ��
//  �޸ļ�¼��
/////////////////////////////////////////////////////////////////////////////////////////
void  ComboInitViewProp(CComboBox *pCombo)
{
	pCombo->ResetContent();

	int i;

	i=pCombo->AddString("˽��");//0
	pCombo->SetItemData(i,0);

	i=pCombo->AddString("���ѿɼ�");//1
	pCombo->SetItemData(i,1);

	//pCombo->AddString("139�û�");//2
	i=pCombo->AddString("����");//3
	pCombo->SetItemData(i,3);
}

/////////////////////////////////////////////////////////////////////////////////////////
//  ComboSelViewProp
//  ���ܣ�ѡ�ж�ӦȨ�޵�����
//  ���룺
//  �����
//  ����ֵ��
//  �޸ļ�¼��
/////////////////////////////////////////////////////////////////////////////////////////
void ComboSelViewProp(CComboBox *pCombo,int iViewProp)
{
	int n = pCombo->GetCount();

	for ( int i=0; i<n; i++ )
	{
		if ( pCombo->GetItemData(i)==iViewProp )
		{
			pCombo->SetCurSel(i);
			break;
		}

	}
}

/////////////////////////////////////////////////////////////////////////////////////////
//  ComboInitFamilyClass
//  ���ܣ���ʼ��Ⱥ����
//  ���룺
//  �����
//  ����ֵ��
//  �޸ļ�¼��
/////////////////////////////////////////////////////////////////////////////////////////
void ComboInitFamilyClass(CComboBox *pComboBox )
{

	static LPCSTR c_type [] = { 
		"����", "Ӱ��", "����","��Ц","����","��ʳ",
			"���л���","����","ʱ��","��Ӱ","����","����",
			"��������","Ӱ������","��������","����","����",
			"�赸","����","��ͷ����","������Ϸ","������Ϸ",
			"������Ϸ","������Ϸ","��Ϸ����","�����˶�","��ѧ",
			"С˵","����","ʫ��","���","����","�������","�ִ�����",
			"��ͳ����","��Ϸ���","��������","����ѧϰ","ͬѧ",
			"����","����","�ֻ�","����","DC/DV","MP3/MP4","����",
			"��ҵ","��ְ","��ѧ����","��ѧ","����","Ů��","ѧϰ����",
			"���ӻ�","����","����","��������","����"
	};

	static int    c_id   [] = {101, 102, 103, 104,105, 201, 202, 203, 204,
		205, 206, 207, 208, 10, 20, 301, 302, 303, 304,
		305,401,402,403,404,405,30,501,502,503,504,
		505,506,507,508,509,40,50,60,605,606,701,
		702,703,704,705,706,601,602,603,604,607,608,
		609,901,902,903,70,90
	};


	int n = sizeof(c_id)/sizeof(int);

	pComboBox->ResetContent();

	for ( int i=0; i<n; i++ )
	{
		pComboBox->AddString(c_type[i]);
		pComboBox->SetItemData(i,c_id[i]);
	}

	pComboBox->SetCurSel(2);
}

/////////////////////////////////////////////////////////////////////////////////////////
//  ComboSelFamilyClass
//  ���ܣ�ѡ�ж�Ӧ��Ⱥ����
//  ���룺
//  �����
//  ����ֵ��
//  �޸ļ�¼��
/////////////////////////////////////////////////////////////////////////////////////////
void ComboSelFamilyClass(CComboBox *pComboBox, int iClass)
{
	int  n = pComboBox->GetCount();
	for ( int i=0; i<pComboBox->GetCount(); i++ )
	{
		int nClass = pComboBox->GetItemData(i);
		if ( nClass == iClass )
		{
			pComboBox->SetCurSel(i);
			break;
		}
	}
}

/////////////////////////////////////////////////////////////////////////////////////////
//  ComboGetFamilyClass
//  ���ܣ���ȡ��Ӧ��Ⱥ����
//  ���룺
//  �����
//  ����ֵ��
//  �޸ļ�¼��
/////////////////////////////////////////////////////////////////////////////////////////
DWORD ComboGetFamilyClass(CComboBox *pComboBox)
{
	int nSel = pComboBox->GetCurSel();
	if ( nSel>-1 )
		return pComboBox->GetItemData(nSel);
	else
		return (DWORD)-1;
}

/////////////////////////////////////////////////////////////////////////////////////////
//  ComboGetViewProp
//  ���ܣ���ȡ��ǰComboBox��Ȩ��ѡ��
//  ���룺
//  �����
//  ����ֵ��
//  �޸ļ�¼��
/////////////////////////////////////////////////////////////////////////////////////////
int ComboGetViewProp(CComboBox *pCombo)
{
	int nSel = pCombo->GetCurSel();
	if ( nSel==-1 )
		return 0;
	else
		return pCombo->GetItemData(nSel);
}

/////////////////////////////////////////////////////////////////////////////////////////
//  ComboGetViewProp
//  ���ܣ���ȡ��ǰComboBox��Ȩ��ѡ��
//  ���룺
//  �����
//  ����ֵ��
//  �޸ļ�¼��
/////////////////////////////////////////////////////////////////////////////////////////
CString MakeMD5Password(char* pPassword)
{
	unsigned char digest[16];

	char password[255] = {0};
//	sprintf(password,"%sasdfds!@#$1234@!#$ASDFasdfQ@#4345asDF",pPassword);
	sprintf_s(password,sizeof(password),"%sasdfds!@#$1234@!#$ASDFasdfQ@#4345asDF",pPassword);

	MD5_CTX md5T;
	md5T.MD5Update ((unsigned char*)password,(unsigned int)strlen(password) );
	md5T.MD5Final (digest);

	return ToHexString(digest,16);
}

CString MakeIMAuthString(CString strUserName)
{
	char crypt[1025] = {0};
	char digest[16];

	strUserName.Trim();	
	strUserName = strUserName+"||"+CTime::GetCurrentTime().Format("%Y-%m-%d");

//	sprintf(crypt,"%sasdfds!@#$1234@!#$ASDFasdfQ@#4345asDF",strUserName);
	sprintf_s(crypt,sizeof(crypt),"%sasdfds!@#$1234@!#$ASDFasdfQ@#4345asDF",strUserName);

	MD5_CTX md5T;
	md5T.MD5Update ((unsigned char*)crypt,strlen(crypt) );
	md5T.MD5Final ((unsigned char*)digest);

	return ToHexString((unsigned char*)digest,16);
}
