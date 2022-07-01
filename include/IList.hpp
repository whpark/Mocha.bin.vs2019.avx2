///////////////////////////////////////////////////////////////////////////////
//
// IList.h: part of TList.hpp
//
//
// PWH
///////////////////////////////////////////////////////////////////////////////

#pragma once

#pragma intrinsic(_InterlockedIncrement, _InterlockedDecrement)

#include <atomic>

#ifndef assert1
	#ifdef _MFC_VER
		#define assert1 ASSERT
	#else
		#include <assert.h>
		#define assert1 assert
		#ifndef ASSERT
			#define ASSERT assert
		#endif
	#endif
#endif

#pragma pack(push, 8)

//=============================================================================

class CListException : public __ExceptionBase__ {
public:
	CListException(const char* pszPosition, const char* pszMessage) : __ExceptionBase__(pszPosition, pszMessage) { }
};

//=============================================================================
// INTERFACE CLASS
//

class IListLocker;
template < class T, class TP, class TPC, typename INDEX_T, typename SIZE_T > class IListIter;
template < class T, class TP, class TPC, typename INDEX_T, typename SIZE_T > class IListDefaultData;
template < class T, class TP, class TPC, typename INDEX_T, typename SIZE_T, class TLISTITER > class IList;

//-----------------------------------------------------------------------------
// IListLocker
//

class IListLocker {
public:
	mutable std::atomic<long> m_nInterlockCounter;	// number of iterator.
	mutable CCriticalSection* m_pCS;
	//mutable volatile long m_nLockCounter;		// number of not-released Lock() calls. for Debuging only.

public:
	class Locker {
		CCriticalSection* pCS;
	public:
		Locker(const IListLocker& B) : pCS(B.m_pCS) {
			__List_check_exception__(B.m_nInterlockCounter <= 0, "Interlock Counter > 0. Cannot use Insert()/Remove() function call, while using ListIter.");
			if (pCS) pCS->Lock();
		}
		template < class T, class TP, class TPC, typename INDEX_T, typename SIZE_T >
		Locker(const IListIter< T, TP, TPC, INDEX_T, SIZE_T >& rListIter);
		Locker(const Locker& B) : pCS(B.pCS) {}
		~Locker() { if (pCS) pCS->Unlock(); }
	};

public:
	explicit IListLocker(bool bInterlockedMode)			: m_nInterlockCounter(-1), m_pCS(NULL)/*, m_nLockCounter(0)*/ { SetInterlockMode(bInterlockedMode); }
	explicit IListLocker(CCriticalSection* pCS = NULL)	: m_nInterlockCounter(-1), m_pCS(pCS) /*, m_nLockCounter(0)*/ { }
private:
	IListLocker(const IListLocker& B) {}

public:
	void Lock(void)		{ if (m_pCS) { /*::_InterlockedIncrement(&m_nLockCounter);*/ m_pCS->Lock(); } }
	void Unlock(void)	{ if (m_pCS) { /*::_InterlockedDecrement(&m_nLockCounter);*/ m_pCS->Unlock(); } }

	int GetInterlockCounter() const	{ return m_nInterlockCounter; }
	//int GetLockCounter() const		{ return m_nLockCounter; }

public:
	bool IsInterlockedMode() const { return m_pCS ? true : false; }
	void SetInterlockMode(bool bInterlockedMode = true) {
		if (bInterlockedMode) {
			if (!m_pCS)
				m_pCS = new CCriticalSection;
		} else {
			CCriticalSection* pCS = m_pCS;
			m_pCS = NULL;
			if (pCS)
				delete pCS;
		}
	}
};



//-----------------------------------------------------------------------------
// IListIter
//
template < class T, class TP, class TPC, typename INDEX_T, typename SIZE_T >
class IListIter {
	friend class IListLocker;
	friend class IList< T, TP, TPC, INDEX_T, SIZE_T, IListIter< T, TP, TPC, INDEX_T, SIZE_T > >;
protected:
	SIZE_T m_nItem;				// virtual function 을 사용할까, Pointer를 쓸까 고민하다가 걍 pointer 사용함. function 보다는 빠를 것 같음.
	mutable INDEX_T m_iCurrent;

	std::atomic<long>* m_pInterlockCounter{};		// virtual function 을 사용할까, Pointer를 쓸까 고민하다가 걍 pointer 사용함. Constructor/Destructor 에서 virtual function 을 호출하는 건 위험함.
	CCriticalSection* m_pCSChild;

protected:
	IListIter(SIZE_T nItem, std::atomic<long>* pInterlockCounter, CCriticalSection* pCS)
		: m_nItem(nItem), m_iCurrent(0), m_pInterlockCounter(pInterlockCounter), m_pCSChild(pCS)
	{
		if (m_pInterlockCounter)
			m_pInterlockCounter->fetch_add(1);
	}
public:
	IListIter(const IListIter& B) : m_nItem(0), m_iCurrent(0), m_pInterlockCounter(NULL), m_pCSChild(NULL) {
		*this = B;
	}
	IListIter& operator = (const IListIter& B) {
		Release();
		m_nItem = B.m_nItem;
		m_pInterlockCounter = B.m_pInterlockCounter;
		if (m_pInterlockCounter) {
			m_pInterlockCounter->fetch_add(1);
		}
		m_iCurrent = B.m_iCurrent;
		return *this;
	}

	//template < class TLISTITER >
	//IListIter(const TLISTITER& B) : m_nItem(0), m_pInterlockCounter(NULL), m_iCurrent(0), m_pCSChild(NULL) {
	//	*this = B;
	//}
	//template < class TLISTITER >
	//IListIter& operator = (const TLISTITER& B) {
	//	Release();
	//	//m_nItem = B.m_nItem;
	//	//m_pInterlockCounter = B.m_pInterlockCounter;
	//	//if (m_pInterlockCounter) {
	//	//	_InterlockedIncrement(m_pInterlockCounter);
	//	//}
	//	//m_iCurrent = B.m_iCurrent;
	//	return *this;
	//}

	virtual ~IListIter() {
		Release();
	}

