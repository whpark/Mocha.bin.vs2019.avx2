#pragma once

#include <afxmt.h>

#include "AFX_EXT_MIP.h"

#include "IP_Definitions.h"
#include "IPCommand.h"

#if (_MSC_VER <= _MSC_VER_VS6)
typedef unsigned long ULONG_PTR, *PULONG_PTR;
#endif

#pragma pack(push, 8)

class CIPClient;
class IIPTask;
class IIPCommandTarget;
class CIPCommand;
class CIPCommandSync;
template < class WNDCLASS > class TIPCommandTarget;

#define ip_handler
typedef BOOL (IIPCommandTarget::*IPCommandFunction)(CIPCommand& cmd);

class __declspec(align(8)) LIST_CMD : public TRefList<CIPCommand> {
public:
	LIST_CMD() : TRefList<CIPCommand>(true) {}
};

class __declspec(align(8)) IP_COMMAND_TARGET_ENTRY_FUNC {
public:
	const char* pszSession;
	const char* pszCommand;
	mutable IPCommandFunction Func;
	BOOL bExclusive;
	BOOL bQueue;
	BOOL bLog;
	const TCHAR* pszPathLog;
};

class __declspec(align(8)) IP_COMMAND_TARGET_ENTRY : public IP_COMMAND_TARGET_ENTRY_FUNC {
public:
	mutable LIST_CMD cmdsRunning;
	mutable LIST_CMD cmdsPending;
	CSimpleLog* pLog;

	IP_COMMAND_TARGET_ENTRY() { pszSession = NULL; pszCommand = NULL; Func = NULL; bExclusive = FALSE; bQueue = FALSE; bLog = TRUE; pLog = NULL; }
	IP_COMMAND_TARGET_ENTRY(const IP_COMMAND_TARGET_ENTRY_FUNC& B) { *this = B; }
	IP_COMMAND_TARGET_ENTRY& operator = (const IP_COMMAND_TARGET_ENTRY_FUNC& B) {
		if ((IP_COMMAND_TARGET_ENTRY_FUNC*)this != &B) {
			pszSession = B.pszSession;
			pszCommand = B.pszCommand;
			Func = B.Func;
			bExclusive = B.bExclusive;
			bQueue = B.bQueue;
			bLog = B.bLog;
			pszPathLog = B.pszPathLog;
		}
		pLog = NULL;
		return *this;
	}
	IP_COMMAND_TARGET_ENTRY(const IP_COMMAND_TARGET_ENTRY& B) { __throw_exception__("IP_COMMAND_TARGET, Copy..."); *this = B; }
	IP_COMMAND_TARGET_ENTRY& operator = (const IP_COMMAND_TARGET_ENTRY& B) {
		// 여기 오면 안됨.
		__throw_exception__("IP_COMMAND_TARGET, Copy...");
		if (this != &B) {
			//cmdsRunning = B.cmdsRunning;
			//cmdsPending = B.cmdsPending;
			pszSession = B.pszSession;
			pszCommand = B.pszCommand;
			Func = B.Func;
			bExclusive = B.bExclusive;
			bQueue = B.bQueue;
			bLog = B.bLog;
			pszPathLog = B.pszPathLog;
			pLog = B.pLog;
		}
		return *this;
	}
};

class AFX_EXT_CLASS_MIP IP_COMMAND_TARGET {
public:
	const IP_COMMAND_TARGET_ENTRY_FUNC* table;
	const IP_COMMAND_TARGET* parent;
};

//=============================================================================
// IIPCommandTarget
//

class AFX_EXT_CLASS_MIP IIPCommandTarget {
friend class CIPClient;
protected:
	CIPClient* m_pIPClient;
private:
	DStringA m_strUnit;
private:
	const BOOL m_bLocalThread;
	BOOL m_bCreated;
	DWORD m_dwThreadID;

//protected:
//	static const DWORD s_dwSign;	// 원래는 TIPCommandTarget에 있어야 하는데, template 이라서, 여기로 옮김

public:
	IIPCommandTarget(CIPClient* pIPClient = NULL, BOOL bLocalThread = FALSE);
	virtual ~IIPCommandTarget();

