// CTaiKlineSetNameTreeGroup.cpp : implementation file
//

#include "stdafx.h"
#include "WH.h"
#include "CTaiKlineSetNameTreeGroup.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTaiKlineSetNameTreeGroup dialog


CTaiKlineSetNameTreeGroup::CTaiKlineSetNameTreeGroup(CWnd* pParent /*=NULL*/)
	: CDialog(CTaiKlineSetNameTreeGroup::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTaiKlineSetNameTreeGroup)
	m_sName = _T("");
	//}}AFX_DATA_INIT
	m_bShowFromMouse = false;
}


void CTaiKlineSetNameTreeGroup::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTaiKlineSetNameTreeGroup)
	DDX_Control(pDX, IDC_EDIT1, m_conEdit);
	DDX_Control(pDX, IDOK, m_ok);
	DDX_Control(pDX, IDCANCEL, m_cancel);
	DDX_Text(pDX, IDC_EDIT1, m_sName);
	DDV_MaxChars(pDX, m_sName, 64);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTaiKlineSetNameTreeGroup, CDialog)
	//{{AFX_MSG_MAP(CTaiKlineSetNameTreeGroup)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTaiKlineSetNameTreeGroup message handlers

void CTaiKlineSetNameTreeGroup::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData();
	if(this->m_sName == "") return;
	CDialog::OnOK();
}

void CTaiKlineSetNameTreeGroup::DoMoveWindow()
{
	if(m_bShowFromMouse)
	{
		CPoint p;
		GetCursorPos(&p);
		CRect r;
		GetWindowRect(r);
		this->SetWindowPos(NULL,p.x,p.y,r.Width (),r.Height (),SWP_NOREDRAW);
	}

}

BOOL CTaiKlineSetNameTreeGroup::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_conEdit.SetFocus();
	DoMoveWindow();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
