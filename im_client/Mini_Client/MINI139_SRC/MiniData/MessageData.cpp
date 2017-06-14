#include "StdAfx.h"
#include ".\messagedata.h"
#include <io.h>

_MINIDATA_BEG

CMiniRecordInfo CDataManager::MSG_INFO = CMiniRecordInfo(MSG_VER1,sizeof(MSG_VER1)/sizeof(FIELD_INFO),XT_MSG::VER);
CMiniRecordInfo CDataManager::SYS_MSG_INFO = CMiniRecordInfo(SYS_MSG_VER1,sizeof(SYS_MSG_VER1)/sizeof(FIELD_INFO),SYSTEM_MSG::VER);
CMiniRecordInfo CDataManager::CLUSTER_SYS_MSG_INFO = CMiniRecordInfo(CLUSTER_SYS_MSG_VER1,sizeof(CLUSTER_SYS_MSG_VER1)/sizeof(FIELD_INFO),CLUSTER_SYSTEM_MSG::VER);

const char	CDataManager::KEY[16] = "5&1(*ax_aw34,sl";



//Following is Standard TEA algorithm, don't modify, just call it by xTEA algorithm
//Linda added at 2005/06/13
#define TEA_DELTA 0x9E3779B9
#define TEA_SUM 0xE3779B90

void tinyEncrypt ( const unsigned long * plain, const unsigned long * key, unsigned long *crypt, unsigned int power)
{
	unsigned long y,z,a,b,c,d;
	unsigned long sum = 0;
	unsigned int i;
	unsigned int rounds = 1<<power;

	y = plain[0];
	z = plain[1];
	a = key[0];
	b = key[1];
	c = key[2];
	d = key[3];

	for (i = 0; i < rounds; i++) {
		sum += TEA_DELTA;
		y += (z << 4) + a ^ z + sum ^ (z >> 5) + b;
		z += (y << 4) + c ^ y + sum ^ (y >> 5) + d;
	}

	crypt[0] = y;
	crypt[1] = z;
}

void tinyDecrypt ( const unsigned long * crypt, const unsigned long * key, unsigned long *plain, unsigned int power)
{
	unsigned long y,z,a,b,c,d;
	unsigned int rounds = 1<<power;
	unsigned long sum = TEA_DELTA<<power;
	unsigned int i;

	y = crypt[0];
	z = crypt[1];
	a = key[0];
	b = key[1];
	c = key[2];
	d = key[3];

	for (i = 0; i < rounds; i++) {
		z -= (y << 4) + c ^ y + sum ^ (y >> 5) + d;
		y -= (z << 4) + a ^ z + sum ^ (z >> 5) + b;
		sum -= TEA_DELTA;
	}

	plain[0] = y;
	plain[1] = z;
}

