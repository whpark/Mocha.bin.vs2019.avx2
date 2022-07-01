// TCircularBuffer.h: interface for the TCircularBuffer class.
// Last Edited
//					2012. 03. 02.
//					2013. 06. 09. : x64
//
// Made by PWH
//////////////////////////////////////////////////////////////////////

#pragma once

#include "default.defines.h"
#include "TList.hpp"

#include "misc/xUtil.h"

//

//#pragma pack(push, 8)

//=============================================================================
// TCircularBuffer
//
//    SIZE : Byte 단위.
//    COUNT : T_DATA_TYPE의 개수.
//    T_DATA_TYPE이 short 일 경우, 10 count --> sizeof(short)*10 == 20 bytes. (size)
//    Max Size : 32bit(x86) : 2 GBytes. -> 1 G counts.
//             : 64bit(x64) : 2^63 Bytes. -> 2^62 counts.
typedef enum { TCB_USE_MEMCPY, TCB_USE_ASSIGN_OPERATOR } eTCB_ASSIGN;
template < typename T_DATA_TYPE, eTCB_ASSIGN eOperator = TCB_USE_MEMCPY, BOOL bAutoIncreaseHeader = FALSE, typename INDEX_T = uintXX_t, typename SIZE_T = sizeXX_t >
class TCircularBuffer {
protected:
	mutable CCriticalSection m_csData;					// Critical Section
	T_DATA_TYPE* m_pData;								// Circular Buffer
	volatile SIZE_T m_nBufferCount;			// NOT in BYTE.
	volatile INDEX_T m_iHead, m_iTail;			// Do as Circular Buffer

	mutable CEvent m_evtDataArrived;

public:
	TCircularBuffer(SIZE_T nCount = 0)	// Not in BYTE. size in bytes = nCount * sizeof(T_DATA_TYPE)
		: m_pData(NULL), m_nBufferCount(0), m_iHead(0), m_iTail(0),
		m_evtDataArrived(FALSE, TRUE, NULL, NULL)
	{	
		MakeBuffer(nCount);
	}
	virtual ~TCircularBuffer() {
		MakeBuffer(0);	// Close
	}
	TCircularBuffer(const TCircularBuffer<T_DATA_TYPE> & B) : m_pData(0), m_nBufferCount(0), m_iHead(0), m_iTail(0) { *this = B; }
	TCircularBuffer<T_DATA_TYPE>& operator = (const TCircularBuffer<T_DATA_TYPE> & B) {
		MakeBuffer(0);
		CS cs(&B.m_csData);
		CS cs(&m_csData);
		if (B.m_nBufferCount) {
			MakeBuffer(B.m_nBufferCount);
			if (eOperator == TCB_USE_ASSIGN_OPERATOR) {
				for (INDEX_T i = 0; i < B.m_nBufferCount; i++)
					m_pData[i] = B.m_pData[i];
			} else {
				memcpy(m_pData, B.m_pData, B.m_nBufferCount*sizeof(T_DATA_TYPE));
			}
			m_iHead = B.m_iHead;
			m_iTail = B.m_iTail;
		}
		return *this;
	}

public:
	CCriticalSection& LockObject() const { return m_csData; }
	BOOL Lock(DWORD dwTimeout = INFINITE) const {
		return m_csData.Lock(dwTimeout);
	}
	BOOL Unlock() const {
		return m_csData.Unlock();
	}
	BOOL Reset() {
		CS cs(&m_csData);

		m_evtDataArrived.ResetEvent();

		m_iHead = m_iTail = 0;

		return TRUE;
	}
	// ----------
	// Make Buffer
	BOOL MakeBuffer(SIZE_T nCount) {
		// ENTER CRITICAL SECTION.
		CS cs(&m_csData);

		m_evtDataArrived.ResetEvent();
		if ( nCount && (((INDEX_T)(-1) >> 1) / nCount < sizeof(T_DATA_TYPE)) )			// Count가 너무 큼. 32/64bit (각각 31bit, 61bit) 주소 공간을 넘어감.
			return FALSE;

		if (nCount == m_nBufferCount) {
			Reset();
		} else {
			// Clean up.
			if (m_pData) {
				delete [] m_pData;
			}
			m_pData = NULL;
			m_nBufferCount = m_iHead = m_iTail = 0;

			// Check Size.
			m_nBufferCount = nCount;
			if (!m_nBufferCount)
				return FALSE;

			// Memory Allocation
			m_pData = new T_DATA_TYPE[m_nBufferCount];
			if (!m_pData) {										// if FAILED
				m_nBufferCount = 0;
				return FALSE;
			}
		}

		// 초기화
		if (eOperator == TCB_USE_ASSIGN_OPERATOR)
			;
		else
			memset(m_pData, 0, sizeof(T_DATA_TYPE)*m_nBufferCount);

		return TRUE;
	}

public:
	// ----------
	// 단순히 Buffer의 크기만 리턴.
	SIZE_T GetBufferCount() const {
		CS cs(&m_csData);
		return m_nBufferCount;
	}

