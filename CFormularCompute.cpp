// CFormularCompute.cpp: implementation of the CFormularCompute class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CFormularCompute.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFormularCompute::CFormularCompute()
{

}

CFormularCompute::~CFormularCompute()
{

}
CFormularCompute::CFormularCompute(int pmin,int pmax,Kline* pp,CString& str):CFormularComputeParent(pmin, pmax, pp, str)
{
	InitStatic2();
}
CFormularCompute::CFormularCompute(int pmin,int pmax,Kline* pp,CFormularContent* pIndex,CString symbol,int stkKind,CBuySellList* pBuySellList,int nKindKline,bool bAddBuySell ):CFormularComputeParent(pmin, pmax, pp, pIndex, symbol, stkKind, pBuySellList, nKindKline, bAddBuySell)
{
	InitStatic2();
}
/*
void CFormularComputeParent::InitStatic()
{
	static bool bExec = false;
	if(bExec==true) return;
	//=================运算符===============
	m_WordTable.InitHashTable( 300 );
	m_LetterTable.InitHashTable( 150 );
	m_ParameterTable.InitHashTable( 100 );

	m_WordTable[_T("*")]  =(void *)6001;
	m_WordTable[_T("/")]  =(void *)6002;
	m_WordTable[_T("*-")]  =(void *)6003;
	m_WordTable[_T("/-")]  =(void *)6004;
	m_WordTable[_T("+")]  =(void *)5001;
	m_WordTable[_T("-")]  =(void *)5002;
	m_WordTable[_T(">")]  =(void *)4001;
	m_WordTable[_T("<")]  =(void *)4002;
	m_WordTable[_T(">=")] =(void *)4005;
	m_WordTable[_T("<=")] =(void *)4006;
	m_WordTable[_T("=")] =(void *)4008;
	m_WordTable[_T("and")] =(void *)3001;
	m_WordTable[_T("or")] =(void *)2001;
	m_WordTable[_T(")")]  =(void *)9002;
	m_WordTable[_T("(")]  =(void *)9001;
	m_WordTable[_T(",")]  =(void *)9005;

	m_WordTable[_T(":")]  =(void *)1001;
	m_WordTable[_T(":=")] =(void *)1002;

	m_WordTable[_T("!=")] =(void *)4009;
	m_WordTable[_T("><")] =(void *)4009;
	m_WordTable[_T("<>")] =(void *)4009;
	//---		常数
	m_WordTable[_T("volstick")]    =(void *)9010;
	m_WordTable[_T("stick")]	   =(void *)9011;
	m_WordTable[_T("circledot")]   =(void *)9012;
	m_WordTable[_T("linestick")]   =(void *)9013;
	m_WordTable[_T("crossdot")]    =(void *)9014;
	m_WordTable[_T("colorstick")]  =(void *)9015;
	m_WordTable[_T("buycase")]  =(void *)9016;
	m_WordTable[_T("sellcase")]  =(void *)9017;
	m_WordTable[_T("pointdot")]  =(void *)9012;

	m_WordTable[_T("colorred")]  =(void *)9100;
	m_WordTable[_T("colorgreen")]  =(void *)9101;
	m_WordTable[_T("colorblue")]  =(void *)9102;
	m_WordTable[_T("coloryellow")]  =(void *)9103;
	m_WordTable[_T("colororange")]  =(void *)9104;
	m_WordTable[_T("colorpurple")]  =(void *)9105;
	m_WordTable[_T("colorblack")]  =(void *)9106;
	m_WordTable[_T("colorwhite")]  =(void *)9107;
	m_WordTable[_T("colormagenta")]  =(void *)9108;//pink
	m_WordTable[_T("colorcyan")]  =(void *)9109;

	m_WordTable[_T("linethick1")]  =(void *)9300;
	m_WordTable[_T("linethick2")]  =(void *)9301;
	m_WordTable[_T("linethick3")]  =(void *)9302;
	m_WordTable[_T("linethick4")]  =(void *)9303;
	m_WordTable[_T("linethick5")]  =(void *)9304;
	m_WordTable[_T("linethick6")]  =(void *)9305;
	m_WordTable[_T("linethick7")]  =(void *)9306;

	m_WordTable[_T("linedot")]  =(void *)CONSLINEDOT;

//	m_WordTable[_T("color")]  =(void *)902;
	//=================参数================
	m_WordTable[_T("open")]    =(void *)9521;
	m_WordTable[_T("close")]   =(void *)9522;
	m_WordTable[_T("high")]    =(void *)9523;
	m_WordTable[_T("low")]     =(void *)9524;
	m_WordTable[_T("o")]    =(void *)9521;
	m_WordTable[_T("c")]   =(void *)9522;
	m_WordTable[_T("h")]    =(void *)9523;
	m_WordTable[_T("l")]     =(void *)9524;
	m_WordTable[_T("advance")] =(void *)9525;
	m_WordTable[_T("weekday")] =(void *)9526;
	m_WordTable[_T("day")]     =(void *)9527;
	m_WordTable[_T("month")]   =(void *)9528;
	m_WordTable[_T("year")]    =(void *)9529;
	m_WordTable[_T("hour")]    =(void *)9530;
	m_WordTable[_T("minute")]  =(void *)9531;
	m_WordTable[_T("amount")]  =(void *)9532;
	m_WordTable[_T("vol")]     =(void *)9533;
	m_WordTable[_T("volume")]     =(void *)9533;
//	m_WordTable[_T("a")]  =(void *)9532;
	m_WordTable[_T("v")]     =(void *)9533;
	m_WordTable[_T("isup")]    =(void *)9534;
	m_WordTable[_T("isdown")]  =(void *)9535;
	m_WordTable[_T("isequal")] =(void *)9536;
	m_WordTable[_T("decline")] =(void *)9537;
	m_WordTable[_T("capital")] =(void *)9538;
//	m_WordTable[_T("ltp")]	   =(void *)9539;
	//new 参数 in vs2.0
	m_WordTable[_T("indexo")]    =(void *)9541;//大盘K线数据
	m_WordTable[_T("indexc")]   =(void *)9542;//大盘K线数据
	m_WordTable[_T("indexh")]    =(void *)9543;//大盘K线数据
	m_WordTable[_T("indexl")]     =(void *)9544;//大盘K线数据
	m_WordTable[_T("indexv")]  =(void *)9545;//大盘K线数据
	m_WordTable[_T("indexa")] =(void *)9546;//大盘K线数据
	m_WordTable[_T("indexdec")] =(void *)9547;//大盘K线数据
	m_WordTable[_T("indexadv")] =(void *)9548;//大盘K线数据

	m_WordTable[_T("buyvol")] =(void *)9550;//换手数据的主动性买量
	m_WordTable[_T("sellvol")]  =(void *)9551;//换手数据的主动性卖量
	m_WordTable[_T("isbuyorder")]  =(void *)9552;//是否是买单
	m_WordTable[_T("activebuyvol")]  =(void *)9553;//主动性买量
	m_WordTable[_T("activesellvol")]  =(void *)9554;//主动性卖量
	m_WordTable[_T("time")]  =(void *)9560;//是否
	m_WordTable[_T("date")]  =(void *)9561;//是否

	m_WordTable[_T("volunit")]  =(void *)9580;//每手股数

	//================函数==================
	m_WordTable[_T("max")]     =(void *)8001;
	m_ParameterTable[8001]         =2;
	m_WordTable[_T("min")]     =(void *)8004;
	m_ParameterTable[8004]         =2;
	m_WordTable[_T("mod")]     =(void *)8005;
	m_ParameterTable[8005]         =2;
	m_WordTable[_T("reverse")] =(void *)8006;
	m_ParameterTable[8006]         =1;
	m_WordTable[_T("if")]      =(void *)8007;
	m_ParameterTable[8007]         =3;

	m_WordTable[_T("abs")]     =(void *)8002;
	m_ParameterTable[8002]         =1;
	m_WordTable[_T("ln")]      =(void *)8008;
	m_ParameterTable[8008]         =1;
	m_WordTable[_T("not")]     =(void *)8009;
	m_ParameterTable[8009]         =1;
	m_WordTable[_T("sgn")]     =(void *)8010;
	m_ParameterTable[8010]         =1;

	m_WordTable[_T("exp")]     =(void *)8003;
	m_ParameterTable[8003]         =1;
	m_WordTable[_T("log")]     =(void *)8011;
	m_ParameterTable[8011]         =1;
	m_WordTable[_T("pow")]     =(void *)8012;
	m_ParameterTable[8012]         =2;
	m_WordTable[_T("sqrt")]     =(void *)8013;
	m_ParameterTable[8013]         =1;


	m_WordTable[_T("avedev")]     =(void *)8030;
	m_ParameterTable[8030]         =2;
	m_WordTable[_T("count")]     =(void *)8031;
	m_ParameterTable[8031]         =2;
	m_WordTable[_T("cross")]     =(void *)8032;
	m_ParameterTable[8032]         =2;

	m_WordTable[_T("devsq")]     =(void *)8033;
	m_ParameterTable[8033]         =2;
	m_WordTable[_T("ema")]     =(void *)8034;
	m_ParameterTable[8034]         =2;
	m_WordTable[_T("hhv")]     =(void *)8035;
	m_ParameterTable[8035]         =2;

	m_WordTable[_T("llv")]     =(void *)8036;
	m_ParameterTable[8036]         =2;
	m_WordTable[_T("sar")]     =(void *)8037;
	m_ParameterTable[8037]         =3;
	m_WordTable[_T("sarturn")]     =(void *)8038;
	m_ParameterTable[8038]         =3;


	m_WordTable[_T("ref")]     =(void *)8039;
	m_ParameterTable[8039]         =2;
	m_WordTable[_T("ma")]     =(void *)8040;
	m_ParameterTable[8040]         =2;
	m_WordTable[_T("std")]     =(void *)8041;
	m_ParameterTable[8041]         =2;

	m_WordTable[_T("stdp")]     =(void *)8042;
	m_ParameterTable[8042]         =2;
	m_WordTable[_T("sum")]     =(void *)8043;
	m_ParameterTable[8043]         =2;
	m_WordTable[_T("varp")]     =(void *)8044;
	m_ParameterTable[8044]         =2;


	m_WordTable[_T("var")]     =(void *)8045;
	m_ParameterTable[8045]         =2;
	m_WordTable[_T("sma")]     =(void *)8046;
	m_ParameterTable[8046]         =3;

	//new function in vs2.0
	m_WordTable[_T("sumbars")]     =(void *)8047;//
	m_ParameterTable[8047]         =2;

	m_WordTable[_T("bidprice")]     =(void *)8048;//委买价
	m_ParameterTable[8048]         =1;
	m_WordTable[_T("bidvol")]    =(void *)8049;//委买量
	m_ParameterTable[8049]         =1;
	m_WordTable[_T("askprice")]  =(void *)8050;//委卖价
	m_ParameterTable[8050]         =1;
	m_WordTable[_T("askvol")] =(void *)8051;//委卖价
	m_ParameterTable[8051]         =1;

	m_WordTable[_T("finance")] =(void *)8052;//财务数据
	m_ParameterTable[8052]         =1;

	m_WordTable[_T("dynainfo")] =(void *)8053;//实时行情数据
	m_ParameterTable[8053]         =1;
	int nParam[]={  5,5,5,2,1,
					1,1,2,2,2,
					2,3,3,3,3,
					2,2,2,2,3,
					3,1,1,3,3,
					1,1,1,1,1,
					1,2,1,1,3,
					2,2,3,3,2,
					3,3,3,3,1,
					1,3,5,3,1,
					4,2,4,2,5,
					3,3,3,3,1,
					2,1,3,3
					};
	CString sName[]={"beeline","radial","wirebar","backset","barscount",//HHVBARS
					"barslast","barssince","dma","hhvbars","llvbars",
					"zig2","peak2","peakbars2","trough2","troughbars2",
					"forcast","slope","upward","downward","slowupcross",
					"slowdowncross","floatpercent","floattype","valueday","selfadd",
					"sin","cos","tan","asin","acos",
					"atan","backset2","towervalue","intpart","floatcommerce",
					"reflast","refbegin","valueaverage","floatcommercetype","zig",
					"peak","peakbars","trough","troughbars","winner",
					"cost",	"drawicon","drawline","drawtext","extdata",
					"deployref","deploysum","deploywin","polyline","stickline",
					"SetLineWidth","polylinevalue","polylinetime","between","ceiling",
					"filter","floor","longcross","range"
					};
	for(int j=0;j<44+20;j++)
	{
		m_WordTable[sName[j]] =(void *)(FuncDllB+j);//直线
		m_ParameterTable[FuncDllB+j]         =nParam[j];
	}
	m_WordTable["winnertype"] =(void *)(FuncDllB+14);//直线


	//=================数字================
	m_LetterTable[_T("0")]  =(void *)3;
	m_LetterTable[_T("1")]  =(void *)3;
	m_LetterTable[_T("2")]  =(void *)3;
	m_LetterTable[_T("3")]  =(void *)3;
	m_LetterTable[_T("4")]  =(void *)3;
	m_LetterTable[_T("5")]  =(void *)3;
	m_LetterTable[_T("6")]  =(void *)3;
	m_LetterTable[_T("7")]  =(void *)3;
	m_LetterTable[_T("8")]  =(void *)3;
	m_LetterTable[_T("9")]  =(void *)3;
	m_LetterTable[_T(".")]  =(void *)3;


	m_LetterTable[_T("a")]  =(void *)4;
	m_LetterTable[_T("b")]  =(void *)4;
	m_LetterTable[_T("c")]  =(void *)4;
	m_LetterTable[_T("d")]  =(void *)4;
	m_LetterTable[_T("e")]  =(void *)4;
	m_LetterTable[_T("f")]  =(void *)4;
	m_LetterTable[_T("g")]  =(void *)4;
	m_LetterTable[_T("h")]  =(void *)4;
	m_LetterTable[_T("i")]  =(void *)4;
	m_LetterTable[_T("j")]  =(void *)4;
	m_LetterTable[_T("k")]  =(void *)4;
	m_LetterTable[_T("l")]  =(void *)4;
	m_LetterTable[_T("m")]  =(void *)4;
	m_LetterTable[_T("n")]  =(void *)4;
	m_LetterTable[_T("o")]  =(void *)4;
	m_LetterTable[_T("p")]  =(void *)4;
	m_LetterTable[_T("q")]  =(void *)4;
	m_LetterTable[_T("r")]  =(void *)4;
	m_LetterTable[_T("s")]  =(void *)4;
	m_LetterTable[_T("t")]  =(void *)4;
	m_LetterTable[_T("u")]  =(void *)4;
	m_LetterTable[_T("v")]  =(void *)4;
	m_LetterTable[_T("w")]  =(void *)4;
	m_LetterTable[_T("x")]  =(void *)4;
	m_LetterTable[_T("y")]  =(void *)4;
	m_LetterTable[_T("z")]  =(void *)4;
	m_LetterTable[_T("_")]  =(void *)4;
	m_LetterTable[_T("#")]  =(void *)4;
	m_LetterTable[_T("$")]  =(void *)4;
	m_LetterTable[_T("@")]  =(void *)4;

	m_LetterTable[_T("+")]  =(void *)5;
	m_LetterTable[_T("-")]  =(void *)5;
	m_LetterTable[_T("*")]  =(void *)5;
	m_LetterTable[_T("/")]  =(void *)5;
	m_LetterTable[_T(">")]  =(void *)5;
	m_LetterTable[_T(",")]  =(void *)5;
	m_LetterTable[_T("<")]  =(void *)5;
	m_LetterTable[_T("=")]  =(void *)5;
	m_LetterTable[_T(")")]  =(void *)5;
	m_LetterTable[_T("(")]  =(void *)5;
	m_LetterTable[_T("!")]  =(void *)5;
	m_LetterTable[_T(":")]  =(void *)5;

	m_LetterTable[_T(" ")]  =(void *)2;
	m_LetterTable[_T("\n")] =(void *)2;
	m_LetterTable[_T("\t")] =(void *)2;
	m_LetterTable[_T("\r")] =(void *)2;
	m_LetterTable[_T("{")] =(void *)1;
	m_LetterTable[_T("}")] =(void *)1;

	m_LetterTable[_T("\"")]  =(void *)6;//"标点符号
	m_LetterTable[_T("'")]  =(void *)7;//'标点符号

	bExec=true;
}

*/
#define BEGIN_NUMBER1	64
void CFormularCompute::InitStatic2()
{//扩展CFormularComputeParent::InitStatic()
	static bool bExec = false;
	if(bExec==true) return;
	int nParam[]={ 1};
	CString sName[]={"sin2"};
	int nb = FuncDllB+BEGIN_NUMBER1;
	int nTemp;
	for(int j=0;j<sizeof(nParam)/sizeof(int);j++)
	{
		if(m_WordTable.Lookup(sName[j],(void*&)nTemp))	
		{
			m_WordTable[sName[j]] = (void *)(nb +j );//
			m_ParameterTable[nb + j] =nParam[j];
		}
	}
	
	
}
int ComputeFormu2(CFormularCompute*& pEqution,INPUT_PARAM& input)// need to delete pEqution outside
{
	ASSERT(pEqution == NULL);
	pEqution = new CFormularCompute(0,input. pmax,input.pKLine,input.pIndex,input. symbol,input.m_stkKind,input. pBuySellList,input. nKindKline,input. bAddBuySell);
	//--		加入参数
	int numPa=input. pIndex->numPara ;
	if(numPa>8)
		numPa = 8;
	if(numPa<0)
		numPa = 0;
	for(int j=0;j<numPa;j++)//CFormularContent::GetParamDataEach(int iIndex, int nKlineType, CFormularContent *pJishu)
	{
		input. pIndex->namePara[j].MakeLower();
		pEqution->AddPara (input. pIndex->namePara[j],
			CFormularContent::GetParamDataEach(j,input. nKindKline, input. pIndex));
	}
	
	return pEqution->FormularCompute();
}

int CFormularCompute::FuncInDllExt(INPUT_INFO *pInput, int nFunc, int j)
{
	pInput->fOutputParam.kind = ARRAY_BE::Normal;

	if(nFunc<BEGIN_NUMBER1) return 0;
/*
case 25://sin
j=pInput->fInputParam[0].b;
for(i=j;i<pInput->nNumData ;i++)
pInput->fOutputParam.line[i] = sin(pInput->fInputParam[0].line[i]);
pInput->fOutputParam.b=j;
break;
	*/
	nFunc-=BEGIN_NUMBER1;
	int i= 0;
	switch(nFunc)
	{
	case 0:
		j=pInput->fInputParam[0].b;
		for(i=j;i<pInput->nNumData ;i++)
			pInput->fOutputParam.line[i] = sin(pInput->fInputParam[0].line[i]);
		pInput->fOutputParam.b=j;
		break;
	}
	return 0;
}
