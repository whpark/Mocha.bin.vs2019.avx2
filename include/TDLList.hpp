//=============================================================================
//
// OBSOLETE.
//
//=============================================================================
//
// TList.h: interface for the TList class.
// Last Edited		2003. 5. 18.
//					2004. 01. 12. : Sort with Compare Function
//					2004. 08. 23. : FindByValue
//					2006.         :
//					                assign operator  (const)
//					                Invert()
//					                ListIterator
//  
//					2009. 09. 14. : Delete Sequence
//
//					2010. 05. 13. : TBuffer : Attach/Detach
//
//					2010. 05. 20. : TListR : new/delete based on Reference Counter
//
//					2010. 06. 23. : TList : InsertAt : insert at position
//
//					2011. 01. 14. : TList : DeleteByValue
//
//					2012. 07. 21. : TList : Swap
//
//					2012. 07. 25. : TList : operator ==
//
//					2012. 09. 06. : TList : debug. InsertAt()
//
//					2013. 05. 29. : TList : Index 에 -1 사용시 Last() 개체를 지칭하도록 수정 (Pop, Remove, Delete, GetItem, SetItem, [] )
//
//					2013. 05. 29. : intro_sort 추가 (by 김명식), MergeSort 추가, Sort Method 선택할 수 있도록 수정
//
//					2013. 06. 23. : TList -> TDLList 이름 변경. 이 파일은 사용 안함.
//
//
// Made by PWH
//////////////////////////////////////////////////////////////////////

#if !defined(__T_DL_LIST_HPP__INCLUDED__PWH__)
#define __T_DL_LIST_HPP__INCLUDED__PWH__

#pragma once

//////////////////////////////////////////////////////////////////////

#include <vector>

#ifndef countof
	#define countof(arr) ( sizeof(arr) / sizeof( (arr)[0] ) )
#endif

#ifndef ZeroVar
	#define ZeroVar(var) memset(&var, 0, sizeof(var))
#endif

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

#ifndef BOOL
	typedef int BOOL;
#endif

#ifndef FALSE
	#define FALSE 0
#endif

#ifndef TRUE
	#define TRUE 1
#endif

#ifndef NULL
	#define NULL 0
#endif

#ifndef _min
#	define _min(a, b) ( ( (a) < (b) ) ? (a) : (b) )
#endif
#ifndef _max
#	define _max(a, b) ( ( (a) > (b) ) ? (a) : (b) )
#endif

template <class T> class TDLList;
template <class T> class TListR;

//-----------------------------------------------------------------------------
// TDLListNode
//
template <class T> class TDLListNode {
	friend class TDLList<T>;
	friend class TListR<T>;
protected:
	T* m_pData;
	TDLListNode* m_pNextNode;
	TDLListNode* m_pPreviousNode;
public:
	TDLListNode(T* _pData = NULL) { m_pData = _pData; m_pNextNode = NULL; m_pPreviousNode = NULL; }
};

