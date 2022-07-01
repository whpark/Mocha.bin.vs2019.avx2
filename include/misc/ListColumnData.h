
#pragma once
#include <vector>

#pragma pack(push, 8)

class CListCtrlColumnEx;
class CListCtrlEx;

enum eListCtrlExTextAlign { 
	eLCETA_LEFT		= DT_LEFT,
	eLCETA_CENTER	= DT_CENTER,
	eLCETA_RIGHT	= DT_RIGHT, 
};

enum eListCtrlExLineType {
	eLCELT_NONE		= -1, 
	eLCELT_SOLID	= PS_SOLID,
	eLCELT_DOT		= PS_DOT, 
};

namespace miscListCtrlColumn {

	struct T_CR_TEXT_BK {

		T_CR_TEXT_BK(COLORREF crT = ::GetSysColor(COLOR_WINDOWTEXT), COLORREF crB = ::GetSysColor(COLOR_WINDOW))
			: crText(crT), crBk(crB) {}

		T_CR_TEXT_BK(const T_CR_TEXT_BK& B) { *this = B; }
		T_CR_TEXT_BK& operator = (const T_CR_TEXT_BK& B) {
			if (this == &B)
				return *this;
			
			crText	= B.crText;
			crBk	= B.crBk;
			return *this;
		}

		COLORREF	crText;
		COLORREF	crBk;

	};

	struct T_PROGRESS_DATA {
	
		T_PROGRESS_DATA(int nPercent = 0, LPCTSTR lpFormat = _T("%d %%"))
			: iPercent(nPercent), lpProgressFormat(lpFormat) {}

		T_PROGRESS_DATA(const T_PROGRESS_DATA& B) { *this = B; }
		T_PROGRESS_DATA& operator = (const T_PROGRESS_DATA& B) {
			if (this == &B)
				return *this;
			
			iPercent			= B.iPercent;
			lpProgressFormat	= B.lpProgressFormat;
			return *this;
		}

		int						iPercent;
		LPCTSTR					lpProgressFormat;
	
	};
	
	struct LISTCTRLEX_HEADER {
	
		LISTCTRLEX_HEADER() { Init(); }
		LISTCTRLEX_HEADER(const LISTCTRLEX_HEADER& B) { *this = B; }
		LISTCTRLEX_HEADER& operator = (const LISTCTRLEX_HEADER& B) {
			if (this == &B)
				return *this;
			// Edit Box
			bEdit					= B.bEdit;
			bNumber					= B.bNumber;
			bDouble					= B.bDouble;
			iPoint					= B.iPoint;
			iMaxLength				= B.iMaxLength;
			dwEditStyle				= B.dwEditStyle;
	
			// Combo Box;
			bCombo					= B.bCombo;
			vtComboText				= B.vtComboText;
			bShowDropDown			= B.bShowDropDown;
	
			// Date Box;
			bDate					= B.bDate;
			bTimeUpdate				= B.bTimeUpdate;
			lpDateFormat			= B.lpDateFormat;
			odtTime					= B.odtTime;
			dwDateStyle				= B.dwDateStyle;
	
			// Spin Box;
			bSpin					= B.bSpin;
			dMinNumber				= B.dMinNumber;
			dMaxNumber				= B.dMaxNumber;
	
			// Check Box;
			bCheck					= B.bCheck;
			bTextChange				= B.bTextChange;
			iShowHeaderCheck		= B.iShowHeaderCheck;
			iTotalCheck				= B.iTotalCheck;
			lpCheckText				= B.lpCheckText;
			lpUnCheckText			= B.lpUnCheckText;
			vtCheckData				= B.vtCheckData;
	
			// Progress Box;
			bProgress				= B.bProgress;
			lpProgressFormat		= B.lpProgressFormat;
			bShowProgressText		= B.bShowProgressText;
			vtProgressData			= B.vtProgressData;

			// Button
			bButton					= B.bButton;
			crButton				= B.crButton;
			lpCaption				= B.lpCaption;
	
			// ETC
			iSubItem				= B.iSubItem;
			vtSubItemText			= B.vtSubItemText;
			width					= B.width;
			alignment				= B.alignment;
			hover					= B.hover;
			bVisible				= B.bVisible;
			vtCrTextBk				= B.vtCrTextBk;
			vtActiveControl			= B.vtActiveControl;
			_tcscpy_s(szDesc, B.szDesc);
	
			return *this;
		}
	
