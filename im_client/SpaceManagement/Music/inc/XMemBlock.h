#pragma once

#include <wtypes.h>
#include <assert.h>

//------------------------------------------------------
// ÄÚ´æ¿é
//------------------------------------------------------

class XMemBlock
{
public:

	XMemBlock()
	{
		_pData = NULL; 
		_nLen = 0; 
	};

	explicit XMemBlock(int nSize)
	{
		assert(nSize > 0);
		_pData = new BYTE[nSize];
		_nLen = nSize;
	}

	~XMemBlock()
	{
		if(_pData)
		{
			delete [] _pData;
		}
	};

public:

	operator BYTE * ()
	{ 
		return _pData;
	};

	operator char * ()
	{ 
		return (char*)_pData; 
	};

	BYTE *	 GetMem()
	{ 
		return _pData; 
	};

	void	Resize(int nNewSize)
	{ 
		if(nNewSize > _nLen)
		{
			if(_pData)
			{
				delete [] _pData;
				_pData = NULL;
			}
			_pData = new BYTE[nNewSize];
			_nLen = nNewSize;
		}
	};

	int		GetSize() const
	{ 
		return _nLen; 
	};

protected:

	void operator = (const XMemBlock & m);

	XMemBlock(const XMemBlock & m);

protected:

	BYTE *		_pData;

	int			_nLen;

};