
// LIN_ProjectDlg.cpp: 구현 파일
//

#include "pch.h"
#include "framework.h"
#include "LIN_Project.h"
#include "LIN_ProjectDlg.h"
#include "afxdialogex.h"

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
	: CDialogEx(IDD_LIN_PROJECT_DIALOG, pParent), m_pThread(nullptr), m_bThreadRunning(false)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CLINProjectDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_Progress, mProgress);
	DDX_Control(pDX, IDC_ErrCode, mErrCode);
	DDX_Control(pDX, IDC_Tx0, mTx0);
	DDX_Control(pDX, IDC_Tx1, mTx1);
	DDX_Control(pDX, IDC_Tx2, mTx2);
	DDX_Control(pDX, IDC_Tx3, mTx3);
	DDX_Control(pDX, IDC_Tx4, mTx4);
	DDX_Control(pDX, IDC_Tx5, mTx5);
	DDX_Control(pDX, IDC_Tx6, mTx6);
	DDX_Control(pDX, IDC_Tx7, mTx7);
	DDX_Control(pDX, IDC_TraceList, mTraceList);
	DDX_Control(pDX, IDC_FrameName, mFrameName);
	DDX_Control(pDX, IDC_FrameId, mFrameId);
	DDX_Control(pDX, IDC_Delay, mDelay);
	DDX_Control(pDX, IDC_Trigger, mTrigger);
	DDX_Control(pDX, IDC_LogFile, mFileName);
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
	ON_BN_CLICKED(IDC_Signal, &CLINProjectDlg::OnBnClickedSignal)
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

	// TODO: 여기에 추가 초기화 작업을 추가합니다.
	// List 초기화
	CRect rt;
	mTraceList.GetWindowRect(&rt);
	mTraceList.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);

	mTraceList.InsertColumn(0, TEXT("Frame ID"), LVCFMT_LEFT, rt.Width() * 0.2);
	mTraceList.InsertColumn(1, TEXT("Frame Data"), LVCFMT_LEFT, rt.Width() * 0.6);
	mTraceList.InsertColumn(2, TEXT("Error Flags"), LVCFMT_LEFT, rt.Width() * 0.2);


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

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 애플리케이션의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

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

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CLINProjectDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

CLINProjectDlg::~CLINProjectDlg() {
	wLIN_clear();
}

// LIN 연결 및 세팅
int CLINProjectDlg::wLIN_connect() {
	// 클라이언트 생성

	result = LIN_RegisterClient(const_cast<char*>("Client"), 0, &hClient);

	mProgress.SetWindowTextW(_T("클라이언트 생성"));
	errCode.Format(_T("%d"), result);
	mErrCode.SetWindowTextW(errCode);


	// 하드웨어 등록
	for (HLINHW hw : HW_TYPES) {
		hHw = hw;
		result = LIN_ConnectClient(hClient, hHw);
		if (result == errOK) {
			break;
		}
	}
	mProgress.SetWindowTextW(_T("하드웨어 등록"));
	errCode.Format(_T("%d"), result);
	mErrCode.SetWindowTextW(errCode);

	if (result != errOK) {
		hHw = 0;
	}

	// 하드웨어 초기화 (마스터 모드, 19200 bit rate)
	result = LIN_InitializeHardware(hClient, hHw, modMaster, 19200);

	mProgress.SetWindowTextW(_T("하드웨어 초기화"));
	errCode.Format(_T("%d"), result);
	mErrCode.SetWindowTextW(errCode);


	if (result != errOK) {
		LIN_DisconnectClient(hClient, hHw);
		LIN_RemoveClient(hClient);
	}
	// 프레임 id 초기화
	for (BYTE frameId : FRAME_IDS) {
		result = LIN_RegisterFrameId(hClient, hHw, frameId, frameId);
	}

	// 프레임 엔트리 설정
	//for (int i = 0; i < LIN_MAX_SCHEDULES; i++) {
	//	Frames[i] = {};
	//	Frames[i].FrameId = FRAME_IDS[i];
	//	Frames[i].Direction = Directions[i];
	//	Frames[i].ChecksumType = cstEnhanced;
	//	Frames[i].Length = Lengths[i];
	//	memcpy(Frames[i].InitialData, &data[i], sizeof(data[0]));


	//	result = LIN_SetFrameEntry(hClient, hHw, &Frames[i]);
	//}
	Frames[0] = {};
	Frames[0].FrameId = FRAME_IDS[0];
	Frames[0].Direction = Directions[0];
	Frames[0].ChecksumType = cstEnhanced;
	Frames[0].Length = Lengths[0];
	memcpy(Frames[0].InitialData, &sendData[0], sizeof(sendData));
	Frames[0].Flags = FRAME_FLAG_RESPONSE_ENABLE;

	result = LIN_SetFrameEntry(hClient, hHw, &Frames[0]);

	Frames[1] = {};
	Frames[1].FrameId = FRAME_IDS[1];
	Frames[1].Direction = Directions[1];
	Frames[1].ChecksumType = cstEnhanced;
	Frames[1].Length = Lengths[1];
	memset(Frames[1].InitialData, 0, sizeof(sendData));

	result = LIN_SetFrameEntry(hClient, hHw, &Frames[1]);

	// 스케줄 설정 (더미)
	Schedules[0].Type = sltUnconditional;
	Schedules[0].FrameId[0] = FRAME_IDS[0];
	Schedules[0].Delay = delay;

	Schedules[1].Type = sltUnconditional;
	Schedules[1].FrameId[0] = FRAME_IDS[1];
	Schedules[1].Delay = delay;

	result = LIN_SetSchedule(hClient, hHw, 0, Schedules, 2);

	mProgress.SetWindowTextW(_T("스케줄 설정"));
	errCode.Format(_T("%d"), result);
	mErrCode.SetWindowTextW(errCode);

	return 0;
}

