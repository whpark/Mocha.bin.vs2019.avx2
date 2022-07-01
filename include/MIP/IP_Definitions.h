#pragma once

#include "AFX_EXT_MIP.h"

#ifdef _M_X64
#	define _IP_COMPATIBILITY_W32 0
#endif

typedef uint32_t IPDATA_UINT32;
typedef int32_t  IPDATA_INT32;
typedef uint64_t IPDATA_UINT64;
typedef int64_t  IPDATA_INT64;

#ifndef BIT
	#define BIT(x) (0x01 << x)
#endif

#pragma pack(push, 8)

enum eINTER_PROCESS_LOG_TYPE {
	IPLT_NORMAL = BIT(0),
	IPLT_COMMAND_INOUT = BIT(30),
	IPLT_ERROR = BIT(31),
};

#define IP_LEN_SIGNATURE 4

#define IP_MAX_CMD_PACKET_SIZE (256UL*1024*1024)	// 64 MBytes (2015.07.30   16 -> 256 MByte)

#define IP_DEFAULT_SERVER_IP	_T("127.0.0.1")
#define IP_DEFAULT_SERVER_PORT	6530

#define IP_DATA_HEADER_ITEM_LEN 256

#define IP_DEFAULT_TIMEOUT (10*1000)

#if _M_64_
#	define IP_MAX_IPDATA_SIZE (8ull*1024*1024*1024)
#else
#	define IP_MAX_IPDATA_SIZE (1*1024*1024*1024)
#endif
#define IP_MAX_IPDATA_ITEM_SIZE (1*1024*1024*1024)	// 1 GBytes 넘기면 안됨. 소스코드 확인

enum eWM_IP {
	WM_IP_COMMAND_MESSAGE = WM_APP+0x30F0,	// 0x8000 + 0x30F0 => 0xB0F0
	WM_IP_RESTART,
	WM_IP_CONNECTED,
	WM_IP_DISCONNECTED,
};

enum eIP_EXCEPTION {
	IPE_UNKNOWN = 0xf0000000,
	IPE_INTERNAL,
	IPE_NO_TARGET,
	IPE_NO_TARGET_FUNCTION,
	IPE_RESET_STEP,
	IPE_VARIABLE_MISMATCH,
};


enum eIP_PACKET_TYPE {
	fIPPT_MASK				= 0x00ffffff,
	// TASK
	fIPPT_TASK				= 0x000000ff,
	IPPT_TASK_OPEN			= BIT( 0),
	IPPT_TASK_CLOSE			= BIT( 1),
	IPPT_TASK_COMMAND		= BIT( 2),
	IPPT_TASK_RESET_STEP	= BIT( 7),
	// COMMAND
	IPPT_COMMAND			= BIT( 8),
	// DATA
	IPPT_OPEN_DATA_PORT		= BIT(16),

	fIPPT_REPLY_MASK		= ~fIPPT_MASK,
	IPPT_REPLY				= BIT( 0)<<24,
	IPPT_ACK				= BIT( 1)<<24,
};

enum eIP_PACKET_STATUS {
	IPPS_OK				= BIT( 0),
	IPPS_STARTED		= BIT( 1),	// Command Dispatched. (Entered to IIPCommandTarget::DoIPCommand)
	IPPS_RESERVE_ERROR	= BIT( 2),

	fIPPS_STEP			= 0x0000ff00,
	IPPS_STEP			= BIT( 8),
	IPPS_DELAYED_STEP	= BIT( 9),
	IPPS_TIME_ARRIVED	= BIT(10),
	IPPS_RESET_STEP		= BIT(11),

	fIPPS_ERROR			= 0xffff0000,
	IPPS_NO_TASK		= BIT(16),	// There are No TASK
	IPPS_NO_SESSION		= BIT(17),	// There are No Session
	IPPS_NO_COMMAND		= BIT(18),	// There are No Command
	IPPS_BUSY			= BIT(19),	// Function was already in running
	IPPS_COMMAND_FAILED	= BIT(20),	// Function Returned FALSE
	IPPS_TIMEDOUT		= BIT(21),
	IPPS_CHILD_FAILED	= BIT(22),
	IPPS_CANCELED		= BIT(23),	// IPPS_RESET_STEP -> IPPS_CANCELED
	IPPS_CANCEL			= BIT(24),	// Will be canceled at IIPCommandTarget::OnPreIPCommand()
	IPPS_STOP			= BIT(25),	// No Error, but Just STOP.

