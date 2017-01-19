// 文件名：TechTreeCtl.h

// 创建时间：2000.8.3

// 内容描述：

// 显示各种指标公式。
/////////////////////////////////////////////////////
#if !defined(AFX_LMBJISHUTREECTL_H__D3B8C427_F659_11D3_96E8_0080C8E1242B__INCLUDED_)
#define AFX_LMBJISHUTREECTL_H__D3B8C427_F659_11D3_96E8_0080C8E1242B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TechTreeCtl.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CTaiKlineTreeCtl window
#include "CTaiShanDoc.h"
#include "CTaiKlineColorTreeCtrl.h"

static CString strNewEquationGroup = "新建公式组";
static CString strDeleteEquationGroup = "删除公式组";
static CString strChangeNameGroup = "更改组名";
static CString strNewEquation = "新建公式";
static CString strModifyEquation = "修改公式";
static CString strDeleteEquation = "删除公式";
static CString strUsageIndex = "指标用法";
static CString strShowIndex = "显示指标";
static CString strSelectStock = "条件选股";
static CString strOftenEquation = "显示常用指标";
static CString strAllEquation = "显示所有指标";
static CString strGroupEquation = "显示分类指标";
static CString strScreenStock = "条件选股";
static CString strScreenStockTest = "条件选股测试";
static CString strScreenStockIndicator = "条件选股指示";
static CString strShowIndicator = "显示指标";
static CString strShowKlineGroup = "K线组合指示";

const int nNewEquationGroup		= 201;
const int nDeleteEquationGroup	= 202;
const int nChangeNameGroup		= 203;
const int nNewEquation			= 204;
const int nModifyEquation		= 205;
const int nDeleteEquation		= 206;
const int nUsageIndex			= 207;
const int nShowIndex			= 208;
const int nSelectStock			= 209;
const int nOftenEquation		= 210; 
const int nAllEquation			= 211;
const int nGroupEquation		= 212; 

const int nScreenStock			= 213;
const int nScreenStockTest		= 214;
const int nScreenStockIndicator = 215;
const int nShowIndicator		= 216;
const int nShowKlineGroup		= 217;

class CTaiShanKlineShowView;
class CTaiKlineTreeCtl : public CTaiKlineColorTreeCtrl
{
// Construction
public:
	CTaiKlineTreeCtl();

// Attributes
public:
	enum
	{
		nKindSubDir = 0,//it is sub directory
		nKindIndex = -1,//技术指标公式
		nKindChoose = -2,  //条件选股公式
		nKindKline = -3,   //K线指标
		nKindMaster = -4,//专家系统公式
		nKindChooseGroup = -5,//条件选股组合
		nKindBaseInfo = -6,//基本面条件选股
		nKindPriceAlert = -7,//价格预警
		nKindUpDownAlert = -8,//涨跌幅预警
		nKindVolAlert = -9,//换手率预警
		nKindBlockIndex = -10,//板块指数
	}nKindItem;//else it is the pointer of LIndex

//	CStringArray		m_strTileArray;
	bool m_bNewGroup;
	bool m_bDeleteGroup;
	bool m_bModifyGroupName;
	bool m_bDeleteEquation;
	bool m_bNewEquation;
	bool m_bModifyEquation;
	bool m_bRefreshEquation;

	bool m_bScreenStock			;
	bool m_bScreenStockTest		;
	bool m_bScreenStockIndicator ;
	bool m_bShowIndicator		;//
	bool m_bShowKlineGroup		;//
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTaiKlineTreeCtl)
	public:
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
	//}}AFX_VIRTUAL

// Implementation
public:
	static CFormularContent * ShowEditIndicatorDlg(CFormularContent * pJishuCurrent,int listitem,int nKindRoot,CString * sGroup = NULL,int nKlineType=-1);
	void ExportFormular(BOOL bCompleteSecret,BOOL bPassword,CString strPassword);
	void ReInsertIterm2(int n);
	void CommandShowKlineGroup();
	void CommandShowIndicator();
	void CommandScreenStockIndicator();
	void CommandScreenStock();
	static CTaiShanKlineShowView* GetTargetView();
	void SelectItemJ(CFormularContent* p);
	void InsertEqutionTreeOther(int  nKind,bool bExtend = false);
	HTREEITEM FindGroupQiTa(HTREEITEM hRoot,CString sName="其它");// );
	int DeleteEquation(HTREEITEM hItem = NULL);//删除公式
	bool ModifyEquation();//修改公式
	bool CreateNewEquation();//新增公式
	bool ModifyGroupName();//修改组名
	bool DeleteGroup();//删除组
	bool CreateNewGroup();//新增组
	bool GetCurrentItemInfo(int & nKindRoot,int& nKindItem,CFormularContent*& pIndex );//得到当前所选公式的信息
	void InsertEqutionTree(int nKind,bool bExtend  = false,int nInsertKind = 0);//把指标公式加入到树状控件中
	void ReInsertIterm(int nID);
	//nInsertKind = 0,分类; nInsertKind = 1,全部;nInsertKind = 2,常用 ;

	virtual ~CTaiKlineTreeCtl();

	// Generated message map functions
protected:
	//{{AFX_MSG(CTaiKlineTreeCtl)
	afx_msg void OnDestroy();
	afx_msg void OnRclick(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
private:
	HTREEITEM RecursiveLook(CFormularContent *p,HTREEITEM root);
	CArray<int ,int&> m_nKindArray;
	CFormularContent* GetLJishuFromInt(int nData);
	HTREEITEM m_hAllIndex;
	int GetFoot(int nKind,CFormularContent* pJishu,bool bDoRemove = false);

	bool m_bShowFromMouse;
protected:
	void RecursiveAddChecked(CFormularArray &arr, HTREEITEM root,bool bTestCheck);
	virtual bool CanBeginDrag();
	virtual bool CanDroped(HTREEITEM	hitem);
	virtual void DoDropOut(CWnd* pWnd,HTREEITEM hItem);
	virtual void DoDropedSelf(HTREEITEM hItem);

};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LMBJISHUTREECTL_H__D3B8C427_F659_11D3_96E8_0080C8E1242B__INCLUDED_)
