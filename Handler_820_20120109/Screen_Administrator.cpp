// Screen_Administrator.cpp : implementation file
//

#include "stdafx.h"
#include "handler.h"
#include "Screen_Administrator.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CScreen_Administrator

IMPLEMENT_DYNCREATE(CScreen_Administrator, CFormView)

CScreen_Administrator::CScreen_Administrator()
	: CFormView(CScreen_Administrator::IDD)
{
	//{{AFX_DATA_INIT(CScreen_Administrator)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CScreen_Administrator::~CScreen_Administrator()
{
}

void CScreen_Administrator::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CScreen_Administrator)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CScreen_Administrator, CFormView)
	//{{AFX_MSG_MAP(CScreen_Administrator)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CScreen_Administrator diagnostics

#ifdef _DEBUG
void CScreen_Administrator::AssertValid() const
{
	CFormView::AssertValid();
}

void CScreen_Administrator::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CScreen_Administrator message handlers
