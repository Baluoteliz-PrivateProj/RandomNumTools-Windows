// DlgImportProj.cpp : implementation file
//

#include "stdafx.h"
#include "RandomNumTools-Windows.h"
#include "DlgImportProj.h"
#include "afxdialogex.h"
#include "Utilc.h"
using namespace CPlusBaluoteli;
using namespace CPlusBaluoteli::control;
using namespace CPlusBaluoteli::util;

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
	DDX_Control(pDX, IDC_BUTTON_SURE, m_btnImport);
}


BEGIN_MESSAGE_MAP(CDlgImportProj, CDialogEx)
	ON_WM_NCHITTEST()
	ON_WM_SHOWWINDOW()
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_BUTTON_NEW, &CDlgImportProj::OnBnClickedButtonNew)
	ON_BN_CLICKED(IDC_BUTTON_EDIT, &CDlgImportProj::OnBnClickedButtonEdit)
	ON_BN_CLICKED(IDC_BUTTON_DELETE, &CDlgImportProj::OnBnClickedButtonDelete)
	ON_BN_CLICKED(IDC_BUTTON_SURE, &CDlgImportProj::OnBnClickedButtonSure)
	ON_BN_CLICKED(IDC_BUTTON_CANCEL, &CDlgImportProj::OnBnClickedButtonCancel)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_PROJ_DATA, &CDlgImportProj::OnNMDBlickListProjData)
	ON_MESSAGE(RandomMsg_UPDATA_LISTCTRL, &CDlgImportProj::OnUpdataListCtrl)
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

void CDlgImportProj::OnPaint()
{
	CPaintDC dc(this);

	CRect rtClient;
	GetClientRect(&rtClient);
	CFont* defFont = dc.SelectObject(&m_ftTag);

	dc.FillSolidRect(rtClient.right / 2-80 , 45, 200, 24, RGB(255, 255, 255));
	dc.SetBkColor(RGB(255, 255, 255));
	dc.SetTextColor(RGB(128, 128, 225));
	CString strSelect;
	if (_T("") != m_strSelectItemText)
		strSelect = L"选中项目名称: " + m_strSelectItemText;
	else
		strSelect = KListCtrlItemText;
	dc.TextOut(rtClient.Width() / 2 - 80, 45, strSelect, _tcslen(strSelect));

	dc.SelectObject(defFont);
}

inline void CDlgImportProj::initCtrl()
{
	m_ftTag.CreateFont(16, 0, 0, 0, FW_BOLD, FALSE, FALSE, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, _T("Arial"));

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
	m_ctrlAllData.InsertColumn(2, KCtrlItemCount, LVCFMT_CENTER, 150);
	
	m_agBtnDelete.EnableWindow(FALSE);
	m_agBtnDelete.SwitchButtonStatus(CAGButton::AGBTN_DISABLE);
	m_agBtnEdit.EnableWindow(FALSE);
	m_agBtnEdit.SwitchButtonStatus(CAGButton::AGBTN_DISABLE);
	m_btnImport.EnableWindow(FALSE);

	m_ctrlAllData.SetRowHeigt(50);

	m_pProjDataInstance = CProjDataInstance::getInstance();
	mapWStrVec mapData;
	m_pProjDataInstance->getProjData(mapData);
	int nRow = 0;
	for (mapWStrVecIt itProj = mapData.begin(); mapData.end() != itProj; itProj++) {
		nRow = m_ctrlAllData.InsertItem(nRow, CommonFun::int2CS(nRow + 1));
		m_ctrlAllData.SetItemText(nRow, 1, itProj->first);
		int nSubCount = itProj->second.size();
		m_ctrlAllData.SetItemText(nRow, 2, CommonFun::int2CS(nSubCount));
		nRow++;
	}
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
	CDlgImportEdit dlgImportEdit(m_strSelectItemText,this);
	INT_PTR nResponse = dlgImportEdit.DoModal();
	if (IDOK == nResponse) {

	}
	else if (IDCANCEL == nResponse) {

	}
}

void CDlgImportProj::OnBnClickedButtonDelete()
{
	// TODO: Add your control notification handler code here
	if (m_pProjDataInstance)
		m_pProjDataInstance->deleteItem(m_strSelectItemText);

	m_ctrlAllData.DeleteItem(m_nSelectItem);

	int nCount = m_ctrlAllData.GetItemCount();
	for (int nIndex = 0; nIndex < nCount; nIndex++) {
		m_ctrlAllData.SetItemText(nIndex, 0, CommonFun::int2CS(nIndex + 1));
	}

	m_btnImport.EnableWindow(FALSE);
	m_agBtnDelete.EnableWindow(FALSE);
	m_agBtnEdit.EnableWindow(FALSE);

	m_strSelectItemText = L"";
	Invalidate(TRUE);
}

