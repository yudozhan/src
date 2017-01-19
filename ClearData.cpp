// ClearData.cpp : implementation file
//

#include "stdafx.h"
#include "WH.h"
#include "ClearData.h"
#include "mainfrm.h"
#include "CTaiKlineFileKLine.h"
#include "KEYBRODEANGEL.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CClearData property page

IMPLEMENT_DYNCREATE(CClearData, CPropertyPage)


CClearData::CClearData() : CPropertyPage(CClearData::IDD)
{
	//m_warning=true;
	m_Shanghai=FALSE;
	m_Shenzhen=FALSE;
	//{{AFX_DATA_INIT(CClearData)
	m_Invacode = FALSE;
	m_Price = FALSE;
	m_Invadate = FALSE;
	m_Continuous = FALSE;
	m_Chaos = FALSE;
	m_Date = FALSE;
	m_Cjiaoliang = FALSE;
	m_clear_start = 0;
	m_clear_end = 0;
	m_Smbl = _T("");
	//}}AFX_DATA_INIT
	m_nFlag=0;
	pDoc=CMainFrame::m_pDoc;
}

CClearData::~CClearData()
{
}

void CClearData::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CClearData)
	DDX_Control(pDX, IDCANCEL, m_cancel);
	DDX_Control(pDX, IDC_BUTTON1, m_button1);
	DDX_Control(pDX, IDC_LIST, m_listfw);
	DDX_Control(pDX, IDC_PROGRESS1, m_progress);
	DDX_Check(pDX, 1029, m_Invacode);
	DDX_Check(pDX, 1024, m_Price);
	DDX_Check(pDX, 1023, m_Invadate);
	DDX_Check(pDX, 1027, m_Chaos);
	DDX_Check(pDX, 1026, m_Date);
	DDX_Check(pDX, 1028, m_Cjiaoliang);
	DDX_DateTimeCtrl(pDX, 1012, m_clear_start);
	DDX_DateTimeCtrl(pDX, 1018, m_clear_end);
	DDX_Text(pDX, stocknumcode, m_Smbl);
	DDV_MaxChars(pDX, m_Smbl, 6);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CClearData, CPropertyPage)
	//{{AFX_MSG_MAP(CClearData)
	ON_BN_CLICKED(1026, Onzdsd)
	ON_BN_CLICKED(IDCANCEL, OnClearData)
	ON_CBN_SELCHANGE(1042, OnSelchangezlfs)
	ON_BN_CLICKED(IDC_clear, OnClearOneStock)
	ON_EN_CHANGE(stocknumcode, OnChangeStockSymbol)
	ON_NOTIFY(NM_CLICK, IDC_LIST, OnClickList)
	ON_WM_HELPINFO()
	ON_BN_CLICKED(IDC_BUTTON1, OnButton4To6)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CClearData message handlers

BOOL CClearData::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	// TODO: Add extra initialization here

	m_Shanghai=FALSE;
	m_Shenzhen=FALSE;
	GetDlgItem(IDC_Sta4)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_Sta5)->ShowWindow(SW_HIDE);
	GetDlgItem(stocknumcode)->ShowWindow(SW_HIDE);
	GetDlgItem(stocknumname)->ShowWindow(SW_HIDE);
	//GetDlgItem(IDC_clear)->ShowWindow(SW_HIDE);

	((CComboBox*)GetDlgItem(1011))->SetCurSel(0);
	((CComboBox*)GetDlgItem(1042))->SetCurSel(0);
	GetDlgItem(1012)->EnableWindow(FALSE);
	GetDlgItem(1018)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON1)->ShowWindow(SW_HIDE);
	LVITEM myitem;
	myitem.mask=LVIF_TEXT|LVIF_IMAGE;
	myitem.iSubItem=0;
	myitem.iImage=6;
	m_listfw.SetImageList(pDoc->m_imagelist,LVSIL_SMALL);
	CString stockgroup[]={"上海","深圳","创业"};
	for(int i=0;i<3;i++)
	{
		if(i==2)
			myitem.iImage=9;
		myitem.iItem=i;
		myitem.pszText=(LPTSTR)stockgroup[i].GetBuffer(stockgroup[i].GetLength());
		m_listfw.InsertItem(&myitem);
		if(i==2)
		   m_stockrange.Add(FALSE);
		else
		   m_stockrange.Add(TRUE);
	}
	//SetTimer(1,50,NULL);
	//时间初始化
	this->m_clear_start=CTime(1999,1,1,1,1,1);
	this->m_clear_end=CTime::GetCurrentTime();
	UpdateData(FALSE);
    ////////////////////////////////////////////////
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

