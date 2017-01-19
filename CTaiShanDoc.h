// CTaiShanDoc.h : interface of the CTaiShanDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_WSSTOCK2000DOC_H__61A31CF1_9707_11D1_ACAB_0000E823DBFD__INCLUDED_)
#define AFX_WSSTOCK2000DOC_H__61A31CF1_9707_11D1_ACAB_0000E823DBFD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "StructKlineView.h"
#include <afxmt.h>
#include "CFormularCompute.h"
#include <afxdao.h>
#include "CFormularContent.h"
#include "NineShowView.h"
#include "MyTreeView.h"
#include "F10HeadView.h"
#include "InfoView.h"
#include "hzpy.h"	//Added by ltr 2004-11-30
#include "BlockAnalysisView.h"

#define  systemcolorlength		34
#define  comblength				14
#define DLINEHEAD				24
#define DLINESMALLHEAD			4096 * 64
#define DLINECHUQ				32 * 6
#ifdef WIDE_NET_VERSION
#define MAXCOLUME  25
#define INITCOLUME 20
#define INITDATAID 8888
#else
#define MAXCOLUME  30
#define INITCOLUME 25
#define INITDATAID 9999
#endif


struct DATA_KEYBOARD {
	char    key[STK_PYJC_LEN];    //关键字
	char    data[STK_NAME_LEN];   //信息字
	BYTE    id;                   //类别
	BYTE	m_stkKind;
};

//此处定义系统的基本设置中选择的部分 	
struct  SYSTEM_OPTION {
	int 		kline;		    //主图方式，K线1、美国线0、宝塔线2
	BOOL 		fourgraph;		//多图组合，4图、9图、16图
	BOOL		autoclose;		//自动做收盘作业
	BOOL		autominute;		//自动补分钟线
	BOOL		autoday;        //自动补日线
	BOOL        showxline;		//显示横坐标
	BOOL        showyline;      //显示纵坐标
	BOOL        organizedata;   //收盘时自动进行数据整理
	BOOL        alarmsound;     //警示比声音开关
};

//此处定义系统的基本设置
struct SYSTEMDATA {
	 int        changepage;     //自动换页时间
	 int        AdvSpTimes ;    //涨速分钟数
	 int        fiveminutesave; //5分钟资料存储时间
	 int        noticesave;     //公告资料保存时间
	 int        middleprice;    //中价计算方式
	 float      buyrate;        //买卖交易费率
	 int        AlarmPercent;   //警示比
 };
//记录路径
struct SETCURRENTDIRECTORY{
    char CurrentDir[255];
	char Free[255];
};
struct SYSTEMHQSET{
	 int       capital;       //资金
	 int       quangtity;     //量比
	 int       swing;         //震幅
	 int       upspeed;       //涨速
	 int       changehand;    //换手率
 };

struct ALERTCHOICE{
	 float Vol;//成交手数
	 float Per;//流通盘百分比
	 float Acount;//成交金额
	 int Format;//统计方法选择  0：成交手数 1 流通盘百分比 2 成交金额
 };
 
 ////////property set 结构：
 typedef struct{
	 int        daysOfDayKline;			//日线保存天数
	 int        daysOfMin5Kline ;		//5分钟线保存天数
	 int        daysOfManyKline;		//多日线的天数,负数为多分钟线分钟数
	 int        countOfHistoryFile;		//历史回忆数据保存天数
	 int		nSecondsAlarm;			//预警时间间隔
	 int		nTestUnfinished;		//未完成的收益测试

	 BOOL       bDoPowerSelectingStock;	//条件选股计算时是否进行除权
	 BOOL       bDoVolPowering;			//是否对成交量,成交金额进行除权
	 BOOL		bDoPowerAfter;			// 是否采用后除权
	 BOOL       bAutoPower;				//是否自动除权
	 BOOL		bSaveParam;				//自动保存指标参数
	 BOOL		bHideLine;				//趋势线自动隐藏
	 BOOL		bShowCrossDlg;			//在十字光标状态下，是否弹出行情窗口显示该日行情
	 BOOL		bRunAlarmBegin;			//［开机自动启动预警］开机后系统自动启动预警，若未设置必须进入预警窗口手工启动。
	 BOOL		bUseMin5ChenBen;		//是否使用5分钟线计算移动成本分布
     int        bTjxgZsFs;				//条件选股指示的方式
	 float		LTP_rate;				//流通盘比例
	 float		CJL_rate;				//成交量比例
	 float      fReserved[1];        //

 }PROPERTY_INIT;




