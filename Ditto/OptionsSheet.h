#if !defined(AFX_OPTIONSSHEET_H__D8A13849_DBC6_4CD6_A981_E572ECDC2E94__INCLUDED_)
#define AFX_OPTIONSSHEET_H__D8A13849_DBC6_4CD6_A981_E572ECDC2E94__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// OptionsSheet.h : header file
//
//#include "ShowTaskBarIcon.h"


/////////////////////////////////////////////////////////////////////////////
// COptionsSheet

class COptionsSheet : public CPropertySheet
{
	DECLARE_DYNAMIC(COptionsSheet)

// Construction
public:
	COptionsSheet(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);

	void SetNotifyWnd(HWND hWnd);

// Attributes
public:

// Operations
public:

	BOOL m_themeChanged;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COptionsSheet)
	public:
	virtual BOOL OnInitDialog();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~COptionsSheet();

	// Generated message map functions
protected:

	HWND m_hWndParent;

	CPropertyPage *m_pKeyBoardOptions;
	CPropertyPage *m_pGeneralOptions;
	CPropertyPage *m_pQuickPasteOptions;
	CPropertyPage *m_pUtilites;
	CPropertyPage *m_pStats;
	CPropertyPage *m_pTypes;
	CPropertyPage *m_pAbout;
	CPropertyPage *m_pFriends;
	CPropertyPage *m_pCopyBuffers;
	CPropertyPage *m_pQuickPasteShortCuts;

	//CShowTaskBarIcon m_taskBar;


	//{{AFX_MSG(COptionsSheet)
		// NOTE - the ClassWizard will add and remove member functions here.
	afx_msg void OnDestroy();
	afx_msg void OnNcDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OPTIONSSHEET_H__D8A13849_DBC6_4CD6_A981_E572ECDC2E94__INCLUDED_)
