//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "StructTaiShares.h"
#include "CFormularComputeParent.h"
//#include "CTaiShanApp.h"
#include "CTaiShanDoc.h"
#include "MainFrm.h"
#include "CTaiKlineFileHS.h"
#include "CTaiKlineFileKLine.h"
#include "CTaiKlineTransferKline.h"
#include "TechNnetBP.h"
#include "CTaiKlineMoveChenben.h"
#include "CTaiKlineDlgHistorySelect.h"
#include "XgnExtDataDlg.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
#define StrToLong StrToInt

CMapStringToPtr CFormularComputeParent::m_LetterTable;	//字母表
CMap<int,int,int,int> CFormularComputeParent::m_ParameterTable;//函数参数个数表
CMapStringToPtr CFormularComputeParent::m_WordTable;	//函数表
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFormularComputeParent::CFormularComputeParent()
{
	m_bRightbox = false;
	m_MaxPoint =0;
	m_bShowColor = false;
	m_stkKind=0;
	m_nCurrentFoot = 0;
}

CFormularComputeParent::CFormularComputeParent(int pmin,int pmax,Kline* pp,CString& str)
{ 
	m_bShowColor = false;
	m_stkKind = 0;
	InitEquation( pmin,  pmax, pp, str);
}
//static TransferHs(CBuySellList* pBuySellList,ARRAY_BE& pp,int nMax,int nKindIn,int nOther)

CFormularComputeParent::CFormularComputeParent(int pmin,int pmax,Kline* pp,CFormularContent* pIndex,CString symbol,int stkKind,CBuySellList* pBuySellList,int nKindKline,bool bAddBuySell)
//bAddBuySell 为是否屏蔽流通盘分布类函数错误

{ 
	m_stkKind = stkKind;
	m_bShowColor = false;
	CString str = pIndex->fomular ;
//	if(bAddBuySell == true)
	{
	}
	str.MakeLower ();
	InitEquation( pmin,  pmax, pp, str);
	if(pp == NULL)
	{
		m_MinPoint = 0;
		m_MaxPoint = pBuySellList->GetCount ();
	}

	m_pIndex = pIndex;
	m_symbol =symbol;
	m_pBuySellList = pBuySellList;
	m_nKindKline =nKindKline;
	m_pDoc->m_sharesInformation .Lookup (this->m_symbol.GetBuffer(0),this->m_pReportData,stkKind);
	m_symbol.ReleaseBuffer();
}

CFormularComputeParent::~CFormularComputeParent()
{
	int i;

	if(m_pKindPos) delete[] m_pKindPos;

	while(m_table.pop_it()){};
	
	//m_bCreateBsList
	if(	m_bCreateBsList == true )
	{
		CTaiKlineFileHS::RemoveHs(*m_pBuySellList);
		delete m_pBuySellList;
	}

	if(m_pKlineIndex!=NULL)
		delete[] m_pKlineIndex;

	m_bTotalData=false;

	for(i=0;i<m_RlineNum;i++)
		delete[]  m_Rline[i].line;
	for(i=0;i<m_VarNum;i++)
		delete[]  m_VarArray[i].line;
	for(i=0;i<m_MidNum;i++)
		delete[]  m_MidArray[i].line;

	m_nExec = 0;

	if(m_pKlineEx != NULL) 
		delete[] m_pKlineEx;

}
int CFormularComputeParent::CheckWord(CString eqm)//词法检查
{
	if(m_bShowColor == true)
	{
		if(m_pKindPos == NULL) 
			m_pKindPos = new KindPos[MAXB_ALL*4];
		if(m_pKindPos == NULL) 
			return 0;
//		m_nPosPre = 0;
//		m_nSizeKindPos = 0;
	}

	CString temp;
	int Bm =0;
	double x;
	char ch,chs;
	char chl[20],*chj;
	int  cl,hj,j,cls,fl;
	int  ck;
	int  mj=eqm.GetLength();
	int rtn;//lmb
	int i;
	for(i=0;i<mj;i++)
	{
		if(m_pKindPos != NULL)
		{
			m_pKindPos[m_nSizeKindPos].m_nPos = i+m_nPosPre;
		}

		ch=eqm.GetAt(i);
		cl=GetCharClass(ch);
		switch(cl)
		{
		case 1:
			m_strErrMsg="非法字符";
			return i;
			break;
		case 2:
			break;
		case 3:
			m_Position[Bm] = i;
			fl=0;
			if(ch=='.')
			{
				fl = 1;
//				m_strErrMsg="非法字符";
//				return i;
			}
			for(ck=0;ck<20;ck++)	
				chl[ck] = ' ';
			j=i;
			chl[0]=ch;
			while(++j<mj)
			{
				chs=eqm.GetAt(j);
				if (chs=='.') 
					fl=1;
				cls=GetCharClass(chs);
				if(cls!=3)  
					break;
				chl[j-i]=chs;				
			};
			i=--j;
			if(fl==1)
			{
				if(m_floatNum <= CEFE-CEFB)
				{
					x = strtod(chl, &chj);
					m_floatArray[m_floatNum]=(float)x;
					m_table.push_b(m_floatNum+CEFB);
					m_floatNum++;
					Bm++;
				}
				else
				{
					m_strErrMsg="浮点数个数超出";
					return i;
				}
			}
			else
			{
				if(m_intNum <= CEIE - CEIB)
				{
					x = strtod(chl, &chj);			
					m_intArray[m_intNum]=(int)x;
					m_table.push_b(m_intNum+CEIB);
					m_intNum++;
					Bm++;
				}
				else
				{
					m_strErrMsg="整数个数超出";
					return i;
				}
			}
			if(m_pKindPos != NULL)
			{
				m_pKindPos[m_nSizeKindPos].m_nKind = 0;
			}

			break;
		case 4:
			m_Position[Bm] = i;
			temp.Empty();
			temp.Insert(0,ch);
			j=i;
			while(++j<mj)
			{
				chs=eqm.GetAt(j);
				cls=GetCharClass(chs);
				if(cls!=3 && cls!=4) 
					break;
				temp.Insert(j-i,chs);				
			};
			
			if(!LookupWordTable(temp,hj))
			{

				while(j<mj&&eqm.GetAt(j)==' ')
					j++;

				if(j<mj)
				{
					if(Bm==0  && eqm.GetAt(j) == ':') 
					{
						if(temp.GetLength()>36)
						{
							m_strErrMsg="中间变量名不能大于36位";
							return i;
						}
						if(j == mj-1)
						{
							m_strErrMsg="结束字符为非法字符！";
							return i;
						}
						else
						{
							if(eqm.GetAt(j+1) == '=')
							{
								if(m_VarNum <= MIDE-MIDB)
								{
									m_WordTableNew[temp]=(void*)(MIDB+m_VarNum);
									m_VarArray[m_VarNum].kind = ARRAY_BE::Normal;
									m_VarArray[m_VarNum].line = new float[m_MaxPoint];
									m_table.push_b(MIDB+m_VarNum);
									m_VarName[m_VarNum] = temp;					
									m_VarNum++;
									Bm++;
								}
								else
								{
									m_strErrMsg="中间变量个数超出";
									return i;
								}
							}
							else
							{
								if(m_RlineNum <= RLINEE-RLINEB)
								{
									m_WordTableNew[temp]=(void*)(RLINEB+m_RlineNum);
									m_Rline[m_RlineNum].kind = ARRAY_BE::Normal;
									m_Rline[m_RlineNum].line = new float[m_MaxPoint];
									m_table.push_b(RLINEB+m_RlineNum);
									m_RlineName[m_RlineNum] = temp;
									m_RlineNum++;
									Bm++;
								}
								else
								{
									m_strErrMsg="返回变量个数超出";
									return i;
								}
							}
						}
					}
					else
					{
						if(temp.GetLength () == 11 && temp.Left(5) == "color")
						{
							m_table.push_b(m_strColorNum+CONSCOLORB);
							CString s2 = temp;
							s2.Delete (0,5);
							m_strColor[m_strColorNum] = s2;
							m_strColorNum++;
							Bm++;
							i=j;
							if(m_pKindPos != NULL)
							{
								m_pKindPos[m_nSizeKindPos].m_nKind = 4;
							}
						}
						else
						{
							m_strErrMsg="非法变量";
							return i;
						}
/**/				}
				}
				else
				{
						if(temp.GetLength () == 11 && temp.Left(5) == "color")
						{
							m_table.push_b(m_strColorNum+CONSCOLORB);
							CString s2 = temp;
							s2.Delete (0,5);
							m_strColor[m_strColorNum] = s2;
							m_strColorNum++;
							Bm++;
							i=j;
							if(m_pKindPos != NULL)
							{
								m_pKindPos[m_nSizeKindPos].m_nKind = 4;
							}
						}
						else
						{
							m_strErrMsg="非法变量";
							return i;
						}
				}
				if(m_pKindPos != NULL)
				{
					m_pKindPos[m_nSizeKindPos].m_nKind = 2;
				}
				
			}
			else
			{
				while(j<mj&&eqm.GetAt(j)==' ')
					j++;

				if(j<mj && Bm==0  && eqm.GetAt(j) == ':') 
				{
					if(hj>=MIDB)
					{
						m_strErrMsg="变量重复定义或与参数同名";
						return i;
					}
				}

				if(hj==8034 || hj == 8046 
					|| hj>=FuncDllB+21 && hj<=FuncDllB+24 
					|| hj>=FuncDllB && hj<=FuncDllB+2
					|| hj==FuncDllB+5 || hj==FuncDllB+6
					|| hj>=FuncDllB+10 && hj<=FuncDllB+14
					|| hj == FuncDllB+34
					|| hj == FuncDllB+4)
					m_bTotalData=true;

				

				m_table.push_b(hj);
				Bm++;
				if(m_pKindPos != NULL)
				{
					m_pKindPos[m_nSizeKindPos].m_nKind = 3;
				}
			}
			i=--j;
			break;
		case 5:
			m_Position[Bm] = i;
			switch(ch)
			{
			case '>':
			case '<':
			case '=':
			case '!':
			case ':':
				if(i==eqm.GetLength()-1)
				{
					m_strErrMsg = "非法结束 ";
					return i;

				}
				chs=eqm.GetAt(i+1);
				if(chs=='=') 
				{
				//	yhh add 995629 
					if(ch=='=')
					{
						m_Position[Bm] = i+1;
						m_strErrMsg = "非法运算符";
						return i;
					}

					if(ch==':')
					{
						int pre;
						pre=m_table.pop_it();
						if(pre>=MIDB && pre<=MIDB+m_VarNum)
							m_table.push_b(pre);
						else
						{
							if(pre>=FuncB && pre<=PAM)
								m_strErrMsg ="中间变量不能用系统保留字";
							else
								m_strErrMsg ="中间变量不合法";
							return i;
						}

					}
				//yhh add end 
					temp.Empty();
					i++;
					if(i==mj) 
					{	
						m_strErrMsg = "非法结束 ";
						return i;
					}
					temp.Insert(0,ch);
					temp.Insert(1,chs);
					LookupWordTable(temp,hj);
					m_table.push_b(hj);
					Bm++;
				}
				else if(chs=='<' ||chs=='>') 
				{
					if(ch!='>' && chs=='<' || ch!='<' &&chs=='>')
					{
						m_Position[Bm] = i+1;
						m_strErrMsg = "非法运算符";
						return i;
					}

					temp.Empty();
					i++;
					if(i==mj) 
					{	
						m_strErrMsg = "非法结束 ";
						return i;
					}
					temp.Insert(0,ch);
					temp.Insert(1,chs);
					LookupWordTable(temp,hj);
					m_table.push_b(hj);
					Bm++;
				}
				else
				{
					if(ch=='!')  
					{	
						m_strErrMsg = "非法结束 ";
						return i;
					}
					else
					{
						//	yhh add 995629 
						if(ch==':')
						{
							int pre;
							pre=m_table.pop_it();
							if(pre>=RLINEB&&pre<=RLINEB+m_RlineNum)
								m_table.push_b(pre);
							else
							{
								if(pre>=FuncB&&pre<=PAM)
									m_strErrMsg ="中间变量不能用系统保留字";
								else
									m_strErrMsg ="中间变量不合法";
								return i;
							}

						}
					// yhh add 995629 end
						temp.Empty();
						temp.Insert(0,ch);
						LookupWordTable(temp,hj);
						m_table.push_b(hj);
						Bm++;
					}
				}
				break;
			case '-':
			case '+':
				rtn=m_table.pop_it();
				if( rtn==0 || (rtn==9001) || (rtn==9005)
					|| (rtn>=1001 && rtn<=4008))//lmb6.13
				{
					if(rtn>0)
						m_table.push_b(rtn);
					m_table.push_b(TOZERO);
					Bm++;
				}
				else if( (rtn==6001 || rtn==6002) && ch == '-' )//lmb6.13
				{
					temp.Empty();
					if(rtn == 6001)//*
						temp = "*-";
					else
						temp = "/-";
					if(!LookupWordTable(temp,hj))
					{
						m_strErrMsg="非法字符";
						return i;
					}
					m_table.push_b(hj);
					break;
				}
				else
				{
					m_table.push_b(rtn);
				}
			default:
					temp.Empty();
					temp.Insert(0,ch);
					if(!LookupWordTable(temp,hj))
					{
						m_strErrMsg="非法字符";
						return i;
					}
					m_table.push_b(hj);
					Bm++;
					break;
			}
			if(m_pKindPos != NULL)
			{
				m_pKindPos[m_nSizeKindPos].m_nKind = 1;
			}

			break;	
		case 6://处理引用
			
			m_Position[Bm] = i+1;// m_Position[Bm]记录变量在子串中位置
			m_QuoteArray[m_QuoteNum].Empty();
			j=i;
			fl=0;
			while(++j<mj)//mj字符串总长
			{
				chs=eqm.GetAt(j);
				cls=GetCharClass(chs);
				if(cls>1 && cls<6)
				{
					m_QuoteArray[m_QuoteNum].Insert(j-i,chs);
					continue;
				}
				if(cls==6)
				{
					fl=1;
					break;
				}
				if(cls==1)
				{
					m_strErrMsg="非法字符";
					return i;
				}
			};
			if(fl==0)
			{
				m_strErrMsg="引用无匹配符";
				return i;
			}

			m_table.push_b(m_QuoteNum+QUB);
			m_QuoteNum++;
			Bm++;
			i=j;
			if(m_pKindPos != NULL)
			{
				m_pKindPos[m_nSizeKindPos].m_nKind = 4;
			}
			break;
		case 7://处理字符串
			
			m_Position[Bm] = i+1;// m_Position[Bm]记录变量在子串中位置
			m_stringArray[m_stringNum].Empty();
			j=i;
			fl=0;
			while(++j<mj)//mj字符串总长
			{
				chs=eqm.GetAt(j);
				cls=GetCharClass(chs);
				if(cls>1 && cls<6)
				{
					m_stringArray[m_stringNum].Insert(j-i,chs);
					continue;
				}
				if(cls==7)
				{
					fl=1;
					break;
				}
				if(cls==1)
				{
					m_strErrMsg="非法字符";
					return i;
				}
			};
			if(fl==0)
			{
				m_strErrMsg="无匹配符";
				return i;
			}

			m_table.push_b(m_stringNum+CECB);
			m_stringNum++;
			Bm++;
			i=j;
			if(m_pKindPos != NULL)
			{
				m_pKindPos[m_nSizeKindPos].m_nKind = 4;
			}
			break;
		}

		m_nSizeKindPos++;
	}
//============出栈==清空链表===================
	m_SentNum=-1;
	do 
	{	m_SentNum++;
		m_Sent[m_SentNum]=m_table.rec_it();
		i=m_Sent[m_SentNum];
	}while(i);
	m_SentNum--;
//---		lmb
	IsDaPan();
//yhh add
	FindLineType()  ;//计算返回线的类型
	return -1;
}

int CFormularComputeParent::GetCharClass(char ch)
{   
	int hj;
	CString tj(ch);

    if(m_LetterTable.Lookup(tj,(void *&)hj)) 
		return hj;
	else 
	{
//		if((ch&0x80) == 0)
//			return 1;
//		else
			return 4;
	}
}
int CFormularComputeParent::YhhCheck(int j, int k)
{
	int tj=0,tu=0;
	int k0=k;
	int i;
	for(i=j;i<=m_SentNum;i++)
	{
		if(tu<0) 
		{
			m_strErrMsg="缺少参数";    
			return i;
		}
		if(tu>1) 
		{
			m_strErrMsg="缺少运算符";    
			return i;
		}

		tj=m_Sent[i];
		if(tj==9001)
		{
			k++;   continue;
		}
		if(tj==9002) 
		{
			k--;
//			if(k==k0)            //?
//			tu+=1;//tu 
     		if((k==0||k==k0) && tu!=1)        //?    //wjy 的代码 
			{
				m_strErrMsg="非法运算";              //wjy 的代码
				return i;
			}
				continue;
		}
		if(tj==9005)            //yhh 是","
		{
			if(tu!=1) 
			{
				m_strErrMsg="非法字符";
				return i;
			}
 			return -1;            //-1代表，退出
		}
		if( (tj>FuncB && tj<=FuncE))     //? k>k0 ||
		{
			if(tj>=QUB&&tj<=FuncE&&i<=m_SentNum)
			{
				if(m_Sent[i+1]!=9001)
					tu+=1;
			}
			continue;
		}
   	   if(tj<FuncB)
			tu-=1;
		if(tj>CONSE||tj==TOZERO)
			tu+=1;
		
	}
	if(tu!=1 && j!=m_SentNum)
	{
		if(tu>1)
			m_strErrMsg="缺少运算符";
		else
			m_strErrMsg="缺少参数";

		return i-1;
	}
	else 
		return -1;

}

int CFormularComputeParent::CheckSentence()
{
	int i=0,k=0;
	int tj=0,tu=0;              //yhh  tu :参数的个数?
//	int fl=0,j=0;
	CString str,cname,cline;

	int start;
	for(start=0;start<=m_SentNum;start++)
	{
		tj=m_Sent[start];
		if(tj==9001)
			k++;
		else if(tj==9002)
		{
			k--;
			if(k<0)
			{
				m_strErrMsg="括号不匹配,多)";
				return start;
			}
		}
	}
	if(start>=m_SentNum&&k>0)
	{
		m_strErrMsg="括号不匹配,多(";
		return start;	
	}
	int check=YhhCheck(0,k);
	if(check!=-1)
		return check;
	for(i=0;i<=m_SentNum;i++)
	{	tj=m_Sent[i];
		int m;

		if( tj==9005) //(	,
		{
			int check=YhhCheck(i+1,k);
			if(check==-1) continue;
			else 
				return check;
		}

	                        	
		if(tj==9001 ) 
		{
		   k++;
		  int check=YhhCheck(i+1,k);
		  if(check==-1) continue;
		  else 
				return check;

		}
		if(tj==9002)
		{
		k--;
		}
					
		if(tj>FuncB && tj<=FuncE)//800	890
		{
		
				//---	找参数个数
			if(tj>=QUB)              //yhh 如果是引用
			{
				str =m_QuoteArray[tj-QUB];
				int nKindE;
				int rtn= GetQuote(str,cname,cline,tu,nKindE,nKindE);//tu参数个数
				if(rtn==1)
				{
					m_strErrMsg="引用的指标不存在";
					return i;	
				}

				if(rtn==2)
				{
					m_strErrMsg="引用的指标的子项不存在";
					return i;	
				}

				m_ParameterTableNew[tj] = tu;
				//lmb 20041123 add 
//				if(i<m_SentNum && m_Sent[i+1]!=9001) m_ParameterTableNew[tj]=0;

			}
			else
			{
					if(!LookupParameterTable(tj,tu)) 
					tu=1;
			}

					//---		
			 int   kk=0;//---k	括号匹配标记   	//wjy
			 if(m_Sent[i+1]!=9001)  //yhh 函数后面不是(
			 {
				 if(tj<QUB)
				 {
					m_strErrMsg="缺括号";
					return i;
				 }
				 //lmb 2004.3.8
				 else
				 {
					 m_ParameterTableNew[tj] = 0;
					 tu =0;
					 continue;
				 }
			 }
			 else 
					kk++;
			 m=i+2;   //YHH  跳过函数名和函数名旁边的(
			 do{    // yhh 这个循环检查函数内的参数是否正确
				tj=m_Sent[m];
				if(tj==9005 && kk==1) //yhh 括号匹配且出现","
					tu-=1;
				if(tj==9001) {kk+=1;}  //yhh 出现(
				if(tj==9002)          //yhh 出现)
				{
					kk-=1;
					if(kk==0 ) //yhh k<0 代表已找到最后的一个)     
					{
						if(tu==1)	break;  //yhh  正常返回
						else
						{
							m_strErrMsg="参数个数有误";
							return i;
						}
					}
				}
				m++;
			 
			 }while(m<=m_SentNum);//do语句结束
		
		}           //if语句结束
		                           
		
	}
	return -1;
							
		

}

int CFormularComputeParent::Sort(int bi,int ej)//运算顺序确定
{
	int tj,tu;
	int lb,le;//lb
	int i=0,j=0,k;
	i=0;//

	//lmb6.13
	if(bi>ej)
		return 0;

	//处理括号,
	while(m_Sent[bi]==9001 && m_Sent[ej]==9002)//9001(,	9002)
	{
		i=bi+1;
		k=0;
		do{
			if(m_Sent[i]==9001)  k++;
			if(m_Sent[i]==9002)  k--;
			if(k<0)  break;
		}while(++i<ej);

		if(k==0)
		{
			bi++;
			ej--;  
			if(bi>=ej)  
			{
				if(bi==ej) m_table.push_b(m_Sent[bi]);
				else ASSERT(FALSE);
				return 0;
			}
			else
				continue;
		}
		else
			break;
	}


	j=0; 
	tj = 10;//最高优先级
	//---		判断最低优先级 
/*	for(k=bi;k<=ej;k++)
	{
		if(m_Sent[k]==9001) {j++;continue;}
		if(m_Sent[k]==9002) j--;
		if(j==0)
		{
			tu=m_Sent[k]/100;
			if(tu<tj) {tj=tu; i=k;}//i
		}
	}*/

	//---找到完整的优先级最低的位置
	for(k=bi;k<=ej;k++)
	{
		if(m_Sent[k]==9001) {j++;continue;}
		if(m_Sent[k]==9002) j--;
		if(j==0)
		{
			tu=m_Sent[k]/1000;
			if(tu<=tj) {tj=tu; i=k;}//i
		}
	}
	//	lmb6.13
	if(bi==ej)
	{
		tj=m_Sent[bi];
		m_table.push_b(tj);
		return 0;
	}

    
	tj=m_Sent[i];
	//---		当为函数时
	if(tj>FuncB && tj<FuncE) 
	{
		m_table.push_b(tj);
		if(tj>=QUB)
		{
//			int nNext = 
			tu=GetNum(tj);//lmb	tu参数个数
		}
		else
		{
			if(!LookupParameterTable(tj,tu)) tu=1;//没有找到则参数个数为1
		}
		lb=i+2;//

		if(tu>0)//tu==0时，不需压入参数
		{
			while(tu--)
			{
				k=0;
				le=lb;
				while(1)
				{
					if(m_Sent[le]==9001) k++;
					if(m_Sent[le]==9002) k--;
					if(k<0 || (k==0 && m_Sent[le] ==9005)) 
					{
						le--;
						break;
					}
					le++;
				}
				if(lb==le)
					m_table.push_b(m_Sent[lb]);
				else
					Sort(lb,le);
				lb=le+2;
			}
		}
	}

	//---		双目操作符时
	if(tj<FuncB)
	{
		m_table.push_b(tj);
		lb=bi;//左边开始点
		le=i-1;//左边结束点
		if(lb==le)
			m_table.push_b(m_Sent[lb]);
		else
			Sort(lb,le);
		lb=i+1;//右边开始点
		le=ej;//右边结束点
		if(lb==le)
			m_table.push_b(m_Sent[lb]);
		else
			Sort(lb,le);
	}
	return 0;
}

//===========分割语句==============//
void CFormularComputeParent::Devide()
{
	int i,tu;
	int lb=0,le=0;//lb记录上一子句的结束
	int Rvdev,el;
	char ch;
	CString ts;//?
	//lmb 20040907
	m_nPosPre = 0;


	if (m_MaxPoint==0) 
		return;
	tu=m_formuCompute.GetLength();
	m_ComSortNum = -1;
	m_nErrPos = -1;
	m_tempWhich=-1;
	for(i=0;i<tu;i++)
	{
		ch=m_formuCompute.GetAt(i);
		ts.Insert(i-lb,ch);
		if(ch==';' || i==tu-1)
		{
			if(i-lb==0)
			{
				m_strErrMsg="非法字符";
				m_nErrPos= i;
				break;
			}
			if(ch==';')	 ts.Delete(i-lb,1);
			Rvdev = CheckWord(ts);
			if(Rvdev != -1)
			{
				m_nErrPos= Rvdev+lb;
				break;
			}
			Rvdev = CheckSentence();

			//---		lmb
//			IsDaPan();
//			FindLineType();
			//---
			if(Rvdev != -1)
			{
				m_nErrPos=m_Position[Rvdev]+lb;
				break;
			}


//===========================================//
			Rvdev = Sort(0,m_SentNum);
			do {
				m_ComSortNum++;
				m_ComSort[m_ComSortNum]=m_table.pop_it();
				el=m_ComSort[m_ComSortNum];
			}while(el);
			m_ComSortNum--;
			ts.Empty();
			lb=i+1;

			//lmb 20040907
			m_nPosPre = i+1;
		}
	}

	for(i=0;i<this->m_RlineNum ;i++)
		if(GetPeriodKind(this->m_RlineName[i])!=0)
		{
			m_nErrPos = 0;
			m_strErrMsg="返回变量名称不能用系统关键字";
			break;
		}
}


int CFormularComputeParent::AddPara(CString pa,float va)//增加浮点参数
{
	if(m_NumGs >=PRAMANUM)
		return 1;
//yhh add 990702
	int hj;
	if(LookupWordTable(pa,hj))
		return 2;
		
//yhh add 990702 end
	m_WordTableNew[pa] =(void *)(m_floatNum+CEFB);
	m_floatArray[m_floatNum]=va;
	m_NumName[m_NumGs] = pa;
	m_NumGs++;
	m_floatNum++;
	return 0;
}

int CFormularComputeParent::AddPara(CString pa, int va)//增加整型参数
{
	if(m_NumGs >=PRAMANUM)
		return 1;
//yhh add 990702
	int hj;
	if(LookupWordTable(pa,hj))
		return 2;
		
//yhh add 990702 end
	m_WordTableNew[pa] =(void *)(m_intNum+CEIB);
	m_intArray[m_intNum]=va;
	m_NumName[m_NumGs] = pa;
	m_NumGs++;
	m_intNum++;
	return 0;
}


