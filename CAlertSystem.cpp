// CAlertSystem.cpp : implementation file
////change 0916

#include "stdafx.h"
#include "WH.h"
#include "CAlertSystem.h"
#include "MainFrm.h"
#include "CTaiShanDoc.h"
#include "SelectStock.h"
#include "BGridCtrl.h"
#include "keybrodeangel.h"
#include "SetParam.h"
#include "CSharesBigBillDlg.h"
#include "CSharesInformation.h"
#include "CAlertSet.h"
#include "CTaiKlineFileHS.h"
#include "CAlertPopDlg.h"
#include "CTaiKlineDoKline.h"
#include "CTaiKlineDoFenshi.h"
#include <afxpriv.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
CListAlarmSet CAlertSystem::m_gAlarmArray;//趋势线预警条件数组
CListAlarmLineSet CAlertSystem::m_gLineAlarmArray;
CList<ALERT_RESULT_ITEM*,ALERT_RESULT_ITEM*&> CAlertSystem::m_gAlarmResultList;//预警结果数组

CAlertSystem* CAlertSystem::m_gDlgAlarmSystem = NULL;
/////////////////////////////////////////////////////////////////////////////
// CAlertSystem dialog
#define TJXG_ALERT  400    /////The Tine Event of Choose_stock_Alarm


CAlertSystem::CAlertSystem(CWnd* pParent /*=NULL*/)
	: CDialog(CAlertSystem::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAlertSystem)
	//}}AFX_DATA_INIT
	pDoc = CMainFrame::m_pDoc ;
}


void CAlertSystem::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAlertSystem)
	DDX_Control(pDX, IDC_BUTTON7, m_button7);
	DDX_Control(pDX, IDC_BUTTON6, m_button6);
	DDX_Control(pDX, IDC_BUTTON4, m_button4);
	DDX_Control(pDX, IDC_BUTTON3, m_button3);
	DDX_Control(pDX, IDC_LIST2, m_alarmResultList);
	DDX_Control(pDX, IDC_LIST1, m_condArrayList);
	DDX_Control(pDX, IDOK, m_btOK);
	DDX_Control(pDX, IDCANCEL, m_btCancel);
	DDX_Control(pDX, IDC_BUTTON1, m_btAdd);
	DDX_Control(pDX, IDC_BUTTON2, m_btDelete);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAlertSystem, CDialog)
	//{{AFX_MSG_MAP(CAlertSystem)
	ON_BN_CLICKED(IDC_BUTTON1, OnAdd)
	ON_BN_CLICKED(IDC_BUTTON2, OnDelete)
	ON_BN_CLICKED(IDC_BUTTON3, OnModify)
	ON_BN_CLICKED(IDC_BUTTON4, OnClear)
	ON_BN_CLICKED(IDC_BUTTON6, OnClearAll)
	ON_BN_CLICKED(IDC_BUTTON7, OnWriteToFile)
	ON_WM_CLOSE()
	ON_WM_TIMER()
	ON_WM_HELPINFO()
	//}}AFX_MSG_MAP
	ON_NOTIFY(NM_DBLCLK, IDC_LIST2, &CAlertSystem::OnNMDblclkList2)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAlertSystem message handlers

void CAlertSystem::OnAdd() //新增条件
{
	// TODO: Add your control notification handler code here
	CAlertSet dlg;
	for(int i=0;i<1000;i++)
	{
		int n = m_gAlarmArray.GetCount ();
		CString s = "";
		s.Format ("预警%d",i);
		bool bExit = false;
		for(int j=0;j<n;j++)
		{
			POSITION pos=m_gAlarmArray.FindIndex (j);
			ALERT_SET_ITEM* item = m_gAlarmArray.GetAt(pos);
			CString s2 = item->m_strAlarmName ;
			if(s==s2)
			{
//				dlg.m_sTitle = s;
				bExit = true;
				break;
			}
		}
		if(bExit == false) 
		{
			dlg.m_sTitle = s;
			break;
		}
	}
	if(dlg.DoModal ()==IDOK)
	{
		ALERT_SET_ITEM* item = new ALERT_SET_ITEM;
		dlg.GetItemData (*item);
		m_gAlarmArray.AddTail (item);
//		static CArray<ALERT_SET_ITEM,ALERT_SET_ITEM&> m_gAlarmArray;//预警条件数组
		AlarmArrInsertRow(item);
	}

	
}

void CAlertSystem::OnDelete() //删除条件
{
	// TODO: Add your control notification handler code here
//	SelectStock::DeleteSelectedItem(&this->m_condArrayList );
    CString Id;
	int temp=m_condArrayList.GetNextItem( -1, LVNI_SELECTED   ) ;
	if(temp==-1)
	{
		AfxMessageBox("请首先选中条件！");
		return;
	}
	if(IDYES == AfxMessageBox("真的删除？",MB_YESNO|MB_ICONQUESTION ))
	{
		while(temp>=0)
		{
			m_condArrayList.DeleteItem(temp);
			POSITION pos=m_gAlarmArray.FindIndex (temp);
			POSITION pos2 = pos;
			delete m_gAlarmArray.GetAt(pos);
			m_gAlarmArray.RemoveAt (pos2);

			temp=m_condArrayList.GetNextItem( -1,LVNI_SELECTED   ) ;
		}
		ClearAllResult();
	}
	//SaveFile();
}

void CAlertSystem::OnModify() //修改条件
{
	// TODO: Add your control notification handler code here
	CAlertSet dlg;
	int temp=m_condArrayList.GetNextItem( -1, LVNI_SELECTED   ) ;
	if(temp==-1)
	{
		AfxMessageBox("请首先选中条件！");
		return;
	}
	if(temp>=m_gAlarmArray.GetCount ())
		return;

	POSITION pos=m_gAlarmArray.FindIndex (temp);
	ALERT_SET_ITEM* item = m_gAlarmArray.GetAt(pos);

//	m_gAlarmArray.AddTail (item);
	dlg.SetInitData (*item);
	dlg.m_nItem = temp;
	if(dlg.DoModal ()==IDOK)
	{
		dlg.GetItemData (*item);
	}

	AlarmArrInsertRow(item,temp);

	//SaveFile();
	
}

void CAlertSystem::OnClear() 
{
	// TODO: Add your control notification handler code here
//	SelectStock::DeleteSelectedItem(&m_alarmResultList);
    CString Id;
	int temp=m_alarmResultList.GetNextItem( -1, LVNI_SELECTED   ) ;
	if(temp==-1)
	{
		AfxMessageBox("请首先选中预警结果！");
		return;
	}
	if(IDYES == AfxMessageBox("删除预警结果？",MB_YESNO|MB_ICONQUESTION ))
		while(temp>=0)
		{
			m_alarmResultList.DeleteItem(temp);
			POSITION pos=m_gAlarmResultList.FindIndex (temp);
			ALERT_RESULT_ITEM* item = m_gAlarmResultList.GetAt(pos);
			delete item;
			m_gAlarmResultList.RemoveAt (pos);
			temp=m_alarmResultList.GetNextItem( -1,LVNI_SELECTED   ) ;
		}
}

void CAlertSystem::OnClearAll() 
{
	// TODO: Add your control notification handler code here
	if(IDYES == AfxMessageBox("删除全部预警结果？",MB_YESNO|MB_ICONQUESTION ))
	{
		ClearAllResult();
	}
}

