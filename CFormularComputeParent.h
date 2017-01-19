// com.h: interface for the CFormularComputeParent class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_COM_H__80BFF943_AB2A_11D1_8C43_0000E823CF90__INCLUDED_)
#define AFX_COM_H__80BFF943_AB2A_11D1_8C43_0000E823CF90__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include <math.h>

#include "StructKlineView.h"
#include "StructTaiShares.h"

#define NUM_LINE_TOT	64
#define PRAMANUM    8//����������

#define MAXB		2000
#define MAXB_ALL	2000

#define MIDB		10000   
#define MIDE		10099

#define RLINEB		11000    
#define RLINEE		11000+NUM_LINE_TOT

#define CEFB		12000    //��������ʼ
#define CEFE		12299	 //����������

#define CEIB		13000    //������ʼ
#define CEIE		13299	//��������

#define CECB		13500    //�ַ�����ʼ
#define CECE		13520	//�ַ�������

#define FuncB		8000	//������ʼ
#define FuncE		8899	//��������
#define FuncDllB	8400	//������ʼ
#define FuncDllE	8600	//��������

#define PAM			9900  // ϵͳ���������ֵ

#define PCMB		14000	//�м������ʼ
#define PCME		14199	//�м��������

#define QUB			8800  //���ÿ�ʼ
#define QUE			8899  //������ֹ

#define TOZERO		9009  //
#define CONSB		9010  //���ͳ�����ʼ
#define CONSE		9498  //���ͳ�����ֹ
#define CONSCOLORB1		9100  //color������ʼ
#define CONSCOLORB		9200  //color������ʼ
#define CONSCOLORE		9280  //color������ʼ
#define CONSLINETHICKB  9300  //
#define CONSLINETHICKE  9320  //
#define CONSLINEDOT		9350  //
/*
typedef union tagKlineEx
{
	struct
	{
		float buyPrice[5];	//��1--��3��
		float buyVol[5];		//��1--��3��
		float sellPrice[5];	//��1--��3��	
		float sellVol[5];		//��1--��3��
	};
	float fDataEx[12];			//����
} KlineEx;

enum klineKind {
	HS_KLINE = 25,//�ֱʳɽ�
	MANY_DAY_KLINE	=	15,//������
	MIN1_KLINE	=	35,//1������
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

	ARRAY_BE 		fInputParam[10]	;//���ò���
	ARRAY_BE 		fOutputParam;	//�������
	klineKind	  klineType;			//��������
	BASEINFO*    financeData;			//��������
	CReportData* pDat1;//��������
	int nReserved;
} INPUT_INFO;
*/

template <class TY> class Intem
{
public:
	Intem(TY v=0){val=v;next=0;prev=0;}
	Intem<TY>* next;
	Intem<TY>* prev;
	TY val;
};

template <class TH> class Intst 
{
public:
	Intst(TH val) { list = new Intem(val);tail=0;};
	Intst() {list = 0;tail=0;};
//===========ѹ��ͷ��=============
	TH push_b(TH val)
	{
		Intem<TH>* pt;
		pt=new Intem<TH>(val);
		pt->next = list;
		if(list!=0) 
			list->prev = pt;
		else
			tail = pt;
		list = pt;
		return val;
	}
//==========ѹ��β��================
/*	TH push_t(TH val)
	{
		Intem<TH> *pi,*pt,*prv;
		for(prv=pi=list;pi;prv=pi,pi=pi->next)  ;
    
		pt=new Intem<TH>(val);	
		if(list==0)
			list=pt;
		else
		{
			pt->prev=prv;
			prv->next=pt;
		}
		return val;
	}
*/
//========����===============
	TH pop_it()
	{
		Intem<TH>* tmp;
		TH val;
		if(list==0)
			return 0;
		tmp=list;
		list=list->next;
		if(list!=0)
			list->prev=0;
		else
			tail=0;
		val = tmp->val;
		delete tmp;
		return val;
	}
//========ѹ��===============
	TH rec_it()
	{
		Intem<TH>* tmp;
		TH val;
		if(tail==0)
			return 0;
		tmp=tail;
		tail=tail->prev;
		if(tail!=0) 
			tail->next=0;
		else
			list=0;
		val = tmp->val;
		delete tmp;
		return val;
	}
private:
	Intem<TH>* list;
	Intem<TH>* tail;
};


class CTaiShanDoc ;
class CFormularContent;
struct INPUT_PARAM
{
	int					pmax;
	Kline*				pKLine;
	CFormularContent*	pIndex;
	CString				symbol;
	CBuySellList*		pBuySellList;
	int					nKindKline;
	bool				bAddBuySell;
	int					m_stkKind;
//	INPUT_PARAM(){bAddBuySell = true;pmin=0;pBuySellList=NULL;nKindKline=5;symbol="1A0001"};
};

