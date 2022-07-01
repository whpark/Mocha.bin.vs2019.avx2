/////////////////////////////////////////////////////////////////////////////
//
//
// Stream Preprocessor (Token Parser)
// PWH.
// 2009.07.15.
//
/////////////////////////////////////////////////////////////////////////////

#pragma once

#include "AFX_EXT_MISC.h"

#include "TList.hpp"
#include "ArchiveU.h"
#include "xUtil.h"

namespace stream_processor {

	class CSPException;
	class CSPMacro;
	class CSPToken;
	class CSPFile;
	class CSPWord;
	class CStreamPreprocessor;

	enum eSP_TYPE { SPT_NONE, SPT_CONST_INT, SPT_CONST_FLOAT, SPT_CONST_CHARACTER, SPT_CONST_STRING, SPT_PUNCTUATOR, SPT_OPERATOR, SPT_RESERVED_WORD, SPT_WORD };


	//-----------------------------------------------------------------------------

	class CSPException : public __ExceptionBase__ {
	protected:
		int m_eErrorCode;
	public:
		CSPException(int eErrorCode, const char* pszPosition, const char* pszMessage = NULL) : m_eErrorCode(eErrorCode), __ExceptionBase__(pszPosition, pszMessage) { }
		int GetErrorCode() { return m_eErrorCode; }
	};

	//-----------------------------------------------------------------------------

	class CSPWord {
	public:
		CString m_strWord;
		eSP_TYPE m_eType;
		int m_eToken;
	public:
		CSPWord(LPCTSTR pszWord = NULL, eSP_TYPE eType = SPT_NONE, int eToken = 0) : m_strWord(pszWord), m_eType(eType), m_eToken(eToken) { }
	};

	//-----------------------------------------------------------------------------

	class CSPMacro {
	public:
		CString m_strName;
		CString m_strValue;
	public:
		CSPMacro(LPCTSTR pszName = NULL, LPCTSTR pszValue = NULL) : m_strName(pszName), m_strValue(pszValue) { }

		bool operator == (LPCTSTR pszName) const {
			return m_strName == pszName;
		}
	};

	//-----------------------------------------------------------------------------

	class CSPToken {
	public:
		CString m_strName;
		int m_eToken;
	public:
		CSPToken(LPCTSTR pszName = NULL, int eToken = 0) : m_strName(pszName), m_eToken(eToken) { }

		bool operator == (LPCTSTR psz) const {
			return (m_strName == psz);
		}
	};

	//-----------------------------------------------------------------------------

	class CSPIfClause {
	protected:
		BOOL m_bValue;
		BOOL m_bInTrueCase;
	public:
		CSPIfClause(BOOL bValue = TRUE) : m_bValue(bValue), m_bInTrueCase(TRUE) { }
		BOOL EnterElseClause() { if (!m_bInTrueCase) return FALSE; m_bInTrueCase = FALSE; return TRUE; }
		BOOL CheckIfClause() const { return CompareBoolean(m_bValue, m_bInTrueCase); }
	};

	//-----------------------------------------------------------------------------

	class CSPFile {
	private:
		mutable std::vector<BYTE> m_buf;
	protected:
		TRefPointer<CFile> m_rFile;
		TRefPointer<CArchiveU> m_rArchive;
		int m_nCurrentLine;
		int m_nCurrentColumn;
		CString m_strBuffer;
		CString m_strFilePath;
		TList<CSPIfClause> m_ifClauses;

	public:
		CSPFile(LPCTSTR pszFilePath = NULL) {
			m_nCurrentLine = 0;
			m_nCurrentColumn = 0;
			if (pszFilePath)
				Open(pszFilePath);
		}
		~CSPFile() {
			Close();
		}

		BOOL IsOpen() const					{ return ( m_rFile && (m_rFile->m_hFile != NULL) && ((HANDLE)m_rFile->m_hFile != INVALID_HANDLE_VALUE) && m_rArchive ) ? TRUE : FALSE; }
		LPCTSTR GetFilePath() const			{ return m_strFilePath; }
		int GetLineNo() const				{ return m_nCurrentLine; }
		int GetColumn() const				{ return m_nCurrentColumn; }

