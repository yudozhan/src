// ChuquanData.cpp : implementation file
//

#include "stdafx.h"
#include "WH.h"
#include "ChuquanData.h"
#include "KEYBRODEANGEL.h"
#include "mainfrm.h"
#include "CTaiChuQuanInDlg.h"
#include "CTaiChuQuanSetDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CChuquanData property page

IMPLEMENT_DYNCREATE(CChuquanData, CPropertyPage)

CChuquanData::CChuquanData() : CPropertyPage(CChuquanData::IDD)
{
	//{{AFX_DATA_INIT(CChuquanData)
	m_strStockCode = _T("");
	//}}AFX_DATA_INIT
	pDoc=CMainFrame::m_pDoc;
}

CChuquanData::~CChuquanData()
{
}

void CChuquanData::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CChuquanData)
	DDX_Control(pDX, 1022, m_button22);
	DDX_Control(pDX, 1012, m_button12);
	DDX_Control(pDX, 1007, m_button7);
	DDX_Control(pDX, 1006, m_button6);
	DDX_Control(pDX, 1004, m_button4);
	DDX_Control(pDX, IDC_LISTCQ, m_ctrlChuQuan);
	DDX_Text(pDX, 1008, m_strStockCode);
	DDV_MaxChars(pDX, m_strStockCode, 6);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CChuquanData, CPropertyPage)
	//{{AFX_MSG_MAP(CChuquanData)
	ON_EN_CHANGE(1008, OnChangeStockSymbol)
	ON_BN_CLICKED(1022, OnAddNew)
	ON_BN_CLICKED(1012, OnModifyCq)
	ON_BN_CLICKED(1004, OnDeleteInfo)
	ON_BN_CLICKED(1007, OnImport)
	ON_BN_CLICKED(1006, OnExport)
	ON_WM_HELPINFO()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CChuquanData message handlers

BOOL CChuquanData::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	// TODO: Add extra initialization here
	CRect Rect;
	LVCOLUMN listColumn;
	char * arColumn[5]={"除权日期","每10股送股","每10股配股","每股配股价", "每10股分红"};
	listColumn.mask=LVCF_FMT|LVCF_WIDTH|LVCF_TEXT|LVCF_SUBITEM;
	listColumn.fmt=LVCFMT_CENTER;
	listColumn.cx=76;
	for( int nColumn=0;nColumn<5;nColumn++)
	{
		listColumn.iSubItem=nColumn;
		listColumn.pszText=arColumn[nColumn];
		//::GetClientRect(
		/*m_ctrlChuQuan.GetClientRect(Rect);
		if(nColumn==0)
            m_ctrlChuQuan.InsertColumn(nColumn,arColumn[nColumn],LVCFMT_LEFT,Rect.Width()*1/4,nColumn);
		else
			m_ctrlChuQuan.InsertColumn(nColumn,arColumn[nColumn],LVCFMT_LEFT,Rect.Width()*3/16,nColumn);
		*/
		m_ctrlChuQuan.InsertColumn(nColumn, &listColumn);
	}
	//Set the listctrl's property
	//
	ListView_SetExtendedListViewStyle(  m_ctrlChuQuan.m_hWnd, LVS_EX_FULLROWSELECT);
//	COLORREF color=RGB(255,255,220);
 //   m_ctrlChuQuan.SetBkColor( color );
//	m_ctrlChuQuan.SetTextBkColor( color );

	//
	//Disable some buttons
	//
	EnableButton(FALSE);

	for(int i=0;i<80;i++)
		m_nChuQuanKind[i]=0;

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CChuquanData::EnableButton(BOOL bEnable)
{
    GetDlgItem(1012)->EnableWindow(bEnable);
	GetDlgItem(1022)->EnableWindow(bEnable);
	GetDlgItem(1004)->EnableWindow(bEnable);
}

