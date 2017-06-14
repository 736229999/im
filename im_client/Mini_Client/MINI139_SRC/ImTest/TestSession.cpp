
#include "stdafx.h"
#ifdef _IM_UNIT_TEST
#include "..\IMClient.h"
#include "afxdialogex.h"
#include <functional>
#include <fstream>
#include <string>
#include <set>
#include "TestSession.h"
#include "ImTest.h"


TestSession::TestSession(boost::asio::io_service& ios,
	FRIEND_INFO* fi, int32_t len, int32_t interval, test_res_t callback)
	: friend_(fi)
	, interval_(interval)
	, timer_(ios)
	, callback_(callback)
{
	InitMsg(len);
	timer_.expires_from_now(boost::posix_time::seconds(0));
}

TestSession::~TestSession()
{

}

void TestSession::Run()
{
	auto self(shared_from_this());
	timer_.expires_at(timer_.expires_at() + interval_);
	timer_.async_wait(
		[this, self](const boost::system::error_code& ec)
		{
			if (!ec && IMTEST->Running())
			{
				g_pProtocol->SendP2PMsg(msg_, CIMProtocol::SMT_SERVER);
				IMTEST->AddCount();
				++send_cnt_;
				Run();
			}
			else
			{
				//²âÊÔ½áÊø
				ImTest::TestResult tr;
				tr.friend_info = friend_;
				tr.send_cnt = send_cnt_;
				callback_(tr);
			}
		}
	);
}

void TestSession::InitMsg(int32_t len)
{
	memset(&msg_, 0, sizeof(msg_));

	msg_.ver = XTREAM_CLIENT_VERSION;
	//	msg_.face		= g_ImUserInfo.face;
	msg_.from_id = g_ImUserInfo.GetId();

	msg_.fontColor = g_ImFont.fontColor;
	msg_.fontSize = g_ImFont.fontSize;
	msg_.fontStyle = g_ImFont.flags;
	strcpy(msg_.fontName, g_ImFont.fontName);
	strcpy(msg_.from_nickname, g_ImUserInfo.nickname);

	msg_.send_time = /*time(0)*/g_LocalData.GetTimeNow();

	msg_.to_id = friend_->id;
	msg_.dest_ip = friend_->ip;
	msg_.dest_port = friend_->port;
	msg_.msg_type = 0;
	msg_.data_type = 0;
	memset(msg_.data, 'a', len);
	msg_.data_len = len + 1;
}

#endif