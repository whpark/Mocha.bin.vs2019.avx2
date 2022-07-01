// TRefPointer.h: interface for the TRefPointer class.
// Last Edited		2012. 03. 30. : copied from opencv template Ptr<>
//
//                  2013. 06. 16. : ARRAY 추가 -> delete [] 추가. (TRefPointer_STATUS 추가)
//                                  Detach 시 발생할 수 있는 오류 있음.
//                                  여러 개체가 있는 상태에서 Detach() 호출 할 경우, Memory Leak 발생할 수 있는 부분 수정.
//                                  Single Thread 에서는 큰 문제가 없어 보이지만, Multi-Thread 에서 Detach 사용은 문제발생 소지 남음.
//                                  (속도 때문에 Critical Section 을 안 걸었음.)
//
//
// PWH
//////////////////////////////////////////////////////////////////////

#if !defined(__T_RPTR_HPP__INCLUDED__PWH__)
#define __T_RPTR_HPP__INCLUDED__PWH__

#pragma once

//#include <Windows.h>
#include <malloc.h>
#include <intrin.h>
#include "default.defines.h"

#pragma intrinsic(_InterlockedExchangeAdd, _interlockedbittestandset)

#ifdef _MFC_VER
	#define assert1 ASSERT
#else
	#include <assert.h>
	#define assert1 assert
	#ifndef ASSERT
		#define ASSERT assert
	#endif
#endif

template < class T, typename INDEX_T = intXX_t, typename SIZE_T = intXX_t > class TRefPointer;
template < class T, typename INDEX_T = intXX_t, typename SIZE_T = intXX_t > class TConstRefPointer;

//----------------------------------------------------------------------------
//
template < class T, class TP, class TPC, typename INDEX_T, typename SIZE_T, typename TLISTITER > class IList;
template < class T, class TP, class TPC, typename INDEX_T, typename SIZE_T > class TDoublyLinkedListIter;
template < class T, class TP, class TPC, typename INDEX_T, typename SIZE_T > class TDoublyLinkedList;
template < class T, class TP, class TPC, typename INDEX_T, typename SIZE_T > class TVectorListIter;
template < class T, class TP, class TPC, typename INDEX_T, typename SIZE_T > class TVectorList;

//=============================================================================
//

#ifdef _DEBUG
	#define  MALLOC_ALIGN    16
	template<class T> static inline T* AlignPtr(T* ptr, size_t n = sizeof(T)) {
		return (T*)(((size_t)ptr + n-1) & (size_t)(-(intptr_t)n));
	}
	inline void* FastMalloc( size_t size ) {
		unsigned char* udata = (unsigned char*)malloc(size + sizeof(void*) + MALLOC_ALIGN);
		if (!udata)
			return NULL;
		unsigned char** adata = AlignPtr((unsigned char**)udata + 1, MALLOC_ALIGN);
		adata[-1] = udata;
		return adata;
	}
    
	inline void FastFree(void* ptr) {
		if(ptr) {
			unsigned char* udata = ((unsigned char**)ptr)[-1];
			assert1(udata < (unsigned char*)ptr && ((unsigned char*)ptr - udata) <= (ptrdiff_t)(sizeof(void*)+MALLOC_ALIGN)); 
			free(udata);
		}
	}
	#undef MALLOC_ALIGN

#else	// _DEBUG

	inline void* FastMalloc(size_t size) { return malloc(size); }
	inline void FastFree(void* ptr) { if (ptr) free(ptr); }

#endif	// _DEBUG

//=============================================================================
//
struct TRefPointer_STATUS {
	volatile long nRefCounter; //< the associated reference counter
	volatile long fStatus;
};

template < class T, typename INDEX_T, typename SIZE_T >
class TRefPointer {
friend class TConstRefPointer<T>;
	typedef enum {
		S_ARRAY = 0, S_DELETED, S_DONTDELETE, 
	} eSTATUS;

protected:
    T* m_pObject;
	TRefPointer_STATUS* m_pStatus;

public:
    TRefPointer() : m_pObject(NULL), m_pStatus(NULL) {}
    //! take ownership of the pointer. The associated reference counter is allocated and set to 1
    TRefPointer(T* pObject, bool bArray = false, bool bAutoDelete = true) : m_pObject(pObject) {
		if (m_pObject) {
			m_pStatus = (TRefPointer_STATUS*)FastMalloc(sizeof(*m_pStatus));
			m_pStatus->nRefCounter = 1;
			m_pStatus->fStatus = bArray ? BIT(TRefPointer<T>::S_ARRAY) : 0;
			if (!bAutoDelete)
				_interlockedbittestandset(&m_pStatus->fStatus, BIT(TRefPointer<T>::S_DONTDELETE));
		}
		else
			m_pStatus = NULL;
	}
	void Assign(SIZE_T n = 1) {
		Release();
		if (n == 1)
			Attach(new T, false);
		else if (n > 1)
			Attach(new T[n], true);
	}
	void Set(T* pObject, bool bArray = false, bool bAutoDelete = true) {
		Release();
		m_pObject = pObject;
		if (m_pObject) {
			m_pStatus = (TRefPointer_STATUS*)FastMalloc(sizeof(*m_pStatus));
			m_pStatus->nRefCounter = 1;
			m_pStatus->fStatus = bArray ? BIT(TRefPointer<T>::S_ARRAY) : 0;
			if (!bAutoDelete)
				_interlockedbittestandset(&m_pStatus->fStatus, BIT(TRefPointer<T>::S_DONTDELETE));
		}
		else
			m_pStatus = NULL;
	}

