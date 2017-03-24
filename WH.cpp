// CWHApp.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "WH.h"
#include "ChooseReceiveDataForm.h"
#include "MainFrm.h"
#include "ChildFrm.h"
#include "CTaiShanDoc.h"
#include "cellRange.h"
#include "CTaiShanReportView.h"
#include "CTaiShanKlineShowView.h"
#include "BlockAnalysisView.h"
#include "CompanyInfoView.h"
#include "MyTreeView.h"
#include "MyRichView.h"
#include "InfoView.h"
////////////////////
#include "NineShowView.h"
#include "NineShowFrm.h"
#include "MySplash.h"
#include "GuiZhouTvView.h"
#include "UserRegPart2.h"
#include "CTaiKlineFileKLine.h"
#include "FyRegister.h"

#include <afxpriv.h>

///////////////////////////
#include "CaiwuData.h"
#include  "CTaiChuQuanInDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#ifdef __cplusplus
extern "C" {
extern long ReadDog( void );
extern long WriteDog( void );
extern short DogAddr,DogBytes;
extern void * DogData;
}
#else
extern long ReadDog( void );
extern long WriteDog( void );
extern short DogAddr,DogBytes;
extern void * DogData;
#endif
extern HANDLE hDevice;

int CWHApp::m_gMessageID = 0;
bool CWHApp::m_gbUseExe = false;
bool CWHApp::m_gbDoInitate = true;

#define TEST_USER1
CTime g_timeUseEnd(2017,12,30,8,0,0);
/////////////////////////////////////////////////////////////////////////////
// CWHApp
void DoHtmlHelp(CWnd* pWnd,int nID)
{
	if(pWnd)
//	HtmlHelp(pWnd->m_hWnd,"ybw.chm",HH_HELP_CONTEXT,nID);
	::ShellExecute( pWnd->m_hWnd, "open", "ts.chm", NULL, NULL, SW_SHOWNORMAL);
};

BEGIN_MESSAGE_MAP(CWHApp, CWinApp)
	//{{AFX_MSG_MAP(CWHApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
	ON_COMMAND(ID_FILE_PRINT_SETUP, OnFilePrintSetup)
	ON_UPDATE_COMMAND_UI(ID_APP_ABOUT, OnUpdateAppAbout)
	//}}AFX_MSG_MAP
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
	// Standard print setup command
	ON_COMMAND(ID_FILE_PRINT_SETUP, CWinApp::OnFilePrintSetup)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWHApp construction
CWHApp::CWHApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
    Line_mode = 5;
	m_key = 0;
	m_bAppAuthorized=TRUE;
	m_gbUseExe = false;
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CWHApp object

CWHApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CWHApp initialization
STARTUPINFO startup;
PROCESS_INFORMATION process;

CString g_strCompanyName1 = "";
CString g_strCompanyName2 = "";
CString g_strCompanyName3 = "";
CString g_strCompanyName = "证券分析软件";
CTaiTestSplash *m_splash = NULL;

