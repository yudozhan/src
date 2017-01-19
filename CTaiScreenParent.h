#if !defined(AFX_TJXGPARENT_H__AD765044_1448_11D4_96F3_0080C8E1242B__INCLUDED_)
#define AFX_TJXGPARENT_H__AD765044_1448_11D4_96F3_0080C8E1242B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CTaiScreenParent.h : header file
//


/////////////////////////////////////////////////////////////////////////////
// CTaiScreenParent dialog
#include "CTaiKlineTreeCtl.h"
#include "StructKlineView.h"
#include "ParamEdit.h"
#include "CTaiShanDoc.h"

#include "FloatEdit.h"
#include "MyEdit.h"
//#include "BtnST.h"

////////////Sub_index
//
typedef struct tagIndexDataInfo
{
       int  iKind;                      //表示条件的种类。
	   char sIndex_name[50];             //指标的名称。
       int  iLine;                      //表示第几条线。
       int  iDataType;                  //表示选股的线型。
       float fParam[8];                   //表示参数1。
       float fSubParam1;                //表示子参数1。
       float fSubParam2;                //表示子参数2。
       int  iSunIndex;                  //表示第几种子公式。
	   BOOL bBuyCase;					//是否是买入条件,0 = 买入条件,1 = 
	   int iBeginFoot;					//此公式的有效的开始个数

	   //20020301
	   int	iLineParam[2];		//子参数1的种类，m_subParam1 = CB_ERR ，使用fSubParam1；
										//m_subParam1 = 1，表示引用第2条指标线...
	   tagIndexDataInfo()
	   {
	   };
}IndexDataInfo;
struct SharesNameArr3//
{
	char m_sSymbol[20];//板块名称或股票代码
	int  m_nType;//-1 :股票 ；0-8分类；1000-1999板块；2000-2999选股板块；3000当前页
};
struct SYMBOL_ISALERTED//
{
	char	m_sSymbol[8];//股票代码
	BYTE	m_btAlarmed;//0 :
	int		m_stkKind;//
};

typedef CArray<SharesNameArr3,SharesNameArr3&> CArrayStockType;
typedef CArray<SYMBOL_ISALERTED,SYMBOL_ISALERTED&> CArrayIsAlarmed;
;
struct ALERT_SET_ITEM//
{
	char	m_strAlarmName[16];	//预警名称
	bool	m_bAnd;				//条件之间的关系
	BYTE	m_btAddtional[4];	//附加条件
	CArray<IndexDataInfo,IndexDataInfo&> m_conditionArray;		//条件数组
	CArray<SharesNameArr3,SharesNameArr3&>  m_symbolTypeArray;	//股票代号数组
	CArray<SYMBOL_ISALERTED,SYMBOL_ISALERTED&> m_bAlarmedArray;	//保存是否已预警的信息
};
struct ALERT_RESULT_ITEM
{
	int		m_nFootAlarmArr;//预警条件的序列号,预警条件数组的下标
	char	m_symbol[8];	//股票代号
	time_t	m_time;			// 预警时间
	float   m_fPrice;		//计算时的股票价格
	int		m_stkKind;
};
class CTaiShanDoc ;
class CFormularContent;


//##ModelId=397D27770046
class CTaiScreenParent : public CDialog
{
   	//friend class CMainFrame;
// Construction
public:
	BOOL m_bCaclYHParam;//加入指标条件时，是否优化参数
	int m_nLineType[64];//返回数值的线形
	CTime m_tmHistory;
	CListCtrl * m_pStockSelectList;
	CWnd* m_pExplainButton;
	static CImageList m_imageList ;
	bool m_bShowBuyCase;//是否加入买入条件字符串
	bool m_bDlgIndicator;//是否是买卖指示
	bool m_bDlgTest;// 是否是选股评测
	bool m_bXgnExt;

	static CFormularContent*	m_gpIndex[5];
	CArrayStockType  m_symbolTypeArray;//股票代号数组
	ADDITIONAL_BUYSELL buySellAdditional;
	SymbolKindArr m_stockchoose;		//所有选股的CARRAY
	SymbolKindArr m_resultarray;		//存放结果的CARRAY
	static ARRAY_BE m_gLineOut;		//输出计算结果
	static CString m_gPeriodName[];	//时间周期名称
	static CString m_subIndexGroup[] ;//技术指标子公式名称
    static int nLKind[12];			//时间周期的序列号
	CArray<SCREEN_RESULT*,SCREEN_RESULT*&> m_testResultArray;

	IndexDataInfo* SetCurrentFormular();	//Set Current Formular to "m_indexSaveArray"
	CTaiShanDoc * pDoc;				//文档对象指针
	CArray<IndexDataInfo,IndexDataInfo>	m_indexSaveArray;//保存条件组合
													//to save the fomular selected to a array

	bool IsSuccess(CString StockId,int stkKind,int * pnBuyResult,int * pnSellResult,int nB,int nCount, Kline *pKline, int range, float profit,int bBuySell,CTime* tmEnd = NULL ,CTime* tmBegin  = NULL);
						//

