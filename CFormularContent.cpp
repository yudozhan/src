// CFormularContent.cpp : implementation file
//

#include "stdafx.h"
#include "CFormularContent.h"
#include "WH.h"
#include "MainFrm.h"
#include "CTaiShanDoc.h"
#include "CTaiScreenParent.h"
#include "KEYBRODEANGEL.h"
#include "CwdOverWriteF.h"
#include "secretdrvhyt.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CStringArray CFormularContent::m_strArrayKind[4];
bool CFormularContent::m_bVer20 = false;
 
/////////////////////////////////////////////////////////////////////////////
// CFormularContent
IMPLEMENT_SERIAL(CFormularContent,CObject,1)
CFormularContent::CFormularContent()
{
	int i;

	memset(this->btMemData,0,LEN_BYTE);
	for(i=0;i<16;i++)
		nPeriodsUsed[i]=1;	//the kline period that is valid
	isOfen	= FALSE;		//是否是常用指标
	pAdditionalBS = NULL;

	{
		name = _T("");		//技术分析指标名字(最多9个字符)
		password	 = _T("");	//密码
		explainBrief = _T("");	//	指标说明
		explainParam = _T("");	//	参数说明

		for(i=0;i<8;i++)
			namePara[i]	 = _T("");	//参数名字（最多15个字符)
		fomular	 = _T("");		//公式
		help	 = _T("");		//帮助注释

		buyStr   = _T("");       //买入的条件
		sellStr  = _T("");       //卖出的条件
		buyStrKong   = _T("");       //买入的条件
		sellStrKong  = _T("");       //卖出的条件

		subKindIndex = _T("");		//the kind of 技术分析指标,the default value is ""
		subKindIndexTime = _T("");		//the kind of 技术分析指标,the default value is ""
		strReserved = _T("");		//
	}

	bYH = 0;
}

CFormularContent::~CFormularContent()
{
	if(pAdditionalBS != NULL)
		delete pAdditionalBS;
}

void CFormularContent::Serialize(CArchive& ar)
{
	if(m_bVer20 == false)
	{
		if(ar.IsStoring())
		{
			ar<<buyStr;
			ar<<defaultVal[0];
			ar<<defaultVal[1];
			ar<<defaultVal[2];
			ar<<defaultVal[3];
			ar<<explainBrief;
			ar<<fomular;
			ar<<help;
			ar<<isMainFiguer;
			ar<<isOfen;
			ar<<isProtected;
			ar<<isSystem;
			ar<<max[0];
			ar<<max[1];
			ar<<max[2];
			ar<<max[3];
			ar<<min[0];
			ar<<min[1];
			ar<<min[2];
			ar<<min[3];
			ar<<name;
			ar<<namePara[0];
			ar<<namePara[1];
			ar<<namePara[2];
			ar<<namePara[3];
			ar<<numPara;
			ar<<password;
			ar<<posFlag;
			ar<<posX[0];
			ar<<posX[1];
			ar<<posX[2];
			ar<<posX[3];
			ar<<posX[4];
			ar<<posX[5];
			ar<<posX[6];
			ar<<sellStr;
			ar<<stepLen[0];
			ar<<stepLen[1];
			ar<<stepLen[2];
			ar<<stepLen[3];
		}
		else
		{
			ar>>buyStr;
			ar>>defaultVal[0];
			ar>>defaultVal[1];
			ar>>defaultVal[2];
			ar>>defaultVal[3];
			ar>>explainBrief;
			ar>>fomular;
			ar>>help;
			ar>>isMainFiguer;
			ar>>isOfen;
			ar>>isProtected;
			ar>>isSystem;
			ar>>max[0];
			ar>>max[1];
			ar>>max[2];
			ar>>max[3];
			ar>>min[0];
			ar>>min[1];
			ar>>min[2];
			ar>>min[3];
			ar>>name;
			ar>>namePara[0];
			ar>>namePara[1];
			ar>>namePara[2];
			ar>>namePara[3];
			ar>>numPara;
			ar>>password;
			ar>>posFlag;
			ar>>posX[0];
			ar>>posX[1];
			ar>>posX[2];
			ar>>posX[3];
			ar>>posX[4];
			ar>>posX[5];
			ar>>posX[6];
			ar>>sellStr;
			ar>>stepLen[0];
			ar>>stepLen[1];
			ar>>stepLen[2];
			ar>>stepLen[3];
		}
	}
	else
	{
		SerializeData(ar);
	}

}



