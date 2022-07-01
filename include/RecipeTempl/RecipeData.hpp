//
//
//
//		AUTHOR							: MyungSic, Kim	(PWH touched)
//		DATE							: 2015. 06. 08.
//


#pragma once

#include "TList.hpp"
#include "misc/Profile.h"
#include "misc/ListCtrlEd.h"
#include "misc/ListCtrlColumnEx.h"

/* 가상함수 내용
	
	< 순수 가상함수 >
	virtual CString			GetRecipeName(const T* pData) const			= NULL;			return pData->strName;
	virtual CString			SetRecipeName(T* pData, CString strName)	= NULL;			return pData->strName = strName;
	virtual void			DataParsing(T* pData, BOOL bStore)			= NULL;			m_profile변수 사용해야 함 ( Save()함수 호출x )
	
	< 가상함수 >
	virtual void			SetListData(const T* pData, int iItemIndex);				ListCtrl사용할 경우 구현필요
		InsertItem() 호출x
		m_pList->SetItemText(iItemIndex, eDATE, pData->szDate);
		m_pList->SetItemText(iItemIndex, eNAME, pData->szName);
		...
*/

namespace DR {
	typedef enum {
		eSUCCESS = 0,		// Recipe를 지웠음

		eAPPLIED_ERROR,		// 적용된 Recipe라 지워지지 않음
		eLAST_ERROR,		// 마지막 Recipe라 지워지지 않음
		eITEM_ERROR,		// 지워야 할 곳이 없음
		eDELETE_ERROR,		// Recipe를 지우는데 실패함
	} eDELETE_RECIPE;
};

template <typename T, typename LISTCTRL = CListCtrlEd> // ListCtrl 사용 할 경우 SetListData함수 반드시 구현
class IRecipeData {
public:
	typedef DR::eDELETE_RECIPE	eDELETE_RECIPE;
	IRecipeData() { m_pList = NULL; m_iApplied = -1; SetDirectoryPath(_T("../data/Recipe/")); SetExtension(_T(".rcp")); }
	IRecipeData(const IRecipeData& B) { *this = B; }
	virtual ~IRecipeData() { m_data.DeleteAll(); m_profile.GetSections().DeleteAll(); }

public:
	TList<T>		m_data;
	CProfile		m_profile;
	LISTCTRL*		m_pList;

protected:
	int				m_iApplied;
	CString			m_strPath;
	CString			m_strExtension;
	
public:
	void			SetListCtrl(LISTCTRL* pList) { m_pList = pList; }	// SetListData 정의 필요

	CString			SetDirectoryPath(CString strPath);
	CString			GetDirectoryPath() const { return m_strPath; }

	CString			SetExtension(CString strExtension);
	CString			GetExtension() const { return m_strExtension; }		

	CString			GetRecipePath(int iRecipeIndex) const;
	CString			GetRecipePath(CString strRecipeName) const;

	int				AppliedIndex() const;
	CString			AppliedName() const;
	const T*		AppliedData() const	{ int rIndex = AppliedIndex(); return rIndex == -1 ? NULL : &m_data[rIndex]; }
	T*				AppliedData()		{ int rIndex = AppliedIndex(); return rIndex == -1 ? NULL : &m_data[rIndex]; }

	const T*		GetRecipe(int iIndex) const	{ if (iIndex < 0 || iIndex >= N()) return NULL; return &(*this)[iIndex]; }
	T*				GetRecipe(int iIndex)		{ if (iIndex < 0 || iIndex >= N()) return NULL; return &(*this)[iIndex]; }
	const T*		GetRecipe(CString strRecipeName) const;
	T*				GetRecipe(CString strRecipeName);

	int				NewRecipe();
	int				CopyRecipe(int iRecipeIndex);
	int				CopyRecipe(CString strRecipeName);
	int				CopyRecipe();
	eDELETE_RECIPE	DeleteRecipe(int iRecipeIndex);
	eDELETE_RECIPE	DeleteRecipe(CString strRecipeName);
	eDELETE_RECIPE	DeleteRecipe();
	BOOL			ApplyRecipe(int iRecipeIndex);
	BOOL			ApplyRecipe(CString strRecipeName);
	BOOL			ApplyRecipe();
	BOOL			SaveRecipe() { return SyncData(TRUE); }
	BOOL			LoadRecipe() { return SyncData(FALSE); }
	
