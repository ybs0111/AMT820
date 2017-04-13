// Dialog_Select_Retest.cpp : implementation file
//

#include "stdafx.h"
#include "handler.h"
#include "Dialog_Select_Retest.h"

#include "Public_Function.h"
#include "Variable.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDialog_Select_Retest dialog


CDialog_Select_Retest::CDialog_Select_Retest(CWnd* pParent /*=NULL*/)
	: StandardDialog(CDialog_Select_Retest::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDialog_Select_Retest)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_caption_main = "Select Lot end Message Dialog Box";
}


void CDialog_Select_Retest::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialog_Select_Retest)
	DDX_Control(pDX, IDC_BTN_SM_LOT_CONTINUE, mp_btn_sm_lotcontinue);
	DDX_Control(pDX, IDC_BTN_SM_BUZZER_OFF, mp_btn_sm_buzzer_off);
	DDX_Control(pDX, IDC_BTN_SM_REJECT_TEST, mp_btn_sm_reject_test);
	DDX_Control(pDX, IDC_MSG_SM_TRAY_CLEAR, mp_msg_sm_tray_clear);
	DDX_Control(pDX, IDC_MSG_SM_INFO, mp_msg_sm_info);
	DDX_Control(pDX, IDC_BTN_SM_LOTEND, mp_btn_sm_lotend);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDialog_Select_Retest, StandardDialog)
	//{{AFX_MSG_MAP(CDialog_Select_Retest)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BTN_SM_LOTEND, OnBtnSmLotend)
	ON_BN_CLICKED(IDC_BTN_SM_BUZZER_OFF, OnBtnSmBuzzerOff)
	ON_BN_CLICKED(IDC_BTN_SM_REJECT_TEST, OnBtnSmRejectTest)
	ON_BN_CLICKED(IDC_BTN_SM_LOT_CONTINUE, OnBtnSmLotContinue)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialog_Select_Retest message handlers

BOOL CDialog_Select_Retest::OnInitDialog() 
{
	StandardDialog::OnInitDialog();
	
	if(st_handler.n_switch_lock != TRUE)
	{
		st_handler.n_switch_lock = TRUE;
		mn_dialog = 1;
	}
	else
	{
		mn_dialog = 0;
	}
	
	// TODO: Add extra initialization here
	mp_sm_font = NULL;
	mp_sm_font = new CFont;
	mp_sm_font->CreateFont(18, 0, 0, 0, 900, 0, 0, 0, 0, 0, 0, ANTIALIASED_QUALITY, 0, "MS Sans Serif");

	OnSM_Label_Set();
	OnSM_Button_Set();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDialog_Select_Retest::OnDestroy() 
{
	StandardDialog::OnDestroy();
	
	if(mp_sm_font)
	{
		delete mp_sm_font ;
		mp_sm_font = NULL;
	}
	if(mn_dialog == 1)
	{
		st_handler.n_switch_lock = FALSE;
	}
	// TODO: Add your message handler code here
	
}

void CDialog_Select_Retest::OnSM_Label_Set()
{
	CString strDisplayInfo;

	mp_msg_sm_tray_clear.SetText(_T(" LotEnd와 Reject \n \n  작업을 선택해 주세요!!"));
	mp_msg_sm_tray_clear.SetTextColor(RGB(255, 0, 0));
	mp_msg_sm_tray_clear.SetBkColor(RGB(255, 255, 255));
	mp_msg_sm_tray_clear.SetFontBold(TRUE);
	mp_msg_sm_tray_clear.SetFontName("Arial");
	mp_msg_sm_tray_clear.SetFontSize(32);

	mp_msg_sm_info.SetFont(mp_sm_font);
	strDisplayInfo = _T("LOT END 진행 조건을 선택하여 주세요.");
	mp_msg_sm_info.SetWindowText(strDisplayInfo);
	mp_msg_sm_info.SetCenterText();
	mp_msg_sm_info.SetColor(RGB(0, 0, 255));
	mp_msg_sm_info.SetGradientColor(RGB(0, 0, 0));
	mp_msg_sm_info.SetTextColor(RGB(255, 255, 255));
}

void CDialog_Select_Retest::OnSM_Button_Set()
{
	int iRejectDvcCount = 0;

//	mp_btn_sm_mlct.EnableWindow(st_basic.i_mode_mlct);
/*
	if(st_basic.i_mode_retest == 1)  // RETEST 모드 
	{
		iRejectDvcCount = st_test.i_test_reject_retestcnt;
	}
	else
	{
		iRejectDvcCount = st_test.i_test_reject_cnt;
	}
*/
//	iRejectDvcCount = st_count.nfail_cnt[0] + st_count.nfail_cnt[1];
//	if(iRejectDvcCount <= 0)
//	{
//		mp_btn_sm_reject_test.EnableWindow(FALSE);
//	}
//	else
//	{
//		mp_btn_sm_reject_test.EnableWindow(TRUE);
//	}
}

BOOL CDialog_Select_Retest::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if(pMsg->message == WM_KEYDOWN && (pMsg->wParam == VK_ESCAPE || pMsg->wParam == VK_RETURN))
	{
		return true;
	}
	
	return StandardDialog::PreTranslateMessage(pMsg);
}

void CDialog_Select_Retest::OnBtnSmLotend() 
{
	StandardDialog::EndDialog(7);
}

void CDialog_Select_Retest::OnBtnSmRejectTest() 
{
	StandardDialog::EndDialog(9);
}

void CDialog_Select_Retest::OnBtnSmBuzzerOff() 
{
	Func.OnSet_IO_Port_Sound(IO_OFF);  // SOUND OFF 시 I/O 출력 함수 
}

void CDialog_Select_Retest::OnBtnSmLotContinue() 
{
	StandardDialog::EndDialog(3);
	
}
