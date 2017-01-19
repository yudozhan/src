#ifndef	_RECT_HEADER_
#define	_RECT_HEADER_
;
//#include "CFormularCompute.h"
#include <afxtempl.h>
#include "CFormularContent.h"
#include "lineRightBox.h"

#define WM_USER_DRAG_BEGIN	WM_USER + 256
/*//---		常数
	m_WordTable[_T("volstick")]    =(void *)910;
	m_WordTable[_T("stick")]	   =(void *)911;
	m_WordTable[_T("circledot")]   =(void *)912;
	m_WordTable[_T("linestick")]   =(void *)913;
	m_WordTable[_T("crossdot")]    =(void *)914;
	m_WordTable[_T("colorstick")]  =(void *)915;
*/
#define VOLSTICK                    1
#define STICK_VOL_FS                2
#define CIRCLE		                3
#define LINESTICK                   4
#define CROSSDOT                    5
#define STICK                       6
#define STICK_LINGXIAN_FS		    7

//  CMap info_jishu	存放技术指标的信息，其元素为类CFormularContent的对象 
//  每个技术分析指标的数据结构(以类的形式构造)：
#define CHUQ_PER_BLK				6		//每小块的除权数据的个数
#define KLINE_PER_BLK				256		//每小块的K线数据的个数
#define NUMBYTE_PER_KLINE			32		//每个K线数据的字节数
#define NUMBYTE_PER_CHUQ			32		//每个除权数据的字节数
#define NUM_STK_K_TOTAL				4096	//股票总数

#define	STCKNAMELEN					50
#define	FENSHINUM					240

//#define m_heightCaption            15
//#define m_midLen                   50
//#define m_rightLength              172
#define m_midLen_L					40
//#define LEFT					     50
#define STATICID                    600667

#define WIDTH_KLINEBODY             6
#define LEN_KLINE                   10


#define CORLOR_ZUOBIAO              RGB(0,0,0)
#define RIGHTBOX_PERLINE            18

//#define m_minGrid                 25
//#define m_minGridX                55

#define RIGHTHEIGHT                 250

#define XIAN_DUAN                   0
#define SHUI_PING                   1
#define QU_SHI                      2
#define PING_XING					3
#define SU_DU		                4
#define SHAN_XING                   5
#define GAN_SHI                     6
#define HUANG_JIN					7
#define BO_DUAN		                8
#define BAI_FEN_BI                  9
#define HUI_GUI                     10
#define HUI_GUI_DAI_1               11
#define HUI_GUI_DAI_2		        12
#define ZHOU_QI						13
#define NA_QI                       14
#define ZHONG_XIAN					15
#define NA_QI_HU		            16
#define HUANG_JIN_HU                17
#define UP_ARROW                    18
#define DOWN_ARROW					19
#define RECT_KIND		            20
#define TEXT_KIND		            21
#define DELETE_LINE		            22
#define BAG_LINE		            23
#define SPIRE_LINE		            24

#define  REGION_NO					0

//左边五个图间的线
#define  REGION_PICTBORDER1			1
#define  REGION_PICTBORDER2			2
#define  REGION_PICTBORDER3			3
#define  REGION_PICTBORDER4			4

//左边五个图的作图区
#define  REGION_PICT1				5
#define  REGION_PICT2				6
#define  REGION_PICT3				7
#define  REGION_PICT4				8
#define  REGION_PICT5				9

//左边图形区的下边标尺
#define  REGION_RULER				11

#define  REGION_TRANSFER			12

//左边五个图的标题区
#define  REGION_PICTCAPTION1		14
#define  REGION_PICTCAPTION2		15
#define  REGION_PICTCAPTION3		16
#define  REGION_PICTCAPTION4		17
#define  REGION_PICTCAPTION5		18

//左边五个图所对应的标尺区
#define  REGION_PICTTEXT1		24
#define  REGION_PICTTEXT2		25
#define  REGION_PICTTEXT3		26
#define  REGION_PICTTEXT4		27
#define  REGION_PICTTEXT5		28


