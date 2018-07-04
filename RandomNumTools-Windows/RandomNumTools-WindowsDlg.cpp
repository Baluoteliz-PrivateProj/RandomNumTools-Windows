
// RandomNumTools-WindowsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "RandomNumTools-Windows.h"
#include "afxdialogex.h"
#include "Utilc.h"
using namespace CPlusBaluoteli;
using namespace CPlusBaluoteli::util;
using namespace CPlusBaluoteli::control;
using namespace CPlusBaluoteli::FormatStr;
#include "RandomNumTools-WindowsDlg.h"
#include "DlgSpeeckTextRender.h"
#include "DlgFullScreenTextSpeech.h"

#include "DlgConfig.h"
#include "DlgImportProj.h"


using namespace CPlusBaluoteli::control;
#include "ProjDataManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CRandomNumToolsWindowsDlg dialog

UINT CRandomNumToolsWindowsDlg::ThreadTTSProc(LPVOID param)
{
	CRandomNumToolsWindowsDlg* pVoid = (CRandomNumToolsWindowsDlg*)param;
	while (!pVoid->m_bResume){

		if (_T("") != pVoid->m_StrHitName) {

			DWORD dwStart = GetTickCount();
			TCHAR szbuf[MAX_PATH] = { '\0' };
			swprintf_s(szbuf, _T("%s,%s,%d\n"), L"ThreadTTSProc", (pVoid->m_StrHitName),GetTickCount());
			OutputDebugStringW(szbuf);

			pVoid->m_TtsWrapper.Speak(pVoid->m_StrHitName);
			DWORD dwEnd = GetTickCount();
			DWORD dwInterval = dwEnd - dwStart;

			//pVoid->m_StrHitName = _T("");
		}

		//Sleep(500);
		pVoid->m_pThreadTTS->SuspendThread();
	}

	CFormatStr::Baluoteliz_WriteLog("ThreadProc Exit m_bResume: %d\n", pVoid->m_bResume);
	pVoid->SetTimer(2, 1000, NULL);

	return 0;
}

CRandomNumToolsWindowsDlg::CRandomNumToolsWindowsDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CRandomNumToolsWindowsDlg::IDD, pParent),
	m_pThreadTTS(NULL)
	, m_bResume(FALSE)
	, m_pProjInstance(nullptr),
	m_pMainTextSpeechRender(nullptr),
	m_pFullScreenTextSpeeckRender(nullptr)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CRandomNumToolsWindowsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON_START, m_btnStart);
	DDX_Control(pDX, IDC_BUTTON_SURE, m_btnSure);
	DDX_Control(pDX, IDC_BUTTON_IMPORTFILE, m_btnImport);
	DDX_Control(pDX, IDC_BUTTON_FULLSCREEN, m_btnFullScreen);
	DDX_Control(pDX, IDC_BUTTON_ADD, m_btnAdd);
	DDX_Control(pDX, IDC_BUTTON_MUTE, m_btnMute);
	DDX_Control(pDX, IDC_BUTTON_DESIGNATION, m_btnDesign);
	DDX_Control(pDX, IDC_BUTTON_CONFIG, m_btnConfig);
	DDX_Control(pDX, IDC_EDIT_ADD, m_editAddName);
	DDX_Control(pDX, IDC_EDIT_MUTE, m_editMuteName);
	DDX_Control(pDX, IDC_EDIT_DESIGNATION, m_editDesignation);
	DDX_Control(pDX, IDC_BUTTON_MIN, m_btnMin);
	DDX_Control(pDX, IDC_BUTTON_CLOSE, m_btnClose);
	DDX_Control(pDX, IDC_BUTTON_RESET, m_btnReset);
	DDX_Control(pDX, IDC_LIST_RandomLIst, m_listRandom);
}

