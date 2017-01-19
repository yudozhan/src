// CTaiKlineNnetBP.cpp: implementation of the CTaiKlineNnetBP class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "WH.h"
#include "TechNnetBP.h"
#include <math.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define kkk	0.05
#define bbb	0.1
#define fMaxInData	0.8
#define fMinInData	-0.8
#define fMaxOutData	20.0
#define fMinOutData	-20.0
const float expRateToTrain = 1;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTaiKlineNnetBP::CTaiKlineNnetBP()
{
	int i;
	for(i=0;i<64;i++)
	{
		m_fInRate[i] = 1;
		m_fOutRate[i] = 1;
		m_fInRateBias[i] = 0;
		m_fOutRateBias[i] = 0;
		m_fReservRate[i] = 1;
	}

	m_sFileName = "weightSave.wgt";
	m_nPeriod = 5;
	m_nDays = 1;
	m_nDaysManyDay = 50;

	m_pJishuIn =NULL;
	m_pJishuOut=NULL;

	m_fOut1=NULL;
	m_fOut2=NULL;
	m_fDif=NULL;
	m_fOutDel1=NULL;
	m_fOutDel2=NULL;
	m_fOutDel3=NULL;
	m_fOutput=NULL;

	m_expRateToTrain=20;

	for(i=0;i<5;i++)
		m_expRate[i] = 1;

}

CTaiKlineNnetBP::~CTaiKlineNnetBP()
{
	if(m_fOut1!=NULL) delete []m_fOut1;
	if(m_fOut2!=NULL) delete []m_fOut2;
	if(m_fDif!=NULL) delete []m_fDif;
	if(m_fOutDel1!=NULL) delete []m_fOutDel1;
	if(m_fOutDel2!=NULL) delete []m_fOutDel2;
	if(m_fOutDel3!=NULL) delete []m_fOutDel3;
	if(m_fOutput!=NULL) delete []m_fOutput;

	if(m_pJishuIn !=NULL)
		delete m_pJishuIn;
	if(m_pJishuOut !=NULL)
		delete m_pJishuOut;
}

bool CTaiKlineNnetBP::InitNnet(int nInputValue,int nOutputValue, float fError,int nRepeat,float a,float n,bool bLoadFile )
{
	m_nCountAdd=0;
	m_e_av = 0;
	m_diffPre = 10000000;
	m_nInputValue = nInputValue;
	m_nOutputValue = nOutputValue;
	m_fError = fError;
	m_nRepeat=nRepeat;
	m_a = a;
	m_n= n;
	m_bTellExpRate = true;

	//initiate the value of weight//[COUNT_NEURAL_LAY1][(nInputValue+1)]
	//input layyer
	int i,j;
	for(i=0;i<COUNT_NEURAL_LAY1;i++)
	{
//		ASSERT(m_pWeight[0][i] == NULL);
		for(j=0;j<nInputValue+1;j++)
		{
			m_pWeight[0][i][j]=(rand()%1000-500)/(float)500;
			m_pWeightPre[0][i][j]=(rand()%1000-500)/(float)500;
			m_pWeightPost[0][i][j]=(rand()%1000-500)/(float)500;
			m_pStudyRate[0][i][j]=m_n;
			m_pOldWeightRate[0][i][j]=0;
			m_pWeightMinSave[0][i][j]=m_pWeight[0][i][j];
		}
	}
	//hide layyer
	for( i=0;i<COUNT_NEURAL_LAY2;i++)
	{
//		ASSERT(m_pWeight[1][i] == NULL);
		for(j=0;j<COUNT_NEURAL_LAY1+1;j++)
		{
			m_pWeight[1][i][j]=(rand()%1000-500)/(float)500;
			m_pWeightPre[1][i][j]=(rand()%1000-500)/(float)500;
			m_pWeightPost[1][i][j]=(rand()%1000-500)/(float)500;
			m_pStudyRate[1][i][j]=m_n;
			m_pOldWeightRate[1][i][j]=0;
			m_pWeightMinSave[1][i][j]=m_pWeight[1][i][j];
		}
	}
	//out layyer
	for( i=0;i<m_nOutputValue;i++)
	{
//		ASSERT(m_pWeight[1][i] == NULL);
		for(j=0;j<COUNT_NEURAL_LAY2+1;j++)
		{
			m_pWeight[2][i][j]=(rand()%1000-500)/(float)500;
			m_pWeightPre[2][i][j]=(rand()%1000-500)/(float)500;
			m_pWeightPost[2][i][j]=(rand()%1000-500)/(float)500;
			m_pStudyRate[2][i][j]=m_n;
			m_pOldWeightRate[2][i][j]=0;
			m_pWeightMinSave[2][i][j]=m_pWeight[2][i][j];
		}
	}
	if(bLoadFile == true)
		LoadWeight();

	if(m_fOut1!=NULL)
		delete m_fOut1;
	if(m_fOut2!=NULL)
		delete m_fOut2;
	if(m_fDif!=NULL)
		delete m_fDif;
	if(m_fOutDel1!=NULL)
		delete m_fOutDel1;
	if(m_fOutDel2!=NULL)
		delete m_fOutDel2;
	if(m_fOutDel3!=NULL)
		delete m_fOutDel3;
	m_fOut1 = new float[COUNT_NEURAL_LAY1];
	m_fOut2 = new float[COUNT_NEURAL_LAY2];
	m_fDif = new float[m_nOutputValue+1];
	m_fOutDel1 = new float[m_nOutputValue+1];
	m_fOutDel2 = new float[COUNT_NEURAL_LAY2+1];
	m_fOutDel3 = new float[COUNT_NEURAL_LAY1+1];
	m_fOutput = new float[m_nOutputValue];
	return true;
}

