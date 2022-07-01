//=================================================================
// by 하얀악마

#pragma once

#include "AFX_EXT_MISC.h"

// Environment Setting
class AFX_EXT_API_MISC CEnvironmentVariable {
protected:
	HKEY m_hKey;
	BOOL m_bBroadcasted;
	CString m_strLastError;
public:
	CEnvironmentVariable() {
		m_hKey = NULL;
		m_bBroadcasted = FALSE;
	}
	virtual ~CEnvironmentVariable() {
		Close();
		if (!m_bBroadcasted)
			Broadcast();
	}

	// Open/Close
	BOOL Open(BOOL bLocalMachine = TRUE);
	BOOL Close();
	BOOL Broadcast(UINT nTimeout = 3000);

	// Operation
	BOOL Get(LPCTSTR szVarName, CString& strValue);
	BOOL Set(LPCTSTR szVarName, LPCTSTR szValue);
	BOOL Delete(LPCTSTR szVarName);

	LPCTSTR GetErrorText() { return m_strLastError; }
};
