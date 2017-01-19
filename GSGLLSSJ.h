#include "afxtempl.h"
#include "StructTaiShares.h"	// Added by ClassView

#if !defined(AFX_GSGLLSSJ_H__804D1366_B12B_11D1_83E6_0000E8593F1A__INCLUDED_)
#define AFX_GSGLLSSJ_H__804D1366_B12B_11D1_83E6_0000E8593F1A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// GSGLLSSJ.h : header file
//

class CTaiShanDoc;

/////////////////////////////////////////////////////////////
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

/////////////////////////////////////////////////////////////
//���������ݴ洢��ʽ 32 ���ֽ�
struct FENXIJIA
{
	DWORD dwDate;    //ʱ  �䣺���Σ�������
	float fOpen;     //���̼ۣ�float
	float fHigh;     //��߼ۣ�float
	float fLow ;     //��ͼۣ�float
	float fClose;    //���̼ۣ�float
	float fAmount;   //�ɽ��float
	float fVolume;   //�ɽ�����float
	float fUnknown;  //δ֪���ĸ��ֽڣ�

};

/////////////////////////////////////////////////////////////
//our���ݴ洢��ʽ, ����32���ֽ�
struct TAI_SHAN_DATA_cw
{
		int	    nTime;      // ��  �ڣ�����  
		float	fOpen;		// ���̼�
		float	fHigh;		// ��߼�
		float	fLow ;		// ��ͼ�
		float 	fClose;     // ���̼�
		float 	fVolume;    // �ɽ���
		float	fAmount;    // �ɽ����
		WORD    wAdvance;   // ���Ǽ���
		WORD    wDecline;   // �µ����� 
};

/////////////////////////////////////////////////////////////
//������ݴ洢��ʽ ��29���ֽ�
struct  HUIJIN
{
	int   nDate;         // ��  �ڣ�  19980219 ��1998/02/19]
	float fOpen;          // ���̼ۣ�   
	float fHigh;          // ��߼ۣ�   
	float fLow;           // ��ͼۣ�   
	float fClose;         // ���̼ۣ�   
	float fMean;          // �ɽ��   
	int   nVolume;        // �ɽ�����  ��λ�ǹɣ��������� 
	BYTE  chUnknown;      // һ���ֽ�
};

//ʤ�������ݽṹ.
struct SHENGLONG
{
	long  date;
	long  oprice;	
	long  cprice;
	long  hprice;	
	long  lprice;
	long  tovalue;	
	long  tohands;
	long  topiece;
	char  free[8];
};

////////////////////////////////////////////////
///���ļ�ͷ
struct KLINEHEAD_cw
{
	int FileID;                 //���ļ�ͷ
	int	NumPerBlk	;			//ÿС��Ĺ�ƱK����������,256��
	int Free1;                  //Ԥ��1 
	int	NumStock	;			//֤ȯ����
	int	NumBlockAll;			//С������
	int Free2;					//Ԥ��2 
};


///////////С�ļ�ͷ
struct KLINESMALLHEAD_cw
{
	char    StockSign[8]; //ǰ8���ֽ�Ϊ��Ʊ���롣
	short 	numChuQ	;	  //2���ֽ�Ϊ��Ȩ���ݵ��ܴ���
	long    numKln	;	  //4���ֽ����ͣ�������K�ߵĸ�����
	short   symBlock[25]; //����ÿ�����ֽڴ���K�����ݵĿ��
};
/////////
//��Ȩ���ݽṹ
struct POWER_cw       //��32���ֽ�
{
	BYTE  byFree[12];  //Ԥ��12���ֽ�
	int   nTime;
	float fGive;
	float fAllocate;
	float fAllocatePrice;
	float fDividend;
};
////////////
///////////5������ͷ�ļ��ṹ
struct MINUTEKLINEHEAD_cw
{
	char    StockSign[8]; //ǰ8���ֽ�Ϊ��Ʊ���롣
	long    numKln	;	  //4���ֽ����ͣ���������ߵĸ�����
	long    pPos;         //д�������ָ��
	long    symBlock;     //���
};
////////////////////////////////
//5���ӽṹ
//1--4 �ֽ� �ļ�ͷ   
//5--8 ÿС��5����K���������� ����75�� 
//9--12 ��֤֤ȯ����
//13--16 ����
struct MKLINEHEAD_cw
{
	int IDFile;
	int	numPerBlk	;			//ÿС��Ĺ�Ʊ5����K����������,75*48��
	int	numKline_sh	;			//��֤֤ȯ����
	//int numKline_sz	;	        //��֤֤ȯ����
	int unKnown;
};

