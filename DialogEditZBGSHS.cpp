// DIAEDITZBGSHS.cpp : implementation file
//

#include "stdafx.h"
#include "DialogEditZBGSHS.h"
#include "CTaiShanDoc.h"
#include "MainFrm.h"
#include "CTaiKlineIndicatorsManage.h"
#include "WH.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDialogEDITZBGSHS dialog


CDialogEDITZBGSHS::CDialogEDITZBGSHS(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogEDITZBGSHS::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDialogEDITZBGSHS)
	m_sExplain = _T("");
	//}}AFX_DATA_INIT
}


void CDialogEDITZBGSHS::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialogEDITZBGSHS)
	DDX_Control(pDX, IDOK, m_ok);
	DDX_Control(pDX, IDC_TREE1, m_tree);
	DDX_Text(pDX, IDC_EDIT1, m_sExplain);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDialogEDITZBGSHS, CDialog)
	//{{AFX_MSG_MAP(CDialogEDITZBGSHS)
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE1, OnSelchangedTree1)
	ON_NOTIFY(NM_DBLCLK, IDC_TREE1, OnDblclkTree1)
	ON_NOTIFY(NM_RETURN, IDC_TREE1, OnReturnTree1)
	ON_WM_HELPINFO()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialogEDITZBGSHS message handlers
