#pragma once

#include "AFX_EXT_MIP.h"
#include "IPData.h"

#pragma pack(push, 8)

class CIPDataException;
class CIPDataItemIterator;

//=============================================================================
// CIPDataItemIterator
//

//-----------------------------------------------------------------------------
// CIPDataException
class AFX_EXT_CLASS_MIP CIPDataException : public CIPException {
public:
	CIPDataException(LPCTSTR pszFunctionName = NULL, LPCTSTR pszReason = NULL) : CIPException(IPE_VARIABLE_MISMATCH, pszFunctionName, pszReason) { }
};

//-----------------------------------------------------------------------------
// CIPDataItemIterator
class AFX_EXT_CLASS_MIP __declspec(novtable) CIPDataItemIterator {
protected:
	void* m_pAddress;
	CIPDataItem* m_pItem;

public:
	CIPDataItemIterator(void* pAddress = NULL, CIPDataItem* pItem = NULL) : m_pAddress(pAddress), m_pItem(pItem) { }
	~CIPDataItemIterator() { }
	void Clear();

public:
	LPCSTR GetName() { return m_pItem->m_strName; }

public:
	uintptr_t GetSize() const {
		if (!m_pItem)
			return 0;
		return m_pItem->GetTotalSize();
	}
	uintptr_t GetCount() const {
		if (!m_pItem)
			return 0;
		return m_pItem->IsArray() ? m_pItem->m_nCount : 1;
	}
	void* GetAddress() {
		if (!m_pItem)
			return NULL;
		return m_pAddress;
	}
	BOOL IsEmpty() const {
		return !m_pItem || !m_pAddress;
	}
public:
	CIPDataItemIterator GetChild(LPCSTR pszVarName, BOOL bSilent = FALSE) const /*throw(CIPDataException)*/;
	CIPDataItemIterator GetChild(uintptr_t iIndex) const /*throw(CIPDataException)*/;
	CIPDataItemIterator operator () (LPCSTR pszVarName) const /*throw(CIPDataException)*/;
	CIPDataItemIterator operator [] (uintptr_t iIndex) const /*throw(CIPDataException)*/;

	CIPDataItem* GetItem() { return m_pItem; }
	const CIPDataItem* GetItem() const { return m_pItem; }

public:
	int32_t		GetInt() const ;
	uint32_t	GetUInt() const ;
	int64_t		GetInt64() const ;
	uint64_t	GetUInt64() const ;
	double		GetDouble() const ;
	BOOL		Set(int32_t iValue);
	BOOL		Set(uint32_t iValue);
	BOOL		Set(int64_t iValue);
	BOOL		Set(uint64_t iValue);
	BOOL		Set(double dValue);
	template < typename T > BOOL SetConst(T value) {
		if (sizeof(T) != GetSize())
			return FALSE;
		void* pAddress = GetAddress();
		if (!pAddress)
			return FALSE;
		*(T*)pAddress = value;
		return TRUE;
	}
	template < typename T > T GetConst() {
		if (sizeof(T) != GetSize())
			return FALSE;

		void* pAddress = GetAddress();
		if (!pAddress)
			return FALSE;
		T value = *(T*)pAddress;
		return value;
	}
	template < typename T > BOOL SetValue(const T& value) {
		if (sizeof(T) != GetSize())
			return FALSE;
		void* pAddress = GetAddress();
		if (!pAddress)
			return FALSE;
		memcpy(pAddress, &value, sizeof(value));
		return TRUE;
	}
	template < typename T > BOOL GetValue(T& value) {
		if (sizeof(T) != GetSize())
			return FALSE;
		void* pAddress = GetAddress();
		if (!pAddress)
			return FALSE;
		memcpy(&value, pAddress, sizeof(value));
		return TRUE;
	}
	BOOL SetData(const TBuffer<BYTE> buf);
	BOOL GetData(TBuffer<BYTE>& buf) const;
	BOOL SetZero();
	BOOL GetString(CStringA& str) const;
	BOOL GetString(CStringW& str) const;
	CString GetString() const;
	CStringA GetStringA() const;
	CStringW GetStringW() const;
	BOOL SetString(LPCSTR pszValue);
	BOOL SetString(LPCWSTR pszValue);

//protected:
//	template < typename TCHAR, typename TSTRING > TSTRING ToStringT() const;
//	template < typename TCHAR, typename TSTRING > TSTRING FromStringT();

public:
	CString ToString() const;
	CStringA ToStringA(uintptr_t iIndent = 0) const;
	CStringW ToStringW(uintptr_t iIndent = 0) const;
	template < typename TCHAR, typename TSTRING >
	BOOL FromStringT(const TCHAR* pszContent, const TCHAR** ppos = NULL);
	BOOL FromString(LPCSTR pszContent, LPCSTR* ppos = NULL);
	BOOL FromString(LPCWSTR pszContent, LPCWSTR* ppos = NULL);

protected:
	template < typename type > inline type GetValueAs() const;
	template < typename type > inline BOOL SetValueAs(type value);
	template < typename type > inline type BitFieldToValueAs(type value) const;
	template < typename type > inline type ValueToBitFieldAs(type value) const;
};


//=============================================================================
// CIPDataCommPacket - under construction
//

class CIPDataCommPacket {
public:
	static char s_signature[IP_LEN_SIGNATURE];
	eIP_DATA_COMMAND m_eCmd;
	TBuffer<BYTE> m_data;
	CString m_strMessage;
protected:

};

//=============================================================================
// CIPDataComm - under construction
//

class CIPDataComm {
protected:
	CSocket2 m_socket;

public:
	CIPDataComm() {
	}
	~CIPDataComm() {
	}

public:
	BOOL IsConnected() const { return m_socket != INVALID_SOCKET; }

public:
	// Server Part
	BOOL AttachSocket(SOCKET socket) { m_socket.Attach(socket); }
	SOCKET DetachSocket() { return m_socket.Detach(); }

	// Client Part
	BOOL ConnectTo(LPCTSTR pszIP, UINT nPort);

};
