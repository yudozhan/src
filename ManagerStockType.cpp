// ManagerStockType.cpp : implementation file
//

#include "stdafx.h"
#include <share.h>
#include  <io.h>
#include <direct.h>
#include "WH.h"
#include "mainfrm.h"
#include "cellRange.h"
#include "PageWnd.h"

#include "CTaiShanReportView.h"
#include "managerchoose.h"
#include "modifystocktype.h"
#include "ManagerStockType.h"
#include "BlockTypeNameDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSuperviseSharesBlock dialog

///////////////////////////////////////////////////////////////////////////////////////////

// 文件名：   ManagerStockType.h 
//            ManagerStockType.cpp
//
// 创建者：   黄玉兔

// 创建时间： 20000825

// 内容描述：用于动态显示牌板块数据与自选股管理部分

// 模块编号：004

///////////////////////////////////////////////////////////////////////////////////////////

CSuperviseSharesBlock::CSuperviseSharesBlock(CWnd* pParent /*=NULL*/)
	: CDialog(CSuperviseSharesBlock::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSuperviseSharesBlock)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CSuperviseSharesBlock::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSuperviseSharesBlock)
	DDX_Control(pDX, IDOK, m_ok);
	DDX_Control(pDX, IDC_UPDATESTOCKTYPE, m_updatestocktype);
	DDX_Control(pDX, IDC_REFRESH_DAY, m_refreshday);
	DDX_Control(pDX, IDC_REFRESH, m_refresh);
	//DDX_Control(pDX, IDC_MANAGERCHOOSE, m_managerchoose);
	DDX_Control(pDX, IDC_DELETESTOCKTYPE, m_delete);
	DDX_Control(pDX, IDC_ADDSTOCKTYPE, m_add);
	DDX_Control(pDX, IDC_PROGRESS1, m_Progress);
	DDX_Control(pDX, IDC_LIST1, m_lstStockTypeName);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_LIST3, m_lstBlockTypeName);
}


BEGIN_MESSAGE_MAP(CSuperviseSharesBlock, CDialog)
	//{{AFX_MSG_MAP(CSuperviseSharesBlock)
	ON_BN_CLICKED(IDC_IMPORT, OnImportStockType)
	ON_BN_CLICKED(IDC_ADDSTOCKTYPE, OnAddStockType)
	ON_BN_CLICKED(IDC_UPDATESTOCKTYPE, OnUpdateStockType)
	//ON_BN_CLICKED(IDC_MANAGERCHOOSE, OnManagerChoose)
	ON_BN_CLICKED(IDC_DELETESTOCKTYPE, OnDeletestocktype)
	ON_BN_CLICKED(IDC_REFRESH, OnRefresh)
	ON_BN_CLICKED(IDC_EXPORT, OnExport)
	ON_BN_CLICKED(IDC_REFRESH_DAY, OnRefreshDayLine)
	ON_WM_CLOSE()
	ON_WM_HELPINFO()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_ADDBLOCKTYPE, &CSuperviseSharesBlock::OnBnClickedAddblocktype)
	ON_BN_CLICKED(IDC_DELBLOCKTYPE, &CSuperviseSharesBlock::OnBnClickedDelblocktype)
	ON_BN_CLICKED(IDC_MODSTOCKTYPE, &CSuperviseSharesBlock::OnBnClickedModstocktype)
	ON_NOTIFY(NM_CLICK, IDC_LIST3, &CSuperviseSharesBlock::OnNMClickList3)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST1, &CSuperviseSharesBlock::OnNMDblclkList1)
	ON_BN_CLICKED(IDC_REFRESH_MINUTE, &CSuperviseSharesBlock::OnBnClickedRefreshMinute)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSuperviseSharesBlock message handlers
//功能：初始化对话框
//输入参数：无
//输出参数：无
BOOL CSuperviseSharesBlock::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
    m_bIsCalc=FALSE;
	this->CenterWindow();
	m_pDoc =((CMainFrame*)AfxGetMainWnd())->m_pDoc ;
   	m_pDoc ->m_bOpenStockTypeDlg=TRUE;
	m_pImageList = new CImageList();
	m_pImageListSmall = new CImageList();
	ASSERT(m_pImageList != NULL && m_pImageListSmall != NULL);    // serious allocation failure checking
	m_pImageList->Create(IDB_BITMAP14,32,0,RGB(255,255,255));    //建立大图象列表
	m_pImageListSmall->Create(IDB_BITMAP23,16,0,RGB(255,255,255)); //建立小图象列表
	//m_pImageListSmall->Create(IDB_BITMAP15,16,0,RGB(192,192,192)); //建立小图象列表
	FillToListCtrlHead();                                          //初始化CListCtrl列名

	//板块分类名列表
    CStringArray BlockTypeNameArray;
	m_pDoc->m_ManagerBlockData.GetAllBlockTypeName(BlockTypeNameArray);//得到全部板块名
	int nCount=BlockTypeNameArray.GetSize();
    for(int i=0;i<nCount;i++)
	{
		m_lstBlockTypeName.InsertItem(i,BlockTypeNameArray[i],0);
	}
	//板块名
    CStringArray m_StockTypeCodeArray;
	m_pDoc->m_ManagerBlockData.GetAllBlockCodes(m_StockTypeCodeArray);//得到全部板块名
	int l_nCount=m_StockTypeCodeArray.GetSize();
    for(int i=0;i<l_nCount;i++)
	{
       BLOCKINFO *l_pStktype;
       CString l_sStockType=m_StockTypeCodeArray.GetAt(i);
       m_pDoc->m_ManagerBlockData.GetBlockPoint(l_pStktype,l_sStockType.GetBuffer(0));
	   this->FillToListCtrlItem(l_pStktype);         //将板块插入CListCtrl控件中
	}

   m_lstBlockTypeName.SetFocus();
   m_lstBlockTypeName.SetItemState(0,LVIS_FOCUSED|LVIS_SELECTED , LVIS_SELECTED|LVIS_FOCUSED) ;

	m_pImageList->Detach();
	delete m_pImageList;
	m_pImageListSmall->Detach();
	delete m_pImageListSmall;