void CFormularComputeParent::GetData(int cl,ARRAY_BE& pp)
{
	pp.b = m_MinPoint; 
	pp.e = m_MaxPoint-1;
	int i;
	float ltp = 0;

	switch(cl)
	{
	case TOZERO:
		for(i=m_MinPoint;i<m_MaxPoint;i++)
			 pp.line[i] =0;
		break;
	case 9521:	//open
		if(m_pData!=NULL)
			for(i=m_MinPoint;i<m_MaxPoint;i++)
			   pp.line[i] = m_pData[i].open;
		else 
			GetHsKlineData(cl,pp);
		break;
	case 9522:	//close
		if(m_pData!=NULL)
			for(i=m_MinPoint;i<m_MaxPoint;i++)
				pp.line[i] = m_pData[i].close;
		else 
			GetHsKlineData(cl,pp);
		break;
	case 9523:	//high
		if(m_pData!=NULL)
			for(i=m_MinPoint;i<m_MaxPoint;i++)
				pp.line[i] = m_pData[i].high;
		else 
			GetHsKlineData(cl,pp);
		break;
	case 9524:	//low
		if(m_pData!=NULL)
			for(i=m_MinPoint;i<m_MaxPoint;i++)
				pp.line[i] = m_pData[i].low;
		else 
			GetHsKlineData(cl,pp);
		break;
	case 9525:	//advence
		for(i=m_MinPoint;i<m_MaxPoint;i++)
			pp.line[i] = (float)m_pData[i].advance;
		break;
	case 9526:	//dayofweek
		CTime *ptime;
		for(i=m_MinPoint;i<m_MaxPoint;i++)
		{	time_t tm = m_pData[i].day;
			ptime = new CTime(tm);
			pp.line[i] = (float)ptime->GetDayOfWeek()-1;
			delete ptime;
		}
		break;
	case 9527:	//day
		for(i=m_MinPoint;i<m_MaxPoint;i++)
		{
			time_t tm = m_pData[i].day;
			ptime = new CTime(tm);
			pp.line[i] = (float)(ptime->GetDay());
			delete ptime;
		}
		break;
	case 9528:	//month
		for(i=m_MinPoint;i<m_MaxPoint;i++)
		{
			time_t tm = m_pData[i].day;
			ptime = new CTime(tm);
			pp.line[i] = (float)(ptime->GetMonth());
			delete ptime;
		}
		break;
	case 9529:	//year
		for(i=m_MinPoint;i<m_MaxPoint;i++)
		{
			time_t tm = m_pData[i].day;
			ptime = new CTime(tm);
			pp.line[i] = (float)(ptime->GetYear());
			delete ptime;
		}
		break;
	case 9530: 	//hour
		for(i=m_MinPoint;i<m_MaxPoint;i++)
		{
			time_t tm = m_pData[i].day;
			ptime = new CTime(tm);
			pp.line[i] = (float)(ptime->GetHour());
			delete ptime;
		}
		break;
	case 9531: 	//minute
		for(i=m_MinPoint;i<m_MaxPoint;i++)
		{
			time_t tm = m_pData[i].day;
			ptime = new CTime(tm);
			pp.line[i] = (float)(ptime->GetMinute());
			delete ptime;
		}
		break;
	case 9560: 	//time
		for(i=m_MinPoint;i<m_MaxPoint;i++)
		{
			time_t tm = m_pData[i].day;
			ptime = new CTime(tm);
			pp.line[i] = (float)(ptime->GetMinute()*100+ptime->GetHour()*10000+ptime->GetSecond ());
			delete ptime;
		}
		break;
	case 9561: 	//date
		for(i=m_MinPoint;i<m_MaxPoint;i++)
		{
			time_t tm = m_pData[i].day;
			ptime = new CTime(tm);
			int nYear = ptime->GetYear();
			if(nYear>1999)
				nYear = 100+nYear%100;
			else
				nYear = nYear%100;
			pp.line[i] = (float)(ptime->GetMonth()*100+nYear*10000+ptime->GetDay ());
			delete ptime;
		}
		break;
	case 9532: 	//amount
		if(m_pData!=NULL)
			for(i=m_MinPoint;i<m_MaxPoint;i++)
				pp.line[i] = m_pData[i].amount;
		else
			GetHsKlineData(cl,pp);
		break;
	
	case 9533: 	//vol
		if(m_pData!=NULL)
			for(i=m_MinPoint;i<m_MaxPoint;i++)
				pp.line[i] = m_pData[i].vol;
		else 
			GetHsKlineData(cl,pp);
		break;
	case 9534: 	//isup
		for(i=m_MinPoint;i<m_MaxPoint;i++)
		{	
			if(m_pData[i].close > m_pData[i].open)
				pp.line[i] = 1;
			else
				pp.line[i] =0;
		}
		break;
	
	case 9535: 	//isdown
		for(i=m_MinPoint;i<m_MaxPoint;i++)
		{	
			if(m_pData[i].close < m_pData[i].open)
				pp.line[i] = 1;
			else
				pp.line[i] =0;
		}
		break;
	
	case 9536: 	//isequal
		for(i=m_MinPoint;i<m_MaxPoint;i++)
		{	
			if(m_pData[i].close < m_pData[i].open)
				pp.line[i] = 1;
			else
				pp.line[i] =0;
		}
		break;
	case 9537: 	//decline
		for(i=m_MinPoint;i<m_MaxPoint;i++)
			pp.line[i] =(float) m_pData[i].decline;
		break;
	case 9538: 	//capital
		ltp = CTaiShanKlineShowView::GetCapital(this->m_pReportData );
		for(i=m_MinPoint;i<m_MaxPoint;i++)
			pp.line[i] =ltp;
		break;
	case 9539: 	//ltp
		for(i=m_MinPoint;i<m_MaxPoint;i++)
			pp.line[i] =CTaiShanKlineShowView::GetCapital(this->m_pReportData );
		break;
	case 9541:
		if(m_pKlineIndex==NULL)	{if(this->GetKlineIndex ()==NULL) break;}
		for(i=m_MinPoint;i<m_MaxPoint;i++)
			pp.line[i] =(float) m_pKlineIndex[i].open;
		break;
	case 9542:
		if(m_pKlineIndex==NULL)	{if(this->GetKlineIndex ()==NULL) break;}
		for(i=m_MinPoint;i<m_MaxPoint;i++)
			pp.line[i] =(float) m_pKlineIndex[i].close;
		break;
	case 9543:
		if(m_pKlineIndex==NULL)	{if(this->GetKlineIndex ()==NULL) break;}
		for(i=m_MinPoint;i<m_MaxPoint;i++)
			pp.line[i] =(float) m_pKlineIndex[i].high;
		break;
	case 9544:
		if(m_pKlineIndex==NULL)	{if(this->GetKlineIndex ()==NULL) break;}
		for(i=m_MinPoint;i<m_MaxPoint;i++)
			pp.line[i] =(float) m_pKlineIndex[i].low;
		break;
	case 9545:
		if(m_pKlineIndex==NULL)	{if(this->GetKlineIndex ()==NULL) break;}
		for(i=m_MinPoint;i<m_MaxPoint;i++)
			pp.line[i] =(float) m_pKlineIndex[i].vol;
		break;
	case 9546:
		if(m_pKlineIndex==NULL)	{if(this->GetKlineIndex ()==NULL) break;}
		for(i=m_MinPoint;i<m_MaxPoint;i++)
			pp.line[i] =(float) m_pKlineIndex[i].amount;
		break;
	case 9547:
		if(m_pKlineIndex==NULL)	{if(this->GetKlineIndex ()==NULL) break;}
		for(i=m_MinPoint;i<m_MaxPoint;i++)
			pp.line[i] =(float) m_pKlineIndex[i].decline;
		break;
	case 9548:
		if(m_pKlineIndex==NULL)	{if(this->GetKlineIndex ()==NULL) break;}
		for(i=m_MinPoint;i<m_MaxPoint;i++)
			pp.line[i] =(float) m_pKlineIndex[i].advance;
		break;

		//hs data
	case 9550:
//		if(false==GetBuySellList() || m_pBuySellList->GetCount() != m_MaxPoint) {ASSERT(FALSE);break;}
		if(this->m_nKindKline == HS_KLINE)
		{
			if( m_pBuySellList!=NULL)
				if(m_pBuySellList->GetCount() == m_MaxPoint)
				{
					if(m_pKlineEx == NULL) CTaiKlineFileHS::TransferDataEx (m_pBuySellList,m_pKlineEx);
					if(m_pKlineEx != NULL) CTaiKlineFileHS::TransferHs(this->m_pBuySellList , pp, m_MaxPoint, 0,0,m_pKlineEx);
				}
		}
		else
		{
			if(this->m_pReportData==NULL) 
				m_pDoc->m_sharesInformation.Lookup (this->m_symbol.GetBuffer(0),this->m_pReportData,m_stkKind);
			m_symbol.ReleaseBuffer();
			PCdat1 cdat1 = m_pReportData;
			if(m_pReportData==NULL) break;
			float f=(float)0 ;
			if(cdat1->nowp <= cdat1->pbuy1 )
				f= cdat1->nowv ;
			for(i=m_MinPoint;i<m_MaxPoint;i++)
				pp.line[i] =f;
		}

		break;
	case 9551:
//		if(false==GetBuySellList() || m_pBuySellList->GetCount() != m_MaxPoint) {ASSERT(FALSE);break;}
		if(this->m_nKindKline == HS_KLINE)
		{
			if( m_pBuySellList!=NULL)
				if(m_pBuySellList->GetCount() == m_MaxPoint)
				{
					if(m_pKlineEx == NULL) CTaiKlineFileHS::TransferDataEx (m_pBuySellList,m_pKlineEx);
					if(m_pKlineEx != NULL) CTaiKlineFileHS::TransferHs(this->m_pBuySellList , pp, m_MaxPoint, 1,0,m_pKlineEx);
				}
		}
		else
		{
			if(this->m_pReportData==NULL) m_pDoc->m_sharesInformation .Lookup (this->m_symbol.GetBuffer(0),this->m_pReportData,m_stkKind);
			m_symbol.ReleaseBuffer();
			PCdat1 cdat1 = m_pReportData;
			if(m_pReportData==NULL) break;
			float f=(float)0 ;
			if(cdat1->nowp >= cdat1->psel1 )
				f= cdat1->nowv ;
			for(i=m_MinPoint;i<m_MaxPoint;i++)
				pp.line[i] =f;
		}

		break;
	case 9552:
		if(this->m_nKindKline == HS_KLINE)
		{
			if( m_pBuySellList!=NULL)
				if(m_pBuySellList->GetCount() == m_MaxPoint)
				{
					if(m_pKlineEx == NULL) CTaiKlineFileHS::TransferDataEx (m_pBuySellList,m_pKlineEx);
					if(m_pKlineEx != NULL) CTaiKlineFileHS::TransferHs(this->m_pBuySellList , pp, m_MaxPoint, 2,0,m_pKlineEx);
				}
		}
		else
		{
			if(this->m_pReportData==NULL) m_pDoc->m_sharesInformation .Lookup (this->m_symbol.GetBuffer(0),this->m_pReportData,m_stkKind);
			m_symbol.ReleaseBuffer();
			PCdat1 cdat1 = m_pReportData;
			if(m_pReportData==NULL) break;
			float f=(float)1 ;
			if(cdat1->nowp >= cdat1->psel1 )
				f= 0 ;
			for(i=m_MinPoint;i<m_MaxPoint;i++)
				pp.line[i] =f;
		}

		break;
	case 9553:
		for(i=m_MinPoint;i<m_MaxPoint;i++)
			pp.line[i] =(float) ((KlineGeneral*)(m_pData+i))->volPositive ;
		break;
	case 9554:
		for(i=m_MinPoint;i<m_MaxPoint;i++)
			pp.line[i] =m_pData[i].vol - (float) ((KlineGeneral*)(m_pData+i))->volPositive ;
		break;//
	case 9580:
		for(i=m_MinPoint;i<m_MaxPoint;i++)
			pp.line[i] =100;
		break;//
	default:
		if( cl>=MIDB && cl<=MIDE )
		{
/*			int bl=m_VarArray[cl-MIDB].b;
			int be=m_VarArray[cl-MIDB].e;
			pp.b = bl;
			pp.e = be;
			if(bl>=0&& be>=0 && be>=bl)
			{
			for(i=bl;i<=be;i++) 
				pp.line[i] =m_VarArray[cl-MIDB].line[i];
			}*/
			pp = m_VarArray[cl-MIDB];
			break;
		}
		if(cl>=RLINEB && cl<=RLINEE)
		{
			pp =m_Rline[cl-RLINEB];
/*
			int bl=m_Rline[cl-RLINEB].b;
			int be=m_Rline[cl-RLINEB].e;
			pp.b = bl;
			pp.e = be;
			if(bl>=0&& be>=0 && be>=bl)
			{
			for(i=bl;i<=be;i++) 
				pp.line[i] =m_Rline[cl-RLINEB].line[i];
			}*/
			break;
		}
		if(cl>=CEFB && cl<=CEFE)
		{
			for(i=m_MinPoint;i<m_MaxPoint;i++) 
				pp.line[i] =m_floatArray[cl-CEFB];
			break;
		}
		if(cl>=CEIB && cl<=CEIE)
		{
			for(i=m_MinPoint;i<m_MaxPoint;i++) 
				pp.line[i] =(float)m_intArray[cl-CEIB];
			break;
		}
		if(cl>=PCMB && cl<=PCME)
		{
			pp =m_MidArray[cl-PCMB];
/*			int bl=m_MidArray[cl-PCMB].b;
			int be=m_MidArray[cl-PCMB].e;
			pp.b=bl;
			pp.e=be;
			if(bl>=0&& be>=0 && be>=bl)
			{
				for(i=bl;i<=be;i++) 
					pp.line[i] =m_MidArray[cl-PCMB].line[i];
			}*/
			delete[] m_MidArray[cl-PCMB].line;
			m_MidNum--;
			break;
		}
		//---		引用
		if(cl>=QUB && cl <=QUE)
		{
			if(Relolve(cl)==0)//把结果写到中间数组
			{
				//---		从中间数组读到pp中(lmb)
				pp =m_MidArray[m_MidNum-1];
/*				int bl=m_MidArray[m_MidNum-1].b;
				int be=m_MidArray[m_MidNum-1].e;
				pp.b=bl;
				pp.e=be;
				if(bl>=0&& be>=0 && be>=bl)
				{
				for(i=bl;i<=be;i++) 
					pp.line[i] =m_MidArray[m_MidNum-1].line[i];
				}*/
				delete[] m_MidArray[m_MidNum-1].line;
				m_MidNum--;
			}
			break;
		}
		if(cl>=CECB && cl <CECE)
		{
			{
				pp.s = this->m_stringArray[cl-CECB];
				pp.kind = ARRAY_BE::StringData;
			}
			break;
		}
		break;
	}
}
//  正常返回 0 ；错误返回代码,错误信息保存在m_strErrMsg中// 
int CFormularComputeParent::FormularCompute()
{	
	int i;

/*	if(this->m_pIndex )
	{
		if(m_pIndex->isProtected || m_pIndex->buyStr != "" || m_pIndex->sellStr != "")
		{
			bool b = ((CMainFrame*)AfxGetMainWnd())->CanUseIndicatorProtected();
			if(!b) return -1;
		}
	}
*/

#ifndef _DEBUG
	try
	{
#endif

	if(	m_nExec == 0)
	{
		m_nExec = 1;
	}
	else
	{
		m_strErrMsg = "公式重新调用必须初始化--SETNEW";
		return -100;
	}

	Devide();//分解

	if(m_nErrPos!=-1) 
		return -100;
	
	int cs,msg=0;
	
	//for right box func
	if(ARRAY_Right_Box::m_nCount>0)
	{
		bool bDay = true;
		if(this->m_nKindKline  < DAY_KLINE || m_nKindKline==HS_KLINE || m_nKindKline==MIN1_KLINE)
			bDay = false;
		int nCurr = CTaiKlineFileKLine::RegularTime(ARRAY_Right_Box::m_timeCompute.GetTime(),bDay);
		for(int i=0;i<m_MaxPoint ;i++)
		{
			if(CTaiKlineFileKLine::RegularTime(this->m_pData[i].day,bDay)>=nCurr)
			{
				m_nCurrentFoot = i;
				break;
			}
		}
	}

	//---		开始计算
	for(i=0;i<=m_ComSortNum;i++)
	{
		if(msg!=0)
		{
			m_strErrMsg = "运行错误";
			if(msg!=999) 
				return msg;
		}
		cs = m_ComSort[i];
		if(cs>=TOZERO)      //是系统参数
		{
			m_table.push_b(cs);
			continue;
		}
		if(cs>=QUB && cs<=QUE) //是引用
		{
			msg = Relolve(cs);
			continue;
		}
		if(cs>=FuncDllB && cs<=FuncDllE) //是dll
		{
			msg = FuncFromDll(cs);
			continue;
		}
		if(cs>=CECB && cs<CECE) //是char
		{
			m_table.push_b(cs);
			continue;
		}
		switch(cs)
		{
		case 1001:    // :
			msg=Func1001();
			break;
		case 1002:    // :=
			msg=Func1002();
			break;
		case 3001:
			msg=Func3001();// and
			break;
		case 2001:
			msg=Func2001();// or
			break;
		case 4001:
			msg=Func4001();// >
			break;
		case 4002:
			msg=Func4002();// <
			break;
		case 4005:
			msg=Func4005();// >=
			break;
		case 4006:
			msg=Func4006();// <=
			break;
		case 4008:
			msg=Func4008();// ==
			break;
		case 4009:
			msg=Func4009();// !=
			break;
		case 5001:
			msg=Func5001();// +
			break;
		case 5002:
			msg=Func5002();// -
			break;
		case 6001:
			msg=Func6001();// *
			break;
		case 6002:
			msg=Func6002();// /
			break;
		case 6003:
			msg=Func6003();// *
			break;
		case 6004:
			msg=Func6004();// /
			break;
		case 8001:
			msg=Func8001();// /
			break;
		case 8002:
			msg=Func8002();// /
			break;
		case 8003:
			msg=Func8003();// /
			break;
		case 8004:
			msg=Func8004();// /
			break;
		case 8005:
			msg=Func8005();// /
			break;
		case 8006:
			msg=Func8006();// /
			break;
		case 8007:
			msg=Func8007();// /
			break;
		case 8008:
			msg=Func8008();// /
			break;
		case 8009:
			msg=Func8009();// /
			break;
		case 8010:
			msg=Func8010();// /
			break;
		case 8011:
			msg=Func8011();// /
			break;
		case 8012:
			msg=Func8012();// /
			break;
		case 8013:
			msg=Func8013();// /
			break;
		case 8030:
			msg=Func8030();// /
			break;
		case 8031:
			msg=Func8031();// /
			break;
		case 8032:
			msg=Func832();// /
			break;
		case 8033:
			msg=Func8033();// /
			break;
		case 8034:
			msg=Func8034();// /
			break;
		case 8035:
			msg=Func8035();// /
			break;
		case 8036:
			msg=Func8036();// /
			break;
		case 8037:
			msg=Func8037();// /
			break;
		case 8038:
			msg=Func8038();// /
			break;
		case 8039:
			msg=Func8039();// /
			break;
		case 8040:
			msg=Func8040();// /
			break;
		case 8041:
			msg=Func8041();
			break;
		case 8042:
			msg=Func8042();
			break;
		case 8043:
			msg=Func8043();
			break;
		case 8044:
			msg=Func8044();
			break;
		case 8045:
			msg=Func8045();
			break;
		case 8046:
			msg=Func8046();
			break;
		case 8047:
			msg=Func8047();
			break;
		case 8048:
		case 8049:
		case 8050:
		case 8051:
			msg=Func8048to8051(3+cs-8048);// /Func8048to8051(int nKind)
			break;
		case 8052:
			msg=Func8052();
			break;
		case 8053:
			msg=Func8053();
			break;
		default:
			break;
		}
	}

	//  返回计算结果
	ARRAY_BE  lt;
	lt.line = new float[m_MaxPoint];

	while(cs = m_table.pop_it())
	{
		if(m_RlineNum > RLINEE-RLINEB)
			break;
		m_Rline[m_RlineNum].kind = ARRAY_BE::Normal;
		m_Rline[m_RlineNum].line = new float[m_MaxPoint];

		if(cs< PAM)//如CLOSE，DAY等
		{
			GetData(cs,lt);
			m_Rline[m_RlineNum].b=lt.b;
			m_Rline[m_RlineNum].e=lt.e;

			if(m_Rline[m_RlineNum].b>=0 && m_Rline[m_RlineNum].e<=m_MaxPoint-1 && m_Rline[m_RlineNum].b<=m_MaxPoint-1 && m_Rline[m_RlineNum].b<=m_Rline[m_RlineNum].e)
				for(i=lt.b;i<=lt.e;i++)
					m_Rline[m_RlineNum].line[i] = lt.line[i];
		}
		else
		{
			m_Rline[m_RlineNum].b=m_MidArray[cs-PCMB].b;
			m_Rline[m_RlineNum].e=m_MidArray[cs-PCMB].e;
			if(m_Rline[m_RlineNum].b>=0 && m_Rline[m_RlineNum].e<=m_MaxPoint-1 && m_Rline[m_RlineNum].b<=m_MaxPoint-1 && m_Rline[m_RlineNum].b<=m_Rline[m_RlineNum].e)
			for(i=m_MidArray[cs-PCMB].b;i<=m_MidArray[cs-PCMB].e;i++)
			{
				m_Rline[m_RlineNum].line[i] = m_MidArray[cs-PCMB].line[i];
			}
		
		}
		m_RlineNum++;
	}

	delete lt.line;
	return 0;
#ifndef _DEBUG
	}
	catch(...)
	{
	}
	return -100;
#endif

}

int CFormularComputeParent::Func1001()
{
	int cl,cr;
	ARRAY_BE fcl;
	fcl.line = new float[m_MaxPoint];
	cr=m_table.pop_it();
	cl=m_table.pop_it();
	GetData(cl,fcl);
	if(fcl.b>fcl.e||fcl.e>m_MaxPoint-1||fcl.b<0)
	{
		m_Rline[cr-RLINEB].b =fcl.b;
		m_Rline[cr-RLINEB].e =fcl.e;
		delete[] fcl.line;
		return 999;
	}
	
	m_Rline[cr-RLINEB] = fcl;
	delete[] fcl.line;

	return 0;
}

int CFormularComputeParent::Func1002()
{
	int cl,cr;
	ARRAY_BE fcr;
	fcr.line = new float[m_MaxPoint];
	cl=m_table.pop_it();
	cr=m_table.pop_it();
	GetData(cr,fcr);
	if(fcr.b>fcr.e||fcr.e>m_MaxPoint-1||fcr.b<0)
	{
		m_VarArray[cl-MIDB].b =fcr.b;
		m_VarArray[cl-MIDB].e =fcr.e;
		delete[] fcr.line;
		return 999;
	}
	m_VarArray[cl-MIDB] =fcr;
	delete[] fcr.line;
	return 0;
}

