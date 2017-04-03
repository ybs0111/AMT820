// Screen_Basic.cpp : implementation file
//

#include "stdafx.h"
#include "HANDLER.h"
#include "Screen_Basic.h"

#include "io.h"  // 파일 존재 유무 검사 함수 호출하기 위해서는 반드시 INCLUDE 필요

/* ****************************************************************************** */
/* 대화 상자 클래스 추가                                                          */
/* ****************************************************************************** */
//#include "Dialog_Infor.h"
#include "Dialog_Select.h"
#include "Dialog_Message.h"
#include "Dialog_KeyBoard.h"

#include "Dialog_KeyPad.h"
#include "Keyboarddll.h"

#include "Public_Function.h"
#include "Dialog_Infor.h"
/* ****************************************************************************** */

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CScreen_Basic

IMPLEMENT_DYNCREATE(CScreen_Basic, CFormView)

CScreen_Basic::CScreen_Basic()
	: CFormView(CScreen_Basic::IDD)
{
	//{{AFX_DATA_INIT(CScreen_Basic)
	//}}AFX_DATA_INIT
}

CScreen_Basic::~CScreen_Basic()
{
}

void CScreen_Basic::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CScreen_Basic)
	DDX_Control(pDX, IDC_BTN_DELETE, m_btn_delete);
	DDX_Control(pDX, IDC_BTN_BASIC_CREATE, m_btn_create);
	DDX_Control(pDX, IDC_GROUP_OPERATE_METHOD, m_group_operate_method);
	DDX_Control(pDX, IDC_GROUP_MODEL, m_group_model);
	DDX_Control(pDX, IDC_BTN_BASIC_CANCEL, m_btn_cancel);
	DDX_Control(pDX, IDC_BTN_BASIC_APPLY, m_btn_apply);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CScreen_Basic, CFormView)
	//{{AFX_MSG_MAP(CScreen_Basic)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BTN_BASIC_CANCEL, OnBtnBasicCancel)
	ON_BN_CLICKED(IDC_BTN_BASIC_APPLY, OnBtnBasicApply)
	ON_BN_CLICKED(IDC_BTN_BASIC_CREATE, OnBtnBasicCreate)
	ON_BN_CLICKED(IDC_BTN_DELETE, OnBtnDelete)
	//}}AFX_MSG_MAP
	ON_MESSAGE(SSM_CLICK  , OnCell_L_Click)
	ON_MESSAGE(SSM_RBUTTON  , OnCell_R_Click)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CScreen_Basic diagnostics

#ifdef _DEBUG
void CScreen_Basic::AssertValid() const
{
	CFormView::AssertValid();
}

void CScreen_Basic::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CScreen_Basic message handlers

void CScreen_Basic::OnInitialUpdate() 
{
	CFormView::OnInitialUpdate();

	st_handler.cwnd_basic = this;  // BASIC 화면에 대한 핸들 정보 설정

	/* ************************************************************************** */
	/* 컨트롤에 설정할 폰트 생성한다                                              */
	/* ************************************************************************** */
	mp_basic_font = NULL;
	mp_basic_font = new CFont;
	mp_basic_font->CreateFont(12,8,0,0,900,0,0,0,0,0,0,DEFAULT_QUALITY,0,"Arial");
	/* ************************************************************************** */
	Data_Init();

	Init_Button();
	Init_Group();
	Init_Digital();
	Init_Label();

	Init_Grid_File();
	Init_Grid_Operate();

	Data_Display();
	OnBasic_Init_List(_T("C:\\AMT820\\motor\\"));  // 파일 폴더 초기화 함수
	/* ************************************************************************** */

	UpdateData(FALSE);
}

void CScreen_Basic::OnDestroy() 
{

	CFormView::OnDestroy();
}

void CScreen_Basic::OnCell_L_Click(WPARAM wParam, LPARAM lParam)
{
	LPSS_CELLCOORD lpcc = (LPSS_CELLCOORD)lParam;
	CDialog_Message msg_dlg;
	CDialog_Select	select_dlg;
	CString			str_tmp;
	int				n_response;
	
	if(wParam == IDC_CUSTOM_FILE_LIST)
	{
		m_grid_file = (TSpread *) GetDlgItem(IDC_CUSTOM_FILE_LIST);

		if (st_work.b_lot_start_flag == true)
		{
			st_other.str_fallacy_msg = _T("It is available after Lot End.");
			
			n_response = msg_dlg.DoModal();
			
			if (n_response == IDOK)
			{
				return ;
			}
			
		}

		mstr_device_name[1] = m_p_grid.GridCellText(m_grid_file, lpcc->Row, lpcc->Col);
		
		if(OnBasic_Input_Device_Check(mstr_device_name[1]) == FALSE)  // 입력된 디바이스 이름 정상 유무 검사 함수
		{
			mstr_device_name[1] = mstr_device_name[0];
			return ;
		}

		st_other.str_confirm_msg = _T(mstr_device_name[1] + " 모델을 시작하시겠습니까?..");
		
		n_response = select_dlg.DoModal();
		
		if(n_response == IDOK)
		{
			st_basic.mstr_device_name = mstr_device_name[1];
			
			mcls_m_basic.OnBasic_Data_Load();
			mcls_m_basic.On_Teach_Data_Load();
			mcls_m_basic.OnMaintenance_Data_Load();
			mcls_m_basic.OnWaitTime_Data_Load();
			mcls_m_basic.OnMotorSpeed_Set_Data_Load();
			mcls_m_basic.OnInterface_Data_Load();
			
			Data_Init();

			m_p_grid.GridCellColor(m_grid_file, m_n_filename_pos, 1, WHITE_C, BLACK_C);
			m_p_grid.GridCellColor(m_grid_file, m_n_filename_pos, 2, WHITE_C, BLACK_C);

			m_n_filename_pos = lpcc->Row;
			m_p_grid.GridCellColor(m_grid_file, m_n_filename_pos, 1, GREEN_C, BLACK_C);
			m_p_grid.GridCellColor(m_grid_file, m_n_filename_pos, 2, GREEN_C, BLACK_C);

			if(st_handler.cwnd_title != NULL)
			{
				st_handler.cwnd_title->PostMessage(WM_STATUS_CHANGE, FILE_MODE, 0);
			}
		}
	}
	else if(wParam == IDC_CUSTOM_OPERATE)
	{
		m_grid_operate = (TSpread*) GetDlgItem(IDC_CUSTOM_OPERATE);

		switch(lpcc->Row)
		{
			case 2:
				if(m_n_mode_interface[1] == 0) return;

				m_p_grid.GridCellColor(m_grid_operate, 2, lpcc->Col, GREEN_C, BLACK_C);
				m_p_grid.GridCellColor(m_grid_operate, 3, lpcc->Col, GREEN_D, BLACK_C);
				m_n_mode_interface[1] = 0;
				break;

			case 3:
				if(m_n_mode_interface[1] == 1) return;
				
				m_p_grid.GridCellColor(m_grid_operate, 2, lpcc->Col, GREEN_D, BLACK_C);
				m_p_grid.GridCellColor(m_grid_operate, 3, lpcc->Col, GREEN_C, BLACK_C);
				m_n_mode_interface[1] = 1;
				break;

			case 5:
				if(m_n_mode_device[1] == 0) return;
				
				m_p_grid.GridCellColor(m_grid_operate, 5, lpcc->Col, GREEN_C, BLACK_C);
				m_p_grid.GridCellColor(m_grid_operate, 6, lpcc->Col, GREEN_D, BLACK_C);
				m_p_grid.GridCellColor(m_grid_operate, 7, lpcc->Col, GREEN_D, BLACK_C);
				m_n_mode_device[1] = 0;
				break;

			case 6:
				if(m_n_mode_device[1] == 1) return;
				
				m_p_grid.GridCellColor(m_grid_operate, 5, lpcc->Col, GREEN_D, BLACK_C);
				m_p_grid.GridCellColor(m_grid_operate, 6, lpcc->Col, GREEN_C, BLACK_C);
				m_p_grid.GridCellColor(m_grid_operate, 7, lpcc->Col, GREEN_D, BLACK_C);
				m_n_mode_device[1] = 1;
				break;

			case 7:
				if(m_n_mode_device[1] == 2) return;
				
				m_p_grid.GridCellColor(m_grid_operate, 5, lpcc->Col, GREEN_D, BLACK_C);
				m_p_grid.GridCellColor(m_grid_operate, 6, lpcc->Col, GREEN_D, BLACK_C);
				m_p_grid.GridCellColor(m_grid_operate, 7, lpcc->Col, GREEN_C, BLACK_C);
				m_n_mode_device[1] = 2;
				break;

			case 9:
				if(m_n_mode_retest[1] == 0) return;
				
				m_p_grid.GridCellColor(m_grid_operate, 9, lpcc->Col, GREEN_C, BLACK_C);
				m_p_grid.GridCellColor(m_grid_operate, 10, lpcc->Col, GREEN_D, BLACK_C);
				m_n_mode_retest[1] = 0;
				break;

			case 10:
				if(m_n_mode_retest[1] == 1) return;
				
				m_p_grid.GridCellColor(m_grid_operate, 9, lpcc->Col, GREEN_D, BLACK_C);
				m_p_grid.GridCellColor(m_grid_operate, 10, lpcc->Col, GREEN_C, BLACK_C);
				m_n_mode_retest[1] = 1;
				break;

			case 11:
				str_tmp.Format("%d", m_n_count_retry[1]);
				str_tmp = GetNumEditString_I((double)0, (double)5, str_tmp);
				m_p_grid.GridCellText(m_grid_operate, lpcc->Row, lpcc->Col, str_tmp);
				m_n_count_retry[1] = atoi(str_tmp);
				break;
		}
	}
}

