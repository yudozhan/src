/****************************************************************************************
*RainStockDrv.h                  ��ʱ������ӿ�5.0                          *
*                                                                                       *
*������ʱ�䣺                           2003.12.05                                     *
*****************************************************************************************/

/****************************************************************************************
*                   ��ע���� HKEY_LOCAL_MACHINE/SOFTWARE/StockDrv ��                  *
*                   Driver  = "����DLLȫ·��"                                           *
*                   Provider= "http://www.rainstock.com"                                *
*                                                                                       *
*                                                                                       *
*                                                                                       *
*                                                                                       *
*   ==========================      ���ֽڶ������      ===========================     *
*                                                                                       *
*                                                                                       *
****************************************************************************************/
#ifndef __STOCKDRV_H__
#define __STOCKDRV_H__

//#define _USE_32BIT_TIME_T //Marenan add

//ͼ�Ŀ�������Ϣ����

// ������ʽ���Ͷ���
#define RCV_WORK_DEMAND             0                   // ��ѯ�ӿ�,��ʽһ
#define RCV_WORK_MEMSHARE           2                   // �ڴ湲��ֱ������
#define RCV_WORK_SENDMSG            4                   // �汾 2 ����ʹ�õķ�ʽ    
#define RCV_WORK_AUTH               6                   // �������Ȩ��ʽ

// ����������ʽ����
#define RCV_WORK_RITE_ACTIVE        0xA95                   //��������ģʽ��������չ�ӿ�



#define  RCV_BUYSELL_NUM            5                      //������ʾ��



// �г���ʶ����
#define     SH_MARKET_EX            'HS'        // �Ϻ�
#define     SZ_MARKET_EX            'ZS'        // ����
#define     EB_MARKET_EX            'BE'        //���ڶ����г�


// �ļ���������
// �ṹ������ʽ���ļ�����
#define		FILE_HISTORY_EX				2			// ����������
#define		FILE_MINUTE_EX				4			// ������������
#define     FILE_POWER_EX				6
#define     FILE_HISTORY_MINUTE_EX       81          //����ʷ���������
#define     FILE_BASEINFO_EX             10          //����ȫ����Ʊ��������
#define     FILE_DISPARTBARGAINING_EX    12          //���ֱʽ�����ϸ����
#define     FILE_STOCKLABELLIST_EX       14          //����ȫ����Ʊ�����
#define     FILE_SYSTEMRUN_EX            16          //����ϵͳ���в���
#define     FILE_INDEXATTRIBUTE_EX       18          //���̺��̾� 
#define     FILE_LOGINAUTH_EX            20          //�ͻ�������Ȩ
#define     FILE_TECHNICINDEX            22          //�ͻ��˼���ָ��
#define     FILE_BASE_EX				0X1000      // Ǯ�����ݻ��������ļ�,m_szFileName�������ļ���
#define     FILE_NEWS_EX				0X1002      // ������,��������m_szFileName����Ŀ¼������
#define     FILE_BZT_EX					0X2000      //����Ͷ����
#define	    FILE_IMTITLE_EX			    0x3001		// ��Ϣ���ױ���


#define     FILE_TRADEHISTORY_EX		0X5000     //���������̵ķֱ�����
#define     FILE_5MIN_K_EX				0X5001     //����5����K��
#define     FILE_TYPE_RES				-1          // ����

// ��Ϣ������
#define     News_Sha_Ex              2          // ��֤��Ϣ
#define     News_Szn_Ex              4          // ��֤��Ϣ
#define     News_Fin_Ex              6          // �ƾ�����
#define     News_TVSta_Ex            8          // ����̨֪ͨ
#define     News_Unknown_Ex          -1         // δ֪�ṩ��

//Definition For nInfo of Function GetStockDrvInfo(int nInfo,void * pBuf);
#define     RI_IDSTRING             1           // ��������,����(LPCSTR)������
#define     RI_IDCODE               2           // ����
#define     RI_VERSION              3           // ��������汾
#define     RI_ERRRATE              4           // ȡ�ŵ�����
#define     RI_STKNUM               5           // ȡ���й�Ʊ�ܼ���
#define     RI_V2SUPPORT            6           // ֧������SJS��ṹ
#define		RI_SUPPORTEXTHQ			 6
#define		RI_DATAREQUEST			    7			      // ˫����ӿڣ�֧���������� 

#define     STKLABEL_LEN            10          // �ɺ����ݳ���,�����г��ɺű������Ǯ��
#define     STKNAME_LEN             32          // ��������
#define     STKPINGYING_LEN         6

#pragma pack(push ,1)


//////////////////////////////////////////////////////////////////////////////////
//��Ԫ����ͷ
//��Ԫ����ͷ m_dwHeadTag == EKE_HEAD_TAG 
#define EKE_HEAD_TAG    0xffffffff

typedef struct  tagRCV_EKE_HEADEx
{
    DWORD   m_dwHeadTag;                                // = EKE_HEAD_TAG
    WORD    m_wMarket;                                  // �г�����
    char    m_szLabel[STKLABEL_LEN];                    // ��Ʊ����
}RCV_EKE_HEADEx;
//////////////////////////////////////////////////////////////////////////////////
//�������ݣ� ֧�������5��ʾ����������ṹ
typedef struct tagRCV_REPORT_STRUCTEx
{
  WORD	m_cbSize;									// �ṹ��С
  time_t	m_time;										// ����ʱ��
  WORD	m_wMarket;									// ��Ʊ�г�����
  char	m_szLabel[STKLABEL_LEN];					// ��Ʊ����,��'\0'��β
  char	m_szName[STKNAME_LEN];						// ��Ʊ����,��'\0'��β
  
  float   m_fLastClose;                               // ����
  float   m_fOpen;                                    // ��
  float   m_fHigh;                                    // ���
  float   m_fLow;                                     // ���
  float   m_fNewPrice;                                // ����
  float   m_fVolume;                                  // �ɽ���
  float   m_fAmount;                                  // �ɽ���
  
  float	m_fBuyPrice[3];								// �����1,2,3
  float	m_fBuyVolume[3];							// ������1,2,3
  float	m_fSellPrice[3];							// ������1,2,3
  float	m_fSellVolume[3];							// ������1,2,3
  
  float	m_fBuyPrice4;								// �����4 
  float	m_fBuyVolume4;								// ������4
  float	m_fSellPrice4;								// ������4 
  float	m_fSellVolume4;								// ������4
  
  float	m_fBuyPrice5;								// �����5
  float	m_fBuyVolume5;								// ������5
  float	m_fSellPrice5;								// ������5
  float	m_fSellVolume5;								// ������5
} RCV_REPORT_STRUCTEx;