void CAlertSystem::OnWriteToFile() 
{
	// TODO: Add your control notification handler code here
	UpdateData();

	//write file
	CLongString longStr;

	int nPerLine=0,nOutLine=0;
	nPerLine =  this->m_alarmResultList .GetItemCount();
	nOutLine = 5; 

	int i,j;
	for(j=0;j<nPerLine;j++)
	{
		for(i =0;i<nOutLine;i++)
		{
			CString s ;
			m_alarmResultList.GetItemText (j,i,s.GetBuffer (24),20);//
			s.ReleaseBuffer ();
			if(i==nOutLine-1)
				s+="\r\n";
			else
				s+="\t";
			longStr + s;
		}
	}

    CString     defaultname="AlarmResultData.xls";

	CString sArray="*.xls";        

	CFileDialog  m_filedia(FALSE,0,defaultname,OFN_CREATEPROMPT|OFN_HIDEREADONLY|OFN_NOCHANGEDIR,sArray);
	m_filedia.m_ofn .lpstrInitialDir = CMainFrame::m_pDoc ->m_CurrentWorkDirectory;
	                                    //将当前目录设为对话框的缺省目录
	CString filename="";
	if(m_filedia.DoModal()==IDOK)
	{
		filename=m_filedia.GetPathName();

		CFile   file;
		if(file.Open(filename,CFile::modeRead)!=0)
		{
			if(AfxMessageBox(filename+"文件已存在，要覆盖吗？",MB_YESNO|MB_ICONQUESTION)==IDNO)
			{
				file.Close();
				return;
			}
			file.Close();
		}

		if(file.Open(filename,CFile::modeWrite|CFile::modeCreate))
		{
			int n = longStr.GetLength ()+1;
			file.Write(longStr.m_pData ,n);
			file.Close();
		}
	}
}

bool CAlertSystem::LoadFile(bool bLine)
{
	CFile fl;
	if(fl.Open("data\\AlarmResult.alm",CFile::shareDenyNone |CFile::modeRead))
	{
		//m_gAlarmArray  DeleteAll
		POSITION pos=m_gAlarmArray.GetHeadPosition ();
		while(pos)
		{
			ALERT_SET_ITEM* item = m_gAlarmArray.GetNext (pos);
			if(item!=NULL) delete item;
		}
		m_gAlarmArray.RemoveAll ();


//read into m_gAlarmArray
		char ch[32]="W2YJ";
		fl.Read(ch,4);
		CString sHead = ch;
		if(sHead != "W2YJ") return false;

		int n;// = m_gAlarmArray.GetCount ();
		fl.Read(&n,4);

		int i,k,j;

		for(i=0;i<n;i++)
		{
//			POSITION pos=m_gAlarmArray.FindIndex (temp);
			ALERT_SET_ITEM* item = new ALERT_SET_ITEM;//m_gAlarmArray.GetAt(pos);
			fl.Read(item->m_strAlarmName ,16);//name

			BYTE bt ;//= (BYTE)item->m_bAnd ;
			fl.Read(&bt,1);//and or:1 and ,0 or
			fl.Read(&bt,1);//and or:1 and ,0 or
			item->m_bAnd = (bool)bt;

			int n2 ;//= item->m_conditionArray .GetSize ();
			fl.Read(&n2,4);
//			item->m_conditionArray .SetSize(n2);item->m_conditionArray .GetData ()
			for(k=0;k<n2;k++)
			{
				IndexDataInfo ind;
				fl.Read(&ind,sizeof(IndexDataInfo));//condition 条件
				item->m_conditionArray.Add (ind);
			}

			fl.Read(item->m_btAddtional ,4);//附加条件

			fl.Read(&n2,4);
			for(k=0;k<n2;k++)
			{
				SharesNameArr3 sym;
				fl.Read(&sym,sizeof(SharesNameArr3));//
				item->m_symbolTypeArray.Add (sym);
			}
			fl.Read(&n2,4);
			for(k=0;k<n2;k++)
			{
				SYMBOL_ISALERTED sym;
				fl.Read(&sym,sizeof(SYMBOL_ISALERTED));//股票代码
				item->m_bAlarmedArray.Add (sym);
			}

			//test item data validity
			//if(item->
			m_gAlarmArray.AddTail (item);//add
		}

		//m_gAlarmResultList

//read into m_gLineAlarmArray
		fl.Read(&n,4);
//		if(bLine == true)
		{

			//ALERT_LINE_ITEM  DeleteAll
			pos=m_gLineAlarmArray.GetHeadPosition ();
			while(pos)
			{
				ALERT_LINE_ITEM* item = m_gLineAlarmArray.GetNext (pos);
				if(item!=NULL) delete item;
			}
			m_gLineAlarmArray.RemoveAll ();

			for(i=0;i<n;i++)
			{
				ALERT_LINE_ITEM* item = new ALERT_LINE_ITEM;//m_gAlarmArray.GetAt(pos);

				SYMBOL_ISALERTED sym;
				fl.Read(&sym,sizeof(SYMBOL_ISALERTED));//股票代码
				item->m_symbolAlarm = (sym);
				;

				DATA_PER_LINE line;
				int k=0;
				fl.Read(&k,4);
				for(j=0;j<k;j++)
				{
					fl.Read(&line,sizeof(DATA_PER_LINE));
					item->m_lineInfoArr .Add (line);
	//				fl.Read(&f,sizeof(float));
	//				item->m_pricePreArr .Add (f);//m_priceOnLineArr
	//				fl.Read(&f,sizeof(float));
	//				item->m_pricePreArr .Add (f);//m_priceOnLineArr
				}

				//test item data validity
				if(item->m_lineInfoArr .GetSize ()<=0)
				{
					delete item;continue;
				}
				else
				{
					for(j=0;j<item->m_lineInfoArr .GetSize ();j++)
					{
						//delete data invalid
						if(item->m_lineInfoArr[j].m_btAlarmed == 1)
						{
							item->m_lineInfoArr.RemoveAt(j);
							j--;
						}
					}
				}
				m_gLineAlarmArray.AddTail (item);//add
			}
		}

		//m_gAlarmResultList
		n = 0;
		int n2 = fl.Read(&n,sizeof(int));
		if(n2 == 4)
		{
//			ClearAllResult();
			for(int i=0;i<n;i++)
			{
				ALERT_RESULT_ITEM* itm = new ALERT_RESULT_ITEM;
				if(fl.Read(itm,sizeof(ALERT_RESULT_ITEM)) == sizeof(ALERT_RESULT_ITEM))
				{
					m_gAlarmResultList.AddTail(itm);
				}
				else
					break;
			}
		}
	}
	return true;
}

