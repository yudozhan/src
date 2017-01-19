// DlgShowInformation.cpp : implementation file
//

#include "stdafx.h"
#include "WH.h"
#include "DlgShowInformation.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDialogShowInformation dialog


CDialogShowInformation::CDialogShowInformation(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogShowInformation::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDialogShowInformation)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_sHtml = "";
}


void CDialogShowInformation::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialogShowInformation)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDialogShowInformation, CDialog)
	//{{AFX_MSG_MAP(CDialogShowInformation)
	ON_WM_CLOSE()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDOK, OnOk)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialogShowInformation message handlers

void CDialogShowInformation::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	;
	EndDialog(IDOK);
	//CDialog::OnClose();
}

BOOL CDialogShowInformation::OnInitDialog() 
{
	CDialog::OnInitDialog();

	CRect r;
	GetClientRect(r);
	r.bottom -= 50;
	m_htmlShow.Create(this,r);

	if(m_sHtml!="")
		m_htmlShow.Navigate (m_sHtml.GetBuffer (0));

	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDialogShowInformation::OnFinalRelease() 
{
	// TODO: Add your specialized code here and/or call the base class
	
	CDialog::OnFinalRelease();
}

void CDialogShowInformation::OnDestroy() 
{
	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	
}

void CDialogShowInformation::PostNcDestroy() 
{
	// TODO: Add your specialized code here and/or call the base class
	
	CDialog::PostNcDestroy();
}

void CDialogShowInformation::OnOk() 
{
	// TODO: Add your control notification handler code here
	OnClose() ;
}

void CDialogShowInformation::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	OnClose() ;
}
