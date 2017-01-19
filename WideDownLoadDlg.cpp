// WideDownLoadDlg.cpp : implementation file
//

#include "stdafx.h"
#include "WH.h"
#include "CTaiShanDoc.h"
#include <share.h>
#include  <io.h>
#include <direct.h>
#include <shellapi.h>
#include "mainfrm.h"
#include "CTaiKlineFileHS.h"
#include "SelectStock.h"
#include "CTaiKlineWideNet.h"
#include "WideDownLoadTimeSet.h"
#include "WideDownLoadDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
#define WM_USER_POSTFOCUS WM_USER+9998
/////////////////////////////////////////////////////////////////////////////
// CDownLoadNetDlg dialog


CDownLoadNetDlg::CDownLoadNetDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDownLoadNetDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDownLoadNetDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
    m_pMainFrm=(CMainFrame *)pParent;
	m_SocketID.hWnd=NULL;
    m_SocketID.iFlag=-1; 
	m_bDownLoadState=FALSE;
	m_pTechWideNet=NULL;
}


void CDownLoadNetDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDownLoadNetDlg)
	DDX_Control(pDX, IDC_SELECT_LOADDATA, m_MyTree);
	DDX_Control(pDX, IDC_LIST1, m_lstStockTypeName);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDownLoadNetDlg, CDialog)
	//{{AFX_MSG_MAP(CDownLoadNetDlg)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_LOAD_COLLECTE, OnLoadCollecte)
	ON_BN_CLICKED(IDC_SAVE_COLLECTE, OnSaveCollecte)
	ON_BN_CLICKED(IDC_ADD_LOADDATA, OnAddLoaddata)
	ON_BN_CLICKED(IDC_DELETE_LOADDATA, OnDeleteLoaddata)
	ON_BN_CLICKED(IDC_DOWNLOAD_START, OnDownloadStart)
	ON_BN_CLICKED(IDC_STOPDOWNLOAD, OnStopdownload)
	ON_BN_CLICKED(IDC_SETUP_TIME, OnSetupTime)
	ON_NOTIFY(NM_DBLCLK, IDC_SELECT_LOADDATA, OnDblclkSelectLoaddata)
	ON_NOTIFY(NM_CLICK, IDC_LIST1, OnClickList1)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST1, OnDblclkList1)
	ON_NOTIFY(LVN_KEYDOWN, IDC_LIST1, OnKeydownList1)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_USER_POSTFOCUS ,OnPostFocus)
	ON_MESSAGE(WM_USER_ADDSTOCK,OnAddStock)
	ON_MESSAGE(WM_USER_TRANSMITINFO,OnRecvTransmitInfoMation)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDownLoadNetDlg message handlers

BOOL CDownLoadNetDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
    m_pDoc =((CMainFrame*)AfxGetMainWnd())->m_pDoc ;
	m_pTechWideNet=new CTaiKlineWideNet;
    char makedirectory[MAX_PATH];

	::GetCurrentDirectory(MAX_PATH, m_szCurrentDir);
	sprintf(makedirectory,"%s\\Extract",m_szCurrentDir);
	if(_access(makedirectory,0)==-1)
		_mkdir(makedirectory);

    sprintf(makedirectory,"%s\\Extract\\SH",m_szCurrentDir);
	if(_access(makedirectory,0)==-1)
	   _mkdir(makedirectory);

	sprintf(makedirectory,"%s\\Extract\\SZ",m_szCurrentDir);
	if(_access(makedirectory,0)==-1)
	_mkdir(makedirectory);

	sprintf(makedirectory,"%s\\Extract\\Download",m_szCurrentDir);
	if(_access(makedirectory,0)==-1)
	_mkdir(makedirectory);

	sprintf(makedirectory,"%s\\Extract\\Download\\Detail",m_szCurrentDir);
	if(_access(makedirectory,0)==-1)
	_mkdir(makedirectory);

	this->CenterWindow();

	m_pImageList = new CImageList();
	m_pImageListSmall = new CImageList();
	ASSERT(m_pImageList != NULL && m_pImageListSmall != NULL);    // serious allocation failure checking
	m_pImageList->Create(IDB_BITMAP14,32,0,RGB(192,192,192));    //建立大图象列表
	m_pImageListSmall->Create(IDB_BITMAP15,16,0,RGB(192,192,192)); //建立小图象列表
	FillToListCtrlHead(); 	//初始化CListCtrl列名
	FillToTreeCtrlHead();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
