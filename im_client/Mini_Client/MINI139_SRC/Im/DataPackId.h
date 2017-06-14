#pragma once

#define XY_BEG namespace xy{
#define XY_END }

#include "ClientProtocol.h"

XY_BEG


struct MSG_ID{
	uint32		nCId;	// Ⱥid
	uint32		nId;	// ̸����id
	uint32		nTime;	// ʱ��
	uint8		nMs;	// ����
	bool operator == (const MSG_ID & msg)
	{
		return (nCId == msg.nCId) && (nId == msg.nId) && (nTime == msg.nTime) && (nMs == msg.nMs);
	}
};







XY_END