#pragma once
#include "..\..\..\..\DuiLib\UIlib.h"

class CDuiPicture : public DuiLib::WindowImplBase
{
public:
	CDuiPicture(void);
	~CDuiPicture(void);
public:
	virtual LPCTSTR    GetWindowClassName() const   {   return _T("DuiPicture");  }
	virtual DuiLib::CDuiString GetSkinFile()                {   return _T("DuiPicture.xml");    }
	virtual DuiLib::CDuiString GetSkinFolder()              {   return DuiLib::CPaintManagerUI::GetInstancePath();  }
	DuiLib::UILIB_RESOURCETYPE GetResourceType() const;
	LPCTSTR GetResourceID() const;
	virtual void InitWindow();

	void Notify(DuiLib::TNotifyUI& msg);
	LRESULT HandleMessage(UINT uMsg,WPARAM wParam,LPARAM lParam);
public:
	static std::tr1::shared_ptr<CDuiPicture> Show(std::string strFilePath);
	static std::tr1::shared_ptr<CDuiPicture> GetInstance() {	return pInstance;	}
	static std::tr1::shared_ptr<CDuiPicture> pInstance;

	void SetFilePath(std::string strFilePath);//设置图片文件名
	void RecoverPic();//图片还原成1:1

	void AmplificationPic();
	void NarrowPic();
	void SetLabelPicPos(int nWidth, int nHeight);
	void SetPicAngle(int nAngle);
private:
	DuiLib::CLabelUI* m_pLabelPic;//图片区域
	DuiLib::CButtonUI* m_pBtnClose;//关闭按钮
	DuiLib::CHorizontalLayoutUI* m_pHLBtn;//按钮区域
	int m_nZoomIndex;//当前缩放的位置
	std::vector<pair<int, int> > m_vecZoomRect;//每个缩放的高度和宽度
	std::string m_strFilePath;//图片文件名
	BOOL m_bIsLButtonDown;//是否按下鼠标左键
	BOOL m_bIsPicDrag;//是否为图片拖动
	POINT m_pointLast;//上一次按下左键的位置
	int m_nImageAngle;
};

