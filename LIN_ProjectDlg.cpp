﻿
// LIN_ProjectDlg.cpp: 구현 파일
//

#include "pch.h"
#include "framework.h"
#include "LIN_ProjectDlg.h"
#include "afxdialogex.h"
#include <ctime>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
protected:
	DECLARE_MESSAGE_MAP()
public:
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CLINProjectDlg 대화 상자



CLINProjectDlg::CLINProjectDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_LIN_PROJECT_DIALOG, pParent), m_pThread1(nullptr), m_pThread2(nullptr), m_bThreadRunning(false)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CLINProjectDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_Tx0, mTx0);
	DDX_Control(pDX, IDC_Tx1, mTx1);
	DDX_Control(pDX, IDC_Tx2, mTx2);
	DDX_Control(pDX, IDC_Tx3, mTx3);
	DDX_Control(pDX, IDC_Tx4, mTx4);
	DDX_Control(pDX, IDC_Tx5, mTx5);
	DDX_Control(pDX, IDC_Tx6, mTx6);
	DDX_Control(pDX, IDC_Tx7, mTx7);
	DDX_Control(pDX, IDC_TraceList, mTraceList);
	DDX_Control(pDX, IDC_FrameId, mFrameId);
	DDX_Control(pDX, IDC_Delay, mDelay);
	DDX_Control(pDX, IDC_LogFile, mFileName);
	DDX_Control(pDX, IDC_SignalList, mSignalList);
	DDX_Control(pDX, IDC_Schedule, mSchedule);
	DDX_Control(pDX, IDC_FrameName, mFrameName);
	DDX_Control(pDX, IDC_Graph1, mGraph[0]);
	DDX_Control(pDX, IDC_Graph2, mGraph[1]);
	DDX_Control(pDX, IDC_Graph3, mGraph[2]);
	DDX_Control(pDX, IDC_Graph4, mGraph[3]);
	DDX_Control(pDX, IDC_Graph5, mGraph[4]);
	DDX_Control(pDX, IDC_Graph6, mGraph[5]);
	DDX_Control(pDX, IDC_Graph7, mGraph[6]);
	DDX_Control(pDX, IDC_Graph8, mGraph[7]);
	DDX_Control(pDX, IDC_Graph9, mGraph[8]);
	DDX_Control(pDX, IDC_Sig1, mSig[0]);
	DDX_Control(pDX, IDC_Sig2, mSig[1]);
	DDX_Control(pDX, IDC_Sig3, mSig[2]);
	DDX_Control(pDX, IDC_Sig4, mSig[3]);
	DDX_Control(pDX, IDC_Sig5, mSig[4]);
	DDX_Control(pDX, IDC_Sig6, mSig[5]);
	DDX_Control(pDX, IDC_Sig7, mSig[6]);
	DDX_Control(pDX, IDC_Sig8, mSig[7]);
	DDX_Control(pDX, IDC_Sig9, mSig[8]);
	DDX_Control(pDX, IDC_SignalDataList, mSignalDataList);
	DDX_Control(pDX, IDC_Hex, mHex);
	DDX_Control(pDX, IDC_EDIT_MOD, mMod);
	DDX_Control(pDX, IDC_LogSave, mLogSave);
	DDX_Control(pDX, IDC_LogViewer, mLogViewer);
}

BEGIN_MESSAGE_MAP(CLINProjectDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_Start, &CLINProjectDlg::OnBnClickedStart)
	ON_BN_CLICKED(IDC_Pause, &CLINProjectDlg::OnBnClickedPause)
	ON_BN_CLICKED(IDC_Stop, &CLINProjectDlg::OnBnClickedStop)
	ON_BN_CLICKED(IDC_SEND, &CLINProjectDlg::OnBnClickedSend)
	ON_BN_CLICKED(IDC_OpenLog, &CLINProjectDlg::OnBnClickedOpenlog)
	ON_CBN_SELCHANGE(IDC_Schedule, &CLINProjectDlg::OnCbnSelchangeSchedule)
	ON_CBN_SELCHANGE(IDC_FrameId, &CLINProjectDlg::OnCbnSelchangeFrameid)
	ON_WM_MEASUREITEM()
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_SignalList, &CLINProjectDlg::OnLvnItemchangedSignallist)
	ON_BN_CLICKED(IDC_Connect, &CLINProjectDlg::OnBnClickedConnect)
	ON_BN_CLICKED(IDC_Disconnect, &CLINProjectDlg::OnBnClickedDisconnect)
	ON_WM_CLOSE()
	ON_CBN_SELCHANGE(IDC_FrameName, &CLINProjectDlg::OnCbnSelchangeFramename)
	ON_BN_CLICKED(IDC_Hex, &CLINProjectDlg::OnBnClickedHex)
	ON_BN_CLICKED(IDC_Apply, &CLINProjectDlg::OnBnClickedApply)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_SignalDataList, &CLINProjectDlg::OnLvnItemchangedSignaldatalist)
	ON_EN_CHANGE(IDC_Tx0, &CLINProjectDlg::OnEnChangeTx)
	ON_EN_CHANGE(IDC_Tx1, &CLINProjectDlg::OnEnChangeTx)
	ON_EN_CHANGE(IDC_Tx2, &CLINProjectDlg::OnEnChangeTx)
	ON_EN_CHANGE(IDC_Tx3, &CLINProjectDlg::OnEnChangeTx)
	ON_EN_CHANGE(IDC_Tx4, &CLINProjectDlg::OnEnChangeTx)
	ON_EN_CHANGE(IDC_Tx5, &CLINProjectDlg::OnEnChangeTx)
	ON_EN_CHANGE(IDC_Tx6, &CLINProjectDlg::OnEnChangeTx)
	ON_EN_CHANGE(IDC_Tx7, &CLINProjectDlg::OnEnChangeTx)
	ON_BN_CLICKED(IDC_LogSave, &CLINProjectDlg::OnBnClickedLogsave)
	ON_BN_CLICKED(IDC_LogViewer, &CLINProjectDlg::OnBnClickedLogviewer)
END_MESSAGE_MAP()


// CLINProjectDlg 메시지 처리기

