/////////////////////////////////////////////////////////////////////////////
//
//
// HSMS
// PWH.
// 2009.03.02.
//
/////////////////////////////////////////////////////////////////////////////

#pragma once

#include "AFX_EXT_HSMS.h"

//#include "TList.hpp"
#include "qtl/qtl.hpp"
#include "misc/ArchiveU.h"
#include "HSMS_Definition.h"

#pragma warning(disable: 4996)

#pragma pack(push, 8)

class CSECSException;
class CSECSVariable;
class CSECSVariableTemplate;
class CSECSVariableIterator;
class CSECSMessage;
class CSECSMessageTemplate;
class CSECSTransaction;
class CSECSTransactionTemplate;

typedef CSECSVariableIterator CSECSVarIter;

//---------------------------------------------------------------------------
enum eSECS_VARIABLE_TYPE {
	SVT_NONE = -1,
	SVT_LIST = 0,
	SVT_BINARY = 010,
	SVT_BOOLEAN = 011,
	SVT_ASCII = 020,
	SVT_JIS8 = 021,
	SVT_2BYTE_CHARACTER = 022,
	SVT_SIGNED_INT_64 = 030,
	SVT_SIGNED_INT_8 = 031,
	SVT_SIGNED_INT_16 = 032,
	SVT_SIGNED_INT_32 = 034,
	SVT_DOUBLE = 040,
	SVT_FLOAT = 041,
	SVT_UNSIGNED_INT_64 = 050,
	SVT_UNSIGNED_INT_8 = 051,
	SVT_UNSIGNED_INT_16 = 052,
	SVT_UNSIGNED_INT_32 = 054,
	SVT_ANY = 0x0101,
	SVT_STRUCT,
	SVT_UNION,
	//SVT_PARENT,
};

//---------------------------------------------------------------------------
#pragma pack(push, 1)
struct HSMS_MESSAGE_HEADER {
	// BYTE 0-1
	uint16_t iSessionID;	// Device ID
	// BYTE 2
	union {
		struct {
			uint8_t iStream:7;	// Lower 7 Bits
			uint8_t bWaitBit:1;	// Upper 1 Bit. (MSB)
		};
		uint8_t eStatus;
	};
	// BYTE 3
	union {
		uint8_t iFunction;
		uint8_t eSelectStatus;
	};
	// BYTE 4
	uint8_t ePresentationType;
	// BYTE 5
	uint8_t eSessionType;
	// BYTE 6-9
	uint32_t dwSystemByte;

	void ChangeByteOrdering() {
		SwapByte(iSessionID);
		//SwapByte(eStatus);
		//SwapByte(eSelectStatus);
		//SwapByte(ePresentationType);
		//SwapByte(eSessionType);
		SwapByte(dwSystemByte);
	}

};
#pragma pack(pop)


//---------------------------------------------------------------------------
class AFX_EXT_CLASS_HSMS CSECSException : public CException {
protected:
	CString m_strCause;
public:
	CSECSException(LPCTSTR pszCause, ...) : CException(TRUE) {
		va_list arglist;
		va_start(arglist, pszCause);

		CString str;
		str.FormatV(pszCause, arglist);

		va_end(arglist);
		m_strCause = str;
	}
	~CSECSException() {
	}
	
	LPCTSTR GetErrorMessage() {
		return m_strCause;
	}

	virtual BOOL GetErrorMessage(LPTSTR lpszError, UINT nMaxError, PUINT pnHelpContext = NULL) {
		if (nMaxError <= (UINT)m_strCause.GetLength())
			return FALSE;
		_tcscpy(lpszError, m_strCause);
		return TRUE;
	}
};


//---------------------------------------------------------------------------
class AFX_EXT_CLASS_HSMS CSECSVariableTemplate {
	friend class CSECSMessageTemplate;
private:
	static CSECSVariableTemplate s_varTemplateNULL;
protected:
	CString m_strName;

	eSECS_VARIABLE_TYPE m_eSECSVariableType;
	CString m_strUserDefinedTypeName;

	int m_nItemFixed;	// -1 when flexible number.

	CString m_strDefaultValue;