	// ----------
	// Queue에 들어온 유효한 데이터의 갯수. iHead 부터 m_iTail.
	SIZE_T GetDataCount(INDEX_T iHead) const {
		CS cs(&m_csData);
		iHead = GetValidHead(iHead);
		return (iHead <= m_iTail) ? (m_iTail - iHead) : (m_nBufferCount - (iHead - m_iTail));
	}

	// ----------
	// Queue에 들어온 유효한 데이터의 갯수.
	SIZE_T GetDataCount() const {
		return GetDataCount(-1);
	}
	SIZE_T N() const {
		return GetDataCount(-1);
	}

	SIZE_T GetValidHead(INDEX_T iHead) const {
		CS cs(&m_csData);
		if (iHead == (INDEX_T)-1)
			iHead = m_iHead;
		if (m_iHead <= m_iTail) {
			if (iHead < m_iHead)
				iHead = m_iHead;
		} else {
			if ( (iHead < m_iHead) && (iHead > m_iTail) ) {
				iHead = m_iHead;
			}
		}
		return iHead;
	}
	INDEX_T GetLastPoint() const {
		CS cs(&m_csData);
		if (GetDataCount() == 0)
			return (INDEX_T)-1;
		if (m_iTail == 0)
			return m_nBufferCount-1;
		return m_iTail - 1;
	}

	// ----------
	// Data가 들어올 경우 return 함.
	BOOL Wait(DWORD dwTimeout = INFINITE) const {
		return m_evtDataArrived.Lock(dwTimeout);
	}
	HANDLE GetEventHandle() const { return m_evtDataArrived.m_hObject; }

public:
	BOOL IsIndexInbound(INDEX_T iIndex) const {
		CS cs(&m_csData);
		if (iIndex >= m_nBufferCount)
			return FALSE;
		if (m_iHead <= m_iTail) {
			return (iIndex >= m_iHead) && (iIndex < m_iTail);
		}
		return ( (iIndex >= m_iHead) && (iIndex < m_nBufferCount) )
				|| (iIndex < m_iTail);
	}

public:
	// ----------
	// Input to Queue.
	BOOL QueueData(const T_DATA_TYPE* pData, SIZE_T nCount = 1) {
		if (!m_nBufferCount || !m_pData)
			return FALSE;

		CS cs(&m_csData);

		if (!bAutoIncreaseHeader) {
			if (GetBufferCount() - GetDataCount() <= nCount)
				return FALSE;
		}

		// To do : 속도 개선.
		for (INDEX_T i = 0; i < nCount; i++) {
			if (eOperator == TCB_USE_ASSIGN_OPERATOR)
				m_pData[m_iTail] = pData[i];
			else
				memcpy(m_pData+m_iTail, pData+i, sizeof(T_DATA_TYPE)*1);

			m_iTail = (m_iTail+1) % m_nBufferCount;		// 1증가
			if (m_iTail == m_iHead)
				m_iHead = (m_iHead+1) % m_nBufferCount;	// 1증가
		}
		// Set Event
		m_evtDataArrived.SetEvent();
		return TRUE;
	}

