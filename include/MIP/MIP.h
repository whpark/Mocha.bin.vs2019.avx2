// MIP.h: interface for the CInterProcess class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

//#include "AFX_EXT_MIP.h"

// C++ Class Declare
#if defined(__cplusplus)

	#include "IPVariable.h"
	#include "IPData.h"
	#include "IPDataIterator.h"
	#include "IPCommand.h"
	#include "IPCommandTarget.h"
	#include "IPConnection.h"
	#include "IPCommandPipe.h"
	#include "IPClient.h"
	//#include "IPServer.h"

	#pragma pack(push, 8)

	//-----------------------------------------------------------------------------
	// Short names
	//
	typedef CIPVariable			CIPVar;
	typedef CIPCommand			CIPCmd;
	//typedef IIPCommandTarget	IIPCmdTgt;
	//typedef TIPCommandTarget	TIPCmdTgt;

	#pragma pack(pop)

#endif	// __cplusplus

