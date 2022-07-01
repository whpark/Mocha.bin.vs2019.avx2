///////////////////////////////////////////////////////////////////////////////
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
//					2013. 06. 09. : x64 호환성 추가
//					                PopFirst(), PopLast(), DeleteFirst(), DeleteLast()
//
//					2013. 06. 19. : TVectorList 추가. (TVList.hpp)
//
//					2013. 06. 19. : TRefList, TList -> TDoublyLinkedList 로 합침.
//					                Sort 루틴을 TListSorter.inl 로 빼냄.
//					                GetVector(T**) -> GetVector(std::vector<TP>)로 수정
//
//					2013. 06. 22. : TVList + TList -> Iterator 분리해서 합침. ListIter 추가
//
//					2013. 08. 15. : Struct Aling - 8
//
//					2016. 01. 13. : __TLIST_NEXT_NODE_OF_LAST_NODE_INDICATES_FIRST_NODE_ = 0 추가 - 마지막 노드의 Next 가 NULL 이 되도록 수정.
//									begin(), end() 추가 - TDoublyLinkedList 에만....
//
// PWH
///////////////////////////////////////////////////////////////////////////////

#if !defined(__T_LIST_HPP__INCLUDED__PWH__)
#define __T_LIST_HPP__INCLUDED__PWH__

#pragma once

//////////////////////////////////////////////////////////////////////

#include <vector>

#include <afxmt.h>
#include "default.defines.h"
#include "TRefPointer.hpp"

#pragma intrinsic(_InterlockedIncrement, _InterlockedDecrement)

#define __Lock_List__(lst)				IListLocker::Locker locker(lst)
#define __Lock_List_name(lst, name)		IListLocker::Locker name(lst)


#define __List_throw_exception__(msg) throw CListException(__FUNCTION__, msg)
#ifdef _DEBUG
#	define __List_check_exception__(condition, msg) { if (!(condition)) { __List_throw_exception__(msg); assert1(false); } }
#else
#	define __List_check_exception__(condition, msg)
#endif

#define __TLIST_NEXT_NODE_OF_LAST_NODE_INDICATES_FIRST_NODE_ 0

#include "IList.hpp"



#pragma pack(push, 8)




template < class T, typename INDEX_T = intXX_t, typename SIZE_T = intXX_t > class TList;
template < class T, typename INDEX_T = intXX_t, typename SIZE_T = intXX_t > class TRefList;
template < class T, typename INDEX_T = intXX_t, typename SIZE_T = intXX_t > class TVList;
template < class T, typename INDEX_T = intXX_t, typename SIZE_T = intXX_t > class TRefVList;

template < class T, typename INDEX_T = intXX_t, typename SIZE_T = intXX_t > class TListIter;
template < class T, typename INDEX_T = intXX_t, typename SIZE_T = intXX_t > class TRefListIter;
template < class T, typename INDEX_T = intXX_t, typename SIZE_T = intXX_t > class TVListIter;
template < class T, typename INDEX_T = intXX_t, typename SIZE_T = intXX_t > class TRefVListIter;

//-----------------------------------------------------------------------------
template < class T, class TP, class TPC, typename INDEX_T, typename SIZE_T > class TDoublyLinkedListIter;
template < class T, class TP, class TPC, typename INDEX_T, typename SIZE_T > class TDoublyLinkedList;
template < class T, class TP, class TPC, typename INDEX_T, typename SIZE_T > class TVectorListIter;
template < class T, class TP, class TPC, typename INDEX_T, typename SIZE_T > class TVectorList;


//=============================================================================
// TDoublyLinkedList
//


//-----------------------------------------------------------------------------
// double-linked list node
//
template < class T, class TP, class TPC, typename INDEX_T, typename SIZE_T >
class TDoublyLinkedListNode {
	friend class TDoublyLinkedListIter< T, TP, TPC, INDEX_T, SIZE_T >;
	friend class TDoublyLinkedList< T, TP, TPC, INDEX_T, SIZE_T >;
protected:
	TP m_pData;
	TDoublyLinkedListNode* m_pPreviousNode;
	TDoublyLinkedListNode* m_pNextNode;
public:
	TDoublyLinkedListNode(TP pData = TP(NULL)) : m_pData(pData), m_pPreviousNode(NULL), m_pNextNode(NULL) { }

	T& operator * () {
		assert1(m_pData);
		return *m_pData;
	}
	const T& operator * () const {
		assert1(m_pData);
		return *m_pData;
	}
	TDoublyLinkedListNode& operator ++ () {
		if (m_pNextNode) {
			auto* pNode = m_pNextNode;
			m_pData = pNode->m_pData;
			m_pPreviousNode = pNode->m_pPreviousNode;
			m_pNextNode = pNode->m_pNextNode;
		} else {
			m_pData = NULL;
			m_pPreviousNode = this;
			m_pNextNode = NULL;
		}
		return *this;
	}
	BOOL operator == (const TDoublyLinkedListNode& B) const { return (m_pData == B.m_pData) && (m_pNextNode == B.m_pNextNode); }
	BOOL operator != (const TDoublyLinkedListNode& B) const { return (m_pData != B.m_pData) || (m_pNextNode != B.m_pNextNode); }
};


//-----------------------------------------------------------------------------
// TDoublyLinkedListIter
//
template < class T, class TP, class TPC, typename INDEX_T, typename SIZE_T >
class TDoublyLinkedListIter : public IListIter< T, TP, TPC, INDEX_T, SIZE_T > {
	friend class TDoublyLinkedList< T, TP, TPC, INDEX_T, SIZE_T >;
	friend class IList< T, TP, TPC, INDEX_T, SIZE_T, TDoublyLinkedListIter< T, TP, TPC, INDEX_T, SIZE_T > >;
protected:
	typedef IListIter< T, TP, TPC, INDEX_T, SIZE_T > parent_t;
	typedef TDoublyLinkedListNode< T, TP, TPC, INDEX_T, SIZE_T > node_t;
	typedef T value_type;
	typedef TDoublyLinkedListIter< T, TP, TPC, INDEX_T, SIZE_T > this_t;

