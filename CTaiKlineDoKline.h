// CTaiKlineDoKline.h: interface for the CTaiKlineShowKline class.
//		
// 文件名：CTaiKlineDoKline.h

// 创建者：

// 创建时间：

// 内容描述：
//		
//		根据K线数据计算技术指标，并把相应的数据画到内存位图上。
//		
//		本类的输入：CTaiShanKlineShowView视图类的指针及相应的文档指针。
//		
//		本类的输出：把计算出的数据画到内存位图上。
//		
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LDOKLINE_H__47E83166_C70C_11D2_91D8_0080C8E1242B__INCLUDED_)
#define AFX_LDOKLINE_H__47E83166_C70C_11D2_91D8_0080C8E1242B__INCLUDED_

#include "LBsInfoRS.h"	// Added by ClassView
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#define Total_Count_Line						64

#include "StructKlineView.h"
#define BUY_CASE 107
#define SELL_CASE 108

class CTaiShanDoc;//CTaiShanDoc
class CTaiShanKlineShowView;
class CFormularCompute;
class CTaiKlineDialogShiDuanTJ ;
class CTaiKlineMoveChenben;
class CTaiKlineTransferKline;

class CTaiKlineShowKline : public CObject
{
	DECLARE_DYNCREATE(CTaiKlineShowKline)
public:
	void GetRect(CRect &r,int nFiguer);
	int m_nValidIndicator[5];//指标在当前周期是否有效
	void SetRectCurrent(int nFiguer);
	void DrawIndicatorAdded(CDC* pDC);
	CString m_sIndicatorAdded[5];//指标叠加名称
	int m_nLineIndicator1[5];

	CString m_sOldSymbol;//上一次的股票代码
	int m_sOldStkKind;//上一次的股票种类

	int m_nKlineType;
	int m_numOldKline;
	bool ValidDoKline(bool bMakeUp = false);
	CTaiKlineTransferKline* m_pLmbTransferKline;

	int m_nBeginFootTJ;//时段统计的开始点
	int m_nEndFootTJ;//时段统计的结束点
	bool m_bInvertRect;//是否把矩形反色

	bool m_bSetPageData;//是否保存当前画面信息
//	bool m_bBTX;//是否是宝塔线
	CString m_zhiBiao0;//first index name
	int m_highFoot;//最高值数据的下标
	int m_lowFoot;//最低点数据的下标
	int m_bDaPan[4];//是否是大盘
	bool m_bInitFoot;//是否计算起止下标
	CTaiKlineMoveChenben* m_pMoveCB;//计算移动成本分布对象指针

	int m_kLine_mode ;//画K线的方式
	bool m_bNewStock;//是否是新的股票

	bool m_bClosed;//是否收盘
	bool m_bDrived;//是否有驱动卡
	Kline* m_pKlineAdd[6];//主图叠加数据 
	int m_nKlineAdd[6];//主图叠加K线数据的个数 
	int m_nKlineFileAdd[6];//主图叠加K线数据的个数 
	CString m_nameAdd[6];//主图叠加股票名称 
	CString m_symbolAdd[6];//主图叠加股票代号

	//lmb 20041012
	int m_stkKindAdd[6];//主图叠加股票种类

	float m_amountAdd[6];//主图叠加日线转换时的成交金额
	float m_volAdd[6];//主图叠加日线转换时的成交量
	CString m_lineName[5][Total_Count_Line];//线的名字
	int m_nKlineCurrent;//是第几根K线
	int m_nTextBox;//第几个子图
	int m_axisType;//0为普通坐标，1为百分比坐标，2为对数坐标
	float m_currentValue;//当前值
	int m_footCurrent;//当前的数组的下标
	CPoint pt[6];//形成区域的点
	CString m_tipStr;//浮动信息窗口的字符串
	CTaiShanDoc* pDoc;		//CTaiShanDoc的指针
 	CTaiShanKlineShowView* pView;		//CTaiShanKlineShowView的指针
	int	m_lineBgn;		//技术分析指标数据数组的开始下标，
						//它用于标记画图的开始下标
	int m_step[5];					//标尺数字步长

	bool	m_bToCacl;		//是否计算K线数据
	int	  m_klinNumDefault;		//显示K线的初始个数
//	int   m_footChenBenB;			//显示K线的开始的数据的个数
	int   m_footBegin;			//显示K线的开始的数据的个数
	int   m_footEnd;			//显示的K线的最后的数据的个数
	int	  m_klinNumDefaultOld;		//显示K线的上次的初始个数
	int   m_footBeginOld;		//显示K线上次的的开始的数据的个数
	int   m_footEndOld;			//显示的K线上次的的最后的数据的个数
	CRect m_rectDrawLine;		//窗口客户区内用于在其中画图的矩形
	ARRAY_JISHU		m_dataFormular[5];	//保存K线图的四个子图的技术分析曲线数据及属性
	float m_min_sun[5];			//在窗口客户区内某子图内， 画图所依据数据的最小值
	float m_max_sun[5];			//在窗口客户区内某子图内，画图所依据数据的最大值
	int	  m_nSon;			//标识第几个子图
	powerArr m_PowerArray;//

