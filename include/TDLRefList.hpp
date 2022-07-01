//=============================================================================
//
// OBSOLETE.
//
//=============================================================================
//
// TRefList.h: interface for the TDLRefList class.
// Last Edited		2012. 03. 30.
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

#if !defined(__T_REF_LIST_HPP__INCLUDED__PWH__)
#define __T_REF_LIST_HPP__INCLUDED__PWH__

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

template <class T> class TDLRefList;

#include "TRefPointer.hpp"

//-----------------------------------------------------------------------------
// TRefListNode
//
template <class T> class TRefListNode {
	friend class TDLRefList<T>;
	//friend class TListR<T>;
protected:
	TRefPointer<T> m_rData;
	TRefListNode* m_pNextNode;
	TRefListNode* m_pPreviousNode;
public:
	TRefListNode(TRefPointer<T> rData = NULL) : m_rData(rData), m_pPreviousNode(NULL), m_pNextNode(NULL) { }
};

//-----------------------------------------------------------------------------
// TDLRefList
//
template <class T> class __declspec(novtable) TDLRefList {
protected:
	int m_nItem;
	TRefListNode<T>* m_pFirstNode;
	mutable int m_iCurrent;
	mutable TRefListNode<T>* m_pCurrentNode;

public:
	TDLRefList() : m_nItem(0), m_pFirstNode(NULL), m_iCurrent(0), m_pCurrentNode(NULL) {
	}
	~TDLRefList(void) {
		for (First(); m_nItem; DeleteObject());
	}
	TDLRefList(const TDLRefList<T>& B) : m_nItem(0), m_pFirstNode(NULL), m_iCurrent(0), m_pCurrentNode(NULL) {
		*this += B;
	}
	TDLRefList<T>& operator = (const TDLRefList<T>& B) {
		if (this == &B) return *this;
		DeleteAll();
		return *this += B;
	}

	TDLRefList<T>& CopyTo(TDLRefList<T>& B) const {
		const TRefListNode<T>* pNode = m_pFirstNode;
		for (int i = 0; i < m_nItem; i++, pNode = pNode->m_pNextNode)
			B.Attach(new T(*(pNode->m_rData.Pointer())));
		return B;
	}
	TDLRefList<T>& CopyFrom(const TDLRefList<T>& B) {
		const TRefListNode<T>* pNode = B.m_pFirstNode;
		for (int i = 0; i < B.m_nItem; i++, pNode = pNode->m_pNextNode)
			Attach(new T(*(pNode->m_rData.Pointer())));
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
		TRefListNode<T>* pNode1 = m_pFirstNode;
		B.First();
		for (int i = 0; i < m_nItem; i++, pNode1 = pNode1->m_pNextNode, B.Next()) {
			if (*(pNode1->m_rData) == B())
				;
			else
				return FALSE;
		}
		return TRUE;
	}

	//-----------------------
	// Set/Get Item
	TRefPointer<T> SetItem(TRefPointer<T> pT)				{ if (!m_pCurrentNode) return NULL; return m_pCurrentNode->m_rData = pT; }
	TRefPointer<T> SetItem(int iIndex, TRefPointer<T> pT)	{ (*this)[iIndex]; return SetItem(pT); }
	TRefPointer<T> GetItem()								{ if (!m_pCurrentNode) return NULL; return m_pCurrentNode->m_rData; }
	TRefPointer<T> GetItem(int iIndex)						{ (*this)[iIndex]; return GetItem(); }
	TConstRefPointer<T> GetItem() const						{ if (!m_pCurrentNode) return NULL; return m_pCurrentNode->m_rData; }
	TConstRefPointer<T> GetItem(int iIndex) const			{ (*this)[iIndex]; return GetItem(); }

	//-----------------------
	// 
	operator T* () const { if (!m_nItem) return NULL; return GetVector(new T[m_nItem]); }
	operator TRefPointer<T>* () const { if (!m_nItem) return NULL; return GetVector(new TRefPointer<T>[m_nItem]); }
	T* GetVector(T* pT) const {
		const TRefListNode<T>* pNode = m_pFirstNode;
		for (int i = 0; i < m_nItem; i++, pNode = pNode->m_pNextNode)
			pT[i] = *(pNode->m_rData);
		return pT;
	}
	TRefPointer<T>* GetVector(TRefPointer<T> ppT[]) const {
		const TRefListNode<T>* pNode = m_pFirstNode;
		for (int i = 0; i < m_nItem; i++, pNode = pNode->m_pNextNode)
			ppT[i] = pNode->m_rData;
		return ppT;
	}
	TDLRefList<T>& operator = (TRefPointer<T> ppT[]) {
		First();
		for (int i = 0; i < m_nItem; i++, Next()) {
			SetItem(ppT[i]);
		}
		First();
		return *this;
	}

	//-----------------------
	//
	TDLRefList<T> operator + (const TDLRefList<T>& B) const;
	TDLRefList<T>& operator += (const TDLRefList<T>& B);

	int N(void) const { return m_nItem; }
	int Index(void) const {return ( (m_pCurrentNode == NULL) ? -1 : (m_iCurrent) ); }

	//-----------------------
	// Iterate
	T& operator [] (int i);
	const T& operator [] (int i) const;
	T& operator () (void) { assert1(m_pCurrentNode != NULL); return *m_pCurrentNode->m_rData; }
	const T& operator () (void) const { assert1(m_pCurrentNode != NULL); return *m_pCurrentNode->m_rData; }

	//-----------------------
	// Iterate
	TRefPointer<T> First(void)					{ m_iCurrent = 0; if (m_nItem) return (m_pCurrentNode = m_pFirstNode)->m_rData; else return NULL;}
	const TRefPointer<T> First(void) const		{ m_iCurrent = 0; if (m_nItem) return (m_pCurrentNode = m_pFirstNode)->m_rData; else return NULL;}
	TRefPointer<T> Current(void)				{ assert1(m_pCurrentNode != NULL); return m_pCurrentNode->m_rData; }
	const TRefPointer<T> Current(void) const	{ assert1(m_pCurrentNode != NULL); return m_pCurrentNode->m_rData; }
	TRefPointer<T> Next(void) {						// do as circular list
		assert1(m_pCurrentNode != NULL);
		m_pCurrentNode = m_pCurrentNode->m_pNextNode;
		m_iCurrent = (m_iCurrent+1) % m_nItem;
		return m_pCurrentNode->m_rData;
	}
	const TRefPointer<T> Next(void) const {						// do as circular list
		assert1(m_pCurrentNode != NULL);
		m_pCurrentNode = m_pCurrentNode->m_pNextNode;
		m_iCurrent = (m_iCurrent+1) % m_nItem;
		return m_pCurrentNode->m_rData;
	}
	TRefPointer<T> Previous(void) {					// do as circular list
		assert1(m_pCurrentNode != NULL);
		m_pCurrentNode = m_pCurrentNode->m_pPreviousNode;
		m_iCurrent = (m_iCurrent + m_nItem - 1) % m_nItem;
		return m_pCurrentNode->m_rData;
	}
	const TRefPointer<T> Previous(void) const {					// do as circular list
		assert1(m_pCurrentNode != NULL);
		m_pCurrentNode = m_pCurrentNode->m_pPreviousNode;
		m_iCurrent = (m_iCurrent + m_nItem - 1) % m_nItem;
		return m_pCurrentNode->m_rData;
	}
	TRefPointer<T> Last(void) {
		if (m_nItem) { m_iCurrent = m_nItem-1; return (m_pCurrentNode = m_pFirstNode->m_pPreviousNode)->m_rData;
		} else return NULL;
	}
	const TRefPointer<T> Last(void) const {
		if (m_nItem) { m_iCurrent = m_nItem-1; return (m_pCurrentNode = m_pFirstNode->m_pPreviousNode)->m_rData;
		} else return NULL;
	}

	int IsEmpty(void) const { return !m_nItem; }
	int NextNotNull(void) const {
		if (m_iCurrent < m_nItem-1) return TRUE;
		else return FALSE;
	}

	//-----------------------
	// Insert
	int Insert(TRefPointer<T> rData);
	int InsertPrevious(TRefPointer<T> rData);
	int Attach(TRefPointer<T> rData) { return InsertAt(rData, -1); }
	int Push(TRefPointer<T> rData) { return Insert(rData);}  ;
	int InsertAt(TRefPointer<T> rData, int iIndex = -2);	// iIndex :
													//    -2 : Inserts after current item
													//    -1 : Inserts as Last Item
													// other : inserts as 'iIndex'. if iIndex is larger than or equal to current item count, Inserts as Last Item
													// over nItem : Inserts as Last Item

public:
	//-----------------------
	// Delete
	TRefPointer<T> Pop(void);
	TRefPointer<T> Pop(const T* pData) { int i = Find(pData); if (i >= 0) return Pop(i); return NULL; }
	TRefPointer<T> Pop(int i) { (*this)[i]; return Pop(); }
	TRefPointer<T> Remove(void) { return Pop(); }
	TRefPointer<T> Remove(const T* pData) { return Pop(pData); }
	TRefPointer<T> Remove(int i) { return Pop(i); }
	void RemoveAll(void) { for (First(); m_nItem; Pop()); }

	BOOL DeleteObject(void);
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
		TRefListNode<T> pNodeA(m_pCurrentNode->m_rData);
		(*this)[i1];
		TRefListNode<T> pNodeB(m_pCurrentNode);
		TRefPointer<T> pC = pNodeA->m_rData;
		pNodeA->m_rData = pNodeB->m_rData;
		pNodeB->m_rData = pC;
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
	BOOL Sort(int (*fnCompare)(const T&, const T&), BOOL bAscending = TRUE, eSORTING_METHOD eSM = SM_AUTO);

private:
	void QuickSort(TRefPointer<T> data[], int left, int right);
	void QuickSort(TRefPointer<T> data[], int left, int right, int (*fnCompare)(const T&, const T&));
	void intro_sort(TRefPointer<T> data[], int left, int right, int iMaxDepth);	// by 김명식
	void intro_sort(TRefPointer<T> data[], int left, int right, int iMaxDepth, int (*fnCompare)(const T&, const T&));	// by 김명식

private:
	// intro sort
	inline int LeftChild(int i) { return ( 2 * i + 1) ; }
	inline void sort_swap(TRefPointer<T> dst[], int i0, int i1) { if (i0 == i1) return; TRefPointer<T> temp = dst[i0]; dst[i0] = dst[i1]; dst[i1] = temp; }
	void insert_sort(TRefPointer<T> dst[], int iLeft, int iRight);
	void insert_sort(TRefPointer<T> dst[], int iLeft, int iRight, int (*fnCompare)(const T&, const T&));
	void heap_sort(TRefPointer<T> dst[], int iLeft, int iRight);
	void heap_sort(TRefPointer<T> dst[], int iLeft, int iRight, int (*fnCompare)(const T&, const T&));
	void heap_down(TRefPointer<T> dst[], int i, int N, int iLeft);
	void heap_down(TRefPointer<T> dst[], int i, int N, int iLeft, int (*fnCompare)(const T&, const T&));
	TRefPointer<T> median_of_three(TRefPointer<T> dst[], int iElement1, int iElement2);
	TRefPointer<T> median_of_three(TRefPointer<T> dst[], int iElement1, int iElement2, int (*fnCompare)(const T&, const T&));

	// merge sort
	BOOL MergeSort(TRefPointer<T> src[], int i0, int i1, int nDepth);
	BOOL MergeSort(TRefPointer<T> src[], int i0, int i1, int nDepth, int (*fnCompare)(const T&, const T&));
	
	//-----------------------
	// Serializing
public:
	template <class Archive> Archive& StoreTo(Archive& ar, BOOL bFlush = FALSE) const {
		ar << m_nItem;
		if (m_nItem) {
			const TRefListNode<T>* pNode = m_pFirstNode;
			for (int i = 0; i < m_nItem; i++, pNode = pNode->m_pNextNode)
				ar << *(pNode->m_rData);
		}
		if (bFlush)
			ar.Flush();
		return ar;
	}
	template <class Archive> Archive& LoadFrom(Archive& ar) {
		int i, n;
		ar >> n;
		TRefPointer<T> p;
		for (i = 0;i < n; i++) {
			p = new T;
			ar >> *p;
			Insert(p);
		}
		return ar;
	}
	template <class Archive>
	friend Archive& operator << (Archive& ar, const TDLRefList<T>& B) {
		return B.StoreTo(ar);
	}
	template <class Archive>
	friend Archive& operator >> (Archive& ar, TDLRefList<T>& B) {
		return B.LoadFrom(ar);
	}
};

