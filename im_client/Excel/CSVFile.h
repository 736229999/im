/*====================================================================  
=  ����:CCSVFile                                    
=  �޸�����:2015-8-19                                                     
=  ����:���ڲ���Csv�ļ�
=  ʹ�÷���:
=   д�룺
=	CCSVFile file(strFileName, CCSVFile::modeWrite);
=   CStringArray array;
=   array.Add("1");
=   array.Add("2");
=   file.WriteData(array);
=   ��ȡ��
=	CCSVFile file(strFileName, CCSVFile::modeRead);
=   CStringArray array;
=   while(file.ReadData(array))
=   {
=       if(array.GetSize() > 0)
=           ...
=   }
====================================================================*/ 
#pragma once
#include "afx.h"

class CCSVFile : public CStdioFile
{
public:
	enum Mode { modeRead, modeWrite };
	CCSVFile(LPCTSTR lpszFilename, Mode mode = modeRead);
	~CCSVFile(void);

	bool ReadData(CStringArray &arr);//��ȡһ������
	void WriteData(CStringArray &arr);//д��һ������

#ifdef _DEBUG
	Mode m_nMode;

#endif
};
