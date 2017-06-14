#include "StdAfx.h"
#include "../inc/minirecord.h"

#ifdef _TEST
#include <iostream>
#include <string>
using namespace std;
#endif

_MINIDATA_BEG

const uint32 CMiniRecord::ID = 0x44524352;		// "RCRD"
const uint32 CMiniRecordInfo::ID = 0x464e4956;	// "VINF"

#ifdef _TEST

void CMiniRecordInfo::Print() const
{
	size_t i,c;
	string sType;
	CString sRestrict;
	c = m_attrs.size();
	cout<<"=="<<(int)c<<"个属性=="<<endl;
	for(i = 0;i < c; i++)
	{
		switch(m_attrs[i].type)
		{
		case MV_INT8:
			sType = "int8";
			break;
		case MV_INT16:
			sType = "int16";
			break;
		case MV_INT32:
			sType = "int32";
		    break;
		case MV_DATA:
			sType = "mini_str";
			break;
		case MV_INT64:
			sType = "int64";
		    break;
		case MV_FLOAT32:
			sType = "float32";
			break;
		case MV_FLOAT64:
			sType = "float64";
			break;
		default:
			sType = "未知类型";
		    break;
		}
		switch(m_attrs[i].restrict.type)
		{
		case MINI_RESTRICT::EMPTY:
			sRestrict = "无约束";
			break;
		case MINI_RESTRICT::DATA_LEN:
			sRestrict.Format("限制长度%d",m_attrs[i].restrict.val);
		    break;
		case MINI_RESTRICT::VAL_SIGNED:
			sRestrict.Format("限制值%d",(int64)m_attrs[i].restrict.val);
			break;
		case MINI_RESTRICT::VAL_UNSIGNED:
			sRestrict.Format("限制值%d",(uint64)m_attrs[i].restrict.val);
			break;
		default:
			sRestrict = "未知约束条件";
		    break;
		}
		cout<<"["<<(int)i<<"]"<<(LPCTSTR)m_attrs[i].sName<<"("
			<<sType<<","<<(LPCTSTR)sRestrict<<"); ";
	}
	cout<<endl;
	
}

#endif

uint32 CMiniRecordInfo::GetDataLen() const
{
	uint32 nLen = FIXED_SIZE;
	size_t i,c;
	c = m_attrs.size();
	for(i = 0;i < c;i++)
	{
		nLen += m_attrs[i].GetLen();
	}
	return nLen;
}

CMiniRecord * CMiniRecordInfo::NewRecord() const
{
	CMiniRecord * p = new CMiniRecord();
	size_t i,c;
	c = m_attrs.size();
	for(i = 0; i < c;i++)
	{
		 p->m_fields.push_back(CMiniVar(m_attrs[i].type,m_attrs[i].restrict));
	}
	p->m_nVer = m_nVer;
	return p;
}

bool CMiniRecordInfo::operator == (const CMiniRecordInfo & ver) const
{
	if(m_attrs.size() != ver.m_attrs.size())
		return false;
	size_t i;
	size_t c = m_attrs.size();
	for(i = 0;i < c;i++)
	{
		if(m_attrs[i].sName != ver.m_attrs[i].sName)
			return false;
		if(m_attrs[i].type != ver.m_attrs[i].type)
			return false;
		if(m_attrs[i].restrict.type != ver.m_attrs[i].restrict.type ||
		   m_attrs[i].restrict.val != ver.m_attrs[i].restrict.val)
			return false;
	}
	return true;
};

STATUS CMiniRecordInfo::Read(CMiniReader & r)
{
	uint32 nAttrs = 0;
	uint32 i;
	uint32 nID = 0;
	m_nLen = 0;
	if(r.Read(nID) == false)
		return FILE_EOF;
	r>>m_nLen
	 >>m_nVer
	 >>m_nNext;
	if(nID != ID)
		return RECORDINF_FORMAT_ERR;
	if(r.Read(nAttrs) == false)
		return READ_EOF;
	if(nAttrs > FIELD_INFO::MAX_ATTRS)
		return RECORDINF_FORMAT_ERR;
	FIELD_INFO attr;
	m_attrs.clear();
	for(i = 0;i < nAttrs;i++)
	{
		attr.Read(r);
		m_attrs.push_back(attr);
	}
	if(r.Read(m_nLen2) == false)
		return READ_EOF;
	if(m_nLen != m_nLen2)
		return RECORDINF_FORMAT_ERR;
	return OK;
}

