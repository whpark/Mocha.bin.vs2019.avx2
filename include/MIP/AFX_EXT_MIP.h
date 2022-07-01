#pragma once

#include "default.defines.h"

#ifndef _MIP_AS_DLL
#	define _MIP_AS_DLL 1
#endif

#if (_MIP_AS_DLL)

#	ifdef _EXPORTS_MIP
#		define AFX_EXT_CLASS_MIP	AFX_CLASS_EXPORT
#		define AFX_EXT_API_MIP		AFX_API_EXPORT
#		define AFX_EXT_DATA_MIP		AFX_DATA_EXPORT
#	else 
#		define AFX_EXT_CLASS_MIP	AFX_CLASS_IMPORT
#		define AFX_EXT_API_MIP		AFX_API_IMPORT
#		define AFX_EXT_DATA_MIP		AFX_DATA_IMPORT
		__IMPORT_MOCHA_LIB__(mip)
#	endif

#else	// _MIP_AS_DLL

#	define AFX_EXT_CLASS_MIP
#	define AFX_EXT_API_MIP
#	define AFX_EXT_DATA_MIP

#endif