		virtual ~LISTCTRLEX_HEADER() {
			DeleteData();
		}

		void Init() {
			DeleteData();
			// Edit Box
			bEdit					= FALSE;
			bNumber					= FALSE;
			bDouble					= FALSE;
			iPoint					= 3;
			iMaxLength				= -1;
			dwEditStyle				= ES_AUTOHSCROLL|ES_AUTOVSCROLL|ES_CENTER|ES_NOHIDESEL|WS_CHILD|WS_VISIBLE;
	
			// Combo Box
			bCombo					= FALSE;
			bShowDropDown			= TRUE;
			
			// Date Box
			bDate					= FALSE;
			bTimeUpdate				= TRUE;
			lpDateFormat			= _T("yyyy-MM-dd HH:mm:ss");
			odtTime					= COleDateTime::GetCurrentTime();
			dwDateStyle				= WS_VISIBLE|WS_CHILD|WS_TABSTOP|DTS_SHORTDATEFORMAT;
	
			// Spin Box
			bSpin					= FALSE;
			dMinNumber				= 0.;
			dMaxNumber				= 0.;
	
			// Check Box
			bCheck					= FALSE;
			bTextChange				= FALSE;
			iShowHeaderCheck		= 1;
			iTotalCheck				= 0;
			lpCheckText				= _T("");
			lpUnCheckText			= _T("");
	
			// Progress Box
			bProgress				= FALSE;
			lpProgressFormat		= _T("%d %%");
			bShowProgressText		= TRUE;

			// Button
			bButton					= FALSE;
			crButton				= RGB(120, 200, 200);
			lpCaption				= _T("");
	
			// ETC
			iSubItem				= -1;
			width					= 0;
			alignment				= eLCETA_CENTER;
			hover					= FALSE;
			bVisible				= TRUE;
			_tcscpy_s(szDesc, _T(""));
		}
		
		void DeleteData() {
			std::vector<CString>			comboTemp;
			std::vector<int>				checkTemp;
			std::vector<T_PROGRESS_DATA>	progressTemp;
			std::vector<CString>			subitemTemp;
			std::vector<T_CR_TEXT_BK>		crTextBkTemp;
			std::vector<BOOL>				buttonTemp;
			std::vector<BOOL>				activeTemp;
	
			comboTemp.swap(vtComboText);
			checkTemp.swap(vtCheckData);
			progressTemp.swap(vtProgressData);
			subitemTemp.swap(vtSubItemText);
			crTextBkTemp.swap(vtCrTextBk);
			buttonTemp.swap(vtButtonDown);
			activeTemp.swap(vtActiveControl);
		}

		// Edit Box
		BOOL							bEdit;
		BOOL							bNumber;
		BOOL							bDouble;
		int								iPoint;
		int								iMaxLength;
		DWORD							dwEditStyle;
	
		// Combo Box
		BOOL							bCombo;
		std::vector<CString>			vtComboText;
		BOOL							bShowDropDown;
	
		// Date Box
		BOOL							bDate;
		BOOL							bTimeUpdate;
		LPCTSTR							lpDateFormat;
		COleDateTime					odtTime;
		DWORD							dwDateStyle;
	
		// Spin Box
		BOOL							bSpin;
		double							dMinNumber;
		double							dMaxNumber;
	
		// Check Box
		BOOL							bCheck;
		BOOL							bTextChange;
		int								iShowHeaderCheck;
		int								iTotalCheck;
		LPCTSTR							lpCheckText;
		LPCTSTR							lpUnCheckText;
		std::vector<int>				vtCheckData;
	
		// Progress Box
		BOOL							bProgress;
		LPCTSTR							lpProgressFormat;
		BOOL							bShowProgressText;
		std::vector<T_PROGRESS_DATA>	vtProgressData;

		// Button
		BOOL							bButton;
		COLORREF						crButton;
		LPCTSTR							lpCaption;
		std::vector<BOOL>				vtButtonDown;
	
