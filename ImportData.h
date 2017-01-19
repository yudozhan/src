#if !defined(AFX_IMPORTDATA_H__B5C1E14C_A053_4797_845A_C1BA8330EEA6__INCLUDED_)
#define AFX_IMPORTDATA_H__B5C1E14C_A053_4797_845A_C1BA8330EEA6__INCLUDED_
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ImportData.h : header file
#define WM_DRAWLIST WM_USER+131
//
class CTaiShanDoc;
/////////////////////////////////////////////////////////////
//钱龙数据存储格式, 40个字节
struct QIANLONG
{
	DWORD dwDate;          // 日  期：  19980219 即1998/02/19]
	DWORD dwOpen;          // 开盘价：   单位：元*1000
	DWORD dwHigh;          // 最高价：   单位：元*1000
	DWORD dwLow;           // 最低价：   单位：元*1000
	DWORD dwClose;         // 收盘价：   单位：元*1000
	DWORD dwAmount;        // 成交额：   单位：元/1000  （取整或四舍五入）
	DWORD dwVolume;        // 成交量：   单位：手数
	DWORD dwUnknow;        // 未知量：   置 0  预留4字节的空间
    WORD  wUp;             // 上涨家数   (只对大盘)
    WORD  wDown;           // 上涨家数   (只对大盘)
    DWORD dwGuben;         // 总股本
};

/////////////////////////////////////////////////////////////
//分析家数据存储格式 32 个字节
struct FENXIJIA
{
	DWORD dwDate;    //时  间：整形（秒数）
	float fOpen;     //开盘价：float
	float fHigh;     //最高价：float
	float fLow ;     //最低价：float
	float fClose;    //收盘价：float
	float fAmount;   //成交额：float
	float fVolume;   //成交量：float
	float fUnknown;  //未知的四个字节：

};

/////////////////////////////////////////////////////////////
//our数据存储格式, 共有32个字节
/*
struct TAI_SHAN_DATA_cw
{
		int	    nTime;      // 日  期，秒数  
		float	fOpen;		// 开盘价
		float	fHigh;		// 最高价
		float	fLow ;		// 最低价
		float 	fClose;     // 收盘价
		float 	fVolume;    // 成交量
		float	fAmount;    // 成交金额
		WORD    wAdvance;   // 上涨家数
		WORD    wDecline;   // 下跌家数 
};
*/

/////////////////////////////////////////////////////////////
//汇金数据存储格式 共29个字节
struct  HUIJIN
{
	int   nDate;         // 日  期：  19980219 即1998/02/19]
	float fOpen;          // 开盘价：   
	float fHigh;          // 最高价：   
	float fLow;           // 最低价：   
	float fClose;         // 收盘价：   
	float fMean;          // 成交额：   
	int   nVolume;        // 成交量：  单位是股，而不是手 
	BYTE  chUnknown;      // 一个字节
};

//胜龙的数据结构.
struct SHENGLONG
{
	long  date;
	long  oprice;	
	long  cprice;
	long  hprice;	
	long  lprice;
	long  tovalue;	
	long  tohands;
	long  topiece;
	char  free[8];
};

////////////////////////////////////////////////
///大文件头
/*struct KLINEHEAD_cw
{
	int FileID;                 //大文件头
	int	NumPerBlk	;			//每小块的股票K线数据数量,256天
	int Free1;                  //预留1 
	int	NumStock	;			//证券总数
	int	NumBlockAll;			//小块总数
	int Free2;					//预留2 
};
*/

///////////小文件头
/*
struct KLINESMALLHEAD_cw
{
	char    StockSign[8]; //前8个字节为股票代码。
	short 	numChuQ	;	  //2个字节为除权数据的总次数
	long    numKln	;	  //4个字节整型，代表日K线的个数。
	short   symBlock[25]; //下面每两个字节代表K线数据的块号
};
*/

/////////
//除权数据结构
/*
struct POWER_cw       //共32个字节
{
	BYTE  byFree[12];  //预留12个字节
	int   nTime;
	float fGive;
	float fAllocate;
	float fAllocatePrice;
	float fDividend;
};
*/
////////////
///////////5分钟线头文件结构delete 2004-11-16
/*struct MINUTEKLINEHEAD_cw
{
	char    StockSign[8]; //前8个字节为股票代码。
	long    numKln	;	  //4个字节整型，代表分钟线的个数。
	long    pPos;         //写入分钟线指针
	long    symBlock;     //块号
};*/
////////////////////////////////
//5分钟结构
//1--4 字节 文件头   
//5--8 每小块5分钟K线数据数量 储存75天 
//9--12 上证证券总数
//13--16 保留

/*
struct MKLINEHEAD_cw
{
	int IDFile;
	int	numPerBlk	;			//每小块的股票5分钟K线数据数量,75*48天
	int	numKline_sh	;			//上证证券总数
	//int numKline_sz	;	        //深证证券总数
	int unKnown;
};
*/

typedef struct
{
	    float 	Price;      //    价格 
		float 	Volume;     //    量 
		float 	Amount;     //    额
        float   accb;       //    买入挂单和（三种买入挂单和）
        float   accs;  
}KdataOld;

