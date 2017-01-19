// ChildFrm.h : interface of the CChildFrame class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_CHILDFRM_H__61A31CEF_9707_11D1_ACAB_0000E823DBFD__INCLUDED_)
#define AFX_CHILDFRM_H__61A31CEF_9707_11D1_ACAB_0000E823DBFD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "MySplitter.h"
#include "CTaiShanDoc.h"
#include "CustomMenu.h"


class CChildFrame : public CMDIChildWnd
{
	DECLARE_DYNCREATE(CChildFrame)
public:
	CChildFrame();
	void CreateF9();
	void CreateF10();
	CSplitterWnd *m_wndSplitter;
	CCreateContext  *m_pContext;
	CTaiTestSplitter   *m_SplitterWnd;
	CTaiShanDoc* m_CurrentDoc;
// Attributes
public:
    int ss;
// Operations
public:
	CCustomMenu m_hMenu;
	int m_iMenuItemWidth;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CChildFrame)
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CChildFrame();
    virtual void OnUpdateFrameTitle(BOOL nAddToTitle);
	void AddCuoHeView();
    void DeleteCuoHeView();

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

// Generated message map functions
protected:
	//{{AFX_MSG(CChildFrame)
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnClose();
	afx_msg void OnMDIActivate(BOOL bActivate, CWnd* pActivateWnd, CWnd* pDeactivateWnd);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	//}}AFX_MSG
	afx_msg LRESULT OnSetSplitterPos(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()
public:
	virtual void ActivateFrame(int nCmdShow = -1);
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
public:
	virtual BOOL LoadFrame(UINT nIDResource, DWORD dwDefaultStyle = WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE, CWnd* pParentWnd = NULL, CCreateContext* pContext = NULL);
public:
	afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);
public:
	afx_msg void OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct);
public:
	afx_msg void OnInitMenu(CMenu* pMenu);
public:
	afx_msg void OnInitMenuPopup(CMenu* pPopupMenu, UINT nIndex, BOOL bSysMenu);
	void CChildFrame::GetMenuCount();
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CHILDFRM_H__61A31CEF_9707_11D1_ACAB_0000E823DBFD__INCLUDED_)