#ifdef WIDE_NET_VERSION
    CWnd *pWnd=this->GetDlgItem(IDC_REFRESH);
	pWnd->EnableWindow(FALSE);
#endif
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
//功能：初始化CListCtrl列名
//输入参数：无
//输出参数：无
void CSuperviseSharesBlock::FillToListCtrlHead()
{ 
   	CRect           rect;
	CString         strItem1= _T("指数代码");
	CString         strItem2= _T("指数名称");
	CString         strItem3= _T("权重类型");
	CString         strItem4= _T("股票总数");

	m_lstStockTypeName.SetExtendedStyle(m_lstStockTypeName.GetExtendedStyle()|LVS_EX_FULLROWSELECT);
    
	//this->m_lstStockTypeName.SetImageList(m_pImageList, LVSIL_NORMAL);
	this->m_lstStockTypeName.SetImageList(m_pImageListSmall, LVSIL_SMALL);

	m_lstBlockTypeName.SetExtendedStyle(m_lstBlockTypeName.GetExtendedStyle()|LVS_EX_FULLROWSELECT);

	//this->m_lstBlockTypeName.SetImageList(m_pImageList, LVSIL_NORMAL);
	this->m_lstBlockTypeName.SetImageList(m_pImageListSmall, LVSIL_SMALL);

	// insert two columns (REPORT mode) and modify the new header items
	//this->m_lstStockTypeName.GetWindowRect(&rect);
	//this->m_lstStockTypeName.InsertColumn(0, strItem1, LVCFMT_LEFT,    //插入列名
	//	rect.Width() * 1/4, 0); 
	//this->m_lstStockTypeName.InsertColumn(1, strItem2, LVCFMT_LEFT,    //插入列名
	//	rect.Width() * 1/4, 1);
	//this->m_lstStockTypeName.InsertColumn(2, strItem3, LVCFMT_LEFT,    //插入列名
	//	rect.Width() * 1/4, 2);
	//this->m_lstStockTypeName.InsertColumn(3, strItem4, LVCFMT_LEFT,    //插入列名
	//	rect.Width() * 1/4, 3);
}
//功能：将板块插入到CListCtrl中
//输入参数：pStockType板块结构指针
//输出参数：无
void CSuperviseSharesBlock::FillToListCtrlItem(BLOCKINFO *pStockType)
{
	int  iIcon, iItem, iSubItem, iActualItem;
	LV_ITEM  lvitem;
	LVFINDINFO  lvf;
	lvf.flags=LVFI_STRING;
	lvf.psz=pStockType->m_szName;
	//lvf.psz=pStockType->m_szCode;
    if(m_lstStockTypeName.FindItem(&lvf)>=0)
		return;
    iItem=this->m_lstStockTypeName.GetItemCount();          //得到插入板块位置

	m_lstStockTypeName.InsertItem(iItem,pStockType->m_szName,1);
	m_lstStockTypeName.SetItemData(iItem,(DWORD_PTR)pStockType);

/*
	for (iSubItem = 0; iSubItem <4; iSubItem++)
	{
		if (iSubItem == 0)
		{
			iIcon = 0;  // choose the icon and legend for the entry
//			lvitem.lParam=nParam;
		}
		lvitem.mask = LVIF_TEXT | (iSubItem == 0? LVIF_IMAGE : 0);
		lvitem.iItem = (iSubItem == 0)? iItem : iActualItem;
		lvitem.iSubItem = iSubItem;

		// calculate the main and sub-item strings for the current item
		CString strIconDesc[4];
        strIconDesc[0]=pStockType->m_szCode;
        strIconDesc[1]=pStockType->m_szName;

		switch(pStockType->m_iRightType)       //权重类型
		{
		case ZGB:     strIconDesc[2]="总股本";
		              break;  	   
		case LTG:     strIconDesc[2]="流通股本";
		              break;  	   
		case OTHER:   strIconDesc[2]="相等权重";
		              break;  	   
		}
        strIconDesc[3].Format("%d",pStockType->m_lStockCount);
        
		lvitem.pszText=strIconDesc[iSubItem].GetBuffer(strIconDesc[iSubItem].GetLength()); 
		lvitem.iImage = iIcon;
		if (iSubItem == 0)
			iActualItem =this->m_lstStockTypeName.InsertItem(&lvitem); // insert new item
		else
			this->m_lstStockTypeName.SetItem(&lvitem); // modify existing item (the sub-item text)
	}
*/
}