	node_t* volatile *m_ppFirstNode;
	mutable node_t* m_pCurrentNode;

protected:
	TDoublyLinkedListIter(void* ppFirstNode, SIZE_T nItem, std::atomic<long>* pInterlockCounter, CCriticalSection* pCS)
		: parent_t(nItem, pInterlockCounter, pCS), m_ppFirstNode((node_t**)ppFirstNode), m_pCurrentNode(NULL)
	{
		__List_check_exception__(m_ppFirstNode, "_FirstNode is NULL");
	}

public:
	TDoublyLinkedListIter(const TDoublyLinkedListIter& B)
		: parent_t((const parent_t&)B)
	{
		*this = B;
	}
	TDoublyLinkedListIter& operator = (const TDoublyLinkedListIter& B) {
		if (this == &B)
			return *this;

		(parent_t&)*this = (const parent_t&)B;

		m_ppFirstNode = B.m_ppFirstNode;
		m_pCurrentNode = B.m_pCurrentNode;

		return *this;
	}

	virtual ~TDoublyLinkedListIter() {
		Release();
	}

	void Release() {
		m_ppFirstNode = NULL;
		m_pCurrentNode = NULL;

		parent_t::Release();
	}

protected:
	virtual TP*		GetPPItem()						{ return m_pCurrentNode ? &m_pCurrentNode->m_pData : NULL; }
	virtual TPC*	GetPPItem() const				{ return m_pCurrentNode ? (TPC*)&m_pCurrentNode->m_pData : NULL; }
	virtual TP*		GetPPItem(INDEX_T iIndex)		{ node_t* pNode = GetNode(iIndex); return pNode ? &pNode->m_pData : NULL; }
	virtual TPC*	GetPPItem(INDEX_T iIndex) const	{ node_t* pNode = GetNode(iIndex); return pNode ? (TPC*)(&pNode->m_pData) : NULL; }

	virtual void SetNullPosition() const {
		__Lock_List__(*this);
		m_pCurrentNode = NULL;
		parent_t::m_iCurrent = 0;
	}

	node_t* FirstNode() const {
		__Lock_List__(*this);
		parent_t::m_iCurrent = 0;
		if (parent_t::m_nItem && m_ppFirstNode && *m_ppFirstNode) {
			m_pCurrentNode = *m_ppFirstNode;
			return m_pCurrentNode;
		}
		SetNullPosition();
		return NULL;
	}
	node_t* LastNode() const {
		__Lock_List__(*this);
		parent_t::m_iCurrent = 0;
		if (parent_t::m_nItem && m_ppFirstNode && *m_ppFirstNode) {
			parent_t::m_iCurrent = parent_t::m_nItem-1;
			m_pCurrentNode = (*m_ppFirstNode)->m_pPreviousNode;
			return m_pCurrentNode;
		}
		SetNullPosition();
		return NULL;
	}

	inline node_t* GetNode(INDEX_T iIndex) const {
		__Lock_List__(*this);

		if (!m_ppFirstNode || !*m_ppFirstNode)
			return NULL;
		if (m_pCurrentNode && (parent_t::m_iCurrent == iIndex))
			return m_pCurrentNode;
		if (iIndex == 0)
			return FirstNode();
		if (iIndex == (INDEX_T)-1)
			return LastNode();
		if (!m_pCurrentNode) parent_t::First();
		for( ; parent_t::m_iCurrent > iIndex; parent_t::m_iCurrent--, m_pCurrentNode = m_pCurrentNode->m_pPreviousNode) ;
		for( ; parent_t::m_iCurrent < iIndex; parent_t::m_iCurrent++, m_pCurrentNode = m_pCurrentNode->m_pNextNode) ;

		return m_pCurrentNode;
	}
};


//-----------------------------------------------------------------------------
// TDoublyLinkedList
//
template < class T, class TP, class TPC, typename INDEX_T, typename SIZE_T >
class TDoublyLinkedList : public IList< T, TP, TPC, INDEX_T, SIZE_T, TDoublyLinkedListIter< T, TP, TPC, INDEX_T, SIZE_T > > {
protected:
	typedef IList< T, TP, TPC, INDEX_T, SIZE_T, TDoublyLinkedListIter< T, TP, TPC, INDEX_T, SIZE_T > > parent_t;
	typedef TDoublyLinkedListIter< T, TP, TPC, INDEX_T, SIZE_T > iter_t;
	typedef TDoublyLinkedListNode< T, TP, TPC, INDEX_T, SIZE_T > node_t;
	typedef T value_type;
	typedef TDoublyLinkedList< T, TP, TPC, INDEX_T, SIZE_T > this_t;
protected:
	node_t* m_pFirstNode;

public:
	//-----------------------
	// Constructor & Destructor
	TDoublyLinkedList(bool bAutoDeleteObject = true, bool bInterlock = false)
		: IList< T, TP, TPC, INDEX_T, SIZE_T, TDoublyLinkedListIter< T, TP, TPC, INDEX_T, SIZE_T > >(bAutoDeleteObject, bInterlock, &m_pFirstNode),
		m_pFirstNode(NULL)
	{
	}

public:
	TDoublyLinkedList(const TDoublyLinkedList& B)
		: IList< T, TP, TPC, INDEX_T, SIZE_T, TDoublyLinkedListIter< T, TP, TPC, INDEX_T, SIZE_T > >(true, false, &m_pFirstNode),
		m_pFirstNode(NULL)
	{
		*this = B;
	}
	TDoublyLinkedList& operator = (const TDoublyLinkedList& B) { if (this != &B) parent_t::CopyFrom(B); return *this; }

	template < class TLIST >
	TDoublyLinkedList(const TLIST& B)
		: IList< T, TP, TPC, INDEX_T, SIZE_T, TDoublyLinkedListIter< T, TP, TPC, INDEX_T, SIZE_T > >(true, false, &m_pFirstNode),
		m_pFirstNode(NULL)
	{
		*this = B;
	}
	template < class TLIST >
	TDoublyLinkedList& operator = (const TLIST& B) { if ((void*)this != (void*)&B) parent_t::CopyFrom(B); return *this; }

	virtual ~TDoublyLinkedList() {
		DeleteAll();
	}

	//-----------------------
	// operators
	//
	template < class TLIST >
	TDoublyLinkedList& operator + (const TLIST& B)	{ TDoublyLinkedList C(*this); C.AddFrom(B); return *this; }
	template < class TLIST >
	TDoublyLinkedList& operator += (const TLIST& B)	{ parent_t::AddFrom(B); return *this; }


public:

