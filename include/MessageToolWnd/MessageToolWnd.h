#pragma once

#include "AFX_EXT_MESSAGE_TOOL_WND.h"

#include "MIP/MIP.h"
#include "MessageTool.hpp"
#include "MessageWnd.h"
#define TIMER_MOCHA_CONNECT	7979
class CMessageWnd;
class AFX_EXT_API_MESSAGE_TOOL_WND CMessageToolWnd : public CWnd, public IWNDIPCommandTarget {
	friend class CMessageThread;
	DECLARE_DYNCREATE(CMessageToolWnd)

public:
	CMessageToolWnd(CWnd* pwndParent = NULL);							 // protected constructor used by dynamic creation
	virtual					~CMessageToolWnd();

public:
	static TRefPointer<CIPClient> s_rIPClient;

public:
	DECLARE_MESSAGE_MAP()
	afx_msg void OnDestroy();
	afx_msg void OnClose();
	afx_msg void OnTimer(UINT_PTR nIDEvent);

protected:
	CWnd* m_pwndParent;
	DECLARE_IP()
	DECLARE_IP_INTERFACE()

	ip_handler BOOL OnIPMessage(CIPCommand& cmd);
	ip_handler BOOL OnIPResponse(CIPCommand& cmd);
	ip_handler BOOL OnIPInput(CIPCommand& cmd);
	ip_handler BOOL OnIPAnimatioin(CIPCommand& cmd);

	void SetMessageData(const CIPVar& var, T_MESSAGE_DATA& tMD);

	TList<CMessageWnd> m_tMessageWnd;
	TList<CMessageWnd> m_tResponseWnd;
	TList<CMessageWnd> m_tInputWnd;
	TList<CMessageWnd> m_tAnimationWnd;
	int	m_iWndCount = 0;
public:
	void Initialize();
	void CalcWndStartPos(CSize& szStart, CSize szWnd, CRect& rect);
};

/*
	- ON_IP("Message", "Message", OnIPMessage) // 단순 Message 출력.
		var :
			"Width" : int, defaule : 600 -> MessageWnd Width.
			"Height" : int, defaule : 230 -> MessageWnd Height.
			"Title" : CString, default : _T("Title")-> Message SetWindowText;

			"Font" : CString, default : _T("맑은고딕") -> Message Font;
			"FontSize" : int, default : 10 -> Message 글씨 크기;
			"FontWeight" : int, default : 300 -> Message 글씨 굵기;
			"TextColor" : COLORREF, default : RGB_BLACK -> 글자색;
			"BkColor" : COLORREF, default : GetSysColor(CTLCOLOR_DLG) -> 배경색
			"PenColor" : COLORREF, default : RGB_BLACK -> 펜색;

			"Message" : CString, default : _T("Message") -> Message 내용.
			"Button" : CString, default : _T("Ok") -> 버튼 내용. ex)Ok;Calce 와 같이 _T(";")로 구분되며 4개까지 입력 가능.
			"Unit" : CString, default : _T("") -> Unit 구분 시 사용.
			"Timeout" : int, default : INFINITE -> Message 유지 시간 (msec). 버튼 클릭시 시간과 상관없이 종료됨.

		varReault : 없음.

	- ON_IP("Message", "Response", OnIPResponse) // Message 출력 및 버튼 응답.
		var :
			"Width" : int, defaule : 600 -> MessageWnd Width.
			"Height" : int, defaule : 230 -> MessageWnd Height.
			"Title" : CString, default : _T("Title")-> Message SetWindowText;

			"Font" : CString, default : _T("맑은고딕") -> Message Font;
			"FontSize" : int, default : 10 -> Message 글씨 크기;
			"FontWeight" : int, default : 300 -> Message 글씨 굵기;
			"TextColor" : COLORREF, default : RGB_BLACK -> 글자색;
			"BkColor" : COLORREF, default : GetSysColor(CTLCOLOR_DLG) -> 배경색
			"PenColor" : COLORREF, default : RGB_BLACK -> 펜색;

			"Message" : CString, default : _T("Message") -> Message 내용.
			"Button" : CString, default : _T("Ok") -> 버튼 내용. ex)Ok;Calce 와 같이 _T(";")로 구분되며 4개까지 입력 가능.
			"Unit" : CString, default : _T("") -> Unit 구분 시 사용.
			"Timeout" : int, default : INFINITE -> Message 유지 시간 (msec). 버튼 클릭시 시간과 상관없이 종료됨.

		varReault : 
			"ButtonResult" : CString, 선택한 버튼의 내용.

	- ON_IP("Message", "Input", OnIPInput) // Message 출력 및 입력 된 내용 응답.
		var :
			"Width" : int, defaule : 600 -> MessageWnd Width.
			"Height" : int, defaule : 230 -> MessageWnd Height.
			"Title" : CString, default : _T("Title")-> Message SetWindowText;

			"Font" : CString, default : _T("맑은고딕") -> Message Font;
			"FontSize" : int, default : 10 -> Message 글씨 크기;
			"FontWeight" : int, default : 300 -> Message 글씨 굵기;
			"TextColor" : COLORREF, default : RGB_BLACK -> 글자색;
			"BkColor" : COLORREF, default : GetSysColor(CTLCOLOR_DLG) -> 배경색
			"PenColor" : COLORREF, default : RGB_BLACK -> 펜색;

			"Message" : CString, default : _T("Message") -> Message 내용.
			"Button" : CString, default : _T("Ok") -> 버튼 내용. ex)Ok;Calce 와 같이 _T(";")로 구분되며 4개까지 입력 가능. (Input의 경우 4번째 버튼 1개만 출력.)
			"Unit" : CString, default : _T("") -> Unit 구분 시 사용.
			"Timeout" : int, default : INFINITE -> Message 유지 시간 (msec). 버튼 클릭시 시간과 상관없이 종료됨.
			"Password" : BOOL, defalult : FALSE -> 입력 받을 내용을 Password로 표시할 지 여부.

		varReault :
			"InputResult" : CString, 입력한 내용을 Return.\.


	- ON_IP("Message", "Animation", OnIPAnimatioin) // 여러개의 Message를 일정 간격마다 변경 하여 출력.
		var :
			"Width" : int, defaule : 600 -> MessageWnd Width.
			"Height" : int, defaule : 230 -> MessageWnd Height.
			"Title" : CString, default : _T("Title")-> Message SetWindowText;

			"Font" : CString, default : _T("맑은고딕") -> Message Font;
			"FontSize" : int, default : 10 -> Message 글씨 크기;
			"FontWeight" : int, default : 300 -> Message 글씨 굵기;
			"TextColor" : COLORREF, default : RGB_BLACK -> 글자색;
			"BkColor" : COLORREF, default : GetSysColor(CTLCOLOR_DLG) -> 배경색
			"PenColor" : COLORREF, default : RGB_BLACK -> 펜색;

			"Message" : CString, default : _T("Message") -> Message 내용. ex)Message 1;Message 2;Message 3 와 같이 _T(";")로 구분되며 입력 된 갯수 * "AnimationInterval" 만큼 Message가 유지됨.
			"Button" : CString, default : _T("Ok") -> 버튼 내용. ex)Ok;Calce 와 같이 _T(";")로 구분되며 4개까지 입력 가능. (Input의 경우 4번째 버튼 1개만 출력.)
			"Unit" : CString, default : _T("") -> Unit 구분 시 사용.
			"AnimationInterval" : int, default : 1000 msec -> Message내용 변경 시간.
		varReault : 없음.
*/
