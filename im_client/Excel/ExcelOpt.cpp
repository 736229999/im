#include "StdAfx.h"
#include "ExcelOpt.h"

COleVariant covOptional((long)DISP_E_PARAMNOTFOUND, VT_ERROR);  


CExcelOpt::CExcelOpt(void)
{

}

BOOL CExcelOpt::CheckExcel()
{
	if (::CoInitialize( NULL ) == E_INVALIDARG)   
	{    
		return FALSE;  
	}  

	//验证office文件是否可以正确运行  

	if( !m_excelApplication.CreateDispatch(_T("Excel.Application")) )  
	{  
		return FALSE;  
	}  

	return TRUE;
}

CExcelOpt::~CExcelOpt(void)
{
	m_excelWorkbooks.ReleaseDispatch();  
	m_excelWorkbook.ReleaseDispatch();  
	m_excelWorksheets.ReleaseDispatch();  
	m_excelWorksheet.ReleaseDispatch();  
	m_excelRange.ReleaseDispatch();   
	m_excelCell.ReleaseDispatch();  

	m_excelApplication.Quit();  

	m_excelApplication.ReleaseDispatch();  
	//::CoUninitialize();  
}

////////////////////////////////////////////////////////////////////////  
///Function:    OpenExcelBook  
///Description: 打开表名为filename的文件，注意，文件路径非自动生成，以后  
///             考虑从下处理方法  
///Call:        GetAppPath()  
///Input:       CString filename 文件名  
//////////////////////////////////////////////////////////////////////// 
bool CExcelOpt::OpenExcelBook(CString filename)  
{  
 	LPDISPATCH lpDisp; //接口指针  
 	m_excelWorkbooks=m_excelApplication.get_Workbooks();  

	CString strBookPath = filename;
	try
	{
		/*打开一个工作簿*/
		lpDisp = m_excelWorkbooks.Open(filename,  
			 		covOptional, covOptional, covOptional, covOptional,  
			 		covOptional, covOptional, covOptional, covOptional,  
			 		covOptional, covOptional, covOptional, covOptional,  
			 		covOptional, covOptional  
			 		);  
		m_excelWorkbook.AttachDispatch(lpDisp);
	}
	catch(...)
	{
		/*增加一个新的工作簿*/
		lpDisp = m_excelWorkbooks.Add(covOptional);
		m_excelWorkbook.AttachDispatch(lpDisp);
	}

	m_excelWorksheets=m_excelWorkbook.get_Sheets();  
	m_excelWorksheet=m_excelWorksheets.get_Item(COleVariant((short)1));        //与的不同，是个参数的，直接在后面加了两个covOptional成功了  
	return true;  
}  

void CExcelOpt::NewExcelBook()  
{  
	m_excelWorkbooks=m_excelApplication.get_Workbooks();  
	m_excelWorkbook=m_excelWorkbooks.Add(covOptional);  
	m_excelWorksheets=m_excelWorkbook.get_Sheets();  
	m_excelWorksheet=m_excelWorksheets.get_Item(COleVariant((short)1));        //与的不同，是个参数的，直接在后面加了两个covOptional成功了  
}

////////////////////////////////////////////////////////////////////////  
///Function:    OpenExcelApp  
///Description: 打开应用程序（要注意以后如何识别用户要打开的是哪个文件）  
////////////////////////////////////////////////////////////////////////  
void CExcelOpt::OpenExcelApp(void)  
{  
	m_excelApplication.put_Visible(TRUE);  
	m_excelApplication.put_UserControl(TRUE);  
}  

////////////////////////////////////////////////////////////////////////  
///Function:    SaveExcel  
///Description: 用于打开数据文件，续存数据后直接保存  
////////////////////////////////////////////////////////////////////////  
void CExcelOpt::SaveExcel()
{  
	m_excelWorkbook.put_Saved(TRUE);  
}  

////////////////////////////////////////////////////////////////////////  
///Function:    SaveAsExcel  
///Description: 保存excel文件  
////////////////////////////////////////////////////////////////////////  
void CExcelOpt::SaveAsExcel(CString filename)  
{  
	m_excelWorkbook.SaveAs(COleVariant(filename),covOptional,  
		covOptional,covOptional,  
		covOptional,covOptional,(long)0,covOptional,covOptional,covOptional,  
		covOptional,covOptional);                     
}  


////////////////////////////////////////////////////////////////////////  
///Function:    SetCellValue  
///Description: 修改单元格内的值  
///Call:        IndexToString() 从(x,y)坐标形式转化为“A1”格式字符串  
///Input:       int row 单元格所在行  
///             int col 单元格所在列  
///             int Align       对齐方式默认为居中  
////////////////////////////////////////////////////////////////////////  
void CExcelOpt::SetCellValue(int row, int col,int Align,CString strValue)  
{  
	m_excelRange=m_excelWorksheet.get_Range(COleVariant(IndexToString(row,col)),COleVariant(IndexToString(row,col)));  
	m_excelRange.put_Value2(COleVariant(strValue));  
	m_excelCell.AttachDispatch((m_excelRange.get_Item(COleVariant(long(1)), COleVariant(long(1)))).pdispVal);  
	m_excelCell.put_HorizontalAlignment(COleVariant((short)Align));  
}  