//-----------------------------------------------------------------------------
// TDLList
//
template <class T> class TDLList {
protected:
	int m_nItem;
	mutable int m_iCurrent;
	int m_bAutoDeleteObject;
	TDLListNode<T>* m_pFirstNode;
	mutable TDLListNode<T>* m_pCurrentNode;
public:
	// Constructor & Destructor, Copy
	TDLList(int bAutoDelete = TRUE) : m_nItem(0), m_iCurrent(0), m_pFirstNode(NULL), m_pCurrentNode(NULL), m_bAutoDeleteObject(bAutoDelete) {
	}
	virtual ~TDLList(void) {
		if (m_bAutoDeleteObject) for (First(); m_nItem; DeleteObject());
		else for (First(); m_nItem; Remove());
	}
	TDLList(TDLList<T>& B) : m_nItem(0), m_iCurrent(0), m_pFirstNode(NULL), m_pCurrentNode(NULL), m_bAutoDeleteObject(true) {
		*this += B;
	}
	TDLList<T>& operator = (const TDLList<T>& B);

	TDLList<T>& CopyTo(TDLList<T>& B) const {
		const TDLListNode<T>* pNode = m_pFirstNode;
		for (int i = 0; i < m_nItem; i++, pNode = pNode->m_pNextNode)
			B.Attach(new T(*pNode->m_pData));
		return B;
	}
	TDLList<T>& CopyFrom(const TDLList<T>& B) {
		const TDLListNode<T>* pNode = B.m_pFirstNode;
		for (int i = 0; i < B.m_nItem; i++, pNode = pNode->m_pNextNode)
			Attach(new T(*pNode->m_pData));
		return *this;
	}

	//-----------------------
	// Compare
	template < class TLIST >
	BOOL operator == (const TLIST& B) const { return Compare(B); }

	template < class T2, template < class T3 > class TLIST >
	BOOL Compare(const TLIST<T2>& B) const {
		if (m_nItem != B.N())
			return FALSE;
		TDLListNode<T>* pNode1 = m_pFirstNode;
		B.First();
		for (int i = 0; i < m_nItem; i++, pNode1 = pNode1->m_pNextNode, B.Next()) {
			if (*pNode1->m_pData == B())
				;
			else
				return FALSE;
		}
		return TRUE;
	}

	//-----------------------
	// Set/Get Item
	T* SetItem(T* pT)					{ if (!m_pCurrentNode) return NULL; return m_pCurrentNode->m_pData = pT; }
	T* SetItem(int iIndex, T* pT)		{ (*this)[iIndex]; return SetItem(pT); }
	T* GetItem()						{ if (!m_pCurrentNode) return NULL; return m_pCurrentNode->m_pData; }
	T* GetItem(int iIndex)				{ (*this)[iIndex]; return GetItem(); }
	const T* GetItem() const			{ if (!m_pCurrentNode) return NULL; return m_pCurrentNode->m_pData; }
	const T* GetItem(int iIndex) const	{ (*this)[iIndex]; return GetItem(); }

	//-----------------------
	// 
	operator T* () const { if (!m_nItem) return NULL; return GetVector(new T[m_nItem]); }
	operator T** () const { if (!m_nItem) return NULL; return GetVector(new T*[m_nItem]); }
	T* GetVector(T* pT) const {
		const TDLListNode<T>* pNode = m_pFirstNode;
		for (int i = 0; i < m_nItem; i++, pNode = pNode->m_pNextNode)
			pT[i] = *(pNode->m_pData);
		return pT;
	}
	T** GetVector(T** ppT) const {
		const TDLListNode<T>* pNode = m_pFirstNode;
		for (int i = 0; i < m_nItem; i++, pNode = pNode->m_pNextNode)
			ppT[i] = pNode->m_pData;
		return ppT;
	}
	TDLList<T>& operator = (T** ppT) {
		First();
		for (int i = 0; i < m_nItem; i++, Next()) {
			SetItem(ppT[i]);
		}
		First();
		return *this;
	}

	//-----------------------
	//
	TDLList<T> operator + (const TDLList<T>& B) const;
	TDLList<T>& operator += (const TDLList<T>& B);
	int GetAutoDelete(void) const { return m_bAutoDeleteObject; }
	void AutoDelete(int bAutoDelete = TRUE) { m_bAutoDeleteObject = bAutoDelete; }

	int N(void) const { return m_nItem; }
	int Index(void) const {return ( (m_pCurrentNode == NULL) ? -1 : (m_iCurrent) ); }

	//-----------------------
	// Iterate
	T& operator [] (int i);
	const T& operator [] (int i) const;
	T& operator () (void) { assert1(m_pCurrentNode != NULL); return *m_pCurrentNode->m_pData; }
	const T& operator () (void) const { assert1(m_pCurrentNode != NULL); return *m_pCurrentNode->m_pData; }

	//-----------------------
	// Iterate
	T* First(void)					{ m_iCurrent = 0; if (m_nItem) return (m_pCurrentNode = m_pFirstNode)->m_pData; else return NULL;}
	const T* First(void) const		{ m_iCurrent = 0; if (m_nItem) return (m_pCurrentNode = m_pFirstNode)->m_pData; else return NULL;}
	T* Current(void)				{ assert1(m_pCurrentNode != NULL); return m_pCurrentNode->m_pData; }
	const T* Current(void) const	{ assert1(m_pCurrentNode != NULL); return m_pCurrentNode->m_pData; }
	T* Next(void) {						// do as circular list
		assert1(m_pCurrentNode != NULL);
		m_pCurrentNode = m_pCurrentNode->m_pNextNode;
		m_iCurrent = (m_iCurrent+1) % m_nItem;
		return m_pCurrentNode->m_pData;
	}
	const T* Next(void) const {						// do as circular list
		assert1(m_pCurrentNode != NULL);
		m_pCurrentNode = m_pCurrentNode->m_pNextNode;
		m_iCurrent = (m_iCurrent+1) % m_nItem;
		return m_pCurrentNode->m_pData;
	}
	T* Previous(void) {					// do as circular list
		assert1(m_pCurrentNode != NULL);
		m_pCurrentNode = m_pCurrentNode->m_pPreviousNode;
		m_iCurrent = (m_iCurrent + m_nItem - 1) % m_nItem;
		return m_pCurrentNode->m_pData;
	}
	const T* Previous(void) const {					// do as circular list
		assert1(m_pCurrentNode != NULL);
		m_pCurrentNode = m_pCurrentNode->m_pPreviousNode;
		m_iCurrent = (m_iCurrent + m_nItem - 1) % m_nItem;
		return m_pCurrentNode->m_pData;
	}
	T* Last(void) {
		if (m_nItem) { m_iCurrent = m_nItem-1; return (m_pCurrentNode = m_pFirstNode->m_pPreviousNode)->m_pData;
		} else return NULL;
	}
	const T* Last(void) const {
		if (m_nItem) { m_iCurrent = m_nItem-1; return (m_pCurrentNode = m_pFirstNode->m_pPreviousNode)->m_pData;
		} else return NULL;
	}

	int IsEmpty(void) const { return !m_nItem; }
	int NextNotNull(void) const {
		if (m_iCurrent < m_nItem-1) return TRUE;
		else return FALSE;
	}

	//-----------------------
	// Insert
	int Insert(const T* pData);
	int InsertPrevious(const T* pData);
	int Attach(const T* pData) { return InsertAt(pData, -1); }
	int Push(const T* pData) { return Insert(pData);}  ;
	int InsertAt(const T* pData, int iIndex = -2);	// iIndex :
													//    -2 : Inserts after current item
													//    -1 : Inserts as Last Item
													// other : inserts as 'iIndex'. if iIndex is larger than or equal to current item count, Inserts as Last Item
													// over nItem : Inserts as Last Item

public:
	//-----------------------
	// Delete
	T* Pop(void);
	T* Pop(const T* pData) { int i = Find(pData); if (i >= 0) return Pop(i); return NULL; }
	T* Pop(int i) { (*this)[i]; return Pop(); }
	T* Remove(void) { return Pop(); }
	T* Remove(const T* pData) { return Pop(pData); }
	T* Remove(int i) { return Pop(i); }
	void RemoveAll(void) { for (First(); m_nItem; Pop()); }

	virtual BOOL DeleteObject(void);
	BOOL Delete(void) { return DeleteObject(); }
	BOOL Delete(const T* pData) { int i = Find(pData); if (i >= 0) return Delete(i); return FALSE; }
	BOOL Delete(int i) { (*this)[i]; return DeleteObject(); }
	template <typename TYPE> BOOL DeleteByValue(const TYPE& B) {
		int iIndex = FindByValue(B);
		if (iIndex < 0)
			return FALSE;
		return Delete(iIndex);
	}
	void DeleteAll(void) { for (First(); m_nItem; DeleteObject()); }

	//-----------------------
	BOOL Swap(int i0, int i1) {
		if ( (i0 < 0) || (i0 >= m_nItem) || (i1 < 0) || (i1 >= m_nItem) )
			return FALSE;
		if (i0 == i1)
			return TRUE;
		(*this)[i0];
		TDLListNode<T>* pNodeA = m_pCurrentNode;
		(*this)[i1];
		TDLListNode<T>* pNodeB = m_pCurrentNode;
		T* pC = pNodeA->m_pData;
		pNodeA->m_pData = pNodeB->m_pData;
		pNodeB->m_pData = pC;
		return TRUE;
	}
	void Invert(void);

	//-----------------------
	// Find
	int Find(const T& B) const;
	int Find(const T* pB) const;
	template <typename TYPE> int FindByValue(const TYPE& B) {
		if (m_pFirstNode == NULL) return -1;

		int i;
		for (First(), i = 0; i < m_nItem; i++) {
			if ((*this)[i] == B) return i;
		}

		return -1;
	}
	template <typename TYPE> int FindByValue(const TYPE& B) const {
		if (m_pFirstNode == NULL) return -1;

		int i;
		for (First(), i = 0; i < m_nItem; i++) {
			if ((*this)[i] == B) return i;
		}

		return -1;
	}

	//-----------------------
	// Sort
public:
	typedef enum { SM_AUTO = -1, SM_QUICK, SM_INTRO, SM_MERGE } eSORTING_METHOD;
	BOOL Sort(BOOL bAscending = TRUE, eSORTING_METHOD eSM = SM_AUTO);
	BOOL Sort(int (*fnCompare)(const T&, const T&) = NULL, BOOL bAscending = TRUE, eSORTING_METHOD eSM = SM_AUTO);

private:
	void QuickSort(T* data[], int left, int right, int (*fnCompare)(const T&, const T&));
	void intro_sort(T* data[], int left, int right, int iMaxDepth, int (*fnCompare)(const T&, const T&));	// by 김명식

private:
	// intro sort
	inline int LeftChild(int i) { return ( 2 * i + 1) ; }
	inline void sort_swap(T* dst[], int i0, int i1) { if (i0 == i1) return; T* temp = dst[i0]; dst[i0] = dst[i1]; dst[i1] = temp; }
	void insert_sort(T* dst[], int iLeft, int iRight, int (*fnCompare)(const T&, const T&));
	void heap_sort(T* dst[], int iLeft, int iRight, int (*fnCompare)(const T&, const T&));
	void heap_down(T* dst[], int i, int N, int iLeft, int (*fnCompare)(const T&, const T&));
	T* median_of_three(T* dst[], int iElement1, int iElement2, int (*fnCompare)(const T&, const T&));

	// merge sort
	BOOL MergeSort(T* src[], int i0, int i1, int nDepth, int (*fnCompare)(const T&, const T&));
	
	//-----------------------
	// Serializing
public:
	template <class Archive> Archive& StoreTo(Archive& ar, BOOL bFlush = FALSE) const {
		ar << m_nItem;
		if (m_nItem) {
			const TDLListNode<T>* pNode = m_pFirstNode;
			for (int i = 0; i < m_nItem; i++, pNode = pNode->m_pNextNode)
				ar << *(pNode->m_pData);
		}
		if (bFlush)
			ar.Flush();
		return ar;
	}
	template <class Archive> Archive& LoadFrom(Archive& ar) {
		int i, n;
		ar >> n;
		T* p;
		for (i = 0;i < n; i++) {
			p = new T;
			ar >> *p;
			Insert(p);
		}
		return ar;
	}
	template <class Archive>
	friend Archive& operator << (Archive& ar, const TDLList<T>& B) {
		return B.StoreTo(ar);
	}
	template <class Archive>
	friend Archive& operator >> (Archive& ar, TDLList<T>& B) {
		return B.LoadFrom(ar);
	}
};

