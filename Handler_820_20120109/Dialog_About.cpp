// Dialog_About.cpp : implementation file
//

#include "stdafx.h"
#include "handler.h"
#include "Dialog_About.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDialog_About dialog


CDialog_About::CDialog_About(CWnd* pParent /*=NULL*/)
	: StandardDialog(CDialog_About::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDialog_About)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_caption_main = "MODULE HANDLER About Dialog Box";
}


void CDialog_About::DoDataExchange(CDataExchange* pDX)
{
	StandardDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialog_About)
	DDX_Control(pDX, IDC_PICTURE_ABOUT, m_picture_about);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDialog_About, StandardDialog)
	//{{AFX_MSG_MAP(CDialog_About)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialog_About message handlers


BOOL CDialog_About::OnInitDialog() 
{
	StandardDialog::OnInitDialog();
	
	OnAbout_Button_Set();			// 대화 상자 칼라 버튼 생성 함수

	OnAbout_StarWars_Control_Set();	// 스타워즈 형태의 컨트롤 생성 함수
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}



void CDialog_About::OnAbout_Button_Set()
{
	mp_about_ok = CreateOkButton(IDOK, this);
}

void CDialog_About::OnAbout_StarWars_Control_Set()
{
	m_picture_about.SetStarSpeed(30);	// 백 그라운드 (별 모양)이 움직이는 속도 설정 
	m_picture_about.SetScrollSpeed(2);	// 버전 정보가 움직이는 속도 설정 

	m_picture_about.AddTextLine("MODULE HANDLER AMT820");
	m_picture_about.AddTextLine("");
	m_picture_about.AddTextLine("Motion Control System");
	m_picture_about.AddTextLine("&");
	m_picture_about.AddTextLine("Human Machine Interface");
	m_picture_about.AddTextLine("");
	m_picture_about.AddTextLine("Version : 1.0.0");
	m_picture_about.AddTextLine("");
	m_picture_about.AddTextLine("");
	m_picture_about.AddTextLine("Development Year : 2017-01");
	m_picture_about.AddTextLine("Development a company : AMT");
	m_picture_about.AddTextLine("Screen Size : 1280*1024");
	m_picture_about.AddTextLine("");
	m_picture_about.AddTextLine("This application was programmed by");
	m_picture_about.AddTextLine("");
	m_picture_about.AddTextLine("ViboX, Kim");
	m_picture_about.AddTextLine("");
	m_picture_about.AddTextLine("Copyright(C) 2002.06.19");
	m_picture_about.AddTextLine("Advanced Mechatronics Tech'. Co.");
	m_picture_about.AddTextLine("");
	m_picture_about.AddTextLine("http://www.imamt.com");
}

BOOL CDialog_About::DestroyWindow() 
{
	// TODO: Add your specialized code here and/or call the base class
	delete mp_about_ok;				// OK 버튼에 대한 정보 삭제 
	return StandardDialog::DestroyWindow();
}