typedef struct tagRCV_RAIN_REPORT_STRUCTEx				//
{
  WORD	m_cbSize;									// �ṹ��С
  time_t	m_time;										// ����ʱ��
  WORD	m_wMarket;									// ��Ʊ�г�����
  char	m_szLabel[STKLABEL_LEN];					// ��Ʊ����,��'\0'��β
  char	m_szName[STKNAME_LEN];						// ��Ʊ����,��'\0'��β

  float   m_fLastClose;                               // ����
  float   m_fOpen;                                    // ��
  float   m_fHigh;                                    // ���
  float   m_fLow;                                     // ���
  float   m_fNewPrice;                                // ����
  float   m_fVolume;                                  // �ɽ���
  float   m_fAmount;                                  // �ɽ���
  union {  
    struct //��m_szLabelΪָ��ʱ�� һ��
    {
      float fBuy,fSell; //  ί�� ί��
      WORD wUp,wDown,wEqual;  //���ǣ��µ���ƽ��
    };
    struct { 
      float	m_fBuyPrice[RCV_BUYSELL_NUM];								// �����1,2,3,4,5
      float	m_fBuyVolume[RCV_BUYSELL_NUM];							// ������1,2,3,4,5
      float	m_fSellPrice[RCV_BUYSELL_NUM];							// ������1,2,3,4,5
      float	m_fSellVolume[RCV_BUYSELL_NUM];							// ������1,2,3,4,5
    };
  };
} RCV_RAIN_REPORT_STRUCTEx;

//������������
//  ע:
//  ÿһ���ݽṹ��Ӧͨ�� m_time == EKE_HEAD_TAG,�ж��Ƿ�Ϊ m_head,Ȼ����������
typedef union tagRCV_HISTORY_STRUCTEx
{
    struct
    {
        time_t  m_time;             //UCT
        float   m_fOpen;            //����
        float   m_fHigh;            //���
        float   m_fLow;             //���
        float   m_fClose;           //����
        float   m_fVolume;          //��
        float   m_fAmount;          //��
        WORD    m_wAdvance;         //����,��������Ч
        WORD    m_wDecline;         //����,��������Ч
    };
    RCV_EKE_HEADEx  m_head;
}RCV_HISTORY_STRUCTEx;

//�����ʱ������
//  ע:
//      ÿһ���ݽṹ��Ӧͨ�� m_time == EKE_HEAD_TAG,�ж��Ƿ�Ϊ m_head,Ȼ����������
typedef union tagRCV_MINUTE_STRUCTEx
{
    struct{
        time_t  m_time;             // UCT
        float   m_fPrice;
        float   m_fVolume;
        float   m_fAmount;
    };
    RCV_EKE_HEADEx  m_head; 
}RCV_MINUTE_STRUCTEx;


typedef union tagRCV_TRADE_HISTORY_STRUCTEx
{
  struct
  {
    time_t  m_time;             // UCT
    float   m_fPrice;
    float   m_fVolume;
    float   m_fAmount;
    float	m_fBuyPrice[RCV_BUYSELL_NUM];								// �����1,2,3,4,5
    float	m_fBuyVolume[RCV_BUYSELL_NUM];							// ������1,2,3,4,5
    float	m_fSellPrice[RCV_BUYSELL_NUM];							// ������1,2,3,4,5
    float	m_fSellVolume[RCV_BUYSELL_NUM];							// ������1,2,3,4,5
  };
  RCV_EKE_HEADEx  m_head; 
}RCV_TRADE_HISTORY_STRUCTEx;

typedef union tagRCV_MIN5_K_STRUCTEx
{
  struct
  {
    time_t  m_time;             //UCT
    float   m_fOpen;            //����
    float   m_fHigh;            //���
    float   m_fLow;             //���
    float   m_fClose;           //����
    float   m_fVolume;          //��
    float   m_fAmount;          //��
    WORD    m_wAdvance;         //����,��������Ч
    WORD    m_wDecline;         //����,��������Ч
  };
  RCV_EKE_HEADEx  m_head;
}RCV_MIN5_K_STRUCTEx;
//////////////////////////////////////////////////////////////////////////////////

//============================�ɽṹ==============
typedef struct tagRCV_REPORT_STRUCTEx_OLD
{
	WORD	m_wMarket;									// ��Ʊ�г�����
	char	m_szLabel[STKLABEL_LEN];					// ��Ʊ����,��'\0'��β
	char	m_szName[STKNAME_LEN];						// ��Ʊ����,��'\0'��β
	
	float	m_fLastClose;								// ����
	float	m_fOpen;									// ��
	float	m_fHigh;									// ���
	float	m_fLow;										// ���
	float	m_fNewPrice;								// ����
	float	m_fVolume;									// �ɽ���,�Թ�Ϊ��λ,�ɵĺ�������Ʊ���Ͷ���
	float	m_fAmount;									// �ɽ���,��ԪΪ��λ

	float	m_fBuyPrice[3];								// �����1,2,3
	float	m_fBuyVolume[3];							// ������1,2,3
	float	m_fSellPrice[3];							// ������1,2,3
	float	m_fSellVolume[3];							// ������1,2,3
} RCV_REPORT_STRUCTEx_OLD;
//============================�ɽṹ===================
//////////////////////////////////////////////////////////////////////////////////
//�����
//========================���ݽṹ==================================
//----------------------------------------ϵͳ���в������-----------------------------------------------
typedef  struct  tagRCV_SYSTEMRUN_STRUCTEx
{
	BYTE  m_btBargainingTime[3];      //���̷����� 
	BYTE  m_btTodayInitialize;        //�����Ƿ��ʼ�� 
	BYTE  m_btCloseWork;              //�����Ƿ�����  
	long  m_BargainingDate;           //��������  
	float   m_fNewIndex[3];           //����ָ�� 
	float   m_fVolume[3];             //���³ɽ���
	float   m_fAmount[3];             //���³ɽ����
	float   m_fRedGreenArmy[3];       //���̺��̾�
}RCV_SYSTEMRUN_STRUCTEx;
//----------------------------------------�ͻ�������Ȩ����ȷ�ϰ�-------------------------------------
typedef struct tagRCV_LOGINAUTH_STRUCTEx
{
	BOOL  bAuthorizationState;              //��Ȩ״̬ 
	DWORD dwAuthorizationPurview;         //��Ȩ��������з�Χ;��û�н�Ϊ0��          
}RCV_LOGINAUTH_STRUCTEx;

