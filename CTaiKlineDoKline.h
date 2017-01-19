// CTaiKlineDoKline.h: interface for the CTaiKlineShowKline class.
//		
// �ļ�����CTaiKlineDoKline.h

// �����ߣ�

// ����ʱ�䣺

// ����������
//		
//		����K�����ݼ��㼼��ָ�꣬������Ӧ�����ݻ����ڴ�λͼ�ϡ�
//		
//		��������룺CTaiShanKlineShowView��ͼ���ָ�뼰��Ӧ���ĵ�ָ�롣
//		
//		�����������Ѽ���������ݻ����ڴ�λͼ�ϡ�
//		
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LDOKLINE_H__47E83166_C70C_11D2_91D8_0080C8E1242B__INCLUDED_)
#define AFX_LDOKLINE_H__47E83166_C70C_11D2_91D8_0080C8E1242B__INCLUDED_

#include "LBsInfoRS.h"	// Added by ClassView
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#define Total_Count_Line						64

#include "StructKlineView.h"
#define BUY_CASE 107
#define SELL_CASE 108

class CTaiShanDoc;//CTaiShanDoc
class CTaiShanKlineShowView;
class CFormularCompute;
class CTaiKlineDialogShiDuanTJ ;
class CTaiKlineMoveChenben;
class CTaiKlineTransferKline;

class CTaiKlineShowKline : public CObject
{
	DECLARE_DYNCREATE(CTaiKlineShowKline)
public:
	void GetRect(CRect &r,int nFiguer);
	int m_nValidIndicator[5];//ָ���ڵ�ǰ�����Ƿ���Ч
	void SetRectCurrent(int nFiguer);
	void DrawIndicatorAdded(CDC* pDC);
	CString m_sIndicatorAdded[5];//ָ���������
	int m_nLineIndicator1[5];

	CString m_sOldSymbol;//��һ�εĹ�Ʊ����
	int m_sOldStkKind;//��һ�εĹ�Ʊ����

	int m_nKlineType;
	int m_numOldKline;
	bool ValidDoKline(bool bMakeUp = false);
	CTaiKlineTransferKline* m_pLmbTransferKline;

	int m_nBeginFootTJ;//ʱ��ͳ�ƵĿ�ʼ��
	int m_nEndFootTJ;//ʱ��ͳ�ƵĽ�����
	bool m_bInvertRect;//�Ƿ�Ѿ��η�ɫ

	bool m_bSetPageData;//�Ƿ񱣴浱ǰ������Ϣ
//	bool m_bBTX;//�Ƿ��Ǳ�����
	CString m_zhiBiao0;//first index name
	int m_highFoot;//���ֵ���ݵ��±�
	int m_lowFoot;//��͵����ݵ��±�
	int m_bDaPan[4];//�Ƿ��Ǵ���
	bool m_bInitFoot;//�Ƿ������ֹ�±�
	CTaiKlineMoveChenben* m_pMoveCB;//�����ƶ��ɱ��ֲ�����ָ��

	int m_kLine_mode ;//��K�ߵķ�ʽ
	bool m_bNewStock;//�Ƿ����µĹ�Ʊ

	bool m_bClosed;//�Ƿ�����
	bool m_bDrived;//�Ƿ���������
	Kline* m_pKlineAdd[6];//��ͼ�������� 
	int m_nKlineAdd[6];//��ͼ����K�����ݵĸ��� 
	int m_nKlineFileAdd[6];//��ͼ����K�����ݵĸ��� 
	CString m_nameAdd[6];//��ͼ���ӹ�Ʊ���� 
	CString m_symbolAdd[6];//��ͼ���ӹ�Ʊ����

	//lmb 20041012
	int m_stkKindAdd[6];//��ͼ���ӹ�Ʊ����

