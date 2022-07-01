#ifndef __SOCKET_FILE_2_H__
#define __SOCKET_FILE_2_H__
#include <afx.h>

#include "AFX_EXT_MISC.h"

/////////////////////////////////////////////////////////////////////////////
// CSocketFile2

#if (_MFC_VER >= 0x0700)
	#define FILE_POSITION_TYPE			LONGLONG
	#define FILE_POSITION_TYPE_UNSIGNED ULONGLONG
#else
	#define FILE_POSITION_TYPE			LONG
	#define FILE_POSITION_TYPE_UNSIGNED ULONG
#endif

class CSocket2;
class AFX_EXT_CLASS_MISC CSocketFile2 : public CFile {
	DECLARE_DYNAMIC(CSocketFile2)
public:
//Constructors
	explicit CSocketFile2(CSocket2* pSocket, BOOL bArchiveCompatible = TRUE);

// Implementation
public:
	CSocket2* m_pSocket;
	BOOL m_bArchiveCompatible;

	virtual ~CSocketFile2();

	virtual UINT GetBufferPtr(UINT nCommand, UINT nCount, void** ppBufStart, void** ppBufMax);
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
	virtual UINT Read(void* lpBuf, UINT nCount);
	virtual void Write(const void* lpBuf, UINT nCount);
	virtual void Close();

// Unsupported APIs
	virtual BOOL Open(LPCTSTR lpszFileName, UINT nOpenFlags, CFileException* pError = NULL);
	virtual CFile* Duplicate() const;

	virtual FILE_POSITION_TYPE_UNSIGNED GetPosition() const;

	virtual
#if (_MFC_VER >= 0x0700)
		FILE_POSITION_TYPE_UNSIGNED 
#else
		FILE_POSITION_TYPE
#endif
		Seek(FILE_POSITION_TYPE lOff, UINT nFrom);

	virtual void SetLength(FILE_POSITION_TYPE_UNSIGNED dwNewLen);
	virtual FILE_POSITION_TYPE_UNSIGNED GetLength() const;
	virtual void LockRange(FILE_POSITION_TYPE_UNSIGNED dwPos, FILE_POSITION_TYPE_UNSIGNED dwCount);
	virtual void UnlockRange(FILE_POSITION_TYPE_UNSIGNED dwPos, FILE_POSITION_TYPE_UNSIGNED dwCount);

	virtual void Flush();
	virtual void Abort();
};

#endif // __SOCKET_FILE_2_H__
