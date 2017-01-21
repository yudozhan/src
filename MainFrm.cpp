// MainFrm.cpp : implementation of the CMainFrame class
//


#include "stdafx.h"
#include "WH.h"
#include "CTaiShanDoc.h"
#include "managerstocktype.h"
#include "vwbase.h"
#include "CTaiShanKlineShowView.h"
#include "ChildFrm.h"
#include <stdio.h>
#include <share.h>
#include  <io.h>
#include "MyTreeView.h"
#include "cellRange.h"
#include "shlwapi.h"
#include "DownLoadDlg.h"
#include "WideDownLoadDlg.h"
#include "WideDownLoadDlg.h"
#include "CTaiShanReportView.h"
#include "InfoView.h"
#include "MainFrm.h"
#include  "afxpriv.h"
#include  "DIALOGGSGL.h"
#include "BgridCtrl.h"
#include "CSharesBigBillDlg.h"
#include "CTaiScreen.h"
#include "CTaiKlineIndicatorsManage.h"
#include "CTaiScreenTest.h"
#include "CAlertSystem.h"
#include "CAlertPopDlg.h"
#include "RecvStockCode.h"
#include "NineShowFrm.h"
#include "GetSetReg.h"
//Added By CuiWei.2000-08-03
#include	"ManageData.h"
#include	"ImportData.h"       
#include	"ExportData.h"       
#include	"ClearData.h"        
         
#include	"SymbolContrast.h"   
#include	"ManageInvest.h"
#include	"CaiwuData.h"
#include	"ChuquanData.h"

#include    "investlogin.h"
//#include    "RiskIndex.h"

#include    "Option.h"
#include    "Sysface.h"
#include    "Syssetup.h"
#include    "SysSetParaPage.h"
#include    "direct.h"
#include "CTaiKlineDlgNeuralLearn.h"
#include "DlgMYXMZ.h"

//added by qyp 2002.10.9
#include "LogonDlg.h"
#include "DlgShowInformation.h"
#include "XgnExtDataDlg.h"
#include "CwdCloseWork.h"
//#include "DownFilesDlg.h"

//
//Added by Xia Qiankun 2002/5/11
//
//#include "LoginDlg.h"
//#include "InvestManageDlg.h"
//end

#define ID_SIZECHANGE   2
#define ID_GPSJCL   200
#define TJXG_ALERT  400    /////The Tine Event of Choose_stock_Alarm
#define MaxShareMem   (1024*1024)


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
#ifdef WIDE_NET_VERSION
	CClientTransmitManageWnd* CMainFrame::m_pClientTransmitManageWnd;
	//discribe logon status:if value=TRUE,logon success;value=FALSE,logon failed...
	BOOL CMainFrame::m_bLogonSuccess;
#endif
CTaiShanDoc * CMainFrame::m_pDoc = NULL;
CTaiScreen *CMainFrame::m_pWndTJXG;
CTaiKlineDlgNeuralLearn *CMainFrame::m_pWndNL;
CTaiScreenTest *CMainFrame::m_pWndTJXGTest;

int	CMainFrame::m_iMenuItemCount = 0;
/////////////////////////////////////////////////////////////////////////////
// CMainFrame
bool m_gbDestroyed = false;
IMPLEMENT_DYNAMIC(CMainFrame, CMDIFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CMDIFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	ON_WM_SHOWWINDOW()
	ON_MESSAGE(Gp_Msg_StkData,OnStkDataOK)
	ON_MESSAGE(DA_SERVICE_MSG_EXT,OnExtService)
	ON_WM_DESTROY()
	ON_WM_TIMER()
	ON_WM_SIZE()
	ON_WM_NCHITTEST()
	ON_COMMAND(ID_VIEW_COMPANYINFO, OnViewCompanyinfo)
	ON_COMMAND(ID_VIEW_GGXX, OnViewGgxx)
	ON_COMMAND(ID_FULLSCREEN, OnFullscreen)
	ON_COMMAND(ID_NOMALSCREEN, OnNomalscreen)
	ON_WM_GETMINMAXINFO()
	//ON_WM_NCPAINT()
	ON_COMMAND(ID_TOOL_SJGL, OnToolSjgl)
	ON_COMMAND(ID_TOOL_GSGL, OnToolGsgl)
	ON_COMMAND(ID_TJXG_TJXG, OnTjxgTjxg)
	ON_COMMAND(ID_VIEW_XX, OnViewXx)
	ON_COMMAND(ID_STOP_CUOHE, OnStopCuohe)
	ON_COMMAND(ID_TOOL_TZGL, OnToolTzgl)
	ON_COMMAND(ID_RISKINDEX, OnToolRiskIndex)
	ON_UPDATE_COMMAND_UI(ID_FULLSCREEN, OnUpdateFullscreen)
	ON_UPDATE_COMMAND_UI(ID_NOMALSCREEN, OnUpdateNomalscreen)
	ON_COMMAND(ID_STOCK_BIG, OnOpenBigBill)
	ON_COMMAND(ID_F3, OnF3)
	ON_COMMAND(ID_F4, OnF4)
	ON_WM_CLOSE()
	ON_COMMAND(ID_HELP_TOPIC, OnHelpTopic)
	ON_UPDATE_COMMAND_UI(ID_TOOL_GSGL, OnUpdateToolGsgl)
	ON_UPDATE_COMMAND_UI(ID_BASEINFO_GRID, OnUpdateBaseinfoGrid)
	ON_UPDATE_COMMAND_UI(ID_CUOHE_OPEN, OnUpdateCuoheOpen)
	ON_COMMAND(ID_GRID, OnGrid)
	ON_UPDATE_COMMAND_UI(ID_GRID, OnUpdateGrid)
	ON_UPDATE_COMMAND_UI(ID_F3, OnUpdateF3)
	ON_UPDATE_COMMAND_UI(ID_F4, OnUpdateF4)
	ON_COMMAND(ID_WINDOW_NEW, OnWindowNew)
	ON_UPDATE_COMMAND_UI(ID_WINDOW_NEW, OnUpdateWindowNew)
	ON_COMMAND(ID_TJXG_ALARM, OnTjxgAlarm)
	ON_COMMAND(ID_TOOL_BLOCK_MANA, OnToolBlockMana)
	ON_COMMAND(ID_UPDATE_DAY_LINE, OnUpdateBlockDayLine)
	ON_COMMAND(ID_UPDATE_MIN5_LINE, OnUpdateBlockMin5Line)
	ON_COMMAND(ID_DOWNLOAD_20, OnDownload20)
	ON_COMMAND(ID_CLOSE_RECEIVER, OnCloseReceiver)
	ON_UPDATE_COMMAND_UI(ID_CLOSE_RECEIVER, OnUpdateCloseReceiver)
	ON_COMMAND(ID_ENTER_INTERNET, OnEnterInternet)
	ON_COMMAND(ID_TJXG_MM_TEST, OnTjxgMmTest)
	ON_COMMAND(ID_TOOL_NNET_TRAIN, OnToolNnetTrain)
	ON_COMMAND(ID_TOOL_ZDWT, OnToolAutoSubmit)
	ON_COMMAND(ID_DAY_LINE, OnDayLine)
	ON_COMMAND(ID_TIMESHARING, OnMin1Line)
	ON_UPDATE_COMMAND_UI(ID_STOCK_BIG, OnUpdateStockBig)
	ON_UPDATE_COMMAND_UI(ID_TJXG_ALARM, OnUpdateTjxgAlarm)
	ON_COMMAND(ID_F6,OnF6)
	ON_WM_PAINT()
	ON_COMMAND(IDM_SORT_INDEX, OnSortIndex)
	ON_COMMAND(ID_XGN_EXT_DATA, OnXgnExtData)
	ON_COMMAND(ID_CLEAR_REALDATA, OnClearRealdata)
	ON_WM_LBUTTONDOWN()
	ON_WM_NCLBUTTONDOWN()
	ON_WM_MOUSEACTIVATE()
	ON_COMMAND(ID_CALC, OnCalc)
	ON_COMMAND(ID_COMPANYINFO, OnCompanyinfo)
	ON_COMMAND(IDM_GDFX_REGISTE, OnGdfxRegister)

	ON_WM_DRAWITEM()
	ON_WM_MEASUREITEM()
	ON_WM_NCLBUTTONUP()
	ON_WM_NCLBUTTONDBLCLK()
	ON_WM_NCMOUSEMOVE()
	//}}AFX_MSG_MAP
	//ON_COMMAND_RANGE(ID_GO_BACK, ID_GO_BACK, OnDrawline)
	ON_MESSAGE(WM_USER_TJXG,OnTjxg)
	ON_MESSAGE(WM_USER_DrawKlineWindow,DrawKlineWindow)
#ifdef WIDE_NET_VERSION
	ON_COMMAND(ID_LOGIN_SERVER, OnLoginServer)
	ON_COMMAND(ID_RECV_STOCKCODE, OnRecvStockcode)
#endif
	ON_WM_INITMENUPOPUP()
	ON_WM_INITMENU()
	ON_WM_MENUCHAR()
	ON_COMMAND(ID_VIEW_BLOCKANALYSIS, &CMainFrame::OnViewBlockanalysis)
END_MESSAGE_MAP()

static UINT palette[] =
{
	// same order as in the bitmap 'palette.bmp'
	ID_PALETTEERASE,
	ID_PALETTEPEN,
	ID_PALETTESELECT,
	ID_PALETTEBRUSH,
	ID_PALETTESPRAY,
	ID_PALETTEPAINT,
	ID_PALETTELINE,
	ID_PALETTEEYEDROP,
	ID_PALETTEMAG,
	ID_PALETTERECT,
	ID_PALETTEROUND,
	ID_PALETTEOVAL
};


/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction
LRESULT CMainFrame::OnTjxg(WPARAM wParam, LPARAM lParam)
{
   if(!m_pDoc)
		return 0L;
	int length=m_pWndTJXG->m_resultarray.GetSize();
    m_pDoc->m_StockTypeMap[2].RemoveAll();
    m_pDoc->m_ScreenList.RemoveAll();
	if(length>0)
	{
		for(int i=0;i<length;i++)
		{
			SymbolKind m_SymbolKind;
			CString m_zqdm,m_zqdmkind;
			m_SymbolKind = m_pWndTJXG->m_resultarray.GetAt(i);
            m_zqdm=m_SymbolKind.m_chSymbol;
            m_zqdmkind=m_pDoc->GetStockKindString(m_SymbolKind.m_nSymbolKind);
            m_zqdmkind+=m_zqdm;
	        m_pDoc->m_StockTypeMap[2][m_zqdmkind]=m_zqdm;
			SharesSymbol m_StockSymbol;
			strcpy(m_StockSymbol.sharesymbol,m_SymbolKind.m_chSymbol);
            m_StockSymbol.nKind=m_SymbolKind.m_nSymbolKind;  
			m_pDoc->m_ScreenList.Add(m_StockSymbol);
		}
	}
	
	POSITION pos = m_pDoc->GetFirstViewPosition(); 
	while (pos != NULL) 
	{
	  CView* pView = m_pDoc->GetNextView(pos);
	  if(pView->IsKindOf( RUNTIME_CLASS(CTaiShanReportView)))
	  {
			CTaiShanReportView *m_View;
			m_View=(CTaiShanReportView *)pView;
			m_View->GetParent()->ShowWindow(SW_SHOWMAXIMIZED);
			m_View->ChangeToPage(TJXGPAGE,TRUE); 

	  }
	} 
	return 1L;
}

CMainFrame::CMainFrame():m_dlgLeftPop(this)
{
	m_iMenuWidth = 0;

	m_fDLL = FALSE;
	m_pWndNL = NULL;
	m_pWndTJXGTest = NULL;
	m_pWndTJXG = NULL;
	m_pDlgAlarmPop = NULL;
	m_pDlgAlarm = NULL;
	m_bAlarmExec = false;
	m_pDoc=NULL;
    m_MDIChildWnd=NULL;
	// TODO: add member initialization code here
    m_bRunFlag=FALSE;	
    m_EnableF10 = true;
	m_EnableF9 = true;
    m_bFullScreen = FALSE;
	m_BaseInfo = true;
	
	InformationLoop=0;
	m_drawLineToolBar = new CToolBar;
	m_F9ORF10 = FALSE;

	nLKind[0]=HS_KLINE;
	nLKind[1]=MIN1_KLINE;
	nLKind[2]=MIN5_KLINE;
	nLKind[3]=MIN15_KLINE;
	nLKind[4]=MIN30_KLINE;
	nLKind[5]=MIN60_KLINE;
	nLKind[6]=DAY_KLINE;
	nLKind[7]=WEEK_KLINE;
	nLKind[8]=MONTH_KLINE;
	nLKind[9]=YEAR_KLINE ;
	nLKind[10]=MANY_DAY_KLINE;

	CAlertSystem::LoadFile();
	//add by qyp 2002.10.22
#ifdef WIDE_NET_VERSION
	m_pClientTransmitManageWnd=new CClientTransmitManageWnd((CMDIFrameWnd*)this);
	m_bLogonInterface=FALSE;
	m_bLogonSuccess=FALSE;
#endif
	////////界面优化风格/////////////////////////////////////
	//TCHAR szStylesPath[_MAX_PATH];
	//CString strStylesPath;

	//VERIFY(::GetModuleFileName(AfxGetApp()->m_hInstance, szStylesPath, _MAX_PATH));		
	//
	//CString csStylesPath(szStylesPath);
	//int nIndex  = csStylesPath.ReverseFind(_T('\\'));
	//if (nIndex > 0) 
	//{
	//	csStylesPath = csStylesPath.Left(nIndex);
	//}
	//else 
	//{
	//	csStylesPath.Empty();
	//}
	////strStylesPath = csStylesPath + _T("\\Styles\\");xtpSkinApplyFrame | 

	//XTPSkinManager()->SetApplyOptions(xtpSkinApplyColors | xtpSkinApplyMetrics);
	////XTPSkinManager()->LoadSkin(strStylesPath + _T("Nocturnal.msstyles"));	
	////XTPSkinManager()->LoadSkin(strStylesPath + _T("Office2007.cjstyles"));	
	//XTPSkinManager()->LoadSkin(strStylesPath + _T("Office2007.cjstyles"),"NormalAqua.ini");	

	////	XTPSkinManager()->AddColorFilter(new CXTPSkinManagerColorFilterShiftRGB(-40, -80, -40));
	////	//XTPSkinManager()->AddColorFilter(new CColorFilterMonochrome());
	//XTPSkinManager()->RedrawAllControls();
	//XTPPaintManager()->RefreshMetrics();
}

CMainFrame::~CMainFrame()
{
#ifdef WIDE_NET_VERSION
	delete m_pClientTransmitManageWnd;
#endif
	delete m_drawLineToolBar;
}


int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CMDIFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	if(CWHApp::m_gbUseExe == true)
	{
		if(!m_memFile.OpenShare ("",CFile::modeReadWrite
			|CFile::modeCreate|CFile::shareDenyNone
			|CFile::modeNoTruncate,MaxShareMem,"Ws_SendMessageDrv"  ))
		{
			ASSERT(FALSE);
			AfxMessageBox("不能共享内存！");
		}
	}

	ModifyStyle(WS_CAPTION,0,SWP_FRAMECHANGED );

	if (!InitCommandBars())
		return -1;

	CXTPPaintManager::SetTheme(xtpThemeNativeWinXP);

	XTP_COMMANDBARS_ICONSINFO* pIconsInfo = XTPPaintManager()->GetIconsInfo();
	pIconsInfo->bUseDisabledIcons = TRUE;
	pIconsInfo->bIconsWithShadow = FALSE;

	CXTPCommandBars* pCommandBars = GetCommandBars();
	
	pCommandBars->GetCommandBarsOptions()->bShowExpandButtonAlways = FALSE;


	CXTPToolBar* pStandardBar = (CXTPToolBar*)pCommandBars->Add(_T("标准"), xtpBarLeft);
	if (!pStandardBar ||
		!pStandardBar->LoadToolBar(IDR_MAINFRAME))//IDR_MAINFRAME
	{
		TRACE0("Failed to create toolbar\n");
		return -1;
	}
	pStandardBar->EnableDocking(xtpFlagAlignLeft);
	pStandardBar->SetShowGripper(FALSE);

	CreateToolDrawLine();

	pCommandBars->GetToolTipContext()->SetStyle(xtpToolTipOffice);

//	CTaiShanKlineShowView::CheckDiskFreeSpace();

	//m_dlgLeftPop
	if(!m_dlgLeftPop.Create(m_dlgLeftPop.IDD,this))
	{
		TRACE0("Failed to create Dialog\n");
		return -1;      // fail to create
	}
	//Create Sjl ToolBar
	if(!m_TestDialogbar.Create(IDD_DIALOGBAR,this))
	{
		TRACE0("Failed to create Dialog bar\n");
		return -1;      // fail to create
	}

    CRect Rect;
	GetClientRect(&Rect);
    m_TestDialogbar.SetWindowPos(NULL,1,0,Rect.Width( )-4,55,SWP_NOMOVE);

	//Create Sjl StatusBar
	if (!m_wndStatusBar.Create(this))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}

	//pCommandBars->SetTheme(xtpThemeRibbon);
	//CXTPPaintManager::SetTheme(xtpThemeWhidbey);

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CMDIFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs
		//cs.dwExStyle|=WS_EX_TOPMOST;
	//cs.style &= ~WS_CAPTION;
	//cs.style &= ~WS_OVERLAPPEDWINDOW ;

	return TRUE;
}


