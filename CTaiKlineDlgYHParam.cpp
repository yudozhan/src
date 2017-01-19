// CTaiKlineDlgYHParam.cpp : implementation file
//

#include "stdafx.h"
#include "WH.h"
#include "CTaiKlineDlgYHParam.h"
#include "CTaiKlineFileHS.h"
#include "MainFrm.h"
#include "CTaiShanDoc.h"
#include "CTaiKlineDlgStopLose.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTaiKlineDlgYHParam dialog
ParamSetting CTaiKlineDlgYHParam::m_gParamSetting;


CTaiKlineDlgYHParam::CTaiKlineDlgYHParam(CWnd* pParent /*=NULL*/)
	: CDialog(CTaiKlineDlgYHParam::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTaiKlineDlgYHParam)
	m_bAuto = FALSE;
	m_nRepeate = 5;
	m_sResult = _T("");
	m_nRange = 20;
	//}}AFX_DATA_INIT

	m_bCaclSucc = false;
	m_bCalcAuto = false;
	m_pList = NULL;
	m_pKline = NULL;
	m_nKline = 0;
	m_pIndex = NULL;
	m_nKlineKind2 = 5;
	m_bContinueCacl = false;

	LOGFONT m_lf;
	memset(&m_lf,0,sizeof(LOGFONT));
	m_lf.lfHeight = -13;
	m_lf.lfWidth=0;
	m_lf.lfWeight=400;
	m_lf.lfCharSet=134;
	strcpy(m_lf.lfFaceName,"����");
	VERIFY(m_Font.CreateFontIndirect(&m_lf));

	//read 
	CFile fl;
	if(fl.Open("data\\ParamSetting.par",CFile::modeCreate |CFile::modeNoTruncate  
		|  CFile::modeReadWrite   |CFile::shareDenyNone   ))
	{
		fl.Read(&m_gParamSetting,sizeof(ParamSetting));
		fl.Close ();
	}
	if(m_gParamSetting.m_nRange >0)
		this->m_nRange = m_gParamSetting.m_nRange;
	if(m_gParamSetting.m_nRepeate >0)
		this->m_nRepeate = m_gParamSetting.m_nRepeate;
//	m_bAuto = m_gParamSetting.m_bAuto;

	bCancel = false;
}


void CTaiKlineDlgYHParam::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTaiKlineDlgYHParam)
	DDX_Control(pDX, IDOK, m_conOk);
	DDX_Control(pDX, IDCANCEL, m_conCanc);
	DDX_Control(pDX, IDC_BUTTON1, m_conButt1);
	DDX_Control(pDX, IDC_EDIT7, m_con7);
	DDX_Control(pDX, IDC_EDIT1, m_con1);
	DDX_Control(pDX, IDC_PROGRESS1, m_prg);
	DDX_Control(pDX, IDC_LIST1, m_param_list);
	DDX_Check(pDX, IDC_CHECK1, m_bAuto);
	DDX_Text(pDX, IDC_EDIT1, m_nRepeate);
	DDV_MinMaxUInt(pDX, m_nRepeate, 2, 1000);
	DDX_Text(pDX, IDC_EDIT2, m_sResult);
	DDX_Text(pDX, IDC_EDIT7, m_nRange);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTaiKlineDlgYHParam, CDialog)
	//{{AFX_MSG_MAP(CTaiKlineDlgYHParam)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BUTTON1, OnButtonAdditional)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST1, OnDblclkListParam)
	ON_NOTIFY(NM_CLICK, IDC_LIST1, OnClickListParam)
	ON_WM_CLOSE()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTaiKlineDlgYHParam message handlers

void CTaiKlineDlgYHParam::OnDestroy() 
{

	CDialog::OnDestroy();
	// TODO: Add your message handler code here
	
}