//=====and=======//
int CFormularComputeParent::Func3001()
{
	int cl,cr;
	int bl,be;
	ARRAY_BE fcl,fcr;

	fcl.line = new float[m_MaxPoint];
	fcr.line = new float[m_MaxPoint];
	
	cl=m_table.pop_it();
	GetData(cl,fcl);
	cr=m_table.pop_it();
	GetData(cr,fcr);
	
	m_MidArray[m_MidNum].kind = ARRAY_BE::Normal;
	m_MidArray[m_MidNum].line = new float[m_MaxPoint];
	m_MidArray[m_MidNum].b =max(fcl.b,fcr.b); 
	m_MidArray[m_MidNum].e =min(fcl.e,fcr.e); 

	bl = m_MidArray[m_MidNum].b;
	be = m_MidArray[m_MidNum].e;
	if(bl>be||be>m_MaxPoint-1||bl<0)
	{
		m_table.push_b(PCMB+m_MidNum);
		m_MidNum++;
		delete[] fcl.line;
		delete[] fcr.line;
		return 999;
	}


	for(int i=bl;i<=be;i++)
	{
		if(fcl.line[i] && fcr.line[i])
			m_MidArray[m_MidNum].line[i] = 1;
		else 
			m_MidArray[m_MidNum].line[i] = 0;
	}
	
	m_table.push_b(PCMB+m_MidNum);
	m_MidNum++;
	
	delete[] fcl.line;
	delete[] fcr.line;
	return 0;
}
//===or==//
int CFormularComputeParent::Func2001()
{
	int cl,cr;
	ARRAY_BE fcl,fcr;
	
	fcl.line = new float[m_MaxPoint];
	fcr.line = new float[m_MaxPoint];
	
	cl=m_table.pop_it();
	GetData(cl,fcl);
	cr=m_table.pop_it();
	GetData(cr,fcr);
	
	m_MidArray[m_MidNum].kind = ARRAY_BE::Normal;
	m_MidArray[m_MidNum].line = new float[m_MaxPoint];
	m_MidArray[m_MidNum].b =max(fcl.b,fcr.b); 
	m_MidArray[m_MidNum].e =min(fcl.e,fcr.e); 

	int bl = m_MidArray[m_MidNum].b;
	int be = m_MidArray[m_MidNum].e;
	if(bl>be||be>m_MaxPoint-1||bl<0)
	{
		m_table.push_b(PCMB+m_MidNum);
		m_MidNum++;
		delete[] fcl.line;
		delete[] fcr.line;
		return 999;
	}

	for(int i=bl;i<=be;i++)
	{
		if(fcl.line[i] || fcr.line[i])
			m_MidArray[m_MidNum].line[i] = 1;
		else 
			m_MidArray[m_MidNum].line[i] = 0;
	}
	
	m_table.push_b(PCMB+m_MidNum);
	m_MidNum++;
	
	delete[] fcl.line;
	delete[] fcr.line;
	return 0;
}
//== >==//
int CFormularComputeParent::Func4001()
{
	int cl,cr;
	ARRAY_BE fcl,fcr;
	
	fcl.line = new float[m_MaxPoint];
	fcr.line = new float[m_MaxPoint];
	
	cl=m_table.pop_it();
	GetData(cl,fcl);
	cr=m_table.pop_it();
	GetData(cr,fcr);
	
	m_MidArray[m_MidNum].kind = ARRAY_BE::Normal;
	m_MidArray[m_MidNum].line = new float[m_MaxPoint];
	m_MidArray[m_MidNum].b =max(fcl.b,fcr.b); 
	m_MidArray[m_MidNum].e =min(fcl.e,fcr.e); 
	

	int bl = m_MidArray[m_MidNum].b;
	int be = m_MidArray[m_MidNum].e;
	if(bl>be||be>m_MaxPoint-1||bl<0)
	{
		m_table.push_b(PCMB+m_MidNum);
		m_MidNum++;
		delete[] fcl.line;
		delete[] fcr.line;
		return 999;
	}

	for(int i=bl;i<=be;i++)
	{
		if(fcl.line[i] > fcr.line[i])
			m_MidArray[m_MidNum].line[i] = 1;
		else 
			m_MidArray[m_MidNum].line[i] = 0;
	}
	
	m_table.push_b(PCMB+m_MidNum);
	m_MidNum++;
	
	delete[] fcl.line;
	delete[] fcr.line;
	return 0;
}
//====<===//
int CFormularComputeParent::Func4002()
{
	int cl,cr;
	ARRAY_BE fcl,fcr;
	
	fcl.line = new float[m_MaxPoint];
	fcr.line = new float[m_MaxPoint];
	
	cl=m_table.pop_it();
	GetData(cl,fcl);
	cr=m_table.pop_it();
	GetData(cr,fcr);
	
	m_MidArray[m_MidNum].kind = ARRAY_BE::Normal;
	m_MidArray[m_MidNum].line = new float[m_MaxPoint];
	m_MidArray[m_MidNum].b =max(fcl.b,fcr.b); 
	m_MidArray[m_MidNum].e =min(fcl.e,fcr.e); 
	

	int bl = m_MidArray[m_MidNum].b;
	int be = m_MidArray[m_MidNum].e;
	if(bl>be||be>m_MaxPoint-1||bl<0)
	{
		m_table.push_b(PCMB+m_MidNum);
		m_MidNum++;
		delete[] fcl.line;
		delete[] fcr.line;
		return 999;
	}

	for(int i=bl;i<=be;i++)
	{
		if(fcl.line[i] < fcr.line[i])
			m_MidArray[m_MidNum].line[i] = 1;
		else 
			m_MidArray[m_MidNum].line[i] = 0;
	}
	
	m_table.push_b(PCMB+m_MidNum);
	m_MidNum++;
	
	delete[] fcl.line;
	delete[] fcr.line;
	return 0;
}
//==== >= =====//
int CFormularComputeParent::Func4005()
{
	int cl,cr;
	ARRAY_BE fcl,fcr;
	
	fcl.line = new float[m_MaxPoint];
	fcr.line = new float[m_MaxPoint];

	cl=m_table.pop_it();
	GetData(cl,fcl);
	cr=m_table.pop_it();
	GetData(cr,fcr);
	
	m_MidArray[m_MidNum].kind = ARRAY_BE::Normal;
	m_MidArray[m_MidNum].line = new float[m_MaxPoint];
	m_MidArray[m_MidNum].b =max(fcl.b,fcr.b); 
	m_MidArray[m_MidNum].e =min(fcl.e,fcr.e); 
	
	int bl = m_MidArray[m_MidNum].b;
	int be = m_MidArray[m_MidNum].e;
	if(bl>be||be>m_MaxPoint-1||bl<0)
	{
		m_table.push_b(PCMB+m_MidNum);
		m_MidNum++;
		delete[] fcl.line;
		delete[] fcr.line;
		return 999;
	}

	for(int i=bl;i<=be;i++)
	{
		if(fcl.line[i] >= fcr.line[i])
			m_MidArray[m_MidNum].line[i] = 1;
		else 
			m_MidArray[m_MidNum].line[i] = 0;
	}
	
	m_table.push_b(PCMB+m_MidNum);
	m_MidNum++;
	
	delete[] fcl.line;
	delete[] fcr.line;
	return 0;
}
//======= <= =========//
int CFormularComputeParent::Func4006()
{
	int cl,cr;
	ARRAY_BE fcl,fcr;
	
	fcl.line = new float[m_MaxPoint];
	fcr.line = new float[m_MaxPoint];
	
	cl=m_table.pop_it();
	GetData(cl,fcl);
	cr=m_table.pop_it();
	GetData(cr,fcr);
	
	m_MidArray[m_MidNum].kind = ARRAY_BE::Normal;
	m_MidArray[m_MidNum].line = new float[m_MaxPoint];
	m_MidArray[m_MidNum].b =max(fcl.b,fcr.b); 
	m_MidArray[m_MidNum].e =min(fcl.e,fcr.e); 
	
	int bl = m_MidArray[m_MidNum].b;
	int be = m_MidArray[m_MidNum].e;
	if(bl>be||be>m_MaxPoint-1||bl<0)
	{
		m_table.push_b(PCMB+m_MidNum);
		m_MidNum++;
		delete[] fcl.line;
		delete[] fcr.line;
		return 999;
	}

	for(int i=bl;i<=be;i++)
	{
		if(fcl.line[i] <= fcr.line[i])
			m_MidArray[m_MidNum].line[i] = 1;
		else 
			m_MidArray[m_MidNum].line[i] = 0;
	}
	
	m_table.push_b(PCMB+m_MidNum);
	m_MidNum++;
	
	delete[] fcl.line;
	delete[] fcr.line;
	return 0;
}
//========= == =======//
int CFormularComputeParent::Func4008()
{
	int cl,cr;
	ARRAY_BE fcl,fcr;
	
	fcl.line = new float[m_MaxPoint];
	fcr.line = new float[m_MaxPoint];
	
	cl=m_table.pop_it();
	GetData(cl,fcl);
	cr=m_table.pop_it();
	GetData(cr,fcr);
	
	m_MidArray[m_MidNum].kind = ARRAY_BE::Normal;
	m_MidArray[m_MidNum].line = new float[m_MaxPoint];
	m_MidArray[m_MidNum].b =max(fcl.b,fcr.b); 
	m_MidArray[m_MidNum].e =min(fcl.e,fcr.e); 
	
	int bl = m_MidArray[m_MidNum].b;
	int be = m_MidArray[m_MidNum].e;
	if(bl>be||be>m_MaxPoint-1||bl<0)
	{
		m_table.push_b(PCMB+m_MidNum);
		m_MidNum++;
		delete[] fcl.line;
		delete[] fcr.line;
		return 999;
	}

	for(int i=bl;i<=be;i++)
	{
		if(fcl.line[i] == fcr.line[i])
			m_MidArray[m_MidNum].line[i] = 1;
		else 
			m_MidArray[m_MidNum].line[i] = 0;
	}
	
	m_table.push_b(PCMB+m_MidNum);
	m_MidNum++;
	
	delete[] fcl.line;
	delete[] fcr.line;
	return 0;
}
//========== != =========//
int CFormularComputeParent::Func4009()
{
	int cl,cr;
	ARRAY_BE fcl,fcr;
	
	fcl.line = new float[m_MaxPoint];
	fcr.line = new float[m_MaxPoint];
	
	cl=m_table.pop_it();
	GetData(cl,fcl);
	cr=m_table.pop_it();
	GetData(cr,fcr);
	
	m_MidArray[m_MidNum].kind = ARRAY_BE::Normal;
	m_MidArray[m_MidNum].line = new float[m_MaxPoint];
	m_MidArray[m_MidNum].b =max(fcl.b,fcr.b); 
	m_MidArray[m_MidNum].e =min(fcl.e,fcr.e); 
	
	int bl = m_MidArray[m_MidNum].b;
	int be = m_MidArray[m_MidNum].e;
	if(bl>be||be>m_MaxPoint-1||bl<0)
	{
		m_table.push_b(PCMB+m_MidNum);
		m_MidNum++;
		delete[] fcl.line;
		delete[] fcr.line;
		return 999;
	}

	for(int i=bl;i<=be;i++)
	{
		if(fcl.line[i] != fcr.line[i])
			m_MidArray[m_MidNum].line[i] = 1;
		else 
			m_MidArray[m_MidNum].line[i] = 0;
	}
	
	m_table.push_b(PCMB+m_MidNum);
	m_MidNum++;
	
	delete[] fcl.line;
	delete[] fcr.line;
	return 0;
}
//=========+=======//
int CFormularComputeParent::Func5001()
{
	int cl,cr;
	ARRAY_BE fcl,fcr;
	
	fcl.line = new float[m_MaxPoint];
	fcr.line = new float[m_MaxPoint];
	
	cl=m_table.pop_it();
	GetData(cl,fcl);
	cr=m_table.pop_it();
	GetData(cr,fcr);
	
	m_MidArray[m_MidNum].kind = ARRAY_BE::Normal;
	m_MidArray[m_MidNum].line = new float[m_MaxPoint];
	m_MidArray[m_MidNum].b =max(fcl.b,fcr.b); 
	m_MidArray[m_MidNum].e =min(fcl.e,fcr.e); 
	
	int bl = m_MidArray[m_MidNum].b;
	int be = m_MidArray[m_MidNum].e;
	if(bl>be||be>m_MaxPoint-1||bl<0)
	{
		m_table.push_b(PCMB+m_MidNum);
		m_MidNum++;
		delete[] fcl.line;
		delete[] fcr.line;
		return 999;
	}

	m_MidArray[m_MidNum] .Add(fcl,fcr);
	
	m_table.push_b(PCMB+m_MidNum);
	m_MidNum++;
	
	delete[] fcl.line;
	delete[] fcr.line;
	return 0;

}
//======-==========//
int CFormularComputeParent::Func5002()
{
	int cl,cr;
	ARRAY_BE fcl,fcr;
	
	fcl.line = new float[m_MaxPoint];
	fcr.line = new float[m_MaxPoint];
	
	cl=m_table.pop_it();
	GetData(cl,fcl);
	cr=m_table.pop_it();
	GetData(cr,fcr);
	
	m_MidArray[m_MidNum].kind = ARRAY_BE::Normal;
	m_MidArray[m_MidNum].line = new float[m_MaxPoint];
	m_MidArray[m_MidNum].b =max(fcl.b,fcr.b); 
	m_MidArray[m_MidNum].e =min(fcl.e,fcr.e); 
	
	int bl = m_MidArray[m_MidNum].b;
	int be = m_MidArray[m_MidNum].e;
	if(bl>be||be>m_MaxPoint-1||bl<0)
	{
		m_table.push_b(PCMB+m_MidNum);
		m_MidNum++;
		delete[] fcl.line;
		delete[] fcr.line;
		return 999;
	}

	m_MidArray[m_MidNum] .Sub(fcl,fcr);

	m_table.push_b(PCMB+m_MidNum);
	m_MidNum++;
	
	delete[] fcl.line;
	delete[] fcr.line;
	return 0;

}
//=======*=====/
int CFormularComputeParent::Func6001()
{
	int cl,cr;
	ARRAY_BE fcl,fcr;
	
	fcl.line = new float[m_MaxPoint];
	fcr.line = new float[m_MaxPoint];
	cl=m_table.pop_it();
	
	GetData(cl,fcl);
	cr=m_table.pop_it();
	GetData(cr,fcr);
	
	m_MidArray[m_MidNum].kind = ARRAY_BE::Normal;
	m_MidArray[m_MidNum].line = new float[m_MaxPoint];
	m_MidArray[m_MidNum].b =max(fcl.b,fcr.b); 
	m_MidArray[m_MidNum].e =min(fcl.e,fcr.e); 
	
	int bl = m_MidArray[m_MidNum].b;
	int be = m_MidArray[m_MidNum].e;
	if(bl>be||be>m_MaxPoint-1||bl<0)
	{
		m_table.push_b(PCMB+m_MidNum);
		m_MidNum++;
		delete[] fcl.line;
		delete[] fcr.line;
		return 999;
	}

	m_MidArray[m_MidNum] .Mult(fcl,fcr);
	
	m_table.push_b(PCMB+m_MidNum);
	m_MidNum++;
	
	delete[] fcl.line;
	delete[] fcr.line;
	return 0;

}
//========/==========//
int CFormularComputeParent::Func6002()
{
	int cl,cr;
	ARRAY_BE fcl,fcr;
	
	fcl.line = new float[m_MaxPoint];
	fcr.line = new float[m_MaxPoint];
	
	cl=m_table.pop_it();
	GetData(cl,fcl);
	cr=m_table.pop_it();
	GetData(cr,fcr);
	
	m_MidArray[m_MidNum].kind = ARRAY_BE::Normal;
	m_MidArray[m_MidNum].line = new float[m_MaxPoint];
	m_MidArray[m_MidNum].b =max(fcl.b,fcr.b); 
	m_MidArray[m_MidNum].e =min(fcl.e,fcr.e); 
	
	int bl = m_MidArray[m_MidNum].b;
	int be = m_MidArray[m_MidNum].e;
	if(bl>be||be>m_MaxPoint-1||bl<0)
	{
		m_table.push_b(PCMB+m_MidNum);
		m_MidNum++;
		delete[] fcl.line;
		delete[] fcr.line;
		return 999;
	}

	m_MidArray[m_MidNum] .Div(fcl,fcr);

	m_table.push_b(PCMB+m_MidNum);
	m_MidNum++;
	
	delete[] fcl.line;
	delete[] fcr.line;
	return 0;
}
//=======*-=====/
int CFormularComputeParent::Func6003()
{
	int cl,cr;
	ARRAY_BE fcl,fcr;
	
	fcl.line = new float[m_MaxPoint];
	fcr.line = new float[m_MaxPoint];
	cl=m_table.pop_it();
	
	GetData(cl,fcl);
	cr=m_table.pop_it();
	GetData(cr,fcr);
	
	m_MidArray[m_MidNum].kind = ARRAY_BE::Normal;
	m_MidArray[m_MidNum].line = new float[m_MaxPoint];
	m_MidArray[m_MidNum].b =max(fcl.b,fcr.b); 
	m_MidArray[m_MidNum].e =min(fcl.e,fcr.e); 
	
	int bl = m_MidArray[m_MidNum].b;
	int be = m_MidArray[m_MidNum].e;
	if(bl>be||be>m_MaxPoint-1||bl<0)
	{
		m_table.push_b(PCMB+m_MidNum);
		m_MidNum++;
		delete[] fcl.line;
		delete[] fcr.line;
		return 999;
	}

	m_MidArray[m_MidNum] .Mult(fcl,fcr);
	m_MidArray[m_MidNum] .Mult(m_MidArray[m_MidNum],-1);
	
	m_table.push_b(PCMB+m_MidNum);
	m_MidNum++;
	
	delete[] fcl.line;
	delete[] fcr.line;
	return 0;

}
//========/-==========//
int CFormularComputeParent::Func6004()
{
	int cl,cr;
	ARRAY_BE fcl,fcr;
	
	fcl.line = new float[m_MaxPoint];
	fcr.line = new float[m_MaxPoint];
	
	cl=m_table.pop_it();
	GetData(cl,fcl);
	cr=m_table.pop_it();
	GetData(cr,fcr);
	
	m_MidArray[m_MidNum].kind = ARRAY_BE::Normal;
	m_MidArray[m_MidNum].line = new float[m_MaxPoint];
	m_MidArray[m_MidNum].b =max(fcl.b,fcr.b); 
	m_MidArray[m_MidNum].e =min(fcl.e,fcr.e); 
	
	int bl = m_MidArray[m_MidNum].b;
	int be = m_MidArray[m_MidNum].e;
	if(bl>be||be>m_MaxPoint-1||bl<0)
	{
		m_table.push_b(PCMB+m_MidNum);
		m_MidNum++;
		delete[] fcl.line;
		delete[] fcr.line;
		return 999;
	}

	m_MidArray[m_MidNum] .Div(fcl,fcr);
	m_MidArray[m_MidNum] .Div(m_MidArray[m_MidNum],-1);

	m_table.push_b(PCMB+m_MidNum);
	m_MidNum++;
	
	delete[] fcl.line;
	delete[] fcr.line;
	return 0;
}
//===max====//
int CFormularComputeParent::Func8001()
{
	int cl,cr;
	ARRAY_BE fcl,fcr;
	
	fcl.line = new float[m_MaxPoint];
	fcr.line = new float[m_MaxPoint];
	
	cl=m_table.pop_it();
	GetData(cl,fcl);
	cr=m_table.pop_it();
	GetData(cr,fcr);
	
	m_MidArray[m_MidNum].kind = ARRAY_BE::Normal;
	m_MidArray[m_MidNum].line = new float[m_MaxPoint];
	m_MidArray[m_MidNum].b =max(fcl.b,fcr.b); 
	m_MidArray[m_MidNum].e =min(fcl.e,fcr.e); 
	
	int bl = m_MidArray[m_MidNum].b;
	int be = m_MidArray[m_MidNum].e;
	if(bl>be||be>m_MaxPoint-1||bl<0)
	{
		m_table.push_b(PCMB+m_MidNum);
		m_MidNum++;
		delete[] fcl.line;
		delete[] fcr.line;
		return 999;
	}

	for(int i=bl;i<=be;i++)
			m_MidArray[m_MidNum].line[i] =
					max(fcl.line[i],fcr.line[i]) ;

	m_table.push_b(PCMB+m_MidNum);
	m_MidNum++;
	
	delete[] fcl.line;
	delete[] fcr.line;
	return 0;
}
//=========min=======//
int CFormularComputeParent::Func8004()
{
	int cl,cr;
	ARRAY_BE fcl,fcr;
	
	fcl.line = new float[m_MaxPoint];
	fcr.line = new float[m_MaxPoint];
	
	cl=m_table.pop_it();
	GetData(cl,fcl);
	cr=m_table.pop_it();
	GetData(cr,fcr);
	
	m_MidArray[m_MidNum].kind = ARRAY_BE::Normal;
	m_MidArray[m_MidNum].line = new float[m_MaxPoint];
	m_MidArray[m_MidNum].b =max(fcl.b,fcr.b); 
	m_MidArray[m_MidNum].e =min(fcl.e,fcr.e); 
	
	int bl = m_MidArray[m_MidNum].b;
	int be = m_MidArray[m_MidNum].e;
	if(bl>be||be>m_MaxPoint-1||bl<0)
	{
		m_table.push_b(PCMB+m_MidNum);
		m_MidNum++;
		delete[] fcl.line;
		delete[] fcr.line;
		return 999;
	}

	for(int i=bl;i<=be;i++)
	{
		m_MidArray[m_MidNum].line[i] 
			=	min(fcl.line[i],fcr.line[i]) ;
	}

	m_table.push_b(PCMB+m_MidNum);
	m_MidNum++;
	
	delete[] fcl.line;
	delete[] fcr.line;
	return 0;
}
//=========mod=======//
int CFormularComputeParent::Func8005()
{
	int cl,cr;
	ARRAY_BE fcl,fcr;
	
	fcl.line = new float[m_MaxPoint];
	fcr.line = new float[m_MaxPoint];
	
	cl=m_table.pop_it();
	GetData(cl,fcl);
	cr=m_table.pop_it();
	GetData(cr,fcr);
	
	m_MidArray[m_MidNum].kind = ARRAY_BE::Normal;
	m_MidArray[m_MidNum].line = new float[m_MaxPoint];
	m_MidArray[m_MidNum].b =max(fcl.b,fcr.b); 
	m_MidArray[m_MidNum].e =min(fcl.e,fcr.e); 
	
	int bl = m_MidArray[m_MidNum].b;
	int be = m_MidArray[m_MidNum].e;
	if(bl>be||be>m_MaxPoint-1||bl<0)
	{
		m_table.push_b(PCMB+m_MidNum);
		m_MidNum++;
		delete[] fcl.line;
		delete[] fcr.line;
		return 999;
	}

	for(int i=bl;i<=be;i++)
	{
		m_MidArray[m_MidNum].line[i]
				=(float)((int)fcl.line[i]/(int)fcr.line[i]) ;
	}
	m_table.push_b(PCMB+m_MidNum);
	m_MidNum++;
	
	delete[] fcl.line;
	delete[] fcr.line;
	return 0;
}
//=========reverse=======//
int CFormularComputeParent::Func8006()
{
	int cl;
	ARRAY_BE fcl;
	
	fcl.line = new float[m_MaxPoint];
	
	cl=m_table.pop_it();
	GetData(cl,fcl);
	
	m_MidArray[m_MidNum].kind = ARRAY_BE::Normal;
	m_MidArray[m_MidNum].line = new float[m_MaxPoint];
	m_MidArray[m_MidNum].b =fcl.b; 
	m_MidArray[m_MidNum].e =fcl.e; 
	
	int bl = m_MidArray[m_MidNum].b;
	int be = m_MidArray[m_MidNum].e;
	if(bl>be||be>m_MaxPoint-1||bl<0)
	{
		m_table.push_b(PCMB+m_MidNum);
		m_MidNum++;
		delete[] fcl.line;
		return 999;
	}

	m_MidArray[m_MidNum].Sub (0.0f,fcl );

	m_table.push_b(PCMB+m_MidNum);
	m_MidNum++;
	
	delete[] fcl.line;
	return 0;
}
//=========if=======//
int CFormularComputeParent::Func8007()
{
	int cl,cr,ce, ret;
	ARRAY_BE fcl,fcr,fce;
	
	fcl.line = new float[m_MaxPoint];
	fcr.line = new float[m_MaxPoint];
	fce.line = new float[m_MaxPoint];

	cl=m_table.pop_it();
	GetData(cl,fcl);
	cr=m_table.pop_it();
	GetData(cr,fcr);
	ce=m_table.pop_it();
	GetData(ce,fce);
	
	m_MidArray[m_MidNum].kind = ARRAY_BE::Normal;
	m_MidArray[m_MidNum].line = new float[m_MaxPoint];
	m_MidArray[m_MidNum].b =max(fcl.b,fcr.b); 
	m_MidArray[m_MidNum].b =max(m_MidArray[m_MidNum].b ,fce.b); 

	m_MidArray[m_MidNum].e =min(fcl.e,fcr.e);
	m_MidArray[m_MidNum].e =min(m_MidArray[m_MidNum].e,fce.e); 
 
	int bl = m_MidArray[m_MidNum].b;
	int be = m_MidArray[m_MidNum].e;
	if(bl>be||be>m_MaxPoint-1||bl<0)
	{
		m_table.push_b(PCMB+m_MidNum);
		m_MidNum++;
		delete[] fcl.line;
		delete[] fcr.line;
		delete[] fce.line;
		return 999;
	}

	for(int i=bl;i<=be;i++)
	{	
		ret =(int)fcl.line[i];
		if(ret)
			m_MidArray[m_MidNum].line[i] =fcr.line[i];
		else
			m_MidArray[m_MidNum].line[i] =fce.line[i];
	}

	m_table.push_b(PCMB+m_MidNum);
	m_MidNum++;
	
	delete[] fcl.line;
	delete[] fcr.line;
	delete[] fce.line;
	return 0;
}
//=========abs=======//
int CFormularComputeParent::Func8002()
{
	int cl;
	ARRAY_BE fcl;
	
	fcl.line = new float[m_MaxPoint];
	
	cl=m_table.pop_it();
	GetData(cl,fcl);
	
	m_MidArray[m_MidNum].kind = ARRAY_BE::Normal;
	m_MidArray[m_MidNum].line = new float[m_MaxPoint];
	m_MidArray[m_MidNum].b =fcl.b; 
	m_MidArray[m_MidNum].e =fcl.e; 
	
	int bl = m_MidArray[m_MidNum].b;
	int be = m_MidArray[m_MidNum].e;
	if(bl>be||be>m_MaxPoint-1||bl<0)
	{
		m_table.push_b(PCMB+m_MidNum);
		m_MidNum++;
		delete[] fcl.line;
		return 999;
	}

	for(int i=bl;i<=be;i++)
		m_MidArray[m_MidNum].line[i] 
				=(float)FABSMY(fcl.line[i]) ;
	
	m_table.push_b(PCMB+m_MidNum);
	m_MidNum++;
	
	delete[] fcl.line;
	return 0;
}
//=========ln=======//
int CFormularComputeParent::Func8008()
{
	int cl;
	ARRAY_BE fcl;
	
	fcl.line = new float[m_MaxPoint];
	
	cl=m_table.pop_it();
	GetData(cl,fcl);
	
	m_MidArray[m_MidNum].kind = ARRAY_BE::Normal;
	m_MidArray[m_MidNum].line = new float[m_MaxPoint];
	m_MidArray[m_MidNum].b =fcl.b; 
	m_MidArray[m_MidNum].e =fcl.e; 
	
	int bl = m_MidArray[m_MidNum].b;
	int be = m_MidArray[m_MidNum].e;
	if(bl>be||be>m_MaxPoint-1||bl<0)
	{
		m_table.push_b(PCMB+m_MidNum);
		m_MidNum++;
		delete[] fcl.line;
		return 999;
	}

	for(int i=bl;i<=be;i++)
	{
		if(fcl.line[i] > 0)
			m_MidArray[m_MidNum].line[i] 
				=(float)log(fcl.line[i]);
		else 
		{
			m_table.push_b(PCMB+m_MidNum);
			m_MidNum++;
			delete[] fcl.line;
			return 8;
		}
	}
	
	m_table.push_b(PCMB+m_MidNum);
	m_MidNum++;
	
	delete[] fcl.line;
	return 0;
}
//=========not=======//
int CFormularComputeParent::Func8009()
{
	int cl,ret;
	ARRAY_BE fcl;
	
	fcl.line = new float[m_MaxPoint];
	
	cl=m_table.pop_it();
	GetData(cl,fcl);
	
	m_MidArray[m_MidNum].kind = ARRAY_BE::Normal;
	m_MidArray[m_MidNum].line = new float[m_MaxPoint];
	m_MidArray[m_MidNum].b =fcl.b; 
	m_MidArray[m_MidNum].e =fcl.e; 
	
	int bl = m_MidArray[m_MidNum].b;
	int be = m_MidArray[m_MidNum].e;
	if(bl>be||be>m_MaxPoint-1||bl<0)
	{
		m_table.push_b(PCMB+m_MidNum);
		m_MidNum++;
		delete[] fcl.line;
		return 999;
	}

	for(int i=bl;i<=be;i++)
	{
		ret =(int)fcl.line[i];
		if(ret == 0)
			m_MidArray[m_MidNum].line[i] =1 ;
		else
			m_MidArray[m_MidNum].line[i] =0 ;
	}
	
	m_table.push_b(PCMB+m_MidNum);
	m_MidNum++;
	
	delete[] fcl.line;
	return 0;
}
//=========sgn=======//
int CFormularComputeParent::Func8010()
{
	int cl,ret;
	ARRAY_BE fcl;
	
	fcl.line = new float[m_MaxPoint];
	
	cl=m_table.pop_it();
	GetData(cl,fcl);
	
	m_MidArray[m_MidNum].kind = ARRAY_BE::Normal;
	m_MidArray[m_MidNum].line = new float[m_MaxPoint];
	m_MidArray[m_MidNum].b =fcl.b; 
	m_MidArray[m_MidNum].e =fcl.e; 
	
	int bl = m_MidArray[m_MidNum].b;
	int be = m_MidArray[m_MidNum].e;
	if(bl>be||be>m_MaxPoint-1||bl<0)
	{
		m_table.push_b(PCMB+m_MidNum);
		m_MidNum++;
		delete[] fcl.line;
		return 999;
	}

	for(int i=bl;i<=be;i++)
	{
		ret =(int)fcl.line[i];
		if(ret == 0) 
			m_MidArray[m_MidNum].line[i] =0 ;
		else
		{
			if(ret <0) 	m_MidArray[m_MidNum].line[i] =-1 ;
			else	    m_MidArray[m_MidNum].line[i] =1 ;
		}
	}
	
	m_table.push_b(PCMB+m_MidNum);
	m_MidNum++;
	
	delete[] fcl.line;
	return 0;
}
//=========exp=======//
int CFormularComputeParent::Func8003()
{
	int cl;
	ARRAY_BE fcl;
	
	fcl.line = new float[m_MaxPoint];
	
	cl=m_table.pop_it();
	GetData(cl,fcl);
	
	m_MidArray[m_MidNum].kind = ARRAY_BE::Normal;
	m_MidArray[m_MidNum].line = new float[m_MaxPoint];
	m_MidArray[m_MidNum].b =fcl.b; 
	m_MidArray[m_MidNum].e =fcl.e; 
	
	int bl = m_MidArray[m_MidNum].b;
	int be = m_MidArray[m_MidNum].e;
	if(bl>be||be>m_MaxPoint-1||bl<0)
	{
		m_table.push_b(PCMB+m_MidNum);
		m_MidNum++;
		delete[] fcl.line;
		return 999;
	}

	for(int i=bl;i<=be;i++)
	{
		m_MidArray[m_MidNum].line[i] 
			=(float)exp(fcl.line[i]);
	}
	
	m_table.push_b(PCMB+m_MidNum);
	m_MidNum++;
	
	delete[] fcl.line;
	return 0;
}
//=========log=======//
int CFormularComputeParent::Func8011()
{
	int cl;
	ARRAY_BE fcl;
	
	fcl.line = new float[m_MaxPoint];
	
	cl=m_table.pop_it();
	
	GetData(cl,fcl);
	
	m_MidArray[m_MidNum].kind = ARRAY_BE::Normal;
	m_MidArray[m_MidNum].line = new float[m_MaxPoint];
	m_MidArray[m_MidNum].b =fcl.b; 
	m_MidArray[m_MidNum].e =fcl.e; 
	
	int bl = m_MidArray[m_MidNum].b;
	int be = m_MidArray[m_MidNum].e;
	if(bl>be||be>m_MaxPoint-1||bl<0)
	{
		m_table.push_b(PCMB+m_MidNum);
		m_MidNum++;
		delete[] fcl.line;
		return 999;
	}

	for(int i=bl;i<=be;i++)
	{
		if(fcl.line[i] > 0)
			m_MidArray[m_MidNum].line[i] 
				=(float)log10(fcl.line[i]);
		else
		{
			m_table.push_b(PCMB+m_MidNum);
			m_MidNum++;
			delete[] fcl.line;
			return 11;
		}

	}
	
	m_table.push_b(PCMB+m_MidNum);
	m_MidNum++;
	
	delete[] fcl.line;
	return 0;
}

