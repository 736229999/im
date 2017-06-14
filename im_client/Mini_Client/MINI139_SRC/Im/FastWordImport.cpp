#include "StdAfx.h"
#include "FastWordImport.h"
#include "localdata.h"
#include "..\..\..\Excel\ExcelOpt.h"
#include "..\..\..\Excel\CSVFile.h"
using namespace DuiLib;
std::tr1::shared_ptr<CFastWordImport> CFastWordImport::pInstance = NULL;
extern void UpdateBk(DuiLib::CControlUI* background, HWND hWnd);
CFastWordImport::CFastWordImport(void)
	: m_wndTalkToolBar(&m_reditSend, &m_font, CWnd::FromHandle(GetHWND()))
{
	//m_parent = NULL;
}


CFastWordImport::~CFastWordImport(void)
{
}

LRESULT CFastWordImport::HandleMessage(UINT uMsg,WPARAM wParam,LPARAM lParam)
{
	// 禁止窗口最大化
	if(WM_NCLBUTTONDBLCLK == uMsg)
	{
		return 0;
	}

	return DuiLib::WindowImplBase::HandleMessage(uMsg,wParam,lParam);
}

void CFastWordImport::InitWindow() 
{
	DuiLib::WindowImplBase::InitWindow();
	m_parent = NULL;
	CHorizontalLayoutUI* pHlTalkBar = static_cast<CHorizontalLayoutUI*>(m_PaintManager.FindControl(_T("hltalkbar")));
	pHlTalkBar->SetVisible(false);
	pHlTalkBar->SetVisible(true);
	m_pRedioExcel = static_cast<COptionUI*>(m_PaintManager.FindControl(_T("radioExcel")));
	m_pRedioCsv = static_cast<COptionUI*>(m_PaintManager.FindControl(_T("radioCsv")));
	m_pRedioClear = static_cast<COptionUI*>(m_PaintManager.FindControl(_T("checkClear")));
	SetIcon(IDR_MAINFRAME);
	UpdateBk();
}

void CFastWordImport::UpdateBk()
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

void CFastWordImport::Notify(DuiLib::TNotifyUI& msg)
{
	if(msg.sType == _T("click")) //点击咨询弹出菜单
	{
		if(msg.pSender->GetName() == _T("closebtn")) 
		{

		}
		else if(msg.pSender->GetName() == _T("btncancel"))
		{
			Close();
		}
		else if(msg.pSender->GetName() == _T("templateicon") || msg.pSender->GetName() == _T("templatetext"))
		{
			CFileDialog saveFile = CFileDialog(FALSE, ".xls", "", OFN_CREATEPROMPT | OFN_PATHMUSTEXIST , "表格(*.xls)|*.xls||");  
			CString defaultFileName = "快捷短语模版";  
			saveFile.m_ofn.lpstrFile = defaultFileName.GetBuffer(MAX_PATH);  
			saveFile.m_ofn.nMaxFile = MAX_PATH;  
			if(IDOK == saveFile.DoModal()){  
				if(FileExist(saveFile.GetPathName()))
				{
					if(IDYES == CMessageBox::Model(GetHWND(), "文件已存在，是否覆盖该文件？", "提示", MB_YESNO))
					{
						DeleteFile(saveFile.GetPathName());
						ExportTemplate(saveFile.GetPathName());
					}
				}
				else
					ExportTemplate(saveFile.GetPathName());
			}  
			defaultFileName.ReleaseBuffer();  
		}
		else if(msg.pSender->GetName() == _T("btnok"))
		{
			if(m_pRedioExcel->IsSelected())
			{

				CString sFilter = "快捷短语(*.xls)|*.xls||";
				CFileDialog readFile(TRUE, 
					NULL, 
					NULL, 
					OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT|OFN_NOCHANGEDIR, 
					sFilter,
					CWnd::FromHandle(GetHWND()));
				CString defaultFileName = "快捷短语";  
				readFile.m_ofn.lpstrFile = defaultFileName.GetBuffer(MAX_PATH);  
				readFile.m_ofn.nMaxFile = MAX_PATH;
				if(IDOK == readFile.DoModal())
				{
					if(!FileExist(readFile.GetPathName()))
					{
						CMessageBox::Model(GetHWND(), "文件不存在！", "提示", MB_OK);
						return;
					}
					ImportExcel(readFile.GetPathName());
				}
				defaultFileName.ReleaseBuffer();  
			}
			else if(m_pRedioCsv->IsSelected())
			{
				CString sFilter = "快捷短语(*.csv)|*.csv||";
				CFileDialog readFile(TRUE, 
					NULL, 
					NULL, 
					OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT|OFN_NOCHANGEDIR, 
					sFilter,
					CWnd::FromHandle(GetHWND()));
				CString defaultFileName = "快捷短语";  
				readFile.m_ofn.lpstrFile = defaultFileName.GetBuffer(MAX_PATH);  
				readFile.m_ofn.nMaxFile = MAX_PATH;
				if(IDOK == readFile.DoModal())
				{
					if(!FileExist(readFile.GetPathName()))
					{
						CMessageBox::Model(GetHWND(), "文件不存在！", "提示", MB_OK);
						return;
					}
					ImportCsv(readFile.GetPathName());
				}
				defaultFileName.ReleaseBuffer();  
			}
			Close();
		}
	}
	DuiLib::WindowImplBase::Notify(msg);
}