BOOL CChuquanData::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
		if( GetFocus() == GetDlgItem(1008) && pMsg->message==WM_CHAR && ( (pMsg->wParam>='0'&&pMsg->wParam<='9') || (pMsg->wParam>='A'&&pMsg->wParam<='z') )  )
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
			// add begin46
			m_nKind=KeyboardWizard.m_stkKind;
			// add end46
			CReportData* pDat;
		 	if ( (CMainFrame::m_pDoc)->m_sharesInformation.Lookup(szSymbol.GetBuffer(0) , pDat,m_nKind) )     //检测该股票是否已记录在内存模板中
				GetDlgItem(1008)->SetWindowText(szSymbol);
			else 
				GetDlgItem(1008)->SetWindowText("");
		}
		return TRUE;
	}
	return CPropertyPage::PreTranslateMessage(pMsg);
}

void CChuquanData::OnChangeStockSymbol() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CPropertyPage::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	CString strStockCode;
	GetDlgItem(1008)->GetWindowText(strStockCode);         
	
	//judge if the stock code is validate
	if(strStockCode.GetLength()==4||strStockCode.GetLength()==6)
	{
		//Get the Document pointer
	//	CTaiShanDoc * pDoc=GetDocument();
		CReportData *Cdat=NULL;
        if(m_ctrlChuQuan.GetItemCount()>0)
			m_ctrlChuQuan.DeleteAllItems();
		if(pDoc->m_sharesInformation.Lookup(strStockCode.GetBuffer(0),Cdat,m_nKind))
		{
			//
			//First Enable three button (Add/Delete/Modify)
			//
			EnableButton(TRUE);
			//Show the stockname
			CString sName=Cdat->name;
			GetDlgItem(IDC_STANAME)->SetWindowText(sName);

			//
			//Get the stock's ChuQuan info and fill all info into the listctrl
			//
			Split *pSplit=NULL;//[80];
			//ZeroMemory(pSplit,sizeof(pSplit));
			int nTotleTimes=GetChuQuanInfo(strStockCode.GetBuffer(0),pSplit);
			if((nTotleTimes>0)&&(pSplit!=NULL))
			{
				POWER Power;
				for(int i=0;i<nTotleTimes;i++)
				{
					CTime time=pSplit[i].nTime;
					CString szTime=time.Format("%Y/%m/%d");

					Power.fAllocate=pSplit[i].Allocate;
					Power.fAllocatePrice=pSplit[i].AllocatePrice;
					Power.fDividend=pSplit[i].Bonus;
					Power.fGive=pSplit[i].Give;
					Power.nFlags=pSplit[i].nFlags;
					Power.nTime=pSplit[i].nTime;
					InsertItem(i,szTime,Power);//m_ctrlChuQuan.GetItemCount()
					m_nChuQuanKind[i]=Power.nFlags;
				}
			}		
		}
	}
	else //invalidate input stock code
	{
	//	m_strStaticName="";
		GetDlgItem(IDC_STANAME)->SetWindowText("");
	//	EnableButton(FALSE);
		if(m_ctrlChuQuan.GetItemCount()>0)
		{
			m_ctrlChuQuan.DeleteAllItems( );
			for(int i=0;i<80;i++)
				m_nChuQuanKind[i]=0;
		}
		EnableButton(FALSE);
	}
	
}

int CChuquanData::GetChuQuanInfo(CString strStockCode, PSplit &pSplit)
{
     return pDoc->m_sharesInformation.GetChuQuanInfo(strStockCode,m_nKind,pSplit);
}

void CChuquanData::InsertItem(int nItem, CString strNo, POWER& nPower)
{
    CString str;

	m_ctrlChuQuan.InsertItem (nItem, strNo);

	str.Format("%6.2f",nPower.fGive*10);
	m_ctrlChuQuan.SetItemText(nItem,1,str);

	str.Format("%6.2f",nPower.fAllocate*10);
	m_ctrlChuQuan.SetItemText(nItem,2,str);

	str.Format("%6.2f",nPower.fAllocatePrice);
	m_ctrlChuQuan.SetItemText(nItem,3,str);

	str.Format("%6.2f",(float)(nPower.fDividend*10));
	m_ctrlChuQuan.SetItemText(nItem,4,str);
}

