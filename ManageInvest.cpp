// ManageInvest.cpp : implementation file
//

#include "stdafx.h"
#include "WH.h"
#include "ManageInvest.h"
#include "TransactFee.h"
#include "EditInvest.h"
#include "KEYBRODEANGEL.h"
#include "mainfrm.h"
#include <io.h>
#include "CTaiKlineTransferKline.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


//���ӣ����Ǳ༭
enum Operation{Add,Edit};

/////////////////////////////////////////////////////////////////////////////
// CSuperviseInvest dialog



CSuperviseInvest::CSuperviseInvest(CWnd* pParent /*=NULL*/)
	: CDialog(CSuperviseInvest::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSuperviseInvest)
	//}}AFX_DATA_INIT

	double ShRate[5][5]={ 
		0.004,0.0035,10.00,0.001,1.00,
		0.003,0.0060,20.00,0.001,1.00,
		0.000,0.0020, 5.00,0.000,0.00,
		0.000,0.0035, 5.00,0.001,1.00,
		0.000,0.0025, 5.00,0.000,0.00
	}; //�Ϻ��ķ���
	memcpy( m_ShRate,ShRate,25*sizeof(double));

	double SzRate[5][5]={
		0.004,0.0035, 5.00,0.000,0.00,
		0.003,0.0043, 0.00,0.005,0.00,
		0.000,0.0020, 5.00,0.000,0.00,
		0.000,0.0030, 5.00,0.000,0.00,
		0.000,0.0025, 5.00,0.000,0.00
	}; //���ڵķ���
	memcpy( m_SzRate,SzRate,25*sizeof(double));

}


void CSuperviseInvest::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSuperviseInvest)
	DDX_Control(pDX, IDCANCEL, m_cancel);
	DDX_Control(pDX, IDC_FEE, m_fee);
	DDX_Control(pDX, IDC_EDIT, m_edit);
	DDX_Control(pDX, IDC_DELETE, m_delete);
	DDX_Control(pDX, IDC_ADD, m_add);
	DDX_Control(pDX, 1050, m_TabCtrl);
	DDX_Control(pDX, IDC_MSFLEXGRID1, m_MsFlexGrid);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSuperviseInvest, CDialog)
	//{{AFX_MSG_MAP(CSuperviseInvest)
	ON_BN_CLICKED(IDC_DELETE, OnDelete)
	ON_BN_CLICKED(IDC_EDIT, OnEdit)
	ON_BN_CLICKED(IDC_ADD, OnAdd)
	ON_BN_CLICKED(IDC_FEE, OnFee)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB, OnSelchangeTab)
	ON_WM_HELPINFO()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSuperviseInvest message handlers

BOOL CSuperviseInvest::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	SetWindowText("Ͷ�ʹ��� - "+m_szFileName);
	
	// ��ʼ��Tabҳ
	TC_ITEM tci;
	CString str;
	tci.mask=TCIF_TEXT;

	str="�ɽ���ϸ";
	tci.pszText=(LPSTR)(LPCTSTR)str;
	tci.cchTextMax=str.GetLength();
	this->m_TabCtrl.InsertItem(0,&tci);

	str="�ֲ�ͳ��";
	tci.pszText=(LPSTR)(LPCTSTR)str;
	tci.cchTextMax=str.GetLength();
	this->m_TabCtrl.InsertItem(1,&tci);

	str="�ʽ��ȡ";
	tci.pszText=(LPSTR)(LPCTSTR)str;
	tci.cchTextMax=str.GetLength();
	this->m_TabCtrl.InsertItem(2,&tci);

	//��ʼ���û���Ϣ��Ͷ�ʼ�¼
	CFile UserFile;
	if( UserFile.Open(m_szFileName,CFile::modeRead) )
	{
		UserFile.Read(&m_Account,sizeof(m_Account));
		for( int j=0;j<16;j++)
			m_Account.chPassword[j]=~m_Account.chPassword[j];

		m_pInvest=new Invest[m_Account.uRecords];
		UserFile.Read(m_pInvest,sizeof(Invest)*m_Account.uRecords);
		UserFile.Close();
	}
	else
	{
		m_Account.uRecords=0;
		m_pInvest=new Invest[m_Account.uRecords];
	}


	//��������ļ�������
	if(_access(g_rate,0)==-1)   
	{
		CFile file(g_rate,CFile::modeCreate|CFile::modeWrite);
		file.Write(m_ShRate,sizeof(m_ShRate));
		file.Write(m_SzRate,sizeof(m_SzRate));
		file.Close();
	}

	CFile file(g_rate,CFile::modeCreate|CFile::modeNoTruncate|CFile::modeReadWrite);
	file.Read(m_ShRate,sizeof(m_ShRate));
	file.Read(m_SzRate,sizeof(m_SzRate));
	//file.Read(m_CyRate,sizeof(m_CyRate));
	file.Close();

	//��ʾ�ɽ���ϸ
	DisplayList(0);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

