#pragma once

#include "default.defines.h"
//#include "AFX_EXT_FRAME_GRABBER.h"
#include "FGComm.h"
#include "MIP/IPVariable.h"

#pragma pack(push, 8)

////-----------------------------------------------------------------------------
////
//class IFGCameraCommand {
//protected:
//	CString m_strName;
//	CStringA m_strCmdFmt;
//	CString m_strComment;
//
//public:
//	IFGCameraCommand(LPCTSTR pszName, LPCSTR pszCmdFmt) : m_strName(pszName), m_strCmdFmt(pszCmdFmt) { }
//	virtual ~IFGCameraCommand(void) { }
//
//	virtual BOOL MakeCommand(std::vector<BYTE>& cmd, const CIPVariable& varIn) = NULL;
//	virtual BOOL ParseCommand(const std::vector<BYTE>& cmd, CIPVariable& varOut) = NULL;
//};

//=============================================================================
// IFGCamera
//
class IFGCamera {
public:
	DECLARE_DYNAMIC_CREATE_BASE_INTERFACE(IFGCamera, LPCSTR, CStringA)

	enum { nDefaultTimeout = 1000, };

protected:
	TRefPointer<IFGComm> m_rComm;
	//TList<IFGCameraCommand> m_cmds;
	//CStringA m_strPreSend, m_strPostSend;
	//CStringA m_strPreRecv, m_strPostRecv;
protected:
	CWnd* m_pDialog;
public:
	IFGCamera() : m_pDialog(NULL) {}
	virtual ~IFGCamera(void) { SAFE_DELETE(m_pDialog); }

public:
	void SetComm(TRefPointer<IFGComm> rComm) { m_rComm = rComm; }
	void ReleaseComm() { m_rComm.Release(); }
	IFGComm* GetComm() { return m_rComm; }
	//void SetDelimiter(LPCSTR pszPreSend = NULL, LPCSTR pszPostSend = NULL, LPCSTR pszPreRecv = NULL, LPCSTR pszPostRecv = NULL) {
	//	m_strPreSend = pszPreSend; m_strPostSend = pszPostSend;
	//	m_strPreRecv = pszPreRecv; m_strPostRecv = pszPostRecv;
	//}

public:
	virtual CWnd* GetDialog(CWnd* pWndParent) = NULL;

public:
	virtual void Init() { }

	//virtual BOOL DoCommand(LPCTSTR pszCameraCommandName, const CIPVariable* pVarIn = NULL, CIPVariable* pVarOut = NULL, DWORD dwTimeout = 1000);
	virtual BOOL DoCommand(const TBuffer<BYTE>& send, TBuffer<BYTE>* pReceived = NULL, DWORD dwTimeout = nDefaultTimeout) = NULL;
	virtual BOOL DoCommand(LPCSTR pszCmd, CStringA& strResult, DWORD dwTimeout = nDefaultTimeout) {
		TBuffer<BYTE> bufCmd, bufResult;
		StringToBuffer(pszCmd, bufCmd);
		if (!DoCommand(bufCmd, &bufResult, dwTimeout))
			return FALSE;
		BufferToString(bufResult, strResult);
		return TRUE;
	}
	virtual BOOL GetResult(const TBuffer<BYTE>& received, CIPVariable& varOut) = NULL;

public:
	static void StringToBuffer(LPCSTR sz, TBuffer<BYTE>& buf) {
		buf.SetData((intXX_t)strlen(sz), (BYTE*)sz);
	}
	static void BufferToString(const TBuffer<BYTE>& buf, CStringA& str) {
		if (!buf.GetSize()) {
			str.Empty();
			return;
		}
		char* pos = str.GetBuffer(buf.GetSize()+1);
		memcpy(pos, buf, buf.GetSize()); pos[buf.GetSize()] = 0;
		str.ReleaseBuffer();
	}

};


//=============================================================================

#pragma pack(pop)
