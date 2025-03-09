
// LIN_ProjectDlg.cpp: 구현 파일
//

#include "pch.h"
#include "framework.h"
#include "LIN_ProjectDlg.h"
#include "afxdialogex.h"
#include <fstream>
#include <sstream>
#include <d2d1.h>

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
	mTraceList.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT | LVS_EX_DOUBLEBUFFER);

	mTraceList.InsertColumn(0, TEXT("Frame ID"), LVCFMT_LEFT, rtTrace.Width() * 0.25);
	mTraceList.InsertColumn(1, TEXT("Frame Data"), LVCFMT_LEFT, rtTrace.Width() * 0.5);
	mTraceList.InsertColumn(2, TEXT("Error Flags"), LVCFMT_LEFT, rtTrace.Width() * 0.25);

	CRect rtSignal;
	mSignalList.GetWindowRect(&rtSignal);
	mSignalList.SetExtendedStyle(LVS_EX_CHECKBOXES | LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);

	mSignalList.InsertColumn(0, TEXT("Signal Name"), LVCFMT_LEFT, rtSignal.Width());

	// 그래프 초기화
	for (int i = 0; i < 9; i++) {
		initGraph(i);
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



int CLINProjectDlg::w_LDF_parse(string filePath) {
	// 초기화
	w_Signals = {};
	w_DiagnosticSignals = {};
	w_Frames = {};
	w_DiagnosticFrames = {};
	w_NodeAttributes = {};
	w_ScheduleTables = {};
	w_SignalEncodings = {};
	Frames = {};
	FrameNames = {};
	FrameIDs = {};
	schedulesSize = 0;
	schedule_position = 0;
	
	// 위젯 초기화
	mSchedule.ResetContent();
	mFrameId.ResetContent();
	mFrameName.SetWindowTextW(_T("- Frame Name -"));
	mTraceList.DeleteAllItems();
	mSignalList.DeleteAllItems();


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
		//else if (line.find("Signal_representation {") != string::npos) {
		//	section = "Signal_representation";
		//}

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
		else if (section == "Schedule_tables") { /////////////////////////////////
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
	result = LIN_RegisterClient(const_cast<char*>(w_Client_name.c_str()), 0, &hClient);

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
	result = LIN_InitializeHardware(hClient, hHw, modMaster, w_LIN_speed * 1000);

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

		mProgress.SetWindowTextW(_T("스케줄 설정"));
		errCode.Format(_T("%d"), result);
		mErrCode.SetWindowTextW(errCode);

		Schdules[schedulesSize] = s;
		schedulesSize++;
		
		CString temp(scheduleTable.name.c_str());
		mSchedule.AddString(temp);
	}
	
	// 신호
	int sigItem = 0;
	for (w_Signal signal : w_Signals) {
		CString temp(signal.name.c_str());
		mSignalList.InsertItem(sigItem++, temp);
	}

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
		result = LIN_StartSchedule(hClient, hHw, mSchedule.GetCurSel());

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
	m_bThreadRunning = false;
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
	m_bThreadRunning = false;
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
			
			int nItemNum = find(FrameIDs.begin(), FrameIDs.end(), (rcvMsg.FrameId & 0x3F)) - FrameIDs.begin();
			CString frameID;
			CString data;
			CString flag;

			frameID.Format(_T("0x%X"), (rcvMsg.FrameId & 0x3F));
			data.Format(_T("%X %X %X %X %X %X %X %X"), rcvMsg.Data[0], rcvMsg.Data[1], rcvMsg.Data[2], rcvMsg.Data[3],
				rcvMsg.Data[4], rcvMsg.Data[5], rcvMsg.Data[6], rcvMsg.Data[7]);
			flag.Format(_T("%X"), rcvMsg.ErrorFlags);

			mTraceList.SetItemText(nItemNum, 0, frameID);
			mTraceList.SetItemText(nItemNum, 1, data);
			mTraceList.SetItemText(nItemNum, 2, flag);
		}
		Sleep(delay);
	}
	return;
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
	static TCHAR BASED_CODE szFilter[] = _T("데이터베이스 (*.ldf) | *.ldf;||");
	CFileDialog dlg(TRUE, _T("*.ldf"), _T(""), OFN_HIDEREADONLY, szFilter);

	if (IDOK == dlg.DoModal()) {
		CString path = dlg.GetPathName();

		// CString → std::string 변환
		CT2CA pszConvertedAnsiString(path);
		string temp(pszConvertedAnsiString);

		w_LDF_parse(temp); // LIN 설정 파일 파싱

	}
}
void CLINProjectDlg::OnCbnSelchangeSchedule()
{
	int selNum = mSchedule.GetCurSel();
	w_Schedules s = Schdules[selNum];

	for (int i = 0; i < s.size; i++) {
		CString frameID;
		frameID.Format(_T("0x%X"), s.Schedule[i].FrameId[0]);
		mFrameId.AddString(frameID);
		mTraceList.InsertItem(i, frameID);
	}
}

