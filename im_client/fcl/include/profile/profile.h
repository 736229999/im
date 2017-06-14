/******************************************************************
** 文件名:	profile.h
** 版  权:
** 创建人:	
** 日  期:	2006-07-11   14:38
** 描  述:	层次化性能评测树(注:线程安全实现)
**************************** 修改记录 ******************************
** 修改人: 
** 日  期: 
** 描  述: 
********************************************************************/

#ifndef __PROFILE_H__
#define __PROFILE_H__
#include "config.h"
#include <list>
#include <map>
#include <set>
#include <string>
#include "thread/ThreadingModel.h"
#include "misc/single.h"

using namespace std;

#ifdef _MSC_VER
#	pragma warning(push)
#	pragma warning(disable:4251)
#endif

/** 
 * 为了执行评测的效率，所有传入的名称(name)必须为常量字符串
 @code
 void CApp::onIdle()
 {
	PROFILE("CApp::onIdle");
	// ...
 }
 @endcode
 */

/// 用于定义性能测试节点的辅助宏

#ifdef SUPPORT_PROFILE
#	define	PROFILE(name)			ProfileSample __profile(__FILE__,__LINE__,name)
#   define  PROFILE_START(name,ID)  do{ManuProfile::instance()->startNode(__FILE__,__LINE__,name,ID);\
}while(0);
#   define  PROFILE_END(ID)         do{ManuProfile::instance()->endNode(ID);\
}while(0);

#   define  PROFILE_OUT(name)       do{\
	if(ProfileManager::instance()->isFinish()) \
	break;\
	ProfileNode* pNode =  ProfileManager::instance()->getRoot();\
	ProfileManager::instance()->outputNode(name,ProfileManager::AverageTime);\
}while(0);
#else
#	define	PROFILE(name)
#   define  PROFILE_OUT(name)
#   define  PROFILE_START(name,ID)     
#   define  PROFILE_END(ID)
#endif



#ifdef SUPPORT_PROFILE

BEGIN_NAMESPACE_FCL


/// 性能层次树中的一个节点
class FCL_CLASS ProfileNode
{
	typedef multiThread   threadMode  ;  //定义是否支持的线程模型
	friend class ProfileManager;
protected:
	ProfileNode*	m_parent;				/// 父节点
	ProfileNode*	m_child;				/// 子节点
	ProfileNode*	m_next;					/// 后一个节点
	char*           m_fileName;             ///所在文件名
	int             m_lineNumber;           ///所在的行号
	char*			m_name;					/// 节点名称
	uint32				m_totalCalls;			/// 全部调用次数
	double			m_totalTime;			/// 耗时
	double          m_averageTime;          ///平均调用时间
	int32			m_userData;				/// 用户自定义数据
    
private:
	uint64			m_startTime;			/// 开始时刻
	int32			m_recursionCounter;		/// 递归计数器
	threadMode::mutex   m_mutex;          ///线程锁
public:
	ProfileNode(char* name, ProfileNode* parent,uint32 Id);
	~ProfileNode();

public:
	/// 获取父节点
	ProfileNode* getParent()			{ return m_parent; }

	/// 获取子节点
	ProfileNode* getChild()				{ return m_child; }

	/// 获取前一个节点
	ProfileNode* getPrev();

	/// 获取后一个节点
	ProfileNode* getNext()				{ return m_next; }

	/// 获取最后一个子节点
	ProfileNode* getLastChild();

	/// 获取指定名字的子节点
	ProfileNode* getSubNode(char* name,uint32 Id);


	/// 获取节点名
	char*	getName() const				{ return m_name; }

	/// 获取节点的调用次数
	int32	getTotalCalls() const			{ return m_totalCalls; }

	/// 获取节点的耗时
	double getTotalTime() const			{ return m_totalTime; }

	/// 获取用户数据
	uint32 getUserData() const			{ return m_userData; }

	/// 设置用户数据
	void setUserData(uint32 userData)	{ m_userData = userData; }
	///获取平均执行时间
	double getAverageTime() const       { return m_averageTime;}

	/// 节点重置
	void reset();

	/// 节点进入（开始性能评测）
	void enter();

	/// 节点离开（统计、计算各项参数）
	bool leave();

};



/// 性能评测树管理器
class FCL_CLASS ProfileManager : public CSingleton<ProfileManager>
{   
	///节点操作函数指针
	typedef void (ProfileManager::*handleNodeType)(ProfileNode*);
	typedef multiThread       threadMode;   //线程安全模型
	struct TNode
	{
		char* fileName;     //文件名
		uint32 lineNumber;   //行号
		//uint32 threadId;     //线程ID
		TNode(char* strName,uint32 line)
		{
			fileName = strName;
			lineNumber = line;
		}
		 bool operator == (const TNode& t1)const
		{
			//bool b = strcmp(this->fileName,t1.fileName) == 0;
			//直接进行地址比较，因为时静态字符串
			bool b = (fileName == t1.fileName);
			return b && (this->lineNumber,t1.lineNumber);
		}
		bool operator < (const TNode& t1)const
		{
			if(lineNumber < t1.lineNumber)
				return true;
			else
				return false;
		}
	};

private:
	ProfileNode*		m_root;			/// 性能评测树根节点
	//ProfileNode*		m_currentNode;	/// 用于性能评测的当前节点
	int64				m_resetTime;	/// 自重置以来逝去的时间
	bool				m_enabled;		/// 是否使性能评测有效
	bool               m_isFinish;     ///只运行一次
	//性能评测的当前节点，threadID <--->m_currentNode,用于多线程，每个线程一个评测树
	map<int,ProfileNode*> m_currentNodeList;  
	map<TNode,ProfileNode*>        m_nodeList;//存放所有的评测节点
	threadMode::mutex  m_mutex;         ///同步锁
public:
	typedef enum
	    {
			CallTimes, //调用次数排序
	    	TotalTime,  //调用时间排序
			AverageTime //平均执行时间排序
	    }SORT_TYPE;
	