//ɾ��ĳһ����¼�������ɽ���ϸ���ʽ�洢
void CSuperviseInvest::OnDelete() 
{
	unsigned int i;

	int uRow=m_MsFlexGrid.GetRow()-1;
	bool b=TRUE;
	for(i=0;i<8;i++)
		b&= (m_MsFlexGrid.GetTextMatrix(uRow+1,0)=="") ;
	if( b )
		return;

	if( AfxMessageBox("���Ҫɾ����?",MB_ICONQUESTION|MB_YESNO|MB_DEFBUTTON2)==IDNO )
		return;

	Invest* p=NULL;
	unsigned int nCount=0;
	switch(m_TabCtrl.GetCurSel())
	{
	case 0://����
		for( i=0;i<m_Account.uRecords;i++)
		{
			if( (m_pInvest+i)->uType == 0 )
			{
				if( uRow-- == 0 )
					nCount=i;
			}
		}

		p=new Invest[m_Account.uRecords];
		memcpy( p, m_pInvest,sizeof(Invest)*m_Account.uRecords);
		delete []m_pInvest;

		m_Account.uRecords--;
		m_pInvest=new Invest[m_Account.uRecords];
		memcpy(m_pInvest,p,sizeof(Invest)*nCount );
		memcpy(m_pInvest+nCount,p+nCount+1,sizeof(Invest)*(m_Account.uRecords-nCount));
		delete []p;

		DisplayList(0);
		break;
	case 2://�ʽ�
		for( i=0;i<this->m_Account.uRecords;i++)
		{
			if( (m_pInvest+i)->uType == 1 )
			{
				if( uRow-- == 0 )
					nCount=i;
			}
		}

		p=new Invest[m_Account.uRecords];
		memcpy( p, m_pInvest,sizeof(Invest)*m_Account.uRecords);
		delete []m_pInvest;

		m_Account.uRecords--;
		m_pInvest=new Invest[m_Account.uRecords];
		memcpy(m_pInvest,p,sizeof(Invest)*nCount );
		memcpy(m_pInvest+nCount,p+nCount+1,sizeof(Invest)*(m_Account.uRecords-nCount));
		delete []p;


		DisplayList(2);

		break;
	}

}

