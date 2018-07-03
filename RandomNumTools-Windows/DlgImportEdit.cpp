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
	DDX_Control(pDX, IDC_STATIC_EDIT_RROJNAME, m_staticProj);
	DDX_Control(pDX, IDC_STATIC_EDIT_RANDOMFILE, m_staticMain);
	DDX_Control(pDX, IDC_STATIC_EDIT_MUTEFILE, m_staticMute);
	DDX_Control(pDX, IDC_STATIC_EDIT_RANDOM_HITFILE, m_staticDesigation);
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

	int nMainIndex = 0; int nDesgnationIndex = 0; int nMuteIndex = 0;
	vecWStrIt itVec = find(vecTemp.begin(), vecTemp.end(), L"mute.dat");
	if (vecTemp.end() != itVec)
		nMuteIndex = itVec - vecTemp.begin();
	itVec = find(vecTemp.begin(), vecTemp.end(), L"designation.dat");
	if (vecTemp.end() != itVec)
		nDesgnationIndex = itVec - vecTemp.begin();
	itVec = find(vecTemp.begin(), vecTemp.end(), m_strProjName + L".dat");
	if (vecTemp.end() != itVec)
		nMainIndex = itVec - vecTemp.begin();

	if (nCount) {

		m_staticProj.ShowWindow(SW_HIDE);
		m_staticMain.MoveWindow(rt.Width() / 2 - 200, rt.top + 100, 100, 30, TRUE);
		m_AgLinkMain.MoveWindow(rt.Width() / 2 - 100, rt.top + 100, 200, 30, TRUE);
		CString strName = vecTemp[nMainIndex];
		strName = strName.Mid(0, strName.ReverseFind(L'.'));
		m_AgLinkMain.SetWindowTextW(strName);

		m_staticDesigation.MoveWindow(rt.Width() / 2 - 200, rt.top + 200, 100, 30, TRUE);
		m_AgDesignation.MoveWindow(rt.Width() / 2 - 100, rt.top + 200, 200, 30, TRUE);
		strName = vecTemp[nDesgnationIndex];
		strName = strName.Mid(0, strName.ReverseFind(L'.'));
		m_AgDesignation.SetWindowTextW(strName);

		m_staticMute.MoveWindow(rt.Width() / 2 - 200, rt.top + 150, 100, 30, TRUE);
		m_AgLinkMute.MoveWindow(rt.Width() / 2 - 100, rt.top + 150, 200, 30, TRUE);
		strName = vecTemp[nMuteIndex];
		strName = strName.Mid(0, strName.ReverseFind(L'.'));
		m_AgLinkMute.SetWindowTextW(strName);

		m_AgLinkMute.SetBackColor(RGB(240, 240, 240));
		m_AgLinkMain.SetBackColor(RGB(240, 240, 240));
		m_AgDesignation.SetBackColor(RGB(240, 240, 240));

		m_AgLinkMain.SetFont(&m_fontFile);
		m_AgLinkMute.SetFont(&m_fontFile);
		m_AgDesignation.SetFont(&m_fontFile);

		CString strPrefixPath = m_pProjInstance->getPrefixPath();
		CString strFormat;
		strFormat.Format(_T("%s%s\\%s"), strPrefixPath, m_strProjName, vecTemp[nMainIndex]);
		m_AgLinkMain.SetCmd(strFormat);
		strFormat.Format(_T("%s%s\\%s"), strPrefixPath, m_strProjName, vecTemp[nDesgnationIndex]);
		m_AgDesignation.SetCmd(strFormat);
		strFormat.Format(_T("%s%s\\%s"), strPrefixPath, m_strProjName, vecTemp[nMuteIndex]);
		m_AgLinkMute.SetCmd(strFormat);

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

	CString strTemp;
	strTemp.Format(L"ÏîÄ¿Ãû³Æ: %s", m_strProjName);
	dc.FillSolidRect(rtClient.left + 30, 45, 200, 24, RGB(240, 240, 240));
	dc.SetBkColor(RGB(240, 240, 240));
	dc.SetTextColor(RGB(128, 128, 225));
	dc.TextOut(rtClient.left + 30, 45, strTemp, _tcslen(strTemp));

	dc.SelectObject(defFont);
}