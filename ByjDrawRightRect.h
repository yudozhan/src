// ByjDrawRightRect.h: interface for the ByjDrawRightRect class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BYJDRAWRIGHTRECT_H__A20CC169_C2B1_43EC_8385_188CE681B1E9__INCLUDED_)
#define AFX_BYJDRAWRIGHTRECT_H__A20CC169_C2B1_43EC_8385_188CE681B1E9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CTaiShanKlineShowView;
class CTaiShanDoc;

class ByjDrawRightRect  
{
public:
	void DrawCaption(CDC* pDC,CFormularContent* pIndex);
	void GetMaxMin(int j,bool bFirst);
	static void SaveName(CString name);
	static bool GetScale(int n1, int n2, float f1, float f2, float& fOut);
	ByjDrawRightRect();
	virtual ~ByjDrawRightRect();
	ByjDrawRightRect(CTaiShanKlineShowView* bsView,CTaiShanDoc* bsDoc);

	CTaiShanDoc* pDoc;		//CTaiShanDoc的指针
 	CTaiShanKlineShowView* pView;		//CTaiShanKlineShowView的指针
	CRect m_rectDrawLine;		//窗口客户区内用于在其中画图的矩形
	void CalcSonFiguer(CDC* pDC,CRect &rt);
	static CString m_nameIndex;
	void DrawSonFiguer(CDC* pDC,int j);

private:
	int m_nValidIndicator;//指标在当前周期是否有效
	ARRAY_JISHU		m_dataFormular;	//保存子图的技术分析曲线数据及属性
	CString m_lineName[18];//线的名字
	float m_min_sun;			//在窗口客户区内某子图内，画图所依据数据的最小值
	float m_max_sun;			//在窗口客户区内某子图内，画图所依据数据的最大值
//
};

#endif // !defined(AFX_BYJDRAWRIGHTRECT_H__A20CC169_C2B1_43EC_8385_188CE681B1E9__INCLUDED_)
