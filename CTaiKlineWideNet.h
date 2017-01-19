// CTaiKlineWideNet.h: interface for the CTaiKlineWideNet class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TECHWIDENET_H__5FFBB872_DAB3_495F_82B2_93EDA6179B84__INCLUDED_)
#define AFX_TECHWIDENET_H__5FFBB872_DAB3_495F_82B2_93EDA6179B84__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
class CTaiShanKlineShowView;
/*#ifndef _SOCKET_ID
typedef struct _SOCKET_ID
{
	HWND	hWnd;	//ע���ID�Ĵ��ھ���������Ժ���˴��ڷ���I/O�����Ϣ
	int iFlag ;	//��ID�ı�־���������ֲ�ͬ��ID����Χ��0��49����50��
}SOCKET_ID;
#endif*/
class CTaiKlineWideNet  
{
public:
	void AddStockKlineDataAll(CString symbol,int stkKind, int nKlineType,bool bAll = false);
	void AddStockHsFileDataAll(CString symbol,int stkKind,bool bAll = false);
	void AddStockHsHistoryFileData(CString symbol,int stkKind,CString sDate);
	void SetHsHistoryRequestFlag(bool bRequested);
	void Request();
	void ReleaseID();
	//ע��ͨ��
	//������symbol��Ʊ����
	void RegisterID();
	//�״μ���ĳ��Ʊ����Ҫ��k�����ݵĸ���
	//������symbol��Ʊ����
	int GetRequestKlineCount(CString symbol,int stkKind,bool bDay = true);
	//�״ν���ĳ��Ʊ������ĳ��Ʊ�����ݵĴ���
	//������symbol��Ʊ����
	void AddStockFirst(CString symbol,int stkKind,bool m_bAutoDeletePre = true);
	//����ĳ��Ʊ��������ϸ����
	//������symbol��Ʊ����
	void AddStockHsFileData(CString symbol,int stkKind);
	//����ĳ��Ʊ��k������
	//������symbol��Ʊ����
	void AddStockKlineData(CString symbol,int stkKind,int nKlineType);
	//����ĳ��Ʊ���������ݵĴ���
	//������symbol��Ʊ����
	void AddStockHs(CString symbol,int stkKind,bool m_bAutoDeletePre = true);
	CTaiKlineWideNet();
	CTaiKlineWideNet(CTaiShanKlineShowView * pView);
	virtual ~CTaiKlineWideNet();

private:
	int GetRequestHsCount(CString symbol,int stkKind);
	int GetRequestMin1Count(CString symbol,int stkKind);
	int GetDaysToToday(Kline* pKline);
	CTaiShanKlineShowView* m_pView;

	CString				m_symbolPre;//ǰһ����Ʊ
	int					m_stkKindPre;

	bool				m_bDayRequested;//����
	bool				m_bMin5Requested;//5����k��
	bool				m_bMin1Requested;//1����
	bool				m_bHsRequested;//�ֱ���ϸ
	bool				m_bHsHistoryRequested;//��ʷ�ֱ���ϸ
	CString				m_sDateHistory;//��ʷ����ʱ��

	static Kline*		m_pKline1A0001;
	static int			m_nKline1A0001;
	static int			m_nKlineToToday;

	//0Ϊʵʱ���飬1Ϊ��ʷk��
	SOCKET_ID	g_socketID[3];
};

#endif // !defined(AFX_TECHWIDENET_H__5FFBB872_DAB3_495F_82B2_93EDA6179B84__INCLUDED_)
