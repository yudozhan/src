#if !defined(AFX_COLUMNSELECT_H__C0089EA1_E150_11D3_920B_00400552E583__INCLUDED_)
#define AFX_COLUMNSELECT_H__C0089EA1_E150_11D3_920B_00400552E583__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ColumnSelect.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CColumnSelect dialog
//"买四价",
const CString g_sBuySell[] = {"买一价","买二价","买三价","买一量","买二量","买三量","卖一价","卖二价","卖三价","卖一量","卖二量","卖三量"
};
enum{

ID_ROWNUM = 5000   
,ID_STOCKID  
,ID_STOCKNAME
,ID_YSTC     
,ID_OPEN     
,ID_HIGH     
,ID_LOW      
,ID_NEW      
,ID_TOTV     
,ID_NOWV     
,ID_TOTP     
,ID_AVGP     
,ID_ADDE     
,ID_HLP      
,ID_RDR      
,ID_BSSPBAS  
,ID_DIFBS    
,ID_VOLBI    
,ID_ACCB     
,ID_ACCS     
,ID_BUY      
,ID_SEL      
,ID_RVOL     
,ID_DVOL     
,ID_EXCHANGE 
,ID_SYL      
,ID_BUYP1     
,ID_BUYP2     
,ID_BUYP3     
//,ID_BUYP4     
,ID_BUYV1     
,ID_BUYV2     
,ID_BUYV3     
//,ID_BUYV4     
,ID_SELP1     
,ID_SELP2     
,ID_SELP3     
//,ID_SELP4     
,ID_SELV1     
,ID_SELV2     
,ID_SELV3     
//,ID_SELV4     

,ID_ZGB       
,ID_GJG       
,ID_FQRFRG    
,ID_FRG       
,ID_ZGG       
,ID_GZAG      
,ID_LTAG      
,ID_HG        
,ID_BG        
,ID_ZPG       

,ID_ZZC       
,ID_LDZC      
,ID_CQTZ      
,ID_GDZC      
,ID_WXZC      
,ID_LDFZ      
,ID_CQFZ      
,ID_GGQY      
,ID_ZBGJJ     
,ID_YYGJJ     
,ID_MGJZ      
,ID_GDQYBL    
,ID_MGGJJ     

,ID_ZYYWSR    
,ID_ZYYWLR    
,ID_QTYWLR    
,ID_LRZE      
,ID_JLR       
,ID_WFPLR     
,ID_MGSY      
,ID_JZCSYL    
,ID_MGWFPLR   
,ID_MGJZC     


,ID_ZJLS      
,ID_VOLBIFM   
,ID_HLPFM     
,ID_EXCHANGEFM
,ID_ADVSP     

,ID_DELITEM
,ID_DEFAULT

,ID_FIRSTSTK//平均涨幅      
,ID_RSTKRATE//权涨幅

};
/*
#define ID_BUYP1     4000  //
#define ID_BUYP2     4001  //
#define ID_BUYP3     4002  //
#define ID_BUYP4     4003  //
#define ID_BUYV1     4004  //
#define ID_BUYV2     4005  //
#define ID_BUYV3     4006  //
#define ID_BUYV4     4007  //
#define ID_SELP1     4008  //
#define ID_SELP2     4009  //
#define ID_SELP3     4010  //
#define ID_SELP4     4011  //
#define ID_SELV1     4012  //
#define ID_SELV2     4013  //
#define ID_SELV3     4014  //
#define ID_SELV4     4015  //
#define ID_SELV5     4016  //
#define ID_SELV6     4017  //

#define ID_ROWNUM     5000  //行号
#define ID_STOCKID    5001  //代码
#define ID_STOCKNAME  5002  //名称
#define ID_YSTC       5003  //昨收
#define ID_OPEN       5004  //今开
#define ID_HIGH       5005  //最高
#define ID_LOW        5006  //最低
#define ID_NEW        5007  //最新
#define ID_TOTV       5008  //总手
#define ID_NOWV       5009  //现手
#define ID_TOTP       5010  //总额
#define ID_AVGP       5011  //均价
#define ID_ADDE       5012  //涨跌
#define ID_HLP        5013  //震幅
#define ID_RDR        5014  //涨幅
#define ID_BSSPBAS    5015  //委比
#define ID_DIFBS      5016  //委差
#define ID_VOLBI      5017  //量比
#define ID_ACCB       5018  //委买
#define ID_ACCS       5019  //委卖
#define ID_BUY        5020  //委买价
#define ID_SEL        5021  //委卖价
#define ID_RVOL       5022  //内盘
#define ID_DVOL       5023  //外盘
#define ID_EXCHANGE   5024  //换手率
#define ID_SYL        5025  //市盈率

#define ID_ZGB        5026//总股本(万股)
#define ID_GJG        5027//国家股(万股)
#define ID_FQRFRG     5028//发起人法人股(万股)
#define ID_FRG        5029//法人股(万股)
#define ID_ZGG        5030//职工股(万股)
#define ID_GZAG       5031//公众A股(万股)
#define ID_LTAG       5032//流通A股(万股)
#define ID_HG         5033//Ｈ股(万股)
#define ID_BG         5034//B股(万股)
#define ID_ZPG        5035//转配股(万股)

#define ID_ZZC        5036//总资产(万元)
#define ID_LDZC       5037//流动资产(万元)
#define ID_CQTZ       5038//长期投资(万元)
#define ID_GDZC       5039//固定资产(万元)
#define ID_WXZC       5040//无形资产(万元)
#define ID_LDFZ       5041//流动负债(万元)
#define ID_CQFZ       5042//长期负债(万元)
#define ID_GGQY       5043//股东权益(万元)
#define ID_ZBGJJ      5044//资本公积金(万元)
#define ID_YYGJJ      5045//盈余公积金(万元)
#define ID_MGJZ       5046//每股净值(元)
#define ID_GDQYBL     5047//股东权益比率(%)
#define ID_MGGJJ      5048//每股公积金(元)

#define ID_ZYYWSR     5049//主营业务收入(万元)
#define ID_ZYYWLR     5050//主营业务利润(万元)
#define ID_QTYWLR     5051//其它业务利润(万元)
#define ID_LRZE       5052//利润总额(万元)
#define ID_JLR        5053//净利润(万元)
#define ID_WFPLR      5054//未分配利润(万元)
#define ID_MGSY       5055//每股收益(元)
#define ID_JZCSYL     5056//净资产收益率(%)
#define ID_MGWFPLR    5057//每股未分配利润(元)
#define ID_MGJZC      5058//每股净资产(元)


#define ID_ZJLS       5059 //资金(%d分)
#define ID_VOLBIFM    5060 //量比(%d分)
#define ID_HLPFM      5061 //震幅(%d分)
#define ID_EXCHANGEFM 5062 //换手率(%d分)
#define ID_ADVSP      5063 //涨速(%d分)


#define ID_DELITEM    5064 //删除表项
#define ID_DEFAULT    5065 //恢复表项
*/
struct DynaTechniqueDef{
   char  	m_nString[50];					// color name (in string table)
   char     m_nExplain[50];
   UINT     m_nID;                      // menu ID
};