BOOL CLINProjectDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// 프로그램명 설정
	SetWindowText(L"LIN Simulation - v."+version);

	/*
	ListCtrl 초기화

	mTraceList의 스타일 지정 (그리드 라인, 선택 시 행 모두 선택, 더블 버퍼 사용(계속 값이 변경되기 때문에))
	각 행 별 값과 너비 지정 (Frame ID, Frame Name, Frame Data, Error Flags)

	mSignalList의 스타일 지정 (체크 박스, 그리드 라인, 선택 시 행 모두 선택)
	각 행 별 값과 너비 지정 (Signal Name, Frame ID)

	mSignalDataList의 스타일 지정 (체크 박스, 그리드 라인, 더블 버퍼 사용(계속 값이 변경되기 때문에))
	각 행 별 값과 너비 지정 (Signal Name, Frame ID)
	*/
	CRect rtTrace;
	mTraceList.GetWindowRect(&rtTrace);
	mTraceList.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT | LVS_EX_DOUBLEBUFFER);

	mTraceList.InsertColumn(0, TEXT("Frame ID"), LVCFMT_LEFT, rtTrace.Width() * 0.15);
	mTraceList.InsertColumn(1, TEXT("Frame Name"), LVCFMT_LEFT, rtTrace.Width() * 0.25);
	mTraceList.InsertColumn(2, TEXT("Frame Data"), LVCFMT_LEFT, rtTrace.Width() * 0.45);
	mTraceList.InsertColumn(3, TEXT("Error Flags"), LVCFMT_LEFT, rtTrace.Width() * 0.15);

	CRect rtSignal;
	mSignalList.GetWindowRect(&rtSignal);
	mSignalList.SetExtendedStyle(LVS_EX_CHECKBOXES | LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);

	mSignalList.InsertColumn(0, TEXT("Signal Name"), LVCFMT_LEFT, rtSignal.Width() * 0.65);
	mSignalList.InsertColumn(1, TEXT("Frame ID"), LVCFMT_LEFT, rtSignal.Width() * 0.35);

	CRect rtSignalData;
	mSignalDataList.GetWindowRect(&rtSignalData);
	mSignalDataList.SetExtendedStyle(LVS_EX_CHECKBOXES | LVS_EX_GRIDLINES | LVS_EX_DOUBLEBUFFER );

	mSignalDataList.InsertColumn(0, TEXT("Signal Name"), LVCFMT_LEFT, rtTrace.Width() * 0.6);
	//mSignalDataList.InsertColumn(1, TEXT("Frame Name"), LVCFMT_LEFT, rtTrace.Width() * 0.35);
	mSignalDataList.InsertColumn(2, TEXT("Data"), LVCFMT_LEFT, rtTrace.Width() * 0.4);

	// 그래프 초기화
	for (int i = 0; i < 9; i++) {
		initGraph(i);
	}

	// 송신 데이터 글자수 제한
	for (int i = 0; i < 8; i++) {
		mTx[i]->SetLimitText(3);
	}


	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CLINProjectDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}
void CLINProjectDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

HCURSOR CLINProjectDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CLINProjectDlg::initPharam() {
	// 초기화
	w_Signals.clear();
	w_DiagnosticSignals.clear();
	w_Frames.clear();
	w_DiagnosticFrames.clear();
	w_NodeAttributes.clear();
	w_ScheduleTables.clear();
	w_SignalEncodings.clear();
	Frames.clear();
	FrameNames.clear();
	FrameIDs.clear();
	schedulesSize = 0;
	schedule_position = 0;
	graphSig.clear();
	signalEncodings.clear();

	// 위젯 초기화
	mSchedule.ResetContent();
	mFrameId.ResetContent();
	mFrameName.ResetContent();
	mTraceList.DeleteAllItems();
	mTraceList.DeleteAllItems();
	mSignalList.DeleteAllItems();
	mSignalDataList.DeleteAllItems();

	// 그래프 초기화
	for (int i = 0; i < 9; i++) {
		mSig[i].SetWindowTextW(_T(""));
		pSeries[i]->RemovePointsFromBegin(pSeries[i]->GetPointsCount());
	}
	graphDatas.clear();

	// 그래프 변수 초기화
	signalEncodings = {};
	time = 0;
	t = 0;

}

// LIN 스케줄 시작
int CLINProjectDlg::wLIN_start() {
	// 버스 깨우기
	result = LIN_XmtWakeUp(hClient, hHw);
	onPause = false;

	// 정지 상태일 때 (재시작)
	if (onPause) {
		result = LIN_ResumeSchedule(hClient, hHw);
	}
	// 정지 상태가 아닐 때 (시작)
	else {
		result = LIN_StartSchedule(hClient, hHw, mSchedule.GetCurSel());
	}
	
	// 시작/재시작 성공 시 읽기 시작
	if (result == errOK && !m_bThreadRunning) {
		m_bThreadRunning = true;
		m_pThread1 = AfxBeginThread(wReadDataThread, this);
		m_pThread2 = AfxBeginThread(wTimerThread, this);
	}	

	return 0;
}

// 정지
int CLINProjectDlg::wLIN_pause() {
	m_bThreadRunning = false;
	// 스케줄 정지
	result = LIN_SuspendSchedule(hClient, hHw);

	onPause = true;
	return 0;
}

int CLINProjectDlg::wLIN_connect() {
	// 그래프 초기화
	for (int i = 0; i < 9; i++) {
		mSig[i].SetWindowTextW(_T(""));
		pSeries[i]->RemovePointsFromBegin(pSeries[i]->GetPointsCount());
	}
	graphDatas.clear();

	// 그래프 변수 초기화
	signalEncodings = {};
	time = 0;
	t = 0;

	Frames.clear();
	FrameNames.clear();
	FrameIDs.clear();
	schedulesSize = 0;
	schedule_position = 0;
	graphSig.clear();
	signalEncodings.clear();

	// 위젯 초기화
	mSchedule.ResetContent();
	mFrameId.ResetContent();
	mFrameName.ResetContent();
	mTraceList.DeleteAllItems();
	mSignalList.DeleteAllItems();
	mSignalDataList.DeleteAllItems();

	//onPause = true;
	//onClear = true;

	result = LIN_RegisterClient(const_cast<char*>(w_Client_name.c_str()), 0, &hClient);


	// 하드웨어 등록
	for (HLINHW hw : HW_TYPES) {
		hHw = hw;
		result = LIN_ConnectClient(hClient, hHw);
		if (result == errOK) {
			break;
		}
	}

	if (result != errOK) {
		hHw = 0;
	}

	// 하드웨어 초기화 (마스터 모드, 19200 bit rate)
	result = LIN_InitializeHardware(hClient, hHw, modMaster, w_LIN_speed * 1000);


	if (result != errOK) {
		LIN_DisconnectClient(hClient, hHw);
		LIN_RemoveClient(hClient);
	}
	// 프레임 id 초기화
	for (BYTE frameId : FRAME_IDS) {
		result = LIN_RegisterFrameId(hClient, hHw, frameId, frameId);
	}

	// 프레임
	for (w_Frame frame : w_Frames) {
		TLINFrameEntry f = {};
		f.FrameId = frame.id;
		f.ChecksumType = cstEnhanced;
		f.Length = frame.length;

		/*CString temp(frame.txNode.c_str());
		MessageBox(_T("/") + temp + "/");*/

		if (frame.txNode == w_Client_name) {

			f.Flags = FRAME_FLAG_RESPONSE_ENABLE;
			f.Direction = dirPublisher;
			memset(f.InitialData, 0, sizeof(data));
		}
		else {
			f.Direction = dirSubscriber;
			memset(f.InitialData, 0, sizeof(data));
		}

		result = LIN_SetFrameEntry(hClient, hHw, &f);

		Frames.push_back(f);
		FrameNames.push_back(frame.name);
		FrameIDs.push_back(frame.id);
	}

	// 스케줄
	for (w_ScheduleTable scheduleTable : w_ScheduleTables) {
		w_Schedules s;
		int i = 0;
		s.schedulesPosition = schedulesSize;
		for (w_Schedule schedule : scheduleTable.schedule) {
			int id = 0;

			id = find(FrameNames.begin(), FrameNames.end(), schedule.name) - FrameNames.begin();
			s.Schedule[i].Type = sltUnconditional;
			s.Schedule[i].FrameId[0] = FrameIDs[id];
			s.Schedule[i].Delay = schedule.delay;
			i++;
		}
		s.size = i;


		result = LIN_SetSchedule(hClient, hHw, schedulesSize, s.Schedule, s.size);

		Schdules[schedulesSize] = s;
		schedulesSize++;

		CString temp(scheduleTable.name.c_str());
		mSchedule.AddString(temp);
	}

	// 신호
	int sigItem = 0;
	for (w_Frame f : w_Frames) {
		vector<signalStartEnd> signalEncoding;
		for (w_DataStruct d : f.w_Data) {
			CString temp(d.name.c_str());
			mSignalList.InsertItem(sigItem, temp);

			signalStartEnd sig;
			sig.sigIndex = sigItem++;
			sig.name = d.name;
			sig.start = d.start;

			int sigLength = 1;

			for (w_Signal wSig : w_Signals) {
				if (wSig.name == sig.name) {
					sigLength = wSig.dataLength;
					break;
				}
			}

			sig.end = sig.start + sigLength;
			signalEncoding.push_back(sig);
		}
		signalEncodings[f.id] = signalEncoding;
	}
	return 0;
}

