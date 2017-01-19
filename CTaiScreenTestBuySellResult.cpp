// CTaiScreenTestBuySellResult.cpp : implementation file
//

#include "stdafx.h"
#include "WH.h"
#include "CTaiScreenTestBuySellResult.h"
#include "CTaiScreenParent.h"
#include "mainfrm.h"
#include "WH.h"
#include "CTaiShanDoc.h"
#include "CTaiScreenTestDetial.h"
#include "KEYBRODEANGEL.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTaiScreenTestBuySellResult dialog


CTaiScreenTestBuySellResult::CTaiScreenTestBuySellResult(CWnd* pParent /*=NULL*/)
	: CDialog(CTaiScreenTestBuySellResult::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTaiScreenTestBuySellResult)
	m_sResult = _T("");
	//}}AFX_DATA_INIT
	GetParenInfo=(CTaiScreenTest*)pParent;
	pDoc=CMainFrame::m_pDoc;
	profitresult=NULL;
	m_bRecursion=FALSE;
	m_SelNum=0;

	m_pKline = 0;
	m_nKline = 0;

}


void CTaiScreenTestBuySellResult::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTaiScreenTestBuySellResult)
	DDX_Control(pDX, IDOK, m_ok);
	DDX_Control(pDX, IDC_TABBAR, m_bTab);
	DDX_Control(pDX, IDC_MSFLEXGRID1, m_grid);
	DDX_Text(pDX, IDC_EDIT1, m_sResult);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTaiScreenTestBuySellResult, CDialog)
	//{{AFX_MSG_MAP(CTaiScreenTestBuySellResult)
	ON_WM_DESTROY()
	ON_NOTIFY(TCN_SELCHANGE, IDC_TABBAR, OnSelchangeTab)
	ON_WM_PAINT()
	ON_WM_MOUSEWHEEL()
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTaiScreenTestBuySellResult message handlers

BOOL CTaiScreenTestBuySellResult::OnInitDialog() 
{
	CDialog::OnInitDialog();
	// TODO: Add extra initialization here
	GetDlgItem(IDC_MSFLEXGRID1)->GetWindowRect(&m_rectsample);
	ScreenToClient(&m_rectsample);
	m_bTab.InsertItem(0,"统计数据");
	m_bTab.InsertItem(1,"详细列表");
	//	GetDlgItem(IDC_MSFLEXGRID1)->ShowWindow(SW_HIDE);
	//m_bTab.InsertItem(2,"图形显示");
	for(int aa=0;aa<10;aa++)
		isaccelerate[aa]=false;
	float TotalEqual[12];
	memset(TotalEqual,0x00,sizeof(float)*12);
	//for(int j=0;j<10;j++)
	//	DataArray[j]=0.0;
	CReportData* pdt;
	int Total=GetParenInfo->m_testResultArray.GetSize();

	int n50 = 0;
	for(int i=0;i<Total;i++)
	{
        chooseresult=GetParenInfo->m_testResultArray.GetAt(i);
		int num;
		float num1=0.0;
		float num2=0.0;
		float DataArray[12];
	    memset(DataArray,0x00,sizeof(float)*12);
		if(!pDoc->m_sharesInformation.Lookup(chooseresult->symbol.GetBuffer(0),pdt,chooseresult->m_stkKind ))
			continue;
		SCREEN_DETAIL detail;
		num=chooseresult->m_detail.GetSize();

//		int nSuccess = 0;
//		int nFail = 0;
		for(int n=0;n<num;n++)
		{
		    detail=chooseresult->m_detail.GetAt(n);
			if(detail.profit>0)
			{
			     DataArray[6]++;
			     num1+=detail.profit;
			}
			else
			{
			     DataArray[7]++;
				 num2+=detail.profit;
			}
            DataArray[0]+=detail.profit;
			if(n==0)
			{
			     DataArray[4]=detail.profit;//
				 DataArray[5]=detail.profit;
			}
			if(DataArray[4]<detail.profit)
		         DataArray[4]=detail.profit;
            if(DataArray[5]>detail.profit)
			     DataArray[5]=detail.profit;

			if(detail.issuccess == 0)
			     DataArray[8]++;
			else if(detail.issuccess == 1)
			     DataArray[9]++;
			else
			     DataArray[10]++;

			
		}
		if(num!=0)
	   {
		   m_SelNum++;
		   DataArray[1]=DataArray[0]/num;
		   if(DataArray[8]+DataArray[9]>0)
			   DataArray[2]=DataArray[8]/(DataArray[8]+DataArray[9])*100;
		   else
			   DataArray[2]=0;
		   DataArray[3]=100*DataArray[6]/num;
		   if(DataArray[4]<0)
			  DataArray[4]=0.0;
		   if(DataArray[5]>0)
			  DataArray[5]=0.0;

		   if(DataArray[2]>=50)
			   n50++;
		}
			profitresult = new PROFIT_RESULT;
			profitresult->StockName=pdt->name;
			for(int nn=0;nn<11;nn++)
			{
				profitresult->profit[nn]=DataArray[nn];
			    TotalEqual[nn]+=DataArray[nn];
			}
			profitresult->index=i;
			m_prosult.Add(profitresult);

	}
	TotalEqual[1]=(TotalEqual[6]+TotalEqual[7]!=0)?TotalEqual[0]/(TotalEqual[6]+TotalEqual[7]):0.0;
	//TotalEqual[1]=TotalEqual[0]/(TotalEqual[6]+TotalEqual[7]);

	int nCount[4] ;
	nCount[0] = (int)(TotalEqual[6]);
	nCount[1] = (int)(TotalEqual[7]);
	nCount[2] = (int)(TotalEqual[8]);
	nCount[3] = (int)(TotalEqual[9]);
    TotalEqual[0]=(m_SelNum==0)?0:TotalEqual[0]/m_SelNum;
	int ii;
	for(ii=2;ii<11;ii++)
        TotalEqual[ii]=(m_SelNum==0)?0:TotalEqual[ii]/m_SelNum;
//	CString titlearray[]={"股票名称","总收益率","平均收益","去顶收益","盈 利 率"
//		,"最大收益","最小收益","盈利次数","亏损次数","盈利平均","亏损平均"};
	CString titlearray[]={"股票名称","总收益率","平均收益","成 功 率","盈 利 率"
		,"最大收益","最小收益","盈利次数","亏损次数","成功次数","失败次数","未完成次数"};
	m_grid.SetRowHeight(0,500);
 
	for(ii=0;ii<12;ii++)
	{
		/*m_grid.SetRow(0);
        m_grid.SetCol(ii);
		m_grid.SetText(titlearray[ii]);*/
		m_grid.SetTextMatrix(0,ii,titlearray[ii]);
		int length=100*titlearray[ii].GetLength();
		m_grid.SetColWidth(ii,length>800?length:800);
	}
	m_grid.SetTextMatrix(1,0,"总平均");
    CString str;
	int j;
    for(j=0;j<11;j++)
	{
//	   if(j>=6)
//		   str.Format("%d",int(TotalEqual[j])   );
//	   else
		   str.Format("%.1f",TotalEqual[j]);
	   m_grid.SetTextMatrix(1,j+1,str.GetBuffer(str.GetLength()));//.SetText(str.GetBuffer(str.GetLength()));
	   str.ReleaseBuffer();
	}
	m_sResult = "\r\n";

	m_sResult += "  测试股票数:";
	CString sTemp;

	sTemp.Format ("%d\r\n",Total);
	m_sResult += sTemp;

	m_sResult += "  共发出指示:";
	sTemp.Format ("%d ",(int)(nCount[0]+nCount[1]));
	m_sResult += sTemp;
	sTemp.Format ("成功指示:%d ",(int)(nCount[2]));
	m_sResult += sTemp;
	sTemp.Format ("失败指示:%d ",(int)(nCount[3]));
	m_sResult += sTemp;
	sTemp.Format ("未完成指示:%d\r\n",(int)(nCount[0]+nCount[1]-nCount[2]-nCount[3]));
	m_sResult += sTemp;
	sTemp.Format ("  平均成功率:%.2f%%\t",TotalEqual[2]);
	m_sResult += sTemp;
	sTemp.Format ("成功率达到50%%的股票有:%d%%\r\n\r\n",(Total>0?n50*100/Total:0));
	m_sResult += sTemp;

	CString ss[] = {"%","%","%","%","%",
					"%","次","次","%","%"};
	ShowResult();


	Total=m_prosult.GetSize();
    for(j=0;j<11;j++)
	{
		int col=j+1;
		if(Total<=0)
				continue;
		if(col>=7 )
				continue;
		m_sResult+="  平均";
		m_sResult+=titlearray[j+1];
		m_sResult+=" : ";
	   str.Format("%.1f",TotalEqual[j]);
	   m_sResult+=str;
	   m_sResult+=ss[j];

		{	
		{
			Quick_Sort(0,Total-1,col);
//	        int index=m_prosult[0]->index;
//			GetParenInfo->m_testResultArray.GetAt(index),m_prosult[0]->StockName);
			if(col!=6)
			{
				m_sResult+="\t最大";
				m_sResult+=titlearray[j+1];
				m_sResult+=":";
				m_sResult+=m_prosult[0]->StockName;
				m_sResult+=" ";
			   if(j==6||j==7)
				   str.Format("%d",int(m_prosult[0]->profit[j])   );
			   else
				   str.Format("%.1f",m_prosult[0]->profit[j]);
			   m_sResult+=str;
				m_sResult+=ss[j];
			}
			m_sResult+=" \t";

			if(col!=5)
			{
				m_sResult+="最小";
				m_sResult+=titlearray[j+1];
				m_sResult+=":";
				m_sResult+=m_prosult[Total-1]->StockName;
				m_sResult+=" ";
			   if(j==6||j==7)
				   str.Format("%d",int(m_prosult[Total-1]->profit[j])   );
			   else
				   str.Format("%.1f",m_prosult[Total-1]->profit[j]);
			   m_sResult+=str;
				m_sResult+=ss[j];
			}
			m_sResult+=" \t";
		}
		}
	   
	   
	   
	   m_sResult+="\r\n";
//	   str.ReleaseBuffer();
	}
   m_sResult+="\r\n";
//	CString titlearray[]={"股票名称","总收益率","平均收益","去顶收益","盈 利 率"
//		,"最大收益","最小收益","盈利次数","亏损次数","平均盈利","平均亏损"};
   m_sResult+="  总收益率: 累计所有交易的收益率；\r\n";
   m_sResult+="  平均收益: 总收益率 / 累计交易次数；\r\n";
//   m_sResult+="  去顶收益: 总收益率 - 最大收益；\r\n";
   m_sResult+="  成 功 率: 成功次数 / (成功次数 + 失败次数)；\r\n";
   m_sResult+="  盈 利 率: 盈利次数 / (盈利次数 + 亏损次数)；\r\n";
   m_sResult+="  最大收益: 最大的一次收益率；\r\n";
   m_sResult+="  最小收益: 最小的一次收益率；\r\n";
   m_sResult+="  盈利次数: 累计盈利交易次数；\r\n";
   m_sResult+="  亏损次数: 累计亏损交易次数；\r\n";
   m_sResult+="  成功次数: 指定周期内达到目标收益的交易次数\r\n";
   m_sResult+="  失败次数: 指定周期内没有达到目标收益的交易次数；\r\n";
   m_sResult+="  未完成次数: 指定周期内没有完成的交易次数；\r\n";
//   m_sResult+="  盈利平均: 累计盈利交易收益 / 盈利次数；\r\n";
//   m_sResult+="  亏损平均: 累计亏损交易收益 / 亏损次数；\r\n";

//	m_grid.SetFocus ();
	ShowResult();
	long result;
	OnSelchangeTab(NULL, &result) ;
	UpdateData(FALSE);
	return FALSE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BEGIN_EVENTSINK_MAP(CTaiScreenTestBuySellResult, CDialog)
    //{{AFX_EVENTSINK_MAP(CTaiScreenTestBuySellResult)
	ON_EVENT(CTaiScreenTestBuySellResult, IDC_MSFLEXGRID1, -600 /* Click */, OnClickMsflexgrid1, VTS_NONE)
	ON_EVENT(CTaiScreenTestBuySellResult, IDC_MSFLEXGRID1, -601 /* DblClick */, OnDblClickMsflexgrid1, VTS_NONE)
	ON_EVENT(CTaiScreenTestBuySellResult, IDC_MSFLEXGRID1, 70 /* RowColChange */, OnRowColChangeMsflexgrid1, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

void CTaiScreenTestBuySellResult::OnClickMsflexgrid1() 
{
	// TODO: Add your control notification handler code here
	int Total=m_prosult.GetSize();
	int row=m_grid.GetMouseRow();
	if(row==0)
	{	
		m_grid.SetHighLight(0);
		int col=m_grid.GetMouseCol();
		int length=m_grid.GetRows();
		if(col>0&&col<11)
		{
			//SortItem(col);
			Quick_Sort(0,Total-1,col);
			if(isaccelerate[col-1])
				isaccelerate[col-1]=false;
			else
				isaccelerate[col-1]=true;
		    ShowResult();
		}
        m_grid.SetRow(2);
	}
   else 
      m_grid.SetHighLight(2);
}

void CTaiScreenTestBuySellResult::OnDblClickMsflexgrid1() 
{
	// TODO: Add your control notification handler code here
	int row=m_grid.GetRowSel();  //.GetMouseRow(); edit by zxb /010914
	int Total=m_prosult.GetSize();
	if(1<row&&row<=(Total+1))
	{
        int index=m_prosult[row-2]->index;
		CTaiScreenTestDetial InDetail; 
		InDetail.SetDetail(GetParenInfo->m_testResultArray.GetAt(index),m_prosult[row-2]->StockName);
		InDetail.DoModal();
	}
	else 
		return;
	
}

void CTaiScreenTestBuySellResult::ShowResult()
{
    int total=m_prosult.GetSize();
	//if( total>=1100 )
	m_grid.SetRows(total+3);
	CString str;
	for(int i=0;i<total;i++)
	{
		m_grid.SetTextMatrix(i+2,0,m_prosult[i]->StockName);
		for(int j=0;j<11;j++)
		{
		   if(j>=6)
			   str.Format("%d",int(m_prosult[i]->profit[j])   );
		   else
		       str.Format("%.1f",m_prosult[i]->profit[j]);
		   m_grid.SetTextMatrix(i+2,j+1,str.GetBuffer(str.GetLength()));//.SetText(str.GetBuffer(str.GetLength()));
		   str.ReleaseBuffer();
		}
	}
	m_grid.SetRow(2);
}

void CTaiScreenTestBuySellResult::SortItem(int index)
{
    PROFIT_RESULT *chsult[2];
	int Total=m_prosult.GetSize();
	for(int i=Total;i>0;i--)
		for(int j=0;j<(i-1);j++)
		{
			chsult[0]=m_prosult.GetAt(j);
			chsult[1]=m_prosult.GetAt(j+1);
			if(isaccelerate[index-1]==true)
				if(chsult[0]->profit[index-1]>chsult[1]->profit[index-1])
				{
					m_prosult.SetAt(j,chsult[1]);
					m_prosult.SetAt((j+1),chsult[0]);
				}
			if(isaccelerate[index-1]==false)
				if(chsult[0]->profit[index-1]<chsult[1]->profit[index-1])
				{
					m_prosult.SetAt(j,chsult[1]);
					m_prosult.SetAt((j+1),chsult[0]);
				}
		}
}

void CTaiScreenTestBuySellResult::OnDestroy() 
{
	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	for(int i=0;i<m_prosult.GetSize();i++)
	   delete m_prosult[i];
}

void CTaiScreenTestBuySellResult::Quick_Sort(int first, int last,int index)
{
    int low,high;
	PROFIT_RESULT *temp;
	float list_separator;
    low=first;
	high=last;
	list_separator=m_prosult[(first+last)/2]->profit[index-1];
    if(isaccelerate[index-1]==true)
	{
		do{
			while(m_prosult[low]->profit[index-1]<list_separator)
				low++;
			while(m_prosult[high]->profit[index-1]>list_separator)
				high--;
			if(low<=high)
			{
				temp=m_prosult[low];
				m_prosult.SetAt(low++,m_prosult[high]);
				m_prosult.SetAt(high--,temp);
			}
		}while(low<=high);
	}
	if(isaccelerate[index-1]==false)
	{
		do{
			while(m_prosult[low]->profit[index-1]>list_separator)
				low++;
			while(m_prosult[high]->profit[index-1]<list_separator)
				high--;
			if(low<=high)
			{
				temp=m_prosult[low];
				m_prosult.SetAt(low++,m_prosult[high]);
				m_prosult.SetAt(high--,temp);
				
			}
		}while(low<=high);
	}
	if(first<high)
		Quick_Sort(first,high,index);
	if(low<last)
		Quick_Sort(low,last,index);

}

BOOL CTaiScreenTestBuySellResult::PreTranslateMessage(MSG* pMsg) 
{
    if( GetFocus() == GetDlgItem(IDC_MSFLEXGRID1) && pMsg->message==WM_CHAR && ( (pMsg->wParam>='0'&&pMsg->wParam<='9') || (pMsg->wParam>='A'&&pMsg->wParam<='z') )  )
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
			CReportData* pDat;
		 	if ( (CMainFrame::m_pDoc)->m_sharesInformation.Lookup(szSymbol.GetBuffer(0) , pDat,KeyboardWizard.m_stkKind ) )     //检测该股票是否已记录在内存模板中
			{
				CString StockName=pDat->name;	
                int total=m_prosult.GetSize();
				BOOL IsFind=FALSE;
				for(int Num=0;Num<total;Num++)
				{
					CString ss = m_prosult[Num]->StockName;
					if(ss==StockName)
					{
						IsFind=TRUE;
                        m_grid.SetRow(Num+2);
						if(Num>10)
						    m_grid.SetTopRow(Num-3);
						else 
							m_grid.SetTopRow(2);
						//m_grid.SetCol(0);
						//m_grid.SetColSel(9);
						//m_grid.SetRightToLeft(1);
						/*unsigned long backcolor=m_grid.GetBackColor();
						m_grid.SetBackColor(0);
						m_grid.SetRow(Num+2);
						m_grid.SetBackColor(backcolor);
						ShowResult();*/
						break;
					}
				}
				if(!IsFind)
				{
					AfxMessageBox("在列表中未找到该股票!");
					return TRUE;
				}
			
			}
			else 
				return TRUE;
		}
		return TRUE;
	}	

//ADD BY ZXB  /010914
	if(pMsg->message == WM_KEYDOWN && (pMsg->wParam)==13) 
	{ 
		if(m_bTab.GetCurSel()==1)
		{
   		  OnDblClickMsflexgrid1(); 
		  return TRUE;
		}
	} 

	return CDialog::PreTranslateMessage(pMsg);
}