	//-----------------------
	// Insert
	// iIndex :
	//    -2 : Inserts after current item
	//    -1 : Inserts as Last Item
	// other : inserts as 'iIndex'. if iIndex is larger than or equal to current item count, Inserts as Last Item
	// over nItem : Inserts as Last Item
	virtual INDEX_T InsertAt(TPC pData, INDEX_T iIndex = -2) {
		__Lock_List__(*this);

		node_t* pNode = new node_t((TP)pData);
		if (pNode == NULL) return -1;
		if (parent_t::m_nItem) {
			node_t* pNodeInsert = NULL;

			if (iIndex == -2) {										// as is.
				if (!parent_t::m_pCurrentNode) parent_t::FirstNode();
				pNodeInsert = parent_t::m_pCurrentNode;
			} else if ( (iIndex == -1) || (iIndex >= parent_t::m_nItem) ) {	// insert at last
				pNodeInsert = parent_t::LastNode();
			} else if (parent_t::m_nItem && (iIndex == 0)) {					// insert at first
#if (__TLIST_NEXT_NODE_OF_LAST_NODE_INDICATES_FIRST_NODE_)
				pNodeInsert = m_pFirstNode->m_pPreviousNode;
				m_iCurrent = -1;
#else
				node_t* pLastNode = m_pFirstNode->m_pPreviousNode;
				pNode->m_pPreviousNode = pLastNode;
				pNode->m_pNextNode = m_pFirstNode;
				m_pFirstNode->m_pPreviousNode = pNode;
				m_pFirstNode = pNode;

				parent_t::m_iCurrent = 0;
				parent_t::m_pCurrentNode = pNode;
				parent_t::m_nItem++;

				return parent_t::m_iCurrent;
#endif
			} else if (iIndex-1 == parent_t::m_iCurrent) {
				pNodeInsert = parent_t::m_pCurrentNode;
			} else {
				pNodeInsert = parent_t::GetNode(iIndex-1);
			}

#if (__TLIST_NEXT_NODE_OF_LAST_NODE_INDICATES_FIRST_NODE_)
			pNode->m_pNextNode = pNodeInsert->m_pNextNode;
			pNodeInsert->m_pNextNode = pNode;
			pNode->m_pNextNode->m_pPreviousNode = pNode;
#else
			pNode->m_pNextNode = pNodeInsert->m_pNextNode;
			pNodeInsert->m_pNextNode = pNode;
			if (pNode->m_pNextNode)
				pNode->m_pNextNode->m_pPreviousNode = pNode;
			else
				m_pFirstNode->m_pPreviousNode = pNode;
#endif
			pNode->m_pPreviousNode = pNodeInsert;
			parent_t::m_iCurrent++;
		} else {
			m_pFirstNode = pNode;
#if (__TLIST_NEXT_NODE_OF_LAST_NODE_INDICATES_FIRST_NODE_)
			m_pFirstNode->m_pNextNode = m_pFirstNode;
#else
			m_pFirstNode->m_pNextNode = NULL;
#endif
			m_pFirstNode->m_pPreviousNode = m_pFirstNode;
		}
		parent_t::m_pCurrentNode = pNode;
		parent_t::m_nItem++;
		if (iIndex == 0)
			m_pFirstNode = pNode;

		return parent_t::m_iCurrent;
	}

public:
	//-----------------------
	// Delete

	void RemoveAll(void) {
		__Lock_List__(*this);

		for (node_t* pNode = m_pFirstNode; parent_t::m_nItem; parent_t::m_nItem--) {
			node_t* pNodeNext = pNode->m_pNextNode;
			__if_exists(TP::Release) { pNode->m_pData.Release(); }
			delete pNode;
			pNode = pNodeNext;
		}
		m_pFirstNode = parent_t::m_pCurrentNode = NULL;
		parent_t::m_iCurrent = 0;
	}
	virtual TP RemoveAt(INDEX_T iIndex) {
		__Lock_List__(*this);

		if (!parent_t::m_nItem) return TP(NULL);

		if (iIndex == -2) {										// as is.
		} else if ( (iIndex == -1) || (iIndex >= parent_t::m_nItem) ) {	// insert at last
			parent_t::LastNode();
		} else if (parent_t::m_nItem && (iIndex == 0)) {					// insert at first
			parent_t::FirstNode();
		} else if (iIndex == parent_t::m_iCurrent) {
		} else {
			parent_t::GetPPItem(iIndex);
		}

		if (!parent_t::m_pCurrentNode)
			return TP(NULL);

		node_t* pNode = parent_t::m_pCurrentNode;

		TP pData = parent_t::m_pCurrentNode->m_pData;

		if (parent_t::m_nItem == 1) {
			delete parent_t::m_pCurrentNode;
			m_pFirstNode = parent_t::m_pCurrentNode = NULL;
			parent_t::m_nItem = parent_t::m_iCurrent = 0;
		} else {
#if (__TLIST_NEXT_NODE_OF_LAST_NODE_INDICATES_FIRST_NODE_)
			m_pCurrentNode->m_pNextNode->m_pPreviousNode = m_pCurrentNode->m_pPreviousNode;
			m_pCurrentNode->m_pPreviousNode->m_pNextNode = m_pCurrentNode->m_pNextNode;
#else
			if (parent_t::m_pCurrentNode->m_pNextNode)
				parent_t::m_pCurrentNode->m_pNextNode->m_pPreviousNode = parent_t::m_pCurrentNode->m_pPreviousNode;
			else
				m_pFirstNode->m_pPreviousNode = parent_t::m_pCurrentNode->m_pPreviousNode;
			if (parent_t::m_pCurrentNode != m_pFirstNode)
				parent_t::m_pCurrentNode->m_pPreviousNode->m_pNextNode = parent_t::m_pCurrentNode->m_pNextNode;
#endif

			if (parent_t::m_pCurrentNode == m_pFirstNode) m_pFirstNode = parent_t::m_pCurrentNode = parent_t::m_pCurrentNode->m_pNextNode;
			else parent_t::m_pCurrentNode = parent_t::m_pCurrentNode->m_pNextNode;
			delete pNode;
			parent_t::m_nItem--;
			if (parent_t::m_iCurrent == parent_t::m_nItem) {
				parent_t::m_iCurrent--;
				parent_t::m_pCurrentNode = m_pFirstNode->m_pPreviousNode;
			}
		}

		return pData;
	}

	void DeleteAll(void) {
		if (!parent_t::m_bAutoDeleteObject) {
			RemoveAll();
			return;
		}

		__Lock_List__(*this);

		for (node_t* pNode = m_pFirstNode; parent_t::m_nItem; parent_t::m_nItem--) {
			node_t* pNodeNext = pNode->m_pNextNode;
			parent_t::DeleteObjectOnly(pNode->m_pData);
			delete pNode;
			pNode = pNodeNext;
		}
		m_pFirstNode = parent_t::m_pCurrentNode = NULL;
		parent_t::m_iCurrent = 0;
	}