//�༭
void CSuperviseInvest::OnEdit() 
{
	unsigned int i;
	int uRow=m_MsFlexGrid.GetRow();
	bool b=TRUE;
	for(i=0;i<8;i++)
		b&= (m_MsFlexGrid.GetTextMatrix(uRow,0)=="") ;
	if( b )
		return;

	uRow--;


	CEditInvest EditInvest;
	EditInvest.m_Operation=Edit;//�༭����
	EditInvest.m_SelectTab=this->m_TabCtrl.GetCurSel();

	switch(EditInvest.m_SelectTab)
	{
	case 0://����
		for( i=0;i<this->m_Account.uRecords;i++)
		{
			if( (m_pInvest+i)->uType == 0 )
			{
				if( uRow-- == 0 )
					break;
			}
		}

		EditInvest.m_RefTime=(m_pInvest+i)->Transact.uTime;//ʱ��
		EditInvest.m_RefIn=(m_pInvest+i)->Transact.nWay;
		EditInvest.m_RefFee=(m_pInvest+i)->Transact.dFee;//������
		EditInvest.m_RefPrice=(m_pInvest+i)->Transact.dPrice;//�۸�
		EditInvest.m_RefShares=(m_pInvest+i)->Transact.dVolume;//�ɽ���
		EditInvest.m_Name=(m_pInvest+i)->Transact.chName;//����
		EditInvest.m_Symbol=(m_pInvest+i)->Transact.chSymbol;
		EditInvest.m_nKind=(m_pInvest+i)->Transact.nKind;
		break;
	case 2://�ʽ�
		for( i=0;i<this->m_Account.uRecords;i++)
		{
			if( (m_pInvest+i)->uType == 1 )
			{
				if( uRow-- == 0 )
					break;
			}
		}

		EditInvest.m_RefTime=(m_pInvest+i)->Money.uTime;//ʱ��
		EditInvest.m_RefIn=(m_pInvest+i)->Money.nWay;
		EditInvest.m_RefFee=(m_pInvest+i)->Money.dAmount;//������
		break;
	}

	if( EditInvest.DoModal() == IDOK )
	{
		if( EditInvest.m_SelectTab== 0 )//����
		{
			(m_pInvest+i)->uType=0;

			(m_pInvest+i)->Transact.uTime   =(EditInvest.m_RefTime).GetTime();//ʱ��
			strcpy( (m_pInvest+i)->Transact.chSymbol,EditInvest.m_Symbol );//����
			(m_pInvest+i)->Transact.nKind=EditInvest.m_nKind;
			strcpy( (m_pInvest+i)->Transact.chName  ,EditInvest.m_Name );//����
			(m_pInvest+i)->Transact.nWay    =EditInvest.m_RefIn;
			(m_pInvest+i)->Transact.dPrice  =EditInvest.m_RefPrice;
			(m_pInvest+i)->Transact.dVolume =EditInvest.m_RefShares;
			(m_pInvest+i)->Transact.dFee    =EditInvest.m_RefFee;

			this->DisplayList(0);
		}

		if( EditInvest.m_SelectTab== 2 )//�ʽ�
		{
			(m_pInvest+i)->uType=1;

			(m_pInvest+i)->Money.uTime=(EditInvest.m_RefTime).GetTime();//ʱ��
			(m_pInvest+i)->Money.nWay=EditInvest.m_RefIn;
			(m_pInvest+i)->Money.dAmount=EditInvest.m_RefFee;

			this->DisplayList(2);
		}

	}


}

//����
void CSuperviseInvest::OnAdd() 
{
	CEditInvest EditInvest;
	EditInvest.m_Operation=Add;//���Ӳ���

	if( EditInvest.DoModal()==IDOK )
	{
		Invest* tmp=new Invest[m_Account.uRecords];
		memcpy( tmp,m_pInvest,sizeof(Invest)*m_Account.uRecords );
		delete []m_pInvest;

		m_pInvest=new Invest[ m_Account.uRecords+1 ];
		memcpy(m_pInvest,tmp,sizeof(Invest)*m_Account.uRecords);
		delete []tmp;

		if( EditInvest.m_SelectTab== 0 )//����
		{
			(m_pInvest+m_Account.uRecords)->uType=0;
			if( EditInvest.m_Symbol=="" ) return;

			(m_pInvest+m_Account.uRecords)->Transact.uTime   =(EditInvest.m_RefTime).GetTime();//ʱ��
			strcpy( (m_pInvest+m_Account.uRecords)->Transact.chSymbol,EditInvest.m_Symbol );//����
			(m_pInvest+m_Account.uRecords)->Transact.nKind=EditInvest.m_nKind;

			strcpy( (m_pInvest+m_Account.uRecords)->Transact.chName  ,EditInvest.m_Name );//����
			(m_pInvest+m_Account.uRecords)->Transact.nWay    =EditInvest.m_RefIn;
			(m_pInvest+m_Account.uRecords)->Transact.dPrice  =EditInvest.m_RefPrice;
			(m_pInvest+m_Account.uRecords)->Transact.dVolume =EditInvest.m_RefShares;
			(m_pInvest+m_Account.uRecords)->Transact.dFee    =EditInvest.m_RefFee;

			m_Account.uRecords+=1;//��¼��+1
			this->DisplayList(0);
			this->m_TabCtrl.SetCurSel(0);
			
		}

		if( EditInvest.m_SelectTab== 1 )//�ʽ�
		{
			(m_pInvest+m_Account.uRecords)->uType=1;
			if( EditInvest.m_RefFee==0.0 ) return;

			(m_pInvest+m_Account.uRecords)->Money.uTime=(EditInvest.m_RefTime).GetTime();//ʱ��
			(m_pInvest+m_Account.uRecords)->Money.nWay=EditInvest.m_RefIn;
			(m_pInvest+m_Account.uRecords)->Money.dAmount=EditInvest.m_RefFee;

			m_Account.uRecords+=1;//��¼��+1
			this->DisplayList(2);
			this->m_TabCtrl.SetCurSel(2);
			
		}

	}
	
}

