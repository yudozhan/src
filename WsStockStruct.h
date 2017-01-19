#ifndef __WSSTOCKSTRUCT_1000_H__
#define __WSSTOCKSTRUCT_1000_H__

#define CALLOC(n,x)    ((x *) farcalloc((long)(n),sizeof(x)))
#define FREE(x)        farfree(x)

#define MaxRights 3
#define MaxStockTYpe 102
#define MAXJISHUCOUNT 10


typedef struct
{
		float 	Price;      //    �۸� 
		float 	Volume;     //    �� 
		float 	Amount;     //    ��
    
}Kdata1;
typedef struct
{
		float 	Price;      //    �۸� 
		float 	Volume;     //    �� 
		float 	Amount;     //    ��
        float   accb;       //    ����ҵ��ͣ���������ҵ��ͣ�
        float   accs;       //    �����ҵ��ͣ����������ҵ��ͣ�
	    float   bsspbas;    //       ����ָ��
		float   dif_accb;   //       ��������ҵ�����ֵ������ - �ϴΣ�
		float   dif_accs;   //       ���������ҵ�����ֵ������ - �ϴΣ�
}DpKdata1;
//��Ȩ���ݽṹ
typedef struct 
{
	int   nFlags;
	int	  nTime;
	float Give;
	float Allocate;
	float AllocatePrice;
	float Bonus;
	BYTE Free[4];
}Split ,*PSplit;

//��Ʊ�����������ݽṹ
typedef struct
{
	char Symbol[10];  //��Ʊ����
	int  NumSplit;   //�Ѿ���Ȩ����
	float zgb;     //�ܹɱ�(���)
	float gjg;     //���ҹ�(���)
	float fqrfrg;  //�����˷��˹�(���)
	float frg;     //���˹�(���)
	float zgg;     //ְ����(���)
	float gzAg;    //����A��(���)
	float ltAg;    //��ͨA��(���)
	float Hg;      //�ȹ�(���)
	float Bg;      //B��(���)
	float zpg;     //ת���(���)

	float zzc;     //���ʲ�(��Ԫ)
	float ldzc;    //�����ʲ�(��Ԫ)
	float cqtz;    //����Ͷ��(��Ԫ)
	float gdzc;    //�̶��ʲ�(��Ԫ)
	float wxzc;    //�����ʲ�(��Ԫ)
	float ldfz;    //������ծ(��Ԫ)
	float cqfz;    //���ڸ�ծ(��Ԫ)
	float ggqy;    //�ɶ�Ȩ��(��Ԫ)
	float zbgjj;   //�ʱ�������(��Ԫ)
	float yygjj;   //ӯ�๫����(��Ԫ)
	float mgjz;    //ÿ�ɾ�ֵ(Ԫ)
	float gdqybl;  //�ɶ�Ȩ�����(%)
	float mggjj;   //ÿ�ɹ�����(Ԫ)

	float zyywsr;  //��Ӫҵ������(��Ԫ)
	float zyywlr;  //��Ӫҵ������(��Ԫ)
	float qtywlr;  //����ҵ������(��Ԫ)
	float lrze;    //�����ܶ�(��Ԫ)
	float jlr;     //������(��Ԫ)
	float wfplr;   //δ��������(��Ԫ)
	float mgsy;    //ÿ������(Ԫ)
	float jzcsyl;  //���ʲ�������(%)
	float mgwfplr; //ÿ��δ��������(Ԫ)

	float mgjzc;   //ÿ�ɾ��ʲ�(Ԫ)

	int m_iPos;
	BYTE  free[8];
	Split m_Split[80];
}BASEINFO;
typedef BASEINFO *PBASEINFO;

//===============�ɵĻ��������ļ��ṹ========================