	void Invert(void) {
		__Lock_List__(*this);

		if (parent_t::m_nItem <= 1)
			return;

#if (__TLIST_NEXT_NODE_OF_LAST_NODE_INDICATES_FIRST_NODE_)
		node_t* pNode;
		node_t* pNodeTemp;

		pNode = m_pFirstNode;
		for (INDEX_T i = 0; i < m_nItem; i++) {
			// Swap Previous <-> Next
			pNodeTemp = pNode->m_pNextNode;
			pNode->m_pNextNode = pNode->m_pPreviousNode;
			pNode->m_pPreviousNode = pNodeTemp;

			// Next. (Previous is the Next Item)
			pNode = pNode->m_pPreviousNode;
		}

		m_pFirstNode = pNode->m_pNextNode;
		m_iCurrent = (m_nItem - 1) - m_iCurrent;
#else
		node_t* pNode;
		node_t* pNodeTemp;

		pNode = m_pFirstNode;
		for (INDEX_T i = 0; i < parent_t::m_nItem; i++) {
			// Swap Previous <-> Next
			if (i == parent_t::m_nItem-1)
				pNodeTemp = m_pFirstNode;
			else
				pNodeTemp = pNode->m_pNextNode;
			pNode->m_pNextNode = pNode->m_pPreviousNode;
			pNode->m_pPreviousNode = pNodeTemp;

			// Next. (Previous is the Next Item)
			pNode = pNode->m_pPreviousNode;
		}

		m_pFirstNode = pNode->m_pNextNode;
		m_pFirstNode->m_pPreviousNode->m_pNextNode = NULL;
		parent_t::m_iCurrent = (parent_t::m_nItem - 1) - parent_t::m_iCurrent;
#endif
	}

	node_t begin() {
		if (m_pFirstNode)
			return *m_pFirstNode;
		static node_t empty;
		return empty;
	}
	node_t begin() const {
		if (m_pFirstNode)
			return *m_pFirstNode;
		static node_t empty;
		return empty;
	}
	node_t end() const {
		static node_t empty;
		return empty;
	}
};




//=============================================================================
// TVectorList
//


//-----------------------------------------------------------------------------
// TVectorListIter
//
template < class T, class TP, class TPC, typename INDEX_T, typename SIZE_T >
class TVectorListIter : public IListIter< T, TP, TPC, INDEX_T, SIZE_T > {
	friend class TVectorList<T, TP, TPC, INDEX_T, SIZE_T>;
	friend class IList< T, TP, TPC, INDEX_T, SIZE_T, TVectorListIter< T, TP, TPC, INDEX_T, SIZE_T > >;
protected:

	typedef IListIter< T, TP, TPC, INDEX_T, SIZE_T > parent_t;
	typedef std::vector < TP > BLOCK_T;
	typedef TDoublyLinkedList < BLOCK_T, BLOCK_T*, const BLOCK_T*, INDEX_T, SIZE_T > DATA_T;

	DATA_T* m_pData;
	mutable INDEX_T m_iCurrentBlock, m_iCurrentSub, m_iSubL, m_iSubR;
	mutable TP* m_ppCurrentData;

protected:
	TVectorListIter(void* pData, SIZE_T nItem, std::atomic<long>* pInterlockCounter, CCriticalSection* pCS)
		: IListIter< T, TP, TPC, INDEX_T, SIZE_T >(nItem, pInterlockCounter, pCS),
		m_pData((DATA_T*)pData), m_iCurrentBlock(0), m_iCurrentSub(0), m_iSubL(0), m_iSubR(0), m_ppCurrentData(NULL)
	{
	};
public:
	TVectorListIter(const TVectorListIter& B)
		: IListIter< T, TP, TPC, INDEX_T, SIZE_T >((const IListIter< T, TP, TPC, INDEX_T, SIZE_T >&)B),
		m_pData(NULL), m_iCurrentBlock(0), m_iCurrentSub(0), m_iSubL(0), m_iSubR(0), m_ppCurrentData(NULL)
	{
		*this = B;
	}
	TVectorListIter& operator = (const TVectorListIter& B) {
		if (this == &B)
			return *this;
		Release();

		(IListIter< T, TP, TPC, INDEX_T, SIZE_T >&)*this = (const IListIter< T, TP, TPC, INDEX_T, SIZE_T >&)B;

		m_pData				= B.m_pData;
		parent_t::m_nItem				= B.m_nItem;
		m_iCurrentBlock		= B.m_iCurrentBlock;
		m_iCurrentSub		= B.m_iCurrentSub;
		m_iSubL				= B.m_iSubL;
		m_iSubR				= B.m_iSubR;
		m_ppCurrentData		= B.m_ppCurrentData;

		return *this;
	}
	virtual ~TVectorListIter() {
		Release();
	}

	void Release() {
		m_pData = NULL;
		parent_t::m_nItem = 0;
		m_iCurrentBlock = 0;
		m_iCurrentSub = 0;
		m_iSubL = 0;
		m_iSubR = 0;
		m_ppCurrentData = NULL;

		IListIter< T, TP, TPC, INDEX_T, SIZE_T >::Release();
	}

protected:
	virtual TP*		GetPPItem()						{ return m_ppCurrentData; }
	virtual TPC*	GetPPItem() const				{ return (TPC*)m_ppCurrentData; }
	virtual TP*		GetPPItem(INDEX_T iIndex)		{ return GetNode(iIndex); }
	virtual TPC*	GetPPItem(INDEX_T iIndex) const	{ return GetConstNode(iIndex); }

public:
	virtual void SetNullPosition() const {
		__Lock_List__(*this);
		parent_t::m_iCurrent = 0;
		m_iCurrentBlock = 0;
		m_iCurrentSub = 0;
		m_iSubL = 0;
		m_iSubR = 0;
		m_ppCurrentData = NULL;
	}

protected:
	inline void IterSet(INDEX_T iBlock, INDEX_T iSubL, INDEX_T iSubR, INDEX_T iIndex) const {
		m_iCurrentBlock = iBlock;
		m_iSubL = iSubL;
		m_iSubR = iSubR;
		parent_t::m_iCurrent = iIndex;
		m_iCurrentSub = iIndex - m_iSubL;
		if (m_pData)
			m_ppCurrentData = &((*m_pData)[m_iCurrentBlock][m_iCurrentSub]);
		else
			m_ppCurrentData = NULL;
	}

