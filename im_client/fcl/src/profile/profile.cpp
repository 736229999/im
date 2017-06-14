/******************************************************************
** 文件名:	profile.cpp
** 版  权:	(C)  
** 创建人:	Tony
** 日  期:
** 描  述:
**************************** 修改记录 ******************************
** 修改人: Tony
** 日  期: 2007-4-48 
** 描  述: 增加新的性能测试函数和宏，提供2种不同的测时方式
********************************************************************/

#include "stdafx.h"
#include "profile/profile.h"
#include "timer/MyTimer.h"
#include "stream/FileStream.h"
#include "trace/Trace.h"
#include <iostream>
#include <time.h>
#ifdef SUPPORT_PROFILE


BEGIN_NAMESPACE_FCL
//计时器
static NewTimer<>  proTimer;


//////////////////////////////////////////////////////////////////////////
// 注意，为了性能问题，这里的name必须是静态常字符串，这里仅仅记录它的指针
ProfileNode::ProfileNode(char* name, ProfileNode* parent,uint32 Id) :
	m_parent(parent), m_child(NULL), m_next(NULL),
	m_name(name), m_totalCalls(0), m_totalTime(0), m_userData(Id), 
	m_startTime(0), m_recursionCounter(0),m_averageTime(0),m_lineNumber(0),
	m_fileName(NULL)
{
	reset();
}


ProfileNode::~ProfileNode()
{
	//if (m_child)
	//{
	//	delete m_child;
	//	m_child = NULL;
	//}

	//if (m_next)
	//{
	//	delete m_next;
	//	m_next = NULL;
	//}
}

ProfileNode* ProfileNode::getPrev()
{
	if (!m_parent) // root?
		return NULL;

	ProfileNode* child = m_parent->m_child;
	ProfileNode* prev = 0;
	while (child)
	{
		if (child->m_name == m_name)
			break;
		prev = child;
		child = child->m_next;
	}
	return prev;
}

ProfileNode* ProfileNode::getLastChild()
{
	ProfileNode* child = m_child;
	while(child && child->m_next)
	{
		child = child->m_next;
	}
	return child;
}

ProfileNode* ProfileNode::getSubNode(char* name,uint32 Id)
{
	threadMode::scope_lock guard(m_mutex);//同步该函数
	// 查找所有的子节点
	ProfileNode* child = m_child;
	ProfileNode* last_child = m_child;
	while (child)
	{
		if (child->m_name == name && child->m_userData == Id)
			return child;

		last_child = child;
		child = child->m_next;
	}

	// 未找到，则添加节点
	ProfileNode* node = new ProfileNode(name, this,Id);
	if (last_child)
		last_child->m_next = node;
	else
		m_child = node;
	node->m_next = 0;
	return node;
}


void ProfileNode::reset()
{
	m_totalCalls = 0;
	m_totalTime = 0.0f;

	if (m_child)
		m_child->reset();

	if (m_next)
		m_next->reset();
}


void ProfileNode::enter()
{
	m_totalCalls++;
	if (m_recursionCounter++ == 0)
		//_profile_get_ticks(&m_startTime);
		m_startTime = proTimer.getTick();
}


bool ProfileNode::leave()
{
	if (--m_recursionCounter == 0 && m_totalCalls != 0)
	{ 
		int64 time;
		double t1;
		//_profile_get_ticks(&time);
		time = proTimer.getTick();
		t1 = ((time - m_startTime)/(double)proTimer.getFrequency());
		m_totalTime += t1;
	}

	return (m_recursionCounter == 0);
}



//////////////////////////////////////////////////////////////////////////
ProfileManager::ProfileManager()
{
	m_root = new ProfileNode("Root", NULL,0);
	m_resetTime = 0;
	m_enabled = true;
	m_isFinish = false;
}


