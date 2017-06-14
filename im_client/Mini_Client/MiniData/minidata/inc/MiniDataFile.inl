#pragma  once


inline const CString &	CMiniDataFile::GetFileName() const
{ return m_sFile; };



inline STATUS	CMiniDataFile::SaveVersion(const FIELD_INFO * pAttrs,int nAttrs,int nVer)
{ return SaveVersion(CMiniRecordInfo(pAttrs,nAttrs,nVer)); };


inline void CMiniDataFile::SeekToBeg()
{ 
	m_npRecord = m_npBeg;
	if(m_bOpen)
		m_reader.Seek(m_npRecord,CFile::begin);
};


inline STATUS CMiniDataFile::SkipRecordInfo()
{
	uint32 nLen = 0;
	m_reader>>nLen;
	if(nLen < CMiniRecordInfo::FIXED_SIZE)
		return RECORDINF_FORMAT_ERR;
	else
		m_reader.Seek(ULONGLONG(nLen - 8),CFile::current);
	// ¼ÇÂ¼Î»ÖÃÒÆ¶¯
	m_npRecord += nLen;
	return OK;
};


inline uint32	CMiniDBHeader::GetLen() const
{ return FIXED_SIZE + m_vReserveData.nLen; };