	qtl::TPtrVector<CSECSVariableTemplate> m_items;

public:
	CSECSVariableTemplate() {
		m_eSECSVariableType = SVT_NONE;
		m_nItemFixed = 0;
		m_items.SetInterlockMode(true);
	}
	~CSECSVariableTemplate() {
	}


#ifdef _UNICODE
	int operator == (LPCSTR pszUserDefinedTypeName) const { return m_strUserDefinedTypeName == CString(pszUserDefinedTypeName); }
	int operator == (LPCWSTR pszUserDefinedTypeName) const { return m_strUserDefinedTypeName == pszUserDefinedTypeName; }
#else
	int operator == (LPCSTR pszUserDefinedTypeName) const { return m_strUserDefinedTypeName == pszUserDefinedTypeName; }
	int operator == (LPCWSTR pszUserDefinedTypeName) const { return m_strUserDefinedTypeName == CString(pszUserDefinedTypeName); }
#endif

	void Init() {
		m_strName.Empty();
		m_eSECSVariableType = SVT_NONE;
		m_strUserDefinedTypeName.Empty();
		m_nItemFixed = 0;
		m_strDefaultValue.Empty();
		m_items.DeleteAll();
	}

	CSECSVariableTemplate& operator [] (int iIndex) { if ( (iIndex < 0) || (iIndex >= m_items.size()) ) return s_varTemplateNULL; return m_items[iIndex]; }
	const CSECSVariableTemplate& operator [] (int iIndex) const { if ( (iIndex < 0) || (iIndex >= m_items.size()) ) return s_varTemplateNULL; return m_items[iIndex]; }

public:
	LPCTSTR GetName() const { return m_strName; }
	CStringA GetNameA() const { return CStringA(m_strName); }
	CStringW GetNameW() const { return CStringW(m_strName); }
	LPCTSTR GetUserDefinedTypeName() const { return m_strUserDefinedTypeName; }
	eSECS_VARIABLE_TYPE GetVariableType() const { return m_eSECSVariableType; }
	static LPCTSTR GetVariableTypeName(eSECS_VARIABLE_TYPE eSECSVariableType);
	LPCTSTR GetVariableTypeName() const {
		if (!m_strUserDefinedTypeName.IsEmpty())
			return m_strUserDefinedTypeName;
		return GetVariableTypeName(m_eSECSVariableType);
	}
	BOOL IsUnion() const { return m_eSECSVariableType == SVT_UNION; }
	BOOL IsValueFixed() const { return !m_strDefaultValue.IsEmpty(); }
	LPCTSTR GetFixedValue() const { if (m_strDefaultValue.IsEmpty()) return NULL; return m_strDefaultValue; }

	BOOL IsCountFixed() const { return (m_nItemFixed >= 0); }
	int GetFixedItemCount() const { return m_nItemFixed; }
	const qtl::TPtrVector<CSECSVariableTemplate>& GetItems() const { return m_items; }

public:
	CSECSVariableTemplate* FindVariable(LPCSTR pszVarName);
	CSECSVariableTemplate* FindVariable(LPCWSTR pszVarName);
protected:
	template <typename TCHAR>
	CSECSVariableTemplate* FindVariableT(const TCHAR* pszVarName);

public:
	BOOL StoreAsLegacyForm(CArchiveU& ar, int nDepth = 0, int iIndex = 0) const;
	BOOL StoreAsStruct(CArchiveU& ar, BOOL bPrintLineEnd, int nDepth = 0) const;
	BOOL StoreAsStructDeclare(CArchiveU& ar, int nDepth = 0) const;
};