#define  REGION_MANY_FIGUER		36

//右下角的Tab区
#define  REGION_LITTLE_FIGUER	37

#define  REGION_SUB				46
#define  REGION_ADD				47
#define  REGION_INFO_TRANSFER	48

#define  REGION_SELFLINE_1		58
#define  REGION_SELFLINE_2		59
#define  REGION_SELFLINE_3		60
#define  REGION_SELFLINE_BODY	61
#define  REGION_RIGHT_LINE		70

#define  REGION_ZOOM            80    //cjg信息栏放大缩小区

#define  XIA_DIE_WORD       15
#define  PING_PAN_WORD      14



/*17．自画线数据的内存中的存储结构：
CArray		lineself		 存放各个自画线数据 ，其元素为INFO_LINESELF结构的对象
INFO_LINESELF结构的成员如下：
	int 	type		
	int	timeFirst
	int 	timeScd
	float		valFir
	float		valScd
*/
struct INFO_LINESELF{
	int 	type;		//线的类型
	int     klineType;	//K线类型
	int		time1;		//各个点的时间值
	float	valFlt1;	//各个点的浮点数值
	int		time2;		//各个点的时间值
	float	valFlt2;	//各个点的浮点数值
	int		time3;		//各个点的时间值
	float	valFlt3;	//各个点的浮点数值
	COLORREF	color;	//线的颜色
	BYTE	byLineKind;	//线的种类:0是实线,1是虚线,2是点横线
	BYTE	byLineWidth;//线的宽度:0是1像素,1是2像素,2是3像素
	BYTE	byAlarm;	//是否是条件预警,只对趋势线有效
	BYTE	byWhichFiguer;//预留
	int     nDays;		//多日线天数
	int     stored;		//预留
	INFO_LINESELF()
	{
		byWhichFiguer = 0;
	}
};

//
typedef struct{
	char ZhBName[20];		//指标名称
 	int  m_yBottom;			//矩形的底部的y坐标
}INDEX_BELLOW;

typedef struct{
//	char stockName[16];
	int  rightX;		//判断是否是指标的画图区域的右部的x坐标
	INDEX_BELLOW rtBlw[5];		//指标
}RECTKLINE;

 typedef struct{
	int  rightX;		//判断是否是指标的画图区域的右部的x坐标
	int  leftX;			//判断是否是指标的画图区域的左部的x坐标
	INDEX_BELLOW rtBlw[5];		//指标
}RECTFENSHI;
/*
class ARRAY_Right_Box
{
public:
	ARRAY_Right_Box()
	{
	};
	virtual ~ARRAY_Right_Box()
	{
	};
}*/
struct LooseValue
{
	int		nFoot;
	float	fVal;
};
typedef CArray<LooseValue,LooseValue&> looseValueArray; 
class ARRAY_BE
{ 
public:
	float *line;
	int b;
	int e;
	float *lineWidth;

	enum Kind
	{
		Normal = 0,
		DrawLine,
		DrawPolyLine,
		DrawStickLine,
		DrawIcon,
		StringData,//DrawString
		LineRt,
	};

	Kind kind;//0 is normal,1 is draw line,3 is line data of right box,2 is string
	WORD nLineWidth;
	WORD isVirtualLine;
	CString s;
	ARRAY_Right_Box lineRight;
	looseValueArray looseArr;

