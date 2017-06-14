#pragma once

class AFX_EXT_CLASS CSkinBitmap : public CBitmap
{
public:
	CSkinBitmap(void);
	virtual ~CSkinBitmap(void);

public:
	BOOL LoadBitmap(UINT nIDResource);
	BOOL Attach(HGDIOBJ hObject);
	HGDIOBJ Detach();
	//operator CBitmap *() const;
	//operator = (CBitmap *p);

private:
	HGDIOBJ   m_hResObj;
};

