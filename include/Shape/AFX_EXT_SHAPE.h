#pragma once

#include "default.defines.h"

#ifndef _SHAPE_AS_DLL
#	define _SHAPE_AS_DLL 1
#endif

#if (_SHAPE_AS_DLL)

#	ifdef _EXPORTS_SHAPE
#		define AFX_EXT_CLASS_SHAPE	AFX_CLASS_EXPORT
#		define AFX_EXT_API_SHAPE	AFX_API_EXPORT
#		define AFX_EXT_DATA_SHAPE	AFX_DATA_EXPORT
#	else 
#		define AFX_EXT_CLASS_SHAPE	AFX_CLASS_IMPORT
#		define AFX_EXT_API_SHAPE	AFX_API_IMPORT
#		define AFX_EXT_DATA_SHAPE	AFX_DATA_IMPORT
		__IMPORT_MOCHA_LIB__(shape)
#	endif

#else	// _SHAPE_AS_DLL

#	define AFX_EXT_CLASS_SHAPE
#	define AFX_EXT_API_SHAPE
#	define AFX_EXT_DATA_SHAPE

#endif
