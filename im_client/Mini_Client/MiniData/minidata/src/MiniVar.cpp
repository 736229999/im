#include "stdafx.h"

#include "../inc/MiniRecord.h"

_MINIDATA_BEG

STATUS	CMiniVar::Write(CMiniWriter & w) const
{
	switch(m_type)
	{
	case MV_INT32:
		if(m_restrict.type == MINI_RESTRICT::EMPTY)
		{
			w<<m_var.i32;
			return OK;
		}
		else if(m_restrict.type == MINI_RESTRICT::VAL_SIGNED)
		{
			if((int32)m_var.i32 > (int32)m_restrict.val)
				return DATA_VAL_OVER;
		}
		else if(m_restrict.type == MINI_RESTRICT::VAL_UNSIGNED)
		{
			if(m_var.i32 > m_restrict.val)
				return DATA_VAL_OVER;
		}
		w<<m_var.i32;
		break;
	case MV_DATA:
		if(m_restrict.type == MINI_RESTRICT::DATA_LEN)
		{
			// 超长
			if(m_var.data.nLen > m_restrict.val)
				return DATA_LEN_OVER;
		}
		w<<m_var.data;
		break;
	case MV_INT8:
		if(m_restrict.type == MINI_RESTRICT::EMPTY)
		{
			w<<m_var.i8;
			return OK;
		}
		else if(m_restrict.type == MINI_RESTRICT::VAL_SIGNED)
		{
			if((int8)m_var.i8 > (int8)m_restrict.val)
				return DATA_VAL_OVER;
		}
		else if(m_restrict.type == MINI_RESTRICT::VAL_UNSIGNED)
		{
			if(m_var.i8 > m_restrict.val)
				return DATA_VAL_OVER;
		}
		w<<m_var.i8;
		break;
	case MV_INT16:
		if(m_restrict.type == MINI_RESTRICT::EMPTY)
		{
			w<<m_var.i16;
			return OK;
		}
		else if(m_restrict.type == MINI_RESTRICT::VAL_SIGNED)
		{
			if((int16)m_var.i16 > (int16)m_restrict.val)
				return DATA_VAL_OVER;
		}
		else if(m_restrict.type == MINI_RESTRICT::VAL_UNSIGNED)
		{
			if(m_var.i16 > m_restrict.val)
				return DATA_VAL_OVER;
		}
		w<<m_var.i16;
		break;
	case MV_FLOAT32:
		if(m_restrict.type == MINI_RESTRICT::EMPTY)
		{
			w<<m_var.f32;
			return OK;
		}
		else if(m_restrict.type == MINI_RESTRICT::VAL_SIGNED)
		{
			if((float32)m_var.f32 > (float32)m_restrict.val)
				return DATA_VAL_OVER;
		}
		else if(m_restrict.type == MINI_RESTRICT::VAL_UNSIGNED)
		{
			if(m_var.f32 > m_restrict.val)
				return DATA_VAL_OVER;
		}
		w<<m_var.f32;
		break;
	case MV_FLOAT64:
		if(m_restrict.type == MINI_RESTRICT::EMPTY)
		{
			w<<m_var.f64;
			return OK;
		}
		else if(m_restrict.type == MINI_RESTRICT::VAL_SIGNED)
		{
			if((float64)m_var.f64 > (float64)m_restrict.val)
				return DATA_VAL_OVER;
		}
		else if(m_restrict.type == MINI_RESTRICT::VAL_UNSIGNED)
		{
			if(m_var.f64 > m_restrict.val)
				return DATA_VAL_OVER;
		}
		w<<m_var.f64;
		break;
	case MV_INT64:
		if(m_restrict.type == MINI_RESTRICT::EMPTY)
		{
			w<<m_var.i64;
			return OK;
		}
		else if(m_restrict.type == MINI_RESTRICT::VAL_SIGNED)
		{
			if((int64)m_var.i64 > (int64)m_restrict.val)
				return DATA_VAL_OVER;
		}
		else if(m_restrict.type == MINI_RESTRICT::VAL_UNSIGNED)
		{
			if(m_var.i64 > m_restrict.val)
				return DATA_VAL_OVER;
		}
		w<<m_var.i64;
		break;
	default:
		return UNKNOWN_VAR_TYPE;
	};
	return OK;
}