//-----------------------------------------------------------------------------

template <class T>
TDLList<T>& TDLList<T>::operator = (const TDLList<T>& B) {
	if (this == &B) return *this;
	if (m_bAutoDeleteObject) while(m_nItem) DeleteObject();
	else while(m_nItem) Pop();

	m_bAutoDeleteObject = B.m_bAutoDeleteObject;

	*this += B;

	if ( (B.m_iCurrent >= 0) && (B.m_iCurrent < m_nItem) ) {
		(*this)[B.m_iCurrent];
	}
	return *this;
}

template <class T>
TDLList<T> TDLList<T>::operator + (const TDLList<T>& B) const {
	TDLList<T> result;

	result += *this;
	result += B;

	return result;
}

template <class T>
TDLList<T>& TDLList<T>::operator += (const TDLList<T>& B) {
	if (!B.m_nItem) return *this;

	if (m_bAutoDeleteObject) {
		const TDLListNode<T>* pNode = B.m_pFirstNode;
		for (int i = 0; i < B.m_nItem; i++, pNode = pNode->m_pNextNode) {
			T* pT = new T;
			assert1(pT);	//
			*pT = *(pNode->m_pData);
			Attach(pT);
		}
	} else {
		const TDLListNode<T>* pNode = B.m_pFirstNode;
		for (int i = 0; i < B.m_nItem; i++, pNode = pNode->m_pNextNode) {
			Attach(pNode->m_pData);
		}
	}

	return *this;
}