BEGIN_MESSAGE_MAP(CRandomNumToolsWindowsDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_NCHITTEST()
	ON_WM_QUERYDRAGICON()
	ON_WM_SHOWWINDOW()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_CLOSE, OnBnClickedBtnClose)
	ON_BN_CLICKED(IDC_BUTTON_MIN, OnBnClickedBtnMin)
	ON_BN_CLICKED(IDC_BUTTON_IMPORTFILE, &CRandomNumToolsWindowsDlg::OnBnClickedButtonImportfile)
	ON_BN_CLICKED(IDC_BUTTON_FULLSCREEN, &CRandomNumToolsWindowsDlg::OnBnClickedButtonFullscreen)
	ON_BN_CLICKED(IDC_BUTTON_CONFIG, &CRandomNumToolsWindowsDlg::OnBnClickedButtonConfig)
	ON_BN_CLICKED(IDC_BUTTON_ADD, &CRandomNumToolsWindowsDlg::OnBnClickedButtonAdd)
	ON_BN_CLICKED(IDC_BUTTON_DESIGNATION, &CRandomNumToolsWindowsDlg::OnBnClickedButtonDesignation)
	ON_BN_CLICKED(IDC_BUTTON_MUTE, &CRandomNumToolsWindowsDlg::OnBnClickedButtonMute)
	ON_BN_CLICKED(IDC_BUTTON_START, &CRandomNumToolsWindowsDlg::OnBnClickedButtonStart)
	ON_BN_CLICKED(IDC_BUTTON_SURE, &CRandomNumToolsWindowsDlg::OnBnClickedButtonSure)
	ON_MESSAGE(RandomMsg_IMPORT_PROJ, OnImportFile)
	ON_MESSAGE(RandomMsg_DELETE_PROJ,OnDeleteFile)
	ON_MESSAGE(RandomMsg_FULLSCREEN_SPEECH, OnFullScreenSpeech)
	ON_MESSAGE(RandomMsg_UpdateRandom_Interval,OnUpdateRandomInterval)
	ON_BN_CLICKED(IDC_BUTTON_RESET, &CRandomNumToolsWindowsDlg::OnBnClickedButtonReset)
	ON_MESSAGE(RandomMsg_UpdateRandom_CandidateList,OnUpdateRandomListBox)
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// CRandomNumToolsWindowsDlg message handlers

BOOL CRandomNumToolsWindowsDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
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

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
#if 0

	std::string mainbuffer;
	int nCount = 0;
	FILE *pFile = fopen("..\\data\\延安军事会议\\延安军事会议.dat", "ab+");
	fflush(pFile);
	long llen1 = ftell(pFile);
	char buffer[10];
	int nLen = 8;
	if (NULL != pFile) {
		while (!feof(pFile)) {
			ZeroMemory(buffer, nLen);
			nCount = fread(buffer, sizeof(char), nLen, pFile);
			if (ferror(pFile)) {
				break;
			}
			mainbuffer.append(buffer,nCount);
			break;
		}
	}

	llen1 = ftell(pFile);
	int nres = fseek(pFile, 0, SEEK_END);
	int nWritten = fwrite("王猛\r\n", 1, 6, pFile);
	fclose(pFile);
#endif
	
#if 0
	CFileData filedata("..\\data\\延安军事会议\\延安军事会议.dat", "..\\data\\延安军事会议\\mute.dat", "..\\data\\延安军事会议\\designation.dat");
	BOOL bres = filedata.dataCheckSelf();
	char szbuf[MAX_PATH] = { '\0' };
	std::string strName;
	if (bres & CFileData::error_Ok) {

		do{
			static int nCount = 0;
			DWORD dwStart = GetTickCount();
			bres = filedata.randomStr(strName);
			DWORD dwEnd = GetTickCount();
			nCount++;
			sprintf_s(szbuf, "[%d]: random str:%s,Interval: %d\n",nCount,strName.data(),dwEnd - dwStart);
			if (bres == CFileData::error_Ok)
				OutputDebugStringA(szbuf);
		} while (bres == CFileData::error_Ok);
	}
	if (bres & CFileData::error_SameItemConflict_MuteAndSpecial){

		sprintf_s(szbuf, "error_config_dataItem\n");
		OutputDebugStringA(szbuf);
	}
	if (bres & CFileData::error_AllDataItemMuted) {

		sprintf_s(szbuf, "error_invalidata\n");
		OutputDebugStringA(szbuf);
	}
	if (bres & CFileData::error_DataFile_Empty) {

		sprintf_s(szbuf, "error_dataFile_Empty\n");
		OutputDebugStringA(szbuf);
	}
	if (bres & CFileData::error_InValidItem_InSpecialFiles) {

		sprintf_s(szbuf, "error_InValidItem_InSpecialFiles\n");
		OutputDebugStringA(szbuf);
	}

#endif