	TP* GetNode(INDEX_T iIndex = 0) const {		// iIndex : 0 : First, n : n th Item, -1 : Last Item.
		__Lock_List__(*this);
		if (!parent_t::m_nItem)
			return NULL;

		if (iIndex == 0) {
			parent_t::m_iCurrent = 0;
			m_iCurrentBlock = 0;
			m_iCurrentSub = 0;
			m_iSubL = 0;
			if (!parent_t::m_nItem || !m_pData || !m_pData->N()) { m_iSubR = 0; m_ppCurrentData = NULL; return NULL; }
			DATA_T& data = *m_pData;
			m_iSubR = m_iSubL + (INDEX_T)data[m_iCurrentBlock].size();
			m_ppCurrentData = &data[m_iCurrentBlock][m_iCurrentSub];
			return m_ppCurrentData;
		} else if (iIndex == (INDEX_T)-1) {
			if (!parent_t::m_nItem || !m_pData || !m_pData->N()) {
				parent_t::m_iCurrent = 0;
				m_iCurrentBlock = 0;
				m_iCurrentSub = 0;
				m_iSubL = 0;
				m_iSubR = 0;
				m_ppCurrentData = NULL;
				return NULL;
			}
			DATA_T& data = *m_pData;
			INDEX_T nItem = parent_t::m_nItem;
			parent_t::m_iCurrent = nItem-1;
			m_iCurrentBlock = data.N()-1;
			m_iCurrentSub = (INDEX_T)data[m_iCurrentBlock].size()-1;
			m_iSubL = nItem - (INDEX_T)data[m_iCurrentBlock].size();
			m_iSubR = nItem;
			m_ppCurrentData = &data[m_iCurrentBlock][m_iCurrentSub];
			return m_ppCurrentData;
		}
		if (iIndex >= parent_t::m_nItem)
			return NULL;

		DATA_T& data = *m_pData;

		if ( (m_iSubL <= iIndex) && (iIndex < m_iSubR) ) {
			parent_t::m_iCurrent = iIndex;
			m_iCurrentSub = iIndex - m_iSubL;
			m_ppCurrentData = &data[m_iCurrentBlock][m_iCurrentSub];
			return m_ppCurrentData;
		}

		int iSubL = m_iSubL, iSubR = m_iSubR;
		INDEX_T iDiff = iIndex - parent_t::m_iCurrent;
		if (iDiff > 0) {
			for (INDEX_T iBlock = m_iCurrentBlock+1; iBlock < (INDEX_T)data.N(); iBlock++) {
				SIZE_T nBlockLen = (SIZE_T)data[iBlock].size();
				iSubL = iSubR;
				iSubR += nBlockLen;
				if (iIndex < iSubR) {
					IterSet(iBlock, iSubL, iSubR, iIndex);
					return m_ppCurrentData;
				}
			}
		} else if (iDiff < 0) {
			for (INDEX_T iBlock = m_iCurrentBlock-1; iBlock >= 0; iBlock--) {
				SIZE_T nBlockLen = (SIZE_T)data[iBlock].size();
				iSubR = iSubL;
				iSubL -= nBlockLen;
				if (iSubL <= iIndex) {
					IterSet(iBlock, iSubL, iSubR, iIndex);
					return (TP*)m_ppCurrentData;
				}
			}
		}
		__List_check_exception__(false, "UNKNOWN ERROR");
		return NULL;
	}
	TPC* GetConstNode(INDEX_T iIndex = 0) const { return (TPC*) GetNode(iIndex); }

};


//-----------------------------------------------------------------------------
// TVectorList
//
template < class T, class TP, class TPC, typename INDEX_T, typename SIZE_T >
class TVectorList : public IList< T, TP, TPC, INDEX_T, SIZE_T, TVectorListIter< T, TP, TPC, INDEX_T, SIZE_T > > {
protected:
	typedef IList< T, TP, TPC, INDEX_T, SIZE_T, TVectorListIter< T, TP, TPC, INDEX_T, SIZE_T > > parent_t;
	typedef TVectorListIter<T, TP, TPC, INDEX_T, SIZE_T> iter_t;

	typedef std::vector < TP > BLOCK_T;
	typedef TDoublyLinkedList < BLOCK_T, BLOCK_T*, const BLOCK_T*, INDEX_T, SIZE_T > DATA_T;

protected:
	DATA_T m_data;
	SIZE_T m_nGrowth;

public:
	// Constructor & Destructor, Copy
	TVectorList(bool bAutoDeleteObject = true, bool bInterlock = false, SIZE_T nGrowth = 1000)
		: IList< T, TP, TPC, INDEX_T, SIZE_T, TVectorListIter< T, TP, TPC, INDEX_T, SIZE_T > >(bAutoDeleteObject, bInterlock, &m_data),
		m_nGrowth(nGrowth)
	{
		parent_t::SetNullPosition();
	}

	TVectorList(const TVectorList& B)
		: IList< T, TP, TPC, INDEX_T, SIZE_T, TVectorListIter< T, TP, TPC, INDEX_T, SIZE_T > >(true, false, &m_data),
		m_nGrowth(B.m_nGrowth)
	{
		parent_t::CopyFrom(B);
	}
	TVectorList& operator = (const TVectorList& B)	{ if (this != &B) parent_t::CopyFrom(B); return *this; }

	template < class TLIST >
	explicit TVectorList(const TLIST& B)
		: IList< T, TP, TPC, INDEX_T, SIZE_T, TVectorListIter< T, TP, TPC, INDEX_T, SIZE_T > >(true, false, &m_data),
		m_nGrowth(1000)
	{
		__if_exists(TLIST::GetGrowth) { m_nGrowth = B.GetGrowth(); }
		*this = B;
	}
	template < class TLIST >
	TVectorList& operator = (const TLIST& B)		{ if ((void*)this != (void*)&B) parent_t::CopyFrom(B); return *this; }

	virtual ~TVectorList(void) {
		DeleteAll();
	}

	//-----------------------
	// operators
	//
	template < class TLIST >
	TVectorList& operator + (const TLIST& B)		{ TVectorList C(*this); C.AddFrom(B); return *this; }
	template < class TLIST >
	TVectorList& operator += (const TLIST& B)		{ AddFrom(B); return *this; }

	//-----------------------
	// mode
	//
	SIZE_T GetGrowth() const						{ return m_nGrowth; }
	bool SetGrowth(SIZE_T nGrowth)					{ if (nGrowth < m_nGrowth) return false; m_nGrowth = nGrowth; }

	////-----------------------
	////
	//template < class TLIST >
	//TVectorList operator + (const TLIST& B) const	{ __Lock_List__(*this, false); TVectorList C(*this); C.AddFrom(B); return C; }
	//template < class TLIST >
	//TVectorList& operator += (const TLIST& B)		{ __Lock_List__(*this, true); AddFrom(B); return *this; }

