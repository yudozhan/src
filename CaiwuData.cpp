// CaiwuData.cpp : implementation file
//

#include "stdafx.h"
#include "WH.h"
#include "CaiwuData.h"
#include "KEYBRODEANGEL.h"
#include "mainfrm.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCaiwuData property page

IMPLEMENT_DYNCREATE(CCaiwuData, CPropertyPage)

CCaiwuData::CCaiwuData() : CPropertyPage(CCaiwuData::IDD)
{
	//{{AFX_DATA_INIT(CCaiwuData)
	m_ShowStatus = _T("");
	//}}AFX_DATA_INIT
	pDoc=CMainFrame::m_pDoc;
}

CCaiwuData::~CCaiwuData()
{
}

void CCaiwuData::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCaiwuData)
	DDX_Control(pDX, IDC_ZDTQ, m_zdtq);
	DDX_Control(pDX, 1006, m_button6);
	DDX_Control(pDX, 1005, m_button5);
	DDX_Control(pDX, IDC_PROGRESS1, m_progress);
	DDX_Control(pDX, IDC_LISTCW, m_listcw);
	DDX_Text(pDX, IDC_SHOWSTA, m_ShowStatus);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCaiwuData, CPropertyPage)
	//{{AFX_MSG_MAP(CCaiwuData)
	ON_EN_CHANGE(1008, OnChangeStockSymbol)
	ON_BN_CLICKED(1006, OnExport)
	ON_BN_CLICKED(1005, OnImport)
	ON_BN_CLICKED(IDC_ZDTQ, OnZdtq)
	ON_WM_HELPINFO()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCaiwuData message handlers

BOOL CCaiwuData::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	// TODO: Add extra initialization here
	LVCOLUMN listColumn;
	listColumn.mask=LVCF_FMT|LVCF_WIDTH|LVCF_TEXT|LVCF_SUBITEM;
	listColumn.fmt=LVCFMT_CENTER;
	CRect Rect;
    m_listcw.GetClientRect(Rect);
	/*CString arColumn[]={"国家股(万股)","发起人法人股(万股)"
	,"法人股(万股)","职工股(万股)","公众A股(万股)"
	,"流通A股(万股)","Ｈ股(万股)","B股(万股)","转配股(万股)"
    ,"总资产(万元)","流动资产(万元)","长期投资(万元)","固定资产(万元)"
	,"无形资产(万元)","流动负债(万元)","长期负债(万元)","股东权益(万元)"
	,"资本公积金(万元)","盈余公积金(万元)","每股净值(元)","股东权益比率(%)"
	,"每股公积金(元)","主营业务收入(万元)","主营业务利润(万元)"
	,"其它业务利润(万元)","利润总额(万元)","净利润(万元)"
	,"未分配利润(万元)","每股收益(元)","净资产收益率(%)"
	,"每股未分配利润(元)","每股净资产(元)"};*/
	CString arColumn[]={"名称","数值"};
	int nColumn;
	for(nColumn=0;nColumn<2;nColumn++)
		m_listcw.InsertColumn(nColumn,arColumn[nColumn],LVCFMT_CENTER,Rect.Width()*1/2,nColumn);
	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CCaiwuData::PreTranslateMessage(MSG* pMsg) 
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
			m_nKind=KeyboardWizard.m_stkKind;
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

void CCaiwuData::OnChangeStockSymbol() 
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
        if(m_listcw.GetItemCount()>0)
			m_listcw.DeleteAllItems();

		pDoc->m_sharesInformation.Lookup(strStockCode.GetBuffer(0),Cdat,m_nKind);
		if(Cdat==NULL)
		{
			GetDlgItem(IDC_STANAME)->SetWindowText("");
			return;
		}
		//Show the stockname
		CString sName=Cdat->name;
		GetDlgItem(IDC_STANAME)->SetWindowText(sName);
       
	    BASEINFO *pBase=Cdat->pBaseInfo;
		if(pBase==NULL)
			return;
		CString s ;
		float* pf = &(pBase->zgb);
		s.LoadString (IDS_ZGB);
		for(int i=0;i<33;i++)
		{
			s.LoadString (IDS_ZGB+i);
			m_listcw.InsertItem(i,s);
			s.Format ("%15.2f",(float)pf[i]);
			m_listcw.SetItemText(i,1,s);
		}
		
	}
	else //invalidate input stock code
	{//	m_strStaticName="";
		GetDlgItem(IDC_STANAME)->SetWindowText("");
		if(m_listcw.GetItemCount()>0)
			m_listcw.DeleteAllItems();
	}
}