void CScreen_Basic::OnCell_R_Click(WPARAM wParam, LPARAM lParam)
{
	LPSS_CELLCOORD lpcc = (LPSS_CELLCOORD)lParam;
	CDialog_Message msg_dlg;
	CDialog_Select	select_dlg;
	CString			str_tmp;
	CString			str_filename;
	int				n_response;
	int				n_existence;			// 파일 존재 유무 플래그

	if(wParam == IDC_CUSTOM_FILE_LIST)
	{
		m_grid_file = (TSpread *) GetDlgItem(IDC_CUSTOM_FILE_LIST);
		
		if (st_work.b_lot_start_flag == true)
		{
			st_other.str_fallacy_msg = _T("It is available after Lot End.");
			
			n_response = msg_dlg.DoModal();
			
			if (n_response == IDOK)
			{
				return ;
			}
			
		}
		
		str_tmp = m_p_grid.GridCellText(m_grid_file, lpcc->Row, lpcc->Col);
		
		if(OnBasic_Input_Device_Check(str_tmp) == FALSE)  // 입력된 디바이스 이름 정상 유무 검사 함수
		{
			return ;
		}
		
		st_other.str_confirm_msg = _T(str_tmp + " 모델 삭제하시겠습니까?..");
		
		n_response = select_dlg.DoModal();
		
		if(n_response == IDOK)
		{
			if(str_tmp == mstr_device_name[1])
			{
				mstr_device_name[0] = mstr_device_name[1] = m_p_grid.GridCellText(m_grid_file, 1, 2);
			}

			str_filename = st_path.mstr_basic + str_tmp;
			
			n_existence = access(str_filename, 0) ;
			
			if (n_existence != -1)			// 파일 존재
			{
				DeleteFile(str_filename);  // 해당 파일 삭제
				
				Init_Grid_File();
				OnBasic_Init_List(st_path.mstr_basic);// 파일 폴더 초기화 함수
			}
			else  
			{
				return ;
			}
		}
	}
}
void CScreen_Basic::Init_Button()
{
	short shBtnColor = 30;

	m_btn_create.SetFont(mp_basic_font);
	m_btn_create.SetIcon(IDI_CREATE);
	m_btn_create.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);
	m_btn_create.SetButtonColor(1);

	m_btn_delete.SetFont(mp_basic_font);
	m_btn_delete.SetIcon(IDI_DELETE_2);
	m_btn_delete.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);
	m_btn_delete.SetButtonColor(1);
/*
	m_chk_work_mode.SetFont(mp_basic_font);
	m_chk_work_mode.SetIcon(IDI_DOUBLE, IDI_MODULE);
	m_chk_work_mode.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);
	m_chk_work_mode.SetButtonColor(1);

	m_chk_module.SetFont(mp_basic_font);
	m_chk_module.SetIcon(IDI_DOUBLE, IDI_MODULE);
	m_chk_module.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);
	m_chk_module.SetButtonColor(1);

	m_chk_conveyor_move_mode.SetFont(mp_basic_font);
	m_chk_conveyor_move_mode.SetIcon(IDI_NONSAFETY, IDI_NONSAFETY);
	m_chk_conveyor_move_mode.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);
	m_chk_conveyor_move_mode.SetButtonColor(1);

	m_chk_length_first.SetFont(mp_basic_font);
	m_chk_length_first.SetIcon(IDI_NONSAFETY, IDI_NONSAFETY);
	m_chk_length_first.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);
	m_chk_length_first.SetButtonColor(1);

	m_chk_length_second.SetFont(mp_basic_font);
	m_chk_length_second.SetIcon(IDI_NONSAFETY, IDI_NONSAFETY);
	m_chk_length_second.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);
	m_chk_length_second.SetButtonColor(1);

	m_chk_length_third.SetFont(mp_basic_font);
	m_chk_length_third.SetIcon(IDI_NONSAFETY, IDI_NONSAFETY);
	m_chk_length_third.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);
	m_chk_length_third.SetButtonColor(1);

	m_chk_thickness.SetFont(mp_basic_font);
	m_chk_thickness.SetIcon(IDI_NONSAFETY, IDI_NONSAFETY);
	m_chk_thickness.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);
	m_chk_thickness.SetButtonColor(1);

	m_chk_barcode_read.SetFont(mp_basic_font);
	m_chk_barcode_read.SetIcon(IDI_NONSAFETY, IDI_NONSAFETY);
	m_chk_barcode_read.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);
	m_chk_barcode_read.SetButtonColor(1);

	m_chk_air_blow.SetFont(mp_basic_font);
	m_chk_air_blow.SetIcon(IDI_NONSAFETY, IDI_NONSAFETY);
	m_chk_air_blow.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);
	m_chk_air_blow.SetButtonColor(1);*/
}