//----------------------------------------��Ʊ�����-------------------------------------------------------
typedef  struct  tagRCV_STKLABEL_STRUCTEx
{
	WORD m_wMarket;                   //��Ʊ�г�����
	WORD m_wPos;                      //����һ�г������й�Ʊ����λ�� 
	char m_szLabel[STKLABEL_LEN];      //��Ʊ����
	char m_szName[STKNAME_LEN];       //��Ʊ����
	char m_szPingYing[STKPINGYING_LEN]; //��Ʊ����ƴ��
	float m_fLastClose;                    //����
	float m_fAverageVolumn;               //5����� 
}RCV_STKLABEL_STRUCTEx;
//-----------------------------------���̺��̾�������ָ�����ݽṹ------------------------
typedef struct tagRCV_INDEXATTRIBUTE_STRUCTEx
{
	time_t m_time;         //          ����ʱ��    
    short sec5;             //          ��Ȩָ��ֵ    
    short rp;               //          ���ָ����RPֵ
    short dp;               //          ���ָ����DPֵ
	short rsn;              //          ���̾� 
	short dnn;
}RCV_INDEXATTRIBUTE_STRUCTEx;
//----------------------------------��Ʊ��������-----------------------------------------------------------
typedef  struct tagRCV_BASEINFO_STRUCTEx
{
	char m_szLabel[STKLABEL_LEN];  //��Ʊ����
	float m_fZgb;                    //�ܹɱ�(���)
	float m_fGjg;                    //���ҹ�(���)
	float m_fFqrfrg;                  //�����˷��˹�(���)
	float m_fFrg;                    //���˹�(���)
	float m_fZgg;                   //ְ����(���)
	float m_fGzAg;                  //����A��(���)
	float m_fLtAg;                  //��ͨA��(���)
	float m_fHg;                    //�ȹ�(���)
	float m_fBg;                    //B��(���)
	float m_fZpg;                   //ת���(���)
	float m_fZzc;                   //���ʲ�(��Ԫ)
	float m_fLdzc;                  //�����ʲ�(��Ԫ)
	float m_fCqtz;                  //����Ͷ��(��Ԫ)
	float m_fGdzc;                  //�̶��ʲ�(��Ԫ)
	float m_fWxzc;                 //�����ʲ�(��Ԫ)
	float m_fLdfz;                  //������ծ(��Ԫ)
	float m_fCqfz;                 //���ڸ�ծ(��Ԫ)
	float m_fGgqy;                 //�ɶ�Ȩ��(��Ԫ)
	float m_fZbgjj;                 //�ʱ�������(��Ԫ)
	float m_fYygjj;                 //ӯ�๫����(��Ԫ)
	float m_fMgjz;                 //ÿ�ɾ�ֵ(Ԫ)
	float m_fGdqybl;               //�ɶ�Ȩ�����(%)
	float m_fMggjj;                //ÿ�ɹ�����(Ԫ)
	float m_fZyywsr;               //��Ӫҵ������(��Ԫ)
	float m_fZyywlr;               //��Ӫҵ������(��Ԫ)
	float m_fQtywlr;               //����ҵ������(��Ԫ)
	float m_fLrze;                 //�����ܶ�(��Ԫ)
	float m_fJlr;                  //������(��Ԫ)
	float m_fWfplr;               //δ��������(��Ԫ)
	float m_fMgsy;                //ÿ������(Ԫ)
	float m_fJzcsyl;                //���ʲ�������(%)
	float m_fMgwfplr;              //ÿ��δ��������(Ԫ)
	float m_fMgjzc;                //ÿ�ɾ��ʲ�(Ԫ)
}RCV_BASEINFO_STRUCTEx;
//-------------------------------------������ʷ�����K������----------------------------------------
typedef union tagRCV_HISMINUTE_STRUCTEx
{
	struct
	{
		time_t  m_time;                  //UCT
		float  m_fOpen;			          //����
		float	m_fHigh;			          //���
		float  m_fLow;                    //���
		float  m_fClose;                   //����
		float  m_fVolume;                 //��
		float  m_fAmount;                 //��
		//Marenan -
		//float   m_fActiveBuyVol;          //����������û�м���m_fActiveBuyVol=0
		//Marenan +
		WORD    m_wAdvance;         //����,��������Ч
		WORD    m_wDecline;         //����,��������Ч
	};
	RCV_EKE_HEADEx  m_head;
}RCV_HISMINUTE_STRUCTEx;
//------------------------------------------------����ϸ����-----------------------------------------------
typedef  union  tagRCV_DISPBARGAINING_STRUCTEx
{
	struct
	{
		time_t m_time;                      //�ɽ�ʱ�� 
		float  m_fNewPrice;                 // ����
		float  m_fVolume;                  // �ɽ���,�Թ�Ϊ��λ,�ɵĺ�������Ʊ���Ͷ���
		float  m_fAmount;                  //�ɽ���,��ԪΪ��λ
		float  m_fBuyPrice[3];              // �����1,2,3
		float  m_fBuyVolume[3];            // ������1,2,3
		float  m_fSellPrice[3];              // ������1,2,3
		float m_fSellVolume[3];             // ������1,2,3
	};
	RCV_EKE_HEADEx  m_head;
}RCV_DISPBARGAINING_STRUCTEx;
//-----------------------------�����Ȩ���ݽṹ---------------------------------------------------------
typedef union tagRCV_POWER_STRUCTEx
{
	struct
	{
		time_t m_time;             //UCT
		float m_fGive;             //ÿ����  
		float m_fPei;              //ÿ����   
		float m_fPeiPrice;          //��ɼ�   
		float m_fProfit;            //ÿ�ɺ��� 
	};
	RCV_EKE_HEADEx  m_head;
}RCV_POWER_STRUCTEx;

