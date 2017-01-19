// TechDlgEditScreenStock.cpp : implementation file
//

#include "stdafx.h"
#include "WH.h"
#include "CTaiKlineDlgEditScreenStock.h"
#include "CTaiShanDoc.h"
#include "MainFrm.h"
#include "CFormularCompute.h"
#include "DialogEditZBGSHS.h"
#include "CFormularContent.h"
#include "CTaiScreenParent.h"
#include "CTaiKlineDlgCanUsedPeriod.h"
#include "CTaiKlineDlgParamExplain.h"
#include "DIALOGXZZB.h"
#include "DIAGSGLMM.h"
#include "CTaiKlineIndicatorsManage.h"
#include "CTaiScreenParent.h"
#include "CTaiKlineDlgChangeIndexParam.h"
#include "KEYBRODEANGEL.h"
#include "DIALOGGSSM.h"
#include "CwdSelectFormu.h"
#include "CwdShowFormu.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTaiKlineDlgEditScreenStock dialog

bool g_bInitDlgEdit = false;

/////////////////////////////////////////////////////////////////////////////
// CTaiKlineDlgEditScreenStock message handlers
CTaiKlineDlgEditScreenStock::CTaiKlineDlgEditScreenStock(int nID,CWnd* pParent)
	: CDialog(nID, pParent)
{
	m_bShowFromMouse = false;
	m_nKlineType = -1;
	m_nTabPre = 0;
	m_maxChars = 30;
	jishunow = NULL;
	m_bInit = false;//CFloatEdit
	//{{AFX_DATA_INIT(CTaiKlineDlgEditScreenStock)
	m_checkmm = FALSE;
	m_checkOften = FALSE;
	m_explainbrief = _T("");
	m_myhelp = _T("");
	m_name = _T("");
	m_password = _T("");
	m_xline = _T("");
	m_sTestInfo = _T("");
	m_sKindTime = _T("����");
	m_sKindFormu = _T("");
	//}}AFX_DATA_INIT
	for(int i=0;i<8;i++)
	{
		for(int j=0;j<4;j++)
			m_editParam[i][j] = _T("");
	}
	m_strPeriodPre ="����";
	memset(m_nPeriodsUsed,1,16);

	m_strParamExplain = "";

	m_nID[0][0] = IDC_EDIT11;
	m_nID[0][1] = IDC_EDIT12;
	m_nID[0][2] = IDC_EDIT13;
	m_nID[0][3] = IDC_EDIT14;

	m_nID[1][0] = IDC_EDIT21;
	m_nID[1][1] = IDC_EDIT22;
	m_nID[1][2] = IDC_EDIT23;
	m_nID[1][3] = IDC_EDIT24;

	m_nID[2][0] = IDC_EDIT31;
	m_nID[2][1] = IDC_EDIT32;
	m_nID[2][2] = IDC_EDIT33;
	m_nID[2][3] = IDC_EDIT34;

	m_nID[3][0] = IDC_EDIT41;
	m_nID[3][1] = IDC_EDIT42;
	m_nID[3][2] = IDC_EDIT43;
	m_nID[3][3] = IDC_EDIT44;

	AfxInitRichEdit( );

}
CTaiKlineDlgEditScreenStock::~CTaiKlineDlgEditScreenStock()
{
}

void CTaiKlineDlgEditScreenStock::DoDataExchange(CDataExchange* pDX)
{

	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTaiKlineDlgEditScreenStock)
	DDX_Control(pDX, IDOK, m_ok);
	DDX_Control(pDX, IDCANCEL, m_cancel);
	DDX_Control(pDX, IDC_DYHS, m_dyhs);
	DDX_Control(pDX, IDC_CSGS, m_csgs);
	DDX_Control(pDX, IDC_BUTTON2, m_button2);
	DDX_Control(pDX, IDC_BUTTON1, m_button1);
	DDX_Control(pDX, IDC_BUTTON_CAN_USED, m_canused);
	DDX_Control(pDX, IDC_EDIT12, m_floatEdit12);
	DDX_Control(pDX, IDC_EDIT13, m_floatEdit13);
	DDX_Control(pDX, IDC_EDIT14, m_floatEdit14);
	DDX_Control(pDX, IDC_EDIT22, m_floatEdit22);
	DDX_Control(pDX, IDC_EDIT23, m_floatEdit23);
	DDX_Control(pDX, IDC_EDIT24, m_floatEdit24);
	DDX_Control(pDX, IDC_EDIT32, m_floatEdit32);
	DDX_Control(pDX, IDC_EDIT33, m_floatEdit33);
	DDX_Control(pDX, IDC_EDIT34, m_floatEdit34);
	DDX_Control(pDX, IDC_EDIT42, m_floatEdit42);
	DDX_Control(pDX, IDC_EDIT43, m_floatEdit43);
	DDX_Control(pDX, IDC_EDIT44, m_floatEdit44);
	DDX_Control(pDX, IDC_TAB1, m_tabParam);
	DDX_Control(pDX, IDC_NAME, m_namecor);
	DDX_Control(pDX, IDC_PASSWORD, m_passwordcor);
	DDX_Control(pDX, IDC_HFQS, m_hfqs);
	DDX_Control(pDX, IDC_INPUT, m_inputcor);
	DDX_Check(pDX, IDC_CHECKMM, m_checkmm);
	DDX_Check(pDX, IDC_CHECK1, m_checkOften);
	DDX_Text(pDX, IDC_EXPLAINBRIEF, m_explainbrief);
	DDX_Text(pDX, IDC_MYHELP, m_myhelp);
	DDX_Text(pDX, IDC_NAME, m_name);
	DDX_Text(pDX, IDC_PASSWORD, m_password);
	DDX_Control(pDX, IDC_XLINE, m_xlinecor);
	DDX_Text(pDX, IDC_XLINE, m_xline);
	DDX_Text(pDX, IDC_STATIC_TEST_INFO, m_sTestInfo);
	DDX_CBString(pDX, IDC_COMBO_KIND_TIME, m_sKindTime);
	DDX_Text(pDX, IDC_EDIT1, m_sKindFormu);
	//}}AFX_DATA_MAP
	if(this->iskline == 2) DDV_MaxChars(pDX, m_name, 10);
}


