// Screen_Pgm_Info.cpp : implementation file
//

#include "stdafx.h"
#include "Handler.h"
#include "Screen_Pgm_Info.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CScreen_Pgm_Info

IMPLEMENT_DYNCREATE(CScreen_Pgm_Info, CFormView)

CScreen_Pgm_Info::CScreen_Pgm_Info()
	: CFormView(CScreen_Pgm_Info::IDD)
{
	//{{AFX_DATA_INIT(CScreen_Pgm_Info)
	//}}AFX_DATA_INIT
}

CScreen_Pgm_Info::~CScreen_Pgm_Info()
{
}

void CScreen_Pgm_Info::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CScreen_Pgm_Info)
	DDX_Control(pDX, IDC_GROUP_PGM_INFO, m_group_pgm_info);
	DDX_Control(pDX, IDC_GROUP_TIME, m_group_time);
	DDX_Control(pDX, IDC_GROUP_MOTOR_POS, m_group_motor_pos);
	DDX_Control(pDX, IDC_GROUP_MOTOR_PART, m_group_motor_part);
	DDX_Control(pDX, IDC_GROUP_IO_PART, m_group_io_part);
	DDX_Control(pDX, IDC_GROUP_IO, m_group_io);
	DDX_Control(pDX, IDC_BTN_TIME_SAVE, m_btn_time_save);
	DDX_Control(pDX, IDC_BTN_TIME_OPEN, m_btn_time_open);
	DDX_Control(pDX, IDC_BTN_MOTOR_POS_SAVE, m_btn_motor_pos_save);
	DDX_Control(pDX, IDC_BTN_MOTOR_POS_OPEN, m_btn_motor_pos_open);
	DDX_Control(pDX, IDC_BTN_MOTOR_PART_SAVE, m_btn_motor_part_save);
	DDX_Control(pDX, IDC_BTN_MOTOR_PART_OPEN, m_btn_motor_part_open);
	DDX_Control(pDX, IDC_BTN_IO_SAVE, m_btn_io_save);
	DDX_Control(pDX, IDC_BTN_IO_PART_SAVE, m_btn_io_part_save);
	DDX_Control(pDX, IDC_BTN_IO_PART_OPEN, m_btn_io_part_open);
	DDX_Control(pDX, IDC_BTN_IO_OPEN, m_btn_io_open);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CScreen_Pgm_Info, CFormView)
	//{{AFX_MSG_MAP(CScreen_Pgm_Info)
	ON_BN_CLICKED(IDC_BTN_OPEN, OnBtnOpen)
	ON_BN_CLICKED(IDC_BTN_SAVE, OnBtnSave)
	ON_BN_CLICKED(IDC_BTN_IO_OPEN, OnBtnIoOpen)
	ON_BN_CLICKED(IDC_BTN_IO_PART_OPEN, OnBtnIoPartOpen)
	ON_BN_CLICKED(IDC_BTN_IO_PART_SAVE, OnBtnIoPartSave)
	ON_BN_CLICKED(IDC_BTN_IO_SAVE, OnBtnIoSave)
	ON_BN_CLICKED(IDC_BTN_MOTOR_PART_OPEN, OnBtnMotorPartOpen)
	ON_BN_CLICKED(IDC_BTN_MOTOR_PART_SAVE, OnBtnMotorPartSave)
	ON_BN_CLICKED(IDC_BTN_MOTOR_POS_OPEN, OnBtnMotorPosOpen)
	ON_BN_CLICKED(IDC_BTN_MOTOR_POS_SAVE, OnBtnMotorPosSave)
	ON_BN_CLICKED(IDC_BTN_TIME_OPEN, OnBtnTimeOpen)
	ON_BN_CLICKED(IDC_BTN_TIME_SAVE, OnBtnTimeSave)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CScreen_Pgm_Info diagnostics

#ifdef _DEBUG
void CScreen_Pgm_Info::AssertValid() const
{
	CFormView::AssertValid();
}

void CScreen_Pgm_Info::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CScreen_Pgm_Info message handlers

