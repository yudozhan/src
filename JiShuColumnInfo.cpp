// JiShuColumnInfo.cpp : implementation file
//

#include "stdafx.h"
#include "Mainfrm.h"
#include "CTaiShanDoc.h"
#include "WH.h"
#include "JiShuColumnInfo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define SYSTEMHEAD     1000

/////////////////////////////////////////////////////////////////////////////
// JiShuColumnInfo

TechniqueColumnInfo::TechniqueColumnInfo(CTaiShanDoc *pDoc)
{
	m_pDoc=pDoc;
/*	for(int i=0;i<MAXJISHUCOUNT;i++)
	{
		pDoc->m_SystemInitData.TechniqueColumn[i].IsUse =0;
        pDoc->m_SystemInitData.TechniqueColumn[i].nID =i+9000;
		memset(&pDoc->m_SystemInitData.TechniqueColumn[i].nIndex,0,sizeof(Index_Technique));
	}
	*/
}
TechniqueColumnInfo::~TechniqueColumnInfo()
{
}

/////////////////////////////////////////////////////////////////////////////
// TechniqueColumnInfo message handlers

UINT TechniqueColumnInfo::FindSolt(PTECHNIQUE_COLUMN &pTechniqueIndex)
{
	for(int i=0;i<MAXJISHUCOUNT;i++)
	{
		if(!m_pDoc->m_SystemInitData.TechniqueColumn[i].IsUse)
		{
			pTechniqueIndex=&m_pDoc->m_SystemInitData.TechniqueColumn[i];
			return m_pDoc->m_SystemInitData.TechniqueColumn[i].nID; 
		}
	}
	return -1; 
}
BOOL TechniqueColumnInfo::GetTechniqueFromId(UINT nId,PTECHNIQUE_COLUMN &pTechniqueIndex)
{
	for(int i=0;i<MAXJISHUCOUNT;i++)
	{
		if(m_pDoc->m_SystemInitData.TechniqueColumn[i].nID ==nId)
		{
			pTechniqueIndex=&m_pDoc->m_SystemInitData.TechniqueColumn[i];
			return TRUE; 
		}
	}
	pTechniqueIndex=NULL;
	return FALSE; 
}
BOOL TechniqueColumnInfo::GetTechniqueFromIndexName(CString IndexName ,PTECHNIQUE_COLUMN &pTechniqueIndex)
{
	for(int i=0;i<MAXJISHUCOUNT;i++)
	{
		if(IndexName==m_pDoc->m_SystemInitData.TechniqueColumn[i].nIndex.sIndex_name)
		{
			pTechniqueIndex=&m_pDoc->m_SystemInitData.TechniqueColumn[i];
			return TRUE; 
		}
	}
	pTechniqueIndex=NULL;
	return FALSE; 
}
void TechniqueColumnInfo::SetColumnNotUse(UINT nId)
{ 
	for(int i=0;i<MAXJISHUCOUNT;i++)
	{
		if(m_pDoc->m_SystemInitData.TechniqueColumn[i].nID ==nId)
		{
			m_pDoc->m_SystemInitData.TechniqueColumn[i].IsUse=FALSE ; 
		}
	}
}
void TechniqueColumnInfo::SetAllColumnNotUse()
{
	for(int i=0;i<MAXJISHUCOUNT;i++)
	{
		m_pDoc->m_SystemInitData.TechniqueColumn[i].IsUse=FALSE ; 
	}
}

