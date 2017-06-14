#include "stdafx.h"

#ifdef _IM_UNIT_TEST

#include "ImTest.h"
#include <direct.h>
#include <fstream>
#include <set>
#include <functional>
#include "..\IMClient.h"
#include "TestSession.h"
#include "boost/program_options.hpp"

using namespace std::placeholders;
std::once_flag	ImTest::s_instance_flag;
ImTest* ImTest::s_ImTest = nullptr;

ImTest::ImTest()
{
	std::string logpath(__argv[0]);
	logpath = logpath.substr(0, logpath.find_last_of("\\") + 1) + "testlog";
	_mkdir(logpath.c_str());
}

ImTest::~ImTest()
{
}

ImTest* ImTest::GetInstance()
{
	std::call_once(s_instance_flag, []() {ASSERT(s_ImTest == nullptr); s_ImTest = new ImTest; });

	return s_ImTest;
}

bool ImTest::ParseCommandLine(std::string& user, std::string& password)
{
	using namespace boost::program_options;

	options_description opts("im test");

	std::string config;
	uint32_t length(0), interval(0), duration(0);

	opts.add_options()
		("user,u", value<std::string>(&user), "用户名")
		("password,p", value<std::string>(&password), "密码")
		("config,c", value<std::string>(&config)->default_value("123456"), "配置文件")
		("length,l", value<uint32_t>(&length), "消息长度")
		("interval,i", value<uint32_t>(&interval), "发送间隔")
		("duration,d", value<uint32_t>(&duration), "测试时间");

	char ** p = __argv;
	variables_map vm;
	store(parse_command_line(__argc, __argv, opts), vm);
	notify(vm);

	if (user.empty() || password.empty())
	{
		//CMessageBox::Model(NULL, "用户名和密码不能为空", NULL, MB_OK);
		return false;
	}
	else
	{
		user_ = user;
	}
	
	if (length != 0)
	{
		length_ = length;
	}

	if (interval != 0)
	{
		interval_ = interval;
	}

	if (duration == 0)
	{
		//CMessageBox::Model(NULL, "测试时间不能为0", NULL, MB_OK);
		return false;
	}
	else
	{
		duration_ = duration;
	}

	if (config.empty())
	{
		config_ = config;
	}

	return true;
}

bool ImTest::Running()
{
	using namespace std::chrono;
	int64_t t = duration_cast<seconds>(steady_clock::now() - begin_time_).count();
	return (run_ && t < duration_);
}

void ImTest::StartTest()
{
	return;
	ReadConfig();
	cnt_.store(0);
	run_ = true;
	thread_.reset(new std::thread(std::bind(&ImTest::TestThread, this)));
	begin_time_ = std::chrono::steady_clock::now();
}

bool ImTest::StartTest(const TestArgs& ta)
{
	if (thread_.get() != nullptr)
	{
		CMessageBox::Model(NULL, _T("正在运行，请先停止"), NULL, 0);
		return false;
	}

	config_ = ta.config;
	hwnd_ = ta.hwnd;
	type_ = ta.type;
	length_ = ta.length;
	interval_ = ta.interval;

	StartTest();

	return true;
}

void ImTest::StopTest()
{
	run_ = false;
	if (thread_.get() != nullptr)
	{
		thread_->join();
		thread_.reset();
	}

	_exit(0);
}

void ImTest::ReadConfig()
{
	friends_.clear();

	if (config_ == _T(SUPER_MIMA))
	{
		for (auto p : g_lsFriend)
		{
			if (g_ImUserInfo.GetId() != p->id)
			{
				friends_.push_back(p);
			}
		}
	}
	else
	{

		std::ifstream ifs(config_);
		if (!ifs)
		{
			CMessageBox::Model(NULL, "无法读取配置文件", NULL, MB_OK);
			_exit(1);
			return;
		}

		std::set<std::string> friend_names;
		friend_names.insert("yaoxing");
		std::string line;
		while (ifs >> line)
		{
			friend_names.insert(line);
		}

		for (auto p : g_lsFriend)
		{
			std::set<string>::iterator ifind = friend_names.find(p->username);
			if (ifind != friend_names.end())
			{
				friends_.push_back(p);
			}
		}
	}
}

void ImTest::TestThread()
{
	OpenLog();
	io_service_.reset();

	for (auto p : friends_)
	{
		std::shared_ptr<TestSession> s(new TestSession(
			io_service_, p, length_, interval_, std::bind(&ImTest::TestCallBack, this, _1)));
		s->Run();
	}
	io_service_.run();

	if (hwnd_ != NULL)
	{
		::PostMessage(hwnd_, WM_FINISH_TEST, 0, 0);
	}

	CloseLog();
	//_exit(0);
}

void ImTest::TestCallBack(const TestResult& tr)
{
	if (hwnd_ != NULL)
	{
		::PostMessage(hwnd_, WM_SEND_COUNT, tr.send_cnt, (LPARAM)tr.friend_info);
	}
	fprintf_s(&*log_file_, "%-20s %10d\n", tr.friend_info->username, tr.send_cnt);
}

void ImTest::OpenLog()
{
	std::string f(__argv[0]);
	char n[256];
	::GetModuleFileName(NULL, n, 256);
	f = f.substr(0, f.find_last_of("\\") + 1) + "testlog\\";

// 	auto tt = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
// 	struct tm* ptm = localtime(&tt);
// 	char date[60] = { 0 };
// 	sprintf(date, "%d-%02d-%02d-%02d:%02d:%02d.txt",
// 		(int)ptm->tm_year + 1900, (int)ptm->tm_mon + 1, (int)ptm->tm_mday,
// 		(int)ptm->tm_hour, (int)ptm->tm_min, (int)ptm->tm_sec);
// 	f += user_ + date;

	f += user_ + ".txt";
	FILE *p = NULL;
	int a = fopen_s(&p, f.c_str(), "w");
	log_file_.reset(p, fclose);
}

void ImTest::CloseLog()
{
	WriteRecord();
	log_file_.reset();
}

void ImTest::WriteRecord()
{
	fprintf_s(&*log_file_, "总数 %d\n", GetCount());
	fprintf_s(&*log_file_, "运行时间 %dms\n", GetRunTime());

	fprintf_s(&*log_file_, "\n接受记录\n");
	for (auto i : friend_msg_map_)
	{
		fprintf_s(&*log_file_, "--------------%s--------\n", i.first.c_str());
		for (auto j: i.second)
		{
			fprintf_s(&*log_file_, "%2d  %d\n", j.first, j.second);
		}
	}
	auto t = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
	tm* ptm = localtime(&t);
	fprintf_s(&*log_file_, "记录时间： %d-%02d-%02d-%02d:%02d:%02d.txt",
		(int)ptm->tm_year + 1900, (int)ptm->tm_mon + 1, (int)ptm->tm_mday,
		(int)ptm->tm_hour, (int)ptm->tm_min, (int)ptm->tm_sec);
}

void ImTest::RecvP2pMsg(const FRIEND_INFO& fi, const XT_MSG& msg)
{
	friend_msg_map_[fi.username][msg.data_type] += strlen(msg.data);
}

#endif