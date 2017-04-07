// HANDLERDoc.cpp : implementation of the CHANDLERDoc class
//

#include "stdafx.h"
#include "HANDLER.h"

#include "HANDLERDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CHANDLERDoc

IMPLEMENT_DYNCREATE(CHANDLERDoc, CDocument)

BEGIN_MESSAGE_MAP(CHANDLERDoc, CDocument)
	//{{AFX_MSG_MAP(CHANDLERDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHANDLERDoc construction/destruction

CHANDLERDoc::CHANDLERDoc()
{
	// TODO: add one-time construction code here

}

CHANDLERDoc::~CHANDLERDoc()
{
}

BOOL CHANDLERDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CHANDLERDoc serialization

void CHANDLERDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

/////////////////////////////////////////////////////////////////////////////
// CHANDLERDoc diagnostics

#ifdef _DEBUG
void CHANDLERDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CHANDLERDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CHANDLERDoc commands
