#include "StdAfx.h"
#include ".\actiontextreader.h"
#include <fstream>
#include <string>
using namespace std;

CActionTextReader::CActionTextReader(void)
{
}

CActionTextReader::~CActionTextReader(void)
{
}

bool CActionTextReader::ParseLine(const string& sLine,ACTION_ITEM& ai)
{
	const char* p;
	const char* pWord;
	bool bAction2=false;
	string sTemp;
	p=sLine.c_str();
	// 过滤空白字符
	while(*p==' '||*p=='\t')
	{
		p++;
		continue;
	}
	pWord=p;
	// 是否为注释行
	if(*p=='\'')
	{
		return false;
	}
	// 先获得动作名
	while(*p)
	{
		if(*p=='=')
		{
			// 动作名定义结束
			sTemp.assign(pWord,p);
			ai.sName=sTemp.c_str();
			p++;
			break;
		}
		// 如果没有遇到=号，则一直判断直到段落结束
		p++;
	}
	pWord=p;
	while(*p)
	{
		if( (*p=='#') && (*(p+1)=='1'))
		{
			// action0结束
			sTemp.assign(pWord,p);
			ai.sAction0=sTemp.c_str();
			p+=2;
			break;
		}
		p++;
	}
	pWord=p;
	while(*p)
	{
		if( (*p=='#') && (*(p+1)=='2'))
		{
			// action1结束
			sTemp.assign(pWord,p);
			ai.sAction1=sTemp.c_str();
			p+=2;
			// action2开始
			bAction2=true;
			break;
			
		}
		p++;
	}
	pWord=p;
	while(*p)
	{
		if(*p=='#')
		{
			sTemp.assign(pWord,p);
			// action2开始
			ai.sAction2=sTemp.c_str();
			p++;
			ai.sMsg=p;
			return true;
		}
		p++;
	}
	if(bAction2)
	{
		ai.sAction2=pWord;
		return true;
	}
	return false;
}

BOOL CActionTextReader::LoadActionText(LPCTSTR szFile)
{
	ASSERT(szFile);
	if(szFile==NULL || *szFile==0)
		return FALSE;
	m_actionList.clear();
	ifstream ifs(szFile);
	if(ifs.fail())
		return FALSE;
	string sLine;
	ACTION_ITEM ai;
	while(getline(ifs,sLine))
	{
		if(ParseLine(sLine,ai))
			m_actionList.push_back(ai);
		ai.Clear();
	}
	return TRUE;
}

