// CTaiKlineDlgChangeIndexParam.cpp : implementation file
//

#include "stdafx.h"
#include "WH.h"
#include "CTaiKlineDlgChangeIndexParam.h"
#include "MainFrm.h"
#include "CFormularCompute.h"
#include "CTaiKlineDoKline.h"
#include "CTaiScreenParent.h"
#include "MainFrm.h"
#include "CTaiShanDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTaiKlineDlgChangeIndexParam dialog

static int g_controlID[3][8] = 
			{{IDC_STATICP1,IDC_STATICP2,IDC_STATIC_P3,IDC_STATIC_P4,IDC_STATICP4,IDC_STATICP3,IDC_STATIC_P5,IDC_STATIC_P6},
			{IDC_EDIT_N,IDC_EDIT_LL,IDC_EDIT_P3_P,IDC_EDIT_LL3_P4,
			IDC_EDIT_N2,IDC_EDIT_LL2,IDC_EDIT_P3_P2,IDC_EDIT_LL3_P5},
			{IDC_SPIN1,IDC_SPIN2,IDC_SPIN_P3,IDC_SPIN_P4,IDC_SPIN3,IDC_SPIN4,IDC_SPIN_P5,IDC_SPIN_P6}};
static CString sMax = "所设参数数值超过最大值，以最大值代替。";
static CString sMin = "所设参数数值超过最小值，以最小值代替。";
CTaiKlineDlgChangeIndexParam::CTaiKlineDlgChangeIndexParam(CWnd* pParent /*=NULL*/)
	: CDialog(CTaiKlineDlgChangeIndexParam::IDD, pParent)
{
	pView = (CTaiShanKlineShowView* )pParent;
	ASSERT(pView!=NULL);

	m_bInitialed=false;
//	m_nPeriod = 5;
	//{{AFX_DATA_INIT(CTaiKlineDlgChangeIndexParam)
	m_bSaveAllPeriod = TRUE;
	//}}AFX_DATA_INIT
}