BOOL CTaiKlineDlgYHParam::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_param_list.SetExtendedStyle(m_param_list.GetExtendedStyle()|LVS_EX_GRIDLINES|LVS_EX_FULLROWSELECT|LVS_EX_INFOTIP);
	m_param_list.InsertColumn(0,"������",LVCFMT_LEFT,85);
	m_param_list.InsertColumn(1,"����",LVCFMT_LEFT,42);
	m_param_list.InsertColumn(2,"��С",LVCFMT_LEFT,42);
	m_param_list.InsertColumn(3,"���",LVCFMT_LEFT,42);    
	m_param_list.InsertColumn(4,"�Ƿ��Ż�",LVCFMT_LEFT,85);    
	pEdit = NULL;

	CTaiScreenParent dlg(IDD_PARENT_TJXG);
	//get the count param that need to optimize
	CFormularContent*	pIndex;
	if(m_IndexSave.iKind >-1 || m_IndexSave.iKind <-4)
	{
		AfxMessageBox("����ָ�겻�ܽ��в����Ż���");
		return TRUE;
	}

	CTaiShanDoc * pDoc = CMainFrame::m_pDoc ;

	pIndex = m_pIndex;
	if(pIndex==NULL )
		pIndex=(CFormularContent*)(dlg.LookUpArray( this->m_IndexSave .iKind, this->m_IndexSave .sIndex_name,pDoc)); 
	if(pIndex==NULL )
	{
		AfxMessageBox("ϵͳ�Ҳ�����ָ�꣡");
		return TRUE;
	}
	m_pIndex = pIndex;
	
	//
	m_param_list.DeleteAllItems();
	for(int i=0;i<m_pIndex->numPara;i++)
	{
	  m_param_list.InsertItem(i,m_pIndex->namePara[i]);

	//	CFormularContent::GetParamDataEach(i,m_nKlineKind2, m_pIndex);

	  CString temp = CLongString::FloatToString(m_pIndex->stepLen [i]);

      m_param_list.SetItemText(i,1,temp);
	  temp = CLongString::FloatToString(m_pIndex->min[i]);
      m_param_list.SetItemText(i,2,temp);
	  temp = CLongString::FloatToString(m_pIndex->max[i]);
      m_param_list.SetItemText(i,3,temp);

	  if(m_pIndex->bYHParam [i]==0)
	  {
			temp = "��";
	  }
	  else
	  {
			temp = "��";
	  }
	  m_param_list.SetItemText (i,4,temp);
	}
	
	SetResultText();

	UpdateData(FALSE);

	if(m_bCalcAuto == true)
	{
		GetDlgItem(IDC_CHECK1)->EnableWindow(FALSE);
		SetTimer(1,100,NULL);
	}
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CTaiKlineDlgYHParam::OnOK() 
{
	// TODO: Add extra validation here
	if(!UpdateData()) return ;


	SaveSetting1();
	if(this->m_pIndex )
	{
		if(this->m_IndexSave.iKind <0 && this->m_IndexSave.iKind > -4) 
			CFormularContent::ReadWriteIndex(CMainFrame::m_pDoc ,-this->m_IndexSave.iKind -1, false);
	}
	//write 
	CFile fl;
	if(fl.Open("data\\ParamSetting.par",CFile::modeCreate |CFile::modeNoTruncate  
		|  CFile::modeReadWrite   |CFile::shareDenyNone   ))
	{
		fl.Write(&m_gParamSetting,sizeof(ParamSetting));
		fl.Close ();
	}


	//CDialog::OnOK();
}

void CTaiKlineDlgYHParam::OnCancel() 
{
	// TODO: Add extra cleanup here
	bCancel = true;
	CString s;
	GetDlgItem(IDC_BUTTON1)->GetWindowText(s);
	if(s == "ȡ������")
	{
//		m_bContinueCacl = false;
		AfxMessageBox("����ֹͣ���㣡");
		return;
	}
	
	CDialog::OnCancel();
}

