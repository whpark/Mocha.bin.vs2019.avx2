// Calculator.h: interface for the Calculator class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CALCULATOR_H__D467178E_CCE8_11D3_A6A9_A66E9F000000__INCLUDED_)
#define AFX_CALCULATOR_H__D467178E_CCE8_11D3_A6A9_A66E9F000000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "AFX_EXT_XMU.h"

#include "TList.hpp"

#pragma pack(push, 8)
WARNING_PUSH_AND_DISABLE(4018 4267 4244)

//=============================================================================

class Variables;
class AFX_EXT_CLASS_XMU Variable {
	friend class Variables;
public:
	TBuffer<double> values;
	CString name;

	Variable() {};
	Variable(LPCTSTR _name, sizeXX_t _size = 1) {
		Init(_name, _size);
	}
	virtual ~Variable() {
	}
	void Init(LPCTSTR _name, sizeXX_t _size = 1) {
		name = _name;
		values.SetSize(_size);
		for (intXX_t i = 0; i < _size; i++)
			values[i] = 0.0;
	}
	Variable(const Variable& B) { *this = B; }
	Variable& operator = (const Variable& B) {
		if (this != &B) {
			Init(B.name, B.values.GetSize());
			memmove(values, B.values, sizeof(values[0]) * values.GetSize());
		}
		return *this;
	}
	bool operator == (const Variable& B) const {
		return name.CompareNoCase(B.name) == 0;
	}
	bool operator == (LPCSTR psz) const {
		return name.CompareNoCase(CString(psz)) == 0;
	}
	bool operator == (LPCWSTR psz) const {
		return name.CompareNoCase(CString(psz)) == 0;
	}
};

class AFX_EXT_CLASS_XMU Variables {
public:
	enum ERROR_CODE { EC_RANGE_EXCEED = -200, EC_NOT_FOUND, EC_SUCCESS = 0 };
	TList<Variable> m_vars;

	Variables() {}
	~Variables() {}

	ERROR_CODE GetValue(double& value, LPCTSTR name, intXX_t index = 1) {
		index --;
		int i = m_vars.FindByValue(name);
		if (i < 0) return EC_NOT_FOUND;
		Variable& var = m_vars[i];
		if ( (index < 0) || (index >= var.values.GetSize()) ) return EC_RANGE_EXCEED;
		value = var.values[index];
		return EC_SUCCESS;
	}
	ERROR_CODE SetValue(double value, LPCTSTR name, intXX_t index = 1) {
		index --;
		intXX_t i = m_vars.FindByValue(name);
		if (i < 0) {
			i = m_vars.Push(new Variable(name));
		}
		Variable& var = m_vars[i];
		if ( (index < 0) || (index >= var.values.GetSize()) ) return EC_RANGE_EXCEED;
		var.values[index] = value;
		return EC_SUCCESS;
	}
};

class AFX_EXT_CLASS_XMU Calculator {
	static TCHAR op[];
public:
	enum TOKEN { TOKEN_UNKNOWN = -1, TOKEN_EOL = 0, TOKEN_OPERATOR, TOKEN_CONSTANT, TOKEN_UNKNOWN_VARIABLE, TOKEN_VARIABLE, TOKEN_VARIABLE_ARRAY, TOKEN_PARENS, TOKEN_BRACKET };
	enum ERROR_CODE { EC_RANGE_EXCEED = Variables::EC_RANGE_EXCEED, EC_DIVIDE_BY_ZERO = -100, EC_SYNTAX_ERROR, EC_ALREADY_DEFINED, EC_SUCCESS = 0} ;
	Variables m_vars;

	Calculator();
	virtual ~Calculator();
	BOOL Calc(double& result, CString expr, CString& msg);
	BOOL Eval(double& result, CString expr, CString& msg);
	ERROR_CODE GetVariableValue(double& value, LPCTSTR name, int index = 1);
	ERROR_CODE SetVariableValue(double value, LPCTSTR name, int index = 1);
protected:
	BOOL PreProcess(CString& expr, int& pos, CString& msg);
	TOKEN GetNextToken(LPCTSTR expr, int& pos, CString& value);
	ERROR_CODE Calc(double& result, LPCTSTR expr, int& pos, int op);
};

WARNING_POP()
#pragma pack(pop)

#endif // !defined(AFX_CALCULATOR_H__D467178E_CCE8_11D3_A6A9_A66E9F000000__INCLUDED_)
