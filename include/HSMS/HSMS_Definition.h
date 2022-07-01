#pragma once

#pragma pack(push, 8)

//===========================================================================
enum {
	HSMS_MAX_PACKET_SIZE = 32*1024*1024,		// 32 MBytes, not from spec.
};

enum eHSMS_TIMEOUT {
	HT_NONE = -1,
	HT_T1_InterCharacter = 0,		// T1 : Inter-Character Timeout (SECS1 only)
	HT_T2_BlockProtocol,			// T2 : Block-Protocol Timeout (SECS1 only)
	HT_T3_Reply,					// T3 : Reply Timeout                          (45 sec, 1~120 sec)
	HT_T4_InterBlock,				// T4 : Inter-Block Timeout (SECS1 only)
	HT_T5_ConnectSeperation,		// T5 : Connect Separation Timeout             (10 sec, 1~240 sec)
	HT_T6_ControlTransaction,		// T6 : Control Transaction Timeout            ( 5 sec, 1~240 sec)
	HT_T7_NotSelected,				// T7 : NOT SELECTED Timeout                   (10 sec, 1~240 sec)
	HT_T8_NetworkInterCharacter,	// T8 : Network Intercharacter Timeout         ( 5 sec, 1~120 sec), N/A
	nHT
};

enum eHSMS_SESSION_TYPE {
	HSMS_ST_DATA = 0,

	HSMS_ST_SELECT_REQ,			// 1
	HSMS_ST_SELECT_RSP,			// 2

	HSMS_ST_DESELECT_REQ,		// 3
	HSMS_ST_DESELECT_RSP,		// 4

	HSMS_ST_LINKTEST_REQ,		// 5
	HSMS_ST_LINKTEST_RSP,		// 6

	HSMS_ST_REJECT_REQ,			// 7
	HSMS_ST_DUMMY1,

	HSMS_ST_SEPERATE_REQ,		// 9
	HSMS_ST_DUMMY2,
};

enum eHSMS_SELECT_STATUS {
	HSMS_SS_OK = 0,
	HSMS_SS_ALREADY_SELECTED,
	HSMS_SS_NOT_READY,
	HSMS_SS_ON_DISCONNECTING,
};

enum eHSMS_DESELECT_STATUS {
	HSMS_DS_OK = 0,
	HSMS_DS_ALREADY_DESELECTED,
	HSMS_DS_NOT_READY,
};

enum eHSMS_EVENT {
	HSMS_E_NONE = 0,

	HSMS_E_CONNECTED = 1000,
	HSMS_E_DISCONNECTED,

	HSMS_E_SELECT_SESSION,
	HSMS_E_SELECT_SESSION_FAILED,
	HSMS_E_DESELECT_SESSION,
	HSMS_E_DESELECT_SESSION_FAILED,

	HSMS_E_TRANSACTION_RECEIVED,
	HSMS_E_TRANSACTION_REPLIED,
	HSMS_E_TRANSACTION_CANCELED,

	HSMS_E_TRANSACTION_ABORT,					// SxF0
	HSMS_E_TRANSACTION_NO_DEVICEID,				// S9F1
	HSMS_E_TRANSACTION_NO_STREAM,				// S9F3
	HSMS_E_TRANSACTION_NO_FUNCTION,				// S9F5
	HSMS_E_TRANSACTION_NO_STRUCTURE,			// S9F7
	HSMS_E_TRANSACTION_TIMEDOUT,				// S9F9, T3
	HSMS_E_TRANSACTION_DATA_TOO_LONG,			// S9F11
	HSMS_E_TRANSACTION_CONVERSATION_TIMEDOUT,	// S9F13	struct { ASCII MEXP[6]; ASCII EDID[14]; };
	HSMS_E_TRANSACTION_NOT_FOUND,

};

enum eHSMS_DIRECTION {
	HSMS_D_NONE,
	HSMS_D_EQP_REQ,			// EQP->HOST (EQP started)
	HSMS_D_EQP_RSP,			// EQP->HOST (HOST started)
	HSMS_D_HOST_REQ,		// HOST->EQP (HOST started)
	HSMS_D_HOST_RSP,		// HOST->EQP (EQP started)
	HSMS_D_EQP,
	HSMS_D_HOST,
	nHSMS_D,
};

enum eHSMS_MISC {
	HSMS_M_DEFAULT_SESSION_ID = (unsigned short)-1,
};

enum eHSMS_TRANSACTION_SENDER { 
	HSMS_TS_BOTH,
	HSMS_TS_HOST,
	HSMS_TS_DEVICE,
};

#define __HSMS_CHILD_FLAG__
#ifdef __HSMS_CHILD_FLAG__
#	define __HSMS_CHILD__ [0]
#else
#	define __HSMS_CHILD__
#endif

#pragma pack(pop)
