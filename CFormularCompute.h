// CFormularCompute.h: interface for the CFormularCompute class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CFORMULARCOMPUTEPARENT_H__D8DB589D_6CB1_4A42_B247_8CF240546DAF__INCLUDED_)
#define AFX_CFORMULARCOMPUTEPARENT_H__D8DB589D_6CB1_4A42_B247_8CF240546DAF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "CFormularComputeParent.h"
class CFormularContent;
class CFormularCompute  : public CFormularComputeParent
{
public:
	virtual int FuncInDllExt(INPUT_INFO *pInput, int nFunc, int j);
	CFormularCompute();
	virtual ~CFormularCompute();
	CFormularCompute(int pmin,int pmax,Kline* pp,CString& str); 
	CFormularCompute(int pmin,int pmax,Kline* pp,CFormularContent* pIndex,CString symbol,int stkKind,CBuySellList* pBuySellList,int nKindKline,bool bAddBuySell = true);
	friend int ComputeFormu2(CFormularCompute*& pEqution,INPUT_PARAM& input);// 计算所指定指标的数值，need to delete pEqution
	
protected:
	static void InitStatic2();
};

#endif // !defined(AFX_CFORMULARCOMPUTEPARENT_H__D8DB589D_6CB1_4A42_B247_8CF240546DAF__INCLUDED_)
