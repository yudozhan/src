#if !defined(AFX_GUIZHOUTVVIEW_H__5F4F0AC5_4F2F_4393_BBD5_173D2CB37139__INCLUDED_)
#define AFX_GUIZHOUTVVIEW_H__5F4F0AC5_4F2F_4393_BBD5_173D2CB37139__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// GuiZhouTvView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CGuiZhouTvView view

class CGuiZhouTvView : public CEditView
{
protected:
	CGuiZhouTvView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CGuiZhouTvView)

// Attributes
public:

// Operations
public:
	BOOL ShowInfo(CString strFile);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGuiZhouTvView)
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CGuiZhouTvView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(CGuiZhouTvView)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GUIZHOUTVVIEW_H__5F4F0AC5_4F2F_4393_BBD5_173D2CB37139__INCLUDED_)
