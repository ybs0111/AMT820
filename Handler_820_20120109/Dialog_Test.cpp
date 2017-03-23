// Dialog_Test.cpp : implementation file
//

#include "stdafx.h"
#include "handler.h"
#include "Dialog_Test.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDialog_Test dialog


CDialog_Test::CDialog_Test(CWnd* pParent /*=NULL*/)
	: CDialog(CDialog_Test::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDialog_Test)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDialog_Test::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialog_Test)
	DDX_Control(pDX, IDC_TAB1, m_test);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDialog_Test, CDialog)
	//{{AFX_MSG_MAP(CDialog_Test)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialog_Test message handlers

BOOL CDialog_Test::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
//	m_test.Init_Tab(this);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
