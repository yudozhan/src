// StatisticSettingDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "WH.h"
#include "StatisticSettingDlg.h"


// CStatisticSettingDlg �Ի���

IMPLEMENT_DYNAMIC(CStatisticSettingDlg, CDialog)

CStatisticSettingDlg::CStatisticSettingDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CStatisticSettingDlg::IDD, pParent)
{

}

CStatisticSettingDlg::~CStatisticSettingDlg()
{
}

void CStatisticSettingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CStatisticSettingDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CStatisticSettingDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_WEIGHT_SUANSHU, &CStatisticSettingDlg::OnBnClickedWeightSuanshu)
	ON_BN_CLICKED(IDC_WEIGHT_LTGB, &CStatisticSettingDlg::OnBnClickedWeightLtgb)
	ON_BN_CLICKED(IDC_WEIGHT_ZGB, &CStatisticSettingDlg::OnBnClickedWeightZgb)
END_MESSAGE_MAP()


// CStatisticSettingDlg ��Ϣ�������

void CStatisticSettingDlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	OnOK();
}

void CStatisticSettingDlg::OnBnClickedWeightSuanshu()
{
	m_nType = WEIGHT_SUANSHU;
}

void CStatisticSettingDlg::OnBnClickedWeightLtgb()
{
	m_nType = WEIGHT_LTGB;
}

void CStatisticSettingDlg::OnBnClickedWeightZgb()
{
	m_nType = WEIGHT_ZGB;
}

BOOL CStatisticSettingDlg::OnInitDialog()
{
	CDialog::OnInitDialog();


	CButton* pBtn = (CButton*)GetDlgItem(IDC_WEIGHT_SUANSHU);
	pBtn->SetCheck(m_nType == WEIGHT_SUANSHU ? BST_CHECKED : BST_UNCHECKED);
	
	pBtn = (CButton*)GetDlgItem(IDC_WEIGHT_LTGB);
	pBtn->SetCheck(m_nType == WEIGHT_LTGB ? BST_CHECKED : BST_UNCHECKED);
	
	pBtn = (CButton*)GetDlgItem(IDC_WEIGHT_ZGB);
	pBtn->SetCheck(m_nType == WEIGHT_ZGB ? BST_CHECKED : BST_UNCHECKED);

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void CStatisticSettingDlg::SetParam(int nType)
{
	m_nType = nType;
}

void CStatisticSettingDlg::GetParam(int& nType)
{
	nType = m_nType;
}
