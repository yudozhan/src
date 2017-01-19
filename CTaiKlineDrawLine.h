// CTaiKlineDrawLine.h: interface for the CTaiKlineDrawing class.
//		
// 文件名：CTaiKlineDrawLine.h

// 创建者：

// 创建时间：

// 内容描述：
//		处理自画线数据及进行画线操作。
//		
//		本类的输入：CTaiShanKlineShowView视图类的指针及相应的文档指针。
//		
//		本类的输出：把自画线画到屏幕上，把自画线数据写到文件中。
//		
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LDRAWLINE_H__75FC8BA1_C4DC_11D2_91DE_0080C8E1242B__INCLUDED_)
#define AFX_LDRAWLINE_H__75FC8BA1_C4DC_11D2_91DE_0080C8E1242B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
typedef CArray<INFO_LINESELF,INFO_LINESELF&> infoLineself;

class CTaiShanDoc;//CTaiShanDoc
class CTaiShanKlineShowView;
class CTaiKlineDrawing :public CObject 
{
public:

	bool IsTrendLine();
	void SetAlarmFlag(int nLine,BYTE bAlarm);
	void SetCurrentLineProp(COLORREF rgb=0, int nLineType=0, int nLineThick=0, bool bAlarm=false,int nParam = 0);
	bool GetCurrentLineProp(COLORREF & rgb,int& nLineType,int& nLineThick,bool& bAlarm);
	//函数
	void CaclTwoPointXD(CPoint &p1, CPoint &p2);
	//功能:计算线段的两个点
	//输入参数:P1,P2为输入的两个点数据
	void ArcInRect(CDC* pDC,int  x1,int y1,int x2,int y2);
	//功能:画圆弧
	//输入参数:X1,Y1,X2,Y2为输入的两个点的x、y坐标数据
	bool IsCrossSideFloat(int nSide,float x1, float y1, float x2, float y2,CPoint &pOut);//nSide = 0 is top,1 is right,2 is bottom ,3 is left
	//功能:判断直线的位置，是否可见
	//输入参数:X1,Y1,X2,Y2为输入的两个点的x、y坐标数据，nSide = 0 is 高,1 is 右,2 is 底 ,3 is 左
	//输出参数:pOut是输出的点
	void CaclTwoPointFloat(float x1,float y1,float x2,float y2);
	//功能:计算线段的两个点
	//输入参数:X1,Y1,X2,Y2为输入的两个点的x、y坐标数据

	bool IsCrossSide(int nSide,CPoint &p1, CPoint &p2,CPoint &pOut);//nSide = 0 is top,1 is right,2 is bottom ,3 is left
	//功能:判断直线的位置，是否可见
	//输入参数:X1,Y1,X2,Y2为输入的两个点的x、y坐标数据，nSide = 0 is 高,1 is 右,2 is 底 ,3 is 左
	//输出参数:pOut是输出的点

	void ShowText(CDC* pDC,float in,int y,int nLine = -1);
	//功能:显示文本
	//输入参数:Y为输入的点的y坐标数据，in为输入的数据
	void SetPointHuiGui(int foot,int footOld);//设置线性回归线的点
	//输入参数:foot为输入的第二个数据下标，footOld为输入的第一个数据下标
	//---函数
	int SetPointPos(CPoint& point);//确定画线的端点
	//输入参数:point输入点
	void DrawPointBE(CDC* pDC);//画自画线的端点矩形
	void DrawXianDuan(CDC* pDC,CPoint& p1,CPoint& p2);//画线段
	//输入参数:p1，p2为两个输入点
	void DrawXianDuanVirtul(CDC *pDC,CPoint& p1,CPoint& p2,COLORREF* clr = NULL);//画虚线段
	//输入参数:p1，p2为两个输入点