//=========sqrt=======//
int CFormularComputeParent::Func8013()
{
	int cl;
	ARRAY_BE fcl;
	
	fcl.line = new float[m_MaxPoint];
	
	cl=m_table.pop_it();
	GetData(cl,fcl);
	
	m_MidArray[m_MidNum].kind = ARRAY_BE::Normal;
	m_MidArray[m_MidNum].line = new float[m_MaxPoint];
	m_MidArray[m_MidNum].b =fcl.b; 
	m_MidArray[m_MidNum].e =fcl.e; 
	
	int bl = m_MidArray[m_MidNum].b;
	int be = m_MidArray[m_MidNum].e;
	if(bl>be||be>m_MaxPoint-1||bl<0)
	{
		m_table.push_b(PCMB+m_MidNum);
		m_MidNum++;
		delete[] fcl.line;
		return 999;
	}

	for(int i=bl;i<=be;i++)
	{
		if(fcl.line[i] >= 0)
			m_MidArray[m_MidNum].line[i]
				=(float)sqrt(fcl.line[i]);
		else
		{
			if(i== bl)
			{
				m_table.push_b(PCMB+m_MidNum);
				m_MidNum++;
				delete[] fcl.line;
				return 12;
			}
			else
				m_MidArray[m_MidNum].line[i]
					=m_MidArray[m_MidNum].line[i-1];
		}
	}
	
	m_table.push_b(PCMB+m_MidNum);
	m_MidNum++;
	
	delete[] fcl.line;
	return 0;
}
//========== pow =========//
int CFormularComputeParent::Func8012()
{
	int cl,cr;
	ARRAY_BE fcl,fcr;
	
	fcl.line = new float[m_MaxPoint];
	fcr.line = new float[m_MaxPoint];
	
	cl=m_table.pop_it();
	GetData(cl,fcl);
	cr=m_table.pop_it();
	GetData(cr,fcr);
	
	m_MidArray[m_MidNum].kind = ARRAY_BE::Normal;
	m_MidArray[m_MidNum].line = new float[m_MaxPoint];
	m_MidArray[m_MidNum].b =max(fcl.b,fcr.b); 
	m_MidArray[m_MidNum].e =min(fcl.e,fcr.e); 
	
	int bl = m_MidArray[m_MidNum].b;
	int be = m_MidArray[m_MidNum].e;
	if(bl>be||be>m_MaxPoint-1||bl<0)
	{
		m_table.push_b(PCMB+m_MidNum);
		m_MidNum++;
		delete[] fcl.line;
		delete[] fcr.line;
		return 999;
	}

	for(int i=bl;i<=be;i++)
	{
		try
		{
		m_MidArray[m_MidNum].line[i] 
			= (float)pow(fcl.line[i],fcr.line[i]);
		}
		catch(...)
		{
			if(i!=bl)
				m_MidArray[m_MidNum].line[i] 
					= m_MidArray[m_MidNum].line[i-1] ;
		}
	}
	
	m_table.push_b(PCMB+m_MidNum);
	m_MidNum++;
	
	delete[] fcl.line;
	delete[] fcr.line;
	return 0;
}
// avedev
int CFormularComputeParent::Func8030()
{
	int cl,cr,ret,j;
	ARRAY_BE fcl,fcr;
	
	fcl.line = new float[m_MaxPoint];
	fcr.line = new float[m_MaxPoint];
	
	cl=m_table.pop_it();
	GetData(cl,fcl);
	cr=m_table.pop_it();
	GetData(cr,fcr);
	
	ret = fcr.b;
	ret =(int)fcr.line[ret];
	if(ret<1)
	{
		delete[] fcl.line;
		delete[] fcr.line;
		return 30;
	}
	m_MidArray[m_MidNum].kind = ARRAY_BE::Normal;
	m_MidArray[m_MidNum].line = new float[m_MaxPoint];
	m_MidArray[m_MidNum].b =fcl.b+ret-1; 
	m_MidArray[m_MidNum].e =fcl.e;
	
	int bl = m_MidArray[m_MidNum].b;
	int be = m_MidArray[m_MidNum].e;
	if(bl>be||be>m_MaxPoint-1||bl<0)
	{
		m_table.push_b(PCMB+m_MidNum);
		m_MidNum++;
		delete[] fcl.line;
		delete[] fcr.line;
		return 999;
	}

	for(int i=bl;i<=be;i++)
	{
		float ax=0,x=0;
		for(j=i-ret+1;j<=i;j++)
				ax+=fcl.line[j];
		ax/=ret;
		for(j=i-ret+1;j<=i;j++)
				x=(float)(FABSMY(fcl.line[j]-ax))+x;
		x/=ret;
		m_MidArray[m_MidNum].line[i] = x;
	}
	
	m_table.push_b(PCMB+m_MidNum);
	m_MidNum++;
	
	delete[] fcl.line;
	delete[] fcr.line;
	return 0;
}
// devsq
int CFormularComputeParent::Func8033()
 {
	int cl,cr,ret,j;
	ARRAY_BE fcl,fcr;
	
	fcl.line = new float[m_MaxPoint];
	fcr.line = new float[m_MaxPoint];
	
	cl=m_table.pop_it();
	GetData(cl,fcl);
	cr=m_table.pop_it();
	GetData(cr,fcr);
	
	ret = fcr.b;
	ret =(int)fcr.line[ret];
	
	if(ret<1) 
	{
		delete[] fcl.line;
		delete[] fcr.line;
		return 33;
	}
	
	m_MidArray[m_MidNum].kind = ARRAY_BE::Normal;
	m_MidArray[m_MidNum].line = new float[m_MaxPoint];
	m_MidArray[m_MidNum].b =fcl.b+ret-1; 
	m_MidArray[m_MidNum].e =fcl.e;
	
	int bl = m_MidArray[m_MidNum].b;
	int be = m_MidArray[m_MidNum].e;
	if(bl>be||be>m_MaxPoint-1||bl<0)
	{
		m_table.push_b(PCMB+m_MidNum);
		m_MidNum++;
		delete[] fcl.line;
		delete[] fcr.line;
		return 999;
	}

	for(int i=bl;i<=be;i++)
	{
		float ax=0,x=0;
		for(j=i-ret+1;j<=i;j++)
				ax+=fcl.line[j];
		ax/=ret;
		for(j=i-ret+1;j<=i;j++)
				x=(fcl.line[j]-ax)*(fcl.line[j]-ax)+x;
		m_MidArray[m_MidNum].line[i] = x;
	}
	
	m_table.push_b(PCMB+m_MidNum);
	m_MidNum++;
	
	delete[] fcl.line;
	delete[] fcr.line;
	return 0;
}
//  
int CFormularComputeParent::Func832() 
{
	int cl,cr;
	ARRAY_BE fcl,fcr;

	fcl.line = new float[m_MaxPoint];
	fcr.line = new float[m_MaxPoint];
	
	cl=m_table.pop_it();
	GetData(cl,fcl);
	cr=m_table.pop_it();
	GetData(cr,fcr);
	
	m_MidArray[m_MidNum].kind = ARRAY_BE::Normal;
	m_MidArray[m_MidNum].line = new float[m_MaxPoint];
	m_MidArray[m_MidNum].b =max(fcl.b,fcr.b)+1; 
	m_MidArray[m_MidNum].e =min(fcl.e,fcr.e);
	
	int bl = m_MidArray[m_MidNum].b;
	int be = m_MidArray[m_MidNum].e;
	if(bl>be||be>m_MaxPoint-1||bl<0)
	{
		m_table.push_b(PCMB+m_MidNum);
		m_MidNum++;
		delete[] fcl.line;
		delete[] fcr.line;
		return 999;
	}

	for(int i=bl;i<=be;i++)
	{
		if(fcl.line[i-1] < fcr.line[i-1] &&
						fcl.line[i] > fcr.line[i])
				m_MidArray[m_MidNum].line[i] = 1;
		else
				m_MidArray[m_MidNum].line[i] = 0;
	}
	
	m_table.push_b(PCMB+m_MidNum);
	m_MidNum++;
	
	delete[] fcl.line;
	delete[] fcr.line;
	return 0;
}
// count
int CFormularComputeParent::Func8031() 
{
	int cl,cr,ret,j;
	ARRAY_BE fcl,fcr;
	
	fcl.line = new float[m_MaxPoint];
	fcr.line = new float[m_MaxPoint];
	
	cl=m_table.pop_it();
	GetData(cl,fcl);
	cr=m_table.pop_it();
	GetData(cr,fcr);
	
	ret = fcr.b;
	ret =(int)fcr.line[ret];
	if(ret<0) 
	{
		delete[] fcl.line;
		delete[] fcr.line;
		return 31;
	}
	
	m_MidArray[m_MidNum].kind = ARRAY_BE::Normal;
	m_MidArray[m_MidNum].line = new float[m_MaxPoint];
	if(ret == 0)
		m_MidArray[m_MidNum].b =fcl.b; 
	else
		m_MidArray[m_MidNum].b =fcl.b+ret-1; 
	m_MidArray[m_MidNum].e =fcl.e;
	
	int bl = m_MidArray[m_MidNum].b;
	int be = m_MidArray[m_MidNum].e;
	if(bl>be||be>m_MaxPoint-1||bl<0)
	{
		m_table.push_b(PCMB+m_MidNum);
		m_MidNum++;
		delete[] fcl.line;
		delete[] fcr.line;
		return 999;
	}

	if(ret>0)
		for(int i=bl;i<=be;i++)
		{
			float x=0;
			for(j=i-ret+1;j<=i;j++)
				if(fcl.line[j] != 0) x+=1;
			m_MidArray[m_MidNum].line[i] = x;
		}
	else
		for(int i=bl;i<=be;i++)
			m_MidArray[m_MidNum].line[i] = i+1;
	
	m_table.push_b(PCMB+m_MidNum);
	m_MidNum++;
	
	delete[] fcl.line;
	delete[] fcr.line;
	return 0;
}
// ema
int CFormularComputeParent::Func8034() 
{
	int cl,cr,ret;
	ARRAY_BE fcl,fcr;
	
	fcl.line = new float[m_MaxPoint];
	fcr.line = new float[m_MaxPoint];
	
	cl=m_table.pop_it();
	GetData(cl,fcl);
	cr=m_table.pop_it();
	GetData(cr,fcr);
	ret = fcr.b;
	ret =(int)fcr.line[ret];
	if(ret<1) 
	{
		delete[] fcl.line;
		delete[] fcr.line;
		return 34;
	}

	m_MidArray[m_MidNum].kind = ARRAY_BE::Normal;
	m_MidArray[m_MidNum].line = new float[m_MaxPoint];
	m_MidArray[m_MidNum].b =fcl.b+ret-1; 
	m_MidArray[m_MidNum].e =fcl.e;

	int bl = m_MidArray[m_MidNum].b;
	int be = m_MidArray[m_MidNum].e;
//（该日DI －前一日的EMA_M）* 2 / （M＋1） ＋前一日的EMA_M
	//m_pData[i].close 
	if(bl>be||be>m_MaxPoint-1||bl<0)
	{
		m_table.push_b(PCMB+m_MidNum);
		m_MidNum++;
		delete[] fcl.line;
		delete[] fcr.line;
		return 999;
	}
	m_MidArray[m_MidNum].line[bl]=fcl.line[bl];

	double db = fcl.line[bl];
	for(int i1=bl+1;i1<=be;i1++)
	{
		//add lmb ;20030301
		ret = fcr.line[i1]+1;
		if(ret<1) ret = 1;
		//end add
		db = (fcl.line[i1]-db)*2.0/(ret) + db;
		m_MidArray[m_MidNum].line[i1]=db;
	}

	m_table.push_b(PCMB+m_MidNum);
	m_MidNum++;
	
	delete[] fcl.line;
	delete[] fcr.line;
	return 0;
}
// hhv
int CFormularComputeParent::Func8035()
 {
	int cl,cr,ret,j;
	ARRAY_BE fcl,fcr;
	
	fcl.line = new float[m_MaxPoint];
	fcr.line = new float[m_MaxPoint];
	
	cl=m_table.pop_it();
	GetData(cl,fcl);
	cr=m_table.pop_it();
	GetData(cr,fcr);
	
		//change lmb ;20030301
	ret = GetBeginFoot( fcr);
		//end add
//	ret = fcr.b;
//	ret =(int)fcr.line[ret];
	
	if(ret<0) //if(ret == 0) is from the first day
	{
		delete[] fcl.line;
		delete[] fcr.line;
		return 35;
	}
	
	m_MidArray[m_MidNum].kind = ARRAY_BE::Normal;
	m_MidArray[m_MidNum].line = new float[m_MaxPoint];
	m_MidArray[m_MidNum].b =fcl.b; 
//	if(ret ==0) m_MidArray[m_MidNum].b =fcl.b; 
	m_MidArray[m_MidNum].e =fcl.e;
	
	int bl = m_MidArray[m_MidNum].b;
	int be = m_MidArray[m_MidNum].e;
	if(bl>be||be>m_MaxPoint-1||bl<0)
	{
		m_table.push_b(PCMB+m_MidNum);
		m_MidNum++;
		delete[] fcl.line;
		delete[] fcr.line;
		return 999;
	}

	if(ret > 0)
	{
		for(int i=bl;i<=be;i++)
		{
			//add lmb ;20030301
			ret = fcr.line[i];
			if(ret<1) ret = 1;
			//end add
			float ax=fcl.line[i];
			if(i-bl<ret)
			{
				for(j=bl;j<=i;j++)
					ax=max(ax,fcl.line[j]);
			}
			else
			{
				for(j=i-ret+1;j<=i;j++)
					ax=max(ax,fcl.line[j]);
			}
			m_MidArray[m_MidNum].line[i] = ax;
		}
	}
	else
	{
		float ax=fcl.line[bl];
		for(int i=bl;i<=be;i++)
		{
//			float ax=fcl.line[i-ret+1];
			ax=max(ax,fcl.line[i]);
			m_MidArray[m_MidNum].line[i] = ax;
		}
	}

	
	m_table.push_b(PCMB+m_MidNum);
	m_MidNum++;
	
	delete[] fcl.line;
	delete[] fcr.line;
	return 0;
}
//  llv
int CFormularComputeParent::Func8036() 
{
	int cl,cr,ret,j;
	ARRAY_BE fcl,fcr;
	
	fcl.line = new float[m_MaxPoint];
	fcr.line = new float[m_MaxPoint];
	
	cl=m_table.pop_it();
	GetData(cl,fcl);
	cr=m_table.pop_it();
	GetData(cr,fcr);
	
		//change lmb ;20030301
	ret = GetBeginFoot( fcr);
		//end add
//	ret = fcr.b;
//	ret =(int)fcr.line[ret];
	
	if(ret<0) 
	{
		delete[] fcl.line;
		delete[] fcr.line;
		return 36;
	}
	
	m_MidArray[m_MidNum].kind = ARRAY_BE::Normal;
	m_MidArray[m_MidNum].line = new float[m_MaxPoint];
	m_MidArray[m_MidNum].b =fcl.b; 
//	if(ret ==0) m_MidArray[m_MidNum].b =fcl.b; 
	m_MidArray[m_MidNum].e =fcl.e;
	
	int bl = m_MidArray[m_MidNum].b;
	int be = m_MidArray[m_MidNum].e;
	if(bl>be||be>m_MaxPoint-1||bl<0)
	{
		m_table.push_b(PCMB+m_MidNum);
		m_MidNum++;
		delete[] fcl.line;
		delete[] fcr.line;
		return 999;
	}

	if(ret > 0)
	{
		for(int i=bl;i<=be;i++)
		{
			//add lmb ;20030301
			ret = fcr.line[i];
			if(ret<1) ret = 1;
			//end add
			float ax=fcl.line[i];
			if(i-bl<ret)
			{
				for(j=bl;j<=i;j++)
					ax=min(ax,fcl.line[j]);
			}
			else
			{
				for(j=i-ret+1;j<=i;j++)
					ax=min(ax,fcl.line[j]);
			}
			m_MidArray[m_MidNum].line[i] = ax;
		}
	}
	else
	{
		float ax=fcl.line[bl];
		for(int i=bl;i<=be;i++)
		{
//			float ax=fcl.line[i-ret+1];
			ax=min(ax,fcl.line[i]);
			m_MidArray[m_MidNum].line[i] = ax;
		}
	}

	m_table.push_b(PCMB+m_MidNum);
	m_MidNum++;
	
	delete[] fcl.line;
	delete[] fcr.line;
	return 0;
}
//sar
int CFormularComputeParent::Func8037()
{	
	int cl,cr,ce,j;
	ARRAY_BE fcl,fcr,fce;
	
	fcl.line = new float[m_MaxPoint];
	fcr.line = new float[m_MaxPoint];
	fce.line = new float[m_MaxPoint];
	
	cl=m_table.pop_it();
	GetData(cl,fcl);
	cr=m_table.pop_it();
	GetData(cr,fcr);
	ce=m_table.pop_it();
	GetData(ce,fce);
/*SAR(N,S,M),N为计算周期,S为步长,M为极值
例如SAR(10,2,20)表示计算10日抛物转向，步长为2%，极限值为20%*/

	int nl = fcl.b;
	nl =(int)fcl.line[nl];
	if(nl<1)
	{
		delete[] fcl.line;
		delete[] fcr.line;
		return 37;
	}

	int nr = fcr.b;
	float cfr =(float)(fcr.line[nr]/100);
	if(cfr<0.001||cfr>0.05) 
	{
		delete[] fcl.line;
		delete[] fcr.line;
		return 37;
	}

	int ne = fce.b;
	float cfe =(float)(fce.line[ne]/100);
	if(cfe<0.01||cfe>0.8) return 37;

	m_MidArray[m_MidNum].kind = ARRAY_BE::Normal;
	m_MidArray[m_MidNum].line = new float[m_MaxPoint];
	m_MidArray[m_MidNum].b =fcl.b+nl-1; 
	m_MidArray[m_MidNum].e =fcl.e;
	
	int bl = m_MidArray[m_MidNum].b;
	int be = m_MidArray[m_MidNum].e;
	if(bl>be||be>m_MaxPoint-1||bl<0)
	{
		m_table.push_b(PCMB+m_MidNum);
		m_MidNum++;
		delete[] fcl.line;
		delete[] fcr.line;
		delete[] fce.line;
		return 999;
	}
	int fl;  // 0 多头继续 1 空头继续
			//  2 多头开始 3 空头开始
	float hl,cft;
	float newhigh,newlow;
	if((float)((m_pData[bl].close>m_pData[bl-nl+1].close)))
		fl=2;
	else
		fl=3;
	for(int i=bl;i<=be;i++)
	{
		switch(fl)
		{
		case 0:
			if(m_pData[i].high>newhigh)
			{
				newhigh =m_pData[i].high;
			}
//			if(m_pData[i].high>m_pData[i-1].high)
				cft+=cfr;

			if(cft>cfe)
				cft =cfe;

			if(m_pData[i].high>m_pData[i-1].high)
			{
				m_MidArray[m_MidNum].line[i] =
					m_MidArray[m_MidNum].line[i-1]+cft*
					(m_pData[i-1].high-
					m_MidArray[m_MidNum].line[i-1]);
			}
			else
				m_MidArray[m_MidNum].line[i] =
					m_MidArray[m_MidNum].line[i-1];
			if(m_pData[i].close<=m_MidArray[m_MidNum].line[i-1])
				fl=3;
			break;
		case 1:
			if(m_pData[i].low<newlow)
			{
				newlow =m_pData[i].low;
			}
//			if(m_pData[i].low<m_pData[i-1].low)
				cft+=cfr;

			if(cft>cfe) 
				cft =cfe;

			if(m_pData[i].low<m_pData[i-1].low)
			{
				m_MidArray[m_MidNum].line[i] =
					m_MidArray[m_MidNum].line[i-1] + cft*
					(m_pData[i-1].low-
					m_MidArray[m_MidNum].line[i-1]);
			}
			else
				m_MidArray[m_MidNum].line[i] =
					m_MidArray[m_MidNum].line[i-1];
			if(m_pData[i].close>=m_MidArray[m_MidNum].line[i-1])
				fl=2;

			break;
		case 2:
			hl=m_pData[i-nl+1].low;
			newhigh =m_pData[i-nl+1].high;
			for(j=i-nl+2;j<=i;j++)
			{
					if(hl>m_pData[j].low)
						hl=m_pData[j].low;
					if(newhigh<m_pData[j].high)
						newhigh = m_pData[j].high;
			}
			m_MidArray[m_MidNum].line[i] =hl;
			fl = 0;
			cft=0;
			break;
		case 3:
			hl=m_pData[i-nl+1].high;
			newlow =m_pData[i-nl+1].low;
			for(j=i-nl+2;j<=i;j++)
			{
				if(hl<m_pData[j].high)
					hl=m_pData[j].high;
				if(newlow<m_pData[j].low)
					newlow = m_pData[j].low;
			}
			fl=1;
			cft=0;
			m_MidArray[m_MidNum].line[i] =hl;
			break;
		}
	}
	
	m_table.push_b(PCMB+m_MidNum);
	m_MidNum++;
	
	delete[] fcl.line;
	delete[] fcr.line;
	delete[] fce.line;

	return 0;
}
int CFormularComputeParent::Func8038()
 {
	int cl,cr,ce,j;
	ARRAY_BE fcl,fcr,fce;
	
	fcl.line = new float[m_MaxPoint];
	fcr.line = new float[m_MaxPoint];
	fce.line = new float[m_MaxPoint];
	
	cl=m_table.pop_it();
	GetData(cl,fcl);
	cr=m_table.pop_it();
	GetData(cr,fcr);
	ce=m_table.pop_it();
	GetData(cr,fce);

	int nl = fcl.b;
	nl =(int)fcl.line[nl];
	if(nl<1) 
	{
		delete[] fcl.line;
		delete[] fcr.line;
		delete[] fce.line;
		return 38;
	}

	int nr = fcr.b;
	float cfr =(float)(fcr.line[nr]/100);
	if(cfr<0.01||cfr>0.05) 
	{
		delete[] fcl.line;
		delete[] fcr.line;
		delete[] fce.line;
		return 38;
	}

	int ne = fce.b;
	float cfe =(float)(fce.line[ne]/100);
	if(cfe<0.1||cfe>0.8) 
	{
		delete[] fcl.line;
		delete[] fcr.line;
		delete[] fce.line;
		return 38;
	}

	m_MidArray[m_MidNum].kind = ARRAY_BE::Normal;
	m_MidArray[m_MidNum].line = new float[m_MaxPoint];
	m_MidArray[m_MidNum].b =fcl.b+nl-1; 
	m_MidArray[m_MidNum].e =fcl.e;
	
	int bl = m_MidArray[m_MidNum].b;
	int be = m_MidArray[m_MidNum].e;
	if(bl>be||be>m_MaxPoint-1||bl<0)
	{
		m_table.push_b(PCMB+m_MidNum);
		m_MidNum++;
		delete[] fcl.line;
		delete[] fcr.line;
		delete[] fce.line;
		return 999;
	}
	int fl;  // 0 多头继续 1 空头继续
			//  2 多头开始 3 空头开始
	float hl,cft;
	float newhigh,newlow;
	if((float)((m_pData[bl].close>m_pData[bl-nl+1].close)))
		fl=2;
	else
		fl=3;
	for(int i=bl;i<=be;i++)
	{
		switch(fl)
		{
		case 0:
			if(m_pData[i].high>newhigh)
			{
				newhigh =m_pData[i].high;
				cft+=cfr;
			}
			if(cft>cfe)
				cft =cfe;
			if(m_pData[i].high>m_pData[i-1].high)
			{
				m_MidArray[m_MidNum].line[i] =
					m_MidArray[m_MidNum].line[i-1]+cft*
					(m_pData[i-1].high-
					m_MidArray[m_MidNum].line[i-1]);
			}
			else
				m_MidArray[m_MidNum].line[i] =
					m_MidArray[m_MidNum].line[i-1];
			if(m_pData[i].close<=m_MidArray[m_MidNum].line[i-1])
				fl=3;
			break;
		case 1:
			if(m_pData[i].low>newlow)
			{
				newlow =m_pData[i].low;
				cft+=cfr;
			}
			if(cft>cfe) 
				cft =cfe;
			if(m_pData[i].low<m_pData[i-1].low)
			{
				m_MidArray[m_MidNum].line[i] =
					m_MidArray[m_MidNum].line[i-1]+cft*
					(m_pData[i-1].low-
					m_MidArray[m_MidNum].line[i-1]);
			}
			else
				m_MidArray[m_MidNum].line[i] =
					m_MidArray[m_MidNum].line[i-1];
			if(m_pData[i].close>=m_MidArray[m_MidNum].line[i-1])
				fl=3;

			break;
		case 2:
			hl=m_pData[i-nl+1].low;
			newhigh =m_pData[i-nl+1].high;
			for(j=i-nl+2;j<=i;j++)
			{
					if(hl>m_pData[j].low)
						hl=m_pData[j].low;
					if(newhigh<m_pData[j].high)
						newhigh = m_pData[j].high;
			}
			m_MidArray[m_MidNum].line[i] =hl;
			fl = 0;
			cft=0;
			break;
		case 3:
			hl=m_pData[i-nl+1].high;
			newlow =m_pData[i-nl+1].low;
			for(j=i-nl+2;j<=i;j++)
			{
				if(hl<m_pData[j].high)
					hl=m_pData[j].high;
				if(newlow<m_pData[j].low)
					newlow = m_pData[j].low;
			}
			fl=1;
			cft=0;
			m_MidArray[m_MidNum].line[i] =hl;
			break;
		}
	}
	
	m_table.push_b(PCMB+m_MidNum);
	m_MidNum++;
	
	delete[] fcl.line;
	delete[] fcr.line;
	delete[] fce.line;

	return 0;
}
//  ma
int CFormularComputeParent::Func8040() 
{
	int cl,cr,ret,j;
	ARRAY_BE fcl,fcr;
	
	fcl.line = new float[m_MaxPoint];
	fcr.line = new float[m_MaxPoint];
	
	cl=m_table.pop_it();
	GetData(cl,fcl);
	cr=m_table.pop_it();
	GetData(cr,fcr);
	
		//change lmb ;20030301
	ret = GetBeginFoot( fcr);
		//end add

	if(ret<1) 
	{
		delete[] fcl.line;
		delete[] fcr.line;
		return 40;
	}
	
	m_MidArray[m_MidNum].kind = ARRAY_BE::Normal;
	m_MidArray[m_MidNum].line = new float[m_MaxPoint];
	m_MidArray[m_MidNum].b =fcl.b+ret-1; 
	m_MidArray[m_MidNum].e =fcl.e;
	
	int bl = m_MidArray[m_MidNum].b;
	int be = m_MidArray[m_MidNum].e;
	if(bl>be||be>m_MaxPoint-1||bl<0)
	{
		m_table.push_b(PCMB+m_MidNum);
		m_MidNum++;
		delete[] fcl.line;
		delete[] fcr.line;
		return 999;
	}

	for(int i=bl;i<=be;i++)
	{
		float ax=0;
		//add lmb ;20030301
		ret = fcr.line[i];
		if(ret<1) ret = 1;
		//end add

		for(j=i-ret+1;j<=i;j++)
				ax+=fcl.line[j];
		ax/=ret;
		m_MidArray[m_MidNum].line[i] = ax;
	}
	
	m_table.push_b(PCMB+m_MidNum);
	m_MidNum++;
	
	delete[] fcl.line;
	delete[] fcr.line;
	return 0;
}
// ref
int CFormularComputeParent::Func8039() 
{
	int cl,cr,ret;
	ARRAY_BE fcl,fcr;
	
	fcl.line = new float[m_MaxPoint];
	fcr.line = new float[m_MaxPoint];
	
	cl=m_table.pop_it();
	GetData(cl,fcl);
	cr=m_table.pop_it();
	GetData(cr,fcr);
	
		//change lmb ;20030301
	ret = GetBeginFoot( fcr);
		//end add
//	ret = fcr.b;
//	ret =(int)fcr.line[ret];
	
	if(ret<1) 
	{
		delete[] fcl.line;
		delete[] fcr.line;
		return 39;
	}
	
	m_MidArray[m_MidNum].kind = ARRAY_BE::Normal;
	m_MidArray[m_MidNum].line = new float[m_MaxPoint];
	m_MidArray[m_MidNum].b =fcl.b+ret; 
	m_MidArray[m_MidNum].e =fcl.e;
	
	int bl = m_MidArray[m_MidNum].b;
	int be = m_MidArray[m_MidNum].e;
	if(bl>be||be>m_MaxPoint-1||bl<0)
	{
		m_table.push_b(PCMB+m_MidNum);
		m_MidNum++;
		delete[] fcl.line;
		delete[] fcr.line;
		return 999;
	}

	for(int i=bl;i<=be;i++)
	{
		//add lmb ;20030301
		ret = fcr.line[i];
		if(ret<1) ret = 1;
		//end add
		m_MidArray[m_MidNum].line[i] = fcl.line[i-ret];
	}
	
	m_table.push_b(PCMB+m_MidNum);
	m_MidNum++;
	
	delete[] fcl.line;
	delete[] fcr.line;
	return 0;
}
// std
int CFormularComputeParent::Func8041()
 {
	int cl,cr,ret,j;
	ARRAY_BE fcl,fcr;
	
	fcl.line = new float[m_MaxPoint];
	fcr.line = new float[m_MaxPoint];
	
	cl=m_table.pop_it();
	GetData(cl,fcl);
	cr=m_table.pop_it();
	GetData(cr,fcr);
	
	ret = fcr.b;
	ret =(int)fcr.line[ret];
	
	if(ret<2) 
	{
		delete[] fcl.line;
		delete[] fcr.line;
		return 41;
	}
	
	m_MidArray[m_MidNum].kind = ARRAY_BE::Normal;
	m_MidArray[m_MidNum].line = new float[m_MaxPoint];
	m_MidArray[m_MidNum].b =fcl.b+ret-1; 
	m_MidArray[m_MidNum].e =fcl.e;
	
	int bl = m_MidArray[m_MidNum].b;
	int be = m_MidArray[m_MidNum].e;
	if(bl>be||be>m_MaxPoint-1||bl<0)
	{
		m_table.push_b(PCMB+m_MidNum);
		m_MidNum++;
		delete[] fcl.line;
		delete[] fcr.line;
		return 999;
	}

	for(int i=bl;i<=be;i++)
	{
		float ax=0,af=0;
		for(j=i-ret+1;j<=i;j++)
		{
			ax+=fcl.line[j]*fcl.line[j];
			af+=fcl.line[j];
		}
		ax=(float)sqrt(FABSMY(ax*ret-af*af)/ret/(ret-1));
		m_MidArray[m_MidNum].line[i] = ax;
	}
/*	for(int i=bl;i<=be;i++)
	{
		float ax=0,af=0;
		for(j=i-ret+1;j<=i;j++)
		{
			af+=fcl.line[j];
		}
		af/=ret;
		for(j=i-ret+1;j<=i;j++)
		{
			ax+=(fcl.line[j]-af)*(fcl.line[j]-af);
		}
		ax=(float)sqrt(ax/(ret-1));
		m_MidArray[m_MidNum].line[i] = ax;
	}*/	
	m_table.push_b(PCMB+m_MidNum);
	m_MidNum++;
	
	delete[] fcl.line;
	delete[] fcr.line;
	return 0;
}
//  stdp
int CFormularComputeParent::Func8042() 
{
	int cl,cr,ret,j;
	ARRAY_BE fcl,fcr;
	
	fcl.line = new float[m_MaxPoint];
	fcr.line = new float[m_MaxPoint];
	
	cl=m_table.pop_it();
	GetData(cl,fcl);
	cr=m_table.pop_it();
	GetData(cr,fcr);
	
	ret = fcr.b;
	ret =(int)fcr.line[ret];
	
	if(ret<1) 
	{
		delete[] fcl.line;
		delete[] fcr.line;
		return 42;
	}
	
	m_MidArray[m_MidNum].kind = ARRAY_BE::Normal;
	m_MidArray[m_MidNum].line = new float[m_MaxPoint];
	m_MidArray[m_MidNum].b =fcl.b+ret-1; 
	m_MidArray[m_MidNum].e =fcl.e;
	
	int bl = m_MidArray[m_MidNum].b;
	int be = m_MidArray[m_MidNum].e;
	if(bl>be||be>m_MaxPoint-1||bl<0)
	{
		m_table.push_b(PCMB+m_MidNum);
		m_MidNum++;
		delete[] fcl.line;
		delete[] fcr.line;
		return 999;
	}

	for(int i=bl;i<=be;i++)
	{
		float ax=0,af=0;
		for(j=i-ret+1;j<=i;j++)
		{
			ax+=fcl.line[j]*fcl.line[j];
			af+=fcl.line[j];
		}
		ax=(float)sqrt((ax*ret-af*af)/ret/ret);
		m_MidArray[m_MidNum].line[i] = ax;
	}
	
	m_table.push_b(PCMB+m_MidNum);
	m_MidNum++;
	
	delete[] fcl.line;
	delete[] fcr.line;
	return 0;
}
//sum
int CFormularComputeParent::Func8043() 
{
	int cl,cr,ret,j;
	ARRAY_BE fcl,fcr;
	
	fcl.line = new float[m_MaxPoint];
	fcr.line = new float[m_MaxPoint];
	
	cl=m_table.pop_it();
	GetData(cl,fcl);
	cr=m_table.pop_it();
	GetData(cr,fcr);
	
		//change lmb ;20030301
	ret = GetBeginFoot( fcr);
		//end add
//	ret = fcr.b;
//	ret =(int)fcr.line[ret];
	if(ret<0) 
	{
		delete[] fcl.line;
		delete[] fcr.line;
		return 43;//lmb6.15
	}

	m_MidArray[m_MidNum].kind = ARRAY_BE::Normal;
	m_MidArray[m_MidNum].line = new float[m_MaxPoint];
	m_MidArray[m_MidNum].b =fcl.b+ret-1; 
	m_MidArray[m_MidNum].e =fcl.e;
	
	int bl = m_MidArray[m_MidNum].b;
	int be = m_MidArray[m_MidNum].e;
	if(ret==0)
	{
		m_MidArray[m_MidNum].b =fcl.b;
		bl = m_MidArray[m_MidNum].b;
	}

	if(be>=bl&&bl>=0&&be>=0)
	{
		if(ret==0)
		{
			m_MidArray[m_MidNum].line[bl]=fcl.line[bl];
			for(int i=bl+1;i<=be;i++)
			{
				m_MidArray[m_MidNum].line[i]=0;
				float ax=0;
				for(j=bl;j<=i;j++)
				{
					ax+=fcl.line[j];
				}
				m_MidArray[m_MidNum].line[i] = fcl.line[i] + m_MidArray[m_MidNum].line[i-1];
			}
		}
		else
		{
			for(int i=bl;i<=be;i++)
			{
				float ax=0;
				//add lmb ;20030301
				ret = fcr.line[i];
				if(ret<1) ret = 1;
				//end add
				for(j=i-ret+1;j<=i;j++)
				{
					ax+=fcl.line[j];
				}
				m_MidArray[m_MidNum].line[i] = ax;
			}
		}
	}	
	m_table.push_b(PCMB+m_MidNum);
	m_MidNum++;
	
	delete[] fcl.line;
	delete[] fcr.line;
	return 0;
}
// varp
int CFormularComputeParent::Func8044() 
{
	int cl,cr,ret,j;
	ARRAY_BE fcl,fcr;
	
	fcl.line = new float[m_MaxPoint];
	fcr.line = new float[m_MaxPoint];
	
	cl=m_table.pop_it();
	GetData(cl,fcl);
	cr=m_table.pop_it();
	GetData(cr,fcr);
	
	ret = fcr.b;
	ret =(int)fcr.line[ret];
	
	if(ret<2) 
	{
		delete[] fcl.line;
		delete[] fcr.line;
		return 44;
	}
	
	m_MidArray[m_MidNum].kind = ARRAY_BE::Normal;
	m_MidArray[m_MidNum].line = new float[m_MaxPoint];
	m_MidArray[m_MidNum].b =fcl.b+ret-1; 
	m_MidArray[m_MidNum].e =fcl.e;
	
	int bl = m_MidArray[m_MidNum].b;
	int be = m_MidArray[m_MidNum].e;
	if(bl>be||be>m_MaxPoint-1||bl<0)
	{
		m_table.push_b(PCMB+m_MidNum);
		m_MidNum++;
		delete[] fcl.line;
		delete[] fcr.line;
		return 999;
	}

	for(int i=bl;i<=be;i++)
	{
		float ax=0,af=0;
		for(j=i-ret+1;j<=i;j++)
		{
			ax+=fcl.line[j]*fcl.line[j];
			af+=fcl.line[j];
		}
		ax=(ax*ret-af*af)/ret/ret;
		m_MidArray[m_MidNum].line[i] = ax;
	}
	
	m_table.push_b(PCMB+m_MidNum);
	m_MidNum++;
	
	delete[] fcl.line;
	delete[] fcr.line;
		return 0;
}
//var
int CFormularComputeParent::Func8045()
{
	int cl,cr,ret,j;
	ARRAY_BE fcl,fcr;
	
	fcl.line = new float[m_MaxPoint];
	fcr.line = new float[m_MaxPoint];
	
	cl=m_table.pop_it();
	GetData(cl,fcl);
	cr=m_table.pop_it();
	GetData(cr,fcr);
	
	ret = fcr.b;
	ret =(int)fcr.line[ret];
	
	if(ret<1)
	{
		delete[] fcl.line;
		delete[] fcr.line;
		return 45;
	}
	
	m_MidArray[m_MidNum].kind = ARRAY_BE::Normal;
	m_MidArray[m_MidNum].line = new float[m_MaxPoint];
	m_MidArray[m_MidNum].b =fcl.b+ret-1; 
	m_MidArray[m_MidNum].e =fcl.e;
	
	int bl = m_MidArray[m_MidNum].b;
	int be = m_MidArray[m_MidNum].e;
	if(bl>be||be>m_MaxPoint-1||bl<0)
	{
		m_table.push_b(PCMB+m_MidNum);
		m_MidNum++;
		delete[] fcl.line;
		delete[] fcr.line;
		return 999;
	}

	for(int i=bl;i<=be;i++)
	{
		float ax=0,af=0;
		for(j=i-ret+1;j<=i;j++)
		{
			ax+=fcl.line[j]*fcl.line[j];
			af+=fcl.line[j];
		}
		ax=(ax*ret-af*af)/ret/(ret-1);
		m_MidArray[m_MidNum].line[i] = ax;
	}
	
	m_table.push_b(PCMB+m_MidNum);
	m_MidNum++;
	delete[] fcl.line;
	delete[] fcr.line;
	return 0;
}
//sma
int CFormularComputeParent::Func8046()
{
	int cl,cr,ce,ret,j;
	ARRAY_BE fcl,fcr,fce;
	
	fcl.line = new float[m_MaxPoint];
	fcr.line = new float[m_MaxPoint];
	fce.line = new float[m_MaxPoint];
	
	cl=m_table.pop_it();
	GetData(cl,fcl);
	cr=m_table.pop_it();
	GetData(cr,fcr);
	ce=m_table.pop_it();
	GetData(ce,fce);
	
	ret = fcr.b;
	ret =(int)fcr.line[ret];
	
	if(ret<1)
	{
		delete[] fcl.line;
		delete[] fcr.line;
		delete[] fce.line;
		return 46;
	}
	int ret2 = fce.b;
	ret2 =(int)fce.line[ret2];
	if(ret2>=ret)
	{
		delete[] fcl.line;
		delete[] fcr.line;
		delete[] fce.line;
//		AfxMessageBox("请修改函数“sma”的第2、3个参数，使得第2个参数大于第3个参数");
		return 46;
	}
	if(ret2<1)
	{
		delete[] fcl.line;
		delete[] fcr.line;
		delete[] fce.line;
		return 46;
	}
	
	m_MidArray[m_MidNum].kind = ARRAY_BE::Normal;
	m_MidArray[m_MidNum].line = new float[m_MaxPoint];
	m_MidArray[m_MidNum].b =fcl.b+ret-1; 
	m_MidArray[m_MidNum].e =fcl.e;
	
	int bl = m_MidArray[m_MidNum].b;
	int be = m_MidArray[m_MidNum].e;
	if(bl>be||be>m_MaxPoint-1||bl<0)
	{
		m_table.push_b(PCMB+m_MidNum);
		m_MidNum++;
		delete[] fcl.line;
		delete[] fcr.line;
		delete[] fce.line;
		return 999;
	}

	if(be>=bl&&bl>=0&&be>=0)
	{
		m_MidArray[m_MidNum].line[bl] = 0;
		for(j=bl-ret+1;j<=bl;j++)
		{
			m_MidArray[m_MidNum].line[bl]+=fcl.line[j];
		}
		m_MidArray[m_MidNum].line[bl] /= ret;

		double db = m_MidArray[m_MidNum].line[bl];
		for(int i=bl+1;i<=be;i++)
		{
/*//求移动平均。
用法:
SMA(X,N,M),求X的N日移动平均，M为权重。
算法: 若Y=SMA(X,N,M)
则 Y=[M*X+(N-M)*Y')/N,其中Y'表示上一周期Y值,N必须大于M。
例如：SMA(CLOSE,30,1)表示求30日移动平均价*/
			db = (ret2*fcl.line[i]+(ret-ret2)*db)/ret;
			m_MidArray[m_MidNum].line[i] = db ;
		}
	}
	
	m_table.push_b(PCMB+m_MidNum);
	m_MidNum++;
	delete[] fcl.line;
	delete[] fcr.line;
	delete[] fce.line;
	return 0;
}

void CFormularComputeParent::GetMsg(CString& str,int& pos)
{
	if(m_strErrMsg.IsEmpty ())
		str = " ";
	else
		str=m_strErrMsg;
	pos=m_nErrPos;
}


int CFormularComputeParent::GetLine(CString& LineName, ARRAY_BE& pline)
{
	int fl=1;
	for(int i=0;i<m_RlineNum;i++)
	{
		CString s1 = m_RlineName[i];
		s1.MakeLower ();
		CString s2 = LineName;
		s2.MakeLower ();
		if(s1==s2)
		{
			fl=0;
			int bl = m_Rline[i].b;
			int be = m_Rline[i].e;
			if(bl>be||bl<0||be<0||be>=m_MaxPoint)
				return 1;

			pline = m_Rline[i];
/*			pline.b = bl;
			pline.e = be;

			for(int j=bl;j<=be;j++)
				pline.line[j] = m_Rline[i].line[j];*/
			break;
		}
	}
	return fl;
}

int CFormularComputeParent::GetLine(int& index, ARRAY_BE& pline,CString& LineName)
{
	if(index >= m_RlineNum)
		return 1;
	int bl = m_Rline[index].b;
	int be = m_Rline[index].e;
	pline.b = bl;
	pline.e = be;
	LineName = m_RlineName[index];

//	for(int j=0;j<bl;j++)
//		pline.line[j] = 0;
	if(bl>be||bl<0||be<0||be>=m_MaxPoint)
		return 1;
	pline = m_Rline[index];
//	for(int j=bl;j<=be;j++)
//		pline.line[j] = m_Rline[index].line[j];
	return 0;
}
int CFormularComputeParent::GetIndexPoint(int &index,float &nPoint)
{
	if(index >= m_RlineNum)
		return 1;
	int bl = m_Rline[index].b;
	int be = m_Rline[index].e;

	if(bl>be||bl<0||be<0||be>=m_MaxPoint)
		return 1;
	nPoint=m_Rline[index].line[be];
	return 0;
}

int CFormularComputeParent::AddLine(CString& LineName, ARRAY_BE& pline)
{
	if(m_VarNum >=MIDE)
		return 1;

	m_WordTableNew[LineName]=(void*)(MIDB+m_VarNum);

	m_MidArray[m_MidNum].kind = ARRAY_BE::Normal;
	m_VarArray[m_VarNum].line = new float[m_MaxPoint];

	m_VarArray[m_VarNum].b = pline.b;
	m_VarArray[m_VarNum].e = pline.e;
	
	for(int i=pline.b;i<=pline.e;i++)
		m_VarArray[m_VarNum].line[i] = pline.line[i];

	m_VarName[m_VarNum] = LineName;					
	m_VarNum++;
	m_nExec     =0;
	return 0;
}