	ProfileManager();
	~ProfileManager();

public:
	///是否已经输出过
	bool isFinish() {return m_isFinish;}
	/// 开始一个性能节点的评测
	void startProfile(char* fileName,int lineNumber,char* name);

	/// 结束当前性能节点的评测
	void endProfile();

	/// 评测数据重置
	void reset();

	/// 获取自重置以来的时间
	float getTimeSinceReset();

	/// 获得根节点
	ProfileNode* getRoot() const		{ return m_root; }

	/// 设置性能评测是否有效
	void setEnabled(bool enabled)		{ m_enabled = enabled; }

	/// 性能评测是否有效
	bool isEnabled() const				{ return m_enabled; }
	///遍历性能评测数，将所有的节点存入链表中
	void visitorProfile(ProfileNode* pNode,handleNodeType fn);
	///节点合并操作
	void mergeNode(ProfileNode* pNode);
	///删除所有节点
	void deleteNode(ProfileNode* pNode);
	///对获取的评测链表进行排序（按调用次数或执行时间等）,输出到文件
	void outputNode(char* fileName,SORT_TYPE st = CallTimes);
	//排序函数
	static bool sortByCallTimes(const ProfileNode* o1, const ProfileNode* o2)
	{
		return o1->getTotalCalls() < o2->getTotalCalls();
	}
	static bool sortByTotalTime(const ProfileNode* o1, const ProfileNode* o2)
	{
		return o1->getTotalTime() < o2->getTotalTime();
	}
	static bool sortByAverTime(const ProfileNode* o1, const ProfileNode* o2)
	{
		return o1->getAverageTime() < o2->getAverageTime();
	}
};

/// 简单性能节点封装
class FCL_CLASS ProfileSample
{
public:
	ProfileSample(char* fileName,int lineNumber,char* name)
	{ 
		ProfileManager::instance()->startProfile(fileName,lineNumber,name); 
	}

	~ProfileSample()					
	{ 
		ProfileManager::instance()->endProfile(); 
	}
};
///手工控制,异步的情况下测时性能使用
class FCL_CLASS  ManuProfile: public CSingleton<ManuProfile>
{
	typedef  multiThread   threadMode;  ///线程安全
private:
	struct ProNode
	{
		uint64 m_startTime;  //开始时间
		uint32 m_Calls;      ///调用次数
		double m_totalTime;  ///耗时
		char* m_name;       ///节点名称
		uint32 m_id;      ///节点标识id
		string m_nameid;   ///节点标识名
		char*  m_fileName;   //所在文件名
		uint32 m_lineNumber;  ///所在行号
		double m_averageTime; ///平均执行时间
		bool   m_isFinish;    ///是否已经结束，防止多次统计
		ProNode(char* fileName,uint32 lineNumber,char* name,string ID)
		{
			m_startTime = 0;
			m_Calls = 0;
			m_totalTime = 0;
			m_id = 0;
			m_name = name;
			m_nameid = ID;
			m_fileName = fileName;
			m_lineNumber = lineNumber;
			m_averageTime = 0;
			m_isFinish = false;
		}
		ProNode(char* fileName,uint32 lineNumber,char* name,uint32 ID)
		{
			m_startTime = 0;
			m_Calls = 0;
			m_totalTime = 0;
			m_id = ID;
			m_name = name;
			m_nameid = "";
			m_fileName = fileName;
			m_lineNumber = lineNumber;
			m_averageTime = 0;
			m_isFinish = false;
		}
		//bool operator == (const ProNode& t1)const
		//{
		//	bool b1 = strcmp(m_name,t1.m_name) == 0;
		//	bool b2 = (m_nameid.compare(t1.m_nameid) == 0);
		//   return b1 && b2 && (m_id == t1.m_id);
		//}
	};
	private:
		map<uint32,ProNode*> m_intList;
		map<string,ProNode*> m_strList;
		set<ProNode*>   m_resultSet;     ///保存收集到的结果集
		typedef  map<uint32,ProNode*>::iterator intListItr;
		typedef  map<string,ProNode*>::iterator strListItr;
		threadMode::mutex  m_mutex;  ///同步锁
		void collectResult();
public:
     void startNode(char* fileName,uint32 lineNumber,char* nodeName,string nodeID);
	 void startNode(char* fileName,uint32 lineNumber,char* nodeName,uint32 nodeID);
	 void endNode(char* nodeID);
	 void endNode(uint32 nodeID);
	 void outputNode(FILE* fp);
	 ~ManuProfile()
	 {   
		/* PROFILE_OUT("profile.log");*/
		 //节点删除
		 threadMode::scope_lock guard(m_mutex);

			 intListItr intItr = m_intList.begin();
			 while(intItr != m_intList.end())
			 {
				 delete (*intItr).second;
				 intItr++;
			 }
			 m_intList.clear();
			 strListItr strItr = m_strList.begin();
			 while(strItr != m_strList.end())
			 {
				 delete (*strItr).second;
				 strItr++;
			 }
			 m_strList.clear();
	 }
};

END_NAMESPACE

#endif // SUPPORT_PROFILE


#ifdef _MSC_VER
#	pragma warning(pop)
#endif
#endif // __PROFILE_H__
