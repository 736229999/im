#pragma once
#include <string>
#include <vector>

//ע�⣺���ַ���Ϊ��ʱ��Ҳ�᷵��һ�����ַ���
void split(std::string& s, std::string& delim,std::vector< std::string >* ret);

BOOL find(std::vector<std::string> &vec, LPCTSTR str);

// ������ְ�񣬷ֻ���
void SplitExtAndJob(LPCTSTR szFlashUrl, std::string &strExt, std::string &strJob);

// ��ȡcpu�ϵ�����������ʱ�䣬���뼶
unsigned __int64 GetCpuCycle( void );