void	CMiniVar::operator = (const char * sz)
{
	size_t nLen; 
	nLen = strlen(sz);
	if(m_type == MV_DATA)
	{
		if(m_var.data.nSpace < nLen)
		{
			if(m_var.data.pData)
				delete [] m_var.data.pData;
			m_var.data.pData = new uint8[nLen + 1];
			m_var.data.nSpace = (uint32)nLen;
		}
		if(nLen != 0)
			memcpy(m_var.data.pData,sz,nLen + 1);
		else if(m_var.data.nSpace != 0)
			m_var.data.pData[0] = 0;	
		m_var.data.nLen = (uint32)nLen;
	}
	else
	{
		m_var.i64 = 0;
	}
}

void	CMiniVar::SetData(const uint8 * pData,size_t nLen)
{
	if(m_type == MV_DATA)
	{
		if(m_var.data.nSpace < nLen)
		{
			if(m_var.data.pData)
				delete [] m_var.data.pData;
			m_var.data.pData = new uint8[nLen + 1];
			m_var.data.nSpace = (uint32)nLen;
		}
		memcpy(m_var.data.pData,pData,nLen);
		if(nLen != 0)
			m_var.data.pData[nLen] = 0;
		else if(m_var.data.nSpace != 0)
			m_var.data.pData[0] = 0;

		m_var.data.nLen = (uint32)nLen;
	}
	else
	{
		m_var.i64 = 0;
	}
};


CMiniVar::CMiniVar(const CMiniVar &v)
{
	if(v.m_type == MV_DATA)
	{
		m_var.data.nLen = v.m_var.data.nLen;
		m_var.data.nSpace = v.m_var.data.nLen;
		if( v.m_var.data.nLen != 0)
		{
			m_var.data.pData = new uint8[v.m_var.data.nLen + 1];
			memcpy(m_var.data.pData,v.m_var.data.pData,v.m_var.data.nLen);
			m_var.data.pData[v.m_var.data.nLen] = 0;
		}
		else
		{
			m_var.data.pData = NULL;
		}
	}
	else
	{
		m_var.i64 = v.m_var.i64;
	}
	m_type = v.m_type;
	m_restrict = v.m_restrict;
};