void CTaiScreenTestBuySellResult::OnRowColChangeMsflexgrid1() 
{
	if (m_bRecursion) 
		return;
	m_bRecursion=TRUE;
	m_grid.SetCol(1);
	m_grid.SetColSel(10);
	m_bRecursion=FALSE;
}


void CTaiScreenTestBuySellResult::OnSelchangeTab(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	if(m_bTab.GetCurSel()==0)
	{
		GetDlgItem(IDC_MSFLEXGRID1)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT1)->ShowWindow(SW_SHOW);
	}
	else if(m_bTab.GetCurSel()==1)
	{
		GetDlgItem(IDC_MSFLEXGRID1)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_EDIT1)->ShowWindow(SW_HIDE);
		m_grid.SetFocus ();
	}
	else if(m_bTab.GetCurSel()==2)
	{
		GetDlgItem(IDC_MSFLEXGRID1)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT1)->ShowWindow(SW_HIDE);

		if(!m_pKline)
		{
			//CString symbol = "1A0001";
			m_nKline = GetParenInfo->GetIndexKline(m_pKline);

			//统计 Kline.high = success ,Kline.low = fail
			int i;
			for(i = 0;i<m_nKline;i++)
			{
				m_pKline[i].high = 0;
				m_pKline[i].low = 0;
			}
			int Total=GetParenInfo->m_testResultArray.GetSize();
			CReportData* pdt;
			for(i=0;i<Total;i++)
			{
				chooseresult=GetParenInfo->m_testResultArray.GetAt(i);
				int num;
				float num1=0.0;
				float num2=0.0;
				float DataArray[10];
				memset(DataArray,0x00,sizeof(float)*10);
				if(!CMainFrame::m_pDoc ->m_sharesInformation.Lookup(chooseresult->symbol.GetBuffer(0),pdt,chooseresult->m_stkKind ))
					continue;
				SCREEN_DETAIL detail;
				num=chooseresult->m_detail.GetSize();

				for(int n=0;n<num;n++)
				{
					detail=chooseresult->m_detail.GetAt(n);
//	bool bDay =false,bAdd = false;
//	if( pView->m_nKlineKind2>=5 && pView->m_nKlineKind2<=8 ||pDoc->m_propertyInitiate.daysOfManyKline>0&& pView->m_nKlineKind2==MANY_DAY_KLINE)
//		bDay = true;
//	int rtn=CTaiKlineFileKLine::LookTwoPath((long&)keyVlu ,pKline,max, bAdd,bDay);
				}
			}
		}
		InvalidateRect(&m_rectsample);
	}
	*pResult = 0;
}