BOOL CWHApp::InitInstance()
{
	AfxEnableControlContainer();

//#ifdef ZJH
	m_gMessageID = ::RegisterWindowMessage("WsSendMessageHqData");
	CFileFind fnd;
	if(S_OK != ::CoInitialize (NULL))
		return FALSE;

	//Marenan----注册或充值----   

//	ReadDiskIDPartCwd();
//	if(!FyRegister::IsValidUser())
//		return FALSE;

   memset( &startup, 0, sizeof( startup ) );
   startup.cb = sizeof( startup );
   memset( &process, 0, sizeof( process ) );
   startup.dwFlags = STARTF_USESHOWWINDOW;
   startup.wShowWindow = SW_HIDE;
	if(fnd.FindFile ("Receiver.exe"))
	{
		m_gbUseExe = true;
	}

	hAppMutex=::CreateMutex(NULL,TRUE,m_pszExeName);
	if(GetLastError() == ERROR_ALREADY_EXISTS)
	{
		CWnd *pPrevWnd = CWnd::GetDesktopWindow()->GetWindow(GW_CHILD);
		while(pPrevWnd)
		{
			if(::GetProp(pPrevWnd->GetSafeHwnd(),m_pszExeName))
			{
			  if(pPrevWnd->IsIconic())
				  pPrevWnd->ShowWindow(SW_RESTORE);
			  pPrevWnd->SetForegroundWindow();
			  pPrevWnd->GetLastActivePopup()->SetForegroundWindow();
			  return false;
			} 
			pPrevWnd = pPrevWnd->GetWindow(GW_HWNDNEXT);
		}
		return false;
	}
//#endif
	m_bAppAuthorized=TRUE;

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	// of your final executable, you should remove from the following
  	// the specific initialization routines you do not need.
	
	BOOL SplashOpen=FALSE;
	m_splash = new CTaiTestSplash;
	SplashOpen=m_splash->Create();
	if( SplashOpen )
		m_splash->ShowWindow(SW_SHOW);
	DWORD Currenttime=GetTickCount();
	BeginWaitCursor();

#ifdef TEST_USER1
	t = CTime::GetCurrentTime();
    CTime t2 = g_timeUseEnd;
	if(t >= t2)
	{
		AfxMessageBox("试用期已过，若想继续使用，请与系统供应商联系!",MB_OK | MB_ICONSTOP);
	    return false;
	}
	else
	{
	//	AfxMessageBox("现在您所使用的是试用版，试用时间至2004年8月30日，到期后若想继续使用，请与系统供应商联系!",MB_OK | MB_ICONSTOP);
	}
#endif

	AfxInitRichEdit( );
#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	// Change the registry key under which our settings are stored.
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization
	UpdateTheOEMInfo();	

	SetRegistryKey("");
	LoadStdProfileSettings();  // Load standard INI file options (including MRU)

	//CMultiDocTemplate* pDocTemplate;
#ifdef WIDE_NET_VERSION
	pDocTemplate = new CMultiDocTemplate(
		IDR_MAINFRAME_WIDE,
		RUNTIME_CLASS(CTaiShanDoc),
		RUNTIME_CLASS(CChildFrame), // custom MDI child frame
		RUNTIME_CLASS(CTaiShanReportView));
	AddDocTemplate(pDocTemplate);

	//to create a new type of view///////////
    pDrawTemplate = new CMultiDocTemplate(
		IDR_VWBASETYPE_WIDE,
		RUNTIME_CLASS(CTaiShanDoc),
		RUNTIME_CLASS(CChildFrame), // custom MDI child frame
		RUNTIME_CLASS(CTaiShanKlineShowView));
#else
	pDocTemplate = new CMultiDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CTaiShanDoc),
		RUNTIME_CLASS(CChildFrame), // custom MDI child frame
		RUNTIME_CLASS(CTaiShanReportView));
	AddDocTemplate(pDocTemplate);

	//to create a new type of view///////////
    pDrawTemplate = new CMultiDocTemplate(
		IDR_BASE_VTYPE,//IDR_NEWTYPE
		RUNTIME_CLASS(CTaiShanDoc),
		RUNTIME_CLASS(CChildFrame), // custom MDI child frame
		RUNTIME_CLASS(CTaiShanKlineShowView));
