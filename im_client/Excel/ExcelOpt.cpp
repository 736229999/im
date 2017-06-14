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

	//��֤office�ļ��Ƿ������ȷ����  

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
///Description: �򿪱���Ϊfilename���ļ���ע�⣬�ļ�·�����Զ����ɣ��Ժ�  
///             ���Ǵ��´�����  
///Call:        GetAppPath()  
///Input:       CString filename �ļ���  
//////////////////////////////////////////////////////////////////////// 
bool CExcelOpt::OpenExcelBook(CString filename)  
{  
 	LPDISPATCH lpDisp; //�ӿ�ָ��  
 	m_excelWorkbooks=m_excelApplication.get_Workbooks();  

	CString strBookPath = filename;
	try
	{
		/*��һ��������*/
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
		/*����һ���µĹ�����*/
		lpDisp = m_excelWorkbooks.Add(covOptional);
		m_excelWorkbook.AttachDispatch(lpDisp);
	}

	m_excelWorksheets=m_excelWorkbook.get_Sheets();  
	m_excelWorksheet=m_excelWorksheets.get_Item(COleVariant((short)1));        //��Ĳ�ͬ���Ǹ������ģ�ֱ���ں����������covOptional�ɹ���  
	return true;  
}  

void CExcelOpt::NewExcelBook()  
{  
	m_excelWorkbooks=m_excelApplication.get_Workbooks();  
	m_excelWorkbook=m_excelWorkbooks.Add(covOptional);  
	m_excelWorksheets=m_excelWorkbook.get_Sheets();  
	m_excelWorksheet=m_excelWorksheets.get_Item(COleVariant((short)1));        //��Ĳ�ͬ���Ǹ������ģ�ֱ���ں����������covOptional�ɹ���  
}

////////////////////////////////////////////////////////////////////////  
///Function:    OpenExcelApp  
///Description: ��Ӧ�ó���Ҫע���Ժ����ʶ���û�Ҫ�򿪵����ĸ��ļ���  
////////////////////////////////////////////////////////////////////////  
void CExcelOpt::OpenExcelApp(void)  
{  
	m_excelApplication.put_Visible(TRUE);  
	m_excelApplication.put_UserControl(TRUE);  
}  

////////////////////////////////////////////////////////////////////////  
///Function:    SaveExcel  
///Description: ���ڴ������ļ����������ݺ�ֱ�ӱ���  
////////////////////////////////////////////////////////////////////////  
void CExcelOpt::SaveExcel()
{  
	m_excelWorkbook.put_Saved(TRUE);  
}  

////////////////////////////////////////////////////////////////////////  
///Function:    SaveAsExcel  
///Description: ����excel�ļ�  
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
///Description: �޸ĵ�Ԫ���ڵ�ֵ  
///Call:        IndexToString() ��(x,y)������ʽת��Ϊ��A1����ʽ�ַ���  
///Input:       int row ��Ԫ��������  
///             int col ��Ԫ��������  
///             int Align       ���뷽ʽĬ��Ϊ����  
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
///Description: �õ��ĵ�Ԫ���е�ֵ  
///Call:        IndexToString() ��(x,y)������ʽת��Ϊ��A1����ʽ�ַ���  
///Input:       int row ��Ԫ��������  
///             int col ��Ԫ��������  
///Return:      CString ��Ԫ���е�ֵ  
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
///Description: �����и�  
///Call:        IndexToString() ��(x,y)������ʽת��Ϊ��A1����ʽ�ַ���  
///Input:       int row ��Ԫ��������  
////////////////////////////////////////////////////////////////////////  
void CExcelOpt::SetRowHeight(int row, CString height)  
{  
	int col = 1;  
	m_excelRange=m_excelWorksheet.get_Range(COleVariant(IndexToString(row,col)),COleVariant(IndexToString(row,col)));  
	m_excelRange.put_RowHeight(COleVariant(height));  
}  
////////////////////////////////////////////////////////////////////////  
///Function:    SetColumnWidth  
///Description: �����п�  
///Call:        IndexToString() ��(x,y)������ʽת��Ϊ��A1����ʽ�ַ���  
///Input:       int col Ҫ�����п����  
///             CString ��ֵ  
////////////////////////////////////////////////////////////////////////  
void CExcelOpt::SetColumnWidth(int col,CString width)  
{  
	int row = 1;  
	m_excelRange=m_excelWorksheet.get_Range(COleVariant(IndexToString(row,col)),COleVariant(IndexToString(row,col)));  
	m_excelRange.put_ColumnWidth(COleVariant(width));  
}  

////////////////////////////////////////////////////////////////////////  
///Function:    SetRowHeight  
///Description: �����и�  
///Call:        IndexToString() ��(x,y)������ʽת��Ϊ��A1����ʽ�ַ���  
///Input:       int row ��Ԫ��������  
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
///Description: �����и�  
///Call:        IndexToString() ��(x,y)������ʽת��Ϊ��A1����ʽ�ַ���  
///Input:       int row Ҫ�����иߵ���  
///             CString ��ֵ  
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
///Description: �õ��ĵ�Ԫ����EXCEL�еĶ�λ�����ַ���  
///Input:       int row ��Ԫ��������  
///             int col ��Ԫ��������  
///Return:      CString ��Ԫ����EXCEL�еĶ�λ�����ַ���  
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
///Description: �õ�����ܵ�һ�����е�����  
///Return:      int ���е�������  
////////////////////////////////////////////////////////////////////////  
int CExcelOpt::LastLineIndex()   
{   
	int i,j,flag=0;  
	CString str;  
	for(i=1;;i++)  
	{  
		flag = 0;  
		//����ͳ�ƣ���Ϊǰ�ж�û�����ݼ�Ϊ����  
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