// 초기화
int CLINProjectDlg::wLIN_clear() {
	m_bThreadRunning = false;
	// 스케줄 삭제, 하드웨어 연결 해제
	result = LIN_DeleteSchedule(hClient, hHw, schedule_position);
	result = LIN_DisconnectClient(hClient, hHw);
	
	onPause = true;
	m_bThreadRunning = false;

	return 0;
}

UINT CLINProjectDlg::wTimerThread(LPVOID pParam) {
	CLINProjectDlg* pDlg = reinterpret_cast<CLINProjectDlg*>(pParam);
	if (pDlg) {
		pDlg->wTimer();  // 실제 데이터 읽기 함수 실행
	}
	return 0;
}

void CLINProjectDlg::wTimer() {
	int index;

	while (m_bThreadRunning) {
		time += 0.01;
		Sleep(10);

		if (t == time || t < time) {
			for (index = 0; index < 9; index++) {
				if (t < 20) 
					mGraph[index].GetBottomAxis()->SetMinMax(0, 25);
				else
					mGraph[index].GetBottomAxis()->SetMinMax(t - 20, t + 5);
			}
			t += 0.1;
		}
	}
}

UINT CLINProjectDlg::wReadDataThread(LPVOID pParam) {
	CLINProjectDlg* pDlg = reinterpret_cast<CLINProjectDlg*>(pParam);
	if (pDlg) {
		pDlg->wReadData();  // 실제 데이터 읽기 함수 실행
	}
	return 0;
}

void CLINProjectDlg::wReadData() {
	// 필터 설정
	unsigned __int64 Filter = 0xFFFFFFFFFFFFFFFF;
	LIN_SetClientFilter(hClient, hHw, Filter);

	while (m_bThreadRunning) {
		// 읽기 정지 (정지, 연결 해제)
		if (onPause) {
			break;
		}
		for (int read = 0; read < w_Frames.size(); read++) {

			// 데이터 읽기
			result = LIN_Read(hClient, &rcvMsg);
			if (result == errOK) {
				double sigTime = time;
				// Trace 값
				int nItemNum = find(FrameIDs.begin(), FrameIDs.end(), (rcvMsg.FrameId & 0x3F)) - FrameIDs.begin();

				CString frameID;
				CString data;
				CString flag;

				frameID.Format(_T("0x%X"), (rcvMsg.FrameId & 0x3F));
				data.Format(_T("%X %X %X %X %X %X %X %X"), rcvMsg.Data[0], rcvMsg.Data[1], rcvMsg.Data[2], rcvMsg.Data[3],
					rcvMsg.Data[4], rcvMsg.Data[5], rcvMsg.Data[6], rcvMsg.Data[7]);
				flag.Format(_T("%X"), rcvMsg.ErrorFlags);

				//mTraceList.SetItemText(nItemNum, 0, frameID);
				mTraceList.SetItemText(nItemNum, 2, data);
				mTraceList.SetItemText(nItemNum, 3, flag);

				ULONG64 Data = 0;
				for (int j = 0; j < 8; j++) {
					Data <<= 8;
					Data += rcvMsg.Data[j];
				}

				for (int m = 0; m < gSettings.size(); m++) {
					if (gSettings[m].id == (rcvMsg.FrameId & 0x3F)) {
						wGraphDraw(Data, sigTime, m);
					}
				}

				// 로그 저장
				if (mLogSave.GetCheck() && logFileName_w != "") {
					log_file_w.open(logFileName_w,ios_base::out | ios_base::app);
					log_file_w << sigTime << "," << (rcvMsg.FrameId & 0x3F) << "," << Data << endl;
					log_file_w.close();
				}
			}
		}
	}
	return;
}

void CLINProjectDlg::wGraphDraw(ULONG64 logData, double logTime, int index) {
	graphSetting g = gSettings[index];
	int length = g.end - g.start;
	double value;
	ULONG64 mask = (1ULL << length) - 1;

	if (g.id % 2 == 0) {
		mask <<= 63 - g.end - 6 + 1;
		value = (logData & mask) >> (63 - g.end - 6 + 1);
	}
	else {
		mask <<= 63 - g.end + 1;
		value = (logData & mask) >> (63 - g.end + 1);
	}

	pSeries[index]->AddPoint(logTime, value);
}


void CLINProjectDlg::OnBnClickedStart()
{
	if (mSchedule.GetCurSel() == -1) {
		MessageBox(_T("Select Schedule!"));
	}
	else if (!m_bThreadRunning) {
		wLIN_start();
	}
}

void CLINProjectDlg::OnBnClickedPause()
{
	wLIN_pause();
}

void CLINProjectDlg::OnBnClickedStop()
{
	wLIN_clear();
}