bool CTaiKlineNnetBP::FormularCompute(float *fInput, float *fOutputReal,bool bTestSucc)
{
//	for(int k = 0;k<10;k++)
	if(m_bTellExpRate)
		TestExpRate(fInput,false);//
	float max,fRtn;
	{
//	float fOutput;
	//first layyer
	max = GetOutValue(0, m_fOut1, COUNT_NEURAL_LAY1,fInput, m_nInputValue);
	max = FABSMY(max);
	//end
	//hide layyer
	fRtn = GetOutValue(1, m_fOut2, COUNT_NEURAL_LAY2,m_fOut1, COUNT_NEURAL_LAY1);
	if(FABSMY(fRtn)>max)
		max = FABSMY(fRtn);
	//end
	//out layyer
	fRtn = GetOutValue(2, m_fOutput, m_nOutputValue,m_fOut2, COUNT_NEURAL_LAY2);
	if(FABSMY(fRtn)>max)
		max = FABSMY(fRtn);

	//end
	int i;
	for(i=0;i<m_nOutputValue;i++)
		m_fDif[i] = fOutputReal[i]-m_fOutput[i];

	//tell diffrent one
	if(bTestSucc == false)
		if(TellDiffOne(m_fDif) == true)
			return false;

	//the out layyer
	m_fOutDel1[0] = 0;
	for(i=0;i<m_nOutputValue;i++)
	{
		m_fOutDel1[i]=m_expRate[2]*m_fDif[i]*(m_fOutput[i])*(1-m_fOutput[i]);
	}
	ModifyWeight(2, m_fOutDel1, m_nOutputValue, m_fOut2, COUNT_NEURAL_LAY2);
	//end
	//the hide layyer
	GetOutValueDel(2, m_fOutDel2,COUNT_NEURAL_LAY2, m_fOut2 ,m_nOutputValue,  m_fOutDel1);
	ModifyWeight(1, m_fOutDel2, COUNT_NEURAL_LAY2, m_fOut1, COUNT_NEURAL_LAY1);
	//end
	//the first layyer
	GetOutValueDel(1, m_fOutDel3,COUNT_NEURAL_LAY1, m_fOut1 ,COUNT_NEURAL_LAY2,  m_fOutDel2);
	ModifyWeight(0, m_fOutDel3, COUNT_NEURAL_LAY1, fInput, m_nInputValue);
	//end

	//to modify weight poiter
	memcpy(m_pWeightPre,m_pWeight,COUNT_LAYYER*COUNT_LAYYER_Max*COUNT_LAYYER_Max*4);
	memcpy(m_pWeight,m_pWeightPost,COUNT_LAYYER*COUNT_LAYYER_Max*COUNT_LAYYER_Max*4);
	}


	//test

	//to compute if diff is right
//	if(bTestSucc==true)
	{
		if(TellDiff(m_fDif,bTestSucc))
		{
			return true;
		}
		else
			return false;
	}
	//end
	m_bTellExpRate = false;
	if(max>=49)
		m_bTellExpRate = true;

	return false;

}
bool CTaiKlineNnetBP::Compute222(float *fInput, float &fOutput)
{
	return true;
}


float CTaiKlineNnetBP::GetOutValue(int nLay, float *fOut, int nOut, float *fIn, int nIn)
{
	float max;
	int i,j;
	for(i=0;i<nOut;i++)
	{
		fOut[i]=-1*m_pWeight[nLay][i][0];
		for(j=1;j<nIn+1;j++)
		{
			fOut[i]+=(m_pWeight[nLay][i][j]*fIn[j-1]);
		}

		float f2 = FABSMY(fOut[i]);
		if(i==0)
			max = f2;
		else if(max<f2)
			max = f2;

		if(fOut[i]>50)
			fOut[i] = 50;
		if(fOut[i]<-50)
			fOut[i] = -50;

	}

	for( i=0;i<nOut;i++)
	{
		fOut[i] = 1/(1+exp(-m_expRate[nLay]*fOut[i]));
	}

	return max;
}

void CTaiKlineNnetBP::GetOutValueDel(int nLay, float *fOut,int nOut,  float *fIn,int nIn,  float *fPre)
{
	for(int i=0;i<nOut;i++)
	{
		float f = 0;
		for(int j=0;j<nIn;j++)
		{
			f+=(fPre[j]*m_pWeight[nLay][j][i+1]);
		}
		fOut[i]=m_expRate[nLay]*f*(fIn[i])*(1-fIn[i]);
	}
}
//modify the weight value
//ModifyWeight(int nLay,float* fDelIn,int nDelIn,float* fYi,int nYi)


