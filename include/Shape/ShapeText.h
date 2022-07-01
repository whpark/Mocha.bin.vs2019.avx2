#pragma once

#include "AFX_EXT_SHAPE.h"

#include "ShapeComplex.h"

#pragma pack(push, 8)

class AFX_EXT_CLASS_SHAPE CShapeText : public CShapeComplex {
public:
	//TRefPointer<CPoint2d> m_rpt;
	//CPoint2d& m_pt;
	LOGFONT m_font;
	double m_dHeight;			// Text Height
	double m_dLineSpacingFactor;
	double m_dWidth;			// Box Width
	DWORD m_dwAlign;			// DT_ values DT_TOP, DT_LEFT, DT_CENTER, DT_RIGHT, DT_VCENTER, DT_BOTTOM

protected:
	CStringW m_str;

public:
	CShapeText(COLORREF cr = CR_DEFAULT) : CShapeComplex(S_TEXT, cr)/*, m_rpt(new CPoint2d), m_pt(*m_rpt)*/ {
		ZeroVar(m_font);
		m_dHeight = 1.0;
		m_dLineSpacingFactor = 1.0;
		m_dWidth = 1.0;
		m_dwAlign = DT_TOP|DT_LEFT;
		m_eHatching = SH_NONE;
		m_dHatching = 1;
	}
	CShapeText(const CShapeText& B) : CShapeComplex(S_TEXT, B.m_cr)/*, m_rpt(new CPoint2d(B.m_pt)), m_pt(*m_rpt)*/ {
		*this = B;
	}
	virtual ~CShapeText(void) {
	}
	DECLARE_NEWCLONE(CShapeText);
	DECLARE_DYNAMIC_CREATE(CShapeText, S_TEXT);

public:

	// default assign operator
	CShapeText& operator = (const CShapeText& B) { return (CShapeText&)CopyFrom(B); }
	virtual CShapeObject& CopyFrom(const CShapeObject& B) {
		if (this == &B)
			return *this;
		__super::CopyFrom(B);
		const CShapeText* pB = dynamic_cast<const CShapeText*>(&B);
		if (!pB)
			return *this;

		m_str					= pB->m_str;
		m_font					= pB->m_font;
		//m_pt					= pB->m_pt;
		m_dHeight				= pB->m_dHeight;
		m_dLineSpacingFactor	= pB->m_dLineSpacingFactor;
		m_dWidth				= pB->m_dWidth;
		m_dwAlign				= pB->m_dwAlign;

		Make();

		return *this;
	}

	virtual CArchive& Serialize(CArchive& ar) {
		__super::Serialize(ar);
		if (ar.IsStoring()) {
			CPoint2d pt;
			ar << pt;	// N/A

			// Font
			//ar.Write(&m_font, nSizeFont);
			ar << m_font.lfHeight;
			ar << m_font.lfWidth;
			ar << m_font.lfEscapement;
			ar << m_font.lfOrientation;
			ar << m_font.lfWeight;
			ar << m_font.lfItalic;
			ar << m_font.lfUnderline;
			ar << m_font.lfStrikeOut;
			ar << m_font.lfCharSet;
			ar << m_font.lfOutPrecision;
			ar << m_font.lfClipPrecision;
			ar << m_font.lfQuality;
			ar << m_font.lfPitchAndFamily;
			ar << CStringW(m_font.lfFaceName);

			ar << m_dHeight;
			ar << m_dLineSpacingFactor;
			ar << m_dWidth;
			ar << m_dwAlign;
#ifdef _UNICODE
			ar << m_str;
#else
			ar << CStringW(m_str);
#endif
		} else {

			CStringW str;

			CPoint2d pt;
			ar >> pt;	// N/A

			// Font
			//ar.Write(&m_font, nSizeFont);
			ar >> m_font.lfHeight;
			ar >> m_font.lfWidth;
			ar >> m_font.lfEscapement;
			ar >> m_font.lfOrientation;
			ar >> m_font.lfWeight;
			ar >> m_font.lfItalic;
			ar >> m_font.lfUnderline;
			ar >> m_font.lfStrikeOut;
			ar >> m_font.lfCharSet;
			ar >> m_font.lfOutPrecision;
			ar >> m_font.lfClipPrecision;
			ar >> m_font.lfQuality;
			ar >> m_font.lfPitchAndFamily;
			ar >> str;
			_tcscpy_s(m_font.lfFaceName, CString(str));

			ar >> m_dHeight;
			ar >> m_dLineSpacingFactor;
			ar >> m_dWidth;
			ar >> m_dwAlign;
#ifdef _UNICODE
			ar >> m_str;
#else
			ar >> str;
			SetString(str);
#endif
			Make();
		}
		return ar;
	}

	void SetString(LPCWSTR psz) {
		m_str = psz;
		if (m_font.lfHeight == 0) {
			CFont font;
			font.CreatePointFont(360, _T("Gulim"));
			font.GetLogFont(&m_font);
		}
		Make();
	}
	LPCWSTR GetString() const { return m_str; }

	virtual BOOL Make();

public:
	BOOL operator == (const CShapeComplex& B) const { return Compare(B); }
	virtual CString Print() const {

		return Format(_T(" \"%s\""), CString(m_str));
	}

public:
	// Operations
	virtual BOOL Compare(const CShapeObject& B) const {
		if (!__super::Compare(B))
			return FALSE;
		CShapeText& _B = (CShapeText&)B;
		return (m_str == _B.m_str)
			//&& (m_pt == _B.m_pt)
			&& (m_dHeight == _B.m_dHeight)
			&& (m_dLineSpacingFactor == _B.m_dLineSpacingFactor)
			&& (m_dWidth == _B.m_dWidth)
			&& (m_dwAlign == _B.m_dwAlign)
			;
	}

};


#pragma pack(pop)
