// Dialog_Skip.cpp : implementation file
//

#include "stdafx.h"
#include "handler.h"
#include "Dialog_Skip.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDialog_Skip dialog


CDialog_Skip::CDialog_Skip(CWnd* pParent /*=NULL*/)
	: CDialog(CDialog_Skip::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDialog_Skip)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDialog_Skip::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialog_Skip)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDialog_Skip, CDialog)
	//{{AFX_MSG_MAP(CDialog_Skip)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialog_Skip message handlers

BOOL CDialog_Skip::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
