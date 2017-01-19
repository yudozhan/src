// BjChenBenSetting.cpp: implementation of the BjChenBenSetting class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "WH.h"
#include "BjChenBenSetting.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

BjChenBenSetting::BjChenBenSetting()
{
	m_colorLine = RGB(0,0,0);
	m_nDays = 0;
//	m_pData = NULL;

}

BjChenBenSetting::~BjChenBenSetting()
{

}
BjChenBenSetting& BjChenBenSetting::operator = (BjChenBenSetting & inObject)
{
	this->m_colorLine = inObject.m_colorLine ;
	this->m_nDays = inObject.m_nDays ;
//	this->m_pData = NULL;
	return *this;
};