BEGIN_MESSAGE_MAP(CTaiKlineDlgEditScreenStock, CDialog)
	//{{AFX_MSG_MAP(CTaiKlineDlgEditScreenStock)
	ON_BN_CLICKED(IDC_HFQS, OnRestoreDefault)
	ON_BN_CLICKED(IDC_CSGS, OnTestFormula)
	ON_BN_CLICKED(IDC_DYHS, OnAddFunc)
	ON_BN_CLICKED(IDC_BUTTON2, OnImportFormula)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_CHECKMM, OnCheckmm)
	ON_WM_CTLCOLOR()
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, OnSelchangeTab1)
	ON_BN_CLICKED(IDC_BUTTON_CAN_USED, OnButtonCanUsed)
	ON_BN_CLICKED(IDC_BUTTON1, OnButton1)
	ON_CBN_SELCHANGE(IDC_COMBO_KIND_TIME, OnSelchangeComboKindTime)
	ON_WM_HELPINFO()
	ON_EN_CHANGE(IDC_INPUT, OnChangeInput)
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTaiKlineDlgEditScreenStock message handlers

///////////////////////////////////////////////////////////////////
//      ����˵�������Թ�ʽ
//      ��ڲ�������
//      ���ز�������
/////////////////////////////////////////////////////////////////////////////
void CTaiKlineDlgEditScreenStock::OnTestFormula() 
{
  bool errpram=false;
  UpdateData();
  CString input=""		;
  m_inputcor.GetWindowText(input);

//  m_inputcor.SetRedraw(FALSE);
	
 	int i,j;
	for(int flag=0;flag<1;flag++)
	{
		//m_edit ����ֵ���浽 m_editParam
		int n = m_tabParam.GetCurSel();
		if(n==0)
		{//�Ѳ�����Ϣ(1-4)��m_edit�лָ���m_editParam
			for(i=0;i<4;i++)
				for(j=0;j<4;j++)
					m_editParam[i][j]=GetStringParam(i,j);//m_edit[i][j];
		}
		else //�Ѳ�����Ϣ(5-8)��m_edit�лָ���m_editParam
		{
			for(i=0;i<4;i++)
				for(j=0;j<4;j++)
					m_editParam[i+4][j]=GetStringParam(i,j);//m_edit[i][j];
//					SetStringParam(i,j,m_editParam[i+4][j]);//=m_edit[i][j];
		}

		//�������Ϸ���
		for(i=0;i<8;i++)
		{
			bool bEmpty = true;
			for(j=1;j<4;j++)
			{
				if(m_editParam[i][j] != "")
				{
					bEmpty = false;
					break;
				}
			}

			//test name
			if(bEmpty == false)
			{
				  if(m_editParam[i][0]=="")
				  {
					  errorposition=2000+(i+1)*10+1;
					  errormessage="���������ܿ�";
					  errpram=true;
					  break;
				  }
				  else if(!CheckParmNameInput(m_editParam[i][0],errormessage))
				  {
					  errorposition=2000+(i+1)*10+1;
					  errpram=true;
					  break;
				  }
			}
			else
			{
				m_editParam[i][0]=="";
				for(int k=i+1;k<8;k++)
				{
					for(int l=0;l<4;l++)
						m_editParam[k][l]=="";
				}

				break;
			}

			//test param
			for(j=1;j<4;j++)
			{
				if(m_editParam[i][j] == "")
				{
					errorposition=2000+(i+1)*10+1+j;
					errormessage="����ֵ���ܿ�";
					errpram=true;
					break;
				}
				else
				{
					  if(!CheckParmInput(m_editParam[i][j]))
					  {
						  errorposition=2000+(i+1)*10+1+j;
						  errormessage="����ֵ���Ϸ�";
						  errpram=true;
						  break;
					  }
				}
				//
				if(j==3)
				{
					if(StringToFloat(m_editParam[i][3])>StringToFloat(m_editParam[i][1])||StringToFloat(m_editParam[i][3])<StringToFloat(m_editParam[i][2]))
					{
						  errorposition=2014;
						  errormessage="ȱʡֵ���ںϷ��ķ�Χ";
						  errpram=true;
						  break;
				   }
				}

			}

		}

	}
//------------------------��XLINE���������ݻ��ɺϷ��ĸ�ʽ
	if(iskline>1)
	{
	   m_xline.Remove(' ');
	  int strstart=0;
	  CString  xpos;
	  int j=0;
	  for(int i=0;i<m_xline.GetLength();i++)
	  {
		  char test=m_xline.GetAt(i);
		  bool isreturn=false;             //�Ƿ��ǻس����ı�־
		  bool isfinish=(test==';');       //�Ƿ��ǣ����ı�־
		  bool isdot=(test=='.');           //�Ƿ���.���ı�־  
		  if(test==13&&m_xline.GetAt(i+1)==10)
		  {
			  isreturn=true;
			  
		  }
		  
		  if(!(isdot||isreturn||isfinish||isdot||(test>='0'&&test<='9'||test=='-')))
		  {
			  errormessage="ָ��ˮƽλ�����ô���";
			  errorposition=3000+i;       //XLINE����Ĵ����+3000
			  errpram=true;
			  break;
		  }
		  if(isfinish||isreturn||i==m_xline.GetLength()-1)
		  {
			  xpos=m_xline.Mid(strstart,i-strstart);
			  if(xpos!="")
			  {
				  if(j<7)
				  {
	//				jishunow->posX[j]=atof(xpos);
					j++;
					for(int k=i+1;k<m_xline.GetLength();k++)
					{
						char test=m_xline.GetAt(k);
						if(test>='0'&&test<='9')
						{
							strstart=k;
							break;
						}

					}
				  }
				  else
				  {
					  errorposition=3000+i;
					  errormessage="ˮƽ��λ��̫��";
					  errpram=true;
					  break;
				  }
			  }

		  }
		  if(isreturn)
			  i++;

	  }
	}//end XLINE

  CString formal=input;
  formal.Remove(' ');
  if(formal.GetLength()==0)
  {
	  errormessage="��ʽ���ݲ���Ϊ��";
	  m_inputcor.SetFocus();
//	  m_inputcor.SetRedraw();
	  UpdateData(FALSE);
	  return;

  }
  else
  {
	  formal=input;
  }
  if(formal.GetAt(formal.GetLength()-1)==';')
	  formal.Delete(formal.GetLength()-1);
  formal.MakeLower();
  Kline kline;
 CFormularCompute *equation=new CFormularCompute(0,0,&kline,formal);

  for(i=0;i<8;i++)
  {
	  if(m_editParam[i][0]!=""&&m_editParam[i][3]!="")
	  {  
		  float fpram=StringToFloat(m_editParam[i][3]);
		  m_editParam[i][0].MakeLower();
		  int addpra=equation->AddPara(m_editParam[i][0],fpram);
		  switch(addpra)
		  {
		  case(1):
			  errorposition=2011;
			  errpram=true;
			  errormessage="��������̫��";
			  break;

		  case(2):
			  errorposition=2011;
			  errpram=true;
			  errormessage="�������ظ�����ϵͳ�ؼ���";
			  break;

		  }
		  if(errpram == true)
			  break;
  
	  }
  }

  if(errpram)
  {
	  errorposition-=2000;         //�������Ĵ���Ż���EDIT�����
	  switch(errorposition)
	  {
	  case 11:
		  GetDlgItem(IDC_EDIT11)->SetFocus();
		  break;
	  case 12:
		  GetDlgItem(IDC_EDIT12)->SetFocus();
		  break;
	  case 13:
		  GetDlgItem(IDC_EDIT13)->SetFocus();
		  break;
	  case 14:
		  GetDlgItem(IDC_EDIT14)->SetFocus();
		  break;

	  case 21:
		  GetDlgItem(IDC_EDIT21)->SetFocus();
		  break;
	  case 22:
		  GetDlgItem(IDC_EDIT22)->SetFocus();
		  break;
	  case 23:
		  GetDlgItem(IDC_EDIT23)->SetFocus();
		  break;
	  case 24:
		  GetDlgItem(IDC_EDIT24)->SetFocus();
		  break;

	  case 31:
		  GetDlgItem(IDC_EDIT31)->SetFocus();
		  break;
	  case 32:
		  GetDlgItem(IDC_EDIT32)->SetFocus();
		  break;
	  case 33:
		  GetDlgItem(IDC_EDIT33)->SetFocus();
		  break;
	  case 34:
		  GetDlgItem(IDC_EDIT34)->SetFocus();
		  break;

	  case 41:
		  GetDlgItem(IDC_EDIT41)->SetFocus();
		  break;
	  case 42:
		  GetDlgItem(IDC_EDIT42)->SetFocus();
		  break;
	  case 43:
		  GetDlgItem(IDC_EDIT43)->SetFocus();
		  break;
	  case 44:
		  GetDlgItem(IDC_EDIT44)->SetFocus();
		  break;
	  default:
		  if(iskline>1)//����ָ��
		  {
			  m_xlinecor.SetFocus();
			  m_xlinecor.SetSel(errorposition-1000,errorposition-1000);
		  }
		  break;
	  }
  		if(errorposition==-1)
			m_sTestInfo="����ͨ��!";
		else
			m_sTestInfo=errormessage;
	  UpdateData(FALSE);

	  return;
  }

equation->Devide();
  if(iskline<2)
  {
	  if(equation->m_RlineNum!=1)
	  {
		  if(equation->m_nErrPos==-1)
		  {
			  
			  if(equation->m_RlineNum==0)
			  {
				  equation->m_strErrMsg="��ʽ������һ������ֵ";
				  equation->m_nErrPos=formal.GetLength();

			  }
			  else
			  {
				  int k=0,j=0;
				  int formallength=formal.GetLength();
				  char test;
				  for(int i=formallength-1;i>=0;i--)
				  {
					  test=formal.GetAt(i);
					  if(test==' '||test==';'||test=='\t'||test=='\n'||test=='\r')
						  continue;
					  else
						  break;
				  }
				  for( j=i+1;j<formal.GetLength();j++)
					  formal.Delete(j);
  
				  formal.Insert(i+1,';');
				  for( i=0;i<formal.GetLength();i++)
				  {
					  char test=formal.GetAt(i);
					  if(test==':'&&formal.GetAt(i+1)=='=')
					  {
						  k++;
					  }
					  if(test==';')
					  {
						  k--;
						  if(k<-1)
							  break;
						  else
							  j=i;

					  }
					  
				  }
				  if(equation->m_nErrPos==-1&&k<-1)
				  {
					  equation->m_nErrPos=j;
					  equation->m_strErrMsg="��ʽֻ����һ�����ʽ";

				  }
			  }
		  }
	  }
	  errorposition=equation->m_nErrPos;
	  errormessage=equation->m_strErrMsg;
  }
  else
  {
	  errorposition=equation->m_nErrPos;
	  errormessage=equation->m_strErrMsg;
	  if(errorposition==-1)
	  {
		  if(equation->m_RlineNum==0)
		  {
			  errorposition=formal.GetLength();
			  errormessage="��ʽ�������ٻ�һ����";
		  }
		  else if(equation->m_RlineNum>NUM_LINE_TOT)
		  {
			  errorposition=formal.GetLength();
			  errormessage="��ʽ���ֻ�ܼ���32����";
		  }


	  }
  }
  delete equation;
//     return;
  if(errorposition!=-1)
  {
	 m_inputcor.SetSel(errorposition,errorposition);
	 long lng = m_inputcor.LineFromChar(errorposition);
	 if(lng>=0)
	 {
		 int nln = m_inputcor.GetFirstVisibleLine();
		 if(nln>lng)
			 m_inputcor.LineScroll(lng-nln ,  0 );
		 else if(nln<lng)
		 {
			 m_inputcor.LineScroll(lng-nln-1 ,  0 );
		 }

	 }
	}
 
  	if(errorposition==-1)
		m_sTestInfo="����ͨ��!";
	else
		m_sTestInfo=errormessage;

//  m_inputcor.SetRedraw();
//	OnChangeInput2();
	m_inputcor.SetFocus();

  UpdateData(FALSE);

}