void CSuperviseInvest::OnFee() 
{

	CTransactFee TransactFee;
	TransactFee.DoModal();
}

void CSuperviseInvest::DisplayList(unsigned int nTab)
{
	unsigned i=0;//ѭ������
	char* chTransact[]={"�ɽ�ʱ��","��Ʊ����","��Ʊ����","���׷���","�ɽ��۸�","�ɽ���(��)","�ɽ���(Ԫ)","������(Ԫ)"};
	char* chHold[]={"��Ʊ����","��Ʊ����","ʵʱ�۸�","�ɱ���","ƽ���","�ֲ���(��)","��ֵ(Ԫ)","����ӯ��(Ԫ)"};
	char *chMoney[]={"��ȡʱ��","��ȡ����","��ȡ����"};
	char ch[255]; //��ʽ���ַ�����
	int nRow=0;//��¼ÿһ���¼�ĸ���
	double dTotalMarketValue=0.0;//����ֵ
	double dTotalFloatEarning=0.0;//�ܸ���ӯ��
	m_dLeftMoney=0.0;//ʣ���ʽ�
	CTime tm;
	switch(nTab)
	{
	case 0://�ɽ���ϸ
		GetDlgItem(IDC_ADD)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_EDIT)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DELETE)->ShowWindow(SW_SHOW);
		m_MsFlexGrid.Clear();

		if( m_Account.uRecords>499 )
		{
			AfxMessageBox("����¼�����ܳ���500.");
			return;
		}
	

		for( i=0;i<8;i++)//����
			m_MsFlexGrid.SetTextArray(i,chTransact[i]);

		for( i=0;i<m_Account.uRecords;i++)
		{
			if( (m_pInvest+i)->uType==0 )//���׼�¼
			{
				nRow++;

				// 0627
				Kline kline[5];
				memset(kline,0,sizeof(Kline)*5);
				for(int k=0;k<5;k++)
				{
					kline[k].day = (m_pInvest+i)->Transact.uTime;
					kline[k].open = (m_pInvest+i)->Transact.dPrice;
					kline[k].close = (m_pInvest+i)->Transact.dPrice;
					kline[k].high = (m_pInvest+i)->Transact.dPrice;
					kline[k].low = (m_pInvest+i)->Transact.dPrice;
					kline[k].vol = (m_pInvest+i)->Transact.dVolume;
				}
				kline[4].day = CTime::GetCurrentTime ().GetTime ();
				powerArr splitArr;
				CString symbol = (m_pInvest+i)->Transact.chSymbol;
				int nKind=(m_pInvest+i)->Transact.nKind;
				CTaiKlineTransferKline::ReadPower(CMainFrame::m_pDoc ,symbol,nKind, splitArr);//m_PowerArray
				CTaiKlineTransferKline::ComputePower(kline , 5, 5 ,splitArr);// this-> );
				//end 0627

				tm=(m_pInvest+i)->Transact.uTime;
				sprintf(ch,"%d-%d-%d", tm.GetYear(), tm.GetMonth(), tm.GetDay() );
				m_MsFlexGrid.SetTextMatrix(nRow,0, ch  );				
				
				m_MsFlexGrid.SetTextMatrix(nRow,1, (m_pInvest+i)->Transact.chSymbol );//����
				m_MsFlexGrid.SetTextMatrix(nRow,2, (m_pInvest+i)->Transact.chName );//����

				char* chtmp=((m_pInvest+i)->Transact.nWay==1) ? "���" : "����";//���׷���
				strcpy(ch,chtmp);
				m_MsFlexGrid.SetTextMatrix(nRow,3,ch);				

				sprintf(ch,"%.2f", kline[0].close  );
				m_MsFlexGrid.SetTextMatrix(nRow,4,ch);				

				sprintf(ch,"%.2f", kline[0].vol );
				m_MsFlexGrid.SetTextMatrix(nRow,5,ch);				

				sprintf(ch,"%.2f", (m_pInvest+i)->Transact.dVolume*(m_pInvest+i)->Transact.dPrice );
				m_MsFlexGrid.SetTextMatrix(nRow,6,ch);				

				sprintf(ch,"%.2f", (m_pInvest+i)->Transact.dFee );
				m_MsFlexGrid.SetTextMatrix(nRow,7,ch);				
			}
		}
		sprintf(ch,"��������%d�ν���.",nRow);
		GetDlgItem(IDC_STATIC_STATISTIC)->SetWindowText(ch);
		GetDlgItem(IDC_EDIT_SYMBOL)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_SHARES)->ShowWindow(SW_HIDE);

		break;
	case 1://�ֲ�ͳ��
		GetDlgItem(IDC_ADD)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DELETE)->ShowWindow(SW_HIDE);
		m_MsFlexGrid.Clear();

		for( i=0;i<8;i++)//����
			m_MsFlexGrid.SetTextArray(i,chHold[i]);

		for( i=0;i<m_Account.uRecords;i++)
		{
			BOOL b=TRUE;//�ֲ�ͳ��ʱʹ��
			double dCost=0.0;//���л��ѣ����������Ѻͳɽ����
			double dVolume=0.0;//�ֲ���

			if( (m_pInvest+i)->uType==0 )//���׼�¼
			{
				for( unsigned int j=0;j<i;j++)
				{
					if( (m_pInvest+j)->uType==0 && 
						strcmp( (m_pInvest+j)->Transact.chSymbol,(m_pInvest+i)->Transact.chSymbol)==0 &&
						(m_pInvest+j)->Transact.nKind==(m_pInvest+i)->Transact.nKind)//z
					{	
						b=FALSE;/*˵���Ѿ�����*/ 		
						break;	
					}
				}

				if( b )//�����δ�����
				{
					for( unsigned int j=i;j<m_Account.uRecords; j++)
					{
						if( (m_pInvest+j)->uType==0 && 
							strcmp( (m_pInvest+j)->Transact.chSymbol,(m_pInvest+i)->Transact.chSymbol)==0 &&
							(m_pInvest+j)->Transact.nKind==(m_pInvest+i)->Transact.nKind)//
						{
							dCost+=(m_pInvest+j)->Transact.dPrice*(m_pInvest+j)->Transact.dVolume*(m_pInvest+j)->Transact.nWay+(m_pInvest+j)->Transact.dFee;//�ܻ���
							dVolume+=(m_pInvest+j)->Transact.dVolume*(m_pInvest+j)->Transact.nWay;//�ֲ���
						}
					}

					if(dVolume!=0.0)
					{
						nRow++;

						m_MsFlexGrid.SetTextMatrix(nRow,0, (m_pInvest+i)->Transact.chSymbol );//����
						m_MsFlexGrid.SetTextMatrix(nRow,1, (m_pInvest+i)->Transact.chName );//����

						CReportData* pDat=NULL;
						if( (CMainFrame::m_pDoc)->m_sharesInformation.Lookup((m_pInvest+i)->Transact.chSymbol , pDat,(m_pInvest+i)->Transact.nKind) && pDat!=NULL )
							sprintf(ch,"%.2f", pDat->nowp );//�ּ�
						else
							sprintf(ch,"%.2f", 0.0 );//�ּ�
						m_MsFlexGrid.SetTextMatrix(nRow,2,ch);				

						sprintf(ch,"%.2f", dCost/dVolume );//�ɱ���
						m_MsFlexGrid.SetTextMatrix(nRow,3,ch);				

						double dRate= (pDat==NULL)? 0.0075 : GetFeeRate(pDat->kind, (m_pInvest+i)->Transact.chSymbol);
						sprintf(ch,"%.2f", dCost/(dVolume*(1- dRate )) );//ƽ���
						m_MsFlexGrid.SetTextMatrix(nRow,4,ch);				

						sprintf(ch,"%.2f", dVolume );//�ֲ���
						m_MsFlexGrid.SetTextMatrix(nRow,5,ch);				

						sprintf(ch,"%.2f", ( (pDat==NULL) ? 0.0:dVolume*pDat->nowp )     );//��ֵ
						m_MsFlexGrid.SetTextMatrix(nRow,6,ch);				

						dTotalMarketValue+=( (pDat==NULL) ? 0.0:dVolume*pDat->nowp );//����ֵ

						sprintf(ch,"%.2f", dVolume*( (pDat==NULL) ? 0.0:pDat->nowp )-dCost );//����ӯ��
						m_MsFlexGrid.SetTextMatrix(nRow,7,ch);				
						dTotalFloatEarning+=dVolume*( (pDat==NULL) ? 0.0 : pDat->nowp )-dCost;//�ܸ���ӯ��
					}
				}
			}
		}

		sprintf(ch,"����ֵ��%.2f.\t�ܸ���ӯ����%.2f.",dTotalMarketValue,dTotalFloatEarning);
		GetDlgItem(IDC_STATIC_STATISTIC)->SetWindowText(ch);
		GetDlgItem(IDC_EDIT_SYMBOL)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_SHARES)->ShowWindow(SW_HIDE);

		break;
	case 2://�ʽ��ȡ
		GetDlgItem(IDC_ADD)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_EDIT)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DELETE)->ShowWindow(SW_SHOW);
		m_MsFlexGrid.Clear();

		for( i=0;i<3;i++)//����
			m_MsFlexGrid.SetTextArray(i,chMoney[i]);

		for( i=0;i<m_Account.uRecords;i++)
		{
			if( (m_pInvest+i)->uType==1 )//�ʽ��ȡ
			{
				nRow++;

				tm=(m_pInvest+i)->Money.uTime;
				sprintf(ch,"%d-%d-%d", tm.GetYear(), tm.GetMonth(), tm.GetDay() );
				m_MsFlexGrid.SetTextMatrix(nRow,0, ch  );				
				
				char* chtmp=((m_pInvest+i)->Money.nWay==1) ? "����" : "ȡ��";//��ȡ����
				strcpy(ch,chtmp);
				m_MsFlexGrid.SetTextMatrix(nRow,1,ch);

				sprintf(ch,"%.2f", (m_pInvest+i)->Money.dAmount );//���
				m_MsFlexGrid.SetTextMatrix(nRow,2,ch);			

				m_dLeftMoney+=(m_pInvest+i)->Money.dAmount*(m_pInvest+i)->Money.nWay;//ʣ���ʽ�
			}
			else
				m_dLeftMoney+=-(m_pInvest+i)->Transact.dVolume*(m_pInvest+i)->Transact.dPrice*(m_pInvest+i)->Transact.nWay-(m_pInvest+i)->Transact.dFee;
		}
		GetDlgItem(IDC_EDIT_SYMBOL)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STATIC_SHARES)->ShowWindow(SW_SHOW);
		sprintf(ch,"ʣ���ʽ�%.2f, �ܹ������Ʊ",m_dLeftMoney);
		GetDlgItem(IDC_STATIC_STATISTIC)->SetWindowText(ch);
		break;
	}
}