	// do not make Relase() virtual.
	void Release() {
		if (m_pInterlockCounter)
			m_pInterlockCounter->fetch_sub(1);

		m_nItem = 0;
		m_iCurrent = 0;

		m_pCSChild = NULL;
		m_pInterlockCounter = NULL;
	}

public:
	//-----------------------
	// Item Count & Index. vector 에서 size() 를 사용하므로, 이것도 추가..
	//
	inline SIZE_T N(void) const							{ return m_nItem; }
	inline SIZE_T size(void) const						{ return m_nItem; }
	inline SIZE_T GetItemCount() const					{ return m_nItem; }
	inline bool IsEmpty(void) const						{ return !m_nItem; }
	inline bool empty(void) const						{ return !m_nItem; }
	inline INDEX_T Index(void) const					{ return m_iCurrent; }

	//-----------------------
	// Iterator
	inline T&		operator () (void)					{ TP*  ppData = GetPPItem();		__List_check_exception__(ppData && *ppData,	"Memory Error"); return **ppData; }
	inline const T&	operator () (void) const			{ TPC* ppData = GetPPItem();		__List_check_exception__(ppData && *ppData,	"Memory Error"); return **ppData; }
	inline T&		operator [] (INDEX_T iIndex)		{ __List_check_exception__((iIndex>=0)&&(iIndex<m_nItem), "IndexError"); TP*  ppData = GetPPItem(iIndex);	__List_check_exception__(ppData && *ppData,	"Memory Error"); return **ppData; }
	inline const T&	operator [] (INDEX_T iIndex) const	{ __List_check_exception__((iIndex>=0)&&(iIndex<m_nItem), "IndexError"); TPC* ppData = GetPPItem(iIndex);	__List_check_exception__(ppData && *ppData,	"Memory Error"); return **ppData; }
	inline TP		operator () (INDEX_T iIndex)		{ __List_check_exception__((iIndex>=0)&&(iIndex<m_nItem), "IndexError"); TP*  ppData = GetPPItem(iIndex);	__List_check_exception__(ppData,			"Memory Error"); return  *ppData; }
	inline TPC		operator () (INDEX_T iIndex) const	{ __List_check_exception__((iIndex>=0)&&(iIndex<m_nItem), "IndexError"); TPC* ppData = GetPPItem(iIndex);	__List_check_exception__(ppData,			"Memory Error"); return  *ppData; }

	// First()/Last(), Current(), Next()/Previous()
	// do as singly-linked list
	virtual bool	NextNotNull(void) const				{ return (m_iCurrent >= 0) && (m_iCurrent < size()-1); }
	virtual TP		First(void)							{ return GetItem(0); }
	virtual TP		Last(void)							{ return GetItem(-1); }
	virtual TP		Current(void)						{ return GetItem(); }
	virtual TPC		First(void) const					{ return GetItem(0); }
	virtual TPC		Last(void) const					{ return GetItem(-1); }
	virtual TPC		Current(void) const					{ return GetItem(); }
	virtual TPC		Next(void) const					{ return ((IListIter*)this)->Next(); }
	virtual TPC		Previous(void) const				{ return ((IListIter*)this)->Previous(); }
	virtual TP		Next(void) {
		__Lock_List__(*this);
		if (!size())
			return TP(NULL);
		if ((m_iCurrent >= 0) && (m_iCurrent == size()-1)) {
			SetNullPosition();
			return TP(NULL);
		}
		TP* ppData = GetPPItem(m_iCurrent+1);
		return ppData ? *ppData : TP(NULL);
	}
	virtual TP		Previous(void) {
		__Lock_List__(*this);
		if (!size())
			return TP(NULL);
		if (m_iCurrent == 0) {
			SetNullPosition();
			return TP(NULL);
		}
		TP* ppData = GetPPItem(m_iCurrent-1);
		return ppData ? *ppData : TP(NULL);
	}
	virtual void SetNullPosition() const {
		//__Lock_List__(*this);
		m_iCurrent = 0;
	}

public:
	// Set/Get Item
	inline TP	SetItem(TP pT)							{ TP* ppData = GetPPItem();       __List_check_exception__(ppData, "Current Data is NULL"); return *ppData = pT; }
	inline TP	SetItem(INDEX_T iIndex, TP pT)			{ TP* ppData = GetPPItem(iIndex); __List_check_exception__(ppData, "Current Data is NULL"); return *ppData = pT; }

public:
	inline TP	GetItem(void)							{ TP* ppData = GetPPItem();			return ppData ? *ppData : TP(NULL); }
	inline TP	GetItem(INDEX_T iIndex)					{ TP* ppData = GetPPItem(iIndex);	return ppData ? *ppData : TP(NULL); }
	inline TPC	GetItem(void) const						{ TPC* ppData = GetPPItem();		return ppData ? *ppData : TPC(NULL); }
	inline TPC	GetItem(INDEX_T iIndex) const			{ TPC* ppData = GetPPItem(iIndex);	return ppData ? *ppData : TPC(NULL); }

	template < typename T2 >
	T2&			GetRef(void)							{ TP* ppData = GetPPItem();			__List_check_exception__(ppData, "Current Data is NULL"); T* pData = *ppData; T2* p = dynamic_cast<T2*>(pData); __List_check_exception__(p, "Cannot Convert Type"); return *p; }
	template < typename T2 >
	const T2&	GetRef(void) const 						{ TP* ppData = GetPPItem();			__List_check_exception__(ppData, "Current Data is NULL"); T* pData = *ppData; T2* p = dynamic_cast<T2*>(pData); __List_check_exception__(p, "Cannot Convert Type"); return *p; }
	template < typename T2 >
	T2&			GetRef(INDEX_T iIndex)					{ TP* ppData = GetPPItem(iIndex);	__List_check_exception__(ppData, "Current Data is NULL"); T* pData = *ppData; T2* p = dynamic_cast<T2*>(pData); __List_check_exception__(p, "Cannot Convert Type"); return *p; }
	template < typename T2 >
	const T2&	GetRef(INDEX_T iIndex) const			{ TP* ppData = GetPPItem(iIndex);	__List_check_exception__(ppData, "Current Data is NULL"); T* pData = *ppData; T2* p = dynamic_cast<T2*>(pData); __List_check_exception__(p, "Cannot Convert Type"); return *p; }

protected:
	virtual TP*		GetPPItem()					    = NULL;
	virtual TPC*	GetPPItem() const			    = NULL;
	virtual TP*		GetPPItem(INDEX_T iIndex)	    = NULL;
	virtual TPC*	GetPPItem(INDEX_T iIndex) const = NULL;


public:
	//-----------------------
	// Find : iStartAfter - Starts from
	// Search : Same as Find, buts returns TP, not index.
//#define __LIST_FIND__(expr) \
//	__Lock_List__(*this);\
//	if (m_nItem <= iStartAfter)\
//		return -1;\
//	if (iStartAfter < -1) iStartAfter = -1;\
//	int n = m_nItem;\
//	for (iStartAfter++; iStartAfteri < n; iStartAfter++)\
//		if (expr) return iStartAfter++;\
//	return -1;
//
//	INDEX_T Find(const T& B, INDEX_T& iStartAfter = INDEX_T_(-1)) const				{ __LIST_FIND__(**GetPPItem(iStarAfter) == B) }
//	INDEX_T Find(const T* pB, INDEX_T& iStartAfter = INDEX_T_(-1)) const				{ __LIST_FIND__((const T*)*GetPPItem(iStarAfter) == pB) }
//	template <typename TYPE>
//	INDEX_T FindByValue(const TYPE& B, INDEX_T& iStartAfter = INDEX_T_(-1)) const	{ __LIST_FIND__(**GetPPItem(iStarAfter) == B) }
//	template <class Compare>
//	INDEX_T FindL(Compare compare, INDEX_T& iStartAfter = INDEX_T_(-1)) const		{ __LIST_FIND__( compare(**GetPPItem(iStarAfter)) ) }
//#undef __LSIT_FIND__