	float m_amountAdd[6];//��ͼ��������ת��ʱ�ĳɽ����
	float m_volAdd[6];//��ͼ��������ת��ʱ�ĳɽ���
	CString m_lineName[5][Total_Count_Line];//�ߵ�����
	int m_nKlineCurrent;//�ǵڼ���K��
	int m_nTextBox;//�ڼ�����ͼ
	int m_axisType;//0Ϊ��ͨ���꣬1Ϊ�ٷֱ����꣬2Ϊ��������
	float m_currentValue;//��ǰֵ
	int m_footCurrent;//��ǰ��������±�
	CPoint pt[6];//�γ�����ĵ�
	CString m_tipStr;//������Ϣ���ڵ��ַ���
	CTaiShanDoc* pDoc;		//CTaiShanDoc��ָ��
 	CTaiShanKlineShowView* pView;		//CTaiShanKlineShowView��ָ��
	int	m_lineBgn;		//��������ָ����������Ŀ�ʼ�±꣬
						//�����ڱ�ǻ�ͼ�Ŀ�ʼ�±�
	int m_step[5];					//������ֲ���

	bool	m_bToCacl;		//�Ƿ����K������
	int	  m_klinNumDefault;		//��ʾK�ߵĳ�ʼ����
//	int   m_footChenBenB;			//��ʾK�ߵĿ�ʼ�����ݵĸ���
	int   m_footBegin;			//��ʾK�ߵĿ�ʼ�����ݵĸ���
	int   m_footEnd;			//��ʾ��K�ߵ��������ݵĸ���
	int	  m_klinNumDefaultOld;		//��ʾK�ߵ��ϴεĳ�ʼ����
	int   m_footBeginOld;		//��ʾK���ϴεĵĿ�ʼ�����ݵĸ���
	int   m_footEndOld;			//��ʾ��K���ϴεĵ��������ݵĸ���
	CRect m_rectDrawLine;		//���ڿͻ��������������л�ͼ�ľ���
	ARRAY_JISHU		m_dataFormular[5];	//����K��ͼ���ĸ���ͼ�ļ��������������ݼ�����
	float m_min_sun[5];			//�ڴ��ڿͻ�����ĳ��ͼ�ڣ� ��ͼ���������ݵ���Сֵ
	float m_max_sun[5];			//�ڴ��ڿͻ�����ĳ��ͼ�ڣ���ͼ���������ݵ����ֵ
	int	  m_nSon;			//��ʶ�ڼ�����ͼ
	powerArr m_PowerArray;//

	bool DrawBTX(CDC* pDC);
	//���ܣ���������
	void DrawKlineHS(CDC* pDC);
	//���ܣ����ֱ���ϸͼ
	void MoveDragMode(CPoint & pPre,CPoint& pNext);
	//���ܣ����ֱ���ϸͼ
	//���������pPre��һ�㣬pNext�ڶ���

	void OutBitmap(CDC *pDC,int id,CPoint & p);
	//���ܣ���λͼ
	//���������idΪλͼ��Դid�ţ�p Ϊ����λ��

	bool DrawRulorTextSelf(CDC * pDC,int wid);
	//���ܣ����Զ�����
	//���������widΪ��־��

	bool ShowFenshiRulor(CDC *pDC,int wid);
	//���ܣ�����ʱ�߱��
	//���������widΪ��־��
	void ShowVirtualLine(CDC* pDC,CPoint& p1,CPoint& p2);
	//���ܣ�������
	//���������p1Ϊ��1��p2Ϊ��2

	void ShowTjxgShow(CDC* pDC);//��ʾ����ѡ��ָʾ
	void DrawPower(CDC* pDC);//����Ȩ
	void DrawKLineAmericaAdded(CDC *pDC,int nKln);//����ͼ����������
	//���������nKlnΪ�ڼ���K��
	void WritePower(CString &symbol,int stkKind);//д��Ȩ����
	//���������symbolΪ��Ʊ����
	int ReadPower(CString& symbol,int stkKind);//����Ȩ����
	//���������symbolΪ��Ʊ����
	CTaiKlineShowKline();

