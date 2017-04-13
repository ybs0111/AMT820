// Dialog_Keyboard.cpp : implementation file
//

#include "stdafx.h"
#include "handler.h"
#include "Dialog_Keyboard.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDialog_Keyboard dialog


CDialog_Keyboard::CDialog_Keyboard(CWnd* pParent /*=NULL*/)
	: StandardDialog(CDialog_Keyboard::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDialog_Keyboard)
	//}}AFX_DATA_INIT
	m_caption_main = "String Input Dialog Box";
}


void CDialog_Keyboard::DoDataExchange(CDataExchange* pDX)
{
	StandardDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialog_Keyboard)
	DDX_Control(pDX, IDC_MSG_BOARD, m_msg_board);
	DDX_Control(pDX, IDC_GROUP_BOARD, m_group_board);
	DDX_Control(pDX, IDC_BTN_BOARD_SHARP, m_btn_board_sharp);
	DDX_Control(pDX, IDC_BTN_BOARD_RBRACE_S, m_btn_board_rbrace_s);
	DDX_Control(pDX, IDC_BTN_BOARD_RBRACE_B, m_btn_board_rbrace_b);
	DDX_Control(pDX, IDC_BTN_BOARD_LINE, m_btn_board_line);
	DDX_Control(pDX, IDC_BTN_BOARD_LBRACE_S, m_btn_board_lbrace_s);
	DDX_Control(pDX, IDC_BTN_BOARD_LBRACE_B, m_btn_board_lbrace_b);
	DDX_Control(pDX, IDC_BTN_BOARD_DOT, m_btn_board_dot);
	DDX_Control(pDX, IDC_BTN_BOARD_DOLLAR, m_btn_board_dollar);
	DDX_Control(pDX, IDC_BTN_BOARD_DIV, m_btn_board_div);
	DDX_Control(pDX, IDC_BTN_BOARD_DEL, m_btn_board_del);
	DDX_Control(pDX, IDC_BTN_BOARD_BACK, m_btn_board_back);
	DDX_Control(pDX, IDC_BTN_BOARD_ADD, m_btn_board_add);
	DDX_Control(pDX, IDC_BTN_BOARD_Z, m_btn_board_z);
	DDX_Control(pDX, IDC_BTN_BOARD_Y, m_btn_board_y);
	DDX_Control(pDX, IDC_BTN_BOARD_X, m_btn_board_x);
	DDX_Control(pDX, IDC_BTN_BOARD_W, m_btn_board_w);
	DDX_Control(pDX, IDC_BTN_BOARD_V, m_btn_board_v);
	DDX_Control(pDX, IDC_BTN_BOARD_U, m_btn_board_u);
	DDX_Control(pDX, IDC_BTN_BOARD_T, m_btn_board_t);
	DDX_Control(pDX, IDC_BTN_BOARD_S, m_btn_board_s);
	DDX_Control(pDX, IDC_BTN_BOARD_R, m_btn_board_r);
	DDX_Control(pDX, IDC_BTN_BOARD_Q, m_btn_board_q);
	DDX_Control(pDX, IDC_BTN_BOARD_P, m_btn_board_p);
	DDX_Control(pDX, IDC_BTN_BOARD_O, m_btn_board_o);
	DDX_Control(pDX, IDC_BTN_BOARD_N, m_btn_board_n);
	DDX_Control(pDX, IDC_BTN_BOARD_M, m_btn_board_m);
	DDX_Control(pDX, IDC_BTN_BOARD_L, m_btn_board_l);
	DDX_Control(pDX, IDC_BTN_BOARD_K, m_btn_board_k);
	DDX_Control(pDX, IDC_BTN_BOARD_J, m_btn_board_j);
	DDX_Control(pDX, IDC_BTN_BOARD_I, m_btn_board_i);
	DDX_Control(pDX, IDC_BTN_BOARD_H, m_btn_board_h);
	DDX_Control(pDX, IDC_BTN_BOARD_G, m_btn_board_g);
	DDX_Control(pDX, IDC_BTN_BOARD_F, m_btn_board_f);
	DDX_Control(pDX, IDC_BTN_BOARD_E, m_btn_board_e);
	DDX_Control(pDX, IDC_BTN_BOARD_D, m_btn_board_d);
	DDX_Control(pDX, IDC_BTN_BOARD_C, m_btn_board_c);
	DDX_Control(pDX, IDC_BTN_BOARD_B, m_btn_board_b);
	DDX_Control(pDX, IDC_BTN_BOARD_A, m_btn_board_a);
	DDX_Control(pDX, IDC_BTN_BOARD_9, m_btn_board_9);
	DDX_Control(pDX, IDC_BTN_BOARD_8, m_btn_board_8);
	DDX_Control(pDX, IDC_BTN_BOARD_7, m_btn_board_7);
	DDX_Control(pDX, IDC_BTN_BOARD_6, m_btn_board_6);
	DDX_Control(pDX, IDC_BTN_BOARD_5, m_btn_board_5);
	DDX_Control(pDX, IDC_BTN_BOARD_4, m_btn_board_4);
	DDX_Control(pDX, IDC_BTN_BOARD_3, m_btn_board_3);
	DDX_Control(pDX, IDC_BTN_BOARD_2, m_btn_board_2);
	DDX_Control(pDX, IDC_BTN_BOARD_1, m_btn_board_1);
	DDX_Control(pDX, IDC_BTN_BOARD_0, m_btn_board_0);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDialog_Keyboard, StandardDialog)
	//{{AFX_MSG_MAP(CDialog_Keyboard)
	ON_BN_CLICKED(IDC_BTN_BOARD_BACK, OnBtnBoardBack)
	ON_BN_CLICKED(IDC_BTN_BOARD_DEL, OnBtnBoardDel)
	ON_BN_CLICKED(IDC_BTN_BOARD_1, OnBtnBoard1)
	ON_BN_CLICKED(IDC_BTN_BOARD_2, OnBtnBoard2)
	ON_BN_CLICKED(IDC_BTN_BOARD_3, OnBtnBoard3)
	ON_BN_CLICKED(IDC_BTN_BOARD_4, OnBtnBoard4)
	ON_BN_CLICKED(IDC_BTN_BOARD_5, OnBtnBoard5)
	ON_BN_CLICKED(IDC_BTN_BOARD_6, OnBtnBoard6)
	ON_BN_CLICKED(IDC_BTN_BOARD_7, OnBtnBoard7)
	ON_BN_CLICKED(IDC_BTN_BOARD_8, OnBtnBoard8)
	ON_BN_CLICKED(IDC_BTN_BOARD_9, OnBtnBoard9)
	ON_BN_CLICKED(IDC_BTN_BOARD_0, OnBtnBoard0)
	ON_BN_CLICKED(IDC_BTN_BOARD_A, OnBtnBoardA)
	ON_BN_CLICKED(IDC_BTN_BOARD_B, OnBtnBoardB)
	ON_BN_CLICKED(IDC_BTN_BOARD_C, OnBtnBoardC)
	ON_BN_CLICKED(IDC_BTN_BOARD_D, OnBtnBoardD)
	ON_BN_CLICKED(IDC_BTN_BOARD_E, OnBtnBoardE)
	ON_BN_CLICKED(IDC_BTN_BOARD_F, OnBtnBoardF)
	ON_BN_CLICKED(IDC_BTN_BOARD_G, OnBtnBoardG)
	ON_BN_CLICKED(IDC_BTN_BOARD_H, OnBtnBoardH)
	ON_BN_CLICKED(IDC_BTN_BOARD_I, OnBtnBoardI)
	ON_BN_CLICKED(IDC_BTN_BOARD_J, OnBtnBoardJ)
	ON_BN_CLICKED(IDC_BTN_BOARD_K, OnBtnBoardK)
	ON_BN_CLICKED(IDC_BTN_BOARD_L, OnBtnBoardL)
	ON_BN_CLICKED(IDC_BTN_BOARD_M, OnBtnBoardM)
	ON_BN_CLICKED(IDC_BTN_BOARD_N, OnBtnBoardN)
	ON_BN_CLICKED(IDC_BTN_BOARD_O, OnBtnBoardO)
	ON_BN_CLICKED(IDC_BTN_BOARD_P, OnBtnBoardP)
	ON_BN_CLICKED(IDC_BTN_BOARD_Q, OnBtnBoardQ)
	ON_BN_CLICKED(IDC_BTN_BOARD_R, OnBtnBoardR)
	ON_BN_CLICKED(IDC_BTN_BOARD_S, OnBtnBoardS)
	ON_BN_CLICKED(IDC_BTN_BOARD_T, OnBtnBoardT)
	ON_BN_CLICKED(IDC_BTN_BOARD_U, OnBtnBoardU)
	ON_BN_CLICKED(IDC_BTN_BOARD_V, OnBtnBoardV)
	ON_BN_CLICKED(IDC_BTN_BOARD_W, OnBtnBoardW)
	ON_BN_CLICKED(IDC_BTN_BOARD_X, OnBtnBoardX)
	ON_BN_CLICKED(IDC_BTN_BOARD_Y, OnBtnBoardY)
	ON_BN_CLICKED(IDC_BTN_BOARD_Z, OnBtnBoardZ)
	ON_BN_CLICKED(IDC_BTN_BOARD_LBRACE_S, OnBtnBoardLbraceS)
	ON_BN_CLICKED(IDC_BTN_BOARD_RBRACE_S, OnBtnBoardRbraceS)
	ON_BN_CLICKED(IDC_BTN_BOARD_DIV, OnBtnBoardDiv)
	ON_BN_CLICKED(IDC_BTN_BOARD_LBRACE_B, OnBtnBoardLbraceB)
	ON_BN_CLICKED(IDC_BTN_BOARD_RBRACE_B, OnBtnBoardRbraceB)
	ON_BN_CLICKED(IDC_BTN_BOARD_DOT, OnBtnBoardDot)
	ON_BN_CLICKED(IDC_BTN_BOARD_ADD, OnBtnBoardAdd)
	ON_BN_CLICKED(IDC_BTN_BOARD_LINE, OnBtnBoardLine)
	ON_BN_CLICKED(IDC_BTN_BOARD_DOLLAR, OnBtnBoardDollar)
	ON_BN_CLICKED(IDC_BTN_BOARD_SHARP, OnBtnBoardSharp)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialog_Keyboard message handlers