//��Ʊ�����������ݽṹ
typedef struct
{
	char Symbol[8];  //��Ʊ����
	int  NumSplit;   //�Ѿ���Ȩ����
	float zgb;     //�ܹɱ�(���)
	float gjg;     //���ҹ�(���)
	float fqrfrg;  //�����˷��˹�(���)
	float frg;     //���˹�(���)
	float zgg;     //ְ����(���)
	float gzAg;    //����A��(���)
	float ltAg;    //��ͨA��(���)
	float Hg;      //�ȹ�(���)
	float Bg;      //B��(���)
	float zpg;     //ת���(���)

	float zzc;     //���ʲ�(��Ԫ)
	float ldzc;    //�����ʲ�(��Ԫ)
	float cqtz;    //����Ͷ��(��Ԫ)
	float gdzc;    //�̶��ʲ�(��Ԫ)
	float wxzc;    //�����ʲ�(��Ԫ)
	float ldfz;    //������ծ(��Ԫ)
	float cqfz;    //���ڸ�ծ(��Ԫ)
	float ggqy;    //�ɶ�Ȩ��(��Ԫ)
	float zbgjj;   //�ʱ�������(��Ԫ)
	float yygjj;   //ӯ�๫����(��Ԫ)
	float mgjz;    //ÿ�ɾ�ֵ(Ԫ)
	float gdqybl;  //�ɶ�Ȩ�����(%)
	float mggjj;   //ÿ�ɹ�����(Ԫ)

	float zyywsr;  //��Ӫҵ������(��Ԫ)
	float zyywlr;  //��Ӫҵ������(��Ԫ)
	float qtywlr;  //����ҵ������(��Ԫ)
	float lrze;    //�����ܶ�(��Ԫ)
	float jlr;     //������(��Ԫ)
	float wfplr;   //δ��������(��Ԫ)
	float mgsy;    //ÿ������(Ԫ)
	float jzcsyl;  //���ʲ�������(%)
	float mgwfplr; //ÿ��δ��������(Ԫ)

	float mgjzc;   //ÿ�ɾ��ʲ�(Ԫ)

	int m_iPos;
	BYTE  free[8];
	Split m_Split[80];
}BASEINFO_OLD;
typedef BASEINFO_OLD *PBASEINFO_OLD;

//===============�ɵĻ��������ļ��ṹ========================
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
	long m_lFileTitle;         //   �ļ���־
    long m_lFileExit;          //   �ļ������˳���־
	long m_lStockTypeCount;    //   �����
    long m_lStockTypeMaxCount; //   �������
    long m_lStockCount;        //   Ŀǰ��Ʊ����
    long m_lStockMaxCount;     //   ����Ʊ����
	long m_lLastTime;          //   �����ʱ��
//    BOOL m_bInitDone;          //   ��ʼ����־  
}STOCKTYPEHEAD;
typedef STOCKTYPEHEAD *PSTOCKTYPEHEAD;

//���ָ�����Խṹ
typedef struct
{
    long m_lIsUse;         //   �Ƿ�ʹ��
	char m_szCode[7];      //   ���ָ������
    char m_szName[12];     //   ���ָ������
	long m_lStockCount;    //   ��Ʊ��
    float m_fTotalRight;   //   �����Ȩ��
	float m_fNewIndex;     //   ����ָ��
	float m_fYstIndex;     //   ����ָ��
	float m_fOpenIndex;
	float m_fHighIndex;    //   �������
	float m_fLowIndex;     //   �������
	float m_fTotP;         //   ���³ɽ����  
	float m_fTotV;         //   ���³ɽ���
	float m_fTotRVol;      //   ����
	float m_fTotDVol;      //   ����
    int  m_iRightType;     //   ȡ��Ȩ������
    BOOL m_bIsDelete;      //   ɾ����־
}STOCKTYPEINFO;
typedef STOCKTYPEINFO *PSTOCKTYPEINFO;
//��Ʊ�ڰ�������Խṹ
typedef struct
{
	char m_szSymbol[9];	//   ��Ʊ����
	BOOL m_bDeleted;
	int m_iPos;
	float m_ClosePrice;
    float m_fRight[MaxRights];     //   ��ƱȨ��
    BYTE  m_btStockType[MaxStockTYpe];    
}STOCK_TYPE_INFO;
typedef STOCK_TYPE_INFO *PSTOCK_TYPE_INFO;
typedef struct
{
	STOCK_TYPE_INFO *pStockInfo;
}STOCK_POINT_INFO;