template <class T>
TDLRefList<T> TDLRefList<T>::operator + (const TDLRefList<T>& B) const {
	TDLRefList<T> result;

	result += *this;
	result += B;

	return result;
}

template <class T>
TDLRefList<T>& TDLRefList<T>::operator += (const TDLRefList<T>& B) {
	if (!B.m_nItem) return *this;

	const TRefListNode<T>* pNode = B.m_pFirstNode;
	for (int i = 0; i < B.m_nItem; i++, pNode = pNode->m_pNextNode) {
		Attach(pNode->m_rData);
	}

	return *this;
}

template <class T>
T& TDLRefList<T>::operator [] (int i) {
	assert1(i < m_nItem);
	//assert1(i >= 0);
	if (i < 0) { if (m_nItem) return *Last(); else assert1(i < 0); }
	for( ; m_iCurrent > i; m_iCurrent--, m_pCurrentNode = m_pCurrentNode->m_pPreviousNode) ;
	for( ; m_iCurrent < i; m_iCurrent++, m_pCurrentNode = m_pCurrentNode->m_pNextNode) ;
	return *(m_pCurrentNode->m_rData);
}
template <class T>
const T& TDLRefList<T>::operator [] (int i) const {
	assert1(i < m_nItem);
	//assert1(i >= 0);
	if (i < 0) { if (m_nItem) return *Last(); else assert1(i < 0); }
	for( ; m_iCurrent > i; m_iCurrent--, m_pCurrentNode = m_pCurrentNode->m_pPreviousNode) ;
	for( ; m_iCurrent < i; m_iCurrent++, m_pCurrentNode = m_pCurrentNode->m_pNextNode) ;
	return *(m_pCurrentNode->m_rData);
}

