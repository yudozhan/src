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
	char    key[STK_PYJC_LEN];    //�ؼ���
	char    data[STK_NAME_LEN];   //��Ϣ��
	BYTE    id;                   //���
	BYTE	m_stkKind;
};

//�˴�����ϵͳ�Ļ���������ѡ��Ĳ��� 	
struct  SYSTEM_OPTION {
	int 		kline;		    //��ͼ��ʽ��K��1��������0��������2
	BOOL 		fourgraph;		//��ͼ��ϣ�4ͼ��9ͼ��16ͼ
	BOOL		autoclose;		//�Զ���������ҵ
	BOOL		autominute;		//�Զ���������
	BOOL		autoday;        //�Զ�������
	BOOL        showxline;		//��ʾ������
	BOOL        showyline;      //��ʾ������
	BOOL        organizedata;   //����ʱ�Զ�������������
	BOOL        alarmsound;     //��ʾ����������
};

//�˴�����ϵͳ�Ļ�������
struct SYSTEMDATA {
	 int        changepage;     //�Զ���ҳʱ��
	 int        AdvSpTimes ;    //���ٷ�����
	 int        fiveminutesave; //5�������ϴ洢ʱ��
	 int        noticesave;     //�������ϱ���ʱ��
	 int        middleprice;    //�мۼ��㷽ʽ
	 float      buyrate;        //�������׷���
	 int        AlarmPercent;   //��ʾ��
 };
//��¼·��
struct SETCURRENTDIRECTORY{
    char CurrentDir[255];
	char Free[255];
};
struct SYSTEMHQSET{
	 int       capital;       //�ʽ�
	 int       quangtity;     //����
	 int       swing;         //���
	 int       upspeed;       //����
	 int       changehand;    //������
 };

struct ALERTCHOICE{
	 float Vol;//�ɽ�����
	 float Per;//��ͨ�̰ٷֱ�
	 float Acount;//�ɽ����
	 int Format;//ͳ�Ʒ���ѡ��  0���ɽ����� 1 ��ͨ�̰ٷֱ� 2 �ɽ����
 };
 
 ////////property set �ṹ��
 typedef struct{
	 int        daysOfDayKline;			//���߱�������
	 int        daysOfMin5Kline ;		//5�����߱�������
	 int        daysOfManyKline;		//�����ߵ�����,����Ϊ������߷�����
	 int        countOfHistoryFile;		//��ʷ�������ݱ�������
	 int		nSecondsAlarm;			//Ԥ��ʱ����
	 int		nTestUnfinished;		//δ��ɵ��������

	 BOOL       bDoPowerSelectingStock;	//����ѡ�ɼ���ʱ�Ƿ���г�Ȩ
	 BOOL       bDoVolPowering;			//�Ƿ�Գɽ���,�ɽ������г�Ȩ
	 BOOL		bDoPowerAfter;			// �Ƿ���ú��Ȩ
	 BOOL       bAutoPower;				//�Ƿ��Զ���Ȩ
	 BOOL		bSaveParam;				//�Զ�����ָ�����
	 BOOL		bHideLine;				//�������Զ�����
	 BOOL		bShowCrossDlg;			//��ʮ�ֹ��״̬�£��Ƿ񵯳����鴰����ʾ��������
	 BOOL		bRunAlarmBegin;			//�ۿ����Զ�����Ԥ���ݿ�����ϵͳ�Զ�����Ԥ������δ���ñ������Ԥ�������ֹ�������
	 BOOL		bUseMin5ChenBen;		//�Ƿ�ʹ��5�����߼����ƶ��ɱ��ֲ�
     int        bTjxgZsFs;				//����ѡ��ָʾ�ķ�ʽ
	 float		LTP_rate;				//��ͨ�̱���
	 float		CJL_rate;				//�ɽ�������
	 float      fReserved[1];        //

 }PROPERTY_INIT;




//////////////// 
 typedef struct{
		float	    yFloatBottom;		//K��ģ����ÿ����ͼ���εĵײ������λ�á�
		char		nameIndex[10];		//ÿ����ͼ�ļ���ָ������֡�
 }Index_INIT;

////////INFOINIT �ṹ��
 typedef struct{
		int				nCountIndex;		//��ͼ�ĸ���
		Index_INIT		initIndex[4];		//�ĸ���ͼ�ĳ�ʼ����Ϣ��	�ĸ���ͼ����Ը߶ȵĺ�Ϊ1��

		int				nCountMin1;			//��ͼ�ĸ���
		Index_INIT		initMin1[4];		//�ĸ���ͼ�ĳ�ʼ����Ϣ��	�ĸ���ͼ����Ը߶ȵĺ�Ϊ1��
		int				flag[4]	;			//ÿ����ͼ��ָ���ID�š�0Ϊ��ʱ���ƣ�
											//1Ϊ�ɽ�����2Ϊ����������3Ϊ����ָ�꣬
											//4Ϊ����ָ�꣬5Ϊ���ָ��
		int				flag_dapan[4]	;	//��ָ����ָ��ÿ����ͼ��ָ���ID�š�
 }INFOINIT;

 //�˴������Ʊ����Ľṹ��������ѡ�ɡ����ɵ�ѡ��
