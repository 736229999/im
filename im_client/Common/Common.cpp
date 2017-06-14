#include "StdAfx.h"
#include "Common.h"
#include <algorithm>
using namespace std;

std::string& trim(std::string &s) 
{
	if (s.empty()) 
	{
		return s;
	}

	s.erase(0,s.find_first_not_of(" "));
	s.erase(s.find_last_not_of(" ") + 1);
	return s;
}

//ע�⣺���ַ���Ϊ��ʱ��Ҳ�᷵��һ�����ַ���
void split(std::string& s, std::string& delim,std::vector< std::string >* ret)
{
	size_t last = 0;
	size_t index=s.find_first_of(delim,last);
	while (index!=std::string::npos)
	{
		ret->push_back(trim(s.substr(last,index-last)));
		last=index+1;
		index=s.find_first_of(delim,last);
	}
	if (index-last>0)
	{
		ret->push_back(trim(s.substr(last,index-last)));
	}
}

BOOL find(vector<string> &vec, LPCTSTR str)
{
	vector<string>::iterator it = std::find(vec.begin(), vec.end(), str);
	return it != vec.end();
}

// ������ְ�񣬷ֻ���
void SplitExtAndJob(LPCTSTR szFlashUrl, string &strExt, string &strJob)
{
	string strIntrenalFriendInfo = szFlashUrl;
	vector<string> vecInfo;
	string strSplit = ",";
	split(strIntrenalFriendInfo, strSplit, &vecInfo);

	// �ֻ���
	strExt = vecInfo.size()>0 ? vecInfo[0] : "";
	// ְ��
	strJob = vecInfo.size()>1 ? vecInfo[1] : "";
}

// ��ȡcpu�ϵ�����������ʱ�䣬���뼶
__declspec (naked) unsigned __int64 GetCpuCycle( void ) 
{ 
	_asm 
	{
		__asm _emit 0x0F
		__asm _emit 0x31
		ret
	}
}