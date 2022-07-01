#pragma once

#include "AFX_EXT_SHAPE.h"

#include "ShapeGroup.h"

#pragma pack(push, 8)

//=============================================================================
// CShapeCollection (Layer...) 단순 집합

class AFX_EXT_CLASS_SHAPE CShapeCollection : public CShapeGroup {
public:
	BOOL m_bUse;
	CProfileSection m_profile;		// other settings for Layer.

protected:
	CShapeCollection(eSHAPE eShape, COLORREF cr = CR_DEFAULT) : CShapeGroup(eShape, cr) {
	}

public:
	CShapeCollection(COLORREF cr = CR_DEFAULT, LPCTSTR pszName = NULL, CPoint2d pt = CPoint2d(), BOOL bUse = TRUE) : CShapeGroup(S_COLLECTION, cr,  pszName, pt), m_bUse(bUse) {
		m_bUse = TRUE;

		//// Sample...
		//CProfileSection& sectionLaser = m_profile.GetSubSection(_T("Laser"));
		//sectionLaser.SetItemValue(_T("LaserRecipeName"), _T("Samples..."));
		//sectionLaser.SetItemValue(_T("LaserWidth"), 0.045);
		//sectionLaser.SetItemValue(_T("LaserPower"), 10.0);
		//sectionLaser.SetItemValue(_T("SlitSizeX"), 3.0);
		//sectionLaser.SetItemValue(_T("SlitSizeY"), 3.0);
		//sectionLaser.SetItemValue(_T("SlitAngle"), 0.0);
		//sectionLaser.SetItemValue(_T("ProcessSpeed"), 10.0);

		//CProfileSection& sectionScanner = m_profile.GetSubSection(_T("Scanner"));
		//sectionScanner.SetItemValue(_T("JumpSpeed"), 7000.0);
		//sectionScanner.SetItemValue(_T("MarkSpeed"), 4000.0);
		//sectionScanner.SetItemValue(_T("JumpDelay"), 100.0);
		//sectionScanner.SetItemValue(_T("MarkDelay"), 100.0);
		//sectionScanner.SetItemValue(_T("PolygonDelay"), 100.0);
		//sectionScanner.SetItemValue(_T("OnDelay"), 100.0);
		//sectionScanner.SetItemValue(_T("OffDelay"), 100.0);
	}
	CShapeCollection(const CShapeCollection& B) : CShapeGroup(S_COLLECTION, B.m_cr) {
		*this = B;
	}
	virtual ~CShapeCollection() {
	}
	DECLARE_NEWCLONE(CShapeCollection);
	DECLARE_DYNAMIC_CREATE(CShapeCollection, S_COLLECTION);
	DECLARE_DYNAMIC_SERIALIZE(CShapeCollection);

	CShapeCollection& operator = (const CShapeCollection& B) { return (CShapeCollection&)CopyFrom(B); }
	virtual CShapeObject& CopyFrom(const CShapeObject& B) {
		if (this == &B)
			return *this;
		__super::CopyFrom(B);
		const CShapeCollection* pB = dynamic_cast<const CShapeCollection*>(&B);
		if (!pB)
			return *this;
		m_bUse		= pB->m_bUse;
		m_profile	= pB->m_profile;
		return *this;
	}

	virtual CShapeObject& CopyPropertiesFrom(const CShapeObject& B) {
		if (this == &B)
			return *this;
		__super::CopyPropertiesFrom(B);
		const CShapeCollection* pB = dynamic_cast<const CShapeCollection*>(&B);
		if (!pB)
			return *this;
		m_bUse		= pB->m_bUse;
		m_profile	= pB->m_profile;
		return *this;
	}

	virtual CArchive& Serialize(CArchive& ar);
	template <class ARCHIVE> friend ARCHIVE& operator << (ARCHIVE& ar, const CShapeCollection& B) { ASSERT(ar.IsStoring()); return ((CShapeCollection&)B).Serialize(ar); }
	template <class ARCHIVE> friend ARCHIVE& operator >> (ARCHIVE& ar, CShapeCollection& B) { return B.Serialize(ar); }

	BOOL operator == (const CShapeObject& B) const { return Compare(B); }
	BOOL operator == (LPCSTR pszName) const { return m_strName == pszName; }
	BOOL operator == (LPCWSTR pszName) const { return m_strName == pszName; }

	virtual BOOL Compare(const CShapeObject& B) const;

	virtual BOOL ClipByRegion(const std::vector<CRect2d>& rects, TRefList<CShapeCollection>& collections);
};

#pragma pack(pop)
