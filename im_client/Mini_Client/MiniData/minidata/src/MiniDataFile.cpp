#include "StdAfx.h"
#include "../inc/MiniDataFile.h"

#include <io.h>

_MINIDATA_BEG

#define _TRY_	try{
#define _CATCH_(e)	}catch(e)


const uint32 CMiniDBHeader::ID = 0x494e494d; //	"MINI";

CMiniDBHeader::CMiniDBHeader()
{
	m_nLen = 0;
	m_nLen2 = 0;
	m_nReserve1 = 0;
	m_nReserve2 = 0;
	m_nVer = 0;
}

STATUS CMiniDBHeader::Read(CMiniReader & r)
{
	uint32 nID = 0;
	r.Read(nID);
	if(nID != ID)
	{
		return FILEHEAD_FORMAT_ERR;
	}
	r>>m_nLen;
	if(MIN_SIZE > m_nLen)
	{
		return FILEHEAD_FORMAT_ERR;
	}
	r>>m_nVer
	 >>m_nReserve1
	 >>m_nReserve2
	 >>m_vReserveData;
	if(r.Read(m_nLen2) == false)
		return READ_EOF;
	if(m_nLen != m_nLen2)
	{
		return FILEHEAD_FORMAT_ERR;
	}
	if(m_nLen != (FIXED_SIZE + m_vReserveData.nLen))
	{
		return FILEHEAD_FORMAT_ERR;
	}
	
	return OK;
}

STATUS CMiniDBHeader::Save(CMiniWriter & w)
{
	m_nLen = FIXED_SIZE + m_vReserveData.nLen;
	m_nLen2 = m_nLen;
	w.ClearBuf();
	w<<ID
	 <<m_nLen
	 <<m_nVer
	 <<m_nReserve1
	 <<m_nReserve2
	 <<m_vReserveData
	 <<m_nLen2;
	w.Save();
	return OK;
}



CMiniDataFile::CMiniDataFile(void)
{
	m_bOpen = false;
}

CMiniDataFile::~CMiniDataFile(void)
{
	Close();
}

STATUS CMiniDataFile::SaveRecord(CMiniRecord * pRecord)
{
	static RecordIt it;
	STATUS s = OK;
	if(m_bOpen == false)
		return FILE_NOT_OPEN;
	assert(pRecord);
	it = m_recordTemplate.find(pRecord->GetVer());
	if(it == m_recordTemplate.end())
		return RECORD_VER_NOT_SUPPORT;
	m_file.SeekToEnd();
	return pRecord->Save(m_writer);
}

bool CMiniDataFile::FindVersion(const CMiniRecordInfo & info)
{
	size_t c = m_infs.size();
	size_t i;
	for(i = 0;i < c;i++)
	{
		if(m_infs[i] == info)
			return true;
	}
	return false;
}

STATUS CMiniDataFile::SaveVersion(CMiniRecordInfo & info)
{
	if(m_bOpen == false)
		return FILE_NOT_OPEN;
	STATUS s = OK;
	uint32 nVer = info.GetVer();
	size_t c = m_infs.size();
	size_t i;
	for(i = 0;i < c;i++)
	{
		if(m_infs[i].GetVer() == nVer)
		{
			return SAVE_RECORDINFO_EXIST;
		}
	}
	info.m_nNext = 0;
	info.m_nPos = (uint32)m_file.GetLength();

	if(c > 0)
	{
		// 更新链接
		m_infs[c-1].m_nNext = info.m_nPos;
		m_file.Seek(m_infs[c - 1].m_nPos + 12,CFile::begin);
		m_file.Write(&info.m_nPos,4);
	}

	

	m_file.SeekToEnd();
	s = info.Save(m_writer);
	// 建立版本映射
	c = m_infs.size();
	FIELD_MAP fm;
	VER_PAIR vp;
	uint32 nVerTemp;
	for(i = 0; i < c;i++)
	{
		vp.v1 = info.GetVer();
		vp.v2 = m_infs[i].GetVer();
		info.GetFieldMap(m_infs[i],fm);
		m_mapVer[vp] = fm;
		nVerTemp = vp.v1;
		vp.v1 = vp.v2;
		vp.v2 = nVerTemp;
		m_infs[i].GetFieldMap(info,fm);
		m_mapVer[vp] = fm;
	}

	m_recordTemplate[info.GetVer()] = info.NewRecord();
	m_infs.push_back(info);
	return s;
}