void CTaiKlineDlgChangeIndexParam::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTaiKlineDlgChangeIndexParam)
	DDX_Control(pDX, IDOK, m_ok);
	DDX_Control(pDX, IDC_BUTTON_SET, m_set);
	DDX_Control(pDX, IDC_BUTTON_RESTORE, m_restore);
	DDX_Control(pDX, IDC_EDIT_P3_P2, m_floatEditP32);
	DDX_Control(pDX, IDC_EDIT_N2, m_floatEditN2);
	DDX_Control(pDX, IDC_EDIT_LL3_P5, m_floatEditP5);
	DDX_Control(pDX, IDC_EDIT_LL2, m_floatEditLl2);
	DDX_Control(pDX, IDC_STATICP2, m_cstaticp2);
	DDX_Control(pDX, IDC_STATICP1, m_cstaticp1);
	DDX_Control(pDX, IDC_STATIC_P4, m_cstaticp4);
	DDX_Control(pDX, IDC_STATIC_P3, m_cstaticp3);
	DDX_Control(pDX, IDC_SPIN_P4, m_cspin4);
	DDX_Control(pDX, IDC_SPIN_P3, m_cspin3);
	DDX_Control(pDX, IDC_SPIN2, m_cspin2);
	DDX_Control(pDX, IDC_SPIN1, m_cspin1);
	DDX_Control(pDX, IDC_EDIT_LL3_P4, m_controlp4);
	DDX_Control(pDX, IDC_EDIT_P3_P, m_controlp3);
	DDX_Control(pDX, IDC_EDIT_LL, m_controlp2);
	DDX_Control(pDX, IDC_EDIT_N, m_controlp1);
	DDX_Check(pDX, IDC_CHECK1, m_bSaveAllPeriod);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTaiKlineDlgChangeIndexParam, CDialog)
	//{{AFX_MSG_MAP(CTaiKlineDlgChangeIndexParam)
	ON_BN_CLICKED(IDC_BUTTON_RESTORE, OnButtonRestore)
	ON_BN_CLICKED(IDC_BUTTON_SET, OnClose)
	ON_WM_CLOSE()
	ON_WM_DESTROY()
	ON_EN_CHANGE(IDC_EDIT_N, OnChangeEditN)
	ON_EN_CHANGE(IDC_EDIT_LL, OnChangeEditLl2)
	ON_EN_CHANGE(IDC_EDIT_LL2, OnChangeEditLl22)
	ON_EN_CHANGE(IDC_EDIT_P3_P, OnChangeEditP3)
	ON_EN_CHANGE(IDC_EDIT_LL3_P4, OnChangeEditP4)
	ON_EN_CHANGE(IDC_EDIT_N2, OnChangeEditN2)
	ON_EN_CHANGE(IDC_EDIT_P3_P2, OnChangeEditP3P2)
	ON_EN_CHANGE(IDC_EDIT_LL3_P5, OnChangeEditLl3P5)
	ON_BN_CLICKED(IDC_CHECK1, OnCheckSaveParam)
	ON_EN_SETFOCUS(IDC_EDIT_LL, OnSetfocusEditLl)
	ON_EN_SETFOCUS(IDC_EDIT_LL2, OnSetfocusEditLl2)
	ON_EN_SETFOCUS(IDC_EDIT_LL3_P4, OnSetfocusEditLl3P4)
	ON_EN_SETFOCUS(IDC_EDIT_LL3_P5, OnSetfocusEditLl3P5)
	ON_EN_SETFOCUS(IDC_EDIT_N, OnSetfocusEditN)
	ON_EN_SETFOCUS(IDC_EDIT_N2, OnSetfocusEditN2)
	ON_EN_SETFOCUS(IDC_EDIT_P3_P, OnSetfocusEditP3P)
	ON_EN_SETFOCUS(IDC_EDIT_P3_P2, OnSetfocusEditP3P2)
	ON_WM_HELPINFO()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTaiKlineDlgChangeIndexParam message handlers

void CTaiKlineDlgChangeIndexParam::OnButtonRestore() 
{
	// TODO: Add your control notification handler code here
	if(m_pJishuNow==NULL)
		return;

	int para_count=m_pJishuNow->numPara;
	for(int i=0;i<para_count;i++)
	{
		float f=m_paramDefault[i];
		CString s;
		s.Format ("%f",f);
		if(s.Find (".")>=0)
		{
			s.TrimRight ("0");
			s.TrimRight (".");
		}
		GetDlgItem(g_controlID[1][i])->SetWindowText(s);
	}
/*	m_p1=m_paramDefault[0];
	m_p2=m_paramDefault[1];
	m_p3=m_paramDefault[2];
	m_p4=m_paramDefault[3];*/
	UpdateData(FALSE);
}