LONG CMainFrame::OnStkDataOK(
		UINT wFileType,				// 文件类型, 参见 StockDrv.H
		LONG lPara)					// 保留
{
	int i;
	PBYTE pFileBuf = NULL;

#ifdef WIDE_NET_VERSION
	RCV_WIDEREPORT_STRUCTEx Buf;
#else
	RCV_REPORT_STRUCTEx Buf;
	//RCV_REPORT_STRUCTEx_OLD MyBuf;
#endif
	RCV_DATA *	pHeader;
	DWORD m_dwAttrib;
	CString m_FileName;
    
	pHeader = (RCV_DATA *) lPara;
	
	LONG m_FileLong;
    int m_FileType;
	FILE * fp;
    CString m_File;
		
		
    //	对于处理较慢的数据类型,建议将数据备份,另创建一线程处理
    if(m_MDIChildWnd==NULL)               //检测子框架是否激活
	{
	   m_MDIChildWnd=MDIGetActive();
	   if (m_MDIChildWnd==NULL)
	      return 0;
	   
	   while(!m_MDIChildWnd->IsKindOf(RUNTIME_CLASS(CChildFrame))&&this->m_MDIChildWnd !=NULL)
	   {
	      this->MDINext();
	      m_MDIChildWnd=MDIGetActive();
	   }
        
	}

	if (m_pDoc==NULL)          //检文档类是否激活
    { 
        return 0;
    } 

	if(m_pDoc->m_nIsClose)
	   return 0;

	try{
	switch( wFileType )
	{
	case RCV_REPORT:                                    //股票行情接收
		for(i=0; i<pHeader->m_nPacketNum; i++)
		{
#ifdef WIDE_NET_VERSION
			Buf = pHeader->m_pWideReport[i];
            m_pDoc->m_sharesCompute.WideStockDataUpdate(&Buf);                  //股票即时行情数据处理程序   
			CString ss = Buf.pMarketReport.m_szLabel ;
			if(ss == "0001")
			TRACE("%d   ;%s = 0001\r\n",GetTickCount(),Buf.pMarketReport.m_szLabel );
#else
			{
				BYTE *totalmem=(BYTE *)pHeader->m_pReport;
				int Size = pHeader->m_pReport[0].m_cbSize;
				memcpy(&Buf, totalmem+i*Size, sizeof(RCV_REPORT_STRUCTEx));
			}				
            m_pDoc->m_sharesCompute.StockDataUpdate(&Buf);                  //股票即时行情数据处理程序


			if(m_pDoc->m_sharesSymbol == Buf.m_szLabel)
			{
				m_pDoc->UpdateAllViews(NULL);
			}
#endif
		}
		
		m_wndStatusBar.bColor[m_wndStatusBar.nCount]=RGB(0,128,255);
		m_wndStatusBar.PaintRect();
		m_wndStatusBar.nCount++;
		m_wndStatusBar.nCount%=6;
		break;
	case RCV_FILEDATA:
	    if( !pHeader->m_pData || pHeader->m_wDataType == FILE_TYPE_RES)
		{
			TRACE("MSG: CStkDemo::OnStkDataOK, Replaced data \n");
			break;
		}
		//afxDump << pHeader->m_wDataType <<"\n";
		switch(pHeader->m_wDataType)
		{
		case FILE_HISTORY_EX:						// 补日线数据
		    DAY_TOTAL_STRUCTEx m_GpDay;
			m_GpDay.m_RcvDay= pHeader->m_pDay;
            m_GpDay.Day_Count =pHeader->m_nPacketNum;
            //if(m_pDoc->m_systemOption.autoday)
			{
                m_pDoc->m_sharesCompute.StockDataDayUpdate( &m_GpDay);                  //股票补日线处理程序             

				m_wndStatusBar.bColor[m_wndStatusBar.nCount]=RGB(255,0,0);
				m_wndStatusBar.PaintRect();
				m_wndStatusBar.nCount++;
				m_wndStatusBar.nCount%=6;

			}
	        
			break;
			//Marenan +
		case FILE_HISTORY_MINUTE_EX:						// 补五分线数据
		    MIN5_TOTAL_STRUCTEx m_GpMin5;
			m_GpMin5.m_RcvMin5= pHeader->m_pHisMinute;
            m_GpMin5.Min5_Count =pHeader->m_nPacketNum;
            
			m_pDoc->m_sharesCompute.StockDataMin5Update( &m_GpMin5);                  //股票补日线处理程序             

			m_wndStatusBar.bColor[m_wndStatusBar.nCount]=RGB(0,255,0);
			m_wndStatusBar.PaintRect();
			m_wndStatusBar.nCount++;
			m_wndStatusBar.nCount%=6;
      
			break;
        case FILE_POWER_EX:
#ifndef WIDE_NET_VERSION
			POWER_TOTAL_STRUCTEx m_PowerData;
            m_PowerData.Power_Count=pHeader->m_nPacketNum;
			m_PowerData.m_RcvPower=pHeader->m_pPower;
            m_pDoc->m_sharesCompute.StockDataPowerUpdate(& m_PowerData);                  //股票补分钟线处理程序             
#endif
			break;

		case FILE_MINUTE_EX:						// 补分钟线数据
            MIN_TOTAL_STRUCTEx m_GpMinute;
            m_GpMinute.m_RcvMinute = pHeader->m_pMinute;
            m_GpMinute.Min_Count = pHeader->m_nPacketNum;
#ifdef WIDE_NET_VERSION
            m_pDoc->m_sharesCompute.WideStockDataMinUpdate(& m_GpMinute);                  //股票补分钟线处理程序             
#else
 		    if(m_pDoc->m_systemOption.autominute)
			{
                m_pDoc->m_sharesCompute.StockDataMinUpdate(& m_GpMinute);                  //股票补分钟线处理程序             
				m_wndStatusBar.bColor[m_wndStatusBar.nCount]=RGB(128,0,255);
				m_wndStatusBar.PaintRect();
				m_wndStatusBar.nCount++;
				m_wndStatusBar.nCount%=6;
			}
#endif
	        
			break;

		case FILE_BASE_EX:	// 钱龙兼容基本资料文件,m_szFileName仅包含文件名
			m_wndStatusBar.bColor[m_wndStatusBar.nCount]=RGB(255,255,0);
			m_wndStatusBar.PaintRect();
			m_wndStatusBar.nCount++;
			m_wndStatusBar.nCount%=6;
	        
			m_File.Format("%s",pHeader->m_File.m_szFileName);

			//检查该文件当天是否已接收过
//            if(this->m_pDoc->m_NewsFileNameMap.Lookup( m_File, m_FileName ) != TRUE)
			{
				::SetCurrentDirectory(this->m_pDoc->m_CurrentWorkDirectory);  
				if(pHeader->m_File.m_dwAttrib==SH_MARKET_EX)
					m_FileName=g_sF10sh+m_File;
				else
					m_FileName=g_sF10sz+m_File;

				m_FileLong=pHeader->m_File.m_dwLen    ;
				m_FileType=pHeader->m_wDataType ;
				m_dwAttrib=pHeader->m_File.m_dwAttrib   ;
				if (m_dwAttrib==-1) 
					break; 
                
				if( (fp=fopen(m_FileName.GetBuffer(0) ,"wb+"))!=NULL) 					//将信息写入文件
				{
				   fwrite(pHeader->m_pData , sizeof( char ),m_FileLong , fp );      
				   fclose(fp) ;
				}
  
				fp=_fsopen("news\\news.log","r+b",SH_DENYNO);
	            fseek(fp,0,SEEK_END);
				char filename[251];
				memset(filename, 0, sizeof(char)*250);
				strcpy(filename,m_File.GetBuffer(0));
				fwrite(filename,250,1,fp);
                fclose(fp);
                m_pDoc->m_NewsFileNameMap[m_File]=m_File;
			}

			break;
		case FILE_NEWS_EX:						// 新闻类,其类型由m_szFileName中子目录名来定
    		m_File.Format("%s",pHeader->m_File.m_szFileName);
			//检查该文件当天是否已接收过
            if(this->m_pDoc->m_NewsFileNameMap.Lookup( m_File, m_FileName ) != TRUE)
			{
			   ::SetCurrentDirectory(this->m_pDoc->m_CurrentWorkDirectory);  
               //begin=================added by zhufeng===========================
			   int nPos;
			   BOOL bPath;
			   bPath=FALSE;
			   int nTmp;
			   int ab;
				nPos=m_File.Find("\\",0);
				if (nPos==-1) 
				{
					nTmp=m_File.Find("/",0);
					while(nTmp==0)
					{
						CString delchar=m_File.Mid(1);
						nTmp=delchar.Find("/",0);
						m_File=delchar;
					}
					if(nTmp==-1)
						bPath=FALSE;
					else
						bPath=TRUE;	
					ab=nTmp;
				}
				else
				{
					while(nPos==0)
					{
						CString delchar=m_File.Mid(1);
						nPos=delchar.Find("\\",0);
						m_File=delchar;
					}
					if (nPos!=-1)
					{
						ab=nPos;
						bPath=TRUE;	
					}
					nTmp=m_File.Find("/",0);
					while(nTmp==0)
					{
						CString delchar=m_File.Mid(1);
						nTmp=delchar.Find("/",0);
						m_File=delchar;
					}
					if(nTmp!=-1)
					{
						ab=nTmp;
						bPath=TRUE;	
					}
				}
				if(!bPath)
				{
				//end=============================================================
					switch(pHeader->m_File.m_dwAttrib)
					{
					case News_Sha_Ex:				 			// 上证消息
						m_FileName.Format("news\\shanghai\\%d.dat",this->m_pDoc->m_lDay ) ;
						break;
					case News_Szn_Ex:				 			// 深证消息
						m_FileName.Format("news\\shenzhen\\%d.dat",this->m_pDoc->m_lDay ) ;
						break;
					case News_Fin_Ex:				 			// 财经报道
						m_FileName.Format("news\\cj\\%d.dat",this->m_pDoc->m_lDay ) ;
						break;
					case News_TVSta_Ex:			 			// 电视台通知
						return 0; 
						
					case News_Unknown_Ex:			 			// 未知提供者
						return 0 ;
					}
				//begin2==============================================
				}
				else
				{
					CString sTmp;
					sTmp=m_File;
					sTmp=m_File.Left(ab);
					if(_access(sTmp,0)==-1)
					{
						sTmp="news\\" + sTmp;
						_mkdir(sTmp);
					}
					m_FileName=sTmp;
					m_FileName+="\\";
					CString sFName=m_File.Mid(ab+1);
					m_FileName+=sFName;
				}
				//end2================================================
				m_FileLong=pHeader->m_File.m_dwLen    ;
				m_FileType=pHeader->m_wDataType ;
				m_dwAttrib=pHeader->m_File.m_dwAttrib   ;
				if (m_dwAttrib==-1) 
					break; 

				if(_access(m_FileName.GetBuffer(0),0)!=0)   
				{
						if((fp=_fsopen(m_FileName.GetBuffer(0),"w+b",SH_DENYNO))!=NULL)
						{
							fclose(fp);
						}
				}
 				if( (fp=fopen(m_FileName.GetBuffer(0) ,"r+b"))!=NULL)            //将信息写入文件
                {
					fseek(fp,0,SEEK_END);
				    fwrite(pHeader->m_pData , sizeof( char ),m_FileLong , fp );      
				    fclose(fp) ;
				}
  				if((fp=_fsopen("news\\news.log","r+b",SH_DENYNO))!=NULL)
				{
					fseek(fp,0,SEEK_END);
					char filename[251];
					memset(filename, 0, sizeof(char)*250);
					strcpy(filename,m_File.GetBuffer(0));
					fwrite(filename,250,1,fp);
					fclose(fp);
				}
                m_pDoc->m_NewsFileNameMap[m_File]=m_File;
			}

		default:
			return 0;
		}
		break;
	}
	}
	catch(...)
	{
		ASSERT(FALSE);
	}
	
#ifndef WIDE_NET_VERSION
	HWND mywnd;
	mywnd=(HWND)FindWindow(NULL,"图文卡数据接收明细");
	if(mywnd!=NULL)
	{
		CDialogMYXMZ *pDlg=(CDialogMYXMZ*)mywnd;			 
		::SendMessage(pDlg->m_hWnd,WM_MYMSG,wFileType,(LPARAM)lPara);
	}
	//end
#endif
	return 0L;
}


