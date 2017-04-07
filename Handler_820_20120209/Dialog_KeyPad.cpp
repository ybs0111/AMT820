// Dialog_KeyPad.cpp : implementation file
//

#include "stdafx.h"
#include "Handler.h"
#include "Dialog_KeyPad.h"

#include "Dialog_Message.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDialog_KeyPad dialog
CDialog_KeyPad KeyPad;

CDialog_KeyPad::CDialog_KeyPad(CWnd* pParent /*=NULL*/)
	: StandardDialog(CDialog_KeyPad::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDialog_KeyPad)
	//}}AFX_DATA_INIT
	m_caption_main = "Keypad Dialog Box";
}


void CDialog_KeyPad::DoDataExchange(CDataExchange* pDX)
{
	StandardDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialog_KeyPad)
	DDX_Control(pDX, IDC_BTN_PAD_MINUS, m_btn_pad_minus);
	DDX_Control(pDX, IDC_BTN_PAD_RESET, m_btn_pad_reset);
	DDX_Control(pDX, IDC_MSG_OLD_VALUE, m_msg_old_value);
	DDX_Control(pDX, IDC_MSG_NEW_VALUE, m_msg_new_value);
	DDX_Control(pDX, IDC_BTN_PAD_DOT, m_btn_pad_dot);
	DDX_Control(pDX, IDC_BTN_PAD_1001, m_btn_pad_1001);
	DDX_Control(pDX, IDC_BTN_PAD_1000, m_btn_pad_1000);
	DDX_Control(pDX, IDC_BTN_PAD_101, m_btn_pad_101);
	DDX_Control(pDX, IDC_BTN_PAD_100, m_btn_pad_100);
	DDX_Control(pDX, IDC_BTN_PAD_11, m_btn_pad_11);
	DDX_Control(pDX, IDC_BTN_PAD_10, m_btn_pad_10);
	DDX_Control(pDX, IDC_BTN_PAD_9, m_btn_pad_9);
	DDX_Control(pDX, IDC_BTN_PAD_8, m_btn_pad_8);
	DDX_Control(pDX, IDC_BTN_PAD_7, m_btn_pad_7);
	DDX_Control(pDX, IDC_BTN_PAD_6, m_btn_pad_6);
	DDX_Control(pDX, IDC_BTN_PAD_5, m_btn_pad_5);
	DDX_Control(pDX, IDC_BTN_PAD_4, m_btn_pad_4);
	DDX_Control(pDX, IDC_BTN_PAD_3, m_btn_pad_3);
	DDX_Control(pDX, IDC_BTN_PAD_2, m_btn_pad_2);
	DDX_Control(pDX, IDC_BTN_PAD_1, m_btn_pad_1);
	DDX_Control(pDX, IDC_BTN_PAD_0, m_btn_pad_0);
	DDX_Control(pDX, IDC_MSG_KEYPAD_LIMIT, m_msg_keypad_limit);
	DDX_Control(pDX, IDC_GROUP_KEYPAD, m_group_keypad);
	DDX_Control(pDX, IDC_BTN_PAD_DEL, m_btn_pad_del);
	DDX_Control(pDX, IDC_BTN_PAD_BACK, m_btn_pad_back);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDialog_KeyPad, StandardDialog)
	//{{AFX_MSG_MAP(CDialog_KeyPad)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BTN_PAD_BACK, OnBtnPadBack)
	ON_BN_CLICKED(IDC_BTN_PAD_DEL, OnBtnPadDel)
	ON_BN_CLICKED(IDC_BTN_PAD_0, OnBtnPad0)
	ON_BN_CLICKED(IDC_BTN_PAD_1, OnBtnPad1)
	ON_BN_CLICKED(IDC_BTN_PAD_2, OnBtnPad2)
	ON_BN_CLICKED(IDC_BTN_PAD_3, OnBtnPad3)
	ON_BN_CLICKED(IDC_BTN_PAD_4, OnBtnPad4)
	ON_BN_CLICKED(IDC_BTN_PAD_5, OnBtnPad5)
	ON_BN_CLICKED(IDC_BTN_PAD_6, OnBtnPad6)
	ON_BN_CLICKED(IDC_BTN_PAD_7, OnBtnPad7)
	ON_BN_CLICKED(IDC_BTN_PAD_8, OnBtnPad8)
	ON_BN_CLICKED(IDC_BTN_PAD_9, OnBtnPad9)
	ON_BN_CLICKED(IDC_BTN_PAD_10, OnBtnPad10)
	ON_BN_CLICKED(IDC_BTN_PAD_11, OnBtnPad11)
	ON_BN_CLICKED(IDC_BTN_PAD_100, OnBtnPad100)
	ON_BN_CLICKED(IDC_BTN_PAD_101, OnBtnPad101)
	ON_BN_CLICKED(IDC_BTN_PAD_1000, OnBtnPad1000)
	ON_BN_CLICKED(IDC_BTN_PAD_1001, OnBtnPad1001)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_BTN_PAD_DOT, OnBtnPadDot)
	ON_BN_CLICKED(IDC_BTN_PAD_RESET, OnBtnPadReset)
	ON_BN_CLICKED(IDC_BTN_PAD_MINUS, OnBtnPadMinus)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialog_KeyPad message handlers

