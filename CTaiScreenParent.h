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
       int  iKind;                      //��ʾ���������ࡣ
	   char sIndex_name[50];             //ָ������ơ�
       int  iLine;                      //��ʾ�ڼ����ߡ�
       int  iDataType;                  //��ʾѡ�ɵ����͡�
       float fParam[8];                   //��ʾ����1��
       float fSubParam1;                //��ʾ�Ӳ���1��
       float fSubParam2;                //��ʾ�Ӳ���2��
       int  iSunIndex;                  //��ʾ�ڼ����ӹ�ʽ��
	   BOOL bBuyCase;					//�Ƿ�����������,0 = ��������,1 = 
	   int iBeginFoot;					//�˹�ʽ����Ч�Ŀ�ʼ����

	   //20020301
	   int	iLineParam[2];		//�Ӳ���1�����࣬m_subParam1 = CB_ERR ��ʹ��fSubParam1��
										//m_subParam1 = 1����ʾ���õ�2��ָ����...
	   tagIndexDataInfo()
	   {
	   };
}IndexDataInfo;
struct SharesNameArr3//
{
	char m_sSymbol[20];//������ƻ��Ʊ����
	int  m_nType;//-1 :��Ʊ ��0-8���ࣻ1000-1999��飻2000-2999ѡ�ɰ�飻3000��ǰҳ
};
struct SYMBOL_ISALERTED//
{
	char	m_sSymbol[8];//��Ʊ����
	BYTE	m_btAlarmed;//0 :
	int		m_stkKind;//
};

typedef CArray<SharesNameArr3,SharesNameArr3&> CArrayStockType;
typedef CArray<SYMBOL_ISALERTED,SYMBOL_ISALERTED&> CArrayIsAlarmed;
;
struct ALERT_SET_ITEM//
{
	char	m_strAlarmName[16];	//Ԥ������
	bool	m_bAnd;				//����֮��Ĺ�ϵ
	BYTE	m_btAddtional[4];	//��������
	CArray<IndexDataInfo,IndexDataInfo&> m_conditionArray;		//��������
	CArray<SharesNameArr3,SharesNameArr3&>  m_symbolTypeArray;	//��Ʊ��������
	CArray<SYMBOL_ISALERTED,SYMBOL_ISALERTED&> m_bAlarmedArray;	//�����Ƿ���Ԥ������Ϣ
};
struct ALERT_RESULT_ITEM
{
	int		m_nFootAlarmArr;//Ԥ�����������к�,Ԥ������������±�
	char	m_symbol[8];	//��Ʊ����
	time_t	m_time;			// Ԥ��ʱ��
	float   m_fPrice;		//����ʱ�Ĺ�Ʊ�۸�
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
	BOOL m_bCaclYHParam;//����ָ������ʱ���Ƿ��Ż�����
	int m_nLineType[64];//������ֵ������
	CTime m_tmHistory;
	CListCtrl * m_pStockSelectList;
	CWnd* m_pExplainButton;
	static CImageList m_imageList ;
	bool m_bShowBuyCase;//�Ƿ�������������ַ���
	bool m_bDlgIndicator;//�Ƿ�������ָʾ
	bool m_bDlgTest;// �Ƿ���ѡ������
	bool m_bXgnExt;

	static CFormularContent*	m_gpIndex[5];
	CArrayStockType  m_symbolTypeArray;//��Ʊ��������
	ADDITIONAL_BUYSELL buySellAdditional;
	SymbolKindArr m_stockchoose;		//����ѡ�ɵ�CARRAY
	SymbolKindArr m_resultarray;		//��Ž����CARRAY
	static ARRAY_BE m_gLineOut;		//���������
	static CString m_gPeriodName[];	//ʱ����������
	static CString m_subIndexGroup[] ;//����ָ���ӹ�ʽ����
    static int nLKind[12];			//ʱ�����ڵ����к�
	CArray<SCREEN_RESULT*,SCREEN_RESULT*&> m_testResultArray;

	IndexDataInfo* SetCurrentFormular();	//Set Current Formular to "m_indexSaveArray"
	CTaiShanDoc * pDoc;				//�ĵ�����ָ��
	CArray<IndexDataInfo,IndexDataInfo>	m_indexSaveArray;//�����������
													//to save the fomular selected to a array

	bool IsSuccess(CString StockId,int stkKind,int * pnBuyResult,int * pnSellResult,int nB,int nCount, Kline *pKline, int range, float profit,int bBuySell,CTime* tmEnd = NULL ,CTime* tmBegin  = NULL);
						//

	static float FormularComputeShares(CString symbol,int stkKind,int kCounts, IndexDataInfo* indexSv,bool& bSucc,Kline *&pKline,CBuySellList* pBuySellList,bool bTjxg = false ,ARRAY_BE* pLine = NULL);//to FormularCompute Each Stock and each formular

	static CFormularContent* LookUpArray(int iKind,CString sIndex,CTaiShanDoc* pDoc);
	static int ReadKLine(CString symbol,int stkKind,Kline*& pKline,int nRead,int nKlineType,CBuySellList* pBuySellList = NULL,int nAddBlank=48,bool bToday=true,CTime * tmEnd = NULL,CTime *tmBegin = NULL);//��ȡԭʼ����

	CTaiScreenParent(int nIDD,CWnd* pParent = NULL);   // standard constructor
	virtual ~CTaiScreenParent();   // standard constructor

	//�õ�����ָ���ܵõ���Чֵ����Ҫ��;
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
	void AddStockToList(CListCtrl* pList);//�ѹ�Ʊ���뵽�б���
	void AddIndexArrayToList(IndexDataInfo* pIndex_Save = NULL);//���������뵽�б���
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
