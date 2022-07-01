#pragma once

#include "AFX_EXT_MIP.h"

#include "IPTask.h"

#pragma pack(push, 8)

//=============================================================================
// CIPClient
//

class AFX_EXT_CLASS_MIP CIPClient : public IIPTask {
protected:
	CIPCommandPipe* m_pCommandPipe;

protected:
	DWORD m_dwThreadID;
	BOOL m_bQuitOnDisconnected;

public:
	CIPClient(void);
	virtual ~CIPClient(void);

// IPCommand Target
protected:
	TList<IIPCommandTarget> m_targets;
public:
	BOOL RegisterIPCommandTarget(IIPCommandTarget* pIPCommandTarget);
	BOOL UnregisterIPCommandTarget(IIPCommandTarget* pIPCommandTarget);

// IPCommand Loop
public:
	BOOL StartIPCommandLoop(BOOL bStandalone = FALSE);
	BOOL StopIPCommandLoop();
	BOOL IsIPCommandLoopStarted() const ;

	void QuitOnDisconnected(BOOL bQuit = TRUE);
	BOOL IsQuittingOnDisconnected() const { return m_bQuitOnDisconnected; }

	BOOL IsStandaloneMode() const { return m_bStandalone; }
	BOOL IsServerConnected() const { return !m_bStandalone && IsIPCommandLoopStarted(); }

	// IPCommand Loop
protected:
	CCriticalSection m_csStartStopIPCommandLoop;
	BOOL m_bStandalone;
	HANDLE m_hIPCommandLoop;
	static DWORD WINAPI IPCommandLoopT(LPVOID pParam);
	DWORD IPCommandLoop();
public:
	HANDLE GetCommandLoop() { return m_hIPCommandLoop; }

protected:
	// Events
	HANDLE m_hEvtStarted;
	HANDLE m_hEvtStopIPCommandLoop;
	HANDLE m_hEvtIPStepCommand;
	HANDLE m_hEvtResetIPStepCommand;

	// Connection
	HANDLE m_hConnector;
	DWORD m_dwThreadIDConnector;
	static DWORD WINAPI ConnectorT(LPVOID pParam);
	DWORD Connector();

protected:
	// Step
	TRefList<CIPCommand> m_cmdsInStep;
	HANDLE m_hIPStepCommandLoop;
	static DWORD WINAPI IPStepCommandLoopT(LPVOID pParam);
	DWORD IPStepCommandLoop();

public:
	BOOL EnqueueIPStepCommand(HIPCOMMAND rCMD);
	HIPCOMMAND FindPendingIPStepCommand(LPCSTR pszSession, LPCSTR pszCommand, IIPCommandTarget* pTargetSender = NULL);
	HIPCOMMAND FindPendingIPStepCommand(LPCSTR pszUnit, LPCSTR pszSession, LPCSTR pszCommand, IIPCommandTarget* pTargetSender = NULL);
	HIPCOMMAND FindPendingIPStepCommand(void* pKey, IIPCommandTarget* pTargetSender = NULL);
	intXX_t FindPendingIPStepCommand(TRefList<CIPCommand>& cmds, LPCSTR pszSession, LPCSTR pszCommand, IIPCommandTarget* pTargetSender = NULL);
	intXX_t FindPendingIPStepCommand(TRefList<CIPCommand>& cmds, LPCSTR pszUnit, LPCSTR pszSession, LPCSTR pszCommand, IIPCommandTarget* pTargetSender = NULL);
	intXX_t FindPendingIPStepCommand(TRefList<CIPCommand>& cmds, void* pKey, IIPCommandTarget* pTargetSender = NULL);
	BOOL ResumeIPStepCommand(PIPCOMMAND pCMD, DWORD dwStatusAdd = 0, DWORD dwStatusRemove = 0, IIPCommandTarget* pTargetSender = NULL);
	BOOL ResumeIPStepCommandByName(LPCSTR pszSession, LPCSTR pszCommand, DWORD dwStatusAdd = 0, DWORD dwStatusRemove = 0, IIPCommandTarget* pTargetSender = NULL);
	BOOL ResumeIPStepCommandByName(LPCSTR pszUnit, LPCSTR pszSession, LPCSTR pszCommand, DWORD dwStatusAdd = 0, DWORD dwStatusRemove = 0, IIPCommandTarget* pTargetSender = NULL);
	BOOL ResumeIPStepCommandByKey(void* pKey, DWORD dwStatusAdd = 0, DWORD dwStatusRemove = 0, IIPCommandTarget* pTargetSender = NULL);
	BOOL ResumeIPStepCommandRemote(LPCSTR pszTask, LPCSTR pszUnit, LPCSTR pszSession, LPCSTR pszCommand, LPCSTR pszPendingStep = NULL, DWORD dwStatusAdd = 0, DWORD dwStatusRemove = 0);

