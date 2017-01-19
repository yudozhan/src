// CTaiScreenTest.cpp : implementation file
//
#include "stdafx.h"
#include "WH.h"
#include "CTaiScreenTest.h"
#include "windowsx.h"
#include "CTaiKlineFileHS.h"
#include "SelectStock.h"
#include "CTaiKlineDlgStopLose.h"
#include <stdio.h>
#include "CTaiScreenTestBuySellResult.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTaiScreenTest dialog
extern CTime g_timeBegin;
extern CTime g_timeEnd;


CTaiScreenTest::CTaiScreenTest(CWnd* pParent /*=NULL*/)
	: CTaiScreenParent(CTaiScreenTest::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTaiScreenTest)
	m_stock_number = 0;
	m_starttime = 0;
	m_endtime = 0;
	m_mbly = 0.0f;
	m_mbzq = 0;
//	m_bCaclYHParam = FALSE;
	//}}AFX_DATA_INIT
	m_bExecuteCalc = false;
	m_bShowBuyCase = true;
	m_pExplainButton = &m_buttonExpl;
	
}


void CTaiScreenTest::DoDataExchange(CDataExchange* pDX)
{
	CTaiScreenParent::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTaiScreenTest)
	DDX_Control(pDX, IDCANCEL, m_cancel);
	DDX_Control(pDX, IDOK, m_ok);
	DDX_Control(pDX, IDC_BUTTON1, m_button1);
	DDX_Control(pDX, IDC_DELETE_STOCKS, m_deleteStocks);
	DDX_Control(pDX, IDC_ADD_STOCKS_PARENT, m_addStocks);
	DDX_Control(pDX, IDC_EDIT7, m_floatEdit2);
	DDX_Control(pDX, IDC_EDIT1, m_floatEdit1);
	DDX_Control(pDX, IDC_BUTTON2, m_buttonExpl);
	DDX_Control(pDX, IDC_PROGRESS1, m_progress);
	DDX_Control(pDX, IDC_LIST_ADD_STOCKS, m_liststockrange);
	DDX_Text(pDX, IDC_STOCK_NUMBER, m_stock_number);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER1, m_starttime);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER2, m_endtime);
	DDX_Text(pDX, IDC_EDIT1, m_mbly);
	DDX_Text(pDX, IDC_EDIT7, m_mbzq);
	DDX_Radio(pDX, IDC_BUY_CASE, m_bBuy);
	DDX_Check(pDX, IDC_CHECK1, m_bCaclYHParam);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTaiScreenTest, CTaiScreenParent)
	//{{AFX_MSG_MAP(CTaiScreenTest)
	ON_BN_CLICKED(IDOK, OnStockTest)
	ON_BN_CLICKED(IDC_ADD_STOCKS_PARENT, OnAddStocksParent)
	ON_BN_CLICKED(IDC_DELETE_STOCKS, OnDeleteStocks)
	ON_BN_CLICKED(IDC_BUTTON2, OnButton2)
	ON_BN_CLICKED(IDC_BUTTON1, OnStopLose)
	ON_WM_CLOSE()
	ON_NOTIFY(LVN_KEYDOWN, IDC_LIST_ADD_STOCKS, OnKeydownListAddStocks)
	ON_WM_HELPINFO()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTaiScreenTest message handlers