typedef CTypedPtrList<CPtrList, DynaTechniqueDef*> m_DynaTechniqueDef;


class CTaiShanReportView;
class CColumnSelect : public CDialog
{
// Construction
public:
	int GetId(int nIn);
	void ShowMenu();
	CColumnSelect(CWnd* pParent = NULL);   // standard constructor
	CColumnSelect(UINT nID,CString nStr,BOOL nDel,BOOL IsJS=FALSE,int x=0,int y=0,CWnd* pParent = NULL);   // standard constructor
	~CColumnSelect();   

	int m_nPos_x;               //保存X坐标位置
	int m_nPos_y;               //保存Y坐标位置
	UINT m_nSelectID;          //保存选择ID号
	CString m_sSelectStr;      //保存选择技术指标名称
	BOOL  m_bIsTechnique;        //是否为技术指标
	m_DynaTechniqueDef m_DynaTechnique; //

	BOOL m_bIsDelete;           //是否删除
	BOOL m_bIsFirst;            //是否第一次打开
	CTaiShanReportView *pView;
// Dialog Data
	//{{AFX_DATA(CColumnSelect)
	enum { IDD = IDD_COLUMN_SELECT };
	CTreeCtrl	m_MyTree;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CColumnSelect)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual int DoModal();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CColumnSelect)
	virtual BOOL OnInitDialog();
	void OnDblclk(NMHDR* wParam, LRESULT *lParam);
	afx_msg void OnClose();
	virtual void OnOK();



	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COLUMNSELECT_H__C0089EA1_E150_11D3_920B_00400552E583__INCLUDED_)
