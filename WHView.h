// WHView.h : interface of the CWHView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_WHVIEW_H__8A83DB08_18C5_4177_9DB2_9EAB7BC4655B__INCLUDED_)
#define AFX_WHVIEW_H__8A83DB08_18C5_4177_9DB2_9EAB7BC4655B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CWHView : public CView
{
protected: // create from serialization only
	CWHView();
	DECLARE_DYNCREATE(CWHView)

// Attributes
public:
	CWHDoc* GetDocument();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWHView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CWHView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CWHView)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in WHView.cpp
inline CWHDoc* CWHView::GetDocument()
   { return (CWHDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WHVIEW_H__8A83DB08_18C5_4177_9DB2_9EAB7BC4655B__INCLUDED_)