/*================================================================ 
* 
* 函 数 名：xTEADecryptWithKey 
** Decrypt the cipher text to plain text with the key

* 参 数： 
* 
* const unsigned long *crypt [IN] : the Cipher text 
* DWORD dwCryptLen[IN]: cipher text length
* const unsigned long *theKey [IN] : the key
* DWORD dwKeyLen[IN]: key length
* unsigned long *plain [[IN,OUT]] : the pointer to plain text(net order byte)
* DWORD * pdwPlainLen[IN,OUT]: Valid plain text length
* 
* 返 回 值：BOOL-	SUCCESS:TRUE
*							Fail:NULL 
* 
* 作 者：刘晓敏 2005/6/17 
* 
================================================================*/ 
/*================================================================ 
* 
* 函 数 名：xTEADecryptWithKey 
** Decrypt the cipher text to plain text with the key

* 参 数： 
* 
* const unsigned long *crypt [IN] : the Cipher text 
* DWORD dwCryptLen[IN]: cipher text length
* const unsigned long *theKey [IN] : the key
* DWORD dwKeyLen[IN]: key length
* unsigned long *plain [[IN,OUT]] : the pointer to plain text(net order byte)
* DWORD * pdwPlainLen[IN,OUT]: Valid plain text length
* 
* 返 回 值：BOOL-	SUCCESS:TRUE
*							Fail:NULL 
* 
* 作 者：刘晓敏 2005/6/17 
* 
================================================================*/ 
BOOL xTEADecryptWithKey(const char *crypt, long crypt_len, char key[16], char *plain, long * plain_len)
{
	const unsigned long *tkey   = (const unsigned long *)key;
	const unsigned long *tcrypt = (const unsigned long *)crypt;

	if( crypt_len<1 || crypt_len%8 )
	{
		TRACE("xTEADecryptWithKey Failed! crypt_len %d\r\n",crypt_len);
		return FALSE;
	}

	try
	{
		long *tplain = new long[crypt_len/4+1];

		unsigned int  length = crypt_len;
		unsigned long pre_plain[2] = {0,0};
		unsigned long p_buf[2];
		unsigned long c_buf[2];

		int padLength;
		int i;

		//Decrypt the first 8 bytes(64 bits)
		tinyDecrypt(tcrypt, tkey, p_buf, 4);

		memcpy(pre_plain, p_buf, 8);
		memcpy(tplain, p_buf, 8);

		//Decrype with TEA and interlace algorithm 
		for (i = 2; i < (int)length/4; i+=2) {
			c_buf[0] = *(tcrypt+i) ^ pre_plain[0];
			c_buf[1] = *(tcrypt+i+1) ^ pre_plain[1];
			tinyDecrypt((const unsigned long *)c_buf, tkey, p_buf, 4);
			memcpy(pre_plain, p_buf, 8);
			*(tplain+i) = p_buf[0] ^ *(tcrypt+i-2);
			*(tplain+i+1) = p_buf[1] ^ *(tcrypt+i-1);
		}

		//check the last 7 bytes is 0x00
		if ( tplain[length/4-1] || (tplain[length/4-2]&0xffffff00)) 
		{
			delete[] tplain;
			TRACE("[]tplain\r\n");
			return FALSE;
		}

		padLength = *((unsigned char *)tplain) & 0x07;
		*plain_len = crypt_len - (padLength+3) -7;/*(pad 7 bytes 0x00 at the end)*/
		if(padLength + 3 + *plain_len > (crypt_len/4+1)*sizeof(long))
		{
			delete [] tplain;
			return FALSE;
		}
		//Remove padding data
		memcpy(tplain,(byte*)tplain+padLength+3,*plain_len);
		memcpy(plain,tplain,*plain_len);

		delete [] tplain;

		return TRUE;
	}
	catch(...)
	{
		TRACE("xTEADecryptWithKey Exception\r\n");
		return FALSE;
	}	
}
/*================================================================ 
* 
* 函 数 名：xTEAEncryptWithKey 
** Encrypt the plain text to cipher text with the key

* 参 数： 
* 
* const unsigned long *plain [IN] : the plain text 
* DWORD dwPlainLen[IN]: plain text length
* const unsigned long *theKey [IN] : the key
* DWORD dwKeyLen[IN]: key length
* unsigned long *crypt [[IN,OUT]] : the pointer to cipher text(net order byte)
* DWORD * pdwCryptLen[IN,OUT]: Valid cipher text length
* 
* 返 回 值：BOOL-	SUCCESS:TRUE
*							Fail:NULL 
* 
* 作 者：刘晓敏 2005/6/17 
* 
================================================================*/ 

BOOL xTEAEncryptWithKey(const char *plain, long plain_len, char key[16], char *crypt, long * crypt_len )
{
	const unsigned char pad[9] = {0xad,0xad,0xad,0xad,0xad,0xad,0xad,0xad,0xad};

	unsigned long *tkey = (unsigned long *)key;
	unsigned long *tplain = (unsigned long *)plain;

	if ( plain_len<1 )
	{
		return FALSE;
	}

	unsigned long pre_plain[2] = {0,0};
	unsigned long pre_crypt[2] = {0,0};
	unsigned long p_buf[2];
	unsigned long c_buf[2];

	int padLength;
	int i;

	// padding data
	padLength = (plain_len+10)%8;//at least pad 2 bytes
	padLength = padLength ? 8 - padLength : 0;//total pad length -2

	long length = padLength+3+plain_len+7;
	*crypt_len = length;

	long *tcrypt = new long[length/4];

	*((unsigned char*)tcrypt) = 0xa8 | (unsigned char)padLength;//first pad byte: total padding bytes - 2 or 0xa8 
	memcpy ( (byte*)tcrypt+1, (byte*)pad, padLength+2);//add other padding data
	memcpy ( (byte*)tcrypt+padLength+3, (byte*)tplain, plain_len);//add plain data
	memset ( (byte*)tcrypt+padLength+3+plain_len, 0, 7);  //pad 7 0x00 at the end      

	//Interlace algorithm(交织算法)
	for (i = 0; i < length/4; i+=2) {
		p_buf[0] = *(tcrypt+i) ^ pre_crypt[0];
		p_buf[1] = *(tcrypt+i+1) ^ pre_crypt[1];
		tinyEncrypt( p_buf, tkey, c_buf, 4);
		*(tcrypt+i) = c_buf[0] ^ pre_plain[0];
		*(tcrypt+i+1) = c_buf[1] ^ pre_plain[1];
		memcpy(pre_crypt, tcrypt+i, 8);
		memcpy(pre_plain, p_buf, 8);
	}

	memcpy(crypt,tcrypt,length);

	delete []tcrypt;

	return TRUE;
}

