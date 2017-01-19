#ifndef __WSSTOCKSTRUCT_1000_H__
#define __WSSTOCKSTRUCT_1000_H__

#define CALLOC(n,x)    ((x *) farcalloc((long)(n),sizeof(x)))
#define FREE(x)        farfree(x)

#define MAX_RIGHTS		3
#define BLOCKTYPE_NUM	512
#define MAXJISHUCOUNT	10

#define STK_SYMBOL_LEN	10
#define STK_NAME_LEN	64
#define STK_PYJC_LEN	32



//typedef enum {
//	SHZS=0,                           //上证指数 
//	SHAG=1,                             //上证A股 
//	SHBG=2,                             //上证B股 
//	SZZS=3,                              //深圳指数
//	SZAG=4,                              //深圳A股
//	SZBG=5,                              //深圳B股
//	SHZQ=6,                              //上证债券
//	SZZQ=7,                              //深圳债券
//	SHJJ=8,                              //上证jijin 
//	SZJJ=9,                              //深证jijin 
//	EBZS=10,                              //二板指数   
//	EBAG=11,                              //二板股票
//	BLOCKINDEX=12,                          //板块指数  
//	CHOOSESTK=20,                           //自选股 
//	QBGP=30                              //全部股票
//};
////////////////////////////////////////////////////////
//客户端应用层数据请求部分
//
enum {
	SHZS=0,                           //上证指数 
	SHAG,                           //上证A股 
	SHBG,                           //上证B股 
	SHJJ,                        //上证基金 
	SHZQ,                           //上证债券
	SHZZ,                           //上证转债
	SHHG,                           //上证回购
	SHETF,                          //上证ETF基金
	SHKF,                     //上证开放基金
	SHQZ,                           //上证权证
	SHOT,                        //上证其它=10

	SZZS,                           //深圳指数=11
	SZAG,                          //深圳A股
	SZBG,                          //深圳B股
	SZJJ,                       //深证基金 
	SZZQ,                          //深圳债券
	SZZZ,                          //深圳转债
	SZHG,                          //深圳回购
	SZETF,                         //深证ETF基金
	SZKF,                    //深证开放基金
	SZQZ,                          //深证权证
	SZSB,						  //深证三板				
	SZOT,                       //深证其它
	SZZXQY,                        //中小企业
	SZCYB,                         //创业板=25

	BLOCKINDEX,                    //板块指数  
	CHOOSESTK,                     //自选股 
	QBGP                           //全部股票=40

};

#define STOCKTYPENUM  CHOOSESTK + 1

enum {FLPAGE = 0,SHPAGE, SZPAGE ,AGPAGE,ZXPAGE,CYPAGE,BGPAGE,QZPAGE,JJPAGE,BLOCKPAGE,CHOOSEPAGE,TJXGPAGE,LASTPAGE};

typedef struct
{
		float 	Price;      //    价格 
		float 	Volume;     //    量 
		float 	Amount;     //    额
    
}Kdata1;
typedef struct
{
		float 	Price;      //    价格 
		float 	Volume;     //    量 
		float 	Amount;     //    额
        float   accb;       //    买入挂单和（三种买入挂单和）
        float   accs;       //    卖出挂单和（三种卖出挂单和）
	    float   bsspbas;    //       量比指标
		float   dif_accb;   //       大盘买入挂单量差值（本次 - 上次）
		float   dif_accs;   //       大盘卖出挂单量差值（本次 - 上次）
}DpKdata1;
//除权数据结构
typedef struct 
{
	int   nFlags;
	int	  nTime;
	float Give;//每股送
	float Allocate;//每股配
	float AllocatePrice;//每股配价
	float Bonus;//每股红利
	BYTE Free[4];
}Split ,*PSplit;