void CChuquanData::OnAddNew() 
{
	// TODO: Add your control notification handler code here
	LVITEM	listItem;
	listItem.mask=LVIF_TEXT|LVIF_IMAGE;
	UpdateData(TRUE);
	CTaiChuQuanSetDlg AddChuquan;
	if(AddChuquan.DoModal()==IDOK)
	{
		int nTimes=m_ctrlChuQuan.GetItemCount();
		
		float fGive =AddChuquan.m_fGive/10.0f;
		float fAlloc=AddChuquan.m_fAlloc/10.0f;
		float fPrice=AddChuquan.m_fPrice;
		float fDivid=AddChuquan.m_fDivid/10.0f;
		
		if( fGive==0&&fAlloc==0&&fPrice==0&&fDivid==0)
			return;

		if( !IsAlreadyChuQuan(AddChuquan.m_timet) )
		{
			AfxMessageBox("同一天只能除权一次!",MB_ICONASTERISK);
			return;
		}

		CTime tm(AddChuquan.m_timet);
		CString str;

		str=tm.Format("%Y/%m/%d");
		POWER Power;
		Power.nTime=tm.GetTime();
		Power.fGive=fGive;
		Power.fAllocate=fAlloc;
		Power.fAllocatePrice=fPrice;
		Power.fDividend=fDivid;
		Power.nFlags=AddChuquan.m_kind;
		m_nChuQuanKind[nTimes]=Power.nFlags;
		//Add ChuQuan Information to the listctrl
		InsertItem(nTimes,str,Power);
		//Add ChuQuan information to Sjl ChuQuan data center
		AddChuQuanInfo(m_strStockCode,&Power);
	}

	
}

BOOL CChuquanData::IsAlreadyChuQuan(int nSeconds)
{
    CTime tm(nSeconds);
	CString DateToAdd=tm.Format("%Y/%m/%d");
	for( int i=0;i<this->m_ctrlChuQuan.GetItemCount();i++)
	{
		CString str=this->m_ctrlChuQuan.GetItemText(i,0);
		//if( DateToAdd == atoi(str.GetBuffer(0)))
		if( DateToAdd == str)
			return FALSE;
	}
	return TRUE;
}

void CChuquanData::AddChuQuanInfo(CString strStockCode,POWER *pPower)
{
    Split pSplit;
	memcpy(&pSplit,pPower,sizeof(Split));
	pDoc->m_sharesInformation.AddChuQuanInfo(strStockCode,m_nKind,&pSplit);
}

void CChuquanData::OnModifyCq() 
{
	// TODO: Add your control notification handler code here
	CTaiChuQuanSetDlg AddChuquan;
	AddChuquan.m_OperateID='M';
	UpdateData(TRUE);

	int nSel=m_ctrlChuQuan.GetNextItem(-1,LVIS_SELECTED);
	if( nSel<0 )
	{
		AfxMessageBox("请选择其中一项!",MB_ICONASTERISK);
		return;
	}

	CString strDate=m_ctrlChuQuan.GetItemText( nSel, 0);
	strDate=strDate.Left(4)+strDate.Mid(5,2)+strDate.Right(2);

	int year=atoi(strDate)/10000;
	int month=(atoi(strDate)/100)%100;
	int day=atoi(strDate)%100;
	CTime tm(year,month,day,8,30,0);

	CString szGive =m_ctrlChuQuan.GetItemText( nSel, 1);
	szGive.TrimLeft();
	CString szAlloc=m_ctrlChuQuan.GetItemText( nSel, 2);
	szAlloc.TrimLeft();
	CString szPrice=m_ctrlChuQuan.GetItemText( nSel, 3);
	szPrice.TrimLeft();
	CString szDivid=m_ctrlChuQuan.GetItemText( nSel, 4);
	szDivid.TrimLeft();

	float fGive =float(atof( szGive ));
	float fAlloc=float(atof( szAlloc));
	float fPrice=float(atof( szPrice));
	float fDivid=float(atof( szDivid));

	AddChuquan.m_timet=tm.GetTime();
	AddChuquan.m_fGive =fGive;
	AddChuquan.m_fAlloc=fAlloc;
	AddChuquan.m_fPrice=fPrice;
	AddChuquan.m_fDivid=fDivid;
    AddChuquan.m_kind=m_nChuQuanKind[nSel];

	if( AddChuquan.DoModal() == IDOK )
	{
		/*if( !IsAlreadyChuQuan(AddChuquan.m_timet) )
		{
			AfxMessageBox("同一天只能除权一次。",MB_ICONASTERISK);
			return;
		}*/
		CTime tmNew(AddChuquan.m_timet);
	    CString DateToCq=tmNew.Format("%Y/%m/%d");
	    for( int i=0;i<this->m_ctrlChuQuan.GetItemCount();i++)
		{
		    CString str=this->m_ctrlChuQuan.GetItemText(i,0);
		    if( DateToCq == str)
			 {
				 nSel=i;
				 break;
			 }
		}
		m_ctrlChuQuan.DeleteItem( nSel );
	
		CTime tm(AddChuquan.m_timet);
		POWER Power;
		Power.nTime=tm.GetTime();
		Power.fGive=AddChuquan.m_fGive/10.0f;
		Power.fAllocate=AddChuquan.m_fAlloc/10.0f;
		Power.fAllocatePrice=AddChuquan.m_fPrice;
		Power.fDividend=AddChuquan.m_fDivid/10.0f;
		Power.nFlags=AddChuquan.m_kind;
		m_nChuQuanKind[nSel]=Power.nFlags;

		if( Power.fGive==0&&Power.fGive==0&&Power.fAllocate==0&&Power.fDividend==0)
			return;

		CString str;
		str=tm.Format("%Y/%m/%d");
		InsertItem(nSel,str,Power);
		ModifyChuQuanInfo(m_strStockCode,nSel,&Power);
	}
	
}

