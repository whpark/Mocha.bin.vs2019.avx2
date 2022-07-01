#pragma once

#include "default.defines.h"

#ifndef _MISC_AS_DLL
#	define _MISC_AS_DLL 1
#endif

#if (_MISC_AS_DLL)

#	ifdef _EXPORTS_MISC
#		define AFX_EXT_CLASS_MISC	AFX_CLASS_EXPORT
#		define AFX_EXT_API_MISC		AFX_API_EXPORT
#		define AFX_EXT_DATA_MISC	AFX_DATA_EXPORT
#	else 
#		define AFX_EXT_CLASS_MISC	AFX_CLASS_IMPORT
#		define AFX_EXT_API_MISC		AFX_API_IMPORT
#		define AFX_EXT_DATA_MISC	AFX_DATA_IMPORT
		__IMPORT_MOCHA_LIB__(misc)
#	endif

#else	// _MISC_AS_DLL

#	define AFX_EXT_CLASS_MISC
#	define AFX_EXT_API_MISC
#	define AFX_EXT_DATA_MISC

#endif