void CTaiScreenTest::OnStockTest() 
{
	// TODO: Add your control notification handler code here
	 //MSG message;
	if(m_bExecuteCalc == true)
	{
		m_bExecuteCalc = false;
		EnableControl(true);
	    return;
	}

	if(!UpdateData(TRUE))
		return;

	IndexDataInfo indexSaveCur;
	bool bSelCur = GetCurSelData(indexSaveCur);
	 if(m_indexSaveArray.GetSize()==0 && !bSelCur)
	 {
		 AfxMessageBox("请先选择指标");
		 return;
	 }
	 //int Starttime=atoi(m_starttime.Format("%Y%m%d"));
	 //int Endtime  =atoi(m_endtime.Format("%Y%m%d"));
	 CTime CurTime=CTime::GetCurrentTime();
	 if(m_starttime>m_endtime)
	 {
		 AfxMessageBox("时间设置颠倒，请重新设置！",MB_ICONSTOP);
		 ((CComboBox*)GetDlgItem(IDC_DATETIMEPICKER1))->SetFocus();
		 return;
	 }
	 if(m_starttime>CurTime)
	 {
		 AfxMessageBox("开始时间设置错误，请重新设置！",MB_ICONSTOP);
		 ((CComboBox*)GetDlgItem(IDC_DATETIMEPICKER1))->SetFocus();
		 return;
	 }
	 if(m_endtime>CurTime)
	 {
		 AfxMessageBox("结束时间设置错误，请重新设置！",MB_ICONSTOP);
		 ((CComboBox*)GetDlgItem(IDC_DATETIMEPICKER2))->SetFocus();
		 return;
	 }
	 if(m_mbly<0)
	 {
		 AfxMessageBox("目标利润不能为负",MB_ICONSTOP);
		 ((CComboBox*)GetDlgItem(IDC_EDIT1))->SetFocus();
		 return;
	 }
	 if(m_mbzq<=0)
	 {
		 AfxMessageBox("目标周期应为大于0的整数",MB_ICONSTOP);
         ((CComboBox*)GetDlgItem(IDC_EDIT7))->SetFocus();
		(GetDlgItem(IDOK))->EnableWindow(TRUE);
		 return;
	 }
	 
//	 (GetDlgItem(IDOK))->EnableWindow(FALSE);
//	 EnableWindow(FALSE);
	 int stocknum=m_stockchoose.GetSize();
	 int index_num=m_indexSaveArray.GetSize();
	 //清除上一次结果
	 ClearTestResultArray();

	 Kline *pKline=NULL;
     CBuySellList buySellList;

     m_progress.SetRange32(0,stocknum);
	 m_bExecuteCalc = true;
	 EnableControl(false);

	 int nPeriod = 6;
	 char str22[20];
	 m_period.GetLBText(m_period.GetCurSel(),str22);
	 int i;
	 for(i=0;i<11;i++)
	 {
		if(strcmp(str22,m_gPeriodName[i]) == 0)
		{
		  nPeriod = i;//nLKind[i];
		}
	 }

	 if(m_indexSaveArray.GetSize()==0 && bSelCur)
		m_indexSaveArray.Add(indexSaveCur);
     for(i=0;i<stocknum && m_bExecuteCalc == true;i++)// 计算每个股票
	 {
		int nRead;
		CString StockId;
		StockId=(m_stockchoose.GetAt(i)).m_chSymbol;
		int stkKind = (m_stockchoose.GetAt(i)).m_nSymbolKind;

		m_progress.OffsetPos(1);
		nRead=ReadKLine(StockId,stkKind,pKline,-1,nLKind[nPeriod],&buySellList,48,TRUE);//,&m_endtime,&m_starttime);
	   	
		TestSuccessEach( StockId,stkKind, nRead, pKline, m_mbzq,m_mbly/100,&buySellList,&m_endtime,&m_starttime);

		if(CMainFrame::m_pWndTJXGTest == NULL) return;
		  //GetDlgItem(IDC_CHOOSED_NUM)->UpdateWindow();
		 // UpdateData(FALSE);
    }
//	if(linebe.line!=NULL)
//		delete [] linebe.line;
	CTaiKlineFileHS::RemoveHs(buySellList);

	  if(pKline)
	  {
		delete[] pKline;        //释放内存
		pKline=NULL;
	  }
	
	//
	UpdateData(TRUE);
	m_progress.SetPos(0);
	EnableControl(true);
	if(m_bExecuteCalc == false) return;
	m_bExecuteCalc = false;

    CTaiScreenTestBuySellResult m_BbResult(this);
	m_BbResult.DoModal();

	 if(m_indexSaveArray.GetSize()==1 && bSelCur)
		 m_indexSaveArray.RemoveAt(0);
	
}