//股票基本资料数据结构
typedef struct
{
	char Symbol[STK_SYMBOL_LEN];  //股票代码
	int  NumSplit;   //已经除权次数
	float zgb;     //总股本(万股)
	float gjg;     //国家股(万股)
	float fqrfrg;  //发起人法人股(万股)
	float frg;     //法人股(万股)
	float zgg;     //职工股(万股)
	float gzAg;    //公众A股(万股)    5
	float ltAg;    //流通A股(万股)
	float Hg;      //Ｈ股(万股)
	float Bg;      //B股(万股)
	float zpg;     //转配股(万股)

	float zzc;     //总资产(万元)    10
	float ldzc;    //流动资产(万元)
	float cqtz;    //长期投资(万元)
	float gdzc;    //固定资产(万元)
	float wxzc;    //无形资产(万元)
	float ldfz;    //流动负债(万元)   15
	float cqfz;    //长期负债(万元)
	float ggqy;    //股东权益(万元)
	float zbgjj;   //资本公积金(万元)
	float yygjj;   //盈余公积金(万元) 
	float mgjz;    //每股净值(元)    20
	float gdqybl;  //股东权益比率(%)
	float mggjj;   //每股公积金(元)

	float zyywsr;  //主营业务收入(万元)
	float zyywlr;  //主营业务利润(万元)
	float qtywlr;  //其它业务利润(万元)25
	float lrze;    //利润总额(万元)
	float jlr;     //净利润(万元)
	float wfplr;   //未分配利润(万元)
	float mgsy;    //每股收益(元)
	float jzcsyl;  //净资产收益率(%)  30
	float mgwfplr; //每股未分配利润(元)

	float mgjzc;   //每股净资产(元)

	int m_iPos;
	BYTE  free[8];
	Split m_Split[80];
}BASEINFO;
typedef BASEINFO *PBASEINFO;

//===============旧的基本资料文件结构========================

//股票基本资料数据结构
typedef struct
{
	char Symbol[STK_SYMBOL_LEN];  //股票代码
	int  NumSplit;   //已经除权次数
	float zgb;     //总股本(万股)
	float gjg;     //国家股(万股)
	float fqrfrg;  //发起人法人股(万股)
	float frg;     //法人股(万股)
	float zgg;     //职工股(万股)
	float gzAg;    //公众A股(万股)
	float ltAg;    //流通A股(万股)
	float Hg;      //Ｈ股(万股)
	float Bg;      //B股(万股)
	float zpg;     //转配股(万股)

	float zzc;     //总资产(万元)
	float ldzc;    //流动资产(万元)
	float cqtz;    //长期投资(万元)
	float gdzc;    //固定资产(万元)
	float wxzc;    //无形资产(万元)
	float ldfz;    //流动负债(万元)
	float cqfz;    //长期负债(万元)
	float ggqy;    //股东权益(万元)
	float zbgjj;   //资本公积金(万元)
	float yygjj;   //盈余公积金(万元)
	float mgjz;    //每股净值(元)
	float gdqybl;  //股东权益比率(%)
	float mggjj;   //每股公积金(元)

	float zyywsr;  //主营业务收入(万元)
	float zyywlr;  //主营业务利润(万元)
	float qtywlr;  //其它业务利润(万元)
	float lrze;    //利润总额(万元)
	float jlr;     //净利润(万元)
	float wfplr;   //未分配利润(万元)
	float mgsy;    //每股收益(元)
	float jzcsyl;  //净资产收益率(%)
	float mgwfplr; //每股未分配利润(元)

	float mgjzc;   //每股净资产(元)

	int m_iPos;
	BYTE  free[8];
	Split m_Split[80];
}BASEINFO_OLD;
typedef BASEINFO_OLD *PBASEINFO_OLD;

//===============旧的基本资料文件结构========================
typedef struct
{
	int DayAddr;
	int Min5Addr;
	BASEINFO *pBaseInfo;
	int HsAddr;
	char Free[8];
}MapFileAddr;

typedef struct
{
	long m_lFileTitle;         //   文件标志
    long m_lFileExit;          //   文件正常退出标志
	long m_lStockTypeCount;    //   板块数
    long m_lStockTypeMaxCount; //   最大板块数
    long m_lStockCount;        //   目前股票个数
    long m_lStockMaxCount;     //   最大股票个数
	long m_lLastTime;          //   最后处理时间
//    BOOL m_bInitDone;          //   初始化标志  
	long m_lBlockTypeCount;		//板块分类个数
	char m_szBlockTypeName[BLOCKTYPE_NUM][STK_NAME_LEN];	//板块分类名
}BLOCKFILEHEAD;
typedef BLOCKFILEHEAD *PBLOCKFILEHEAD;