//---------------------�����û��Թ�ʽ���޸�
void CTaiKlineDlgEditScreenStock::OnOK() 
{
    UpdateData();
	if(jishunow)
	{
		if(jishunow->sellStr && jishunow->sellStr != "")
		{
			return;
		}
	}

	CTaiShanDoc*	pDoc=CMainFrame::m_pDoc ;//(CTaiShanDoc*)pchildfram->GetActiveDocument();
	
	CString name=m_name;
	CString input="";
	m_inputcor.GetWindowText(input);
	name.Remove(' ');
//	input.Remove(' ');
	bool isnameok=true;
	
	int i;
	for(i=0;i<m_name.GetLength();i++)
	{
		char test=m_name.GetAt(i);
		if((test&0x80)!=0)
		{
			i=i+1;
			continue;
		}
//		if(!((test>='a'&&test<='z')||test=='#'||test=='_'||test=='%'||test == '(' || test == ')'||(test>='A'&&test<='Z')||(test>='0'&&test<='9')))
		{
//			isnameok=false;
//			break;
		}
//		if(i==0&&test>='0'&&test<='9')
		{
//			isnameok=false;
//			break;
		}

	}
	if(!isnameok||name==""||name=="STOCKMODEKLINE"||name=="STOCKFILTER")
	{
		AfxMessageBox("ָ����Ϊ�ջ�Ƿ���\n����������",MB_OK);
		m_namecor.SetFocus();
		return;
	}

	OnTestFormula();

	
//-----------------�����ʽû��ͨ���﷨��飬�����û���Ҫ����		
	if(errorposition!=-1)
		if(AfxMessageBox("���༭�Ĺ�ʽû��ͨ��\n�﷨��飬ϵͳ��ǿ�ƴ洢��\n\Ҫ������",MB_OKCANCEL|MB_ICONQUESTION)==IDCANCEL)
			return;

	CFormularContent  *jishunew = NULL;
	if(item == -1)
	{
		jishunew=new CFormularContent();
		jishunow=jishunew;
	}
	else
	{
		if(iskline == 1)
			jishunow =pDoc->m_formuar_kline.GetAt(item);
		else if(iskline == 0)
			jishunow =pDoc->m_formuar_choose.GetAt(item);
		else//����ָ�깫ʽ
		{
			jishunow =pDoc->m_formuar_index.GetAt(item);
			CString namepre=jishunow->name;
			CTaiKeyBoardAngelDlg::DeleteIndicatorName(namepre);
		}

		int nKind = 0;
		if(iskline == 1)
			nKind =1;
		else if(iskline == 0)
			nKind =2;
		if(FindIndicator(nKind, m_name,jishunow))
		{
			AfxMessageBox(m_name+" ��ʽ�Ѵ��ڣ�������ѡ��!");
			return;
		}
	}
	
	int k;
	if(item == -1)
	{
		if(iskline == 1)      //��K��ģʽ��ʽ
		{
			int length=pDoc->m_formuar_kline.GetSize();
			if(length>0)
			{
				for(k=0;k<length;k++)
				{
					if(m_name.Compare(pDoc->m_formuar_kline.GetAt(k)->name)<=0)
						break;
				}
				if(k>=length)
				{
					pDoc->m_formuar_kline.Add(jishunew);
					addpos=length;
				}
				else
				{

					if(m_name.Compare(pDoc->m_formuar_kline.GetAt(k)->name)==0)
					{
						
						AfxMessageBox(m_name+"K����Ϲ�ʽ�Ѵ��ڣ�������ѡ��!");
						m_namecor.SetFocus();
						delete jishunew;
						return ;
					}
					else
					{
						pDoc->m_formuar_kline.InsertAt(k,jishunew);
						addpos=k;
					}
				}
			}
			else
			{
				addpos=0;
				pDoc->m_formuar_kline.Add(jishunew);
			}
		}
			
		else  if(iskline == 0)//������ѡ�ɹ�ʽ
		{
			int length=pDoc->m_formuar_choose.GetSize();
			if(length>0)
			{
				for(int k=0;k<length;k++)
				{
					if(m_name.Compare(pDoc->m_formuar_choose.GetAt(k)->name)<=0)
						break;
				}
				if(k>=length)
				{
					pDoc->m_formuar_choose.Add(jishunew);
					addpos=length;
				}
				else
				{
					if(m_name.Compare(pDoc->m_formuar_choose.GetAt(k)->name)==0)
					{
						AfxMessageBox(m_name+"����ѡ�ɹ�ʽ�Ѵ��ڣ�������ѡ��!");
						m_namecor.SetFocus();
						delete jishunew;
						return ;
					}
					else
					{
					
						pDoc->m_formuar_choose.InsertAt(k,jishunew);
						addpos=k;
					}
				}
			}
			else
			{
				addpos=0;
				pDoc->m_formuar_choose.Add(jishunew);
			}
		}
		else//����ָ�깫ʽ
		{
			int length=pDoc->m_formuar_index.GetSize();
			if(length>0)
			{
				int k = 0;
				for(k=0;k<length;k++)
				{
					if(m_name.Compare(pDoc->m_formuar_index.GetAt(k)->name)<=0)
						break;
				}
				if(k>=length)
				{
					pDoc->m_formuar_index.Add(jishunew);
					addpos=length;
				}
				else
				{

					if(m_name.Compare(pDoc->m_formuar_index.GetAt(k)->name)==0)
					{
						AfxMessageBox(m_name+"ָ�깫ʽ�Ѵ��ڣ�������ѡ��!");
						delete jishunew;
						return ;
					}
					else
					{
						pDoc->m_formuar_index.InsertAt(k,jishunew);
						addpos=k;
					}
				}
			}
			else
			{
				pDoc->m_formuar_index.Add(jishunew);
				addpos=0;
			}
		}
	}
			
	if(iskline == 2)
	{
	//----------------------------��������λ��д��ṹ
		for(int k=0;k<7;k++)
		{
			jishunow->posX[k]=-10e20;
		} 
		CString xpos;

		int j=0,strstart=0;
		for( i=0;i<m_xline.GetLength();i++)
		{
		  char test=m_xline.GetAt(i);
		  bool isreturn=false;             //�Ƿ��ǻس����ı�־
		  bool isfinish=(test==';');       //�Ƿ��ǣ����ı�־
		  if(test==13&&m_xline.GetAt(i+1)==10)
		  {
			  isreturn=true;
			  
		  }
		  
		  if(isfinish||isreturn||i==m_xline.GetLength()-1)
		  {
			  xpos=m_xline.Mid(strstart,i-strstart);
			  if(xpos!="")
			  {
				 jishunow->posX[j]=atof(xpos);
				 j++;
				for(int k=i+1;k<m_xline.GetLength();k++)
				{
					char test=m_xline.GetAt(k);
					if(test>='0'&&test<='9'||test == '-')
					{
						strstart=k;
						break;
					}
				}
				  
			  }

		  }
		  if(isreturn)
			  i++;
		}
//-------------------------����ͼ������Ϣд��ṹ
		int itemid;
		itemid=GetCheckedRadioButton(IDC_ZTDJ,IDC_FT);
		if(itemid==IDC_ZTDJ)
			jishunow->isMainFiguer=true;
		else
			jishunow->isMainFiguer=false;
	}
	
	
		
//-------------------���û��趨�Ĳ�������д��ṹ
	int pramnum=0;
	for(i=0;i<8;i++)
	{
		if(m_editParam[i][0]!=""&&m_editParam[i][3]!="")
			pramnum++;
		else
			break;
	}
	jishunow->numPara=pramnum;
	for(i=0;i<pramnum;i++)
	{
		jishunow->namePara[i]=m_editParam[i][0];
		jishunow->namePara[i].MakeLower ();
		jishunow->max[i]=StringToFloat(m_editParam[i][1]);
		jishunow->min[i]=StringToFloat(m_editParam[i][2]);

		float f = StringToFloat(m_editParam[i][3]);
		if(m_nKlineType!=-1)
			CTaiKlineDlgChangeIndexParam::SetParamLjishu(jishunow,m_nKlineType,i,(CMainFrame::m_pDoc)->m_propertyInitiate.bSaveParam,f);
		else
			jishunow->defaultVal[i]=f;
	}	

	 //temp initiate
	 jishunow->InitDefaultValArray();

//-------------------------��˵������ʽ������д��ṹ
	 jishunow->fomular=input;
	 if((m_checkmm==TRUE))
		 jishunow->password=m_password;
	 jishunow->explainBrief=m_explainbrief;
	 jishunow->name=m_name;
	 jishunow->isProtected=m_checkmm;
	 jishunow->help=m_myhelp;
	 if(m_sKindTime!="����"&&m_sKindTime!="����"&&m_sKindTime!="����")
		 m_sKindTime = "����";                  
	 jishunow->subKindIndexTime = m_sKindTime;
	 memcpy(jishunow->nPeriodsUsed , m_nPeriodsUsed,11);
//	 CComboBox* pCombo = (CComboBox* )GetDlgItem(IDC_COMBO_KIND_TIME)->GetWindowText();
	 jishunow->explainParam = m_strParamExplain ;
	 jishunow->subKindIndex = m_sKindFormu;
	 jishunow->isOfen  = m_checkOften;

	 if(pramnum!=jishunow->defaultValArray.GetSize())
		jishunow->AddDefaultValToArray();

	 CFormularCompute::FanEach(jishunow);

	 CDialog::OnOK();
}

