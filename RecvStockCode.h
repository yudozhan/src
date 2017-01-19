#if !defined(AFX_RECVSTOCKCODE_H__5E075EF7_14F3_4740_AE48_4242193C94E3__INCLUDED_)
#define AFX_RECVSTOCKCODE_H__5E075EF7_14F3_4740_AE48_4242193C94E3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RecvStockCode.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CRecvStockCode dialog

class CRecvStockCode : public CDialog
{
// Construction
public:
	CRecvStockCode(CWnd* pParent = NULL);   // standard constructor
    CMainFrame *m_pMainFrm;
	TRANS_PACKAGE_STRUCTEx m_TransPackageStruct;
	SOCKET_ID m_SocketID;

// Dialog Data
	//{{AFX_DATA(CRecvStockCode)
	enum { IDD = IDD_DIALOG1 };
	CProgressCtrl	m_MyProgressCtrl;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRecvStockCode)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CRecvStockCode)
	virtual BOOL OnInitDialog();
	afx_msg void OnClose();
	virtual void OnCancel();
	//}}AFX_MSG
	afx_msg LRESULT OnRecvStockCode(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RECVSTOCKCODE_H__5E075EF7_14F3_4740_AE48_4242193C94E3__INCLUDED_)
