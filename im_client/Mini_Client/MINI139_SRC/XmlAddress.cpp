#include "stdafx.h"
#include "XmlAddress.h"
#include "..\..\..\tinyxml\tinyxml.h"
#include <algorithm>
using namespace std;


// utf8编码转换为asc编码
string Utf82Asc(const char* utf8)  
{  
	// 先将utf8转换为unicode编码
	if(!utf8 || !strlen(utf8))  
	{  
		return NULL;  
	}  
	int dwUnicodeLen = MultiByteToWideChar(CP_UTF8,0,utf8,-1,NULL,0);  
	size_t num = dwUnicodeLen*sizeof(wchar_t);  
	wchar_t *pwUnicode = (wchar_t*)malloc(num);  
	memset(pwUnicode,0,num);  
	MultiByteToWideChar(CP_UTF8,0,utf8,-1,pwUnicode,dwUnicodeLen);  

	// 再讲unicode编码转为为asc编码
	int len = WideCharToMultiByte(CP_ACP, 0, pwUnicode, -1, NULL, 0, NULL, NULL);  
	char *asc = (char*)malloc(len + 1);  
	memset(asc, 0, len + 1);  
	WideCharToMultiByte(CP_ACP, 0, pwUnicode, -1, asc, len, NULL,NULL);  
	string strAsc = asc;

	free(pwUnicode);
	free(asc);

	return strAsc;  
} 

// 单个市结构体
struct Stru_City
{	
	int id;							// 市ID
	string strName;					// 市名
	map<int, string> mapDistrict;	// 县ID---县名	

	// 重载==，便于stl中的find
	BOOL operator==(int nCityId)
	{
		return id == nCityId;
	}
};

// 单个省份结构体
struct Stru_Province
{
	int id;								// 省ID
	string strName;						// 省名
	map<int, Stru_City> m_mapCity;		// 省下面的市	

	// 重载==，便于stl中的find
	BOOL operator==(int nProvinceId)
	{
		return id == nProvinceId;
	}
};

static vector<Stru_Province> g_vecProvince;			// 省数组

// 获取所有的省
void GetAllProvince(map<int, string> &mapProvince)
{
	mapProvince.clear();

	// 加载省份，市，区三级联动的配置文件（为了效率，只加载一次）
	if(g_vecProvince.size() == 0)
	{	
		CString sFile = GetModulePath() + "\\address.xml";
		TiXmlDocument doc;                              
		if (doc.LoadFile(sFile)) 
		{
			TiXmlElement* childElement_P = doc.RootElement();  //School元素  
			// 省
			while(childElement_P != NULL)
			{
				CString strPID = Utf82Asc(childElement_P->Attribute("PID")).c_str();
				CString strPName = Utf82Asc(childElement_P->Attribute("PName")).c_str();

				Stru_Province province;
				province.id = atoi(strPID);
				province.strName = (LPCTSTR)strPName;

				// 市
				TiXmlElement* childElement_C = childElement_P->FirstChildElement();
				while(childElement_C != NULL)
				{
					CString strCID = Utf82Asc(childElement_C->Attribute("CID")).c_str();
					CString strCName = Utf82Asc(childElement_C->Attribute("CName")).c_str();

					Stru_City city;
					city.id = atoi(strCID);
					city.strName = (LPCTSTR)strCName;

					// 区县
					TiXmlElement* childElement_D = childElement_C->FirstChildElement();
					while(childElement_D != NULL)
					{
						CString strDID = Utf82Asc(childElement_D->Attribute("DID")).c_str();
						CString strDName = Utf82Asc(childElement_D->Attribute("DName")).c_str();

						city.mapDistrict[atoi(strDID)] = (LPCTSTR)strDName;

						childElement_D = childElement_D->NextSiblingElement();
					}

					province.m_mapCity[city.id] = city;

					childElement_C = childElement_C->NextSiblingElement();
				}

				g_vecProvince.push_back(province);
				childElement_P = childElement_P->NextSiblingElement();
			}
		} 
	}

	// 省Combo加载数据
	for(int i = 0; i != g_vecProvince.size(); ++i)
	{
		Stru_Province &province = g_vecProvince[i];
		mapProvince[province.id] = province.strName;
	}
}

// 获取所有的市
void GetAllCity(int nProvinceID, map<int, string> &mapCity)
{
	if(g_vecProvince.size() == 0)
		return;

	// 得到省份结构体
	vector<Stru_Province>::iterator it = std::find(g_vecProvince.begin(), g_vecProvince.end(), nProvinceID);
	if(it != g_vecProvince.end())
	{
		map<int, Stru_City>::iterator map_it = it->m_mapCity.begin();
		for(; map_it != it->m_mapCity.end(); ++map_it)
		{
			mapCity[map_it->first] = map_it->second.strName;
		}
	}
}

// 获取所有的区县
void GetAllDistrict(int nProvinceID, int nCityID, map<int, string> &mapDistrict)
{
	if(g_vecProvince.size() == 0)
		return;

	mapDistrict.clear();
	// 得到省份结构体
	vector<Stru_Province>::iterator it = std::find(g_vecProvince.begin(), g_vecProvince.end(), nProvinceID);
	if(it != g_vecProvince.end())
	{
		// 得到具体的市
		map<int, Stru_City>::iterator map_it = it->m_mapCity.find(nCityID);
		if(map_it != it->m_mapCity.end())
		{
			map<int, string>::iterator map_D = map_it->second.mapDistrict.begin();
			for(; map_D != map_it->second.mapDistrict.end(); ++map_D)
			{
				mapDistrict[map_D->first] = map_D->second;
			}
		}
	}
}