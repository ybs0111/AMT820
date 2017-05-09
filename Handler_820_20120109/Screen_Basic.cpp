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
#include "AMTLotManager.h" //kwlee 2017.0403
#include "Public_Function.h"
#include "Dialog_Infor.h"
#include "SrcPart/APartHandler.h"
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
	DDX_Control(pDX, IDC_DGT_HARDNESS_TIME, m_dgt_hardness_time);
	DDX_Control(pDX, IDC_DGT_EPOXY_CLEAN_TIME, m_dgt_epoxy_clean_time);
	DDX_Control(pDX, IDC_GROUP_TRAY_Y, m_group_Tray_Y);
	DDX_Control(pDX, IDC_GROUP_TRAY_X, m_group_Tray_X);
	DDX_Control(pDX, IDC_GROUP_HSTRAY_Y, m_group_HsTray_Y);
	DDX_Control(pDX, IDC_GROUP_HSTRAY_X, m_group_HsTrayX);
	DDX_Control(pDX, IDC_GROUP_DISPENSERVPPMA, m_group_DispenserVppmA);
	DDX_Control(pDX, IDC_DGT_TRAY_Y, m_dgt_Tray_Y);
	DDX_Control(pDX, IDC_DGT_TRAY_X, m_dgt_Tray_X);
	DDX_Control(pDX, IDC_DGT_HSTRAY_Y, m_dgt_HsTray_Y);
	DDX_Control(pDX, IDC_DGT_HSTRAY_X, m_dgt_HsTray_X);
	DDX_Control(pDX, IDC_DGT_DISPENSERVPPMA, m_dgt_DispenserVppmA);
	DDX_Control(pDX, IDC_DGT_EPOXY_SCREW_CNT, m_dgt_EpoxyScrewCnt);
	DDX_Control(pDX, IDC_GROUP_EPOXY_SCREW_CNT, m_group_EpoxyScrewCnt);
	DDX_Control(pDX, IDC_GROUP_RETRY_CNT, m_group_Retry_Cnt);
	DDX_Control(pDX, IDC_DGT_RETRY_CNT, m_dgt_Retry_Cnt);
	DDX_Control(pDX, IDC_DGT_RUBB, m_dgt_Rubb);
	DDX_Control(pDX, IDC_DGT_PICK_RETRY_CNT, m_dgt_Pick_Retry_Cnt);
	DDX_Control(pDX, IDC_DGT_PARTIAL_CNT, m_dgt_Partial_Cnt);
<<<<<<< HEAD
=======
	
>>>>>>> c6e69b6ca871ea7a83253cb4bb4092c82b1ae2a4
	DDX_Control(pDX, IDC_DGT_EPOXY_Y, m_dgt_Epoxy_Y);
	DDX_Control(pDX, IDC_DGT_EPOXY_X, m_dgt_Epoxy_X);
	DDX_Control(pDX, IDC_DGT_ELV_PARTIAL_CNT, m_dgt_Elv_Partial_Cnt);
	DDX_Control(pDX, IDC_DGT_7387_ALARM_CNT, m_dgt_7387_Alarm_Cnt);
	DDX_Control(pDX, IDC_MSG_XPOXY_Y, m_msg_Epoxy_Y);
<<<<<<< HEAD
=======
	
>>>>>>> c6e69b6ca871ea7a83253cb4bb4092c82b1ae2a4
	DDX_Control(pDX, IDC_MSG_EPOXY_X, m_msg_Epoxy_X);
	DDX_Control(pDX, IDC_GROUP_RUBB_CNT, m_group_Rubb_Cnt);
	DDX_Control(pDX, IDC_GROUP_PARTIAL_CNT, m_group_Patial_Cnt);
	DDX_Control(pDX, IDC_GROUP_EPOXY_OFFSET, m_group_Epoxy_Offset);
	DDX_Control(pDX, IDC_GROUP_ELV_PARTIAL_CNT, m_group_Elv_Partial_Cnt);
<<<<<<< HEAD
=======
	
>>>>>>> c6e69b6ca871ea7a83253cb4bb4092c82b1ae2a4
	DDX_Control(pDX, IDC_GROUP_7387_ALARM_CNT, m_group_7387_Alarm_Cnt);
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
	ON_BN_CLICKED(IDC_DGT_EPOXY_X, OnDgtEpoxyX)
	ON_BN_CLICKED(IDC_DGT_EPOXY_Y, OnDgtEpoxyY)
<<<<<<< HEAD
=======
	
>>>>>>> c6e69b6ca871ea7a83253cb4bb4092c82b1ae2a4
	ON_BN_CLICKED(IDC_DGT_RUBB, OnDgtRubb)
	ON_BN_CLICKED(IDC_DGT_7387_ALARM_CNT, OnDgt7387AlarmCnt)
	ON_BN_CLICKED(IDC_DGT_ELV_PARTIAL_CNT, OnDgtElvPartialCnt)
	ON_BN_CLICKED(IDC_DGT_PARTIAL_CNT, OnDgtPartialCnt)
	ON_BN_CLICKED(IDC_DGT_RETRY_CNT, OnDgtRetryCnt)
	ON_BN_CLICKED(IDC_DGT_EPOXY_SCREW_CNT, OnDgtEpoxyScrewCnt)
	ON_BN_CLICKED(IDC_DGT_HSTRAY_Y, OnDgtHstrayY)
	ON_BN_CLICKED(IDC_DGT_HSTRAY_X, OnDgtHstrayX)
	ON_BN_CLICKED(IDC_DGT_TRAY_Y, OnDgtTrayY)
	ON_BN_CLICKED(IDC_DGT_TRAY_X, OnDgtTrayX)
	ON_BN_CLICKED(IDC_DGT_DISPENSERVPPMA, OnDgtDispenservppmA)
	ON_BN_CLICKED(IDC_DGT_EPOXY_CLEAN_TIME, OnDgtEpoxyCleanTime)
	ON_BN_CLICKED(IDC_BUTTON_CLEAN_RESET, OnButtonCleanReset)
	ON_BN_CLICKED(IDC_BUTTON_HARDNESS_RESET, OnButtonHardnessReset)
	ON_BN_CLICKED(IDC_DGT_HARDNESS_TIME, OnDgtHardnessTime)
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
	OnBasic_Data_Set();

	OnBasic_Data_Button();
	OnBasic_Data_Group();
	OnBasic_Digital();
	OnBasic_Data_Label();
	OnBasic_Data_EditBox_Set();

	Init_Grid_File();
	Init_Grid_Operate();
	Init_Grid_Operate_1();
	OnBasic_Data_Display();
	OnBasic_Init_List(st_path.mstr_path_Model);  // 파일 폴더 초기화 함수

	/* ************************************************************************** */

	st_handler.cwnd_basic = this;  // BASIC 화면에 대한 핸들 정보 설정

	UpdateData(FALSE);
}

void CScreen_Basic::OnDestroy() 
{
	if( mp_basic_font != NULL )
	{
		delete mp_basic_font;
		mp_basic_font = NULL;
	}

	st_handler.cwnd_basic = NULL;

	CFormView::OnDestroy();
}

