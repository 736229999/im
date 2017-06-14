// ----------------------------------------------------------------
// �ļ�:ActionTextReader.h
// �汾: 
// ����: xy
// ����: 2006-6-13
// ----------------------------------------------------------------
// �汾��ʷ			ע��
// ----------------------------------------------------------------
// v0.1				��ʼ�汾
// 
// ----------------------------------------------------------------

#pragma once


// �������ʽ����

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

// ��������

struct ACTION_ITEM{
	CString		sName;
	CString		sAction0;
	CString		sAction1;
	CString		sAction2;
	CString		sMsg;		// �Ի���Ϣ
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
// �����ű�������
//------------------------------------------------------

class CActionTextReader
{
public:

	CActionTextReader(void);

	~CActionTextReader(void);

public:
	//------------------------------------------------------
	// �Ӷ����ű��ļ�����
	//------------------------------------------------------
	BOOL	LoadActionText(LPCTSTR szFile);
	//------------------------------------------------------
	// ����ѽ����Ķ����ű�����
	//------------------------------------------------------
	const vector<ACTION_ITEM>	& GetActionList() const
	{ return m_actionList;	};
	//------------------------------------------------------
	// �������ж����ű�
	//------------------------------------------------------
	bool ParseLine(const string& sLine,ACTION_ITEM& ai);

protected:

	vector<ACTION_ITEM>	m_actionList;

};