	ARRAY_BE()
	{
		s = "";
		line = NULL;
		lineWidth = NULL;
		kind = Normal;
		b=0;
		e = 0;
		nLineWidth = 1;
		isVirtualLine = 0;
	};
	virtual ~ARRAY_BE()
	{
		if(lineWidth) delete [] lineWidth;
	};
	ARRAY_BE& operator=(ARRAY_BE& in)
	{
		this->b=in.b;
		e = in.e;
		kind = in.kind;
		nLineWidth = in.nLineWidth;
		isVirtualLine = in.isVirtualLine;
		s = in.s;
		if(line&&in.line && b>=0)
		{
			for(int i = b;i<=e;i++)
				this->line[i]=in.line[i];
		}

		if(kind == LineRt)
			lineRight = in.lineRight;
		if(kind	>=	DrawLine)
		{
			looseArr.Copy(in.looseArr );
		}

		if(kind == DrawStickLine)
		{
			if(this->lineWidth) delete [] lineWidth;
			if(in.e>=0)
			{
				lineWidth = new float[in.e+1];
				if(in.lineWidth)
					memcpy(lineWidth,in.lineWidth ,sizeof(float)*(in.e+1));
			}
			else
				lineWidth = NULL;
		}


		return *this;
	};
	bool Add(ARRAY_BE& in1,ARRAY_BE& in2 )
	{
		 b=max(in1.b,in2.b);
		 e=min(in1.e,in2.e);
		if(!in1.s.IsEmpty())
			 s = in1.s;
		if(!in2.s.IsEmpty())
			 s = in2.s;

		if(in1.kind == LineRt || in2.kind == LineRt)
		{
			 kind = LineRt;
			 lineRight = in1.lineRight+in2.lineRight;
		}
		else
		{
			 kind = in1.kind>in2.kind ? in1.kind : in2.kind ;

			if(in1.line&&in2.line &&  line &&  b>=0)
			{
				for(int i =  b;i<= e;i++)
					 line[i]=in1.line[i]+in2.line[i];
			}
		}
		return true;
	};
	bool Add(ARRAY_BE& in1,float fIn2 )
	{
		 b=in1.b;
		 e=in1.e;
		if(!in1.s.IsEmpty())
			 s = in1.s;

		 kind = in1.kind;
		if(in1.kind == LineRt )
		{
			 lineRight = in1.lineRight+fIn2;
		}
		else
		{
			if(in1.line &&  line &&  b>=0)
			{
				for(int i =  b;i<=e;i++)
					 line[i]=in1.line[i]+fIn2;
			}
		}
		return true;
	};
	bool Sub(ARRAY_BE& in1,ARRAY_BE& in2 )
	{
		 b=max(in1.b,in2.b);
		 e=min(in1.e,in2.e);
		if(!in1.s.IsEmpty())
			 s = in1.s;
		if(!in2.s.IsEmpty())
			 s = in2.s;

		if(in1.kind == LineRt || in2.kind == LineRt)
		{
			 kind = LineRt;
			 lineRight = in1.lineRight-in2.lineRight;
		}
		else
		{
			 kind = in1.kind>in2.kind ? in1.kind : in2.kind ;

			if(in1.line&&in2.line &&  line &&  b>=0)
			{
				for(int i =  b;i<=e;i++)
					 line[i]=in1.line[i]-in2.line[i];
			}
		}
		return true;
	};
	bool Sub(ARRAY_BE& in1,float fIn2 )
	{
		 b=in1.b;
		 e=in1.e;
		if(!in1.s.IsEmpty())
			 s = in1.s;

		 kind = in1.kind;
		if(in1.kind == LineRt )
		{
			 lineRight = in1.lineRight-fIn2;
		}
		else
		{
			if(in1.line &&  line &&  b>=0)
			{
				for(int i =  b;i<=e;i++)
					 line[i]=in1.line[i]-fIn2;
			}
		}
		return true;
	};
	bool Sub(float fIn2,ARRAY_BE& in1 )
	{
		 b=in1.b;
		 e=in1.e;
		if(!in1.s.IsEmpty())
			 s = in1.s;

		 kind = in1.kind;
		if(in1.kind == LineRt )
		{
			 lineRight = fIn2 -in1.lineRight;
		}
		else
		{
			if(in1.line &&  line &&  b>=0)
			{
				for(int i =  b;i<=e;i++)
					 line[i]=fIn2-in1.line[i];
			}
		}
		return true;
	};
	bool Mult(ARRAY_BE& in1,ARRAY_BE& in2 )
	{
		 b=max(in1.b,in2.b);
		 e=min(in1.e,in2.e);
		if(!in1.s.IsEmpty())
			 s = in1.s;
		if(!in2.s.IsEmpty())
			 s = in2.s;

		if(in1.kind == LineRt || in2.kind == LineRt)
		{
			 kind = LineRt;
			 lineRight = in1.lineRight*in2.lineRight;
		}
		else
		{
			 kind = in1.kind>in2.kind ? in1.kind : in2.kind ;

			if(in1.line&&in2.line &&  line &&  b>=0)
			{
				for(int i =  b;i<=e;i++)
					 line[i]=in1.line[i]*in2.line[i];
			}
		}
		return true;
	};
	bool Mult(ARRAY_BE& in1,float fIn2 )
	{
		 b=in1.b;
		 e=in1.e;
		if(!in1.s.IsEmpty())
			 s = in1.s;

		 kind = in1.kind;
		if(in1.kind == LineRt )
		{
			 lineRight = in1.lineRight*fIn2;
		}
		else
		{
			if(in1.line &&  line &&  b>=0)
			{
				for(int i =  b;i<=e;i++)
					 line[i]=in1.line[i]*fIn2;
			}
		}
		return true;
	};
	bool Div(ARRAY_BE& in1,ARRAY_BE& in2 )
	{
		 b=max(in1.b,in2.b);
		 e=min(in1.e,in2.e);
		if(!in1.s.IsEmpty())
			 s = in1.s;
		if(!in2.s.IsEmpty())
			 s = in2.s;

		if(in1.kind == LineRt || in2.kind == LineRt)
		{
			 kind = LineRt;
			 lineRight = in1.lineRight/in2.lineRight;
		}
		else
		{
			 kind = in1.kind>in2.kind ? in1.kind : in2.kind ;

			if(in1.line&&in2.line &&  line &&  b>=0)
			{
				for(int i =  b;i<=e;i++)
				{
					if(in2.line[i]!=0)
						 line[i]=in1.line[i]/in2.line[i];
					else
					{
						if(i>0)
							 line[i] =  line[i-1];
						else
							 line[i]= 0;
					}
				}
			}
		}
		return true;
	};
	bool Div(ARRAY_BE& in1,float fIn2 )
	{
		 b=in1.b;
		 e=in1.e;
		if(!in1.s.IsEmpty())
			 s = in1.s;

		 kind = in1.kind;
		if(in1.kind == LineRt )
		{
			 lineRight = in1.lineRight/fIn2;
		}
		else
		{
			if(in1.line &&  line &&  b>=0)
			{
				for(int i =  b;i<=e;i++)
				{
					if(fIn2!=0)
						 line[i]=in1.line[i]/fIn2;
					else
					{
						if(i>0)
							 line[i] =  line[i-1];
						else
							 line[i]= 0;
					}
				}
			}
		}
		return true;
	};
	bool Div(float fIn2,ARRAY_BE& in1 )
	{
		 b=in1.b;
		 e=in1.e;
		if(!in1.s.IsEmpty())
			 s = in1.s;

		 kind = in1.kind;
		if(in1.kind == LineRt )
		{
			 lineRight = fIn2 /in1.lineRight;
		}
		else
		{
			if(in1.line &&  line &&  b>=0)
			{
				for(int i =  b;i<=e;i++)
				{
					if(in1.line[i] != 0 )
						 line[i]=fIn2/in1.line[i];
					else
					{
						if(i>0)
							 line[i] =  line[i-1];
						else
							 line[i]= 0;
					}
				}
			}
		}
		return true;
	};
};