//功能：从板块数据文件中导入板块数据
//输入参数：无
//输出参数：无
void CSuperviseSharesBlock::OnImportStockType() 
{
	// TODO: Add your control notification handler code here

	int stocklength;
	MSG message;
	char  blank[2]={' ',' '};  
	CString filename;
	CString DefExt="*";  //文件扩展名.
	CString FileName="板块数据\\*.*";  //最初出现在文件名编辑框中的文件名
	CString Filter="板块文件名 ( *.* )| *.* ||";  //.文件扩展名过滤器
	CFileDialog OpenFileDialog(TRUE,(LPCSTR)DefExt,
		(LPCTSTR)FileName,
		OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT|OFN_NOCHANGEDIR|OFN_ALLOWMULTISELECT ,
		(LPCSTR)Filter,this);
	if( OpenFileDialog.DoModal() ==IDOK )         //打开文件选择对话框
	{
		CWnd *pWnd=this->GetDlgItem(IDC_IMPORT);
		pWnd->EnableWindow(FALSE);
        BeginWaitCursor();
//**************************************************************************************
		int index=0;
		int AddCounts=0;
        POSITION l_position=OpenFileDialog.GetStartPosition();
		while(l_position)
		{
//			int l_nPos;
			CFile stockfile;
//			CReportData *pCdat;
//			BLOCKINFO *l_pStock;
			filename=OpenFileDialog.GetNextPathName(l_position);
			index++;
   			if(stockfile.Open((LPCTSTR)filename,CFile::modeReadWrite))
			{
				int FileLength=stockfile.GetLength();
				stockfile.Read(&stocklength,4);
				int FileCalcLength=4+stocklength*12;
				if(FileLength!=FileCalcLength)
				{
					MessageBox("导入板块数据文件不正确,请重选板块文件! 本次导入操作失败.","板块导入操作提示",MB_OK);
				    stockfile.Close();
		            pWnd->EnableWindow(TRUE);
                    EndWaitCursor();
					return;

				}
				for(int j=0;j<stocklength;j++)
				{
					STOCKINFOINBLOCK *pStock;
					SharesSymbol *symbol=new SharesSymbol;
					stockfile.Read(symbol->sharesymbol,6);
					stockfile.Read(&symbol->nKind,4);
					symbol->sharesymbol[6]='\0';
					stockfile.Read(&blank,2);
					if(!m_pDoc->m_ManagerBlockData.Lookup(symbol->sharesymbol,symbol->nKind,pStock))
					{
						AddCounts++;
					}
					delete symbol;
				}
				stockfile.Close();

			}
		}
		m_pDoc->m_ManagerBlockData.CheckStockCount(AddCounts);
//*************************************************************************************
        m_Progress.SetRange(0,index);
        m_Progress.SetPos(0);
		index=0;
        l_position=OpenFileDialog.GetStartPosition();
		while(l_position)
		{
			CString l_sFileName;
			CString l_sFilePath;
			int l_nPos;
			CReportData *pCdat;
			BLOCKINFO *l_pStockkType;
			filename=OpenFileDialog.GetNextPathName(l_position);
			filename.MakeReverse();
			l_nPos=filename.Find("\\",0);
			l_sFileName=filename.Left(l_nPos);
			l_sFileName.MakeReverse();
			if(l_sFileName.GetLength()>8)
			{
				MessageBox("输入板块名的长度不能大于8个字节","板块操作提示",MB_OK);
				continue;
			}
			filename.MakeReverse();
			l_sFilePath=filename.Left(filename.GetLength() - l_nPos-1);   //得到导入文件的路径与文件名
			m_pDoc->m_ManagerBlockData.ImportBlockData(l_sFilePath,l_sFileName,l_pStockkType);  //导入板块
			
			m_pDoc->m_sharesInformation.InsertItem(l_pStockkType->m_szCode,pCdat,BLOCKINDEX);              //增加股票代码
			m_pDoc->m_pReportView->LoadShowData(BLOCKINDEX);              //在CGRID中增加一行 
			strcpy(pCdat->name ,l_pStockkType->m_szName );
			strcpy(pCdat->id ,l_pStockkType->m_szCode );
            pCdat->kind=BLOCKINDEX  ;
			if(strlen(pCdat->name)>0)
			   m_pDoc->StockNameConvert(pCdat->name,pCdat->Gppyjc) ;

			m_pDoc->AddStockToKeyboard(pCdat->id,pCdat->kind,pCdat->name,pCdat->Gppyjc);
			WORD type=BLOCKINDEX;
			m_pDoc->m_pReportView->AddGridRow(type);              //在CGRID中增加一行 

			FillToListCtrlItem(l_pStockkType);           //将板块插入到CListCtrl中
//*************************************************************************************
			index++;
			m_Progress.SetPos(index);
			if(PeekMessage(&message,NULL,0,0,PM_REMOVE))
			{
				TranslateMessage(&message);
				DispatchMessage(&message);
			}

//*************************************************************************************
		}	
		pWnd->EnableWindow(TRUE);
        EndWaitCursor();

	}
	m_Progress.SetPos(0);
}