///////////////////////////////////////////////////////////////////
//      ����˵�������Ӻ���
//      ��ڲ�������
//      ���ز�������
/////////////////////////////////////////////////////////////////////////////
void CTaiKlineDlgEditScreenStock::OnAddFunc() 
{
	UpdateData();
	CDialogEDITZBGSHS m_diazbgshs;
	if(m_diazbgshs.DoModal()==IDOK)
	{
		m_inputcor.SetFocus();
		functionstr=m_diazbgshs.m_selectedstr;
		int length=functionstr.GetLength();
		int i;
		for(i=0;i<length;i++)
		{
			char test=functionstr.GetAt(i);
			if(test=='-'&&functionstr.GetAt(i+1)=='-')
				break;
		}
		if(i==length)
			functionstr="";
		else
			functionstr=functionstr.Left(i);
		m_inputcor.ReplaceSel(functionstr);
	
	}

	
}

BOOL CTaiKlineDlgEditScreenStock::OnInitDialog() 
{
	CDialog::OnInitDialog();

	CTaiShanDoc*	pDoc=(CMainFrame::m_pDoc );

	if(m_nKlineType!=-1)
	{
		CString sTl ;
		GetWindowText(sTl);
		CString s2 = CTaiScreenParent::GetPeriodName(m_nKlineType);
		sTl+="��";
		sTl+=s2;
		sTl+="��";
		SetWindowText(sTl);
	}

	if(m_bInit == false)
	{
		m_tabParam.InsertItem (0,"����1-4");
		m_tabParam.InsertItem (1,"����5-8");
		m_bInit = true;
	}

	if(item!=-1)
	{
//		GetDlgItem(IDC_BUTTON2)->EnableWindow(FALSE);
	}

	if(iskline<2)                      //������ѡ�ɹ�ʽ
	{
		if(iskline==0)
			this->SetWindowText("����ѡ�ɹ�ʽ�༭��");
		else
			this->SetWindowText("K����Ϲ�ʽ�༭��");
		
		if(item!=-1)
		{
			if(iskline==0)
				jishunow=pDoc->m_formuar_choose.GetAt(item);
			if(iskline==1)
				jishunow=pDoc->m_formuar_kline.GetAt(item);
			m_name=jishunow->name;
			m_myhelp=jishunow->help;
			m_checkOften = jishunow->isOfen ;
		//	m_namecor.SetReadOnly(TRUE);
			m_explainbrief=jishunow->explainBrief;
			if(jishunow->isProtected)
			{
				m_checkmm=true;
				m_password=jishunow->password;
			}
			else 
				m_passwordcor.EnableWindow(FALSE);

			 m_inputcor.SetWindowText(jishunow->fomular);
			 m_inputcor.SetFocus();
			 numPara = jishunow->numPara ;

			 //temp initiate
			 jishunow->InitDefaultValArray();

			 ParamIntoOther(0);
			 ParamIntoOther(2);


//			m_strPeriodPre ="";
			memcpy(m_nPeriodsUsed,jishunow->nPeriodsUsed,16);

			m_sKindTime=jishunow->subKindIndexTime  ;
			this->m_sKindFormu = jishunow->subKindIndex ;
			m_strParamExplain = jishunow->explainParam ;

			UpdateData(FALSE);
		}
		else
		{
			 m_checkmm=false;
			 m_hfqs.EnableWindow(FALSE);	 
			 UpdateData(FALSE);

		}
	}
	else//����ָ�깫ʽ
	{
		if(item!=-1)            //�������������ָ�깫ʽ
		{
			jishunow=pDoc->m_formuar_index.GetAt(item);
			m_name=jishunow->name;
	//		m_namecor.SetReadOnly(TRUE);
			if(pDoc->m_formuar_index.GetAt(item)->isMainFiguer==1)
				 CheckRadioButton(IDC_ZTDJ,IDC_FT,IDC_ZTDJ);
			else 
				 CheckRadioButton(IDC_ZTDJ,IDC_FT,IDC_FT);
			CString strxline;
			for(int k=0;k<7;k++)
			{
				if(jishunow->posX[k]>=-10e19)
				{
					CString test;
					test.Format("%.2f",jishunow->posX[k]);
					strxline+=test;
					strxline+=';';

				}
				else
					break;
			}
			m_xline=strxline;
			m_explainbrief=jishunow->explainBrief;
			if(jishunow->isProtected)
			{
				m_checkmm=true;
				m_password=jishunow->password;
			}
			else 
			{
				m_checkmm=false;
				m_passwordcor.EnableWindow(FALSE);
			}

			if(jishunow->isSystem)
				m_namecor.SetReadOnly(TRUE);


		//------------------------��ָ�깫ʽ�Ĳ�����ʼ��
			m_myhelp=jishunow->help;
			m_inputcor.SetWindowText(pDoc->m_formuar_index.GetAt(item)->fomular);
				m_inputcor.SetFocus();

			 //temp initiate
			 jishunow->InitDefaultValArray();
			m_checkOften = jishunow->isOfen ;

			ParamIntoOther(0);
			ParamIntoOther(2);

			memcpy(m_nPeriodsUsed,jishunow->nPeriodsUsed,16);
			m_sKindTime=jishunow->subKindIndexTime  ;
			this->m_sKindFormu = jishunow->subKindIndex ;
			m_strParamExplain = jishunow->explainParam ;

			UpdateData(FALSE);
		}
		else               //��������ָ�깫ʽ
		{
			m_myhelp="";
			 CheckRadioButton(IDC_ZTDJ,IDC_FT,IDC_FT);
			 m_checkmm=false;
			 m_hfqs.EnableWindow(FALSE);	 
			 UpdateData(FALSE);

		}

//		m_xlinecor.ShowWindow (SW_SHOW);
			;
	}

	if(jishunow)
		if(jishunow->sellStr != "")
		{
			m_inputcor.SetWindowText("");
		}


	g_bInitDlgEdit = false;
	OnChangeInput2() ;
	m_inputcor.SetEventMask(m_inputcor.GetEventMask() |
    ENM_CHANGE);
	g_bInitDlgEdit = true;

	DoMoveWindow();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

///////////////////////////////////////////////////////////////////
//      ����˵����������ת��Ϊ�ַ���
//      ��ڲ�����fdata��������
//      ���ز�����str:�����ַ���
/////////////////////////////////////////////////////////////////////////////
void CTaiKlineDlgEditScreenStock::ShowData(CString &str, float fdata)
{
	if(fdata==-1000)
		str="";
	else
	{
		str.Format("%.4f",fdata);
		int strlength=str.GetLength();
		str.TrimRight ("0");
		str.TrimRight (".");
	}
}

///////////////////////////////////////////////////////////////////
//      ����˵�����ַ���ת��Ϊ������
//      ��ڲ�����str:�ַ���
//      ���ز��������ظ�����
/////////////////////////////////////////////////////////////////////////////
float CTaiKlineDlgEditScreenStock::StringToFloat(CString str)
{
	char * test;
	int length=str.GetLength();
	test=str.GetBuffer(length);
	return (float)atof(test);
}

void CTaiKlineDlgEditScreenStock::OnPaint() 
{
	CDialog::OnPaint();
	
	// Do not call CDialog::OnPaint() for painting messages
}

///////////////////////////////////////////////////////////////////
//      ����˵�������Բ��������Ƿ�Ϸ�
//      ��ڲ�����parminput:��������
//      ���ز����������Ƿ���ȷ
/////////////////////////////////////////////////////////////////////////////
void CTaiKlineDlgEditScreenStock::OnCheckmm() 
{
	if(IsDlgButtonChecked(IDC_CHECKMM))
	   m_passwordcor.EnableWindow(TRUE);
   else
	   m_passwordcor.EnableWindow(FALSE);
	   

	
}

///////////////////////////////////////////////////////////////////
//      ����˵�������Բ��������Ƿ�Ϸ�
//      ��ڲ�����parminput:��������
//      ���ز����������Ƿ���ȷ
/////////////////////////////////////////////////////////////////////////////
BOOL CTaiKlineDlgEditScreenStock::CheckParmInput(CString parminput)
{
	int inputlength=parminput.GetLength();
	char test;
	for(int i=0;i<inputlength;i++)
	{
		
		test=parminput.GetAt(i);
		if(!(test=='-'||test=='+'||test=='.'||(test>='0'&&test<='9')))
		{
			return FALSE;
		}

	}
	return TRUE;

}

///////////////////////////////////////////////////////////////////
//      ����˵���������������Ƿ�Ϸ�
//      ��ڲ�����parname:�������ƣ�errmessage��������Ϣ
//      ���ز����������Ƿ���ȷ
/////////////////////////////////////////////////////////////////////////////
BOOL CTaiKlineDlgEditScreenStock::CheckParmNameInput(CString parname, CString &errmessage)
{
	parname.Remove(' ');
	char test=parname.GetAt(0);
	if(test&0x80 == 0 && (test<'A'||test>'z'||test=='_'))
	{
		errormessage="��������������ĸ��ͷ";
		return FALSE;
	}
	int length=parname.GetLength();
	for(int i=1;i<length;i++)
	{
		test=parname.GetAt(i);
		if((test>='0'&&test<='9')||(test>='A'&&test<='z')||test=='_'||((BYTE)test)>=0x80 )
			continue;
		else
		{
			errormessage="��������ɷǷ�";
			return FALSE;
		}
	}
	return TRUE;

}

///////////////////////////////////////////////////////////////////
//      ����˵�����ָ�ȱʡ����
//      ��ڲ�������
//      ���ز������޷�����
/////////////////////////////////////////////////////////////////////////////
void CTaiKlineDlgEditScreenStock::OnRestoreDefault() 
{
	// TODO: Add your control notification handler code here
	((CComboBox*)GetDlgItem(IDC_COMBO_KIND_TIME))->ResetContent( );

	OnInitDialog();
	
}

///////////////////////////////////////////////////////////////////
//      ����˵�������뼼��ָ�깫ʽ
//      ��ڲ�������
//      ���ز������޷�����
/////////////////////////////////////////////////////////////////////////////
void CTaiKlineDlgEditScreenStock::OnImportFormula() 
{
	// TODO: Add your control notification handler code here
	CFxjSelectFormu dlg;
	if(dlg.DoModal () == IDOK)
	{
		CFxjShowFormu dlg2 (this);
		dlg2.m_s =dlg.GetJishu() ->fomular ;

		if(dlg.GetJishu() ->sellStr != "")
			return;

		dlg2.m_title = dlg.GetJishu() ->name ;
		if(dlg2.DoModal () == IDOK)
		{
//			m_inputcor.Paste ();
		}

	}
	return;

	CDialogLOGXZZB mdiaxzzb(this);
	int item2 = -1;
	if(mdiaxzzb.DoModal() == IDOK)
	{
		CString s = mdiaxzzb.result;
		CTaiShanDoc* pDoc = CMainFrame::m_pDoc ;
		int n = pDoc->m_formuar_index .GetSize ();
		for(int i=0;i<n;i++)
		{
			if(s == pDoc->m_formuar_index.GetAt (i)->name )
			{
				item2 = i;
				break;
			}
		}
		if(item2==-1)
			return;

		//test
		if(pDoc->m_formuar_index.GetAt (item2)->isProtected !=0)
		{
			CDialogGSGLMM mdiagsglmm;
			mdiagsglmm.password=pDoc->m_formuar_index.GetAt (item2)->password;
			if(mdiagsglmm.DoModal()!=IDOK)
				return;
		}
		
		ASSERT(item==-1);
		item = item2;
		int temp = iskline;
		iskline = 2;

		CString s2 = this->m_sKindFormu ;
		((CComboBox*)GetDlgItem(IDC_COMBO_KIND_TIME))->ResetContent( );

		this->OnInitDialog ();
		this->m_sKindFormu = s2;
		UpdateData(FALSE);

		iskline = temp;
		item = -1;
	
	}
	
}


void CTaiKlineDlgEditScreenStock::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CDialog::OnCancel();
}



