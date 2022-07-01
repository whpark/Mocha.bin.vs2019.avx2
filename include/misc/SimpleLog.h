///////////////////////////////////////////////////////////////////////////////
//
// Simple Log
//
// PWH. 2009.04.21
// 추후 변경 : CSimpleLogObject 를 만들어서, 로그 관련 데이터를 CSimpleLogObject에 두고, 
//           CSimpleLog 는 RefPointer로 CSimpleLogObject 를 가지고, CSimpeLog 는 interface 로서만 수행.
//           그러면, CSimpleLog 를 복사해서 여러군데서 사용할 때 편리할 것 같음. 일단은 CSimpeLogInterface 만 만들어 사용. 2013.12.05.
//
///////////////////////////////////////////////////////////////////////////////

#pragma once

#ifndef BIT
#define BIT(x) (0x01 << (x))
#endif

#include "AFX_EXT_MISC.h"

#include <afxmt.h>

#include "TList.hpp"
#include "TRefPointer.hpp"
#include "ArchiveU.h"

//=============================================================================
//

#pragma pack(push, 8)

enum eSIMPLE_LOG_TYPE {
	SLT_NORMAL = BIT(0),
	SLT_ERROR = BIT(31),
};

#ifndef FMT_STRA
#	ifdef _UNICODE
#		define FMT_STRA _T("%S")
#	else
#		define FMT_STRA _T("%s")
#	endif
#endif
#ifndef FMT_STRW
#	ifdef _UNICODE
#		define FMT_STRW _T("%s")
#	else
#		define FMT_STRW _T("%S")
#	endif
#endif

//AFX_EXT_API_MISC BOOL WriteLog(LPCSTR szFormat, ...);
//AFX_EXT_API_MISC BOOL WriteLog(DWORD fType, LPCSTR szFormat, ...);
//AFX_EXT_API_MISC BOOL WriteErrorLog(LPCSTR szFormat, ...);
//
//AFX_EXT_API_MISC BOOL WriteLog(LPCWSTR szFormat, ...);
//AFX_EXT_API_MISC BOOL WriteLog(DWORD fType, LPCWSTR szFormat, ...);
//AFX_EXT_API_MISC BOOL WriteErrorLog(LPCWSTR szFormat, ...);
//
//AFX_EXT_API_MISC void InitSimpleLog(LPCTSTR pszTaskName, LPCTSTR pszLogFilePath = _T("D:\\Log\\[TaskName]\\%m\\"), LPCTSTR pszLogFileNameFormat = _T("[TaskName]_%m_%d.log"), BOOL bLogDeleteOld = TRUE);
//AFX_EXT_API_MISC BOOL CreateSimpleLogWnd();
//AFX_EXT_API_MISC BOOL ShowSimpleLogWnd(int cmd);

//-----------------------------------------------------------------------------
// CSimpleLog
//
class AFX_EXT_CLASS_MISC CSimpleLog {
protected:
	CString m_strTaskName;						// Task Name
	CString m_strLogFileNameFormat;				// Format of Log File
	CString m_strLogFilePath;					// Path
	BOOL m_bLogDeleteOld;						// Delete Old Log files if TRUE
	CTimeSpan m_tsOld;							// Old File
	CCriticalSection m_csLog;					// Critical Section
	DWORD m_fLogType;							// Log Type MASK
	CString m_strFile;							// Current File Name
	CFile m_file;								// File
	TRefPointer<CArchiveU> m_pArchive;			// Archive
	CArchiveU::eENCODING m_eDefaultEncoding;	// Default Encoding
	BOOL m_bCloseFileAfterWrite;				// Close Log File After writing each Line
	volatile long m_nIndent;
	BOOL m_bIndent;

public:
	CSimpleLog() {
		m_fLogType = (DWORD)-1;
		m_strTaskName = _T("TaskName");
		m_strLogFilePath = _T("D:\\Log\\[TaskName]\\%m\\");
		m_strLogFileNameFormat = _T("[TaskName]_%m_%d.log");
		m_bLogDeleteOld = TRUE;
		m_pWndSimpleLog = NULL;
		m_bQuitLogWnd = FALSE;
		m_bCloseFileAfterWrite = FALSE;
		m_eDefaultEncoding = CArchiveU::E_AUTO;
		m_tsOld = CTimeSpan(25, 0, 0, 0);
		m_bIndent = TRUE;
		m_nIndent = 0;
	}
	~CSimpleLog() {
		m_pArchive.Release();
		if (m_file.m_hFile && (m_file.m_hFile != INVALID_HANDLE_VALUE))
			m_file.Close();
		DeleteLogWnd();
	}