	struct INDEX_T_ {
		INDEX_T value;
		INDEX_T_(INDEX_T v) : value(v) {}
		operator INDEX_T& () { return value; }
	};


	template < class Compare_F >
	INDEX_T TFind(Compare_F compare_f, INDEX_T& iStartAfter) const {
		__Lock_List__(*this);
		if (m_nItem <= iStartAfter)
			return -1;
		if (iStartAfter < -1) iStartAfter = -1;
		int n = m_nItem;
		for (iStartAfter++; iStartAfter < n; iStartAfter++) {
			if (compare_f(iStartAfter))
				return iStartAfter;
		}
		return -1;
	}
	INDEX_T Find(const T& B, INDEX_T& iStartAfter = INDEX_T_(-1)) const				{ return TFind([&](INDEX_T iIndex) { return (**GetPPItem(iIndex) == B); }, iStartAfter); }
	INDEX_T Find(const T* pB, INDEX_T& iStartAfter = INDEX_T_(-1)) const			{ return TFind([&](INDEX_T iIndex) { return ((const T*)*GetPPItem(iIndex) == pB); }, iStartAfter); }
	template <typename TYPE>
	INDEX_T FindByValue(const TYPE& B, INDEX_T& iStartAfter = INDEX_T_(-1)) const	{ return TFind([&](INDEX_T iIndex) { return (**GetPPItem(iIndex) == B); }, iStartAfter); }
	template <class Compare>
	INDEX_T FindL(Compare compare, INDEX_T& iStartAfter = INDEX_T_(-1)) const		{ return TFind([&](INDEX_T iIndex) { return compare(**GetPPItem(iIndex)); }, iStartAfter); }

//#define __LIST_SEARCH__(expr) \
//	__Lock_List__(*this);\
//	if (m_nItem <= iStartAfter)\
//		return NULL;\
//	if (iStartAfter < -1) iStartAfter = -1;\
//	int n = m_nItem;\
//	for (iStartAfter++; iStartAfter < n; iStartAfter++)\
//		if (expr) return *GetPPItem(iStartAfter);\
//	return NULL;
//
//	TP	Search(const T& B, INDEX_T& iStartAfter = INDEX_T_(-1))						{ __LIST_SEARCH__(**GetPPItem(iStartAfter) == B) }
//	TPC	Search(const T& B, INDEX_T& iStartAfter = INDEX_T_(-1)) const				{ __LIST_SEARCH__(**GetPPItem(iStartAfter) == B) }
//	TP	Search(const T* pB, INDEX_T& iStartAfter = INDEX_T_(-1))					{ __LIST_SEARCH__((const T*)*GetPPItem(iStartAfter) == pB) }
//	TPC	Search(const T* pB, INDEX_T& iStartAfter = INDEX_T_(-1)) const				{ __LIST_SEARCH__((const T*)*GetPPItem(iStartAfter) == pB) }
//	template <typename TYPE>
//	TP	SearchByValue(const TYPE& B, INDEX_T& iStartAfter = INDEX_T_(-1))			{ __LIST_SEARCH__(**GetPPItem(iStartAfter) == B) }
//	template <typename TYPE>
//	TPC	SearchByValue(const TYPE& B, INDEX_T& iStartAfter = INDEX_T_(-1)) const		{ __LIST_SEARCH__(**GetPPItem(iStartAfter) == B) }
//	template <class Compare>
//	TP SearchL(Compare compare, INDEX_T& iStartAfter = INDEX_T_(-1))				{ __LIST_SEARCH__( compare(**GetPPItem(iStartAfter)) ) }
//	template <class Compare>
//	TPC SearchL(Compare compare, INDEX_T& iStartAfter = INDEX_T_(-1)) const			{ __LIST_SEARCH__( compare(**GetPPItem(iStartAfter)) ) }
//#undef __LIST_SEARCH__
	template < class Compare_F >
	TP TSearch(Compare_F compare_f, INDEX_T& iStartAfter) {
		__Lock_List__(*this);
		if (m_nItem <= iStartAfter)
			return nullptr;
		if (iStartAfter < -1) iStartAfter = -1;
		int n = m_nItem;
		for (iStartAfter++; iStartAfter < n; iStartAfter++) {
			if (compare_f(iStartAfter))
				return *GetPPItem(iStartAfter);
		}
		return nullptr;
	}
	template < class Compare_F >
	TPC TSearch(Compare_F compare_f, INDEX_T& iStartAfter) const {
		__Lock_List__(*this);
		if (m_nItem <= iStartAfter)
			return nullptr;
		if (iStartAfter < -1) iStartAfter = -1;
		int n = m_nItem;
		for (iStartAfter++; iStartAfter < n; iStartAfter++) {
			if (compare_f(iStartAfter))
				return *GetPPItem(iStartAfter);
		}
		return nullptr;
	}

