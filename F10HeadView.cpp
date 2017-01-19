// F10HeadView.cpp : 实现文件
//

#include "stdafx.h"
#include "WH.h"
#include "F10HeadView.h"
#include "MyRichView.h"

#include "MainFrm.h"
#include "ChildFrm.h"
#include "CTaiShanDoc.h"



// CF10HeadView

IMPLEMENT_DYNCREATE(CF10HeadView, CFormView)

CF10HeadView::CF10HeadView()
	: CFormView(CF10HeadView::IDD)
{
	m_sCurrSymbol = "";
	m_sPrevSymbol = "";
	m_sNextSymbol = "";
	m_sCurrItemName = "";

	m_pF10Data = NULL;
	m_pTaiTestRichView = NULL;


}

CF10HeadView::~CF10HeadView()
{
	ReleaseSources();
	((CMainFrame *)AfxGetMainWnd())->m_EnableF10 = true;
}

void CF10HeadView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CF10HeadView, CFormView)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_RADIO1, IDC_RADIO1 + 20, OnBnClickedRadioItem )

	ON_WM_CREATE()
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(IDC_F10_PREV, &CF10HeadView::OnBnClickedF10Prev)
	ON_BN_CLICKED(IDC_F10_NEXT, &CF10HeadView::OnBnClickedF10Next)
END_MESSAGE_MAP()


// CF10HeadView 诊断

#ifdef _DEBUG
void CF10HeadView::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CF10HeadView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CF10HeadView 消息处理程序

void CF10HeadView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

}

void CF10HeadView::OnBnClickedRadioItem(UINT nID)
{
	int iItem;
	iItem = nID - IDC_RADIO1;

	if(iItem >= 30)
	{
		return;
	}
	//显示第一项的内容
	if(m_arrItemName.GetCount())
		m_sCurrItemName = m_arrItemName[iItem];
	else
		m_sCurrItemName = "";
	ShowFileText(m_sCurrItemName);

}

void CF10HeadView::GetAllF10FileName(void)
{
	CString filename;
	CFileFind finder;
	CString sCurrFile;

	CMainFrame* pFm = (CMainFrame*)AfxGetMainWnd();
	CTaiShanDoc* pDoc = pFm->m_pDoc ;

	sCurrFile = pDoc->m_sharesSymbol;
	sCurrFile.MakeUpper();

	BOOL bWorking = finder.FindFile(g_sF10sh + "*.txt");
	while(bWorking)
	{
		bWorking = finder.FindNextFile();
		filename = finder.GetFileTitle();
		filename.MakeUpper();
		if (sCurrFile == filename)
		{
			//当前股票代码
			m_sCurrSymbol = "1" + pDoc->m_sharesSymbol;
		}
		int iValue = atoi(filename.GetBuffer(0));
		iValue += 1000000;
		m_vAllF10FileName.push_back(iValue);
		filename.ReleaseBuffer();
	}
	bWorking = finder.FindFile(g_sF10sz + "*.txt");
	while(bWorking)
	{
		bWorking = finder.FindNextFile();
		filename = finder.GetFileTitle();
		filename.MakeUpper();
		if (sCurrFile == filename)
		{
			//当前股票代码
			m_sCurrSymbol = "2" + pDoc->m_sharesSymbol;
		}

		int iValue = atoi(filename.GetBuffer(0));
		iValue += 2000000;
		m_vAllF10FileName.push_back(iValue);
		filename.ReleaseBuffer();
	}
	//排序
	sort(m_vAllF10FileName.begin(),m_vAllF10FileName.end());
}

int CF10HeadView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFormView::OnCreate(lpCreateStruct) == -1)
		return -1;

	CMainFrame* pFm = (CMainFrame*)AfxGetMainWnd();
	CTaiShanDoc* pDoc = pFm->m_pDoc ;


	//获取所有股票代码
	GetTotalStockId();
	//当前股票代码
	m_sCurrSymbol = pDoc->m_sharesSymbol;
	TransCurrStockSymbol();
	//设置前一和后一股票的代码
	GetPrevNextSymbol();
	//初始化文件中的各项
	GetFileItems();
	//创建按钮
	CreateButton();

	return 0;
}