std::tr1::shared_ptr<CFastWordImport> CFastWordImport::NewDlg(HWND wnd)
{
	// 	if(pInstance2 != NULL && pInstance2->m_hWnd != NULL)
	// 	{
	// 		pInstance2->Close();
	// 		pInstance2 = NULL;
	// 	}
	if(pInstance == NULL || pInstance->m_hWnd == NULL)
	{
		pInstance = tr1::shared_ptr<CFastWordImport>(new CFastWordImport());
		pInstance->Create(wnd, "导入", UI_WNDSTYLE_FRAME, WS_EX_WINDOWEDGE);
		pInstance->CenterWindow();
	}

	pInstance->CenterWindow();
	::BringWindowToTop(*pInstance);
	pInstance->SetParentHwnd(wnd);
	return pInstance;
}

void CFastWordImport::SetParentHwnd(HWND pDlg)
{
	m_parent = pDlg;
}

void CFastWordImport::ExportTemplate(CString strFileName)
{
	CString strFastWordCsvCustom = g_LocalData.GetProgramPath() + "\\ExcelTemplate\\FastWordCustom.xls";
	g_LocalData.CommonCopyFile(strFastWordCsvCustom, strFileName);
}

BOOL CFastWordImport::ImportExcel(CString strFileName)
{
	vector<FastWordGroupInfo> vecFastWordGroupInfo;
	if(!m_pRedioClear->IsSelected())
		vecFastWordGroupInfo = g_LocalData.GetFastWordInfo();
	CExcelOpt excelOpt;
	if(!excelOpt.CheckExcel())
	{
		CMessageBox::Model(GetHWND(), "检测到您的计算机未安装Excel，请安装后重新导出或选择导入csv文件。", "提示", MB_OK);
		return FALSE;
	}
	excelOpt.OpenExcelBook(strFileName);
	if(excelOpt.GetCellValue(1,1) != "快捷编码" || 
		excelOpt.GetCellValue(1,2) != "快捷短语" ||
		excelOpt.GetCellValue(1,3) != "快捷短语分组" )
	{
		return FALSE;
	}
	int nCount = excelOpt.GetRowCount() - 1;
	for(int nFastWord = 0; nFastWord < nCount; nFastWord++)
	{
		CString strNum = excelOpt.GetCellValue(nFastWord+2, 1);
		CString strFastWord = excelOpt.GetCellValue(nFastWord+2, 2);
		CString strGroup = excelOpt.GetCellValue(nFastWord+2, 3);

		//if(strNum == "" && strFastWord == "" && strGroup == "")
		//	continue;

		if(strGroup == "")
			strGroup = "未分组";

		//导入如果超过4个字则取前4个字
		int nMaxChar = 0;
		for(int nChar = 0; nChar < strNum.GetLength(); nChar++)
		{
			BYTE bChar = strNum.GetAt(nChar);
			if(bChar > 127)
			{
				nChar++;
			}
			nMaxChar++;
			if(nMaxChar == 4)
			{
				strNum = strNum.Left(nMaxChar);
				break;
			}
		}
		
		vector<FastWordGroupInfo>::iterator it = std::find(vecFastWordGroupInfo.begin(), vecFastWordGroupInfo.end(), strGroup);
		if(it == vecFastWordGroupInfo.end())
		{
			FastWordGroupInfo fastWordGroupInfo;
			fastWordGroupInfo.strGroupName = strGroup;
			vecFastWordGroupInfo.push_back(fastWordGroupInfo);
			it = vecFastWordGroupInfo.end();
			it--;
		}
		FastWordGroupInfo& fastWordGroupInfo = *it;

		//快捷短语改为不去重
		FastWordInfo fastWordInfo;

		fastWordInfo.strWord = strFastWord;
		fastWordInfo.strNum = strNum;
		fastWordInfo.font = m_font;

		fastWordGroupInfo.vecFastWordInfo.push_back(fastWordInfo);
	}
	g_LocalData.SetFastWordInfo(vecFastWordGroupInfo);
	g_LocalData.SaveFastWordInfo();
	return TRUE;
}

