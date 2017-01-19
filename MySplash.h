#if !defined(AFX_MYSPLASH_H__1B252F44_4941_11D3_88F2_0080C8E20736__INCLUDED_)
#define AFX_MYSPLASH_H__1B252F44_4941_11D3_88F2_0080C8E20736__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MySplash.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CTaiTestSplash window

class CTaiTestSplash : public CWnd
{
// Construction
public:
	CTaiTestSplash();
	void HideWindow(void);

// Attributes
public:
    CBitmap m_bitmap;
	HBITMAP  hBitmap;
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTaiTestSplash)
	protected:
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CTaiTestSplash();
//protected:
	BOOL Create(CWnd* pParentWnd = NULL);
	// Generated message map functions
protected:
	//{{AFX_MSG(CTaiTestSplash)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaint();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYSPLASH_H__1B252F44_4941_11D3_88F2_0080C8E20736__INCLUDED_)