    //! calls release()
    ~TRefPointer() { Release(); }
    //! copy constructor. Copies the members and calls addref()
    TRefPointer(const TRefPointer& ptr) {
		m_pObject = ptr.m_pObject;
		m_pStatus = ptr.m_pStatus;
		if (m_pStatus)
			_InterlockedExchangeAdd(&m_pStatus->nRefCounter, 1);
	}
    //! copy operator. Calls ptr.addref() and release() before copying the members
    TRefPointer& operator = (const TRefPointer& ptr) {
		Release();
		m_pObject = ptr.m_pObject;
		m_pStatus = ptr.m_pStatus;
		if (m_pStatus)
			_InterlockedExchangeAdd(&m_pStatus->nRefCounter, 1);
		return *this;
	}

	//bool operator == (const TRefPointer<T>& B) const { return m_pObject == B->m_pObject; }
	//bool operator != (const TRefPointer<T>& B) const { return m_pObject != B->m_pObject; }

	//! decrements the reference counter. If it reaches 0, DeleteObject() is called
    T* Release() {
		T* pObject = m_pObject;
		if ( m_pStatus && _InterlockedExchangeAdd(&m_pStatus->nRefCounter, -1) == 1 ) {
			DeleteObject();
			FastFree(m_pStatus);
			pObject = NULL;
		}
		m_pStatus = NULL;
		m_pObject = NULL;
		return pObject;
	}

	T* Attach(T* pObject, bool bArray = false) {
		T* pObjectOld = Release();
		m_pObject = pObject;
		if (m_pObject) {
			m_pStatus = (TRefPointer_STATUS*)FastMalloc(sizeof(*m_pStatus));
			m_pStatus->nRefCounter = 1;
			m_pStatus->fStatus = bArray ? BIT(TRefPointer<T>::S_ARRAY) : 0;
		}
		else
			m_pStatus = NULL;
		return pObjectOld;
	}
	T* Detach() {
		T* pObject = m_pObject;
		if (m_pStatus) {
			_interlockedbittestandset(&m_pStatus->fStatus, TRefPointer<T>::S_DONTDELETE);
			if (_InterlockedExchangeAdd(&m_pStatus->nRefCounter, -1) == 1 ) {
				FastFree(m_pStatus);
			}
			m_pStatus = NULL;
			m_pObject = NULL;
		}
		m_pObject = NULL;
		return pObject;
	}
	void DontDelete() {
		if (m_pStatus) {
			_interlockedbittestandset(&m_pStatus->fStatus, TRefPointer<T>::S_DONTDELETE);
		}
	}

    //! returns true iff m_pObject==NULL
    bool IsEmpty() const { return (m_pObject == NULL) || (!m_pStatus || (m_pStatus->fStatus & BIT(TRefPointer<T>::S_DELETED)) ) ;}
	//! returns if unique
	bool IsUnique() const { return m_pStatus ? (m_pStatus->nRefCounter == 1) : false; }