//功能：增加新的板块数据
//输入参数：无
//输出参数：无
void CSuperviseSharesBlock::OnAddStockType() 
{
	int nItem = -1;
	CString strItem = "";
	CStringArray stockTypeCodeArray;

	POSITION pos = m_lstBlockTypeName.GetFirstSelectedItemPosition();
	if (pos == NULL)
		return;
	else
	{
		while (pos)
		{
			nItem = m_lstBlockTypeName.GetNextSelectedItem(pos);
			strItem = m_lstBlockTypeName.GetItemText(nItem,0);
			break;
		}
	}


	BLOCKINFO *pBlockInfo=new BLOCKINFO;
	char temp[10];
	int l_nPos=m_pDoc->m_ManagerBlockData.GetInsertBlockPos();  //得到新板块位置
    sprintf(temp,"%3d",l_nPos);         
	for(int i=0;i<3;i++)
	{
		if(temp[i]==' ')temp[i]='0';
	}
	sprintf(pBlockInfo->m_szCode,"%s%s","8K0",temp);         //从得到新板块位置生成板块代码
	strcpy(pBlockInfo->m_szBlockTypeName,strItem.GetBuffer(0));//板块分类名

	pBlockInfo->m_iRightType=0;       

	CModifyStockType l_ModifyStockType(pBlockInfo,TRUE,this);   //打开增加板块对话框
    if(l_ModifyStockType.DoModal()== IDOK)
	{
		RefreshShowData();                                          //增加成功刷新板块数据
	}
    delete pBlockInfo;
}

//功能：打开板块数据修改对话框，用于管理板块修改
//输入参数：无
//输出参数：无
void CSuperviseSharesBlock::OnUpdateStockType() 
{
	// TODO: Add your control notification handler code here
	BLOCKINFO *l_pStockType,*l_pStockTypeInfo,*pStockType;
	l_pStockType=new BLOCKINFO;
	int temp=m_lstStockTypeName.GetNextItem( -1, LVNI_SELECTED   ) ;  //得到正确将要修改板块数据
	char l_StockTypeCode[10];
	if(temp==-1)
	{
		if(l_pStockType) delete l_pStockType;
		AfxMessageBox("请选择正确的板块!");
		return;
    }
	//m_lstStockTypeName.GetItemText( temp, 0, l_StockTypeCode, 10);
	pStockType = (BLOCKINFO *)m_lstStockTypeName.GetItemData(temp);
	m_pDoc->m_ManagerBlockData.GetBlockPoint(l_pStockTypeInfo,pStockType->m_szCode);
	memcpy(l_pStockType,l_pStockTypeInfo,sizeof(BLOCKINFO));
	CModifyStockType l_ModifyStockType(l_pStockType,FALSE);
    if(l_ModifyStockType.DoModal()== IDOK)         //打开板块修改对话框
	{
		RefreshShowData();                         //成功修改后刷新数据
	}
	delete l_pStockType;
}

//功能：打开自选股管理窗口
//输入参数：无
//输出参数：无
void CSuperviseSharesBlock::OnManagerChoose() 
{
	// TODO: Add your control notification handler code here
	CSuperviseChoose l_ManagerChooseDlg;
    l_ManagerChooseDlg.DoModal();    //打开自选股管理窗口
}
//功能：刷新CListCtrl板块数据，在进行板块数据修改后进行数据刷新
//输入参数：无
//输出参数：无
void CSuperviseSharesBlock::RefreshShowData()
{
    BeginWaitCursor();

	int nItem = -1;
	CString strItem = "";
	CStringArray stockTypeCodeArray;

	POSITION pos = m_lstBlockTypeName.GetFirstSelectedItemPosition();
	if (pos == NULL)
		TRACE0("No items were selected!\n");
	else
	{
		while (pos)
		{
			nItem = m_lstBlockTypeName.GetNextSelectedItem(pos);
			break;
		}
	}
	if (nItem != -1)
	{
		strItem = m_lstBlockTypeName.GetItemText(nItem,0);
		m_pDoc->m_ManagerBlockData.GetAllBlockCodesFromBlockTypeName(stockTypeCodeArray,strItem);
	}

	this->m_lstStockTypeName.DeleteAllItems();
	//m_pDoc->m_ManagerBlockData.GetAllBlockCodes(stockTypeCodeArray); //得到板块数据
	int l_nCount=stockTypeCodeArray.GetSize();
    for(int i=0;i<l_nCount;i++)     //将板块数据插入CListCtrl
	{
       BLOCKINFO *l_pStktype;
       CString l_sStockType=stockTypeCodeArray.GetAt(i);
       m_pDoc->m_ManagerBlockData.GetBlockPoint(l_pStktype,l_sStockType.GetBuffer(0));
	   this->FillToListCtrlItem(l_pStktype);
	}

    EndWaitCursor();

}

//功能：将某一板块从全部板块中删除
//输入参数：无
//输出参数：无
void CSuperviseSharesBlock::OnDeletestocktype() 
{
	// TODO: Add your control notification handler code here
	int temp=m_lstStockTypeName.GetNextItem( -1, LVNI_SELECTED   ) ;  //查找删除板块
	char l_StockTypeCode[10];
	if(temp==-1)
	{
		AfxMessageBox("请选择将删除的板块!");
		return;
    }
    int rtn = MessageBox("是否确实进行这一删除操作?","警告",MB_YESNO|MB_ICONWARNING);
    if(rtn!=6)
	  return;

	//m_lstStockTypeName.GetItemText( temp, 0, l_StockTypeCode, 10);        //得到删除板块代码
	//m_pDoc->m_ManagerBlockData.DeleteBlock(l_StockTypeCode);   //删除板块
	
	BLOCKINFO *pStockType;
	pStockType = (BLOCKINFO *)m_lstStockTypeName.GetItemData(temp);
	if (!pStockType)
	{
		return;
	}
	strcpy(l_StockTypeCode, pStockType->m_szCode);
	m_pDoc->m_ManagerBlockData.DeleteBlock(l_StockTypeCode);   //删除板块


    CReportData *pCdat = NULL;
	m_pDoc->m_sharesInformation.Lookup(l_StockTypeCode,pCdat,BLOCKINDEX);         //删除板块股票代码
	if (pCdat)
	{
		m_pDoc->DeleteStockFromKeyboard(pCdat->id,pCdat->kind,pCdat->name,pCdat->Gppyjc);
	}
    m_pDoc->m_sharesInformation.DeleteOneStockInfo(l_StockTypeCode,BLOCKINDEX);
	RefreshShowData();
}