void CCaiwuData::OnExport() 
{
	// TODO: Add your control notification handler code here
	CString DefExt="*.cai";
	CString FileName="CaiWu.cai";
	CString FilePathName;
	CString Filter="系统数据格式(*.cai)|*.cai||";
	CFileDialog bSaveFileDialog(FALSE,(LPCSTR)DefExt,(LPCTSTR)FileName,
		OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT|OFN_NOCHANGEDIR,(LPCSTR)Filter,this);
	if( bSaveFileDialog.DoModal() ==IDOK )
	{
		FilePathName=bSaveFileDialog.GetPathName();
        OutCaiWuData(FilePathName);
	}
//	UpdateData(FALSE);
	//CTaiShanDoc * pDoc=GetDocument();
//	::SetCurrentDirectory(pDoc->m_CurrentWorkDirectory);


	
}

void CCaiwuData::OutCaiWuData(CString PathFileName)
{
    m_progress.ShowWindow(SW_HIDE);
	((CComboBox*)GetDlgItem(IDC_SHOWSTA))->ShowWindow(SW_SHOW);
	
	::SetCurrentDirectory(pDoc->m_CurrentWorkDirectory);

	UpdateData(TRUE);
	CFile OutFile;
	if(!OutFile.Open(PathFileName,CFile::modeCreate|CFile::modeWrite,NULL))
	{
		MessageBox("导出财务信息失败!","警告",MB_OK|MB_ICONEXCLAMATION);
		return;
	}
	int nHeaderFlag=65792509;
	char nVer[5]="v2.0";
	int nTotalCount=0;
	//CString StockSymbol;
	//POWER Power;
	//int nSplitMask=-1;
	//
	//first initialize the file header
	OutFile.Write(&nHeaderFlag,sizeof(int));
	OutFile.Write(nVer,4);
	OutFile.Write(&nTotalCount,sizeof(int));
	OutFile.Seek(16,CFile::begin);
	//m_progress.SetRange32(0,8);
	m_ShowStatus="正在导出...";
	UpdateData(FALSE);
	//Get all stock Caiwu info and write to the outfile
	CReportData *pDat1=NULL;
	for(int nStockType=0;nStockType < STOCKTYPENUM;nStockType++)
	{
		for(int i=0;i<pDoc->m_sharesInformation.GetStockTypeCount(nStockType);i++)
		{
			pDoc->m_sharesInformation.GetStockItem(nStockType,i,pDat1);
			if(pDat1 && pDat1->pBaseInfo )
			{
	            OutFile.Write(pDat1->pBaseInfo,sizeof(BASEINFO)-sizeof(pDat1->pBaseInfo->m_Split)-12);
				nTotalCount++;
			}
		}	
	  //  m_progress.OffsetPos(1);		
	}		
				/*	if(pDat1->pBaseInfo->NumSplit>0)
					{
						nTotalCount++;
						OutFile.Write(&nSplitMask,sizeof(int));
						OutFile.Write(&(pDat1->id),8*sizeof(char));
						for(int j=0;j<pDat1->pBaseInfo->NumSplit;j++)
						{
							memcpy(&Power,&(pDat1->
								pBaseInfo->m_Split[j]),sizeof(POWER));
							OutFile.Write(&Power,sizeof(POWER));
						}
					}
				}
			}
		}
	}*/
	//文件的9-12字节为导出股票的总数。
	if(nTotalCount>0)
	{
	    OutFile.Seek(8, CFile::begin);
	    OutFile.Write(&nTotalCount, sizeof(int));
	    OutFile.Close();
//	m_Progress.ShowWindow(SW_HIDE);
//	CString str;
//	str.Format("%d", nTotalCount );
//	m_FormName="导出完毕, 共导出"+str+"只股票！"+"\n位于"+m_OutFile;
//	UpdateData(FALSE);
	//	m_progress.SetPos(8);
	//	m_progress.SetPos(0);
	  //  AfxMessageBox("导出完毕！");
		m_ShowStatus="导出完毕!";
		
	}
	else
	{
        //AfxMessageBox("没有财务数据!",MB_ICONASTERISK);
		OutFile.Close();
		m_ShowStatus="没有财务数据!";
		OutFile.Remove(PathFileName);
	}
	UpdateData(FALSE);
}

