#pragma once


// CStatisticSettingDlg 对话框

class CStatisticSettingDlg : public CDialog
{
	DECLARE_DYNAMIC(CStatisticSettingDlg)

public:
	CStatisticSettingDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CStatisticSettingDlg();

	int m_nType;
// 对话框数据
	enum { IDD = IDD_STATISTIC };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

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
