#pragma once

#pragma pack(push, 4)

#include "Mocha.type.hxx"

#ifndef _IP_DATA_INTERNAL_
#	define ERROR3
#	define ERROR4
#	define ERROR5
#	define ERROR6
#	define ERROR7
#endif

#ifdef _IP_DATA_INTERNAL_
#	ifdef _IP_DATA_INTERNAL_
#	else
//		ERROR2
#	endif
#else
	ERROR3
#	ifdef _IP_DATA_INTERNAL_
		ERROR4
#	else
		ERROR5
#	endif
#endif

#define MOCHA_MAX_PROCESS 100

enum eTRIGGER {
	TRIGGER_0,
	TRIGGER_1,
	TRIGGER_2,
};

typedef struct {
	char		szVersion[128] = "version";
	BOOL		bInited = 2;
	int			nProcess = 1;
	double		dValue = 3.22;
} TEST_MI;
//TEST_MI test;

//struct _TAG_NAME2 {
//	char		szVersion[128] = "version";
//	BOOL		bInited = 2;
//	int			nProcess =1;
//} T_MOCHA_INFO2[4];
typedef struct {
	char		szVersion[128] = "version";
	BOOL		bInited = 2;
	int			nProcess =1;
} _TAG_NAME2;
//_TAG_NAME2 T_MOCHA_INFO2[4];

typedef struct _TAG_NAME {
	char		szVersion[128];
	BOOL		bInited;
	int			nProcess;
#ifndef _IP_DATA_INTERNAL_
	_TAG_NAME() {

	}
#endif
} T_MOCHA_INFO;

typedef struct {
	char		szProcessName[128];
	char		szProcessPath[512];
	char		szCmdLine[1024];
	BOOL		bConnected;
	double		tConnected;		// Time
	double		tDisconnected;	// Time
	BOOL		bStartedByFramework;
} T_MOCHA_PROCESS_INFO;

#define _DEBUG_IP_DATA_
#ifdef _DEBUG_IP_DATA_
typedef struct {
	int a;
	char b;
	double c;
	wchar_t szData[300];
	int a1:1;
	int a2:1;
	int a3:1;
} T_TESTA;
typedef struct {
	T_TESTA st;
	T_TESTA stA[10];
	T_TESTA stB;
	union {
		T_TESTA b;
		BYTE buffer[128];
		BYTE buffer2[2][3][4];
	} un;
} T_TEST;

//=================================================

#define MAX_CELL	50
#define MAX_LAYERS	50
typedef enum {	// order of Align move
	eAP_LB,
	eAP_LT,
	eAP_RT,
	eAP_RB,
	eAP_END,
} eALIGN_POSIION;

typedef enum {
	eTABLE_MASTER = 0,
	eTABLE_SLAVE,
	eTABLE_END,
} eTABLE;

typedef enum {
	eSCANNER_1,
	eSCANNER_2,
	eSCANNER_3,
	eSCANNER_4,
	eSCANNER_END,
} eSCANNNER;

typedef enum  {
	eMOTOR_NONE	= 0,

	eMOTOR_1	= 1,
	eMOTOR_2,
	eMOTOR_3,
	eMOTOR_4,
	eMOTOR_5,
	eMOTOR_6,
	eMOTOR_7,
	eMOTOR_8,
	eMOTOR_9,
	eMOTOR_10,
	eMOTOR_END,

	eAXIS_X1 = eMOTOR_1,
	eAXIS_Y1 = eMOTOR_2,
	eAXIS_Z1 = eMOTOR_3,
	eAXIS_VX1 = eMOTOR_4,

	eAXIS_X2 = eMOTOR_5,
	eAXIS_Y2 = eMOTOR_6,
	eAXIS_Z2 = eMOTOR_7,
	eAXIS_VX2 = eMOTOR_8,

	eAXIS_VZ1 = eMOTOR_9,
	eAXIS_VZ2 = eMOTOR_10,
} eMOTOR;

