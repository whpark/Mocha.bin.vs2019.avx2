#pragma once

#include "AFX_EXT_HSMS.h"

#include "HSMS_Definition.h"
#include "HSMSSession.h"

#pragma pack(push, 8)

class CHSMSTimeoutEvent;
class CHSMSSession;
class CHSMSTimeoutEvent;
class CHSMSConnection;
class CHSMS;

//---------------------------------------------------------------------------
class AFX_EXT_CLASS_HSMS CHSMSTimeoutEvent {
	friend class CHSMSConnection;
protected:
	DWORD m_dwTickStart;
	DWORD m_dwTimeout;	// TickCount (NOT end-tick)
	eHSMS_TIMEOUT m_eHT;
	eHSMS_EVENT m_eHE;
	CHSMSSession* m_pSession;
	CSECSTransaction* m_pTransaction;
public:
	CHSMSTimeoutEvent(eHSMS_TIMEOUT eHT = HT_NONE, eHSMS_EVENT eHE = HSMS_E_NONE, DWORD dwTimeout = INFINITE, CHSMSSession* pSession = NULL, CSECSTransaction* pTransaction = NULL)
		: m_eHT(eHT), m_eHE(eHE), m_dwTimeout(dwTimeout), m_pSession(pSession), m_pTransaction(pTransaction)
	{
		m_dwTickStart = GetTickCount();
	}

	BOOL operator == (const CHSMSSession* pSession) const { return m_pSession == pSession; }
	BOOL operator == (const CSECSTransaction* pTransaction) const { return m_pTransaction == pTransaction; }
	BOOL operator == (const CHSMSTimeoutEvent& B) const {
		return (m_eHT == B.m_eHT) && (m_eHE == B.m_eHE) && (m_pSession == B.m_pSession) && (m_pTransaction == B.m_pTransaction);
	}

	BOOL operator < (DWORD dwTickCurrent) {
		if (m_dwTimeout == INFINITE)
			return FALSE;	// NOT REACHED
		DWORD dwTickEnding = m_dwTickStart + m_dwTimeout;
		if (dwTickEnding < m_dwTickStart) {	// case of overflow
			if (m_dwTickStart <= dwTickCurrent)
				return FALSE;
		}
		return dwTickEnding < dwTickCurrent;
	}
	BOOL operator == (DWORD dwTickCurrent) {
		if (m_dwTimeout == INFINITE)
			return FALSE;
		return (m_dwTickStart + m_dwTimeout) == dwTickCurrent;
	}
	BOOL operator > (DWORD dwTickCurrent) {
		if (m_dwTimeout == INFINITE)
			return TRUE;	// NOT OVERED
		DWORD dwTickEnding = m_dwTickStart + m_dwTimeout;
		if (dwTickEnding < m_dwTickStart) {	// case of overflow
			if (m_dwTickStart <= dwTickCurrent)
				return TRUE;
		}
		return dwTickEnding > dwTickCurrent;
		//return !(*this == dwTickCurrent) && !(*this < dwTickCurrent);
	}
};

//---------------------------------------------------------------------------
class AFX_EXT_CLASS_HSMS CHSMSConnection {
public:
	//eTIMER_EVENT { TE_NONE, TE_T3_REPLY, TE_T5_ConnectSeperation, TE_T6_ControlTransaction, TE_T7_NotSelected, };
private:
	friend class CHSMS;
	friend class CHSMSSession;
private:
	CHSMSConnection() {}
protected:
	CHSMS* m_pHSMS;
	CCriticalSection m_csHSMS;
	CString m_strIPPeer;
	unsigned short m_nPortPeer;
	CSocket2 m_socketWorker;
	TList<CHSMSSession> m_sessions;
	CCriticalSection m_csSession;
	//CEvent m_evtSeperated;
	int m_nMaxPacketBufferSize = 8*1024;

protected:
	DWORD m_dwLastSystemByte;
	BOOL m_bActive;
	BOOL m_bHost;
	HANDLE m_hEvtClosed;
	std::atomic<bool> m_bClosing;
	DWORD m_dwTickLastPing;

public:
	CHSMSConnection(CHSMS* pHSMS, SOCKET hSocket, int nMaxPacketBufferSize = 8*1024);
	~CHSMSConnection();

	BOOL AttachSocket(SOCKET socket) {
		if (m_socketWorker.m_hSocket != INVALID_SOCKET)
			return FALSE;
		return m_socketWorker.Attach(socket);
	}

public:
	LPCTSTR GetIPPeer() { return m_strIPPeer; }
	unsigned short GetPortPeer() { return m_nPortPeer; }

public:
	BOOL IsActive() const { return m_bActive; }
	BOOL IsHost() const { return m_bHost; }
	CHSMS* GetHSMS() { return m_pHSMS; }

protected:
	BOOL Start(BOOL bActive, BOOL bHost, DWORD dwSystemByteStart, HANDLE hEvtClosed = NULL);
	BOOL Close(BOOL bForce = FALSE);

protected:
	BOOL AddSession(unsigned short iSessionID = HSMS_M_DEFAULT_SESSION_ID);
	BOOL DeleteSession(unsigned short iSessionID = HSMS_M_DEFAULT_SESSION_ID);
	BOOL DeleteSession(CHSMSSession* pSession);
public:
	CHSMSSession* GetSession(unsigned short iSessionID = HSMS_M_DEFAULT_SESSION_ID);

protected:
	CCriticalSection m_csSendSECSMessage;
public:
	BOOL SendSECSMessage(CSECSMessage& msg, BOOL bAutoSystemByte = TRUE);

public:
	BOOL AddTimeoutEvent(eHSMS_TIMEOUT eHT, eHSMS_EVENT eHE, CHSMSSession* pSession, CSECSTransaction* pTransaction);
	BOOL DeleteTimeoutEvent(eHSMS_TIMEOUT eHT, eHSMS_EVENT eHE, CHSMSSession* pSession, CSECSTransaction* pTransaction);

protected:
	BOOL ProcessReceivedSECSMessage(const CSECSMessage& msg);

	//BOOL Read(void* pBuf, int nSize, BOOL bAddToLog = FALSE);
	BOOL Write(void* pBuf, int nSize, BOOL bAddToLog = FALSE);

	BOOL ControlSend(unsigned short iSessionID, unsigned char eSessionType, unsigned char eStatus = 0);
	BOOL ControlSendSelect(CHSMSSession* pSession);
	BOOL ControlSendDeSelect(CHSMSSession* pSession);
	BOOL ControlSendLinkTest();
	BOOL ControlSendSeparate();

	// Worker (Sending & Receiving)
	CCriticalSection m_csTimeoutEvent;
	TList<CHSMSTimeoutEvent> m_lstTimeoutEvents;
	static DWORD WINAPI WorkerT(LPVOID pParam);
	HANDLE m_hWorker;
	CEvent m_evtStopWorker;
	BOOL Worker();

protected:
	// IO Completion Port
	HANDLE m_hReader;
	static DWORD __stdcall ReaderT(LPVOID pParam);
	DWORD Reader();

};

#pragma pack(pop)
