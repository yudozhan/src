// DownLoadDlg.cpp : implementation file
//

#include "stdafx.h"
#include "WH.h"
#include "CELLRANGE.h"
#include "CTaiShanReportView.h"
#include "mainfrm.h"
#include <share.h>
#include  <io.h>
#include <shellapi.h>
#include "PlayGif.h"
#include "CaiwuData.h"
#include "keybrodeangel.h"
#include "SelectStock.h"
#include "DayLineSelect.h"
#include "StructKlineView.h"
#include "CTaiKlineFileHS.h"
#include "DownLoadDlg.h"
#include  "ImportData.h"
#include <direct.h>
#include  "CTaiChuQuanInDlg.h"
#include <io.h>
#include "CTaiKlineFileKLine.h"
#include "CTaiShanKlineShowView.h"
#include "SelectStockHsDetail2.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
#define ID_DOWNLOAD_STATE 10098
#define WM_USER_DISPLAY_DOWNLOAD_INFOMATION WM_USER+9999
#define WM_USER_POSTFOCUS WM_USER+9998
CRITICAL_SECTION g_csDownLoad;
BOOL gbCloseDialog;
void WINAPI DispalyDownLoadInfomation(REQUEST_CONTEXT *pRequestContext)
{
	LVFINDINFO  lvf;
	int index;
	if(gbCloseDialog)
		return;

//    EnterCriticalSection( &g_csDownLoad);
	CDownLoadDlg *pDlg=(CDownLoadDlg *)pRequestContext->pWnd;
	if(pRequestContext->DownloadDataType==TaiInfoFiguer)
	{
		if(pRequestContext->nDownLoadState==DOWNLOADCOMPLETE)
		{
            pDlg->SetGifDisplay(pRequestContext);
		}
	}

	lvf.flags=LVFI_STRING;
	lvf.psz=pRequestContext->szFileName;
	index=pDlg->m_lstStockTypeName.FindItem(&lvf);
    if(index>=0)
	{
		CString szText;
		char Temp[MAX_PATH];
		szText.Format("%d",pRequestContext->DownloadFileLength);
		pDlg->m_lstStockTypeName.SetItemText( index, 2, szText );

        if(pRequestContext->DownloadFileLength!=0)
		{
			float fRate = (100*pRequestContext->DownloadDataCount/pRequestContext->DownloadFileLength);
			if(fRate>100) fRate = 100;
			sprintf(Temp,"%6.0f%s",(float)fRate,"%");
		}
		else
			sprintf(Temp,"%6.0f%s",0.0f,"%");
        szText=Temp;
		pDlg->m_lstStockTypeName.SetItemText( index, 3, szText );
		switch(pRequestContext->nDownLoadState)
		{

				case READY:
			         szText="׼������";
					 break;
                case CONNECTSERVER:
			         szText="��������";
					 break;
                case CONNECTSERVERED:
			         szText="���ӳɹ�";
					 break;
                case CONNECTSERVERERROR:
			         szText="���Ӵ���";
					 break;
				case NOTFINDFILE:
			         szText="�ļ�������";
					 break;
				case DOWNLOADING:
			         szText="��������";
					 break;
				case DOWNLOADERROR:
			         szText="���س���";
					 break;
				case DOWNLOADCOMPLETE:
			         szText="�������";
					 break;
                case STOPDOWNLOAD:
			         szText="ֹͣ����";
					 break;
				case OPENORSAVEERROR:
			         szText="�򿪻򱣴��ļ���";
					break;
                case INSTALLCOMPLETE:
			         szText="��װ���";
					 break;
                case NOTINSTALLCOMPLETE:
			         szText="��װ�����";
					 break;
		}
		pDlg->m_lstStockTypeName.SetItemText( index, 4, szText );
	}
//    LeaveCriticalSection( &g_csDownLoad);
}
DWORD WINAPI DownLoadThreader(LPVOID lpvContext)
{
	REQUEST_CONTEXT *cpContext;
	cpContext= (REQUEST_CONTEXT*)lpvContext;
    cpContext->nDownLoadState=CONNECTSERVER;
    DispalyDownLoadInfomation(cpContext);
	char URLFilePath[MAX_PATH];
	CString URLPath=cpContext->szURLFilePath;
    URLPath.MakeLower();
    if(URLPath.Find("http://",0)>=0)
        strcpy(URLFilePath,cpContext->szURLFilePath);
	else if(URLPath.Find("http:\\\\",0)>=0)
		sprintf(URLFilePath,"%s%s","http://",cpContext->szURLFilePath+7);
	else
        sprintf(URLFilePath,"%s%s","http://",cpContext->szURLFilePath);
       
	cpContext->hResource = InternetOpenUrl(cpContext->hOpen, URLFilePath, NULL, 0, 
	         INTERNET_FLAG_TRANSFER_BINARY, 0);
	//check the HINTERNET handle for errors
	if (cpContext->hResource == NULL)
	{
        cpContext->nDownLoadState=CONNECTSERVERERROR;
        DispalyDownLoadInfomation(cpContext);
		cpContext->bResourceOpen=FALSE;
		cpContext->dwThreadID=-1;
		cpContext->hThread=NULL;
		sprintf(cpContext->szMemo,"���ش���ţ�%d",	GetLastError()); 
        return FALSE;
	}
	char szOption[10];
	strcpy(szOption,"10000");
	InternetSetOption(cpContext->hResource,INTERNET_OPTION_DATA_RECEIVE_TIMEOUT , (LPVOID) szOption,sizeof(DWORD));
    cpContext->nDownLoadState=CONNECTSERVERED;
	cpContext->bResourceOpen=TRUE;
    DispalyDownLoadInfomation(cpContext);
	DownLoadDump(cpContext,  cpContext->hResource);
	return S_OK;
}
int WINAPI DownLoadDump(REQUEST_CONTEXT *cpContext,  HINTERNET hResource)
{
	char 	lpszData[1024*5];	
	int lReadCount=0;
	int lPendingCount=0;
	if(!DownLoadHeader(cpContext, hResource))
	{
	    InternetCloseHandle(hResource);
        cpContext->bResourceOpen=FALSE;
        cpContext->dwThreadID=-1;
		cpContext->hThread=NULL;
		return TRUE;
	}
	char szFilelength[16];
	DWORD dwFileSize=sizeof(szFilelength);
    if(HttpQueryInfo(hResource,HTTP_QUERY_CONTENT_LENGTH,szFilelength,&dwFileSize,NULL))
	{
          cpContext->DownloadFileLength=atol(szFilelength);
	}
	else
	{
		  int cc=GetLastError();
	}
	if(cpContext->DownloadDataCount>=cpContext->DownloadFileLength)
        cpContext->DownloadDataCount=0;
	if(cpContext->DownloadDataCount>=0&&cpContext->DownloadDataCount < cpContext->DownloadFileLength)
	{
        if(InternetSetFilePointer(hResource,cpContext->DownloadDataCount,NULL,FILE_BEGIN ,0)<0)
		{
              cpContext->DownloadDataCount=0;
		}
	}
	cpContext->nDownLoadState=DOWNLOADING;
    cpContext->LastDownloadCount= cpContext->DownloadDataCount; 

    DispalyDownLoadInfomation(cpContext);
	INTERNET_BUFFERS Internet_Buffers;
    Internet_Buffers.dwStructSize=sizeof(INTERNET_BUFFERS);
    Internet_Buffers.Next=NULL;
	Internet_Buffers.lpcszHeader=NULL;
	Internet_Buffers.dwHeadersLength=0;
	Internet_Buffers.dwHeadersTotal=0;
	Internet_Buffers.lpvBuffer=(LPVOID)lpszData;
    Internet_Buffers.dwBufferLength=sizeof(lpszData);
	Internet_Buffers.dwBufferTotal=sizeof(lpszData);
    Internet_Buffers.dwOffsetHigh=0;
    Internet_Buffers.dwOffsetLow=0;
	cpContext->bIsContinueDown=FALSE;

	lpszData[0]=0;

	FILE *fp;
	if(_access(cpContext->szSaveFilePath,0)==-1)   
	{
		char cc=0;
	   fp=fopen(cpContext->szSaveFilePath,"w+b");
	   fclose(fp);
	   fp=fopen(cpContext->szSaveFilePath,"r+b");
	}
    else
	{
		if(cpContext->DownloadDataCount==0)
		{
		   char cc=0;
		   fp=fopen(cpContext->szSaveFilePath,"w+b");
		   fclose(fp);
		}
	    fp=fopen(cpContext->szSaveFilePath,"r+b");
	}
	if(fp==NULL)
	{
	    InternetCloseHandle(hResource);
		cpContext->nDownLoadState=OPENORSAVEERROR;
        cpContext->bResourceOpen=FALSE;
        cpContext->dwThreadID=-1;
		cpContext->hThread=NULL;
        DispalyDownLoadInfomation(cpContext);
		return TRUE;
	}
    if(fseek(fp,cpContext->DownloadDataCount,SEEK_SET)!=0) 
	{
	    InternetCloseHandle(hResource);
		cpContext->nDownLoadState=OPENORSAVEERROR;
        DispalyDownLoadInfomation(cpContext);
        cpContext->bResourceOpen=FALSE;
        cpContext->dwThreadID=-1;
		cpContext->hThread=NULL;
		return TRUE;
	}
	int tempLength=0;
    do
	{
//try_again:  if(!InternetReadFileEx(hResource,&Internet_Buffers,IRF_SYNC |IRF_NO_WAIT  ,0))
try_again:  if(!InternetReadFileEx(hResource,&Internet_Buffers,IRF_SYNC ,0))
			{
	            cpContext->bIsContinueDown=FALSE;
				if(cpContext->bIsStopDownload)
				{
					cpContext->nDownLoadState=STOPDOWNLOAD;
					fclose(fp);
					InternetCloseHandle(hResource);
                    DispalyDownLoadInfomation(cpContext);
					cpContext->bResourceOpen=FALSE;
					cpContext->dwThreadID=-1;
					cpContext->hThread=NULL;
					return TRUE;
				}
				if (GetLastError()== ERROR_IO_PENDING)
				{
					Sleep(500);
					goto try_again;
				}
				cpContext->nDownLoadState=DOWNLOADERROR;
				sprintf(cpContext->szMemo,"ȡ���ݴ�");
				cpContext->downloaderrors++;
 				break;
			}
			else
			{
				if (Internet_Buffers.dwBufferLength == 0)
				{
					cpContext->nDownLoadState=DOWNLOADCOMPLETE;
	                sprintf(cpContext->szMemo,"�������");
					break;
				}
				else
				{
//					Sleep(1000);
/*					if(Internet_Buffers.dwBufferLength<500&&(cpContext->DownloadDataCount+Internet_Buffers.dwBufferLength)<cpContext->DownloadFileLength)
					{
						cpContext->nDownLoadState=DOWNLOADING;
						fclose(fp);
						InternetCloseHandle(hResource);
						cpContext->bResourceOpen=FALSE;
						cpContext->dwThreadID=-1;
						cpContext->hThread=NULL;
						cpContext->bIsContinueDown=TRUE;
						return TRUE;
					}
					else
*/					{
						fseek(fp,cpContext->DownloadDataCount,SEEK_SET); 
						fwrite(lpszData,Internet_Buffers.dwBufferLength,1,fp);
						lpszData[0]=0;
						cpContext->DownloadDataCount += Internet_Buffers.dwBufferLength ;
					}
					if(cpContext->bIsStopDownload)
					{
						cpContext->nDownLoadState=STOPDOWNLOAD;
						fclose(fp);
						InternetCloseHandle(hResource);
                        DispalyDownLoadInfomation(cpContext);
			            cpContext->bResourceOpen=FALSE;
				        cpContext->dwThreadID=-1;
				        cpContext->hThread=NULL;
						return TRUE;
					}
					cpContext->nDownLoadState=DOWNLOADING;
                    DispalyDownLoadInfomation(cpContext);
				}
			}
	}
	while(TRUE);
	
	fclose(fp);
	InternetCloseHandle(hResource);
	cpContext->bResourceOpen=FALSE;
	cpContext->dwThreadID=-1;
	cpContext->hThread=NULL;
    DispalyDownLoadInfomation(cpContext);
	return TRUE;
}
int WINAPI DownLoadHeader(REQUEST_CONTEXT *cpContext, HINTERNET hHttp)
{
	DWORD dwHeaderType=-1;
	long dwDownLoadState=0;
	char szDownloadState[256];
	DWORD dwSize=sizeof(szDownloadState);

	dwHeaderType = HTTP_QUERY_STATUS_CODE ;	
//retry:
	if(!HttpQueryInfo(hHttp,dwHeaderType,(LPVOID)szDownloadState,&dwSize,NULL))
	{
		if (GetLastError()==ERROR_HTTP_HEADER_NOT_FOUND)
		{
			cpContext->nDownLoadState=DOWNLOADERROR;
			sprintf(cpContext->szMemo,"���ݲ�����");
            DispalyDownLoadInfomation(cpContext);
			return FALSE;
		}
		else
		{
			cpContext->nDownLoadState=DOWNLOADERROR;
			sprintf(cpContext->szMemo,"��ѯ�ļ�����");
            DispalyDownLoadInfomation(cpContext);
			return FALSE;
		}
	}

	dwDownLoadState=atol(szDownloadState);
	switch(dwDownLoadState)
	{
	case HTTP_STATUS_OK:
		return TRUE;
		break;
	case HTTP_STATUS_NOT_FOUND:
		cpContext->nDownLoadState=NOTFINDFILE;
		sprintf(cpContext->szMemo,"�����ļ�������");
		break;
	case HTTP_STATUS_SERVER_ERROR:
		cpContext->nDownLoadState=DOWNLOADERROR;
		sprintf(cpContext->szMemo,"����������");
		break;
	case HTTP_STATUS_BAD_REQUEST: 
		cpContext->nDownLoadState=DOWNLOADERROR;
		sprintf(cpContext->szMemo,"�������");
		break;
	default:
        sprintf(cpContext->szMemo,"����ţ�%d",dwDownLoadState);
		break;
	}
    DispalyDownLoadInfomation(cpContext);
	return FALSE;
}	

/////////////////////////////////////////////////////////////////////////////
// CDownLoadDlg dialog


CDownLoadDlg::CDownLoadDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDownLoadDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDownLoadDlg)
	//}}AFX_DATA_INIT
//	m_PlayGif=NULL;
	m_bIsCalc=FALSE;
}
CDownLoadDlg::~CDownLoadDlg()   // standard constructor
{
}



void CDownLoadDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDownLoadDlg)
	DDX_Control(pDX, IDC_PROGRESS_DOWNLOAD, m_Progress);
	DDX_Control(pDX, IDC_SELECT_LOADDATA, m_MyTree);
	DDX_Control(pDX, IDC_DOWNLOAD_INFO, m_lstStockTypeName);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDownLoadDlg, CDialog)
	//{{AFX_MSG_MAP(CDownLoadDlg)
	ON_BN_CLICKED(IDC_ADD_LOADDATA, OnAddLoadData)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_DELETE_LOADDATA, OnDeleteLoadData)
	ON_BN_CLICKED(IDC_DOWNLOAD_START, OnDownloadStart)
	ON_BN_CLICKED(IDC_STOPDOWNLOAD, OnStopDownload)
	ON_BN_CLICKED(IDC_INSTALL_LOADDATA, OnInstallLoaddata)
	ON_BN_CLICKED(IDC_LOAD_COLLECTE, OnLoadCollecte)
	ON_BN_CLICKED(IDC_SAVE_COLLECTE, OnSaveCollecte)
	ON_WM_TIMER()
	ON_NOTIFY(NM_DBLCLK, IDC_SELECT_LOADDATA, OnDblclkSelectLoaddata)
	ON_WM_HELPINFO()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_USER_ADDSTOCK,OnAddStockCode)
	ON_MESSAGE(WM_USER_ADDDAYLINE,OnAddStockDayLine)
	ON_MESSAGE(WM_USER_DISPLAY_DOWNLOAD_INFOMATION,OnDispalyDownLoadInfomation)
	ON_MESSAGE(WM_USER_POSTFOCUS ,OnPostFocus)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDownLoadDlg message handlers

BOOL CDownLoadDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	gbCloseDialog=FALSE;
	// TODO: Add extra initialization here
    char makedirectory[MAX_PATH];
    CString lpszAgent = ""+g_strCompanyName+"��������ϵͳ";
    m_pDoc =((CMainFrame*)AfxGetMainWnd())->m_pDoc ;

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
	m_pImageList->Create(IDB_BITMAP14,32,0,RGB(192,192,192));    //������ͼ���б�
	m_pImageListSmall->Create(IDB_BITMAP15,16,0,RGB(192,192,192)); //����Сͼ���б�
	FillToListCtrlHead(); 	//��ʼ��CListCtrl����
	FillToTreeCtrlHead();

 //   m_PlayGif=new CPlayGif(this->m_hWnd);
 //   m_PlayGif->Load("newsjl.gif");
 //   m_PlayGif->SetPosition(15,5);
 //   m_PlayGif->Play();

	hOpen = InternetOpen(lpszAgent.GetBuffer(0), INTERNET_OPEN_TYPE_PRECONFIG,
						NULL, NULL, NULL );

	char szOption[10];
	strcpy(szOption,"5");
	BOOL btemp=InternetSetOption(hOpen,INTERNET_OPTION_CONNECT_RETRIES, (LPVOID) szOption,sizeof(DWORD));

	strcpy(szOption,"10000");
	btemp=InternetSetOption(hOpen,INTERNET_OPTION_CONNECT_TIMEOUT, (LPVOID) szOption,sizeof(DWORD));

	strcpy(szOption,"10000");
	btemp=InternetSetOption(hOpen,INTERNET_OPTION_RECEIVE_TIMEOUT, (LPVOID) szOption,sizeof(DWORD));


	char temp[250];
	CString temp1;
	temp[0] = 0;
//	GetPrivateProfileString("3WADDRESS","Address",m_pDoc->m_SystemInitData.NetAddr,temp,250,"Config.ini");
	strcpy(temp,m_pDoc->m_SystemInitData.NetAddr);
	temp1.Format("%s",temp);
	strcpy(temp,"/");
    if(temp1.Right(1) != '/')
	  temp1 = temp1 + temp;

	m_InfomationContext.DownloadDataType=TaiInfoFiguer;
	strcpy(m_InfomationContext.szFileName ,"Sjl.gif");
    m_InfomationContext.DownloadDataCount=0;
    m_InfomationContext.DownloadFileLength=0; 
	m_InfomationContext.nDownLoadState=READY;
	sprintf(m_InfomationContext.szURLFilePath,"%sdownload/%s",temp1,m_InfomationContext.szFileName);
	sprintf(m_InfomationContext.szSaveFilePath,"%s\\Extract\\%s",m_szCurrentDir,m_InfomationContext.szFileName);
	m_InfomationContext.pWnd=this;	 	//���ھ��
    m_InfomationContext.hResource=NULL;
	strcpy(	m_InfomationContext.szMemo,""+g_strCompanyName+"��������ϵͳ");
	m_InfomationContext.bResourceOpen=FALSE;
	m_InfomationContext.bIsStopDownload=FALSE;
    m_InfomationContext.bIsContinueDown=FALSE;
	m_InfomationContext.downloaderrors=0;
    m_InfomationContext.LastDownloadCount=0;
    m_InfomationContext.DownLoadNotChangeCount=0;

    m_InfomationContext.dwThreadID=-1;
	m_InfomationContext.hThread=NULL;

    InitializeCriticalSection( &g_csDownLoad);
	return TRUE;  // return TRUE unless you set the focus to a control
}

BEGIN_EVENTSINK_MAP(CDownLoadDlg, CDialog)
    //{{AFX_EVENTSINK_MAP(CDownLoadDlg)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

