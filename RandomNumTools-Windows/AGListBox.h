#pragma once

class CAGListBox:public CListBox
{
public:
	CAGListBox();
	~CAGListBox();

public:
	int AddString(LPCTSTR lpszItem);											// Adds a string to the list box
	int AddString(LPCTSTR lpszItem, COLORREF rgb);						// Adds a colored string to the list box
	int InsertString(int nIndex, LPCTSTR lpszItem);						// Inserts a string to the list box
	int InsertString(int nIndex, LPCTSTR lpszItem, COLORREF rgb);	// Inserts a colored string to the list box
	void SetItemColor(int nIndex, COLORREF rgb);							// Sets the color of an item in the list box

	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CColorListBox)
public:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	virtual void MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct);
};