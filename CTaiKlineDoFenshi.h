// CTaiKlineDoFenshi.h: interface for the CTaiKlineMin1 class.
//		
// �ļ�����CTaiKlineDoFenshi.h

// ����������
//		
//		����ʵʱ�������ݻ�ʵʱ�������ߣ�������Ӧ�����ݻ����ڴ�λͼ�ϡ�
//		
//		��������룺CTaiShanKlineShowView��ͼ���ָ�뼰��Ӧ���ĵ�ָ�롣
//		
//		�����������Ѽ���������ݻ����ڴ�λͼ�ϡ�
//		
//
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LDOFENSHI_H__20183F81_BDC6_11D2_91D9_0080C8E1242B__INCLUDED_)
#define AFX_LDOFENSHI_H__20183F81_BDC6_11D2_91D9_0080C8E1242B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
const int  FS_ZOUSHI=		0;
const int  FS_VOL	=		1;
const int  FS_MMLD	=		2;
const int  FS_LB	=		3;
const int  FS_LINXIAN	=	4;
const int  FS_DUOKONG	=	5;
const int  FS_TONGLUO	=	6;
#include "CTaiShanDoc.h"

#include "CTaiKlineDoKline.h"

class CTaiShanDoc;
class CTaiShanKlineShowView;
class CTaiKlineFileHS;
class CTaiKlineDialogShiDuanTJ ;
class CTaiKlineMin1  :public	CTaiKlineShowKline
{
	DECLARE_DYNCREATE(CTaiKlineMin1)
public:
	int TransferX(int x);//��x����ת��Ϊ��Ӧ�������±�
	static void GetFenJia(int first,CBuySellList &buySellList,CFJList&	fenjiaUp,CFJList&	fenjiaDown,int nFlag = 0);
	void SetRectDraw(int nFigu);
	CReportData m_dt;	//���ɵ�����
	CBuySellList m_hsMin1 ;//�������ݵ�����
	TRADE_DETAIL_H_PER	m_hsMin[240];//ת�䵽�����ߵĻ�������
	bool 	m_bHist;//�Ƿ�����ʾ��ʷ����

	Tidxd **m_Tidx;//��������
	Rsdn1 **m_Nidx;//��������
	CTaiKlineFileHS*	m_pFileHs;//���������ļ�ָ��
	int m_nCnp;//�Ƿ���ʾ�ɽ����ѻ�
	float m_amount[240];//�ɽ���
	float m_vol[240];//�ɽ���
	float m_volPre;//��һ�εĳɽ���
	int  num_sun;//�ұ�Сͼ�ĵڼ�����ͼ
	float m_max_dapan[2][2];//�������ݵ����ֵ
	float m_min_dapan[2][2];//�������ݵ���Сֵ
	float* m_lineDapan[2][3];//������������
	float m_close_Dapan[2];//���̵����̼�
	float m_close;//�������̼�
	int m_pFlg[5];//��ͼ�ı�־��Ϣ 
	int		m_isDapan;//�Ƿ��Ǵ���ָ��
	CReportData* m_pReportData;	//���ɵ�����ָ��
	CReportData* m_pS0;	//���ɵ�����ָ��
	CReportData* m_pS1;	//���ɵ�����ָ��
	CString m_nameSon[5];//��ͼ������
	bool m_bInvertRect;		//�Ƿ�Ѿ��η�ɫ

	int m_nCount;


	void ShowCNP(CDC *pDC,int nFlags=0);//��ʾ�ɽ����ѻ�

	void DrawDapanOther(CDC* pDC);
	//����:������������ͼ

	void CaclMaxAdded();
	//����:������ͼ�����������ֵ

	void AddFenshiAdded(CString symbol, int nStock,int stkKind);
	//����:������ͼ���ӹ�Ʊ
	//�������:symbolҪ���ӵĹ�Ʊ����,nStock�ǵڼ�����Ʊ

//	int OneTo5(Kline* pKline,int nFlags,CString symbol,int n);//�����ʱ����ת��Ϊ5����K��
	//�������:symbol��Ʊ����,pKline�Ǵ��������ָ��,nFlags��ʶ����K��

	void ShowMultiFiguer(CDC *pDC,int flag=0);//��ʾ��ͼ
	void DrawRulorX(CDC *pDC);//��x����
	int OutString(int num, CPoint p);//����ַ���
	//�������:num�ǵڼ�����ͼ,p�����ĵ�

	int OnHome();//home��ͷ������
	int OnEnd();//end��ͷ������
	int OnRight();//right��ͷ������
	int OnLeft();//left��ͷ������
	CString TimeToString(int foot,bool bRight= false);//ʱ��ת��Ϊ�ַ���
	//�������:foot���ݵ��±�

	void DrawLittleFrame(CDC* pDC,int nMarket=0);//��С����
	void DrawLittleDapan(CDC* pDC,int flagLine,int nMarket=0);//flagLine=0Ϊ��ʱ�ߣ�flagLine=1Ϊ����,flagLine=2Ϊ�ɽ���	void DrawLittleGegu(CDC* pDC);//���ұ�Сͼ
	void DrawLittleGegu(CDC *pDC);//������С����
	void DrawDapan(CDC *pDC,int nMarket=0,int flagFrame=0);//���ұ�Сͼ
	int DrawHs(CDC *pDC,int nBegin, BYTE flag);//���ұ߳ɽ���ϸСͼ
	//�������:nBegin�ǿ�ʼ��ʾ�����ݵ��±�
	
	void InitHs(bool bRemoveAll=true,bool bSkip = true);//��ʼ�����ּ��ּ�����
	//�������:bRemoveAll=true�ǰѳɽ���ϸ����ȫ������ٶ���

	void DrawCapt(CDC* pDC);//������
	void DrawSon(CDC* pDC);//����ͼ
	void InitMinuteLine();//��ʼ������
	//�������:symbol��Ʊ����
//	CTaiShanDoc* pDoc;//�ĵ�ָ��
//	CTaiShanKlineShowView* pView;//��ͼָ��

	void DrawMin1Figuer(CDC* pDC);//����ʱ����
	CTaiKlineMin1();
	CTaiKlineMin1(CTaiShanDoc* pDocI,CTaiShanKlineShowView* pViewI	);

	float TongJi(CTaiKlineDialogShiDuanTJ* pTJ,int nFlags=0);//ͳ������
	//���������pTJΪʱ��ͳ�ƶԻ���ָ��
	//��������������Ƿ�ɹ�

	virtual ~CTaiKlineMin1();

};

#endif // !defined(AFX_LDOFENSHI_H__20183F81_BDC6_11D2_91D9_0080C8E1242B__INCLUDED_)