//--------------------------------����������ݽṹ-------------------------------------------------------
typedef struct tagRCV_WIDOFEREPORT_STRUCTEx
{
	RCV_REPORT_STRUCTEx *pMarketReport;
	float m_fActiveBuyVolumn;   //����������  
	float m_fConsignTotalBuyVolumn;  //ί�������� 
	float m_fConsignTotalSellVolumn; //ί��������
}RCV_WIDOFEREPORT_STRUCTEx;


#define		TITLE_LEN				68			// ��Ϣ���ױ���

typedef struct tagRCV_IMTITLE_STRUCTEx
{
  DWORD     m_dwuid;    //�����Ϣ���׵�ID
  time_t    m_time ;    //ʱ��
  WORD      m_market;   //�г�
  CHAR      m_szcode[STKLABEL_LEN]; //��ش���
  CHAR      m_szcategory[64];       //���
  CHAR      m_szsource[64];         //��Դ
  CHAR      m_sztitle[64];          //����
} RCV_IMTITLE_STRUCTEx;


//////////////////////////////////////////////////////////////////////////////////
// �ļ��������ݰ�ͷ
//  עһ:
//  m_wDataType == FILE_BASE_EX
//              m_dwAttrib = ��Ʊ֤ȯ�г�,m_szFileName�������ļ���
//              m_FileTime = �����������ļ���������
//  m_wDataType == FILE_NEWS_EX
//              m_dwAttrib = ��Ϣ��Դ,m_szFileName ����Ŀ¼���ļ���,Ŀ¼��Ϊ��Ϣ��Դ
//              m_dwSerialNo = ���к�
//              ��:  "�Ͻ�����Ϣ\\0501Z012.TXT","������\\XLD0001.TXT"
//  ע��:
//      �����ļ�ѭ������,ÿ���ļ���Ψһ�����к�,�Ա����ظ�����
typedef struct tagRCV_FILE_HEADEx
{
    DWORD   m_dwAttrib;                         // �ļ�������
    DWORD   m_dwLen;                            // �ļ�����
    union {
        DWORD   m_dwSerialNo;                   // ���к�,�Թ���
        time_t  m_FileTime;                     // �ļ�����ʱ��
    };
    char    m_szFileName[_MAX_PATH];            // �ļ��� or URL
} RCV_FILE_HEADEx;



//////////////////////////////////////////////////////////////////////////////////
//  ����֪ͨ��Ϣ
//  wParam = RCV_WPARAM;
//  lParam ָ�� RCV_DATA�ṹ;
//  ���� 1 �Ѿ�����, 0 δ������ܴ���,Ŀǰ�÷���ֵ������

#define RCV_REPORT          0x3f001234
#define RCV_FILEDATA        0x3f001235
#define RCV_REPORT_V5       0x3f001236

/************************************************************************/
/* RCV_REPORT_V5 ��ʾʵʱ����ṹ��tagRCV_RAIN_REPORT_STRUCTEx�ṹ����                                                                     */
/************************************************************************/


// עһ:
//    ��¼����ʾ�������ݺͲ�������(���� Header)�����ݰ���,���ļ���������, = 1
// ע��:
//    �� m_bDISK = FALSE, m_pData Ϊ��Ϣ���������ݻ�����ָ��
//
//       ******** ���ݹ���,�����޸����� **********
//
//       m_bDISK = TRUE,  m_pData Ϊ���ļ��Ĵ����ļ���,һ�����ֻ��
//       ��������ȴ��ļ��ô��̷�ʽ

typedef struct tagRCV_DATA
{
	int					m_wDataType;			// �ļ�����
	int					m_nPacketNum;			// ��¼��,�μ�עһ
	RCV_FILE_HEADEx		m_File;					// �ļ��ӿ�
	BOOL				m_bDISK;				// �ļ��Ƿ��Ѵ��̵��ļ�
	union
	{
		RCV_REPORT_STRUCTEx *m_pReport;                   //��������
		RCV_WIDOFEREPORT_STRUCTEx *m_pWideReport;         //������������� 
		RCV_HISTORY_STRUCTEx *m_pDay;                     //�������� 
		RCV_MINUTE_STRUCTEx *m_pMinute;                   //�������ʱ����   
		RCV_POWER_STRUCTEx *m_pPower;                     //��Ȩ����
		RCV_DISPBARGAINING_STRUCTEx *m_pDispBargaining;  //��ϸ����
		RCV_BASEINFO_STRUCTEx *m_pBaseInfo;               //��������
		RCV_STKLABEL_STRUCTEx *m_pStkLabel;               //������ձ�
		RCV_HISMINUTE_STRUCTEx *m_pHisMinute;             //����ʷ�������
		RCV_SYSTEMRUN_STRUCTEx *m_pSystemRun;            //ϵͳ���в���
		RCV_INDEXATTRIBUTE_STRUCTEx *m_pIndexAttribute;   //���̺��̾� 
		RCV_LOGINAUTH_STRUCTEx *m_pLoginAuth;             //������Ȩ״̬       
		void *m_pData;                                    //�ļ��ڴ�ָ��   
	};
} RCV_DATA;

//==================ͬ����ͨѶ��ӿ������ݵ���ṹ=====================
typedef struct RCV_COMMINTERFACE_STRUCTEx
{
	RCV_DATA *m_pRcvData;                       //���鴫������
	DWORD m_dwTransEnvironment;                 //���鴫�ͻ������� 
} RCV_COMMINTERFACE_STRUCTEx;