/*void CClearData::OnClickxzfw(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	//替换位图，将选中范围以另一种位图来显示
	int item=m_listfw.GetNextItem(-1,LVNI_SELECTED);
	if(item!=-1)
	{
		LVITEM myitem;
		myitem.mask=LVIF_TEXT|LVIF_IMAGE;
		myitem.iSubItem=0;
		CString text=m_listfw.GetItemText(item,0);
		myitem.pszText=text.GetBuffer(text.GetLength());
		myitem.iItem=item;
		if(m_stockrange.GetAt(item)==TRUE)
		{
			myitem.iImage=6;
			m_stockrange.SetAt(item,FALSE);
		}
		else 
		{
			myitem.iImage=7;
			m_stockrange.SetAt(item,TRUE);
		}
		m_listfw.SetItem(&myitem);
		m_listfw.Update(item);
	}

	*pResult = 0;
}*/

void CClearData::Onzdsd() 
{
	// TODO: Add your control notification handler code here
    if(!((CButton*)GetDlgItem(1026))->GetCheck())
	{
		GetDlgItem(1012)->EnableWindow(FALSE);
		GetDlgItem(1018)->EnableWindow(FALSE);
	}
	else
	{
		GetDlgItem(1012)->EnableWindow(TRUE);
		GetDlgItem(1018)->EnableWindow(TRUE);
	}
	
}