void CCaiwuData::OnImport() 
{
	// TODO: Add your control notification handler code here
    CString FilePathName;
	CString DefExt="*.cai,*.fin";
	CString FileName="";
	CString Filter="系统数据格式(*.cai) 分析家格式(*.fin) |*.cai;*.fin||";

	CFileDialog bOpenFileDialog(TRUE,(LPCSTR)DefExt,(LPCTSTR)FileName,
		OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT|OFN_NOCHANGEDIR,(LPCSTR)Filter,this);
	if( bOpenFileDialog.DoModal() ==IDOK )
	{
		m_progress.ShowWindow(SW_HIDE);
		((CComboBox*)GetDlgItem(IDC_SHOWSTA))->ShowWindow(SW_SHOW);
		m_ShowStatus="正在导入财务数据...";
		FilePathName=bOpenFileDialog.GetPathName();
		UpdateData(FALSE);
		BOOL IsSucc=ImportCwData(FilePathName);
		//m_progress.SetPos(100);
        //m_progress.SetPos(0);
		//AfxMessageBox("导入完毕！");
		if(IsSucc)
		   m_ShowStatus="导入完毕!";
		else 
           m_ShowStatus="导入失败!";  
		UpdateData(FALSE);
	}

	UpdateData(FALSE);
//	CTaiShanDoc * pDoc=GetDocument();
//	::SetCurrentDirectory(pDoc->m_CurrentWorkDirectory);
	
	
}

BOOL CCaiwuData::ImportCwData(CString FileName)
{
	//sunquan begin
	if(FileName.GetLength ()<5) return FALSE;

	CString sFxj = FileName;
	sFxj.MakeLower ();
	sFxj = sFxj.Right(4);

	if(sFxj == ".fin")
	{
		CFile InFile;
		if(!(InFile.Open(FileName,CFile::modeRead,NULL)))
		{
			AfxMessageBox("引入财务文件不可读!");
			return FALSE;
		}
		int nFlag;
		InFile.Read(&nFlag,4);
		char nVer[5];
		int nTotal;
		InFile.Read(&nTotal,4);
		InFile.Seek(8,CFile::begin);
 
		BASEINFO baseinfo;
		BASEINFO * pBase = &baseinfo;
		char ch[16];
		float fVal[50];
		int nVal[50];
		while(1)
		{
		   memset(&baseinfo,0,sizeof(BASEINFO));
		   if(InFile.Read (ch,4)!=4) break;
		   ch[2] = '\0';
		   CString ss = ch;
		   if(InFile.Read (ch,8)!=8) break;
		   ch[6] = '\0';
		   CString ss2 = ch;
		   int nKind = 0;
		   if(ss == "SH")
			   nKind = CMainFrame::m_pDoc->m_sharesInformation.GetStockKind (SH_MARKET_EX,ch);
		   else
			   nKind = CMainFrame::m_pDoc->m_sharesInformation.GetStockKind (SZ_MARKET_EX,ch);

		   CString s = CMainFrame::m_pDoc->GetStockKindString(nKind);
		   s+=ch;
		   CString s640 = ch;
		   strcpy(baseinfo.Symbol ,s);

		   if(InFile.Read (ch,2)!=2) break;
		   float f2 = 0;
		   if(InFile.Read (&f2,4)!=4) break;
		   if(InFile.Read (fVal,148)!=148) break;
		   pBase->zgb = fVal[0];
		   pBase->gjg = fVal[1];
		   pBase->fqrfrg = fVal[2];
		   pBase->frg = fVal[3];
		   pBase->Bg = fVal[4];

		   pBase->Hg = fVal[5];
		   pBase->ltAg = fVal[6];
		   pBase->zgg = fVal[7];
//		   pBase->zpg = fVal[8];//A2转配股
		   pBase->zzc = fVal[9]/10;//10

		   pBase->ldzc = fVal[10]/10;
		   pBase->gdzc = fVal[11]/10;
		   pBase->wxzc = fVal[12]/10;
		   pBase->cqtz = fVal[13]/10;
		   pBase->ldfz = fVal[14]/10;

		   pBase->cqfz = fVal[15]/10;
		   pBase->zbgjj = fVal[16]/10;
		   pBase->mggjj = fVal[17];
		   pBase->ggqy = fVal[18]/10;
		   pBase->zyywsr = fVal[19]/10;//20

		   pBase->zyywlr = fVal[20]/10;
		   pBase->qtywlr = fVal[21]/10;
		   pBase->zyywlr = fVal[22]/10;
//		   pBase->tz = fVal[23];
//		   pBase->zgb = fVal[24];

//		   pBase->zgb = fVal[25];
//		   pBase->zgb = fVal[26];
		   pBase->lrze = fVal[27]/10;
//		   pBase->jlr = fVal[28]/10;
		   pBase->jlr = fVal[29]/10;//30

		   pBase->wfplr = fVal[30]/10;
		   pBase->mgwfplr  = fVal[31];
		   pBase->mgsy = fVal[32];//mgsy
		   pBase->mgjzc = fVal[33];//mgjzc
//		   pBase-> = fVal[34];

		   pBase->gdqybl  = fVal[35];
		   pBase->jzcsyl = fVal[36];
//		   pBase->zgb = fVal[37];

		   if(nKind!=SHZS && nKind!=SZZS)//指数没有基本资料
			CMainFrame::m_pDoc->m_sharesInformation.ImportCaiwuInfo(&baseinfo);
		}
		InFile.Close();
		;
		return TRUE;
	}

    CFile InFile;
	if(!(InFile.Open(FileName,CFile::modeRead,NULL)))
	{
		AfxMessageBox("引入财务文件不可读!");
		return FALSE;
	}
	int nFlag;
	InFile.Read(&nFlag,4);
	if(nFlag!=65792509)
	{
		AfxMessageBox("不是"+g_strCompanyName+"财务数据文件!");
		InFile.Close();
		return FALSE;
	}
	char nVer[5];
	//InFile.Seek(4,CFile::begin);
	InFile.Read(nVer,4);
	nVer[4]='\0';
	if(strcmp(nVer,"v2.0")!=0)
	{
		AfxMessageBox("财务数据版本不符!");
		InFile.Close();
		return FALSE;
	}
	int nTotal;
	//char StockSymbol[8];
	InFile.Read(&nTotal,4);
	InFile.Seek(16,CFile::begin);
 
	//CReportData *pDat1=NULL;
    BASEINFO baseinfo;
	for(int i=0;i<nTotal;i++)
	{
	   InFile.Read(&baseinfo,sizeof(BASEINFO)-sizeof(baseinfo.m_Split)-12);
	   CMainFrame::m_pDoc->m_sharesInformation.ImportCaiwuInfo(&baseinfo);
	   //if(pDat1&&pDat1->pBaseInfo)
		 // memcpy(pDat1->pBaseInfo,&baseinfo,sizeof(BASEINFO)-sizeof(baseinfo.m_Split)-12);
	}
	InFile.Close();
	return TRUE;
}