void CDlgImportProj::OnBnClickedButtonSure()
{
	// TODO: Add your control notification handler code here
	if (L"" != m_strSelectItemText) {

		if (m_pProjDataInstance) {

			CString strErrorDesc;
			strErrorDesc.GetBuffer(MAX_PATH);
			int nRes = m_pProjDataInstance->ImportProj(m_strSelectItemText);
			if (nRes & CFileData::error_AllDataItemMuted)				
				strErrorDesc.Format(L"%s\r\n%s", strErrorDesc,KERROR_AllDataItemMuted);
			if (nRes & CFileData::error_InValidItem_InSpecialFiles)
				strErrorDesc.Format(L"%s\r\n%s", strErrorDesc, KERROR_InSpecialFiles);
			if (nRes & CFileData::error_SameItemConflict_MuteAndSpecial) 
				strErrorDesc.Format(L"%s\r\n%s", strErrorDesc, KERROR_SameItemConflict_MuteAndSpecial);
			if (nRes & CFileData::error_Random_NoValidProj)
				strErrorDesc.Format(_T("%s\r\n%s"), strErrorDesc, KERROR_NoValidProj);
			
			if (!strErrorDesc.IsEmpty()) {

				FormatStr::CFormatStr::Baluoteliz_MessageBox(_T("%s\r\n\r\n \t导入失败,请编辑修改!!!!!"),strErrorDesc);
				return;
			}
			else {
				LPRANDOM_IMPORT_PROJ lpData = new RANDOM_IMPORT_PROJ;
				if (lpData) {

					lpData->nRes = nRes;
					lpData->strProjName = m_strSelectItemText;
					::PostMessage(theApp.GetMainWnd()->m_hWnd, RandomMsg_IMPORT_PROJ, WPARAM(lpData), NULL);
				}
			}

			strErrorDesc.ReleaseBuffer(MAX_PATH);
		}
	}

	CDialogEx::OnOK();
}


void CDlgImportProj::OnBnClickedButtonCancel()
{
	// TODO: Add your control notification handler code here

	CDialogEx::OnCancel();
}

void CDlgImportProj::OnNMDBlickListProjData(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;

	NM_LISTVIEW* pListView = (NM_LISTVIEW*)pNMHDR;
	m_nSelectItem = pListView->iItem;
	m_strSelectItemText = m_ctrlAllData.GetItemText(m_nSelectItem, 1);
	
	m_agBtnDelete.EnableWindow(TRUE);
	m_agBtnDelete.SwitchButtonStatus(CAGButton::AGBTN_NORMAL);
	m_agBtnEdit.EnableWindow(TRUE);
	m_agBtnEdit.SwitchButtonStatus(CAGButton::AGBTN_NORMAL);
	m_agBtnNew.EnableWindow(TRUE);
	m_agBtnNew.SwitchButtonStatus(CAGButton::AGBTN_NORMAL);
	m_agBtnNew.Invalidate(TRUE);
	m_agBtnDelete.Invalidate(TRUE);
	m_agBtnEdit.Invalidate(TRUE);
	m_btnImport.EnableWindow(TRUE);

	Invalidate(TRUE);
}

LRESULT CDlgImportProj::OnUpdataListCtrl(WPARAM wParam, LPARAM lParam)
{
	LPRANDOM_NEW_PROJ lpData = (LPRANDOM_NEW_PROJ)wParam;
	if (lpData) {
		
		if (m_pProjDataInstance)
			m_pProjDataInstance->insertProjDataItem(lpData);
		int nRow = m_ctrlAllData.GetItemCount();
		m_ctrlAllData.InsertItem(nRow, CommonFun::int2CS(nRow + 1));
		m_ctrlAllData.SetItemText(nRow, 1, lpData->strProjName);
		m_ctrlAllData.SetItemText(nRow, 2, CommonFun::int2CS(lpData->nGroupCount));
		//m_ctrlAllData.SetItemState(nRow, LVIS_FOCUSED | LVIS_SELECTED, LVIS_SELECTED | LVIS_FOCUSED);
		m_ctrlAllData.EnsureVisible(nRow, true);

		NMITEMACTIVATE NMItemAct;
		NMItemAct.iItem = nRow;
		NMItemAct.iSubItem = 0;

		NMHDR hdr;
		hdr.hwndFrom = m_ctrlAllData.m_hWnd;
		hdr.idFrom = IDC_LIST_PROJ_DATA;
		hdr.code = NM_CLICK;

		NMItemAct.hdr = hdr;

		//::PostMessage(m_ctrlAllData.m_hWnd, WM_NOTIFY, NM_CLICK, (LPARAM)(&NMItemAct.hdr));

		m_ctrlAllData.Invalidate(TRUE);

		delete lpData; 
		lpData = NULL;
	}

	return TRUE;
}