void CScreen_Basic::OnCell_L_Click(WPARAM wParam, LPARAM lParam)
{
	LPSS_CELLCOORD lpcc = (LPSS_CELLCOORD)lParam;
	CDialog_Message msg_dlg;
	CDialog_Select	select_dlg;
	CString			str_tmp;
	int				n_response;
	int             nCnt;
	
	CString mstr_temp;  // 저장할 정보 임시 저장 변수 
//	char chr_buf[20] ;
// 	double md_Temp;

	nCnt = lpcc->Row;

	//kwlee 2017.0403
// 	if (nCnt <= 0 || g_lotMgr.GetLotCount() > 0 )
// 	{
// 		return;
// 	}
	///

	if(wParam == IDC_CUSTOM_FILE_LIST)
	{
		TSpread* m_grid_file;
		m_grid_file = (TSpread *) GetDlgItem(IDC_CUSTOM_FILE_LIST);

		if (st_work.b_lot_start_flag == true)
		{
			//st_other.str_fallacy_msg = _T("It is available after Lot End.");
			//kwlee 2017.0421
			st_other.str_confirm_msg = _T("It is available after Lot End.");
			n_response = msg_dlg.DoModal();
			
			if (n_response == IDOK)
			{
				return ;
			}
			
		}

		
		//mstr_device_name[1] = m_p_grid.GridCellText(m_grid_file, lpcc->Row, lpcc->Col);
		//kwlee 2017.0403
		char ch_temp[16];
		m_grid_file->GetData(lpcc->Col,lpcc->Row,ch_temp);
		mstr_device_name[1] = ch_temp;
		
// 		if(OnBasic_Input_Device_Check(mstr_device_name[1]) == FALSE)  // 입력된 디바이스 이름 정상 유무 검사 함수
// 		{
// 			mstr_device_name[1] = mstr_device_name[0];
// 			return ;
// 		}

		st_other.str_confirm_msg = _T(mstr_device_name[1] + " 모델을 시작하시겠습니까?..");
		
		n_response = select_dlg.DoModal();
		
		if(n_response == IDOK)
		{
			//kwlee 2017.0403
			if(OnBasic_Input_Device_Check(mstr_device_name[1]) == FALSE)  // 입력된 디바이스 이름 정상 유무 검사 함수
			{
				mstr_device_name[1] = mstr_device_name[0];
				return;
			}

			st_basic.mstr_device_name = mstr_device_name[1];

			
			mcls_m_basic.OnBasic_Data_Load();
			mcls_m_basic.On_Teach_Data_Load();
			mcls_m_basic.OnMaintenance_Data_Load();
			mcls_m_basic.OnWaitTime_Data_Load();
			mcls_m_basic.OnMotorSpeed_Set_Data_Load();
			mcls_m_basic.OnInterface_Data_Load();
			
			 //Data_Init();
			
			m_p_grid.GridCellColor(m_grid_file, m_n_filename_pos, 1, WHITE_C, BLACK_C);
			m_p_grid.GridCellColor(m_grid_file, m_n_filename_pos, 2, WHITE_C, BLACK_C);

			m_n_filename_pos = lpcc->Row;
			m_p_grid.GridCellColor(m_grid_file, m_n_filename_pos, 1, GREEN_C, BLACK_C);
			m_p_grid.GridCellColor(m_grid_file, m_n_filename_pos, 2, GREEN_C, BLACK_C);

			//kwlee 2017.0403
			m_p_grid.GridCellColor(m_grid_file, 0, 1, BLACK_L, YELLOW_C);
			m_p_grid.GridCellColor(m_grid_file, 0, 2, BLACK_L, YELLOW_C);
			if(st_handler.cwnd_title != NULL)
			{
				st_handler.cwnd_title->PostMessage(WM_STATUS_CHANGE, FILE_MODE, 0);
			}
			Invalidate(FALSE);

		}

		m_grid_file = NULL;
		delete m_grid_file;
		
	}
	else if(wParam == IDC_CUSTOM_OPERATE)
	{
		TSpread *m_grid_operate;
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
				
				//kwlee 2017.0405

// 			case 11:
// 				str_tmp.Format("%d", m_n_count_retry[1]);
// 				str_tmp = GetNumEditString_I((double)0, (double)5, str_tmp);
// 				m_p_grid.GridCellText(m_grid_operate, lpcc->Row, lpcc->Col, str_tmp);
// 				m_n_count_retry[1] = atoi(str_tmp);
// 				break;

			case 12:
				if(m_nModeTester[1] == 0) return;
				
				m_p_grid.GridCellColor(m_grid_operate, 12, lpcc->Col, GREEN_C, BLACK_C);
				m_p_grid.GridCellColor(m_grid_operate, 13, lpcc->Col, GREEN_D, BLACK_C);
				m_nModeTester[1] = 0;
				break;
				
			case 13:
				if(m_nModeTester[1] == 1) return;
				
				m_p_grid.GridCellColor(m_grid_operate, 12, lpcc->Col, GREEN_D, BLACK_C);
				m_p_grid.GridCellColor(m_grid_operate, 13, lpcc->Col, GREEN_C, BLACK_C);
				m_nModeTester[1] = 1;
				break;
			
			case 15:
				if(m_nModeBcr[1] == 0) return;
				
				m_p_grid.GridCellColor(m_grid_operate, 15, lpcc->Col, GREEN_C, BLACK_C);
				m_p_grid.GridCellColor(m_grid_operate, 16, lpcc->Col, GREEN_D, BLACK_C);
				m_nModeBcr[1] = 0;
				break;
				
			case 16:
				if(m_nModeBcr[1] == 1) return;
				
				m_p_grid.GridCellColor(m_grid_operate, 15, lpcc->Col, GREEN_D, BLACK_C);
				m_p_grid.GridCellColor(m_grid_operate, 16, lpcc->Col, GREEN_C, BLACK_C);
				m_nModeBcr[1] = 1;
				break;
				
			case 18:
				if(m_nModeModule[1] == 0) return;
				
				m_p_grid.GridCellColor(m_grid_operate, 18, lpcc->Col, GREEN_C, BLACK_C);
				m_p_grid.GridCellColor(m_grid_operate, 19, lpcc->Col, GREEN_D, BLACK_C);
				m_nModeModule[1] = 0;
				break;
				
			case 19:
				if(m_nModeModule[1] == 1) return;
				
				m_p_grid.GridCellColor(m_grid_operate, 18, lpcc->Col, GREEN_D, BLACK_C);
				m_p_grid.GridCellColor(m_grid_operate, 19, lpcc->Col, GREEN_C, BLACK_C);
				m_nModeModule[1] = 1;
				break;

			case 21:
				if(m_nmode_7387[1] == 0) return;
				
				m_p_grid.GridCellColor(m_grid_operate, 21, lpcc->Col, GREEN_C, BLACK_C);
				m_p_grid.GridCellColor(m_grid_operate, 22, lpcc->Col, GREEN_D, BLACK_C);
				m_nmode_7387[1] = 0;
				break;
				
			case 22:
				if(m_nmode_7387[1] == 1) return;
				
				m_p_grid.GridCellColor(m_grid_operate, 21, lpcc->Col, GREEN_D, BLACK_C);
				m_p_grid.GridCellColor(m_grid_operate, 22, lpcc->Col, GREEN_C, BLACK_C);
				m_nmode_7387[1] = 1;
				break;

			case 24:
				if(m_nmode_tray_type[1] == 0) return;
				
				m_p_grid.GridCellColor(m_grid_operate, 24, lpcc->Col, GREEN_C, BLACK_C);
				m_p_grid.GridCellColor(m_grid_operate, 25, lpcc->Col, GREEN_D, BLACK_C);
				m_nmode_tray_type[1] = 0;
				break;
				
			case 25:
				if(m_nmode_tray_type[1] == 1) return;
				
				m_p_grid.GridCellColor(m_grid_operate, 24, lpcc->Col, GREEN_D, BLACK_C);
				m_p_grid.GridCellColor(m_grid_operate, 25, lpcc->Col, GREEN_C, BLACK_C);
				m_nmode_tray_type[1] = 1;
				break;

			case 27:
				if(m_n7387InspectAlarmSkipMode[1] == 0) return;
				
				m_p_grid.GridCellColor(m_grid_operate, 27, lpcc->Col, GREEN_C, BLACK_C);
				m_p_grid.GridCellColor(m_grid_operate, 28, lpcc->Col, GREEN_D, BLACK_C);
				m_n7387InspectAlarmSkipMode[1] = 0;
				break;
				
			case 28:
				if(m_n7387InspectAlarmSkipMode[1] == 1) return;
				
				m_p_grid.GridCellColor(m_grid_operate, 27, lpcc->Col, GREEN_D, BLACK_C);
				m_p_grid.GridCellColor(m_grid_operate, 28, lpcc->Col, GREEN_C, BLACK_C);
				m_n7387InspectAlarmSkipMode[1] = 1;
				break;


		}
		m_grid_operate = NULL;
		delete m_grid_operate;
	}
	//kwlee 2017.0405
	else if(wParam == IDC_CUSTOM_OPERATE2)
	{

// 		if (nCnt >= 10)
// 		{
// 			return;
// 		}

		TSpread* m_grid_operate;

		m_grid_operate = (TSpread*) GetDlgItem(IDC_CUSTOM_OPERATE2);
		
		switch(lpcc->Row)
		{
		case 2:
			if(m_n3874InspectMode[1] == 0) return;
			
			m_p_grid.GridCellColor(m_grid_operate, 2, lpcc->Col, GREEN_C, BLACK_C);
			m_p_grid.GridCellColor(m_grid_operate, 3, lpcc->Col, GREEN_D, BLACK_C);
			m_n3874InspectMode[1] = 0;
			break;
			
		case 3:
			if(m_n3874InspectMode[1] == 1) return;
			
			m_p_grid.GridCellColor(m_grid_operate, 2, lpcc->Col, GREEN_D, BLACK_C);
			m_p_grid.GridCellColor(m_grid_operate, 3, lpcc->Col, GREEN_C, BLACK_C);
			m_n3874InspectMode[1] = 1;
			break;
			
		case 5:
			if(n_Light_Curtain_Mode[1] == 0) return;
			
			m_p_grid.GridCellColor(m_grid_operate, 5, lpcc->Col, GREEN_C, BLACK_C);
			m_p_grid.GridCellColor(m_grid_operate, 6, lpcc->Col, GREEN_D, BLACK_C);
		//	m_p_grid.GridCellColor(m_grid_operate, 7, lpcc->Col, GREEN_D, BLACK_C);
			n_Light_Curtain_Mode[1] = 0;
			break;
			
		case 6:
			if(n_Light_Curtain_Mode[1] == 1) return;
			
			m_p_grid.GridCellColor(m_grid_operate, 5, lpcc->Col, GREEN_D, BLACK_C);
			m_p_grid.GridCellColor(m_grid_operate, 6, lpcc->Col, GREEN_C, BLACK_C);
		//	m_p_grid.GridCellColor(m_grid_operate, 7, lpcc->Col, GREEN_D, BLACK_C);
			n_Light_Curtain_Mode[1] = 1;
			break;
			
// 		case 7:
// 			if(m_n_mode_device[1] == 2) return;
// 			
// 			m_p_grid.GridCellColor(m_grid_operate, 5, lpcc->Col, GREEN_D, BLACK_C);
// 			m_p_grid.GridCellColor(m_grid_operate, 6, lpcc->Col, GREEN_D, BLACK_C);
// 			m_p_grid.GridCellColor(m_grid_operate, 7, lpcc->Col, GREEN_C, BLACK_C);
// 			m_n_mode_device[1] = 2;
// 			break;
			
		case 8:
			if(n_mode_case_assembly[1] == 0) return;
			
			m_p_grid.GridCellColor(m_grid_operate, 8, lpcc->Col, GREEN_C, BLACK_C);
			m_p_grid.GridCellColor(m_grid_operate, 9, lpcc->Col, GREEN_D, BLACK_C);
			n_mode_case_assembly[1] = 0;
			break;
			
		case 9:
			if(n_mode_case_assembly[1] == 1) return;
			
			m_p_grid.GridCellColor(m_grid_operate, 8, lpcc->Col, GREEN_D, BLACK_C);
			m_p_grid.GridCellColor(m_grid_operate, 9, lpcc->Col, GREEN_C, BLACK_C);
			n_mode_case_assembly[1] = 1;
			break;

			//kwlee 2017.0411
		case 11:
			if(m_n3874InspectAlarmSkipMode[1] == 0) return;
			
			m_p_grid.GridCellColor(m_grid_operate, 11, lpcc->Col, GREEN_C, BLACK_C);
			m_p_grid.GridCellColor(m_grid_operate, 12, lpcc->Col, GREEN_D, BLACK_C);
			m_n3874InspectAlarmSkipMode[1] = 0;
			break;
			
		case 12:
			if(m_n3874InspectAlarmSkipMode[1] == 1) return;
			
			m_p_grid.GridCellColor(m_grid_operate, 11, lpcc->Col, GREEN_D, BLACK_C);
			m_p_grid.GridCellColor(m_grid_operate, 12, lpcc->Col, GREEN_C, BLACK_C);
			m_n3874InspectAlarmSkipMode[1] = 1;
			break;



		default:
			break;
		}
		m_grid_operate = NULL;
		delete m_grid_operate;
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
		TSpread* m_grid_file;
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
		m_grid_file = NULL;
		delete m_grid_file;
	}
}
void CScreen_Basic::OnBasic_Data_Button()
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
}

void CScreen_Basic::OnBasic_Data_Group()
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

	//kwlee 2017.0405
	m_group_Rubb_Cnt.SetFont(main_font);
	m_group_Rubb_Cnt.SetCatptionTextColor(RGB(145,25,0));
	m_group_Rubb_Cnt.SetFontBold(TRUE);
	
// 	m_group_Pick_Retry_Cnt.SetFont(main_font);
// 	m_group_Pick_Retry_Cnt.SetCatptionTextColor(RGB(145,25,0));
// 	m_group_Pick_Retry_Cnt.SetFontBold(TRUE);
	
	m_group_Patial_Cnt.SetFont(main_font);
	m_group_Patial_Cnt.SetCatptionTextColor(RGB(145,25,0));
	m_group_Patial_Cnt.SetFontBold(TRUE);
	
	m_group_Epoxy_Offset.SetFont(main_font);
	m_group_Epoxy_Offset.SetCatptionTextColor(RGB(145,25,0));
	m_group_Epoxy_Offset.SetFontBold(TRUE);
	
	m_group_Elv_Partial_Cnt.SetFont(main_font);
	m_group_Elv_Partial_Cnt.SetCatptionTextColor(RGB(145,25,0));
	m_group_Elv_Partial_Cnt.SetFontBold(TRUE);
	
	m_group_7387_Alarm_Cnt.SetFont(main_font);
	m_group_7387_Alarm_Cnt.SetCatptionTextColor(RGB(145,25,0));
	m_group_7387_Alarm_Cnt.SetFontBold(TRUE);
	
	m_group_Retry_Cnt.SetFont(main_font);
	m_group_Retry_Cnt.SetCatptionTextColor(RGB(145,25,0));
	m_group_Retry_Cnt.SetFontBold(TRUE);

	m_group_EpoxyScrewCnt.SetFont(main_font);
	m_group_EpoxyScrewCnt.SetCatptionTextColor(RGB(145,25,0));
	m_group_EpoxyScrewCnt.SetFontBold(TRUE);

	//kwlee 2017.0406
	m_group_HsTray_Y.SetFont(main_font);
	m_group_HsTray_Y.SetCatptionTextColor(RGB(145,25,0));
	m_group_HsTray_Y.SetFontBold(TRUE);
	
	m_group_HsTrayX.SetFont(main_font);
	m_group_HsTrayX.SetCatptionTextColor(RGB(145,25,0));
	m_group_HsTrayX.SetFontBold(TRUE);
	
	m_group_Tray_X.SetFont(main_font);
	m_group_Tray_X.SetCatptionTextColor(RGB(145,25,0));
	m_group_Tray_X.SetFontBold(TRUE);
	
	m_group_Tray_Y.SetFont(main_font);
	m_group_Tray_Y.SetCatptionTextColor(RGB(145,25,0));
	m_group_Tray_Y.SetFontBold(TRUE);
	
	m_group_DispenserVppmA.SetFont(main_font);
	m_group_DispenserVppmA.SetCatptionTextColor(RGB(145,25,0));
	m_group_DispenserVppmA.SetFontBold(TRUE);
	


}


void CScreen_Basic::OnBasic_Data_EditBox_Set()
{
	m_edit_device_type.SubclassDlgItem(IDC_EDIT_DEVICE_TYPE, this);
	m_edit_device_type.bkColor(RGB(50, 100, 150));
	m_edit_device_type.textColor(RGB(255, 255,255));
	m_edit_device_type.setFont(-16, FW_BOLD, DEFAULT_PITCH | FF_DONTCARE, "Arial");
}


