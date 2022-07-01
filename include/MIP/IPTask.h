#pragma once

#include "AFX_EXT_MIP.h"

#include "misc/Socket2.h"
#include "misc/SimpleLog.h"
#include "misc/Profile.h"

#include "IPCommandTarget.h"

#ifndef SC_CLOSE_FORCED
#define SC_CLOSE_FORCED (SC_CLOSE+1)
#endif

#pragma pack(push, 8)

class CIPCommand;
class CIPConnection;
class CIPCommandPipe;
class IIPTask;

//=============================================================================
// IIPTask
//

class AFX_EXT_CLASS_MIP IIPTask {
	friend class CIPCommandPipe;
protected:
	CString m_strStartingFolder;
	DStringA m_strTask;
	CString m_strFolderProfile;
	CString m_strPathProfile;
	CString m_strServerIP;
	unsigned short m_nServerPort;
	CString m_strLogPath;
	CString m_strLogFile;
	CProfile m_profile;
	HWND m_hMainWnd;
	TList<CSimpleLog> m_logs;	// Log for each IPHandler

	BOOL m_bQuitInProgress;
public:
	IIPTask();
	~IIPTask();

public:
	virtual BOOL LoadProfile(LPCTSTR pszPathProfile = NULL);
	BOOL Init(LPCTSTR pszTask = NULL, LPCTSTR pszPathProfile = NULL, BOOL BTaskNameFromArgv0 = FALSE);

	CString GetTaskName() const { return CString((LPCSTR)m_strTask); }
	LPCTSTR GetProfilePath() const { return m_strPathProfile; }
	LPCTSTR GetServerIP() const { return m_strServerIP; }
	unsigned short GetServerPort() const { return m_nServerPort; }
	LPCTSTR GetLogPath() const { return m_strLogPath; }
	LPCTSTR GetLogFile() const { return m_strLogFile; }
	CProfile& GetProfile() { return m_profile; }
	const CProfile& GetProfile() const { return m_profile; }

	void SetMainWnd(HWND hWnd) { m_hMainWnd = hWnd; }
	HWND GetMainWnd() const { return m_hMainWnd; }

	// misc
	LPCTSTR GetStartingFolder() const { return m_strStartingFolder; }
	LPCTSTR GetStartingDirectory() const { return m_strStartingFolder; }
	CString GetAbsPath(LPCTSTR pszRelativePath) const;
	CString GetRelPath(LPCTSTR pszFullPath) const;

	CSimpleLog* GetLogObject(LPCTSTR pszPathTemplate);

	// Interfaces
protected:
	virtual BOOL OnConnected(CIPCommandPipe* pCommandPipe, HIPCOMMAND& hCMD) = NULL;	// For Server Only
	virtual BOOL OnDisconnected(CIPCommandPipe* pCommandPipe) = NULL;

public:
	virtual BOOL OnIPCommandReceived(CIPCommandPipe* pCommandPipe, HIPCOMMAND& hCMD, BOOL& bClosed) = NULL;
	virtual BOOL OnIPCommandAcked(CIPCommandPipe* pCommandPipe, HIPCOMMAND& hCMD) = NULL;
	virtual BOOL OnIPCommandReplied(CIPCommandPipe* pCommandPipe, HIPCOMMAND& hCMD) = NULL;

	// Log
public:
	CSimpleLog m_log;

protected:
	static CString s_strFolderProfile;
	static BOOL GetInfoFromCmdLine(LPCTSTR pszCmdLine, CString& strTaskName, CString& strPathProfile, BOOL bTaskNameFromArgv0 = FALSE);
	static BOOL DeletePlatformSign(CString& strTitle, BOOL bDeleteAllSign = FALSE);
};

#pragma pack(pop)