	// iIndex :
	//    -2 : Inserts after current item
	//    -1 : Inserts as Last Item
	// other : inserts as 'iIndex'. if iIndex is larger than or equal to current item count, Inserts as Last Item
	// over nItem : Inserts as Last Item
	virtual INDEX_T InsertAt(TPC pData, INDEX_T iIndex = -2) {
		assert1(pData != NULL);

		__Lock_List__(*this);

		if (!m_data.N()) {
			m_data.Push(new BLOCK_T());
			BLOCK_T& block = m_data();
			block.reserve(m_nGrowth+1);
			iIndex = 0;
			parent_t::m_iCurrentBlock = 0;
			parent_t::m_iCurrentSub = 0;
			parent_t::m_iSubL = 0;
			parent_t::m_iSubR = 0;
		} else {
			if (iIndex == -2) {															// as is.
				iIndex = parent_t::m_iCurrent;
			} else if ( (iIndex == -1) || (iIndex >= (INDEX_T)parent_t::m_nItem) ) {				// at the end of list
				iIndex = parent_t::m_nItem;
				parent_t::m_iCurrent = parent_t::m_nItem;
				parent_t::m_iCurrentBlock = m_data.N()-1;
				parent_t::m_iCurrentSub = (INDEX_T)m_data[parent_t::m_iCurrentBlock].size()-1+1;
				parent_t::m_iSubL = parent_t::m_nItem - (INDEX_T)m_data[parent_t::m_iCurrentBlock].size();
				parent_t::m_iSubR = parent_t::m_nItem+1-1;
			} else if (iIndex == 0) {													// at the first of list
				parent_t::m_iCurrent = 0;
				parent_t::m_iCurrentBlock = 0;
				parent_t::m_iCurrentSub = 0;
				parent_t::m_iSubL = 0;
				parent_t::m_iSubR = (INDEX_T)m_data[parent_t::m_iCurrentBlock].size();
			} else if ( (parent_t::m_iSubL <= iIndex) && (iIndex < parent_t::m_iSubR) ) {
				parent_t::m_iCurrentSub = iIndex - parent_t::m_iSubL;
			} else {
				parent_t::GetNode(iIndex);
			}
		}

		BLOCK_T& block = m_data[parent_t::m_iCurrentBlock];
		block.insert(block.begin()+parent_t::m_iCurrentSub, (TP)pData);
		parent_t::m_ppCurrentData = &(block[parent_t::m_iCurrentSub]);

		parent_t::m_nItem++;

		if (block.size() <= (size_t)m_nGrowth) {
			parent_t::m_iSubR++;
		} else {
			if ( (parent_t::m_iCurrentBlock+1 >= m_data.N()) || (m_data[parent_t::m_iCurrentBlock+1].size() >= (size_t)m_nGrowth) ) {
				m_data.InsertAt(new BLOCK_T(), parent_t::m_iCurrentBlock+1);
				m_data[parent_t::m_iCurrentBlock+1].reserve(m_nGrowth+1);
			}
			BLOCK_T& block = m_data[parent_t::m_iCurrentBlock];	// insert 한 후 내용 변경됨.
			BLOCK_T& blockNew = m_data[parent_t::m_iCurrentBlock+1];

			TP pData = *(block.end()-1);
			blockNew.insert(blockNew.begin(), pData);
			block.erase(block.end()-1);

			if (parent_t::m_iSubR == parent_t::m_iCurrent) {	// 마지막 pData 가 다음 블럭으로 옮겨졌을 경우
				parent_t::m_iCurrentBlock++;
				parent_t::m_iCurrentSub = 0;
				parent_t::m_iSubL = parent_t::m_iSubR;
				parent_t::m_iSubR = parent_t::m_iSubL + (INDEX_T)m_data[parent_t::m_iCurrentBlock].size();
				parent_t::m_ppCurrentData = &m_data[parent_t::m_iCurrentBlock][parent_t::m_iCurrentSub];
			}
		}

		return parent_t::Index();
	}

public:
	//-----------------------
	// Delete
	//

	virtual TP RemoveAt(INDEX_T iIndex) {
		__Lock_List__(*this);

		if (!parent_t::m_nItem) return TP(NULL);

		if (iIndex == -2) {										// as is.
		} else if ( (iIndex == -1) || (iIndex >= parent_t::m_nItem) ) {	// insert at last

			BLOCK_T& block = *m_data.Last();
			TP pLast = *(block.end()-1);
			block.erase(block.end()-1);
			if (!block.size())
				m_data.DeleteLast();
			parent_t::GetPPItem(-1);

			return pLast;
			
		} else if (parent_t::m_nItem && (iIndex == 0)) {					// insert at first

			parent_t::GetPPItem(0);

		} else if (iIndex != parent_t::m_iCurrent) {
		} else {
			return TP(NULL);
		}

		TP pData = *parent_t::m_ppCurrentData;
		m_data[parent_t::m_iCurrentBlock].erase(m_data[parent_t::m_iCurrentBlock].begin()+parent_t::m_iCurrentSub);
		parent_t::m_iSubR--;
		parent_t::m_nItem--;

		if (m_data[parent_t::m_iCurrentBlock].size()) {
			if (parent_t::m_iCurrent >= parent_t::m_iSubR) {
				parent_t::m_iCurrentBlock++;
				if (parent_t::m_iCurrentBlock >= m_data.N()) {
					parent_t::Last();
					return pData;
				}
				parent_t::m_iSubL = parent_t::m_iSubR;
				parent_t::m_iSubR = parent_t::m_iSubL + (INDEX_T)m_data[parent_t::m_iCurrentBlock].size();
				parent_t::m_iCurrentSub = 0;
			}
		} else {
			assert1(parent_t::m_iSubL == parent_t::m_iSubR);
			m_data.Delete(parent_t::m_iCurrentBlock);
			if (!m_data.N()) {
				parent_t::First();
				return pData;
			}
			if (m_data.N() <= parent_t::m_iCurrentBlock) {
				parent_t::m_iCurrentBlock--;
				const std::vector<TP>& block = m_data[parent_t::m_iCurrentBlock];
				parent_t::m_iSubL = parent_t::m_iSubL - (INDEX_T)block.size();
				parent_t::m_iCurrentSub = (INDEX_T)block.size()-1;
			} else {
				const std::vector<TP>& block = m_data[parent_t::m_iCurrentBlock];
				parent_t::m_iSubR = parent_t::m_iSubL + (INDEX_T)block.size();
				parent_t::m_iCurrentSub = 0;
			}
		}
		parent_t::m_ppCurrentData = &m_data[parent_t::m_iCurrentBlock][parent_t::m_iCurrentSub];
		return pData;
	}

