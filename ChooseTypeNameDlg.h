#pragma once
#include "afxwin.h"


// CChooseTypeNameDlg 对话框

class CChooseTypeNameDlg : public CDialog
{
	DECLARE_DYNAMIC(CChooseTypeNameDlg)

public:
	CChooseTypeNameDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CChooseTypeNameDlg();

// 对话框数据
	enum { IDD = IDD_CHOOSETYPENAME };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
public:
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	void GetString(CString& strName);

public:
	CEdit mCtrlChooseTypeName;
	CString mStrName;

};