void CLINProjectDlg::OnBnClickedOpenlog()
{
	initPharam();

	static TCHAR BASED_CODE szFilter[] = _T("데이터베이스 파일 (*.ldf)|*.ldf; ||");
	CFileDialog dlg(TRUE, _T("*.ldf"), _T(""), OFN_HIDEREADONLY, szFilter);

	if (IDOK == dlg.DoModal()) {
		CString path = dlg.GetPathName();
		CString fileName = dlg.GetFileName();
		CString extend = dlg.GetFileExt();

		string temp = string(CT2CA(path));
		openFileName = string(CT2CA(fileName));
		openFileExt = string(CT2CA(extend));
		

		if (extend == _T("ldf")) {
			w_LDF_parse(temp); // LIN 설정 파일 파싱
		}
	}
}


void CLINProjectDlg::OnBnClickedLogviewer()
{
	static TCHAR BASED_CODE szFilter[] = _T("로그 파일 (*.csv)|*.csv; ||");
	CFileDialog dlg(TRUE, _T("*.csv"), _T(""), OFN_HIDEREADONLY, szFilter);

	if (IDOK == dlg.DoModal()) {
		CString path = dlg.GetPathName();
		CString fileName = dlg.GetFileName();
		CString extend = dlg.GetFileExt();

		string temp = string(CT2CA(path));

		if (extend == _T("csv")) {
			log_file_r.open(temp);// 저장한 로그 파일 열기
			if (!log_file_r.is_open()) {
				MessageBox(L"로그 파일 불러오기 실패");
				return;
			}
			else {
				logFileName_r = temp;

				string line, log;
				
				double log_time;
				BYTE log_id;
				ULONG64 log_data;

				while (getline(log_file_r, line)) {
					stringstream ss(line);
					getline(ss, log, ',');
					log_time = stod(log);
					getline(ss, log, ',');
					log_id = stoi(log);
					getline(ss, log, ',');
					log_data = stoull(log);

					logDatas[log_id].push_back(logData{ log_time, log_data });
				}
				MessageBox(fileName);
			}
			log_file_r.close();
		}
	}
}

void CLINProjectDlg::OnCbnSelchangeSchedule()
{
	int selNum = mSchedule.GetCurSel();
	w_Schedules s = Schdules[selNum];
	mFrameId.ResetContent();
	mFrameName.ResetContent();

	for (int i = 0; i < s.size; i++) {
		CString frameID;
		CString frameNAME(FrameNames[find(FrameIDs.begin(), FrameIDs.end(), (s.Schedule[i].FrameId[0])) - FrameIDs.begin()].c_str());
		frameID.Format(_T("0x%X"), s.Schedule[i].FrameId[0]);
		mFrameId.AddString(frameID);
		mTraceList.InsertItem(i, frameID);
		
		mFrameName.AddString(frameNAME);
		mTraceList.SetItemText(i,1,frameNAME);
	}
}

void CLINProjectDlg::OnCbnSelchangeFrameid()
{
	int selNum = mFrameId.GetCurSel();
	w_Frame f = w_Frames[selNum];
	//CString fName(f.name.c_str());

	mFrameName.SetCurSel(selNum);

	frameId_global = f.id;
	frameLength_global = f.length;

	mSignalDataList.DeleteAllItems();
	int i = 0;
	for (w_DataStruct sig : f.w_Data) {
		CString s(sig.name.c_str());
		mSignalDataList.InsertItem(i, s);
		//mSignalDataList.SetItemText(i, 1, fName);
		mSignalDataList.SetItemText(i, 2, _T("1"));
		i++;
	}

	for (i = 0; i < 8; i++) {
		mTx[i]->SetWindowTextW(L"");
		sendData[i] = 0;
	}
}

void CLINProjectDlg::OnCbnSelchangeFramename()
{
	int selNum = mFrameName.GetCurSel();
	w_Frame f = w_Frames[selNum];
	//CString fName(f.name.c_str());

	mFrameId.SetCurSel(selNum);

	frameId_global = f.id;
	frameLength_global = f.length;

	mSignalDataList.DeleteAllItems();
	int i = 0;
	for (w_DataStruct sig : f.w_Data) {
		CString s(sig.name.c_str());
		mSignalDataList.InsertItem(i, s);
		//mSignalDataList.SetItemText(i, 1, fName);
		mSignalDataList.SetItemText(i, 2, _T("2"));
		i++;
	}

	for (i = 0; i < 8; i++) {
		mTx[i]->SetWindowTextW(L"");
		sendData[i] = 0;
	}
}

void CLINProjectDlg::OnBnClickedSend()
{
	int high, low, i;
	CString temp, delay;

	mDelay.GetWindowTextW(delay);
	if (mFrameId.GetCurSel() == -1) {
		MessageBox(_T("Select Frame ID!"));
	}
	else if (!m_bThreadRunning) {
		MessageBox(_T("Start Schedule!"));
	}
	else {
		if (delay != _T("")) {
			int d = _ttoi(delay);
			Sleep(d);
		}

		if (mHex.GetCheck()) {
			for (i = 0; i < 8; i++) {
				temp = "";
				mTx[i]->GetWindowTextW(temp);
				if (temp.GetLength() >= 2) {
					high = (temp[0] > '9') ? temp[0] - 'A' + 10 : temp[0] - '0';
					low = (temp[1] > '9') ? temp[1] - 'A' + 10 : temp[1] - '0';

					sendData[i] = (high << 4) | low;

					LIN_UpdateByteArray(hClient, hHw, frameId_global, i, 1, &sendData[i]);
				}
				else if (temp.GetLength() == 1) {
					high = 0;
					low = (temp[0] > '9') ? temp[0] - 'A' + 10 : temp[0] - '0';

					sendData[i] = (high << 4) | low;

					LIN_UpdateByteArray(hClient, hHw, frameId_global, i, 1, &sendData[i]);
				}
				else {
					sendData[i] = NULL;
				}
			}
		}
		else {
			for (i = 0; i < 8; i++) {
				temp = "";
				mTx[i]->GetWindowTextW(temp);
				if (temp.GetLength() > 0) {
					sendData[i] = _ttoi(temp);

					LIN_UpdateByteArray(hClient, hHw, frameId_global, i, 1, &sendData[i]);
				}
			}
		}
	}
}