int CFormularComputeParent::Relolve(int cl)//处理引用
//cl为这个引用的ID号
{
	CString  str;//指标名称 
	CString  ename;//指标公式
	CString  linename;//线条名称

	CString  sl;
	sl=m_QuoteArray[cl-QUB];
	sl.Replace ("$",".");
	sl.Replace ("#",".");

	int tu;
	ARRAY_BE  fcl;
	int equationKind = 0;
	int nPeriods = 0;
	if(GetQuote(sl,ename,linename,tu,equationKind,nPeriods))
	{
		ASSERT(FALSE);
		return 1;
	}
	if(equationKind ==1000)//神经网络函数 equationKind =1000
	{
		return 1;
	}//

	int nParamInTable = 0;
	if(!LookupParameterTable(cl,nParamInTable))
	{
		ASSERT(FALSE);
		return 1;
	}

	if(equationKind<10 || equationKind>=100)
	{
		cl = m_MaxPoint ;

		ename.MakeLower();
		linename.MakeLower();
		sl.MakeLower();

		//GET LJISHU
		CString zb;
		zb = ename;
		zb.MakeUpper();
		CFormularContent*	pIndex = NULL;
		int nKindEqt = equationKind;
		if(equationKind>=100)
			nKindEqt = equationKind-100;

		switch(nKindEqt)
		{
		case 0:
			pIndex=(CFormularContent*)(m_pDoc->LookUpArray (m_pDoc->m_formuar_index,zb));
			break;
		case 1:
			pIndex=(CFormularContent*)(m_pDoc->LookUpArray (m_pDoc->m_formuar_choose ,zb));
			break;
		case 2:
			pIndex=(CFormularContent*)(m_pDoc->LookUpArray (m_pDoc->m_formuar_kline ,zb));
			break;
		case 3:
			pIndex=(CFormularContent*)(m_pDoc->LookUpArray (m_pDoc->m_formuar_master,zb));
			break;
		}
		if(pIndex==NULL)
		{
			ASSERT(FALSE);
			return 1;
		}
		
		Kline * pKline = NULL;

		//if is other stock
		int nKlineInOut = m_MaxPoint;
		CString symbol = m_symbol;
		if(equationKind>=100)
		{
			int nFind = sl.Find (".",0);
			if(nFind<=0)
			{
				ASSERT(FALSE);
				return 1;
			}

			symbol= sl.Left (nFind);
			symbol.MakeUpper ();
			pKline = new Kline;
			nKlineInOut=ReadKline(symbol,m_stkKind,pKline);
			cl = nKlineInOut;
		}
		else 
			pKline = m_pData;

		//end if
		Kline * pKlineNew = NULL;
		bool bTransfer = false;

		bool bCalcHs = false;//是否在计算日线周期计算明细周期的数据 lmb 20030712
		bool bCalcMin5 = false;//是否在计算日线周期计算5分钟周期的数据 lmb 20031022

		if(nPeriods>0 && m_nKindKline>0 &&m_nKindKline!= NUM_MANY_DAY)
		//
		{
			//transfer
			if( m_nKindKline == HS_KLINE)
			{
				if(nPeriods==MIN1_KLINE)
					bTransfer = true;
			}
			else
			{
				if(nPeriods<5)
				{
					if(m_nKindKline<5&&m_nKindKline>0)
					{
						if(nPeriods - m_nKindKline>=1) bTransfer = true;
					}
				}
				else
				{
					if(m_nKindKline<9 && m_nKindKline>4)
					{
						if(nPeriods - m_nKindKline>=1) bTransfer = true;
					}
				}
			}

		}
		if( m_nKindKline == 5)
		{
			if(nPeriods == HS_KLINE )
				bCalcHs = true;
			if(nPeriods == 1 )
				bCalcMin5 = true;
		}

		//在日线周期引用5分钟k线
		if(bCalcMin5 == true)
		{
			CalcMin5InDay(pIndex, nParamInTable, linename, tu);
			//clear
			return 0;
		}

		//
		if(bTransfer == true)//&&equationKind<10)
		{
			CTaiKlineTransferKline trans;
			if(equationKind>=100)
			{
				pKlineNew = pKline;
				trans.TransferKlineHistory(symbol,m_stkKind,pKlineNew, nKlineInOut,nPeriods,true);//used to transfer k line data to another kind of k line
			}
			else
			{
				pKlineNew = new Kline[m_MaxPoint];
				memcpy(pKlineNew,m_pData,m_MaxPoint*sizeof(Kline));
				trans.TransferKlineHistory(symbol,m_stkKind,pKlineNew, nKlineInOut,nPeriods,false);//used to transfer k line data to another kind of k line
			}
			pKline = pKlineNew;
			if(nKlineInOut<=0)
			{
				delete[] pKlineNew;
				ASSERT(FALSE);
				return 1;
			}
			cl = nKlineInOut;
		}

		int nMaxPoint = (m_MaxPoint>cl?m_MaxPoint:cl);
		fcl.line = new float[nMaxPoint];
		memset(fcl.line,0,(m_MaxPoint>cl?m_MaxPoint:cl));

		{
			CFormularComputeParent temp(m_MinPoint,cl-1,pKline,pIndex,symbol,m_stkKind,this->m_pBuySellList ,m_nKindKline);
			
			if(tu>8)
				tu=0;
			if(nParamInTable == tu)
			{
				for(int i=0;i<tu;i++)
				{
					cl=m_table.pop_it();
					GetData(cl,fcl);
					CString sname(pIndex->namePara[i] );
					sname.MakeLower();
					temp.AddPara(sname,fcl.line[fcl.b]);
				}
			}
			else	if(nParamInTable == 0)
			{
				for(int i=0;i<tu;i++)
				{
					CString sname(pIndex->namePara[i] );
					sname.MakeLower();
					temp.AddPara(sname,
						CFormularContent::GetParamDataEach(i,m_nKindKline, pIndex));//
				}
			}
			else
				ASSERT(FALSE);

			if(temp.FormularCompute()!=0)
			{
				delete[] fcl.line;
				if(bTransfer == true)
					if(pKlineNew!=0)
						delete[] pKlineNew;
				return 1;//m_bTotalData
			}
			if(temp.m_bTotalData != 0) m_bTotalData = temp.m_bTotalData;

			m_MidArray[m_MidNum].kind = ARRAY_BE::Normal;
			m_MidArray[m_MidNum].line = new float[m_MaxPoint];
			m_MidArray[m_MidNum].b=0;
			m_MidArray[m_MidNum].e=m_MaxPoint-1;

			int nRtn = 0;
			if(bTransfer ==true || equationKind>=100)
			{
				//to transfer value 
				nRtn=temp.GetLine(linename,fcl);
				if(nRtn == 1)
				{
					delete[] fcl.line;

					if(bTransfer == true)
						if(pKlineNew!=0)
							delete[] pKlineNew;
	//				ASSERT(FALSE);
					return 1;
				}
				int b = fcl.b;
				int bMidArray=0;
				int j=0;
				{
					int tmb ;
					tmb = pKline[b].day;
					for( j=0;j<m_MaxPoint;j++)
					{
						if(m_pData[j].day >= tmb)
							break;
					}
					m_MidArray[m_MidNum].b=j<m_MaxPoint?j:(m_MaxPoint-1);
				}

				int timeBegin = pKline[b].day;//fcl.line[b];
				int k=m_MidArray[m_MidNum].b;
				for( j=b;j<=fcl.e && k<m_MaxPoint;)
				{
					if(m_pData[k].day < pKline[j].day)
					{
						m_MidArray[m_MidNum].line[k] = fcl.line[j];
						k++;
					}
					else if(m_pData[k].day == pKline[j].day)
					{
						m_MidArray[m_MidNum].line[k] = fcl.line[j];
						k++;
						j++;
					}
					else
						j++;
				}
				if(k<=m_MaxPoint-1)
				{
					int nK=0;
					if(nK>0)
						nK = k-1;
					for(;k<m_MaxPoint;k++)
						m_MidArray[m_MidNum].line[k] =  m_MidArray[m_MidNum].line[nK];
				}

			}
			else
				nRtn = temp.GetLine(linename,m_MidArray[m_MidNum]);
			if(nRtn ==1)
			{
				delete[]	m_MidArray[m_MidNum].line;
				delete[] fcl.line;
	//			ASSERT(FALSE);
				if(bTransfer == true)
					if(pKlineNew!=0)
						delete[] pKlineNew;
				return 1;
			}
			m_table.push_b(PCMB+m_MidNum);
			m_MidNum++;
		}

		delete[] fcl.line;

		if(bTransfer == true)
			if(pKlineNew!=0)
				delete[] pKlineNew;
	}
	else//such as '1000.high'
	{
		//ename is stock symbol; tu is the kind of data
		//(0=open,1=high,2= low,3= close,4=vol,5=amount,6=advance,7=decline,8=neipan)

		//read from file;
		CString symbol ;
		int nFind = sl.Find (".",0);
		if(nFind<=0)
			return 1;

		symbol= sl.Left (nFind);
		symbol.MakeUpper ();
		int n = 0;
		Kline* pKline = NULL;
		 char *strLabel = symbol.GetBuffer (0);
//		 CReportData *pStockData;
		 int stkKind = GetStockKind(strLabel);//CSharesInformation::GetStockKind(SH_MARKET_EX,strLabel);
/*		 bool bFind = false;
		 {
			 if(stkKind>=0)
				 if(m_pDoc->m_sharesInformation .Lookup (strLabel,pStockData,stkKind))
					 bFind = true;
		 }
		 if(bFind == false)
		 {
			 stkKind = CSharesInformation::GetStockKind(SZ_MARKET_EX,strLabel);
			 if(stkKind>=0)
				 if(m_pDoc->m_sharesInformation .Lookup (strLabel,pStockData,stkKind))
					 bFind = true;
		 }*/
		n=ReadKline(symbol,stkKind,pKline);

		//transfer
		m_MidArray[m_MidNum].kind = ARRAY_BE::Normal;
		m_MidArray[m_MidNum].line = new float[m_MaxPoint];
		int nRate =1;
		if(m_nKindKline>4&&m_nKindKline<9)
			nRate = 24*60*60;
		else
			nRate = 24*60;
		int j =0;
		Kline * pKline2 = new Kline[m_MaxPoint];
		
		int i;
		for(i=m_MinPoint;i<m_MaxPoint;i++)
		{
			if(m_pData[i].day/nRate >= pKline[0].day/nRate)
				break;
		}
		m_MidArray[m_MidNum].e=m_MaxPoint-1;
		if(i<m_MaxPoint)
			m_MidArray[m_MidNum].b=i;
		else
		{
			m_MidArray[m_MidNum].e = -1;
			m_MidArray[m_MidNum].b = 0;
		}

		for(;i<m_MaxPoint && j<n;)
		{
			if(m_pData[i].day/nRate == pKline[j].day/nRate)
			{
				//KlineToLine(Kline *pKline, float & f,int nFlag)
				memcpy(pKline2+i,pKline+j,sizeof(Kline));// = 
				j++;
				i++;
			}
			else if(m_pData[i].day/nRate > pKline[j].day/nRate)
			{
				j++;
			}
			else
			{
				memcpy(pKline2+i,pKline+j,sizeof(Kline));// = 
				pKline2[i].day = m_pData[i].day;// = 
				i++;
			}
		}//pKline2
		if(i<=m_MaxPoint-1)
		{
			int nK =0;
			if(i>0)
				nK = i-1;
			for(;i<m_MaxPoint;i++)
				memcpy(pKline2+i,pKline2+nK,sizeof(Kline));
		}


		int nSubMem = equationKind-10;
		for(i=m_MinPoint;i<m_MaxPoint;i++)
		{
			KlineToLine(pKline2+i, m_MidArray[m_MidNum].line[i],nSubMem );
		}

		if(pKline2!=NULL)
			delete[] pKline2;
		if(pKline!=NULL)
			delete[] pKline;

		m_table.push_b(PCMB+m_MidNum);
		m_MidNum++;
		//
	}
	return 0;
}

 int CFormularComputeParent::GetQuote(CString &str, CString &zb, CString &cline, int &tu,int &equationKind,int &nPeriods)//equationKind is the kind of CFormularContent(0 is zhibiao ,1 is xuangu,2 is klinemode,3 is stock system,10 is other stock's kline price)
//str引用的总字符串，zb指标名字，cline线的名字，tu参数个数
//equationKind is the kind of CFormularContent(0 is zhibiao ,1 is xuangu,2 is klinemode,3 is stock system,10 is other stock's kline price)
//nPeriods is the kind of periods(0 is no period,35=min1,1=min5,2=min15,3=min30,4=min60,5=day,6=week;7=month;9=year)
//返回值：0为正确，1为没有找到指标，2为没有找到线
{
	 //parse string 
	 CStringArray sName;
	str.Replace ("$",".");
	str.Replace ("#",".");
	 int len =str.GetLength(); 
	 if(len<=0)
		 return 1;

	 //神经网络函数 equationKind =1000
	 if(str[0] == '#')
	 {
		 int n = str.Delete (0);
		 if(n<=0)
			 return 1;
		 CFile fl;
		 str = "知识数据库\\"+str;
		 if(!fl.Open(str+".wgt",CFile::modeRead))
			 return 1;
		 tu=1;
		 equationKind =1000;
		 return 0;
	 }
	 //end 神经网络函数

	 int lb = 0;
	 while(true)
	 {
		 int le=str.Find('.',lb);
		 CString s;

		 if(le<0)
			 s =  str.Mid(lb,len-lb);
		 else
			s =  str.Mid(lb,le-lb);
		s.TrimLeft (" ");
		s.TrimRight (" ");
		s.MakeUpper();
		sName.Add (s);
		if(le<0)
			break;

		lb = le+1;
	 }
	 if(sName.GetSize ()<=0)
		 return 1;

	 //if it is other stock
	 CReportData* pStockData;
	 int nFirst = 0;
	 CString s2 = sName[nFirst];

	 int nAddEquationKind = 0;
	 //CSharesInformation::GetStockKind(int MarketType,char *strLabel)
	 char *strLabel = s2.GetBuffer (0);
	 int stkKind = CSharesInformation::GetStockKind(SH_MARKET_EX,strLabel);
	 bool bFind = false;
	 if(stkKind>=0)
		 if(m_pDoc->m_sharesInformation .Lookup (strLabel,pStockData,stkKind))
			 bFind = true;
		 ;
	 if(bFind == false)
	 {
		 stkKind = CSharesInformation::GetStockKind(SZ_MARKET_EX,strLabel);
		 if(stkKind>=0)
			 if(m_pDoc->m_sharesInformation .Lookup (strLabel,pStockData,stkKind))
				 bFind = true;
	 }
	 if(bFind == true)
	 {
		 nFirst++;
		 if(sName.GetSize ()<=nFirst)
			return 2;

		 if(sName.GetSize()==2)
		 {
			 bool bFound = true;
			 if(sName[1]=="OPEN")	equationKind = 0;
			 else if(sName[1]=="HIGH")	equationKind = 1;
			 else if(sName[1]=="LOW")	equationKind = 2;
			 else if(sName[1]=="CLOSE")	equationKind = 3;
			 else if(sName[1]=="VOL")	equationKind = 4;
			 else if(sName[1]=="AMOUNT")	equationKind = 5;
			 else if(sName[1]=="ADVANCE")	equationKind = 6;
			 else if(sName[1]=="DECLINE")	equationKind = 7;
			 else if(sName[1]=="BUYVOL")	equationKind = 8;
			 else if(sName[1]=="SELLVOL")	equationKind = 9;
			 else bFound = false;

			 if(bFound == true)
			 {
				 equationKind = equationKind+10;
				 tu =0;
				 return 0;
			 }
			 else
				 nAddEquationKind = 100;

		 }
		 else if(sName.GetSize()>2)
			 nAddEquationKind = 100;
		 else
			 return 1;//error
	 }
		s2.ReleaseBuffer();
	 //
	CFormularContent*	pIndex;
	 equationKind = 0;
	 if(sName[nFirst] == "STOCKFILTER")
	 {
		 equationKind = 1;
		 nFirst ++;
		 if(sName.GetSize ()<=nFirst)
			return 1;
	     pIndex=(CFormularContent*)(m_pDoc->LookUpArray (m_pDoc->m_formuar_choose,sName[nFirst])); 
	 }
	 else if(sName[nFirst] == "STOCKMODEKLINE")
	 {
		 equationKind = 2;
		 nFirst ++;
		 if(sName.GetSize ()<=nFirst)
			return 1;
	     pIndex=(CFormularContent*)(m_pDoc->LookUpArray (m_pDoc->m_formuar_kline,sName[nFirst])); 
	 }
	 else if(sName[nFirst] == "STOCKSYSTEM")
	 {
		 equationKind = 3;
		 nFirst ++;
		 if(sName.GetSize ()<=nFirst)
			return 1;
	     pIndex=(CFormularContent*)(m_pDoc->LookUpArray (m_pDoc->m_formuar_master,sName[nFirst])); 
	 }
	 else 
	     pIndex=(CFormularContent*)(m_pDoc->LookUpArray (m_pDoc->m_formuar_index,sName[nFirst])); 


	 zb = sName[nFirst];
	 equationKind += nAddEquationKind;

	int rtn=0;
	if(pIndex==NULL)
		return 1;


	nPeriods = 0;
	tu=pIndex->numPara ;
	cline = "";
	 nFirst ++;

	CString fml=pIndex->fomular;
	fml.MakeLower();
	CStringArray sArray;
	//add lmb 20031103
	int k;

	Kline kline;
	CFormularComputeParent equation(0,0,&kline,fml);
	for(k=0;k<pIndex->numPara ;k++)
	{
		equation.AddPara(pIndex->namePara[k] ,pIndex->defaultVal[k]);
	}
	equation.Devide();
	if(equation.m_bTotalData != 0) m_bTotalData = equation.m_bTotalData;

	int nRtnEqt = equation.GetRlineNum ();
//	rtn = 2;
	for(k=0;k<nRtnEqt;k++)
	{
		sArray.Add (equation.GetRlineName (k));
	}

	 if(sName.GetSize ()<=nFirst)
	 {
		 //TO USE DEFAULT LINE
//		GetLineNameArray(fml,  sArray);
		for(int i = 0;i<sArray.GetSize ();i++)
		{
			if(sArray[i]==zb)
			{
				cline=zb;
				return 0;
			}
		}
		if(sArray.GetSize ())
		{
			{
				cline=sArray[0];
				return 0;
			}
		}
		return 2;
	 }

	 cline = sName[nFirst];
	 if(sName.GetSize () == nFirst+1)
	 {
		 if(nPeriods = GetPeriodKind(sName[nFirst]))
		 {
//			 cline = "";
			for(int i = 0;i<sArray.GetSize ();i++)
			{
				if(sArray[i]==zb)
				{
					cline=zb;
					return 0;
				}
			}
			if(sArray.GetSize ())
			{
				{
					cline=sArray[0];
					return 0;
				}
			}
			 return 2;
		 }
	 }
	 else if(sName.GetSize () == nFirst+2)
	 {
		 nPeriods = GetPeriodKind(sName[nFirst+1]);
		 if(nPeriods==0)
			 return 2;
	 }
	 else
	 {
		 ASSERT(FALSE);
		 return 1;
	 }


	//--		找线
	cline.MakeLower();
	rtn = 2;
	for(k=0;k<sArray.GetSize ();k++)
	{
		if(cline == sArray[k])
		{
			rtn = 0;
			break;
		}
	}

//	zb=fml;
/*	int ind=fml.Find (cline,0);
	if(ind==-1)
	{
		return 2;
	}
	else
	{
		char ch;
		if(ind>0)
		{
			ch=pIndex->fomular.GetAt (ind-1);
			if(GetCharClass(ch)!=2 &&  GetCharClass(ch)!=7)
				rtn=2;
		}
		if(pIndex->fomular.GetLength()>ind+cline.GetLength ())
		{
			ch=pIndex->fomular.GetAt (ind+cline.GetLength ());
			if(GetCharClass(ch)!=2&&(ch)!=':')
				rtn=2;
		}
	}*/
	return rtn;

}

int CFormularComputeParent::GetNum(int tj)
{
/*	CString  ename;//指标名字
	CString  linename;//线条名称

	CString  sl;
	sl=m_QuoteArray[tj-QUB];
	int tu;
	int n10,n20;
	int nRtn = GetQuote(sl,ename,linename,tu,n10,n20);
// int CFormularComputeParent::GetQuote(CString &str, CString &zb, CString &cline, int &tu)
//str引用的总字符串，zb指标名字，cline线的名字，tu参数个数
//返回值：0为正确，1为没有找到指标，2为没有找到线*/
	int tu;
	if(!LookupParameterTable(tj,tu))
	{
		AfxMessageBox("引用的公式的参数个数没有找到");
		return 0;
	}
	return tu;
}

int CFormularComputeParent::GetRlineNum()
{
	return m_RlineNum;

}

CString CFormularComputeParent::GetRlineName(int foot)
{
	return m_RlineName[foot];
}
int GetInt(CString s)
{
	if(s.GetLength ()<2)
		return 0;
	int n = 0;
	int nRate[2] = {16,1};
	for(int i = 0;i<2;i++)
	{
		int ntmp = 0;
		if(s[i]>='0' && s[i]<='9')
			ntmp = s[i]-'0';
		else
			ntmp = 10+(s[i]-'a');
		n+=(ntmp*nRate[i]);
	}
	return n;
}
int CFormularComputeParent::FindLineType()//计算返回线的类型
{
	if( m_SentNum<0)
		return 0;

	int flag=0;

	int nRtn = 0;

//	m_bRightbox = false;

	int i;
	m_tempWhich++;
	for(i=0;i<=m_SentNum;i++)
	{
		if(m_Sent[i]==1002)
		{
			m_tempWhich--;
			flag=2;
			break;
		}
		if(m_Sent[i]==1001)
		{
			flag=1;
			break;
		}

		if(m_Sent[i]>=FuncDllB+50 && m_Sent[i]<= FuncDllB+52)
		{
			m_bRightbox = true;
		}
	}
	if(flag==0 )//	of no name
	{

		m_SentNum=m_SentNum+2;
		for(i=m_SentNum-2;i>=0;i--)
		{
			m_Sent[i+2]=m_Sent[i];
		}
		CString temp;

		temp.Format("%d",(int)m_tempWhich);
		temp="l"+temp;
		m_WordTableNew[temp]=(void*)(RLINEB+m_RlineNum);
		m_Rline[m_RlineNum].kind = ARRAY_BE::Normal;
		m_Rline[m_RlineNum].line = new float[m_MaxPoint];
//		m_table.push_b(RLINEB+m_RlineNum);
		m_RlineName[m_RlineNum] = temp;

		m_Sent[0]=RLINEB+m_RlineNum;
		m_Sent[1]=1001;
		m_Sent[m_SentNum+1]=0;

		m_RlineNum++;

	}

	if(flag == 2) 
		return 0;

	m_RlineType[m_tempWhich] = 0;
	for(i=0;i<=m_SentNum;i++)
	{
//		if(m_Sent[i]==1001)
//		{
//			m_tempWhich++;
//			continue;
//		}
		if(m_Sent[i]>=CONSB&&m_Sent[i]<=CONSE && i>0)
		{
			if(m_Sent[i-1]!=9005 )
			{
				for(int k = m_SentNum;k>=i;k--)
					m_Sent[k+1] = m_Sent[k];
				m_Sent[i] = 9005;
				m_SentNum++;
				i--;
				continue;
			}
		}

		if(m_Sent[i+1]>=CONSB&&m_Sent[i+1]<=CONSE)
		{
			if(m_Sent[i]!=9005 && i!=m_SentNum)
			{
				for(int k = m_SentNum;k>i;k--)
					m_Sent[k+1] = m_Sent[k];
				m_Sent[i+1] = 9005;
				m_SentNum++;
				continue;
			}

			if(m_Sent[i+1]>=CONSCOLORB1 && m_Sent[i+1]<CONSLINETHICKB)
			{
				if(m_Sent[i+1]>=CONSCOLORB)
				{
					CString s = m_strColor[m_Sent[i+1]-CONSCOLORB];
					m_RlineColor[m_tempWhich] = RGB(GetInt(s.Right(2)),GetInt(s.Mid(2,2)),GetInt(s.Left (2)));
				}
				else
				{
					switch(m_Sent[i+1])
					{
					case 9100:
						m_RlineColor[m_tempWhich] = RGB(255,0,0);
						break;
					case 9101:
						m_RlineColor[m_tempWhich] = RGB(0,255,0);
						break;
					case 9102:
						m_RlineColor[m_tempWhich] = RGB(0,0,255);
						break;
					case 9103:
						m_RlineColor[m_tempWhich] = RGB(255,255,0);
						break;
					case 9104:
						m_RlineColor[m_tempWhich] = RGB(255,100,0);
						break;
					case 9105:
						m_RlineColor[m_tempWhich] = RGB(255,0,255);
						break;
					case 9106:
						m_RlineColor[m_tempWhich] = RGB(0,0,0);
						break;
					case 9107:
						m_RlineColor[m_tempWhich] = RGB(255,255,255);
						break;
					case 9108:
						m_RlineColor[m_tempWhich] = RGB(255,0,255);
						break;
					case 9109:
						m_RlineColor[m_tempWhich] = RGB(0,255,255);
						break;
//	m_WordTable[_T("colormagenta")]  =(void *)9108;//pink
//	m_WordTable[_T("colorcyan")]  =(void *)9109;
					}
				}
/*	m_WordTable[_T("colorred")]  =(void *)9100;
	m_WordTable[_T("colorgreen")]  =(void *)9101;
	m_WordTable[_T("colorblue")]  =(void *)9102;
	m_WordTable[_T("coloryellow")]  =(void *)9103;
	m_WordTable[_T("colororange")]  =(void *)9104;
	m_WordTable[_T("colorpurple")]  =(void *)9105;
	m_WordTable[_T("colorblack")]  =(void *)9106;
	m_WordTable[_T("colorwhite")]  =(void *)9107;
*/			}
			else if(m_Sent[i+1]>=CONSLINETHICKB)
			{
				if(m_Sent[i+1]!= CONSLINEDOT)
					m_RlineThick[m_tempWhich] = m_Sent[i+1]-CONSLINETHICKB+1;
				else
					m_RlineThick[m_tempWhich]+=100;
			}
			else
			{
					m_RlineType[m_tempWhich]=m_Sent[i+1]-CONSB+1;
					if(m_RlineType[m_tempWhich] == 7 || m_RlineType[m_tempWhich] == 8)
						m_RlineType[m_tempWhich] += 100;

			}

			for(int j=i;j<m_SentNum-1;j++)
			{
				m_Sent[j]=m_Sent[j+2];

			}
			m_SentNum-=2;
			m_Sent[m_SentNum+1]=0;

			i--;
	

		}
	}
	return nRtn;
}

void CFormularComputeParent::IsDaPan()
{
//925,937
	for(int i=0;i<m_SentNum;i++)
	{
		if(m_Sent[i]==9525||m_Sent[i]==9537)
		{
			m_isDaPan=1;
		}
	}

}

void CFormularComputeParent::InitEquation(int pmin, int pmax, Kline *pp, CString &str)
{
	m_bRightbox = false;
	m_nCurrentFoot = 0;
	m_nSizeKindPos = 0;
	m_nPosPre = 0;
	m_pKindPos = NULL;
	m_pKlineEx = NULL;

	m_strErrMsg = _T(" ");
	m_bTotalData=false;
	m_isDaPan=0;
	for(int k = 0;k<NUM_LINE_TOT;k++)
	{
		m_RlineColor[k] = -1;
		m_RlineThick[k] = 1;
		m_RlineType[k]=0;
	}

	m_pData      = pp;
	m_QuoteNum   =0;
	m_floatNum = 0;
	m_intNum   = 0;
	m_VarNum   = 0;
	m_RlineNum   = 0;
	m_MidNum   = 0;
	m_NumGs    = 0;
	m_stringNum = 0;
	m_strColorNum = 0;

	//eliminate the explain
	int b = 0;
	while(true)
	{
		int e = str.Find ("{",b);
		if(e<0)
			break;
		b = e;
		e = str.Find ("}",b);
		if(e<0)
			break;

		//replace "[]" to " "
		for(int i=b;i<=e;i++)
		{
			str.SetAt (i,' ');
		}
		b = e;
	}
	//end
	m_formuCompute   = str;
	m_nExec     =0;

	m_MaxPoint = pmax+1;
	m_MinPoint = pmin;
	
	//first init
	InitStatic();

	m_symbol = _T("");
	m_pBuySellList = NULL; 
	m_pDoc =((CMainFrame*)AfxGetMainWnd())->m_pDoc ;
	m_pKlineIndex = NULL;
	m_pReportData = NULL;
	m_nKindKline = -1;
	m_bCreateBsList = false;
	m_pIndex = NULL;

}
//////////////////////////////////////////////////////////////////////

int CFormularComputeParent::Func8048to8051(int nKind)
{
	int cl,ret;
	ARRAY_BE fcl;
	
	fcl.line = new float[m_MaxPoint];
	
	cl=m_table.pop_it();
	GetData(cl,fcl);
	
	ret = fcl.b;
	ret =(int)fcl.line[ret];
	if(ret<1)
		ret = 1;
	if(ret>3)
		ret =3;
	
	m_MidArray[m_MidNum].kind = ARRAY_BE::Normal;
	m_MidArray[m_MidNum].line = new float[m_MaxPoint];
	m_MidArray[m_MidNum].b =fcl.b; 
	m_MidArray[m_MidNum].e =fcl.e;
	
	if(m_nKindKline!=HS_KLINE)
	{
		int nIn ;
		if(nKind-3<2)
			nIn = 9+(ret-1)*2+nKind-3;
		else
			nIn = 15+(ret-1)*2+nKind-5;
		TransferRealInfo(m_MidArray[m_MidNum].b,m_MidArray[m_MidNum].e, nIn);
	}
	else
	{
		ASSERT(this->m_pBuySellList!=NULL);
		ASSERT(this->m_pBuySellList->GetCount() == m_MaxPoint);
		if(m_MaxPoint!=m_pBuySellList->GetCount ())
		{
			m_table.push_b(PCMB+m_MidNum);
			m_MidNum++;
			delete[] fcl.line;
			return 999;
		}

		{
			if(m_pKlineEx == NULL) CTaiKlineFileHS::TransferDataEx (m_pBuySellList,m_pKlineEx);
			if(m_pKlineEx != NULL) CTaiKlineFileHS::TransferHs(this->m_pBuySellList , m_MidArray[m_MidNum], m_MaxPoint, nKind,ret,m_pKlineEx);
		}
	}
	int bl = m_MidArray[m_MidNum].b;
	int be = m_MidArray[m_MidNum].e;
	if(bl>be||be>m_MaxPoint-1||bl<0)
	{
		m_table.push_b(PCMB+m_MidNum);
		m_MidNum++;
		delete[] fcl.line;
		return 999;
	}

	m_table.push_b(PCMB+m_MidNum);
	m_MidNum++;
	delete[] fcl.line;
	return 0;

}

//sumbars

int CFormularComputeParent::Func8047()
{
	int cl,cr,j;
	ARRAY_BE fcl,fcr;
	
	fcl.line = new float[m_MaxPoint];
	fcr.line = new float[m_MaxPoint];
	
	cl=m_table.pop_it();
	GetData(cl,fcl);
	cr=m_table.pop_it();
	GetData(cr,fcr);
	
	m_MidArray[m_MidNum].kind = ARRAY_BE::Normal;
	m_MidArray[m_MidNum].line = new float[m_MaxPoint];
	m_MidArray[m_MidNum].b =fcl.b; 
	m_MidArray[m_MidNum].e =fcl.e;
	
	int bl = m_MidArray[m_MidNum].b;
	int be = m_MidArray[m_MidNum].e;
	float fVal = fcr.line[bl];

	if(be>=bl&&bl>=0&&be>=0)
	{
		m_MidArray[m_MidNum].line[bl] = 0;
		for(j=bl;j<=be;j++)
		{
			float f=0;
			int i =0;
			for( i = j;i>=bl;i--)
			{
				f+=fcl.line[i];
				if(f>=fVal)
				{
					i--;
					break;
				}

			}
			m_MidArray[m_MidNum].line[j] =float( j-i-1 );
		}
	}
	
	m_table.push_b(PCMB+m_MidNum);
	m_MidNum++;
	delete[] fcl.line;
	delete[] fcr.line;
	return 0;
}
//finance
int CFormularComputeParent::Func8052()
{
	if(this->m_pReportData==NULL)
		m_pDoc->m_sharesInformation .Lookup (this->m_symbol.GetBuffer(0),this->m_pReportData,m_stkKind);
	m_symbol.ReleaseBuffer();
	ASSERT(this->m_pReportData!=NULL);
	if(m_pReportData==NULL)
		return 852;

	ASSERT(this->m_symbol.GetLength()>=4);

	int cl,ret,j;
	ARRAY_BE fcl;

	fcl.line = new float[m_MaxPoint];
	
	cl=m_table.pop_it();
	GetData(cl,fcl);
	ret = int( fcl.line[fcl.b ] );
	if(ret<1)
		ret = 1;
	if(ret>100)
		ret = 1;

	m_MidArray[m_MidNum].kind = ARRAY_BE::Normal;
	m_MidArray[m_MidNum].line = new float[m_MaxPoint];
	m_MidArray[m_MidNum].b =fcl.b; 
	m_MidArray[m_MidNum].e =fcl.e;
	
	int bl = m_MidArray[m_MidNum].b;
	int be = m_MidArray[m_MidNum].e;

	m_MidArray[m_MidNum].line[bl] = 0;
	BASEINFO* pBase = this->m_pReportData->pBaseInfo;
	if(pBase==NULL)
	{
		m_table.push_b(PCMB+m_MidNum);
		m_MidNum++;
		delete[] fcl.line;
		return 999;
	}

//	float* pf = &pBase->zgb;
	float fVal[50] = {pBase->zgb ,pBase->gjg ,pBase->fqrfrg ,pBase->frg ,pBase->Bg,
	                  pBase->Hg  ,pBase->ltAg,pBase->zgg    ,pBase->zpg ,pBase->zzc*10 ,
					pBase->ldzc*10  ,pBase->gdzc*10,pBase->wxzc*10   ,pBase->cqtz*10,pBase->ldfz*10 ,
					pBase->cqfz*10  ,pBase->zbgjj*10 ,pBase->mggjj*10,pBase->ggqy*10 ,pBase->zyywsr*10,
					pBase->zyywlr*10,pBase->qtywlr*10,pBase->zyywlr*10,0          ,0,
					0            ,0            ,pBase->lrze*10 ,pBase->jlr*10 ,pBase->jlr*10,
					pBase->wfplr*10 ,pBase->mgwfplr ,pBase->mgsy ,pBase->mgjzc ,0,
					pBase->gdqybl ,pBase->jzcsyl };
	for(j=bl;j<=be;j++)	 m_MidArray[m_MidNum].line[j] = fVal[ret-1];     //all
//	for(j=bl;j<=be;j++)	 m_MidArray[m_MidNum].line[j] = pf[ret-1];  

	m_table.push_b(PCMB+m_MidNum);
	m_MidNum++;
	delete[] fcl.line;

	return 0;
}
//inforeal
int CFormularComputeParent::Func8053()
{
	if(this->m_pReportData==NULL)
	{
		m_pDoc->m_sharesInformation .Lookup (this->m_symbol.GetBuffer(0),this->m_pReportData,m_stkKind);
		m_symbol.ReleaseBuffer();
	}
	ASSERT(this->m_pReportData!=NULL);
	if(m_pReportData==NULL)
		return 853;

	ASSERT(this->m_symbol.GetLength()>=4);

	int cl,ret;
	ARRAY_BE fcl;

	fcl.line = new float[m_MaxPoint];
	
	cl=m_table.pop_it();
	GetData(cl,fcl);
	ret = int( fcl.line[fcl.b ] );
	if(ret<1)
		ret = 1;
	if(ret>100)
		ret = 1;

	m_MidArray[m_MidNum].kind = ARRAY_BE::Normal;
	m_MidArray[m_MidNum].line = new float[m_MaxPoint];
	m_MidArray[m_MidNum].b =fcl.b; 
	m_MidArray[m_MidNum].e =fcl.e;
	
	int bl = m_MidArray[m_MidNum].b;
	int be = m_MidArray[m_MidNum].e;

	m_MidArray[m_MidNum].line[bl] = 0;
	TransferRealInfo( bl,  be,  ret);

	m_table.push_b(PCMB+m_MidNum);
	m_MidNum++;
	delete[] fcl.line;

	return 0;
}