void CChuquanData::ModifyChuQuanInfo(CString strStockCode, int nWhichItem, POWER *pPower)
{
    Split pSplit;
	memcpy(&pSplit,pPower,sizeof(Split));
	pDoc->m_sharesInformation.ModifyChuQuanInfo(strStockCode,nWhichItem,&pSplit,m_nKind);
}

void CChuquanData::OnDeleteInfo() 
{
	// TODO: Add your control notification handler code here
		int Index=m_ctrlChuQuan.GetNextItem(-1,LVIS_SELECTED);
	if( Index<0 )
	{
		AfxMessageBox("请选择其中一项!",MB_ICONASTERISK);
		return;
	}

	if( AfxMessageBox("要删除吗？",MB_ICONQUESTION|MB_YESNO|MB_DEFBUTTON2)==IDNO )
		return;
    UpdateData(TRUE);
	m_nChuQuanKind[Index]=0;
    //Delete ChuQuan info from Sjl Data center
	DeleteChuQuanInfo(m_strStockCode,Index);
	//Delete ChuQuan info from the listctrl
	m_ctrlChuQuan.DeleteItem( Index );

	
}

void CChuquanData::DeleteChuQuanInfo(CString strStockCode, int nWhichItem)
{
    pDoc->m_sharesInformation.DeleteChuQuanInfo(strStockCode,nWhichItem,m_nKind);
}

