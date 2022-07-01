#pragma once

#include "AFX_EXT_MIP.h"
#include "TList.hpp"
#include "misc/TString.h"
#include "misc/StreamPreprocessor.h"
#include "misc/Socket2.h"
#include "IP_Definitions.h"

#pragma pack(push, 8)

class CIPDataItem;
class CIPDataItemIterator;
class CIPData;

//=============================================================================
// CIPDataItem
//

class AFX_EXT_CLASS_MIP CIPDataItem {
	friend class CIPData;
	friend class CIPDataItemIterator;
private:
	static CIPDataItem s_dataNULL;
public:
protected:
	eIP_DATA_TYPE		m_eType;
	CStringA			m_strName;
	CString				m_strTypeName;
	CString				m_strTagName;
	uintXX_t				m_nStructAlign;
	uintptr_t			m_nOffset;
	uintptr_t			m_nItemSize;	// Item Size in Bytes. the TotalSize is m_nCount * m_nItemSize. for ex, int K[3], m_nItemSize is sizeof(int), and m_nCount is 3.
	uintptr_t			m_nCount;		// Array Count
	struct BIT_FIELD {
		BYTE nBitField;
		BYTE iStart;
		BYTE dummy[2];
	};
	BIT_FIELD m_bitfield;
	TBuffer<BYTE>		m_default;
	TList<CIPDataItem>	m_child;
public:
	CIPDataItem();
	~CIPDataItem();

	void Init();

	BOOL operator == (const char* pszVarName) const { return m_strName == pszVarName; }

	BOOL IsEmpty() const { return m_nItemSize ? FALSE : TRUE; }

	BOOL CompareDataType(const CIPDataItem& B) const;

public:
	const void* GetAddressConst(const void* pStartAddress, uintptr_t iIndex = 0, uintptr_t* pnSize = NULL) const;
	const void* GetAddress(const void* pStartAddress, uintptr_t iIndex = 0, uintptr_t* pnSize = NULL) const;
	void* GetAddress(const void* pStartAddress, uintptr_t iIndex = 0, uintptr_t* pnSize = NULL);

public:
	eIP_DATA_TYPE GetType() const { return m_eType; }
	LPCSTR  GetName() const { return m_strName; }
	LPCTSTR GetTypeName() const { return m_strTypeName; }
	LPCTSTR GetTagName() const { return m_strTagName; }
	uintXX_t GetStructAlign() const { return m_nStructAlign; }
	uintptr_t GetOffset() const { return m_nOffset; }
	BOOL IsUnion() const { return m_eType == IPDT_UNION; }
	BOOL IsStruct() const { return m_eType == IPDT_STRUCT; }
	BOOL IsArray() const { return (m_eType == IPDT_ARRAY) && (m_child.N() == 1); }
	BOOL IsKindOfInt() const {
		switch (m_eType) {
		case IPDT_INT8: case IPDT_INT16: case IPDT_INT32: case IPDT_INT64:
		case IPDT_UINT8: case IPDT_UINT16: case IPDT_UINT32: case IPDT_UINT64: case IPDT_WCHAR: return TRUE;
		}
		return FALSE;
	}
	BOOL IsKindOfFloat() const { return (m_eType == IPDT_DOUBLE) || (m_eType == IPDT_DOUBLE); }
	BOOL GetBitField() const { return m_bitfield.nBitField; }
	BOOL GetBitFieldStart() const { return m_bitfield.iStart; }
	void GetDefaultValue(TBuffer<BYTE>& defaultValue) const { defaultValue = m_default; }

	const TList<CIPDataItem>& GetChildItems() const { return m_child; }			// by 이상영
	uintptr_t GetItemSize() const { return m_nItemSize; }					// size in byte of single item
	uintptr_t GetTotalSize() const { return m_nItemSize * m_nCount; }		// size in byte
	uintptr_t GetItemCount() const { return m_nCount; }				// array count
	const CIPDataItem& GetChildItem(LPCSTR pszVarName) const;
	CIPDataItem& GetChildItem(LPCSTR pszVarName);
	const CIPDataItem* GetChildItemP(LPCSTR pszVarName) const;
	CIPDataItem* GetChildItemP(LPCSTR pszVarName);

public:
	BOOL GetValueRaw(const void* pStartAddress, void* pData, uintptr_t nSize, uintptr_t iIndex) const;
	BOOL SetValueRaw(const void* pStartAddress, const void* pData, uintptr_t nSize, uintptr_t iIndex);

};


//=============================================================================
// CIPData
//

class CIPDataServer;

