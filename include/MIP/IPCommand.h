
#pragma once

#include <afxmt.h>

#include "AFX_EXT_MIP.h"

#include <map>

#include "TList.hpp"
#include "misc/xUtil.h"
#include "misc/SimpleLog.h"
#include "IP_Definitions.h"
#include "IPVariable.h"

#pragma pack(push, 8)

WARNING_PUSH_AND_DISABLE(4244)

class IIPTask;
class IIPCommandTarget;
class CIPCommandPipe;
class CIPCommandPipeClient;
class CIPCommandPipeServer;

class CIPCommand;
class CIPCommandSync;
class CIPServer;
class CIPClient;

class IIPLocalVar {
public:
	IIPLocalVar(){}
	virtual ~IIPLocalVar() {}
};

//=============================================================================
// CIPCommand
//

class AFX_EXT_CLASS_MIP __declspec(novtable) CIPStep {
protected:
	int iStep;

public:
	CIPStep(int _iStep = -1) : iStep(_iStep) {}

	int GetStep() const { return iStep; }
};

class AFX_EXT_CLASS_MIP CIPCommand : public CSimpleLogInterface {
	friend class IIPTask;
	friend class IIPCommandTarget;
	friend class CIPCommandPipe;
	friend class CIPCommandPipeClient;
	friend class CIPCommandPipeServer;
	friend class CIPCommandSync;
	friend class CIPClient;
	friend class CIPServer;
	friend class TRefPointer<CIPCommand>;
	friend class TConstRefPointer<CIPCommand>;

private:
#pragma pack(push, 8)	// ensure word align.
#if (_MSC_VER >= _MSC_VER_VS2005)
	static std::atomic<LONG> s_dwIndex;
#else
	static LONG s_dwIndex;
#endif
#pragma
protected:
	HANDLE m_hEvtQueued;
	HANDLE m_hEvtReleased;

protected:
	DWORD m_eType;			// eIP_PACKET_TYPE
	DWORD m_dwIndex;		// Unique ID
	DWORD m_fStatus;		// eIP_PACKET_STATUS

	DStringA m_strTaskSender;
	DStringA m_strTaskTarget;
	DStringA m_strUnitSender;
	DStringA m_strUnitTarget;
	DStringA m_strSession;
	DStringA m_strCommand;

	IIPCommandTarget* m_pTargetSender;

public:
	CIPVariable m_var;
	CIPVariable m_varResult;

	// Constructors & Destructors
private:
	// Constructor for Sending a Command
	CIPCommand();
	virtual ~CIPCommand();

	BOOL operator == (DWORD dwIndex) const { return m_dwIndex == dwIndex; }

private:
	CIPCommand(const CIPCommand& B) { ASSERT(FALSE); }
	CIPCommand& operator = (const CIPCommand& B) { ASSERT(FALSE); return *this; }

public:
	DWORD GetType() const;
	DWORD GetStatus() const;
	BOOL IsTimedout() const;
	BOOL IsOK() const;
	BOOL ResetErrorAndChild();
	void ReserveError();

	const DStringA& GetTaskSender() const { return m_strTaskSender; }
	const DStringA& GetTaskTarget() const { return m_strTaskTarget; }
	const DStringA& GetUnitSender() const { return m_strUnitSender; }
	const DStringA& GetUnitTarget() const { return m_strUnitTarget; }
	const DStringA& GetUnitName() const { return m_strUnitTarget; }
	const DStringA& GetSession() const { return m_strSession; }
	const DStringA& GetCommand() const { return m_strCommand; }

protected:
	BOOL SetFromPacket(const IP_PACKET_HEADER& header, const void* pData, BOOL bRaw = FALSE);
	BOOL SetToPacket(IP_PACKET_HEADER& header, CMemFile& f, DWORD eReplyType) const;

public:
	CArchive& StoreTo(CArchive& ar) const;
	CArchive& LoadFrom(CArchive& ar);
	CArchive& StoreResultTo(CArchive& ar) const;
	CArchive& LoadResultFrom(CArchive& ar);