#endif
	
	////////////////////////////////////
    m_pBroadcastDocTemplate = new CMultiDocTemplate(
		IDR_BASEINFO,
		RUNTIME_CLASS(CTaiShanDoc),
		RUNTIME_CLASS(CChildFrame), // custom MDI child frame
		RUNTIME_CLASS(CTaiTestRichView)); 

    // added by Xia Qiankun
	pNineShowTemplate = new CMultiDocTemplate(
		IDR_NINE_VIEW,
		RUNTIME_CLASS(CTaiShanDoc),
		RUNTIME_CLASS(CNineShowFrame), // custom MDI child frame
		RUNTIME_CLASS(CNineShowView));

	m_pMyNewTemplate=new CMultiDocTemplate(
		IDI_ICON1,
		RUNTIME_CLASS(CTaiShanDoc),
		RUNTIME_CLASS(CChildFrame),
		RUNTIME_CLASS(CGuiZhouTvView));
	m_pBlockAnalysisTemplate = new CMultiDocTemplate(
		IDI_ICON1,
		RUNTIME_CLASS(CTaiShanDoc),
		RUNTIME_CLASS(CChildFrame),
		RUNTIME_CLASS(CBlockAnalysisView));

	//if(m_gbUseExe == true)
	//{
	//	BOOL tSuccess = CreateProcess( _T("Receiver.exe"), NULL, NULL, NULL,
	//		FALSE, 0, NULL, NULL, &startup, &process );
	//	//ToTerminateChildProcess();
	//}

	// create main MDI Frame window
	CMainFrame* pMainFrame = new CMainFrame;
	if (!pMainFrame->LoadFrame(IDR_MAINFRAME))
		return FALSE;
	m_pMainWnd = pMainFrame;

	pDocTemplate->m_hMenuShared = pMainFrame->AttachReportViewMenu();	
	pDrawTemplate->m_hMenuShared = pMainFrame->AttachDrawLineMenu();	

	pMainFrame->OnUpdateFrameMenu(pMainFrame->m_hMenuDefault);


	//////界面优化风格/////////////////////////////////////
	TCHAR szStylesPath[_MAX_PATH];
	CString strStylesPath;

	VERIFY(::GetModuleFileName(AfxGetApp()->m_hInstance, szStylesPath, _MAX_PATH));		

	CString csStylesPath(szStylesPath);
	int nIndex  = csStylesPath.ReverseFind(_T('\\'));
	if (nIndex > 0) 
	{
		csStylesPath = csStylesPath.Left(nIndex);
	}
	else 
	{
		csStylesPath.Empty();
	}
	//strStylesPath = csStylesPath + _T("\\Styles\\");xtpSkinApplyFrame | 

	XTPSkinManager()->SetApplyOptions(xtpSkinApplyFrame |xtpSkinApplyColors | xtpSkinApplyMetrics);
	//XTPSkinManager()->LoadSkin(strStylesPath + _T("Nocturnal.msstyles"));	
	XTPSkinManager()->LoadSkin(strStylesPath + _T("Vista.cjstyles"),_T("NormalBlack.INI"));	
	//XTPSkinManager()->LoadSkin(strStylesPath + _T("Office2007.cjstyles"));	
	//XTPSkinManager()->LoadSkin(strStylesPath + _T("Office2007.cjstyles"),"NormalAqua.ini");	

	//	XTPSkinManager()->AddColorFilter(new CXTPSkinManagerColorFilterShiftRGB(-40, -80, -40));
	//	//XTPSkinManager()->AddColorFilter(new CColorFilterMonochrome());
	XTPSkinManager()->RedrawAllControls();
	XTPPaintManager()->RefreshMetrics();

	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);
	CString sCommand = this->m_lpCmdLine;
	if(sCommand.GetLength ()>0)
		m_gbDoInitate = false;

	// Dispatch commands specified on the command line
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	m_nCmdShow |= SW_SHOWMAXIMIZED;

	pMainFrame->ShowWindow(m_nCmdShow);
	pMainFrame->UpdateWindow();	// The main window has been initialized, so show and update it.


	Currenttime = GetTickCount() - Currenttime;

    if(Currenttime<8000)
	{
		if(SplashOpen)
		{
			try
			{
				m_splash->SetWindowPos(&(CWnd::wndTop),0,0,0,0,SWP_NOMOVE | SWP_NOSIZE);
			}
			catch(...)
			{
			}
		}
		else
		{
			delete m_splash;
			m_splash = NULL;
		}
	}
    ::SetProp(pMainFrame->GetSafeHwnd(),m_pszExeName,(HANDLE)1);
	EndWaitCursor(); 
	//m_splash->HideWindow();
	//pMainFrame->SendMessage(WM_SYSCOMMAND,SC_MAXIMIZE,0);

	return TRUE;
}




/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	CStatic	m_static_Icon;
	CString	m_strCredit;
	CString	m_strSerial;
	CString	m_strCompany;
	CString	m_strUser;
	CString	m_strPName;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	m_strCredit = _T("");
	m_strSerial = _T("");
	m_strCompany = _T("");
	m_strUser = _T("");
	m_strPName = _T("");
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	DDX_Control(pDX, IDC_STA_ICON, m_static_Icon);
	DDX_Text(pDX, IDC_STATIC_COMPANY, m_strCompany);
	DDX_Text(pDX, IDC_STATIC_USER, m_strUser);
	DDX_Text(pDX, IDC_STATIC_PNAME, m_strPName);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// App command to run the dialog
void CWHApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}


BOOL CAboutDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	CWHApp * pApp = (CWHApp*)AfxGetApp();
	
	// TODO: Add extra initialization here
	CTaiShanDoc *pDoc = CMainFrame::m_pDoc;
	m_strUser=pDoc->m_strUser;
	m_strPName = "定制分析软件";
    SetWindowText(m_strPName);  
	m_strPName += "，1.00.001";

	m_strUser=_T("本软件使用权属于：") + m_strUser;
	CString str=_T("，版权所有 (C) 2012-2018 ");
	m_strCompany = CString("上海科技有限公司版权所有") + str;

	m_strSerial=pDoc->m_strSerial;
	m_strSerial.MakeUpper();
	m_strSerial.Insert(4,'-');
	m_strSerial.Insert(9,'-');
	m_strSerial.Insert(14,'-');
	
	m_strCredit=pDoc->m_strCredit;
	m_strCredit.MakeUpper();
	m_strCredit.Insert(4,'-');
	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