//功能：板块数据刷新，重计算板块每分钟指数
//输入参数：无
//输出参数：无
void CSuperviseSharesBlock::OnRefresh() 
{
	//CWnd *pWnd=this->GetDlgItem(IDC_REFRESH);
	//pWnd->EnableWindow(FALSE);
 //   BeginWaitCursor();

	//m_pDoc->m_ManagerBlockData.CalcBlockKlineData(); //计算板块当天每分钟板块指数
	//m_pDoc->m_ManagerBlockData.CalcBlockOpenClosePrice();  //计算板块当天开盘与昨收板块指数
	//pWnd->EnableWindow(TRUE);
 //   EndWaitCursor();
	//pWnd->EnableWindow(TRUE);

	//更新板块的5分钟线数据
	CWnd *pWnd=this->GetDlgItem(IDC_REFRESH);
	pWnd->EnableWindow(FALSE);
	pWnd=this->GetDlgItem(IDOK);
	pWnd->EnableWindow(FALSE);
	pWnd=this->GetDlgItem(IDC_ADDSTOCKTYPE);
	pWnd->EnableWindow(FALSE);
	pWnd=this->GetDlgItem(IDC_UPDATESTOCKTYPE);
	pWnd->EnableWindow(FALSE);
	pWnd=this->GetDlgItem(IDC_DELETESTOCKTYPE);
	pWnd->EnableWindow(FALSE);
	pWnd=this->GetDlgItem(IDC_REFRESH_DAY);
	pWnd->EnableWindow(FALSE);
	pWnd=this->GetDlgItem(IDC_REFRESH_MINUTE);
	pWnd->EnableWindow(FALSE);
	pWnd=this->GetDlgItem(IDC_ADDBLOCKTYPE);
	pWnd->EnableWindow(FALSE);
	pWnd=this->GetDlgItem(IDC_DELBLOCKTYPE);
	pWnd->EnableWindow(FALSE);
	pWnd=this->GetDlgItem(IDC_MODSTOCKTYPE);
	pWnd->EnableWindow(FALSE);
	CMenu *pMenu=GetSystemMenu(FALSE);
	pMenu->EnableMenuItem(SC_CLOSE,MF_BYCOMMAND|MF_DISABLED|MF_GRAYED); 
	m_bIsCalc=TRUE;
	
	BeginWaitCursor();
	CStringArray BlockCodeArray;
	m_pDoc->m_ManagerBlockData.GetAllBlockCodes(BlockCodeArray);
	for (int i = 0;i < BlockCodeArray.GetCount();i++)
	{
		m_pDoc->m_ManagerBlockData.RefreshHistoryMin5LineData((LPTSTR)(LPCTSTR)BlockCodeArray[i],&m_Progress);
		ProcMessage();
	}

    EndWaitCursor();
	pWnd=this->GetDlgItem(IDC_REFRESH);
	pWnd->EnableWindow(TRUE);
	pWnd=this->GetDlgItem(IDOK);
	pWnd->EnableWindow(TRUE);
	pWnd=this->GetDlgItem(IDC_ADDSTOCKTYPE);
	pWnd->EnableWindow(TRUE);
	pWnd=this->GetDlgItem(IDC_UPDATESTOCKTYPE);
	pWnd->EnableWindow(TRUE);
	pWnd=this->GetDlgItem(IDC_DELETESTOCKTYPE);
	pWnd->EnableWindow(TRUE);
	pWnd=this->GetDlgItem(IDC_REFRESH_DAY);
	pWnd->EnableWindow(TRUE);
	pWnd=this->GetDlgItem(IDC_REFRESH_MINUTE);
	pWnd->EnableWindow(TRUE);
	pWnd=this->GetDlgItem(IDC_ADDBLOCKTYPE);
	pWnd->EnableWindow(TRUE);
	pWnd=this->GetDlgItem(IDC_DELBLOCKTYPE);
	pWnd->EnableWindow(TRUE);
	pWnd=this->GetDlgItem(IDC_MODSTOCKTYPE);
	pWnd->EnableWindow(TRUE);

	m_bIsCalc=FALSE;
	pMenu=GetSystemMenu(FALSE);
	pMenu->EnableMenuItem(SC_CLOSE,MF_BYCOMMAND|MF_ENABLED); 

}