void CLINProjectDlg::OnLvnItemchangedSignallist(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	if (pNMLV->uNewState & LVIS_STATEIMAGEMASK) // 체크박스 변경 감지
	{
		int index = pNMLV->iItem;
		auto i = find(graphSig.begin(), graphSig.end(), index);

		if (mSignalList.GetCheck(index) && graphSig.size() == 9) {
			mSignalList.SetCheck(index, false);
			MessageBox(_T("Full Graph Size."));
			return;
		}
		else if (!mSignalList.GetCheck(index) && graphSig.size() > 0 && i != graphSig.end()) {
			int m = find(graphSig.begin(), graphSig.end(), index) - graphSig.begin();

			graphSig.erase(remove(graphSig.begin(), graphSig.end(), index), graphSig.end());

			graphDatas.erase(graphDatas.begin() + m);
			gSettings.erase(gSettings.begin() + m);
			for (int j = m; j < graphSig.size()+1; j++) {
				int pos = pSeries[j]->GetPointsCount();
				pSeries[j]->RemovePointsFromBegin(pos);
				if (j >= graphSig.size()) {
					mSig[j].SetWindowTextW(_T(""));
					continue;
				}

				if (j < graphSig.size()) {
					int pointCnt = pSeries[j + 1]->GetPointsCount();
					for (int k = 0; k < pointCnt; k++) {
						pSeries[j]->AddPoint(pSeries[j + 1]->GetXPointValue(k), pSeries[j + 1]->GetYPointValue(k));
					}
				}

				CString signal(mSignalList.GetItemText(graphSig[j], 0));
				mSig[j].SetWindowTextW(signal);
			}
		}
		else if (mSignalList.GetCheck(index)&& graphSig.size() < 9) {
			graphSig.push_back(index);
			int m = graphSig.size() - 1;

			CString signal(mSignalList.GetItemText(index, 0));
			mSig[m].SetWindowTextW(signal);

			graphData a;
			a.name = string(CT2CA(mSignalList.GetItemText(index, 0)));
			a.time = 0;
			a.value = 0;

			graphSetting g;

			for (int id : FrameIDs) {
				for (signalStartEnd sss : signalEncodings[id]) {
					if (a.name != sss.name) continue;
					else {
						g.id = id;
						g.start = sss.start;
						g.end = sss.end;
						break;
					}
				}
			}
			gSettings.push_back(g);
			graphDatas.push_back(a);

			if (logFileName_r != "") {
				for (logData log : logDatas[g.id]) {
					int length = g.end - g.start;
					double value;
					ULONG64 mask = (1ULL << length) - 1;

					if (g.id % 2 == 0) {
						mask <<= 63 - g.end - 6 + 1;
						value = (log.data & mask) >> (63 - g.end - 6 + 1);
					}
					else {
						mask <<= 63 - g.end + 1;
						value = (log.data & mask) >> (63 - g.end + 1);
					}

					pSeries[m]->AddPoint(log.time, value);
				}
			}
		}
	}
	*pResult = 0;
}

void CLINProjectDlg::initGraph(int index) {
	CChartStandardAxis* pBottomAxis =
		mGraph[index].CreateStandardAxis(CChartCtrl::BottomAxis);
	CChartStandardAxis* pLeftAxis =
		mGraph[index].CreateStandardAxis(CChartCtrl::LeftAxis);
	pLeftAxis->SetAutomaticMode(CChartAxis::FullAutomatic);
	pBottomAxis->SetAutomaticMode(CChartAxis::NotAutomatic);
	pBottomAxis->SetMinMax(0, 25);

	pBottomAxis->SetDiscrete(false);
	//mGraph[index].ShowMouseCursor(true);
	//CChartCrossHairCursor* pCrossHair = mGraph[index].CreateCrossHairCursor();
	pLeftAxis->SetMarginSize(false, 80);
	//pBottomAxis->EnableScrollBar(true);

	/// 라인차트 파트
	pSeries[index] = mGraph[index].CreateLineSerie();
	
	pSeries[index]->SetColor(RGB(255, 0, 0));
}

BOOL CLINProjectDlg::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_RETURN) // ENTER키 눌릴 시
			return TRUE;
		else if (pMsg->wParam == VK_ESCAPE) // ESC키 눌릴 시
			return TRUE;
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}

void CLINProjectDlg::OnBnClickedConnect()
{
	wLIN_connect();
}

void CLINProjectDlg::OnBnClickedDisconnect()
{
	wLIN_clear();
}

void CLINProjectDlg::OnClose()
{
	wLIN_clear();

	if (log_file_w.is_open()) {
		log_file_w.close();
	}

	CDialogEx::OnClose();
}


void CLINProjectDlg::OnBnClickedHex()
{
	int a = 0, high, low;
	CString temp;

	if (mHex.GetCheck()) {
		for (int i = 0; i < 8; i++) {
			mTx[i]->GetWindowTextW(temp);
			a = _ttoi(temp);

			temp.Format(_T("%X"), a);
			mTx[i]->SetLimitText(2);
			mTx[i]->SetWindowTextW(temp);
		}
	}
	else {
		for (int i = 0; i < 8; i++) {
			mTx[i]->GetWindowTextW(temp);
			if (temp.GetLength() >= 2) {
				high = (temp[0] > '9') ? temp[0] - 'A' + 10 : temp[0] - '0';
				low = (temp[1] > '9') ? temp[1] - 'A' + 10 : temp[1] - '0';

				a = (high << 4) | low;
			}
			else if (temp.GetLength() == 1) {
				high = 0;
				low = (temp[0] > '9') ? temp[0] - 'A' + 10 : temp[0] - '0';

				a = (high << 4) | low;
			}

			temp.Format(_T("%d"), a);
			mTx[i]->SetLimitText(3);
			mTx[i]->SetWindowTextW(temp);
		}
	}
}

void CLINProjectDlg::OnLvnItemchangedSignaldatalist(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	if (pNMLV->uNewState & LVIS_STATEIMAGEMASK) // 체크박스 변경 감지
	{
		int index = pNMLV->iItem;
		if (mSignalDataList.GetCheck(index)) {
			for (int i = 0; i < mSignalDataList.GetItemCount(); i++) {
				if (mSignalDataList.GetCheck(i) && i != index) {
					mSignalDataList.SetCheck(i, false);
				}
			}
		}
	}
	*pResult = 0;
}

void CLINProjectDlg::OnEnChangeTx()
{
	CString temp;
	int high, low, i;

	if (mHex.GetCheck()) {
		for (i = 0; i < 8; i++) {
			temp = "";
			mTx[i]->GetWindowTextW(temp);
			if (temp.GetLength() >= 2) {
				high = (temp[0] > '9') ? temp[0] - 'A' + 10 : temp[0] - '0';
				low = (temp[1] > '9') ? temp[1] - 'A' + 10 : temp[1] - '0';

				sendData[i] = (high << 4) | low;

				LIN_UpdateByteArray(hClient, hHw, frameId_global, i, 1, &sendData[i]);
			}
			else if (temp.GetLength() == 1) {
				high = 0;
				low = (temp[0] > '9') ? temp[0] - 'A' + 10 : temp[0] - '0';

				sendData[i] = (high << 4) | low;
			}
		}
	}
	else {
		for (i = 0; i < 8; i++) {
			temp = "";
			mTx[i]->GetWindowTextW(temp);
			if (temp.GetLength() > 0) {
				sendData[i] = _ttoi(temp);
			}
		}
	}
	for (int j = 0; j < 8; j++) {
		sData <<= 8;
		sData += sendData[j];
	}

	//temp.Format(_T("%d %d"), sData >> 32, sData);
	//MessageBox(temp);
}