	BOOL			SyncData(BOOL bStore = TRUE);
	BOOL			BackupDir(CString strDirPath) const;							// Directory
	BOOL			BackupFile(int iRecipeIndex, CString strFilePath) const;		// File
	BOOL			BackupFile(CString strRecipeName, CString strFilePath) const;	// File

	BOOL			IsEmpty() const { return m_data.IsEmpty(); }
	int				N() const  { return m_data.N(); }

	BOOL			CopyDirectory(LPCTSTR lpSrcPath, LPCTSTR lpDestPath) const;
	BOOL			DeleteDirectory(LPCTSTR lpDirPath) const;

public:
	T&			operator[](int iIndex)			{ return m_data[iIndex]; }
	const T&	operator[](int iIndex) const	{ return m_data[iIndex]; }

	// ListCtrl은 복사 안함
	IRecipeData& operator=(const IRecipeData& B) {
		if (&B == this) return *this;
		m_data			= B.m_data;
		m_strPath		= B.m_strPath;
		m_profile		= B.m_profile;
		m_strExtension	= B.m_strExtension;
		m_iApplied		= B.m_iApplied;
		return *this;
	}

//<< 가상함수
protected:
	// 반드시 구현
	virtual CString			GetRecipeName(const T* pData) const			= NULL;
	virtual CString			SetRecipeName(T* pData, CString strName)	= NULL;
	virtual void			DataParsing(T* pData, BOOL bStore)			= NULL;

	// ListCtrl 사용 할 경우 밖에서 구현
	virtual void			SetListData(const T* pData, int iItemIndex) {}

//>>

protected:
	BOOL SyncApplyRecipe(CString& strName, BOOL bStore = TRUE) const {
		CProfile profile(m_strPath + _T("RecipeInfo.ini"));
		CProfileSection& section = profile.GetSection(_T("Applied Recipe"));
		section.SyncItemValue(bStore, _T("Name"), strName);

		if (bStore) {
			if (!profile.Save())
				return FALSE;
		} else {
			if (strName.IsEmpty())
				return FALSE;
		}

		return TRUE;
	}

	int GetRecipeIndex(CString strRecipeName) const {
		for (int i = 0; i < m_data.N(); ++i) {
			if (strRecipeName.Compare(GetRecipeName(&m_data[i])) == 0)
				return i;
		}

		return -1;
	}

	void PathReplace(CString& strPath) const {
		strPath.Replace(_T("\\"), _T("/"));
		if (strPath.Right(1) != _T("/"))
			strPath += _T("/");
	}

};

template <typename T, typename LISTCTRL>
BOOL IRecipeData<T, LISTCTRL>::SyncData(BOOL bStore)
{
	CFileFind ff;
	CString strSection(_T("Recipe Index"));
	CProfile rInfo(m_strPath + _T("RecipeInfo.ini"));

	int nRecipe = bStore ? m_data.N() : 0;
	BOOL bFind = ff.FindFile(m_strPath + Format(_T("*%s"), m_strExtension));
	if (bStore) {
		while (bFind) {
			bFind = ff.FindNextFile();
			DeleteFile(m_strPath + ff.GetFileName());
		}

		rInfo.DeleteSection(strSection);
	} else {
		while (bFind) {
			++nRecipe;
			bFind = ff.FindNextFile();
		}
	
		m_data.DeleteAll();
		if (m_pList)
			m_pList->DeleteAllItems();
	}

	if (!nRecipe) {
		ff.Close();
		return FALSE;
	}

	T* pData = NULL;
	for (int i = 0; i < nRecipe; ++i) {
		if (bStore) {
			pData = &m_data[i];
			if (!pData)
				continue;

			m_profile.Init(m_strPath + GetRecipeName(pData) + m_strExtension);
			m_profile.GetSections().DeleteAll();
		} else {
			int nIndex = rInfo.GetSection(strSection).GetItems().N();
			if (nIndex <= i)
				break;

			CString strName = rInfo.GetSection(strSection).GetItem(i).GetValue();
			CString strPath = m_strPath + strName + m_strExtension;
			if (!ff.FindFile(strPath))
				continue;

			pData = new T;
			SetRecipeName(pData, strName);
			m_profile.Init(strPath);
		}

		DataParsing(pData, bStore);
		if (bStore) {
			if (!m_profile.Save()) {
				ff.Close();
				return FALSE;
			}

			rInfo.SetProfileValue(strSection, Format(_T("Recipe_%d"), i), GetRecipeName(pData));
			if (!rInfo.Save()) {
				ff.Close();
				return FALSE;
			}
		} else {
			m_data.Attach(pData);
			if (m_pList) {
				m_pList->InsertItem(i, _T(""));
				SetListData(pData, i);
			}
		}
	}

	if (!bStore)
		m_iApplied = AppliedIndex();
	else
		ApplyRecipe(m_iApplied);

	ff.Close();
	return TRUE;
}

