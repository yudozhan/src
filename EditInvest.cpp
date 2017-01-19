// EditInvest.cpp : implementation file
//

#include "stdafx.h"
#include "WH.h"
#include "EditInvest.h"
#include "KEYBRODEANGEL.h"
#include "mainfrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEditInvest dialog


CEditInvest::CEditInvest(CWnd* pParent /*=NULL*/)
	: CDialog(CEditInvest::IDD, pParent)
{
	//{{AFX_DATA_INIT(CEditInvest)
	m_Symbol = _T("");
	m_time = CTime::GetCurrentTime();
	m_In = -1;
	m_PositiveFee = 0.0f;
	m_PositivePrice = 0.0f;
	m_PositiveShares = 0.0f;
	//}}AFX_DATA_INIT
	m_RefFee=0.0;
	m_RefPrice=0.0;
	m_RefShares=0.0;
	m_RefIn = 1;
	m_SelectTab=0;
	m_Operation=0;
	
}


void CEditInvest::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEditInvest)
	DDX_Control(pDX, IDOK, m_ok);
	DDX_Control(pDX, IDCANCEL, m_cancel);
	DDX_Control(pDX, IDC_EDIT_SYMBOL, m_SymbolControl);
	DDX_Control(pDX, IDC_DATETIMEPICKER1, m_DateTimeCtrl);
	DDX_Control(pDX, IDC_EDIT_VALUE, m_Value);
	DDX_Control(pDX, IDC_EDIT_SHARES, m_Shares);
	DDX_Control(pDX, IDC_EDIT_FEE, m_Fee);
	DDX_Control(pDX, IDC_EDIT_PRICE, m_Price);
	DDX_Control(pDX, IDC_TAB1, m_TabCtrl);
	DDX_Text(pDX, IDC_EDIT_SYMBOL, m_Symbol);
	DDV_MaxChars(pDX, m_Symbol, 6);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER1, m_time);
	DDX_Radio(pDX, IDC_RADIO_BUY, m_In);
	DDX_Text(pDX, IDC_EDIT_FEE, m_PositiveFee);
	DDV_MinMaxFloat(pDX, m_PositiveFee, 0.f, 1.e+010f);
	DDX_Text(pDX, IDC_EDIT_PRICE, m_PositivePrice);
	DDV_MinMaxFloat(pDX, m_PositivePrice, 0.f, 1.e+006f);
	DDX_Text(pDX, IDC_EDIT_SHARES, m_PositiveShares);
	DDV_MinMaxFloat(pDX, m_PositiveShares, 0.f, 1.e+007f);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CEditInvest, CDialog)
	//{{AFX_MSG_MAP(CEditInvest)
	ON_NOTIFY(NM_CLICK, IDC_TAB1, OnClickTab)
	ON_EN_CHANGE(IDC_EDIT_SHARES, OnChangeEditShares)
	ON_EN_CHANGE(IDC_EDIT_PRICE, OnChangeEditPrice)
	ON_EN_CHANGE(IDC_EDIT_SYMBOL, OnChangeEditSymbol)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEditInvest message handlers