template <class T>
int TDLRefList<T>::Insert(TRefPointer<T> rData) {
	TRefListNode<T>* pNode = new TRefListNode<T>(rData);
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
int TDLRefList<T>::InsertAt(TRefPointer<T> rData, int iIndex) {
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
		return InsertPrevious(rData);
	} else if (iIndex-1 != m_iCurrent) {
		(*this)[iIndex-1];
	}
	return Insert(rData);
}

template <class T>
int TDLRefList<T>::InsertPrevious(TRefPointer<T> rData) {
	if (m_nItem) {
		Previous();
		Insert(rData);
		if (m_pFirstNode->m_pPreviousNode == m_pCurrentNode) { m_pFirstNode = m_pCurrentNode; m_iCurrent = 0; }
	} else Insert(rData);
	return m_iCurrent;
}

template <class T>
TRefPointer<T> TDLRefList<T>::Pop(void) {
	if (!m_nItem) return NULL;
	if (!m_pCurrentNode)
		return FALSE;
	assert1(m_pCurrentNode != NULL);

	TRefListNode<T>* pNode = m_pCurrentNode;

	TRefPointer<T> rData = m_pCurrentNode->m_rData;

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

	return rData;
}

template <class T>
BOOL TDLRefList<T>::DeleteObject(void) {
	Pop();
	return TRUE;
}

