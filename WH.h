// WH.h : main header file for the WSSTOCK2000 application
//
#include "grid.h"
#if !defined(AFX_WSSTOCK2000_H__61A31CE9_9707_11D1_ACAB_0000E823DBFD__INCLUDED_)
#define AFX_WSSTOCK2000_H__61A31CE9_9707_11D1_ACAB_0000E823DBFD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

//#define TEST_USER1

#define WM_USER_CUOHE				(WM_USER + 1)
#define WM_USER_CLEAR				(WM_USER + 2)
#define WM_USER_CUOHEDIALOG			(WM_USER + 3)
#define WM_USER_HOTKEYPROGRESS		(WM_USER + 4)
#define WM_USER_HSCROLL				(WM_USER + 5)
#define WM_USER_ALERT				(WM_USER + 6)
#define WM_USER_TJXG				(WM_USER + 7)
#define WM_USER_CHANGECOLOR			(WM_USER + 8)
#define WM_USER_CHANGEPOS			(WM_USER + 9)
#define WM_USER_CHANGESTOCK			(WM_USER + 10)
#define WM_USER_COLUMNSELECT		(WM_USER + 11)
#define WM_USER_JISHUCOLUMNSELECT	(WM_USER + 12)
#define WM_USER_NINESHOW_ROW_CHANGED (WM_USER + 13)
#define WM_USER_FROM_KLINE			(WM_USER + 100)
#define WM_USER_TELL_CHANGE			(WM_USER + 101)
#define WM_USER_DRAGDROP			(WM_USER + 102)
#define WM_USER_ADDSTOCK			(WM_USER + 1200)
#define WM_USER_ADDDAYLINE			(WM_USER + 1201)

#define WM_USER_RECVSTOCKCODE		(WM_USER + 1202)
#define WM_USER_TRANSMITINFO		(WM_USER + 1203)

#define WM_USER_COLNUM				(WM_USER + 1204)
 

#include "stdio.h"
#define MFMESBOX(info)                      ((CMainFrame *)(AfxGetApp()->m_pMainWnd))->MFLogs(info)
#define LOGS(fm, ...)                       {char tmpbuf[1024]; sprintf(tmpbuf, fm, __VA_ARGS__); MFMESBOX(tmpbuf);}



//////////////////////////////
//following message are used in nineshowview
//these messages ID is defined by Xia Qiankun
//define message ID used by CShareslistbox and CNineShowView
#define WM_USERKEYDOWN				(WM_USER + 200)
#define WM_KEYBOARDGENIUS			(WM_USER + 201) //keyboard genius message
#define WM_REFRESHNINESHOW			(WM_USER + 202) //Refresh nineshow view  
#define WM_CHANGEALLCOLOR			(WM_USER + 203) //Change all items's color in nineshowview
#define WM_USER_DrawKlineWindow		(WM_USER + 301) //Change all items's color in nineshowview
//////////////////////////////
#define BACKGROUNDCOLOR  RGB(63,63,63)
/////////////////////////////////////////////////////////////////////////////
// CWsStock2000App:
// See CWHApp.cpp for the implementation of this class
//
void DoHtmlHelp(CWnd* pWnd,int nID = 0);

class CLongString
{
public:
	static void FloatArrayToExcelString(CLongString & longStr,float* pFloat,int nTot,int nBegin);
	static CString FloatToString(float fVal);
	int GetLength();
	int m_nMax;
	int m_nCurrentNum;
	int m_nAdd;
	char* m_pData;
	void operator+(CString s);
	CLongString();
	virtual ~CLongString();
};

