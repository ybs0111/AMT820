// Dialog_Event_Msg.cpp : implementation file
//

#include "stdafx.h"
#include "handler.h"
#include "Dialog_Event_Msg.h"
#include "MainFrm.h"
#include "FastechPublic_IO.h"
#include "IO_Manager.h"
#include "SrcPart/APartHandler.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDialog_Event_Msg dialog


//CDialog_Event_Msg::CDialog_Event_Msg(CWnd* pParent /*=NULL*/)
//	: StandardDialog(CDialog_Event_Msg::IDD, pParent)
//{
//	//{{AFX_DATA_INIT(CDialog_Event_Msg)
//		// NOTE: the ClassWizard will add member initialization here
//	//}}AFX_DATA_INIT
//	m_caption_main = "Event Message Box";
//}


CDialog_Event_Msg::CDialog_Event_Msg(CString str1, CString str2, CString str3, CWnd* pParent /*=NULL*/)
	: StandardDialog(CDialog_Event_Msg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDialog_Event_Msg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_caption_main = "Event Message Box";

	if (str1 != "")
	{
		mstr_event_msg[0] = str1;
	}
	if (str2 != "")
	{
		mstr_event_msg[1] = str2;
	}
	if (str3 != "")
	{
		mstr_event_msg[2] = str3;
	}
}


void CDialog_Event_Msg::DoDataExchange(CDataExchange* pDX)
{
	StandardDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialog_Event_Msg)
	DDX_Control(pDX, IDOK, m_btn_ok);
	DDX_Control(pDX, IDC_LABEL_EVENT_MSG, m_label_event_msg);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDialog_Event_Msg, StandardDialog)
	//{{AFX_MSG_MAP(CDialog_Event_Msg)
	ON_WM_DESTROY()
	ON_WM_TIMER()
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialog_Event_Msg message handlers

BOOL CDialog_Event_Msg::Create() 
{  
	return StandardDialog::Create(IDD);
}

BOOL CDialog_Event_Msg::OnInitDialog() 
{
	StandardDialog::OnInitDialog();
	
	OnEventMsg_Label_Set();
	OnEventMsg_Text_Set();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDialog_Event_Msg::OnDestroy() 
{
	g_ioMgr.set_out_bit(st_io.o_Buzzer1, CTL_OFF);

	StandardDialog::OnDestroy();
}

void CDialog_Event_Msg::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	
	StandardDialog::OnTimer(nIDEvent);
}

BOOL CDialog_Event_Msg::PreTranslateMessage(MSG* pMsg) 
{
	if (pMsg->message == WM_KEYDOWN)
		if (pMsg->wParam == VK_ESCAPE)
			return TRUE;
		
	return StandardDialog::PreTranslateMessage(pMsg);
}

void CDialog_Event_Msg::OnEventMsg_Label_Set()
{
	m_label_event_msg.SetText("");
	m_label_event_msg.SetTextColor(RGB(250, 50, 50));
	m_label_event_msg.SetBkColor(RGB(181, 182, 140));
	m_label_event_msg.SetFontBold(FALSE);
	m_label_event_msg.SetFontName("MS Sans Serif");
	m_label_event_msg.SetFontSize(36);
}

void CDialog_Event_Msg::OnEventMsg_Text_Set()
{
	int i;
	CString strMsg;
	
	for (i = 0; i < 3; i++)
	{
//		if (st_msg.mstr_event_msg[i] != "")
//		{
//			strMsg += st_msg.mstr_event_msg[i];
//			strMsg += "\n";
//		}
		//2017.0115
		if (mstr_event_msg[i] != "")
		{
			strMsg += mstr_event_msg[i];
			strMsg += "\n";
		}
	}
	
	m_label_event_msg.SetText(strMsg);
}

void CDialog_Event_Msg::OnOK() 
{
	::PostMessage(st_handler.hWnd, WM_MAIN_EVENT, CTL_NO, 0);

}

void CDialog_Event_Msg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	CRect r;
	GetWindowRect(&r);
	
	int w = r.Width(), h = r.Height();
	
	r.left = 1280 / 2 - w / 2;
	r.right = r.left + w;
	
	r.top = 1024 / 2 - h / 2;
	r.bottom = r.top + h;
	
	MoveWindow(&r);
}
