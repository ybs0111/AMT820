// Dialog_Color.cpp : implementation file
//

#include "stdafx.h"
#include "handler.h"
#include "Dialog_Color.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDialog_Color dialog


CDialog_Color::CDialog_Color(CWnd* pParent /*=NULL*/)
	: CDialog(CDialog_Color::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDialog_Color)
	//}}AFX_DATA_INIT
}


void CDialog_Color::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialog_Color)
	DDX_Control(pDX, IDC_BTN_O_TEXT2, m_btn_o_text1);
	DDX_Control(pDX, IDC_BTN_O_GRID2, m_btn_o_grid1);
	DDX_Control(pDX, IDC_BTN_I_TEXT2, m_btn_i_text1);
	DDX_Control(pDX, IDC_BTN_I_GRID2, m_btn_i_grid1);
	DDX_Control(pDX, IDOK, m_btn_ok);
	DDX_Control(pDX, IDC_BTN_O_TEXT, m_btn_o_text);
	DDX_Control(pDX, IDC_BTN_O_GRID, m_btn_o_grid);
	DDX_Control(pDX, IDC_BTN_I_TEXT, m_btn_i_text);
	DDX_Control(pDX, IDC_BTN_I_GRID, m_btn_i_grid);
	DDX_Control(pDX, IDC_GROUP_OUTPUT, m_group_output);
	DDX_Control(pDX, IDC_GROUP_INPUT, m_group_input);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDialog_Color, CDialog)
	//{{AFX_MSG_MAP(CDialog_Color)
	ON_BN_CLICKED(IDC_BTN_I_GRID, OnBtnIGrid)
	ON_BN_CLICKED(IDC_BTN_I_TEXT, OnBtnIText)
	ON_BN_CLICKED(IDC_BTN_O_GRID, OnBtnOGrid)
	ON_BN_CLICKED(IDC_BTN_O_TEXT, OnBtnOText)
	ON_BN_CLICKED(IDC_BTN_I_GRID2, OnBtnIGrid2)
	ON_BN_CLICKED(IDC_BTN_I_TEXT2, OnBtnIText2)
	ON_BN_CLICKED(IDC_BTN_O_GRID2, OnBtnOGrid2)
	ON_BN_CLICKED(IDC_BTN_O_TEXT2, OnBtnOText2)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialog_Color message handlers

BOOL CDialog_Color::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	Init_Group();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDialog_Color::OnBtnIGrid() 
{
	// TODO: Add your control notification handler code here
	CColorDialog dlg;
	COLORREF color;

	if(dlg.DoModal() == IDOK)
	{
		color = dlg.GetColor();
		st_work.n_grid_r[0][0] = GetRValue(color);
		st_work.n_grid_g[0][0] = GetGValue(color);
		st_work.n_grid_b[0][0] = GetBValue(color);
	}
}

void CDialog_Color::OnBtnIText() 
{
	// TODO: Add your control notification handler code here
	CColorDialog dlg;
	COLORREF color;

	if(dlg.DoModal() == IDOK)
	{
		color = dlg.GetColor();
		st_work.n_text_r[0][0] = GetRValue(color);
		st_work.n_text_g[0][0] = GetGValue(color);
		st_work.n_text_b[0][0] = GetBValue(color);
	}
}

void CDialog_Color::OnBtnOGrid() 
{
	// TODO: Add your control notification handler code here
	CColorDialog dlg;
	COLORREF color;

	if(dlg.DoModal() == IDOK)
	{
		color = dlg.GetColor();
		st_work.n_grid_r[1][0] = GetRValue(color);
		st_work.n_grid_g[1][0] = GetGValue(color);
		st_work.n_grid_b[1][0] = GetBValue(color);
	}
}

void CDialog_Color::OnBtnOText() 
{
	// TODO: Add your control notification handler code here
	CColorDialog dlg;
	COLORREF color;

	if(dlg.DoModal() == IDOK)
	{
		color = dlg.GetColor();
		st_work.n_text_r[1][0] = GetRValue(color);
		st_work.n_text_g[1][0] = GetGValue(color);
		st_work.n_text_b[1][0] = GetBValue(color);
	}
}

void CDialog_Color::OnBtnIGrid2() 
{
	// TODO: Add your control notification handler code here
	CColorDialog dlg;
	COLORREF color;
	
	if(dlg.DoModal() == IDOK)
	{
		color = dlg.GetColor();
		st_work.n_grid_r[0][1] = GetRValue(color);
		st_work.n_grid_g[0][1] = GetGValue(color);
		st_work.n_grid_b[0][1] = GetBValue(color);
	}
}

void CDialog_Color::OnBtnIText2() 
{
	// TODO: Add your control notification handler code here
	CColorDialog dlg;
	COLORREF color;
	
	if(dlg.DoModal() == IDOK)
	{
		color = dlg.GetColor();
		st_work.n_text_r[0][1] = GetRValue(color);
		st_work.n_text_g[0][1] = GetGValue(color);
		st_work.n_text_b[0][1] = GetBValue(color);
	}
}

void CDialog_Color::OnBtnOGrid2() 
{
	// TODO: Add your control notification handler code here
	CColorDialog dlg;
	COLORREF color;
	
	if(dlg.DoModal() == IDOK)
	{
		color = dlg.GetColor();
		st_work.n_grid_r[1][1] = GetRValue(color);
		st_work.n_grid_g[1][1] = GetGValue(color);
		st_work.n_grid_b[1][1] = GetBValue(color);
	}
}

void CDialog_Color::OnBtnOText2() 
{
	// TODO: Add your control notification handler code here
	CColorDialog dlg;
	COLORREF color;
	
	if(dlg.DoModal() == IDOK)
	{
		color = dlg.GetColor();
		st_work.n_text_r[1][1] = GetRValue(color);
		st_work.n_text_g[1][1] = GetGValue(color);
		st_work.n_text_b[1][1] = GetBValue(color);
	}
}

void CDialog_Color::Init_Group()
{
	CSxLogFont main_font(15,FW_SEMIBOLD,false,"Arial");

	m_group_input.SetFont(main_font);
	m_group_input.SetCatptionTextColor(RGB(145,25,0));
	m_group_input.SetFontBold(TRUE);

	m_group_output.SetFont(main_font);
	m_group_output.SetCatptionTextColor(RGB(145,25,0));
	m_group_output.SetFontBold(TRUE);
}
