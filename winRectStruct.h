#ifndef	_RECT_HEADER_
#define	_RECT_HEADER_
;
//#include "equation.h"
#include <afxtempl.h>
#include "LJishu.h"
#include "lineRightBox.h"

#define WM_USER_DRAG_BEGIN	WM_USER+ 256
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

//  CMap		info_jishu	��ż���ָ�����Ϣ����Ԫ��Ϊ��LJishu�Ķ��� 
//  ÿ����������ָ������ݽṹ(�������ʽ����)��
#define CHUQ_PER_BLK		6	//ÿС��ĳ�Ȩ���ݵĸ���
#define KLINE_PER_BLK		256 //ÿС���K�����ݵĸ���
#define NUMBYTE_PER_KLINE	32 //ÿ��K�����ݵ��ֽ���
#define NUMBYTE_PER_CHUQ	32 //ÿ����Ȩ���ݵ��ֽ���
#define NUM_STK_K_TOTAL		4096 //��Ʊ����

#define	STCKNAMELEN		50
#define	FENSHINUM		240

//#define CAPTIONHEIGHT                    15
//#define MIDLEN                    50
//#define RIGHTLENGTH               172
#define MIDLEN_L                  40
//#define LEFT					  50
#define STATICID                       600667

#define WIDTH_KLINEBODY                    6
#define LEN_KLINE                       10


#define CORLOR_ZUOBIAO                    RGB(0,0,0)
#define RIGHTBOX_PERLINE                    18

//#define MIN_GRID                  25
//#define MIN_GRID_X                  55

#define RIGHTHEIGHT                      250

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

#define  HIT_NO		0

//������ͼ�����
#define  HIT_PICTBORDER1		1
#define  HIT_PICTBORDER2		2
#define  HIT_PICTBORDER3		3
#define  HIT_PICTBORDER4		4

//������ͼ����ͼ��
#define  HIT_PICT1		5
#define  HIT_PICT2		6
#define  HIT_PICT3		7
#define  HIT_PICT4		8
#define  HIT_PICT5		9

//���ͼ�������±߱��
#define  HIT_RULER		11

#define  HIT_TRANSFER		12

//������ͼ�ı�����
#define  HIT_PICTCAPTION1		14
#define  HIT_PICTCAPTION2		15
#define  HIT_PICTCAPTION3		16
#define  HIT_PICTCAPTION4		17
#define  HIT_PICTCAPTION5		18

//������ͼ����Ӧ�ı����
#define  HIT_PICTTEXT1		24
#define  HIT_PICTTEXT2		25
#define  HIT_PICTTEXT3		26
#define  HIT_PICTTEXT4		27
#define  HIT_PICTTEXT5		28


#define  HIT_MANY_FIGUER		36

//���½ǵ�Tab��
#define  HIT_LITTLE_FIGUER		37

#define  HIT_SUB				46
#define  HIT_ADD				47
#define  HIT_INFO_TRANSFER		48

#define  HIT_SELFLINE_1		58
#define  HIT_SELFLINE_2		59
#define  HIT_SELFLINE_3		60
#define  HIT_SELFLINE_BODY		61
#define  HIT_RIGHT_LINE		70

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
typedef struct{
	int 	type;//�ߵ�����
	int     klineType;//K������
	int		time1;//�������ʱ��ֵ
	float	valFlt1;//������ĸ�����ֵ
	int		time2;//�������ʱ��ֵ
	float	valFlt2;//������ĸ�����ֵ
	int		time3;//�������ʱ��ֵ
	float	valFlt3;//������ĸ�����ֵ
	COLORREF	color;//�ߵ���ɫ
	BYTE	byLineKind;//�ߵ�����:0��ʵ��,1������,2�ǵ����
	BYTE	byLineWidth;//�ߵĿ��:0��1����,1��2����,2��3����
	BYTE	byAlarm;//�Ƿ�������Ԥ��,ֻ����������Ч
	BYTE	byStored;//Ԥ��
	int     nDays;//����������
	int     stored;//Ԥ��
}INFO_LINESELF;

//
typedef struct{
	char ZhBName[20];		//ָ������
 	int  yBottom;			//���εĵײ���y����
}ZHIBIAOBELLOW;