//---------------------------------------------------------------------------
class AFX_EXT_CLASS_HSMS CSECSVariable {
protected:
	eSECS_VARIABLE_TYPE m_eSECSVariableType;
	TBuffer<BYTE> m_value;
	qtl::TPtrVector<CSECSVariable> m_items;

public:
// Constructor & Destructor
	CSECSVariable(eSECS_VARIABLE_TYPE eSECSVariableType = SVT_NONE, int nItem = 0) {
		Set(eSECSVariableType, nItem);
		m_items.SetInterlockMode(true);
	}
	~CSECSVariable() {
	}
	BOOL Set(eSECS_VARIABLE_TYPE eSECSVariableType = SVT_NONE, int nItem = 0);

// Attributes
	eSECS_VARIABLE_TYPE GetVariableType() const { return m_eSECSVariableType; }
	int GetItemSize() const;
	static int GetItemSize(eSECS_VARIABLE_TYPE eSECSVariableType);
	int GetItemCount() const {
		if (GetVariableType() == SVT_LIST) return m_items.size();
		int nItemSize = GetItemSize();
		if (nItemSize == 0) return 0;
		return m_value.GetSize()/nItemSize;
	}
	int SetItemCount(int nItem);
	int AddNewChildItem();

// Get & Set Variables
	// Int
	BOOL		IsSigned() const;
	int32_t		GetSignedInt(int iIndex = 0) const;
	uint32_t	GetUnsignedInt(int iIndex = 0) const;
	int64_t		GetSignedInt64(int iIndex = 0) const;
	uint64_t	GetUnsignedInt64(int iIndex = 0) const;
	int32_t		GetInt(int iIndex = 0) const   { return IsSigned() ? GetSignedInt(iIndex)   : GetUnsignedInt(iIndex); }
	int64_t		GetInt64(int iIndex = 0) const { return IsSigned() ? GetSignedInt64(iIndex) : GetUnsignedInt64(iIndex); }
	uintXX_t		GetUInt(int iIndex = 0) const  { return GetUnsignedInt(iIndex); }
	BOOL SetInt(uint32_t nValue, int iIndex = 0);
	BOOL SetSignedInt(int32_t nValue, int iIndex = 0);
	BOOL SetUnsignedInt(uint32_t nValue, int iIndex = 0);
	BOOL SetSignedInt64(int64_t nValue, int iIndex = 0);
	BOOL SetUnsignedInt64(uint64_t nValue, int iIndex = 0);

	// Floating point
	double GetDouble(int iIndex = 0) const;
	float GetFloat(int iIndex = 0) const;
	BOOL SetDouble(double dValue, int iIndex = 0);
	BOOL SetFloat(float fValue, int iIndex = 0);

	// Boolean
	BOOL GetBoolean(int iIndex = 0) const;
	BOOL SetBoolean(BOOL bValue, int iIndex = 0);

	// Binary
	BOOL GetBinary(void* pData, int nBufferSize, int* pnCopied = NULL) const;
	BOOL SetBinary(const void *pData, int nBufferSize, int* pnCopied = NULL);
	BYTE GetBinary(int iIndex = 0) const;
	BOOL SetBinary(BYTE bValue, int iIndex = 0);
	const TBuffer<BYTE>& GetRawData() const { return m_value; }
	TBuffer<BYTE>& GetRawData() { return m_value; }

	// Strings
	BOOL GetString(CString& str, BOOL bTrim = TRUE) const;
	CString GetString(BOOL bTrim = TRUE) const;
	BOOL GetStringA(CStringA& str, BOOL bTrim = TRUE) const;
	CStringA GetStringA(BOOL bTrim = TRUE) const;
	BOOL GetStringW(CStringW& str, BOOL bTrim = TRUE) const;
	CStringW GetStringW(BOOL bTrim = TRUE) const;
	BOOL SetString(LPCSTR szString, BOOL bAdjustSize = FALSE);
	BOOL SetString(LPCWSTR szString, BOOL bAdjustSize = FALSE);

public:
	BOOL SetFromString(LPCTSTR pszValue);

// Operator
	BOOL operator == (const CSECSVariable& B) const;
	BOOL operator != (const CSECSVariable& B) const { return !(*this == B); }
	CSECSVariable& operator [] (int iIndex);
	const CSECSVariable& operator [] (int iIndex) const;

	// Streaming
public:
	BOOL Serialize(CArchiveU& ar) {
		try {
			if (ar.IsStoring()) {
				if (!StoreToArchive(ar))
					return FALSE;
			} else {
				if (!LoadFromArchive(ar))
					return FALSE;
			}
		} catch (CArchiveException* ) {
			// to do : Error Log
			return FALSE;
		}
		return TRUE;
	}
public:
	BOOL StoreToArchive(CArchiveU& ar) const;
	BOOL LoadFromArchive(CArchiveU& ar);

public:
	CString ToString() const;

protected:
	template <typename TCHAR, class TSTRING>
	BOOL GetStringT(TSTRING& str, BOOL bTrim = TRUE) const;
	template <typename TCHAR>
	BOOL SetStringT(const TCHAR* szString, BOOL bAdjustSize);
};


