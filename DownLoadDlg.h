//{{AFX_INCLUDES()
//}}AFX_INCLUDES
#if !defined(AFX_DOWNLOADDLG_H__6D8D3787_87C7_11D4_882A_00400552E583__INCLUDED_)
#define AFX_DOWNLOADDLG_H__6D8D3787_87C7_11D4_882A_00400552E583__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DownLoadDlg.h : header file
//
#import "ZtZip.tlb" no_namespace

/////////////////////////////////////////////////////////////////////////////
// CDownLoadDlg dialog
struct FIRSTHEAD
{
	int NumStock;
	int NumTotalBlock;
	int FixedTouchPerBlock;
	int MaxNumStock;
};

struct SECONDHEAD
{
	char StockSymbol[8];
	int NumTouch;
	int NumBlock;
	short BlockArray[16];
};


typedef enum FileAttributes{ 
	StockName=0,               //股票代码
	TodayDayLine,              //当日日线
	MonthDayLine,              //当月日线 
    HistroyDayLine,            //历史日线
	ExRightData,               //除权数据
	StockBaseInfo,             //个股资料
	NewUpdateBaseInfo,         //最近更新
	ShNewUpdateBaseInfo,       //上证最近更新
	SzNewUpdateBaseInfo,       //深证最近更新
    EbNewUpdateBaseInfo,       //二板最近更新
	StockMinute,               //个股分时
	ShZsMinute,                //上证指数
	ShAgMinute,                //上证Ａ股
	ShBgMinute,                //上证Ｂ股
	ShJjMinute,				   //上证基金
	ShKjMinute,				   //上证开放基金
	ShEfMinute,				   //上证ETF
	ShQzMinute,				   //上证权证
	ShOtMinute,				   //上证其它
	ShZqMinute,                //上证债券
	ShZzMinute,                //上证转债
	ShHgMinute,                //上证回购

	SzZsMinute,                //深证指数
	SzAgMinute,                //深证Ａ股
	SzBgMinute,                //深证Ｂ股
	SzJjMinute,                //深证基金
	SzZqMinute,                //深证债券
	SzZzMinute,                //深证转债
	SzHgMinute,                //深证回购
	SzEfMinute,                //深证ETF
	SzQzMinute,                //深证权证
	SzKjMinute,                //深证开放基金
	SzSbMinute,                //深证三板
	SzOtMinute,                //深证其它
	SzZxMinute,                //深证其它
    SzCyMinute,                //创业板

    ShInfomation,              //上证公告
	SzInfomation,              //深证公告  
    EbInfomation,              //二板公告  
	ZjInfomation,               //财经报道
    TaiInfoFiguer,
    CaiwuData,
	TaiDetailData			//明细数据
};
enum DownLoadAttributes{
	READY=0,
    CONNECTSERVER,
	CONNECTSERVERED,
	CONNECTSERVERERROR,
    NOTFINDFILE,
	DOWNLOADING,
	DOWNLOADERROR,
	DOWNLOADCOMPLETE,
	STOPDOWNLOAD,
	OPENORSAVEERROR,
	INSTALLCOMPLETE,
	NOTINSTALLCOMPLETE
};

typedef struct {
    CWnd		*pWnd;	          //窗口句柄
	char		szMemo[256];
    char        szFileName[MAX_PATH] ; 
    int         kind;
	char        szSaveFilePath[MAX_PATH];
	char        szURLFilePath[MAX_PATH];
    FileAttributes  DownloadDataType;      //下载数据类型
    LONG        DownloadDataCount;     // 下载数据字节数
    LONG        DownloadFileLength;    //下载文件总字节数
	DownLoadAttributes nDownLoadState;
	HINTERNET	hOpen;		           //HINTERNET 句柄 通过 InternetOpen 建立
	HINTERNET	hResource;	           //HINTERNET 句柄  通过 InternetOpenUrl 建立
	HANDLE		hThread;	           // 线程句柄
	DWORD		dwThreadID;	           // 线程类型 
	BOOL bResourceOpen;
	BOOL bIsStopDownload;
	BOOL bIsContinueDown;
	int  downloaderrors;
	LONG LastDownloadCount;
	int DownLoadNotChangeCount;
} REQUEST_CONTEXT;