typedef enum {
	eALIGN_EDGE = 0,
	eALIGN_MARK,
} eALIGNMENT_TYPE;

typedef enum {
	ePROCESS_TOTAL = 0,		// Single-Head for one Cell
	ePROCESS_REGION,		// Multi-Head for (one) Cell(s)
} ePROCESS_TYPE;

typedef enum {
	eSEQ_NONE,
	eSEQ_ALIGN,
	eSEQ_ALIGN_COMPLETE,
	eSEQ_DRAW,
	eSEQ_DRAW_COMPLETE,
} eSEQUENCE;

typedef enum {
	eMATERIAL_FILM = 0,
	eMATERIAL_GLASS,
} eMATERIAL;

typedef struct {
	double	x;
	double	y;
} T_POINT;

typedef struct {
	double	dWidth;
	double	dHeight;
	double	dThickness;
	eMATERIAL	eMaterial;
} T_MATERIAL_INFO;

typedef struct {
	int						nIndex;				// 
	BOOL					bProcess;
	BOOL					bAlignmentResult;
	int						nDrawScannerNo;
	int						iProcessStep;
	int						eDirection;
	DWORD					dwTactAlignment;
	DWORD					dwTactDraw;
	T_POINT					ptPosition;			// Logical Position (Base : Left Bottom Cell)
	T_POINT					ptOffset;
	T_POINT					ptAlignmentPosition[eAP_END];
} T_CELL_INFO;

typedef struct
#ifdef _IP_DATA_INTERNAL_
#else
	_T_LAYER_CONDITION
#endif
{
	wchar_t					szName[128];
	BOOL					bProcess;
	int						nProcessCount;

	int						nJumpSpeed;
	int						nMarkSpeed;
	int						nJumpDelay;
	int						nMarkDelay;
	int						nPolygonDelay;
	int						nLaserOnDelay;
	int						nLaserOffDelay;
	
	double					dLaserPower[eSCANNER_END];
	double					dLaserPRR;
	int						nLaserPulseDuration;

	BOOL					bSkywritingUsed;
	double					dSkywritingTimelag;
	int						nSkywritingLaserOnShift;

	double					dSkywritingPrev;
	double					dSkywritingPost;

	int						nSkyWritingMode;
	double					dSkyWritinglimit;
#ifdef _IP_DATA_INTERNAL_
#else
	_T_LAYER_CONDITION() {
		szName[0] = 0;
		bProcess = TRUE;
		nProcessCount = 1;

		nJumpSpeed = 2000;
		nMarkSpeed = 3000;
		nJumpDelay = 400;
		nMarkDelay = 400;
		nPolygonDelay = 50;
		nLaserOnDelay = 200;
		nLaserOffDelay = 400;

		for (int i = 0; i < countof(dLaserPower); i++)
			dLaserPower[i] = 10;
		dLaserPRR = 60.0;
		nLaserPulseDuration = 4;	// 4, 8, 14, 20, 100, 200

		bSkywritingUsed = TRUE;
		dSkywritingTimelag = 318;
		nSkywritingLaserOnShift = -7;

		dSkywritingPrev = 25;
		dSkywritingPost = 25;

		nSkyWritingMode = 2;
		dSkyWritinglimit = 0;
	}
#endif
} T_LAYER_CONDITION;

#define MAX_PATH 260

typedef struct {
	BOOL				bUse;
	T_POINT				ptInDXF;
	T_POINT				ptOffset;
	wchar_t				szImage[MAX_PATH];
	double				dThreshold;
} T_ALIGN;

typedef struct
#ifdef _IP_DATA_INTERNAL_
#else
	_T_RECIPE_INFO