void CDownLoadDlg::FillToListCtrlHead()    //��ʼ��CListCtrl����
{
   	CRect           rect;
	CString         strItem1= _T("�ļ�����");
	CString         strItem2= _T("�ļ�����");
	CString         strItem3= _T("�ļ�����");
	CString         strItem4= _T("���ؽ���");
	CString         strItem5= _T("����״̬");
	m_lstStockTypeName.SetExtendedStyle(m_lstStockTypeName.GetExtendedStyle()|LVS_EX_FULLROWSELECT);
    
	this->m_lstStockTypeName.SetImageList(m_pImageList, LVSIL_NORMAL);
	this->m_lstStockTypeName.SetImageList(m_pImageListSmall, LVSIL_SMALL);

	// insert two columns (REPORT mode) and modify the new header items
	this->m_lstStockTypeName.GetWindowRect(&rect);
	this->m_lstStockTypeName.InsertColumn(0, strItem1, LVCFMT_LEFT,    //��������
		rect.Width() * 1/5+30, 0); 
	this->m_lstStockTypeName.InsertColumn(1, strItem2, LVCFMT_LEFT,    //��������
		rect.Width() * 1/5-5, 1);
	this->m_lstStockTypeName.InsertColumn(2, strItem3, LVCFMT_LEFT,    //��������
		rect.Width() * 1/5-15, 2);
	this->m_lstStockTypeName.InsertColumn(3, strItem4, LVCFMT_LEFT,    //��������
		rect.Width() * 1/5-15, 3);
	this->m_lstStockTypeName.InsertColumn(4, strItem5, LVCFMT_LEFT,    //��������
		rect.Width() * 1/5+5, 3);

}
void CDownLoadDlg::FillToTreeCtrlHead()          //��ʼ��CTreeCtrl������
{
	// TODO: Add extra initialization here
	m_MyTree.SetImageList(m_pImageListSmall,TVSIL_NORMAL);

	TV_INSERTSTRUCT TreeCtrlItem;
//	HTREEITEM TreeCtrlItemCurr;
	BOOL IsFind=FALSE;

	TreeCtrlItem.hParent = TVI_ROOT;
	TreeCtrlItem.hInsertAfter = TVI_LAST;
	TreeCtrlItem.item.mask = TVIF_TEXT | TVIF_PARAM|TVIF_IMAGE|TVIF_SELECTEDIMAGE  ;
	TreeCtrlItem.item.pszText = "��Ʊ����";
    TreeCtrlItem.item.iImage=0;
	TreeCtrlItem.item.iSelectedImage=0;
	TreeCtrlItem.item.lParam =StockName;
	this->m_MyTree.InsertItem(&TreeCtrlItem);

	TreeCtrlItem.hParent = TVI_ROOT;
	TreeCtrlItem.hInsertAfter = TVI_LAST;
	TreeCtrlItem.item.mask = TVIF_TEXT | TVIF_PARAM|TVIF_IMAGE|TVIF_SELECTEDIMAGE  ;
	TreeCtrlItem.item.pszText = "��������";
    TreeCtrlItem.item.iImage=0;
	TreeCtrlItem.item.iSelectedImage=0;
	TreeCtrlItem.item.lParam =1000;
	HTREEITEM hTreeItem1 = this->m_MyTree.InsertItem(&TreeCtrlItem);

	TreeCtrlItem.hParent = hTreeItem1;
	TreeCtrlItem.item.pszText = "��������" ;
	TreeCtrlItem.item.lParam = TodayDayLine;
    TreeCtrlItem.item.iImage=1;
	TreeCtrlItem.item.iSelectedImage=1;
	this->m_MyTree.InsertItem(&TreeCtrlItem);

	TreeCtrlItem.hParent = hTreeItem1;
	TreeCtrlItem.item.pszText = "��������" ;
	TreeCtrlItem.item.lParam = MonthDayLine;
    TreeCtrlItem.item.iImage=1;
	TreeCtrlItem.item.iSelectedImage=1;
	this->m_MyTree.InsertItem(&TreeCtrlItem);

	TreeCtrlItem.hParent = hTreeItem1;
	TreeCtrlItem.item.pszText = "��ʷ����" ;
	TreeCtrlItem.item.lParam = HistroyDayLine;
    TreeCtrlItem.item.iImage=1;
	TreeCtrlItem.item.iSelectedImage=1;
	this->m_MyTree.InsertItem(&TreeCtrlItem);

	TreeCtrlItem.hParent = TVI_ROOT;
	TreeCtrlItem.hInsertAfter = TVI_LAST;
	TreeCtrlItem.item.mask = TVIF_TEXT | TVIF_PARAM|TVIF_IMAGE|TVIF_SELECTEDIMAGE  ;
	TreeCtrlItem.item.pszText = "��Ȩ����";
    TreeCtrlItem.item.iImage=0;
	TreeCtrlItem.item.iSelectedImage=0;
	TreeCtrlItem.item.lParam =ExRightData;
	this->m_MyTree.InsertItem(&TreeCtrlItem);

	TreeCtrlItem.hParent = TVI_ROOT;
	TreeCtrlItem.hInsertAfter = TVI_LAST;
	TreeCtrlItem.item.mask = TVIF_TEXT | TVIF_PARAM|TVIF_IMAGE|TVIF_SELECTEDIMAGE  ;
	TreeCtrlItem.item.pszText = "��������";
    TreeCtrlItem.item.iImage=0;
	TreeCtrlItem.item.iSelectedImage=0;
	TreeCtrlItem.item.lParam =CaiwuData;
	this->m_MyTree.InsertItem(&TreeCtrlItem);

	TreeCtrlItem.hParent = TVI_ROOT;
	TreeCtrlItem.hInsertAfter = TVI_LAST;
	TreeCtrlItem.item.mask = TVIF_TEXT | TVIF_PARAM|TVIF_IMAGE|TVIF_SELECTEDIMAGE  ;
	TreeCtrlItem.item.pszText = "��������";
    TreeCtrlItem.item.iImage=0;
	TreeCtrlItem.item.iSelectedImage=0;
	TreeCtrlItem.item.lParam =1001;
	hTreeItem1=this->m_MyTree.InsertItem(&TreeCtrlItem);
	
	TreeCtrlItem.hParent = hTreeItem1;
	TreeCtrlItem.item.pszText = "��������" ;
	TreeCtrlItem.item.lParam = StockBaseInfo;
    TreeCtrlItem.item.iImage=1;
	TreeCtrlItem.item.iSelectedImage=1;
	this->m_MyTree.InsertItem(&TreeCtrlItem);

	TreeCtrlItem.hParent = hTreeItem1;
	TreeCtrlItem.item.pszText = "���ո���" ;
	TreeCtrlItem.item.lParam = NewUpdateBaseInfo;
    TreeCtrlItem.item.iImage=1;
	TreeCtrlItem.item.iSelectedImage=1;
	this->m_MyTree.InsertItem(&TreeCtrlItem);


	TreeCtrlItem.hParent = TVI_ROOT;
	TreeCtrlItem.hInsertAfter = TVI_LAST;
	TreeCtrlItem.item.mask = TVIF_TEXT | TVIF_PARAM|TVIF_IMAGE|TVIF_SELECTEDIMAGE  ;
	TreeCtrlItem.item.pszText = "��ʱ����";
    TreeCtrlItem.item.iImage=0;
	TreeCtrlItem.item.iSelectedImage=0;
	TreeCtrlItem.item.lParam =1002;
	hTreeItem1=this->m_MyTree.InsertItem(&TreeCtrlItem);

	TreeCtrlItem.hParent = hTreeItem1;
	TreeCtrlItem.item.pszText = "���ɷ�ʱ" ;
	TreeCtrlItem.item.lParam = StockMinute;
    TreeCtrlItem.item.iImage=1;
	TreeCtrlItem.item.iSelectedImage=1;
	this->m_MyTree.InsertItem(&TreeCtrlItem);
	
	TreeCtrlItem.hParent = hTreeItem1;
	TreeCtrlItem.item.pszText = "��ָ֤��" ;
	TreeCtrlItem.item.lParam = ShZsMinute;
    TreeCtrlItem.item.iImage=1;
	TreeCtrlItem.item.iSelectedImage=1;
	this->m_MyTree.InsertItem(&TreeCtrlItem);

	TreeCtrlItem.hParent = hTreeItem1;
	TreeCtrlItem.item.pszText = "��֤����" ;
	TreeCtrlItem.item.lParam = ShAgMinute;
    TreeCtrlItem.item.iImage=1;
	TreeCtrlItem.item.iSelectedImage=1;
	this->m_MyTree.InsertItem(&TreeCtrlItem);

	TreeCtrlItem.hParent = hTreeItem1;
	TreeCtrlItem.item.pszText = "��֤�¹�" ;
	TreeCtrlItem.item.lParam = ShBgMinute;
    TreeCtrlItem.item.iImage=1;
	TreeCtrlItem.item.iSelectedImage=1;
	this->m_MyTree.InsertItem(&TreeCtrlItem);

	TreeCtrlItem.hParent = hTreeItem1;
	TreeCtrlItem.item.pszText = "��֤����" ;
	TreeCtrlItem.item.lParam = ShJjMinute;
    TreeCtrlItem.item.iImage=1;
	TreeCtrlItem.item.iSelectedImage=1;
	this->m_MyTree.InsertItem(&TreeCtrlItem);

	TreeCtrlItem.hParent = hTreeItem1;
	TreeCtrlItem.item.pszText = "��֤ծȯ" ;
	TreeCtrlItem.item.lParam = ShZqMinute;
	TreeCtrlItem.item.iImage=1;
	TreeCtrlItem.item.iSelectedImage=1;
	this->m_MyTree.InsertItem(&TreeCtrlItem);

	TreeCtrlItem.hParent = hTreeItem1;
	TreeCtrlItem.item.pszText = "��֤תծ" ;
	TreeCtrlItem.item.lParam = ShZzMinute;
	TreeCtrlItem.item.iImage=1;
	TreeCtrlItem.item.iSelectedImage=1;
	this->m_MyTree.InsertItem(&TreeCtrlItem);

	TreeCtrlItem.hParent = hTreeItem1;
	TreeCtrlItem.item.pszText = "��֤�ع�" ;
	TreeCtrlItem.item.lParam = ShHgMinute;
	TreeCtrlItem.item.iImage=1;
	TreeCtrlItem.item.iSelectedImage=1;
	this->m_MyTree.InsertItem(&TreeCtrlItem);

	TreeCtrlItem.hParent = hTreeItem1;
	TreeCtrlItem.item.pszText = "��֤ETF" ;
	TreeCtrlItem.item.lParam = ShEfMinute;
	TreeCtrlItem.item.iImage=1;
	TreeCtrlItem.item.iSelectedImage=1;
	this->m_MyTree.InsertItem(&TreeCtrlItem);

	TreeCtrlItem.hParent = hTreeItem1;
	TreeCtrlItem.item.pszText = "��֤���Ż���" ;
	TreeCtrlItem.item.lParam = ShKjMinute;
	TreeCtrlItem.item.iImage=1;
	TreeCtrlItem.item.iSelectedImage=1;
	this->m_MyTree.InsertItem(&TreeCtrlItem);

	TreeCtrlItem.hParent = hTreeItem1;
	TreeCtrlItem.item.pszText = "��֤Ȩ֤" ;
	TreeCtrlItem.item.lParam = ShQzMinute;
	TreeCtrlItem.item.iImage=1;
	TreeCtrlItem.item.iSelectedImage=1;
	this->m_MyTree.InsertItem(&TreeCtrlItem);

	TreeCtrlItem.hParent = hTreeItem1;
	TreeCtrlItem.item.pszText = "��֤����" ;
	TreeCtrlItem.item.lParam = ShOtMinute;
	TreeCtrlItem.item.iImage=1;
	TreeCtrlItem.item.iSelectedImage=1;
	this->m_MyTree.InsertItem(&TreeCtrlItem);

	TreeCtrlItem.hParent = hTreeItem1;
	TreeCtrlItem.item.pszText = "��ָ֤��" ;
	TreeCtrlItem.item.lParam = SzZsMinute;
    TreeCtrlItem.item.iImage=1;
	TreeCtrlItem.item.iSelectedImage=1;
	this->m_MyTree.InsertItem(&TreeCtrlItem);

	TreeCtrlItem.hParent = hTreeItem1;
	TreeCtrlItem.item.pszText = "��֤����" ;
	TreeCtrlItem.item.lParam = SzAgMinute;
    TreeCtrlItem.item.iImage=1;
	TreeCtrlItem.item.iSelectedImage=1;
	this->m_MyTree.InsertItem(&TreeCtrlItem);

	TreeCtrlItem.hParent = hTreeItem1;
	TreeCtrlItem.item.pszText = "��֤�¹�" ;
	TreeCtrlItem.item.lParam = SzBgMinute;
    TreeCtrlItem.item.iImage=1;
	TreeCtrlItem.item.iSelectedImage=1;
	this->m_MyTree.InsertItem(&TreeCtrlItem);

	TreeCtrlItem.hParent = hTreeItem1;
	TreeCtrlItem.item.pszText = "��֤����" ;
	TreeCtrlItem.item.lParam = SzJjMinute;
    TreeCtrlItem.item.iImage=1;
	TreeCtrlItem.item.iSelectedImage=1;
	this->m_MyTree.InsertItem(&TreeCtrlItem);

	TreeCtrlItem.hParent = hTreeItem1;
	TreeCtrlItem.item.pszText = "��֤ծȯ" ;
	TreeCtrlItem.item.lParam = SzZqMinute;
	TreeCtrlItem.item.iImage=1;
	TreeCtrlItem.item.iSelectedImage=1;
	this->m_MyTree.InsertItem(&TreeCtrlItem);

	TreeCtrlItem.hParent = hTreeItem1;
	TreeCtrlItem.item.pszText = "��֤תծ" ;
	TreeCtrlItem.item.lParam = SzZzMinute;
	TreeCtrlItem.item.iImage=1;
	TreeCtrlItem.item.iSelectedImage=1;
	this->m_MyTree.InsertItem(&TreeCtrlItem);

	TreeCtrlItem.hParent = hTreeItem1;
	TreeCtrlItem.item.pszText = "��֤�ع�" ;
	TreeCtrlItem.item.lParam = SzHgMinute;
	TreeCtrlItem.item.iImage=1;
	TreeCtrlItem.item.iSelectedImage=1;
	this->m_MyTree.InsertItem(&TreeCtrlItem);

	TreeCtrlItem.hParent = hTreeItem1;
	TreeCtrlItem.item.pszText = "��֤ETF" ;
	TreeCtrlItem.item.lParam = SzEfMinute;
	TreeCtrlItem.item.iImage=1;
	TreeCtrlItem.item.iSelectedImage=1;
	this->m_MyTree.InsertItem(&TreeCtrlItem);

	TreeCtrlItem.hParent = hTreeItem1;
	TreeCtrlItem.item.pszText = "��֤���Ż���" ;
	TreeCtrlItem.item.lParam = SzKjMinute;
	TreeCtrlItem.item.iImage=1;
	TreeCtrlItem.item.iSelectedImage=1;
	this->m_MyTree.InsertItem(&TreeCtrlItem);

	TreeCtrlItem.hParent = hTreeItem1;
	TreeCtrlItem.item.pszText = "��ҵ��" ;
	TreeCtrlItem.item.lParam = SzCyMinute;
    TreeCtrlItem.item.iImage=1;
	TreeCtrlItem.item.iSelectedImage=1;
	this->m_MyTree.InsertItem(&TreeCtrlItem);

	TreeCtrlItem.hParent = hTreeItem1;
	TreeCtrlItem.item.pszText = "��֤Ȩ֤" ;
	TreeCtrlItem.item.lParam = SzQzMinute;
	TreeCtrlItem.item.iImage=1;
	TreeCtrlItem.item.iSelectedImage=1;
	this->m_MyTree.InsertItem(&TreeCtrlItem);

	TreeCtrlItem.hParent = hTreeItem1;
	TreeCtrlItem.item.pszText = "��֤����" ;
	TreeCtrlItem.item.lParam = SzSbMinute;
	TreeCtrlItem.item.iImage=1;
	TreeCtrlItem.item.iSelectedImage=1;
	this->m_MyTree.InsertItem(&TreeCtrlItem);

	TreeCtrlItem.hParent = hTreeItem1;
	TreeCtrlItem.item.pszText = "��֤����" ;
	TreeCtrlItem.item.lParam = SzOtMinute;
	TreeCtrlItem.item.iImage=1;
	TreeCtrlItem.item.iSelectedImage=1;
	this->m_MyTree.InsertItem(&TreeCtrlItem);

	TreeCtrlItem.hParent = TVI_ROOT;
	TreeCtrlItem.hInsertAfter = TVI_LAST;
	TreeCtrlItem.item.mask = TVIF_TEXT | TVIF_PARAM|TVIF_IMAGE|TVIF_SELECTEDIMAGE  ;
	TreeCtrlItem.item.pszText = "������Ϣ";
    TreeCtrlItem.item.iImage=0;
	TreeCtrlItem.item.iSelectedImage=0;
	TreeCtrlItem.item.lParam =1003;
	hTreeItem1=this->m_MyTree.InsertItem(&TreeCtrlItem);

	TreeCtrlItem.hParent = hTreeItem1;
	TreeCtrlItem.item.pszText = "��֤����" ;
	TreeCtrlItem.item.lParam = ShInfomation;
    TreeCtrlItem.item.iImage=1;
	TreeCtrlItem.item.iSelectedImage=1;
	this->m_MyTree.InsertItem(&TreeCtrlItem);

	TreeCtrlItem.hParent = hTreeItem1;
	TreeCtrlItem.item.pszText = "��֤����" ;
	TreeCtrlItem.item.lParam = SzInfomation;
    TreeCtrlItem.item.iImage=1;
	TreeCtrlItem.item.iSelectedImage=1;
	this->m_MyTree.InsertItem(&TreeCtrlItem);

	TreeCtrlItem.hParent = hTreeItem1;
	TreeCtrlItem.item.pszText = "�������" ;
	TreeCtrlItem.item.lParam = EbInfomation;
    TreeCtrlItem.item.iImage=1;
	TreeCtrlItem.item.iSelectedImage=1;
	this->m_MyTree.InsertItem(&TreeCtrlItem);

	TreeCtrlItem.hParent = hTreeItem1;
	TreeCtrlItem.item.pszText = "�ƾ�����" ;
	TreeCtrlItem.item.lParam =ZjInfomation;
    TreeCtrlItem.item.iImage=1;
	TreeCtrlItem.item.iSelectedImage=1;
	this->m_MyTree.InsertItem(&TreeCtrlItem);
//lmb11
#ifndef OEM
/*	TreeCtrlItem.hParent = TVI_ROOT;
	TreeCtrlItem.hInsertAfter = TVI_LAST;
	TreeCtrlItem.item.mask = TVIF_TEXT | TVIF_PARAM|TVIF_IMAGE|TVIF_SELECTEDIMAGE  ;
	TreeCtrlItem.item.pszText = "��ʷ��ϸ����";
    TreeCtrlItem.item.iImage=0;
	TreeCtrlItem.item.iSelectedImage=0;
	TreeCtrlItem.item.lParam =1004;
	this->m_MyTree.InsertItem(&TreeCtrlItem);*/
#endif
}
void CDownLoadDlg::OnAddLoadData() 
{
    AddDownLoadData() ;
}
void CDownLoadDlg::AddDownLoadData() 
{
	// TODO: Add your control notification handler code here
    Time_Now = CTime::GetCurrentTime();
    HTREEITEM hTreeItem;
	REQUEST_CONTEXT *pRequestContext;
	int nDayLineType;
	BOOL bOpenAddStockDlg=FALSE;
	BOOL bOpenSelectHistoryDlg=FALSE;
	char temp[250];
	CString temp1;
	temp[0] = 0;
//	GetPrivateProfileString("3WADDRESS","Address",m_pDoc->m_SystemInitData.NetAddr,temp,250,"Config.ini");
	strcpy(temp,m_pDoc->m_SystemInitData.NetAddr);
	temp1.Format("%s",temp);
	strcpy(temp,"/");
    if(temp1.Right(1) != '/')
	  temp1 = temp1 + temp;

    hTreeItem=this->m_MyTree.GetSelectedItem();
	if(hTreeItem==NULL)
	{
		AfxMessageBox("����ȷѡ��Ҫ������������");
		return;
	}
	UINT nId=this->m_MyTree.GetItemData(hTreeItem); 
	switch(nId)
	{
    case StockName:                 //��Ʊ����
			if(!CheckList("StockName.zip",QBGP))
			{
				 pRequestContext=new REQUEST_CONTEXT;
				 pRequestContext->DownloadDataType=StockName;
				 strcpy(pRequestContext->szFileName ,"StockName.zip");
				 pRequestContext->DownloadDataCount=0;
				 pRequestContext->kind=QBGP;
                 pRequestContext->DownloadFileLength=0; 
				 pRequestContext->nDownLoadState=READY;
				 sprintf(pRequestContext->szURLFilePath,"%sdownload/%s",temp1,pRequestContext->szFileName);
				 sprintf(pRequestContext->szSaveFilePath,"%s\\Extract\\Download\\%s",m_szCurrentDir,pRequestContext->szFileName);
                 InsertDownLoadDataToList(pRequestContext );
			}
			else
			   AfxMessageBox("�������ѱ����");
            break; 
    case 1000: 
	case TodayDayLine:              //��������
		{ 
			char szDate[10];
			char szFilename[256];
			sprintf(szDate,"%d%2d%2d",Time_Now.GetYear(),Time_Now.GetMonth(),Time_Now.GetDay());
			for(int i=0;i<8;i++)
			{
				if(szDate[i]==' ')
				   szDate[i]='0';
			}
			szDate[8]='\0';	
            sprintf(szFilename,"%sd.zip",szDate);
			if(!CheckList(szFilename,QBGP))
			{
				 pRequestContext=new REQUEST_CONTEXT;
				 pRequestContext->DownloadDataType=TodayDayLine;
				 strcpy(pRequestContext->szFileName ,szFilename);
				 pRequestContext->kind=QBGP;
				 pRequestContext->DownloadDataCount=0;
                 pRequestContext->DownloadFileLength=0; 
				 pRequestContext->nDownLoadState=READY;
				 sprintf(pRequestContext->szURLFilePath,"%sdownload/data/%s",temp1,pRequestContext->szFileName);
				 sprintf(pRequestContext->szSaveFilePath,"%s\\Extract\\Download\\%s",m_szCurrentDir,pRequestContext->szFileName);
                 InsertDownLoadDataToList(pRequestContext );
			}
			else
			   AfxMessageBox("�������ѱ����");
		}
         break; 
	case MonthDayLine:              //�������� 
		 nDayLineType=0;
		 bOpenSelectHistoryDlg=TRUE; 
         break; 
    case HistroyDayLine:            //��ʷ����
		 nDayLineType=1;
		 bOpenSelectHistoryDlg=TRUE; 
         break; 
    case ExRightData:               //��Ȩ����
		if(!CheckList("chuquan.zip",QBGP))
		{
			 pRequestContext=new REQUEST_CONTEXT;
			 pRequestContext->DownloadDataType=ExRightData;
			 strcpy(pRequestContext->szFileName ,"chuquan.zip");
			 pRequestContext->DownloadDataCount=0;
             pRequestContext->DownloadFileLength=0; 
			 pRequestContext->kind=QBGP;
			 pRequestContext->nDownLoadState=READY;
			 //sprintf(pRequestContext->szURLFilePath,"%sdownload/data/%s",temp1,pRequestContext->szFileName);
			 ///QQQ by cuiwei on 2004-04-18
			 sprintf(pRequestContext->szURLFilePath,"%sdownload/%s",temp1,pRequestContext->szFileName);
			 sprintf(pRequestContext->szSaveFilePath,"%s\\Extract\\Download\\%s",m_szCurrentDir,pRequestContext->szFileName);
             InsertDownLoadDataToList(pRequestContext );
		}
		else
		   AfxMessageBox("�������ѱ����");
         break; 
	case CaiwuData:               //��������
		if(!CheckList("finance.zip",QBGP))
		{
			 pRequestContext=new REQUEST_CONTEXT;
			 pRequestContext->DownloadDataType=CaiwuData;
			 strcpy(pRequestContext->szFileName ,"finance.zip");
			 pRequestContext->DownloadDataCount=0;
			 pRequestContext->kind=QBGP;
             pRequestContext->DownloadFileLength=0; 
			 pRequestContext->nDownLoadState=READY;
			 sprintf(pRequestContext->szURLFilePath,"%sdownload/data/%s",temp1,pRequestContext->szFileName);
			 sprintf(pRequestContext->szSaveFilePath,"%s\\Extract\\Download\\%s",m_szCurrentDir,pRequestContext->szFileName);
             InsertDownLoadDataToList(pRequestContext );
		}
		else
		   AfxMessageBox("�������ѱ����");
         break; 
    case 1001:
	case StockBaseInfo:             //��������
	     bOpenAddStockDlg=TRUE;
         break; 
	case NewUpdateBaseInfo:         //�������
		if(!CheckList("shF10.zip",QBGP))
		{
			 pRequestContext=new REQUEST_CONTEXT;
			 pRequestContext->DownloadDataType=ShNewUpdateBaseInfo;
			 strcpy(pRequestContext->szFileName ,"shF10.zip");
			 pRequestContext->DownloadDataCount=0;
			 pRequestContext->kind=QBGP;
             pRequestContext->DownloadFileLength=0; 
			 pRequestContext->nDownLoadState=READY;
			 sprintf(pRequestContext->szURLFilePath,"%sdownload/%s",temp1,pRequestContext->szFileName);
			 sprintf(pRequestContext->szSaveFilePath,"%s\\Extract\\Download\\%s",m_szCurrentDir,pRequestContext->szFileName);
             InsertDownLoadDataToList(pRequestContext );
		}
		else
		   AfxMessageBox("�������ѱ����");
		if(!CheckList("szF10.zip",QBGP))
		{
			 pRequestContext=new REQUEST_CONTEXT;
			 pRequestContext->DownloadDataType=SzNewUpdateBaseInfo;
			 strcpy(pRequestContext->szFileName ,"szF10.zip");
			 pRequestContext->DownloadDataCount=0;
			 pRequestContext->kind=QBGP;
             pRequestContext->DownloadFileLength=0; 
			 pRequestContext->nDownLoadState=READY;
			 sprintf(pRequestContext->szURLFilePath,"%sdownload/%s",temp1,pRequestContext->szFileName);
			 sprintf(pRequestContext->szSaveFilePath,"%s\\Extract\\Download\\%s",m_szCurrentDir,pRequestContext->szFileName);
             InsertDownLoadDataToList(pRequestContext );
		}
		else
		   AfxMessageBox("�������ѱ����");
		if(!CheckList("ebF10.zip",QBGP))
		{
			 pRequestContext=new REQUEST_CONTEXT;
			 pRequestContext->DownloadDataType=EbNewUpdateBaseInfo;
			 strcpy(pRequestContext->szFileName ,"ebF10.zip");
			 pRequestContext->kind=QBGP;
			 pRequestContext->DownloadDataCount=0;
             pRequestContext->DownloadFileLength=0; 
			 pRequestContext->nDownLoadState=READY;
			 sprintf(pRequestContext->szURLFilePath,"%sdownload/%s",temp1,pRequestContext->szFileName);
			 sprintf(pRequestContext->szSaveFilePath,"%s\\Extract\\Download\\%s",m_szCurrentDir,pRequestContext->szFileName);
             InsertDownLoadDataToList(pRequestContext );
		}
		else
		   AfxMessageBox("�������ѱ����");
         break; 
    case 1002:                       
	case StockMinute:               //���ɷ�ʱ
	     bOpenAddStockDlg=TRUE;
         break; 
	case ShZsMinute:                //��ָ֤��
		if(!CheckList("��ָ.zip",SHZS))
		{
			 pRequestContext=new REQUEST_CONTEXT;
			 pRequestContext->DownloadDataType=ShZsMinute;
			 strcpy(pRequestContext->szFileName ,"��ָ.zip");
			 pRequestContext->DownloadDataCount=0;
			 pRequestContext->kind=SHZS;
             pRequestContext->DownloadFileLength=0; 
			 pRequestContext->nDownLoadState=READY;
			 sprintf(pRequestContext->szURLFilePath,"%sdownload/min/%s",temp1,pRequestContext->szFileName);
			 sprintf(pRequestContext->szSaveFilePath,"%s\\Extract\\Download\\%s",m_szCurrentDir,pRequestContext->szFileName);
             InsertDownLoadDataToList(pRequestContext );
		}
		else
		   AfxMessageBox("�������ѱ����");
         break; 
	case ShAgMinute:                //��֤����
		if(!CheckList("��A.zip",SHAG))
		{
			 pRequestContext=new REQUEST_CONTEXT;
			 pRequestContext->DownloadDataType=ShAgMinute;
			 strcpy(pRequestContext->szFileName ,"��A.zip");
			 pRequestContext->kind=SHAG;
			 pRequestContext->DownloadDataCount=0;
             pRequestContext->DownloadFileLength=0; 
			 pRequestContext->nDownLoadState=READY;
			 sprintf(pRequestContext->szURLFilePath,"%sdownload/min/%s",temp1,pRequestContext->szFileName);
			 sprintf(pRequestContext->szSaveFilePath,"%s\\Extract\\Download\\%s",m_szCurrentDir,pRequestContext->szFileName);
             InsertDownLoadDataToList(pRequestContext );
		}
		else
		   AfxMessageBox("�������ѱ����");
         break; 
	case ShBgMinute:                //��֤�¹�
		if(!CheckList("��B.zip",SHBG))
		{
			 pRequestContext=new REQUEST_CONTEXT;
			 pRequestContext->DownloadDataType=ShBgMinute;
			 strcpy(pRequestContext->szFileName ,"��B.zip");
			 pRequestContext->kind=SHBG;
			 pRequestContext->DownloadDataCount=0;
             pRequestContext->DownloadFileLength=0; 
			 pRequestContext->nDownLoadState=READY;
			 sprintf(pRequestContext->szURLFilePath,"%sdownload/min/%s",temp1,pRequestContext->szFileName);
			 sprintf(pRequestContext->szSaveFilePath,"%s\\Extract\\Download\\%s",m_szCurrentDir,pRequestContext->szFileName);
             InsertDownLoadDataToList(pRequestContext );
		}
		else
		   AfxMessageBox("�������ѱ����");
         break; 
	case ShJjMinute:                //��֤����
		if(!CheckList("������.zip",SHJJ))
		{
			pRequestContext=new REQUEST_CONTEXT;
			pRequestContext->DownloadDataType=ShJjMinute;
			strcpy(pRequestContext->szFileName ,"������.zip");
			pRequestContext->kind=SHJJ;
			pRequestContext->DownloadDataCount=0;
			pRequestContext->DownloadFileLength=0; 
			pRequestContext->nDownLoadState=READY;
			sprintf(pRequestContext->szURLFilePath,"%sdownload/min/%s",temp1,pRequestContext->szFileName);
			sprintf(pRequestContext->szSaveFilePath,"%s\\Extract\\Download\\%s",m_szCurrentDir,pRequestContext->szFileName);
			InsertDownLoadDataToList(pRequestContext );
		}
		else
			AfxMessageBox("�������ѱ����");
		break; 
	case ShZqMinute:                //��֤ծȯ
		if(!CheckList("��ծȯ.zip",SHZQ))
		{
			 pRequestContext=new REQUEST_CONTEXT;
			 pRequestContext->DownloadDataType=ShZqMinute;
			 strcpy(pRequestContext->szFileName ,"��ծȯ.zip");
			 pRequestContext->kind=SHZQ;
			 pRequestContext->DownloadDataCount=0;
             pRequestContext->DownloadFileLength=0; 
			 pRequestContext->nDownLoadState=READY;
			 sprintf(pRequestContext->szURLFilePath,"%sdownload/min/%s",temp1,pRequestContext->szFileName);
			 sprintf(pRequestContext->szSaveFilePath,"%s\\Extract\\Download\\%s",m_szCurrentDir,pRequestContext->szFileName);
             InsertDownLoadDataToList(pRequestContext );
		}
		else
		   AfxMessageBox("�������ѱ����");
         break; 
	case ShZzMinute:                //��֤תծ
		if(!CheckList("��תծ.zip",SHZZ))
		{
			 pRequestContext=new REQUEST_CONTEXT;
			 pRequestContext->DownloadDataType=ShZzMinute;
			 strcpy(pRequestContext->szFileName ,"��תծ.zip");
			 pRequestContext->kind=SHZZ;
			 pRequestContext->DownloadDataCount=0;
             pRequestContext->DownloadFileLength=0; 
			 pRequestContext->nDownLoadState=READY;
			 sprintf(pRequestContext->szURLFilePath,"%sdownload/min/%s",temp1,pRequestContext->szFileName);
			 sprintf(pRequestContext->szSaveFilePath,"%s\\Extract\\Download\\%s",m_szCurrentDir,pRequestContext->szFileName);
             InsertDownLoadDataToList(pRequestContext );
		}
		else
		   AfxMessageBox("�������ѱ����");
         break; 
	case ShHgMinute:                //��֤�ع�
		if(!CheckList("���ع�.zip",SHHG))
		{
			pRequestContext=new REQUEST_CONTEXT;
			pRequestContext->DownloadDataType=ShHgMinute;
			strcpy(pRequestContext->szFileName ,"���ع�.zip");
			pRequestContext->kind=SHHG;
			pRequestContext->DownloadDataCount=0;
			pRequestContext->DownloadFileLength=0; 
			pRequestContext->nDownLoadState=READY;
			sprintf(pRequestContext->szURLFilePath,"%sdownload/min/%s",temp1,pRequestContext->szFileName);
			sprintf(pRequestContext->szSaveFilePath,"%s\\Extract\\Download\\%s",m_szCurrentDir,pRequestContext->szFileName);
			InsertDownLoadDataToList(pRequestContext );
		}
		else
			AfxMessageBox("�������ѱ����");
		break; 
	case ShEfMinute:                //��֤ETF
		if(!CheckList("��ETF.zip",SHETF))
		{
			pRequestContext=new REQUEST_CONTEXT;
			pRequestContext->DownloadDataType=ShEfMinute;
			strcpy(pRequestContext->szFileName ,"��ETF.zip");
			pRequestContext->kind=SHETF;
			pRequestContext->DownloadDataCount=0;
			pRequestContext->DownloadFileLength=0; 
			pRequestContext->nDownLoadState=READY;
			sprintf(pRequestContext->szURLFilePath,"%sdownload/min/%s",temp1,pRequestContext->szFileName);
			sprintf(pRequestContext->szSaveFilePath,"%s\\Extract\\Download\\%s",m_szCurrentDir,pRequestContext->szFileName);
			InsertDownLoadDataToList(pRequestContext );
		}
		else
			AfxMessageBox("�������ѱ����");
		break; 
	case ShKjMinute:                //��֤���Ż���
		if(!CheckList("�����Ż���.zip",SHKF))
		{
			pRequestContext=new REQUEST_CONTEXT;
			pRequestContext->DownloadDataType=ShKjMinute;
			strcpy(pRequestContext->szFileName ,"�����Ż���.zip");
			pRequestContext->kind=SHKF;
			pRequestContext->DownloadDataCount=0;
			pRequestContext->DownloadFileLength=0; 
			pRequestContext->nDownLoadState=READY;
			sprintf(pRequestContext->szURLFilePath,"%sdownload/min/%s",temp1,pRequestContext->szFileName);
			sprintf(pRequestContext->szSaveFilePath,"%s\\Extract\\Download\\%s",m_szCurrentDir,pRequestContext->szFileName);
			InsertDownLoadDataToList(pRequestContext );
		}
		else
			AfxMessageBox("�������ѱ����");
		break; 
	case ShQzMinute:                //��֤Ȩ֤
		if(!CheckList("��Ȩ֤.zip",SHQZ))
		{
			pRequestContext=new REQUEST_CONTEXT;
			pRequestContext->DownloadDataType=ShQzMinute;
			strcpy(pRequestContext->szFileName ,"��Ȩ֤.zip");
			pRequestContext->kind=SHQZ;
			pRequestContext->DownloadDataCount=0;
			pRequestContext->DownloadFileLength=0; 
			pRequestContext->nDownLoadState=READY;
			sprintf(pRequestContext->szURLFilePath,"%sdownload/min/%s",temp1,pRequestContext->szFileName);
			sprintf(pRequestContext->szSaveFilePath,"%s\\Extract\\Download\\%s",m_szCurrentDir,pRequestContext->szFileName);
			InsertDownLoadDataToList(pRequestContext );
		}
		else
			AfxMessageBox("�������ѱ����");
		break; 
	case ShOtMinute:                //��֤����
		if(!CheckList("������.zip",SHOT))
		{
			pRequestContext=new REQUEST_CONTEXT;
			pRequestContext->DownloadDataType=ShOtMinute;
			strcpy(pRequestContext->szFileName ,"������.zip");
			pRequestContext->kind=SHOT;
			pRequestContext->DownloadDataCount=0;
			pRequestContext->DownloadFileLength=0; 
			pRequestContext->nDownLoadState=READY;
			sprintf(pRequestContext->szURLFilePath,"%sdownload/min/%s",temp1,pRequestContext->szFileName);
			sprintf(pRequestContext->szSaveFilePath,"%s\\Extract\\Download\\%s",m_szCurrentDir,pRequestContext->szFileName);
			InsertDownLoadDataToList(pRequestContext );
		}
		else
			AfxMessageBox("�������ѱ����");
		break; 

	case SzZsMinute:                //��ָ֤��
		if(!CheckList("��ָ.zip",SZZS))
		{
			 pRequestContext=new REQUEST_CONTEXT;
			 pRequestContext->DownloadDataType=SzZsMinute;
			 strcpy(pRequestContext->szFileName ,"��ָ.zip");
			 pRequestContext->kind=SZZS;
			 pRequestContext->DownloadDataCount=0;
             pRequestContext->DownloadFileLength=0; 
			 pRequestContext->nDownLoadState=READY;
			 sprintf(pRequestContext->szURLFilePath,"%sdownload/min/%s",temp1,pRequestContext->szFileName);
			 sprintf(pRequestContext->szSaveFilePath,"%s\\Extract\\Download\\%s",m_szCurrentDir,pRequestContext->szFileName);
             InsertDownLoadDataToList(pRequestContext );
		}
		else
		   AfxMessageBox("�������ѱ����");
         break; 
	case SzAgMinute:                //��֤����
		if(!CheckList("��A.zip",SZAG))
		{
			 pRequestContext=new REQUEST_CONTEXT;
			 pRequestContext->DownloadDataType=SzAgMinute;
			 strcpy(pRequestContext->szFileName ,"��A.zip");
			 pRequestContext->kind=SZAG;
			 pRequestContext->DownloadDataCount=0;
             pRequestContext->DownloadFileLength=0; 
			 pRequestContext->nDownLoadState=READY;
			 sprintf(pRequestContext->szURLFilePath,"%sdownload/min/%s",temp1,pRequestContext->szFileName);
			 sprintf(pRequestContext->szSaveFilePath,"%s\\Extract\\Download\\%s",m_szCurrentDir,pRequestContext->szFileName);
             InsertDownLoadDataToList(pRequestContext );
		}
		else
		   AfxMessageBox("�������ѱ����");
         break; 
	case SzBgMinute:                //��֤�¹�
		if(!CheckList("��B.zip",SZBG))
		{
			 pRequestContext=new REQUEST_CONTEXT;
			 pRequestContext->DownloadDataType=SzBgMinute;
			 strcpy(pRequestContext->szFileName ,"��B.zip");
			 pRequestContext->DownloadDataCount=0;
			 pRequestContext->kind=SZBG;
             pRequestContext->DownloadFileLength=0; 
			 pRequestContext->nDownLoadState=READY;
			 sprintf(pRequestContext->szURLFilePath,"%sdownload/min/%s",temp1,pRequestContext->szFileName);
			 sprintf(pRequestContext->szSaveFilePath,"%s\\Extract\\Download\\%s",m_szCurrentDir,pRequestContext->szFileName);
             InsertDownLoadDataToList(pRequestContext );
		}
		else
		   AfxMessageBox("�������ѱ����");
         break; 
	case SzJjMinute:                //��֤����
		if(!CheckList("�����.zip",SZJJ))
		{
			 pRequestContext=new REQUEST_CONTEXT;
			 pRequestContext->DownloadDataType=SzJjMinute;
			 strcpy(pRequestContext->szFileName ,"�����.zip");
			 pRequestContext->kind=SZJJ;
			 pRequestContext->DownloadDataCount=0;
             pRequestContext->DownloadFileLength=0; 
			 pRequestContext->nDownLoadState=READY;
			 sprintf(pRequestContext->szURLFilePath,"%sdownload/min/%s",temp1,pRequestContext->szFileName);
			 sprintf(pRequestContext->szSaveFilePath,"%s\\Extract\\Download\\%s",m_szCurrentDir,pRequestContext->szFileName);
             InsertDownLoadDataToList(pRequestContext );
		}
		else
		   AfxMessageBox("�������ѱ����");
         break; 

	case SzZqMinute:                //��֤ծȯ
		if(!CheckList("��ծȯ.zip",SZZQ))
		{
			pRequestContext=new REQUEST_CONTEXT;
			pRequestContext->DownloadDataType=SzZqMinute;
			strcpy(pRequestContext->szFileName ,"��ծȯ.zip");
			pRequestContext->kind=SZZQ;
			pRequestContext->DownloadDataCount=0;
			pRequestContext->DownloadFileLength=0; 
			pRequestContext->nDownLoadState=READY;
			sprintf(pRequestContext->szURLFilePath,"%sdownload/min/%s",temp1,pRequestContext->szFileName);
			sprintf(pRequestContext->szSaveFilePath,"%s\\Extract\\Download\\%s",m_szCurrentDir,pRequestContext->szFileName);
			InsertDownLoadDataToList(pRequestContext );
		}
		else
			AfxMessageBox("�������ѱ����");
		break; 
	case SzZzMinute:                //��֤תծ
		if(!CheckList("��תծ.zip",SZZZ))
		{
			pRequestContext=new REQUEST_CONTEXT;
			pRequestContext->DownloadDataType=SzZzMinute;
			strcpy(pRequestContext->szFileName ,"��תծ.zip");
			pRequestContext->kind=SZZZ;
			pRequestContext->DownloadDataCount=0;
			pRequestContext->DownloadFileLength=0; 
			pRequestContext->nDownLoadState=READY;
			sprintf(pRequestContext->szURLFilePath,"%sdownload/min/%s",temp1,pRequestContext->szFileName);
			sprintf(pRequestContext->szSaveFilePath,"%s\\Extract\\Download\\%s",m_szCurrentDir,pRequestContext->szFileName);
			InsertDownLoadDataToList(pRequestContext );
		}
		else
			AfxMessageBox("�������ѱ����");
		break; 
	case SzHgMinute:                //��֤�ع�
		if(!CheckList("��ع�.zip",SZHG))
		{
			pRequestContext=new REQUEST_CONTEXT;
			pRequestContext->DownloadDataType=SzHgMinute;
			strcpy(pRequestContext->szFileName ,"��ع�.zip");
			pRequestContext->kind=SZHG;
			pRequestContext->DownloadDataCount=0;
			pRequestContext->DownloadFileLength=0; 
			pRequestContext->nDownLoadState=READY;
			sprintf(pRequestContext->szURLFilePath,"%sdownload/min/%s",temp1,pRequestContext->szFileName);
			sprintf(pRequestContext->szSaveFilePath,"%s\\Extract\\Download\\%s",m_szCurrentDir,pRequestContext->szFileName);
			InsertDownLoadDataToList(pRequestContext );
		}
		else
			AfxMessageBox("�������ѱ����");
		break; 
	case SzEfMinute:                //��֤ETF
		if(!CheckList("��ETF.zip",SZETF))
		{
			pRequestContext=new REQUEST_CONTEXT;
			pRequestContext->DownloadDataType=SzEfMinute;
			strcpy(pRequestContext->szFileName ,"��ETF.zip");
			pRequestContext->kind=SZETF;
			pRequestContext->DownloadDataCount=0;
			pRequestContext->DownloadFileLength=0; 
			pRequestContext->nDownLoadState=READY;
			sprintf(pRequestContext->szURLFilePath,"%sdownload/min/%s",temp1,pRequestContext->szFileName);
			sprintf(pRequestContext->szSaveFilePath,"%s\\Extract\\Download\\%s",m_szCurrentDir,pRequestContext->szFileName);
			InsertDownLoadDataToList(pRequestContext );
		}
		else
			AfxMessageBox("�������ѱ����");
		break; 
	case SzKjMinute:                //��֤���Ż���
		if(!CheckList("��Ż���.zip",SZKF))
		{
			pRequestContext=new REQUEST_CONTEXT;
			pRequestContext->DownloadDataType=SzKjMinute;
			strcpy(pRequestContext->szFileName ,"��Ż���.zip");
			pRequestContext->kind=SZKF;
			pRequestContext->DownloadDataCount=0;
			pRequestContext->DownloadFileLength=0; 
			pRequestContext->nDownLoadState=READY;
			sprintf(pRequestContext->szURLFilePath,"%sdownload/min/%s",temp1,pRequestContext->szFileName);
			sprintf(pRequestContext->szSaveFilePath,"%s\\Extract\\Download\\%s",m_szCurrentDir,pRequestContext->szFileName);
			InsertDownLoadDataToList(pRequestContext );
		}
		else
			AfxMessageBox("�������ѱ����");
		break; 
	case SzQzMinute:                //��֤Ȩ֤
		if(!CheckList("��Ȩ֤.zip",SZQZ))
		{
			pRequestContext=new REQUEST_CONTEXT;
			pRequestContext->DownloadDataType=SzQzMinute;
			strcpy(pRequestContext->szFileName ,"��Ȩ֤.zip");
			pRequestContext->kind=SZQZ;
			pRequestContext->DownloadDataCount=0;
			pRequestContext->DownloadFileLength=0; 
			pRequestContext->nDownLoadState=READY;
			sprintf(pRequestContext->szURLFilePath,"%sdownload/min/%s",temp1,pRequestContext->szFileName);
			sprintf(pRequestContext->szSaveFilePath,"%s\\Extract\\Download\\%s",m_szCurrentDir,pRequestContext->szFileName);
			InsertDownLoadDataToList(pRequestContext );
		}
		else
			AfxMessageBox("�������ѱ����");
		break; 
	case SzSbMinute:                //��֤����
		if(!CheckList("������.zip",SZSB))
		{
			pRequestContext=new REQUEST_CONTEXT;
			pRequestContext->DownloadDataType=SzSbMinute;
			strcpy(pRequestContext->szFileName ,"������.zip");
			pRequestContext->kind=SZSB;
			pRequestContext->DownloadDataCount=0;
			pRequestContext->DownloadFileLength=0; 
			pRequestContext->nDownLoadState=READY;
			sprintf(pRequestContext->szURLFilePath,"%sdownload/min/%s",temp1,pRequestContext->szFileName);
			sprintf(pRequestContext->szSaveFilePath,"%s\\Extract\\Download\\%s",m_szCurrentDir,pRequestContext->szFileName);
			InsertDownLoadDataToList(pRequestContext );
		}
		else
			AfxMessageBox("�������ѱ����");
		break; 
	case SzOtMinute:                //��֤����
		if(!CheckList("������.zip",SZOT))
		{
			pRequestContext=new REQUEST_CONTEXT;
			pRequestContext->DownloadDataType=SzOtMinute;
			strcpy(pRequestContext->szFileName ,"������.zip");
			pRequestContext->kind=SZOT;
			pRequestContext->DownloadDataCount=0;
			pRequestContext->DownloadFileLength=0; 
			pRequestContext->nDownLoadState=READY;
			sprintf(pRequestContext->szURLFilePath,"%sdownload/min/%s",temp1,pRequestContext->szFileName);
			sprintf(pRequestContext->szSaveFilePath,"%s\\Extract\\Download\\%s",m_szCurrentDir,pRequestContext->szFileName);
			InsertDownLoadDataToList(pRequestContext );
		}
		else
			AfxMessageBox("�������ѱ����");
		break; 
	case SzZxMinute:                //��֤����
		if(!CheckList("����С.zip",SZZXQY))
		{
			pRequestContext=new REQUEST_CONTEXT;
			pRequestContext->DownloadDataType=SzZxMinute;
			strcpy(pRequestContext->szFileName ,"����С.zip");
			pRequestContext->kind=SZZXQY;
			pRequestContext->DownloadDataCount=0;
			pRequestContext->DownloadFileLength=0; 
			pRequestContext->nDownLoadState=READY;
			sprintf(pRequestContext->szURLFilePath,"%sdownload/min/%s",temp1,pRequestContext->szFileName);
			sprintf(pRequestContext->szSaveFilePath,"%s\\Extract\\Download\\%s",m_szCurrentDir,pRequestContext->szFileName);
			InsertDownLoadDataToList(pRequestContext );
		}
		else
			AfxMessageBox("�������ѱ����");
		break; 

	case  SzCyMinute:                //����ָ��
		if(!CheckList("��ҵ��.zip",SZCYB))
		{
			 pRequestContext=new REQUEST_CONTEXT;
			 pRequestContext->DownloadDataType=SzCyMinute;
			 strcpy(pRequestContext->szFileName ,"��ҵ��.zip");
			 pRequestContext->kind=SZCYB;
			 pRequestContext->DownloadDataCount=0;
             pRequestContext->DownloadFileLength=0; 
			 pRequestContext->nDownLoadState=READY;
			 sprintf(pRequestContext->szURLFilePath,"%sdownload/min/%s",temp1,pRequestContext->szFileName);
			 sprintf(pRequestContext->szSaveFilePath,"%s\\Extract\\Download\\%s",m_szCurrentDir,pRequestContext->szFileName);
             InsertDownLoadDataToList(pRequestContext );
		}
		else
		   AfxMessageBox("�������ѱ����");
         break; 
    case 1003:                      //������Ϣ
		  if(!CheckList("����Ϣ.dat",QBGP))
		  {
		      char szLastDate[10];
			  pRequestContext=new REQUEST_CONTEXT;
			  pRequestContext->DownloadDataType=ShInfomation;
			  strcpy(pRequestContext->szFileName ,"����Ϣ.dat");
			  pRequestContext->kind=QBGP;
			  pRequestContext->DownloadDataCount=0;
              pRequestContext->DownloadFileLength=0; 
			  pRequestContext->nDownLoadState=READY;
		      GetLastDayInfomationDate(szLastDate);
		      sprintf(pRequestContext->szURLFilePath,"%sdownload/News/Shanghai/%s.dat",temp1,szLastDate);
			  sprintf(pRequestContext->szSaveFilePath,"%s\\News\\Shanghai\\%s.dat",m_szCurrentDir,szLastDate);
              InsertDownLoadDataToList(pRequestContext );
		  }
		  else
			  AfxMessageBox("�������ѱ����");
		  if(!CheckList("����Ϣ.dat",QBGP))
		  {
		      char szLastDate[10];
			  pRequestContext=new REQUEST_CONTEXT;
			  pRequestContext->DownloadDataType=SzInfomation;
			  strcpy(pRequestContext->szFileName ,"����Ϣ.dat");
			  pRequestContext->DownloadDataCount=0;
              pRequestContext->DownloadFileLength=0; 
			  pRequestContext->kind=QBGP;
			  pRequestContext->nDownLoadState=READY;
		      GetLastDayInfomationDate(szLastDate);
		      sprintf(pRequestContext->szURLFilePath,"%sdownload/News/Shenzhen/%s.dat",temp1,szLastDate);
			  sprintf(pRequestContext->szSaveFilePath,"%s\\News\\Shenzhen\\%s.dat",m_szCurrentDir,szLastDate);
              InsertDownLoadDataToList(pRequestContext );
		  }
		  else
			  AfxMessageBox("�������ѱ����");
		  if(!CheckList("�����Ϣ.dat",QBGP))
		  {
		      char szLastDate[10];
			  pRequestContext=new REQUEST_CONTEXT;
			  pRequestContext->DownloadDataType=EbInfomation;
			  pRequestContext->kind=QBGP;
			  strcpy(pRequestContext->szFileName ,"�����Ϣ.dat");
			  pRequestContext->DownloadDataCount=0;
              pRequestContext->DownloadFileLength=0; 
			  pRequestContext->nDownLoadState=READY;
		      GetLastDayInfomationDate(szLastDate);
		      sprintf(pRequestContext->szURLFilePath,"%sdownload/News/erban/%s.dat",temp1,szLastDate);
			  sprintf(pRequestContext->szSaveFilePath,"%s\\News\\erban\\%s.dat",m_szCurrentDir,szLastDate);
              InsertDownLoadDataToList(pRequestContext );
		  }
		  else
			  AfxMessageBox("�������ѱ����");
		  if(!CheckList("�ƾ���Ϣ.dat",QBGP))
		  {
		      char szLastDate[10];
			  pRequestContext=new REQUEST_CONTEXT;
			  pRequestContext->DownloadDataType=ZjInfomation;
			  strcpy(pRequestContext->szFileName ,"�ƾ���Ϣ.dat");
			  pRequestContext->DownloadDataCount=0;
			  pRequestContext->kind=QBGP;
              pRequestContext->DownloadFileLength=0; 
			  pRequestContext->nDownLoadState=READY;
		      GetLastDayInfomationDate(szLastDate);
		      sprintf(pRequestContext->szURLFilePath,"%sdownload/News/cj/%s.dat",temp1,szLastDate);
			  sprintf(pRequestContext->szSaveFilePath,"%s\\News\\cj\\%s.dat",m_szCurrentDir,szLastDate);
              InsertDownLoadDataToList(pRequestContext );
		  }
		  else
			  AfxMessageBox("�������ѱ����");
		  break; 
    case ShInfomation:              //��֤����
	  if(!CheckList("����Ϣ.dat",QBGP))
	  {
		  char szLastDate[10];
		  pRequestContext=new REQUEST_CONTEXT;
		  pRequestContext->DownloadDataType=ShInfomation;
		  strcpy(pRequestContext->szFileName ,"����Ϣ.dat");
	      pRequestContext->kind=QBGP;
		  pRequestContext->DownloadDataCount=0;
          pRequestContext->DownloadFileLength=0; 
		  pRequestContext->nDownLoadState=READY;
		  GetLastDayInfomationDate(szLastDate);
		  sprintf(pRequestContext->szURLFilePath,"%sdownload/News/Shanghai/%s.dat",temp1,szLastDate);
		  sprintf(pRequestContext->szSaveFilePath,"%s\\News\\Shanghai\\%s.dat",m_szCurrentDir,szLastDate);
          InsertDownLoadDataToList(pRequestContext );
	  }
	  else
	      AfxMessageBox("�������ѱ����");
          break; 
	case SzInfomation:              //��֤����  
	  if(!CheckList("����Ϣ.dat",QBGP))
	  {
		  char szLastDate[10];
		  pRequestContext=new REQUEST_CONTEXT;
		  pRequestContext->DownloadDataType=SzInfomation;
		  strcpy(pRequestContext->szFileName ,"����Ϣ.dat");
	      pRequestContext->DownloadDataCount=0;
	      pRequestContext->kind=QBGP;
          pRequestContext->DownloadFileLength=0; 
		  pRequestContext->nDownLoadState=READY;
	      GetLastDayInfomationDate(szLastDate);
	      sprintf(pRequestContext->szURLFilePath,"%sdownload/News/Shenzhen/%s.dat",temp1,szLastDate);
		  sprintf(pRequestContext->szSaveFilePath,"%s\\News\\shenzhen\\%s.dat",m_szCurrentDir,szLastDate);
          InsertDownLoadDataToList(pRequestContext );
	  }
	  else
	      AfxMessageBox("�������ѱ����");
         break; 
	case EbInfomation:              //�ƾ�����
		  if(!CheckList("�����Ϣ.dat",QBGP))
		  {
		      char szLastDate[10];
			  pRequestContext=new REQUEST_CONTEXT;
			  pRequestContext->DownloadDataType=EbInfomation;
			  strcpy(pRequestContext->szFileName ,"�����Ϣ.dat");
			  pRequestContext->DownloadDataCount=0;
	          pRequestContext->kind=QBGP;
              pRequestContext->DownloadFileLength=0; 
			  pRequestContext->nDownLoadState=READY;
		      GetLastDayInfomationDate(szLastDate);
		      sprintf(pRequestContext->szURLFilePath,"%sdownload/News/erban/%s.dat",temp1,szLastDate);
			  sprintf(pRequestContext->szSaveFilePath,"%s\\News\\erban\\%s.dat",m_szCurrentDir,szLastDate);
              InsertDownLoadDataToList(pRequestContext );
		  }
		  else
			  AfxMessageBox("�������ѱ����");
		  break;
	case ZjInfomation:              //�ƾ�����
	  if(!CheckList("�ƾ���Ϣ.dat",QBGP))
	  {
		  char szLastDate[10];
		  pRequestContext=new REQUEST_CONTEXT;
		  pRequestContext->DownloadDataType=ZjInfomation;
		  strcpy(pRequestContext->szFileName ,"�ƾ���Ϣ.dat");
		  pRequestContext->DownloadDataCount=0;
          pRequestContext->DownloadFileLength=0; 
		  pRequestContext->nDownLoadState=READY;
          pRequestContext->kind=QBGP;
	      GetLastDayInfomationDate(szLastDate);
		  sprintf(pRequestContext->szURLFilePath,"%sdownload/News/cj/%s.dat",temp1,szLastDate);
		  sprintf(pRequestContext->szSaveFilePath,"%s\\News\\cj\\%s.dat",m_szCurrentDir,szLastDate);
          InsertDownLoadDataToList(pRequestContext );
	  }
	  else
	      AfxMessageBox("�������ѱ����");
         break; 
	case 1004:		//��ʷ��ϸ����
		CSelectStockHsDetail2 dlg(this);
		dlg.m_bScreenStockBlock = true;
		if (dlg.DoModal()==IDOK)
		{
			int nCount=dlg.m_FileArray.GetSize();
			CString strFile;
			CString strDate;
			CString strName;
			for (int i=0;i<nCount;i++)
			{
				strFile=dlg.m_FileArray.GetAt(i);
				strDate=strFile.Mid(2,8);
				strName=strFile.Left(2)+strFile.Mid(10);
				if(!CheckList(strFile.GetBuffer(0),QBGP))
				{
					pRequestContext=new REQUEST_CONTEXT;
					pRequestContext->DownloadDataType=TaiDetailData;
					strcpy(pRequestContext->szFileName,strFile.GetBuffer(0));
					pRequestContext->DownloadDataCount=0;
	                pRequestContext->kind=QBGP;
					pRequestContext->DownloadFileLength=0; 
					pRequestContext->nDownLoadState=READY;
					sprintf(pRequestContext->szURLFilePath,"%sdownload/Detail/%s/%s",temp1,strDate,strName);
					sprintf(pRequestContext->szSaveFilePath,"%s/Extract/download/Detail/%s",m_szCurrentDir,strFile);
					InsertDownLoadDataToList(pRequestContext );
				}
			}
		}
		break;
	}
    if(bOpenAddStockDlg)
	{
		SelectStock pDlg(this,TRUE);
		pDlg.DoModal();
	}
    if(bOpenSelectHistoryDlg)
	{
		CDayLineSelect pDlg(nDayLineType,this);
		pDlg.DoModal();
	}
	PostMessage(WM_USER_POSTFOCUS,0,0);
}
BOOL CDownLoadDlg::CheckList(char *szAddName,int nKind)
{
	int nCount=m_lstStockTypeName.GetItemCount();
	for( int i=0;i<nCount;i++)
	{
	    int ItemData=m_lstStockTypeName.GetItemData(i);
        REQUEST_CONTEXT *pRequestContext=(REQUEST_CONTEXT *)ItemData;
		if(strcmp(pRequestContext->szFileName,szAddName)!=0)
			continue;
		if(nKind!=pRequestContext->kind)
			continue;
		else
			return TRUE;
	}
	return FALSE;
}
BOOL CDownLoadDlg::InsertDownLoadDataToList(REQUEST_CONTEXT *pRequestContext )
{
	LV_ITEM  lvitem;
	int  iIcon, iItem, iSubItem, iActualItem;
    iItem=this->m_lstStockTypeName.GetItemCount();          //�õ�������λ��
	CString strIconDesc[5];

	pRequestContext->pWnd=this;	 	//���ھ��
    pRequestContext->hResource=NULL;
	strcpy(	pRequestContext->szMemo,""+g_strCompanyName+"��������ϵͳ");
	pRequestContext->bResourceOpen=FALSE;
	pRequestContext->bIsStopDownload=FALSE;
    pRequestContext->bIsContinueDown=FALSE;
	pRequestContext->downloaderrors=0;
    pRequestContext->LastDownloadCount=0;
    pRequestContext->DownLoadNotChangeCount=0;

    pRequestContext->dwThreadID=-1;
	pRequestContext->hThread=NULL;
    strIconDesc[0]=pRequestContext->szFileName;
	switch(pRequestContext->DownloadDataType)       //Ȩ������
	{
		case StockName:     strIconDesc[1]="��Ʊ����";
			break;
		case TodayDayLine:  strIconDesc[1]="��������";
			break;
		case MonthDayLine:  strIconDesc[1]="��������"; 
			break;
		case HistroyDayLine:strIconDesc[1]="��ʷ����";
			break;
		case ExRightData:   strIconDesc[1]="��Ȩ����";
			break;
		case CaiwuData:   strIconDesc[1]="��������";
			break;
		case StockBaseInfo: strIconDesc[1]="��������";
			break;
		case ShNewUpdateBaseInfo:strIconDesc[1]="��֤�������";
			break;
		case SzNewUpdateBaseInfo: strIconDesc[1]="��֤�������";
			break;
		case EbNewUpdateBaseInfo: strIconDesc[1]="�����������";
			break;
		case StockMinute:        strIconDesc[1]="���ɷ�ʱ";
			break;
		case ShZsMinute:         strIconDesc[1]="��ָ֤��";
			break;
		case ShAgMinute:         strIconDesc[1]="��֤����";
			break;
		case ShBgMinute:         strIconDesc[1]="��֤�¹�";
			break;
		case ShJjMinute:         strIconDesc[1]="��֤����";
			break;
		case ShZqMinute:         strIconDesc[1]="��֤ծȯ";
			break;
		case ShZzMinute:         strIconDesc[1]="��֤תծ";
			break;
		case ShHgMinute:         strIconDesc[1]="��֤�ع�";
			break;
		case ShEfMinute:         strIconDesc[1]="��֤ETF";
			break;
		case ShKjMinute:         strIconDesc[1]="��֤���Ż���";
			break;
		case ShQzMinute:         strIconDesc[1]="��֤Ȩ֤";
			break;
		case ShOtMinute:         strIconDesc[1]="��֤����";
			break;
		case SzZsMinute:         strIconDesc[1]="��ָ֤��";
			break;
		case SzAgMinute:         strIconDesc[1]="��֤����";
			break;
		case SzBgMinute:         strIconDesc[1]="��֤�¹�";
			break;
		case SzZqMinute:         strIconDesc[1]="��֤ծȯ";
			break;
		case SzJjMinute:         strIconDesc[1]="��֤����";
			break;
		case SzZzMinute:         strIconDesc[1]="��֤תծ";
			break;
		case SzHgMinute:         strIconDesc[1]="��֤�ع�";
			break;
		case SzEfMinute:         strIconDesc[1]="��֤ETF";
			break;
		case SzKjMinute:         strIconDesc[1]="��֤���Ż���";
			break;
		case SzQzMinute:         strIconDesc[1]="��֤Ȩ֤";
			break;
		case SzSbMinute:         strIconDesc[1]="��֤����";
			break;
		case SzOtMinute:         strIconDesc[1]="��֤����";
			break;
		case SzCyMinute:         strIconDesc[1]="��ҵ��" ;     //����ָ��
			break;
		case ShInfomation:       strIconDesc[1]="��֤����";
			break;
		case SzInfomation:       strIconDesc[1]="��֤����";  
			break;
		case EbInfomation:       strIconDesc[1]="�������";  
			break;
		case ZjInfomation:       strIconDesc[1]="�ƾ�����";
			break;
		case TaiDetailData:			strIconDesc[1]="��ϸ����";
			break;
	}
    strIconDesc[2].Format("%d",pRequestContext->DownloadDataCount);
    strIconDesc[3].Format("%d",pRequestContext->DownloadFileLength);

	switch(pRequestContext->nDownLoadState)       //Ȩ������
	{
	case  READY:  strIconDesc[4]="׼��״̬";
          break;					  
	}
	for (iSubItem = 0; iSubItem <5; iSubItem++)
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

void CDownLoadDlg::OnOK() 
{
	// TODO: Add extra validation here
	if(m_bIsCalc)
	{
		AfxMessageBox("�������ڰ�װ���ݲ��ܽ����˳�����!");
		return;
	}

	 gbCloseDialog=TRUE;
     KillTimer(ID_DOWNLOAD_STATE);
     if(m_InfomationContext.bResourceOpen)
	 {
	    InternetCloseHandle(m_InfomationContext.hResource);
		m_InfomationContext.bResourceOpen=FALSE;
		Sleep(100);
	 }
	 if(m_InfomationContext.hThread)
	 {
		if(!TerminateThread( m_InfomationContext.hThread,m_InfomationContext.dwThreadID))
		{
			AfxMessageBox("kill thread error");
		}
		CloseHandle(m_InfomationContext.hThread);
		m_InfomationContext.hThread=NULL;
        m_InfomationContext.dwThreadID=-1;
	 }
     int nCount=m_lstStockTypeName.GetItemCount();
	 int i;
	 for(i=0;i<nCount;i++)
	 {
		 int ItemData=m_lstStockTypeName.GetItemData(i);
         REQUEST_CONTEXT *pRequestContext=(REQUEST_CONTEXT *)ItemData;
         if(pRequestContext->bResourceOpen)
		 {
	        InternetCloseHandle(pRequestContext->hResource);
			Sleep(100);
		 }
		 if(pRequestContext->hThread)
		 {
			if(!TerminateThread( pRequestContext->hThread,pRequestContext->dwThreadID))
			{
				AfxMessageBox("kill thread error");
			}
			CloseHandle(pRequestContext->hThread);
			pRequestContext->hThread=NULL;
		 }
	 }
	 InternetCloseHandle(hOpen);
	 for( i=0;i<nCount;i++)
	 {
		 int ItemData=m_lstStockTypeName.GetItemData(i);
         REQUEST_CONTEXT *pRequestContext=(REQUEST_CONTEXT *)ItemData;
		 delete pRequestContext;
	 }
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
	CDialog::OnOK();
    DestroyWindow();	

}

void CDownLoadDlg::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	if(m_bIsCalc)
	{
		AfxMessageBox("�������ڰ�װ���ݲ��ܽ����˳�����!");
		return;
	}

	 gbCloseDialog=TRUE;
     KillTimer(ID_DOWNLOAD_STATE);
     if(m_InfomationContext.bResourceOpen)
	 {
	    InternetCloseHandle(m_InfomationContext.hResource);
		m_InfomationContext.bResourceOpen=FALSE;
		Sleep(100);
	 }
	 if(m_InfomationContext.hThread)
	 {
		if(!TerminateThread( m_InfomationContext.hThread,m_InfomationContext.dwThreadID))
		{
			AfxMessageBox("kill thread error");
		}
		CloseHandle(m_InfomationContext.hThread);
		m_InfomationContext.hThread=NULL;
        m_InfomationContext.dwThreadID=-1;
	 }
     int nCount=m_lstStockTypeName.GetItemCount();
	 int i;
	 for(i=0;i<nCount;i++)
	 {
		 int ItemData=m_lstStockTypeName.GetItemData(i);
         REQUEST_CONTEXT *pRequestContext=(REQUEST_CONTEXT *)ItemData;
         if(pRequestContext->bResourceOpen)
		 {
	        InternetCloseHandle(pRequestContext->hResource);
			pRequestContext->bResourceOpen=FALSE;
			Sleep(100);
		 }
		 if(pRequestContext->hThread)
		 {
			if(!TerminateThread( pRequestContext->hThread,pRequestContext->dwThreadID))
			{
				AfxMessageBox("kill thread error");
			}
			CloseHandle(pRequestContext->hThread);
			pRequestContext->hThread=NULL;
            pRequestContext->dwThreadID=-1;
		 }
	 }
	 InternetCloseHandle(hOpen);
	 for(i=0;i<nCount;i++)
	 {
		 int ItemData=m_lstStockTypeName.GetItemData(i);
         REQUEST_CONTEXT *pRequestContext=(REQUEST_CONTEXT *)ItemData;
		 delete pRequestContext;
	 }

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
	
	CDialog::OnClose();
    DestroyWindow();	
}

