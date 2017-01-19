#if !defined(AFX_LMBLISTCTRL_H__096D7941_8DDB_11D3_9203_0080C8E1242B__INCLUDED_)
#define AFX_LMBLISTCTRL_H__096D7941_8DDB_11D3_9203_0080C8E1242B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CTaiKlineListCtrl.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CTaiKlineListCtrl window

class CTaiKlineListCtrl : public CListCtrl
{
// Construction
public:
	CTaiKlineListCtrl();

// Attributes
public:
	CString			m_strResult;
	int				m_stkKind;
	CWnd*			m_wndTarget;

	BOOL            m_bDragging;
	int             m_iItemDrag;
	CPoint          m_ptHotSpot;
	CPoint          m_ptOrigin;
	CSize			m_sizeDelta;
	CImageList      *m_pimageListDrag;

	WPARAM			m_wParam;
	LPARAM			m_lParam;

	void    OnButtonUp(CPoint point);
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTaiKlineListCtrl)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CTaiKlineListCtrl();

	// Generated message map functions
protected:
	//{{AFX_MSG(CTaiKlineListCtrl)
		// NOTE - the ClassWizard will add and remove member functions here.
	afx_msg void OnEndLabelEdit(LPNMHDR pnmhdr, LRESULT *pLResult);
	afx_msg void OnBeginDrag(LPNMHDR pnmhdr, LRESULT *pLResult);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LMBLISTCTRL_H__096D7941_8DDB_11D3_9203_0080C8E1242B__INCLUDED_)