void CDownLoadNetDlg::FillToListCtrlHead()    //初始化CListCtrl列名
{
   	CRect           rect;
	CString         strItem1= _T("数据类型");
	CString         strItem2= _T("证券名称");
	CString         strItem3= _T("证券类型");
	CString         strItem4= _T("下载日期");
	CString         strItem5= _T("下载总数");
	CString         strItem6= _T("完成数");


	m_lstStockTypeName.SetExtendedStyle(m_lstStockTypeName.GetExtendedStyle()|LVS_EX_FULLROWSELECT);
    
	this->m_lstStockTypeName.SetImageList(m_pImageList, LVSIL_NORMAL);
	this->m_lstStockTypeName.SetImageList(m_pImageListSmall, LVSIL_SMALL);

	// insert two columns (REPORT mode) and modify the new header items
	this->m_lstStockTypeName.GetWindowRect(&rect);
	this->m_lstStockTypeName.InsertColumn(0, strItem1, LVCFMT_LEFT,    //插入
		rect.Width() * 1/6+10, 0); 
	this->m_lstStockTypeName.InsertColumn(1, strItem2, LVCFMT_LEFT,    //插入
		rect.Width() * 1/6-10, 1);
	this->m_lstStockTypeName.InsertColumn(2, strItem3, LVCFMT_LEFT,    //插入
		rect.Width() * 1/6-5, 2);
	this->m_lstStockTypeName.InsertColumn(3, strItem4, LVCFMT_LEFT,    //插入
		rect.Width() * 1/6+40, 3);
	this->m_lstStockTypeName.InsertColumn(4, strItem5, LVCFMT_LEFT,    //插入
		rect.Width() * 1/6-15, 4);
	this->m_lstStockTypeName.InsertColumn(5, strItem6, LVCFMT_LEFT,    //插入
		rect.Width() * 1/6-20, 5);
}
void CDownLoadNetDlg::FillToTreeCtrlHead()          //初始化CTreeCtrl标题列
{
	// TODO: Add extra initialization here
	m_MyTree.SetImageList(m_pImageListSmall,TVSIL_NORMAL);
	TV_INSERTSTRUCT TreeCtrlItem;
	TreeCtrlItem.hParent = TVI_ROOT;
	TreeCtrlItem.hInsertAfter = TVI_LAST;
	TreeCtrlItem.item.mask = TVIF_TEXT | TVIF_PARAM|TVIF_IMAGE|TVIF_SELECTEDIMAGE  ;
	TreeCtrlItem.item.pszText = "日线数据";
    TreeCtrlItem.item.iImage=0;
	TreeCtrlItem.item.iSelectedImage=0;
	TreeCtrlItem.item.lParam =1000;
	HTREEITEM hTreeItem1 = this->m_MyTree.InsertItem(&TreeCtrlItem);

	TreeCtrlItem.hParent = hTreeItem1;
	TreeCtrlItem.hInsertAfter = TVI_LAST;
	TreeCtrlItem.item.mask = TVIF_TEXT | TVIF_PARAM|TVIF_IMAGE|TVIF_SELECTEDIMAGE  ;
	TreeCtrlItem.item.pszText = "当日日线";
    TreeCtrlItem.item.iImage=1;
	TreeCtrlItem.item.iSelectedImage=1;
	TreeCtrlItem.item.lParam =WideTodayDayLine;
	this->m_MyTree.InsertItem(&TreeCtrlItem);

	TreeCtrlItem.hParent = hTreeItem1;
	TreeCtrlItem.hInsertAfter = TVI_LAST;
	TreeCtrlItem.item.mask = TVIF_TEXT | TVIF_PARAM|TVIF_IMAGE|TVIF_SELECTEDIMAGE  ;
	TreeCtrlItem.item.pszText = "当日全部";
    TreeCtrlItem.item.iImage=1;
	TreeCtrlItem.item.iSelectedImage=1;
	TreeCtrlItem.item.lParam =WideTodayAllDayLine;
	this->m_MyTree.InsertItem(&TreeCtrlItem);

	TreeCtrlItem.hParent = hTreeItem1;
	TreeCtrlItem.hInsertAfter = TVI_LAST;
	TreeCtrlItem.item.mask = TVIF_TEXT | TVIF_PARAM|TVIF_IMAGE|TVIF_SELECTEDIMAGE  ;
	TreeCtrlItem.item.pszText = "历史日线";
    TreeCtrlItem.item.iImage=1;
	TreeCtrlItem.item.iSelectedImage=1;
	TreeCtrlItem.item.lParam =WideHistoryDayLine;
	this->m_MyTree.InsertItem(&TreeCtrlItem);

	TreeCtrlItem.hParent = TVI_ROOT;
	TreeCtrlItem.hInsertAfter = TVI_LAST;
	TreeCtrlItem.item.mask = TVIF_TEXT | TVIF_PARAM|TVIF_IMAGE|TVIF_SELECTEDIMAGE  ;
	TreeCtrlItem.item.pszText = "五分钟Κ线";
    TreeCtrlItem.item.iImage=0;
	TreeCtrlItem.item.iSelectedImage=0;
	TreeCtrlItem.item.lParam =2000;
	hTreeItem1 = this->m_MyTree.InsertItem(&TreeCtrlItem);

	TreeCtrlItem.hParent = hTreeItem1;
	TreeCtrlItem.hInsertAfter = TVI_LAST;
	TreeCtrlItem.item.mask = TVIF_TEXT | TVIF_PARAM|TVIF_IMAGE|TVIF_SELECTEDIMAGE  ;
	TreeCtrlItem.item.pszText = "当日分线";
    TreeCtrlItem.item.iImage=1;
	TreeCtrlItem.item.iSelectedImage=1;
	TreeCtrlItem.item.lParam =WideFiveMinuteLine;
	this->m_MyTree.InsertItem(&TreeCtrlItem);

	TreeCtrlItem.hParent = hTreeItem1;
	TreeCtrlItem.hInsertAfter = TVI_LAST;
	TreeCtrlItem.item.mask = TVIF_TEXT | TVIF_PARAM|TVIF_IMAGE|TVIF_SELECTEDIMAGE  ;
	TreeCtrlItem.item.pszText = "当日全部";
    TreeCtrlItem.item.iImage=1;
	TreeCtrlItem.item.iSelectedImage=1;
	TreeCtrlItem.item.lParam =WideAllFiveMinuteLine;
	this->m_MyTree.InsertItem(&TreeCtrlItem);

	TreeCtrlItem.hParent = hTreeItem1;
	TreeCtrlItem.hInsertAfter = TVI_LAST;
	TreeCtrlItem.item.mask = TVIF_TEXT | TVIF_PARAM|TVIF_IMAGE|TVIF_SELECTEDIMAGE  ;
	TreeCtrlItem.item.pszText = "历史分线";
    TreeCtrlItem.item.iImage=1;
	TreeCtrlItem.item.iSelectedImage=1;
	TreeCtrlItem.item.lParam =WideHistoryMinuteLine;
	this->m_MyTree.InsertItem(&TreeCtrlItem);

	TreeCtrlItem.hParent = TVI_ROOT;
	TreeCtrlItem.hInsertAfter = TVI_LAST;
	TreeCtrlItem.item.mask = TVIF_TEXT | TVIF_PARAM|TVIF_IMAGE|TVIF_SELECTEDIMAGE  ;
	TreeCtrlItem.item.pszText = "明细数据";
    TreeCtrlItem.item.iImage=0;
	TreeCtrlItem.item.iSelectedImage=0;
	TreeCtrlItem.item.lParam =3000;
	hTreeItem1 = this->m_MyTree.InsertItem(&TreeCtrlItem);

	TreeCtrlItem.hParent = hTreeItem1;
	TreeCtrlItem.hInsertAfter = TVI_LAST;
	TreeCtrlItem.item.mask = TVIF_TEXT | TVIF_PARAM|TVIF_IMAGE|TVIF_SELECTEDIMAGE  ;
	TreeCtrlItem.item.pszText = "当日明细";
    TreeCtrlItem.item.iImage=1;
	TreeCtrlItem.item.iSelectedImage=1;
	TreeCtrlItem.item.lParam =WideTodayDetail;
	this->m_MyTree.InsertItem(&TreeCtrlItem);

	TreeCtrlItem.hParent = hTreeItem1;
	TreeCtrlItem.hInsertAfter = TVI_LAST;
	TreeCtrlItem.item.mask = TVIF_TEXT | TVIF_PARAM|TVIF_IMAGE|TVIF_SELECTEDIMAGE  ;
	TreeCtrlItem.item.pszText = "当日全部";
    TreeCtrlItem.item.iImage=1;
	TreeCtrlItem.item.iSelectedImage=1;
	TreeCtrlItem.item.lParam =WideAllTodayDetail;
	this->m_MyTree.InsertItem(&TreeCtrlItem);

	TreeCtrlItem.hParent = hTreeItem1;
	TreeCtrlItem.hInsertAfter = TVI_LAST;
	TreeCtrlItem.item.mask = TVIF_TEXT | TVIF_PARAM|TVIF_IMAGE|TVIF_SELECTEDIMAGE  ;
	TreeCtrlItem.item.pszText = "历史明细";
    TreeCtrlItem.item.iImage=1;
	TreeCtrlItem.item.iSelectedImage=1;
	TreeCtrlItem.item.lParam =WideHistoryDetail;
	this->m_MyTree.InsertItem(&TreeCtrlItem);

	TreeCtrlItem.hParent = TVI_ROOT;
	TreeCtrlItem.hInsertAfter = TVI_LAST;
	TreeCtrlItem.item.mask = TVIF_TEXT | TVIF_PARAM|TVIF_IMAGE|TVIF_SELECTEDIMAGE  ;
	TreeCtrlItem.item.pszText = "基本资料";
    TreeCtrlItem.item.iImage=0;
	TreeCtrlItem.item.iSelectedImage=0;
	TreeCtrlItem.item.lParam =4000;
	hTreeItem1 = this->m_MyTree.InsertItem(&TreeCtrlItem);

	TreeCtrlItem.hParent = hTreeItem1;
	TreeCtrlItem.hInsertAfter = TVI_LAST;
	TreeCtrlItem.item.mask = TVIF_TEXT | TVIF_PARAM|TVIF_IMAGE|TVIF_SELECTEDIMAGE  ;
	TreeCtrlItem.item.pszText = "个股资料";
    TreeCtrlItem.item.iImage=1;
	TreeCtrlItem.item.iSelectedImage=1;
	TreeCtrlItem.item.lParam =WideSingleStockBaseInfomation;
	this->m_MyTree.InsertItem(&TreeCtrlItem);

	TreeCtrlItem.hParent = hTreeItem1;
	TreeCtrlItem.hInsertAfter = TVI_LAST;
	TreeCtrlItem.item.mask = TVIF_TEXT | TVIF_PARAM|TVIF_IMAGE|TVIF_SELECTEDIMAGE  ;
	TreeCtrlItem.item.pszText = "全部资料";
    TreeCtrlItem.item.iImage=1;
	TreeCtrlItem.item.iSelectedImage=1;
	TreeCtrlItem.item.lParam =WideStockBaseInfomation;
	this->m_MyTree.InsertItem(&TreeCtrlItem);

	TreeCtrlItem.hParent = TVI_ROOT;
	TreeCtrlItem.hInsertAfter = TVI_LAST;
	TreeCtrlItem.item.mask = TVIF_TEXT | TVIF_PARAM|TVIF_IMAGE|TVIF_SELECTEDIMAGE  ;
	TreeCtrlItem.item.pszText = "除权数据";
    TreeCtrlItem.item.iImage=0;
	TreeCtrlItem.item.iSelectedImage=0;
	TreeCtrlItem.item.lParam =5000;
	hTreeItem1 = this->m_MyTree.InsertItem(&TreeCtrlItem);

	TreeCtrlItem.hParent = hTreeItem1;
	TreeCtrlItem.hInsertAfter = TVI_LAST;
	TreeCtrlItem.item.mask = TVIF_TEXT | TVIF_PARAM|TVIF_IMAGE|TVIF_SELECTEDIMAGE  ;
	TreeCtrlItem.item.pszText = "个股除权";
    TreeCtrlItem.item.iImage=1;
	TreeCtrlItem.item.iSelectedImage=1;
	TreeCtrlItem.item.lParam =WideSingleExRight;
	this->m_MyTree.InsertItem(&TreeCtrlItem);

	TreeCtrlItem.hParent = hTreeItem1;
	TreeCtrlItem.hInsertAfter = TVI_LAST;
	TreeCtrlItem.item.mask = TVIF_TEXT | TVIF_PARAM|TVIF_IMAGE|TVIF_SELECTEDIMAGE  ;
	TreeCtrlItem.item.pszText = "全部除权";
    TreeCtrlItem.item.iImage=1;
	TreeCtrlItem.item.iSelectedImage=1;
	TreeCtrlItem.item.lParam =WideExRight;
	this->m_MyTree.InsertItem(&TreeCtrlItem);

	TreeCtrlItem.hParent = TVI_ROOT;
	TreeCtrlItem.hInsertAfter = TVI_LAST;
	TreeCtrlItem.item.mask = TVIF_TEXT | TVIF_PARAM|TVIF_IMAGE|TVIF_SELECTEDIMAGE  ;
	TreeCtrlItem.item.pszText = "财务数据";
    TreeCtrlItem.item.iImage=0;
	TreeCtrlItem.item.iSelectedImage=0;
	TreeCtrlItem.item.lParam =6000;
	hTreeItem1 = this->m_MyTree.InsertItem(&TreeCtrlItem);

	TreeCtrlItem.hParent = hTreeItem1;
	TreeCtrlItem.hInsertAfter = TVI_LAST;
	TreeCtrlItem.item.mask = TVIF_TEXT | TVIF_PARAM|TVIF_IMAGE|TVIF_SELECTEDIMAGE  ;
	TreeCtrlItem.item.pszText = "个股财务";
    TreeCtrlItem.item.iImage=1;
	TreeCtrlItem.item.iSelectedImage=1;
	TreeCtrlItem.item.lParam =WideSingleCaiwuData;
	this->m_MyTree.InsertItem(&TreeCtrlItem);

	TreeCtrlItem.hParent = hTreeItem1;
	TreeCtrlItem.hInsertAfter = TVI_LAST;
	TreeCtrlItem.item.mask = TVIF_TEXT | TVIF_PARAM|TVIF_IMAGE|TVIF_SELECTEDIMAGE  ;
	TreeCtrlItem.item.pszText = "全部财务";
    TreeCtrlItem.item.iImage=1;
	TreeCtrlItem.item.iSelectedImage=1;
	TreeCtrlItem.item.lParam =WideCaiwuData;
	this->m_MyTree.InsertItem(&TreeCtrlItem);
}
BOOL CDownLoadNetDlg::InsertDownLoadDataToList(WIDEREQUEST_CONTEXT *pRequestContext )
{
	LV_ITEM  lvitem;
	int  iIcon, iItem, iSubItem, iActualItem;
    iItem=this->m_lstStockTypeName.GetItemCount();          //得到插入板块位置
	CString strIconDesc[6];
    pRequestContext->m_dwDownLoadCompleteCounts=0;
    pRequestContext->m_dwTimeCompleteCounts=0;
	switch(pRequestContext->m_DownLoadAttributes)
	{
	case WideTodayDayLine:              //当日日线
	case WideTodayAllDayLine:           //当日全部
    case WideHistoryDayLine:            //历史日线  
		 strIconDesc[0]="日线数据";
		 break;
    case WideFiveMinuteLine:            //五分钟K线数据 
    case WideAllFiveMinuteLine:         //全部五分钟K线数据 
	case WideHistoryMinuteLine:         //五分钟历史K线数据 
		 strIconDesc[0]="五分钟Κ线";
		 break;
	case WideTodayDetail:               //当天明细数据
	case WideAllTodayDetail:            //当天全部明细数据
	case WideHistoryDetail:             //历史明细数据
		 strIconDesc[0]="明细数据";
		 break;
	case WideSingleStockBaseInfomation: //个股资料
	case WideStockBaseInfomation:       //全部资料
		 strIconDesc[0]="基本资料";
		 break;
	case WideSingleExRight:             //除权数据
	case WideExRight:                   //全部除权数据
		 strIconDesc[0]="除权数据";
		 break;
	case WideSingleCaiwuData:           //财务数据 
	case WideCaiwuData:                  //全部财务数据 
		 strIconDesc[0]="财务数据";
		 break;
	}
    strIconDesc[1]=pRequestContext->m_strDownLoadName;
	switch(pRequestContext->m_dwDownLoadType)
	{
	case SHZS:                            //上证指数 
    case SHAG:                            //上证A股 
	case SHBG:                            //上证B股 
	case SHZQ:                            //上证债券
	case SHHG:                            //上证债券
	case SHZZ:                            //上证债券
	case SZZS:                            //深圳指数
	case SZAG:                            //深圳A股
	case SZBG:                            //深圳B股
	case SZZQ:                            //深圳债券
	case SZZZ:                            //深圳债券
	case SZHG:                            //深圳债券
	case SZETF:                            //深圳债券
	case SZSB:                            //深圳债券
	case SZQZ:                            //深圳债券
	case SZZXQY:                            //深圳债券
	case SZCYB:                            //二板指数   
	case QBGP:                            //全部股票
		strIconDesc[2]="内定板块";
		break;
    case CHOOSESTK:
		strIconDesc[2]="自选股板块";
		break;
	default:
		if(pRequestContext->m_dwDownLoadType<0)
		{
			strIconDesc[2]="股票";
		}
		else
		{
			strIconDesc[2]="自选板块";
		}
		break;
	}
	switch(pRequestContext->m_DownLoadAttributes)
	{
	case WideTodayDayLine:              //当日日线
		strIconDesc[3]="当日";
		break;
	case WideTodayAllDayLine:           //当日全部
		strIconDesc[3]="当日全部";
		break;
    case WideHistoryDayLine:            //历史日线  
		 strIconDesc[3]="历史";
		 break;
    case WideFiveMinuteLine:            //五分钟K线数据 
		strIconDesc[3]="当日";
		break;
    case WideAllFiveMinuteLine:         //全部五分钟K线数据 
		strIconDesc[3]="当日全部";
		break;
	case WideHistoryMinuteLine:         //五分钟历史K线数据 
		strIconDesc[3]="历史";
		break;
	case WideTodayDetail:               //当天明细数据
		strIconDesc[3]="当日";
		break;
	case WideAllTodayDetail:            //当天全部明细数据
		strIconDesc[3]="当日全部";
		break;
	case WideHistoryDetail:             //历史明细数据
		strIconDesc[3]=pRequestContext->m_strDownLoadDate;
		break;
	case WideSingleStockBaseInfomation: //个股资料
		strIconDesc[3]="当日最新";
		break;
	case WideStockBaseInfomation:       //全部资料
		strIconDesc[3]="当日最新";
		break;
	case WideSingleExRight:             //除权数据
		strIconDesc[3]="当日最新";
		break;
	case WideExRight:                   //全部除权数据
		strIconDesc[3]="当日最新";
		break;
	case WideSingleCaiwuData:           //财务数据 
		strIconDesc[3]="当日最新";
		break;
	case WideCaiwuData:                  //全部财务数据 
		strIconDesc[3]="当日最新";
		break;

	}
    strIconDesc[4].Format("%d",pRequestContext->m_dwDownLoadTotalCounts);
    strIconDesc[5].Format("%d",pRequestContext->m_dwDownLoadCompleteCounts);
	for (iSubItem = 0; iSubItem <6; iSubItem++)
	{
		if (iSubItem == 0)
		{
			iIcon = 0;  // choose the icon and legend for the entry
		}
		lvitem.mask = LVIF_TEXT | (iSubItem == 0? LVIF_IMAGE : 0);
		lvitem.iItem = (iSubItem == 0)? iItem : iActualItem;
		lvitem.iSubItem = iSubItem;

		// calculate the main and sub-item strings for the current item
        
		lvitem.pszText=strIconDesc[iSubItem].GetBuffer(strIconDesc[iSubItem].GetLength()); 
		lvitem.iImage = iIcon;
		if (iSubItem == 0)
			iActualItem =this->m_lstStockTypeName.InsertItem(&lvitem); // insert new item
		else
			this->m_lstStockTypeName.SetItem(&lvitem); // modify existing item (the sub-item text)
	} 
	m_lstStockTypeName.SetItemData( iItem, (DWORD)pRequestContext );
	return TRUE;
}