void CF10HeadView::GetFileItems(void)
{
	CString filename,name;

	m_arrItemName.RemoveAll();

	name = m_sCurrSymbol.Mid(1);
	if (m_sCurrSymbol.Left(1) == "1")//上证
	{
		filename.Format("%s%s.txt",g_sF10sh,name);
	}
	else//深证
	{
		filename.Format("%s%s.txt",g_sF10sz,name);
	}
	//打开文件
	CStdioFile file;
	if( file.Open( filename, CFile::modeRead | CFile::typeText ) ) 
	{
		CString strLine = "";
		CString strItemName = "";
		char* pszItemText = NULL;
		CString	strItemText = "";
		//读取其中各项
		while (file.ReadString(strLine))
		{
			int iFirstIndex = strLine.Find(_T("☆"));
			int iLastIndex = strLine.Find(_T("☆"),iFirstIndex + 2);
			if (iFirstIndex == 0 && iLastIndex != -1)
			{
				strItemName = strLine.Mid(iFirstIndex + 2,iLastIndex - 2);
				strItemName.Trim();
				m_arrItemName.Add(strItemName);
			}
		}
		file.Close();
	}

	m_arrItemPos.RemoveAll();

	CFile file1;
	if(file1.Open(filename, CFile::modeRead)) 
	{
		__int64 iFileLen = 0;
		//文件的长度
		iFileLen = file1.GetLength();
		if(iFileLen > 0)
		{
			if(m_pF10Data)
			{
				delete [] m_pF10Data;
				m_pF10Data = NULL;
			}
			m_pF10Data = (char*)malloc(iFileLen);
			if(m_pF10Data)
			{
				memset(m_pF10Data,0,iFileLen);
				iFileLen = file1.Read(m_pF10Data,iFileLen );
			}	
			//对各项的位置进行定位
			char head[3] = "☆";
			char pos[3];
			int iIndex = 0;
			__int64 iStartPos = 0;

			//strcpy(head,"☆");

			for (__int64 i = 0;i < iFileLen; i++)
			{
				pos[0] = *(m_pF10Data + i + 1);
				pos[1] = *(m_pF10Data + i + 2);
				pos[2] = '\0';
				if ((m_pF10Data[i] == '\n') && (strcmp(pos,head) == 0))
				{
					ItemPosition item;
					item.strName = m_arrItemName[iIndex++];
					item.iStartPos = iStartPos;
					item.iLen = i + 1 - iStartPos;
					m_arrItemPos.Add(item);
					iStartPos = i + 1;
				}
			}
			ItemPosition item;
			item.strName = m_arrItemName[iIndex];
			item.iStartPos = iStartPos;
			item.iLen = file1.GetLength() - iStartPos;
			m_arrItemPos.Add(item);
		}
		file1.Close();
	}


	return;

}

void CF10HeadView::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CFormView::OnShowWindow(bShow, nStatus);

	if (bShow == SW_HIDE)
	{
		return;
	}
	//移动按钮
	MoveButton();
	//设置前一和后一按钮的标题
	SetPrevNextButtonText();
	////显示第一项的内容
	//m_sCurrItemName = m_arrItemName[0];
	//ShowFileText(m_sCurrItemName);
}

void CF10HeadView::CreateButton(void)
{
	//生成各项的按钮，并与ID相映射
	for (int i = 1;i < 30;i++)
	{
		m_myButton[i].Create(NULL,WS_CHILD|BS_RADIOBUTTON|BS_PUSHLIKE ,CRect(0,0,20,20),this,IDC_RADIO1 + i);
		m_myButton[i].SetButtonStyle(9);
	}

}

