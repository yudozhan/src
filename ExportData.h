#if !defined(AFX_EXPORTDATA_H__538A8A7F_AB92_4E5D_8A10_6759A0995D07__INCLUDED_)
#define AFX_EXPORTDATA_H__538A8A7F_AB92_4E5D_8A10_6759A0995D07__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ExportData.h : header file
#include "FloatEdit.h"
#define WM_DRAWL WM_USER+132
/////////////////////////////////////////////////////////////////////////////
// CExportData dialog
/*
//Ǯ�����ݴ洢��ʽ, 40���ֽ�
struct QIANLONG
{
	DWORD dwDate;          // ��  �ڣ�  19980219 ��1998/02/19]
	DWORD dwOpen;          // ���̼ۣ�   ��λ��Ԫ*1000
	DWORD dwHigh;          // ��߼ۣ�   ��λ��Ԫ*1000
	DWORD dwLow;           // ��ͼۣ�   ��λ��Ԫ*1000
	DWORD dwClose;         // ���̼ۣ�   ��λ��Ԫ*1000
	DWORD dwAmount;        // �ɽ��   ��λ��Ԫ/1000  ��ȡ�����������룩
	DWORD dwVolume;        // �ɽ�����   ��λ������
	DWORD dwUnknow;        // δ֪����   �� 0  Ԥ��4�ֽڵĿռ�
    WORD  wUp;             // ���Ǽ���   (ֻ�Դ���)
    WORD  wDown;           // ���Ǽ���   (ֻ�Դ���)
    DWORD dwGuben;         // �ܹɱ�
};
*/

class CExportData : public CPropertyPage
{
	DECLARE_DYNCREATE(CExportData)

// Construction
public:
	//void drwalist();
	BOOL IsCreatePath(CString PathName,BOOL bPath);
	void ProduceToSoftDisk();
	CString m_news;
	BOOL m_Shenzhen;
	BOOL m_Shanghai;
	int m_nShijianflag;
	//int m_nFlag;
    CArray<BOOL,BOOL> m_stockrange;
	CString SourceFileName;
	CFile m_DestFile;
	void CExportData::YieldDay(CString filename,int &NumberOfStock,int bFormat);
	void CExportData::AssignDays(CString filename, int &NumberOfStock,int bFormat);
	void CExportData::Produce2FloppyDisk(CString DestFileName,CString FloppyFileName);
	void CExportData::SplitOneFileToManyFiles(CString Dir,CString DestFileName,bool bDayKline);
    int CExportData::YieldMinuteFile(bool bShangHai,bool bAssignDays,int bFormat);
    BOOL CExportData::ProduceMin(LPTSTR lpStockSymbol,int bFormat,int nKind);
	BOOL CExportData::IsDirectoryExist();
	void CExportData::ProduceMinuteData(int bFormat);
	CTaiShanDoc* pDoc;

	CExportData();
	~CExportData();
    void CExportData::ProduceDayKline(CString FileName,int BeginDate,int EndDate,int bFormat);
	void CExportData::ProduceDayWbKline(CString DestFileName,int BeginDate,int EndDate);
	BOOL IsFileExists( CString filename);

// Dialog Data
	//{{AFX_DATA(CExportData)
	enum { IDD = IDD_DATAEXPOR };
	CButton	m_cancel;
	CButton	m_button1;
	CListCtrl	m_listfw;
	CFloatEdit	m_NumberOfD;
	CComboBox	m_DataFormat;
	CSpinButtonCtrl	m_contSpin;
	CProgressCtrl	m_ProgressYield;
	CEdit	m_destination;
	int		m_AssignPeriod;
	int		m_NumberOfDays;
	CTime	m_install_start;
	CTime	m_install_end;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CExportData)
	public:
	virtual void OnOK();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CExportData)
	virtual BOOL OnInitDialog();
	afx_msg void Onzjts();
	afx_msg void Onsdxz();
	afx_msg void OnButtonDestination();
	afx_msg void OnButtonBeginproduce();
	afx_msg void OnSelchangeDatetFormat();
	afx_msg void OnSelchangeFiletype();
	afx_msg void OnClickList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EXPORTDATA_H__538A8A7F_AB92_4E5D_8A10_6759A0995D07__INCLUDED_)