void CTaiKlineDlgYHParam::OnButtonAdditional() 
{
	// TODO: Add your control notification handler code here
	if(!UpdateData()) return ;

	this->m_bAuto = TRUE;
	CString s;
	m_bContinueCacl = true;
	GetDlgItem(IDC_BUTTON1)->GetWindowText(s);
	if(s == "ȡ������")
		m_bContinueCacl = false;
	else
	{
		s = "ȡ������";
		GetDlgItem(IDC_BUTTON1)->SetWindowText(s);
	}

	SaveSetting1();

	s = "";
	if(OnCalc() == false)
	{
//		m_bContinueCacl = true;
		m_bCaclSucc = false;
		this->m_bAuto = FALSE;
		m_sResult = "�����Ѿ�ȡ��!";
	}
	else
	{
		m_bCaclSucc = true;
		s = m_sResult ;
		SetResultText();
		m_sResult =m_sResult +s;
	}

	if(::IsWindow(m_hWnd))
	{
		m_prg.SetPos (0);
		UpdateData(FALSE);

		s = "�����Ż�����";
		GetDlgItem(IDC_BUTTON1)->SetWindowText(s);
	}

/*	//it maybe use if the future
	if(m_pIndex ==NULL) return;
	if(m_pIndex->pAdditionalBS ==NULL)
	{
		m_pIndex->pAdditionalBS = new ADDITIONAL_BUYSELL;
		CTaiScreenParent dlg(IDD_PARENT_TJXG);
		memcpy(m_pIndex->pAdditionalBS ,&(dlg.buySellAdditional),sizeof(ADDITIONAL_BUYSELL));
	}

	CTaiKlineDlgStopLose DlgInsert(NULL);
	DlgInsert.SetAdditional ((*m_pIndex->pAdditionalBS));
	if(DlgInsert.DoModal() == IDOK)
		DlgInsert.GetAdditional ((*m_pIndex->pAdditionalBS));
*/
	if(m_bCalcAuto == true)
		this->m_bAuto = FALSE;

}

void CTaiKlineDlgYHParam::OnDblclkListParam(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	
	*pResult = 0;
}

void CTaiKlineDlgYHParam::OnClickListParam(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	
	int nItem = 0;
	////////Get the selected List Item/////////
	POSITION pos = m_param_list.GetFirstSelectedItemPosition();
    if(pos == NULL)
	{
		TRACE0("No items were selected!\n");
		return;
	}
    else
	{
      while (pos)
	  {
        nItem = m_param_list.GetNextSelectedItem(pos);
        TRACE1("Item %d was selected!\n", nItem);
	  }
	}
	CRect rect;
	CRect list_rect;  ///////the rect of list ctrl////
	
	m_param_list.GetSubItemRect(nItem,1,LVIR_BOUNDS,rect);
	m_param_list.GetWindowRect(list_rect);
	POINT pt;
	CRect diaRect;
	this->GetWindowRect(diaRect);
	pt.x = rect.left + list_rect.left - diaRect.left;
	pt.y = rect.top + list_rect.top - diaRect.top - GetSystemMetrics(SM_CYCAPTION);
	SIZE sz;
	sz.cx = rect.Width();
	sz.cy = rect.Height();
	CRect newrect(pt,sz);
	
	POINT pt2;
	::GetCursorPos (&pt2);
	CRect rt;
	m_param_list.GetSubItemRect(nItem,1,LVIR_BOUNDS,rt);
	m_param_list.ClientToScreen (rt);
	char str[20];
	if(rt.PtInRect (pt2))
	{
		m_param_list.GetItemText(nItem,1,str,20);
	//	m_param_list.SetItemText(nItem,1,"");
		pEdit = new CParamEdit(&m_param_list,nItem);
		pEdit->m_bTestMax = false;
		///////////we make the Edit Box Bigger/////////
		pEdit->Create(ES_LEFT|WS_VISIBLE|WS_VISIBLE|WS_TABSTOP|WS_BORDER,newrect,this,1234);
		pEdit->SetFont(&m_Font);
		pEdit->SetWindowText(str);
		pEdit->SetSel(0,-1);
		pEdit->SetFocus(); 
	}
	else
	{
		m_param_list.GetSubItemRect(nItem,4,LVIR_BOUNDS,rt);
		m_param_list.ClientToScreen (rt);
		if(rt.PtInRect (pt2))
		{
			m_param_list.GetItemText (nItem,4,str,20);
			CString s = str;
			if(s == "��")//��
			{
				s = "��";
				m_param_list.SetItemText (nItem,4,s);
			}
			else
			{
				s = "��";
				m_param_list.SetItemText (nItem,4,s);
			}
		}
	}

	*pResult = 0;
}

