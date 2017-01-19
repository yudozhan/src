//{{AFX_INCLUDES()
#include "msflexgrid.h"
//}}AFX_INCLUDES
#if !defined(AFX_TECHEXPORTEXECEL_H__C54E43A2_67B1_11D4_970B_0080C8D6450E__INCLUDED_)
#define AFX_TECHEXPORTEXECEL_H__C54E43A2_67B1_11D4_970B_0080C8D6450E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CTaiKlineExportExecel.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CTaiKlineExportExecel dialog
class CTaiShanKlineShowView;

class CTaiKlineExportExecel : public CDialog
{
// Construction
public:
	CTaiKlineExportExecel(CWnd* pParent = NULL);   // standard constructor
	CTaiShanKlineShowView* pView;
	int m_nFiguer;
// Dialog Data
	//{{AFX_DATA(CTaiKlineExportExecel)
	enum { IDD = IDD_OUT_DATA };
	CButton	m_ok;
	CButton	m_cancel;
	BOOL	m_bShowExcel;
	CMSFlexGrid	m_gridDataOut;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTaiKlineExportExecel)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CTaiKlineExportExecel)
	virtual BOOL OnInitDialog();
	afx_msg void OnExport();//�����ݸ��Ƶ�excel�ļ���
	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TECHEXPORTEXECEL_H__C54E43A2_67B1_11D4_970B_0080C8D6450E__INCLUDED_)