void CDataManager::FlushAllMsg()
{
	MapMsgDbIt it,itE;
	it = m_dbUser.begin();
	itE = m_dbUser.end();
	for(;it != itE;it++)
	{
		delete it->second;
	}
	it = m_dbCluster.begin();
	itE = m_dbCluster.end();
	for(;it != itE;it++)
	{
		delete it->second;
	}
	m_dbUser.clear();
	m_dbCluster.clear();
}

CDataManager::CDataManager(void)
{
	
	m_pMsg = NULL;//MSG_INFO.NewRecord();
	m_pSysMsg = NULL;//SYS_MSG_INFO.NewRecord();
	m_pClusterSysMsg = NULL;
}

CDataManager::~CDataManager(void)
{
	if(m_pMsg != NULL)
	{
		delete m_pMsg;
		m_pMsg = NULL;
	}
	if(m_pSysMsg != NULL)
	{
		delete m_pSysMsg;
		m_pSysMsg = NULL;
	}
	if(m_pClusterSysMsg != NULL)
	{
		delete m_pClusterSysMsg;
		m_pClusterSysMsg = NULL;
	}
	MapMsgDbIt it,itE;
	it = m_dbUser.begin();
	itE = m_dbUser.end();
	for(;it != itE;it++)
	{
		delete it->second;
	}
	it = m_dbCluster.begin();
	itE = m_dbCluster.end();
	for(;it != itE;it++)
	{
		delete it->second;
	}
	m_dbUser.clear();
	m_dbCluster.clear();
}


bool CDataManager::GetMsg(const CMiniRecord * pRecord, XT_MSG & msg)
{
	static char szMsgData[2048];
	long nPlain = 0;				// 解密后数据长度
	size_t nCrypt = 0;				// 加密内容的长度
	const uint8 * pCryptData = NULL;// 加密的聊天内容

	msg.msg_type		= pRecord->m_fields[0];
	msg.cluster_id		= pRecord->m_fields[1];
	msg.cluster_type	= pRecord->m_fields[2];
	msg.recv_flag		= pRecord->m_fields[3];
	msg.from_id			= pRecord->m_fields[4];
	strxcpy(msg.from_nickname,(const char *)pRecord->m_fields[5],sizeof(msg.from_nickname));
	msg.to_id			= pRecord->m_fields[6];
	msg.ver				= pRecord->m_fields[7];
	msg.dest_ip			= pRecord->m_fields[8];
	msg.dest_port		= pRecord->m_fields[9];
	msg.send_time		= pRecord->m_fields[10];
	msg.face			= pRecord->m_fields[11];
	msg.charset			= pRecord->m_fields[12];
	strxcpy(msg.fontName,(const char *)pRecord->m_fields[13],sizeof(msg.fontName));
	msg.fontStyle		= pRecord->m_fields[14];
	msg.fontSize		= pRecord->m_fields[15];
	msg.fontColor		= pRecord->m_fields[16];
	msg.data_type		= pRecord->m_fields[17];
	// 获取加密的聊天内容
	pCryptData = pRecord->m_fields[18].GetData(nCrypt);
	// 解密内容
	if(xTEADecryptWithKey((const char *)pCryptData,(long)nCrypt,(char *)KEY,msg.data,&nPlain) == false)
		return false;
	if(nPlain >= sizeof(msg.data))
	{
		assert(0 && "聊天记录中的信息长度超长！");
		nPlain = sizeof(msg.data) - 1;
	}
	msg.data[nPlain] = 0;
	msg.data_len		= pRecord->m_fields[19];
	return true;
}



