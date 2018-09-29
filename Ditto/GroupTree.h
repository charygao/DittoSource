#if !defined(AFX_GROUPTREE_H__A1F01358_5440_45A8_B402_3F32F46FCAA5__INCLUDED_)
#define AFX_GROUPTREE_H__A1F01358_5440_45A8_B402_3F32F46FCAA5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// GroupTree.h : header file
//
#include "Accels.h"

/////////////////////////////////////////////////////////////////////////////
// CGroupTree window

class CGroupTree : public CTreeCtrl
{
// Construction
public:
	CGroupTree();

// Attributes
public:

// Operations
public:
	void FillTree();
	void SetNotificationWndEx(HWND hWnd)	{ m_NotificationWnd = hWnd;	}
	bool m_bHide;
	int m_selectedFolderID;
	int GetSelectedTree();
	bool AddNode(CString csText, int id);
	bool m_showRightClickMenu;

protected:
	void FillTree(int parentId, HTREEITEM hParent);
	void SendToParent(int parentId);
	UINT GetSelectedCount() const;
	bool CheckActions(MSG * pMsg);
	bool DoAction(DWORD actionId);
	bool DoActionNewGroup();
	bool DoActionDeleteSelected();
	bool DoActionClipProperties();
	void UpdateMenuShortCut(CCmdUI *pCmdUI, DWORD action);

	HWND m_NotificationWnd;
	CBitmap m_bmOpenFolder;
	CBitmap m_bmClosedFolder;
	bool m_bSendAllready;
	CAccels m_actions;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGroupTree)
	public:
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
	virtual BOOL PreTranslateMessage(MSG *pMsg);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CGroupTree();

	// Generated message map functions
protected:
	//{{AFX_MSG(CGroupTree)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSelchanged(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	afx_msg void OnDblclk(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnKeydown(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnRclickQuickPaste(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
	// Generated OLE dispatch map functions
	//{{AFX_DISPATCH(CGroupTree)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_DISPATCH
public:
	afx_msg void OnMenuNewgroup32896();
	afx_msg void OnMenuDeletegroup();
	afx_msg void OnMenuProperties32898();
	afx_msg void OnUpdateMenuNewgroup32896(CCmdUI *pCmdUI);
	afx_msg void OnUpdateMenuDeletegroup(CCmdUI *pCmdUI);
	afx_msg void OnUpdateMenuProperties32898(CCmdUI *pCmdUI);
	afx_msg void OnInitMenuPopup(CMenu *pPopupMenu, UINT nIndex,BOOL bSysMenu);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GROUPTREE_H__A1F01358_5440_45A8_B402_3F32F46FCAA5__INCLUDED_)