	void DeleteLineAll();//删除所有画线
	void DeleteLine();//删除画线
	void AddLine();//加一条画线
	int FloatToY(float val,float& out,int flag=0);//把浮点数转换为Y轴坐标或反之
	//输入参数:val为输入的浮点数
	//输出参数：返回y轴坐标，out为输出的浮点数
	int TimeToX(int time,int flag=0);//把时间转换为X轴坐标或反之 
	void DrawTotal(CDC* pDC);//画所有画线
	int ScreenToFile(int n,bool isToFile,bool bPrinting=false);//把屏幕坐标转换为相对位置
	//输入参数:n为数组的下标，isToFile为判断转换方向的标志字
	//输出参数：返回错误码
	void DrawText(CDC* pDC,CPoint* pp=NULL,CString * s=NULL);//画文本
	void DrawArrowUp(UINT id,CDC* pDC,int nPnt=0,int nWidth = 23);//画箭头
	void DrawHuiGuiTop(CDC* pDC,int flag=0,COLORREF* clr=NULL);//画回归带的上下两线
	void CaclTwoPoint(CPoint& p1,CPoint& p2,int flag=0);//计算直线与画图矩形的交点
	//功能:计算线段的两个点
	//输入参数:X1,Y1,X2,Y2为输入的两个点的x、y坐标数据
	int DrawSelfLine(CDC* pDC,bool bSetProp = false);//画一个自画线
	void DrawRect(CDC* pDC,CRect &r);//画矩形
	int IsOnPoint(CPoint &p1, CPoint &p2, CPoint &pointIn, int flag);//是否在点或矩形上
	//输入参数:p1，p2为两个输入端点，pointIn为要判断的输入点
	//输出参数：返回输入点所在的位置
	int IsOnLine(CPoint &p1, CPoint &p2, CPoint &pointIn, int flag);//输入点是否在线上
	//输入参数:p1，p2为两个输入端点，pointIn为要判断的输入点
	//输出参数：返回输入点所在的位置
	int IsOnSelfLine(CPoint& point);//输入点是否在自画线上 
	//输入参数:point输入点
	int IsOnSelfLineTotal(CPoint& point);//输入点是否在自画线上
	//输入参数:point输入点
	int SetPointMoved(int cx,int cy);//自画线移动时，设置起始点
	//入口参数：cx、cy为宽度和高度
	void SetDrawLineRect();//初始化画线矩形

	//--- 
	int WriteLineself();//把画线写到文件中
	int ReadLineself();//从文件中读出狐仙数据

	//---有关文件的数据 
	int m_nPosStock;//股票在画线文件中的索引号
	int m_nLine;//本股票的画线的个数
	int m_nShares;//画线文件的股票的总个数
	infoLineself	m_info_lineself;//自画线数据


	int m_nCurrentLineClicked;//具有焦点的线对象
	int m_nLineTypeChanging;// 正在被画的线形
	int m_nZhongXianDrawing;//正在画时的第几笔
	CFile* m_fileSh;//画线文件
	CFile* m_fileSz;//画线文件

	//---画线操作的数据
	bool m_bSelected;//当前的线是否被选择
	bool m_bLineValidate;//当前正在画的线是否有效
	int m_nCurrentLine;//当前选中的线的索引

	int m_bMoveZhongXian;//中线线型的第几笔
	int m_nZhongXian;//中线线型的第几笔
	int m_lenTextNow;//文本的长度
	CString  m_textNow;//画出的文本
	int m_nPntGroup[30];//各种画线类型的端点的数目
	int m_lineTypeOld;//上一个自画线的类型
	int m_lineType;//自画线的类型
	bool m_bDrawLine;//是否正在执行画线操作
	int m_nPointCurrent;//当前正在画第几个点
	CPoint m_ptOldPingXing[2];//平行线操作中，原来的线段的坐标
	CPoint m_ptOldDrawing[3];//中线操作中，原来的线段的坐标
	CPoint m_ptOld[3];//鼠标移动操作中，原来的点的坐标
	CPoint m_pt[3];//自画线的端点的屏幕坐标
	CRect m_rtDrawLine;//画线所在的矩形
	float m_widthPer;//每个K线的宽度

	CTaiShanDoc* pDoc;		//CTaiShanDoc的指针
 	CTaiShanKlineShowView* pView;		//CTaiShanKlineShowView的指针

	CTaiKlineDrawing();
	CTaiKlineDrawing(CTaiShanKlineShowView* bsView,CTaiShanDoc* bsDoc);
	virtual ~CTaiKlineDrawing();

};

#endif // !defined(AFX_LDRAWLINE_H__75FC8BA1_C4DC_11D2_91DE_0080C8E1242B__INCLUDED_)
