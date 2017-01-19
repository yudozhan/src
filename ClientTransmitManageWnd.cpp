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
//����˵����ע��һ��socket��ID���Ժ��ͨѶ�������ڴ�ID��
//����˵����HWND hWndΪע���ID�Ĵ��ھ��
//����ֵ���õ�һ��socket��ID
{
	SOCKET_ID sock_id;
	sock_id.hWnd=hWnd;
	sock_id.iFlag=m_iNextID;
	m_bIDStatus[m_iNextID]=TRUE;
	AdjustNextIDNumber();
	return sock_id;
}

void CClientTransmitManageWnd::ReleaseID(SOCKET_ID socket_id)
//����˵����ע�����뵽��socket��ID��
//����˵����SOCKET_ID socket_idΪ���뵽��ID��
//����ֵ���޷���ֵ��
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

//�����ص�����
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
			//����pView��Ӧ��ͨ���ţ��ڴ���Ϊÿ��Viewֻ����һ��������ʷ����ͨ��
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
//����˵����ͨ�����뵽��ID�������ݵ�����
//����˵����SOCKET_ID sock_idΪ���뵽��ID��
//			TRANS_PACKAGE_STRUCTEx* pTransPackageΪԤ�õ����ݵ�����˵����
//			��action ˵�����£�	0	��ʾֻ����һ������
//								1	��ʾע�ᴫ������
//								-1	��ʾע�ᴫ�ͽ���
//����ֵ���ڷ���˵����ݴ��͵������һ����Ϣ���������ݵĴ��ڡ�
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
//��ڲ�����pTransPackage          �ͻ��˴��Ͱ�����
//		    action                 ���ͷ�ʽ˵��
//			dwTransEnvironment    �ͻ��˴��ͻ���˵������
//���ز�����1                 ��ʾ���óɹ�
//			-1                ��ʾ����ʧ��
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

//���ڿ����,�յ��Ļ������ϸ�ʽ��ԭ���Ĵ����ʽ��һ��,����������ת��
void CClientTransmitManageWnd::ChangeFormat(RCV_BASEINFO_STRUCTEx* pBaseInfo, BASEINFO* baseinfo)
{
	strcpy(baseinfo->Symbol,pBaseInfo->m_szLabel);  //��Ʊ����
//	int  NumSplit;   //�Ѿ���Ȩ����
	baseinfo->zgb=pBaseInfo->m_fZgb;     //�ܹɱ�(���)
	baseinfo->gjg=pBaseInfo->m_fGjg;     //���ҹ�(���)
	baseinfo->fqrfrg=pBaseInfo->m_fFqrfrg;  //�����˷��˹�(���)
	baseinfo->frg=pBaseInfo->m_fFrg;     //���˹�(���)
	baseinfo->zgg=pBaseInfo->m_fZgg;     //ְ����(���)
	baseinfo->gzAg=pBaseInfo->m_fGzAg;    //����A��(���)
	baseinfo->ltAg=pBaseInfo->m_fLtAg;    //��ͨA��(���)
	baseinfo->Hg=pBaseInfo->m_fHg;      //�ȹ�(���)
	baseinfo->Bg=pBaseInfo->m_fBg;      //B��(���)
	baseinfo->zpg=pBaseInfo->m_fZpg;     //ת���(���)

	baseinfo->zzc=pBaseInfo->m_fZzc;     //���ʲ�(��Ԫ)
	baseinfo->ldzc=pBaseInfo->m_fLdzc;    //�����ʲ�(��Ԫ)
	baseinfo->cqtz=pBaseInfo->m_fCqtz;    //����Ͷ��(��Ԫ)
	baseinfo->gdzc=pBaseInfo->m_fGdzc;    //�̶��ʲ�(��Ԫ)
	baseinfo->wxzc=pBaseInfo->m_fWxzc;    //�����ʲ�(��Ԫ)
	baseinfo->ldfz=pBaseInfo->m_fLdfz;    //������ծ(��Ԫ)
	baseinfo->cqfz=pBaseInfo->m_fCqfz;    //���ڸ�ծ(��Ԫ)
	baseinfo->ggqy=pBaseInfo->m_fGgqy;    //�ɶ�Ȩ��(��Ԫ)
	baseinfo->zbgjj=pBaseInfo->m_fZbgjj;   //�ʱ�������(��Ԫ)
	baseinfo->yygjj=pBaseInfo->m_fYygjj;   //ӯ�๫����(��Ԫ)
	baseinfo->mgjz=pBaseInfo->m_fMgjz;    //ÿ�ɾ�ֵ(Ԫ)
	baseinfo->gdqybl=pBaseInfo->m_fGdqybl;  //�ɶ�Ȩ�����(%)
	baseinfo->mggjj=pBaseInfo->m_fMggjj;   //ÿ�ɹ�����(Ԫ)

	baseinfo->zyywsr=pBaseInfo->m_fZyywsr;  //��Ӫҵ������(��Ԫ)
	baseinfo->zyywlr=pBaseInfo->m_fZyywlr;  //��Ӫҵ������(��Ԫ)
	baseinfo->qtywlr=pBaseInfo->m_fQtywlr;  //����ҵ������(��Ԫ)
	baseinfo->lrze=pBaseInfo->m_fLrze;    //�����ܶ�(��Ԫ)
	baseinfo->jlr=pBaseInfo->m_fJlr;     //������(��Ԫ)
	baseinfo->wfplr=pBaseInfo->m_fWfplr;   //δ��������(��Ԫ)
	baseinfo->mgsy=pBaseInfo->m_fMgsy;    //ÿ������(Ԫ)
	baseinfo->jzcsyl=pBaseInfo->m_fJzcsyl;  //���ʲ�������(%)
	baseinfo->mgwfplr=pBaseInfo->m_fMgwfplr; //ÿ��δ��������(Ԫ)

	baseinfo->mgjzc=pBaseInfo->m_fMgjzc;   //ÿ�ɾ��ʲ�(Ԫ)

//	int m_iPos;
//	BYTE  free[8];
//	Split m_Split[80];
}
