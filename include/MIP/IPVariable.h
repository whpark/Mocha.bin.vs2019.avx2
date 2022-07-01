#pragma once

#include "AFX_EXT_MIP.h"

#include "TList.hpp"
#include "misc/xUtil.h"

#include "IP_Definitions.h"

#pragma pack(push, 8)

class AFX_EXT_CLASS_MIP CIPVariableException : public CIPException {
public:
	CIPVariableException(LPCTSTR pszFunctionName = NULL, LPCTSTR pszReason = NULL) : CIPException(IPE_VARIABLE_MISMATCH, pszFunctionName, pszReason) { }
};

class AFX_EXT_CLASS_MIP CIPVariable {
private:
	static CIPVariable s_varNULL;
private:
	mutable DStringA strA;
	mutable DStringW strW;
	mutable VARIANT variant;
protected:
	eIP_VARIABLE_TYPE m_eVarType;
	DStringA m_strName;
	TBuffer<BYTE> m_value;
	TRefList<CIPVariable> m_child;
	sizeXX_t m_nUserTypeItemSize;	// Valid only when type is IPVT_BINARY.
public:
	// Constructors
	explicit CIPVariable(eIP_VARIABLE_TYPE eVarType = IPVT_NONE, LPCSTR pszName = NULL, sizeXX_t nArrayCount = 1, sizeXX_t nUserTypeItemSize = 0) {
		m_eVarType = IPVT_NONE;
		m_nUserTypeItemSize = 0;
		SetType(eVarType, pszName, nArrayCount, nUserTypeItemSize);
	}
	explicit CIPVariable(const CIPVariable& B) {
		*this = B;
	}
	CIPVariable& operator = (const CIPVariable& B) {
		if (this != &B) {
			m_eVarType = B.m_eVarType;
			m_nUserTypeItemSize = B.m_nUserTypeItemSize;
			m_strName = B.m_strName;
			m_value.SetData(B.m_value.GetSize(), B.m_value);
			m_child.CopyFrom(B.m_child);
		}
		return *this;
	}
	eIP_VARIABLE_TYPE GetType() const { return m_eVarType; }
	static eIP_VARIABLE_TYPE GetTypeFromTypeName(LPCSTR pszTypeName);
	LPCSTR GetTypeName() const { return GetTypeName(m_eVarType); }
	static LPCSTR GetTypeName(eIP_VARIABLE_TYPE eVarType);
	void SetType(eIP_VARIABLE_TYPE eVarType = IPVT_NONE, LPCSTR pszName = NULL, sizeXX_t nArrayCount = 1, sizeXX_t nUserTypeItemSize = 0 /*Valid for IPVT_BINARY*/);
	template < typename T > void SetUserType(int nArrayCount = 1) { SetType(IPVT_BINARY, NULL, nArrayCount, sizeof(T)); }

	// Attribute.
	static sizeXX_t GetItemSize(eIP_VARIABLE_TYPE eVarType);		// IPVT_BINARY or User Type 경우에는 sizeof(BYTE)
	sizeXX_t GetItemSize() const { if ((m_eVarType == IPVT_BINARY) && (m_nUserTypeItemSize > 0) ) return m_nUserTypeItemSize; return GetItemSize(m_eVarType); }
	sizeXX_t GetCount() const;								// count of item.
	template <typename T>
	sizeXX_t GetCount() const { return m_value.GetSize() / sizeof(T); }
	LPCSTR GetName() const { return m_strName; }
	void SetName(LPCSTR pszName) { pszName = m_strName; }

	// Default Type
	operator int32_t () const;
	operator int64_t () const;
	operator uint32_t () const;
	operator uint64_t () const;
	operator double () const;
	operator const char* () const;
	operator const wchar_t* () const;
	operator VARIANT& () const;

	CIPVariable& operator = (int32_t);
	CIPVariable& operator = (int64_t);
	CIPVariable& operator = (uint32_t);
	CIPVariable& operator = (uint64_t);
	CIPVariable& operator = (double);
	CIPVariable& operator = (const char*);
	CIPVariable& operator = (const wchar_t*);
	CIPVariable& operator = (const VARIANT&);