BOOL CDialog_KeyPad::OnInitDialog() 
{
	StandardDialog::OnInitDialog();

	// Start Switch의 동작을 막는다.
	if (st_handler.mn_switch_lock == FALSE)
	{
		st_handler.mn_switch_lock = TRUE;
		mn_dialog = 1;
	}
	else	mn_dialog = 0;

	mn_minus = CTL_NO;
	
	// **************************************************************************
	// 컨트롤에 설정할 폰트 생성한다                                             
	// **************************************************************************
 	mp_pad_font = NULL;
 	mp_pad_font = new CFont;
 	mp_pad_font->CreateFont(12,0,0,0,900,0,0,0,0,0,0,ANTIALIASED_QUALITY,0,"MS Sans Serif");
	// **************************************************************************

	mstr_new_val = st_msg.mstr_keypad_val;	// 전역 변수에 저장된 값을 클래스 변수에 설정
	mstr_old_val = st_msg.mstr_keypad_val;	// 전역 변수에 저장된 값을 클래스 변수에 설정

	OnKeyPad_Button_Set();					// 키패드 대화 상자 칼라 숫자 버튼 생성 함수
	OnKeyPad_GroupBox_Set();				// 키패드 대화 상자 칼라 그룹 박스 생성 함수
	OnKeyPad_Label_Set();					// 키패드 대화 상자 칼라 텍스트 박스 생성 함수
	OnKeyPad_CountData_Set();				// 디지털 카운터 정보 설정 함수

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDialog_KeyPad::OnDestroy() 
{
	// **************************************************************************
	// 생성한 폰트 정보 삭제한다                                                 
	// **************************************************************************
 	delete mp_pad_font;
 	mp_pad_font = NULL;
	// **************************************************************************

	st_msg.mstr_pad_high_limit = "";
	st_msg.mstr_pad_low_limit = "";

	delete mp_pad_ok;		// OK 버튼에 설정된 정보 삭제 
	delete mp_pad_cancel;	// Cancel 버튼에 설정된 정보 삭제 

	if (mn_dialog == 1)
	{
		st_handler.mn_switch_lock = FALSE;
	}

	st_msg.mn_dot_use = TRUE;		// 기본적으로 TRUE가 되야 하기 때문에...

	StandardDialog::OnDestroy();
}

void CDialog_KeyPad::OnKeyPad_GroupBox_Set()
{
	CSxLogFont pad_Logo(15,FW_SEMIBOLD,false,"MS Sans Serif");

	m_group_keypad.SetXPGroupStyle(CXPGroupBox::XPGB_WINDOW);
	m_group_keypad.SetBackgroundColor(RGB(201, 201, 151), ::GetSysColor(COLOR_BTNFACE));
	m_group_keypad.SetFont(pad_Logo);
	m_group_keypad.SetCatptionTextColor(RGB(145,25,0));
	m_group_keypad.SetFontBold(TRUE);
}

void CDialog_KeyPad::OnKeyPad_Label_Set()
{
	int ndx;
	int n_pos;

	m_edit_keypad_title.SubclassDlgItem(IDC_EDIT_KEYPAD_TITLE, this);
	m_edit_keypad_title.bkColor( RGB(0, 0, 0) );
	m_edit_keypad_title.textColor( RGB(255, 255,255) );
	m_edit_keypad_title.setFont(-14, FW_BOLD, DEFAULT_PITCH | FF_DONTCARE, "Bitstream Vera Sans Mono");

	n_pos = st_msg.mstr_keypad_msg.Find("\r\n");

	if (n_pos == -1)		// 한줄 짜리일때..
	{
//		((CEdit *)GetDlgItem(IDC_EDIT_KEYPAD_TITLE))->Multiline(TRUE);
	}
	else
	{

	}

	((CEdit*)GetDlgItem(IDC_EDIT_KEYPAD_TITLE))->SetWindowText(_T("")); 
	((CEdit*)GetDlgItem(IDC_EDIT_KEYPAD_TITLE))->SetFocus();
	
	ndx = ((CEdit*)GetDlgItem(IDC_EDIT_KEYPAD_TITLE))->GetWindowTextLength();
	
	((CEdit*)GetDlgItem(IDC_EDIT_KEYPAD_TITLE))->SendMessage(EM_SETSEL, ndx, ndx);
	((CEdit*)GetDlgItem(IDC_EDIT_KEYPAD_TITLE))->SetWindowText(_T(st_msg.mstr_keypad_msg));

	if (st_msg.mstr_pad_high_limit != "" && st_msg.mstr_pad_low_limit != "")
	{
		m_msg_keypad_limit.ShowWindow(SW_SHOW);
		b_limit_value = TRUE;
	}
	else 
	{
		m_msg_keypad_limit.ShowWindow(SW_HIDE);
		b_limit_value = FALSE;
	}

	m_msg_keypad_limit.SetFont(mp_pad_font);
	m_msg_keypad_limit.SetWindowText(_T("Input Limit : " + st_msg.mstr_pad_low_limit + " ~ " + st_msg.mstr_pad_high_limit));
	m_msg_keypad_limit.SetCenterText();
	m_msg_keypad_limit.SetColor(RGB(0,0,255));
	m_msg_keypad_limit.SetGradientColor(RGB(0,0,0));
	m_msg_keypad_limit.SetTextColor(RGB(255,255,255));

	m_msg_old_value.SetFont(mp_pad_font);
	m_msg_old_value.SetWindowText(_T("Prev."));
	m_msg_old_value.SetCenterText();
	m_msg_old_value.SetColor(RGB(0,0,255));
	m_msg_old_value.SetGradientColor(RGB(0,0,0));
	m_msg_old_value.SetTextColor(RGB(255,255,255));

	m_msg_new_value.SetFont(mp_pad_font);
	m_msg_new_value.SetWindowText(_T("New"));
	m_msg_new_value.SetCenterText();
	m_msg_new_value.SetColor(RGB(0,0,255));
	m_msg_new_value.SetGradientColor(RGB(0,0,0));
	m_msg_new_value.SetTextColor(RGB(255,255,255));
}

void CDialog_KeyPad::OnKeyPad_Button_Set()
{
	mp_pad_ok = CreateOkButton(IDOK, this);
	mp_pad_cancel = CreateCancelButton(IDCANCEL, this);

	if (st_msg.mn_dot_use == FALSE)
	{
		m_btn_pad_dot.ShowWindow(false);
	}
	else
	{
		m_btn_pad_dot.ShowWindow(true);
	}
}

void CDialog_KeyPad::OnKeyPad_CountData_Set()
{
	mp_dgt_val.SubclassDlgItem(IDC_DGT_NEW_VALUE, this);
	mp_dgt_val.SetStyle(IDB_BIG3, 10);

	mp_dgt_old_val.SubclassDlgItem(IDC_DGT_OLD_VALUE, this);
	mp_dgt_old_val.SetStyle(IDB_BIG3, 10);

	if (mstr_new_val.IsEmpty())  mstr_new_val =_T("0");  // 공백 정보가 전달된 경우 0으로 초기값 설정 
	if (mstr_old_val.IsEmpty())  mstr_old_val =_T("0");  // 공백 정보가 전달된 경우 0으로 초기값 설정 

	md_cnt_chk = atof(mstr_new_val);
	md_old_cnt_chk = atof(mstr_old_val);
	
	if (md_cnt_chk > 999999990 || md_cnt_chk == 0)
	{
		mstr_new_val =_T("0");
		mp_dgt_val.SetValue(0);
	}
	else  
	{
		mp_dgt_val.SetValue(mstr_new_val);
	}

	if (md_old_cnt_chk > 999999990 || md_old_cnt_chk == 0)
	{
		mstr_old_val =_T("0");
		mp_dgt_old_val.SetValue(0);
	}
	else  
	{
		mp_dgt_old_val.SetValue(mstr_old_val);
	}
}

void CDialog_KeyPad::OnOK() 
{
	int n_response;				// 대화 상자 리턴 플래그
	int n_error_flag = FALSE;

	CDialog_Message  Msg_dlg;

	if (b_limit_value == TRUE)
	{
		if (atol(mstr_new_val) > atol(st_msg.mstr_pad_high_limit))
		{
			st_msg.str_fallacy_msg = _T("Setting Data is Over then Limit!");
			n_error_flag = TRUE;
		}
		else if (atol(mstr_new_val) < atol(st_msg.mstr_pad_low_limit))
		{
			st_msg.str_fallacy_msg = _T("Setting Data is Lower then Limit!");
			n_error_flag = TRUE;
		}

		if (n_error_flag == TRUE)
		{
			n_response = Msg_dlg.DoModal();

			if (n_response == IDOK)
			{
				mstr_new_val = mstr_old_val;

				if (mstr_new_val.IsEmpty())  mstr_new_val =_T("0");  // 공백 정보가 전달된 경우 0으로 초기값 설정 

				md_cnt_chk = atof(mstr_old_val);					// Limit 넘어 간 경우에는 기존 OLD로 넣는다.
				
				if (md_cnt_chk > 999999990 || md_cnt_chk == 0)
				{
					mstr_new_val =_T("0");
					mp_dgt_val.SetValue(0);
				}
				else  
				{
					mp_dgt_val.SetValue(mstr_new_val);
				}
				return;	
			}
		}
	}
	st_msg.mstr_keypad_val = mstr_new_val;  // 키패드 입력 값을 전역 변수에 설정 
	
	StandardDialog::OnOK();
}

void CDialog_KeyPad::OnBtnPadBack() 
{
	if (mstr_new_val ==_T("") || mstr_new_val ==_T("0")) return ;  // 현재 입력 값이 공백이거나 0이면 리턴

	mn_cnt = strlen(mstr_new_val);

	if (mn_cnt == 1) 
	{
		mn_pos = mstr_new_val.Find('-');

		if (mn_pos != -1)  return;
		else
		{
			mstr_new_val.Empty();
			mp_dgt_val.SetValue(0);
		}
	}
	else 
	{
		CString s( _T(mstr_new_val));
		mstr_new_val=s.Left(mn_cnt-1);

		md_cnt_chk = atof(mstr_new_val);
		mp_dgt_val.SetValue(mstr_new_val);
	}
}

void CDialog_KeyPad::OnBtnPadDel() 
{
	mstr_new_val.Empty();

	mn_minus = CTL_NO;

	mp_dgt_val.SetValue(0);
}

void CDialog_KeyPad::OnBtnPad0() 
{
	if (mstr_new_val.IsEmpty())  return;

	mn_cnt=strlen(mstr_new_val);
	
	if (mn_cnt > 8) return;

	// 소수점 3자리까지만 입력을 받는다. 2K8/01/10/ViboX
	mn_dot_pos = mstr_new_val.Find('.');
	if (mn_dot_pos != -1)
	{
		if (mn_cnt - mn_dot_pos > 3)	return;
	}

	md_cnt_chk = atof(mstr_new_val);

	if (md_cnt_chk != 0)  mstr_new_val +=_T("0");
	else 
	{
		mn_pos = mstr_new_val.Find('-');
		mn_dot_pos = mstr_new_val.Find('.');

		if (mn_pos == -1 && mn_dot_pos == -1)  mstr_new_val =_T("0");
		else  mstr_new_val +=_T("0") ;
	}
	
	mp_dgt_val.SetValue(mstr_new_val);
}

void CDialog_KeyPad::OnBtnPad1() 
{
	mn_cnt = strlen(mstr_new_val);

	if (mn_cnt > 8) return;

	// 소수점 3자리까지만 입력을 받는다. 2K8/01/10/ViboX
	mn_dot_pos = mstr_new_val.Find('.');

	if (mn_dot_pos != -1)
	{
		if (mn_cnt - mn_dot_pos > 3)	return;
	}
	
	md_cnt_chk = atof(mstr_new_val);

	if (md_cnt_chk != 0)  mstr_new_val +=_T("1");
	else 
	{
		mn_pos = mstr_new_val.Find('-');
		mn_dot_pos = mstr_new_val.Find('.');

		if (mn_pos == -1 && mn_dot_pos == -1)  mstr_new_val =_T("1");
		else  mstr_new_val +=_T("1") ;
	}

	mp_dgt_val.SetValue(mstr_new_val);
}

void CDialog_KeyPad::OnBtnPad2() 
{
	mn_cnt = strlen(mstr_new_val);

	if (mn_cnt > 8) return;

	// 소수점 3자리까지만 입력을 받는다. 2K8/01/10/ViboX
	mn_dot_pos = mstr_new_val.Find('.');
	if (mn_dot_pos != -1)
	{
		if (mn_cnt - mn_dot_pos > 3)	return;
	}

	md_cnt_chk = atof(mstr_new_val);
	
	if (md_cnt_chk != 0)  mstr_new_val +=_T("2");
	else 
	{
		mn_pos = mstr_new_val.Find('-');
		mn_dot_pos = mstr_new_val.Find('.');

		if (mn_pos == -1 && mn_dot_pos == -1)  mstr_new_val =_T("2");
		else  mstr_new_val +=_T("2") ;
	}

	mp_dgt_val.SetValue(mstr_new_val);
}

void CDialog_KeyPad::OnBtnPad3() 
{
	mn_cnt = strlen(mstr_new_val);

	if (mn_cnt > 8) return;

	// 소수점 3자리까지만 입력을 받는다. 2K8/01/10/ViboX
	mn_dot_pos = mstr_new_val.Find('.');
	if (mn_dot_pos != -1)
	{
		if (mn_cnt - mn_dot_pos > 3)	return;
	}

	md_cnt_chk = atof(mstr_new_val);
	
	if (md_cnt_chk != 0)  mstr_new_val +=_T("3");
	else 
	{
		mn_pos = mstr_new_val.Find('-');
		mn_dot_pos = mstr_new_val.Find('.');
		
		if (mn_pos == -1 && mn_dot_pos == -1)  mstr_new_val =_T("3");
		else  mstr_new_val +=_T("3") ;
	}

	mp_dgt_val.SetValue(mstr_new_val);
}

void CDialog_KeyPad::OnBtnPad4() 
{
	mn_cnt = strlen(mstr_new_val);

	if (mn_cnt > 8) return;

	// 소수점 3자리까지만 입력을 받는다. 2K8/01/10/ViboX
	mn_dot_pos = mstr_new_val.Find('.');
	if (mn_dot_pos != -1)
	{
		if (mn_cnt - mn_dot_pos > 3)	return;
	}

	md_cnt_chk = atof(mstr_new_val);
	
	if (md_cnt_chk != 0)  mstr_new_val +=_T("4");
	else 
	{
		mn_pos = mstr_new_val.Find('-');
		mn_dot_pos = mstr_new_val.Find('.');
		
		if (mn_pos == -1 && mn_dot_pos == -1)  mstr_new_val =_T("4");
		else  mstr_new_val +=_T("4") ;
	}

	mp_dgt_val.SetValue(mstr_new_val);
}

void CDialog_KeyPad::OnBtnPad5() 
{
	mn_cnt = strlen(mstr_new_val);

	if (mn_cnt > 8) return;

	// 소수점 3자리까지만 입력을 받는다. 2K8/01/10/ViboX
	mn_dot_pos = mstr_new_val.Find('.');
	if (mn_dot_pos != -1)
	{
		if (mn_cnt - mn_dot_pos > 3)	return;
	}

	md_cnt_chk = atof(mstr_new_val);
	
	if (md_cnt_chk != 0)  mstr_new_val +=_T("5");
	else 
	{
		mn_pos = mstr_new_val.Find('-');
		mn_dot_pos = mstr_new_val.Find('.');
		
		if (mn_pos == -1 && mn_dot_pos == -1)  mstr_new_val =_T("5");
		else  mstr_new_val +=_T("5") ;
	}

	mp_dgt_val.SetValue(mstr_new_val);
}

void CDialog_KeyPad::OnBtnPad6() 
{
	mn_cnt = strlen(mstr_new_val);

	if (mn_cnt > 8) return;

	// 소수점 3자리까지만 입력을 받는다. 2K8/01/10/ViboX
	mn_dot_pos = mstr_new_val.Find('.');
	if (mn_dot_pos != -1)
	{
		if (mn_cnt - mn_dot_pos > 3)	return;
	}

	md_cnt_chk = atof(mstr_new_val);
	
	if (md_cnt_chk != 0)  mstr_new_val +=_T("6");
	else 
	{
		mn_pos = mstr_new_val.Find('-');
		mn_dot_pos = mstr_new_val.Find('.');
		
		if (mn_pos == -1 && mn_dot_pos == -1)  mstr_new_val =_T("6");
		else  mstr_new_val +=_T("6") ;
	}

	mp_dgt_val.SetValue(mstr_new_val);
}

void CDialog_KeyPad::OnBtnPad7() 
{
	mn_cnt = strlen(mstr_new_val);

	if (mn_cnt > 8) return;

	// 소수점 3자리까지만 입력을 받는다. 2K8/01/10/ViboX
	mn_dot_pos = mstr_new_val.Find('.');
	if (mn_dot_pos != -1)
	{
		if (mn_cnt - mn_dot_pos > 3)	return;
	}

	md_cnt_chk = atof(mstr_new_val);
	
	if (md_cnt_chk != 0)  mstr_new_val +=_T("7");
	else 
	{
		mn_pos = mstr_new_val.Find('-');
		mn_dot_pos = mstr_new_val.Find('.');
		
		if (mn_pos == -1 && mn_dot_pos == -1)  mstr_new_val =_T("7");
		else  mstr_new_val +=_T("7") ;
	}

	mp_dgt_val.SetValue(mstr_new_val);
}

void CDialog_KeyPad::OnBtnPad8() 
{
	mn_cnt = strlen(mstr_new_val);

	if (mn_cnt > 8) return;

	// 소수점 3자리까지만 입력을 받는다. 2K8/01/10/ViboX
	mn_dot_pos = mstr_new_val.Find('.');
	if (mn_dot_pos != -1)
	{
		if (mn_cnt - mn_dot_pos > 3)	return;
	}

	md_cnt_chk = atof(mstr_new_val);
	
	if (md_cnt_chk != 0)  mstr_new_val +=_T("8");
	else 
	{
		mn_pos = mstr_new_val.Find('-');
		mn_dot_pos = mstr_new_val.Find('.');
		
		if (mn_pos == -1 && mn_dot_pos == -1)  mstr_new_val =_T("8");
		else  mstr_new_val +=_T("8") ;
	}

	mp_dgt_val.SetValue(mstr_new_val);
}

void CDialog_KeyPad::OnBtnPad9() 
{
	mn_cnt = strlen(mstr_new_val);

	if (mn_cnt > 8) return;

	// 소수점 3자리까지만 입력을 받는다. 2K8/01/10/ViboX
	mn_dot_pos = mstr_new_val.Find('.');

	if (mn_dot_pos != -1)
	{
		if (mn_cnt - mn_dot_pos > 3)	return;
	}

	md_cnt_chk = atof(mstr_new_val);
	
	if (md_cnt_chk != 0)  mstr_new_val +=_T("9");
	else 
	{
		mn_pos = mstr_new_val.Find('-');
		mn_dot_pos = mstr_new_val.Find('.');
		
		if (mn_pos == -1 && mn_dot_pos == -1)  mstr_new_val =_T("9");
		else  mstr_new_val +=_T("9") ;
	}

	mp_dgt_val.SetValue(mstr_new_val);
}

void CDialog_KeyPad::OnBtnPad10() 
{
	if (mstr_new_val.IsEmpty())  mstr_new_val=_T("0");

	md_cnt_chk = atof(mstr_new_val);
	if (md_cnt_chk >= 999999990) return;

	md_cnt_chk += 10 ;
	if (st_msg.mn_dot_use == FALSE)
	{
		mstr_new_val.Format("%d", (int)md_cnt_chk);
	}
	else
	{
		mstr_new_val.Format("%4.3f", md_cnt_chk);
	}
	mp_dgt_val.SetValue(mstr_new_val);
}

void CDialog_KeyPad::OnBtnPad11() 
{
	if (mstr_new_val.IsEmpty())  mstr_new_val=_T("0");

	md_cnt_chk = atof(mstr_new_val);
	if (md_cnt_chk >= 999999990) return;

	md_cnt_chk -= 10 ;
	if (st_msg.mn_dot_use == FALSE)
	{
		mstr_new_val.Format("%d", (int)md_cnt_chk);
	}
	else
	{
		mstr_new_val.Format("%4.3f", md_cnt_chk);
	}
	mp_dgt_val.SetValue(mstr_new_val);
}

void CDialog_KeyPad::OnBtnPad100() 
{
	if (mstr_new_val.IsEmpty())  mstr_new_val=_T("0");

	md_cnt_chk = atof(mstr_new_val);
	if (md_cnt_chk >= 999999990) return;

	md_cnt_chk += 100;
	if (st_msg.mn_dot_use == FALSE)
	{
		mstr_new_val.Format("%d", (int)md_cnt_chk);
	}
	else
	{
		mstr_new_val.Format("%4.3f", md_cnt_chk);
	}
	mp_dgt_val.SetValue(mstr_new_val);
}

void CDialog_KeyPad::OnBtnPad101() 
{
	if (mstr_new_val.IsEmpty())  mstr_new_val=_T("0");

	md_cnt_chk = atof(mstr_new_val);
	if (md_cnt_chk >= 999999990) return;

	md_cnt_chk -= 100;
	if (st_msg.mn_dot_use == FALSE)
	{
		mstr_new_val.Format("%d", (int)md_cnt_chk);
	}
	else
	{
		mstr_new_val.Format("%4.3f", md_cnt_chk);
	}
	mp_dgt_val.SetValue(mstr_new_val);
}

void CDialog_KeyPad::OnBtnPad1000() 
{
	if (mstr_new_val.IsEmpty())  mstr_new_val=_T("0");

	md_cnt_chk = atof(mstr_new_val);
	if (md_cnt_chk >= 999999990) return;

	md_cnt_chk += 1;
	if (st_msg.mn_dot_use == FALSE)
	{
		mstr_new_val.Format("%d", (int)md_cnt_chk);
	}
	else
	{
		mstr_new_val.Format("%4.3f", md_cnt_chk);
	}
	mp_dgt_val.SetValue(mstr_new_val);
}

void CDialog_KeyPad::OnBtnPad1001() 
{
	if (mstr_new_val.IsEmpty())  mstr_new_val=_T("0");

	md_cnt_chk = atof(mstr_new_val);
	if (md_cnt_chk >= 999999990) return;

	md_cnt_chk -= 1;
	if (st_msg.mn_dot_use == FALSE)
	{
		mstr_new_val.Format("%d", (int)md_cnt_chk);
	}
	else
	{
		mstr_new_val.Format("%4.3f", md_cnt_chk);
	}
	mp_dgt_val.SetValue(mstr_new_val);
}

void CDialog_KeyPad::OnBtnPadDot() 
{
	if (mstr_new_val.IsEmpty())  mstr_new_val=_T("0");
	
	md_cnt_chk = atof(mstr_new_val);
	if (ml_cnt_chk >= 999999990) return;

	// 소수점이 있으면 또 안찍힌다. 2K8/01/10/ViboX
	mn_dot_pos = mstr_new_val.Find('.');
	if (mn_dot_pos != -1)	return;
	
	if (md_cnt_chk == 0)
	{
		mstr_new_val = "0.";
	}
	else
	{
		mstr_new_val += ".";
	}

	mp_dgt_val.SetValue(mstr_new_val);
}

void CDialog_KeyPad::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	if (st_msg.mn_pad_dialog == CTL_YES || m_point == CTL_YES) return;		// 다이얼로그 위에서 한번 더 다이얼로그를 띄울 경우에는
																			// 이 옵션이 정상적으로 동작을 하지 않으므로, 통과한다.
	// TODO: Add your message handler code here
	if (m_bFirst)
	{
		m_bFirst = FALSE;
		
		if (m_ptRef != CPoint(-1, -1))
		{
			CRect r;
			GetWindowRect(&r);
			
			int w = r.Width(), h = r.Height();

			if (m_ptRef.x + w < 1280)
			{
				r.left = m_ptRef.x;
				r.right = m_ptRef.x + w;
			}
			else
			{
				r.left = m_ptRefLeft.x - w;
				r.right = m_ptRefLeft.x;
			}
			
			if (m_ptRef.y + h < 1024)
			{
				r.top = m_ptRef.y;
				r.bottom = r.top + h;
			}
			else
			{
				r.top = 998 - h;
				r.bottom = 998;
			}
			MoveWindow(&r);
		}
	}
}