///////////////////////////////////////////////////////////////////
//      ����˵�����ı��ǩ���������ɫ
//      ��ڲ���������msdn�����ļ�
//      ���ز������޷�����
/////////////////////////////////////////////////////////////////////////////
HBRUSH CTaiKlineDlgEditScreenStock::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	// TODO: Change any attributes of the DC here
   if (pWnd->GetDlgCtrlID() == IDC_STATIC_TEST_INFO )
   {
	   if(errorposition==-1)
			pDC->SetTextColor(RGB(255,0,0));
		else
			pDC->SetTextColor(RGB(0,0,255));
   }

	
	// TODO: Return a different brush if the default is not desired
	return hbr;
}

///////////////////////////////////////////////////////////////////
//      ����˵��������1-4�����5-8֮����л�����
//      ��ڲ�������
//      ���ز������޷�����
/////////////////////////////////////////////////////////////////////////////
void CTaiKlineDlgEditScreenStock::OnSelchangeTab1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	//�Ѳ�����Ϣ���浽m_editParam��
	UpdateData();
//	static int nTabPre = 0;
	int n = m_tabParam.GetCurSel();
	int i,j;
	if(m_nTabPre!=n)
	{
		if(n==0)
		{//�Ѳ�����Ϣ(5-8)���浽m_editParam��,�Ѳ�����Ϣ(1-4)��m_editParam�лָ���m_edit
			for(i=0;i<4;i++)
				for(j=0;j<4;j++)
					m_editParam[4+i][j]=GetStringParam(i,j);//m_edit[i][j];
			for(i=0;i<4;i++)
				for(j=0;j<4;j++)
					SetStringParam(i,j,m_editParam[i][j]);
		}
		else if(n==1)//
		{
		  if(GetStringParam(0,0)==""||GetStringParam(1,0)==""||GetStringParam(2,0)==""||GetStringParam(3,0)=="")
		  {
			  m_tabParam.SetCurSel(0);
			  n=0;
		  }
		  else//�Ѳ�����Ϣ(1-4)���浽m_editParam��,�Ѳ�����Ϣ(5-8)��m_editParam�лָ���m_edit
		  {
			for(i=0;i<4;i++)
				for(j=0;j<4;j++)
					m_editParam[i][j]=GetStringParam(i,j);//m_edit[i][j];
			for(i=0;i<4;i++)
				for(j=0;j<4;j++)
					SetStringParam(i,j,m_editParam[i+4][j]);
		  }
		}
		UpdateData(FALSE);
	}
	m_nTabPre = n;

	*pResult = 0;
}

