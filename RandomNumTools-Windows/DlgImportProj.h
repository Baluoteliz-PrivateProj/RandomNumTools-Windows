#pragma once

#include "AGButton.h"
// CDlgImportProj dialog

#include "AGListCtrl.h"
class CProjDataInstance;

class CDlgImportProj : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgImportProj)

public:
	CDlgImportProj(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgImportProj();

// Dialog Data
	enum { IDD = IDD_DIALOG_IMPORTPROJ };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	virtual BOOL OnInitDialog();
	afx_msg LRESULT OnNcHitTest(CPoint point);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	DECLARE_MESSAGE_MAP()

protected:
	inline void initCtrl();
	inline void uninitCtrl();

private:

	CAGButton m_agBtnNew;
	CAGButton m_agBtnEdit;
	CAGButton m_agBtnDelete;

	CAGListCtrl m_ctrlAllData;

	CProjDataInstance* m_pProjDataInstance;

public:
	afx_msg void OnBnClickedButtonNew();
	afx_msg void OnBnClickedButtonEdit();
	afx_msg void OnBnClickedButtonDelete();
	afx_msg void OnBnClickedButtonSure();
	afx_msg void OnBnClickedButtonCancel();
	afx_msg void OnNMClickListProjData(NMHDR *pNMHDR, LRESULT *pResult);
};