	void Init(LPCTSTR pszTaskName, LPCTSTR pszLogFilePath = _T("D:\\Log\\[TaskName]\\%m\\"), LPCTSTR pszLogFileNameFormat = _T("[TaskName]_%m_%d.log"), BOOL bLogDeleteOld = TRUE, BOOL bCloseFileAfterWrite = FALSE, CArchiveU::eENCODING eEncoding = CArchiveU::E_AUTO, BOOL bLoad = FALSE);
	//void Init(LPCTSTR pszTaskName, LPCTSTR pszLogFilePath = _T("D:\\Log\\[TaskName]\\%m\\"), LPCTSTR pszLogFileNameFormat = _T("[TaskName]_%m_%d.log"), BOOL bLogDeleteOld = TRUE, BOOL bCloseFileAfterWrite = FALSE, CArchiveU::eENCODING eEncoding = CArchiveU::E_AUTO);
	BOOL SetEncoding(CArchiveU::eENCODING eEncoding = CArchiveU::E_AUTO);
	CArchiveU::eENCODING GetEncoding() const { return m_eDefaultEncoding; }

	BOOL UseIndent(BOOL bUse = TRUE) { BOOL bOldValue = m_bIndent; m_bIndent = bUse; return bOldValue; }
	void IncreaseIndent() { ::InterlockedIncrement(&m_nIndent); }
	void DecreaseIndent() { ::InterlockedDecrement(&m_nIndent); }

	CTimeSpan GetTimespanOld() const { return m_tsOld; }

	void SetLogMask(DWORD fLogType) { m_fLogType = fLogType; }
	DWORD GetLogMask() const { return m_fLogType; }

	// Write Log
	BOOL WriteLog(LPCSTR szFormat, ...);
	BOOL WriteLog(DWORD fType, LPCSTR szFormat, ...);
	BOOL WriteErrorLog(LPCSTR szFormat, ...);
	BOOL WriteErrorLog(DWORD fType, LPCSTR szFormat, ...);

	BOOL WriteLog(LPCWSTR szFormat, ...);
	BOOL WriteLog(DWORD fType, LPCWSTR szFormat, ...);
	BOOL WriteErrorLog(LPCWSTR szFormat, ...);
	BOOL WriteErrorLog(DWORD fType, LPCWSTR szFormat, ...);

	BOOL WriteLogV(DWORD fType, const char* pszFormat, va_list args, int nIndent = -1)		{ CStringA str; str.FormatV(pszFormat, args); return WriteLog0(fType, str, nIndent); }
	BOOL WriteLogV(DWORD fType, const wchar_t* pszFormat, va_list args, int nIndent = -1)	{ CStringW str; str.FormatV(pszFormat, args); return WriteLog0(fType, str, nIndent); }

	template < typename TCHAR, typename TSTRING >
	BOOL WriteLog0T(DWORD fType, TSTRING& strText, int nIndent = -1);
	BOOL WriteLog0(DWORD fType, CStringA& strText, int nIndent = -1);
	BOOL WriteLog0(DWORD fType, CStringW& strText, int nIndent = -1);
	BOOL WriteErrorLog0(DWORD fType, CStringA& strText, int nIndent = -1);
	BOOL WriteErrorLog0(DWORD fType, CStringW& strText, int nIndent = -1);

protected:
	BOOL OpenFile(const SYSTEMTIME& t);
	BOOL CloseFile();