bool CAlertSystem::SaveFile()
{
	/*	文件头四个字节为文件标识号"WSYJ"，接着四字节为预警的个数。
	每个预警条件如下保存：第1-16字节为预警名称，第17个字节为条件之间的关系，
	第18个字节预留，第19到20字节为条件的个数，接着填充IndexDataInfo结构的数据，
	直到所有条件都填完；接着保存四字节附加设置条件；接着四字节为股票的个数，
	接着是股票代码，直到所有股票代码都填完（每个股票代码占8个字节，以\0结束）。
*/
	CFile fl;
	if(fl.Open("data\\AlarmResult.alm",CFile::modeCreate|CFile::shareDenyNone |CFile::modeWrite))
	{
		char ch[32]="W2YJ";
		fl.Write(ch,4);
		int n = m_gAlarmArray.GetCount ();
		fl.Write(&n,4);

		int i;
		for(i=0;i<n;i++)
		{
			POSITION pos=m_gAlarmArray.FindIndex (i);
			ALERT_SET_ITEM* item = m_gAlarmArray.GetAt(pos);
			fl.Write(item->m_strAlarmName ,16);//name

			BYTE bt = (BYTE)item->m_bAnd ;
			fl.Write(&bt,1);//and or:1 and ,0 or
			fl.Write(&bt,1);//and or:1 and ,0 or

			int n2 = item->m_conditionArray .GetSize ();
			fl.Write(&n2,4);
			fl.Write(item->m_conditionArray .GetData (),sizeof(IndexDataInfo)*n2);//condition 条件

			fl.Write(item->m_btAddtional ,4);//附加条件

			n2= item->m_symbolTypeArray.GetSize();
			fl.Write(&n2,4);
			fl.Write(item->m_symbolTypeArray .GetData () ,sizeof(SharesNameArr3)* n2);//股票类型
			n2= item->m_bAlarmedArray .GetSize();
			fl.Write(&n2,4);
			fl.Write(item->m_bAlarmedArray .GetData () ,sizeof(SYMBOL_ISALERTED)* n2);//股票代码
		}

//write m_gLineAlarmArray to file
		n = m_gLineAlarmArray.GetCount ();
		fl.Write(&n,4);

		for(i=0;i<n;i++)
		{
			POSITION pos=m_gLineAlarmArray.FindIndex (i);
			ALERT_LINE_ITEM* item = m_gLineAlarmArray.GetAt(pos);

			fl.Write(&(item->m_symbolAlarm ),sizeof(SYMBOL_ISALERTED));//股票代码//

			int k=item->m_lineInfoArr.GetSize ();
			fl.Write(&k,4);
			for(int j=0;j<k;j++)
			{
				fl.Write(&(item->m_lineInfoArr [j]),sizeof(DATA_PER_LINE));
//				fl.Write(&(item->m_pricePreArr[j]),sizeof(float));
			}
		}
		//m_gAlarmResultList
		n = m_gAlarmResultList.GetCount();
		fl.Write(&n,sizeof(int));
		{
			for(int i=0;i<n;i++)
			{
				POSITION pos=m_gAlarmResultList.FindIndex (i);
				ALERT_RESULT_ITEM* itm = m_gAlarmResultList.GetAt(pos);
				fl.Write(itm,sizeof(ALERT_RESULT_ITEM));
			}
		}

	}
	return true;
}

void CAlertSystem::CaclAlarm(CMainFrame *pFrame)
{
	if(CMainFrame::m_pDoc == 0)
		return;
	if(CMainFrame::m_pDoc ->m_bInitDone ==FALSE)
		return;
	if(pFrame->m_bAlarmExec == false)
		return;

	static int g_nTime = 0;
	int nLineAlarm = m_gLineAlarmArray.GetCount ();
	int nAlarm = m_gAlarmArray.GetCount ();
	if(g_nTime>=nLineAlarm+nAlarm)
		g_nTime = 0;

	if(g_nTime>=nAlarm)
	{
		//to calculate line alarm
		CaclLineAlarm(pFrame,g_nTime-nAlarm);
		g_nTime++;
	    return;
	}
	POSITION pos=m_gAlarmArray.FindIndex (g_nTime);
	ALERT_SET_ITEM* item = m_gAlarmArray.GetAt(pos);

	CArray<IndexDataInfo,IndexDataInfo&>& indexSaveArray = item->m_conditionArray ;
	bool bAnd = item->m_bAnd ;//m_and_or
	//
	if(indexSaveArray.GetSize() == 0)
	{
		g_nTime++;
	    return;
	}
  
    int stocknum = item->m_bAlarmedArray .GetSize();
	int index_num = indexSaveArray.GetSize();
    Kline *pKline=NULL;
	CBuySellList buySellList ;//need remove all

	int * pnNeedCount = new int [index_num];
	int i;
	for(i=0;i<index_num;i++)
	{
		IndexDataInfo index_save = indexSaveArray.GetAt(i);
		int nDays=index_save.iBeginFoot ;//CTaiScreenParent::GetCountValid(&index_save,pDoc,10);
		pnNeedCount[i]=nDays;
	}

	static int g_nStockCurrent = 0;
	DWORD dwTick = GetTickCount();
    for(i=g_nStockCurrent;i<stocknum;i++)//计算每一个股票
    {
		CTaiShanDoc* pDoc =CMainFrame::m_pDoc ;
		if(pDoc == NULL)
				break;
		if(pFrame->m_bAlarmExec == false)
			break;

//		if((i+1)%3 == 0)
		{
			DWORD dwTick2 = GetTickCount();
			if(dwTick2-dwTick>100)
			{
				g_nStockCurrent = i;
				goto END_2;
			}
		}
		bool bSucc;
		int nRead;
		CString StockId;
		StockId = item->m_bAlarmedArray.GetAt(i).m_sSymbol ;
		int stkKind = item->m_bAlarmedArray.GetAt(i).m_stkKind ;

		int result = 0 ;

		if(item->m_bAlarmedArray.GetAt(i).m_btAlarmed ==1 &&  item->m_btAddtional [2] == 0)
				continue;

	    for(int k=0;k<index_num;k++)//计算每一个指标
		{
			IndexDataInfo index_save = indexSaveArray.GetAt(k);
			int nDays=pnNeedCount[k];
			if(nDays<-1)
				continue;
			float a = 0;

			if(index_save.iKind == CTaiKlineTreeCtl::nKindPriceAlert 
				|| index_save.iKind == CTaiKlineTreeCtl::nKindUpDownAlert )
			{
				CReportData* dt1;
				if(pDoc->m_sharesInformation.Lookup(StockId.GetBuffer (0),dt1,stkKind))
				{
					CFormularContent* pJs = CTaiScreenParent::GetSpecialLJishu(-index_save.iKind-7);
					switch(index_save.iKind)
					{
					case CTaiKlineTreeCtl::nKindPriceAlert :
						if(pJs!=NULL)
						{
							if(dt1->nowp <= index_save.fParam [0] || dt1->nowp >= index_save.fParam [1] )
								a=1;
						}
						break;
					case CTaiKlineTreeCtl::nKindUpDownAlert :
						if(pJs!=NULL && dt1->ystc > 0 )
						{
							float fUp =( dt1->nowp - dt1->ystc )/dt1->ystc ;
							if(fUp <= index_save.fParam [0] || fUp >= index_save.fParam [1] )
								a=1;
						}
						break;
					}
				}
			}
			else
			{
			//当日或历史选股
			nRead = CTaiScreenParent::ReadKLine(StockId,stkKind, pKline, nDays,CTaiScreenParent::nLKind[index_save.iDataType],&buySellList, 48 ,TRUE);
			
			if(nRead < nDays || nDays<-1)
			{
				if(bAnd == 1)
				  continue;
				else
					break;
			}
			a = CTaiScreenParent::FormularComputeShares(StockId,stkKind , nRead,&index_save,bSucc,pKline, &buySellList,true);
			}
			  if(a == 1)
				  result++;
			  if(bAnd==1 &&result>0 ||bAnd==0 &&result<k+1)
				  break;
		}
		if(bAnd==0 &&result==index_num || bAnd ==1 &&result>0)
		{
			;//to do alarm now
			//add alarm data to list
			CReportData* dt1;

			//if need to alarm
//			if(item->m_bAlarmedArray.GetAt(i).m_btAlarmed ==0 || item->m_btAddtional [2] == 1)
			{
				if(pDoc->m_sharesInformation.Lookup(StockId.GetBuffer (0),dt1,stkKind))
				{
					AlarmSendMessage(pFrame, dt1,item,g_nTime);

					SYMBOL_ISALERTED symIs; 
					strcpy(symIs.m_sSymbol ,StockId);
					symIs.m_btAlarmed = 1;
					symIs.m_stkKind = stkKind;
					item->m_bAlarmedArray.SetAt(i,symIs);
				}
			}

		}
//		CWHApp::DispatchMessageEachTime();
    }
END_1:	g_nTime++;
		g_nStockCurrent = 0;
END_2:	CTaiKlineFileHS::RemoveHs(buySellList);
	delete [] pnNeedCount;

	if(pKline)
	{
		delete[] pKline;        //释放内存
		pKline=NULL;
	}


}