typedef struct 
{
	int     day;//时间
	float	open;//开盘价
	float   high;//最高价
	float   low;//最低价
	float   close;//收盘价
	float   vol;//成交量
	float   amount;//成交金额
	union
	{
		struct
		{
			WORD    advance;//上涨家数
			WORD    decline;//下跌家数
		};
		float   volPositive;//主动买量
	};
}Kline;
typedef struct 
{
	int     day;
	float	open;
	float   high;
	float   low;
	float   close;
	float   vol;
	float   amount;
	float   volPositive;
}KlineGeneral;

struct ARRAY_BE_SHOW
{ 
 	ARRAY_BE m_arrBE;
	int bNow;
	int type;
	int lineThick;
	COLORREF	clr;
	ARRAY_BE_SHOW()
	{
		clr = -1;
		lineThick = 1;
	};
//	char name[10];
};
typedef struct{
	 int numLine;
	 ARRAY_BE_SHOW	line[64];
}ARRAY_JISHU;

typedef struct{
	long    date;
	long	open;
	long	high;
	long	low;
	long	close;
	long    trade;
	long    volume;
	long    amout_1;
	char    pri_1;
	char    pri_2;
	WORD    pri_3;
	int    guben;
}DATA_KLINE2;

typedef struct{
	 short time;
	 float price;
	 float vol;
}HS;
typedef struct{
	 float price;
	 float vol;
}FENJIA;