BOOL CEditInvest::OnInitDialog() 
{
	CDialog::OnInitDialog();


	CTime BeginDate(1990,1,1,1,1,0);
	CTime EndDate(2037,1,1,1,1,0);
	this->m_DateTimeCtrl.SetRange(&BeginDate,&EndDate);

	((CButton*)GetDlgItem(IDC_RADIO_BUY))->SetCheck(1);	//���롢���

	POINT pt;
	::GetCursorPos(&pt);
	CRect rect;
	GetWindowRect(rect);
	//ScreenToClient(rect);
	SetWindowPos(NULL,pt.x-rect.Width(),pt.y,rect.Width(),rect.Height(),SWP_SHOWWINDOW);

	
	//��ʼ������
	if( m_Operation )
	{
		this->SetWindowText("�༭");

		this->m_bInit=FALSE;

		// ��ʼ��TABҳ
		TC_ITEM tci;
		CString str;
		tci.mask=TCIF_TEXT;

		if( m_SelectTab== 0 ) 		//ȱʡΪ���������ס�ҳ
		{
			str="��������";
			tci.pszText=(LPSTR)(LPCTSTR)str;
			tci.cchTextMax=str.GetLength();
			this->m_TabCtrl.InsertItem(0,&tci);
		}
		else
		{
			str="�ʽ��ȡ";
			tci.pszText=(LPSTR)(LPCTSTR)str;
			tci.cchTextMax=str.GetLength();
			this->m_TabCtrl.InsertItem(1,&tci);
		}
	}
	else
	{
		// ��ʼ��TABҳ
		TC_ITEM tci;
		CString str;
		tci.mask=TCIF_TEXT;

		str="��������";
		tci.pszText=(LPSTR)(LPCTSTR)str;
		tci.cchTextMax=str.GetLength();
		this->m_TabCtrl.InsertItem(0,&tci);

		str="�ʽ��ȡ";
		tci.pszText=(LPSTR)(LPCTSTR)str;
		tci.cchTextMax=str.GetLength();
		this->m_TabCtrl.InsertItem(1,&tci);

	}


	if( m_SelectTab== 0 && m_Operation) 		//ȱʡΪ���������ס�ҳ
	{
		m_time=m_RefTime;
		UpdateData(FALSE);

		m_In=m_RefIn;  

		if( m_In == 1 )
			((CButton*)GetDlgItem(IDC_RADIO_BUY))->SetCheck(1);	//���롢���
		else // -1
			((CButton*)GetDlgItem(IDC_RADIO_SELL))->SetCheck(1);

		if( m_Name != "" )
			GetDlgItem(IDC_STATIC_NAME)->SetWindowText(m_Name);

		if( m_Symbol != "" )
			GetDlgItem(IDC_EDIT_SYMBOL)->SetWindowText(m_Symbol);

		char ch[255];
		if( m_RefPrice != 0.0 )
		{
			sprintf(ch,"%.2f",m_RefPrice);
			GetDlgItem(IDC_EDIT_PRICE)->SetWindowText(ch);
		}

		if( m_RefFee != 0.0 )
		{
			sprintf(ch,"%.2f",m_RefFee);
			GetDlgItem(IDC_EDIT_FEE)->SetWindowText(ch);
		}

		if( m_RefShares != 0.0 )
		{
			sprintf(ch,"%.2f",m_RefShares);
			GetDlgItem(IDC_EDIT_SHARES)->SetWindowText(ch);
		}

		if( m_RefPrice != 0.0 && m_RefShares!=0.0 )
		{
			sprintf(ch,"%.2f",m_RefPrice*m_RefShares);
			GetDlgItem(IDC_EDIT_VALUE)->SetWindowText(ch);
		}

		this->m_bInit=TRUE;

	}
	else  if( m_SelectTab== 1 && m_Operation)//Fund Tab
	{
		this->m_TabCtrl.SetCurSel(1);

		m_time=m_RefTime;
		UpdateData(FALSE);

		m_In=m_RefIn;  

		if( m_In == 1 )
			((CButton*)GetDlgItem(IDC_RADIO_BUY))->SetCheck(1);	//���롢���
		else // -1
			((CButton*)GetDlgItem(IDC_RADIO_SELL))->SetCheck(1);

		char ch[255];
		if( m_RefFee != 0.0 )
		{
			sprintf(ch,"%.2f",m_RefFee);
			GetDlgItem(IDC_EDIT_FEE)->SetWindowText(ch);
		}

		GetDlgItem(IDC_RADIO_BUY)->SetWindowText("����");
		GetDlgItem(IDC_RADIO_SELL)->SetWindowText("��ȡ");

		GetDlgItem(IDC_STATIC_NAME)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT_SYMBOL)->ShowWindow(SW_HIDE);

		GetDlgItem(IDC_STATIC_PRICE)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT_PRICE)->ShowWindow(SW_HIDE);

		GetDlgItem(IDC_STATIC_SHARES)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT_SHARES)->ShowWindow(SW_HIDE);

		GetDlgItem(IDC_STATIC_VALUE)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT_VALUE)->ShowWindow(SW_HIDE);

		GetDlgItem(IDC_STATIC_OPM)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_OPE)->ShowWindow(SW_HIDE);
		
		GetDlgItem(IDC_STATIC_FEE)->SetWindowText("���:");
	}


	//��ʼ������
	CFile file(g_rate,CFile::modeRead);
	file.Read(m_ShRate,sizeof(m_ShRate));
	file.Read(m_SzRate,sizeof(m_SzRate));
	//file.Read(m_CyRate,sizeof(m_CyRate));
	file.Close();


	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CEditInvest::OnClickTab(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	int nCurTab=this->m_TabCtrl.GetCurSel();
	switch(nCurTab)
	{
	case 0://��������
		this->m_SelectTab=0;

		GetDlgItem(IDC_RADIO_BUY)->SetWindowText("����");
		GetDlgItem(IDC_RADIO_SELL)->SetWindowText("����");

		GetDlgItem(IDC_STATIC_NAME)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_EDIT_SYMBOL)->ShowWindow(SW_SHOW);

		GetDlgItem(IDC_STATIC_PRICE)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_EDIT_PRICE)->ShowWindow(SW_SHOW);

		GetDlgItem(IDC_STATIC_SHARES)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_EDIT_SHARES)->ShowWindow(SW_SHOW);

		GetDlgItem(IDC_STATIC_VALUE)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_EDIT_VALUE)->ShowWindow(SW_SHOW);

		GetDlgItem(IDC_STATIC_FEE)->SetWindowText("������(Ԫ):");
		GetDlgItem(IDC_EDIT_FEE)->ShowWindow(SW_SHOW);

		GetDlgItem(IDC_STATIC_OPM)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STATIC_OPE)->ShowWindow(SW_SHOW);

		break;
	case 1://�ʽ��ȡ
		this->m_SelectTab=1;

		GetDlgItem(IDC_RADIO_BUY)->SetWindowText("����");
		GetDlgItem(IDC_RADIO_SELL)->SetWindowText("��ȡ");

		GetDlgItem(IDC_STATIC_NAME)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT_SYMBOL)->ShowWindow(SW_HIDE);

		GetDlgItem(IDC_STATIC_PRICE)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT_PRICE)->ShowWindow(SW_HIDE);

		GetDlgItem(IDC_STATIC_SHARES)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT_SHARES)->ShowWindow(SW_HIDE);

		GetDlgItem(IDC_STATIC_VALUE)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT_VALUE)->ShowWindow(SW_HIDE);

		GetDlgItem(IDC_STATIC_OPM)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_OPE)->ShowWindow(SW_HIDE);

		GetDlgItem(IDC_STATIC_FEE)->SetWindowText("���(Ԫ):");
		break;
	}
	
	*pResult = 0;
}

