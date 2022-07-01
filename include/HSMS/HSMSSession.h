#pragma once

#include <afxmt.h>

#include "AFX_EXT_HSMS.h"

#include "HSMS_Definition.h"
#include "SECSMessageTemplate.h"
#include "misc/SimpleLog.h"

#pragma pack(push, 8)

class CHSMSSession;
class CHSMSConnection;
class CHSMS;


//---------------------------------------------------------------------------
class AFX_EXT_CLASS_HSMS CSECSTransaction {
protected:
	CString m_strName;
	CSECSMessage m_msgs[2];
	CSECSVariableTemplate m_varTemplates[2];
	BOOL m_bReplied;

	BOOL m_bLog;

	int m_eThread;
	uintptr_t m_cookie;

	static DWORD s_idUnique;
	DWORD m_idUnique;

public:
	CSECSTransaction(LPCTSTR pszName = NULL, int eThread = 0, uintptr_t cookie = 0, BOOL bLog = TRUE) : m_strName(pszName), m_eThread(eThread), m_cookie(cookie), m_bLog(bLog), m_bReplied(FALSE) {
		m_idUnique = ::InterlockedIncrement(&s_idUnique);
	}

public:
	void SetName(LPCTSTR pszName) { m_strName = pszName; }
	LPCTSTR GetName() const { return m_strName; }
	CStringA GetNameA() const { return CStringA(m_strName); }
	CStringW GetNameW() const { return CStringW(m_strName); }

	DWORD GetUniqueID() const { return m_idUnique; }

	BOOL IsReplied() const { return m_bReplied; }
	void SetReplied(BOOL bReplied = TRUE) { m_bReplied = bReplied; }

public:
	CSECSMessage&					GetMessage(BOOL bPrimary = TRUE) { return m_msgs[bPrimary ? 0 : 1]; }
	const CSECSMessage&				GetMessage(BOOL bPrimary = TRUE) const { return m_msgs[bPrimary ? 0 : 1]; }
	CSECSVariable&					GetVariable(BOOL bPrimary = TRUE) { return m_msgs[bPrimary ? 0 : 1].GetVariable(); }
	const CSECSVariable&			GetVariable(BOOL bPrimary = TRUE) const { return m_msgs[bPrimary ? 0 : 1].GetVariable(); }
	CSECSVariableTemplate&			GetVariableTemplate(BOOL bPrimary = TRUE) { return m_varTemplates[bPrimary ? 0 : 1]; }
	const CSECSVariableTemplate&	GetVariableTemplate(BOOL bPrimary = TRUE) const { return m_varTemplates[bPrimary ? 0 : 1]; }

public:
	CSECSVariableIterator GetPrimaryVariableIterator() { return GetVariableIterator(TRUE); }
	CSECSVariableIterator GetSecondaryVariableIterator() { return GetVariableIterator(FALSE); }
	CSECSVariableIterator GetVariableIterator(BOOL bPrimary = TRUE) { return CSECSVariableIterator(&m_varTemplates[bPrimary?0:1], &m_msgs[bPrimary?0:1].GetVariable()); }

public:
	void SetLog(BOOL bLog = TRUE) { m_bLog = bLog; }
	BOOL GetLog() const { return m_bLog; }

	void SetThread(int eThread) { m_eThread = eThread; }
	int GetThread() const { return m_eThread; }

	void SetCookie(uintptr_t cookie) { m_cookie = cookie; }
	uintptr_t GetCookie() const { return m_cookie; }

public:
#ifdef _UNICODE
	int operator == (LPCSTR pszName) const { return m_strName == CString(pszName); }
	int operator == (LPCWSTR pszName) const { return m_strName == pszName; }
#else
	int operator == (LPCSTR pszName) const { return m_strName == pszName; }
	int operator == (LPCWSTR pszName) const { return m_strName == CString(pszName); }
#endif
	int operator == (DWORD dwSystemByte) const { return m_msgs[0].GetSystemByte() == dwSystemByte; }

};

//---------------------------------------------------------------------------
class AFX_EXT_CLASS_HSMS CSECSTransactionTemplate : public CSECSTransaction {
protected:
	eHSMS_TRANSACTION_SENDER m_eSender;
	unsigned char m_iStream;
	unsigned char m_iFunction;
	BOOL m_bWait;

public:
	CSECSTransactionTemplate(LPCTSTR pszName = NULL, int iStream = -1, int iFunction = -1, eHSMS_TRANSACTION_SENDER eSender = HSMS_TS_BOTH, BOOL bWait = TRUE, BOOL bLog = TRUE)
		: CSECSTransaction(pszName), m_iStream(iStream), m_iFunction(iFunction), m_eSender(eSender), m_bWait(bWait) { m_bLog = bLog; }

#ifdef _UNICODE
	BOOL operator == (LPCSTR pszName) const { return m_strName == CString(pszName); }
	BOOL operator == (LPCWSTR pszName) const { return m_strName == pszName; }
#else
	BOOL operator == (LPCSTR pszName) const { return m_strName == pszName; }
	BOOL operator == (LPCWSTR pszName) const { return m_strName == CString(pszName); }
#endif

public:
	int GetStream() const { return m_iStream; }
	void SetStream(int iStream) { m_iStream = iStream; }

