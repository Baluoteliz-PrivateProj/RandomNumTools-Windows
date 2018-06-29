#pragma once

class CAGListCtrl :public CListCtrl
{
public:
	CAGListCtrl(void);
	~CAGListCtrl(void);

	DECLARE_MESSAGE_MAP()

	virtual void MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	virtual void OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	virtual void DrawItem(LPDRAWITEMSTRUCT lpMeasureItemStruct);

public:
	void SetRowHeigt(int nHeight);

private:
	int m_nRowHeight;
};