// ----------------------------------------------------------------
// 文件:ActionTextReader.h
// 版本: 
// 作者: xy
// 日期: 2006-6-13
// ----------------------------------------------------------------
// 版本历史			注释
// ----------------------------------------------------------------
// v0.1				初始版本
// 
// ----------------------------------------------------------------

#pragma once


// 动作表达式数据

struct ACTION_DATA{
	CString		sObject;
	CString		sAction0;
	CString		sAction1;
	CString		sAction2;
	CString		sMsg;
	int			id;
	void		Clear()
	{
		sObject = "";
		sAction0 = "";
		sAction1 = "";
		sAction2 = "";
		sMsg = "";
		id = 0;
	}
	CString		ToStr() const
	{
		return CString("{\\"+sAction0+"[]"+sAction1+"[\\"+sObject+"]"+sAction2+"\\}");
	};
};

// 动作数据

struct ACTION_ITEM{
	CString		sName;
	CString		sAction0;
	CString		sAction1;
	CString		sAction2;
	CString		sMsg;		// 对话信息
	void		Clear()
	{
		sName = "";
		sAction0 = "";
		sAction1 = "";
		sAction2 = "";
		sMsg = "";
	}
};


//------------------------------------------------------
// 动作脚本解析器
//------------------------------------------------------

class CActionTextReader
{
public:

	CActionTextReader(void);

	~CActionTextReader(void);

public:
	//------------------------------------------------------
	// 从动作脚本文件加载
	//------------------------------------------------------
	BOOL	LoadActionText(LPCTSTR szFile);
	//------------------------------------------------------
	// 获得已解析的动作脚本数据
	//------------------------------------------------------
	const vector<ACTION_ITEM>	& GetActionList() const
	{ return m_actionList;	};
	//------------------------------------------------------
	// 解析单行动作脚本
	//------------------------------------------------------
	bool ParseLine(const string& sLine,ACTION_ITEM& ai);

protected:

	vector<ACTION_ITEM>	m_actionList;

};
