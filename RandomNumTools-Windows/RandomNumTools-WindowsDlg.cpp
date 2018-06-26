
// RandomNumTools-WindowsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "RandomNumTools-Windows.h"
#include "RandomNumTools-WindowsDlg.h"
#include "afxdialogex.h"
#include "Utilc.h"
using namespace CPlusBaluoteli;
using namespace CPlusBaluoteli::FormatStr;

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

		Sleep(500);
		if (_T("") != pVoid->m_StrHitName) {

			DWORD dwStart = GetTickCount();
			char szbuf[MAX_PATH] = { '\0' };
			sprintf_s(szbuf, "%s,%s,%d\n", "ThreadTTSProc", util::CommonFun::cs2s(pVoid->m_StrHitName).data(),GetTickCount());
			OutputDebugStringA(szbuf);

			pVoid->m_TtsWrapper.Speak(pVoid->m_StrHitName);
			DWORD dwEnd = GetTickCount();
			DWORD dwInterval = dwEnd - dwStart;

			pVoid->m_StrHitName = _T("");
		}

	}

	return 0;
}

CRandomNumToolsWindowsDlg::CRandomNumToolsWindowsDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CRandomNumToolsWindowsDlg::IDD, pParent),
	m_pThreadTTS(NULL)
	, m_bResume(FALSE)
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
}

BEGIN_MESSAGE_MAP(CRandomNumToolsWindowsDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_NCHITTEST()
	ON_WM_QUERYDRAGICON()
	ON_WM_SHOWWINDOW()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_CLOSE,OnBnClickedBtnClose)
	ON_BN_CLICKED(IDC_BUTTON_MIN,OnBnClickedBtnMin)
	ON_BN_CLICKED(IDC_BUTTON_IMPORTFILE, &CRandomNumToolsWindowsDlg::OnBnClickedButtonImportfile)
	ON_BN_CLICKED(IDC_BUTTON_FULLSCREEN, &CRandomNumToolsWindowsDlg::OnBnClickedButtonFullscreen)
	ON_BN_CLICKED(IDC_BUTTON_CONFIG, &CRandomNumToolsWindowsDlg::OnBnClickedButtonConfig)
	ON_BN_CLICKED(IDC_BUTTON_ADD, &CRandomNumToolsWindowsDlg::OnBnClickedButtonAdd)
	ON_BN_CLICKED(IDC_BUTTON_DESIGNATION, &CRandomNumToolsWindowsDlg::OnBnClickedButtonDesignation)
	ON_BN_CLICKED(IDC_BUTTON_MUTE, &CRandomNumToolsWindowsDlg::OnBnClickedButtonMute)
	ON_BN_CLICKED(IDC_BUTTON_START, &CRandomNumToolsWindowsDlg::OnBnClickedButtonStart)
	ON_BN_CLICKED(IDC_BUTTON_SURE, &CRandomNumToolsWindowsDlg::OnBnClickedButtonSure)
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
	SetBackgroundImage(IDB_BITMAP_MAIN);	
	initCtrl();
	initTTS();

	return TRUE;  // return TRUE  unless you set the focus to a control
}

inline void CRandomNumToolsWindowsDlg::initCtrl()
{
	m_ftBtn.CreateFont(16, 0, 0, 0, FW_NORMAL, FALSE, FALSE, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, _T("Arial"));
	m_ftHead.CreateFont(15, 0, 0, 0, FW_NORMAL, FALSE, FALSE, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, _T("Arial"));
	m_ftTag.CreateFont(16, 0, 0, 0, FW_NORMAL, FALSE, FALSE, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, _T("Arial"));
	m_ftTitle.CreateFont(30, 0, 0, 0, FW_BOLD, FALSE, FALSE, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, _T("Arial"));
	
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

	m_btnSure.EnableRoundCorner(TRUE);
	m_btnSure.SetBackColor(RGB(0x00, 0xA0, 0xE9), RGB(0x05, 0x78, 0xAA), RGB(0x05, 0x78, 0xAA), RGB(0xE6, 0xE6, 0xE6));
	m_btnSure.SetFont(&m_ftBtn);
	m_btnSure.SetTextColor(RGB(0xFF, 0xFF, 0xFF), RGB(0xFF, 0xFF, 0xFF), RGB(0xFF, 0xFF, 0xFF), RGB(0xCC, 0xCC, 0xCC));
	m_btnSure.SetWindowTextW(KStrSure);

	gConfig.setTagName("V1.0.0,Build100,2018/06/25,V1.0.0  @baluoteliz@gmail.com All Right Reserverd");
	gConfig.setTitleName("上海兆言网络科技有限公司");
	gConfig.setImprtDirPre("C:\\");
	gConfig.setVendorName("上海智慧树");

	m_strVendorTitle = _T("上海兆言网络科技有限公司");
	m_strVersionTag = _T("V1.0.0,Build100,2018/06/25,V1.0.0  @baluoteliz@gmail.com All Right Reserverd");

	CMFCButton::EnableWindowsTheming(FALSE);
}

inline void CRandomNumToolsWindowsDlg::uninitCtrl()
{

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

	SetTimer(1, 2000, nullptr);
	AFX_THREADPROC threadTTsProc = ThreadTTSProc;
	m_pThreadTTS = AfxBeginThread(threadTTsProc, this);
}