struct POWER       //共32个字节
{
	int   nFlags;     //除权的种类，0为“先送后配”，1为“先配后送”，2为“股本不变”
	int   nTime;
	float fGive;      //song
	float fAllocate;
	float fAllocatePrice;//
	float fDividend;
	BYTE  byFree[4];  //预留12个字节
};
struct	SCREEN_DETAIL_L           
{
	int buytime;
	int selltime;
	float  buyprice;
	float  sellprice;
	int    timerange;
	float  profit;
	BOOL   issuccess;
};
struct  SCREEN_RESULT_L
{
	CString symbol;
	CString name;
	int     choosecount;
	int     successcount;
	float   successpercent;
	BOOL    isuseful;
	CArray <SCREEN_DETAIL_L*,SCREEN_DETAIL_L*> m_detail;
};

#define IDM_POP_TJXG_SHOW		300//
#define IDM_POP_KLINE_ADDED		301//
#define IDM_POP_CHANGE_PARAM		302//
#define IDM_POP_DRAG_MODE		303//
//#define ID_KLINE_MANY_DAY		304//

//#define MANY_DAY_KLINE		15
//#define HS_KLINE			25
//#define MIN1_KLINE		35
//#define NUM_MANY_DAY		50
struct  INDEX_INFO
{
	WORD  advance;	//上涨家数
	WORD  decline;	//下跌家数
	float amount;	//成交金额
	Tidxd tidxd;	//大盘信息
	float faccbaccs;//预留
};
struct  TRADE_DETAIL_INFO
{
	float unitCountVol;	//六个挂单买卖量的最大值
	float unitCountPrc;	//六个挂单买卖价减去当前价的绝对值的最大值
	BYTE buyP[5];		//三个挂单买入价（0：买1；1：买2；2：买3）
	BYTE sellP[5];		//三个挂单卖出价（0：卖1；1：卖2；2：卖3）
	BYTE buyV[5];		//三个挂单买入量（0：买1；1：买2；2：买3）
	BYTE sellV[5];		//三个挂单卖出量（0：卖1；1：卖2；2：卖3）
};
union	TRADE_DETAIL_INDEX
{
	TRADE_DETAIL_INFO buySellInfo;//个股信息
	INDEX_INFO	indexInfo;//大盘信息
};
struct  TRADE_DETAIL_H_PER
{
	float vol;	//陈较量
	float price;//成交价
	int time;	//时间

	TRADE_DETAIL_INDEX buySellOrIndex;//详细信息

};
//历史回忆文件：几天数据存为一个文件，文件格式跟日线文件格式一样。
//历史回忆处理以类封装。
typedef CTypedPtrList<CPtrList,TRADE_DETAIL_H_PER*> CBuySellList; 
typedef CArray<POWER,POWER&> powerArr; 
typedef union tagKlineEx
{
	struct
	{
		float buyPrice[5];	//买1--买3价
		float buyVol[5];		//买1--买3量
		float sellPrice[5];	//卖1--卖3价	
		float sellVol[5];		//卖1--卖3量
	};
	float fDataEx[20];			//保留
} KlineEx;

//#define MANY_DAY_KLINE		15
//#define HS_KLINE			25
//#define MIN1_KLINE		35
//#define NUM_MANY_DAY		50