Kline* CFormularComputeParent::GetKlineIndex()
{
	//
	if(m_pKlineIndex!=NULL)
		return m_pKlineIndex;
	if(m_nKindKline==-1)
	{
		ASSERT(FALSE);
		return NULL;
	}

	bool bSh = true;//CSharesCompute::GetIndexSymbol(int nKind)
	CString s = CSharesCompute::GetIndexSymbol(0);
	int stkKind = SHZS;
	if(CSharesCompute::GetMarketKind(m_stkKind) == SZ_MARKET_EX)
	{
		bSh = false;
		s = CSharesCompute::GetIndexSymbol(1);
		stkKind = SZZS;
	}
	int n = 0;
	Kline* pKline = NULL;
	n=ReadKline(s,stkKind,pKline);

	m_pKlineIndex = new Kline[this->m_MaxPoint];
	memset(m_pKlineIndex,0,sizeof(Kline)*(this->m_MaxPoint));

	//day kline
	int nRate =1;
	if(m_nKindKline>4&&m_nKindKline<9)
		nRate = 24*60*60;
	else
		nRate = 24*60;
	int j =0;
	for(int i=m_MinPoint;i<m_MaxPoint && j<n;)
	{
		if(m_pData[i].day/nRate == pKline[j].day/nRate)
		{
			memcpy(m_pKlineIndex+i,pKline+j,sizeof(Kline));// = 
			j++;
			i++;
		}
		else if(m_pData[i].day/nRate > pKline[j].day/nRate)
		{
			j++;
		}
		else
		{
			memcpy(m_pKlineIndex+i,pKline+j,sizeof(Kline));// = 
			m_pKlineIndex[i].day = m_pData[i].day;// = 
			i++;
		}
	}
	delete[] pKline;

	return m_pKlineIndex;
}


bool CFormularComputeParent::GetBuySellList()
{
	if(m_pBuySellList!=NULL)
		return true;
	//read from file
	m_pBuySellList = new CBuySellList;
	CTaiKlineFileHS*	pFileHs;
	pFileHs=CTaiKlineFileHS::GetFilePointer (m_symbol,m_stkKind);//m_pFileHsSh ;
	pFileHs->ReadHS (m_symbol,*m_pBuySellList,false);//CHANGE ,it is a day
	if(m_bCreateBsList==false)
		m_bCreateBsList = true;
	else
	{
		ASSERT(FALSE);
		return false;
	}

	return true;
}

int CFormularComputeParent::ReadKline(CString s,int stkKind, Kline *&pKline)
{
	int n = 0;
	if(m_nKindKline<0)
		return 0;

//	Kline* pKline = NULL;
	//hs file
	if(m_nKindKline==HS_KLINE||m_nKindKline==MIN1_KLINE
		||(m_nKindKline==MANY_DAY_KLINE&&m_pDoc->m_propertyInitiate.daysOfManyKline<0))
	{
		//todays
		//trans
		CTaiKlineTransferKline trans;
		trans.TransferKlineHistory(s,stkKind,pKline, n,m_nKindKline,true);//used to transfer k line data to another kind of k line

		ASSERT(n>=0&&pKline!=NULL);
	}
	else
	{

		bool bDay = true;
		if(m_nKindKline>0&&m_nKindKline<5)
			bDay = false;
		CTaiKlineFileKLine* pFile = CTaiKlineFileKLine::GetFilePointer(s,stkKind,bDay) ;
		//read now
		n = pFile->ReadKLine(s,pKline,-1,48)	;//读取K线数据

		//TRANSER NOW
		CTaiKlineTransferKline trans;
		trans.TransferKlineHistory(s,stkKind,pKline, n,m_nKindKline,true);//used to transfer k line data to another kind of k line
	}
	return n;


}

void CFormularComputeParent::KlineToLine(Kline *pKline, float & f,int nFlag)
{
	switch(nFlag)
	{
	case 0:
		f = pKline->open;
		break;
	case 1:
		f = pKline->high;
		break;
	case 2:
		f = pKline->low;
		break;
	case 3:
		f = pKline->close;
		break;
	case 4:
		f = pKline->vol;
		break;
	case 5:
		f = pKline->amount;
		break;
	case 6:
		f = pKline->advance;
		break;
	case 7:
		f = pKline->decline;
		break;
	case 8:
		f=  *((float*)&(pKline->advance));
		break;
	case 9:
		f =float(  FABSMY(FABSMY(pKline->vol) - *((float*)&(pKline->decline)))    );
		break;
	}

}

int CFormularComputeParent::GetPeriodKind(CString sIn)
{
	sIn.MakeLower ();
	if(sIn == "hsdata")
		return HS_KLINE;
	if(sIn == "min1")
		return MIN1_KLINE;
	if(sIn == "min5")
		return 1;
	if(sIn == "min15")
		return 2;
	if(sIn == "min30")
		return 3;
	if(sIn == "min60")
		return 4;
	if(sIn == "day")
		return 5;
	if(sIn == "week")
		return 6;
	if(sIn == "month")
		return 7;
	if(sIn == "year")
		return 8;
	return 0;
}

void CFormularComputeParent::GetLineNameArray(CString fml, CStringArray &sArray)
{
//	try
	{
		Kline kline[1000];
		CFormularComputeParent equ(0,999,kline,fml);
		equ.FormularCompute () ;
		for(int i=0;i<equ.GetRlineNum ();i++)
		{
			CString s = equ.GetRlineName (i);
			s.MakeUpper ();
			sArray.Add (s);
		}
	}
//	catch(...)
	{
//		ASSERT(FALSE);
	}

}


void CFormularComputeParent::TransferRealInfo(int bl, int be, int nKind)
{
	int j;
	float * fp = &(m_pReportData->ystc);
	float f = CTaiShanKlineShowView::GetCapital(this->m_pReportData );
	BASEINFO* pBase = this->m_pReportData->pBaseInfo;
	float f2 = 0;

	if(pBase)
		f2 = (pBase->mgsy!=0?m_pReportData->ystc / pBase->mgsy:0);

	float fVal[50] = {m_pReportData->ystc ,m_pReportData->opnp ,m_pReportData->higp ,
		m_pReportData->lowp ,m_pReportData->nowp,m_pReportData->totv ,m_pReportData->nowv ,m_pReportData->totp,
		(m_pReportData->totv>0? (m_pReportData->totp /m_pReportData->totv *100):0),(m_pReportData->nowp -m_pReportData->ystc ),
		(m_pReportData->ystc>0?((m_pReportData->higp -m_pReportData->lowp )/m_pReportData->ystc ):0),
		(m_pReportData->ystc>0?((m_pReportData->nowp -m_pReportData->ystc )/m_pReportData->ystc) :0),
		((m_pReportData->accb+m_pReportData->accs)>0?((m_pReportData->accb-m_pReportData->accs)/(m_pReportData->accb+m_pReportData->accs)*100.0):0),
		(m_pReportData->accb-m_pReportData->accs),
		(((CMainFrame::m_pDoc ->m_nANT[0] * (m_pReportData->volume5 ))!=0)?(m_pReportData->totv / (CMainFrame::m_pDoc ->m_nANT[0]) * (m_pReportData->volume5 )):0),
		m_pReportData->accb,m_pReportData->accs,m_pReportData->pbuy1 ,
		m_pReportData->psel1 ,m_pReportData->dvol ,m_pReportData->rvol,0,m_pReportData->vbuy1 ,
		m_pReportData->vbuy2 ,m_pReportData->vbuy3,m_pReportData->pbuy1,m_pReportData->pbuy2,m_pReportData->pbuy3,
		m_pReportData->vsel1,m_pReportData->vsel2,m_pReportData->vsel3,m_pReportData->psel1,m_pReportData->psel2,
		m_pReportData->psel3,(f>0?m_pReportData->totv /f:0) , m_pReportData->volume5 ,f2};

	//
	for(j=bl;j<=be;j++)	 
		m_MidArray[m_MidNum].line[j] =fVal[nKind-3];// this->m_pReportData->ystc;
/*	switch(nKind)
	{
	case 1:
		for(j=bl;j<=be;j++)	 m_MidArray[m_MidNum].line[j] = this->m_pReportData->ystc;
		break;
	case 2:
		for(j=bl;j<=be;j++)	 m_MidArray[m_MidNum].line[j] = this->m_pReportData->opnp;
		break;
	case 3:
		for(j=bl;j<=be;j++)	 m_MidArray[m_MidNum].line[j] = this->m_pReportData->higp;
		break;
	case 4:
		for(j=bl;j<=be;j++)	 m_MidArray[m_MidNum].line[j] = this->m_pReportData->lowp;
		break;
	case 5:
		for(j=bl;j<=be;j++)	 m_MidArray[m_MidNum].line[j] = this->m_pReportData->nowp;
		break;
	case 6:
		for(j=bl;j<=be;j++)	 m_MidArray[m_MidNum].line[j] = this->m_pReportData->nowv;
		break;
	case 7:
		for(j=bl;j<=be;j++)	 m_MidArray[m_MidNum].line[j] = this->m_pReportData->totv;
		break;
	case 8:
		for(j=bl;j<=be;j++)	 m_MidArray[m_MidNum].line[j] = this->m_pReportData->totp;
		break;
	case 9:
		for(j=bl;j<=be;j++)	 m_MidArray[m_MidNum].line[j] = this->m_pReportData->pbuy1;             //      买入价一
		break;
	case 10:
		for(j=bl;j<=be;j++)	 m_MidArray[m_MidNum].line[j] = this->m_pReportData->vbuy1;             //      买入量一
		break;
	case 11:
		for(j=bl;j<=be;j++)	 m_MidArray[m_MidNum].line[j] = this->m_pReportData->pbuy2;             //      买入价二  
		break;
	case 12:
		for(j=bl;j<=be;j++)	 m_MidArray[m_MidNum].line[j] = this->m_pReportData->vbuy2;             //      买入量二
		break;
	case 13:
		for(j=bl;j<=be;j++)	 m_MidArray[m_MidNum].line[j] = this->m_pReportData->pbuy3;             //      买入价三 
		break;
	case 14:
		for(j=bl;j<=be;j++)	 m_MidArray[m_MidNum].line[j] = this->m_pReportData->vbuy3;             //      买入量三      
		break;
	case 15:
		for(j=bl;j<=be;j++)	 m_MidArray[m_MidNum].line[j] = this->m_pReportData->psel1;             //      卖出价一
		break;
	case 16:
		for(j=bl;j<=be;j++)	 m_MidArray[m_MidNum].line[j] = this->m_pReportData->vsel1;             //      卖出量一
		break;
	case 17:
		for(j=bl;j<=be;j++)	 m_MidArray[m_MidNum].line[j] = this->m_pReportData->psel2;             //      卖出价二 
		break;
	case 18:
		for(j=bl;j<=be;j++)	 m_MidArray[m_MidNum].line[j] = this->m_pReportData->vsel2;             //      卖出量二   
		break;
	case 19:
		for(j=bl;j<=be;j++)	 m_MidArray[m_MidNum].line[j] = this->m_pReportData->psel3;             //      卖出价三
		break;
	case 20:
		for(j=bl;j<=be;j++)	 m_MidArray[m_MidNum].line[j] = this->m_pReportData->vsel3;             //      卖出量三 
		break;
	case 21:
		for(j=bl;j<=be;j++)	 m_MidArray[m_MidNum].line[j] = this->m_pReportData->accb;              //      买入挂单和（三种买入挂单和）
		break;
	case 22:
		for(j=bl;j<=be;j++)	 m_MidArray[m_MidNum].line[j] = this->m_pReportData->accs;              //      卖出挂单和（三种卖出挂单和）
		break;
	case 23:
		for(j=bl;j<=be;j++)	 m_MidArray[m_MidNum].line[j] = this->m_pReportData->volume5;           //      5天均量
		break;
	case 24:
		for(j=bl;j<=be;j++)	 m_MidArray[m_MidNum].line[j] = this->m_pReportData->rvol;              //      外盘
		break;
	case 25:
		for(j=bl;j<=be;j++)	 m_MidArray[m_MidNum].line[j] = this->m_pReportData->dvol;              //      内盘
		break;
//	case 1:
//		for(j=bl;j<=be;j++)	 m_MidArray[m_MidNum].line[j] = this->m_pReportData->
//		break;
	}
*/
}

void CFormularComputeParent::GetHsKlineData(int cl,ARRAY_BE &pp)
{
	if(m_nKindKline == HS_KLINE)
	{
		//TRANSER NOW
		CTaiKlineTransferKline trans(this->m_pBuySellList );
		int n = this->m_pBuySellList ->GetCount();
		Kline* pKline = new Kline[n];
		trans.TransferKlineHistory(m_symbol,m_stkKind,pKline, n,HS_KLINE,false);//used to transfer k line data to another kind of k line
		for(int i=0;i<n;i++)
		   pp.line[i] = pKline[i].open;
		delete[] pKline;
	}
}

bool CFormularComputeParent::LookupWordTable(CString sKey, int &n)
{
	if(m_WordTable.Lookup(sKey,(void*&)n))	
		return true;
	else if(m_WordTableNew.Lookup(sKey,(void*&)n))	
		return true;

	return false;
}

bool CFormularComputeParent::LookupParameterTable(int nKey, int &nValue)
{
	if(m_ParameterTable.Lookup(nKey,nValue))	return true;
	else if(m_ParameterTableNew.Lookup(nKey,nValue))	return true;
	return false;

}

void CFormularComputeParent::InitStatic()
{
	static bool bExec = false;
	if(bExec==true) return;
	m_WordTable.InitHashTable( 300 );
	m_LetterTable.InitHashTable( 150 );
	m_ParameterTable.InitHashTable( 100 );

	//运算符 
	m_WordTable[_T("*")]  =(void *)6001;
	m_WordTable[_T("/")]  =(void *)6002;
	m_WordTable[_T("*-")]  =(void *)6003;
	m_WordTable[_T("/-")]  =(void *)6004;
	m_WordTable[_T("+")]  =(void *)5001;
	m_WordTable[_T("-")]  =(void *)5002;
	m_WordTable[_T(">")]  =(void *)4001;
	m_WordTable[_T("<")]  =(void *)4002;
	m_WordTable[_T(">=")] =(void *)4005;
	m_WordTable[_T("<=")] =(void *)4006;
	m_WordTable[_T("=")] =(void *)4008;
	m_WordTable[_T("and")] =(void *)3001;
	m_WordTable[_T("or")] =(void *)2001;
	m_WordTable[_T("(")]  =(void *)9001;
	m_WordTable[_T(")")]  =(void *)9002;
	m_WordTable[_T(",")]  =(void *)9005;

	m_WordTable[_T(":")]  =(void *)1001;
	m_WordTable[_T(":=")] =(void *)1002;

	m_WordTable[_T("!=")] =(void *)4009;
	m_WordTable[_T("><")] =(void *)4009;
	m_WordTable[_T("<>")] =(void *)4009;
	//常数
	m_WordTable[_T("volstick")]    =(void *)9010;
	m_WordTable[_T("stick")]	   =(void *)9011;
	m_WordTable[_T("circledot")]   =(void *)9012;
	m_WordTable[_T("pointdot")]    =(void *)9012;
	m_WordTable[_T("linestick")]   =(void *)9013;
	m_WordTable[_T("crossdot")]    =(void *)9014;
	m_WordTable[_T("colorstick")]  =(void *)9015;
	m_WordTable[_T("buycase")]     =(void *)9016;
	m_WordTable[_T("sellcase")]    =(void *)9017;

	m_WordTable[_T("colorred")]		=(void *)9100;
	m_WordTable[_T("colorgreen")]	=(void *)9101;
	m_WordTable[_T("colorblue")]	=(void *)9102;
	m_WordTable[_T("coloryellow")]  =(void *)9103;
	m_WordTable[_T("colororange")]  =(void *)9104;
	m_WordTable[_T("colorpurple")]  =(void *)9105;
	m_WordTable[_T("colorblack")]	=(void *)9106;
	m_WordTable[_T("colorwhite")]	=(void *)9107;
	m_WordTable[_T("colormagenta")] =(void *)9108;//pink
	m_WordTable[_T("colorcyan")]	=(void *)9109;

	m_WordTable[_T("linethick1")]  =(void *)9300;
	m_WordTable[_T("linethick2")]  =(void *)9301;
	m_WordTable[_T("linethick3")]  =(void *)9302;
	m_WordTable[_T("linethick4")]  =(void *)9303;
	m_WordTable[_T("linethick5")]  =(void *)9304;
	m_WordTable[_T("linethick6")]  =(void *)9305;
	m_WordTable[_T("linethick7")]  =(void *)9306;

	m_WordTable[_T("linedot")]  =(void *)CONSLINEDOT;

//	m_WordTable[_T("color")]  =(void *)902;
	//=================参数================
	m_WordTable[_T("open")]		=(void *)9521;
	m_WordTable[_T("close")]	=(void *)9522;
	m_WordTable[_T("high")]		=(void *)9523;
	m_WordTable[_T("low")]      =(void *)9524;
	m_WordTable[_T("o")]		=(void *)9521;
	m_WordTable[_T("c")]		=(void *)9522;
	m_WordTable[_T("h")]		=(void *)9523;
	m_WordTable[_T("l")]		=(void *)9524;
	m_WordTable[_T("advance")] =(void *)9525;
	m_WordTable[_T("weekday")] =(void *)9526;
	m_WordTable[_T("day")]     =(void *)9527;
	m_WordTable[_T("month")]   =(void *)9528;
	m_WordTable[_T("year")]    =(void *)9529;
	m_WordTable[_T("hour")]    =(void *)9530;
	m_WordTable[_T("minute")]  =(void *)9531;
	m_WordTable[_T("amount")]  =(void *)9532;
	m_WordTable[_T("vol")]     =(void *)9533;
	m_WordTable[_T("volume")]  =(void *)9533;
//	m_WordTable[_T("a")]  =(void *)9532;
	m_WordTable[_T("v")]     =(void *)9533;
	m_WordTable[_T("isup")]    =(void *)9534;
	m_WordTable[_T("isdown")]  =(void *)9535;
	m_WordTable[_T("isequal")] =(void *)9536;
	m_WordTable[_T("decline")] =(void *)9537;
	m_WordTable[_T("capital")] =(void *)9538;
//	m_WordTable[_T("ltp")]	   =(void *)9539;
	//new 参数 in vs2.0
	m_WordTable[_T("indexo")]	=(void *)9541;//大盘K线数据
	m_WordTable[_T("indexc")]   =(void *)9542;//大盘K线数据
	m_WordTable[_T("indexh")]	=(void *)9543;//大盘K线数据
	m_WordTable[_T("indexl")]	=(void *)9544;//大盘K线数据
	m_WordTable[_T("indexv")]	=(void *)9545;//大盘K线数据
	m_WordTable[_T("indexa")]	=(void *)9546;//大盘K线数据
	m_WordTable[_T("indexdec")] =(void *)9547;//大盘K线数据
	m_WordTable[_T("indexadv")] =(void *)9548;//大盘K线数据

	m_WordTable[_T("buyvol")]		=(void *)9550;//换手数据的主动性买量
	m_WordTable[_T("sellvol")]		=(void *)9551;//换手数据的主动性卖量
	m_WordTable[_T("isbuyorder")]	=(void *)9552;//是否是买单
	m_WordTable[_T("activebuyvol")]	=(void *)9553;//主动性买量
	m_WordTable[_T("activesellvol")]	=(void *)9554;//主动性卖量
	m_WordTable[_T("time")]  =(void *)9560;//是否
	m_WordTable[_T("date")]  =(void *)9561;//是否

	m_WordTable[_T("volunit")]  =(void *)9580;//每手股数

	//================函数==================
	m_WordTable[_T("max")]     =(void *)8001;
	m_ParameterTable[8001]     =2;
	m_WordTable[_T("min")]     =(void *)8004;
	m_ParameterTable[8004]     =2;
	m_WordTable[_T("mod")]     =(void *)8005;
	m_ParameterTable[8005]     =2;
	m_WordTable[_T("reverse")] =(void *)8006;
	m_ParameterTable[8006]     =1;
	m_WordTable[_T("if")]      =(void *)8007;
	m_ParameterTable[8007]     =3;

	m_WordTable[_T("abs")]     =(void *)8002;
	m_ParameterTable[8002]     =1;
	m_WordTable[_T("ln")]      =(void *)8008;
	m_ParameterTable[8008]     =1;
	m_WordTable[_T("not")]     =(void *)8009;
	m_ParameterTable[8009]     =1;
	m_WordTable[_T("sgn")]     =(void *)8010;
	m_ParameterTable[8010]     =1;

	m_WordTable[_T("exp")]     =(void *)8003;
	m_ParameterTable[8003]     =1;
	m_WordTable[_T("log")]     =(void *)8011;
	m_ParameterTable[8011]     =1;
	m_WordTable[_T("pow")]     =(void *)8012;
	m_ParameterTable[8012]     =2;
	m_WordTable[_T("sqrt")]    =(void *)8013;
	m_ParameterTable[8013]     =1;
	m_WordTable[_T("avedev")]	=(void *)8030;
	m_ParameterTable[8030]		=2;
	m_WordTable[_T("count")]	=(void *)8031;
	m_ParameterTable[8031]		=2;
	m_WordTable[_T("cross")]	=(void *)8032;
	m_ParameterTable[8032]		=2;

	m_WordTable[_T("devsq")]	=(void *)8033;
	m_ParameterTable[8033]		=2;
	m_WordTable[_T("ema")]		=(void *)8034;
	m_ParameterTable[8034]		=2;
	m_WordTable[_T("hhv")]		=(void *)8035;
	m_ParameterTable[8035]		=2;

	m_WordTable[_T("llv")]		=(void *)8036;
	m_ParameterTable[8036]		=2;
	m_WordTable[_T("sar")]		=(void *)8037;
	m_ParameterTable[8037]		=3;
	m_WordTable[_T("sarturn")]	=(void *)8038;
	m_ParameterTable[8038]		=3;

	m_WordTable[_T("ref")]		=(void *)8039;
	m_ParameterTable[8039]		=2;
	m_WordTable[_T("ma")]		=(void *)8040;
	m_ParameterTable[8040]		=2;
	m_WordTable[_T("std")]		=(void *)8041;
	m_ParameterTable[8041]		=2;

	m_WordTable[_T("stdp")]     =(void *)8042;
	m_ParameterTable[8042]		=2;
	m_WordTable[_T("sum")]		=(void *)8043;
	m_ParameterTable[8043]		=2;
	m_WordTable[_T("varp")]     =(void *)8044;
	m_ParameterTable[8044]		=2;

	m_WordTable[_T("var")]		=(void *)8045;
	m_ParameterTable[8045]		=2;
	m_WordTable[_T("sma")]		=(void *)8046;
	m_ParameterTable[8046]		=3;

	//new function in vs2.0
	m_WordTable[_T("sumbars")]	=(void *)8047;//
	m_ParameterTable[8047]		=2;

	m_WordTable[_T("bidprice")]	=(void *)8048;//委买价
	m_ParameterTable[8048]		=1;
	m_WordTable[_T("bidvol")]	=(void *)8049;//委买量
	m_ParameterTable[8049]		=1;
	m_WordTable[_T("askprice")]	=(void *)8050;//委卖价
	m_ParameterTable[8050]		=1;
	m_WordTable[_T("askvol")]	=(void *)8051;//委卖价
	m_ParameterTable[8051]		=1;

	m_WordTable[_T("finance")]	=(void *)8052;//财务数据
	m_ParameterTable[8052]		=1;

	m_WordTable[_T("dynainfo")] =(void *)8053;//实时行情数据
	m_ParameterTable[8053]		=1;
	int nParam[]={  5,5,5,2,1,
					1,1,2,2,2,
					2,3,3,3,3,
					2,2,2,2,3,
					3,1,1,3,3,
					1,1,1,1,1,
					1,2,1,1,3,
					2,2,3,3,2,
					3,3,3,3,1,
					1,3,5,3,1,
					4,2,4,2,5,
					3,3,3,3,1,
					2,1,3,3	};
	CString sName[]={"beeline","radial","wirebar","backset","barscount",//HHVBARS
					"barslast","barssince","dma","hhvbars","llvbars",
					"zig2","peak2","peakbars2","trough2","troughbars2",
					"forcast","slope","upward","downward","slowupcross",
					"slowdowncross","floatpercent","floattype","valueday","selfadd",
					"sin","cos","tan","asin","acos",
					"atan","backset2","towervalue","intpart","floatcommerce",
					"reflast","refbegin","valueaverage","floatcommercetype","zig",
					"peak","peakbars","trough","troughbars","winner",
					"cost",	"drawicon","drawline","drawtext","extdata",
					"deployref","deploysum","deploywin","polyline","stickline",/*fan(vol,low,high,0),fansum(vol,6),fanwin(vol,200,ltp)*/
					"SetLineWidth","polylinevalue","polylinetime","between","ceiling",
					"filter","floor","longcross","range"
					};
	for(int j=0;j<64;j++)
	{
		m_WordTable[sName[j]]		 = (void *)(FuncDllB+j);//直线
		m_ParameterTable[FuncDllB+j] = nParam[j];
	}
	m_WordTable["winnertype"] =(void *)(FuncDllB+14);//直线


	//=================数字================
	m_LetterTable[_T("0")]  =(void *)3;
	m_LetterTable[_T("1")]  =(void *)3;
	m_LetterTable[_T("2")]  =(void *)3;
	m_LetterTable[_T("3")]  =(void *)3;
	m_LetterTable[_T("4")]  =(void *)3;
	m_LetterTable[_T("5")]  =(void *)3;
	m_LetterTable[_T("6")]  =(void *)3;
	m_LetterTable[_T("7")]  =(void *)3;
	m_LetterTable[_T("8")]  =(void *)3;
	m_LetterTable[_T("9")]  =(void *)3;
	m_LetterTable[_T(".")]  =(void *)3;


	m_LetterTable[_T("a")]  =(void *)4;
	m_LetterTable[_T("b")]  =(void *)4;
	m_LetterTable[_T("c")]  =(void *)4;
	m_LetterTable[_T("d")]  =(void *)4;
	m_LetterTable[_T("e")]  =(void *)4;
	m_LetterTable[_T("f")]  =(void *)4;
	m_LetterTable[_T("g")]  =(void *)4;
	m_LetterTable[_T("h")]  =(void *)4;
	m_LetterTable[_T("i")]  =(void *)4;
	m_LetterTable[_T("j")]  =(void *)4;
	m_LetterTable[_T("k")]  =(void *)4;
	m_LetterTable[_T("l")]  =(void *)4;
	m_LetterTable[_T("m")]  =(void *)4;
	m_LetterTable[_T("n")]  =(void *)4;
	m_LetterTable[_T("o")]  =(void *)4;
	m_LetterTable[_T("p")]  =(void *)4;
	m_LetterTable[_T("q")]  =(void *)4;
	m_LetterTable[_T("r")]  =(void *)4;
	m_LetterTable[_T("s")]  =(void *)4;
	m_LetterTable[_T("t")]  =(void *)4;
	m_LetterTable[_T("u")]  =(void *)4;
	m_LetterTable[_T("v")]  =(void *)4;
	m_LetterTable[_T("w")]  =(void *)4;
	m_LetterTable[_T("x")]  =(void *)4;
	m_LetterTable[_T("y")]  =(void *)4;
	m_LetterTable[_T("z")]  =(void *)4;
	m_LetterTable[_T("_")]  =(void *)4;
	m_LetterTable[_T("#")]  =(void *)4;
	m_LetterTable[_T("$")]  =(void *)4;
	m_LetterTable[_T("@")]  =(void *)4;

	m_LetterTable[_T("+")]  =(void *)5;
	m_LetterTable[_T("-")]  =(void *)5;
	m_LetterTable[_T("*")]  =(void *)5;
	m_LetterTable[_T("/")]  =(void *)5;
	m_LetterTable[_T(">")]  =(void *)5;
	m_LetterTable[_T(",")]  =(void *)5;
	m_LetterTable[_T("<")]  =(void *)5;
	m_LetterTable[_T("=")]  =(void *)5;
	m_LetterTable[_T(")")]  =(void *)5;
	m_LetterTable[_T("(")]  =(void *)5;
	m_LetterTable[_T("!")]  =(void *)5;
	m_LetterTable[_T(":")]  =(void *)5;

	m_LetterTable[_T(" ")]  =(void *)2;
	m_LetterTable[_T("\n")] =(void *)2;
	m_LetterTable[_T("\t")] =(void *)2;
	m_LetterTable[_T("\r")] =(void *)2;
	m_LetterTable[_T("{")]  =(void *)1;
	m_LetterTable[_T("}")]  =(void *)1;

	m_LetterTable[_T("\"")] =(void *)6;// " 引用
	m_LetterTable[_T("'")]  =(void *)7;// ' 字符串

	bExec=true;
}

int CFormularComputeParent::FuncFromDll(int n)
{
	INPUT_INFO inputInfo;
	int nParam ;
	LookupParameterTable(n, nParam);
	ASSERT(nParam>=0&&nParam<=10);

	if(m_pReportData==NULL)
	{
		m_pDoc->m_sharesInformation .Lookup (this->m_symbol.GetBuffer(0),this->m_pReportData,m_stkKind);
		m_symbol.ReleaseBuffer();
	}
	ASSERT(m_pReportData!=NULL);

	if(m_pReportData==NULL)
		return 8500;

	BASEINFO* pBase = this->m_pReportData->pBaseInfo;
	inputInfo.financeData = pBase;
	inputInfo.klineType  = (klineKind)m_nKindKline;
	inputInfo.pDat1   = m_pReportData;
	inputInfo.strSymbol   = m_symbol.GetBuffer(0);
	m_symbol.ReleaseBuffer();
	inputInfo.nNumData    = m_MaxPoint;
	inputInfo.pData    = m_pData;
	inputInfo.pDataEx     = NULL;
	inputInfo.m_stkKind = m_stkKind;

	if(inputInfo.klineType==HS_KLINE)
	{
		if(m_pKlineEx == NULL) 
			CTaiKlineFileHS::TransferDataEx (m_pBuySellList,m_pKlineEx);
		if(m_pKlineEx != NULL) 
			inputInfo.pDataEx = m_pKlineEx;
	}
//		CTaiKlineFileHS::TransferDataEx(m_pBuySellList, inputInfo.pDataEx);

//	m_pIndex = pIndex;
//	m_pBuySellList = pBuySellList;

	int cl;
	int i;
	for(i=0;i<nParam;i++)
	{
		inputInfo.fInputParam[i].kind = ARRAY_BE::Normal;
		inputInfo.fInputParam[i].line = new float[m_MaxPoint];
		cl=m_table.pop_it();
		GetData(cl,inputInfo.fInputParam[i]);
	}
	if(nParam<10)
		inputInfo.fInputParam[nParam].line = NULL;
	inputInfo.fOutputParam.line = new float[m_MaxPoint];

	//do from dll
	int nRtn;
	nRtn = FuncInDll(&inputInfo, n-FuncDllB);
	//end
//	m_MidArray[m_MidNum] = inputInfo.fOutputParam  ;//new float[m_MaxPoint];
	m_MidArray[m_MidNum].line = inputInfo.fOutputParam.line;//new float[m_MaxPoint];
	m_MidArray[m_MidNum] = inputInfo.fOutputParam;
/*	m_MidArray[m_MidNum].b =inputInfo.fOutputParam.b; 
	m_MidArray[m_MidNum].e =inputInfo.fOutputParam.e;
*/	
	m_table.push_b(PCMB+m_MidNum);
	m_MidNum++;
	
	for(i=0;i<nParam;i++)
	{
		delete[] inputInfo.fInputParam[i].line;
	}
//	if(inputInfo.pDataEx!=NULL) delete[]inputInfo.pDataEx;

	return nRtn;
}
struct ValueFoot
{
	int		nFoot;
	float	fValue;
};