template <class T>
T& TDLList<T>::operator [] (int i) {
	assert1(i < m_nItem);
	//assert1(i >= 0);
	if (i < 0) { if (m_nItem) return *Last(); else assert1(i < 0); }
	for( ; m_iCurrent > i; m_iCurrent--, m_pCurrentNode = m_pCurrentNode->m_pPreviousNode) ;
	for( ; m_iCurrent < i; m_iCurrent++, m_pCurrentNode = m_pCurrentNode->m_pNextNode) ;
	return *(m_pCurrentNode->m_pData);
}
template <class T>
const T& TDLList<T>::operator [] (int i) const {
	assert1(i < m_nItem);
	//assert1(i >= 0);
	if (i < 0) { if (m_nItem) return *Last(); else assert1(i < 0); }
	for( ; m_iCurrent > i; m_iCurrent--, m_pCurrentNode = m_pCurrentNode->m_pPreviousNode) ;
	for( ; m_iCurrent < i; m_iCurrent++, m_pCurrentNode = m_pCurrentNode->m_pNextNode) ;
	return *(m_pCurrentNode->m_pData);
}

template <class T>
int TDLList<T>::Insert(const T* pData) {
	TDLListNode<T>* pNode = new TDLListNode<T>((T*)pData);
	if (pNode == NULL) return -1;
	if (m_nItem) {
		pNode->m_pNextNode = m_pCurrentNode->m_pNextNode;
		m_pCurrentNode->m_pNextNode = pNode;
		pNode->m_pNextNode->m_pPreviousNode = pNode;
		pNode->m_pPreviousNode = m_pCurrentNode;
		m_iCurrent++;
	}
	else {
		m_pFirstNode = pNode;
		m_pFirstNode->m_pNextNode = m_pFirstNode;
		m_pFirstNode->m_pPreviousNode = m_pFirstNode;
	}
	m_pCurrentNode = pNode;
	m_nItem++;
	return m_iCurrent;
}

template <class T>
int TDLList<T>::InsertAt(const T* pData, int iIndex) {
	if (iIndex >= m_nItem)
		iIndex = -1;	// insert at last

	if (iIndex == -2) {
		// as is.
	} else if (iIndex == -1) {
		// insert at last
		Last();
	} else if (iIndex == 0) {
		// insert at first
		First();
		return InsertPrevious(pData);
	} else if (iIndex-1 != m_iCurrent) {
		(*this)[iIndex-1];
	}
	return Insert(pData);
}

template <class T>
int TDLList<T>::InsertPrevious(const T* pData) {
	if (m_nItem) {
		Previous();
		Insert(pData);
		if (m_pFirstNode->m_pPreviousNode == m_pCurrentNode) { m_pFirstNode = m_pCurrentNode; m_iCurrent = 0; }
	} else Insert(pData);
	return m_iCurrent;
}

template <class T>
T* TDLList<T>::Pop(void) {
	if (!m_nItem) return NULL;
	if (!m_pCurrentNode)
		return FALSE;
	assert1(m_pCurrentNode != NULL);

	TDLListNode<T>* pNode = m_pCurrentNode;

	T* pData = m_pCurrentNode->m_pData;

	if (m_nItem == 1) {
		delete m_pCurrentNode;
		m_pFirstNode = m_pCurrentNode = NULL;
		m_nItem = m_iCurrent = 0;
	} else {
		m_pCurrentNode->m_pNextNode->m_pPreviousNode = m_pCurrentNode->m_pPreviousNode;
		m_pCurrentNode->m_pPreviousNode->m_pNextNode = m_pCurrentNode->m_pNextNode;

		if (m_pCurrentNode == m_pFirstNode) m_pFirstNode = m_pCurrentNode = m_pCurrentNode->m_pNextNode;
		else m_pCurrentNode = m_pCurrentNode->m_pNextNode;
		delete pNode;
		m_nItem--;
		if (m_iCurrent == m_nItem) {
			m_iCurrent--;
			m_pCurrentNode = m_pFirstNode->m_pPreviousNode;
		}
	}

	return pData;
}

template <class T>
BOOL TDLList<T>::DeleteObject(void) {
	T* pData = Pop();
	if (!pData)
		return FALSE;

	if (pData != NULL) delete pData;

	return TRUE;
}