void CChuquanData::OnImport() 
{
	// TODO: Add your control notification handler code here
//	CTaiChuQuanInDlg dlg;
//	dlg.DoModal();
	UpdateData(TRUE);

	FXJPOWER power_fxj;
//	POWER Power;
//	char strStockCode[8]; 
	int nSplitMask=0;
	int nTotalCount=0;
	int nEachChuQuanNo=0;
	CString m_InFile = "";
	CString DefExt="*.chu,*.pwr";
	CString FileName="";
	CString Filter="系统数据格式(*.chu) 分析家格式(*.pwr) |*.chu;*.pwr||";

	CFileDialog bOpenFileDialog(TRUE,(LPCSTR)DefExt,(LPCTSTR)FileName,
		OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT|OFN_NOCHANGEDIR,(LPCSTR)Filter,this);
	if( bOpenFileDialog.DoModal() !=IDOK )
		return ;
	m_InFile=bOpenFileDialog.GetPathName();

	m_InFile.MakeLower ();

	CFile InFile;
	if(!(InFile.Open(m_InFile,CFile::modeRead,NULL)))
	{
		MessageBox("引入除权文件不可读!","警告",MB_OK|MB_ICONEXCLAMATION);
		return;
	}
	CString StockSymbol;      //股票代码
	int Marker;               //隔离标记
	//short int nCount=0;

	Split pSplit[80];
	BOOL IsSucc;
	if(m_InFile.Right (4) != ".pwr")
	{
		InFile.Close();
		IsSucc=InstallPowerData(m_InFile);
		
	}
	//从分析家的除权数据文件导入
	else 
	{
		{
			CString StockCode;
			WORD nFlag=0;
			InFile.Read(&nFlag,2);
			if( nFlag!=51250 )
			{
			   AfxMessageBox("不是分析家的除权数据格式!");
			   InFile.Close();
			   IsSucc=FALSE;
			   return;
			}
			InFile.Seek(8,CFile::begin);
			nEachChuQuanNo=0;
        	BOOL bFirstTime=TRUE;
			bool bSh = true;
		    while( InFile.Read(&Marker,4) == 4 )
			{
				if( Marker == 0xFFFFFFFF )
				{
					if(bFirstTime==TRUE)
					   bFirstTime=FALSE;  
					else
					{
						//	将上次pSplit写进内存；
						int stkKind = 0;
						if(bSh == true)
							stkKind = pDoc->m_sharesInformation.GetStockKind(SH_MARKET_EX,StockSymbol.GetBuffer(0));
						else
							stkKind = pDoc->m_sharesInformation.GetStockKind(SZ_MARKET_EX,StockSymbol.GetBuffer(0));
						if(!pDoc->m_sharesInformation.ImportChuQuanInfo(StockSymbol,pSplit,nEachChuQuanNo,stkKind))
						{
							return;
						}
					}
		        		//清空PSPLIT；
						//读出除权数据；写进pSplit；
				   ::ZeroMemory(pSplit,80*sizeof(Split));
				   nEachChuQuanNo=0;
				   InFile.Read(StockSymbol.GetBuffer(12),12);
				   StockSymbol.ReleaseBuffer(); 
				   InFile.Seek(4,CFile::current);

				   //lmb add
				   StockSymbol.MakeLower ();
				   bSh = false;
				   if(StockSymbol.Left (2) == "sh") bSh = true;

				   if( StockSymbol.GetLength() == 8 )
                      StockSymbol=StockSymbol.Right(6);
				   else if(StockSymbol.GetLength() == 6)
					  StockSymbol=StockSymbol.Right(4);
				   //InFile.Seek(-8,CFile::current);
				   InFile.Read(&power_fxj, sizeof(FXJPOWER));
				   //InFile.Read(&(pSplit[nEachChuQuanNo].Free),8); 
				   //InFile.Read(&(pSplit[nEachChuQuanNo].nFlags),4);
				   pSplit[nEachChuQuanNo].nTime=power_fxj.nTime;
				   pSplit[nEachChuQuanNo].Give=power_fxj.fGive;
				   pSplit[nEachChuQuanNo].Allocate=power_fxj.fAlloc;
				   pSplit[nEachChuQuanNo].AllocatePrice=power_fxj.fAllocPrice;
				   pSplit[nEachChuQuanNo].Bonus=power_fxj.fDividend;
				   nEachChuQuanNo++;
				}
				else 
				{
					//指针后退4；写进pSplit；
					InFile.Seek(-4,CFile::current);
					InFile.Read(&power_fxj, sizeof(FXJPOWER));
					//InFile.Read(&(pSplit[nEachChuQuanNo].Free),8); 
					//InFile.Read(&(pSplit[nEachChuQuanNo].nFlags),4);
					pSplit[nEachChuQuanNo].nTime=power_fxj.nTime;
					pSplit[nEachChuQuanNo].Give=power_fxj.fGive;
					pSplit[nEachChuQuanNo].Allocate=power_fxj.fAlloc;
					pSplit[nEachChuQuanNo].AllocatePrice=power_fxj.fAllocPrice;
					pSplit[nEachChuQuanNo].Bonus=power_fxj.fDividend;
					nEachChuQuanNo++;
				}
			}
		}
		   //将最后一次pSplit写进内存；
		   InFile.Close();
		   IsSucc=TRUE;
	}
    //	m_Progress.ShowWindow(SW_HIDE);
	CString sMsg = "引入失败!";
	if(IsSucc)
	   sMsg = "引入完毕!";
	AfxMessageBox(sMsg);
	UpdateData(FALSE);
	
	
}