//板块指数属性结构
typedef struct
{
    long  m_lIsUse;         //   是否使用
	char  m_szCode[STK_SYMBOL_LEN];      //   板块指数代码
    char  m_szName[STK_NAME_LEN];     //   板块指数名称，如电力，安徽
	long  m_lStockCount;    //   股票数
    float m_fTotalRight;   //   板块总权重
	float m_fNewIndex;     //   最新指数
	float m_fYstIndex;     //   昨天指数
	float m_fOpenIndex;	   //	当日指数
	float m_fHighIndex;    //   当日最高
	float m_fLowIndex;     //   当日最低
	float m_fTotP;         //   最新成交金额  
	float m_fTotV;         //   最新成交量
	float m_fTotRVol;      //   外盘
	float m_fTotDVol;      //   内盘
	float m_fLTP;			//	流动盘
	float m_fMGSY;			//	每股收益
	long  m_lUpStockCount;	//	涨股数
    int   m_iRightType;     //   取用权重类型
    BOOL  m_bIsDelete;      //   删除标志
	float m_fRDR;			//	最大涨幅
	char  m_szFirstStock[STK_NAME_LEN];	//领涨股票名
	char  m_szBlockTypeName[STK_NAME_LEN]; //  板块的分类名 如行业，地域
}BLOCKINFO;

typedef BLOCKINFO *PBLOCKINFO;

//股票在板块中属性结构
typedef struct
{
	char	m_szSymbol[STK_SYMBOL_LEN];	//   股票代码
	BOOL	m_bDeleted;
	int		m_iPos;
	float	m_ClosePrice;
    float	m_fRight[MAX_RIGHTS];     //   股票权重
    int		m_nBlockType[BLOCKTYPE_NUM];    
}STOCKINFOINBLOCK;

typedef STOCKINFOINBLOCK *PSTOCKINFOINBLOCK;

typedef struct
{
	STOCKINFOINBLOCK *pStockInfo;
}STOCKPOINTINFO;

//======================
typedef struct
{
	char	m_szSymbol[STK_SYMBOL_LEN];	//   股票代码
	BOOL	m_bDeleted;
	int		m_iPos;
	float	m_ClosePrice;
    float	m_fRight[MAX_RIGHTS];     //   股票权重
    int		m_nBlockType[BLOCKTYPE_NUM];    
}STOCK_TYPE_INFO_OLD;
//======================
//CReportData结构说明
//CReportData结构主要用于即日即时股票行情信息
typedef struct
{
 BYTE kind;
 BYTE  rdp[240];           //      用于计算大盘强弱指标
 char id[STK_SYMBOL_LEN];             //      证券代码
 char name[STK_NAME_LEN];           //     证券名称 
 char Gppyjc[STK_PYJC_LEN];         //      证券名称拼音简称
 int sel;
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
 int  InOut;			  //	  0:外盘，1：内盘，2：平盘
 BOOL  IsDelete;
 BOOL  IsMxTj;
 
 float Index[10];
 Kdata1   m_Kdata1[240];
 BASEINFO *pBaseInfo;

 STOCKINFOINBLOCK *pBlockInfo;

 int	m_serialNumber;		//		当日换手文件中股票数据的序列号
 time_t	time;
} CReportData;
typedef CReportData *PCdat1;
typedef struct
{
 	CReportData *pItem;
	char StockId[STK_SYMBOL_LEN]; 
	float ItemData;
}STOCKDATASHOW;
typedef STOCKDATASHOW *PSTOCKDATASHOW; 


typedef struct
{
 char name[STK_NAME_LEN];           //      证券名称 
 float ystc;
 float nowp;             //      最新价格
 float nowb;             //      现在买入价
 float nows;             //      现在卖出价 
 float nowv;              //      最新成交量
 BOOL InOut;
 char kind;
}CUOHEDATA;

/*
对Kind 股票种类说明
上证股票
a. 当id[0]<5 或者id[0]=7 时 Kind=7  属于债券，配股类  序号存在QhzqSerl 
b. 当id[0]<9 时Kind=1               属于A股          序号存在ShowSerl
c. 其它         Kind=2               属于B股          序号存在ShowSerl
深圳股票
a. 当id[0]=6 或者id[0]=1 或者id[0]!=0 时 Kind=6 债券，配股类
b. 当id[0]!=2 或者id[0]!=1                Kind=4    属于A股
c. 其它                                    Kind=5    属于B股
*/

