#pragma once

#include "misc/TMDialog.h"
#include <afxmt.h>

class CProgressDlg;
class CProgressThread;

//=============================================================================
// CProgressThread
class AFX_EXT_CLASS_MISC CProgressThread : public CWinThread {
	DECLARE_DYNCREATE(CProgressThread)

protected:
	CProgressThread();           // protected constructor used by dynamic creation
	virtual ~CProgressThread();

public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

	CEvent m_evtCreated;
	CProgressDlg* m_pDLG;

protected:
	DECLARE_MESSAGE_MAP()
};

//-----------------------------------------------------------------------------
// CProgressDlg dialog
class AFX_EXT_CLASS_MISC CProgressDlg : public CMDialogEx {
	DECLARE_DYNAMIC(CProgressDlg)

public:
	CProgressDlg(CEvent* pEvent, CWnd* pParent = NULL);   // standard constructor
	virtual ~CProgressDlg();

//// Dialog Data
//	enum { IDD = "IDD_PROGRESS" };

public:
	void SetRange(int iMin, int iMax);
	void GetRange(int& dwMin, int& iMax);
	int GetPos();
	void SetPos(int iCurrentPos, BOOL bUpdateText = FALSE);
	void GetText(CString& strText);
	void SetText(LPCTSTR pszText);
	void CloseDialog();

protected:
	CProgressCtrl m_progress;
	CEvent* m_pEvent;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	virtual void OnOK() {}
	virtual void OnCancel() {}

	DECLARE_MESSAGE_MAP()
	afx_msg LRESULT OnCmdProgress(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnCloseDialog(WPARAM wParam, LPARAM lParam);

};