		BOOL Open(LPCTSTR pszFilePath);
		BOOL Open(const BYTE* data, int nSize, BOOL bCopy);
		void Close();

		BOOL ReadChar(TCHAR& c);
		BOOL Flush(CString& str);

		void EnterIfClause(BOOL bValue)		{ m_ifClauses.Push(new CSPIfClause(bValue)); }
		BOOL ExitIfClause()					{ return m_ifClauses.DeleteLast(); }
		CSPIfClause* GetCurrentIfClause()	{ return m_ifClauses.Last(); }
		BOOL CheckIfClauseCondition() const;

	protected:
		BOOL ReadLine();
	};

	//-----------------------------------------------------------------------------

	class CSPStreamIn {
	protected:
		CString m_strPushed;
		CString m_strBaseFolder;
		TList<CSPFile> m_files;
		CStrings m_strFileHistory;
	private:
		mutable struct {
			CString strFilePathLast;
			int nCurrentLine;
			int nCurrentColumn;
		} m_lastFile;

	public:
		CSPStreamIn() {
			m_lastFile.nCurrentLine = 0;
			m_lastFile.nCurrentColumn = 0;
		}

		BOOL SearchFileHistory(LPCTSTR pszFilePath);
		void ClearFileHsitory();

		BOOL Open(LPCTSTR pszFileName, LPCTSTR pszBaseFolder = NULL);
		BOOL Open(const BYTE* data, int nSize, BOOL bCopy, LPCTSTR pszBaseFolder = NULL);
		BOOL Close();
		void CloseAll();
		BOOL DiscardCurrentFile();

		BOOL GetChar(TCHAR& c, int eErrorCode, BOOL bContinuous);
		void Push(LPCTSTR pszString);
		void Push(TCHAR c);

		BOOL FlushLine();

		BOOL GetLine(CString& str);


		BOOL IsOpen() const					{ if (m_files.Last()) return m_files.Last()->IsOpen(); return FALSE; }
		BOOL ReadChar(TCHAR& c)				{ if (m_files.N()) return m_files.Last()->ReadChar(c); return FALSE; }
		BOOL Flush(CString& str)			{ if (m_files.N()) return m_files.Last()->Flush(str); return FALSE; }

		void EnterIfClause(BOOL bValue)		{ if (m_files.Last()) m_files.Last()->EnterIfClause(bValue); }
		BOOL ExitIfClause()					{ if (m_files.Last()) return m_files.Last()->ExitIfClause(); else return FALSE; }
		CSPIfClause* GetCurrentIfClause()	{ if (m_files.Last()) return m_files.Last()->GetCurrentIfClause(); else return NULL; }
		BOOL CheckIfClauseCondition() const	{ if (m_files.Last()) return m_files.Last()->CheckIfClauseCondition(); else return FALSE; }

		LPCTSTR GetCurrentFilePath()		{ if (m_files.Last()) return m_files.Last()->GetFilePath(); return NULL; }
		int GetCurrentLineNo()				{ if (m_files.Last()) return m_files.Last()->GetLineNo(); return -1; }
		int GetCurrentColumn()				{ if (m_files.Last()) return m_files.Last()->GetColumn(); return -1; }

		void PrintError(const TCHAR* pszFormat, ...);
		void PrintErrorV(const TCHAR* pszFormat, va_list argList);
	};

	//=============================================================================

	class AFX_EXT_CLASS_MISC CStreamPreprocessor {
	protected:
		CString m_strBaseFolder;
		CSPStreamIn m_streamIn;

	protected:
		TList<CSPMacro> m_macros;
		BOOL m_bNewLine;

		TList<CSPWord> m_words;

	protected:
		TList<CSPToken> m_tokensReservedWord;
		TList<CSPToken> m_tokensOperator;

	protected:
		CString m_strOperators;
		CString m_strPunctuators;
		CString m_strWhiteSpace;
		CString m_strCommentFlagLine;
		CString m_strCommentFlagOpening;
		CString m_strCommentFlagClosing;

