#pragma once
#include "afxwin.h"


// CBlockTypeNameDlg 对话框

class CBlockTypeNameDlg : public CDialog
{
	DECLARE_DYNAMIC(CBlockTypeNameDlg)

public:
	CBlockTypeNameDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CBlockTypeNameDlg();

// 对话框数据
	enum { IDD = IDD_BLOCKTYPENAME };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
public:
	virtual BOOL OnInitDialog();
public:
	CEdit mCtrlBlockTypeName;
	CString mStrName;
	void GetString(CString& strName);
public:
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
};