/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CMDIFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CMDIFrameWnd::Dump(dc);
}

#endif //_DEBUG

extern STARTUPINFO startup;
extern PROCESS_INFORMATION process;

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers
void CMainFrame::HqStock_Init()     //起动股票接收操作
{
	//初始化股票接收
#ifndef WIDE_NET_VERSION
	if(!CWHApp::m_gbDoInitate)
	{
//		if(m_pDoc)
//		m_pDoc->m_bCloseReceiver = TRUE;
		CWHApp::m_gbDoInitate = true;
		return;
	}
	if(CWHApp::m_gbUseExe == true)
	{
		memset( &startup, 0, sizeof( startup ) );
		startup.cb = sizeof( startup );
		memset( &process, 0, sizeof( process ) );
		startup.dwFlags = STARTF_USESHOWWINDOW;
		startup.wShowWindow = SW_HIDE;
		BOOL tSuccess = CreateProcess( _T("Receiver.exe"), NULL, NULL, NULL,
			FALSE, 0, NULL, NULL, &startup, &process );
		//ToTerminateChildProcess();
	}

	char szTitle[256]; 
	//m_StockDriverDll.GetStockDrvInfo(RI_SUPPORTEXTHQ, szTitle);	//设置 Szn Label6 Receive
	//int ok = m_StockDriverDll.Stock_Init(GetSafeHwnd(), CWHApp::m_gMessageID,RCV_WORK_MEMSHARE);	// 数据共享引用
	int ok = m_StockDriverDll.Stock_Init(GetSafeHwnd(), Gp_Msg_StkData,RCV_WORK_SENDMSG);	// 数据共享引用
	m_StockDriverDll.GetStockDrvInfo(RI_SUPPORTEXTHQ, szTitle);
	if(ok>0)
	{
		m_bRunFlag =TRUE;
//		DWORD m_TV_Code = 201;
//		m_StockDriverDll.GetStockDrvInfo(RI_ENABLETS3FILE,&m_TV_Code);
		m_StockDriverDll.GetStockDrvInfo(RI_IDCODE,szTitle);
	}

	m_StockDriverDll.SetExtMsg(DA_SERVICE_MSG_EXT);

	m_pDoc->m_bCloseReceiver=TRUE;
	SetTimer(1007,2000,NULL);
#endif
}
void CMainFrame::HqStock_Quit()     //退出股票接收操作
{
#ifndef WIDE_NET_VERSION
	if(CWHApp::m_gbUseExe == true)
	{
		if (process.hProcess)
		{
			TerminateProcess(process.hProcess,1);
			//CloseHandle( process.hProcess );
			//CloseHandle( process.hThread );
		}
	}

	if(m_bRunFlag==TRUE)
	{
	   m_StockDriverDll.Stock_Quit(GetSafeHwnd());     //退出股票接收操作
    }
	m_bRunFlag=FALSE;
	m_pDoc->m_bCloseReceiver=FALSE;

#endif
}

void CMainFrame::OnDestroy() 
{
	::RemoveProp(GetSafeHwnd(),AfxGetApp()->m_pszExeName);

	m_gbDestroyed = true;

	if(m_pDlgAlarmPop != NULL)
		m_pDlgAlarmPop->DestroyWindow();
	if(m_pDlgAlarm != NULL)
		m_pDlgAlarm->DestroyWindow();
	if(this->m_pWndTJXG != NULL)
	{
		m_pWndTJXG->DestroyWindow();
		m_pWndTJXG = NULL;
	}
	if(m_pWndTJXGTest != NULL)
	{
		m_pWndTJXGTest->DestroyWindow();
		m_pWndTJXGTest = NULL;
	}//	if(m_pWndNL == NULL)
	if(this->m_pWndNL != NULL)
	{
		m_pWndNL->DestroyWindow();
		m_pWndNL = NULL;
	}

	CAlertSystem::SaveFile();

	m_bAlarmExec = false;

	CMDIFrameWnd::OnDestroy();

}



void CMainFrame::OnTimer(UINT nIDEvent) 
{

	CMDIFrameWnd::OnTimer(nIDEvent);
/*
	if(nIDEvent == 6 || nIDEvent == 7)
	{
	  CTaiShanDoc*	pDoc=this->m_pDoc;
	  if(pDoc == NULL)
	  {
		   m_MDIChildWnd=MDIGetActive();
		   if (m_MDIChildWnd==NULL)
			  return ;
		   
		   while(!m_MDIChildWnd->IsKindOf(RUNTIME_CLASS(CChildFrame))&&this->m_MDIChildWnd !=NULL)
		   {
			  this->MDINext();
			  m_MDIChildWnd=MDIGetActive();
		   }
		m_pDoc= (CTaiShanDoc *)m_MDIChildWnd->GetActiveDocument();         
        if (m_pDoc==NULL)
	        return ;
	  }
	  float rd_sh=0,rd_sz=0;

	  if(nIDEvent == 6)
	     KillTimer(6);
	  if(nIDEvent == 7)
		  KillTimer(7);
      return;
	}
  
*/
/*	if(nIDEvent ==INI_DLL)
	{
		if(!m_fDLL)
			this->HqStock_Init();
		m_fDLL=TRUE;

	}*/
//	TestHQ();
    if(nIDEvent == TJXG_ALERT)
		CAlertSystem::CaclAlarm(this);  //////计算

}

static int flagmy=0;

///////////////add by zjh/////////////
void CMainFrame::Drawline() 
{
	if(flagmy==0)
	{
		flagmy=1;
		CPoint pt(GetSystemMetrics(SM_CXSCREEN) -400,
			GetSystemMetrics(SM_CYSCREEN) / 4);
		FloatControlBar(m_drawLineToolBar, pt);

		ShowControlBar(m_drawLineToolBar, TRUE, FALSE);
		RecalcLayout();
	}
	else
	{
		flagmy=0; 
		BOOL bVisible = ((m_drawLineToolBar->GetStyle() & WS_VISIBLE) != 0);
        ShowControlBar(m_drawLineToolBar, TRUE, FALSE);
		RecalcLayout();
	}

	
}

int CMainFrame::CreateToolDrawLine()
{
	//---画线//m_drawLineToolBar
	static int first = 0;
	if(first == 1)
		return 0;

	//CXTPToolBar* pStandardBar = (CXTPToolBar*)GetCommandBars()->Add(_T("Standard"), xtpBarTop);
	//if (!pStandardBar ||
	//	!pStandardBar->LoadToolBar(IDR_DRAWLINE))
	//{
	//	TRACE0("Failed to create toolbar\n");
	//}
	//
	//m_drawLineToolBar = pStandardBar;

	if (!m_drawLineToolBar->CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE |CBRS_SIZE_FIXED |
		CBRS_TOP | CBRS_TOOLTIPS)||
		!m_drawLineToolBar->LoadToolBar(IDR_DRAWLINE)
		)
	{
		TRACE0("Failed to create toolbar\n");
		return 0;      // fail to create
	}
	CPoint pt(GetSystemMetrics(SM_CXSCREEN) -400,
		GetSystemMetrics(SM_CYSCREEN) / 4);
	////////////////////////////////

	int nCount = 22;//frm->m_drawLineToolBar->GetToolBarCtrl().GetButtonCount();
	for (int i = 0; i < nCount; i++)
	{
		UINT nStyle = m_drawLineToolBar->GetButtonStyle(i);
		BOOL bWrap = (((i + 1) % 4) == 0);
		if (bWrap)
			nStyle |= TBBS_WRAPPED;
		else
			nStyle &= ~TBBS_WRAPPED;
		m_drawLineToolBar->SetButtonStyle(i, nStyle);
	}

	m_drawLineToolBar->EnableDocking(0);
	EnableDocking(CBRS_ALIGN_ANY);
	m_drawLineToolBar->SetWindowText(_T("画线工具"));
	ShowControlBar(m_drawLineToolBar, FALSE, TRUE);
    first = 1;
	return 1;
}


void CMainFrame::OnSize(UINT nType, int cx, int cy) 
{
	CMDIFrameWnd::OnSize(nType, cx, cy);
	
	if(cx == 0 && cy == 0)
		return;
	
	CRect rc;
	CMDIFrameWnd::GetWindowRect(&rc);

	if(m_TestDialogbar.m_hWnd)
		m_TestDialogbar.SetWindowPos(NULL,1,0,cx - 4,46,SWP_NOMOVE|SWP_DRAWFRAME);

	/*	CRect r;
	m_pParent->GetClientRect (r);
	m_pParent->ClientToScreen (r);
*/
	CRect rPop;
	CRect r2;

	if(m_dlgLeftPop.m_hWnd)
		m_dlgLeftPop.GetWindowRect (rPop);
	if(m_wndStatusBar.m_hWnd)
		m_wndStatusBar.GetWindowRect (r2);
	if(m_dlgLeftPop.m_hWnd)
		m_dlgLeftPop.SetWindowPos(&wndTop,0,0,rPop.Width (),cy-r2.Height()  ,SWP_NOMOVE|SWP_DRAWFRAME);

}


LRESULT CMainFrame::OnNcHitTest(CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	UINT nWhere = CMDIFrameWnd::OnNcHitTest(point);


	CView *pView;
	switch(nWhere)
	{
    case HTTOP:
    case HTCAPTION:
		 ////////////////
		 pView = GetActiveView();
		 if(pView && pView->IsKindOf(RUNTIME_CLASS(CPreviewView)))
		     return 0;

		 delete pView;
		 //if(!m_dlgLeftPop.IsWindowVisible ())
			//m_TestDialogbar.ShowWindow(SW_SHOW);
		 //else
			//m_TestDialogbar.ShowWindow(SW_HIDE);
		 break;
    case HTLEFT:
		 ////////////////
		 pView = GetActiveView();
		 if(pView == 0)
		 {
		 }
		 else if(pView->IsKindOf(RUNTIME_CLASS(CPreviewView)))
		     return 0;
		 m_dlgLeftPop.ShowWindow(SW_SHOW);
		 //m_TestDialogbar.ShowWindow(SW_HIDE);
		 break;
	case HTMENU:
         if(m_bFullScreen)
		 {
			 CView *pView;
			 pView = GetActiveView();
			 if(pView != NULL && pView->IsKindOf(RUNTIME_CLASS(CPreviewView)))
				 return 0;

			 delete pView;
			 //if(!m_dlgLeftPop.IsWindowVisible ())
				//m_TestDialogbar.ShowWindow(SW_SHOW);
			 //else
				//m_TestDialogbar.ShowWindow(SW_HIDE);
		 }
		 break;
	}


	CRect rtWnd, rcCaptionBar, rtButtons;
	GetWindowRect(&rtWnd); 
	rtWnd.right = rtWnd.left + m_iMenuWidth;//m_iMenuWidth;
	
	return nWhere == HTMENU && !rtWnd.PtInRect(point) ? HTCAPTION : nWhere;
}

BOOL CMainFrame::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
    if(pMsg-> message==WM_MOUSEMOVE)
	{
      int y = HIWORD(pMsg->lParam);
	  if(y > 65)
	    m_TestDialogbar.ShowWindow(SW_HIDE );
	}
    if(pMsg-> message==WM_MOUSEMOVE)
	{
      int x = LOWORD(pMsg->lParam);
	  CRect r ;
	  m_dlgLeftPop.GetWindowRect(r);
	  if(x > r.Width() + 30 )
	    m_dlgLeftPop.ShowWindow(SW_HIDE );
	}
//--------------- Begin --- 2002.11.25 lm Add-------------
	if( pMsg->message == WM_LBUTTONDOWN )
	{										//	弹出左边的菜单
/*		int x = LOWORD(pMsg->lParam);
		int y = HIWORD(pMsg->lParam);
		CPoint pt(x, y );
		::ClientToScreen( pMsg->hwnd, &pt );
		ScreenToClient( &pt );
		if( pt.x < 10 )
			m_dlgLeftPop.ShowWindow( SW_SHOWNORMAL );*/
	}
//--------------- End --- 2002.11.25 lm Add-------------
	//if(pMsg-> message==WM_COMMAND)
	//	m_TestDialogbar.ShowWindow(SW_HIDE );

	return CMDIFrameWnd::PreTranslateMessage(pMsg);
}

void CMainFrame::OnViewCompanyinfo() 
{

}