void CTaiKlineDlgYHParam::ReadSetting()
{

}

void CTaiKlineDlgYHParam::SaveSetting()
{

}
//	Init(IndexDataInfo* pIndexSave,	Kline*	pKline,	int m_nKline,CFormularContent*	pIndex,	int m_nKlineKind2,CBuySellList *m_pList ,CStringArray* stockArray)

//	if(pIndex==NULL)//ѡ�� else //k�߽���(pIndexSave = NULL)
bool CTaiKlineDlgYHParam::Init(IndexDataInfo *pIndexSave, Kline *pKline, int nKline, CFormularContent *pIndex, int kLineType, CBuySellList *pList, SymbolKindArr *stockArray)
{
	if(pIndexSave)
		m_IndexSave = *pIndexSave;
	else
		m_IndexSave.iKind  = -1;
	m_pKline = pKline;
	m_nKline = nKline;
	m_pIndex = pIndex;
	m_nKlineKind2 = kLineType;
	m_pList = pList;
	m_stockArray.Copy (*stockArray);

	if(m_IndexSave.iKind >-1 || m_IndexSave.iKind <-4)
	{
		AfxMessageBox("����ָ�겻�ܽ��в����Ż���");
		return false;
	}

	CTaiShanDoc * pDoc = CMainFrame::m_pDoc ;

	if(pIndex==NULL)//ѡ��
	{
		if(m_pIndex==NULL )
			m_pIndex=(CFormularContent*)(CTaiScreenParent::LookUpArray( this->m_IndexSave .iKind, this->m_IndexSave .sIndex_name,pDoc)); 
		if(m_pIndex==NULL )
		{
			AfxMessageBox("ϵͳ�Ҳ�����ָ�꣡");
			return false;
		}
		m_nKlineKind2 =CTaiScreenParent::nLKind[ m_IndexSave.iDataType ];
	}
	else//k�߽���ʱ
	{
		int nLine;
		int nRtn = GetIndexLineType(pIndex,nLine);
		if(nRtn==0)
		{
			AfxMessageBox("��ָ��û�������������������������������ѡ������");
			return false;
		}
		m_IndexSave.iLine = nLine;
		m_IndexSave.iDataType  = CTaiScreenParent::FromKlineKindToFoot(kLineType);
		if(nRtn ==1)
			m_IndexSave.bBuyCase = 0;
		else
			m_IndexSave.bBuyCase = 1;
		m_IndexSave.iBeginFoot = 1500;
		m_IndexSave.iSunIndex =0;
		m_IndexSave.fSubParam1 = 0;
		m_IndexSave.fSubParam2 = 0;
		strcpy(m_IndexSave.sIndex_name,m_pIndex->name);

	}
	if(m_pIndex->bYH !=0)
		this->m_bAuto = TRUE;
	else
		this->m_bAuto = FALSE;

	return true;

}