///
void CScreen_Basic::OnBasic_Digital()
{

	CString strTmp;

	strTmp.Format("%d",n_rubb_count[1]);
	m_dgt_Rubb.SubclassDlgItem(IDC_DGT_RUBB, this);
	m_dgt_Rubb.SetStyle(IDB_BIG3, 2);
	m_dgt_Rubb.SetValue(strTmp);
	
// 	strTmp.Format("%d",m_nCountPickRetry[1]);
// 	m_dgt_Pick_Retry_Cnt.SubclassDlgItem(IDC_DGT_PICK_RETRY_CNT, this);
// 	m_dgt_Pick_Retry_Cnt.SetStyle(IDB_BIG3, 2);
// 	m_dgt_Pick_Retry_Cnt.SetValue(strTmp);
	
	strTmp.Format("%d",m_nCountPartial[1]);
	m_dgt_Partial_Cnt.SubclassDlgItem(IDC_DGT_PARTIAL_CNT, this);
	m_dgt_Partial_Cnt.SetStyle(IDB_BIG3, 2);
	m_dgt_Partial_Cnt.SetValue(strTmp);
	
	
	strTmp.Format("%0.2f",dEpoxyYLineOffSet[1]);
	m_dgt_Epoxy_Y.SubclassDlgItem(IDC_DGT_EPOXY_Y, this);
	m_dgt_Epoxy_Y.SetStyle(IDB_BIG3, 6);
	m_dgt_Epoxy_Y.SetValue(strTmp);
	
	strTmp.Format("%0.2f",dEpoxyXLineOffSet[1]);
	m_dgt_Epoxy_X.SubclassDlgItem(IDC_DGT_EPOXY_X, this);
	m_dgt_Epoxy_X.SetStyle(IDB_BIG3, 6);
	m_dgt_Epoxy_X.SetValue(strTmp);
	
	strTmp.Format("%d",m_nCount_elevator_Partial[1]);
	m_dgt_Elv_Partial_Cnt.SubclassDlgItem(IDC_DGT_ELV_PARTIAL_CNT, this);
	m_dgt_Elv_Partial_Cnt.SetStyle(IDB_BIG3, 2);
	m_dgt_Elv_Partial_Cnt.SetValue(strTmp);
	
	strTmp.Format("%d",m_n7387AlarmCount[1]);
	m_dgt_7387_Alarm_Cnt.SubclassDlgItem(IDC_DGT_7387_ALARM_CNT, this);
	m_dgt_7387_Alarm_Cnt.SetStyle(IDB_BIG3, 2);
	m_dgt_7387_Alarm_Cnt.SetValue(strTmp);

	strTmp.Format("%d",m_n_count_retry[1]);
	m_dgt_Retry_Cnt.SubclassDlgItem(IDC_DGT_RETRY_CNT, this);
	m_dgt_Retry_Cnt.SetStyle(IDB_BIG3, 2);
	m_dgt_Retry_Cnt.SetValue(strTmp);
	
	strTmp.Format("%d",m_nEpoxyDotScrewCount[1]);
	m_dgt_EpoxyScrewCnt.SubclassDlgItem(IDC_DGT_EPOXY_SCREW_CNT, this);
	m_dgt_EpoxyScrewCnt.SetStyle(IDB_BIG3, 2);
	m_dgt_EpoxyScrewCnt.SetValue(strTmp);
	
	//kwlee 2017.0406
	strTmp.Format("%d",m_nHsTrayY[1]);
	m_dgt_HsTray_Y.SubclassDlgItem(IDC_DGT_HSTRAY_Y, this);
	m_dgt_HsTray_Y.SetStyle(IDB_BIG3, 2);
	m_dgt_HsTray_Y.SetValue(strTmp);
	
	strTmp.Format("%d",m_nHsTrayX[1]);
	m_dgt_HsTray_X.SubclassDlgItem(IDC_DGT_HSTRAY_X, this);
	m_dgt_HsTray_X.SetStyle(IDB_BIG3, 2);
	m_dgt_HsTray_X.SetValue(strTmp);
	
	strTmp.Format("%d",m_nTrayX[1]);
	m_dgt_Tray_X.SubclassDlgItem(IDC_DGT_TRAY_X, this);
	m_dgt_Tray_X.SetStyle(IDB_BIG3, 2);
	m_dgt_Tray_X.SetValue(strTmp);
	
	strTmp.Format("%d",m_nTrayY[1]);
	m_dgt_Tray_Y.SubclassDlgItem(IDC_DGT_TRAY_Y, this);
	m_dgt_Tray_Y.SetStyle(IDB_BIG3, 2);
	m_dgt_Tray_Y.SetValue(strTmp);

	strTmp.Format("%d",m_nfDispenserVppmA[1]);
	m_dgt_DispenserVppmA.SubclassDlgItem(IDC_DGT_DISPENSERVPPMA, this);
	m_dgt_DispenserVppmA.SetStyle(IDB_BIG3, 2);
	m_dgt_DispenserVppmA.SetValue(strTmp);

	
	strTmp.Format("%d",m_nEPoxyCleanTime[1]);
	m_dgt_epoxy_clean_time.SubclassDlgItem(IDC_DGT_EPOXY_CLEAN_TIME, this);
	m_dgt_epoxy_clean_time.SetStyle(IDB_BIG3, 2);
	m_dgt_epoxy_clean_time.SetValue(strTmp);


	strTmp.Format("%d",m_nHardnessUseTime[1]);
	m_dgt_hardness_time.SubclassDlgItem(IDC_DGT_HARDNESS_TIME, this);
	m_dgt_hardness_time.SetStyle(IDB_BIG3, 3);
	m_dgt_hardness_time.SetValue(strTmp);

//	m_dgt_conveyor_move_time.SetStyle(CDigit::DS_INT, 5, CDigit::DC_BGREEN, CDigit::DC_BDISABLE);
//	m_dgt_conveyor_move_time.SetVal(m_n_conveyor_move_time[1]);*/
}

void CScreen_Basic::OnBasic_Data_Set()
{
	
	mstr_device_name[1]							= st_basic.mstr_device_name;
	m_n_mode_interface[1]						= st_basic.n_mode_interface;
	m_n_mode_device[1]							= st_basic.n_mode_device;
	m_n_mode_retest[1]							= st_basic.n_mode_retest;
	
	//kwlee 2017.0405
	m_nModeTester[1]								= 	st_basic.n_mode_tester;						
	m_nModeBcr[1]									= 	st_basic.n_mode_bcr;							
	m_nModeModule[1]								= 	st_basic.n_mode_module;						
	m_nmode_7387[1]									= 	st_basic.n_mode_7387;						
	m_nmode_tray_type[1]							= 	st_basic.n_mode_tray_type;					
	m_n7387InspectAlarmSkipMode[1]					= 	st_basic.n_7387InspectAlarmSkipMode;			
	m_n3874InspectAlarmSkipMode[1]					= 	st_basic.n_3874InspectAlarmSkipMode; //kwlee 2017.0411
	m_n3874InspectMode[1]							= 	st_basic.n_3874InspectMode;					
	n_Light_Curtain_Mode[1]							= 	st_basic.n_Light_Curtain_Mode;				
	n_mode_case_assembly[1]							= 	st_basic.n_mode_case_assembly;				
	
	m_nCountPickRetry[1]							= 	st_basic.n_count_pick_retry;					
	m_nCountPartial[1]								= 	st_basic.n_count_partial;					
	m_nCount_elevator_Partial[1]					= 	st_basic.n_count_elevator_partial;			
	m_n7387AlarmCount[1]							= 	st_basic.n7387AlarmCount;					
	n_rubb_count[1]									= 	st_basic.n_rubb_count;						
	dEpoxyXLineOffSet[1]							= 	st_basic.dEpoxyXLineOffSet;					
<<<<<<< HEAD
	dEpoxyYLineOffSet[1]							= 	st_basic.dEpoxyYLineOffSet;	
	m_nEPoxyCleanTime[1]							=	st_basic.n_EPoxyCleanTime;
	m_nHardnessUseTime[1]							=   st_basic.n_HardnessUseTime;
=======
	dEpoxyYLineOffSet[1]							= 	st_basic.dEpoxyYLineOffSet;					
>>>>>>> c6e69b6ca871ea7a83253cb4bb4092c82b1ae2a4
	
	m_n_count_retry[1]								= 	st_basic.n_count_retry;

	//kwlee 2017.0406
// 	m_dLoaderTransferTrayDeviceGap[1]             =  st_recipe.dLoaderTransferTrayDeviceGap;
// 	m_dSatbleTime[1]							  =  st_recipe.dSatbleTime;
// 	m_nEpoxyRunSpeed[1]							  =  st_recipe.nEpoxyRunSpeed;
// 	m_nEpoxyXYRunSpeed[1][EPOXY_VEL]               =  st_recipe.nEpoxyXYRunSpeed[EPOXY_VEL];
// 	m_nEpoxyXYRunSpeed[1][EPOXY_ACC]               =  st_recipe.nEpoxyXYRunSpeed[EPOXY_ACC];
// 	m_nEpoxyXYRunSpeed[1][EPOXY_DEC]               =  st_recipe.nEpoxyXYRunSpeed[EPOXY_DEC];
// 	m_dEpoxyXOffSet[1]                            =  st_recipe.dEpoxyXOffSet;
// 	m_dEpoxyYOffSet[1]                            =  st_recipe.dEpoxyYOffSet;
// 	m_nRubHSRunSpeed[1]                           = st_recipe.nRubHSRunSpeed;
// 	m_dTrayPitch_Y[1]			                  = st_recipe.dTrayPitch_Y;
// 	m_dTrayPitch_X[1]			                  = st_recipe.dTrayPitch_X;
// 	m_nfDispenserVppmA[1]                         = st_recipe.fDispenserVppmA;
// 	m_nEpoxyDotScrewCount[1]                      = st_recipe.nEpoxyDotScrewCount; 
// 	m_nHsTrayY[1]						          = st_recipe.nHsTrayY;
// 	m_nHsTrayX[1]						          = st_recipe.nHsTrayX;
// 	m_nTrayY[1]						              = st_recipe.nTrayY;
//     m_nTrayX[1]                                   = st_recipe.nTrayX;

	OnBasic_Data_Backup();
	
}


void CScreen_Basic::OnBasic_Data_Label()
{
	m_msg_Epoxy_Y.SetFont(mp_basic_font);
	m_msg_Epoxy_Y.SetWindowText(_T("Y"));
	m_msg_Epoxy_Y.SetCenterText();
	m_msg_Epoxy_Y.SetColor(WHITE_C);
	m_msg_Epoxy_Y.SetGradientColor(GREEN_C);
	m_msg_Epoxy_Y.SetTextColor(BLACK_C);
	
	m_msg_Epoxy_X.SetFont(mp_basic_font);
	m_msg_Epoxy_X.SetWindowText(_T("X"));
	m_msg_Epoxy_X.SetCenterText();
	m_msg_Epoxy_X.SetColor(WHITE_C);
	m_msg_Epoxy_X.SetGradientColor(GREEN_C);
	m_msg_Epoxy_X.SetTextColor(BLACK_C);

	
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

	//	Data_Init();//kwlee 2017.0403

		Init_Pass();
		Init_Reject();
		
	//	Data_Display(); //kwlee 2017.0403

	}
}



void CScreen_Basic::OnBasic_Data_Backup()
{
	mstr_device_name[0]						= mstr_device_name[1];

	m_n_mode_interface[0]					= m_n_mode_interface[1];
	m_n_mode_device[0]						= m_n_mode_device[1];
	m_n_mode_retest[0]						= m_n_mode_retest[1];
	
	//kwlee 2017.0405
	m_nModeTester[0]						= m_nModeTester[1];
	m_nModeBcr[0]							= m_nModeBcr[1];
	m_nModeModule[0]						= m_nModeModule[1];
	m_nmode_7387[0]							= m_nmode_7387[1];
	m_nmode_tray_type[0]					= m_nmode_tray_type[1];
	m_n7387InspectAlarmSkipMode[0]			= m_n7387InspectAlarmSkipMode[1];
	m_n3874InspectMode[0]					= m_n3874InspectMode[1];
	n_Light_Curtain_Mode[0]					= n_Light_Curtain_Mode[1];
	n_mode_case_assembly[0]					= n_mode_case_assembly[1];
	
	m_nCountPickRetry[0]					= m_nCountPickRetry[1];
	m_nCountPartial[0]						= m_nCountPartial[1];
	m_nCount_elevator_Partial[0]			= m_nCount_elevator_Partial[1];
	m_n7387AlarmCount[0]					= m_n7387AlarmCount[1];
	n_rubb_count[0]							= n_rubb_count[1];
	dEpoxyXLineOffSet[0]					= dEpoxyXLineOffSet[1];
	dEpoxyYLineOffSet[0]					= dEpoxyYLineOffSet[1];
	
	m_n_count_retry[0]						= m_n_count_retry[1];
	m_nEPoxyCleanTime[0]					= m_nEPoxyCleanTime[1];
	m_nHardnessUseTime[0]					= m_nHardnessUseTime[1];

	//kwlee 2017.0406
// 	m_dLoaderTransferTrayDeviceGap[0]  = m_dLoaderTransferTrayDeviceGap[1];  
// 	m_dSatbleTime[0]				   = m_dSatbleTime[1];					
// 	m_nEpoxyRunSpeed[0]				   = m_nEpoxyRunSpeed[1];					
// 	m_nEpoxyXYRunSpeed[0][EPOXY_VEL]   = m_nEpoxyXYRunSpeed[1][EPOXY_VEL];
// 	m_nEpoxyXYRunSpeed[0][EPOXY_ACC]   = m_nEpoxyXYRunSpeed[1][EPOXY_ACC];
// 	m_nEpoxyXYRunSpeed[0][EPOXY_DEC]   = m_nEpoxyXYRunSpeed[1][EPOXY_DEC];
// 	m_dEpoxyXOffSet[0]                 = m_dEpoxyXOffSet[1];                 
// 	m_dEpoxyYOffSet[0]                 = m_dEpoxyYOffSet[1];                 
// 	m_nRubHSRunSpeed[0]                = m_nRubHSRunSpeed[1];                
// 	m_dTrayPitch_Y[0]			       = m_dTrayPitch_Y[1];			       
// 	m_dTrayPitch_X[0]			       = m_dTrayPitch_X[1];			       
// 	m_nfDispenserVppmA[0]              =  m_nfDispenserVppmA[1];              
// 	m_nEpoxyDotScrewCount[0]           =  m_nEpoxyDotScrewCount[1];           
// 	m_nHsTrayY[0]						= m_nHsTrayY[1];						
// 	m_nHsTrayX[0]						= m_nHsTrayX[1];						
// 	m_nTrayY[0]						   =  m_nTrayY[1];						   
// 	m_nTrayX[0]                        =  m_nTrayX[1];  
}

