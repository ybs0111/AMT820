// Dialog_Message.cpp : implementation file
//

#include "stdafx.h"
#include "handler.h"
#include "Dialog_Message.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDialog_Message dialog

CDialog_Message::CDialog_Message(CWnd* pParent /*=NULL*/)
	: StandardDialog(CDialog_Message::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDialog_Message)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_caption_main = "User Message Dialog Box";
	mp_msg_ok = NULL;
}

void CDialog_Message::DoDataExchange(CDataExchange* pDX)
{
	StandardDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialog_Message)
	DDX_Control(pDX, IDC_MSG_MESSAGE, m_msg_message);
	DDX_Control(pDX, IDC_GIF_MESSAGE, m_gif_message);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDialog_Message, StandardDialog)
	//{{AFX_MSG_MAP(CDialog_Message)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialog_Message message handlers

BOOL CDialog_Message::OnInitDialog() 
{
	StandardDialog::OnInitDialog();
	
	/* ************************************************************************** */
	/* ��Ʈ�ѿ� ������ ��Ʈ �����Ѵ�                                              */
	/* ************************************************************************** */
	mp_msg_font = NULL;
	mp_msg_font = new CFont;
	mp_msg_font->CreateFont(15,0,0,0,900,0,0,0,0,0,0,ANTIALIASED_QUALITY,0,"MS Sans Serif");
	/* ************************************************************************** */
	
	OnMsg_Button_Set(); // ��ȭ ���� Į�� ��ư ���� �Լ�
	OnMsg_Lable_Set();  // Į�� �ؽ�Ʈ �ڽ� ���� �Լ�
	OnMsg_Picture_Set();// GIF �׸� ���� �ε� �Լ�
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}



void CDialog_Message::OnMsg_Button_Set()
{
	mp_msg_ok = CreateOkButton(IDOK, this);
}

void CDialog_Message::OnMsg_Lable_Set()
{
	m_msg_message.SetFont(mp_msg_font);
	m_msg_message.SetWindowText(_T(st_other.str_fallacy_msg));
	m_msg_message.SetCenterText();
	m_msg_message.SetColor(RGB(0,0,255));
	m_msg_message.SetGradientColor(RGB(0,0,0));
	m_msg_message.SetTextColor(RGB(255,255,255));
}

void CDialog_Message::OnMsg_Picture_Set()
{
	if (m_gif_message.Load(MAKEINTRESOURCE(IDR_GIF_MSG),_T("GIF")))
		m_gif_message.Draw();
}

void CDialog_Message::OnOK() 
{
	// TODO: Add extra validation here
//	delete mp_msg_font;
//	mp_msg_font = NULL;
	/* ************************************************************************** */
	
//	if(mp_msg_ok != NULL)
//	{
//		mp_msg_ok = NULL;
//		delete mp_msg_ok;
		
//	}
//	delete mp_msg_ok;  // OK ��ư�� ������ ���� ���� 

	StandardDialog::OnOK();
}

BOOL CDialog_Message::DestroyWindow() 
{
	// TODO: Add your specialized code here and/or call the base class
	delete mp_msg_font;
	mp_msg_font = NULL;
	/* ************************************************************************** */
	
	delete mp_msg_ok;  // OK ��ư�� ������ ���� ���� 
	return StandardDialog::DestroyWindow();
}
