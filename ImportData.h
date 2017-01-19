#if !defined(AFX_IMPORTDATA_H__B5C1E14C_A053_4797_845A_C1BA8330EEA6__INCLUDED_)
#define AFX_IMPORTDATA_H__B5C1E14C_A053_4797_845A_C1BA8330EEA6__INCLUDED_
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ImportData.h : header file
#define WM_DRAWLIST WM_USER+131
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
/*
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
*/

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
/*struct KLINEHEAD_cw
{
	int FileID;                 //���ļ�ͷ
	int	NumPerBlk	;			//ÿС��Ĺ�ƱK����������,256��
	int Free1;                  //Ԥ��1 
	int	NumStock	;			//֤ȯ����
	int	NumBlockAll;			//С������
	int Free2;					//Ԥ��2 
};
*/

///////////С�ļ�ͷ
/*
struct KLINESMALLHEAD_cw
{
	char    StockSign[8]; //ǰ8���ֽ�Ϊ��Ʊ���롣
	short 	numChuQ	;	  //2���ֽ�Ϊ��Ȩ���ݵ��ܴ���
	long    numKln	;	  //4���ֽ����ͣ�������K�ߵĸ�����
	short   symBlock[25]; //����ÿ�����ֽڴ���K�����ݵĿ��
};
*/

/////////
//��Ȩ���ݽṹ
/*
struct POWER_cw       //��32���ֽ�
{
	BYTE  byFree[12];  //Ԥ��12���ֽ�
	int   nTime;
	float fGive;
	float fAllocate;
	float fAllocatePrice;
	float fDividend;
};
*/
////////////
///////////5������ͷ�ļ��ṹdelete 2004-11-16
/*struct MINUTEKLINEHEAD_cw
{
	char    StockSign[8]; //ǰ8���ֽ�Ϊ��Ʊ���롣
	long    numKln	;	  //4���ֽ����ͣ���������ߵĸ�����
	long    pPos;         //д�������ָ��
	long    symBlock;     //���
};*/
////////////////////////////////
//5���ӽṹ
//1--4 �ֽ� �ļ�ͷ   
//5--8 ÿС��5����K���������� ����75�� 
//9--12 ��֤֤ȯ����
//13--16 ����

/*
struct MKLINEHEAD_cw
{
	int IDFile;
	int	numPerBlk	;			//ÿС��Ĺ�Ʊ5����K����������,75*48��
	int	numKline_sh	;			//��֤֤ȯ����
	//int numKline_sz	;	        //��֤֤ȯ����
	int unKnown;
};
*/

typedef struct
{
	    float 	Price;      //    �۸� 
		float 	Volume;     //    �� 
		float 	Amount;     //    ��
        float   accb;       //    ����ҵ��ͣ���������ҵ��ͣ�
        float   accs;  
}KdataOld;

//Cdat2�ṹ��Ҫ���ڼ��ռ�ʱ��Ʊ������Ϣ(Ver2.0)
struct  CdatOld
{
 BYTE kind;
 BYTE  rdp[240];           //      ���ڼ������ǿ��ָ��
 char id[8];             //      ֤ȯ����
 char name[10];           //      ֤ȯ���� 
 char Gppyjc[6];         //      ֤ȯ����ƴ�����
 int   sel;
 float ystc;              //      ��������
 float opnp;              //      ���տ��� 
 float higp;              //      �������
 float lowp;              //      �������
 float nowp;              //      ���¼۸�
 float nowv;              //      ���³ɽ���
 float totv;              //      �ܳɽ���
 float totp;              //      �ܳɽ����
 float pbuy1;             //      �����һ
 float vbuy1;             //      ������һ
 float pbuy2;             //      ����۶�  
 float vbuy2;             //      ��������
 float pbuy3;             //      ������� 
 float vbuy3;             //      ��������      
 float pbuy4;             //      ������� 
 float vbuy4;             //      ��������      
 float pbuy5;             //      ������� 
 float vbuy5;             //      ��������      
 float psel1;             //      ������һ
 float vsel1;             //      ������һ
 float psel2;             //      �����۶� 
 float vsel2;             //      ��������   
 float psel3;             //      ��������
 float vsel3;             //      �������� 
 float psel4;             //      ��������
 float vsel4;             //      �������� 
 float psel5;             //      ��������
 float vsel5;             //      �������� 
 float accb;              //      ����ҵ��ͣ���������ҵ��ͣ�
 float accs;              //      �����ҵ��ͣ����������ҵ��ͣ�
 float volume5;           //      5�����
 float rvol;              //      ����
 float dvol;              //      ����
 
