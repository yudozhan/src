// SHEETBHADD.cpp : implementation file
//

#include "stdafx.h"
#include "WH.h"
#include "SHEETBHADD.h"
#include "CTaiShanDoc.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSHEETBHADD dialog


CSHEETBHADD::CSHEETBHADD(CWnd* pParent /*=NULL*/)
	: CDialog(CSHEETBHADD::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSHEETBHADD)
	m_bkname = _T("");
	//}}AFX_DATA_INIT
	m_OpenKind=FALSE;
}


void CSHEETBHADD::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSHEETBHADD)
	DDX_Control(pDX, IDC_BKNAME, m_cBKNAME);
	DDX_Control(pDX, IDC_STATIC99, m_Static);
	DDX_Text(pDX, IDC_BKNAME, m_bkname);
	DDV_MaxChars(pDX, m_bkname, 10);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSHEETBHADD, CDialog)
	//{{AFX_MSG_MAP(CSHEETBHADD)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSHEETBHADD message handlers

BOOL CSHEETBHADD::OnInitDialog() 
{
	CMDIChildWnd* pchildfram= ((CMainFrame*)(AfxGetApp()->m_pMainWnd))->MDIGetActive();
    CTaiShanDoc*	pDoc=(CTaiShanDoc*)pchildfram->GetActiveDocument();

	CDialog::OnInitDialog();
	CString  namedefault,namefirst="�°��";
///////////////////////////////////////////////////

	if(m_OpenKind)
	{
		  this->m_Static.SetWindowText(_T("��������������ѡ�ɰ����"));
          namefirst="����ѡ��";
		  this->SetWindowText(_T("��������ѡ�ɰ�� ")) ;
	}
///////////////////////////////////////////////////
	int j=0;
	bool isnewnameok;
	do
	{
		isnewnameok=true;
		j++;
		CString str;
		str.Format("%d",j);
		namedefault=namefirst+str;
		POSITION  positionhead=pDoc->m_Tjxgblocklist.GetHeadPosition();
		POSITION  position=positionhead;
		int totalbk=pDoc->m_Tjxgblocklist.GetCount();
		
		for(int i=0;i<totalbk;i++)
		{
			if(strcmp(namedefault,pDoc->m_Tjxgblocklist.GetNext(position)->blockname)==0)
			isnewnameok=false;
		}
	}while(!isnewnameok);
	m_bkname=namedefault;
	this->UpdateData(FALSE);
	
	// TODO: Add extra initialization here

	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CSHEETBHADD::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData();
	m_bkname.Remove(' ');
	if(m_bkname!="")
		CDialog::OnOK();
	else
	{
		m_cBKNAME.SetFocus();
		AfxMessageBox("���������Ϊ��",MB_ICONSTOP);
	}
}