/////////////////////////////////////////////////////////////////////////////
// CGSGLLSSJ dialog
class CTaiShanDoc;
class CGSGLLSSJ : public CPropertyPage
{
	DECLARE_DYNCREATE(CGSGLLSSJ)

// Construction
public:
	CTaiShanDoc* pDoc;

	CString DestinationFile;
	CString SourceFileName;
	CArray <Kline,Kline &> m_InstallArray;
	
	CGSGLLSSJ();
	~CGSGLLSSJ();

// Dialog Data
	//{{AFX_DATA(CGSGLLSSJ)
	enum { IDD = IDD_LSSJGL };
	CProgressCtrl	m_ProgressYield;
	CButtonST	m_check_fixtime;
	CEdit	m_destination;
	CButtonST	m_SearchFixData;
	CEdit	m_DataSource;
	CComboBox	m_ComboDatatype;
	CProgressCtrl	m_progress1;
	BOOL	m_sh;
	BOOL	m_sz;
	int		m_Cover;
	int		m_AssignPeriod;
	int		m_NumberOfDays;
	CTime	m_install_start;
	CTime	m_install_end;
	CTime	m_produce_end;
	CTime	m_produce_start;
	//}}AFX_DATA

public:
	BOOL IsFileExists( CString filename);
	void SplitOneFileToManyFiles(CString Dir,CString DestFileName,bool bDayKline = true);
	BOOL InstallFromFloppyDisk(CString FloppyFileName,bool bDayKline = true);
	void Produce2FloppyDisk(CString DestFileName,CString FloppyFileName);

	CFile m_OutMinuteFile;
	CFile m_LocalFile;
	CFile m_DestFile;
	CFile m_file;
	KLINEHEAD_cw m_BigFileHead;
	char* file_sz;
	char* file_sh;
	void YieldDay(CString filename, int &NumberOfStock);
	void AssignDays(CString filename, int &NumberOfStock);  //ָ����������

	LONG GetStockDay(time_t NumSeconds );
	int myf(CString szStr);

	BOOL GetStockTime(int ymd, LONG& tm);
	void InstallHuijin  (CString srcfilename, int BeginDate, int EndDate); 
	void InstallQianlong(CString srcfilename, int BeginDate, int EndDate); 
	void InstallWanshen (CString srcfilename, int BeginDate, int EndDate); 
	void InstallFenxijia(CString srcfilename, int BeginDate, int EndDate); 
	// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CGSGLLSSJ)
	public:
	virtual void OnOK();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CGSGLLSSJ)
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonSearchfixdata();
	afx_msg void OnButtonBeginInstall();
	afx_msg void OnRadioRepaire();
	afx_msg void OnButtonDestination();
	afx_msg void OnButtonBeginproduce();
	afx_msg void OnRadioDay();
	virtual void OnCancel();
	afx_msg void OnSelchangeComboDatatype();
	afx_msg void OnRadioInstalldate();
	afx_msg void OnRadioCover();
	afx_msg void OnRadioAssignperiod();
	afx_msg void OnRadioRecentdays();
	afx_msg void OnSelchangeIntype();
	afx_msg void OnCloseupOuttype();
	afx_msg void OnChangeEditAssigndays();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

///////////////////////

public:
	int YieldMinuteFile(bool bShangHai = true,bool bAssignDays = true);
	BOOL ShenglongToTianlong(SHENGLONG shenglong, Kline& wanshen );
//	int InstallFromSeveralDiskfile_min(CString FileName);
//	void SplitOneFileToManyFiles_min(CString Dir, CString DestFileName);
	void InstallShenglong(CString srcfilename,int BeginDate, int EndDate);
	int InstallFromSeveralDiskfile(CString FileName,bool bDayKline = true);
    void InstallMinteData();
    void ProduceMinuteData() ;
private:
	void InstallStockKlineEach(CString symbol,int BeginDate,int EndDate,bool bDayKline = true);
};



//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GSGLLSSJ_H__804D1366_B12B_11D1_83E6_0000E8593F1A__INCLUDED_)