void CTaiKlineDlgChangeIndexParam::OnButtonSet() 
{
	// TODO: Add your control notification handler code here
	if(m_pJishuNow==NULL)
		return;

	UpdateData(TRUE);
	int para_count=m_pJishuNow->numPara;
	for(int i=0;i<para_count;i++)
	{
		float f;
		CString s;
		GetDlgItem(g_controlID[1][i])->GetWindowText(s);
		m_paramDefault[i] = atof(s);

		if(m_pJishuNow->max [i]<m_paramDefault[i])
		{
			AfxMessageBox(sMax);
			m_paramDefault[i] = m_pJishuNow->max [i];
		}
		if(m_pJishuNow->min [i]>m_paramDefault[i])
		{
			AfxMessageBox(sMin);
			m_paramDefault[i] = m_pJishuNow->min [i];
		}
	}
	CTaiShanDoc*   pDoc=((CMainFrame*)(AfxGetApp()->m_pMainWnd))->m_pDoc ;

	pDoc->m_propertyInitiate.bSaveParam = m_bSaveAllPeriod;
	CFormularContent::ReadWriteIndex(pDoc, 0, false);//write

	DestroyWindow();
}
void CTaiKlineDlgChangeIndexParam::UpdateParamArea()
{
	CTaiShanDoc*   pDoc=CMainFrame::m_pDoc ;
	
	bool bFinded=false;                   
	CFormularContent *jishunow=NULL;
	m_pJishuNow=NULL;
	if(m_nHit<0||m_nHit>4)
		return;
	CString name=pView->m_infoInit.initIndex[m_nHit].nameIndex;
	
	int i;
	for(i=0;i<pDoc->m_formuar_index.GetSize();i++)
	{
		jishunow=pDoc->m_formuar_index.GetAt(i);
		if(strcmp(name,jishunow->name )==0)
		{
			bFinded=true;
			m_pJishuNow=jishunow;
			break;
		}
	}
	if(m_pJishuNow==NULL)
		return;

	int para_count=jishunow->numPara;
	for(i=0;i<para_count;i++)
	{
		m_paramDefault[i]=CFormularContent::GetParamDataEach(i,pView->m_nKlineKind2,jishunow);
	}
	for(i=0;i<para_count;i++)
	{
		GetDlgItem(g_controlID[0][i])->SetWindowText (jishunow->namePara[i]);
		GetDlgItem(g_controlID[0][i])->ShowWindow (SW_SHOW);
		CString s ;
		float f ;//= jishunow->defaultVal[i];
		//使用保存各个周期数据
		f = CFormularContent::GetParamDataEach(i,pView->m_nKlineKind2,jishunow);

/*		if(m_bSaveAllPeriod == TRUE)
		{
			if(jishunow->defaultValArray .GetSize()>i)
			{
				int nInd = CTaiScreenParent::FromKlineKindToFoot(pView->m_nKlineKind2 );
				if(nInd>=0)
					f = jishunow->defaultValArray[i].fValue[nInd];
			}
		}*/

		s.Format ("%f",f);
		if(s.Find (".")>=0)
		{
			s.TrimRight ("0");
			s.TrimRight (".");
		}
		GetDlgItem(g_controlID[1][i])->SetWindowText (s);
		GetDlgItem(g_controlID[1][i])->ShowWindow (SW_SHOW);
		((CSpinButtonCtrl*)GetDlgItem(g_controlID[2][i]))->SetRange(jishunow->min[i] ,jishunow->max[i]);
		GetDlgItem(g_controlID[2][i])->ShowWindow (SW_SHOW);
	}

	//resize window
	CRect rDlg;
	GetWindowRect(rDlg);
	if(para_count>2)
	{
		CRect r;
		GetDlgItem(g_controlID[0][para_count-1])->GetWindowRect (r);
		rDlg.bottom = r.bottom + 15;
	}
	else
	{
		CRect r;
		GetDlgItem(IDC_BUTTON_RESTORE)->GetWindowRect (r);
		rDlg.bottom = r.bottom + 15;
	}
//	pView->ScreenToClient(rDlg);
	CRect r2(m_pt.x ,m_pt.y,m_pt.x+rDlg.Width (),m_pt.y+rDlg.Height ());
	MoveWindow(r2,FALSE);

}
BOOL CTaiKlineDlgChangeIndexParam::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_bSaveAllPeriod = ((CTaiShanDoc*)(CMainFrame::m_pDoc))->m_propertyInitiate.bSaveParam;
	UpdateParamArea();
	UpdateData(FALSE);
	m_bInitialed=true;