#if 0
	CProjDataInstance *pInstance = CProjDataInstance::getInstance();
#endif

	SetBackgroundImage(IDB_BITMAP_MAIN);	
	initCtrl();
	initTTS();
	m_pProjInstance = CProjDataInstance::getInstance();

	return TRUE;  // return TRUE  unless you set the focus to a control
}

inline void CRandomNumToolsWindowsDlg::initCtrl()
{
	m_ftBtn.CreateFont(16, 0, 0, 0, FW_NORMAL, FALSE, FALSE, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, _T("Arial"));
	m_ftHead.CreateFont(15, 0, 0, 0, FW_NORMAL, FALSE, FALSE, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, _T("Arial"));
	m_ftTag.CreateFont(16, 0, 0, 0, FW_NORMAL, FALSE, FALSE, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, _T("Arial"));
	m_ftTitle.CreateFont(30, 0, 0, 0, FW_BOLD, FALSE, FALSE, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, _T("Arial"));
	m_hbrush = CreateSolidBrush(RGB(255, 0, 0));

#if 0
	m_editAddName.SetFont(&m_ftHead);
	m_editAddName.SetCaretPos(CPoint(12,148));
	m_editAddName.ShowCaret();
	m_editAddName.SetTip(KStrDesignationTip);
	m_editAddName.SetWindowTextW(KStrAddTip);

	m_editMuteName.SetFont(&m_ftHead);
	m_editMuteName.SetCaretPos(CPoint(12, 148));
	m_editMuteName.ShowCaret();
	m_editMuteName.SetTip(KStrDesignationTip);
	m_editMuteName.SetWindowTextW(KStrMuteTip);

	m_editDesignation.SetFont(&m_ftHead);
	m_editDesignation.SetCaretPos(CPoint(12, 148));
	m_editDesignation.ShowCaret();
	m_editDesignation.SetTip(KStrDesignationTip);
	m_editDesignation.SetWindowTextW(KStrDesignationTip);
#endif
	
	CreateDirectory(L"..\\data\\", NULL);
	CreateDirectory(L"..\\随机结果\\", NULL);

	m_btnAdd.EnableWindow(FALSE);
	m_btnMute.EnableWindow(FALSE);
	m_btnDesign.EnableWindow(FALSE);

	m_btnImport.EnableRoundCorner(TRUE);
	m_btnImport.SetBackColor(RGB(0x00, 0xA0, 0xE9), RGB(0x05, 0x78, 0xAA), RGB(0x05, 0x78, 0xAA), RGB(0xE6, 0xE6, 0xE6));
	m_btnImport.SetFont(&m_ftBtn);
	m_btnImport.SetTextColor(RGB(0xFF, 0xFF, 0xFF), RGB(0xFF, 0xFF, 0xFF), RGB(0xFF, 0xFF, 0xFF), RGB(0xCC, 0xCC, 0xCC));
	m_btnImport.SetWindowTextW(KStrImportFile);

	m_btnImport.EnableRoundCorner(TRUE);
	m_btnImport.SetBackColor(RGB(0x00, 0xA0, 0xE9), RGB(0x05, 0x78, 0xAA), RGB(0x05, 0x78, 0xAA), RGB(0xE6, 0xE6, 0xE6));
	m_btnImport.SetFont(&m_ftBtn);
	m_btnImport.SetTextColor(RGB(0xFF, 0xFF, 0xFF), RGB(0xFF, 0xFF, 0xFF), RGB(0xFF, 0xFF, 0xFF), RGB(0xCC, 0xCC, 0xCC));
	m_btnImport.SetWindowTextW(KStrImportFile);

	m_btnFullScreen.EnableRoundCorner(TRUE);
	m_btnFullScreen.SetBackColor(RGB(0x00, 0xA0, 0xE9), RGB(0x05, 0x78, 0xAA), RGB(0x05, 0x78, 0xAA), RGB(0xE6, 0xE6, 0xE6));
	m_btnFullScreen.SetFont(&m_ftBtn);
	m_btnFullScreen.SetTextColor(RGB(0xFF, 0xFF, 0xFF), RGB(0xFF, 0xFF, 0xFF), RGB(0xFF, 0xFF, 0xFF), RGB(0xCC, 0xCC, 0xCC));
	m_btnFullScreen.SetWindowTextW(KStrFullScreen);

	m_btnConfig.EnableRoundCorner(TRUE);
	m_btnConfig.SetBackColor(RGB(0x00, 0xA0, 0xE9), RGB(0x05, 0x78, 0xAA), RGB(0x05, 0x78, 0xAA), RGB(0xE6, 0xE6, 0xE6));
	m_btnConfig.SetFont(&m_ftBtn);
	m_btnConfig.SetTextColor(RGB(0xFF, 0xFF, 0xFF), RGB(0xFF, 0xFF, 0xFF), RGB(0xFF, 0xFF, 0xFF), RGB(0xCC, 0xCC, 0xCC));
	m_btnConfig.SetWindowTextW(KStrConfig);

	m_btnStart.EnableRoundCorner(TRUE);
	m_btnStart.SetBackColor(RGB(0x00, 0xA0, 0xE9), RGB(0x05, 0x78, 0xAA), RGB(0x05, 0x78, 0xAA), RGB(0xE6, 0xE6, 0xE6));
	m_btnStart.SetFont(&m_ftBtn);
	m_btnStart.SetTextColor(RGB(0xFF, 0xFF, 0xFF), RGB(0xFF, 0xFF, 0xFF), RGB(0xFF, 0xFF, 0xFF), RGB(0xCC, 0xCC, 0xCC));
	m_btnStart.SetWindowTextW(KStrStart);
	m_btnStart.EnableWindow(FALSE);

	m_btnSure.EnableRoundCorner(TRUE);
	m_btnSure.SetBackColor(RGB(0x00, 0xA0, 0xE9), RGB(0x05, 0x78, 0xAA), RGB(0x05, 0x78, 0xAA), RGB(0xE6, 0xE6, 0xE6));
	m_btnSure.SetFont(&m_ftBtn);
	m_btnSure.SetTextColor(RGB(0xFF, 0xFF, 0xFF), RGB(0xFF, 0xFF, 0xFF), RGB(0xFF, 0xFF, 0xFF), RGB(0xCC, 0xCC, 0xCC));
	m_btnSure.SetWindowTextW(KStrSure);
	m_btnSure.EnableWindow(FALSE);

	gConfig.setTagName("V1.0.0,Build100,2018/06/25,V1.0.0  @baluoteliz@gmail.com All Right Reserverd");
	gConfig.setTitleName("上海兆言网络科技有限公司");
	gConfig.setImportDirPrefix("C:\\");

	m_strVersionTag = CommonFun::s2cs(gConfig.getTagName());
	m_strVendorTitle = CommonFun::s2cs(gConfig.getVendorName());

	CMFCButton::EnableWindowsTheming(FALSE);

	if (nullptr == m_pMainTextSpeechRender) {
		m_pMainTextSpeechRender = new CDlgSpeeckTextRender(this);
		m_pMainTextSpeechRender->Create(CDlgSpeeckTextRender::IDD, this);
		m_pMainTextSpeechRender->ShowWindow(SW_SHOW);
	}
}