BOOL CDownLoadDlg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if(pMsg->message==WM_CHAR&&((pMsg->wParam>='0'&&pMsg->wParam<='9')||(pMsg->wParam>='A'&&pMsg->wParam<='z')))
	{
		HTREEITEM hTreeItem;
		REQUEST_CONTEXT *pRequestContext;
		char temp[250];
		CString temp1;
		temp[0] = 0;
//		GetPrivateProfileString("3WADDRESS","Address",m_pDoc->m_SystemInitData.NetAddr,temp,250,"Config.ini");
	    strcpy(temp,m_pDoc->m_SystemInitData.NetAddr);
		temp1.Format("%s",temp);
		strcpy(temp,"/");
		if(temp1.Right(1) != '/')
		  temp1 = temp1 + temp;

		hTreeItem=this->m_MyTree.GetSelectedItem();
		if(hTreeItem==NULL)
		{
			return TRUE;
		}
		UINT nId=this->m_MyTree.GetItemData(hTreeItem); 
		   
		CTaiKeyBoardAngelDlg m_diakeybrode;
		if(pMsg->wParam>='a'&&pMsg->wParam<='z')
			pMsg->wParam-=32;
		char input=pMsg->wParam;
		m_diakeybrode.input=input;
		if(pMsg->wParam>='0'&&pMsg->wParam<='9')
			m_diakeybrode.ischar=false;
		else
			m_diakeybrode.ischar=true;

		m_diakeybrode.DoModal();
		if(m_diakeybrode.isresultok)
		{
		   char result[10];                //���صĽ��
           strcpy(result,m_diakeybrode.result);
		   int nKind=m_diakeybrode.m_stkKind;
		   if(strlen(result)==6||strlen(result)==4)
		   {
				switch(nId)
				{
					case 1001:
					case StockBaseInfo:             //��������
						{
							char szfileName[255];
							sprintf(szfileName,"%s.txt",result);
							if(!CheckList(szfileName,-1-nKind))
							{
								 pRequestContext=new REQUEST_CONTEXT;
								 pRequestContext->DownloadDataType=StockBaseInfo;
								 strcpy(pRequestContext->szFileName ,szfileName);
                                 pRequestContext->kind=-1-nKind;
								 pRequestContext->DownloadDataCount=0;
								 pRequestContext->DownloadFileLength=0; 
								 pRequestContext->nDownLoadState=READY;
								 sprintf(pRequestContext->szURLFilePath,"%sdownload/F10/%s",temp1,pRequestContext->szFileName);
 								 if(nKind==SHAG||nKind==SHBG||nKind==SHZQ||nKind==SHZZ||nKind==SHHG||nKind==SHETF||nKind==SHKF||nKind==SHQZ||nKind==SHOT)
				 					 sprintf(pRequestContext->szSaveFilePath,"%s\\%s%s",m_szCurrentDir,g_sF10sh,pRequestContext->szFileName);
								 else if(nKind==SZAG||nKind==SZBG||nKind==SZZQ||nKind==SZZZ||nKind==SZHG||nKind==SZETF||
										nKind==SZKF||nKind==SZQZ||nKind==SZOT||nKind==SZSB||nKind==SZZXQY||nKind==SZCYB)
									 sprintf(pRequestContext->szSaveFilePath,"%s\\%s%s",m_szCurrentDir,g_sF10sz,pRequestContext->szFileName);
							     InsertDownLoadDataToList(pRequestContext );
							}
							else
							   AfxMessageBox("�������ѱ����");

						}
						 break;
					case 1002:                       
					case StockMinute:               //���ɷ�ʱ
						{
							char szfileName[255];
							sprintf(szfileName,"%s.1mn",result);
							if(!CheckList(szfileName,-1-nKind))
							{
								 pRequestContext=new REQUEST_CONTEXT;
								 switch(nKind)
								 {
									case SHZS:
										sprintf(pRequestContext->szURLFilePath,"%sdownload/MIN/��ָ/%s",temp1,szfileName);
										break;
									case SHAG:
										sprintf(pRequestContext->szURLFilePath,"%sdownload/MIN/��A/%s",temp1,szfileName);
										break;
									case SHBG:
										sprintf(pRequestContext->szURLFilePath,"%sdownload/MIN/��B/%s",temp1,szfileName);
										break;
									case SHZQ:
										sprintf(pRequestContext->szURLFilePath,"%sdownload/MIN/��ծȯ/%s",temp1,szfileName);
										break;
									case SHJJ:
										sprintf(pRequestContext->szURLFilePath,"%sdownload/MIN/������/%s",temp1,szfileName);
										break;
									case SHZZ:
										sprintf(pRequestContext->szURLFilePath,"%sdownload/MIN/��תծ/%s",temp1,szfileName);
										break;
									case SHHG:
										sprintf(pRequestContext->szURLFilePath,"%sdownload/MIN/���ع�/%s",temp1,szfileName);
										break;
									case SHETF:
										sprintf(pRequestContext->szURLFilePath,"%sdownload/MIN/��ETF/%s",temp1,szfileName);
										break;
									case SHKF:
										sprintf(pRequestContext->szURLFilePath,"%sdownload/MIN/�����Ż���/%s",temp1,szfileName);
										break;
									case SHQZ:
										sprintf(pRequestContext->szURLFilePath,"%sdownload/MIN/��Ȩ֤/%s",temp1,szfileName);
										break;
									case SHOT:
										sprintf(pRequestContext->szURLFilePath,"%sdownload/MIN/������/%s",temp1,szfileName);
										break;
									case SZZS:
										sprintf(pRequestContext->szURLFilePath,"%sdownload/MIN/��ָ/%s",temp1,szfileName);
										break;
									case SZAG:
										sprintf(pRequestContext->szURLFilePath,"%sdownload/MIN/��A/%s",temp1,szfileName);
										break;
									case SZBG:
										sprintf(pRequestContext->szURLFilePath,"%sdownload/MIN/��B/%s",temp1,szfileName);
										break;
									case SZJJ:
										sprintf(pRequestContext->szURLFilePath,"%sdownload/MIN/�����/%s",temp1,szfileName);
										break;
									case SZZQ:
										sprintf(pRequestContext->szURLFilePath,"%sdownload/MIN/��ծȯ/%s",temp1,szfileName);
										break;
									case SZZZ:
										sprintf(pRequestContext->szURLFilePath,"%sdownload/MIN/��תծ/%s",temp1,szfileName);
										break;
									case SZHG:
										sprintf(pRequestContext->szURLFilePath,"%sdownload/MIN/��ع�/%s",temp1,szfileName);
										break;
									case SZETF:
										sprintf(pRequestContext->szURLFilePath,"%sdownload/MIN/��ETF/%s",temp1,szfileName);
										break;
									case SZKF:
										sprintf(pRequestContext->szURLFilePath,"%sdownload/MIN/��Ż���/%s",temp1,szfileName);
										break;
									case SZQZ:
										sprintf(pRequestContext->szURLFilePath,"%sdownload/MIN/��Ȩ֤/%s",temp1,szfileName);
										break;
									case SZSB:
										sprintf(pRequestContext->szURLFilePath,"%sdownload/MIN/������/%s",temp1,szfileName);
										break;
									case SZOT:
										sprintf(pRequestContext->szURLFilePath,"%sdownload/MIN/������/%s",temp1,szfileName);
										break;
									case SZZXQY:
										sprintf(pRequestContext->szURLFilePath,"%sdownload/MIN/����С/%s",temp1,szfileName);
										break;
									case SZCYB:
										sprintf(pRequestContext->szURLFilePath,"%sdownload/MIN/��ҵ��/%s",temp1,szfileName);
										break;
								 }
								 pRequestContext->DownloadDataType=StockMinute;
								 strcpy(pRequestContext->szFileName ,szfileName);
								 pRequestContext->kind=-1-nKind;
								 pRequestContext->DownloadDataCount=0;
								 pRequestContext->DownloadFileLength=0; 
								 pRequestContext->nDownLoadState=READY;
 			                     sprintf(pRequestContext->szSaveFilePath,"%s\\Extract\\Download\\%s",m_szCurrentDir,pRequestContext->szFileName);

								 InsertDownLoadDataToList(pRequestContext );
							}
							else
							   AfxMessageBox("�������ѱ����");

						}
                         break;
				}
		   }
		}
		return true;
	}
	return CDialog::PreTranslateMessage(pMsg);
}
LRESULT CDownLoadDlg::OnPostFocus(WPARAM wParam, LPARAM lParam)
{
	int nCount=m_lstStockTypeName.GetItemCount();
	if(nCount>0)
	{
       m_lstStockTypeName.SetFocus();
       m_lstStockTypeName.SetItemState(0,LVIS_FOCUSED|LVIS_SELECTED , LVIS_SELECTED|LVIS_FOCUSED) ;
	}
	return 0L;
}
LRESULT CDownLoadDlg::OnDispalyDownLoadInfomation(WPARAM wParam, LPARAM lParam)
{
    REQUEST_CONTEXT *pRequestContext=(REQUEST_CONTEXT *)lParam;
	DispalyDownLoadInfomation(pRequestContext);
	return 0L;
}
LRESULT CDownLoadDlg::OnAddStockDayLine(WPARAM wParam, LPARAM lParam)
{
	ADDDAYLINEDATA *pAddDayLine;
	char temp[250];
	CString temp1;
	temp[0] = 0;
//	GetPrivateProfileString("3WADDRESS","Address",m_pDoc->m_SystemInitData.NetAddr,temp,250,"Config.ini");
	strcpy(temp,m_pDoc->m_SystemInitData.NetAddr);
	temp1.Format("%s",temp);
	strcpy(temp,"/");
	if(temp1.Right(1) != '/')
	  temp1 = temp1 + temp;
	int lsCount=(int)lParam;
    pAddDayLine=(ADDDAYLINEDATA *)wParam;
	for(int i=0;i<lsCount;i++)
	{
		REQUEST_CONTEXT *pRequestContext; 
		char szfileName[255];
		switch(pAddDayLine[i].DayLineType)
		{
		case 0:
				sprintf(szfileName,"%sd.zip",pAddDayLine[i].DayLineName);
				if(!CheckList(szfileName,QBGP))
				{
					 pRequestContext=new REQUEST_CONTEXT;
					 pRequestContext->DownloadDataType=MonthDayLine;
					 strcpy(pRequestContext->szFileName ,szfileName);
					 pRequestContext->DownloadDataCount=0;
	                 pRequestContext->kind=QBGP;
					 pRequestContext->DownloadFileLength=0; 
					 pRequestContext->nDownLoadState=READY;
				     sprintf(pRequestContext->szURLFilePath,"%sdownload/data/%s",temp1,pRequestContext->szFileName);
	                 sprintf(pRequestContext->szSaveFilePath,"%s\\Extract\\Download\\%s",m_szCurrentDir,pRequestContext->szFileName);
					 InsertDownLoadDataToList(pRequestContext );
				}
				else
				   AfxMessageBox("�������ѱ����");
			   break;
		case 1:
		case 2:
				sprintf(szfileName,"%sd.zip",pAddDayLine[i].DayLineName);
				if(!CheckList(szfileName,QBGP))
				{
					 pRequestContext=new REQUEST_CONTEXT;
					 pRequestContext->DownloadDataType=HistroyDayLine;
					 strcpy(pRequestContext->szFileName ,szfileName);
	                 pRequestContext->kind=QBGP;
					 pRequestContext->DownloadDataCount=0;
					 pRequestContext->DownloadFileLength=0; 
					 pRequestContext->nDownLoadState=READY;
				     sprintf(pRequestContext->szURLFilePath,"%sdownload/data/%s",temp1,pRequestContext->szFileName);
	                 sprintf(pRequestContext->szSaveFilePath,"%s\\Extract\\Download\\%s",m_szCurrentDir,pRequestContext->szFileName);
					 InsertDownLoadDataToList(pRequestContext );
				}
				else
				   AfxMessageBox("�������ѱ����");
                break;
		}
	}
	return 1L;
}
LRESULT CDownLoadDlg::OnAddStockCode(WPARAM wParam, LPARAM lParam) //���ӹ�Ʊ��LISTCTRL
{
	SHARES_DATA_ADD *pAddCode;
	int lsCount=(int)lParam;
    pAddCode=(SHARES_DATA_ADD *)wParam;
	HTREEITEM hTreeItem;
	REQUEST_CONTEXT *pRequestContext;
	char temp[250];
	CString temp1;
	temp[0] = 0;
//	GetPrivateProfileString("3WADDRESS","Address",m_pDoc->m_SystemInitData.NetAddr,temp,250,"Config.ini");
	strcpy(temp,m_pDoc->m_SystemInitData.NetAddr);
	temp1.Format("%s",temp);
	strcpy(temp,"/");
	if(temp1.Right(1) != '/')
	  temp1 = temp1 + temp;
	hTreeItem=this->m_MyTree.GetSelectedItem();
	if(hTreeItem==NULL)
	{
		return 0L;
	}
	UINT nId=this->m_MyTree.GetItemData(hTreeItem); 
	for(int i=0;i<lsCount;i++)
	{
		if(pAddCode[i].StockType<0)
		{
			int nKind=-1-pAddCode[i].StockType;
			switch(nId)
			{
				case 1001:
				case StockBaseInfo:             //��������
					{
						char szfileName[255];
						sprintf(szfileName,"%s.txt",pAddCode[i].name);
						if(!CheckList(szfileName,pAddCode[i].StockType))
						{
							 pRequestContext=new REQUEST_CONTEXT;
							 pRequestContext->DownloadDataType=StockBaseInfo;
							 strcpy(pRequestContext->szFileName ,szfileName);
                             pRequestContext->kind=pAddCode[i].StockType; 
							 pRequestContext->DownloadDataCount=0;
							 pRequestContext->DownloadFileLength=0; 
							 pRequestContext->nDownLoadState=READY;
							 sprintf(pRequestContext->szURLFilePath,"%sdownload/F10/%s",temp1,pRequestContext->szFileName);
							 if(nKind==SHAG||nKind==SHBG||nKind==SHZQ||nKind==SHZZ||nKind==SHHG||nKind==SHETF||nKind==SHKF||nKind==SHQZ||nKind==SHOT)
					 			 sprintf(pRequestContext->szSaveFilePath,"%s\\%s%s",m_szCurrentDir,g_sF10sh,pRequestContext->szFileName);
							 else if(nKind==SZAG||nKind==SZBG||nKind==SZZQ||nKind==SZZZ||nKind==SZHG||nKind==SZETF||nKind==SZKF||nKind==SZQZ||
								 nKind==SZOT||nKind==SZSB||nKind==SZZXQY||nKind==SZCYB)
			                     sprintf(pRequestContext->szSaveFilePath,"%s\\%s%s",m_szCurrentDir,g_sF10sz,pRequestContext->szFileName);
							 InsertDownLoadDataToList(pRequestContext );
						}
						else
						   AfxMessageBox("�������ѱ����");
					}
					 break;
				case 1002:                       
				case StockMinute:               //���ɷ�ʱ
					{
						char szfileName[255];
						sprintf(szfileName,"%s.1mn",pAddCode[i].name);
						if(!CheckList(szfileName,pAddCode[i].StockType))
						{
							 pRequestContext=new REQUEST_CONTEXT;
							 switch(nKind)
							 {
							 case SHZS:
								 sprintf(pRequestContext->szURLFilePath,"%sdownload/MIN/��ָ/%s",temp1,szfileName);
								 break;
							 case SHAG:
								 sprintf(pRequestContext->szURLFilePath,"%sdownload/MIN/��A/%s",temp1,szfileName);
								 break;
							 case SHBG:
								 sprintf(pRequestContext->szURLFilePath,"%sdownload/MIN/��B/%s",temp1,szfileName);
								 break;
							 case SHZQ:
								 sprintf(pRequestContext->szURLFilePath,"%sdownload/MIN/��ծȯ/%s",temp1,szfileName);
								 break;
							 case SHJJ:
								 sprintf(pRequestContext->szURLFilePath,"%sdownload/MIN/������/%s",temp1,szfileName);
								 break;
							 case SHZZ:
								 sprintf(pRequestContext->szURLFilePath,"%sdownload/MIN/��תծ/%s",temp1,szfileName);
								 break;
							 case SHHG:
								 sprintf(pRequestContext->szURLFilePath,"%sdownload/MIN/���ع�/%s",temp1,szfileName);
								 break;
							 case SHETF:
								 sprintf(pRequestContext->szURLFilePath,"%sdownload/MIN/��ETF/%s",temp1,szfileName);
								 break;
							 case SHKF:
								 sprintf(pRequestContext->szURLFilePath,"%sdownload/MIN/�����Ż���/%s",temp1,szfileName);
								 break;
							 case SHQZ:
								 sprintf(pRequestContext->szURLFilePath,"%sdownload/MIN/��Ȩ֤/%s",temp1,szfileName);
								 break;
							 case SHOT:
								 sprintf(pRequestContext->szURLFilePath,"%sdownload/MIN/������/%s",temp1,szfileName);
								 break;
							 case SZZS:
								 sprintf(pRequestContext->szURLFilePath,"%sdownload/MIN/��ָ/%s",temp1,szfileName);
								 break;
							 case SZAG:
								 sprintf(pRequestContext->szURLFilePath,"%sdownload/MIN/��A/%s",temp1,szfileName);
								 break;
							 case SZBG:
								 sprintf(pRequestContext->szURLFilePath,"%sdownload/MIN/��B/%s",temp1,szfileName);
								 break;
							 case SZJJ:
								 sprintf(pRequestContext->szURLFilePath,"%sdownload/MIN/�����/%s",temp1,szfileName);
								 break;
							 case SZZQ:
								 sprintf(pRequestContext->szURLFilePath,"%sdownload/MIN/��ծȯ/%s",temp1,szfileName);
								 break;
							 case SZZZ:
								 sprintf(pRequestContext->szURLFilePath,"%sdownload/MIN/��תծ/%s",temp1,szfileName);
								 break;
							 case SZHG:
								 sprintf(pRequestContext->szURLFilePath,"%sdownload/MIN/��ع�/%s",temp1,szfileName);
								 break;
							 case SZETF:
								 sprintf(pRequestContext->szURLFilePath,"%sdownload/MIN/��ETF/%s",temp1,szfileName);
								 break;
							 case SZKF:
								 sprintf(pRequestContext->szURLFilePath,"%sdownload/MIN/��Ż���/%s",temp1,szfileName);
								 break;
							 case SZQZ:
								 sprintf(pRequestContext->szURLFilePath,"%sdownload/MIN/��Ȩ֤/%s",temp1,szfileName);
								 break;
							 case SZSB:
								 sprintf(pRequestContext->szURLFilePath,"%sdownload/MIN/������/%s",temp1,szfileName);
								 break;
							 case SZOT:
								 sprintf(pRequestContext->szURLFilePath,"%sdownload/MIN/������/%s",temp1,szfileName);
								 break;	
							 case SZZXQY:
								 sprintf(pRequestContext->szURLFilePath,"%sdownload/MIN/����С/%s",temp1,szfileName);
								 break;							 
							 case SZCYB:
								 sprintf(pRequestContext->szURLFilePath,"%sdownload/MIN/��ҵ��/%s",temp1,szfileName);
								 break;							 
							 }

							 pRequestContext->DownloadDataType=StockMinute;
							 strcpy(pRequestContext->szFileName ,szfileName);
                             pRequestContext->kind=pAddCode[i].StockType;
							 pRequestContext->DownloadDataCount=0;
							 pRequestContext->DownloadFileLength=0; 
							 pRequestContext->nDownLoadState=READY;
			                 sprintf(pRequestContext->szSaveFilePath,"%s\\Extract\\Download\\%s",m_szCurrentDir,pRequestContext->szFileName);
							 InsertDownLoadDataToList(pRequestContext );
						}
						else
						   AfxMessageBox("�������ѱ����");

					}
                     break;
			}
		}
	}
	return 1L;
}

