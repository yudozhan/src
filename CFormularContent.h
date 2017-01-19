#if !defined(AFX_LJISHU_H__A3E1ADE5_B7F2_11D1_83EC_0000E8593F1A__INCLUDED_)
#define AFX_LJISHU_H__A3E1ADE5_B7F2_11D1_83EC_0000E8593F1A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CFormularContent.h : header file
//
#define CFormularArray CArray<CFormularContent*,CFormularContent*>
#define LEN_BYTE	240
struct FLOAT11
{
	float fValue[11];
};
/*enum ENUM_ENTER_POINT
{
	MIDDLE = 0;//�������м�
	MIDDLE ;//�������м�
	MIDDLE ;//�������м�
	MIDDLE ;//�������м�
	MIDDLE ;//�������м�

}*/
struct ADDITIONAL_BUYSELL
{
	int nEnterPoint[4];		//�����
	/*
	0:�������м�
	1:�����ڿ���
	2:���������
	3:���������
	4:����������
	5:�������м�
	6:�����ڿ���
	7:���������
	8:���������
	9:����������
	*/
	COLORREF nColorPoint[4];		//�������ɫ

	BYTE bLose;	//ʹ�����������ʧ��
	BYTE bWin;	//ʹ���������ӯ����
	BYTE bMaxDays;///ʹ���������
	BYTE bSubstractFee;//ʹ�ÿ۳�������
	BYTE bAddPreProfit;//����ǰһ�ε�ӯ�����м���
	BYTE bUseInitFinace;//ʹ�ó�ʼ�ʽ�
	BYTE bRes1;
	BYTE bRes2;

	float fMaxLose;//���������ʧ��
	float fMaxWin;//�������ӯ����
	float fMaxDays;//�������
	float fInitFinace;//��ʼ�ʽ���
	float fRes1;
	float fRes2;
	float fRes3;
	float fRes4;
	ADDITIONAL_BUYSELL()
	{
		for(int i=0;i<4;i++)
			nEnterPoint[i]=5;
		nColorPoint[0]=0x00ff0000;
		nColorPoint[1]=0x0000ff00;
		nColorPoint[2]=0x00ff0000;
		nColorPoint[3]=0x0000ff00;
		bLose =0;
		bWin =0;
		bMaxDays =0;
		bSubstractFee =0;
		bAddPreProfit =0;
		bUseInitFinace =0;
		fMaxLose = -0.2f;//���������ʧ��
		fMaxWin = 0.5;//�������ӯ����
		fMaxDays = 20;//�������
		fInitFinace = 10000;//��ʼ�ʽ���
	};
};


/////////////////////////////////////////////////////////////////////////////
// CFormularContent command target
class CTaiShanDoc ;
class CFormularContent : public CObject
{
// protected constructor used by dynamic creation

// Attributes
public:
		CFormularContent();
		union
		{
			struct
			{
				float		max[8];	//���ֵ
				float		min[8];	//	��Сֵ
				float		defaultVal[8];	//ȱʡֵ
//				float		defaultValArray[11][8];	//ȱʡֵ
				//0:HS 1:MIN1 2:MIN5 3:MIN15 4:MIN30 5:MIN60 6:DAY (MANYDAY ) 7:WEEK
				//8:MONTH 9:YEAR 

				float		stepLen[8];	//����
				float		posX[8];		//Y������ĺ��ߵ���ֵ
				float		fReserved;		//

				int			nKindPeriod;		//
				//0:HS 1:MIN1 2:MIN5 3:MIN15 4:MIN30 5:MIN60 6:DAY  7:WEEK
				//8:MONTH 9:YEAR 10:MANYDAY 
				BYTE	nPeriodsUsed[16];	//��Ч����

				BOOL	isSystem;		//�Ƿ���ϵͳָ��
				BOOL	isOfen;		//�Ƿ��ǳ���ָ��
				BOOL	isProtected;		//�Ƿ����뱣��

				BYTE	isMainFiguer;	//	�Ƿ�����ͼ���ӣ�0Ϊ��ͼ��1Ϊ��ͼ��2Ϊ�����ã�
				BYTE	numPara;	//��������(���Ϊ8������)
				BYTE 	posFlag;		//Y������ĺ��ߵ�ʽ����0Ϊ�Զ���1Ϊ���ƣ�����7������2Ϊ�����ã�
				BYTE 	isAdditionalCond;		//�Ƿ��и�������

				BYTE	bYHParam[8];//�Ƿ��Ż�����
				BYTE	bYH;//�Ƿ��Զ��Ż�����
				BYTE	bRightBox;//�Ƿ����ұߵ�ָ��

				//�Ƿ������Է�������ָ�꣬���nIsFromServer = 33134999�����Է�������ָ��
				int		nIsFromServer;//

			};
			BYTE btMemData[LEN_BYTE];
		};
		ADDITIONAL_BUYSELL *		pAdditionalBS;
		CArray<FLOAT11,FLOAT11&>	defaultValArray;	

		CString		name;		//��������ָ������(���9���ַ�)
		CString		password;	//����
		CString		explainBrief;	//	ָ��˵��
		CString		explainParam;	//	����˵��

		CString		namePara[8];	//�������֣����15���ַ�)
		CString		fomular;		//��ʽ
		CString		help;		//����ע��

		CString		buyStr;       //���������
		CString		sellStr;       //����������
		CString		buyStrKong;       //���������
		CString		sellStrKong;       //����������
		// add ,3.13
		CString		subKindIndex;		//��������ָ������,ȱʡֵ�� ""
		CString		subKindIndexTime;		//��������ָ������
		CString		strReserved;		//

		static bool m_bVer20;
		static CStringArray m_strArrayKind[4];
		float		defaultValYH[8];	//�Ż�ȱʡֵ


// Operations
public:
	CString GetFormular();
	static bool ExportFormular(CFormularArray* pArr,int nArr,CString fileName,BOOL bCompleteSecret, BOOL bPassword, CString strPassword);
	static bool InstallIndicator(CString sFilePathName,bool bPromp = false,bool bFromServer = true);
	static bool InstallIndicatorCwd(CString sFilePathName,bool bPromp = false,bool bFromServer = true);
	static float GetParamDataEach(int iIndex,int nKlineType,CFormularContent* pJishu);
	static void  DeleteKindName(CString s, int nKind);
	static void AddKindName(CString s,int nKind = 0 );
	static void ReadWriteIndex(CTaiShanDoc *pDoc,int nKind=0,bool bRead=true);
	bool IsValid();
	void SecretForm(CString& s,bool bLoad );
	void AddDefaultValToArray();
	void InitDefaultValArray();
	void SerializeData(CArchive& ar);
	void SetData(CFormularContent * data);
	


	virtual ~CFormularContent();
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFormularContent)
	//}}AFX_VIRTUAL

// Implementation
protected:
	
	virtual void Serialize(CArchive& ar);
	DECLARE_SERIAL(CFormularContent)

	// Generated message map functions
	//{{AFX_MSG(CFormularContent)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG


};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LJISHU_H__A3E1ADE5_B7F2_11D1_83EC_0000E8593F1A__INCLUDED_)