void CTaiKlineNnetBP::ModifyWeight(int nLay, float *fDelIn, int nDelIn, float *fYi, int nYi)
{
	for(int i=0;i<nDelIn;i++)
	{
		m_pWeightPost[nLay][i][0] = m_pWeight[nLay][i][0] +m_a*(m_pWeight[nLay][i][0] 
			-m_pWeightPre[nLay][i][0])+m_n*fDelIn[i]* (-1);//?
		for(int j=1;j<nYi+1;j++)
		{
//			m_pStudyRate[2][i][j]=m_n;
//			m_pOldWeightRate[2][i][j]=0;
			float f = fDelIn[i]* fYi[j-1];
			m_pWeightPost[nLay][i][j] = m_pWeight[nLay][i][j] 
				+m_a*(m_pWeight[nLay][i][j] -m_pWeightPre[nLay][i][j])
				+m_pStudyRate[nLay][i][j]*f;//?

			if(m_pOldWeightRate[nLay][i][j]*f>0)
				m_pStudyRate[nLay][i][j] += kkk;
			else if(m_pOldWeightRate[nLay][i][j]*f<0)
				m_pStudyRate[nLay][i][j] += (-bbb*m_pStudyRate[nLay][i][j]);
			if(m_pStudyRate[nLay][i][j]>=1)
				m_pStudyRate[nLay][i][j] = 0.999;
			if(m_pStudyRate[nLay][i][j]<=0)
				m_pStudyRate[nLay][i][j] = 0.001;
				
			m_pOldWeightRate[nLay][i][j] = f;
		}
	}

}

bool CTaiKlineNnetBP::TellDiff(float *fDif,bool bTestSucc)
{
	if(bTestSucc==true)
	{
		m_nCountAdd=0;
		m_e_av = 0;
	}
	m_nCountAdd++;
	if(m_nCountAdd==0)
		m_nCountAdd++;
	float ee = 0;
	for(int i=0;i<this->m_nOutputValue ;i++)
	{
		ee+=(fDif[i]*fDif[i]);
	}
	ee/=2;

	m_e_av+=ee;
	if(bTestSucc==true)
	{
		m_diffLast=(m_e_av/m_nCountAdd);

		static int nJump = 0;
//		TRACE("%f   e_av\n",e_av);exp
 		if(m_diffLast<m_diffPre)
		{
			memcpy(m_pWeightMinSave,m_pWeightPre,COUNT_LAYYER*COUNT_LAYYER_Max*COUNT_LAYYER_Max*4);
			m_diffPre = m_diffLast;
			nJump = 0;
		}
 		if(m_diffLast<=this->m_fError )
			return true;

		nJump++;
		if(nJump>this->m_nRepeat /10)
		{
			if(m_diffPre<m_fError*100)
				return true;
		}
	}

	return false;
}
int CTaiKlineNnetBP::SimulatNow(float *fInput,float *fOutput)
{
	//now normalize
	{
		for(int j=0;j<m_nInputValue;j++)
		{
			fInput[j] = m_fInRate[j]*fInput[j]+m_fInRateBias[j] ;
		}
	}

	//first layyer
	GetOutValue(0, m_fOut1, COUNT_NEURAL_LAY1,fInput, m_nInputValue);
	//end
	//hide layyer
	GetOutValue(1, m_fOut2, COUNT_NEURAL_LAY2,m_fOut1, COUNT_NEURAL_LAY1);
	//end
	//out layyer
	GetOutValue(2, fOutput, m_nOutputValue,m_fOut2, COUNT_NEURAL_LAY2);

	//归一化数据
	int i;
	for(i=0;i<m_nOutputValue;i++)
	{
		fOutput[i] = RegularData(fOutput[i],false);
	}
	for(i=0;i<m_nOutputValue;i++)
	{
		if(m_fOutRate[i]!=0)
			fOutput[i] = (fOutput[i]-m_fOutRateBias[i])/m_fOutRate[i] ;
	}

	return m_nOutputValue;
}

