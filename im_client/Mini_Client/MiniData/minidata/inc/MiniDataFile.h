// ----------------------------------------------------------------
// �ļ�:MiniDataFile.h
// �汾: 
// ����: xy
// ����: 2006-11-23
// ----------------------------------------------------------------
// �汾��ʷ			ע��
// ----------------------------------------------------------------
// v0.1				��ʼ�汾
// 
// ----------------------------------------------------------------

#pragma once

#include "minidata.h"
#include "MiniRecord.h"
#include <map>



#define _MINIDATA_BEG	namespace mini_data{
#define _MINIDATA_END	}

//------------------------------------------------------
// Mini139ʹ�õ����ݶ�д��
//------------------------------------------------------

_MINIDATA_BEG


const uint32 CUR_MINIDB_VERSION = 0x00000001;


class CMiniDataFile;

//------------------------------------------------------
// �����ļ�ͷ
//------------------------------------------------------


class CMiniDBHeader
{
protected:

	friend class CMiniDataFile;

	CMiniDBHeader();

public:

	
	STATUS				Read(CMiniReader & r);

	STATUS				Save(CMiniWriter & w);

	uint32				GetLen() const;

public:

	const static uint32	MIN_SIZE = 28;
	const static uint32 FIXED_SIZE = 28;
	const static uint32	ID;

	uint32				m_nVer;
	uint32				m_nLen;
	uint32				m_nReserve1;
	uint32				m_nReserve2;
	CMiniData			m_vReserveData;
	uint32				m_nLen2;
};


//------------------------------------------------------
// ���ݿ������
//------------------------------------------------------

class MINI_API CMiniDataFile
{
public:

	CMiniDataFile(void);

	~CMiniDataFile(void);

	enum SEEK_POS{
		CUR = CFile::current,	// ��ǰλ��
		BEG = CFile::begin,		// ��ʼλ��
		END	= CFile::end		// ���λ��
	};

	

public:

	//------------------------------------------------------
	// �򿪼�¼�ļ�,����ERRORS��Ϣ
	//------------------------------------------------------
	STATUS			Open(const char * szFile,bool bCreate = true);

	//------------------------------------------------------
	// �رռ�¼�ļ�
	//------------------------------------------------------
	void			Close();

	//------------------------------------------------------
	// ����ָ���汾�ļ�¼�ṹ�����ݿ����Ƿ����
	//------------------------------------------------------
	bool			FindVersion(const CMiniRecordInfo &	info);

	//------------------------------------------------------
	// ����汾��¼,������������򷵻�ERRORS��Ϣ
	//------------------------------------------------------
	STATUS			SaveVersion(CMiniRecordInfo & inf);

	//------------------------------------------------------
	// ����汾��¼,������������򷵻�ERRORS��Ϣ
	//------------------------------------------------------
	STATUS			SaveVersion(const FIELD_INFO * pAttrs,int nAttrs,int nVer);

	//------------------------------------------------------
	// Ѱ�Ҽ�¼,������������򷵻�ERRORS��Ϣ
	//------------------------------------------------------
	//STATUS		Seek(int n,SEEK_POS pos = CUR);
	
	//------------------------------------------------------
	// ��ȡ��¼����,�����¼ָ��,��ָ�����´ε��ñ�����ʱʧЧ
	// ������������򷵻�ERRORS��Ϣ
	//------------------------------------------------------
	STATUS			GetRecord(CMiniRecord * pRecord);	

	//------------------------------------------------------
	// �����¼���ݵ��ļ���
	//------------------------------------------------------
	STATUS			SaveRecord(CMiniRecord * pRecord);

	//------------------------------------------------------
	// ������һ����¼
	//------------------------------------------------------
	void			SeekToBeg();

	//------------------------------------------------------
	// �Ӽ�¼ĩβ��ǰ����n����¼
	//------------------------------------------------------
	STATUS			SeekFromEnd(uint32 n);

	//------------------------------------------------------
	// ��ȡ�ļ���
	//------------------------------------------------------
	const CString &	GetFileName() const;

protected:

	//------------------------------------------------------
	// ��ͬ�汾��¼֮���ת��
	//------------------------------------------------------
	void			Convert(const CMiniRecord * pSrc,CMiniRecord * pDst);

	//------------------------------------------------------
	// ������¼�ṹ��Ϣ��¼��
	//------------------------------------------------------
	STATUS			SkipRecordInfo();

protected:

	typedef std::map<uint32,CMiniRecord*>::iterator RecordIt;
	typedef std::map<VER_PAIR,FIELD_MAP>::iterator	VerPairIt;
	
	bool							m_bOpen;	// �ļ��Ƿ��
	uint32							m_npRecord;	// ��¼�α� 
	uint32							m_npBeg;	// ��¼��ʼλ��

	CString							m_sFile;	// �ļ���
	CMiniDBHeader					m_header;	// �ļ�ͷ
	std::vector<CMiniRecordInfo>	m_infs;		// ��¼�ṹ������Ϣ
	std::map<uint32,CMiniRecord*>	m_recordTemplate;	// ��¼ģ��
	std::map<VER_PAIR,FIELD_MAP>	m_mapVer;	// �汾ӳ��

	CFile							m_file;		// ���ݿ��ļ�����
	CMiniReader						m_reader;	// ������
	CMiniWriter						m_writer;	// д����

};

#ifdef _DLL_EXPORT
#include "MiniDataFile.inl"
#endif

_MINIDATA_END