inline void CRandomNumToolsWindowsDlg::uninitCtrl()
{
	if (m_pMainTextSpeechRender) {
		delete m_pMainTextSpeechRender;
		m_pMainTextSpeechRender = nullptr;
	}
}

inline void CRandomNumToolsWindowsDlg::initTTS()
{
	m_TtsWrapper.Init();

	CString arrayVoicePackageName[50];
	int nVoicePackageCount = 50;

	int nCount = m_TtsWrapper.EnumAudioToken(arrayVoicePackageName, nVoicePackageCount);
	m_TtsWrapper.CreateSpVoice();
	ISpObjectToken* ppToken = nullptr;

	m_TtsWrapper.GetVoice(0, &ppToken);
	m_TtsWrapper.SetVoice(ppToken);
	m_TtsWrapper.SetRate(0);
	m_TtsWrapper.SetVolume(100);

#ifdef TTS_OLD
	//SetTimer(1, 1000, nullptr);
	AFX_THREADPROC threadTTsProc = ThreadTTSProc;
	m_pThreadTTS = AfxBeginThread(threadTTsProc, this);
#endif
}

inline void CRandomNumToolsWindowsDlg::uninitTTS()
{
	DWORD dwExitCode = 0;
	if (0 != GetExitCodeThread(m_pThreadTTS->m_hThread, &dwExitCode)) {
		
		//AfxEndThread(dwExitCode);
		//ExitThread(dwExitCode);
		//WaitForSingleObject(m_pThreadTTS->m_hThread, INFINITY);
		//CloseHandle(m_pThreadTTS->m_hThread);
		AfxEndThread(dwExitCode);
	}

	KillTimer(1);

	m_TtsWrapper.DeleteSpVoice();
	m_TtsWrapper.UnInit();
}

