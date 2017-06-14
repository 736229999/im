/******************************************************************
** �ļ���:	profile.h
** ��  Ȩ:
** ������:	
** ��  ��:	2006-07-11   14:38
** ��  ��:	��λ�����������(ע:�̰߳�ȫʵ��)
**************************** �޸ļ�¼ ******************************
** �޸���: 
** ��  ��: 
** ��  ��: 
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
 * Ϊ��ִ�������Ч�ʣ����д��������(name)����Ϊ�����ַ���
 @code
 void CApp::onIdle()
 {
	PROFILE("CApp::onIdle");
	// ...
 }
 @endcode
 */

/// ���ڶ������ܲ��Խڵ�ĸ�����

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


/// ���ܲ�����е�һ���ڵ�
class FCL_CLASS ProfileNode
{
	typedef multiThread   threadMode  ;  //�����Ƿ�֧�ֵ��߳�ģ��
	friend class ProfileManager;
protected:
	ProfileNode*	m_parent;				/// ���ڵ�
	ProfileNode*	m_child;				/// �ӽڵ�
	ProfileNode*	m_next;					/// ��һ���ڵ�
	char*           m_fileName;             ///�����ļ���
	int             m_lineNumber;           ///���ڵ��к�
	char*			m_name;					/// �ڵ�����
	uint32				m_totalCalls;			/// ȫ�����ô���
	double			m_totalTime;			/// ��ʱ
	double          m_averageTime;          ///ƽ������ʱ��
	int32			m_userData;				/// �û��Զ�������
    
private:
	uint64			m_startTime;			/// ��ʼʱ��
	int32			m_recursionCounter;		/// �ݹ������
	threadMode::mutex   m_mutex;          ///�߳���
public:
	ProfileNode(char* name, ProfileNode* parent,uint32 Id);
	~ProfileNode();

public:
	/// ��ȡ���ڵ�
	ProfileNode* getParent()			{ return m_parent; }

	/// ��ȡ�ӽڵ�
	ProfileNode* getChild()				{ return m_child; }

	/// ��ȡǰһ���ڵ�
	ProfileNode* getPrev();

	/// ��ȡ��һ���ڵ�
	ProfileNode* getNext()				{ return m_next; }

	/// ��ȡ���һ���ӽڵ�
	ProfileNode* getLastChild();

	/// ��ȡָ�����ֵ��ӽڵ�
	ProfileNode* getSubNode(char* name,uint32 Id);


	/// ��ȡ�ڵ���
	char*	getName() const				{ return m_name; }

	/// ��ȡ�ڵ�ĵ��ô���
	int32	getTotalCalls() const			{ return m_totalCalls; }

	/// ��ȡ�ڵ�ĺ�ʱ
	double getTotalTime() const			{ return m_totalTime; }

	/// ��ȡ�û�����
	uint32 getUserData() const			{ return m_userData; }

	/// �����û�����
	void setUserData(uint32 userData)	{ m_userData = userData; }
	///��ȡƽ��ִ��ʱ��
	double getAverageTime() const       { return m_averageTime;}

	/// �ڵ�����
	void reset();

	/// �ڵ���루��ʼ�������⣩
	void enter();

	/// �ڵ��뿪��ͳ�ơ�������������
	bool leave();

};



/// ����������������
class FCL_CLASS ProfileManager : public CSingleton<ProfileManager>
{   
	///�ڵ��������ָ��
	typedef void (ProfileManager::*handleNodeType)(ProfileNode*);
	typedef multiThread       threadMode;   //�̰߳�ȫģ��
	struct TNode
	{
		char* fileName;     //�ļ���
		uint32 lineNumber;   //�к�
		//uint32 threadId;     //�߳�ID
		TNode(char* strName,uint32 line)
		{
			fileName = strName;
			lineNumber = line;
		}
		 bool operator == (const TNode& t1)const
		{
			//bool b = strcmp(this->fileName,t1.fileName) == 0;
			//ֱ�ӽ��е�ַ�Ƚϣ���Ϊʱ��̬�ַ���
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
	ProfileNode*		m_root;			/// �������������ڵ�
	//ProfileNode*		m_currentNode;	/// ������������ĵ�ǰ�ڵ�
	int64				m_resetTime;	/// ������������ȥ��ʱ��
	bool				m_enabled;		/// �Ƿ�ʹ����������Ч
	bool               m_isFinish;     ///ֻ����һ��
	//��������ĵ�ǰ�ڵ㣬threadID <--->m_currentNode,���ڶ��̣߳�ÿ���߳�һ��������
	map<int,ProfileNode*> m_currentNodeList;  
	map<TNode,ProfileNode*>        m_nodeList;//������е�����ڵ�
	threadMode::mutex  m_mutex;         ///ͬ����
public:
	typedef enum
	    {
			CallTimes, //���ô�������
	    	TotalTime,  //����ʱ������
			AverageTime //ƽ��ִ��ʱ������
	    }SORT_TYPE;
	
	ProfileManager();
	~ProfileManager();

public:
	///�Ƿ��Ѿ������
	bool isFinish() {return m_isFinish;}
	/// ��ʼһ�����ܽڵ������
	void startProfile(char* fileName,int lineNumber,char* name);

	/// ������ǰ���ܽڵ������
	void endProfile();

	/// ������������
	void reset();

	/// ��ȡ������������ʱ��
	float getTimeSinceReset();

	/// ��ø��ڵ�
	ProfileNode* getRoot() const		{ return m_root; }

	/// �������������Ƿ���Ч
	void setEnabled(bool enabled)		{ m_enabled = enabled; }

	/// ���������Ƿ���Ч
	bool isEnabled() const				{ return m_enabled; }
	///���������������������еĽڵ����������
	void visitorProfile(ProfileNode* pNode,handleNodeType fn);
	///�ڵ�ϲ�����
	void mergeNode(ProfileNode* pNode);
	///ɾ�����нڵ�
	void deleteNode(ProfileNode* pNode);
	///�Ի�ȡ����������������򣨰����ô�����ִ��ʱ��ȣ�,������ļ�
	void outputNode(char* fileName,SORT_TYPE st = CallTimes);
	//������
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

/// �����ܽڵ��װ
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
///�ֹ�����,�첽������²�ʱ����ʹ��
class FCL_CLASS  ManuProfile: public CSingleton<ManuProfile>
{
	typedef  multiThread   threadMode;  ///�̰߳�ȫ
private:
	struct ProNode
	{
		uint64 m_startTime;  //��ʼʱ��
		uint32 m_Calls;      ///���ô���
		double m_totalTime;  ///��ʱ
		char* m_name;       ///�ڵ�����
		uint32 m_id;      ///�ڵ��ʶid
		string m_nameid;   ///�ڵ��ʶ��
		char*  m_fileName;   //�����ļ���
		uint32 m_lineNumber;  ///�����к�
		double m_averageTime; ///ƽ��ִ��ʱ��
		bool   m_isFinish;    ///�Ƿ��Ѿ���������ֹ���ͳ��
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
		set<ProNode*>   m_resultSet;     ///�����ռ����Ľ����
		typedef  map<uint32,ProNode*>::iterator intListItr;
		typedef  map<string,ProNode*>::iterator strListItr;
		threadMode::mutex  m_mutex;  ///ͬ����
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
		 //�ڵ�ɾ��
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
