// DlgMYXMZ.cpp : implementation file
//

#include "stdafx.h"
#include "WH.h"
#include "DlgMYXMZ.h"
#include "Stockdrv.h"
#include "MainFrm.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDialogMYXMZ dialog


CDialogMYXMZ::CDialogMYXMZ(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogMYXMZ::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDialogMYXMZ)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	
	
}


void CDialogMYXMZ::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialogMYXMZ)
	DDX_Control(pDX, IDC_MYXMZLIST, m_lstStockTypeName);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDialogMYXMZ, CDialog)
	//{{AFX_MSG_MAP(CDialogMYXMZ)
	ON_MESSAGE(WM_MYMSG,DealMsg)	
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialogMYXMZ message handlers
LRESULT CDialogMYXMZ::DealMsg(WPARAM wParam,LPARAM lParam)
{
	
	int i=0;
	PBYTE pFileBuf = NULL;
	RCV_REPORT_STRUCTEx Buf;
	RCV_DATA *	pHeader;
	CString m_FileName;      
	pHeader = (RCV_DATA *) lParam;
    CString m_File;
	switch( wParam )
	{
	case RCV_REPORT: 		//股票行情接收
			
		if(pHeader->m_pReport[i].m_cbSize ==SH_MARKET_EX||pHeader->m_pReport[i].m_cbSize ==SZ_MARKET_EX)//如果是老格式，则进行转化
			{CTime tm;
			CString str;
	//		int nRow;			
			tm=CTime::GetCurrentTime();
			str=tm.Format("%H:%M:%S");
			//nRow=m_lstStockTypeName.GetItemCount();
			m_lstStockTypeName.InsertItem(0,str,0);
			m_lstStockTypeName.SetItemText(0,1,"实时行情");
				RCV_REPORT_STRUCTEx_OLD *pMyBuf;
				pMyBuf=(RCV_REPORT_STRUCTEx_OLD*)pHeader->m_pReport;
				m_lstStockTypeName.SetItemText(0,2,pMyBuf[0].m_szLabel);
				m_lstStockTypeName.SetItemText(0,3,pMyBuf[0].m_szName);
		}
		else
		{
			CTime tm;
			CString str;
	//		int nRow;			
			tm=CTime::GetCurrentTime();
			str=tm.Format("%H:%M:%S");
			//nRow=m_lstStockTypeName.GetItemCount();
			m_lstStockTypeName.InsertItem(0,str,0);
			m_lstStockTypeName.SetItemText(0,1,"实时行情");
			memset(&Buf,0,sizeof(Buf));
				BYTE *totalmem;
				totalmem=(BYTE *)pHeader->m_pReport;
				BYTE mymem[142];
				memcpy(mymem,totalmem,142);
				char sym[10];
				char name[32];
				/*WORD mysize;
				time_t mytime;
				WORD mymarket;
				memcpy(&mysize,mymem,2);
				memcpy(&mytime,mymem+2,4);
				memcpy(&mymarket,mymem+6,2);*/
				memcpy(sym,mymem+8,10);
				memcpy(name,mymem+18,32);
				/*Buf.m_cbSize=mysize;
				Buf.m_time=mytime;
				Buf.m_wMarket=mymarket;*/
				strcpy(Buf.m_szLabel,sym);
				strcpy(Buf.m_szName,name);
				//memcpy(&(Buf.m_fLastClose),mymem+50,94);
			m_lstStockTypeName.SetItemText(0,2,Buf.m_szLabel);
			m_lstStockTypeName.SetItemText(0,3,Buf.m_szName);
//			m_lstStockTypeName.SetHotItem(0); 
			
		}
				
		break;
	case RCV_FILEDATA:
	    if( !pHeader->m_pData || pHeader->m_wDataType == FILE_TYPE_RES)
		{
			TRACE("MSG: CStkDemo::OnStkDataOK, Replaced data \n");
			break;
		}
		int index=0;
		CString StockId;	
		switch(pHeader->m_wDataType)
		{
		case FILE_HISTORY_EX:				// 补日线数据
	    case FILE_POWER_EX:
		case FILE_MINUTE_EX:					// 补分钟线数据            		
			do
			{
				if(pHeader->m_pMinute[index].m_time==EKE_HEAD_TAG)
				{
					StockId=pHeader->m_pMinute[index].m_head.m_szLabel;			
					CTime tm;
					CString str;
					int nRow;
					tm=CTime::GetCurrentTime();
					str=tm.Format("%H:%M:%S");
					//nRow=m_lstStockTypeName.GetItemCount();
					m_lstStockTypeName.InsertItem(0,str,1);
					if(pHeader->m_wDataType == FILE_HISTORY_EX)
						m_lstStockTypeName.SetItemText(0,1,"补日K线");
					else
						m_lstStockTypeName.SetItemText(0,1,"补分时线");
					m_lstStockTypeName.SetItemText(0,2,StockId);
					m_lstStockTypeName.SetHotItem(0);
				}
				index++;				 
			}while(index<pHeader->m_nPacketNum)	;	

			break;

		case FILE_BASE_EX:	// 钱龙兼容基本资料文件,m_szFileName仅包含文件名
		case FILE_NEWS_EX:						// 新闻类,其类型由m_szFileName中子目录名来定
    		m_File.Format("%s",pHeader->m_File.m_szFileName);
			//检查该文件当天是否已接收过           
			CMainFrame* pMf = (CMainFrame* )AfxGetMainWnd();
			if(pMf->m_pDoc->m_NewsFileNameMap.Lookup( m_File, m_FileName ) != TRUE)
			{
					CTime tm;
					CString str;
//					int nRow;
					tm=CTime::GetCurrentTime();
					str=tm.Format("%H:%M:%S");
					//nRow=m_lstStockTypeName.GetItemCount();
					m_lstStockTypeName.InsertItem(0,str,2);
					if(pHeader->m_wDataType==FILE_BASE_EX)
					{
						m_lstStockTypeName.SetItemText(0,1,"基本资料");
					}
					else
					{
						m_lstStockTypeName.SetItemText(0,1,"新闻");
					}
					m_lstStockTypeName.SetItemText(0,2,m_File);
					m_lstStockTypeName.SetHotItem(0);
			}		
			break;
		}
	}
	while(m_lstStockTypeName.GetItemCount()>14)
	{
		for(int i=0;i>1000 ||m_lstStockTypeName.GetItemCount()>14;i++)
		m_lstStockTypeName.DeleteItem(m_lstStockTypeName.GetItemCount()-1);		
	}
	return 0L;
}

BOOL CDialogMYXMZ::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	HICON hIcon;
	CWHApp *pApp=(CWHApp*)AfxGetApp();
	hIcon=pApp->LoadIcon(IDI_MYXMZ);
	this->SetIcon(hIcon,FALSE);
//	m_imagelist.Create(IDB_MYLIST,16,4,RGB(255,255,255));
	m_lstStockTypeName.SetImageList(&m_imagelist,LVSIL_SMALL);  
	//LV_COLUMN Col;//定义一个列头
	//Col.mask=LVCF_FMT|LVCF_WIDTH|LVCF_TEXT|LVCF_SUBITEM;
	m_lstStockTypeName.InsertColumn(0,"时间",LVCFMT_CENTER,100);
	m_lstStockTypeName.InsertColumn(1,"类型",LVCFMT_CENTER,80);
	m_lstStockTypeName.InsertColumn(2,"股票代码",LVCFMT_CENTER,80);
	m_lstStockTypeName.InsertColumn(3,"股票名称",LVCFMT_CENTER,100);
	SetWindowLong(m_lstStockTypeName.m_hWnd,GWL_STYLE,WS_VISIBLE|WS_CHILD|WS_BORDER|LVS_REPORT|LVS_EDITLABELS);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