void CClearData::OnClearData() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	if(((CButton*)GetDlgItem(1026))->GetCheck() )
       if(m_clear_start<=0||m_clear_end<=0)
		{
			AfxMessageBox("时间设置错误!");
			return;
		}
	if(((CComboBox*)GetDlgItem(1042))->GetCurSel()==0)
	{
		m_Shanghai=FALSE;
		m_Shenzhen=FALSE;
		if(m_Date)
		{
			int clearstart=atoi(m_clear_start.Format("%Y%m%d"));
			int clearend  =atoi(m_clear_end.Format("%Y%m%d"));
			if(clearstart>clearend)
			{
				AfxMessageBox("时间颠倒，重新输入！",MB_ICONSTOP);
				return;
			}
		}
		int length=m_stockrange.GetSize();
		for(int i=0;i<length;i++)
		{
			if(m_stockrange.GetAt(i)==TRUE)
				if(i==0)
					m_Shanghai=TRUE;
				else if(i==1)
					m_Shenzhen=TRUE;
		}
		/*HTREEITEM hTreeItem;
		CTreeCtrl *pTreeCtrl=((CTreeCtrl*)GetDlgItem(1000));
		hTreeItem=pTreeCtrl->GetFirstVisibleItem();
		while(hTreeItem!=NULL)
		{
			if(pTreeCtrl->GetCheck(hTreeItem))
			{
				if(pTreeCtrl->GetItemText(hTreeItem)=="上海")
				//AfxMessageBox( pTreeCtrl->GetItemText(hTreeItem) );
					m_Shanghai=TRUE;
				if(pTreeCtrl->GetItemText(hTreeItem)=="深圳")
					m_Shenzhen=TRUE;
			}
			hTreeItem=pTreeCtrl->GetNextVisibleItem(hTreeItem);
		}*/
		if(m_Shanghai==FALSE&&m_Shenzhen==FALSE)
		{
			AfxMessageBox("请选择范围！",MB_ICONSTOP);
			return;
		}
		//if(((CComboBox*)GetDlgItem(1042))->GetCurSel()==0)
		
		//得到文档指针
		CMainFrame *pMainWnd=(CMainFrame*)AfxGetApp()->m_pMainWnd;
		CTaiShanDoc *pDoc=pMainWnd->m_pDoc;
		if(pDoc==NULL)
			return;
		::SetCurrentDirectory(pDoc->m_CurrentWorkDirectory);

		//if(m_warning==true)
			if(AfxMessageBox("是否真的要进行整理？",MB_ICONQUESTION|MB_YESNO|MB_DEFBUTTON2)==IDNO)
				return;

		 if(!((CButton*)GetDlgItem(1029))->GetCheck()&&
			 !((CButton*)GetDlgItem(1024))->GetCheck()&&
			 !((CButton*)GetDlgItem(1023))->GetCheck()&&
			 !((CButton*)GetDlgItem(1027))->GetCheck()&&
			 !((CButton*)GetDlgItem(1028))->GetCheck()&&
			 !((CButton*)GetDlgItem(1026))->GetCheck()  )
		 {
			 AfxMessageBox("没有指定条件");
			 return;
		 }
		 
		 SetCapture();
		 BeginWaitCursor();

		 //深圳和上海的日线数据文件
		 //the total number of stocks
		 if(((CComboBox*)GetDlgItem(1011))->GetCurSel()==0)
		 {
		 int TotalNumStock=0,NumStock_sh=0,NumStock_sz=0;
		 if(m_Shanghai==TRUE)
		 {
			 NumStock_sh=CTaiKlineFileKLine::m_fileDaySh->GetStockNumber();
			 TotalNumStock=NumStock_sh;
		 }
		 if(m_Shenzhen==TRUE)
		 {
			 NumStock_sz=CTaiKlineFileKLine::m_fileDaySz->GetStockNumber();
			 TotalNumStock+=NumStock_sz;
		 }
		 m_progress.SetRange(0,TotalNumStock);
		 //define sharesymbol and klinesmallhead pointer
		 int ii;
		 //for shanghai 
		 if(m_Shanghai==TRUE)
		 {
			 for(ii=0;ii<NumStock_sh;ii++)
			 {
				 m_progress.OffsetPos(1);
				 Clear(ii,true,true);
			 }
		 }
		 //for shenzhen
		 if(m_Shenzhen==TRUE)
		 {
			 for(ii=0;ii<NumStock_sz;ii++)
			 {
				 m_progress.OffsetPos(1);
				 Clear(ii,false,true);
			 }
		 }
		 m_progress.SetPos(TotalNumStock);
		 }
		 else if(((CComboBox*)GetDlgItem(1011))->GetCurSel()==1)
		 {
			 int TotalNumStock=0,NumStock_sh=0,NumStock_sz=0;
			 if(m_Shanghai==TRUE)
			 {
				 NumStock_sh=CTaiKlineFileKLine::m_fileMin5Sh->GetStockNumber();
				 TotalNumStock=NumStock_sh;
			 }
			 if(m_Shenzhen==TRUE)
			 {
				 NumStock_sz=CTaiKlineFileKLine::m_fileMin5Sz->GetStockNumber();
				 TotalNumStock+=NumStock_sz;
			 }
			 m_progress.SetRange(0,TotalNumStock);
			 int ii;
    		 //for shanghai 
			if(m_Shanghai==TRUE)
			{
				for(ii=0;ii<NumStock_sh;ii++)
			 {
				 m_progress.OffsetPos(1);
				 Clear(ii,true,false);
			 }
			}
			//for shenzhen
			 if(m_Shenzhen==TRUE)
			 {
			   for(ii=0;ii<NumStock_sz;ii++)
			 {
				 m_progress.OffsetPos(1);
				 Clear(ii,false,false);
			 }
		 }
		 m_progress.SetPos(TotalNumStock);
		 }
		 m_progress.SetPos(0);
		 ReleaseCapture();
		 EndWaitCursor();
		// AfxMessageBox("整理完毕！");
		
}
	if(((CComboBox*)GetDlgItem(1042))->GetCurSel()==1)
	{
        ::SetCurrentDirectory(pDoc->m_CurrentWorkDirectory);
		((CProgressCtrl*)GetDlgItem(IDC_PROGRESS1))->SetRange(0,100);
		//根据代码对照表来进行整理, 只保存代码对照表中有的股票.
		if(((CComboBox*)GetDlgItem(1011))->GetCurSel()==0)
		    CTaiKlineFileKLine::ReorgnizeFile(true,true,true);
		else if(((CComboBox*)GetDlgItem(1011))->GetCurSel()==1)
			CTaiKlineFileKLine::ReorgnizeFile(true,true,false);
		((CProgressCtrl*)GetDlgItem(IDC_PROGRESS1))->OffsetPos(100);
		((CProgressCtrl*)GetDlgItem(IDC_PROGRESS1))->SetPos(0);
		AfxMessageBox(_T("整理完成"));
	}

	//删除指定股票的所有数据
	if(((CComboBox*)GetDlgItem(1042))->GetCurSel()==2)
	{
//		OnClearOneStock();
	}
}