void CF10HeadView::MoveButton(void)
{
	int iCount = m_arrItemName.GetCount();
	if(iCount == 0)
	{
		CButton *pBtn = (CButton *)GetDlgItem(IDC_RADIO1);
		pBtn->ShowWindow(SW_HIDE);
		pBtn = (CButton *)GetDlgItem(IDC_ITEMGROUP);
		pBtn->ShowWindow(SW_HIDE);

		return;
	}
	//移动按钮到正确的位置
	CButton *pBtn = (CButton *)GetDlgItem(IDC_RADIO1);
	pBtn->SetWindowText(m_arrItemName[0]);
	pBtn->ShowWindow(SW_SHOW);

	DWORD dwStyle = pBtn->GetButtonStyle();
	CRect rcBtn,rcBtn1,rcSrc;
	pBtn->GetWindowRect(rcBtn);
	ScreenToClient(rcBtn);
	rcSrc = rcBtn;

	CFont *pFont;
	pFont = pBtn->GetFont();

	rcBtn1 = rcSrc;
	for (int i = 2;i < iCount;i += 2)
	{
		rcBtn1.left = rcBtn.left + rcBtn.Width() + 4;
		rcBtn1.right = rcBtn1.left + rcBtn.Width();

		m_myButton[i].SetButtonStyle(dwStyle);
		m_myButton[i].SetFont(pFont);
		m_myButton[i].MoveWindow(rcBtn1);
		m_myButton[i].SetWindowText(m_arrItemName[i]);
		m_myButton[i].ShowWindow(SW_SHOW);

		rcBtn = rcBtn1;
	}
	rcBtn = rcSrc;
	rcBtn1 = rcSrc;
	for (int i = 1;i < iCount;i += 2)
	{
		rcBtn1.top = rcSrc.top + rcSrc.Height() + 4;
		rcBtn1.bottom = rcBtn1.top + rcSrc.Height();
		if(i != 1)
		{
			rcBtn1.left = rcBtn.left + rcBtn.Width() + 4;
			rcBtn1.right = rcBtn1.left + rcBtn.Width();
		}

		m_myButton[i].SetButtonStyle(dwStyle);
		m_myButton[i].SetFont(pFont);
		m_myButton[i].MoveWindow(rcBtn1);
		m_myButton[i].SetWindowText(m_arrItemName[i]);
		m_myButton[i].ShowWindow(SW_SHOW);

		rcBtn = rcBtn1;
	}
	//移动Group框到正确位置
	pBtn = (CButton *)GetDlgItem(IDC_ITEMGROUP);
	pBtn->GetWindowRect(rcBtn);
	ScreenToClient(rcBtn);
	rcBtn.right = rcSrc.left + (rcSrc.Width() + 4) * int((iCount + 1)/2) + 5;
	pBtn->MoveWindow(rcBtn);

	pBtn->ShowWindow(SW_SHOW);

}
void CF10HeadView::GetPrevNextSymbol(void)
{
	//设置前一和后一股票的代码
	vector<CString>::iterator it;
	it = find(m_vAllStockSymbol.begin(),m_vAllStockSymbol.end(),m_sCurrSymbol.GetBuffer(0));
	//it = find(m_vAllStockSymbol.begin(),m_vAllStockSymbol.end(),atoi(m_sCurrSymbol.GetBuffer(0)));
	m_sCurrSymbol.ReleaseBuffer();

	if(it != m_vAllStockSymbol.end())
	{
		CString sTemp;
		//前一股票代码
		if (it != m_vAllStockSymbol.begin())
		{
			sTemp.Format("%.7d",*(it - 1));
			m_sPrevSymbol = *(it - 1);
		}
		else
		{
			sTemp.Format("%.7d",*(m_vAllStockSymbol.end() - 1));
			m_sPrevSymbol = *(m_vAllStockSymbol.end() - 1);
		}
		//后一股票代码
		if (it + 1 != m_vAllStockSymbol.end())
		{
			sTemp.Format("%.7d",*(it + 1));
			m_sNextSymbol = *(it + 1);
		}
		else
		{
			sTemp.Format("%.7d",*(m_vAllStockSymbol.begin()));
			m_sNextSymbol = *(m_vAllStockSymbol.begin());
		}
	}
}
void CF10HeadView::SetPrevNextButtonText(void)
{
	//设置前一和后一按钮的标题
	CButton *pBtn;

	CReportData *pCdat;
	CString strCurrStockName = "";
	CString strPrevStockName = "";
	CString strNextStockName = "";
	CString strTemp = "";

	strTemp = m_sCurrSymbol.Mid(1);
	strCurrStockName = SymbolToName(strTemp.GetBuffer(0));
	strTemp.ReleaseBuffer();
	strTemp = m_sPrevSymbol.Mid(1);
	strPrevStockName = SymbolToName(strTemp.GetBuffer(0));
	strTemp.ReleaseBuffer();
	strTemp = m_sNextSymbol.Mid(1);
	strNextStockName = SymbolToName(strTemp.GetBuffer(0));
	strTemp.ReleaseBuffer();

	pBtn = (CButton *)GetDlgItem(IDC_STOCK_SYMBOL_NAME);
	strTemp.Format("%s  %s",m_sCurrSymbol.Mid(1),strCurrStockName);
	pBtn->SetWindowText(strTemp);

	pBtn = (CButton *)GetDlgItem(IDC_F10_PREV);
	strTemp.Format("<< %s",strPrevStockName);
	pBtn->SetWindowText(strTemp);

	pBtn = (CButton *)GetDlgItem(IDC_F10_NEXT);
	strTemp.Format("%s >>",strNextStockName);
	pBtn->SetWindowText(strTemp);
}

