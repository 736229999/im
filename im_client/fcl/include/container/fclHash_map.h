#ifndef __FCL_HASH_MAP__
#define __FCL_HASH_MAP__

/******************************************************************
** 文件名:	fclHash_map.h
** 版  权:	(C)  
** 创建人:	Tony
** 日  期:	2007-5-21
** 描  述:	线程安全hash_map类封装

**************************** 修改记录 ******************************
** 修改人: 
** 日  期: 
** 描  述: 
********************************************************************/
#include <list>
#include <xhash>
#include <hash_map>
#include "thread/ThreadingModel.h"
using namespace stdext;

BEGIN_NAMESPACE_FCL

///原始hash_map继承，线程安全封装
template<class _Kty,
class _Ty,
class threadMode = multiThread,
class _Tr = _STDEXT hash_compare<_Kty, less<_Kty> >,
class _Alloc = allocator<pair<const _Kty, _Ty> > >
class fclHash_map 
{
private:
	typedef hash_map<_Ty,_Tr>   BASE;
	stdext::hash_map<_Kty,_Ty,_Tr,_Alloc> m_map;//原始map
	//typedef _STDEXT _Hash< _STDEXT _Hmap_traits<_Kty, _Ty, _Tr, _Alloc, false> > _Mybase;
public:
	typename threadMode::mutex m_mutex;///同步锁

	typedef _Hash<_Hmap_traits<_Kty, _Ty, _Tr, _Alloc, false> > _Mybase;
	typedef _Kty key_type;
	typedef _Ty mapped_type;
	typedef _Ty referent_type;
	typedef _Tr key_compare;
	typedef typename _Mybase::value_compare value_compare;
	typedef typename _Mybase::allocator_type allocator_type;
	typedef typename _Mybase::size_type size_type;
	typedef typename _Mybase::difference_type difference_type;
	typedef typename _Mybase::pointer pointer;
	typedef typename _Mybase::const_pointer const_pointer;
	typedef typename _Mybase::reference reference;
	typedef typename _Mybase::const_reference const_reference;
	typedef typename _Mybase::iterator iterator;
	typedef typename _Mybase::const_iterator const_iterator;
	typedef typename _Mybase::reverse_iterator reverse_iterator;
	typedef typename _Mybase::const_reverse_iterator
		const_reverse_iterator;
	typedef typename _Mybase::value_type value_type;
public:
	 mapped_type& operator[](const key_type& _Keyval)
	{	// find element matching _Keyval or insert with default mapped
		threadMode::scope_lock  guard(m_mutex);

		return m_map[_Keyval];
	}
	iterator begin()
	{	// return iterator for beginning of mutable sequence
		threadMode::scope_lock guard(m_mutex);
		return m_map.begin();
	}

	iterator end()
	{	// return iterator for end of mutable sequence
		threadMode::scope_lock guard(m_mutex);
		return m_map.end();
	}


	reverse_iterator rbegin()
	{	// return iterator for beginning of reversed mutable sequence
		threadMode::scope_lock guard(m_mutex);

		return m_map.rbegin();
	}


	reverse_iterator rend()
	{	// return iterator for end of reversed mutable sequence
		threadMode::scope_lock guard(m_mutex);
		return m_map.rend();
	}

	//_Pairib insert(const value_type& _Val)
	//{	// try to insert node with value _Val
	//	threadMode::scope_lock guard(m_mutex);
	//	return m_map.insert(_Val);
	//}

	iterator insert(iterator, const value_type& _Val)
	{	// try to insert node with value _Val, ignore hint
		threadMode::scope_lock guard(m_mutex);
		return m_map.insert(iterator,_Val);
	}


	iterator erase(iterator _Where)
	{	// erase element at _Where
		threadMode::scope_lock guard(m_mutex);
		return m_map.erase(_Where);
	}

	iterator erase(iterator _First, iterator _Last)
	{	// erase [_First, _Last)
		threadMode::scope_lock guard(m_mutex);
		return m_map.erase(_First,_Last);
	}

	size_type erase(const key_type& _Keyval)
	{	
		threadMode::scope_lock guard(m_mutex);
		return m_map.erase(_Keyval);
	}

	void erase(const key_type *_First,
		const key_type *_Last)
	{	// erase all that match array of keys [_First, _Last)
		threadMode::scope_lock guard(m_mutex);
		return m_map.erase(_First,_Last);
	}

	void clear()
	{	// erase all
		threadMode::scope_lock guard(m_mutex);
		return m_map.clear();
	}

	iterator find(const key_type& _Keyval)
	{	// find an element in mutable hash table that matches _Keyval
		threadMode::scope_lock guard(m_mutex);
		return m_map.find(_Keyval);
	}


	size_type count(const key_type& _Keyval) 
	{	// count all elements that match _Keyval
		threadMode::scope_lock guard(m_mutex);
		return m_map.count(_Keyval);
	}
	size_type size() const
	{	// return length of sequence
		return m_map.size();
	}

};


END_NAMESPACE


#endif/*__FCL_HASH_MAP__*/