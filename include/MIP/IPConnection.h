#pragma once

#include "misc/xUtil.h"
#include "misc/Socket2.h"
#include "misc/SimpleLog.h"

#include "IP_Definitions.h"

#pragma pack(push, 8)

//=============================================================================
// CIPConnection
//

class CIPConnection : public CSimpleLogInterface {
private:
	static BYTE s_signature[IP_LEN_SIGNATURE];
protected:
	CSocket2 m_socketWorker;

public:
	CIPConnection();
	virtual ~CIPConnection();

protected:
	// these constructors are NOT allowed to call.
	CIPConnection(const CIPConnection& B) { ASSERT(FALSE); }
	CIPConnection& operator = (const CIPConnection& B) { ASSERT(FALSE); return *this; }

	// Socket Operation
public:
	BOOL AttachSocket(SOCKET hSocket);
	SOCKET DetachSocket();
	BOOL Close(BOOL bNotify = TRUE);
	const SOCKET GetSocket() const { return m_socketWorker.m_hSocket; }

	// overridable
protected:
	//virtual BOOL OnConnected(HIPCOMMAND hCMD) = NULL;
	virtual BOOL OnDisconnected() = NULL;
	virtual BOOL OnNewPacket(const IP_PACKET_HEADER& header, const void* pData) = NULL;

	// Operation
public:
	BOOL PrepareNewPacket();
	BOOL SendPacket(const IP_PACKET_HEADER& header, const void* pData);

	// Read/Write through socket
protected:
	CCriticalSection m_csSendPacket;
	std::atomic<DWORD> m_dwIndexLast;

	HANDLE m_hReader;
	static DWORD __stdcall ReaderT(LPVOID pParam);
	DWORD Reader();

};

#pragma pack(pop)