void CSuperviseSharesBlock::OnExport() 
{
	// TODO: Add your control notification handler code here
	char m_szCurrentDir[MAX_PATH],makedirectory[MAX_PATH];
	::GetCurrentDirectory(MAX_PATH, m_szCurrentDir);
	sprintf(makedirectory,"%s\\板块数据",m_szCurrentDir);
	if(_access(makedirectory,0)==-1)
		_mkdir(makedirectory);

	int nCount=m_lstStockTypeName.GetItemCount();
	char  blank[2]={' ',' '};              //数据之间的间隔
	for(int i=0;i<nCount;i++)
	{
		char strStockCode[10];
		char strStockName[10];
		FILE *fp;
		SymbolKindArr StockCodeArray;
        char FileName[256];
		m_lstStockTypeName.GetItemText( i, 0, strStockCode, sizeof(strStockCode) ) ;
		m_lstStockTypeName.GetItemText( i, 1, strStockName, sizeof(strStockName) ) ;
		sprintf(FileName,"%s\\%s",makedirectory,strStockName);
        m_pDoc->m_ManagerBlockData.GetStockFromBlockCode(StockCodeArray,strStockCode);
        int length=StockCodeArray.GetSize();
		fp=_fsopen(FileName,"w+b",SH_DENYNO);
		if(fp!=NULL)
		{
			fwrite(&length,4,1,fp);
			for(int i=0;i<StockCodeArray.GetSize();i++)
			{
				SymbolKind m_SymbolKind=StockCodeArray.GetAt(i);
				fwrite(m_SymbolKind.m_chSymbol ,1,6,fp);
				fwrite(&m_SymbolKind.m_nSymbolKind ,1,4,fp);

				fwrite(&blank,1,2,fp);
			}
			fclose(fp);
		}       
	}
	if(nCount>0)
		AfxMessageBox("板块数据导出成功!");
}

void CSuperviseSharesBlock::OnRefreshDayLine() 
{
	/*Marenan
	int temp=m_lstStockTypeName.GetNextItem( -1, LVNI_SELECTED   ) ;  //查找删除板块
	char l_StockTypeCode[20];
	if(temp==-1)
	{
		AfxMessageBox("请选择正确的刷新板块!");
		return;
    }
	CWnd *pWnd=this->GetDlgItem(IDC_REFRESH_DAY);
	pWnd->EnableWindow(FALSE);
	pWnd=this->GetDlgItem(IDOK);
	pWnd->EnableWindow(FALSE);
	CMenu *pMenu=GetSystemMenu(FALSE);
	pMenu->EnableMenuItem(SC_CLOSE,MF_BYCOMMAND|MF_DISABLED|MF_GRAYED); 

    BeginWaitCursor();
    m_bIsCalc=TRUE;
	m_lstStockTypeName.GetItemText( temp, 0, l_StockTypeCode, 20);        //得到删除板块代码
	BLOCKINFO *pStockType = NULL;
	pStockType = (BLOCKINFO *)m_lstStockTypeName.GetItemData(temp);
	if (!pStockType)
	{
		return;
	}
    m_pDoc->m_ManagerBlockData.RefreshHistoryDayLineData( pStockType->m_szCode,&m_Progress);
	pWnd=this->GetDlgItem(IDC_REFRESH_DAY);
	pWnd->EnableWindow(TRUE);
	pWnd=this->GetDlgItem(IDOK);
	pWnd->EnableWindow(TRUE);

	//if(temp+1<m_lstStockTypeName.GetItemCount())
	//{
	//	m_lstStockTypeName.SetFocus();
	//	m_lstStockTypeName.SetItemState(temp+1,LVIS_FOCUSED|LVIS_SELECTED , LVIS_SELECTED|LVIS_FOCUSED) ;
	//}
	EndWaitCursor();
	m_bIsCalc=FALSE;
	pMenu->EnableMenuItem(SC_CLOSE,MF_BYCOMMAND|MF_ENABLED); 
	
	Marenan*/

	//更新板块的日线数据
	CWnd *pWnd=this->GetDlgItem(IDC_REFRESH_DAY);
	pWnd->EnableWindow(FALSE);
	pWnd=this->GetDlgItem(IDOK);
	pWnd->EnableWindow(FALSE);
	pWnd->EnableWindow(FALSE);
	pWnd=this->GetDlgItem(IDC_ADDSTOCKTYPE);
	pWnd->EnableWindow(FALSE);
	pWnd=this->GetDlgItem(IDC_UPDATESTOCKTYPE);
	pWnd->EnableWindow(FALSE);
	pWnd=this->GetDlgItem(IDC_DELETESTOCKTYPE);
	pWnd->EnableWindow(FALSE);
	pWnd=this->GetDlgItem(IDC_REFRESH);
	pWnd->EnableWindow(FALSE);
	pWnd=this->GetDlgItem(IDC_REFRESH_MINUTE);
	pWnd->EnableWindow(FALSE);
	pWnd=this->GetDlgItem(IDC_ADDBLOCKTYPE);
	pWnd->EnableWindow(FALSE);
	pWnd=this->GetDlgItem(IDC_DELBLOCKTYPE);
	pWnd->EnableWindow(FALSE);
	pWnd=this->GetDlgItem(IDC_MODSTOCKTYPE);
	pWnd->EnableWindow(FALSE);
	CMenu *pMenu=GetSystemMenu(FALSE);
	pMenu->EnableMenuItem(SC_CLOSE,MF_BYCOMMAND|MF_DISABLED|MF_GRAYED); 
	m_bIsCalc=TRUE;
	
	CStringArray BlockCodeArray;
	m_pDoc->m_ManagerBlockData.GetAllBlockCodes(BlockCodeArray);
	for (int i = 0;i < BlockCodeArray.GetCount();i++)
	{
		m_pDoc->m_ManagerBlockData.RefreshHistoryDayLineData((LPTSTR)(LPCTSTR)BlockCodeArray[i],&m_Progress);
		ProcMessage();
	}
	pWnd=this->GetDlgItem(IDC_REFRESH_DAY);
	pWnd->EnableWindow(TRUE);
	pWnd=this->GetDlgItem(IDOK);
	pWnd->EnableWindow(TRUE);
	pWnd=this->GetDlgItem(IDC_ADDSTOCKTYPE);
	pWnd->EnableWindow(TRUE);
	pWnd=this->GetDlgItem(IDC_UPDATESTOCKTYPE);
	pWnd->EnableWindow(TRUE);
	pWnd=this->GetDlgItem(IDC_DELETESTOCKTYPE);
	pWnd->EnableWindow(TRUE);
	pWnd=this->GetDlgItem(IDC_REFRESH);
	pWnd->EnableWindow(TRUE);
	pWnd=this->GetDlgItem(IDC_REFRESH_MINUTE);
	pWnd->EnableWindow(TRUE);
	pWnd=this->GetDlgItem(IDC_ADDBLOCKTYPE);
	pWnd->EnableWindow(TRUE);
	pWnd=this->GetDlgItem(IDC_DELBLOCKTYPE);
	pWnd->EnableWindow(TRUE);
	pWnd=this->GetDlgItem(IDC_MODSTOCKTYPE);
	pWnd->EnableWindow(TRUE);

	m_bIsCalc=FALSE;
	pMenu=GetSystemMenu(FALSE);
	pMenu->EnableMenuItem(SC_CLOSE,MF_BYCOMMAND|MF_ENABLED); 

}
void CSuperviseSharesBlock::OnCancel()
{
	if(m_bIsCalc)
	{
		AfxMessageBox("程序正在运算不能进行退出操作!");
		return;
	}

   	m_pDoc ->m_bOpenStockTypeDlg=FALSE;
	CDialog::OnCancel();
    SendMessage(WM_CLOSE,0,0); 
}

