// FyRegister.cpp : implementation file
//

#include "stdafx.h"
#include "WH.h"
#include "FyRegister.h"
#include "FyAddMoney.h"
#include "GetSetReg.h"
#include "MainFrm.h"
#include "CTaiShanDoc.h"
#include "UserRegPart2.h"
#include "secretdrvhyt.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// FyRegister dialog


FyRegister::FyRegister(CWnd* pParent /*=NULL*/)
	: CDialog(FyRegister::IDD, pParent)
{
	//{{AFX_DATA_INIT(FyRegister)
	m_sMachineID = _T("");
	m_sEmail = _T("");
	m_sID = _T("");
	m_sPhone = _T("");
	m_sAddress = _T("");
	m_sPower = _T("");
	//}}AFX_DATA_INIT

	m_sMachineID = g_chMachineID;
}


void FyRegister::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(FyRegister)
	DDX_Text(pDX, IDC_EDIT_DISKID, m_sMachineID);
	DDV_MaxChars(pDX, m_sMachineID, 32);
	DDX_Text(pDX, IDC_EDIT10, m_sEmail);
	DDX_Text(pDX, IDC_EDIT7, m_sID);
	DDV_MaxChars(pDX, m_sID, 16);
	DDX_Text(pDX, IDC_EDIT8, m_sPhone);
	DDX_Text(pDX, IDC_EDIT9, m_sAddress);
	DDX_Text(pDX, IDC_EDIT_POWER, m_sPower);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(FyRegister, CDialog)
	//{{AFX_MSG_MAP(FyRegister)
	ON_BN_CLICKED(IDC_BUTTON1, OnButtonHand)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// FyRegister message handlers

BOOL FyRegister::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	FyAddMoney::ReadSettings(m_sPower, m_sID);
	UpdateData(FALSE);
	
		
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void FyRegister::OnOK() 
{
	if(!UpdateData()) return;
	if(m_sMachineID == ""
		||m_sID == ""
		||m_sPhone == ""
		||m_sAddress == ""
		||m_sEmail == ""
		)
	{
		AfxMessageBox("信息不完整，请重新填写！");
		return;
	}
	CString s = FyAddMoney::GetUrl();
	s.TrimRight ("\\");
	s.TrimRight ("/");
	s+="/ServerLogOnManage/RegisteResult.asp";

	CSecretDrv2 secret;
	bool bSuccess;
	CString strTemp = secret.DesecretSerialID(m_sID, bSuccess);

	CString url = s;
	url+="?suser=";
	url+=strTemp;
	url+="&cardid=";

//	url+=this->m_sID ;
	url+=strTemp ;
//	url+=strTemp ;
	CString sMachine = g_chMachineID;
	url+="&spassword=";
	url+=sMachine;

	url+="&phone=";
	url+=m_sPhone;

	url+="&address=";
	url+=m_sAddress;

	url+="&zone=";
	url+=m_sEmail;

	int nErrorCode;
	s = FyAddMoney::GetReturnCode(url,true, nErrorCode);

	if(nErrorCode == 0)
	{
		CString strCreditNew;
		CSecretDrv2 secret;
		if (!secret.CreateImpowerID(strTemp,m_sMachineID,strCreditNew))
			return ;
		strCreditNew.MakeUpper();

		//to save settings
		FyAddMoney::SaveSettings(strCreditNew, m_sID);
		//end
		CString ss ;
		ss.Format ("注册成功！您的授权号是 %s ，请记住。",strCreditNew);
		AfxMessageBox(ss);
		CDialog::OnOK();
	}
}

void FyRegister::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CDialog::OnCancel();
}

void FyRegister::OnButtonHand() 
{
	if(!UpdateData()) return;

	BOOL b = IsCreditUserPart2(this->m_sID ,this->m_sPower ,this->m_sMachineID );
	if(!b)
	{
		AfxMessageBox("错误的授权号或产品序列号，请重新输入！");
	}
	else
	{
		//to save settings
		FyAddMoney::SaveSettings(m_sPower, m_sID);
		//end
		AfxMessageBox("注册成功！");
		CDialog::OnOK();
	}
}

bool FyRegister::IsValidUser()
{
	CString strSerial;
	CString strCredit;
	CString strMachine = g_chMachineID;
	FyAddMoney::ReadSettings(strCredit, strSerial);
//	strSerial = "338b7f593d961882";
	BOOL b = IsCreditUserPart2(strSerial,strCredit,strMachine);
	if(!b)
	{
		FyRegister dlg;
		if(dlg.DoModal () == IDOK)
			b = true;
	}//
	return b;
}
