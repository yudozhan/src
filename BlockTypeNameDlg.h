#pragma once
#include "afxwin.h"


// CBlockTypeNameDlg �Ի���

class CBlockTypeNameDlg : public CDialog
{
	DECLARE_DYNAMIC(CBlockTypeNameDlg)

public:
	CBlockTypeNameDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CBlockTypeNameDlg();

// �Ի�������
	enum { IDD = IDD_BLOCKTYPENAME };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

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
