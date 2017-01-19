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
//	SHZS=0,                           //��ָ֤�� 
//	SHAG=1,                             //��֤A�� 
//	SHBG=2,                             //��֤B�� 
//	SZZS=3,                              //����ָ��
//	SZAG=4,                              //����A��
//	SZBG=5,                              //����B��
//	SHZQ=6,                              //��֤ծȯ
//	SZZQ=7,                              //����ծȯ
//	SHJJ=8,                              //��֤jijin 
//	SZJJ=9,                              //��֤jijin 
//	EBZS=10,                              //����ָ��   
//	EBAG=11,                              //�����Ʊ
//	BLOCKINDEX=12,                          //���ָ��  
//	CHOOSESTK=20,                           //��ѡ�� 
//	QBGP=30                              //ȫ����Ʊ
//};
////////////////////////////////////////////////////////
//�ͻ���Ӧ�ò��������󲿷�
//
enum {
	SHZS=0,                           //��ָ֤�� 
	SHAG,                           //��֤A�� 
	SHBG,                           //��֤B�� 
	SHJJ,                        //��֤���� 
	SHZQ,                           //��֤ծȯ
	SHZZ,                           //��֤תծ
	SHHG,                           //��֤�ع�
	SHETF,                          //��֤ETF����
	SHKF,                     //��֤���Ż���
	SHQZ,                           //��֤Ȩ֤
	SHOT,                        //��֤����=10

	SZZS,                           //����ָ��=11
	SZAG,                          //����A��
	SZBG,                          //����B��
	SZJJ,                       //��֤���� 
	SZZQ,                          //����ծȯ
	SZZZ,                          //����תծ
	SZHG,                          //���ڻع�
	SZETF,                         //��֤ETF����
	SZKF,                    //��֤���Ż���
	SZQZ,                          //��֤Ȩ֤
	SZSB,						  //��֤����				
	SZOT,                       //��֤����
	SZZXQY,                        //��С��ҵ
	SZCYB,                         //��ҵ��=25

	BLOCKINDEX,                    //���ָ��  
	CHOOSESTK,                     //��ѡ�� 
	QBGP                           //ȫ����Ʊ=40

};

#define STOCKTYPENUM  CHOOSESTK + 1

enum {FLPAGE = 0,SHPAGE, SZPAGE ,AGPAGE,ZXPAGE,CYPAGE,BGPAGE,QZPAGE,JJPAGE,BLOCKPAGE,CHOOSEPAGE,TJXGPAGE,LASTPAGE};

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
	float Give;//ÿ����
	float Allocate;//ÿ����
	float AllocatePrice;//ÿ�����
	float Bonus;//ÿ�ɺ���
	BYTE Free[4];
}Split ,*PSplit;

//��Ʊ�����������ݽṹ
typedef struct
{
	char Symbol[STK_SYMBOL_LEN];  //��Ʊ����
	int  NumSplit;   //�Ѿ���Ȩ����
	float zgb;     //�ܹɱ�(���)
	float gjg;     //���ҹ�(���)
	float fqrfrg;  //�����˷��˹�(���)
	float frg;     //���˹�(���)
	float zgg;     //ְ����(���)
	float gzAg;    //����A��(���)    5
	float ltAg;    //��ͨA��(���)
	float Hg;      //�ȹ�(���)
	float Bg;      //B��(���)
	float zpg;     //ת���(���)

	float zzc;     //���ʲ�(��Ԫ)    10
	float ldzc;    //�����ʲ�(��Ԫ)
	float cqtz;    //����Ͷ��(��Ԫ)
	float gdzc;    //�̶��ʲ�(��Ԫ)
	float wxzc;    //�����ʲ�(��Ԫ)
	float ldfz;    //������ծ(��Ԫ)   15
	float cqfz;    //���ڸ�ծ(��Ԫ)
	float ggqy;    //�ɶ�Ȩ��(��Ԫ)
	float zbgjj;   //�ʱ�������(��Ԫ)
	float yygjj;   //ӯ�๫����(��Ԫ) 
	float mgjz;    //ÿ�ɾ�ֵ(Ԫ)    20
	float gdqybl;  //�ɶ�Ȩ�����(%)
	float mggjj;   //ÿ�ɹ�����(Ԫ)

	float zyywsr;  //��Ӫҵ������(��Ԫ)
	float zyywlr;  //��Ӫҵ������(��Ԫ)
	float qtywlr;  //����ҵ������(��Ԫ)25
	float lrze;    //�����ܶ�(��Ԫ)
	float jlr;     //������(��Ԫ)
	float wfplr;   //δ��������(��Ԫ)
	float mgsy;    //ÿ������(Ԫ)
	float jzcsyl;  //���ʲ�������(%)  30
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
	char Symbol[STK_SYMBOL_LEN];  //��Ʊ����
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
	long m_lBlockTypeCount;		//���������
	char m_szBlockTypeName[BLOCKTYPE_NUM][STK_NAME_LEN];	//��������
}BLOCKFILEHEAD;
typedef BLOCKFILEHEAD *PBLOCKFILEHEAD;