BOOL CTaiScreenTest::OnInitDialog() 
{
	m_pStockSelectList = &m_liststockrange;
	CTaiScreenParent::OnInitDialog();

	m_bBuy = 0;
	m_bDlgTest = true;
	
	// TODO: Add extra initialization here
	//条件选股公式
	this->m_treeFormularCtrl.InsertEqutionTree (1,true);
	this->m_treeFormularCtrl.InsertEqutionTree (-6);
	//0为指标公式
	this->m_treeFormularCtrl.InsertEqutionTree (0,true);
	//K线组合公式
	this->m_treeFormularCtrl.InsertEqutionTree (2);
	HTREEITEM hh = m_treeFormularCtrl.GetRootItem( );
	m_treeFormularCtrl.EnsureVisible(hh);
	m_and_or = 0;
	//buy_or_sell=0;
	//m_starttime=CTime::GetCurrentTime();
	m_endtime = g_timeEnd;
	this->m_starttime  = g_timeBegin;
	m_mbly=10.0;
	m_mbzq=20;
	CTime pMinRange = CTime(1988,9,1,1,1,1);
	CTime pMaxRange = CTime(2037,9,1,1,1,1);
	((CDateTimeCtrl*)GetDlgItem(IDC_DATETIMEPICKER1))->SetRange( &pMinRange, &pMaxRange );
	((CDateTimeCtrl*)GetDlgItem(IDC_DATETIMEPICKER2))->SetRange( &pMinRange, &pMaxRange );
	UpdateData(FALSE);
	///////////////////
	///////////////Add stock kind//////////////////
	LVITEM myitem;
	myitem.mask=LVIF_TEXT|LVIF_IMAGE;
	myitem.iSubItem=0;
	myitem.iImage=6;
	m_liststockrange.SetImageList(&m_imageList,LVSIL_SMALL);

   	CRect           rect;
	CString         strItem1= _T("证券名称");
	CString         strItem2= _T("类型");
	// insert two columns (REPORT mode) and modify the new header items
	CTaiScreenParent::FillToListCtrlHead(&m_liststockrange);

	//CString  stockgroup[]={"沪指","上证A股","上证B股","深指","深证A股","深证B股","自选板块"};
	SelectStock::FillToListCtrlItem(&m_liststockrange, "上证Ａ股", 1);
	SelectStock::FillToListCtrlItem(&m_liststockrange, "深证Ａ股", 4);
	SelectStock::GetStockSymbolArray(&m_liststockrange,m_stockchoose);
	m_stock_number = m_stockchoose.GetSize ();
	ListCtrlSetSelectItem(&m_liststockrange);//select an item

	//SelectStock::FillToListCtrlItem(CListCtrl *pList, char *Id, int nParam, int StockType);
	////////////Add for Test/////////////
	for(int i=0;i<8;i++)
      m_sSubFormular.AddString(m_subIndexGroup[i]);
    m_sSubFormular.SetCurSel(0);
	////////////
	OnSelchangeListSubFormular();
//	this->m_save_list.SetCurSel(0);
    /////////////////////////
//	ShowSetCompute(false);

	//设置时间范围

//	this->m_conTmEnd.SetRange( &pMinRange, &pMaxRange );
//	m_conTmEnd.EnableWindow(FALSE);
//	m_tmEnd = CTime::GetCurrentTime ();
	UpdateData(FALSE);
	//end

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CTaiScreenTest::OnAddStocksParent() 
{
	// TODO: Add your control notification handler code here
/*	UpdateData();
	SelectStock pDlg(this,FALSE,true);
	pDlg.m_pSelectResultList  = &m_liststockrange;
	pDlg.DoModal();
	SelectStock::GetStockSymbolArray(&m_liststockrange,m_stockchoose);
	m_stock_number = m_stockchoose.GetSize ();
	ListCtrlSetSelectItem(&m_liststockrange);//select an item

	UpdateData(FALSE);*/
	AddStocks();

}

void CTaiScreenTest::OnDeleteStocks() 
{
	// TODO: Add your control notification handler code here
	DeleteStocks();
	/*
	UpdateData();
	SelectStock::DeleteSelectedItem(&m_liststockrange);
	SelectStock::GetStockSymbolArray(&m_liststockrange,m_stockchoose);
	m_stock_number = m_stockchoose.GetSize ();
	UpdateData(FALSE);*/
	
}

void CTaiScreenTest::OnButton2() 
{
	// TODO: Add your control notification handler code here
	ShowExplain(this->m_pIndex);
	
}

void CTaiScreenTest::OnStopLose() 
{
	// TODO: Add your control notification handler code here
	CTaiKlineDlgStopLose DlgInsert(this);
	DlgInsert.DoModal();
	
}

void CTaiScreenTest::RefreshJishuTreeCtrl()
{
	m_treeFormularCtrl.DeleteAllItems();

	//条件选股公式
	this->m_treeFormularCtrl.InsertEqutionTree (1);
	//0为指标公式
	this->m_treeFormularCtrl.InsertEqutionTree (0);
	//K线组合公式
	this->m_treeFormularCtrl.InsertEqutionTree (2);

	this->m_pIndex = NULL;

}

void CTaiScreenTest::OnCancel() 
{
	// TODO: Add extra cleanup here
	OnClose();
}

void CTaiScreenTest::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	UpdateData();
	g_timeEnd = m_endtime;
	g_timeBegin  = m_starttime;

	if(m_bExecuteCalc == false)
		DestroyWindow();
	
//	CTaiScreenParent::OnClose();
}