void CScreen_Basic::Init_Group()
{
	CSxLogFont main_font(16,FW_SEMIBOLD,false,"MS Sans Serif");

	m_group_model.SetFont(main_font);
	m_group_model.SetCatptionTextColor(BLUE_C);
	m_group_model.SetBorderColor(YELLOW_L);
	m_group_model.SetFontBold(TRUE);

	m_group_operate_method.SetFont(main_font);
	m_group_operate_method.SetCatptionTextColor(BLUE_C);
	m_group_operate_method.SetBorderColor(YELLOW_L);
	m_group_operate_method.SetFontBold(TRUE);
/*
	m_groupWork_Info.SetFont(main_font);
	m_groupWork_Info.SetCatptionTextColor(BLUE_C);
	m_groupWork_Info.SetBorderColor(YELLOW_L);
//	m_groupWork_Info.SetBackgroundColor(YELLOW_L);
	m_groupWork_Info.SetFontBold(TRUE);

	m_groupPass_Tray_Info.SetFont(main_font);
	m_groupPass_Tray_Info.SetCatptionTextColor(BLUE_C);
	m_groupPass_Tray_Info.SetBorderColor(YELLOW_L);
//	m_groupPass_Tray_Info.SetBackgroundColor(YELLOW_L);
	m_groupPass_Tray_Info.SetFontBold(TRUE);

	m_groupPass_Tray.SetFont(main_font);
	m_groupPass_Tray.SetCatptionTextColor(BLUE_C);
	m_groupPass_Tray.SetBorderColor(YELLOW_L);
//	m_groupPass_Tray.SetBackgroundColor(YELLOW_L);
	m_groupPass_Tray.SetFontBold(TRUE);

	m_groupTray_Used.SetFont(main_font);
	m_groupTray_Used.SetCatptionTextColor(BLUE_C);
	m_groupTray_Used.SetBorderColor(YELLOW_L);
//	m_groupTray_Used.SetBackgroundColor(YELLOW_L);
	m_groupTray_Used.SetFontBold(TRUE);

	m_group_work_mode.SetFont(main_font);
	m_group_work_mode.SetCatptionTextColor(BLUE_C);
	m_group_work_mode.SetBorderColor(YELLOW_L);
//	m_group_work_mode.SetBackgroundColor(YELLOW_L);
	m_group_work_mode.SetFontBold(TRUE);

	m_group_turn_mode.SetFont(main_font);
	m_group_turn_mode.SetCatptionTextColor(BLUE_C);
	m_group_turn_mode.SetBorderColor(YELLOW_L);
//	m_group_turn_mode.SetBackgroundColor(YELLOW_L);
	m_group_turn_mode.SetFontBold(TRUE);

	m_group_device_mode.SetFont(main_font);
	m_group_device_mode.SetCatptionTextColor(BLUE_C);
	m_group_device_mode.SetBorderColor(YELLOW_L);
//	m_group_device_mode.SetBackgroundColor(YELLOW_L);
	m_group_device_mode.SetFontBold(TRUE);

	m_group_retry.SetFont(main_font);
	m_group_retry.SetCatptionTextColor(BLUE_C);
	m_group_retry.SetBorderColor(YELLOW_L);
//	m_group_retry.SetBackgroundColor(YELLOW_L);
	m_group_retry.SetFontBold(TRUE);

	m_group_tray_gap_info.SetFont(main_font);
	m_group_tray_gap_info.SetCatptionTextColor(BLUE_C);
	m_group_tray_gap_info.SetBorderColor(YELLOW_L);
//	m_group_tray_gap_info.SetBackgroundColor(YELLOW_L);
	m_group_tray_gap_info.SetFontBold(TRUE);

	m_group_air_blow.SetFont(main_font);
	m_group_air_blow.SetCatptionTextColor(BLUE_C);
	m_group_air_blow.SetBorderColor(YELLOW_L);
	//	m_group_tray_gap_info.SetBackgroundColor(YELLOW_L);
	m_group_air_blow.SetFontBold(TRUE);

	m_group_conveyor_move_mode.SetFont(main_font);
	m_group_conveyor_move_mode.SetCatptionTextColor(BLUE_C);
	m_group_conveyor_move_mode.SetBorderColor(YELLOW_L);
	//	m_group_tray_gap_info.SetBackgroundColor(YELLOW_L);
	m_group_conveyor_move_mode.SetFontBold(TRUE);

	m_group_length_measure.SetFont(main_font);
	m_group_length_measure.SetCatptionTextColor(BLUE_C);
	m_group_length_measure.SetBorderColor(YELLOW_L);
	//	m_group_tray_gap_info.SetBackgroundColor(YELLOW_L);
	m_group_length_measure.SetFontBold(TRUE);

	m_group_wait_time.SetFont(main_font);
	m_group_wait_time.SetCatptionTextColor(BLUE_C);
	m_group_wait_time.SetBorderColor(YELLOW_L);
	//	m_group_tray_gap_info.SetBackgroundColor(YELLOW_L);
	m_group_wait_time.SetFontBold(TRUE);

	m_group_barcode_read.SetFont(main_font);
	m_group_barcode_read.SetCatptionTextColor(BLUE_C);
	m_group_barcode_read.SetBorderColor(YELLOW_L);
	//	m_group_tray_gap_info.SetBackgroundColor(YELLOW_L);
	m_group_barcode_read.SetFontBold(TRUE);*/
}

void CScreen_Basic::Init_Digital()
{
/*	m_dgtPass_Row.SubclassDlgItem(IDC_DGT_PASS_ROW, this);
	m_dgtPass_Row.SetStyle(IDB_BIG3, 2);
	m_dgtPass_Row.SetValue(m_nPass_Row[1]);

	m_dgtPass_Col.SubclassDlgItem(IDC_DGT_PASS_COL, this);
	m_dgtPass_Col.SetStyle(IDB_BIG3, 2);
	m_dgtPass_Col.SetValue(m_nPass_Col[1]);

	m_dgt_retry.SetStyle(CDigit::DS_INT, 3, CDigit::DC_BGREEN, CDigit::DC_BDISABLE);
	m_dgt_retry.SetVal(m_n_retry_cnt[1]);

	m_dgt_pass_gap_x.SetStyle(CDigit::DS_FLOAT, 5, CDigit::DC_BGREEN, CDigit::DC_BDISABLE);
	m_dgt_pass_gap_x.SetFloatVal(m_f_pass_tray_gap_x[1]);

	m_dgt_pass_gap_y.SetStyle(CDigit::DS_FLOAT, 5, CDigit::DC_BGREEN, CDigit::DC_BDISABLE);
	m_dgt_pass_gap_y.SetFloatVal(m_f_pass_tray_gap_y[1]);

	m_dgt_reject_gap_x.SetStyle(CDigit::DS_FLOAT, 5, CDigit::DC_BGREEN, CDigit::DC_BDISABLE);
	m_dgt_reject_gap_x.SetFloatVal(m_f_reject_tray_gap_x[1]);

	m_dgt_air_blow_dn.SetStyle(CDigit::DS_INT, 5, CDigit::DC_BGREEN, CDigit::DC_BDISABLE);
	m_dgt_air_blow_dn.SetVal(m_n_air_blow_dn[1]);

	m_dgt_air_blow_up.SetStyle(CDigit::DS_INT, 5, CDigit::DC_BGREEN, CDigit::DC_BDISABLE);
	m_dgt_air_blow_up.SetVal(m_n_air_blow_up[1]);

	m_dgt_air_blow_count.SetStyle(CDigit::DS_INT, 5, CDigit::DC_BGREEN, CDigit::DC_BDISABLE);
	m_dgt_air_blow_count.SetVal(m_n_air_blow_count[1]);

	m_dgt_router_wait_time.SetStyle(CDigit::DS_INT, 5, CDigit::DC_BGREEN, CDigit::DC_BDISABLE);
	m_dgt_router_wait_time.SetVal(m_n_router_wait_time[1]);

	m_dgt_separator_wait_time.SetStyle(CDigit::DS_INT, 5, CDigit::DC_BGREEN, CDigit::DC_BDISABLE);
	m_dgt_separator_wait_time.SetVal(m_n_separator_wait_time[1]);

	m_dgt_module_wait_time.SetStyle(CDigit::DS_INT, 5, CDigit::DC_BGREEN, CDigit::DC_BDISABLE);
	m_dgt_module_wait_time.SetVal(m_n_module_wait_time[1]);

	m_dgt_conveyor_move_time.SetStyle(CDigit::DS_INT, 5, CDigit::DC_BGREEN, CDigit::DC_BDISABLE);
	m_dgt_conveyor_move_time.SetVal(m_n_conveyor_move_time[1]);*/
}

void CScreen_Basic::Data_Init()
{
	
	mstr_device_name[1]				= st_basic.mstr_device_name;
	
	m_n_mode_interface[1]			= st_basic.n_mode_interface;
	m_n_mode_device[1]				= st_basic.n_mode_device;
	m_n_mode_retest[1]				= st_basic.n_mode_retest;

	m_n_count_retry[1]				= st_basic.n_count_retry;
	Data_Backup();
	
}