void CDownLoadNetDlg::PostNcDestroy() 
{
	// TODO: Add your specialized code here and/or call the base class
#ifdef WIDE_NET_VERSION
	if(m_SocketID.iFlag!=-1)
	   m_pMainFrm->m_pClientTransmitManageWnd->ReleaseID(this->m_SocketID);
#endif
	if(m_pTechWideNet) delete m_pTechWideNet;

	m_pDoc->m_pDownLoadDlg=NULL;
	if(m_pImageList) delete m_pImageList;
	if(m_pImageListSmall) delete m_pImageListSmall;
    char rmdirectory[255];
	SHFILEOPSTRUCT FileOp;
	FileOp.hwnd=NULL;
    FileOp.wFunc=FO_DELETE;
	FileOp.pFrom=rmdirectory;
	FileOp.pTo=NULL;
	FileOp.fFlags=FOF_FILESONLY|FOF_SILENT|FOF_NOCONFIRMATION  ;
	FileOp.fAnyOperationsAborted=NULL;
	FileOp.hNameMappings=NULL; 
    FileOp.lpszProgressTitle=NULL; 
	sprintf(rmdirectory,"%s\\Extract\\DownLoad\\*.*",m_szCurrentDir);
	rmdirectory[strlen(rmdirectory)]='\0';
	rmdirectory[strlen(rmdirectory)+1]='\0';
    SHFileOperation(&FileOp);

	sprintf(rmdirectory,"%s\\Extract\\*.*",m_szCurrentDir);
	rmdirectory[strlen(rmdirectory)]='\0';
	rmdirectory[strlen(rmdirectory)+1]='\0';
    SHFileOperation(&FileOp);

	sprintf(rmdirectory,"%s\\Extract\\sh\\*.*",m_szCurrentDir);
	rmdirectory[strlen(rmdirectory)]='\0';
	rmdirectory[strlen(rmdirectory)+1]='\0';
    SHFileOperation(&FileOp);

	sprintf(rmdirectory,"%s\\Extract\\sz\\*.*",m_szCurrentDir);
	rmdirectory[strlen(rmdirectory)]='\0';
	rmdirectory[strlen(rmdirectory)+1]='\0';
    SHFileOperation(&FileOp);

    delete this;
	CDialog::PostNcDestroy();
}

void CDownLoadNetDlg::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	int nCount=m_lstStockTypeName.GetItemCount();
	for( int i=0;i<nCount;i++)
	{
	    int ItemData=m_lstStockTypeName.GetItemData(i);
        WIDEREQUEST_CONTEXT *pRequestContext=(WIDEREQUEST_CONTEXT *)ItemData;
        if(pRequestContext->m_tDownLoadTimes)  
		{
			delete [] pRequestContext->m_tDownLoadTimes;
		}
		delete pRequestContext;
	}
	CDialog::OnClose();
    DestroyWindow();	
}

void CDownLoadNetDlg::OnOK() 
{
	// TODO: Add extra validation here
	int nCount=m_lstStockTypeName.GetItemCount();
	for( int i=0;i<nCount;i++)
	{
	    int ItemData=m_lstStockTypeName.GetItemData(i);
        WIDEREQUEST_CONTEXT *pRequestContext=(WIDEREQUEST_CONTEXT *)ItemData;
        if(pRequestContext->m_tDownLoadTimes)  
		{
			delete [] pRequestContext->m_tDownLoadTimes;
		}
		delete pRequestContext;
	}
	CDialog::OnOK();
    DestroyWindow();	
}
void CDownLoadNetDlg::OnCancel()
{
	CDialog::OnCancel();
    SendMessage(WM_CLOSE,0,0); 
}

void CDownLoadNetDlg::OnLoadCollecte() 
{
	// TODO: Add your control notification handler code here
	CString m_InFile;
	CString DefExt="*.ini";
	CString FileName="WideDownLoadSys.ini";
	CString Filter="power file (*.ini)|*.ini||";
	CString strdir;
	::GetCurrentDirectory(MAX_PATH,strdir.GetBuffer(MAX_PATH));

	FILE *fp;

	WIDEREQUEST_CONTEXT *pRequestContext;
	CFileDialog bOpenFileDialog(TRUE,(LPCSTR)DefExt,(LPCTSTR)FileName,
		OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT|OFN_NOCHANGEDIR,(LPCSTR)Filter,this);
	bOpenFileDialog.m_ofn.lpstrInitialDir=strdir;

	if( bOpenFileDialog.DoModal() ==IDOK )
		m_InFile=bOpenFileDialog.GetPathName();
	else
	{
	    PostMessage(WM_USER_POSTFOCUS,0,0);
		return;
	}
	if(_access(m_InFile.GetBuffer(0),0)==-1)
	{
		AfxMessageBox(m_InFile+"文件不存在");
	    PostMessage(WM_USER_POSTFOCUS,0,0);
	}
	else
	{
        fp=fopen(m_InFile.GetBuffer(0),"rb");
        if(fp!=NULL)
		{
			int nId=0;
			int nCount=0;
			fread(&nId,4,1,fp);
			if(nId!=INITDATAID)
			{
		        AfxMessageBox(m_InFile+"不是下载信息文件!");
				fclose(fp);
                return;
			}
			fread(&nCount,4,1,fp);
	
            for(int i=0;i<nCount;i++)
			{
				WideDownLoadAttributes DownLoadAttributes;     //下载数据类型
				char strDownLoadName[MAX_PATH];                //下载证券名称   
				int dwDownLoadType;                          //下载数据类型 
				char strDownLoadDate[MAX_PATH];                //下载数据时间选择
				fread(&DownLoadAttributes,1,sizeof(WideDownLoadAttributes),fp);
				fread(&dwDownLoadType,1,sizeof(DWORD),fp);
				fread(strDownLoadName,1,MAX_PATH,fp);
				fread(strDownLoadDate,1,MAX_PATH,fp);
                
//**********************************************************
				switch(DownLoadAttributes)
				{
					case WideTodayDayLine:              //当日日线
					case WideTodayAllDayLine:           //当日全部
					case WideHistoryDayLine:            //历史日线  
					case WideFiveMinuteLine:            //五分钟K线数据 
					case WideAllFiveMinuteLine:         //全部五分钟K线数据 
					case WideHistoryMinuteLine:         //五分钟历史K线数据 
					case WideTodayDetail:               //当天明细数据
					case WideAllTodayDetail:            //当天全部明细数据
					case WideSingleStockBaseInfomation: //个股资料
					case WideStockBaseInfomation:       //全部资料
					case WideSingleExRight:             //除权数据
					case WideExRight:                   //全部除权数据
					case WideSingleCaiwuData:           //财务数据 
					case WideCaiwuData:                  //全部财务数据 
						 if(!CheckList(DownLoadAttributes,dwDownLoadType,strDownLoadName))
						 {
							 pRequestContext=new WIDEREQUEST_CONTEXT;
							 memset(pRequestContext,0,sizeof(WIDEREQUEST_CONTEXT));
							 pRequestContext->m_DownLoadAttributes=DownLoadAttributes;
							 pRequestContext->m_dwDownLoadType=dwDownLoadType;
							 strcpy(pRequestContext->m_strDownLoadName,strDownLoadName);

							 pRequestContext->m_dwTimeCompleteCounts=0;
							 pRequestContext->m_dwDownLoadTotalCounts=GetStockMarketCounts(dwDownLoadType);
							 InsertDownLoadDataToList(pRequestContext );
						 }
						 break;
					case WideHistoryDetail:             //历史明细数据
						 if(!CheckList(WideHistoryDetail,dwDownLoadType,strDownLoadName))
						 {
							 time_t t1,t2;
							 pRequestContext=new WIDEREQUEST_CONTEXT;
							 memset(pRequestContext,0,sizeof(WIDEREQUEST_CONTEXT));
							 pRequestContext->m_DownLoadAttributes=WideHistoryDetail;
							 pRequestContext->m_dwDownLoadType=dwDownLoadType;
							 strcpy(pRequestContext->m_strDownLoadName,strDownLoadName);
							 strcpy(pRequestContext->m_strDownLoadDate,strDownLoadDate);
							 this->GetStartAndEndTime(t1,t2,strDownLoadDate);
							 GetHistoryDetailTimeCounts(t1,t2,pRequestContext);
							 pRequestContext->m_dwTimeCompleteCounts=0;
							 pRequestContext->m_dwDownLoadTotalCounts=GetStockMarketCounts(dwDownLoadType);
							 InsertDownLoadDataToList(pRequestContext );
						 }
						 break;
				}
			}
			fclose(fp);
		}
	}
    PostMessage(WM_USER_POSTFOCUS,0,0);
	
}

void CDownLoadNetDlg::OnSaveCollecte() 
{
	// TODO: Add your control notification handler code here
	CString m_InFile;
	CString DefExt="*.ini";
	CString FileName="WideDownLoadSys.ini";
	CString Filter="power file (*.ini)|*.ini||";
	CString strdir;
	::GetCurrentDirectory(MAX_PATH,strdir.GetBuffer(MAX_PATH));

	FILE *fp;
	CFileDialog bOpenFileDialog(FALSE,(LPCSTR)DefExt,(LPCTSTR)FileName,
		OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT|OFN_NOCHANGEDIR,(LPCSTR)Filter,this);
	bOpenFileDialog.m_ofn.lpstrInitialDir=strdir;

	if( bOpenFileDialog.DoModal() ==IDOK )
		m_InFile=bOpenFileDialog.GetPathName();
	else
		return;
    fp=fopen(m_InFile.GetBuffer(0),"w+b");
	if(fp!=NULL	)
	{
		 int nId=INITDATAID;
		 fwrite(&nId,1,4,fp);
		 int nCount=m_lstStockTypeName.GetItemCount();
		 fwrite(&nCount,1,4,fp);
	     for(int i=0;i<nCount;i++)
		 {
			 int ItemData=m_lstStockTypeName.GetItemData(i);
			 WIDEREQUEST_CONTEXT *pRequestContext=(WIDEREQUEST_CONTEXT *)ItemData;
			 if(pRequestContext!=NULL)
			 {
				 fwrite(&pRequestContext->m_DownLoadAttributes,1,sizeof(WideDownLoadAttributes),fp);
				 fwrite(&pRequestContext->m_dwDownLoadType,1,sizeof(int),fp);
				 fwrite(pRequestContext->m_strDownLoadName,1,MAX_PATH,fp);
				 fwrite(pRequestContext->m_strDownLoadDate,1,MAX_PATH,fp);
			 }
		 }
		fclose(fp);
	}
	PostMessage(WM_USER_POSTFOCUS,0,0);
}

