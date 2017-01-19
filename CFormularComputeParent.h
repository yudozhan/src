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
#define PRAMANUM    8//最大参数个数

#define MAXB		2000
#define MAXB_ALL	2000

#define MIDB		10000   
#define MIDE		10099

#define RLINEB		11000    
#define RLINEE		11000+NUM_LINE_TOT

#define CEFB		12000    //浮点数开始
#define CEFE		12299	 //浮点数结束

#define CEIB		13000    //整数开始
#define CEIE		13299	//整数结束

#define CECB		13500    //字符串开始
#define CECE		13520	//字符串结束

#define FuncB		8000	//函数开始
#define FuncE		8899	//函数结束
#define FuncDllB	8400	//函数开始
#define FuncDllE	8600	//函数结束

#define PAM			9900  // 系统参数的最大值

#define PCMB		14000	//中间变量开始
#define PCME		14199	//中间变量结束

#define QUB			8800  //引用开始
#define QUE			8899  //引用终止

#define TOZERO		9009  //
#define CONSB		9010  //线型常数开始
#define CONSE		9498  //线型常数终止
#define CONSCOLORB1		9100  //color常数开始
#define CONSCOLORB		9200  //color常数开始
#define CONSCOLORE		9280  //color常数开始
#define CONSLINETHICKB  9300  //
#define CONSLINETHICKE  9320  //
#define CONSLINEDOT		9350  //
/*
typedef union tagKlineEx
{
	struct
	{
		float buyPrice[5];	//买1--买3价
		float buyVol[5];		//买1--买3量
		float sellPrice[5];	//卖1--卖3价	
		float sellVol[5];		//卖1--卖3量
	};
	float fDataEx[12];			//保留
} KlineEx;

enum klineKind {
	HS_KLINE = 25,//分笔成交
	MANY_DAY_KLINE	=	15,//多日线
	MIN1_KLINE	=	35,//1分钟线
	MIN5_KLINE = 1,//5分钟线	
	MIN15_KLINE = 2,//15分钟线
	MIN30_KLINE = 3,//30分钟线
	MIN60_KLINE = 4,//60分钟线
	DAY_KLINE = 5,//日线
	WEEK_KLINE = 6,//周线
	MONTH_KLINE = 7,//月线
	YEAR_KLINE = 8//年
};
typedef struct tagINPUT_INFO
{
	char*			  strSymbol;		//股票代码
	bool			  bIndex;				//大盘

	int			  nNumData;		//数据数量(pData,pDataEx,pResultBuf数据数量)
	Kline*		  pData;					//常规数据,注意:当m_nNumData==0时可能为 NULL
	KlineEx*	      pDataEx;				//扩展数据,分笔成交买卖盘,注意:可能为 NULL

	ARRAY_BE 		fInputParam[10]	;//调用参数
	ARRAY_BE 		fOutputParam;	//输出参数
	klineKind	  klineType;			//数据类型
	BASEINFO*    financeData;			//财务数据
	CReportData* pDat1;//行情数据
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
//===========压入头部=============
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
//==========压入尾部================
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
//========弹出===============
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
//========压出===============
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
	bool	m_bShowColor;//是否显示彩色
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
		int m_nKind;//元素种类
		int m_nPos;//元素结束位置
	};
	KindPos*	m_pKindPos;//
	int			m_nSizeKindPos;//m_pKindPos元素个数
	int			m_nPosPre;//前一个元素的位置

	KlineEx *	m_pKlineEx ;
	ARRAY_BE	m_Rline[RLINEE-RLINEB];  //返回变量
	CString		m_RlineName[RLINEE-RLINEB];  //返回变量名称
	int			m_RlineNum;              //返回变量个数

	CString		m_formuCompute;   //计算表达式
	CString		m_strErrMsg;   //错误信息
	int			m_nErrPos;	//错误位置

	bool		m_bTotalData;
	int			m_isDaPan;	//是否是大盘指标
	int			m_RlineType[NUM_LINE_TOT];	//返回的线的类型
	int			m_RlineColor[NUM_LINE_TOT];	//返回的线的类型
	int			m_RlineThick[NUM_LINE_TOT];	//返回的线的宽度

	CFormularComputeParent(int pmin,int pmax,Kline* pp,CString& str); 
	//输入说明：pmin 为K线数据的最小下标,pmax为K线数据的最大下标,pp为K线数据的指针,pIndex为技术指标数据指针,
	//			symbol为股票代码,pBuySellList为换手数据链表,nKindKline为K线数据时间周期,bAddBuySell为是否把买卖条件加到公式中。
	//输出说明：无。
	CFormularComputeParent(int pmin,int pmax,Kline* pp,CFormularContent* pIndex,CString symbol,int stkKind,CBuySellList* pBuySellList,int nKindKline,bool bAddBuySell = true);
	CFormularComputeParent();
	virtual ~CFormularComputeParent();

	static int GetStockKind(char * strLabel);
	static bool FanEach(CFormularContent * pJishu);
	virtual int FuncInDllExt(INPUT_INFO *pInput, int nFunc,int j);

	//得到返回数据的名称
	//输入参数:foot 表示指定第几组数据
	//输出参数:返回指定组数据的名称
	CString GetRlineName(int foot);

	void IsDaPan();	//判断是否是大盘指标
	int GetIndexPoint(CString sName,float &nPoint);
	int FindLineType();//计算返回线的类型
	int YhhCheck(int j, int k);
	int GetRlineNum();//得到指标公式的数据数组的个数

	//得到某个元函数的参数个数
	//输入参数:tj标识特定的元函数
	//输出参数:返回元函数的参数个数
	int GetNum(int tj);

	int AddPara(CString pa,int va);   //增加整型参数
	int AddPara(CString pa,float va); //增加浮点参数
	int AddLine(CString& LineName,ARRAY_BE& pline);//增加线参数
	//取得错误信息									  
	//输入说明：str是错误信息字符串,pos 是错误位置
	//输出说明：无。
	void GetMsg(CString& str,int& pos);

	//取得计算结果
	//输入参数:index 表示指定第几组数据
	//输出参数:pline是输出的数据,name是输出的数据名称,返回指定组数据的名称
	int GetLine(int& index,ARRAY_BE& pline,CString& name);
	int GetIndexPoint(int &index,float &nPoint);

	int GetLine(CString& LineName,ARRAY_BE& pline);//取得计算结果
	int FormularCompute();    //计算
	//对公式引用进行处理
	//输入参数:cl 表示特定的公式引用
	//输出参数:返回错误码,0为正确,1为错误
	int Relolve(int cl);

	void Devide();     //分解公式成可以顺序计算的关键码
	void GetData(int cl,ARRAY_BE& pp);
	int Sort(int bi,int ej);     //运算顺序确定

	//词法检查
	//输入参数:emp 是输入的公式字符串
	//输出参数:返回错误发生的位置
	int CheckWord(CString emp);

	int CheckSentence();	//语法检查	
	int GetCharClass(char ch);   //确定字符类别


protected:
	int m_nCurrentFoot;//for right box funcs
	int m_tempWhich;//当前计算到哪个线

	int m_MaxPoint;    // 计算起始点
	int m_MinPoint;    // 计算终止点

	CString		m_QuoteArray[QUE-QUB];	//引用的名称的数组
	int			m_QuoteNum;	//引用的个数//

	CString		m_stringArray[CECE-CECB];	//字符串的数组
	int			m_stringNum;	//字符串的个数

	CString		m_strColor[CONSCOLORE-CONSCOLORB+1];
	int			m_strColorNum;	//color的个数

	int	 m_Sent[MAXB];  //子公式计算顺序//
	int  m_SentNum;     //子公式中实体个数(-1)//

	int	 m_ComSort[MAXB_ALL];    // 公式计算顺序//-------lmb?
	int  m_ComSortNum;		 // 公式中实体个数(-1)//
	int  m_Position[MAXB];  // 记录变量在子串中位置//

	ARRAY_BE	m_VarArray[MIDE-MIDB]; // 中间变量
	int			m_VarNum;			  // 中间变量个数
	CString		m_VarName[MIDE-MIDB];             // 中间变量名称

	int			m_floatNum;			  // 浮点系数个数
	float		m_floatArray[CEFE-CEFB];//浮点系数

	CString		m_NumName[PRAMANUM];    //参数名称
	int			m_NumGs;		  //参数个数	

	int			m_intNum;		// 整数系数个数
	int			m_intArray[CEIE-CEIB]; // 整数系数

	ARRAY_BE	m_MidArray[PCME-PCMB]; // 中间数据
	int			m_MidNum;    // 中间数据个数

	Kline*		m_pData;       // K线数据指针
	Intst<int>	m_table;	//堆栈

	CMapStringToPtr					m_WordTableNew;	//新加的函数表
	CMap<int,int,int,int>			m_ParameterTableNew;//新加的函数参数个数表

	static CMapStringToPtr			m_LetterTable;	//字母表
	static CMap<int,int,int,int>	m_ParameterTable;//函数参数个数表
	static CMapStringToPtr			m_WordTable;	//函数表
	
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
	int FuncInDll(INPUT_INFO* pInput,int  nFunc);//计算2.0版新增加的元函数
	//输入参数:pInput 是输入的原始数据,nFunc是表示第几个函数
	//输出参数:返回错误码,0表示成功

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
	void InitEquation(int pmin,int pmax,Kline* pp,CString& str);//初始化输入数据
	int GetQuote(CString &str, CString &zb, CString &cline, int &tu,int &equationKind,int &nPeriods);//equationKind is the kind of CFormularContent(0 is zhibiao ,1 is xuangu,2 is klinemode,3 is stock system,10 is other stock's kline price, 11 is the string including a periods(such as #week))
	void FanEach(ARRAY_BE& lineIn,int nFoot,ARRAY_BE& lineOut);
};

#endif // !defined(AFX_COM_H__80BFF943_AB2A_11D1_8C43_0000E823CF90__INCLUDED_)