/////////////////////////////////////////////////////////////////////////////
// CFormularContent message handlers



void CFormularContent::SetData(CFormularContent *data)
{
	int i;
	for(i=0;i<LEN_BYTE;i++)
		btMemData[i]=data->btMemData[i];
	if(pAdditionalBS != NULL)
	{
		isAdditionalCond = 1;
	}
	isAdditionalCond = data->isAdditionalCond;
	if(isAdditionalCond == 1)
	{
		if(pAdditionalBS == NULL)
			pAdditionalBS = new ADDITIONAL_BUYSELL;
		memcpy(pAdditionalBS,data->pAdditionalBS,sizeof(ADDITIONAL_BUYSELL));
	}

	defaultValArray.RemoveAll();
	int nArray =data->defaultValArray.GetSize();
	for(i=0;i<nArray;i++)
	{
		defaultValArray.Add(data->defaultValArray[i]);
	}
	//defaultValArray
	{
		name = data->name;		//技术分析指标名字(最多9个字符)
		password = data->password;	//密码
		explainBrief = data->explainBrief;	//	指标说明
		explainParam = data->explainParam;	//	参数说明

		for(i=0;i<8;i++)
			namePara[i]=data->namePara[i];	//参数名字（最多15个字符)
		fomular=data->fomular;		//公式
		help=data->help;		//帮助注释

		buyStr	=data->buyStr;       //买入的条件
		sellStr	=data->sellStr;       //卖出的条件
		buyStrKong =data->buyStrKong;       //买入的条件
		sellStrKong=data->sellStrKong;       //卖出的条件

		subKindIndex=data->subKindIndex;		//the kind of 技术分析指标,the default value is ""
		subKindIndexTime=data->subKindIndexTime;		//the kind of 技术分析指标,the default value is ""
		strReserved=data->strReserved;		//
	}
}


