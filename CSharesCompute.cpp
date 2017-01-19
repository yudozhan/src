// CSharesCompute.cpp: implementation of the CSharesCompute class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include <stdio.h>
#include <share.h>
#include  <io.h>
#include "WH.h"
#include "MainFrm.h"
#include "CTaiShanDoc.h"
#include "cellRange.h"
#include "CTaiShanReportView.h"
#include "StockDrv.h"
#include "StructTaiShares.h"
#include "CSharesCompute.h"
#include "CTaiKlineMemFile.h"
#include "CTaiKlineFileKLine.h"
#include "CTaiKlineFileHS.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CSharesCompute::CSharesCompute()
{

}
CSharesCompute::~CSharesCompute()
{

}
void CSharesCompute::chk_rdp(CReportData  *Cdat,float NewPrice,int iTime)                                                       //����ù�ƱRDPֵ
{
	float fAverage = 0.0;
	if(iTime < 0)
		iTime = 0;
	if(iTime > 239)
		iTime = 239;
	int prev = iTime - 1;
	if (prev < 0)
	{
		prev = 0;
	}
	//fAverage = Cdat->m_Kdata1[0].Price;
	//for (int i = 1;i <= prev;i++)
	//{
	//	fAverage = (fAverage * i + Cdat->m_Kdata1[prev].Price)/(i + 1);
	//}
	fAverage = Cdat->m_Kdata1[prev].Price;
	//fAverage = Cdat->nowp;
	if (NewPrice==0 || (Cdat->psel1==0 && Cdat->pbuy1==0))
		Cdat->rdp[iTime]='9';
	else if (Cdat->psel1==0 && Cdat->pbuy1==0 && NewPrice==fAverage)
		Cdat->rdp[iTime]='0';
	else if (Cdat->psel1==0 && Cdat->pbuy1==0 && NewPrice>fAverage && fAverage>0)
		Cdat->rdp[iTime]='3';
	else if (Cdat->psel1==0 && Cdat->pbuy1==0 && NewPrice<fAverage && NewPrice>0)
		Cdat->rdp[iTime]='4';
	else if (Cdat->psel1==0 && Cdat->pbuy1>0)
		Cdat->rdp[iTime]='1';
	else if (Cdat->pbuy1==0 && Cdat->psel1>0)
		Cdat->rdp[iTime]='2';
	else if (fAverage==0)
		Cdat->rdp[iTime]='9';
	else if (fAverage < NewPrice)
		Cdat->rdp[iTime]='5';
	else if (fAverage > NewPrice)
		Cdat->rdp[iTime]='6';
	else if (Cdat->rdp[iTime]=='9' && Cdat->pbuy1>0 && NewPrice>Cdat->pbuy1)
		Cdat->rdp[iTime]='5';
	else if (Cdat->rdp[iTime]=='9' && Cdat->psel1>0 && NewPrice<Cdat->psel1)
		Cdat->rdp[iTime]='6';
}
	 float Vol;//�ɽ�����
	 float Per;//��ͨ�̰ٷֱ�
	 float Acount;//�ɽ����

void CSharesCompute::SendCuoHeDataToDialog(CReportData *Cdat)
{
	CMainFrame *pMain=(CMainFrame *)(AfxGetApp()->m_pMainWnd);
	char  strBuySell[10];
	if(Cdat->InOut==0)
        strcpy(strBuySell,"������");
	else if(Cdat->InOut==1)
        strcpy(strBuySell,"������");
	else if(Cdat->InOut==2)
        strcpy(strBuySell,"�ڲ����");
    else
		return;
    BOOL IsSend=FALSE;
	if(Cdat->kind==SHAG||Cdat->kind==SHBG||Cdat->kind==SZAG||Cdat->kind==SZBG||Cdat->kind==SZQZ||Cdat->kind==SZZXQY)
	{
		switch(this->m_MainDocument->m_dadanchoice.Format)
		{
		case 0:     if(Cdat->nowv>=this->m_MainDocument->m_dadanchoice.Vol)
					{
                       IsSend=TRUE;
					}
                    break; 
        case 1:     if(Cdat->pBaseInfo!=NULL)
					{
						if(Cdat->kind==SHBG||Cdat->kind==SZBG)  //B
						{
							if(Cdat->pBaseInfo->Bg>0&&(Cdat->nowv>=this->m_MainDocument->m_dadanchoice.Per*Cdat->pBaseInfo->Bg))
							{
                               IsSend=TRUE;
							}
						}
						else                              //A
						{
		                    if(Cdat->pBaseInfo->ltAg>0&&(Cdat->nowv>=this->m_MainDocument->m_dadanchoice.Per*Cdat->pBaseInfo->ltAg))
							{
                               IsSend=TRUE;
							}
						}
					}
			        else
						return;
			        break;
		case 2:     if(Cdat->nowp*Cdat->nowv/100>this->m_MainDocument->m_dadanchoice.Acount)
					 {
                               IsSend=TRUE;
					 }
			         break;
		} 
	}
	if(IsSend)
       pMain->m_wndStatusBar.SetBigBill(Cdat->id, strBuySell, Cdat->nowv, Cdat->nowp,TRUE);

}
void CSharesCompute::UpdateHs(CReportData  *Cdat,  short  Curr_Min)                                      //���¸ù�Ʊÿ�γɽ�����
{
	CTaiKlineFileHS* pHsFile ;
	CString s(Cdat->id );
	pHsFile = CTaiKlineFileHS::GetFilePointer (s,Cdat->kind);//m_pFileHsSz ;
	if (Curr_Min != -1)
	{
		pHsFile->WriteHS (Cdat);
	}
	else
	{
		pHsFile->WriteHS (Cdat,true);
	}
 
}

