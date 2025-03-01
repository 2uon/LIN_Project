#pragma once
#include "PLinApi.h"

class LIN
{
public:
	int wLIN_connect();
	int wLIN_start();
	int wLIN_pause();
	int wLIN_clear();

private:
	TLINError result;

	HLINCLIENT hClient;
	HLINHW hHw;

	HLINHW HW_TYPES[3] = { LIN_HW_TYPE_USB_PRO, LIN_HW_TYPE_USB_PRO_FD, LIN_HW_TYPE_PLIN_USB };
	BYTE FRAME_IDS[LIN_MAX_SCHEDULES] = { 0x18, 0x19 };

	TLINFrameEntry Frames[LIN_MAX_SCHEDULES] = {};
	BYTE Directions[LIN_MAX_SCHEDULES] = { dirPublisher, dirSubscriber };
	int Lengths[LIN_MAX_SCHEDULES] = { 8, 8 };
	TLINScheduleSlot Schedules[LIN_MAX_SCHEDULES] = {};

	TLINRcvMsg rcvMsg = {};
	int delay = 20;

	BYTE data1[8] = { 0, 0, 0, 1, 0, 0, 0, 0 };
	BYTE data2[8] = { 0, 0, 0, 1, 0, 0, 0, 0 };

	int schedule_position = 0;

	
};