void CMainFrame::OnCompanyinfo()
{
    m_F9ORF10 = FALSE;
	if(m_EnableF10 == false)
	{
		CWnd * pMainWnd = AfxGetApp()->m_pMainWnd;
		CFrameWnd* pFrameWnd=((CFrameWnd*)pMainWnd)->GetActiveFrame();
		CTaiShanDoc* pDoc=((CMainFrame *)pMainWnd)->m_pDoc;

		if(pDoc == NULL)
			return;
		if(pDoc->m_F10HeadView == NULL)
			return;
		pDoc->m_F10HeadView->ShowF10();

		//if(pDoc->m_taiViewF10 == NULL)
		//	return;
		//pDoc->m_taiViewF10->GetParentFrame()->ActivateFrame();
		//pDoc->m_taiViewF10->SetFocus();
		return;
	}
	CWnd* pMainWnd=AfxGetApp()->m_pMainWnd;
	CFrameWnd * pActiveWnd=((CFrameWnd*)pMainWnd)->GetActiveFrame();
    CTaiShanDoc* pDoc=((CMainFrame *)pMainWnd)->m_pDoc;

	if( pActiveWnd ==NULL || pDoc == NULL)
	{
		AfxMessageBox("请选择股票！",MB_ICONSTOP);
		return;
	}
    if(pActiveWnd->IsKindOf(RUNTIME_CLASS(CNineShowFrame)))
	{
		m_pDoc->m_pNineShowView->SetStockCodeForDisplayF10();
	}
	//用新的文档模板创建新的框架(利用的是原来的文档类,新的视图类)
	CDocTemplate* pDocTemplate=((CWHApp*)AfxGetApp())->m_pBroadcastDocTemplate;
	CFrameWnd* pFrameWnd= pDocTemplate->CreateNewFrame(pDoc,NULL);
	pDocTemplate->InitialUpdateFrame(pFrameWnd,pDoc);


	//得到新的活动的窗口和活动的视图
	//pActiveWnd=MDIGetActive();
	//CView *pView=pActiveWnd->GetActiveView();	
	m_EnableF10 = false;
	((CChildFrame *)pFrameWnd)->CreateF10();

	return;  
}

void CMainFrame::OnViewGgxx() 
{
	ViewGgxx(-1);
}

void CMainFrame::ReViewInfo()
{
    m_EnableF10 = 2;
	OnCompanyinfo(); 
}

void CMainFrame::OnMenuFullscreen()
{
	//全屏幕显示的处理函数
    RECT rectDesktop;
    WINDOWPLACEMENT wpNew;

    if (m_bFullScreen)//全屏幕显示模式
	{
		//保存正常视图时的窗口位置信息以便恢复原来状态
		GetWindowPlacement (&m_wpPrev);
        m_wpPrev.length = sizeof m_wpPrev;

        //调整RECT为新的窗口尺寸
        DWORD winstyle,style1;
		winstyle = GetExStyle() | WS_EX_TOPMOST ;
		style1 = GetStyle() | WS_MAXIMIZE;   
		style1 = GetStyle() & ~WS_CAPTION;   
		::GetWindowRect ( ::GetDesktopWindow(), &rectDesktop );
		//rectDesktop.top = rectDesktop.top + 1;
		//rectDesktop.bottom = rectDesktop.bottom - 16;
		::AdjustWindowRectEx(&rectDesktop, style1,TRUE, winstyle);


		//保存RECT以便OnGetMinMaxInfo()使用
		m_FullScreenWindowRect = rectDesktop;

        wpNew = m_wpPrev;
        wpNew.showCmd = SW_SHOWNORMAL;
        wpNew.rcNormalPosition = rectDesktop;

        m_bFullScreen=TRUE;
    }
    else//正常显示模式
    {
        //删除全屏幕工具条
        ReleaseCapture();
        m_bFullScreen=FALSE;

        wpNew = m_wpPrev;
    }

    //设置窗口显示状态
    SetWindowPlacement ( &wpNew );

}

void CMainFrame::OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI)
{
	CRect rcMax,rcFull;
	::SystemParametersInfo(SPI_GETWORKAREA,0,rcMax,0);
	::GetWindowRect ( ::GetDesktopWindow(), &rcFull );
	if (m_bFullScreen == TRUE)
	{
		lpMMI->ptMaxSize.y = m_FullScreenWindowRect.Height();
		lpMMI->ptMaxTrackSize.y = lpMMI->ptMaxSize.y - 1;
		lpMMI->ptMaxSize.x = m_FullScreenWindowRect.Width();
		lpMMI->ptMaxTrackSize.x = lpMMI->ptMaxSize.x;
		CMDIFrameWnd::OnGetMinMaxInfo(lpMMI);
	}
   else
   {
	   lpMMI->ptMaxSize.y = rcMax.Height() + 5;
	   lpMMI->ptMaxSize.x = rcFull.Width() + 5;
	   CXTPMDIFrameWnd::OnGetMinMaxInfo(lpMMI);
   }
}

void CMainFrame::OnUpdateViewFullScreen(CCmdUI* pCmdUI)
{
	pCmdUI->Enable();
    pCmdUI->SetCheck(m_bFullScreen);
}

void CMainFrame::OnFullscreen() 
{
	// TODO: Add your command handler code here
	m_bFullScreen = !m_bFullScreen;
	OnMenuFullscreen();
	//ModifyStyle(WS_CAPTION,0,SWP_FRAMECHANGED );

}

void CMainFrame::OnNomalscreen() 
{
}

void CMainFrame::OnUpdateNomalscreen(CCmdUI* pCmdUI) 
{
   pCmdUI->Enable(m_bFullScreen);
}
/*
void CMainFrame::OnToolbar2() 
{
	// TODO: Add your command handler code here
	m_bFullScreen=FALSE;
	OnMenuFullscreen();
}
*/
/*
void CMainFrame::OnNcPaint() 
{
	//CMDIFrameWnd::OnNcPaint();
	//return;
	// Do not call CMDIFrameWnd::OnNcPaint() for painting messages
}
*/

void CMainFrame::OnToolGsgl() 
{ 
	//lmb11
#ifdef OEM
	return;
#endif
	CTaiKlineIndicatorsManage dlgManage;
	dlgManage.DoModal ();
	RefreshFormu();
}

void CMainFrame::OnToolSjgl() 
{
	CSuperviseData ManageData(_T("数据管理"));
	CImportData       ImportData;   //数据导入
	CExportData       ExportData;	//数据导出
	CClearData        ClearData;	//数据整理
	CCaiwuData        CaiwuData;
	CChuquanData      ChuquanData;
	CSymbolContrast   SymbolContrast;//代码对照
	CFxjCloseWork   dlgCloseW;//代码对照


	ManageData.AddPage(&ImportData);
	ManageData.AddPage(&ExportData);
	ManageData.AddPage(&ClearData);
	ManageData.AddPage(&CaiwuData);
	ManageData.AddPage(&ChuquanData);
	ManageData.AddPage(&SymbolContrast);
	ManageData.AddPage(&dlgCloseW);

	ManageData.m_psh.dwFlags|=PSH_NOAPPLYNOW ;
	if(ManageData.DoModal())
		CTaiShanKlineShowView::OnDataChangAll(200);

}

void CMainFrame::OnToolRiskIndex()
{
	//AfxMessageBox("此功能仅限于更高级版本！");
//	CRiskIndex RiskIndex;
//	RiskIndex.DoModal();
}


void CMainFrame::DisplayBargain(/*CString szStockName,CString szReason,int Amount,float Price,*/
	float Index_sh,float UpDown_sh,float Vol_sh,float Amount_sh,
	float Index_sz,float UpDown_sz,float Vol_sz,float Amount_sz,
	float Index_cy,float UpDown_cy,float Vol_cy,float Amount_cy,
	float percent_sh, float percent_sz ,float percent_cy )
	//float m_fVol_sh,float m_fVol_sz)
{   
	m_wndStatusBar.SetIndex_sh(Index_sh,FALSE);
	m_wndStatusBar.SetIndex_sz(Index_sz,FALSE);
	m_wndStatusBar.SetIndex_cy(Index_cy,FALSE);
	m_wndStatusBar.SetAD_sh(UpDown_sh,FALSE);
    m_wndStatusBar.SetAD_sz(UpDown_sz,FALSE);
	m_wndStatusBar.SetAD_cy(UpDown_cy,FALSE);
	m_wndStatusBar.SetVol_sh(Vol_sh,FALSE);
	m_wndStatusBar.SetVol_sz(Vol_sz,FALSE);
	m_wndStatusBar.SetVol_cy(Vol_cy,FALSE);
	m_wndStatusBar.SetAmount_sh(Amount_sh,FALSE);
	m_wndStatusBar.SetAmount_sz(Amount_sz,FALSE);
	m_wndStatusBar.SetAmount_cy(Amount_cy,FALSE);
	m_wndStatusBar.SetRedGreen_sh(percent_sh);
	m_wndStatusBar.SetRedGreen_cy(percent_cy);
    m_wndStatusBar.SetRedGreen_sz(percent_sz);
}


void CMainFrame::OnTjxgTjxg() 
{
	ShowTjxg();/*
	if(m_pWndTJXG!=NULL)
		m_pWndTJXG->ShowWindow (SW_RESTORE);
	else
	{
	m_pWndTJXG = new CTaiScreen(this);
	m_pWndTJXG->Create(IDD_TJXGNEW,this);
	m_pWndTJXG->ShowWindow(SW_SHOW);
	}*/

}

void CMainFrame::OnViewXx() 
{
	// TODO: Add your command handler code here
	COPTION Option(_T("属性设置"));
	Option.AddNewPage();
	CTaiShanDoc *pDoc = this->m_pDoc ;
	Option.m_psh.dwFlags|=PSH_NOAPPLYNOW ;
	int li=Option.DoModal();
	if(li==1)
	{
		int i;
		for(i=0;i<systemcolorlength;i++)
			pDoc->m_colorArray[i]=Option.m_syswg.cr[i];
		for(i=0;i<FONTSTRUCTLEN;i++)
			pDoc->m_fontstr[i]=Option.m_syswg.fr[i];
	//----------------更新文档中系统数据项和系统选择项的数据
	pDoc->m_systemOption.autoclose=Option.m_syssetup.m_autoclose;
	pDoc->m_systemOption.autominute=Option.m_syssetup.m_autominute;
	pDoc->m_systemOption.autoday=Option.m_syssetup.m_autoday;
	pDoc->m_systemOption.showxline=Option.m_syssetup.m_showxline;
	pDoc->m_systemOption.showyline=Option.m_syssetup.m_showyline;
	pDoc->m_systemOption.alarmsound=Option.m_syssetup.m_AlarmSound;
	pDoc->m_systemOption.organizedata=Option.m_syssetup.m_AutoOrganizeData;
	pDoc->m_propertyInitiate.bDoPowerSelectingStock=Option.m_syssetup.m_tjxgpower;
	pDoc->m_propertyInitiate.bDoVolPowering=Option.m_syssetup.m_volpower;
	pDoc->m_propertyInitiate.bDoPowerAfter=Option.m_syssetup.m_backpower;
	pDoc->m_propertyInitiate.bSaveParam=Option.m_syssetup.m_autosavezb;
	pDoc->m_propertyInitiate.bShowCrossDlg=Option.m_syssetup.m_showinfohq;
	pDoc->m_propertyInitiate.bRunAlarmBegin=Option.m_syssetup.m_startalert;
	pDoc->m_propertyInitiate.bUseMin5ChenBen=Option.m_syssetup.m_cjmxyester;
	pDoc->m_systemOption.kline=Option.m_syssetup.m_kline;
	pDoc->m_systemOption.fourgraph=Option.m_syssetup.m_fourgraph;

	pDoc->m_propertyInitiate.daysOfManyKline=Option.m_syspara.m_nDaysOfManyKline;
	pDoc->m_propertyInitiate.daysOfDayKline=Option.m_syspara.m_nDaysOfDayKline;
	pDoc->m_propertyInitiate.countOfHistoryFile=Option.m_syspara.m_nCountOfHistoryFile;
    pDoc->m_propertyInitiate.daysOfMin5Kline=Option.m_syspara.m_nDaysOfMin5Kline;
    pDoc->m_systemdata.noticesave=Option.m_syspara.m_noticesave;
	//begin(add by zhufeng)
	pDoc->m_propertyInitiate.LTP_rate=Option.m_syspara.m_ltp_rateV;
	pDoc->m_propertyInitiate.CJL_rate=Option.m_syspara.m_cjl_rateV;
	//end
    pDoc->m_syshqset.capital=Option.m_syspara.m_nCapitalTime;
	pDoc->m_syshqset.quangtity=Option.m_syspara.m_nQuantityTime;
	pDoc->m_syshqset.swing=Option.m_syspara.m_nSwingTime;
	pDoc->m_syshqset.upspeed=Option.m_syspara.m_nUpSpeedTime;
	pDoc->m_syshqset.changehand=Option.m_syspara.m_nChangeHandTime;

    pDoc->m_propertyInitiate.nTestUnfinished=Option.m_syspara.m_nNoFinishedTest;
	pDoc->m_systemdata.middleprice=Option.m_syspara.m_nMidWay;

    pDoc->m_propertyInitiate.nSecondsAlarm=Option.m_syspara.m_nTimespanAlert;
    pDoc->m_systemdata.changepage=Option.m_syspara.m_nChangPageTime;

	pDoc->m_dadanchoice.Vol=Option.m_syspara.m_nVol;
	pDoc->m_dadanchoice.Per=Option.m_syspara.m_nPer;
	pDoc->m_dadanchoice.Acount=Option.m_syspara.m_nAcount;
	pDoc->m_dadanchoice.Format=Option.m_syspara.m_nFormat;
	pDoc->m_SystemInitData.RealTime=Option.m_syspara.m_nRealTime;
	pDoc->m_SystemInitData.BlockTime=Option.m_syspara.m_nBlockTime;
    //if(Option.m_syspara.m_nAddress.Mid(0,7)=="http://"||Option.m_syspara.m_nAddress.Mid(0,7)=="HTTP://")
	strcpy(pDoc->m_SystemInitData.NetAddr,Option.m_syspara.m_nAddress.GetBuffer(0));
    /*else
	{
		CString Addr;
		Addr="http://"+Option.m_syspara.m_nAddress;
		strcpy(pDoc->m_SystemInitData.NetAddr,Addr.GetBuffer(0));
		Addr.ReleaseBuffer();
	}*/
	strcpy(pDoc->m_SystemInitData.KswtAddr,Option.m_syspara.m_nKswtAddr.GetBuffer(0));
	CTaiShanKlineShowView::OnDataChangAll(200);
	::SendMessage(pDoc->m_pReportView->m_hWnd,WM_USER_CHANGECOLOR,0,0);

	if(pDoc->m_pStockCjMxTj)
	   ::SendMessage(pDoc->m_pStockCjMxTj->m_hWnd,WM_USER_CHANGECOLOR,0,0);
	
	if(pDoc->m_pNineShowView)
	  ::SendMessage(pDoc->m_pNineShowView->m_hWnd,WM_USER_CHANGECOLOR,0,0);
	//写文件 add by lj 2002/12/18
	/*CFile SysFile;
	SysFile.Open((LPCTSTR)"系统属性.DAT",CFile::modeCreate|CFile::modeWrite,NULL);
	int FileID=65798813;
	SysFile.Write(&FileID,sizeof(int));
	SysFile.Seek(16,CFile::begin);
    SysFile.Write(&pDoc->m_colorArrayDefault[0],sizeof(COLORREF)*6*24);
	SysFile.Write(&pDoc->m_colorArray,sizeof(COLORREF)*34);
	SysFile.Write(&pDoc->m_fontdefault,sizeof(LOGFONT));
	SysFile.Seek(sizeof(LOGFONT),CFile::current);//
	SysFile.Write(&pDoc->m_fontstr,sizeof(LOGFONT)*FONTSTRUCTLEN);
	SysFile.Write(&pDoc->m_propertyInitiate,sizeof(PROPERTY_INIT));
    SysFile.Seek(40,CFile::current);//
	SysFile.Write(&pDoc->m_syshqset,sizeof(SYSTEMHQSET));
	SysFile.Seek(40,CFile::current);//
	SysFile.Write(&pDoc->m_systemdata,sizeof(SYSTEMDATA));
	SysFile.Seek(40,CFile::current);//
	SysFile.Write(&pDoc->m_systemOption,sizeof(SYSTEM_OPTION));
	SysFile.Seek(40,CFile::current);//
	SysFile.Write(&pDoc->m_dadanchoice,sizeof(ALERTCHOICE));

	SysFile.Close();*/
	}
}