typedef struct{
//	char stockName[16];
	int  rightX;		//�ж��Ƿ���ָ��Ļ�ͼ������Ҳ���x����
	ZHIBIAOBELLOW rtBlw[5];		//ָ��
}RECTKLINE;

 typedef struct{
	int  rightX;		//�ж��Ƿ���ָ��Ļ�ͼ������Ҳ���x����
	int  leftX;			//�ж��Ƿ���ָ��Ļ�ͼ������󲿵�x����
	ZHIBIAOBELLOW rtBlw[5];		//ָ��
}RECTFENSHI;
/*
class lineRightBox
{
public:
	lineRightBox()
	{
	};
	virtual ~lineRightBox()
	{
	};
}*/
struct LooseValue
{
	int nFoot;
	float fVal;
};
typedef CArray<LooseValue,LooseValue&> looseValueArray; 
class lineBE
{ 
public:
	float *line;
	int b;
	int e;

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
	CString s;
	lineRightBox lineRight;
	looseValueArray looseArr;

	lineBE()
	{
		s = "";
		line = NULL;
		kind = Normal;
		b=0;
		e = 0;
	};
	virtual ~lineBE()
	{
	};
	lineBE& operator=(lineBE& in)
	{
		this->b=in.b;
		e = in.e;
		kind = in.kind;
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


		return *this;
	};
	bool Add(lineBE& in1,lineBE& in2 )
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
	bool Add(lineBE& in1,float fIn2 )
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
	bool Sub(lineBE& in1,lineBE& in2 )
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
	bool Sub(lineBE& in1,float fIn2 )
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
	bool Sub(float fIn2,lineBE& in1 )
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
	bool Mult(lineBE& in1,lineBE& in2 )
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
	bool Mult(lineBE& in1,float fIn2 )
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
	bool Div(lineBE& in1,lineBE& in2 )
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
	bool Div(lineBE& in1,float fIn2 )
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
	bool Div(float fIn2,lineBE& in1 )
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

struct lineBE_SHOW
{ 
 	lineBE line_be;
	int bNow;
	int type;
	COLORREF	clr;
	lineBE_SHOW()
	{
		clr = -1;
	};
//	char name[10];
};
typedef struct{
	 int numLine;
	 lineBE_SHOW	line[18];
}lineJISHU;

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
}KLINEDATA2;

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
struct	CHOOSE_DETAIL_L           
{
	int buytime;
	int selltime;
	float  buyprice;
	float  sellprice;
	int    timerange;
	float  profit;
	BOOL   issuccess;
};
struct  CHOOSE_RESULT_L
{
	CString symbol;
	CString name;
	int     choosecount;
	int     successcount;
	float   successpercent;
	BOOL    isuseful;
	CArray <CHOOSE_DETAIL_L*,CHOOSE_DETAIL_L*> m_detail;
};

#define IDM_POP_TJXG_SHOW		300//
#define IDM_POP_KLINE_ADDED		301//
#define IDM_POP_CHANGE_PARAM		302//
#define IDM_POP_DRAG_MODE		303//
//#define ID_KLINE_MANY_DAY		304//

#define MANY_DAY_K_LINE		15
#define HS_K_LINE		25
#define MINUTE_1_K_LINE		35
#define NUM_MANY_DAY		50
struct  INDEX_INFO
{
	WORD  advance;	//���Ǽ���
	WORD  decline;	//�µ�����
	float amount;	//�ɽ����
	Tidxd tidxd;	//������Ϣ
	float faccbaccs;//Ԥ��
};
struct  BUY_SELL_INFO
{
	float unitCountVol;	//�����ҵ������������ֵ
	float unitCountPrc;	//�����ҵ������ۼ�ȥ��ǰ�۵ľ���ֵ�����ֵ
	BYTE buyP[3];		//�����ҵ�����ۣ�0����1��1����2��2����3��
	BYTE sellP[3];		//�����ҵ������ۣ�0����1��1����2��2����3��
	BYTE buyV[3];		//�����ҵ���������0����1��1����2��2����3��
	BYTE sellV[3];		//�����ҵ���������0����1��1����2��2����3��
};
union	BUYSELL_INDEX
{
	BUY_SELL_INFO buySellInfo;//������Ϣ
	INDEX_INFO	indexInfo;//������Ϣ
};
struct  BUY_SELL_H_PER
{
	float vol;	//�½���
	float price;//�ɽ���
	int time;	//ʱ��

