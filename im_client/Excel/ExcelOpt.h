/*====================================================================  
=  类名:CExcelOpt类                                    
=  修改日期:2015-8-19                                                     
=  作用:用于操作Excel文件
=  使用方法:
=   写入：
=	CExcelOpt excelOpt;
=	excelOpt.OpenExcelBook(strFileName);
=	excelOpt.SetCellValue(nRow, nCol, XlHAlign::xlHAlignGeneral,"...");
=	excelOpt.SaveAsExcel(strFileName);
=   读取：
=   CExcelOpt excelOpt;
=   excelOpt.OpenExcelBook(strFileName);
=   CString str = excelOpt.GetCellValue(nRow,nCol); 
====================================================================*/ 
#pragma once
#include "ExcelApplication.h"
#include "ExcelWorkbooks.h"
#include "ExcelWorkbook.h"
#include "ExcelWorksheets.h"
#include "ExcelWorksheet.h"
#include "ExcelRange.h"

//单元格中显示的样式
enum XlHAlign
{
	xlHAlignCenter = -4108,
	xlHAlignCenterAcrossSelection = 7,
	xlHAlignDistributed = -4117,
	xlHAlignFill = 5,
	xlHAlignGeneral = 1,
	xlHAlignJustify = -4130,
	xlHAlignLeft = -4131,
	xlHAlignRight = -4152
};

class CExcelOpt
{
private:
	CApplication m_excelApplication;
	CWorkbooks m_excelWorkbooks;
	CWorkbook m_excelWorkbook;
	CWorksheets m_excelWorksheets;
	CWorksheet m_excelWorksheet;
	CRange m_excelRange;
	CRange m_excelCell;
public:
	CExcelOpt(void);
	~CExcelOpt(void);

private:
	CString IndexToString( int row, int col );//将行列设置为excel的行列格式
	//下列接口没使用到，暂时设置为私有成员
	void NewExcelBook();
	void OpenExcelApp(void);
	void SaveExcel();
	void SetRowHeight(int row, CString height);
	void SetColumnWidth(int col,CString width);
	CString GetColumnWidth(int col);
	CString GetRowHeight(int row);
public:
	BOOL CheckExcel();//检测Excel是否安装
	bool OpenExcelBook(CString filename);//打开Excel文件
	void SaveAsExcel(CString filename);//保存Excel文件
	void SetCellValue(int row, int col,int Align,CString strValue);//设置一个单元格的值
	CString GetCellValue(int row, int col);//获取一个单元格的值
	int LastLineIndex();//最后一行的行数
	int GetRowCount();//获取行数
};

