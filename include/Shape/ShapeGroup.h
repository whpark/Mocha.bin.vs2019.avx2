#pragma once

#include "AFX_EXT_SHAPE.h"

#include "ShapeObject.h"

#pragma pack(push, 8)

//=============================================================================
// CShapeGroup : 단순 Object 집합
class CProgressDlg;
class AFX_EXT_CLASS_SHAPE CShapeGroup : public CShapeObject {
public:
	CString m_strName;			// used by CDXFLoader
	TRefPointer<CPoint2d> m_rpt;	// used by CDXFLoader
	CPoint2d& m_pt;				// used by CDXFLoader

	CRect2d m_rectBounding;

public:
	TRefList<CShapeObject> m_objects;

public:
	CShapeGroup(COLORREF cr = CR_DEFAULT, LPCTSTR pszName = NULL, CPoint2d pt = CPoint2d()) : CShapeObject(S_GROUP, cr), m_strName(pszName), m_rpt(new CPoint2d(pt)), m_pt(*m_rpt) {
	}
protected:
	CShapeGroup(eSHAPE eShape, COLORREF cr = CR_DEFAULT, LPCTSTR pszName = NULL, CPoint2d pt = CPoint2d()) : CShapeObject(eShape, cr), m_strName(pszName), m_rpt(new CPoint2d(pt)), m_pt(*m_rpt) {
	}
public:
	CShapeGroup(const CShapeGroup& B) : CShapeObject(S_GROUP, B.m_cr), m_rpt(new CPoint2d), m_pt(*m_rpt) {
		*this = B;
	}
	virtual ~CShapeGroup() {
	}
	DECLARE_NEWCLONE(CShapeGroup);
	DECLARE_DYNAMIC_CREATE(CShapeGroup, S_GROUP);

	CShapeGroup& operator = (const CShapeGroup& B) { return (CShapeGroup&)CopyFrom(B); }
	virtual CShapeObject& CopyFrom(const CShapeObject& B) {
		CopyPropertiesFrom(B);
		m_objects.DeleteAll();

		const CShapeGroup* pB = dynamic_cast<const CShapeGroup*>(&B);
		if (!pB)
			return *this;
		m_objects = pB->m_objects;
		return *this;
	}
	virtual CShapeObject& CopyPropertiesFrom(const CShapeObject& B) {
		if (this == &B)
			return *this;
		__super::CopyFrom(B);
		const CShapeGroup* pB = dynamic_cast<const CShapeGroup*>(&B);
		if (!pB)
			return *this;
		m_strName		= pB->m_strName;
		m_pt			= pB->m_pt;
		m_rectBounding	= pB->m_rectBounding;
		return *this;
	}

	virtual CArchive& Serialize(CArchive& ar);

	BOOL operator == (const CShapeObject& B) const { return Compare(B); }
	BOOL operator == (LPCTSTR pszName) const { return m_strName == pszName; }

public:
	virtual CString Print() const { return CString(); }