void CFormularContent::SerializeData(CArchive &ar)
{
	int i;
	if(ar.IsStoring())
	{
		for(i=0;i<LEN_BYTE;i++)
			ar<<btMemData[i];

		int nTemp = 62332442;
		ar<<nTemp;

		if(pAdditionalBS != NULL)
		{
			isAdditionalCond = 1;
		}
		ar<<isAdditionalCond;
		if(isAdditionalCond == 1)
		{
//			ar<<*pAdditionalBS;
			BYTE * pb = (BYTE*)pAdditionalBS;
			for(i=0;i<sizeof(ADDITIONAL_BUYSELL);i++)
			{
				ar<<pb[i];
			}
		}

		BYTE nArray = (BYTE)defaultValArray.GetSize();
		if(nArray!=this->numPara) nArray = 0;

		ar<<nArray;
		for(i=0;i<nArray;i++)
		{
			float*  pf11 = (float*)(&(defaultValArray[i]));
			for(int j=0;j<11;j++)
			{
				ar<<pf11[j];
			}
		}
		//defaultValArray
		{
			ar<<name ;		//技术分析指标名字(最多9个字符)
			CString s2 = password;

			//加密
			SecretForm(s2,false);
			ar<<s2	 ;	//密码
			ar<<explainBrief ;	//	指标说明
			ar<<explainParam ;	//	参数说明

			for(i=0;i<8;i++)
				ar<<namePara[i]	 ;	//参数名字（最多15个字符)

			//加密
			s2 = fomular;
			SecretForm(s2,false);

			ar<<s2	 ;		//公式
			ar<<help	 ;		//帮助注释

			ar<<buyStr   ;       //买入的条件
			ar<<sellStr  ;       //卖出的条件
			ar<<buyStrKong   ;       //买入的条件
			ar<<sellStrKong  ;       //卖出的条件

			ar<<subKindIndex ;		//the kind of 技术分析指标,the default value is ""
			ar<<subKindIndexTime ;		//the kind of 技术分析指标,the default value is ""
			ar<<strReserved ;		//
		}
	}
	else
	{
		for(i=0;i<LEN_BYTE;i++)
			ar>>btMemData[i];
		int nTemp = 62332442;
		ar>>nTemp;


		ar>>isAdditionalCond;
		if(isAdditionalCond == 1)
		{
			if(pAdditionalBS == NULL)
				pAdditionalBS = new ADDITIONAL_BUYSELL;
			BYTE * pb = (BYTE*)pAdditionalBS;
			for(i=0;i<sizeof(ADDITIONAL_BUYSELL);i++)
			{
				ar>>pb[i];
			}
		}

		BYTE nArray ;//= (BYTE)defaultValArray.GetSize();
		ar>>nArray;
		defaultValArray.RemoveAll();
		for(i=0;i<nArray;i++)
		{
			FLOAT11 f11;
			float*  pf11 = (float*)&f11;
			for(int j=0;j<11;j++)
			{
				ar>>pf11[j];
			}
			defaultValArray.Add(f11);
		}

		if(nArray != this->numPara)
		{
			AddDefaultValToArray();
		}

		//消除错误参数
		for(int iIndex = 0;iIndex< numPara ;iIndex++)
		{
			if( defaultValYH [iIndex]> max [iIndex]||
				 defaultValYH [iIndex]< min [iIndex])
				 defaultValYH [iIndex] =  defaultVal [iIndex];
		}

		//defaultValArray

		{
			ar>>name ;		//技术分析指标名字(最多9个字符)

			ar>>password	 ;	//密码
			//解密
			CString s2 = password;
			SecretForm(s2,true);
			password = s2;

			ar>>explainBrief ;	//	指标说明
			ar>>explainParam ;	//	参数说明

			for(i=0;i<8;i++)
				ar>>namePara[i]	 ;	//参数名字（最多15个字符)
			ar>>fomular	 ;		//公式
			s2 = fomular;
			SecretForm(s2,true);
			fomular = s2;

			ar>>help	 ;		//帮助注释

			ar>>buyStr   ;       //买入的条件
			ar>>sellStr  ;       //卖出的条件
			ar>>buyStrKong   ;       //买入的条件
			ar>>sellStrKong  ;       //卖出的条件

			ar>>subKindIndex ;		//the kind of 技术分析指标,the default value is ""
			ar>>subKindIndexTime ;		//the kind of 技术分析指标,the default value is ""
			ar>>strReserved ;		//


			//		ADDITIONAL_BUYSELL * pAdditionalBS;
		//CArray<FLOAT11,FLOAT11&>	defaultValArray;	

		}
	}

}
void CFormularContent::ReadWriteIndex(CTaiShanDoc *pDoc, int nKind, bool bRead)
{
	if(pDoc==NULL)
		return;
	m_bVer20 = false;
	CString sName = g_zbgs_hz;//
	CFormularArray* pArray = &(pDoc->m_formuar_index );
	if(nKind==1)
	{
		sName = g_tixg_hz;
		pArray = &(pDoc->m_formuar_choose) ;
	}
	else if(nKind==2)
	{
		sName = g_kxzh_hz;
		pArray = &(pDoc->m_formuar_kline );
	}
//DeviceIoControl
	try
	{
		if(bRead == false)
		{
			CFile gsfile;

			m_bVer20 = true;
			if(!gsfile.Open(sName+"备份",CFile::modeCreate|CFile::modeWrite))
				return;
			CArchive ar(&gsfile,CArchive::store);
			int length = -1;//=pDoc->m_formuar_index.GetSize();
			ar<<length;
			length=pArray->GetSize();
			int ii = 0;

			//来自服务器的指标不写入文件
			int i;
			for(i = 0;i<length;i++)
			{
				if(pArray->GetAt (i)->nIsFromServer != 33134999)
					ii++;
			}
			ar<<ii;
			for(i=0;i<length;i++)
			{
				CFormularContent * jishu;
				jishu=pArray->GetAt(i);

				//only 1 times
//				jishu->isSystem = 1;
//				jishu->isProtected = 0;

				if(jishu->nIsFromServer != 33134999)
					ar<<jishu;
			}
  
			ar.Close();
			gsfile.Close();

			::DeleteFile (sName);

			CFile::Rename(sName+ "备份", sName);
		}
		else
		{
			CFile gsfile;

			if(!gsfile.Open(sName,CFile::modeRead))
				return;

			int length;//=pDoc->m_formuar_index.GetSize();


			CArchive ar(&gsfile,CArchive::load);
			ar >> length;
			if(length==-1)
			{
				m_bVer20 = true;
				ar>>length;
			}

			m_strArrayKind[nKind].RemoveAll();

//			if(nKind==0)
//				m_strArrayKind[nKind].Add("所有技术指标");

			m_strArrayKind[nKind].Add("其它");

			for(int i=0;i<length;i++)
			{
				CFormularContent * jishu;
				ar>>jishu;
				pArray->Add(jishu);//GetAt(i);

				AddKindName(jishu->subKindIndex , nKind);
			}
  
			ar.Close();
			gsfile.Close();

		}

	}
	catch(...)
	{
	}

	m_bVer20 = false;

}