void CLINProjectDlg::OnBnClickedApply()
{
	CString temp;
	mMod.GetWindowTextW(temp);

	for (int i = 0; i < mSignalDataList.GetItemCount(); i++) {
		if (mSignalDataList.GetCheck(i)) {
			mSignalDataList.SetItemText(i, 1, temp);

			ULONG64 d = _ttoi(temp);
			signalStartEnd sig = signalEncodings[frameId_global][i];
			int length = sig.end - sig.start;
			ULONG64 mask;

			if (frameId_global % 2 == 0) {
				mask = (((1ULL << length) - 1) << (63 - sig.end - 1));
				sData = (sData & mask);
				sData += d << (63 - sig.end - 5); // 5 = 6 - 1
			}
			else {
				mask = ~(((1ULL << length) - 1) << (63 - sig.end + 1));
				sData = (sData & mask);
				sData += d << (63 - sig.end + 1);
			}
			////
			/*temp.Format(_T("%X %X"), mask >> 32, (mask << 32) >> 32);
			MessageBox(temp);
			temp.Format(_T("%X %X"), sData >> 32, sData);
			MessageBox(temp);*/
			//sData = 4294967296;

			ULONG64 t_sData = sData;
			for (int j = 0; j < 8; j++) {
				ULONG64 a = 0;
				a = t_sData >> 56;
				t_sData <<= 8;


				if (mHex.GetCheck()) {
					temp.Format(_T("%X"), (int)a);
				}
				else {
					temp.Format(_T("%d"), (int)a);
				}
				mTx[j]->SetWindowTextW(temp);
				sendData[j] = a;
			}
			////
			/*temp.Format(_T("%X %X %X %X %X %X %X %X"),
				sendData[0], sendData[1], sendData[2], sendData[3],
				sendData[4], sendData[5], sendData[6], sendData[7]);
			MessageBox(temp);*/
		}
	}
}

void CLINProjectDlg::OnBnClickedLogsave()
{
	if (!mLogSave.GetCheck()) {
		logFileName_w = "";
		if (log_file_w.is_open()) {
			log_file_w.close();
		}
	}
	else if (mLogSave.GetCheck()) {
		time_t now = std::time(nullptr);
		tm tm1;

		localtime_s(&tm1, &now);

		logFileName_w = "log" + to_string(tm1.tm_year + 1900) + "_" + to_string(tm1.tm_mon + 1) + "_"
			+ to_string(tm1.tm_mday) + "_" + to_string(tm1.tm_hour) + "_" + to_string(tm1.tm_min) + "_"
			+ to_string(tm1.tm_sec) + ".csv";

		log_file_w.open(logFileName_w);
	}
}