	TP	Search(const T& B, INDEX_T& iStartAfter = INDEX_T_(-1))						{ return TSearch([&](INDEX_T iIndex) { return **GetPPItem(iIndex) == B; }, iStartAfter); }
	TPC	Search(const T& B, INDEX_T& iStartAfter = INDEX_T_(-1)) const				{ return TSearch([&](INDEX_T iIndex) { return **GetPPItem(iIndex) == B; }, iStartAfter); }
	TP	Search(const T* pB, INDEX_T& iStartAfter = INDEX_T_(-1))					{ return TSearch([&](INDEX_T iIndex) { return (const T*)*GetPPItem(iIndex) == pB; }, iStartAfter); }
	TPC	Search(const T* pB, INDEX_T& iStartAfter = INDEX_T_(-1)) const				{ return TSearch([&](INDEX_T iIndex) { return (const T*)*GetPPItem(iIndex) == pB; }, iStartAfter); }
	template <typename TYPE>
	TP	SearchByValue(const TYPE& B, INDEX_T& iStartAfter = INDEX_T_(-1))			{ return TSearch([&](INDEX_T iIndex) { return **GetPPItem(iIndex) == B; }, iStartAfter); }
	template <typename TYPE>
	TPC	SearchByValue(const TYPE& B, INDEX_T& iStartAfter = INDEX_T_(-1)) const		{ return TSearch([&](INDEX_T iIndex) { return **GetPPItem(iIndex) == B; }, iStartAfter); }
	template <class Compare>
	TP SearchL(Compare compare, INDEX_T& iStartAfter = INDEX_T_(-1))				{ return TSearch([&](INDEX_T iIndex) { return compare(**GetPPItem(iIndex)); }, iStartAfter); }
	template <class Compare>
	TPC SearchL(Compare compare, INDEX_T& iStartAfter = INDEX_T_(-1)) const			{ return TSearch([&](INDEX_T iIndex) { return compare(**GetPPItem(iIndex)); }, iStartAfter); }
#undef __LIST_SEARCH__

	//-----------------------
	// CopyTo,    AddTo    : copy objects to B.
	// CopyRefTo, AddRefTo : copy Pointers or RefPointers.
	// CopyTo, CopyRefTo   : 사용 주의. MultiThread에서, 두 개의 리스트 모두에 Insert/Delete 를 동시에 할 경우 DeadLock 걸릴 수 있음.
	template < class TLIST >
	bool CopyTo(TLIST& B) const {			// 주의! DeadLock 걸릴 수 있음....
		__Lock_List__(*this);
		__if_exists(TLIST::Lock) { B.Lock(); }
		B.clear();
		bool bResult = AddTo(B);
		__if_exists(TLIST::Unlock) { B.Unlock(); }
		return bResult;
	}
	template < class TLIST >
	bool CopyRefTo(TLIST& B) const {		// 주의! DeadLock 걸릴 수 있음....
		__Lock_List__(*this);
		__if_exists(TLIST::Lock) { B.Lock(); }
		B.clear();
		bool bResult = AddRefTo(B);
		__if_exists(TLIST::Unlock) { B.Unlock(); }
		return bResult;
	}

	template < class TLIST >
	bool AddTo(TLIST& B) const {
		__Lock_List__(*this);

		if (!m_nItem)
			return true;

		__if_not_exists(TLIST::pvalue_type::Release) {
		__if_exists(TLIST::GetAutoDelete) {
			if (!B.GetAutoDelete()) {
				if (B.size())
					return false;
				else
					B.AutoDelete(true);
			}
		}
		}

		GetPPItem(0);
		for (INDEX_T i = 0; i < m_nItem; i++, Next()) {
			__if_exists(T::NewClone) { B.Push(Current()->NewClone()); }
			__if_not_exists(T::NewClone) { B.Push(new T(*Current())); }
		}
		return true;
	}
	template < class TLIST >
	bool AddRefTo(TLIST& B) const {
		__Lock_List__(*this);

		if (!m_nItem)
			return true;

		__if_not_exists(TLIST::pvalue_type::Release) {
		__if_exists(TLIST::GetAutoDelete) {
			if (B.GetAutoDelete()) {
				if (B.size())
					return false;
				else
					B.AutoDelete(false);
			}
		}
		}

		GetPPItem(0);
		for (INDEX_T i = 0; i < m_nItem; i++, Next()) {
			B.Push(Current());
		}
		return true;
	}


	//-----------------------
	// List -> vector
	//
#define __LIST_GET_VECTOR__(expr)\
		__Lock_List__(*this);\
		vec.clear();\
		int nItem = m_nItem;\
		if (!nItem)\
			return true;\
		if ( (iStart < 0) || (iStart >= nItem) )\
			return false;\
		if ( (n == (SIZE_T)-1) || (n > nItem-iStart) )\
			n = nItem - iStart;\
		vec.reserve(n);\
		for (INDEX_T i = 0; i < n; i++)\
			vec.push_back(expr);\
		return true;

	bool GetVector(std::vector<T>& vec, INDEX_T iStart = 0, SIZE_T n = (SIZE_T)-1) const	{ __LIST_GET_VECTOR__(**GetPPItem(i+iStart)) }
	bool GetVector(std::vector<TP>& vec, INDEX_T iStart = 0, SIZE_T n = (SIZE_T)-1)			{ __LIST_GET_VECTOR__(*GetPPItem(i+iStart)) }
	bool GetVector(std::vector<TPC>& vec, INDEX_T iStart = 0, SIZE_T n = (SIZE_T)-1) const	{ __LIST_GET_VECTOR__(*GetPPItem(i+iStart)) }
#undef __LIST_GET_VECTOR__

};


template < class T, class TP, class TPC, typename INDEX_T, typename SIZE_T >
IListLocker::Locker::Locker(const IListIter< T, TP, TPC, INDEX_T, SIZE_T >& rListIter) : pCS(rListIter.m_pCSChild) {
	if (pCS) pCS->Lock();
}


//-----------------------------------------------------------------------------
// IListDefaultData
//

template < class T, class TP, class TPC, typename INDEX_T, typename SIZE_T >
class IListDefaultData : public IListLocker {
	friend class IList< T, TP, TPC, INDEX_T, SIZE_T, IListIter< T, TP, TPC, INDEX_T, SIZE_T > >;
public:
	typedef T value_type;
	typedef TP pvalue_type;
protected:
	bool m_bAutoDeleteObject;
	char buf[3]; // padding

public:
	IListDefaultData(bool bAutoDeleteObject = true, bool bInterlockedMode = false) : IListLocker(bInterlockedMode), m_bAutoDeleteObject(bAutoDeleteObject) {
	}