    //! helper operators making "TRefPointer<T> ptr" use very similar to "T* ptr".
	T* GetPointer()					{ if (!m_pStatus || (m_pStatus->fStatus & BIT(TRefPointer<T>::S_DELETED)) || (m_pStatus->nRefCounter == 0) ) return NULL; return m_pObject; }
	const T* GetPointer() const		{ if (!m_pStatus || (m_pStatus->fStatus & BIT(TRefPointer<T>::S_DELETED)) || (m_pStatus->nRefCounter == 0) ) return NULL; return m_pObject; }
	T* get()						{ if (!m_pStatus || (m_pStatus->fStatus & BIT(TRefPointer<T>::S_DELETED)) || (m_pStatus->nRefCounter == 0) ) return NULL; return m_pObject; }
	const T* get() const			{ if (!m_pStatus || (m_pStatus->fStatus & BIT(TRefPointer<T>::S_DELETED)) || (m_pStatus->nRefCounter == 0) ) return NULL; return m_pObject; }
	T* operator -> ()				{ if (!m_pStatus || (m_pStatus->fStatus & BIT(TRefPointer<T>::S_DELETED)) || (m_pStatus->nRefCounter == 0) ) return NULL; return m_pObject; }
    const T* operator -> () const	{ if (!m_pStatus || (m_pStatus->fStatus & BIT(TRefPointer<T>::S_DELETED)) || (m_pStatus->nRefCounter == 0) ) return NULL; return m_pObject; }
    operator T* ()					{ if (!m_pStatus || (m_pStatus->fStatus & BIT(TRefPointer<T>::S_DELETED)) || (m_pStatus->nRefCounter == 0) ) return NULL; return m_pObject; }
    operator const T*() const		{ if (!m_pStatus || (m_pStatus->fStatus & BIT(TRefPointer<T>::S_DELETED)) || (m_pStatus->nRefCounter == 0) ) return NULL; return m_pObject; }

protected:
    //! increments the reference counter
    void AddReferenceCounter() {
		if (m_pStatus) _InterlockedExchangeAdd(&m_pStatus->nRefCounter, 1);
	}
	//! deletes the object. Override if needed
	bool DeleteObject() {
		if (m_pObject && m_pStatus && !(m_pStatus->fStatus & BIT(TRefPointer<T>::S_DONTDELETE)) && !_interlockedbittestandset(&m_pStatus->fStatus, TRefPointer<T>::S_DELETED)) {
			T* pObject = m_pObject;
			m_pObject = NULL;
			if ( m_pStatus->fStatus & BIT(TRefPointer<T>::S_ARRAY) ) delete [] pObject;
			else delete pObject;
			return true;
		}
		return false;
	}

};

//-----------------------------------------------------------------------------
//
template < class T, typename INDEX_T, typename SIZE_T >
class TConstRefPointer {
protected:
    const T* m_pObject; //< the object pointer.
    TRefPointer_STATUS* m_pStatus;

public:
    TConstRefPointer() : m_pObject(NULL), m_pStatus(NULL) {}
    //! take ownership of the pointer. The associated reference counter is allocated and set to 1
    TConstRefPointer(const T* pObject, bool bArray = false, bool bAutoDelete = true) : m_pObject(pObject) {
		if (m_pObject) {
			m_pStatus = (TRefPointer_STATUS*)FastMalloc(sizeof(*m_pStatus));
			m_pStatus->nRefCounter = 1;
			m_pStatus->fStatus = bArray ? BIT(TRefPointer<T>::S_ARRAY) : 0;
			if (!bAutoDelete)
				_interlockedbittestandset(&m_pStatus->fStatus, BIT(TRefPointer<T>::S_DONTDELETE));
		}
		else
			m_pStatus = NULL;
	}
	void Assign(SIZE_T n) {
		Release();
		if (n == 1)
			Attach(new T, false);
		else if (n > 1)
			Attach(new T[n], true);
	}
	void Set(const T* pObject, bool bArray = false, bool bAutoDelete = true) {
		Release();
		m_pObject = pObject;
		if (m_pObject) {
			m_pStatus = (TRefPointer_STATUS*)FastMalloc(sizeof(*m_pStatus));
			m_pStatus->nRefCounter = 1;
			m_pStatus->fStatus = bArray ? BIT(TRefPointer<T>::S_ARRAY) : 0;
			if (!bAutoDelete)
				_interlockedbittestandset(&m_pStatus->fStatus, BIT(TRefPointer<T>::S_DONTDELETE));
		}
		else
			m_pStatus = NULL;
	}

    //! calls release()
    ~TConstRefPointer() { Release(); }
    TConstRefPointer(const TConstRefPointer& ptr) {
		TRefPointer_STATUS* pStatus = ptr.m_pStatus;
		if (pStatus)
			_InterlockedExchangeAdd(&pStatus->nRefCounter, 1);
		m_pObject = ptr.m_pObject;
		m_pStatus = ptr.m_pStatus;
	}

    TConstRefPointer& operator = (const TConstRefPointer& ptr) {
		TRefPointer_STATUS* pStatus = ptr.m_pStatus;
		if (pStatus)
			_InterlockedExchangeAdd(&pStatus->nRefCounter, 1);
		Release();
		m_pObject = ptr.m_pObject;
		m_pStatus = pStatus;
		return *this;
	}

	TConstRefPointer(const TRefPointer<T>& ptr) {
		m_pObject = ptr.m_pObject;
		m_pStatus = ptr.m_pStatus;
		if (m_pStatus)
			_InterlockedExchangeAdd(&m_pStatus->nRefCounter, 1);
	}
    TConstRefPointer& operator = (const TRefPointer<T>& ptr) {
		Release();
		m_pObject = ptr.m_pObject;
		m_pStatus = ptr.m_pStatus;
		if (m_pStatus)
			_InterlockedExchangeAdd(&m_pStatus->nRefCounter, 1);
		return *this;
	}

