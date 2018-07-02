// DlgImportNew.cpp : implementation file
//

#include "stdafx.h"
#include "RandomNumTools-Windows.h"
#include "DlgImportNew.h"
#include "afxdialogex.h"

#include "Utilc.h"
using namespace CPlusBaluoteli;
using namespace CPlusBaluoteli::control;
using namespace CPlusBaluoteli::util;
#include "ProjDataManager.h"

// CDlgImportNew dialog

IMPLEMENT_DYNAMIC(CDlgImportNew, CDialogEx)

CDlgImportNew::CDlgImportNew(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgImportNew::IDD, pParent),
	m_pProjData(NULL)
{

}

CDlgImportNew::~CDlgImportNew()
{
}

void CDlgImportNew::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_PROJNAME, m_EditNewProj);
}


BEGIN_MESSAGE_MAP(CDlgImportNew, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_OK, &CDlgImportNew::OnBnClickedButtonOk)
	ON_BN_CLICKED(IDC_BUTTON_CANCEL, &CDlgImportNew::OnBnClickedButtonCancel)
END_MESSAGE_MAP()


// CDlgImportNew message handlers
BOOL CDlgImportNew::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	
	initCtrl();

	return TRUE;
}

void CDlgImportNew::OnShowWindow(BOOL bShow, UINT nStatus)
{

}

inline void CDlgImportNew::initCtrl()
{
	m_pProjData = CProjDataInstance::getInstance();
	assert(m_pProjData);
}

inline void CDlgImportNew::uninitCtrl()
{

}

void CDlgImportNew::OnBnClickedButtonOk()
{
	// TODO: Add your control notification handler code here
	CString strParam;
	m_EditNewProj.GetWindowTextW(strParam);
	if (_T("") != strParam) {

		CString sDir;
		sDir.Format(_T("..\\data\\%s\\"), strParam.GetBuffer());
		BOOL bres = CreateDirectory(sDir, NULL);

		if (bres) {

			LPRANDOM_NEW_PROJ lpData = new RANDOM_NEW_PROJ;
			lpData->strProjName = strParam;
			lpData->dataTpye = eType_Group_NO;
			lpData->nGroupCount = 3;

			char szbuf[MAX_PATH] = { '\0' };
			std::string strNewProjName = CommonFun::cs2s(strParam);
			lpData->m_vecFileName.push_back(strParam + _T(".dat"));
			sprintf_s(szbuf, "..\\data\\%s\\%s.dat", strNewProjName.data(), strNewProjName.data());
			FILE *pFile = fopen(szbuf,"ab+");
			fclose(pFile);
			ZeroMemory(szbuf, MAX_PATH);
			lpData->m_vecFileName.push_back(_T("mute.dat"));
			sprintf_s(szbuf, "..\\data\\%s\\%s.dat", strNewProjName.data(), "mute");
			pFile = fopen(szbuf,"ab+");
			fclose(pFile);
			ZeroMemory(szbuf, MAX_PATH);
			lpData->m_vecFileName.push_back(_T("designation.dat"));
			sprintf_s(szbuf, "..\\data\\%s\\%s.dat", strNewProjName.data(), "designation");
			pFile = fopen(szbuf, "ab+");
			fclose(pFile);

			::PostMessage(GetParent()->m_hWnd, RandomMsg_UPDATA_LISTCTRL, (WPARAM)lpData,NULL);
		}
	}

	CDialogEx::OnOK();
}


void CDlgImportNew::OnBnClickedButtonCancel()
{
	// TODO: Add your control notification handler code here

	CDialogEx::OnCancel();
}