////////////////////////////////////////////////////////////////////////  
///Function:    GetCellValue  
///Description: 得到的单元格中的值  
///Call:        IndexToString() 从(x,y)坐标形式转化为“A1”格式字符串  
///Input:       int row 单元格所在行  
///             int col 单元格所在列  
///Return:      CString 单元格中的值  
////////////////////////////////////////////////////////////////////////  
CString CExcelOpt::GetCellValue(int row, int col)  
{  
	m_excelRange=m_excelWorksheet.get_Range(COleVariant(IndexToString(row,col)),COleVariant(IndexToString(row,col)));  
	COleVariant rValue;  
	rValue=COleVariant(m_excelRange.get_Value2());  
	rValue.ChangeType(VT_BSTR);  
	return CString(rValue.bstrVal);  
}  
////////////////////////////////////////////////////////////////////////  
///Function:    SetRowHeight  
///Description: 设置行高  
///Call:        IndexToString() 从(x,y)坐标形式转化为“A1”格式字符串  
///Input:       int row 单元格所在行  
////////////////////////////////////////////////////////////////////////  
void CExcelOpt::SetRowHeight(int row, CString height)  
{  
	int col = 1;  
	m_excelRange=m_excelWorksheet.get_Range(COleVariant(IndexToString(row,col)),COleVariant(IndexToString(row,col)));  
	m_excelRange.put_RowHeight(COleVariant(height));  
}  
////////////////////////////////////////////////////////////////////////  
///Function:    SetColumnWidth  
///Description: 设置列宽  
///Call:        IndexToString() 从(x,y)坐标形式转化为“A1”格式字符串  
///Input:       int col 要设置列宽的列  
///             CString 宽值  
////////////////////////////////////////////////////////////////////////  
void CExcelOpt::SetColumnWidth(int col,CString width)  
{  
	int row = 1;  
	m_excelRange=m_excelWorksheet.get_Range(COleVariant(IndexToString(row,col)),COleVariant(IndexToString(row,col)));  
	m_excelRange.put_ColumnWidth(COleVariant(width));  
}  

////////////////////////////////////////////////////////////////////////  
///Function:    SetRowHeight  
///Description: 设置行高  
///Call:        IndexToString() 从(x,y)坐标形式转化为“A1”格式字符串  
///Input:       int row 单元格所在行  
////////////////////////////////////////////////////////////////////////  
CString CExcelOpt::GetColumnWidth(int col)  
{  
	int row = 1;  
	m_excelRange=m_excelWorksheet.get_Range(COleVariant(IndexToString(row,col)),COleVariant(IndexToString(row,col)));  
	VARIANT width = m_excelRange.get_ColumnWidth();  
	CString strwidth;  
	strwidth.Format(CString((LPCSTR)(_bstr_t)(_variant_t)width));  
	return strwidth;  
}  

////////////////////////////////////////////////////////////////////////  
///Function:    GetRowHeight  
///Description: 设置行高  
///Call:        IndexToString() 从(x,y)坐标形式转化为“A1”格式字符串  
///Input:       int row 要设置行高的行  
///             CString 宽值  
////////////////////////////////////////////////////////////////////////  
CString CExcelOpt::GetRowHeight(int row)  
{  
	int col = 1;  
	m_excelRange=m_excelWorksheet.get_Range(COleVariant(IndexToString(row,col)),COleVariant(IndexToString(row,col)));  
	VARIANT height = m_excelRange.get_RowHeight();  
	CString strheight;  
	strheight.Format(CString((LPCSTR)(_bstr_t)(_variant_t)height));  
	return strheight;  
}  


////////////////////////////////////////////////////////////////////////  
///Function:    IndexToString  
///Description: 得到的单元格在EXCEL中的定位名称字符串  
///Input:       int row 单元格所在行  
///             int col 单元格所在列  
///Return:      CString 单元格在EXCEL中的定位名称字符串  
////////////////////////////////////////////////////////////////////////  
CString CExcelOpt::IndexToString( int row, int col )   
{   
	CString strResult;  
	if( col > 26 )   
	{   
		strResult.Format(_T("%c%c%d"),'A' + (col-1)/26-1,'A' + (col-1)%26,row);  
	}   
	else   
	{   
		strResult.Format(_T("%c%d"), 'A' + (col-1)%26,row);  
	}   
	return strResult;  
}   

////////////////////////////////////////////////////////////////////////  
///Function:    LastLineIndex  
///Description: 得到表格总第一个空行的索引  
///Return:      int 空行的索引号  
////////////////////////////////////////////////////////////////////////  
int CExcelOpt::LastLineIndex()   
{   
	int i,j,flag=0;  
	CString str;  
	for(i=1;;i++)  
	{  
		flag = 0;  
		//粗略统计，认为前列都没有数据即为空行  
		for(j=1;j<=5;j++)  
		{  
			str.Format(_T("%s"),this->GetCellValue(i,j).Trim());  
			if(str.Compare(_T(""))!=0)  
			{  
				flag = 1;  
				break;  
			}  

		}  
		if(flag==0)  
			return i;  

	}  
}  

int CExcelOpt::GetRowCount()  
{  
	CRange range;  
	CRange usedRange;  
	usedRange.AttachDispatch(m_excelWorksheet.get_UsedRange(), true);  
	range.AttachDispatch(usedRange.get_Rows(), true);  
	int count = range.get_Count();  
	usedRange.ReleaseDispatch();  
	range.ReleaseDispatch();  
	return count;  
}  