void CDownLoadNetDlg::OnAddLoaddata() 
{
	// TODO: Add your control notification handler code here
    AddDownLoadData(); 
}

void CDownLoadNetDlg::OnDeleteLoaddata() 
{
	// TODO: Add your control notification handler code here
	int temp=m_lstStockTypeName.GetNextItem( -1, LVNI_SELECTED   ) ;
	if(temp==-1)
		return;
    int rtn = MessageBox("是否确实进行这一删除操作?","警告",MB_YESNO|MB_ICONWARNING);
    if(rtn!=6)
	  return;
	while(temp>=0)
	{
		WIDEREQUEST_CONTEXT *pRequestContext;
		pRequestContext=(WIDEREQUEST_CONTEXT *)m_lstStockTypeName.GetItemData(temp);
        delete pRequestContext;
		m_lstStockTypeName.DeleteItem(temp);
        temp=m_lstStockTypeName.GetNextItem( -1,LVNI_SELECTED   ) ;
	}
	PostMessage(WM_USER_POSTFOCUS,0,0);
}
void CDownLoadNetDlg::OnDownloadStart() 
{
	// TODO: Add your control notification handler code here
#ifdef WIDE_NET_VERSION
	if(!this->m_pMainFrm->m_bLogonSuccess)
	{
		AfxMessageBox("网络没有正常连接! 不能进行数据下载操作");
		this->m_bDownLoadState=FALSE;
		return;
	}
	if(m_pMainFrm->m_pClientTransmitManageWnd)
	{
		if(m_SocketID.iFlag==-1)
		{
             m_SocketID=m_pMainFrm->m_pClientTransmitManageWnd->RegisterID(this->m_hWnd);
		}
		if(m_SocketID.iFlag!=-1)
		{
		   AddDownLoadDataInfo(FALSE);
		   this->m_bDownLoadState=TRUE;
		}
	}
#endif
}

void CDownLoadNetDlg::OnStopdownload() 
{
	// TODO: Add your control notification handler code here
	this->m_bDownLoadState=FALSE;
#ifdef WIDE_NET_VERSION
    m_pMainFrm->m_pClientTransmitManageWnd->ReleaseID(this->m_SocketID);
	m_SocketID.iFlag=-1;
	m_SocketID.hWnd=NULL;
#endif
}

void CDownLoadNetDlg::OnSetupTime() 
{
	// TODO: Add your control notification handler code here
	int temp1=m_lstStockTypeName.GetNextItem( -1, LVNI_SELECTED   ) ;
	if(temp1==-1)
		return;
	time_t t1,t2;
	char strTime[MAX_PATH];
	WIDEREQUEST_CONTEXT *pRequestContext;
	pRequestContext=(WIDEREQUEST_CONTEXT *)m_lstStockTypeName.GetItemData(temp1);
    this->GetStartAndEndTime(t1,t2,pRequestContext->m_strDownLoadDate);
	CDownLoadNetTimeSet Dlg(t1,t2);
	if(Dlg.DoModal()==IDOK)
	{
		t1=Dlg.m_Start.GetTime();
		t2=Dlg.m_End.GetTime();
		this->GetTimeString(t1,t2,strTime);
		while(temp1>=0)
		{
			WIDEREQUEST_CONTEXT *pRequestContext;
			pRequestContext=(WIDEREQUEST_CONTEXT *)m_lstStockTypeName.GetItemData(temp1);
			if(pRequestContext->m_DownLoadAttributes==WideHistoryDetail)
			{
				strcpy(pRequestContext->m_strDownLoadDate,strTime);
				m_lstStockTypeName.SetItemText(temp1,3,strTime);
			    GetHistoryDetailTimeCounts(t1,t2,pRequestContext);
			}
			temp1=m_lstStockTypeName.GetNextItem( temp1,LVNI_SELECTED   ) ;
		}
	}
}
void CDownLoadNetDlg::AddDownLoadData() 
{
	// TODO: Add your control notification handler code here
    HTREEITEM hTreeItem;
	WIDEREQUEST_CONTEXT *pRequestContext;
	BOOL bOpenAddStockDlg=FALSE;
    hTreeItem=this->m_MyTree.GetSelectedItem();
	if(hTreeItem==NULL)
	{
		AfxMessageBox("请正确选择将要下载数据类型");
		return;
	}
	UINT nId=this->m_MyTree.GetItemData(hTreeItem); 
	switch(nId)
	{
	    case 1000:
		case WideTodayDayLine:              //当日日线
			 bOpenAddStockDlg=TRUE;
			 break;
		case WideTodayAllDayLine:           //当日全部
			 if(!CheckList(WideTodayAllDayLine,QBGP))
			 {
				 pRequestContext=new WIDEREQUEST_CONTEXT;
				 memset(pRequestContext,0,sizeof(WIDEREQUEST_CONTEXT));
				 pRequestContext->m_DownLoadAttributes=WideTodayAllDayLine;
                 pRequestContext->m_dwDownLoadType=QBGP;
                 strcpy(pRequestContext->m_strDownLoadName,"全部股票");
				 pRequestContext->m_dwTimeCompleteCounts=0;
				 pRequestContext->m_dwDownLoadTotalCounts=m_pDoc->m_sharesInformation.GetCount();
				 InsertDownLoadDataToList(pRequestContext );
			 }
			 else
			     AfxMessageBox("该任务已被添加");
			 break;
		case WideHistoryDayLine:            //历史日线  
			 bOpenAddStockDlg=TRUE;
			 break;
	    case 2000:
		case WideFiveMinuteLine:            //五分钟K线数据 
			 bOpenAddStockDlg=TRUE;
			 break;
		case WideAllFiveMinuteLine:         //全部五分钟K线数据 
			 if(!CheckList(WideAllFiveMinuteLine,QBGP))
			 {
				 pRequestContext=new WIDEREQUEST_CONTEXT;
				 memset(pRequestContext,0,sizeof(WIDEREQUEST_CONTEXT));
				 pRequestContext->m_DownLoadAttributes=WideAllFiveMinuteLine;
                 pRequestContext->m_dwDownLoadType=QBGP;
                 strcpy(pRequestContext->m_strDownLoadName,"全部股票");
				 pRequestContext->m_dwTimeCompleteCounts=0;
				 pRequestContext->m_dwDownLoadTotalCounts=m_pDoc->m_sharesInformation.GetCount();
				 InsertDownLoadDataToList(pRequestContext );
			 }
			 else
			     AfxMessageBox("该任务已被添加");
			 break;
		case WideHistoryMinuteLine:         //五分钟历史K线数据 
			 bOpenAddStockDlg=TRUE;
			 break;
	    case 3000:
		case WideTodayDetail:               //当天明细数据
			 bOpenAddStockDlg=TRUE;
			 break;
		case WideAllTodayDetail:            //当天全部明细数据
			 if(!CheckList(WideAllTodayDetail,QBGP))
			 {
				 pRequestContext=new WIDEREQUEST_CONTEXT;
				 memset(pRequestContext,0,sizeof(WIDEREQUEST_CONTEXT));
				 pRequestContext->m_DownLoadAttributes=WideAllTodayDetail;
                 pRequestContext->m_dwDownLoadType=QBGP;
                 strcpy(pRequestContext->m_strDownLoadName,"全部股票");
				 pRequestContext->m_dwTimeCompleteCounts=0;
				 pRequestContext->m_dwDownLoadTotalCounts=m_pDoc->m_sharesInformation.GetCount();
				 InsertDownLoadDataToList(pRequestContext );
			 }
			 else
			     AfxMessageBox("该任务已被添加");
			 break;
		case WideHistoryDetail:             //历史明细数据
			 bOpenAddStockDlg=TRUE;
			 break;
	    case 4000:
		case WideSingleStockBaseInfomation: //个股资料
			 bOpenAddStockDlg=TRUE;
			 break;
		case WideStockBaseInfomation:       //全部资料
			 if(!CheckList(WideStockBaseInfomation,QBGP))
			 {
				 pRequestContext=new WIDEREQUEST_CONTEXT;
				 memset(pRequestContext,0,sizeof(WIDEREQUEST_CONTEXT));
				 pRequestContext->m_DownLoadAttributes=WideStockBaseInfomation;
                 pRequestContext->m_dwDownLoadType=QBGP;
                 strcpy(pRequestContext->m_strDownLoadName,"全部股票");
				 pRequestContext->m_dwTimeCompleteCounts=0;
				 pRequestContext->m_dwDownLoadTotalCounts=m_pDoc->m_sharesInformation.GetCount();
				 InsertDownLoadDataToList(pRequestContext );
			 }
			 else
			     AfxMessageBox("该任务已被添加");
			 break;
	    case 5000:
		case WideSingleExRight:             //除权数据
			 bOpenAddStockDlg=TRUE;
			 break;
		case WideExRight:                   //全部除权数据
			 if(!CheckList(WideExRight,QBGP))
			 {
				 pRequestContext=new WIDEREQUEST_CONTEXT;
				 memset(pRequestContext,0,sizeof(WIDEREQUEST_CONTEXT));
				 pRequestContext->m_DownLoadAttributes=WideExRight;
                 pRequestContext->m_dwDownLoadType=QBGP;
                 strcpy(pRequestContext->m_strDownLoadName,"全部股票");
				 pRequestContext->m_dwTimeCompleteCounts=0;
				 pRequestContext->m_dwDownLoadTotalCounts=m_pDoc->m_sharesInformation.GetCount();
				 InsertDownLoadDataToList(pRequestContext );
			 }
			 else
			     AfxMessageBox("该任务已被添加");
			 break;
	    case 6000:
		case WideSingleCaiwuData:           //财务数据 
			 bOpenAddStockDlg=TRUE;
			 break;
		case WideCaiwuData:                  //全部财务数据 
			 if(!CheckList(WideCaiwuData,QBGP))
			 {
				 pRequestContext=new WIDEREQUEST_CONTEXT;
				 memset(pRequestContext,0,sizeof(WIDEREQUEST_CONTEXT));
				 pRequestContext->m_DownLoadAttributes=WideCaiwuData;
                 pRequestContext->m_dwDownLoadType=QBGP;
                 strcpy(pRequestContext->m_strDownLoadName,"全部股票");
				 pRequestContext->m_dwTimeCompleteCounts=0;
				 pRequestContext->m_dwDownLoadTotalCounts=m_pDoc->m_sharesInformation.GetCount();
				 InsertDownLoadDataToList(pRequestContext );
			 }
			 else
			     AfxMessageBox("该任务已被添加");
			 break;
	}
    if(bOpenAddStockDlg)
	{
		SelectStock pDlg(this,FALSE);
		pDlg.DoModal();
	}
	PostMessage(WM_USER_POSTFOCUS,0,0);
}
LRESULT CDownLoadNetDlg::OnPostFocus(WPARAM wParam, LPARAM lParam)
{
	int nCount=m_lstStockTypeName.GetItemCount();
	if(nCount>0)
	{
       m_lstStockTypeName.SetFocus();
	   m_lstStockTypeName.SetItemState(-1, ~LVIS_SELECTED, LVIS_SELECTED);
       m_lstStockTypeName.SetItemState(nCount-1,LVIS_FOCUSED|LVIS_SELECTED , LVIS_SELECTED|LVIS_FOCUSED) ;
       m_lstStockTypeName.EnsureVisible(nCount-1, FALSE);
	}
	int temp=m_lstStockTypeName.GetNextItem( -1, LVNI_SELECTED   ) ;
	if(temp==-1)
		return 0L;
	WIDEREQUEST_CONTEXT *pRequestContext;
	pRequestContext=(WIDEREQUEST_CONTEXT *)m_lstStockTypeName.GetItemData(temp);
	if(pRequestContext->m_DownLoadAttributes==WideHistoryDetail)
	{
		CWnd *pWnd=this->GetDlgItem(IDC_SETUP_TIME);
		pWnd->EnableWindow(TRUE);
	}
	else
	{
		CWnd *pWnd=this->GetDlgItem(IDC_SETUP_TIME);
		pWnd->EnableWindow(FALSE);
	}
	return 1L;
}
BOOL CDownLoadNetDlg::CheckList(WideDownLoadAttributes DownLoadAttributes,	int dwDownLoadType,
								 char *strDownLoadName)
{
	int nCount=m_lstStockTypeName.GetItemCount();
	for( int i=0;i<nCount;i++)
	{
	    int ItemData=m_lstStockTypeName.GetItemData(i);
        WIDEREQUEST_CONTEXT *pRequestContext=(WIDEREQUEST_CONTEXT *)ItemData;
		if(DownLoadAttributes!=pRequestContext->m_DownLoadAttributes)
			continue;
		if(dwDownLoadType!=pRequestContext->m_dwDownLoadType)
			continue;

		if(dwDownLoadType!=-1)
			return TRUE;
		if(strcmp(strDownLoadName,pRequestContext->m_strDownLoadName)==0)
			return TRUE;
		else
			continue;
	}
	return FALSE;
}

