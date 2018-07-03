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
	afx_msg void OnPaint();
	DECLARE_MESSAGE_MAP()

protected:
	inline void initCtrl();
	inline void uninitCtrl();

private:

	CFont       m_ftTag;

	CAGButton m_agImportDir;//imprt
	CAGButton m_agBtnNew;
	CAGButton m_agBtnEdit;
	CAGButton m_agBtnDelete;
	CButton m_btnImport;//sure

	CAGListCtrl m_ctrlAllData;

	CProjDataInstance* m_pProjDataInstance;
	CString m_strSelectItemText;

	int m_nSelectItem;
public:
	afx_msg void OnBnClickedButtonNew();
	afx_msg void OnBnClickedButtonEdit();
	afx_msg void OnBnClickedButtonDelete();
	afx_msg void OnBnClickedButtonSure();
	afx_msg void OnBnClickedButtonCancel();
	afx_msg void OnNMDBlickListProjData(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg LRESULT OnUpdataListCtrl(WPARAM wParam, LPARAM lParam);
	afx_msg void OnBnClickedButtonImportproj();
};
