#ifndef	_RECT_HEADER_
#define	_RECT_HEADER_
;
//#include "CFormularCompute.h"
#include <afxtempl.h>
#include "CFormularContent.h"
#include "lineRightBox.h"

#define WM_USER_DRAG_BEGIN	WM_USER + 256
/*//---		����
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

//  CMap info_jishu	��ż���ָ�����Ϣ����Ԫ��Ϊ��CFormularContent�Ķ��� 
//  ÿ����������ָ������ݽṹ(�������ʽ����)��
#define CHUQ_PER_BLK				6		//ÿС��ĳ�Ȩ���ݵĸ���
#define KLINE_PER_BLK				256		//ÿС���K�����ݵĸ���
#define NUMBYTE_PER_KLINE			32		//ÿ��K�����ݵ��ֽ���
#define NUMBYTE_PER_CHUQ			32		//ÿ����Ȩ���ݵ��ֽ���
#define NUM_STK_K_TOTAL				4096	//��Ʊ����

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

//������ͼ�����
#define  REGION_PICTBORDER1			1
#define  REGION_PICTBORDER2			2
#define  REGION_PICTBORDER3			3
#define  REGION_PICTBORDER4			4

//������ͼ����ͼ��
#define  REGION_PICT1				5
#define  REGION_PICT2				6
#define  REGION_PICT3				7
#define  REGION_PICT4				8
#define  REGION_PICT5				9

//���ͼ�������±߱��
#define  REGION_RULER				11

#define  REGION_TRANSFER			12

//������ͼ�ı�����
#define  REGION_PICTCAPTION1		14
#define  REGION_PICTCAPTION2		15
#define  REGION_PICTCAPTION3		16
#define  REGION_PICTCAPTION4		17
#define  REGION_PICTCAPTION5		18

//������ͼ����Ӧ�ı����
#define  REGION_PICTTEXT1		24
#define  REGION_PICTTEXT2		25
#define  REGION_PICTTEXT3		26
#define  REGION_PICTTEXT4		27
#define  REGION_PICTTEXT5		28


#define  REGION_MANY_FIGUER		36

//���½ǵ�Tab��
#define  REGION_LITTLE_FIGUER	37

#define  REGION_SUB				46
#define  REGION_ADD				47
#define  REGION_INFO_TRANSFER	48

#define  REGION_SELFLINE_1		58
#define  REGION_SELFLINE_2		59
#define  REGION_SELFLINE_3		60
#define  REGION_SELFLINE_BODY	61
#define  REGION_RIGHT_LINE		70

#define  REGION_ZOOM            80    //cjg��Ϣ���Ŵ���С��

#define  XIA_DIE_WORD       15
#define  PING_PAN_WORD      14



/*17���Ի������ݵ��ڴ��еĴ洢�ṹ��
CArray		lineself		 ��Ÿ����Ի������� ����Ԫ��ΪINFO_LINESELF�ṹ�Ķ���
INFO_LINESELF�ṹ�ĳ�Ա���£�
	int 	type		
	int	timeFirst
	int 	timeScd
	float		valFir
	float		valScd
*/
struct INFO_LINESELF{
	int 	type;		//�ߵ�����
	int     klineType;	//K������
	int		time1;		//�������ʱ��ֵ
	float	valFlt1;	//������ĸ�����ֵ
	int		time2;		//�������ʱ��ֵ
	float	valFlt2;	//������ĸ�����ֵ
	int		time3;		//�������ʱ��ֵ
	float	valFlt3;	//������ĸ�����ֵ
	COLORREF	color;	//�ߵ���ɫ
	BYTE	byLineKind;	//�ߵ�����:0��ʵ��,1������,2�ǵ����
	BYTE	byLineWidth;//�ߵĿ��:0��1����,1��2����,2��3����
	BYTE	byAlarm;	//�Ƿ�������Ԥ��,ֻ����������Ч
	BYTE	byWhichFiguer;//Ԥ��
	int     nDays;		//����������
	int     stored;		//Ԥ��
	INFO_LINESELF()
	{
		byWhichFiguer = 0;
	}
};

//
typedef struct{
	char ZhBName[20];		//ָ������
 	int  m_yBottom;			//���εĵײ���y����
}INDEX_BELLOW;

