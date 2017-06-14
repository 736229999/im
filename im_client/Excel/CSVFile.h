/*====================================================================  
=  类名:CCSVFile                                    
=  修改日期:2015-8-19                                                     
=  作用:用于操作Csv文件
=  使用方法:
=   写入：
=	CCSVFile file(strFileName, CCSVFile::modeWrite);
=   CStringArray array;
=   array.Add("1");
=   array.Add("2");
=   file.WriteData(array);
=   读取：
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

	bool ReadData(CStringArray &arr);//读取一行数据
	void WriteData(CStringArray &arr);//写入一行数据

#ifdef _DEBUG
	Mode m_nMode;

#endif
};