	void DrawKLineAddedPage(CDC *pDC);//����ͼ����
	void CaclMaxMin(Kline* pKline,int b,int e);//����K�����ݵ������Сֵ
	//���������pKlineΪ�����k�����ݣ�bΪ��ʼ�㣬eΪ������
	int CaclEndFoot(int nKline);//������ͼ���ӵĹ�Ʊ�Ľ����±�
	//���������nKlineΪ�ڼ���K��
	void CaclMaxAdded();//������ͼ���ӵ����ֵ
	int CaclBeginFoot(int nKline);//������ͼ���ӵĹ�Ʊ�Ŀ�ʼ�±�
	//���������nKlineΪ�ڼ���K��
	int FindTimeK(int keyVlu,Kline* pKline,int max);//����ĳʱ���K��
	//���������keyVluΪҪ���ҵ�ʱ�䣬pKlineΪ�����k�����ݣ�max��k�����ݵ�������
	//������������ز��ҵ�����������±�

	void DeleteKlineAdded();//��ͼ����ɾ��K��
	void AddKlineAdded(CString symbol,int stkKind);//��ͼ���Ӽ���һK��
	//���������symbolΪ��Ʊ����
	void CaclPercentMaxMin(float& maxPct,float& minPct);//����ٷֱ����ֵ
	void DrawBaseInfo(CDC* pDC);//����������
	void RestoreKLine();//
	void ComputePower(CString symbol,int stkKind,Kline* pKline,int nKln);//�����Ȩ
	//���������symbolΪ��Ʊ���룬pKlineΪ�����k�����ݣ�nKln��k�����ݵĸ���
	int TransferKlineTypeDataToday(Kline*& pKline,CString symbol,int stkKind,int nFlags);//ת�����������
	//���������symbolΪ��Ʊ���룬pKlineΪ�����k�����ݣ�nFlags�Ǳ�־��
	//�������������k�����ݵĸ���
	int TransferKlineTypeData(CString symbol,int stkKind,Kline*& pKline,int nFlags=-1,int nKln=-1);//�����ߵ�ת��Ϊ��������
	//���������symbolΪ��Ʊ���룬pKlineΪ�����k�����ݣ�nFlags�Ǳ�־�֣�nKln��k�����ݵĸ���
	//�������������k�����ݵĸ���
	float TongJi(CTaiKlineDialogShiDuanTJ* pTJ,int nFlags=0);//ͳ������
	//���������pTJΪʱ��ͳ�ƶԻ���ָ��
	//��������������Ƿ�ɹ�
	float ShowChengBen(CDC *pDC,int nFlags=0);//��ʾ�ɱ�ͳ��
	void ShowCNP(CDC* pDC,int nFlags=0);//��ʾ�ɽ����ѻ�
	void ShowMultiPage(CDC* pDC);//��ʾ��ͼ
	void ShowMultiFiguer(CDC* pDC,int flag=0);//��ʾ��ͼ֮һ
	void ShowLittleRulor(CDC* pDC,int wid=0);//��ʾ��ͼ�ı��
	void DrawRulorX(CDC *pDC);//��ʱ����
	int DrawRulorTextLog(CDC *pDC,BYTE wid=0);//��ָ������Y���
    int OnHome();//��home��
    int OnEnd();//��end��
	int OnUp();//���ϼ�ͷ����
	int OnDown();//���¼�ͷ����
	int OnLeft();//�����ͷ����
	int OnRight();//���Ҽ�ͷ����
	int TransferX(int x);//��x����ת��Ϊ��Ӧ�������±�
	int OutString(int num, CPoint p);//����ַ���
	//�������:num�ǵڼ�����ͼ,p�����ĵ�
	int MoveDisplays(int num);//�ƶ���ʾK �߻���
	//�������:num���ƶ���������
	int SetShowTotal(int num);//������ʾ����ʼ��ͽ�����
	CString TimeToStr(int foot);//ʱ��ת��Ϊ�ַ���

