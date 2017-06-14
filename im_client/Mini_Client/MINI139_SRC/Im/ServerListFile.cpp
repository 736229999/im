#include "stdafx.h"

#include "serverlistfile.h"
#import "msxml3.dll"
#include <fstream>
#include <string>
#ifdef _DEBUG
#include <iostream>
#endif
using namespace std;

// 可选择的服务器的最大个数
#define MAX_CHOOSE 3

bool IMSERVER_LIST::GetBestSrv(IM_SERVER & srv) const
{
	size_t i,n,p;
	unsigned short max,v;
	n = size();
	if( n == 0)
		return false;
	max = operator [](0).maxConn;
	p = 0;
	for( i = 0; i < n; i++ )
	{
		v = operator [](i).maxConn;
		if( v > max)
		{
			p = i;
			max = v;
		}
	}
	srv = operator [](p);
	return true;
}

bool IMSERVER_LIST::GetRandSrv(IM_SERVER & srv) 
{
	size_t c;
	int n = 0;			// 当前被选出的候选服务器个数
	int idx;			// 被选中的服务器
	IMSERVER_LIST hx;	// 候选列表
	c = size();
	if(c == 0)
		return false;
	n = c > MAX_CHOOSE ? MAX_CHOOSE:c;
	// 按服务器能力排序，选出最好的n个做候选
	sort(begin(),end(),greater<IM_SERVER>());
	hx.assign(begin(),begin() + n);
	idx = GetTickCount() % n;
	srv = hx[idx];
	return true;
}



CServerListFile::CServerListFile( )
{
	CoInitialize(NULL);
}

CServerListFile::~CServerListFile()
{
	try
	{
		CoUninitialize();
	}
	catch(...)
	{
		;
	}
}

bool LoadSrvList(MSXML2::IXMLDOMNodePtr nodePtr,IMSERVER_LIST & arrSrv)
{
	arrSrv.clear();
	MSXML2::IXMLDOMNodeListPtr nodeListPtr;
	MSXML2::IXMLDOMNamedNodeMapPtr attrsPtr;
	MSXML2::IXMLDOMNodePtr itPtr,attrPtr;
	nodeListPtr = nodePtr->childNodes;
	int iLen = nodeListPtr->length;
	int i;
	string sNode;
	for(i = 0;i < iLen; i++)
	{
		IM_SERVER srv;
		itPtr = nodeListPtr->Getitem(i);
		sNode = (char * )itPtr->nodeName;
		// 如果为无法识别的标识则忽略
		if(stricmp( sNode.c_str(),"server") != 0)
			continue;
		// 获得服务器的属性定义
		attrsPtr = itPtr->attributes;
		//
		// 获得服务器IP
		//
		attrPtr = attrsPtr->getNamedItem("ip");
		if(attrPtr != NULL)
		{
			hostent * pEnt;
			pEnt = gethostbyname((char *)attrPtr->text);
			if(pEnt)
			{
				srv.ip = *((int*)pEnt->h_addr_list[0]);
			}
			else
				srv.ip = 0;
			//srv.ip = inet_addr((char *)attrPtr->text) ;
		}
		else
			srv.ip = 0;
		//
		// 获得服务器端口
		//
		attrPtr = attrsPtr->getNamedItem("port");
		if(attrPtr != NULL)
			srv.port = atoi((char *)attrPtr->text); 
		else
			srv.port = 0;
		//
		// 获得最大连接数
		//
		//
		attrPtr =  attrsPtr->getNamedItem("max");
		if(attrPtr != NULL)
			srv.maxConn = atoi((char *)attrPtr->text); 
		else
			srv.maxConn = 0;

		// ...
		// 

		// 保存结果
		arrSrv.push_back(srv);
	}
	return true;
}