	BUYSELL_INDEX buySellOrIndex;//��ϸ��Ϣ

};
//��ʷ�����ļ����������ݴ�Ϊһ���ļ����ļ���ʽ�������ļ���ʽһ����
//��ʷ���䴦�������װ��
typedef CTypedPtrList<CPtrList,BUY_SELL_H_PER*> CBuySellList; 
typedef CArray<POWER,POWER&> splitArray; 
typedef union tagKlineEx
{
	struct
	{
		float buyPrice[3];	//��1--��3��
		float buyVol[3];		//��1--��3��
		float sellPrice[3];	//��1--��3��	
		float sellVol[3];		//��1--��3��
	};
	float fDataEx[12];			//����
} KlineEx;

enum klineKind {
	HS_KLINE = 25,//�ֱʳɽ�
	MANY_DAY_KLINE	=	15,//������
	MINUTE1_KLINE	=	35,//1������
	MIN5_KLINE = 1,//5������	
	MIN15_KLINE = 2,//15������
	MIN30_KLINE = 3,//30������
	MIN60_KLINE = 4,//60������
	DAY_KLINE = 5,//����
	WEEK_KLINE = 6,//����
	MONTH_KLINE = 7,//����
	YEAR_KLINE = 8//��
};
typedef struct tagINPUT_INFO
{
	char*			  strSymbol;		//��Ʊ����
	bool			  bIndex;				//����

	int			  nNumData;		//��������(pData,pDataEx,pResultBuf��������)
	Kline*		  pData;					//��������,ע��:��m_nNumData==0ʱ����Ϊ NULL
	KlineEx*	      pDataEx;				//��չ����,�ֱʳɽ�������,ע��:����Ϊ NULL

	lineBE 		fInputParam[10]	;//���ò���
	lineBE 		fOutputParam;	//�������
	klineKind	  klineType;			//��������
	BASEINFO*    financeData;			//��������
	Cdat1* pDat1;//��������
	int nReserved;
	void* pBuySellList;
	int m_stkKind;
} INPUT_INFO;

 struct SUB_FIGUER_INIT{
	float	    yBottomFlt;			//K��ģ����ÿ����ͼ���εĵײ������λ�á�
	char		nameZhiBiao[20]	;	//ÿ����ͼ�ļ���ָ������֡�
 };

////////INFOINIT �ṹ��
 typedef struct{
		int				zhiBiaoNum;				//��ͼ�ĸ���
		SUB_FIGUER_INIT		zbInit[5];		//��ͼ�ĳ�ʼ����Ϣ��	�ĸ���ͼ����Ը߶ȵĺ�Ϊ1��

		int				fenShiNum;				//��ͼ�ĸ���
		SUB_FIGUER_INIT		fsInit[5];		//��ͼ�ĳ�ʼ����Ϣ��	�ĸ���ͼ����Ը߶ȵĺ�Ϊ1��
		int		flag[5]	;	//ÿ����ͼ��ָ���ID�š�0Ϊ��ʱ���ƣ�
							//1Ϊ�ɽ�����2Ϊ����������3Ϊ����ָ�꣬
							//4Ϊ����ָ�꣬5Ϊ���ָ��
		int		flag_dapan[5]	;	//��ָ����ָ��ÿ����ͼ��ָ���ID�š�

		int nIDRightSheet[10];
 }SUB_FIGUER_INIT_INFO;

struct  BUY_SELL_BIGBILL_PER
{
	float vol;	//�½���
	float price;//�ɽ���
	int   time;	//ʱ��
	float buyPrice[3];	//��1--��3��
	float buyVol[3];		//��1--��3��
    float sellPrice[3];	//��1--��3��	
	float sellVol[3];		//��1--��3��
};
#define FABSMY(x) ((x)<0?-(x):(x))

typedef struct tagSymbolKind{
	char m_chSymbol[8];
	int	 m_nSymbolKind;
	tagSymbolKind()
	{
		memset(m_chSymbol,0,8);
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
const CString g_strCompanyName = "ս�Լ�";
#define FILEID22	33133313
#endif