BOOL CDialog_KeyPad::PreTranslateMessage(MSG* pMsg) 
{
	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_NUMPAD0 || pMsg->wParam == '0')			{OnBtnPad0(); return true;}
		else if (pMsg->wParam == VK_NUMPAD1 || pMsg->wParam == '1')		{OnBtnPad1(); return true;}
		else if (pMsg->wParam == VK_NUMPAD2 || pMsg->wParam == '2')		{OnBtnPad2(); return true;}
		else if (pMsg->wParam == VK_NUMPAD3 || pMsg->wParam == '3')		{OnBtnPad3(); return true;}
		else if (pMsg->wParam == VK_NUMPAD4 || pMsg->wParam == '4')		{OnBtnPad4(); return true;}
		else if (pMsg->wParam == VK_NUMPAD5 || pMsg->wParam == '5')		{OnBtnPad5(); return true;}
		else if (pMsg->wParam == VK_NUMPAD6 || pMsg->wParam == '6')		{OnBtnPad6(); return true;}
		else if (pMsg->wParam == VK_NUMPAD7 || pMsg->wParam == '7')		{OnBtnPad7(); return true;}
		else if (pMsg->wParam == VK_NUMPAD8 || pMsg->wParam == '8')		{OnBtnPad8(); return true;}
		else if (pMsg->wParam == VK_NUMPAD9 || pMsg->wParam == '9')		{OnBtnPad9(); return true;}
		else if ((pMsg->wParam == VK_DECIMAL || pMsg->wParam == 0xBE) && st_msg.mn_dot_use == TRUE)	{OnBtnPadDot(); return true;}
		else if (pMsg->wParam == VK_BACK)								{OnBtnPadBack(); return true;}
		else if (pMsg->wParam == VK_DELETE)								{OnBtnPadDel(); return true;}
		else if (pMsg->wParam == VK_RETURN)								{OnOK(); return true;}
	}
	
	return StandardDialog::PreTranslateMessage(pMsg);
}