template <typename T, typename LISTCTRL>
BOOL IRecipeData<T, LISTCTRL>::BackupDir(CString strDirPath) const
{
	strDirPath.Trim();
	if (strDirPath.IsEmpty() || m_strPath.IsEmpty())
		return FALSE;

	PathReplace(strDirPath);
	DeleteDirectory(strDirPath);
	CreateIntermediateDirectory(strDirPath);
	return CopyDirectory(m_strPath, strDirPath);
}

template <typename T, typename LISTCTRL>
BOOL IRecipeData<T, LISTCTRL>::BackupFile(int iRecipeIndex, CString strFilePath) const
{
	strFilePath.Trim();
	if (strFilePath.IsEmpty() || m_strPath.IsEmpty())
		return FALSE;
	if (iRecipeIndex < 0 || m_data.N() <= iRecipeIndex)
		return FALSE;

	CString strF, strN, strP;
	SplitPath(strFilePath, strF, strN);
	CreateIntermediateDirectory(strF);

	strP = GetRecipePath(iRecipeIndex);
	if (strP.IsEmpty())
		return FALSE;

	return CopyFile(strP, strFilePath, FALSE);
}

template <typename T, typename LISTCTRL>
BOOL IRecipeData<T, LISTCTRL>::BackupFile(CString strRecipeName, CString strFilePath) const
{
	return BackupFile(GetRecipeIndex(strRecipeName), strFilePath);
}

template <typename T, typename LISTCTRL>
BOOL IRecipeData<T, LISTCTRL>::CopyDirectory(LPCTSTR lpOriginPath, LPCTSTR lpCopyPath) const
{
	if (!lpOriginPath || !*lpOriginPath)
		return FALSE;
	if (!lpCopyPath || !*lpCopyPath)
		return FALSE;

	CString strOriginPath(lpOriginPath); PathReplace(strOriginPath);
	CString strCopyPath(lpCopyPath); PathReplace(strCopyPath);
	CString strRoot = Format(_T("%s*.*"), strOriginPath);

	CFileFind ff;
	BOOL bResult = ff.FindFile(strRoot);
	if (!bResult)
		return FALSE;

	while (bResult) {
		bResult = ff.FindNextFile();
		if (ff.IsDots())
			continue; 

		if (ff.IsDirectory()) {
			CreateIntermediateDirectory(strCopyPath += ff.GetFileName());
			CopyDirectory(ff.GetFilePath(), strCopyPath);
		}
		else {
			if (!CopyFile(ff.GetFilePath(), strCopyPath + ff.GetFileName(), FALSE))
				return FALSE;
		}
	}

	ff.Close();
	return TRUE;
}