dataFunc stritemsArr[] = //196
	{
	"���麯��",-1,"",//24

	"ACTIVEBUYVOL---����������",0,"����������",
	"ACTIVESELLVOL---����������",0,"����������",
	"ADVANCE---���ǹ�Ʊ��(����ָ��)",0,"ȡ�ø��������Ǽ�����\r\n�÷�:\r\nADVANCE\r\n(���������Դ�����Ч)",
	"AMOUNT---�ɽ���",0,"ȡ�ø����ڳɽ��\r\n�÷�:\r\nAMOUNT",
	"ASKPRICE(1)---ί����1",0,"ί����1(�������������ڷֱʳɽ�����������Ч)",
	"ASKPRICE(2)---ί����2",0,"ί����2(�������������ڷֱʳɽ�����������Ч)",
	"ASKPRICE(3)---ί����3",0,"ί����3(�������������ڷֱʳɽ�����������Ч)",
	"ASKVOL(1)---ί����1",0,"ί����1(�������������ڷֱʳɽ�����������Ч)",
	"ASKVOL(2)---ί����2",0,"ί����2(�������������ڷֱʳɽ�����������Ч)",
	"ASKVOL(3)---ί����3",0,"ί����3(�������������ڷֱʳɽ�����������Ч)",
	"BIDVOL(1)---ί����1",0,"ί����1(�������������ڷֱʳɽ�����������Ч)",
	"BIDVOL(2)---ί����2",0,"ί����2(�������������ڷֱʳɽ�����������Ч)",
	"BIDVOL(3)---ί����3",0,"ί����3(�������������ڷֱʳɽ�����������Ч)",
	"BIDPRICE(1)---ί���1",0,"ί���1(�������������ڷֱʳɽ�����������Ч)",
	"BIDPRICE(2)---ί���2",0,"ί���2(�������������ڷֱʳɽ�����������Ч)",
	"BIDPRICE(3)---ί���3",0,"ί���3(�������������ڷֱʳɽ�����������Ч)",
	"BUYVOL---�������ݵ�����������",0,"ȡ��������������\r\n�÷�:\r\nBUYVOL\r\n�����ʳɽ�Ϊ����������ʱ,����ֵ���ڳɽ���,����Ϊ0\r\n(�������������ڷֱʳɽ�����������Ч)\r\n",
	"CLOSE---���̼�",0,"ȡ�ø��������̼ۡ�\r\n�÷�:\r\nCLOSE\r\n",
	"DECLINE---�µ���Ʊ��(����ָ��)",0,"ȡ�ø������µ�������\r\n�÷�:\r\nDECLINE\r\n(���������Դ�����Ч)",
	"HIGH---��߼�",0,"ȡ�ø�������߼ۡ�\r\n�÷�:\r\nHIGH",
	"ISBUYORDER---�Ƿ�����",0,"ȡ�øóɽ��Ƿ�Ϊ�������򵥡�\r\n�÷�:\r\nISBUYORDER\r\n�����ʳɽ�Ϊ����������ʱ,����1,����Ϊ0\r\n(�������������ڷֱʳɽ�����������Ч)",
	"LOW---��ͼ�",0,"ȡ�ø�������ͼۡ�\r\n�÷�:\r\nLOW",
	"OPEN---���̼�",0,"ȡ�ø����ڿ��̼ۡ�\r\n�÷�:\r\nOPEN",
	"SELLVOL---�������ݵ�����������",0,"ȡ����������������\r\n�÷�:\r\nBUYVOL\r\n�����ʳɽ�Ϊ����������ʱ,����ֵ���ڳɽ���,����Ϊ0\r\n(�������������ڷֱʳɽ�����������Ч)\r\n",
	"VOL---�ɽ���",0,"ȡ�ø����ڳɽ�����\r\n�÷�:\r\nVOL",

	"ʱ�亯��",-1,"",//8
	"DAY---����",0,"ȡ�ø����ڵ����ڡ�\r\n�÷�:\r\nDAY\r\n����������Чֵ��ΧΪ(1-31)",
	"DATE---������,ȡ�ø����ڴ�1900������������",0,"ȡ�ø����ڴ�1900�����������ա�\r\n�÷�:\r\nDATE\r\n����������Чֵ��ΧΪ(700101-1341231),��ʾ19700101-20341231",
	"HOUR---Сʱ",0,"ȡ�ø����ڵ�Сʱ����\r\n�÷�:\r\nHOUR\r\n����������Чֵ��ΧΪ(0-23)���������߼������ķ�������ֵΪ0",
	"MINUTE---����",0,"ȡ�ø����ڵķ�������\r\n�÷�:\r\nMINUTE\r\n����������Чֵ��ΧΪ(0-59)���������߼������ķ�������ֵΪ0",
	"MONTH---�·�",0,"ȡ�ø����ڵ��·ݡ�\r\n�÷�:\r\nMONTH\r\n����������Чֵ��ΧΪ(1-12)",
	"TIME---ʱ����",0,"ȡ�ø����ڵ�ʱ���롣\r\n�÷�:\r\nTIME\r\n����������Чֵ��ΧΪ(000000-235959)",
	"VALUEDAY(DAY,HOUR,X)---DAY���»������գ�HOUR��ʱ�ֻ�ʱ�ֵ�X��ֵ��",0,"�磺VALUEDAY(200103,210930,CLOSE) ��ʾ2001��3��21��9��30��ʱ�����̼۸�",
	"WEEK---����",0,"ȡ�ø����ڵ��·ݡ�\r\n�÷�:\r\nWEEK\r\n����������Чֵ��ΧΪ(0-6)",
	"YEAR---���",0,"ȡ�ø����ڵ���ݡ�\r\n�÷�:\r\nYEAR\r\n����������Чֵ��ΧΪ(1970-2038)",

	"���ú���",-1,"",//20
	"BACKSET(X,N)---��X��0,�򽫵�ǰλ�õ�N����ǰ����ֵ��Ϊ1",0,"����ǰλ�õ���������ǰ��������Ϊ1��\r\n�÷�:\r\nBACKSET(X,N),��X��0,�򽫵�ǰλ�õ�N����ǰ����ֵ��Ϊ1��\r\n���磺BACKSET(CLOSE>OPEN,2)�������򽫸����ڼ�ǰһ������ֵ��Ϊ1,����Ϊ0",
	"BARSCOUNT(X)---��һ����Ч���ݵ���ǰ������",0,"���ܵ���������\r\n�÷�:\r\nBARSCOUNT(X)��һ����Ч���ݵ���ǰ������\r\n���磺BARSCOUNT(CLOSE)������������ȡ�����������ܽ������������ڷֱʳɽ�ȡ�õ��ճɽ�����������1������ȡ�õ��ս��׷�����",
	"BARSLAST(X)---��һ��X��Ϊ0�����ڵ�����",0,"��һ��������������ǰ����������\r\n�÷�:\r\nBARSLAST(X):��һ��X��Ϊ0�����ڵ�����\r\n���磺BARSLAST(CLOSE/REF(CLOSE,1)>=1.1)��ʾ��һ����ͣ�嵽��ǰ��������",
	"BARSSINCE(X)---��һ��X��Ϊ0�����ڵ�����",0,"���磺BARSSINCE(HIGH>10)��ʾ�ɼ۳���10Ԫʱ����ǰ��������",
	"COUNT(X,N)---ͳ��N����������X������������,��N=0��ӵ�һ����Чֵ��ʼ",0,"��һ��������������ǰ����������\r\n�÷�:\r\nBARSSINCE(X):��һ��X��Ϊ0�����ڵ�����\r\n���磺BARSSINCE(HIGH>10)��ʾ�ɼ۳���10Ԫʱ����ǰ��������",
	"DMA(X,A)---��X�Ķ�̬�ƶ�ƽ��",0,"��̬�ƶ�ƽ����\r\n�÷�:\r\nDMA(X,A),��X�Ķ�̬�ƶ�ƽ����\r\n�㷨: ��Y=DMA(X,A)\r\n�� Y=A*X+(1-A)*Y',����Y'��ʾ��һ����Yֵ,A����С��1��\r\n���磺DMA(CLOSE,VOL/CAPITAL)��ʾ���Ի�������ƽ�����ӵ�ƽ����",
	"EMA(X,N)---����X��N��ָ��ƽ���ƶ�ƽ��ֵ",0,"��ָ��ƽ���ƶ�ƽ����\r\n�÷�:\r\nEMA(X,N),��X��N��ָ��ƽ���ƶ�ƽ�����㷨����Y=EMA(X,N)\r\n��Y=[2*X+(N-1)*Y']/(N+1),����Y'��ʾ��һ����Yֵ��\r\n���磺EMA(CLOSE,30)��ʾ��30��ָ��ƽ������",
	"HHV(X,N)---����N����X�����ֵ",0,"�����ֵ��\r\n�÷�:\r\nHHV(X,N),��N������X���ֵ,N=0��ӵ�һ����Чֵ��ʼ��\r\n���磺HHV(HIGH,30)��ʾ��30����߼�",
	"HHVBARS(X,N)---��N������X���ֵ����ǰ��������N=0��ʾ�ӵ�һ����Чֵ��ʼͳ��",0,"����һ�ߵ㵽��ǰ����������\r\n�÷�:\r\nHHVBARS(X,N):��N������X���ֵ����ǰ��������N=0��ʾ�ӵ�һ����Чֵ��ʼͳ��\r\n���磺HHVBARS(HIGH,0)�����ʷ�¸ߵ�����ǰ��������",
	"LLV(X,N)---����N����X�����ֵ",0,"�����ֵ��\r\n�÷�:\r\nLLV(X,N),��N������X���ֵ,N=0��ӵ�һ����Чֵ��ʼ��\r\n���磺LLV(LOW,0)��ʾ����ʷ��ͼ�",
	"LLVBARS(X,N)---��N������X���ֵ����ǰ��������N=0��ʾ�ӵ�һ����Чֵ��ʼͳ��",0,"����һ�͵㵽��ǰ����������\r\n�÷�:\r\nLLVBARS(X,N):��N������X���ֵ����ǰ��������N=0��ʾ�ӵ�һ����Чֵ��ʼͳ��\r\n���磺LLVBARS(HIGH,20)���20����͵㵽��ǰ��������",
	"MA(X,N)---����X��N���ƶ�ƽ��ֵ",0,"����ƶ�ƽ����\r\n�÷�:\r\nMA(X,N),��X��N���ƶ�ƽ��ֵ���㷨��(X1+X2+X3+...+Xn)/N\r\n���磺MA(CLOSE,10)��ʾ��10�վ���",
	"REF(X,N)---����X��N��ǰ��ֵ",0,"������������ǰ�����ݡ�\r\n�÷�:\r\nREF(X,A),����A����ǰ��Xֵ��\r\n���磺REF(CLOSE,1)��ʾ��һ���ڵ����̼ۣ��������Ͼ�������",
	"REFBEGIN(X,N)---����X���������ݵĵ�N�����ݵ�ֵ",0,"���磺REFBEGIN(CLOSE,1)��ʾCLOSE���������ݵĵ�1�����ݵ�ֵ",
	"REFLAST(X,N)---����X���������ݵĵ�����N�����ݵ�ֵ",0,"���磺REFLAST(CLOSE,1)��ʾCLOSE���������ݵĵ�����1�����ݵ�ֵ",
	"SMA(X,N,M)---��X��N���ƶ�ƽ����MΪȨ��",0,"���ƶ�ƽ����\r\n�÷�:\r\nSMA(X,N,M),��X��N���ƶ�ƽ����MΪȨ�ء�\r\n�㷨: ��Y=SMA(X,N,M)\r\n�� Y=[M*X+(N-M)*Y')/N,����Y'��ʾ��һ����Yֵ,N�������M��\r\n���磺SMA(CLOSE,30,1)��ʾ��30���ƶ�ƽ����",
	"SUM(X,N)---����N����X���ܺ�ֵ",0,"���ܺ͡�\r\n�÷�:\r\nSUM(X,N),ͳ��N������X���ܺ�,N=0��ӵ�һ����Чֵ��ʼ��\r\n���磺SUM(VOL,0)��ʾͳ�ƴ����е�һ�������ĳɽ����ܺ�",
	"SUMBARS(X,A)---��X��ǰ�ۼ�ֱ�����ڵ���A,������������������",0,"��ǰ�ۼӵ�ָ��ֵ�����ڵ���������\r\n�÷�:\r\nSUMBARS(X,A):��X��ǰ�ۼ�ֱ�����ڵ���A,������������������\r\n���磺SUMBARS(VOL,CAPITAL)����ȫ���ֵ����ڵ�������",
	"EXTDATA(N)---����ID��ΪN����չָ������",0,"ȡ��������չ����(1-999)��\r\n�÷�:\r\nEXTDATA(N),Nȡ1--999\r\n(�������������߷���������Ч)",
//					"reflast","refbegin","valueaverage"

	"��������",-1,"",//9
	"ABS(X)---X�ľ���ֵ",0,"�����ֵ��\r\n�÷�:\r\nABS(X)����X�ľ���ֵ\r\n���磺ABS(-34)����34",
	"BETWEEN(A,B,C)---��ʾA����B��C֮��ʱ����1�����򷵻�0",0,"����������֮�䡣\r\n�÷�:\r\nBETWEEN(A,B,C)��ʾA����B��C֮��ʱ����1�����򷵻�0\r\n���磺BETWEEN(CLOSE,MA(CLOSE,10),MA(CLOSE,5))��ʾ���̼۽���5�վ��ߺ�10�վ���֮��",
	"CROSS(A,B)---A�����ϴ�ԽB��Ϊ1,����Ϊ0",0,"�����߽��档\r\n�÷�:\r\nCROSS(A,B)��ʾ��A���·����ϴ���Bʱ����1�����򷵻�0\r\n���磺CROSS(MA(CLOSE,5),MA(CLOSE,10))��ʾ5�վ�����10�վ��߽����",
	"FILTER(X,N)---X����������ɾ�����N�����ڵ�������Ϊ0",0,"�����������ֵ��źš�\r\n�÷�:\r\nFILTER(X,N):X����������ɾ�����N�����ڵ�������Ϊ0\r\n���磺FILTER(CLOSE>OPEN,5)�������ߣ�5�����ٴγ��ֵ����߲�����¼����",
	"LONGCROSS(A,B,N)---��ʾA��N�����ڶ�С��B�������ڴ��·����ϴ���Bʱ����1�����򷵻�0",0,"���磺LONGCROSS(MA(CLOSE,5),MA(CLOSE,10),5)��ʾ5�վ���ά��5���ں���10�վ��߽����",
	"MAX(X,N)---���ֵ����",0,"�����ֵ��\r\n�÷�:\r\nMAX(A,B)����A��B�еĽϴ�ֵ\r\n���磺MAX(CLOSE-OPEN,0)��ʾ�����̼۴��ڿ��̼۷������ǵĲ�ֵ�����򷵻�0",
	"MIN(X,N)---��Сֵ����",0,"����Сֵ��\r\n�÷�:\r\nMIN(A,B)����A��B�еĽ�Сֵ\r\n���磺MIN(CLOSE,OPEN)���ؿ��̼ۺ����̼��еĽ�Сֵ",
	"MOD(A,B)---ȡ��A��B��ģ",0,"��ģ���㡣\r\n�÷�:\r\nMOD(A,B)����A��B��ģ\r\n���磺MOD(26,10)����6",
	"NOT(X)---ȡ���߼���",0,"���߼��ǡ�\r\n�÷�:\r\nNOT(X)���ط�X,����X=0ʱ����1�����򷵻�0\r\n���磺NOT(ISUP)��ʾƽ�̻�����\r\n",
	"RANGE(A,B,C)---��ʾA����BͬʱС��Cʱ����1�����򷵻�0",0,"����ĳ����Χ֮�䡣\r\n�÷�:\r\nRANGE(A,B,C)��ʾA����BͬʱС��Cʱ����1�����򷵻�0\r\n���磺RANGE(CLOSE,MA(CLOSE,5),MA(CLOSE,10))��ʾ���̼۴���5�վ��߲���С��10�վ���",
	"REVERSE(X)---ȡ��X���෴��-X",0,"���෴����\r\n�÷�:\r\nREVERSE(X)����-X\r\n����REVERSE(CLOSE)����-CLOSE",
	"SGN(X)---ȡ��X�ķ��ţ�����(1,0,-1)",0,"�����ֵ��\r\n�÷�:\r\nSGN(X)����X>0,X=0,X<0�ֱ𷵻�1,0,-1\r\n",

	"��ѧ����",-1,"",//13
	"ACOS(X)---����X�ķ�����ֵ",0,"������ֵ",
	"ASIN(X)---����X�ķ�����ֵ",0,"������ֵ",
	"ATAN(X)---����X�ķ�����ֵ",0,"������ֵ",
	"CEILING(X)---��������",0,"���磺CEILING(12.3)���13,CEILING(-3.5)���-3",
	"COS(X)---����X������ֵ",0,"����ֵ",
	"EXP(X)---��eΪ�׵�X����",0,"EXP(CLOSE)����e��CLOSE����",
	"FLOOR(X)---��������",0,"���磺FLOOR(12.3)���12,FLOOR(-3.5)���-4",
	"INTPART(X)---X������",0,"���磺INTPART(12.3)���12,INTPART(-3.5)���-3",
	"LN(X)---X����eΪ�׵Ķ���",0,"���磺LN(CLOSE)�����̼۵Ķ���",
	"LOG(X)---X����10Ϊ�׵Ķ���",0,"���磺LOG(100)����2",
	"POW(A,B)---A��B����",0,"���磺POW(CLOSE,3)������̼۵�3�η�",
	"SIN(X)---����X������ֵ",0,"����ֵ",
	"SQRT(X)---X��ƽ����",0,"���磺SQRT(CLOSE)���̼۵�ƽ����",
	"TAN(X)---����X������ֵ",0,"����ֵ",

	"�߼�����",-1,"",//5
	"IF(X,A,B)---��X�򷵻�A,���򷵻�B",0,"����������ͬ��ֵ��\r\n�÷�:\r\nIF(X,A,B)��X��Ϊ0�򷵻�A,���򷵻�B\r\n���磺IF(CLOSE>OPEN,HIGH,LOW)��ʾ�����������򷵻����ֵ�����򷵻����ֵ",
	"ISDOWN---�����µ���Ϊ1,����Ϊ0",0,"������<����ʱ������ֵΪ1������Ϊ0",
	"ISEQUAL---����ƽ����Ϊ1,����Ϊ0",0,"������=����ʱ������ֵΪ1������Ϊ0",
	"ISUP---����������Ϊ1,����Ϊ0",0,"������>����ʱ������ֵΪ1������Ϊ0",

	"ͳ�ƺ���",-1,"",//10
	"AVEDEV(X,N)---����X��N���ƽ������ƫ��",0,"ƽ������ƫ�\r\n�÷�:\r\nAVEDEV(X,N)\r\n",
	"DEVSQ(X,N)---����X��N���ƫ��ƽ����",0,"����ƫ��ƽ���͡�\r\n�÷�:\r\nDEVSQ(X,N)\r\n",
	"FORCAST(X,N)---X��N�������Իع�Ԥ��ֵ",0,"���Իع�Ԥ��ֵ��\r\n�÷�:\r\nFORCAST(X,N)ΪX��N�������Իع�Ԥ��ֵ\r\n����:FORCAST(CLOSE,10)��ʾ��10�������Իع�Ԥ�Ȿ�������̼�",
	"STD(X,N)---����X��N��Ĺ����׼��",0,"�����׼�\r\n�÷�:\r\nSTD(X,N)ΪX��N�չ����׼��\r\n",
	"STDP(X,N)---����X��N��������׼��",0,"�����׼�\r\n�÷�:\r\nSTDP(X,N)ΪX��N�������׼��\r\n",
	"SLOPE(X,N)---X��N�������Իع��ߵ�б��",0,"���Իع�б�ʡ�\r\n�÷�:\r\nSLOPE(X,N)ΪX��N�������Իع��ߵ�б��\r\n����:SLOPE(CLOSE,10)��ʾ��10�������Իع��ߵ�б��\r\n",
	"VALUEAVERAGE( PRICE,VOL,PERCENT)---����VOL��PERCENT����PRICE�ľ���",0,"",
	"VAR(X,N)---����X��N��Ĺ�����������",0,"�����������\r\n�÷�:\r\nVAR(X,N)ΪX��N�չ�����������\r\n",
	"VARP(X,N)---����X��N���������������",0,"�����������\r\n�÷�:\r\nVARP(X,N)ΪX��N��������������",

	"ָ�꺯��",-1,"",//15
	"COST(N)---��ʾN%�����̵ļ۸��Ƕ���",0,"�ɱ��ֲ������\r\n�÷�:\r\nCOST(10),��ʾ10%�����̵ļ۸��Ƕ��٣�����10%�ĳֲ����ڸü۸����£�����90%�ڸü۸����ϣ�Ϊ������\r\n�ú����������߷���������Ч",
	"DOWNWARD(X,N)---X����N���½�",0,"���磺DOWNWARD(CLOSE,5)��ʾCLOSE����5���½�",
	"PEAK(X,N,M)---ǰM��ZIGת�򲨷�ֵ",0,"ǰM��ZIGת�򲨷�ֵ��\r\n�÷�:\r\nPEAK(K,N,M)��ʾ֮��ת��ZIG(K,N)��ǰM���������ֵ,M������ڵ���1\r\n���磺PEAK(1,5,1)��ʾ%5��߼�ZIGת�����һ���������ֵ",
	"PEAKBARS(X,N,M)---ǰM��ZIGת�򲨷嵽��ǰ����",0,"ǰM��ZIGת�򲨷嵽��ǰ���롣\r\n�÷�:\r\nPEAKBARS(K,N,M)��ʾ֮��ת��ZIG(K,N)��ǰM�����嵽��ǰ��������,M������ڵ���1\r\n���磺PEAK(0,5,1)��ʾ%5���̼�ZIGת�����һ�����嵽��ǰ��������",
	"SAR(N,S,M)---����ת��,N:���� S:���� M:��ֵ",0,"����ת��\r\n�÷�:\r\nSAR(N,S,M),NΪ��������,SΪ����,MΪ��ֵ\r\n����SAR(10,2,20)��ʾ����10������ת�򣬲���Ϊ2%������ֵΪ20%",
	"SARTURN(N,S,M)---����ת��λ��,N:���� S:���� M:��ֵ",0,"����ת��㡣\r\n�÷�:\r\nSARTURN(N,S,M),NΪ��������,SΪ����,MΪ��ֵ,����������ת���򷵻�1������������ת���򷵻�-1������Ϊ0\r\n���÷���SAR������ͬ",
	"SLOWDOWNCROSS(N, X1,X2)---X1ά��N���ں��´�X2",0,"X1ά��N���ں��´�X2",
	"SLOWUPCROSS(N, X1,X2)---X1ά��N���ں��ϴ�X2",0,"X1ά��N���ں��ϴ�X2",
	"TROUGH(X,N,M)---��ʾ֮��ת��ZIG(X,N)��ǰM�����ȵ���ֵ,M������ڵ���1",0,"ǰM��ZIGת�򲨹�ֵ��\r\n�÷�:\r\nTROUGH(K,N,M)��ʾ֮��ת��ZIG(K,N)��ǰM�����ȵ���ֵ,M������ڵ���1\r\n���磺TROUGH(2,5,2)��ʾ%5��ͼ�ZIGת���ǰ2�����ȵ���ֵ",
	"TROUGHBARS(X,N,M)---��ʾ֮��ת��ZIG(X,N)��ǰM�����ȵ���ǰ��������,M������ڵ���1",0,"���磺TROUGH(2,5,2)��ʾ%5��ͼ�ZIGת���ǰ2�����ȵ���ǰ��������",
	"UPWARD(X,N)---X����N������",0,"X����N������",
	"ZIG(X,N)---���۸�仯������N%ʱת��,X��ʾ������ֵ",0,"֮��ת��\r\n�÷�:\r\nZIG(K,N),���۸�仯������N%ʱת��,K��ʾ0:���̼�,1:��߼�,2:��ͼ�,3:���̼�\r\n���磺ZIG(3,5)��ʾ���̼۵�5%��ZIGת��",
	"TOWERVALUE(N)---�����ߵĸ�����ֵ��N = 1��ʾ�����ߵ����ֵ��N = 2��ʾ�����ߵ����ֵ��\r\nN = 3��ʾ�����ߵ��м�ֵ��N = 4��ʾ�����ߵ�״̬��1Ϊ��ɫ��2Ϊ��ɫ��3Ϊ���к�������ɫ��",0,"�����ߵĸ�����ֵ",
	"WINNER(X)---�ֲ���X(ָ�����������)���ϵ����м�λ�ĳɽ����ĺ�ռ����ͨ�̵İٷֱ�",0,"�÷�:\r\nWINNER(CLOSE),��ʾ�Ե�ǰ���м������Ļ����̱��������緵��0.1��ʾ10%�����̣�WINNER(10.5)��ʾ10.5Ԫ�۸�Ļ����̱���\r\n�ú����������߷���������Ч",
	"WINNERTYPE(N)---��ͨ�̷ֲ��ຯ����N=1Ϊ��߷崦�ļ۸�N=2Ϊ��߷崦�ĳɽ����ܶȣ���/�֣���N=3Ϊƽ���ɱ����ļ۸�",0,"��ͨ�̷ֲ��ຯ��",

	"���̺���",-1,"",//9 
	"INDEXA---ָ���ɽ���",0,"ָ���ɽ���",
	"INDEXADV---�������Ǽ���",0,"�������Ǽ���",
	"INDEXC---ָ�����̼�",0,"ָ�����̼�",
	"INDEXDEC---�����µ�����",0,"�����µ�����",
	"INDEXH---ָ����߼�",0,"ָ����߼�",
	"INDEXL---ָ����ͼ�",0,"ָ����ͼ�",
	"INDEXO---ָ�����̼�",0,"ָ�����̼�",
	"INDEXV---ָ���ɽ���",0,"ָ���ɽ���",

	"��������",-1,"",//75
	"CAPITAL---��ͨ��",0,"��ͨ��",
	"VOLUNIT---ÿ�ֹ���",0,"ÿ�ֹ���",
	"DYNAINFO(3)---����(��������)",0,"��������",
	"DYNAINFO(4)---����(���տ���)",0,"���տ���",
	"DYNAINFO(5)---����(�������)",0,"�������",
	"DYNAINFO(6)---����(�������)",0,"�������",
	"DYNAINFO(7)---����(���¼۸�)",0,"���¼۸�",
	"DYNAINFO(8)---����(�ܳɽ���)",0,"�ܳɽ���",
	"DYNAINFO(9)---����(���³ɽ���)",0,"���³ɽ���",
	"DYNAINFO(10)---����(�ܳɽ����)",0,"�ܳɽ����",
	"DYNAINFO(11)---����(����)",0,"����",
	"DYNAINFO(12)---����(�ǵ�)",0,"�ǵ�",
	"DYNAINFO(13)---����(���)",0,"���",
	"DYNAINFO(14)---����(�Ƿ�)",0,"�Ƿ�",
	"DYNAINFO(15)---����(ί��)",0,"ί��",
	"DYNAINFO(16)---����(ί��)",0,"ί��",
	"DYNAINFO(17)---����(����)",0,"����",
	"DYNAINFO(18)---����(ί��)",0,"ί��",
	"DYNAINFO(19)---����(ί��)",0,"ί��",
	"DYNAINFO(20)---����(ί���)",0,"ί���",
	"DYNAINFO(21)---����(ί����)",0,"ί����",
	"DYNAINFO(22)---����(����)",0,"����",
	"DYNAINFO(23)---����(����)",0,"����",
	"DYNAINFO(24)---����(����)",0,"����",

	"DYNAINFO(25)---����(������һ)",0,"������һ",
	"DYNAINFO(26)---����(��������)",0,"��������",
	"DYNAINFO(27)---����(��������)",0,"��������",
	"DYNAINFO(28)---����(�����һ)",0,"�����һ",
	"DYNAINFO(29)---����(����۶�)",0,"����۶�",
	"DYNAINFO(30)---����(�������)",0,"�������",

	"DYNAINFO(31)---����(������һ)",0,"������һ",
	"DYNAINFO(32)---����(��������)",0,"��������",
	"DYNAINFO(33)---����(��������)",0,"��������",
	"DYNAINFO(34)---����(������һ)",0,"������һ",
	"DYNAINFO(35)---����(�����۶�)",0,"�����۶�",
	"DYNAINFO(36)---����(��������)",0,"��������",

	"DYNAINFO(37)---����(������)",0,"������",
	"DYNAINFO(38)---����(5�����)",0,"5�����",

/*	"FINANCE(1)---��������(�ܹɱ�(���))",0,"",
	"FINANCE(2)---��������(���ҹ�(���))",0,"",
	"FINANCE(3)---��������(�����˷��˹�(���))",0,"",
	"FINANCE(4)---��������(���˹�(���))",0,"",
	"FINANCE(5)---��������(ְ����(���))",0,"",
	"FINANCE(6)---��������(����A��(���))",0,"",
	"FINANCE(7)---��������(��ͨA��(���))",0,"",
	"FINANCE(8)---��������(�ȹ�(���))",0,"",
	"FINANCE(9)---��������(B��(���))",0,"",
	"FINANCE(10)---��������(ת���(���))",0,"",
	"FINANCE(11)---��������(���ʲ�(��Ԫ))",0,"",
	"FINANCE(12)---��������(�����ʲ�(��Ԫ))",0,"",
	"FINANCE(13)---��������(����Ͷ��(��Ԫ))",0,"",
	"FINANCE(14)---��������(�̶��ʲ�(��Ԫ))",0,"",
	"FINANCE(15)---��������(�����ʲ�(��Ԫ))",0,"",
	"FINANCE(16)---��������(������ծ(��Ԫ))",0,"",
	"FINANCE(17)---��������(���ڸ�ծ(��Ԫ))",0,"",
	"FINANCE(18)---��������(�ɶ�Ȩ��(��Ԫ))",0,"",
	"FINANCE(19)---��������(�ʱ�������(��Ԫ))",0,"",
	"FINANCE(20)---��������(ӯ�๫����(��Ԫ))",0,"",
	"FINANCE(21)---��������(ÿ�ɾ�ֵ(Ԫ))",0,"",
	"FINANCE(22)---��������(�ɶ�Ȩ�����(%))",0,"",
	"FINANCE(23)---��������(ÿ�ɹ�����(Ԫ))",0,"",
	"FINANCE(24)---��������(��Ӫҵ������(��Ԫ))",0,"",
	"FINANCE(25)---��������(��Ӫҵ������(��Ԫ))",0,"",
	"FINANCE(26)---��������(����ҵ������(��Ԫ))",0,"",
	"FINANCE(27)---��������(�����ܶ�(��Ԫ))",0,"",
	"FINANCE(28)---��������(������(��Ԫ))",0,"",
	"FINANCE(29)---��������(δ��������(��Ԫ))",0,"",
	"FINANCE(30)---��������(ÿ������(Ԫ))",0,"",
	"FINANCE(31)---��������(���ʲ�������(%))",0,"",
	"FINANCE(32)---��������(ÿ��δ��������(Ԫ))",0,"",
	"FINANCE(33)---��������(ÿ�ɾ��ʲ�(Ԫ))",0,"",*/
	"FINANCE(1)---����(�ܹɱ�(���))",0,"�ܹɱ�(���)",
	"FINANCE(2)---����(���ҹ�(���))",0,"���ҹ�(���)",
	"FINANCE(3)---����(�����˷��˹�(���))",0,"�����˷��˹�(���)",
	"FINANCE(4)---����(���˹�(���))",0,"���˹�(���)",
	"FINANCE(5)---����(B��(���))",0,"B��(���)",
	"FINANCE(6)---����(�ȹ�(���))",0,"�ȹ�(���)",
	"FINANCE(7)---����(��ͨA��(���))",0,"��ͨA��(���)",
	"FINANCE(8)---����(ְ����(���))",0,"ְ����(���)",
	"FINANCE(9)---����(ת���(���))",0,"ת���(���)",
	"FINANCE(10)---����(���ʲ�(ǧԪ))",0,"���ʲ�(ǧԪ)",
	"FINANCE(11)---����(�����ʲ�)",0,"�����ʲ�",
	"FINANCE(12)---����(�̶��ʲ�)",0,"�̶��ʲ�",
	"FINANCE(14)---����(�����ʲ�)",0,"�����ʲ�",
	"FINANCE(15)---����(����Ͷ��)",0,"����Ͷ��",
	"FINANCE(13)---����(������ծ)",0,"������ծ",
	"FINANCE(16)---����(���ڸ�ծ)",0,"���ڸ�ծ",
	"FINANCE(17)---����(�ʱ�������)    ",0,"(�ʱ�������)    ",
	"FINANCE(18)---����(ÿ�ɹ�����(Ԫ))",0,"(ÿ�ɹ�����(Ԫ))",
	"FINANCE(19)---����(�ɶ�Ȩ��)      ",0,"(�ɶ�Ȩ��)      ",
	"FINANCE(20)---����(��Ӫҵ������)  ",0,"(��Ӫҵ������)  ",
	"FINANCE(21)---����(��Ӫҵ������)  ",0,"(��Ӫҵ������)  ",
	"FINANCE(22)---����(����ҵ������)  ",0,"(����ҵ������)  ",
	"FINANCE(23)---����(�����ܶ�)      ",0,"(�����ܶ�)      ",
	"FINANCE(24)---����(Ͷ������)      ",0,"(Ͷ������)      ",
	"FINANCE(25)---����(��������)      ",0,"(��������)      ",
	"FINANCE(26)---����(Ӫҵ����֧)    ",0,"(Ӫҵ����֧)    ",
	"FINANCE(27)---����(�����������)  ",0,"(�����������)  ",
	"FINANCE(28)---����(Ͷ������)      ",0,"(Ͷ������)      ",
	"FINANCE(29)---����(�����ܶ�)      ",0,"(�����ܶ�)      ",
	"FINANCE(30)---����(˰������)      ",0,"(˰������)      ",
	"FINANCE(31)---����(������)        ",0,"(������)        ",
	"FINANCE(32)---����(δ��������)    ",0,"(δ��������)    ",
	"FINANCE(33)---����(ÿ��δ��������(Ԫ))",0,"ÿ��δ��������(Ԫ)",
	"FINANCE(34)---����(ÿ������(Ԫ))",0,"ÿ������(Ԫ)",
	"FINANCE(35)---����(ÿ�ɾ��ʲ�(Ԫ))",0,"ÿ�ɾ��ʲ�(Ԫ)",
	"FINANCE(36)---����(�ɶ�Ȩ�����(%))",0,"�ɶ�Ȩ�����(%)",
	"FINANCE(37)---����(���ʲ�������(%))",0,"���ʲ�������(%)",

	"��ͼ����",-1,"",//14
	"BUYCASE---��������",0,"��������",
	"COLORSTICK---��ɫ��״��",0,"��ɫ��״��",
	"CROSSDOT---Ȧ��ͼ",0,"Ȧ��ͼ",
	"CIRCLEDOT---Բ��ͼ",0,"Բ��ͼ",
	"LINESTICK---��״����״����",0,"��״����״����",
	"SELLCASE---��������",0,"��������",
	"STICK---��ͨ��״��",0,"��ͨ��״��",
	"VOLSTICK---�ɽ�����״��",0,"�ɽ�����״��",

	"DRAWICON(CON,PRICE,N)---������CON����ʱ���ڼ۸�PRICE������N��λͼ",0,"��ͼ���ϻ���Сͼ�ꡣ\r\n�÷�:\r\nDRAWICON(COND,PRICE,TYPE),��COND��������ʱ,��PRICEλ�û�TYPE��ͼ�ꡣ\r\n���磺DRAWICON(CLOSE>OPEN,LOW,1)��ʾ������ʱ����ͼ�λ�û�1��ͼ�ꡣ",
	"DRAWLINE(CON1,PRICE1,CON2,PRICE2,N)---������CON1��CON2����ʱ�����۸�PRICE1��PRICE2���������",0,"��ͼ���ϻ���ֱ�߶Ρ�\r\n�÷�:\r\nDRAWLINE(COND1,PRICE1,COND2,PRICE2,EXPAND),��COND1��������ʱ,��PRICE1λ�û�ֱ����㣬��COND2��������ʱ,��PRICE2λ�û�ֱ���յ㣬EXPANDΪ�ӳ����͡�\r\n���磺DRAWILINE(HIGH>=HHV(HIGH,20),HIGH,LOW<=LLV(LOW,20),LOW,1)��ʾ�ڴ�20���¸��봴20���µ�֮�仭ֱ�߲��������ӳ���",
	"DRAWTEXT(CON,PRICE,TEXT)---������CON����ʱ���ڼ۸�PRICE�����ı�TEXT",0,"��ͼ������ʾ���֡�\r\n�÷�:\r\nDRAWTEXT(COND,PRICE,TEXT),��COND��������ʱ,��PRICEλ����д����TEXT��\r\n���磺DRAWTEXT(CLOSE/OPEN>1.08,LOW,'������')��ʾ�����Ƿ�����8%ʱ����ͼ�λ����ʾ'������'������",
	"POLYLINE(CON,PRICE)---��CON��������ʱ,��PRICEλ��Ϊ���㻭��������",0,"���磺POLYILINE(HIGH>=HHV(HIGH,20),HIGH)��ʾ�ڴ�20���¸ߵ�֮�仭���ߡ�",
	"STICKLINE(COND,PRICE1,PRICE2,WIDTH,EMPTY)---��COND��������ʱ,��PRICE1��PRICE2λ��֮�仭��״�ߣ����ΪWIDTH(10Ϊ��׼���),EMPTH��Ϊ0�򻭿�����",0,"��ͼ���ϻ������ߡ�\r\n�÷�:\r\nSTICKLINE(COND,PRICE1,PRICE2,WIDTH,EMPTY),��COND��������ʱ,��PRICE1��PRICE2λ��֮�仭��״�ߣ����ΪWIDTH(10Ϊ��׼���),EMPTH��Ϊ0�򻭿�������\r\n���磺STICKLINE(CLOSE>OPEN,CLOSE,OPEN,0.8,1)��ʾ��K�������ߵĿ������岿�֡�",

	//new
	"��ɫ����",-1,"",//9
	"COLORRED---��ɫ",0,"�� vol,colorred ��ʾ�ú�ɫ���ɽ���",
	"COLORGREEN---��ɫ",0,"��ɫ",
	"COLORBLUE---��ɫ",0,"��ɫ",
	"COLORYELLOW---��ɫ",0,"��ɫ",
	"COLORORANGE---��ɫ",0,"��ɫ",
	"COLORPURPLE---��ɫ",0,"��ɫ",
	"COLORWHITE---��ɫ",0,"��ɫ",
	"COLORBLACK---��ɫ",0,"��ɫ",

	//new
	"�ֲ�չ������",-1,"",//4  = EXTDATA
	"DEPLOYREF(VOL,LOW,HIGH,N)---������괦��ʱ���ǰN���ڵģ���VOL���ȷֲ���LOW��HIGH�۸�֮�������",0,"��VOL���ȷֲ���LOW��HIGH�۸�֮�������",
	"DEPLOYSUM(VOL,N)---������괦��ʱ���ǰN�����ڵģ�VOL�ľ��ȷֲ����ݵ��ۼӺ�",0,"VOL�ľ��ȷֲ����ݵ��ۼӺ�",
	"DEPLOYWIN(VOL,N,LTP)---������괦��ʱ���ǰN�����ڵġ���LTPΪ��ͨ�̵ġ���ֵVOL���ƶ��ɱ��ֲ�����",0,"������괦��ʱ���ǰN�����ڵġ���LTPΪ��ͨ�̵ġ���ֵVOL���ƶ��ɱ��ֲ�����",

	};