void CSuperviseInvest::OnCancel() 
{
	CFile UserFile(m_szFileName,CFile::modeWrite|CFile::modeCreate);
	
	strcpy(m_Account.chPassword,this->m_Password);
	for( int j=0;j<16;j++)
		m_Account.chPassword[j]=~m_Account.chPassword[j];

	UserFile.Write(&m_Account,sizeof(m_Account));

	UserFile.Write(m_pInvest,sizeof(Invest)*m_Account.uRecords);
	UserFile.Close();

	//��̬������ڴ����
	delete []m_pInvest;

	CDialog::OnCancel();
}

BOOL CSuperviseInvest::PreTranslateMessage(MSG* pMsg) 
{
  
	if( GetFocus() == GetDlgItem(IDC_EDIT_SYMBOL) 
		&& pMsg->message==WM_CHAR 
		&& ( (pMsg->wParam>='0'&&pMsg->wParam<='9') 
		|| (pMsg->wParam>='A'&&pMsg->wParam<='z') )  )
	{
		CTaiKeyBoardAngelDlg KeyboardWizard;

		if(pMsg->wParam>='a' && pMsg->wParam<='z' )
			pMsg->wParam-=32;
			
		KeyboardWizard.input=pMsg->wParam;

		KeyboardWizard.ischar= ( pMsg->wParam>='0' && pMsg->wParam<='9' )? false:true;

		KeyboardWizard.DoModal();
	    
		if(KeyboardWizard.isresultstock )
		{
			CString szSymbol=KeyboardWizard.result;
			m_nKind=KeyboardWizard.m_stkKind;
			CReportData* pDat;
			char ch[255];
		 	if ( (CMainFrame::m_pDoc)->m_sharesInformation.Lookup(szSymbol.GetBuffer(0) , pDat,m_nKind) )     //���ù�Ʊ�Ƿ��Ѽ�¼���ڴ�ģ����
			{
				GetDlgItem(IDC_EDIT_SYMBOL)->SetWindowText(szSymbol);
				if(pDat->nowp!=0.0f)
					sprintf(ch,"%s%.2f��.", pDat->name,m_dLeftMoney/pDat->nowp);
				else
					sprintf(ch,"%s%.2f��(ʵʱ�۸�δ֪).", pDat->name,0.0);
				GetDlgItem(IDC_STATIC_SHARES)->SetWindowText(ch);
			}
		}
		return TRUE;
	}
	else if( pMsg->wParam == VK_F1)	
		this->OnHelpInfo(NULL);

	
	return CDialog::PreTranslateMessage(pMsg);
}