bool CServerListFile::Load(const char * szFile)
{
	m_arrBusiSrv.clear();
	m_arrTranSrv.clear();
	if(szFile == NULL)
		return false;
	_variant_t varXml(szFile);
	_variant_t varOut((bool)TRUE);
	MSXML2::IXMLDOMDocumentPtr docPtr;
	HRESULT hr;
	hr = docPtr.CreateInstance("msxml2.domdocument");
	if(hr < 0)
	{
		// "msxml解析器加载失败，检测是否已经安装";
		return false;
	}
	docPtr->PutresolveExternals(FALSE);
	docPtr->PutvalidateOnParse(FALSE);

	varOut = docPtr->load(varXml);
	if ((bool)varOut == false)
	{
		return false;
	}
	MSXML2::IXMLDOMNodePtr	nodePtr;
	nodePtr = docPtr->GetlastChild();
	string sNode = (char*)nodePtr->nodeName;
	if(stricmp(sNode.c_str(),"serverList") != 0)
	{
		return false;
	}	

	MSXML2::IXMLDOMNodeListPtr nodeListPtr;
	MSXML2::IXMLDOMNodePtr itPtr;
	nodeListPtr = nodePtr->childNodes;
	int iLen = nodeListPtr->length;
	int i;
	// 遍列当前服务器列表
	for(i = 0;i < iLen; i++)
	{
		itPtr=nodeListPtr->Getitem(i);
		if(itPtr->nodeType==MSXML2::NODE_TEXT ||
			itPtr->nodeType==MSXML2::NODE_COMMENT)
			continue;
		else if(itPtr->nodeType==MSXML2::NODE_ELEMENT)
		{
			sNode = (char*)itPtr->nodeName;
			if(stricmp( sNode.c_str(),"busiServerList") == 0)
			{
				// 获取业务服务器列表
				LoadSrvList(itPtr,m_arrBusiSrv);
			}
			else if(stricmp( sNode.c_str(),"tranServerList") == 0)
			{
				// 获取中转服务器列表
				LoadSrvList(itPtr,m_arrTranSrv);
			}	
		}
	}
	return true;
}

bool CServerListFile::Save(const char * szFile)
{
	if(szFile == NULL)
		return false;
	ofstream ofs(szFile);
	if(ofs.fail())
		return false;
	// 写入xml头
	ofs<<"<?xml version=\"1.0\" encoding=\"gb2312\" ?>"<<endl;
	ofs<<"<serverList>"<<endl;
	size_t i,iC;
	iC = m_arrBusiSrv.size();
	if( iC > 0)
	{
		// 写入业务服务器列表
		ofs<<"<busiServerList>"<<endl;
		for( i = 0;i < iC;i++ )
		{
			ofs <<"\t<server"
				<<" ip=\""<< inet_ntoa(*(in_addr*)&m_arrBusiSrv[i].ip)<<"\""
				<<" port=\""<< m_arrBusiSrv[i].port<<"\""
				<<"/>"<<endl;
		}
		ofs<<"</busiServerList>"<<endl;
	}
	iC = m_arrTranSrv.size();
	if( iC > 0 )
	{
		// 写入中转服务器列表
		ofs<<"<tranServerList>"<<endl;
		for( i = 0;i < iC;i++ )
		{
			ofs <<"\t<server"
				<<" ip=\""<< inet_ntoa(*(in_addr*)&m_arrTranSrv[i].ip)<<"\""
				<<" port=\""<< m_arrTranSrv[i].port<<"\""
				<<"/>"<<endl;
		}
		ofs<<"</tranServerList>"<<endl;
	}

	ofs<<"</serverList>"<<endl;
	ofs.close();
	return true;
}



#ifdef _DEBUG
void CServerListFile::Print(IMSERVER_LISTC & svrList)
{
	size_t i,iC;
	iC = svrList.size();
	cout<<"服务器列表共有"<<(int)iC<<"项"<<endl;
	cout<<"-----------------------------"<<endl;
	for(i = 0;i < iC; i++)
	{
		cout<<inet_ntoa(*(in_addr*)&svrList[i].ip)<<":"<<svrList[i].port<<endl;
	}
	cout<<"-----------------------------"<<endl;
	

}
#endif