//======================
typedef struct
{
	char m_szSymbol[7];	//   ��Ʊ����
	BOOL m_bDeleted;
	int m_iPos;
	float m_ClosePrice;
    float m_fRight[MaxRights];     //   ��ƱȨ��
    BYTE  m_btStockType[MaxStockTYpe];    
}STOCK_TYPE_INFO_OLD;
//======================
//Cdat1�ṹ˵��
//Cdat1�ṹ��Ҫ���ڼ��ռ�ʱ��Ʊ������Ϣ
typedef struct
{
 BYTE kind;
 BYTE  rdp;           //      ���ڼ������ǿ��ָ��
 char id[8];             //      ֤ȯ����
 char name[10];           //     ֤ȯ���� 
 char Gppyjc[6];         //      ֤ȯ����ƴ�����
 int sel;
 float ystc;              //      ��������
 float opnp;              //      ���տ��� 
 float higp;              //      �������
 float lowp;              //      �������
 float nowp;              //      ���¼۸�
 float nowv;              //      ���³ɽ���
 float totv;              //      �ܳɽ���
 float totp;              //      �ܳɽ����
 float pbuy1;             //      �����һ
 float vbuy1;             //      ������һ
 float pbuy2;             //      ����۶�  
 float vbuy2;             //      ��������
 float pbuy3;             //      ������� 
 float vbuy3;             //      ��������      
 float psel1;             //      ������һ
 float vsel1;             //      ������һ
 float psel2;             //      �����۶� 
 float vsel2;             //      ��������   
 float psel3;             //      ��������
 float vsel3;             //      �������� 
 float accb;              //      ����ҵ��ͣ���������ҵ��ͣ�
 float accs;              //      �����ҵ��ͣ����������ҵ��ͣ�
 float volume5;           //      5�����
 float rvol;              //      ����
 float dvol;              //      ����
 
 short lastclmin;         //      �ϴδ��������
 short initdown;          //      ��Ʊ��ʼ����־   0 = δ�ܳ�ʼ����1=�ѳ�ʼ��   
 int  InOut;
 BOOL  IsDelete;
 BOOL  IsMxTj;
 
 float JiShu[10];
 Kdata1   m_Kdata1[240];
 BASEINFO *pBaseInfo;
 STOCK_TYPE_INFO *pStockTypeInfo;

 int	m_serialNumber;		//		���ջ����ļ��й�Ʊ���ݵ����к�
} Cdat1;
typedef Cdat1 *PCdat1;
typedef struct
{
 	Cdat1 *pItem;
	char StockId[7]; 
	float ItemData;
}STOCKDATASHOW;
typedef STOCKDATASHOW *PSTOCKDATASHOW; 


typedef struct
{
 char name[9];           //      ֤ȯ���� 
 float ystc;
 float nowp;             //      ���¼۸�
 float nowb;             //      ���������
 float nows;             //      ���������� 
 float nowv;              //      ���³ɽ���
 BOOL InOut;
 char kind;
}CUOHEDATA;

/*
��Kind ��Ʊ����˵��
��֤��Ʊ
a. ��id[0]<5 ����id[0]=7 ʱ Kind=7  ����ծȯ�������  ��Ŵ���QhzqSerl 
b. ��id[0]<9 ʱKind=1               ����A��          ��Ŵ���ShowSerl
c. ����         Kind=2               ����B��          ��Ŵ���ShowSerl
���ڹ�Ʊ
a. ��id[0]=6 ����id[0]=1 ����id[0]!=0 ʱ Kind=6 ծȯ�������
b. ��id[0]!=2 ����id[0]!=1                Kind=4    ����A��
c. ����                                    Kind=5    ����B��
*/