typedef struct StockSymbolStr {
	char   sharesymbol[7];
	int    nKind;
}SharesSymbol;

 //�˴�������ɵĽṹ
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
	float  buyprice;//�����
	float  sellprice;//������
	int    timerange;//ʱ����
	float  profit;//������
	int   issuccess;//0 �ǲ��ɹ�,1 �ǳɹ�,2��û��������ڵļ���
	int	nKindSell;//����������,0:������������,1:������������
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
	int		nKindBuySell;//0������������1������������2��������������//used by  

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
	SUB_FIGUER_INIT_INFO	m_propertyInitiate2;	//��ʼ����Ϣ
	VIEWARRAY m_arKLineViews;
	CString	m_sShF10;
	CString	m_sSzF10;
	CString	m_sNews;
	CTaiTestTreeView *	m_taiViewF9;
    CTaiTestTreeView *	m_taiViewF10;
	CF10HeadView *		m_F10HeadView;
//  end

	CSpz m_spz;						//������Ƨ��ƴ��
	CDyz m_dyz;						//���������ƴ��
	static BOOL m_bAppAuthorized;	//��Ʒ�Ƿ���Ȩ
	CString m_strPName;			//��Ʒ����
	CString m_strUser;			//�û���
	CString m_strCompany;		//��˾��
	CString m_strSerial;		//ע���
	CString m_strCredit;		//��Ȩ��
	CTaiShanTesting* m_pPingCe;	//


	INFOINIT	m_infoInit;	//��ʼ����Ϣ
	CTypedPtrArray<CPtrArray,NAMESYMBOL*> m_namesymbol;//ϵͳ���ձ��ARRAY
	SETCURRENTDIRECTORY m_Directory; 
	CArray<CString,CString&>		index_keyb_f;	//������������
	CArray<CString,CString&>		index_keyb_a;	//���̹�Ʊƴ������

	COLORREF  m_colorArrayDefault[13][34]; //ϵͳ��ɫ����
	COLORREF  m_colorArray[34];  //�������������е���ɫ����
	LOGFONT   m_fontdefault;           //ϵͳȱʡ������
	LOGFONT   m_fontstr[FONTSTRUCTLEN]; //�������������е���������
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

	CImageList  *m_imagelist;          //ϵͳ��ͼ���б�
	CBitmap       m_bitmap;            //λͼ����

    CFormularArray         m_formuar_index;          //����ָ�깫ʽ��ARRAY
	CFormularArray         m_formuar_choose;         //����ѡ�ɹ�ʽ��ARRAY
	CFormularArray         m_formuar_master;       //ר��ϵͳ��ʽ��ARRAY
	CFormularArray         m_formuar_kline;          //K��ָ���ARRAY

	bool ChooseFileExist;              ////////if true the file exist else false//
	bool m_bAddKline;
	
	SYSTEM_OPTION  m_systemOption;        //ϵͳ�����е�ѡ����
	SYSTEMDATA    m_systemdata;          //ϵͳ�����е�������
	SYSTEMHQSET   m_syshqset;            //ϵͳ�����еļ�ʱ��������
    ALERTCHOICE   m_dadanchoice;          //ϵͳ�����е�ͳ�Ʒ�ʽ����
 
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
	long m_lDay;                                                //��¼��������ʱ�� 
	BOOL m_bInitDone;											//��¼�����ʼ����ɱ�־
	BOOL m_bInitCurrDone;										//��¼Ŀǰ��������ʼ����־
	BOOL m_bCloseWorkDone; 										//��¼����������ҵ���
	int m_nBlockCalcCounter;
	int m_nBlockCalcLastMin;

	long m_lStockTypeCount[13];                                 //���ڼ�¼��������֤ȯ��  
	CMapStringToString m_StockTypeMap[3];

    short m_nANT[3];                                             //Ŀǰ�Ϻ������ڿ��з����� 
	short m_nOldANT[3];                                            //��¼��ǰ�Ϻ������ڿ��з����� 
	short m_nMaxANT[3];                                            //�Ϻ������ڿ��з�����   
	short m_nMaxMaxANT;                                            //�Ϻ�����������з�����
	short m_nDel_Half_ANT[3];                                      //���翪�з�����
	short m_nDel_Start_A_hr[3];                                    //���翪��Сʱ�� 
	short m_nDel_Start_A_min[3];                                   //���翪�̷�����
	short m_nDel_Start_B_hr[3];                                    //���翪��Сʱ��
	short m_nDel_Start_B_min[3];                                   //���翪�̷�����
	short m_nDel_End_A_hr[3];                                      //��������Сʱ��
	short m_nDel_End_A_min[3];                                     //�������̷�����
	short m_nDel_End_B_hr[3];                                      //��������Сʱ�� 
	short m_nDel_End_B_min[3];                                     //�������̷�����
    short m_nHave_Olddata;                                         //��¼���������Ƿ��ѱ��� 
    int m_nNowHour;                                                //����Сʱ��   
    int m_nNowMin;                                                 //���з�����
    int m_nNowSec;                                                 //�������� 
	long m_lStartMinA[3];                                          //���翪��ʱ��
    long m_lEndMinA[3];                                            //��������ʱ��
	long m_lStartMinB[3];                                          //���翪��ʱ��
    long m_lEndMinB[3];                                            //��������ʱ��
    short m_nRN[3][240],m_nRN1[3][240],m_nDN[3][240],m_nRD[3][240],m_nDN1[3][240],m_nCalStk[3][240];              //���ڼ������ǿ��ָ���м����

	int m_nStockTypeShPage;
	int m_nStockTypeSzPage;
	int m_nStockTypeFlPage;

	int m_CalcBlockCounts;

	int m_nCurrentBlockPage;           //�������а��ҳ 
	int m_nCurrentReportPage;           //������������ҳ 