//////////////// 
 typedef struct{
		float	    yFloatBottom;		//K线模块中每个子图矩形的底部的相对位置。
		char		nameIndex[10];		//每个子图的技术指标的名字。
 }Index_INIT;

////////INFOINIT 结构：
 typedef struct{
		int				nCountIndex;		//子图的个数
		Index_INIT		initIndex[4];		//四个子图的初始化信息。	四个子图的相对高度的和为1。

		int				nCountMin1;			//子图的个数
		Index_INIT		initMin1[4];		//四个子图的初始化信息。	四个子图的相对高度的和为1。
		int				flag[4]	;			//每个子图的指标的ID号。0为分时走势，
											//1为成交量，2为买卖力道，3为量比指标，
											//4为领先指标，5为多空指标
		int				flag_dapan[4]	;	//沪指、深指的每个子图的指标的ID号。
 }INFOINIT;

 //此处定义股票代码的结构，用于自选股、板块股的选择
typedef struct StockSymbolStr {
	char   sharesymbol[7];
	int    nKind;
}SharesSymbol;

 //此处定义板块股的结构
 struct  BLOCKSTOCK{
	CString   blockname;
	CList<SharesSymbol*,SharesSymbol*> stocklist;
    BLOCKSTOCK(CString string);
	void AddStockToTail(SharesSymbol* sharesymbol);
	void AddStockToHead(SharesSymbol* sharesymbol);
 public:
	 SharesSymbol* GetAt(POSITION position);
};

struct	SCREEN_DETAIL           
{
	time_t buytime;
	time_t selltime;
	float  buyprice;//买入价
	float  sellprice;//卖出价
	int    timerange;//时间跨度
	float  profit;//收益率
	int   issuccess;//0 是不成功,1 是成功,2是没有完成周期的计算
	int	nKindSell;//卖出的种类,0:卖出条件卖出,1:附加条件卖出
};

struct  SCREEN_RESULT
{
	CString symbol;
	CString name;
	int		m_stkKind;
	int     choosecount;
	int     successcount;
	float   successpercent;
	BOOL    isuseful;
	int		nKindBuySell;//0：买入条件，1：卖出条件，2：买入卖出条件//used by  

//	union{
		CArray <SCREEN_DETAIL,SCREEN_DETAIL> m_detail;
//		CArray <SCREEN_DETAIL,SCREEN_DETAIL> m_detailArray;
//	};

public:
	void CalculateData();
};


enum CUOHETYPE 
{
	None,StockId_ASh,StockId_BSh,StockId_ASz,StockId_BSz,ChooseId,StockTypeId
};


typedef CTypedPtrMap< CMapStringToString , CString , CString > CMapNewsFileName ;
typedef CTypedPtrList<CPtrList,HS*> CHSList; 
typedef CTypedPtrList<CPtrList,FENJIA*> CFJList; 
class CTaiShanReportView;
class CTaiShanTesting;
class CTaiShanKlineShowView;

typedef CTypedPtrMap<CMapStringToPtr,CString,CPoint*> CMapStringToCP;

//yhh
#define  FONTSTRUCTLEN    4

const int      listlength=17;
class CProgressDialog;
typedef CArray<CTaiShanKlineShowView*,CTaiShanKlineShowView*> VIEWARRAY;
class CTaiKlineFileKLine;
class CTaiKlineFileHS;
class CSharesBigBillDlg;
class CSharesBigBillData;
class CDownLoadDlg;
class CDownLoadNetDlg;
class CTaiShanDoc : public CDocument
{
protected: // create from serialization only
	CTaiShanDoc();
	DECLARE_DYNCREATE(CTaiShanDoc)
	