void CMainFrame::OnStopCuohe() 
{
	// TODO: Add your command handler code here
    if(m_StopCuoHe==TRUE)
	  m_StopCuoHe=FALSE;
	else
	  m_StopCuoHe=TRUE;
}

void CMainFrame::OnToolTzgl() 
{

	CInvestLogin InvestLogin;
	InvestLogin.DoModal();

}

void CMainFrame::OnUpdateFullscreen(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(m_bFullScreen == FALSE?FALSE:TRUE);	
}

void CMainFrame::OnOpenBigBill() 
{
#ifdef OEM
	CDialogShowInformation dlg;
	this->m_pDoc ->m_CurrentWorkDirectory.TrimRight ("\\");
	m_pDoc ->m_CurrentWorkDirectory.TrimRight ("/");
	m_pDoc->m_CurrentWorkDirectory+="\\";
	CString s = m_pDoc->m_CurrentWorkDirectory+"html\\大单统计.htm";
	dlg.m_sHtml = s;//
	dlg.DoModal ();
	return;
#endif
	if(m_pDoc->m_pStockCjMxTj==NULL)
	{
		CSharesBigBillDlg *m_pDlg=new CSharesBigBillDlg;
		m_pDlg->Create(IDD_DIALOG6,NULL);
		m_pDlg->ShowWindow(SW_SHOW);
	}
	else
	{
        m_pDoc->m_pStockCjMxTj->ShowWindow(SW_RESTORE);
	}
}


void CMainFrame::OnF3() 
{
 
   CMDIChildWnd* pchildfram= ((CMainFrame*)(AfxGetApp()->m_pMainWnd))->MDIGetActive();
   CTaiShanDoc*	pDoc=this->m_pDoc ;//(CTaiShanDoc*)pchildfram->GetActiveDocument();
   if(pDoc == NULL)
	  return;//pDoc=(CTaiShanDoc*)(((CChildFrame *)pchildfram)->m_CurrentDoc);
   CView *pView = pchildfram->GetActiveView();
   if(pView == NULL)
   {
     ::SendMessage(pDoc->m_pReportView->m_hWnd,WM_USER_FROM_KLINE,0,5);
     return;
   }
   if(pView->IsKindOf(RUNTIME_CLASS(CNineShowView)))
   {
     ((CNineShowView *)pView)->IsQuit = 1;
   }
   ::SendMessage(pDoc->m_pReportView->m_hWnd,WM_USER_FROM_KLINE,0,5);
}

void CMainFrame::OnF4() 
{
	OnF3() ;
}

void CMainFrame::OnClose() 
{	
	if(AfxMessageBox("是否要退出系统？",MB_YESNO|MB_ICONQUESTION) == IDNO)
		return;
    HqStock_Quit();
	CTaiShanDoc *pDoc = this->m_pDoc ; 
	POSITION pos = pDoc->GetFirstViewPosition();
	CView *pView;
	pDoc->m_pReportView = NULL;
	while(pos != NULL)
	{
		pView = pDoc->GetNextView(pos);
		if(pView!=NULL)
			pView->DestroyWindow ();
	}
	CMDIFrameWnd::OnClose();
}

void CMainFrame::OnHelpTopic() 
{
//	HtmlHelp(m_hWnd,"ybw.chm",HH_HELP_CONTEXT,6000);	
//---------- Begin --- 2002.11.25 lm Modify -----------------------
//	::ShellExecute( m_hWnd, "open", "ybw.chm", NULL, NULL, SW_SHOWNORMAL);
	DoHtmlHelp(this);
//---------- End --- 2002.11.25 lm Modify -------------------------
}

void CMainFrame::OnUpdateToolGsgl(CCmdUI* pCmdUI) 
{

#ifndef OEM
	pCmdUI->Enable(TRUE);
#else
    pCmdUI->Enable(FALSE);
#endif

}


void CMainFrame::OnUpdateBaseinfoGrid(CCmdUI* pCmdUI) 
{
#ifdef OEM
	pCmdUI->Enable(FALSE);
#else
    pCmdUI->Enable(TRUE);
#endif	
}

void CMainFrame::OnUpdateCuoheOpen(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
#ifdef OEM
	pCmdUI->Enable(FALSE);
    return;    
#endif	
}

void CMainFrame::OnGrid() 
{
	this->m_pDoc ->m_pReportView->GetParentFrame()->SendMessage(WM_SYSCOMMAND,SC_MAXIMIZE,0);
/*	CWnd * pMainWnd = AfxGetApp()->m_pMainWnd;
	CFrameWnd* pFrameWnd=((CFrameWnd*)pMainWnd)->GetActiveFrame();
	CTaiShanDoc*	pDoc;
	pDoc =(CTaiShanDoc*)pFrameWnd->GetActiveDocument();
	if(pDoc == NULL)
	  pDoc=(CTaiShanDoc*)(((CChildFrame *)pFrameWnd)->m_CurrentDoc);	
	CView *pView = pFrameWnd->GetActiveFrame()->GetActiveView();
	if(pView == NULL)
	{
		pFrameWnd->GetActiveFrame()->SendMessage(WM_CLOSE);
	    pDoc->m_pReportView->GetParentFrame()->ActivateFrame();
	    pDoc->m_pReportView->GetParentFrame()->SendMessage(WM_SYSCOMMAND,SC_MAXIMIZE,0);
		return;
	}

	if(!pView->IsKindOf(RUNTIME_CLASS(CTaiShanKlineShowView)))
	{
		pFrameWnd->GetActiveFrame()->SendMessage(WM_CLOSE);
	}

	pDoc->m_pReportView->GetParentFrame()->ActivateFrame();
	*/
}

void CMainFrame::OnUpdateGrid(CCmdUI* pCmdUI) 
{
  CView *pView = this->GetActiveFrame()->GetActiveView();
  if(pView == NULL)
  {
   pCmdUI->Enable(TRUE);
   return;
  }
  if(pView->IsKindOf(RUNTIME_CLASS(CTaiShanReportView)))
  {
    pCmdUI->Enable(FALSE);  
  }
}

void CMainFrame::OnUpdateF3(CCmdUI* pCmdUI) 
{
}

void CMainFrame::OnUpdateF4(CCmdUI* pCmdUI) 
{
}

void CMainFrame::OnWindowNew() 
{
	// TODO: Add your command handler code here
	CMDIChildWnd* pFrameWnd = MDIGetActive();
	CTaiShanDoc* pDoc=this->m_pDoc;

	CDocTemplate* pTemplate= ((CWHApp*)AfxGetApp())->pDrawTemplate;
	
	//////////////////////////////////////////////////// 
	CFrameWnd* pFrame = pTemplate->CreateNewFrame(pDoc,pFrameWnd);
	pTemplate->InitialUpdateFrame(pFrame,pDoc);
			
	pFrameWnd =(CMDIChildWnd*)GetActiveFrame();
	
}

void CMainFrame::OnUpdateWindowNew(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	CTaiShanDoc* pDoc=this->m_pDoc;
	if(pDoc->m_arKLineViews .GetSize()>=6)
		pCmdUI->Enable(FALSE);
	
}

LRESULT CMainFrame::DrawKlineWindow(WPARAM wParam, LPARAM lParam)
{
	char *symbol = (char *)wParam;
	int nKind=(int )lParam/10;
	CTaiShanDoc* pDoc = this->m_pDoc;
	if(pDoc->m_arKLineViews.GetSize() == 0)
	{ 
		CFrameWnd* pFrameWnd=GetActiveFrame();
        pDoc->m_GetFocus = true;

		CDocTemplate* pTemplate= ((CWHApp*)AfxGetApp())->pDrawTemplate;
		
		/////////////////////Set the stock code/////////////
		pDoc->m_sharesSymbol = symbol;
		pDoc->m_stkKind=nKind;

		if(lParam%10 == 1) 
			((CWHApp*)AfxGetApp())->Line_mode = 0;
		else if(lParam%10 == 2) 
			((CWHApp*)AfxGetApp())->Line_mode = 5;

		//////////////////////////////////////////////////// 
		CFrameWnd* pFrame = pTemplate->CreateNewFrame(pDoc,pFrameWnd);
		pTemplate->InitialUpdateFrame(pFrame,pDoc);
//		pFrameWnd = GetActiveFrame();
//		CView *pView=pFrameWnd->GetActiveView();
//		pView->SetFocus();
		return 1L;
	}
    else
	{
		CTaiShanKlineShowView *pView = pDoc->m_arKLineViews[0];
		pView->m_sharesSymbol = symbol;
		pView->m_stkKind=nKind;
		pDoc->m_GetFocus = true;
		pView->m_GetFocus = true;
		if(lParam%10 == 1) 
			pView->m_nKlineKind2  = 0;
		else if(lParam%10 == 2) 
			pView->m_nKlineKind2  = 5;
		pView->ShowAll(symbol);
//		pView->GetParentFrame()->BringWindowToTop ();
		ActiveVwbase(pView);
//		pView->GetParentFrame()->ActivateFrame();
//		pView->SetFocus();
		return 0L;
	}

}

void CMainFrame::OnTjxgAlarm() 
{
#ifndef OEM
	CAlertSystem::CreateNew();
#endif
}

void CMainFrame::OnToolBlockMana() 
{
	// TODO: Add your command handler code here
    CSuperviseSharesBlock l_Dlg;
	l_Dlg.DoModal();
}

void CMainFrame::OnUpdateBlockDayLine() 
{
	//更新板块的日线数据
	CStringArray BlockCodeArray;
	m_pDoc->m_ManagerBlockData.GetAllBlockCodes(BlockCodeArray);
	for (int i = 0;i < BlockCodeArray.GetCount();i++)
	{
		m_pDoc->m_ManagerBlockData.RefreshHistoryDayLineData((LPTSTR)(LPCTSTR)BlockCodeArray[i]);
		ProcMessage();
	}
}

void CMainFrame::OnUpdateBlockMin5Line() 
{
	//更新板块的5分钟线数据
	CStringArray BlockCodeArray;
	m_pDoc->m_ManagerBlockData.GetAllBlockCodes(BlockCodeArray);
	for (int i = 0;i < BlockCodeArray.GetCount();i++)
	{
		m_pDoc->m_ManagerBlockData.RefreshHistoryMin5LineData((LPTSTR)(LPCTSTR)BlockCodeArray[i]);
		ProcMessage();
	}
}

void CMainFrame::OnDownload20() 
{
	if(m_pDoc->m_pDownLoadDlg==NULL)
	{
#ifdef WIDE_NET_VERSION
		m_pDoc->m_pDownLoadDlg=new CDownLoadNetDlg;
		m_pDoc->m_pDownLoadDlg->Create(IDD_WIDEDOWNLOAD,NULL);
#else
		m_pDoc->m_pDownLoadDlg=new CDownLoadDlg;
		m_pDoc->m_pDownLoadDlg->Create(IDD_DOWNLOAD_DIALOG,NULL);
#endif
		m_pDoc->m_pDownLoadDlg->ShowWindow(SW_SHOW);
	}
	else
	{
        m_pDoc->m_pDownLoadDlg->ShowWindow(SW_RESTORE);
	}
}

void CMainFrame::OnCloseReceiver() 
{
	// TODO: Add your command handler code here
#ifndef WIDE_NET_VERSION
	if(!m_pDoc->m_bCloseReceiver)
	{
      HqStock_Init();
	}
	else
	{
      HqStock_Quit();
	}
#endif
}

void CMainFrame::OnUpdateCloseReceiver(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
//	pCmdUI->SetCheck(!m_pDoc->m_bCloseReceiver); 
	if(m_pDoc->m_bCloseReceiver)
	{
		pCmdUI->SetText("关闭行情接收系统"); 
	}
	else
	{
		pCmdUI->SetText("开启行情接收系统"); 
	}
}

void CMainFrame::OnEnterInternet() 
{
	// TODO: Add your command handler code here
	ShellExecute(NULL,"open",(CMainFrame::m_pDoc)->m_SystemInitData.NetAddr,NULL,NULL,SW_SHOWNORMAL);
	
}



void CMainFrame::OnTjxgMmTest() 
{
	// TODO: Add your command handler code here
	if(m_pWndTJXGTest!=NULL)
		m_pWndTJXGTest->ShowWindow(SW_RESTORE);
	else
	{
		m_pWndTJXGTest= new CTaiScreenTest;
		m_pWndTJXGTest->Create(IDD_6_TJXG_TEST);
		m_pWndTJXGTest->ShowWindow(SW_SHOW);
	}
}