	IListDefaultData(const IListDefaultData& B) : IListLocker(B.IsInterlockedMode()), m_bAutoDeleteObject(true) {
		*this = B;
	}

	IListDefaultData& operator = (const IListDefaultData& B) {
		if (this == &B)
			return *this;
		//SetInterlockMode(B.IsInterlockedMode());
		//AutoDelete(B.m_bAutoDeleteObject);
		return *this;
	}

	template < class TLIST >
	IListDefaultData(const TLIST& B) : IListLocker(B.IsInterlockedMode()), m_bAutoDeleteObject(true) {
		*this = B;
	}
	template < class TLIST >
	IListDefaultData& operator = (const TLIST& B) {
		if ((void*)this == (void*)&B)
			return *this;
		//__if_exists(TLIST::GetAutoDelete) {
		//	AutoDelete(bAutoDeleteObject);
		//}
		//__if_exists(TLIST::IsInterlockMode) {
		//	SetInterlockMode(B.IsInterlockedMode());
		//}
		return *this;
	}

	virtual ~IListDefaultData() {
		SetInterlockMode(false);
	}

	//-----------------------
	// mode
	//
	bool GetAutoDelete(void) const					{ return m_bAutoDeleteObject; }
	void AutoDelete(bool bAutoDeleteObject = true)	{ __if_exists(T::Release) { m_bAutoDeleteObject = false; } __if_not_exists(T::Release) { m_bAutoDeleteObject = bAutoDeleteObject; } }
};


//-----------------------------------------------------------------------------
// IList
//
template < class T, class TP, class TPC, typename INDEX_T, typename SIZE_T, class TLISTITER >
class IList : public IListDefaultData< T, TP, TPC, INDEX_T, SIZE_T >, public TLISTITER {
public:
	typedef IListDefaultData< T, TP, TPC, INDEX_T, SIZE_T > parent_t;
	typedef TLISTITER iter_t;
	typedef T type_t;
//	IList(bool bAutoDeleteObject = true, bool bInterlockedMode = false) : IListDefaultData(bAutoDeleteObject, bInterlockedMode), TLISTITER(m_nItem, &m_nInterlockCounter, m_pCS) { }
	IList(bool bAutoDeleteObject, bool bInterlockedMode, void* pData)
		: parent_t(bAutoDeleteObject, bInterlockedMode),
		TLISTITER(pData, 0, &(parent_t::m_nInterlockCounter), parent_t::m_pCS) { }

	IList(const IList& B) : IListDefaultData(B), TLISTITER(B) {
		*this = B;
	}
	IList(const IList& B, void* pData, SIZE_T nItem, std::atomic<long>* pInterlockCounter, CCriticalSection* pCS)
		: IListDefaultData(B), TLISTITER(pData, nItem, pInterlockCounter, pCS) { *this = B; }

	IList& operator = (const IList& B) {
		if (this == &B)
			return *this;

		(IListDefaultData&)*this = (IListDefaultData&)B;
		(TLISTITER&)*this = (TLISTITER&)B;

		//SetInterlockMode(B.IsInterlockedMode());
		//AutoDelete(B.m_bAutoDeleteObject);
		return *this;
	}

	//template < class TLIST >
	//IList(const TLIST& B) : IListDefaultData(B), TLISTITER() { *this = B; }

	//template < class TLIST >
	//IList& operator = (const TLIST& B) {
	//	if ((void*)this == (void*)&B)
	//		return *this;

	//	//(IListDefaultData&)*this = B;

	//	//(TLISTITER&)*this = B;

	//	//__if_exists(TLIST::GetAutoDelete) {
	//	//	AutoDelete(bAutoDeleteObject);
	//	//}
	//	//__if_exists(TLIST::IsInterlockMode) {
	//	//	SetInterlockMode(B.IsInterlockedMode());
	//	//}
	//	return *this;
	//}

	virtual ~IList() {
	}

	//-----------------------
	// Compare
	template < class TLIST >
	bool operator == (const TLIST& B) const { return Compare(B); }

	template < class TLIST >
	bool Compare(const TLIST& B) const {
		__Lock_List__(*this);

		if (iter_t::m_nItem != B.size())
			return false;
		for (INDEX_T i = 0; i < (INDEX_T)B.size(); i++) {
			if (**iter_t::GetPPItem(i) == B[i])
				;
			else
				return false;
		}
		return true;
	}

	//-----------------------
	// Iserts
	//-----------------------
	// iIndex :
	//    -2 : current
	//    -1 : Last
public:
	virtual INDEX_T InsertAt(TPC pData, INDEX_T iIndex = -2) = NULL;		// pure virtual

	INDEX_T Attach(TPC pData)			{ return InsertAt(pData, -1); }
	INDEX_T Push(TPC pData)				{ return InsertAt(pData, -1); }
	INDEX_T Insert(TPC pData)			{ return InsertAt(pData, -2); }
	INDEX_T InsertPrevious(TPC pData)	{ __Lock_List__(*this); INDEX_T iIndex = iter_t::m_iCurrent-1; if (iIndex < 0) iIndex = 0; return InsertAt(pData, iIndex); }

	//-----------------------
	// Remove/Delete
	//-----------------------
	// iIndex :
	//    -2 : current
	//    -1 : Last
protected:
	inline void DeleteObjectOnly(TP& pData) {
		if (!pData) return;
		__if_exists(TP::Release) { pData.Release(); }
		__if_not_exists(TP::Release) { delete pData; pData = NULL; }
	}
protected:
	virtual TP RemoveAt(INDEX_T iIndex) = NULL;// pure virtual
			// iIndex :
			//    -2 : current
			//    -1 : Last
	virtual bool DeleteAt(INDEX_T iIndex) {
		__Lock_List__(*this);
		TP pData = RemoveAt(iIndex);
		if (!pData)
			return false;
		if (parent_t::m_bAutoDeleteObject)
			DeleteObjectOnly(pData);
		return true;
	}

public:
	TP	 Pop(void)						{ return RemoveAt(-1); }
	TP   PopFirst(void)					{ return RemoveAt(0); }
	TP   Pop(INDEX_T iIndex)			{ return RemoveAt(iIndex); }
	TP   Pop(const T* pData)			{ __Lock_List__(*this); int iIndex = iter_t::Find(pData); if (iIndex < 0) return TP(NULL); return RemoveAt(iIndex); }
	TP   Remove(void)					{ return RemoveAt(-2); }
	TP   RemoveFirst(void)				{ return RemoveAt(0); }
	TP   RemoveLast(void)				{ return RemoveAt(-1); }
	TP   Remove(INDEX_T iIndex)			{ return RemoveAt(iIndex); }
	TP   Remove(const T* pData)			{ __Lock_List__(*this); int iIndex = iter_t::Find(pData); if (iIndex < 0) return TP(NULL); return RemoveAt(iIndex); }
	template <typename TYPE>
	TP   RemoveByValue(const TYPE& B)	{ __Lock_List__(*this); INDEX_T iIndex = iter_t::FindByValue(B); if (iIndex < 0) return TP(NULL); return RemoveAt(iIndex); }
	virtual void RemoveAll(void)		{ __Lock_List__(*this); while (iter_t::m_nItem) RemoveAt(0); }