enum klineKind {
	MANY_DAY_KLINE	= 15,//多日线
	HS_KLINE		= 25,//分笔成交
	MIN1_KLINE		= 35,//1分钟线
	MIN5_KLINE		= 1,//5分钟线	
	MIN15_KLINE		= 2,//15分钟线
	MIN30_KLINE		= 3,//30分钟线
	MIN60_KLINE		= 4,//60分钟线
	DAY_KLINE		= 5,//日线
	WEEK_KLINE		= 6,//周线
	MONTH_KLINE		= 7,//月线
	YEAR_KLINE		= 8,//年
	NUM_MANY_DAY	= 50 //
};
typedef struct tagINPUT_INFO
{
	char*		strSymbol;		//股票代码
	bool		bIndex;				//大盘

	int			nNumData;		//数据数量(pData,pDataEx,pResultBuf数据数量)
	Kline*		pData;					//常规数据,注意:当m_nNumData==0时可能为 NULL
	KlineEx*	pDataEx;				//扩展数据,分笔成交买卖盘,注意:可能为 NULL

	ARRAY_BE	fInputParam[10]	;//调用参数
	ARRAY_BE	fOutputParam;	//输出参数
	klineKind	klineType;			//数据类型
	BASEINFO*	financeData;			//财务数据
	CReportData* pDat1;//行情数据
	int			nReserved;
	void*		pBuySellList;
	int			m_stkKind;
} INPUT_INFO;

 struct SUB_FIGUER_INIT{
	float	    yFloatBottom;	//K线模块中每个子图矩形的底部的相对位置。
	char		nameIndex[20];	//每个子图的技术指标的名字。
 };

////////INFOINIT 结构：
 typedef struct{
		int					nCountIndex;		//子图的个数
		SUB_FIGUER_INIT		initIndex[5];		//子图的初始化信息。	四个子图的相对高度的和为1。

		int					nCountMin1;			//子图的个数
		SUB_FIGUER_INIT		initMin1[5];		//子图的初始化信息。	四个子图的相对高度的和为1。
		int					flag[5]	;			//每个子图的指标的ID号。0为分时走势，
												//1为成交量，2为买卖力道，3为量比指标，
												//4为领先指标，5为多空指标
		int					flag_dapan[5]	;	//沪指、深指的每个子图的指标的ID号。

		int					nIDRightSheet[10];
 }SUB_FIGUER_INIT_INFO;

struct  TRADE_DETAIL_BIG_LIST_PER
{
	float vol;	//陈较量
	float price;//成交价
	int   time;	//时间
	float buyPrice[5];	//买1--买3价
	float buyVol[5];		//买1--买3量
    float sellPrice[5];	//卖1--卖3价	
	float sellVol[5];		//卖1--卖3量
};
#define FABSMY(x) ((x)<0?-(x):(x))

typedef struct tagSymbolKind{
	char m_chSymbol[STK_SYMBOL_LEN];
	int	 m_nSymbolKind;
	tagSymbolKind()
	{
		memset(m_chSymbol,0,STK_SYMBOL_LEN);
		m_nSymbolKind = 0;
	};
 }SymbolKind;

typedef CArray<SymbolKind,SymbolKind&> SymbolKindArr;

CString CombineSymbol(CString symbol,int stkKind)
{
	CString s = symbol;
	int n = s.GetLength ();
	ASSERT(n<=6);
	for(int i = 0;i<10-n;i++)
		s+=" ";

	CString s2 ;
	s2.Format ("%d",stkKind);
	s+=s2;
	s+="s";
	return s;
};

int SeparateSymbol(CString symbol,CString & sOut)
{
	CString s = symbol;
	int n = s.GetLength ();
	sOut = "";
	if(n<=0) return 0;

	ASSERT(s[n-1] == 's');
	s = s.Left (n-1);
	n--;
	sOut = s.Left (10);
	sOut.TrimRight (" ");
	CString s3 = s.Right (n-10);
	n = atoi(s3);
	ASSERT(n>0 && n<20);
	return n;
};
extern CString g_strCompanyName;
#define FILEID22	33133313
#endif