inline void CRandomNumToolsWindowsDlg::stop()
{
	CFormatStr::Baluoteliz_WriteLog("Stop\n");
	uninitTTS();
	CFormatStr::Baluoteliz_WriteLog("uninitTTS Already Exit\n");
	uninitCtrl();
}

inline void CRandomNumToolsWindowsDlg::DrawClient(CDC *pDC)
{
	CRect rtText;
	CRect rtClient;
	CFont* defFont = pDC->SelectObject(&m_ftTitle);

	m_strVendorTitle = util::CommonFun::s2cs(gConfig.getVendorName());

	GetClientRect(&rtClient);
	pDC->FillSolidRect(rtClient.right / 2 - 200, 5, 400, 24, RGB(214, 219, 233));
	pDC->SetBkColor(RGB(214, 219, 233));
	pDC->SetTextColor(RGB(27, 41, 62));
	pDC->TextOut(rtClient.left  + 120,10,m_strVendorTitle,_tcslen(m_strVendorTitle));

	if (!m_strImprtProjName.IsEmpty()) {

		CString strTemp = L"选中的项目：" + m_strImprtProjName;
		pDC->SetBkColor(RGB(230, 231, 232));
		pDC->SetTextColor(RGB(0, 122, 204));
		pDC->TextOut(rtClient.left + 330, rtClient.top + 100, strTemp, _tcslen(strTemp));
	}

	pDC->SelectObject(&m_ftTag);
	pDC->FillSolidRect(rtClient.right / 2 - 300, rtClient.Height()-24, 600, 24, RGB(0, 122, 204));
	pDC->SetBkColor(RGB(0, 122, 204));
	pDC->SetTextColor(RGB(0xFF, 0xFF, 0xFF));
	pDC->TextOut(rtClient.Width() / 2 - 250, rtClient.Height() - 21, m_strVersionTag, _tcslen(m_strVersionTag));
	
	pDC->SelectObject(defFont);
}

void CRandomNumToolsWindowsDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CRandomNumToolsWindowsDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	if (IsIconic())
	{
		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		DrawClient(&dc);

		//CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CRandomNumToolsWindowsDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

LRESULT CRandomNumToolsWindowsDlg::OnNcHitTest(CPoint point)
{
	CRect rt;
	GetClientRect(&rt);
	ClientToScreen(&rt);

	return rt.PtInRect(point) ? HTCAPTION : 0;
}

void CRandomNumToolsWindowsDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CRect rt;
	GetClientRect(&rt);
	ClientToScreen(&rt);
	 
	::SetWindowPos(m_hWnd,NULL,rt.left, rt.top, rt.Width(), rt.Height(), TRUE);
	rt.right = rt.left + 1280;
	rt.bottom = rt.top + 720;
	MoveWindow(&rt, TRUE);
	CenterWindow();

	GetClientRect(&rt);
	m_btnMin.MoveWindow(rt.right - 46, 3, 22, 22, TRUE);
	m_btnClose.MoveWindow(rt.right - 23, 3, 22, 22, TRUE);
	m_btnMin.SetBackColor(RGB(214, 219, 233), RGB(214, 219, 233), RGB(214, 219, 233), RGB(214, 219, 233));
	m_btnMin.SetBorderColor(RGB(214, 219, 233), RGB(214, 219, 233), RGB(214, 219, 233), RGB(214, 219, 233));
	m_btnMin.SetBackImage(IDB_BITMAP_MIN, RGB(0xFF, 0x00, 0xFF));

	m_btnClose.SetBackColor(RGB(214, 219, 233), RGB(214, 219, 233), RGB(214, 219, 233), RGB(214, 219, 233));
	m_btnClose.SetBorderColor(RGB(214, 219, 233), RGB(214, 219, 233), RGB(214, 219, 233), RGB(214, 219, 233));
	m_btnClose.SetBackImage(IDB_BITMAP_CLOSE, RGB(0xFF, 0x00, 0xFF));

	//m_btnStart.MoveWindow(rt.Width() /2 - 400, rt.Height() -50 , 200, 24, TRUE);
	//m_btnSure.MoveWindow(rt.Width()/2 + 100, rt.Height() - 50, 200, 24, TRUE);

	if (m_pMainTextSpeechRender){
		m_pMainTextSpeechRender->MoveWindow(rt.left + 10, rt.top + 221, 1050, 414, TRUE);
	}

	m_listRandom.MoveWindow(rt.left + 1070, rt.top + 221, 200, 414, TRUE);
}

