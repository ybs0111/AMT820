// Dialog_Select.cpp : implementation file
//

#include "stdafx.h"
#include "handler.h"
#include "Dialog_Select.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDialog_Select dialog


CDialog_Select::CDialog_Select(CWnd* pParent /*=NULL*/)
	: StandardDialog(CDialog_Select::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDialog_Select)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_caption_main = "User Select Dialog Box"; 
//	m_is_shape_3d = YES;
}


void CDialog_Select::DoDataExchange(CDataExchange* pDX)
{
	StandardDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialog_Select)
	DDX_Control(pDX, IDC_SELECT_MESSAGE, m_select_message);
	DDX_Control(pDX, IDC_GIF_SELECT, m_gif_select);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDialog_Select, StandardDialog)
	//{{AFX_MSG_MAP(CDialog_Select)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialog_Select message handlers

BOOL CDialog_Select::OnInitDialog() 
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
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}



void CDialog_Select::OnSelect_Picture_Set()
{
	
 	if (m_gif_select.Load(MAKEINTRESOURCE(IDR_GIF_CHOICE),_T("GIF")))
 		m_gif_select.Draw();
}

void CDialog_Select::OnSelect_Lable_Set()
{
	m_select_message.SetFont(mp_select_font);
	m_select_message.SetWindowText(_T(st_other.str_confirm_msg));
	m_select_message.SetCenterText();
	m_select_message.SetColor(RGB(0,0,255));
	m_select_message.SetGradientColor(RGB(0,0,0));
	m_select_message.SetTextColor(RGB(255,255,255));
}

void CDialog_Select::OnSelect_Button_Set()
{
	mp_select_yes = CreateOkButton(IDOK, this);
	mp_select_no = CreateCancelButton(IDCANCEL, this);
}

BOOL CDialog_Select::DestroyWindow() 
{
	// TODO: Add your specialized code here and/or call the base class
	/* ************************************************************************** */
	/* 생성한 폰트 정보 삭제한다                                                  */
	/* ************************************************************************** */
	delete mp_select_font;
	mp_select_font = NULL;
	/* ************************************************************************** */

	delete mp_select_yes;	// OK 버튼에 설정된 정보 삭제 
	delete mp_select_no;	// Cancel 버튼에 설정된 정보 삭제 

	if (mn_dialog == 1)
	{
		st_handler.n_switch_lock = FALSE;
	}

	return StandardDialog::DestroyWindow();
}