bool CTaiKlineDlgYHParam::OnCalc()
{
	if(m_bContinueCacl == false) return false;

	CFormularContent*	pIndex;
	pIndex = m_pIndex;
	if(pIndex==NULL )
	{
		AfxMessageBox("ϵͳ�Ҳ�����ָ�꣡");
		return false;
	}
	if(pIndex->bYH ==0) 
	{

		AfxMessageBox("��ָ�겻�����Ż�������");
		return false;
	}

	CTaiScreenParent dlg(IDD_PARENT_TJXG);
	int i;
	for(i=0;i<m_pIndex->numPara;i++)
	{
		if(m_pIndex->min[i]<=m_pIndex->defaultValYH[i] && m_pIndex->max[i]>=m_pIndex->defaultValYH[i] )
			m_IndexSave.fParam [i] = m_pIndex->defaultValYH[i] ;
		else
			m_IndexSave.fParam [i] = m_pIndex->defaultVal [i];
	}

	dlg.m_indexSaveArray.Add(this->m_IndexSave );

	//get the count param that need to optimize
	if(m_IndexSave.iKind >-1 || m_IndexSave.iKind <-4)
	{
		AfxMessageBox("����ָ�겻�ܽ��в����Ż���");
		return false;
	}

	CTaiShanDoc * pDoc = CMainFrame::m_pDoc ;

	if(pIndex->pAdditionalBS !=NULL)
	{
		memcpy(&(dlg.buySellAdditional),pIndex->pAdditionalBS ,sizeof(ADDITIONAL_BUYSELL));
	}

	int nCount = 0;
	for(i=0;i<pIndex->numPara ;i++)
	{
		if(pIndex->bYHParam [i]) nCount++;
	}
	if(nCount==0 )
	{
		AfxMessageBox("û����Ҫ�Ż��Ĳ�����");
		return false;
	}

	nCount = 0;
	for(i=0;i<pIndex->numPara ;i++)
	{
		if(pIndex->stepLen [i]<=0) 
		{
			pIndex->stepLen [i] = 1;
			CString temp = "1";
			m_param_list.SetItemText(i,1,temp);
		}
		if(pIndex->bYHParam [i] && pIndex->stepLen [i]>0) 
		{
			nCount+=((pIndex->max [i]-pIndex->min [i])/ pIndex->stepLen [i]);
		}
	}

	if(m_stockArray.GetSize()==0)
	{
		AfxMessageBox("û��Ҫ�Ż������Ĺ�Ʊ�����ȼ����Ʊ��");
		return false;
	}
	if(m_stockArray.GetSize()>10)
	{
		if(AfxMessageBox("��Ʊ��������10�������ʹ����ʱ��ܳ����Ƿ������",MB_YESNO) == IDNO)
			return false;
	}
	//0 step:���ò�ͬ����
	int nMax = m_gParamSetting.m_nRepeate ;
	int ns = m_stockArray.GetSize ();

	CBuySellList *pList = this->m_pList ;
	CBuySellList bsList;
	if(m_pList==NULL) pList = &bsList;

	int nKlineMax = 1500;//???

	m_sResult = "";//���ڽ����Ż�����...\r\n";
	UpdateData(FALSE);
//	float fParam[8];
//	for(int j=0;j<pIndex->numPara;j++)
//		fParam[j]=pIndex->defaultVal [j];
	float fMaxProfit = 0;
	int j = 0;
	int nPos = 0;
//	for(int k=0;k<ns;k++)
	{
		Kline *pKline = this->m_pKline ;
		int nCount2 = 0;

		this->m_prg .SetRange32 (0,nCount);

		for(i=0;i<nMax;i++)
		{
			m_prg.SetPos (0);
			for( j=0;j<pIndex->numPara;j++)
			{
				if(pIndex->bYHParam [j] == 0 || pIndex->stepLen [j] == 0) continue;
				float ff = pIndex->min [j];
				float maxf = pIndex->max [j];
				float fParam = dlg.m_indexSaveArray[0].fParam[j];

				if(i>0)//���ټ������
				{
					int nCacl = (maxf-ff)/pIndex->stepLen [j];

					maxf = fParam + pIndex->stepLen [j]*(int)((float(nMax-i)/(3*nMax)+0.1)*nCacl);
					if(maxf> pIndex->max [j])
						maxf = pIndex->max [j];
					if(maxf< pIndex->min [j])
						maxf = pIndex->max [j];
					ff = fParam - pIndex->stepLen [j]*(int)((float(nMax-i)/(3*nMax)+0.1)*nCacl);
					if(ff< pIndex->min [j])
						ff = pIndex->min [j];
					if(ff> pIndex->max [j])
						ff = pIndex->max [j];
				}
				if(ff<pIndex->min [j])
					ff = pIndex->min [j];
				for(;ff<maxf && ff >= pIndex->min [j];ff+=pIndex->stepLen [j])
				{

					dlg.m_indexSaveArray[0].fParam[j] = ff;

					nPos ++;
					m_prg.OffsetPos (1);

					//1 step:����ָ����������
					for(int k=0;k<ns;k++)
					{
						if(m_bContinueCacl == false)
						{
							CTaiKlineFileHS::RemoveHs(bsList);
							if(pKline && m_pKline == NULL)
							{
								delete[] pKline;        //�ͷ��ڴ�
								pKline=NULL;
							}
							return false;
						}

						CString s = this->m_stockArray [k].m_chSymbol;
						if(this->m_pKline == NULL)//read now
						{
							nCount2=dlg.ReadKLine( s,this->m_stockArray [k].m_nSymbolKind ,pKline,nKlineMax,dlg.nLKind[m_IndexSave.iDataType ],pList,48,TRUE);
						}
						else
							nCount2 = this->m_nKline ;
						dlg.TestSuccessEach(s,this->m_stockArray [k].m_nSymbolKind, nCount2, pKline, m_gParamSetting.m_nRange , 0.1, pList);
					}
					if(m_bContinueCacl == false)
					{
							CTaiKlineFileHS::RemoveHs(bsList);
							if(pKline && m_pKline == NULL)
							{
								delete[] pKline;        //�ͷ��ڴ�
								pKline=NULL;
							}
							return false;
					}

					//2 step:����ƽ������
					float fRateSucc=0;
					int nBuy = 0;
					float fProf = dlg.GetProfitAverage(fRateSucc,nBuy);//fProfƽ�����棬fRateSuccΪӯ��ϵ��

					if(fMaxProfit*1.001 < fProf *fRateSucc && nBuy>2)
					{
						fMaxProfit = fProf *fRateSucc;
						pIndex->defaultValYH [j] = ff;

						//��ʾ
						CString s="������";
						CString tmp;
						for(int ii= 0;ii<pIndex->numPara;ii++)
						{
							if(ii == j)
								tmp = CLongString::FloatToString(ff);
							else
								tmp = CLongString::FloatToString(dlg.m_indexSaveArray[0].fParam[ii]);
							s += (tmp+" ");
						}
						s+="\r\n";
						tmp.Format ("ƽ�����棺%.2f ӯ��ϵ����%d ���״�����%d\r\n",fProf,(int)fRateSucc,nBuy);
						s+=tmp;
						m_sResult += s;
						UpdateData(FALSE);
						if(m_bContinueCacl == false)
						{
							CTaiKlineFileHS::RemoveHs(bsList);
							if(pKline && m_pKline == NULL)
							{
								delete[] pKline;        //�ͷ��ڴ�
								pKline=NULL;
							}
							return false;
						}

					}

					//3 step:
					dlg.ClearTestResultArray();
				}

				dlg.m_indexSaveArray[0].fParam[j] = pIndex->defaultValYH [j] ;
			}
		}
		CTaiKlineFileHS::RemoveHs(bsList);
		if(pKline && m_pKline == NULL)
		{
			delete[] pKline;        //�ͷ��ڴ�
			pKline=NULL;
		}
	}
	

	return true;
	//CDialog::OnOK();

}