STATUS CMiniDataFile::SeekFromEnd(uint32 n)
{
	if(m_bOpen == false)
		return FILE_NOT_OPEN;
	m_reader.Seek(0,CFile::end);
	if(n == 0)
	{
		return OK;
	}
	m_file.Seek(-4,CFile::current);
	uint32 nID = 0;
	int32 nLen = 0;
	int32 nLen2 = 0;
	ULONGLONG nFileSize = m_file.GetLength();
	_TRY_
	while(n)
	{
		nLen = 0;
		nLen2 = 0;
		nID = 0;
		// 移动到记录尾部
		if(m_file.Read(&nLen,4) != 4)
			return FILE_FORMAT_ERR;
		if(nLen > nFileSize)
			return FILE_FORMAT_ERR;
		// 移动到记录开头
		m_file.Seek(-nLen,CFile::current);
		m_file.Read(&nID,4);
		m_file.Read(&nLen2,4);
		if(nLen != nLen2)
		{
			return DATA_LEN_NOT_EQUAL;
		}
		if(nID == CMiniRecord::ID)
		{
			n--;
			if(n)
				m_file.Seek(-12,CFile::current);
			else
				m_file.Seek(-8,CFile::current);
			continue;
		}
		else if(nID == CMiniRecordInfo::ID)
		{
			m_file.Seek(-12,CFile::current);
			continue;
		}
		else if(nID == CMiniDBHeader::ID)
		{
			// 移动回记录开始
			m_file.Seek(nLen - 8,CFile::current);
			return OK;
		}
	}
	_CATCH_(CException * pE)
	{
		pE->Delete();
		return FILE_EXCEPTION;
	}
	return OK;
}

STATUS CMiniDataFile::GetRecord(CMiniRecord * pRecord)
{
	if(m_bOpen == false)
	{
		return FILE_NOT_OPEN;
	}
	static uint32 nID;
	static uint32 nLen1;
	static uint32 nVer;
	static uint32 nLen2;

	STATUS s = OK;
	nID = 0;
	nLen1 = 0;
	nVer = 0;
	nLen2 = 0;


	//m_reader.Seek(m_npRecord,CFile::begin);

	while(true)
	{
		if(m_reader.Read(nID) == false)
			return FILE_EOF;
		if(nID == CMiniRecord::ID)
		{
			break;
		}
		else if(nID == CMiniRecordInfo::ID)
		{
			s = SkipRecordInfo();
			if(s  != OK)
				return s;
			nID = 0;
		}
		else
			return RECORD_FORMAT_ERR;
	}
	
	
	m_reader>>nLen1;
	if(m_reader.Read((uint8*)&nVer,4) == false)
		return READ_EOF;

	// 根据版本获取模板
	RecordIt it = m_recordTemplate.find(nVer);
	// 当前版本没有模板则为无效记录
	if(it == m_recordTemplate.end())
		return RECORD_UNKNOWN_VER;
	std::vector<CMiniVar> & fields = it->second->m_fields;
	size_t n = fields.size();
	size_t i;

	// 读取记录
	for(i = 0;i < n;i++)
	{
		s = fields[i].Read(m_reader);
		if(s != OK)
			return s;
	}
	if(m_reader.Read(nLen2) == false)
		return READ_EOF;
	if(nLen1 != nLen2)
		return RECORD_FORMAT_ERR;
	
	m_npRecord += nLen1;

	Convert(it->second,pRecord );
	return OK;
}

void CMiniDataFile::Close()
{ 
	if(m_bOpen)
		m_file.Close();
	m_bOpen = false;
	m_infs.clear();
	RecordIt it,itE;
	it = m_recordTemplate.begin();
	itE = m_recordTemplate.end();
	CMiniRecord * pRecord;
	for(;it != itE;it++)
	{
		pRecord = it->second;
		if(pRecord)
			delete pRecord;
	}
	m_recordTemplate.clear();
	m_sFile = "";
	m_reader.Clear();
	m_writer.Clear();
	m_npBeg = m_npRecord = 0;
};

