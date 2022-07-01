#pragma once

// CMFCButton2
#include "AFX_EXT_MISC.h"

#define ON_BN_PUSHED(id, memberFxn)			ON_CONTROL(BN_PUSHED, id, memberFxn)
#define ON_BN_UNPUSHED(id, memberFxn)		ON_CONTROL(BN_UNPUSHED, id, memberFxn)

class AFX_EXT_CLASS_MISC CMFCButton2 : public CMFCButton {
	DECLARE_DYNAMIC(CMFCButton2)

public:
	CMFCButton2();
	virtual ~CMFCButton2();

protected:
	DECLARE_MESSAGE_MAP()
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
};