bool CTaiKlineNnetBP::Test()
{
/*	CTaiKlineNnetBP net;
	int nIn = 20;
	int nOut = 3;
	int tot =200;
	net.InitNnet (nIn,3,0.000001,10,0.5,0.5,false);
	float fIn[500][20];
	float fOut[200][3];
	int i;

	int tm = GetTickCount();
	CFile fl;
	fl.Open ("dataSave.txt",CFile::modeWrite|CFile::modeCreate);

	CString s;
	for( i=0;i<tot;i++)
	{
		float f2=10;
		if(i%2==0)
			f2=100;
		for(int j=0;j<nIn;j++)
		{
			float f=1;
			if(rand()%2==0)
				f=-1;
			fIn[i][j] = (rand()%1000)/(float)1000*(f);

			s.Format ("%.3f,\t",fIn[i][j]);
			fl.Write (s.GetBuffer (0),s.GetLength ());
			TRACE("%.3f,\t",fIn[i][j]);
		}
		s="\r\n\r\n";
		fl.Write (s.GetBuffer (0),s.GetLength ());
		TRACE("\n");
	}
//	CFile flIn;
//	flIn.Open ("fInData.dat",CFile::modeRead);//Write|CFile::modeCreate);
//	flIn.Read (fIn,500*100*4);
//	flIn.Close();
		TRACE("out  \n");
		TRACE("out  \n");
		TRACE("out  \n");
		TRACE("out \n");
		TRACE("out  \r\n");

	for( i=0;i<tot;i++)
	{
		float f=0.8;
		if(i%2==0)
			f=-0.8;
		for(int j=0;j<nOut;j++)
		{
			fOut[i][j] = f;//+0.5;
			s.Format ("%.3f,\t",fOut[i][j]);
			fl.Write (s.GetBuffer (0),s.GetLength ());
			TRACE("%.3f,\t",fOut[i][j]);
		}
		TRACE("\n");
		s = "\r\n\r\n";
		fl.Write (s.GetBuffer (0),s.GetLength ());
	}
	bool bTestSucc = false;

	net.TestExpRate(fIn[0]);

	int nTrainNow = 10;
	if(nTrainNow>tot)
		nTrainNow = tot; 
	bool bSu  ;
	for(int k=0;k<net.m_nRepeat ;k++)
	{
		bSu = false;
		for( i=0;i<nTrainNow;i++)
		{
			bTestSucc = false;
			if(i==nTrainNow-1)
				bTestSucc = true;
			if(bSu = net.FormularCompute(fIn[i],fOut[i], bTestSucc))
				break;
		}
		if(bSu==true&&nTrainNow>=tot)
			break;
		if(bSu==true||(k>=net.m_nRepeat-1&&nTrainNow<tot))
		{
			nTrainNow*=2;
			if(nTrainNow>tot)
				nTrainNow = tot; 
			k=-1;
			continue;
		}
	}


	TRACE("simulat\n");
	TRACE("simulat\n");
	TRACE("simulat\n");
	TRACE("simulat\n");
	s = "simulat       \r\n\r\n";
	fl.Write (s.GetBuffer (0),s.GetLength ());

	float fSimOut[3];
	for( i=0;i<tot;i++)
	{
		net.SimulatNow (fIn[i],fSimOut);
		for(int j=0;j<nOut;j++)
		{
			s .Format ("%.3f,\t",fSimOut[j]);
			fl.Write (s.GetBuffer (0),s.GetLength ());
			TRACE("%.3f,\t",fSimOut[j]);
		}
		s = "\r\n\r\n";
		fl.Write (s.GetBuffer (0),s.GetLength ());
	TRACE("\n");
	}

	fl.Close ();

	net.SaveWeight();

	tm = (GetTickCount()-tm)/60;
	s.Format ("%d  minute",tm);
	AfxMessageBox(s);
*/
	return true;

}