//
//This function get stock name according to the given stock code
//
//Input parameter:
//        pSymbol : the pointer of the stock code
//Return parameter:
//        return a pointer of the stock name
//
char* CF10HeadView::SymbolToName(char *pSymbol)
{
	CTaiShanDoc* pDoc = CMainFrame::m_pDoc;

	int nStockLength = pDoc->m_keynumberdata.GetSize();
	int i;
	for(i = 0;i < nStockLength;i++)
	{
		char *pSearchKey = NULL;
		pSearchKey = pDoc->m_keynumberdata.GetAt(i)->key;
		if(strcmp(pSymbol,pSearchKey) == 0)
			break;
	}
	if(i < nStockLength)
		return pDoc->m_keynumberdata.GetAt(i)->data;
	else
		return NULL;
}
void CF10HeadView::OnBnClickedF10Prev()
{
	m_vAllF10FileName.empty();
	m_arrItemName.RemoveAll();

	for (int i = 1;i < 30;i++)
	{
		m_myButton[i].ShowWindow(SW_HIDE);
	}

	m_sCurrSymbol = m_sPrevSymbol;
	//设置前一和后一股票的代码
	GetPrevNextSymbol();
	//初始化文件中的各项
	GetFileItems();
	//创建按钮
	MoveButton();
	//设置前一和后一按钮的标题
	SetPrevNextButtonText();
	//if (m_arrItemName.GetCount())
	{
		//显示当前项的内容
		ShowFileText(m_sCurrItemName);
	}
}

void CF10HeadView::OnBnClickedF10Next()
{
	m_vAllF10FileName.empty();
	m_arrItemName.RemoveAll();

	for (int i = 1;i < 30;i++)
	{
		m_myButton[i].ShowWindow(SW_HIDE);
	}

	m_sCurrSymbol = m_sNextSymbol;
	//设置前一和后一股票的代码
	GetPrevNextSymbol();
	//初始化文件中的各项
	GetFileItems();
	//创建按钮
	MoveButton();
	//设置前一和后一按钮的标题
	SetPrevNextButtonText();
	//if (m_arrItemName.GetCount())
	//{
		//显示当前项的内容
		ShowFileText(m_sCurrItemName);
		//ShowFileText(m_sCurrItemName == ""?m_arrItemName[0]:m_sCurrItemName);
	//}
}

void CF10HeadView::ReleaseSources(void)
{
	m_vAllF10FileName.empty();
	for (int i = 1;i < 30;i++)
	{
		m_myButton[i].DestroyWindow();
	}
	m_arrItemName.RemoveAll();

	if(m_pF10Data)
	{
		delete [] m_pF10Data;
		m_pF10Data = NULL;
	}

}

