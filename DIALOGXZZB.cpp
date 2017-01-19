// DIALOGXZZB.cpp : implementation file
//

#include "stdafx.h"
#include "WH.h"
#include "DIALOGXZZB.h"
#include "mainfrm.h"
#include "CTaiShanDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDialogLOGXZZB dialog


CDialogLOGXZZB::CDialogLOGXZZB(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogLOGXZZB::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDialogLOGXZZB)
	m_edit = _T("");
	//}}AFX_DATA_INIT
}


void CDialogLOGXZZB::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialogLOGXZZB)
	DDX_Control(pDX, IDOK, m_ok);
	DDX_Control(pDX, IDCANCEL, m_cancel);
	DDX_Control(pDX, IDC_LIST1, m_list);
	DDX_Text(pDX, IDC_EDIT1, m_edit);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDialogLOGXZZB, CDialog)
	//{{AFX_MSG_MAP(CDialogLOGXZZB)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST1, OnItemchangedList1)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST1, OnDblclkList1)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialogLOGXZZB message handlers

void CDialogLOGXZZB::OnItemchangedList1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
//	CMDIChildWnd* pchildfram= ((CMainFrame*)(AfxGetApp()->m_pMainWnd))->MDIGetActive();
    CTaiShanDoc*	pDoc=CMainFrame::m_pDoc ;//(CTaiShanDoc*)pchildfram->GetActiveDocument();
	
//	POSITION position=m_list.GetFirstSelectedItemPosition();
//	int index=m_list.GetNextSelectedItem(position);
	int index=m_list.GetNextItem(-1,LVIS_SELECTED);
	if(index>=0)
		m_edit=pDoc->m_formuar_index.GetAt(index)->help;
	UpdateData(FALSE);
	
	*pResult = 0;
}

void CDialogLOGXZZB::OnOK() 
{
	// TODO: Add extra validation here
    CTaiShanDoc*	pDoc=CMainFrame::m_pDoc ;//(CTaiShanDoc*)pchildfram->GetActiveDocument();
	
	int index=m_list.GetNextItem(-1,LVIS_SELECTED);
	if(index>=0)
		result=pDoc->m_formuar_index.GetAt(index)->name;

	
	CDialog::OnOK();
}

BOOL CDialogLOGXZZB::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
    CTaiShanDoc*	pDoc=CMainFrame::m_pDoc ;//(CTaiShanDoc*)pchildfram->GetActiveDocument();
	
	int length=pDoc->m_formuar_index.GetSize();
	if(length == 0)
	{
		MessageBox("���κ�ָ�꣬��������ָ�깫ʽ");
	    return true;
	}
	m_list.InsertColumn(0,"ָ������",LVCFMT_LEFT,80,0);
	m_list.InsertColumn(1,"ָ������ʡ��ص�ļ��˵��",LVCFMT_LEFT,200,1);
	m_list.SetImageList(pDoc->m_imagelist,LVSIL_SMALL);
	for(int i=0;i<length;i++)
	{
		CString data;
		data=pDoc->m_formuar_index.GetAt(i)->name;
		m_list.InsertItem(i,data,4);
		data=pDoc->m_formuar_index.GetAt(i)->explainBrief;
		m_list.SetItemText(i,1,data);
	}
	m_edit=pDoc->m_formuar_index.GetAt(0)->help;
	m_list.SetItemState(0,LVIS_SELECTED ,LVIF_STATE );
	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDialogLOGXZZB::OnDblclkList1(NMHDR* pNMHDR, LRESULT* pResult) 
{
   *pResult = 0;
   OnOK();
   EndDialog(1);
}