template <class T>
void TDLList<T>::Invert(void) {
	if (m_nItem <= 1)
		return;

	TDLListNode<T>* pNode;
	TDLListNode<T>* pNodeTemp;

	pNode = m_pFirstNode;
	for (int i = 0; i < m_nItem; i++) {
		// Swap Previous <-> Next
		pNodeTemp = pNode->m_pNextNode;
		pNode->m_pNextNode = pNode->m_pPreviousNode;
		pNode->m_pPreviousNode =  pNodeTemp;

		// Next. (Previous is the Next Item)
		pNode = pNode->m_pPreviousNode;
	}

	m_pFirstNode = pNode->m_pNextNode;
	m_iCurrent = (m_nItem - 1) - m_iCurrent;
}

template <class T>
int TDLList<T>::Find(const T& B) const {
	if (m_pFirstNode == NULL) return -1;
	int i;
	for (First(), i = 0; i < m_nItem; i++) {
		if ((*this)[i] == B) return i;
	}
	return -1;
}

template <class T>
int TDLList<T>::Find(const T* pB) const {
	if (m_pFirstNode == NULL) return -1;
	First();
	for (int i = 0; i < m_nItem; i++) {
		(*this)[i];
		if (m_pCurrentNode->m_pData == pB) return i;
	}
	return -1;
}

//-----------------------------------------------------------------------------

template <class T>
BOOL TDLList<T>::Sort(BOOL bAscending, eSORTING_METHOD eSM) {
	if (!m_nItem)
		return TRUE;
	int i;
	TDLListNode<T>* pNode = m_pFirstNode;

	T** backup = *this;

	if (eSM == SM_AUTO) {
		eSM = (m_nItem < 500 * 1000) ? SM_INTRO : SM_MERGE;
	}

	class compare {
	public:
		static int fnCompare(const T& a, const T& b) { return a == b ? 0 : a < b ? -1 : 1; }
	};

	switch (eSM) {
	case SM_QUICK : QuickSort(backup, 0, m_nItem-1, compare::fnCompare); break;
	case SM_INTRO : intro_sort(backup, 0, m_nItem-1, m_nItem-1, compare::fnCompare); break;
	case SM_MERGE : MergeSort(backup, 0, m_nItem, 0, compare::fnCompare); break;
	}

	pNode = m_pFirstNode;
	if (bAscending) {
		for (i = 0; i < m_nItem; i++) {
			pNode->m_pData = backup[i];
			pNode = pNode->m_pNextNode;
		}
	} else {
		for (i = m_nItem-1; i >= 0; i--) {
			pNode->m_pData = backup[i];
			pNode = pNode->m_pNextNode;
		}
	}

	delete [] backup;

	First();

	return TRUE;
}
template <class T>
BOOL TDLList<T>::Sort(int (*fnCompare)(const T&, const T&), BOOL bAscending, eSORTING_METHOD eSM) {
	if (!m_nItem)
		return TRUE;
	int i;
	TDLListNode<T>* pNode = m_pFirstNode;

	if (fnCompare == NULL)
		return FALSE;

	T** backup = *this;

	if (eSM == SM_AUTO) {
		eSM = (m_nItem < 500 * 1000) ? SM_INTRO : SM_MERGE;
	}

	switch (eSM) {
	case SM_QUICK : QuickSort(backup, 0, m_nItem-1, fnCompare); break;
	case SM_INTRO : intro_sort(backup, 0, m_nItem-1, m_nItem-1, fnCompare); break;
	case SM_MERGE : MergeSort(backup, 0, m_nItem, 0, fnCompare); break;
	}

	pNode = m_pFirstNode;
	if (bAscending) {
		for (i = 0; i < m_nItem; i++) {
			pNode->m_pData = backup[i];
			pNode = pNode->m_pNextNode;
		}
	} else {
		for (i = m_nItem-1; i >= 0; i--) {
			pNode->m_pData = backup[i];
			pNode = pNode->m_pNextNode;
		}
	}

	delete [] backup;

	First();

	return TRUE;
}

//-----------------------------------------------------------------------------

template <class T>
void TDLList<T>::QuickSort(T* data[], int left, int right, int (*fnCompare)(const T&, const T&)) {
	if (left < right) {
		int i = left, j = right+1;
		T** pivot = data+left;
		do {
			do i++; while ((i < right) && (fnCompare(*(data[i]), **pivot) < 0));
			do j--; while ((j >= left) && (fnCompare(*(data[j]), **pivot) > 0));
			if (i < j) {
				T* temp = data[i];
				data[i] = data[j];
				data[j] = temp;
			}
		} while (i < j);
		T* temp = data[left];
		data[left] = data[j];
		data[j] = temp;

		QuickSort(data, left, j-1, fnCompare);
		QuickSort(data, j+1, right, fnCompare);
	}
}

//-----------------------------------------------------------------------------

template < class T >
T* TDLList<T>::median_of_three(T* dst[], int iElement1, int iElement2, int (*fnCompare)(const T&, const T&)) {
	int iMid = (iElement1 + iElement2) >> 1;
	if (fnCompare(*dst[iElement1], *dst[iMid]) > 0)
		sort_swap(dst, iElement1, iMid);
	
	if (fnCompare(*dst[iElement1], *dst[iElement2]) > 0)
		sort_swap(dst, iElement1, iElement2);
	
	if (fnCompare(*dst[iMid], *dst[iElement2]) > 0)
		sort_swap(dst, iMid, iElement2);
	
	sort_swap(dst, iMid, iElement2 - 1);
	return dst[iElement2 - 1];
}