short CSharesCompute::GetStockMinute(time_t time,int mode )              //��Ʊ�������             
{
  int tmp=0;
  struct tm* osTime;                            // A pointer to a structure containing time 
  struct tm  tmTime;                           
  
  CTime m_Time = time;

  osTime = m_Time.GetLocalTm( &tmTime );
  //osTime = m_Time.GetLocalTm( NULL );Marenan Modify
 
  if (osTime->tm_hour < m_MainDocument->m_nDel_Start_B_hr[mode-1] )
  {   
      tmp=(osTime->tm_hour -m_MainDocument->m_nDel_Start_A_hr[mode-1])*60 + osTime->tm_min  - m_MainDocument->m_nDel_Start_A_min[mode-1] ;
      if (tmp>m_MainDocument->m_nDel_Half_ANT[mode-1])                                 //����Ƿ�Ϊ�������ʱ��
          tmp=m_MainDocument->m_nDel_Half_ANT[mode-1];
  }
  else 
  {                                                                   //�õ����翪�з�����
      tmp=(osTime->tm_hour - m_MainDocument->m_nDel_Start_B_hr[mode-1])*60+osTime->tm_min - m_MainDocument->m_nDel_Start_B_min[mode-1]-1;
      if (tmp<0)
          tmp=m_MainDocument->m_nDel_Half_ANT[mode-1];                                  //����Ƿ�Ϊ�������ʱ��
      else
	  {
          tmp+=m_MainDocument->m_nDel_Half_ANT[mode-1]+1;
          if (tmp>m_MainDocument->m_nMaxANT[mode-1] )                                    //����Ƿ�Ϊ�������ʱ��
             tmp=m_MainDocument->m_nMaxANT[mode-1] ;
	  }
  }
  if(tmp<0)
  {
	  tmp = 0;
//	  ASSERT(FALSE);
  }
  return(tmp);
}
void CSharesCompute::StockDataMin5Update(MIN5_TOTAL_STRUCTEx * m_GpMin5)                  //��Ʊ�������             
{
    int index=0;
	if(m_GpMin5->m_RcvMin5->m_head.m_dwHeadTag != EKE_HEAD_TAG)
		return;
	do
	{
        int  endindex=CheckStockMin5time(m_GpMin5,index) ; 
		if(endindex <= index && index<m_GpMin5->Min5_Count)
		{
			index++;
			continue;
		}
		else if(endindex <= index)
			break;
		int KLineCount=0;
		Kline *pDay=NULL;
        CString StockId;
		int stkKind = -1;
		HGLOBAL	hMem;
		LPVOID hp;
		hMem = GlobalAlloc( GPTR, (endindex - index  )* sizeof(Kline ) );
		hp=GlobalLock(hMem);
		if( hp )
			pDay= (Kline *)hp;
		else
			AfxMessageBox("�����ڴ����",MB_ICONSTOP);
		if(m_GpMin5->m_RcvMin5[index].m_head.m_dwHeadTag == EKE_HEAD_TAG)
		{
		   StockId=m_GpMin5->m_RcvMin5[index].m_head.m_szLabel;
		   stkKind=CSharesInformation::GetStockKind(m_GpMin5->m_RcvMin5[index].m_head.m_wMarket ,m_GpMin5->m_RcvMin5[index].m_head.m_szLabel);
		}

		index++;
		do
		{
			pDay[KLineCount].day =m_GpMin5->m_RcvMin5[index].m_time ;
			pDay[KLineCount].open =m_GpMin5->m_RcvMin5[index].m_fOpen ;
			pDay[KLineCount].high=m_GpMin5->m_RcvMin5[index].m_fHigh  ;
			pDay[KLineCount].low =m_GpMin5->m_RcvMin5[index].m_fLow  ;
			pDay[KLineCount].close =m_GpMin5->m_RcvMin5[index].m_fClose ;
			pDay[KLineCount].vol =m_GpMin5->m_RcvMin5[index].m_fVolume ;
			pDay[KLineCount].amount =m_GpMin5->m_RcvMin5[index].m_fAmount  ;
	        pDay[KLineCount].advance =m_GpMin5->m_RcvMin5[index].m_wAdvance ;
		    pDay[KLineCount].decline =m_GpMin5->m_RcvMin5[index].m_wDecline ;
			KLineCount++;
			index++;
		}while(m_GpMin5->m_RcvMin5[index].m_head.m_dwHeadTag != EKE_HEAD_TAG && index<m_GpMin5->Min5_Count);

	   CTaiKlineFileKLine::GetFilePointer(StockId,stkKind,false)->WriteKLine(StockId,pDay,KLineCount,0);   //����K������

	    GlobalUnlock((HGLOBAL)pDay);        //�ͷ��ڴ�
		GlobalFree( (HGLOBAL)pDay);
		if(CMainFrame::m_pDoc->m_sharesSymbol == StockId)
		{
			CMainFrame::m_pDoc->UpdateAllViews(NULL);
		}
		ProcMessage();
	}while(index<m_GpMin5->Min5_Count);
}
void CSharesCompute::StockDataDayUpdate(DAY_TOTAL_STRUCTEx * m_GpDay)                  //��Ʊ�������             
{
    int index=0;
	if(m_GpDay->m_RcvDay->m_head.m_dwHeadTag != EKE_HEAD_TAG)
		return;
	do
	{
        int  endindex=CheckStockDaytime(m_GpDay,index) ; 
		if(endindex <= index&&index<m_GpDay->Day_Count)
		{
			index++;
			continue;
		}
		else if(endindex <= index)
			break;
		int KLineCount=0;
		Kline *pDay=NULL;
        CString StockId;
		int stkKind = -1;
		HGLOBAL	hMem;
		LPVOID hp;
		hMem = GlobalAlloc( GPTR, (endindex - index  )* sizeof(Kline ) );
		hp=GlobalLock(hMem);
		if( hp )
			pDay= (Kline *)hp;
		else
			AfxMessageBox("�����ڴ����",MB_ICONSTOP);
		if(m_GpDay->m_RcvDay[index].m_head.m_dwHeadTag == EKE_HEAD_TAG)
		{
		   StockId=m_GpDay->m_RcvDay[index].m_head.m_szLabel;
		   stkKind=CSharesInformation::GetStockKind(m_GpDay->m_RcvDay[index].m_head.m_wMarket ,m_GpDay->m_RcvDay[index].m_head.m_szLabel);
		}

		index++;
		do
		{
			pDay[KLineCount].day =m_GpDay->m_RcvDay[index].m_time ;
			pDay[KLineCount].open =m_GpDay->m_RcvDay[index].m_fOpen ;
			pDay[KLineCount].high=m_GpDay->m_RcvDay[index].m_fHigh  ;
			pDay[KLineCount].low =m_GpDay->m_RcvDay[index].m_fLow  ;
			pDay[KLineCount].close =m_GpDay->m_RcvDay[index].m_fClose ;
			pDay[KLineCount].vol =m_GpDay->m_RcvDay[index].m_fVolume ;
			pDay[KLineCount].amount =m_GpDay->m_RcvDay[index].m_fAmount  ;
	        pDay[KLineCount].advance =m_GpDay->m_RcvDay[index].m_wAdvance ;
		    pDay[KLineCount].decline =m_GpDay->m_RcvDay[index].m_wDecline ;
			KLineCount++;
			index++;
		}while(m_GpDay->m_RcvDay[index].m_head.m_dwHeadTag != EKE_HEAD_TAG && index<m_GpDay->Day_Count);

	   CTaiKlineFileKLine::GetFilePointer(StockId,stkKind,true)->WriteKLine(StockId,pDay,KLineCount,0);   //����K������

	    GlobalUnlock((HGLOBAL)pDay);        //�ͷ��ڴ�
		GlobalFree( (HGLOBAL)pDay);
		if(CMainFrame::m_pDoc->m_sharesSymbol == StockId)
		{
			CMainFrame::m_pDoc->UpdateAllViews(NULL);
		}
		ProcMessage();

	}while(index<m_GpDay->Day_Count);
}
int CSharesCompute::CheckStockMin5time(MIN5_TOTAL_STRUCTEx * m_GpMin5,int index)              //��Ʊ�������             
{
    int ls_pos=index;
	do
	{
		ls_pos++;
	}while(m_GpMin5->m_RcvMin5[ls_pos].m_head.m_dwHeadTag != EKE_HEAD_TAG && ls_pos<m_GpMin5->Min5_Count);
	ls_pos--;
	return ls_pos;  
}

int CSharesCompute::CheckStockDaytime(DAY_TOTAL_STRUCTEx * m_GpDay,int index)              //��Ʊ�������             
{
    int ls_pos=index;
	do
	{
		ls_pos++;
	}while(m_GpDay->m_RcvDay[ls_pos].m_head.m_dwHeadTag != EKE_HEAD_TAG && ls_pos<m_GpDay->Day_Count);
	ls_pos--;
	return ls_pos;  
}
void CSharesCompute::StockDataPowerUpdate(POWER_TOTAL_STRUCTEx * m_GpPower)                  //��Ʊ�������             
{
	int index =0;
	static int steps=0;
	int nKind=0;
	if(m_GpPower->m_RcvPower->m_head.m_dwHeadTag != EKE_HEAD_TAG)
		return;
	do
	{
		CReportData * Cdat;
		Cdat=NULL;
		CString StockId;
		if( m_GpPower->m_RcvPower[index].m_head .m_dwHeadTag == EKE_HEAD_TAG )   //�ж��Ƿ�Ϊ��������ͷ��Ϣ
		{
			StockId=m_GpPower->m_RcvPower[index].m_head .m_szLabel ;          //ȡ֤ȯ������Ϣ
            StockId.MakeUpper(); 
			nKind=m_MainDocument->m_sharesInformation.GetStockKind(m_GpPower->m_RcvPower[index].m_head.m_wMarket,m_GpPower->m_RcvPower[index].m_head.m_szLabel);
			if(m_MainDocument->m_sharesInformation.Lookup(StockId.GetBuffer(0) , Cdat,nKind) != TRUE)       //���Ҳ������߽ṹ
			{
				Cdat=NULL;
				index++;
				continue;
			}
		}
		if(Cdat==NULL)                        //�ж�Cdat �Ƿ�Ϊ��,��Ϊ�չ�Ʊ��������û���ҵ�
		{
			index++;
			continue;
		}
		index++;
		if(Cdat->pBaseInfo==NULL)
		{
			 //if(Cdat->pBaseInfo==NULL)
			 {
				BASEINFO *pBaseItem;
				if(!m_MainDocument->m_sharesInformation.AddBaseinfoPoint(Cdat->id,Cdat->kind,pBaseItem))              //�ڻ������������ļ�������һ����������
				{
					AfxMessageBox("���ӹ�Ʊ�������Ͽռ�����ʱ����");
					return ;
				}
				Cdat->pBaseInfo=pBaseItem;
			 }
		}
		else
			 Cdat->pBaseInfo->NumSplit=0;

		do
		{
            Cdat->pBaseInfo->m_Split[Cdat->pBaseInfo->NumSplit].nTime= m_GpPower->m_RcvPower[index].m_time;
            Cdat->pBaseInfo->m_Split[Cdat->pBaseInfo->NumSplit].Give= m_GpPower->m_RcvPower[index].m_fGive;
            Cdat->pBaseInfo->m_Split[Cdat->pBaseInfo->NumSplit].Allocate= m_GpPower->m_RcvPower[index].m_fPei;
            Cdat->pBaseInfo->m_Split[Cdat->pBaseInfo->NumSplit].AllocatePrice= m_GpPower->m_RcvPower[index].m_fPeiPrice;
            Cdat->pBaseInfo->m_Split[Cdat->pBaseInfo->NumSplit].Bonus= m_GpPower->m_RcvPower[index].m_fProfit;
            Cdat->pBaseInfo->NumSplit++;
			index++;
		}while(m_GpPower->m_RcvPower[index].m_head.m_dwHeadTag != EKE_HEAD_TAG && index<m_GpPower->Power_Count);

		ProcMessage();

	}while(index<m_GpPower->Power_Count);
}

