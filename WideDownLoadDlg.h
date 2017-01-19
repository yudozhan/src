#if !defined(AFX_WIDEDOWNLOADDLG_H__39DA3222_EC25_4E96_92B4_F868D55F49A0__INCLUDED_)
#define AFX_WIDEDOWNLOADDLG_H__39DA3222_EC25_4E96_92B4_F868D55F49A0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// WideDownLoadDlg.h : header file
//
#include "ClientTransmitManageWnd.h"

typedef enum WideDownLoadAttributes{ 
	WideTodayDayLine,              //��������
	WideTodayAllDayLine,           //����ȫ��
    WideHistoryDayLine,            //��ʷ����  
    WideFiveMinuteLine,            //�����K������ 
    WideAllFiveMinuteLine,         //ȫ�������K������ 
	WideHistoryMinuteLine,         //�������ʷK������ 
	WideTodayDetail,               //������ϸ����
	WideAllTodayDetail,            //����ȫ����ϸ����
	WideHistoryDetail,             //��ʷ��ϸ����
	WideSingleStockBaseInfomation, //��������
	WideStockBaseInfomation,       //ȫ������
	WideSingleExRight,             //��Ȩ����
	WideExRight,                   //ȫ����Ȩ����
	WideSingleCaiwuData,           //�������� 
	WideCaiwuData                  //ȫ���������� 
};
typedef struct {
	WideDownLoadAttributes m_DownLoadAttributes;     //������������
	char m_strDownLoadName[MAX_PATH];                //����֤ȯ����
	int m_dwDownLoadType;                            //������������ 
	char m_strDownLoadDate[MAX_PATH];                //��������ʱ��ѡ��
    DWORD m_dwDownLoadTotalCounts;                   //��������
	DWORD m_dwDownLoadCompleteCounts;                //��������
    time_t *m_tDownLoadTimes;                        //������ϸ����ʱ��
	DWORD m_dwTimeTotalCounts;                       //������ϸ����������
    DWORD m_dwTimeCompleteCounts;                    //��������ϸ��������
} WIDEREQUEST_CONTEXT,*PWIDEREQUEST_CONTEXT;

/////////////////////////////////////////////////////////////////////////////
// CDownLoadNetDlg dialog
class CTaiKlineWideNet;
class CMainFrame;
class CDownLoadNetDlg : public CDialog
{
// Construction
public:
	CDownLoadNetDlg(CWnd* pParent = NULL);   // standard constructor
    CTaiShanDoc *m_pDoc;
	char m_szCurrentDir[MAX_PATH];
	CImageList * m_pImageList;    //��ͼ���б�
	CImageList * m_pImageListSmall; //Сͼ���б�

    CMainFrame *m_pMainFrm;
	TRANS_PACKAGE_STRUCTEx m_TransPackageStruct;
	TransmitStockAttribute m_TransmitStockAttribute;
	SOCKET_ID m_SocketID;
	BOOL m_bDownLoadState;
	CTaiKlineWideNet *m_pTechWideNet;


public:
    void FillToListCtrlHead();    //��ʼ��CListCtrl����
    void FillToTreeCtrlHead();    //��ʼ��CTreeCtrl������
    BOOL InsertDownLoadDataToList(WIDEREQUEST_CONTEXT *pRequestContext );
    void AddDownLoadData(); 
    BOOL CheckList(WideDownLoadAttributes DownLoadAttributes,int dwDownLoadType,
								 char *strDownLoadName=NULL);
    int GetStockMarketCounts(int DownLoadType);
    void GetTimeString(time_t t1,time_t t2,char *strTime);
    void GetStartAndEndTime(time_t &t1,time_t &t2,char *strTime);
    void GetHistoryDetailTimeCounts(time_t t1,time_t t2,PWIDEREQUEST_CONTEXT &pRequestContext);
    void AddDownLoadDataInfo(BOOL bIsSendMsg);
    CReportData *GetCdatPointFromStockTypeAndPos(char strDownLoadName[MAX_PATH],int nStockType,int Pos);
    void DispalyDownLoadInfomation();


// Dialog Data
	//{{AFX_DATA(CDownLoadNetDlg)
	enum { IDD = IDD_WIDEDOWNLOAD };
	CTreeCtrl	m_MyTree;
	CListCtrl	m_lstStockTypeName;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDownLoadNetDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void PostNcDestroy();
	virtual void OnCancel();
	//}}AFX_VIRTUAL


// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDownLoadNetDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnClose();
	virtual void OnOK();
	afx_msg void OnLoadCollecte();
	afx_msg void OnSaveCollecte();
	afx_msg void OnAddLoaddata();
	afx_msg void OnDeleteLoaddata();
	afx_msg void OnDownloadStart();
	afx_msg void OnStopdownload();
	afx_msg void OnSetupTime();
	afx_msg void OnDblclkSelectLoaddata(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnClickList1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblclkList1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnKeydownList1(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
    afx_msg LRESULT OnPostFocus(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnAddStock(WPARAM wParam, LPARAM lParam);
    afx_msg LRESULT OnRecvTransmitInfoMation(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WIDEDOWNLOADDLG_H__39DA3222_EC25_4E96_92B4_F868D55F49A0__INCLUDED_)
