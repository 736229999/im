#include "StdAfx.h"
#include "FastWordExport.h"
#include "..\..\..\Excel\ExcelOpt.h"
#include "..\..\..\Excel\CSVFile.h"
#include "localdata.h"
using namespace DuiLib;
std::tr1::shared_ptr<CFastWordExport> CFastWordExport::pInstance = NULL;
extern void UpdateBk(DuiLib::CControlUI* background, HWND hWnd);
CFastWordExport::CFastWordExport(void)
{
	//m_parent = NULL;
}


CFastWordExport::~CFastWordExport(void)
{
}

LRESULT CFastWordExport::HandleMessage(UINT uMsg,WPARAM wParam,LPARAM lParam)
{
	// ��ֹ�������
	if(WM_NCLBUTTONDBLCLK == uMsg)
	{
		return 0;
	}

	return DuiLib::WindowImplBase::HandleMessage(uMsg,wParam,lParam);
}

void CFastWordExport::InitWindow() 
{
	DuiLib::WindowImplBase::InitWindow();
	m_parent = NULL;
	m_pRedioExcel = static_cast<COptionUI*>(m_PaintManager.FindControl(_T("radioExcel")));
	m_pRedioCsv = static_cast<COptionUI*>(m_PaintManager.FindControl(_T("radioCsv")));
	SetIcon(IDR_MAINFRAME);
	UpdateBk();
}

void CFastWordExport::UpdateBk()
{
	if(pInstance != NULL && ::IsWindow(pInstance->GetHWND()))
	{
		DuiLib::CControlUI* background = pInstance->m_PaintManager.FindControl("background");
		if (background != NULL)
		{
			::UpdateBk(background, pInstance->GetHWND());
		}
	}
}

void CFastWordExport::Notify(DuiLib::TNotifyUI& msg)
{
	if(msg.sType == _T("click")) //�����ѯ�����˵�
	{
		if(msg.pSender->GetName() == _T("closebtn")) 
		{

		}
		else if(msg.pSender->GetName() == _T("btncancel"))
		{
			Close();
		}
		else if(msg.pSender->GetName() == _T("btnok"))
		{
			if(m_pRedioExcel->IsSelected())
			{
				CFileDialog saveFile = CFileDialog(FALSE, ".xls", "", OFN_CREATEPROMPT | OFN_PATHMUSTEXIST , "���(*.xls)|*.xls||");  
				CString defaultFileName = "��ݶ���";  
				saveFile.m_ofn.lpstrFile = defaultFileName.GetBuffer(MAX_PATH);  
				saveFile.m_ofn.nMaxFile = MAX_PATH;  
				if(IDOK == saveFile.DoModal()){  
					if(FileExist(saveFile.GetPathName()))
					{
						if(IDYES == CMessageBox::Model(GetHWND(), "�ļ��Ѵ��ڣ��Ƿ񸲸Ǹ��ļ���", "��ʾ", MB_YESNO))
						{
							DeleteFile(saveFile.GetPathName());
							ExportExcel(saveFile.GetPathName());
						}
					}
					else
						ExportExcel(saveFile.GetPathName());
				}  
				defaultFileName.ReleaseBuffer();  
			}
			else if(m_pRedioCsv->IsSelected())
			{
				CFileDialog saveFile = CFileDialog(FALSE, ".csv", "", OFN_CREATEPROMPT | OFN_PATHMUSTEXIST , "���(*.csv)|*.csv||");  
				CString defaultFileName = "��ݶ���";  
				saveFile.m_ofn.lpstrFile = defaultFileName.GetBuffer(MAX_PATH);  
				saveFile.m_ofn.nMaxFile = MAX_PATH;  
				if(IDOK == saveFile.DoModal()){  
					if(FileExist(saveFile.GetPathName()))
					{
						if(IDYES == CMessageBox::Model(GetHWND(), "�ļ��Ѵ��ڣ��Ƿ񸲸Ǹ��ļ���", "��ʾ", MB_YESNO))
						{
							DeleteFile(saveFile.GetPathName());
							ExportCsv(saveFile.GetPathName());
						}
					}
					else
						ExportCsv(saveFile.GetPathName());
				}  
				defaultFileName.ReleaseBuffer();  
			}
			Close();
		}
	}
	DuiLib::WindowImplBase::Notify(msg);
}

std::tr1::shared_ptr<CFastWordExport> CFastWordExport::NewDlg(HWND wnd)
{
	// 	if(pInstance2 != NULL && pInstance2->m_hWnd != NULL)
	// 	{
	// 		pInstance2->Close();
	// 		pInstance2 = NULL;
	// 	}
	if(pInstance == NULL || pInstance->m_hWnd == NULL)
	{
		pInstance = tr1::shared_ptr<CFastWordExport>(new CFastWordExport());
		pInstance->Create(wnd, "����", UI_WNDSTYLE_FRAME, WS_EX_WINDOWEDGE);
		pInstance->CenterWindow();
	}

	pInstance->CenterWindow();
	::BringWindowToTop(*pInstance);
	pInstance->SetParentHwnd(wnd);
	return pInstance;
}

void CFastWordExport::SetParentHwnd(HWND pDlg)
{
	m_parent = pDlg;
}

