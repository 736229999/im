/*====================================================================  
=  ����:CExcelOpt��                                    
=  �޸�����:2015-8-19                                                     
=  ����:���ڲ���Excel�ļ�
=  ʹ�÷���:
=   д�룺
=	CExcelOpt excelOpt;
=	excelOpt.OpenExcelBook(strFileName);
=	excelOpt.SetCellValue(nRow, nCol, XlHAlign::xlHAlignGeneral,"...");
=	excelOpt.SaveAsExcel(strFileName);
=   ��ȡ��
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

//��Ԫ������ʾ����ʽ
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
	CString IndexToString( int row, int col );//����������Ϊexcel�����и�ʽ
	//���нӿ�ûʹ�õ�����ʱ����Ϊ˽�г�Ա
	void NewExcelBook();
	void OpenExcelApp(void);
	void SaveExcel();
	void SetRowHeight(int row, CString height);
	void SetColumnWidth(int col,CString width);
	CString GetColumnWidth(int col);
	CString GetRowHeight(int row);
public:
	BOOL CheckExcel();//���Excel�Ƿ�װ
	bool OpenExcelBook(CString filename);//��Excel�ļ�
	void SaveAsExcel(CString filename);//����Excel�ļ�
	void SetCellValue(int row, int col,int Align,CString strValue);//����һ����Ԫ���ֵ
	CString GetCellValue(int row, int col);//��ȡһ����Ԫ���ֵ
	int LastLineIndex();//���һ�е�����
	int GetRowCount();//��ȡ����
};