void CTaiScreenTest::PostNcDestroy() 
{
	// TODO: Add your specialized code here and/or call the base class
	
	CMainFrame::m_pWndTJXGTest  = NULL;
	m_bExecuteCalc = false;
	delete this;
	CTaiScreenParent::PostNcDestroy();
}

void CTaiScreenTest::OnKeydownListAddStocks(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_KEYDOWN* pLVKeyDow = (LV_KEYDOWN*)pNMHDR;
	// TODO: Add your control notification handler code here
	
	*pResult = 0;
}

BOOL CTaiScreenTest::OnHelpInfo(HELPINFO* pHelpInfo) 
{
//	HtmlHelp(m_hWnd,"stock.chm",HH_HELP_CONTEXT,IDD_6_TJXG_TEST);
	DoHtmlHelp(this);return TRUE;
//	return TRUE;	
}

void CTaiScreenTest::EnableControl(bool bEnable)
{
	int nID[] = {IDC_ADD,IDC_DELETE,IDC_LOAD,IDC_SAVE,IDC_ADD_STOCKS_PARENT,IDC_DELETE_STOCKS,IDC_BUTTON1,
		IDC_DATETIMEPICKER1,IDC_DATETIMEPICKER2,IDC_EDIT1,IDC_EDIT7,
		IDC_TREE_FORMULAR_ALL,IDC_LIST_ADD_STOCKS};

	for(int i=0;i<13;i++)
		GetDlgItem(nID[i])->EnableWindow(bEnable);
	if(bEnable == true)
		GetDlgItem(IDOK)->SetWindowText("测试");
	else
		GetDlgItem(IDOK)->SetWindowText("停止");

}

void CTaiScreenTest::NotifySubclass()
{
	UpdateData();
	SelectStock::GetStockSymbolArray(&m_liststockrange,m_stockchoose);
	m_stock_number = m_stockchoose.GetSize ();
	UpdateData(FALSE);
}

int CTaiScreenTest::GetPeriodTest()
{
	int n = 5;
	if(m_indexSaveArray.GetSize ()>0)
	{
		n = nLKind[m_indexSaveArray[m_indexSaveArray.GetSize ()-1].iDataType];
	}
	return n;

}

int CTaiScreenTest::GetIndexKline(Kline *&pKline)
{
	CString symbol = "1A0001";
     CBuySellList buySellList;
	int nRead=ReadKLine(symbol,SHZS,pKline,-1,GetPeriodTest(),&buySellList,48,TRUE,&m_endtime,&m_starttime);
	CTaiKlineFileHS::RemoveHs(buySellList);
	return nRead;
}
