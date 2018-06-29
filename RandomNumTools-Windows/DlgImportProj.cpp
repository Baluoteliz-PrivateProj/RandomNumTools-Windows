// DlgImportProj.cpp : implementation file
//

#include "stdafx.h"
#include "RandomNumTools-Windows.h"
#include "DlgImportProj.h"
#include "afxdialogex.h"
#include "Utilc.h"
using namespace CPlusBaluoteli;
using namespace CPlusBaluoteli::control;

#include "DlgImportNew.h"
#include "DlgImportEdit.h"
#include "ProjDataManager.h"

// CDlgImportProj dialog

IMPLEMENT_DYNAMIC(CDlgImportProj, CDialogEx)

CDlgImportProj::CDlgImportProj(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgImportProj::IDD, pParent)
	, m_pProjDataInstance(NULL)
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
	DDX_Control(pDX, IDC_LIST_PROJ_DATA, m_ctrlAllData);
}


BEGIN_MESSAGE_MAP(CDlgImportProj, CDialogEx)
	ON_WM_NCHITTEST()
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(IDC_BUTTON_NEW, &CDlgImportProj::OnBnClickedButtonNew)
	ON_BN_CLICKED(IDC_BUTTON_EDIT, &CDlgImportProj::OnBnClickedButtonEdit)
	ON_BN_CLICKED(IDC_BUTTON_DELETE, &CDlgImportProj::OnBnClickedButtonDelete)
	ON_BN_CLICKED(IDC_BUTTON_SURE, &CDlgImportProj::OnBnClickedButtonSure)
	ON_BN_CLICKED(IDC_BUTTON_CANCEL, &CDlgImportProj::OnBnClickedButtonCancel)
	ON_NOTIFY(NM_CLICK, IDC_LIST_PROJ_DATA, &CDlgImportProj::OnNMClickListProjData)
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

	LONG lStyle;
	lStyle = GetWindowLong(m_ctrlAllData.m_hWnd, GWL_STYLE);
	lStyle &= ~LVS_TYPEMASK;
	lStyle |= LVS_REPORT;
	lStyle |= LVS_OWNERDRAWFIXED;
	SetWindowLong(m_ctrlAllData.m_hWnd, GWL_STYLE, lStyle);

	DWORD dwStyle = m_ctrlAllData.GetExtendedStyle();
	dwStyle |= LVS_EX_FULLROWSELECT;
	dwStyle |= LVS_EX_GRIDLINES;
	dwStyle |= LVS_EX_CHECKBOXES;
	dwStyle |= LVS_SHOWSELALWAYS;
	m_ctrlAllData.SetExtendedStyle(dwStyle);

	m_ctrlAllData.InsertColumn(0, KCtrlItemIndex, LVCFMT_CENTER, 100);
	m_ctrlAllData.InsertColumn(1, KCtrlItemName, LVCFMT_CENTER, 200);
	m_ctrlAllData.InsertColumn(2, KCtrlItemCount, LVCFMT_CENTER, 200);
	
	m_agBtnDelete.EnableWindow(FALSE);
	m_agBtnDelete.SwitchButtonStatus(CAGButton::AGBTN_DISABLE);
	m_agBtnEdit.EnableWindow(FALSE);
	m_agBtnEdit.SwitchButtonStatus(CAGButton::AGBTN_DISABLE);
	m_agBtnNew.EnableWindow(FALSE);
	m_agBtnNew.SwitchButtonStatus(CAGButton::AGBTN_DISABLE);


#if 0
	int nRow = m_ctrlAllData.InsertItem(0, _T("1"));
	m_ctrlAllData.SetItemText(nRow, 1, L"武汉一中");
	m_ctrlAllData.SetItemText(nRow, 2, L"3");

	nRow = m_ctrlAllData.InsertItem(nRow + 1, _T("2"));	
	m_ctrlAllData.SetItemText(nRow, 1, L"武汉二中");
	m_ctrlAllData.SetItemText(nRow, 2, L"3");

	nRow = m_ctrlAllData.InsertItem(nRow + 1, _T("3"));
	m_ctrlAllData.SetItemText(nRow, 1, L"武汉三中");
	m_ctrlAllData.SetItemText(nRow, 2, L"3");

	nRow = m_ctrlAllData.InsertItem(nRow + 1, _T("4"));
	m_ctrlAllData.SetItemText(nRow, 1, L"武汉四中");
	m_ctrlAllData.SetItemText(nRow, 2, L"3");

	nRow = m_ctrlAllData.InsertItem(nRow + 1, _T("5"));
	m_ctrlAllData.SetItemText(nRow, 1, L"武汉五中");
	m_ctrlAllData.SetItemText(nRow, 2, L"3");

	nRow = m_ctrlAllData.InsertItem(nRow + 1, _T("6"));
	m_ctrlAllData.SetItemText(nRow, 1, L"武汉六中");
	m_ctrlAllData.SetItemText(nRow, 2, L"3");
#endif

	m_ctrlAllData.SetRowHeigt(50);

	m_pProjDataInstance = CProjDataInstance::getInstance();
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


void CDlgImportProj::OnNMClickListProjData(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;

	m_agBtnDelete.EnableWindow(TRUE);
	m_agBtnDelete.SwitchButtonStatus(CAGButton::AGBTN_NORMAL);
	m_agBtnEdit.EnableWindow(TRUE);
	m_agBtnEdit.SwitchButtonStatus(CAGButton::AGBTN_NORMAL);
	m_agBtnNew.EnableWindow(TRUE);
	m_agBtnNew.SwitchButtonStatus(CAGButton::AGBTN_NORMAL);
	m_agBtnNew.Invalidate(TRUE);
	m_agBtnDelete.Invalidate(TRUE);
	m_agBtnEdit.Invalidate(TRUE);


}