void CScreen_Pgm_Info::OnBtnOpen() 
{
	// TODO: Add your control notification handler code here
	CFileDialog dlgFileOpen(TRUE, 
		"xls", 
		NULL, 
		OFN_ALLOWMULTISELECT|OFN_FILEMUSTEXIST, 
		"xls File Format (*.xls)|*.xls|All Files(*.*)|*.*||",
		//		"Row File Format (*.raw)|*.raw|All Files(*.*)|*.*||",
		NULL );
	
	CString filename;
	CString filetitle;
	
	m_grid_info = (TSpread*)GetDlgItem(IDC_CUSTOM_PGM_INFO);
	
	if(dlgFileOpen.DoModal()==IDOK )
	{
		filetitle = dlgFileOpen.GetFileTitle();
		
		filename.Format("%s.xls",filetitle);
		
		m_p_grid.GridFileOpen(m_grid_info, filename, "Sheet1");
	}
}

void CScreen_Pgm_Info::OnBtnSave() 
{
	// TODO: Add your control notification handler code here
	CFileDialog dlgFileOpen(FALSE, 
		"xls", 
		NULL, 
		OFN_ALLOWMULTISELECT|OFN_FILEMUSTEXIST, 
		"xls File Format (*.xls)|*.xls|All Files(*.*)|*.*||",
		//		"Row File Format (*.raw)|*.raw|All Files(*.*)|*.*||",
		NULL );
	
	CString filename;
	CString filetitle;
	
	m_grid_info = (TSpread*)GetDlgItem(IDC_CUSTOM_PGM_INFO);
	
	if(dlgFileOpen.DoModal()==IDOK )
	{
		filetitle = dlgFileOpen.GetFileTitle();
		
		filename.Format("%s.xls",filetitle);
		
		m_p_grid.GridFileSave(m_grid_info, filename, "Sheet1");
	}
}

void CScreen_Pgm_Info::OnBtnIoOpen() 
{
	// TODO: Add your control notification handler code here
	m_grid_info = (TSpread*)GetDlgItem(IDC_CUSTOM_PGM_INFO);
	m_p_grid.GridFileOpen(m_grid_info, st_path.mstr_io_map, "Sheet1");

	m_btn_io_save.EnableWindow(TRUE);
	m_btn_io_part_save.EnableWindow(FALSE);
	
	m_btn_motor_part_save.EnableWindow(FALSE);
	m_btn_motor_pos_save.EnableWindow(FALSE);

	m_btn_time_save.EnableWindow(FALSE);
}

void CScreen_Pgm_Info::OnBtnIoPartOpen() 
{
	// TODO: Add your control notification handler code here
	m_grid_info = (TSpread*)GetDlgItem(IDC_CUSTOM_PGM_INFO);
	m_p_grid.GridFileOpen(m_grid_info, st_path.mstr_io_part_map, "Sheet1");

	m_btn_io_save.EnableWindow(FALSE);
	m_btn_io_part_save.EnableWindow(TRUE);
	
	m_btn_motor_part_save.EnableWindow(FALSE);
	m_btn_motor_pos_save.EnableWindow(FALSE);
	
	m_btn_time_save.EnableWindow(FALSE);
}

void CScreen_Pgm_Info::OnBtnIoPartSave() 
{
	// TODO: Add your control notification handler code here
	m_grid_info = (TSpread*)GetDlgItem(IDC_CUSTOM_PGM_INFO);
	m_p_grid.GridFileSave(m_grid_info, st_path.mstr_io_part_map, "Sheet1");

	m_btn_io_part_save.EnableWindow(FALSE);
}

void CScreen_Pgm_Info::OnBtnIoSave() 
{
	// TODO: Add your control notification handler code here
	m_grid_info = (TSpread*)GetDlgItem(IDC_CUSTOM_PGM_INFO);	
	m_p_grid.GridFileSave(m_grid_info, st_path.mstr_io_map, "Sheet1");

	m_btn_io_save.EnableWindow(FALSE);
}

