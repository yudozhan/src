// XgnDlgSelID.cpp : implementation file
//

#include "stdafx.h"
#include "WH.h"
#include "XgnDlgSelID.h"
#include "CTaiScreenParent.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// XgnDlgSelID dialog


XgnDlgSelID::XgnDlgSelID(CWnd* pParent /*=NULL*/)
	: CDialog(XgnDlgSelID::IDD, pParent)
{
	//{{AFX_DATA_INIT(XgnDlgSelID)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_nSel = 0;
}


void XgnDlgSelID::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(XgnDlgSelID)
	DDX_Control(pDX, IDC_LIST1, m_listData);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(XgnDlgSelID, CDialog)
	//{{AFX_MSG_MAP(XgnDlgSelID)
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// XgnDlgSelID message handlers

void XgnDlgSelID::OnOK() 
{
	// TODO: Add extra validation here
	int nItem = 0;
	////////Get the selected List Item/////////
	POSITION pos = this->m_listData .GetFirstSelectedItemPosition();
    if(pos == NULL)
	{
		TRACE0("No items were selected!\n");
		return;
	}
    else
	{
        nItem = m_listData.GetNextSelectedItem(pos);
	}

	if(nItem>= m_infoArr.GetSize ()) return ;
	this->m_nSel = m_infoArr[nItem].m_nID ;
	
	CDialog::OnOK();
}

void XgnDlgSelID::OnCancel() 
{
	// TODO: Add extra cleanup here
	m_nSel = -1;
	
	CDialog::OnCancel();
}

BOOL XgnDlgSelID::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_listData.SetExtendedStyle(m_listData.GetExtendedStyle()|LVS_EX_GRIDLINES|LVS_EX_FULLROWSELECT|LVS_EX_INFOTIP);
	m_listData.InsertColumn(0,"����",LVCFMT_LEFT,80);
	m_listData.InsertColumn(1,"˵��",LVCFMT_LEFT,120);
	m_listData.InsertColumn(2,"���к�",LVCFMT_LEFT,60);
	m_listData.InsertColumn(3,"Ŀ������",LVCFMT_LEFT,60);

	//insert
	InsertListInfo();

	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void XgnDlgSelID::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	m_nSel = -1;
	CDialog::OnClose();
}

void XgnDlgSelID::InsertListInfo()
{
	m_infoArr.RemoveAll ();
	IndicatorExtendData::GetIndicatorInfo(m_infoArr);

	this->m_listData .DeleteAllItems ();
	int n = this->m_infoArr .GetSize ();
	for(int i = 0;i<n;i++)
	{
		CString s= m_infoArr[i].m_chDataName;
		m_listData.InsertItem(i, s);
		s = m_infoArr[i].m_chName ;
		s+= "(" ;
		s+= m_infoArr[i].m_chSubName ;
		s += ")  ";
		s += CTaiScreenParent::GetPeriodName(m_infoArr[i].m_nKlineKindCompute );
		m_listData.SetItemText(i,1,s );
		s.Format ("%d",m_infoArr[i].m_nID );
		m_listData.SetItemText(i,2,s);
		s = "��K��";
		if(m_infoArr[i].m_nKlineKind == 1)
			s = "5����K��";
		m_listData.SetItemText(i,3,s );
	}
}
