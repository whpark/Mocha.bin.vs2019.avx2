#pragma once

#pragma pack(push, 8)

#include "AFX_EXT_MIP.h"

#include "IPTask.h"
#include "IPData.h"
#include "IPCommandPipe.h"
//#include "../../data/Mocha.hxx"

class CIPDataServer;
class CIPProcess;
class CIPServer;
class CIPCommandPipe;

//=============================================================================
// CIPDataServer
//
class AFX_EXT_CLASS_MIP CIPDataServer : public CSimpleLogInterface {
protected:
	CIPData* m_pDataMocha;	// link.
	TList<CIPData> m_data;
	CProfile& m_rprofile;
	CString m_strMapNamePrefix;

public:
	CIPDataServer(CProfile& profile, CSimpleLog* pLog, LPCTSTR pszMapNamePrefx = _T(IP_DEFAULT_MAP_FILE_PREFIX)) : m_rprofile(profile), CSimpleLogInterface(pLog), m_pDataMocha(NULL), m_strMapNamePrefix(pszMapNamePrefx) {
	}
	virtual ~CIPDataServer() {
		m_pDataMocha = NULL;
	}

public:
	BOOL LoadData(CProfile* pProfile = NULL, BOOL bCreateProcessInfoData = TRUE);
	//BOOL StartServer();
	//BOOL StopServer();

};


//=============================================================================
// CIPProcess
//
class AFX_EXT_CLASS_MIP CIPProcess {
public:
	HANDLE m_hProcess;
	HANDLE m_hThread;

public:
	CIPProcess(HANDLE hProcess = NULL, HANDLE hThread = NULL) : m_hProcess(hProcess), m_hThread(hThread) { }
	CIPProcess(const CIPProcess& B) { *this = B; }
	~CIPProcess() { Close(TRUE, IP_DEFAULT_TIMEOUT); }
	void Close(BOOL bWait = FALSE, DWORD dwTimeout = INFINITE);
	CIPProcess& operator = (const CIPProcess& B);
};

//=============================================================================
// CIPServer
//

class AFX_EXT_CLASS_MIP CIPServer : public IIPTask, public CIPDataServer {
protected:
	CSocket2 m_socketListener;
	CEvent m_evtListenerStarted;
public:
	CIPServer(LPCTSTR pszMapNamePrefx = _T(IP_DEFAULT_MAP_FILE_PREFIX));
	virtual ~CIPServer(void);

	// Start/Close
public:
	virtual BOOL Start(unsigned short nPort = 0);
	virtual BOOL Close();
	virtual BOOL CloseMocha();

	virtual BOOL StartIP(LPCTSTR pszTaskName, LPCTSTR pszPathProfile, BOOL bExecuteProcesses, CString& strError);
	virtual BOOL ExecuteProcesses(BOOL bAll = TRUE);

	// Connection
protected:
	TList<CIPCommandPipe> m_commandPipes;
	HANDLE m_hListener;
	static DWORD WINAPI ListenerT(LPVOID pParam);
	DWORD Listener();

	TList<CIPProcess> m_processes;

protected:
	BOOL CloseCommandPipe(CIPCommandPipe* pCommandPipe);
	BOOL CloseCommandPipe(const char* pszProcessName);
	virtual BOOL OnConnected(CIPCommandPipe* pCommandPipe, HIPCOMMAND& rCMD);	// For Server Only
	virtual BOOL OnDisconnected(CIPCommandPipe* pCommandPipe);
	virtual BOOL OnIPCommandReceived(CIPCommandPipe* pCommandPipe, HIPCOMMAND& rCMD, BOOL& bClosed);
	virtual BOOL OnIPCommandAcked(CIPCommandPipe* pCommandPipe, HIPCOMMAND& rCMD);
	virtual BOOL OnIPCommandReplied(CIPCommandPipe* pCommandPipe, HIPCOMMAND& rCMD);

	BOOL ProcessTaskCommand(CIPCommandPipe* pCommandPipe, HIPCOMMAND& rCMD, BOOL& bClosed);
	BOOL ResetStepSequences();
};

#pragma pack(pop)