	bool Delete(void)					{ return DeleteAt(-2); }	//__Lock_List__(*this); TP pData = RemoveAt(-2); if (pData) DeleteObjectOnly(pData); return true;
	bool DeleteFirst(void)				{ return DeleteAt(0); }
	bool DeleteLast(void)				{ return DeleteAt(-1); }
	bool Delete(INDEX_T iIndex)			{ return DeleteAt(iIndex); }
	bool Delete(const T* pData)			{ __Lock_List__(*this); INDEX_T iIndex = iter_t::Find(pData); if (iIndex < 0) return false; return DeleteAt(iIndex); }
	template <typename TYPE>
	bool DeleteByValue(const TYPE& B)	{ __Lock_List__(*this); INDEX_T iIndex = iter_t::FindByValue(B); if (iIndex < 0) return false; return DeleteAt(iIndex); }
	void clear(void)					{ DeleteAll(); }
	virtual void DeleteAll(void)		{ __Lock_List__(*this); if (!parent_t::m_bAutoDeleteObject) { RemoveAll(); return; } while (iter_t::m_nItem) DeleteAt(0); }

public:
	//-----------------------
	// Swap/Invert
	virtual bool Swap(INDEX_T i0, INDEX_T i1) {
		__Lock_List__(*this);

		if ( (i0 < 0) || (i0 >= iter_t::m_nItem) || (i1 < 0) || (i1 >= iter_t::m_nItem) )
			return false;
		if (i0 == i1)
			return true;
		TP* ppData0 = iter_t::GetPPItem(i0);
		TP* ppData1 = iter_t::GetPPItem(i1);
		TP pData = *ppData0;
		*ppData0 = *ppData1;
		*ppData1 = pData;
		return true;
	}

	virtual void Invert(void) {
		__Lock_List__(*this);

		if (iter_t::m_nItem <= 1)
			return;

		TLISTITER iterL(*this);
		TLISTITER iterR(*this);

		SIZE_T n = iter_t::m_nItem/2;
		INDEX_T i = 0;
		for (iterL.First(), iterR.Last(); i < n; i++, iterL.Next(), iterR.Previous()) {
			TP pData = *iterL.GetPPItem();
			*iterL.GetPPItem() = *iterR.GetPPItem();
			*iterR.GetPPItem() = pData;
		}
	}


public:
	//-----------------------
	// CopyFrom. AddFrom       : copy objects From B.
	// CopyRefFrom, AddRefFrom : copy Pointers or RefPointers.
	template < class TLIST >
	bool CopyFrom(const TLIST& B) {
		__Lock_List__(*this);
		DeleteAll();
		return AddFrom(B);
	}
	template < class TLIST >
	bool CopyRefFrom(const TLIST& B) {
		__Lock_List__(*this);
		DeleteAll();
		return AddRefFrom(B);
	}

	template < class TLIST >
	bool AddFrom(const TLIST& B) {
		__Lock_List__(*this);

		__if_not_exists(TP::Release) {
			if (!parent_t::m_bAutoDeleteObject)
				if (iter_t::m_nItem)
					return false;
				else
					parent_t::AutoDelete(true);
		}

		for (INDEX_T i = 0; i < (INDEX_T)B.size(); i++) {
			__if_exists(TLIST::value_type::NewClone) { Push(TPC((T*) (B[i].NewClone()))); }
WARNING_PUSH_AND_DISABLE(4244)
			__if_not_exists(TLIST::value_type::NewClone) { Push(TPC(new T(B[i]))); }
WARNING_POP()
		}
		return true;
	}
	template < class TLIST >
	bool AddRefFrom(TLIST& B) {
		__Lock_List__(*this);

		__if_not_exists(TP::Release) {
			if (parent_t::m_bAutoDeleteObject)
				if (iter_t::m_nItem)
					return false;
				else
					parent_t::AutoDelete(false);
		}

		for (INDEX_T i = 0; i < (INDEX_T)B.size(); i++) {
			Push(B.GetItem(i));
		}
		return true;
	}

	//-----------------------
	// vector -> List
	//
	bool SetFromVector(std::vector< TP >& rrT, bool bAscending = true, INDEX_T iStart = 0) {
		__Lock_List__(*this);

		SIZE_T n = (SIZE_T)rrT.size();
		n = _min(iter_t::m_nItem-iStart, n);

		if (iStart >= iter_t::m_nItem)
			return false;

		if (bAscending) {
			for (INDEX_T i = 0; i < n; i++)
				*iter_t::GetPPItem(i) = rrT[i];
		} else {
			for (INDEX_T i = n-1; i >= 0; i--)
				*iter_t::GetPPItem(i) = rrT[i];
		}
		return true;
	}
	bool SetFromVector(std::vector< TPC >& rrT, bool bAscending = true, INDEX_T iStart = 0) {
		__Lock_List__(*this);

		SIZE_T n = (SIZE_T)rrT.size();
		n = _min(iter_t::m_nItem-iStart, n);

		if (iStart >= iter_t::m_nItem)
			return false;

		if (bAscending) {
			for (INDEX_T i = 0; i < n; i++)
				*iter_t::GetPPItem(i) = rrT[i];
		} else {
			for (INDEX_T i = n-1; i >= 0; i--)
				*iter_t::GetPPItem(i) = rrT[i];
		}
		return true;
	}

	//-----------------------
	// Sort
	//
public:
	typedef enum { SM_AUTO = -1, SM_QUICK, SM_INTRO, SM_MERGE } eSORTING_METHOD;