int CWHApp::ExitInstance() 
{
	if(m_splash)
		delete m_splash;
	if(pDrawTemplate)
		delete pDrawTemplate;
	if(m_pMyNewTemplate)
		delete m_pMyNewTemplate;
	if(m_pBlockAnalysisTemplate)
		delete m_pBlockAnalysisTemplate;
//   if(m_pF9DocTemplate)
//		delete m_pF9DocTemplate;
	if(m_pBroadcastDocTemplate)
		delete m_pBroadcastDocTemplate;
	if(pNineShowTemplate)
		delete pNineShowTemplate;

    ReleaseMutex(hAppMutex);

//	ToTerminateChildProcess();


	return CWinApp::ExitInstance();
}

void CWHApp::OnFilePrintSetup() 
{
	// TODO: Add your command handler code here
	try
	{
		CWinApp::OnFilePrintSetup();
	}
	catch(...)
	{
	}
	
}
BOOL CWHApp::OnIdle(LONG lCount)
{
	BOOL bMore = CWinApp::OnIdle(lCount);
	// if we still need more processing time, ask for it...
	CTaiShanDoc* pDoc = (CTaiShanDoc*)(((CMainFrame*)m_pMainWnd)->m_pDoc);
	if(pDoc==NULL)
	   return bMore;
//	CTaiShanDoc* pDoc = (CTaiShanDoc*)(((CMainFrame*)m_pMainWnd)->GetActiveDocument());
	if ( pDoc->IdleProc( lCount ) )
	   bMore = TRUE;
	return bMore;
}

CLongString::CLongString()
{
	m_nMax = 4096;
	m_nCurrentNum =0;
	m_nAdd = 4096;
	m_pData = new char[4096];
	memset(m_pData,0,m_nMax);
}

CLongString::~CLongString()
{
	if(m_pData!=NULL)
		delete []m_pData;
}
void CLongString::operator+(CString s)
{
	int n = s.GetLength ();
	if(n+m_nCurrentNum>=m_nMax)
	{
		m_nMax = ((n+m_nCurrentNum)/m_nAdd+1)*m_nAdd;
		if(m_nMax<=0)
			m_nMax = 1;
		char* pch = new char[m_nMax];
		strcpy(pch,m_pData);
//		memset(m_pData,0,m_nMax);
		delete []m_pData;
		m_pData = pch;
	}
	strcpy(m_pData+m_nCurrentNum,s);
	m_nCurrentNum+=n;
}

int CLongString::GetLength()
{
	return m_nCurrentNum;
}

CString CLongString::FloatToString(float fVal)
{
	CString s;
	s.Format ("%.3f",fVal);
	if(s.Find (".")>=0)
	{
		s.TrimRight ("0");
		s.TrimRight (".");
	}

	return s;
}

void CLongString::FloatArrayToExcelString(CLongString & longStr,float *pFloat, int nTot, int nBegin)
{
	CString s;
	for(int i=0;i<nTot;i++)
	{
		if(i<nBegin)
			s="    ";
		else
			s = FloatToString(pFloat[i]);
		s = s+"\t";
		longStr+s;
	}
	s="\n";
	longStr+s;

}

void CWHApp::DispatchMessageEachTime()
{
		MSG message;
		if(PeekMessage(&message,NULL,0,0,PM_REMOVE))
		{
			if (message.message != WM_KICKIDLE && !theApp.PreTranslateMessage(&message))
			{
				 TranslateMessage(&message);
				 DispatchMessage(&message);
			}
		}
}