int CFormularComputeParent::FuncInDll(INPUT_INFO *pInput, int nFunc)
{
	pInput->fOutputParam.kind = ARRAY_BE::Normal;

	bool bAdd;
	int i,j,k,l,m,nRtn=0;
	float f,fTemp;
	if(pInput->nNumData <=0)
		return 800;
	int nParam ;
	LookupParameterTable(nFunc+FuncDllB, nParam);
	ASSERT(nParam>=0&&nParam<=10);
	j = 0;
	for(i = 0;i<nParam;i++)
	{
		if(j<pInput->fInputParam[i].b)
			j=pInput->fInputParam[i].b;
		if(pInput->fInputParam[i].b<0 ||pInput->fInputParam[i].b>pInput->fInputParam[i].e)
		{
//			ASSERT(FALSE);
			nRtn = 800;
			return nRtn;
		}
	}

	switch(nFunc)
	{
	case 0://beeline
	case 1://radial
	case 2://wirebar
		//GetFoot(INPUT_INFO *pInput, int nParam)
		f=pInput->fInputParam[4].line[pInput->fInputParam[4].b];
		k=GetFoot(pInput,0);
		l=k+pInput->fInputParam[3].line[pInput->fInputParam[3].b];//GetFoot(pInput,3);
		j=max(pInput->fInputParam[4].b,pInput->fInputParam[2].b);
		if(nFunc>1)
			j=max(j,k);
		if(l!=k)
			for(i=j;i<pInput->nNumData ;i++)
			{
				pInput->fOutputParam .line[i] = (f-pInput->fInputParam[2].line[pInput->fInputParam[2].b]) * (i-l)/(l-k)+f;
			}
		else
			for(i=j;i<pInput->nNumData ;i++)
			{
				pInput->fOutputParam .line[i]=f;
			}
		pInput->fOutputParam.b=j;
		pInput->fOutputParam.e=pInput->nNumData-1;
		break;
	case 3://BACKSET
		k=pInput->fInputParam[1].line[pInput->fInputParam[1].b];
		ASSERT(k>0);
		if(k<0)
			break;
		j=pInput->fInputParam[0].b;
		j+=(k);

		memset(pInput->fOutputParam .line ,0,pInput->nNumData*4);
		for(i=j;i<pInput->nNumData ;i++)
		{
			if(pInput->fInputParam[0].line[i]!=0)
			{
				pInput->fOutputParam .line[i]=1;
				for(int l=0;l<k;l++)
					pInput->fOutputParam .line[i-k-1] =1;
			}
		}
		pInput->fOutputParam.b=j;
		pInput->fOutputParam.e=pInput->nNumData-1;
		break;
	case 4://BARSCOUNT
		j=pInput->fInputParam[0].b;
		k=pInput->nNumData-j;
		for(i=j;i<pInput->nNumData ;i++)
		{
			pInput->fOutputParam .line[i] =i-j;
		}
		pInput->fOutputParam.b=j;
		pInput->fOutputParam.e=pInput->nNumData-1;
		break;
		/*
BACKSET 	将当前位置到若干周期前的数据设为1。
用法:
BACKSET(X,N),若X非0,则将当前位置到N周期前的数值设为1。
例如：BACKSET(CLOSE>OPEN,2)若收阳则将该周期及前一周期数值设为1,否则为0

BARSCOUNT		求总的周期数。
用法:
BARSCOUNT(X)第一个有效数据到当前的天数
例如：BARSCOUNT(CLOSE)对于日线数据取得上市以来总交易日数，对于分笔成交取得当日成交笔数，对于1分钟线取得当日交易分钟数
*/
	case 5://BARSLAST
		j=pInput->fInputParam[0].b;
		k=j;
		l=j;
		for(i=j;i<pInput->nNumData ;i++)
		{
			pInput->fOutputParam .line[i] =i-k;
			if(pInput->fInputParam[0].line[i]!=0)
			{
				if(j==k)
					l=i;
				k=i;
			}
		}
		pInput->fOutputParam.b=l;
		pInput->fOutputParam.e=pInput->nNumData-1;
		break;
/*BARSLAST		上一次条件成立到当前的周期数。
用法:
BARSLAST(X):上一次X不为0到现在的天数
例如：BARSLAST(CLOSE/REF(CLOSE,1)>=1.1)表示上一个涨停板到当前的周期数

BARSSINCE		第一个条件成立到当前的周期数。
用法:
BARSSINCE(X):第一次X不为0到现在的天数
例如：BARSSINCE(HIGH>10)表示股价超过10元时到当前的周期数
*/
	case 6://BARSSINCE
		j=pInput->fInputParam[0].b;
		k=j;
		l=j;
		for(i=j;i<pInput->nNumData ;i++)
		{
			pInput->fOutputParam .line[i] =i-k;
			if(pInput->fInputParam[0].line[i]!=0)
			{
				if(j==k)
				{
					l=i;
					k=i;
				}
			}
		}
		pInput->fOutputParam.b=l;
		pInput->fOutputParam.e=pInput->nNumData-1;
		break;
	case 7://DMA
		j=pInput->fInputParam[0].b;
		if(pInput->fInputParam[1].b>j)
			j=pInput->fInputParam[1].b;
		f=pInput->fInputParam[1].line[pInput->fInputParam[1].b];
		j++;
		pInput->fOutputParam .line[j-1] = pInput->fInputParam[0].line[j];
		for(i=j;i<pInput->nNumData ;i++)
		{
			f=pInput->fInputParam[1].line[i];

			if(f>1)
			{
				f = 1;
				ASSERT(FALSE);
			}
			pInput->fOutputParam .line[i] =f*pInput->fInputParam[0].line[i]+(1-f)*pInput->fOutputParam .line[i-1];
		}
		pInput->fOutputParam.b=j;
		pInput->fOutputParam.e=pInput->nNumData-1;
		break;
/*DMA	求动态移动平均。
用法:
DMA(X,A),求X的动态移动平均。
算法: 若Y=DMA(X,A)
则 Y=A*X+(1-A)*Y',其中Y'表示上一周期Y值,A必须小于1。
例如：DMA(CLOSE,VOL/CAPITAL)表示求以换手率作平滑因子的平均价

HHVBARS		求上一高点到当前的周期数。
用法:
HHVBARS(X,N):求N周期内X最高值到当前周期数，N=0表示从第一个有效值开始统计
例如：HHVBARS(HIGH,0)求得历史新高到到当前的周期数

LLVBARS		求上一低点到当前的周期数。
用法:
LLVBARS(X,N):求N周期内X最低值到当前周期数，N=0表示从第一个有效值开始统计
例如：LLVBARS(HIGH,20)求得20日最低点到当前的周期数

*/
	case 8://HHVBARS
	case 9://LLVBARS
		j=pInput->fInputParam[0].b;
		k=pInput->fInputParam[1].line[pInput->fInputParam[1].b];
		if(k<0)
			break;
		if(k==0)
		{
			l=j;//max foot
			f=pInput->fInputParam[0].line[j];//max or min
			for(i=j;i<pInput->nNumData ;i++)
			{
				if(pInput->fInputParam[0].line[i]>f&& nFunc==8
					||pInput->fInputParam[0].line[i]<f&& nFunc==9)
				{
					f=pInput->fInputParam[0].line[i];
					l=i;
				}
				pInput->fOutputParam .line[i] = i-l;
			}
		}
		else
		{
			j+=(k-1);
			for(int ii=j;ii<pInput->nNumData ;ii++)
			{
				l=ii;//max foot
				f=pInput->fInputParam[0].line[ii];//max
				for(i=ii-k+1;i<=ii ;i++)
				{
					if(pInput->fInputParam[0].line[i]>f&& nFunc==8
						||pInput->fInputParam[0].line[i]<f&& nFunc==9)
					{
						f=pInput->fInputParam[0].line[i];
						l=i;
					}
				}
				pInput->fOutputParam .line[ii] = ii-l;
			}
		}
		pInput->fOutputParam.b=j;
		pInput->fOutputParam.e=pInput->nNumData-1;
		break;
	case 10://ZIG2
	case 39://ZIG
/*ZIG		之字转向。
用法:
ZIG(K,N),当价格变化量超过N%时转向,K表示0:开盘价,1:最高价,2:最低价,3:收盘价
例如：ZIG(3,5)表示收盘价的5%的ZIG转向
*/
		{
			CArray<ValueFoot,ValueFoot&> fArray;
			FuncZigPre(pInput, nFunc);
			FuncZigGroup(pInput, &fArray);
		//to compute mid value
			for(i=1;i<fArray.GetSize();i++)
			{
				j=fArray[i-1].nFoot ;
				k=fArray[i].nFoot ;
				if(j==k)
				{
					pInput->fOutputParam .line[j]=fArray[i].fValue ;
					continue;
				}
				for(int ii=j;ii<=k;ii++)
				{
					pInput->fOutputParam .line[ii]=(fArray[i].fValue -fArray[i-1].fValue )
						*(ii-j)/(k-j)+fArray[i-1].fValue ;
				}
			}
		//end
		}
		break;
/*TROUGH	前M个ZIG转向波谷值。
用法:
TROUGH(K,N,M)表示之字转向ZIG(K,N)的前M个波谷的数值,M必须大于等于1
例如：TROUGH(2,5,2)表示%5最低价ZIG转向的前2个波谷的数值

TROUGHBARS	前M个ZIG转向波谷到当前距离。
用法:
TROUGHBARS(K,N,M)表示之字转向ZIG(K,N)的前M个波谷到当前的周期数,M必须大于等于1
例如：TROUGH(2,5,2)表示%5最低价ZIG转向的前2个波谷到当前的周期数


PEAK	前M个ZIG转向波峰值。
用法:
PEAK(K,N,M)表示之字转向ZIG(K,N)的前M个波峰的数值,M必须大于等于1
例如：PEAK(1,5,1)表示%5最高价ZIG转向的上一个波峰的数值

PEAKBARS	前M个ZIG转向波峰到当前距离。
用法:
PEAKBARS(K,N,M)表示之字转向ZIG(K,N)的前M个波峰到当前的周期数,M必须大于等于1
例如：PEAK(0,5,1)表示%5开盘价ZIG转向的上一个波峰到当前的周期数

*/
	case 11://PEAK2
	case 12://PEAKBARS2
	case 13://TROUGH2
	case 14://TROUGHBARS2
	case 40://PEAK
	case 41://PEAKBARS
	case 42://TROUGH
	case 43://TROUGHBARS
		{
			CArray<ValueFoot,ValueFoot&> fArr;
			FuncZigPre(pInput, nFunc);
			FuncZigGroup(pInput, &fArr);
			if(fArr.GetSize ()==0)
				break;
			j=pInput->fOutputParam .b;
			m=pInput->fInputParam[2].line[pInput->fInputParam[2].b];//m is count
			if(m<1)
				break;
			if(j<0||j>=pInput->nNumData) break;

			float* fp=pInput->fOutputParam.line;
			k=0;
			l=j;//begin point

			CArray<ValueFoot,ValueFoot&> fArray;
			ValueFoot valueFo;
			fTemp=fArr[0].fValue ;

			//add to array
			for(i=1;i<fArr.GetSize ();i++)
			{
				if(nFunc==11||nFunc==12)
				{
					if(fArr[i].fValue <fArr[i-1].fValue )
					{
						fArray.Add ( fArr[i-1]);
					}
				}
				else if(fArr[i].fValue > fArr[i-1].fValue )
						fArray.Add ( fArr[i-1]);
			}
			if(fArray.GetSize ()<1 || fArr.GetSize ()<1)
				break;
			if(fArr[fArr.GetSize ()-1].nFoot > fArray[fArray.GetSize ()-1].nFoot )
				fArray.Add(fArr[fArr.GetSize ()-1]);
			
			if(fArray.GetSize ()<m)
				break;
			l=fArray[m-1].nFoot+1 ;
			for(i=m-1;i<fArray.GetSize ()-1;i++)
			{
				for(j=fArray[i].nFoot+1;j<=fArray[i+1].nFoot ;j++)
				{
					if(nFunc==11||nFunc==13)
						fp[j]=fArray[i-m+1].fValue;
					else
						fp[j]=j-fArray[i-m+1].nFoot;
				}
			}
		}
		pInput->fOutputParam .b=l;
		break;
	case 15://FORCAST
	case 16://SLOPE
		{
			j=pInput->fInputParam[0].b;
			k=pInput->fInputParam[1].line[pInput->fInputParam[1].b];
			if(k<2)
				break;
			j=j+k-1;
	/*根据一元回归分析的最小二乘法求N根日K线的线性回归。
	Yi=K0+K1*Xi    I=1，…n,
	K0=Y-K1*X
	K1=(Σ(Xi-X)Yi/ Σ(Xi-X) (Xi-X)
	其中: X=(1/N) Σxi
		 Y=(1/N) ΣYi
		 N=1,   N
	*/
			//--		计算平均值
			for(m=j;m<pInput->nNumData;m++)
			{
				float x=0;// X=(1/N) Σxi
				float y=0;// Y=(1/N) ΣYi
				float sumTop=0;
				float sumBottom=0;
				float k0=0;
				float k1=0;

				for(i=m-k+1;i<=m;i++)
				{
					x+=i;
					y+=pInput->fInputParam[0].line[i];
				}

				x=x/(k);
				y=y/(k);
				for(i=m-k+1;i<=m;i++)
				{
					sumTop+=(i-x)*pInput->fInputParam[0].line[i];
					sumBottom+=(i-x)*(i-x);
				}
				if(sumBottom!=0)
					k1=sumTop/sumBottom;
				else k1=1;
				k0=y-k1*x;

				if(nFunc==15)
					pInput->fOutputParam.line[m]=m*k1+k0;
				else
					pInput->fOutputParam.line[m]=k1;

			}
		}
		pInput->fOutputParam.b=j;
		break;
/*	UPWARD		连日上升。
                   	用法:
UPWARD (P，D),P指标或行情数据，D为天数，返回是否一直上升。
	DOWNWARD		连日下降。
                   	用法:
DOWNWARD (P，D),P指标或行情数据，D为天数，返回是否一直下降。

*/
	case 17://UPWARD
	case 18://DOWNWARD
		j=pInput->fInputParam[1].b;
		k=pInput->fInputParam[1].line[j];//周期数
		if(pInput->fInputParam[0].b>j)
			j=pInput->fInputParam[0].b;
		l=j+k;
		for(i=l;i<pInput->nNumData ;i++)
		{
			float isOk = 1;
			for(int ii=0;ii<k;ii++)
			{
				if(nFunc ==17)
				{
					if(pInput->fInputParam[0].line[i-ii]<pInput->fInputParam[0].line[i-ii-1])
					{
						isOk = 0;
						break;
					}
				}
				else
				{
					if(pInput->fInputParam[0].line[i-ii]>pInput->fInputParam[0].line[i-ii-1])
					{
						isOk = 0;
						break;
					}
				}
			}
			pInput->fOutputParam .line[i] =(float)isOk;
		}
		pInput->fOutputParam.b=l;
		pInput->fOutputParam.e=pInput->nNumData-1;
		break;
/*	  SLOWUPCROSS	维持几周期后上穿。
                   		用法:
SLOWUPCROSS (D, P1，P2),D维持几天没有上穿，P1指标或行情数据，P2指标或行情数据，返回是否成功。D等于1相当于CROSS(P1,P2)。

	  SLOWDOWNCROSS		维持几周期后下穿。
                   			用法:
SLOWDOWNCROSS (D, P1，P2),D维持几天没有下穿，P1指标或行情数据，P2指标或行情数据，返回是否成功。
		
*/
	case 19://SLOWUPCROSS
	case 20://SLOWDOWNCROSS
		j=pInput->fInputParam[0].b;
		k=pInput->fInputParam[0].line[j];//周期数
		if(pInput->fInputParam[1].b>j)
			j=pInput->fInputParam[1].b;
		if(pInput->fInputParam[2].b>j)
			j=pInput->fInputParam[2].b;
		l=j+k;
		for(i=l;i<pInput->nNumData ;i++)
		{
			float isOk = 1;
			if(nFunc ==19)
			{
				for(int ii=1;ii<k+1;ii++)
				{
					if(pInput->fInputParam[1].line[i-ii]>=pInput->fInputParam[2].line[i-ii])
					{
						isOk = 0;
						break;
					}
				}
				if(pInput->fInputParam[1].line[i]<pInput->fInputParam[2].line[i])
					isOk = 0;
			}
			else
			{
				for(int ii=1;ii<k+1;ii++)
				{
					if(pInput->fInputParam[1].line[i-ii]<=pInput->fInputParam[2].line[i-ii])
					{
						isOk = 0;
						break;
					}
				}
				if(pInput->fInputParam[1].line[i]>pInput->fInputParam[2].line[i])
					isOk = 0;
			}

			pInput->fOutputParam .line[i] =(float)isOk;
		}
		pInput->fOutputParam.b=l;
		pInput->fOutputParam.e=pInput->nNumData-1;
		break;
/*	    FLOATPERCENT		流通盘分布比例函数
                   			用法:
    FLOATPERCENT (P),分布在P(指标或行情数据)以上的所有价位的成交量的和占总流通盘的百分比。
     FLOATPERCENT (P)- FLOATPERCENT (Q)为P与Q之间的百分比。
		  FLOATTAPE	流通盘分布类函数
                   		用法:
FLOATTAPE (N),N=1为最高峰处的价格。
N=2为最高峰处的成交量密度（手/分）。
N=3为平均成本处的价格。
N=4为次高峰处的价格。
N=5为次高峰处的成交量密度（手/分）。

*/
	case 21://FLOATPERCENT
	case 22://FLOATTAPE
	case 44://winner
	case 45://cost
		k=1;
		while(1)
		{
			if(pInput->pData == NULL)
			{
				k=0;
				break;
			}
			j=pInput->fInputParam[0].b;
			l=j;

			float fMax=0;
			float fMin=90000000;
			CTaiKlineMoveChenben::GetMaxMin(pInput->pData , pInput->nNumData , fMax, fMin);
			float fRuler;
			int n =CTaiKlineMoveChenben::GetRuler( fMax,  fMin, fRuler);

			if(n<=0 )
			{
				k=0;
				break;
			}
			CReportData* pdt = NULL;
			if(!CMainFrame::m_pDoc ->m_sharesInformation .Lookup (pInput->strSymbol ,pdt,pInput->m_stkKind))
			{
				k=0;
				break;
			}
			float ltp = CTaiShanKlineShowView::GetCapital(pdt);
			if(pdt->id[1] == 'A')
				ltp = ltp/100;
			if( pdt->pBaseInfo!=NULL && ltp>0 )
			{
//				k=0;
//				break;
				float* cnp = new float[n];
				memset(cnp,0,n*4);

				if(nFunc == 21 ||nFunc == 44 )
				{
					for(i=0;i<pInput->nNumData ;i++)
					{
						CTaiKlineMoveChenben::GetMoveCBPerFoot(pInput->pData+i, cnp, ltp, fMax, fMin, i);
						if(i<l)
						{
							continue;
						}
						f=pInput->fInputParam[0].line[i];//
						if(f>fMax )
						{
							f=0;
						}
						else if(f<fMin)
						{
							f=1;
						}
						else
						{
							int nSeprate = (int)(f*fRuler)-(int)(fMin*fRuler);
							f=0;
							for(int ii=0;ii<=nSeprate;ii++)
							{
								f+=cnp[ii];
							}
							f/=ltp;
							f=1-f;
							if(f<0)
								f=0;
						}
						pInput->fOutputParam .line[i] =f;
						if(nFunc == 44) pInput->fOutputParam .line[i] =1-f;

					}
				}
				else if(nFunc == 45)
				{
					float fPercent = pInput->fInputParam[0].line[l]/100;
					float fPrc = 0;
					for(i=0;i<pInput->nNumData ;i++)
					{
						CTaiKlineMoveChenben::GetMoveCBPerFoot(pInput->pData+i, cnp, ltp, fMax, fMin, i);
						if(i<l)
						{
							continue;
						}
						{
							f=0;
							for(int ii=0;ii<n ;ii++)
							{
								f+=cnp[ii];
								if(f/ltp>=fPercent)
								{
									fPrc = fMin+(float)ii/fRuler;
									break;
								}
							}
						}
						pInput->fOutputParam .line[i] =fPrc;

					}
				}
				else
				{
					int nIn =pInput->fInputParam[0].line[l];//
					for(i=0;i<pInput->nNumData ;i++)
					{
						CTaiKlineMoveChenben::GetMoveCBPerFoot(pInput->pData+i, cnp, ltp, fMax, fMin, i);
						if(i<l)
						{
							continue;
						}

						f=cnp[0];
						float amount=0;
						int nFootMax = 0;
						int nFootMax2 = 0;
						int nFootMax3 = 0;
						float max = 0;
						int nBeginY=fMin*fRuler;
						fTemp =cnp[0];
						for(int ii=0;ii<n;ii++)
						{
							amount+=(cnp[ii]*(ii+nBeginY)/(float)fRuler);
							if(cnp[ii]>max)
							{
								nFootMax = ii;
								max=cnp[ii];
							}

							//second max
							if(ii>1)
							{
								if(cnp[ii]<cnp[ii-1] && cnp[ii-1]>=cnp[ii-2])
								{
									if(cnp[ii-1]>f)
									{
										nFootMax3 = nFootMax2;
										nFootMax2 = ii-1;
										fTemp = f;
										f=cnp[ii-1];
									}
								}
							}
						}
						switch(nIn)
						{
						case 1:
							f=(nFootMax+nBeginY)/(float)fRuler;
							break;
						case 2:
							f=cnp[nFootMax];
							break;
						case 3:
							f=amount/ltp;
							break;
	//					case 4:
	//						f=(nFootMax3+nBeginY)/(float)100;
	//						break;
	//					case 5:
	//						f=cnp[nFootMax3];
	//						break;
						default:
							ASSERT(FALSE);
							k=0;
							break;
						}
						pInput->fOutputParam .line[i] =f;

					}
				}
			}
			else
			{
				for(i=0;i<pInput->nNumData ;i++)
					pInput->fOutputParam .line[i] = 0;
			}
			break;
		}
		if(k==0)
			pInput->fOutputParam.b=-1;
		else
			pInput->fOutputParam.b=l;
		pInput->fOutputParam.e=pInput->nNumData-1;
		break;
	case 23://VALUEDAY 0
		{
		k=1;
//		m_bTotalData = TRUE;
		j=pInput->fInputParam[0].b;
		if(j<pInput->fInputParam[1].b)
			j=pInput->fInputParam[1].b;
		if(j<pInput->fInputParam[2].b)
			j=pInput->fInputParam[2].b;
		if(CTaiKlineFileKLine::IsDayKline(pInput->klineType ))
			k=0;
		f=0;
		for(i=j;i<pInput->nNumData ;i++)
		{
			f=pInput->fInputParam[2].line[i];
			CString s=CTime(pInput->pData[i].day ).Format ("%Y%m%d");
			int nDay= atoi(s);
			if(k==0)
			{
				if(pInput->fInputParam[0].line[pInput->fInputParam[0].b]<19700101)
				{
					if(nDay>= pInput->fInputParam[0].line[pInput->fInputParam[0].b]*100+pInput->fInputParam[1].line[pInput->fInputParam[1].b]/10000)
						break;
				}
				else if(nDay>= pInput->fInputParam[0].line[pInput->fInputParam[0].b])
					break;
			}
			else
			{
				CString s2=CTime(pInput->pData[i].day ).Format ("%H%M");
				int nHour= atoi(s2);

				if(pInput->fInputParam[0].line[pInput->fInputParam[0].b]<19700101)
				{
					if(nDay>= pInput->fInputParam[0].line[pInput->fInputParam[0].b]*100+pInput->fInputParam[1].line[pInput->fInputParam[1].b]/10000
						&&nHour>= (int)(pInput->fInputParam[1].line[pInput->fInputParam[1].b])%10000)
						break;
				}
				else if(nDay>= pInput->fInputParam[0].line[pInput->fInputParam[0].b]
					&& nHour>= pInput->fInputParam[1].line[pInput->fInputParam[1].b])
					break;
			}

		}
		for(i=j;i<pInput->nNumData ;i++)
			pInput->fOutputParam.line[i]=f;

		pInput->fOutputParam.b=j;
		pInput->fOutputParam.e=pInput->nNumData-1;
		}
		break;
/*
	6、时间函数
VALUEDAY	取得该周期的日期。
用法:
VALUEDAY（DAY，HOUR，P）返回DAY年月日HOUR时分的P的值
如：VALUEDAY（19990956，0945，CLOSE）

自累加函数
selfadd	
用法:
selfadd（a，b，isadd）表示 如果isadd 等于0则x[i] = a+b*x[i-1],如果isadd 等于0 则x[i]=x[i-1]
如：selfadd（0，close，1）	*/	
	case 24://selfadd
		j=pInput->fInputParam[0].b;
		if(j<pInput->fInputParam[1].b)
			j=pInput->fInputParam[1].b;
		if(j<pInput->fInputParam[2].b)
			j=pInput->fInputParam[2].b;
		for(i=j;i<pInput->nNumData ;i++)
		{
			if(pInput->fInputParam[2].line[j] == 2 && i==j) 
			{
				pInput->fOutputParam.line[i] = 100;
				continue;
			}
			if(pInput->fInputParam[2].line[i]>0)
			{
				pInput->fOutputParam.line[i]=
					pInput->fInputParam[0].line[i]+pInput->fInputParam[1].line[i]*(i==j?100:pInput->fOutputParam.line[i-1]);
			}
			else
			{
				pInput->fOutputParam.line[i]=(i==j?100:pInput->fOutputParam.line[i-1]);
			}
		}
		pInput->fOutputParam.b=j;
		pInput->fOutputParam.e=pInput->nNumData-1;
		break;
	case 25://sin
		j=pInput->fInputParam[0].b;
		for(i=j;i<pInput->nNumData ;i++)
			pInput->fOutputParam.line[i] = sin(pInput->fInputParam[0].line[i]);
		pInput->fOutputParam.b=j;
		break;
	case 26://cos
		j=pInput->fInputParam[0].b;
		for(i=j;i<pInput->nNumData ;i++)
			pInput->fOutputParam.line[i] = cos(pInput->fInputParam[0].line[i]);
		pInput->fOutputParam.b=j;
		break;
	case 27://tan
		j=pInput->fInputParam[0].b;
		try
		{
		for(i=j;i<pInput->nNumData ;i++)
			pInput->fOutputParam.line[i] = tan(pInput->fInputParam[0].line[i]);
		}
		catch(...)
		{
		for(i=j;i<pInput->nNumData ;i++)
			pInput->fOutputParam.line[i] = 0;
		}
		pInput->fOutputParam.b=j;
		break;
	case 28://asin
		j=pInput->fInputParam[0].b;
		for(i=j;i<pInput->nNumData ;i++)
		{
			f = pInput->fInputParam[0].line[i];
			if(f>1) f=1;
			if(f<-1)f=-1;
			pInput->fOutputParam.line[i] = asin(f);
		}
		pInput->fOutputParam.b=j;
		break;
	case 29://acos
		j=pInput->fInputParam[0].b;
		for(i=j;i<pInput->nNumData ;i++)
		{
			f = pInput->fInputParam[0].line[i];
			if(f>1) f=1;
			if(f<-1)f=-1;
			pInput->fOutputParam.line[i] = acos(f);
		}
		pInput->fOutputParam.b=j;
		break;
	case 30://atan
		j=pInput->fInputParam[0].b;
		for(i=j;i<pInput->nNumData ;i++)
			pInput->fOutputParam.line[i] = atan(pInput->fInputParam[0].line[i]);
		pInput->fOutputParam.b=j;
		break;
	case 31://BACKSET2
		k=pInput->fInputParam[1].line[pInput->fInputParam[1].b];
//		ASSERT(k>0);
		if(k<0) k=0;
//			break;
		{// get the max begin foot
			for(int m = pInput->fInputParam[1].b+1;m<pInput->nNumData ;m++)
			{
				int nn = pInput->fInputParam[1].line[m];
				if(nn>m)
					k = nn;
			}
		}
		j=pInput->fInputParam[0].b;
		if(j<pInput->fInputParam[1].b) j= pInput->fInputParam[1].b;
		j+=(k+1);

		for(i=j;i<pInput->nNumData ;i++)
		{
			k=pInput->fInputParam[1].line[i];
			if(k<0) k=0;
			for(int l=-1;l<k;l++)
			{
				pInput->fOutputParam .line[i-l-1] =pInput->fInputParam[0].line[i];
			}
		}
		pInput->fOutputParam.b=j;
		break;
		//towervalue
	case 32://towervalue
		j=pInput->fInputParam[0].b;
		j++;
		if(j<0) j=0;
		if(j>=pInput->nNumData) 
		{
			pInput->fOutputParam.b=-1;
			break;
		}

		k=pInput->fInputParam[0].line[j] ;
		if(k>4 || k<1) 
		{
			pInput->fOutputParam.b=-1;
			break;
		}

		{
			float closeYest = pInput->pData[j-1].close;//m_pkline[0].close;//起步价
			float openYest = pInput->pData[j-1].open;
			float f11[4];
			f11[0] = closeYest,f11[1] = closeYest,f11[2] = closeYest,f11[3] = 1;
			for(i=j;i<pInput->nNumData ;i++)
			{
				float close = pInput->pData[i].close;
				if( openYest <= closeYest )
				{
					if(close>=closeYest)//draw red
					{
						f11[0] = close;
						f11[1] = closeYest;
						f11[2] = close;
						f11[3] = 1;

						openYest = closeYest;
						closeYest = close;
					}
					else
					{
						if(close>=openYest)//draw red
						{
							f11[0] = close;
							f11[1] = closeYest;
							f11[2] = close;
							f11[3] = 1;

							openYest = close;
						}
						else//draw red and green
						{
							f11[0] = closeYest;
							f11[1] = close;
							f11[2] = openYest;
							f11[3] = 3;

							openYest = closeYest;
							closeYest = close;
						}
					}
				}
				else
				{
					if(close<=closeYest)//draw green
					{
						f11[0] = closeYest;
						f11[1] = close;
						f11[2] = close;
						f11[3] = 2;

						openYest = closeYest;
						closeYest = close;
					}
					else
					{
						if(close<=openYest)//draw green
						{
							f11[0] = close;
							f11[1] = closeYest;
							f11[2] = closeYest;
							f11[3] = 2;

							openYest = close;
						}
						else//draw red and green
						{
							f11[0] = close;
							f11[1] = closeYest;
							f11[2] = openYest;
							f11[3] = 3;

							openYest = closeYest;
							closeYest = close;
						}
					}
				}
				pInput->fOutputParam .line[i] = f11[k-1];
			}
		}
		pInput->fOutputParam.b=j;
		break;
	case 33://int
		j=pInput->fInputParam[0].b;
		for(i=j;i<pInput->nNumData ;i++)
			pInput->fOutputParam.line[i] = int(pInput->fInputParam[0].line[i]);
		pInput->fOutputParam.b=j;
		break;
	case 34:
	case 38:
		k=1;
		while(1)
		{
			if(pInput->pData == NULL)
			{
				k=0;
				break;
			}
			j=pInput->fInputParam[0].b;
			l=j;
			float fLTP;
			float fCJL;
			fLTP=pInput->fInputParam[1].line[0];
			fCJL=pInput->fInputParam[2].line[0];
			float fMax=0;
			float fMin=90000000;
			CTaiKlineMoveChenben::GetMaxMin(pInput->pData , pInput->nNumData , fMax, fMin);
			float fRuler;
			int n =CTaiKlineMoveChenben::GetRuler( fMax,  fMin, fRuler);

			if(n<=0 )
			{
				k=0;
				break;
			}
			CReportData* pdt = NULL;
			if(!CMainFrame::m_pDoc ->m_sharesInformation .Lookup (pInput->strSymbol ,pdt,pInput->m_stkKind))
			{
				k=0;
				break;
			}
			float ltp = CTaiShanKlineShowView::GetCapital(pdt);
			ltp=ltp*fLTP;
			if(pdt->id[1] == 'A')
				ltp = ltp/100;
			if( pdt->pBaseInfo!=NULL && ltp>0 )
			{
//				k=0;
//				break;
				float* cnp = new float[n];
				memset(cnp,0,n*4);

				if(nFunc == 34)
				{
					for(i=0;i<pInput->nNumData ;i++)
					{
						CTaiKlineMoveChenben::GetMoveCBPerFoot(pInput->pData+i, cnp, ltp, fMax, fMin, i,fCJL);
						if(i<l)
						{
							continue;
						}
						f=pInput->fInputParam[0].line[i];//
						if(f>fMax )
						{
							f=0;
						}
						else if(f<fMin)
						{
							f=1;
						}
						else
						{
							int nSeprate = (int)(f*fRuler)-(int)(fMin*fRuler);
							f=0;
							for(int ii=0;ii<=nSeprate;ii++)
							{
								f+=cnp[ii];
							}
							f/=ltp;
							f=1-f;
							if(f<0)
								f=0;
						}
						pInput->fOutputParam .line[i] =f;

					}
				}
				else //38
				{
					int nIn =pInput->fInputParam[0].line[l];//
					for(i=0;i<pInput->nNumData ;i++)
					{
						CTaiKlineMoveChenben::GetMoveCBPerFoot(pInput->pData+i, cnp, ltp, fMax, fMin, i,fCJL);
						if(i<l)
						{
							continue;
						}
						f=cnp[0];
						float amount=0;
						int nFootMax = 0;
						float max = 0;
						int nBeginY=fMin*fRuler;
						fTemp =cnp[0];
						for(int ii=0;ii<n;ii++)
						{
							amount+=(cnp[ii]*(ii+nBeginY)/(float)fRuler);
							if(cnp[ii]>max)
							{
								nFootMax = ii;
								max=cnp[ii];
							}

						}
						switch(nIn)
						{
						case 1:
							f=(nFootMax+nBeginY)/(float)fRuler;
							break;
						case 2:
							f=cnp[nFootMax];
							break;
						case 3:
							f=amount/ltp;
							break;
						default:
//							ASSERT(FALSE);
							k=0;
							break;
						}
						pInput->fOutputParam .line[i] =f;

					}
				}
				
			}
			else
			{
				for(i=0;i<pInput->nNumData ;i++)
					pInput->fOutputParam .line[i] = 0;
			}
			break;
		}
		if(k==0)
			pInput->fOutputParam.b=-1;
		else
			pInput->fOutputParam.b=l;
		pInput->fOutputParam.e=pInput->nNumData-1;
		break;
	case 35://reflast
		j=pInput->fInputParam[0].b;
		if( pInput->fInputParam[1].b>=0 && pInput->fInputParam[1].b<pInput->nNumData &&pInput->nNumData>0)
		{
			int nMove = pInput->fInputParam[1].line[ pInput->fInputParam[1].b];
			if(nMove<0) nMove = 0;
			if(nMove>= pInput->nNumData) nMove = pInput->nNumData-1;
			int nFoot = pInput->nNumData-1-nMove;
			if(nFoot<j) nFoot = j;
			for(i=j;i<pInput->nNumData ;i++)
				pInput->fOutputParam.line[i] = (pInput->fInputParam[0].line[nFoot]);
			pInput->fOutputParam.b=j;
		}
		break;
	case 36://refbegin
		j=pInput->fInputParam[0].b;
		if( pInput->fInputParam[1].b>=0 && pInput->fInputParam[1].b<pInput->nNumData &&pInput->nNumData>0)
		{
			int nMove = pInput->fInputParam[1].line[ pInput->fInputParam[1].b];
			if(nMove<0) nMove = 0;
			if(nMove>= pInput->nNumData) nMove = (pInput->nNumData-1);
			int nFoot = nMove;
			if(nFoot<j) nFoot = j;
			for(i=j;i<pInput->nNumData ;i++)
				pInput->fOutputParam.line[i] = (pInput->fInputParam[0].line[nFoot]);
			pInput->fOutputParam.b=j;
		}
		break;
	case 37://valueaverage
		j=pInput->fInputParam[0].b;
		if(pInput->fInputParam[1].b>j)
			j =pInput->fInputParam[1].b;
		if(pInput->fInputParam[2].b>j)
			j =pInput->fInputParam[2].b;
		if(pInput->nNumData>0)
		{
			//总成交量
			float vol = 0;
			for(i=j;i<pInput->nNumData ;i++)
				vol += (pInput->fInputParam[1].line[i]);
			vol *= pInput->fInputParam[2].line[j];

			//排序
			float* fPrice = new float[pInput->nNumData];
			memcpy(fPrice,pInput->fInputParam[0].line ,sizeof(float)*pInput->nNumData);
			float* fVol = new float[pInput->nNumData];
			memcpy(fVol,pInput->fInputParam[1].line ,sizeof(float)*pInput->nNumData);
			for(i=j;i<pInput->nNumData ;i++)
			{
				for(int k=j;k<pInput->nNumData-1-(i-j);k++)
				{
					if(fPrice[k]>fPrice[k+1]) 
					{
						float ff = fPrice[k];
						fPrice[k] = fPrice[k+1];
						fPrice[k+1] = ff;
						ff = fVol[k];
						fVol[k] = fVol[k+1];
						fVol[k+1] = ff;
					}
				}
			}

			//计算
			float fTot = 0;
			for(i=j;i<pInput->nNumData ;i++)
			{
				fTot+=fVol[i];
				if(fTot>=vol)
				{
					fTot = fPrice[i];
					break;
				}
			}
			delete[] fPrice;
			delete[] fVol;
			if(i!=pInput->nNumData)
			{
				for(i=j;i<pInput->nNumData ;i++)
				{
					pInput->fOutputParam.line[i] =fTot;
				}
				pInput->fOutputParam.b=j;
			}
		}
		break;
	case 46://drawicon
	case 48://drawtext
		j=pInput->fInputParam[0].b;
		if(j<pInput->fInputParam[1].b)
			j=pInput->fInputParam[1].b;
		if(j<pInput->fInputParam[2].b)
			j=pInput->fInputParam[2].b;
		{
			if(nFunc == 46)
			{
				CString s;
				s.Format("%d",(int)(pInput->fInputParam[2].line[j]));
				pInput->fOutputParam.s = s;
			}
			else
				pInput->fOutputParam.s = pInput->fInputParam[2].s;

			for(i=j;i<pInput->nNumData ;i++)
			{
				pInput->fOutputParam.line[i] = (pInput->fInputParam[1].line[i]);
				if(pInput->fInputParam[0].line[i]>0)
				{
					LooseValue val;
					val.nFoot = i;
					val.fVal = pInput->fInputParam[1].line[i];
					pInput->fOutputParam.looseArr.Add(val);
				}
			}
			if(nFunc == 46)
				pInput->fOutputParam.kind = ARRAY_BE::DrawIcon;
			else
				pInput->fOutputParam.kind = ARRAY_BE::StringData;
			pInput->fOutputParam.b=j;
		}
		break;
	case 47://drawline/zig
		j=pInput->fInputParam[0].b;
		if(j<pInput->fInputParam[1].b)
			j=pInput->fInputParam[1].b;
		if(j<pInput->fInputParam[2].b)
			j=pInput->fInputParam[2].b;
		if(j<pInput->fInputParam[3].b)
			j=pInput->fInputParam[3].b;
		if(j<pInput->fInputParam[4].b)
			j=pInput->fInputParam[4].b;
		{
			int np = 0;
			CString s;
			s.Format("%d",pInput->fInputParam[4].line[j]);
			pInput->fOutputParam.s = s;

			for(i=j;i<pInput->nNumData ;i++)
			{
				pInput->fOutputParam.line[i] =  max(pInput->fInputParam[1].line[i],pInput->fInputParam[3].line[i]);
				if(np ==0)
				{
					if( pInput->fInputParam[0].line[i]>0 )
					{
						LooseValue val;
						val.nFoot = i;
						val.fVal =(pInput->fInputParam[1].line[i]);
						pInput->fOutputParam.looseArr.Add(val);

						np=1;
					}
				}
				else
				{
					if( pInput->fInputParam[2].line[i]>0 )
					{
						LooseValue val;
						val.nFoot = i;
						val.fVal = pInput->fInputParam[3].line[i];
						pInput->fOutputParam.looseArr.Add(val);

						np=0;
					}
				}

			}
			pInput->fOutputParam.b=j;
			pInput->fOutputParam.kind = ARRAY_BE::DrawLine;
		}
		break;
	case 49://extdata
		j=pInput->fInputParam[0].b;
		if( pInput->fInputParam[0].line[j]>=0)// && pInput->fInputParam[1].b<pInput->nNumData &&pInput->nNumData>0)
		{
			if(pInput->pDat1 == 0 )
				break;
			CString s = XgnExtDataDlg::FormatStr(m_pReportData);

			pInput->fOutputParam.b=j;
			if(!XgnExtDataDlg::GetLineBE(pInput->pData ,pInput->nNumData,pInput->fOutputParam,pInput->fInputParam[0].line[j],pInput->pDat1 ))
				pInput->fOutputParam.b = -1;
		}
		break;
	case 50://fanref
		{
			j=pInput->fInputParam[0].b;
			if(pInput->fInputParam[1].b>j)
				j =pInput->fInputParam[1].b;
			if(pInput->fInputParam[2].b>j)
				j =pInput->fInputParam[2].b;
			if(pInput->fInputParam[3].b>j)
				j =pInput->fInputParam[3].b;

			int k = pInput->fInputParam[3].line[j];
			if(k<0) break;
			j += k;

			if(m_nCurrentFoot<j) break;
			if(m_nCurrentFoot>=pInput->nNumData) break;
			int k2 = m_nCurrentFoot-k;

			ARRAY_Right_Box::FanRect(pInput->fInputParam[0].line[k2],pInput->fInputParam[1].line[k2],pInput->fInputParam[2].line[k2], pInput->fOutputParam.lineRight);
//			for(i = j;j<pInput->nNumData ;i++)
			//{
			//}
			pInput->fOutputParam.b=j;
			pInput->fOutputParam.kind = ARRAY_BE::LineRt;
			//m_nCurrentFoot
			//ARRAY_Right_Box::FanRect(float fVal, float fLow, float fHigh, ARRAY_Right_Box &out)
		}
		break;
	case 51://fansum
		{
			j=pInput->fInputParam[0].b;
			if(pInput->fInputParam[1].b>j)
				j =pInput->fInputParam[1].b;

			int k = pInput->fInputParam[1].line[j];
			if(k<0) break;
			j += k;

			if(m_nCurrentFoot<j) break;
			if(m_nCurrentFoot>=pInput->nNumData) break;

			int k2 = m_nCurrentFoot-k+1;
			if(k<=0) k2 = j;

			for(i = k2 ;i<=m_nCurrentFoot ;i++)
			{
				ARRAY_Right_Box::FanRect(pInput->fInputParam[0].line[i],
					/*pInput->fInputParam[1].line[k2],pInput->fInputParam[2].line[k2],*/
					pInput->pData[i].low , pInput->pData[i].high , pInput->fOutputParam.lineRight);
			}
			pInput->fOutputParam.b=j;
			pInput->fOutputParam.kind = ARRAY_BE::LineRt;
		}
		break;
	case 52://fanwin
		{
			int k = pInput->fInputParam[1].line[j];
			if(k<0) break;
			j += k;

			int nPre = pInput->fInputParam[3].line[j];
			int nCurrentFoot = m_nCurrentFoot - nPre;
			if(nCurrentFoot<j) break;
			if(nCurrentFoot>=pInput->nNumData) break;

			int k2 = nCurrentFoot-k+1;
			if(k<=0) k2 = j;
			float f3 = pInput->fInputParam[2].line[j];

			ARRAY_Right_Box::FanRect(f3,
					pInput->pData[k2].low , pInput->pData[k2].high , pInput->fOutputParam.lineRight);
			for(i = k2+1 ;i<=nCurrentFoot ;i++)
			{
				//FanAllPrice(float fVal, float fInitTotal ,ARRAY_Right_Box &out)
				ARRAY_Right_Box::FanAllPrice(pInput->fInputParam[0].line[i],
					f3 ,pInput->fOutputParam.lineRight);
				ARRAY_Right_Box::FanRect(pInput->fInputParam[0].line[i],
					pInput->pData[i].low , pInput->pData[i].high , pInput->fOutputParam.lineRight);
			}
			pInput->fOutputParam.b=j;
			pInput->fOutputParam.kind = ARRAY_BE::LineRt;
		}
		break;
	case 53://polyline
		j=pInput->fInputParam[0].b;
		if(pInput->fInputParam[1].b>j)
			j =pInput->fInputParam[1].b;
		{
			for(i=j;i<pInput->nNumData ;i++)
			{
				pInput->fOutputParam.line[i] = (pInput->fInputParam[1].line[i]);
				if(pInput->fInputParam[0].line[i]>0)
				{
					LooseValue val;
					val.nFoot = i;
					val.fVal = pInput->fInputParam[1].line[i];
					pInput->fOutputParam.looseArr.Add(val);
				}
			}
			pInput->fOutputParam.kind = ARRAY_BE::DrawPolyLine;
			pInput->fOutputParam.b=j;
		}
		break;
/*在图形上绘制折线段。
用法:
POLYLINE(COND,PRICE),当COND条件满足时,以PRICE位置为顶点画折线连接。
例如：POLYILINE(HIGH>=HHV(HIGH,20),HIGH)表示在创20天新高点之间画折线。*/
	case 54://stickline
		{
			float width = pInput->fInputParam[3].line[j];
			int empty = pInput->fInputParam[4].line[j];
			empty%=10;
			CString ss ;
			ss.Format("%.2f %d",width,empty);
			if(!pInput->fOutputParam.lineWidth)  pInput->fOutputParam.lineWidth = new float[pInput->fInputParam[2].e+1];
			for(i=j;i<pInput->nNumData ;i++)
			{
				pInput->fOutputParam.line[i] = (pInput->fInputParam[2].line[i]);
				pInput->fOutputParam.lineWidth[i] = pInput->fInputParam[3].line[i];
				if(pInput->fInputParam[0].line[i]>0)
				{
					LooseValue val;
					val.nFoot = i;
					val.fVal = pInput->fInputParam[1].line[i];
					pInput->fOutputParam.looseArr.Add(val);
					TRACE("STICKLINE = %d\n",i);
				}
			}
			pInput->fOutputParam.s = ss;
			pInput->fOutputParam.kind = ARRAY_BE::DrawStickLine;
			pInput->fOutputParam.b=j;
		}
		break;
/*在图形上绘制柱线。
用法:
STICKLINE(COND,PRICE1,PRICE2,WIDTH,EMPTY),当COND条件满足时,在PRICE1和PRICE2位置之间画柱状线，宽度为WIDTH(10为标准间距),EMPTH不为0则画空心柱。
例如：STICKLINE(CLOSE>OPEN,CLOSE,OPEN,0.8,1)表示画K线中阳线的空心柱体部分。*/
	case 55://SetLineWidth
		{
/*			float width = pInput->fInputParam[1].line[j];
			float isVirt = pInput->fInputParam[2].line[j];
			pInput->fOutputParam = pInput->fInputParam[0];
			
			pInput->fOutputParam.nLineWidth = (int)width;
			pInput->fOutputParam.isVirtualLine  = isVirt>0.01?1:0;
			pInput->fOutputParam.b=j;*/
		}
		break;
	case 56://polylinevalue
		{
			int nFirst = 0;
			float fPre = 0;
			int nTime = (int) pInput->fInputParam[2].line[j];
			if(nTime<=0) break;

			for(i=j;i<pInput->nNumData ;i++)
			{
				pInput->fOutputParam.line[i] = (pInput->fInputParam[1].line[i]);
				if(pInput->fInputParam[0].line[i]>0)
				{
					if(nFirst < nTime)
					{
						nFirst++;
						if(nFirst == nTime)
							j = i+1;
						else 
							continue;
					}
					pInput->fOutputParam.line[i] = fPre;
					fPre =  pInput->fInputParam[1].line[i];
				}
				else
					pInput->fOutputParam.line[i] = fPre;
			}
			if(nFirst < nTime) break;
			pInput->fOutputParam.b=j;
		}
		break;
	case 57://polylinetime
		{
			int nFirst = 0;
			int nPre = 0;
			int nTime = (int) pInput->fInputParam[2].line[j];
			if(nTime<=0) break;

			CArray<int ,int&> intArr;
			for(i=j;i<pInput->nNumData ;i++)
			{
				pInput->fOutputParam.line[i] = (pInput->fInputParam[1].line[i]);
				nPre++;
				if(pInput->fInputParam[0].line[i]>0)
				{
					intArr.Add (i);

					if(nFirst < nTime)
					{
						nFirst++;
						if(nFirst == nTime)
							j = i+1;
						else 
							continue;
					}
					pInput->fOutputParam.line[i] = nPre;
					ASSERT(intArr.GetSize ()>=nTime);
					nPre =  i - intArr[intArr.GetSize () - nTime];
				}
				else
					pInput->fOutputParam.line[i] = nPre;
			}
			if(nFirst < nTime) break;
			pInput->fOutputParam.b=j;
		}
		break;
/*在图形上绘制折线段。
用法:
POLYLINE(COND,PRICE),当COND条件满足时,以PRICE位置为顶点画折线连接。
例如：POLYILINE(HIGH>=HHV(HIGH,20),HIGH)表示在创20天新高点之间画折线。*/
	case 58://between
		{
			for(i=j;i<pInput->nNumData ;i++)
			{
				if(pInput->fInputParam[0].line[i]>= pInput->fInputParam[1].line[i] && pInput->fInputParam[0].line[i]<=pInput->fInputParam[2].line[i]
					||pInput->fInputParam[0].line[i]>= pInput->fInputParam[2].line[i] && pInput->fInputParam[0].line[i]<=pInput->fInputParam[1].line[i])
					pInput->fOutputParam.line[i] = 1;
				else
					pInput->fOutputParam.line[i] = 0;
			}
			pInput->fOutputParam.b=j;
		}
		break;
	case 59://CEILING 向上舍入
		{
			for(i=j;i<pInput->nNumData ;i++)
			{
				pInput->fOutputParam.line[i] =(float) ceil(pInput->fInputParam[0].line[i]);
			}
			pInput->fOutputParam.b=j;
		}
		break;
	case 60://FILTER(X,N):X满足条件后，删除其后N周期内的数据置为0
		{
			int n = pInput->fInputParam[1].line[j];
			if(n<0) break;
			for(i=j;i<pInput->nNumData ;i++)
			{
				if(pInput->fInputParam[0].line[i]>0.01)
				{
					pInput->fOutputParam.line[i] = 1;
					i+=n;
				}
				else
					pInput->fOutputParam.line[i] = 0;
			}
			pInput->fOutputParam.b=j;
		}
		break;
	case 61://FLOOR 向下舍入
		{
			for(i=j;i<pInput->nNumData ;i++)
			{
				pInput->fOutputParam.line[i] =(float) floor(pInput->fInputParam[0].line[i]);
			}
			pInput->fOutputParam.b=j;
		}
		break;
	case 62://LONGCROSS(A,B,N)表示A在N周期内都小于B，本周期从下方向上穿过B时返回1，否则返回0
		{
			int n = pInput->fInputParam[1].line[j];
			if(n<1) break;
			for(i=j+n;i<pInput->nNumData ;i++)
			{
				bool b = false;
				for(int k=0;k<n;k++)
				{
					if(pInput->fInputParam[0].line[i-n+k]>=pInput->fInputParam[1].line[i-n+k])
					{
						b = true;
						break;
					}
				}
				if(!b)
				{
					if(pInput->fInputParam[0].line[i]>pInput->fInputParam[1].line[i])
						pInput->fOutputParam.line[i] =1;
					else
						pInput->fOutputParam.line[i] =0;
				}
				else
					pInput->fOutputParam.line[i] =0;
			}
			pInput->fOutputParam.b=j;
		}
		break;
	case 63://RANGE(A,B,C)表示A大于B同时小于C时返回1，否则返回0
		{
			for(i=j;i<pInput->nNumData ;i++)
			{
				if(pInput->fInputParam[0].line[i]> pInput->fInputParam[1].line[i] 
					&& pInput->fInputParam[0].line[i]<pInput->fInputParam[2].line[i])
					pInput->fOutputParam.line[i] = 1;
				else
					pInput->fOutputParam.line[i] = 0;
			}
			pInput->fOutputParam.b=j;
		}
		break;
	default:
		FuncInDllExt(pInput,  nFunc,  j);
		break;
	}
	pInput->fOutputParam.e=pInput->nNumData-1;
	if(pInput->fOutputParam.b<0 || pInput->fOutputParam.b>pInput->fOutputParam.e)
		nRtn = 800;
	return nRtn;
/*
获利盘比例。
用法:
44 . WINNER(CLOSE),表示以当前收市价卖出的获利盘比例，例如返回0.1表示10%获利盘；WINNER(10.5)表示10.5元价格的获利盘比例
该函数仅对日线分析周期有效
  
成本分布情况。
用法:
COST(10),表示10%获利盘的价格是多少，即有10%的持仓量在该价格以下，其余90%在该价格以上，为套牢盘
该函数仅对日线分析周期有效
在图形上绘制小图标。
用法:
DRAWICON(COND,PRICE,TYPE),当COND条件满足时,在PRICE位置画TYPE号图标。
例如：DRAWICON(CLOSE>OPEN,LOW,1)表示当收阳时在最低价位置画1号图标。	
	
在图形上绘制直线段。
用法:
DRAWLINE(COND1,PRICE1,COND2,PRICE2,EXPAND),当COND1条件满足时,在PRICE1位置画直线起点，当COND2条件满足时,在PRICE2位置画直线终点，EXPAND为延长类型。
例如：DRAWILINE(HIGH>=HHV(HIGH,20),HIGH,LOW<=LLV(LOW,20),LOW,1)表示在创20天新高与创20天新低之间画直线并且向右延长。

在图形上显示文字。
用法:
DRAWTEXT(COND,PRICE,TEXT),当COND条件满足时,在PRICE位置书写文字TEXT。
例如：DRAWTEXT(CLOSE/OPEN>1.08,LOW,'大阳线')表示当日涨幅大于8%时在最低价位置显示'大阳线'字样。  
	*//*
FORCAST		线性回归预测值。
用法:
FORCAST(X,N)为X的N周期线性回归预测值
例如:FORCAST(CLOSE,10)表示求10周期线性回归预测本周期收盘价

SLOPE	线性回归斜率。
用法:
SLOPE(X,N)为X的N周期线性回归线的斜率
例如:SLOPE(CLOSE,10)表示求10周期线性回归线的斜率

VOLUNIT	每手股数。
用法:
VOLUNIT，返回每手股数，对于股票值为100，债券为1000
注意：该函数返回常数

*/
}

