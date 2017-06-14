//====================================================================
//  ����һ��duilib���Զ���ؼ��ļ����ӣ�������duilib��������ѧϰ��ô���Զ���ؼ�
//  �����������ṷ����ģ�����ÿһ������ͼ��Ŀ�Ŀؼ����õ����Զ�������
//  �������ʣ�������ϵ�ң�Redrain  QQ��491646717
//  ��������Ƶ����󣬿����޸������
//====================================================================


#ifndef SKIN_PICKER_PICTURE_ITEM_H
#define SKIN_PICKER_PICTURE_ITEM_H

#include "..\..\..\..\DuiLib\UIlib.h"


//xml sample:<SkinPikerPictureItem name="" width="118" height="70" bkimage="UI\BKImage\1small.jpg" bkname="����" author="Redrain" />
//�����ͽӿ�������CreateControl�����л��õ�
const TCHAR kSkinPickerPictureItemClassName[] =	_T("SkinPikerPictureItemUI");
const TCHAR kSkinPickerPictureItemInterface[] =	_T("SkinPikerPictureItem");

//��ɫ��ǰ��ͼ��λ��
const TCHAR kSkinPickerPictureItemForeImage[] =	_T("file='UI\\LeftTab\\listitem\\ListBk.png' fade='150'");

//�߿����ɫ��ͼƬ���Ƶ�������ɫ��������Ϣ��������ɫ
const DWORD kBorderColor					  = 0xFF546772 ;//0xFF64B0FA;
const DWORD kBkNameColor					  = 0xFFFFFFFF;
const DWORD kAuthorColor					  = 0xFFAAAAAA;

class CSkinPikerPictureItemUI : public DuiLib::CButtonUI
{
public:
	CSkinPikerPictureItemUI();

	LPCTSTR GetClass() const;
	LPVOID GetInterface(LPCTSTR pstrName);

	void EraseAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);
	void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);
	void PaintStatusImage(HDC hDC);
    CString GetChooseTips();
private:
	DuiLib::CDuiString m_BkName;
	DuiLib::CDuiString m_Author;
	DuiLib::CDuiString m_ChooseTips;
};

#endif // SKIN_PICKER_PICTURE_ITEM_H