ProfileManager::~ProfileManager()
{
	 /*PROFILE_OUT("profile.csv");*/
	 char strDate[255];
	 _strdate(strDate);
	 char* p = strDate - 1;
	 //转换为windows识别的文件名'/'替换成'-'
	 while(*(++p))
	 {
		 if((*p == '/') || (*p == '\\') || (*p == ':'))
			 *p = '-';
	 }
	 strcat(strDate,".csv"); //按日期生成文件名
	 PROFILE_OUT(strDate);
	visitorProfile(m_root,ProfileManager::deleteNode);
	//m_currentNode = 0;
	m_resetTime = 0;
}
void ProfileManager::deleteNode(ProfileNode* pNode)
{
	delete pNode;
	pNode = NULL;
}
void ProfileManager::startProfile(char* fileName,int lineNumber,char* name)
{
	if (!m_enabled) return;
     
     uint32 threadId = GetCurrentThreadId();
	 ProfileNode* pCurrentNode = m_currentNodeList[threadId];
	 {   ///括号限定同步的作用域
		 threadMode::scope_lock guard(m_mutex);
		 if(pCurrentNode == NULL)
		 {
			 pCurrentNode = m_root->getSubNode(name,threadId);
			 pCurrentNode->m_fileName = fileName;
			 pCurrentNode->m_lineNumber = lineNumber;
			 m_currentNodeList[threadId] = pCurrentNode;
		 }
	 }
	if (name != pCurrentNode->getName() || threadId != pCurrentNode->m_userData)
	{
		pCurrentNode = pCurrentNode->getSubNode(name,threadId);
		pCurrentNode->m_fileName = fileName;
		pCurrentNode->m_lineNumber = lineNumber;
	}
	//
	m_currentNodeList[threadId] = pCurrentNode;
	pCurrentNode->enter();
}

void ProfileManager::endProfile()
{
	if (!m_enabled) return;

    uint32 threadId = GetCurrentThreadId();
	ProfileNode* pCurrentNode = m_currentNodeList[threadId];
	if (pCurrentNode->leave())
	{
		pCurrentNode = pCurrentNode->getParent();
		m_currentNodeList[threadId] = pCurrentNode;
	}
}

void ProfileManager::reset()
{ 
	m_root->reset(); 
	//_profile_get_ticks(&m_resetTime);
	m_resetTime = proTimer.getTick();
}