inline void CRandomNumToolsWindowsDlg::uninitTTS()
{
	m_bResume = TRUE;

	DWORD dwExitCode = 0;
	GetExitCodeThread(m_pThreadTTS->m_hThread, &dwExitCode);
	AfxEndThread(dwExitCode);
	KillTimer(1);

	m_TtsWrapper.DeleteSpVoice();
	m_TtsWrapper.UnInit();
}


inline void CRandomNumToolsWindowsDlg::DrawClient(CDC *pDC)
{
	CRect rtText;
	CRect rtClient;
	CFont* defFont = pDC->SelectObject(&m_ftTitle);

	GetClientRect(&rtClient);
	pDC->FillSolidRect(rtClient.right / 2 - 200, 5, 400, 24, RGB(214, 219, 233));
	pDC->SetBkColor(RGB(214, 219, 233));
	pDC->SetTextColor(RGB(27, 41, 62));
	pDC->TextOut(rtClient.Width() / 2 - 120,8,m_strVendorTitle,_tcslen(m_strVendorTitle));

	defFont = pDC->SelectObject(&m_ftTag);
	pDC->FillSolidRect(rtClient.right / 2 - 300, rtClient.Height()-24, 600, 24, RGB(0, 122, 204));
	pDC->SetBkColor(RGB(0, 122, 204));
	pDC->SetTextColor(RGB(0xFF, 0xFF, 0xFF));
	pDC->TextOut(rtClient.Width() / 2 - 250, rtClient.Height() - 21, m_strVersionTag, _tcslen(m_strVersionTag));

	pDC->SelectObject(defFont);
}

inline void CRandomNumToolsWindowsDlg::playSoundBk()
{
	PlaySound(NULL, NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
	PlaySound(_T("..\\Sounds\\Working.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
}

inline void CRandomNumToolsWindowsDlg::playSoundHit()
{
	PlaySound(NULL, NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
	PlaySound(_T("..\\Sounds\\OK.wav"), NULL, SND_FILENAME | SND_ASYNC);
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
	 
	::SetWindowPos(m_hWnd,HWND_TOPMOST,rt.left, rt.top, rt.Width(), rt.Height(), TRUE);
	rt.right = rt.left + 900;
	rt.bottom = rt.top + 550;
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
}

void CRandomNumToolsWindowsDlg::OnTimer(UINT_PTR nIDEvent)
{
	if (1 == nIDEvent) {

		static int nCount = 1;
		nCount++;

		char szbuf1[MAX_PATH] = { '\0' };
		sprintf_s(szbuf1, "Timer nCount:%d, %d\n", nCount, GetTickCount());
		OutputDebugStringA(szbuf1);
 
		m_StrHitName = util::CommonFun::s2cs(util::CommonFun::int2str(nCount));
	}
}

void CRandomNumToolsWindowsDlg::OnBnClickedButtonImportfile()
{
	// TODO: Add your control notification handler code here
	
	CFormatStr::Baluoteliz_MessageBox(_T("OnBnClickedButtonImportfile"));
}


void CRandomNumToolsWindowsDlg::OnBnClickedButtonFullscreen()
{
	// TODO: Add your control notification handler code here
	CFormatStr::Baluoteliz_MessageBox(_T("OnBnClickedButtonFullscreen"));
}


void CRandomNumToolsWindowsDlg::OnBnClickedButtonConfig()
{
	// TODO: Add your control notification handler code here
	CFormatStr::Baluoteliz_MessageBox(_T("OnBnClickedButtonConfig"));
}


void CRandomNumToolsWindowsDlg::OnBnClickedButtonAdd()
{
	// TODO: Add your control notification handler code here
	CFormatStr::Baluoteliz_MessageBox(_T("OnBnClickedButtonAdd"));
}


void CRandomNumToolsWindowsDlg::OnBnClickedButtonDesignation()
{
	// TODO: Add your control notification handler code here
	CFormatStr::Baluoteliz_MessageBox(_T("OnBnClickedButtonDesignation"));
}


void CRandomNumToolsWindowsDlg::OnBnClickedButtonMute()
{
	// TODO: Add your control notification handler code here
	CFormatStr::Baluoteliz_MessageBox(_T("OnBnClickedButtonMute"));
}


void CRandomNumToolsWindowsDlg::OnBnClickedButtonStart()
{
	// TODO: Add your control notification handler code here
	m_btnStart.EnableWindow(FALSE);
	m_btnStart.SwitchButtonStatus(CAGButton::AGBTN_DISABLE);
	playSoundBk();
	m_btnSure.EnableWindow(TRUE);
	m_btnSure.SwitchButtonStatus(CAGButton::AGBTN_NORMAL);
	m_btnSure.Invalidate(TRUE);
	m_btnStart.Invalidate(TRUE);
}


void CRandomNumToolsWindowsDlg::OnBnClickedButtonSure()
{
	// TODO: Add your control notification handler code here
	m_btnSure.EnableWindow(FALSE);
	m_btnSure.SwitchButtonStatus(CAGButton::AGBTN_DISABLE);
	playSoundHit();
	m_btnStart.EnableWindow(TRUE);
	m_btnStart.SwitchButtonStatus(CAGButton::AGBTN_NORMAL);
	m_btnSure.Invalidate(TRUE);
	m_btnStart.Invalidate(TRUE);
}

void CRandomNumToolsWindowsDlg::OnBnClickedBtnClose()
{
	uninitTTS();
	uninitCtrl();

	CDialogEx::OnCancel();;
}

void CRandomNumToolsWindowsDlg::OnBnClickedBtnMin()
{
	ShowWindow(SW_MINIMIZE);
}