void CFormularContent::AddKindName(CString s, int nKind)
{
	int j,k;

	bool bFind = false;
	if(s=="")
		return;
	for(j=0;j<m_strArrayKind[nKind].GetSize();j++)
	{
		if(m_strArrayKind[nKind][j]==s)
		{
			bFind = true;
			break;
		}
	}
	if(bFind == false)
	{
		if(m_strArrayKind[nKind].GetSize()<=0)
		{
			m_strArrayKind[nKind].Add(s);
			return;
		}

		for(j=0;j<m_strArrayKind[nKind].GetSize();j++)
		{
			k = s.Compare (m_strArrayKind[nKind][j]);
			if(k<0)
			{
				m_strArrayKind[nKind].InsertAt(j,s);
				return;
			}
		}
		m_strArrayKind[nKind].Add(s);
	}

}
/*
本周期中价
本周期开盘价
本周期最高价
本周期最低价
本周期收盘价
次周期中价
次周期开盘价
次周期最高价
次周期最低价
次周期收盘价

*/

void CFormularContent::InitDefaultValArray()
{
	 if(defaultValArray.GetSize()!=numPara)
	 {
		 defaultValArray.RemoveAll();
		 for(int j=0;j<numPara;j++)
		 {
			 FLOAT11 f11;
			 for(int i=0;i<11;i++)
			 {
				 f11.fValue [i] = defaultVal [j];
			 }
			 defaultValArray.Add(f11);
		 }
	 }

}

void CFormularContent::DeleteKindName(CString s, int nKind)
{
	if(s=="")
		return;
	for(int j=0;j<m_strArrayKind[nKind].GetSize();j++)
	{
		if(m_strArrayKind[nKind][j]==s)
		{
			m_strArrayKind[nKind].RemoveAt(j);
			break;
		}
	}
}

float CFormularContent::GetParamDataEach(int iIndex, int nKlineType, CFormularContent *pJishu)
{
	if(pJishu->bYH!=0) 
	{
		return pJishu->defaultValYH [iIndex];
	}
	float f = pJishu->defaultVal[iIndex];

	//使用保存各个周期数据
//	m_bSaveAllPeriod = ((CTaiShanDoc*)(CMainFrame::m_pDoc))->m_propertyInitiate.bSaveParam;
	if(((CTaiShanDoc*)(CMainFrame::m_pDoc))->m_propertyInitiate.bSaveParam == TRUE)
	{
		if(pJishu->defaultValArray .GetSize()>iIndex)
		{
			int nInd = CTaiScreenParent::FromKlineKindToFoot(nKlineType);
			if(nInd>=0)
				f = pJishu->defaultValArray[iIndex].fValue[nInd];
		}
	}
	return f;
}

void CFormularContent::AddDefaultValToArray()
{
	defaultValArray.RemoveAll();
	FLOAT11 f11;//defaultVal[8];
	for(int i=0;i<this->numPara;i++)
	{
		for(int j=0;j<11;j++)
		{
			f11.fValue[j]=defaultVal[i];
		}
		defaultValArray.Add(f11);
	}

}
//void ADDITIONAL_BUYSELL::ADDITIONAL_BUYSELL()
//{
//}