bool CDataManager::GetLastMsg(MSG_LIST & msgs,uint32 id,MSG_TYPE type,uint32 nCount)
{
	if(msgs.GetSize()>0)
	{
		msgs.RemoveAll();
	}
	if(m_pMsg == NULL)
	{
		m_pMsg = MSG_INFO.NewRecord();
	}

	STATUS s = OK;
	CMiniDataFile * pF = FindDataFile(id,type);
	if(pF == NULL)
		pF = CreateDataFile(id,type);
	if(pF == NULL)
		return false;
	pF->SeekFromEnd(nCount);

	XT_MSG msg;

	while(s == OK)
	{
		s = pF->GetRecord(m_pMsg);
		if(s == OK)
		{
			if(GetMsg(m_pMsg,msg))
				msgs.AddTail(msg);
		}
	}
	return true;
}

bool CDataManager::GetAllSysMsg(SYSTEM_MSG_LIST & sml,uint32 uid)
{
	if(m_pSysMsg == NULL)
	{
		m_pSysMsg = SYS_MSG_INFO.NewRecord();
	}

	sml.RemoveAll();
	CString strMsgFile = _T("");
   	strMsgFile.Format("%s\\%s\\sd%u.msg",m_sAppDir,m_sCurUser,uid);

	STATUS s;
	CMiniDataFile f;
	SYSTEM_MSG msg;
	s = f.Open(strMsgFile);
	while(s == OK)
	{
		s = f.GetRecord(m_pSysMsg);
		if(s == OK)
		{
			msg.iType = m_pSysMsg->m_fields[0];
			msg.dwTime = (uint32)m_pSysMsg->m_fields[1];
			strxcpy(msg.szMessage,(const char *)m_pSysMsg->m_fields[2],sizeof(msg.szMessage));
			sml.AddTail(msg);
		}
	}
	
	
	return true;
}

bool CDataManager::GetAllMsg(MSG_LIST & msgs,uint32 id,MSG_TYPE type)
{
	msgs.RemoveAll();
	if(m_pMsg == NULL)
	{
		m_pMsg = MSG_INFO.NewRecord();
	}
	
	STATUS s = OK;
	CMiniDataFile * pF = FindDataFile(id,type);
	if(pF == NULL)
		pF = CreateDataFile(id,type);
	if(pF == NULL)
		return false;

	XT_MSG msg;
	pF->SeekToBeg();

	while(s == OK)
	{
		s = pF->GetRecord(m_pMsg);
		if(s == OK)
		{
			if(GetMsg(m_pMsg,msg))
				msgs.AddTail(msg);
		}
	}
	// 如果文件格式错误，则保存到新的文件中
	if(s == FILE_FORMAT_ERR || s == RECORD_FORMAT_ERR)
	{
		ShowMessage("聊天记录文件出现错误，正在恢复中...");
		// 保存最近的聊天记录
		MSG_LIST msgLast;
		GetLastMsg(msgLast,id,type,1000);

		CString sName = pF->GetFileName();
		CloseDataFile(id,type);
		DeleteFile(sName);
		pF = CreateDataFile(id,type);
		POSITION it;
		int c = msgs.GetCount();
		it = msgs.GetHeadPosition();
		int i;
		for(i = 0; i < c; i++)
		{
			SaveMsg(msgs.GetNext(it),id);
		}
		
		c = msgLast.GetCount();
		it = msgLast.GetHeadPosition();
		XT_MSG  msg;
		for(i = 0; i < c; i++)
		{
			msg = msgLast.GetNext(it);
			msgs.AddTail(msg);
			SaveMsg(msg,id);
		}
		ShowMessage("聊天记录文件恢复成功");
	}
	return true;
}


bool CDataManager::GetClusterSysMsg(CLUSTER_SYSTEM_MSG_LIST & sml,uint32 uid)
{
	if(m_pClusterSysMsg == NULL)
	{
		m_pClusterSysMsg = CLUSTER_SYS_MSG_INFO.NewRecord();
	}

	sml.RemoveAll();
	CString strMsgFile = _T("");
	strMsgFile.Format("%s\\%s\\sdc%u.msg",m_sAppDir,m_sCurUser,uid);

	STATUS s;
	CMiniDataFile f;
	CLUSTER_SYSTEM_MSG msg;
	s = f.Open(strMsgFile);
	while(s == OK)
	{
		s = f.GetRecord(m_pClusterSysMsg);
		if(s == OK)
		{
			msg.iType = m_pClusterSysMsg->m_fields[0];
			msg.dwTime = (uint32)m_pClusterSysMsg->m_fields[1];
			strxcpy(msg.szMessage,(const char *)m_pClusterSysMsg->m_fields[2],sizeof(msg.szMessage));
			msg.nInviteId = m_pClusterSysMsg->m_fields[3];
			strxcpy(msg.szNickName,(const char *)m_pClusterSysMsg->m_fields[4],sizeof(msg.szNickName));
			msg.nClusterId = m_pClusterSysMsg->m_fields[5];
			strxcpy(msg.szClusterName,(const char *)m_pClusterSysMsg->m_fields[6],sizeof(msg.szClusterName));
			sml.AddTail(msg);
		}
	}


	return true;
}