	IPPS_UNKNOWN		= BIT(31),
};

struct IP_PACKET_HEADER {
	DWORD eType;							// Packet Type
	DWORD dwIndex;							// Index (per client)
	DWORD fStatus;							// Packet Status
	DWORD dwSizeDataPacket;					// Data Packet Size 
											//     (exclude signature, and size-variable itself. packet size must be smaller than or equal to IP_MAX_VARIABLE_SIZE)
	char szTaskSender[64];
	char szTaskTarget[64];
};

enum eIP_COMMAND_MESSAGE_RESULT {
	IPCMR_NONE = -100,
	IPCMR_UNKNOWN,
	IPCMR_NOT_FOUND,
	IPCMR_BUSY,
	IPCMR_DISCARDED,
	IPCMR_OK = 0,				// Command Handler Function Worked.
	IPCMR_QUEUED,				// Command Queued
};

enum eIP_VARIABLE_TYPE {
	IPVT_NONE = 0,
	IPVT_INT32,
	IPVT_INT64,
	IPVT_UINT32,
	IPVT_UINT64,
	IPVT_DOUBLE,
	IPVT_STRING,
	IPVT_WSTRING,
	IPVT_BINARY,
	nIPVT,

#if defined(_M_64_) && (_X64_FORCE_INT64_NATIVE_)
	IPVT_INT = IPVT_INT64,
	IPVT_UINT = IPVT_UINT64,
#else
	IPVT_INT = IPVT_INT32,
	IPVT_UINT = IPVT_UINT32,
#endif
};

enum eIP_DATA_TYPE {
	IPDT_NONE = 0,
	IPDT_INT8,  IPDT_INT16,  IPDT_INT32,  IPDT_INT64,
	IPDT_UINT8, IPDT_UINT16, IPDT_UINT32, IPDT_UINT64,
	IPDT_WCHAR,
	IPDT_FLOAT, IPDT_DOUBLE,
	IPDT_STRUCT,
	IPDT_UNION,
	IPDT_ARRAY,
};

enum eIP_DATA_COMMAND {
	IPDC_NONE,
	IPDC_GET_STRUCT,
	IPDC_GET_VAR,
	IPDC_SET_VAR,
	IPDC_LOCK_VAR,
	IPDC_UNLOCK_VAR,
};

//---------------------------------------------------------------------------------------------------------------------
#define DECLARE_IP() \
	virtual const IP_COMMAND_TARGET* GetIPCommandTargetStatic();

#define BEGIN_IP_COMMAND_TABLE(theClass, parentClass)	\
	const IP_COMMAND_TARGET* theClass::GetIPCommandTargetStatic() {\
		static const IP_COMMAND_TARGET* pTargetParent = parentClass::GetIPCommandTargetStatic();\
		static const IP_COMMAND_TARGET_ENTRY_FUNC cmdTargetEntrys[] = {\
			__pragma(warning(push)) __pragma(warning(disable:4867))\

#define END_IP_COMMAND_TABLE()\
			__pragma(warning(pop))\
			{ (LPCSTR)NULL, (LPCSTR)NULL, NULL, FALSE, FALSE, }\
		};\
		static IP_COMMAND_TARGET ipCommandTarget = { cmdTargetEntrys, pTargetParent };\
		return &ipCommandTarget;\
	}\

#define ON_IP(szSession, szCommand, Func) \
		{ szSession, szCommand, (IPCommandFunction)Func, FALSE, FALSE, TRUE, NULL, },

#define ON_IP_EXCL(szSession, szCommand, Func) \
		{ szSession, szCommand, (IPCommandFunction)Func, TRUE, FALSE, TRUE, NULL, },

#define ON_IP_QUEUE(szSession, szCommand, Func) \
		{ szSession, szCommand, (IPCommandFunction)Func, FALSE, TRUE, TRUE, NULL, },

#define ON_IP_LOG(szSession, szCommand, Func, bLog, pszLogPath) \
		{ szSession, szCommand, (IPCommandFunction)Func, FALSE, FALSE, bLog, pszLogPath, },

