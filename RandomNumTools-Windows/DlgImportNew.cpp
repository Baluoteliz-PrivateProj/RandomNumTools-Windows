// DlgImportNew.cpp : implementation file
//

#include "stdafx.h"
#include "RandomNumTools-Windows.h"
#include "DlgImportNew.h"
#include "afxdialogex.h"


// CDlgImportNew dialog

IMPLEMENT_DYNAMIC(CDlgImportNew, CDialogEx)

CDlgImportNew::CDlgImportNew(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgImportNew::IDD, pParent)
{

}

CDlgImportNew::~CDlgImportNew()
{
}

void CDlgImportNew::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgImportNew, CDialogEx)
END_MESSAGE_MAP()


// CDlgImportNew message handlers
BOOL CDlgImportNew::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	
	initCtrl();

	return TRUE;
}

void CDlgImportNew::OnShowWindow(BOOL bShow, UINT nStatus)
{

}

inline void CDlgImportNew::initCtrl()
{

}

inline void CDlgImportNew::uninitCtrl()
{

}