void CMiniVar::operator = (const CMiniVar & v)
{ 
	switch(v.m_type)
	{
	case MV_INT32:
		switch(m_type)
		{		
		case MV_INT32:
			m_var.i32 = v.m_var.i32;
			break;
		case MV_DATA:
			m_var.data.nLen = 0;
			//m_var.data.nSpace = 0;
			if(m_var.data.pData)
			{
				//delete [] m_var.data.pData;
				*m_var.data.pData = 0;
			}
			break;
		case MV_INT8:
			// 强行截取
			m_var.i8 = (uint8)v.m_var.i32;
			break;
		case MV_INT16:
			// 强行截取
			m_var.i16 = (uint16)v.m_var.i32;
			break;	
		case MV_FLOAT32:
			// 强行截取
			m_var.f32 = (float32)v.m_var.i32;
			break;
		case MV_FLOAT64:
			m_var.f64 = v.m_var.i32;
			break;
		case MV_INT64:
			m_var.i64 = v.m_var.i32;
			break;
		};
		break;
	case MV_DATA:
		switch(m_type)
		{
		case MV_DATA:
			if(m_var.data.nSpace < v.m_var.data.nLen)
			{
				if(m_var.data.pData)
					delete [] m_var.data.pData;
				m_var.data.pData = new uint8[v.m_var.data.nLen + 1];
				m_var.data.nSpace = v.m_var.data.nLen;
			}
			m_var.data.nLen = v.m_var.data.nLen;
			memcpy(m_var.data.pData,v.m_var.data.pData,m_var.data.nLen);
			if(m_var.data.nLen != 0)
				m_var.data.pData[m_var.data.nLen] = 0;
			else if(m_var.data.nSpace != 0)
				m_var.data.pData[0] = 0;
			break;
		default:
			m_var.i64 = 0;
			break;
		};
		break;
	case MV_INT8:
		switch(m_type)
		{
		case MV_INT8:
			m_var.i8 = v.m_var.i8;
			break;
		case MV_INT32:
			m_var.i32 = v.m_var.i8;
			break;
		case MV_DATA:
			m_var.data.nLen = 0;
			//m_var.data.nSpace = 0;
			if(m_var.data.pData)
			{
				//delete [] m_var.data.pData;
				*m_var.data.pData = 0;
			}
			break;	
		case MV_INT16:
			m_var.i16 = v.m_var.i8;
			break;
		case MV_FLOAT32:
			m_var.f32 = (float32)v.m_var.i8;
			break;
		case MV_FLOAT64:
			m_var.f64 = v.m_var.i8;
			break;
		case MV_INT64:
			m_var.i64 = v.m_var.i8;
			break;
		};
		break;
	case MV_INT16:
		switch(m_type)
		{
		case MV_INT16:
			m_var.i16 = v.m_var.i16;
			break;		
		case MV_INT32:
			m_var.i32 = v.m_var.i16;
			break;
		case MV_DATA:
			m_var.data.nLen = 0;
			//m_var.data.nSpace = 0;
			if(m_var.data.pData)
			{
				//delete [] m_var.data.pData;
				*m_var.data.pData = 0;
			}
			break;
		case MV_INT8:
			// 强行截取
			m_var.i8 = (uint8)v.m_var.i16;
			break;
		case MV_FLOAT32:
			m_var.f32 = v.m_var.i16;
			break;
		case MV_FLOAT64:
			m_var.f64 = v.m_var.i16;
			break;
		case MV_INT64:
			m_var.i64 = v.m_var.i16;
			break;
		};
		break;
	case MV_FLOAT32:
		switch(m_type)
		{
		case MV_FLOAT32:
			m_var.f32 = v.m_var.f32;
			break;			
		case MV_INT32:
			m_var.i32 = (uint32)v.m_var.f32;
			break;
		case MV_DATA:
			m_var.data.nLen = 0;
			//m_var.data.nSpace = 0;
			if(m_var.data.pData)
			{
				//delete [] m_var.data.pData;
				*m_var.data.pData = 0;
			}
			break;
		case MV_INT8:
			// 强行截取
			m_var.i8 = (uint8)v.m_var.f32;
			break;
		case MV_INT16:
			// 强行截取
			m_var.i16 = (uint16)v.m_var.f32;
			break;
		case MV_FLOAT64:
			m_var.f64 = v.m_var.f32;
			break;
		case MV_INT64:
			m_var.i64 = (uint64)v.m_var.f32;
			break;
		};
		break;
	case MV_FLOAT64:
		switch(m_type)
		{
		case MV_FLOAT64:
			m_var.f64 = v.m_var.f64;
			break;		
		case MV_INT32:
			m_var.i32 = (uint32)v.m_var.f64;
			break;
		case MV_DATA:
			m_var.data.nLen = 0;
			//m_var.data.nSpace = 0;
			if(m_var.data.pData)
			{
				//delete [] m_var.data.pData;
				*m_var.data.pData = 0;
			}
			break;
		case MV_INT8:
			// 强行截取
			m_var.i8 = (uint8)v.m_var.f64;
			break;
		case MV_INT16:
			// 强行截取
			m_var.i16 = (uint16)v.m_var.f64;
			break;
		case MV_FLOAT32:
			m_var.f32 = (float32)v.m_var.f64;
			break;
		case MV_INT64:
			m_var.i64 = (uint64)v.m_var.f64;
			break;	
		};
		break;
	case MV_INT64:
		switch(m_type)
		{
		case MV_INT64:
			m_var.i64 = v.m_var.i64;
			break;		
		case MV_INT32:
			// 强行截取
			m_var.i32 = (uint32)v.m_var.i64;
			break;
		case MV_DATA:
			m_var.data.nLen = 0;
			//m_var.data.nSpace = 0;
			if(m_var.data.pData)
			{
				//delete [] m_var.data.pData;
				*m_var.data.pData = 0;
			}
			break;
		case MV_INT8:
			// 强行截取
			m_var.i8 = (uint8)v.m_var.i64;
			break;
		case MV_INT16:
			// 强行截取
			m_var.i16 = (uint16)v.m_var.i64;
			break;
		case MV_FLOAT32:
			// 强行截取
			m_var.f32 = (float32)v.m_var.i64;
			break;
		case MV_FLOAT64:
			// 强行截取
			m_var.f64 = (float64)v.m_var.i64;
			break;	
		};
		break;
	default:
		assert(0);
		break;
	}
};