bool CDataManager::DeleteClusterTalkFile(uint32 cid)
{
	
	MapMsgDbIt it;
	it = m_dbCluster.find(cid);
	if(it != m_dbCluster.end())
	{
		it->second->Close();
		delete it->second;
		m_dbCluster.erase(it);
	}
	CString str;
	str.Format("%s\\%s\\cd%u.msg",m_sAppDir,m_sCurUser,cid);
	return DeleteFile(str);
}

bool CDataManager::DeleteUserTalkFile(uint32 uid)
{
	MapMsgDbIt it;
	it = m_dbUser.find(uid);
	if(it != m_dbUser.end())
	{
		it->second->Close();
		delete it->second;
		m_dbUser.erase(it);
	}
	CString str;
	str.Format("%s\\%s\\fd%u.msg",m_sAppDir,m_sCurUser,uid);
	return DeleteFile(str);
}

CMiniDataFile * CDataManager::CreateDataFile(uint32 id,int type)
{
	CMiniDataFile * pF;
	CString strUserFolder;
	CString strFile;
	
	strUserFolder.Format("%s\\%s",m_sAppDir,m_sCurUser);
	if ( _access(strUserFolder,0)==-1 )
	{
		::CreateDirectory(strUserFolder,NULL);
	}
	if (type == USER)
	{	
		strFile.Format("%s\\fd%u.msg",strUserFolder,id);
	}
	else if (type == CLUSTER)
	{
		strFile.Format("%s\\cd%u.msg",strUserFolder,id);
	}
	else
	{
		ASSERT(0);
	}
	
	// 建立数据文件
	STATUS s;
	pF = new CMiniDataFile();
	s  = pF->Open(strFile,true);
	if(s != OK)
	{
		delete pF;
		return NULL;
	}
	if(type == USER)
	{
		m_dbUser[id] = pF;
	}
	else if(type == CLUSTER)
	{
		m_dbCluster[id] = pF;
	}
	// 保存当前的版本定义信息
	s = pF->SaveVersion(MSG_INFO);
	
	
	return pF;
}


//#define PERM_FAMILY				1

bool CDataManager::SaveSysMsg(uint32 uid,const SYSTEM_MSG & msg)
{
	if(m_pSysMsg == NULL)
	{
		m_pSysMsg = SYS_MSG_INFO.NewRecord();
	}
	
	CString strUserFolder;
	strUserFolder.Format("%s\\%s",m_sAppDir,m_sCurUser);
	if ( _access(strUserFolder,0) == -1 )
	{
		CreateDirectory(strUserFolder,NULL);
	}

	STATUS s = OK;	
	CString strFile = _T("");
	strFile.Format("%s\\%s\\sd%u.msg",m_sAppDir,m_sCurUser,uid);
	
	CMiniDataFile f;
	s = f.Open(strFile);
	if(s != OK)
		return false;
	s = f.SaveVersion(SYS_MSG_INFO);
	
	m_pSysMsg->m_fields[0] = msg.iType;
	m_pSysMsg->m_fields[1] = (uint32)msg.dwTime;
	m_pSysMsg->m_fields[2] = msg.szMessage;

	
	s = f.SaveRecord(m_pSysMsg);
	return s == OK;
}

bool CDataManager::SaveClusterSysMsg(uint32 uid,const CLUSTER_SYSTEM_MSG & msg)
{
	if(m_pClusterSysMsg == NULL)
	{
		m_pClusterSysMsg = CLUSTER_SYS_MSG_INFO.NewRecord();
	}

	CString strUserFolder;
	strUserFolder.Format("%s\\%s",m_sAppDir,m_sCurUser);
	if ( _access(strUserFolder,0) == -1 )
	{
		CreateDirectory(strUserFolder,NULL);
	}

	STATUS s = OK;	
	CString strFile = _T("");
	strFile.Format("%s\\%s\\sdc%u.msg",m_sAppDir,m_sCurUser,uid);

	CMiniDataFile f;
	s = f.Open(strFile);
	if(s != OK)
		return false;
	s = f.SaveVersion(CLUSTER_SYS_MSG_INFO);

	m_pClusterSysMsg->m_fields[0] = msg.iType;
	m_pClusterSysMsg->m_fields[1] = (uint32)msg.dwTime;
	m_pClusterSysMsg->m_fields[2] = msg.szMessage;
	m_pClusterSysMsg->m_fields[3] = msg.nInviteId;
	m_pClusterSysMsg->m_fields[4] = msg.szNickName;
	m_pClusterSysMsg->m_fields[5] = msg.nClusterId;
	m_pClusterSysMsg->m_fields[6] = msg.szClusterName;

	s = f.SaveRecord(m_pClusterSysMsg);
	return s == OK;
}