#define FILE_PATH_HSH_SH "data\\historysh\\"
#define FILE_PATH_HSH_SZ "data\\historysz\\"
/*     我这边需要几个函数，你帮我写一下，好吧。
 
     1、给定股票代码和市场类型，返回股票名称的字符串指针。
        virtual LPCSTR GetStkName( LPCSTR lpszLabel, WORD wMarket );
 
     2、给定日期和股票代码、市场类型，返回那天日线结构的指针，
        返回NULL表示失败。
        virtual GetLineDayData( LPCSTR lpszLabel, WORD wMarket, BOOL bDoPowerSplit = FALSE );
        bDoPowerSplit 表示是否进行还权。
 
     3、给定股票代码和市场类型，返回该股票的基本面资料（如财务数据等) 。
        返回NULL表示失败。
 
     4、给定股票代码和市场类型，切换View到行情显示牌。
 
     5、给定股票代码和市场类型，切换View到日线分析。
 
     6、给定股票代码和市场类型，切换View到F10资料显示。
 
     7、键盘精灵命令响应函数。即响应"05","61","62"之类的命令
      virtual void  OnKeyboardGeniusCmd( LPCSTR lpszCmd );
 
     这几个函数，全部在 theApp 对象中实现，且采用 virtual 方式，
     这样在我的 App 类型写上纯虚函数，即可实现。
        
     有了这几个函数，我就可以进行整合了。
     我急切盼望早日得到这几个函数，谢谢！
        

*/

class CWHApp : public CWinApp
{
public:
//     1、给定股票代码和市场类型，返回股票名称的字符串指针。
        virtual CString GetStkName( LPCSTR lpszLabel, WORD wMarket );
 
//     2、给定日期和股票代码、市场类型，返回那天日线结构的指针，
//        返回NULL表示失败。        bDoPowerSplit 表示是否进行还权。
        virtual void * GetLineDayData( LPCSTR lpszLabel, WORD wMarket, CTime tm, BOOL bDoPowerSplit = FALSE );
 
//     3、给定股票代码和市场类型，返回该股票的基本面资料（如财务数据等) 。
//      返回NULL表示失败。
        virtual void * GetBaseInfoData( LPCSTR lpszLabel, WORD wMarket);
 
//     4、给定股票代码和市场类型，切换View到行情显示牌。
        virtual void SwitchToReportView( LPCSTR lpszLabel, WORD wMarket);
 
//     5、给定股票代码和市场类型，切换View到日线分析。
        virtual void SwitchToKlineView( LPCSTR lpszLabel, WORD wMarket);
 
//     6、给定股票代码和市场类型，切换View到F10资料显示。
        virtual void SwitchToF10View( LPCSTR lpszLabel, WORD wMarket);
 
//     7、键盘精灵命令响应函数。即响应"05","61","62"之类的命令
        virtual void  OnKeyboardGeniusCmd( LPCSTR lpszCmd );
	 	virtual COLORREF GetColorScheme( int nColorIndex );
		virtual BOOL GetFontScheme( int nFontIndex, LOGFONT & OutLogFont );

public:
	void ToTerminateChildProcess();
	static bool m_gbUseExe;
	static bool m_gbDoInitate;
	static int m_gMessageID ;
	static void DispatchMessageEachTime();
	//virtual BOOL PreTranslateMessage(MSG* pMsg);
	CWHApp();
	HANDLE hAppMutex;
    CMultiDocTemplate* pDocTemplate;
	///////////Add by zjh
	CMultiDocTemplate* pDrawTemplate;
	CMultiDocTemplate *m_pMyNewTemplate;
//	CMultiDocTemplate* m_pNewDocTemplate;
//    CMultiDocTemplate* m_pF9DocTemplate;
	CMultiDocTemplate* m_pBroadcastDocTemplate;
// add a template used by NineShowView
	CMultiDocTemplate* pNineShowTemplate;
	CMultiDocTemplate* m_pBlockAnalysisTemplate;
	int Line_mode;
	int m_key;
	CTime t;
	BOOL m_bAppAuthorized;	//产品是否被授权
	//////////////////////
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWHApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	virtual BOOL OnIdle(LONG lCount);
	virtual int Run();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CWHApp)
	afx_msg void OnAppAbout();
	afx_msg void OnFilePrintSetup();
	afx_msg void OnUpdateAppAbout(CCmdUI* pCmdUI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	void UpdateTheOEMInfo();
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WSSTOCK2000_H__61A31CE9_9707_11D1_ACAB_0000E823DBFD__INCLUDED_)