void CScreen_Basic::OnBasic_Data_Apply()
{
	((CEdit*)GetDlgItem(IDC_EDIT_DEVICE_TYPE))->GetWindowText(mstr_device_name[1]);
	mstr_device_name[1].MakeUpper();
	mstr_device_name[1].TrimLeft(' ');               
	mstr_device_name[1].TrimRight(' ');	


	st_basic.mstr_device_name				= mstr_device_name[1];

	st_basic.n_mode_interface				= m_n_mode_interface[1];
	st_basic.n_mode_device					= m_n_mode_device[1];
	st_basic.n_mode_retest					= m_n_mode_retest[1];

	//kwlee 2017.0405
	st_basic.n_mode_tester						= m_nModeTester[1];
	st_basic.n_mode_bcr							= m_nModeBcr[1];
	st_basic.n_mode_module						= m_nModeModule[1];
	st_basic.n_mode_7387						= m_nmode_7387[1];
	st_basic.n_mode_tray_type					= m_nmode_tray_type[1];
	st_basic.n_7387InspectAlarmSkipMode			= m_n7387InspectAlarmSkipMode[1];
	st_basic.n_3874InspectAlarmSkipMode			= m_n3874InspectAlarmSkipMode[1]; //kwlee 2017.0411
	st_basic.n_3874InspectMode					= m_n3874InspectMode[1];
	st_basic.n_Light_Curtain_Mode				= n_Light_Curtain_Mode[1];
	st_basic.n_mode_case_assembly				= n_mode_case_assembly[1];
	
	st_basic.n_count_pick_retry					= m_nCountPickRetry[1];
	st_basic.n_count_partial					= m_nCountPartial[1];
	st_basic.n_count_elevator_partial			= m_nCount_elevator_Partial[1];
	st_basic.n7387AlarmCount					= m_n7387AlarmCount[1];
	st_basic.n_rubb_count						= n_rubb_count[1];
	st_basic.dEpoxyXLineOffSet					= dEpoxyXLineOffSet[1];
	st_basic.dEpoxyYLineOffSet					= dEpoxyYLineOffSet[1];
	st_basic.n_count_retry						= m_n_count_retry[1];
	st_basic.n_EPoxyCleanTime					= m_nEPoxyCleanTime[1];
	st_basic.n_HardnessUseTime					= m_nHardnessUseTime[1];

	//kwlee 2017.0406
// 	st_recipe.dLoaderTransferTrayDeviceGap   = m_dLoaderTransferTrayDeviceGap[1];
// 	st_recipe.dSatbleTime                    = m_dSatbleTime[1];
// 	st_recipe.nEpoxyRunSpeed                 = m_nEpoxyRunSpeed[1];             
// 	st_recipe.nEpoxyXYRunSpeed[EPOXY_VEL]    = m_nEpoxyXYRunSpeed[1][EPOXY_VEL];           ;//[0]//vel [1]:acc [2] dec
// 	st_recipe.nEpoxyXYRunSpeed[EPOXY_ACC]    = m_nEpoxyXYRunSpeed[1][EPOXY_ACC];
// 	st_recipe.nEpoxyXYRunSpeed[EPOXY_DEC]    = m_nEpoxyXYRunSpeed[1][EPOXY_DEC];
// 	
// 	st_recipe.dEpoxyXOffSet                  = m_dEpoxyXOffSet[1];
// 	st_recipe.dEpoxyYOffSet                  = m_dEpoxyYOffSet[1];
// 	st_recipe.nRubHSRunSpeed                 = m_nRubHSRunSpeed[1];
// 	
// 	st_recipe.dTrayPitch_Y                   = m_dTrayPitch_Y[1];			//트레이 피치간격 
// 	st_recipe.dTrayPitch_X                   = m_dTrayPitch_X[1];			//트레이 피치간격 	
// 	
// 	st_recipe.fDispenserVppmA                = m_nfDispenserVppmA[1];
// 	st_recipe.nEpoxyDotScrewCount            = m_nEpoxyDotScrewCount[1];
// 	st_recipe.nHsTrayY                       = m_nHsTrayY[1];						// HeatSink tray Y 정보.......
// 	st_recipe.nHsTrayX                       = m_nHsTrayX[1];						// HeatSink tray X 정보.......
// 	
// 	st_recipe.nTrayY                         = m_nTrayY[1];						// tray Y 정보.......
// 	st_recipe.nTrayX						 = m_nTrayX[1];	
}

int CScreen_Basic::Data_Comparison()
{
	int Ret = RET_GOOD;

	if(mstr_device_name[0]					!= mstr_device_name[1])				return RET_ERROR;

	if(m_n_mode_interface[0]				!= m_n_mode_interface[1])			return RET_ERROR;
	if(m_n_mode_device[0]					!= m_n_mode_device[1])				return RET_ERROR;
	if(m_n_mode_retest[0]					!= m_n_mode_retest[1])				return RET_ERROR;
	//kwlee 2017.0405
	if(m_nModeTester[0]						!= m_nModeTester[1])				return RET_ERROR;
	if(m_nModeBcr[0]						!= m_nModeBcr[1])					return RET_ERROR;
	if(m_nModeModule[0]						!= m_nModeModule[1])				return RET_ERROR;
	if(m_nmode_7387[0]						!= m_nmode_7387[1])					return RET_ERROR;
	if(m_nmode_tray_type[0]					!= m_nmode_tray_type[1])			return RET_ERROR;
	if(m_n7387InspectAlarmSkipMode[0]		!= m_n7387InspectAlarmSkipMode[1])	return RET_ERROR;
	if(m_n3874InspectAlarmSkipMode[0]		!= m_n3874InspectAlarmSkipMode[1])	return RET_ERROR;//kwlee 2017.0411
	if(m_n3874InspectMode[0]				!= m_n3874InspectMode[1])			return RET_ERROR;
	if(n_Light_Curtain_Mode[0]				!= n_Light_Curtain_Mode[1])			return RET_ERROR;
	if(n_mode_case_assembly[0]				!= n_mode_case_assembly[1])			return RET_ERROR;

	if(m_n_count_retry[0]					!= m_n_count_retry[1])				return RET_ERROR;
	if(m_nCountPickRetry[0]					!= m_nCountPickRetry[1])			return RET_ERROR;
	if(m_nCountPartial[0]					!= m_nCountPartial[1])				return RET_ERROR;
	if(m_nCount_elevator_Partial[0]			!= m_nCount_elevator_Partial[1])	return RET_ERROR;
	if(m_n7387AlarmCount[0]					!= m_n7387AlarmCount[1])			return RET_ERROR;
	if(n_rubb_count[0]						!= n_rubb_count[1])					return RET_ERROR;
	if(dEpoxyXLineOffSet[0]					!= dEpoxyXLineOffSet[1])			return RET_ERROR;
	if(dEpoxyYLineOffSet[0]					!= dEpoxyYLineOffSet[1])			return RET_ERROR;
<<<<<<< HEAD
	if(m_nEPoxyCleanTime[0]					!= m_nEPoxyCleanTime[1])			return RET_ERROR;
	if(m_nHardnessUseTime[0]				!= m_nHardnessUseTime[1])			return RET_ERROR;
=======
>>>>>>> c6e69b6ca871ea7a83253cb4bb4092c82b1ae2a4
	
	
	//kwlee 2017.0406
	if(m_dLoaderTransferTrayDeviceGap[0]	!= m_dLoaderTransferTrayDeviceGap[1])   return RET_ERROR;
	if(m_dSatbleTime[0]						!= m_dSatbleTime[1])					return RET_ERROR;
	if(m_nEpoxyRunSpeed[0]					!= m_nEpoxyRunSpeed[1])					return RET_ERROR;
	if(m_nEpoxyXYRunSpeed[0][EPOXY_VEL]		!= m_nEpoxyXYRunSpeed[1][EPOXY_VEL])	return RET_ERROR;
	if(m_nEpoxyXYRunSpeed[0][EPOXY_ACC]		!= m_nEpoxyXYRunSpeed[1][EPOXY_ACC])	return RET_ERROR;
	if(m_nEpoxyXYRunSpeed[0][EPOXY_DEC]		!= m_nEpoxyXYRunSpeed[1][EPOXY_DEC])	return RET_ERROR;
	if(m_dEpoxyXOffSet[0]					!= m_dEpoxyXOffSet[1])					return RET_ERROR;
	if(m_dEpoxyYOffSet[0]					!= m_dEpoxyYOffSet[1])					return RET_ERROR;
	if(m_nRubHSRunSpeed[0]					!= m_nRubHSRunSpeed[1])					return RET_ERROR;
	
	if(m_dTrayPitch_Y[0]					!= m_dTrayPitch_Y[1])					return RET_ERROR;
	if(m_dTrayPitch_X[0]					!= m_dTrayPitch_X[1])					return RET_ERROR;
	if(m_nfDispenserVppmA[0]				!= m_nfDispenserVppmA[1])				return RET_ERROR;
	if(m_nEpoxyDotScrewCount[0]				!= m_nEpoxyDotScrewCount[1])			return RET_ERROR;
	if(m_nHsTrayY[0]						!= m_nHsTrayY[1])						return RET_ERROR;
	if(m_nHsTrayX[0]						!= m_nHsTrayX[1])						return RET_ERROR;
	if(m_nTrayY[0]							!= m_nTrayY[1])							return RET_ERROR;
	if(m_nTrayX[0]							!= m_nTrayX[1])							return RET_ERROR;

	return Ret;
}

void CScreen_Basic::Data_Recovery()
{
	mstr_device_name[1]				= mstr_device_name[0];

	m_n_mode_interface[1]			= m_n_mode_interface[0];
	m_n_mode_device[1]				= m_n_mode_device[0];
	m_n_mode_retest[1]				= m_n_mode_retest[0];
	//kwlee 2017.0405
	m_nModeTester[1]						= m_nModeTester[0];
	m_nModeBcr[1]							= m_nModeBcr[0];
	m_nModeModule[1]						= m_nModeModule[0];
	m_nmode_7387[1]							= m_nmode_7387[0];
	m_nmode_tray_type[1]					= m_nmode_tray_type[0];
	m_n7387InspectAlarmSkipMode[1]			= m_n7387InspectAlarmSkipMode[0];
	m_n3874InspectAlarmSkipMode[1]			= m_n3874InspectAlarmSkipMode[0]; //kwlee 2017.0411
	m_n3874InspectMode[1]					= m_n3874InspectMode[0];
	n_Light_Curtain_Mode[1]					= n_Light_Curtain_Mode[0];
	n_mode_case_assembly[1]					= n_mode_case_assembly[0];
	
	m_n_count_retry[1]						= m_n_count_retry[0];
	m_nCountPickRetry[1]					= m_nCountPickRetry[0];
	m_nCountPartial[1]						= m_nCountPartial[0];
	m_nCount_elevator_Partial[1]			= m_nCount_elevator_Partial[0];
	m_n7387AlarmCount[1]					= m_n7387AlarmCount[0];
	n_rubb_count[1]							= n_rubb_count[0];
	dEpoxyXLineOffSet[1]					= dEpoxyXLineOffSet[0];
	dEpoxyYLineOffSet[1]					= dEpoxyYLineOffSet[0];
<<<<<<< HEAD
	m_nEPoxyCleanTime[1]					= m_nEPoxyCleanTime[0];
	m_nHardnessUseTime[1]					= m_nHardnessUseTime[0];
=======
>>>>>>> c6e69b6ca871ea7a83253cb4bb4092c82b1ae2a4
	
	m_n_count_retry[1]						= m_n_count_retry[0];

	//kwlee 2017.0406
// 	m_dLoaderTransferTrayDeviceGap[1]   =   m_dLoaderTransferTrayDeviceGap[0];
// 	m_dSatbleTime[1]				    =	 m_dSatbleTime[0];
// 	m_nEpoxyRunSpeed[1]			 	    =	 m_nEpoxyRunSpeed[0];	
// 	m_nEpoxyXYRunSpeed[1][EPOXY_VEL]    =   m_nEpoxyXYRunSpeed[0][EPOXY_VEL];
// 	m_nEpoxyXYRunSpeed[1][EPOXY_ACC]    =   m_nEpoxyXYRunSpeed[0][EPOXY_ACC];
// 	m_nEpoxyXYRunSpeed[1][EPOXY_DEC]    =   m_nEpoxyXYRunSpeed[0][EPOXY_DEC];
// 	m_dEpoxyXOffSet[1]                  =   m_dEpoxyXOffSet[0];
// 	m_dEpoxyYOffSet[1]                  =   m_dEpoxyYOffSet[0];
// 	m_nRubHSRunSpeed[1]                 =   m_nRubHSRunSpeed[0];
// 	m_dTrayPitch_Y[1]			        =  	 m_dTrayPitch_Y[0];   
// 	m_dTrayPitch_X[1]			        =  	 m_dTrayPitch_X[0];   
// 	m_nfDispenserVppmA[1]               =    m_nfDispenserVppmA[0];
// 	m_nEpoxyDotScrewCount[1]            =    m_nEpoxyDotScrewCount[0];
// 	m_nHsTrayY[1]				 	    =	 m_nHsTrayY[0];
// 	m_nHsTrayX[1]					    =	 m_nHsTrayX[0];
// 	m_nTrayY[1]					        =	 m_nTrayY[0];   
//     m_nTrayX[1]                         =    m_nTrayX[0];  
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
	//kwlee 2017.0422
// 	if(st_handler.b_program_exit == false)
// 	{
// 		nRet = Data_Comparison();
// 		
// 		if(nRet == RET_ERROR){
// 			st_other.str_confirm_msg = _T("데이터가 변경되었습니다. 적용하시겠습니까?");
// 			nResponse = select_dlg.DoModal();
// 			
// 			if(nResponse == IDOK){
// 				//Data_Apply(); //kwlee 2017.0403
// 				::PostMessage(st_handler.hWnd, WM_DATA_INIT_SAVE, 2, 1);  //데이터를 파일에 저장하라는 메세지
// 			}
// 		}
// 	}
	

	if(mp_basic_font){
		delete mp_basic_font;
		mp_basic_font = NULL;
	}

	return CFormView::DestroyWindow();
}