//-----------------------------------�ͻ����������Ͷ���-------------------------------------------------
typedef enum ClientTransmitAttributes{
	LoginAuthorization=0,            //������Ȩ 
    SystemRunAttribute,              //ϵͳ��������  
	InitStockCodeList,               //ȡ�ó�ʼ��ȫ����Ʊ��������   
    IndexRealtimeData,               //ȡ��ĳһ����ָ����ʱ��������1A0001��2A01   
	StockRealtimeData,               //ȡ��ĳһ��Ʊ��ʱ��������
	StockMinuteData,                 //ȡ��ĳһ��Ʊ�����������
    StockRealtimeDataKLine,          //ȡ��ĳһ��Ʊ��K�߽��漴ʱ��������
	StockDisPartData,                //ȡ��ĳһ��Ʊ�ֱ���ϸ����
    StockTypesBaseRealTimeData,      //ȡ��ĳһ���Ʊ������������ ĳһ���Ʊȫ����ߡ���͡���������  
    StockTypesRealTimeMarketData,    //ȡ��ĳһ���Ʊ��������   ĳһ���Ʊȫ�����¡��ɽ��������
    StockTypesRealTimeMarketWithMinute,      //ȡ��ĳһ���Ʊ��ʱ��������   ĳһ����ȫ����Ʊ��ʱ����
	HistoryDayLineData,              //���ڱ�ʾ��ʷ�������� 
	HistoryMinuteLineData,           //���ڱ�ʾ��ʷ��ʱ���� 
	HistoryDisPartBargainingData,    //���ڱ�ʾ��ʷ��ϸ����
	WideExRightData,                     //���ڱ�ʾ��Ȩ����
	WideFinancingData,                   //���ڱ�ʾ��������
	WideStockBaseInfo,                   //���ڱ�ʾ��������
    WideShInfomation,                    //���ڱ�ʾ��֤����
	WideSzInfomation,                    //���ڱ�ʾ��֤����  
    WideEbInfomation,                    //���ڱ�ʾ���幫��  
	WideZjInfomation,                    //���ڱ�ʾ�ƾ�����
	WideTechnicIndex,                    //���ڱ�ʾ����ָ�� 
	WideNotifyInfomation                 //���ڱ�ʾ֪ͨ��Ϣ
};
typedef struct tagTransmitStockAttribute
{
	WORD m_wMarket;                                  //��Ʊ�г�����
	char m_szTransmitStockLabel[8];                  //���͹�Ʊ����
	DWORD m_dwTransmitCounts;                        //��ʾ��¼��     
}TransmitStockAttribute, *PTransmitStockAttribute;   //���͹�Ʊ����ָ��
//=====================�ͻ����������ݽṹ============================
typedef struct tagTRANS_PACKAGE_STRUCTEx
{
	ClientTransmitAttributes  m_TransmitType;    //�ͻ��������ݰ�����  
	DWORD m_dwTransmitStockType;                 //���ڱ�ʾ�������  
	union
	{
		struct
		{
			TransmitStockAttribute *m_pTransmitStockAttribute;
			DWORD m_dwTransmitStockCount;                        //��ʾ���͹�Ʊ��
		};
		DWORD m_dwTransmitStockTypeCounts;                       //��ʾ��¼��,���ڰ��
	};
} TRANS_PACKAGE_STRUCTEx;
/*�ṹ˵����m_TransmitType���ڱ�ʾ�ͻ����������������
m_dwTransmitStockType���ڱ�ʾ��鴫��
m_dwTransmitStockType== 	SHZS��ʾ��ָ֤�� 
m_dwTransmitStockType== SHAG,��ʾ��֤A�� 
m_dwTransmitStockType==SHBG,��ʾ��֤B�� 
m_dwTransmitStockType==SHZQ��ʾ��֤ծȯ
m_dwTransmitStockType==SZZS��ʾ����ָ��
m_dwTransmitStockType==SZAG,��ʾ����A��
m_dwTransmitStockType==SZBG,��ʾ����B��
m_dwTransmitStockType==SZZQ��ʾ����ծȯ
m_dwTransmitStockType==EBZS��ʾ����ָ��   
m_dwTransmitStockType==EBAG,��ʾ�����Ʊ
������ʾ��һ��Ʊ����Ʊ����
m_pTransmitStockAttributeΪ�ṹ����ָ�룬���ڱ�ʾ��һ��Ʊ����Ʊ��������˵��
����m_szTransmitStockLabelΪ��Ʊ���룬m_dwTransmitCounts��ʾ������˵�����ڲ�ͬ�������ݴ����н���ʾ��ͬ����
m_dw TransmitStockCountΪ��ʾ��Ʊ����
m_dwTransmitStockTypeCounts���ڱ�ʾ��鴫��ʱ��������˵����
*/


#ifndef __RAINSTOCKDRV_H__
#define __RAINSTOCKDRV_H__



//�����������ݵ���Ϣ�ṹ
#define REQUEST_DYNAMIC			0X0001			//����ֱ�����
#define REQUEST_MIN				0X0002			//�����ʱ����
#define REQUEST_MIN5			0X0004			//����5��������
#define REQUEST_DAY				0X0008			//������ʷ��K������
#define REQUEST_BASE			0X0010			//�������������
#define REQUEST_STOCKTABLE		0X0020			//�����Ʊ�����
#define REQUEST_NEWS			0X0040			//�������Ź�������

#define REQUEST_FINDATA   0X0A00      //����������� 
#define REQUEST_PWRDATA   0X0A01      //�����Ȩ����
#define REQUEST_5MKDATA   0x0A02      //�����K��


//������֤
#define  REQUEST_AUTH     0XFF00    


typedef struct tagDRV_SEND_REQUEST_INF
{
  DWORD   m_size ;        //�ṹ���ȣ� ����ʶ��汾
	WORD		m_wType;				//�������������
  WORD		m_wMarket;				//��Ʊ�г�����
  TCHAR   m_szCode[STKLABEL_LEN];	//��Ʊ���룬��'\0'����
	time_t		m_tmBegin;				//�������ʼʱ��
	time_t		m_tmEnd;				//�������ֹʱ��
  DWORD     m_dwlen ;       //�������ݽṹ�ĳ��ȣ�����pBufָ�������
  CHAR      m_pBuf[1];       //������չ����
}DRV_SEND_REQUEST_INF;