void CTaiKlineNnetBP::SaveWeight()
{
	if(m_pJishuIn ==NULL)
		return;
	if(m_pJishuOut ==NULL)
		return;

	CFile fl;
	CString sPath = "知识数据库\\"+m_sFileName;
	if(fl.Open (sPath,CFile::modeWrite|CFile::modeCreate)==FALSE)
		return ;//|CFile::modeNoTruncate

//	CFileFind filefind;
//	if(filefind.FindFile (sPath) == TRUE)
//		if(AfxMessageBox("是否覆盖原始文件的数据？",MB_YESNO)!=IDYES   )
//			return;
/*
经验知识数据库文件的格式：
0-3： 文件标识符
4-7：输入点的个数 
8-11：层1点的个数 
12-15：层2点的个数 
16-19：输出点的个数 
20-23：总层数 

接着4字节为时间周期
接着4字节为输出指标数据向前移动的周期数
接着4字节为多日线的天数，负数表示多分钟

接着32字节为输入指标名称
接着32字节为输出指标名称

接着4字节为输入指标公式参数个数
接着4字节为输出指标公式参数个数
接着“输入指标公式参数个数*4”字节为输入指标的参数数值
接着“输出指标公式参数个数*4”字节为输出指标的参数数值
接着“输入指标公式参数个数*32”字节为输入指标的参数名称
接着“输出指标公式参数个数*32”字节为输出指标的参数名称

接着4字节为输入指标公式字符串的字符个数
接着4字节为输出指标公式字符串的字符个数

接着“输入指标公式字符串的字符个数”字节为输入指标公式字符串
接着“输出指标公式字符串的字符个数”字节为输出指标公式字符串

接着COUNT_LAYYER*COUNT_LAYYER_Max*COUNT_LAYYER_Max*4字节为知识库信息
接着COUNT_LAYYER*COUNT_LAYYER_Max*COUNT_LAYYER_Max*4字节为学习率库信息

接着64*3*4字节为浮点数

*/
	int n = 65798809;
	fl.Write (&n,4);
	fl.Write (&m_nInputValue,4);
	n=COUNT_NEURAL_LAY1;
	fl.Write (&n,4);
	n=COUNT_NEURAL_LAY2;
	fl.Write (&n,4);//m_nOutputValue
	fl.Write (&m_nOutputValue,4);//m_nOutputValue
	n = 3;
	fl.Write (&n,4);
//	int m_nPeriod;
//	int m_nDays;
	fl.Write (&m_nPeriod,4);
	fl.Write (&m_nDays,4);
	fl.Write (&m_nDaysManyDay,4);

	//指标公式写入文件
	char ch[32];
	strcpy(ch,this->m_pJishuIn ->name );
	ch[31]=0;
	fl.Write (ch,32);
	strcpy(ch,this->m_pJishuOut ->name );
	ch[31]=0;
	fl.Write (ch,32);

	n = this->m_pJishuIn ->numPara ;
	if(n>8)
		n=0;
	fl.Write (&n,4);
	int n2 = this->m_pJishuOut ->numPara ;
	if(n2>8)
		n2=0;
	fl.Write (&n2,4);

	int i;
	for(i=0;i<m_pJishuIn ->numPara;i++)
	{
		fl.Write (&(m_pJishuIn ->defaultVal  [i]),4);
	}
	for(i=0;i<m_pJishuOut ->numPara;i++)
	{
		fl.Write (&(m_pJishuOut ->defaultVal  [i]),4);
	}
	for( i=0;i<n;i++)
	{
		strcpy(ch,this->m_pJishuIn ->namePara [i] );
		ch[31]=0;
		fl.Write (ch,32);
	}
	for(i=0;i<n2;i++)
	{
		strcpy(ch,this->m_pJishuOut ->namePara [i] );
		ch[31]=0;
		fl.Write (ch,32);
	}

	n = this->m_pJishuIn ->fomular .GetLength () ;
	if(n<0) n=0;
	fl.Write (&n,4);
	n2 = this->m_pJishuOut ->fomular .GetLength () ;
	if(n2<0) n2=0;
	fl.Write (&n2,4);
	fl.Write (this->m_pJishuIn ->fomular .GetBuffer (n),n);
	m_pJishuIn ->fomular .ReleaseBuffer ();
	fl.Write (this->m_pJishuOut ->fomular .GetBuffer (n2),n2);
	m_pJishuOut ->fomular .ReleaseBuffer ();


	fl.Write (this->m_pWeight ,COUNT_LAYYER*COUNT_LAYYER_Max*COUNT_LAYYER_Max*4);
	fl.Write (this->m_pStudyRate ,COUNT_LAYYER*COUNT_LAYYER_Max*COUNT_LAYYER_Max*4);

	{
		fl.Write (m_fInRate,64*4);
		fl.Write (m_fOutRate,64*4);
		fl.Write (m_fInRateBias,64*4);
		fl.Write (m_fOutRateBias,64*4);
		fl.Write (m_fReservRate,64*4);
	}

	fl.Close();

	//to get min weight
	if(m_diffPre < m_diffLast)
		memcpy(m_pWeight,m_pWeightMinSave,COUNT_LAYYER*COUNT_LAYYER_Max*COUNT_LAYYER_Max*4);

}

