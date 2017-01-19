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
	MIDDLE = 0;//本周期中价
	MIDDLE ;//本周期中价
	MIDDLE ;//本周期中价
	MIDDLE ;//本周期中价
	MIDDLE ;//本周期中价

}*/
struct ADDITIONAL_BUYSELL
{
	int nEnterPoint[4];		//进入点
	/*
	0:本周期中价
	1:本周期开盘
	2:本周期最高
	3:本周期最低
	4:本周期收盘
	5:次周期中价
	6:次周期开盘
	7:次周期最高
	8:次周期最低
	9:次周期收盘
	*/
	COLORREF nColorPoint[4];		//进入点颜色

	BYTE bLose;	//使用最大利润损失率
	BYTE bWin;	//使用最大利润盈利率
	BYTE bMaxDays;///使用最大天数
	BYTE bSubstractFee;//使用扣除手续费
	BYTE bAddPreProfit;//加入前一次的盈利进行计算
	BYTE bUseInitFinace;//使用初始资金
	BYTE bRes1;
	BYTE bRes2;

	float fMaxLose;//最大利润损失率
	float fMaxWin;//最大利润盈利率
	float fMaxDays;//最大天数
	float fInitFinace;//初始资金数
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
		fMaxLose = -0.2f;//最大利润损失率
		fMaxWin = 0.5;//最大利润盈利率
		fMaxDays = 20;//最大天数
		fInitFinace = 10000;//初始资金数
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
				float		max[8];	//最大值
				float		min[8];	//	最小值
				float		defaultVal[8];	//缺省值
//				float		defaultValArray[11][8];	//缺省值
				//0:HS 1:MIN1 2:MIN5 3:MIN15 4:MIN30 5:MIN60 6:DAY (MANYDAY ) 7:WEEK
				//8:MONTH 9:YEAR 

				float		stepLen[8];	//步长
				float		posX[8];		//Y坐标轴的横线的数值
				float		fReserved;		//

				int			nKindPeriod;		//
				//0:HS 1:MIN1 2:MIN5 3:MIN15 4:MIN30 5:MIN60 6:DAY  7:WEEK
				//8:MONTH 9:YEAR 10:MANYDAY 
				BYTE	nPeriodsUsed[16];	//有效周期

				BOOL	isSystem;		//是否是系统指标
				BOOL	isOfen;		//是否是常用指标
				BOOL	isProtected;		//是否密码保护

				BYTE	isMainFiguer;	//	是否是主图叠加（0为副图，1为主图，2为不可用）
				BYTE	numPara;	//参数个数(最多为8个参数)
				BYTE 	posFlag;		//Y坐标轴的横线的式样（0为自动，1为定制（可有7条），2为不可用）
				BYTE 	isAdditionalCond;		//是否含有附加条件

				BYTE	bYHParam[8];//是否优化参数
				BYTE	bYH;//是否自动优化参数
				BYTE	bRightBox;//是否是右边的指标

				//是否是来自服务器的指标，如果nIsFromServer = 33134999是来自服务器的指标
				int		nIsFromServer;//

			};
			BYTE btMemData[LEN_BYTE];
		};
		ADDITIONAL_BUYSELL *		pAdditionalBS;
		CArray<FLOAT11,FLOAT11&>	defaultValArray;	

		CString		name;		//技术分析指标名字(最多9个字符)
		CString		password;	//密码
		CString		explainBrief;	//	指标说明
		CString		explainParam;	//	参数说明

		CString		namePara[8];	//参数名字（最多15个字符)
		CString		fomular;		//公式
		CString		help;		//帮助注释

		CString		buyStr;       //买入的条件
		CString		sellStr;       //卖出的条件
		CString		buyStrKong;       //买入的条件
		CString		sellStrKong;       //卖出的条件
		// add ,3.13
		CString		subKindIndex;		//技术分析指标种类,缺省值是 ""
		CString		subKindIndexTime;		//技术分析指标种类
		CString		strReserved;		//

		static bool m_bVer20;
		static CStringArray m_strArrayKind[4];
		float		defaultValYH[8];	//优化缺省值


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
