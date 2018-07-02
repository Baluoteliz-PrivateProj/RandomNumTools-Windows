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

CDlgImportEdit::CDlgImportEdit(CString strName,CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgImportEdit::IDD, pParent),
	m_strProjName(strName)
{

}

CDlgImportEdit::~CDlgImportEdit()
{
}

void CDlgImportEdit::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_MAIN, m_AgLinkMain);
	DDX_Control(pDX, IDC_STATIC_Mute, m_AgLinkMute);
	DDX_Control(pDX, IDC_STATIC_Designation, m_AgDesignation);
}

BEGIN_MESSAGE_MAP(CDlgImportEdit, CDialogEx)
	ON_WM_PAINT()
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
	m_fontTitle.CreateFont(25, 0, 0, 0, FW_BOLD, FALSE, FALSE, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, _T("Arial"));
	m_fontFile.CreateFont(20, 0, 0, 0, FW_BOLD, FALSE, FALSE, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, _T("Arial"));

	CRect rt;
	GetClientRect(&rt);

	std::vector<CString> vecTemp;
	m_pProjInstance = CProjDataInstance::getInstance();
	m_pProjInstance->getVectorFiles(m_strProjName, vecTemp);
	int nCount = vecTemp.size();

	if (nCount) {

		m_AgLinkMain.MoveWindow(rt.Width() / 2 - 100, rt.top + 100, 200, 30, TRUE);
		m_AgLinkMain.SetWindowTextW(vecTemp[0]);
		m_AgLinkMute.MoveWindow(rt.Width() / 2 - 100, rt.top + 150, 200, 30, TRUE);
		m_AgLinkMute.SetWindowTextW(vecTemp[1]);
		m_AgDesignation.MoveWindow(rt.Width() / 2 - 100, rt.top + 200, 200, 30, TRUE);
		m_AgDesignation.SetWindowTextW(vecTemp[2]);

		m_AgLinkMute.SetBackColor(RGB(240, 240, 240));
		m_AgLinkMain.SetBackColor(RGB(240, 240, 240));
		m_AgDesignation.SetBackColor(RGB(240, 240, 240));

		m_AgLinkMain.SetFont(&m_fontFile);
		m_AgLinkMute.SetFont(&m_fontFile);
		m_AgDesignation.SetFont(&m_fontFile);

		CString strPrefixPath = m_pProjInstance->getPrefixPath();
		CString strFormat;
		strFormat.Format(_T("%s%s\\%s"), strPrefixPath, m_strProjName, vecTemp[0]);
		m_AgLinkMain.SetCmd(strFormat);
		strFormat.Format(_T("%s%s\\%s"), strPrefixPath, m_strProjName, vecTemp[1]);
		m_AgLinkMute.SetCmd(strFormat);
		strFormat.Format(_T("%s%s\\%s"), strPrefixPath, m_strProjName, vecTemp[2]);
		m_AgDesignation.SetCmd(strFormat);
	}
}

inline void CDlgImportEdit::uninitCtrl()
{

}

void CDlgImportEdit::OnPaint()
{
	CPaintDC dc(this);
	
	CRect rtClient;
	GetClientRect(&rtClient);
	CFont* defFont = dc.SelectObject(&m_fontTitle);

	dc.FillSolidRect(rtClient.left + 50, 45, 200, 24, RGB(240, 240, 240));
	dc.SetBkColor(RGB(240, 240, 240));
	dc.SetTextColor(RGB(128, 128, 225));
	dc.TextOut(rtClient.left +  50, 45, m_strProjName, _tcslen(m_strProjName));

	dc.SelectObject(defFont);
}