int CLINProjectDlg::w_LDF_parse(string filePath) {
	ifstream file(filePath);
	if (!file.is_open()) {
		return -1;
	}

	string line;
	string section = "";
	while (getline(file, line)) {
		//
		if (line.find("Nodes {") != string::npos) {
			section = "Nodes";
		}
		else if (line.find("Signals {") != string::npos) {
			section = "Signals";
		}
		else if (line.find("Diagnostic_signals {") != string::npos) {
			section = "Diagnostic_signals";
		}
		else if (line.find("Frames {") != string::npos) {
			section = "Frames";
		}
		else if (line.find("Diagnostic_frames {") != string::npos) {
			section = "Diagnostic_frames";
		}
		else if (line.find("Node_attributes {") != string::npos) {
			section = "Node_attributes";
		}
		else if (line.find("Schedule_tables {") != string::npos) {
			section = "Schedule_tables";
		}
		else if (line.find("Signal_encoding_types {") != string::npos) {
			section = "Signal_encoding_types";
		}
		// Config
		else if (line.find("LIN_protocol_version = ") != string::npos) {
			section = "";
			w_Parser_Config(line);
		}
		else if (line.find("LIN_language_version = ") != string::npos) {
			section = "";
			w_Parser_Config(line);
		}
		else if (line.find("LIN_speed = ") != string::npos) {
			section = "";
			w_Parser_Config(line);
		}

		//
		else if (line.find("}") != string::npos) {
			section = "";
		}

		//
		else if (section == "Nodes") {
			w_Parser_Nodes(line);
		}
		else if (section == "Signals") {
			w_Parser_Signals(line);
		}
		else if (section == "Diagnostic_signals") {
			w_Parser_DiagnosticSignals(line);
		}
		else if (section == "Frames") {
			stringstream ss(line);
			w_Frame frame;
			string value;

			getline(ss, value, ':');
			value.erase(remove(value.begin(), value.end(), ' '), value.end());
			frame.name = value;

			// 데이터 추출
			string data;
			getline(ss, data, ';');
			stringstream dataStream(data);

			getline(dataStream, value, ',');
			frame.id = stoi(value);
			getline(dataStream, value, ',');
			value.erase(remove(value.begin(), value.end(), ' '), value.end());
			frame.txNode = value;
			getline(dataStream, value, ',');
			frame.length = stoi(value);


			while (true) {
				getline(file, line);
				if (line.find("}") != string::npos) {
					w_Frames.push_back(frame);
					break;
				}
				w_DataStruct dataStruct;
				stringstream ss(line);
				string data;
				getline(ss, data, ';');

				stringstream dataStream(data);
				string value;
				getline(dataStream, value, ',');
				value.erase(remove(value.begin(), value.end(), ' '), value.end());
				dataStruct.name = value;
				getline(dataStream, value, ',');
				dataStruct.start = stoi(value);

				frame.w_Data.push_back(dataStruct);
			}
		}
		else if (section == "Diagnostic_frames") {
			stringstream ss(line);
			w_DiagnosticFrame frame;
			getline(ss, frame.name, ':');

			// 데이터 추출
			string data;
			getline(ss, data, ';');
			stringstream dataStream(data);
			string value;

			getline(dataStream, value, ',');
			frame.id = stoi(value);

			while (true) {
				getline(file, line);
				if (line.find("}") != string::npos) {
					w_DiagnosticFrames.push_back(frame);
					break;
				}
				w_DataStruct dataStruct;
				stringstream ss(line);
				string data;
				getline(ss, data, ';');

				stringstream dataStream(data);
				string value;
				getline(dataStream, value, ',');
				value.erase(remove(value.begin(), value.end(), ' '), value.end());
				dataStruct.name = value;
				getline(dataStream, value, ',');
				dataStruct.start = stoi(value);
				frame.w_Data.push_back(dataStruct);
			}
		}
		else if (section == "Node_attributes") {
			stringstream ss(line);
			w_NodeAttribute attribute;
			getline(ss, attribute.name, '{');

			// 데이터 추출
			string data;
			getline(ss, data, ';');
			stringstream dataStream(data);
			string value;

			while (true) {
				getline(file, line);
				if (line.find("}") != string::npos) {
					w_NodeAttributes.push_back(attribute);
					break;
				}
				else if (line.find("LIN_protocol =") != string::npos) {
					string key, value;
					size_t pos = line.find('=');

					key = line.substr(0, pos);
					value = line.substr(pos + 1);

					// 공백 및 특수문자 제거
					value.erase(remove(value.begin(), value.end(), ' '), value.end());
					value.erase(remove(value.begin(), value.end(), '\"'), value.end());
					value.erase(remove(value.begin(), value.end(), ';'), value.end());

					attribute.linProtocol = value;

				}
				else if (line.find("configured_NAD =") != string::npos) {
					string key, value;
					size_t pos = line.find('=');

					key = line.substr(0, pos);
					value = line.substr(pos + 1);

					// 공백 및 특수문자 제거
					value.erase(remove(value.begin(), value.end(), ' '), value.end());
					value.erase(remove(value.begin(), value.end(), '\"'), value.end());
					value.erase(remove(value.begin(), value.end(), ';'), value.end());

					attribute.configuredNAD = stof(value);
				}
				else if (line.find("initial_NAD =") != string::npos) {
					string key, value;
					size_t pos = line.find('=');

					key = line.substr(0, pos);
					value = line.substr(pos + 1);

					// 공백 및 특수문자 제거
					value.erase(remove(value.begin(), value.end(), ' '), value.end());
					value.erase(remove(value.begin(), value.end(), '\"'), value.end());
					value.erase(remove(value.begin(), value.end(), ';'), value.end());

					attribute.initialNAD = stof(value);
				}
				else if (line.find("product_id =") != string::npos) {
					string key, value;
					size_t pos = line.find('=');

					key = line.substr(0, pos);
					value = line.substr(pos + 1);

					// 공백 및 특수문자 제거
					value.erase(remove(value.begin(), value.end(), ' '), value.end());
					value.erase(remove(value.begin(), value.end(), '\"'), value.end());
					value.erase(remove(value.begin(), value.end(), ';'), value.end());

					stringstream ss(value);
					string part;
					vector<string> parts;
					while (getline(ss, part, ',')) parts.push_back(part);
					attribute.productID1 = stoi(parts[0], nullptr, 16);
					attribute.productID2 = stoi(parts[1], nullptr, 16);
					attribute.productVersion = stoi(parts[2]);
				}
				else if (line.find("response_error =") != string::npos) {
					string key, value;
					size_t pos = line.find('=');

					key = line.substr(0, pos);
					value = line.substr(pos + 1);

					// 공백 및 특수문자 제거
					value.erase(remove(value.begin(), value.end(), ' '), value.end());
					value.erase(remove(value.begin(), value.end(), '\"'), value.end());
					value.erase(remove(value.begin(), value.end(), ';'), value.end());

					attribute.responseErr = value;
				}
				else if (line.find("P2_min =") != string::npos) {
					string key, value;
					size_t pos = line.find('=');

					key = line.substr(0, pos);
					value = line.substr(pos + 1);

					// 공백 및 특수문자 제거
					value.erase(remove(value.begin(), value.end(), ' '), value.end());
					value.erase(remove(value.begin(), value.end(), '\"'), value.end());
					value.erase(remove(value.begin(), value.end(), ';'), value.end());

					size_t ms_pos = value.find("ms");
					if (ms_pos != string::npos) {
						value = value.substr(0, ms_pos); // "kbps" 제거
					}
					attribute.P2_min = stof(value);
				}
				else if (line.find("ST_min =") != string::npos) {
					string key, value;
					size_t pos = line.find('=');

					key = line.substr(0, pos);
					value = line.substr(pos + 1);

					// 공백 및 특수문자 제거
					value.erase(remove(value.begin(), value.end(), ' '), value.end());
					value.erase(remove(value.begin(), value.end(), '\"'), value.end());
					value.erase(remove(value.begin(), value.end(), ';'), value.end());

					size_t ms_pos = value.find("ms");
					if (ms_pos != string::npos) {
						value = value.substr(0, ms_pos); // "kbps" 제거
					}
					attribute.ST_min = stof(value);
				}
				else if (line.find("N_As_timeout =") != string::npos) {
					string key, value;
					size_t pos = line.find('=');

					key = line.substr(0, pos);
					value = line.substr(pos + 1);

					// 공백 및 특수문자 제거
					value.erase(remove(value.begin(), value.end(), ' '), value.end());
					value.erase(remove(value.begin(), value.end(), '\"'), value.end());
					value.erase(remove(value.begin(), value.end(), ';'), value.end());

					size_t ms_pos = value.find("ms");
					if (ms_pos != string::npos) {
						value = value.substr(0, ms_pos); // "kbps" 제거
					}
					attribute.N_As_timeout = stof(value);
				}
				else if (line.find("N_Cr_timeout =") != string::npos) {
					string key, value;
					size_t pos = line.find('=');

					key = line.substr(0, pos);
					value = line.substr(pos + 1);

					// 공백 및 특수문자 제거
					value.erase(remove(value.begin(), value.end(), ' '), value.end());
					value.erase(remove(value.begin(), value.end(), '\"'), value.end());
					value.erase(remove(value.begin(), value.end(), ';'), value.end());

					size_t ms_pos = value.find("ms");
					if (ms_pos != string::npos) {
						value = value.substr(0, ms_pos); // "kbps" 제거
					}
					attribute.N_Cr_timeout = stof(value);
				}
				else if (line.find("configurable_frames {") != string::npos) {
					while (true) {
						getline(file, line);
						if (line.find("}") != string::npos) {
							break;
						}
						stringstream ss(line);
						string configurable_frame;
						getline(ss, configurable_frame, ';');
						configurable_frame.erase(remove(configurable_frame.begin(), configurable_frame.end(), ' '), configurable_frame.end());
						attribute.configurable_frames.push_back(configurable_frame);
					}
				}
			}
		}
		else if (section == "Schedule_tables") {
			stringstream ss(line);
			string name;
			getline(ss, name, '{');
			w_ScheduleTable table;
			name.erase(remove(name.begin(), name.end(), ' '), name.end());
			table.name = name;

			// 데이터 추출
			string data;
			getline(ss, data, '{');
			stringstream dataStream(data);
			string value;

			// 테스트
			//CString test(table.name.c_str());
			//MessageBox(_T("schedule table:") + test);

			while (true) {
				getline(file, line);
				if (line.find("}") != string::npos) {
					w_ScheduleTables.push_back(table);
					break;
				}
				w_Schedule schedule;
				string temp;

				istringstream stream(line);
				stream >> schedule.name >> temp >> schedule.delay;

				table.schedule.push_back(schedule);

				// 테스트
				//CString test1(schedule.name.c_str());
				//CString test2;
				//test2.Format(_T("%d"), schedule.delay);
				//MessageBox(_T("schedule :") + test1 + "/" + test2);
			}
		}
		else if (section == "Signal_encoding_types") {
			stringstream ss(line);
			w_SignalEncoding encodeing;
			string name;
			getline(ss, name, '{');
			name.erase(remove(name.begin(), name.end(), ' '), name.end());
			encodeing.name = name;

			while (true) {
				getline(file, line);
				if (line.find("}") != string::npos) {
					w_SignalEncodings.push_back(encodeing);
					break;
				}
				// 데이터 추출
				stringstream lineStream(line);
				string data;
				getline(lineStream, data, ';');

				string value;
				stringstream dataStream(data);

				getline(dataStream, value, ',');
				value.erase(remove(value.begin(), value.end(), ' '), value.end());

				if (value == "logical_value") {
					encodeing.isPhysical = false;
					w_LogicalValue l_value;

					getline(dataStream, value, ',');
					l_value.value = stoi(value);
					getline(dataStream, value, ',');
					value.erase(remove(value.begin(), value.end(), '"'), value.end());
					l_value.description = value;

					encodeing.logicalValues.push_back(l_value);
				}
				else {
					encodeing.isPhysical = true;
					w_PhysicalValue p_value;

					getline(dataStream, value, ',');
					p_value.minValue = stoi(value);
					getline(dataStream, value, ',');
					p_value.maxValue = stoi(value);
					getline(dataStream, value, ',');
					p_value.scale = stoi(value);
					getline(dataStream, value, ',');
					p_value.offset = stoi(value);

					// 테스트
					//CString test1;
					//test1.Format(_T("%d"), p_value.offset);
					//MessageBox(_T("w_PhysicalValue :") + test1 + "/");

					encodeing.physicalValue = p_value;
				}
			}
		}
		//else if (section == "Signal_representation") {
		//	w_Parser_SignalRepresentation(line);
		//}
	}

	// Nodes test
	//for (int i = 0; i < 8; i++) {
	//	CString test(w_Slave_names[i].c_str());
	//	MessageBox(test);
	//}
	// Signals test
	//CString test(w_Signals[0].txNode.c_str());
	//MessageBox(_T("tx") + test);
	// Frames test
	//for (int i = 0; i < 8; i++) {
	//	CString test(w_Frames[i].name.c_str());
	//	MessageBox(test);
	//}
	// Node_attributes test
	//CString test(w_NodeAttributes[0].linProtocol.c_str());
	//MessageBox(_T("linProtocol  : ") + test);
	//CString test2(w_NodeAttributes[0].configurable_frames[0].c_str());
	//CString test3(w_NodeAttributes[0].configurable_frames[1].c_str());
	//MessageBox(_T("configurable_frames  : ") + test2 + ", " + test3);
	// Schedule_tables test
	//for (int i = 0; i < 8; i++) {
	//	CString test(w_ScheduleTables[0].schedule[i].name.c_str());
	//	MessageBox(test);
	//}

	// 기본 세팅
	// 클라이언트 생성

	CString FilePath(filePath.c_str());
	mFileName.SetWindowTextW(FilePath);
	MessageBox(FilePath);

	file.close();

	return 0;
}


