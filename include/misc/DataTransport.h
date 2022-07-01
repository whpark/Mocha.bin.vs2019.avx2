#pragma once

#include "AFX_EXT_MISC.h"
#include "Socket2.h"
#include "TList.hpp"
#include <vector>

//=============================================================================
//

class AFX_EXT_CLASS_MISC IDataTransport {
public:
	IDataTransport(void);
	virtual ~IDataTransport(void);

public:
	virtual BOOL OpenPort(UINT nPort, BOOL bCallBackMode = FALSE) = NULL;
	virtual BOOL ClosePort() = NULL;
	virtual BOOL IsPortOpen() const = NULL;
	virtual BOOL SendData(LPCTSTR pszIP, UINT nPort, const void* pData, size_t nLength, DWORD dwTimeout1 = 100, DWORD dwTimeout2 = 1000) = NULL;
	virtual BOOL OnDataReceived(LPCTSTR pszIP, UINT nPort, std::vector<BYTE>& received) = NULL;

};

//=============================================================================
//-----------------------------------------------------------------------------
//
class CDataTransportUDPPacket {
public:
	SOCKADDR_IN sockAddrSender;
	uint16_t iPacketIndex;
	UINT nPacket;
	std::vector<BYTE> packetStatus;
	std::vector<std::vector<BYTE> > data;
	CEvent evt;
	DWORD dwTickLast;
	BOOL bComplete;
public:
	CDataTransportUDPPacket(const SOCKADDR_IN* pSockAddr = NULL, uint16_t _iPacketIndex = 0, int _nPacket = 0) : iPacketIndex(_iPacketIndex), nPacket(_nPacket), evt(FALSE, FALSE, NULL, NULL) {
		if (nPacket) {
			packetStatus.reserve(nPacket);
			packetStatus.assign(nPacket, 0);
		}
		ZeroVar(sockAddrSender);
		if (pSockAddr)
			memcpy(&sockAddrSender, pSockAddr, sizeof(sockAddrSender));
		dwTickLast = GetTickCount();
		bComplete = FALSE;
	}
	CDataTransportUDPPacket(const CDataTransportUDPPacket& B) { ASSERT(FALSE); }
	CDataTransportUDPPacket& operator = (const CDataTransportUDPPacket& B) { ASSERT(FALSE); return *this; }
	~CDataTransportUDPPacket() { evt.Unlock(); }
};
//-----------------------------------------------------------------------------
class AFX_EXT_CLASS_MISC CDataTransportUDP : public IDataTransport {
protected:
	CSocket2 m_socket;
	long m_nPort {};
	long m_iPacketIndex {};
	DWORD m_dwTimeout1{100}, m_dwTimeout2{1000};
	TList<CDataTransportUDPPacket> m_DTPackets;
public:
	CDataTransportUDP();
	virtual ~CDataTransportUDP();

public:
	virtual BOOL OpenPort(UINT nPort, BOOL bCallBackMode = FALSE);
	virtual BOOL ClosePort();
	virtual BOOL IsPortOpen() const { return (m_socket.m_hSocket != INVALID_SOCKET) && m_hReceiver; }
	virtual BOOL SendData(LPCTSTR pszIP, UINT nPort, const void* pData, size_t nLength, DWORD dwTimeout1 = 100, DWORD dwTimeout2 = 1000);
	virtual BOOL OnDataReceived(LPCTSTR pszIP, UINT nPort, std::vector<BYTE>& received);// = NULL;
	virtual UINT GetPort() const { return IsPortOpen() ? m_nPort : 0; }

	virtual BOOL GetPacket(CString& strIPSender, UINT& nPortSender, std::vector<BYTE>& received);
	virtual int GetPacketCount() const { return m_received.size(); }

protected:
	HANDLE m_hReceiver{};
	HANDLE m_hWaiter{};
	CEvent m_evtReceived{};
	CEvent m_evtStop{};
	static DWORD __stdcall ReceiverT(LPVOID pParam);
	static DWORD __stdcall WaiterT(LPVOID pParam);
	struct T_RECEIVED {
		std::vector<BYTE> received;
		CString strIPSender;
		UINT nPortSender{};
	};
	TList<T_RECEIVED> m_received;
	DWORD Receiver();
	DWORD Waiter();
	static void GarbageCollection(TList<CDataTransportUDPPacket>& DTPackets, DWORD dwTimeout);
};