//Aidx1�ṹ˵��
//Aidx1�ṹ���ڴ��̼��ռ�ʱ������Ϣ
typedef struct
{
 float ystc;              //       ��������ָ��     
 float nowi;              //       ����ָ��
 float opni;              //       ����ָ��  
 float higi;              //       ���ָ��
 float lowi;              //       ���ָ��
 float nowp;              //       ��ʱ�ɽ����
 float nowv;              //       ��ʱ�ɽ�����
 float accb;              //       ����������ҵ�   
 float accs;              //       �����������ҵ�
 float bsspbas;           //       ί��ָ��
 float dif_accb;          //       ��������ҵ�����ֵ������ - �ϴΣ�
 float dif_accs;          //       ���������ҵ�����ֵ������ - �ϴΣ�
 float volume5;           //       5����� 

 float StartP;            //       ��ʼʱ��ָ�� 
 float EndP;              //       ����ʱ��ָ��


 float totp;              //       �ܳɽ����  
 float totv;              //       �ܳɽ���
 float lastaccb;          //       �ϴδ���������ҵ�
 float lastaccs;          //       �ϴδ����������ҵ�
 float lasttotb;          //                
 float lasttots;
 float lasttotp;          //       �ϴδ����ܳɽ����                
 float lasttotv;          //       �ϴδ����ܳɽ����� 

 DpKdata1 *m_DpKdate;     //        ���ڱ�����̷�����
} Aidx1;

//Rsdn1�ṹ˵��
typedef struct
{
 short rsn;       
 short dnn;
} Rsdn1;

typedef Rsdn1 **PRsdn1;

//Tidxd�ṹ˵��
//Tidxd�ṹ���ڼ�Ȩָ����������ָ����
typedef struct
{
 short sec5;             //          ��Ȩָ��ֵ    
 short rp;               //          ���ָ����RPֵ
 short dp;               //          ���ָ����DPֵ
 short res;
} Tidxd;
typedef Tidxd **PTidxd;


//ʵʱ��������ͷ�ṹ
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
//��Ʊ������������ͷ�ṹ
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

typedef struct _BIGBILL
{
	char id[8];
	char name[10];
	int nKind;
	time_t ntime;
	int  ntype; 
	float nowv; 
	float nowp;  
	float rdr;
}BIGBILL;
typedef BIGBILL *PBIGBILL;
typedef struct _BIGBILLHEAD
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
}BIGBILLHEAD;
typedef BIGBILLHEAD *PBIGBILLHEAD;
//
typedef struct _ADDSTOCKDATA
{
	char name[10];
	int  StockType;
}ADDSTOCKDATA;

//���ָ�� = RP - DP

typedef struct  NameSymbolStr 
{
        char   stocksymblo[7];
		char   stockname[9];
		char   stockhypy[5];
		char   group;
}NAMESYMBOL;

//��Ʊ�����������ݽṹ
typedef struct  tagMIN_TOTAL_STRUCTEx 
{
	RCV_MINUTE_STRUCTEx *m_RcvMinute;     //�����߽ӿ�
	long Min_Count;                       //�����߼�¼��

}MIN_TOTAL_STRUCTEx;

//��Ʊ���������ݽṹ
typedef struct  tagDAY_TOTAL_STRUCTEx 
{
	RCV_HISTORY_STRUCTEx *m_RcvDay;     //�����߽ӿ�
	long Day_Count;                       //�����߼�¼��

}DAY_TOTAL_STRUCTEx;
typedef struct  tagPOWER_TOTAL_STRUCTEx 
{
	RCV_POWER_STRUCTEx *m_RcvPower;     //�����߽ӿ�
	long Power_Count;                       //�����߼�¼��

}POWER_TOTAL_STRUCTEx;

