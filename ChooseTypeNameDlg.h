#pragma once
#include "afxwin.h"


// CChooseTypeNameDlg �Ի���

class CChooseTypeNameDlg : public CDialog
{
	DECLARE_DYNAMIC(CChooseTypeNameDlg)

public:
	CChooseTypeNameDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CChooseTypeNameDlg();

// �Ի�������
	enum { IDD = IDD_CHOOSETYPENAME };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

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
