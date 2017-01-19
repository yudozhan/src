#if !defined(AFX_ALERTSYSTEM_H__F218F663_69E0_11D4_970C_0080C8D6450E__INCLUDED_)
#define AFX_ALERTSYSTEM_H__F218F663_69E0_11D4_970C_0080C8D6450E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CAlertSystem.h : header file
//
#include "CTaiScreenParent.h"

/////////////////////////////////////////////////////////////////////////////
// CAlertSystem dialog
class CMainFrame;
class CTaiShanDoc ;
typedef CList<ALERT_SET_ITEM*,ALERT_SET_ITEM*&> CListAlarmSet;
struct DATA_PER_LINE
{
	INFO_LINESELF lineInfo;
	int m_gapFromBeginArr;
	BYTE m_btAlarmed;
	BYTE m_bUpwards;
};
struct ALERT_LINE_ITEM//
{
	SYMBOL_ISALERTED m_symbolAlarm;//股票名称
	CArray<DATA_PER_LINE,DATA_PER_LINE&>  m_lineInfoArr;//趋势线数据信息
//	char m_symbol[8];//股票名称
//	CArray<float,float&>  m_pricePreArr;//上一次计算时的价格
//	CArray<int,int&>  m_gapFromBeginArr;//昨日数据与趋势线第一点的周期个数

//	bool m_bUpwards;//true:从下向上穿越趋势线时预警
};
typedef CList<ALERT_LINE_ITEM*,ALERT_LINE_ITEM*&> CListAlarmLineSet;

class CAlertSystem : public CDialog
{
// Construction
public:
	void ClearAllResult();
	static void SwitchAlarm();
	static void GetScreenStockBlockSymbol(SymbolKindArr & sArr,CString sBlockName,int k=-1);
//	CImageList m_imageList;
	static void GetLineAlarmInitData(ALERT_LINE_ITEM* item);
	static void DeleteLineAlarmItem(INFO_LINESELF &lineInfo,CString s);
	static void AddLineAlarmItem(INFO_LINESELF& lineInfo,CString s,bool bUpwards);
	static void AlarmSendMessage(CMainFrame* pFrame,CReportData* dt1 ,ALERT_SET_ITEM* item=NULL,int nCondFoot=-1);
	static void CaclLineAlarm(CMainFrame *pFrame,int nLineAlarm);
	static void CreateNew();
	static void StartAlarm(bool bFirst = true);
	static void ListCtrlToSymbolType(CArrayStockType &typeArr,CListCtrl* pList);
	static void InsertRow(CListCtrl* pList,CString *strPt,int nStr,int nImag,int nPos=-1,int stkKind = 0);
	static void GetSymbolArray(CArrayStockType& typeArr,SymbolKindArr & symbolArr);
	static void CaclAlarm(CMainFrame* pFrame);//进行预警计算一次
	static bool SaveFile();//把预警条件保存到文件中
	static bool LoadFile(bool bLine=false);//从文件中读出预警条件

	CAlertSystem(CWnd* pParent = NULL);   // standard constructor

	CTaiShanDoc * pDoc;
	bool m_bStartAlarm;//是否启动了预警
	static CListAlarmSet m_gAlarmArray;//预警条件数组//
	static CListAlarmLineSet m_gLineAlarmArray;//趋势线预警条件数组//CListAlarmLineSet
	static CList<ALERT_RESULT_ITEM*,ALERT_RESULT_ITEM*&> m_gAlarmResultList;//预警结果数组
	static CAlertSystem* m_gDlgAlarmSystem;//预警系统对话框指针
// Dialog Data
	//{{AFX_DATA(CAlertSystem)
	enum { IDD = IDD_MYALERT_SYSTEM };
	CButton	m_button7;
	CButton	m_button6;
	CButton	m_button4;
	CButton	m_button3;
	CXTListCtrl	m_alarmResultList;
	CListCtrl	m_condArrayList;
	//CListCtrl	m_alarmResultList;
	//CListCtrl	m_condArrayList;
	CButton m_btOK;
	CButton m_btCancel;
	CButton m_btAdd;
	CButton m_btDelete;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAlertSystem)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CAlertSystem)
	afx_msg void OnAdd();//增加预警条件
	afx_msg void OnDelete();//删除预警条件
	afx_msg void OnModify();//修改预警条件
	afx_msg void OnClear();//清除记录
	afx_msg void OnClearAll();//清除全部记录
	afx_msg void OnWriteToFile();//把记录写到文件
	virtual BOOL OnInitDialog();
	virtual void OnCancel();
	virtual void OnOK();
	afx_msg void OnClose();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	void AlarmArrInsertRow(ALERT_SET_ITEM* item ,int nItem = -1);
	static void ResultListAddRow(CListCtrl* pList,int nRow);
	void FillListctrlHeader();
	afx_msg void OnNMDblclkList2(NMHDR *pNMHDR, LRESULT *pResult);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ALERTSYSTEM_H__F218F663_69E0_11D4_970C_0080C8D6450E__INCLUDED_)