//Aidx1结构说明
//Aidx1结构用于大盘即日即时行情信息
typedef struct
{
 float ystc;              //       昨天收盘指数     
 float nowi;              //       最新指数
 float opni;              //       开盘指数  
 float higi;              //       最高指数
 float lowi;              //       最低指数
 float nowp;              //       即时成交金额
 float nowv;              //       即时成交数量
 float accb;              //       大盘总买入挂单   
 float accs;              //       大盘总卖出挂单
 float bsspbas;           //       委比指标
 float dif_accb;          //       大盘买入挂单量差值（本次 - 上次）
 float dif_accs;          //       大盘卖出挂单量差值（本次 - 上次）
 float volume5;           //       5天均量 

 float StartP;            //       开始时间指数 
 float EndP;              //       结束时间指数


 float totp;              //       总成交金额  
 float totv;              //       总成交量
 float lastaccb;          //       上次大盘总买入挂单
 float lastaccs;          //       上次大盘总卖出挂单
 float lasttotb;          //                
 float lasttots;
 float lasttotp;          //       上次大盘总成交金额                
 float lasttotv;          //       上次大盘总成交数量 

 DpKdata1 *m_DpKdate;     //        用于保存大盘分钟线
} Aidx1;

//Rsdn1结构说明
typedef struct
{
 short rsn;       
 short dnn;
} Rsdn1;

typedef Rsdn1 **PRsdn1;

//Tidxd结构说明
//Tidxd结构用于加权指数曲线与多空指标线
typedef struct
{
 short sec5;             //          加权指数值    
 short rp;               //          多空指标中RP值
 short dp;               //          多空指标中DP值
 short res;
} Tidxd;
typedef Tidxd **PTidxd;


//实时行情数据头结构
typedef struct
{
	long filetitle;
	long Day;
	short OldANT[3];
	BOOL CloseWorkDone;
	long StockCount;
	long MaxStockCount;
	long FileExitDone;
	BYTE Res[12];
}REALDATA;
typedef REALDATA *PREALDATA;
//股票基本资料数据头结构
typedef struct
{
	long filetitle;            
	long FileExitDone;
	long StockCount;
	long MaxStockCount;
    long FixedNumSplit;
	BYTE Res[20];
}BASEINFOHEAD;
typedef BASEINFOHEAD *PBASEINFOHEAD;

typedef struct _BIG_LIST
{
	char id[STK_SYMBOL_LEN];
	char name[STK_NAME_LEN];
	int nKind;
	time_t ntime;
	int  ntype; 
	float nowv; 
	float nowp;  
	float rdr;
}BIG_LIST;
typedef BIG_LIST *PBIG_LIST;
typedef struct _BIG_LISTHEAD
{
	BOOL Done;
	BOOL IsGbjg;
	BOOL IsChange;
	float BigSet[4];
	float BigMaxSet[4];
	float BigBillSetType[10];
	float BigSetGB[4];
	float BigMaxSetGB[4];
	float BigBillSetTypeGB[10];
    long ItemCount ;
	long MaxItemCount;
	long totalBigCount[4];
	float totalBigVolumn[4];
	long totalCount[10];
	float totalVolumn[10];
    float totalV;
	BOOL IsCalc[4];
	BOOL IsSaveData;
}BIG_LISTHEAD;
typedef BIG_LISTHEAD *PBIG_LISTHEAD;
//
typedef struct _SHARES_DATA_ADD
{
	char name[STK_NAME_LEN];
	int  StockType;
}SHARES_DATA_ADD;

//多空指标 = RP - DP

typedef struct  NameSymbolStr 
{
        char   stocksymblo[STK_SYMBOL_LEN];
		char   stockname[STK_NAME_LEN];
		char   stockhypy[STK_PYJC_LEN];
		char   group;
}NAMESYMBOL;

//股票补分钟线数据结构
typedef struct  tagMIN_TOTAL_STRUCTEx 
{
	RCV_MINUTE_STRUCTEx *m_RcvMinute;     //分钟线接口
	long Min_Count;                       //分钟线纪录数

}MIN_TOTAL_STRUCTEx;

//股票补日线数据结构
typedef struct  tagDAY_TOTAL_STRUCTEx 
{
	RCV_HISTORY_STRUCTEx *m_RcvDay;     //分钟线接口
	long Day_Count;                       //分钟线纪录数

}DAY_TOTAL_STRUCTEx;
//股票补历史5分钟线数据结构
typedef struct  tagMIN5_TOTAL_STRUCTEx 
{
	RCV_HISMINUTE_STRUCTEx *m_RcvMin5;     //5分钟线接口
	long Min5_Count;                       //5分钟线纪录数

}MIN5_TOTAL_STRUCTEx;