float ProfileManager::getTimeSinceReset()
{
	int64 time;
	//_profile_get_ticks(&time);
	time = proTimer.getTick();
	time -= m_resetTime;
	return (float)time;
}
///对所有的节点进行合并相同的函数节点，放入m_nodeList链表中
void ProfileManager::mergeNode(ProfileNode* pNode)
{
	std::map<TNode,ProfileNode*>::iterator itr;
	TNode node(pNode->m_fileName,pNode->m_lineNumber);
	itr = m_nodeList.find(node);
	if(itr != m_nodeList.end())
	{
		//对所有相同的节点数据进行合并
		m_nodeList[node]->m_totalTime += pNode->m_totalTime;
		m_nodeList[node]->m_totalCalls += pNode->m_totalCalls;
	}
	else
		m_nodeList[node] = pNode;
	///重新计算平均执行时间
	itr = m_nodeList.find(node);
	if(itr != m_nodeList.end())
	{
		(*itr).second->m_averageTime = (uint32)((*itr).second->m_totalTime / (*itr).second->m_totalCalls);
	}
	else
	{
		std::cout<<"error!"<<std::endl;
	}
}
void ProfileManager::visitorProfile(ProfileNode* pNode,handleNodeType fn)
{
	//使用visitor模式
   ProfileNode* p = pNode;
   std::map<TNode,ProfileNode*>::iterator itr;
   if(!p)
	   return;
    ProfileNode* nextNode = p->getChild();
	ProfileNode* pTemp;
	while(nextNode)
	{  
		///递归访问下一个节点,因为可能会删除节点，所以事先保存
		pTemp = nextNode->getNext();
		visitorProfile(nextNode,fn);
		nextNode = pTemp;
	}
	//对当前节点进行操作
	(this->*fn)(p);
}
//输出统计信息
void ProfileManager::outputNode(char* fileName,SORT_TYPE st)
{
	///遍历所有的节点，进行合并操作
	visitorProfile(m_root,&ProfileManager::mergeNode);
	std::list<ProfileNode*>  m_list;  //转存所有节点到链表中
	std::map<TNode,ProfileNode*>::iterator tmpItr = m_nodeList.begin();
	//避免重复输出
	m_isFinish = true;
	for(;tmpItr != m_nodeList.end(); ++tmpItr)
	{
		m_list.push_back((*tmpItr).second);
	}
	switch(st)
	{
	case CallTimes:
		m_list.sort(sortByCallTimes);
		break;
	case TotalTime:
		m_list.sort(sortByTotalTime);
		break;
	case AverageTime:
		m_list.sort(sortByAverTime);
		break;
	}
	//写入文件
	char buf[200],path[255];
	::GetCurrentDirectory(255,path);
	strcat(path,"\\profile");
	createDirectoryRec(path);//创建目录，存在则直接返回
	strcat(path,"\\");
	strcat(path,fileName);
	FILE* file;
	file = fopen(path,"a");//创建文件
	if(!file)
	{
		Trace("创建文件失败:%s",path);
		return;
	}
	fseek(file,SEEK_END,SEEK_SET); //定位到文件尾
	/*if(!file) 
		TRACE("error");*/
	std::list<ProfileNode*>::iterator itr = m_list.begin();
	///输出评测结果标题
	sprintf(buf,"函数名,     调用次数,\
				执行总时间       ,	平均时间,\
				文件,   行号\n");
	fprintf(file,buf);
	while(itr != m_list.end())
	{
		sprintf(buf,"%s,     %-5d,\
					%lf微妙,	%ld微妙,\
					%s,   %d\n",\
			    (*itr)->m_name,(*itr)->m_totalCalls,\
				(*itr)->m_totalTime,(long int)(*itr)->m_averageTime,\
				 (*itr)->m_fileName,(*itr)->m_lineNumber);
		//sprintf(buf,"函数名:%s,     调用次数:%-5d,\
		//			执行总时间:%lf微妙,		平均时间:%ld微妙,\
		//			文件:%s,   行号:%d\n",\
		//			(*itr)->m_name,(*itr)->m_totalCalls,\
		//			(*itr)->m_totalTime,(long int)(*itr)->m_averageTime,\
		//			(*itr)->m_fileName,(*itr)->m_lineNumber);
        fprintf(file,buf);
		itr++;
	}
    ///输出手动控制的性能评测数据
	ManuProfile::instance()->outputNode(file);
	fclose(file);
}