/*
    ��SEND_REQUEST_INF�ṹ���˵����

	��m_wType=REQUEST_DYNAMIC ʱ������ֱ�����
			m_szCode : ��Ʊ����
			m_wMarket: �г�����
			m_tmBegin: �����Ľ�����,ʱ�侫ȷ����,Сʱ���ֲ���
			m_tmEnd  : (����)

	��m_wType=REQUEST_MIN ʱ�������ʱ����(�Ե�����ĸ�ʽ����)
			m_szCode : ��Ʊ����
			m_wMarket: �г�����
			m_tmBegin: �����Ľ�����,ʱ�侫ȷ����,Сʱ���ֲ���
			m_tmEnd  : (����)

	��m_wType=REQUEST_MIN5 ʱ������5��������(�Ե�����ĸ�ʽ����)
			m_szCode : ��Ʊ����
			m_wMarket: �г�����
			m_tmBegin: �����Ľ�����,ʱ�侫ȷ����,Сʱ���ֲ���
			m_tmEnd  : (����)

	��m_wType=REQUEST_DAY ʱ��������ʷ��K������(�Ե�����ĸ�ʽ����)
			m_szCode : ��Ʊ����
			m_wMarket: �г�����
			m_tmBegin: ��ʼ������,ʱ�侫ȷ����,Сʱ���ֲ��ơ�m_tmBegin=0,ȡ������K��
			m_tmEnd  : (����)

	��m_wType=REQUEST_BASE ʱ���������������(�Ե�����ĸ�ʽ����)
			m_szCode : ��Ʊ����
			m_wMarket: �г�����
			m_tmBegin: (����)
			m_tmEnd  : (����)

	��m_wType=REQUEST_STOCKTABLE ʱ�������Ʊ�����
			m_szCode : (����)
			m_wMarket: (����)
			m_tmBegin: (����)
			m_tmEnd  : (����)

  	��m_wType=REQUEST_NEWS ʱ���������Ź�������(�Ե�����ĸ�ʽ����)
			m_szCode : (����)
			m_wMarket: (����)
			m_tmBegin: (����)
			m_tmEnd  : (����)
*/



/*****************************************************************

�ⲿ���Ѿ����ϣ�Ϊ���ּ��ݣ�������Ȼ֧�֣����������ڴ˻����Ͻ��п���, ��Ӧ���ܽ������DRV_SEND_REQUEST_INF�ṹʵ��

******************************************************************/ 
#define DA_SERVICE_MSG_EXT      WM_APP + 255    //��չ�ӿ�
#define	DA_SERVICE_MSG			WM_APP + 256  

#define RCV_EXT_TRADEDATA   0X5A11      //������3�ķֱ�����  STCP_QBD_STRUCT
#define RCV_EXT_KDATA       0X5A21      //������������  RITK_STRUCT 
#define RCV_EXT_TRADEDATAV2 0X5A15      //������4�ķֱ�����, STCP_QBD6_STRUCT
#define RCV_EXT_STOCKTABLE  0x5A22      //��Ʊ�����    RCV_STOCK_CELLEx
#define RCV_EXT_COMPASSRIGHT 0X5B20     //ָ������ͨ������
#define	RCV_EXT_M5K		      0x5A23		//�����K��

typedef struct _STRUCT_REXT_HEAD_
{
    TCHAR       m_Code[STKLABEL_LEN];
    TCHAR       m_Name[STKLABEL_LEN];
    WORD        m_wMarket;  // �г�����
    WORD        m_Service;  //  ��������
                            /*
                            #define RCV_EXT_TRADEDATA   0X5A11      //������3�ķֱ�����  STCP_QBD_STRUCT
                            #define RCV_EXT_KDATA       0X5A21      //������������  RITK_STRUCT 
                            #define RCV_EXT_TRADEDATAV2 0X5A15      //������4�ķֱ�����, STCP_QBD6_STRUCT
                            #define RCV_EXT_STOCKTABLE  0x5A22      //��Ʊ�����    RCV_STOCK_CELLEx
                            #define  RCV_EXT_COMPASSRIGHT 0X5B20     //ָ������ͨ������
                            */
    int         m_num;  //���ױ���
    time_t      m_mt;   //����ʱ��
    DWORD       m_unused[4];
}STRUCT_REXT_HEAD;

typedef struct _STRUCT_STCP_QBD_
{
    time_t          fTime ;             //ʱ��(��ȷ����)fTime       
    float           fPrice ;            //��ǰ�ɽ���         fPrice     
    float           fVolumn ;           //�ܳɽ���           fVolumn    
    float           fAmount;            //�ܳɽ���           fAmount    
    float           fBuyP[5];           //�����             fBuyP[5]   
    float           fBuyV[5];           //������              
    float           fSellP[5];          //������             
    float           fSellV[5];          //������
} STCP_QBD_STRUCT ;       // 

typedef struct _STRUCT_STCP_QBD6_
{
    time_t          fTime ;                   //ʱ��(��ȷ����)fTime       
    float           fPrice ;            //��ǰ�ɽ���         fPrice     
    float           fVolumn ;           //�ܳɽ���           fVolumn    
    float           fAmount;            //�ܳɽ���           fAmount    
    float           fBuyP[5];           //�����             fBuyP[5]   
    float           fBuyV[5];           //������              
    float           fSellP[5];          //������             
    float           fSellV[5];          //������
} STCP_QBD6_STRUCT ;       //


//K�����ݽṹ
typedef struct _STRUCT_RITK_
{
    time_t  m_time;             //UCT   
    float   m_fOpen;            //����
    float   m_fHigh;            //���
    float   m_fLow;             //���
    float   m_fClose;           //����
    float   m_fVolume;          //��
    float   m_fAmount;          //��
}RITK_STRUCT ;  



//��Ʊ�����Ԫ�ṹ��
typedef  struct tagRCV_STOCK_CELLEx
{
    WORD    m_wMarket;              // ��Ʊ�г�����
    char    m_szLabel[STKLABEL_LEN];// ��Ʊ����,��'\0'��β
    char    m_szName[STKNAME_LEN];  // ��Ʊ����,��'\0'��β
}RCV_STOCK_CELLEx ;

#pragma pack(pop)


//////////////////////////////////////////////////////////////////////////////////
//APIs