BOOL CDialog_Keyboard::OnInitDialog() 
{
	StandardDialog::OnInitDialog();
	
	/* ************************************************************************** */
	/* 컨트롤에 설정할 폰트 생성한다                                              */
	/* ************************************************************************** */
	mp_board_font = NULL;
	mp_board_font = new CFont;
	mp_board_font->CreateFont(15,0,0,0,900,0,0,0,0,0,0,ANTIALIASED_QUALITY,0,"MS Sans Serif");
	/* ************************************************************************** */

	OnKeyboard_GroupBox_Set();		// 키보드 대화 상자 칼라 그룹 박스 생성 함수
	OnKeyboard_Lable_Set();			// 키보드 대화 상자 칼라 텍스트 박스 생성 함수
	OnKeyboard_EditBoxSet();		// 키보드 대화 상자 칼라 에디트 박스 생성 함수
	OnKeyboard_Button_Set();		// 키보드 대화 상자 칼라 버튼 생성 함수
	OnKeyboard_Title_data_Set();	// 메시지 정보 클래스 변수 설정 및 키보드 화면 출력 함수
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}



void CDialog_Keyboard::OnKeyboard_GroupBox_Set()
{
	CSxLogFont board_Logo(15,FW_SEMIBOLD,false,"MS Sans Serif");

//	m_group_board.SetXPGroupStyle(CXPGroupBox::XPGB_WINDOW);
//	m_group_board.SetBackgroundColor(RGB(201, 201, 151), RGB(255, 255, 204));
	m_group_board.SetFont(board_Logo);
	m_group_board.SetCatptionTextColor(RGB(145,25,0));
	m_group_board.SetFontBold(TRUE);
}

