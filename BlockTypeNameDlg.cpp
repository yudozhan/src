// BlockTypeNameDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "WH.h"
#include "BlockTypeNameDlg.h"


// CBlockTypeNameDlg �Ի���

IMPLEMENT_DYNAMIC(CBlockTypeNameDlg, CDialog)

CBlockTypeNameDlg::CBlockTypeNameDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CBlockTypeNameDlg::IDD, pParent)
{

}

CBlockTypeNameDlg::~CBlockTypeNameDlg()
{
}

void CBlockTypeNameDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, mCtrlBlockTypeName);
}


BEGIN_MESSAGE_MAP(CBlockTypeNameDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CBlockTypeNameDlg::OnBnClickedOk)
	ON_WM_SHOWWINDOW()
END_MESSAGE_MAP()


// CBlockTypeNameDlg ��Ϣ�������

void CBlockTypeNameDlg::OnBnClickedOk()
{
	mCtrlBlockTypeName.GetWindowText(mStrName);
	if (mStrName.GetLength() > 10)
	{
		AfxMessageBox("����̫����");
		mCtrlBlockTypeName.SetFocus();
		mCtrlBlockTypeName.SetSel(0,-1);
		
		return;
	}
	OnOK();
}

BOOL CBlockTypeNameDlg::OnInitDialog()
{
	CDialog::OnInitDialog();


	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}
void CBlockTypeNameDlg::GetString(CString& strName)
{
	strName = mStrName;
}

void CBlockTypeNameDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);
	mCtrlBlockTypeName.SetFocus();
	mCtrlBlockTypeName.SetWindowText("�·���");
	mCtrlBlockTypeName.SetSel(0,-1);

	// TODO: �ڴ˴������Ϣ����������
}
