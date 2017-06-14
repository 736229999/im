#ifndef _PUBLIC_DEF_H_
#define _PUBLIC_DEF_H_

#include <afxmt.h>

#define WM_THREAD_NOTIFY        WM_USER+1
#define WM_WEBFACE_NOTIFY       WM_USER+2
#define WM_WEBPIC_NOTIFY        WM_USER+3
#define WM_WEBPICTURE_NOTIFY    WM_USER+4
#define WM_WEBDATA_NOTIFY       WM_USER+5

typedef unsigned char    uint8;
typedef unsigned short   uint16;
typedef unsigned int     uint32;
typedef unsigned __int64 uint64;


template <class T>
class FIFO : public list<T>
{
public:
	inline bool Pop(T& data)
	{
		if ( size()>0 ) 
		{
			data = front();
			pop_front();
			return true;
		}
		return false;
	}

	inline bool Push(const T& data,bool bTail=true)
	{
		if ( bTail )
			push_back(data);
		else
			push_front(data);

		return true;
	}

	inline long  GetCount()
	{
		return size();
	}

	inline void Clear()
	{
		clear();
	}

	T operator [] (int n)
	{
		ASSERT( n>=0 || n<size() );

		list<T>::iterator it;
		it=begin();
		while (n-->0)
			it++;
		return *it;
	}
};

#endif