void ManuProfile::startNode(char* fileName,uint32 lineNumber,char* nodeName,string nodeID)
{
	ProNode* pd = new ProNode(fileName,lineNumber,nodeName,nodeID);
	pd->m_startTime = proTimer.getTick();
	pd->m_Calls++;
	threadMode::scope_lock guard(m_mutex);
    if(m_strList.find(nodeName) != m_strList.end())
		assert(1);
	m_strList[string(nodeID)] = pd;
}
void ManuProfile::startNode(char* fileName,uint32 lineNumber,char* nodeName,uint32 nodeID)
{
	ProNode* pd = new ProNode(fileName,lineNumber,nodeName,nodeID);
	pd->m_startTime = proTimer.getTick();
	pd->m_Calls++;
	threadMode::scope_lock guard(m_mutex);

	if(m_intList.find(nodeID) != m_intList.end())
		assert(1);

	m_intList[nodeID] = pd;
}
void ManuProfile::endNode(char* nodeID)
{
	threadMode::scope_lock guard(m_mutex);
	strListItr strItr = m_strList.find(string(nodeID));
	if(strItr == m_strList.end())
	{
		assert(1);//错误
		return;
	}
	if((*strItr).second->m_isFinish)
        return;
     int64  endTime = proTimer.getTick();
	 int64 startTime = (*strItr).second->m_startTime;
     (*strItr).second->m_totalTime = (double)((endTime - startTime)/(double)proTimer.getFrequency());
	 (*strItr).second->m_Calls++;
	(*strItr).second->m_isFinish = true;  ///防止多次统计

}
void ManuProfile::endNode(uint32 nodeID)
{
	threadMode::scope_lock guard(m_mutex);
	intListItr intItr = m_intList.find(nodeID);
	if(intItr == m_intList.end())
	{
		assert(1);
		return;
	}
	if((*intItr).second->m_isFinish)
		return;
     int64  endTime = proTimer.getTick();
	 int64  startTime = (*intItr).second->m_startTime;
	(*intItr).second->m_totalTime = (double)((endTime - startTime)/(double)proTimer.getFrequency());
	(*intItr).second->m_isFinish = true;
}

void ManuProfile::collectResult()
{
	threadMode::scope_lock guard(m_mutex);

	set<ProNode*>::iterator sItr;
	intListItr intItr = m_intList.begin();
	while(intItr != m_intList.end())
	{
		sItr = m_resultSet.find((*intItr).second);
		if(sItr != m_resultSet.end())
		{//计算调用次数，平均时间
			(*sItr)->m_totalTime += (*intItr).second->m_totalTime;
			(*sItr)->m_Calls   += (*intItr).second->m_Calls;
			(*sItr)->m_averageTime = (*sItr)->m_totalTime / (*sItr)->m_Calls;
		}
		else
		{
			(*intItr).second->m_averageTime = (*intItr).second->m_totalTime / ((*intItr).second->m_Calls);
			m_resultSet.insert((*intItr).second);
		}
		intItr++;
	}
	strListItr strItr = m_strList.begin();
	while(strItr != m_strList.end())
	{
		sItr = m_resultSet.find((*strItr).second);
		if(sItr != m_resultSet.end())
		{
			//计算调用次数，平均时间
			(*sItr)->m_totalTime += (*strItr).second->m_totalTime;
			(*sItr)->m_Calls   += (*strItr).second->m_Calls;
			(*sItr)->m_averageTime = (*sItr)->m_totalTime / (*sItr)->m_Calls;
		}
		else
		{
			(*strItr).second->m_averageTime = (*strItr).second->m_totalTime / ((*strItr).second->m_Calls);
			m_resultSet.insert((*strItr).second);
		}
		strItr++;
	}
}

void ManuProfile::outputNode(FILE* fp)
{

	assert(fp);
    ///收集，合并结果集
	collectResult();
	///输出结果集
    char buf[200];
	set<ProNode*>::iterator itr = m_resultSet.begin();
	for(;itr != m_resultSet.end(); ++itr)
	{
		ProNode* pNode = (*itr);
		sprintf(buf,"%s,   %-5d,\
			    %lf微秒,   %d微秒,\
			    %s,  %d\n",\
			(*itr)->m_name,(*itr)->m_Calls,\
			(*itr)->m_totalTime,(uint32)(*itr)->m_averageTime,\
			(*itr)->m_fileName,(*itr)->m_lineNumber);
		/*sprintf(buf,"函数名:%s,   调用次数:%-5d,\
					执行总时间:%lf微秒,   平均时间:%d微秒,\
					文件名:%s,  行号:%d\n",\
					(*itr)->m_name,(*itr)->m_Calls,\
					(*itr)->m_totalTime,(uint32)(*itr)->m_averageTime,\
					(*itr)->m_fileName,(*itr)->m_lineNumber);*/
		fprintf(fp,buf);

	}
}
END_NAMESPACE
#endif // SUPPORT_PROFILE
