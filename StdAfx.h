// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__61A31CEB_9707_11D1_ACAB_0000E823DBFD__INCLUDED_)
#define AFX_STDAFX_H__61A31CEB_9707_11D1_ACAB_0000E823DBFD__INCLUDED_
            
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define   WINVER   0x0502   //Ö§³ÖWin2003


//#define OEM   01
#define PRO 03
// #define TEST 04

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxdisp.h>        // MFC Automation classes
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#include <afxtempl.h>
#include <wininet.h>


#include "resource.h"
#include "STKDRV.H"
#include "stockdrv.h"
#include "defs.h"
#include "StructTaiShares.h"
#include "CSharesBaseInfo.h"
#include "CSharesInformation.h"
#include "CSharesCompute.h"
#include "StructKlineView.h"
#include "ManagerStockTypeData.h"
#include "JiShuColumnInfo.h"
#include "CSharesBigBillData.h"
#include "CFormularCompute.h"
#include <afxdao.h>
#include "Funclist.h"

#include "HtmlHelp.h"

#include "ClientTransmitManageWnd.h"
#include "global.h"

#include <XTToolkitPro.h>   // Codejock Software Components

#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT


#include "BtnST.h"

const char g_realtime[64] = "Data\\stockdata.dat";
const char g_caiwudata[64] = "Data\\finacial.dat";
const char g_baseinfo[64] = "Data\\companybasedata.dat";
const char g_stocktypeinfo[64] = "Data\\blockinfo.dat";
const char g_blockdir[64] = "block\\";
const char g_hzpy[64] = "Data\\languagedata.dat";
const char g_rate[64] = "Data\\stockrate.dat";
const char g_xtsx_hz[64] = "Data\\systemsetting.dat";
const char g_kxzh_hz[64] = "Data\\klinegroup.dat";
const char g_tixg_hz[64] = "Data\\screendata.dat";
const char g_zbgs_hz[64] = "Data\\indexdata.dat";

const char g_daysh[64] = "Data\\sh\\daykline.dat";
const char g_daysz[64] = "Data\\sz\\daykline.dat";
const char g_minutesh[64] = "Data\\sh\\minutekline.dat";
const char g_minutesz[64] = "Data\\sz\\minutekline.dat";
const char g_buysellsh[64] = "Data\\sz\\tick.dat";
const char g_buysellsz[64] = "Data\\sh\\tick.dat";

const CString g_sF10sh = "Data\\sh\\f10\\";
const CString g_sF10sz = "Data\\sz\\f10\\";



//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__61A31CEB_9707_11D1_ACAB_0000E823DBFD__INCLUDED_)