void CDownLoadDlg::OnDeleteLoadData() 
{
	// TODO: Add your control notification handler code here
    CString Id;
	int temp=m_lstStockTypeName.GetNextItem( -1, LVNI_SELECTED   ) ;
	if(temp==-1)
	{
		AfxMessageBox("��ѡȡɾ����");
		return;
	}
	REQUEST_CONTEXT *pRequestContext;
	pRequestContext=(REQUEST_CONTEXT *)m_lstStockTypeName.GetItemData(temp);
	if(pRequestContext->bResourceOpen||pRequestContext->hThread)
	{
		int rtn = MessageBox("���ļ����ڽ������ӻ����ز���!�Ƿ��������ɾ������?","����",MB_YESNO|MB_ICONWARNING);
		if(rtn==6)
		{
			 if(pRequestContext->bResourceOpen)
			 {
				InternetCloseHandle(pRequestContext->hResource);
                pRequestContext->bResourceOpen=FALSE;
				Sleep(100);
			 }
			 if(pRequestContext->hThread)
			 {
				if(!TerminateThread( pRequestContext->hThread,pRequestContext->dwThreadID))
				{
					AfxMessageBox("kill thread error");
				}
				CloseHandle(pRequestContext->hThread);
				pRequestContext->hThread=NULL;
                pRequestContext->dwThreadID=-1;
			 }
			Sleep(100);
			delete pRequestContext;
     		m_lstStockTypeName.DeleteItem(temp);
		}
	}
	else
	{
		delete pRequestContext;
     	m_lstStockTypeName.DeleteItem(temp);
	}
	int nCount=m_lstStockTypeName.GetItemCount();
	if(nCount>0)
	{
       m_lstStockTypeName.SetFocus();
       m_lstStockTypeName.SetItemState(0,LVIS_FOCUSED|LVIS_SELECTED , LVIS_SELECTED|LVIS_FOCUSED) ;
	}


}
void CDownLoadDlg::AddDownLoadData(int mode)
{
	 if(gbCloseDialog)
		return;
	 int index=0;
	 if(m_InfomationContext.hThread!=NULL)
         index++;
	 int nCount=m_lstStockTypeName.GetItemCount();
	 for(int i=0;i<nCount;i++)
	 {
		 int ItemData=m_lstStockTypeName.GetItemData(i);
		 REQUEST_CONTEXT *pRequestContext=(REQUEST_CONTEXT *)ItemData;
		 if(pRequestContext->nDownLoadState==DOWNLOADCOMPLETE||pRequestContext->nDownLoadState==INSTALLCOMPLETE)
			 continue;
		 if(pRequestContext->nDownLoadState==READY)
		 {
		   AsyncDownLoadDirect(pRequestContext);
		   index++;
		 }
		 else
		   index++;
		 if(index==2)
			 break;
	 }
}
void CDownLoadDlg::OnDownloadStart() 
{
	// TODO: Add your control notification handler code here
	if(m_InfomationContext.nDownLoadState!=DOWNLOADCOMPLETE&&m_InfomationContext.hThread==NULL)
	{
        AsyncDownLoadDirect(&m_InfomationContext);
	}
	 int nCount=m_lstStockTypeName.GetItemCount();
	 for(int i=0;i<nCount;i++)
	 {
		 int ItemData=m_lstStockTypeName.GetItemData(i);
         REQUEST_CONTEXT *pRequestContext=(REQUEST_CONTEXT *)ItemData;
		 if(pRequestContext->nDownLoadState==INSTALLCOMPLETE||pRequestContext->nDownLoadState==DOWNLOADCOMPLETE)
			 continue;
		 if(pRequestContext->hThread!=NULL)
			 continue;
         pRequestContext->nDownLoadState=READY;
		 pRequestContext->hResource=NULL;
		 strcpy(	pRequestContext->szMemo,""+g_strCompanyName+"��������ϵͳ");
		 pRequestContext->bResourceOpen=FALSE;
		 pRequestContext->bIsStopDownload=FALSE;
	     pRequestContext->bIsContinueDown=FALSE;
	     pRequestContext->downloaderrors=0;
         pRequestContext->LastDownloadCount=0;
	     pRequestContext->DownLoadNotChangeCount=0;

	     pRequestContext->dwThreadID=-1;
	     pRequestContext->hThread=NULL;
	 }
     AddDownLoadData(0);
	 SetTimer(ID_DOWNLOAD_STATE,5000,NULL);
}