void CRandomNumToolsWindowsDlg::OnTimer(UINT_PTR nIDEvent)
{
	if (1 == nIDEvent) {

		static int nCount = 1;
		nCount++;

		char szbuf1[MAX_PATH] = { '\0' };
		sprintf_s(szbuf1, "Timer nCount:%d, %d\n", nCount, GetTickCount());
		OutputDebugStringA(szbuf1);

		m_pThreadTTS->ResumeThread();
 
		m_StrHitName = util::CommonFun::s2cs(util::CommonFun::int2str(nCount));
	}

	if (2 == nIDEvent) {
	
		CFormatStr::Baluoteliz_WriteLog("Enter Timer nIDEvent : %d \n", nIDEvent);
		KillTimer(nIDEvent);
		stop();
		PostMessage(WM_COMMAND, WPARAM(IDCANCEL), NULL);
		CFormatStr::Baluoteliz_WriteLog("postMessge IDCANCLE");
	}
}

void CRandomNumToolsWindowsDlg::OnBnClickedButtonImportfile()
{
	// TODO: Add your control notification handler code here
	CFormatStr::Baluoteliz_WriteLog("%s", __FUNCTION__);

#if 0
	std::string strDefPath = util::CommonFun::getFilePath();
	CString strDefPathName = util::CommonFun::s2cs(strDefPath);
	CFileDialog ImportFileDlg(TRUE, NULL, strDefPathName, 
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_ALLOWMULTISELECT
		, L"数据文档(*.dat)|*.dat|Normal text file (*.txt)|*.txt||", this);

	INT_PTR nResponse = ImportFileDlg.DoModal();
	if (IDOK == nResponse) {

		CString strOpenFileName = ImportFileDlg.GetPathName();
		gConfig.setImportDirPrefix(CommonFun::cs2s(strOpenFileName));
	}
#else

	CDlgImportProj dlgImportProj;
	INT_PTR nResponse = dlgImportProj.DoModal();
	if (IDOK == nResponse) {

	}
	else if (IDCANCEL == nResponse) {

	}

#endif
}


void CRandomNumToolsWindowsDlg::OnBnClickedButtonFullscreen()
{
	// TODO: Add your control notification handler code here
	CFormatStr::Baluoteliz_WriteLog("%s", __FUNCTION__);

	CDlgFullScreenTextSpeech dlgFullScreenText;
	m_pFullScreenTextSpeeckRender = &dlgFullScreenText;
	INT_PTR nResponse =  dlgFullScreenText.DoModal();
	if (IDOK == nResponse){

	}
	else if(IDCANCEL == nResponse){

	}

	m_pFullScreenTextSpeeckRender = nullptr;
}


void CRandomNumToolsWindowsDlg::OnBnClickedButtonConfig()
{
	// TODO: Add your control notification handler code here
	CFormatStr::Baluoteliz_WriteLog("%s", __FUNCTION__);

	CDlgConfig dlgConfig;
	INT_PTR nResponse = dlgConfig.DoModal();
	if (IDOK == nResponse) {

		Invalidate(TRUE);
	}
}


void CRandomNumToolsWindowsDlg::OnBnClickedButtonAdd()
{
	// TODO: Add your control notification handler code here
	CFormatStr::Baluoteliz_WriteLog("%s", __FUNCTION__);
	if (m_pProjInstance) {
		CString strParam;
		m_editAddName.GetWindowTextW(strParam);
		m_editAddName.SetWindowTextW(L"");
		std::string strName = CommonFun::cs2s(strParam);
		if (m_pProjInstance->addStr(strName)) {

			FormatStr::CFormatStr::Baluoteliz_MessageBox(L"添加成功");
			updateCandidateListBox(strParam);
		}
	}
}