void CF10HeadView::GetTotalStockId(void)
{
	CTaiShanDoc* pDoc = CMainFrame::m_pDoc;

	int i,j;
	for(i = 0;i < BLOCKINDEX;i++)//暂时只考虑前23类股票，还有要添加的股票待定
	{
		int temp = pDoc->m_sharesInformation.GetStockTypeCount(i);
		for(j = 0;j < temp;j++)
		{
			CReportData *Cdat;
			CString StockId ; 
			pDoc->m_sharesInformation.GetStockItem(i,j,Cdat);
			if(Cdat == NULL)
				continue;
			StockId =Cdat->id;
			//if (StockId == "11")
			//{
			//	int io = 0;
			//}
			if(i ==	SHZS || i == SHAG || i == SHBG || i == SHZQ || i ==	SHJJ || i ==	SHZZ ||
				i == SHHG || i == SHETF || i == SHKF || i == SHQZ || i == SHOT)
				StockId = "1" + StockId;
			else if (i ==	SZZS || i == SZAG || i == SZBG || i == SZZQ || i ==	SZJJ || i == SZZZ || 
				i == SZHG || i == SZETF || i == SZKF || i == SZQZ || i == SZSB || i == SZOT || i == SZZXQY || i == SZCYB)
				StockId = "2" + StockId;

			m_vAllStockSymbol.push_back(StockId);
		}
	}
}
void CF10HeadView::TransCurrStockSymbol(void)
{	
	vector<CString>::iterator it;
	CString sTemp = "1" + m_sCurrSymbol;

	it = find(m_vAllStockSymbol.begin(),m_vAllStockSymbol.end(),sTemp.GetBuffer(0));
	sTemp.ReleaseBuffer();

	if(it != m_vAllStockSymbol.end())
		m_sCurrSymbol = "1" + m_sCurrSymbol;
	else
		m_sCurrSymbol = "2" + m_sCurrSymbol;

}
void CF10HeadView::ShowFileText(CString strItemName /* = "" */)
{
	ItemPosition item;
	char* broadcastmessage = NULL;
	BOOL bFind = FALSE;
	int	iWhich = 0;

	if (m_pTaiTestRichView == NULL)// || strItemName == "")
	{
		return;
	}
	for (int i = 0;i < m_arrItemName.GetCount();i++)
	{
		if (strItemName == m_arrItemName[i])
		{
			bFind = TRUE;
			iWhich = i;
			break;
		}
	}
	if(!bFind && m_arrItemName.GetCount())
	{
		strItemName = m_arrItemName[0];
		m_sCurrItemName = m_arrItemName[0];
	}
	for (int i = 0;i < 30;i++)
	{
		CButton *pBtn = (CButton *)GetDlgItem(IDC_RADIO1 + i);
		pBtn->SetCheck(0);
	}
	CButton *pBtn = (CButton *)GetDlgItem(IDC_RADIO1 + iWhich);
	pBtn->SetCheck(1);

	item.strName = strItemName;

	if (GetItemPos(item) && m_pF10Data)
	{
		broadcastmessage = new char[item.iLen];
		memcpy(broadcastmessage,m_pF10Data + item.iStartPos,item.iLen);
	}
	else
	{
		broadcastmessage = new char[12];
		memset(broadcastmessage,0,12);
		strcpy(broadcastmessage,"无F10资料!");
	}
	

	m_pTaiTestRichView->GetRichEditCtrl().ShowWindow(SW_HIDE);
	m_pTaiTestRichView->GetRichEditCtrl().SetReadOnly(TRUE);

	m_pTaiTestRichView->GetRichEditCtrl().SetSel(0,-1);
	m_pTaiTestRichView->GetRichEditCtrl().ReplaceSel(broadcastmessage);
	m_pTaiTestRichView->GetRichEditCtrl().SetSel(0,0);
	m_pTaiTestRichView->GetRichEditCtrl().ShowWindow(SW_SHOW);
	m_pTaiTestRichView->GetRichEditCtrl().GetLimitText();

	CMainFrame* pMainWnd = (CMainFrame*)AfxGetApp()->m_pMainWnd;
	CFrameWnd * pActiveWnd = ((CFrameWnd*)pMainWnd)->GetActiveFrame();
	CTaiShanDoc* pDoc = pMainWnd->m_pDoc;
	pDoc->SetModifiedFlag(FALSE);
	((CWnd*)m_pTaiTestRichView)->UpdateData(FALSE);
	//((CChildFrame *)pActiveWnd)->SetActiveView(m_pTaiTestRichView,TRUE);
	CChildFrame* pFrame = (CChildFrame*)m_pTaiTestRichView->GetParent()->GetParentFrame();
	pFrame->ShowWindow(SW_SHOWMAXIMIZED);
	//pMainWnd->MDIActivate(pFrame);
	pFrame->SetActiveView(m_pTaiTestRichView,TRUE);
	m_pTaiTestRichView->SetFocus();
	
	delete [] broadcastmessage;
	broadcastmessage = NULL;

}
BOOL CF10HeadView::GetItemPos(ItemPosition& item)
{
	BOOL bFind = FALSE;
	for(int i = 0;i < m_arrItemPos.GetCount();i++)
	{
		if (item.strName == m_arrItemPos[i].strName)
		{
			item = m_arrItemPos[i];
			bFind = TRUE;
			break;
		}
	}
	return bFind;
}


void CF10HeadView::ShowF10()
{
	CMainFrame* pFm = (CMainFrame*)AfxGetMainWnd();
	CTaiShanDoc* pDoc = pFm->m_pDoc ;

	m_vAllF10FileName.empty();
	m_arrItemName.RemoveAll();

	for (int i = 1;i < 30;i++)
	{
		m_myButton[i].ShowWindow(SW_HIDE);
	}
	//当前股票代码
	m_sCurrSymbol = pDoc->m_sharesSymbol;
	TransCurrStockSymbol();
	//设置前一和后一股票的代码
	GetPrevNextSymbol();
	//初始化文件中的各项
	GetFileItems();
	//创建按钮
	MoveButton();
	//设置前一和后一按钮的标题
	SetPrevNextButtonText();
	//显示当前项的内容
	//ShowFileText(m_sCurrItemName);
	OnBnClickedRadioItem(IDC_RADIO1);

}