void CDialog_Keyboard::OnKeyboard_Lable_Set()
{
	m_msg_board.SetFont(mp_board_font);
	m_msg_board.SetWindowText(_T("Input : String Data~"));
	m_msg_board.SetCenterText();
	m_msg_board.SetColor(RGB(0,0,255));
	m_msg_board.SetGradientColor(RGB(0,0,0));
	m_msg_board.SetTextColor(RGB(255,255,255));
}

void CDialog_Keyboard::OnKeyboard_EditBoxSet()
{
	m_edit_board_data.SubclassDlgItem(IDC_EDIT_BOARD_DATA, this);
	m_edit_board_data.bkColor( RGB(50, 100, 150) );
	m_edit_board_data.textColor( RGB(255, 255,255) );
    m_edit_board_data.setFont(-16, FW_BOLD, DEFAULT_PITCH | FF_DONTCARE, "MS Sans Serif");
}

void CDialog_Keyboard::OnKeyboard_Button_Set()
{
	mp_board_ok = CreateOkButton(IDOK, this);
	mp_board_cancel = CreateCancelButton(IDCANCEL, this);
}

void CDialog_Keyboard::OnKeyboard_Title_data_Set()
{
	mp_title_info = st_other.str_typing_msg;  // 전역 변수에 저장된 타이틀 정보 클래스 변수에 설정 
	((CEdit*)GetDlgItem(IDC_EDIT_BOARD_DATA))->SetWindowText(mp_title_info);
}

void CDialog_Keyboard::OnKeyboard_Title_data_Transfer()
{
	((CEdit*)GetDlgItem(IDC_EDIT_BOARD_DATA))->GetWindowText(mp_title_info) ;

	mp_title_info.TrimLeft(' ');               
	mp_title_info.TrimRight(' ');
}

BOOL CDialog_Keyboard::OnKeyboard_Length_Check(CString strData)
{
	int mn_cnt;

	mn_cnt = strlen(strData);
	if (mn_cnt > 15) return FALSE;

	return TRUE;
}

void CDialog_Keyboard::OnBtnBoardBack() 
{
	CString Data;
	int nCnt;

	((CEdit*)GetDlgItem(IDC_EDIT_BOARD_DATA))->GetWindowText(mp_title_info);

	nCnt=strlen(mp_title_info);
	
	if (nCnt == 0) return;
	else 
	{
		Data = mp_title_info;
		mp_title_info=Data.Left(nCnt-1);
		((CEdit*)GetDlgItem(IDC_EDIT_BOARD_DATA))->SetWindowText(mp_title_info);
	}
}

