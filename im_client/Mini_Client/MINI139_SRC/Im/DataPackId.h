#pragma once

#define XY_BEG namespace xy{
#define XY_END }

#include "ClientProtocol.h"

XY_BEG


struct MSG_ID{
	uint32		nCId;	// 群id
	uint32		nId;	// 谈话者id
	uint32		nTime;	// 时间
	uint8		nMs;	// 毫秒
	bool operator == (const MSG_ID & msg)
	{
		return (nCId == msg.nCId) && (nId == msg.nId) && (nTime == msg.nTime) && (nMs == msg.nMs);
	}
};







XY_END