void CScreen_Pgm_Info::OnBtnMotorPartOpen() 
{
	// TODO: Add your control notification handler code here
	m_grid_info = (TSpread*)GetDlgItem(IDC_CUSTOM_PGM_INFO);
	m_p_grid.GridFileOpen(m_grid_info, st_path.mstr_motor_part_map, "Sheet1");

	m_btn_io_save.EnableWindow(FALSE);
	m_btn_io_part_save.EnableWindow(FALSE);
	
	m_btn_motor_part_save.EnableWindow(TRUE);
	m_btn_motor_pos_save.EnableWindow(FALSE);
	
	m_btn_time_save.EnableWindow(FALSE);
}

void CScreen_Pgm_Info::OnBtnMotorPartSave() 
{
	// TODO: Add your control notification handler code here
	m_grid_info = (TSpread*)GetDlgItem(IDC_CUSTOM_PGM_INFO);
	m_p_grid.GridFileSave(m_grid_info, st_path.mstr_motor_part_map, "Sheet1");

	m_btn_motor_part_save.EnableWindow(FALSE);
}

void CScreen_Pgm_Info::OnBtnMotorPosOpen() 
{
	// TODO: Add your control notification handler code here
	m_grid_info = (TSpread*)GetDlgItem(IDC_CUSTOM_PGM_INFO);
	m_p_grid.GridFileOpen(m_grid_info, st_path.mstr_motor_axis_map, "Sheet1");

	m_btn_io_save.EnableWindow(FALSE);
	m_btn_io_part_save.EnableWindow(FALSE);
	
	m_btn_motor_part_save.EnableWindow(FALSE);
	m_btn_motor_pos_save.EnableWindow(TRUE);
	
	m_btn_time_save.EnableWindow(FALSE);
}

void CScreen_Pgm_Info::OnBtnMotorPosSave() 
{
	// TODO: Add your control notification handler code here
	m_grid_info = (TSpread*)GetDlgItem(IDC_CUSTOM_PGM_INFO);	
	m_p_grid.GridFileSave(m_grid_info, st_path.mstr_motor_axis_map, "Sheet1");

	m_btn_motor_pos_save.EnableWindow(FALSE);
}

void CScreen_Pgm_Info::OnBtnTimeOpen() 
{
	// TODO: Add your control notification handler code here
	m_grid_info = (TSpread*)GetDlgItem(IDC_CUSTOM_PGM_INFO);
	m_p_grid.GridFileOpen(m_grid_info, st_path.mstr_wait_time_map, "Sheet1");

	m_btn_io_save.EnableWindow(FALSE);
	m_btn_io_part_save.EnableWindow(FALSE);
	
	m_btn_motor_part_save.EnableWindow(FALSE);
	m_btn_motor_pos_save.EnableWindow(FALSE);
	
	m_btn_time_save.EnableWindow(TRUE);
}

void CScreen_Pgm_Info::OnBtnTimeSave() 
{
	// TODO: Add your control notification handler code here
	m_grid_info = (TSpread*)GetDlgItem(IDC_CUSTOM_PGM_INFO);
	m_p_grid.GridFileSave(m_grid_info, st_path.mstr_wait_time_map, "Sheet1");

	m_btn_time_save.EnableWindow(FALSE);
}

void CScreen_Pgm_Info::Init_Group()
{
	CSxLogFont main_font(15,FW_SEMIBOLD,false,"Arial");
	
	m_group_io.SetFont(main_font);
	m_group_io.SetCatptionTextColor(BLUE_C);
	m_group_io.SetBorderColor(YELLOW_L);
	m_group_io.SetFontBold(TRUE);

	m_group_io_part.SetFont(main_font);
	m_group_io_part.SetCatptionTextColor(BLUE_C);
	m_group_io_part.SetBorderColor(YELLOW_L);
	m_group_io_part.SetFontBold(TRUE);

	m_group_motor_part.SetFont(main_font);
	m_group_motor_part.SetCatptionTextColor(BLUE_C);
	m_group_motor_part.SetBorderColor(YELLOW_L);
	m_group_motor_part.SetFontBold(TRUE);

	m_group_motor_pos.SetFont(main_font);
	m_group_motor_pos.SetCatptionTextColor(BLUE_C);
	m_group_motor_pos.SetBorderColor(YELLOW_L);
	m_group_motor_pos.SetFontBold(TRUE);

	m_group_time.SetFont(main_font);
	m_group_time.SetCatptionTextColor(BLUE_C);
	m_group_time.SetBorderColor(YELLOW_L);
	m_group_time.SetFontBold(TRUE);

	m_group_pgm_info.SetFont(main_font);
	m_group_pgm_info.SetCatptionTextColor(BLUE_C);
	m_group_pgm_info.SetBorderColor(YELLOW_L);
	m_group_pgm_info.SetFontBold(TRUE);
}