//����ֵ��SH_MARKET_EXΪ��֤��SZ_MARKET_EXΪ��֤��HK_MARKET_EXΪ��ۣ�EB_MARKET_EXΪ2��
int CSharesCompute::GetMarketKind(int nKind)
{
	if(	nKind==SHZS||nKind==SHAG||nKind==SHBG||nKind==SHZQ||
		nKind==SHJJ||nKind==SHZZ||nKind==SHHG||nKind==SHETF||
		nKind==SHKF||nKind==SHQZ||nKind==SHOT)
	  return SH_MARKET_EX;
	else if(nKind==SZZS||nKind==SZAG||nKind==SZBG||
		nKind==SZZQ||nKind==SZJJ||nKind==SZZZ||
		nKind==SZHG||nKind==SZETF||nKind==SZKF||
		nKind==SZQZ||nKind==SZSB||nKind==SZOT||nKind==SZZXQY||nKind==SZCYB)
	  return SZ_MARKET_EX;
	else 
      return EB_MARKET_EX;
}

//nKind��0Ϊ��֤��1Ϊ��֤��2Ϊ��ۣ�3Ϊ2��
CString CSharesCompute::GetIndexSymbol(int nKind)
{
	//lmb11
	if (nKind < 0 || nKind > 2)
	{
		return "";
	}
	static CString s[4]={"1A0001","399001","399001","100001"};
	static bool bFirst = true;
	if(bFirst == true)
	{
		CFile fl;
		if(fl.Open ("IndexSymbolTxt.txt",CFile::modeRead))
		{
			int n = fl.GetLength ();
			char * pch = new char[n+1];
			fl.Read (pch,n);
			pch[n] = 0;
			CString ss = pch;
			delete[] pch;
			ss.TrimLeft (" ");
			int nStart = 0;
			int nNext=0;
			int i = 0;
			while(i<4)
			{
				nNext = ss.Find (" ",0);
				if(nNext<0) break;

				CString s2 = ss.Left (nNext);
				s2.TrimRight (" ");
				s[i] = s2;
				i++;

				ss = ss.Right (ss.GetLength ()-s2.GetLength ());
				ss.TrimLeft (" ");
			}

		}
		bFirst = false;
	}
	
	if(nKind > 0)
	{
		int stkKind[4] = {SHZS,SZZS,SZZS,SZZS};
		CReportData * pdt;
		if(CMainFrame::m_pDoc->m_sharesInformation.GetStockItem(stkKind[nKind],0,pdt))//.Lookup(s[nKind].GetBuffer (0),pdt,stkKind[nKind] )==0)
		{
			if(pdt) 
				s[nKind] = pdt->id;
		}
	}
	return s[nKind];
}
#ifdef WIDE_NET_VERSION
void CSharesCompute::WideStockDataUpdate(RCV_WIDOFEREPORT_STRUCTEx* m_WideGpHqReport)                           //���¼�ʱ��������             
{
	CString m_Zqdm;
    CReportData  *Cdat;
	RCV_REPORT_STRUCTEx *m_GpHqReport=m_WideGpHqReport->pMarketReport;
	if(strstr(m_GpHqReport->m_szLabel," "))
		return;
	if(strlen(m_GpHqReport->m_szLabel)!=6&&strlen(m_GpHqReport->m_szLabel)!=4)
		return;
    m_GpHqReport->m_szName[8]='\0';
	int nKind=m_MainDocument->m_sharesInformation.GetStockKind(m_GpHqReport->m_wMarket,m_GpHqReport->m_szLabel);
    if(nKind==-1)
		return;
	if (this->m_MainDocument->m_sharesInformation.Lookup(m_GpHqReport->m_szLabel, Cdat,nKind) != TRUE)     //���ù�Ʊ�Ƿ��Ѽ�¼���ڴ�ģ����
	{
         return; 
	} 
	// add
    if(Cdat->lastclmin<0 )
       Cdat->lastclmin=0;
	if(Cdat->lastclmin>240)
       Cdat->lastclmin=239;
    WideStockItemUpdate(Cdat, m_WideGpHqReport);                                     //���ڴ����Ʊÿһ��Ʊ����
}
void CSharesCompute::WideStockItemUpdate(CReportData  *Cdat,RCV_WIDOFEREPORT_STRUCTEx * m_WideGpHqReport)             //���ڼ���ÿһ��Ʊ����
{
    int chg=0;
	short Select_Market;
	float m_Stockvalue;
	float OldBuyP1,OldSellP1;
	RCV_REPORT_STRUCTEx *m_GpHqReport=m_WideGpHqReport->pMarketReport;
	if(strcmp(Cdat->id,m_GpHqReport->m_szLabel)!=0)
		return;
    OldBuyP1=Cdat->pbuy1;
	OldSellP1=Cdat->psel1;
    CString StockId=m_GpHqReport->m_szLabel;
	if(m_GpHqReport->m_wMarket == SH_MARKET_EX)
       Select_Market=0;                                        //ѡ����֤��Ʊ
	else
	   Select_Market=1;  		//ѡ�����ڹ�Ʊ      

	Cdat->initdown =TRUE;                                   //���ø�֤ȯ�Ѵ���
	if(m_GpHqReport->m_fLastClose !=0&& m_GpHqReport->m_fLastClose!=-1)
	   Cdat->ystc=m_GpHqReport->m_fLastClose;               //�õ������� 

	bool bBuySell = false;
	if(Cdat ->ystc>0 && m_GpHqReport->m_fNewPrice>0)
	{
		float ff =( m_GpHqReport->m_fNewPrice - Cdat ->ystc )/Cdat ->ystc;
		if(ff>0.095 || ff<-0.095)//��ͣ��
			bBuySell = true;
		if(Cdat->id[0] == 'S' && Cdat->id[1] == 'T')
			if(ff>0.047 || ff<-0.047)//��ͣ��
				bBuySell = true;
	}
	//��������

	if ((m_GpHqReport->m_fBuyPrice[0]>0 || bBuySell == true) && m_GpHqReport->m_fBuyPrice[0]!=-1)
	{
		if (m_GpHqReport->m_fBuyPrice[0]!=Cdat->pbuy1)                    //��������۷����仯
		{
			if (m_MainDocument->m_nANT[Select_Market]<0)
				m_MainDocument->m_nANT[Select_Market]=0;
			Cdat->pbuy1 =m_GpHqReport->m_fBuyPrice[0];                     //�õ���������� 
			chg=1;                                        //��������۷����仯                                                       
		}
	}
	//�õ����������� 
	if ((m_GpHqReport->m_fSellPrice[0] >0 || bBuySell == true) && m_GpHqReport->m_fSellPrice[0]!=-1)
	{
		if (m_GpHqReport->m_fSellPrice[0]!=Cdat->psel1)
		{
			if (m_MainDocument->m_nANT[Select_Market]<0)
				m_MainDocument->m_nANT[Select_Market]=0;
			Cdat->psel1=m_GpHqReport->m_fSellPrice[0];                      //�õ����������� 
			chg=1;                                        //��������������۷����仯������CHG=1 
		}
	}
	if (m_GpHqReport->m_fHigh!=0&&m_GpHqReport->m_fHigh!=-1)                        //�õ���߼� 
		Cdat->higp=m_GpHqReport->m_fHigh;          
	
	if (m_GpHqReport->m_fLow!=0&&m_GpHqReport->m_fLow!=-1)
		Cdat->lowp=m_GpHqReport->m_fLow;                 //�õ���ͼ�
	
	if (m_GpHqReport->m_fOpen!=0&&m_GpHqReport->m_fOpen!=-1)
		Cdat->opnp=m_GpHqReport->m_fOpen;                //�õ����տ��̼�

	if (m_GpHqReport->m_fNewPrice!=0&&m_GpHqReport->m_fNewPrice!=-1)                    //�õ����¼� 
	{
		CTime tm1(m_GpHqReport->m_time);
		CTime tm0(tm1.GetYear(),tm1.GetMonth(),tm1.GetDay(),9,30,0,0);
		CTimeSpan tmSpan(tm1 - tm0);
		chk_rdp(Cdat,m_GpHqReport->m_fNewPrice,tmSpan.GetMinutes());
		//chk_rdp(Cdat,m_GpHqReport->m_fNewPrice,m_GpHqReport->m_time);
		if (m_GpHqReport->m_fNewPrice!=Cdat->nowp)                    //�Ƚ�����һ�����¼�
		{
			if (m_MainDocument->m_nANT[Select_Market]<0)
				m_MainDocument->m_nANT[Select_Market]=0;
			Cdat->nowp=m_GpHqReport->m_fNewPrice;                     //�õ����¼�
			chg=2;                                       //���¼۱仯ʱ������chg��Ϊ2
		}
	}
	bool bValid = false;
	for(int k = 0;k<3;k++)
	{
		if(m_GpHqReport->m_fBuyVolume[k]!=0) 
		{bValid = true;break;}
		if(m_GpHqReport->m_fSellVolume[k]!=0) 
		{bValid = true;break;}
	}

	if((m_GpHqReport->m_fBuyVolume[0]>0 || bValid == true) &&m_GpHqReport->m_fBuyVolume[0]!=-1 )          //�õ���һ��
	   Cdat->vbuy1=m_GpHqReport->m_fBuyVolume[0];

	if((m_GpHqReport->m_fBuyPrice[1]>0.01 ||m_GpHqReport->m_fBuyPrice[1] == 0 && bBuySell) &&m_GpHqReport->m_fBuyPrice[1] !=-1)           //�õ������
	   Cdat->pbuy2=m_GpHqReport->m_fBuyPrice[1];          

	if((m_GpHqReport->m_fBuyVolume[1]>0 || bValid == true)&&m_GpHqReport->m_fBuyVolume[1] !=-1)          //�õ������
	   Cdat->vbuy2=m_GpHqReport->m_fBuyVolume[1];

	if((m_GpHqReport->m_fBuyPrice[2]>0.01||m_GpHqReport->m_fBuyPrice[2] == 0 && bBuySell)  &&m_GpHqReport->m_fBuyPrice[2]!=-1)         //�õ�������
	   Cdat->pbuy3=m_GpHqReport->m_fBuyPrice[2];

	if((m_GpHqReport->m_fBuyVolume[2]>0 || bValid == true) && m_GpHqReport->m_fBuyVolume[2]!=-1)         //�õ�������
	   Cdat->vbuy3=m_GpHqReport->m_fBuyVolume[2];

	if((m_GpHqReport->m_fBuyPrice4>0.01||m_GpHqReport->m_fBuyPrice4 == 0 && bBuySell)  &&m_GpHqReport->m_fBuyPrice4!=-1)         //�õ�������
	   Cdat->pbuy4=m_GpHqReport->m_fBuyPrice4;

	if((m_GpHqReport->m_fBuyVolume4>0 || bValid == true) && m_GpHqReport->m_fBuyVolume4!=-1)         //�õ�������
	   Cdat->vbuy4=m_GpHqReport->m_fBuyVolume4;

	if((m_GpHqReport->m_fBuyPrice5>0.01||m_GpHqReport->m_fBuyPrice5 == 0 && bBuySell)  &&m_GpHqReport->m_fBuyPrice5!=-1)         //�õ�������
	   Cdat->pbuy5=m_GpHqReport->m_fBuyPrice5;

	if((m_GpHqReport->m_fBuyVolume5>0 || bValid == true) && m_GpHqReport->m_fBuyVolume5!=-1)         //�õ�������
	   Cdat->vbuy5=m_GpHqReport->m_fBuyVolume5;

	if((m_GpHqReport->m_fSellVolume[0]>0 || bValid == true)&& m_GpHqReport->m_fBuyVolume[0]!=-1)      //�õ���һ��
	   Cdat->vsel1=m_GpHqReport->m_fSellVolume[0];

	if((m_GpHqReport->m_fSellPrice[1]>0.001||m_GpHqReport->m_fSellPrice[1] == 0 && bBuySell)   &&m_GpHqReport->m_fSellPrice[1]!=-1)       //�õ�������
	   Cdat->psel2=m_GpHqReport->m_fSellPrice[1];          

	if((m_GpHqReport->m_fSellVolume[1]>0 || bValid == true)&&m_GpHqReport->m_fSellVolume[1]!=-1)      //�õ�������
	   Cdat->vsel2=m_GpHqReport->m_fSellVolume[1];

	if((m_GpHqReport->m_fSellPrice[2]>0.001||m_GpHqReport->m_fSellPrice[1] == 0 && bBuySell) &&m_GpHqReport->m_fSellPrice[2]!=-1)       //�õ�������
	   Cdat->psel3=m_GpHqReport->m_fSellPrice[2];

	if((m_GpHqReport->m_fSellVolume[2]>0 || bValid == true) &&m_GpHqReport->m_fSellVolume[2]!=-1)        //�õ�������
	  Cdat->vsel3=m_GpHqReport->m_fSellVolume[2];

	if((m_GpHqReport->m_fSellPrice4>0.001||m_GpHqReport->m_fSellPrice[1] == 0 && bBuySell) &&m_GpHqReport->m_fSellPrice4!=-1)       //�õ�������
	   Cdat->psel4=m_GpHqReport->m_fSellPrice4;

	if((m_GpHqReport->m_fSellVolume4>0 || bValid == true) &&m_GpHqReport->m_fSellVolume4!=-1)        //�õ�������
	  Cdat->vsel4=m_GpHqReport->m_fSellVolume4;

	if((m_GpHqReport->m_fSellPrice5>0.001||m_GpHqReport->m_fSellPrice[1] == 0 && bBuySell) &&m_GpHqReport->m_fSellPrice5!=-1)       //�õ�������
	   Cdat->psel5=m_GpHqReport->m_fSellPrice5;

	if((m_GpHqReport->m_fSellVolume5>0 || bValid == true) &&m_GpHqReport->m_fSellVolume5!=-1)        //�õ�������
	  Cdat->vsel5=m_GpHqReport->m_fSellVolume5;

	m_Stockvalue=m_GpHqReport->m_fVolume  ;                     //�õ���Ʊ�ɽ���
	if (m_GpHqReport->m_fVolume>0)
	{
		if(m_Stockvalue==0 && Cdat->totv>0 && m_MainDocument->m_nANT[0]>0)
		  m_Stockvalue=Cdat->totv;                        
		if (m_Stockvalue!=Cdat->totv)
		{
			if (m_MainDocument->m_nANT[Select_Market]<0)
				m_MainDocument->m_nANT[Select_Market]=0;
			if (m_Stockvalue>Cdat->totv)                                        //��Ʊ�������仯
			{
				Cdat->nowv=m_Stockvalue - Cdat->totv;                           //�õ���ʱ�ɽ���
				chg=3;                                                  //�����������仯 
			}
		}
	}
	if(m_Stockvalue!=0)
	   Cdat->totv=m_Stockvalue;                                                //�����Ʊ�ܳɽ���  
	if (m_GpHqReport->m_fAmount!=0&&m_GpHqReport->m_fAmount!=-1)                          //�õ���Ʊ�ɽ����
	   Cdat->totp =m_GpHqReport->m_fAmount;

	Cdat->rvol=m_WideGpHqReport->m_fActiveBuyVolumn;
	Cdat->dvol=Cdat->totv - Cdat->rvol;
	
	if (Cdat->nowv<0)
		Cdat->nowv=0;
}
void CSharesCompute::WideStockDataMinUpdate(MIN_TOTAL_STRUCTEx * m_GpMinute)                  //��Ʊ�������             
{
	int index =0;
	static int steps=0;
	if(m_GpMinute->m_RcvMinute->m_head.m_dwHeadTag != EKE_HEAD_TAG)
		return;
	do
	{
		CReportData * Cdat;
		Cdat=NULL;
		CString StockId;
		if( m_GpMinute->m_RcvMinute[index].m_head .m_dwHeadTag == EKE_HEAD_TAG )   //�ж��Ƿ�Ϊ��������ͷ��Ϣ
		{

			StockId=m_GpMinute->m_RcvMinute[index].m_head.m_szLabel ;          //ȡ֤ȯ������Ϣ
            StockId.MakeUpper(); 
			int nKind=m_MainDocument->m_sharesInformation.GetStockKind(m_GpMinute->m_RcvMinute[index].m_head.m_wMarket,m_GpMinute->m_RcvMinute[index].m_head.m_szLabel);
			if(m_MainDocument->m_sharesInformation.Lookup(StockId.GetBuffer(0) , Cdat,nKind) != TRUE)       //���Ҳ������߽ṹ
			{
				Cdat=NULL;
				index++;
				continue;
			}
		}
		if(Cdat==NULL)                        //�ж�Cdat �Ƿ�Ϊ��,��Ϊ�չ�Ʊ��������û���ҵ�
		{
			index++;
			continue;
		}
		index++;
		int mode=0;
		if(m_GpMinute->m_RcvMinute[index].m_time<0)           //��UTCС��0 �����ݲ���ȷ
		{
			Cdat=NULL;
			index++;
			continue;
		}
		if(m_GpMinute->m_RcvMinute[index].m_head.m_wMarket ==SZ_MARKET_EX)    //�ζ��Ƿ�Ϊ���ڹ�Ʊ
			mode=1;
        short MinuteCount=0;
		do
		{
			if(m_GpMinute->m_RcvMinute[index].m_time>240)
			{
			   MinuteCount=GetStockMinute(m_GpMinute->m_RcvMinute[index].m_time,mode+1   ); //���������
			}
			else
			{
			   MinuteCount=m_GpMinute->m_RcvMinute[index].m_time;
			}
			if(MinuteCount<=m_MainDocument->m_nANT[0])
			{
                if(m_GpMinute->m_RcvMinute[index].m_fPrice!=0)
				{
					Cdat->m_Kdata1[MinuteCount].Price   =m_GpMinute->m_RcvMinute[index].m_fPrice ;
					Cdat->m_Kdata1[MinuteCount].Amount  =m_GpMinute->m_RcvMinute[index].m_fAmount;
					Cdat->m_Kdata1[MinuteCount].Volume  =m_GpMinute->m_RcvMinute[index].m_fVolume;
				}
			}
			index++;
		}while(m_GpMinute->m_RcvMinute[index].m_head.m_dwHeadTag != EKE_HEAD_TAG && index<m_GpMinute->Min_Count&&MinuteCount < 240);
		Cdat->lastclmin =MinuteCount ;
		Cdat->initdown =TRUE;
		Cdat=NULL;
	}while(index<m_GpMinute->Min_Count);
}
#else
void CSharesCompute::StockDataUpdate(RCV_REPORT_STRUCTEx* m_GpHqReport)
{
	CString m_Zqdm;
    CReportData  *Cdat = NULL;
	if(strstr(m_GpHqReport->m_szLabel," "))
		return;
	if(strlen(m_GpHqReport->m_szLabel)!=6&&strlen(m_GpHqReport->m_szLabel)!=4)
		return;


    m_GpHqReport->m_szName[8]='\0';
	int nKind=m_MainDocument->m_sharesInformation.GetStockKind(m_GpHqReport->m_wMarket,m_GpHqReport->m_szLabel);
    if(nKind==-1)
		return;
	if (this->m_MainDocument->m_sharesInformation.Lookup(m_GpHqReport->m_szLabel, Cdat,nKind) != TRUE)     //���ù�Ʊ�Ƿ��Ѽ�¼���ڴ�ģ����
	{
        if(!m_MainDocument->m_sharesInformation.InsertItem(m_GpHqReport->m_szLabel,Cdat,nKind))
			return;
		if(Cdat == NULL)
			return;
		m_MainDocument->m_pReportView->LoadShowData(nKind);              //��CGRID������һ�� 
		strcpy(Cdat->name ,m_GpHqReport->m_szName );
		strcpy(Cdat->id ,m_GpHqReport->m_szLabel );
		Cdat->kind=nKind;
		if(strlen(Cdat->name)>0)
		{
            m_MainDocument->StockNameConvert(Cdat->name,Cdat->Gppyjc) ;
			m_MainDocument->AddStockToKeyboard(Cdat->id,Cdat->kind,Cdat->name,Cdat->Gppyjc);
		}
		m_MainDocument->m_pReportView->AddGridRow(nKind);              //��CGRID������һ�� 

		STOCKINFOINBLOCK *m_pStock;
	    if(m_MainDocument->m_ManagerBlockData.Lookup(Cdat->id,Cdat->kind,m_pStock))
		{
           Cdat->pBlockInfo= m_pStock;
		}
		BASEINFO *m_pStockBase;
	    if(m_MainDocument->m_sharesInformation.LookupBase(Cdat->id,nKind,m_pStockBase))
		{
           Cdat->pBaseInfo=m_pStockBase;
		}
	} 
	m_Zqdm=Cdat->id;
	// add
	if(Cdat->ystc!=0)
	{
        if(fabs(m_GpHqReport->m_fNewPrice)<0.01 )
		{
			if(strlen(m_GpHqReport->m_szName)>0&&strcmp(Cdat->name,m_GpHqReport->m_szName)!=0 )
			{
			   m_MainDocument->StockNameConvert(m_GpHqReport->m_szName,Cdat->Gppyjc) ;
			   strcpy(Cdat->name , m_GpHqReport->m_szName );
			   Cdat->opnp=m_GpHqReport->m_fOpen;
			   m_MainDocument->AddStockToKeyboard(Cdat->id,Cdat->kind,Cdat->name,Cdat->Gppyjc);
			}
			return;
		}
	}
    if(Cdat->lastclmin<0 )
       Cdat->lastclmin=0;
	if(Cdat->lastclmin>240)
       Cdat->lastclmin=239;
    StockItemUpdate(Cdat, m_GpHqReport);   //���ڴ����Ʊÿһ��Ʊ����
}
void CSharesCompute::StockItemUpdate(CReportData  *Cdat,RCV_REPORT_STRUCTEx* m_GpHqReport)             //���ڼ���ÿһ��Ʊ����
{
 
	int chg=0;
	short Select_Market;
	float m_Stockvalue,ltmp;
	float OldBuyP1,OldSellP1;
	if(strcmp(Cdat->id,m_GpHqReport->m_szLabel)!=0)
		return;
    OldBuyP1=Cdat->pbuy1;
	OldSellP1=Cdat->psel1;
    CString StockId=m_GpHqReport->m_szLabel;
    if(m_MainDocument->m_pStockBigBillData&&m_MainDocument->m_pStockBigBillData->m_pBigBillHead->Done)
	{
       if(Cdat->IsMxTj)
	   { 
           m_MainDocument->m_pStockBigBillData->CalcBigBillData(Cdat, m_GpHqReport);
	   }
	}
	if(strlen(m_GpHqReport->m_szName)>0&&strcmp(Cdat->name,m_GpHqReport->m_szName)!=0 )
	{
       m_MainDocument->StockNameConvert(m_GpHqReport->m_szName,Cdat->Gppyjc) ;
	   strcpy(Cdat->name , m_GpHqReport->m_szName );
       m_MainDocument->AddStockToKeyboard(Cdat->id,Cdat->kind,Cdat->name,Cdat->Gppyjc);
	}


	//
	if(m_GpHqReport->m_wMarket == SH_MARKET_EX)
       Select_Market=0;         //ѡ����֤��Ʊ
	else
	   Select_Market=1;  		//ѡ�����ڹ�Ʊ      
	for( int i=Cdat->lastclmin   ; i < m_MainDocument->m_nANT[Select_Market]  ;i++ )     //��ɲ������ߴ���
	{
         if(Cdat->lastclmin==0 && Cdat->initdown ==FALSE && m_MainDocument->m_nANT[Select_Market] > 0)      //���̵�һ�β������ߴ���
		 {
              Cdat->m_Kdata1[i].Amount =0;
              Cdat->m_Kdata1[i].Price =0 ;
              Cdat->m_Kdata1[i].Volume =0;
		 }
		 else
		 {
              Cdat->m_Kdata1[i].Amount =Cdat->m_Kdata1[Cdat->lastclmin  ].Amount;
              Cdat->m_Kdata1[i].Price =Cdat->m_Kdata1[Cdat->lastclmin  ].Price ;
              Cdat->m_Kdata1[i].Volume =Cdat->m_Kdata1[Cdat->lastclmin ].Volume ;
		 }
	}
	if(Cdat->id[1]=='A')
	{
		Rsdn1 **Nidx2;
        Tidxd **Tidx2;
        m_MainDocument->m_sharesInformation.GetIndexRsdn(Nidx2); 
        m_MainDocument->m_sharesInformation.GetIndexTidxd(Tidx2); 
		for( short i=Cdat->lastclmin ; i < m_MainDocument->m_nANT[Select_Market]  ;i++ )     //��ɲ������ߴ���
		{
			 if(Cdat->lastclmin==0 && Cdat->initdown ==FALSE&&m_MainDocument->m_nANT[Select_Market] > 0)      //���̵�һ�β������ߴ���
			 {
					 Nidx2[Select_Market][i].rsn=0;
					 Nidx2[Select_Market][i].dnn=0;
			 }
			 else
			 {
					 Nidx2[Select_Market][i].rsn=Nidx2[Select_Market][Cdat->lastclmin].rsn;
					 Nidx2[Select_Market][i].dnn=Nidx2[Select_Market][Cdat->lastclmin].dnn;
			 }
		}        
	}
   if(!Cdat->initdown)
	   m_MainDocument->InitFiveDaysVolumnForStock(Cdat);
    Cdat->initdown =TRUE;                                   //���ø�֤ȯ�Ѵ���
	if(m_GpHqReport->m_fLastClose !=0&& m_GpHqReport->m_fLastClose!=-1)
	   Cdat->ystc=m_GpHqReport->m_fLastClose;               //�õ������� 

	//���������������Ƿ����������������˳�
	{
	}
	bool bBuySell = false;
	if(Cdat ->ystc>0 && m_GpHqReport->m_fNewPrice>0)
	{
		float ff =( m_GpHqReport->m_fNewPrice - Cdat ->ystc )/Cdat ->ystc;
		if(ff>0.095 || ff<-0.095)//��ͣ��
			bBuySell = true;
		if(Cdat->id[0] == 'S' && Cdat->id[1] == 'T')
			if(ff>0.047 || ff<-0.047)//��ͣ��
				bBuySell = true;
	}
	//��������
//
	if(Cdat->kind!=SHZS && Cdat->kind!=SZZS)
	{
		if ((m_GpHqReport->m_fBuyPrice[0]>0 || bBuySell == true) && m_GpHqReport->m_fBuyPrice[0]!=-1)
		{
			if (m_GpHqReport->m_fBuyPrice[0]!=Cdat->pbuy1)                    //��������۷����仯
			{
				if (m_MainDocument->m_nANT[Select_Market]<0)
					m_MainDocument->m_nANT[Select_Market]=0;
				Cdat->pbuy1 =m_GpHqReport->m_fBuyPrice[0];                     //�õ���������� 
				chg=1;                                        //��������۷����仯                                                       
			}
		}
		//�õ����������� 
		if ((m_GpHqReport->m_fSellPrice[0] >0 || bBuySell == true) && m_GpHqReport->m_fSellPrice[0]!=-1)
		{
			if (m_GpHqReport->m_fSellPrice[0]!=Cdat->psel1)
			{
				if (m_MainDocument->m_nANT[Select_Market]<0)
					m_MainDocument->m_nANT[Select_Market]=0;
				Cdat->psel1=m_GpHqReport->m_fSellPrice[0];                      //�õ����������� 
				chg=1;                                        //��������������۷����仯������CHG=1 
			}
		}
	}

	if (m_GpHqReport->m_fHigh!=0&&m_GpHqReport->m_fHigh!=-1)                        //�õ���߼� 
		Cdat->higp=m_GpHqReport->m_fHigh;          
	
	if (m_GpHqReport->m_fLow!=0&&m_GpHqReport->m_fLow!=-1)
		Cdat->lowp=m_GpHqReport->m_fLow;                 //�õ���ͼ�
	
	if (m_GpHqReport->m_fOpen!=0&&m_GpHqReport->m_fOpen!=-1)
		Cdat->opnp=m_GpHqReport->m_fOpen;                //�õ����տ��̼�
///
	int MinuteCount = 0;
	if (m_GpHqReport->m_fNewPrice!=0&&m_GpHqReport->m_fNewPrice!=-1)                    //�õ����¼� 
	{

		//CTime tm1(m_GpHqReport->m_time);
		//CTime tm0(tm1.GetYear(),tm1.GetMonth(),tm1.GetDay(),9,30,0,0);
		//CTimeSpan tmSpan(tm1 - tm0);
		//chk_rdp(Cdat,m_GpHqReport->m_fNewPrice,tmSpan.GetTotalMinutes());
		if(m_GpHqReport->m_time > 240)
		{
			MinuteCount = GetStockMinute(m_GpHqReport->m_time,Select_Market + 1); //���������
		}
		else
		{
			MinuteCount = m_GpHqReport->m_time;
		}
		chk_rdp(Cdat,m_GpHqReport->m_fNewPrice,MinuteCount);
		if (m_GpHqReport->m_fNewPrice!=Cdat->nowp)                    //�Ƚ�����һ�����¼�
		{
			if (m_MainDocument->m_nANT[Select_Market]<0)
				m_MainDocument->m_nANT[Select_Market]=0;
			Cdat->nowp = m_GpHqReport->m_fNewPrice;                     //�õ����¼�
			chg=2;                                       //���¼۱仯ʱ������chg��Ϊ2
		}
	}

	bool bValid = false;
	for(int k = 0;k<3;k++)
	{
		if(fabs(m_GpHqReport->m_fBuyVolume[k])>=0.5) 
		{bValid = true;break;}
		if(fabs(m_GpHqReport->m_fSellVolume[k])>=0.5) 
		{bValid = true;break;}
	}
	if(Cdat->kind!=SHZS && Cdat->kind!=SZZS)
	{
		if((m_GpHqReport->m_fBuyVolume[0]>0 || bValid == true) &&m_GpHqReport->m_fBuyVolume[0]!=-1 )          //�õ���һ��
		   Cdat->vbuy1=m_GpHqReport->m_fBuyVolume[0];

		if((m_GpHqReport->m_fBuyPrice[1]>0.01 ||m_GpHqReport->m_fBuyPrice[1] == 0 && bBuySell) &&m_GpHqReport->m_fBuyPrice[1] !=-1)           //�õ������
		   Cdat->pbuy2=m_GpHqReport->m_fBuyPrice[1];          

		if((m_GpHqReport->m_fBuyVolume[1]>0 || bValid == true)&&m_GpHqReport->m_fBuyVolume[1] !=-1)          //�õ������
		   Cdat->vbuy2=m_GpHqReport->m_fBuyVolume[1];

		if((m_GpHqReport->m_fBuyPrice[2]>0.01||m_GpHqReport->m_fBuyPrice[2] == 0 && bBuySell)  &&m_GpHqReport->m_fBuyPrice[2]!=-1)         //�õ�������
		   Cdat->pbuy3=m_GpHqReport->m_fBuyPrice[2];

		if((m_GpHqReport->m_fBuyVolume[2]>0 || bValid == true) && m_GpHqReport->m_fBuyVolume[2]!=-1)         //�õ�������
		   Cdat->vbuy3=m_GpHqReport->m_fBuyVolume[2];

		if((m_GpHqReport->m_fBuyPrice4>0.01||m_GpHqReport->m_fBuyPrice4 == 0 && bBuySell)  &&m_GpHqReport->m_fBuyPrice4!=-1)         //�õ�������
		   Cdat->pbuy4=m_GpHqReport->m_fBuyPrice4;

		if((m_GpHqReport->m_fBuyVolume4>0 || bValid == true) && m_GpHqReport->m_fBuyVolume4!=-1)         //�õ�������
		   Cdat->vbuy4=m_GpHqReport->m_fBuyVolume4;

		if((m_GpHqReport->m_fBuyPrice5>0.01||m_GpHqReport->m_fBuyPrice5 == 0 && bBuySell)  &&m_GpHqReport->m_fBuyPrice5!=-1)         //�õ�������
		   Cdat->pbuy5=m_GpHqReport->m_fBuyPrice5;

		if((m_GpHqReport->m_fBuyVolume5>0 || bValid == true) && m_GpHqReport->m_fBuyVolume5!=-1)         //�õ�������
		   Cdat->vbuy5=m_GpHqReport->m_fBuyVolume5;

		if((m_GpHqReport->m_fSellVolume[0]>0 || bValid == true)&& m_GpHqReport->m_fBuyVolume[0]!=-1)      //�õ���һ��
		   Cdat->vsel1=m_GpHqReport->m_fSellVolume[0];

		if((m_GpHqReport->m_fSellPrice[1]>0.001||m_GpHqReport->m_fSellPrice[1] == 0 && bBuySell)   &&m_GpHqReport->m_fSellPrice[1]!=-1)       //�õ�������
		   Cdat->psel2=m_GpHqReport->m_fSellPrice[1];          

		if((m_GpHqReport->m_fSellVolume[1]>0 || bValid == true)&&m_GpHqReport->m_fSellVolume[1]!=-1)      //�õ�������
		   Cdat->vsel2=m_GpHqReport->m_fSellVolume[1];

		if((m_GpHqReport->m_fSellPrice[2]>0.001||m_GpHqReport->m_fSellPrice[1] == 0 && bBuySell) &&m_GpHqReport->m_fSellPrice[2]!=-1)       //�õ�������
		   Cdat->psel3=m_GpHqReport->m_fSellPrice[2];

		if((m_GpHqReport->m_fSellVolume[2]>0 || bValid == true) &&m_GpHqReport->m_fSellVolume[2]!=-1)        //�õ�������
		  Cdat->vsel3=m_GpHqReport->m_fSellVolume[2];

		if((m_GpHqReport->m_fSellPrice4>0.001||m_GpHqReport->m_fSellPrice[1] == 0 && bBuySell) &&m_GpHqReport->m_fSellPrice4!=-1)       //�õ�������
		   Cdat->psel4=m_GpHqReport->m_fSellPrice4;

		if((m_GpHqReport->m_fSellVolume4>0 || bValid == true) &&m_GpHqReport->m_fSellVolume4!=-1)        //�õ�������
		  Cdat->vsel4=m_GpHqReport->m_fSellVolume4;

		if((m_GpHqReport->m_fSellPrice5>0.001||m_GpHqReport->m_fSellPrice[1] == 0 && bBuySell) &&m_GpHqReport->m_fSellPrice5!=-1)       //�õ�������
		   Cdat->psel5=m_GpHqReport->m_fSellPrice5;

		if((m_GpHqReport->m_fSellVolume5>0 || bValid == true) &&m_GpHqReport->m_fSellVolume5!=-1)        //�õ�������
		  Cdat->vsel5=m_GpHqReport->m_fSellVolume5;

		Cdat->accb=0;
		ltmp = Cdat->nowp*0.97;                                             //Cdat[sel].nowpΪ���¼�      
		if ( Cdat->pbuy1>=ltmp )                                            
			Cdat->accb=Cdat->vbuy1;
		if ( Cdat->pbuy2>=ltmp )
			Cdat->accb+=Cdat->vbuy2;
		if ( Cdat->pbuy3>=ltmp )
			Cdat->accb+=Cdat->vbuy3;                                    //����ҳ�������
		if ( Cdat->pbuy4>=ltmp )
			Cdat->accb+=Cdat->vbuy4;                                    //����ҳ�������
		if ( Cdat->pbuy5>=ltmp )
			Cdat->accb+=Cdat->vbuy5;                                    //����ҳ�������
		Cdat->accs=0;
		ltmp = Cdat->nowp*1.03;
		if ( Cdat->psel1<=ltmp )
			Cdat->accs=Cdat->vsel1;
		if ( Cdat->psel2<=ltmp )
			Cdat->accs+=Cdat->vsel2;
		if ( Cdat->psel3<=ltmp )
			Cdat->accs+=Cdat->vsel3;                            //����ҳ������� 
		if ( Cdat->psel4<=ltmp )
			Cdat->accs+=Cdat->vsel4;                            //����ҳ������� 
		if ( Cdat->psel5<=ltmp )
			Cdat->accs+=Cdat->vsel5;                            //����ҳ������� 
	}
	m_Stockvalue=m_GpHqReport->m_fVolume  ;                     //�õ���Ʊ�ɽ���
	if (m_GpHqReport->m_fVolume>0)
	{
		if(m_Stockvalue==0 && Cdat->totv>0 && m_MainDocument->m_nANT[0]>0)
		  m_Stockvalue=Cdat->totv;                        
		if (m_Stockvalue!=Cdat->totv)
		{
			if (m_MainDocument->m_nANT[Select_Market]<0)
				m_MainDocument->m_nANT[Select_Market]=0;
			if (m_Stockvalue>Cdat->totv)                                        //��Ʊ�������仯
			{
				Cdat->nowv=m_Stockvalue - Cdat->totv;                           //�õ���ʱ�ɽ���
                
				if(Cdat->kind!=SHZS && Cdat->kind!=SZZS)
				{
					if(Cdat->totv==0)
					{
						Cdat->dvol+=Cdat->nowv/2;                             //��������
						Cdat->rvol+=Cdat->nowv/2;                             //��������
						Cdat->InOut=2;
						SendCuoHeDataToDialog(Cdat);                                  //����ʾ�ȴ���һ������ʱ����������������ʾ
					}
					else
					{
						if (Cdat->nowp >=OldSellP1&&m_GpHqReport->m_fSellPrice[0]!=0)
						{
							Cdat->rvol+=Cdat->nowv;                             //��������
							Cdat->InOut=0;
							SendCuoHeDataToDialog(Cdat);                        //����ʾ�ȴ���һ������ʱ����������������ʾ
						}
						else if (Cdat->nowp <=OldBuyP1&&m_GpHqReport->m_fBuyPrice[0]!=0)
						{
							Cdat->dvol+=Cdat->nowv;                             //��������
							Cdat->InOut=1;
							SendCuoHeDataToDialog(Cdat);                                  //����ʾ�ȴ���һ������ʱ����������������ʾ
						}
						else 
						{
							Cdat->dvol+=Cdat->nowv/2;                             //��������
							Cdat->rvol+=Cdat->nowv/2;                             //��������
							Cdat->InOut=2;
							SendCuoHeDataToDialog(Cdat);                                  //����ʾ�ȴ���һ������ʱ����������������ʾ
						}
					}
					chg=3;                                                  //�����������仯 
				}
			}
		}
	}

	if(m_Stockvalue!=0)
	   Cdat->totv=m_Stockvalue;                                                //�����Ʊ�ܳɽ���  
	if (m_GpHqReport->m_fAmount!=0&&m_GpHqReport->m_fAmount!=-1)                          //�õ���Ʊ�ɽ����
	   Cdat->totp =m_GpHqReport->m_fAmount;
	
	
	if (Cdat->nowv<0)
		Cdat->nowv=0;

		//����ÿ����K������
	if(m_GpHqReport->m_fNewPrice >0)
		Cdat->m_Kdata1[m_MainDocument->m_nANT[Select_Market]].Price=m_GpHqReport->m_fNewPrice ;
	if(m_GpHqReport->m_fAmount >0)
		Cdat->m_Kdata1[m_MainDocument->m_nANT[Select_Market]].Amount= m_GpHqReport->m_fAmount ;
	if(m_GpHqReport->m_fVolume >0)
		Cdat->m_Kdata1[m_MainDocument->m_nANT[Select_Market]].Volume =m_GpHqReport->m_fVolume ;
    if(Cdat->id[1] < 65) 
	if (chg==2 || chg==3)                                               //��Ʊ����۸����仯
	{
		if(m_GpHqReport->m_time > 240)
		{
			MinuteCount = GetStockMinute(m_GpHqReport->m_time,Select_Market + 1); //���������
		}
		else
		{
			MinuteCount = m_GpHqReport->m_time;
		}
		
		CTime tm1(m_GpHqReport->m_time);
		CTime tm0(tm1.GetYear(),tm1.GetMonth(),tm1.GetDay(),9,30,0,0);

		Cdat->time = tm1.GetTime();

	    UpdateHs(Cdat,m_MainDocument->m_nANT[Select_Market]);
	}
	//Cdat->lastclmin = MinuteCount;
	Cdat->lastclmin=m_MainDocument->m_nANT[Select_Market];

	AddDataCdat1(Cdat);

}
void CSharesCompute::StockDataMinUpdate(MIN_TOTAL_STRUCTEx * m_GpMinute)                  //��Ʊ�������             
{
	int i;
	int index =0;
	static int steps=0;
	if(m_GpMinute->m_RcvMinute->m_head.m_dwHeadTag != EKE_HEAD_TAG)
		return;
	do
	{
		CReportData * Cdat;
		Cdat=NULL;
		CString StockId;
		if( m_GpMinute->m_RcvMinute[index].m_head .m_dwHeadTag == EKE_HEAD_TAG )   //�ж��Ƿ�Ϊ��������ͷ��Ϣ
		{

			StockId=m_GpMinute->m_RcvMinute[index].m_head.m_szLabel ;          //ȡ֤ȯ������Ϣ
            StockId.MakeUpper(); 
			int nKind=m_MainDocument->m_sharesInformation.GetStockKind(m_GpMinute->m_RcvMinute[index].m_head.m_wMarket,m_GpMinute->m_RcvMinute[index].m_head.m_szLabel);
			if(m_MainDocument->m_sharesInformation.Lookup(StockId.GetBuffer(0) , Cdat,nKind) != TRUE)       //���Ҳ������߽ṹ
			{
				Cdat=NULL;
				index++;
				continue;
			}
		}
		if(Cdat==NULL)                        //�ж�Cdat �Ƿ�Ϊ��,��Ϊ�չ�Ʊ��������û���ҵ�
		{
			index++;
			continue;
		}
		//Marenan ��������if���ִ�У���index�������Σ����Ƶ�if�������
		index++;
		int mode=0;
		if(m_GpMinute->m_RcvMinute[index].m_time<0)           //��UTCС��0 �����ݲ���ȷ
		{
			Cdat=NULL;
			index++;
			continue;
		}
		//index++;
		//int mode=0;
		if(m_GpMinute->m_RcvMinute[index].m_head.m_wMarket ==SZ_MARKET_EX)    //�ζ��Ƿ�Ϊ���ڹ�Ʊ
			mode=1;
        short MinuteCount=0;
		do
		{
			if(m_GpMinute->m_RcvMinute[index].m_time > 240)
			{
			   MinuteCount = GetStockMinute(m_GpMinute->m_RcvMinute[index].m_time,mode + 1); //���������
			}
			else
			{
			   MinuteCount = m_GpMinute->m_RcvMinute[index].m_time;
			}
			if(MinuteCount <= m_MainDocument->m_nANT[0])
			{
                if(m_GpMinute->m_RcvMinute[index].m_fPrice!=0)
				{
					Cdat->m_Kdata1[MinuteCount].Price   = m_GpMinute->m_RcvMinute[index].m_fPrice ;
					Cdat->m_Kdata1[MinuteCount].Amount  = m_GpMinute->m_RcvMinute[index].m_fAmount;
					Cdat->m_Kdata1[MinuteCount].Volume  = m_GpMinute->m_RcvMinute[index].m_fVolume;
					chk_rdp(Cdat,Cdat->m_Kdata1[MinuteCount].Price,MinuteCount);
				}
			}
			index++;
		}while(m_GpMinute->m_RcvMinute[index].m_head.m_dwHeadTag != EKE_HEAD_TAG && index < m_GpMinute->Min_Count && MinuteCount < 240);
		
		if(MinuteCount < m_MainDocument->m_nANT[0] && MinuteCount > m_MainDocument->m_nANT[0]-3)
		{
             for(int k=MinuteCount ;k<m_MainDocument->m_nANT[0];k++)
			 {
				Cdat->m_Kdata1[k].Price   =m_GpMinute->m_RcvMinute[index - 1].m_fPrice ;
				Cdat->m_Kdata1[k].Amount  =m_GpMinute->m_RcvMinute[index - 1].m_fAmount;
				Cdat->m_Kdata1[k].Volume  =m_GpMinute->m_RcvMinute[index - 1].m_fVolume;
			 }
		}
		if( Cdat->kind == SZZS)
		{
			float max = 0;
			for(int i = 0 ;i<m_MainDocument->m_nANT[0];i++)
			{
				if(Cdat->m_Kdata1[i].Volume >max)
					max = Cdat->m_Kdata1[i].Volume;
			}
			if(max>Cdat->totv )
			{
				int rate = 100;
				for(i = 0;max/100>Cdat->totv;i++)
				{
					rate*=100;
					max/=100;
				}
				for(i = 0 ;i<m_MainDocument->m_nANT[0];i++)
				{
					Cdat->m_Kdata1[i].Volume /= rate;
				}
			}
		}
		Cdat->lastclmin=m_MainDocument->m_nANT[mode];
		//Cdat->lastclmin =m_MainDocument->m_nANT[0] - 1 ;
		Cdat->initdown =TRUE;
		ProcMessage();
		UpdateHs(Cdat,-1);
		Cdat=NULL;

		if(CMainFrame::m_pDoc->m_sharesSymbol == StockId)
		{
			CMainFrame::m_pDoc->UpdateAllViews(NULL);
		}
	}while(index<m_GpMinute->Min_Count);
}
#endif	