void CTaiKlineNnetBP::LoadWeight()
{
	CFile fl;
	CString sPath = "知识数据库\\"+m_sFileName;
	if(fl.Open (sPath,CFile::modeRead)==FALSE)
		return ;//|CFile::modeNoTruncate
/*
经验知识数据库文件的格式：
0-3： 文件标识符
4-7：输入点的个数 
8-11：层1点的个数 
12-15：层2点的个数 
16-19：输出点的个数 
20-23：总层数 

接着4字节为时间周期
接着4字节为输出指标数据向前移动的周期数
接着4字节为多日线的天数，负数表示多分钟

接着32字节为输入指标名称
接着32字节为输出指标名称

接着4字节为输入指标公式参数个数
接着4字节为输出指标公式参数个数
接着“输入指标公式参数个数*4”字节为输入指标的参数数值
接着“输出指标公式参数个数*4”字节为输出指标的参数数值
接着“输入指标公式参数个数*32”字节为输入指标的参数名称
接着“输出指标公式参数个数*32”字节为输出指标的参数名称

接着4字节为输入指标公式字符串的字符个数
接着4字节为输出指标公式字符串的字符个数

接着“输入指标公式字符串的字符个数”字节为输入指标公式字符串
接着“输出指标公式字符串的字符个数”字节为输出指标公式字符串

接着COUNT_LAYYER*COUNT_LAYYER_Max*COUNT_LAYYER_Max*4字节为知识库信息
接着COUNT_LAYYER*COUNT_LAYYER_Max*COUNT_LAYYER_Max*4字节为学习率库信息

接着64*3*4字节为浮点数


*/

	int n = 3;
	fl.Read (&n,4);
	if(n!=65798809)
		return;

	fl.Read (&n,4);
	m_nInputValue = n;
//	if(n!=m_nInputValue)
//		return;
	fl.Read (&n,4);
	if(n!=COUNT_NEURAL_LAY1)
		return;
	fl.Read (&n,4);//m_nOutputValue
	if(n!=COUNT_NEURAL_LAY2)
		return;
	fl.Read (&n,4);//m_nOutputValue
	m_nOutputValue=n;
//	if(n!=m_nOutputValue)
//		return;
	fl.Read (&n,4);
	if(n!=3)
		return;
//	int m_nPeriod;
//	int m_nDays;
	fl.Read (&m_nPeriod,4);
	fl.Read (&m_nDays,4);
	fl.Read (&m_nDaysManyDay,4);

	//begin 指标公式写入文件
	if(m_pJishuIn ==NULL)
		m_pJishuIn = new CFormularContent;
	if(m_pJishuOut ==NULL)
		m_pJishuOut = new CFormularContent;
	char ch[32];
	fl.Read (ch,32);
	m_pJishuIn ->name = ch;
	fl.Read (ch,32);
	m_pJishuOut ->name = ch;

	fl.Read (&m_pJishuIn ->numPara,4);
	if(m_pJishuIn ->numPara>8)
		m_pJishuIn ->numPara=0;
	fl.Read (&m_pJishuOut ->numPara,4);
	if(m_pJishuOut ->numPara>8)
		m_pJishuOut ->numPara=0;

	int i;
	for(i=0;i<m_pJishuIn ->numPara;i++)
	{
		fl.Read (&(m_pJishuIn ->defaultVal  [i]),4);
	}
	for(i=0;i<m_pJishuOut ->numPara;i++)
	{
		fl.Read (&(m_pJishuOut ->defaultVal  [i]),4);
	}
	for(i=0;i<m_pJishuIn ->numPara;i++)
	{
		fl.Read (m_pJishuIn ->namePara [i].GetBuffer (32),32);
		m_pJishuIn ->namePara [i].ReleaseBuffer ();
	}
	for(i=0;i<m_pJishuOut ->numPara;i++)
	{
		fl.Read (m_pJishuOut ->namePara [i].GetBuffer (32),32);
		m_pJishuOut ->namePara [i].ReleaseBuffer ();
	}

//	n = this->m_pJishuIn ->fomular .GetLength () ;
	fl.Read (&n,4);
	if(n<0) n=0;
	int n2;
	fl.Read (&n2,4);
	if(n2<0) n2=0;

	char* pch = new char[n+1];
	pch[n]=0;
	fl.Read (pch,n);
	this->m_pJishuIn ->fomular = pch;
	delete []pch;
	pch = new char[n2+1];
	pch[n2]=0;
	fl.Read (pch,n2);
	this->m_pJishuOut ->fomular = pch;
	delete []pch;
	//end 技术指标

	fl.Read (this->m_pWeight ,COUNT_LAYYER*COUNT_LAYYER_Max*COUNT_LAYYER_Max*4);
	fl.Read (this->m_pStudyRate ,COUNT_LAYYER*COUNT_LAYYER_Max*COUNT_LAYYER_Max*4);

	{
		fl.Read (m_fInRate,64*4);
		fl.Read (m_fOutRate,64*4);
		fl.Read (m_fInRateBias,64*4);
		fl.Read (m_fOutRateBias,64*4);
		fl.Read (m_fReservRate,64*4);
	}

	fl.Close();
	memcpy(m_pWeightPre,m_pWeight,COUNT_LAYYER*COUNT_LAYYER_Max*COUNT_LAYYER_Max*4);

}

bool CTaiKlineNnetBP::TellDiffOne(float *fDif)
{
	float ee = 0;
	for(int i=0;i<this->m_nOutputValue ;i++)
	{
		ee+=(fDif[i]*fDif[i]);
	}
	ee/=2;

	if(ee<=this->m_fError )
		return true;

	return false;

}

void CTaiKlineNnetBP::TestExpRate(float *fInput,bool bChangeExp)//m_bTellExpRate
{
	//to test weight
	if(m_pWeight[0][0][m_nInputValue/2]>10E5||m_pWeight[0][0][m_nInputValue/2]<-10E5)
	{
		for(int k=0;k<1;k++)
			for(int i=0;i<COUNT_LAYYER_Max;i++)
			{
				for(int j=0;j<COUNT_LAYYER_Max;j++)
				{
					m_pWeight[k][i][j]=m_pWeight[k][i][j]/(10);
					m_pWeightPre[k][i][j]=m_pWeightPre[k][i][j]/(10);
				}
			}
	}

	//end
	//to regular
	float max = GetOutValue(0, m_fOut1, COUNT_NEURAL_LAY1,fInput, m_nInputValue);
	if(max>=8)
	{
		m_expRate[0] = 8/max;
		GetOutValue(0, m_fOut1, COUNT_NEURAL_LAY1,fInput, m_nInputValue);
	}
	//end
	//hide layyer
	max = GetOutValue(1, m_fOut2, COUNT_NEURAL_LAY2,m_fOut1, COUNT_NEURAL_LAY1);
	if(max>=8)
	{
		m_expRate[1] = 8/max;
		GetOutValue(1, m_fOut2, COUNT_NEURAL_LAY2,m_fOut1, COUNT_NEURAL_LAY1);
	}
	//end
	//out layyer
	max = GetOutValue(2, m_fOutput, m_nOutputValue,m_fOut2, COUNT_NEURAL_LAY2);
	if(max>=8)
	{
		m_expRate[2] = 8/max;
		GetOutValue(2, m_fOutput, m_nOutputValue,m_fOut2, COUNT_NEURAL_LAY2);
	}

	//end

}

