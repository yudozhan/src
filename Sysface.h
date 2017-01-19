#if !defined(AFX_SYSFACE_H__3603CBA1_A124_11D4_9426_0080C8E20736__INCLUDED_)
#define AFX_SYSFACE_H__3603CBA1_A124_11D4_9426_0080C8E20736__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Sysface.h : header file
//
#include "CTaiShanDoc.h"
/////////////////////////////////////////////////////////////////////////////
// CSYSFACE dialog

class CSYSFACE : public CPropertyPage
{
	DECLARE_DYNCREATE(CSYSFACE)

// Construction
public:
	CSYSFACE();
	~CSYSFACE();
	RECT     m_rectsample;                    //图形预览的区域
	LOGFONT  m_logfont,fontdefault;           //缺省的字体
	COLORREF cr[systemcolorlength];
	LOGFONT  fr[FONTSTRUCTLEN];

	int     iLastComItem;
	COLORREF crbak[systemcolorlength];
	LOGFONT  frbak[FONTSTRUCTLEN];
    CTaiShanDoc *pDoc;


// Dialog Data
	//{{AFX_DATA(CSYSFACE)
	enum { IDD = IDD_WG2_SYSTEMINFO };
	CButton	m_setcolor;
	CButton	m_setfont;
	CComboBox	m_sheetwgcom1;
	CListBox	m_sheetwglist1;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CSYSFACE)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CSYSFACE)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnSelchangeCombo1();
	afx_msg void OnSetfont();
	afx_msg void OnSetcolor();
	afx_msg void OnSelchangeList1();
	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
	//}}AFX_MSG
	afx_msg LRESULT OnSysfaceItemChange(WPARAM,LPARAM);
	DECLARE_MESSAGE_MAP()

};

typedef struct tagSysFace
{
	HWND hWnd;
	int  nItem;
}SYSFACESTRUCT,FAR * PSYSFACESTRUCT;

UINT SysfacePreviewThread(LPVOID);
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SYSFACE_H__3603CBA1_A124_11D4_9426_0080C8E20736__INCLUDED_)