#endif
{
	// info
	//wchar_t					szProjectName[MAX_PATH];
	wchar_t					szDxfFileName[MAX_PATH];
	T_MATERIAL_INFO			MaterialInfo;

	// Align
	int						eAlignMode;	// 0 : Pattern Matching, Edge Detecting
	int						nPatternMatchinRate;
	T_POINT					ptStartPosition[eTABLE_END];
	T_ALIGN					align[eAP_END];

	// DXF Layer (가공 정보)
	int						nLayerCount;
	T_LAYER_CONDITION		LayerCondition[MAX_LAYERS];

	// Cell Info
	ePROCESS_TYPE			iProcessMode;
	int						nCellCount;
	int						nSheetArrayX;
	int						nSheetArrayY;
	T_POINT					ptCellPitch;
	T_CELL_INFO				CellInfos[MAX_CELL];
#ifdef _IP_DATA_INTERNAL_
#else
	_T_RECIPE_INFO() {
		szDxfFileName[0] = NULL;
		ZeroVar(MaterialInfo);

		// Align
		eAlignMode = 0;	// 0 : Pattern Matching, Edge Detecting
		nPatternMatchinRate = 80;
		ZeroVar(ptStartPosition);
		ZeroVar(align);

		ptStartPosition[eTABLE_MASTER].x = 3;
		ptStartPosition[eTABLE_MASTER].y = 816;
		ptStartPosition[eTABLE_SLAVE].x = -414;
		ptStartPosition[eTABLE_SLAVE].y = 816;

		// DXF Layer (가공 정보)
		nLayerCount = 0;
		ZeroVar(LayerCondition);

		// Cell Info
		iProcessMode = ePROCESS_REGION;
		nCellCount = 0;
		nSheetArrayX = 0;
		nSheetArrayY = 0;
		ZeroVar(ptCellPitch);
		ZeroVar(CellInfos);
	}
#endif
} T_RECIPE_INFO;
//=================================================
typedef enum {
	eAMS_READY,
	eAMS_AREA_SENSOR_ON,
	eAMS_CHECK_VACCUM,
	eAMS_MOVE_ALIGN_POS,
	eAMS_MOVE_CHECK,
	eAMS_ALIGN,
	eAMS_ALIGNING,
	eAMS_ALIGN_COMPLETE,
	eAMS_DRAW,
	eAMS_DRAWING,
	eAMS_DRAW_COMPLETE,
	eAMS_MOVE_LOADING_POSITION,
	eAMS_CHECK_LOADING_POSITION,
	eAMS_COMPLETE,
} eAUTO_MODE_SEQUENCE;

typedef struct {
	BOOL		bAutoModeOn;
	eAUTO_MODE_SEQUENCE		eProcessMode[eTABLE_END];

	int			iCurrentProcessIndex;			// Align ~ Drawing하고 있는 Table Index
	BOOL		bAlignPass;						// Align만 Pass
	BOOL		bProcessPass;					// 가공만 Pass

	T_POINT					ptCenter[eTABLE_END];
	
	int						nCellCount[eTABLE_END];
	T_CELL_INFO				CellInfo[eTABLE_END][MAX_CELL];
} T_LOT_PROCESS;

#endif

typedef struct {
	double x, y;
} T_VISION_POSITION;