class AFX_EXT_CLASS_MIP CIPData {
	friend class CIPDataServer;
private:
	const static sizeXX_t s_nSizeDummy;
	const static char s_szSignDataFile[IP_DATA_HEADER_ITEM_LEN];
protected:
	CString m_strDefinitionFile;
	CString m_strMapFile;
	CString m_strMapName;
	CString m_strMapNamePrefix;
	HANDLE m_hFile;
	HANDLE m_hFileMapping;
	BYTE* m_pAddress;
	TList<CIPDataItem> m_data;
public:
	CIPData(LPCTSTR pszMapNamePrefx = _T(IP_DEFAULT_MAP_FILE_PREFIX));
	~CIPData(void);

public:
	CIPData(const CIPData& B) { ASSERT(FALSE); }
	CIPData& operator = (const CIPData& B) { ASSERT(FALSE); return *this; }

public:
	LPCTSTR GetMapName() const { return m_strMapName; }
	BOOL operator == (LPCSTR pszMapName) { return m_strMapName == pszMapName; }
	BOOL operator == (LPCWSTR pszMapName) { return m_strMapName == pszMapName; }

public:
	BOOL Open(LPCSTR pszMapName, LPCTSTR pszMapNamePrefx = _T(IP_DEFAULT_MAP_FILE_PREFIX));										// Open Memory Map
	BOOL Open(LPCSTR pszMapName, CString& strErrorMessage, LPCTSTR pszMapNamePrefx = _T(IP_DEFAULT_MAP_FILE_PREFIX));				// Open Memory Map
	BOOL Close();
	BOOL IsOpen() const;
	CString MakeMapName(LPCSTR pszMapName);

public:
	const TList<CIPDataItem>& GetItems() const { return m_data; }	// by 이상영

	BOOL GetDataValue(LPCSTR pszVarName, void* pData, uintptr_t nSize, uintptr_t iIndex = 0);
	BOOL SetDataValue(LPCSTR pszVarName, const void* pData, uintptr_t nSize, uintptr_t iIndex = 0);

	CIPDataItemIterator GetDataItem(LPCSTR pszVarExpr);
	CIPDataItemIterator GetDataItem(LPCWSTR pszVarExpr);

	const CIPDataItem& GetItem(LPCSTR pszVarName) const;
	const void* GetAddress(LPCSTR pszVarName, uintptr_t *pnSize = NULL) const;
	void* GetAddress(LPCSTR pszVarName, uintptr_t *pnSize = NULL);

	template < typename T >
	BOOL GetAddress(LPCSTR pszVarName, T** ppAddress) {
		if (!pszVarName || !ppAddress)
			return FALSE;
		*ppAddress = NULL;

		uintptr_t nSize = 0;
		void* pAddress = GetAddress(pszVarName, &nSize);

		const CIPDataItem& item = GetItem(pszVarName);
		if (item.GetItemSize() != sizeof(T))	// Check Size
			return FALSE;
		*ppAddress = (T*)pAddress;

		return *ppAddress ? TRUE : FALSE;
	}
	template < typename T >
	T* GetAddress(LPCSTR pszVarName) {
		if (!pszVarName)
			return FALSE;

		uintptr_t nSize = 0;
		T* pAddress = (T*)GetAddress(pszVarName, &nSize);

		const CIPDataItem& item = GetItem(pszVarName);
		if (item.GetItemSize() != sizeof(T))	// Check Size
			return FALSE;

		return pAddress;
	}

protected:
	const void* GetAddressConst(LPCSTR pszVarName, uintptr_t *pnSize = NULL) const;

public:
	// Creates Memory Map (Called by Frameword(IPServer) only)
	BOOL CreateDataFile(LPCSTR pszMapName, LPCSTR pszDefinitionFile, BOOL bPhysicalFile = TRUE, BOOL bForceInit = FALSE);
	BOOL ValidateMap();
	BOOL BackupSafeData();

protected:
	// Definition File
	BOOL LoadDefinitionFile(LPCSTR pszDefinitionFile);
	BOOL GetWordAlign(stream_processor::CStreamPreprocessor& sp, std::vector<uint32_t>& nWordAligns, uint32_t& nCurrentWordAlign);
	BOOL GetDataType(stream_processor::CStreamPreprocessor& sp, const TList<CIPDataItem>& userDefinedDataTypes, CIPDataItem& dataType, uintptr_t nStartOffset, uintXX_t nWordAlign, BOOL bTopMost = FALSE);
	BOOL GetDataVarName(stream_processor::CStreamPreprocessor& sp, const CIPDataItem& dataType, CIPDataItem& dataVar, uintptr_t nStartOffset, uintXX_t nWordAlign, BOOL bTopMost = FALSE);
	BOOL GetDefaultValue(stream_processor::CStreamPreprocessor& sp, const TList<CIPDataItem>& userDefinedDataTypes, CIPDataItem& dataType, void* pAddress);
	BOOL GetDefaultValueIndex(stream_processor::CStreamPreprocessor& sp, const TList<CIPDataItem>& userDefinedDataTypes, CIPDataItem& dataType, uintptr_t iIndex, void* pAddress);

	bool SetDefaultValue(BYTE* pAddress, const CIPDataItem& item);

};

template < typename T >
BOOL GetIPDataAddress(CIPData& ipData, LPCSTR pszVarName, T** ppAddress, uintptr_t iIndex = 0) {
	if (!pszVarName || !ppAddress)
		return FALSE;
	*ppAddress = NULL;

	CIPDataItem& item = ipData.GetItem(pszVarName);
	if (item.GetItemSize() != sizeof(T))	// Check Size
		return FALSE;
	*ppAddress = ((T*)ipData.GetAddress(pszVarName));
	if ( (0 > iIndex) || (item.GetItemCount() >= iIndex) )
		return FALSE;
	//(uintXX_t&)(*ppAddress) += item.GetItemSize() * iIndex;
	if (iIndex)
		*ppAddress += iIndex;

	return *ppAddress ? TRUE : FALSE;
}
