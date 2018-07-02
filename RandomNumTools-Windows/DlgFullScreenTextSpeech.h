#pragma once

class CProjDataInstance;

// CDlgFullScreenTextSpeech dialog

#include <list>
class CDlgFullScreenTextSpeech : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgFullScreenTextSpeech)

public:
	CDlgFullScreenTextSpeech(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgFullScreenTextSpeech();

// Dialog Data
	enum { IDD = IDD_DIALOG_FullScreen_TextRender };

protected:
	virtual BOOL OnInitDialog();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnPaint();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnClose();

	DECLARE_MESSAGE_MAP()

private:

	bool m_bStartRandom;
	bool m_bChooseRandom;
	bool m_bShowListChoose;

	CFont m_fontRandom;
	CFont m_fontList;
	CFont m_fontOperatorNote;

	DWORD m_dwStartInterval;
	DWORD m_dwchooseInterval;

	CProjDataInstance* m_pProjInstance;
	CString m_strHitName;
	CString m_strRandomName;
	CLock m_lock;

	std::vector<Random_FULLSCREEN_SHOWRESULT> m_vecChoose;
	typedef std::vector<Random_FULLSCREEN_SHOWRESULT>::iterator vecWstrIt;
};