void CScreen_Basic::OnBasic_Device_Display(CString str_dvc)
{
	((CEdit*)GetDlgItem(IDC_EDIT_DEVICE_TYPE))->SetWindowText(_T(str_dvc));
}

int CScreen_Basic::OnBasic_Init_List(LPCTSTR pszPath)
{
	int n_chk;						// 함수 리턴 플래그
    CString str_path = pszPath;		// 폴더 설정
	CString strTmp;

	HANDLE hFind;
    WIN32_FIND_DATA fd;
	int n_count = 0;				// 리스트 박스에 추가한 목록 갯수 저장 변수

	mn_device_name = -1;

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
	((CEdit*)GetDlgItem(IDC_EDIT_DEVICE_TYPE))->SetWindowText(_T(str_device));

	str_device.MakeUpper();
	str_device.TrimLeft(' ');               
	str_device.TrimRight(' ');

	if(str_device.IsEmpty())  
	{
		if (st_handler.cwnd_list != NULL)  // 리스트 바 화면 존재
		{
			st_other.str_normal_msg = _T("[DEVICE] A name input error occurrence.");
			sprintf(st_msg.c_normal_msg, st_other.str_normal_msg);
			st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // 동작 실패 출력 요청
		}
		return FALSE;
	}
	/* ************************************************************************** */

	//str_chk_file = _T("C:\\AMT820\\Motor\\") + str_device;  // 생성할 [폴더]+[파일명] 설정
	//kwlee 2017.0403
	str_chk_file = st_path.mstr_path_Model + str_device;  // 생성할 [폴더]+[파일명] 설정
	
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
				sprintf(st_msg.c_normal_msg, st_other.str_normal_msg);
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

	//((CEdit*)GetDlgItem(IDC_EDIT_DEVICE_TYPE))->GetWindowText(mstr_device_name[1]);
	//kwlee 2017.0403
	((CEdit*)GetDlgItem(IDC_EDIT_DEVICE_TYPE))->SetWindowText(mstr_device_name[1]);
	mstr_device_name[1].MakeUpper();
	mstr_device_name[1].TrimLeft(' ');               
	mstr_device_name[1].TrimRight(' ');

	if(mstr_device_name[1].IsEmpty())  
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
	//kwlee 2017.0405
	if(m_nModeTester[0] != m_nModeTester[1])
	{
		str_msg.Format( "[BASIC] Tester Mode Change %d -> %d", m_nModeTester[0], m_nModeTester[1]);
		Func.On_LogFile_Add(0, str_msg);
	}
	
	if(m_nModeBcr[0] != m_nModeBcr[1])
	{
		str_msg.Format( "[BASIC] BCR Mode Change %d -> %d", m_nModeBcr[0], m_nModeBcr[1]);
		Func.On_LogFile_Add(0, str_msg);
	}
	
	if(m_nModeModule[0] != m_nModeModule[1])
	{
		str_msg.Format( "[BASIC] Module Mode Change %d -> %d", m_nModeModule[0], m_nModeModule[1]);
		Func.On_LogFile_Add(0, str_msg);
	}
	
	if(m_nmode_7387[0] != m_nmode_7387[1])
	{
		str_msg.Format( "[BASIC] 7387 Mode Change %d -> %d", m_nmode_7387[0], m_nmode_7387[1]);
		Func.On_LogFile_Add(0, str_msg);
	}
	if(m_nmode_tray_type[0] != m_nmode_tray_type[1])
	{
		str_msg.Format( "[BASIC] m_nmode_tray_type Change %d -> %d", m_nmode_tray_type[0], m_nmode_tray_type[1]);
		Func.On_LogFile_Add(0, str_msg);
	}
	
	if(m_n7387InspectAlarmSkipMode[0] != m_n7387InspectAlarmSkipMode[1])
	{
		str_msg.Format( "[BASIC] 7387InspectAlarmSkip Mode Change %d -> %d", m_n7387InspectAlarmSkipMode[0], m_n7387InspectAlarmSkipMode[1]);
		Func.On_LogFile_Add(0, str_msg);
	}
	
	//kwlee 2017.0411
	if(m_n3874InspectAlarmSkipMode[0] != m_n3874InspectAlarmSkipMode[1])
	{
		str_msg.Format( "[BASIC] 3874InspectAlarmSkip Mode Change %d -> %d", m_n3874InspectAlarmSkipMode[0], m_n3874InspectAlarmSkipMode[1]);
		Func.On_LogFile_Add(0, str_msg);
	}
	//

	if(m_n3874InspectMode[0] != m_n3874InspectMode[1])
	{
		str_msg.Format( "[BASIC] 3874Inspect Mode Change %d -> %d", m_n3874InspectMode[0], m_n3874InspectMode[1]);
		Func.On_LogFile_Add(0, str_msg);
	}
	
	if(n_Light_Curtain_Mode[0] != n_Light_Curtain_Mode[1])
	{
		str_msg.Format( "[BASIC] Light_Curtain Mode Change %d -> %d", n_Light_Curtain_Mode[0], n_Light_Curtain_Mode[1]);
		Func.On_LogFile_Add(0, str_msg);
	}
	if(n_mode_case_assembly[0] != n_mode_case_assembly[1])
	{
		str_msg.Format( "[BASIC] case_assembly Mode Change %d -> %d", n_mode_case_assembly[0], n_mode_case_assembly[1]);
		Func.On_LogFile_Add(0, str_msg);
	}
	
// 	if(m_nCountPickRetry[0] != m_nCountPickRetry[1])
// 	{
// 		str_msg.Format( "[BASIC] CountPickRetry Change %d -> %d", m_nCountPickRetry[0], m_nCountPickRetry[1]);
// 		Func.On_LogFile_Add(0, str_msg);
// 	}
	
	if(m_nCountPartial[0] != m_nCountPartial[1])
	{
		str_msg.Format( "[BASIC] CountPartial Change %d -> %d", m_nCountPartial[0], m_nCountPartial[1]);
		Func.On_LogFile_Add(0, str_msg);
	}
	
	if(m_nCount_elevator_Partial[0] != m_nCount_elevator_Partial[1])
	{
		str_msg.Format( "[BASIC] Count_elevator_Partial Change %d -> %d", m_nCount_elevator_Partial[0], m_nCount_elevator_Partial[1]);
		Func.On_LogFile_Add(0, str_msg);
	}
	if(m_n7387AlarmCount[0] != m_n7387AlarmCount[1])
	{
		str_msg.Format( "[BASIC] 7387AlarmCount Change %d -> %d", m_n7387AlarmCount[0], m_n7387AlarmCount[1]);
		Func.On_LogFile_Add(0, str_msg);
	}
	
	if(n_rubb_count[0] != n_rubb_count[1])
	{
		str_msg.Format( "[BASIC] rubb_count Change %d -> %d", n_rubb_count[0], n_rubb_count[1]);
		Func.On_LogFile_Add(0, str_msg);
	}
	
	if(dEpoxyXLineOffSet[0] != dEpoxyXLineOffSet[1])
	{
		str_msg.Format( "[BASIC] EpoxyXLineOffSet Change %0.2f -> 0.2f", dEpoxyXLineOffSet[0], dEpoxyXLineOffSet[1]);
		Func.On_LogFile_Add(0, str_msg);
	}
	
	if(m_n_count_retry[0] != m_n_count_retry[1])
<<<<<<< HEAD
	{
		str_msg.Format( "[BASIC] Retry Count Change %d -> %d", m_n_count_retry[0], m_n_count_retry[1]);
		Func.On_LogFile_Add(0, str_msg);
	}
	
	if(m_nEPoxyCleanTime[0] != m_nEPoxyCleanTime[1])
	{
		str_msg.Format( "[BASIC] EPoxy Clean Time Change %d -> %d", m_nEPoxyCleanTime[0], m_nEPoxyCleanTime[1]);
		Func.On_LogFile_Add(0, str_msg);
	}

	if(m_nHardnessUseTime[0] != m_nHardnessUseTime[1])
=======
>>>>>>> c6e69b6ca871ea7a83253cb4bb4092c82b1ae2a4
	{
		str_msg.Format( "[BASIC] Hardness Use Time Change %d -> %d", m_nHardnessUseTime[0], m_nHardnessUseTime[1]);
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
	CString m_strTemp;
	
	st_other.str_confirm_msg = _T("Basic : Setting Data Apply!");
	
	n_response = info_dlg.DoModal();
	
	if (n_response == IDOK)
	{
		if (st_basic.mstr_device_name != mstr_device_name[1])
		{
			if( g_lotMgr.GetLotCount() > 0 )
			{
				m_strTemp = "진행중인 랏이 있습니다.";
				if ( g_local.GetLocalType() == LOCAL_ENG ) m_strTemp = "There is a Running LOT.";
				DoModal_Msg( m_strTemp );
				//OnBasic_Device_Focus_Set(); //kwlee 2017.0403
				return;
			}
		}

		///Data_Apply(); //kwlee 2017.0403
		if (Data_Comparison() == RET_ERROR)
		{	
			OnBasic_Data_Apply();
			OnBasic_Data_HisoryLog();
			OnBasic_Data_Backup();

			if(st_handler.cwnd_title != NULL)
			{
				st_handler.cwnd_title->PostMessage(WM_STATUS_CHANGE, MODE_7387, st_basic.n_mode_7387);			// Device Mode
				st_handler.cwnd_title->PostMessage(WM_STATUS_CHANGE, MODE_3874, st_basic.n_3874InspectMode);			// Device Mode
				st_handler.cwnd_title->PostMessage(WM_STATUS_CHANGE, MODE_BCR, st_basic.n_mode_bcr);
			}
			if(st_handler.cwnd_list != NULL)
			{
				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, MACHINE_INFO); 
				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, DEVICE_INFO); 
			}

		}
		//Data_Backup(); //kwlee 2017.0403

	}
}


//kwlee 2017.0403
// void CScreen_Basic::OnBasic_Device_Focus_Set()
// {
// 	if (mn_device_name == -1)
// 		return;
// 	
// 	m_list_device_type.SetCurSel(mn_device_name);
// }

void CScreen_Basic::OnBasic_Data_Display()
{
	((CEdit*)GetDlgItem(IDC_EDIT_DEVICE_TYPE))->SetWindowText( st_basic.mstr_device_name );	

	
}

void CScreen_Basic::Init_Grid_File()
{
	int max_row, max_col;

	max_row = 0;
	max_col = 2;

	TSpread* m_grid_file;
	m_grid_file = (TSpread*)GetDlgItem(IDC_CUSTOM_FILE_LIST);
	
	m_p_grid.GridReset(m_grid_file);
	
	// 대문자 
	m_p_grid.GridCellSelectDisplay(m_grid_file, TRUE);
	m_p_grid.GridRowHeader(m_grid_file, FALSE);
	m_p_grid.GridColHeader(m_grid_file, TRUE);
	m_p_grid.GridHorizontal(m_grid_file, FALSE);
	m_p_grid.GridVertical(m_grid_file, FALSE);
	m_p_grid.GridAutoSize(m_grid_file, FALSE);
	m_p_grid.GridCellRows(m_grid_file, max_row);
	m_p_grid.GridCellCols(m_grid_file, max_col);
	
	m_p_grid.GridCellHeight_L(m_grid_file, 0, 20);
	m_p_grid.GridCellWidth_L(m_grid_file, 1, 18);
	m_p_grid.GridCellControlStatic(m_grid_file, 0, 1);
	m_p_grid.GridCellWidth_L(m_grid_file, 2, 32);
	m_p_grid.GridCellControlStatic(m_grid_file, 0, 2);

	m_p_grid.GridCellFont(m_grid_file, 0, 1, "MS Sans Serif", 12);
	m_p_grid.GridCellColor(m_grid_file, 0, 1, BLACK_L, YELLOW_C);
	m_p_grid.GridCellText(m_grid_file, 0, 1, "No.");

	m_p_grid.GridCellFont(m_grid_file, 0, 2, "MS Sans Serif", 12);
	m_p_grid.GridCellColor(m_grid_file, 0, 2, BLACK_L, YELLOW_C);
	m_p_grid.GridCellText(m_grid_file, 0, 2, "파일명");

	m_grid_file = NULL;
	delete m_grid_file;
}