	// Value
	template <typename T>
	T& Value(int iIndex = 0) {
		if (m_eVarType == IPVT_NONE)
			throw CIPVariableException(__TFUNCTION__, _T("Variable is EMPTY"));
		if ((size_t)m_value.GetSize() < sizeof(T)*(iIndex+1))
			throw CIPVariableException(__TFUNCTION__, _T("Variable size is small"));
		return *(((T*)m_value.GetBuffer()) + iIndex);	//  [iIndex] -> 이거 쓰면, operator overide 등 걸려서 오류 날 수 있음.
														// m_value.GetPointer<T>(iIndex); 이거 써도 됨. (buffer size 이중 검사)
	}
	template <typename T>
	const T& Value(int iIndex = 0) const {
		if (m_eVarType == IPVT_NONE)
			throw CIPVariableException(__TFUNCTION__, _T("Variable is EMPTY"));
		if ((size_t)m_value.GetSize() < sizeof(T)*(iIndex+1))
			throw CIPVariableException(__TFUNCTION__, _T("Variable size is small"));
		return *(((T*)m_value.GetBuffer()) + iIndex);	//  [iIndex] -> 이거 쓰면, operator overide 등 걸려서 오류 날 수 있음.
														// m_value.GetPointer<T>(iIndex); 이거 써도 됨. (buffer size 이중 검사)
	}
	// Child Value
	template <typename T>
	T& ChildValue(int iChildIndex, int iIndex = 0)								{ return (*this)(iChildIndex).Value<T>(iIndex); }
	template <typename T>
	T& ChildValue(const char* szChildName, int iIndex = 0)						{ return (*this)(szChildName).Value<T>(iIndex); }
	template <typename T>
	T& ChildValue(const wchar_t* szChildName, int iIndex = 0)					{ return (*this)(szChildName).Value<T>(iIndex); }
	template <typename T>
	const T& ChildValue(int iChildIndex, int iIndex = 0) const					{ return (*this)(iChildIndex).Value<T>(iIndex); }
	template <typename T>
	const T& ChildValue(const char* szChildName, int iIndex = 0) const			{ return (*this)(szChildName).Value<T>(iIndex); }
	template <typename T>
	const T& ChildValue(const wchar_t* szChildName, int iIndex = 0) const		{ return (*this)(szChildName).Value<T>(iIndex); }

	// Child Value (NOT FOR CLASS). If Empty, creates 1 child (in this case, iIndex must be 0).
	template <typename T>
	T& ChildLValue(const char* szChildName, const T& rDefault/*class 사용 금지, raw memory 만 복사함.*/, int iIndex = 0) {
		auto* pVar = FindChild(szChildName);
		if (!pVar)
			return SetChildItemUserType<T>(szChildName, &rDefault, iIndex+1).Value<T>(iIndex);
		return pVar->Value<T>(iIndex);
	}

	// Binary Data // User Type
	// Mat type
	BOOL IsTypeMat() const;
	template < typename MAT > BOOL SetMat(const MAT& mat);
	template < typename MAT > BOOL GetMat(MAT& mat) const;

	//// User Type
	//template <typename T>
	//T& operator[] (int iIndex) { return Value<T>(iIndex); }
	//template <typename T>
	//T& operator[] (int iIndex) const { return Value<T>(iIndex); }

	sizeXX_t GetSize() const { return m_value.GetSize(); }	// size of buffer. (in byte)
	BOOL GetRawData(void* pData, sizeXX_t nSize) const { if (!pData || (nSize <= 0)) return FALSE; memmove(pData, m_value, _min(nSize, (sizeXX_t)m_value.GetSize())); return TRUE; }
	void* GetRawData() { return (BYTE*)m_value; }
	const void* GetRawData() const { return (const BYTE*)m_value; }
	BOOL SetRawData(const void* pData, sizeXX_t nSize) { if (m_eVarType == IPVT_NONE) m_eVarType = IPVT_BINARY; if (m_eVarType != IPVT_BINARY) return FALSE; m_value.SetData(nSize, (const BYTE*)pData); return TRUE; }