void CDownLoadNetDlg::OnDblclkSelectLoaddata(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	HTREEITEM  hItem = m_MyTree.GetSelectedItem();
	if(m_MyTree.ItemHasChildren(hItem))
	{
       return;
	}
    AddDownLoadData() ;
	*pResult = 0;
}
LRESULT CDownLoadNetDlg::OnAddStock(WPARAM wParam, LPARAM lParam)
{
	SHARES_DATA_ADD *pAddMxtj;
	int lsCount=(int)lParam;
    pAddMxtj=(SHARES_DATA_ADD *)wParam;
	HTREEITEM hTreeItem;
	WIDEREQUEST_CONTEXT *pRequestContext;
	hTreeItem=this->m_MyTree.GetSelectedItem();
	if(hTreeItem==NULL)
	{
		return 1L;
	}
	WideDownLoadAttributes nId;
	time_t tStartTime;
	time_t tEndTime;
	int TempnId=this->m_MyTree.GetItemData(hTreeItem); 
	if(TempnId==WideHistoryDetail)
	{
		CDownLoadNetTimeSet Dlg;
		if(Dlg.DoModal()==IDOK)
		{
            tStartTime=Dlg.m_Start.GetTime();
			tEndTime=Dlg.m_End.GetTime();
		}
	}
	switch(TempnId)
	{
	case 1000:
		nId=WideTodayDayLine; 
		break;
    case 2000:
		nId=WideFiveMinuteLine; 
		break;
	case 3000:
		nId=WideTodayDetail; 
		break;
	case 4000:
		nId=WideSingleStockBaseInfomation; 
		break;
	case 5000:
		nId=WideSingleExRight;
		break;
	case 6000:
		nId=WideSingleCaiwuData; 
		break;
	default:
		nId=( WideDownLoadAttributes )TempnId;
		break;
	}
	for(int i=0;i<lsCount;i++)
	{
		if(pAddMxtj[i].StockType<0)
		{
          if(CheckList(nId,pAddMxtj[i].StockType, pAddMxtj[i].name))
		  {
			  AfxMessageBox("该任务已被添加");
			  continue;
		  }
		}
		else
		{
          if(CheckList(nId,pAddMxtj[i].StockType))
		  {
			  AfxMessageBox("该任务已被添加");
			  continue;
		  }
		}
	    pRequestContext =new WIDEREQUEST_CONTEXT;
		memset(pRequestContext,0,sizeof(WIDEREQUEST_CONTEXT));
	    if(TempnId==WideHistoryDetail)
		{
			GetTimeString(tStartTime,tEndTime,pRequestContext->m_strDownLoadDate);
		    GetHistoryDetailTimeCounts(tStartTime,tEndTime,pRequestContext);
		}
		switch(pAddMxtj[i].StockType)
		{
			case SHZS:                            //上证指数
				pRequestContext->m_DownLoadAttributes=nId;
                pRequestContext->m_dwDownLoadType=pAddMxtj[i].StockType;
                strcpy(pRequestContext->m_strDownLoadName,pAddMxtj[i].name);
				pRequestContext->m_dwTimeCompleteCounts=0;
				pRequestContext->m_dwDownLoadTotalCounts=m_pDoc->m_sharesInformation.GetStockTypeCount(SHZS);
				InsertDownLoadDataToList(pRequestContext );
				break;
			case SHAG:                            //上证A股 
				pRequestContext->m_DownLoadAttributes=nId;
                pRequestContext->m_dwDownLoadType=pAddMxtj[i].StockType;
                strcpy(pRequestContext->m_strDownLoadName,pAddMxtj[i].name);
				pRequestContext->m_dwTimeCompleteCounts=0;
				pRequestContext->m_dwDownLoadTotalCounts=m_pDoc->m_sharesInformation.GetStockTypeCount(SHAG);
				InsertDownLoadDataToList(pRequestContext );
				break;
			case SHBG:                            //上证B股 
				pRequestContext->m_DownLoadAttributes=nId;
                pRequestContext->m_dwDownLoadType=pAddMxtj[i].StockType;
                strcpy(pRequestContext->m_strDownLoadName,pAddMxtj[i].name);
				pRequestContext->m_dwTimeCompleteCounts=0;
				pRequestContext->m_dwDownLoadTotalCounts=m_pDoc->m_sharesInformation.GetStockTypeCount(SHBG);
				InsertDownLoadDataToList(pRequestContext );
				break;
			case SHZQ:                            //上证债券
				pRequestContext->m_DownLoadAttributes=nId;
                pRequestContext->m_dwDownLoadType=pAddMxtj[i].StockType;
                strcpy(pRequestContext->m_strDownLoadName,pAddMxtj[i].name);
				pRequestContext->m_dwTimeCompleteCounts=0;
				pRequestContext->m_dwDownLoadTotalCounts=m_pDoc->m_sharesInformation.GetStockTypeCount(SHZQ);
				InsertDownLoadDataToList(pRequestContext );
				break;
			case SHZZ:                            //上证债券
				pRequestContext->m_DownLoadAttributes=nId;
				pRequestContext->m_dwDownLoadType=pAddMxtj[i].StockType;
				strcpy(pRequestContext->m_strDownLoadName,pAddMxtj[i].name);
				pRequestContext->m_dwTimeCompleteCounts=0;
				pRequestContext->m_dwDownLoadTotalCounts=m_pDoc->m_sharesInformation.GetStockTypeCount(SHZZ);
				InsertDownLoadDataToList(pRequestContext );
				break;
			case SHHG:                            //上证债券
				pRequestContext->m_DownLoadAttributes=nId;
				pRequestContext->m_dwDownLoadType=pAddMxtj[i].StockType;
				strcpy(pRequestContext->m_strDownLoadName,pAddMxtj[i].name);
				pRequestContext->m_dwTimeCompleteCounts=0;
				pRequestContext->m_dwDownLoadTotalCounts=m_pDoc->m_sharesInformation.GetStockTypeCount(SHHG);
				InsertDownLoadDataToList(pRequestContext );
				break;
			case SHOT:                            //上证债券
				pRequestContext->m_DownLoadAttributes=nId;
				pRequestContext->m_dwDownLoadType=pAddMxtj[i].StockType;
				strcpy(pRequestContext->m_strDownLoadName,pAddMxtj[i].name);
				pRequestContext->m_dwTimeCompleteCounts=0;
				pRequestContext->m_dwDownLoadTotalCounts=m_pDoc->m_sharesInformation.GetStockTypeCount(SHOT);
				InsertDownLoadDataToList(pRequestContext );
				break;
			case SZZS:                            //深圳指数
				pRequestContext->m_DownLoadAttributes=nId;
                pRequestContext->m_dwDownLoadType=pAddMxtj[i].StockType;
                strcpy(pRequestContext->m_strDownLoadName,pAddMxtj[i].name);
				pRequestContext->m_dwTimeCompleteCounts=0;
				pRequestContext->m_dwDownLoadTotalCounts=m_pDoc->m_sharesInformation.GetStockTypeCount(SZZS);
				InsertDownLoadDataToList(pRequestContext );
				break;
			case SZAG:                            //深圳A股
				pRequestContext->m_DownLoadAttributes=nId;
                pRequestContext->m_dwDownLoadType=pAddMxtj[i].StockType;
                strcpy(pRequestContext->m_strDownLoadName,pAddMxtj[i].name);
				pRequestContext->m_dwTimeCompleteCounts=0;
				pRequestContext->m_dwDownLoadTotalCounts=m_pDoc->m_sharesInformation.GetStockTypeCount(SZAG);
				InsertDownLoadDataToList(pRequestContext );
				break;
			case SZBG:                            //深圳B股
				pRequestContext->m_DownLoadAttributes=nId;
                pRequestContext->m_dwDownLoadType=pAddMxtj[i].StockType;
                strcpy(pRequestContext->m_strDownLoadName,pAddMxtj[i].name);
				pRequestContext->m_dwTimeCompleteCounts=0;
				pRequestContext->m_dwDownLoadTotalCounts=m_pDoc->m_sharesInformation.GetStockTypeCount(SZBG);
				InsertDownLoadDataToList(pRequestContext );
				break;
			case SZZQ:                            //深圳债券
				pRequestContext->m_DownLoadAttributes=nId;
                pRequestContext->m_dwDownLoadType=pAddMxtj[i].StockType;
                strcpy(pRequestContext->m_strDownLoadName,pAddMxtj[i].name);
				pRequestContext->m_dwTimeCompleteCounts=0;
				pRequestContext->m_dwDownLoadTotalCounts=m_pDoc->m_sharesInformation.GetStockTypeCount(SZZQ);
				InsertDownLoadDataToList(pRequestContext );
				break;
			case SZHG:                            //深圳债券
				pRequestContext->m_DownLoadAttributes=nId;
				pRequestContext->m_dwDownLoadType=pAddMxtj[i].StockType;
				strcpy(pRequestContext->m_strDownLoadName,pAddMxtj[i].name);
				pRequestContext->m_dwTimeCompleteCounts=0;
				pRequestContext->m_dwDownLoadTotalCounts=m_pDoc->m_sharesInformation.GetStockTypeCount(SZHG);
				InsertDownLoadDataToList(pRequestContext );
				break;
			case SZZZ:                            //深圳债券
				pRequestContext->m_DownLoadAttributes=nId;
				pRequestContext->m_dwDownLoadType=pAddMxtj[i].StockType;
				strcpy(pRequestContext->m_strDownLoadName,pAddMxtj[i].name);
				pRequestContext->m_dwTimeCompleteCounts=0;
				pRequestContext->m_dwDownLoadTotalCounts=m_pDoc->m_sharesInformation.GetStockTypeCount(SZZZ);
				InsertDownLoadDataToList(pRequestContext );
				break;
			case SZETF:                            //深圳债券
				pRequestContext->m_DownLoadAttributes=nId;
				pRequestContext->m_dwDownLoadType=pAddMxtj[i].StockType;
				strcpy(pRequestContext->m_strDownLoadName,pAddMxtj[i].name);
				pRequestContext->m_dwTimeCompleteCounts=0;
				pRequestContext->m_dwDownLoadTotalCounts=m_pDoc->m_sharesInformation.GetStockTypeCount(SZETF);
				InsertDownLoadDataToList(pRequestContext );
				break;
			case SZQZ:                            //深圳债券
				pRequestContext->m_DownLoadAttributes=nId;
				pRequestContext->m_dwDownLoadType=pAddMxtj[i].StockType;
				strcpy(pRequestContext->m_strDownLoadName,pAddMxtj[i].name);
				pRequestContext->m_dwTimeCompleteCounts=0;
				pRequestContext->m_dwDownLoadTotalCounts=m_pDoc->m_sharesInformation.GetStockTypeCount(SZQZ);
				InsertDownLoadDataToList(pRequestContext );
				break;
			case SZSB:                            //深圳债券
				pRequestContext->m_DownLoadAttributes=nId;
				pRequestContext->m_dwDownLoadType=pAddMxtj[i].StockType;
				strcpy(pRequestContext->m_strDownLoadName,pAddMxtj[i].name);
				pRequestContext->m_dwTimeCompleteCounts=0;
				pRequestContext->m_dwDownLoadTotalCounts=m_pDoc->m_sharesInformation.GetStockTypeCount(SZSB);
				InsertDownLoadDataToList(pRequestContext );
				break;
			case SZKF:                            //深圳债券
				pRequestContext->m_DownLoadAttributes=nId;
				pRequestContext->m_dwDownLoadType=pAddMxtj[i].StockType;
				strcpy(pRequestContext->m_strDownLoadName,pAddMxtj[i].name);
				pRequestContext->m_dwTimeCompleteCounts=0;
				pRequestContext->m_dwDownLoadTotalCounts=m_pDoc->m_sharesInformation.GetStockTypeCount(SZKF);
				InsertDownLoadDataToList(pRequestContext );
				break;
			case SZOT:                            //深圳债券
				pRequestContext->m_DownLoadAttributes=nId;
				pRequestContext->m_dwDownLoadType=pAddMxtj[i].StockType;
				strcpy(pRequestContext->m_strDownLoadName,pAddMxtj[i].name);
				pRequestContext->m_dwTimeCompleteCounts=0;
				pRequestContext->m_dwDownLoadTotalCounts=m_pDoc->m_sharesInformation.GetStockTypeCount(SZOT);
				InsertDownLoadDataToList(pRequestContext );
				break;
			case SZZXQY:                            //深圳债券
				pRequestContext->m_DownLoadAttributes=nId;
				pRequestContext->m_dwDownLoadType=pAddMxtj[i].StockType;
				strcpy(pRequestContext->m_strDownLoadName,pAddMxtj[i].name);
				pRequestContext->m_dwTimeCompleteCounts=0;
				pRequestContext->m_dwDownLoadTotalCounts=m_pDoc->m_sharesInformation.GetStockTypeCount(SZZXQY);
				InsertDownLoadDataToList(pRequestContext );
				break;
			case SZCYB:                            //二板指数   
				pRequestContext->m_DownLoadAttributes=nId;
                pRequestContext->m_dwDownLoadType=pAddMxtj[i].StockType;
                strcpy(pRequestContext->m_strDownLoadName,pAddMxtj[i].name);
				pRequestContext->m_dwTimeCompleteCounts=0;
				pRequestContext->m_dwDownLoadTotalCounts=m_pDoc->m_sharesInformation.GetStockTypeCount(SZCYB);
				InsertDownLoadDataToList(pRequestContext );
				break;
			case CHOOSESTK:
				{
					 SymbolKindArr l_StockArray;
					 m_pDoc->m_ManagerBlockData.GetChooseStockCode(l_StockArray);
					 pRequestContext->m_DownLoadAttributes=nId;
					 pRequestContext->m_dwDownLoadType=pAddMxtj[i].StockType;
					 strcpy(pRequestContext->m_strDownLoadName,"自选板块");
					 pRequestContext->m_dwTimeCompleteCounts=0;
					 pRequestContext->m_dwDownLoadTotalCounts=l_StockArray.GetSize();
					 InsertDownLoadDataToList(pRequestContext );
				}
				break;
			case BLOCKINDEX:
				{
			         AfxMessageBox("该数据下载任务不能被添加!");
				}
				break;
			default:
				{
					if(pAddMxtj[i].StockType<0)
					{
						 pRequestContext->m_DownLoadAttributes=nId;
						 pRequestContext->m_dwDownLoadType=pAddMxtj[i].StockType;
						 strcpy(pRequestContext->m_strDownLoadName,pAddMxtj[i].name);
						 pRequestContext->m_dwTimeCompleteCounts=0;
						 pRequestContext->m_dwDownLoadTotalCounts=1;
						 InsertDownLoadDataToList(pRequestContext );
					}
					else
					{
						 SymbolKindArr l_StockCodeArray;
						 CStringArray l_StockTypeNameArray;
						 m_pDoc->m_ManagerBlockData.GetAllBlockNames(l_StockTypeNameArray);
						 CString l_sStockTypeName=l_StockTypeNameArray.GetAt(pAddMxtj[i].StockType-1000);
						 m_pDoc->m_ManagerBlockData.GetStockFromBlockName(l_StockCodeArray,l_sStockTypeName.GetBuffer(0));          
						 pRequestContext->m_DownLoadAttributes=nId;
						 pRequestContext->m_dwDownLoadType=pAddMxtj[i].StockType;
						 strcpy(pRequestContext->m_strDownLoadName,l_sStockTypeName.GetBuffer(0));
						 pRequestContext->m_dwTimeCompleteCounts=0;
						 pRequestContext->m_dwDownLoadTotalCounts=l_StockCodeArray.GetSize();
						 InsertDownLoadDataToList(pRequestContext );
					}
				}
				break;
		}
	}
	return 0L;
}
int CDownLoadNetDlg::GetStockMarketCounts(int DownLoadType)
{
		switch(DownLoadType)
		{
			case SHZS:                            //上证指数
			case SHAG:                            //上证A股 
			case SHBG:                            //上证B股 
			case SHZQ:                            //上证债券
			case SHZZ:                            //上证债券
			case SHHG:                            //上证债券
			case SHOT:                            //上证债券
			case SZAG:                            //深圳A股
			case SZBG:                            //深圳B股
			case SZZQ:                            //深圳债券
			case SZZZ:                            //深圳债券
			case SZHG:                            //深圳债券
			case SZETF:                           //深圳债券
			case SZKF:                      //深圳债券
			case SZQZ:                           //深圳债券
			case SZSB:                           //深圳债券
			case SZOT:                           //深圳债券
			case SZZXQY:                           //深圳债券
			case SZCYB:                            //二板指数   
				return m_pDoc->m_sharesInformation.GetStockTypeCount(DownLoadType);
				break;
            case QBGP:
				return m_pDoc->m_sharesInformation.GetCount();
				break;
			case CHOOSESTK:
				{
					 SymbolKindArr l_StockArray;
					 m_pDoc->m_ManagerBlockData.GetChooseStockCode(l_StockArray);
					 return l_StockArray.GetSize();
				}
				break;
			case BLOCKINDEX:
				{
			         AfxMessageBox("该数据下载任务不能被添加!");
				}
				break;
			default:
				{
					if(DownLoadType<0)
					{
						return 1;
					}
					else
					{
						 SymbolKindArr l_StockCodeArray;
						 CStringArray l_StockTypeNameArray;
						 m_pDoc->m_ManagerBlockData.GetAllBlockNames(l_StockTypeNameArray);
						 CString l_sStockTypeName=l_StockTypeNameArray.GetAt(DownLoadType -1000);
						 m_pDoc->m_ManagerBlockData.GetStockFromBlockName(l_StockCodeArray,l_sStockTypeName.GetBuffer(0));          
						 return l_StockCodeArray.GetSize();
					}
				}
				break;
         }
         return 0;
}

