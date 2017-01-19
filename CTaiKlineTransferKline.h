#if !defined(AFX_LMBTRANSFERKLINE_H__92CDDF41_D7BE_11D3_96E6_0080C8E1242B__INCLUDED_)
#define AFX_LMBTRANSFERKLINE_H__92CDDF41_D7BE_11D3_96E6_0080C8E1242B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CTaiKlineTransferKline.h : header file
//

#include "StructKlineView.h"


/////////////////////////////////////////////////////////////////////////////
// CTaiKlineTransferKline command target

class CTaiShanDoc;
class CTaiKlineTransferKline : public CCmdTarget
{
	DECLARE_DYNCREATE(CTaiKlineTransferKline)

// Attributes
public:
	bool m_bCalPowerToday;
	CReportData* m_pReportData;	//���ɵ�����ָ��
	CString m_symbolOld;
	int m_stkKindOld;
	bool m_bUseMinute1Data;
	Kline m_klineOld;
	CTaiShanDoc * pDoc;
	CTaiKlineTransferKline();           //  constructor 
	CTaiKlineTransferKline(CBuySellList* pBSList);           //  constructor 

	virtual ~CTaiKlineTransferKline();
// Operations
public:
	static float GetVolHs(TRADE_DETAIL_H_PER* pHs,TRADE_DETAIL_H_PER* pHsPre,int& nFlag);
	static void ComputePowerToday(Kline *pKlineToday,int nKline,powerArr &splitArr);
	static void ComputePower(Kline *pKlineHistory, int nKlineInOut, int nTypeTarget, powerArr& splitArr);
	//����:�����Ȩ
	//�������: pKlineHistory��k������ָ��, nKlineInOut��k�����ݸ���,  nTypeTarget��ʱ����������, splitArr�ǳ�Ȩ��������
	static int ReadPower(CTaiShanDoc * pDoc,CString symbol,int stkKind,powerArr& splitArr);
	//����:�����Ȩ
	//�������: symbol��Ʊ����,
	//�������:splitArr�ǳ�Ȩ��������,�������ݸ���
	int OneTo5(Kline *pKline,int nEach,CString symbol,int stkKind);
	//����:ת����������
	//�������: pKline��k������ָ��, symbol��Ʊ����,nEach=1��ת��Ϊ1*5����k��
	//�������:pKlineҲ�������k������ָ��
	int TransferToday(bool bAnother,CString symbol,int stkKind,Kline* pKline,int nKline);
	//����:���㵱������
    //����˵����symbolΪ��Ʊ���룬pKlineΪK������ָ�룬nKlineΪ�����K�����ݸ�����bAnotherΪ�Ƿ������һ�����ݡ�
    //���˵������k�����ݸ�����
	void InPutValue(Kline* pKline,int i, int& j, bool bFrst,int& nCountDay,int nKindStock = 2,bool isMinute1 = false);
	//����:ת��k������
    //����˵����pKlineΪK������ָ��
	bool TransferKlineToday(CString symbol,int stkKind,Kline*& pKline,int& nKlineInOut,int nTypeTarget);
	bool TransferKlineHistory(CString symbol,int stkKind,Kline*& pKlineHistory,int& nKlineInOut,int nTypeTarget,bool bToday = true,bool bSplit = false);
    //����˵����symbolΪ��Ʊ���룬pKlineHistoryΪ��ʷK������ָ�룬nKlineInOutΪ�����K�����ݸ�����nTypeTargetΪĿ���������ͣ�bTodayΪ�Ƿ�ת���������ݣ�bSplit�Ƿ�����ݽ��г�Ȩ��
    //���˵����nKlineInOutΪ�����K�����ݸ�����pKlineHistoryΪ�����K������ָ�롣

	int CaclKline1Minu(Kline*& pKline,CString name,int stkKind, int nMinu);
	//����:ת��k������
    //����˵����pKlineΪK������ָ��,nameΪ��Ʊ���룬nMinu������(ת����Ŀ������)
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTaiKlineTransferKline)
	//}}AFX_VIRTUAL

// Implementation
protected:
	CBuySellList* pBuySellList; 
	bool m_bRemoveBuySellList;

	int CaclKline1MinuIndex(Kline*& pKline,CString name,int stkKind, int nMinu);

	// Generated message map functions
	//{{AFX_MSG(CTaiKlineTransferKline)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
private:
	static float CalcPowerAfterPer(float fIn, POWER &pwr);
	static float CalcPowerPer(float fIn,POWER& pwr);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LMBTRANSFERKLINE_H__92CDDF41_D7BE_11D3_96E6_0080C8E1242B__INCLUDED_)