void CCaiwuData::OnZdtq() 
{
	// TODO: Add your control notification handler code here
	long num=pDoc->m_sharesInformation.GetCount();
	m_progress.ShowWindow(SW_SHOW);
    ((CComboBox*)GetDlgItem(IDC_SHOWSTA))->ShowWindow(SW_HIDE);
	m_progress.SetRange32(0,num);
	for(int i=0;i<STOCKTYPENUM;i++)
	{
		if(i==SHZS||i==SZZS)
			continue;
		int temp=pDoc->m_sharesInformation.GetStockTypeCount(i);
		for(int j=0;j<temp;j++)
		{
			CString str;
			CReportData * Cdat;
			pDoc->m_sharesInformation.GetStockItem(i, j, Cdat);
			if(Cdat==NULL)
				continue;
			pDoc->m_sharesInformation.ReadBaseInfoData(Cdat);
			m_progress.OffsetPos(1);
		}
	}
	m_progress.SetPos(num);
	m_progress.SetPos(0);
	//AfxMessageBox("财务数据提取完毕");
    m_progress.ShowWindow(SW_HIDE);
	((CComboBox*)GetDlgItem(IDC_SHOWSTA))->ShowWindow(SW_SHOW);
	m_ShowStatus="财务数据提取完毕!";
	UpdateData(FALSE);
}

BOOL CCaiwuData::OnHelpInfo(HELPINFO* pHelpInfo) 
{
	DoHtmlHelp(this);return TRUE;

//	HtmlHelp(m_hWnd,"stock.chm",HH_HELP_CONTEXT,IDD_CAIWUINOUT);
//	return TRUE;	
}