void CSuperviseSharesBlock::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	if(m_bIsCalc)
	{
		AfxMessageBox("程序正在运算不能进行退出操作!");
		return;
	}

	::SendMessage(m_pDoc->m_pReportView->m_hWnd,WM_USER_CHANGESTOCK,0,0);
   	m_pDoc ->m_bOpenStockTypeDlg=FALSE;

	CDialog::OnClose();
}

void CSuperviseSharesBlock::OnOK() 
{
	// TODO: Add extra validation here
	if(m_bIsCalc)
	{
		AfxMessageBox("程序正在运算不能进行退出操作!");
		return;
	}

	::SendMessage(m_pDoc->m_pReportView->m_hWnd,WM_USER_CHANGESTOCK,0,0);
   	m_pDoc ->m_bOpenStockTypeDlg=FALSE;

	CDialog::OnOK();
}

BOOL CSuperviseSharesBlock::OnHelpInfo(HELPINFO* pHelpInfo) 
{
	// TODO: Add your message handler code here and/or call default
//	HtmlHelp(m_hWnd,"stock.chm",HH_HELP_CONTEXT,248);
  //  return TRUE;	
	DoHtmlHelp(this);return TRUE;
	
//	return CDialog::OnHelpInfo(pHelpInfo);
}

void CSuperviseSharesBlock::OnBnClickedAddblocktype()
{
	CBlockTypeNameDlg dlg;
	if (dlg.DoModal() == IDOK)
	{
		CString strName;
		BOOL bFind = FALSE;
		dlg.GetString(strName);

		for (int i = 0;i < m_lstBlockTypeName.GetItemCount();i++)
		{
			CString strItem = m_lstBlockTypeName.GetItemText(i,0);
			if (strItem == strName)
			{
				bFind = TRUE;
				break;
			}
		}
		if (!bFind)
		{
			m_lstBlockTypeName.InsertItem(m_lstBlockTypeName.GetItemCount(),strName,0);
			//更改数据库
			m_pDoc->m_ManagerBlockData.AddBlockType(strName);
		}
		else
		{
			AfxMessageBox("分类已经存在!");
		}

	}
}

void CSuperviseSharesBlock::OnBnClickedDelblocktype()
{
	int nItem = -1;
	CString strItem = "";
	CStringArray stockTypeCodeArray;

	POSITION pos = m_lstBlockTypeName.GetFirstSelectedItemPosition();
	if (pos == NULL)
		TRACE0("No items were selected!\n");
	else
	{
		while (pos)
		{
			nItem = m_lstBlockTypeName.GetNextSelectedItem(pos);
			strItem = m_lstBlockTypeName.GetItemText(nItem,0);
			break;
		}
	}
	if (strItem == "全部板块" || strItem == "其它板块")
	{
		AfxMessageBox("内置板块分类，不能删除!");
	}
	if (nItem != -1)
	{
		m_lstBlockTypeName.DeleteItem(nItem);
		//更改数据库,删除所有相关的板块数据
		m_pDoc->m_ManagerBlockData.DelBlockType(strItem);
		RefreshShowData();
	}
}

void CSuperviseSharesBlock::OnBnClickedModstocktype()
{
	// TODO: 在此添加控件通知处理程序代码
}

