#pragma once


// CDlgImportEdit dialog

class CDlgImportEdit : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgImportEdit)

public:
	CDlgImportEdit(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgImportEdit();

// Dialog Data
	enum { IDD = IDD_DIALOG_PROJ_EDIT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	virtual BOOL OnInitDialog();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	DECLARE_MESSAGE_MAP()

protected:
	inline void initCtrl();
	inline void uninitCtrl();
};