void CDialog_Keyboard::OnBtnBoardDel() 
{
	mp_title_info =_T("") ;
	((CEdit*)GetDlgItem(IDC_EDIT_BOARD_DATA))->SetWindowText(mp_title_info);
}

void CDialog_Keyboard::OnOK() 
{
	OnKeyboard_Title_data_Transfer();			// 키보드 입력 정보 전역 변수에 설정 함수

	st_other.str_typing_msg = mp_title_info;	// 클래스 변수에 저장된 타이틀 정보 전역 변수에 설정 

	StandardDialog::OnOK();
}

void CDialog_Keyboard::OnBtnBoard1() 
{
	((CEdit*)GetDlgItem(IDC_EDIT_BOARD_DATA))->GetWindowText(mp_title_info);

	if(!OnKeyboard_Length_Check(mp_title_info))  return;   // 입력 디바이스 정보 Length 체크 함수
	else 
	{
		mp_title_info +=_T("1");
		((CEdit*)GetDlgItem(IDC_EDIT_BOARD_DATA))->SetWindowText(mp_title_info);
	}
}

void CDialog_Keyboard::OnBtnBoard2() 
{
	((CEdit*)GetDlgItem(IDC_EDIT_BOARD_DATA))->GetWindowText(mp_title_info);

	if(!OnKeyboard_Length_Check(mp_title_info))  return;   // 입력 디바이스 정보 Length 체크 함수
	else 
	{
		mp_title_info +=_T("2");
		((CEdit*)GetDlgItem(IDC_EDIT_BOARD_DATA))->SetWindowText(mp_title_info);
	}
}

void CDialog_Keyboard::OnBtnBoard3() 
{
	((CEdit*)GetDlgItem(IDC_EDIT_BOARD_DATA))->GetWindowText(mp_title_info);

	if(!OnKeyboard_Length_Check(mp_title_info))  return;   // 입력 디바이스 정보 Length 체크 함수
	else 
	{
		mp_title_info +=_T("3");
		((CEdit*)GetDlgItem(IDC_EDIT_BOARD_DATA))->SetWindowText(mp_title_info);
	}
}

void CDialog_Keyboard::OnBtnBoard4() 
{
	((CEdit*)GetDlgItem(IDC_EDIT_BOARD_DATA))->GetWindowText(mp_title_info);

	if(!OnKeyboard_Length_Check(mp_title_info))  return;   // 입력 디바이스 정보 Length 체크 함수
	else 
	{
		mp_title_info +=_T("4");
		((CEdit*)GetDlgItem(IDC_EDIT_BOARD_DATA))->SetWindowText(mp_title_info);
	}
}

void CDialog_Keyboard::OnBtnBoard5() 
{
	((CEdit*)GetDlgItem(IDC_EDIT_BOARD_DATA))->GetWindowText(mp_title_info);

	if(!OnKeyboard_Length_Check(mp_title_info))  return;   // 입력 디바이스 정보 Length 체크 함수
	else 
	{
		mp_title_info +=_T("5");
		((CEdit*)GetDlgItem(IDC_EDIT_BOARD_DATA))->SetWindowText(mp_title_info);
	}
}

void CDialog_Keyboard::OnBtnBoard6() 
{
	((CEdit*)GetDlgItem(IDC_EDIT_BOARD_DATA))->GetWindowText(mp_title_info);

	if(!OnKeyboard_Length_Check(mp_title_info))  return;   // 입력 디바이스 정보 Length 체크 함수
	else 
	{
		mp_title_info +=_T("6");
		((CEdit*)GetDlgItem(IDC_EDIT_BOARD_DATA))->SetWindowText(mp_title_info);
	}
}

void CDialog_Keyboard::OnBtnBoard7() 
{
	((CEdit*)GetDlgItem(IDC_EDIT_BOARD_DATA))->GetWindowText(mp_title_info);

	if(!OnKeyboard_Length_Check(mp_title_info))  return;   // 입력 디바이스 정보 Length 체크 함수
	else 
	{
		mp_title_info +=_T("7");
		((CEdit*)GetDlgItem(IDC_EDIT_BOARD_DATA))->SetWindowText(mp_title_info);
	}
}

void CDialog_Keyboard::OnBtnBoard8() 
{
	((CEdit*)GetDlgItem(IDC_EDIT_BOARD_DATA))->GetWindowText(mp_title_info);

	if(!OnKeyboard_Length_Check(mp_title_info))  return;   // 입력 디바이스 정보 Length 체크 함수
	else 
	{
		mp_title_info +=_T("8");
		((CEdit*)GetDlgItem(IDC_EDIT_BOARD_DATA))->SetWindowText(mp_title_info);
	}
}

void CDialog_Keyboard::OnBtnBoard9() 
{
	((CEdit*)GetDlgItem(IDC_EDIT_BOARD_DATA))->GetWindowText(mp_title_info);

	if(!OnKeyboard_Length_Check(mp_title_info))  return;   // 입력 디바이스 정보 Length 체크 함수
	else 
	{
		mp_title_info +=_T("9");
		((CEdit*)GetDlgItem(IDC_EDIT_BOARD_DATA))->SetWindowText(mp_title_info);
	}
}