template < class T >
void TDLList<T>::heap_down(T* dst[], int i, int N, int iLeft, int (*fnCompare)(const T&, const T&)) {
	int iChild;
	T* data;
	for (data = dst[i + iLeft]; LeftChild(i) < N; i = iChild - iLeft) {
		iChild = LeftChild(i) + iLeft;
		if (((iChild - iLeft + 1) < N) && fnCompare(*dst[iChild], *dst[iChild + 1]) < 0)
			++iChild;
		if (fnCompare(*data, *dst[iChild]) < 0)
			dst[i + iLeft] = dst[iChild];
		else
			break;
	}
	i += iLeft;
	dst[i] = data;
}

template < class T >
void TDLList<T>::heap_sort(T* dst[], int iLeft, int iRight, int (*fnCompare)(const T&, const T&)) {
	int N = iRight - iLeft + 1;
	for (int i = N / 2; i > 0; )
		heap_down(dst, --i, N, iLeft, fnCompare);

	for (int i = N - 1; i >= 1; i--) {
		//T* temp = dst[iLeft];
		//dst[iLeft] = dst[i + iLeft];
		//dst[i + iLeft] = temp;
		sort_swap(dst, iLeft, i+iLeft);
		heap_down(dst, 0, i, iLeft, fnCompare);
	}
}