template <class T>
void TDLRefList<T>::Invert(void) {
	if (m_nItem <= 1)
		return;

	TRefListNode<T>* pNode;
	TRefListNode<T>* pNodeTemp;

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
int TDLRefList<T>::Find(const T& B) const {
	if (m_pFirstNode == NULL) return -1;
	int i;
	for (First(), i = 0; i < m_nItem; i++) {
		if ((*this)[i] == B) return i;
	}
	return -1;
}

template <class T>
int TDLRefList<T>::Find(const T* pB) const {
	if (m_pFirstNode == NULL) return -1;
	First();
	for (int i = 0; i < m_nItem; i++) {
		(*this)[i];
		if (m_pCurrentNode->m_rData.Pointer() == pB) return i;
	}
	return -1;
}

//-----------------------------------------------------------------------------

template <class T>
BOOL TDLRefList<T>::Sort(BOOL bAscending, eSORTING_METHOD eSM) {
	if (!m_nItem)
		return TRUE;
	int i;
	TRefListNode<T>* pNode = m_pFirstNode;

	TRefPointer<T>* backup = *this;

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
			pNode->m_rData = backup[i];
			pNode = pNode->m_pNextNode;
		}
	} else {
		for (i = m_nItem-1; i >= 0; i--) {
			pNode->m_rData = backup[i];
			pNode = pNode->m_pNextNode;
		}
	}

	delete [] backup;

	First();

	return TRUE;
}
template <class T>
BOOL TDLRefList<T>::Sort(int (*fnCompare)(const T&, const T&), BOOL bAscending, eSORTING_METHOD eSM) {
	if (!m_nItem)
		return TRUE;
	int i;
	TRefListNode<T>* pNode = m_pFirstNode;

	//std::vector<T*> backup;
	//backup.reserve(m_nItem);
	//const TListNode<T>* pNode = m_pFirstNode;
	//for (int i = 0; i < m_nItem; i++, pNode = pNode->m_pNextNode)
	//	backup.push_back(pNode->m_pData);

	if (fnCompare == NULL)
		return FALSE;

	TRefPointer<T>* backup = *this;

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
			pNode->m_rData = backup[i];
			pNode = pNode->m_pNextNode;
		}
	} else {
		for (i = m_nItem-1; i >= 0; i--) {
			pNode->m_rData = backup[i];
			pNode = pNode->m_pNextNode;
		}
	}

	delete [] backup;

	First();

	return TRUE;
}

