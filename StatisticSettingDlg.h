#pragma once


// CStatisticSettingDlg �Ի���

class CStatisticSettingDlg : public CDialog
{
	DECLARE_DYNAMIC(CStatisticSettingDlg)

public:
	CStatisticSettingDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CStatisticSettingDlg();

	int m_nType;
// �Ի�������
	enum { IDD = IDD_STATISTIC };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
public:
	afx_msg void OnBnClickedWeightSuanshu();
public:
	afx_msg void OnBnClickedWeightLtgb();
public:
	afx_msg void OnBnClickedWeightZgb();
public:
	virtual BOOL OnInitDialog();

	void	SetParam(int nType);
	void	GetParam(int& nType);
};
