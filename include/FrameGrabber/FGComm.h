#pragma once

#include "misc/XComm.h"

#pragma pack(push, 8)

class IFGComm;	// Serial-Communication over FrameGrabber (CameraLink)

//-----------------------------------------------------------------------------
// IFrameGrabberInterface Comm Interface
class IFGComm {
public:
	IFGComm() {}
	virtual ~IFGComm() {
		//CloseComm();
	}
public:
	virtual BOOL IsOpen() const = NULL;
	virtual BOOL OpenComm(LPCTSTR pszPort = NULL, DWORD baud = 9600, BYTE ByteSize = 8, BYTE Parity = NOPARITY, BYTE StopBits = ONESTOPBIT, BYTE eXOnOff = 0) { return FALSE; }
	virtual BOOL CloseComm() { return FALSE; }
	virtual BOOL WriteComm(const void* buf, DWORD nSize, DWORD dwTimeout = INFINITE) = NULL;
	virtual BOOL ReadComm1byte(BYTE& c, DWORD dwTimeout = INFINITE) = NULL;
	virtual int ReadComm(void* buf, int nMaxSize, const char* pszEnd = NULL, DWORD dwTimeout = INFINITE) = NULL;
	virtual void PurgeComm(DWORD dwFlags = PURGE_RXABORT|PURGE_RXCLEAR|PURGE_TXABORT|PURGE_TXCLEAR) = NULL;
public:
	virtual int ReadCommLine(void* buf, int nMaxSize, const char* pszEnd = "\r", DWORD dwTimeout = INFINITE) {
		if (!buf || nMaxSize <= 0)
			return 0;
		int nRead = 0;
		DWORD dwTick0 = GetTickCount();
		DWORD dwTick1 = dwTick0;
		int nFilterLength = (int)strlen(pszEnd);
		BYTE* pHead = (BYTE*)buf;
		BYTE* pos = (BYTE*)buf;
		for (nRead = 0; nRead < (nMaxSize-1); pos++, nRead++) {
			DWORD nTickPassed = dwTick1 - dwTick0;
			if ( (dwTimeout != INFINITE) && (nTickPassed >= dwTimeout) )
				return FALSE;
			if (!ReadComm1byte(*pos, dwTimeout == INFINITE ? INFINITE : dwTimeout-nTickPassed))
				break;
			if ( (pos - pHead >= nFilterLength) && (memcmp(pos-nFilterLength+1, pszEnd, nFilterLength) == 0) ) {
				pos++;
				break;
			}
			dwTick1 = GetTickCount();
		}
		*pos = 0;
		return nRead;
	}
};

//-----------------------------------------------------------------------------
class CFGComm : public IFGComm {
	// Comm over CameraLink
protected:
	CXComm m_comm;
public:
public:
	CFGComm() {}
	virtual ~CFGComm() { CloseComm(); }

public:
	virtual BOOL IsOpen() const { return m_comm.IsOpen(); }
	virtual BOOL OpenComm(LPCTSTR pszPort = NULL, DWORD baud = 9600, BYTE ByteSize = 8, BYTE Parity = NOPARITY, BYTE StopBits = ONESTOPBIT, BYTE eXOnOff = 0) {
		if (!m_comm.Open(pszPort, baud, TRUE, ByteSize, Parity, StopBits, RTS_CONTROL_DISABLE, DTR_CONTROL_DISABLE, eXOnOff))
			return FALSE;
		m_comm.Purge();
		return TRUE;
	}
	virtual BOOL CloseComm() {
		return m_comm.Close();
	}
	virtual BOOL WriteComm(const void* buf, DWORD nSize, DWORD dwTimeout = INFINITE) {
		return (m_comm.Write(buf, nSize, dwTimeout) == nSize);
	}
	virtual BOOL ReadComm1byte(BYTE& c, DWORD dwTimeout = INFINITE) {
		if (!m_comm.IsOpen())
			return FALSE;
		return (m_comm.Read(&c, 1, dwTimeout) == 1);
	}
	virtual int ReadComm(void* buf, int nMaxSize, const char* pszEnd = NULL, DWORD dwTimeout = INFINITE) {
		if (!m_comm.IsOpen())
			return FALSE;
		if (pszEnd && pszEnd[0] != 0)
			return ReadCommLine(buf, nMaxSize, pszEnd, dwTimeout);

		return (int)m_comm.Read(buf, nMaxSize, dwTimeout);
	}
	virtual void PurgeComm(DWORD dwFlags = PURGE_RXABORT|PURGE_RXCLEAR|PURGE_TXABORT|PURGE_TXCLEAR) {
		if (!m_comm.IsOpen())
			return;
		m_comm.Purge(dwFlags);
	}
};

//-----------------------------------------------------------------------------

#pragma pack(pop)
