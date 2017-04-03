// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__9DB8C72F_4614_469B_8067_B933060CEB94__INCLUDED_)
#define AFX_STDAFX_H__9DB8C72F_4614_469B_8067_B933060CEB94__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#pragma warning(disable:4786)
#pragma warning(disable:4652)
#endif // _MSC_VER > 1000

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxdisp.h>        // MFC Automation classes
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <afxsock.h>		// MFC socket extensions
#include <afxtempl.h>		// Tool Tip용 ViboX 추가.
#include <afxdlgs.h>		// Tips Ware용...
#include "afxinet.h"

#include "./lib/Criticalsection.h"
#include "./lib/MultiThreadSync.h"
#include "./lib/CircularQueue.h"

#include "afxmt.h"		// Mutex header
#include <afxcmn.h>			// MFC support for Windows Common Controls
/* ****************************************************************************** */
/* 대화 상자에 대한 OK, Cancel 버튼 생성 함수                                     */
/* ****************************************************************************** */
#include "tw_step.h"  // 대화 상자 생성 클래스 추가 
#include "resource.h"       // main symbols
//#include "DataAPI.h"
#include "ssdllmfc.h"
#include "GridControlAlg.h"
#include "Public_Function.h"
#include "CtlBd_Library.h"

#include "PIODA.H"

#include "ServerSocket.h"
#include "Variable.h"


//extern void DoEvents();
extern CButton *CreateOkButton(int parm_resource_id, CWnd *parm_parent);
extern CButton *CreateCancelButton(int parm_resource_id, CWnd *parm_parent);
extern CButton *CreateSaveOkButton(int parm_resource_id, CWnd *parm_parent);
extern CButton *CreateExitButton(int parm_resource_id, CWnd *parm_parent);

//extern CFileManagement				FileAccess;
extern CPublic_Function				Func;
//extern CMainFrame					cmain;
/* ****************************************************************************** */

/* ****************************************************************************** */
/* TCP/IP 통신 관련 클래스 추가                                                   */
/* ****************************************************************************** */
//#include <winsock2.h>
//#include <mswsock.h>
//#pragma comment(lib, "ws2_32")
/* ****************************************************************************** */



//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__9DB8C72F_4614_469B_8067_B933060CEB94__INCLUDED_)