int CWHApp::Run() 
{
	ASSERT_VALID(this);

	// for tracking the idle time state
	BOOL	bIdle = TRUE;
	LONG	lIdleCount = 0;
	MSG		m_msgCur;

	CFile file;
	file.Open ("reportError.txt",CFile::modeCreate|CFile::modeWrite);
	// acquire and dispatch messages until a WM_QUIT message is received.
	for (;;)
	{
		// phase1: check to see if we can do idle work
		while (bIdle &&
			!::PeekMessage(&m_msgCur, NULL, NULL, NULL, PM_NOREMOVE))
		{
			// call OnIdle while in bIdle state
			if (!OnIdle(lIdleCount++))
				bIdle = FALSE; // assume "no idle" state
		}

		// phase2: pump messages while available
#ifndef _DEBUG
		try
		{
#endif //_DEBUG
			do
			{
				// pump message, but quit on WM_QUIT
				if (!PumpMessage())
					return ExitInstance();

				// reset "no idle" state after pumping "normal" message
				if (IsIdleMessage(&m_msgCur))
				{
					bIdle = TRUE;
					lIdleCount = 0;
				}

			} while (::PeekMessage(&m_msgCur, NULL, NULL, NULL, PM_NOREMOVE));
#ifndef _DEBUG
		}
		/*		catch(CException e)
		{
		char szCause [255];
		e.GetErrorMessage(szCause, 255);
		CString s=szCause;
		s+="\r\n";
		file.Write(s.GetBuffer(s.GetLength()),s.GetLength());
		if(!this->m_pMainWnd->IsWindowEnabled())
		m_pMainWnd->EnableWindow();
		//			AfxMessageBox("meet an error");
		}*/
		//		__except(1)
		catch(...)
		{
			CString s;
			int n = GetLastError();
			s.Format("%d\r\n",n);
			file.Write(s.GetBuffer(s.GetLength()),s.GetLength());
			if(!this->m_pMainWnd->IsWindowEnabled())
				m_pMainWnd->EnableWindow();
		}
#endif //_DEBUG

	}

	ASSERT(FALSE);  // not reachable


	//	return CWinApp::Run();
}

void CWHApp::ToTerminateChildProcess()
{
	if(process.hProcess !=0)
	{
		::TerminateProcess (process.hProcess,9);
	}
}
//     1、给定股票代码和市场类型，返回股票名称的字符串指针。
CString CWHApp::GetStkName( LPCSTR lpszLabel, WORD wMarket )
{
	CString s = "";
	int kind = CMainFrame::m_pDoc ->m_sharesInformation .GetStockKind(wMarket,(char*)lpszLabel);
	if(kind>=0)
	{
		PCdat1 pStockData = NULL;
		if(CMainFrame::m_pDoc ->m_sharesInformation.Lookup ((char*)lpszLabel,pStockData,kind))
			s = pStockData->name ;
	}
	return s;
}
 
//     2、给定日期和股票代码、市场类型，返回那天日线结构的指针，
//        返回NULL表示失败。        bDoPowerSplit 表示是否进行还权。
void* CWHApp::GetLineDayData( LPCSTR lpszLabel, WORD wMarket, CTime tm, BOOL bDoPowerSplit )
{
	Kline* pKline = NULL;
	int kind = CMainFrame::m_pDoc ->m_sharesInformation .GetStockKind(wMarket,(char*)lpszLabel);
	if(kind>=0)
	{
		PCdat1 pStockData = NULL;
		if(CMainFrame::m_pDoc ->m_sharesInformation.Lookup ((char*)lpszLabel,pStockData,kind))
		{
			CTaiKlineFileKLine * pFile = CTaiKlineFileKLine::GetFilePointer(lpszLabel,kind,true);
			Kline* pKline2 = NULL;
			int n = pFile->ReadKlineAccordingTime(lpszLabel, pKline2, tm ,2,0);
			if(n>0)
			{
				static Kline retval;
				pKline = &retval;
				pKline[0] = pKline2[n-1];
			}
			if(pKline2)
				delete [] pKline2;
		}
	}
	return pKline;//need to delete this pointer out of this function;
}
 
//     3、给定股票代码和市场类型，返回该股票的基本面资料（如财务数据等) 。
//      返回NULL表示失败。
void * CWHApp::GetBaseInfoData( LPCSTR lpszLabel, WORD wMarket)
{
	BASEINFO* pBase = NULL;
	int kind = CMainFrame::m_pDoc ->m_sharesInformation .GetStockKind(wMarket,(char*)lpszLabel);
	if(kind>=0)
	{
		PCdat1 pStockData = NULL;
		if(CMainFrame::m_pDoc ->m_sharesInformation.Lookup ((char*)lpszLabel,pStockData,kind))
		{
			pBase = pStockData->pBaseInfo;
		}
	}
	return pBase;
}
 