	CString m_CurrentWorkDirectory;

// Attributes
public:
	int m_nColumnWidth[MAXCOLUME];
//  begin
	//  12.2
	long m_TodayDate;
	PROPERTY_INIT m_propertyInitiate;
	SUB_FIGUER_INIT_INFO	m_propertyInitiate2;	//初始化信息
	VIEWARRAY m_arKLineViews;
	CString	m_sShF10;
	CString	m_sSzF10;
	CString	m_sNews;
	CTaiTestTreeView *	m_taiViewF9;
    CTaiTestTreeView *	m_taiViewF10;
	CF10HeadView *		m_F10HeadView;
//  end

	CSpz m_spz;						//处理生僻字拼音
	CDyz m_dyz;						//处理多音字拼音
	static BOOL m_bAppAuthorized;	//产品是否被授权
	CString m_strPName;			//产品名称
	CString m_strUser;			//用户名
	CString m_strCompany;		//公司名
	CString m_strSerial;		//注册号
	CString m_strCredit;		//授权号
	CTaiShanTesting* m_pPingCe;	//


	INFOINIT	m_infoInit;	//初始化信息
	CTypedPtrArray<CPtrArray,NAMESYMBOL*> m_namesymbol;//系统对照表的ARRAY
	SETCURRENTDIRECTORY m_Directory; 
	CArray<CString,CString&>		index_keyb_f;	//键盘数字索引
	CArray<CString,CString&>		index_keyb_a;	//键盘股票拼音索引

	COLORREF  m_colorArrayDefault[13][34]; //系统配色方案
	COLORREF  m_colorArray[34];  //基本属性设置中的颜色数组
	LOGFONT   m_fontdefault;           //系统缺省的字体
	LOGFONT   m_fontstr[FONTSTRUCTLEN]; //基本属性设置中的字体数组
	CArray<SCREEN_RESULT*,SCREEN_RESULT*> m_chooseresult;

// Operations
public:
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTaiShanDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	virtual void OnCloseDocument();
	virtual void DeleteContents();
    /*virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);*/
	//}}AFX_VIRTUAL

// Implementation
public:
	void* LookUpArray(CFormularArray& js,CString& str);
	CString m_sharesSymbol;
    int m_stkKind;
	bool m_GetFocus;      /////////Get focus

	CImageList  *m_imagelist;          //系统的图像列表
	CBitmap       m_bitmap;            //位图对象

    CFormularArray         m_formuar_index;          //技术指标公式的ARRAY
	CFormularArray         m_formuar_choose;         //条件选股公式的ARRAY
	CFormularArray         m_formuar_master;       //专家系统公式的ARRAY
	CFormularArray         m_formuar_kline;          //K线指标的ARRAY

	bool ChooseFileExist;              ////////if true the file exist else false//
	bool m_bAddKline;
	
	SYSTEM_OPTION  m_systemOption;        //系统设置中的选择项
	SYSTEMDATA    m_systemdata;          //系统设置中的数据项
	SYSTEMHQSET   m_syshqset;            //系统设置中的即时行情设置
    ALERTCHOICE   m_dadanchoice;          //系统设置中的统计方式设置
 
public:

	virtual ~CTaiShanDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
public:
	void ClearRealData();
	int m_4or6;


	//These function are added by Xia Qiankun 2004/1/26
	//These function are set to refresh or reset keyboard angel data
	void RefreshKeyBoardAngel(BOOL IsAddStock);
	void AddStockToKeyboard(CString strStockCode,int stkKind,CString strStockName,CString strStockHypy);
	BOOL DeleteStockFromKeyboard(CString strStockCode,int stkKind,CString strStockName,CString strStockHypy);
	BOOL ModifyStockOfKeyboard(CString strStockCode,int stkKind,CString strStockName,CString strStockHypy);
    int GetStockKind(CString strKind);
    CString GetStockKindString(int nKind);

	SYSTEMINITDATA m_SystemInitData;
	long m_lDay;                                                //记录当天日期时间 
	BOOL m_bInitDone;											//记录当天初始化完成标志
	BOOL m_bInitCurrDone;										//记录目前正在做初始化标志
	BOOL m_bCloseWorkDone; 										//记录当天收盘作业情况
	int m_nBlockCalcCounter;
	int m_nBlockCalcLastMin;

