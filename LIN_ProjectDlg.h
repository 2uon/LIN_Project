
// LIN_ProjectDlg.h: 헤더 파일
//
#include "PLinApi.h"
#include <thread>

#pragma once


// CLINProjectDlg 대화 상자
class CLINProjectDlg : public CDialogEx
{
// 생성입니다.
public:
	CLINProjectDlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.
	~CLINProjectDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_LIN_PROJECT_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.

	int wLIN_connect();
	int wLIN_start();
	int wLIN_pause();
	int wLIN_clear();
	void wReadData();

// 구현입니다.
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedStop();
	afx_msg void OnBnClickedPause();
	afx_msg void OnBnClickedStart();
	afx_msg void OnBnClickedSend();

	// 스레드
	CWinThread* m_pThread;
	bool m_bThreadRunning;

	static UINT WINAPI wReadDataThread(LPVOID pParam);

	// LIN
	TLINError result;

	HLINCLIENT hClient;
	HLINHW hHw;

	HLINHW HW_TYPES[3] = { LIN_HW_TYPE_USB_PRO, LIN_HW_TYPE_USB_PRO_FD, LIN_HW_TYPE_PLIN_USB };
	BYTE FRAME_IDS[LIN_MAX_SCHEDULES] = { 0x18, 0x19 };

	TLINFrameEntry Frames[LIN_MAX_SCHEDULES] = {};
	BYTE Directions[LIN_MAX_SCHEDULES] = { dirPublisher, dirSubscriber };
	int Lengths[LIN_MAX_SCHEDULES] = { 8, 8 };
	TLINScheduleSlot Schedules[LIN_MAX_SCHEDULES] = {};

	BYTE frameId = 0x18; // 예시

	TLINRcvMsg rcvMsg = {};
	int delay = 200;
	CString cst;
	CString flag;

	BYTE sendData[8] = { 0, 0, 0, 1, 0, 0, 0, 0 };
	BYTE data[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };

	int schedule_position = 0;
	BOOL onPause = false;
	BOOL onClear = false;

	CString progress;
	CString errCode;
	CString tx;
	CString rx;

	CStatic mProgress;
	CStatic mErrCode;
	CStatic mRx;
	CEdit mTx0;
	CEdit mTx1;
	CEdit mTx2;
	CEdit mTx3;
	CEdit mTx4;
	CEdit mTx5;
	CEdit mTx6;
	CEdit mTx7;
};