	//-----------------------------------------------------------------------------

	bool Sort(bool bAscending = true, eSORTING_METHOD eSM = SM_AUTO) {
		class compare {
		public:
			static int fnCompare(const T& a, const T& b, void* pCookie) { return a == b ? 0 : a < b ? -1 : 1; }
		};
		return Sort(compare::fnCompare, NULL, bAscending, eSM);
	}
	bool Sort(int (*fnCompare)(const T&, const T&), bool bAscending = true, eSORTING_METHOD eSM = SM_AUTO) {
		class compare {
		public:
			typedef int (*fnCompare2)(const T&, const T&);
			static int fnCompare(const T& a, const T& b, void* pCookie) { return ((fnCompare2)pCookie)(a, b); }
		};
		return Sort(compare::fnCompare, fnCompare, bAscending, eSM);
	}
	bool Sort(int (*fnCompare)(const T&, const T&, void* pCookie), void* pCookie, bool bAscending = true, eSORTING_METHOD eSM = SM_AUTO) {
		if (!iter_t::m_nItem)
			return false;

		if (fnCompare == NULL)
			return false;

		__Lock_List__(*this);

		if (eSM == SM_AUTO) {
			eSM = (iter_t::m_nItem < 500 * 1000) ? SM_INTRO : SM_MERGE;
		}

		std::vector< TP > backup;
		iter_t::GetVector(backup);

		switch (eSM) {
		case SM_QUICK : QuickSort(backup.data(), 0, iter_t::m_nItem-1, fnCompare, pCookie); break;
		case SM_INTRO : intro_sort(backup.data(), 0, iter_t::m_nItem-1, iter_t::m_nItem-1, fnCompare, pCookie); break;
		case SM_MERGE : MergeSort(backup.data(), 0, iter_t::m_nItem, 0, fnCompare, pCookie); break;
		}

		SetFromVector(backup, bAscending);

		iter_t::GetPPItem(0);

		return true;
	}

	//-----------------------------------------------------------------------------

	void QuickSort(TP data[], INDEX_T left, INDEX_T right, int (*fnCompare)(const T&, const T&, void* pCookie), void* pCookie) {
		if (left < right) {
			INDEX_T i = left, j = right+1;
			TP* pivot = data+left;
			do {
				do i++; while ((i < right) && (fnCompare(*(data[i]), **pivot, pCookie) < 0));
				do j--; while ((j >= left) && (fnCompare(*(data[j]), **pivot, pCookie) > 0));
				if (i < j) {
					TP temp = data[i];
					data[i] = data[j];
					data[j] = temp;
				}
			} while (i < j);
			TP temp = data[left];
			data[left] = data[j];
			data[j] = temp;

			QuickSort(data, left, j-1, fnCompare, pCookie);
			QuickSort(data, j+1, right, fnCompare, pCookie);
		}
	}

	//-----------------------------------------------------------------------------

	TP median_of_three(TP dst[], INDEX_T iElement1, INDEX_T iElement2, int (*fnCompare)(const T&, const T&, void* pCookie), void* pCookie) {
		INDEX_T iMid = (iElement1 + iElement2) >> 1;
		if (fnCompare(*dst[iElement1], *dst[iMid], pCookie) > 0)
			sort_swap(dst, iElement1, iMid);
	
		if (fnCompare(*dst[iElement1], *dst[iElement2], pCookie) > 0)
			sort_swap(dst, iElement1, iElement2);
	
		if (fnCompare(*dst[iMid], *dst[iElement2], pCookie) > 0)
			sort_swap(dst, iMid, iElement2);
	
		sort_swap(dst, iMid, iElement2 - 1);
		return dst[iElement2 - 1];
	}

	void heap_down(TP dst[], INDEX_T i, SIZE_T N, INDEX_T iLeft, int (*fnCompare)(const T&, const T&, void* pCookie), void* pCookie) {
		INDEX_T iChild;
		TP data;
		for (data = dst[i + iLeft]; LeftChild(i) < N; i = iChild - iLeft) {
			iChild = LeftChild(i) + iLeft;
			if (((iChild - iLeft + 1) < N) && fnCompare(*dst[iChild], *dst[iChild + 1], pCookie) < 0)
				++iChild;
			if (fnCompare(*data, *dst[iChild], pCookie) < 0)
				dst[i + iLeft] = dst[iChild];
			else
				break;
		}
		i += iLeft;
		dst[i] = data;
	}

	void heap_sort(TP dst[], INDEX_T iLeft, INDEX_T iRight, int (*fnCompare)(const T&, const T&, void* pCookie), void* pCookie) {
		INDEX_T N = iRight - iLeft + 1;
		for (INDEX_T i = N / 2; i > 0; )
			heap_down(dst, --i, N, iLeft, fnCompare, pCookie);

		for (INDEX_T i = N - 1; i >= 1; i--) {
			//TP temp = dst[iLeft];
			//dst[iLeft] = dst[i + iLeft];
			//dst[i + iLeft] = temp;
			sort_swap(dst, iLeft, i+iLeft);
			heap_down(dst, 0, i, iLeft, fnCompare, pCookie);
		}
	}

	// intro sort
	inline INDEX_T LeftChild(INDEX_T i) {
		return ( 2 * i + 1) ;
	}

	inline void sort_swap(TP dst[], INDEX_T i0, INDEX_T i1) {
		if (i0 == i1)
			return;
		TP temp = dst[i0];
		dst[i0] = dst[i1];
		dst[i1] = temp;
	}

	void insert_sort(TP dst[], INDEX_T iLeft, INDEX_T iRight,  int (*fnCompare)(const T&, const T&, void* pCookie), void* pCookie) {
		iRight++;
		for (INDEX_T i = iLeft + 1; i < iRight; i++) {
			TP temp = dst[i];
			INDEX_T j = i - 1;
		
			if (j >= iLeft) {
				while (fnCompare(*dst[j], *temp, pCookie) > 0) {
					dst[j + 1] = dst[j];
					--j;
					if (j < iLeft) break;
				}
			}

			dst[j + 1] = temp;
		}
	}