void CScreen_Basic::Display_File(int n_mode, int n_count, CString str_filename)
{
	CString str_tmp;
	TSpread* m_grid_file;

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
	m_grid_file = NULL;
	delete m_grid_file;
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


		//Data_Backup(); //kwlee 2017.0403
		mcls_m_basic.OnBasic_Data_Save_As(str_filename); 
	//	mcls_m_basic.On_Teach_Data_Save_As(str_filename);
		mcls_m_basic.OnMaintenance_Data_Save_As(str_filename);
		mcls_m_basic.OnInterface_Data_Save_As(str_filename);
		
		Init_Grid_File();

		//OnBasic_Init_List(_T("C:\\AMT820\\motor\\"));// 파일 폴더 초기화 함수
		//kwlee 2017.0403
		OnBasic_Init_List(st_path.mstr_path_Model);// 파일 폴더 초기화 함수
			
	}
	UpdateData(FALSE);
}

void CScreen_Basic::Init_Grid_Operate()
{
	CRect	area_p, area_r;
	TSpread			*m_grid_operate;
	//int		x_p, y_p;
	//int		width_p, height_p;
	//int		cen_x_p, cen_y_p;
	int		i, j;
	int		max_row, max_col;
	CString str_tmp;

	m_n_operate_used[0] = YES;
	m_n_operate_used[1] = YES;
	m_n_operate_used[2] = YES;

	m_n_operate_cnt[0] = 3;
	m_n_operate_cnt[1] = 4;
	m_n_operate_cnt[2] = 3;

	//max_row = m_n_operate_cnt[0] + m_n_operate_cnt[1] + m_n_operate_cnt[2];
	//kwlee 2017.0404
	max_row = 28;
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

//kwlee 2017.0404
// 	width_p		= area_p.Width() - 15;
// 	height_p	= area_p.Height() - 10;
// 	
// 	x_p			= (width_p / max_col);
// 	y_p			= (height_p / max_row);
// 	
// 	// group box에서 grid 센터에 표시하기 위한 x좌표 구하기 
// 	cen_x_p = area_p.left + (((area_p.Width() - 15) - (x_p * max_col)) / 2) + 8;
// 	// group box에서 grid 센터에 표시하기 위한 y좌표 구하기 
// 	cen_y_p = area_p.top + (((area_p.Height() - 30) - (y_p * max_row)) / 2) + 20;
	
	// grid size 계산하여 중심에 다시 그리기 
//	m_grid_operate->MoveWindow(cen_x_p, cen_y_p, x_p*(max_col), y_p*(max_row), TRUE); //kwlee 2017.0404
	
	
	for(i=0; i<max_row + 1; i++)
	{
		//m_p_grid.GridCellHeight(m_grid_operate, i, y_p);
		//kwlee 2017.0404
		m_p_grid.GridCellHeight(m_grid_operate, i, 18);
		
		for(j=0; j<max_col+1; j++)
		{	
			//m_p_grid.GridCellWidth(m_grid_operate, j, x_p);
			//kwlee 2017.0404
			m_p_grid.GridCellWidth(m_grid_operate, j, 21);
			m_p_grid.GridCellControlStatic(m_grid_operate, i, j);
		}
	}

// 	if(m_n_operate_used[0] == YES)
// 	{
	m_p_grid.GridCellFont(m_grid_operate, 1, 1, "MS Sans Serif", 18);
	m_p_grid.GridCellColor(m_grid_operate, 1, 1, BLACK_L, YELLOW_C);
	m_p_grid.GridCellText(m_grid_operate, 1, 1, "Interface 방법");

	
	m_p_grid.GridCellFont(m_grid_operate, 2, 1, "MS Sans Serif", 12);
	m_p_grid.GridCellText(m_grid_operate, 2, 1, "Off-Line");
	
	m_p_grid.GridCellFont(m_grid_operate, 3, 1, "MS Sans Serif", 12);
	m_p_grid.GridCellText(m_grid_operate, 3, 1, "On-Line");
	
	if(m_n_mode_interface[1] == NOT_USE)
	{	
		m_p_grid.GridCellColor(m_grid_operate, 2, 1, GREEN_C, BLACK_C);
		m_p_grid.GridCellColor(m_grid_operate, 3, 1, GREEN_D, BLACK_C);	
	}
	else
	{
		m_p_grid.GridCellColor(m_grid_operate, 2, 1, GREEN_D, BLACK_C);		
		m_p_grid.GridCellColor(m_grid_operate, 3, 1, GREEN_C, BLACK_C);
	}
		
// 		if(m_n_operate_used[1] == YES)
// 		{
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

// 			if(m_n_operate_used[2] == YES)
// 			{
	m_p_grid.GridCellFont(m_grid_operate, 8, 1, "MS Sans Serif", 18);
	m_p_grid.GridCellColor(m_grid_operate, 8, 1, BLACK_L, YELLOW_C);
	m_p_grid.GridCellText(m_grid_operate, 8, 1, "Retest 방법");
	
	m_p_grid.GridCellFont(m_grid_operate, 9, 1, "MS Sans Serif", 12);
	m_p_grid.GridCellText(m_grid_operate, 9, 1, "Retest Off");

	m_p_grid.GridCellFont(m_grid_operate, 10, 1, "MS Sans Serif", 12);
	m_p_grid.GridCellText(m_grid_operate, 10, 1, "Retest On");

	if(m_n_mode_retest[1] == NOT_USE)
	{
		m_p_grid.GridCellColor(m_grid_operate, 9, 1, GREEN_C, BLACK_C);
		m_p_grid.GridCellColor(m_grid_operate, 10, 1, GREEN_D, BLACK_C);
	}
	else
	{
		m_p_grid.GridCellColor(m_grid_operate, 9, 1, GREEN_D, BLACK_C);
		m_p_grid.GridCellColor(m_grid_operate, 10, 1, GREEN_C, BLACK_C);
	}


	//kwlee 2017.0404
	m_p_grid.GridCellFont(m_grid_operate, 11, 1, "MS Sans Serif", 18);
	m_p_grid.GridCellColor(m_grid_operate, 11, 1, BLACK_L, YELLOW_C);
	m_p_grid.GridCellText(m_grid_operate, 11, 1, "Test 방법");
	
	m_p_grid.GridCellFont(m_grid_operate, 12, 1, "MS Sans Serif", 12);
	m_p_grid.GridCellText(m_grid_operate, 12, 1, "Test Mode Off");
	
	m_p_grid.GridCellFont(m_grid_operate, 13, 1, "MS Sans Serif", 12);
	m_p_grid.GridCellText(m_grid_operate, 13, 1, "Test Mode On");
	

	if(m_nModeTester[1] == NOT_USE)
	{
		m_p_grid.GridCellColor(m_grid_operate, 12, 1, GREEN_C, BLACK_C);
		m_p_grid.GridCellColor(m_grid_operate, 13, 1, GREEN_D, BLACK_C);
	}
	else
	{
		m_p_grid.GridCellColor(m_grid_operate, 12, 1, GREEN_D, BLACK_C);
		m_p_grid.GridCellColor(m_grid_operate, 13, 1, GREEN_C, BLACK_C);
	}


	//kwlee 2017.0404
	m_p_grid.GridCellFont(m_grid_operate, 14, 1, "MS Sans Serif", 18);
	m_p_grid.GridCellColor(m_grid_operate, 14, 1, BLACK_L, YELLOW_C);
	m_p_grid.GridCellText(m_grid_operate, 14, 1, "바코드 사용");
	
	m_p_grid.GridCellFont(m_grid_operate, 15, 1, "MS Sans Serif", 12);
	m_p_grid.GridCellText(m_grid_operate, 15, 1, "Barcode Off");
	
	m_p_grid.GridCellFont(m_grid_operate, 16, 1, "MS Sans Serif", 12);
	m_p_grid.GridCellText(m_grid_operate, 16, 1, "Barcode On");
	
	
	if(m_nModeBcr[1] == NOT_USE)
	{
		m_p_grid.GridCellColor(m_grid_operate, 15, 1, GREEN_C, BLACK_C);
		m_p_grid.GridCellColor(m_grid_operate, 16, 1, GREEN_D, BLACK_C);
	}
	else
	{
		m_p_grid.GridCellColor(m_grid_operate, 15, 1, GREEN_D, BLACK_C);
		m_p_grid.GridCellColor(m_grid_operate, 16, 1, GREEN_C, BLACK_C);
	}
	
	
	m_p_grid.GridCellFont(m_grid_operate, 17, 1, "MS Sans Serif", 18);
	m_p_grid.GridCellColor(m_grid_operate, 17, 1, BLACK_L, YELLOW_C);
	m_p_grid.GridCellText(m_grid_operate, 17, 1, "Module 방법");
	
	m_p_grid.GridCellFont(m_grid_operate, 18, 1, "MS Sans Serif", 12);
	m_p_grid.GridCellText(m_grid_operate, 18, 1, "Module Mode Off");
	
	m_p_grid.GridCellFont(m_grid_operate, 19, 1, "MS Sans Serif", 12);
	m_p_grid.GridCellText(m_grid_operate, 19, 1, "Module Mode On");
	
	
	if(m_nModeModule[1] == NOT_USE)
	{
		m_p_grid.GridCellColor(m_grid_operate, 18, 1, GREEN_C, BLACK_C);
		m_p_grid.GridCellColor(m_grid_operate, 19, 1, GREEN_D, BLACK_C);
	}
	else
	{
		m_p_grid.GridCellColor(m_grid_operate, 18, 1, GREEN_D, BLACK_C);
		m_p_grid.GridCellColor(m_grid_operate, 19, 1, GREEN_C, BLACK_C);
	}


	m_p_grid.GridCellFont(m_grid_operate, 20, 1, "MS Sans Serif", 18);
	m_p_grid.GridCellColor(m_grid_operate, 20, 1, BLACK_L, YELLOW_C);
	m_p_grid.GridCellText(m_grid_operate, 20, 1, "7387  방법");
	
	m_p_grid.GridCellFont(m_grid_operate, 21, 1, "MS Sans Serif", 12);
	m_p_grid.GridCellText(m_grid_operate, 21, 1, "7387 Mode Off");
	
	m_p_grid.GridCellFont(m_grid_operate, 22, 1, "MS Sans Serif", 12);
	m_p_grid.GridCellText(m_grid_operate, 22, 1, "7387 Mode On");
	
	
	if(m_nmode_7387[1] == NOT_USE)
	{
		m_p_grid.GridCellColor(m_grid_operate, 21, 1, GREEN_C, BLACK_C);
		m_p_grid.GridCellColor(m_grid_operate, 22, 1, GREEN_D, BLACK_C);
	}
	else
	{
		m_p_grid.GridCellColor(m_grid_operate, 21, 1, GREEN_D, BLACK_C);
		m_p_grid.GridCellColor(m_grid_operate, 22, 1, GREEN_C, BLACK_C);
	}

	m_p_grid.GridCellFont(m_grid_operate, 23, 1, "MS Sans Serif", 18);
	m_p_grid.GridCellColor(m_grid_operate, 23, 1, BLACK_L, YELLOW_C);
	m_p_grid.GridCellText(m_grid_operate, 23, 1, "Tray Type");
	
	m_p_grid.GridCellFont(m_grid_operate, 24, 1, "MS Sans Serif", 12);
	m_p_grid.GridCellText(m_grid_operate, 24, 1, "Type1");
	
	m_p_grid.GridCellFont(m_grid_operate, 25, 1, "MS Sans Serif", 12);
	m_p_grid.GridCellText(m_grid_operate, 25, 1, "Type2");
	
	
	if(m_nmode_tray_type[1] == NOT_USE)
	{
		m_p_grid.GridCellColor(m_grid_operate, 24, 1, GREEN_C, BLACK_C);
		m_p_grid.GridCellColor(m_grid_operate, 25, 1, GREEN_D, BLACK_C);
	}
	else
	{
		m_p_grid.GridCellColor(m_grid_operate, 24, 1, GREEN_D, BLACK_C);
		m_p_grid.GridCellColor(m_grid_operate, 25, 1, GREEN_C, BLACK_C);
	}

	m_p_grid.GridCellFont(m_grid_operate, 26, 1, "MS Sans Serif", 18);
	m_p_grid.GridCellColor(m_grid_operate, 26, 1, BLACK_L, YELLOW_C);
	m_p_grid.GridCellText(m_grid_operate, 26, 1, "7387 Alarm Skip");
	
	m_p_grid.GridCellFont(m_grid_operate, 27, 1, "MS Sans Serif", 12);
	m_p_grid.GridCellText(m_grid_operate, 27, 1, "Alarm Skip");
	
	m_p_grid.GridCellFont(m_grid_operate, 28, 1, "MS Sans Serif", 12);
	m_p_grid.GridCellText(m_grid_operate, 28, 1, "Alarm Use");
	
	
	if(m_n7387InspectAlarmSkipMode[1] == NOT_USE)
	{
		m_p_grid.GridCellColor(m_grid_operate, 27, 1, GREEN_C, BLACK_C);
		m_p_grid.GridCellColor(m_grid_operate, 28, 1, GREEN_D, BLACK_C);
	}
	else
	{
		m_p_grid.GridCellColor(m_grid_operate, 27, 1, GREEN_D, BLACK_C);
		m_p_grid.GridCellColor(m_grid_operate, 28, 1, GREEN_C, BLACK_C);
	}

	m_grid_operate = NULL;
	delete m_grid_operate;
	Invalidate(FALSE);
}
//kwlee 2017.0405
void CScreen_Basic::Init_Grid_Operate_1()
{
	CRect	area_p, area_r;
	//int		x_p, y_p;
	//int		width_p, height_p;
	//int		cen_x_p, cen_y_p;
	int		i, j;
	int		max_row, max_col;
	CString str_tmp;

	m_n_operate_used[0] = YES;
	m_n_operate_used[1] = YES;
	m_n_operate_used[2] = YES;

	m_n_operate_cnt[0] = 3;
	m_n_operate_cnt[1] = 4;
	m_n_operate_cnt[2] = 3;

	//max_row = m_n_operate_cnt[0] + m_n_operate_cnt[1] + m_n_operate_cnt[2];
	//kwlee 2017.0404
	max_row = 28;
	max_col = 1;
	
	TSpread* m_grid_operate;
	m_grid_operate = (TSpread*)GetDlgItem(IDC_CUSTOM_OPERATE2);
	
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
	
	
	for(i=0; i<max_row + 1; i++)
	{
		//m_p_grid.GridCellHeight(m_grid_operate, i, y_p);
		//kwlee 2017.0404
		m_p_grid.GridCellHeight(m_grid_operate, i, 18);
		
		for(j=0; j<max_col+1; j++)
		{	
			//m_p_grid.GridCellWidth(m_grid_operate, j, x_p);
			//kwlee 2017.0404
			m_p_grid.GridCellWidth(m_grid_operate, j, 21);
			m_p_grid.GridCellControlStatic(m_grid_operate, i, j);
		}
	}

// 	if(m_n_operate_used[0] == YES)
// 	{
	m_p_grid.GridCellFont(m_grid_operate, 1, 1, "MS Sans Serif", 18);
	m_p_grid.GridCellColor(m_grid_operate, 1, 1, BLACK_L, YELLOW_C);
	m_p_grid.GridCellText(m_grid_operate, 1, 1, "3874 모드 사용");

	
	m_p_grid.GridCellFont(m_grid_operate, 2, 1, "MS Sans Serif", 12);
	m_p_grid.GridCellText(m_grid_operate, 2, 1, "Not Use");
	
	m_p_grid.GridCellFont(m_grid_operate, 3, 1, "MS Sans Serif", 12);
	m_p_grid.GridCellText(m_grid_operate, 3, 1, "Use");
	
	if(m_n3874InspectMode[1] == NOT_USE)
	{	
		m_p_grid.GridCellColor(m_grid_operate, 2, 1, GREEN_C, BLACK_C);
		m_p_grid.GridCellColor(m_grid_operate, 3, 1, GREEN_D, BLACK_C);	
	}
	else
	{
		m_p_grid.GridCellColor(m_grid_operate, 2, 1, GREEN_D, BLACK_C);		
		m_p_grid.GridCellColor(m_grid_operate, 3, 1, GREEN_C, BLACK_C);
	}
		

	m_p_grid.GridCellFont(m_grid_operate, 4, 1, "MS Sans Serif", 18);
	m_p_grid.GridCellColor(m_grid_operate, 4, 1, BLACK_L, YELLOW_C);
	m_p_grid.GridCellText(m_grid_operate, 4, 1, "Light Cutain 방법");
	
	m_p_grid.GridCellFont(m_grid_operate, 5, 1, "MS Sans Serif", 12);
	m_p_grid.GridCellText(m_grid_operate, 5, 1, "LightCutain Not Use");

	m_p_grid.GridCellFont(m_grid_operate, 6, 1, "MS Sans Serif", 12);
	m_p_grid.GridCellText(m_grid_operate, 6, 1, "LightCutain Use");

//	m_p_grid.GridCellFont(m_grid_operate, 7, 1, "MS Sans Serif", 12);
	//m_p_grid.GridCellText(m_grid_operate, 7, 1, "No Tray");

	if(n_Light_Curtain_Mode[1] == 0)
	{
		m_p_grid.GridCellColor(m_grid_operate, 5, 1, GREEN_C, BLACK_C);
		m_p_grid.GridCellColor(m_grid_operate, 6, 1, GREEN_D, BLACK_C);
	//	m_p_grid.GridCellColor(m_grid_operate, 7, 1, GREEN_D, BLACK_C);
	}
	else
	{
		m_p_grid.GridCellColor(m_grid_operate, 5, 1, GREEN_D, BLACK_C);
		m_p_grid.GridCellColor(m_grid_operate, 6, 1, GREEN_C, BLACK_C);
	//	m_p_grid.GridCellColor(m_grid_operate, 7, 1, GREEN_D, BLACK_C);
	}


	m_p_grid.GridCellFont(m_grid_operate, 7, 1, "MS Sans Serif", 18);
	m_p_grid.GridCellColor(m_grid_operate,7, 1, BLACK_L, YELLOW_C);
	m_p_grid.GridCellText(m_grid_operate, 7, 1, "Case Assembly 방법");
	
	m_p_grid.GridCellFont(m_grid_operate, 8, 1, "MS Sans Serif", 12);
	m_p_grid.GridCellText(m_grid_operate, 8, 1, "Not Use");

	m_p_grid.GridCellFont(m_grid_operate, 9, 1, "MS Sans Serif", 12);
	m_p_grid.GridCellText(m_grid_operate, 9, 1, "Use");

	if(n_mode_case_assembly[1] == NOT_USE)
	{
		m_p_grid.GridCellColor(m_grid_operate, 8, 1, GREEN_C, BLACK_C);
		m_p_grid.GridCellColor(m_grid_operate, 9, 1, GREEN_D, BLACK_C);
	}
	else
	{
		m_p_grid.GridCellColor(m_grid_operate, 8, 1, GREEN_D, BLACK_C);
		m_p_grid.GridCellColor(m_grid_operate, 9, 1, GREEN_C, BLACK_C);
	}


	//kwlee 2017.0411
	m_p_grid.GridCellFont(m_grid_operate, 10, 1, "MS Sans Serif", 18);
	m_p_grid.GridCellColor(m_grid_operate, 10, 1, BLACK_L, YELLOW_C);
	m_p_grid.GridCellText(m_grid_operate, 10, 1, "3874 Alarm Skip");
	
	m_p_grid.GridCellFont(m_grid_operate, 11, 1, "MS Sans Serif", 12);
	m_p_grid.GridCellText(m_grid_operate, 11, 1, "Alarm Skip");
	
	m_p_grid.GridCellFont(m_grid_operate, 12, 1, "MS Sans Serif", 12);
	m_p_grid.GridCellText(m_grid_operate, 12, 1, "Alarm Use");
	
	
	if(m_n3874InspectAlarmSkipMode[1] == NOT_USE)
	{
		m_p_grid.GridCellColor(m_grid_operate, 11, 1, GREEN_C, BLACK_C);
		m_p_grid.GridCellColor(m_grid_operate, 12, 1, GREEN_D, BLACK_C);
	}
	else
	{
		m_p_grid.GridCellColor(m_grid_operate, 11, 1, GREEN_D, BLACK_C);
		m_p_grid.GridCellColor(m_grid_operate, 12, 1, GREEN_C, BLACK_C);
	}
	m_p_grid.GridCellColor(m_grid_operate, 13, 1, BLACK_L, YELLOW_C);
 	m_p_grid.GridCellColor(m_grid_operate, 14, 1, GREEN_C, YELLOW_C);
 	m_p_grid.GridCellColor(m_grid_operate, 15, 1, GREEN_C, BLACK_C);
 	m_p_grid.GridCellColor(m_grid_operate, 16, 1, BLACK_L, YELLOW_C);
 	m_p_grid.GridCellColor(m_grid_operate, 17, 1, GREEN_C, BLACK_C);
 	m_p_grid.GridCellColor(m_grid_operate, 18,1, GREEN_C, BLACK_C);
 	m_p_grid.GridCellColor(m_grid_operate, 19,1, GREEN_C, BLACK_C);
 	m_p_grid.GridCellColor(m_grid_operate, 20, 1, BLACK_L, YELLOW_C);
 	m_p_grid.GridCellColor(m_grid_operate, 21, 1, GREEN_C, BLACK_C);
 	m_p_grid.GridCellColor(m_grid_operate, 22, 1, BLACK_L, YELLOW_C);
 	m_p_grid.GridCellColor(m_grid_operate, 23, 1, GREEN_C, BLACK_C);
 	m_p_grid.GridCellColor(m_grid_operate, 24, 1, BLACK_L, YELLOW_C);
 	m_p_grid.GridCellColor(m_grid_operate, 25, 1, GREEN_C, BLACK_C);
 	m_p_grid.GridCellColor(m_grid_operate, 26, 1, BLACK_L, YELLOW_C);
 	m_p_grid.GridCellColor(m_grid_operate, 27, 1, GREEN_C, BLACK_C);
 	m_p_grid.GridCellColor(m_grid_operate, 28, 1, GREEN_C, BLACK_C);


	m_grid_operate = NULL;
	delete m_grid_operate;
	Invalidate(FALSE);
}