void CDownLoadNetDlg::OnClickList1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	
	*pResult = 0;
	int temp=m_lstStockTypeName.GetNextItem( -1, LVNI_SELECTED   ) ;
	BOOL isWideHistoryDetail=TRUE;
	if(temp==-1)
		return;
	if((GetKeyState(VK_SHIFT) & (1 << (sizeof(SHORT)*8-1))) != 0)
	{
		while(temp>=0)
		{
			WIDEREQUEST_CONTEXT *pRequestContext;
			pRequestContext=(WIDEREQUEST_CONTEXT *)m_lstStockTypeName.GetItemData(temp);
		    if(pRequestContext->m_DownLoadAttributes!=WideHistoryDetail)
			{
				isWideHistoryDetail=FALSE;
				break;
			}
			temp=m_lstStockTypeName.GetNextItem( temp,LVNI_SELECTED   ) ;
		}
		if(isWideHistoryDetail)
		{
			CWnd *pWnd=this->GetDlgItem(IDC_SETUP_TIME);
			pWnd->EnableWindow(TRUE);
		}
		else
		{
			CWnd *pWnd=this->GetDlgItem(IDC_SETUP_TIME);
			pWnd->EnableWindow(FALSE);
		}
	}
	else
	{
		WIDEREQUEST_CONTEXT *pRequestContext;
		pRequestContext=(WIDEREQUEST_CONTEXT *)m_lstStockTypeName.GetItemData(temp);
		if(pRequestContext->m_DownLoadAttributes==WideHistoryDetail)
		{
			CWnd *pWnd=this->GetDlgItem(IDC_SETUP_TIME);
			pWnd->EnableWindow(TRUE);
		}
		else
		{
			CWnd *pWnd=this->GetDlgItem(IDC_SETUP_TIME);
			pWnd->EnableWindow(FALSE);
		}
	}
}