	// Child Items...
		// Add Child Item
public:
	CIPVariable& SetChildItemEmpty(LPCSTR pszName, eIP_VARIABLE_TYPE eVarType = IPVT_NONE, sizeXX_t nArrayCount = 1, sizeXX_t nUserTypeItemSize = 0);
	CIPVariable& SetChildItem(LPCSTR pszName, long value, sizeXX_t nArrayCount = 1);
	CIPVariable& SetChildItem(LPCSTR pszName, int32_t value, sizeXX_t nArrayCount = 1);
	CIPVariable& SetChildItem(LPCSTR pszName, int64_t value, sizeXX_t nArrayCount = 1);
	CIPVariable& SetChildItem(LPCSTR pszName, uint32_t value, sizeXX_t nArrayCount = 1);
	CIPVariable& SetChildItem(LPCSTR pszName, uint64_t value, sizeXX_t nArrayCount = 1);
	CIPVariable& SetChildItem(LPCSTR pszName, double value, sizeXX_t nArrayCount = 1);
	CIPVariable& SetChildItem(LPCSTR pszName, const char* value, sizeXX_t nArrayCount = 1);
	CIPVariable& SetChildItem(LPCSTR pszName, const wchar_t* value, sizeXX_t nArrayCount = 1);
	CIPVariable& SetChildItem(LPCSTR pszName, const void* pData, int nSize, sizeXX_t nArrayCount = 1);	// 하나(pData)를 여러(nArrayCount)번 복사하는 루틴임.

	// 하나(T* pData)를 여러(nArrayCount)번 복사하는 루틴임.
	template < typename T >
	CIPVariable& SetChildItemUserType(LPCSTR pszName, const T* pData/*class 사용 금지, raw memory 만 복사함.*/, sizeXX_t nArrayCount = 1) {
		CIPVariable& varChild = SetChildItemEmpty(pszName, IPVT_BINARY, nArrayCount, sizeof(T));
		if (pData) {
			for (uintXX_t i = 0; i < nArrayCount; i++)
				memcpy( ((T*)(BYTE*)varChild.m_value) + i, pData, sizeof(T));
		}
		return varChild;
	}
	template < typename T >
	CIPVariable& SetChildItemT(LPCSTR pszName, const T& data/*class 사용 금지, raw memory 만 복사함.*/)	{
		CIPVariable& varChild = SetChildItemEmpty(pszName, IPVT_BINARY, 1, sizeof(data));
		*((T*)(BYTE*)varChild.m_value) = data;
		return varChild;
	}
	template < class MAT >
	CIPVariable& SetChildItemMat(LPCSTR pszName, const MAT& mat);
	CIPVariable& SetChildItem(LPCSTR pszName, DWORD value, sizeXX_t nArrayCount = 1)		{ return SetChildItem(pszName, (uint32_t)value, nArrayCount); }

	CIPVariable& AddChildItem(const CIPVariable& item) { CIPVariable& child = SetChildItemEmpty(item.GetName()); child = item; return child; }
	CIPVariable& AddChildItemEmpty(LPCSTR pszName = NULL)									{ return SetChildItemEmpty(pszName); }
	CIPVariable& AddChildItem(long value, LPCSTR pszName = NULL)							{ return SetChildItem(pszName, value); }
	CIPVariable& AddChildItem(int32_t value, LPCSTR pszName = NULL)							{ return SetChildItem(pszName, value); }
	CIPVariable& AddChildItem(int64_t value, LPCSTR pszName = NULL)							{ return SetChildItem(pszName, value); }
	CIPVariable& AddChildItem(uint32_t value, LPCSTR pszName = NULL)						{ return SetChildItem(pszName, value); }
	CIPVariable& AddChildItem(uint64_t value, LPCSTR pszName = NULL)						{ return SetChildItem(pszName, value); }
	CIPVariable& AddChildItem(double value, LPCSTR pszName = NULL)							{ return SetChildItem(pszName, value); }
	CIPVariable& AddChildItem(const char* pszValue, LPCSTR pszName = NULL)					{ return SetChildItem(pszName, pszValue); }
	CIPVariable& AddChildItem(const wchar_t* pwszValue, LPCSTR pszName = NULL)				{ return SetChildItem(pszName, pwszValue); }
	CIPVariable& AddChildItem(const void* pData, int nSize, LPCSTR pszName = NULL)			{ return SetChildItem(pszName, pData, nSize); }
	template < typename T >
	CIPVariable& AddChildItemUserType(const T* pData, LPCSTR pszName = NULL)				{ return SetChildItemUserType(pszName, pData); }
	template < class MAT >
	CIPVariable& AddChildItemMat(const MAT& mat, LPCSTR pszName = NULL)						{ return SetChildItemMat(pszName, mat); }
	CIPVariable& AddChildItem(DWORD value, LPCSTR pszName)									{ return SetChildItem(pszName, (uint32_t)value); }

