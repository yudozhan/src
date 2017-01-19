// DialogCwdLeftPop.cpp : implementation file
//

#include "stdafx.h"
#include "WH.h"
#include "DialogCwdLeftPop.h"
#include "CTaiScreenParent.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDialogCwdLeftPop dialog


CDialogCwdLeftPop::CDialogCwdLeftPop(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogCwdLeftPop::IDD, pParent)
{
	m_pIndex = NULL;  
	nKind = -1;         ///////KLine Type/////////
	m_hItem = NULL;

	m_pParent = pParent;
	//{{AFX_DATA_INIT(CDialogCwdLeftPop)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDialogCwdLeftPop::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialogCwdLeftPop)
	DDX_Control(pDX, IDC_TREE1, m_treeFormularCtrl);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDialogCwdLeftPop, CDialog)
	//{{AFX_MSG_MAP(CDialogCwdLeftPop)
	ON_WM_SIZE()
	ON_WM_CLOSE()
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE1, OnSelchangedTreeFormularAll)
	ON_NOTIFY(NM_DBLCLK, IDC_TREE1, OnDblclkTree1)
	ON_WM_NCHITTEST()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialogCwdLeftPop message handlers

void CDialogCwdLeftPop::OnCancel() 
{
	// TODO: Add extra cleanup here
	
//	CDialog::OnCancel();
}

void CDialogCwdLeftPop::OnOK() 
{
	// TODO: Add extra validation here
	
	CDialog::OnOK();
}

BOOL CDialogCwdLeftPop::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_treeFormularCtrl.m_bRefreshEquation   = true;
	m_treeFormularCtrl.m_bNewGroup			=true;
	m_treeFormularCtrl.m_bDeleteGroup		=true;
	m_treeFormularCtrl.m_bModifyGroupName	=true;
	m_treeFormularCtrl.m_bDeleteEquation	=true;
	m_treeFormularCtrl.m_bNewEquation		=true;
#ifdef OEM
	m_treeFormularCtrl.m_bModifyEquation	=false;
#endif

	m_treeFormularCtrl.m_bScreenStock 	=true;
	m_treeFormularCtrl.m_bShowIndicator =true;
	m_treeFormularCtrl.m_bScreenStockIndicator 	=true;
	m_treeFormularCtrl.m_bShowKlineGroup =true;
//	m_treeFormularCtrl.m_bNewEquation		=false;
//	InsertData();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDialogCwdLeftPop::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
/*	CRect r;
	m_pParent->GetClientRect (r);
	m_pParent->ClientToScreen (r);

	CRect rPop;
	this->GetWindowRect (rPop);*/
	CRect r2;
	this->GetClientRect (r2);
	r2.DeflateRect (1,1);
	if(m_treeFormularCtrl.m_hWnd )
		this->m_treeFormularCtrl .MoveWindow (r2);

	
}

void CDialogCwdLeftPop::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	
//	CDialog::OnClose();
}

void CDialogCwdLeftPop::OnSelchangedTreeFormularAll(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	
	HTREEITEM hItem = this->m_treeFormularCtrl.GetSelectedItem ();//this->m_treeFormularCtrl.HitTest(pt, &uFlags);
	if(hItem == NULL)
	{
	  return;
	}
	if ((hItem != NULL) )
	{
	   this->m_treeFormularCtrl .SelectItem(hItem);
	}
    m_hItem = hItem;
	int nKindRoot,nKindItem;
	m_pIndex = NULL;
	bool bSucc = m_treeFormularCtrl.GetCurrentItemInfo (nKindRoot,nKindItem,m_pIndex);	

	int i = 0,j=0;
	CString temp;
	this->nKind = nKindRoot;
	if(nKind == -7 || nKind == -8)
		m_pIndex = CTaiScreenParent::GetSpecialLJishu(-nKind-7);
//	if(m_pIndex == NULL || nKind == -6)
//		goto DO_END;

	*pResult = 0;
}

void CDialogCwdLeftPop::OnDblclkTree1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	m_treeFormularCtrl.CommandShowIndicator();

	*pResult = 0;
}

void CDialogCwdLeftPop::InsertData()
{
	//0为指标公式
	this->m_treeFormularCtrl.InsertEqutionTree (0,true);
	//条件选股公式
	this->m_treeFormularCtrl.InsertEqutionTree (1,true);
	//K线组合公式
	this->m_treeFormularCtrl.InsertEqutionTree (2);


}

void CDialogCwdLeftPop::RefreshJishuTreeCtrl()
{
	m_treeFormularCtrl.DeleteAllItems();

	//0为指标公式
	this->m_treeFormularCtrl.InsertEqutionTree (0);
	//条件选股公式
	this->m_treeFormularCtrl.InsertEqutionTree (1);
	//K线组合公式
	this->m_treeFormularCtrl.InsertEqutionTree (2);

	this->m_pIndex = NULL;
}

LRESULT CDialogCwdLeftPop::OnNcHitTest(CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	LRESULT un = CDialog::OnNcHitTest(point);
	if(un!= HTRIGHT)
		un = HTNOWHERE ;
	return un;
}