// LIN 스케줄 시작
int CLINProjectDlg::wLIN_start() {
	// 버스 깨우기
	result = LIN_XmtWakeUp(hClient, hHw);
	onPause = false;

	// 정지 상태일 때 (재시작)
	if (onPause) {
		result = LIN_ResumeSchedule(hClient, hHw);

		mProgress.SetWindowTextW(_T("스케줄 재시작"));
		errCode.Format(_T("%d"), result);
		mErrCode.SetWindowTextW(errCode);
	}
	// 정지 상태가 아닐 때 (시작)
	else {
		result = LIN_StartSchedule(hClient, hHw, 0);

		mProgress.SetWindowTextW(_T("스케줄 시작"));
		errCode.Format(_T("%d"), result);
		mErrCode.SetWindowTextW(errCode);
	}
	
	// 시작/재시작 성공 시 읽기 시작
	if (result == errOK && !m_bThreadRunning) {
		m_bThreadRunning = true;
		m_pThread = AfxBeginThread(wReadDataThread, this);
	}

	return 0;
}

// 정지
int CLINProjectDlg::wLIN_pause() {
	// 스케줄 정지
	result = LIN_SuspendSchedule(hClient, hHw);
	mProgress.SetWindowTextW(_T("스케줄 정지"));
	errCode.Format(_T("%d"), result);
	mErrCode.SetWindowTextW(errCode);

	onPause = true;
	return 0;
}

// 초기화
int CLINProjectDlg::wLIN_clear() {
	// 스케줄 삭제, 하드웨어 연결 해제
	result = LIN_DeleteSchedule(hClient, hHw, schedule_position);
	result = LIN_DisconnectClient(hClient, hHw);

	mProgress.SetWindowTextW(_T("연결 해제"));
	errCode.Format(_T("%d"), result);
	mErrCode.SetWindowTextW(errCode);

	
	onPause = false;
	onClear = true;
	return 0;
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
		if (onClear) {
			onClear = false;
			break;
		}

		// 데이터 읽기
		result = LIN_Read(hClient, &rcvMsg);
		if (result != errOK) {
			cout << "데이터 읽기 실패 :" << result << endl;

			mProgress.SetWindowTextW(_T("데이터 읽기"));
			errCode.Format(_T("%d"), result);
			mErrCode.SetWindowTextW(errCode);

		}
		else {
			cout << "데이터 읽기 성공 frame id :" << (rcvMsg.FrameId & 0x3F) << endl;
			cout << "Data :";

			for (int i = 0; i < 8; i++) {
				cout << hex << uppercase << (int)rcvMsg.Data[i] << " ";
			}
			cout << endl;
			
			mProgress.SetWindowTextW(_T("데이터 읽기"));
			errCode.Format(_T("%d"), result);
			mErrCode.SetWindowTextW(errCode);
			/*rx.Format(_T("%02X %02X %02X %02X %02X %02X %02X %02X"), rcvMsg.Data[0], rcvMsg.Data[1], rcvMsg.Data[2], rcvMsg.Data[3]
				, rcvMsg.Data[4], rcvMsg.Data[5], rcvMsg.Data[6], rcvMsg.Data[7]);
			mRx.SetWindowTextW(rx);*/

		}
		Sleep(delay);
	}
	return;
}

void CLINProjectDlg::OnBnClickedStart()
{
	if (!m_bThreadRunning) {
		wLIN_connect();
		wLIN_start();
	}
}

void CLINProjectDlg::OnBnClickedPause()
{
	wLIN_pause();
	m_bThreadRunning = false;
}

void CLINProjectDlg::OnBnClickedStop()
{
	wLIN_clear();
	m_bThreadRunning = false;
}
void CLINProjectDlg::OnBnClickedSend()
{
	CString temp;
	int high, low, i;
	CEdit* mTx[8] = { &mTx0, &mTx1, &mTx2, &mTx3, &mTx4, &mTx5, &mTx6, &mTx7 };

	for (i = 0; i < 8; i++) {
		temp = "";
		mTx[i] -> GetWindowTextW(temp);
		if (temp.GetLength() >= 2) {
			high = (temp[0] > '9') ? temp[0] - 'A' + 10 : temp[0] - '0';
			low = (temp[1] > '9') ? temp[1] - 'A' + 10 : temp[1] - '0';

			sendData[i] = (high << 4) | low;
		}
		else if (temp.GetLength() == 1){
			high = 0;
			low = (temp[0] > '9') ? temp[0] - 'A' + 10 : temp[0] - '0';

			sendData[i] = (high << 4) | low;
		}
	}

	LIN_UpdateByteArray(hClient, hHw, frameId, 0, 8, &sendData[0]);
}
void CLINProjectDlg::OnBnClickedOpenlog()
{
	static TCHAR BASED_CODE szFilter[] = _T("데이터베이스 (*.ldf) | *.ldf;||");

	CFileDialog dlg(TRUE, _T("*.ldf"), _T("database"), OFN_HIDEREADONLY, szFilter);

	if (IDOK == dlg.DoModal())

	{

		CString pathName = dlg.GetFileName();
		MessageBox(pathName);
		mFileName.SetWindowTextW(pathName);


	}
}

void CLINProjectDlg::OnBnClickedSignal()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}