#ifdef __cplusplus
extern "C"{
#endif

//////////////////////////////////////////////////////////////////////////////////
// ע�ắ��

// ��Ʊ��ʼ��
// ��ڲ���:
//      hWnd            ������Ϣ�Ĵ��ھ��
//      Msg             �û��Զ�����Ϣ
//      nWorkMode       �ӿڹ�����ʽ,Ŀǰֻ��Ϊ RCV_WORK_MEMSHARE
// ���ز���:
//       1              �ɹ�
//      <0              ʧ��
// ע:
//      ע���,��������������ڷ�����Ϣ; ����ע��,ͨ����ѯ��ʽ���ȡ������
//      ����Ʊ����û����,��������Ʊ���ճ���
int WINAPI  Stock_Init(HWND hWnd,UINT Msg,int nWorkMode);

//���û���֤����������
int WINAPI  Stock_InitExt(HWND hWnd,UINT Msg,int nWorkMode, LPCSTR strUser, LPCSTR strPassword);

// �˳�,ֹͣ������Ϣ
// ��ڲ���:
//      hWnd            ������Ϣ�Ĵ��ھ��,ͬ Stock_Init �ĵ�����ڲ���
//  ���ز���:
//       1              �ɹ�    
//      -1              ʧ��
int WINAPI  Stock_Quit(HWND hWnd);

//////////////////////////////////////////////////////////////////////////////////
// ����ӿ�       
      
// ȡ�ѽ��յ��Ĺ�Ʊ����
int WINAPI  GetTotalNumber();                                                   

// �����ȡ��Ʊ����(��չ)
// ��ڲ���:    
//          nNo         ���
//          pBuf        ��Ź�Ʊ���ݵĻ�����
// ���ز���:
//          NoStockData �޹�Ʊ����  
// ע:
//          �ú����ṩ��Ʊ���ݵ���Ҫ����;�������������ʱ,���Կ��ٽ������ݿ��
int WINAPI  GetStockByNoEx(int nNo,RCV_REPORT_STRUCTEx * pBuf);         

// �ɹɺ�ȡ��Ʊ����(��չ)
// ��ڲ���:    
//          pszStockCode��Ʊ����    
//          nMarket     ֤ȯ�г�
//          pBuf        ��Ź�Ʊ���ݵĻ�����
// ���ز���:
//          NoStockData �޹�Ʊ����  
// ע:
//          �ú����ṩ��Ʊ���ݵ���Ҫ����;�������������ʱ,���Կ��ٽ������ݿ��
int WINAPI  GetStockByCodeEx(char * pszStockCode,DWORD nMarket, void * pBuf);

// ������ճ���,��������
// ��ڲ���:
//          bSetup      TRUE        ��ʾ����,��������
// ���ز���:
//           1          �ɹ�
//          -1          ʧ��
int WINAPI  SetupReceiver(BOOL bSetup);

//  ȡ�ù�Ʊ������Ϣ
//  ��ڲ���:
//          nInfo       ����
//          pBuf        ������
//  ���ڲ���:
//          nInfo == RI_IDSTRING,   ���������ַ�������, pBuf Ϊ�����ַ���
//                                  ��: "TongShi_StockDrv_1.00"
//          nInfo == RI_IDCODE,     ������Ϣ�� ID ��, pBuf Ϊ�ַ�����ʽ�� ID ��
//                                  ��: 0x78001234    "78001234"
//          nInfo == RI_VERSION,    ������Ϣ���汾��, pBuf Ϊ�ַ����汾
//                                  ��:  1.00         "1.00"        
//          nInfo == RI_ERRRATE,    ȡ�ŵ�����,
//          nInfo == RI_STKNUM,     ȡ���й�Ʊ�ܼ���
//          nInfo == RI_V2SUPPORT,  �����Ƿ�֧������SJS��ṹ, pBuf��Ч

DWORD  WINAPI GetStockDrvInfo(int nInfo,void * pBuf);

// ����:������������
// ��ڲ���:
//			lpInfo :�������ݵ���Ϣ������һ��ָ��SEND_REQUEST_INF�ṹ�������ָ�롣
//		����������ϸ��Ϣ����μ�SEND_REQUEST_INF�Ķ��塣
// ���ز���:
//			1		�ɹ�
//		   -1		ʧ��
int WINAPI QueryData(const void* pInfo);

#ifdef __cplusplus
}
#endif


#endif // __STOCKDRV_H__



//////////////////////////////////////////////////////////////////////////////////
//APIs

