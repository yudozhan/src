#if !defined(AFX_SHEETWG_H__72D05F61_BA0F_11D2_9086_0000E8593F1A__INCLUDED_)
#define AFX_SHEETWG_H__72D05F61_BA0F_11D2_9086_0000E8593F1A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SHEETWG.h : header file
//
#include "resource.h"
#include "CTaiShanDoc.h"

/////////////////////////////////////////////////////////////////////////////
// CTaiSysSetInterfacePage dialog

class CTaiSysSetInterfacePage : public CPropertyPage
{
	DECLARE_DYNCREATE(CTaiSysSetInterfacePage)
// Construction
public:
	CTaiSysSetInterfacePage();
	~CTaiSysSetInterfacePage();

	RECT     m_rectsample;                    //图形预览的区域
	LOGFONT  m_logfont,fontdefault;           //缺省的字体
	COLORREF cr[systemcolorlength];
	LOGFONT  fr[FONTSTRUCTLEN];

	int     iLastComItem;
	COLORREF crbak[systemcolorlength];
	LOGFONT  frbak[FONTSTRUCTLEN];
// Dialog Data
	//{{AFX_DATA(CTaiSysSetInterfacePage)
	enum { IDD = IDD_SHEETWG };
	CButton	m_setfont;
	CComboBox	m_sheetwgcom1;
	CListBox	m_sheetwglist1;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CTaiSysSetInterfacePage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CTaiSysSetInterfacePage)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeList1();
	afx_msg void OnSetcolor();
	afx_msg void OnSetfont();
	afx_msg void OnPaint();
	afx_msg void OnSelchangeCombo1();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SHEETWG_H__72D05F61_BA0F_11D2_9086_0000E8593F1A__INCLUDED_)