	// ----------
	// ReadData : (Get Data from Queue)
	// return value : 읽은 크기를 리턴.(byte 단위 아니라 개수 단위임)
	// nCountToRead가 -1 일 경우, 마지막까지 읽음. 단 pBuffer가 충분히 커야 함.
	SIZE_T ReadData(T_DATA_TYPE* pBuffer, SIZE_T nCountToRead, INDEX_T* piHead = NULL) {
		CS cs(&m_csData);
		if (!m_nBufferCount || !m_pData || !pBuffer || !nCountToRead)
			return FALSE;

		INDEX_T iHead = m_iHead;
		SIZE_T nRead = PeakData(pBuffer, nCountToRead, &iHead);
		m_iHead = iHead;

		// Reset Event
		if (m_iHead == m_iTail)
			m_evtDataArrived.ResetEvent();

		if (piHead)
			*piHead = iHead;

		return nRead;
	}

	// ----------
	// PeakData : (Get Data from Queue, without changing header pointer)
	// return value : 읽은 크기를 리턴.(byte 단위 아니라 개수 단위임)
	// nCountToRead가 -1 일 경우, 마지막까지 읽음. 단 pBuffer가 충분히 커야 함.
	SIZE_T PeakData(T_DATA_TYPE* pBuffer, SIZE_T nCountToRead, intXX_t& iHead) const {
		if (!m_nBufferCount || !m_pData || !pBuffer || !nCountToRead)
			return FALSE;

		// Copy Data
		CS cs(&m_csData);
		iHead = GetValidHead(iHead);

		SIZE_T nRead = 0;
		for (nRead = 0; (nRead < nCountToRead) && (iHead != m_iTail); nRead++, iHead = (iHead+1) % m_nBufferCount) {
			if (eOperator == TCB_USE_ASSIGN_OPERATOR)
				pBuffer[nRead] = m_pData[iHead];
			else
				memcpy(pBuffer+nRead, m_pData+iHead, sizeof(T_DATA_TYPE));
		}

		return nRead;
	}

	SIZE_T PeakData(TBuffer<T_DATA_TYPE>& buffer, SIZE_T nCountToRead, INDEX_T& iHead) const {
		CS cs(&m_csData);
		if (nCountToRead == (SIZE_T)-1) {
			nCountToRead = GetDataCount(iHead);
			if (!nCountToRead)
				return 0;
		}
		buffer.SetSize(nCountToRead);
		return PeakData((T_DATA_TYPE*)buffer, nCountToRead, iHead);
	}
	SIZE_T PeakData(TBuffer<T_DATA_TYPE>& buffer, SIZE_T nCountToRead) const {
		INDEX_T iHead = -1;
		return PeakData(buffer, nCountToRead, iHead);
	}
	SIZE_T PeakData(T_DATA_TYPE* pBuffer, SIZE_T nCountToRead) const {
		INDEX_T iHead = -1;
		return PeakData(pBuffer, nCountToRead, iHead);
	}
};