	BOOL DeleteChildItem(int iChildIndex);
	BOOL DeleteChildItem(const char* szChildName);
	BOOL DeleteChildItem(const wchar_t* wszChildName);
	void DeleteAllChildItems();

	BOOL IsChildItem(int iChildIndex, eIP_VARIABLE_TYPE eType = IPVT_INT) const ;
	BOOL IsChildItem(const char* szChildName, eIP_VARIABLE_TYPE eType = IPVT_INT) const ;
	BOOL IsChildItem(const wchar_t* szChildName, eIP_VARIABLE_TYPE eType = IPVT_INT) const ;

	BOOL IsChildItemMat(int iChildIndex) const ;
	BOOL IsChildItemMat(const char* szChildName) const ;
	BOOL IsChildItemMat(const wchar_t* szChildName) const ;

	template < typename TCHAR > int FindChildIndex(const TCHAR* szChildName) const {
		DStringA strChildName(szChildName);
		for (int i = 0; i < m_child.N(); i++) {
			if (m_child[i].m_strName == strChildName)
				return i;
		}
		return -1;
	}
	int FindChildIndex(int iIndex) const {
		if (iIndex < 0)
			return iIndex;
		if (iIndex >= m_child.N())
			return -1;
		return iIndex;
	}
	CIPVariable* FindChild(const char* szChildName) {
#if (_MSC_VER >= 1800)
		for (auto& child : m_child)
#else
		for each (auto& child in m_child)
#endif
			if (child.m_strName == szChildName)
				return &child;
		return NULL;
	}
	const CIPVariable* FindChild(const char* szChildName) const {
#if (_MSC_VER >= 1800)
		for (const auto& child : m_child)
#else
		for each (const auto& child in m_child)
#endif
			if (child.m_strName == szChildName)
				return &child;
		return NULL;
	}

	// Child Item Iterater
	intXX_t GetChildCount() const { return m_child.N(); }
	CIPVariable& operator() (int iChildIndex);
	CIPVariable& operator() (const char* szChildName);
	CIPVariable& operator() (const wchar_t* szChildName);
	const CIPVariable& operator() (int iChildIndex) const;
	const CIPVariable& operator() (const char* szChildName) const;
	const CIPVariable& operator() (const wchar_t* wszChildName) const;
	TRefPointer<CIPVariable> GetChildRef(int iChildIndex);
	TRefPointer<CIPVariable> GetChildRef(const char* szChildName);
	TRefPointer<CIPVariable> GetChildRef(const wchar_t* szChildName);
	TConstRefPointer<CIPVariable> GetChildRef(int iChildIndex) const;
	TConstRefPointer<CIPVariable> GetChildRef(const char* szChildName) const;
	TConstRefPointer<CIPVariable> GetChildRef(const wchar_t* szChildName) const;

	int32_t				operator() (int iChildIndex,				int32_t iDefaultValue			) const;
	int64_t				operator() (int iChildIndex,				int64_t iDefaultValue64			) const;
	uint32_t	operator() (int iChildIndex,				uint32_t iDefaultValue	) const;
	uint64_t	operator() (int iChildIndex,				uint64_t iDefaultValue64) const;
	double				operator() (int iChildIndex,				double dDefaultValue			) const;
	const char*			operator() (int iChildIndex,				const char* pszDefaultValue		) const;
	const wchar_t*		operator() (int iChildIndex,				const wchar_t* pwszDefaultValue	) const;

	int32_t				operator() (const char* szChildName,		int32_t iDefaultValue			) const;
	int64_t				operator() (const char* szChildName,		int64_t iDefaultValue64			) const;
	uint32_t	operator() (const char* szChildName,		uint32_t iDefaultValue	) const;
	uint64_t	operator() (const char* szChildName,		uint64_t iDefaultValue64) const;
	double				operator() (const char* szChildName,		double dDefaultValue			) const;
	const char*			operator() (const char* szChildName,		const char* pszDefaultValue		) const;
	const wchar_t*		operator() (const char* szChildName,		const wchar_t* pwszDefaultValue	) const;

