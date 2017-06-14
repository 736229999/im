#pragma once

#ifndef __MINI_VAR_H__
#define __MINI_VAR_H__

inline void strxcpy(char * szDst,const char * szSrc,int x)
{
	char * p = szDst + x - 1;
	while (x && (*szDst++ = *szSrc++))
		x--;
	if (x)  
	{
		if(--x)
			*szDst = '\0';
	}
	else
		*p = '\0';
}



inline  CMiniVar::CMiniVar(MINI_VAR_TYPE t)
{
	m_type = t;
	m_var.i64 = 0;
	m_restrict = NULL_RESTIRCT;
	m_var.data.nSpace = 0;
}

inline CMiniVar::CMiniVar(MINI_VAR_TYPE t,MINI_RESTRICT r)
{
	m_type = t;
	m_restrict = r;
	m_var.i64 = 0;
	m_var.data.nSpace = 0;
}

inline CMiniVar::CMiniVar()
{ m_type = MV_INT32; m_var.i64 = 0; m_restrict = NULL_RESTIRCT; };

inline CMiniVar::CMiniVar(uint8 v)
{ m_type = MV_INT8; m_var.i8 = v; m_restrict = NULL_RESTIRCT; };

inline CMiniVar::CMiniVar(uint16 v)
{ m_type = MV_INT16; m_var.i16 = v; m_restrict = NULL_RESTIRCT; };

inline CMiniVar::CMiniVar(uint32 v)
{ m_type = MV_INT32; m_var.i32 = v; m_restrict = NULL_RESTIRCT; };

inline CMiniVar::CMiniVar(uint64 v)
{ m_type = MV_INT64; m_var.i64 = v; m_restrict = NULL_RESTIRCT;};

inline CMiniVar::CMiniVar(float32 v)
{ m_type = MV_FLOAT32; m_var.f32 = v; m_restrict = NULL_RESTIRCT;};

inline CMiniVar::CMiniVar(float64 v)
{ m_type = MV_FLOAT64; m_var.f64 = v; m_restrict = NULL_RESTIRCT;};



inline void CMiniVar::operator = (uint8 v)
{
	switch(m_type)
	{
	case MV_INT8:
		m_var.i8 = v;
		break;
	case MV_INT32:
		m_var.i32 = v;
		break;
	case MV_DATA:
		if(m_var.data.pData)
		{
			//delete [] m_var.data.pData;
			*m_var.data.pData = 0;
		}
		m_var.data.nLen = 0;
		//m_var.data.nSpace = 0;
		break;
	case MV_INT16:
		m_var.i16 = v;
		break;	
	case MV_FLOAT32:
		m_var.f32 = v;
		break;
	case MV_FLOAT64:
		m_var.f64 = v;
		break;
	case MV_INT64:
		m_var.i64 = v;
		break;
	default:
		break;
	};
}

inline void CMiniVar::operator = (int8 v)
{
	switch(m_type)
	{
	case MV_INT8:
		m_var.i8 = v;
		break;
	case MV_INT32:
		m_var.i32 = v;
		break;
	case MV_DATA:
		if(m_var.data.pData)
		{
			//delete [] m_var.data.pData;
			*m_var.data.pData = 0;
		}
		m_var.data.nLen = 0;
		//m_var.data.nSpace = 0;
		break;
	case MV_INT16:
		m_var.i16 = v;
		break;	
	case MV_FLOAT32:
		m_var.f32 = v;
		break;
	case MV_FLOAT64:
		m_var.f64 = v;
		break;
	case MV_INT64:
		m_var.i64 = v;
		break;
	default:
		break;
	};
}

inline void	CMiniVar::operator = (uint16 v)
{
	switch(m_type)
	{
	case MV_INT16:
		m_var.i16 = v;
		break;
	case MV_INT32:
		m_var.i32 = v;
		break;
	case MV_DATA:
		if(m_var.data.pData)
		{
			//delete [] m_var.data.pData;
			*m_var.data.pData = 0;
		}
		m_var.data.nLen = 0;
		//m_var.data.nSpace = 0;
		break;
	case MV_INT8:
		m_var.i8 = (uint8)v;
		break;	
	case MV_FLOAT32:
		m_var.f32 = v;
		break;
	case MV_FLOAT64:
		m_var.f64 = v;
		break;
	case MV_INT64:
		m_var.i64 = v;
		break;
	default:
		break;
	};
}