	// UI
protected:
	CWnd* m_pWndSimpleLog;
	BOOL m_bQuitLogWnd;
public:
	CString GetLogPathTemplate() const { return m_strLogFilePath + m_strLogFileNameFormat; }
	const CString& GetCurrentLogPath() const { return m_strFile; }
	BOOL CreateLogWnd(CWnd* pParentWnd = NULL, LPCRECT pRect = NULL);
	static CWnd* CreateLogWndStatic(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID);
	BOOL SetLogWnd(CWnd* pWndSimpleLog);
	BOOL ShowLogWnd(int cmd);
	CWnd* GetLogWnd();
	void RemoveLogWnd();
	BOOL DeleteLogWnd();
	static BOOL DeleteLogWnd(CWnd* pWndSimpleLog);
	BOOL Load();
};


//=============================================================================
// Define Log Interface
//
class /*AFX_EXT_CLASS_MISC*/ CSimpleLogInterface {
protected:
	mutable CSimpleLog* m_pLog;
public:
	CSimpleLogInterface(CSimpleLog* pLog = NULL) : m_pLog(pLog) {}
	virtual ~CSimpleLogInterface() {}

	void SetLog(CSimpleLog* pLog) const { m_pLog = pLog; }
	CSimpleLog* GetLog() const { return m_pLog; }

public:
	virtual BOOL LogV(DWORD fType, LPCSTR pszFormat, va_list args, int nIndent = -1)  const { if (!m_pLog) return FALSE; return m_pLog->WriteLogV(fType, pszFormat, args, nIndent); }
	virtual BOOL LogV(DWORD fType, LPCWSTR pszFormat, va_list args, int nIndent = -1) const { if (!m_pLog) return FALSE; return m_pLog->WriteLogV(fType, pszFormat, args, nIndent); }

public:
	BOOL Log(LPCSTR pszFormat, ...) const						{ if (!m_pLog) return FALSE; va_list args; va_start(args, pszFormat); return LogV(0, pszFormat, args); }\
	BOOL Log(LPCWSTR pszFormat, ...) const						{ if (!m_pLog) return FALSE; va_list args; va_start(args, pszFormat); return LogV(0, pszFormat, args); }\
	BOOL Log(DWORD fType, LPCSTR pszFormat, ...) const			{ if (!m_pLog) return FALSE; va_list args; va_start(args, pszFormat); return LogV(fType, pszFormat, args); }\
	BOOL Log(DWORD fType, LPCWSTR pszFormat, ...) const			{ if (!m_pLog) return FALSE; va_list args; va_start(args, pszFormat); return LogV(fType, pszFormat, args); }\
	BOOL ErrorLog(LPCSTR pszFormat, ...) const					{ if (!m_pLog) return FALSE; va_list args; va_start(args, pszFormat); return LogV(0|SLT_ERROR, pszFormat, args); }\
	BOOL ErrorLog(LPCWSTR pszFormat, ...) const					{ if (!m_pLog) return FALSE; va_list args; va_start(args, pszFormat); return LogV(0|SLT_ERROR, pszFormat, args); }\
	BOOL ErrorLog(DWORD fType, LPCSTR pszFormat, ...) const		{ if (!m_pLog) return FALSE; va_list args; va_start(args, pszFormat); return LogV(fType|SLT_ERROR, pszFormat, args); }\
	BOOL ErrorLog(DWORD fType, LPCWSTR pszFormat, ...) const	{ if (!m_pLog) return FALSE; va_list args; va_start(args, pszFormat); return LogV(fType|SLT_ERROR, pszFormat, args); }\
};

#pragma pack(pop)
