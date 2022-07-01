#pragma once

#include "misc/DrawSymbol.h"
#include "MIP/MIP.h"

// Message Tool

#pragma pack(push, 8)

struct T_MESSAGE_DATA {
	// Input Data
	CSize szStart = CSize(GetSystemMetrics(SM_CXSCREEN)/2, GetSystemMetrics(SM_CYSCREEN) / 2);
	CSize szWnd = CSize(600, 230);
	CString strFont = CString(_T("Segoe UI"));
	int iFontSize = 10;
	int iFontWeight = 400;
	COLORREF crText = RGB_BLACK;
	COLORREF crBk = GetSysColor(CTLCOLOR_DLG);
	COLORREF crPen = RGB_BLACK;

	BOOL bPassWord = FALSE;
	BOOL bInput = FALSE;
	BOOL bAnimation = FALSE;
	BOOL bResponse = FALSE;

	int iAnimationInterval = 1000; // 1 sec
	int iTimeout = INFINITE;

	CString strTitle = CString(_T("Title"));
	CString strMessage = CString(_T("Message"));
	CString strButton = CString(_T("Ok"));
	CString strTask;
	CString strUnit;
	CString strInput;

};


class CMessageFunction {
public: 
	void SendIPMessage(CIPClient& ipClient, T_MESSAGE_DATA tMD) {
		CIPVar var;
		var.SetChildItem("Width", (int)tMD.szWnd.cx);
		var.SetChildItem("Height", (int)tMD.szWnd.cy);
		var.SetChildItem("Title", tMD.strTitle);

		var.SetChildItem("Font", tMD.strFont);
		var.SetChildItem("FontSize", tMD.iFontSize);
		var.SetChildItem("FontWeight", tMD.iFontWeight);

		var.SetChildItem("TextColor", tMD.crText);
		var.SetChildItem("BkColor", tMD.crBk);
		var.SetChildItem("PenColor", tMD.crPen);

		var.SetChildItem("Message", tMD.strMessage);
		var.SetChildItem("Button", tMD.strButton);
		var.SetChildItem("Unit", tMD.strUnit);
		var.SetChildItem("Timeout", tMD.iTimeout);

		ipClient.PostIPCommand("Message", NULL, "Message",  "Message", var);

		return;
	}

	CString	ShowResMessage(CIPClient& ipClient, T_MESSAGE_DATA tMD) {
		CIPVar var, varResult;
		var.SetChildItem("Width", (int)tMD.szWnd.cx);
		var.SetChildItem("Height", (int)tMD.szWnd.cy);
		var.SetChildItem("Title", tMD.strTitle);

		var.SetChildItem("Font", tMD.strFont);
		var.SetChildItem("FontSize", tMD.iFontSize);
		var.SetChildItem("FontWeight", tMD.iFontWeight);

		var.SetChildItem("TextColor", tMD.crText);
		var.SetChildItem("BkColor", tMD.crBk);
		var.SetChildItem("PenColor", tMD.crPen);

		var.SetChildItem("Message", tMD.strMessage);
		var.SetChildItem("Button", tMD.strButton);
		var.SetChildItem("Unit", tMD.strUnit);
		var.SetChildItem("Timeout", tMD.iTimeout);

		ipClient.SendIPCommand("Message", NULL, "Message", "Response", var, varResult);

		return varResult.GetChildItem("ButtonResult", _T(""));
	};

	void ShowResMessageCmd(PIPCOMMAND cmd, CIPClient& ipClient, T_MESSAGE_DATA tMD) {
		CIPVar var, varResult;
		var.SetChildItem("Width", (int)tMD.szWnd.cx);
		var.SetChildItem("Height", (int)tMD.szWnd.cy);
		var.SetChildItem("Title", tMD.strTitle);

		var.SetChildItem("Font", tMD.strFont);
		var.SetChildItem("FontSize", tMD.iFontSize);
		var.SetChildItem("FontWeight", tMD.iFontWeight);

		var.SetChildItem("TextColor", tMD.crText);
		var.SetChildItem("BkColor", tMD.crBk);
		var.SetChildItem("PenColor", tMD.crPen);

		var.SetChildItem("Message", tMD.strMessage);
		var.SetChildItem("Button", tMD.strButton);
		var.SetChildItem("Unit", tMD.strUnit);
		var.SetChildItem("Timeout", tMD.iTimeout);

		ipClient.SendIPStepCommand(cmd, "Message", NULL, "Message", "Response", var);
		
	};
	CString SendInPutMessage(CIPClient& ipClient, T_MESSAGE_DATA tMD) {
		CIPVar var, varResult;
		var.SetChildItem("Width", (int)tMD.szWnd.cx);
		var.SetChildItem("Height", (int)tMD.szWnd.cy);
		var.SetChildItem("Title", tMD.strTitle);

		var.SetChildItem("Font", tMD.strFont);
		var.SetChildItem("FontSize", tMD.iFontSize);
		var.SetChildItem("FontWeight", tMD.iFontWeight);

		var.SetChildItem("TextColor", tMD.crText);
		var.SetChildItem("BkColor", tMD.crBk);
		var.SetChildItem("PenColor", tMD.crPen);

		var.SetChildItem("Message", tMD.strMessage);
		var.SetChildItem("Button", tMD.strButton);
		var.SetChildItem("Unit", tMD.strUnit);
		var.SetChildItem("Timeout", tMD.iTimeout);
		var.SetChildItem("Password", tMD.bPassWord);

		ipClient.SendIPCommand("Message", NULL, "Message", "Input", var, varResult);

		return varResult.GetChildItem("InputResult", _T(""));
	}

	void SendAnimationMessage(CIPClient& ipClient, T_MESSAGE_DATA tMD, CIPCommand* pCMD) {
		CIPVar var;
		var.SetChildItem("Width", (int)tMD.szWnd.cx);
		var.SetChildItem("Height", (int)tMD.szWnd.cy);
		var.SetChildItem("Title", tMD.strTitle);

		var.SetChildItem("Font", tMD.strFont);
		var.SetChildItem("FontSize", tMD.iFontSize);
		var.SetChildItem("FontWeight", tMD.iFontWeight);

		var.SetChildItem("TextColor", tMD.crText);
		var.SetChildItem("BkColor", tMD.crBk);
		var.SetChildItem("PenColor", tMD.crPen);

		var.SetChildItem("Message", tMD.strMessage);
		var.SetChildItem("Button", tMD.strButton);
		var.SetChildItem("Unit", tMD.strUnit);
		//var.SetChildItem("Timeout", tMD.iTimeout);
		var.SetChildItem("AnimationInterval", tMD.iAnimationInterval);

		if (!pCMD)
			ipClient.PostIPCommand("Message", NULL, "Message", "Animation", var);
		else
			ipClient.SendIPStepCommand(pCMD, "Message", NULL, "Message", "Animation", var);

		return;
	}
};

#pragma pack(pop)