void CScreen_Basic::OnBtnDelete() 
{
	// TODO: Add your control notification handler code here
	CDialog_Select	select_dlg;
	CString			str_tmp;
	CString			str_filename;
	int				n_response;
	int				n_existence;			// 파일 존재 유무 플래그

	TSpread* m_grid_file;
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

			OnBasic_Init_List(st_path.mstr_basic);// 파일 폴더 초기화 함수
			Init_Grid_File();
			
		}
		else  
		{
			return ;
		}
	}

	m_grid_file = NULL;
	delete m_grid_file;
}

void CScreen_Basic::OnDgtEpoxyX() 
{
	// TODO: Add your control notification handler code here
	CString mstr_temp;  // 저장할 정보 임시 저장 변수 
	char chr_buf[20] ;
	double md_X;

	
	md_X = m_dgt_Epoxy_X.GetValue();
	mstr_temp = LPCTSTR(_ltoa(md_X, chr_buf, 10));
	
	st_msg.mstr_keypad_msg = _T("EPoxy X");
    
	st_msg.mstr_keypad_val = mstr_temp;
	
	mstr_temp = KeyPad.GetNumEditString_D(1, 10000, atof(st_msg.mstr_keypad_val), st_msg.mstr_keypad_msg);
	dEpoxyXLineOffSet[1] = atof(mstr_temp);
	//int iTemp = atoi(mstr_temp);
	//g_dataMgr.SetDataTemp( &st_basic.n_tray_x, iTemp );
	mstr_temp.Format("%0.2f",dEpoxyXLineOffSet[1]);
	m_dgt_Epoxy_X.SetValue( mstr_temp);
}

void CScreen_Basic::OnDgtEpoxyY() 
{
	CString mstr_temp;  // 저장할 정보 임시 저장 변수 
	char chr_buf[20] ;
	double md_Y;
	
	
	md_Y = m_dgt_Epoxy_Y.GetValue();
	mstr_temp = LPCTSTR(_ltoa(md_Y, chr_buf, 10));
	
	st_msg.mstr_keypad_msg = _T("EPoxy Y");
    
	st_msg.mstr_keypad_val = mstr_temp;
	
	mstr_temp = KeyPad.GetNumEditString_D(1, 10000, atof(st_msg.mstr_keypad_val), st_msg.mstr_keypad_msg);
	dEpoxyYLineOffSet[1] = atof(mstr_temp);
	//int iTemp = atoi(mstr_temp);
	//g_dataMgr.SetDataTemp( &st_basic.n_tray_x, iTemp );
	mstr_temp.Format("%0.2f",dEpoxyYLineOffSet[1]);
	m_dgt_Epoxy_Y.SetValue( mstr_temp);
	
}


<<<<<<< HEAD
void CScreen_Basic::OnDgtEpoxyCleanTime() 
{	
	CString mstr_temp;
	char chr_buf[20] ;
	int mn_Cnt;
	
	mn_Cnt = m_dgt_epoxy_clean_time.GetValue();
	mstr_temp = LPCTSTR(_itoa(mn_Cnt, chr_buf, 10));
	
	st_msg.mstr_keypad_msg = _T("Epoxy Clean time(분)");
    
	st_msg.mstr_keypad_val = mstr_temp;
	mstr_temp = KeyPad.GetNumEditString_I(1, 100, atoi(st_msg.mstr_keypad_val), st_msg.mstr_keypad_msg);
	m_nEPoxyCleanTime[1] = atoi(mstr_temp);
	m_dgt_epoxy_clean_time.SetValue( m_nEPoxyCleanTime[1]);
}