	// Step Command
protected:
	int m_iStep;
	int m_iStepPrevious;
	int m_iStepNext;
	struct T_TIMEOUT {	// Timeout
		DWORD dwTickStart;
		DWORD dwTimeoutAsChild;

		DWORD dwTickLastStep;
		DWORD dwTimeoutStep;
		BOOL bUpdatedTickChildFinished;
		DWORD dwTickChildFinished;	// Child Command 가 종료된 시각
		BOOL bAsError;
	} m_timeout;
	struct T_KEY {	// Key for Pause/Resume Step
		TRefPointer<CIPCommand> rCMDParent;
		mutable void* pKey;

		mutable CCriticalSection csChildIPCommand;
		TRefList<CIPCommand> cmdsPending;
		TRefList<CIPCommand> cmdsFinished;
		CStringA strPendingStep;
	} m_key;

	// Step
public:
	int GetStep() const;			// 현재 수행되고 있는 Step Index. 함수 전체에서 변경 되지 않음.
	int GetCurrentStep() const;		// 현재 수행되고 있는 Step Index. 함수 전체에서 변경 되지 않음.
	int GetPreviousStep() const;	// 이전 Step Index. (-1 : 이전 스텝이 없음. --> 현재가 첫번째 스텝임)
	int GetNextStep() const;		// 다음에 진행할 Step Index. (-1)일 경우, 현재 스텝에서 1 증가 된 스텝이 다음 스텝이 됨. (-2) 일 경우, 스텝 강제 종료 됨.

	// Discard Step : 
	void DiscardStep();	// 다음 스텝을 진행하지 않음.

	// PauseStep : 일정 시간 대기후, 다음 스텝시작. INFINIT (-1, 0xffffffff) 일 경우, 무기한 대기.
	//             Child step 이 있을 경우에는 모두 종료된 후, 추가로 dwTimeinterval 대기 한 후 다음 시작
	int PauseStep(DWORD dwTimeInterval, CIPStep iStepNext = -1, void* pKey = NULL, LPCSTR pszPendingCondition = NULL);

	// Wait Step : 일정 시간 대기후, Child Step 이 종료되든 종료 되지 않았든, 정해진 시간 후에는 다음 스텝 시작
	//             외부에서 Continue 시켜주지 않고, timeout 으로 다음 스텝이 시작될 경우 GetStatus()에 IPPS_TIMEDOUT이 추가 됨.
	int WaitStep(DWORD dwTimeout, CIPStep iStepNext = -1, void* pKey = NULL, LPCSTR pszPendingCondition = NULL);				// with timeout error

	// 다음 Step을 세팅함. ReserveError() 호출 하고 EnqueNextStep() 호출 하는 것과 동일함. 들어올 때 Error 가짐. Child Step 이 있을경우에는, 모두 종료할 때까지 기다렸다가, 종료되면 다음 스텝 시작.
	int ReserveErrorStep(CIPStep iStepNext = -1, DWORD dwTimeInterval = 0, void* pKey = NULL, LPCSTR pszPendingCondition = NULL);

	// 다음 Step을 세팅함. Child Step 이 있을경우에는, 모두 종료할 때까지 기다렸다가, 종료되면 다음 스텝 시작.
	int EnqueueNextStep(CIPStep iStepNext = -1, DWORD dwTimeInterval = 0, void* pKey = NULL, LPCSTR pszPendingCondition = NULL);

	// 다음 Step을 현재 Step 으로 세팅함. ( by 이범호 )
	int EnqueueNextStepCurrent(DWORD dwTimeInterval = 10, void* pKey = NULL, LPCSTR pszPendingCondition = NULL);

	// (bTimeoutAsError == TRUE) 일 경우, EnqueueNextStep() 과 동일한 동작.
	// (bTimeoutAsError != TRUE) 일 경우, WaitStep() 과 동일한 동작.
	int EnqueueStep(DWORD dwTimeInterval = 0, CIPStep iStepNext = -1, void* pKey = NULL, BOOL bTimeoutAsError = FALSE, LPCSTR pszPendingCondition = NULL);

	//// 되도록이면 쓰지 마세요.
	//int ProceedStep(int iIncrement = 1) { m_iStepNext = m_iStep + iIncrement; return m_iStep; }