//���ָ�����Խṹ
typedef struct
{
    long  m_lIsUse;         //   �Ƿ�ʹ��
	char  m_szCode[STK_SYMBOL_LEN];      //   ���ָ������
    char  m_szName[STK_NAME_LEN];     //   ���ָ�����ƣ������������
	long  m_lStockCount;    //   ��Ʊ��
    float m_fTotalRight;   //   �����Ȩ��
	float m_fNewIndex;     //   ����ָ��
	float m_fYstIndex;     //   ����ָ��
	float m_fOpenIndex;	   //	����ָ��
	float m_fHighIndex;    //   �������
	float m_fLowIndex;     //   �������
	float m_fTotP;         //   ���³ɽ����  
	float m_fTotV;         //   ���³ɽ���
	float m_fTotRVol;      //   ����
	float m_fTotDVol;      //   ����
	float m_fLTP;			//	������
	float m_fMGSY;			//	ÿ������
	long  m_lUpStockCount;	//	�ǹ���
    int   m_iRightType;     //   ȡ��Ȩ������
    BOOL  m_bIsDelete;      //   ɾ����־
	float m_fRDR;			//	����Ƿ�
	char  m_szFirstStock[STK_NAME_LEN];	//���ǹ�Ʊ��
	char  m_szBlockTypeName[STK_NAME_LEN]; //  ���ķ����� ����ҵ������
}BLOCKINFO;

typedef BLOCKINFO *PBLOCKINFO;