BOOL CAlertSystem::OnInitDialog() 
{
	CDialog::OnInitDialog();
	//m_btDelete.SetIcon(IDI_ICON_DELETE);
	//m_btAdd.SetIcon(IDI_ICON_ADD2);
	//m_btOK.SetIcon(IDI_ICON_OK);
	//m_btCancel.SetIcon(IDI_EXIT16);

	SetTimer(1,10000,NULL);
	
//	m_imageList.Create(IDB_BITMAP12,16,0,RGB(192,192,192));

	FillListctrlHeader();

//	LoadFile();

	CTaiShanDoc* pDoc =CMainFrame::m_pDoc ;
	CString sArr[10];
	int i;
	for(i=0;i<m_gAlarmArray.GetCount ();i++)
	{
		POSITION pos=m_gAlarmArray.FindIndex (i);//预警条件数组
		ALERT_SET_ITEM* item = m_gAlarmArray.GetAt(pos);

		//add to listctrl
		AlarmArrInsertRow(item);
	}
	 
//	m_condArrayList.SetExtendedStyle
//		(m_condArrayList.GetExtendedStyle()|LVS_EX_SUBITEMIMAGES);
/*	m_condArrayList.SetItemCount (50005);
	for(i=0;i<50000;i++)
	{
		InsertRow(&this->m_condArrayList , sArr, 2, 0);
	}*/

	//add alarm result to result listctrl
	m_alarmResultList.EnableUserRowColor(TRUE);
	int nCount = m_gAlarmResultList.GetCount ();
	m_alarmResultList.SetItemCount (nCount);
	for(i=0;i<nCount;i++)
	{
		ResultListAddRow(&this->m_alarmResultList ,i);
	}

//close alarm
//	SwitchAlarm();
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	if(pFrame->m_bAlarmExec == true)
	{
		GetDlgItem(IDC_BUTTON1)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON2)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON3)->EnableWindow(FALSE);
		GetDlgItem(IDOK)->SetWindowText("关闭预警");
	}
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CAlertSystem::GetSymbolArray(CArrayStockType &typeArr, SymbolKindArr &symbolArr)
{
	CTaiShanDoc *m_pDoc = CMainFrame::m_pDoc ;

	symbolArr.RemoveAll();
	int n = typeArr.GetSize ();//GetItemCount();

	//
	CTypedPtrMap<CMapStringToPtr,CString,int*> mapStringToInt;
	int* nMap;
	CString s;
	CString s2;
	for(int i=0;i<n;i++)
	{
		s = typeArr[i].m_sSymbol ;//->GetItemText (i,0);
		int k = typeArr[i].m_nType ;//GetItemData (i);
		if(k<0)
		{
			s2 = m_pDoc->GetStockKindString(-k-1);
			s = s2+s;
			if(!mapStringToInt.Lookup (s,nMap))
				mapStringToInt[s] = (int*)0;
		}
		if(k>=0 && k< CHOOSESTK)//2 ban maybe change
		{
			   int temp=m_pDoc->m_sharesInformation.GetStockTypeCount(k);
			   for(int j=0;j<temp;j++)
			   {
			 	    CReportData *Cdat;
				    m_pDoc->m_sharesInformation.GetStockItem(k,j,Cdat);
				    if(Cdat==NULL)
					   continue;
					s=Cdat->id ;
					s2 = m_pDoc->GetStockKindString(Cdat->kind );
					s= s2+s;
				    if(!mapStringToInt.Lookup (s,nMap))
						mapStringToInt[s] = (int*)0;
			   }
		}
		if(k==CHOOSESTK)//2 ban maybe change
		{
			  SymbolKindArr l_StockArray;
			  m_pDoc->m_ManagerBlockData.GetChooseStockCode(l_StockArray);
			  int nStk = l_StockArray.GetSize();
			  for(int j=0;j<nStk;j++)
			  {
				  s=l_StockArray[j].m_chSymbol ;
					s2 = m_pDoc->GetStockKindString(l_StockArray[j].m_nSymbolKind );
					s= s2+s;
				  if(!mapStringToInt.Lookup (s,nMap))
						mapStringToInt[s] = (int*)0;
			  }
		}
		if(k>=1000 && k<2000)
		{
				SymbolKindArr l_StockCodeArray;
				CStringArray l_StockTypeNameArray;
		        m_pDoc->m_ManagerBlockData.GetAllBlockNames(l_StockTypeNameArray);
				CString l_sStockTypeName=l_StockTypeNameArray.GetAt(k-1000);
				m_pDoc->m_ManagerBlockData.GetStockFromBlockName(l_StockCodeArray,l_sStockTypeName.GetBuffer(0));          
				int DisplayCount=l_StockCodeArray.GetSize();
				for(int row=0 ;row <  (int)DisplayCount  ;row++)           //更新一屏数据
				{    
					 s = l_StockCodeArray[row].m_chSymbol ; 
					s2 = m_pDoc->GetStockKindString(l_StockCodeArray[row].m_nSymbolKind );
					s= s2+s;
					 if(!mapStringToInt.Lookup (s,nMap))
						mapStringToInt[s] = (int*)0;
				}
		}
		if(k>=2000&&k<3000)
		{
			SymbolKindArr sArr;
			GetScreenStockBlockSymbol(sArr, s,k);
			int n=sArr.GetSize ();
			for(int j=0;j<n;j++)
			{
				s = sArr[j].m_chSymbol ; 
				s2 = m_pDoc->GetStockKindString(sArr[j].m_nSymbolKind );
				s= s2+s;
				if(!mapStringToInt.Lookup (s,nMap))
					mapStringToInt[s] = (int*)0;
			}
		}

		if(k==3000)//当前页
		{
			POSITION pos=m_pDoc->m_StockTypeMap[2].GetStartPosition ();  
			while(pos)
			{
				CString zqdm;
				CString ss;
				m_pDoc->m_StockTypeMap[2] .GetNextAssoc(pos,zqdm,ss);
				s=zqdm;
				if(!mapStringToInt.Lookup (s,nMap))
						mapStringToInt[s] = (int*)0;
			}//例如，为安全起见，我从加密的文件中生成一内存块，此内存块的数据相当于一可执行程序文件的内容。那么，怎样用此内存块执行程序？谢谢！
		}

	}

	//get array
	
	POSITION position=mapStringToInt.GetStartPosition ();//GetHeadPosition();
	while(position)
	{
		mapStringToInt.GetNextAssoc (position,s,nMap);
		SymbolKind sm;
		CString s3 = s.Left (2);
		sm.m_nSymbolKind = atoi(s3);
		s.Delete (0,2);
		strcpy(sm.m_chSymbol ,s);
		symbolArr.Add (sm);
	}
	//end

}