inline void	CMiniVar::operator = (int16 v)
{
	switch(m_type)
	{
	case MV_INT16:
		m_var.i16 = v;
		break;
	case MV_INT32:
		m_var.i32 = v;
		break;
	case MV_DATA:
		if(m_var.data.pData)
		{
			//delete [] m_var.data.pData;
			*m_var.data.pData = 0;
		}
		m_var.data.nLen = 0;
		//m_var.data.nSpace = 0;
		break;
	case MV_INT8:
		m_var.i8 = (uint8)v;
		break;	
	case MV_FLOAT32:
		m_var.f32 = v;
		break;
	case MV_FLOAT64:
		m_var.f64 = v;
		break;
	case MV_INT64:
		m_var.i64 = v;
		break;
	default:
		break;
	};
}

inline void	CMiniVar::operator = (uint32 v)
{
	switch(m_type)
	{
	case MV_INT32:
		m_var.i32 = v;
		break;
	case MV_DATA:
		if(m_var.data.pData)
		{
			//delete [] m_var.data.pData;
			*m_var.data.pData = 0;
		}
		m_var.data.nLen = 0;
		//m_var.data.nSpace = 0;
		break;
	case MV_INT8:
		m_var.i8 = (uint8)v;
		break;
	case MV_INT16:
		m_var.i16 = (uint16)v;
		break;
	case MV_FLOAT32:
		m_var.f32 = (float32)v;
		break;
	case MV_FLOAT64:
		m_var.f64 = v;
		break;
	case MV_INT64:
		m_var.i64 = v;
		break;
	default:
		break;
	};
}

inline void	CMiniVar::operator = (int32 v)
{
	switch(m_type)
	{
	case MV_INT32:
		m_var.i32 = v;
		break;
	case MV_DATA:
		if(m_var.data.pData)
		{
			//delete [] m_var.data.pData;
			*m_var.data.pData = 0;
		}
		m_var.data.nLen = 0;
		//m_var.data.nSpace = 0;
		break;
	case MV_INT8:
		m_var.i8 = (uint8)v;
		break;
	case MV_INT16:
		m_var.i16 = (uint16)v;
		break;
	case MV_FLOAT32:
		m_var.f32 = (float32)v;
		break;
	case MV_FLOAT64:
		m_var.f64 = v;
		break;
	case MV_INT64:
		m_var.i64 = v;
		break;
	default:
		break;
	};
}

inline void	CMiniVar::operator = (uint64 v)
{
	switch(m_type)
	{
	case MV_INT64:
		m_var.i64 = v;
		break;
	case MV_INT32:
		m_var.i32 = (uint32)v;
		break;
	case MV_DATA:
		if(m_var.data.pData)
		{
			//delete [] m_var.data.pData;
			*m_var.data.pData = 0;
		}
		m_var.data.nLen = 0;
		//m_var.data.nSpace = 0;
		break;
	case MV_INT8:
		m_var.i8 = (uint8)v;
		break;
	case MV_INT16:
		m_var.i16 = (uint16)v;
		break;
	case MV_FLOAT32:
		m_var.f32 = (float32)v;
		break;
	case MV_FLOAT64:
		m_var.f64 = (float64)v;
		break;
	default:
		break;
	};
}

inline void	CMiniVar::operator = (float32 v)
{
	switch(m_type)
	{
	case MV_FLOAT32:
		m_var.f32 = v;
		break;
	case MV_INT32:
		m_var.i32 = (uint32)v;
		break;
	case MV_DATA:
		if(m_var.data.pData)
		{
			//delete [] m_var.data.pData;
			*m_var.data.pData = 0;
		}
		m_var.data.nLen = 0;
		//m_var.data.nSpace = 0;
		break;
	case MV_INT16:
		m_var.i16 = (uint16)v;
		break;
	case MV_INT8:
		m_var.i8 = (uint8)v;
		break;
	case MV_FLOAT64:
		m_var.f64 = v;
		break;
	case MV_INT64:
		m_var.i64 = (uint64)v;
		break;
	default:
		break;
	};
}

inline void	CMiniVar::operator = (float64 v)
{
	switch(m_type)
	{
	case MV_FLOAT64:
		m_var.f64 = v;
		break;
	case MV_INT32:
		m_var.i32 = (uint32)v;
		break;
	case MV_DATA:
		if(m_var.data.pData)
		{
			//delete [] m_var.data.pData;
			*m_var.data.pData = 0;
		}
		m_var.data.nLen = 0;
		//m_var.data.nSpace = 0;
		break;
	case MV_INT16:
		m_var.i16 = (uint16)v;
		break;
	case MV_INT8:
		m_var.i8 = (uint8)v;
		break;
	case MV_FLOAT32:
		m_var.f32 = (float32)v;
		break;
	case MV_INT64:
		m_var.i64 = (uint64)v;
		break;
	default:
		break;
	};
}

