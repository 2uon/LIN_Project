#include "pch.h"
#include "LIN.h"


int LIN::wLIN_connect() {
	// 클라이언트 생성
	result = LIN_RegisterClient(const_cast<char*>("Client"), 0, &hClient);
	if (result != errOK) {
		printf("LIN 클라이언트 생성 실패 : %d \n", result);
		return -1;
	}
	else {
		printf("LIN 클라이언트 생성 성공 \n");
	}

	// 하드웨어 등록
	for (HLINHW hw : HW_TYPES) {
		result = LIN_ConnectClient(hClient, hw);
		if (result == errOK) {
			printf("LIN 하드웨어 등록 성공 :%d \n", hw);
			break;
		}
	}
	if (result != errOK) {
		printf("LIN 하드웨어 등록 실패 : %d \n", result);
		return -2;
	}

	// 하드웨어 초기화 (마스터 모드, 19200 bit rate)
	result = LIN_InitializeHardware(hClient, hHw, modMaster, 19200);
	if (result != errOK) {
		printf("LIN 하드웨어 초기화 실패 : %d \n", result);
		LIN_DisconnectClient(hClient, hHw);
		LIN_RemoveClient(hClient);
		return -3;
	}
	else {
		printf("LIN 하드웨어 초기화 성공 \n");
	}

	// 프레임 id 초기화
	for (BYTE frameId : FRAME_IDS) {
		result = LIN_RegisterFrameId(hClient, hHw, frameId, frameId);
		if (result == errOK) {
			printf("프레임 ID 추가 실패 : %d \n", result);
		}
		else {
			printf("프레임 ID 추가 성공 : %d \n", frameId);
		}
	}

	// 프레임 엔트리 설정
	for (int i = 0; i < LIN_MAX_SCHEDULES; i++) {
		Frames[i] = {};
		Frames[i].Direction = Directions[i];
		Frames[i].ChecksumType = cstEnhanced;
		Frames[i].Length = Lengths[i];
		memset(Frames[i].InitialData, 0, sizeof(data1));

		result = LIN_SetFrameEntry(hClient, hHw, &Frames[i]);
		if (result == errOK) {
			printf("프레임 엔트리 추가 실패 : %d \n", result);
		}
		else {
			printf("프레임 엔트리 추가 성공 : %d \n", Frames[i].FrameId);
		}
	}

	// 스케줄 설정 (더미)
	Schedules[0].Type = sltUnconditional;
	Schedules[0].FrameId[0] = FRAME_IDS[0];
	Schedules[0].Delay = delay;

	Schedules[1].Type = sltUnconditional;
	Schedules[1].FrameId[0] = FRAME_IDS[1];
	Schedules[1].Delay = delay;

	result = LIN_SetSchedule(hClient, hHw, 0, Schedules, 2);

	if (result == errOK) {
		printf("스케줄 설정 실패 : %d \n", result);
	}
	else {
		printf("스케줄 설정 성공 \n");
	}
	
	return 0;
}
int LIN::wLIN_start() {
	result = LIN_StartSchedule(hClient, hHw, 0);
	if (result == errOK) {
		printf("스케줄 시작 실패 : %d \n", result);
	}
	else {
		printf("스케줄 시작 성공 \n");
	}

	

	return 0;
}

int LIN::wLIN_pause() {
	result = LIN_SuspendSchedule(hClient, hHw);
	printf("스케줄 정지 성공 \n");
	return 0;
}

int LIN::wLIN_clear() {
	result = LIN_DeleteSchedule(hClient, hHw, schedule_position);
	printf("스케줄 정지 성공 \n");
	
	result = LIN_DisconnectClient(hClient, hHw);
	printf("연결 해제 성공 \n");
	return 0;
}