void CClearData::Clear(int nStock, bool bShanghai,bool bDayKline)
{
	CTaiKlineFileKLine *pFile;//=CTaiKlineFileKLine::GetFilePointer(nStock,bDayKline);
	if(bShanghai==true&&bDayKline==true)
		pFile=CTaiKlineFileKLine::m_fileDaySh;
	else if(bShanghai==false&&bDayKline==true)
		pFile=CTaiKlineFileKLine::m_fileDaySz;
	else if(bShanghai==true&&bDayKline==false)
		pFile=CTaiKlineFileKLine::m_fileMin5Sh;
	else if(bShanghai==false&&bDayKline==false)
		pFile=CTaiKlineFileKLine::m_fileMin5Sz;
		

	int MarkerChange=0;//标志是否被修改
	CString stockcode=pFile->GetSymbol(nStock);//得到股票代码
	///读所有的日线数据到一安全数组
	CArray<TAI_SHAN_DATA,TAI_SHAN_DATA&>arrayKline;
	arrayKline.RemoveAll();
	Kline *pKline=NULL;
	int n=pFile->ReadKLine(nStock,pKline,-1);
	if(n<=0)
	{
		if(pKline!=NULL)
			delete[]pKline;
		return;
	}
	for(int i=0;i<n;i++)
	{
		arrayKline.Add(*(((TAI_SHAN_DATA*)pKline)+i));
	}
	if(pKline!=NULL)
		delete[]pKline;
    //进行数据清理
	if(m_Date)
	{
		int clearstart=atoi(m_clear_start.Format("%Y%m%d"));
		int clearend  =atoi(m_clear_end.Format("%Y%m%d"));
		
		int nSize=arrayKline.GetSize();
		while(nSize--)
		{
			CTime dt(arrayKline[nSize].nTime);
			int stocktime=atoi(dt.Format("%Y%m%d"));
			if(stocktime>=clearstart&&stocktime<=clearend)
			{
				arrayKline.RemoveAt(nSize);
				MarkerChange=1;
			}
		}
	}
	while(m_Invacode)//无效代码
	{
		/*int stocklen=lenstockcode.GetLength();
		if(stocklen!=4&&stocklen!=6)
		{

		}*/
		//by limingbo
		break;
	}
	while(m_Price)//价格其一为负
	{
		int nSize=arrayKline.GetSize();
		while(nSize--)
		{
			if(arrayKline[nSize].fOpen<0||
				arrayKline[nSize].fHigh<0||
				arrayKline[nSize].fLow<0||
				arrayKline[nSize].fClose<0)
				//arrayKline[nSize].fAmount<0||
				//arrayKline[nSize].fVolume<0)
			{
				arrayKline.RemoveAt(nSize);
				MarkerChange=1;
			}
		}
		break;
	}
	while(m_Invadate)//无效日期
	{
        int nSize=arrayKline.GetSize();
		while(nSize--)
		{
			CTime dt(arrayKline[nSize].nTime);
			int stotime=atoi(dt.Format("%Y%m%d"));
			CTime tm=CTime::GetCurrentTime();
			if(stotime<19880101||dt>tm)
			{
				arrayKline.RemoveAt(nSize);
				MarkerChange=1;
			}
		}
		break;
	}
	while(m_Continuous)//连续等值
	{
		for(int i=0;i<arrayKline.GetSize()-1;i++)
		{
			if(arrayKline[i].fOpen==arrayKline[i+1].fOpen &&
				arrayKline[i].fHigh==arrayKline[i+1].fHigh &&
				arrayKline[i].fLow==arrayKline[i+1].fLow &&
				arrayKline[i].fClose==arrayKline[i+1].fClose &&
				arrayKline[i].fAmount==arrayKline[i+1].fAmount &&
				arrayKline[i].fVolume==arrayKline[i+1].fVolume )
			{
				arrayKline.RemoveAt(i);
				i--;
				MarkerChange=1;
			}
		}
		break;
	}
	while(m_Chaos)//时间序列紊乱
	{
        for(int i=0;i<(arrayKline.GetSize()-1);i++)
		{
			CTime dt1(arrayKline[i].nTime);
			CTime dt2(arrayKline[i+1].nTime);
			if(atoi(dt2.Format("%Y%m%d"))<atoi(dt1.Format("%Y%m%d")))
			{
				arrayKline.RemoveAt(i+1);
				i--;
				MarkerChange=1;
			}
		}
		break;
	}
	while(m_Cjiaoliang)//成交量为负
	{
		int nSize=arrayKline.GetSize(); 
		while(nSize--)
		{
			if(arrayKline[nSize].fVolume<0)
			{
				arrayKline.RemoveAt(nSize);
				MarkerChange=1;
			}
		}
		break;
	}
	
	//重新写回到原数据文件中
	if(!MarkerChange)  
		return;
	CString s=pFile->GetSymbol(nStock);
	KLINE_SMALLHEAD klineSmallHead;
	int nIndexStock=pFile->GetKlineSmallHeader(s,&klineSmallHead);
	klineSmallHead.numKln=0;
	pFile->SetKlineSmallHeader(nIndexStock,&klineSmallHead);
	pFile->WriteKLine(s,(Kline*)arrayKline.GetData(),arrayKline.GetSize(),0);





}

