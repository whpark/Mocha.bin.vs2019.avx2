/////////////////////////////////////////////////////////////////////////////
//
//
// HSMS
// PWH.
// 2009.03.02.
//
/////////////////////////////////////////////////////////////////////////////

#pragma once

#include <afxmt.h>

#include "AFX_EXT_HSMS.h"

#include "TList.hpp"
#include "misc/Socket2.h"
#include "misc/SimpleLog.h"
#include "misc/xUtil.h"

#include "HSMS_Definition.h"
#include "HSMSSession.h"
#include "HSMSConnection.h"
#include "SECSMessage.h"
#include "SECSMessageTemplate.h"

#pragma pack(push, 8)

//---------------------------------------------------------------------------
// Event Call Back Function
typedef BOOL PASCAL HSMSEventHandlerFunction(eHSMS_EVENT eEvent, eHSMS_DIRECTION eDirection, void* pCode, CHSMS* pHSMS, CHSMSConnection* pConnection, CHSMSSession* pSession, CSECSTransaction* pTransaction);

//---------------------------------------------------------------------------
class AFX_EXT_CLASS_HSMS CHSMS {
protected:
	HSMSEventHandlerFunction* m_HSMSEventHandlerFunc;
	void* m_pCode;

	CSocket2 m_socketListener;
	BOOL m_bSingleSession;				// TRUE if HSMS-SS. FALSE for HSMS-GS
	int m_nMaxConnection;
	BOOL m_bCloseOldConnectionOnOverflow = TRUE;	// 최대 연결 회수 초과 연결시 기존 연결된 Socket 을 종료시킴
	TList<CHSMSConnection> m_connections;
	TList<CSECSMessageTemplate> m_messageTemplates;
	mutable CCriticalSection m_csMessageTemplate;
	CCriticalSection m_csConnection;
	BOOL m_bClosing;

	DWORD m_dwTimeout[nHT];
	DWORD m_dwLinktestInterval;

public:
	CHSMS(BOOL bSingleSession = TRUE, HSMSEventHandlerFunction *HSMSEventHandlerFunc = NULL, void* pCode = NULL) {
		m_bSingleSession = bSingleSession;
		m_HSMSEventHandlerFunc = HSMSEventHandlerFunc;
		m_pCode = pCode;
		m_hConnector = NULL;
		m_nMaxConnection = 1;
		m_log.Init(_T("HSMS"), _T(""), _T("HSMS_%m%d.log"), TRUE);
		m_bClosing = FALSE;
		m_dwTimeout[HT_T3_Reply] = 45*1000;
		m_dwTimeout[HT_T5_ConnectSeperation] = 10*1000;
		m_dwTimeout[HT_T6_ControlTransaction] = 5*1000;
		m_dwTimeout[HT_T7_NotSelected] = 10*1000;
		m_dwTimeout[HT_T8_NetworkInterCharacter] = 5*1000;
		m_dwLinktestInterval = 120*1000;
	}
	~CHSMS() {
		Close(TRUE);
		//m_log.DeleteLogWnd();
	}

	// Callback or Virtual Callback
	void SetCallBackFunction(HSMSEventHandlerFunction *HSMSEventHandlerFunc, void* pCode) { m_HSMSEventHandlerFunc = HSMSEventHandlerFunc; m_pCode = pCode;  }
	virtual BOOL OnHSMSEventHandlerFunction(eHSMS_EVENT eEvent, eHSMS_DIRECTION eDirection, CHSMSConnection* pConnection, CHSMSSession* pSession, CSECSTransaction* pTransaction);

public:
	BOOL SetSingleSession(BOOL bSingleSession = TRUE);
	BOOL IsSingleSession() const { return m_bSingleSession; }

	void SetLinktestTimeInterval(DWORD dwInterval = 120*1000) { m_dwLinktestInterval = dwInterval; }
	DWORD GetLinktestTimeInterval() { return m_dwLinktestInterval; }

public:
	void SetMaxConnection(int nMaxConnection, BOOL bCloseOnOverflow) { m_nMaxConnection = nMaxConnection; m_bCloseOldConnectionOnOverflow = bCloseOnOverflow; }
	int GetMaxConnection() { return m_nMaxConnection; }
	BOOL StartPassive(void* pCookie, unsigned short nPort, BOOL bHost = FALSE, DWORD dwSystemByteStart = (DWORD)-1/*AUTO*/);
	BOOL StartActive(void* pCookie, LPCTSTR pszIP, unsigned short nPort, BOOL bHost = FALSE, DWORD dwSystemByteStart = (DWORD)-1/*AUTO*/);
	BOOL Close(BOOL bForce = FALSE);
protected:
	// Connection
	static DWORD WINAPI ConnectorT(LPVOID pParam);
	HANDLE m_hConnector;
	CEvent m_evtStopConnector;
	BOOL Connector(void* pCookie, BOOL bActive, LPCTSTR pszIP, unsigned short nPort, BOOL bHost, DWORD dwSystemByteStart);

public:
	// Session
	BOOL LoadMessageTemplate(LPCTSTR pszTemplateFilePath, unsigned short iSessionID = HSMS_M_DEFAULT_SESSION_ID);
	const CSECSMessageTemplate* FindMessageTemplate(unsigned short iSessionID = HSMS_M_DEFAULT_SESSION_ID) const;

	// Timeout
public:
	int GetTimeout(eHSMS_TIMEOUT eHT) const;	// in msec
	BOOL SetTimeout(eHSMS_TIMEOUT eHT, int iTimeout);	// in  msec

public:
	BOOL DeleteConnection(CHSMSConnection* pConnection);
	BOOL PopConnection(CHSMSConnection* pConnection);

public:
	BOOL FireEvent(eHSMS_EVENT eEvent, eHSMS_DIRECTION eDirection, CHSMSConnection* pConnection = NULL, CHSMSSession* pSession = NULL, CSECSTransaction* pTransaction = NULL);

	// Log
public:
	static LPCTSTR GetDirectionName(eHSMS_DIRECTION eDirection);
	CSimpleLog m_log;
public:
	void InitLog(LPCTSTR pszTaskName, LPCTSTR pszLogFilePath = _T("D:\\Log\\[TaskName]\\"), LPCTSTR pszLogFileNameFormat = _T("[TaskName]_%d.log"), BOOL bDeleteOld = TRUE) {
		m_log.Init(pszTaskName, pszLogFilePath, pszLogFileNameFormat, bDeleteOld, FALSE, CArchiveU::E_AUTO);
	}
	BOOL CreateLogWindow() { return m_log.CreateLogWnd(); }
	BOOL ShowLogWindow(int cmd) { return m_log.ShowLogWnd(cmd); }
	void RemoveLogWindow() { m_log.RemoveLogWnd(); }
	BOOL DeleteLogWindow() { return m_log.DeleteLogWnd(); }


protected:
	int m_nMaxPacketBufferSize = 8*1024;
public:
	bool SetMaxPacketBufferSize(int nMaxBufferSize = 8*1024);
	int GetMaxPacketBufferSize() const { return m_nMaxPacketBufferSize; }
};

AFX_EXT_DATA_HSMS extern BOOL g_bHSMSThrowException;

#pragma pack(pop)