void CTaiKlineDlgYHParam::SaveSetting1()
{
	CFormularContent*	pIndex;
	pIndex = m_pIndex;
	if(pIndex==NULL )
	{
		return ;
	}

	pIndex->bYH = 0;
	if(this->m_bAuto == TRUE)
		pIndex->bYH = 1;

	//�ѽ����ϵ�����д�뵽ָ�����m_pIndex��
	for(int i=0;i<pIndex->numPara ;i++)
	{
		CString s;
		s = m_param_list.GetItemText (i,1);
		pIndex->stepLen [i] = atof(s);
		if(pIndex->stepLen [i]>(pIndex->max [i]-pIndex->min[i])/2)
			pIndex->stepLen [i] = (pIndex->max [i]-pIndex->min[i])/2;

		s = m_param_list.GetItemText (i,4);
		pIndex->bYHParam [i] = 1 ;
		if(s == "��")//��
			pIndex->bYHParam [i] = 0 ;
	}
	m_gParamSetting.m_nRange = m_nRange;
	m_gParamSetting.m_nRepeate = m_nRepeate;
//	m_gParamSetting.m_bAuto = m_bAuto;
	;
	//end �ѽ����ϵ�����д�뵽ָ�����m_pIndex��


}

void CTaiKlineDlgYHParam::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	bCancel = true;
	CString s;
	GetDlgItem(IDC_BUTTON1)->GetWindowText(s);
	if(s == "ȡ������")
	{
//		m_bContinueCacl = false;
		AfxMessageBox("����ֹͣ���㣡");
		return;
	}
	
	CDialog::OnClose();
}