template <typename T, typename LISTCTRL>
BOOL IRecipeData<T, LISTCTRL>::DeleteDirectory(LPCTSTR lpDirPath) const
{
	if (!lpDirPath || !*lpDirPath)
		return FALSE;

	CString strDirPath(lpDirPath); PathReplace(strDirPath);
	CString strRoot = Format(_T("%s*.*"), strDirPath);

	CFileFind ff;
	BOOL bResult = ff.FindFile(strRoot);
	if (!bResult)
		return FALSE;

	while (bResult) {
		bResult = ff.FindNextFile();
		if (ff.IsDots())
			continue;

		if (ff.IsDirectory())
			DeleteDirectory(ff.GetFilePath());
		else {
			if (!DeleteFile(ff.GetFilePath()))
				return FALSE;
		}
	}

	strRoot = ff.GetRoot();
	ff.Close();
	return RemoveDirectory(strRoot);
}

template <typename T, typename LISTCTRL>
int IRecipeData<T, LISTCTRL>::NewRecipe()
{
	int iCount = 1;
	CString strNewName;
	T* pData = new T;
	SetRecipeName(pData, _T("New Recipe"));
	for (int i = 0; i < m_data.N(); ++i) {
		if (GetRecipeName(pData).Compare(GetRecipeName(&m_data[i])) == 0) {
			strNewName.Format(_T("New Recipe_%d"), iCount++);
			SetRecipeName(pData, strNewName);
			i = -1;
		}
	}

	int iIndex = m_data.Attach(pData);
	if (m_pList) {
		m_pList->InsertItem(iIndex, _T(""));
		SetListData(pData, iIndex);
	}

	return iIndex;
}

template <typename T, typename LISTCTRL>
int IRecipeData<T, LISTCTRL>::CopyRecipe(int iRecipeIndex)
{
	if (iRecipeIndex < 0 || m_data.N() <= iRecipeIndex)
		return -1;

	int iCount = 1;
	CString strNewName;
	T* pData = new T(m_data[iRecipeIndex]);
	for (int i = 0; i < m_data.N(); ++i) {
		if (GetRecipeName(pData).Compare(GetRecipeName(&m_data[i])) == 0) {
			strNewName.Format(_T("%s_%d"), GetRecipeName(&m_data[iRecipeIndex]), iCount++);
			SetRecipeName(pData, strNewName);
			i = -1;
		}
	}

	int iIndex = m_data.Attach(pData);
	if (m_pList) {
		m_pList->InsertItem(iIndex, _T(""));
		SetListData(pData, iIndex);
	}

	return iIndex;
}

template <typename T, typename LISTCTRL>
int IRecipeData<T, LISTCTRL>::CopyRecipe(CString strRecipeName)
{
	return CopyRecipe(GetRecipeIndex(strRecipeName));
}

template <typename T, typename LISTCTRL>
int IRecipeData<T, LISTCTRL>::CopyRecipe()
{
	int iRecipeIndex = -1;
	if (m_pList)
		iRecipeIndex = m_pList->GetSelectionMark();

	return CopyRecipe(iRecipeIndex);
}

template <typename T, typename LISTCTRL>
DR::eDELETE_RECIPE IRecipeData<T, LISTCTRL>::DeleteRecipe(int iRecipeIndex)
{
	if (iRecipeIndex < 0 || m_data.N() <= iRecipeIndex)
		return DR::eITEM_ERROR;
	if (iRecipeIndex == m_iApplied)
		return DR::eAPPLIED_ERROR;
	if (iRecipeIndex == AppliedIndex())
		return DR::eAPPLIED_ERROR;

	if (m_data.N() > 1) {
		if (!m_data.Delete(iRecipeIndex))
			return DR::eDELETE_ERROR;
	}
	else
		return DR::eLAST_ERROR;

	if (m_pList)
		m_pList->DeleteItem(iRecipeIndex);

	m_iApplied += m_iApplied > iRecipeIndex ? -1 : 0;
	return DR::eSUCCESS;
}

template <typename T, typename LISTCTRL>
DR::eDELETE_RECIPE IRecipeData<T, LISTCTRL>::DeleteRecipe(CString strRecipeName)
{
	return DeleteRecipe(GetRecipeIndex(strRecipeName));
}

