// ClientSocketManageWnd.cpp : implementation file
//

#include "stdafx.h"
#include "ClientTransmitManageWnd.h"
#include "WH.h"
#include "mainfrm.h"
#include "share.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CClientTransmitManageWnd

CClientTransmitManageWnd::CClientTransmitManageWnd()
{
	LPCTSTR lpszClassName = AfxRegisterWndClass(CS_HREDRAW|CS_VREDRAW);
	CRect rect(0,0,0,0);
	CreateEx(0, lpszClassName, "MarketDataMainWindow", WS_OVERLAPPEDWINDOW, rect, NULL, 0);
	ShowWindow(SW_HIDE);
//	m_hostName="qiuyp";
	m_iNextID=0;
	for(int i=0;i<50;i++)
	{
		m_bIDStatus[i]=FALSE;
		m_transArray[i][0].bIsNull=TRUE;
		m_transArray[i][1].bIsNull=TRUE;
	}
	m_pMainFrame=NULL;
}

CClientTransmitManageWnd::CClientTransmitManageWnd(CMDIFrameWnd* pMainFrame)
{
	LPCTSTR lpszClassName = AfxRegisterWndClass(CS_HREDRAW|CS_VREDRAW);
	CRect rect(0,0,0,0);
	CreateEx(0, lpszClassName, "MarketDataMainWindow", WS_OVERLAPPEDWINDOW, rect, NULL, 0);
	ShowWindow(SW_HIDE);
//	m_hostName="qiuyp";
	m_iNextID=0;
	for(int i=0;i<50;i++)
	{
		m_bIDStatus[i]=FALSE;
		m_transArray[i][0].bIsNull=TRUE;
		m_transArray[i][1].bIsNull=TRUE;
	}
	m_pMainFrame=pMainFrame;
}

CClientTransmitManageWnd::~CClientTransmitManageWnd()
{
}


BEGIN_MESSAGE_MAP(CClientTransmitManageWnd, CWnd)
	//{{AFX_MSG_MAP(CClientTransmitManageWnd)
	ON_WM_CREATE()
	ON_MESSAGE(WM_SOCKET, OnStkDataOK)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CClientTransmitManageWnd message handlers

int CClientTransmitManageWnd::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	Stock_Init(this->GetSafeHwnd(),WM_SOCKET,0);
	return 0;
}

void CClientTransmitManageWnd::PostNcDestroy() 
{
	// TODO: Add your specialized code here and/or call the base class
	Stock_Quit(this->GetSafeHwnd());
	CWnd::PostNcDestroy();
}

SOCKET_ID CClientTransmitManageWnd::RegisterID(HWND hWnd)
//功能说明：注册一个socket的ID，以后的通讯将建立在此ID上
//参数说明：HWND hWnd为注册此ID的窗口句柄
//返回值：得到一个socket的ID
{
	SOCKET_ID sock_id;
	sock_id.hWnd=hWnd;
	sock_id.iFlag=m_iNextID;
	m_bIDStatus[m_iNextID]=TRUE;
	AdjustNextIDNumber();
	return sock_id;
}

void CClientTransmitManageWnd::ReleaseID(SOCKET_ID socket_id)
//功能说明：注销申请到的socket的ID。
//参数说明：SOCKET_ID socket_id为申请到的ID。
//返回值：无返回值。
{
	m_bIDStatus[socket_id.iFlag]=FALSE;
}