int nItemCountText = 192+35;
BOOL CDialogEDITZBGSHS::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
//	CMDIChildWnd* pchildfram= ((CMainFrame*)(AfxGetApp()->m_pMainWnd))->MDIGetActive();
    CTaiShanDoc*	pDoc=CMainFrame::m_pDoc ;//(CTaiShanDoc*)pchildfram->GetActiveDocument();

	CString             stritems[FUNCNUM];
	int                 item;

	//new 
	m_tree.SetImageList(pDoc->m_imagelist,TVSIL_NORMAL);

	int nItem = nItemCountText;
	HTREEITEM hParent = 0;
	if(stritemsArr[0].nFlag  != -1) return TRUE;
	hParent = m_tree.InsertItem("���к���",16,16);
	for(item=0;item<nItem;item++)
	{
		if(stritemsArr[item].nFlag == 0)
		{
			HTREEITEM h1 = m_tree.InsertItem(stritemsArr[item].s,8,8,hParent);
			m_tree.SetItemData (h1,item);
		}
	}
	m_tree.SortChildren(  hParent );

	for(item=0;item<nItem;item++)
	{
		if(stritemsArr[item].nFlag  == -1)
		{
			hParent=m_tree.InsertItem(stritemsArr[item].s ,16,16);
			m_tree.SetItemData (hParent,0);
		}
		else
		{
			HTREEITEM h1 = m_tree.InsertItem(stritemsArr[item].s ,8,8,hParent);
			m_tree.SetItemData (h1,item);
		}
	}
	
	m_ok.EnableWindow(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}