//-----------------------------------------------------------------------------

template <class T>
void TDLRefList<T>::QuickSort(TRefPointer<T> data[], int left, int right, int (*fnCompare)(const T&, const T&)) {
	if (left < right) {
		int i = left, j = right+1;
		TRefPointer<T>* pivot = data+left;
		do {
			do i++; while ((i < right) && (fnCompare(*(data[i]), **pivot) < 0));
			do j--; while ((j >= left) && (fnCompare(*(data[j]), **pivot) > 0));
			if (i < j) {
				TRefPointer<T> temp = data[i];
				data[i] = data[j];
				data[j] = temp;
			}
		} while (i < j);
		TRefPointer<T> temp = data[left];
		data[left] = data[j];
		data[j] = temp;

		QuickSort(data, left, j-1, fnCompare);
		QuickSort(data, j+1, right, fnCompare);
	}
}

//-----------------------------------------------------------------------------

template < class T >
TRefPointer<T> TDLRefList<T>::median_of_three(TRefPointer<T> dst[], int iElement1, int iElement2, int (*fnCompare)(const T&, const T&)) {
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
void TDLRefList<T>::heap_down(TRefPointer<T> dst[], int i, int N, int iLeft, int (*fnCompare)(const T&, const T&)) {
	int iChild;
	TRefPointer<T> data;
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
void TDLRefList<T>::heap_sort(TRefPointer<T> dst[], int iLeft, int iRight, int (*fnCompare)(const T&, const T&)) {
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
void TDLRefList<T>::insert_sort(TRefPointer<T> dst[], int iLeft, int iRight,  int (*fnCompare)(const T&, const T&)) {
	iRight++;
	for (int i = iLeft + 1; i < iRight; i++) {
		TRefPointer<T> temp = dst[i];
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

template < class T >
void TDLRefList<T>::intro_sort(TRefPointer<T> dst[], int iLeft, int iRight, int iMaxDepth, int (*fnCompare)(const T&, const T&)) {
	if (iLeft >= iRight)
		return;
	if ((iRight - iLeft + 1) > 20) {
		if (0 < iMaxDepth) {
			int iLow = iLeft;
			int iHigh = iRight - 1;
			TRefPointer<T> pivot = median_of_three(dst, iLeft, iRight, fnCompare);
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

template < class T >
BOOL TDLRefList<T>::MergeSort(TRefPointer<T> src[], int i0, int i1, int nDepth, int (*fnCompare)(const T&, const T&)) {
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

		TRefPointer<T>* dst = new TRefPointer<T>[nSize];
		int iPos = 0;
		while (true) {
			int iMinThread = -1;
			TRefPointer<T> pMin = NULL;

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

		//__if_exists(TRefPointer<T>::Release) {
			for (int i = 0; i < nSize; i++)
				src[i+i0] = dst[i];
		//}
		//__if_not_exists(TRefPointer<T>::Release) {
		//	memcpy(src+i0, dst, nSize*sizeof(TRefPointer<T>));
		//}

		delete [] dst;
	}

	return true;
}


#endif // !defined(__T_REF_LIST_HPP__INCLUDED__PWH__)