void CTaiScreenTestBuySellResult::OnPaint() 
{
/*
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	COLORREF backcolor=RGB(203,216,237);
	LOGFONT  ft = CMainFrame::m_pDoc ->m_fontstr[0]; //字体
	CBrush brush;                      
	CBrush *poldbrush;
	brush.CreateSolidBrush(backcolor);
	poldbrush=dc.SelectObject(&brush);
	dc.FillRect(&m_rectsample,&brush);
	dc.SelectObject(poldbrush);
	brush.DeleteObject();
	//-----------------------------------------画坐标线
	COLORREF cr=RGB(55,66,0);
	CPen pen;                          
	CPen *poldpen;
	pen.CreatePen(PS_SOLID,1,cr);
    poldpen=dc.SelectObject(&pen);
	int m_rect_middle=(m_rectsample.left+m_rectsample.right)/2; 
	int Width=(m_rectsample.right-m_rectsample.left-90)/30;
	int High=m_rectsample.bottom-m_rectsample.top-80;
	dc.SetBkColor(backcolor);
	dc.TextOut(m_rectsample.right-30,m_rectsample.bottom-48,">");
	dc.TextOut(m_rectsample.left+38,m_rectsample.top+3,"^");
	dc.MoveTo(m_rectsample.left+40,m_rectsample.top+5);    //画垂直线
	dc.LineTo(m_rectsample.left+40,m_rectsample.bottom-5);
    dc.MoveTo(m_rectsample.left+20,m_rectsample.bottom-40);
	dc.LineTo(m_rectsample.right-25,m_rectsample.bottom-40);//划水平线
	dc.SelectObject(poldpen);
	pen.DeleteObject();


	CFont fontrise;
	CFont *priseold;
    fontrise.CreateFontIndirect(&ft);
	priseold=dc.SelectObject(&fontrise);
	dc.TextOut(m_rectsample.left+15,m_rectsample.top+5,"交易次数");
	//dc.TextOut(m_rectsample.left+20,m_rectsample.top+25,"易");
	//dc.TextOut(m_rectsample.left+20,m_rectsample.top+45,"次");
	//dc.TextOut(m_rectsample.left+20,m_rectsample.top+65,"数");

	float m_mbly=GetParenInfo->m_mbly;
	int length=m_mbly/7+1;//m_mbly/7;
	int NumStock[15];
	int NumSucc[3];
	memset(NumStock,0,sizeof(int)*15);
	memset(NumSucc,0,sizeof(int)*3);
	int Total=GetParenInfo->m_testResultArray.GetSize();
	for(int i=0;i<Total;i++)
	{
        chooseresult=GetParenInfo->m_testResultArray.GetAt(i);
		SCREEN_DETAIL detail;
		int num;
		num=chooseresult->m_detail.GetSize();
		for(int n=0;n<num;n++)
		{
		    detail=chooseresult->m_detail.GetAt(n);
			if(detail.profit>=m_mbly)
				NumSucc[0]++;
			else if(detail.profit>0&&detail.profit<m_mbly)
				NumSucc[1]++;
			else NumSucc[2]++;
			for(int j=0;j<15;j++)
			{
				if(detail.profit<length)
				{
					NumStock[0]++;
                    break;
				}else
				if(detail.profit>length*14)
				{
					NumStock[14]++;
                    break;
				}else if(detail.profit>length*j&&detail.profit<length*(j+1))
				{
                     NumStock[j]++;
					 break;
				}
			}
		}
	}
	int TopNum=0;
	for(int ii=0;ii<15;ii++)
		if(NumStock[ii]>TopNum)
			TopNum=NumStock[ii];
    int TotalNum=NumSucc[0]+NumSucc[1]+NumSucc[2];
	CString str;
	if (TotalNum > 0)
//------------------------------------
	{
		str.Format("%s%.2f","大于目标收益:",float(NumSucc[0])/TotalNum*100);
		str+="%";
		dc.TextOut(m_rectsample.right-152,m_rectsample.bottom-250,str);
		str.Format("%s%.2f","零收益与目标收益之间:",float(NumSucc[1])/TotalNum*100);
		str+="%";
		dc.TextOut(m_rectsample.right-215,m_rectsample.bottom-270,str);
		str.Format("%s%.2f","收益率小于零:",float(NumSucc[2])/TotalNum*100);
		str+="%";
		dc.TextOut(m_rectsample.right-152,m_rectsample.bottom-230,str);
	}
	else
	{
		str.Format("%s%.2f","大于目标收益:",(float)0.00);
		str+="%";
		dc.TextOut(m_rectsample.right-152,m_rectsample.bottom-250,str);
		str.Format("%s%.2f","零收益与目标收益之间:",(float)0.00);
		str+="%";
		dc.TextOut(m_rectsample.right-215,m_rectsample.bottom-270,str);
		str.Format("%s%.2f","收益率小于零:",(float)0.00);
		str+="%";
		dc.TextOut(m_rectsample.right-152,m_rectsample.bottom-230,str);
//--------------------------------------modify by zxb /010914
	}
    for(ii=1;ii<16;ii++)
	{
		str.Format("%d",length*ii);
		dc.TextOut(m_rectsample.left+36+Width*(2*ii-1),m_rectsample.bottom-35,str);
	}
	int TopNumShow=float(TopNum)/High*(High+10);
	for(ii=1;ii<9;ii++)
	{
	    dc.MoveTo(m_rectsample.left+40,m_rectsample.bottom-40-(High+10)/8*ii);    //画垂直线
	    dc.LineTo(m_rectsample.left+45,m_rectsample.bottom-40-(High+10)/8*ii);
		str.Format("%d",TopNumShow/8*ii);
		dc.TextOut(m_rectsample.left+12,m_rectsample.bottom-47-(High+10)/8*ii,str);  
	}


	CBrush brushupline;                   //柱线实体     
	COLORREF bcolor=RGB(255,255,0);
    COLORREF mcolor=RGB(255,0,0);
	brushupline.CreateSolidBrush(bcolor);    
	poldbrush=dc.SelectObject(&brushupline);
	RECT   upkline;

	int nMidValu = 7;
	if(length>0)
		nMidValu = m_mbly/length;
	if(nMidValu>12)
		nMidValu = 7;

	for(int k=0;k<15;k++)
	{
		upkline.left=m_rectsample.left+40+Width*(2*k+1);
		upkline.right=m_rectsample.left+40+Width*2*(k+1);
		float lenPer=float(High)/TopNum;
		int Nleng=lenPer*NumStock[k];
		upkline.top=m_rectsample.bottom-40-Nleng;
		upkline.bottom=m_rectsample.bottom-40;
		if(k==nMidValu)
		{
			brushupline.DeleteObject();
			brushupline.CreateSolidBrush(mcolor);    
	        dc.SelectObject(&brushupline);
			dc.FillRect(&upkline,&brushupline);
			brushupline.DeleteObject();
			brushupline.CreateSolidBrush(bcolor);    
	        dc.SelectObject(&brushupline);
			continue;
		}
		dc.FillRect(&upkline,&brushupline);
	}
	dc.SelectObject(poldbrush);
	brushupline.DeleteObject();

	dc.TextOut(m_rectsample.right-70,m_rectsample.bottom-20,"收益率(%)");
	dc.SelectObject(priseold);
	fontrise.DeleteObject();*/
	if(m_bTab.GetCurSel()==2)
	{
	}
	else
	CDialog::OnPaint() ;
}

BOOL CTaiScreenTestBuySellResult::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) 
{
	// TODO: Add your message handler code here and/or call default
	if(zDelta>=0)
	{
		int n = zDelta;
		if(n>=15) n = 7;
		m_grid.SendMessage(WM_KEYDOWN,VK_UP,n);
	}
	else
	{
		int n = abs(zDelta);
		if(n>=15) n = 7;
		m_grid.SendMessage(WM_KEYDOWN,VK_DOWN,n);
	}
	return CDialog::OnMouseWheel(nFlags, zDelta, pt);
}

void CTaiScreenTestBuySellResult::OnOK() 
{
	// TODO: Add extra validation here
	if(AfxMessageBox("是否关闭？",MB_YESNO) == IDYES)
		CDialog::OnOK();
}

void CTaiScreenTestBuySellResult::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	
	OnOK();
}