bool CTaiKlineNnetBP::PrepareKlineData(inKlineArray &inArray, float& fDiff, int maxOut,int nIn,int nOut,bool bAverageOut,bool bStatistic )
{
	int k,i,j;
	propArray propArr;
	if(inArray.GetSize() > 8000)
	{
		while(inArray.GetSize ()>8000)
		{
			inArray.RemoveAt(8000,inArray.GetSize ()-8000);
		}
	}
	if(bStatistic == true)
	{
		for(i=0;i<inArray.GetSize ();i++)
		{
			if(i==0)
			{
				inKlineArray * inTemp = new inKlineArray;
				inArray[i].differ =0;
				inTemp->Add (inArray[i]);
				propArr.Add (inTemp);
				continue;
			}

			//to classify the input data
			bool bNewClass = true;
			bool bContinue = false;
			for(j=0;j<propArr.GetSize ();j++)
			{
				float ee = 0,eeOut =0;
				for(k=0;k<nIn;k++)
				{
					float f= (inArray[i].dataIn [k]-propArr.GetAt (j)->GetAt (0).dataIn [k]);
					ee+=(f*f);
				}
				ee=ee/(nIn*2);
				for( k=0;k<nOut;k++)
				{
					float f= (inArray[i].dataOut [k]-propArr.GetAt (j)->GetAt (0).dataOut [k]);
					eeOut+=(f*f);
				}
				eeOut=eeOut/(nOut*2);

				//
				inArray[i].differOut = eeOut;
				if(ee<fDiff)
				{
					inArray[i].differ = ee;
					propArr.GetAt (j)->Add (inArray[i]);
					bNewClass = false;
					break;
				}
			}

			//new class
			if(bNewClass == true)
			{
				if(propArr.GetSize () >maxOut)
				{
					//clear all array
					for(j=0;j<propArr.GetSize ();j++)
					{
						propArr.GetAt (j)->RemoveAll ();
						delete propArr.GetAt (j);
					}
					propArr.RemoveAll ();

					//jump to begin
					fDiff = fDiff*2.0;
					i=-1;
					continue;
				}
				inKlineArray * inTemp = new inKlineArray;
				inArray[i].differ =0;
				inTemp->Add (inArray[i]);
				propArr.Add (inTemp);
			}
		}

		//to statistics input data
		for(i=0;i<propArr.GetSize ();i++)
		{
			int j=0,m=0,min =0;
			if(propArr[i]->GetSize ()>=3)
			{
				float fMax = propArr[i]->GetAt (0).differOut ;
				float fMin = fMax;
				for(j=1;j<propArr[i]->GetSize ();j++)
				{
					if(propArr[i]->GetAt (j).differOut >fMax)
					{
						m=j;
						fMax = propArr[i]->GetAt (j).differOut;
					}
				}
				propArr[i]->RemoveAt(m);
				for(j=1;j<propArr[i]->GetSize ();j++)
				{
					if(propArr[i]->GetAt (j).differOut <fMin)
					{
						min=j;
						fMin = propArr[i]->GetAt (j).differOut;
					}
				}
				propArr[i]->RemoveAt(min);
			}

			//average
			for(k = 0;k< nOut;k++)
			{
				float out = 0;
				for(j=0;j<propArr[i]->GetSize ();j++)
				{
					out+=propArr[i]->GetAt (j).dataOut [k];
				}
				out = out/propArr[i]->GetSize ();
				propArr[i]->GetAt (0).dataOut [k] = out;
			}
		}

		//to statistics output data
/*		if(bAverageOut == true)
		{
			int mid =0;
			int upCount = 0;
			for(i=0;i<propArr.GetSize ();i++)
			{
				int up = 0;
				for(int j=0;j<nOut;j++)
				{
					if(propArr[i]->GetAt (0).dataOut [j]>0.5)
						up++;
				}
				if(up>nOut*0.5)
				{
					upCount++;
				}
				if(up==nOut*0.5)
					mid++;
			}
			int countDelete = propArr.GetSize ()-mid-2*upCount;
			int n = abs(countDelete);
			while(n>0)
			{
				int n2 = propArr.GetSize ();
				if(n2<=0)
					break;

				while(propArr.GetSize ())
				{
					int nArr = rand()%propArr.GetSize ();
					int up = 0;
					for(int j=0;j<nOut;j++)
					{
						if(propArr[nArr]->GetAt (0).dataOut [j]>0.5)
							up++;
					}
					if(countDelete>0 &&up>nOut*0.5)
					{
						delete propArr[nArr];
						propArr.RemoveAt(nArr);
						break;
					}
					else if(up<nOut*0.5)
					{
						delete propArr[nArr];
						propArr.RemoveAt(nArr);
						break;
					}
					else 
					{
						static int nStc = 0;
						nStc++;
						if(nStc>=20)
						{
							nStc=0;
							break;
						}
					}
					
				}
				n--;
			}
		}*/
		//end

		//add to array
		inArray.RemoveAll ();
		for(i=0;i<propArr.GetSize ();i++)
		{
			inArray.Add(propArr[i]->GetAt (0));
		}
		for(j=0;j<propArr.GetSize ();j++)
		{
			propArr.GetAt (j)->RemoveAll ();
			delete propArr.GetAt (j);
		}
	}

	//归一化输入输出数据
	float fMax[64];
	float fMin[64];
	float fOutMax[64];
	float fOutMin[64];
	int nData = inArray.GetSize();
	j=0;
	for(i=0;i<nData;i++)
	{
		if(i==0)
		{
			for(j=0;j<nIn;j++)
			{
				fMax[j] = inArray[0].dataIn [j];
				fMin[j] = inArray[0].dataIn [j];
			}
			for(j=0;j<nOut;j++)
			{
				fOutMax[j] = inArray[0].dataOut [j];
				fOutMin[j] = inArray[0].dataOut [j];
			}
			
		}
		else
		{
		//
			for(j=0;j<nIn;j++)
			{
				if(fMax[j] < inArray[i].dataIn [j]) fMax[j] = inArray[i].dataIn [j];
				if(fMin[j] > inArray[i].dataIn [j]) fMin[j] = inArray[i].dataIn [j];
			}
			for(j=0;j<nOut;j++)
			{
				if(fOutMax[j] < inArray[i].dataOut [j]) fOutMax[j] = inArray[i].dataOut [j];
				if(fOutMin[j] > inArray[i].dataOut [j]) fOutMin[j] = inArray[i].dataOut [j];
			}
		}
	}
	//
	if(nData>1)
	{
		for(j=0;j<nIn;j++)
		{
//			if(fMax[j] < inArray[i].dataIn [j]) fMax[j] = inArray[i].dataIn [j];
//			if(fMin[j] > inArray[i].dataIn [j]) fMin[j] = inArray[i].dataIn [j];
			float fDf = fMax[j]-fMin[j];
			if(fDf!=0)
			{
				m_fInRate[j] = (fMaxInData-fMinInData)/fDf;
				m_fInRateBias[j] = -(fMax[j]+fMin[j])*(fMaxInData-fMinInData)/2/fDf;
			}
			else
			{
				m_fInRate[j] = 1;
				m_fInRateBias[j] = 0;
			}
		}
		for(j=0;j<nOut;j++)
		{
			float fDf = fOutMax[j]-fOutMin[j];
			if(fDf!=0)
			{
				m_fOutRate[j] = (fMaxOutData-fMinOutData)/fDf;
				m_fOutRateBias[j] = -(fOutMax[j]+fOutMin[j])*(fMaxOutData-fMinOutData)/2/fDf;
			}
			else
			{
				m_fOutRate[j] = 1;
				m_fOutRateBias[j] = 0;
			}
		}

		//now normalize
		for(i=0;i<nData;i++)
		{
			for(j=0;j<nIn;j++)
			{
				float f = m_fInRate[j]*inArray[i].dataIn [j]+m_fInRateBias[j] ;
				inArray[i].dataIn [j] = f;
			}
			for(j=0;j<nOut;j++)
			{
				float f = m_fOutRate[j]*inArray[i].dataOut [j]+m_fOutRateBias[j] ;
				inArray[i].dataOut [j] = f;
			}
		}
//		m_fInRate
	}
	//end

	//对输出值取exp
	for(i=0;i<nData;i++)
	{
		for(j=0;j<nOut;j++)
		{
			float f = inArray[i].dataOut [j] ;
			inArray[i].dataOut [j] = RegularData(f);
		}
	}
	return true;//DealInData(inKlineArray &inArray,int nIn,int nOut)
}
/*	2、神经网络预测函数
		
		PRICENEXTDAY		用神经网络预测第二天价格
                   用法:
PRICENEXT (NVALUE),表示第二天价格。
例如：PRICENEXT（0）表示第二天开盘价。

		PRICENEXTMANY		用神经网络预测将来几天价格
                   用法:
PRICENEXT (NVALUE,DAYS),表示将来DAYS天价格。
例如：PRICENEXT（0,5）表示将来5天开盘价。

	3、画线函数
*/

float CTaiKlineNnetBP::RegularData(float fIn, bool bExp)
{
	float f;
	if(bExp == true)
	{
		f = 1/(1+exp(expRateToTrain	*fIn));
	}
	else
	{
		if(fIn>=0.99999)
			fIn = 0.99999;
		f = log(1/fIn-1);//1/(1+exp(expRateToTrain	*fIn));
	}
	return f;
}