/////////////////////////////////////////////////////////////
//our���ݴ洢��ʽ
struct WANSHEN
{
		int	    nTime;      // ��  �ڣ�����  
		float	fOpen;		// ���̼�
		float	fHigh;		// ��߼�
		float	fLow ;		// ��ͼ�
		float 	fClose;     // ���̼�
		float 	fVolume;    // �ɽ���
		float	fAmount;    // �ɽ����
		WORD    wAdvance;   // ���Ǽ���
		WORD    wDecline;   // �µ�����
};

////////////////////////////////////////////////
///���ߴ��ļ�ͷ
struct KLINEHEAD
{
	int	NumChuQPerBlk;	    	//ÿС��Ĺ�Ʊ��Ȩ���ݵ�����	
	int	numPerBlk	;			//ÿС��Ĺ�ƱK����������,256��
	int	numKline_sh	;			//��֤֤ȯ����
	int	numBlkAll_sh;			//��֤С�������
	int numKline_sz	;	        //��֤֤ȯ����
	int numBlkAll_sz;	        //��֤С������
};
///////////����С�ļ�ͷ
struct KLINESMALLHEAD
{
	char    StockSign[8]; //ǰ8���ֽ�Ϊ��Ʊ���롣
	short 	numChuQ	;	  //2���ֽ�Ϊ��Ȩ���ݵ��ܴ���
	long    numKln	;	  //4���ֽ����ͣ�������K�ߵĸ�����
	short   symBlock[25]; //����ÿ�����ֽڴ���K�����ݵĿ��
	long    index;
};

///////////5������ͷ�ļ��ṹ
struct MINUTEKLINEHEAD
{
	char    StockSign[8]; //ǰ8���ֽ�Ϊ��Ʊ���롣
	long    numKln	;	  //4���ֽ����ͣ���������ߵĸ�����
	long    pPos;         //д�������ָ��
	long    symBlock;     //���
};
////////////////////////////////
//5���ӽṹ
//1--4 �ֽ� �ļ�ͷ   
//5--8 ÿС��5����K���������� ����75�� 
//9--12 ��֤֤ȯ����
//13--16 ����
struct MKLINEHEAD
{
	int	numPerBlk	;			//ÿС��Ĺ�Ʊ5����K����������,75*48��
	int	numKline_sh	;			//��֤֤ȯ����
	int numKline_sz	;	        //��֤֤ȯ����
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


typedef struct _ADDDAYLINEDATA
{
	char DayLineName[10];
	int  DayLineType;
}ADDDAYLINEDATA;

typedef struct _tagIndex_Technique
{
       int  iKind;                      //��ʾ���������ࡣ
       char sIndex_name[50];            //ָ������ơ�
       int  iLine;                      //��ʾ�ڼ����ߡ�
       int  iDataType;                  //��ʾѡ�ɵ����͡�
       float fParam[8];                   //��ʾ����1��
       float fSubParam1;                //��ʾ�Ӳ���1��
       float fSubParam2;                //��ʾ�Ӳ���2��
       int  iSunIndex;                  //��ʾ�ڼ����ӹ�ʽ��
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
    char StockTypeName[20];        //�������
	char ScreenStockName[20];      //����ѡ������ 
	int  ColumnIndex[30];          //��̬��ʾ���к�
	int  ColumnType;               //��̬��ʾ��Ŀǰ��ʾ��������
	int  ColumnWidth[30];          //��̬��ʾ���п��
	TECHNIQUE_COLUMN TechniqueColumn[MAXJISHUCOUNT];        ////��̬��ʾ��������
    char NetAddr[255];               //Ĭ����ַ
	char KswtAddr[255];              //����ί��ȱʡ·��
    int  RealTime;                  //ʵʱ����ʱ����
	int  BlockTime;                 //���ָ������ʱ����         
	TotalNineViewData m_TotalNineViewData;
}SYSTEMINITDATA;	 







#endif // __WSSTOCKSTRUCT_1000_H__
