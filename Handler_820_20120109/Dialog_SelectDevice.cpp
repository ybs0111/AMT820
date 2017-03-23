// Dialog_SelectDevice.cpp : implementation file
//

#include "stdafx.h"
#include "handler.h"
#include "Dialog_SelectDevice.h"
#include "Public_Function.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDialog_SelectDevice dialog


CDialog_SelectDevice::CDialog_SelectDevice(CWnd* pParent /*=NULL*/)
	: StandardDialog(CDialog_SelectDevice::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDialog_SelectDevice)
	//}}AFX_DATA_INIT
	m_caption_main = "Select Good Reject Device Dialog Box";
}


void CDialog_SelectDevice::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialog_SelectDevice)
	DDX_Control(pDX, IDC_GIF_SELECT, m_gif_select);
	DDX_Control(pDX, IDC_SELECT_MESSAGE, m_select_message);
	DDX_Control(pDX, IDC_CHK_ACTION9, m_chk_action9);
	DDX_Control(pDX, IDC_CHK_ACTION8, m_chk_action8);
	DDX_Control(pDX, IDC_CHK_ACTION7, m_chk_action7);
	DDX_Control(pDX, IDC_CHK_ACTION6, m_chk_action6);
	DDX_Control(pDX, IDC_CHK_ACTION5, m_chk_action5);
	DDX_Control(pDX, IDC_CHK_ACTION4, m_chk_action4);
	DDX_Control(pDX, IDC_CHK_ACTION3, m_chk_action3);
	DDX_Control(pDX, IDC_CHK_ACTION2, m_chk_action2);
	DDX_Control(pDX, IDC_CHK_ACTION10, m_chk_action10);
	DDX_Control(pDX, IDC_CHK_ACTION1, m_chk_action1);
	DDX_Control(pDX, IDC_GROUP_PICKER10, m_group_picker10);
	DDX_Control(pDX, IDC_GROUP_PICKER9, m_group_picker9);
	DDX_Control(pDX, IDC_GROUP_PICKER8, m_group_picker8);
	DDX_Control(pDX, IDC_GROUP_PICKER7, m_group_picker7);
	DDX_Control(pDX, IDC_GROUP_PICKER6, m_group_picker6);
	DDX_Control(pDX, IDC_GROUP_PICKER5, m_group_picker5);
	DDX_Control(pDX, IDC_GROUP_PICKER4, m_group_picker4);
	DDX_Control(pDX, IDC_GROUP_PICKER3, m_group_picker3);
	DDX_Control(pDX, IDC_GROUP_PICKER2, m_group_picker2);
	DDX_Control(pDX, IDC_GROUP_PICKER1, m_group_picker1);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDialog_SelectDevice, StandardDialog)
	//{{AFX_MSG_MAP(CDialog_SelectDevice)
	ON_BN_CLICKED(IDC_CHK_ACTION1, OnChkAction1)
	ON_BN_CLICKED(IDC_CHK_ACTION2, OnChkAction2)
	ON_BN_CLICKED(IDC_CHK_ACTION3, OnChkAction3)
	ON_BN_CLICKED(IDC_CHK_ACTION4, OnChkAction4)
	ON_BN_CLICKED(IDC_CHK_ACTION5, OnChkAction5)
	ON_BN_CLICKED(IDC_CHK_ACTION6, OnChkAction6)
	ON_BN_CLICKED(IDC_CHK_ACTION7, OnChkAction7)
	ON_BN_CLICKED(IDC_CHK_ACTION8, OnChkAction8)
	ON_BN_CLICKED(IDC_CHK_ACTION9, OnChkAction9)
	ON_BN_CLICKED(IDC_CHK_ACTION10, OnChkAction10)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialog_SelectDevice message handlers

