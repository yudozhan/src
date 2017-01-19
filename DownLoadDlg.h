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
	StockName=0,               //��Ʊ����
	TodayDayLine,              //��������
	MonthDayLine,              //�������� 
    HistroyDayLine,            //��ʷ����
	ExRightData,               //��Ȩ����
	StockBaseInfo,             //��������
	NewUpdateBaseInfo,         //�������
	ShNewUpdateBaseInfo,       //��֤�������
	SzNewUpdateBaseInfo,       //��֤�������
    EbNewUpdateBaseInfo,       //�����������
	StockMinute,               //���ɷ�ʱ
	ShZsMinute,                //��ָ֤��
	ShAgMinute,                //��֤����
	ShBgMinute,                //��֤�¹�
	ShJjMinute,				   //��֤����
	ShKjMinute,				   //��֤���Ż���
	ShEfMinute,				   //��֤ETF
	ShQzMinute,				   //��֤Ȩ֤
	ShOtMinute,				   //��֤����
	ShZqMinute,                //��֤ծȯ
	ShZzMinute,                //��֤תծ
	ShHgMinute,                //��֤�ع�

	SzZsMinute,                //��ָ֤��
	SzAgMinute,                //��֤����
	SzBgMinute,                //��֤�¹�
	SzJjMinute,                //��֤����
	SzZqMinute,                //��֤ծȯ
	SzZzMinute,                //��֤תծ
	SzHgMinute,                //��֤�ع�
	SzEfMinute,                //��֤ETF
	SzQzMinute,                //��֤Ȩ֤
	SzKjMinute,                //��֤���Ż���
	SzSbMinute,                //��֤����
	SzOtMinute,                //��֤����
	SzZxMinute,                //��֤����
    SzCyMinute,                //��ҵ��

    ShInfomation,              //��֤����
	SzInfomation,              //��֤����  
    EbInfomation,              //���幫��  
	ZjInfomation,               //�ƾ�����
    TaiInfoFiguer,
    CaiwuData,
	TaiDetailData			//��ϸ����
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
    CWnd		*pWnd;	          //���ھ��
	char		szMemo[256];
    char        szFileName[MAX_PATH] ; 
    int         kind;
	char        szSaveFilePath[MAX_PATH];
	char        szURLFilePath[MAX_PATH];
    FileAttributes  DownloadDataType;      //������������
    LONG        DownloadDataCount;     // ���������ֽ���
    LONG        DownloadFileLength;    //�����ļ����ֽ���
	DownLoadAttributes nDownLoadState;
	HINTERNET	hOpen;		           //HINTERNET ��� ͨ�� InternetOpen ����
	HINTERNET	hResource;	           //HINTERNET ���  ͨ�� InternetOpenUrl ����
	HANDLE		hThread;	           // �߳̾��
	DWORD		dwThreadID;	           // �߳����� 
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

	CImageList * m_pImageList;    //��ͼ���б�
	CImageList * m_pImageListSmall; //Сͼ���б�
	CArray<CTaiKlineFileHS*,CTaiKlineFileHS*> m_pFileHs;
    CTaiShanDoc *m_pDoc;        //�ĵ�ָ��
	CTime Time_Now;    
	HINTERNET hOpen;
	//instance of the callback function
	INTERNET_STATUS_CALLBACK iscCallback;
	char m_szCurrentDir[MAX_PATH];
    BOOL m_bIsCalc;
    REQUEST_CONTEXT m_InfomationContext;
    CPlayGif *m_PlayGif;



    void FillToListCtrlHead();      //��ʼ��CListCtrl������
    void FillToTreeCtrlHead();      //��ʼ��CTreeCtrl������
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
	BOOL InstallStockHsDetail(REQUEST_CONTEXT *pRequestContext);	//��װ������ϸ����

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
	afx_msg LRESULT OnAddStockCode(WPARAM wParam, LPARAM lParam); //���ӹ�Ʊ��LISTCTRL
    afx_msg LRESULT OnAddStockDayLine(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnDispalyDownLoadInfomation(WPARAM wParam, LPARAM lParam);
    afx_msg LRESULT OnPostFocus(WPARAM wParam, LPARAM lParam);


	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DOWNLOADDLG_H__6D8D3787_87C7_11D4_882A_00400552E583__INCLUDED_)