void CDownLoadDlg::OnStopDownload() 
{
	// TODO: Add your control notification handler code here
	int rtn = MessageBox("�Ƿ�ֹͣȫ�����ز���!","����",MB_YESNO|MB_ICONWARNING);
	if(rtn!=6)
	{
		int temp=m_lstStockTypeName.GetNextItem( -1, LVNI_SELECTED   ) ;
		if(temp==-1)
        {
		    AfxMessageBox("��ѡֹͣ���ز�����!");
			return;
		}
		REQUEST_CONTEXT *pRequestContext;
		pRequestContext=(REQUEST_CONTEXT *)m_lstStockTypeName.GetItemData(temp);
		pRequestContext->bIsStopDownload=TRUE;
		Sleep(2000);
		if(pRequestContext->bResourceOpen||pRequestContext->hThread)
		{
			 if(pRequestContext->bResourceOpen)
			 {
				InternetCloseHandle(pRequestContext->hResource);
				pRequestContext->bResourceOpen=FALSE;
				Sleep(100);
			 }
			 if(pRequestContext->hThread)
			 {
				if(!TerminateThread( pRequestContext->hThread,pRequestContext->dwThreadID))
				{
					AfxMessageBox("kill thread error");
				}
				CloseHandle(pRequestContext->hThread);
				pRequestContext->hThread=NULL;
				pRequestContext->dwThreadID=-1;
			 }
			 pRequestContext->nDownLoadState=STOPDOWNLOAD;
             DispalyDownLoadInfomation(pRequestContext);
			 Sleep(100);
		}
        return;
	}
	else
	{
		int nCount=m_lstStockTypeName.GetItemCount();
		for(int i=0;i<nCount;i++)
		{
		   int ItemData=m_lstStockTypeName.GetItemData(i);
		   REQUEST_CONTEXT *pRequestContext=(REQUEST_CONTEXT *)ItemData;
		   pRequestContext->bIsStopDownload=TRUE;
		}
	}
	int nCount=m_lstStockTypeName.GetItemCount();
	if(nCount>0)
	{
       m_lstStockTypeName.SetFocus();
       m_lstStockTypeName.SetItemState(0,LVIS_FOCUSED|LVIS_SELECTED , LVIS_SELECTED|LVIS_FOCUSED) ;
	}

}

void CDownLoadDlg::OnInstallLoaddata() 
{
	// TODO: Add your control notification handler code here
  	 m_bIsCalc=TRUE;
	CMenu *pMenu=GetSystemMenu(FALSE);
	pMenu->EnableMenuItem(SC_CLOSE,MF_BYCOMMAND|MF_DISABLED|MF_GRAYED); 
    CWnd *pWnd=this->GetDlgItem(IDOK);
	pWnd->EnableWindow(FALSE);
    pWnd=this->GetDlgItem(IDC_INSTALL_LOADDATA);
	pWnd->EnableWindow(FALSE);
     
	 int nCount=m_lstStockTypeName.GetItemCount();
	 for(int i=0;i<nCount;i++)
	 {
		 int ItemData=m_lstStockTypeName.GetItemData(i);
         REQUEST_CONTEXT *pRequestContext=(REQUEST_CONTEXT *)ItemData;
		 if(pRequestContext->nDownLoadState!=DOWNLOADCOMPLETE||pRequestContext->nDownLoadState==INSTALLCOMPLETE)
			 continue;
		 try
		 {
		     IZipPtr pIZip(__uuidof(Zip));
		     IUnZipPtr pIUnZip(pIZip);
		     pIUnZip->InitUnZip();

			 switch(pRequestContext->DownloadDataType)
			 {
				case StockName:                 //��Ʊ����
						pIUnZip->SetExtractFile(pRequestContext->szSaveFilePath);
						pIUnZip->SetDestPath("EXTRACT");
						pIUnZip->Extract();
						if(!InstallStockName(pRequestContext))
						{
							pRequestContext->nDownLoadState=NOTINSTALLCOMPLETE;
							DispalyDownLoadInfomation(pRequestContext);
							continue;
						}
						break; 
				case TodayDayLine:              //��������
						pIUnZip->SetExtractFile(pRequestContext->szSaveFilePath);
						pIUnZip->SetDestPath("EXTRACT");
						pIUnZip->Extract();
						if(!InstallStockDayLine(pRequestContext))
						{
							pRequestContext->nDownLoadState=NOTINSTALLCOMPLETE;
							DispalyDownLoadInfomation(pRequestContext);
							continue;
						}
					 break; 
				case MonthDayLine:              //�������� 
						pIUnZip->SetExtractFile(pRequestContext->szSaveFilePath);
						pIUnZip->SetDestPath("EXTRACT");
						pIUnZip->Extract();
						if(!InstallStockDayLine(pRequestContext))
						{
							pRequestContext->nDownLoadState=NOTINSTALLCOMPLETE;
							DispalyDownLoadInfomation(pRequestContext);
							continue;
						}

					 break; 
				case HistroyDayLine:            //��ʷ����
						pIUnZip->SetExtractFile(pRequestContext->szSaveFilePath);
						pIUnZip->SetDestPath("EXTRACT");
						pIUnZip->Extract();
						if(!InstallStockDayLine(pRequestContext))
						{
							pRequestContext->nDownLoadState=NOTINSTALLCOMPLETE;
							DispalyDownLoadInfomation(pRequestContext);
							continue;
						}
					 break; 
				    case ExRightData:               //��Ȩ����
						pIUnZip->SetExtractFile(pRequestContext->szSaveFilePath);
						pIUnZip->SetDestPath("EXTRACT");
						pIUnZip->Extract();
						if(!InstallStockChuQuan(pRequestContext))
						{
							pRequestContext->nDownLoadState=NOTINSTALLCOMPLETE;
							DispalyDownLoadInfomation(pRequestContext);
							continue;
						}

					 break; 
				    case CaiwuData:               //��������
						pIUnZip->SetExtractFile(pRequestContext->szSaveFilePath);
						pIUnZip->SetDestPath("EXTRACT");
						pIUnZip->Extract();
						if(!InstallStockCaiwuData(pRequestContext))
						{
							pRequestContext->nDownLoadState=NOTINSTALLCOMPLETE;
							DispalyDownLoadInfomation(pRequestContext);
							continue;
						}

					 break; 
				case 1001:
				case StockBaseInfo:             //��������
					 break; 
				case ShNewUpdateBaseInfo:         //�������
						{
						pIUnZip->SetExtractFile(pRequestContext->szSaveFilePath);
						CString ss = g_sF10sh;
						ss.TrimRight("\\");
						pIUnZip->SetDestPath(ss.GetBuffer(0));
						pIUnZip->Extract();
						}
					 break; 
				case SzNewUpdateBaseInfo:         //�������
					{
						CString ss = g_sF10sz;
						ss.TrimRight("\\");
						pIUnZip->SetExtractFile(pRequestContext->szSaveFilePath);
						pIUnZip->SetDestPath(ss.GetBuffer(0));
						pIUnZip->Extract();
					}
					 break; 
				case EbNewUpdateBaseInfo:         //�������
						pIUnZip->SetExtractFile(pRequestContext->szSaveFilePath);
						pIUnZip->SetDestPath("data\\sz\\f10");
						pIUnZip->Extract();
					 break; 
				case StockMinute:               //���ɷ�ʱ
					{
					    InstallStockMenute(pRequestContext->szSaveFilePath); 
					}
					 break; 
				case ShZsMinute:                //��ָ֤��
						pIUnZip->SetExtractFile(pRequestContext->szSaveFilePath);
						pIUnZip->SetDestPath("EXTRACT\\sh");
						pIUnZip->Extract();
						InstallStockMunuteForType(pRequestContext);
					 break; 
				case ShAgMinute:                //��֤����
						pIUnZip->SetExtractFile(pRequestContext->szSaveFilePath);
						pIUnZip->SetDestPath("EXTRACT\\sh");
						pIUnZip->Extract();
						InstallStockMunuteForType(pRequestContext);
					 break; 
				case ShBgMinute:                //��֤�¹�
						pIUnZip->SetExtractFile(pRequestContext->szSaveFilePath);
						pIUnZip->SetDestPath("EXTRACT\\sh");
						pIUnZip->Extract();
						InstallStockMunuteForType(pRequestContext);
					 break; 
				case ShZqMinute:                //��֤ծȯ
						pIUnZip->SetExtractFile(pRequestContext->szSaveFilePath);
						pIUnZip->SetDestPath("EXTRACT\\sh");
						pIUnZip->Extract();
						InstallStockMunuteForType(pRequestContext);
					 break; 
				case ShJjMinute:                //��֤ծȯ
					pIUnZip->SetExtractFile(pRequestContext->szSaveFilePath);
					pIUnZip->SetDestPath("EXTRACT\\sh");
					pIUnZip->Extract();
					InstallStockMunuteForType(pRequestContext);
					break; 
				case ShZzMinute:                //��֤ծȯ
					pIUnZip->SetExtractFile(pRequestContext->szSaveFilePath);
					pIUnZip->SetDestPath("EXTRACT\\sh");
					pIUnZip->Extract();
					InstallStockMunuteForType(pRequestContext);
					break; 
				case ShHgMinute:                //��֤ծȯ
					pIUnZip->SetExtractFile(pRequestContext->szSaveFilePath);
					pIUnZip->SetDestPath("EXTRACT\\sh");
					pIUnZip->Extract();
					InstallStockMunuteForType(pRequestContext);
					break; 
				case ShEfMinute:                //��֤ծȯ
					pIUnZip->SetExtractFile(pRequestContext->szSaveFilePath);
					pIUnZip->SetDestPath("EXTRACT\\sh");
					pIUnZip->Extract();
					InstallStockMunuteForType(pRequestContext);
					break; 
				case ShKjMinute:                //��֤ծȯ
					pIUnZip->SetExtractFile(pRequestContext->szSaveFilePath);
					pIUnZip->SetDestPath("EXTRACT\\sh");
					pIUnZip->Extract();
					InstallStockMunuteForType(pRequestContext);
					break; 
				case ShQzMinute:                //��֤ծȯ
					pIUnZip->SetExtractFile(pRequestContext->szSaveFilePath);
					pIUnZip->SetDestPath("EXTRACT\\sh");
					pIUnZip->Extract();
					InstallStockMunuteForType(pRequestContext);
					break; 
				case ShOtMinute:                //��֤ծȯ
					pIUnZip->SetExtractFile(pRequestContext->szSaveFilePath);
					pIUnZip->SetDestPath("EXTRACT\\sh");
					pIUnZip->Extract();
					InstallStockMunuteForType(pRequestContext);
					break; 
				case SzZsMinute:                //��ָ֤��
						pIUnZip->SetExtractFile(pRequestContext->szSaveFilePath);
						pIUnZip->SetDestPath("EXTRACT\\sz");
						pIUnZip->Extract();
						InstallStockMunuteForType(pRequestContext);
					 break; 
				case SzAgMinute:                //��֤����
						pIUnZip->SetExtractFile(pRequestContext->szSaveFilePath);
						pIUnZip->SetDestPath("EXTRACT\\sz");
						pIUnZip->Extract();
						InstallStockMunuteForType(pRequestContext);
					 break; 
				case SzBgMinute:                //��֤�¹�
						pIUnZip->SetExtractFile(pRequestContext->szSaveFilePath);
						pIUnZip->SetDestPath("EXTRACT\\sz");
						pIUnZip->Extract();
						InstallStockMunuteForType(pRequestContext);
					 break; 
				case SzZqMinute:                //��֤ծȯ
				case SzJjMinute:                //��֤ծȯ
				case SzHgMinute:                //��֤ծȯ
				case SzZzMinute:                //��֤ծȯ
				case SzEfMinute:                //��֤ծȯ
				case SzKjMinute:                //��֤ծȯ
				case SzQzMinute:                //��֤ծȯ
				case SzSbMinute:                //��֤ծȯ
				case SzOtMinute:                //��֤ծȯ
                case SzCyMinute:                //����ָ��
						pIUnZip->SetExtractFile(pRequestContext->szSaveFilePath);
						pIUnZip->SetDestPath("EXTRACT\\sz");
						pIUnZip->Extract();
						InstallStockMunuteForType(pRequestContext);
					 break; 
				case ShInfomation:              //��֤����
					  break; 
				case SzInfomation:              //��֤����  
				case EbInfomation:              //��֤����  
					 break; 
				case ZjInfomation:              //�ƾ�����
					 break; 
				case TaiDetailData:
					if (InstallStockHsDetail(pRequestContext))
					{
						pRequestContext->nDownLoadState=INSTALLCOMPLETE;
						DispalyDownLoadInfomation(pRequestContext);
					}
					else
					{
						pRequestContext->nDownLoadState=NOTINSTALLCOMPLETE;
						DispalyDownLoadInfomation(pRequestContext);
					}
					break;
			 }
		}
		catch(const _com_error& error)
		{
		  CString strError;
		  strError.Format("An Error,%x\n",error.Error());
		  AfxMessageBox(strError);
		}
        pRequestContext->nDownLoadState=INSTALLCOMPLETE;
        DispalyDownLoadInfomation(pRequestContext);
	 }
  	 m_bIsCalc=FALSE;
	 pMenu->EnableMenuItem(SC_CLOSE,MF_BYCOMMAND|MF_ENABLED); 
    pWnd=this->GetDlgItem(IDOK);
	pWnd->EnableWindow(TRUE);
    pWnd=this->GetDlgItem(IDC_INSTALL_LOADDATA);
	pWnd->EnableWindow(TRUE);

	//Added by ltr
	nCount=m_pFileHs.GetSize();
	CTaiKlineFileHS* pFileHs;
	if (nCount>0)
	{
		for (int i=0;i<nCount;i++)
		{
			pFileHs=m_pFileHs.GetAt(i);
			pFileHs->Close();
			delete pFileHs;
		}
		m_pFileHs.RemoveAll();
	}
}