//	SetWindowPos(0,m_pt.x,m_pt.y,160 *2-60,100*2-20,SWP_NOREDRAW);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CTaiKlineDlgChangeIndexParam::OnOK() 
{
	// TODO: Add extra validation here
/*	if(m_pJishuNow==NULL)
	{
		DestroyWindow();
		return;
	}
	UpdateData(TRUE);
	((CTaiShanDoc*)(CMainFrame::m_pDoc))->m_propertyInitiate.bSaveParam = m_bSaveAllPeriod;
*/	OnButtonSet() ;
}
void CTaiKlineDlgChangeIndexParam::OnCancel()
{
	OnClose() ;
}

void CTaiKlineDlgChangeIndexParam::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	OnButtonRestore();
	DestroyWindow();
	
//	CDialog::OnClose();
}

void CTaiKlineDlgChangeIndexParam::OnDestroy() 
{
	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	
}

void CTaiKlineDlgChangeIndexParam::PostNcDestroy() 
{
	// TODO: Add your specialized code here and/or call the base class
	if(	pView->m_pChangeIndexParam!=NULL)
	{
		pView->m_pChangeIndexParam=NULL;
		pView->RedrawWindowNoCacl ();
		delete this;
		CDialog::PostNcDestroy();
	}
}

void CTaiKlineDlgChangeIndexParam::OnChangeEditN() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	UpdateParam(g_controlID[1][0], 0);	
	// TODO: Add your control notification handler code here
}

void CTaiKlineDlgChangeIndexParam::OnChangeEditLl2() 
{
	// TODO: Add your control notification handler code here
	UpdateParam(g_controlID[1][1], 1);	
}

void CTaiKlineDlgChangeIndexParam::OnChangeEditP3() 
{
	// TODO: Add your control notification handler code here
	UpdateParam(g_controlID[1][2], 2);	
}

void CTaiKlineDlgChangeIndexParam::OnChangeEditP4() 
{
	// TODO: Add your control notification handler code here
	UpdateParam(g_controlID[1][3], 3);	
}

void CTaiKlineDlgChangeIndexParam::UpdateParam(int nID, int nParam)
{
	if(m_pJishuNow==NULL)
		return;
	if(m_bInitialed==false)
		return;
	CFormularContent *jishunow=m_pJishuNow;
	CString s;
	GetDlgItem(nID)->GetWindowText(s);
	float f = atof(s); 
	if(jishunow->max [nParam]<f)
	{
/*		f=jishunow->max [nParam];
		s.Format ("%f",f);
		if(s.Find (".")>=0)
		{
			s.TrimRight ("0");
			s.TrimRight (".");
		}
		GetDlgItem(nID)->SetWindowText(s);*/
		return;
	}
	if(jishunow->min [nParam]>f)
	{
		return;
/*		f=jishunow->min [nParam];
		s.Format ("%f",f);
		if(s.Find (".")>=0)
		{
			s.TrimRight ("0");
			s.TrimRight (".");
		}
		GetDlgItem(nID)->SetWindowText(s);*/
	}
	SetParamLjishu(jishunow, pView->m_nKlineKind2, nParam,m_bSaveAllPeriod,f);
/*	if(m_bSaveAllPeriod == TRUE)
	{
		int nInd = CTaiScreenParent::FromKlineKindToFoot(pView->m_nKlineKind2 );
		if(nInd>=0 && nParam < jishunow->numPara && jishunow->defaultValArray.GetSize()>nParam)
			jishunow->defaultValArray[nParam].fValue[nInd] = f;
		else 
			jishunow->defaultVal [nParam]=f;
	}
	else
		jishunow->defaultVal [nParam]=f;*/

	pView->pKlineDrawing ->m_bToCacl =true;
	pView->pKlineDrawing ->m_bInitFoot =false;//m_bInitFoot
	pView->RedrawWindow();

}

void CTaiKlineDlgChangeIndexParam::OnChangeEditN2() 
{
	UpdateParam(g_controlID[1][4], 4);	
	
}

void CTaiKlineDlgChangeIndexParam::OnChangeEditLl22() 
{
	// TODO: Add your control notification handler code here
	UpdateParam(g_controlID[1][5], 5);	
}