STATUS	CMiniVar::Read(CMiniReader & r)
{
	bool b = false;
	switch(m_type)
	{
	case MV_INT32:
		if(r.Read((uint8*)&m_var.i32,4))
		{
			if(m_restrict.type == MINI_RESTRICT::EMPTY)
				return OK;
			if(m_restrict.type == MINI_RESTRICT::VAL_SIGNED)
				return ((int32)m_var.i32 <= (int32)m_restrict.val)?OK:DATA_VAL_OVER;
			else
				return (m_var.i32 <= m_restrict.val)?OK:DATA_VAL_OVER;
		}
		else
		{
			m_var.i64 = 0;
		}
		return READ_EOF;
	case MV_DATA:
		{
			uint32 iLen = 0; 
			if(r.Read((uint8*)&iLen,4) == false)
				return READ_EOF;
			if(m_restrict.type == MINI_RESTRICT::DATA_LEN)
			{
				// 数据超长
				if(iLen > m_restrict.val)
					return DATA_LEN_OVER;
			}
			if(m_var.data.nSpace < iLen)
			{
				if(m_var.data.pData)
					delete [] m_var.data.pData;
				m_var.data.pData = new uint8[iLen + 1];
				m_var.data.nSpace = iLen;
			}
			if (r.Read(m_var.data.pData,iLen))
			{
				if(iLen != 0)
					m_var.data.pData[iLen] = 0;
				else if(m_var.data.nSpace)
					m_var.data.pData[0] = 0;
				m_var.data.nLen = iLen;
				return OK;
			}
			else
			{
				if(m_var.data.pData)
				{
					delete [] m_var.data.pData;
					m_var.data.pData = NULL;
				}
				m_var.data.nLen = 0;
				m_var.data.nSpace = 0;
				return READ_EOF;
			}
			return OK;
		};
	case MV_INT8:
		if(r.Read((uint8*)&m_var.i8,1))
		{
			if(m_restrict.type == MINI_RESTRICT::EMPTY)
				return OK;
			else if(m_restrict.type == MINI_RESTRICT::VAL_SIGNED)
				return ((int8)m_var.i8 <= (int8)m_restrict.val)?OK:DATA_VAL_OVER;
			else
				return (m_var.i8 <= m_restrict.val)?OK:DATA_VAL_OVER;
		}
		else
		{
			m_var.i64 = 0;
		}
		return READ_EOF;
	case MV_INT16:
		if(r.Read((uint8*)&m_var.i16,2))
		{
			if(m_restrict.type == MINI_RESTRICT::EMPTY)
				return OK;
			else if(m_restrict.type == MINI_RESTRICT::VAL_SIGNED)
				return ((int16)m_var.i16 <= (int16)m_restrict.val)?OK:DATA_VAL_OVER;
			else
				return (m_var.i16 <= m_restrict.val)?OK:DATA_VAL_OVER;
		}
		else
		{
			m_var.i64 = 0;
		}
		return READ_EOF;
	case MV_FLOAT32:
		if(r.Read((uint8*)&m_var.f32,4))
		{
			if(m_restrict.type == MINI_RESTRICT::EMPTY)
				return OK;
			else if(m_restrict.type == MINI_RESTRICT::VAL_SIGNED)
				return ((float32)m_var.f32 <= (float32)m_restrict.val)?OK:DATA_VAL_OVER;
			else
				return (m_var.f32 <= m_restrict.val)?OK:DATA_VAL_OVER;
		}
		return READ_EOF;
	case MV_FLOAT64:
		if(r.Read((uint8*)&m_var.f64,8))
		{
			if(m_restrict.type == MINI_RESTRICT::EMPTY)
				return OK;
			else if(m_restrict.type == MINI_RESTRICT::VAL_SIGNED)
				return ((float64)m_var.f64 <= (float64)m_restrict.val)?OK:DATA_VAL_OVER;
			else
				return (m_var.f64 <= m_restrict.val)?OK:DATA_VAL_OVER;
		}
		else
		{
			m_var.i64 = 0;
		}
		return READ_EOF;
	case MV_INT64:
		if(r.Read((uint8*)&m_var.i64,8))
		{
			if(m_restrict.type == MINI_RESTRICT::EMPTY)
				return OK;
			else if(m_restrict.type == MINI_RESTRICT::VAL_SIGNED)
				return ((int64)m_var.i64 <= (int64)m_restrict.val)?OK:DATA_VAL_OVER;
			else
				return (m_var.i64 <= m_restrict.val)?OK:DATA_VAL_OVER;
		}
		else
		{
			m_var.i64 = 0;
		}
		return READ_EOF;
	default:
		return UNKNOWN_VAR_TYPE;
	};
	return OK;
};


CMiniVar::CMiniVar(const char * sz)
{ 
	size_t nLen; 
	nLen = strlen(sz);
	m_var.data.pData = new uint8[nLen + 1];
	memcpy(m_var.data.pData,sz,nLen + 1);
	m_type = MV_DATA;
	m_var.data.nLen = (uint32)nLen;
	m_var.data.nSpace = (uint32)nLen;
	m_restrict = NULL_RESTIRCT;
};


CMiniVar::~CMiniVar()
{
	if(m_type == MV_DATA)
	{
		if(m_var.data.pData)
		{
			delete [] m_var.data.pData;
		}
	}
}


_MINIDATA_END