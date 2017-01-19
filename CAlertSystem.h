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
	SYMBOL_ISALERTED m_symbolAlarm;//��Ʊ����
	CArray<DATA_PER_LINE,DATA_PER_LINE&>  m_lineInfoArr;//������������Ϣ
//	char m_symbol[8];//��Ʊ����
//	CArray<float,float&>  m_pricePreArr;//��һ�μ���ʱ�ļ۸�
//	CArray<int,int&>  m_gapFromBeginArr;//���������������ߵ�һ������ڸ���

//	bool m_bUpwards;//true:�������ϴ�Խ������ʱԤ��
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
	static void CaclAlarm(CMainFrame* pFrame);//����Ԥ������һ��
	static bool SaveFile();//��Ԥ���������浽�ļ���
	static bool LoadFile(bool bLine=false);//���ļ��ж���Ԥ������

	CAlertSystem(CWnd* pParent = NULL);   // standard constructor

	CTaiShanDoc * pDoc;
	bool m_bStartAlarm;//�Ƿ�������Ԥ��
	static CListAlarmSet m_gAlarmArray;//Ԥ����������//
	static CListAlarmLineSet m_gLineAlarmArray;//������Ԥ����������//CListAlarmLineSet
	static CList<ALERT_RESULT_ITEM*,ALERT_RESULT_ITEM*&> m_gAlarmResultList;//Ԥ���������
	static CAlertSystem* m_gDlgAlarmSystem;//Ԥ��ϵͳ�Ի���ָ��
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
	afx_msg void OnAdd();//����Ԥ������
	afx_msg void OnDelete();//ɾ��Ԥ������
	afx_msg void OnModify();//�޸�Ԥ������
	afx_msg void OnClear();//�����¼
	afx_msg void OnClearAll();//���ȫ����¼
	afx_msg void OnWriteToFile();//�Ѽ�¼д���ļ�
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