	public:
		CStreamPreprocessor(
			LPCTSTR pszFileName = NULL,
			LPCTSTR pszOperators = _T("!%&*+,-./:<=>?@^|~"),
			LPCTSTR pszPunctuators = _T("!%&*+,-./:<=>?@^|~\'\"") _T("();[]{}"),
			LPCTSTR pszWhiteSpace = _T(" \t\r\n"),
			LPCTSTR pszCommentFlagLine = _T("//"),
			LPCTSTR pszCommentFlagOpening = _T("/*"),
			LPCTSTR pszCommentFlagClosing = _T("*/")
			)
		{
			Load(pszFileName);
			m_strOperators = pszOperators;
			m_strPunctuators = pszPunctuators;
			m_strWhiteSpace = pszWhiteSpace;
			m_strCommentFlagLine = pszCommentFlagLine;
			m_strCommentFlagOpening = pszCommentFlagOpening;
			m_strCommentFlagClosing = pszCommentFlagClosing;
		}

		BOOL Load(LPCTSTR pszFullPath);
		BOOL OpenMemBlock(const BYTE* data, int nSize, BOOL bCopy, LPCTSTR pszBaseFolder = NULL);
		void CloseAll();
		BOOL DiscardCurrentFile();

	public:
		LPCTSTR GetCurrentFileName()				{ return m_streamIn.GetCurrentFilePath(); }
		int GetCurrentLineNo()						{ return m_streamIn.GetCurrentLineNo(); }
		int GetCurrentColumn()						{ return m_streamIn.GetCurrentColumn(); }
		BOOL SearchFileHistory(LPCTSTR pszFilePath)	{ return m_streamIn.SearchFileHistory(pszFilePath); }

	public:
		BOOL PushMacro(LPCTSTR pszName, LPCTSTR pszValue = NULL);
		BOOL PopMacro(LPCTSTR pszName);
		BOOL GetWord(CString& strWord, eSP_TYPE& eType, int& eToken, BOOL bExpandMacro = TRUE);
		void PushWord(LPCTSTR pszWord, eSP_TYPE eType, int eToken) {
			m_words.Push(new CSPWord(pszWord, eType, eToken));
		}
		BOOL ConsumeWord(LPCTSTR pszWord = NULL, eSP_TYPE eType = SPT_NONE, int eToken = -1, BOOL bRestore = FALSE);
		BOOL ConsumeType(eSP_TYPE eType, CString& strWord, int eToken = -1, BOOL bRestore = FALSE);
		BOOL GetLine(CString& str) { return m_streamIn.GetLine(str); }
	protected:
		BOOL PreCheck(LPCTSTR pszValue, TCHAR c = 0, BOOL bRestore = FALSE, BOOL bSkipSpaces = FALSE);
		BOOL GetChar(TCHAR& c, int eErrorCode = 1, BOOL bContinuous = FALSE) { return m_streamIn.GetChar(c, eErrorCode, bContinuous); }
		BOOL FlushLine() { return m_streamIn.FlushLine(); }

	public:
		BOOL AddReservedWord(LPCTSTR pszReservedWord, int eToken) {
			if (m_tokensReservedWord.FindByValue(pszReservedWord) < 0)
				m_tokensReservedWord.Push(new CSPToken(pszReservedWord, eToken));
			else
				return FALSE;
			return TRUE;
		}
		BOOL AddOperator(LPCTSTR pszOperator, int eToken) {
			if (m_tokensOperator.FindByValue(pszOperator) < 0)
				m_tokensOperator.Push(new CSPToken(pszOperator, eToken));
			else
				return FALSE;

			for (LPCTSTR pos = pszOperator; *pos; pos++) {
				if (m_strOperators.Find(*pos) < 0)
					m_strOperators += *pos;
				for (int iIndex = m_strPunctuators.Find(*pos); iIndex >= 0; iIndex = m_strPunctuators.Find(*pos))
					m_strPunctuators.Delete(iIndex);
			}
			return TRUE;
		}

	public:
		BOOL PrintError(LPCTSTR pszFormat, ...);
	};

};	// namespace
