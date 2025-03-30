
// LIN_ProjectDlg.h: 헤더 파일
//
#include "StdClass.h"
#include "LIN_Project.h"
#include "ChartApi/ChartCtrl.h"
#include "ChartApi/ChartLineSerie.h"
#include <thread>

#pragma once


// CLINProjectDlg 대화 상자
class CLINProjectDlg : public CDialogEx
{
// 생성입니다.
public:
	CLINProjectDlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_LIN_PROJECT_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.

	int wLIN_start();
	int wLIN_pause();

	int wLIN_connect();
	int wLIN_clear();
	
	void wReadData();

	// 파싱 함수
	int w_LDF_parse(string filePath);
	void w_CSV_parse(string filePath);

	void w_Parser_Config(string& line);
	void w_Parser_Nodes(string& line);
	void w_Parser_Signals(string& line);
	void w_Parser_DiagnosticSignals(string& line);

	// 그래프 함수
	void initGraph(int index);
	void wTimer();
	void wGraphDraw(ULONG64 logData, double logTime, int index);

	// 변수 초기화 함수
	void initPharam();


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
	afx_msg void OnBnClickedOpenlog();
	afx_msg void OnCbnSelchangeSchedule();
	afx_msg void OnCbnSelchangeFrameid();
	afx_msg void OnLvnItemchangedSignallist(NMHDR* pNMHDR, LRESULT* pResult);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnBnClickedConnect();
	afx_msg void OnBnClickedDisconnect();
	afx_msg void OnClose();

	// 파일
	string openFileName;
	string openFileExt;

	// 로그 파일
	string logFileName;
	ofstream log_file;
	

	// 스레드
	CWinThread* m_pThread1, *m_pThread2;
	bool m_bThreadRunning;
	static UINT WINAPI wTimerThread(LPVOID pParam);
	static UINT WINAPI wReadDataThread(LPVOID pParam);

	// 파일 데이터
	float w_LIN_protocol_version;
	float w_LIN_language_version;
	float w_LIN_speed;

	// Nodes
	string w_Client_name;
	int w_delay;
	int w_IFS;
	int slave_cnt = 0;
	string w_Slave_names[LIN_MAX_SCHEDULES];

	// Signals
	struct w_Signal {
		string name;
		int dataLength;
		int defaultValue;
		string txNode;
		string rxNode;
	};
	vector<w_Signal> w_Signals;

	// Diagnostic_signals
	struct w_DiagnosticSignal {
		string name;
		int size;
		int value;
	};
	vector<w_DiagnosticSignal> w_DiagnosticSignals;

	// Frames
	struct w_DataStruct {
		string name;
		int start;
	};
	struct w_Frame {
		string name;
		int id;
		string txNode;
		int length;
		vector<w_DataStruct> w_Data;
	};
	vector<w_Frame> w_Frames;

	// Diagnostic_frames
	struct w_DiagnosticFrame {
		string name;
		int id;
		vector<w_DataStruct> w_Data;
	};
	vector<w_DiagnosticFrame> w_DiagnosticFrames;

	// Node_attributes
	struct w_NodeAttribute {
		string name;
		string linProtocol;
		int configuredNAD;
		int initialNAD;
		int productID1;
		int productID2;
		int productVersion;
		string responseErr;
		int P2_min;
		int ST_min;
		int N_As_timeout;
		int N_Cr_timeout;
		vector<string> configurable_frames;
	};
	vector<w_NodeAttribute> w_NodeAttributes;

	// Schedule_tables
	struct w_Schedule {
		string name;
		int delay;
	};
	struct w_ScheduleTable {
		string name;
		vector<w_Schedule> schedule;
	};
	vector<w_ScheduleTable> w_ScheduleTables;

	// Signal_encoding_types 
	struct w_LogicalValue {
		int value;
		string description;
	};
	struct w_PhysicalValue {
		int minValue;       // 최소값
		int maxValue;       // 최대값
		int scale;          // 스케일 값
		int offset;         // 오프셋 값
		string unit;	    // 단위 (예: "STEP", "HW_VER" ...)
	};
	struct w_SignalEncoding {
		string name;  // 신호 이름 (예: "C_3way1_LinError")
		vector<w_LogicalValue> logicalValues; // 논리 값 리스트
		w_PhysicalValue physicalValue; // 물리 값 (존재하지 않을 경우 기본값)
		bool isPhysical;   // 물리 값인지 여부 (true: physical, false: logical)
	};
	vector<w_SignalEncoding> w_SignalEncodings;

