// DATA_INFO_SZ.cpp : implementation file
//

#include "stdafx.h"
#include "WH.h"
#include "INFORMATION_SZ.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDATA_INFO_SZ

IMPLEMENT_DYNAMIC(CDATA_INFO_SZ, CDaoRecordset)

CDATA_INFO_SZ::CDATA_INFO_SZ(CDaoDatabase* pdb)
	: CDaoRecordset(pdb)
{
	//{{AFX_FIELD_INIT(CDATA_INFO_SZ)
	m_ID = 0;
	m_column1 = _T("");
	m_column2 = _T("");
	m_column3 = _T("");
	m_column4 = 0.0;
	m_column5 = 0.0;
	m_column6 = 0.0;
	m_column7 = 0.0;
	m_B_ = 0.0;
	m_H_ = 0.0;
	m___A_ = 0.0;
	m_column8 = 0.0;
	m_A2___ = 0.0;
	m_column9 = 0.0;
	m_column10 = 0.0;
	m_column11 = 0.0;
	m_column12 = 0.0;
	m_column13 = 0.0;
	m_column14 = 0.0;
	m_column15 = 0.0;
	m_column16 = 0.0;
	m_column17 = 0.0;
	m_column18 = 0.0;
	m_column19 = 0.0;
	m_column20 = 0.0;
	m_column21 = 0.0;
	m_column22 = 0.0;
	m_column23 = 0.0;
	m_column24 = 0.0;
	m_column25 = 0.0;
	m_column26 = 0.0;
	m_column27 = 0.0;
	m_column28 = 0.0;
	m_column29 = 0.0;
	m_column30 = 0.0;
	m_column31 = 0.0;
	m_column32 = 0.0;
	m_column33 = 0.0;
	m_column34 = 0.0;
	m_column35 = 0.0;
	m_column36 = 0.0;
	m_nFields = 41;
	//}}AFX_FIELD_INIT
	m_nDefaultType = dbOpenDynaset;
}


CString CDATA_INFO_SZ::GetDefaultDBName()
{
	return _T("��������.mdb");
}

CString CDATA_INFO_SZ::GetDefaultSQL()
{
	return _T("[���ڻ�������]");
}

void CDATA_INFO_SZ::DoFieldExchange(CDaoFieldExchange* pFX)
{
	//{{AFX_FIELD_MAP(CDATA_INFO_SZ)
	pFX->SetFieldType(CDaoFieldExchange::outputColumn);
	DFX_Long(pFX, _T("[ID]"), m_ID);
	DFX_Text(pFX, _T("[����]"), m_column1);
	DFX_Text(pFX, _T("[����]"), m_column2);
	DFX_Text(pFX, _T("[��������]"), m_column3);
	DFX_Double(pFX, _T("[�ܹɱ�(���)]"), m_column4);
	DFX_Double(pFX, _T("[���ҹ�]"), m_column5);
	DFX_Double(pFX, _T("[�����˷��˹�]"), m_column6);
	DFX_Double(pFX, _T("[���˹�]"), m_column7);
	DFX_Double(pFX, _T("[B��]"), m_B_);
	DFX_Double(pFX, _T("[H��]"), m_H_);
	DFX_Double(pFX, _T("[��ͨA��]"), m___A_);
	DFX_Double(pFX, _T("[ְ����]"), m_column8);
	DFX_Double(pFX, _T("[A2ת���]"), m_A2___);
	DFX_Double(pFX, _T("[���ʲ�(ǧԪ)]"), m_column9);
	DFX_Double(pFX, _T("[�����ʲ�]"), m_column10);
	DFX_Double(pFX, _T("[�̶��ʲ�]"), m_column11);
	DFX_Double(pFX, _T("[�����ʲ�]"), m_column12);
	DFX_Double(pFX, _T("[����Ͷ��]"), m_column13);
	DFX_Double(pFX, _T("[������ծ]"), m_column14);
	DFX_Double(pFX, _T("[���ڸ�ծ]"), m_column15);
	DFX_Double(pFX, _T("[�ʱ�������]"), m_column16);
	DFX_Double(pFX, _T("[ÿ�ɹ�����]"), m_column17);
	DFX_Double(pFX, _T("[�ɶ�Ȩ��]"), m_column18);
	DFX_Double(pFX, _T("[��Ӫ����]"), m_column19);
	DFX_Double(pFX, _T("[��Ӫ����]"), m_column20);
	DFX_Double(pFX, _T("[��������]"), m_column21);
	DFX_Double(pFX, _T("[Ӫҵ����]"), m_column22);
	DFX_Double(pFX, _T("[Ͷ������]"), m_column23);
	DFX_Double(pFX, _T("[��������]"), m_column24);
	DFX_Double(pFX, _T("[Ӫҵ����֧]"), m_column25);
	DFX_Double(pFX, _T("[�����������]"), m_column26);
	DFX_Double(pFX, _T("[�����ܶ�]"), m_column27);
	DFX_Double(pFX, _T("[˰������]"), m_column28);
	DFX_Double(pFX, _T("[������]"), m_column29);
	DFX_Double(pFX, _T("[δ��������]"), m_column30);
	DFX_Double(pFX, _T("[ÿ��δ����]"), m_column31);
	DFX_Double(pFX, _T("[ÿ������]"), m_column32);
	DFX_Double(pFX, _T("[ÿ�ɾ��ʲ�]"), m_column33);
	DFX_Double(pFX, _T("[����ÿ�ɾ���]"), m_column34);
	DFX_Double(pFX, _T("[�ɶ�Ȩ���]"), m_column35);
	DFX_Double(pFX, _T("[����������]"), m_column36);
	//}}AFX_FIELD_MAP
}

/////////////////////////////////////////////////////////////////////////////
// CDATA_INFO_SZ diagnostics

#ifdef _DEBUG
void CDATA_INFO_SZ::AssertValid() const
{
	CDaoRecordset::AssertValid();
}

void CDATA_INFO_SZ::Dump(CDumpContext& dc) const
{
	CDaoRecordset::Dump(dc);
}
#endif //_DEBUG