double CSuperviseInvest::GetFeeRate(unsigned uKind,char chSymbol[])
{
	enum StockType{AShare,BShare,Bond,Fund,InvestFund};//��Ʊ����
	enum RateType{Tax,Comm,MiniComm,Fee,MiniFee};//��������

	double dRate=0.0;
	switch(uKind)
	{
	case 0:
		//dRate=atof(szPrice)*atof(szShares)*0.075;
		break;
	case 1://��A
		if( chSymbol[0]=='5')//����
			dRate=m_ShRate[Fund][Tax]+m_ShRate[Fund][Comm]+m_ShRate[Fund][Fee];
		else
			dRate=m_ShRate[AShare][Tax]+m_ShRate[AShare][Comm]+m_ShRate[AShare][Fee];
		break;
	case 2://��B
		dRate=m_ShRate[BShare][Tax]+m_ShRate[BShare][Comm]+m_ShRate[BShare][Fee];
		break;
	case 3://��ָ
		break;
	case 4://��A
		dRate=m_SzRate[AShare][Tax]+m_SzRate[AShare][Comm]+m_SzRate[AShare][Fee];
		break;
	case 5://��B
		dRate=m_SzRate[BShare][Tax]+m_SzRate[BShare][Comm]+m_SzRate[BShare][Fee];		
		break;
	case 6://��ծ
		dRate=m_ShRate[Bond][Tax]+m_ShRate[Bond][Comm]+m_ShRate[Bond][Fee];
		break;
	case 7://��ծ
		dRate=m_SzRate[Bond][Tax]+m_SzRate[Bond][Comm]+m_SzRate[Bond][Fee];		
		break;
	case 8://��ָ
		break;
	case 9://����
		break;
	}
	return dRate;

}

