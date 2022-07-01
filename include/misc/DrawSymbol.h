#ifndef __DRAWSYMBOL_H__SEONGHYEONG_LEE__
#define __DRAWSYMBOL_H__SEONGHYEONG_LEE__

#pragma once


#include					"AFX_EXT_MISC.h"


#define		MAX_PEN			10
#define		MAX_BRUSH		7
#define		MAX_FONT		10
#define		MAX_EVENT_COLOR	10

#define	RGB_WHITE			RGB(255,255,255)
#define	RGB_BLACK			RGB(  0,  0,  0)
#define	RGB_GRAY			RGB(125,125,125)
#define	RGB_GRAY_DARK		RGB( 75, 75, 75)
#define	RGB_GRAY_LIGHT		RGB(200,200,200)
#define	RGB_RED				RGB(255,  0,  0)
#define	RGB_GREEN			RGB(  0,255,  0)
#define	RGB_BLUE			RGB(  0,  0,255)
#define RGB_CYAN			RGB(  0,255,255)
#define	RGB_YELLOW			RGB(255,255,  0)
#define	RGB_ORANGE			RGB(255, 50,  0)
#define RGB_VIOLET			RGB(180,  0,  180)
#define	RGB_SKY				RGB(120,120,255)

// 2013.12.19.		Start	Add By	SeongHyeong, Lee
enum eEVENT_COLOR {
	eEC_DEFAULT,
	eEC_DISABLE,
	eEC_NORMAL,
	eEC_MOUSE_MOVE,
	eEC_MOUSE_DOWN,
	eEC_MOUSE_UP,
	eEC_SELECTED,
	eEC_7,
	eEC_8,
	eEC_END,
};
enum eRECT_AREA {
	eAREA_FULL	= 0,
	eAREA_TITLE,
	eAREA_MAIN_MENU,
	eAREA_SUB_MENU,
	eAREA_WORK,
	eAREA_VISION,
	eAREA_6,
	eAREA_7,
	eAREA_8,
	eAREA_9,
	eAREA_END,
};
// 2014.03.10.		End

#pragma pack(push, 4/*MFC*/)

class AFX_EXT_CLASS_MISC CDrawSymbol  
{
public:
	enum ePEN_COLOR{ 
				ePEN_WHITE = 0,
				ePEN_BLACK,
				ePEN_GRAY,
				ePEN_GRAY_DARK, 
				ePEN_GRAY_LIGHT,
				ePEN_RED, 
				ePEN_GREEN, 
				ePEN_BLUE, 
				ePEN_YELLOW,
				ePEN_WARNING2,  
	};

	enum eFONT_TYPE {
				eFONT_SIZE_1 = 0, 
				eFONT_SIZE_2, 
				eFONT_SIZE_3, 
				eFONT_SIZE_4,
				eFONT_SIZE_5,
				eFONT_SIZE_6,
				eFONT_SIZE_7,
				eFONT_SIZE_8,
				eFONT_SIZE_9,
				eFONT_SIZE_10,
	};


	CDrawSymbol();
	virtual				~CDrawSymbol();
	
	static void			InitInstance();
	static void			ExitInstance();

	static void			DrawLineRect(		CDC* pDC,	LPRECT lrect,	COLORREF colorPen);
	static void			DrawButtonRect(		CDC* pDC,	LPRECT lrect,	COLORREF colorPen,	COLORREF crBack);
	static void			DrawText(			CDC* pDC,	LPRECT lrect,											COLORREF crText,	DWORD dwAlign=DT_CENTER,int iSize=2,				LPCTSTR szText=NULL);
	static void			SelectedDrawText(	CDC* pDC,	LPRECT lrect,	COLORREF colorPen, 	COLORREF crBack,	COLORREF crText,	DWORD dwAlign=DT_CENTER, int iSize=2,				LPCTSTR szText=NULL);
	static void			SelectedDrawInt(	CDC* pDC,	LPRECT lrect,	COLORREF colorPen,	COLORREF crBack,	COLORREF crText,	DWORD dwAlign=DT_RIGHT,  int iSize=2,				int iValue=0);
	static void			SelectedDrawDouble(	CDC* pDC,	LPRECT lrect,	COLORREF colorPen,	COLORREF crBack,	COLORREF crText,	DWORD dwAlign=DT_RIGHT,  int iSize=2, int iPoint=3,	double dValue=0);
public:
	static CPen			m_Pens[MAX_PEN];											// Red/Green/Blue pen
	static CFont		m_ftLabel[MAX_FONT];										// 0:MainMenu, 1:SubMenu
};

extern AFX_EXT_DATA_MISC COLORREF		g_colorText			[MAX_EVENT_COLOR];
extern AFX_EXT_DATA_MISC COLORREF		g_colorForeground	[MAX_EVENT_COLOR];
extern AFX_EXT_DATA_MISC COLORREF		g_colorBackground	[MAX_EVENT_COLOR];
extern AFX_EXT_DATA_MISC CBrush			g_brushForeground	[MAX_EVENT_COLOR];
extern AFX_EXT_DATA_MISC CBrush			g_brushBackground	[MAX_EVENT_COLOR];

extern AFX_EXT_API_MISC void			DrawButton			(CDC* pDC, CRect rect, CString str);
extern AFX_EXT_API_MISC void			DrawButtonDisable	(CDC* pDC, CRect rect, CString str);
extern AFX_EXT_API_MISC void			DrawButtonMouseMove	(CDC* pDC, CRect rect, CString str);
extern AFX_EXT_API_MISC void			DrawButtonMouseDown	(CDC* pDC, CRect rect, CString str);
extern AFX_EXT_API_MISC void			DrawButtonMouseUp	(CDC* pDC, CRect rect, CString str);
extern AFX_EXT_API_MISC void			DrawButtonSelected	(CDC* pDC, CRect rect, CString str);
extern AFX_EXT_DATA_MISC CRect			g_rectArea[eAREA_END];
#pragma pack(pop)

#endif 