	bool DrawBTX(CDC* pDC);
	//功能：画宝塔线
	void DrawKlineHS(CDC* pDC);
	//功能：画分笔明细图
	void MoveDragMode(CPoint & pPre,CPoint& pNext);
	//功能：画分笔明细图
	//输入参数：pPre第一点，pNext第二点

	void OutBitmap(CDC *pDC,int id,CPoint & p);
	//功能：画位图
	//输入参数：id为位图资源id号，p 为画的位置

	bool DrawRulorTextSelf(CDC * pDC,int wid);
	//功能：画自定义标尺
	//输入参数：wid为标志字

	bool ShowFenshiRulor(CDC *pDC,int wid);
	//功能：画分时线标尺
	//输入参数：wid为标志字
	void ShowVirtualLine(CDC* pDC,CPoint& p1,CPoint& p2);
	//功能：画虚线
	//输入参数：p1为点1，p2为点2

	void ShowTjxgShow(CDC* pDC);//显示条件选股指示
	void DrawPower(CDC* pDC);//画除权
	void DrawKLineAmericaAdded(CDC *pDC,int nKln);//画主图叠加美国线
	//输入参数：nKln为第几组K线
	void WritePower(CString &symbol,int stkKind);//写除权数据
	//输入参数：symbol为股票代码
	int ReadPower(CString& symbol,int stkKind);//读除权数据
	//输入参数：symbol为股票代码
	CTaiKlineShowKline();

	void DrawKLineAddedPage(CDC *pDC);//画主图叠加
	void CaclMaxMin(Kline* pKline,int b,int e);//计算K线数据的最大最小值
	//输入参数：pKline为输入的k线数据，b为起始点，e为结束点
	int CaclEndFoot(int nKline);//计算主图叠加的股票的结束下标
	//输入参数：nKline为第几组K线
	void CaclMaxAdded();//计算主图叠加的最大值
	int CaclBeginFoot(int nKline);//计算主图叠加的股票的开始下标
	//输入参数：nKline为第几组K线
	int FindTimeK(int keyVlu,Kline* pKline,int max);//查找某时间的K线
	//输入参数：keyVlu为要查找的时间，pKline为输入的k线数据，max是k线数据的最大个数
	//输出参数：返回查找到数据数组的下标

	void DeleteKlineAdded();//主图叠加删除K线
	void AddKlineAdded(CString symbol,int stkKind);//主图叠加加入一K线
	//输入参数：symbol为股票代码
	void CaclPercentMaxMin(float& maxPct,float& minPct);//计算百分比最大值
	void DrawBaseInfo(CDC* pDC);//画基本资料
	void RestoreKLine();//
	void ComputePower(CString symbol,int stkKind,Kline* pKline,int nKln);//计算除权
	//输入参数：symbol为股票代码，pKline为输入的k线数据，nKln是k线数据的个数
	int TransferKlineTypeDataToday(Kline*& pKline,CString symbol,int stkKind,int nFlags);//转换今天的数据
	//输入参数：symbol为股票代码，pKline为输入的k线数据，nFlags是标志字
	//输出参数：返回k线数据的个数
	int TransferKlineTypeData(CString symbol,int stkKind,Kline*& pKline,int nFlags=-1,int nKln=-1);//把日线等转换为其他类型
	//输入参数：symbol为股票代码，pKline为输入的k线数据，nFlags是标志字，nKln是k线数据的个数
	//输出参数：返回k线数据的个数
	float TongJi(CTaiKlineDialogShiDuanTJ* pTJ,int nFlags=0);//统计数据
	//输入参数：pTJ为时段统计对话框指针
	//输出参数：返回是否成功
	float ShowChengBen(CDC *pDC,int nFlags=0);//显示成本统计
	void ShowCNP(CDC* pDC,int nFlags=0);//显示成交量堆积
	void ShowMultiPage(CDC* pDC);//显示多图
	void ShowMultiFiguer(CDC* pDC,int flag=0);//显示多图之一
	void ShowLittleRulor(CDC* pDC,int wid=0);//显示多图的标尺
	void DrawRulorX(CDC *pDC);//画时间标尺
	int DrawRulorTextLog(CDC *pDC,BYTE wid=0);//画指数坐标Y标尺
    int OnHome();//按home键
    int OnEnd();//按end键
	int OnUp();//向上箭头处理
	int OnDown();//向下箭头处理
	int OnLeft();//向左箭头处理
	int OnRight();//向右箭头处理
	int TransferX(int x);//把x坐标转换为对应的数组下标
	int OutString(int num, CPoint p);//输出字符串
	//输入参数:num是第几个子图,p是鼠标的点
	int MoveDisplays(int num);//移动显示K 线画面
	//输入参数:num是移动几个周期
	int SetShowTotal(int num);//设置显示的起始点和结束点
	CString TimeToStr(int foot);//时间转换为字符串