void CRandomNumToolsWindowsDlg::OnBnClickedButtonDesignation()
{
	// TODO: Add your control notification handler code here
	CFormatStr::Baluoteliz_WriteLog("%s", __FUNCTION__);
	if (m_pProjInstance) {
		CString strParam;
		m_editDesignation.GetWindowTextW(strParam);
		m_editDesignation.SetWindowTextW(L"");
		std::string strName = CommonFun::cs2s(strParam);
		m_pProjInstance->designationStr(strName);
	}
}


void CRandomNumToolsWindowsDlg::OnBnClickedButtonMute()
{
	// TODO: Add your control notification handler code here
	CFormatStr::Baluoteliz_WriteLog("%s", __FUNCTION__);
	if (m_pProjInstance) {
		CString strParam;
		m_editMuteName.GetWindowTextW(strParam);
		m_editMuteName.SetWindowTextW(L"");
		std::string strName = CommonFun::cs2s(strParam);
		m_pProjInstance->muteStr(strName);
	}
}


void CRandomNumToolsWindowsDlg::OnBnClickedButtonStart()
{
	// TODO: Add your control notification handler code here
	CFormatStr::Baluoteliz_WriteLog("%s", __FUNCTION__);

	if (m_pProjInstance){

		m_btnStart.EnableWindow(FALSE);
		m_btnStart.SwitchButtonStatus(CAGButton::AGBTN_DISABLE);
		SoundControl::playSoundBk();
		m_btnSure.EnableWindow(TRUE);
		m_btnSure.SwitchButtonStatus(CAGButton::AGBTN_NORMAL);
		m_btnSure.Invalidate(TRUE);
		m_btnStart.Invalidate(TRUE);

		if (m_pMainTextSpeechRender){
			m_pMainTextSpeechRender->btnRandom();
		}
	}
}

void CRandomNumToolsWindowsDlg::OnBnClickedButtonSure()
{
	// TODO: Add your control notification handler code here
	CFormatStr::Baluoteliz_WriteLog("%s", __FUNCTION__);
	
	if (m_pProjInstance && m_pProjInstance->RandomStr(m_StrHitName)) {

		updateCandidateListBox(m_StrHitName);
#ifdef TTS_OLD
		m_pThreadTTS->ResumeThread();
#endif
	}
	else{
		m_StrHitName = L"";
		CFormatStr::Baluoteliz_MessageBox(L"项目数据为空，或者 所有的数据都已经随机出现过了");
	}

	m_btnSure.EnableWindow(FALSE);
	m_btnSure.SwitchButtonStatus(CAGButton::AGBTN_DISABLE);
	SoundControl::playSoundHit();
	m_btnStart.EnableWindow(TRUE);
	m_btnStart.SwitchButtonStatus(CAGButton::AGBTN_NORMAL);
	m_btnSure.Invalidate(TRUE);
	m_btnStart.Invalidate(TRUE);

	if (m_pMainTextSpeechRender)
		m_pMainTextSpeechRender->btnChoose(m_StrHitName);
}

void CRandomNumToolsWindowsDlg::OnBnClickedBtnClose()
{
	CFormatStr::Baluoteliz_WriteLog("%s", __FUNCTION__);

#ifdef TTS_OLD
	KillTimer(1);
	m_bResume = TRUE;
	DWORD dwRet = 0;
	do
	{
		Sleep(100);
		dwRet = m_pThreadTTS->ResumeThread();
		CFormatStr::Baluoteliz_WriteLog("ResumeThread Return: %d;  m_bResume: %d\n", dwRet,m_bResume);
	} while ( 1 != dwRet);
#endif
}

void CRandomNumToolsWindowsDlg::OnBnClickedBtnMin()
{
	CFormatStr::Baluoteliz_WriteLog("%s", __FUNCTION__);

	ShowWindow(SW_MINIMIZE);
}