	void SetFunction(int iFunction) { m_iFunction = iFunction; }
	int GetFunction() const { return m_iFunction; }

	void SetStreamFunction(int iStream, int iFunction) {
		m_iStream = iStream;
		m_iFunction = iFunction;
	}

	void SetSender(eHSMS_TRANSACTION_SENDER eSender) { m_eSender = eSender; }
	eHSMS_TRANSACTION_SENDER GetSender() const { return m_eSender; }

	void SetWait(BOOL bWait = TRUE) { m_bWait = bWait; }
	BOOL GetWait() const { return m_bWait; }

public:
	CSECSTransaction* GetNewTransaction(const CSECSMessage& msgPrimary) const;
	CSECSTransaction* GetNewTransaction() const;

public:
	BOOL StoreAsLegacyForm(CArchiveU& ar) const;
	BOOL StoreAsStruct(CArchiveU& ar) const;
};

//---------------------------------------------------------------------------
class AFX_EXT_CLASS_HSMS CHSMSSession {
	friend class CHSMSConnection;
public:
	enum eSESSION_STATUS { SS_SELECTING, SS_SELECTED, SS_DESELECTING, SS_DESELECTED, };
protected:
	CHSMSConnection* m_pConnection;
	const CSECSMessageTemplate* m_pMessageTemplate;
	BOOL m_bHost;
	eSESSION_STATUS m_eSessionStatus;
	unsigned short m_iSessionID;		// Session/Device ID
	unsigned short m_iSessionIDPeer;	// Peer Session/Device ID

	mutable CCriticalSection m_csTransaction;
	TList<CSECSTransaction> m_transactions;

	CSimpleLog m_log;

	CEvent m_evtSessionClosed;

public:
	CHSMSSession(CHSMSConnection* pConnection, const CSECSMessageTemplate* pMessageTemplate, BOOL bHost = FALSE, unsigned short iSessionIDPeer = HSMS_M_DEFAULT_SESSION_ID)
		: m_evtSessionClosed(FALSE, TRUE) {
		m_eSessionStatus = SS_DESELECTED;
		Init(pConnection, pMessageTemplate, bHost, iSessionIDPeer);
		m_evtSessionClosed.ResetEvent();
		m_transactions.SetInterlockMode(true);
	}
	~CHSMSSession() {
	}

	BOOL operator == (unsigned short iSessionID) const { return m_iSessionID == iSessionID; }

	unsigned short GetSessionID() const { return m_iSessionID; }
	unsigned short GetSessionIDPeer() const { return m_iSessionIDPeer; }

	HANDLE GetEvtSessionClosed() { return m_evtSessionClosed.m_hObject; }

protected:
	void Init(CHSMSConnection* pConnection, const CSECSMessageTemplate* pMessageTemplate, BOOL bHost = FALSE, unsigned short iSessionIDPeer = HSMS_M_DEFAULT_SESSION_ID);

public:
	BOOL IsHost() const { return m_bHost; }

public:
	BOOL IsSelecting() const { return m_eSessionStatus == SS_SELECTING; }
	BOOL IsSelected() const { return m_eSessionStatus == SS_SELECTED; }
	BOOL IsDeselecting() const { return m_eSessionStatus == SS_DESELECTING; }
	BOOL IsDeselected() const { return m_eSessionStatus == SS_DESELECTED; }
protected:
	eSESSION_STATUS SetSelected() { return m_eSessionStatus = SS_SELECTED; }
	eSESSION_STATUS SetSelecting() { return m_eSessionStatus = SS_SELECTING; }
	eSESSION_STATUS SetDeselecting() { return m_eSessionStatus = SS_DESELECTING; }
	eSESSION_STATUS SetDeselected() { m_evtSessionClosed.SetEvent(); return m_eSessionStatus = SS_DESELECTED; }
	void SetSessionStatus(eSESSION_STATUS eSessionStatus) { m_eSessionStatus = eSessionStatus; }

public:
	CHSMSConnection* GetConnection() const { return m_pConnection; }

public:
	CSECSTransaction* GetNewTransaction(LPCSTR pszName);
	CSECSTransaction* GetNewTransaction(LPCWSTR pszName);
	BOOL IsTransactionValid(const CSECSTransaction* pTransaction) const { CS cs(&m_csTransaction); return m_transactions.Search(pTransaction) ? TRUE : FALSE; }
protected:
	CCriticalSection m_csNewTransaction;
	template <typename TCHAR>
	CSECSTransaction* GetNewTransactionT(const TCHAR* pszName);

public:
// Transaction
	BOOL SendHSMSTransaction(CSECSTransaction* pTransaction);
	BOOL ReplyHSMSTransaction(CSECSTransaction* pTransaction);
	BOOL AbortHSMSTransaction(CSECSTransaction* pTransaction);
	BOOL DeleteTransaction(CSECSTransaction* pTransaction);

protected:
	BOOL ProcessReceivedSECSMessage(const CSECSMessage& msg);

protected:
	BOOL AddMessageLog(const CSECSMessage& msg, const CSECSVariableTemplate* pVarTemplate, LPCTSTR pszTransactionName, eHSMS_DIRECTION eDirection);
	BOOL AddMessageLog(CSECSVariableIterator iter, CArchive& ar, int nDepth = 0);
};

#pragma pack(pop)