int CFormularComputeParent::GetFoot(INPUT_INFO *pInput, int nParam)
{
	int i=(int)(pInput->fInputParam[nParam].line[pInput->fInputParam[nParam].b]+0.5);
	if(i<198001||i>203701)
		return -1;
	int j=pInput->fInputParam[nParam+1].line[pInput->fInputParam[nParam+1].b];
	if(j/10000>31||j%100>=60||j/100%100>24||j<0)
		return -1;

	CTime tm(i/100,i%100,j/10000,j/100%100,j%100,0);
	time_t tmt = tm.GetTime ();
	bool bAdd;
	bool bDay=true;
	if(pInput->klineType <DAY_KLINE || pInput->klineType==HS_KLINE || pInput->klineType==MIN1_KLINE)
		bDay = false;

	int nRtn = CTaiKlineFileKLine::LookTwoPath((time_t&)tmt ,pInput->pData , pInput->nNumData , bAdd,bDay);
	//int nRtn = CTaiKlineFileKLine::LookTwoPath((long&)tmt ,pInput->pData , pInput->nNumData , bAdd,bDay);

	if(nRtn==-1&&bAdd==true)
		nRtn = 0;
	return nRtn;

}

int CFormularComputeParent::FuncZigGroup(INPUT_INFO *pInput, void* pArray)
{
	int i,j,k,l,m,nMaxFoot,nMinFoot,nRtn=0;
	float f,fTemp,max,min,fOld1,fOld2;
	float* fp;

	CArray<ValueFoot,ValueFoot&>* pArr=(CArray<ValueFoot,ValueFoot&>*)pArray;

	j=pInput->fInputParam[0].b;
	fp=pInput->fInputParam[0].line ;
	fTemp=pInput->fInputParam[1].line[pInput->fInputParam[1].b]/(float)100.0;
	l=0;//0 is down ; 1 is up
	m=j;//first point
	ValueFoot valueFo;
	valueFo.fValue = fp[j];
	valueFo.nFoot  = j;
	pArr->Add(valueFo);
	max = fp[j];
	min = fp[j];
	fOld1=fp[j];//the last one
	fOld2=fp[j];//the last second one
	nMaxFoot = j;
	nMinFoot = j;

	//lmb add 20030326
	for(i=j+1;i<pInput->nNumData ;i++)
	{
		//if max > 5%,
		if(fOld1==0||fOld2==0)
			break;
		//peak
		if((max-fp[i])/fp[i] >fTemp && fp[i]<max && (l==0 || l==2))
		{
			valueFo.fValue = max;//fp[i-1];//max
			valueFo.nFoot  = nMaxFoot;
			pArr->Add(valueFo);

			max = fp[i];
			min = fp[i];
			nMaxFoot = i;
			nMinFoot = i;
			l=1;

		}

		//trough
		if((fp[i]-min)/fp[i] >fTemp && fp[i]>min&& (l==0 || l==1))
		{
			valueFo.fValue = min;//fp[i-1];//max
			valueFo.nFoot  = nMinFoot;
			pArr->Add(valueFo);

			max = fp[i];
			min = fp[i];
			nMaxFoot = i;
			nMinFoot = i;
			l=2;

		}
		if(fp[i]>max)
		{
			nMaxFoot = i;
			max= fp[i];
		}
		if(fp[i]<min)
		{
			nMinFoot = i;
			min= fp[i];
		}

	}

	//the last to add
	if(pArr->GetSize ()-1>=0)
	{
		valueFo=pArr->GetAt(pArr->GetSize ()-1);
		if(valueFo.nFoot <pInput->nNumData-1)
		{
			valueFo.fValue = fp[pInput->nNumData-1];
			valueFo.nFoot =pInput->nNumData-1;
			pArr->Add(valueFo);
		}
	}
	pInput->fOutputParam.b=j;
/*	for(i=j+1;i<pInput->nNumData ;i++)
	{
		//if max > 5%,
		if(fOld1==0||fOld2==0)
			break;
		//peak
		if((max-fOld1)/fOld1 >fTemp && fp[i]<max)
		{
			valueFo.fValue = fp[i-1];//max
			valueFo.nFoot  = i-1;
			if(fOld1>fOld2 && pArr->GetSize ()>=2)
			{
				pArr->SetAt(pArr->GetSize ()-1,valueFo);
			}
			else
			{
				pArr->Add(valueFo);
			}
			fOld1 = fp[i-1];
			if(pArr->GetSize ()-2>=0)
				valueFo=pArr->GetAt(pArr->GetSize ()-2);
			fOld2 = valueFo.fValue ;
			max = fp[i-1];
			min = fp[i-1];

		}

		//trough
		if((fOld1-min)/fOld1 >fTemp && fp[i]>min)
		{
			valueFo.fValue = fp[i-1];//min
			valueFo.nFoot  = i-1;
			if(fOld1<fOld2 && pArr->GetSize ()>=2)
			{
				pArr->SetAt(pArr->GetSize ()-1,valueFo);
			}
			else
			{
				pArr->Add(valueFo);
			}
			fOld1 = fp[i-1];
			if(pArr->GetSize ()-2>=0)
				valueFo = pArr->GetAt(pArr->GetSize ()-2);
			fOld2 = valueFo.fValue ;
			max = fp[i-1];
			min = fp[i-1];
		}
		if(fp[i]>max)
			max= fp[i];
		if(fp[i]<min)
			min= fp[i];

	}

	//the last to add
	if(pArr->GetSize ()-1>=0)
	{
		valueFo=pArr->GetAt(pArr->GetSize ()-1);
		if(valueFo.nFoot <pInput->nNumData-1)
		{
			ValueFoot valueFo2;
			bool bAdd=true;
			if(pArr->GetSize ()>=2)
			{
				valueFo2=pArr->GetAt(pArr->GetSize ()-2);
				if( fp[pInput->nNumData-1]>valueFo.fValue )
				{
					if(valueFo.fValue >valueFo2.fValue )
						bAdd = false;
				}
				else
				{
					if(valueFo.fValue <valueFo2.fValue )
						bAdd = false;
				}
			}
			valueFo.fValue = fp[pInput->nNumData-1];
			valueFo.nFoot =pInput->nNumData-1;
			if(bAdd == true)
				pArr->Add(valueFo);
			else
				pArr->SetAt(pArr->GetSize ()-1,valueFo);
		}
	}
	pInput->fOutputParam.b=j;*/
/*	j=pInput->fInputParam[0].b;
	k=pInput->fInputParam[0].line[j];
	f=0;
	fTemp=pInput->fInputParam[1].line[pInput->fInputParam[0].b]/(float)100.0;
	l=0;//0 is down ; 1 is up
	m=j;//first point
	for(i=j;i<pInput->nNumData ;i++)
	{
		float f1;
		switch(k)
		{
		case 0://open
			f1=(pInput->pData[i].open) ;
			break;
		case 1://high
			f1=(pInput->pData[i].high) ;
			break;
		case 2://low
			f1=(pInput->pData[i].low) ;
			break;
		case 3://close
			f1=(pInput->pData[i].close) ;
			break;
		}
		if(i<=j+1)
		{
			if(i==j)
				pInput->fOutputParam.line[i]=f1;
			else if(f>f1)
				l=0;
			else
				l=1;
			f=f1;//fTemp=f1;
			continue;
		}


		if(f!=0)
		{
			if((f1-f)/f>fTemp && l==0)
			{
				pInput->fOutputParam .line[i-1] = f;
				l=1;
			}
			else if((f1-f)/f<-fTemp && l==1)
			{
				pInput->fOutputParam .line[i-1] = f;
				l=0;
			}
			else
				pInput->fOutputParam .line[i-1] = 0;
		}
		else
			pInput->fOutputParam .line[i-1] = 0;
		if(pInput->nNumData-1==i)
			pInput->fOutputParam.line[pInput->nNumData-1]=f1;

		//to compute mid value
		if(nGroup==0)
			if(pInput->fOutputParam .line[i-1]!=0 && (i-1-m)!=0)
			{
				for(int ii=m+1;ii<i-1;ii++)
				{
					pInput->fOutputParam .line[ii]=(pInput->fOutputParam .line[i-1]-pInput->fOutputParam .line[m])
						*(ii-i+1)/(i-1-m)+pInput->fOutputParam .line[i-1];
				}
			}
		//end
		if(pInput->fOutputParam .line[i-1]!=0)
			m = i-1;

		f=f1;
	}

	pInput->fOutputParam.b=j;
	pInput->fOutputParam.e=pInput->nNumData-1;*/
	return 0;
}

int CFormularComputeParent::RelolveNnet(int cl)
{//
	return 1;
}

int CFormularComputeParent::GetBeginFoot(ARRAY_BE &ln2)
{
	int ret =ln2.b;
	if(ret<0) return -1;
	int nFoot = (int)ln2.line[ret];
	ret =(int)ln2.line[ret];
	for(int i=ln2.b;i<=ln2.e;i++)
	{
		if((int)ln2.line[i]>ret) 
			ret = (int)ln2.line[i];
	}
	if(ret == nFoot) return ret;

	if(ret<1) ret = 1;

	return ret;
}

int CFormularComputeParent::GetIndexPoint(CString sName, float &nPoint)
{
	int fl=1;
	for(int i=0;i<m_RlineNum;i++)
	{
		CString s1 = m_RlineName[i];
		s1.MakeLower ();
		CString s2 = sName;
		s2.MakeLower ();
		if(s1==s2)
		{
			fl=0;
			int bl = m_Rline[i].b;
			int be = m_Rline[i].e;
			if(bl>be||bl<0||be<0||be>=m_MaxPoint)
				return 1;

			nPoint = m_Rline[i].line[be];
			break;
		}
	}
	return fl;

}

int CFormularComputeParent::CalcMin5InDay(CFormularContent *pIndex,int nParamInTable,CString linename,int tu)
{
	return 1;
}

int CFormularComputeParent::GetStockKind(char *strLabel)
{
	 CReportData *pStockData;
	 int stkKind = CSharesInformation::GetStockKind(SH_MARKET_EX,strLabel);
	 bool bFind = false;
	 {
		 if(stkKind>=0)
			 if(CMainFrame::m_pDoc ->m_sharesInformation .Lookup (strLabel,pStockData,stkKind))
				 bFind = true;
	 }
	 if(bFind == false)
	 {
		 stkKind = CSharesInformation::GetStockKind(SZ_MARKET_EX,strLabel);
		 if(stkKind>=0)
			 if(CMainFrame::m_pDoc->m_sharesInformation .Lookup (strLabel,pStockData,stkKind))
				 bFind = true;
	 }
	 return stkKind;

}

void CFormularComputeParent::FuncZigPre(INPUT_INFO *pInput, int nFunc)
{
	if(nFunc>20)
	{
		int j=pInput->fInputParam[0].b;
		int f2 = (int)(pInput->fInputParam[0].line[j]);
		if(f2<0)f2 = 0;
		if(f2>3)f2 = 3;

		for(int i=j;i<pInput->nNumData ;i++)
		{
			switch(f2)
			{
			case 0:
				pInput->fInputParam[0].line[i] = pInput->pData[i].open ;
				break;
			case 1:
				pInput->fInputParam[0].line[i] = pInput->pData[i].high ;
				break;
			case 2:
				pInput->fInputParam[0].line[i] = pInput->pData[i].low ;
				break;
			case 3:
				pInput->fInputParam[0].line[i] = pInput->pData[i].close ;
				break;
			}
		}
	}
}

void CFormularComputeParent::FanEach(ARRAY_BE &lineIn, int nFoot, ARRAY_BE &lineOut)
{
}

bool CFormularComputeParent::FanEach(CFormularContent * pJishu)
{
	CString formal = pJishu->fomular ;
	formal.MakeLower();
	Kline kline;
	CFormularComputeParent equation(0,0,&kline,formal);
	equation.Devide ();
	bool b = equation.m_bRightbox ;
	pJishu->bRightBox = 0;
	if(b)
		pJishu->bRightBox = 1;
	return b;
}

int CFormularComputeParent::FuncInDllExt(INPUT_INFO *pInput, int nFunc, int j)
{
	return 0;
}
