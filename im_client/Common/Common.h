#pragma once
#include <string>
#include <vector>

//注意：当字符串为空时，也会返回一个空字符串
void split(std::string& s, std::string& delim,std::vector< std::string >* ret);

BOOL find(std::vector<std::string> &vec, LPCTSTR str);

// 解析出职务，分机号
void SplitExtAndJob(LPCTSTR szFlashUrl, std::string &strExt, std::string &strJob);

// 获取cpu上电以来经过的时间，纳秒级
unsigned __int64 GetCpuCycle( void );