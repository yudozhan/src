#if !defined(AFX_CLIENTSOCKETMANAGEWND_H__F3F0A262_E27F_406C_9B33_1D6E8950628E__INCLUDED_)
#define AFX_CLIENTSOCKETMANAGEWND_H__F3F0A262_E27F_406C_9B33_1D6E8950628E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ClientSocketManageWnd.h : header file
//

#define	WM_SOCKET		WM_USER+10	//收到服务器返回的信息
#define MSG_LOGON		WM_USER+20	//发送到登录窗口消息
#define MSG_MYTREEVIEW		WM_USER+30	//发送到F10的TREEVIEW窗口
#define	Gp_Msg_StkData	WM_APP + 1
/////////////////////////////////////////////////////////////////////////////
// CClientTransmitManageWnd window

typedef struct _SOCKET_ID
{
	HWND	hWnd;		//注册此ID的窗口句柄，用来以后给此窗口发送I/O完成消息
	int		iFlag;		//此ID的标志，用以区分不同的ID；范围从0到49，共50个;初试化时为-1
}SOCKET_ID;

typedef struct _TRANSENVIRONMENT
{
		TRANS_PACKAGE_STRUCTEx transPackage;//申请数据包
		int action;							//是否不停的申请数据
		SOCKET_ID sock_id;					//申请数据的通道ID
		BOOL bIsNull;						//本结构上述信息是否有效，为TRUE时，上述信息无效
}TRANSENVIRONMENT;

//#include "MainFrm.h"

class CClientTransmitManageWnd : public CWnd 
{
// Construction
public:
//	CString m_hostName;
	CClientTransmitManageWnd();
	CClientTransmitManageWnd(CMDIFrameWnd* pMainFrame);
	virtual ~CClientTransmitManageWnd();

	void GetMarketData(TRANS_PACKAGE_STRUCTEx* pTransPackage,int action,SOCKET_ID sock_id,BOOL AutoUnregister=FALSE);
	void ReleaseID(SOCKET_ID socket_id);
	SOCKET_ID RegisterID(HWND hWnd);

	CMDIFrameWnd* m_pMainFrame;

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CClientTransmitManageWnd)
	protected:
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
public:
	int WINAPI Stock_Quit(HWND hWnd);
	int WINAPI Stock_Init(HWND hWnd,UINT Msg,int nWorkMode);

	int WINAPI InitNetworkAndClientLogin(char* szServerIp,char* ServerPort,char* useName,char* usePass);

	// Generated message map functions
protected:
	//{{AFX_MSG(CClientTransmitManageWnd)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg LONG OnStkDataOK(UINT wParam,LONG lParam);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	void ChangeFormat(RCV_BASEINFO_STRUCTEx* pBaseInfo,BASEINFO* baseinfo);
	void StkDataManage(UINT wParam,LONG lParam);
	BOOL GetMarketData(TRANSENVIRONMENT transEnvironment);
	int WINAPI GetMarketData(TRANS_PACKAGE_STRUCTEx *pTransPackage ,int action , DWORD dwTransEnvironment);
	void AdjustNextIDNumber();
	CView* GetCurrentActiveView();
	
	BOOL m_bIDStatus[50];	//	m_bIDStatus[n]为TRUE时：第n个通道已被注册；
							//	为FALSE时：第n个通道没有被注册或已被注销。
	int m_iNextID;			//	初始化时m_iNextID为0，以后每一次注册通道ID就加1，如果此通道为TRUE，则找下一个，直到找到一个未注册的通道ID为止；
							//	m_iNextID的取值范围是0到49循环。
	TRANSENVIRONMENT m_transArray[50][2];
							//	m_transArray[n]（n对应通道ID号）数组有两个值，分别用0元素和1元素表示；0元素储存已经发出但数据还没返回的申请；1元素储存还没发出的申请。
	TRANSENVIRONMENT m_transUnending;
							//	储存实时行情申请

};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CLIENTSOCKETMANAGEWND_H__F3F0A262_E27F_406C_9B33_1D6E8950628E__INCLUDED_)