void CFormularContent::SecretForm(CString &s,bool bLoad)
{
	if(this->isProtected == TRUE)
	{
/*		int nLen = s.GetLength();
		char* pch = s.GetBuffer(nLen);
		for(int ii=0;ii<nLen;ii++)
		{
			char ch = pch[ii];
			ch =~ch;
			if(ch==0) ch = (char)255;
			pch[ii] = ch;
		}
		for(int j = 0;j<nLen-7;j+=7)
		{
			for(int k = 0;k<7/2;k++)
			{
				BYTE  tmp = pch[j+k];
				pch[j+k] = pch[j+6-k];
				pch[j+6-k] = tmp;
			}
		}
		s.ReleaseBuffer();*/

		if(bLoad==false)
		{
			int nLen = s.GetLength()+1;
			int nLen2 = (nLen/8+1)*8;
			char* pch = s.GetBuffer(nLen2+1);

			pch[nLen-1] = '\0';
			int k;
			for(k=nLen;k<nLen2;k++)
			{
				pch[k] = 'a'+rand()%20;
			}
			CSecretDrv2 secr;
			for(k=0;k<nLen2;k+=8)
			{
				secr.secret ((BYTE*)(pch+k), (BYTE*)(pch+k));//= 'a'+rand()%20;
			}

			CString s2;
			for(k=0;k<nLen2;k++)
			{
				CString ss;
				ss.Format("%2x",(BYTE)(pch[k]));
				s2+=ss;//= 'a'+rand()%20;
			}
			pch[nLen2]='\0';

			s.ReleaseBuffer();
			s = s2;
		}
		else
		{
			int nLen = s.GetLength();
			int nLen2 = (nLen/8)*8;
			char* pch = s.GetBuffer(nLen2);
			ASSERT(nLen2 == nLen);

			CString s2;
			CSecretDrv2 secr;
			int k;
			for(k=0;k<nLen2;k+=2)
			{
				BYTE byt;
				byt = (BYTE)secr.From16ToInt(pch+k);
				pch[k/2] = byt;
			}

			for(k=0;k<nLen2/2;k+=8)
			{
				secr.desecret ((BYTE*)(pch+k), (BYTE*)(pch+k));//= 'a'+rand()%20;
			}
			pch[nLen2/2] = '\0';

			s.ReleaseBuffer();
		}

	}

}

bool CFormularContent::IsValid()
{
	CTaiShanDoc*   pDoc=CMainFrame::m_pDoc ;
	if( buyStr .GetLength () >0)
	{
		if( buyStr .CompareNoCase ( pDoc->m_strSerial)!=0)
		{
			return false;
		}
	}
	return true;

}

