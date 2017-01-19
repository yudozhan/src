#if !defined(AFX_LMBDLGHISTORYSELECT_H__A3A59949_FF03_11D3_96E9_0080C8E1242B__INCLUDED_)
#define AFX_LMBDLGHISTORYSELECT_H__A3A59949_FF03_11D3_96E9_0080C8E1242B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// LmbDlgHistorySelect.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// LmbDlgHistorySelect dialog
class LmbFileHS;
class LmbHistoryFileSelect : public CObject
{
public:
	int FilterFileName(CTime& begin,CTime& end);//get the needed file name
	bool OpenAll();//open all the needed file
	LmbHistoryFileSelect();

	CStringArray m_fileNameArray;

	CTypedPtrMap<CMapStringToPtr,CString,LmbFileHS*> m_fileHsShArray;
	CTypedPtrMap<CMapStringToPtr,CString,LmbFileHS*> m_fileHsSzArray;

	virtual ~LmbHistoryFileSelect();
private:
	void CloseAll();
}
;

class LmbDlgHistorySelect : public CDialog
{
// Construction
public:
	static void GetFileNameArray(CStringArray& sArry);
	CString m_fileName;
	LmbDlgHistorySelect(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(LmbDlgHistorySelect)
	enum { IDD = IDD_HISTORY_SELECT1 };
	CListCtrl	m_fileNameList;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(LmbDlgHistorySelect)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(LmbDlgHistorySelect)
	virtual BOOL OnInitDialog();
	afx_msg void OnOk();
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LMBDLGHISTORYSELECT_H__A3A59949_FF03_11D3_96E9_0080C8E1242B__INCLUDED_)