void CScreen_Pgm_Info::Init_Button()
{
	short	shBtnColor = 30;

	m_btn_io_open.SetFont(m_p_font);
	m_btn_io_open.SetIcon(IDI_OPEN);
	m_btn_io_open.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);
	m_btn_io_open.SetButtonColor(1);

	m_btn_io_save.SetFont(m_p_font);
	m_btn_io_save.SetIcon(IDI_SAVE);
	m_btn_io_save.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);
	m_btn_io_save.SetButtonColor(1);
	m_btn_io_save.EnableWindow(FALSE);

	m_btn_io_part_open.SetFont(m_p_font);
	m_btn_io_part_open.SetIcon(IDI_OPEN);
	m_btn_io_part_open.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);
	m_btn_io_part_open.SetButtonColor(1);
	
	m_btn_io_part_save.SetFont(m_p_font);
	m_btn_io_part_save.SetIcon(IDI_SAVE);
	m_btn_io_part_save.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);
	m_btn_io_part_save.SetButtonColor(1);
	m_btn_io_part_save.EnableWindow(FALSE);

	m_btn_motor_part_open.SetFont(m_p_font);
	m_btn_motor_part_open.SetIcon(IDI_OPEN);
	m_btn_motor_part_open.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);
	m_btn_motor_part_open.SetButtonColor(1);
	
	m_btn_motor_part_save.SetFont(m_p_font);
	m_btn_motor_part_save.SetIcon(IDI_SAVE);
	m_btn_motor_part_save.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);
	m_btn_motor_part_save.SetButtonColor(1);
	m_btn_motor_part_save.EnableWindow(FALSE);

	m_btn_motor_pos_open.SetFont(m_p_font);
	m_btn_motor_pos_open.SetIcon(IDI_OPEN);
	m_btn_motor_pos_open.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);
	m_btn_motor_pos_open.SetButtonColor(1);
	
	m_btn_motor_pos_save.SetFont(m_p_font);
	m_btn_motor_pos_save.SetIcon(IDI_SAVE);
	m_btn_motor_pos_save.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);
	m_btn_motor_pos_save.SetButtonColor(1);
	m_btn_motor_pos_save.EnableWindow(FALSE);

	m_btn_time_open.SetFont(m_p_font);
	m_btn_time_open.SetIcon(IDI_OPEN);
	m_btn_time_open.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);
	m_btn_time_open.SetButtonColor(1);
	
	m_btn_time_save.SetFont(m_p_font);
	m_btn_time_save.SetIcon(IDI_SAVE);
	m_btn_time_save.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);
	m_btn_time_save.SetButtonColor(1);
	m_btn_time_save.EnableWindow(FALSE);
}

BOOL CScreen_Pgm_Info::DestroyWindow() 
{
	// TODO: Add your specialized code here and/or call the base class
	if(m_p_font != NULL)
	{
		delete m_p_font;
		m_p_font = NULL;
	}

	if(st_handler.n_level_master)
	{
		st_handler.n_level_master = FALSE;
	}
	return CFormView::DestroyWindow();
}

void CScreen_Pgm_Info::OnInitialUpdate() 
{
	CFormView::OnInitialUpdate();
	
	// TODO: Add your specialized code here and/or call the base class
	m_p_font = NULL;
	m_p_font = new CFont;
	m_p_font->CreateFont(24,0,0,0,900,0,0,0,0,0,0,ANTIALIASED_QUALITY,0,"MS Sans Serif");;

	Init_Button();
	Init_Group();
}