LONG CClientTransmitManageWnd::OnStkDataOK(UINT wParam,LONG lParam)
{
	RCV_COMMINTERFACE_STRUCTEx  *pCommInterface;  
	SOCKET_ID sock_id;
	DWORD dwTransEnvironment;
	pCommInterface=(RCV_COMMINTERFACE_STRUCTEx*)lParam;
	dwTransEnvironment= pCommInterface->m_dwTransEnvironment;
	sock_id=*(SOCKET_ID*)dwTransEnvironment;

//处理返回的数据
	StkDataManage(wParam,lParam);

	if(!m_bIDStatus[sock_id.iFlag])
	{
		m_transArray[sock_id.iFlag][0].bIsNull=TRUE;
		m_transArray[sock_id.iFlag][1].bIsNull=TRUE;
	}
	else
	{
		m_transArray[sock_id.iFlag][0].bIsNull=TRUE;
		CView* pView;
		pView=GetCurrentActiveView();
		if(pView->GetSafeHwnd()==sock_id.hWnd)
		{
			if(!m_transArray[sock_id.iFlag][1].bIsNull)
			{
				m_transArray[sock_id.iFlag][0].action=m_transArray[sock_id.iFlag][1].action;
				m_transArray[sock_id.iFlag][0].sock_id=m_transArray[sock_id.iFlag][1].sock_id;
				m_transArray[sock_id.iFlag][0].transPackage=m_transArray[sock_id.iFlag][1].transPackage;
				m_transArray[sock_id.iFlag][0].bIsNull=FALSE;
				m_transArray[sock_id.iFlag][1].bIsNull=TRUE;

				GetMarketData(m_transArray[sock_id.iFlag][0]);
			}
		}
		else
		{
			//查找pView对应的通道号；在此认为每个View只申请一个传送历史行情通道
			for(int i=0;i<50;i++)
			{
				if(m_bIDStatus[i] && m_transArray[i][1].bIsNull==FALSE && m_transArray[i][1].sock_id.hWnd==pView->GetSafeHwnd())
				{					
					m_transArray[i][0].action=m_transArray[i][1].action;
					m_transArray[i][0].sock_id=m_transArray[i][1].sock_id;
					m_transArray[i][0].transPackage=m_transArray[i][1].transPackage;
					m_transArray[i][0].bIsNull=FALSE;
					m_transArray[i][1].bIsNull=TRUE;
					
					GetMarketData(m_transArray[i][0]);
					break;					
				}
			}
		}
	}

	return 0L;
}

CView* CClientTransmitManageWnd::GetCurrentActiveView()
{
	ASSERT(m_pMainFrame);
	CMDIChildWnd *pChild = 
		(CMDIChildWnd *)m_pMainFrame->GetActiveFrame();
	
	CView *pView = (CView *) pChild->GetActiveView();	

	return pView;
}

void CClientTransmitManageWnd::AdjustNextIDNumber()
{
	BOOL bOK=FALSE;
	for(int i=0;i<50;i++)
	{
		if(m_iNextID++>=50)
			m_iNextID=0;
		if(m_bIDStatus[m_iNextID]==FALSE)
		{
			bOK=TRUE;
			break;
		}
	}
	if(!bOK)
		TRACE("Error:The ID is full!");
}

void CClientTransmitManageWnd::GetMarketData(TRANS_PACKAGE_STRUCTEx* pTransPackage,int action,SOCKET_ID sock_id,BOOL AutoUnregister)
//功能说明：通过申请到的ID发送数据的请求。
//参数说明：SOCKET_ID sock_id为申请到的ID；
//			TRANS_PACKAGE_STRUCTEx* pTransPackage为预得到数据的类型说明；
//			对action 说明如下：	0	表示只传送一次数据
//								1	表示注册传送数据
//								-1	表示注册传送结束
//返回值：在服务端的数据传送到达后发送一个消息给申请数据的窗口。
{
	BOOL rsp;
	if(action==1 || action==-1)
	{
		m_transUnending.transPackage=*pTransPackage;
		m_transUnending.action=action;
		m_transUnending.sock_id=sock_id;
		m_transUnending.bIsNull=FALSE;
		rsp=GetMarketData(m_transUnending);
		if(!rsp)
			TRACE("Error:GetMarketData() failed!");
	}
	else
	{
		if(!m_transArray[sock_id.iFlag][0].bIsNull)
		{
			m_transArray[sock_id.iFlag][1].transPackage=*pTransPackage;
			m_transArray[sock_id.iFlag][1].action=action;
			m_transArray[sock_id.iFlag][1].sock_id=sock_id;
			m_transArray[sock_id.iFlag][1].bIsNull=FALSE;
		}
		else
		{
			m_transArray[sock_id.iFlag][0].transPackage=*pTransPackage;
			m_transArray[sock_id.iFlag][0].action=action;
			m_transArray[sock_id.iFlag][0].sock_id=sock_id;
			m_transArray[sock_id.iFlag][0].bIsNull=FALSE;

			rsp=GetMarketData(m_transArray[sock_id.iFlag][0]);
			if(!rsp)
				TRACE("Error:GetMarketData() failed!");
		}
	}	
	if(AutoUnregister)
		ReleaseID(sock_id);
}


BOOL CClientTransmitManageWnd::GetMarketData(TRANSENVIRONMENT transEnvironment)
{
	int rsp=GetMarketData(&(transEnvironment.transPackage),transEnvironment.action,DWORD(&(transEnvironment.sock_id)));
	if(rsp==-1)
		return FALSE;
	return TRUE;
}