void CChuquanData::OnExport() 
{
	// TODO: Add your control notification handler code here
	CString DefExt="*.chu";
	CString FileName="ChuQuan.chu";
	CString FilePathName;
	CString Filter= "系统数据格式(*.chu)|*.chu||";
	CFileDialog bSaveFileDialog(FALSE,(LPCSTR)DefExt,(LPCTSTR)FileName,
		OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT|OFN_NOCHANGEDIR,(LPCSTR)Filter,this);
	if( bSaveFileDialog.DoModal() ==IDOK )
	{
		FilePathName=bSaveFileDialog.GetPathName();
        OutChuQuanData(FilePathName);
	}

}

void CChuquanData::OutChuQuanData(CString FilePath)
{
    ::SetCurrentDirectory(pDoc->m_CurrentWorkDirectory);

	UpdateData(TRUE);
	CFile OutFile;
	if(!OutFile.Open(FilePath,CFile::modeCreate|CFile::modeWrite,NULL))
	{
		MessageBox("导出除权信息失败!","警告",MB_OK|MB_ICONEXCLAMATION);
		return;
	}
	int nHeaderFlag=-1;
	int nTotalCount=0;
	POWER Power;
	struct POWER_1th       //共32个字节
	{
		BYTE  byFree[8];  //预留8个字节
		int   nFlags;     //除权的种类，0为“先送后配”，1为“先配后送”，2为“股本不变”
		int   nTime;
		float fGive;      //song
		float fAllocate;
		float fAllocatePrice;//
		float fDividend;
	};
	POWER_1th Power_1th;
	int nSplitMask=-1;
	//
	//first initialize the file header
	//
    //	OutFile.Write(&nHeaderFlag,sizeof(int));
    //	OutFile.Write(&nTotalCount,sizeof(int));
	int FileId=FILEID22;
	OutFile.Write(&FileId,4);
	OutFile.Seek(8,CFile::begin);
	
	//
	//Get all stock Chuquan info and write to the outfile
	//
    //	m_Progress.ShowWindow(SW_SHOW);
    //  m_Progress.SetRange32(0,pDoc->m_ZqdmToGpHqxx.GetCount());
	for(int nStockType=0;nStockType<STOCKTYPENUM;nStockType++)
	{
		for(int i=0;i<pDoc->m_sharesInformation.GetStockTypeCount(nStockType);i++)
		{
		//	m_Progress.StepIt();
			CReportData *pDat1=NULL;
			pDoc->m_sharesInformation.GetStockItem(nStockType,i,pDat1);
			if(pDat1)
			{
				if(pDat1->pBaseInfo)
				{
					if(pDat1->pBaseInfo->NumSplit>0)
					{
						nTotalCount++;
						OutFile.Write(&nSplitMask,sizeof(int));
						OutFile.Write(&(pDat1->id),8*sizeof(char));	
						// add begin46
						int a =pDat1->kind;
						OutFile.Write(&a,4);
						// add end46
						for(int j=0;j<pDat1->pBaseInfo->NumSplit;j++)
						{
						    memcpy(&Power,&(pDat1->
							pBaseInfo->m_Split[j]),sizeof(POWER));
							Power_1th.fAllocate=Power.fAllocate;
                            Power_1th.fAllocatePrice=Power.fAllocatePrice;
							Power_1th.fDividend=Power.fDividend;
							Power_1th.fGive=Power.fGive;
							Power_1th.nTime=Power.nTime;
							OutFile.Write(&Power_1th,sizeof(POWER_1th));
						}
					}
				}
			}
		}
	}
	//文件的5-8字节为导出股票的总数。
	//if(nTotalCount>0)
	//{
    //  OutFile.Seek(4, CFile::begin);
    //  OutFile.Write(&nTotalCount, sizeof(int));
    
    //	m_Progress.ShowWindow(SW_HIDE);
    //	CString str;
    //	str.Format("%d", nTotalCount );
    //	m_FormName="导出完毕, 共导出"+str+"只股票！"+"\n位于"+m_OutFile;
    //	UpdateData(FALSE);
    OutFile.Close();
    //	}
    if(nTotalCount==0)
	 {
        AfxMessageBox("没有除权数据!",MB_ICONASTERISK);
		OutFile.Remove(FilePath);
	}
	else
		AfxMessageBox("导出完毕!",MB_ICONASTERISK);
}