typedef struct {
	double		dPosition;
	double		dWorkPosition;
	double		dWorkPositionVX0;
	BOOL		bValidWorkPosition;
	double		dCommandPosition;
	double		dVelocity;
	double		dActualLoad;
	DWORD		fEncoder_Error;
	union {
		struct {
			DWORD fReserved_00					:1;
			DWORD fReserved_01					:1;
			DWORD fReserved_02					:1;
			DWORD fReserved_03					:1;
			DWORD fReserved_04					:1;
			DWORD fReserved_05					:1;
			DWORD fReserved_06					:1;
			DWORD fReserved_07					:1;

			DWORD fReserved_08					:1;
			DWORD fReserved_09					:1;			
			DWORD fHomeSearchActive				:1;			
			DWORD fBlockRequest					:1;
			DWORD fAbortDecelertation			:1;
			DWORD fDesiredVelocityZero			:1;	// 33
			DWORD fDataBlockError				:1;
			DWORD fDwellInProgress				:1;

			DWORD fIntegrationMode				:1;
			DWORD fRunningProgram				:1;
			DWORD fOpenLoopMode					:1;
			DWORD fPhasedMotor					:1;
			DWORD fEnable						:1;
			DWORD fPosition_Limit				:1;	// 21
			DWORD fNegative_Limit				:1;	// 22
			DWORD fMotorActivated				:1;	// 39
			
			DWORD fReserved_24					:1;
			DWORD fReserved_25					:1;
			DWORD fReserved_26					:1;
			DWORD fReserved_27					:1;
			DWORD fReserved_28					:1;
			DWORD fReserved_29					:1;
			DWORD fReserved_30					:1;
			DWORD fReserved_31					:1;
		};
		DWORD	dwLow;
	};
	union {
		struct {
			DWORD fInPosition					:1;	// 40
			DWORD fFollowingErrorWarning		:1;	// 
			DWORD fFollowing_Error				:1;	// 42
			DWORD fAmp_Fault					:1;	// 43
			DWORD fBacklashDirectionFlag		:1;	// 
			DWORD fI2TAmpFaultError				:1;
			DWORD fIntegratedFatalFollowError	:1;
			DWORD fTriggerMove					:1;	// 

			DWORD fPhasingSearchError			:1;
			DWORD fReserved_09					:1;			
			DWORD fHomeCompleted				:1;	// 45		
			DWORD fStoppedOnPositionLimit		:1;
			DWORD fReserved_12					:1;
			DWORD fReserved_13					:1;
			DWORD fReserved_14					:1;
			DWORD fReserved_15					:1;

			DWORD fReserved_16					:1;
			DWORD fReserved_17					:1;
			DWORD fReserved_18					:1;
			DWORD fReserved_19					:1;
			DWORD fCS1_LSB						:1;
			DWORD fCS1							:1;
			DWORD fCS1_MSB						:1;
			DWORD fAssignedToCS					:1;

			DWORD fReserved_24					:1;
			DWORD fReserved_25					:1;
			DWORD fReserved_26					:1;
			DWORD fReserved_27					:1;
			DWORD fReserved_28					:1;
			DWORD fReserved_29					:1;
			DWORD fReserved_30					:1;
			DWORD fReserved_31					:1;
		};
		DWORD	dwHigh;
	};
	//int		i24BitServoCycleCount;				// 0
	//int		i24BitCountPosition;				// 1
	//
	//double dPositionCommand;					// 61	
	//double dPositionActual;						// 62	
	//double dPositionBios;						// 64
	//double dDac;								// 68
	//double dPositionTargetJog;					// 72
	//double dPhaseFindingOutputValue;			// 73
	//double dActualVelocity;						// 74
	//double dCurrentActualDirect;				// 76
	//double dScaleFactor_XUABC;					// 91
	//double dScaleFactor_YV;						// 92
	//double dScaleFactor_ZW;						// 93
} T_MOTOR_STATUS;


//------------------------------------------------------------------------------------------------------
#define MAX_SYSTEM_ALARM	500
#define MAX_SVID			30
#define MAX_TRID			100
#define MAX_REPGSZ			100	// Reporting Group Size
#define MAX_LOADING_PORT 1
#define MAX_UNLOADING_PORT 2
#define MAX_LOADING_UNLOADING_PORT 3

typedef enum {
	MCMD_OFFLINE = 1, 
	MCMD_LOCAL, 
	MCMD_REMOTE,
} eMCMD_CODE;

typedef enum {
	ES_NORMAL = 1, 
	ES_FAULT, 
	ES_PM, 
} eEQP_STATE;

typedef enum {
	PS_INIT = 1, 
	PS_IDLE,
	PS_SETUP, 
	PS_READY, 
	PS_EXECUTE, 
	PS_PAUSE, 
	ePS_END,
} ePROCESS_STATE;

typedef enum {
	BW_HOST = 1, 
	BW_OPERATOR, 
	BW_EQUIPMENT, 
	BW_INDEXER,
} eBY_WHO;

typedef enum {
	GS_IDLE = 1,
	GS_SELECTED, 
	GS_PROCESSING, 
	GS_DONE, 
	GS_ABORTING, 
	GS_ABORTED, 
	GS_CANCELED,
} eGLASS_STATES;