	BOOL ContinueIPStepCommand(PIPCOMMAND pCMD, DWORD dwStatusAdd = 0, DWORD dwStatusRemove = 0, IIPCommandTarget* pTargetSender = NULL) {
		return ResumeIPStepCommand(pCMD, dwStatusAdd, dwStatusRemove, pTargetSender);
	}
	BOOL ContinueIPStepCommandByName(LPCSTR pszSession, LPCSTR pszCommand, DWORD dwStatusAdd = 0, DWORD dwStatusRemove = 0, IIPCommandTarget* pTargetSender = NULL) {
		return ResumeIPStepCommandByName(pszSession, pszCommand, dwStatusAdd, dwStatusRemove, pTargetSender);
	}
	BOOL ContinueIPStepCommandByName(LPCSTR pszUnit, LPCSTR pszSession, LPCSTR pszCommand, DWORD dwStatusAdd = 0, DWORD dwStatusRemove = 0, IIPCommandTarget* pTargetSender = NULL) {
		return ResumeIPStepCommandByName(pszUnit, pszSession, pszCommand, dwStatusAdd, dwStatusRemove, pTargetSender);
	}
	BOOL ContinueIPStepCommandByKey(void* pKey, DWORD dwStatusAdd = 0, DWORD dwStatusRemove = 0, IIPCommandTarget* pTargetSender = NULL) {
		return ResumeIPStepCommandByKey(pKey, dwStatusAdd, dwStatusRemove, pTargetSender);
	}

	HIPCOMMAND FindReceivedIPCommand(LPCSTR pszSession, LPCSTR pszCommand, IIPCommandTarget* pTargetSender = NULL);
	HIPCOMMAND FindReceivedIPCommand(LPCSTR pszUnit, LPCSTR pszSession, LPCSTR pszCommand, IIPCommandTarget* pTargetSender = NULL);
	HIPCOMMAND FindReceivedIPCommand(void* pKey, IIPCommandTarget* pTargetSender = NULL);
	intXX_t FindReceivedIPCommand(TRefList<CIPCommand>& cmds, LPCSTR pszSession, LPCSTR pszCommand, IIPCommandTarget* pTargetSender = NULL);
	intXX_t FindReceivedIPCommand(TRefList<CIPCommand>& cmds, LPCSTR pszUnit, LPCSTR pszSession, LPCSTR pszCommand, IIPCommandTarget* pTargetSender = NULL);
	intXX_t FindReceivedIPCommand(TRefList<CIPCommand>& cmds, void* pKey, IIPCommandTarget* pTargetSender = NULL);

