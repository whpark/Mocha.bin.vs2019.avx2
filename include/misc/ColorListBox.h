#ifndef __COLORLISTBOX_H__SEONGHYEONG_LEE__
#define __COLORLISTBOX_H__SEONGHYEONG_LEE__

#pragma once

#pragma pack(push, 4/*MFC*/)

class CColorListBox : public CListBox
{

public:
	CColorListBox();
	virtual				~CColorListBox();

//{{AFX_VIRTUAL(CColorListBox)														// ClassWizard generated virtual function overrides
public:
	virtual void			DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	virtual void			MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct);
//}}AFX_VIRTUAL

protected:
//{{AFX_MSG(CColorListBox)															// Generated message map functions
//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

public:
	int					AddString(LPCTSTR lpszItem, COLORREF color=RGB(0,0,0));
};

#pragma pack(pop)

#endif