	long m_lStockTypeCount[13];                                 //用于记录各种类型证券数  
	CMapStringToString m_StockTypeMap[3];

    short m_nANT[3];                                             //目前上海与深圳开市分钟数 
	short m_nOldANT[3];                                            //记录以前上海与深圳开市分钟数 
	short m_nMaxANT[3];                                            //上海与深圳开市分钟数   
	short m_nMaxMaxANT;                                            //上海与深圳最大开市分钟数
	short m_nDel_Half_ANT[3];                                      //上午开市分钟数
	short m_nDel_Start_A_hr[3];                                    //上午开盘小时数 
	short m_nDel_Start_A_min[3];                                   //上午开盘分钟数
	short m_nDel_Start_B_hr[3];                                    //下午开盘小时数
	short m_nDel_Start_B_min[3];                                   //下午开盘分钟数
	short m_nDel_End_A_hr[3];                                      //上午收盘小时数
	short m_nDel_End_A_min[3];                                     //上午收盘分钟数
	short m_nDel_End_B_hr[3];                                      //下午收盘小时数 
	short m_nDel_End_B_min[3];                                     //下午收盘分钟数
    short m_nHave_Olddata;                                         //记录当天数据是否已保存 
    int m_nNowHour;                                                //开市小时数   
    int m_nNowMin;                                                 //开市分钟数
    int m_nNowSec;                                                 //开市秒数 
	long m_lStartMinA[3];                                          //上午开盘时间
    long m_lEndMinA[3];                                            //上午收盘时间
	long m_lStartMinB[3];                                          //下午开盘时间
    long m_lEndMinB[3];                                            //下午收盘时间
    short m_nRN[3][240],m_nRN1[3][240],m_nDN[3][240],m_nRD[3][240],m_nDN1[3][240],m_nCalStk[3][240];              //用于计算大盘强弱指标中间变量

	int m_nStockTypeShPage;
	int m_nStockTypeSzPage;
	int m_nStockTypeFlPage;

	int m_CalcBlockCounts;

	int m_nCurrentBlockPage;           //保存现有板块页 
	int m_nCurrentReportPage;           //保存现有行情页 
//    CStringList	m_ScreenList;       //条件选股列表                             //用于保存条件选股股票代码
	CArray<SharesSymbol,SharesSymbol&> m_ScreenList;
	struct tm m_week;
	int m_nIsClose;
    int m_nColumnIndex[4][MAXCOLUME];	//用于记录列索引号



    CSharesInformation m_sharesInformation;
    CSharesCompute m_sharesCompute;
	CManageBlockData m_ManagerBlockData;
	TechniqueColumnInfo *m_pTechniqueColumn;
#ifdef WIDE_NET_VERSION
	CDownLoadNetDlg *m_pDownLoadDlg;
#else
	CDownLoadDlg *m_pDownLoadDlg;
#endif	
	BOOL m_bCloseReceiver;
	BOOL m_bOpenStockTypeDlg;

	STOCKDATASHOW *		m_pStockDataShow;
	int					m_nShowMaxCount;
	int					m_nShowCount;

	STOCKDATASHOW *		m_pStatisticData;    //平均值，最大值，最小值和方差
	bool				m_bStatistic[4];
	bool				m_bStatisticed;
	int					m_nWeightType;//0 = 算术平均，1=流通股本，2= 总股本

	BLOCKINFO*			m_pBlockDataShow;
	int					m_nBlockShowMaxCount;
	int					m_nBlockShowCount;

	CMapNewsFileName	m_NewsFileNameMap;                           //定义记载当天接收文件信息
	KLINEHEAD			m_KLineHead;                              //定义日线文件大头结构
	MKLINEHEAD			m_MinKLineHead;                           //定义5分钟线文件大头结构


//YHH
	CList<SharesSymbol*,SharesSymbol*> m_choosesymbol;//自选股的代码LIST
    CList<BLOCKSTOCK*,BLOCKSTOCK*> m_blocklist;     //板块的LIST
    CList<BLOCKSTOCK*,BLOCKSTOCK*> m_Tjxgblocklist;     //条件选股的LIST
    CString          m_cybkname[10];

