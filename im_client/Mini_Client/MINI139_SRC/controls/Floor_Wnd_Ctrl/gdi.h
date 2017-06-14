#ifndef _GDINODESTACK
#define _GDINODESTACK

#include <afxtempl.h>

struct CGdiNodeStack
{
	CDC* m_pDC;
	CGdiObject* m_pGdiObject;
	CGdiObject* m_pSelectObject;
	CGdiNodeStack(CDC* pDC, CGdiObject* pGdiObject)
	{
		m_pDC = pDC;
		m_pGdiObject = pGdiObject;
		m_pSelectObject = m_pDC->SelectObject(m_pGdiObject);
	}
	~CGdiNodeStack()
	{
		m_pDC->SelectObject(m_pSelectObject);
	}
};

struct CGdiStack : public CList<CGdiNodeStack*, CGdiNodeStack*>
{
	void Push(CDC* pDC, CGdiObject* pGdiObject)
	{
		AddTail(new CGdiNodeStack(pDC, pGdiObject));
	}
	void Pop()
	{
		CGdiNodeStack* p = RemoveTail();
		delete p;
	}
};


#endif //_GDINODESTACK