	// Child Step 의 결과 값을 확인.
	BOOL IsChildOK();
	void* GetKey() const { return m_key.pKey; }
protected:
	int SetNextStepImpl(DWORD dwTimeout, DWORD fStatus, CIPStep iStepNext, void* pKey, BOOL bTimeoutAsError, LPCSTR pszPendingCondition);
public:
	TRefList<CIPCommand>& GetChildIPCommandsPending() { return m_key.cmdsPending; }
	TRefList<CIPCommand>& GetChildIPCommands() { return m_key.cmdsFinished; }
	const CIPCommand& GetChildIPCommand(LPCSTR pszTask, LPCSTR pszSession, LPCSTR pszCommand) const;
	const CIPCommand& GetChildIPCommand(LPCSTR pszTask, LPCSTR pszUnit, LPCSTR pszSession, LPCSTR pszCommand) const;
	const CIPVariable& GetChildResult(LPCSTR pszTask, LPCSTR pszSession, LPCSTR pszCommand) const;
	const CIPVariable& GetChildResult(LPCSTR pszTask, LPCSTR pszUnit, LPCSTR pszSession, LPCSTR pszCommand) const;

	// Cookies
	uint64_t m_dwCookie;	// DWORD -> uintptr_t -> uint64_t
	void* m_pCookie;
	TBuffer<BYTE> m_cookie;
	CStringA m_strCookieA;
	CStringW m_strCookieW;
	CString m_strCookie;
	CIPVariable m_varCookie;
	// Cookie for Local Var
	template<class T>
	T& LocalVarCookie(LPCSTR pszName, const T& rDefaultValue) {
		return m_varCookie.ChildLValue<T>(pszName, rDefaultValue);
	}

	// Local Variable
	std::map<CStringA, TRefPointer<IIPLocalVar> > m_mapLocalVar;
	template<class T, class CInitializer> T& LocalVar(LPCSTR pszName, CInitializer initializer, BOOL* pbCreated = NULL) {
		auto& rVar = m_mapLocalVar[pszName];
		if (rVar.IsEmpty()) {
			rVar = dynamic_cast<IIPLocalVar*>(new T());
			initializer(*(T*)(IIPLocalVar*)rVar);
			if (pbCreated)
				*pbCreated = TRUE;
		} else {
			if (pbCreated)
				*pbCreated = FALSE;
		}

		T* p = dynamic_cast<T*>((IIPLocalVar*)rVar);
		//T* p = (T*)(IIPLocalVar*)rVar;
		if (!p)
			__throw_exception__(Format("Local Variable(%s) Type Mismatch...", pszName ? pszName : "@default"));
		return *p;
	}
	template<class T> T& LocalVar(LPCSTR pszName, BOOL* pbCreated = NULL) { return LocalVar<T>(pszName, [](T& l){}, pbCreated); }
	template<class T> T& LocalVar(BOOL* pbCreated = NULL) { return LocalVar<T>(NULL, pbCreated); }

	// Log
protected:
	int m_nDepth;
	int m_nAdditionalIndent;
	mutable IIPTask* m_pTask;
	virtual BOOL LogV(DWORD fType, LPCSTR pszFormat, va_list args, int nIndent = -1) const;
	virtual BOOL LogV(DWORD fType, LPCWSTR pszFormat, va_list args, int nIndent = -1) const;
public:
	int GetDepth() const { return m_nDepth; }
};