=======
>>>>>>> c6e69b6ca871ea7a83253cb4bb4092c82b1ae2a4

void CScreen_Basic::OnDgtRubb() 
{
	// TODO: Add your control notification handler code here
	// TODO: Add your control notification handler code here
	CString mstr_temp;  // 저장할 정보 임시 저장 변수 
	char chr_buf[20] ;
	int mn_Cnt;
	
	mn_Cnt = m_dgt_Rubb.GetValue();
	mstr_temp = LPCTSTR(_itoa(mn_Cnt, chr_buf, 10));
	
	st_msg.mstr_keypad_msg = _T("Rubb Cnt(홀수)");
    
	st_msg.mstr_keypad_val = mstr_temp;
	
	mstr_temp = KeyPad.GetNumEditString_I(1, 100, atoi(st_msg.mstr_keypad_val), st_msg.mstr_keypad_msg);
	n_rubb_count[1] = atoi(mstr_temp);
	if( n_rubb_count[1] % 2 == 0) n_rubb_count[1] = n_rubb_count[1] +1;
	m_dgt_Rubb.SetValue( n_rubb_count[1]);
}

void CScreen_Basic::OnDgt7387AlarmCnt() 
{
	CString mstr_temp;  // 저장할 정보 임시 저장 변수 
	char chr_buf[20] ;
	int mn_Cnt;
	
	mn_Cnt = m_dgt_7387_Alarm_Cnt.GetValue();
	mstr_temp = LPCTSTR(_itoa(mn_Cnt, chr_buf, 10));
	
	st_msg.mstr_keypad_msg = _T("Rubb Cnt");
    
	st_msg.mstr_keypad_val = mstr_temp;
	mstr_temp = KeyPad.GetNumEditString_I(1, 100, atoi(st_msg.mstr_keypad_val), st_msg.mstr_keypad_msg);
	m_n7387AlarmCount[1] = atoi(mstr_temp);
	m_dgt_7387_Alarm_Cnt.SetValue( m_n7387AlarmCount[1]);
}

void CScreen_Basic::OnDgtElvPartialCnt() 
{
	// TODO: Add your control notification handler code here
	CString mstr_temp;  // 저장할 정보 임시 저장 변수 
	char chr_buf[20] ;
	int mn_Cnt;
	
	mn_Cnt = m_dgt_Elv_Partial_Cnt.GetValue();
	mstr_temp = LPCTSTR(_itoa(mn_Cnt, chr_buf, 10));
	
	st_msg.mstr_keypad_msg = _T("Elv Partial Cnt");
    
	st_msg.mstr_keypad_val = mstr_temp;
	mstr_temp = KeyPad.GetNumEditString_I(1, 100, atoi(st_msg.mstr_keypad_val), st_msg.mstr_keypad_msg);
	m_nCount_elevator_Partial[1] = atoi(mstr_temp);
	m_dgt_Elv_Partial_Cnt.SetValue( m_nCount_elevator_Partial[1]);
}

void CScreen_Basic::OnDgtPickRetryCnt() 
{
	// TODO: Add your control notification handler code here
	CString mstr_temp;  // 저장할 정보 임시 저장 변수 
	char chr_buf[20] ;
	int mn_Cnt;
	
	mn_Cnt = m_dgt_Pick_Retry_Cnt.GetValue();
	mstr_temp = LPCTSTR(_itoa(mn_Cnt, chr_buf, 10));
	
	st_msg.mstr_keypad_msg = _T("Picker Retry Cnt");
    
	st_msg.mstr_keypad_val = mstr_temp;
	mstr_temp = KeyPad.GetNumEditString_I(1, 100, atoi(st_msg.mstr_keypad_val), st_msg.mstr_keypad_msg);
	m_nCountPickRetry[1] = atoi(mstr_temp);
	m_dgt_Pick_Retry_Cnt.SetValue( m_nCountPickRetry[1]);
}

void CScreen_Basic::OnDgtPartialCnt() 
{
	// TODO: Add your control notification handler code here
	CString mstr_temp;  // 저장할 정보 임시 저장 변수 
	char chr_buf[20] ;
	int mn_Cnt;
	
	mn_Cnt = m_dgt_Partial_Cnt.GetValue();
	mstr_temp = LPCTSTR(_itoa(mn_Cnt, chr_buf, 10));
	
	st_msg.mstr_keypad_msg = _T("Partial Cnt");
    
	st_msg.mstr_keypad_val = mstr_temp;
	mstr_temp = KeyPad.GetNumEditString_I(1, 100, atoi(st_msg.mstr_keypad_val), st_msg.mstr_keypad_msg);
	m_nCountPartial[1] = atoi(mstr_temp);
	m_dgt_Partial_Cnt.SetValue( m_nCountPartial[1]);
}

void CScreen_Basic::OnDgtRetryCnt() 
{
	// TODO: Add your control notification handler code here
	CString mstr_temp;  // 저장할 정보 임시 저장 변수 
	char chr_buf[20] ;
	int mn_Cnt;
	
	mn_Cnt = m_dgt_Retry_Cnt.GetValue();
	mstr_temp = LPCTSTR(_itoa(mn_Cnt, chr_buf, 10));
	
	st_msg.mstr_keypad_msg = _T("Retry Cnt");
    
	st_msg.mstr_keypad_val = mstr_temp;
	mstr_temp = KeyPad.GetNumEditString_I(1, 100, atoi(st_msg.mstr_keypad_val), st_msg.mstr_keypad_msg);
	m_n_count_retry[1] = atoi(mstr_temp);
	m_dgt_Retry_Cnt.SetValue( m_n_count_retry[1]);
}

void CScreen_Basic::OnDgtEpoxyScrewCnt() 
{
	// TODO: Add your control notification handler code here
	CString mstr_temp;  // 저장할 정보 임시 저장 변수 
	char chr_buf[20] ;
	int mn_Cnt;
	
	mn_Cnt = m_dgt_EpoxyScrewCnt.GetValue();
	mstr_temp = LPCTSTR(_itoa(mn_Cnt, chr_buf, 10));
	
	st_msg.mstr_keypad_msg = _T("EPoxy Screw Cnt");
    
	st_msg.mstr_keypad_val = mstr_temp;
	mstr_temp = KeyPad.GetNumEditString_I(1, 100, atoi(st_msg.mstr_keypad_val), st_msg.mstr_keypad_msg);
	m_nEpoxyDotScrewCount[1] = atoi(mstr_temp);
	m_dgt_EpoxyScrewCnt.SetValue( m_nEpoxyDotScrewCount[1]);
}

void CScreen_Basic::OnDgtHstrayY() 
{
	// TODO: Add your control notification handler code here
	CString mstr_temp;  // 저장할 정보 임시 저장 변수 
	char chr_buf[20] ;
	int mn_Cnt;
	
	mn_Cnt = m_dgt_HsTray_Y.GetValue();
	mstr_temp = LPCTSTR(_itoa(mn_Cnt, chr_buf, 10));
	
	st_msg.mstr_keypad_msg = _T("HsTray Y Cnt");
    
	st_msg.mstr_keypad_val = mstr_temp;
	mstr_temp = KeyPad.GetNumEditString_I(1, 100, atoi(st_msg.mstr_keypad_val), st_msg.mstr_keypad_msg);
	m_nHsTrayY[1] = atoi(mstr_temp);
	m_dgt_HsTray_Y.SetValue( m_nHsTrayY[1]);
}

void CScreen_Basic::OnDgtHstrayX() 
{
	// TODO: Add your control notification handler code here
	CString mstr_temp;  // 저장할 정보 임시 저장 변수 
	char chr_buf[20] ;
	int mn_Cnt;
	
	mn_Cnt = m_dgt_HsTray_X.GetValue();
	mstr_temp = LPCTSTR(_itoa(mn_Cnt, chr_buf, 10));
	
	st_msg.mstr_keypad_msg = _T("HsTray X Cnt");
    
	st_msg.mstr_keypad_val = mstr_temp;
	mstr_temp = KeyPad.GetNumEditString_I(1, 100, atoi(st_msg.mstr_keypad_val), st_msg.mstr_keypad_msg);
	m_nHsTrayX[1] = atoi(mstr_temp);
	m_dgt_HsTray_X.SetValue( m_nHsTrayX[1]);
}

void CScreen_Basic::OnDgtTrayY() 
{
	// TODO: Add your control notification handler code here
	CString mstr_temp;  // 저장할 정보 임시 저장 변수 
	char chr_buf[20] ;
	int mn_Cnt;
	
	mn_Cnt = m_dgt_Tray_Y.GetValue();
	mstr_temp = LPCTSTR(_itoa(mn_Cnt, chr_buf, 10));
	
	st_msg.mstr_keypad_msg = _T("Tray Y Cnt");
    
	st_msg.mstr_keypad_val = mstr_temp;
	mstr_temp = KeyPad.GetNumEditString_I(1, 100, atoi(st_msg.mstr_keypad_val), st_msg.mstr_keypad_msg);
	m_nTrayY[1] = atoi(mstr_temp);
	m_dgt_Tray_Y.SetValue( m_nTrayY[1]);
}

void CScreen_Basic::OnDgtTrayX() 
{
	// TODO: Add your control notification handler code here
	CString mstr_temp;  // 저장할 정보 임시 저장 변수 
	char chr_buf[20] ;
	int mn_Cnt;
	
	mn_Cnt = m_dgt_Tray_X.GetValue();
	mstr_temp = LPCTSTR(_itoa(mn_Cnt, chr_buf, 10));
	
	st_msg.mstr_keypad_msg = _T("Tray X Cnt");
    
	st_msg.mstr_keypad_val = mstr_temp;
	mstr_temp = KeyPad.GetNumEditString_I(1, 100, atoi(st_msg.mstr_keypad_val), st_msg.mstr_keypad_msg);
	m_nTrayX[1] = atoi(mstr_temp);
	m_dgt_Tray_X.SetValue( m_nTrayX[1]);
}

void CScreen_Basic::OnDgtDispenservppmA() 
{
	// TODO: Add your control notification handler code here
	CString mstr_temp;  // 저장할 정보 임시 저장 변수 
	char chr_buf[20] ;
	int mn_Cnt;
	
	mn_Cnt = m_dgt_DispenserVppmA.GetValue();
	mstr_temp = LPCTSTR(_itoa(mn_Cnt, chr_buf, 10));
	
	st_msg.mstr_keypad_msg = _T("EPoxy Screw Cnt");
    
	st_msg.mstr_keypad_val = mstr_temp;
	mstr_temp = KeyPad.GetNumEditString_I(1, 100, atoi(st_msg.mstr_keypad_val), st_msg.mstr_keypad_msg);
	m_nfDispenserVppmA[1] = atoi(mstr_temp);
	m_dgt_DispenserVppmA.SetValue( m_nfDispenserVppmA[1]);
}

void CScreen_Basic::OnButtonCleanReset() 
{
	CDialog_Select	select_dlg;
	st_other.str_confirm_msg = _T("Epoxy Clean시간을 리셋하시겠습니까?");
	
	int n_response = select_dlg.DoModal();

	if( n_response == IDOK)
	{
		st_handler.m_nEpoxyCleanAlarm = CTL_NO;
		st_handler.m_dwEpoxyCleanWaitTime[0] = GetCurrentTime();
		if( st_handler.cwnd_list != NULL )
		{
			sprintf( st_msg.c_normal_msg,"Epoxy Clean시간을 리셋했습니다.");
			st_handler.cwnd_list->PostMessage( WM_LIST_DATA, 0, NORMAL_MSG);
		}	
	}
}

void CScreen_Basic::OnButtonHardnessReset() 
{
	CDialog_Select	select_dlg;
	st_other.str_confirm_msg = _T("경화제 사용시간(480분)을 리셋하시겠습니까?");
	
	int n_response = select_dlg.DoModal();
	
	if( n_response == IDOK)
	{
		g_handler.ClearHardnessTime();
		if( st_handler.cwnd_list != NULL )
		{
			sprintf( st_msg.c_normal_msg,"경화제 사용시간을 리셋했습니다.");
			st_handler.cwnd_list->PostMessage( WM_LIST_DATA, 0, NORMAL_MSG);
		}
	}	
}

void CScreen_Basic::OnDgtHardnessTime() 
{
	CString mstr_temp;
	char chr_buf[20] ;
	int mn_Cnt;
	
	mn_Cnt = m_dgt_hardness_time.GetValue();
	mstr_temp = LPCTSTR(_itoa(mn_Cnt, chr_buf, 10));
	
	st_msg.mstr_keypad_msg = _T("경화제 사용 시간(분)");
    
	st_msg.mstr_keypad_val = mstr_temp;
	mstr_temp = KeyPad.GetNumEditString_I(1, 2000, atoi(st_msg.mstr_keypad_val), st_msg.mstr_keypad_msg);
	m_nHardnessUseTime[1] = atoi(mstr_temp);
	m_dgt_hardness_time.SetValue( m_nHardnessUseTime[1]);
}