void CDialogEDITZBGSHS::OnOK() 
{
	// TODO: Add extra validation here
	if(	m_selectedstr == "") 
	{
		AfxMessageBox("��ѡ������");
		return ;
	}
	CDialog::OnOK();
}

void CDialogEDITZBGSHS::OnSelchangedTree1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here

	m_selecteditem=m_tree.GetSelectedItem();
	if(m_tree.GetParentItem(m_selecteditem)==NULL)
	{
		if(m_ok.IsWindowEnabled())
			m_ok.EnableWindow(FALSE);
	}
	else
		m_ok.EnableWindow();
	m_selectedstr=m_tree.GetItemText(m_selecteditem);
	int nArr = m_tree.GetItemData (m_selecteditem);
	if(nArr>=0 && nArr<nItemCountText)
	{
		m_sExplain = stritemsArr[nArr].s2;
		UpdateData(FALSE);
	}
	
	*pResult = 0;
}

void CDialogEDITZBGSHS::OnDblclkTree1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	if(m_ok.IsWindowEnabled()) OnOK();
	*pResult = 0;
}

void CDialogEDITZBGSHS::OnReturnTree1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	
	if(m_ok.IsWindowEnabled()) OnOK();
	*pResult = 0;
}

BOOL CDialogEDITZBGSHS::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if(pMsg->hwnd == m_tree.m_hWnd)
	{
		if(pMsg->message==WM_KEYDOWN&&pMsg->wParam==13)
		{
			if(m_ok.IsWindowEnabled()) OnOK();
				return TRUE;
		}
	}
	return CDialog::PreTranslateMessage(pMsg);
}

BOOL CDialogEDITZBGSHS::OnHelpInfo(HELPINFO* pHelpInfo) 
{
	// TODO: Add your message handler code here and/or call default
	DoHtmlHelp(this);return TRUE;
	
//	HtmlHelp(m_hWnd,"stock.chm",HH_HELP_CONTEXT,CDialogEDITZBGSHS::IDD);
//	return CDialog::OnHelpInfo(pHelpInfo);
}