	Rsdn1 *Nidx[2];     //用于储存大盘个股涨跌数
    Tidxd *Tidx[2];     //于储存大盘强弱指标
    Aidx1 *Adat[2];   	//用于储存大盘指标数据
	int InitSetp;

	int m_nPreDisplayRow;   //用于保存显示行数


	CBlockAnalysisView*		m_pBlockAnalysisView;
	CTaiShanReportView*		m_pReportView;
	CSharesBigBillDlg*		m_pStockCjMxTj;
	CSharesBigBillData*		m_pStockBigBillData;
	CNineShowView*			m_pNineShowView;
	CTaiTestTreeView*		m_pTestTreeView;
    CInfoView*				m_pInfoView;
	CTaiShanKlineShowView*	m_pKlineShowView;
	///is a splitter window////////


	void Init_dat();                                            //初始化开市时间与昨天数据保存情况   
    void Chk_Ystc();                                            //检查今天是否有数据保存
	void chk_date();                                            //得到当天日期 
	void Init_EveryDay();                                       //每天初始化行情数据 
	void Init_StockData(int mode);                              //初始化股票行情数据 
    int GetStocktime(int mode);                                 //得到开市即时时间
	void LoadStockData(int mode);                               //得到股票名称表及行情数据
    void CreateFileData(int mode);                              //建立当日数据库文件 
	void LoadFileData(int mode);                                //装储数据库文件数据 
	void GetStockCount();                                       //得到当天上市股票数
	void OnCalcHqDataProgress();                                //计算大盘数据与当前分钟数
    float OnCalcDpTidxdData(int which_stk);                     //计算大盘强弱指标
	long GetStockDay(time_t time );								//股票处理程序         
	__int64 GetStockMinute(time_t time );						//股票处理程序             
    void StockCloseWork();
	void SaveFileData();
	void StockNameConvert(char *StockName,char *pyjc);
	void InitTjxg();           //初始化条件选股
	BOOL IdleProc( LONG lCount );
	void InitChooseAndStockType();
    void InitStockFiveDaysVolumn();
    void InitFiveDaysVolumnForStock(PCdat1 &pCdat);
    void WriteStockInfoToFile();
	BOOL StatisticStockData(byte nWeightType,int nStockType = -1);
	void ZeroStatisticStockData();


#ifdef WIDE_NET_VERSION
    void WideNetTransactSystemRunPara(RCV_SYSTEMRUN_STRUCTEx *pSystemRunPara);
	void WideNetTransactIndexAttribute(RCV_INDEXATTRIBUTE_STRUCTEx *pIndexAttribute, int nParaCounts, int nIndexType);
	void WideNetInitMarketBargainingData(RCV_STKLABEL_STRUCTEx *pStkLabel,int nStkLabelCounts);
#endif
    

//fellowing four array are used by keyboard angel ///////////////////////////////
	CArray<DATA_KEYBOARD*,DATA_KEYBOARD*> m_hotkey; //系统的基于数字的热键
	CArray<DATA_KEYBOARD*,DATA_KEYBOARD*> m_hotchar;//系统的基于字符的热键
	CArray<DATA_KEYBOARD*,DATA_KEYBOARD*> m_keychardata; //基于字符的键盘数据
	CArray<DATA_KEYBOARD*,DATA_KEYBOARD*> m_keynumberdata;//基于数字的键盘数据
/////////////////////////////////////////////////////////////////////////////////

protected:
///Added by Xia Qiankun 2004/1/24
	//Free any array buffer related to key board angel
	void FreeKeyBoardAngel();
	//Initialize or refresh key board angel data center
	void InitializeKeyBoardAngle(BOOL IsAddStock);
//////////////////End added 2004/1/24

// Generated message map functions
protected:
	void CheckKind();
	//{{AFX_MSG(CTaiShanDoc)
	afx_msg void OnOpenDocument();
	afx_msg void OnToolClosework();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WSSTOCK2000DOC_H__61A31CF1_9707_11D1_ACAB_0000E823DBFD__INCLUDED_)