void CDialog_Keyboard::OnBtnBoard0() 
{
	((CEdit*)GetDlgItem(IDC_EDIT_BOARD_DATA))->GetWindowText(mp_title_info);

	if(!OnKeyboard_Length_Check(mp_title_info))  return;   // 입력 디바이스 정보 Length 체크 함수
	else 
	{
		mp_title_info +=_T("0");
		((CEdit*)GetDlgItem(IDC_EDIT_BOARD_DATA))->SetWindowText(mp_title_info);
	}
}

void CDialog_Keyboard::OnBtnBoardA() 
{
	((CEdit*)GetDlgItem(IDC_EDIT_BOARD_DATA))->GetWindowText(mp_title_info);

	if(!OnKeyboard_Length_Check(mp_title_info))  return;   // 입력 디바이스 정보 Length 체크 함수
	else 
	{
		mp_title_info +=_T("A");
		((CEdit*)GetDlgItem(IDC_EDIT_BOARD_DATA))->SetWindowText(mp_title_info);
	}
}

void CDialog_Keyboard::OnBtnBoardB() 
{
	((CEdit*)GetDlgItem(IDC_EDIT_BOARD_DATA))->GetWindowText(mp_title_info);

	if(!OnKeyboard_Length_Check(mp_title_info))  return;   // 입력 디바이스 정보 Length 체크 함수
	else 
	{
		mp_title_info +=_T("B");
		((CEdit*)GetDlgItem(IDC_EDIT_BOARD_DATA))->SetWindowText(mp_title_info);
	}
}

void CDialog_Keyboard::OnBtnBoardC() 
{
	((CEdit*)GetDlgItem(IDC_EDIT_BOARD_DATA))->GetWindowText(mp_title_info);

	if(!OnKeyboard_Length_Check(mp_title_info))  return;   // 입력 디바이스 정보 Length 체크 함수
	else 
	{
		mp_title_info +=_T("C");
		((CEdit*)GetDlgItem(IDC_EDIT_BOARD_DATA))->SetWindowText(mp_title_info);
	}
}

void CDialog_Keyboard::OnBtnBoardD() 
{
	((CEdit*)GetDlgItem(IDC_EDIT_BOARD_DATA))->GetWindowText(mp_title_info);

	if(!OnKeyboard_Length_Check(mp_title_info))  return;   // 입력 디바이스 정보 Length 체크 함수
	else 
	{
		mp_title_info +=_T("D");
		((CEdit*)GetDlgItem(IDC_EDIT_BOARD_DATA))->SetWindowText(mp_title_info);
	}
}

void CDialog_Keyboard::OnBtnBoardE() 
{
	((CEdit*)GetDlgItem(IDC_EDIT_BOARD_DATA))->GetWindowText(mp_title_info);

	if(!OnKeyboard_Length_Check(mp_title_info))  return;   // 입력 디바이스 정보 Length 체크 함수
	else 
	{
		mp_title_info +=_T("E");
		((CEdit*)GetDlgItem(IDC_EDIT_BOARD_DATA))->SetWindowText(mp_title_info);
	}
}

void CDialog_Keyboard::OnBtnBoardF() 
{
	((CEdit*)GetDlgItem(IDC_EDIT_BOARD_DATA))->GetWindowText(mp_title_info);

	if(!OnKeyboard_Length_Check(mp_title_info))  return;   // 입력 디바이스 정보 Length 체크 함수
	else 
	{
		mp_title_info +=_T("F");
		((CEdit*)GetDlgItem(IDC_EDIT_BOARD_DATA))->SetWindowText(mp_title_info);
	}
}

void CDialog_Keyboard::OnBtnBoardG() 
{
	((CEdit*)GetDlgItem(IDC_EDIT_BOARD_DATA))->GetWindowText(mp_title_info);

	if(!OnKeyboard_Length_Check(mp_title_info))  return;   // 입력 디바이스 정보 Length 체크 함수
	else 
	{
		mp_title_info +=_T("G");
		((CEdit*)GetDlgItem(IDC_EDIT_BOARD_DATA))->SetWindowText(mp_title_info);
	}
}

void CDialog_Keyboard::OnBtnBoardH() 
{
	((CEdit*)GetDlgItem(IDC_EDIT_BOARD_DATA))->GetWindowText(mp_title_info);

	if(!OnKeyboard_Length_Check(mp_title_info))  return;   // 입력 디바이스 정보 Length 체크 함수
	else 
	{
		mp_title_info +=_T("H");
		((CEdit*)GetDlgItem(IDC_EDIT_BOARD_DATA))->SetWindowText(mp_title_info);
	}
}

