#pragma once

#include "AFX_EXT_MIP.h"

#include "IP_Definitions.h"
#include "IPConnection.h"
#include "IPCommand.h"

#pragma pack(push, 8)

class IIPTask;
class CIPServer;
class CIPClient;

class CIPCommandPipe;
class CIPCommandPipeClient;
class CIPCommandPipeServer;

//=============================================================================
// CIPCommandPipe
//

class CIPCommandPipe : public CIPConnection {
	friend class IIPTask;
	friend class CIPServer;
	friend class CIPClient;
	// Task Name
protected:
	DStringA m_strTaskName;	// for IPServer
public:
	LPCSTR GetTaskName() const { return m_strTaskName; }
	BOOL operator == (LPCSTR pszTaskName) const { return m_strTaskName == pszTaskName;}

protected:
	// IPCommands Received. (from other task or from self)
	TRefList<CIPCommand> m_cmdsReceived;

	// IPCommands Sent. (to other task or to self)
	TRefList<CIPCommand> m_cmdsSent;

protected:
	IIPTask* const m_pTask;
public:
	CIPCommandPipe(IIPTask* pTask, LPCSTR pszTaskName);
	virtual ~CIPCommandPipe();

	// operations
public:
	BOOL SendIPCommand(HIPCOMMAND hCMD);
	BOOL OnIPCommandReceived(HIPCOMMAND hCMD);

	BOOL AckIPCommand(HIPCOMMAND hCMD);
	BOOL OnIPCommandAcked(HIPCOMMAND hCMD);

	BOOL ReplyIPCommand(HIPCOMMAND hCMD);
	BOOL OnIPCommandReplied(HIPCOMMAND hCMD);

	// Events from Connection
protected:
	BOOL OnConnected(HIPCOMMAND hCMD);
	BOOL OnDisconnected();
	BOOL OnNewPacket(const IP_PACKET_HEADER& header, const void* pData);

	// Send IP Packet
protected:
	virtual BOOL ProcessIPCommand(HIPCOMMAND hCMD, eIP_PACKET_TYPE eType) = NULL;

};

class CIPCommandPipeClient : public CIPCommandPipe {
public:
	CIPCommandPipeClient(IIPTask* pTask, LPCSTR pszTaskName) : CIPCommandPipe(pTask, pszTaskName) {
	}
protected:
	virtual BOOL ProcessIPCommand(HIPCOMMAND hCMD, eIP_PACKET_TYPE eType);
};

class CIPCommandPipeServer : public CIPCommandPipe {
public:
	CIPCommandPipeServer(IIPTask* pTask) : CIPCommandPipe(pTask, NULL) {}
protected:
	virtual BOOL ProcessIPCommand(HIPCOMMAND hCMD, eIP_PACKET_TYPE eType);
};