	// Signal_representation
	//struct w_SignalRepresentation {
	//	string name;
	//	vector<string> signals;
	//};
	//vector<w_SignalRepresentation> w_SignalRepresentations;

	// LIN
	TLINError result;

	HLINCLIENT hClient;
	HLINHW hHw;

	HLINHW HW_TYPES[3] = { LIN_HW_TYPE_USB_PRO, LIN_HW_TYPE_USB_PRO_FD, LIN_HW_TYPE_PLIN_USB };
	BYTE FRAME_IDS[LIN_MAX_SCHEDULES] = {};

	vector<TLINFrameEntry> Frames;

	vector<string> FrameNames;
	vector<BYTE> FrameIDs;
	int schedulesSize = 0;

	struct w_Schedules {
		int schedulesPosition;
		int size = 0;
		TLINScheduleSlot Schedule[LIN_MAX_SCHEDULES] = {};
	};
	w_Schedules Schdules[LIN_MAX_SCHEDULES];

	
	int frameId_global;
	int frameLength_global;

	TLINRcvMsg rcvMsg = {};
	int delay = 200;
	CString cst;
	CString flag;

	BYTE sendData[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };
	BYTE data[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };

	int schedule_position = 0;
	BOOL onPause = false;
	BOOL onClear = false;

	// UI 접근
	CString tx;
	CStatic mRx;

	CEdit mTx0;
	CEdit mTx1;
	CEdit mTx2;
	CEdit mTx3;
	CEdit mTx4;
	CEdit mTx5;
	CEdit mTx6;
	CEdit mTx7;
	CEdit* mTx[8] = { &mTx0, &mTx1, &mTx2, &mTx3, &mTx4, &mTx5, &mTx6, &mTx7 };

	CEdit mDelay;
	CEdit mFileName;

	CListCtrl mTraceList;
	CListCtrl mSignalList;

	CButton mCheckSig;

	CComboBox mFrameId;
	CComboBox mFrameName;
	CComboBox mSchedule;
	// 레이블
	CEdit mSig[9];

	// 그래프
	CChartCtrl mGraph[9];
	//CChartLineSerie* pGraph[9];
	int GRAPHS[9] = {
		IDC_Graph1, IDC_Graph2, IDC_Graph3,
		IDC_Graph4, IDC_Graph5, IDC_Graph6,
		IDC_Graph7, IDC_Graph8, IDC_Graph9
	};
	CChartXYSerie* pSeries[9];

	// 데이터
	vector<int> graphSig = {}; // 선택된 그래프(의 신호의 인덱스) 벡터

	struct signalStartEnd {
		int sigIndex;
		string name;	// 신호 이름
		int start;		// 신호의 비트 시작 인덱스
		int end;		// 신호의 비트 끝 인덱스
	};
	map<int, vector<signalStartEnd>> signalEncodings; // Frame ID, 신호 구조

	struct logData {
		ULONG64 data;
		double time;
	};
	map<int, vector<logData>> logDatas; // 데이터(8byte->int)

	double time = 0;

	// graphData로 이루어진 벡터(각 신호에 대한 데이터 저장)을 벡터로 모음.
	struct graphData {
		string name;
		double time;
		double value;
	};
	vector<graphData> graphDatas;

	struct graphSetting {
		int id;
		int start;
		int end;
	};
	vector<graphSetting> gSettings;
	afx_msg void OnCbnSelchangeFramename();
	CListCtrl mSignalDataList;
	CButton mHex;
	afx_msg void OnBnClickedHex();

	int iSavedItem, iSavedSubItem;
	CEdit mMod;
	afx_msg void OnBnClickedApply();
	afx_msg void OnLvnItemchangedSignaldatalist(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnEnChangeTx();
	ULONG64 sData = 0;
	CButton mLogSave;
	afx_msg void OnBnClickedLogsave();
	CButton mLogViewer;
};
