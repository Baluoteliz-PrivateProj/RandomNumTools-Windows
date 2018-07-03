// DlgSpeeckTextRender.cpp : implementation file
//

#include "stdafx.h"
#include "RandomNumTools-Windows.h"
#include "afxdialogex.h"

#include "Utilc.h"
using namespace CPlusBaluoteli;
using namespace CPlusBaluoteli::control;
using namespace  CPlusBaluoteli::util;
#include "DlgSpeeckTextRender.h"

#include "ProjDataManager.h"


// CDlgSpeeckTextRender dialog

IMPLEMENT_DYNAMIC(CDlgSpeeckTextRender, CDialogEx)

CDlgSpeeckTextRender::CDlgSpeeckTextRender(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgSpeeckTextRender::IDD, pParent),
	m_bStartRandom(false),
	m_bChooseRandom(false),
	m_pProjInstance(nullptr)
{

}

CDlgSpeeckTextRender::~CDlgSpeeckTextRender()
{
	KillTimer(Event_Main_Start);
	KillTimer(Event_Main_Choose);
}

void CDlgSpeeckTextRender::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgSpeeckTextRender, CDialogEx)
	ON_WM_SHOWWINDOW()
	ON_WM_PAINT()
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CDlgSpeeckTextRender message handlers
BOOL CDlgSpeeckTextRender::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	
	m_fontRandom.CreateFont(84, 0, 0, 0, FW_NORMAL, FALSE, FALSE, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, _T("ËÎÌו"));
	SetBackgroundImage(IDB_BITMAP_MainTextRender);

	m_dwStartInterval = CommonFun::str2int(gConfig.getRandomInterval());
	if (0 == m_dwStartInterval)
		m_dwStartInterval = 50;

	gConfig.setRandomInterval(CommonFun::int2str(m_dwStartInterval));
	SetTimer(Event_Main_Start, m_dwStartInterval, NULL);
	SetTimer(Event_Main_Choose, 1000, NULL);

	m_pProjInstance = CProjDataInstance::getInstance();

	return TRUE;

}

void CDlgSpeeckTextRender::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialogEx::OnShowWindow(bShow, nStatus);

}

void CDlgSpeeckTextRender::OnPaint()
{
	//render
	CPaintDC dc(this);
	CAutoLock al(&m_lock);
	if (m_bStartRandom) {
		CRect rt;
		GetClientRect(&rt);

		CFont* defFont = dc.SelectObject(&m_fontRandom);
		dc.SetBkColor(RGB(172, 208, 233));
		dc.SetTextColor(RGB(255, 255, 255));
		int nCharLen = strlen(((CStringA)(m_strRandomName.GetBuffer())).GetBuffer());
		int nOffset = 22 * nCharLen;
		dc.SetBkMode(TRANSPARENT);
		dc.TextOut(rt.Width() / 2 - nOffset, rt.Height() / 2 - 50, m_strRandomName);
		dc.SelectObject(defFont);

		static int nCount = 0;
		nCount++;
		FormatStr::CFormatStr::Baluoteliz_OutDebugStr(L"StartRandom [%d] [%d]: %s\n", GetTickCount(), nCount, m_strRandomName);
	}

	if (m_bChooseRandom) {
		if (m_bStartRandom)
			Invalidate(TRUE);
		m_bStartRandom = false;
		CRect rt;
		GetClientRect(&rt);

		static int nCount1 = 0;
		nCount1++;
		CFont* defFont = dc.SelectObject(&m_fontRandom);
		dc.SetBkColor(RGB(172, 208, 233));
		if (nCount1 % 2)
			dc.SetTextColor(RGB(255, 255, 255));
		else
			dc.SetTextColor(RGB(255,0,0));
		int nCharLen = strlen(((CStringA)(m_strHitName.GetBuffer())).GetBuffer());
		int nOffset = 22 * nCharLen;
		dc.SetBkMode(TRANSPARENT);
		dc.TextOut(rt.Width() / 2 - nOffset, rt.Height() / 2 - 50, m_strHitName);
		dc.SelectObject(defFont);

		FormatStr::CFormatStr::Baluoteliz_OutDebugStr(L"ChooseRandom [%d] [%d]: %s\n", GetTickCount(), nCount1, m_strHitName);
	}
}

void CDlgSpeeckTextRender::OnTimer(UINT_PTR nIDEvent)
{
	if (Event_Main_Start == nIDEvent) {

		CAutoLock al(&m_lock);
		if (m_bStartRandom){
			if (m_pProjInstance && m_pProjInstance->RandomStrRepeatable(m_strRandomName))
				Invalidate(TRUE);
		}
	}

	if (Event_Main_Choose == nIDEvent) {
		CAutoLock al(&m_lock);
		if (m_bChooseRandom) {
			Invalidate(TRUE);
		}
	}
}

void CDlgSpeeckTextRender::btnRandom()
{
	CAutoLock al(&m_lock);
	m_bStartRandom = true;
	m_bChooseRandom = false;
}

void CDlgSpeeckTextRender::btnChoose(const CString &strChoose)
{
	CAutoLock al(&m_lock);
	m_bChooseRandom = true;
	//m_bStartRandom = false;
	m_strHitName = strChoose;
}

void CDlgSpeeckTextRender::updateStartInterval(DWORD dwInterval)
{
	if (dwInterval != m_dwStartInterval) {
		KillTimer(Event_Main_Start);
		SetTimer(Event_Main_Start, dwInterval, NULL);
		m_dwStartInterval = dwInterval;
	}
}