void CDownLoadDlg::OnLoadCollecte() 
{
	// TODO: Add your control notification handler code here
	CString m_InFile;
	CString DefExt="*.ini";
	CString FileName="DownLoadSys.ini";
	CString Filter="power file (*.ini)|*.ini||";
	CString strdir;
	::GetCurrentDirectory(MAX_PATH,strdir.GetBuffer(MAX_PATH));

	FILE *fp;

    Time_Now = CTime::GetCurrentTime();
	REQUEST_CONTEXT *pRequestContext;
	char temp[250];
	CString temp1;
	temp[0] = 0;
//	GetPrivateProfileString("3WADDRESS","Address",m_pDoc->m_SystemInitData.NetAddr,temp,250,"Config.ini");
	strcpy(temp,m_pDoc->m_SystemInitData.NetAddr);
	temp1.Format("%s",temp);
	strcpy(temp,"/");
    if(temp1.Right(1) != '/')
	  temp1 = temp1 + temp;

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
		AfxMessageBox(m_InFile+"�ļ�������");
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
		        AfxMessageBox(m_InFile+"����������Ϣ�ļ�!");
				fclose(fp);
                return;
			}
			fread(&nCount,4,1,fp);
	
            for(int i=0;i<nCount;i++)
			{
				char filename[51];
				int  DownLoadType=0;
				int  nKindType=0; 
				int  nKind;
				fread(&DownLoadType,4,1,fp);
				fread(filename,1,50,fp);
				fread(&nKindType,1,4,fp);
				nKind=-1-nKindType;
				switch(DownLoadType)
				{
//**********************************************************
					case StockName:                 //��Ʊ����
							if(!CheckList("StockName.zip",QBGP))
							{
								 pRequestContext=new REQUEST_CONTEXT;
								 pRequestContext->DownloadDataType=StockName;
								 strcpy(pRequestContext->szFileName ,"StockName.zip");
								 pRequestContext->DownloadDataCount=0;
	                             pRequestContext->kind=QBGP;
								 pRequestContext->DownloadFileLength=0; 
								 pRequestContext->nDownLoadState=READY;
								 sprintf(pRequestContext->szURLFilePath,"%sdownload/%s",temp1,pRequestContext->szFileName);
								 sprintf(pRequestContext->szSaveFilePath,"%s\\Extract\\Download\\%s",m_szCurrentDir,pRequestContext->szFileName);
								 InsertDownLoadDataToList(pRequestContext );
							}
							break; 
					case TodayDayLine:              //��������
						{ 
							char szDate[10];
							char szFilename[256];
							sprintf(szDate,"%d%2d%2d",Time_Now.GetYear(),Time_Now.GetMonth(),Time_Now.GetDay());
							for(int i=0;i<8;i++)
							{
								if(szDate[i]==' ')
								   szDate[i]='0';
							}
							szDate[8]='\0';	
							sprintf(szFilename,"%sd.zip",szDate);
							if(!CheckList(szFilename,QBGP))
							{
								 pRequestContext=new REQUEST_CONTEXT;
								 pRequestContext->DownloadDataType=TodayDayLine;
								 strcpy(pRequestContext->szFileName ,szFilename);
								 pRequestContext->DownloadDataCount=0;
								 pRequestContext->DownloadFileLength=0; 
								 pRequestContext->nDownLoadState=READY;
	                             pRequestContext->kind=QBGP;
								 sprintf(pRequestContext->szURLFilePath,"%sdownload/data/%s",temp1,pRequestContext->szFileName);
								 sprintf(pRequestContext->szSaveFilePath,"%s\\Extract\\Download\\%s",m_szCurrentDir,pRequestContext->szFileName);
								 InsertDownLoadDataToList(pRequestContext );
							}
						}
						 break; 
					case MonthDayLine:              //�������� 
						if(!CheckList(filename,QBGP))
						{
							 pRequestContext=new REQUEST_CONTEXT;
							 pRequestContext->DownloadDataType=MonthDayLine;
							 strcpy(pRequestContext->szFileName ,filename);
							 pRequestContext->DownloadDataCount=0;
                             pRequestContext->kind=QBGP;
							 pRequestContext->DownloadFileLength=0; 
							 pRequestContext->nDownLoadState=READY;
							 sprintf(pRequestContext->szURLFilePath,"%sdownload/data/%s",temp1,pRequestContext->szFileName);
							 sprintf(pRequestContext->szSaveFilePath,"%s\\Extract\\Download\\%s",m_szCurrentDir,pRequestContext->szFileName);
							 InsertDownLoadDataToList(pRequestContext );
						}
						 break; 
					case HistroyDayLine:            //��ʷ����
						if(!CheckList(filename,QBGP))
						{
							 pRequestContext=new REQUEST_CONTEXT;
							 pRequestContext->DownloadDataType=HistroyDayLine;
							 strcpy(pRequestContext->szFileName ,filename);
							 pRequestContext->DownloadDataCount=0;
							 pRequestContext->DownloadFileLength=0; 
                             pRequestContext->kind=QBGP;
							 pRequestContext->nDownLoadState=READY;
							 sprintf(pRequestContext->szURLFilePath,"%sdownload/data/%s",temp1,pRequestContext->szFileName);
							 sprintf(pRequestContext->szSaveFilePath,"%s\\Extract\\Download\\%s",m_szCurrentDir,pRequestContext->szFileName);
							 InsertDownLoadDataToList(pRequestContext );
						}
						 break; 
					case ExRightData:               //��Ȩ����
						if(!CheckList("chuquan.zip",QBGP))
						{
							 pRequestContext=new REQUEST_CONTEXT;
							 pRequestContext->DownloadDataType=ExRightData;
							 strcpy(pRequestContext->szFileName ,"chuquan.zip");
							 pRequestContext->DownloadDataCount=0;
                             pRequestContext->kind=QBGP;
							 pRequestContext->DownloadFileLength=0; 
							 pRequestContext->nDownLoadState=READY;
							 sprintf(pRequestContext->szURLFilePath,"%sdownload/data/%s",temp1,pRequestContext->szFileName);
							 sprintf(pRequestContext->szSaveFilePath,"%s\\Extract\\Download\\%s",m_szCurrentDir,pRequestContext->szFileName);
							 InsertDownLoadDataToList(pRequestContext );
						}
						 break; 
					case CaiwuData:               //��������
						if(!CheckList("finance.zip",QBGP))
						{
							 pRequestContext=new REQUEST_CONTEXT;
							 pRequestContext->DownloadDataType=CaiwuData;
							 strcpy(pRequestContext->szFileName ,"finance.zip");
                             pRequestContext->kind=QBGP;
							 pRequestContext->DownloadDataCount=0;
							 pRequestContext->DownloadFileLength=0; 
							 pRequestContext->nDownLoadState=READY;
							 sprintf(pRequestContext->szURLFilePath,"%sdownload/data/%s",temp1,pRequestContext->szFileName);
							 sprintf(pRequestContext->szSaveFilePath,"%s\\Extract\\Download\\%s",m_szCurrentDir,pRequestContext->szFileName);
							 InsertDownLoadDataToList(pRequestContext );
						}
						 break; 
				    case StockBaseInfo:             //��������
						if(!CheckList(filename,nKindType))
						{
							 pRequestContext=new REQUEST_CONTEXT;
							 pRequestContext->DownloadDataType=StockBaseInfo;
							 strcpy(pRequestContext->szFileName ,filename);
							 pRequestContext->DownloadDataCount=0;
							 pRequestContext->DownloadFileLength=0; 
							 pRequestContext->nDownLoadState=READY;
							 pRequestContext->kind=nKindType;
							 sprintf(pRequestContext->szURLFilePath,"%sdownload/F10/%s",temp1,pRequestContext->szFileName);
							 if(nKind==SHZS||nKind==SHAG||nKind==SHBG||nKind==SHZQ||nKind==SHJJ||nKind==SHZZ||nKind==SHHG||nKind==SHETF||nKind==SHKF||nKind==SHQZ||nKind==SHOT)
					 			 sprintf(pRequestContext->szSaveFilePath,"%s\\%s%s",m_szCurrentDir,g_sF10sh,pRequestContext->szFileName);
							 else if(nKind==SZZS||nKind==SZAG||nKind==SZBG||nKind==SZZQ||nKind==SZJJ||nKind==SZZZ||nKind==SZHG||
								 nKind==SZETF||nKind==SZKF||nKind==SZQZ||nKind==SZSB||nKind==SZOT||nKind==SZZXQY||nKind==SZCYB)
			                     sprintf(pRequestContext->szSaveFilePath,"%s\\%s%s",m_szCurrentDir,g_sF10sz,pRequestContext->szFileName);
							 InsertDownLoadDataToList(pRequestContext );
						}

						 break; 
					case ShNewUpdateBaseInfo:         //�������
						if(!CheckList("shF10.zip",QBGP))
						{
							 pRequestContext=new REQUEST_CONTEXT;
							 pRequestContext->DownloadDataType=ShNewUpdateBaseInfo;
							 strcpy(pRequestContext->szFileName ,"shF10.zip");
							 pRequestContext->DownloadDataCount=0;
							 pRequestContext->DownloadFileLength=0; 
                             pRequestContext->kind=QBGP;
							 pRequestContext->nDownLoadState=READY;
							 sprintf(pRequestContext->szURLFilePath,"%sdownload/%s",temp1,pRequestContext->szFileName);
							 sprintf(pRequestContext->szSaveFilePath,"%s\\Extract\\Download\\%s",m_szCurrentDir,pRequestContext->szFileName);
							 InsertDownLoadDataToList(pRequestContext );
						}
						break;
					case SzNewUpdateBaseInfo:         //�������
						if(!CheckList("szF10.zip",QBGP))
						{
							 pRequestContext=new REQUEST_CONTEXT;
							 pRequestContext->DownloadDataType=SzNewUpdateBaseInfo;
							 strcpy(pRequestContext->szFileName ,"szF10.zip");
							 pRequestContext->DownloadDataCount=0;
                             pRequestContext->kind=QBGP;
							 pRequestContext->DownloadFileLength=0; 
							 pRequestContext->nDownLoadState=READY;
							 sprintf(pRequestContext->szURLFilePath,"%sdownload/%s",temp1,pRequestContext->szFileName);
							 sprintf(pRequestContext->szSaveFilePath,"%s\\Extract\\Download\\%s",m_szCurrentDir,pRequestContext->szFileName);
							 InsertDownLoadDataToList(pRequestContext );
						}
						break;
					case EbNewUpdateBaseInfo:         //�������
						if(!CheckList("ebF10.zip",QBGP))
						{
							 pRequestContext=new REQUEST_CONTEXT;
							 pRequestContext->DownloadDataType=EbNewUpdateBaseInfo;
                             pRequestContext->kind=QBGP;
							 strcpy(pRequestContext->szFileName ,"ebF10.zip");
							 pRequestContext->DownloadDataCount=0;
							 pRequestContext->DownloadFileLength=0; 
							 pRequestContext->nDownLoadState=READY;
							 sprintf(pRequestContext->szURLFilePath,"%sdownload/%s",temp1,pRequestContext->szFileName);
							 sprintf(pRequestContext->szSaveFilePath,"%s\\Extract\\Download\\%s",m_szCurrentDir,pRequestContext->szFileName);
							 InsertDownLoadDataToList(pRequestContext );
						}
						 break; 
					case StockMinute:               //���ɷ�ʱ
						if(!CheckList(filename,nKindType))
						{
							char stockId[7];
							char *FileA,*FileB;
							FileA=filename;
							FileB=strchr(FileA,'.');
							strncpy(stockId,filename,FileB-FileA);
							stockId[FileB-FileA]='\0';
							 pRequestContext=new REQUEST_CONTEXT;
							 switch(nKind)
							 {
							 case SHZS:
								 sprintf(pRequestContext->szURLFilePath,"%sdownload/MIN/��ָ/%s",temp1,filename);
								 break;
							 case SHAG:
								 sprintf(pRequestContext->szURLFilePath,"%sdownload/MIN/��A/%s",temp1,filename);
								 break;
							 case SHBG:
								 sprintf(pRequestContext->szURLFilePath,"%sdownload/MIN/��B/%s",temp1,filename);
								 break;
							 case SHZQ:
								 sprintf(pRequestContext->szURLFilePath,"%sdownload/MIN/��ծȯ/%s",temp1,filename);
								 break;
							 case SHJJ:
								 sprintf(pRequestContext->szURLFilePath,"%sdownload/MIN/������/%s",temp1,filename);
								 break;
							 case SHZZ:
								 sprintf(pRequestContext->szURLFilePath,"%sdownload/MIN/��תծ/%s",temp1,filename);
								 break;
							 case SHHG:
								 sprintf(pRequestContext->szURLFilePath,"%sdownload/MIN/���ع�/%s",temp1,filename);
								 break;
							 case SHETF:
								 sprintf(pRequestContext->szURLFilePath,"%sdownload/MIN/��ETF/%s",temp1,filename);
								 break;
							 case SHKF:
								 sprintf(pRequestContext->szURLFilePath,"%sdownload/MIN/�����Ż���/%s",temp1,filename);
								 break;
							 case SHQZ:
								 sprintf(pRequestContext->szURLFilePath,"%sdownload/MIN/��Ȩ֤/%s",temp1,filename);
								 break;
							 case SHOT:
								 sprintf(pRequestContext->szURLFilePath,"%sdownload/MIN/������/%s",temp1,filename);
								 break;
							 case SZZS:
								 sprintf(pRequestContext->szURLFilePath,"%sdownload/MIN/��ָ/%s",temp1,filename);
								 break;
							 case SZAG:
								 sprintf(pRequestContext->szURLFilePath,"%sdownload/MIN/��A/%s",temp1,filename);
								 break;
							 case SZBG:
								 sprintf(pRequestContext->szURLFilePath,"%sdownload/MIN/��B/%s",temp1,filename);
								 break;
							 case SZJJ:
								 sprintf(pRequestContext->szURLFilePath,"%sdownload/MIN/�����/%s",temp1,filename);
								 break;
							 case SZZQ:
								 sprintf(pRequestContext->szURLFilePath,"%sdownload/MIN/��ծȯ/%s",temp1,filename);
								 break;
							 case SZZZ:
								 sprintf(pRequestContext->szURLFilePath,"%sdownload/MIN/��תծ/%s",temp1,filename);
								 break;
							 case SZHG:
								 sprintf(pRequestContext->szURLFilePath,"%sdownload/MIN/��ع�/%s",temp1,filename);
								 break;
							 case SZETF:
								 sprintf(pRequestContext->szURLFilePath,"%sdownload/MIN/��ETF/%s",temp1,filename);
								 break;
							 case SZKF:
								 sprintf(pRequestContext->szURLFilePath,"%sdownload/MIN/��Ż���/%s",temp1,filename);
								 break;
							 case SZQZ:
								 sprintf(pRequestContext->szURLFilePath,"%sdownload/MIN/��Ȩ֤/%s",temp1,filename);
								 break;
							 case SZSB:
								 sprintf(pRequestContext->szURLFilePath,"%sdownload/MIN/������/%s",temp1,filename);
								 break;
							 case SZOT:
								 sprintf(pRequestContext->szURLFilePath,"%sdownload/MIN/������/%s",temp1,filename);
								 break;
							 case SZZXQY:
								 sprintf(pRequestContext->szURLFilePath,"%sdownload/MIN/����С/%s",temp1,filename);
								 break;
							 case SZCYB:
								 sprintf(pRequestContext->szURLFilePath,"%sdownload/MIN/��ҵ��/%s",temp1,filename);
								 break;
							 }
							 pRequestContext->DownloadDataType=StockMinute;
							 strcpy(pRequestContext->szFileName ,filename);
							 pRequestContext->DownloadDataCount=0;
							 pRequestContext->kind=nKindType;
							 pRequestContext->DownloadFileLength=0; 
							 pRequestContext->nDownLoadState=READY;
			                 sprintf(pRequestContext->szSaveFilePath,"%s\\Extract\\Download\\%s",m_szCurrentDir,pRequestContext->szFileName);
							 InsertDownLoadDataToList(pRequestContext );
						}
						 break; 
					case ShZsMinute:                //��ָ֤��
						if(!CheckList("��ָ.zip",SHZS))
						{
							 pRequestContext=new REQUEST_CONTEXT;
							 pRequestContext->DownloadDataType=ShZsMinute;
							 strcpy(pRequestContext->szFileName ,"��ָ.zip");
							 pRequestContext->DownloadDataCount=0;
                             pRequestContext->kind=SHZS;
							 pRequestContext->DownloadFileLength=0; 
							 pRequestContext->nDownLoadState=READY;
							 sprintf(pRequestContext->szURLFilePath,"%sdownload/min/%s",temp1,pRequestContext->szFileName);
							 sprintf(pRequestContext->szSaveFilePath,"%s\\Extract\\Download\\%s",m_szCurrentDir,pRequestContext->szFileName);
							 InsertDownLoadDataToList(pRequestContext );
						}
						 break; 
					case ShAgMinute:                //��֤����

						if(!CheckList("��A.zip",SHAG))
						{
							 pRequestContext=new REQUEST_CONTEXT;
							 pRequestContext->DownloadDataType=ShAgMinute;
							 strcpy(pRequestContext->szFileName ,"��A.zip");
							 pRequestContext->DownloadDataCount=0;
                             pRequestContext->kind=SHAG;
							 pRequestContext->DownloadFileLength=0; 
							 pRequestContext->nDownLoadState=READY;
							 sprintf(pRequestContext->szURLFilePath,"%sdownload/min/%s",temp1,pRequestContext->szFileName);
							 sprintf(pRequestContext->szSaveFilePath,"%s\\Extract\\Download\\%s",m_szCurrentDir,pRequestContext->szFileName);
							 InsertDownLoadDataToList(pRequestContext );
						}
						 break; 
					case ShBgMinute:                //��֤�¹�
						if(!CheckList("��B.zip",SHBG))
						{
							 pRequestContext=new REQUEST_CONTEXT;
							 pRequestContext->DownloadDataType=ShBgMinute;
							 strcpy(pRequestContext->szFileName ,"��B.zip");
							 pRequestContext->DownloadDataCount=0;
                             pRequestContext->kind=SHBG;
							 pRequestContext->DownloadFileLength=0; 
							 pRequestContext->nDownLoadState=READY;
							 sprintf(pRequestContext->szURLFilePath,"%sdownload/min/%s",temp1,pRequestContext->szFileName);
							 sprintf(pRequestContext->szSaveFilePath,"%s\\Extract\\Download\\%s",m_szCurrentDir,pRequestContext->szFileName);
							 InsertDownLoadDataToList(pRequestContext );
						}
						 break; 
					case ShJjMinute:                //��֤����
						if(!CheckList("������.zip",SHJJ))
						{
							pRequestContext=new REQUEST_CONTEXT;
							pRequestContext->DownloadDataType=ShJjMinute;
							strcpy(pRequestContext->szFileName ,"������.zip");
							pRequestContext->kind=SHJJ;
							pRequestContext->DownloadDataCount=0;
							pRequestContext->DownloadFileLength=0; 
							pRequestContext->nDownLoadState=READY;
							sprintf(pRequestContext->szURLFilePath,"%sdownload/min/%s",temp1,pRequestContext->szFileName);
							sprintf(pRequestContext->szSaveFilePath,"%s\\Extract\\Download\\%s",m_szCurrentDir,pRequestContext->szFileName);
							InsertDownLoadDataToList(pRequestContext );
						}
						break; 
					case ShZqMinute:                //��֤ծȯ
						if(!CheckList("��ծȯ.zip",SHZQ))
						{
							pRequestContext=new REQUEST_CONTEXT;
							pRequestContext->DownloadDataType=ShZqMinute;
							strcpy(pRequestContext->szFileName ,"��ծȯ.zip");
							pRequestContext->kind=SHZQ;
							pRequestContext->DownloadDataCount=0;
							pRequestContext->DownloadFileLength=0; 
							pRequestContext->nDownLoadState=READY;
							sprintf(pRequestContext->szURLFilePath,"%sdownload/min/%s",temp1,pRequestContext->szFileName);
							sprintf(pRequestContext->szSaveFilePath,"%s\\Extract\\Download\\%s",m_szCurrentDir,pRequestContext->szFileName);
							InsertDownLoadDataToList(pRequestContext );
						}
						break; 
					case ShZzMinute:                //��֤תծ
						if(!CheckList("��תծ.zip",SHZZ))
						{
							pRequestContext=new REQUEST_CONTEXT;
							pRequestContext->DownloadDataType=ShZzMinute;
							strcpy(pRequestContext->szFileName ,"��תծ.zip");
							pRequestContext->kind=SHZZ;
							pRequestContext->DownloadDataCount=0;
							pRequestContext->DownloadFileLength=0; 
							pRequestContext->nDownLoadState=READY;
							sprintf(pRequestContext->szURLFilePath,"%sdownload/min/%s",temp1,pRequestContext->szFileName);
							sprintf(pRequestContext->szSaveFilePath,"%s\\Extract\\Download\\%s",m_szCurrentDir,pRequestContext->szFileName);
							InsertDownLoadDataToList(pRequestContext );
						}
						break; 
					case ShHgMinute:                //��֤�ع�
						if(!CheckList("���ع�.zip",SHHG))
						{
							pRequestContext=new REQUEST_CONTEXT;
							pRequestContext->DownloadDataType=ShHgMinute;
							strcpy(pRequestContext->szFileName ,"���ع�.zip");
							pRequestContext->kind=SHHG;
							pRequestContext->DownloadDataCount=0;
							pRequestContext->DownloadFileLength=0; 
							pRequestContext->nDownLoadState=READY;
							sprintf(pRequestContext->szURLFilePath,"%sdownload/min/%s",temp1,pRequestContext->szFileName);
							sprintf(pRequestContext->szSaveFilePath,"%s\\Extract\\Download\\%s",m_szCurrentDir,pRequestContext->szFileName);
							InsertDownLoadDataToList(pRequestContext );
						}
						break; 
					case ShEfMinute:                //��֤ETF
						if(!CheckList("��ETF.zip",SHETF))
						{
							pRequestContext=new REQUEST_CONTEXT;
							pRequestContext->DownloadDataType=ShEfMinute;
							strcpy(pRequestContext->szFileName ,"��ETF.zip");
							pRequestContext->kind=SHETF;
							pRequestContext->DownloadDataCount=0;
							pRequestContext->DownloadFileLength=0; 
							pRequestContext->nDownLoadState=READY;
							sprintf(pRequestContext->szURLFilePath,"%sdownload/min/%s",temp1,pRequestContext->szFileName);
							sprintf(pRequestContext->szSaveFilePath,"%s\\Extract\\Download\\%s",m_szCurrentDir,pRequestContext->szFileName);
							InsertDownLoadDataToList(pRequestContext );
						}
						break; 
					case ShKjMinute:                //��֤���Ż���
						if(!CheckList("�����Ż���.zip",SHKF))
						{
							pRequestContext=new REQUEST_CONTEXT;
							pRequestContext->DownloadDataType=ShKjMinute;
							strcpy(pRequestContext->szFileName ,"�����Ż���.zip");
							pRequestContext->kind=SHKF;
							pRequestContext->DownloadDataCount=0;
							pRequestContext->DownloadFileLength=0; 
							pRequestContext->nDownLoadState=READY;
							sprintf(pRequestContext->szURLFilePath,"%sdownload/min/%s",temp1,pRequestContext->szFileName);
							sprintf(pRequestContext->szSaveFilePath,"%s\\Extract\\Download\\%s",m_szCurrentDir,pRequestContext->szFileName);
							InsertDownLoadDataToList(pRequestContext );
						}
						break; 
					case ShQzMinute:                //��֤Ȩ֤
						if(!CheckList("��Ȩ֤.zip",SHQZ))
						{
							pRequestContext=new REQUEST_CONTEXT;
							pRequestContext->DownloadDataType=ShQzMinute;
							strcpy(pRequestContext->szFileName ,"��Ȩ֤.zip");
							pRequestContext->kind=SHQZ;
							pRequestContext->DownloadDataCount=0;
							pRequestContext->DownloadFileLength=0; 
							pRequestContext->nDownLoadState=READY;
							sprintf(pRequestContext->szURLFilePath,"%sdownload/min/%s",temp1,pRequestContext->szFileName);
							sprintf(pRequestContext->szSaveFilePath,"%s\\Extract\\Download\\%s",m_szCurrentDir,pRequestContext->szFileName);
							InsertDownLoadDataToList(pRequestContext );
						}
						break; 
					case ShOtMinute:                //��֤����
						if(!CheckList("������.zip",SHOT))
						{
							pRequestContext=new REQUEST_CONTEXT;
							pRequestContext->DownloadDataType=ShOtMinute;
							strcpy(pRequestContext->szFileName ,"������.zip");
							pRequestContext->kind=SHOT;
							pRequestContext->DownloadDataCount=0;
							pRequestContext->DownloadFileLength=0; 
							pRequestContext->nDownLoadState=READY;
							sprintf(pRequestContext->szURLFilePath,"%sdownload/min/%s",temp1,pRequestContext->szFileName);
							sprintf(pRequestContext->szSaveFilePath,"%s\\Extract\\Download\\%s",m_szCurrentDir,pRequestContext->szFileName);
							InsertDownLoadDataToList(pRequestContext );
						}
						break; 

					case SzZsMinute:                //��ָ֤��
						if(!CheckList("��ָ.zip",SZZS))
						{
							pRequestContext=new REQUEST_CONTEXT;
							pRequestContext->DownloadDataType=SzZsMinute;
							strcpy(pRequestContext->szFileName ,"��ָ.zip");
							pRequestContext->kind=SZZS;
							pRequestContext->DownloadDataCount=0;
							pRequestContext->DownloadFileLength=0; 
							pRequestContext->nDownLoadState=READY;
							sprintf(pRequestContext->szURLFilePath,"%sdownload/min/%s",temp1,pRequestContext->szFileName);
							sprintf(pRequestContext->szSaveFilePath,"%s\\Extract\\Download\\%s",m_szCurrentDir,pRequestContext->szFileName);
							InsertDownLoadDataToList(pRequestContext );
						}
						break; 
					case SzAgMinute:                //��֤����
						if(!CheckList("��A.zip",SZAG))
						{
							pRequestContext=new REQUEST_CONTEXT;
							pRequestContext->DownloadDataType=SzAgMinute;
							strcpy(pRequestContext->szFileName ,"��A.zip");
							pRequestContext->kind=SZAG;
							pRequestContext->DownloadDataCount=0;
							pRequestContext->DownloadFileLength=0; 
							pRequestContext->nDownLoadState=READY;
							sprintf(pRequestContext->szURLFilePath,"%sdownload/min/%s",temp1,pRequestContext->szFileName);
							sprintf(pRequestContext->szSaveFilePath,"%s\\Extract\\Download\\%s",m_szCurrentDir,pRequestContext->szFileName);
							InsertDownLoadDataToList(pRequestContext );
						}
						break; 
					case SzBgMinute:                //��֤�¹�
						if(!CheckList("��B.zip",SZBG))
						{
							pRequestContext=new REQUEST_CONTEXT;
							pRequestContext->DownloadDataType=SzBgMinute;
							strcpy(pRequestContext->szFileName ,"��B.zip");
							pRequestContext->DownloadDataCount=0;
							pRequestContext->kind=SZBG;
							pRequestContext->DownloadFileLength=0; 
							pRequestContext->nDownLoadState=READY;
							sprintf(pRequestContext->szURLFilePath,"%sdownload/min/%s",temp1,pRequestContext->szFileName);
							sprintf(pRequestContext->szSaveFilePath,"%s\\Extract\\Download\\%s",m_szCurrentDir,pRequestContext->szFileName);
							InsertDownLoadDataToList(pRequestContext );
						}
						break; 
					case SzJjMinute:                //��֤����
						if(!CheckList("�����.zip",SZJJ))
						{
							pRequestContext=new REQUEST_CONTEXT;
							pRequestContext->DownloadDataType=SzJjMinute;
							strcpy(pRequestContext->szFileName ,"�����.zip");
							pRequestContext->kind=SZJJ;
							pRequestContext->DownloadDataCount=0;
							pRequestContext->DownloadFileLength=0; 
							pRequestContext->nDownLoadState=READY;
							sprintf(pRequestContext->szURLFilePath,"%sdownload/min/%s",temp1,pRequestContext->szFileName);
							sprintf(pRequestContext->szSaveFilePath,"%s\\Extract\\Download\\%s",m_szCurrentDir,pRequestContext->szFileName);
							InsertDownLoadDataToList(pRequestContext );
						}
						break; 

					case SzZqMinute:                //��֤ծȯ
						if(!CheckList("��ծȯ.zip",SZZQ))
						{
							pRequestContext=new REQUEST_CONTEXT;
							pRequestContext->DownloadDataType=SzZqMinute;
							strcpy(pRequestContext->szFileName ,"��ծȯ.zip");
							pRequestContext->kind=SZZQ;
							pRequestContext->DownloadDataCount=0;
							pRequestContext->DownloadFileLength=0; 
							pRequestContext->nDownLoadState=READY;
							sprintf(pRequestContext->szURLFilePath,"%sdownload/min/%s",temp1,pRequestContext->szFileName);
							sprintf(pRequestContext->szSaveFilePath,"%s\\Extract\\Download\\%s",m_szCurrentDir,pRequestContext->szFileName);
							InsertDownLoadDataToList(pRequestContext );
						}
						break; 
					case SzZzMinute:                //��֤תծ
						if(!CheckList("��תծ.zip",SZZZ))
						{
							pRequestContext=new REQUEST_CONTEXT;
							pRequestContext->DownloadDataType=SzZzMinute;
							strcpy(pRequestContext->szFileName ,"��תծ.zip");
							pRequestContext->kind=SZZZ;
							pRequestContext->DownloadDataCount=0;
							pRequestContext->DownloadFileLength=0; 
							pRequestContext->nDownLoadState=READY;
							sprintf(pRequestContext->szURLFilePath,"%sdownload/min/%s",temp1,pRequestContext->szFileName);
							sprintf(pRequestContext->szSaveFilePath,"%s\\Extract\\Download\\%s",m_szCurrentDir,pRequestContext->szFileName);
							InsertDownLoadDataToList(pRequestContext );
						}
						break; 
					case SzHgMinute:                //��֤�ع�
						if(!CheckList("��ع�.zip",SZHG))
						{
							pRequestContext=new REQUEST_CONTEXT;
							pRequestContext->DownloadDataType=SzHgMinute;
							strcpy(pRequestContext->szFileName ,"��ع�.zip");
							pRequestContext->kind=SZHG;
							pRequestContext->DownloadDataCount=0;
							pRequestContext->DownloadFileLength=0; 
							pRequestContext->nDownLoadState=READY;
							sprintf(pRequestContext->szURLFilePath,"%sdownload/min/%s",temp1,pRequestContext->szFileName);
							sprintf(pRequestContext->szSaveFilePath,"%s\\Extract\\Download\\%s",m_szCurrentDir,pRequestContext->szFileName);
							InsertDownLoadDataToList(pRequestContext );
						}
						break; 
					case SzEfMinute:                //��֤ETF
						if(!CheckList("��ETF.zip",SZETF))
						{
							pRequestContext=new REQUEST_CONTEXT;
							pRequestContext->DownloadDataType=SzEfMinute;
							strcpy(pRequestContext->szFileName ,"��ETF.zip");
							pRequestContext->kind=SZETF;
							pRequestContext->DownloadDataCount=0;
							pRequestContext->DownloadFileLength=0; 
							pRequestContext->nDownLoadState=READY;
							sprintf(pRequestContext->szURLFilePath,"%sdownload/min/%s",temp1,pRequestContext->szFileName);
							sprintf(pRequestContext->szSaveFilePath,"%s\\Extract\\Download\\%s",m_szCurrentDir,pRequestContext->szFileName);
							InsertDownLoadDataToList(pRequestContext );
						}
						break; 
					case SzKjMinute:                //��֤���Ż���
						if(!CheckList("��Ż���.zip",SZKF))
						{
							pRequestContext=new REQUEST_CONTEXT;
							pRequestContext->DownloadDataType=SzKjMinute;
							strcpy(pRequestContext->szFileName ,"��Ż���.zip");
							pRequestContext->kind=SZKF;
							pRequestContext->DownloadDataCount=0;
							pRequestContext->DownloadFileLength=0; 
							pRequestContext->nDownLoadState=READY;
							sprintf(pRequestContext->szURLFilePath,"%sdownload/min/%s",temp1,pRequestContext->szFileName);
							sprintf(pRequestContext->szSaveFilePath,"%s\\Extract\\Download\\%s",m_szCurrentDir,pRequestContext->szFileName);
							InsertDownLoadDataToList(pRequestContext );
						}
						break; 
					case SzQzMinute:                //��֤Ȩ֤
						if(!CheckList("��Ȩ֤.zip",SZQZ))
						{
							pRequestContext=new REQUEST_CONTEXT;
							pRequestContext->DownloadDataType=SzQzMinute;
							strcpy(pRequestContext->szFileName ,"��Ȩ֤.zip");
							pRequestContext->kind=SZQZ;
							pRequestContext->DownloadDataCount=0;
							pRequestContext->DownloadFileLength=0; 
							pRequestContext->nDownLoadState=READY;
							sprintf(pRequestContext->szURLFilePath,"%sdownload/min/%s",temp1,pRequestContext->szFileName);
							sprintf(pRequestContext->szSaveFilePath,"%s\\Extract\\Download\\%s",m_szCurrentDir,pRequestContext->szFileName);
							InsertDownLoadDataToList(pRequestContext );
						}
						break; 
					case SzSbMinute:                //��֤����
						if(!CheckList("������.zip",SZSB))
						{
							pRequestContext=new REQUEST_CONTEXT;
							pRequestContext->DownloadDataType=SzSbMinute;
							strcpy(pRequestContext->szFileName ,"������.zip");
							pRequestContext->kind=SZSB;
							pRequestContext->DownloadDataCount=0;
							pRequestContext->DownloadFileLength=0; 
							pRequestContext->nDownLoadState=READY;
							sprintf(pRequestContext->szURLFilePath,"%sdownload/min/%s",temp1,pRequestContext->szFileName);
							sprintf(pRequestContext->szSaveFilePath,"%s\\Extract\\Download\\%s",m_szCurrentDir,pRequestContext->szFileName);
							InsertDownLoadDataToList(pRequestContext );
						}
						break; 
					case SzOtMinute:                //��֤����
						if(!CheckList("������.zip",SZOT))
						{
							pRequestContext=new REQUEST_CONTEXT;
							pRequestContext->DownloadDataType=SzOtMinute;
							strcpy(pRequestContext->szFileName ,"������.zip");
							pRequestContext->kind=SZOT;
							pRequestContext->DownloadDataCount=0;
							pRequestContext->DownloadFileLength=0; 
							pRequestContext->nDownLoadState=READY;
							sprintf(pRequestContext->szURLFilePath,"%sdownload/min/%s",temp1,pRequestContext->szFileName);
							sprintf(pRequestContext->szSaveFilePath,"%s\\Extract\\Download\\%s",m_szCurrentDir,pRequestContext->szFileName);
							InsertDownLoadDataToList(pRequestContext );
						}
						break; 
					case SzZxMinute:                //��֤����
						if(!CheckList("����С.zip",SZZXQY))
						{
							pRequestContext=new REQUEST_CONTEXT;
							pRequestContext->DownloadDataType=SzZxMinute;
							strcpy(pRequestContext->szFileName ,"����С.zip");
							pRequestContext->kind=SZZXQY;
							pRequestContext->DownloadDataCount=0;
							pRequestContext->DownloadFileLength=0; 
							pRequestContext->nDownLoadState=READY;
							sprintf(pRequestContext->szURLFilePath,"%sdownload/min/%s",temp1,pRequestContext->szFileName);
							sprintf(pRequestContext->szSaveFilePath,"%s\\Extract\\Download\\%s",m_szCurrentDir,pRequestContext->szFileName);
							InsertDownLoadDataToList(pRequestContext );
						}
						break; 
					case  SzCyMinute:                //����ָ��
						if(!CheckList("��ҵ��.zip",SZCYB))
						{
							 pRequestContext=new REQUEST_CONTEXT;
							 pRequestContext->DownloadDataType=SzCyMinute;
							 strcpy(pRequestContext->szFileName ,"��ҵ��.zip");
							 pRequestContext->DownloadDataCount=0;
							 pRequestContext->DownloadFileLength=0; 
                             pRequestContext->kind=SZCYB;
							 pRequestContext->nDownLoadState=READY;
							 sprintf(pRequestContext->szURLFilePath,"%sdownload/min/%s",temp1,pRequestContext->szFileName);
							 sprintf(pRequestContext->szSaveFilePath,"%s\\Extract\\Download\\%s",m_szCurrentDir,pRequestContext->szFileName);
							 InsertDownLoadDataToList(pRequestContext );
						}
						 break; 
					case ShInfomation:              //��֤����
					  if(!CheckList("����Ϣ.dat",QBGP))
					  {
						  char szLastDate[10];
						  pRequestContext=new REQUEST_CONTEXT;
						  pRequestContext->DownloadDataType=ShInfomation;
						  strcpy(pRequestContext->szFileName ,"����Ϣ.dat");
						  pRequestContext->DownloadDataCount=0;
						  pRequestContext->DownloadFileLength=0; 
                          pRequestContext->kind=QBGP;
						  pRequestContext->nDownLoadState=READY;
						  GetLastDayInfomationDate(szLastDate);
						  sprintf(pRequestContext->szURLFilePath,"%sdownload/News/Shanghai/%s.dat",temp1,szLastDate);
						  sprintf(pRequestContext->szSaveFilePath,"%s\\News\\Shanghai\\%s.dat",m_szCurrentDir,szLastDate);
						  InsertDownLoadDataToList(pRequestContext );
					  }
						  break; 
					case SzInfomation:              //��֤����  
					  if(!CheckList("����Ϣ.dat",QBGP))
					  {
						  char szLastDate[10];
						  pRequestContext=new REQUEST_CONTEXT;
						  pRequestContext->DownloadDataType=SzInfomation;
						  strcpy(pRequestContext->szFileName ,"����Ϣ.dat");
						  pRequestContext->DownloadDataCount=0;
						  pRequestContext->DownloadFileLength=0; 
                          pRequestContext->kind=QBGP;
						  pRequestContext->nDownLoadState=READY;
						  GetLastDayInfomationDate(szLastDate);
						  sprintf(pRequestContext->szURLFilePath,"%sdownload/News/Shenzhen/%s.dat",temp1,szLastDate);
						  sprintf(pRequestContext->szSaveFilePath,"%s\\News\\shenzhen\\%s.dat",m_szCurrentDir,szLastDate);
						  InsertDownLoadDataToList(pRequestContext );
					  }
						 break; 
					case EbInfomation:              //�ƾ�����
						  if(!CheckList("�����Ϣ.dat",QBGP))
						  {
							  char szLastDate[10];
							  pRequestContext=new REQUEST_CONTEXT;
							  pRequestContext->DownloadDataType=EbInfomation;
							  strcpy(pRequestContext->szFileName ,"�����Ϣ.dat");
							  pRequestContext->DownloadDataCount=0;
							  pRequestContext->DownloadFileLength=0; 
							  pRequestContext->nDownLoadState=READY;
                              pRequestContext->kind=QBGP;
							  GetLastDayInfomationDate(szLastDate);
							  sprintf(pRequestContext->szURLFilePath,"%sdownload/News/erban/%s.dat",temp1,szLastDate);
							  sprintf(pRequestContext->szSaveFilePath,"%s\\News\\erban\\%s.dat",m_szCurrentDir,szLastDate);
							  InsertDownLoadDataToList(pRequestContext );
						  }
						  break;
					case ZjInfomation:              //�ƾ�����
					  if(!CheckList("�ƾ���Ϣ.dat",QBGP))
					  {
						  char szLastDate[10];
						  pRequestContext=new REQUEST_CONTEXT;
						  pRequestContext->DownloadDataType=ZjInfomation;
						  strcpy(pRequestContext->szFileName ,"�ƾ���Ϣ.dat");
                          pRequestContext->kind=QBGP;
						  pRequestContext->DownloadDataCount=0;
						  pRequestContext->DownloadFileLength=0; 
						  pRequestContext->nDownLoadState=READY;
						  GetLastDayInfomationDate(szLastDate);
						  sprintf(pRequestContext->szURLFilePath,"%sdownload/News/cj/%s.dat",temp1,szLastDate);
						  sprintf(pRequestContext->szSaveFilePath,"%s\\News\\cj\\%s.dat",m_szCurrentDir,szLastDate);
						  InsertDownLoadDataToList(pRequestContext );
					  }
						 break; 
//**********************************************************
				}
			}
			fclose(fp);
		}
	}
    PostMessage(WM_USER_POSTFOCUS,0,0);
}

