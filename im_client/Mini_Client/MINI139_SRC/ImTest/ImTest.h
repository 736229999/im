#pragma once

#ifdef _IM_UNIT_TEST

#include <mutex>
#include <string>
#include <atomic>
#include <vector>
#include <thread>
#include <cstdio>
#include <map>
#include <boost/asio.hpp>

#define WM_FINISH_TEST	WM_USER + 0x4000
#define WM_SEND_COUNT	WM_USER + 0x4001
#define SUPER_MIMA		"123456"

#define IMTEST ImTest::GetInstance()

struct FRIEND_INFO;
struct XT_MSG;

class ImTest
{
public:
	enum TestType : uint32_t
	{
		P2P,		//��������
	};

	struct TestArgs
	{
		//��Ϣ����
		uint32_t		type = P2P;

		//���ô���
		HWND			hwnd = NULL;

		//�����ļ�
		std::string		config = SUPER_MIMA;

		//��Ϣ����
		uint32_t		length = 1000;

		//���ͼ��
		uint32_t		interval = 100;
	};

	struct TestResult
	{
		FRIEND_INFO*	friend_info;
		
		//��������
		uint32_t		send_cnt;
	};

	ImTest();
	~ImTest();

	static ImTest*		GetInstance();

	//����������
	bool		ParseCommandLine(std::string& user, std::string& password);

	void		StartTest();
	bool		StartTest(const TestArgs& ta);
	void		StopTest();

	bool		Running();

	//������ʱ��
	int64_t		GetRunTime() { 
		using namespace std::chrono;
		return duration_cast<milliseconds>(steady_clock::now() - begin_time_).count();
	}

	//���ʹ���
	uint32_t	GetCount() { return std::atomic_load(&cnt_); }
	uint32_t	AddCount() { return cnt_.fetch_add(1); }

	void		RecvP2pMsg(const FRIEND_INFO& fi, const XT_MSG& msg);

private:
	void		ReadConfig();
	void		TestThread();

	void		OpenLog();
	void		CloseLog();

	void		WriteRecord();

	//�Ự������Ļص�
	void		TestCallBack(const TestResult& tr);

private:
	static std::once_flag			s_instance_flag;
	static ImTest*					s_ImTest;

private:
	bool							run_ = false;
	std::atomic<uint32_t>			cnt_;

	std::string						user_;
	std::string						config_ = SUPER_MIMA;
	HWND							hwnd_ = NULL;					//��Ϣ���ܴ���
	uint32_t						type_ = P2P;
	uint32_t						length_ = 1000;					//��Ϣ����
	uint32_t						interval_ = 100;				//���ͼ��(ms)
	uint32_t						duration_ = 60 * 10;			//����ʱ��(s)

	std::vector<FRIEND_INFO*>		friends_;

	using time_point_t = std::chrono::steady_clock::time_point;
	time_point_t					begin_time_;

	std::shared_ptr<FILE>			log_file_;				//ͳ����־
	std::unique_ptr<std::thread>	thread_;
	boost::asio::io_service			io_service_;

	//������Ϣͳ��////////////////////////////////////////////////////////////////////
	using msgtype_cnt_map_t = std::map<uint8_t, uint32_t>;
	using friend_msg_map_t = std::map < std::string, msgtype_cnt_map_t > ;
	friend_msg_map_t				friend_msg_map_;
};

#endif