///////////////////////////////////////////////////////////////////////////////
//
// qtl_iterator.hpp
//
//			QTL : (Q)QRS (T)Template (L)Library
//
//
// extension of std::recursive_mutex
//
//
// 2017.07.20. template class CSmartLock; // lockable object
//
// PWH
//
///////////////////////////////////////////////////////////////////////////////


#pragma once

#include <mutex>
#include <atomic>


#pragma pack(push, 8)
namespace qtl {	// Q(QRS) T(Template) L(library)


	#define _LOCK_GUARD_VAR(object, _lock)					std::lock_guard<const CSmartLock> _lock(object);
	#define _LOCK_GUARD(object)								_LOCK_GUARD_VAR(object, lock)
	#define _LOCK_GUARD_THIS_								_LOCK_GUARD(*this);
	

	//-------------------------------------------------------------------------
	// CSmartLock
	//
	class CSmartLock {
	protected:
		mutable std::unique_ptr<std::recursive_mutex> m_rMTX;
	public:
		CSmartLock() = default;
		bool IsInterlockedMode() const { return m_rMTX ? true : false; }
		void SetInterlockMode(bool bInterlockedMode = true) {
			if (bInterlockedMode) {
				if (!m_rMTX)
					m_rMTX = std::make_unique<std::recursive_mutex>();
			} else {
				if (m_rMTX)
					m_rMTX.release();
			}
		}
		bool try_lock() const { if (! m_rMTX) return false; m_rMTX->lock(); return true; }
		void lock()		const { if (m_rMTX) m_rMTX->lock(); }
		void unlock()	const { if (m_rMTX) m_rMTX->unlock(); }
		bool TryLock()	const { if (! m_rMTX) return false; m_rMTX->lock(); return true; }
		void Lock()		const { if (m_rMTX) m_rMTX->lock(); }
		void Unlock()	const { if (m_rMTX) m_rMTX->unlock(); }
	};



}	// namespace qtl
#pragma pack(pop)