	CIPClient* GetIPClient() { return m_pIPClient; }
	const CIPClient* GetIPClient() const { return m_pIPClient; }
protected:
	IIPCommandTarget(const IIPCommandTarget& B) : m_pIPClient(NULL), m_bLocalThread(FALSE) { ASSERT(FALSE); }
	IIPCommandTarget& operator = (const IIPCommandTarget& B) { ASSERT(FALSE); return *this; }

protected:
	// Init And Destroy
	BOOL CreateIPCommandTarget(CIPClient* pIPClient);
	BOOL DestroyIPCommandTarget();
public:
	const DStringA& GetUnitName() const { return m_strUnit; }
	void SetUnitName(LPCSTR pszUnit) { m_strUnit = pszUnit; }

private:
	virtual BOOL ResetSequences();
protected:
	HIPCOMMAND m_rCMDCurrent;
	TRefPointer<IP_COMMAND_TARGET_ENTRY>	m_tblIPCommandTargetEntry;
	const IP_COMMAND_TARGET_ENTRY*			FindIPCommandTarget(PCIPCOMMAND pCMD);
	virtual eIP_COMMAND_MESSAGE_RESULT		DispatchIPCommand(HIPCOMMAND rCMD, const IP_COMMAND_TARGET_ENTRY* pEntry = NULL);
	static void CALLBACK					DispatchIPCommandAPC(ULONG_PTR pParam);

	virtual BOOL							OnPreIPCommand(HIPCOMMAND rCMD, const IP_COMMAND_TARGET_ENTRY* pEntry);

	virtual BOOL							DoIPCommand(HIPCOMMAND rCMD, const IP_COMMAND_TARGET_ENTRY* pEntry);

	// IPCommand Loop (for the local thread)
private:
	HANDLE m_hEvtStopIPCommandLoop;
private:
	HANDLE m_hIPCommandLoop;
	static DWORD WINAPI IPCommandLoopT(LPVOID pParam);
	DWORD IPCommandLoop();

protected:
	DECLARE_IP()

	// Operation
	// Send/Post IPCommand helper
public:
	BOOL PostIPCommand(                                    LPCSTR pszSession, LPCSTR pszCommand, DWORD dwTimeout = IP_DEFAULT_TIMEOUT);
	BOOL PostIPCommand(                                    LPCSTR pszSession, LPCSTR pszCommand, const CIPVariable& var, DWORD dwTimeout = IP_DEFAULT_TIMEOUT);
	BOOL PostIPCommand(                                    LPCSTR pszSession, LPCSTR pszCommand, const CIPVariable& var, CIPVariable& varResult, CIPCommandSync& sync, DWORD dwTimeout = IP_DEFAULT_TIMEOUT);
	BOOL SendIPCommand(                                    LPCSTR pszSession, LPCSTR pszCommand, DWORD dwTimeout = INFINITE);
	BOOL SendIPCommand(                                    LPCSTR pszSession, LPCSTR pszCommand, const CIPVariable& var, DWORD dwTimeout = INFINITE);
	BOOL SendIPCommand(                                    LPCSTR pszSession, LPCSTR pszCommand, const CIPVariable& var, CIPVariable& varResult, DWORD dwTimeout = INFINITE);
	BOOL SendIPStepCommand(                                LPCSTR pszSession, LPCSTR pszCommand, DWORD dwTimeout = INFINITE);
	BOOL SendIPStepCommand(                                LPCSTR pszSession, LPCSTR pszCommand, const CIPVariable& var, DWORD dwTimeout = INFINITE);

protected:
	BOOL PostIPCommand(    LPCSTR pszTask,                 LPCSTR pszSession, LPCSTR pszCommand, DWORD dwTimeout = IP_DEFAULT_TIMEOUT);
	BOOL PostIPCommand(    LPCSTR pszTask,                 LPCSTR pszSession, LPCSTR pszCommand, const CIPVariable& var, DWORD dwTimeout = IP_DEFAULT_TIMEOUT);
	BOOL PostIPCommand(    LPCSTR pszTask,                 LPCSTR pszSession, LPCSTR pszCommand, const CIPVariable& var, CIPVariable& varResult, CIPCommandSync& sync, DWORD dwTimeout = IP_DEFAULT_TIMEOUT);
	BOOL SendIPCommand(    LPCSTR pszTask,                 LPCSTR pszSession, LPCSTR pszCommand, DWORD dwTimeout = INFINITE);
	BOOL SendIPCommand(    LPCSTR pszTask,                 LPCSTR pszSession, LPCSTR pszCommand, const CIPVariable& var, DWORD dwTimeout = INFINITE);
	BOOL SendIPCommand(    LPCSTR pszTask,                 LPCSTR pszSession, LPCSTR pszCommand, const CIPVariable& var, CIPVariable& varResult, DWORD dwTimeout = INFINITE);
	BOOL SendIPStepCommand(LPCSTR pszTask,                 LPCSTR pszSession, LPCSTR pszCommand, DWORD dwTimeout = INFINITE);
	BOOL SendIPStepCommand(LPCSTR pszTask,                 LPCSTR pszSession, LPCSTR pszCommand, const CIPVariable& var, DWORD dwTimeout = INFINITE);