//inline void	CMiniVar::operator = (const char * sz)
//{
//	size_t nLen; 
//	nLen = strlen(sz);
//	if(m_type == MV_DATA)
//	{
//		if(m_var.data.nSpace < nLen)
//		{
//			if(m_var.data.pData)
//				delete [] m_var.data.pData;
//			m_var.data.pData = new uint8[nLen + 1];
//			m_var.data.nSpace = (uint32)nLen;
//		}
//		if(nLen != 0)
//			memcpy(m_var.data.pData,sz,nLen + 1);
//		else if(m_var.data.nSpace != 0)
//			m_var.data.pData[0] = 0;	
//		m_var.data.nLen = (uint32)nLen;
//	}
//	else
//	{
//		m_var.i64 = 0;
//	}
//}

//inline void	CMiniVar::SetData(const uint8 * pData,size_t nLen)
//{
//	if(m_type == MV_DATA)
//	{
//		if(m_var.data.nSpace < nLen)
//		{
//			if(m_var.data.pData)
//				delete [] m_var.data.pData;
//			m_var.data.pData = new uint8[nLen + 1];
//			m_var.data.nSpace = (uint32)nLen;
//		}
//		memcpy(m_var.data.pData,pData,nLen);
//		if(nLen != 0)
//			m_var.data.pData[nLen] = 0;
//		else if(m_var.data.nSpace != 0)
//			m_var.data.pData[0] = 0;
//		
//		m_var.data.nLen = (uint32)nLen;
//	}
//	else
//	{
//		m_var.i64 = 0;
//	}
//};

inline const uint32 CMiniVar::GetLen() const
{
	switch(m_type)
	{
	case MV_DATA:
		return 4 + m_var.data.nLen;
	case MV_INT32:
		return 4;
	case MV_INT16:
		return 2;
	case MV_INT8:
		return 1;
	case MV_FLOAT32:
		return 4;
	case MV_FLOAT64:
		return 8;
	case MV_INT64:
		return 8;
	default:
		return 0;
	};
}

inline const uint8 *	CMiniVar::GetData(size_t & nLen) const
{
	switch(m_type)
	{
	case MV_DATA:
		nLen = m_var.data.nLen;
		return m_var.data.pData;
	case MV_INT32:
		nLen = 4;
		return (const uint8 *)&m_var.i32;
	case MV_INT16:
		nLen = 2;
		return (const uint8 *)&m_var.i16;
	case MV_INT8:
		nLen = 1;
		return (const uint8 *)&m_var.i8;
	case MV_FLOAT32:
		nLen = 4;
		return (const uint8 *)&m_var.f32;
	case MV_FLOAT64:
		nLen = 8;
		return (const uint8 *)&m_var.f64;
	case MV_INT64:
		nLen = 8;
		return (const uint8 *)&m_var.i64;
	default:
		nLen = 0;
		return 0;
	};
}






inline CMiniVar::operator uint8 () const
{ 
	switch(m_type)
	{
	case MV_INT8:
		return m_var.i8;
	case MV_INT32:
		return (uint8)m_var.i32;
	case MV_DATA:
		return 0;
	case MV_INT16:
		return (uint8)m_var.i16;
	case MV_FLOAT32:
		return (uint8)m_var.f32;
	case MV_FLOAT64:
		return (uint8)m_var.f64;
	case MV_INT64:
		return (uint8)m_var.i64;
	default:
		return 0;
	};
};

inline CMiniVar::operator int8 () const
{ 
	switch(m_type)
	{
	case MV_INT8:
		return m_var.i8;
	case MV_INT32:
		return (int8)m_var.i32;
	case MV_DATA:
		return 0;
	case MV_INT16:
		return (int8)m_var.i16;
	case MV_FLOAT32:
		return (int8)m_var.f32;
	case MV_FLOAT64:
		return (int8)m_var.f64;
	case MV_INT64:
		return (int8)m_var.i64;
	default:
		return 0;
	};
};

inline CMiniVar::operator uint16 () const
{ 
	switch(m_type)
	{
	case MV_INT16:
		return m_var.i16;
	case MV_INT32:
		return m_var.i32;
	case MV_DATA:
		return 0;
	case MV_INT8:
		return m_var.i8;
	case MV_FLOAT32:
		return (uint16)m_var.f32;
	case MV_FLOAT64:
		return (uint16)m_var.f64;
	case MV_INT64:
		return (uint16)m_var.i64;
	default:
		return 0;
	};
};

