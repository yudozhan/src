#include "afxtempl.h"
#include "StructTaiShares.h"	// Added by ClassView

#if !defined(AFX_GSGLLSSJ_H__804D1366_B12B_11D1_83E6_0000E8593F1A__INCLUDED_)
#define AFX_GSGLLSSJ_H__804D1366_B12B_11D1_83E6_0000E8593F1A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// GSGLLSSJ.h : header file
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
struct KLINEHEAD_cw
{
	int FileID;                 //大文件头
	int	NumPerBlk	;			//每小块的股票K线数据数量,256天
	int Free1;                  //预留1 
	int	NumStock	;			//证券总数
	int	NumBlockAll;			//小块总数
	int Free2;					//预留2 
};


///////////小文件头
struct KLINESMALLHEAD_cw
{
	char    StockSign[8]; //前8个字节为股票代码。
	short 	numChuQ	;	  //2个字节为除权数据的总次数
	long    numKln	;	  //4个字节整型，代表日K线的个数。
	short   symBlock[25]; //下面每两个字节代表K线数据的块号
};
/////////
//除权数据结构
struct POWER_cw       //共32个字节
{
	BYTE  byFree[12];  //预留12个字节
	int   nTime;
	float fGive;
	float fAllocate;
	float fAllocatePrice;
	float fDividend;
};
////////////
///////////5分钟线头文件结构
struct MINUTEKLINEHEAD_cw
{
	char    StockSign[8]; //前8个字节为股票代码。
	long    numKln	;	  //4个字节整型，代表分钟线的个数。
	long    pPos;         //写入分钟线指针
	long    symBlock;     //块号
};
////////////////////////////////
//5分钟结构
//1--4 字节 文件头   
//5--8 每小块5分钟K线数据数量 储存75天 
//9--12 上证证券总数
//13--16 保留
struct MKLINEHEAD_cw
{
	int IDFile;
	int	numPerBlk	;			//每小块的股票5分钟K线数据数量,75*48天
	int	numKline_sh	;			//上证证券总数
	//int numKline_sz	;	        //深证证券总数
	int unKnown;
};

/////////////////////////////////////////////////////////////////////////////
// CGSGLLSSJ dialog
class CTaiShanDoc;
class CGSGLLSSJ : public CPropertyPage
{
	DECLARE_DYNCREATE(CGSGLLSSJ)

// Construction
public:
	CTaiShanDoc* pDoc;

	CString DestinationFile;
	CString SourceFileName;
	CArray <Kline,Kline &> m_InstallArray;
	
	CGSGLLSSJ();
	~CGSGLLSSJ();

// Dialog Data
	//{{AFX_DATA(CGSGLLSSJ)
	enum { IDD = IDD_LSSJGL };
	CProgressCtrl	m_ProgressYield;
	CButtonST	m_check_fixtime;
	CEdit	m_destination;
	CButtonST	m_SearchFixData;
	CEdit	m_DataSource;
	CComboBox	m_ComboDatatype;
	CProgressCtrl	m_progress1;
	BOOL	m_sh;
	BOOL	m_sz;
	int		m_Cover;
	int		m_AssignPeriod;
	int		m_NumberOfDays;
	CTime	m_install_start;
	CTime	m_install_end;
	CTime	m_produce_end;
	CTime	m_produce_start;
	//}}AFX_DATA

public:
	BOOL IsFileExists( CString filename);
	void SplitOneFileToManyFiles(CString Dir,CString DestFileName,bool bDayKline = true);
	BOOL InstallFromFloppyDisk(CString FloppyFileName,bool bDayKline = true);
	void Produce2FloppyDisk(CString DestFileName,CString FloppyFileName);

	CFile m_OutMinuteFile;
	CFile m_LocalFile;
	CFile m_DestFile;
	CFile m_file;
	KLINEHEAD_cw m_BigFileHead;
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
	//{{AFX_VIRTUAL(CGSGLLSSJ)
	public:
	virtual void OnOK();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CGSGLLSSJ)
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonSearchfixdata();
	afx_msg void OnButtonBeginInstall();
	afx_msg void OnRadioRepaire();
	afx_msg void OnButtonDestination();
	afx_msg void OnButtonBeginproduce();
	afx_msg void OnRadioDay();
	virtual void OnCancel();
	afx_msg void OnSelchangeComboDatatype();
	afx_msg void OnRadioInstalldate();
	afx_msg void OnRadioCover();
	afx_msg void OnRadioAssignperiod();
	afx_msg void OnRadioRecentdays();
	afx_msg void OnSelchangeIntype();
	afx_msg void OnCloseupOuttype();
	afx_msg void OnChangeEditAssigndays();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

///////////////////////

public:
	int YieldMinuteFile(bool bShangHai = true,bool bAssignDays = true);
	BOOL ShenglongToTianlong(SHENGLONG shenglong, Kline& wanshen );
//	int InstallFromSeveralDiskfile_min(CString FileName);
//	void SplitOneFileToManyFiles_min(CString Dir, CString DestFileName);
	void InstallShenglong(CString srcfilename,int BeginDate, int EndDate);
	int InstallFromSeveralDiskfile(CString FileName,bool bDayKline = true);
    void InstallMinteData();
    void ProduceMinuteData() ;
private:
	void InstallStockKlineEach(CString symbol,int BeginDate,int EndDate,bool bDayKline = true);
};



//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GSGLLSSJ_H__804D1366_B12B_11D1_83E6_0000E8593F1A__INCLUDED_)