STATUS CMiniRecordInfo::Save(CMiniWriter & w) const
{
	w.ClearBuf();
	w<<ID		// 版本
	 <<m_nLen	// 长度
	 <<m_nVer	// 版本
	 <<m_nNext;	// 链接
	uint32 nC = (uint32)m_attrs.size();
	w<<nC;
	uint32 i;
	for(i = 0;i < nC;i++)
	{
		m_attrs[i].Write(w);
	}
	w<<m_nLen2;
	w.Save();
	return OK;
}

void CMiniRecordInfo::GetFieldMap(const CMiniRecordInfo & ver,FIELD_MAP & fm) const
{
	size_t i,j;
	size_t c,c2;
	int32 nMap;
	bool bFind;
	fm.clear();
	c = m_attrs.size();
	c2 = ver.m_attrs.size();
	for(i = 0;i < c;i++)
	{
		const CString & sName = m_attrs[i].sName;
		bFind = false;
		for(j = 0;j < c2;j++)
		{
			if(sName == ver.m_attrs[j].sName)
			{
				nMap = (int32)j;
				bFind = true;
				break;
			}
		}
		if(bFind == false)
		{
			nMap = (int32)-1;
		}
		fm.push_back(nMap);
	}
}



CMiniRecord::CMiniRecord()
{
	m_nVer = 0;
}

CMiniRecord::~CMiniRecord(void)
{

}



#ifdef _TEST

std::ostream & operator << (std::ostream & os,const CMiniVar & v)
{
	switch(v.m_type)
	{

	case MV_INT32:
		os<<v.m_var.i32;
		break;
	case MV_DATA:
		if(v.m_var.data.nLen != 0)
			os<<v.m_var.data.pData;
		else
			os<<"";
		break;
	case MV_INT16:
		os<<v.m_var.i16;
		break;
	case MV_INT8:
		os<<v.m_var.i8;
		break;
	case MV_FLOAT32:
		os<<v.m_var.f32;
		break;
	case MV_FLOAT64:
		os<<v.m_var.f64;
		break;
	case MV_INT64:
		os<<v.m_var.i64;
		break;
	default:
		break;
	};
	return os;
};

void CMiniRecord::Print()
{
	size_t i,c;
	c = m_fields.size();
	cout<<"=record= ver["<<m_nVer<<"],列数["<<(uint32)c<<"]"<<endl;
	for(i = 0;i < c;i++)
	{
		cout<<m_fields[i]<<",";
	}
	cout<<endl;
	
}



#endif


STATUS CMiniRecord::Save(CMiniWriter & w)
{
	STATUS s = OK;
	size_t i;
	size_t c = m_fields.size();
	m_nLen = GetDataLen();
	m_nLen2 = m_nLen;
	w.ClearBuf();
	w<<ID
	 <<m_nLen
	 <<m_nVer;
	// 写数据
	for(i = 0; i < c; i++)
	{
		s = m_fields[i].Write(w);
		if(s != OK)
			return s;
	}
	w<<m_nLen2;
	w.Save();
	return OK;
}

STATUS CMiniRecord::Read(CMiniReader & r)
{
	STATUS s = OK;
	size_t i;
	size_t c = m_fields.size();;
	uint32 nID = 0;
	
	if(r.Read(nID) == false)
		return FILE_EOF;
	if(nID != ID)
		return RECORD_FORMAT_ERR;
	r>>m_nLen;

	for(i = 0; i < c; i++)
	{
		s = m_fields[i].Read(r);
		if(s != OK)
			return s;
	}
	if(r.Read(m_nLen2) == false)
		return READ_EOF;
	return s;
}

uint32 CMiniRecord::GetDataLen() const
{
	uint32 nLen = FIXED_SIZE;
	size_t i;
	size_t c = m_fields.size();
	for(i = 0; i < c; i++)
	{
		nLen += m_fields[i].GetLen();
	}
	return nLen;
}


_MINIDATA_END