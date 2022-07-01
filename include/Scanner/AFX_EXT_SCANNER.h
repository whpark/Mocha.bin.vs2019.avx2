#pragma once

#include "default.defines.h"

#ifndef _SCANNER_AS_DLL
#	define _SCANNER_AS_DLL 1
#endif

#if (_SCANNER_AS_DLL)

#	ifdef _EXPORTS_SCANNER
#		define AFX_EXT_CLASS_SCANNER	AFX_CLASS_EXPORT
#		define AFX_EXT_API_SCANNER		AFX_API_EXPORT
#		define AFX_EXT_DATA_SCANNER		AFX_DATA_EXPORT
#	else 
#		define AFX_EXT_CLASS_SCANNER	AFX_CLASS_IMPORT
#		define AFX_EXT_API_SCANNER		AFX_API_IMPORT
#		define AFX_EXT_DATA_SCANNER		AFX_DATA_IMPORT
		__IMPORT_MOCHA_LIB__(scanner)
#	endif

#else	// _SCANNER_AS_DLL

#	define AFX_EXT_CLASS_SCANNER
#	define AFX_EXT_API_SCANNER
#	define AFX_EXT_DATA_SCANNER

#endif