//     4、给定股票代码和市场类型，切换View到行情显示牌。
void CWHApp::SwitchToReportView( LPCSTR lpszLabel, WORD wMarket)
{
	CMainFrame::m_pDoc ->m_pReportView->GetParentFrame()->SendMessage(WM_SYSCOMMAND,SC_MAXIMIZE,0);
	if( NULL == lpszLabel || 0 == strlen(lpszLabel) )
		return;					//	 仅仅为了切换到行情显示牌
	int kind = CMainFrame::m_pDoc ->m_sharesInformation .GetStockKind(wMarket,(char*)lpszLabel);
	if(kind>=0)
	{
		PCdat1 pStockData = NULL;
		if(CMainFrame::m_pDoc ->m_sharesInformation.Lookup ((char*)lpszLabel,pStockData,kind))
		{			
			CMainFrame::m_pDoc ->m_pReportView->ScrollToSelectStock((char*)lpszLabel,kind);
		}
	}
}
 
//     5、给定股票代码和市场类型，切换View到日线分析。
void CWHApp::SwitchToKlineView( LPCSTR lpszLabel, WORD wMarket)
{
	int kind = CMainFrame::m_pDoc ->m_sharesInformation .GetStockKind(wMarket,(char*)lpszLabel);
	if(kind>=0)
	{
		PCdat1 pStockData = NULL;
		if(CMainFrame::m_pDoc ->m_sharesInformation.Lookup ((char*)lpszLabel,pStockData,kind))
		{
			CMainFrame::m_pDoc->m_sharesSymbol = lpszLabel;
			CMainFrame::m_pDoc->m_stkKind = kind;
			CMainFrame::m_pDoc->m_GetFocus = true;
			AfxGetMainWnd()->SendMessage (WM_USER_DrawKlineWindow,(WPARAM)lpszLabel,(LPARAM)(2+CMainFrame::m_pDoc->m_stkKind *10));
		}
	}
}
 
//     6、给定股票代码和市场类型，切换View到F10资料显示。
void CWHApp::SwitchToF10View( LPCSTR lpszLabel, WORD wMarket)
{
	int kind = CMainFrame::m_pDoc ->m_sharesInformation .GetStockKind(wMarket,(char*)lpszLabel);
	if(kind>=0)
	{
		PCdat1 pStockData = NULL;
		if(CMainFrame::m_pDoc ->m_sharesInformation.Lookup ((char*)lpszLabel,pStockData,kind))
		{
			CMainFrame::m_pDoc->m_sharesSymbol = lpszLabel;
			CMainFrame::m_pDoc->m_stkKind = kind;
			CWnd * pMainWnd = AfxGetApp()->m_pMainWnd;
			((CMainFrame *)pMainWnd)->OnCompanyinfo();
		}
	}
}
 
//     7、键盘精灵命令响应函数。即响应"05","61","62"之类的命令
void  CWHApp::OnKeyboardGeniusCmd( LPCSTR lpszCmd )
{
	if(CMainFrame::m_pDoc ->m_pReportView)
	{
		CMainFrame::m_pDoc ->m_pReportView->HotKeyDownProgress((char*)lpszCmd);
	}
}

///-----------------------------------------------
COLORREF CWHApp::GetColorScheme( int nColorIndex )
{
	COLORREF colorRetVal;

	CTaiShanDoc * pDoc = CMainFrame::m_pDoc;
	if( NULL == pDoc )
		return RGB(0,0,0);
	switch( nColorIndex )
	{
	case 0:			//	网格背景颜色
		colorRetVal = pDoc->m_colorArray[18];
		break;
	case 1:			//	网格线
		colorRetVal = pDoc->m_colorArray[17];
		break;
	case 2:			//	上涨文字颜色
		colorRetVal = pDoc->m_colorArray[13];
		break;
	case 3:			//	下跌文字颜色
		colorRetVal = pDoc->m_colorArray[15];
		break;
	case 4:			//	平的文字颜色
		colorRetVal = pDoc->m_colorArray[14];
		break;
	case 5:			//	选中的高亮颜色
		colorRetVal = pDoc->m_colorArray[16];
		break;

	default:
		ASSERT( FALSE );
	}
	
	return colorRetVal;
}

///-----------------------------------------------
BOOL CWHApp::GetFontScheme( int nFontIndex, LOGFONT & OutLogFont )
{
	return FALSE;
}


void CWHApp::OnUpdateAppAbout(CCmdUI* pCmdUI) 
{
}

void CWHApp::UpdateTheOEMInfo()
{
}