#ifdef __cplusplus
extern "C"{
#endif
/*************************************************************
****
****               һ�º����ӿڲ�����ʹ��
****
**************************************************************/
/************************������չ��Ϣ��־********************
******************************************************/
int WINAPI SetExtMsg(UINT msg);


/************************ȡ����K������********************
    scode: ��Ʊ���룬 ��'\0'��β
    mt   : ����������ʼ�����գ� ʱ�侫�ȵ��죬Сʱ���ֲ��ƣ� mt ��0��ʾȡ��������
    ���� : 1    �ɹ�
           -1   ʧ��
    ����������ͨ�ӿ��ĸ�ʽ����
******************************************************/
int WINAPI GetKData(LPCSTR scode, time_t mt);

int WINAPI QueryKData(LPCSTR scode, DWORD nMarket, time_t mt);
/************************ȡ��ʱ����********************
    scode: ��Ʊ���룬 ��'\0'��β
    mt   : ��ʱ���������׽����գ� ʱ�侫�ȵ��죬Сʱ���ֲ���
    ���� : 1    �ɹ�
           -1   ʧ��
    ��ʱ������ͨ�ӿ��ĸ�ʽ����
******************************************************/
int  WINAPI GetMinData(LPCSTR scode, time_t mt);


int  WINAPI QueryMinData(LPCSTR scode, DWORD nMarket, time_t mt);
/************************ȡ���׷ֱ�����********************
    scode: ��Ʊ���룬 ��'\0'��β
    mt   : ��ʱ���������׽����գ� ʱ�侫�ȵ��죬Сʱ���ֲ���
    ���� : 1    �ɹ�
           -1   ʧ��
    �ֱ����ݸ�ʽ������, ������׼���ú�ͨ����ϢDA_SERVICE_MSG_EXT֪ͨ������׼���á�

******************************************************/
int  WINAPI GetTradeData(LPCSTR scode, time_t mt);

int  WINAPI QueryTradeData(LPCSTR scode, DWORD nMarket, time_t mt);



int WINAPI GetInfoPath(CHAR* szPath, INT iPathLen, DWORD* pdwStoreType);

/************************ȡ���Ź�������********************
    channel: ����Ƶ���� ��'\0'��β
    fmt: ������Ŀǰ��Ч
    ���� : 1    �ɹ�
           -1   ʧ��
        
**********************************************************/  
int  WINAPI GetNews(LPCSTR  channel = NULL,  time_t fmt =0 );

/************************ȡ���ɻ���������********************
    scode: ��Ʊ���룬 ��'\0'��β
    fmt  : ����
    ���� : 1    �ɹ�
           -1   ʧ��
        
**********************************************************/  
int  WINAPI GetBaseInfo(LPCSTR  scode,  time_t fmt);

int  WINAPI QueryBaseInfo(LPCSTR scode, DWORD nMarket, time_t mt);

/***********************ȡ���й�Ʊ����***********************
     ͨ����Ϣ���ش����
     STRUCT_REXT_HEAD   *pHead  pHead->m_Service == RCV_EXT_STOCKTABLE ;
     RCV_STOCK_CELLEx   * pBuf ;
     
*************************************************************/
int  WINAPI GetStockTable();

/***********************ȡ������ʱ��***********************
     ���ڿͻ������������ʱ��ͬ��
     ���أ�TRUE �ɹ�
           FALSE ʧ��
*************************************************************/
BOOL  WINAPI GetServerTime(time_t *ptime);




/*************************����Ȩ֤�������������ʽ*******************
           CId�� Ԥ�����֤��Id������ָ�����CId��1, ÿ��Ҫ����֤�Ĺ�˾����ͳһ��ID
           Certification�� ֤������
           Length�� ֤�鳤��
           ����ֵ�� TRUE   ֤����Ч
                    FALSE  ֤����Ч
*************************************************************/
int WINAPI CStock_Init(HWND hWnd, UINT Msg, int nWorkMode,
                       DWORD CId ,
                       DWORD dwUsrInfoLen, BYTE* pUsrInfo, 
                       DWORD dwSignLen, BYTE* pSignBuf);




/*************************����Զ����Ͷ���*******************
           strMsg:   Ҫ���͵Ķ�������, �����������ó���200��
           strMobileList: ���Ž����ŵ��ֻ��ţ������Ƕ���ֻ���
           strSoftID:  �������ID�� 
           dwMsgType:  ��������
           ����ֵ�� TRUE   ֤����Ч
                    FALSE  ֤����Ч
*************************************************************/
int WINAPI SendSM(const char* strMobileList, const char* strMsg, const char* strSoftID, DWORD dwMsgType);

/********************************�����ʱ����******************
**************************************************************/
int WINAPI  QueryAllMinute();

/*******************************�����ļ�*********************
**************************************************************/
int WINAPI  QueryFile(LPCSTR filename, time_t mt);

/***********************ȡ��ͨ������**************************
              tLastUpdate: ������tLastUpdate�������
              nMarket:     �г�����
**************************************************************/
int  WINAPI QueryRightData(time_t tLastUpdate,  DWORD nMarket);


#ifdef __cplusplus
}
#endif

#endif // __RAINSTOCKDRV_H__



/* ͼ�Ŀ�������Ϣ�������ʾ��
LONG OnStkDataOK(UINT wParam,LONG lParam)
{
RCV_DATA    *       pHeader;
int         i;

    pHeader = (RCV_DATA *)lParam;

//  ���ڴ����ٶ�������������,��ý� pHeader->m_pData ָ������ݱ���,��������

    switch( wParam )
    {
    case RCV_REPORT:                        // �����������÷�ʽ,��Ʊ����
        for(i=0; i<pHeader->m_nPacketNum; i++)
        {
            pHeader->m_pReport[i] ...
            // ���ݴ���
        }
        break;

    case RCV_FILEDATA:                      // �����������÷�ʽ,�ļ�
        switch(pHeader->m_wDataType)
        {
        case FILE_HISTORY_EX:               // ����������
            break;

        case FILE_MINUTE_EX:                // ������������
            break;

        case FILE_BASE_EX:                  // Ǯ�����ݻ��������ļ�,m_szFileName�������ļ���
            break;

        case FILE_NEWS_EX:                  // ������,��������m_szFileName����Ŀ¼������
            break;
        }
        break;

    default:
        return 0;                           // unknown data
    }
    return 1;
}
*/

/******************************************��ʱͨ��չ���ݴ���ʾ��******************************************
void OnExtService(UINT wFileType, LONG lPara)
{
    STRUCT_REXT_HEAD *pHead = (STRUCT_REXT_HEAD*)lPara ;
    ASSERT(wFileType == pHead->m_Service);
    CString msg ;
    if ( wFileType == RCV_EXT_TRADEDATA)
    {
        msg.Format("%.6s %.8s %d �ֱ�" , pHead->m_Code, pHead->m_Name, pHead->m_num);
        STCP_QBD_STRUCT *pk = (STCP_QBD_STRUCT *) (lPara + sizeof(STRUCT_REXT_HEAD));

    }else if ( wFileType == RCV_EXT_TRADEDATAV2){
        msg.Format("��%.6s %.8s %d �ֱ�" , pHead->m_Code, pHead->m_Name, pHead->m_num);
        STCP_QBD6_STRUCT *pk = (STCP_QBD6_STRUCT *) (lPara + sizeof(STRUCT_REXT_HEAD));
    }else if ( wFileType == RCV_EXT_KDATA)
    {
        msg.Format("%.6s %.8s %d K��" , pHead->m_Code, pHead->m_Name, pHead->m_num);
        RITK_STRUCT *pk = (RITK_STRUCT *) (lPara + sizeof(STRUCT_REXT_HEAD));
    }else if (wFileType == RCV_EXT_STOCKTABLE)
    {
        msg.Format("��Ʊ�����" );
        RCV_STOCK_CELLEx *pk = (RCV_STOCK_CELLEx *) (lPara + sizeof(STRUCT_REXT_HEAD));
    }
}
*/



