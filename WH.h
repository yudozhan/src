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
/*     �������Ҫ���������������дһ�£��ðɡ�
 
     1��������Ʊ������г����ͣ����ع�Ʊ���Ƶ��ַ���ָ�롣
        virtual LPCSTR GetStkName( LPCSTR lpszLabel, WORD wMarket );
 
     2���������ں͹�Ʊ���롢�г����ͣ������������߽ṹ��ָ�룬
        ����NULL��ʾʧ�ܡ�
        virtual GetLineDayData( LPCSTR lpszLabel, WORD wMarket, BOOL bDoPowerSplit = FALSE );
        bDoPowerSplit ��ʾ�Ƿ���л�Ȩ��
 
     3��������Ʊ������г����ͣ����ظù�Ʊ�Ļ��������ϣ���������ݵ�) ��
        ����NULL��ʾʧ�ܡ�
 
     4��������Ʊ������г����ͣ��л�View��������ʾ�ơ�
 
     5��������Ʊ������г����ͣ��л�View�����߷�����
 
     6��������Ʊ������г����ͣ��л�View��F10������ʾ��
 
     7�����̾���������Ӧ����������Ӧ"05","61","62"֮�������
      virtual void  OnKeyboardGeniusCmd( LPCSTR lpszCmd );
 
     �⼸��������ȫ���� theApp ������ʵ�֣��Ҳ��� virtual ��ʽ��
     �������ҵ� App ����д�ϴ��麯��������ʵ�֡�
        
     �����⼸���������ҾͿ��Խ��������ˡ�
     �Ҽ����������յõ��⼸��������лл��
        

*/

class CWHApp : public CWinApp
{
public:
//     1��������Ʊ������г����ͣ����ع�Ʊ���Ƶ��ַ���ָ�롣
        virtual CString GetStkName( LPCSTR lpszLabel, WORD wMarket );
 
//     2���������ں͹�Ʊ���롢�г����ͣ������������߽ṹ��ָ�룬
//        ����NULL��ʾʧ�ܡ�        bDoPowerSplit ��ʾ�Ƿ���л�Ȩ��
        virtual void * GetLineDayData( LPCSTR lpszLabel, WORD wMarket, CTime tm, BOOL bDoPowerSplit = FALSE );
 
//     3��������Ʊ������г����ͣ����ظù�Ʊ�Ļ��������ϣ���������ݵ�) ��
//      ����NULL��ʾʧ�ܡ�
        virtual void * GetBaseInfoData( LPCSTR lpszLabel, WORD wMarket);
 
//     4��������Ʊ������г����ͣ��л�View��������ʾ�ơ�
        virtual void SwitchToReportView( LPCSTR lpszLabel, WORD wMarket);
 
//     5��������Ʊ������г����ͣ��л�View�����߷�����
        virtual void SwitchToKlineView( LPCSTR lpszLabel, WORD wMarket);
 
//     6��������Ʊ������г����ͣ��л�View��F10������ʾ��
        virtual void SwitchToF10View( LPCSTR lpszLabel, WORD wMarket);
 
//     7�����̾���������Ӧ����������Ӧ"05","61","62"֮�������
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
	BOOL m_bAppAuthorized;	//��Ʒ�Ƿ���Ȩ
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