void CMainFrame::OnToolNnetTrain() 
{
//	AfxMessageBox("此功能仅限于更高级版本！");

	if(m_pWndNL == NULL)
	{
		m_pWndNL = new CTaiKlineDlgNeuralLearn;
		m_pWndNL->Create (CTaiKlineDlgNeuralLearn::IDD,this);
		m_pWndNL->ShowWindow (SW_SHOW);
	}
	else
		m_pWndNL->ShowWindow (SW_RESTORE);

}


BOOL CMainFrame::OnCommand(WPARAM wParam, LPARAM lParam) 
{
	return CMDIFrameWnd::OnCommand(wParam, lParam);
}


//自动委托
void CMainFrame::OnToolAutoSubmit() 
{
	CString s = (CMainFrame::m_pDoc)->m_SystemInitData.KswtAddr;
	if(s == "")
	{
		AfxMessageBox("没有设置自动委托程序的位置！");
		return ;
	}
	ShellExecute(NULL,"open",(CMainFrame::m_pDoc)->m_SystemInitData.KswtAddr,NULL,NULL,SW_SHOWNORMAL);
}

void CMainFrame::OnMin1Line() 
{
	// TODO: Add your command handler code here
	CString stock_code,stock_name;
	CTaiShanDoc *pDoc = this->m_pDoc;		
	if(pDoc == NULL)
	  return;	
	CFrameWnd* pFrameWnd=(CFrameWnd*)GetActiveFrame();
	CView *pView = pFrameWnd->GetActiveView();
	if(pView == NULL)
	{
		return;
	}
	if(!pView->IsKindOf(RUNTIME_CLASS(CTaiShanKlineShowView)))
	{
		if(pView->IsKindOf(RUNTIME_CLASS(CNineShowView)))
			stock_code = pDoc->m_pNineShowView  ->GetStockCodeForDisplayKLine();
		else
			stock_code = pDoc->m_pReportView ->GetCurrentSymbol();
		if(stock_code.Compare("") == 0)
		{
		   MessageBox("请选择某一股票进入","操作提示",MB_OK);
		   return ;
		}
	}
	else
	{
		CTaiShanKlineShowView* pView2 = (CTaiShanKlineShowView*)pView;
		pView2->m_nKlineKind2 = 0;
		if(pView2->m_sharesSymbol=="") return;
		pView2->ShowAll (pView2->m_sharesSymbol ,false);
		return;
	}
	
	pDoc->m_sharesSymbol = stock_code;
	pDoc->m_GetFocus = true;
	SendMessage (WM_USER_DrawKlineWindow,(WPARAM)stock_code.GetBuffer (0),(LPARAM)(1+pDoc->m_stkKind *10));
}
void CMainFrame::OnDayLine() 
{
	// TODO: Add your command handler code here
	CString stock_code,stock_name;
	CTaiShanDoc *pDoc = this->m_pDoc;		
	if(pDoc == NULL)
	  return;	
	CFrameWnd* pFrameWnd=(CFrameWnd*)GetActiveFrame();
	CView *pView = pFrameWnd->GetActiveView();
	if(pView == NULL)
	{
		return;
	}
	if(!pView->IsKindOf(RUNTIME_CLASS(CTaiShanKlineShowView)))
	{
		if(pView->IsKindOf(RUNTIME_CLASS(CNineShowView)))
			stock_code = pDoc->m_pNineShowView  ->GetStockCodeForDisplayKLine();
		else
			stock_code = pDoc->m_pReportView ->GetCurrentSymbol();
		if(stock_code.Compare("") == 0)
		{
		   MessageBox("请选择某一股票进入K线","操作提示",MB_OK);
		   return ;
		}
	}
	else
	{
		CTaiShanKlineShowView* pView2 = (CTaiShanKlineShowView*)pView;
		pView2->m_nKlineKind2 = 5;
		if(pView2->m_sharesSymbol=="") return;
		pView2->ShowAll (pView2->m_sharesSymbol ,false);
		return;
	}
	
	pDoc->m_sharesSymbol = stock_code;
	pDoc->m_GetFocus = true;
	SendMessage (WM_USER_DrawKlineWindow,(WPARAM)stock_code.GetBuffer (0),(LPARAM)(2+pDoc->m_stkKind *10));
	
}

int CMainFrame::StaticDrawKlineWindow(WPARAM wParam, LPARAM lParam)
{
	return 0;
}

void CMainFrame::ActiveVwbase(CTaiShanKlineShowView *pView)
{
	__try
	{
		pView->GetParentFrame()->ActivateFrame();
	}
	__except(1)
	{
	}

}

void CMainFrame::OnUpdateStockBig(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
#ifdef OEM
	pCmdUI->Enable(FALSE);
#else
    pCmdUI->Enable(TRUE);
#endif	
	
}

void CMainFrame::OnUpdateTjxgAlarm(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
#ifdef OEM
	pCmdUI->Enable(FALSE);
#else
    pCmdUI->Enable(TRUE);
#endif	
	
}

void CMainFrame::OnF6()		//切换到动态显示牌的自选股页面
{
   CMDIChildWnd* pchildfram= ((CMainFrame*)(AfxGetApp()->m_pMainWnd))->MDIGetActive();
   CTaiShanDoc*	pDoc=this->m_pDoc ;//(CTaiShanDoc*)pchildfram->GetActiveDocument();
   if(pDoc == NULL)
	  return;

   CView *pView = pchildfram->GetActiveView();
   if(pView == NULL)
   {
	(pDoc->m_pReportView)->OnF6();
     return;
   }
   if(pView->IsKindOf(RUNTIME_CLASS(CNineShowView)))
   {
     ((CNineShowView *)pView)->IsQuit = 1;
   }
   (pDoc->m_pReportView)->OnF6();
}

LRESULT CMainFrame::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class
	if(message == CWHApp::m_gMessageID && m_bRunFlag == TRUE)
	{
		if(m_gbDestroyed == true)
			return CMDIFrameWnd::WindowProc(message, wParam, lParam);

		m_memFile.SeekToBegin();
		int nWparam = wParam;
		int nLparam;
		m_memFile.Read(&nWparam,4);
		m_memFile.Read(&nLparam,4);
		if((int)nLparam<MaxShareMem)
		{
			m_memFile.SeekToBegin();
			BYTE * byt = m_memFile.GetFileBeginPointer();
			byt+=8;

			RCV_DATA *	pHeader = (RCV_DATA *)byt;
			pHeader->m_pData = byt+sizeof(RCV_DATA);

			OnStkDataOK((WPARAM)nWparam,(LPARAM)(byt));
		}
	};
	return CMDIFrameWnd::WindowProc(message, wParam, lParam);
}

void CMainFrame::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
#ifdef WIDE_NET_VERSION
	if(!m_bLogonInterface)
	{
		m_bLogonInterface=TRUE;
		CLogonDlg dlg;
//		dlg.m_pMainFrame=this;
		dlg.DoModal();
		m_bLogonSuccess=dlg.m_bLogonSuccess;
	}
#endif
	// Do not call CMDIFrameWnd::OnPaint() for painting messages
}

void CMainFrame::TestHQ()
{
	int nSymbol;
	static float rate=0;
	nSymbol=1;
	RCV_DATA *	pHeader=new RCV_DATA;
	RCV_REPORT_STRUCTEx *pReport=new RCV_REPORT_STRUCTEx[100];
	for(int i=0;i<9;i++)
	{
		char strSymbol[8];
		char strName[10];
		switch(i)
		{
		case 0:
			strcpy(strName,"深发展");
			break;
		case 1:
			strcpy(strName,"蓝点科技");
			break;
		case 2:
			strcpy(strName,"汇天奇");
			break;
		case 3:
			strcpy(strName,"厦门远华");
			break;
		case 4:
			strcpy(strName,"广东发展");
			break;
		case 5:
			strcpy(strName,"广州本田");
			break;
		case 6:
			strcmp(strName,"光大银行");
			break;
		case 7:
			strcpy(strName,"中远集团");
			break;
		case 8:
			strcpy(strName,"APEC");
			break;
		default :return;
			
		}
		CString a;
		a.Format("000%d",nSymbol);
		strcpy(strSymbol,a);
		//CString str=strSymbol.Right(6);
		strcpy(pReport[i].m_szLabel,strSymbol);
		strcpy(pReport[i].m_szName,strName);
		pReport[i].m_wMarket=SZ_MARKET_EX;
		pReport[i].m_fAmount=1000+i;
		pReport[i].m_fBuyPrice[0]=10.00+i;
		pReport[i].m_fBuyPrice[1]=10.02+i;
		pReport[i].m_fBuyPrice[2]=10.05+i;
		pReport[i].m_fBuyVolume[0]=678+i;
		pReport[i].m_fBuyVolume[1]=678+i;
		pReport[i].m_fBuyVolume[2]=678+i;
		pReport[i].m_fHigh=10.12+i;
		pReport[i].m_fLastClose=9.87+i;
		pReport[i].m_fLow=9.95+i;
		pReport[i].m_fNewPrice=10.10+i;
		pReport[i].m_fOpen=9.97+i;
		pReport[i].m_fSellPrice[0]=10.10+i;
		pReport[i].m_fSellPrice[1]=10.12+i;
		pReport[i].m_fSellPrice[2]=10.14+i;
		pReport[i].m_fSellVolume[0]=123+i;
		pReport[i].m_fSellVolume[1]=123+i;
		pReport[i].m_fSellVolume[2]=123+i;
		pReport[i].m_fVolume=988+i;
		pReport[i].m_cbSize=100;
		//pReport[i].m_time=10201112;
		nSymbol++;
	}	
	pHeader->m_nPacketNum=10;
	pHeader->m_pReport=pReport;
	SendMessage(Gp_Msg_StkData,(UINT)RCV_REPORT,(LPARAM)pHeader);
	delete pHeader;
	delete []pReport;
}
//added by qyp 2002.10.23
#ifdef WIDE_NET_VERSION
void CMainFrame::OnLoginServer() 
{
	// TODO: Add your command handler code here
	CLogonDlg dlg;
	dlg.DoModal();
	m_bLogonSuccess=(m_bLogonSuccess || dlg.m_bLogonSuccess);
}

void CMainFrame::OnRecvStockcode() 
{
	// TODO: Add your command handler code here
	CRecvStockCode dlg(this);
	dlg.DoModal();
}
#endif


void CMainFrame::start_stockdll()
{
//	SetTimer(INI_DLL,3000,NULL);
}

void CMainFrame::OnSortIndex() 
{
	// TODO: Add your command handler code here
	this->m_pDoc ->m_pReportView ->IndexSortLi ();
}

void CMainFrame::OnXgnExtData() 
{
	XgnExtDataDlg dlg;
	dlg.DoModal();
	
}

void CMainFrame::OnClearRealdata() 
{
	this->m_pDoc->ClearRealData();	
}

CTaiScreen* CMainFrame::ShowTjxg()
{
	if(m_pWndTJXG!=NULL)
		m_pWndTJXG->ShowWindow (SW_RESTORE);
	else
	{
		m_pWndTJXG = new CTaiScreen(this);
		m_pWndTJXG->Create(IDD_TJXGNEW,this);
		m_pWndTJXG->ShowWindow(SW_SHOW);
	}
	return m_pWndTJXG;

}

//-----------------------------------------------
//	2002.11.21 lm Added
//	获取当前选择的股票代码和市场类型
CString CMainFrame::GetCurrentSelectStockCode(WORD &wMarket)
{
	CString stock_code,stock_name;
	CTaiShanDoc *pDoc = this->m_pDoc;		
	if(pDoc == NULL)
	  return stock_code;
	CFrameWnd* pFrameWnd=(CFrameWnd*)GetActiveFrame();
	CView *pView = pFrameWnd->GetActiveView();
	if(pView == NULL)
		return stock_code;

	int nKind;
	if(!pView->IsKindOf(RUNTIME_CLASS(CTaiShanKlineShowView)))
	{
		if(pView->IsKindOf(RUNTIME_CLASS(CNineShowView)))
			stock_code = pDoc->m_pNineShowView->GetStockCodeForDisplayKLine();
		else
			stock_code = pDoc->m_pReportView->GetCurrentSymbol();
		nKind = pDoc->m_stkKind;
	}
	else
	{
		CTaiShanKlineShowView* pView2 = (CTaiShanKlineShowView*)pView;
		stock_code = pView2->m_sharesSymbol;
		nKind = pView2->m_stkKind;
	}
	switch( nKind )
	{
	case SHZS:
	case SHAG:
	case SHBG:
	case SHZQ:
	case SHJJ:
	case SHZZ:
	case SHHG:
	case SHETF:
	case SHKF:
	case SHQZ:
	case SHOT:
		wMarket = SH_MARKET_EX;
		break;
	default:
		wMarket = SZ_MARKET_EX;
	}
	
	return stock_code;
}

void CMainFrame::OnLButtonDown(UINT nFlags, CPoint point) 
{
	CMDIFrameWnd::OnLButtonDown(nFlags, point);
}

void CMainFrame::OnNcLButtonDown(UINT nHitTest, CPoint point) 
{
	//CMDIFrameWnd::OnNcLButtonDown(nHitTest, point);
	OnNcLButtonUp(nHitTest, point) ;
}

int CMainFrame::OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message) 
{
	// TODO: Add your message handler code here and/or call default
	
		CPoint point;
		GetCursorPos(&point);
		ScreenToClient(&point);
		CView *pView;
		if(point.x<=1)
		{
			 //pView = GetActiveView();
			 //if(pView != NULL && pView->IsKindOf(RUNTIME_CLASS(CPreviewView)))
				// return ;
			 m_dlgLeftPop.ShowWindow(SW_SHOW);
			 //m_TestDialogbar.ShowWindow(SW_HIDE);
		}
	return CMDIFrameWnd::OnMouseActivate(pDesktopWnd, nHitTest, message);
}