void CLINProjectDlg::OnCbnSelchangeFrameid()
{
	int selNum = mFrameId.GetCurSel();
	w_Frame f = w_Frames[selNum];

	CString fName(f.name.c_str());
	mFrameName.SetWindowTextW(fName);

	frameId = f.id;
	/*CString temp;
	temp.Format(_T("%X, %d"), f.id, selNum);
	MessageBox(temp);*/
}

void CLINProjectDlg::OnBnClickedSend()
{
	int high, low, i;
	CEdit* mTx[8] = { &mTx0, &mTx1, &mTx2, &mTx3, &mTx4, &mTx5, &mTx6, &mTx7 };
	CString temp, delay;
	bool TxIsEmpty = false;
	for (i = 0; i < 8; i++) {
		temp = "";
		mTx[i]->GetWindowTextW(temp);
		if (temp == _T("")) {
			TxIsEmpty = true;
			break;
		}
	}

	mDelay.GetWindowTextW(delay);
	if (mFrameId.GetCurSel() == -1) {
		MessageBox(_T("Select Frame ID!"));
	}
	else if (delay == _T("")) {
		MessageBox(_T("Edit Delay!"));
	}
	else if (TxIsEmpty) {
		MessageBox(_T("Edit All Data!"));
	}
	else if (!m_bThreadRunning) {
		MessageBox(_T("Start Schedule!"));
	}
	else {
		for (i = 0; i < 8; i++) {
			temp = "";
			mTx[i]->GetWindowTextW(temp);
			if (temp.GetLength() >= 2) {
				high = (temp[0] > '9') ? temp[0] - 'A' + 10 : temp[0] - '0';
				low = (temp[1] > '9') ? temp[1] - 'A' + 10 : temp[1] - '0';

				sendData[i] = (high << 4) | low;
			}
			else if (temp.GetLength() == 1) {
				high = 0;
				low = (temp[0] > '9') ? temp[0] - 'A' + 10 : temp[0] - '0';

				sendData[i] = (high << 4) | low;
			}
		}

		LIN_UpdateByteArray(hClient, hHw, 0x18, 0, 8, &sendData[0]);
	}
}

void CLINProjectDlg::OnLvnItemchangedSignallist(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	if (pNMLV->uNewState & LVIS_STATEIMAGEMASK) // 체크박스 변경 감지
	{
		int index = pNMLV->iItem;
		auto i = find(begin(graphSig), end(graphSig), index);

		if (mSignalList.GetCheck(index)) {
			graphSig.push_back(index);
			for (int sig : graphSig) {
				CString signal(w_Signals[sig].name.c_str());
				mSig[sig].SetWindowTextW(signal);
			}
		}
		else if (i != end(graphSig)) {
			graphSig.erase(remove(graphSig.begin(), graphSig.end(), index), graphSig.end());
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
	pBottomAxis->SetAutomaticMode(CChartAxis::FullAutomatic);

	pBottomAxis->SetDiscrete(false);
	mGraph[index].ShowMouseCursor(false);
	CChartCrossHairCursor* pCrossHair = mGraph[index].CreateCrossHairCursor();

	/// 라인차트 파트
	CChartXYSerie* pSeries = nullptr;
	pSeries = mGraph[index].CreateLineSerie();

	/*double XVal[50];
	double YVal[50];
	for(int i = 0; i < 50; i++) {
		XVal[i] = i * 16;
		YVal[i] = sin(i) * 5000 + 47000;
	}
	pSeries->SetPoints(XVal, YVal, 50);
	pSeries->SetColor(RGB(255, 0, 0));*/
}