void CDialog_Keyboard::OnBtnBoardI() 
{
	((CEdit*)GetDlgItem(IDC_EDIT_BOARD_DATA))->GetWindowText(mp_title_info);

	if(!OnKeyboard_Length_Check(mp_title_info))  return;   // 입력 디바이스 정보 Length 체크 함수
	else 
	{
		mp_title_info +=_T("I");
		((CEdit*)GetDlgItem(IDC_EDIT_BOARD_DATA))->SetWindowText(mp_title_info);
	}
}

void CDialog_Keyboard::OnBtnBoardJ() 
{
	((CEdit*)GetDlgItem(IDC_EDIT_BOARD_DATA))->GetWindowText(mp_title_info);

	if(!OnKeyboard_Length_Check(mp_title_info))  return;   // 입력 디바이스 정보 Length 체크 함수
	else 
	{
		mp_title_info +=_T("J");
		((CEdit*)GetDlgItem(IDC_EDIT_BOARD_DATA))->SetWindowText(mp_title_info);
	}
}

void CDialog_Keyboard::OnBtnBoardK() 
{
	((CEdit*)GetDlgItem(IDC_EDIT_BOARD_DATA))->GetWindowText(mp_title_info);

	if(!OnKeyboard_Length_Check(mp_title_info))  return;   // 입력 디바이스 정보 Length 체크 함수
	else 
	{
		mp_title_info +=_T("K");
		((CEdit*)GetDlgItem(IDC_EDIT_BOARD_DATA))->SetWindowText(mp_title_info);
	}
}

void CDialog_Keyboard::OnBtnBoardL() 
{
	((CEdit*)GetDlgItem(IDC_EDIT_BOARD_DATA))->GetWindowText(mp_title_info);

	if(!OnKeyboard_Length_Check(mp_title_info))  return;   // 입력 디바이스 정보 Length 체크 함수
	else 
	{
		mp_title_info +=_T("L");
		((CEdit*)GetDlgItem(IDC_EDIT_BOARD_DATA))->SetWindowText(mp_title_info);
	}
}

void CDialog_Keyboard::OnBtnBoardM() 
{
	((CEdit*)GetDlgItem(IDC_EDIT_BOARD_DATA))->GetWindowText(mp_title_info);

	if(!OnKeyboard_Length_Check(mp_title_info))  return;   // 입력 디바이스 정보 Length 체크 함수
	else 
	{
		mp_title_info +=_T("M");
		((CEdit*)GetDlgItem(IDC_EDIT_BOARD_DATA))->SetWindowText(mp_title_info);
	}
}

void CDialog_Keyboard::OnBtnBoardN() 
{
	((CEdit*)GetDlgItem(IDC_EDIT_BOARD_DATA))->GetWindowText(mp_title_info);

	if(!OnKeyboard_Length_Check(mp_title_info))  return;   // 입력 디바이스 정보 Length 체크 함수
	else 
	{
		mp_title_info +=_T("N");
		((CEdit*)GetDlgItem(IDC_EDIT_BOARD_DATA))->SetWindowText(mp_title_info);
	}
}

void CDialog_Keyboard::OnBtnBoardO() 
{
	((CEdit*)GetDlgItem(IDC_EDIT_BOARD_DATA))->GetWindowText(mp_title_info);

	if(!OnKeyboard_Length_Check(mp_title_info))  return;   // 입력 디바이스 정보 Length 체크 함수
	else 
	{
		mp_title_info +=_T("O");
		((CEdit*)GetDlgItem(IDC_EDIT_BOARD_DATA))->SetWindowText(mp_title_info);
	}
}

void CDialog_Keyboard::OnBtnBoardP() 
{
	((CEdit*)GetDlgItem(IDC_EDIT_BOARD_DATA))->GetWindowText(mp_title_info);

	if(!OnKeyboard_Length_Check(mp_title_info))  return;   // 입력 디바이스 정보 Length 체크 함수
	else 
	{
		mp_title_info +=_T("P");
		((CEdit*)GetDlgItem(IDC_EDIT_BOARD_DATA))->SetWindowText(mp_title_info);
	}
}

void CDialog_Keyboard::OnBtnBoardQ() 
{
	((CEdit*)GetDlgItem(IDC_EDIT_BOARD_DATA))->GetWindowText(mp_title_info);

	if(!OnKeyboard_Length_Check(mp_title_info))  return;   // 입력 디바이스 정보 Length 체크 함수
	else 
	{
		mp_title_info +=_T("Q");
		((CEdit*)GetDlgItem(IDC_EDIT_BOARD_DATA))->SetWindowText(mp_title_info);
	}
}

void CDialog_Keyboard::OnBtnBoardR() 
{
	((CEdit*)GetDlgItem(IDC_EDIT_BOARD_DATA))->GetWindowText(mp_title_info);

	if(!OnKeyboard_Length_Check(mp_title_info))  return;   // 입력 디바이스 정보 Length 체크 함수
	else 
	{
		mp_title_info +=_T("R");
		((CEdit*)GetDlgItem(IDC_EDIT_BOARD_DATA))->SetWindowText(mp_title_info);
	}
}