//��Ʊ�ڰ�������Խṹ
typedef struct
{
	char	m_szSymbol[STK_SYMBOL_LEN];	//   ��Ʊ����
	BOOL	m_bDeleted;
	int		m_iPos;
	float	m_ClosePrice;
    float	m_fRight[MAX_RIGHTS];     //   ��ƱȨ��
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
	char	m_szSymbol[STK_SYMBOL_LEN];	//   ��Ʊ����
	BOOL	m_bDeleted;
	int		m_iPos;
	float	m_ClosePrice;
    float	m_fRight[MAX_RIGHTS];     //   ��ƱȨ��
    int		m_nBlockType[BLOCKTYPE_NUM];    
}STOCK_TYPE_INFO_OLD;
//======================
//CReportData�ṹ˵��
//CReportData�ṹ��Ҫ���ڼ��ռ�ʱ��Ʊ������Ϣ
typedef struct
{
 BYTE kind;
 BYTE  rdp[240];           //      ���ڼ������ǿ��ָ��
 char id[STK_SYMBOL_LEN];             //      ֤ȯ����
 char name[STK_NAME_LEN];           //     ֤ȯ���� 
 char Gppyjc[STK_PYJC_LEN];         //      ֤ȯ����ƴ�����
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
 float pbuy4;             //      ������� 
 float vbuy4;             //      ��������      
 float pbuy5;             //      ������� 
 float vbuy5;             //      ��������      
 float psel1;             //      ������һ
 float vsel1;             //      ������һ
 float psel2;             //      �����۶� 
 float vsel2;             //      ��������   
 float psel3;             //      ��������
 float vsel3;             //      �������� 
 float psel4;             //      ��������
 float vsel4;             //      �������� 
 float psel5;             //      ��������
 float vsel5;             //      �������� 
 float accb;              //      ����ҵ��ͣ���������ҵ��ͣ�
 float accs;              //      �����ҵ��ͣ����������ҵ��ͣ�
 float volume5;           //      5�����
 float rvol;              //      ����
 float dvol;              //      ����
 
 short lastclmin;         //      �ϴδ��������
 short initdown;          //      ��Ʊ��ʼ����־   0 = δ�ܳ�ʼ����1=�ѳ�ʼ��   
 int  InOut;			  //	  0:���̣�1�����̣�2��ƽ��
 BOOL  IsDelete;
 BOOL  IsMxTj;
 
 float Index[10];
 Kdata1   m_Kdata1[240];
 BASEINFO *pBaseInfo;

 STOCKINFOINBLOCK *pBlockInfo;

 int	m_serialNumber;		//		���ջ����ļ��й�Ʊ���ݵ����к�
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
 char name[STK_NAME_LEN];           //      ֤ȯ���� 
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

//���ָ�� = RP - DP

typedef struct  NameSymbolStr 
{
        char   stocksymblo[STK_SYMBOL_LEN];
		char   stockname[STK_NAME_LEN];
		char   stockhypy[STK_PYJC_LEN];
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
//��Ʊ����ʷ5���������ݽṹ
typedef struct  tagMIN5_TOTAL_STRUCTEx 
{
	RCV_HISMINUTE_STRUCTEx *m_RcvMin5;     //5�����߽ӿ�
	long Min5_Count;                       //5�����߼�¼��

}MIN5_TOTAL_STRUCTEx;

typedef struct  tagPOWER_TOTAL_STRUCTEx 
{
	RCV_POWER_STRUCTEx *m_RcvPower;     //�����߽ӿ�
	long Power_Count;                       //�����߼�¼��

}POWER_TOTAL_STRUCTEx;

/////////////////////////////////////////////////////////////
//our���ݴ洢��ʽ
struct TAI_SHAN_DATA
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
	char    StockSign[STK_SYMBOL_LEN]; //ǰ8���ֽ�Ϊ��Ʊ���롣
	short 	numChuQ	;	  //2���ֽ�Ϊ��Ȩ���ݵ��ܴ���
	long    numKln	;	  //4���ֽ����ͣ�������K�ߵĸ�����
	short   symBlock[25]; //����ÿ�����ֽڴ���K�����ݵĿ��
	long    index;
};

///////////5������ͷ�ļ��ṹ
struct MINUTEKLINEHEAD
{
	char    StockSign[STK_SYMBOL_LEN]; //ǰ8���ֽ�Ϊ��Ʊ���롣
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


typedef struct _ADD_DATA_DAY_LINE
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
    char StockTypeName[STK_NAME_LEN];        //�������
	char ScreenStockName[STK_NAME_LEN];      //����ѡ������ 
	int  ColumnIndex[30];          //��̬��ʾ���к�
	int  ColumnType;               //��̬��ʾ��Ŀǰ��ʾ��������
	int  ColumnWidth[30];          //��̬��ʾ���п��
	TECHNIQUE_COLUMN TechniqueColumn[MAXJISHUCOUNT];        ////��̬��ʾ��������
    char NetAddr[255];               //Ĭ����ַ
	char KswtAddr[255];              //����ί��ȱʡ·��
    int  RealTime;                  //ʵʱ����ʱ����
	int  BlockTime;                 //���ָ������ʱ����         
	TotalNineViewData m_TotalNineViewData;
	char ChooseTypeName[STK_NAME_LEN];        //��ѡ������
}SYSTEMINITDATA;	 







#endif // __WSSTOCKSTRUCT_1000_H__
