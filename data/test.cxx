
#include "test.hxx"

//#ifdef BLOCK_TEMP.

typedef enum {
	LOM_NONE = -1,
	LOM_INTERNAL_POWER_PW_FREQ_CLOSED	= 0x0000,
	LOM_INTERNAL_POWER_PW_FREQ_OPEN		= 0x0011,
	LOM_INTERNAL_POWER_PW_NC			= 0x0010,
	LOM_EXTERNAL_PWM					= 0x1313,
} eLASER_OPERATION_MODE;

struct {
	eTRIGGER	eTrigger = 1;

	POINT2D pts[3];
	POINT2D pt0, pt1;
	RECT2D rect0, rect1;
	RECT2D rects[5];
	POINT2D ptTest;
} T_POINTS;

struct {
	char szVersion[32];
	struct {
		char szSerialNumber[8];
		char eHeadType;
		char eDuty = 9;
		char eTubeType;
		char eWaveLength;
		char PeakPower[4];
	} info;

	double dPower;			// in W
	double dCurrentPower;	// in W
	double dFreq;			// in kHz
	double dPulseWidth;		// in usec	(2~400)
	eLASER_OPERATION_MODE eOperationMode = LOM_INTERNAL_POWER_PW_FREQ_OPEN;
	DWORD dwTriggerDelay;	// in msec
	double dFlowRate = 0.3;		// in liter/minute

	union {
		struct {
			struct {
				WORD bLaserOn : 1;
				WORD _eFlag0Bit1 : 1;
				WORD bShutterError : 1;
				WORD _eFlag0Bit3 : 1;
				WORD _eFlag0Bit4 : 1;
				WORD _eFlag0Bit5 : 1;
				WORD bFlowFailing : 1;
				WORD _eFlag0Bit7 : 1;
				WORD bPowerLow : 1;
				WORD bInterlockFault : 1;
				WORD _eFlag0BitA : 1;
				WORD bRFTemperatureFault : 1;
				WORD _eFlag0BitC : 1;
				WORD _eFlag0BitD : 1;
				WORD bFlowFailed : 1;
				WORD _eFlag0BitF : 1;
			};
			struct {
				WORD bVSWRError : 1;
				WORD bOverModulationError : 1;
				WORD bDCDisable : 1;
				WORD bRFDisable : 1;
				WORD _eFlag1Bit4 : 1;
				WORD bControlLimit : 1;
				WORD bShutterNotClosed : 1;
				WORD bShutterNotOpen : 1;
				WORD _eFlag1Bit8 : 1;
				WORD _eFlag1Bit9 : 1;
				WORD _eFlag1BitA : 1;
				WORD bUserShutterDisable : 1;
				WORD bPowerHight : 1;
				WORD _eFlag1BitD : 1;
				WORD _eFlag1BitE : 1;
				//WORD _eFlag1BitF : 1;
			};
			struct {
				WORD bUpdate : 1;
				WORD bNewCommand : 1;
				WORD _eFlag2Bit2 : 1;
				WORD bDCFault : 1;
				WORD bNewMode : 1;
				WORD _eFlag2Bit5 : 1;
				WORD _eFlag2Bit6 : 1;
				WORD bUserRFEnable : 1;
				WORD bShutterClosed : 1;
				WORD bShutterOpen : 1;
				WORD bStandbyMode : 1;
				WORD bRunMode : 1;
				WORD bRFEnabled : 1;
				WORD bArmed : 1;
				WORD _eFlag2BitE : 1;
				WORD _eFlag2BitF : 1;
			};
		};
		WORD wStatusFlags[3];
	};
} T_LASER_STATUS;

//T_MOTOR_STATUS MotorStatus[eMOTOR_END];

//typedef struct {
//} BYTE_BIT_FIELD;

//union {
//	struct {
//		DWORD a:1, b:1, c:1;
//		unsigned int d:1;
//		struct {
//			BYTE e:3;
//			BYTE f:6;
//			BYTE g:2;
//		} t;
//		int dummy;
//		BYTE a0:1, a1:1, a2:1, a3:1, a4:1, a5:1, a6:1, a7:1, a8:1, a9:1, a10:1;
//	} t;
//	int d[4];
//} TYPE;
//
//T_MOCHA_INFO MochaInfo;
//
//T_MOCHA_PROCESS_INFO MochaProcessInfo[MOCHA_MAX_PROCESS];
//
//#ifdef _DEBUG_IP_DATA_
//unsigned char a = '1';
//unsigned char b = -2;
//char c = 3;
//char d = -4;
//char e = '0';
//BYTE dd[4][5][6];
//
//T_TESTA TestA = {
//	1, 2, 3.3, L"abcde"
//};
//T_TESTA TestAArray[10] = {
//	{  1,  3, 10.1, L"0" },
//	{  2,  4, 10.2, L"1" },
//	{  3,  5, 10.3, L"2" },
//	{  4,  6, 10.4, L"3" },
//	{  5,  7, 10.5, L"4" },
//	{  6,  8, 10.6, L"5" },
//	{  7,  9, 10.7, L"6" },
//	{  8, 10, 10.8, L"7" },
//	{  9, 11, 10.9, L"8" },
//	{ 10, 12, 11.0, L"9" },
//};
//T_TEST Test = {
//	{ 0, 2, 3.3, L"abcde" },
//	{
//		{  1,  3, 20.1, L"0" },
//		{  2,  4, 20.2, L"1" },
//		{  3,  5, 20.3, L"2" },
//		{  4,  6, 20.4, L"3" },
//		{  5,  7, 20.5, L"4" },
//		{  6,  8, 20.6, L"5" },
//		{  7,  9, 20.7, L"6" },
//		{  8, 10, 20.8, L"7" },
//		{  9, 11, 20.9, L"8" },
//		{ 10, 12, 21.0, L"9" },
//	},
//	{ 100, 2, 103.3, L"abcde" },
//};
//T_TEST TestArray[100];
//
//T_RECIPE_INFO		RecipeInfo;
//T_LOT_PROCESS		LotProcess;

//#endif


//------------------------------------------------------------------------------------------------------
T_SYSTEM System = {
	"0000000000000001",
	"CRP",
	"ONLINE",
	"C2CRP08C",
	"operator",

	"",

	MCMD_OFFLINE,
	ES_NORMAL,
	PS_IDLE,


};

T_SYSTEM_PARAMETER	SystemParameter;

T_SYSTEM_ALARM SystemAlarm;

T_SVID	SVID;

T_TRACE_SVID TraceSVID;

//BYTE bufBig1[536870912];
//BYTE bufBig2[536870912];//1
//
//BYTE bufBig3[536870912];
//BYTE bufBig4[536870912];//2
//
//BYTE bufBig5[536870912];
//BYTE bufBig6[536870912];//3
//
//BYTE bufBig7[536870912];
//BYTE bufBig8[536870912];//4
//
//BYTE bufBig9[536870912];
//BYTE bufBig10[536870912];//5

//#endif // BLOCK_TEMP
