// ChooseTypeNameDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "WH.h"
#include "ChooseTypeNameDlg.h"


// CChooseTypeNameDlg 对话框

IMPLEMENT_DYNAMIC(CChooseTypeNameDlg, CDialog)

CChooseTypeNameDlg::CChooseTypeNameDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CChooseTypeNameDlg::IDD, pParent)
{

}

CChooseTypeNameDlg::~CChooseTypeNameDlg()
{
}

void CChooseTypeNameDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, mCtrlChooseTypeName);
}


BEGIN_MESSAGE_MAP(CChooseTypeNameDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CChooseTypeNameDlg::OnBnClickedOk)
	ON_WM_SHOWWINDOW()
END_MESSAGE_MAP()


// CChooseTypeNameDlg 消息处理程序

void CChooseTypeNameDlg::OnBnClickedOk()
{
	mCtrlChooseTypeName.GetWindowText(mStrName);
	if (mStrName.GetLength() > 10)
	{
		AfxMessageBox("名称太长！");
		mCtrlChooseTypeName.SetFocus();
		mCtrlChooseTypeName.SetSel(0,-1);

		return;
	}	OnOK();
}

void CChooseTypeNameDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);

	mCtrlChooseTypeName.SetFocus();
	mCtrlChooseTypeName.SetWindowText("自选1");
	mCtrlChooseTypeName.SetSel(0,-1);
}
void CChooseTypeNameDlg::GetString(CString& strName)
{
	strName = mStrName;
}