	// intro_sort()
	// 기본 퀵정렬을 수행하다가, 재귀의 깊이가 깊어져 성능이 저하되면, 언제나 nlogn의 성능을 가지는 힢소트로 전환
	// 단, 정렬구간이 작을 경우 삽입정렬이 가장 빠르므로 삽입정렬 수행
	// ※ 힢소트가 수행되는 경우는 median_of_three()함수로 인해 거의 없음
	void intro_sort(TP dst[], INDEX_T iLeft, INDEX_T iRight, INDEX_T iMaxDepth, int (*fnCompare)(const T&, const T&, void* pCookie), void* pCookie) {
		if (iLeft >= iRight)
			return;
		if ((iRight - iLeft + 1) > 20) {
			if (0 < iMaxDepth) {
				INDEX_T iLow = iLeft;
				INDEX_T iHigh = iRight - 1;
				TP pivot = median_of_three(dst, iLeft, iRight, fnCompare, pCookie);
				while (1) {
					while ((iLow < iHigh) && fnCompare(*dst[++iLow], *pivot, pCookie) < 0);
					while ((iLow < iHigh) && fnCompare(*dst[--iHigh], *pivot, pCookie) > 0);

					if (iLow >= iHigh) break;
					sort_swap(dst, iLow, iHigh);
				}

				iMaxDepth /= 2, iMaxDepth += iMaxDepth / 2;
				sort_swap(dst, iHigh, iRight - 1);
				intro_sort(dst, iLeft, iLow - 1, iMaxDepth, fnCompare, pCookie);
				intro_sort(dst, iLow + 1, iRight, iMaxDepth, fnCompare, pCookie);
			}
			else if (iLeft < iRight)
				heap_sort(dst, iLeft, iRight, fnCompare, pCookie);
		}
		else
			insert_sort(dst, iLeft, iRight, fnCompare, pCookie);
	}

	//-----------------------------------------------------------------------------
	bool MergeSort(TP src[], INDEX_T i0, INDEX_T i1, int nDepth, int (*fnCompare)(const T&, const T&, void* pCookie), void* pCookie) {
		if ( (src == NULL) || (i0 >= i1) || (fnCompare == NULL) )
			return false;

		const int nMaxCPUThread = 8;
		const int nMaxItem = 2000;
		const int nThread = 4;

		const SIZE_T nSize = i1-i0;

		if ((nMaxItem < 0) || (nSize <= nMaxItem)) {
			// Sort
			intro_sort(src, i0, i1-1, i1-1, fnCompare, pCookie);
		} else {
			// Split and Merge
			INDEX_T iFronts[nThread];
			INDEX_T iEnds[nThread];
			INDEX_T ic = i0;

			const SIZE_T iStep = (nSize-1)/nThread + 1;
			for (int iThread = 0; iThread < nThread; iThread++) {
				iFronts[iThread] = ic;
				ic = _min(ic + iStep, i1);
				iEnds[iThread] = ic;
			}

			// 분기 & Sort
			if ( (nDepth+1) * nThread <= nMaxCPUThread ) {
				// Multi Threaded...
				#pragma omp parallel for
				for (int iThread = 0; iThread < nThread; iThread++) {
					MergeSort(src, iFronts[iThread], iEnds[iThread], nDepth+1, fnCompare, pCookie);
				}
			} else {
				for (int iThread = 0; iThread < nThread; iThread++) {
					MergeSort(src, iFronts[iThread], iEnds[iThread], nDepth+1, fnCompare, pCookie);
				}
			}

			TP* dst = new TP[nSize];
			INDEX_T iPos = 0;
			while (true) {
				INDEX_T iMinThread = -1;
				TP pMin = NULL;

				for (INDEX_T iThread = 0; iThread < nThread; iThread++) {
					if (iFronts[iThread] >= iEnds[iThread])
						continue;
					if ( !pMin || (fnCompare(*src[iFronts[iThread]], *pMin, pCookie) < 0) ) {
						iMinThread = iThread;
						pMin = src[iFronts[iThread]];
					}
				}

				if (!pMin)
					break;
				dst[iPos++] = pMin;
				iFronts[iMinThread]++;
			}

			__if_exists(TP::Release) {
				for (INDEX_T i = 0; i < nSize; i++)
					src[i+i0] = dst[i];
			}
			__if_not_exists(TP::Release) {
				memcpy(src+i0, dst, nSize*sizeof(TP));
			}

			delete [] dst;
		}

		return true;
	}


	//-----------------------
	// Serializing
public:
	template <class Archive> Archive& StoreTo(Archive& ar, bool bFlush = false) const {
		__Lock_List__(*this);

#if _X86_COMPATIBLE_MODE_FOR_IO_
		ar << (int32_t)iter_t::m_nItem;
#else
		ar << iter_t::m_nItem;
#endif
		if (iter_t::m_nItem) {
			for (INDEX_T i = 0; i < iter_t::m_nItem; i++) {
				__if_exists(T::SerializeDynamic) {
					T::SerializeDynamic(ar, *((IList*)this)->GetPPItem(i));
				}
				__if_not_exists(T::SerializeDynamic) {
					ar << **iter_t::GetPPItem(i);
				}
			}
		}
		if (bFlush)
			ar.Flush();
		return ar;
	}
	template <class Archive>
	Archive& LoadFrom(Archive& ar) {
		__Lock_List__(*this);

		DeleteAll();
#if _X86_COMPATIBLE_MODE_FOR_IO_
		int32_t n = 0;
#else
		SIZE_T n = 0;
#endif
		ar >> n;
		for (INDEX_T i = 0; i < n; i++) {
			TP p(NULL);
			__if_exists(T::SerializeDynamic) {
				p = TP(T::SerializeDynamic(ar, (T*)NULL));
			}
			__if_not_exists(T::SerializeDynamic) {
				p = TP(new T);
				ar >> *p;
			}
			Push(p);
		}
		return ar;
	}

	template <class Archive>
	friend Archive& operator << (Archive& ar, const IList<T, TP, TPC, INDEX_T, SIZE_T, TLISTITER >& B) {
		return B.StoreTo(ar);
	}
	template <class Archive>
	friend Archive& operator >> (Archive& ar, IList<T, TP, TPC, INDEX_T, SIZE_T, TLISTITER >& B) {
		return B.LoadFrom(ar);
	}

	template <class Archive>
	friend Archive& operator || (Archive& ar, const IList<T, TP, TPC, INDEX_T, SIZE_T, TLISTITER >& B) {
		if (ar.IsStoring())
			return B.StoreTo(ar);
		else
			return B.LoadFrom(ar);
		return *this;
	}
};

#pragma pack(pop)
