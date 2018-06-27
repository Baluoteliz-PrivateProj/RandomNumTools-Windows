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
}


BEGIN_MESSAGE_MAP(CDlgConfig, CDialogEx)
	ON_WM_NCHITTEST()
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(IDC_BUTTON_SAVE, &CDlgConfig::OnBnClickedButtonSave)
	ON_BN_CLICKED(IDC_BUTTON_CANCLE, &CDlgConfig::OnBnClickedButtonCancle)
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