DWORD WINAPI DownLoadThreader(LPVOID);
int WINAPI DownLoadDump(REQUEST_CONTEXT *pContext, HINTERNET);
int WINAPI DownLoadHeader(REQUEST_CONTEXT *cpContext, HINTERNET hHttp);

class CTaiKlineFileHS;
class CPlayGif;
class CDownLoadDlg : public CDialog
{
// Construction
public:
	CDownLoadDlg(CWnd* pParent = NULL);   // standard constructor
	~CDownLoadDlg();   // standard constructor

	CImageList * m_pImageList;    //大图象列表
	CImageList * m_pImageListSmall; //小图象列表
	CArray<CTaiKlineFileHS*,CTaiKlineFileHS*> m_pFileHs;
    CTaiShanDoc *m_pDoc;        //文档指针
	CTime Time_Now;    
	HINTERNET hOpen;
	//instance of the callback function
	INTERNET_STATUS_CALLBACK iscCallback;
	char m_szCurrentDir[MAX_PATH];
    BOOL m_bIsCalc;
    REQUEST_CONTEXT m_InfomationContext;
    CPlayGif *m_PlayGif;



    void FillToListCtrlHead();      //初始化CListCtrl标题列
    void FillToTreeCtrlHead();      //初始化CTreeCtrl标题列
	BOOL CheckList(char *szAddName,int nKind);
	BOOL InsertDownLoadDataToList(REQUEST_CONTEXT *pRequestContext );
	void AsyncDownLoadDirect (REQUEST_CONTEXT *prcContext);
	void GetLastDayInfomationDate(char *szLastDate);
	void AddDownLoadData(int mode);
	BOOL CloseAllDownLoad();
    void SetGifDisplay(REQUEST_CONTEXT *pInfomationContext);




	//Install DownLoad Data
	BOOL InstallStockName(REQUEST_CONTEXT *pRequestContext);
	BOOL InstallStockDayLine(REQUEST_CONTEXT *pRequestContext);
	BOOL InstallStockChuQuan(REQUEST_CONTEXT *pRequestContext);
	void InstallStockMenute(char *pFilePath) ;
	void InstallStockMunuteForType(REQUEST_CONTEXT *pRequestContext);
	BOOL InstallStockCaiwuData(REQUEST_CONTEXT *pRequestContext);
	BOOL InstallStockHsDetail(REQUEST_CONTEXT *pRequestContext);	//安装换手明细数据

	BOOL ReadHS(CString strFileName, CBuySellList &buySellList);


    BOOL CreateUrlCacheFile(REQUEST_CONTEXT *pRequestContext);
	BOOL SetUrlCacheFileSize(REQUEST_CONTEXT *pRequestContext);
    void AddDownLoadData() ;




// Dialog Data
	//{{AFX_DATA(CDownLoadDlg)
	enum { IDD = IDD_DOWNLOAD_DIALOG };
	CProgressCtrl	m_Progress;
	CTreeCtrl	m_MyTree;
	CListCtrl	m_lstStockTypeName;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDownLoadDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDownLoadDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnAddLoadData();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnClose();
	afx_msg void OnDeleteLoadData();
	afx_msg void OnDownloadStart();
	afx_msg void OnStopDownload();
	afx_msg void OnInstallLoaddata();
	afx_msg void OnLoadCollecte();
	afx_msg void OnSaveCollecte();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnDblclkSelectLoaddata(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	afx_msg LRESULT OnAddStockCode(WPARAM wParam, LPARAM lParam); //增加股票至LISTCTRL
    afx_msg LRESULT OnAddStockDayLine(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnDispalyDownLoadInfomation(WPARAM wParam, LPARAM lParam);
    afx_msg LRESULT OnPostFocus(WPARAM wParam, LPARAM lParam);


	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DOWNLOADDLG_H__6D8D3787_87C7_11D4_882A_00400552E583__INCLUDED_)
