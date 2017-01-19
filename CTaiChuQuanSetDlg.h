#if !defined(AFX_ADDCHUQUAN_H__F2A78EE4_D1DD_11D1_88E5_0080C8E20736__INCLUDED_)
#define AFX_ADDCHUQUAN_H__F2A78EE4_D1DD_11D1_88E5_0080C8E20736__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CTaiChuQuanSetDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CTaiChuQuanSetDlg dialog
class CTaiShanKlineShowView;

class CTaiChuQuanSetDlg : public CDialog
{
// Construction
public:
	CTaiShanKlineShowView* pView ;

	void OnDTNotify(NMHDR *pHdr, LRESULT *pRes);
	int m_fromWhich;
	char m_OperateID;
	CTaiChuQuanSetDlg(CWnd* pParent = NULL);   // standard constructor
	time_t m_timet;
	CDateTimeCtrl m_DateTime;

// Dialog Data
	//{{AFX_DATA(CTaiChuQuanSetDlg)
	enum { IDD = IDD_DIALOGCHUQUAN };
	CButton	m_ok;
	CButton	m_cancel;
	CString	m_CqDate;
	float	m_fGive;
	float	m_fDivid;
	float	m_fPrice;
	float	m_fAlloc;
	int		m_kind;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTaiChuQuanSetDlg)
	protected:
	virtual void OnOK();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	void ValidateCheck(UINT nID);

	// Generated message map functions
	//{{AFX_MSG(CTaiChuQuanSetDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnChangeEditCqgive();
	afx_msg void OnChangeEditCqalloc();
	afx_msg void OnChangeEditCqdivid();
	afx_msg void OnChangeEditCqprice();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ADDCHUQUAN_H__F2A78EE4_D1DD_11D1_88E5_0080C8E20736__INCLUDED_)