	enum eFIND_PENDING_IP_STEP_FLAG { FPIPSCMD_UNIT = BIT(0), FPIPSCMD_SESSION = BIT(1), FPIPSCMD_CMD = BIT(2), FPIPSCMD_KEY = BIT(3), FPIPSCMD_TARGET = BIT(4) };
	intXX_t FindPendingIPStepCommandImpl(DWORD dwFlag, TRefList<CIPCommand>& cmds, BOOL bFindFirstOne, LPCSTR pszUnit, LPCSTR pszSession, LPCSTR pszCommand, void* pKey, IIPCommandTarget* pTarget);
	intXX_t FindReceivedIPCommandImpl(DWORD dwFlag, TRefList<CIPCommand>& cmds, BOOL bFindFirstOne, LPCSTR pszUnit, LPCSTR pszSession, LPCSTR pszCommand, void* pKey, IIPCommandTarget* pTarget);

protected:
	// MUST be protected.
	intXX_t FindPendingIPStepCommandImpl(DWORD dwFlag, TRefList<CIPCommand>& cmds, BOOL bFindFirstOne, LPCSTR pszUnit, LPCSTR pszSession, LPCSTR pszCommand, void* pKey);
	intXX_t FindReceivedIPCommandImpl(DWORD dwFlag, TRefList<CIPCommand>& cmds, BOOL bFindFirstOne, LPCSTR pszUnit, LPCSTR pszSession, LPCSTR pszCommand, void* pKey);
	BOOL ResumeIPStepCommandImpl(TRefList<CIPCommand>& cmds, DWORD dwStatusAdd, DWORD dwStatusRemove);

protected:
	CCriticalSection m_csDispatch;
	const IP_COMMAND_TARGET_ENTRY* FindIPCommandTarget(PCIPCOMMAND pCMD, IIPCommandTarget** ppTarget = NULL);
	eIP_COMMAND_MESSAGE_RESULT FindTargetAndDispatch(CIPCommandPipe* pConnection, HIPCOMMAND& rCMD);

protected:
	friend class IIPCommandTarget;
	BOOL SendIPCommand(HIPCOMMAND& pCMDParent, DWORD eType, LPCSTR pszTask, LPCSTR pszUnit, LPCSTR pszSession, LPCSTR pszCommand, const CIPVariable* pVarParameter, CIPVariable* pVarResult, BOOL bWait, CIPCommandSync* pSync, DWORD dwTimeout, IIPCommandTarget* pTargetSender = NULL);
	BOOL ReplyIPCommand(HIPCOMMAND& pCMD);

public:
	BOOL PostIPCommand(LPCSTR pszTask,                     LPCSTR pszSession, LPCSTR pszCommand, IIPCommandTarget* pTargetSender = NULL);
	BOOL PostIPCommand(                                    LPCSTR pszSession, LPCSTR pszCommand, IIPCommandTarget* pTargetSender = NULL);
	BOOL PostIPCommand(LPCSTR pszTask,                     LPCSTR pszSession, LPCSTR pszCommand, const CIPVariable& var, IIPCommandTarget* pTargetSender = NULL);
	BOOL PostIPCommand(                                    LPCSTR pszSession, LPCSTR pszCommand, const CIPVariable& var, IIPCommandTarget* pTargetSender = NULL);
	BOOL PostIPCommand(LPCSTR pszTask,                     LPCSTR pszSession, LPCSTR pszCommand, const CIPVariable& var, CIPVariable& varResult, CIPCommandSync& sync, IIPCommandTarget* pTargetSender = NULL);
	BOOL PostIPCommand(                                    LPCSTR pszSession, LPCSTR pszCommand, const CIPVariable& var, CIPVariable& varResult, CIPCommandSync& sync, IIPCommandTarget* pTargetSender = NULL);

	BOOL SendIPCommand(LPCSTR pszTask,                     LPCSTR pszSession, LPCSTR pszCommand, DWORD dwTimeout = INFINITE, IIPCommandTarget* pTargetSender = NULL);
	BOOL SendIPCommand(                                    LPCSTR pszSession, LPCSTR pszCommand, DWORD dwTimeout = INFINITE, IIPCommandTarget* pTargetSender = NULL);
	BOOL SendIPCommand(LPCSTR pszTask,                     LPCSTR pszSession, LPCSTR pszCommand, const CIPVariable& var, DWORD dwTimeout = INFINITE, IIPCommandTarget* pTargetSender = NULL);
	BOOL SendIPCommand(                                    LPCSTR pszSession, LPCSTR pszCommand, const CIPVariable& var, DWORD dwTimeout = INFINITE, IIPCommandTarget* pTargetSender = NULL);
	BOOL SendIPCommand(LPCSTR pszTask,                     LPCSTR pszSession, LPCSTR pszCommand, const CIPVariable& var, CIPVariable& varResult, DWORD dwTimeout = INFINITE, IIPCommandTarget* pTargetSender = NULL);
	BOOL SendIPCommand(                                    LPCSTR pszSession, LPCSTR pszCommand, const CIPVariable& var, CIPVariable& varResult, DWORD dwTimeout = INFINITE, IIPCommandTarget* pTargetSender = NULL);