	void DrawCaption(CDC* pDC);//画标题
	void DrawCaptionAdded(CDC *pDC);//画标题
	void CalcSonFiguer(CString name);//计算子图数据
	void DrawSonFiguer(CDC* pDC);	//画子图
	CTaiKlineShowKline(CTaiShanKlineShowView* bsView,CTaiShanDoc* bsDoc);

	void DrawRulorText(CDC* pDC,BYTE wid=0);	//画标尺的标记数字
	void DrawKlineFiguer(CDC* pDC);//画K线画面
	void DrawKLine(CDC *pDC);		//把K线画到内存位图上
	void DrawKLineAdded(CDC *pDC,int nKln);		//把K线画到内存位图上
	void DrawKLineAmeric(CDC *pDC);//把K线画到内存位图上
	void InitDrawedLineBE();		//设置画K线的起始点
	int InitLineParam();	//初始化曲线的起始点
	void CaclMaxFlt(Kline *pK,int footBegin);	//计算K线的最大、 最小值
	void CaclMaxFlt(float* pFlt,int footBegin,int nLine=-1);	//计算线的最大、 最小值

	void DrawLineIndex(CDC* pDC,bool bFenshi = false);	//画技术指标曲线

	float YTransfer(int y);		//屏幕坐标Y转换为实际数据
	int YTransfer(float y);		//实际数据转换为屏幕坐标Y

	void CaclRulorMovable(CPoint point);		//计算和画出游标小矩形

	//-- 除权

	virtual ~CTaiKlineShowKline();

private:
	void DrawTextIconLine(CDC *pDC,int i);
	CString GetParamString(CString sName);
	void DrawBTXAdded(CDC *pDC, int nKln);
};
#ifndef _DEBUG  // debug version in CTaiShanKlineShowView.cpp
inline	float CTaiKlineShowKline::YTransfer(int y)		//屏幕坐标Y转换为实际数据
{
	float rtn;
	if((m_rectDrawLine.top-m_rectDrawLine.bottom)==0)
		return 0;

	if(!(m_nSon==0&&m_axisType==2))
		rtn=((float)(m_max_sun[m_nSon]-m_min_sun[m_nSon]))*(y-m_rectDrawLine.bottom)/(m_rectDrawLine.top-m_rectDrawLine.bottom)+m_min_sun[m_nSon];
	else
	{
		rtn=(float)(pow(10,(log10(m_max_sun[m_nSon])-log10(m_min_sun[m_nSon]))
			*(y-m_rectDrawLine.bottom)/(m_rectDrawLine.top-m_rectDrawLine.bottom)
			+log10(m_min_sun[m_nSon])));
	}

	return rtn;
}
inline	int CTaiKlineShowKline::YTransfer(float y)		//实际数据转换为屏幕坐标Y
{
	int rtn;
	if((m_max_sun[m_nSon]-m_min_sun[m_nSon])==0)
	{
		rtn = -10;
		return rtn;
	}

	if(!(m_nSon==0&&m_axisType==2))
		rtn=(int)((float)(m_rectDrawLine.top-m_rectDrawLine.bottom)*(y-m_min_sun[m_nSon])/(m_max_sun[m_nSon]-m_min_sun[m_nSon])+m_rectDrawLine.bottom+0.5);
	else
	{
		if(m_min_sun[m_nSon]<=0 || m_max_sun[m_nSon]<=0 || y<=0)
			return 0;

		rtn=(int)((m_rectDrawLine.top-m_rectDrawLine.bottom)*(log10(y)-log10(m_min_sun[m_nSon]))/(log10(m_max_sun[m_nSon])-log10(m_min_sun[m_nSon]))+m_rectDrawLine.bottom+0.5);
	}
	return rtn;
}
#endif
#endif // !defined(AFX_LDOKLINE_H__47E83166_C70C_11D2_91D8_0080C8E1242B__INCLUDED_)