void CDialog_Keyboard::OnBtnBoardS() 
{
	((CEdit*)GetDlgItem(IDC_EDIT_BOARD_DATA))->GetWindowText(mp_title_info);

	if(!OnKeyboard_Length_Check(mp_title_info))  return;   // 입력 디바이스 정보 Length 체크 함수
	else 
	{
		mp_title_info +=_T("S");
		((CEdit*)GetDlgItem(IDC_EDIT_BOARD_DATA))->SetWindowText(mp_title_info);
	}
}

void CDialog_Keyboard::OnBtnBoardT() 
{
	((CEdit*)GetDlgItem(IDC_EDIT_BOARD_DATA))->GetWindowText(mp_title_info);

	if(!OnKeyboard_Length_Check(mp_title_info))  return;   // 입력 디바이스 정보 Length 체크 함수
	else 
	{
		mp_title_info +=_T("T");
		((CEdit*)GetDlgItem(IDC_EDIT_BOARD_DATA))->SetWindowText(mp_title_info);
	}
}

void CDialog_Keyboard::OnBtnBoardU() 
{
	((CEdit*)GetDlgItem(IDC_EDIT_BOARD_DATA))->GetWindowText(mp_title_info);

	if(!OnKeyboard_Length_Check(mp_title_info))  return;   // 입력 디바이스 정보 Length 체크 함수
	else 
	{
		mp_title_info +=_T("U");
		((CEdit*)GetDlgItem(IDC_EDIT_BOARD_DATA))->SetWindowText(mp_title_info);
	}
}

void CDialog_Keyboard::OnBtnBoardV() 
{
	((CEdit*)GetDlgItem(IDC_EDIT_BOARD_DATA))->GetWindowText(mp_title_info);

	if(!OnKeyboard_Length_Check(mp_title_info))  return;   // 입력 디바이스 정보 Length 체크 함수
	else 
	{
		mp_title_info +=_T("V");
		((CEdit*)GetDlgItem(IDC_EDIT_BOARD_DATA))->SetWindowText(mp_title_info);
	}
}

void CDialog_Keyboard::OnBtnBoardW() 
{
	((CEdit*)GetDlgItem(IDC_EDIT_BOARD_DATA))->GetWindowText(mp_title_info);

	if(!OnKeyboard_Length_Check(mp_title_info))  return;   // 입력 디바이스 정보 Length 체크 함수
	else 
	{
		mp_title_info +=_T("W");
		((CEdit*)GetDlgItem(IDC_EDIT_BOARD_DATA))->SetWindowText(mp_title_info);
	}
}

void CDialog_Keyboard::OnBtnBoardX() 
{
	((CEdit*)GetDlgItem(IDC_EDIT_BOARD_DATA))->GetWindowText(mp_title_info);

	if(!OnKeyboard_Length_Check(mp_title_info))  return;   // 입력 디바이스 정보 Length 체크 함수
	else 
	{
		mp_title_info +=_T("X");
		((CEdit*)GetDlgItem(IDC_EDIT_BOARD_DATA))->SetWindowText(mp_title_info);
	}
}

void CDialog_Keyboard::OnBtnBoardY() 
{
	((CEdit*)GetDlgItem(IDC_EDIT_BOARD_DATA))->GetWindowText(mp_title_info);

	if(!OnKeyboard_Length_Check(mp_title_info))  return;   // 입력 디바이스 정보 Length 체크 함수
	else 
	{
		mp_title_info +=_T("Y");
		((CEdit*)GetDlgItem(IDC_EDIT_BOARD_DATA))->SetWindowText(mp_title_info);
	}
}

void CDialog_Keyboard::OnBtnBoardZ() 
{
	((CEdit*)GetDlgItem(IDC_EDIT_BOARD_DATA))->GetWindowText(mp_title_info);

	if(!OnKeyboard_Length_Check(mp_title_info))  return;   // 입력 디바이스 정보 Length 체크 함수
	else 
	{
		mp_title_info +=_T("Z");
		((CEdit*)GetDlgItem(IDC_EDIT_BOARD_DATA))->SetWindowText(mp_title_info);
	}
}

void CDialog_Keyboard::OnBtnBoardLbraceS() 
{
	((CEdit*)GetDlgItem(IDC_EDIT_BOARD_DATA))->GetWindowText(mp_title_info);

	if(!OnKeyboard_Length_Check(mp_title_info))  return;   // 입력 디바이스 정보 Length 체크 함수
	else 
	{
		mp_title_info +=_T("(");
		((CEdit*)GetDlgItem(IDC_EDIT_BOARD_DATA))->SetWindowText(mp_title_info);
	}
}

void CDialog_Keyboard::OnBtnBoardRbraceS() 
{
	((CEdit*)GetDlgItem(IDC_EDIT_BOARD_DATA))->GetWindowText(mp_title_info);

	if(!OnKeyboard_Length_Check(mp_title_info))  return;   // 입력 디바이스 정보 Length 체크 함수
	else 
	{
		mp_title_info +=_T(")");
		((CEdit*)GetDlgItem(IDC_EDIT_BOARD_DATA))->SetWindowText(mp_title_info);
	}
}