//---------------------------------------------------------------------------
class AFX_EXT_CLASS_HSMS CSECSVariableIterator {
private:
	static CSECSVariable s_varNULL;

protected:
	const CSECSVariableTemplate* m_pVarTemplate;
	CSECSVariable* m_pVariable;
public:
	CSECSVariableIterator(const CSECSVariableTemplate* pVarTemplate = NULL, CSECSVariable* pVariable = NULL) {
		m_pVarTemplate = NULL;
		m_pVariable = NULL;
		Set(pVarTemplate, pVariable);
	}
	~CSECSVariableIterator() {
		m_pVarTemplate = NULL;
		m_pVariable = NULL;
	}
	void Set(const CSECSVariableTemplate* pVarTemplate = NULL, CSECSVariable* pVariable = NULL) {
		m_pVarTemplate = pVarTemplate;
		m_pVariable = pVariable;
	}

// operators
public:
	BOOL PrepareVariable(int nArraySize = -1 /* as default size */, LPCTSTR pszVarName = NULL /* for union member */);
	CSECSVariableIterator AddItem();
	BOOL IsNull() { return !m_pVariable || (m_pVariable->GetVariableType() == SVT_NONE); }
	CSECSVariableIterator operator [] (LPCWSTR pszVarName);
	CSECSVariableIterator operator [] (LPCSTR pszVarName);
	CSECSVariableIterator operator [] (int iIndex);
	CSECSVariable& operator () (void);

	const CSECSVariable* GetVariable() const { return m_pVariable; }
	const CSECSVariableTemplate* GetVariableTemplate() const { return m_pVarTemplate; }

	int GetItemCount() const { return m_pVariable ? m_pVariable->GetItemCount() : 0; }
	LPCTSTR GetItemName() const { return m_pVarTemplate ? m_pVarTemplate->GetName() : _T(""); }
	CStringA GetItemNameA() const { return m_pVarTemplate ? m_pVarTemplate->GetNameA() : CStringA(""); }
	eSECS_VARIABLE_TYPE GetVariableType() const { return m_pVarTemplate ? m_pVarTemplate->GetVariableType() : SVT_NONE ; }
	LPCTSTR GetVariableTypeName() const {
		return
			m_pVarTemplate
				? m_pVarTemplate->GetVariableTypeName()
				: m_pVariable 
					? CSECSVariableTemplate::GetVariableTypeName(m_pVariable->GetVariableType())
					: _T("")
			;
	}

public:
	int GetMatchingCount() const ;
	int GetMatchingCount(const CSECSVariable& var) const ;
	int GetMatchingCount(const CSECSVariableTemplate& varTemplate) const ;
	static int GetMatchingCount(const CSECSVariableTemplate& varTemplate, const CSECSVariable& var, int* piMostFittingUnionVarTemplate = NULL);

protected:
	CSECSVariableIterator GetVariable(LPCTSTR pszVarName);
	BOOL FindVariable(CSECSVariableIterator& varIterator, LPCTSTR pszVarName) const ;
	int FindUnionVariableTemplate(LPCTSTR pszVarName = NULL) const ;
};


//---------------------------------------------------------------------------
class AFX_EXT_CLASS_HSMS CSECSMessage {
protected:
	HSMS_MESSAGE_HEADER m_header;
	CString m_strName;
	CSECSVariable m_variable;

public:
	CSECSMessage() {
		Init();
	}
	CSECSMessage(const CSECSMessage& B) { *this = B; }

	BOOL Init(LPCTSTR pszName = NULL, signed char iStream = 0, unsigned char iFunction = 0, BOOL bWait = FALSE) {
		ZeroVar(m_header);
#ifdef __HSMS_CHILD_FLAG__
		m_variable.Set(SVT_LIST, 1);
#endif
		m_strName = pszName;
		m_header.bWaitBit = bWait ? 1 : 0;
		m_header.iStream = iStream & 0x7F;
		m_header.iFunction = iFunction;
		return TRUE;
	}

