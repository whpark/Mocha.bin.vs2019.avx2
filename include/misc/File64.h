#pragma once

#include "AFX_EXT_MISC.h"

#include "afx.h"

class AFX_EXT_CLASS_MISC CFile64 : public CFile {
	DECLARE_DYNAMIC(CFile64)

public:
	CFile64() : CFile() {}
	virtual ~CFile64() {}

#if _M_64_
	virtual size64_t Read(void* lpBuf, size64_t nCount);
	virtual void Write(const void* lpBuf, size64_t nCount);
#endif

};