	virtual void RemoveAll(void)				{ __Lock_List__(*this); parent_t::m_nItem = 0; m_data.DeleteAll(); parent_t::First(); }

	virtual void DeleteAll(void) {
		if (!parent_t::m_bAutoDeleteObject) {
			RemoveAll();
			return;
		}

		__if_exists(TP::Release) {
			RemoveAll();
			return;
		}

		__Lock_List__(*this);

		for (INDEX_T iBlock = (INDEX_T)0; iBlock < m_data.N(); iBlock++) {
			std::vector<TP>& block = m_data[iBlock];
			for (INDEX_T iSub = 0; iSub < (INDEX_T)block.size(); iSub++) {
				parent_t::DeleteObjectOnly(block[iSub]);
			}
		}
		parent_t::m_nItem = 0;
		m_data.DeleteAll();
		parent_t::First();
	}

};



//-----------------------------------------------------------------------------
//

template < class T, typename INDEX_T, typename SIZE_T >
class TList : public TDoublyLinkedList< T, T*, const T*, INDEX_T, SIZE_T > {
	typedef TDoublyLinkedList< T, T*, const T*, INDEX_T, SIZE_T > parent_t;
public:
	explicit TList(bool bAutoDeleteObject = true, bool bInterlock = false)
		: TDoublyLinkedList< T, T*, const T*, INDEX_T, SIZE_T >(bAutoDeleteObject, bInterlock) { }
	TList(const TList& B)
		: TDoublyLinkedList< T, T*, const T*, INDEX_T, SIZE_T >((TDoublyLinkedList< T, T*, const T*, INDEX_T, SIZE_T >&)B) { }
	template < class TLIST >
	explicit TList(const TLIST& B)
		: TDoublyLinkedList< T, T*, const T*, INDEX_T, SIZE_T >(B) { }

	TList& operator = (const TList& B) { if (this != &B) parent_t::CopyFrom(B); return *this; }
	template < class TLIST >
	TList& operator = (const TLIST& B) { if ((void*)this != (void*)&B) parent_t::CopyFrom(B); return *this; }
};


template < class T, typename INDEX_T, typename SIZE_T >
class TRefList : public TDoublyLinkedList< T, TRefPointer<T>, TConstRefPointer<T>, INDEX_T, SIZE_T > {
	typedef TDoublyLinkedList< T, TRefPointer<T>, TConstRefPointer<T>, INDEX_T, SIZE_T > parent_t;
public:
	explicit TRefList(bool bInterlock = false)
		: TDoublyLinkedList< T, TRefPointer<T>, TConstRefPointer<T>, INDEX_T, SIZE_T >(false, bInterlock) { }
	TRefList(const TRefList& B)
		: TDoublyLinkedList< T, TRefPointer<T>, TConstRefPointer<T>, INDEX_T, SIZE_T >((TDoublyLinkedList< T, TRefPointer<T>, TConstRefPointer<T>, INDEX_T, SIZE_T >&)B) { }
	template < class TLIST >
	explicit TRefList(const TLIST& B)
		: TDoublyLinkedList< T, TRefPointer<T>, TConstRefPointer<T>, INDEX_T, SIZE_T >(B) { }

	TRefList& operator = (const TRefList& B) { if (this != &B) parent_t::CopyFrom(B); return *this; }
	template < class TLIST >
	TRefList& operator = (const TLIST& B) { if ((void*)this != (void*)&B) parent_t::CopyFrom(B); return *this; }
};


template < class T, typename INDEX_T, typename SIZE_T >
class TVList : public TVectorList< T, T*, const T*, INDEX_T, SIZE_T > {
	typedef TVectorList< T, T*, const T*, INDEX_T, SIZE_T > parent_t;
public:
	TVList(bool bAutoDeleteObject = true, bool bInterlock = false, SIZE_T nGrowth = 1000)
		: TVectorList< T, T*, const T*, INDEX_T, SIZE_T >(bAutoDeleteObject, bInterlock, nGrowth) { }
	TVList(const TVList& B)
		: TVectorList< T, T*, const T*, INDEX_T, SIZE_T >((TVectorList< T, T*, const T*, INDEX_T, SIZE_T >&)B) { }
	template < class TLIST >
	explicit TVList(const TLIST& B)
		: TVectorList< T, T*, const T*, INDEX_T, SIZE_T >(B) { }

	TVList& operator = (const TVList& B)			{ if (this != &B) parent_t::CopyFrom(B); return *this; }
	template < class TLIST >
	TVList& operator = (const TLIST& B)				{ if ((void*)this != (void*)&B) parent_t::CopyFrom(B); return *this; }
};

template < class T, typename INDEX_T, typename SIZE_T >
class TRefVList : public TVectorList< T, TRefPointer<T>, TConstRefPointer<T>, INDEX_T, SIZE_T > {
	typedef TVectorList< T, TRefPointer<T>, TConstRefPointer<T>, INDEX_T, SIZE_T > parent_t;
public:
	explicit TRefVList(bool bInterlock = false, SIZE_T nGrowth = 1000)
		: TVectorList< T, TRefPointer<T>, TConstRefPointer<T>, INDEX_T, SIZE_T >(false, bInterlock, nGrowth) { }
	TRefVList(const TRefVList& B)
		: TVectorList< T, TRefPointer<T>, TConstRefPointer<T>, INDEX_T, SIZE_T >((TVectorList< T, TRefPointer<T>, TConstRefPointer<T>, INDEX_T, SIZE_T >&)B) { }
	template < class TLIST >
	explicit TRefVList(const TLIST& B)
		: TVectorList< T, TRefPointer<T>, TConstRefPointer<T>, INDEX_T, SIZE_T >(B) { }

	TRefVList& operator = (const TRefVList& B)		{ if (this != &B) parent_t::CopyFrom(B); return *this; }
	template < class TLIST >
	TRefVList& operator = (const TLIST& B)			{ if ((void*)this != (void*)&B) parent_t::CopyFrom(B); return *this; }
};


template < class T, typename INDEX_T, typename SIZE_T >
class TListIter : public TDoublyLinkedListIter< T, T*, const T*, INDEX_T, SIZE_T > {
public:
	TListIter(const TDoublyLinkedListIter< T, T*, const T*, INDEX_T, SIZE_T >& B) : TDoublyLinkedListIter<T, T*, const T*, INDEX_T, SIZE_T>(B) { }
	TListIter(const TListIter& B) : TDoublyLinkedListIter<T, T*, const T*, INDEX_T, SIZE_T>(B) { }
};

