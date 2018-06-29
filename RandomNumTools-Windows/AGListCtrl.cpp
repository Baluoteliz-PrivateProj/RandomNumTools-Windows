#include "stdafx.h"
#include "AGListCtrl.h"

BEGIN_MESSAGE_MAP(CAGListCtrl, CHeaderCtrl)
	//{{AFX_MSG_MAP(CAGListCtrl)
	// NOTE - the ClassWizard will add and remove mapping macros here.
	ON_WM_MEASUREITEM_REFLECT()
	ON_WM_MEASUREITEM()
	ON_WM_DRAWITEM()
	//}}AFX_MSG_MAP	
END_MESSAGE_MAP()

CAGListCtrl::CAGListCtrl(void)
{
}


CAGListCtrl::~CAGListCtrl(void)
{
}

void CAGListCtrl::MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
	if (m_nRowHeight > 0)
	{
		lpMeasureItemStruct->itemHeight = m_nRowHeight;
	}
}

void CAGListCtrl::OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
	CListCtrl::OnMeasureItem(nIDCtl, lpMeasureItemStruct);
}

void CAGListCtrl::SetRowHeigt(int nHeight)
{
	m_nRowHeight = nHeight;
	CRect rcWin;
	GetWindowRect(&rcWin);
	WINDOWPOS wp;
	wp.hwnd = m_hWnd;
	wp.cx = rcWin.Width();
	wp.cy = rcWin.Height();
	wp.flags = SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOOWNERZORDER | SWP_NOZORDER;
	SendMessage(WM_WINDOWPOSCHANGED, 0, (LPARAM)&wp);
}

void CAGListCtrl::DrawItem(LPDRAWITEMSTRUCT lpMeasureItemStruct)
{
	CDC* pDC = CDC::FromHandle(lpMeasureItemStruct->hDC);
	LVITEM lvi = { 0 };
	lvi.mask = LVIF_STATE;//|LVIF_IMAGE; 
	lvi.stateMask = LVIS_FOCUSED | LVIS_SELECTED;
	lvi.iItem = lpMeasureItemStruct->itemID;
	BOOL bGet = GetItem(&lvi);
	//高亮显示
	BOOL bHighlight = ((lvi.state & LVIS_DROPHILITED) || ((lvi.state & LVIS_SELECTED) &&
		((GetFocus() == this) || (GetStyle() & LVS_SHOWSELALWAYS))));
	// 画文本背景 
	CRect rcBack = lpMeasureItemStruct->rcItem;
	pDC->SetBkMode(TRANSPARENT);
	if (bHighlight) //如果被选中
	{
		pDC->SetTextColor(RGB(255, 255, 255)); //文本为白色
		pDC->FillRect(rcBack, &CBrush(RGB(90, 162, 0)));
	}
	else
	{
		pDC->SetTextColor(RGB(0, 0, 0));       //文本为黑色
		pDC->FillRect(rcBack, &CBrush(RGB(255, 255, 255)));
	}
	if (lpMeasureItemStruct->itemAction & ODA_DRAWENTIRE)
	{
		//写文本 
		CString szText;
		int nCollumn = GetHeaderCtrl()->GetItemCount();//列数
		for (int i = 0; i < GetHeaderCtrl()->GetItemCount(); i++)
		{ //循环得到文本 
			CRect rcItem;
			if (!GetSubItemRect(lpMeasureItemStruct->itemID, i, LVIR_LABEL, rcItem))
				continue;
			szText = GetItemText(lpMeasureItemStruct->itemID, i);
			rcItem.left += 5; rcItem.right -= 1;
			pDC->DrawText(szText, lstrlen(szText), &rcItem, DT_VCENTER | DT_NOPREFIX | DT_SINGLELINE);
		}
	}

}