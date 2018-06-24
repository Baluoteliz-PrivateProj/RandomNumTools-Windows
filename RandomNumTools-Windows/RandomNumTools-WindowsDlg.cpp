
// RandomNumTools-WindowsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "RandomNumTools-Windows.h"
#include "RandomNumTools-WindowsDlg.h"
#include "afxdialogex.h"

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



CRandomNumToolsWindowsDlg::CRandomNumToolsWindowsDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CRandomNumToolsWindowsDlg::IDD, pParent)
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

	return TRUE;  // return TRUE  unless you set the focus to a control
}

inline void CRandomNumToolsWindowsDlg::initCtrl()
{
	m_ftBtn.CreateFont(16, 0, 0, 0, FW_NORMAL, FALSE, FALSE, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, _T("Arial"));
	m_ftHead.CreateFont(15, 0, 0, 0, FW_NORMAL, FALSE, FALSE, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, _T("Arial"));
	m_ftTag.CreateFont(16, 0, 0, 0, FW_NORMAL, FALSE, FALSE, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, _T("Arial"));
	m_ftTitle.CreateFont(17, 0, 0, 0, FW_BOLD, FALSE, FALSE, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, _T("Arial"));
	
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

	m_strVendorTitle = _T("上海兆言网络科技有限公司");
	m_strVersionTag = _T("V1.0.0,Build100,2018/06/25,V1.0.0  @baluoteliz@gmail.com All Right Reserverd");

	CMFCButton::EnableWindowsTheming(FALSE);
}

inline void CRandomNumToolsWindowsDlg::uninitCtrl()
{

}

inline void CRandomNumToolsWindowsDlg::DrawClient(CDC *pDC)
{
	CRect rtText;
	CRect rtClient;
	CFont* defFont = pDC->SelectObject(&m_ftTitle);

	GetClientRect(&rtClient);
	pDC->FillSolidRect(rtClient.right / 2 - 200, 10, 400, 24, RGB(214, 219, 233));
	pDC->SetBkColor(RGB(214, 219, 233));
	pDC->SetTextColor(RGB(0xFF, 0xFF, 0xFF));
	pDC->TextOut(rtClient.Width() / 2 - 80,13,m_strVendorTitle,_tcslen(m_strVendorTitle));

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

void CRandomNumToolsWindowsDlg::OnBnClickedButtonImportfile()
{
	// TODO: Add your control notification handler code here
}


void CRandomNumToolsWindowsDlg::OnBnClickedButtonFullscreen()
{
	// TODO: Add your control notification handler code here
}


void CRandomNumToolsWindowsDlg::OnBnClickedButtonConfig()
{
	// TODO: Add your control notification handler code here
}


void CRandomNumToolsWindowsDlg::OnBnClickedButtonAdd()
{
	// TODO: Add your control notification handler code here
}


void CRandomNumToolsWindowsDlg::OnBnClickedButtonDesignation()
{
	// TODO: Add your control notification handler code here
}


void CRandomNumToolsWindowsDlg::OnBnClickedButtonMute()
{
	// TODO: Add your control notification handler code here
}


void CRandomNumToolsWindowsDlg::OnBnClickedButtonStart()
{
	// TODO: Add your control notification handler code here
}


void CRandomNumToolsWindowsDlg::OnBnClickedButtonSure()
{
	// TODO: Add your control notification handler code here
}

void CRandomNumToolsWindowsDlg::OnBnClickedBtnClose()
{

}

void CRandomNumToolsWindowsDlg::OnBnClickedBtnMin()
{


}