	BOOL PostIPCommand(LPCSTR pszTask,     LPCSTR pszUnit, LPCSTR pszSession, LPCSTR pszCommand, IIPCommandTarget* pTargetSender = NULL);
	BOOL PostIPCommand(LPCSTR pszTask,     LPCSTR pszUnit, LPCSTR pszSession, LPCSTR pszCommand, const CIPVariable& var, IIPCommandTarget* pTargetSender = NULL);
	BOOL PostIPCommand(LPCSTR pszTask,     LPCSTR pszUnit, LPCSTR pszSession, LPCSTR pszCommand, const CIPVariable& var, CIPVariable& varResult, CIPCommandSync& sync, IIPCommandTarget* pTargetSender = NULL);

	BOOL SendIPCommand(LPCSTR pszTask,     LPCSTR pszUnit, LPCSTR pszSession, LPCSTR pszCommand, DWORD dwTimeout = INFINITE, IIPCommandTarget* pTargetSender = NULL);
	BOOL SendIPCommand(LPCSTR pszTask,     LPCSTR pszUnit, LPCSTR pszSession, LPCSTR pszCommand, const CIPVariable& var, DWORD dwTimeout = INFINITE, IIPCommandTarget* pTargetSender = NULL);
	BOOL SendIPCommand(LPCSTR pszTask,     LPCSTR pszUnit, LPCSTR pszSession, LPCSTR pszCommand, const CIPVariable& var, CIPVariable& varResult, DWORD dwTimeout = INFINITE, IIPCommandTarget* pTargetSender = NULL);

	BOOL SendIPStepCommand(HIPCOMMAND& rCMDParent, LPCSTR pszTask, LPCSTR pszUnit, LPCSTR pszSession, LPCSTR pszCommand, const CIPVariable& var, DWORD dwTimeout = INFINITE, IIPCommandTarget* pTargetSender = NULL);
	BOOL SendIPStepCommand(PIPCOMMAND pCMDParent, LPCSTR pszTask, LPCSTR pszUnit, LPCSTR pszSession, LPCSTR pszCommand, const CIPVariable& var, DWORD dwTimeout = INFINITE, IIPCommandTarget* pTargetSender = NULL);

public:
	void QuitAll();
	void RestartAll();
	void RestartDead();
	void ResetAllSequences();

protected:
	virtual BOOL OnIPCommandReceived(CIPCommandPipe* pConnection, HIPCOMMAND& rCMD, BOOL& bClosed);
	virtual BOOL OnIPCommandAcked(CIPCommandPipe* pConnection, HIPCOMMAND& rCMD);
	virtual BOOL OnIPCommandReplied(CIPCommandPipe* pConnection, HIPCOMMAND& rCMD);
	virtual BOOL OnConnected(CIPCommandPipe* pConnection, HIPCOMMAND& rCMD);
	virtual BOOL OnDisconnected(CIPCommandPipe* pConnection);
	BOOL ProcessTaskCommand(CIPCommandPipe* pCommandPipe, HIPCOMMAND& rCMD, BOOL& bClosed);

protected:
	virtual BOOL OnIPCommandQuit();
	virtual BOOL Quit();	// User Provided Quitting Function. Default : Posting WM_SYSCOMMAND(w/SC_CLOSE, and SC_CLOSE_FORCED), WM_QUIT
	CCriticalSection m_csResetSequences;
	BOOL OnIPCommandResetSequences();

public:
	BOOL IsValidCmd(PCIPCOMMAND pCMD);
	BOOL IsValidCmdReceived(PCIPCOMMAND pCMD);
	BOOL IsValidCmdSent(PCIPCOMMAND pCMD);
	HIPCOMMAND GetHCMD(PIPCOMMAND pCMD);
};

#pragma pack(pop)