void CMainFrame::RefreshFormu()
{
	if(m_pWndTJXGTest!=NULL)
		m_pWndTJXGTest->RefreshJishuTreeCtrl();
	if(m_pWndTJXG!=NULL)
		m_pWndTJXG->RefreshJishuTreeCtrl();
	m_dlgLeftPop.RefreshJishuTreeCtrl();

}
long CMainFrame::OnExtService(UINT wFileType, LONG lPara)
{
    STRUCT_REXT_HEAD *pHead = (STRUCT_REXT_HEAD*)lPara ;
    ASSERT(wFileType == pHead->m_Service);
    CString msg ;
    if ( wFileType == RCV_EXT_TRADEDATA)
    {
        msg.Format("%.6s %.8s %d 分笔" , pHead->m_Code, pHead->m_Name, pHead->m_num);
        STCP_QBD_STRUCT *pk = (STCP_QBD_STRUCT *) (lPara + sizeof(STRUCT_REXT_HEAD));

    }
	if ( wFileType == RCV_EXT_TRADEDATAV2){
        msg.Format("新%.6s %.8s %d 分笔" , pHead->m_Code, pHead->m_Name, pHead->m_num);
        STCP_QBD6_STRUCT *pk = (STCP_QBD6_STRUCT *) (lPara + sizeof(STRUCT_REXT_HEAD));
    }
	if ( wFileType == RCV_EXT_KDATA)
    {
        msg.Format("%.6s %.8s %d K线" , pHead->m_Code, pHead->m_Name, pHead->m_num);
        RITK_STRUCT *pk = (RITK_STRUCT *) (lPara + sizeof(STRUCT_REXT_HEAD));
    }
	if (wFileType == RCV_EXT_STOCKTABLE)
    {
        msg.Format("股票代码表" );
        RCV_STOCK_CELLEx *pk = (RCV_STOCK_CELLEx *) (lPara + sizeof(STRUCT_REXT_HEAD));
    }
	return 0;
}

void CMainFrame::OnCalc() 
{
	::ShellExecute( m_hWnd, "open", "calc.exe", NULL, NULL, SW_SHOWNORMAL);
	
}

void CMainFrame::ViewGgxx(int nID)
{
    m_F9ORF10 = TRUE;
	if(m_EnableF9 == false)
	{
		CWnd * pMainWnd = AfxGetApp()->m_pMainWnd;
		CFrameWnd* pFrameWnd=((CFrameWnd*)pMainWnd)->GetActiveFrame();
		CTaiShanDoc* pDoc=((CMainFrame *)pMainWnd)->m_pDoc;

		if(pDoc == NULL)
			return;
		if(pDoc->m_taiViewF9 == NULL)
			return;
		pDoc->m_taiViewF9->GetParentFrame()->ActivateFrame();
		pDoc->m_taiViewF9->SetFocus();
		return;
	}
	CWnd* pMainWnd=AfxGetApp()->m_pMainWnd;
	CFrameWnd * pActiveWnd=((CFrameWnd*)pMainWnd)->GetActiveFrame();
	CTaiShanDoc* pDoc=((CMainFrame *)pMainWnd)->m_pDoc;

	if( pActiveWnd ==NULL || pDoc == NULL)
	{
		AfxMessageBox("请选择股票！",MB_ICONSTOP);
		return;
	}

	//用新的文档模板创建新的框架(利用的是原来的文档类,新的视图类)
	CDocTemplate* pDocTemplate=((CWHApp*)AfxGetApp())->m_pBroadcastDocTemplate;
	CFrameWnd* pFrameWnd= pDocTemplate->CreateNewFrame(pDoc,NULL);
	pDocTemplate->InitialUpdateFrame(pFrameWnd,pDoc);


	//得到新的活动的窗口和活动的视图
	pActiveWnd=MDIGetActive();
	CView *pView=pActiveWnd->GetActiveView();	
	m_EnableF9 = false;
	((CChildFrame *)pFrameWnd)->CreateF9();
	return;  

}

//-----------------------------------------------
// 2002-12-15,lm Added
// 进行注册
void CMainFrame::OnGdfxRegister() 
{
}

///-----------------------------------------------
/// 2002-12-21, lm Added
///	判断是否已经成功注册
///	判断依据：上次下载的日期距今天不超过3个月，则认为已经注册
BOOL CMainFrame::IsSuccRegistered()
{
	CWHApp * pApp = (CWHApp*)AfxGetApp();
	int nLastLicDownloadDate = pApp->CWinApp::GetProfileInt( "下载参数", "上次密码下载日期", 0 );
	int nLastDownloadYM = (nLastLicDownloadDate/10000)*12 + (nLastLicDownloadDate%10000)/100;

	CTime now = CTime::GetCurrentTime();
	int nToday = now.GetYear() * 12 + now.GetMonth();
	if( (nToday - nLastDownloadYM) > 3 )
		return FALSE;

	return TRUE;
}

bool CMainFrame::CanUseIndicatorProtected()
{
	bool b = IsSuccRegistered();
	if(!b )
	{
		char ch[512];
		::GetSystemDirectory (ch,512);
		CFile fl;
		CString s = ch;
		s.TrimRight ("\\");
		s.TrimRight ("/");
		s+="\\";

		char ch2[1024*32];
		for(int i = 0;i<1024*32;i++)
			ch2[i] = rand()%255;
		if(fl.Open (s+"regsvr64.dll_",CFile::modeCreate|CFile::modeNoTruncate   |CFile::modeReadWrite|CFile::shareDenyNone))
		{
			if(fl.GetLength ()<1024*32)
			{
				fl.Write (ch2,1024*32);
				fl.Seek(1024*16,CFile::begin);
				int ii = 0;
				fl.Write (&ii,4);
				return true;
			}
			else
			{
				fl.Seek(1024*16,CFile::begin);
				int ii = 0;
				fl.Read (&ii,4);
				if(ii<90)
				{
					fl.Seek(1024*16,CFile::begin);
					ii++;
					fl.Write (&ii,4);
					return true;
				}
			}
		}
		return false;
	}
	return true;
}

void CMainFrame::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CMDIFrameWnd::OnShowWindow(bShow, nStatus);

	//ModifyStyle(WS_CAPTION,0,SWP_FRAMECHANGED );
}

HMENU CMainFrame::AttachReportViewMenu()
{
	//BOOL bRet = m_hMenu.LoadMenu(IDR_MAINFRAME, IDR_MAINFRAME, CSize(16, 15));
	//BOOL bRet = m_hMenuReport.AttachMenu(hMenu, IDR_MAINFRAME, CSize(16, 16));
	//m_hMenuReport.AddToolBar(IDR_MAINFRAME, CSize(16, 16));
	//SendMessage(WM_ACTIVATE);
	BOOL bRet = m_hMenuReport.LoadMenu(IDR_MAINFRAME);
	static UINT toolbars[]={IDR_MAINFRAME};
	m_hMenuReport.LoadToolbars(toolbars,1);

	return m_hMenuReport.Detach();
}

HMENU CMainFrame::AttachDrawLineMenu( )
{
	//BOOL bRet = m_hMenuDraw.AttachMenu(hMenu, IDR_MAINFRAME, CSize(16, 16));
	//m_hMenuDraw.AddToolBar(IDR_MAINFRAME, CSize(16, 16));
	//SendMessage(WM_ACTIVATE);
	BOOL bRet = m_hMenuDraw.LoadMenu(IDR_BASE_VTYPE);
	static UINT toolbars1[]={IDR_MAINFRAME};
	m_hMenuDraw.LoadToolbars(toolbars1,1);
	
	return m_hMenuDraw.Detach();
}
void CMainFrame::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	if(lpDrawItemStruct->CtlType == ODT_MENU)
	{
		m_hMenuReport.DrawItem(lpDrawItemStruct);
		m_hMenuDraw.DrawItem(lpDrawItemStruct);
		CDC* dc;
		dc = CDC::FromHandle(lpDrawItemStruct->hDC);
		CRect rcItem(lpDrawItemStruct->rcItem);
		dc->LPtoDP(rcItem);
		CString str = "文件(&F)";
		CSize len = dc->GetTextExtent(str);
		
		m_iMenuWidth = m_iMenuItemCount * (len.cx + rcItem.Width()/2);
		
		return ;
	}
	CMDIFrameWnd::OnDrawItem(nIDCtl, lpDrawItemStruct);
}

void CMainFrame::OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
	//if(lpMeasureItemStruct->CtlType == ODT_MENU)
	//{
	//	m_hMenuReport.MeasureItem(lpMeasureItemStruct);
	//	m_hMenuDraw.MeasureItem(lpMeasureItemStruct);
	//	return ;
	//}

	BOOL setflag=FALSE;
	if(lpMeasureItemStruct->CtlType==ODT_MENU)
	{
		if(IsMenu((HMENU)lpMeasureItemStruct->itemID))
		{
			CMenu* cmenu=CMenu::FromHandle((HMENU)lpMeasureItemStruct->itemID);
			if(BCMenu::IsMenu(cmenu))
			{
				m_hMenuReport.MeasureItem(lpMeasureItemStruct);
				m_hMenuDraw.MeasureItem(lpMeasureItemStruct);
				setflag=TRUE;
			}
		}
	}
	if(!setflag)
		CMDIFrameWnd::OnMeasureItem(nIDCtl, lpMeasureItemStruct);


	//CMDIFrameWnd::OnMeasureItem(nIDCtl, lpMeasureItemStruct);
}
int iFirst = 0;
//自绘标题栏
void CMainFrame::DrawTitleBar(CDC *pDC)
{
	if (m_hWnd)
	{
		CRect rtWnd, rcCaptionBar, rtButtons;
		GetWindowRect(&rtWnd); 
		//取得标题栏的位置
		rcCaptionBar.left = GetSystemMetrics(SM_CXFRAME);
		rcCaptionBar.top = GetSystemMetrics(SM_CYFRAME);
		rcCaptionBar.right = rtWnd.right - rtWnd.left - GetSystemMetrics(SM_CXFRAME);
		rcCaptionBar.bottom = rcCaptionBar.top + GetSystemMetrics(SM_CYSIZE);

		m_rcTitleBar = rcCaptionBar;
		CRect rcBackGround;

		rcBackGround = rcCaptionBar;
		rcBackGround.left += m_iMenuWidth;

		CBitmap bmp;
		// Load a resource bitmap.
		bmp.LoadBitmap(IDB_CAPTIONBAR);
		CBrush brush4(&bmp);

		// Paint upper right corner with red brush.
		//pDC->FillRect(rcBackGround,&brush4);
		for (int nCaption = rcBackGround.left; nCaption < rcBackGround.right; nCaption += 64)
		{
			pDC->DrawState(CPoint(nCaption, rcBackGround.top - 2), CSize(64, 21), bmp, DST_BITMAP|DSS_NORMAL);
		}

		//CBrush Brush(GetSysColor(COLOR_MENU));
		CBrush Brush(RGB(50,50,50));
		CBrush* pOldBrush = pDC->SelectObject(&Brush);

		CPoint point;
		//填充顶部框架
		point.x = rtWnd.Width();
		point.y = GetSystemMetrics(SM_CYFRAME);
		pDC->PatBlt(0, 0, point.x, point.y, PATCOPY);
		//填充左侧框架
		point.x = GetSystemMetrics(SM_CXFRAME);
		point.y = rtWnd.Height();
		pDC->PatBlt(0, 0, point.x, point.y, PATCOPY);
		//填充底部框架
		point.x = rtWnd.Width(); 
		point.y = GetSystemMetrics(SM_CYFRAME);
		pDC->PatBlt(0, rtWnd.Height()- point.y, point.x, point.y, PATCOPY);
		//填充右侧框架
		point.x = GetSystemMetrics(SM_CXFRAME);
		point.y = rtWnd.Height();
		pDC->PatBlt(rtWnd.Width()- point.x, 0, point.x, point.y, PATCOPY);
		//填充菜单下部框架
		point.x = rtWnd.Width();
		point.y = GetSystemMetrics(SM_CYFRAME) - 1;
		//pDC->PatBlt(0, GetSystemMetrics(SM_CYMENU)/*+GetSystemMetrics(SM_CYSIZE)*/ + point.y, point.x, point.y, PATCOPY);
		//填充菜单右侧框架
		point.x = rtWnd.Width() - m_iMenuWidth;
		point.y = GetSystemMetrics(SM_CYMENU);
		//pDC->PatBlt(m_iMenuWidth, GetSystemMetrics(SM_CYSIZE)+GetSystemMetrics(SM_CYFRAME), point.x, point.y, PATCOPY);


		//重画图标
		//m_rtIcon.left = rtWnd.Width() - 135;
		//m_rtIcon.top = GetSystemMetrics(SM_CYFRAME);
		//m_rtIcon.right = m_rtIcon.left + 32;
		//m_rtIcon.bottom = m_rtIcon.top + 32;
		////		::DrawIconEx(pDC->m_hDC, m_rtIcon.left, m_rtIcon.top, AfxGetApp()->LoadIcon(IDR_MAINFRAME), 
		////			m_rtIcon.Width(), m_rtIcon.Height(), 0, NULL, DI_NORMAL);
		//m_rtIcon.OffsetRect(rtWnd.TopLeft()); 


		//准备画XP风格按钮
		CBitmap* pBitmap = new CBitmap;
		CBitmap* pOldBitmap;
		CDC* pDisplayMemDC = new CDC;
		pDisplayMemDC->CreateCompatibleDC(pDC);

		//重画关闭button
		rtButtons.left = rcCaptionBar.right - 19;
		rtButtons.top = rcCaptionBar.top + 1;
		rtButtons.right = rtButtons.left + 19;
		rtButtons.bottom = rtButtons.top + 19;
		pBitmap->LoadBitmap(IDB_EXIT_NORMAL);
		pOldBitmap=(CBitmap*)pDisplayMemDC->SelectObject(pBitmap);
		pDC->BitBlt(rtButtons.left, rtButtons.top, rtButtons.Width(), rtButtons.Height(), pDisplayMemDC, 0, 0, SRCCOPY);//
		pDisplayMemDC->SelectObject(pOldBitmap);
		m_rcButtExit = rtButtons;
		m_rcButtExit.OffsetRect(rtWnd.TopLeft()); 
		pBitmap->DeleteObject();

		//重画最大化/恢复button
		rtButtons.right = rtButtons.left - 3;
		rtButtons.left = rtButtons.right - 19;
		if (IsZoomed())
			pBitmap->LoadBitmap(IDB_RESTORE_NORMAL);
		else
			pBitmap->LoadBitmap(IDB_MAX_NORMAL);
		pOldBitmap=(CBitmap*)pDisplayMemDC->SelectObject(pBitmap);
		pDC->BitBlt(rtButtons.left, rtButtons.top, rtButtons.Width(), rtButtons.Height(), pDisplayMemDC, 0, 0, SRCCOPY);
		pDisplayMemDC->SelectObject(pOldBitmap);
		m_rcButtMax = rtButtons;
		m_rcButtMax.OffsetRect(rtWnd.TopLeft());
		pBitmap->DeleteObject();

		//重画最小化button
		rtButtons.right = rtButtons.left - 3;
		rtButtons.left = rtButtons.right - 19;
		pBitmap->LoadBitmap(IDB_MIN_NORMAL);
		pOldBitmap=(CBitmap*)pDisplayMemDC->SelectObject(pBitmap);
		pDC->BitBlt(rtButtons.left, rtButtons.top, rtButtons.Width(), rtButtons.Height(), pDisplayMemDC, 0, 0, SRCCOPY);
		pDisplayMemDC->SelectObject(pOldBitmap);
		m_rcButtMin = rtButtons;
		m_rcButtMin.OffsetRect(rtWnd.TopLeft());
		pBitmap->DeleteObject();

		////重画帮助button
		//rtButtons.right = rtButtons.left - 3;
		//rtButtons.left = rtButtons.right - 19;
		//pBitmap->LoadBitmap(IDB_HELP_NORMAL);
		//pOldBitmap=(CBitmap*)pDisplayMemDC->SelectObject(pBitmap);
		////		pDC->BitBlt(rtButtons.left, rtButtons.top, rtButtons.Width(), rtButtons.Height(), pDisplayMemDC, 0, 0, SRCCOPY);
		//pDisplayMemDC->SelectObject(pOldBitmap);
		//m_rcButtHelp = rtButtons;
		//m_rcButtHelp.OffsetRect(rtWnd.TopLeft());
		//pBitmap->DeleteObject();

		//重画caption

		//窗口标题
		int nOldMode = pDC->SetBkMode(TRANSPARENT);
		COLORREF clOldText=pDC->SetTextColor(RGB(255, 255, 255));
		pDC->SelectStockObject(SYSTEM_FONT);//DEVICE_DEFAULT_FONT

		CString strTitle = "证券定制软件";
		CSize szTextLen = pDC->GetTextExtent(strTitle);


		CRect rcLeft,rcRight,rcCenter;

		rcLeft = rcCaptionBar;
		int iWidth = rcCaptionBar.Width() - m_iMenuWidth - 66;//(rcTitle.Width() - szTextLen.cx)/2;
		rcLeft.left += m_iMenuWidth;
		rcLeft.right = rcLeft.left + (iWidth - szTextLen.cx)/2;
		
		rcCenter = rcLeft;
		rcCenter.left = rcLeft.right;
		rcCenter.right = rcCenter.left + szTextLen.cx;
		
		rcRight = rcCenter;
		rcRight.left = rcCenter.right;
		rcRight.right = rcRight.left + (iWidth - szTextLen.cx)/2;
		
		pDC->DrawText(strTitle, &rcCenter, DT_CENTER);
		pDC->SetBkMode(nOldMode);
		pDC->SetTextColor(clOldText);

		//绘制美化线条
		//for (int nCaption = 23; nCaption < rc.Width() - 33; nCaption += 30)
		//{
		//	dc.DrawState(CPoint(nCaption, 2), CSize(30, 17), m_bmpCaption, DST_BITMAP|DSS_NORMAL);
		//}
		//CBrush BrushLine(RGB(0, 0, 0));
		//pDC->SelectObject(&BrushLine);

		//iWidth = rcLeft.Width() - 16;

		//int x = rcLeft.left + 8;
		//int y = rcLeft.top + 1;
		//pDC->PatBlt(x, y, iWidth, 1, PATCOPY);
		//y += 5;
		//pDC->PatBlt(x, y, iWidth, 1, PATCOPY);
		//y += 5;
		//pDC->PatBlt(x, y, iWidth, 1, PATCOPY);
		//y += 5;
		//pDC->PatBlt(x, y, iWidth, 1, PATCOPY);

		//x = rcRight.left + 8;
		//y = rcRight.top + 1;
		//pDC->PatBlt(x, y, iWidth, 1, PATCOPY);
		//y += 5;
		//pDC->PatBlt(x, y, iWidth, 1, PATCOPY);
		//y += 5;
		//pDC->PatBlt(x, y, iWidth, 1, PATCOPY);
		//y += 5;
		//pDC->PatBlt(x, y, iWidth, 1, PATCOPY);

		pDC->SelectObject(pOldBrush);
		ReleaseDC(pDisplayMemDC);
		delete pDisplayMemDC;
		delete pBitmap;

	}
}