typedef struct _T_SYSTEM_PARAMETER {
	int		iAutoModeTowerLampChangeTime; //Auto Mode Green -> Yellow (No Run)
	int		iFumeCollectorAutoOffTime;
	int		iProcessResetTime; //Alarm Count, Total Tact...
	int		iLogAndIngKeepTime; //Log, Align Image Delete

	BOOL	bDoorLockSkip;
	BOOL	bAreaSensorSkip;
	BOOL	bAlignFailAutoSkip;
	BOOL	bLaserSettingSkip;

	BOOL	bLightRemote;
	BOOL	b2CameraAlign;


	//eALIGN_ORDER eAlignOrder;		// Align Mode.

#ifndef _IP_DATA_INTERNAL_
	_T_SYSTEM_PARAMETER() {
		//iAutoModeTowerLampChangeTime;
		//iFumeCollectorAutoOffTime;
		//iProcessResetTime;

		bDoorLockSkip = FALSE;
		bAreaSensorSkip = FALSE;
		bAlignFailAutoSkip = TRUE;
		b2CameraAlign = FALSE;
		//eAlignOrder = AM_4TH_ORDER;
	}
#endif
} T_SYSTEM_PARAMETER;


typedef struct {
	BOOL bHeavy;
	union {
		struct {
			BYTE eALCD : 7;
			BYTE bInAlarm : 1;
		};
		BYTE alcd;
	};
	UINT eALID;
	char szALTM[32];		// YYYYmmddHHMMSS
	char szALTX[120];

} T_SYSTEM_ALARM_ITEM;

typedef struct {
	int nALARM;
	T_SYSTEM_ALARM_ITEM alarms[MAX_SYSTEM_ALARM];
} T_SYSTEM_ALARM;

typedef struct {
	char szSoftRev[24];
	char szMDLN[12];
	char szOnline[12];
	char szModuleID[64];
	char szOperator[32];

	char szTimeNewEQPState[32];

	eMCMD_CODE eMCMD, eMCMD_Old;
	eEQP_STATE eEQPState, eEQPState_Old;
	ePROCESS_STATE eProcState, eProcState_Old;

	ePROCESS_STATE eSubProcState[MAX_LOADING_UNLOADING_PORT], eSubProcState_Old[MAX_LOADING_UNLOADING_PORT];

	T_SYSTEM_ALARM_ITEM alarms[20];

} T_SYSTEM;

typedef struct {
	//No,SVID,SV Name,Type,Unit,Range,ModuleID,Description
	int no;
	int eSVID;
	char szSV[32];
	char szSVIDName[64];
	char szType[32];
	char szUnit[32];
	char szRange[32];
	char szModuleID[32];
	char szDescription[256];
} T_SVID_ITEM;

typedef struct {
	int nSVID;
	T_SVID_ITEM items[MAX_SVID];
} T_SVID;

typedef struct {
	BOOL bToBeApplied;				// Trigger

	uint32_t eTRID;					// Trace Request ID (MAX 100)

	uint32_t nDataSamplingInterval;	// in msec,
	uint32_t dwLastTick;			// in msec,

	uint16_t nTotalSample;			// 1~65535
	uint16_t iSampleIndex;			// 1~65535

	uint32_t nReportingGroup;	// REPGSZ
								//uint32_t nItemCollected;		// Current Iems

	uint32_t nSVID;
	uint32_t SVIDs[MAX_SVID];

	int iReportingGroup;
	struct {
		char szTime[32];			// YYYYMMDDhhmmss
		struct {
			uint32_t __eSVID;			// obsolete
			char szValue[32];
		} svids[MAX_SVID];
	} group[MAX_REPGSZ];

} T_TRACE_SVID_ITEM;

typedef struct {
	int __nTrace;						// Obsolete
	T_TRACE_SVID_ITEM items[MAX_TRID];
} T_TRACE_SVID;


#pragma pack(pop)