void CDownLoadNetDlg::OnDblclkList1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
    BOOL isWideHistoryDetail=TRUE;
	int temp=m_lstStockTypeName.GetNextItem( -1, LVNI_SELECTED   ) ;
	if(temp==-1)
		return;
	while(temp>=0)
	{
		WIDEREQUEST_CONTEXT *pRequestContext;
		pRequestContext=(WIDEREQUEST_CONTEXT *)m_lstStockTypeName.GetItemData(temp);
		if(pRequestContext->m_DownLoadAttributes!=WideHistoryDetail)
		{
			isWideHistoryDetail=FALSE;
			break;
		}
		temp=m_lstStockTypeName.GetNextItem( temp,LVNI_SELECTED   ) ;
	}
    if(isWideHistoryDetail)
	{
	    int temp1=m_lstStockTypeName.GetNextItem( -1, LVNI_SELECTED   ) ;
		time_t t1,t2;
		char strTime[MAX_PATH];
		WIDEREQUEST_CONTEXT *pRequestContext;
		pRequestContext=(WIDEREQUEST_CONTEXT *)m_lstStockTypeName.GetItemData(temp1);
        this->GetStartAndEndTime(t1,t2,pRequestContext->m_strDownLoadDate);
		CDownLoadNetTimeSet Dlg(t1,t2);
		if(Dlg.DoModal()==IDOK)
		{
			t1=Dlg.m_Start.GetTime();
			t2=Dlg.m_End.GetTime();
			this->GetTimeString(t1,t2,strTime);
			while(temp1>=0)
			{
				WIDEREQUEST_CONTEXT *pRequestContext;
				pRequestContext=(WIDEREQUEST_CONTEXT *)m_lstStockTypeName.GetItemData(temp1);
				if(pRequestContext->m_DownLoadAttributes==WideHistoryDetail)
				{
					strcpy(pRequestContext->m_strDownLoadDate,strTime);
					m_lstStockTypeName.SetItemText(temp1,3,strTime);
			        GetTimeString(t1,t2,pRequestContext->m_strDownLoadDate);
					this->GetHistoryDetailTimeCounts(t1,t2,pRequestContext);
				}
				temp1=m_lstStockTypeName.GetNextItem( temp1,LVNI_SELECTED   ) ;
			}
		}
	}
	*pResult = 0;
}