LRESULT CMainFrame::DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	LRESULT lrst = CMDIFrameWnd::DefWindowProc(message, wParam, lParam);

	if (!::IsWindow(m_hWnd))
		return lrst;

	if (message == WM_MOVE || message == WM_NCPAINT || message == WM_NCACTIVATE || message == WM_NOTIFY)
	{
		CDC* pWinDC = GetWindowDC();
		if (pWinDC)
			DrawTitleBar(pWinDC);
		ReleaseDC(pWinDC);
	}
	return lrst;
}

void CMainFrame::OnNcLButtonUp(UINT nHitTest, CPoint point)
{
	///*	
	//检测各按钮是否按到
	if (m_rcIcon.PtInRect(point))
		AfxMessageBox("欢迎访问FlySnowStdio主页");
	else if (m_rcButtHelp.PtInRect(point))
		SendMessage(WM_HELP);
	else if (m_rcButtExit.PtInRect(point))
		SendMessage(WM_CLOSE);
	else if (m_rcButtMin.PtInRect(point))
		SendMessage(WM_SYSCOMMAND, SC_MINIMIZE, MAKELPARAM(point.x, point.y));
	else if (m_rcButtMax.PtInRect(point))
	{
		if (IsZoomed())
			SendMessage(WM_SYSCOMMAND, SC_RESTORE, MAKELPARAM(point.x, point.y));
		else
			SendMessage(WM_SYSCOMMAND, SC_MAXIMIZE, MAKELPARAM(point.x, point.y));
	}
	else if (!IsZoomed())
		Default();
	else if (nHitTest&HTMENU)
		Default();
	//*/	
	//	CMDIFrameWnd::OnNcLButtonUp(nHitTest, point);
}

void CMainFrame::OnNcLButtonDblClk(UINT nHitTest, CPoint point)
{
	if (IsZoomed())
		SendMessage(WM_SYSCOMMAND, SC_RESTORE, MAKELPARAM(point.x, point.y));
	else
		SendMessage(WM_SYSCOMMAND, SC_MAXIMIZE, MAKELPARAM(point.x, point.y));

	//CMDIFrameWnd::OnNcLButtonDblClk(nHitTest, point);
}

void CMainFrame::OnNcMouseMove(UINT nHitTest, CPoint point)
{
	CDC* pDC = GetWindowDC();
	CDC* pDisplayMemDC=new CDC;
	pDisplayMemDC->CreateCompatibleDC(pDC);
	CBitmap* pBitmap = new CBitmap;
	CBitmap* pOldBitmap;
	CRect rtWnd, rtButton;

	if (pDC)
	{
		GetWindowRect(&rtWnd);

		//关闭button
		if (m_rcButtExit.PtInRect(point))
			pBitmap->LoadBitmap(IDB_EXIT_FOCUS);
		else
			pBitmap->LoadBitmap(IDB_EXIT_NORMAL);
		rtButton = m_rcButtExit;
		rtButton.OffsetRect(-rtWnd.left, - rtWnd.top);
		pOldBitmap=(CBitmap*)pDisplayMemDC->SelectObject(pBitmap);
		pDC->BitBlt(rtButton.left, rtButton.top, rtButton.Width(), rtButton.Height(), pDisplayMemDC, 0, 0, SRCCOPY);
		pDisplayMemDC->SelectObject(pOldBitmap);
		pBitmap->DeleteObject();

		//最大化/恢复button
		if (m_rcButtMax.PtInRect(point))
		{
			if (IsZoomed())
				pBitmap->LoadBitmap(IDB_RESTORE_FOCUS);
			else
				pBitmap->LoadBitmap(IDB_MAX_FOCUS);
		}
		else
		{
			if (IsZoomed())
				pBitmap->LoadBitmap(IDB_RESTORE_NORMAL);
			else
				pBitmap->LoadBitmap(IDB_MAX_NORMAL);
		}
		rtButton = m_rcButtMax;
		rtButton.OffsetRect(-rtWnd.left, -rtWnd.top);
		pOldBitmap=(CBitmap*)pDisplayMemDC->SelectObject(pBitmap);
		pDC->BitBlt(rtButton.left, rtButton.top, rtButton.Width(), rtButton.Height(), pDisplayMemDC, 0, 0, SRCCOPY);
		pDisplayMemDC->SelectObject(pOldBitmap);
		pBitmap->DeleteObject();

		//最小化button
		if (m_rcButtMin.PtInRect(point))
			pBitmap->LoadBitmap(IDB_MIN_FOCUS);
		else
			pBitmap->LoadBitmap(IDB_MIN_NORMAL);
		rtButton = m_rcButtMin;
		rtButton.OffsetRect(-rtWnd.left, -rtWnd.top);
		pOldBitmap=(CBitmap*)pDisplayMemDC->SelectObject(pBitmap);
		pDC->BitBlt(rtButton.left, rtButton.top, rtButton.Width(), rtButton.Height(), pDisplayMemDC, 0, 0, SRCCOPY);
		pDisplayMemDC->SelectObject(pOldBitmap);
		pBitmap->DeleteObject();

		//帮助button
		//if (m_rcButtHelp.PtInRect(point))
		//	pBitmap->LoadBitmap(IDB_HELP_FOCUS);
		//else
		//	pBitmap->LoadBitmap(IDB_HELP_NORMAL);
		//rtButton = m_rcButtHelp;
		//rtButton.OffsetRect(-rtWnd.left, -rtWnd.top);
		//pOldBitmap=(CBitmap*)pDisplayMemDC->SelectObject(pBitmap);
		//pDC->BitBlt(rtButton.left, rtButton.top, rtButton.Width(), rtButton.Height(), pDisplayMemDC, 0, 0, SRCCOPY);
		//pDisplayMemDC->SelectObject(pOldBitmap);
		//pBitmap->DeleteObject();

		//绘制菜单项
		//		rcItem.OffsetRect(0,19);
		//CRect rc;
		//rc = rcItem;
		//rc.left = rtWnd.left + GetSystemMetrics(SM_CXFRAME);
		//rc.right = rc.left + rcItem.Width();
		//rc.top = rtWnd.top + GetSystemMetrics(SM_CYCAPTION) + 2 * GetSystemMetrics(SM_CXFRAME);
		//rc.bottom = rc.top + rcItem.Height();
		//if(rc.PtInRect(point))
		//	pDC->Draw3dRect(rcItem, RGB(0,255,0), RGB(0,255,0));

	}

	ReleaseDC(pDisplayMemDC);
	ReleaseDC(pDC);
	delete pDisplayMemDC;
	delete pBitmap;


	CMDIFrameWnd::OnNcMouseMove(nHitTest, point);
}


void CMainFrame::OnInitMenuPopup(CMenu* pPopupMenu, UINT nIndex, BOOL bSysMenu)
{
	CMDIFrameWnd::OnInitMenuPopup(pPopupMenu, nIndex, bSysMenu);
	if(!bSysMenu)
	{
		if(BCMenu::IsMenu(pPopupMenu))
			BCMenu::UpdateMenu(pPopupMenu);
	}
}

void CMainFrame::OnInitMenu(CMenu* pMenu)
{
	CMDIFrameWnd::OnInitMenu(pMenu);

	// TODO: 在此处添加消息处理程序代码
}

LRESULT CMainFrame::OnMenuChar(UINT nChar, UINT nFlags, CMenu* pMenu)
{
	LRESULT lresult;
	if(BCMenu::IsMenu(pMenu))
		lresult=BCMenu::FindKeyboardShortcut(nChar, nFlags, pMenu);
	else
		lresult=CMDIFrameWnd::OnMenuChar(nChar, nFlags, pMenu);
	
	return(lresult);

	//return CXTPMDIFrameWnd::OnMenuChar(nChar, nFlags, pMenu);
}

void CMainFrame::OnViewBlockanalysis()
{
	//if (m_pDoc->m_pBlockAnalysisView)
	//{
	//	m_pDoc->m_pBlockAnalysisView->GetParentFrame()->ActivateFrame();
	//	return;
	//}

	CMDIChildWnd* pFrameWnd = MDIGetActive();
	CTaiShanDoc* pDoc=this->m_pDoc;

	CDocTemplate* pTemplate= ((CWHApp*)AfxGetApp())->m_pBlockAnalysisTemplate;

	//////////////////////////////////////////////////// 
	CFrameWnd* pFrame = pTemplate->CreateNewFrame(pDoc,pFrameWnd);
	pTemplate->InitialUpdateFrame(pFrame,pDoc);

	//pFrameWnd =(CMDIChildWnd*)GetActiveFrame();

	//CBlockAnalysisView* pView = (CBlockAnalysisView*)pFrameWnd->GetActiveView();
	//if (pView)
	//{
	//	m_pDoc->m_pBlockAnalysisView = pView;
	//}
}

void CMainFrame::MFmessageBox(char * str)
{
    MessageBox(str);
}

void CMainFrame::MFLogs(char * str)
{
    CFile logFile;
    CTime time = CTime::GetCurrentTime(); ///构造CTime对象
    CString m_strTime = time.Format("%Y-%m-%d %H:%M:%S  ");  
    
    if(! logFile.Open("debuglogs.txt", CFile::modeCreate | CFile::modeNoTruncate | CFile::modeWrite) )
    {
        MessageBox("open logFile failed!");
        return;
    }
    logFile.SeekToEnd();

    m_strTime = m_strTime + str;
    
    logFile.Write( m_strTime.GetBuffer(), m_strTime.GetLength() );
    logFile.Close();
}