void CSuperviseSharesBlock::OnNMClickList3(NMHDR *pNMHDR, LRESULT *pResult)
{
	int nItem = -1;
	CString strItem = "";
	CStringArray stockTypeCodeArray;

	POSITION pos = m_lstBlockTypeName.GetFirstSelectedItemPosition();
	if (pos == NULL)
		TRACE0("No items were selected!\n");
	else
	{
		while (pos)
		{
			nItem = m_lstBlockTypeName.GetNextSelectedItem(pos);
			break;
		}
	}
	if (nItem != -1)
	{
		strItem = m_lstBlockTypeName.GetItemText(nItem,0);
		m_pDoc->m_ManagerBlockData.GetAllBlockCodesFromBlockTypeName(stockTypeCodeArray,strItem);
	}

	m_lstStockTypeName.DeleteAllItems();
	int nCount = stockTypeCodeArray.GetSize();
	for(int i=0;i<nCount;i++)
	{
		BLOCKINFO *pStktype;
		CString sStockType = stockTypeCodeArray.GetAt(i);
		m_pDoc->m_ManagerBlockData.GetBlockPoint(pStktype,sStockType.GetBuffer(0));
		this->FillToListCtrlItem(pStktype);         //将板块插入CListCtrl控件中
	}

	*pResult = 0;
}

void CSuperviseSharesBlock::OnNMDblclkList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	int nItem = -1;
	CString strItem = "";

	POSITION pos = m_lstStockTypeName.GetFirstSelectedItemPosition();
	if (pos == NULL)
		TRACE0("No items were selected!\n");
	else
	{
		while (pos)
		{
			nItem = m_lstBlockTypeName.GetNextSelectedItem(pos);
			break;
		}
	}
	if (nItem != -1)
	{
		strItem = m_lstStockTypeName.GetItemText(nItem,0);
		strcpy(m_pDoc->m_SystemInitData.StockTypeName,strItem.GetBuffer(0));
		m_pDoc->m_pReportView->ChangeToPage(BLOCKPAGE, 0);
		m_pDoc->m_pReportView->m_pPageWnd->ReplacePage(BLOCKPAGE, strItem);
	}
	*pResult = 0;
}

void CSuperviseSharesBlock::OnBnClickedRefreshMinute()
{
	//更新板块的分钟线数据
	CWnd *pWnd=this->GetDlgItem(IDC_REFRESH);
	pWnd->EnableWindow(FALSE);
	pWnd=this->GetDlgItem(IDOK);
	pWnd->EnableWindow(FALSE);
	pWnd=this->GetDlgItem(IDC_ADDSTOCKTYPE);
	pWnd->EnableWindow(FALSE);
	pWnd=this->GetDlgItem(IDC_UPDATESTOCKTYPE);
	pWnd->EnableWindow(FALSE);
	pWnd=this->GetDlgItem(IDC_DELETESTOCKTYPE);
	pWnd->EnableWindow(FALSE);
	pWnd=this->GetDlgItem(IDC_REFRESH_DAY);
	pWnd->EnableWindow(FALSE);
	pWnd=this->GetDlgItem(IDC_REFRESH_MINUTE);
	pWnd->EnableWindow(FALSE);
	pWnd=this->GetDlgItem(IDC_ADDBLOCKTYPE);
	pWnd->EnableWindow(FALSE);
	pWnd=this->GetDlgItem(IDC_DELBLOCKTYPE);
	pWnd->EnableWindow(FALSE);
	pWnd=this->GetDlgItem(IDC_MODSTOCKTYPE);
	pWnd->EnableWindow(FALSE);
	CMenu *pMenu=GetSystemMenu(FALSE);
	pMenu->EnableMenuItem(SC_CLOSE,MF_BYCOMMAND|MF_DISABLED|MF_GRAYED); 
	m_bIsCalc=TRUE;

	BeginWaitCursor();
	
	m_pDoc->m_ManagerBlockData.CalcBlockKlineData(); //计算板块当天每分钟板块指数
	m_pDoc->m_ManagerBlockData.CalcBlockOpenClosePrice();  //计算板块当天开盘与昨收板块指数

	EndWaitCursor();
	pWnd=this->GetDlgItem(IDC_REFRESH);
	pWnd->EnableWindow(TRUE);
	pWnd=this->GetDlgItem(IDOK);
	pWnd->EnableWindow(TRUE);
	pWnd=this->GetDlgItem(IDC_ADDSTOCKTYPE);
	pWnd->EnableWindow(TRUE);
	pWnd=this->GetDlgItem(IDC_UPDATESTOCKTYPE);
	pWnd->EnableWindow(TRUE);
	pWnd=this->GetDlgItem(IDC_DELETESTOCKTYPE);
	pWnd->EnableWindow(TRUE);
	pWnd=this->GetDlgItem(IDC_REFRESH_DAY);
	pWnd->EnableWindow(TRUE);
	pWnd=this->GetDlgItem(IDC_REFRESH_MINUTE);
	pWnd->EnableWindow(TRUE);
	pWnd=this->GetDlgItem(IDC_ADDBLOCKTYPE);
	pWnd->EnableWindow(TRUE);
	pWnd=this->GetDlgItem(IDC_DELBLOCKTYPE);
	pWnd->EnableWindow(TRUE);
	pWnd=this->GetDlgItem(IDC_MODSTOCKTYPE);
	pWnd->EnableWindow(TRUE);

	m_bIsCalc=FALSE;
	pMenu=GetSystemMenu(FALSE);
	pMenu->EnableMenuItem(SC_CLOSE,MF_BYCOMMAND|MF_ENABLED); 
}