void CAlertSystem::FillListctrlHeader()
{
	m_condArrayList.SetExtendedStyle(m_condArrayList.GetExtendedStyle()|LVS_EX_FULLROWSELECT);

   	CRect           rect;
	CString			strItem1[]= {_T("预警条件"),_T("监控股票"),_T("预警数")};
	int nCol = 3;
	int gap = 2;
	this->m_condArrayList .SetImageList(pDoc->m_imagelist,LVSIL_SMALL);
	int i;
	for(i=0;i<nCol;i++)
	{
		this->m_condArrayList .InsertColumn(i, strItem1[i], LVCFMT_LEFT,
			125, i);
	}
	// case list

	//result list
	m_alarmResultList.SetExtendedStyle(m_alarmResultList.GetExtendedStyle()|LVS_EX_FULLROWSELECT);
	CString strItem[]= {_T("股票代码"),_T("预警条件"),_T("预警时间"),__T("预警价格"),_T("现价/盈亏")};
//	CString strItem[]= {_T("股票名称"),_T("预警价格"),_T("预警名称"),_T("当前价格"),_T("预警时间")};
	nCol = 5;
	this->m_alarmResultList  .SetImageList(pDoc->m_imagelist,LVSIL_SMALL);
	int nWid[] = {75,75,120,75,120};
	for( i=0;i<nCol;i++)
	{
		this->m_alarmResultList .InsertColumn(i, strItem[i], LVCFMT_LEFT,
			nWid[i], i);
	}

}
//插入一行字符串
void CAlertSystem::InsertRow(CListCtrl *pList, CString *strPt, int nStr, int nImag,int nPos,int stkKind )
{
	LV_ITEM  lvitem;
	
	CXTListCtrl *pList1 = (CXTListCtrl *)pList;
	int n = pList->GetItemCount ();

	lvitem.mask = LVIF_TEXT|LVIF_IMAGE ;
	if(nPos == -1)
		lvitem.iItem =n;
	else//modify at nPos
	{
		if(nPos >=n)
			return;
		lvitem.iItem =nPos;
	}
	lvitem.iSubItem = 0;
	lvitem.iImage = nImag;
	lvitem.pszText=strPt[0].GetBuffer (0);
	strPt[0].ReleaseBuffer ();
	int k ;

	if (nStr > 4)
	{
		CXTListBase::ROWCOLOR rowColor;
		rowColor.iRow = lvitem.iItem;
		rowColor.crBack = RGB(255,255,255);
		rowColor.crText = RGB(0,0,0);

		CString value;
		CString strBonus = strPt[4];
		int index = strBonus.Find('/');
		if(index != -1)
		{
			value = strBonus.Mid(index + 1,1);
			if(value == "-")//负
				rowColor.crText = RGB(0,255,0);
			else
			{
				//value = strBonus.Mid(index + 4,1);		
				if(value != "0" || strBonus[index + 4] != '0' || strBonus[index + 3] != '0')
					rowColor.crText = RGB(255,0,0);
			}

			pList1->SetRowColor(&rowColor);	
		}
	}
	if(nPos == -1)
		k= pList->InsertItem(&lvitem);
	else//modify at nPos
		k= pList->SetItem(&lvitem);
	if(k==-1)
		return;



	for(int i=1;i<nStr;i++)
	{
		lvitem.iSubItem = i;
	//	lvitem.iImage = -1;
		lvitem.pszText=strPt[i].GetBuffer (0);;
		strPt[i].ReleaseBuffer ();
		pList->SetItem(&lvitem);
	}

	pList->SetItemData (lvitem.iItem,stkKind);

}

void CAlertSystem::ListCtrlToSymbolType(CArrayStockType &typeArr, CListCtrl *pList)
{
	typeArr.RemoveAll ();

//	CTaiShanDoc *m_pDoc = CMainFrame::m_pDoc ;
	int  iIcon=0, iItem=0, iSubItem=0;
	
    int n = pList->GetItemCount();
	for(int item = 0;item<n;item++)
	{
		SharesNameArr3 stockType;
		
		pList->GetItemText (item,0,stockType.m_sSymbol ,18);
		stockType.m_nType = pList->GetItemData (item);
		typeArr.Add (stockType);
	} 
}

void CAlertSystem::OnCancel() 
{
	// TODO: Add extra cleanup here
	DestroyWindow();
//	CDialog::OnCancel();
}

void CAlertSystem::OnOK() 
{
	SwitchAlarm();
	// TODO: Add extra validation here
//	CDialog::OnOK();
}

void CAlertSystem::StartAlarm(bool bFirst)
{
#ifdef OEM
	return;
#endif

	CMainFrame* pFm = (CMainFrame* )AfxGetMainWnd();
//	#define TJXG_ALERT  400    /////The Tine Event of Choose_stock_Alarm  CAlertSystem::LoadFile()
	CTaiShanDoc *pDoc = CMainFrame::m_pDoc ;
//	if(bFirst == true )
//		LoadFile();

	if(bFirst == true && pDoc->m_propertyInitiate.bRunAlarmBegin != TRUE)
		return ;

	pFm->KillTimer (TJXG_ALERT);
	int nTime = pDoc->m_propertyInitiate.nSecondsAlarm*1000;

	int nItem = m_gAlarmArray.GetCount ();
	if(nItem == 0 )
		return;

	nTime /=nItem;//定时间隔(ms)
	if(nTime<500)
		nTime = 500;

	pFm->SetTimer(TJXG_ALERT,nTime,NULL);
	pFm->m_bAlarmExec = true;
	
}

void CAlertSystem::PostNcDestroy() 
{
	// TODO: Add your specialized code here and/or call the base class
	delete this;
	CMainFrame* pFm = (CMainFrame* )AfxGetMainWnd();
	pFm->m_pDlgAlarm =NULL;
	CDialog::PostNcDestroy();
}

void CAlertSystem::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	DestroyWindow();
	
//	CDialog::OnClose();
}

void CAlertSystem::CreateNew()
{
	CMainFrame* pFm = (CMainFrame* )AfxGetMainWnd();
	if(pFm->m_pDlgAlarm == NULL)
	{
		CAlertSystem* dlg = new CAlertSystem;
		pFm->m_pDlgAlarm = dlg;
		dlg->Create ( IDD_MYALERT_SYSTEM ,pFm);
	}
	pFm->m_pDlgAlarm ->ShowWindow (SW_RESTORE);

}

void CAlertSystem::ResultListAddRow(CListCtrl* pList,int nRow)
{
	CTaiShanDoc* pDoc =CMainFrame::m_pDoc ;
	POSITION pos=m_gAlarmResultList.FindIndex (nRow);
	ALERT_RESULT_ITEM* item = m_gAlarmResultList.GetAt(pos);
	//add to listctrl
	CString sArr[10];
	sArr[0]=item->m_symbol ;//stock symbol
	sArr[3].Format ("%.2f",item->m_fPrice);//alarm price

	if(item->m_nFootAlarmArr >=0)
//	if(nRow>=0)
	{
		pos=m_gAlarmArray.FindIndex (item->m_nFootAlarmArr );
		ALERT_SET_ITEM* itemCond = m_gAlarmArray.GetAt(pos);
		sArr[1]=itemCond->m_strAlarmName  ;//alarm name
	}
	else
		sArr[1]="趋势线预警" ;//alarm name

	//now price
	CReportData* dt1;
	float fPrc = 0;
	if(pDoc->m_sharesInformation.Lookup(sArr[0].GetBuffer (0),dt1,item->m_stkKind ))
		fPrc= dt1->nowp ;
//	sArr[2].ReleaseBuffer ();
	if(item->m_fPrice!=0)
		sArr[4].Format ("%.2f/%.2f%%",fPrc,(fPrc-item->m_fPrice)*100/item->m_fPrice);
	else
		sArr[4].Format ("%.2f/0.00%%",fPrc);

	//alarm time

	sArr[2] = CTime(item->m_time ).Format ("%m/%d %H:%M:%S");

	InsertRow(pList  , sArr, 5, 13,-1,dt1->kind );

}