	void DrawCaption(CDC* pDC);//������
	void DrawCaptionAdded(CDC *pDC);//������
	void CalcSonFiguer(CString name);//������ͼ����
	void DrawSonFiguer(CDC* pDC);	//����ͼ
	CTaiKlineShowKline(CTaiShanKlineShowView* bsView,CTaiShanDoc* bsDoc);

	void DrawRulorText(CDC* pDC,BYTE wid=0);	//����ߵı������
	void DrawKlineFiguer(CDC* pDC);//��K�߻���
	void DrawKLine(CDC *pDC);		//��K�߻����ڴ�λͼ��
	void DrawKLineAdded(CDC *pDC,int nKln);		//��K�߻����ڴ�λͼ��
	void DrawKLineAmeric(CDC *pDC);//��K�߻����ڴ�λͼ��
	void InitDrawedLineBE();		//���û�K�ߵ���ʼ��
	int InitLineParam();	//��ʼ�����ߵ���ʼ��
	void CaclMaxFlt(Kline *pK,int footBegin);	//����K�ߵ���� ��Сֵ
	void CaclMaxFlt(float* pFlt,int footBegin,int nLine=-1);	//�����ߵ���� ��Сֵ

	void DrawLineIndex(CDC* pDC,bool bFenshi = false);	//������ָ������

	float YTransfer(int y);		//��Ļ����Yת��Ϊʵ������
	int YTransfer(float y);		//ʵ������ת��Ϊ��Ļ����Y

	void CaclRulorMovable(CPoint point);		//����ͻ����α�С����

	//-- ��Ȩ

	virtual ~CTaiKlineShowKline();

private:
	void DrawTextIconLine(CDC *pDC,int i);
	CString GetParamString(CString sName);
	void DrawBTXAdded(CDC *pDC, int nKln);
};
#ifndef _DEBUG  // debug version in CTaiShanKlineShowView.cpp
inline	float CTaiKlineShowKline::YTransfer(int y)		//��Ļ����Yת��Ϊʵ������
{
	float rtn;
	if((m_rectDrawLine.top-m_rectDrawLine.bottom)==0)
		return 0;

	if(!(m_nSon==0&&m_axisType==2))
		rtn=((float)(m_max_sun[m_nSon]-m_min_sun[m_nSon]))*(y-m_rectDrawLine.bottom)/(m_rectDrawLine.top-m_rectDrawLine.bottom)+m_min_sun[m_nSon];
	else
	{
		rtn=(float)(pow(10,(log10(m_max_sun[m_nSon])-log10(m_min_sun[m_nSon]))
			*(y-m_rectDrawLine.bottom)/(m_rectDrawLine.top-m_rectDrawLine.bottom)
			+log10(m_min_sun[m_nSon])));
	}

	return rtn;
}
inline	int CTaiKlineShowKline::YTransfer(float y)		//ʵ������ת��Ϊ��Ļ����Y
{
	int rtn;
	if((m_max_sun[m_nSon]-m_min_sun[m_nSon])==0)
	{
		rtn = -10;
		return rtn;
	}

	if(!(m_nSon==0&&m_axisType==2))
		rtn=(int)((float)(m_rectDrawLine.top-m_rectDrawLine.bottom)*(y-m_min_sun[m_nSon])/(m_max_sun[m_nSon]-m_min_sun[m_nSon])+m_rectDrawLine.bottom+0.5);
	else
	{
		if(m_min_sun[m_nSon]<=0 || m_max_sun[m_nSon]<=0 || y<=0)
			return 0;

		rtn=(int)((m_rectDrawLine.top-m_rectDrawLine.bottom)*(log10(y)-log10(m_min_sun[m_nSon]))/(log10(m_max_sun[m_nSon])-log10(m_min_sun[m_nSon]))+m_rectDrawLine.bottom+0.5);
	}
	return rtn;
}
#endif
#endif // !defined(AFX_LDOKLINE_H__47E83166_C70C_11D2_91D8_0080C8E1242B__INCLUDED_)