void CLINProjectDlg::w_Parser_Config(string& line) {
	string key, value;
	size_t pos = line.find('=');
	if (pos == string::npos) return; // '=' 없으면 무시

	key = line.substr(0, pos);
	value = line.substr(pos + 1);

	// 공백 및 특수문자 제거
	key.erase(remove(key.begin(), key.end(), ' '), key.end());
	key.erase(remove(key.begin(), key.end(), '\"'), key.end());
	value.erase(remove(value.begin(), value.end(), ' '), value.end());
	value.erase(remove(value.begin(), value.end(), '\"'), value.end());
	value.erase(remove(value.begin(), value.end(), ';'), value.end());

	// 값 저장
	if (key == "LIN_protocol_version") {
		w_LIN_protocol_version = stof(value);

		//CString test;
		//test.Format(_T("%f"), w_LIN_protocol_version);
		//MessageBox(test);
	}
	else if (key == "LIN_language_version") {
		w_LIN_language_version = stof(value);
	}
	else if (key == "LIN_speed") {
		size_t kbps_pos = value.find("kbps");
		if (kbps_pos != string::npos) {
			value = value.substr(0, kbps_pos); // "kbps" 제거
		}
		w_LIN_speed = stof(value);
	}
}
void CLINProjectDlg::w_Parser_Nodes(string& line) {
	line.erase(remove(line.begin(), line.end(), ';'), line.end());
	stringstream ss(line);
	string value;

	getline(ss, value, ':');
	value.erase(remove(value.begin(), value.end(), ' '), value.end());

	if (value == "Master") {
		getline(ss, value, ',');
		value.erase(remove(value.begin(), value.end(), ' '), value.end());

		w_Client_name = value;

		getline(ss, value, ',');
		value.erase(remove(value.begin(), value.end(), ' '), value.end());
		value.erase(remove(value.begin(), value.end(), 'ms'), value.end());
		w_delay = stoi(value);
		getline(ss, value, ',');
		value.erase(remove(value.begin(), value.end(), ' '), value.end());
		value.erase(remove(value.begin(), value.end(), 'ms'), value.end());
		w_IFS = stoi(value);
	}
	else if (value == "Slaves:") {
		while (getline(ss, value, ',')) {
			value.erase(remove(value.begin(), value.end(), ' '), value.end());
			w_Slave_names[slave_cnt++] = value;
		}
	}

}

void CLINProjectDlg::w_Parser_Signals(string& line) {
	stringstream ss(line);
	w_Signal sig;

	// 데이터 추출
	string data;
	getline(ss, data, ':');  // 신호 이름 추출
	data.erase(remove(data.begin(), data.end(), ' '), data.end());
	sig.name = data;
	getline(ss, data, ';');
	stringstream dataStream(data);
	string value;


	getline(dataStream, value, ',');
	sig.dataLength = stoi(value);
	getline(dataStream, value, ',');
	sig.defaultValue = stoi(value);
	getline(dataStream, value, ',');
	sig.txNode = value;
	if (getline(dataStream, value, ',')) {
		sig.rxNode = value;
	}
	else {
		sig.rxNode = ""; // 값이 없을 경우 기본값 설정
	}

	w_Signals.push_back(sig);

	// 테스트
	//CString test(sig.txNode.c_str());
	//MessageBox(_T("tx") + test);
	//CString test2(sig.rxNode.c_str());
	//MessageBox(_T("rx") + test2);
}
void CLINProjectDlg::w_Parser_DiagnosticSignals(string& line) {
	stringstream ss(line);
	w_DiagnosticSignal sig;
	getline(ss, sig.name, ':');

	// 데이터 추출
	string data;
	getline(ss, data, ';');
	stringstream dataStream(data);
	string value;

	getline(dataStream, value, ',');
	sig.size = stoi(value);
	getline(dataStream, value, ',');
	sig.value = stoi(value);

	w_DiagnosticSignals.push_back(sig);
}