 short lastclmin;         //      �ϴδ��������
 short initdown;          //      ��Ʊ��ʼ����־   0 = δ�ܳ�ʼ����1=�ѳ�ʼ��   
 BOOL  InOut;
 
 float StartP;        //      ��ʼʱ��۸� 
 float EndP;          //      ����ʱ��۸�
 float Res[11];
 KdataOld   m_Kdata[240];
 int      nReservation;
};



class CTaiShanDoc;
class CImportData : public CPropertyPage
{
	DECLARE_DYNCREATE(CImportData)

// Construction
public:
    CTaiShanDoc * pDoc;
	CArray<BOOL,BOOL> m_stockrange;
	CString DestinationFile;
	CString SourceFileName;
	CArray <Kline,Kline &> m_InstallArray;

	CImportData();
	~CImportData();

// Dialog Data 
	//{{AFX_DATA(CImportData)
	enum { IDD = IDD_DATAIMPORT };
	CButton	m_button1;
	CDateTimeCtrl	m_ctrlTimeEnd;
	CDateTimeCtrl	m_ctrlTimeStart;
	CListCtrl	m_listfw;
	CButton	m_okbutton;
	CProgressCtrl	m_progress1;
	CComboBox	m_ComboDatatype;
	CComboBox	m_ComboFiletype;
	CEdit	m_DataSource;
	int m_importAll;
	CTime	m_install_end;
	CTime	m_install_start;
	//}}AFX_DATA
public:
    BOOL IsFileExists(CString filename);
    void SplitOneFileToManyFiles(CString Dir,CString DestFileName,bool bDayKline =true);
    BOOL InstallFromFloppyDisk(CString FloppyFileName,bool bDayKline=true);
	void Produce2FloppyDisk(CString DestFileName,CString FloppyFileName);
 
	CFile m_OutMinuteFile;
	CFile m_LocalFile;
	CFile m_DestFile;
	CFile m_file;
	//KLINEHEAD_cw m_BigFileHead;
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
	//{{AFX_VIRTUAL(CImportData)
	public:
	virtual void OnOK();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CImportData)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangedatatype();
	afx_msg void OnSelchangedataformat();
	afx_msg void OnSetfocusfwxz();
	afx_msg void OnSetfocus();
	afx_msg void Onqbyr();
	afx_msg void Onsdyr();
	afx_msg void OnSetfocusup();
	afx_msg void OnSetfocusdown();
	afx_msg void Onimport();
	afx_msg void OnButtonSearchfixdata();
	afx_msg void OnClickList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
	//}}AFX_MSG
	//afx_msg void drawzqsc(WPARAM,LPARAM);
	DECLARE_MESSAGE_MAP()


public:
	int IsFileType(CString Filename);
	BOOL m_bImport;
	static void InstallOneMinStock(CString FilePath);
	CString m_news;
	BOOL m_Shenzhen;
	BOOL m_Shanghai;
	int m_nImportorcancel;
	int m_nFlag;
	int YieldMinuteFile(bool bShangHai = true,bool bAssignDays = true);
	BOOL ShenglongToTianlong(SHENGLONG shenglong, Kline& wanshen );
//	int InstallFromSeveralDiskfile_min(CString FileName);
//	void SplitOneFileToManyFiles_min(CString Dir, CString DestFileName);
	void InstallShenglong(CString srcfilename,int BeginDate, int EndDate);
	int InstallFromSeveralDiskfile(CString FileName,bool bDayKline = true);
    void InstallMinteData();
    void InstallMin(CString lpStockSymbol);
	void ProduceMinuteData() ;

private:
	void InstallStockKlineEach(CString symbol,int wMarketType,int BeginDate,int EndDate,bool bDayKline = true);


};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_IMPORTDATA_H__B5C1E14C_A053_4797_845A_C1BA8330EEA6__INCLUDED_)