void CTaiKlineDlgChangeIndexParam::OnChangeEditP3P2() 
{
	UpdateParam(g_controlID[1][6], 6);	
	
}

void CTaiKlineDlgChangeIndexParam::OnChangeEditLl3P5() 
{
	UpdateParam(g_controlID[1][7], 7);	
	
}

float CTaiKlineDlgChangeIndexParam::StringToFloat(CString s)
{
	return 0;
}

void CTaiKlineDlgChangeIndexParam::OnCheckSaveParam() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	((CTaiShanDoc*)(CMainFrame::m_pDoc))->m_propertyInitiate.bSaveParam = m_bSaveAllPeriod;
	
}

void CTaiKlineDlgChangeIndexParam::OnSetfocusEditLl() 
{
	// TODO: Add your control notification handler code here
	((CEdit*)GetDlgItem(g_controlID[1][1]))->SetSel(0,-1);
}

void CTaiKlineDlgChangeIndexParam::OnSetfocusEditLl2() 
{
	// TODO: Add your control notification handler code here
	((CEdit*)GetDlgItem(g_controlID[1][5]))->SetSel(0,-1);
	
}

void CTaiKlineDlgChangeIndexParam::OnSetfocusEditLl3P4() 
{
	// TODO: Add your control notification handler code here
	((CEdit*)GetDlgItem(g_controlID[1][3]))->SetSel(0,-1);
	
}

void CTaiKlineDlgChangeIndexParam::OnSetfocusEditLl3P5() 
{
	// TODO: Add your control notification handler code here
	((CEdit*)GetDlgItem(g_controlID[1][7]))->SetSel(0,-1);
	
}

void CTaiKlineDlgChangeIndexParam::OnSetfocusEditN() 
{
	// TODO: Add your control notification handler code here
	((CEdit*)GetDlgItem(g_controlID[1][0]))->SetSel(0,-1);
	
}

void CTaiKlineDlgChangeIndexParam::OnSetfocusEditN2() 
{
	// TODO: Add your control notification handler code here
	((CEdit*)GetDlgItem(g_controlID[1][4]))->SetSel(0,-1);
	
}

void CTaiKlineDlgChangeIndexParam::OnSetfocusEditP3P() 
{
	// TODO: Add your control notification handler code here
	((CEdit*)GetDlgItem(g_controlID[1][2]))->SetSel(0,-1);
	
}

void CTaiKlineDlgChangeIndexParam::OnSetfocusEditP3P2() 
{
	// TODO: Add your control notification handler code here
	((CEdit*)GetDlgItem(g_controlID[1][6]))->SetSel(0,-1);
	
}

BOOL CTaiKlineDlgChangeIndexParam::OnHelpInfo(HELPINFO* pHelpInfo) 
{
	// TODO: Add your message handler code here and/or call default
	
//	HtmlHelp(m_hWnd,"stock.chm",HH_HELP_CONTEXT,CTaiKlineDlgChangeIndexParam::IDD);
//	return CDialog::OnHelpInfo(pHelpInfo);
	DoHtmlHelp(this);return TRUE;
}

void CTaiKlineDlgChangeIndexParam::SetParamLjishu(CFormularContent *jishunow, int nKlineType, int nParam, BOOL bSaveAll,float f)
{
	if(jishunow->bYH == 0)
	{
		if(bSaveAll == TRUE)
		{
			int nInd = CTaiScreenParent::FromKlineKindToFoot(nKlineType );
			if(nInd>=0 && nParam < jishunow->numPara && jishunow->defaultValArray.GetSize()>nParam)
				jishunow->defaultValArray[nParam].fValue[nInd] = f;
			else 
				jishunow->defaultVal [nParam]=f;
		}
		else
			jishunow->defaultVal [nParam]=f;
	}
	else
	{
		jishunow->defaultValYH  [nParam] = f;
	}

}
