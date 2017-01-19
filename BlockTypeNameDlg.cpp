// BlockTypeNameDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "WH.h"
#include "BlockTypeNameDlg.h"


// CBlockTypeNameDlg 对话框

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


// CBlockTypeNameDlg 消息处理程序

void CBlockTypeNameDlg::OnBnClickedOk()
{
	mCtrlBlockTypeName.GetWindowText(mStrName);
	if (mStrName.GetLength() > 10)
	{
		AfxMessageBox("名称太长！");
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
	// 异常: OCX 属性页应返回 FALSE
}
void CBlockTypeNameDlg::GetString(CString& strName)
{
	strName = mStrName;
}

void CBlockTypeNameDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);
	mCtrlBlockTypeName.SetFocus();
	mCtrlBlockTypeName.SetWindowText("新分类");
	mCtrlBlockTypeName.SetSel(0,-1);

	// TODO: 在此处添加消息处理程序代码
}