#define ON_IP_EXCL_LOG(szSession, szCommand, Func, bLog, pszLogPath) \
		{ szSession, szCommand, (IPCommandFunction)Func, TRUE, FALSE, bLog, pszLogPath, },

#define ON_IP_QUEUE_LOG(szSession, szCommand, Func, bLog, pszLogPath) \
		{ szSession, szCommand, (IPCommandFunction)Func, FALSE, TRUE, bLog, pszLogPath, },

#define ON_IP_FULL(szSession, szCommand, Func, bExclusive, bQueue, bLog, pszLogPath) \
		{ szSession, szCommand, (IPCommandFunction)Func, bExclusive, bQueue, bLog, pszLogPath, },

		// Session,   Command,   Function,                Exclusive, Interceptor, LIST_CMD(), LIST_CMD()


//---------------------------------------------------------------------------------------------------------------------

//#define IP_LOCAL_VAR_1_START()				struct LOCAL_VAR : IIPLocalVar
//#define IP_LOCAL_VAR_2_ONE_TIME_INIT()		& l = cmd.LocalVar<LOCAL_VAR>(NULL, [&] (LOCAL_VAR& l)
//#define IP_LOCAL_VAR_3_END()				);
#define IP_LOCAL_VAR_1_START()\
	struct LOCAL_VAR : IIPLocalVar

#define IP_LOCAL_VAR_2_END()\
	LOCAL_VAR; \
	LOCAL_VAR & l = cmd.LocalVar<LOCAL_VAR>();

#define IP_LOCAL_VAR_2_ONE_TIME_INIT()\
	LOCAL_VAR; \
	BOOL bLocalVarCreated = FALSE;\
	LOCAL_VAR & l = cmd.LocalVar<LOCAL_VAR>(&bLocalVarCreated);\
	if (bLocalVarCreated)

//---------------------------------------------------------------------------------------------------------------------

class CSimpleLog;
class AFX_EXT_CLASS_MIP CIPException : public __ExceptionBase__ {
protected:
	eIP_EXCEPTION m_eCause;

public:
	CIPException(const CIPException& B) : __ExceptionBase__("", "") { *this = B; }
	CIPException(eIP_EXCEPTION eCause, LPCTSTR pszFunctionName = NULL, LPCTSTR pszReason = NULL) : m_eCause(eCause), __ExceptionBase__(pszFunctionName, pszReason) { }
	~CIPException() {
	}

public:
	void ReportError() {
		AfxMessageBox(GetErrorString(), MB_ICONERROR);
	}
	CString GetErrorString() const {
		CString str;
		str.Format(_T("-- ERROR ! at %s \r\n\r\n-- Reason(x0%08x) : %s"), (LPCTSTR)m_strFunctionName, (unsigned int)m_eCause, (LPCTSTR)m_strReason);
		return str;
	}
	eIP_EXCEPTION GetCause() { return m_eCause; }

};


//---------------------------------------------------------------------------------------------------------------------
#define IP_DEFAULT_MAP_FILE_PREFIX	"Mocha"

#define IP_TASK_SERVER				"IP_SERVER"

#define IP_UNIT_FRAMEWORK			NULL
#define IP_SESSION_FRAMEWORK		"IP_SESSION_FRAMEWORK"

#define IP_SF_QUIT					"IP_SF_QUIT"
#define IP_SF_RESTART				"IP_SF_RESTART"
#define IP_SF_RESTART_DEAD			"IP_SF_RESTART_DEAD"
#define IP_SF_RESET_ALL_SEQUENCE	"IP_SF_RESET_ALL_SEQUENCE"
#define IP_SF_REGISTER_TASK			"IP_SF_REGISTER_TASK"
#define IP_SF_UNREGISTER_TASK		"IP_SF_UNREGISTER_TASK"
#define IP_SF_COMMAND_INIT_TARGET	"IP_SF_INIT_TARGET"
#define IP_SF_COMMAND_CLOSE_TARGET	"IP_SF_CLOSE_TARGET"
#define IP_SF_GET_DATA_LIST			"IP_SF_GET_DATA_LIST"
#define IP_SF_RESUME_STEP_COMMAND	"IP_SF_RESUME_STEP_COMMAND"

//---------------------------------------------------------------------------------------------------------------------

#pragma pack(pop)
