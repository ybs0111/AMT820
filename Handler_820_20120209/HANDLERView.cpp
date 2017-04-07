// HANDLERView.cpp : implementation of the CHANDLERView class
//

#include "stdafx.h"
#include "HANDLER.h"

#include "HANDLERDoc.h"
#include "HANDLERView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CHANDLERView
extern CHANDLERApp theApp;
IMPLEMENT_DYNCREATE(CHANDLERView, CFormView)

BEGIN_MESSAGE_MAP(CHANDLERView, CFormView)
	//{{AFX_MSG_MAP(CHANDLERView)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
	// Standard printing commands
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHANDLERView construction/destruction

CHANDLERView::CHANDLERView()
	: CFormView(CHANDLERView::IDD)
{
	//{{AFX_DATA_INIT(CHANDLERView)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// TODO: add construction code here
}

CHANDLERView::~CHANDLERView()
{
}

void CHANDLERView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CHANDLERView)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BOOL CHANDLERView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CFormView::PreCreateWindow(cs);
}

void CHANDLERView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	GetParentFrame()->RecalcLayout();
	ResizeParentToFit();

}

/////////////////////////////////////////////////////////////////////////////
// CHANDLERView printing

BOOL CHANDLERView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CHANDLERView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CHANDLERView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

void CHANDLERView::OnPrint(CDC* pDC, CPrintInfo* /*pInfo*/)
{
	// TODO: add customized printing code here
}

/////////////////////////////////////////////////////////////////////////////
// CHANDLERView diagnostics

#ifdef _DEBUG
void CHANDLERView::AssertValid() const
{
	CFormView::AssertValid();
}

void CHANDLERView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}

CHANDLERDoc* CHANDLERView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CHANDLERDoc)));
	return (CHANDLERDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CHANDLERView message handlers