void CDownLoadDlg::OnSaveCollecte() 
{
	// TODO: Add your control notification handler code here
	CString m_InFile;
	CString DefExt="*.ini";
	CString FileName="DownLoadSys.ini";
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
			 REQUEST_CONTEXT *pRequestContext=(REQUEST_CONTEXT *)ItemData;
			 if(pRequestContext!=NULL)
			 {
				 int cc=pRequestContext->DownloadDataType;
				 fwrite(&cc,1,4,fp);
				 fwrite(pRequestContext->szFileName,1,50,fp);
				 fwrite(&pRequestContext->kind,1,4,fp);
			 }
		 }
		fclose(fp);
	}
	PostMessage(WM_USER_POSTFOCUS,0,0);

}
BOOL CDownLoadDlg::CloseAllDownLoad()
{
	 int nCount=m_lstStockTypeName.GetItemCount();
	 for(int i=0;i<nCount;i++)
	 {
		 int ItemData=m_lstStockTypeName.GetItemData(i);
         REQUEST_CONTEXT *pRequestContext=(REQUEST_CONTEXT *)ItemData;
		 if(pRequestContext->hResource!=NULL)
		 {
		    InternetCloseHandle(pRequestContext->hResource);
			pRequestContext->hResource=NULL;
		 }
	 }
     return TRUE;
}
void CDownLoadDlg::AsyncDownLoadDirect (REQUEST_CONTEXT *prcContext)
{
	strcpy(	prcContext->szMemo,""+g_strCompanyName+"��������ϵͳ");
    prcContext->hOpen=this->hOpen;
	prcContext->hThread = CreateThread(NULL, 0, 
		(LPTHREAD_START_ROUTINE)DownLoadThreader,LPVOID(prcContext), THREAD_TERMINATE, 
		&prcContext->dwThreadID);

}
void CDownLoadDlg::GetLastDayInfomationDate(char *szLastDate)
{
	CTimeSpan timespan(1,0,0,0);
	CTime newTime=CTime::GetCurrentTime();
	char szDate[10];
	if(newTime.GetDayOfWeek()==1)
	{
        newTime =newTime- timespan -timespan;         
	}
    else if(newTime.GetDayOfWeek()==7)
	{
        newTime -= timespan;         
	}
	else if(newTime.GetHour()<15)
	{
		if(newTime.GetDayOfWeek()==2)
           newTime =newTime- timespan -timespan - timespan;         
        else
           newTime -= timespan;         
	}
	sprintf(szDate,"%d%2d%2d",newTime.GetYear(),newTime.GetMonth(),newTime.GetDay());
    for(int i=0;i<8;i++)
		if(szDate[i]==' ')
           szDate[i]='0';
   szDate[8]='\0';
   strcpy(szLastDate,szDate);
}