template < class T, typename INDEX_T, typename SIZE_T >
class TRefListIter : public TDoublyLinkedListIter< T, TRefPointer<T>, TConstRefPointer<T>, INDEX_T, SIZE_T > {
public:
	TRefListIter(const TDoublyLinkedListIter< T, TRefPointer<T>, TConstRefPointer<T>, INDEX_T, SIZE_T >& B) : TDoublyLinkedListIter< T, TRefPointer<T>, TConstRefPointer<T>, INDEX_T, SIZE_T >(B) { }
	TRefListIter(const TRefListIter& B) : TDoublyLinkedListIter< T, TRefPointer<T>, TConstRefPointer<T>, INDEX_T, SIZE_T >(B) { }
};

template < class T, typename INDEX_T, typename SIZE_T >
class TVListIter : public TVectorListIter<T, T*, const T*, INDEX_T, SIZE_T> {
public:
	TVListIter(const TVectorListIter<T, T*, const T*, INDEX_T, SIZE_T>& B) : TVectorListIter<T, T*, const T*, INDEX_T, SIZE_T>(B) { }
	TVListIter(const TVListIter& B) : TVectorListIter<T, T*, const T*, INDEX_T, SIZE_T>(B) { }
};

template < class T, typename INDEX_T, typename SIZE_T >
class TRefVListIter : public TVectorListIter<T, TRefPointer<T>, TConstRefPointer<T>, INDEX_T, SIZE_T> {
public:
	TRefVListIter(const TVectorListIter<T, TRefPointer<T>, TConstRefPointer<T>, INDEX_T, SIZE_T>& B) : TVectorListIter<T, TRefPointer<T>, TConstRefPointer<T>, INDEX_T, SIZE_T>(B) { }
	TRefVListIter(const TRefVListIter& B) : TVectorListIter<T, TRefPointer<T>, TConstRefPointer<T>, INDEX_T, SIZE_T>(B) { }
};


//
// END of LIST
//=============================================================================


//=============================================================================
//-----------------------------------------------------------------------------
// TBuffer
//
template < class T, typename INDEX_T = intXX_t, typename SIZE_T = intXX_t > class TBuffer;

template < class T, typename INDEX_T, typename SIZE_T > class TBuffer {
protected:
	T* m_pData;
	SIZE_T m_nCount;
	INDEX_T m_iCurrent;
public:
	TBuffer(SIZE_T nCount = 0, const T* pData = NULL) : m_nCount(0), m_pData(NULL) {
		SetData(nCount, pData);
	}
	TBuffer(const TBuffer& B) : m_nCount(0), m_pData(NULL) {
		*this = B;
	}
	~TBuffer() {
		if (m_pData && m_nCount) delete [] m_pData;
	}
	TBuffer& operator = (const TBuffer& B) {
		if (this == &B) return *this;
		SetData(B.GetSize(), (const T*)B);
		return *this;
	}

	void Attach(T* pData, SIZE_T nCount) {
		if (m_pData && m_nCount) delete [] m_pData;
		m_pData = pData;
		m_nCount = nCount;
		m_iCurrent = 0;
	}
	T* Detach() {
		T* pData = m_pData;
		m_pData = NULL;
		m_nCount = 0;
		m_iCurrent = 0;
		return pData;
	}

	T& operator [] (INDEX_T iIndex) {
		__List_check_exception__( (iIndex >= 0) && (iIndex < m_nCount), "iIndex error" );
		return m_pData[iIndex];
	}
	operator T*() { return m_pData; }
	operator const T*() const { return m_pData; }
	T* GetBuffer() { return m_pData; }
	const T* GetBuffer() const { return m_pData; }

	template < typename TYPE >       TYPE* GetPointer(INDEX_T iIndex = 0)       { assert1(m_nCount*sizeof(T) >= (sizeof(TYPE)*(iIndex+1))); return (TYPE*)m_pData; }
	template < typename TYPE > const TYPE* GetPointer(INDEX_T iIndex = 0) const { assert1(m_nCount*sizeof(T) >= (sizeof(TYPE)*(iIndex+1))); return (TYPE*)m_pData; }

	bool SetData(SIZE_T nCount, const T* pData) {
		if (pData && (pData == m_pData))
			return false;
		SetSize(nCount);
		if (m_nCount && m_pData && pData)
			memmove(m_pData, pData, m_nCount*sizeof(T));
		return true;
	}

	// For Loop
	void First() { m_iCurrent = 0; }
	INDEX_T IsNotNull() { return m_iCurrent < m_nCount; }
	INDEX_T Next() { return m_iCurrent++; }
	T& Current() { __List_check_exception__(m_pData && (m_iCurrent < m_nCount), "current is NULL"); return m_pData[m_iCurrent]; }

	SIZE_T GetSize() const { return m_nCount; }
	bool SetSize(SIZE_T nCount) {
		if ( (m_nCount > 0) && m_pData )
			delete [] m_pData;
		m_nCount = nCount;
		if (m_nCount > 0)
			m_pData = new T[m_nCount];
		else
			m_pData = NULL;
		if (m_pData == NULL) m_nCount = 0;
		return m_nCount > 0;
	}
	bool Resize(SIZE_T nCount) {
		if ( (nCount <= 0) || (m_nCount <= 0) )
			return SetSize(nCount);
		T* pNewData = new T[nCount];
		memset(pNewData, 0, nCount*sizeof(T));
		memcpy(pNewData, m_pData, _min(nCount*sizeof(T), m_nCount*sizeof(T)));
		delete [] m_pData;
		m_pData = pNewData;
		m_nCount = nCount;
		if (m_iCurrent > m_nCount)
			m_iCurrent = m_nCount;
		return m_nCount > 0;
	}

	bool SetZero(INDEX_T iStart = 0, INDEX_T nCount = -1) {
		if (iStart < 0)
			iStart = 0;
		if (iStart >= m_nCount)
			return false;
		if (nCount < 0)
			nCount = m_nCount-iStart;
		if ( iStart + nCount > m_nCount)
			return false;
		if ( (m_nCount > 0) && m_pData )
			memset(m_pData+iStart, 0, sizeof(T)*nCount);
		return true;
	}
};

//
// END of TBuffer
//=============================================================================



#pragma pack(pop)



#endif // __T_LIST_HPP__INCLUDED__PWH__
