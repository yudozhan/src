// CTaiKlineDoFenshi.h: interface for the CTaiKlineMin1 class.
//		
// 文件名：CTaiKlineDoFenshi.h

// 内容描述：
//		
//		根据实时行情数据画实时行情曲线，并把相应的数据画到内存位图上。
//		
//		本类的输入：CTaiShanKlineShowView视图类的指针及相应的文档指针。
//		
//		本类的输出：把计算出的数据画到内存位图上。
//		
//
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LDOFENSHI_H__20183F81_BDC6_11D2_91D9_0080C8E1242B__INCLUDED_)
#define AFX_LDOFENSHI_H__20183F81_BDC6_11D2_91D9_0080C8E1242B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
const int  FS_ZOUSHI=		0;
const int  FS_VOL	=		1;
const int  FS_MMLD	=		2;
const int  FS_LB	=		3;
const int  FS_LINXIAN	=	4;
const int  FS_DUOKONG	=	5;
const int  FS_TONGLUO	=	6;
#include "CTaiShanDoc.h"

#include "CTaiKlineDoKline.h"

class CTaiShanDoc;
class CTaiShanKlineShowView;
class CTaiKlineFileHS;
class CTaiKlineDialogShiDuanTJ ;
class CTaiKlineMin1  :public	CTaiKlineShowKline
{
	DECLARE_DYNCREATE(CTaiKlineMin1)
public:
	int TransferX(int x);//把x坐标转换为对应的数组下标
	static void GetFenJia(int first,CBuySellList &buySellList,CFJList&	fenjiaUp,CFJList&	fenjiaDown,int nFlag = 0);
	void SetRectDraw(int nFigu);
	CReportData m_dt;	//个股的数据
	CBuySellList m_hsMin1 ;//换手数据的链表
	TRADE_DETAIL_H_PER	m_hsMin[240];//转变到分钟线的换手数据
	bool 	m_bHist;//是否是显示历史回忆

	Tidxd **m_Tidx;//大盘数据
	Rsdn1 **m_Nidx;//大盘数据
	CTaiKlineFileHS*	m_pFileHs;//换手数据文件指针
	int m_nCnp;//是否显示成交量堆积
	float m_amount[240];//成交额
	float m_vol[240];//成交量
	float m_volPre;//上一次的成交量
	int  num_sun;//右边小图的第几个子图
	float m_max_dapan[2][2];//大盘数据的最大值
	float m_min_dapan[2][2];//大盘数据的最小值
	float* m_lineDapan[2][3];//大盘数据数组
	float m_close_Dapan[2];//大盘的收盘价
	float m_close;//昨日收盘价
	int m_pFlg[5];//子图的标志信息 
	int		m_isDapan;//是否是大盘指标
	CReportData* m_pReportData;	//个股的数据指针
	CReportData* m_pS0;	//个股的数据指针
	CReportData* m_pS1;	//个股的数据指针
	CString m_nameSon[5];//子图的名称
	bool m_bInvertRect;		//是否把矩形反色

	int m_nCount;


	void ShowCNP(CDC *pDC,int nFlags=0);//显示成交量堆积

	void DrawDapanOther(CDC* pDC);
	//功能:画大盘数据子图

	void CaclMaxAdded();
	//功能:计算主图叠加数据最大值

	void AddFenshiAdded(CString symbol, int nStock,int stkKind);
	//功能:增加主图叠加股票
	//输入参数:symbol要增加的股票名称,nStock是第几个股票

//	int OneTo5(Kline* pKline,int nFlags,CString symbol,int n);//当天分时数据转换为5分钟K线
	//输入参数:symbol股票名称,pKline是传入的数据指针,nFlags标识何种K线

	void ShowMultiFiguer(CDC *pDC,int flag=0);//显示多图
	void DrawRulorX(CDC *pDC);//画x轴标尺
	int OutString(int num, CPoint p);//输出字符串
	//输入参数:num是第几个子图,p是鼠标的点

	int OnHome();//home箭头键处理
	int OnEnd();//end箭头键处理
	int OnRight();//right箭头键处理
	int OnLeft();//left箭头键处理
	CString TimeToString(int foot,bool bRight= false);//时间转换为字符串
	//输入参数:foot数据的下标

	void DrawLittleFrame(CDC* pDC,int nMarket=0);//画小窗口
	void DrawLittleDapan(CDC* pDC,int flagLine,int nMarket=0);//flagLine=0为分时线，flagLine=1为均价,flagLine=2为成交量	void DrawLittleGegu(CDC* pDC);//画右边小图
	void DrawLittleGegu(CDC *pDC);//画个股小窗口
	void DrawDapan(CDC *pDC,int nMarket=0,int flagFrame=0);//画右边小图
	int DrawHs(CDC *pDC,int nBegin, BYTE flag);//画右边成交明细小图
	//输入参数:nBegin是开始显示的数据的下标
	
	void InitHs(bool bRemoveAll=true,bool bSkip = true);//初始化换手及分价数据
	//输入参数:bRemoveAll=true是把成交明细链表全部清除再读入

	void DrawCapt(CDC* pDC);//画标题
	void DrawSon(CDC* pDC);//画子图
	void InitMinuteLine();//初始化参数
	//输入参数:symbol股票代码
//	CTaiShanDoc* pDoc;//文档指针
//	CTaiShanKlineShowView* pView;//视图指针

	void DrawMin1Figuer(CDC* pDC);//画分时画面
	CTaiKlineMin1();
	CTaiKlineMin1(CTaiShanDoc* pDocI,CTaiShanKlineShowView* pViewI	);

	float TongJi(CTaiKlineDialogShiDuanTJ* pTJ,int nFlags=0);//统计数据
	//输入参数：pTJ为时段统计对话框指针
	//输出参数：返回是否成功

	virtual ~CTaiKlineMin1();

};

#endif // !defined(AFX_LDOFENSHI_H__20183F81_BDC6_11D2_91D9_0080C8E1242B__INCLUDED_)