///////////////////////////////////////////////////////////////////
//      ����˵��������ת��
//      ��ڲ�����nFlag=0:ǰ�ĸ�����������༭��,nFlag=1:���ĸ�����������༭��,nFlag=2:���в������������
//      ���ز������޷�����
/////////////////////////////////////////////////////////////////////////////
void CTaiKlineDlgEditScreenStock::ParamIntoOther(int nFlag)//
{
	int i,j;
	if(nFlag >1)
	{
		if(jishunow->numPara>8)
			jishunow->numPara = 0;
		for(i=0;i<8;i++)
		{
			if(i<jishunow->numPara)
			{
				m_editParam[i][0]=jishunow->namePara[i];
				ShowData(m_editParam[i][1],jishunow->max[i]);
				ShowData(m_editParam[i][2],jishunow->min[i]);
				float f = jishunow->defaultVal[i];
				if(m_nKlineType!=-1)
					f = CFormularContent::GetParamDataEach(i,m_nKlineType,jishunow);
				ShowData(m_editParam[i][3],f);
//				for(j=0;j<11;j++)
//				{
//					ShowData(m_editParamPeriod[i][j],jishunow->defaultValArray[i].fValue[j]);
//				}
			}
			else
			{
				for(j=0;j<4;j++)
					m_editParam[i][j]="";
			}
			;
		}
	}
	else
	{
		if(jishunow->numPara>8)
			jishunow->numPara = 0;
		int nBegin = 0;
		if(nFlag==1&&jishunow->numPara>4)
			nBegin = 4;
		for(i=nBegin;i<4+nBegin;i++)
		{
			if(i<jishunow->numPara-nBegin)
			{
				SetStringParam(i,0,jishunow->namePara[i]);
				CString s;
				ShowData(s,jishunow->max[i]);
				SetStringParam(i,1,s);
				ShowData(s,jishunow->min[i]);
				SetStringParam(i,2,s);

				float f = jishunow->defaultVal[i];
				if(m_nKlineType!=-1)
					f = CFormularContent::GetParamDataEach(i,m_nKlineType,jishunow);

				ShowData(s,f);
				SetStringParam(i,3,s);
			}
			else
				for(j=0;j<4;j++)
					SetStringParam(i,j,"");
		}
	}
}