//    CStringList	m_ScreenList;       //����ѡ���б�                             //���ڱ�������ѡ�ɹ�Ʊ����
	CArray<SharesSymbol,SharesSymbol&> m_ScreenList;
	struct tm m_week;
	int m_nIsClose;
    int m_nColumnIndex[4][MAXCOLUME];	//���ڼ�¼��������



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

	STOCKDATASHOW *		m_pStatisticData;    //ƽ��ֵ�����ֵ����Сֵ�ͷ���
	bool				m_bStatistic[4];
	bool				m_bStatisticed;
	int					m_nWeightType;//0 = ����ƽ����1=��ͨ�ɱ���2= �ܹɱ�

	BLOCKINFO*			m_pBlockDataShow;
	int					m_nBlockShowMaxCount;
	int					m_nBlockShowCount;

	CMapNewsFileName	m_NewsFileNameMap;                           //������ص�������ļ���Ϣ
	KLINEHEAD			m_KLineHead;                              //���������ļ���ͷ�ṹ
	MKLINEHEAD			m_MinKLineHead;                           //����5�������ļ���ͷ�ṹ


//YHH
	CList<SharesSymbol*,SharesSymbol*> m_choosesymbol;//��ѡ�ɵĴ���LIST
    CList<BLOCKSTOCK*,BLOCKSTOCK*> m_blocklist;     //����LIST
    CList<BLOCKSTOCK*,BLOCKSTOCK*> m_Tjxgblocklist;     //����ѡ�ɵ�LIST
    CString          m_cybkname[10];

	Rsdn1 *Nidx[2];     //���ڴ�����̸����ǵ���
    Tidxd *Tidx[2];     //�ڴ������ǿ��ָ��
    Aidx1 *Adat[2];   	//���ڴ������ָ������
	int InitSetp;

	int m_nPreDisplayRow;   //���ڱ�����ʾ����


	CBlockAnalysisView*		m_pBlockAnalysisView;
	CTaiShanReportView*		m_pReportView;
	CSharesBigBillDlg*		m_pStockCjMxTj;
	CSharesBigBillData*		m_pStockBigBillData;
	CNineShowView*			m_pNineShowView;
	CTaiTestTreeView*		m_pTestTreeView;
    CInfoView*				m_pInfoView;
	CTaiShanKlineShowView*	m_pKlineShowView;
	///is a splitter window////////


	void Init_dat();                                            //��ʼ������ʱ�����������ݱ������   
    void Chk_Ystc();                                            //�������Ƿ������ݱ���
	void chk_date();                                            //�õ��������� 
	void Init_EveryDay();                                       //ÿ���ʼ���������� 
	void Init_StockData(int mode);                              //��ʼ����Ʊ�������� 
    int GetStocktime(int mode);                                 //�õ����м�ʱʱ��
	void LoadStockData(int mode);                               //�õ���Ʊ���Ʊ���������
    void CreateFileData(int mode);                              //�����������ݿ��ļ� 
	void LoadFileData(int mode);                                //װ�����ݿ��ļ����� 
	void GetStockCount();                                       //�õ��������й�Ʊ��
	void OnCalcHqDataProgress();                                //������������뵱ǰ������
    float OnCalcDpTidxdData(int which_stk);                     //�������ǿ��ָ��
	long GetStockDay(time_t time );								//��Ʊ�������         
	__int64 GetStockMinute(time_t time );						//��Ʊ�������             
    void StockCloseWork();
	void SaveFileData();
	void StockNameConvert(char *StockName,char *pyjc);
	void InitTjxg();           //��ʼ������ѡ��
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
	CArray<DATA_KEYBOARD*,DATA_KEYBOARD*> m_hotkey; //ϵͳ�Ļ������ֵ��ȼ�
	CArray<DATA_KEYBOARD*,DATA_KEYBOARD*> m_hotchar;//ϵͳ�Ļ����ַ����ȼ�
	CArray<DATA_KEYBOARD*,DATA_KEYBOARD*> m_keychardata; //�����ַ��ļ�������
	CArray<DATA_KEYBOARD*,DATA_KEYBOARD*> m_keynumberdata;//�������ֵļ�������
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