	virtual COLORREF SetColor(COLORREF cr) {
		COLORREF crOld = CShapeObject::SetColor(cr);
		TRefListIter<CShapeObject> objects(m_objects);
		for (int i = 0; i < objects.N(); i++)
			objects[i].SetColor(cr);
		return crOld;
	}
	//virtual DWORD GetHatching() const { return m_eHatching; }
	//virtual double GetHatchingDensity() const { return m_dHatching; }
	virtual void SetHatching(DWORD eHatching = SH_NONE, double dHatching = 0.0) {
		TRefListIter<CShapeObject> objects(m_objects);
		for (int i = 0; i < objects.N(); i++)
			objects[i].SetHatching(eHatching, dHatching);
	}

public:
	virtual BOOL AddObject(TRefPointer<CShapeObject> rObject, int iIndex = -1) { return m_objects.InsertAt(rObject, iIndex) >= 0; }
	virtual int DeleteObject(TRefPointer<CShapeObject>& rObject) {
		int iIndex = m_objects.Find((CShapeObject*)rObject);
		if (iIndex < 0)
			return -1;
		m_objects.Delete(iIndex);
		return iIndex;
	}
	const TRefList<CShapeObject>& GetObjects() const { return m_objects; }
	TRefList<CShapeObject>& GetObjects() { return m_objects; }

public:
	virtual CPoint2d GetCenterPoint() const {
		TRectD rectBound;
		GetBoundingRect(rectBound, TRUE);
		return rectBound.CenterPoint();
	}
	virtual void GetPoints(TList<TLineD>& lines) const {
		TRefListIter<CShapeObject> objects(m_objects);
		for (int i = 0; i < objects.N(); i++)
			objects[i].GetPoints(lines);
	}
	virtual void GetBoundingRect(TRectD& rect, BOOL bResetRect = TRUE) const {
		// to do .... 새로 만들 때 다시 만들어야 됨. 
		TRefListIter<CShapeObject> objects(m_objects);
		if (bResetRect)
			rect.SetRect(DBL_MAX, DBL_MAX, -DBL_MAX, -DBL_MAX);
		for (int i = 0; i < objects.N(); i++) {
			CRect2d r;
			objects[i].GetBoundingRect(r, TRUE);
			//if (r.IsRectEmpty())
			//	continue;
			if (rect.left > r.left) rect.left = r.left;
			if (rect.top > r.top) rect.top = r.top;
			if (rect.right < r.right) rect.right = r.right;
			if (rect.bottom < r.bottom) rect.bottom = r.bottom;
		}
		//if (rect.IsRectEmpty())
		//	rect.SetRectEmpty();
	}
	virtual BOOL GetStartEndPoint(CPoint2d& pt0, CPoint2d& pt1) const {
		m_objects.First();
		if (m_objects.N()) {
			CPoint2d ptA1, ptB0;
			m_objects.First()->GetStartEndPoint(pt0, ptA1);
			m_objects.Last()->GetStartEndPoint(ptB0, pt1);
		} else {
			return FALSE;
		}
		return TRUE;
	}
	virtual BOOL Compare(const CShapeObject& B) const;

	virtual BOOL AddLaserOffset(double dThickness) {
		TRefListIter<CShapeObject> objects(m_objects);
		for (int i = 0; i < objects.N(); i++)
			objects[i].AddLaserOffset(dThickness);
		return TRUE;
	}

	virtual void Revert() {
		{
			TRefListIter<CShapeObject> objects(m_objects);
			for (int i = 0; i < objects.N(); i++) objects[i].Revert();
		}
		m_objects.Invert();
	}
	virtual void AddOffset(const CPoint2d& ptOffset) {
		TRefListIter<CShapeObject> objects(m_objects);
		for (int i = 0; i < objects.N(); i++)
			objects[i].AddOffset(ptOffset);
	}
	virtual void FlipX(const CPoint2d& ptCenter) {
		TRefListIter<CShapeObject> objects(m_objects);
		for (int i = 0; i < objects.N(); i++)
			objects[i].FlipX(ptCenter);
	}
	virtual void FlipY(const CPoint2d& ptCenter) {
		TRefListIter<CShapeObject> objects(m_objects);
		for (int i = 0; i < objects.N(); i++)
			objects[i].FlipY(ptCenter);
	}
	virtual void Rotate(rad_t dTheta, const CPoint2d& ptCenter) {
		TRefListIter<CShapeObject> objects(m_objects);
		for (int i = 0; i < objects.N(); i++)
			objects[i].Rotate(dTheta, ptCenter);
	}
	virtual void Resize(double dScale, const CPoint2d& ptCenter) {
		TRefListIter<CShapeObject> objects(m_objects);
		for (int i = 0; i < objects.N(); i++)
			objects[i].Resize(dScale, ptCenter);
	}
	virtual void Resize(double dScaleX, double dScaleY, const CPoint2d& ptCenter) {
		TRefListIter<CShapeObject> objects(m_objects);
		for (int i = 0; i < objects.N(); i++)
			objects[i].Resize(dScaleX, dScaleY, ptCenter);
	}
	virtual BOOL Deflate(double dSizeX, double dSizeY) {
		TRefListIter<CShapeObject> objects(m_objects);
		for (int i = 0; i < objects.N(); i++)
			objects[i].Deflate(dSizeX, dSizeY);
		return TRUE;
	}
	virtual BOOL ClipByRect(const CRect2d& rect, TRefList<CShapeObject>& objectsResult) {
		TRefListIter<CShapeObject> objects(m_objects);
		for (int i = 0; i < objects.N(); i++)
			objects[i].ClipByRect(rect, objectsResult);
		return TRUE;
	}
	virtual void ChangeDirection(eSORT_DIRECTION eDirection, const CPoint2d& ptCenter = CPoint2d()) {
		TRefListIter<CShapeObject> objects(m_objects);
		for (int i = 0; i < objects.N(); i++)
			objects[i].ChangeDirection(eDirection, ptCenter);
	}

public:
	virtual BOOL Draw(ISCanvas& canvas) const {
		TRefListIter<CShapeObject> objects(m_objects);
		for (int i = 0; i < objects.N(); i++)
			objects[i].Draw(canvas);
		return TRUE;
	}

public:
	virtual BOOL BreakAndConnect(double dMaxSizeX, double dMaxSizeY, TRefList<CShapeGroup>& groupsNew, CShapeObject* pShapeLinkPolyline = NULL, CShapeObject* pShapeLinkPolylineBulge = NULL);
	virtual BOOL GetLinkagePosition(const TRefList<CShapeGroup>& groups, TLineD& pts, double dMinDistance = 0.0);
	virtual BOOL InsertLinkage(TRefList<CShapeGroup>& groups, CShapeObject* pObject, double dMinDistance = 0.0);