bool CFormularContent::InstallIndicator(CString sFilePathName,bool bPromp,bool bFromServer)
{
	CTaiShanDoc*   pDoc=CMainFrame::m_pDoc ;
	CString filename=sFilePathName;//m_filedia.GetPathName();
	CFile   file;
	if(file.Open(filename,CFile::modeRead)==0)
	{
		if(bPromp == true)
			AfxMessageBox(filename+"文件不存在");
		//::SetCurrentDirectory (CMainFrame::m_pDoc ->m_CurrentWorkDirectory);
		return false;
	}
	
	CArchive ar(&file,CArchive::load);
	int selectcount,ncurtab;
	ar>>selectcount;
	if(selectcount > 0)
		CFormularContent::m_bVer20 = false;
	else
	{
		CFormularContent::m_bVer20 = true;
		ar>>selectcount;
	}
	
	ar>>ncurtab;
	int readpos=0;
	CFormularArray* pArray[] = {&(pDoc->m_formuar_index ),&(pDoc->m_formuar_choose ),&(pDoc->m_formuar_kline )};

	for(int i=0;i<selectcount;i++)
	{
		BOOL isoverwrite=TRUE;
		BOOL isfind=FALSE;
	
		CFormularContent *jishu=NULL;//new CFormularContent;
		ar>>jishu;

		//来自服务器端的指标
		if(bFromServer == true)
		{
			jishu->subKindIndex = "服务端特色指标";
			jishu->nIsFromServer = 33134999;
			jishu->buyStr = "";
			if(jishu->isProtected == FALSE)
				jishu->isProtected = TRUE;
			if(jishu->password == "")
				jishu->password = "33134999lmb";
		}
		AddKindName(jishu->subKindIndex, ncurtab);

		//test product code
		if(jishu->buyStr .GetLength () >0)
		{
			int nLen2 = jishu->buyStr.GetLength ();
			CString ss = pDoc->m_strSerial;
//			if(nLen2 == 8)
//				ss = g_chMachineID;

			if(jishu->buyStr .CompareNoCase ( ss)!=0)
			{
				if(bPromp == true) AfxMessageBox("此公式不能用于您的软件！");
				continue;
			}
			else
			{
				if(jishu->password .GetLength ()<=0)
					jishu ->isProtected = FALSE;
			}
		}
		//end

		CString name=jishu->name;
		int k,length;
		length=pArray[ncurtab]->GetSize();
		for( k=0;k<length;k++)
		{
			int result=name.Compare(pArray[ncurtab]->GetAt(k)->name);
			if(result>0)          		 
				continue;
			else
				if(result==0)
				{
					isfind=TRUE;
					if(bPromp == true)
					{
						if(AfxMessageBox(name+"公式已存在，是否要覆盖？",MB_YESNO|MB_ICONQUESTION)==IDNO)
							isoverwrite=FALSE;
					}
					
					break;
				}
				else
					break;
		}
		if(isfind)
		{
			if(isoverwrite)
			{
				CFormularContent *jishutodel=pArray[ncurtab]->GetAt(k);
				delete jishutodel;
				pArray[ncurtab]->SetAt(k,jishu);
			}


		}
		else
		{
			pArray[ncurtab]->InsertAt(k,jishu);

			if(ncurtab == 0)
			{
						                   //加入到键盘精灵中
				CString name=jishu->name;
				CTaiKeyBoardAngelDlg dlg;
				dlg.AddIndicatorName(name);
/*				char *namestr=name.GetBuffer(name.GetLength());
				DATA_KEYBOARD *keytoadd=new DATA_KEYBOARD;
				strcpy(keytoadd->key,namestr);
				if(name.GetLength()<=4)
					name+="指标";
				else
					name+="线";
				strcpy(keytoadd->data,name.GetBuffer(name.GetLength()));
				keytoadd->id=3;
				int keylength=pDoc->m_keychardata.GetSize();
				for( int j=0;j<keylength;j++)
				{
					if(strcmp(pDoc->m_keychardata.GetAt(j)->key,namestr)>0)
						break;
				}
				pDoc->m_keychardata.InsertAt(j,keytoadd);*/
			}
		}
	 readpos=k;            //保存更新的位置
	}
	ar.Close();
	file.Close();

	CFormularContent::m_bVer20 = false;

	return true;

}
bool CFormularContent::InstallIndicatorCwd(CString sFilePathName,bool bPromp,bool bFromServer)
{
	CTaiShanDoc*   pDoc=CMainFrame::m_pDoc ;
	CString filename=sFilePathName;//m_filedia.GetPathName();
	CFile   file;
	if(file.Open(filename,CFile::modeReadWrite))
	{
		char ch[32] ;
		file.Read (ch,32);
		ch[31] = 0;
		CString ss = ch;

		if(ss == "YBW SECRET FILE1.0")
		{
			int nLen = file.GetLength ();
			if(nLen<=0)
				nLen = 1;
			BYTE * byt = new BYTE[nLen-32];
			file.Read (byt,nLen-32);
			for(int j = 0;j<nLen-7-32;j+=7)
			{
				for(int k = 0;k<7/2;k++)
				{
					BYTE  tmp = byt[j+k];
					byt[j+k] = byt[j+6-k];
					byt[j+6-k] = tmp;
				}
			}
			file.SeekToBegin ();
			file.Write (byt,nLen-32);

		}
	}
	else
	{
		if(bPromp == true)
			AfxMessageBox("文件不存在或文件只读！请把文件设为可写属性！");
		return false;
	}
	file.SeekToBegin ();
	CArchive ar(&file,CArchive::load);
	int selectcount,ncurtab;
	ar>>selectcount;

	int nTotalKind = 1;
	if(selectcount > 0)
	{
		CFormularContent::m_bVer20 = false;
		ar>>ncurtab;
	}
	else
	{
		nTotalKind = 3;
		CFormularContent::m_bVer20 = true;
		ar>>selectcount;
		ar>>selectcount;
		ar>>ncurtab;
	}
	
	CFormularArray* pArray[] = {&(pDoc->m_formuar_index ),&(pDoc->m_formuar_choose ),&(pDoc->m_formuar_kline )};

	int isoverwrite=0;
	for(int k = 0;k<nTotalKind;k++)
	{
		int readpos=0;
		for(int i=0;i<selectcount;i++)
		{
			if(isoverwrite == 0)
				isoverwrite=1;
			BOOL isfind=FALSE;
		
			CFormularContent *jishu=NULL;//new CFormularContent;
			ar>>jishu;

			//来自服务器端的指标
			if(bFromServer == true)
			{
				jishu->subKindIndex = "服务端特色指标";
				jishu->nIsFromServer = 33134999;
				jishu->buyStr = "";
				if(jishu->isProtected == FALSE)
					jishu->isProtected = TRUE;
				if(jishu->password == "")
					jishu->password = "33134999lmb";
			}
			AddKindName(jishu->subKindIndex, ncurtab);

			//test product code
			if(jishu->buyStr .GetLength () >0)
			{
				int nLen2 = jishu->buyStr.GetLength ();
				CString ss = pDoc->m_strSerial;
	//			if(nLen2 == 8)
	//				ss = g_chMachineID;

				if(jishu->buyStr .CompareNoCase ( ss)!=0)
				{
					if(bPromp == true) AfxMessageBox("此公式不能用于您的软件！");
					continue;
				}
				else
				{
					if(jishu->password .GetLength ()<=0)
						jishu ->isProtected = FALSE;
				}
			}
			//end

			CString name=jishu->name;
			int k,length;
			length=pArray[ncurtab]->GetSize();
			for( k=0;k<length;k++)
			{
				int result=name.Compare(pArray[ncurtab]->GetAt(k)->name);
				if(result>0)          		 
					continue;
				else
					if(result==0)
					{
						isfind=TRUE;
						if(bPromp == true && isoverwrite <2)
						{
							CFxjOverWriteF dlg;
							dlg.m_s1 = name+"公式已存在，是否要覆盖？";
							dlg.DoModal ();
							isoverwrite = dlg.m_nResponse ;
//
//							if(AfxMessageBox(name+"公式已存在，是否要覆盖？",MB_YESNO|MB_ICONQUESTION)==IDNO)
//								isoverwrite=FALSE;
						}
						
						break;
					}
					else
						break;
			}
			if(isfind)
			{
				if(isoverwrite)
				{
					CFormularContent *jishutodel=pArray[ncurtab]->GetAt(k);
					delete jishutodel;
					pArray[ncurtab]->SetAt(k,jishu);
				}


			}
			else
			{
				pArray[ncurtab]->InsertAt(k,jishu);

				if(ncurtab == 0)
				{
											   //加入到键盘精灵中
					CString name=jishu->name;
					CTaiKeyBoardAngelDlg dlg;
					dlg.AddIndicatorName(name);
				}
			}
		 readpos=k;            //保存更新的位置
		}

		if(k == nTotalKind-1) break;
		ar>>selectcount;
		ar>>ncurtab;
	}
	ar.Close();
	file.Close();

	CFormularContent::m_bVer20 = false;

	return true;

}