void CClearData::OnSelchangezlfs() 
{
	// TODO: Add your control notification handler code here
    if(((CComboBox*)GetDlgItem(1042))->GetCurSel()==2)
	{
		return;
		//GetDlgItem(IDC_STATIC)->
		//((CButton*)GetDlgItem(1029))->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_Sta0)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_Sta1)->ShowWindow(SW_HIDE);
		GetDlgItem(1029)->ShowWindow(SW_HIDE);
		GetDlgItem(1024)->ShowWindow(SW_HIDE);
		GetDlgItem(1023)->ShowWindow(SW_HIDE);
		GetDlgItem(1027)->ShowWindow(SW_HIDE);
		GetDlgItem(1028)->ShowWindow(SW_HIDE);
		GetDlgItem(1026)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_Sta2)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_Sta3)->ShowWindow(SW_HIDE);
		GetDlgItem(1012)->ShowWindow(SW_HIDE);
		GetDlgItem(1018)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_LIST)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_PROGRESS1)->ShowWindow(SW_HIDE);
		GetDlgItem(IDCANCEL)->SetWindowText("删除");
		

		GetDlgItem(IDC_Sta4)->ShowWindow(SW_SHOW);
     	GetDlgItem(IDC_Sta5)->ShowWindow(SW_SHOW);
	    GetDlgItem(stocknumcode)->ShowWindow(SW_SHOW);
	    GetDlgItem(stocknumname)->ShowWindow(SW_SHOW);
	    //GetDlgItem(IDC_clear)->ShowWindow(SW_SHOW);
			

	}
	if(((CComboBox*)GetDlgItem(1042))->GetCurSel()==0)
	{
        GetDlgItem(IDC_Sta0)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_Sta1)->ShowWindow(SW_SHOW);