void CScreen_Basic::Init_Label()
{
/*	m_msgPass_Row.SetFont(mp_basic_font);
	m_msgPass_Row.SetWindowText(_T("X"));
	m_msgPass_Row.SetCenterText();
	m_msgPass_Row.SetColor(WHITE_C);
	m_msgPass_Row.SetGradientColor(GREEN_C);
	m_msgPass_Row.SetTextColor(BLACK_C);

	m_msgPass_Col.SetFont(mp_basic_font);
	m_msgPass_Col.SetWindowText(_T("Y"));
	m_msgPass_Col.SetCenterText();
	m_msgPass_Col.SetColor(WHITE_C);
	m_msgPass_Col.SetGradientColor(GREEN_C);
	m_msgPass_Col.SetTextColor(BLACK_C);

	m_msg_pass_gap_x.SetFont(mp_basic_font);
	m_msg_pass_gap_x.SetWindowText(_T("양품 가로"));
	m_msg_pass_gap_x.SetCenterText();
	m_msg_pass_gap_x.SetColor(WHITE_C);
	m_msg_pass_gap_x.SetGradientColor(GREEN_C);
	m_msg_pass_gap_x.SetTextColor(BLACK_C);

	m_msg_pass_gap_y.SetFont(mp_basic_font);
	m_msg_pass_gap_y.SetWindowText(_T("양품 세로"));
	m_msg_pass_gap_y.SetCenterText();
	m_msg_pass_gap_y.SetColor(WHITE_C);
	m_msg_pass_gap_y.SetGradientColor(GREEN_C);
	m_msg_pass_gap_y.SetTextColor(BLACK_C);

	m_msg_reject_gap_x.SetFont(mp_basic_font);
	m_msg_reject_gap_x.SetWindowText(_T("불량  가로"));
	m_msg_reject_gap_x.SetCenterText();
	m_msg_reject_gap_x.SetColor(WHITE_C);
	m_msg_reject_gap_x.SetGradientColor(RED_C);
	m_msg_reject_gap_x.SetTextColor(BLACK_C);

	m_msg_air_blow_dn.SetFont(mp_basic_font);
	m_msg_air_blow_dn.SetWindowText(_T("하강 대기"));
	m_msg_air_blow_dn.SetCenterText();
	m_msg_air_blow_dn.SetColor(WHITE_C);
	m_msg_air_blow_dn.SetGradientColor(RED_C);
	m_msg_air_blow_dn.SetTextColor(BLACK_C);

	m_msg_air_blow_up.SetFont(mp_basic_font);
	m_msg_air_blow_up.SetWindowText(_T("상승 대기"));
	m_msg_air_blow_up.SetCenterText();
	m_msg_air_blow_up.SetColor(WHITE_C);
	m_msg_air_blow_up.SetGradientColor(RED_C);
	m_msg_air_blow_up.SetTextColor(BLACK_C);

	m_msg_air_blow_count.SetFont(mp_basic_font);
	m_msg_air_blow_count.SetWindowText(_T("수 량"));
	m_msg_air_blow_count.SetCenterText();
	m_msg_air_blow_count.SetColor(WHITE_C);
	m_msg_air_blow_count.SetGradientColor(RED_C);
	m_msg_air_blow_count.SetTextColor(BLACK_C);

	m_msg_router_wait_time.SetFont(mp_basic_font);
	m_msg_router_wait_time.SetWindowText(_T("후 면"));
	m_msg_router_wait_time.SetCenterText();
	m_msg_router_wait_time.SetColor(WHITE_C);
	m_msg_router_wait_time.SetGradientColor(RED_C);
	m_msg_router_wait_time.SetTextColor(BLACK_C);

	m_msg_separator_wait_time.SetFont(mp_basic_font);
	m_msg_separator_wait_time.SetWindowText(_T("전 면"));
	m_msg_separator_wait_time.SetCenterText();
	m_msg_separator_wait_time.SetColor(WHITE_C);
	m_msg_separator_wait_time.SetGradientColor(RED_C);
	m_msg_separator_wait_time.SetTextColor(BLACK_C);

	m_msg_module_wait_time.SetFont(mp_basic_font);
	m_msg_module_wait_time.SetWindowText(_T("모듈 이동"));
	m_msg_module_wait_time.SetCenterText();
	m_msg_module_wait_time.SetColor(WHITE_C);
	m_msg_module_wait_time.SetGradientColor(RED_C);
	m_msg_module_wait_time.SetTextColor(BLACK_C);

	m_msg_conveyor_move_time.SetFont(mp_basic_font);
	m_msg_conveyor_move_time.SetWindowText(_T("이동"));
	m_msg_conveyor_move_time.SetCenterText();
	m_msg_conveyor_move_time.SetColor(WHITE_C);
	m_msg_conveyor_move_time.SetGradientColor(RED_C);
	m_msg_conveyor_move_time.SetTextColor(BLACK_C);*/
}

void CScreen_Basic::OnBtnBasicCancel() 
{
	// TODO: Add your control notification handler code here
	int nResponse;  // 대화 상자 리턴 플래그
	
	CDialog_Infor  select_dlg;
	
	st_other.str_confirm_msg = _T("Basic : 이전 데이터로 변경하시겠습니까?");
	
	nResponse = select_dlg.DoModal();
	
	if(nResponse == IDOK){
		Data_Recovery();					// 화면 셋팅 정보 백업 받아놓은 변수로 복구 함수

		Data_Init();

		Init_Pass();
		Init_Reject();
		
		Data_Display();
	}
}



void CScreen_Basic::Data_Backup()
{
	mstr_device_name[0]				= mstr_device_name[1];

	m_n_mode_interface[0]			= m_n_mode_interface[1];
	m_n_mode_device[0]				= m_n_mode_device[1];
	m_n_mode_retest[0]				= m_n_mode_retest[1];
	
	m_n_count_retry[0]				= m_n_count_retry[1];
}

void CScreen_Basic::Data_Apply()
{
	st_basic.mstr_device_name		= mstr_device_name[1];

	st_basic.n_mode_interface		= m_n_mode_interface[1];
	st_basic.n_mode_device			= m_n_mode_device[1];
	st_basic.n_mode_retest			= m_n_mode_retest[1];

	st_basic.n_count_retry			= m_n_count_retry[1];

	OnBasic_Data_HisoryLog();
}

int CScreen_Basic::Data_Comparison()
{
	int Ret = RET_GOOD;

	if(mstr_device_name[0]				!= mstr_device_name[1])			return RET_ERROR;

	if(m_n_mode_interface[0]			!= m_n_mode_interface[1])		return RET_ERROR;
	if(m_n_mode_device[0]				!= m_n_mode_device[1])			return RET_ERROR;
	if(m_n_mode_retest[0]				!= m_n_mode_retest[1])			return RET_ERROR;

	if(m_n_count_retry[0]				!= m_n_count_retry[1])			return RET_ERROR;

	return Ret;
}

void CScreen_Basic::Data_Recovery()
{
	mstr_device_name[1]				= mstr_device_name[0];

	m_n_mode_interface[1]			= m_n_mode_interface[0];
	m_n_mode_device[1]				= m_n_mode_device[0];
	m_n_mode_retest[1]				= m_n_mode_retest[0];

	m_n_count_retry[1]				= m_n_count_retry[0];
}