int WINAPI CClientTransmitManageWnd::GetMarketData(TRANS_PACKAGE_STRUCTEx *pTransPackage, int action, DWORD dwTransEnvironment)
//入口参数：pTransPackage          客户端传送包类型
//		    action                 传送方式说明
//			dwTransEnvironment    客户端传送环境说明参数
//返回参数：1                 表示调用成功
//			-1                表示调用失败
{
	return 0;
}

int WINAPI CClientTransmitManageWnd::Stock_Init(HWND hWnd, UINT Msg, int nWorkMode)
{
	return 0;
}

int WINAPI CClientTransmitManageWnd::Stock_Quit(HWND hWnd)
{
	return 0;
}

int WINAPI CClientTransmitManageWnd::InitNetworkAndClientLogin(char *szServerIp, char *ServerPort, char *useName, char *usePass)
{
	return 0;
}

void CClientTransmitManageWnd::StkDataManage(UINT wParam, LONG lParam)
{
}

//对于宽带版,收到的基本资料格式与原来的处理格式不一致,在下面做个转换
void CClientTransmitManageWnd::ChangeFormat(RCV_BASEINFO_STRUCTEx* pBaseInfo, BASEINFO* baseinfo)
{
	strcpy(baseinfo->Symbol,pBaseInfo->m_szLabel);  //股票代码
//	int  NumSplit;   //已经除权次数
	baseinfo->zgb=pBaseInfo->m_fZgb;     //总股本(万股)
	baseinfo->gjg=pBaseInfo->m_fGjg;     //国家股(万股)
	baseinfo->fqrfrg=pBaseInfo->m_fFqrfrg;  //发起人法人股(万股)
	baseinfo->frg=pBaseInfo->m_fFrg;     //法人股(万股)
	baseinfo->zgg=pBaseInfo->m_fZgg;     //职工股(万股)
	baseinfo->gzAg=pBaseInfo->m_fGzAg;    //公众A股(万股)
	baseinfo->ltAg=pBaseInfo->m_fLtAg;    //流通A股(万股)
	baseinfo->Hg=pBaseInfo->m_fHg;      //Ｈ股(万股)
	baseinfo->Bg=pBaseInfo->m_fBg;      //B股(万股)
	baseinfo->zpg=pBaseInfo->m_fZpg;     //转配股(万股)

	baseinfo->zzc=pBaseInfo->m_fZzc;     //总资产(万元)
	baseinfo->ldzc=pBaseInfo->m_fLdzc;    //流动资产(万元)
	baseinfo->cqtz=pBaseInfo->m_fCqtz;    //长期投资(万元)
	baseinfo->gdzc=pBaseInfo->m_fGdzc;    //固定资产(万元)
	baseinfo->wxzc=pBaseInfo->m_fWxzc;    //无形资产(万元)
	baseinfo->ldfz=pBaseInfo->m_fLdfz;    //流动负债(万元)
	baseinfo->cqfz=pBaseInfo->m_fCqfz;    //长期负债(万元)
	baseinfo->ggqy=pBaseInfo->m_fGgqy;    //股东权益(万元)
	baseinfo->zbgjj=pBaseInfo->m_fZbgjj;   //资本公积金(万元)
	baseinfo->yygjj=pBaseInfo->m_fYygjj;   //盈余公积金(万元)
	baseinfo->mgjz=pBaseInfo->m_fMgjz;    //每股净值(元)
	baseinfo->gdqybl=pBaseInfo->m_fGdqybl;  //股东权益比率(%)
	baseinfo->mggjj=pBaseInfo->m_fMggjj;   //每股公积金(元)

	baseinfo->zyywsr=pBaseInfo->m_fZyywsr;  //主营业务收入(万元)
	baseinfo->zyywlr=pBaseInfo->m_fZyywlr;  //主营业务利润(万元)
	baseinfo->qtywlr=pBaseInfo->m_fQtywlr;  //其它业务利润(万元)
	baseinfo->lrze=pBaseInfo->m_fLrze;    //利润总额(万元)
	baseinfo->jlr=pBaseInfo->m_fJlr;     //净利润(万元)
	baseinfo->wfplr=pBaseInfo->m_fWfplr;   //未分配利润(万元)
	baseinfo->mgsy=pBaseInfo->m_fMgsy;    //每股收益(元)
	baseinfo->jzcsyl=pBaseInfo->m_fJzcsyl;  //净资产收益率(%)
	baseinfo->mgwfplr=pBaseInfo->m_fMgwfplr; //每股未分配利润(元)

	baseinfo->mgjzc=pBaseInfo->m_fMgjzc;   //每股净资产(元)

//	int m_iPos;
//	BYTE  free[8];
//	Split m_Split[80];
}
