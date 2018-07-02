
// RandomNumTools-WindowsDlg.h : header file
//

#pragma once
#include "AGButton.h"
#include "AGEdit.h"
#include "AGComboBox.h"
#include "TTSWrapper.h"

class CProjDataInstance;
// CRandomNumToolsWindowsDlg dialog
class CRandomNumToolsWindowsDlg : public CDialogEx
{
// Construction
public:
	CRandomNumToolsWindowsDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_RANDOMNUMTOOLSWINDOWS_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg LRESULT OnNcHitTest(CPoint point);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	DECLARE_MESSAGE_MAP()

protected:
	inline void initCtrl();
	inline void uninitCtrl();

	inline void initTTS();
	inline void uninitTTS();
	inline void stop();

	inline void DrawClient(CDC *pDC);

	afx_msg void OnBnClickedBtnClose();
	afx_msg void OnBnClickedBtnMin();
	afx_msg void OnBnClickedButtonImportfile();
	afx_msg void OnBnClickedButtonFullscreen();
	afx_msg void OnBnClickedButtonConfig();
	afx_msg void OnBnClickedButtonAdd();
	afx_msg void OnBnClickedButtonDesignation();
	afx_msg void OnBnClickedButtonMute();
	afx_msg void OnBnClickedButtonStart();
	afx_msg void OnBnClickedButtonSure();

	afx_msg LRESULT OnImportFile(WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT OnDeleteFile(WPARAM wParam, LPARAM lParam);

	inline void playSoundBk();
	inline void playSoundHit();

	static UINT ThreadTTSProc(LPVOID param);

private:

	CFont		m_ftBtn;
	CFont       m_ftHead;
	CFont       m_ftTitle;
	CFont       m_ftTag;

	CAGButton	m_btnStart;
	CAGButton   m_btnSure;
	CAGButton   m_btnImport;
	CAGButton   m_btnFullScreen;
	CAGButton   m_btnConfig;
	CAGButton   m_btnAdd;
	CAGButton   m_btnMute;
	CAGButton   m_btnDesign;

	CAGEdit	m_editAddName;
	CAGEdit    m_editMuteName;
	CAGEdit    m_editDesignation;

	CAGButton  m_btnMin;
	CAGButton  m_btnClose;

	CString m_strVendorTitle;
	CString m_strVersionTag;
	CString m_strImprtProjName;

	CTTSWrapper m_TtsWrapper;
	BOOL m_bResume;
	CString m_StrHitName;
	CWinThread* m_pThreadTTS;
	CProjDataInstance* m_pProjInstance;
};
