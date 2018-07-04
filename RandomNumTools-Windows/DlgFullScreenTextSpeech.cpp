// DlgFullScreenTextSpeech.cpp : implementation file
//

#include "stdafx.h"
#include "RandomNumTools-Windows.h"
#include "Utilc.h"
using namespace CPlusBaluoteli;
using namespace CPlusBaluoteli::control;
using namespace CPlusBaluoteli::util;
using namespace CPlusBaluoteli::FormatStr;
#include "DlgFullScreenTextSpeech.h"
#include "afxdialogex.h"

#include "ProjDataManager.h"


// CDlgFullScreenTextSpeech dialog

IMPLEMENT_DYNAMIC(CDlgFullScreenTextSpeech, CDialogEx)

CDlgFullScreenTextSpeech::CDlgFullScreenTextSpeech(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgFullScreenTextSpeech::IDD, pParent),
	m_bStartRandom(false),
	m_bChooseRandom(false),
	m_bShowListChoose(false),
	m_pProjInstance(nullptr)
{

}

CDlgFullScreenTextSpeech::~CDlgFullScreenTextSpeech()
{
}

void CDlgFullScreenTextSpeech::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgFullScreenTextSpeech, CDialogEx)
	ON_WM_SHOWWINDOW()
	ON_WM_PAINT()
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CDlgFullScreenTextSpeech message handlers
BOOL CDlgFullScreenTextSpeech::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_fontRandom.CreateFont(94, 0, 0, 0, FW_NORMAL, FALSE, FALSE, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, _T("宋体"));
	m_fontList.CreateFont(20, 0, 0, 0, FW_NORMAL, FALSE, FALSE, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, _T("Arial"));
	m_fontOperatorNote.CreateFont(40, 0, 0, 0, FW_BOLD, FALSE, FALSE, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, _T("Arial"));

	SetBackgroundImage(IDB_BITMAP_FULLSCREENRENDER_768);

	m_dwStartInterval = CommonFun::str2int(gConfig.getRandomInterval());
	if (0 == m_dwStartInterval)
		m_dwStartInterval = 50;

	gConfig.setRandomInterval(CommonFun::int2str(m_dwStartInterval));
	SetTimer(Event_FulllScreen_Start, m_dwStartInterval, NULL);
	SetTimer(Event_FullScreen_Choose, 1000, NULL);

	m_pProjInstance = CProjDataInstance::getInstance();

	return TRUE;
}

void CDlgFullScreenTextSpeech::OnShowWindow(BOOL bShow, UINT nStatus)
{
	int nWidth = GetSystemMetrics(SM_CXFULLSCREEN);
	int nHeight = GetSystemMetrics(SM_CYFULLSCREEN);
	if (1920 == nWidth)
		SetBackgroundImage(IDB_BITMAP_FullScreenRender);

	MoveWindow(0, 0, nWidth, nHeight, TRUE);
}

void CDlgFullScreenTextSpeech::OnPaint()
{
	//render
	CPaintDC dc(this);
	CAutoLock al(&m_lock);
	if (m_bStartRandom) {
		CRect rt;
		GetClientRect(&rt);

		CFont* defFont = dc.SelectObject(&m_fontRandom);
		dc.SetBkColor(RGB(56, 250, 231));
		dc.SetTextColor(RGB(255, 255, 255));
		int nCharLen = strlen(((CStringA)(m_strRandomName.GetBuffer())).GetBuffer());
		int nOffset = 24 * nCharLen; 
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
		dc.SetBkColor(RGB(56, 250, 231));
		if (nCount1 % 2)
			dc.SetTextColor(RGB(255, 255, 255));
		else
			dc.SetTextColor(RGB(255, 0, 0));
		int nCharLen = strlen(((CStringA)(m_strHitName.GetBuffer())).GetBuffer());
		int nOffset = 24 * nCharLen;
		dc.SetBkMode(TRANSPARENT);
		dc.TextOut(rt.Width() / 2 - nOffset, rt.Height() / 2 - 50, m_strHitName);
		dc.SelectObject(defFont);

		FormatStr::CFormatStr::Baluoteliz_OutDebugStr(L"ChooseRandom [%d] [%d]: %s\n", GetTickCount(), nCount1, m_strHitName);
	}

	if (m_bShowListChoose || true) {

		int nWidth = GetSystemMetrics(SM_CXFULLSCREEN);
		int nHeight = GetSystemMetrics(SM_CYFULLSCREEN);

		int nYPosRangeMin = 200;
		int nYPosRangeMax = nHeight - 200;

		int nXPos = 20;
		int nYPos = nYPosRangeMax;
		int nCountVec = m_vecChoose.size();
		for (int nIndex = nCountVec - 1; 0 <= nIndex; nIndex--) {

			nYPos -= 30;
			if (nYPos < nYPosRangeMin)
				break;
			CFont* defFont = dc.SelectObject(&m_fontList);
			dc.SetBkColor(RGB(56, 250, 231));
			dc.SetTextColor(RGB(255, 255, 255));
			dc.SetBkMode(TRANSPARENT);
			CString strText;
			strText.GetBuffer(MAX_PATH);
			strText.Format(L"[%4d] %s  %s", m_vecChoose[nIndex].nIndex, m_vecChoose[nIndex].strData, m_vecChoose[nIndex].strResult);
			dc.TextOut(nXPos, nYPos, strText);
			dc.SelectObject(defFont);
		}
		m_bShowListChoose = false;

		CFont* defFont = dc.SelectObject(&m_fontOperatorNote);
		dc.SetBkColor(RGB(56, 250, 231));
		dc.SetTextColor(RGB(179, 138, 82));
		dc.SetBkMode(TRANSPARENT);
		dc.TextOut(nWidth / 2 - 250, nHeight - 50, KOperatorNote);
		dc.SelectObject(defFont);
	}
}