void CDialog_Keyboard::OnBtnBoardDiv() 
{
	((CEdit*)GetDlgItem(IDC_EDIT_BOARD_DATA))->GetWindowText(mp_title_info);

	if(!OnKeyboard_Length_Check(mp_title_info))  return;   // 입력 디바이스 정보 Length 체크 함수
	else 
	{
		mp_title_info +=_T("/");
		((CEdit*)GetDlgItem(IDC_EDIT_BOARD_DATA))->SetWindowText(mp_title_info);
	}
}

void CDialog_Keyboard::OnBtnBoardLbraceB() 
{
	((CEdit*)GetDlgItem(IDC_EDIT_BOARD_DATA))->GetWindowText(mp_title_info);

	if(!OnKeyboard_Length_Check(mp_title_info))  return;   // 입력 디바이스 정보 Length 체크 함수
	else 
	{
		mp_title_info +=_T("[");
		((CEdit*)GetDlgItem(IDC_EDIT_BOARD_DATA))->SetWindowText(mp_title_info);
	}
}

void CDialog_Keyboard::OnBtnBoardRbraceB() 
{
	((CEdit*)GetDlgItem(IDC_EDIT_BOARD_DATA))->GetWindowText(mp_title_info);

	if(!OnKeyboard_Length_Check(mp_title_info))  return;   // 입력 디바이스 정보 Length 체크 함수
	else 
	{
		mp_title_info +=_T("]");
		((CEdit*)GetDlgItem(IDC_EDIT_BOARD_DATA))->SetWindowText(mp_title_info);
	}
}

void CDialog_Keyboard::OnBtnBoardDot() 
{
	((CEdit*)GetDlgItem(IDC_EDIT_BOARD_DATA))->GetWindowText(mp_title_info);

	if(!OnKeyboard_Length_Check(mp_title_info))  return;   // 입력 디바이스 정보 Length 체크 함수
	else 
	{
		mp_title_info +=_T(".");
		((CEdit*)GetDlgItem(IDC_EDIT_BOARD_DATA))->SetWindowText(mp_title_info);
	}
}

void CDialog_Keyboard::OnBtnBoardAdd() 
{
	((CEdit*)GetDlgItem(IDC_EDIT_BOARD_DATA))->GetWindowText(mp_title_info);

	if(!OnKeyboard_Length_Check(mp_title_info))  return;   // 입력 디바이스 정보 Length 체크 함수
	else 
	{
		mp_title_info +=_T("+");
		((CEdit*)GetDlgItem(IDC_EDIT_BOARD_DATA))->SetWindowText(mp_title_info);
	}
}

void CDialog_Keyboard::OnBtnBoardLine() 
{
	((CEdit*)GetDlgItem(IDC_EDIT_BOARD_DATA))->GetWindowText(mp_title_info);

	if(!OnKeyboard_Length_Check(mp_title_info))  return;   // 입력 디바이스 정보 Length 체크 함수
	else 
	{
		mp_title_info +=_T("-");
		((CEdit*)GetDlgItem(IDC_EDIT_BOARD_DATA))->SetWindowText(mp_title_info);
	}
}

void CDialog_Keyboard::OnBtnBoardDollar() 
{
	((CEdit*)GetDlgItem(IDC_EDIT_BOARD_DATA))->GetWindowText(mp_title_info);

	if(!OnKeyboard_Length_Check(mp_title_info))  return;   // 입력 디바이스 정보 Length 체크 함수
	else 
	{
		mp_title_info +=_T("$");
		((CEdit*)GetDlgItem(IDC_EDIT_BOARD_DATA))->SetWindowText(mp_title_info);
	}
}

void CDialog_Keyboard::OnBtnBoardSharp() 
{
	((CEdit*)GetDlgItem(IDC_EDIT_BOARD_DATA))->GetWindowText(mp_title_info);

	if(!OnKeyboard_Length_Check(mp_title_info))  return;   // 입력 디바이스 정보 Length 체크 함수
	else 
	{
		mp_title_info +=_T("#");
		((CEdit*)GetDlgItem(IDC_EDIT_BOARD_DATA))->SetWindowText(mp_title_info);
	}
}

BOOL CDialog_Keyboard::DestroyWindow() 
{
	// TODO: Add your specialized code here and/or call the base class
	/* ************************************************************************** */
	/* 생성한 폰트 정보 삭제한다                                                  */
	/* ************************************************************************** */
	delete mp_board_font;
	mp_board_font = NULL;
	/* ************************************************************************** */

	delete mp_board_ok;		// OK 버튼에 설정된 정보 삭제 
	delete mp_board_cancel; // Cancel 버튼에 설정된 정보 삭제 
	return StandardDialog::DestroyWindow();
}