//typedef TRefPointer<CIPCommand> HIPCOMMAND;
class HIPCOMMAND : public TRefPointer<CIPCommand> {
public:
	// 일반 포인터를 HIPCOMMAND 에 할당하는 것을 막기 위해서....(ex, HIPCOMMAND(&cmd) -> 메모리 오류 발생함)
	HIPCOMMAND() : TRefPointer<CIPCommand>(NULL) { }
	HIPCOMMAND(const TRefPointer<CIPCommand>& rCMD) : TRefPointer<CIPCommand>(rCMD) { }
	HIPCOMMAND(const HIPCOMMAND& B) : TRefPointer<CIPCommand>(B) { }
	HIPCOMMAND& operator = (const HIPCOMMAND& B) { (TRefPointer<CIPCommand>&)*this = (TRefPointer<CIPCommand>&)B; return *this; }
	HIPCOMMAND(CIPCommand& cmd) : TRefPointer<CIPCommand>(NULL) { Set(&cmd, false, false); }
	HIPCOMMAND& operator = (CIPCommand& cmd) { Set(&cmd, false, false); return *this; }

private:
	// 아래와 같이 사용하면 안됨.
	HIPCOMMAND(const CIPCommand* pCMD) {}

public:
	BOOL Create() { Assign(); return TRUE; }
};
class HCIPCOMMAND : public TConstRefPointer<CIPCommand> {
public:
	// 일반 포인터를 HIPCOMMAND 에 할당하는 것을 막기 위해서....(ex, HIPCOMMAND(&cmd) -> 메모리 오류 발생함)
	HCIPCOMMAND() : TConstRefPointer<CIPCommand>(NULL) { }
	HCIPCOMMAND(const TRefPointer<CIPCommand>& rCMD) : TConstRefPointer<CIPCommand>(rCMD) { }
	HCIPCOMMAND(const TConstRefPointer<CIPCommand>& rCMD) : TConstRefPointer<CIPCommand>(rCMD) { }
	HCIPCOMMAND(const HIPCOMMAND& B) : TConstRefPointer<CIPCommand>(B) { }
	HCIPCOMMAND(const HCIPCOMMAND& B) : TConstRefPointer<CIPCommand>(B) { }
	HCIPCOMMAND& operator = (const HIPCOMMAND& B) { (TConstRefPointer<CIPCommand>&)*this = (TRefPointer<CIPCommand>&)B; return *this; }
	HCIPCOMMAND& operator = (const HCIPCOMMAND& B) { (TConstRefPointer<CIPCommand>&)*this = (TRefPointer<CIPCommand>&)B; return *this; }
	HCIPCOMMAND(const CIPCommand& cmd) : TConstRefPointer<CIPCommand>(NULL) { Set(&cmd, false, false); }
	HCIPCOMMAND& operator = (const CIPCommand& cmd) { Set(&cmd, false, false); return *this; }
private:
	// 아래와 같이 사용하면 안됨.
	HCIPCOMMAND(const CIPCommand* pCMD) {}
};
typedef CIPCommand* PIPCOMMAND;
typedef const CIPCommand* PCIPCOMMAND;


//=============================================================================
// CIPCommandSync
//
class AFX_EXT_CLASS_MIP CIPCommandSync {
protected:
	class AFX_EXT_CLASS_MIP CIPCommandVariablePair {
	public:
		HIPCOMMAND m_rCMD;
		CIPVariable* m_pVarResult;
	public:
		CIPCommandVariablePair(HIPCOMMAND rCMD, CIPVariable* pVarResult = NULL);
		CIPCommandVariablePair(const CIPCommandVariablePair& B);
		~CIPCommandVariablePair();
	};
protected:
	HWND m_hWnd;
	TList<CIPCommandVariablePair> m_cmdsPending;
	TList<CIPCommandVariablePair> m_cmdsFailed;
	TList<CIPCommandVariablePair> m_cmdsDone;
public:
	CIPCommandSync() { m_hWnd = NULL; }
	CIPCommandSync(HWND hWnd) { m_hWnd = hWnd; }
	CIPCommandSync(CWnd* pWnd) { if (pWnd) m_hWnd = pWnd->m_hWnd; else m_hWnd = NULL; }

	CIPCommandSync(const CIPCommandSync& B) { *this = B; }
	CIPCommandSync& operator = (const CIPCommandSync& B);

public:
	intXX_t GetPendingCount() { return m_cmdsPending.N(); }
	BOOL AddIPCommand(HIPCOMMAND rCMD, CIPVariable* pVarResult);
	BOOL Wait(DWORD dwTimeout = INFINITE);
	BOOL WaitQueued(DWORD dwTimeout = IP_DEFAULT_TIMEOUT);

};


//=============================================================================
// 

WARNING_POP()

#pragma pack(pop)