template <typename T, typename LISTCTRL>
DR::eDELETE_RECIPE IRecipeData<T, LISTCTRL>::DeleteRecipe()
{
	int iRecipeIndex = -1;
	if (m_pList)
		iRecipeIndex = m_pList->GetSelectionMark();

	return DeleteRecipe(iRecipeIndex);
}

template <typename T, typename LISTCTRL>
BOOL IRecipeData<T, LISTCTRL>::ApplyRecipe(int iRecipeIndex)
{
	if (iRecipeIndex < 0 || m_data.N() <= iRecipeIndex)
		return FALSE;

	BOOL bResult = SyncApplyRecipe(GetRecipeName(&m_data[iRecipeIndex]));
	if (bResult)
		m_iApplied = iRecipeIndex;

	return bResult;
}

template <typename T, typename LISTCTRL>
BOOL IRecipeData<T, LISTCTRL>::ApplyRecipe(CString strRecipeName)
{
	return ApplyRecipe(GetRecipeIndex(strRecipeName));
}

template <typename T, typename LISTCTRL>
BOOL IRecipeData<T, LISTCTRL>::ApplyRecipe()
{
	int iRecipeIndex = -1;
	if (m_pList)
		iRecipeIndex = m_pList->GetSelectionMark();

	return ApplyRecipe(iRecipeIndex);
}

template <typename T, typename LISTCTRL>
int IRecipeData<T, LISTCTRL>::AppliedIndex() const
{
	CString strName = AppliedName();
	if (strName.IsEmpty())
		return -1;

	return GetRecipeIndex(strName);
}

template <typename T, typename LISTCTRL>
CString IRecipeData<T, LISTCTRL>::AppliedName() const
{
	CString strName;
	BOOL bResult = SyncApplyRecipe(strName, FALSE);
	return (bResult == TRUE) ? strName : _T("");
}

template <typename T, typename LISTCTRL>
const T* IRecipeData<T, LISTCTRL>::GetRecipe(CString strRecipeName) const
{
	for (int i = 0; i < m_data.N(); ++i) {
		if (strRecipeName.Compare(GetRecipeName(&m_data[i])) == 0)
			return &m_data[i];
	}

	return NULL;
}

template <typename T, typename LISTCTRL>
T* IRecipeData<T, LISTCTRL>::GetRecipe(CString strRecipeName)
{
	for (int i = 0; i < m_data.N(); ++i) {
		if (strRecipeName.Compare(GetRecipeName(&m_data[i])) == 0)
			return &m_data[i];
	}

	return NULL;
}

template <typename T, typename LISTCTRL>
CString IRecipeData<T, LISTCTRL>::GetRecipePath(int iRecipeIndex) const
{
	if (iRecipeIndex < 0 || m_data.N() <= iRecipeIndex)
		return _T("");

	CFileFind ff;
	CString strName = GetRecipeName(&m_data[iRecipeIndex]);
	if (ff.FindFile(m_strPath+strName+m_strExtension)) {
		ff.FindNextFile();
		return ff.GetFilePath();
	}

	return _T("");
}

template <typename T, typename LISTCTRL>
CString IRecipeData<T, LISTCTRL>::GetRecipePath(CString strRecipeName) const
{
	return GetRecipePath(GetRecipeIndex(strRecipeName));
}

template <typename T, typename LISTCTRL>
CString IRecipeData<T, LISTCTRL>::SetDirectoryPath(CString strPath)
{
	strPath.Trim();
	if (strPath.IsEmpty()) {
		CreateIntermediateDirectory(m_strPath);
		return m_strPath;
	}

	PathReplace(strPath);
	m_strPath = strPath;
	CreateIntermediateDirectory(m_strPath);
	return m_strPath;
}

template <typename T, typename LISTCTRL>
CString IRecipeData<T, LISTCTRL>::SetExtension(CString strExtension)
{
	strExtension.Trim();
	if (strExtension.IsEmpty()) {
		strExtension = m_strExtension;
		return m_strExtension;
	}

	if (strExtension.Left(1) != _T("."))
		strExtension.Insert(0, _T("."));

	return m_strExtension = strExtension;
}
