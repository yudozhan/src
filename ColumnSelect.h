#if !defined(AFX_COLUMNSELECT_H__C0089EA1_E150_11D3_920B_00400552E583__INCLUDED_)
#define AFX_COLUMNSELECT_H__C0089EA1_E150_11D3_920B_00400552E583__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ColumnSelect.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CColumnSelect dialog
//"���ļ�",
const CString g_sBuySell[] = {"��һ��","�����","������","��һ��","�����","������","��һ��","������","������","��һ��","������","������"
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

,ID_FIRSTSTK//ƽ���Ƿ�      
,ID_RSTKRATE//Ȩ�Ƿ�

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

#define ID_ROWNUM     5000  //�к�
#define ID_STOCKID    5001  //����
#define ID_STOCKNAME  5002  //����
#define ID_YSTC       5003  //����
#define ID_OPEN       5004  //��
#define ID_HIGH       5005  //���
#define ID_LOW        5006  //���
#define ID_NEW        5007  //����
#define ID_TOTV       5008  //����
#define ID_NOWV       5009  //����
#define ID_TOTP       5010  //�ܶ�
#define ID_AVGP       5011  //����
#define ID_ADDE       5012  //�ǵ�
#define ID_HLP        5013  //���
#define ID_RDR        5014  //�Ƿ�
#define ID_BSSPBAS    5015  //ί��
#define ID_DIFBS      5016  //ί��
#define ID_VOLBI      5017  //����
#define ID_ACCB       5018  //ί��
#define ID_ACCS       5019  //ί��
#define ID_BUY        5020  //ί���
#define ID_SEL        5021  //ί����
#define ID_RVOL       5022  //����
#define ID_DVOL       5023  //����
#define ID_EXCHANGE   5024  //������
#define ID_SYL        5025  //��ӯ��

#define ID_ZGB        5026//�ܹɱ�(���)
#define ID_GJG        5027//���ҹ�(���)
#define ID_FQRFRG     5028//�����˷��˹�(���)
#define ID_FRG        5029//���˹�(���)
#define ID_ZGG        5030//ְ����(���)
#define ID_GZAG       5031//����A��(���)
#define ID_LTAG       5032//��ͨA��(���)
#define ID_HG         5033//�ȹ�(���)
#define ID_BG         5034//B��(���)
#define ID_ZPG        5035//ת���(���)

#define ID_ZZC        5036//���ʲ�(��Ԫ)
#define ID_LDZC       5037//�����ʲ�(��Ԫ)
#define ID_CQTZ       5038//����Ͷ��(��Ԫ)
#define ID_GDZC       5039//�̶��ʲ�(��Ԫ)
#define ID_WXZC       5040//�����ʲ�(��Ԫ)
#define ID_LDFZ       5041//������ծ(��Ԫ)
#define ID_CQFZ       5042//���ڸ�ծ(��Ԫ)
#define ID_GGQY       5043//�ɶ�Ȩ��(��Ԫ)
#define ID_ZBGJJ      5044//�ʱ�������(��Ԫ)
#define ID_YYGJJ      5045//ӯ�๫����(��Ԫ)
#define ID_MGJZ       5046//ÿ�ɾ�ֵ(Ԫ)
#define ID_GDQYBL     5047//�ɶ�Ȩ�����(%)
#define ID_MGGJJ      5048//ÿ�ɹ�����(Ԫ)

#define ID_ZYYWSR     5049//��Ӫҵ������(��Ԫ)
#define ID_ZYYWLR     5050//��Ӫҵ������(��Ԫ)
#define ID_QTYWLR     5051//����ҵ������(��Ԫ)
#define ID_LRZE       5052//�����ܶ�(��Ԫ)
#define ID_JLR        5053//������(��Ԫ)
#define ID_WFPLR      5054//δ��������(��Ԫ)
#define ID_MGSY       5055//ÿ������(Ԫ)
#define ID_JZCSYL     5056//���ʲ�������(%)
#define ID_MGWFPLR    5057//ÿ��δ��������(Ԫ)
#define ID_MGJZC      5058//ÿ�ɾ��ʲ�(Ԫ)


#define ID_ZJLS       5059 //�ʽ�(%d��)
#define ID_VOLBIFM    5060 //����(%d��)
#define ID_HLPFM      5061 //���(%d��)
#define ID_EXCHANGEFM 5062 //������(%d��)
#define ID_ADVSP      5063 //����(%d��)


#define ID_DELITEM    5064 //ɾ������
#define ID_DEFAULT    5065 //�ָ�����
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

	int m_nPos_x;               //����X����λ��
	int m_nPos_y;               //����Y����λ��
	UINT m_nSelectID;          //����ѡ��ID��
	CString m_sSelectStr;      //����ѡ����ָ������
	BOOL  m_bIsTechnique;        //�Ƿ�Ϊ����ָ��
	m_DynaTechniqueDef m_DynaTechnique; //

	BOOL m_bIsDelete;           //�Ƿ�ɾ��
	BOOL m_bIsFirst;            //�Ƿ��һ�δ�
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
