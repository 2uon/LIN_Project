
// LIN_ProjectDlg.cpp: 구현 파일
//

#include "pch.h"
#include "framework.h"
#include "LIN_ProjectDlg.h"
#include "afxdialogex.h"
#include <fstream>
#include <sstream>

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
	DDX_Control(pDX, IDC_SignalList, mSignalList);
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
	CRect rtTrace;
	mTraceList.GetWindowRect(&rtTrace);
	mTraceList.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);

	mTraceList.InsertColumn(0, TEXT("Frame ID"), LVCFMT_LEFT, rtTrace.Width() * 0.2);
	mTraceList.InsertColumn(1, TEXT("Frame Data"), LVCFMT_LEFT, rtTrace.Width() * 0.6);
	mTraceList.InsertColumn(2, TEXT("Error Flags"), LVCFMT_LEFT, rtTrace.Width() * 0.2);

	CRect rtSignal;
	mSignalList.GetWindowRect(&rtSignal);
	mSignalList.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);

	mSignalList.InsertColumn(0, TEXT(""), LVCFMT_LEFT, rtSignal.Width() * 0.15);
	mSignalList.InsertColumn(1, TEXT(""), LVCFMT_LEFT, rtSignal.Width() * 0.15);
	mSignalList.InsertColumn(2, TEXT("Name"), LVCFMT_LEFT, rtSignal.Width() * 0.7);


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


int CLINProjectDlg::w_LDF_parse(string filePath) {
	ifstream file(filePath);
	if (!file.is_open()) {
		return -1;
	}
	CString FilePath(filePath.c_str());
	MessageBox(FilePath);
	mFileName.SetWindowTextW(FilePath);

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
		else if (line.find("Signal_representation {") != string::npos) {
			section = "Signal_representation";
		}
		else if (line.find("Signal_representation {") != string::npos) {
			section = "Signal_representation";
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
			getline(ss, frame.name, ':');

			// 데이터 추출
			string data;
			getline(ss, data, ';');
			stringstream dataStream(data);
			string value;

			getline(dataStream, value, ',');
			frame.id = stoi(value);
			getline(dataStream, value, ',');
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
				getline(dataStream, dataStruct.name, ',');
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
				getline(dataStream, dataStruct.name, ',');
				getline(dataStream, value, ',');
				dataStruct.start = stoi(value);
				frame.w_Data.push_back(dataStruct);
			}
		}
		else if (section == "Node_attributes") {
			stringstream ss(line);
			w_NodeAttribute attribute;
			getline(ss, attribute.name, ':');

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
				else if (line.find("LIN_protocol") == string::npos) {
					string key, value;
					size_t pos = line.find('=');

					key = line.substr(0, pos);
					value = line.substr(pos + 1);

					// 공백 및 특수문자 제거
					value.erase(remove(value.begin(), value.end(), ' '), value.end());
					value.erase(remove(value.begin(), value.end(), '\"'), value.end());
					value.erase(remove(value.begin(), value.end(), ';'), value.end());

					attribute.linProtocol = stof(value);

				}
				else if (line.find("configured_NAD") == string::npos) {
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
				else if (line.find("initial_NAD") == string::npos) {
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
				else if (line.find("product_id") == string::npos) {
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
				else if (line.find("response_error") == string::npos) {
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
				else if (line.find("P2_min") == string::npos) {
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
				else if (line.find("ST_min") == string::npos) {
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
				else if (line.find("N_As_timeout") == string::npos) {
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
				else if (line.find("N_Cr_timeout") == string::npos) {
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
				else if (line.find("configurable_frames {") == string::npos) {
					while (true) {
						getline(file, line);
						if (line.find("}") != string::npos) {
							break;
						}
						stringstream ss(line);
						string configurable_frame;
						getline(ss, configurable_frame, ';');
						attribute.configurable_frames.push_back(configurable_frame);
					}
				}
			}
		}
		else if (section == "Schedule_tables") { /////////////////////////////////
			w_Parser_ScheduleTables(line);
		}
		else if (section == "Signal_encoding_types") {
			w_Parser_SignalEncodingTypes(line);
		}
		else if (section == "Signal_representation") {
			w_Parser_SignalRepresentation(line);
		}
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
	stringstream ss(line);
	string key, temp;
	ss >> key;
	if (key == "Master:") {
		ss >> w_Client_name >> w_delay >> temp >> w_IFS;
	}
	else if (key == "Slaves:") {
		while (getline(ss, temp, ',')) {
			temp.erase(remove(temp.begin(), temp.end(), ';'), temp.end());
			w_Slave_names[slave_cnt++] = temp;
		}
	}
}

void CLINProjectDlg::w_Parser_Signals(string& line) {
	stringstream ss(line);
	w_Signal sig;
	getline(ss, sig.name, ':');  // 신호 이름 추출

	// 데이터 추출
	string data;
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


void CLINProjectDlg::w_Parser_ScheduleTables(string& line) {

}

void CLINProjectDlg::w_Parser_SignalEncodingTypes(string& line) {

}

void CLINProjectDlg::w_Parser_SignalRepresentation(string& line) {

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

	CFileDialog dlg(TRUE, _T("*.ldf"), _T(""), OFN_HIDEREADONLY, szFilter);

	if (IDOK == dlg.DoModal())

	{

		CString path = dlg.GetPathName();

		// CString → std::string 변환
		CT2CA pszConvertedAnsiString(path);
		string temp(pszConvertedAnsiString);

		w_LDF_parse(temp); // LIN 설정 파일 파싱

	}
}

void CLINProjectDlg::OnBnClickedSignal()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}
