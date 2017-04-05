// Dialog_Manual_Move.cpp : implementation file
//

#include "stdafx.h"
#include "handler.h"
#include "Dialog_Manual_Move.h"
#include "Public_Function.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDialog_Manual_Move dialog

CDialog_Manual_Move::CDialog_Manual_Move(CWnd* pParent /*=NULL*/)
	: CDialog(CDialog_Manual_Move::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDialog_Manual_Move)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDialog_Manual_Move::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialog_Manual_Move)
	DDX_Control(pDX, IDC_BTN_EXIT, m_btn_exit);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDialog_Manual_Move, CDialog)
	//{{AFX_MSG_MAP(CDialog_Manual_Move)
	ON_BN_CLICKED(IDC_BTN_EXIT, OnBtnExit)
	ON_BN_CLICKED(IDC_BTN_CARRIER_LOOP, OnBtnCarrierLoop)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BTN_MOTOR_STOP, OnBtnMotorStop)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialog_Manual_Move message handlers

BOOL CDialog_Manual_Move::Create()
{
	return CDialog::Create( CDialog_Manual_Move::IDD );
}


BOOL CDialog_Manual_Move::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	CRect rect_area;
	
	m_p_font = NULL;
	m_p_font = new CFont;
	m_p_font->CreateFont(14,0,0,0,900,0,0,0,0,0,0,ANTIALIASED_QUALITY,0,"MS Sans Serif");
	
	GetWindowRect(&rect_area);
	MoveWindow(m_cp_coord.x, m_cp_coord.y, rect_area.Width(), rect_area.Height());

	Init_Button();
	Init_Group();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDialog_Manual_Move::OnBtnExit() 
{
	// TODO: Add your control notification handler code here
	PostNcDestroy();
}

void CDialog_Manual_Move::Init_Button()
{
	short	shBtnColor = 30;
	
	m_btn_exit.SetFont(m_p_font);
	m_btn_exit.SetIcon(IDI_EXIT);
	m_btn_exit.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);
	m_btn_exit.SetButtonColor(1);
	m_btn_exit.SetTooltipText("IO Dialog Exit");
}

void CDialog_Manual_Move::Init_Group()
{
	CSxLogFont main_font(15,FW_SEMIBOLD,false,"Arial");
	
}

void CDialog_Manual_Move::OnButtonControl(bool bflag)
{
	m_btn_exit.EnableWindow(bflag);
}



void CDialog_Manual_Move::PostNcDestroy() 
{
	// TODO: Add your specialized code here and/or call the base class
	if(m_p_font != NULL)
	{
		delete m_p_font;
		m_p_font = NULL;
	}
	
	delete this;
	Func.m_p_move = NULL;

	CDialog::PostNcDestroy();
}

void CDialog_Manual_Move::OnBtnCarrierLoop() 
{
	//mn_move_step[25] = 0;
	SetTimer( TM_CARRIRER_LOOP, 100, NULL);
	SetTimer( TM_CARRIRER_LOOP, 100, NULL);
	
}

int CDialog_Manual_Move::Move_Carrier_Loop()
{
	int i = 0, nRet_1 = 0, nFuncRet = RET_PROCEED;
	int m_axis = M_CARRIER_X;
	
	switch( m_move_step[m_axis])
	{
		case 0:
			m_move_step[m_axis] = 100;
			break;

		case 100:
			if( Run_Device_Carrier_Robot.Check_Carrier_Move_Enable(0) != RET_GOOD )//초기상태
			{
				break;
			}
			st_sync.nCarrierBcr_Req = CTL_REQ;
			m_dwBcrWaitTime[m_axis][0] = GetCurrentTime();
			m_move_step[m_axis] = 110;
			break;

		case 110:
			m_dwBcrWaitTime[1] = GetCurrentTime();
			m_dwBcrWaitTime[2] = m_dwBcrWaitTime[1] - m_dwBcrWaitTime[0];
			if( m_dwBcrWaitTime[2] <= 0 ) m_dwBcrWaitTime[0] = GetCurrentTime();
			
			if( st_sync.nCarrierBcr_Req == CTL_READY )
			{
				m_move_step[m_axis] = 120;
			}
			else
			{
				if( m_dwBcrWaitTime[2] > 5000 )
				{//940000 1 A "BARCODE_IS_NOT_RESPONSE."
					CTL_Lib.Alarm_Error_Occurrence(1100, dWARNING, "940000");	
					m_move_step[m_axis] = 100;
				}
			}
			break;

		case 120:
			nRet_1 = atoi(st_msg.mstr_barcode);
			if( nRet_1 > 0 && nRet_1 < 13)
			{
				for ( i = 0; i < 3; i++ )
				{
					sprintf(st_carrier_buff_info[TOPSHIFT_BUFF_HEATSINK_VISION].c_chBarcode[i], "%d",nRet_1);
				}			
				m_move_step[m_axis] = 130;
			}
			else
			{//940001 1 A "BARCODE_IS_NOT_CORRECT_BARCODE_NUMBER."
				CTL_Lib.Alarm_Error_Occurrence(1100, dWARNING, "940001");
				m_move_step[m_axis] = 100;
			}
			break;

		case 130:
			if( Func.Check_RunAllSafety() != RET_GOOD )
			{
				break;
			}
			
			if( CheckCarrierType() == RET_GOOD )
			{
				m_move_step[m_axis] = 1000;
			}
			else
			{
				m_dwWaitTime[0] = GetCurrentTime();
				m_move_step[m_axis] = 140;
			}
			break;




	}

	return nFuncRet;
}

void CDialog_Manual_Move::OnTimer(UINT nIDEvent) 
{
	int nRet_1 = 0;
	// TODO: Add your message handler code here and/or call default
	if( nIDEvent == TM_CARRIRER_LOOP)
	{
		nRet_1 = Move_Carrier_Loop();
		if( nRet_1 == RET_GOOD || nRet_1 == RET_ERROR )
		{

		}

	}
	
	CDialog::OnTimer(nIDEvent);
}

void CDialog_Manual_Move::OnBtnMotorStop() 
{
	int i = 0;
	KillTimer( TM_CARRIRER_LOOP );	

	for (i = 0; i < M_MOTOR_COUNT; i++)
	{
		COMI.Set_MotStop( 0 , i);
	}
}
