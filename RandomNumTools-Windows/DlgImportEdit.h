#pragma once

#include "AGHyperLink.h"
// CDlgImportEdit dialog

class CProjDataInstance;
class CDlgImportEdit : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgImportEdit)

public:
	CDlgImportEdit(CString strName,CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgImportEdit();

// Dialog Data
	enum { IDD = IDD_DIALOG_PROJ_EDIT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	DECLARE_MESSAGE_MAP()

protected:
	inline void initCtrl();
	inline void uninitCtrl();

private:
	
	CFont m_fontTitle;
	CFont m_fontFile;
	CAGHyperLink m_AgLinkMain;
	CAGHyperLink m_AgLinkMute;
	CAGHyperLink m_AgDesignation;

	CStatic m_staticProj;
	CStatic m_staticDesigation;
	CStatic m_staticMute;
	CStatic m_staticMain;

	CString m_strProjName;
	CProjDataInstance* m_pProjInstance;
};