//������Ԥ�����óֲֹ�Ʊ
void CSuperviseInvest::AlarmCallInvest(SymbolKindArr &SymbolArray,CString AccountName,CString Password)
{
	CFile UserFile;
	AccountID Account;
	Invest* pInvest;
	AccountName="Data/Account/"+AccountName;
	if( UserFile.Open(AccountName,CFile::modeRead) )
	{
		UserFile.Read(&Account,sizeof(Account));
		for( int j=0;j<16;j++)
			Account.chPassword[j]=~Account.chPassword[j];

		if( Account.chPassword!=Password )
		{
			AfxMessageBox("�����������.");
			UserFile.Close();
			return;
		}
		pInvest=new Invest[Account.uRecords];
		UserFile.Read(pInvest,sizeof(Invest)*Account.uRecords);
		UserFile.Close();
	}
	else
	{
		AfxMessageBox("���ʻ�������.");
		return;
	}


	double dVolume=0.0;

	for(int i=0;i<int(Account.uRecords);i++)
	{
		BOOL b=TRUE;//�ֲ�ͳ��ʱʹ��

		if( (pInvest+i)->uType==0 )//���׼�¼
		{
			for(  int j=0;j<i;j++)
			{
				dVolume=0.0;//�ֲ���
				if( (pInvest+j)->uType==0 && strcmp( (pInvest+j)->Transact.chSymbol,(pInvest+i)->Transact.chSymbol)==0 && 
					(pInvest+j)->Transact.nKind==(pInvest+i)->Transact.nKind)//
				{	
					b=FALSE;/*˵���Ѿ�����*/ 		
					break;	
				}
			}

			if( b )//�����δ�����
			{
				for( unsigned int j=i;j<Account.uRecords; j++)
				{
					if( (pInvest+j)->uType==0 && strcmp( (pInvest+j)->Transact.chSymbol,(pInvest+i)->Transact.chSymbol)==0 && 
						(pInvest+j)->Transact.nKind==(pInvest+i)->Transact.nKind)//
						dVolume+=(pInvest+j)->Transact.dVolume*(pInvest+j)->Transact.nWay;//�ֲ���
				}

				if(dVolume!=0.0)
				{
					SymbolKind symbolkind;
					strcmp(symbolkind.m_chSymbol,(pInvest+i)->Transact.chSymbol);
					symbolkind.m_nSymbolKind=((pInvest+i)->Transact.nKind);
					//SymbolArray.Add( (pInvest+i)->Transact.chSymbol );
					SymbolArray.Add( symbolkind );
				}
			}
		}
	}

	delete []pInvest;

}