bool CFormularContent::ExportFormular(CFormularArray *pArr, int nArr,CString fileName,BOOL bCompleteSecret, BOOL bPassword, CString strPassword)
{
	int nTotal = 0;
	int i;
	for(i = 0;i<nArr;i++)
	{
		nTotal+=pArr[i].GetSize ();
	}
	if(nTotal==0)
		return false;
	CTaiShanDoc*   pDoc=((CMainFrame*)(AfxGetApp()->m_pMainWnd))->m_pDoc ;

    CString     defaultname="";              //对话框的缺省名字

	int ncurtab=0;
	CString sArray="*.ts";        //指标公式   //条件选股公式 //K线模式公式
	CFormularContent::m_bVer20 = false;
	{
		CString filename=fileName;
		if(filename.Find('.') == -1)
		{
	      CString temp = sArray;
		  temp.TrimLeft ("*");
		  filename+=temp;
		}

		CFile   file;
		if(file.Open(filename,CFile::modeRead)!=0)
		{
			if(AfxMessageBox(filename+"文件已存在，要覆盖吗？",MB_YESNO|MB_ICONQUESTION)==IDNO)
			{
				file.Close();
				//::SetCurrentDirectory (CMainFrame::m_pDoc ->m_CurrentWorkDirectory);
				return false;
			}
			file.Close();
		}
		CFormularContent::m_bVer20 = true;
		file.Open(filename,CFile::modeCreate|CFile::modeWrite);
		CArchive ar(&file,CArchive::store);
		int selectcount=0;
		ar<<selectcount;
		ar<<selectcount;

		int nOut = 0;
		CFormularArray* pArray = pArr;

		int nOutAll = 0;
		for(int j =0;j<nArr;j++)
		{
			nOut = 0;
			for(i=0;i<pArray[j].GetSize ();i++)
			{
				CFormularContent *jishu;
				jishu=pArray[j].GetAt(i);          //指标公式			 

				CString buyStr = jishu->buyStr;
				if(buyStr=="" && jishu->sellStr == "")
					nOut++;
			}
			nOutAll+=nOut;

			ar<<nOut;
			ar<<j;
			for(i=0;i<pArray[j].GetSize ();i++)
			{
				CFormularContent *jishu;
				jishu=pArray[j].GetAt(i);          //指标公式			 

				CString buyStr = jishu->buyStr;
				if(buyStr=="" && jishu->sellStr == "")
				{
					int isProt = jishu->isProtected;
					CString sP = jishu->password;
					CString sOut = jishu->sellStr ;
					if(bPassword)
					{
						if( jishu->isProtected == 0 )
						{
							jishu->isProtected = 1;
							jishu->password = strPassword;
						}
					}
					if(bCompleteSecret)
					{
						jishu->sellStr = "dls";
						jishu->isProtected = 1;
						if(jishu->password == "")
						{
							for(int k = 0; k<108; k++)
							{
								char ch = (char)((rand()+128)%255+1);
								CString s(ch);
								
								jishu->password += s;
							}
						}
					}

					ar<<jishu;
					jishu->isProtected = isProt;
					jishu->password  = sP;
					jishu->sellStr  = sOut;
				}
			}
		}

		CString lengthstr;
		lengthstr.Format("%d",nOutAll);
		if(nTotal>nOutAll)
			AfxMessageBox("部分完全加密的公式不允许导出！");
		AfxMessageBox("一共在文件里写了"+lengthstr+"个公式");
		ar.Close();

		file.Close();
		CFormularContent::m_bVer20 = false;

		if(nOutAll<=0) 
			return false;
		if(bCompleteSecret)
		{
			if(file.Open(filename,CFile::modeReadWrite))
			{
				int nLen = file.GetLength ();
				if(nLen<=0)
					nLen = 1;
				BYTE * byt = new BYTE[nLen];

				file.Read (byt,nLen);
				for(int j = 0;j<nLen-7;j+=7)
				{
					for(int k = 0;k<7/2;k++)
					{
						BYTE  tmp = byt[j+k];
						byt[j+k] = byt[j+6-k];
						byt[j+6-k] = tmp;
					}
				}

				char ch[32] = "YBW SECRET FILE1.0";
				file.SeekToBegin ();
				file.Write (ch,32);
				file.Write (byt,nLen);

				file.Close();
			}

		}
	}

	return true;
}

CString CFormularContent::GetFormular()
{
	CString sFormular;
	if( buyStr != "")
		sFormular = "";
	else if(sellStr != "" )
		sFormular = "";
	else if(isProtected )
		sFormular = "此公式被密码保护！";
	else 
		sFormular  = fomular  ;
	return sFormular;

}
