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
		P2P,		//单人聊天
	};

	struct TestArgs
	{
		//消息类型
		uint32_t		type = P2P;

		//调用窗口
		HWND			hwnd = NULL;

		//配置文件
		std::string		config = SUPER_MIMA;

		//消息长度
		uint32_t		length = 1000;

		//发送间隔
		uint32_t		interval = 100;
	};

	struct TestResult
	{
		FRIEND_INFO*	friend_info;
		
		//发包次数
		uint32_t		send_cnt;
	};

	ImTest();
	~ImTest();

	static ImTest*		GetInstance();

	//解析命令行
	bool		ParseCommandLine(std::string& user, std::string& password);

	void		StartTest();
	bool		StartTest(const TestArgs& ta);
	void		StopTest();

	bool		Running();

	//总运行时间
	int64_t		GetRunTime() { 
		using namespace std::chrono;
		return duration_cast<milliseconds>(steady_clock::now() - begin_time_).count();
	}

	//发送次数
	uint32_t	GetCount() { return std::atomic_load(&cnt_); }
	uint32_t	AddCount() { return cnt_.fetch_add(1); }

	void		RecvP2pMsg(const FRIEND_INFO& fi, const XT_MSG& msg);

private:
	void		ReadConfig();
	void		TestThread();

	void		OpenLog();
	void		CloseLog();

	void		WriteRecord();

	//会话结束后的回调
	void		TestCallBack(const TestResult& tr);

private:
	static std::once_flag			s_instance_flag;
	static ImTest*					s_ImTest;

private:
	bool							run_ = false;
	std::atomic<uint32_t>			cnt_;

	std::string						user_;
	std::string						config_ = SUPER_MIMA;
	HWND							hwnd_ = NULL;					//消息接受窗口
	uint32_t						type_ = P2P;
	uint32_t						length_ = 1000;					//消息长度
	uint32_t						interval_ = 100;				//发送间隔(ms)
	uint32_t						duration_ = 60 * 10;			//测试时间(s)

	std::vector<FRIEND_INFO*>		friends_;

	using time_point_t = std::chrono::steady_clock::time_point;
	time_point_t					begin_time_;

	std::shared_ptr<FILE>			log_file_;				//统计日志
	std::unique_ptr<std::thread>	thread_;
	boost::asio::io_service			io_service_;

	//已收消息统计////////////////////////////////////////////////////////////////////
	using msgtype_cnt_map_t = std::map<uint8_t, uint32_t>;
	using friend_msg_map_t = std::map < std::string, msgtype_cnt_map_t > ;
	friend_msg_map_t				friend_msg_map_;
};

#endif