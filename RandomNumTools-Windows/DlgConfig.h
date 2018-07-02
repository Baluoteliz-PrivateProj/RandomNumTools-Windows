#pragma once


// CDlgConfig dialog

class CDlgConfig : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgConfig)

public:
	CDlgConfig(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgConfig();

// Dialog Data
	enum { IDD = IDD_DIALOG_CONFIG };

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
	CEdit m_editVendorName;
	CSliderCtrl m_sliderCtrl;

public:
	afx_msg void OnBnClickedButtonSave();
	afx_msg void OnBnClickedButtonCancle();
	afx_msg void OnNMCustomdrawSliderRandominterval(NMHDR *pNMHDR, LRESULT *pResult);
};