void CFastWordExport::ExportExcel(CString strFileName)
{
	CExcelOpt excelOpt;
	if(!excelOpt.CheckExcel())
	{
		CMessageBox::Model(GetHWND(), "��⵽���ļ����δ��װExcel���밲װ�����µ�����ѡ�񵼳�csv�ļ���", "��ʾ", MB_OK);
		return;
	}
	excelOpt.OpenExcelBook(strFileName);
	vector<FastWordGroupInfo> vecWordGroupInfo = g_LocalData.GetFastWordInfo();
	int nRow = 1;
	int nCol = 1;
	excelOpt.SetCellValue(nRow, nCol++, XlHAlign::xlHAlignGeneral,"��ݱ���");
	excelOpt.SetCellValue(nRow, nCol++, XlHAlign::xlHAlignGeneral,"��ݶ���");
	excelOpt.SetCellValue(nRow, nCol++, XlHAlign::xlHAlignGeneral,"��ݶ������");
	nRow++;
	nCol = 1;
	for(int nGroup = 0; nGroup < vecWordGroupInfo.size(); nGroup++)
	{
		FastWordGroupInfo fastWordGroupInfo = vecWordGroupInfo[nGroup];
		for(int nFastWord = 0; nFastWord < fastWordGroupInfo.vecFastWordInfo.size(); nFastWord++)
		{
			FastWordInfo& fastWordInfo = fastWordGroupInfo.vecFastWordInfo[nFastWord];

			CString strNum = fastWordInfo.strNum;
			CString strWord = fastWordInfo.strWord;
			CString strGroupName = fastWordGroupInfo.strGroupName;
			if(strGroupName == "δ����")
				strGroupName = "";
			excelOpt.SetCellValue(nRow, nCol++, XlHAlign::xlHAlignGeneral,strNum);
			excelOpt.SetCellValue(nRow, nCol++, XlHAlign::xlHAlignGeneral,strWord);
			excelOpt.SetCellValue(nRow, nCol++, XlHAlign::xlHAlignGeneral,strGroupName);
			nRow++;
			nCol = 1;
		}
		if(fastWordGroupInfo.vecFastWordInfo.size() == 0)
		{
			CString strNum = "";
			CString strWord = "";
			CString strGroupName = fastWordGroupInfo.strGroupName;
			if(strGroupName == "δ����")
				strGroupName = "";
			excelOpt.SetCellValue(nRow, nCol++, XlHAlign::xlHAlignGeneral,strNum);
			excelOpt.SetCellValue(nRow, nCol++, XlHAlign::xlHAlignGeneral,strWord);
			excelOpt.SetCellValue(nRow, nCol++, XlHAlign::xlHAlignGeneral,strGroupName);
			nRow++;
			nCol = 1;
		}
	}
	excelOpt.SaveAsExcel(strFileName);
}

void CFastWordExport::ExportCsv(CString strFileName)
{
	CCSVFile file(strFileName, CCSVFile::modeWrite);
	vector<FastWordGroupInfo> vecWordGroupInfo = g_LocalData.GetFastWordInfo();
	CStringArray arrayHead;
	arrayHead.Add("��ݱ���");
	arrayHead.Add("��ݶ���");
	arrayHead.Add("��ݶ������");
	file.WriteData(arrayHead);
	for(int nGroup = 0; nGroup < vecWordGroupInfo.size(); nGroup++)
	{
		FastWordGroupInfo fastWordGroupInfo = vecWordGroupInfo[nGroup];
		for(int nFastWord = 0; nFastWord < fastWordGroupInfo.vecFastWordInfo.size(); nFastWord++)
		{
			FastWordInfo& fastWordInfo = fastWordGroupInfo.vecFastWordInfo[nFastWord];

			CString strNum = fastWordInfo.strNum;
			CString strWord = fastWordInfo.strWord;
			CString strGroupName = fastWordGroupInfo.strGroupName;
			if(strGroupName == "δ����")
				strGroupName = "";

			strWord.Replace("\r\n", "&#13;");
			strWord.Replace("\n", "&#13;");
			strWord.Replace("\r", "&#13;");
			strWord.Replace(",", "&#44;");
			strGroupName.Replace("\r\n", "&#13;");
			strGroupName.Replace("\n", "&#13;");
			strGroupName.Replace("\r", "&#13;");
			strGroupName.Replace(",", "&#44;");

			CStringArray arrayBody;
			arrayBody.Add(strNum);
			arrayBody.Add(strWord);
			arrayBody.Add(strGroupName);
			file.WriteData(arrayBody);
		}
		if(fastWordGroupInfo.vecFastWordInfo.size() == 0)
		{
			CString strNum = "";
			CString strWord = "";
			CString strGroupName = fastWordGroupInfo.strGroupName;
			if(strGroupName == "δ����")
				strGroupName = "";
			strGroupName.Replace("\r\n", "&#13;");
			strGroupName.Replace("\n", "&#13;");
			strGroupName.Replace("\r", "&#13;");
			strGroupName.Replace(",", "&#44;");
			CStringArray arrayBody;
			arrayBody.Add(strNum);
			arrayBody.Add(strWord);
			arrayBody.Add(strGroupName);
			file.WriteData(arrayBody);
		}
	}
	file.Close();
}