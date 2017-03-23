// Dialog_Manual_Move.cpp : implementation file
//

#include "stdafx.h"
#include "handler.h"
#include "Dialog_Manual_Move.h"
#include "Public_Function.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDialog_Manual_Move dialog

CDialog_Manual_Move::CDialog_Manual_Move(CWnd* pParent /*=NULL*/)
	: CDialog(CDialog_Manual_Move::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDialog_Manual_Move)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDialog_Manual_Move::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialog_Manual_Move)
	DDX_Control(pDX, IDC_BTN_EXIT, m_btn_exit);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDialog_Manual_Move, CDialog)
	//{{AFX_MSG_MAP(CDialog_Manual_Move)
	ON_BN_CLICKED(IDC_BTN_EXIT, OnBtnExit)
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialog_Manual_Move message handlers

BOOL CDialog_Manual_Move::Create()
{
	return CDialog::Create( CDialog_Manual_Move::IDD );
}


BOOL CDialog_Manual_Move::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	CRect rect_area;
	
	m_p_font = NULL;
	m_p_font = new CFont;
	m_p_font->CreateFont(14,0,0,0,900,0,0,0,0,0,0,ANTIALIASED_QUALITY,0,"MS Sans Serif");
	
	GetWindowRect(&rect_area);
	MoveWindow(m_cp_coord.x, m_cp_coord.y, rect_area.Width(), rect_area.Height());

	Init_Button();
	Init_Group();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDialog_Manual_Move::OnBtnExit() 
{
	// TODO: Add your control notification handler code here
	PostNcDestroy();
}

void CDialog_Manual_Move::Init_Button()
{
	short	shBtnColor = 30;
	
	m_btn_exit.SetFont(m_p_font);
	m_btn_exit.SetIcon(IDI_EXIT);
	m_btn_exit.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);
	m_btn_exit.SetButtonColor(1);
	m_btn_exit.SetTooltipText("IO Dialog Exit");
}

void CDialog_Manual_Move::Init_Group()
{
	CSxLogFont main_font(15,FW_SEMIBOLD,false,"Arial");
	
}

void CDialog_Manual_Move::OnButtonControl(bool bflag)
{
	m_btn_exit.EnableWindow(bflag);
}



void CDialog_Manual_Move::PostNcDestroy() 
{
	// TODO: Add your specialized code here and/or call the base class
	if(m_p_font != NULL)
	{
		delete m_p_font;
		m_p_font = NULL;
	}
	
	delete this;
	Func.m_p_move = NULL;

	CDialog::PostNcDestroy();
}