void CDownLoadDlg::PostNcDestroy() 
{
	// TODO: Add your specialized code here and/or call the base class
    DeleteCriticalSection( &g_csDownLoad);
	m_pDoc->m_pDownLoadDlg=NULL;
	if(m_pImageList) delete m_pImageList;
	if(m_pImageListSmall) delete m_pImageListSmall;
//	if(m_PlayGif)
//		delete m_PlayGif;

	//Added by ltr
	int nCount=m_pFileHs.GetSize();
	CTaiKlineFileHS* pFileHs;
	if (nCount>0)
	{
		for (int i=0;i<nCount;i++)
		{
			pFileHs=m_pFileHs.GetAt(i);
			pFileHs->Close();
			delete pFileHs;
		}
		m_pFileHs.RemoveAll();
	}

    delete this;
	CDialog::PostNcDestroy();
}
///install down load data
BOOL CDownLoadDlg::InstallStockName(REQUEST_CONTEXT *pRequestContext)
{
	typedef struct 
	{
		 char kind;              //      ��Ʊ����
		 char id[7];             //      ֤ȯ����
		 char name[9];           //      ֤ȯ���� 
		 char Gppyjc[5];         //      ֤ȯ����ƴ�����
		 char group;             //      ��Ʊ���
		 long sel;
	}TempCdat;

	FILE *fp;
	long index=0;
	MSG message;
	int m_totalCount=0;
    CReportData *pCdat;
	TempCdat Cdat;
	int StockCount[11];
    char SaveFileName[255];
	sprintf(SaveFileName,"%s\\Extract\\stockname.dat",m_szCurrentDir);

	if((fp=_fsopen(SaveFileName,"rb",SH_DENYNO))!=NULL)
	{
		fseek(fp,0,SEEK_SET);
		fread(&StockCount[0],4,11, fp);
        for(int j=0;j<8;j++)
			m_totalCount +=StockCount[j];
		this->m_Progress.SetRange(0,m_totalCount);
		this->m_Progress.SetPos(0);
        while(!feof(fp)&&!ferror(fp))
		{
	        CString Zqdm;
			long Serl=0;
			char group=0;
			memset(&Cdat,0,sizeof(TempCdat));
			fread(&Cdat.kind,1,1,fp);      // ��Ʊ����           1�ֽ�
			fread(Cdat.id ,1,6,fp);        // ��Ʊ����           6�ֽ�
            Cdat.id[6]='\0';
			fread(Cdat.name,1,8,fp);       // ��Ʊ����           8�ֽ� 
            Cdat.name[8]='\0';
			fread(Cdat.Gppyjc ,1,4,fp);    // ��Ʊ����ƴ�����   4�ֽ� 
            Cdat.Gppyjc[4]='\0'; 
			fread(&Cdat.sel,1,4,fp);      // ��Ʊ˳���	      4�ֽ� 

			//lmb11
			char ch123 ;
			fread(&ch123,1,1,fp);


			//lmb11
			if(Cdat.kind<0 ||Cdat.kind>=11)
				continue;
			//lmb11 end
			if (this->m_pDoc->m_sharesInformation.Lookup(Cdat.id, pCdat,Cdat.kind) != TRUE)     //���ù�Ʊ�Ƿ��Ѽ�¼���ڴ�ģ����
			{
				if(strlen(Cdat.id)==6||strlen(Cdat.id)==4)
				{
					int stocktype=Cdat.kind;
					if(stocktype>=0&&stocktype<=10)
					{
						if(!m_pDoc->m_sharesInformation.InsertItem(Cdat.id,pCdat,Cdat.kind))
							continue;
						m_pDoc->m_pReportView->LoadShowData(Cdat.kind);              //��CGRID������һ�� 
						strcpy(pCdat->name , Cdat.name);
						strcpy(pCdat->id ,Cdat.id );
                        pCdat->kind=Cdat.kind;
						strcpy(pCdat->Gppyjc ,Cdat.Gppyjc );


						m_pDoc->AddStockToKeyboard(pCdat->id,pCdat->kind,pCdat->name,pCdat->Gppyjc);
						WORD type=Cdat.kind;
						m_pDoc->m_pReportView->AddGridRow(type);              //��CGRID������һ�� 

						STOCKINFOINBLOCK *m_pStock;
						if(m_pDoc->m_ManagerBlockData.Lookup(pCdat->id,pCdat->kind,m_pStock))
						{
						   pCdat->pBlockInfo= m_pStock;
						}
						BASEINFO *m_pStockBase;
						if(m_pDoc->m_sharesInformation.LookupBase(pCdat->id,pCdat->kind,m_pStockBase))
						{
						   pCdat->pBaseInfo=m_pStockBase;
						}
					}
				}
			} 
			else
			{
				if(strcmp(pCdat->name,Cdat.name)!=0)
				{
					strcpy(pCdat->name,Cdat.name);
					strcpy(pCdat->Gppyjc,Cdat.Gppyjc);
				}
			}
//*************************************************
	        index++;
			m_Progress.SetPos(index);
			if(PeekMessage(&message,NULL,0,0,PM_REMOVE))
			{
				TranslateMessage(&message);
				DispatchMessage(&message);
			}
//*************************************************
		}
		m_pDoc->m_pReportView->AddGridRow(0);              //��CGRID������һ�� 
		fclose(fp);
        m_Progress.SetPos(0);
		return TRUE;
	}
	else 
	{
        m_Progress.SetPos(0);
        return FALSE;
	}
}
BOOL CDownLoadDlg::InstallStockDayLine(REQUEST_CONTEXT *pRequestContext)
{
    char SaveFileName[255];
	FILE *fp;
	char *FileA,*FileB;
    char FileName[255];
	MSG message;
	CTime m_NowTime = CTime::GetCurrentTime();
	CTime m_LastTime;
	CTimeSpan  lDaySpan(1, 0,0, 0 );
    time_t tTemp;
    if(m_NowTime.GetDayOfWeek()==1)
    {
       tTemp=m_NowTime.GetTime( );
	   tTemp -=lDaySpan.GetTotalSeconds();
	   tTemp -=lDaySpan.GetTotalSeconds();
       m_LastTime=tTemp;
    }else if(m_NowTime.GetDayOfWeek()==7)
    {
       tTemp=m_NowTime.GetTime( );
	   tTemp -=lDaySpan.GetTotalSeconds();
       m_LastTime=tTemp;
    }else 
	{
       m_LastTime=m_NowTime;
	}
    FileA=pRequestContext->szFileName;
	FileB=strchr(FileA,'.');
	strncpy(FileName,FileA,(FileB-FileA));
    FileName[FileB-FileA]='\0';
	sprintf(SaveFileName,"%s\\Extract\\%s.day",m_szCurrentDir,FileName);
	if((fp=_fsopen(SaveFileName,"rb",SH_DENYNO))!=NULL)
	{
		int FileID=0;  //�ļ���ʶ
		int NumStock=0;
		fread(&FileID,4,1,fp);
		if( FileID!=FILEID22)
		{
			AfxMessageBox("��"+g_strCompanyName+"���ݸ�ʽ��",MB_ICONSTOP);
			return FALSE;
		}
		fread(&NumStock,sizeof(int),1,fp);
		fseek(fp,16,SEEK_SET);
		m_Progress.SetRange32(0,NumStock);
	    m_Progress.SetPos(0);
		for( int index=0; index<NumStock; index++)
		{
            DWORD NumKline=0;
			char StockName[8];
			int nKind=0;
			int nMarketType=0;
			//StockName[0]=0;
			CString strStockName;
			HGLOBAL	hMem;
			LPVOID hp;
			Kline *pKline;
		    fread(StockName,8,1,fp);// modified by cuiwei. 2004-04-18
			fread(&nMarketType,4,1,fp);
		    fread(&NumKline,4,1,fp);
			strStockName=StockName;
			nKind=m_pDoc->m_sharesInformation.GetStockKind(nMarketType,StockName);
			CTaiKlineFileKLine* pFile = CTaiKlineFileKLine::GetFilePointer (strStockName,nKind,true);
            hMem = GlobalAlloc( GPTR, (NumKline)* sizeof( Kline) ) ;
	        hp=GlobalLock(hMem);
			if( hp )
				pKline= (Kline *)hp;
			else
			{ 
				CString ss;
				ss.Format("can't alloc Buffers. stockcode is %s Kline is %d",StockName,NumKline);
				AfxMessageBox(ss,MB_ICONSTOP) ;           
				return FALSE;
			}
            fread(pKline, sizeof( Kline),NumKline,fp);
			if(!strstr(StockName,"8K")&&!strstr(StockName,"8k")&&!strstr(StockName,"IA"))
			{
				time_t lasttime=pKline[NumKline-1].day;
				if(m_pDoc->GetStockDay(lasttime)==m_pDoc->GetStockDay(m_LastTime.GetTime()))
				{
					 CReportData *pCdat; 
					 if(m_pDoc->m_sharesInformation.Lookup(StockName,pCdat,nKind))
					 {
						 CString ss = StockName;
						 Kline * pKline2 = 0;
						 int nn = CTaiKlineFileKLine::ReadKLineAny(ss,nKind, pKline2, 2, 5, NULL, 48,false);
						 if(nn>0)
						 {
							 if(nn>1)
							 {
								 if(m_pDoc->GetStockDay(m_LastTime.GetTime())!=m_pDoc->GetStockDay(pKline2[1].day))
									pCdat->ystc=pKline2[1].close;
								 else if(m_pDoc->GetStockDay(m_LastTime.GetTime())!=m_pDoc->GetStockDay(pKline2[0].day))
									pCdat->ystc=pKline2[0].close;
							 }
							 else
							 {
								 if(m_pDoc->GetStockDay(m_LastTime.GetTime())!=m_pDoc->GetStockDay(pKline2[0].day))
									pCdat->ystc=pKline2[0].close;
							 }
						 }
						 if(pKline2) delete [] pKline2;

						 pCdat->opnp=pKline[NumKline-1].open;
						 pCdat->lowp=pKline[NumKline-1].low;
						 pCdat->higp=pKline[NumKline-1].high;
						 pCdat->nowp=pKline[NumKline-1].close;
						 pCdat->totp=pKline[NumKline-1].amount;
						 pCdat->totv=pKline[NumKline-1].vol;
					 }
				}
			    pFile ->WriteKLine(StockName,pKline,NumKline,CTaiKlineFileKLine::overWriteAll);
			}
			GlobalUnlock((HGLOBAL)pKline);
			GlobalFree((HGLOBAL)pKline);
//*************************************************
			m_Progress.SetPos(index);
			if(PeekMessage(&message,NULL,0,0,PM_REMOVE))
			{
				TranslateMessage(&message);
				DispatchMessage(&message);
			}
//*************************************************
		}
		fclose(fp);
        m_Progress.SetPos(0);
		CTaiShanKlineShowView::OnDataChangAll(200);
		return TRUE;
	}
	else
	{
        m_Progress.SetPos(0);
		return FALSE;
	}
}
BOOL CDownLoadDlg::InstallStockChuQuan(REQUEST_CONTEXT *pRequestContext)
{
    char SaveFileName[255];
	sprintf(SaveFileName,"%s\\Extract\\dest.pwr",m_szCurrentDir);
	return CTaiChuQuanInDlg::InstallPowerData(SaveFileName);
}
void CDownLoadDlg::InstallStockMenute(char *pFilePath) 
{
	CImportData::InstallOneMinStock(pFilePath);
}
void CDownLoadDlg::InstallStockMunuteForType(REQUEST_CONTEXT *pRequestContext)
{
	int StockType;

	switch(pRequestContext->DownloadDataType)
	{
		case ShZsMinute:                //��ָ֤��
             StockType=0; 
			 break; 
		case ShAgMinute:                //��֤����
             StockType=1; 
			 break; 
		case ShBgMinute:                //��֤�¹�
             StockType=2; 
			 break; 
		case ShJjMinute:                //��֤ծȯ
             StockType=3; 
			 break; 
		case ShZqMinute:                //��ָ֤��
             StockType=4; 
			 break; 
		case ShZzMinute:                //��֤����
             StockType=5; 
			 break; 
		case ShHgMinute:                //��֤�¹�
             StockType=6; 
			 break; 
		case ShEfMinute:                //��֤ծȯ
             StockType=7; 
			 break; 
		case ShKjMinute:                //��ָ֤��
             StockType=8; 
			 break; 
		case ShQzMinute:                //��֤����
             StockType=9; 
			 break; 
		case ShOtMinute:                //��֤�¹�
             StockType=10; 
			 break; 
		case SzZsMinute:                //��ָ֤��
             StockType=11; 
			 break; 
		case SzAgMinute:                //��֤����
             StockType=12; 
			 break; 
		case SzBgMinute:                //��֤�¹�
             StockType=13; 
			 break; 
		case SzJjMinute:                //��֤ծȯ
             StockType=14; 
			 break; 
		case SzZqMinute:                //��ָ֤��
             StockType=15; 
			 break; 
		case SzZzMinute:                //��֤����
             StockType=16; 
			 break; 
		case SzHgMinute:                //��֤�¹�
             StockType=17; 
			 break; 
		case SzEfMinute:                //��֤ծȯ
             StockType=18; 
			 break; 
		case SzKjMinute:                //��ָ֤��
             StockType=19; 
			 break; 
		case SzQzMinute:                //��֤����
             StockType=20; 
			 break; 
		case SzSbMinute:                //��֤�¹�
             StockType=21; 
			 break; 
		case SzOtMinute:                //��֤ծȯ
             StockType=22; 
			 break; 
		case SzZxMinute:                //��֤ծȯ
             StockType=23; 
			 break; 
         case SzCyMinute:                //����ָ��
             StockType=24; 
			 break; 
	}   
   int temp=m_pDoc->m_sharesInformation.GetStockTypeCount(StockType);
   this->m_Progress.SetRange(0,temp);
   m_Progress.SetPos(0);
   MSG message;
   for(int j=0;j<temp;j++)
   {
		CReportData *Cdat;
		char filepath[255];
		m_pDoc->m_sharesInformation.GetStockItem(StockType,j,Cdat);
		if(Cdat==NULL)
		   continue;
		if(StockType==0||StockType==1||StockType==2||StockType==6)
		   sprintf(filepath,"%s\\Extract\\sh\\%s.1mn",m_szCurrentDir,Cdat->id);
		else
		   sprintf(filepath,"%s\\Extract\\sz\\%s.1mn",m_szCurrentDir,Cdat->id);
        InstallStockMenute(filepath) ;
//*************************************************
		m_Progress.SetPos(j);
		if(PeekMessage(&message,NULL,0,0,PM_REMOVE))
		{
			TranslateMessage(&message);
			DispatchMessage(&message);
		}
   }
   m_Progress.SetPos(0);

   ;CMainFrame::m_pDoc ->m_bCloseWorkDone = TRUE;
}

BOOL CDownLoadDlg::InstallStockHsDetail(REQUEST_CONTEXT *pRequestContext)
{
//   char        szFileName[MAX_PATH] ;     
//	char        szSaveFilePath[MAX_PATH];
	char szFileName[MAX_PATH];
	char szDestFileName[MAX_PATH];
	CString strSourceFileName;
	CString strDestFileName;
	CString strFileName;
	CTime tm;
	char szCity[3];
	char szDate[9];
	CString strToday;
//	bool bIsSh;
	int nMarket = SH_MARKET_EX;
	bool bIsToday;

	CString strCity;
	CString strID;
	strSourceFileName=pRequestContext->szSaveFilePath;
	strcpy(szFileName,pRequestContext->szFileName);
	strncpy(szCity,szFileName,2);
	szCity[2]=0;
	strCity.Format("%s",szCity);
	strCity.MakeUpper();


	strncpy(szDate,szFileName+2,8);
	szDate[8]=0;

	tm=CTime::GetCurrentTime();
	strToday=tm.Format("%Y%m%d");

	if(strCity!=_T("SH")) nMarket = SZ_MARKET_EX;
	bIsToday=(strToday==CString(szDate)) ? true : false;

	sprintf(szDestFileName,"%s\\Data\\history%s\\%s.hst",m_szCurrentDir,szCity,szDate);
	strDestFileName=szDestFileName;
	strFileName=szFileName;
	strID=strFileName.Mid(10,strFileName.Find(_T('.'))-10);
	CTaiKlineFileHS* pFileHs=NULL;
	CBuySellList buySellList;
	int i;
	int nCount=m_pFileHs.GetSize();
	BOOL IsNew=TRUE;
	m_Progress.SetRange(0,10);
	m_Progress.SetPos(0);

	//��Ŀ���ļ�
	if (nCount>0)
	{
		for (i=0;i<nCount;i++)
		{
			pFileHs=m_pFileHs.GetAt(i);
			if (pFileHs->GetFileName()==strDestFileName)
			{
				IsNew=FALSE;
				break;
			}
		}
	}

	if (IsNew)
	{
		bIsToday=true;
		pFileHs=new CTaiKlineFileHS(nMarket,bIsToday);
		if (!pFileHs->Open(strDestFileName,0))
			goto FAILDOOR;
	}

	if (!ReadHS(strSourceFileName,buySellList))
		goto FAILDOOR;

	m_Progress.SetPos(4);
	pFileHs->WriteHS2(strID,buySellList);
	pFileHs->Close();
	delete pFileHs;
	m_Progress.SetPos(10);
	m_Progress.SetPos(0);
	return TRUE;

FAILDOOR:
	if (IsNew && pFileHs!=NULL)
		delete pFileHs;
	m_Progress.SetPos(0);
	return FALSE;
}

///////**************************************************************
void CDownLoadDlg::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	if(m_InfomationContext.nDownLoadState!=DOWNLOADCOMPLETE&&m_InfomationContext.hThread==NULL&&m_InfomationContext.nDownLoadState!=NOTFINDFILE)
	{
        AsyncDownLoadDirect(&m_InfomationContext);
	}
	int nCount=m_lstStockTypeName.GetItemCount();
	int DownloadComplete=0;
	int nDownLoadCount=0;
	if(m_InfomationContext.hThread!=NULL)
         nDownLoadCount++;
	for(int i=0;i<nCount;i++)
	{
	   int ItemData=m_lstStockTypeName.GetItemData(i);
       REQUEST_CONTEXT *pRequestContext=(REQUEST_CONTEXT *)ItemData;
	   if(pRequestContext->nDownLoadState==INSTALLCOMPLETE||pRequestContext->nDownLoadState==DOWNLOADCOMPLETE
		   ||pRequestContext->nDownLoadState==NOTFINDFILE||pRequestContext->nDownLoadState==STOPDOWNLOAD||
		   pRequestContext->nDownLoadState==NOTINSTALLCOMPLETE)
	   {
		   DownloadComplete++;
		   continue;
	   }
       if(pRequestContext->hThread!=NULL)   //������
          nDownLoadCount++;
	}
	if(DownloadComplete==nCount&&m_InfomationContext.nDownLoadState==DOWNLOADCOMPLETE)
	{
       KillTimer(ID_DOWNLOAD_STATE);
 	   CDialog::OnTimer(nIDEvent);
       return ;
	}
    nCount=m_lstStockTypeName.GetItemCount();
	int i;
	if(nDownLoadCount<2)
	{
		for(i=0;i<nCount;i++)
		{
		   if(nDownLoadCount>=2)
			  break;
		   int ItemData=m_lstStockTypeName.GetItemData(i);
		   REQUEST_CONTEXT *pRequestContext=(REQUEST_CONTEXT *)ItemData;
		   if(pRequestContext->hThread!=NULL)
			   continue;
		   if(pRequestContext->nDownLoadState==NOTFINDFILE||pRequestContext->bIsStopDownload)
			   continue;
		   if(pRequestContext->nDownLoadState==DOWNLOADCOMPLETE||pRequestContext->nDownLoadState==INSTALLCOMPLETE)
			   continue;
		   if(pRequestContext->nDownLoadState==NOTINSTALLCOMPLETE)
			   continue;
		   pRequestContext->nDownLoadState=READY;
		   pRequestContext->hResource=NULL;
		   strcpy(pRequestContext->szMemo,""+g_strCompanyName+"��������ϵͳ");
		   pRequestContext->bResourceOpen=FALSE;
		   pRequestContext->bIsStopDownload=FALSE;
		   pRequestContext->bIsContinueDown=FALSE;
		   pRequestContext->downloaderrors=0;
           pRequestContext->LastDownloadCount=0;
           
	       pRequestContext->DownLoadNotChangeCount=0;

		   pRequestContext->dwThreadID=-1;
		   pRequestContext->hThread=NULL;
           AsyncDownLoadDirect (pRequestContext);
           nDownLoadCount++;
		}
	}
//************************************************************
		for( i=0;i<nCount;i++)
		{
		   int ItemData=m_lstStockTypeName.GetItemData(i);
		   REQUEST_CONTEXT *pRequestContext=(REQUEST_CONTEXT *)ItemData;
		   if(pRequestContext->nDownLoadState==NOTFINDFILE||pRequestContext->bIsStopDownload)
			   continue;
		   if(pRequestContext->nDownLoadState==DOWNLOADCOMPLETE||pRequestContext->nDownLoadState==INSTALLCOMPLETE)
			   continue;
		   if(pRequestContext->nDownLoadState==NOTINSTALLCOMPLETE)
			   continue;
           if(pRequestContext->nDownLoadState==DOWNLOADING)
		   {
              if(pRequestContext->DownloadDataCount>pRequestContext->LastDownloadCount)
			  {
                   pRequestContext->LastDownloadCount= pRequestContext->DownloadDataCount; 
				   pRequestContext->DownLoadNotChangeCount=0;
			  }
			  else
			  {
                   pRequestContext->DownLoadNotChangeCount++;
				   if(pRequestContext->DownLoadNotChangeCount==4)
				   {
						if(pRequestContext->bResourceOpen||pRequestContext->hThread)
						{
							 if(pRequestContext->bResourceOpen)
							 {
								InternetCloseHandle(pRequestContext->hResource);
								pRequestContext->bResourceOpen=FALSE;
								Sleep(100);
							 }
							 if(pRequestContext->hThread)
							 {
								if(!TerminateThread( pRequestContext->hThread,pRequestContext->dwThreadID))
								{
									AfxMessageBox("kill thread error");
								}
								CloseHandle(pRequestContext->hThread);
								pRequestContext->hThread=NULL;
								pRequestContext->dwThreadID=-1;
							 }
							 pRequestContext->nDownLoadState=STOPDOWNLOAD;
							 DispalyDownLoadInfomation(pRequestContext);
						}
	                   AsyncDownLoadDirect (pRequestContext);
                       pRequestContext->DownLoadNotChangeCount=0;
				   }
			  }
		   }
		}

//************************************************************
	CDialog::OnTimer(nIDEvent);
}
void CDownLoadDlg::SetGifDisplay(REQUEST_CONTEXT *pInfomationContext)
{
/*		 try
		 {
		     IZipPtr pIZip(__uuidof(Zip));
		     IUnZipPtr pIUnZip(pIZip);
		     pIUnZip->InitUnZip();

			 pIUnZip->SetExtractFile(pInfomationContext->szSaveFilePath);
			 pIUnZip->SetDestPath("EXTRACT");
			 pIUnZip->Extract();
		 }
		catch(const _com_error& error)
		{
		  CString strError;
		  strError.Format("An Error,%x\n",error.Error());
		  AfxMessageBox(strError);
		}
 */ char SaveFileName[255];
	sprintf(SaveFileName,"%s\\Extract\\Sjl.Gif",m_szCurrentDir);
//    m_PlayGif->Load(SaveFileName);
//    m_PlayGif->Play();
	this->Invalidate();
}

void CDownLoadDlg::OnDblclkSelectLoaddata(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	HTREEITEM  hItem = m_MyTree.GetSelectedItem();
	if(m_MyTree.ItemHasChildren(hItem))
	{
/*      int state=m_MyTree.GetItemState(hItem,TVIS_EXPANDED); 
		int state1=state&TVIS_EXPANDED;
		int state2=state&TVIS_EXPANDEDONCE;
		if(state1>0&&state2>0)
		{
            AddDownLoadData() ; 
			return;
		}
		else
*/			return;
       return;
	}
    AddDownLoadData() ;
	*pResult = 0;
}
BOOL CDownLoadDlg::InstallStockCaiwuData(REQUEST_CONTEXT *pRequestContext)
{
    char SaveFileName[255];
	sprintf(SaveFileName,"%s\\Extract\\finance.dat",m_szCurrentDir);
	return CCaiwuData::ImportCwData(SaveFileName);
}
void CDownLoadDlg::OnCancel()
{
	if(m_bIsCalc)
	{
		AfxMessageBox("�������ڰ�װ���ݲ��ܽ����˳�����!");
		return;
	}
	CDialog::OnCancel();
    SendMessage(WM_CLOSE,0,0); 
}

BOOL CDownLoadDlg::OnHelpInfo(HELPINFO* pHelpInfo) 
{
	// TODO: Add your message handler code here and/or call default
	::HtmlHelp(m_hWnd,"stock.chm",HH_HELP_CONTEXT,102);
    return TRUE;	
//	return CDialog::OnHelpInfo(pHelpInfo);
}

BOOL CDownLoadDlg::ReadHS(CString strFileName, CBuySellList &buySellList)
{
		
	CTime tm;
	char szCity[3];
	char szDate[9];
	CString strToday;
	bool bIsSh;
	bool bIsToday;

	CString sFileName = strFileName.Right (20);
	sFileName.MakeUpper ();
	if(sFileName.Left (2)!="SH")
		sFileName = strFileName.Right (18);
	CString strCity;
	CString strID;
	strCity=sFileName.Left (2);
	strCity.MakeUpper();

	CString sDate = sFileName.Mid (2,8);

	tm=CTime::GetCurrentTime();
	strToday=tm.Format("%Y%m%d");

	bIsSh=(strCity==_T("SH")) ? true : false;
	bIsToday=(strToday==sDate) ? true : false;
	strID=sFileName.Mid(10,sFileName.Find(_T('.'))-10);
	strID.MakeUpper ();

	FIRSTHEAD TempFirstHead;
	SECONDHEAD TempSecondHead;
	TRADE_DETAIL_H_PER* pHS;
	int nHs=sizeof(TRADE_DETAIL_H_PER);
	int nFHBytes=sizeof(FIRSTHEAD);		
	int nDataBytes=nFHBytes+4096*sizeof(SECONDHEAD);	//��ʽ���ݵ�ƫ����
	int nTouchPerBlock=240;							//ÿ�������ɵĳɽ�����
	int nTouchSize=sizeof(TRADE_DETAIL_H_PER);
	int nBlockSize=nTouchPerBlock*nTouchSize;
	int nSHBytes=sizeof(SECONDHEAD);
	CFile f;
	try
	{
		if (f.Open(strFileName,CFile::modeRead,NULL)==0)
			return FALSE;

		f.Read(&TempFirstHead,nFHBytes);
		f.Read(&TempSecondHead,nSHBytes);
		int nTouch=TempSecondHead.NumTouch;
		for (int i=0;i<nTouch;i++)
		{
			pHS=new TRADE_DETAIL_H_PER;
			f.Read(pHS,nHs);
			if(pHS->price == 0 )
				delete pHS;
			else
				buySellList.AddHead(pHS);

			//lmb 20040913
			if(bIsToday && nTouch == 240)
			{
				Tidxd** m_Tidx;
				Rsdn1** m_Nidx;
				CMainFrame::m_pDoc ->m_sharesInformation.GetIndexTidxd(m_Tidx); 
				CMainFrame::m_pDoc ->m_sharesInformation.GetIndexRsdn(m_Nidx); 
				if(bIsSh && strID == CSharesCompute::GetIndexSymbol(0))
				{

					memcpy(&m_Tidx [0][i],&(pHS->buySellOrIndex .indexInfo .tidxd ),sizeof(Tidxd));
					m_Nidx [0][i].dnn = pHS->buySellOrIndex .indexInfo .decline ;
					m_Nidx [0][i].rsn  = pHS->buySellOrIndex .indexInfo .advance  ;
				}
				else if(bIsSh == false && strID == CSharesCompute::GetIndexSymbol(1))
				{
					memcpy(&m_Tidx [1][i],&(pHS->buySellOrIndex .indexInfo .tidxd ),sizeof(Tidxd));
					m_Nidx [1][i].dnn = pHS->buySellOrIndex .indexInfo .decline ;
					m_Nidx [1][i].rsn  = pHS->buySellOrIndex .indexInfo .advance  ;
				}
			}
		}
		f.Close();
	}
	catch(...)
	{
		f.Close();
		return FALSE;
	}
	return TRUE;
}