		// ETC
		int								iSubItem;
		TCHAR							szDesc[128];
		std::vector<CString>			vtSubItemText;
		int								width;
		eListCtrlExTextAlign			alignment;
		BOOL							hover;
		BOOL							bVisible;	// PWH
		std::vector<T_CR_TEXT_BK>		vtCrTextBk;
		std::vector<BOOL>				vtActiveControl;
	
	};
	
	
	class CColumnSpinBox : public CSpinButtonCtrl
	{
		DECLARE_DYNAMIC(CColumnSpinBox)
		friend class CListCtrlColumnEx;
		friend class CListCtrlEx;
	
	public:
		CColumnSpinBox() {}
		virtual ~CColumnSpinBox() {}
	
	protected:
		DECLARE_MESSAGE_MAP()
		afx_msg void OnDeltapos(NMHDR *pNMHDR, LRESULT *pResult);
	
	};
	
	
	class CColumnEditBox : public CEdit
	{
		DECLARE_DYNAMIC(CColumnEditBox)
		friend class CListCtrlColumnEx;
		friend class CListCtrlEx;
	
	public:
		CColumnEditBox() : m_iVK(0), m_bSpin(0), m_dMinNumber(0), m_dMaxNumber(0) {}
		virtual ~CColumnEditBox() {}
	
	protected:
		WPARAM			m_iVK;
		int				m_bSpin;
		double			m_dMinNumber;
		double			m_dMaxNumber;
		CColumnSpinBox	m_spinBox;

	protected:
		virtual	BOOL	PreTranslateMessage(MSG* pMsg);
		void			OnCloseEdit();
		void			SetNumberRange(double dMinNumber, double dMaxNumber) { m_dMinNumber = dMinNumber; m_dMaxNumber = dMaxNumber; }
		BOOL			SetSpin(BOOL bSpin) { return m_bSpin = bSpin; }
		BOOL			IsSpin() const { return m_bSpin; }
		double			GetMinNumber() const { return m_dMinNumber; }
		double			GetMaxNumber() const { return m_dMaxNumber; }
	
		DECLARE_MESSAGE_MAP()
		afx_msg void OnKillFocus(CWnd* pNewWnd);
		afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	
	};
	
	
	class CColumnComboBox : public CComboBox
	{
		DECLARE_DYNAMIC(CColumnComboBox)
		friend class CListCtrlColumnEx;
		friend class CListCtrlEx;
	
	public:
		CColumnComboBox() : m_iVK(0), m_iCurSel(-1) {}
		virtual ~CColumnComboBox() {}
	
	protected:
		WPARAM			m_iVK;
		int				m_iCurSel;
	
	protected:
		virtual	BOOL	PreTranslateMessage(MSG* pMsg);
		void			OnCloseCombo();
	
		DECLARE_MESSAGE_MAP()
		afx_msg void OnKillFocus(CWnd* pNewWnd);
		afx_msg void OnCbnCloseup();
	
	};
	
	
	class CColumnDateBox : public CDateTimeCtrl
	{
		DECLARE_DYNAMIC(CColumnDateBox)
		friend class CListCtrlColumnEx;
		friend class CListCtrlEx;
	
	public:
		CColumnDateBox() : m_iVK(0) {}
		virtual ~CColumnDateBox() {}
	
	protected:
		WPARAM			m_iVK;
	
	protected:
		virtual	BOOL	PreTranslateMessage(MSG* pMsg);
		void			OnCloseDate();
		void			SetDateTime(LISTCTRLEX_HEADER* pData, const CString& strDate);
		CString			GetDateTime(const COleDateTime& odtTime, const CString& strFormat) const;
	
		DECLARE_MESSAGE_MAP()
		afx_msg void OnKillFocus(CWnd* pNewWnd);
		afx_msg void OnDtnCloseup(NMHDR* pNMHDR, LRESULT* pResult);
	
	};

	#define WM_LISTCTRLEX_ITEM_CHANGED			WM_APP+13
	#define WM_LISTCTRLEX_ITEM_LBUTTONDOWN		WM_APP+14
	#define WM_LISTCTRLEX_ITEM_RBUTTONDOWN		WM_APP+15
	#define WM_LISTCTRLEX_KEY_DOWN				WM_APP+16

};	// namespace miscListCtrlColumn

#pragma pack(pop)