BOOL CChuquanData::OnHelpInfo(HELPINFO* pHelpInfo) 
{
	DoHtmlHelp(this);return TRUE;

//	HtmlHelp(m_hWnd,"stock.chm",HH_HELP_CONTEXT,IDD_POWER2);
//	return TRUE;	
}

BOOL CChuquanData::InstallPowerData(CString FilePath)
{
	CFile InFile;
	if(!InFile.Open(FilePath,CFile::modeRead))
		return FALSE;
    //CString StockSymbol;      //股票代码
	int Marker;               //隔离标记
	//POWER Power;
	int nEachChuQuanNo=0;
	char strStockCode[8]; 
	int nKind;
	//BYTE nKind;
    Split pSplit[80];
    int nFlag=0;
	InFile.Read(&nFlag,4);
    if(nFlag!=FILEID22)
	{
		AfxMessageBox("不是"+g_strCompanyName+"除权数据格式!");
		InFile.Close();
		return FALSE;
	}
	::ZeroMemory(pSplit,80*sizeof(Split));
	InFile.Seek(8,CFile::begin);
	BOOL bFirstTime=TRUE;
	//pDoc=CMainFrame::m_pDoc;
	while( InFile.Read(&Marker,4) == 4 )
	{
		if( Marker == 0xFFFFFFFF )
		{
			if(bFirstTime==TRUE)
                bFirstTime=FALSE;  
            else//	将上次pSplit写进内存；
	 	        CMainFrame::m_pDoc->m_sharesInformation.ImportChuQuanInfo(strStockCode,pSplit,nEachChuQuanNo,nKind);
			//	if(!pDoc->m_sharesInformation.ImportChuQuanInfo(strStockCode,pSplit,nEachChuQuanNo))
			//	{
		      //	return;
			  //}
				
			//清空PSPLIT；
			//读出除权数据；写进pSplit；
			::ZeroMemory(pSplit,80*sizeof(Split));
			nEachChuQuanNo=0;
            InFile.Read(strStockCode,8*sizeof(char));
			InFile.Read(&nKind,4);
            InFile.Seek(8,CFile::current);			
			//InFile.Read(&(pSplit[nEachChuQuanNo].Free),8); 
			InFile.Read(&(pSplit[nEachChuQuanNo].nFlags),4);
			InFile.Read(&(pSplit[nEachChuQuanNo].nTime),4);
			InFile.Read(&(pSplit[nEachChuQuanNo].Give),4);
			InFile.Read(&(pSplit[nEachChuQuanNo].Allocate),4);
			InFile.Read(&(pSplit[nEachChuQuanNo].AllocatePrice),4);
			InFile.Read(&(pSplit[nEachChuQuanNo].Bonus),4);
			nEachChuQuanNo++;
			}
		    else 
			{
				//指针后退4；写进pSplit；
			InFile.Seek(-4,CFile::current);			
			InFile.Seek(8,CFile::current);			
			//InFile.Read(&(pSplit[nEachChuQuanNo].Free),8); 
			InFile.Read(&(pSplit[nEachChuQuanNo].nFlags),4);
			InFile.Read(&(pSplit[nEachChuQuanNo].nTime),4);
			InFile.Read(&(pSplit[nEachChuQuanNo].Give),4);
			InFile.Read(&(pSplit[nEachChuQuanNo].Allocate),4);
			InFile.Read(&(pSplit[nEachChuQuanNo].AllocatePrice),4);
			InFile.Read(&(pSplit[nEachChuQuanNo].Bonus),4);
            nEachChuQuanNo++;
			}
		}
		//将最后一次pSplit写进内存；
		CMainFrame::m_pDoc->m_sharesInformation.ImportChuQuanInfo(strStockCode,pSplit,nEachChuQuanNo,nKind);  
       InFile.Close();
	   return TRUE;

}