void CAlertSystem::CaclLineAlarm(CMainFrame *pFrame, int nLineAlarm)
{
	CTaiShanDoc* pDoc =CMainFrame::m_pDoc ;
	POSITION pos=m_gLineAlarmArray.FindIndex (nLineAlarm);
	ALERT_LINE_ITEM* item = m_gLineAlarmArray.GetAt(pos);

	static int g_nToday = 0;
	CTime tmNow = CTime::GetCurrentTime ();
	int nToday = atoi(tmNow.Format ("%Y%m%d"));

    int stocknum = 1;
	int index_num = item->m_lineInfoArr .GetSize ();
    Kline *pKline=NULL;
	CBuySellList buySellList ;//need remove all
	
	if(pDoc == NULL)
		return;
	int k;
	CString StockId;
	StockId = item->m_symbolAlarm .m_sSymbol ;
	int stkKind =  item->m_symbolAlarm .m_stkKind ;
	int result = 0 ;

//	int tmLastPer[48];
	if(nToday != g_nToday)///重新计算趋势线上昨日距离趋势线起始点的周期个数
	{
		GetLineAlarmInitData(item);
		g_nToday = nToday;
	}

//计算趋势线上今日距离趋势线起始点的周期个数
	for(k=0;k<index_num;k++)//计算趋势线
	{
		if(item->m_lineInfoArr [k] .m_btAlarmed  == 1)
			continue;
		if(item->m_lineInfoArr [k].m_gapFromBeginArr == 0)
			continue;
		int nNow;
		if(item->m_lineInfoArr [k].m_gapFromBeginArr == 0)
			nNow= -item->m_lineInfoArr [k].m_gapFromBeginArr ;
		else
		{
			int nAdd = 0;
			if(item->m_lineInfoArr [k].lineInfo.klineType >0 && item->m_lineInfoArr [k].lineInfo.klineType < 5)
			{//bool bMIn5 = true;
				int isSz=0;
				if(CSharesCompute::GetMarketKind(stkKind) == SZ_MARKET_EX) isSz=1;
				int nFoot;
				if(pDoc->m_bInitDone ==FALSE)
					nFoot=0;
				else
					nFoot=pDoc->m_nANT [isSz];

				int nRate = 5;
				switch (item->m_lineInfoArr [k].lineInfo.klineType )
				{
				case 1:
					nRate = 5;
					break;
				case 2:
					nRate = 15;
					break;
				case 3:
					nRate = 30;
					break;
				case 4:
					nRate = 60;
					break;
				}
				nAdd = (nFoot+1)/nRate + (nFoot+1)%nRate == 0? 0:1;
			}
			nNow += nAdd;
		}

		//compute nowprice on line
		if(item->m_lineInfoArr [k].lineInfo.time2 <=0)
			continue;

		CReportData * pdt;
		if(pDoc->m_sharesInformation.Lookup(StockId.GetBuffer (0),pdt,stkKind))
		{
			float fPriceNow = pdt->nowp ;
			if(fPriceNow>0)
			{
				float fNow = (item->m_lineInfoArr [k].lineInfo.valFlt2 - item->m_lineInfoArr [k].lineInfo.valFlt1 )/item->m_lineInfoArr [k].lineInfo.time2 *nNow+item->m_lineInfoArr [k].lineInfo.valFlt1 ;
				bool bOk = false;
				if(item->m_lineInfoArr [k] .m_bUpwards == true && fPriceNow>=fNow)
					bOk = true;
				if(item->m_lineInfoArr [k] .m_bUpwards == false && fPriceNow<=fNow)
					bOk = true;
				if(bOk == true)//alarm now
				{
					AlarmSendMessage(pFrame, pdt,NULL,-1);
					item->m_lineInfoArr [k] .m_btAlarmed = 1;
				}
			}
		}
		StockId.ReleaseBuffer ();


	}
  
	CTaiKlineFileHS::RemoveHs(buySellList);

	if(pKline)
	{
		delete[] pKline;        //释放内存
		pKline=NULL;
	}


}

void CAlertSystem::AlarmSendMessage(CMainFrame *pFrame, CReportData *dt1, ALERT_SET_ITEM *item,int nCondFoot)
{
	if(dt1==0)
		return ;
	if(dt1->nowp<=0)
		return;
	//add alarm data to list
	ALERT_RESULT_ITEM* pItem = new ALERT_RESULT_ITEM;
	
	pItem->m_nFootAlarmArr =nCondFoot;//000
	CString StockId = dt1->id ;
	strcpy(pItem->m_symbol , dt1->id );//item->m_strAlarmName 
	pItem->m_time = CTime::GetCurrentTime ().GetTime ();
	pItem->m_fPrice = dt1->nowp ;
	pItem->m_stkKind = dt1->kind ;
	m_gAlarmResultList.AddTail (pItem);
	if(m_gAlarmResultList.GetCount ()> 5000)
		delete m_gAlarmResultList.RemoveHead ();


	//add result to list
	if(pFrame->m_pDlgAlarm !=NULL)
	{
		int nCount = pFrame->m_pDlgAlarm->m_alarmResultList.GetItemCount( );
		int nCount2 = m_gAlarmResultList.GetCount ();
		if(nCount2>0)
		{
			ResultListAddRow(&pFrame->m_pDlgAlarm->m_alarmResultList ,nCount2-1);
			if(nCount> 2000)
				pFrame->m_pDlgAlarm->m_alarmResultList.DeleteItem(0);
		}
	}

	bool bFirst = true;
	//beep message
	bool bSend = true;
	if(item!=NULL)
	{
		if(item->m_btAddtional [0] ==0)
			bSend = false;
		static int nFoot =-1;
		if(nCondFoot == nFoot)
			bFirst = false;
		nFoot = nCondFoot;
	}
	if(bSend == true)
	{
		static DWORD dw = 0;

		DWORD dw2 = GetTickCount();
		if(dw2-dw>2000)
		{
			::MessageBeep (-1);
			dw = dw2;
		}
//		::MessageBeep (-1);
	}
	//show info dlg
//	bSend = true;
//	if(item!=NULL)
//		if(item->m_btAddtional [1] ==0)
//			bSend = false;
	if(bSend == true )
	{
		CString s = StockId+": ";
		CAlertPopDlg* dlg;
		if(pFrame->m_pDlgAlarmPop != NULL)//CAlertPopDlg
		{
			pFrame->m_pDlgAlarmPop->ShowWindow(SW_SHOW);
		}
		else
		{
			dlg = new CAlertPopDlg;
			dlg->Create(IDD_1UPALERT );
			dlg->ShowWindow(SW_SHOW);
			::MessageBeep (-1);
		}
		if(pFrame->m_pDlgAlarmPop != NULL)//CAlertPopDlg
		{
			CString s2;
			POSITION pos=m_gAlarmArray.FindIndex (pItem->m_nFootAlarmArr );
			CString s3 = "";
			if(pos)
			{
				ALERT_SET_ITEM* itemCond = m_gAlarmArray.GetAt(pos);

				s3=itemCond->m_strAlarmName;//alarm name
				s3 = s3+" "  ;
			}
			s2.Format ("%s%.2f",s3 , pItem->m_fPrice );
			s+=s2;
			//dt1->kind ;
			pFrame->m_pDlgAlarmPop->m_conInfo.AddString (s);
			int nC = pFrame->m_pDlgAlarmPop->m_conInfo.GetCount();
			if(nC>0) pFrame->m_pDlgAlarmPop->m_conInfo.SetItemData (nC-1,dt1->kind );

			if(pFrame->m_pDlgAlarmPop->m_conInfo.GetCount()>500)
				pFrame->m_pDlgAlarmPop->m_conInfo.DeleteString( 0 );

		}

	}
}