typedef struct  tagPOWER_TOTAL_STRUCTEx 
{
	RCV_POWER_STRUCTEx *m_RcvPower;     //分钟线接口
	long Power_Count;                       //分钟线纪录数

}POWER_TOTAL_STRUCTEx;

/////////////////////////////////////////////////////////////
//our数据存储格式
struct TAI_SHAN_DATA
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

////////////////////////////////////////////////
///日线大文件头
struct KLINEHEAD
{
	int	NumChuQPerBlk;	    	//每小块的股票除权数据的容量	
	int	numPerBlk	;			//每小块的股票K线数据数量,256天
	int	numKline_sh	;			//上证证券总数
	int	numBlkAll_sh;			//上证小块块总数
	int numKline_sz	;	        //深证证券总数
	int numBlkAll_sz;	        //深证小块总数
};
///////////日线小文件头
struct KLINESMALLHEAD
{
	char    StockSign[STK_SYMBOL_LEN]; //前8个字节为股票代码。
	short 	numChuQ	;	  //2个字节为除权数据的总次数
	long    numKln	;	  //4个字节整型，代表日K线的个数。
	short   symBlock[25]; //下面每两个字节代表K线数据的块号
	long    index;
};

///////////5分钟线头文件结构
struct MINUTEKLINEHEAD
{
	char    StockSign[STK_SYMBOL_LEN]; //前8个字节为股票代码。
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
struct MKLINEHEAD
{
	int	numPerBlk	;			//每小块的股票5分钟K线数据数量,75*48天
	int	numKline_sh	;			//上证证券总数
	int numKline_sz	;	        //深证证券总数
};
//
typedef struct
{
  time_t day;
  float opn;
  float hig;
  float low;
  float clo;
  float vol;
  float amt;
  short adv;
  short dec;
} Misd;

typedef struct
{
  char  m_zqdm[7]; 
  float SortID;
} SORTSTRUCT;


typedef struct
{
	long day;
	Misd misd[48];
}MINUTEData;


typedef struct _ADD_DATA_DAY_LINE
{
	char DayLineName[10];
	int  DayLineType;
}ADDDAYLINEDATA;

typedef struct _tagIndex_Technique
{
       int  iKind;                      //表示条件的种类。
       char sIndex_name[50];            //指标的名称。
       int  iLine;                      //表示第几条线。
       int  iDataType;                  //表示选股的线型。
       float fParam[8];                   //表示参数1。
       float fSubParam1;                //表示子参数1。
       float fSubParam2;                //表示子参数2。
       int  iSunIndex;                  //表示第几种子公式。
	   char SubLineType[10];
}Index_Technique;

typedef struct _TECHNIQUE_COLUMN{ 
    UINT     nID ;        //  ID number
	Index_Technique nIndex;
	BOOL IsUse;
} TECHNIQUE_COLUMN ,*PTECHNIQUE_COLUMN; 

typedef struct _NineViewData
{
	int minutecount;
	BOOL IsDay;
	UINT showtype;
}NineViewData;

typedef struct _TotalNineViewData
{
	int NineInitDone;
    NineViewData m_NineViewDate[9];
}TotalNineViewData;


typedef struct _SystemInitData
{
	int InitComplete;
    char StockTypeName[STK_NAME_LEN];        //板块名称
	char ScreenStockName[STK_NAME_LEN];      //条件选股名称 
	int  ColumnIndex[30];          //动态显示窗列号
	int  ColumnType;               //动态显示窗目前显示数据种类
	int  ColumnWidth[30];          //动态显示窗列宽度
	TECHNIQUE_COLUMN TechniqueColumn[MAXJISHUCOUNT];        ////动态显示窗技术列
    char NetAddr[255];               //默认网址
	char KswtAddr[255];              //可视委托缺省路径
    int  RealTime;                  //实时排序时间间隔
	int  BlockTime;                 //板块指数计算时间间隔         
	TotalNineViewData m_TotalNineViewData;
	char ChooseTypeName[STK_NAME_LEN];        //自选股名称
}SYSTEMINITDATA;	 







#endif // __WSSTOCKSTRUCT_1000_H__
