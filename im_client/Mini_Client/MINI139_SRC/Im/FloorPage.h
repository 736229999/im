// ----------------------------------------------------------------
// 文件:FloorPage.h
// 版本: 
// 作者: xy
// 日期: 2006-7-6
// ----------------------------------------------------------------
// 版本历史			注释
// ----------------------------------------------------------------
// v0.1				初始版本
// 
// ----------------------------------------------------------------


#pragma once

#include "../resource.h"
#include "impublic.h"


//------------------------------------------------------
// 主界面中滑动窗体基类
//------------------------------------------------------

class CFloorPage : public CDialog
{
	DECLARE_DYNAMIC(CFloorPage)

public:
	CFloorPage(UINT nIDTemplate, CWnd* pParentWnd = NULL);
	virtual ~CFloorPage();

public:
	//------------------------------------------------------
	// 转换消息中包含的特殊表达式，并提取需要下载的任务,如果
	// 没有表达式，返回false
	//------------------------------------------------------
	static bool TranslateMsg(
		/*IN*/const CString & sMsg,
		/*OUT*/CString & sShowMsg,
		/*OUT*/vector<DOWNLOAD_OBJ> & arrDownloadTask
	);

protected:

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};