LRESULT CRandomNumToolsWindowsDlg::OnImportFile(WPARAM wParam, LPARAM lParam)
{
	LPRANDOM_IMPORT_PROJ lpData = (LPRANDOM_IMPORT_PROJ)wParam;
	if (lpData) {

		if (lpData->nRes & CFileData::error_DataFile_Empty)
			FormatStr::CFormatStr::Baluoteliz_MessageBox(L"该项目数据为空.请为该项目编辑数据.");
		m_strImprtProjName = lpData->strProjName;
		m_btnAdd.EnableWindow(TRUE);
		m_btnMute.EnableWindow(TRUE);
		m_btnDesign.EnableWindow(TRUE);
		m_btnStart.EnableWindow(TRUE);
		//m_btnFullScreen.EnableWindow(TRUE);

		updateCandidateListBoxFirst();
		Invalidate(TRUE);

		delete lpData; lpData = nullptr;
	}

	return TRUE;
}

LRESULT CRandomNumToolsWindowsDlg::OnDeleteFile(WPARAM wParam, LPARAM lParam)
{
	LPRANDOM_DELETE_PROJ lpData = (LPRANDOM_DELETE_PROJ)wParam;
	if (lpData) {

		if (m_strImprtProjName == lpData->strProjName) {
			m_strImprtProjName = "";

			m_btnAdd.EnableWindow(FALSE);
			m_btnDesign.EnableWindow(FALSE);
			m_btnMute.EnableWindow(FALSE);

			Invalidate(TRUE);

			delete lpData; lpData = nullptr;
		}
	}

	return TRUE;
}

LRESULT CRandomNumToolsWindowsDlg::OnFullScreenSpeech(WPARAM wParam, LPARAM lParam)
{
	LPRANDOM_FULLSCREEN_SPEECH lpDatat = (LPRANDOM_FULLSCREEN_SPEECH)wParam;
	if (lpDatat) {

		m_StrHitName = lpDatat->strSpeechText;
		m_pThreadTTS->ResumeThread();

		delete lpDatat; lpDatat = nullptr;
	}

	return TRUE;
}

LRESULT CRandomNumToolsWindowsDlg::OnUpdateRandomInterval(WPARAM wParam, LPARAM lParam)
{
	DWORD dwInterval = wParam;
	
	if (m_pMainTextSpeechRender)
		m_pMainTextSpeechRender->updateStartInterval(dwInterval);
	
	if (m_pFullScreenTextSpeeckRender)
		m_pFullScreenTextSpeeckRender->updateStartInterval(dwInterval);

	return TRUE;
}

LRESULT CRandomNumToolsWindowsDlg::OnUpdateRandomListBox(WPARAM wParam, LPARAM lParam)
{
	LPRANDOM_UPDATE_CANDIDATELILST lpData = (LPRANDOM_UPDATE_CANDIDATELILST)wParam;
	if (lpData) {
		
		updateCandidateListBox(lpData->strRandom);
		delete lpData; 
		lpData = nullptr;
	}

	return TRUE;
}

inline void CRandomNumToolsWindowsDlg::updateCandidateListBoxFirst()
{
	m_listRandom.ResetContent();
	m_vecCandidateList.clear();

	if (m_pProjInstance)
		m_pProjInstance->getCandidateList(m_vecCandidateList);

	for (vecStrIt it = m_vecCandidateList.begin(); m_vecCandidateList.end() != it; it++)	{

		CString strItem = CommonFun::s2cs(*it);
		m_listRandom.AddString(strItem);
	}
}

inline void CRandomNumToolsWindowsDlg::updateCandidateListBox(const CString &str)
{
	int nFindIndex = m_listRandom.FindStringExact(0, str);
	if (CB_ERR != nFindIndex) {
		m_listRandom.DeleteString(nFindIndex);
	}
	else
		m_listRandom.AddString(str);
}

void CRandomNumToolsWindowsDlg::OnBnClickedButtonReset()
{
	// TODO: Add your control notification handler code here
	if (m_strImprtProjName.IsEmpty())
		FormatStr::CFormatStr::Baluoteliz_MessageBox(L"需要先建项目.");

	if (m_pProjInstance) {

		int nres = m_pProjInstance->resetProj(m_strImprtProjName);
		if (nres & CFileData::error_Ok) {

			FormatStr::CFormatStr::Baluoteliz_MessageBox(L"重置成功");
			updateCandidateListBoxFirst();
		}
	}
}


HBRUSH CRandomNumToolsWindowsDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	if (IDC_LIST_RandomLIst == nCtlColor){
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(RGB(255, 255, 255));
		return m_hbrush;
	}
	else
		return CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  Change any attributes of the DC here

	// TODO:  Return a different brush if the default is not desired
}
