#pragma once

#include "AFX_EXT_MISC.h"

#include <wincrypt.h>

#include <TList.hpp>

AFX_EXT_API_MISC BOOL xEncryptFile(CFile* pFileSource, CFile* pFileDest, LPCSTR pszPassword, CString& strError, ALG_ID eAlgorithm = CALG_RC4);
AFX_EXT_API_MISC BOOL xDecryptFile(CFile* pFileSource, CFile* pFileDest, LPCSTR pszPassword, CString& strError, ALG_ID eAlgorithm = CALG_RC4);

#if 0
AFX_EXT_API_MISC BOOL xGetKeys(int nBitKey, CString& strKeyPublic, CString& strKeySecret, CString& strError);
AFX_EXT_API_MISC BOOL xEncryptFileRSA(const CString& source, CString& dest, const CString& strKeyPublic, const CString& strKeySecret, CString& strError);
AFX_EXT_API_MISC BOOL xDecryptFileRSA(const CString& source, CString& dest, const CString& strKeyPublic, const CString& strKeySecret, CString& strError);
#endif