	int32_t				operator() (const wchar_t* szChildName,		int32_t iDefaultValue			) const;
	int64_t				operator() (const wchar_t* szChildName,		int64_t iDefaultValue64			) const;
	uint32_t	operator() (const wchar_t* szChildName,		uint32_t iDefaultValue	) const;
	uint64_t	operator() (const wchar_t* szChildName,		uint64_t iDefaultValue64) const;
	double				operator() (const wchar_t* szChildName,		double dDefaultValue			) const;
	const char*			operator() (const wchar_t* szChildName,		const char* pszDefaultValue		) const;
	const wchar_t*		operator() (const wchar_t* szChildName,		const wchar_t* pwszDefaultValue	) const;

	int32_t				GetChildItem(int iChildIndex,				int32_t iDefaultValue			) const;
	int64_t				GetChildItem(int iChildIndex,				int64_t iDefaultValue64			) const;
	uint32_t	GetChildItem(int iChildIndex,				uint32_t iDefaultValue	) const;
	uint64_t	GetChildItem(int iChildIndex,				uint64_t iDefaultValue64) const;
	double				GetChildItem(int iChildIndex,				double dDefaultValue			) const;
	const char*			GetChildItem(int iChildIndex,				const char* pszDefaultValue		) const;
	const wchar_t*		GetChildItem(int iChildIndex,				const wchar_t* pwszDefaultValue	) const;

	int32_t				GetChildItem(const char* szChildName,		int32_t iDefaultValue			) const;
	int64_t				GetChildItem(const char* szChildName,		int64_t iDefaultValue64			) const;
	uint32_t	GetChildItem(const char* szChildName,		uint32_t iDefaultValue	) const;
	uint64_t	GetChildItem(const char* szChildName,		uint64_t iDefaultValue64) const;
	double				GetChildItem(const char* szChildName,		double dDefaultValue			) const;
	const char*			GetChildItem(const char* szChildName,		const char* pszDefaultValue		) const;
	const wchar_t*		GetChildItem(const char* szChildName,		const wchar_t* pwszDefaultValue	) const;

	int32_t				GetChildItem(const wchar_t* szChildName,	int32_t iDefaultValue			) const;
	int64_t				GetChildItem(const wchar_t* szChildName,	int64_t iDefaultValue64			) const;
	uint32_t	GetChildItem(const wchar_t* szChildName,	uint32_t iDefaultValue	) const;
	uint64_t	GetChildItem(const wchar_t* szChildName,	uint64_t iDefaultValue64) const;
	double				GetChildItem(const wchar_t* szChildName,	double dDefaultValue			) const;
	const char*			GetChildItem(const wchar_t* szChildName,	const char* pszDefaultValue		) const;
	const wchar_t*		GetChildItem(const wchar_t* szChildName,	const wchar_t* pwszDefaultValue	) const;

	template < typename T > BOOL GetChildItemUserType(int iChildIndex, T* pData/*class 사용 금지, raw memory 만 복사함.*/) const;
	template < typename T > BOOL GetChildItemUserType(const char* pszName, T* pData/*class 사용 금지, raw memory 만 복사함.*/) const;
	template < typename T > BOOL GetChildItemUserType(const wchar_t* pszName, T* pData/*class 사용 금지, raw memory 만 복사함.*/) const;
	template < typename T > BOOL GetChildItemT(const char* pszName, T& data/*class 사용 금지, raw memory 만 복사함.*/) const;
	template < typename T > BOOL GetChildItemT(const wchar_t* pszName, T& data/*class 사용 금지, raw memory 만 복사함.*/) const;
	template < class MAT > BOOL GetChildItemMat(int iChildIndex, MAT& mat) const;
	template < class MAT > BOOL GetChildItemMat(const char* szChildName, MAT& mat) const;
	template < class MAT > BOOL GetChildItemMat(const wchar_t* szChildName, MAT& mat) const;

public:
	// Serialize
	CArchive& StoreTo(CArchive& ar) const;
	CArchive& LoadFrom(CArchive& ar);

	friend CArchive& operator << (CArchive& ar, const CIPVariable& var) { return var.StoreTo(ar); }
	friend CArchive& operator >> (CArchive& ar, CIPVariable& var) { return var.LoadFrom(ar); }
};