//		GetDlgItem(1029)->ShowWindow(SW_SHOW);
		GetDlgItem(1024)->ShowWindow(SW_SHOW);
		GetDlgItem(1023)->ShowWindow(SW_SHOW);
		GetDlgItem(1027)->ShowWindow(SW_SHOW);
		GetDlgItem(1028)->ShowWindow(SW_SHOW);
		GetDlgItem(1026)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_Sta2)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_Sta3)->ShowWindow(SW_SHOW);
		GetDlgItem(1012)->ShowWindow(SW_SHOW);
		GetDlgItem(1018)->ShowWindow(SW_SHOW);
        GetDlgItem(IDC_LIST)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_PROGRESS1)->ShowWindow(SW_SHOW);
		GetDlgItem(IDCANCEL)->SetWindowText("清除");

		GetDlgItem(IDC_Sta4)->ShowWindow(SW_HIDE);
     	GetDlgItem(IDC_Sta5)->ShowWindow(SW_HIDE);
	    GetDlgItem(stocknumcode)->ShowWindow(SW_HIDE);
	    GetDlgItem(stocknumname)->ShowWindow(SW_HIDE);
	    //GetDlgItem(IDC_clear)->ShowWindow(SW_HIDE);


	}
	if(((CComboBox*)GetDlgItem(1042))->GetCurSel()==1)
	{
		GetDlgItem(IDC_Sta0)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_Sta1)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_Sta2)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_Sta3)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_Sta4)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_Sta5)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_LIST)->ShowWindow(SW_HIDE);
		GetDlgItem(1029)->ShowWindow(SW_HIDE);
		GetDlgItem(1024)->ShowWindow(SW_HIDE);
		GetDlgItem(1023)->ShowWindow(SW_HIDE);
//		GetDlgItem(1025)->ShowWindow(SW_HIDE);
		GetDlgItem(1027)->ShowWindow(SW_HIDE);
		GetDlgItem(1028)->ShowWindow(SW_HIDE);
		GetDlgItem(1026)->ShowWindow(SW_HIDE);
		GetDlgItem(1012)->ShowWindow(SW_HIDE);
		GetDlgItem(1018)->ShowWindow(SW_HIDE);
		GetDlgItem(stocknumcode)->ShowWindow(SW_HIDE);
		GetDlgItem(stocknumname)->ShowWindow(SW_HIDE);
		//GetDlgItem(IDC_clear)->ShowWindow(SW_HIDE);
		GetDlgItem(IDCANCEL)->SetWindowText("整理");
		GetDlgItem(IDC_PROGRESS1)->ShowWindow(SW_SHOW);
	}
	
}

void CClearData::OnClearOneStock() 
{
	// TODO: Add your control notification handler code here
	::SetCurrentDirectory(pDoc->m_CurrentWorkDirectory);
	CString StockSymbol;//股票代码
	((CEdit*)GetDlgItem(stocknumcode))->GetWindowText(StockSymbol);
//	char *strLabel = StockSymbol.GetBuffer (0);
//	int stkKind = CFormularCompute::GetStockKind(strLabel)
	int Stolength=StockSymbol.GetLength();
	if(Stolength!=4&&Stolength!=6 ||m_nKind<0)
	{
        AfxMessageBox("无效代码",MB_ICONSTOP);
		return;
	}
    CTaiKlineFileKLine *pFile;
	if(((CComboBox*)GetDlgItem(1011))->GetCurSel()==0)	
	    pFile=CTaiKlineFileKLine::GetFilePointer(StockSymbol,m_nKind,true);
	else if(((CComboBox*)GetDlgItem(1011))->GetCurSel()==1)
		pFile=CTaiKlineFileKLine::GetFilePointer(StockSymbol,m_nKind,false);
	int i;
	pFile->LookUpIndexFromMap(StockSymbol,i);
	if(i==-1)
	{
		AfxMessageBox("没有该股票！",MB_ICONASTERISK);
		return;
	}
	if( AfxMessageBox("真的要删除吗？",MB_ICONQUESTION|MB_YESNO|MB_DEFBUTTON2)==IDNO )
		return;
	KLINE_SMALLHEAD klineSmallHead;
	int nIndexStock=pFile->GetKlineSmallHeader(StockSymbol,&klineSmallHead);
	klineSmallHead.numKln=0;
	pFile->SetKlineSmallHeader(nIndexStock,&klineSmallHead);
	AfxMessageBox("成功删除！");
	
}

void CClearData::OnChangeStockSymbol() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CPropertyPage::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	CString StockSymbol;
	GetDlgItem(stocknumcode)->GetWindowText(StockSymbol);
	int stolength=StockSymbol.GetLength();
	CString StockNo="            ";
	((CEdit*)GetDlgItem(stocknumname))->SetWindowText(StockNo.GetBuffer(12));
	if(stolength!=4&&stolength!=6)
			return;
	//得到文档指针
	::SetCurrentDirectory(pDoc->m_CurrentWorkDirectory);
	//查找股票名称
	