BEGIN_EVENTSINK_MAP(CSuperviseInvest, CDialog)
    //{{AFX_EVENTSINK_MAP(CSuperviseInvest)
	ON_EVENT(CSuperviseInvest, IDC_MSFLEXGRID1, -601 /* DblClick */, OnDblClickMsflexgrid1, VTS_NONE)
	ON_EVENT(CSuperviseInvest, IDC_MSFLEXGRID1, -602 /* KeyDown */, OnKeyDownMsflexgrid1, VTS_PI2 VTS_I2)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

void CSuperviseInvest::OnDblClickMsflexgrid1() 
{
	// TODO: Add your control notification handler code here
	if( m_TabCtrl.GetCurSel()!=1 )
		this->OnEdit();
	
}

void CSuperviseInvest::OnKeyDownMsflexgrid1(short FAR* KeyCode, short Shift) 
{
	// TODO: Add your control notification handler code here
	if( *KeyCode == VK_DELETE )
		this->OnDelete();
	
}

void CSuperviseInvest::OnSelchangeTab(NMHDR* pNMHDR, LRESULT* pResult) 
{
	int nCurTab=this->m_TabCtrl.GetCurSel();
	switch(nCurTab)
	{
	case 0://��������
		DisplayList(nCurTab);
		break;
	case 1://�ֲ�ͳ��
		this->DisplayList(nCurTab);
		break;
	case 2://�ʽ��ȡ
		this->DisplayList(nCurTab);
		break;
	}
	
	*pResult = 0;
}

BOOL CSuperviseInvest::OnHelpInfo(HELPINFO* pHelpInfo) 
{
	// TODO: Add your message handler code here and/or call default
	return TRUE;//CDialog::OnHelpInfo(pHelpInfo);
}