void CDownLoadNetDlg::OnKeydownList1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_KEYDOWN* pLVKeyDow = (LV_KEYDOWN*)pNMHDR;
	// TODO: Add your control notification handler code here
	if(pLVKeyDow->wVKey==VK_DOWN)
	{
		int temp=m_lstStockTypeName.GetNextItem( -1, LVNI_SELECTED   ) ;
		if(temp==-1)
			return;
		if(temp==m_lstStockTypeName.GetItemCount()-1)
			return;
		temp++;
		WIDEREQUEST_CONTEXT *pRequestContext;
		pRequestContext=(WIDEREQUEST_CONTEXT *)m_lstStockTypeName.GetItemData(temp);
		if(pRequestContext->m_DownLoadAttributes==WideHistoryDetail)
		{
			CWnd *pWnd=this->GetDlgItem(IDC_SETUP_TIME);
			pWnd->EnableWindow(TRUE);
		}
		else
		{
			CWnd *pWnd=this->GetDlgItem(IDC_SETUP_TIME);
			pWnd->EnableWindow(FALSE);
		}
		return ;
	}
	if(pLVKeyDow->wVKey==VK_UP)
	{
		int temp=m_lstStockTypeName.GetNextItem( -1, LVNI_SELECTED   ) ;
		if(temp==-1)
			return;
		if(temp==0)
			return;
		temp-=1;
		WIDEREQUEST_CONTEXT *pRequestContext;
		pRequestContext=(WIDEREQUEST_CONTEXT *)m_lstStockTypeName.GetItemData(temp);
		if(pRequestContext->m_DownLoadAttributes==WideHistoryDetail)
		{
			CWnd *pWnd=this->GetDlgItem(IDC_SETUP_TIME);
			pWnd->EnableWindow(TRUE);
		}
		else
		{
			CWnd *pWnd=this->GetDlgItem(IDC_SETUP_TIME);
			pWnd->EnableWindow(FALSE);
		}
		return ;
	}
	if(pLVKeyDow->wVKey==VK_END)
	{
		WIDEREQUEST_CONTEXT *pRequestContext;
		pRequestContext=(WIDEREQUEST_CONTEXT *)m_lstStockTypeName.GetItemData(m_lstStockTypeName.GetItemCount()-1);
		if(pRequestContext->m_DownLoadAttributes==WideHistoryDetail)
		{
			CWnd *pWnd=this->GetDlgItem(IDC_SETUP_TIME);
			pWnd->EnableWindow(TRUE);
		}
		else
		{
			CWnd *pWnd=this->GetDlgItem(IDC_SETUP_TIME);
			pWnd->EnableWindow(FALSE);
		}
		return ;
	}
	if(pLVKeyDow->wVKey==VK_HOME)
	{
		WIDEREQUEST_CONTEXT *pRequestContext;
		pRequestContext=(WIDEREQUEST_CONTEXT *)m_lstStockTypeName.GetItemData(0);
		if(pRequestContext->m_DownLoadAttributes==WideHistoryDetail)
		{
			CWnd *pWnd=this->GetDlgItem(IDC_SETUP_TIME);
			pWnd->EnableWindow(TRUE);
		}
		else
		{
			CWnd *pWnd=this->GetDlgItem(IDC_SETUP_TIME);
			pWnd->EnableWindow(FALSE);
		}
		return ;
	}

	*pResult = 0;
}
void CDownLoadNetDlg::GetTimeString(time_t t1,time_t t2,char *strTime)
{
    CString strT1,strT2,strT;
	CTime m_Time = t1;
    long lt=((long)m_Time.GetYear())*10000L+(long)(m_Time.GetDay())+(long)(m_Time.GetMonth())*100;
	strT1.Format("%ld",lt);

	m_Time = t2;
    lt=((long)m_Time.GetYear())*10000L+(long)(m_Time.GetDay())+(long)(m_Time.GetMonth())*100;
	strT2.Format("%ld",lt);

	strT=strT1;
	strT+="-";
	strT+=strT2;
	strcpy(strTime,strT.GetBuffer(0));
}
void CDownLoadNetDlg::GetStartAndEndTime(time_t &t1,time_t &t2,char *strTime)
{
	CString strT=strTime;
	CTime m_ctime1(atol(strT.Left(4)),atol(strT.Mid(4,2)),atol(strT.Mid(6,2)),12,0,0);
    t1=m_ctime1.GetTime();
	CTime m_ctime2(atol(strT.Mid(9,4)),atol(strT.Mid(13,2)),atol(strT.Right(2)),12,0,0);
    t2=m_ctime2.GetTime();
}
void CDownLoadNetDlg::GetHistoryDetailTimeCounts(time_t t1,time_t t2,PWIDEREQUEST_CONTEXT &pRequestContext)
{
	CTimeSpan  lDaySpan(1, 0,0, 0 );
	int index=0;
    CTimeSpan  nCountSpan(t2-t1);
	int nDays=nCountSpan.GetDays()+2;
	if(pRequestContext->m_dwTimeTotalCounts!=0&&pRequestContext->m_tDownLoadTimes!=NULL)
	{
         delete [] pRequestContext->m_tDownLoadTimes;
		 pRequestContext->m_tDownLoadTimes=NULL;
		 pRequestContext->m_dwTimeTotalCounts=0;
         pRequestContext->m_dwTimeCompleteCounts=0; 
	}
    pRequestContext->m_tDownLoadTimes = (time_t *)new long [nDays]; 
	time_t tTemp=t1;
	while(tTemp<=t2)
	{ 
		CTime ctTime(tTemp);
        if(ctTime.GetDayOfWeek()!=1&&ctTime.GetDayOfWeek()!=7)
		{
			pRequestContext->m_tDownLoadTimes[index++]=tTemp;
		}
        tTemp +=lDaySpan.GetTotalSeconds(); 
	}
    pRequestContext->m_dwTimeTotalCounts=index;
}
void CDownLoadNetDlg::AddDownLoadDataInfo(BOOL bIsSendMsg)
{
    if(!this->m_bDownLoadState)
		 return;
	int nCount=m_lstStockTypeName.GetItemCount();
//计算已下载个数
	if(bIsSendMsg)
	{
		for( int i=0;i<nCount;i++)
		{
			int ItemData=m_lstStockTypeName.GetItemData(i);
			WIDEREQUEST_CONTEXT *pRequestContext=(WIDEREQUEST_CONTEXT *)ItemData;
			if(pRequestContext->m_dwDownLoadTotalCounts==
				pRequestContext->m_dwDownLoadCompleteCounts)
				continue;
			if(pRequestContext->m_DownLoadAttributes!=WideHistoryDetail)
			{
				pRequestContext->m_dwDownLoadCompleteCounts++;
				break;
			}else
			{
				if(pRequestContext->m_dwTimeTotalCounts!=pRequestContext->m_dwTimeCompleteCounts)
				{
					pRequestContext->m_dwTimeCompleteCounts++;
					break;
				}
				else
				{
					pRequestContext->m_dwDownLoadCompleteCounts++;
                    pRequestContext->m_dwTimeCompleteCounts=0;
					break;
				}
			}
		}
	}
//计算并得到下载股票代码
	BOOL IsAddDownLoadData=FALSE;
	for( int i=0;i<nCount;i++)
	{
	    int ItemData=m_lstStockTypeName.GetItemData(i);
        WIDEREQUEST_CONTEXT *pRequestContext=(WIDEREQUEST_CONTEXT *)ItemData;
        do
		{
			if(pRequestContext->m_dwDownLoadTotalCounts==
				pRequestContext->m_dwDownLoadCompleteCounts)
				break;
			CReportData *pCdat;
			if(pRequestContext->m_dwDownLoadType<0)
			{
				pCdat=GetCdatPointFromStockTypeAndPos(
					pRequestContext->m_strDownLoadName,
					pRequestContext->m_dwDownLoadType,
					pRequestContext->m_dwDownLoadCompleteCounts);
				if(pCdat==NULL)
				{
					 pRequestContext->m_dwDownLoadCompleteCounts++;
					 break;
				}
			}
			else
			{
				int tempStockCount=pRequestContext->m_dwDownLoadCompleteCounts;
				for(int i=tempStockCount;i<pRequestContext->m_dwDownLoadTotalCounts;i++)
				{
					pCdat=GetCdatPointFromStockTypeAndPos(
						pRequestContext->m_strDownLoadName,
						pRequestContext->m_dwDownLoadType,i);
					if(pCdat==NULL)
						 continue;
					else
						break;
				}
				pRequestContext->m_dwDownLoadCompleteCounts=i; 
				if(pCdat==NULL)
					break;
			}
			switch(pRequestContext->m_DownLoadAttributes)
			{
				case WideTodayDayLine:              //当日日线
				case WideTodayAllDayLine:           //当日全部
					 m_TransPackageStruct.m_TransmitType=HistoryDayLineData;
					 m_TransPackageStruct.m_dwTransmitStockType=-1;
					 strcpy(m_TransmitStockAttribute.m_szTransmitStockLabel,pCdat->id);
					 m_TransmitStockAttribute.m_dwTransmitCounts=1;
					 m_TransmitStockAttribute.m_wMarket=m_pDoc->m_sharesCompute.GetMarketKind(pCdat->kind);
					 m_TransPackageStruct.m_pTransmitStockAttribute=&m_TransmitStockAttribute;
					 m_TransPackageStruct.m_dwTransmitStockCount=1;
					 IsAddDownLoadData=TRUE;
					 break;
				case WideHistoryDayLine:            //历史日线  
					{
						 if(!m_pTechWideNet)
						 {
							 IsAddDownLoadData=FALSE;
							 break;
						 }
						 int KLineCount=m_pTechWideNet->GetRequestKlineCount(pCdat->id,pCdat->kind,TRUE);
						 if(KLineCount>=0)
						 {
							 m_TransPackageStruct.m_TransmitType=HistoryDayLineData;
							 m_TransPackageStruct.m_dwTransmitStockType=-1;
							 strcpy(m_TransmitStockAttribute.m_szTransmitStockLabel,pCdat->id);
							 m_TransmitStockAttribute.m_dwTransmitCounts=KLineCount;
							 m_TransPackageStruct.m_pTransmitStockAttribute=&m_TransmitStockAttribute;
							 m_TransPackageStruct.m_dwTransmitStockCount=1;
							 m_TransmitStockAttribute.m_wMarket=m_pDoc->m_sharesCompute.GetMarketKind(pCdat->kind);
							 IsAddDownLoadData=TRUE;
						 }
						 else
						 {
							 IsAddDownLoadData=FALSE;
						 }
					}				 
					 break;
				case WideFiveMinuteLine:            //五分钟K线数据 
				case WideAllFiveMinuteLine:         //全部五分钟K线数据 
					 m_TransPackageStruct.m_TransmitType=HistoryMinuteLineData;
					 m_TransPackageStruct.m_dwTransmitStockType=-1;
					 strcpy(m_TransmitStockAttribute.m_szTransmitStockLabel,pCdat->id);
					 m_TransmitStockAttribute.m_dwTransmitCounts=48;
					 m_TransPackageStruct.m_pTransmitStockAttribute=&m_TransmitStockAttribute;
					 m_TransmitStockAttribute.m_wMarket=m_pDoc->m_sharesCompute.GetMarketKind(pCdat->kind);
					 m_TransPackageStruct.m_dwTransmitStockCount=1;
					 IsAddDownLoadData=TRUE;
					 break;
				case WideHistoryMinuteLine:         //五分钟历史K线数据 
					{
						 if(!m_pTechWideNet)
						 {
							 IsAddDownLoadData=FALSE;
							 break;
						 }
						 int KLineCount=m_pTechWideNet->GetRequestKlineCount(pCdat->id,pCdat->kind,FALSE);
						 if(KLineCount>=0)
						 {
							 m_TransPackageStruct.m_TransmitType=HistoryMinuteLineData;
							 m_TransPackageStruct.m_dwTransmitStockType=-1;
							 strcpy(m_TransmitStockAttribute.m_szTransmitStockLabel,pCdat->id);
							 m_TransmitStockAttribute.m_dwTransmitCounts=KLineCount;
							 m_TransmitStockAttribute.m_wMarket=m_pDoc->m_sharesCompute.GetMarketKind(pCdat->kind);
							 m_TransPackageStruct.m_pTransmitStockAttribute=&m_TransmitStockAttribute;
							 m_TransPackageStruct.m_dwTransmitStockCount=1;
							 IsAddDownLoadData=TRUE;
						 }
						 else
						 {
							 IsAddDownLoadData=FALSE;
						 }
					}				 
					 break;
				case WideTodayDetail:               //当天明细数据
				case WideAllTodayDetail:            //当天全部明细数据
					 m_TransPackageStruct.m_TransmitType=StockDisPartData;
					 m_TransPackageStruct.m_dwTransmitStockType=-1;
					 strcpy(m_TransmitStockAttribute.m_szTransmitStockLabel,pCdat->id);
					 m_TransmitStockAttribute.m_dwTransmitCounts=0;
					 m_TransmitStockAttribute.m_wMarket=m_pDoc->m_sharesCompute.GetMarketKind(pCdat->kind);
					 m_TransPackageStruct.m_pTransmitStockAttribute=&m_TransmitStockAttribute;
					 m_TransPackageStruct.m_dwTransmitStockCount=1;
					 IsAddDownLoadData=TRUE;
					 break;
				case WideSingleStockBaseInfomation: //个股资料
				case WideStockBaseInfomation:       //全部资料
					 m_TransPackageStruct.m_TransmitType=WideStockBaseInfo;
					 m_TransPackageStruct.m_dwTransmitStockType=-1;
					 strcpy(m_TransmitStockAttribute.m_szTransmitStockLabel,pCdat->id);
					 m_TransmitStockAttribute.m_dwTransmitCounts=1;
					 m_TransmitStockAttribute.m_wMarket=m_pDoc->m_sharesCompute.GetMarketKind(pCdat->kind);
					 m_TransPackageStruct.m_pTransmitStockAttribute=&m_TransmitStockAttribute;
					 m_TransPackageStruct.m_dwTransmitStockCount=1;
					 IsAddDownLoadData=TRUE;
					 break; 
				case WideSingleExRight:             //除权数据
				case WideExRight:                   //全部除权数据
					 m_TransPackageStruct.m_TransmitType=WideExRightData;
					 m_TransPackageStruct.m_dwTransmitStockType=-1;
					 strcpy(m_TransmitStockAttribute.m_szTransmitStockLabel,pCdat->id);
					 m_TransmitStockAttribute.m_dwTransmitCounts=1;
					 m_TransmitStockAttribute.m_wMarket=m_pDoc->m_sharesCompute.GetMarketKind(pCdat->kind);
					 m_TransPackageStruct.m_pTransmitStockAttribute=&m_TransmitStockAttribute;
					 m_TransPackageStruct.m_dwTransmitStockCount=1;
					 IsAddDownLoadData=TRUE;
					 break;
				case WideSingleCaiwuData:           //财务数据 
				case WideCaiwuData:                  //全部财务数据 
					 m_TransPackageStruct.m_TransmitType=WideFinancingData;
					 m_TransPackageStruct.m_dwTransmitStockType=-1;
					 strcpy(m_TransmitStockAttribute.m_szTransmitStockLabel,pCdat->id);
					 m_TransmitStockAttribute.m_dwTransmitCounts=1;
					 m_TransmitStockAttribute.m_wMarket=m_pDoc->m_sharesCompute.GetMarketKind(pCdat->kind);
					 m_TransPackageStruct.m_pTransmitStockAttribute=&m_TransmitStockAttribute;
					 m_TransPackageStruct.m_dwTransmitStockCount=1;
					 IsAddDownLoadData=TRUE;
					 break;
				case WideHistoryDetail:             //历史明细数据
					{
						int tempCount=pRequestContext->m_dwTimeCompleteCounts;
						for(int i=tempCount;i<pRequestContext->m_dwTimeTotalCounts;i++)
						{
							time_t t=pRequestContext->m_tDownLoadTimes[i];
							CTime ctTime(t);
							CString strTime=ctTime.Format("%Y%m%d");
							if(CTaiKlineFileHS::IsNeedHsHistData(pCdat->id,pCdat->kind,strTime))
							{
								 m_TransPackageStruct.m_TransmitType=HistoryDisPartBargainingData;
								 m_TransPackageStruct.m_dwTransmitStockType=-1;
								 strcpy(m_TransmitStockAttribute.m_szTransmitStockLabel,pCdat->id);
								 m_TransmitStockAttribute.m_dwTransmitCounts=t;
								 m_TransmitStockAttribute.m_wMarket=m_pDoc->m_sharesCompute.GetMarketKind(pCdat->kind);
								 m_TransPackageStruct.m_pTransmitStockAttribute=&m_TransmitStockAttribute;
								 m_TransPackageStruct.m_dwTransmitStockCount=1;
								 IsAddDownLoadData=TRUE;
								 pRequestContext->m_dwTimeCompleteCounts=i;
								 break;
							}
						}
						if(!IsAddDownLoadData)
						{
							pRequestContext->m_dwTimeCompleteCounts=i;
							if(pRequestContext->m_dwTimeCompleteCounts==pRequestContext->m_dwTimeTotalCounts)
                               pRequestContext->m_dwDownLoadCompleteCounts++;
						}
					}
					break;
			}
        }while(!IsAddDownLoadData) ;
		if(IsAddDownLoadData)
		{
#ifdef WIDE_NET_VERSION
			if(m_SocketID.iFlag!=-1)
			{
				 m_pMainFrm->m_pClientTransmitManageWnd->GetMarketData(&m_TransPackageStruct,0,m_SocketID);
			}
#endif
            break;
		}
	}
	DispalyDownLoadInfomation();
}
CReportData *CDownLoadNetDlg::GetCdatPointFromStockTypeAndPos(char strDownLoadName[MAX_PATH],int nStockType,int Pos)
{
    CReportData *pCdat=NULL;
    if(nStockType<0)
	{
		int nKind=-1-nStockType;
		if(!m_pDoc->m_sharesInformation.Lookup(strDownLoadName,pCdat,nKind))
            pCdat=NULL;
		return pCdat;
	}
	else
	{
		switch(nStockType)
		{
			case SHZS:                            //上证指数
			case SHAG:                            //上证A股 
			case SHBG:                            //上证B股 
			case SHZQ:                            //上证债券
			case SZZS:                            //深圳指数
			case SZAG:                            //深圳A股
			case SZBG:                            //深圳B股
			case SZZQ:                            //深圳债券
				if(!m_pDoc->m_sharesInformation.GetStockItem(SHZS,Pos,pCdat))
					pCdat=NULL;
				break;
			case CHOOSESTK:
				{
					 SymbolKindArr l_StockArray;
					 SymbolKind l_SymbolKind;
					 m_pDoc->m_ManagerBlockData.GetChooseStockCode(l_StockArray);
					 if(Pos<l_StockArray.GetSize())
					 {
                        l_SymbolKind=l_StockArray.GetAt(Pos);
						if(!m_pDoc->m_sharesInformation.Lookup(l_SymbolKind.m_chSymbol,pCdat,l_SymbolKind.m_nSymbolKind))
							pCdat=NULL;
					 }
					 else
						 pCdat=NULL;
				}
				break;
			case BLOCKINDEX:
				{
			         AfxMessageBox("该数据下载任务不能被添加!");
				}
				break;
			default:
				{
					 SymbolKindArr l_StockCodeArray;
					 CStringArray l_StockTypeNameArray;
					 m_pDoc->m_ManagerBlockData.GetAllBlockNames(l_StockTypeNameArray);
					 CString l_sStockTypeName=l_StockTypeNameArray.GetAt(nStockType-1000);
					 m_pDoc->m_ManagerBlockData.GetStockFromBlockName(l_StockCodeArray,l_sStockTypeName.GetBuffer(0));          
					 if(Pos<l_StockCodeArray.GetSize())
					 {
                        SymbolKind l_SymbolKind=l_StockCodeArray.GetAt(Pos);
						if(!m_pDoc->m_sharesInformation.Lookup(l_SymbolKind.m_chSymbol,pCdat,l_SymbolKind.m_nSymbolKind))
							pCdat=NULL;
					 }
					 else
						 pCdat=NULL;
				}
				break;
		}
	}
    return pCdat;
}
void CDownLoadNetDlg::DispalyDownLoadInfomation()
{
	int nCount=m_lstStockTypeName.GetItemCount();
	for( int i=0;i<nCount;i++)
	{
		CString szText;
		int ItemData=m_lstStockTypeName.GetItemData(i);
		WIDEREQUEST_CONTEXT *pRequestContext=(WIDEREQUEST_CONTEXT *)ItemData;
		szText.Format("%d",pRequestContext->m_dwDownLoadCompleteCounts);
		m_lstStockTypeName.SetItemText( i, 5, szText );
	}
}
LRESULT CDownLoadNetDlg::OnRecvTransmitInfoMation(WPARAM wParam, LPARAM lParam)
{
#ifdef WIDE_NET_VERSION
	if(!this->m_pMainFrm->m_bLogonSuccess)
	{
		AfxMessageBox("网络没有正常连接! 不能进行数据下载操作");
		this->m_bDownLoadState=FALSE;
		return;
	}
	if(m_pMainFrm->m_pClientTransmitManageWnd)
	{
		if(m_SocketID.iFlag==-1)
		{
             m_SocketID=m_pMainFrm->m_pClientTransmitManageWnd->RegisterID(this->m_hWnd);
		}
		if(m_SocketID.iFlag!=-1)
		{
		   AddDownLoadDataInfo(TRUE);
		   this->m_bDownLoadState=TRUE;
		}
	}
#endif
	return 1L;
}