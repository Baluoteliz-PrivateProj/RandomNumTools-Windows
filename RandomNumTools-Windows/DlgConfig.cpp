// DlgConfig.cpp : implementation file
//

#include "stdafx.h"
#include "RandomNumTools-Windows.h"
#include "DlgConfig.h"
#include "afxdialogex.h"
#include "Utilc.h"
using namespace CPlusBaluoteli;

// CDlgConfig dialog

IMPLEMENT_DYNAMIC(CDlgConfig, CDialogEx)

CDlgConfig::CDlgConfig(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgConfig::IDD, pParent)
{

}

CDlgConfig::~CDlgConfig()
{
}

void CDlgConfig::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_VENDORNAME, m_editVendorName);
	DDX_Control(pDX, IDC_SLIDER_RandomInterval, m_sliderCtrl);
}


BEGIN_MESSAGE_MAP(CDlgConfig, CDialogEx)
	ON_WM_NCHITTEST()
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(IDC_BUTTON_SAVE, &CDlgConfig::OnBnClickedButtonSave)
	ON_BN_CLICKED(IDC_BUTTON_CANCLE, &CDlgConfig::OnBnClickedButtonCancle)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_RandomInterval, &CDlgConfig::OnNMCustomdrawSliderRandominterval)
END_MESSAGE_MAP()


// CDlgConfig message handlers
BOOL CDlgConfig::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	SetBackgroundImage(IDB_BITMAP_PARAM);
	initCtrl();

	AnimateWindow(1000, AW_BLEND | AW_SLIDE);
	Invalidate(TRUE);

	return TRUE;
}

inline void CDlgConfig::initCtrl()
{
	std::string strVendorName = gConfig.getVendorName();
	m_editVendorName.SetWindowTextW(util::CommonFun::s2cs(strVendorName));

	CWnd* pParentWnd = GetParent();
	CRect rtParent;
	pParentWnd->GetClientRect(&rtParent);
	ClientToScreen(&rtParent);

	CRect rtConfig;
	GetClientRect(&rtConfig);

	int nPosX = rtParent.left + rtParent.Width() / 2 - rtConfig.Width() / 2;
	int nPosY = rtParent.top + rtParent.Height() / 2 - rtConfig.Height() / 2;
	MoveWindow(nPosX, nPosY, rtConfig.Width(), rtConfig.Height(), TRUE);

	m_sliderCtrl.SetRange(1, 5);
	DWORD dwRandomInterval = util::CommonFun::str2int(gConfig.getRandomInterval());
	m_sliderCtrl.SetPos(dwRandomInterval / 10);
	
	CMFCButton::EnableWindowsTheming(FALSE);
}

inline void CDlgConfig::uninitCtrl()
{

}

LRESULT CDlgConfig::OnNcHitTest(CPoint point)
{
	CRect rt;
	GetClientRect(&rt);
	ClientToScreen(&rt);

	return rt.PtInRect(point) ? HTCAPTION : FALSE;
}

void CDlgConfig::OnBnClickedButtonSave()
{
	// TODO: Add your control notification handler code here
	CString strParam;
	m_editVendorName.GetWindowText(strParam);
	gConfig.setVendorName(util::CommonFun::cs2s(strParam));

	DWORD dwTimeInterval = 0;
	int nPos = m_sliderCtrl.GetPos();
	dwTimeInterval = nPos * 10;

	::PostMessage(theApp.GetMainWnd()->m_hWnd, RandomMsg_UpdateRandom_Interval, WPARAM(dwTimeInterval), NULL);

	CDialogEx::OnOK();
}


void CDlgConfig::OnBnClickedButtonCancle()
{
	// TODO: Add your control notification handler code here
	CDialogEx::OnCancel();
}

void CDlgConfig::OnShowWindow(BOOL bShow, UINT nStatus)
{
	int i = 0;
}

void CDlgConfig::OnNMCustomdrawSliderRandominterval(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;

	DWORD dwTimeInterval = 0;
	int nPos = m_sliderCtrl.GetPos();
	dwTimeInterval = nPos * 10;

	gConfig.setRandomInterval(util::CommonFun::int2str(dwTimeInterval));
}