class CFormularComputeParent  
{
public:
	int		m_stkKind;
	bool	m_bRightbox;
	bool	m_bShowColor;//�Ƿ���ʾ��ɫ
	enum ColorKind 
	{
		digit = 0,
		operater ,
		variable ,
		function,
		quotevar,
	};
	struct KindPos
	{
		int m_nKind;//Ԫ������
		int m_nPos;//Ԫ�ؽ���λ��
	};
	KindPos*	m_pKindPos;//
	int			m_nSizeKindPos;//m_pKindPosԪ�ظ���
	int			m_nPosPre;//ǰһ��Ԫ�ص�λ��

	KlineEx *	m_pKlineEx ;
	ARRAY_BE	m_Rline[RLINEE-RLINEB];  //���ر���
	CString		m_RlineName[RLINEE-RLINEB];  //���ر�������
	int			m_RlineNum;              //���ر�������

	CString		m_formuCompute;   //������ʽ
	CString		m_strErrMsg;   //������Ϣ
	int			m_nErrPos;	//����λ��

	bool		m_bTotalData;
	int			m_isDaPan;	//�Ƿ��Ǵ���ָ��
	int			m_RlineType[NUM_LINE_TOT];	//���ص��ߵ�����
	int			m_RlineColor[NUM_LINE_TOT];	//���ص��ߵ�����
	int			m_RlineThick[NUM_LINE_TOT];	//���ص��ߵĿ��

	CFormularComputeParent(int pmin,int pmax,Kline* pp,CString& str); 
	//����˵����pmin ΪK�����ݵ���С�±�,pmaxΪK�����ݵ�����±�,ppΪK�����ݵ�ָ��,pIndexΪ����ָ������ָ��,
	//			symbolΪ��Ʊ����,pBuySellListΪ������������,nKindKlineΪK������ʱ������,bAddBuySellΪ�Ƿ�����������ӵ���ʽ�С�
	//���˵�����ޡ�
	CFormularComputeParent(int pmin,int pmax,Kline* pp,CFormularContent* pIndex,CString symbol,int stkKind,CBuySellList* pBuySellList,int nKindKline,bool bAddBuySell = true);
	CFormularComputeParent();
	virtual ~CFormularComputeParent();

	static int GetStockKind(char * strLabel);
	static bool FanEach(CFormularContent * pJishu);
	virtual int FuncInDllExt(INPUT_INFO *pInput, int nFunc,int j);

	//�õ��������ݵ�����
	//�������:foot ��ʾָ���ڼ�������
	//�������:����ָ�������ݵ�����
	CString GetRlineName(int foot);

	void IsDaPan();	//�ж��Ƿ��Ǵ���ָ��
	int GetIndexPoint(CString sName,float &nPoint);
	int FindLineType();//���㷵���ߵ�����
	int YhhCheck(int j, int k);
	int GetRlineNum();//�õ�ָ�깫ʽ����������ĸ���

	//�õ�ĳ��Ԫ�����Ĳ�������
	//�������:tj��ʶ�ض���Ԫ����
	//�������:����Ԫ�����Ĳ�������
	int GetNum(int tj);

	int AddPara(CString pa,int va);   //�������Ͳ���
	int AddPara(CString pa,float va); //���Ӹ������
	int AddLine(CString& LineName,ARRAY_BE& pline);//�����߲���
	//ȡ�ô�����Ϣ									  
	//����˵����str�Ǵ�����Ϣ�ַ���,pos �Ǵ���λ��
	//���˵�����ޡ�
	void GetMsg(CString& str,int& pos);

	//ȡ�ü�����
	//�������:index ��ʾָ���ڼ�������
	//�������:pline�����������,name���������������,����ָ�������ݵ�����
	int GetLine(int& index,ARRAY_BE& pline,CString& name);
	int GetIndexPoint(int &index,float &nPoint);

	int GetLine(CString& LineName,ARRAY_BE& pline);//ȡ�ü�����
	int FormularCompute();    //����
	//�Թ�ʽ���ý��д���
	//�������:cl ��ʾ�ض��Ĺ�ʽ����
	//�������:���ش�����,0Ϊ��ȷ,1Ϊ����
	int Relolve(int cl);

	void Devide();     //�ֽ⹫ʽ�ɿ���˳�����Ĺؼ���
	void GetData(int cl,ARRAY_BE& pp);
	int Sort(int bi,int ej);     //����˳��ȷ��

	//�ʷ����
	//�������:emp ������Ĺ�ʽ�ַ���
	//�������:���ش�������λ��
	int CheckWord(CString emp);

	int CheckSentence();	//�﷨���	
	int GetCharClass(char ch);   //ȷ���ַ����


protected:
	int m_nCurrentFoot;//for right box funcs
	int m_tempWhich;//��ǰ���㵽�ĸ���

	int m_MaxPoint;    // ������ʼ��
	int m_MinPoint;    // ������ֹ��

	CString		m_QuoteArray[QUE-QUB];	//���õ����Ƶ�����
	int			m_QuoteNum;	//���õĸ���//

	CString		m_stringArray[CECE-CECB];	//�ַ���������
	int			m_stringNum;	//�ַ����ĸ���

	CString		m_strColor[CONSCOLORE-CONSCOLORB+1];
	int			m_strColorNum;	//color�ĸ���

	int	 m_Sent[MAXB];  //�ӹ�ʽ����˳��//
	int  m_SentNum;     //�ӹ�ʽ��ʵ�����(-1)//

	int	 m_ComSort[MAXB_ALL];    // ��ʽ����˳��//-------lmb?
	int  m_ComSortNum;		 // ��ʽ��ʵ�����(-1)//
	int  m_Position[MAXB];  // ��¼�������Ӵ���λ��//

	ARRAY_BE	m_VarArray[MIDE-MIDB]; // �м����
	int			m_VarNum;			  // �м��������
	CString		m_VarName[MIDE-MIDB];             // �м��������

	int			m_floatNum;			  // ����ϵ������
	float		m_floatArray[CEFE-CEFB];//����ϵ��

	CString		m_NumName[PRAMANUM];    //��������
	int			m_NumGs;		  //��������	

	int			m_intNum;		// ����ϵ������
	int			m_intArray[CEIE-CEIB]; // ����ϵ��

	ARRAY_BE	m_MidArray[PCME-PCMB]; // �м�����
	int			m_MidNum;    // �м����ݸ���

	Kline*		m_pData;       // K������ָ��
	Intst<int>	m_table;	//��ջ

	CMapStringToPtr					m_WordTableNew;	//�¼ӵĺ�����
	CMap<int,int,int,int>			m_ParameterTableNew;//�¼ӵĺ�������������

	static CMapStringToPtr			m_LetterTable;	//��ĸ��
	static CMap<int,int,int,int>	m_ParameterTable;//��������������
	static CMapStringToPtr			m_WordTable;	//������
	
	Kline*		m_pKlineIndex;

	int			m_nExec;
	int			m_nKindKline;//m_nKindKline 
	bool		m_bCreateBsList;
	CString		m_symbol;

	CBuySellList *		m_pBuySellList; 
	CTaiShanDoc *		m_pDoc;
	CFormularContent*	m_pIndex;
	CReportData*		m_pReportData;
	
	static int FuncZigGroup(INPUT_INFO *pInput, void* pArray);
	static int GetFoot(INPUT_INFO *pInput, int nParam);
	static void InitStatic();

	void FuncZigPre(INPUT_INFO *pInput,int nFunc);
	int CalcMin5InDay(CFormularContent*	pIndex,int nParamInTable,CString linename,int tu);
	int GetBeginFoot(ARRAY_BE& ln2);
	int RelolveNnet(int cl);
	int FuncInDll(INPUT_INFO* pInput,int  nFunc);//����2.0�������ӵ�Ԫ����
	//�������:pInput �������ԭʼ����,nFunc�Ǳ�ʾ�ڼ�������
	//�������:���ش�����,0��ʾ�ɹ�

	int FuncFromDll(int n);
	bool LookupParameterTable(int nKey,int& nValue);
	bool LookupWordTable(CString sKey,int& n);
	void GetHsKlineData(int cl,ARRAY_BE& pp);
	void TransferRealInfo(int bl,int be,int nKind);
	void KlineToLine(Kline* pKline,float& f,int nFlag);


    int Func1001();
	int Func1002();
	int Func3001();
	int Func2001();
	int Func4001();
	int Func4002();
	int Func4005();
	int Func4006();
	int Func4008();
	int Func4009();
	int Func5001();
	int Func5002();
	int Func6001();
	int Func6002();
	int Func6003();
	int Func6004();
	int Func8001();
	int Func8002();
	int Func8003();
	int Func8004();
	int Func8005();
	int Func8006();
	int Func8007();
	int Func8008();
	int Func8009();
	int Func8010();
	int Func8011();
	int Func8012();
	int Func8013();
	int Func8030();
	int Func8031();
	int Func832();
	int Func8033();
	int Func8034();
	int Func8035();
	int Func8036();
	int Func8037();
	int Func8038();
	int Func8039();
	int Func8040();
	int Func8041();
	int Func8042();
	int Func8043();
	int Func8044();
	int Func8045();
	int Func8046();
	int Func8047();
	int Func8053();
	int Func8052();
	int Func8048to8051(int nKind);

	void GetLineNameArray(CString fml,CStringArray& sArray);
	int GetPeriodKind(CString sIn);
	int ReadKline(CString s,int stkKind,Kline*& pKline);
	bool GetBuySellList();
	Kline* GetKlineIndex();
	void InitEquation(int pmin,int pmax,Kline* pp,CString& str);//��ʼ����������
	int GetQuote(CString &str, CString &zb, CString &cline, int &tu,int &equationKind,int &nPeriods);//equationKind is the kind of CFormularContent(0 is zhibiao ,1 is xuangu,2 is klinemode,3 is stock system,10 is other stock's kline price, 11 is the string including a periods(such as #week))
	void FanEach(ARRAY_BE& lineIn,int nFoot,ARRAY_BE& lineOut);
};

#endif // !defined(AFX_COM_H__80BFF943_AB2A_11D1_8C43_0000E823CF90__INCLUDED_)