STATUS CMiniDataFile::Open(const char * szFile,bool bCreate /* = true */)
{
	STATUS s;

	Close();

	int nFlag = CFile::modeReadWrite|CFile::typeBinary;
	bool bExist = true;
	if(_access(szFile,0) == -1)
	{
		if(bCreate == false)
		{
			return FILE_NOT_EXIST;
		}
		else
		{
			bExist = false;
			nFlag |= CFile::modeCreate;
		}
	}
	

	if(m_file.Open(szFile,nFlag) == FALSE)
	{
		return FILE_OPEN_FAILED;
	}

	m_reader.SetFile(&m_file);
	m_writer.SetFile(&m_file);

	// 首先,文件存在时验证文件格式,文件不存在时写入文件头
	ULONGLONG nLen = 0;
	if(bExist)
	{
		nLen = m_file.GetLength();

		if(nLen < CMiniDBHeader::MIN_SIZE)
			return FILEHEAD_FORMAT_ERR;
		
		s = m_header.Read(m_reader);
		if(s != OK)
			return s;
		
		// 文件中只有头信息
		if(nLen == m_header.m_nLen)
		{
			m_sFile = szFile;
			m_bOpen = true;
			return OK;
		}
		// 然后开始读取记录结构定义信息
		CMiniRecordInfo rinf;
		ULONGLONG nPos;
		ULONGLONG nFirstRecord;
		nFirstRecord = m_header.m_nLen;
		// 获取第一个记录结构定义信息
		s = rinf.Read(m_reader);
		if(s != OK)
			return s;
#ifdef _TEST
		rinf.Print();
#endif 
		// 记录在文件中的位置
		rinf.m_nPos = m_header.m_nLen;
		m_infs.push_back(rinf);
		nFirstRecord += rinf.m_nLen;
		nPos = rinf.GetNext();
		while(nPos != 0)
		{
			m_reader.Seek(nPos,CFile::begin);
			s = rinf.Read(m_reader);
			if(s != OK)
				return s;
#ifdef _TEST
			rinf.Print();
#endif 
			if(nPos == nFirstRecord)
				nFirstRecord += rinf.m_nLen;
			rinf.m_nPos = (uint32)nPos;
			m_infs.push_back(rinf);
			nPos = rinf.GetNext();
		}

		// 保存记录开始位置
		m_npBeg = m_npRecord = (uint32)nFirstRecord;

		// 初始化记录版本模板,并建立版本映射
		size_t i,j;
		size_t c;
		c = m_infs.size();
		FIELD_MAP fm;
		VER_PAIR vp;
		uint32 nVerTemp;
		for(i = 0;i < c;i++)
		{
			CMiniRecordInfo & inf = m_infs[i];
			m_recordTemplate[inf.GetVer()] = inf.NewRecord();
			for(j = 0; j < i;j++)
			{
				vp.v1 = inf.GetVer();
				vp.v2 = m_infs[j].GetVer();
				inf.GetFieldMap(m_infs[j],fm);
				m_mapVer[vp] = fm;
				nVerTemp = vp.v1;
				vp.v1 = vp.v2;
				vp.v2 = nVerTemp;
				m_infs[j].GetFieldMap(inf,fm);
				m_mapVer[vp] = fm;
			}	
		}
	}
	else
	{
		// 写入一个空文件头
		m_header.Save(m_writer);
		// 保存记录开始位置
		m_npBeg = m_npRecord = m_header.GetLen();

	}
	m_bOpen = true;
	// 移动到第一条记录处
	SeekToBeg();
	m_sFile = szFile;
	return OK;
}

void CMiniDataFile::Convert(const CMiniRecord * pSrc,CMiniRecord * pDst)
{
	VER_PAIR vp;
	vp.v1 = pDst->GetVer();
	vp.v2 = pSrc->GetVer();
	if(vp.v1 == vp.v2)
	{
		*pDst = *pSrc;
	}
	else
	{
		VerPairIt it = m_mapVer.find(vp);
		size_t i;
		int nPos = 0;
		size_t c = pDst->m_fields.size();
		if(it == m_mapVer.end())
		{
			for(i = 0;i < c;i++)
			{
				pDst->m_fields[i] = (uint32)0;
			}
		}
		else
		{
			for(i = 0;i < c;i++)
			{
				nPos  = it->second[i];
				if(nPos == -1)
					pDst->m_fields[i] = (uint32)0;
				else
					pDst->m_fields[i] = pSrc->m_fields[nPos];
			}
		}
	}
}

//STATUS CMiniDataFile::Seek(int n,SEEK_POS pos /* = CUR */)
//{
//
//	return OK;
//}



_MINIDATA_END