	virtual void SplitPolyline();

	virtual void DeleteDuplicatedObjects(BOOL bCheckReverse = FALSE);
	virtual void DeleteOverlappedObjects();

	// Split Region
	virtual BOOL CalcClippingRegion(const CSize2d& sizeMax, const CSize2d& sizeMin, std::vector<CRect2d>& rects, double dScale = 1.0, BOOL bVertFirst = TRUE);
	virtual BOOL ClipByRegion(const std::vector<CRect2d>& rects, TRefList<CShapeGroup>& groups);

	// Sort
	enum eSORT_METHOD		{ SM_NONE, SM_AUTO, SM_BY_DIRECTION, SM_FOR_OTF };
	struct T_SORT_OPTION {
		eSORT_METHOD eSortMethod;
		eSORT_DIRECTION eDirection;
		BOOL bSplitPolyline;
		double dParam1, dParam2, dParam3;
		T_SORT_OPTION() : eSortMethod(SM_BY_DIRECTION), eDirection(SD_ANY), bSplitPolyline(FALSE), dParam1(0.1), dParam2(3.0), dParam3(20.0) { }
	};

	virtual BOOL Sort(const T_SORT_OPTION& option, const CPoint2d& ptStart, BOOL bDisplayProgress = TRUE);
	static BOOL GetSortOptionFromUI(T_SORT_OPTION& option);

	virtual BOOL SortPre(eSORT_DIRECTION eDirection);
	static BOOL SortLines		(std::vector<TRefPointer<CShapeLine> >& lines, CPoint2d& ptLast, BOOL bReversible);
	static BOOL SortGeneral		(const T_SORT_OPTION& option, std::vector<TRefPointer<CShapeObject> >& objects, const CPoint2d& ptStart, CProgressDlg* pProgressDlg = NULL);
	static BOOL SortByDirection	(const T_SORT_OPTION& option, std::vector<TRefPointer<CShapeObject> >& objects, const CPoint2d& ptStart, CProgressDlg* pProgressDlg = NULL);
	static BOOL SortForOTF		(const T_SORT_OPTION& option, std::vector<TRefPointer<CShapeObject> >& objects, const CPoint2d& ptStart, CProgressDlg* pProgressDlg = NULL);

	virtual BOOL GetDrawLength(int& nDraw, double& dDrawLength, int& nJump, double& dJumpLength, BOOL bInitValues = TRUE) const;

};

//-----------------------------------------------------------------------------


#pragma pack(pop)
