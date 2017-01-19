#if !defined(AFX_STOCKDETAILDAYS_H__C10A0911_5CD9_440F_BC24_DD6CC9C75139__INCLUDED_)
#define AFX_STOCKDETAILDAYS_H__C10A0911_5CD9_440F_BC24_DD6CC9C75139__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CSharesDetailDays.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSharesDetailDays dialog

class CSharesDetailDays : public CDialog
{
// Construction
private:
	int m_nLower;
	int m_nUpper;
	BOOL m_bIncludeToday;
public:
	CSharesDetailDays(CWnd* pParent = NULL);   // standard constructor
	

// Dialog Data
	//{{AFX_DATA(CSharesDetailDays)
	enum { IDD = IDD_DIALOG11 };
	CSpinButtonCtrl	m_spinDay;
	UINT	m_nDays;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSharesDetailDays)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
public:
	void SetRange(int nLower=1,int nUpper=3);
	void SetDays(int nDays);
	void SetToday(BOOL bInclude=FALSE);
	int GetDays();
	BOOL GetToday();
protected:

	// Generated message map functions
	//{{AFX_MSG(CSharesDetailDays)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STOCKDETAILDAYS_H__C10A0911_5CD9_440F_BC24_DD6CC9C75139__INCLUDED_)
