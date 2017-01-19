#if !defined(AFX_MYTREEVIEW_H__072EDC24_E8F8_11D1_88EB_0080C8E20736__INCLUDED_)
#define AFX_MYTREEVIEW_H__072EDC24_E8F8_11D1_88EB_0080C8E20736__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MyTreeView.h : header file
//
#define MSG_MYTREEVIEW		WM_USER+30	//���͵�F10��TREEVIEW����
#include "Afxcview.h"
/////////////////////////////////////////////////////////////////////////////
// CTaiTestTreeView view
class CTaiShanDoc;
class CTaiTestRichView;

class CTaiTestTreeView : public CTreeView
{
protected:        // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CTaiTestTreeView)

// Attributes
public:

// Operations
public:
	static void ClearAll();
	void ShowFileText(CString filename,CTaiShanDoc* pDoc,CTaiTestRichView* pView);
	CString GetFileFullName(CString sDirect,CString sName);
	static void GetDirectorys(CString sParen,CStringArray& sArrOut);
	CStringArray m_sArrayItemName;
	CStringArray m_sArrayFilePath;
	CTaiTestTreeView();   
	virtual ~CTaiTestTreeView();
	void ShowAll(void);      //////for F9////////
	void ShowAllF10(void);   //////for F10///////
	bool ChangeView;     //////if true,change View///////////
	bool m_bF9ORF10;      /////////////true is F9 else F10///////////
	void CheckFileTime(int iDays);
    static void CheckNewsInfomationFileTime();
    void FindStockBaseInfoPos(char *stockcode);
	afx_msg void OnGetUpdate();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTaiTestTreeView)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
protected:

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	afx_msg void OnContextMenu(CWnd*, CPoint point);
	//{{AFX_MSG(CTaiTestTreeView)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDeleteFile();
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnKeydown(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblclk(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnUpdateNewF10(CCmdUI* pCmdUI);
	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
	afx_msg void OnSelchanged(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg LRESULT OnMsgMyTreeView(UINT wParam,LONG lParam);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	void ShowFileTextNow();
	//added by qyp 2004.10.25
#ifdef WIDE_NET_VERSION
	SOCKET_ID socket_id;
	CView* m_pView;
	CString m_filename;
	BOOL m_bRequestF10;
	BOOL m_bRequestF9;
#endif
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYTREEVIEW_H__072EDC24_E8F8_11D1_88EB_0080C8E20736__INCLUDED_)
