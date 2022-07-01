#pragma once

#include "AFX_EXT_MISC.h"
#include "xUtil.h"
#include "TList.hpp"

#pragma pack(push, 8)	// default align. (8 bytes)

//=============================================================================
//

class AFX_EXT_CLASS_MISC CStopWatchItem {
public:
	CString m_strName;
	DWORD m_dwTick;
public:
	CStopWatchItem(LPCTSTR pszName = NULL) {
		m_dwTick = GetTickCount();
		m_strName = pszName;
	}
	CStopWatchItem(const CStopWatchItem& B) { *this = B; }
	CStopWatchItem& operator = (const CStopWatchItem& B) { if (this != &B) { m_strName = B.m_strName; m_dwTick = B.m_dwTick; } return *this; }
};

class AFX_EXT_CLASS_MISC CStopWatch {
protected:
	DWORD m_dwTickStart;
	TRefList<CStopWatchItem> m_laps;
	struct LOG_FILE {
		CCriticalSection cs;
		CFile file;
		TRefPointer< CArchiveU > pArchive;
	};
	TRefPointer<LOG_FILE> m_pLogFile;

public:
	CStopWatch(LPCTSTR pszFileName = NULL, BOOL bAutoTick = TRUE, DWORD dwTickStart = 0) {
		Start(pszFileName, bAutoTick, dwTickStart);
	}
	CStopWatch(const CStopWatch& B) { *this = B; }
	~CStopWatch() {
		Close();
	}
	CStopWatch& operator = (const CStopWatch& B);

public:
	DWORD GetTickStart() const { return m_dwTickStart; }
	DWORD GetLastTick() const;

public:
	BOOL Start(LPCTSTR pszFileName = NULL, BOOL bAutoTick = TRUE, DWORD dwTickStart = 0);
	BOOL Close();
public:
	BOOL InsertText(LPCSTR pszFmt, ...);
	BOOL InsertText(LPCWSTR pszFmt, ...);
	BOOL InsertLap(LPCSTR pszFmt, ...);
	BOOL InsertLap(LPCWSTR pszFmt, ...);
protected:
	BOOL InsertText0(LPCTSTR pszText);
	BOOL InsertLap0(LPCTSTR pszName);
};

//=============================================================================
//

class AFX_EXT_CLASS_MISC CHDStopWatchItem {
public:
	CString m_strName;
	LARGE_INTEGER m_liTick;
public:
	CHDStopWatchItem(LPCTSTR pszName = NULL) {
		QueryPerformanceCounter(&m_liTick);
		m_strName = pszName;
	}
	CHDStopWatchItem(const CHDStopWatchItem& B) { *this = B; }
	CHDStopWatchItem& operator = (const CHDStopWatchItem& B) { if (this != &B) { m_strName = B.m_strName; m_liTick = B.m_liTick; } return *this; }
};

class AFX_EXT_CLASS_MISC CHDStopWatch {
protected:
	LARGE_INTEGER m_liTickStart;
	LARGE_INTEGER m_liFrequency;
	TRefList<CHDStopWatchItem> m_laps;
	struct LOG_FILE {
		CCriticalSection cs;
		CFile file;
		TRefPointer< CArchiveU > pArchive;
	};
	TRefPointer<LOG_FILE> m_pLogFile;

public:
	CHDStopWatch(LPCTSTR pszFileName = NULL, BOOL bAutoTick = TRUE, int64_t liTickStart = 0) {
		Start(pszFileName, bAutoTick, liTickStart);
	}
	CHDStopWatch(const CHDStopWatch& B) { *this = B; }
	~CHDStopWatch() {
		Close();
	}
	CHDStopWatch& operator = (const CHDStopWatch& B);

public:
	int64_t GetTickStart() const { return m_liTickStart.QuadPart; }
	int64_t GetLastTick() const;

public:
	BOOL Start(LPCTSTR pszFileName = NULL, BOOL bAutoTick = TRUE, int64_t liTickStart = 0);
	BOOL Close();
public:
	BOOL InsertText(LPCSTR pszFmt, ...);
	BOOL InsertText(LPCWSTR pszFmt, ...);
	BOOL InsertLap(LPCSTR pszFmt, ...);
	BOOL InsertLap(LPCWSTR pszFmt, ...);
protected:
	BOOL InsertText0(LPCTSTR pszText);
	BOOL InsertLap0(LPCTSTR pszName);
};

#pragma pack(pop)