BOOL CDialog_SelectDevice::OnInitDialog() 
{
	StandardDialog::OnInitDialog();
	
	// Start Switch의 동작을 막는다.
	if (st_handler.n_switch_lock == FALSE)
	{
		st_handler.n_switch_lock = TRUE;
		mn_dialog = 1;
	}
	else	mn_dialog = 0;
	
	/* ************************************************************************** */
	/* 컨트롤에 설정할 폰트 생성한다                                              */
	/* ************************************************************************** */
	mp_select_font = NULL;
	mp_select_font = new CFont;
	mp_select_font->CreateFont(15,0,0,0,900,0,0,0,0,0,0,ANTIALIASED_QUALITY,0,"MS Sans Serif");
	/* ************************************************************************** */


	OnSelect_Button_Set();	// 대화 상자 칼라 버튼 생성 함수
	OnSelect_Lable_Set();	// 칼라 텍스트 박스 생성 함수
	OnSelect_Picture_Set();	// GIF 그림 파일 로딩 함수

	On_Macro_Action_Change();
	OnSelect_Action_Change_Set();	// 칼라 텍스트 박스 생성 함수
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDialog_SelectDevice::OnSelect_Picture_Set()
{
	if (m_gif_select.Load(MAKEINTRESOURCE(IDR_GIF_CHOICE),_T("GIF")))
		m_gif_select.Draw();
}

void CDialog_SelectDevice::OnSelect_Lable_Set()
{
	CSxLogFont main_font(15,FW_SEMIBOLD,false,"Arial");
	
	m_select_message.SetFont(mp_select_font);
	m_select_message.SetWindowText(_T(st_other.str_confirm_msg));
	m_select_message.SetCenterText();
	m_select_message.SetColor(RGB(0,0,255));
	m_select_message.SetGradientColor(RGB(0,0,0));
	m_select_message.SetTextColor(RGB(255,255,255));

	m_group_picker1.SetFont(main_font);
	m_group_picker1.SetCatptionTextColor(RGB(145,25,0));
	m_group_picker1.SetFontBold(TRUE);
	
	m_group_picker2.SetFont(main_font);
	m_group_picker2.SetCatptionTextColor(RGB(145,25,0));
	m_group_picker2.SetFontBold(TRUE);
	
	m_group_picker3.SetFont(main_font);
	m_group_picker3.SetCatptionTextColor(RGB(145,25,0));
	m_group_picker3.SetFontBold(TRUE);
	
	m_group_picker4.SetFont(main_font);
	m_group_picker4.SetCatptionTextColor(RGB(145,25,0));
	m_group_picker4.SetFontBold(TRUE);
	
	m_group_picker5.SetFont(main_font);
	m_group_picker5.SetCatptionTextColor(RGB(145,25,0));
	m_group_picker5.SetFontBold(TRUE);
	
	m_group_picker6.SetFont(main_font);
	m_group_picker6.SetCatptionTextColor(RGB(145,25,0));
	m_group_picker6.SetFontBold(TRUE);
	
	m_group_picker7.SetFont(main_font);
	m_group_picker7.SetCatptionTextColor(RGB(145,25,0));
	m_group_picker7.SetFontBold(TRUE);
	
	m_group_picker8.SetFont(main_font);
	m_group_picker8.SetCatptionTextColor(RGB(145,25,0));
	m_group_picker8.SetFontBold(TRUE);
	
	m_group_picker9.SetFont(main_font);
	m_group_picker9.SetCatptionTextColor(RGB(145,25,0));
	m_group_picker9.SetFontBold(TRUE);
	
	m_group_picker10.SetFont(main_font);
	m_group_picker10.SetCatptionTextColor(RGB(145,25,0));
	m_group_picker10.SetFontBold(TRUE);
}

void CDialog_SelectDevice::OnSelect_Button_Set()
{
	mp_select = CreateOkButton(IDOK, this);
}

BOOL CDialog_SelectDevice::DestroyWindow() 
{
	// TODO: Add your specialized code here and/or call the base class
	/* ************************************************************************** */
	/* 생성한 폰트 정보 삭제한다                                                  */
	/* ************************************************************************** */
	delete mp_select_font;
	mp_select_font = NULL;
	/* ************************************************************************** */

	delete mp_select;

	if (mn_dialog == 1)
	{
		st_handler.n_switch_lock = FALSE;
	}

	return StandardDialog::DestroyWindow();
}

void CDialog_SelectDevice::On_Macro_Action_Change()
{
	if (mn_action[0] < 0 || mn_action[0] > 1)	mn_action[0] = 0;
	
	m_chk_action1.SetCheck(mn_action[0]);
	
	if (mn_action[0] == 1)	
	{
		SetDlgItemText(IDC_CHK_ACTION1, "GOOD");
	}
	else
	{
		SetDlgItemText(IDC_CHK_ACTION1, "REJECT");
	}

	if (mn_action[1] < 0 || mn_action[1] > 1)	mn_action[1] = 0;
	
	m_chk_action1.SetCheck(mn_action[1]);
	
	if (mn_action[1] == 1)	
	{
		SetDlgItemText(IDC_CHK_ACTION2, "GOOD");
	}
	else
	{
		SetDlgItemText(IDC_CHK_ACTION2, "REJECT");
	}

	if (mn_action[2] < 0 || mn_action[2] > 1)	mn_action[2] = 0;
	
	m_chk_action1.SetCheck(mn_action[2]);
	
	if (mn_action[2] == 1)	
	{
		SetDlgItemText(IDC_CHK_ACTION3, "GOOD");
	}
	else
	{
		SetDlgItemText(IDC_CHK_ACTION3, "REJECT");
	}

	if (mn_action[3] < 0 || mn_action[3] > 1)	mn_action[3] = 0;
	
	m_chk_action1.SetCheck(mn_action[3]);
	
	if (mn_action[3] == 1)	
	{
		SetDlgItemText(IDC_CHK_ACTION4, "GOOD");
	}
	else
	{
		SetDlgItemText(IDC_CHK_ACTION4, "REJECT");
	}

	if (mn_action[4] < 0 || mn_action[4] > 1)	mn_action[4] = 0;
	
	m_chk_action1.SetCheck(mn_action[4]);
	
	if (mn_action[4] == 1)	
	{
		SetDlgItemText(IDC_CHK_ACTION5, "GOOD");
	}
	else
	{
		SetDlgItemText(IDC_CHK_ACTION5, "REJECT");
	}

	if (mn_action[5] < 0 || mn_action[5] > 1)	mn_action[5] = 0;
	
	m_chk_action1.SetCheck(mn_action[5]);
	
	if (mn_action[5] == 1)	
	{
		SetDlgItemText(IDC_CHK_ACTION6, "GOOD");
	}
	else
	{
		SetDlgItemText(IDC_CHK_ACTION6, "REJECT");
	}

	if (mn_action[6] < 0 || mn_action[6] > 1)	mn_action[6] = 0;
	
	m_chk_action1.SetCheck(mn_action[6]);
	
	if (mn_action[6] == 1)	
	{
		SetDlgItemText(IDC_CHK_ACTION7, "GOOD");
	}
	else
	{
		SetDlgItemText(IDC_CHK_ACTION7, "REJECT");
	}

	if (mn_action[7] < 0 || mn_action[7] > 1)	mn_action[7] = 0;
	
	m_chk_action1.SetCheck(mn_action[0]);
	
	if (mn_action[7] == 1)	
	{
		SetDlgItemText(IDC_CHK_ACTION8, "GOOD");
	}
	else
	{
		SetDlgItemText(IDC_CHK_ACTION8, "REJECT");
	}

	if (mn_action[8] < 0 || mn_action[8] > 1)	mn_action[8] = 0;
	
	m_chk_action1.SetCheck(mn_action[8]);
	
	if (mn_action[8] == 1)	
	{
		SetDlgItemText(IDC_CHK_ACTION9, "GOOD");
	}
	else
	{
		SetDlgItemText(IDC_CHK_ACTION9, "REJECT");
	}

	if (mn_action[9] < 0 || mn_action[9] > 1)	mn_action[9] = 0;
	
	m_chk_action1.SetCheck(mn_action[9]);
	
	if (mn_action[9] == 1)	
	{
		SetDlgItemText(IDC_CHK_ACTION10, "GOOD");
	}
	else
	{
		SetDlgItemText(IDC_CHK_ACTION10, "REJECT");
	}
}

void CDialog_SelectDevice::OnSelect_Action_Change_Set()
{
	CString str_msg;
	m_group_picker1.EnableWindow(FALSE);
	m_group_picker2.EnableWindow(FALSE);
	m_group_picker3.EnableWindow(FALSE);
	m_group_picker4.EnableWindow(FALSE);
	m_group_picker5.EnableWindow(FALSE);
	m_group_picker6.EnableWindow(FALSE);
	m_group_picker7.EnableWindow(FALSE);
	m_group_picker8.EnableWindow(FALSE);
	m_group_picker9.EnableWindow(FALSE);
	m_group_picker10.EnableWindow(FALSE);
	m_chk_action1.EnableWindow(FALSE);
	m_chk_action2.EnableWindow(FALSE);
	m_chk_action3.EnableWindow(FALSE);
	m_chk_action4.EnableWindow(FALSE);
	m_chk_action5.EnableWindow(FALSE);
	m_chk_action6.EnableWindow(FALSE);
	m_chk_action7.EnableWindow(FALSE);
	m_chk_action8.EnableWindow(FALSE);
	m_chk_action9.EnableWindow(FALSE);
	m_chk_action10.EnableWindow(FALSE);
	for (int i = 0; i < SORTER_PICKER_NUM; i++)
	{
		if(mn_action[i] == 1)
		{
			if( i == 0)
			{
				m_chk_action1.EnableWindow(TRUE);
				m_group_picker1.EnableWindow(TRUE);
			}
			else if( i == 1)
			{
				m_chk_action2.EnableWindow(TRUE);
				m_group_picker2.EnableWindow(TRUE);
			}
			else if( i == 2)
			{
				m_chk_action3.EnableWindow(TRUE);
				m_group_picker3.EnableWindow(TRUE);
			}
			else if( i == 3)
			{
				m_chk_action4.EnableWindow(TRUE);
				m_group_picker4.EnableWindow(TRUE);
			}
			else if( i == 4)
			{
				m_chk_action5.EnableWindow(TRUE);
				m_group_picker5.EnableWindow(TRUE);
			}
			else if( i == 5)
			{
				m_chk_action6.EnableWindow(TRUE);
				m_group_picker6.EnableWindow(TRUE);
			}
			else if( i == 6)
			{
				m_chk_action7.EnableWindow(TRUE);
				m_group_picker7.EnableWindow(TRUE);
			}
			else if( i == 7)
			{
				m_chk_action8.EnableWindow(TRUE);
				m_group_picker8.EnableWindow(TRUE);
			}
			else if( i == 8)
			{
				m_chk_action9.EnableWindow(TRUE);
				m_group_picker9.EnableWindow(TRUE);
			}
			else if( i == 9)
			{
				m_chk_action10.EnableWindow(TRUE);
				m_group_picker10.EnableWindow(TRUE);
			}
			else
			{
				AfxMessageBox("Error 관리자에게 전달해 주세요");
				str_msg.Format( "[Select Device] Error 관리자에게 전달해 주세요 : %d", i);
				Func.On_LogFile_Add(99, str_msg);
			}

		}
	}
}

void CDialog_SelectDevice::OnChkAction1() 
{
	if (m_chk_action1.GetCheck() == 1)	
	{
		mn_action[0] = 0;
	}
	else
	{
		mn_action[0] = 1;
	}
	
	On_Macro_Action_Change();		
}

void CDialog_SelectDevice::OnChkAction2() 
{
	if (m_chk_action2.GetCheck() == 1)	
	{
		mn_action[1] = 0;
	}
	else
	{
		mn_action[1] = 1;
	}
	
	On_Macro_Action_Change();		
	
}

void CDialog_SelectDevice::OnChkAction3() 
{
	if (m_chk_action3.GetCheck() == 1)	
	{
		mn_action[2] = 0;
	}
	else
	{
		mn_action[2] = 1;
	}
	
	On_Macro_Action_Change();		
	
}

void CDialog_SelectDevice::OnChkAction4() 
{
	if (m_chk_action4.GetCheck() == 1)	
	{
		mn_action[3] = 0;
	}
	else
	{
		mn_action[3] = 1;
	}
	
	On_Macro_Action_Change();		
	
}

void CDialog_SelectDevice::OnChkAction5() 
{
	if (m_chk_action5.GetCheck() == 1)	
	{
		mn_action[4] = 0;
	}
	else
	{
		mn_action[4] = 1;
	}
	
	On_Macro_Action_Change();		
	
}

void CDialog_SelectDevice::OnChkAction6() 
{
	if (m_chk_action6.GetCheck() == 1)	
	{
		mn_action[5] = 0;
	}
	else
	{
		mn_action[5] = 1;
	}
	
	On_Macro_Action_Change();		
	
}

void CDialog_SelectDevice::OnChkAction7() 
{
	if (m_chk_action7.GetCheck() == 1)	
	{
		mn_action[6] = 0;
	}
	else
	{
		mn_action[6] = 1;
	}
	
	On_Macro_Action_Change();		
	
}

void CDialog_SelectDevice::OnChkAction8() 
{
	if (m_chk_action8.GetCheck() == 1)	
	{
		mn_action[7] = 0;
	}
	else
	{
		mn_action[7] = 1;
	}
	
	On_Macro_Action_Change();		
	
}

void CDialog_SelectDevice::OnChkAction9() 
{
	if (m_chk_action9.GetCheck() == 1)	
	{
		mn_action[8] = 0;
	}
	else
	{
		mn_action[8] = 1;
	}
	
	On_Macro_Action_Change();		
	
}

void CDialog_SelectDevice::OnChkAction10() 
{
	if (m_chk_action10.GetCheck() == 1)	
	{
		mn_action[9] = 0;
	}
	else
	{
		mn_action[9] = 1;
	}
	
	On_Macro_Action_Change();		
	
}