///////////////////////////////////////////////////////////////////
//      ����˵�����ı似��ָ�����ݵ�ָ�����ʱ�����ڵ�����
//      ��ڲ�������
//      ���ز������޷�����
/////////////////////////////////////////////////////////////////////////////
void CTaiKlineDlgEditScreenStock::OnButtonCanUsed() 
{
	// TODO: Add your control notification handler code here
	CTaiKlineDlgCanUsedPeriod dlgCanUsed;
	dlgCanUsed.InputString(m_nPeriodsUsed);
	if(dlgCanUsed.DoModal ()==IDOK)
		dlgCanUsed.OutSetting (m_nPeriodsUsed);
}

///////////////////////////////////////////////////////////////////
//      ����˵�����ı�ָ�깫ʽ���ݵ�ָ�����ע������
//      ��ڲ�������
//      ���ز������޷�����
/////////////////////////////////////////////////////////////////////////////
void CTaiKlineDlgEditScreenStock::OnButton1() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	CDialogLOGGSSM mdiagssm;
	mdiagssm.m_bReadOnly = false;
	mdiagssm.m_edit=m_strParamExplain;
	if(mdiagssm.DoModal()==IDOK)
		m_strParamExplain=mdiagssm.m_edit;
	UpdateData(FALSE);
}

