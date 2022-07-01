#pragma once

#include "default.defines.h"

#ifndef _HSMS_AS_DLL
#	define _HSMS_AS_DLL 1
#endif

#if (_HSMS_AS_DLL)

#	ifdef _EXPORTS_HSMS
#		define AFX_EXT_CLASS_HSMS	AFX_CLASS_EXPORT
#		define AFX_EXT_API_HSMS		AFX_API_EXPORT
#		define AFX_EXT_DATA_HSMS	AFX_DATA_EXPORT
#	else 
#		define AFX_EXT_CLASS_HSMS	AFX_CLASS_IMPORT
#		define AFX_EXT_API_HSMS		AFX_API_IMPORT
#		define AFX_EXT_DATA_HSMS	AFX_DATA_IMPORT
		__IMPORT_MOCHA_LIB__(hsms)
#	endif

#else	// _HSMS_AS_DLL

#	define AFX_EXT_CLASS_HSMS
#	define AFX_EXT_API_HSMS
#	define AFX_EXT_DATA_HSMS

#endif
