#if !defined(AFX_CLIENTSOCKETMANAGEWND_H__F3F0A262_E27F_406C_9B33_1D6E8950628E__INCLUDED_)
#define AFX_CLIENTSOCKETMANAGEWND_H__F3F0A262_E27F_406C_9B33_1D6E8950628E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ClientSocketManageWnd.h : header file
//

#define	WM_SOCKET		WM_USER+10	//�յ����������ص���Ϣ
#define MSG_LOGON		WM_USER+20	//���͵���¼������Ϣ
#define MSG_MYTREEVIEW		WM_USER+30	//���͵�F10��TREEVIEW����
#define	Gp_Msg_StkData	WM_APP + 1
/////////////////////////////////////////////////////////////////////////////
// CClientTransmitManageWnd window

typedef struct _SOCKET_ID
{
	HWND	hWnd;		//ע���ID�Ĵ��ھ���������Ժ���˴��ڷ���I/O�����Ϣ
	int		iFlag;		//��ID�ı�־���������ֲ�ͬ��ID����Χ��0��49����50��;���Ի�ʱΪ-1
}SOCKET_ID;

typedef struct _TRANSENVIRONMENT
{
		TRANS_PACKAGE_STRUCTEx transPackage;//�������ݰ�
		int action;							//�Ƿ�ͣ����������
		SOCKET_ID sock_id;					//�������ݵ�ͨ��ID
		BOOL bIsNull;						//���ṹ������Ϣ�Ƿ���Ч��ΪTRUEʱ��������Ϣ��Ч
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
	
	BOOL m_bIDStatus[50];	//	m_bIDStatus[n]ΪTRUEʱ����n��ͨ���ѱ�ע�᣻
							//	ΪFALSEʱ����n��ͨ��û�б�ע����ѱ�ע����
	int m_iNextID;			//	��ʼ��ʱm_iNextIDΪ0���Ժ�ÿһ��ע��ͨ��ID�ͼ�1�������ͨ��ΪTRUE��������һ����ֱ���ҵ�һ��δע���ͨ��IDΪֹ��
							//	m_iNextID��ȡֵ��Χ��0��49ѭ����
	TRANSENVIRONMENT m_transArray[50][2];
							//	m_transArray[n]��n��Ӧͨ��ID�ţ�����������ֵ���ֱ���0Ԫ�غ�1Ԫ�ر�ʾ��0Ԫ�ش����Ѿ����������ݻ�û���ص����룻1Ԫ�ش��滹û���������롣
	TRANSENVIRONMENT m_transUnending;
							//	����ʵʱ��������

};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CLIENTSOCKETMANAGEWND_H__F3F0A262_E27F_406C_9B33_1D6E8950628E__INCLUDED_)
