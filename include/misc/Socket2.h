#ifndef __SOCKET_2_H__
#define __SOCKET_2_H__

#include "AFX_EXT_MISC.h"
#include <afx.h>
#include <WS2tcpip.h>

#pragma pack(push, 8)

class AFX_EXT_CLASS_MISC CSocket2 {
private:
	CSocket2(const CSocket2& rSrc);    // no implementation
	void operator=(const CSocket2& rSrc);  // no implementation

// Construction
public:
	static BOOL InitSocket2(CString& strErrorMessage);
	CSocket2();
	BOOL Create(UINT nSocketPort = 0, int nSocketType=SOCK_STREAM, LPCTSTR lpszSocketAddress = NULL);

// Attributes
public:
	SOCKET m_hSocket;

	operator SOCKET() const;
	BOOL Attach(SOCKET hSocket);
	SOCKET Detach();

	BOOL GetPeerName(CString& rPeerAddress, UINT& rPeerPort);
	BOOL GetPeerName(SOCKADDR* lpSockAddr, int* lpSockAddrLen);

	BOOL GetSockName(CString& rSocketAddress, UINT& rSocketPort);
	BOOL GetSockName(SOCKADDR* lpSockAddr, int* lpSockAddrLen);

	BOOL SetSockOpt(int nOptionName, const void* lpOptionValue, int nOptionLen, int nLevel = SOL_SOCKET);
	BOOL GetSockOpt(int nOptionName, void* lpOptionValue, int* lpOptionLen, int nLevel = SOL_SOCKET);

	static int PASCAL GetLastError();

// Operations
public:

	virtual BOOL Accept(CSocket2& rConnectedSocket, SOCKADDR* lpSockAddr = NULL, int* lpSockAddrLen = NULL);

	BOOL Bind(UINT nSocketPort, LPCTSTR lpszSocketAddress = NULL);
	BOOL Bind (const SOCKADDR* lpSockAddr, int nSockAddrLen);

	virtual void Close();

	BOOL Connect(LPCTSTR lpszHostAddress, UINT nHostPort);
	BOOL Connect(const SOCKADDR* lpSockAddr, int nSockAddrLen);

	BOOL IOCtl(long lCommand, DWORD* lpArgument);

	BOOL Listen(int nConnectionBacklog=5);

	virtual BOOL Read(void* lpBuf, size_t nSize, DWORD dwTimeout = 0/*Timeout NOT Implemented.*/);
	virtual BOOL Write(const void* lpBuf, size_t nSize, size_t nBlockSize = 8*1024, DWORD dwTimeout = 0/*Timeout NOT Implemented.*/);
	virtual int Receive(void* lpBuf, int nBufLen, int nFlags = 0);

	int ReceiveFrom(void* lpBuf, int nBufLen, CString& rSocketAddress, UINT& rSocketPort, int nFlags = 0);
	int ReceiveFrom(void* lpBuf, int nBufLen, SOCKADDR* lpSockAddr, int* lpSockAddrLen, int nFlags = 0);

	enum { receives = 0, sends = 1, both = 2 };
	BOOL ShutDown(int nHow = sends);

	virtual int Send(const void* lpBuf, int nBufLen, int nFlags = 0);

	int SendTo(const void* lpBuf, int nBufLen, UINT nHostPort, LPCTSTR lpszHostAddress = NULL, int nFlags = 0);
	int SendTo(const void* lpBuf, int nBufLen, const SOCKADDR* lpSockAddr, int nSockAddrLen, int nFlags = 0);

// Implementation
public:
	virtual ~CSocket2();
	BOOL Socket(int nSocketType=SOCK_STREAM, int nProtocolType = 0, int nAddressFormat = PF_INET);

protected:
	virtual BOOL ConnectHelper(const SOCKADDR* lpSockAddr, int nSockAddrLen);
	virtual int ReceiveFromHelper(void* lpBuf, int nBufLen, SOCKADDR* lpSockAddr, int* lpSockAddrLen, int nFlags);
	virtual int SendToHelper(const void* lpBuf, int nBufLen, const SOCKADDR* lpSockAddr, int nSockAddrLen, int nFlags);
};

inline int CSocket2::ReceiveFromHelper(void* lpBuf, int nBufLen, SOCKADDR* lpSockAddr, int* lpSockAddrLen, int nFlags) {
	return recvfrom(m_hSocket, (LPSTR)lpBuf, nBufLen, nFlags, lpSockAddr, lpSockAddrLen);
}