//=============================================================================
// Hash Iterator
template < typename T_DATA_TYPE, typename INDEX_T = uintXX_t, typename SIZE_T = sizeXX_t >
class IHashIterator {
public:
	virtual SIZE_T GetHashIndex(const T_DATA_TYPE* pCookie) = NULL;
	virtual T_DATA_TYPE GetFakeData(INDEX_T iIndex, const T_DATA_TYPE* pCookie) = NULL;
	virtual T_DATA_TYPE* GetFakeDataPtr(INDEX_T iIndex, const T_DATA_TYPE* pCookie) = NULL;
};
//-----------------------------------------------------------------------------
template < typename T_DATA_TYPE, eTCB_ASSIGN eOperator = TCB_USE_MEMCPY, BOOL bAutoIncreaseHeader = FALSE, BOOL bMakeContiguous = TRUE, typename INDEX_T = uintXX_t, typename SIZE_T = sizeXX_t >
class TIOHashQueue : public TCircularBuffer<T_DATA_TYPE, eOperator, bAutoIncreaseHeader > {
	typedef TCircularBuffer<T_DATA_TYPE, eOperator, bAutoIncreaseHeader > parent_t;
protected:
	IHashIterator<T_DATA_TYPE, INDEX_T, SIZE_T>* m_pHashIterator;
	SIZE_T m_nMaxInboundLength;
public:
	TIOHashQueue(IHashIterator<T_DATA_TYPE, INDEX_T, SIZE_T>* pHashIterator = NULL, SIZE_T nCount = 0) : m_pHashIterator(pHashIterator), TCircularBuffer(nCount) {
		m_nMaxInboundLength = 0;
	}
	virtual ~TIOHashQueue() {
		m_pHashIterator = NULL;
	}
	TIOHashQueue(const TIOHashQueue & B) : TCircularBuffer(B) {
		m_pHashIterator = B.m_pHashIterator;
	}
	TIOHashQueue& operator = (const TIOHashQueue & B) {
		m_pHashIterator = B.m_pHashIterator;
		return *this = (TCircularBuffer)B;
	}
	void SetHashIterator(IHashIterator<T_DATA_TYPE, INDEX_T, SIZE_T>* pHashIterator, SIZE_T nMaxInboundLength) {
		m_pHashIterator = pHashIterator;
		m_nMaxInboundLength = nMaxInboundLength;
	}

public:
	// ----------
	// Input to Queue.
	BOOL QueueData(const T_DATA_TYPE* pData, SIZE_T nCount) {
		if (!m_pHashIterator)
			return FALSE;
		if (nCount <= 0)
			return FALSE;
		CS cs(&parent_t::m_csData);

		SIZE_T nQueued = 0;

		for (INDEX_T i = 0; i < nCount; i++) {
			INDEX_T iIndex = m_pHashIterator->GetHashIndex(pData+i) % parent_t::m_nBufferCount;
			INDEX_T nItem = 1;

			if (IsIndexNearTailInbound(iIndex)) {
				if (eOperator == TCB_USE_ASSIGN_OPERATOR)
					parent_t::m_pData[iIndex] = pData[i];
				else
					memcpy(parent_t::m_pData+iIndex, pData+i, sizeof(T_DATA_TYPE)*1);
				nQueued++;
			} else {
				BOOL bProceed = TRUE;
				do {
					if (iIndex == parent_t::m_iTail)
						bProceed = FALSE;

					if (!bAutoIncreaseHeader) {
						if (parent_t::GetBufferCount() - parent_t::GetDataCount() <= 1)
							return FALSE;
					}

					if (bMakeContiguous) {
						if (iIndex == parent_t::m_iTail) {
							if (eOperator == TCB_USE_ASSIGN_OPERATOR)
								parent_t::m_pData[parent_t::m_iTail] = pData[i];
							else
								memcpy(parent_t::m_pData+parent_t::m_iTail, pData+i, sizeof(T_DATA_TYPE)*1);
						} else {
							if (eOperator == TCB_USE_ASSIGN_OPERATOR)
								parent_t::m_pData[parent_t::m_iTail] = m_pHashIterator->GetFakeData(parent_t::m_iTail, pData+i);
							else
								memcpy(parent_t::m_pData+parent_t::m_iTail, m_pHashIterator->GetFakeDataPtr(parent_t::m_iTail, pData+i), sizeof(T_DATA_TYPE)*1);
						}
					} else {
						if (eOperator == TCB_USE_ASSIGN_OPERATOR)
							parent_t::m_pData[parent_t::m_iTail] = T_DATA_TYPE();
						else
							memset(parent_t::m_pData+parent_t::m_iTail, 0, sizeof(T_DATA_TYPE)*1);
					}

					parent_t::m_iTail = (parent_t::m_iTail+1) % parent_t::m_nBufferCount;		// 1증가
					if (parent_t::m_iTail == parent_t::m_iHead)
						parent_t::m_iHead = (parent_t::m_iHead+1) % parent_t::m_nBufferCount;	// 1증가

				} while (bProceed);
			}
		}
		// Set Event
		parent_t::m_evtDataArrived.SetEvent();
		return nQueued;
	}

protected:
	BOOL IsIndexNearTailInbound(INDEX_T iIndex) const {
		if (!IsIndexInbound(iIndex))
			return FALSE;
		if (iIndex < parent_t::m_iTail)
			return parent_t::m_iTail - iIndex <= m_nMaxInboundLength;
		if (iIndex >= parent_t::m_iTail)
			return (parent_t::m_nBufferCount - iIndex - 1) + parent_t::m_iTail <= m_nMaxInboundLength;
		return FALSE;
	}
};


//#pragma pack(pop)