//return : 0 = û�����롢��������;1 = �����ҵ���������;2 = �����ҵ���������;
int CTaiKlineDlgYHParam::GetIndexLineType(CFormularContent *pIndex,int &nLine)
{
	Kline nKline[20];
	nLine = 0;
	CFormularCompute nEquation(0,19,nKline,pIndex,CSharesCompute::GetIndexSymbol(0) ,SHZS,NULL,5 );
	int numPa=pIndex->numPara ;
	for(int j=0;j<numPa;j++)
	{
		nEquation.AddPara (pIndex->namePara [j],pIndex->defaultVal [j]);
	}
	nEquation.Devide();
	for(int i=0;i<nEquation.GetRlineNum ();i++)
	{
		int nLineType = nEquation.m_RlineType[i];
		if(nLineType == 107)
		{
			nLine = i;
			return 1;
		}
		if(nLineType == 108)
		{
			nLine = i;
			return 2;
		}
	}
	return 0;
}

void CTaiKlineDlgYHParam::SetResultText()
{
	if(m_pIndex!=NULL && this->m_bAuto == TRUE)
	{
		this->m_sResult = "�Ż���Ĳ���Ϊ��\r\n" ;
		for(int i=0;i<m_pIndex->numPara;i++)
		{
			CString s = m_pIndex->namePara [i]+":";
			float f= m_pIndex->defaultValYH [i];
			if(f>m_pIndex->max[i] || f<m_pIndex->min[i]) m_pIndex->defaultValYH [i] = m_pIndex->defaultVal [i];
			CString temp = CLongString::FloatToString(m_pIndex->defaultValYH [i]);
			s+=(temp+"  ");
			m_sResult+= s;
		}
		m_sResult+="\r\n";
	}
	else
	{
		m_sResult = "�����úø�������󣬵����ִ���Ż����㡱��ť�����Ż�������";
	}

}

BOOL CTaiKlineDlgYHParam::CalcYH(IndexDataInfo *pIndexSave, SymbolKindArr *stockArray)
{
	CTaiKlineDlgYHParam dlg;
	dlg.m_bCalcAuto = true;
	if(dlg.Init(pIndexSave, NULL, 0, NULL, 5, NULL,stockArray) == false)
		return FALSE;
	dlg.DoModal ();
	if(dlg.m_bCaclSucc == true)
	{
		if(AfxMessageBox("�Ƿ�ʹ���Ż���Ĳ�����",MB_YESNO) == IDYES)
		{
			if(dlg.m_pIndex == NULL) return FALSE;
			for(int i=0;i<dlg.m_pIndex->numPara;i++)
			{
				pIndexSave->fParam [i] = dlg.m_pIndex->defaultValYH[i] ;
			}
			return TRUE;
		}
	}
	return FALSE;
}

void CTaiKlineDlgYHParam::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	if(nIDEvent == 1)
	{
		KillTimer(1);
		OnButtonAdditional() ;
	}
	CDialog::OnTimer(nIDEvent);
}
