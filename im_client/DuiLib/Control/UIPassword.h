#ifndef __UIPASSWORD__H__
#define __UIPASSWORD__H__

#pragma  once
#include <string>
namespace DuiLib
{

class UILIB_API CPasswordUI:public CButtonUI
{
public:
	enum {PASSWORD_CARET=12};
	CPasswordUI();
	LPCTSTR GetClass() const;
	LPVOID  GetInterface(LPCTSTR pstrName);
	void	DoEvent(TEventUI& event);
	void	PaintText(HDC hDC);

	void	SetActive(BOOL active);
	INT		PosFromChar(UINT nChar) ;
	INT		CharFromPos(INT pos) ;
	void    SetLimitText(UINT nMax);
	int		GetTextLenth( std::string strLen);
	void	Clear();
	TCHAR*  GetPassword();

	void	SetCaretColor(DWORD caretColor);
	void	SetCaretSpeed(int speed);
	void	SetCaretWidth( int iWidth );

	void	SetPasswordClr( DWORD pswColor );
	void	SetPasswordChar(TCHAR nChar);
	void	SetPswFont(int index);
	void	SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);
protected:
	void	ShowCaret();
	void	HideCaret();
	void	SetCaretPos(int x, int y);
private:
	std::string		m_strText;
	POINT			m_ptCursor;		// ��ǰ����λ�á�
	INT				m_nCharPos;			// ��ǰ��괦�ڵڼ����ַ��ĺ��棬��0��ʼ������
	HIMC			m_hIMC;
	int				m_iFontPsw;
	TCHAR			m_nPswChar;
	SIZE            m_szCaret;
	BOOL            m_blShowCaret;
	DWORD			m_dwCaretClr;
	DWORD			m_dwPswClr;
	int				m_iSpeed;
	int				m_iCaretWidth;

};

}
#endif