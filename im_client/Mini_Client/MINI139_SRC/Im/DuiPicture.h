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

	void SetFilePath(std::string strFilePath);//����ͼƬ�ļ���
	void RecoverPic();//ͼƬ��ԭ��1:1

	void AmplificationPic();
	void NarrowPic();
	void SetLabelPicPos(int nWidth, int nHeight);
	void SetPicAngle(int nAngle);
private:
	DuiLib::CLabelUI* m_pLabelPic;//ͼƬ����
	DuiLib::CButtonUI* m_pBtnClose;//�رհ�ť
	DuiLib::CHorizontalLayoutUI* m_pHLBtn;//��ť����
	int m_nZoomIndex;//��ǰ���ŵ�λ��
	std::vector<pair<int, int> > m_vecZoomRect;//ÿ�����ŵĸ߶ȺͿ��
	std::string m_strFilePath;//ͼƬ�ļ���
	BOOL m_bIsLButtonDown;//�Ƿ���������
	BOOL m_bIsPicDrag;//�Ƿ�ΪͼƬ�϶�
	POINT m_pointLast;//��һ�ΰ��������λ��
	int m_nImageAngle;
};