	CSECSMessage& operator = (const CSECSMessage& B) {
		if (this == &B)
			return *this;

		memcpy(&m_header, &B.m_header, sizeof(m_header));
		m_strName = B.m_strName;
		m_variable = B.m_variable;
		return *this;
	}

	const HSMS_MESSAGE_HEADER& GetHeader() const { return m_header; }
	HSMS_MESSAGE_HEADER& GetHeader() { return m_header; }

	unsigned short GetSessionID() const { return m_header.iSessionID; }
	unsigned char GetStatus() const { return m_header.eStatus; }
	unsigned char GetStream() const { return m_header.iStream; }
	BOOL IsWaiting() const { return m_header.bWaitBit ? TRUE : FALSE; }
	unsigned char GetFunction() const { return m_header.iFunction; }
	unsigned char GetSelectStatus() const { return m_header.eSelectStatus; }
	unsigned char GetPresentaionType() const { return m_header.ePresentationType; }
	unsigned char GetSessionType() const { return m_header.eSessionType; }
	unsigned long GetSystemByte() const { return m_header.dwSystemByte; }

	void SetSessionID(unsigned short iSessionID) { m_header.iSessionID = iSessionID; }
	void SetStatus(unsigned char eStatus) { m_header.eStatus = eStatus; }
	void SetStream(unsigned char iStream) { m_header.iStream = iStream; }
	void SetWaiting(BOOL bWaitBit) { m_header.bWaitBit = bWaitBit ? 1 : 0; }
	void SetFunction(unsigned char iFunction) { m_header.iFunction = iFunction; }
	void SetSelectStatus(unsigned char eSelectStatus) { m_header.eSelectStatus = eSelectStatus; }
	void SetPresentaionType(unsigned char ePresentationType) { m_header.ePresentationType = ePresentationType; }
	void SetSessionType(unsigned char eSessionType) { m_header.eSessionType = eSessionType; }
	void SetSystemByte(unsigned long dwSystemByte) { m_header.dwSystemByte = dwSystemByte; }

	const CSECSVariable& GetVariable() const { return m_variable; }
	CSECSVariable& GetVariable() { return m_variable; }

public:
	BOOL StoreToArchive(CArchiveU& ar, BOOL bVariable = TRUE) const {
		if (!ar.IsStoring())
			return FALSE;
		ar << m_header.iSessionID;
		ar << m_header.eStatus;
		ar << m_header.eSelectStatus;
		ar << m_header.ePresentationType;
		ar << m_header.eSessionType;
		ar << m_header.dwSystemByte;
		if ( (m_header.eSessionType == 0) && bVariable ) {
#ifdef __HSMS_CHILD_FLAG__
			if (m_variable.GetVariableType() == SVT_LIST) {
				m_variable __HSMS_CHILD__.StoreToArchive(ar);
			}
#else
			m_variable.StoreToArchive(ar);
#endif
		}
		return TRUE;
	}
	BOOL LoadFromArchive(CArchiveU& ar, BOOL bVariable = TRUE) {
		if (ar.IsStoring())
			return FALSE;
		ar >> m_header.iSessionID;
		ar >> m_header.eStatus;
		ar >> m_header.eSelectStatus;
		ar >> m_header.ePresentationType;
		ar >> m_header.eSessionType;
		ar >> m_header.dwSystemByte;
		if ( (m_header.eSessionType == 0) && bVariable ) {
#ifdef __HSMS_CHILD_FLAG__
			if (m_variable.GetVariableType() == SVT_LIST) {
				m_variable __HSMS_CHILD__.LoadFromArchive(ar);
			}
#else
			m_variable.LoadFromArchive(ar);
#endif
		}
		return TRUE;
	}
	BOOL Serialize(CArchiveU& ar, BOOL bVariable = TRUE) {
		if (ar.IsStoring()) {
			return StoreToArchive(ar, bVariable);
		} else {
			return LoadFromArchive(ar, bVariable);
		}
		return FALSE;
	}
};

#pragma pack(pop)