//Cdat2结构主要用于即日即时股票行情信息(Ver2.0)
struct  CdatOld
{
 BYTE kind;
 BYTE  rdp[240];           //      用于计算大盘强弱指标
 char id[8];             //      证券代码
 char name[10];           //      证券名称 
 char Gppyjc[6];         //      证券名称拼音简称
 int   sel;
 float ystc;              //      昨天收盘
 float opnp;              //      今日开盘 
 float higp;              //      今日最高
 float lowp;              //      今日最低
 float nowp;              //      最新价格
 float nowv;              //      最新成交量
 float totv;              //      总成交量
 float totp;              //      总成交金额
 float pbuy1;             //      买入价一
 float vbuy1;             //      买入量一
 float pbuy2;             //      买入价二  
 float vbuy2;             //      买入量二
 float pbuy3;             //      买入价三 
 float vbuy3;             //      买入量三      
 float pbuy4;             //      买入价三 
 float vbuy4;             //      买入量三      
 float pbuy5;             //      买入价三 
 float vbuy5;             //      买入量三      
 float psel1;             //      卖出价一
 float vsel1;             //      卖出量一
 float psel2;             //      卖出价二 
 float vsel2;             //      卖出量二   
 float psel3;             //      卖出价三
 float vsel3;             //      卖出量三 
 float psel4;             //      卖出价三
 float vsel4;             //      卖出量三 
 float psel5;             //      卖出价三
 float vsel5;             //      卖出量三 
 float accb;              //      买入挂单和（三种买入挂单和）
 float accs;              //      卖出挂单和（三种卖出挂单和）
 float volume5;           //      5天均量
 float rvol;              //      外盘
 float dvol;              //      内盘
 
 short lastclmin;         //      上次处理分钟数
 short initdown;          //      股票初始化标志   0 = 未能初始化，1=已初始化   
 BOOL  InOut;
 
 float StartP;        //      开始时间价格 
 float EndP;          //      结束时间价格
 float Res[11];
 KdataOld   m_Kdata[240];
 int      nReservation;
};



class CTaiShanDoc;
class CImportData : public CPropertyPage
{
	DECLARE_DYNCREATE(CImportData)

// Construction
public:
    CTaiShanDoc * pDoc;
	CArray<BOOL,BOOL> m_stockrange;
	CString DestinationFile;
	CString SourceFileName;
	CArray <Kline,Kline &> m_InstallArray;

	CImportData();
	~CImportData();

// Dialog Data 
	//{{AFX_DATA(CImportData)
	enum { IDD = IDD_DATAIMPORT };
	CButton	m_button1;
	CDateTimeCtrl	m_ctrlTimeEnd;
	CDateTimeCtrl	m_ctrlTimeStart;
	CListCtrl	m_listfw;
	CButton	m_okbutton;
	CProgressCtrl	m_progress1;
	CComboBox	m_ComboDatatype;
	CComboBox	m_ComboFiletype;
	CEdit	m_DataSource;
	int m_importAll;
	CTime	m_install_end;
	CTime	m_install_start;
	//}}AFX_DATA
public:
    BOOL IsFileExists(CString filename);
    void SplitOneFileToManyFiles(CString Dir,CString DestFileName,bool bDayKline =true);
    BOOL InstallFromFloppyDisk(CString FloppyFileName,bool bDayKline=true);
	void Produce2FloppyDisk(CString DestFileName,CString FloppyFileName);
 
	CFile m_OutMinuteFile;
	CFile m_LocalFile;
	CFile m_DestFile;
	CFile m_file;
	//KLINEHEAD_cw m_BigFileHead;
    char* file_sz;
	char* file_sh;
   

	void YieldDay(CString filename, int &NumberOfStock);
	void AssignDays(CString filename, int &NumberOfStock);  //指定天数生成

	LONG GetStockDay(time_t NumSeconds );
	int myf(CString szStr);

	BOOL GetStockTime(int ymd, LONG& tm);
	void InstallHuijin  (CString srcfilename, int BeginDate, int EndDate); 
	void InstallQianlong(CString srcfilename, int BeginDate, int EndDate); 
	void InstallWanshen (CString srcfilename, int BeginDate, int EndDate); 
	void InstallFenxijia(CString srcfilename, int BeginDate, int EndDate); 
	
// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CImportData)
	public:
	virtual void OnOK();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CImportData)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangedatatype();
	afx_msg void OnSelchangedataformat();
	afx_msg void OnSetfocusfwxz();
	afx_msg void OnSetfocus();
	afx_msg void Onqbyr();
	afx_msg void Onsdyr();
	afx_msg void OnSetfocusup();
	afx_msg void OnSetfocusdown();
	afx_msg void Onimport();
	afx_msg void OnButtonSearchfixdata();
	afx_msg void OnClickList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
	//}}AFX_MSG
	//afx_msg void drawzqsc(WPARAM,LPARAM);
	DECLARE_MESSAGE_MAP()


public:
	int IsFileType(CString Filename);
	BOOL m_bImport;
	static void InstallOneMinStock(CString FilePath);
	CString m_news;
	BOOL m_Shenzhen;
	BOOL m_Shanghai;
	int m_nImportorcancel;
	int m_nFlag;
	int YieldMinuteFile(bool bShangHai = true,bool bAssignDays = true);
	BOOL ShenglongToTianlong(SHENGLONG shenglong, Kline& wanshen );
//	int InstallFromSeveralDiskfile_min(CString FileName);
//	void SplitOneFileToManyFiles_min(CString Dir, CString DestFileName);
	void InstallShenglong(CString srcfilename,int BeginDate, int EndDate);
	int InstallFromSeveralDiskfile(CString FileName,bool bDayKline = true);
    void InstallMinteData();
    void InstallMin(CString lpStockSymbol);
	void ProduceMinuteData() ;

private:
	void InstallStockKlineEach(CString symbol,int wMarketType,int BeginDate,int EndDate,bool bDayKline = true);


};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_IMPORTDATA_H__B5C1E14C_A053_4797_845A_C1BA8330EEA6__INCLUDED_)