	static float FormularComputeShares(CString symbol,int stkKind,int kCounts, IndexDataInfo* indexSv,bool& bSucc,Kline *&pKline,CBuySellList* pBuySellList,bool bTjxg = false ,ARRAY_BE* pLine = NULL);//to FormularCompute Each Stock and each formular

	static CFormularContent* LookUpArray(int iKind,CString sIndex,CTaiShanDoc* pDoc);
	static int ReadKLine(CString symbol,int stkKind,Kline*& pKline,int nRead,int nKlineType,CBuySellList* pBuySellList = NULL,int nAddBlank=48,bool bToday=true,CTime * tmEnd = NULL,CTime *tmBegin = NULL);//读取原始数据

	CTaiScreenParent(int nIDD,CWnd* pParent = NULL);   // standard constructor
	virtual ~CTaiScreenParent();   // standard constructor

	//得到计算指标能得到有效值所需要的;
	static int GetCountValid(IndexDataInfo* indexSv, CTaiShanDoc *pDoc, int nAdd);
public:
	CStringArray nLineName;
	void GetIndexLineName(CFormularContent *pIndex);
public:
	bool GetCurSelData(IndexDataInfo& indexSave);
	static int GetFinanceFoot(int nFootIn);
	static  int GetPeriodFoot(int nKlineType);
	float GetProfitAverage(float& fRateSucc,int& nBuy);
	bool TestSuccessEach(CString StockId,int stkKind,int nCount, Kline *pKline, int nRange, float fProfit,CBuySellList* pList = NULL,CTime* tmEnd = NULL,CTime* tmBegin = NULL);
	float CaclPrice(Kline* pKline,int nEnterPoint,int& nMove);
	BOOL LoadTjxgZsFile(CString FileName,bool bShow=true);
	void WriteTizhToFile(CString FileName,int m_and_or,bool bZh=true,int m_mbzq=20,float m_mbly=10.0);
	static int IsPeriodValid(int nKlineType,CFormularContent* pJishu);
	static void ListCtrlSetSelectItem(CListCtrl* pList,int nItem = -1);
	static CString GetPeriodName(int nKlineType);
	void AddAStockKeyBoard(CListCtrl* pList);
	float GetParamValue(int i);
	void ClearTestResultArray();
	float CaclMid(Kline kline);
	static CFormularContent* GetSpecialLJishu(int nJishu=0);
	void AddStockToList(CListCtrl* pList);//把股票加入到列表中
	void AddIndexArrayToList(IndexDataInfo* pIndex_Save = NULL);//把条件加入到列表中
	static void FillToListCtrlHead(CListCtrl* pList);
	void ShowExplain(CFormularContent * pJishu );
	void ReAddPeriodName();
	static int FromKlineKindToFoot(int nKlineKind);
	//##ModelId=397D2777026F
	BOOL UpdateParams(IndexDataInfo *pIndex_save);
	//##ModelId=397D27770124
	CParamEdit *pEdit;
	CTaiKlineTreeCtl	m_treeFormularCtrl;
protected:
	static CFont m_Font;
	CFormularContent*	m_pIndex;  //////store the selected index////
	int nKind;         ///////KLine Type/////////
	HTREEITEM m_hItem;
// Dialog Data
	//{{AFX_DATA(CTaiScreenParent)
//	enum { IDD = IDD_PARENT_TJXG };
	CTaiTestEdit	m_conRich;
	CListBox	m_choose_list;
	CComboBox m_sSubFormular;
	CComboBox m_sLineAll;
//	CListCtrl	m_param_list;
	CComboBox	m_period;
	int		m_and_or;
	int		m_bBuy;
	CButton	m_btAddCond;
	CButton	m_btDeleteCond;
	CButton	m_btOpenGroup;
	CButton	m_btSaveGroup;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTaiScreenParent)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void PostNcDestroy();
	virtual BOOL PreTranslateMessage(MSG* pMsg) ;
	//}}AFX_VIRTUAL

// Implementation
protected:
	void DeleteStocks();
	void AddStocks();
	virtual void NotifySubclass();

	// Generated message map functions
	//{{AFX_MSG(CTaiScreenParent)
	virtual BOOL OnInitDialog();
	afx_msg void OnDblclkParamList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSetfocusParamList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnClickTreeFormularAll(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelchangedTreeFormularAll(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnAdd();
	afx_msg void OnSave();
	afx_msg void OnDelete();
	afx_msg void OnLoad();
	afx_msg void OnSelchangeListSubFormular();
	afx_msg void OnDestroy();
	afx_msg void OnReturnTreeFormularAll(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblclkTreeFormularAll(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelchangeListFormularLineAll();
	afx_msg void OnClickListParam(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelchangeTabSelect(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	void SetHorizonExtent();
	static CString GetIndexLineName(CFormularContent *pIndex,int nLine);
	static CString GetGuoteIndex(IndexDataInfo* indexSv,CFormularContent *pIndex = NULL);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TJXGPARENT_H__AD765044_1448_11D4_96F3_0080C8E1242B__INCLUDED_)