void CDlgFullScreenTextSpeech::OnTimer(UINT_PTR nIDEvent)
{
	if (Event_FulllScreen_Start == nIDEvent) {

		CAutoLock al(&m_lock);
		if (m_bStartRandom){
			if (m_pProjInstance && m_pProjInstance->RandomStrRepeatable(m_strRandomName))
				Invalidate(TRUE);
		}
	}

	if (Event_FullScreen_Choose == nIDEvent) {
		CAutoLock al(&m_lock);
		if (m_bChooseRandom) {
			Invalidate(TRUE);
		}
	}
}

BOOL CDlgFullScreenTextSpeech::PreTranslateMessage(MSG* pMsg)
{
	if (m_hWnd == pMsg->hwnd && VK_SPACE == pMsg->wParam && WM_KEYDOWN == pMsg->message) {
		CAutoLock al(&m_lock);
		if (!m_bChooseRandom && !m_bStartRandom) {
			m_bStartRandom = true;
			SoundControl::playSoundBk();
		}
		else if (m_bStartRandom && !m_bChooseRandom) {
			m_bChooseRandom = true;
			if (m_pProjInstance) {
				if (!m_pProjInstance->RandomStr(m_strHitName)) {

					m_strHitName = L"";
					CFormatStr::Baluoteliz_MessageBox(L"项目数据为空，或者 所有的数据都已经随机出现过了");
				}
				else{
					LPRANDOM_UPDATE_CANDIDATELILST lpData = new RANDOM_UPDATE_CANDIDATELILST;
					if (lpData) {
						lpData->strRandom = m_strHitName;
						::PostMessage(theApp.GetMainWnd()->m_hWnd, RandomMsg_UpdateRandom_CandidateList, WPARAM(lpData), NULL);
					}

					Random_FULLSCREEN_SHOWRESULT ShowListTemp;
					ShowListTemp.nIndex = m_vecChoose.size() + 1;
					ShowListTemp.strResult = m_strHitName;
					ShowListTemp.strData = CommonFun::getTimeStr();
					m_vecChoose.push_back(ShowListTemp);
				}
			}
			LPRANDOM_FULLSCREEN_SPEECH lpData = new RANDOM_FULLSCREEN_SPEECH;
			m_bShowListChoose = true;
			lpData->strSpeechText = m_strHitName;
			SoundControl::playSoundHit();
			::SendMessage(theApp.GetMainWnd()->m_hWnd, RandomMsg_FULLSCREEN_SPEECH, (WPARAM)lpData, NULL);
		}
		else if (!m_bStartRandom && m_bChooseRandom) {
			m_bStartRandom = true;
			m_bChooseRandom = false;
			SoundControl::playSoundBk();
		}
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}

void CDlgFullScreenTextSpeech::OnClose()
{
	KillTimer(Event_FulllScreen_Start);
	KillTimer(Event_FullScreen_Choose);

	CDialogEx::OnCancel();
}

void CDlgFullScreenTextSpeech::updateStartInterval(DWORD dwInterval)
{
	if (m_dwStartInterval != dwInterval) {

		KillTimer(Event_FulllScreen_Start);
		SetTimer(Event_FulllScreen_Start, dwInterval,NULL);
		m_dwStartInterval = dwInterval;
	}
}