void CDialog_KeyPad::OnBtnPadReset() 
{
	mstr_new_val = mstr_old_val;
	
	if (mstr_new_val.IsEmpty())  mstr_new_val =_T("0");  // 공백 정보가 전달된 경우 0으로 초기값 설정 
	
	md_cnt_chk = atof(mstr_old_val);						// Limit 넘어 간 경우에는 기존 OLD로 넣는다.
	
	if (md_cnt_chk > 999999990 || md_cnt_chk == 0)
	{
		mstr_new_val =_T("0");
		mp_dgt_val.SetValue(0);
	}
	else  
	{
		mp_dgt_val.SetValue(mstr_new_val);
	}
}

void CDialog_KeyPad::OnBtnPadMinus() 
{
	if (mstr_new_val == "0" || mstr_new_val == "")			return;

	mn_pos = mstr_new_val.Find('-');

	if (mn_pos != -1)
	{
		mn_minus = CTL_YES;
	}

	if (mn_minus)
	{
		mstr_new_val.TrimLeft("-");
	}
	else
	{
		mstr_new_val =_T("-") + mstr_new_val;
	}
	
	mp_dgt_val.SetValue(mstr_new_val);

	mn_minus = !mn_minus;
}

CString CDialog_KeyPad::GetNumEditString_D(double dMin, double dMax, double dPrev, CString strText, CRect rRect)
{
	int mn_response;
	CString strResult;

	CDialog_KeyPad pad_dlg;

	st_msg.mstr_pad_high_limit.Format("%4.3f", dMax);
	st_msg.mstr_pad_low_limit.Format("%4.3f", dMin);
	st_msg.mstr_keypad_val.Format("%4.3f", dPrev);
	st_msg.mstr_keypad_msg = strText;
	
	st_msg.mn_dot_use = TRUE;
	st_msg.mn_pad_dialog = CTL_NO;

	if (rRect.right > 12800 || rRect.left < 0 || rRect.left > 12800)
	{
		st_msg.mn_pad_dialog = CTL_YES;
	}
	else
	{
		pad_dlg.m_ptRef = CPoint(rRect.right, rRect.top);
		pad_dlg.m_ptRefLeft = CPoint(rRect.left, rRect.top);
	}
	
	mn_response = pad_dlg.DoModal();
	
	if (mn_response == IDOK)
	{
		strResult = st_msg.mstr_keypad_val;
	}
	else
	{
		strResult.Format("%4.3f", dPrev);
	}

	return strResult;
}

CString CDialog_KeyPad::GetNumEditString_I(int nMin, int nMax, int nPrev, CString strText, CRect rRect)
{
	int mn_response;
	CString strResult;
	
	CDialog_KeyPad pad_dlg;
	
	st_msg.mstr_pad_high_limit.Format("%d", (int)nMax);
	st_msg.mstr_pad_low_limit.Format("%d", (int)nMin);
	st_msg.mstr_keypad_val.Format("%d", (int)nPrev);
	st_msg.mstr_keypad_msg = strText;
	
	st_msg.mn_dot_use = FALSE;
	st_msg.mn_pad_dialog = CTL_NO;

	if (rRect.right > 12800|| rRect.left < 0 || rRect.left > 12800)
	{
		st_msg.mn_pad_dialog = CTL_YES;
	}
	else
	{
		pad_dlg.m_ptRef = CPoint(rRect.right, rRect.top);
		pad_dlg.m_ptRefLeft = CPoint(rRect.left, rRect.top);
	}
	
	mn_response = pad_dlg.DoModal();
	
	if (mn_response == IDOK)
	{
		strResult = st_msg.mstr_keypad_val;
	}
	else
	{
		strResult.Format("%d", (int)nPrev);
	}
	
	return strResult;
}