//	CTaiShanDoc * pDoc = CMainFrame::m_pDoc;
	CReportData* pdt;
	if(!pDoc->m_sharesInformation.Lookup(StockSymbol.GetBuffer (0),pdt,m_nKind))
		return ;
	if(pdt == NULL)
	{
		//((CEdit*)GetDlgItem(stocknumname))->SetWindowText(StockNo.GetBuffer(0));
		return;
	}
	CString sName = pdt->name ;
	((CEdit*)GetDlgItem(stocknumname))->SetWindowText(sName);
	

	//int pos=pDoc->m_namesymbol.GetSize();//
	/*for(int i=0;i<pos;i++)
	{
		if(pDoc->m_namesymbol.GetAt(i)->stocksymblo==StockSymbol)
		{
			StockName=pDoc->m_namesymbol.GetAt(i)->stockname;
			AfxMessageBox("test");
			break;
		}
		((CEdit*)GetDlgItem(stocknumname))->SetWindowText(StockName);
	}*/


	// TODO: Add your control notification handler code here
	
}

BOOL CClearData::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if( GetFocus() == GetDlgItem(stocknumcode) && pMsg->message==WM_CHAR && ( (pMsg->wParam>='0'&&pMsg->wParam<='9') || (pMsg->wParam>='A'&&pMsg->wParam<='z') )  )
	{
		CTaiKeyBoardAngelDlg KeyboardWizard;

		if(pMsg->wParam>='a' && pMsg->wParam<='z' )
			pMsg->wParam-=32;
			
		KeyboardWizard.input=pMsg->wParam;

		KeyboardWizard.ischar= ( pMsg->wParam>='0' && pMsg->wParam<='9' )? false:true;

		KeyboardWizard.DoModal();
	    
		if(KeyboardWizard.isresultstock )
		{
			CString szSymbol=KeyboardWizard.result;
			//  add begin46
			m_nKind=KeyboardWizard.m_stkKind;
			//  add end46
			CReportData* pDat;
		 	if ( (CMainFrame::m_pDoc)->m_sharesInformation.Lookup(szSymbol.GetBuffer(0) , pDat,m_nKind) )     //检测该股票是否已记录在内存模板中
				GetDlgItem(stocknumcode)->SetWindowText(szSymbol);
			else 
				GetDlgItem(stocknumcode)->SetWindowText("");
		}
		return TRUE;
	}
	
	return CPropertyPage::PreTranslateMessage(pMsg);
}

void CClearData::OnClickList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	    int ItemIndex=m_listfw.GetNextItem(-1,LVNI_SELECTED);
	if (ItemIndex==-1)
		return;
    LVITEM myitem;
	myitem.mask=LVIF_TEXT|LVIF_IMAGE;
	myitem.iSubItem=0;
	//m_listfw.SetImageList(pDoc->m_imagelist,LVSIL_SMALL);
	if(m_stockrange.GetAt(ItemIndex)==TRUE)
	{
		myitem.iImage=9;
		m_stockrange.SetAt(ItemIndex,FALSE);
	}
	else
	{
		myitem.iImage=6;
		m_stockrange.SetAt(ItemIndex,TRUE);
	}
    CString stockgroup[]={"上海","深圳","创业"};	
	myitem.iItem=ItemIndex;
	myitem.pszText=(LPTSTR)stockgroup[ItemIndex].GetBuffer(stockgroup[ItemIndex].GetLength());
	//
	m_listfw.SetItem(&myitem);
	m_listfw.Update(ItemIndex);

	*pResult = 0;
}

BOOL CClearData::OnHelpInfo(HELPINFO* pHelpInfo) 
{
	DoHtmlHelp(this);return TRUE;

//	HtmlHelp(m_hWnd,"stock.chm",HH_HELP_CONTEXT,IDD_CLEAR_DATA);
//	return TRUE;	
}

void CClearData::OnButton4To6() 
{
	// TODO: Add your control notification handler code here
	CTaiKlineFileKLine::Transfer4To6(false);
}