void CAlertSystem::AddLineAlarmItem(INFO_LINESELF &lineInfo,CString s,bool bUpwards)
{
//	ALERT_LINE_ITEM* item = new ALERT_LINE_ITEM;//m_gLineAlarmArray.GetAt(pos);
	ALERT_LINE_ITEM* item = NULL;
	bool bFound = false;
	for(int i=0;i<m_gLineAlarmArray.GetCount ();i++)
	{
		POSITION pos=m_gLineAlarmArray.FindIndex (i);
		item = m_gLineAlarmArray.GetAt(pos);
		CString s2 = item->m_symbolAlarm .m_sSymbol ;
		if(s==s2)
		{
			bFound = true;
			for(int j=0;j<item->m_lineInfoArr .GetSize ();j++)
			{
				if(item->m_lineInfoArr[j].lineInfo.time1 == lineInfo.time1 &&  item->m_lineInfoArr[j].lineInfo.valFlt1  == lineInfo.valFlt1  )
				{
					if(item->m_lineInfoArr[j].lineInfo.time2 == lineInfo.time2 &&  item->m_lineInfoArr[j].lineInfo.valFlt2  == lineInfo.valFlt2  )
					{
						item->m_lineInfoArr[j] .m_btAlarmed  = 0;
						item->m_lineInfoArr[j] .m_bUpwards  = bUpwards;
						
						//SaveFile();
						GetLineAlarmInitData(item);
						return;
					}
				}
			}
			DATA_PER_LINE dataLine;//lineInfo
			dataLine.lineInfo = lineInfo;
			dataLine.m_btAlarmed =0;
			dataLine.m_bUpwards = bUpwards;

			item->m_lineInfoArr .Add (dataLine);
			//SaveFile();

			break;
		}
	}

	if(bFound == false)
	{
		item = new ALERT_LINE_ITEM;
		strcpy(item->m_symbolAlarm .m_sSymbol ,s);
		DATA_PER_LINE dataLine;//lineInfo
		dataLine.lineInfo = lineInfo;
		dataLine.m_btAlarmed =0;
		dataLine.m_bUpwards = bUpwards;

		item->m_lineInfoArr .Add (dataLine);
		m_gLineAlarmArray.AddTail (item);
		//SaveFile();
	}
	GetLineAlarmInitData(item);
}

void CAlertSystem::DeleteLineAlarmItem(INFO_LINESELF &lineInfo, CString s)
{
	ALERT_LINE_ITEM* item = NULL;
	for(int i=0;i<m_gLineAlarmArray.GetCount ();i++)
	{
		POSITION pos=m_gLineAlarmArray.FindIndex (i);
		item = m_gLineAlarmArray.GetAt(pos);
		CString s2 = item->m_symbolAlarm .m_sSymbol ;
		if(s==s2)
		{
			for(int j=0;j<item->m_lineInfoArr .GetSize ();j++)
			{
				if(item->m_lineInfoArr[j].lineInfo.time1 == lineInfo.time1 &&  FABSMY(item->m_lineInfoArr[j].lineInfo.valFlt1  - lineInfo.valFlt1)<0.1  )
				{
					if(item->m_lineInfoArr[j].lineInfo.time2 == lineInfo.time2 &&  FABSMY(item->m_lineInfoArr[j].lineInfo.valFlt2  - lineInfo.valFlt2)<0.1  )
					{
						item->m_lineInfoArr.RemoveAt(j);
						//SaveFile();
						return;
					}
				}
			}
			break;
		}
	}


}

void CAlertSystem::GetLineAlarmInitData(ALERT_LINE_ITEM* item)
{
	CTaiShanDoc* pDoc =CMainFrame::m_pDoc ;

    int stocknum = 1;
	int index_num = item->m_lineInfoArr .GetSize ();
	if(pDoc == NULL)
		return;
	int nRead,k;
	CString StockId;
	StockId = item->m_symbolAlarm .m_sSymbol ;
	int stkKind = item->m_symbolAlarm .m_stkKind ;
	int result = 0 ;

	//重新计算趋势线上昨日距离趋势线起始点的周期个数
	{
		Kline *pKline=NULL;
		CBuySellList buySellList ;//need remove all
		CTime tmNow = CTime::GetCurrentTime ();
		int nToday = atoi(tmNow.Format ("%Y%m%d"));

	    for(k=0;k<index_num;k++)//计算趋势线
		{
			int nDays=-1;
			int tmLineB = item->m_lineInfoArr [k].lineInfo.time1 ;
			bool bMIn5 = true;
			if(item->m_lineInfoArr [k].lineInfo.klineType >4 && item->m_lineInfoArr [k].lineInfo.klineType < 9)
			{
				tmLineB = atoi(CTime(tmLineB).Format ("%Y%m%d"));
				bMIn5 = false;
			}

			// read kline data
			nRead = CTaiScreenParent::ReadKLine(StockId,stkKind, pKline, nDays,item->m_lineInfoArr [k].lineInfo.klineType ,&buySellList, 48 ,FALSE);
			
			if(nRead<=0)
			{
				item->m_lineInfoArr [k].m_gapFromBeginArr= 0;
				continue;
			}

			int nFlag = 1;//没有今日数据
			if(atoi(CTime(pKline[nRead-1].day).Format ("%Y%m%d")) >= nToday)
				nFlag = -1;//有今日数据

			int nAdd = 0;
			if(nFlag == 1 && bMIn5 == false)
			{
				CTime tm(pKline[nRead-1].day);
				int nYear = tm.GetYear ();
				int nMon = tm.GetMonth  ();
				int nDay = tm.GetDay  ();
				int nDayOfWeek=tmNow.GetDayOfWeek ()-1;

				CTime tm2(nYear,nMon,nDay,0,0,0);
				CTimeSpan tmSpan = tmNow-tm2;
				switch (item->m_lineInfoArr [k].lineInfo.klineType )
				{
				case 5:
					nAdd = 1;
					break;
				case 6:
					if(nDayOfWeek - tmSpan.GetDays ()>0)
						nAdd=1;
					break;
				case 7:
					if(nYear!=tmNow.GetYear () || nMon!=tmNow.GetMonth () )
						nAdd=1;
					break;
				case 8:
					if(nYear!=tmNow.GetYear () )
						nAdd=1;
					break;
				}

			}

			int j=0;
			while(nRead-1-j>=0)
			{
				int tmb = pKline[nRead-1-j].day ;
				if(bMIn5 == false)
					tmb = atoi(CTime(tmb).Format ("%Y%m%d"));

				if(tmb<=tmLineB)
					break;
				j++;
			}
			item->m_lineInfoArr [k].m_gapFromBeginArr=(j+nAdd)*nFlag;
		}
		CTaiKlineFileHS::RemoveHs(buySellList);

		if(pKline)
		{
			delete[] pKline;        //释放内存
			pKline=NULL;
		}
	}

}

void CAlertSystem::AlarmArrInsertRow(ALERT_SET_ITEM *item,int nItem)
{
//static CArray<ALERT_SET_ITEM,ALERT_SET_ITEM&> m_gAlarmArray;//预警条件数组

	//add to listctrl
	CString sArr[10];
	sArr[0]=item->m_strAlarmName ;
	sArr[1].Format ("%d",item->m_bAlarmedArray .GetSize () );
	int nNotAlarmed = 0;
	for(int i=0;i<item->m_bAlarmedArray .GetSize();i++)
	{
		if(item->m_bAlarmedArray [i].m_btAlarmed == 0)
			nNotAlarmed ++;
	}
	sArr[2].Format ("%d",item->m_bAlarmedArray .GetSize ()-nNotAlarmed);
	InsertRow(&this->m_condArrayList , sArr, 3, 10,nItem);

	//SaveFile();

}