template < typename MAT >
BOOL CIPVariable::SetMat(const MAT& mat) {
	DeleteAllChildItems();
	if (mat.empty())
		return FALSE;
	SetChildItem("rows", (int)mat.rows);
	SetChildItem("cols", (int)mat.cols);
	SetChildItem("type", (int)mat.type());
	if (mat.isContinuous()) {
		int nSize = (int)(mat.step * mat.rows);
		if (nSize > 0) {
			SetChildItem("data", (const void*)mat.data, nSize, 1);
		}
	} else {
		int nRowSize = (int)(mat.cols*mat.elemSize());
		int nSize = (int)(nRowSize * mat.rows);
		if (nSize > 0) {
			TBuffer<BYTE> buf(nSize);
			BYTE* pos = buf;
			for (int y = 0; y < mat.rows; y++, pos += nRowSize) {
				memcpy(pos, mat.ptr(y), nRowSize);
			}
			SetChildItem("data", (const void*)buf, buf.GetSize(), 1);
		}
	}
	return TRUE;
}

template < typename MAT >
BOOL CIPVariable::GetMat(MAT& mat) const {
	mat.release();

	if (!IsTypeMat())
		return FALSE;
	int nRow = GetChildItem("rows", 0);
	int nCol = GetChildItem("cols", 0);
	int nType = GetChildItem("type", 0);

	if ( (nRow <= 0) || (nCol <= 0) ) {
		return FALSE;
	}

	try {
		mat = mat.zeros(nRow, nCol, nType);
	} catch(...) {
		return FALSE;
	}

	int iIndex = FindChildIndex("data");
	if ( (iIndex >= 0) && (m_child[iIndex].GetType() == IPVT_BINARY) ) {
		const CIPVariable& var = m_child[iIndex];
		int nSize = var.GetSize();
		int nRowSize = (int)(mat.cols*mat.elemSize());
		if ( (nSize >= 0) && (nRowSize > 0) ) {
			const BYTE* p0 = (const BYTE*)var.GetRawData();
			const BYTE* pos = p0;
			for (int y = 0; y < mat.rows; y++, pos += nRowSize) {
				if (pos - p0 + nRowSize > (sizeXX_t)nSize)
					break;
				memcpy(mat.ptr(y), pos, mat.step);
			}
		}
	}
	return TRUE;
}

template < typename T > BOOL CIPVariable::GetChildItemUserType(int iChildIndex, T* pData) const {
	if ( (iChildIndex < 0) || (iChildIndex >= m_child.N()) )
		return FALSE;
	if (m_child[iChildIndex].m_value.GetSize() < sizeof(*pData))
		return FALSE;
	return (*this)(iChildIndex).GetRawData(pData, sizeof(*pData));
}
template < typename T > BOOL CIPVariable::GetChildItemUserType(const char* pszName, T* pData) const		{ return GetChildItemUserType(FindChildIndex(pszName), pData); }
template < typename T > BOOL CIPVariable::GetChildItemUserType(const wchar_t* pszName, T* pData) const	{ return GetChildItemUserType(FindChildIndex(pszName), pData); }
template < typename T > BOOL CIPVariable::GetChildItemT(const char* pszName, T& data) const				{ return GetChildItemUserType(FindChildIndex(pszName), &data); }
template < typename T > BOOL CIPVariable::GetChildItemT(const wchar_t* pszName, T& data) const			{ return GetChildItemUserType(FindChildIndex(pszName), &data); }

template < class MAT > CIPVariable& CIPVariable::SetChildItemMat(LPCSTR pszName, const MAT& mat) {
	CIPVariable& var = SetChildItemEmpty(pszName);
	var.SetMat(mat);
	return var;
}
template < class MAT > BOOL CIPVariable::GetChildItemMat(int iChildIndex, MAT& mat) const				{ return ((*this)(iChildIndex)).GetMat(mat); }
template < class MAT > BOOL CIPVariable::GetChildItemMat(const char* szChildName, MAT& mat) const		{ return ((*this)(szChildName)).GetMat(mat); }
template < class MAT > BOOL CIPVariable::GetChildItemMat(const wchar_t* szChildName, MAT& mat) const	{ return ((*this)(szChildName)).GetMat(mat); }

#pragma pack(pop)