//
//bool CDataManager::SaveFriendList(const FRIEND_LIST & fl)
//{
//	CString strUserFolder;
//	strUserFolder.Format("%sdata\\%s",m_strPath,g_ImUserInfo.GetUserName().c_str());
//
//	if ( _access(strUserFolder,0)==-1 )
//	{
//		CreateDirectory(strUserFolder,NULL);
//	}
//
//	CString strFile;
//	strFile.Format("%sdata\\%s\\fl.db",m_strPath,g_ImUserInfo.GetUserName().c_str());
//	DeleteFile(strFile);
//
//	CStdioFile file;
//	char buff[2048];
//	long buff_len;
//
//	if ( file.Open(strFile,CFile::modeCreate|CFile::modeWrite|CFile::typeBinary) )
//	{
//		for ( it=fl.begin(); it!=fl.end(); it++ )
//		{
//			xTEAEncryptWithKey((char *)(*it),sizeof(FRIEND_INFO),
//				(char *)key,
//				buff,&buff_len);
//
//			file.Write(buff,buff_len);			
//		}
//		file.Close();
//	}
//	return true;
//}

bool CDataManager::SaveMsg(const XT_MSG & msg,uint32 fid)
{
	if(m_pMsg == NULL)
	{
		m_pMsg = MSG_INFO.NewRecord();
	}
	ASSERT(m_pMsg);
	MSG_TYPE type = USER;
	uint32 id = fid;
	if (msg.cluster_type == PERM_FAMILY)
	{
		type = CLUSTER;
		id = msg.cluster_id;
	}

	CMiniDataFile * pF = FindDataFile(id,type);
	if(pF == NULL)
	{
		pF = CreateDataFile(id,type);
		if(pF == NULL)
		{
			return false;
		}
	}
	int nPlain = (int)strlen(msg.data);		// 加密前的长度
	if(nPlain > sizeof(msg.data))
	{
		assert(0 && "保存的聊天消息长度过长");
		nPlain = sizeof(msg.data);
	}
	static char szCryptMsgData[2048];	// 加密后的数据
	long nCrypt = 0;						// 加密后的长度
	if(nPlain == 0)
	{
		nPlain = 1;
	}
	// 加密聊天内容
	if(xTEAEncryptWithKey(msg.data,nPlain,(char *)KEY,szCryptMsgData,&nCrypt) == false)
	{
		// 加密失败
		return false;;
	}

	

	m_pMsg->m_fields[0] = msg.msg_type;
	m_pMsg->m_fields[1] = msg.cluster_id;
	m_pMsg->m_fields[2] = msg.cluster_type;
	m_pMsg->m_fields[3] = msg.recv_flag;
	m_pMsg->m_fields[4] = msg.from_id;
	m_pMsg->m_fields[5] = msg.from_nickname;
	m_pMsg->m_fields[6] = msg.to_id;
	m_pMsg->m_fields[7] = msg.ver;
	m_pMsg->m_fields[8] = msg.dest_ip;
	m_pMsg->m_fields[9] = msg.dest_port;
	m_pMsg->m_fields[10] = msg.send_time;
	m_pMsg->m_fields[11] = msg.face;
	m_pMsg->m_fields[12] = msg.charset;
	m_pMsg->m_fields[13] = msg.fontName;
	m_pMsg->m_fields[14] = msg.fontStyle;
	m_pMsg->m_fields[15] = msg.fontSize;
	m_pMsg->m_fields[16] = msg.fontColor;
	m_pMsg->m_fields[17] = msg.data_type;
	// 保存加密后的聊天内容
	m_pMsg->m_fields[18].SetData((const uint8*)szCryptMsgData,nCrypt);
	m_pMsg->m_fields[19] = msg.data_len;

	return pF->SaveRecord(m_pMsg) == OK;
}

_MINIDATA_END