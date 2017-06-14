#pragma once

#ifdef _IM_UNIT_TEST

#include <memory>
#include <thread>
#include <chrono>
#include <vector>
#include <cstdint>
#include <functional>
#include "boost/asio.hpp"
#include "boost/noncopyable.hpp"
#include "ImTest.h"

using test_res_t = std::function < void(ImTest::TestResult&) > ;

class TestSession :
	public std::enable_shared_from_this<TestSession>, boost::noncopyable
{
public:
	TestSession(boost::asio::io_service&,
		FRIEND_INFO*, int32_t len, int32_t interval, test_res_t callback);
	~TestSession();

	void		Run();

private:
	void		InitMsg(int32_t len);

	FRIEND_INFO*				friend_;
	XT_MSG						msg_;
	boost::asio::deadline_timer	timer_;
	boost::posix_time::millisec	interval_;				//·¢ËÍ¼ä¸ô
	uint32_t					send_cnt_ = 0;

	test_res_t					callback_;
};

#endif