#pragma once

#include "AFX_EXT_MISC.h"

//-----------------
//xUnZipFile
AFX_EXT_API BOOL xUnZipFile(LPCTSTR pszZipFileName, LPCTSTR pszTargetFolder, CStrings& strsPath, BOOL bListOnly = FALSE, LPCSTR pszPassword = NULL);

//-----------------
//xZipFile
AFX_EXT_API BOOL xZipFile(LPCTSTR pszSourceFolder, const CStrings& strFiles, LPCTSTR pszTargetZipFileName, LPCSTR pszPassword = NULL);

//-----------------
//xUnZipMem
AFX_EXT_API BOOL xUnZipMem(const std::vector<BYTE>& src, std::vector<BYTE>& dest, const int nDefaultBufferSize = 102400);

//-----------------
//xZipMem
//	eDeflateLevel : 0 (non)~ 9 (best), -1: default
//		Z_NO_COMPRESSION         0
//		Z_BEST_SPEED             1
//		Z_BEST_COMPRESSION       9
//		Z_DEFAULT_COMPRESSION  (-1)
AFX_EXT_API BOOL xZipMem(const std::vector<BYTE>& src, std::vector<BYTE>& dest, int eDeflateLevel = -1);	
