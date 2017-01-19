// WHDoc.h : interface of the CWHDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_WHDOC_H__3557F0C6_F66B_42EB_B1CE_D3291E4C1D4F__INCLUDED_)
#define AFX_WHDOC_H__3557F0C6_F66B_42EB_B1CE_D3291E4C1D4F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CWHDoc : public CDocument
{
protected: // create from serialization only
	CWHDoc();
	DECLARE_DYNCREATE(CWHDoc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWHDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CWHDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CWHDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WHDOC_H__3557F0C6_F66B_42EB_B1CE_D3291E4C1D4F__INCLUDED_)