    //! decrements the reference counter. If it reaches 0, DeleteObject() is called
    const T* Release() {
		const T* pObject = m_pObject;
		if ( m_pStatus && _InterlockedExchangeAdd(&m_pStatus->nRefCounter, -1) == 1 ) {
			DeleteObject();
			FastFree(m_pStatus);
			pObject = NULL;
		}
		m_pStatus = NULL;
		m_pObject = NULL;
		return pObject;
	}

	T* Attach(T* pObject, bool bArray = false) {
		const T* pObjectOld = Release();
		m_pObject = pObject;
		if (m_pObject) {
			m_pStatus = (TRefPointer_STATUS*)FastMalloc(sizeof(*m_pStatus));
			m_pStatus->nRefCounter = 1;
			m_pStatus->fStatus = bArray ? BIT(TRefPointer<T>::S_ARRAY) : 0;
		}
		else
			m_pStatus = NULL;
		return pObjectOld;
	}
	T* Detach() {
		T* pObject = m_pObject;
		if (m_pStatus) {
			_interlockedbittestandset(&m_pStatus->fStatus, TRefPointer<T>::S_DONTDELETE);
			if (_InterlockedExchangeAdd(&m_pStatus->nRefCounter, -1) == 1 ) {
				FastFree(m_pStatus);
			}
			m_pStatus = NULL;
			m_pObject = NULL;
		}
		m_pObject = NULL;
		return pObject;
	}
	void DontDelete() {
		if (m_pStatus) {
			_interlockedbittestandset(&m_pStatus->fStatus, TRefPointer<T>::S_DONTDELETE);
		}
	}

    //! returns true iff m_pObject==NULL
    bool IsEmpty() const { return (m_pObject == NULL) || (!m_pStatus || (m_pStatus->fStatus & BIT(TRefPointer<T>::S_DELETED)) ) ;}
	//! returns if unique
	bool IsUnique() const { return m_pStatus ? (m_pStatus->nRefCounter == 1) : false; }

    //! helper operators making "TConstRefPointer<T> ptr" use very similar to "T* ptr".
	const T* GetPointer() const		{ if (!m_pStatus || (m_pStatus->fStatus & BIT(TRefPointer<T>::S_DELETED)) || (m_pStatus->nRefCounter == 0) ) return NULL; return m_pObject; }
    const T* operator -> () const	{ if (!m_pStatus || (m_pStatus->fStatus & BIT(TRefPointer<T>::S_DELETED)) || (m_pStatus->nRefCounter == 0) ) return NULL; return m_pObject; }
    operator const T*() const		{ if (!m_pStatus || (m_pStatus->fStatus & BIT(TRefPointer<T>::S_DELETED)) || (m_pStatus->nRefCounter == 0) ) return NULL; return m_pObject; }

protected:
	friend class IList<T, TRefPointer<T>, TConstRefPointer<T>, INDEX_T, SIZE_T, TDoublyLinkedListIter<T, TRefPointer<T>, TConstRefPointer<T>, INDEX_T, SIZE_T > >;
	friend class IList<T, TRefPointer<T>, TConstRefPointer<T>, INDEX_T, SIZE_T, TVectorListIter<T, TRefPointer<T>, TConstRefPointer<T>, INDEX_T, SIZE_T > >;
	friend class TDoublyLinkedList<T, TRefPointer<T>, TConstRefPointer<T>, INDEX_T, SIZE_T>;
	friend class TVectorList<T, TRefPointer<T>, TConstRefPointer<T>, INDEX_T, SIZE_T>;
	operator TRefPointer<T> () { return *(TRefPointer<T>*)this; }

protected:
    //! increments the reference counter
    void AddReferenceCounter() {
		if (m_pStatus) _InterlockedExchangeAdd(&m_pStatus->nRefCounter, 1);
	}

	//! deletes the object. Override if needed
    bool DeleteObject() {
		if (m_pObject
			&& m_pStatus
			&& !(m_pStatus->fStatus & BIT(TRefPointer<T>::S_DONTDELETE))
			&& !_interlockedbittestandset(&m_pStatus->fStatus, TRefPointer<T>::S_DELETED)) {
			const T* pObject = m_pObject;
			m_pObject = NULL;
			if ( m_pStatus->fStatus & (TRefPointer<T>::S_ARRAY) ) delete [] pObject;
			else delete pObject;

			return true;
		}
		return false;
	}
};

#endif // !defined(__T_RPTR_HPP__INCLUDED__PWH__)