typedef struct{
//	char stockName[16];
	int  rightX;		//�ж��Ƿ���ָ��Ļ�ͼ������Ҳ���x����
	INDEX_BELLOW rtBlw[5];		//ָ��
}RECTKLINE;

 typedef struct{
	int  rightX;		//�ж��Ƿ���ָ��Ļ�ͼ������Ҳ���x����
	int  leftX;			//�ж��Ƿ���ָ��Ļ�ͼ������󲿵�x����
	INDEX_BELLOW rtBlw[5];		//ָ��
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
	int     day;//ʱ��
	float	open;//���̼�
	float   high;//��߼�
	float   low;//��ͼ�
	float   close;//���̼�
	float   vol;//�ɽ���
	float   amount;//�ɽ����
	union
	{
		struct
		{
			WORD    advance;//���Ǽ���
			WORD    decline;//�µ�����
		};
		float   volPositive;//��������
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

struct POWER       //��32���ֽ�
{
	int   nFlags;     //��Ȩ�����࣬0Ϊ�����ͺ��䡱��1Ϊ��������͡���2Ϊ���ɱ����䡱
	int   nTime;
	float fGive;      //song
	float fAllocate;
	float fAllocatePrice;//
	float fDividend;
	BYTE  byFree[4];  //Ԥ��12���ֽ�
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
	WORD  advance;	//���Ǽ���
	WORD  decline;	//�µ�����
	float amount;	//�ɽ����
	Tidxd tidxd;	//������Ϣ
	float faccbaccs;//Ԥ��
};
struct  TRADE_DETAIL_INFO
{
	float unitCountVol;	//�����ҵ������������ֵ
	float unitCountPrc;	//�����ҵ������ۼ�ȥ��ǰ�۵ľ���ֵ�����ֵ
	BYTE buyP[5];		//�����ҵ�����ۣ�0����1��1����2��2����3��
	BYTE sellP[5];		//�����ҵ������ۣ�0����1��1����2��2����3��
	BYTE buyV[5];		//�����ҵ���������0����1��1����2��2����3��
	BYTE sellV[5];		//�����ҵ���������0����1��1����2��2����3��
};
union	TRADE_DETAIL_INDEX
{
	TRADE_DETAIL_INFO buySellInfo;//������Ϣ
	INDEX_INFO	indexInfo;//������Ϣ
};
struct  TRADE_DETAIL_H_PER
{
	float vol;	//�½���
	float price;//�ɽ���
	int time;	//ʱ��

	TRADE_DETAIL_INDEX buySellOrIndex;//��ϸ��Ϣ

};
//��ʷ�����ļ����������ݴ�Ϊһ���ļ����ļ���ʽ�������ļ���ʽһ����
//��ʷ���䴦�������װ��
typedef CTypedPtrList<CPtrList,TRADE_DETAIL_H_PER*> CBuySellList; 
typedef CArray<POWER,POWER&> powerArr; 
typedef union tagKlineEx
{
	struct
	{
		float buyPrice[5];	//��1--��3��
		float buyVol[5];		//��1--��3��
		float sellPrice[5];	//��1--��3��	
		float sellVol[5];		//��1--��3��
	};
	float fDataEx[20];			//����
} KlineEx;

//#define MANY_DAY_KLINE		15
//#define HS_KLINE			25
//#define MIN1_KLINE		35
//#define NUM_MANY_DAY		50

enum klineKind {
	MANY_DAY_KLINE	= 15,//������
	HS_KLINE		= 25,//�ֱʳɽ�
	MIN1_KLINE		= 35,//1������
	MIN5_KLINE		= 1,//5������	
	MIN15_KLINE		= 2,//15������
	MIN30_KLINE		= 3,//30������
	MIN60_KLINE		= 4,//60������
	DAY_KLINE		= 5,//����
	WEEK_KLINE		= 6,//����
	MONTH_KLINE		= 7,//����
	YEAR_KLINE		= 8,//��
	NUM_MANY_DAY	= 50 //
};
typedef struct tagINPUT_INFO
{
	char*		strSymbol;		//��Ʊ����
	bool		bIndex;				//����

	int			nNumData;		//��������(pData,pDataEx,pResultBuf��������)
	Kline*		pData;					//��������,ע��:��m_nNumData==0ʱ����Ϊ NULL
	KlineEx*	pDataEx;				//��չ����,�ֱʳɽ�������,ע��:����Ϊ NULL

	ARRAY_BE	fInputParam[10]	;//���ò���
	ARRAY_BE	fOutputParam;	//�������
	klineKind	klineType;			//��������
	BASEINFO*	financeData;			//��������
	CReportData* pDat1;//��������
	int			nReserved;
	void*		pBuySellList;
	int			m_stkKind;
} INPUT_INFO;

 struct SUB_FIGUER_INIT{
	float	    yFloatBottom;	//K��ģ����ÿ����ͼ���εĵײ������λ�á�
	char		nameIndex[20];	//ÿ����ͼ�ļ���ָ������֡�
 };

////////INFOINIT �ṹ��
 typedef struct{
		int					nCountIndex;		//��ͼ�ĸ���
		SUB_FIGUER_INIT		initIndex[5];		//��ͼ�ĳ�ʼ����Ϣ��	�ĸ���ͼ����Ը߶ȵĺ�Ϊ1��

		int					nCountMin1;			//��ͼ�ĸ���
		SUB_FIGUER_INIT		initMin1[5];		//��ͼ�ĳ�ʼ����Ϣ��	�ĸ���ͼ����Ը߶ȵĺ�Ϊ1��
		int					flag[5]	;			//ÿ����ͼ��ָ���ID�š�0Ϊ��ʱ���ƣ�
												//1Ϊ�ɽ�����2Ϊ����������3Ϊ����ָ�꣬
												//4Ϊ����ָ�꣬5Ϊ���ָ��
		int					flag_dapan[5]	;	//��ָ����ָ��ÿ����ͼ��ָ���ID�š�

		int					nIDRightSheet[10];
 }SUB_FIGUER_INIT_INFO;

struct  TRADE_DETAIL_BIG_LIST_PER
{
	float vol;	//�½���
	float price;//�ɽ���
	int   time;	//ʱ��
	float buyPrice[5];	//��1--��3��
	float buyVol[5];		//��1--��3��
    float sellPrice[5];	//��1--��3��	
	float sellVol[5];		//��1--��3��
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