BOOL CScreen_Basic::DestroyWindow() 
{
	// TODO: Add your specialized code here and/or call the base class
	int nResponse;  // 대화 상자에 대한 리턴 값 저장 변수
	int nRet;
	
	st_handler.cwnd_basic = NULL;		// Shifter 화면에 대한 핸들 정보 설정 

	CDialog_Infor select_dlg;
	
	// 프로그램을 빠져나가면서 Save 하지 않고, Form이 Destroy되는것이라면 Setting을 포기한다는 말이다.
	// 비교도 하지 말자.
	if(st_handler.b_program_exit == false)
	{
		nRet = Data_Comparison();
		
		if(nRet == RET_ERROR){
			st_other.str_confirm_msg = _T("데이터가 변경되었습니다. 적용하시겠습니까?");
			nResponse = select_dlg.DoModal();
			
			if(nResponse == IDOK){
				Data_Apply();

				::PostMessage(st_handler.hWnd, WM_DATA_INIT_SAVE, 2, 1);  //데이터를 파일에 저장하라는 메세지
			}
		}
	}
	

	if(mp_basic_font){
		delete mp_basic_font;
		mp_basic_font = NULL;
	}

	return CFormView::DestroyWindow();
}


int CScreen_Basic::OnBasic_Init_List(LPCTSTR pszPath)
{
	int n_chk;						// 함수 리턴 플래그
    CString str_path = pszPath;		// 폴더 설정
	CString strTmp;

	HANDLE hFind;
    WIN32_FIND_DATA fd;
	int n_count = 0;				// 리스트 박스에 추가한 목록 갯수 저장 변수

	if (str_path.Right (1) != "\\")
        str_path += "\\";

	str_path += "*.*";
    
    if ((hFind = ::FindFirstFile ((LPCTSTR) str_path, &fd)) != INVALID_HANDLE_VALUE) 
	{
        if (!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) 
		{
			n_chk = AddItem(n_count, &fd);
			if (n_chk == TRUE)
				n_count++;
        }

        while (::FindNextFile (hFind, &fd)) 
		{
			n_chk = READY;
            if (!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
			{
				n_chk = AddItem(n_count, &fd);
				if (n_chk == FALSE) 
					break;
			}
			if(n_chk != READY){
				n_count++;
			}
        }
		::FindClose(hFind);
    }
	
	return n_count;
}

BOOL CScreen_Basic::AddItem(int nIndex, WIN32_FIND_DATA *pfd)
{
	CString str_temp_dvc;
	CString str_device;
	CString str_expansion;
	int n_pos;

	HICON	hIcon = NULL;

	str_temp_dvc = (pfd->cFileName);	// 파일 이름 설정

	str_temp_dvc.MakeUpper();
	
	/* ************************************************************************** */
	/* 파일 이름 중 확장자 확인                                                   */
	/* ************************************************************************** */
	n_pos = str_temp_dvc.Find('.');		// 확장자 위치 설정
	
	if (n_pos == -1)					// 확장자 미존재
		str_expansion = "";
	else								// 확장자 존재
		str_expansion = str_temp_dvc.Mid(n_pos + 1);
	/* ************************************************************************** */

	if (str_expansion != "TXT")
	{
		return READY;	
	}

	str_device = str_temp_dvc;			// 파일명 전체 화면 출력
	
	if (str_device == mstr_device_name[1])
	{
		mn_device_name = nIndex;
		Display_File(1, nIndex, str_device);
	}
	else
	{
		Display_File(0, nIndex, str_device);
	}
	/* ************************************************************************** */

    return TRUE;
}

int CScreen_Basic::OnBasic_New_Device_Check(CString str_device)
{
	CString str_chk_file;  // 생성할 [폴더]+[파일명] 저장 변수
	CString str_chk_ext;   // 확장자 저장 변수
	int n_pos;

	/* ************************************************************************** */
    /* 입력된 디바이스명 설정한다                                                 */
    /* ************************************************************************** */
	str_device.MakeUpper();
	str_device.TrimLeft(' ');               
	str_device.TrimRight(' ');

	if(str_device.IsEmpty())  
	{
		if (st_handler.cwnd_list != NULL)  // 리스트 바 화면 존재
		{
			st_other.str_normal_msg = _T("[DEVICE] A name input error occurrence.");
			sprintf(st_other.c_normal_msg, st_other.str_normal_msg);
			st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // 동작 실패 출력 요청
		}
		return FALSE;
	}
	/* ************************************************************************** */

	str_chk_file = _T("C:\\AMT820\\Motor\\") + str_device;  // 생성할 [폴더]+[파일명] 설정
	n_pos = str_chk_file.Find(".");  // 확장자 검사
	if (n_pos == -1) 
		str_chk_file += _T(".TXT");
	else 
	{
		str_chk_ext = str_chk_file.Mid(n_pos);  // 확장자 정보 설정
		if (str_chk_ext != _T(".TXT"))  
		{
			if (st_handler.cwnd_list != NULL)  // 리스트 바 화면 존재
			{
				st_other.str_normal_msg = _T("[DEVICE] A name extension input error occurrence.");
				sprintf(st_other.c_normal_msg, st_other.str_normal_msg);
				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // 동작 실패 출력 요청
			}
			return FALSE;
		}
	}

	mstr_load_file.Empty();

	mstr_load_file.Format("%s", str_chk_file);  // 생성할 [폴더]+[파일명]+[확장자] 설정

	return TRUE;
}



int CScreen_Basic::OnBasic_Input_Device_Check(CString str_device)
{
	CString str_chk_file;  /* 생성할 [폴더]+[파일명] 저장 변수 */
	CString str_chk_ext;   /* 확장자 저장 변수 */
	int n_pos;

	/* ************************************************************************** */
    /* 입력된 디바이스명 설정한다                                                 */
    /* ************************************************************************** */

	str_device.MakeUpper();
	str_device.TrimLeft(' ');               
	str_device.TrimRight(' ');

	if(str_device.IsEmpty())  
	{
		if(st_handler.cwnd_list != NULL)  // 리스트 바 화면 존재
		{
			st_other.str_op_msg = _T("[DEVICE] A name input error occurrence.");
			st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
		}
		return FALSE;
	}
	/* ************************************************************************** */

	str_chk_file = st_path.mstr_path_dvc + str_device;  // 생성할 [폴더]+[파일명] 설정
	n_pos = str_chk_file.Find(".");  // 확장자 검사
	if (n_pos == -1) 
		str_chk_file += _T(".TXT");
	else 
	{
		str_chk_ext = str_chk_file.Mid(n_pos);  // 확장자 정보 설정
		if (str_chk_ext != _T(".TXT"))  
		{
			if (st_handler.cwnd_list != NULL)  // 리스트 바 화면 존재
			{
				st_other.str_op_msg = _T("[DEVICE] A name extension input error occurrence.");
				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
			}
			return FALSE;
		}
	}

	mstr_load_file = str_chk_file;  // 생성할 [폴더]+[파일명]+[확장자] 설정

	return TRUE;
}



void CScreen_Basic::OnBasic_Data_HisoryLog()
{
	CString str_msg;

	if(mstr_device_name[0] != mstr_device_name[1])
	{
		str_msg.Format( "[BASIC] Model Change %s -> %s", mstr_device_name[0], mstr_device_name[1]);
		Func.On_LogFile_Add(0, str_msg);
	}

	if(m_n_mode_interface[0] != m_n_mode_interface[1])
	{
		str_msg.Format( "[BASIC] Interface Mode Change %d -> %d", m_n_mode_interface[0], m_n_mode_interface[1]);
		Func.On_LogFile_Add(0, str_msg);
	}

	if(m_n_mode_device[0] != m_n_mode_device[1])
	{
		str_msg.Format( "[BASIC] Device Mode Change %d -> %d", m_n_mode_device[0], m_n_mode_device[1]);
		Func.On_LogFile_Add(0, str_msg);
	}

	if(m_n_mode_retest[0] != m_n_mode_retest[1])
	{
		str_msg.Format( "[BASIC] Retest Mode Change %d -> %d", m_n_mode_retest[0], m_n_mode_retest[1]);
		Func.On_LogFile_Add(0, str_msg);
	}

	if(m_n_count_retry[0] != m_n_count_retry[1])
	{
		str_msg.Format( "[BASIC] Retry Count Change %d -> %d", m_n_count_retry[0], m_n_count_retry[1]);
		Func.On_LogFile_Add(0, str_msg);
	}
}

void CScreen_Basic::Init_Pass()
{ 

}

void CScreen_Basic::Init_Reject()
{

}

void CScreen_Basic::OnBtnBasicApply() 
{
	// TODO: Add your control notification handler code here
	int n_response;  //  대화 상자 리턴 플래그
	
	CDialog_Infor  info_dlg;
	
	st_other.str_confirm_msg = _T("Basic : Setting Data Apply!");
	
	n_response = info_dlg.DoModal();
	
	if (n_response == IDOK)
	{
		Data_Apply();

		OnBasic_Data_HisoryLog();
		Data_Backup();
	}
}




void CScreen_Basic::Data_Display()
{
/*	m_chk_work_mode.SetCheck(m_n_work_mode[1]);
	if(m_n_work_mode[1])
	{
		SetDlgItemText(IDC_CHK_WORK_MODE, "Dual");
	}
	else
	{
		SetDlgItemText(IDC_CHK_WORK_MODE, "Single");
	}

	m_chk_module.SetCheck(m_n_module[1]);
	if(m_n_module[1])
	{
		SetDlgItemText(IDC_CHK_MODULE_1, "UB Dimm");
	}
	else
	{
		SetDlgItemText(IDC_CHK_MODULE_1, "So Dimm");
	}

	m_chk_conveyor_move_mode.SetCheck(m_n_conveyor_mode[1]);
	if(m_n_conveyor_mode[1])
	{
		SetDlgItemText(IDC_CHK_CONVEYOR_MOVE_MODE, "Manual Move");
	}
	else
	{
		SetDlgItemText(IDC_CHK_CONVEYOR_MOVE_MODE, "Auto Move");
	}

	m_rad_turn_mode		= m_n_turn_mode[1];
	m_rad_device_mode	= m_n_device_mode[1];

	m_chk_length_first.SetCheck(m_n_first_length_measure[1]);
	if(m_n_first_length_measure[1])
	{
		SetDlgItemText(IDC_CHK_LENGTH_FIRST, "첫번째 측정");
	}
	else
	{
		SetDlgItemText(IDC_CHK_LENGTH_FIRST, "첫번째 미측정");
	}

	m_chk_length_second.SetCheck(m_n_second_length_measure[1]);
	if(m_n_second_length_measure[1])
	{
		SetDlgItemText(IDC_CHK_LENGTH_SECOND, "두번째 측정");
	}
	else
	{
		SetDlgItemText(IDC_CHK_LENGTH_SECOND, "두번째 미측정");
	}

	m_chk_length_third.SetCheck(m_n_third_length_measure[1]);
	if(m_n_third_length_measure[1])
	{
		SetDlgItemText(IDC_CHK_LENGTH_THIRD, "세번째 측정");
	}
	else
	{
		SetDlgItemText(IDC_CHK_LENGTH_THIRD, "세번째 미측정");
	}

	m_chk_thickness.SetCheck(m_n_thickness_measure[1]);
	if(m_n_thickness_measure[1])
	{
		SetDlgItemText(IDC_CHK_THICKNESS, "두께 측정");
	}
	else
	{
		SetDlgItemText(IDC_CHK_THICKNESS, "두께 미측정");
	}

	m_chk_barcode_read.SetCheck(m_n_barcode_read[1]);
	if(m_n_barcode_read[1])
	{
		SetDlgItemText(IDC_CHK_BARCODE_READ, "바코드 사용");
	}
	else
	{
		SetDlgItemText(IDC_CHK_BARCODE_READ, "바코드 미사용");
	}

	m_chk_air_blow.SetCheck(m_n_air_blow_mode[1]);
	if(m_n_air_blow_mode[1])
	{
		SetDlgItemText(IDC_CHK_AIR_BLOW, "동시 샤워");
	}
	else
	{
		SetDlgItemText(IDC_CHK_AIR_BLOW, "개별 샤워");
	}
	UpdateData(FALSE);

	m_dgtPass_Row.SetValue(m_nPass_Row[1]);
	m_dgtPass_Col.SetValue(m_nPass_Col[1]);

	m_dgt_retry.SetVal(m_n_retry_cnt[1]);
	m_dgt_pass_gap_x.SetFloatVal(m_f_pass_tray_gap_x[1]);
	m_dgt_pass_gap_y.SetFloatVal(m_f_pass_tray_gap_y[1]);
	m_dgt_reject_gap_x.SetFloatVal(m_f_reject_tray_gap_x[1]);
	m_dgt_air_blow_dn.SetVal(m_n_air_blow_dn[1]);
	m_dgt_air_blow_up.SetVal(m_n_air_blow_up[1]);
	m_dgt_air_blow_count.SetVal(m_n_air_blow_count[1]);
	m_dgt_router_wait_time.SetVal(m_n_router_wait_time[1]);
	m_dgt_separator_wait_time.SetVal(m_n_separator_wait_time[1]);
	m_dgt_module_wait_time.SetVal(m_n_module_wait_time[1]);
	m_dgt_conveyor_move_time.SetVal(m_n_conveyor_move_time[1]);*/
}

void CScreen_Basic::Init_Grid_File()
{
	int max_row, max_col;

	max_row = 0;
	max_col = 2;

	m_grid_file = (TSpread*)GetDlgItem(IDC_CUSTOM_FILE_LIST);
	
	m_p_grid.GridReset(m_grid_file);
	
	// 대문자 
	m_p_grid.GridCellSelectDisplay(m_grid_file, TRUE);
	m_p_grid.GridRowHeader(m_grid_file, FALSE);
	m_p_grid.GridColHeader(m_grid_file, TRUE);
	m_p_grid.GridHorizontal(m_grid_file, FALSE);
	m_p_grid.GridVertical(m_grid_file, TRUE);
	m_p_grid.GridAutoSize(m_grid_file, FALSE);
	m_p_grid.GridCellRows(m_grid_file, max_row);
	m_p_grid.GridCellCols(m_grid_file, max_col);
	
	m_p_grid.GridCellHeight_L(m_grid_file, 0, 20);
	m_p_grid.GridCellWidth_L(m_grid_file, 1, 5);
	m_p_grid.GridCellControlStatic(m_grid_file, 0, 1);
	m_p_grid.GridCellWidth_L(m_grid_file, 2, 27);
	m_p_grid.GridCellControlStatic(m_grid_file, 0, 2);

	m_p_grid.GridCellFont(m_grid_file, 0, 1, "MS Sans Serif", 12);
	m_p_grid.GridCellColor(m_grid_file, 0, 1, BLACK_L, YELLOW_C);
	m_p_grid.GridCellText(m_grid_file, 0, 1, "No.");

	m_p_grid.GridCellFont(m_grid_file, 0, 2, "MS Sans Serif", 12);
	m_p_grid.GridCellColor(m_grid_file, 0, 2, BLACK_L, YELLOW_C);
	m_p_grid.GridCellText(m_grid_file, 0, 2, "파일명");
}

void CScreen_Basic::Display_File(int n_mode, int n_count, CString str_filename)
{
	CString str_tmp;

	m_grid_file = (TSpread*)GetDlgItem(IDC_CUSTOM_FILE_LIST);
	m_p_grid.GridCellRows(m_grid_file, n_count+1);

	m_p_grid.GridCellHeight_L(m_grid_file, n_count+1, 20);
	m_p_grid.GridCellWidth_L(m_grid_file, 1, 5);
	m_p_grid.GridCellControlStatic(m_grid_file, n_count+1, 1);
	m_p_grid.GridCellWidth_L(m_grid_file, 2, 27);
	m_p_grid.GridCellControlStatic(m_grid_file, n_count+1, 2);

	if(n_mode == 0)
	{
		m_p_grid.GridCellFont(m_grid_file, n_count+1, 1, "MS Sans Serif", 12);
		m_p_grid.GridCellColor(m_grid_file, n_count+1, 1, WHITE_C, BLACK_L);
		str_tmp.Format("%d", n_count+1);
		m_p_grid.GridCellText(m_grid_file, n_count+1, 1, str_tmp);
		
		m_p_grid.GridCellFont(m_grid_file, n_count+1, 2, "MS Sans Serif", 12);
		m_p_grid.GridCellColor(m_grid_file, n_count+1, 2, WHITE_C, BLACK_L);
		m_p_grid.GridCellText(m_grid_file, n_count+1, 2, str_filename);
	}
	else
	{
		m_p_grid.GridCellFont(m_grid_file, n_count+1, 1, "MS Sans Serif", 12);
		m_p_grid.GridCellColor(m_grid_file, n_count+1, 1, GREEN_C, BLACK_L);
		str_tmp.Format("%d", n_count+1);
		m_p_grid.GridCellText(m_grid_file, n_count+1, 1, str_tmp);
		
		m_p_grid.GridCellFont(m_grid_file, n_count+1, 2, "MS Sans Serif", 12);
		m_p_grid.GridCellColor(m_grid_file, n_count+1, 2, GREEN_C, BLACK_L);
		m_p_grid.GridCellText(m_grid_file, n_count+1, 2, str_filename);

		m_n_filename_pos = n_count+1;
	}
}

void CScreen_Basic::OnBtnBasicCreate() 
{
	// TODO: Add your control notification handler code here
	int pos;
	CString str_tmp, str_filename;
	
	str_filename = GetEditString(mstr_device_name[1]);  // 키보드 상자 입력 정보 클래스 변수에 설정
	
	if(str_filename != "")
	{
		pos = -1;
		pos = str_filename.Find(".", 0);
		
		str_tmp = "";
		if(pos > 0)
		{
			str_tmp = str_filename.Mid(pos+1, 3);
		}
		
		if(str_tmp != "TXT")
		{
			str_filename += ".TXT";
		}
		
		if(OnBasic_New_Device_Check(str_filename) == FALSE)
		{
			return;
		}

		Data_Backup();

		mcls_m_basic.OnBasic_Data_Save_As(str_filename);
//		mcls_m_basic.On_Teach_Data_Save_As(str_filename);
		mcls_m_basic.OnMaintenance_Data_Save_As(str_filename);
		mcls_m_basic.OnInterface_Data_Save_As(str_filename);

		Init_Grid_File();
		OnBasic_Init_List(_T("C:\\AMT820\\motor\\"));// 파일 폴더 초기화 함수
	}
}

void CScreen_Basic::Init_Grid_Operate()
{
	CRect	area_p, area_r;
	int		x_p, y_p;
	int		width_p, height_p;
	int		cen_x_p, cen_y_p;
	int		i, j;
	int		max_row, max_col;
	CString str_tmp;

	m_n_operate_used[0] = YES;
	m_n_operate_used[1] = YES;
	m_n_operate_used[2] = YES;

	m_n_operate_cnt[0] = 3;
	m_n_operate_cnt[1] = 4;
	m_n_operate_cnt[2] = 4;

	max_row = m_n_operate_cnt[0] + m_n_operate_cnt[1] + m_n_operate_cnt[2];
	max_col = 1;
	
	m_grid_operate = (TSpread*)GetDlgItem(IDC_CUSTOM_OPERATE);
	
	m_p_grid.GridReset(m_grid_operate);
	
	// 대문자 
	m_p_grid.GridCellSelectDisplay(m_grid_operate, TRUE);
	m_p_grid.GridRowHeader(m_grid_operate, FALSE);
	m_p_grid.GridColHeader(m_grid_operate, FALSE);
	m_p_grid.GridHorizontal(m_grid_operate, FALSE);
	m_p_grid.GridVertical(m_grid_operate, FALSE);
	m_p_grid.GridAutoSize(m_grid_operate, FALSE);

	m_p_grid.GridCellRows(m_grid_operate, max_row);
	m_p_grid.GridCellCols(m_grid_operate, max_col);

	GetDlgItem(IDC_GROUP_OPERATE_METHOD)->GetWindowRect(area_p); // group box rect 구하기 
	ScreenToClient(area_p); // window상에서 group box 좌표 구하기
	
	width_p		= area_p.Width() - 15;
	height_p	= area_p.Height() - 30;
	
	x_p			= (width_p / max_col);
	y_p			= (height_p / max_row);
	
	// group box에서 grid 센터에 표시하기 위한 x좌표 구하기 
	cen_x_p = area_p.left + (((area_p.Width() - 15) - (x_p * max_col)) / 2) + 8;
	// group box에서 grid 센터에 표시하기 위한 y좌표 구하기 
	cen_y_p = area_p.top + (((area_p.Height() - 30) - (y_p * max_row)) / 2) + 20;
	
	// grid size 계산하여 중심에 다시 그리기 
	m_grid_operate->MoveWindow(cen_x_p, cen_y_p, x_p*(max_col), y_p*(max_row), TRUE);
	
	
	for(i=0; i<max_row+1; i++)
	{
		m_p_grid.GridCellHeight(m_grid_operate, i, y_p);
		
		for(j=0; j<max_col+1; j++)
		{	
			m_p_grid.GridCellWidth(m_grid_operate, j, x_p);
			m_p_grid.GridCellControlStatic(m_grid_operate, i, j);
		}
	}

	if(m_n_operate_used[0] == YES)
	{
		m_p_grid.GridCellFont(m_grid_operate, 1, 1, "MS Sans Serif", 18);
		m_p_grid.GridCellColor(m_grid_operate, 1, 1, BLACK_L, YELLOW_C);
		m_p_grid.GridCellText(m_grid_operate, 1, 1, "Interface 방법");
		
		m_p_grid.GridCellFont(m_grid_operate, 2, 1, "MS Sans Serif", 12);
		m_p_grid.GridCellText(m_grid_operate, 2, 1, "Off-Line");

		m_p_grid.GridCellFont(m_grid_operate, 3, 1, "MS Sans Serif", 12);
		m_p_grid.GridCellText(m_grid_operate, 3, 1, "On-Line");

		if(m_n_mode_interface[1] == 0)
		{	
			m_p_grid.GridCellColor(m_grid_operate, 2, 1, GREEN_C, BLACK_C);
			m_p_grid.GridCellColor(m_grid_operate, 3, 1, GREEN_D, BLACK_C);	
		}
		else
		{
			m_p_grid.GridCellColor(m_grid_operate, 2, 1, GREEN_D, BLACK_C);		
			m_p_grid.GridCellColor(m_grid_operate, 3, 1, GREEN_C, BLACK_C);
		}
		
		if(m_n_operate_used[1] == YES)
		{
			m_p_grid.GridCellFont(m_grid_operate, 4, 1, "MS Sans Serif", 18);
			m_p_grid.GridCellColor(m_grid_operate, 4, 1, BLACK_L, YELLOW_C);
			m_p_grid.GridCellText(m_grid_operate, 4, 1, "Device 방법");
			
			m_p_grid.GridCellFont(m_grid_operate, 5, 1, "MS Sans Serif", 12);
			m_p_grid.GridCellText(m_grid_operate, 5, 1, "With");

			m_p_grid.GridCellFont(m_grid_operate, 6, 1, "MS Sans Serif", 12);
			m_p_grid.GridCellText(m_grid_operate, 6, 1, "WithOut");

			m_p_grid.GridCellFont(m_grid_operate, 7, 1, "MS Sans Serif", 12);
			m_p_grid.GridCellText(m_grid_operate, 7, 1, "No Tray");

			if(m_n_mode_device[1] == 0)
			{
				m_p_grid.GridCellColor(m_grid_operate, 5, 1, GREEN_C, BLACK_C);
				m_p_grid.GridCellColor(m_grid_operate, 6, 1, GREEN_D, BLACK_C);
				m_p_grid.GridCellColor(m_grid_operate, 7, 1, GREEN_D, BLACK_C);
			}
			else if(m_n_mode_device[1] == 1)
			{
				m_p_grid.GridCellColor(m_grid_operate, 5, 1, GREEN_D, BLACK_C);
				m_p_grid.GridCellColor(m_grid_operate, 6, 1, GREEN_C, BLACK_C);
				m_p_grid.GridCellColor(m_grid_operate, 7, 1, GREEN_D, BLACK_C);
			}
			else if(m_n_mode_device[1] == 2)
			{
				m_p_grid.GridCellColor(m_grid_operate, 5, 1, GREEN_D, BLACK_C);
				m_p_grid.GridCellColor(m_grid_operate, 6, 1, GREEN_D, BLACK_C);
				m_p_grid.GridCellColor(m_grid_operate, 7, 1, GREEN_C, BLACK_C);
			}

			if(m_n_operate_used[2] == YES)
			{
				m_p_grid.GridCellFont(m_grid_operate, 8, 1, "MS Sans Serif", 18);
				m_p_grid.GridCellColor(m_grid_operate, 8, 1, BLACK_L, YELLOW_C);
				m_p_grid.GridCellText(m_grid_operate, 8, 1, "Retest 방법");
				
				m_p_grid.GridCellFont(m_grid_operate, 9, 1, "MS Sans Serif", 12);
				m_p_grid.GridCellText(m_grid_operate, 9, 1, "Retest Off");

				m_p_grid.GridCellFont(m_grid_operate, 10, 1, "MS Sans Serif", 12);
				m_p_grid.GridCellText(m_grid_operate, 10, 1, "Retest On");

				if(m_n_mode_retest[1] == 0)
				{
					m_p_grid.GridCellColor(m_grid_operate, 9, 1, GREEN_C, BLACK_C);
					m_p_grid.GridCellColor(m_grid_operate, 10, 1, GREEN_D, BLACK_C);
				}
				else
				{
					m_p_grid.GridCellColor(m_grid_operate, 9, 1, GREEN_D, BLACK_C);
					m_p_grid.GridCellColor(m_grid_operate, 10, 1, GREEN_C, BLACK_C);
				}
				
				m_p_grid.GridCellFont(m_grid_operate, 11, 1, "MS Sans Serif", 20);
				m_p_grid.GridCellColor(m_grid_operate, 11, 1, WHITE_C, BLACK_C);
				str_tmp.Format("%d", m_n_count_retry[1]);
				m_p_grid.GridCellText(m_grid_operate, 11, 1, str_tmp);
			}
		}
	}
	else
	{
		if(m_n_operate_used[1] == YES)
		{
			m_p_grid.GridCellFont(m_grid_operate, 1, 1, "MS Sans Serif", 18);
			m_p_grid.GridCellColor(m_grid_operate, 1, 1, BLACK_L, YELLOW_C);
			m_p_grid.GridCellText(m_grid_operate, 1, 1, "Device 방법");
		
			m_p_grid.GridCellFont(m_grid_operate, 2, 1, "MS Sans Serif", 12);
			m_p_grid.GridCellText(m_grid_operate, 2, 1, "With");

			m_p_grid.GridCellFont(m_grid_operate, 3, 1, "MS Sans Serif", 12);
			m_p_grid.GridCellText(m_grid_operate, 3, 1, "WithOut");

			m_p_grid.GridCellFont(m_grid_operate, 4, 1, "MS Sans Serif", 12);
			m_p_grid.GridCellText(m_grid_operate, 4, 1, "No Tray");

			if(m_n_mode_device[1] == 0)
			{
				m_p_grid.GridCellColor(m_grid_operate, 2, 1, GREEN_C, BLACK_C);
				m_p_grid.GridCellColor(m_grid_operate, 3, 1, GREEN_D, BLACK_C);
				m_p_grid.GridCellColor(m_grid_operate, 4, 1, GREEN_D, BLACK_C);
			}
			else if(m_n_mode_device[1] == 1)
			{
				m_p_grid.GridCellColor(m_grid_operate, 2, 1, GREEN_D, BLACK_C);
				m_p_grid.GridCellColor(m_grid_operate, 3, 1, GREEN_C, BLACK_C);
				m_p_grid.GridCellColor(m_grid_operate, 4, 1, GREEN_D, BLACK_C);
			}
			else if(m_n_mode_device[1] == 2)
			{
				m_p_grid.GridCellColor(m_grid_operate, 2, 1, GREEN_D, BLACK_C);
				m_p_grid.GridCellColor(m_grid_operate, 3, 1, GREEN_D, BLACK_C);
				m_p_grid.GridCellColor(m_grid_operate, 4, 1, GREEN_C, BLACK_C);
			}
			
			if(m_n_operate_used[2] == YES)
			{
				m_p_grid.GridCellFont(m_grid_operate, 5, 1, "MS Sans Serif", 18);
				m_p_grid.GridCellColor(m_grid_operate, 5, 1, BLACK_L, YELLOW_C);
				m_p_grid.GridCellText(m_grid_operate, 5, 1, "Retest 방법");
				
				m_p_grid.GridCellFont(m_grid_operate, 6, 1, "MS Sans Serif", 12);
				m_p_grid.GridCellText(m_grid_operate, 6, 1, "Retest Off");

				m_p_grid.GridCellFont(m_grid_operate, 7, 1, "MS Sans Serif", 12);
				m_p_grid.GridCellText(m_grid_operate, 7, 1, "Retest On");

				if(m_n_mode_retest[1]== 0)
				{
					m_p_grid.GridCellColor(m_grid_operate, 6, 1, GREEN_C, BLACK_C);
					m_p_grid.GridCellColor(m_grid_operate, 7, 1, GREEN_D, BLACK_C);
				}
				else
				{
					m_p_grid.GridCellColor(m_grid_operate, 6, 1, GREEN_D, BLACK_C);
					m_p_grid.GridCellColor(m_grid_operate, 7, 1, GREEN_C, BLACK_C);
				}
				
				m_p_grid.GridCellFont(m_grid_operate, 8, 1, "MS Sans Serif", 20);
				m_p_grid.GridCellColor(m_grid_operate, 8, 1, WHITE_C, BLACK_C);
				str_tmp.Format("%d", m_n_count_retry[1]);
				m_p_grid.GridCellText(m_grid_operate, 8, 1, str_tmp);
			}
		}
	}
}

void CScreen_Basic::OnBtnDelete() 
{
	// TODO: Add your control notification handler code here
	CDialog_Select	select_dlg;
	CString			str_tmp;
	CString			str_filename;
	int				n_response;
	int				n_existence;			// 파일 존재 유무 플래그

	m_grid_file = (TSpread *) GetDlgItem(IDC_CUSTOM_FILE_LIST);

	str_tmp = mstr_device_name[1];
	if(OnBasic_Input_Device_Check(str_tmp) == FALSE)  // 입력된 디바이스 이름 정상 유무 검사 함수
	{
		return ;
	}
	
	st_other.str_confirm_msg = _T(str_tmp + " 모델 삭제하시겠습니까?..");
	
	n_response = select_dlg.DoModal();
	
	if(n_response == IDOK)
	{
		if(str_tmp == mstr_device_name[1])
		{
			mstr_device_name[0] = mstr_device_name[1] = m_p_grid.GridCellText(m_grid_file, 1, 2);
			if(mstr_device_name[1] == str_tmp)
			{
				mstr_device_name[1] = "";
				mstr_device_name[0] = mstr_device_name[1] = m_p_grid.GridCellText(m_grid_file, 2, 2);
			}

		}
		
		str_filename = st_path.mstr_basic + str_tmp;
		
		n_existence = access(str_filename, 0) ;
		
		if (n_existence != -1)			// 파일 존재
		{
			DeleteFile(str_filename);  // 해당 파일 삭제
			
			Init_Grid_File();
			OnBasic_Init_List(st_path.mstr_basic);// 파일 폴더 초기화 함수
		}
		else  
		{
			return ;
		}
	}
}