BOOL CFastWordImport::ImportCsv(CString strFileName)
{
	vector<FastWordGroupInfo> vecFastWordGroupInfo;
	if(!m_pRedioClear->IsSelected())
		vecFastWordGroupInfo = g_LocalData.GetFastWordInfo();
	CCSVFile file(strFileName, CCSVFile::modeRead);
	CStringArray arrayHead;
	file.ReadData(arrayHead);

	if(arrayHead.GetSize() < 3)
	{
		return FALSE;
	}
	if(arrayHead.GetAt(0) != "快捷编码" || arrayHead.GetAt(1) != "快捷短语" || arrayHead.GetAt(2) != "快捷短语分组")
	{
		return FALSE;
	}
	CStringArray arrayBody;
	while(file.ReadData(arrayBody))
	{
		CString strNum = "";
		CString strFastWord = "";
		CString strGroup = "";

		for(int nBody = 0; nBody < arrayBody.GetSize(); nBody++)
		{
			if(nBody == 0)
				strNum = arrayBody.GetAt(nBody);
			else if(nBody == 1)
				strFastWord = arrayBody.GetAt(nBody);
			else if(nBody == 2)
				strGroup = arrayBody.GetAt(nBody);
		}

		//if(strNum == "" && strFastWord == "" && strGroup == "")
		//	continue;

		if(strGroup == "")
			strGroup = "未分组";
		strFastWord.Replace("&#13;", "\r");
		strFastWord.Replace("&#44;", ",");
		strGroup.Replace("&#13;", "\r");
		strGroup.Replace("&#44;", ",");

		//导入如果超过4个字则取前4个字
		int nMaxChar = 0;
		for(int nChar = 0; nChar < strNum.GetLength(); nChar++)
		{
			BYTE bChar = strNum.GetAt(nChar);
			if(bChar > 127)
			{
				nChar++;
			}
			nMaxChar++;
			if(nMaxChar == 4)
			{
				strNum = strNum.Left(nMaxChar);
				break;
			}
		}

        //把其他组里的同名的快捷短语先删掉
		for(int nGroup = 0; nGroup < vecFastWordGroupInfo.size(); nGroup++)
		{
			FastWordGroupInfo& fastWordGroupInfo = vecFastWordGroupInfo[nGroup];
			if(strGroup != fastWordGroupInfo.strGroupName)
			{
				vector<FastWordInfo>::iterator itr = std::find(fastWordGroupInfo.vecFastWordInfo.begin(), fastWordGroupInfo.vecFastWordInfo.end(), strFastWord);
				if(itr != fastWordGroupInfo.vecFastWordInfo.end())
				{
					fastWordGroupInfo.vecFastWordInfo.erase(itr);
				}
			}
		}
		BOOL bExist = FALSE;
		vector<FastWordGroupInfo>::iterator it = std::find(vecFastWordGroupInfo.begin(), vecFastWordGroupInfo.end(), strGroup);
		if(it != vecFastWordGroupInfo.end())
		{
			bExist = TRUE;
			FastWordGroupInfo& fastWordGroupInfo = *it;
			BOOL bFastWordExist = FALSE;

			vector<FastWordInfo>::iterator itr = std::find(fastWordGroupInfo.vecFastWordInfo.begin(), fastWordGroupInfo.vecFastWordInfo.end(), strFastWord);
			if(itr != fastWordGroupInfo.vecFastWordInfo.end())
			{
				FastWordInfo &fastWordInfo = *itr;
				fastWordInfo.strNum = strNum;
				fastWordInfo.font = m_font;
				bFastWordExist = TRUE;
			}
			if(!bFastWordExist)
			{
				FastWordInfo fastWordInfo;
				fastWordInfo.strNum = strNum;
				fastWordInfo.strWord = strFastWord;
				fastWordInfo.font = m_font;
				if(strFastWord != "")
					fastWordGroupInfo.vecFastWordInfo.push_back(fastWordInfo);
			}
		}
		if(!bExist)
		{
			FastWordGroupInfo fastWordGroupInfo;
			fastWordGroupInfo.strGroupName = strGroup;

			FastWordInfo fastWordInfo;
			fastWordInfo.strNum = strNum;
			fastWordInfo.strWord = strFastWord;
			fastWordInfo.font = m_font;
			if(strFastWord != "")
				fastWordGroupInfo.vecFastWordInfo.push_back(fastWordInfo);

			if(strGroup == "未分组")
			{
				vector<FastWordGroupInfo>::iterator itr = vecFastWordGroupInfo.begin();
				vecFastWordGroupInfo.insert(itr, fastWordGroupInfo);
			}
			else
				vecFastWordGroupInfo.push_back(fastWordGroupInfo);
		}
	}
	file.Close();
	g_LocalData.SetFastWordInfo(vecFastWordGroupInfo);
	g_LocalData.SaveFastWordInfo();
	return TRUE;
}