void CAlertSystem::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	if(nIDEvent == 1)
	{
		int i;
		for(i=0;i<m_gAlarmArray.GetCount ();i++)
		{
			POSITION pos=m_gAlarmArray.FindIndex (i);//预警条件数组
			ALERT_SET_ITEM* item = m_gAlarmArray.GetAt(pos);

			CString s="";
			int nNotAlarmed = 0;
			for(int j=0;j<item->m_bAlarmedArray .GetSize();j++)
			{
				if(item->m_bAlarmedArray [j].m_btAlarmed == 0)
					nNotAlarmed ++;
			}
			s.Format ("%d",item->m_bAlarmedArray .GetSize()-nNotAlarmed);
			m_condArrayList.SetItemText (i,2,s);
		}

//		static int g_nFresh = 0;
//		g_nFresh++;
//		if(g_nFresh% ==0)
		{
			int n = m_alarmResultList.GetItemCount();
			for(i=0;i<n;i++)
			{
				CReportData* dt1;
				float fPrc = 0;
				CString s = m_alarmResultList.GetItemText (i,0);
//				CTaiShanDoc* pDoc =CMainFrame::m_pDoc ;
//				ASSERT(m_gAlarmResultList.GetCount() == n);
//				POSITION pos=m_gAlarmResultList.FindIndex (i);
//				ALERT_RESULT_ITEM* item = m_gAlarmResultList.GetAt(pos);
				int stkKind = m_alarmResultList.GetItemData (i);
				CXTListBase::ROWCOLOR rowColor;
				rowColor.iRow = i;
				rowColor.crBack = RGB(255,255,255);
				rowColor.crText = RGB(0,0,0);

				CString value;
				CString strBonus = m_alarmResultList.GetItemText (i,4);
				int index = strBonus.Find('/');
				if(index != -1)
				{
					value = strBonus.Mid(index + 1,1);
					if(value == "-")//负
						rowColor.crText = RGB(0,255,0);
					else
					{
						//value = strBonus.Mid(index + 4,1);		
						if(value != "0" || strBonus[index + 4] != '0' || strBonus[index + 3] != '0')
							rowColor.crText = RGB(255,0,0);
					}
					m_alarmResultList.SetRowColor(&rowColor);	
				}

				if(pDoc->m_sharesInformation.Lookup(s.GetBuffer (0),dt1,stkKind))
				{
					fPrc= dt1->nowp ;
					
					s.ReleaseBuffer ();
					s.Format ("%.2f",fPrc);
					m_alarmResultList.SetItemText (i,3,s);
				}
			}
		}

	}
	
	CDialog::OnTimer(nIDEvent);
}

void CAlertSystem::GetScreenStockBlockSymbol(SymbolKindArr &sArr, CString sBlockName,int k)
{
	sArr.RemoveAll ();
	CString s = sBlockName;

	CTaiShanDoc *m_pDoc = CMainFrame::m_pDoc ;
	POSITION position=m_pDoc->m_Tjxgblocklist.GetHeadPosition();
	int j=0;
	BLOCKSTOCK * blocknow=NULL;
	bool bFind = false;
	while(position)
	{
		blocknow=m_pDoc->m_Tjxgblocklist.GetNext(position);
		CString s2 = blocknow->blockname;
		s2.MakeLower ();
		s.MakeLower ();
		if(s2 == s || k>=0 && j==k-2000)
		{
			bFind = true;
			break;
		}
		j++;
	}

	if(bFind == true)
	{
		POSITION position=blocknow->stocklist.GetHeadPosition();
		int j=0;
//				BLOCKSTOCK * blocknow;
		SharesSymbol* pStockSymbol = NULL;
		while(position)
		{
			SymbolKind sm;
			pStockSymbol=blocknow->stocklist.GetNext(position);
			s = pStockSymbol->sharesymbol; 
			strcpy(sm.m_chSymbol ,s);
			sm.m_nSymbolKind  = pStockSymbol->nKind ;
			CString s2 = s;
			CReportData* Cdat;
		 	if (m_pDoc->m_sharesInformation.Lookup(s.GetBuffer (0), Cdat,sm.m_nSymbolKind ) == TRUE)     //检测该股票是否已记录在内存模板中
				sArr.Add (sm);
		}
	}

}

void CAlertSystem::SwitchAlarm()
{
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	pFrame->m_wndStatusBar.InvalidateRect(pFrame->m_wndStatusBar.m_aPaneRect[5]);//m_aPaneRect[5]
	if(pFrame->m_bAlarmExec == false)
	{
		StartAlarm(false);//bool bFirst
		if(pFrame->m_pDlgAlarm !=NULL&& pFrame->m_bAlarmExec == true)
		{
			pFrame->m_pDlgAlarm ->GetDlgItem(IDC_BUTTON1)->EnableWindow(FALSE);
			pFrame->m_pDlgAlarm ->GetDlgItem(IDC_BUTTON2)->EnableWindow(FALSE);
			pFrame->m_pDlgAlarm ->GetDlgItem(IDC_BUTTON3)->EnableWindow(FALSE);
			pFrame->m_pDlgAlarm ->GetDlgItem(IDOK)->SetWindowText("关闭预警");
		}
	}
	else
	{
		pFrame->KillTimer (TJXG_ALERT);
		if(pFrame->m_pDlgAlarm !=NULL)
		{
			pFrame->m_pDlgAlarm ->GetDlgItem(IDC_BUTTON1)->EnableWindow();
			pFrame->m_pDlgAlarm ->GetDlgItem(IDC_BUTTON2)->EnableWindow();
			pFrame->m_pDlgAlarm ->GetDlgItem(IDC_BUTTON3)->EnableWindow();
			pFrame->m_pDlgAlarm ->GetDlgItem(IDOK)->SetWindowText("启动预警");
		}
		pFrame->m_bAlarmExec = false;
	}

}

BOOL CAlertSystem::OnHelpInfo(HELPINFO* pHelpInfo) 
{
	// TODO: Add your message handler code here and/or call default
	DoHtmlHelp(this);return TRUE;
	
//	HtmlHelp(m_hWnd,"stock.chm",HH_HELP_CONTEXT,2);//CAlertSystem::IDD);
//	return CDialog::OnHelpInfo(pHelpInfo);
}

void CAlertSystem::ClearAllResult()
{
	CString Id;
	POSITION pos=m_gAlarmResultList.GetHeadPosition ();//
	while(pos)
	{
		delete m_gAlarmResultList.GetNext (pos);// (temp);
	}
	m_gAlarmResultList.RemoveAll();

	m_alarmResultList.DeleteAllItems ();

}

void CAlertSystem::OnNMDblclkList2(NMHDR *pNMHDR, LRESULT *pResult)
{
	CMainFrame* pFm = (CMainFrame* )AfxGetMainWnd();
	CString rString;
	int n = m_alarmResultList.GetSelectedCount();
	if(n==0)
		return;


	POSITION pos;
	pos = m_alarmResultList.GetFirstSelectedItemPosition();
	while (pos)
	{
		int nItem = m_alarmResultList.GetNextSelectedItem(pos);

		rString = m_alarmResultList.GetItemText (nItem,0);
		if(rString.GetLength ()==4 || rString.GetLength ()==6)
		{
			int stkKind = m_alarmResultList.GetItemData (nItem);
			pFm->SendMessage (WM_USER_DrawKlineWindow,(WPARAM)rString.GetBuffer (0),stkKind*10);
			DestroyWindow();
		}

	}

	*pResult = 0;
}
