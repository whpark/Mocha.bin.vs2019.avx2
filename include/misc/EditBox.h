#pragma once

//=============================================================================
// CEditBox - by 이범호 2014.01.23.
//
// 사용법 : 
//

#include "AFX_EXT_MISC.h"
#define		WM_APP_ED_EDIT_TAB				(WM_APP+4102)
#define		WM_APP_ED_EDIT_FINISHED			(WM_APP+4103)


namespace miscEditBox {	// PWH. hiding
    class CEditBoxEx : public CEdit
    {
	    DECLARE_DYNAMIC(CEditBoxEx)

		enum VARIABLE_TYPE{
		    VT_NONE = -1,
		    VT_INT,
		    VT_DOUBLE,
		    VT_WSTR,
			VT_STR,
	    };

    public:
	    CEditBoxEx();
	    virtual ~CEditBoxEx();
    
    protected:
	    DECLARE_MESSAGE_MAP()
    public:
	    virtual BOOL PreTranslateMessage(MSG* pMsg);
    
    protected :
	    void EditStart(CRect rect, CString strValue);
    
    public :
	    CWnd*			m_pWnd;
	    VARIABLE_TYPE	m_iType;
    
	    union {
		    int*			m_piValue;
		    double*			m_pdValue;
			CStringA*		m_pstrAValue;
		    CStringW*		m_pstrWValue;
	    };

		int				m_iMinValue;
		int				m_iMaxValue;

		double			m_dMinValue;
		double			m_dMaxValue;
		BOOL			m_bEditEnd;
    
	    void EditStart(CWnd* pWnd, CRect rect, int& iValue);
	    void EditStart(CWnd* pWnd, CRect rect, double& dValue, int iPoint = 3);
		void EditStart(CWnd* pWnd, CRect rect, CStringA& strValue);
	    void EditStart(CWnd* pWnd, CRect rect, CStringW& strValue);
	    void EditEnd(BOOL bEdit = FALSE);
	    afx_msg void OnKillFocus(CWnd* pNewWnd);
	    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
    };

};

//-----------------------------------------------------------------------------
// CEditBox
class /*AFX_EXT_CLASS_MISC*/ CEditBox {
public:
	CEditBox();
	virtual ~CEditBox();

protected:
	miscEditBox::CEditBoxEx		m_edit;
	CFont						m_font;
public:
	CString strEditEnd;
public :
// EditStart 지역변수 할당 금지.
// bEditEnd == TRUE일 경우 EditStart 지역변수 사용 가능. //Edit End 함수 따로 처리.
	void EditStart(CWnd* pWnd, CRect rect, int& iValue, BOOL bEditEnd = FALSE, int iMinValue = -INT_MAX, int iMaxValue = INT_MAX, DWORD dwStyle = ES_LEFT | ES_AUTOHSCROLL, int iFontPointSize = 9, CString strFont = _T("Segoe UI")); // Pixel이 아닌 Point Size);
	void EditStart(CWnd* pWnd, CRect rect, double& dValue, BOOL bEditEnd = FALSE, int iPoint = 3, double dMinValue = -DBL_MAX, double dMaxValue = DBL_MAX, DWORD dwStyle = ES_LEFT | ES_AUTOHSCROLL, int iFontPointSize = 9, CString strFont = _T("Segoe UI"));
	void EditStart(CWnd* pWnd, CRect rect, CStringA& strValue, BOOL bEditEnd = FALSE, DWORD dwStyle = ES_LEFT | ES_AUTOHSCROLL, int iFontPointSize = 9, CString strFont = _T("Segoe UI"));
	void EditStart(CWnd* pWnd, CRect rect, CStringW& strValue, BOOL bEditEnd = FALSE, DWORD dwStyle = ES_LEFT | ES_AUTOHSCROLL, int iFontPointSize = 9, CString strFont = _T("Segoe UI"));
	void EditEnd();

protected:
	void EditSetting(CWnd* pWnd, CRect rect, DWORD dwStyle = ES_LEFT, int iFontPointSize = 9, CString strFont = _T("Segoe UI")); //Font Setting 및 CEditBoxEx 생성.
};
AFX_EXT_API_MISC void EditStart(CWnd* pWnd, CRect rect, CStringA& strValue, BOOL bEditEnd = FALSE, BOOL bOsk = FALSE, DWORD dwStyle = ES_LEFT | ES_AUTOHSCROLL, int iFontPointSize = 9, CString strFont = _T("Segoe UI"));
AFX_EXT_API_MISC void EditStart(CWnd* pWnd, CRect rect, CStringW& strValue, BOOL bEditEnd = FALSE, BOOL bOsk = FALSE, DWORD dwStyle = ES_LEFT | ES_AUTOHSCROLL, int iFontPointSize = 9, CString strFont = _T("Segoe UI"));
AFX_EXT_API_MISC void EditStart(CWnd* pWnd, CRect rect, int& iValue, BOOL bEditEnd = FALSE, BOOL bOsk = FALSE, int iMinValue = -INT_MAX, int iMaxValue = INT_MAX, DWORD dwStyle = ES_LEFT | ES_AUTOHSCROLL, int iFontPointSize = 9, CString strFont = _T("Segoe UI"));
AFX_EXT_API_MISC void EditStart(CWnd* pWnd, CRect rect, double& dValue, BOOL bEditEnd = FALSE, BOOL bOsk = FALSE, double dMinValue = -DBL_MAX, double dMaxValue = DBL_MAX, int iPoint = 3, DWORD dwStyle = ES_LEFT | ES_AUTOHSCROLL, int iFontPointSize = 9, CString strFont = _T("Segoe UI"));
AFX_EXT_API_MISC CString GetEditString();
void ShowOsk(BOOL bShow = FALSE);