void CEditInvest::OnOK() 
{
	UpdateData(1);

	CString szPrice,szShares,szFee;
	m_Price.GetWindowText(szPrice);
	m_Shares.GetWindowText(szShares);
	m_Fee.GetWindowText(szFee);

	GetDlgItem(IDC_STATIC_NAME)->GetWindowText(m_Name);//����
	m_RefPrice=atof(szPrice);//�۸�
	m_RefShares=atof(szShares);//�ɽ���
	m_RefFee=atof(szFee);//������
	m_RefTime=m_time;//ʱ��
	m_RefIn=((CButton*)GetDlgItem(IDC_RADIO_BUY))->GetCheck() ? 1:-1;

	CDialog::OnOK();
}

//���ü��̾���
BOOL CEditInvest::PreTranslateMessage(MSG* pMsg) 
{
	if( GetFocus() == GetDlgItem(IDC_EDIT_SYMBOL) && pMsg->message==WM_CHAR)
	{
		if ((pMsg->wParam>='0'&&pMsg->wParam<='9') || (pMsg->wParam>='A'&&pMsg->wParam<='z'))
		{

			CTaiKeyBoardAngelDlg KeyboardWizard;
				
			KeyboardWizard.input=pMsg->wParam;

			KeyboardWizard.ischar= ( pMsg->wParam>='0' && pMsg->wParam<='9' )? false:true;

			//KeyboardWizard.DoModal();
			
			if( KeyboardWizard.DoModal()==IDOK && KeyboardWizard.isresultstock && KeyboardWizard.isresultok)
			{
				m_Symbol=KeyboardWizard.result;
				m_nKind=KeyboardWizard.m_stkKind;
				CReportData* pDat;
		 		if ( (CMainFrame::m_pDoc)->m_sharesInformation.Lookup(m_Symbol.GetBuffer(0) , pDat,m_nKind) )     //���ù�Ʊ�Ƿ��Ѽ�¼���ڴ�ģ����
				{
					GetDlgItem(IDC_EDIT_SYMBOL)->SetWindowText(m_Symbol);
					GetDlgItem(IDC_STATIC_NAME)->SetWindowText( pDat->name);
					//CString str;
					//str.Format("%.3f",pDat->nowp);
					//GetDlgItem(IDC_EDIT_PRICE)->SetWindowText( str );
				}
				return TRUE;
			}
			else
				return TRUE;
		}
		else
		{
			if ((pMsg->wParam = 8) || (pMsg->wParam = 13))
				return CDialog::PreTranslateMessage(pMsg);
			else
				return true;
		}
	}
	else
		return CDialog::PreTranslateMessage(pMsg);
}

void CEditInvest::OnChangeEditShares() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	this->OnChangeEditPrice();
	
}

