#pragma once
#include <afxmt.h>

#include "AFX_EXT_MISC.h"
#include "xUtil.h"
#include "SimpleLog.h"

#pragma pack(push, 8)

// CXComm
#define WM_COMM_RECEIVED (WM_APP+100)
#define WM_COMM_EVT (WM_APP+101)
#define WM_COMM_CLOSE (WM_APP+102)
#define WM_COMM_RECEIVING_TIMEDOUT (WM_APP+103)

#define    MAX_RECV_BUF   1000
#define    MAX_SEND_BUF   1000

#define    RECV_ENQ_TIME      30  //5

#define    ACK      0x06
#define    NAK      0x15
#define    ENQ      0x05
#define    EOT      0x04
#define    STX      0x02
#define    ETX      0x03

#define    CTS      MS_CTS_ON
#define    DSR      MS_DSR_ON
#define    RING     MS_RING_ON
#define    DCD      MS_RLSD_ON

#define FC_DTRDSR       0x01
#define FC_RTSCTS       0x02
#define FC_XONXOFF      0x04

#define ASCII_XON       0x11
#define ASCII_XOFF      0x13

class AFX_EXT_CLASS_MISC CXComm : public CSimpleLogInterface {
protected:
	HANDLE m_hComm;
	std::vector<BYTE> m_buf;
	mutable CCriticalSection m_csReadBuffer;
	CEvent m_eStopReader;
	CEvent m_eStopEventHandler;
	CEvent m_eEventHandlerStopped;
	HANDLE m_hReader;
	HANDLE m_hEventHandler;
	size_t m_nRThreshold;
	CWnd* m_pOwner;
	OVERLAPPED m_overlappedR, m_overlappedW;

	DWORD m_dwError;
	DWORD m_dwTimeoutTX;
	DWORD m_dwTimeoutRX;

public:
	CXComm();
	virtual ~CXComm();

	HANDLE GetCommHandle() { return m_hComm; }

public:
	void SetOwner(CWnd* pOwner) { m_pOwner = pOwner; }
	CWnd* GetOwner() { return m_pOwner; }
	BOOL IsOpened(void) const;
	BOOL IsOpen(void) const;
	BOOL Open(ULONG nPort/*1 base*/, DWORD baud = 9600, DWORD fBinary = 1, BYTE ByteSize = 8, BYTE Parity = NOPARITY, BYTE StopBits = ONESTOPBIT, int eRTS = RTS_CONTROL_ENABLE, int eDTR = DTR_CONTROL_ENABLE, BOOL bXOnOff = FALSE);
	BOOL Open(LPCTSTR pszPortName, DWORD baud = 9600, DWORD fBinary = 1, BYTE ByteSize = 8, BYTE Parity = NOPARITY, BYTE StopBits = ONESTOPBIT, int eRTS = RTS_CONTROL_ENABLE, int eDTR = DTR_CONTROL_ENABLE, BOOL bXOnOff = FALSE);
	BOOL Close(void);
	BOOL Attach(HANDLE hComm);
	HANDLE Detach();
	BOOL SetRThreshold(size_t nByte, DWORD dwTimeoutRX = INFINITE /*in msec*/);
	BOOL SetCommMask(DWORD dwEvtMask);
	BOOL EscapeCommFunction(DWORD dwFunc) {
		if (IsOpen()) {
			return ::EscapeCommFunction(m_hComm, dwFunc);
		}
		return FALSE;
	}

	size_t Read(void* buf, DWORD nMaxSize, DWORD dwTimeout = INFINITE);
	size_t Write(const void* buf, DWORD nSize, DWORD dwTimeout = INFINITE);
	BOOL Purge(DWORD dwFlags = PURGE_RXABORT|PURGE_RXCLEAR|PURGE_TXABORT|PURGE_TXCLEAR);

	void DeleteBuffer();
	size_t PeakBuffer(void* buf, size_t nMaxSize) const;
	size_t PeakBuffer(CByteArray& buf) const;
	size_t PeakBuffer(std::vector<BYTE>& buf) const;

	size_t ReadBuffer(void* buf, size_t nMaxSize);
	size_t ReadBuffer(CByteArray& buf);
	size_t ReadBuffer(CByteArray& buf, size_t nMaxSize);
	size_t ReadBuffer(std::vector<BYTE>& buf);
	size_t ReadBuffer(std::vector<BYTE>& buf, size_t nMaxSize);

	size_t ReadBufferAndAppend(std::vector<BYTE>& buf);

	size_t ConsumeBuffer(size_t nSize);

	std::vector<BYTE>& GetBuffer() { return m_buf; }
	const std::vector<BYTE>& GetBuffer() const { return m_buf; }
	size_t GetInBufferCount() const { m_csReadBuffer.Lock(); size_t t = m_buf.size(); m_csReadBuffer.Unlock(); return t; }

public:
	// Enumerates Serial Ports ( Reg. HKEY_LOCAL_MACHINE\\HARDWARE\\DEVICEMAP\\SERIALCOMM )
	static BOOL FindSerialPorts(CStrings& strsPort);

protected:
	static DWORD WINAPI Reader(LPVOID pVoid);
	static DWORD WINAPI EventHandler(LPVOID pVoid);

	void FireReceived(size_t nReceived) {
		if (m_pOwner)
			m_pOwner->SendMessage(WM_COMM_RECEIVED, (WPARAM)this, nReceived);
	}
	void FireEvt(DWORD dwEvt) {
		if (m_pOwner)
			m_pOwner->SendMessage(WM_COMM_EVT, (WPARAM)this, (LPARAM)dwEvt);
	}
	LRESULT FireReceivingTimeout(size_t nReceived) {
		if (m_pOwner)
			return m_pOwner->SendMessage(WM_COMM_RECEIVING_TIMEDOUT, (WPARAM)this, nReceived);
		return 0L;
	}
};

#pragma pack(pop)