inline int CSocket2::SendToHelper(const void* lpBuf, int nBufLen, const SOCKADDR* lpSockAddr, int nSockAddrLen, int nFlags) {
	return sendto(m_hSocket, (LPSTR)lpBuf, nBufLen, nFlags, lpSockAddr, nSockAddrLen);
}

inline BOOL CSocket2::ConnectHelper(const SOCKADDR* lpSockAddr, int nSockAddrLen) {
	return connect(m_hSocket, lpSockAddr, nSockAddrLen) != SOCKET_ERROR;
}

inline CSocket2::operator SOCKET() const
	{ return m_hSocket; }
inline BOOL CSocket2::GetPeerName(SOCKADDR* lpSockAddr, int* lpSockAddrLen)
	{ return (SOCKET_ERROR != getpeername(m_hSocket, lpSockAddr, lpSockAddrLen)); }
inline BOOL CSocket2::GetSockName(SOCKADDR* lpSockAddr, int* lpSockAddrLen)
	{ return (SOCKET_ERROR != getsockname(m_hSocket, lpSockAddr, lpSockAddrLen)); }
inline BOOL CSocket2::SetSockOpt(int nOptionName, const void* lpOptionValue, int nOptionLen, int nLevel)
	{ return (SOCKET_ERROR != setsockopt(m_hSocket, nLevel, nOptionName, (LPCSTR)lpOptionValue, nOptionLen)); }
inline BOOL CSocket2::GetSockOpt(int nOptionName, void* lpOptionValue, int* lpOptionLen, int nLevel)
	{ return (SOCKET_ERROR != getsockopt(m_hSocket, nLevel, nOptionName, (LPSTR)lpOptionValue, lpOptionLen)); }
inline int PASCAL CSocket2::GetLastError()
	{ return WSAGetLastError(); }
inline BOOL CSocket2::Bind(const SOCKADDR* lpSockAddr, int nSockAddrLen)
	{ return (SOCKET_ERROR != bind(m_hSocket, lpSockAddr, nSockAddrLen)); }
inline BOOL CSocket2::Connect(const SOCKADDR* lpSockAddr, int nSockAddrLen)
	{ return ConnectHelper(lpSockAddr, nSockAddrLen); }
inline BOOL CSocket2::IOCtl(long lCommand, DWORD* lpArgument)
	{ return (SOCKET_ERROR != ioctlsocket(m_hSocket, lCommand, lpArgument)); }
inline BOOL CSocket2::Listen(int nConnectionBacklog)
	{ return (SOCKET_ERROR != listen(m_hSocket, nConnectionBacklog)); }
inline int CSocket2::ReceiveFrom(void* lpBuf, int nBufLen, SOCKADDR* lpSockAddr, int* lpSockAddrLen, int nFlags)
	{ return ReceiveFromHelper(lpBuf, nBufLen, lpSockAddr, lpSockAddrLen, nFlags); }
inline BOOL CSocket2::ShutDown(int nHow)
	{ return (SOCKET_ERROR != shutdown(m_hSocket,nHow)); }
inline int CSocket2::SendTo(const void* lpBuf, int nBufLen, const SOCKADDR* lpSockAddr, int nSockAddrLen, int nFlags)
	{ return SendToHelper(lpBuf, nBufLen, lpSockAddr, nSockAddrLen, nFlags); }

inline CString __inet_ntop(in_addr& addr) {
#if ( (_MSC_VER >= 1600) && (_WIN32_WINNT > _WIN32_WINNT_VISTA) )
	CStringA strBuf;
	inet_ntop(AF_INET, &addr, strBuf.GetBuffer(256), 256);
	strBuf.ReleaseBuffer();
	return CString(strBuf);
#else
	return CString(inet_ntoa(addr));
#endif
}

inline unsigned long __inet_pton(LPCSTR pszAddress) {
#if ( (_MSC_VER >= 1600) && (_WIN32_WINNT > _WIN32_WINNT_VISTA) )
	IN_ADDR addr;
	if (!inet_pton(AF_INET, pszAddress, &addr))
		return 0;
	return addr.s_addr;
#else
	return inet_addr(pszAddress);
#endif
}

#pragma pack(pop)

#endif // __ASYNC_SOCKET_EX_H__
