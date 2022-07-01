/////////////////////////////////////////////////////////////////////////////
//
//
// HSMS
// PWH.
// 2009.03.27.
//
/////////////////////////////////////////////////////////////////////////////

#pragma once

#include "AFX_EXT_HSMS.h"

#include "misc/StreamPreprocessor.h"
#include "SECSMessage.h"
#include "qtl/qtl.hpp"

#pragma pack(push, 8)

//enum eHSMS_TOKEN_OPERATOR {
//	// = : ; { } [ ] < > 
//};


//---------------------------------------------------------------------------

class AFX_EXT_CLASS_HSMS CSECSMessageTemplate {
protected:
	qtl::TUniquePtrVector<CSECSVariableTemplate>	m_SECSVariableTemplates;
	qtl::TUniquePtrVector<CSECSTransactionTemplate>	m_SECSTransactionTemplates;
	unsigned short m_iSessionID;

public:
	CSECSMessageTemplate(LPCTSTR pszPathTemplateFile = NULL, unsigned short iSessionID = 0xFFFF/*Single Session. same as '1'*/) {
		m_iSessionID = (iSessionID == 0xFFFF) ? 1 : iSessionID;
		LoadTemplate(pszPathTemplateFile);
		m_SECSVariableTemplates.SetInterlockMode(true);
		m_SECSTransactionTemplates.SetInterlockMode(true);
	}
	CSECSMessageTemplate(const CSECSMessageTemplate& B) {
		*this = B;
	}
	CSECSMessageTemplate& operator = (const CSECSMessageTemplate& B) {
		if (this == &B)
			return *this;
		m_iSessionID = B.m_iSessionID;
		m_SECSVariableTemplates = B.m_SECSVariableTemplates;
		m_SECSTransactionTemplates = B.m_SECSTransactionTemplates;
		return *this;
	}

public:
	unsigned short GetSessionID() const { return m_iSessionID; }
	BOOL operator == (unsigned short iSessionID) const { return m_iSessionID == iSessionID; }

public:
	typedef enum { SCHEMA_TYPE_BY_EXT, SCHEMA_TYPE_STRUCT, SCHEMA_TYPE_BRACKET, SCHEMA_TYPE_SML, } eSCHEMA_TYPE;
	BOOL LoadTemplate(LPCTSTR pszPathTemplateFile, eSCHEMA_TYPE eSchemaType = SCHEMA_TYPE_BY_EXT);
	BOOL LoadTemplateStruct(LPCTSTR pszPathTemplateFile);
	BOOL LoadTemplateBracket(LPCTSTR pszPathTemplateFile);
	BOOL LoadTemplateSML(LPCTSTR pszPathTemplateFile);

	const qtl::TUniquePtrVector<CSECSVariableTemplate>&		GetSECSVariableTemplates() const { return m_SECSVariableTemplates; }
	const qtl::TUniquePtrVector<CSECSTransactionTemplate>&	GetSECSTransactionTemplates() const { return m_SECSTransactionTemplates; }

protected:
	BOOL GetVariableName(stream_processor::CStreamPreprocessor& sp, CSECSVariableTemplate& varTemplate, BOOL bCheckArray = TRUE);
	BOOL GetConstant(stream_processor::CStreamPreprocessor& sp, CSECSVariableTemplate& varTemplate);
	BOOL GetConstantList(stream_processor::CStreamPreprocessor& sp, CSECSVariableTemplate& varTemplate);
	BOOL GetVariable(stream_processor::CStreamPreprocessor& sp, CSECSVariableTemplate& varTemplate);
	BOOL GetVariableTypeFromToken(int eToken, eSECS_VARIABLE_TYPE& eSECSVariableType, int* pItemSize = NULL);

	BOOL GetTransaction(stream_processor::CStreamPreprocessor& sp, CSECSTransactionTemplate& varTemplate);

protected:
	BOOL GetVariableBracket(stream_processor::CStreamPreprocessor& sp, CSECSVariableTemplate& varTemplate);

protected:
	BOOL GetVariableSML(stream_processor::CStreamPreprocessor& sp, CSECSVariableTemplate& varTemplate);

public:
	BOOL StoreAsLegacyForm(CArchiveU& ar) const;
	BOOL StoreAsStruct(CArchiveU& ar) const;
};

//---------------------------------------------------------------------------

#pragma pack(pop)
