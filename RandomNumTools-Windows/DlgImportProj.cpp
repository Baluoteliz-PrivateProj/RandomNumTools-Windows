// DlgImportProj.cpp : implementation file
//

#include "stdafx.h"
#include "RandomNumTools-Windows.h"
#include "DlgImportProj.h"
#include "afxdialogex.h"
#include "Utilc.h"
using namespace CPlusBaluoteli;

#include "DlgImportNew.h"
#include "DlgImportEdit.h"


// CDlgImportProj dialog

IMPLEMENT_DYNAMIC(CDlgImportProj, CDialogEx)

CDlgImportProj::CDlgImportProj(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgImportProj::IDD, pParent)
{

}

CDlgImportProj::~CDlgImportProj()
{
}

void CDlgImportProj::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON_NEW, m_agBtnNew);
	DDX_Control(pDX, IDC_BUTTON_EDIT, m_agBtnEdit);
	DDX_Control(pDX, IDC_BUTTON_DELETE, m_agBtnDelete);
}


BEGIN_MESSAGE_MAP(CDlgImportProj, CDialogEx)
	ON_WM_NCHITTEST()
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(IDC_BUTTON_NEW, &CDlgImportProj::OnBnClickedButtonNew)
	ON_BN_CLICKED(IDC_BUTTON_EDIT, &CDlgImportProj::OnBnClickedButtonEdit)
	ON_BN_CLICKED(IDC_BUTTON_DELETE, &CDlgImportProj::OnBnClickedButtonDelete)
	ON_BN_CLICKED(IDC_BUTTON_SURE, &CDlgImportProj::OnBnClickedButtonSure)
	ON_BN_CLICKED(IDC_BUTTON_CANCEL, &CDlgImportProj::OnBnClickedButtonCancel)
END_MESSAGE_MAP()


// CDlgImportProj message handlers
BOOL CDlgImportProj::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	SetBackgroundImage(IDB_BITMAP_PARAM);
	initCtrl();

	AnimateWindow(1000, AW_BLEND | AW_SLIDE);
	Invalidate(TRUE);

	CMFCButton::EnableWindowsTheming(FALSE);
	
	return TRUE;
}

LRESULT CDlgImportProj::OnNcHitTest(CPoint point)
{
	CRect rt;
	GetClientRect(&rt);
	ClientToScreen(&rt);

	return rt.PtInRect(point) ? HTCAPTION : FALSE;
}

void CDlgImportProj::OnShowWindow(BOOL bShow, UINT nStatus)
{
}

inline void CDlgImportProj::initCtrl()
{
	CWnd* pParentWnd = GetParent();
	CRect rtParent;
	pParentWnd->GetClientRect(&rtParent);
	ClientToScreen(&rtParent);

	CRect rtImprt;
	GetClientRect(&rtImprt);

	int nPosX = rtParent.left + rtParent.Width() / 2 - rtImprt.Width() / 2;
	int nPosY = rtParent.top + rtParent.Height() / 2 - rtImprt.Height() / 2;
	MoveWindow(nPosX, nPosY, rtImprt.Width(), rtImprt.Height(), TRUE);


}

inline void CDlgImportProj::uninitCtrl()
{

}

void CDlgImportProj::OnBnClickedButtonNew()
{
	// TODO: Add your control notification handler code here
	CDlgImportNew dlgImportNew;
	INT_PTR nResponse = dlgImportNew.DoModal();
	if (IDOK == nResponse) {

	}
	else if (IDCANCEL == nResponse) {

	}
}


void CDlgImportProj::OnBnClickedButtonEdit()
{
	// TODO: Add your control notification handler code here
	CDlgImportEdit dlgImportEdit;
	INT_PTR nResponse = dlgImportEdit.DoModal();
	if (IDOK == nResponse) {

	}
	else if (IDCANCEL == nResponse) {

	}
}


void CDlgImportProj::OnBnClickedButtonDelete()
{
	// TODO: Add your control notification handler code here
}


void CDlgImportProj::OnBnClickedButtonSure()
{
	// TODO: Add your control notification handler code here

	CDialogEx::OnOK();
}


void CDlgImportProj::OnBnClickedButtonCancel()
{
	// TODO: Add your control notification handler code here

	CDialogEx::OnCancel();
}