	BOOL PostIPCommand(    LPCSTR pszTask, LPCSTR pszUnit, LPCSTR pszSession, LPCSTR pszCommand, DWORD dwTimeout = IP_DEFAULT_TIMEOUT);
	BOOL PostIPCommand(    LPCSTR pszTask, LPCSTR pszUnit, LPCSTR pszSession, LPCSTR pszCommand, const CIPVariable& var, DWORD dwTimeout = IP_DEFAULT_TIMEOUT);
	BOOL PostIPCommand(    LPCSTR pszTask, LPCSTR pszUnit, LPCSTR pszSession, LPCSTR pszCommand, const CIPVariable& var, CIPVariable& varResult, CIPCommandSync& sync, DWORD dwTimeout = IP_DEFAULT_TIMEOUT);
	BOOL SendIPCommand(    LPCSTR pszTask, LPCSTR pszUnit, LPCSTR pszSession, LPCSTR pszCommand, DWORD dwTimeout = INFINITE);
	BOOL SendIPCommand(    LPCSTR pszTask, LPCSTR pszUnit, LPCSTR pszSession, LPCSTR pszCommand, const CIPVariable& var, DWORD dwTimeout = INFINITE);
	BOOL SendIPCommand(    LPCSTR pszTask, LPCSTR pszUnit, LPCSTR pszSession, LPCSTR pszCommand, const CIPVariable& var, CIPVariable& varResult, DWORD dwTimeout = INFINITE);
	BOOL SendIPStepCommand(LPCSTR pszTask, LPCSTR pszUnit, LPCSTR pszSession, LPCSTR pszCommand, DWORD dwTimeout = INFINITE);
	BOOL SendIPStepCommand(LPCSTR pszTask, LPCSTR pszUnit, LPCSTR pszSession, LPCSTR pszCommand, const CIPVariable& var, DWORD dwTimeout = INFINITE);

	HIPCOMMAND FindPendingIPStepCommand(LPCSTR pszSession, LPCSTR pszCommand);
	HIPCOMMAND FindPendingIPStepCommand(LPCSTR pszUnit, LPCSTR pszSession, LPCSTR pszCommand);
	HIPCOMMAND FindPendingIPStepCommand(void* pKey);
	BOOL ResumeIPStepCommand(PIPCOMMAND pCMD, DWORD dwStatusAdd = 0, DWORD dwStatusRemove = 0);
	BOOL ResumeIPStepCommandByName(LPCSTR pszSession, LPCSTR pszCommand, DWORD dwStatusAdd = 0, DWORD dwStatusRemove = 0);
	BOOL ResumeIPStepCommandByName(LPCSTR pszUnit, LPCSTR pszSession, LPCSTR pszCommand, DWORD dwStatusAdd = 0, DWORD dwStatusRemove = 0);
	BOOL ResumeIPStepCommandByKey(void* pKey, DWORD dwStatusAdd = 0, DWORD dwStatusRemove = 0);
	BOOL ResumeIPStepCommandRemote(LPCSTR pszTask, LPCSTR pszUnit, LPCSTR pszSession, LPCSTR pszCommand, LPCSTR pszPendingStep = NULL, DWORD dwStatusAdd = 0, DWORD dwStatusRemove = 0);
	BOOL ContinueIPStepCommand(PIPCOMMAND pCMD, DWORD dwStatusAdd = 0, DWORD dwStatusRemove = 0) {
		return ResumeIPStepCommand(pCMD, dwStatusAdd, dwStatusRemove);
	}
	BOOL ContinueIPStepCommandByName(LPCSTR pszSession, LPCSTR pszCommand, DWORD dwStatusAdd = 0, DWORD dwStatusRemove = 0) {
		return ResumeIPStepCommandByName(pszSession, pszCommand, dwStatusAdd, dwStatusRemove);
	}
	BOOL ContinueIPStepCommandByName(LPCSTR pszUnit, LPCSTR pszSession, LPCSTR pszCommand, DWORD dwStatusAdd = 0, DWORD dwStatusRemove = 0) {
		return ResumeIPStepCommandByName(pszUnit, pszSession, pszCommand, dwStatusAdd, dwStatusRemove);
	}
	BOOL ContinueIPStepCommandByKey(void* pKey, DWORD dwStatusAdd = 0, DWORD dwStatusRemove = 0) {
		return ResumeIPStepCommandByKey(pKey, dwStatusAdd, dwStatusRemove);
	}

