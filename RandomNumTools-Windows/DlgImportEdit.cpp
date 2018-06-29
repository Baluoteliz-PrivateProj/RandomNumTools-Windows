// DlgImportEdit.cpp : implementation file
//

#include "stdafx.h"
#include "RandomNumTools-Windows.h"
#include "DlgImportEdit.h"
#include "afxdialogex.h"

#include "Utilc.h"
using namespace CPlusBaluoteli;
using namespace CPlusBaluoteli::control;
#include "ProjDataManager.h"

// CDlgImportEdit dialog

IMPLEMENT_DYNAMIC(CDlgImportEdit, CDialogEx)

CDlgImportEdit::CDlgImportEdit(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgImportEdit::IDD, pParent)
{

}

CDlgImportEdit::~CDlgImportEdit()
{
}

void CDlgImportEdit::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgImportEdit, CDialogEx)
END_MESSAGE_MAP()


// CDlgImportEdit message handlers
BOOL CDlgImportEdit::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	initCtrl();

	return TRUE;
}

void CDlgImportEdit::OnShowWindow(BOOL bShow, UINT nStatus)
{

}

inline void CDlgImportEdit::initCtrl()
{

}

inline void CDlgImportEdit::uninitCtrl()
{

}