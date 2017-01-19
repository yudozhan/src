#if !defined(AFX_SJLCHUQUANINDLG_H__5A9C8081_8D90_11D4_9426_0080C8E20736__INCLUDED_)
#define AFX_SJLCHUQUANINDLG_H__5A9C8081_8D90_11D4_9426_0080C8E20736__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CTaiChuQuanInDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CTaiChuQuanInDlg dialog
	//分析家导出的除权数据结构
	typedef struct tagFXJPOWER
	{
		int   nTime;
		float fGive;
		float fAlloc;
		float fAllocPrice;
		float fDividend;
	}FXJPOWER;

class CTaiChuQuanInDlg : public CDialog
{
// Construction
public:
	static BOOL InstallPowerData(CString FilePath);
//	CString m_FormName;
	CTaiChuQuanInDlg(CWnd* pParent = NULL);   // standard constructor
    CTaiShanDoc* pDoc;
// Dialog Data
	//{{AFX_DATA(CTaiChuQuanInDlg)
	enum { IDD = IDD_CHUQUANIMPORT };
	CButton	m_cancel;
	CButton	m_read;
	CButton	m_import;
	CComboBox	m_PowerForm;
	CString	m_InFile;
	CString	m_FormName;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTaiChuQuanInDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CTaiChuQuanInDlg)
	afx_msg void OnImport();
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeGeshi();
	afx_msg void OnRead();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SJLCHUQUANINDLG_H__5A9C8081_8D90_11D4_9426_0080C8E20736__INCLUDED_)