void CSharesCompute::AddDataCdat1(CReportData *p)
{
//#define MAX_COUNT_CDAT1    30;

//	SymbolKindArr m_dataArr;//SymbolKind
	if(p->kind == SHZS || p->kind == SZZS)
		return;

	int nMarket = 0;
	if(p->kind == SZAG ||p->kind == SZBG ||p->kind == SZZQ ||
		p->kind == SZJJ || p->kind == SZZZ || p->kind == SZHG || 
		p->kind == SZETF || p->kind == SZKF || p->kind == SZQZ || 
		p->kind == SZSB || p->kind == SZOT || p->kind == SZZXQY || p->kind == SZCYB)
		nMarket = 1;

	int n = m_dataArr[nMarket].GetSize();
	SymbolKind sym;
	sym.m_nSymbolKind = p->kind ;
	strcpy(sym.m_chSymbol,p->id);

	for(int i = 0;i<n;i++)
	{
		if(strcmp(m_dataArr[nMarket][i].m_chSymbol,p->id) ==0 && p->kind == m_dataArr[nMarket][i].m_nSymbolKind)
		{
			m_dataArr[nMarket].RemoveAt(i);
			m_dataArr[nMarket].Add(sym);
			return;
		}
	}

	if(n>=MAX_COUNT_CDAT1)
		m_dataArr[nMarket].RemoveAt(0);
	m_dataArr[nMarket].Add(sym);


}
