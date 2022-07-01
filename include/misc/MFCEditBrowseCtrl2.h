#pragma once

#include "AFX_EXT_MISC.h"
#include <AFXEditBrowseCtrl.h>

// CMFCEditBrowseCtrl2

class AFX_EXT_CLASS_MISC CMFCEditBrowseCtrl2 : public CMFCEditBrowseCtrl {
	DECLARE_DYNAMIC(CMFCEditBrowseCtrl2)
private:
	BOOL m_bUpdated;
protected:
#if (_MFC_VER <= 0x0C00)
	DWORD m_dwFileDialogFlags;
#endif
#if (_MFC_VER <= 0x0A00)
	CString m_strBrowseFolderTitle;
	UINT m_ulBrowseFolderFlags;
#endif

public:
	CString m_strInitialFolder;

	CMFCEditBrowseCtrl2();
	virtual ~CMFCEditBrowseCtrl2();

	virtual void OnBrowse();
	virtual void OnDrawBrowseButton(CDC* pDC, CRect rect, BOOL bIsButtonPressed, BOOL bIsButtonHot);
	virtual void OnChangeLayout();
	virtual void OnAfterUpdate();
	virtual BOOL OnIllegalFileName(CString& strFileName);

	void EnableFileBrowseButton(LPCTSTR lpszDefExt = NULL, LPCTSTR lpszFilter = NULL, DWORD dwFlags = OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_NOCHANGEDIR);
	void EnableFolderBrowseButton(LPCTSTR lpszBrowseFolderTitle = NULL, UINT ulBrowseFolderFlags = BIF_RETURNONLYFSDIRS);
protected:
	DECLARE_MESSAGE_MAP()
};