inline CMiniVar::operator int16 () const
{ 
	switch(m_type)
	{
	case MV_INT16:
		return m_var.i16;
	case MV_INT32:
		return m_var.i32;
	case MV_DATA:
		return 0;
	case MV_INT8:
		return m_var.i8;
	case MV_FLOAT32:
		return (int16)m_var.f32;
	case MV_FLOAT64:
		return (int16)m_var.f64;
	case MV_INT64:
		return (int16)m_var.i64;
	default:
		return 0;
	};
};

inline CMiniVar::operator int32() const
{
	switch(m_type)
	{
	case MV_INT32:
		return m_var.i32;
	case MV_DATA:
		return 0;
	case MV_INT16:
		return m_var.i16;
	case MV_INT8:
		return m_var.i8;
	case MV_FLOAT32:
		return (int32)m_var.f32;
	case MV_FLOAT64:
		return (int32)m_var.f64;
	case MV_INT64:
		return (int32)m_var.i64;
	default:
		return 0;
	};
}

inline CMiniVar::operator uint32() const
{
	switch(m_type)
	{
	case MV_INT32:
		return m_var.i32;
	case MV_DATA:
		return 0;
	case MV_INT16:
		return m_var.i16;
	case MV_INT8:
		return m_var.i8;
	case MV_FLOAT32:
		return (uint32)m_var.f32;
	case MV_FLOAT64:
		return (uint32)m_var.f64;
	case MV_INT64:
		return (uint32)m_var.i64;
	default:
		return 0;
	};
}

inline CMiniVar::operator uint64() const
{
	switch(m_type)
	{
	case MV_INT64:
		return m_var.i64;
	case MV_INT32:
		return m_var.i32;
	case MV_DATA:
		return 0;
	case MV_INT16:
		return m_var.i16;
	case MV_INT8:
		return m_var.i8;
	case MV_FLOAT32:
		return (uint64)m_var.f32;
	case MV_FLOAT64:
		return (uint64)m_var.f64;
	default:
		return 0;
	};
}

inline CMiniVar::operator float32() const
{
	switch(m_type)
	{
	case MV_FLOAT32:
		return m_var.f32;
	case MV_INT32:
		return (float32)m_var.i32;
	case MV_DATA:
		return 0;
	case MV_INT16:
		return m_var.i16;
	case MV_INT8:
		return m_var.i8;
	case MV_FLOAT64:
		return (float32)m_var.f64;
	case MV_INT64:
		return (float32)m_var.i64;
	default:
		return 0;
	};
}

inline CMiniVar::operator const char * () const
{
	if(m_type == MV_DATA)
		return (m_var.data.nLen) ? (const char *)m_var.data.pData : ("");
	else
		return "";
};




//------------------------------------------------------
// 记录属性结构
//------------------------------------------------------


inline uint32	FIELD_INFO::GetLen() const
{ return sName.GetLength() + 20; };

inline STATUS	FIELD_INFO::Read(CMiniReader & r)
{
	if( r.Read(sName) && 
		r.Read((uint32 &)type) &&
		r.Read((uint32 &)restrict.type) &&
		r.Read(restrict.val)
		)
		return OK;
	else
		return READ_EOF;
};

inline STATUS	FIELD_INFO::Write(CMiniWriter & w) const
{
	w<<sName
	 <<(uint32)type
	 <<(uint32)restrict.type
	 <<restrict.val;
	return OK;
};

inline uint32	CMiniRecord::GetVer() const
{ return m_nVer; };

inline CMiniRecordInfo::CMiniRecordInfo()
{
	m_nVer = 0;
	m_nNext = 0;
	m_nLen2 = m_nLen = 0;
	m_nPos = -1;
};

inline CMiniRecordInfo::CMiniRecordInfo(const FIELD_INFO * pAttrs,int nAttrs,int nVer)
{
	assert(pAttrs);
	size_t n = nAttrs;
	size_t i;
	m_nLen = FIXED_SIZE;
	for(i = 0;i < n;i++)
	{
		m_attrs.push_back(pAttrs[i]);
		m_nLen += pAttrs[i].GetLen();
	}
	m_nVer = nVer;
	m_nNext = 0;
	m_nLen2 = m_nLen;
	m_nPos = -1;
};

inline uint32	CMiniRecordInfo::GetVer() const
{ return m_nVer; };

inline uint32	CMiniRecordInfo::GetNext() const
{ return m_nNext; };



#endif