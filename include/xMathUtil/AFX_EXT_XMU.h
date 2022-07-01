#pragma once

#include "default.defines.h"

#ifndef _XMU_AS_DLL
#	define _XMU_AS_DLL 1
#endif

#if (_XMU_AS_DLL)

#	ifdef _EXPORTS_XMU
#		define AFX_EXT_CLASS_XMU	AFX_CLASS_EXPORT
#		define AFX_EXT_API_XMU		AFX_API_EXPORT
#		define AFX_EXT_DATA_XMU		AFX_DATA_EXPORT
#	else 
#		define AFX_EXT_CLASS_XMU	AFX_CLASS_IMPORT
#		define AFX_EXT_API_XMU		AFX_API_IMPORT
#		define AFX_EXT_DATA_XMU		AFX_DATA_IMPORT
		__IMPORT_MOCHA_LIB__(xMathUtil)
#	endif

#else	// _XMU_AS_DLL

#	define AFX_EXT_CLASS_XMU
#	define AFX_EXT_API_XMU
#	define AFX_EXT_DATA_XMU

#endif