	HIPCOMMAND FindReceivedIPCommand(LPCSTR pszSession, LPCSTR pszCommand);
	HIPCOMMAND FindReceivedIPCommand(LPCSTR pszUnit, LPCSTR pszSession, LPCSTR pszCommand);
	HIPCOMMAND FindReceivedIPCommand(void* pKey);

	// CommandTarget INIT/CLOSE
protected:
	ip_handler BOOL OnIP_INIT(CIPCommand& cmd);
	ip_handler BOOL OnIP_CLOSE(CIPCommand& cmd);
};

#define IIPCT IIPCommandTarget


//=============================================================================
// IWNDIPCommandTarget
//

#define DECLARE_IP_INTERFACE()\
	virtual BOOL OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult) {\
		BOOL bReturn = FALSE;\
		BOOL bResult = IP_OnWndMsg(message, wParam, lParam, pResult, bReturn);\
		if (bReturn)\
			return bResult;\
		return __super::OnWndMsg(message, wParam, lParam, pResult);\
	}

#define DECLARE_IP_AND_INTERFACE()\
	DECLARE_IP()\
	DECLARE_IP_INTERFACE()


class AFX_EXT_CLASS_MIP IWNDIPCommandTarget : public IIPCommandTarget {
private:
	HWND& r_hWnd;
public:
	// Default
	//IWNDIPCommandTarget() = delete;
	//IWNDIPCommandTarget(CIPClient* pIPClient = NULL) = delete;
	IWNDIPCommandTarget(HWND& hWnd, CIPClient* pIPClient = NULL) : r_hWnd(hWnd), IIPCommandTarget(pIPClient, FALSE) { }
	virtual ~IWNDIPCommandTarget() { }

protected:
	DECLARE_IP()

private:
	struct __DO_IP_COMMAND {
		HIPCOMMAND rCMD;	// be careful. HIPCOMMAND is TRefPointer
		const IP_COMMAND_TARGET_ENTRY* pEntry;
	};

protected:
	virtual BOOL IP_OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult, BOOL& bReturn);
	virtual BOOL DoIPCommand(HIPCOMMAND rCMD, const IP_COMMAND_TARGET_ENTRY* pEntry);

	// misc
	ip_handler BOOL OnIPShowWindow(CIPCommand& cmd);

};


//=============================================================================
// IIPCommandTarget
//

template < class WNDCLASS >
class TIPCommandTarget : public WNDCLASS, public IWNDIPCommandTarget {
public:
	// Default
	TIPCommandTarget<WNDCLASS>(CIPClient* pIPClient = NULL) : IWNDIPCommandTarget(WNDCLASS::m_hWnd, pIPClient) {}

	template <typename ... A>
	TIPCommandTarget<WNDCLASS>(CIPClient* pIPClient, A... a) : IWNDIPCommandTarget(WNDCLASS::m_hWnd, pIPClient), WNDCLASS(a...) {}

	// or other Constructors which needs WNDCLASS(a, b...) constructors
	//template <typename A>
	//TIPCommandTarget<WNDCLASS>(CIPClient* pIPClient, A a) : IWNDIPCommandTarget(m_hWnd, pIPClient), WNDCLASS(a) {}
	//template <typename A, typename B>
	//TIPCommandTarget<WNDCLASS>(CIPClient* pIPClient, A a, B b) : IWNDIPCommandTarget(m_hWnd, pIPClient), WNDCLASS(a, b) {}
	//template <typename A, typename B, typename C>
	//TIPCommandTarget<WNDCLASS>(CIPClient* pIPClient, A a, B b, C c) : IWNDIPCommandTarget(m_hWnd, pIPClient), WNDCLASS(a, b, c) {}
	//template <typename A, typename B, typename C, typename D>
	//TIPCommandTarget<WNDCLASS>(CIPClient* pIPClient, A a, B b, C c, D d) : IWNDIPCommandTarget(m_hWnd, pIPClient), WNDCLASS(a, b, c, d) {}
	//template <typename A, typename B, typename C, typename D, typename E>
	//TIPCommandTarget<WNDCLASS>(CIPClient* pIPClient, A a, B b, C c, D d, E e) : IWNDIPCommandTarget(m_hWnd, pIPClient), WNDCLASS(a, b, c, d, e) {}
	//template <typename A, typename B, typename C, typename D, typename E, typename F>
	//TIPCommandTarget<WNDCLASS>(CIPClient* pIPClient, A a, B b, C c, D d, E e, F f) : IWNDIPCommandTarget(m_hWnd, pIPClient), WNDCLASS(a, b, c, d, e, f) {}

	virtual ~TIPCommandTarget<WNDCLASS>() { }

	DECLARE_IP_INTERFACE()

};

#define TIPCT TIPCommandTarget



#pragma pack(pop)
