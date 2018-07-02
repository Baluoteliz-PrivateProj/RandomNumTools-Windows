#pragma once


// CDlgImportNew dialog

class CProjDataInstance;
class CDlgImportNew : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgImportNew)

public:
	CDlgImportNew(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgImportNew();

// Dialog Data
	enum { IDD = IDD_DIALOG_PROJ_NEW };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	virtual BOOL OnInitDialog();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	DECLARE_MESSAGE_MAP()

protected:
	inline void initCtrl();
	inline void uninitCtrl();

private:

	CProjDataInstance* m_pProjData;
	CEdit m_EditNewProj;
public:
	afx_msg void OnBnClickedButtonOk();
	afx_msg void OnBnClickedButtonCancel();
};
