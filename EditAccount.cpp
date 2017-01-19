// EditAccount.cpp : implementation file
//

#include "stdafx.h"
#include "WH.h"
#include "EditAccount.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEditAccount dialog


CEditAccount::CEditAccount(CWnd* pParent /*=NULL*/)
	: CDialog(CEditAccount::IDD, pParent)
{
	//{{AFX_DATA_INIT(CEditAccount)
	m_limitName = _T("");
	m_limitOld = _T("");
	m_limitPassword = _T("");
	m_limitVerify = _T("");
	//}}AFX_DATA_INIT
}


void CEditAccount::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEditAccount)
	DDX_Control(pDX, IDOK, m_ok);
	DDX_Control(pDX, IDCANCEL, m_cancel);
	DDX_Text(pDX, IDC_NAME, m_limitName);
	DDV_MaxChars(pDX, m_limitName, 15);
	DDX_Text(pDX, IDC_OLDPASSWORD, m_limitOld);
	DDV_MaxChars(pDX, m_limitOld, 15);
	DDX_Text(pDX, IDC_PASSWORD, m_limitPassword);
	DDV_MaxChars(pDX, m_limitPassword, 15);
	DDX_Text(pDX, IDC_VERIFY, m_limitVerify);
	DDV_MaxChars(pDX, m_limitVerify, 15);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CEditAccount, CDialog)
	//{{AFX_MSG_MAP(CEditAccount)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEditAccount message handlers

BOOL CEditAccount::OnInitDialog() 
{
	CDialog::OnInitDialog();



/*	POINT pt;
	::GetCursorPos(&pt);
	CRect rect;
	GetWindowRect(rect);
	SetWindowPos(NULL,pt.x, pt.y-rect.Height(), rect.Width(),rect.Height(),SWP_SHOWWINDOW);
*/
	
	// TODO: Add extra initialization here
	switch(m_Operate)
	{
	case 0:
		SetWindowText("�����ʻ�");
		GetDlgItem(IDC_STATIC_OLD)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_OLDPASSWORD)->ShowWindow(SW_HIDE);
		((CEdit*)GetDlgItem(IDC_NAME))->EnableWindow(1);

		((CEdit*)GetDlgItem(IDC_NAME))->SetWindowText("");
		((CEdit*)GetDlgItem(IDC_PASSWORD))->SetWindowText("");
		((CEdit*)GetDlgItem(IDC_VERIFY))->SetWindowText("");
		break;
	case 1:
		SetWindowText("�޸�����");
		GetDlgItem(IDC_STATIC_OLD)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_OLDPASSWORD)->ShowWindow(SW_SHOW);
		((CEdit*)GetDlgItem(IDC_NAME))->EnableWindow(0);

		((CEdit*)GetDlgItem(IDC_NAME))->SetWindowText(m_szName);
		((CEdit*)GetDlgItem(IDC_PASSWORD))->SetWindowText(m_szPassword);
		((CEdit*)GetDlgItem(IDC_VERIFY))->SetWindowText(m_szVerify);
		break;
	case 2:		//when select stock 2001-05-22
		SetWindowText("�ʻ���Ϣ");
		GetDlgItem(IDC_STATIC_OLD)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_OLDPASSWORD)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_QUEREN)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_VERIFY)->ShowWindow(SW_HIDE);
		((CEdit*)GetDlgItem(IDC_NAME))->EnableWindow(1);
	default:
		SetWindowText("�ʻ���Ϣ");
		GetDlgItem(IDC_STATIC_OLD)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_OLDPASSWORD)->ShowWindow(SW_HIDE);
		((CEdit*)GetDlgItem(IDC_NAME))->EnableWindow(1);
		//GetDlgItem(IDC_QUEREN)->ShowWindow(SW_HIDE);
		//((CEdit*)GetDlgItem(IDC_VERIFY))->ShowWindow(SW_HIDE);



		((CEdit*)GetDlgItem(IDC_NAME))->SetWindowText("");
		((CEdit*)GetDlgItem(IDC_PASSWORD))->SetWindowText("");
		((CEdit*)GetDlgItem(IDC_VERIFY))->SetWindowText("");
		
	}
	//UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CEditAccount::OnOK() 
{
	((CEdit*)GetDlgItem(IDC_NAME))->GetWindowText(m_szName);
	((CEdit*)GetDlgItem(IDC_PASSWORD))->GetWindowText(m_szPassword);
	((CEdit*)GetDlgItem(IDC_VERIFY))->GetWindowText(m_szVerify);
//	if( (m_szPassword==""||m_szPassword!=m_szVerify || m_szName=="")&& m_Operate!=2     )
	if( (m_szPassword!=m_szVerify || m_szName=="")&& m_Operate!=2     )
	{
		AfxMessageBox("����������Ч���ʻ�����Ϊ��.",MB_ICONSTOP);
		return;
	}

	if( this->m_Operate==1)//when edit.
	{
		CString str;
		((CEdit*)GetDlgItem(IDC_OLDPASSWORD))->GetWindowText(str);
		if( m_szOldPassword!=str)
		{
			AfxMessageBox("ֻ����ȷ����ԭ�������룬����Ȩ�޽����޸�.");
			return;
		}
	}

	if( this->m_Operate==2)//when select stock 2001-05-22
	{
		if( m_szName=="")
		{
			AfxMessageBox("�ʻ�������Ϊ��.");
			return;
		}
	}


  	CDialog::OnOK();
}
