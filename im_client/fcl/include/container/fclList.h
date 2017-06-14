/******************************************************************
** 文件名:	fclList.h
** 版  权:	(C)  
** 创建人:	Tony
** 日  期:	2007-5-21
** 描  述:	线程安全链表类封装

**************************** 修改记录 ******************************
** 修改人: 
** 日  期: 
** 描  述: 
********************************************************************/

#ifndef __FCL_LIST__
#define __FCL_LIST__
#include <list>
#include "thread/ThreadingModel.h"
using namespace std;
BEGIN_NAMESPACE_FCL

template <class _Ty,
class threadMode = multiThread ,
class _Ax = allocator<_Ty> >
class fclList: protected list<_Ty,_Ax>
{
private:
	typedef list<_Ty,_Ax>   BASE;
public:
	typename threadMode::mutex m_mutex;///同步锁
	typedef typename BASE::iterator iterator;
	typedef typename BASE::const_iterator const_iterator;
	//list<_Ty,_Ax> m_list;///对象适配器模式
public:
	iterator begin()
	{	// return iterator for beginning of mutable sequence
		threadMode::scope_lock guard(m_mutex);
		return BASE::begin();
	}

	iterator end()
	{	// return iterator for end of mutable sequence
		//return (iterator(_Myhead));
		threadMode::scope_lock guard(m_mutex);
		return BASE::end();
	}


	reverse_iterator rbegin()
	{	// return iterator for beginning of reversed mutable sequence
		//return (reverse_iterator(end()));
		threadMode::scope_lock guard(m_mutex);
		return BASE::rbegin();
	}

	reverse_iterator rend()
	{	// return iterator for end of reversed mutable sequence
		//return (reverse_iterator(begin()));
		threadMode::scope_lock guard(m_mutex);
		return BASE::rend();
	}

	void resize(size_type _Newsize)
	{	// determine new length, padding with _Ty() elements as needed
		//resize(_Newsize, _Ty());
		threadMode::scope_lock guard(m_mutex);
		return BASE::resize(_Newsize);
	}

	void resize(size_type _Newsize, _Ty _Val)
	{	// determine new length, padding with _Val elements as needed
		//if (_Mysize < _Newsize)
		//	_Insert_n(end(), _Newsize - _Mysize, _Val);
		//else
		//	while (_Newsize < _Mysize)
		//		pop_back();
		threadMode::scope_lock guard(m_mutex);
		return BASE::resize(_Newsize, _Val);
	}

	size_type size() 
	{	// return length of sequence
		//return (_Mysize);
		threadMode::scope_lock guard(m_mutex);
		return BASE::size();
	}

	size_type max_size() 
	{	// return maximum possible length of sequence
		//return (this->_Alval.max_size());
		threadMode::scope_lock guard(m_mutex);
		return BASE::max_size();
	}

	bool empty()
	{	// test if sequence is empty
		//return (_Mysize == 0);
		threadMode::scope_lock guard((m_mutex));
		return BASE::empty();
	}

	allocator_type get_allocator() const
	{	// return allocator object for values
		//return (this->_Alval);
		threadMode::scope_lock guard(m_mutex);
		return BASE::get_allocator();
	}

	reference front()
	{	// return first element of mutable sequence
		//return (*begin());
		threadMode::scope_lock guard(m_mutex);
		return BASE::front();
	}

	const_reference front() const
	{	// return first element of nonmutable sequence
		//return (*begin());
		threadMode::scope_lock guard(m_mutex);
		return BASE::front();
	}

	reference back()
	{	// return last element of mutable sequence
		//return (*(--end()));
		threadMode::scope_lock guard(m_mutex);
		return BASE::back();
	}

	const_reference back() const
	{	// return last element of nonmutable sequence
		//return (*(--end()));
		threadMode::scope_lock guard(m_mutex);
		return BASE::back();
	}

	void push_front(const _Ty& _Val)
	{	// insert element at beginning
		//_Insert(begin(), _Val);
		threadMode::scope_lock guard(m_mutex);
		return BASE::push_front();
	}

	void pop_front()
	{	// erase element at beginning
		//erase(begin());
		threadMode::scope_lock guard(m_mutex);
		return BASE::pop_front();
	}

	void push_back(const _Ty& _Val)
	{	// insert element at end
		//_Insert(end(), _Val);
		threadMode::scope_lock guard(m_mutex);
		return BASE::push_back(_Val);
	}

	void pop_back()
	{	// erase element at end
		//erase(--end());
		threadMode::scope_lock guard(m_mutex);
		return BASE::pop_back();
	}
	iterator erase(iterator _Where)
	{	// erase element at _Where
		threadMode::scope_lock guard(m_mutex);
		return BASE::erase(_Where);
	}

	iterator erase(iterator _First, iterator _Last)
	{	// erase [_First, _Last)
		threadMode::scope_lock guard(m_mutex);
		return BASE::erase(_First,_Last);
	}

	void clear()
	{	// erase all
		threadMode::scope_lock guard(m_mutex);
		return BASE::clear();
	}

	void swap(_Myt& _Right)
	{	// exchange contents with _Right
		threadMode::scope_lock guard(m_mutex);
		return BASE::swap(_Right);
	}

	void splice(iterator _Where, _Myt& _Right)
	{	// splice all of _Right at _Where
		threadMode::scope_lock guard(m_mutex);
		return BASE::splice(_Where,_Right);
	}

	void remove(const _Ty& _Val)
	{	// erase each element matching _Val
		threadMode::scope_lock guard(m_mutex);
		return BASE::remove(_Val);
	}
};
//
//template <class _Ty,
//class _Ax = allocator<_Ty> ,
//class threadMode = singleThread >
//class _fclList2: public list<_Ty,_Ax>
//{
//private:
//	typedef list<_Ty,_Ax>   BASE;
//public:
//	typename threadMode::mutex m_mutex;///同步锁
//	typedef typename BASE::iterator iterator;
//	typedef typename BASE::const_iterator const_iterator;
//};
//template <class _Ty,
//class threadMode = multiThread >
//class fclList
//{
//public:
//	typedef _fclList1<_Ty,allocator<_Ty>,threadMode> list;
//	typedef typename list::iterator iterator;
//	typedef typename list::const_iterator const_iterator;
//};
//template<class _Ty>
//class fclList<_Ty,singleThread>
//{
//public:
//	typedef _fclList2<_Ty,allocator<_Ty>, singleThread>   list;
//	typedef typename list::iterator iterator;
//	typedef typename list::const_iterator const_iterator;
//};
END_NAMESPACE
#endif/*__FCL_LIST*/