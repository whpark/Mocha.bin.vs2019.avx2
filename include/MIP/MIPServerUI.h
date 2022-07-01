// MIPServerUI.h : main header file for the MIPServerUI DLL
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "AFX_EXT_IPSERVER_UI.h"

#include "MIP/IPServer.h"

#ifdef _EXPORTS_MIPSERVER_UI

	class CMIPServerUIApp : public CWinAppEx {
	public:
		CMIPServerUIApp();
	
	// Overrides
	public:
		CEvent m_evtStarted;
		CIPServer* m_pIPServer;
		bool m_bDebugMode = false;

		virtual BOOL InitInstance();
	
		DECLARE_MESSAGE_MAP()
	
		afx_msg void OnAppAbout();
	};

	extern CMIPServerUIApp theApp;

#endif

struct T_IP_SERVER_TASK_PARAM {
	CString strServerTaskName;
	CString strPathProfile;
	BOOL bExecuteProcess = true;
	CIPServer* pIPServer = nullptr;
	HANDLE hEvtCreated = nullptr;
	BOOL bError = false;
	BOOL* pbResult = nullptr;
	BOOL bStandalone = false;

	T_IP_SERVER_TASK_PARAM(LPCTSTR pszServerTaskName = _T("mocha"), LPCTSTR pszPathProfile = nullptr, BOOL _bExecuteProcess = true)
		: strServerTaskName(pszServerTaskName), strPathProfile(pszPathProfile), bExecuteProcess(_bExecuteProcess)
	{
	}
};

AFX_EXT_CLASS_MIPSERVER_UI CIPServer* IPStartServerUI(const T_IP_SERVER_TASK_PARAM& param);
AFX_EXT_CLASS_MIPSERVER_UI CIPServer* IPStartServerUI(LPCTSTR pszServerTaskName = _T("mocha"), LPCTSTR pszPathProfile = NULL, BOOL bExecuteProcess = TRUE);