void CEditInvest::OnChangeEditPrice() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	if( m_bInit==FALSE )
		return;
	
	CString szPrice,szShares,szValue,szFee;
	m_Price.GetWindowText(szPrice);
	m_Shares.GetWindowText(szShares);

	enum StockType{AShare,BShare,Bond,Fund,InvestFund};//��Ʊ����
	enum RateType{Tax,Comm,MiniComm,Fee,MiniFee};//��������

	if( szPrice =="" || szShares=="" )
		return;

	//�Զ�������
	szValue.Format("%.2f",  atof(szPrice)*atof(szShares) );
	m_Value.SetWindowText(szValue);

	//�Զ�����������
	CReportData* pDat;
	m_RefFee=0.0;
	double temp=atof(szPrice)*atof(szShares);
	if ( (CMainFrame::m_pDoc)->m_sharesInformation.Lookup(m_Symbol.GetBuffer(0), pDat,m_nKind) )     //���ù�Ʊ�Ƿ��Ѽ�¼���ڴ�ģ����
	{
		switch(pDat->kind)
		{
		case 0://��ָ֤��
			//m_RefFee=atof(szPrice)*atof(szShares)*0.075;
			break;
		case 1://��A
			if( m_Symbol[0]=='5')//����
			{
				m_RefFee+=temp*m_ShRate[Fund][Tax];
				m_RefFee+=temp*m_ShRate[Fund][Comm]>m_ShRate[Fund][MiniComm]? temp*m_ShRate[Fund][Comm]:m_ShRate[Fund][MiniComm];
				m_RefFee+=temp*m_ShRate[Fund][Fee] >m_ShRate[Fund][MiniFee] ? temp*m_ShRate[Fund][Fee] :m_ShRate[Fund][MiniFee];
			}
			m_RefFee+=temp*m_ShRate[AShare][Tax];
			m_RefFee+=temp*m_ShRate[AShare][Comm]>m_ShRate[AShare][MiniComm]? temp*m_ShRate[AShare][Comm]:m_ShRate[AShare][MiniComm];
			m_RefFee+=temp*m_ShRate[AShare][Fee] >m_ShRate[AShare][MiniFee] ? temp*m_ShRate[AShare][Fee] :m_ShRate[AShare][MiniFee];
			break;
		case 2://��B
			m_RefFee+=temp*m_ShRate[BShare][Tax];
			m_RefFee+=temp*m_ShRate[BShare][Comm]>m_ShRate[BShare][MiniComm]? temp*m_ShRate[BShare][Comm]:m_ShRate[BShare][MiniComm];
			m_RefFee+=temp*m_ShRate[BShare][Fee] >m_ShRate[BShare][MiniFee] ? temp*m_ShRate[BShare][Fee] :m_ShRate[BShare][MiniFee];
			break;
		case 3://��ָ
			break;
		case 4://��A
			m_RefFee+=temp*m_SzRate[AShare][Tax];
			m_RefFee+=temp*m_SzRate[AShare][Comm]>m_SzRate[AShare][MiniComm]? temp*m_SzRate[AShare][Comm]:m_SzRate[AShare][MiniComm];
			m_RefFee+=temp*m_SzRate[AShare][Fee] >m_SzRate[AShare][MiniFee] ? temp*m_SzRate[AShare][Fee] :m_SzRate[AShare][MiniFee];
			break;
		case 5://��B
			m_RefFee+=temp*m_SzRate[BShare][Tax];
			m_RefFee+=temp*m_SzRate[BShare][Comm]>m_SzRate[BShare][MiniComm]? temp*m_SzRate[BShare][Comm]:m_SzRate[BShare][MiniComm];
			m_RefFee+=temp*m_SzRate[BShare][Fee] >m_SzRate[BShare][MiniFee] ? temp*m_SzRate[BShare][Fee] :m_SzRate[BShare][MiniFee];
			break;
		case 6://��ծ
			m_RefFee+=temp*m_ShRate[Bond][Tax];
			m_RefFee+=temp*m_ShRate[Bond][Comm]>m_ShRate[Bond][MiniComm]? temp*m_ShRate[Bond][Comm]:m_ShRate[Bond][MiniComm];
			m_RefFee+=temp*m_ShRate[Bond][Fee] >m_ShRate[Bond][MiniFee] ? temp*m_ShRate[Bond][Fee] :m_ShRate[Bond][MiniFee];
			break;
		case 7://��ծ
			m_RefFee+=temp*m_SzRate[Bond][Tax];
			m_RefFee+=temp*m_SzRate[Bond][Comm]>m_SzRate[Bond][MiniComm]? temp*m_SzRate[Bond][Comm]:m_SzRate[Bond][MiniComm];
			m_RefFee+=temp*m_SzRate[Bond][Fee] >m_SzRate[Bond][MiniFee] ? temp*m_SzRate[Bond][Fee] :m_SzRate[Bond][MiniFee];
			break;
		case 8://��ָ
			break;
		case 9://����
			break;
		default:
			m_RefFee=0.0075;
			break;
		}
	}

	szFee.Format("%.2f",m_RefFee);
	m_Fee.SetWindowText(szFee);
}

void CEditInvest::OnChangeEditSymbol() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	this->OnChangeEditPrice();

}
