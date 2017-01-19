#if !defined(AFX_SHEETSCROLLBAR_H__5690D562_FA91_11D2_88F0_0000E8A1A5C0__INCLUDED_)
#define AFX_SHEETSCROLLBAR_H__5690D562_FA91_11D2_88F0_0000E8A1A5C0__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// SheetScrollBar.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSheetScrollBar window

class CSheetScrollBar : public CScrollBar
{
// Construction
public:
	CSheetScrollBar();
        
  // Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSheetScrollBar)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CSheetScrollBar();
	void OnDrawScrollBarTest(CRect * m_Rect);
	BOOL m_bRefresh;


	// Generated message map functions
protected:
	//{{AFX_MSG(CSheetScrollBar)
	afx_msg void OnPaint();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SHEETSCROLLBAR_H__5690D562_FA91_11D2_88F0_0000E8A1A5C0__INCLUDED_)
