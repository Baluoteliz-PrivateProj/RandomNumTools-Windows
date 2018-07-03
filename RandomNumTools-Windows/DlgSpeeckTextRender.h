#pragma once


// CDlgSpeeckTextRender dialog
class CProjDataInstance;
class CDlgSpeeckTextRender : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgSpeeckTextRender)

public:
	CDlgSpeeckTextRender(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgSpeeckTextRender();

	void btnRandom();
	void btnChoose(const CString &strChoose);	
	void updateStartInterval(DWORD dwInterval);

// Dialog Data
	enum { IDD = IDD_DIALOG_SPEECHTEXT_RENDER };

protected:
	virtual BOOL OnInitDialog();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnPaint();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

private:
	bool m_bStartRandom;
	bool m_bChooseRandom;

	CFont m_fontRandom;

	DWORD m_dwStartInterval;
	DWORD m_dwchooseInterval;

	CProjDataInstance* m_pProjInstance;
	CString m_strHitName;
	CString m_strRandomName;
	CLock m_lock;
};