template < class T >
void TDLList<T>::insert_sort(T* dst[], int iLeft, int iRight,  int (*fnCompare)(const T&, const T&)) {
	iRight++;
	for (int i = iLeft + 1; i < iRight; i++) {
		T* temp = dst[i];
		int j = i - 1;
		
		if (j >= iLeft) {
			while (fnCompare(*dst[j], *temp) > 0) {
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
template < class T >
void TDLList<T>::intro_sort(T* dst[], int iLeft, int iRight, int iMaxDepth, int (*fnCompare)(const T&, const T&)) {
	if (iLeft >= iRight)
		return;
	if ((iRight - iLeft + 1) > 20) {
		if (0 < iMaxDepth) {
			int iLow = iLeft;
			int iHigh = iRight - 1;
			T* pivot = median_of_three(dst, iLeft, iRight, fnCompare);
			while (1) {
				while ((iLow < iHigh) && fnCompare(*dst[++iLow], *pivot) < 0);
				while ((iLow < iHigh) && fnCompare(*dst[--iHigh], *pivot) > 0);

				if (iLow >= iHigh) break;
				sort_swap(dst, iLow, iHigh);
			}

			iMaxDepth /= 2, iMaxDepth += iMaxDepth / 2;
			sort_swap(dst, iHigh, iRight - 1);
			intro_sort(dst, iLeft, iLow - 1, iMaxDepth, fnCompare);
			intro_sort(dst, iLow + 1, iRight, iMaxDepth, fnCompare);
		}
		else if (iLeft < iRight)
			heap_sort(dst, iLeft, iRight, fnCompare);
	}
	else
		insert_sort(dst, iLeft, iRight, fnCompare);
}

//-----------------------------------------------------------------------------
template < class T >
BOOL TDLList<T>::MergeSort(T* src[], int i0, int i1, int nDepth, int (*fnCompare)(const T&, const T&)) {
	if ( (src == NULL) || (i0 >= i1) || (fnCompare == NULL) )
		return false;

	const int nMaxCPUThread = 8;
	const int nMaxItem = 2000;
	const int nThread = 4;

	const int nSize = i1-i0;

	if ((nMaxItem < 0) || (nSize <= nMaxItem)) {
		// Sort
		intro_sort(src, i0, i1-1, i1-1, fnCompare);
	} else {
		// Split and Merge
		int iFronts[nThread];
		int iEnds[nThread];
		int ic = i0;

		const int iStep = (nSize-1)/nThread + 1;
		for (int iThread = 0; iThread < nThread; iThread++) {
			iFronts[iThread] = ic;
			ic = min(ic + iStep, i1);
			iEnds[iThread] = ic;
		}

		// 분기 & Sort
		if ( (nDepth+1) * nThread <= nMaxCPUThread ) {
			// Multi Threaded...
			#pragma omp parallel for
			for (int iThread = 0; iThread < nThread; iThread++) {
				MergeSort(src, iFronts[iThread], iEnds[iThread], nDepth+1, fnCompare);
			}
		} else {
			for (int iThread = 0; iThread < nThread; iThread++) {
				MergeSort(src, iFronts[iThread], iEnds[iThread], nDepth+1, fnCompare);
			}
		}

		T** dst = new T*[nSize];
		int iPos = 0;
		while (true) {
			int iMinThread = -1;
			T* pMin = NULL;

			for (int iThread = 0; iThread < nThread; iThread++) {
				if (iFronts[iThread] >= iEnds[iThread])
					continue;
				if ( !pMin || (fnCompare(*src[iFronts[iThread]], *pMin) < 0) ) {
					iMinThread = iThread;
					pMin = src[iFronts[iThread]];
				}
			}

			if (!pMin)
				break;
			dst[iPos++] = pMin;
			iFronts[iMinThread]++;
		}

		//__if_exists(T*::Release) {
		//	for (int i = 0; i < nSize; i++)
		//		src[i+i0] = dst[i];
		//}
		//__if_not_exists(T*::Release) {
			memcpy(src+i0, dst, nSize*sizeof(TRefPointer<T>));
		//}

		delete [] dst;
	}

	return TRUE;
}

//=============================================================================
//-----------------------------------------------------------------------------
// TListR : using Reference Counter
//
#ifndef DECLARE_REFERENCE_CONTROL
#ifdef _DEBUG
#	define DECLARE_REFERENCE_CONTROL(CClass)\
		private:\
			mutable LONG m_nReferenceCounter;\
		public:\
			const CClass* Duplicate() const { ::InterlockedIncrement(&m_nReferenceCounter); return this; }\
			CClass* Duplicate() { ::InterlockedIncrement(&m_nReferenceCounter); return this; }\
			static CClass* NewInstance() { return new CClass; }\
			BOOL DeleteInstance() { if (::InterlockedDecrement(&m_nReferenceCounter) > 0) return FALSE; delete this; return TRUE; }\
		private:\
			void* operator new (size_t t) { CClass* pClass = ::new CClass; pClass->m_nReferenceCounter = 1; return pClass; }\
			void operator delete (void* p) { ::delete p; }\
			void* operator new (size_t t, LPCSTR lpszFileName, int nLine) { CClass* pClass = ::new(lpszFileName, nLine) CClass; pClass->m_nReferenceCounter = 1; return pClass; }\
			void operator delete (void* p, LPCSTR lpszFileName, int nLine) { ::delete p; }\

#else

#	define DECLARE_REFERENCE_CONTROL(CClass)\
		private:\
			mutable LONG m_nReferenceCounter;\
		public:\
			const CClass* Duplicate() const { ::InterlockedIncrement(&m_nReferenceCounter); return this; }\
			CClass* Duplicate() { ::InterlockedIncrement(&m_nReferenceCounter); return this; }\
			static CClass* NewInstance() { return new CClass; }\
			BOOL DeleteInstance() { if (::InterlockedDecrement(&m_nReferenceCounter) > 0) return FALSE; delete this; return TRUE; }\
		private:\
			void* operator new (size_t t) { CClass* pClass = ::new CClass; pClass->m_nReferenceCounter = 1; return pClass; }\
			void operator delete (void* p) { ::delete p; }\

#endif	// _DEBUG
#endif	// DECLARE_REFERENCE_CONTROL

template <class T> class TListR : public TDLList < T > {
	typedef TDLList < T > parent_t;
public:
	TListR(int bAutoDelete = TRUE) : TDLList<T>(bAutoDelete) { }
	virtual ~TListR(void) {
		if (parent_t::m_bAutoDeleteObject) for (parent_t::First(); parent_t::m_nItem; DeleteObject());
		else for (parent_t::First(); parent_t::m_nItem; parent_t::Remove());
	}
	TListR<T>& operator = (const TListR<T>& B);
	TListR<T>& operator = (T** pp) {
		parent_t::First();
		for (int i = 0; i < parent_t::m_nItem; i++, parent_t::Next()) {
			parent_t::SetItem(pp[i]);
		}
		parent_t::First();
		return *this;
	}

	TListR<T> operator + (const TListR<T>& B) const;
	TListR<T>& operator += (const TListR<T>& B);

	T& operator [] (int i);
	const T& operator [] (int i) const;
	T& operator () (void) { assert1(parent_t::m_pCurrentNode != NULL); return *parent_t::m_pCurrentNode->m_pData; }
	const T& operator () (void) const { assert1(parent_t::m_pCurrentNode != NULL); return *parent_t::m_pCurrentNode->m_pData; }

	virtual BOOL DeleteObject(void);

	template <class Archive> Archive& LoadFrom(Archive& ar) {
		int i, n;
		ar >> n;
		T* p;
		for (i = 0;i < n; i++) {
			p = T::NewInstance();
			ar >> *p;
			Insert(p);
		}
		return ar;
	}
};

template <class T>
TListR<T>& TListR<T>::operator = (const TListR<T>& B) {
	if (this == &B) return *this;
	if (parent_t::m_bAutoDeleteObject) parent_t::DeleteAll();
	else parent_t::RemoveAll();

	parent_t::m_bAutoDeleteObject = B.m_bAutoDeleteObject;

	return *this += B;
}

template <class T>
TListR<T> TListR<T>::operator + (const TListR<T>& B) const {
	TListR<T> result;

	result += *this;
	result += B;

	return result;
}

template <class T>
TListR<T>& TListR<T>::operator += (const TListR<T>& B) {
	if (!B.m_nItem) return *this;

	if (parent_t::m_bAutoDeleteObject) {
		const TDLListNode<T>* pNode = B.m_pFirstNode;
		for (int i = 0; i < B.m_nItem; i++, pNode = pNode->m_pNextNode) {
			T* pT = T::NewInstance();
			assert1(pT);	//
			*pT = *(pNode->m_pData);
			Attach(pT);
		}
	} else {
		const TDLListNode<T>* pNode = B.m_pFirstNode;
		for (int i = 0; i < B.m_nItem; i++, pNode = pNode->m_pNextNode) {
			Attach(pNode->m_pData);
		}
	}

	return *this;
}

template <class T>
T& TListR<T>::operator [] (int i) {
	assert1(i < parent_t::m_nItem);
	//assert1(i >= 0);
	if (i < 0) { if (parent_t::m_nItem) return *parent_t::Last(); else assert1(i < 0); }
	for( ; parent_t::m_iCurrent > i; parent_t::m_iCurrent--, parent_t::m_pCurrentNode = parent_t::m_pCurrentNode->m_pPreviousNode) ;
	for( ; parent_t::m_iCurrent < i; parent_t::m_iCurrent++, parent_t::m_pCurrentNode = parent_t::m_pCurrentNode->m_pNextNode) ;
	return *(parent_t::m_pCurrentNode->m_pData);
}
template <class T>
const T& TListR<T>::operator [] (int i) const {
	assert1(i < parent_t::m_nItem);
	//assert1(i >= 0);
	if (i < 0) { if (parent_t::m_nItem) return *parent_t::Last(); else assert1(i < 0); }
	for( ; parent_t::m_iCurrent > i; parent_t::m_iCurrent--, parent_t::m_pCurrentNode = parent_t::m_pCurrentNode->m_pPreviousNode) ;
	for( ; parent_t::m_iCurrent < i; parent_t::m_iCurrent++, parent_t::m_pCurrentNode = parent_t::m_pCurrentNode->m_pNextNode) ;
	return *(parent_t::m_pCurrentNode->m_pData);
}

template <class T>
BOOL TListR<T>::DeleteObject(void) {
	T* pData = parent_t::Pop();
	if (!pData)
		return FALSE;

	if (pData != NULL) pData->DeleteInstance();

	return TRUE;
}

//-----------------------------------------------------------------------------
// TStack
//

template < class T > class TStack {
protected:
	TBuffer<T> m_data;
	int m_iPosition;
	int m_nGrowth;
public:
	TStack(int nSize = 1024) : m_iPosition(-1), m_data(nSize) {
		m_nGrowth = 1024;
	}
	~TStack() {
	}

	void Push(T& t) {
		if (m_iPosition >= m_data.m_nSize-1)	// TStack Overflow
			Grow(m_nGrowth);

		m_data[++m_iPosition] = t;
	}
	T& Pop() {
		if (m_iPosition < 0)
			assert1(FALSE);
		return m_data[m_iPosition--];
	}

	void SetGrowth(int nGrowth = 1024) { m_nGrowth = nGrowth; if (m_nGrowth <= 0) m_nGrowth = 1024; }
	int GetGrowth() { return m_nGrowth; }

protected:
	BOOL Grow() {
		TBuffer<T> data = m_data;
		m_data.SetSize(m_data.GetSize()+m_nGrowth);
		for (int i = 0; i < data.GetSize(); i++)
			m_data[i] = data[i];
		return TRUE;
	}
	BOOL SetSize(int nSize) {
		m_iPosition = -1;
		m_data.SetSize(nSize);
		return TRUE;
	}
	BOOL Destroy() {
		m_data.SetSize(0);
		return TRUE;
	}
};

//-----------------------------------------------------------------------------
// TVector
//
template < class T > class TVector;
template < class T > class TVectorNode {
	friend class TVector < T >;
protected:
	T* m_pData;
	int m_n;
public:
	TVectorNode(int nSize) {
		assert1(nSize >= 0);
		m_pData = (m_n = nSize) ? new T[m_n] : NULL;
	}
	TVectorNode(const T* pB, int nSize) {
		assert1(nSize >= 0);
		m_pData = (m_n = nSize) ? new T[m_n] : NULL;
		memmove(m_pData, pB, sizeof(T)*nSize);
	}
	~TVectorNode() { if (m_pData && m_n) delete [] m_pData; }
};

template < class T > class TVector {
protected:
	TDLList < TVectorNode < T > > m_nodes;
	int m_nGrow;
	int m_nUpperBound;
public:
	TVector(int nGrow = 10) {
		m_nGrow = nGrow;
		m_nUpperBound = 0;
	}
	TVector(int nSize, int nGrow) {
		m_nUpperBound = nSize;
		m_nGrow = nGrow;
		m_nodes.Attach(new TVectorNode<T>(nSize));
	};
	~TVector() {};
	operator T*();
	int Grow();
	int GrowTo(int nSize) {
		while (Grow() < nSize) ;
		return m_nUpperBound;
	}
	BOOL Add(const T* pB, int nSize) {
		if (nSize <= 0)
			return FALSE;
		m_nodes.Attach(new TVectorNode<T>(pB, nSize));
		m_nUpperBound += nSize;
		return TRUE;
	}
	BOOL Delete() {
		m_nodes.DeleteAll();
		m_nUpperBound = 0;
		return TRUE;
	}
	int GetUpperBound() { return m_nUpperBound; }
	T& operator [] (int index) {
		int i = 0, n = m_nodes.N();
		for (i = 0; (i < n) && (index >= m_nodes[i].m_n); index -= m_nodes[i++].m_n) ;
		assert1(i < n);
		assert1(index > 0);
		return m_nodes[i].m_pData[index];
	}
};

template < class T >
int TVector<T>::Grow() {
	if (m_nGrow) {
		m_nodes.Attach(new TVectorNode<T>(m_nGrow));
		m_nUpperBound += m_nGrow;
	}
	return m_nUpperBound;
}

template < class T >
TVector<T>::operator T* () {
	TVectorNode<T>* pResult = new TVectorNode<T>(m_nUpperBound);
	int i, j, n = m_nodes.N();
	for (i = j = 0; i < m_nodes.N(); j += m_nodes[i++].m_n) {
		int nSize;
		if (pResult->m_n - j < m_nodes[i].m_n)
			nSize = pResult->m_n - j;
		else
			nSize = m_nodes[i].m_n;
		if (nSize <= 0)
			break;
		memmove(pResult->m_pData+j, m_nodes[i].m_pData, nSize);
	}
	m_nodes.DeleteAll();
	m_nodes.Attach(pResult);

	return pResult->m_pData;
}


//-----------------------------------------------------------------------------
// misc
//


#endif // !defined(__T_DL_LIST_HPP__INCLUDED__PWH____)
