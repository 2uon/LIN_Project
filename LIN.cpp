#include "pch.h"
#include "LIN.h"


int LIN::wLIN_connect() {
	// Ŭ���̾�Ʈ ����
	result = LIN_RegisterClient(const_cast<char*>("Client"), 0, &hClient);
	if (result != errOK) {
		printf("LIN Ŭ���̾�Ʈ ���� ���� : %d \n", result);
		return -1;
	}
	else {
		printf("LIN Ŭ���̾�Ʈ ���� ���� \n");
	}

	// �ϵ���� ���
	for (HLINHW hw : HW_TYPES) {
		result = LIN_ConnectClient(hClient, hw);
		if (result == errOK) {
			printf("LIN �ϵ���� ��� ���� :%d \n", hw);
			break;
		}
	}
	if (result != errOK) {
		printf("LIN �ϵ���� ��� ���� : %d \n", result);
		return -2;
	}

	// �ϵ���� �ʱ�ȭ (������ ���, 19200 bit rate)
	result = LIN_InitializeHardware(hClient, hHw, modMaster, 19200);
	if (result != errOK) {
		printf("LIN �ϵ���� �ʱ�ȭ ���� : %d \n", result);
		LIN_DisconnectClient(hClient, hHw);
		LIN_RemoveClient(hClient);
		return -3;
	}
	else {
		printf("LIN �ϵ���� �ʱ�ȭ ���� \n");
	}

	// ������ id �ʱ�ȭ
	for (BYTE frameId : FRAME_IDS) {
		result = LIN_RegisterFrameId(hClient, hHw, frameId, frameId);
		if (result == errOK) {
			printf("������ ID �߰� ���� : %d \n", result);
		}
		else {
			printf("������ ID �߰� ���� : %d \n", frameId);
		}
	}

	// ������ ��Ʈ�� ����
	for (int i = 0; i < LIN_MAX_SCHEDULES; i++) {
		Frames[i] = {};
		Frames[i].Direction = Directions[i];
		Frames[i].ChecksumType = cstEnhanced;
		Frames[i].Length = Lengths[i];
		memset(Frames[i].InitialData, 0, sizeof(data1));

		result = LIN_SetFrameEntry(hClient, hHw, &Frames[i]);
		if (result == errOK) {
			printf("������ ��Ʈ�� �߰� ���� : %d \n", result);
		}
		else {
			printf("������ ��Ʈ�� �߰� ���� : %d \n", Frames[i].FrameId);
		}
	}

	// ������ ���� (����)
	Schedules[0].Type = sltUnconditional;
	Schedules[0].FrameId[0] = FRAME_IDS[0];
	Schedules[0].Delay = delay;

	Schedules[1].Type = sltUnconditional;
	Schedules[1].FrameId[0] = FRAME_IDS[1];
	Schedules[1].Delay = delay;

	result = LIN_SetSchedule(hClient, hHw, 0, Schedules, 2);

	if (result == errOK) {
		printf("������ ���� ���� : %d \n", result);
	}
	else {
		printf("������ ���� ���� \n");
	}
	
	return 0;
}
int LIN::wLIN_start() {
	result = LIN_StartSchedule(hClient, hHw, 0);
	if (result == errOK) {
		printf("������ ���� ���� : %d \n", result);
	}
	else {
		printf("������ ���� ���� \n");
	}

	

	return 0;
}

int LIN::wLIN_pause() {
	result = LIN_SuspendSchedule(hClient, hHw);
	printf("������ ���� ���� \n");
	return 0;
}

int LIN::wLIN_clear() {
	result = LIN_DeleteSchedule(hClient, hHw, schedule_position);
	printf("������ ���� ���� \n");
	
	result = LIN_DisconnectClient(hClient, hHw);
	printf("���� ���� ���� \n");
	return 0;
}