void CTaiKlineDlgEditScreenStock::OnSelchangeComboKindTime() 
{
	// TODO: Add your control notification handler code here

}

void CTaiKlineDlgEditScreenStock::ChangePeriodParamData(bool bSave)//���桢��ȡ���ڵĲ�������
{
/*	if(this->jishunow==NULL)
		return ;

	UpdateData();
	CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_COMBO1);
	CString sName ;
	pCombo->GetWindowText(sName);
	int n=-1;
//	n=pCombo->GetCount ();
	for(int i=0;i<11;i++)
	{
		if(sName == CTaiScreenParent::m_gPeriodName[i] )
		{
			n=i;
			break;
		}
	}
	ASSERT(n>=0);

	OnTestFormula()	;
	if(errorposition!=-1)
		if(AfxMessageBox("���༭�Ĺ�ʽû��ͨ��\n�﷨��飬\nҪ������",MB_OKCANCEL|MB_ICONQUESTION)==IDCANCEL)
				    return;

	jishunow->InitDefaultValArray();

	if(bSave ==true)
	{

//		m_editParamPeriod;
	}
	else
	{
	}
*/



}



CString CTaiKlineDlgEditScreenStock::GetStringParam(int x, int y)
{
	CString s ="";
	GetDlgItem(m_nID[x][y])->GetWindowText(s);
	return s;
}

void CTaiKlineDlgEditScreenStock::SetStringParam(int x, int y, CString sIn)
{
	GetDlgItem(m_nID[x][y])->SetWindowText(sIn);
}

BOOL CTaiKlineDlgEditScreenStock::OnHelpInfo(HELPINFO* pHelpInfo) 
{
	// TODO: Add your message handler code here and/or call default
	
//	HtmlHelp(m_hWnd,"stock.chm",HH_HELP_CONTEXT,CTaiKlineIndicatorsManage::IDD);
//	return TRUE;//
	DoHtmlHelp(this);return TRUE;
}

bool CTaiKlineDlgEditScreenStock::FindIndicator(int nKind, CString sName,CFormularContent  *jishunow)
{
	CTaiShanDoc*	pDoc=CMainFrame::m_pDoc ;//(CTaiShanDoc*)pchildfram->GetActiveDocument();
	CFormularArray* pArray[3] = {&(pDoc->m_formuar_index ),&(pDoc->m_formuar_choose),&(pDoc->m_formuar_kline )};
	int length=pArray[nKind]->GetSize();
	if(length>0)
	{
		for(int k=0;k<length;k++)
		{
			if(sName.Compare(pArray[nKind]->GetAt(k)->name)==0 && jishunow!=pArray[nKind]->GetAt(k))
				return true;
		}
	}
	else return false;
	return false;
}

void CTaiKlineDlgEditScreenStock::OnChangeInput() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	UpdateData();
	if(g_bInitDlgEdit)
		OnChangeInput2();
	
	
}

void CTaiKlineDlgEditScreenStock::OnChangeInput2()
{
	// TODO: Add your control notification handler code here
  CString formal=""		;
  m_inputcor.GetWindowText(formal);

  if(formal.GetLength()==0)
  {
	  return;
  }
  formal.MakeLower();
  Kline kline;
  CFormularCompute equation(0,0,&kline,formal);
  equation.m_bShowColor = true;
	
  bool errpram = false;
  for(int i=0;i<8;i++)
  {
	  if(m_editParam[i][0]!=""&&m_editParam[i][3]!="")
	  {  
		  float fpram=StringToFloat(m_editParam[i][3]);
		  m_editParam[i][0].MakeLower();
		  int addpra=equation.AddPara(m_editParam[i][0],fpram);
		  switch(addpra)
		  {
		  case(1):
		  case(2):
			  errpram=true;
			  break;
		  }
		  if(errpram == true)
		  {
			  AfxMessageBox("����������ȷ������");
			  return;
		  }
	  }
  }
  equation.Devide();

  //ȡ��λ����ɫ��Ϣ
  if(equation.m_nErrPos==-1 && equation.m_pKindPos )
  {
  //formal;
	  COLORREF  clr[10] = {RGB(0,0,255),RGB(0,150,0),
		  RGB(120,0,0),RGB(255,0,0),
		  RGB(0,255,255),RGB(0,0,120)};

	  m_inputcor.SetRedraw(FALSE);
	  CPoint pt =  GetCaretPos( );
	  long nStartChar, nEndChar;
	  m_inputcor.GetSel( nStartChar, nEndChar ) ;

	  static int iii = 0;
	  iii++;
	  TRACE("iii = %d",iii);
	  for(int k = 0;k<equation.m_nSizeKindPos ;k++)
	  {
		  int b = equation.m_pKindPos[k].m_nPos;
		  int e ;
		  if(k == equation.m_nSizeKindPos -1)
			  e = formal.GetLength ()-1;
		  else
			  e = equation.m_pKindPos[k+1].m_nPos;
		  if(e<0) e = 0;
		  if(e>formal.GetLength ()-1)
			  e = formal.GetLength ()-1;
		  if(b<0) b = 0;
		  if(b>formal.GetLength ()-1)
			  b = formal.GetLength ()-1;

		  m_inputcor.SetSel(b,e);
		  if(equation.m_pKindPos[k].m_nKind<10
			  &&equation.m_pKindPos[k].m_nKind>=0)
			m_inputcor.SetSelectColor( clr[equation.m_pKindPos[k].m_nKind]);

	  }
	  m_inputcor.SetSel( nStartChar, nEndChar ) ;
//	  m_inputcor.HideSelection(TRUE, FALSE);

//SetSel(-1,-1);
	  SetCaretPos( pt );
	  m_inputcor.SetRedraw();
	  m_inputcor.RedrawWindow();
  }

}

void CTaiKlineDlgEditScreenStock::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	CRect r;
	if(m_inputcor.m_hWnd )
	{
		GetDlgItem (IDC_EDIT41)->GetWindowRect(r);
		this->ScreenToClient (r);
//		m_inputcor.GetWindowRect(r);
		CRect r2(5,r.bottom+12,cx-5,cy-5);
		m_inputcor.MoveWindow(r2);
	}
	
}
void CTaiKlineDlgEditScreenStock::DoMoveWindow()
{
	if(m_bShowFromMouse)
	{
		CPoint p;
		GetCursorPos(&p);
		CRect r;
		GetWindowRect(r);
		this->SetWindowPos(NULL,p.x,p.y,r.Width (),r.Height (),SWP_NOREDRAW);
	}

}
