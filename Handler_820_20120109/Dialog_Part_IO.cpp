// Dialog_Part_IO.cpp : implementation file
//

#include "stdafx.h"
#include "handler.h"
#include "Dialog_Part_IO.h"
#include "Public_Function.h"
#include "IO_Manager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDialog_Part_IO dialog
#define TM_IN_READ		100
#define TM_OUT_READ		200

CDialog_Part_IO::CDialog_Part_IO(CWnd* pParent /*=NULL*/)
	: CDialog(CDialog_Part_IO::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDialog_Part_IO)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDialog_Part_IO::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialog_Part_IO)
	DDX_Control(pDX, IDC_BTN_EXIT, m_btn_exit);
	DDX_Control(pDX, IDC_GROUP_OUTPUT, m_group_output);
	DDX_Control(pDX, IDC_GROUP_INPUT, m_group_input);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDialog_Part_IO, CDialog)
	//{{AFX_MSG_MAP(CDialog_Part_IO)
	ON_BN_CLICKED(IDC_BTN_EXIT, OnBtnExit)
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
	ON_MESSAGE(SSM_CLICK  , OnCellClick)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialog_Part_IO message handlers

BOOL CDialog_Part_IO::Create()
{
	return CDialog::Create( CDialog_Part_IO::IDD );
}

void CDialog_Part_IO::Init_Button()
{
	short	shBtnColor = 30;
	
	m_btn_exit.SetFont(m_p_font);
	m_btn_exit.SetIcon(IDI_EXIT);
	m_btn_exit.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);
	m_btn_exit.SetButtonColor(1);
	m_btn_exit.SetTooltipText("IO Dialog Exit");
}

void CDialog_Part_IO::Init_Group()
{
	CSxLogFont main_font(15,FW_SEMIBOLD,false,"Arial");
	
	m_group_input.SetFont(main_font);
	m_group_input.SetCatptionTextColor(BLUE_C);
	m_group_input.SetBorderColor(YELLOW_L);
	m_group_input.SetFontBold(TRUE);
	m_group_input.SetWindowText("Input");
	
	m_group_output.SetFont(main_font);
	m_group_output.SetCatptionTextColor(BLUE_C);
	m_group_output.SetBorderColor(YELLOW_L);
	m_group_output.SetFontBold(TRUE);
	m_group_output.SetWindowText("Output");
}

void CDialog_Part_IO::OnBtnExit() 
{
	// TODO: Add your control notification handler code here
	PostNcDestroy();
}

BOOL CDialog_Part_IO::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	CRect rectArea;

	m_p_font = NULL;
	m_p_font = new CFont;
	m_p_font->CreateFont(16,0,0,0,900,0,0,0,0,0,0,ANTIALIASED_QUALITY,0,"MS Sans Serif");


	GetClientRect(rectArea);
	MoveWindow(m_cp_coord.x, m_cp_coord.y, rectArea.Width(), rectArea.Height(), TRUE);
	
	Init_Grid_IO();
	Init_Button();
	Init_Group();

	SetTimer(TM_IN_READ, 100, NULL);
	SetTimer(TM_OUT_READ, 100, NULL);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDialog_Part_IO::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	switch(nIDEvent)
	{
		case TM_IN_READ:
			OnInputCheck();
			break;

		case TM_OUT_READ:
			OnOutputCheck();
			break;
	}
	CDialog::OnTimer(nIDEvent);
}

void CDialog_Part_IO::Init_Grid_IO()
{
	int   i;
	CString str_tmp;
	m_grid_input = (TSpread*)GetDlgItem(IDC_CUSTOM_INPUT);
	
	m_p_grid.GridFileOpen(m_grid_input, st_path.mstr_io_part_map, "test");
	
	m_n_in_cnt	= 0;
	m_n_out_cnt	= 0;

	m_n_in_cnt	= atoi(m_p_grid.GridCellText(m_grid_input, 3, ((m_n_part * 6) + 3)+1));
	m_n_out_cnt = atoi(m_p_grid.GridCellText(m_grid_input, 3, ((m_n_part * 6) + 6)+1));

	for(i=0; i<m_n_in_cnt; i++)
	{
		m_n_in_port[i]	= atoi(m_p_grid.GridCellText(m_grid_input, i+5, ((m_n_part * 6) + 0)+2));
		m_n_in_num[i]	= atoi(m_p_grid.GridCellText(m_grid_input, i+5, ((m_n_part * 6) + 1)+2));
		m_str_in[i]		= m_p_grid.GridCellText(m_grid_input, i+5, ((m_n_part * 6) + 2)+2);
	}

	for(i=0; i<m_n_out_cnt; i++)
	{
		m_n_out_port[i]	= atoi(m_p_grid.GridCellText(m_grid_input, i+5, ((m_n_part * 6) + 3)+2));
		m_n_out_num[i]	= atoi(m_p_grid.GridCellText(m_grid_input, i+5, ((m_n_part * 6) + 4)+2));
		m_str_out[i]	= m_p_grid.GridCellText(m_grid_input, i+5, ((m_n_part * 6) + 5)+2);
	}

	Init_Input();
	Init_Output();
}

void CDialog_Part_IO::Init_Input()
{
	int   i, j;
	CString str_tmp;
	
	m_grid_input = (TSpread*)GetDlgItem(IDC_CUSTOM_INPUT);
	
	m_p_grid.GridReset(m_grid_input);
	// 대문자 
	m_p_grid.GridCellSelectDisplay(m_grid_input, TRUE);
	
	m_p_grid.GridRowHeader(m_grid_input, FALSE);
	m_p_grid.GridColHeader(m_grid_input, TRUE);
	m_p_grid.GridHorizontal(m_grid_input, FALSE);
	m_p_grid.GridVertical(m_grid_input, TRUE);
	m_p_grid.GridAutoSize(m_grid_input, FALSE);
	m_p_grid.GridAutoSize(m_grid_input, FALSE);
	m_p_grid.GridCellRows(m_grid_input, m_n_in_cnt);
	m_p_grid.GridCellCols(m_grid_input, 2);
	
	for(i=0; i<m_n_in_cnt+1; i++)
	{
		m_p_grid.GridCellHeight_L(m_grid_input, i, 25);
		for(j=0; j<2; j++)
		{
			if(j==0)
			{
				m_p_grid.GridCellWidth_L(m_grid_input, j+1, 10);
			}
			else
			{
				m_p_grid.GridCellWidth_L(m_grid_input, j+1, 40);
			}
			
			m_p_grid.GridCellControlStatic(m_grid_input, i, j+1);
		}
	}

	m_p_grid.GridCellFont(m_grid_input, 0, 1, "MS Sans Serif", 14);
	m_p_grid.GridCellColor(m_grid_input, 0, 1, BLACK_L, YELLOW_C);
	m_p_grid.GridCellText(m_grid_input, 0, 1, "ON / OFF");

	m_p_grid.GridCellFont(m_grid_input, 0, 2, "MS Sans Serif", 14);
	m_p_grid.GridCellColor(m_grid_input, 0, 2, BLACK_L, YELLOW_C);
	m_p_grid.GridCellText(m_grid_input, 0, 2, "Description");

	for(i=0; i<m_n_in_cnt; i++)
	{
		m_p_grid.GridCellFont(m_grid_input, i+1, 1, "MS Sans Serif", 10);
		m_p_grid.GridCellColor(m_grid_input, i+1, 1, RGB(st_work.n_grid_r[0][1], st_work.n_grid_g[0][1], st_work.n_grid_b[0][1]), 
													 RGB(st_work.n_text_r[0][1], st_work.n_text_g[0][1], st_work.n_text_b[0][1]));
		m_p_grid.GridCellText(m_grid_input, i+1, 1, "OFF");

		m_p_grid.GridCellFont(m_grid_input, i+1, 2, "MS Sans Serif", 10);
		m_p_grid.GridCellColor(m_grid_input, i+1, 2, YELLOW_L, BLACK_L);
		m_p_grid.GridCellText(m_grid_input, i+1, 2, m_str_in[i]);
	}
}

void CDialog_Part_IO::Init_Output()
{
	int   i, j;
	CString str_tmp;
	
	m_grid_output = (TSpread*)GetDlgItem(IDC_CUSTOM_OUTPUT);
	
	m_p_grid.GridReset(m_grid_output);
	// 대문자 
	m_p_grid.GridCellSelectDisplay(m_grid_output, TRUE);
	
	m_p_grid.GridRowHeader(m_grid_output, FALSE);
	m_p_grid.GridColHeader(m_grid_output, TRUE);
	m_p_grid.GridHorizontal(m_grid_output, FALSE);
	m_p_grid.GridVertical(m_grid_output, TRUE);
	m_p_grid.GridAutoSize(m_grid_output, FALSE);
	m_p_grid.GridAutoSize(m_grid_output, FALSE);
	m_p_grid.GridCellRows(m_grid_output, m_n_out_cnt);
	m_p_grid.GridCellCols(m_grid_output, 2);
	
	for(i=0; i<m_n_out_cnt+1; i++)
	{
		m_p_grid.GridCellHeight_L(m_grid_output, i, 25);
		for(j=0; j<2; j++)
		{
			if(j==0)
			{
				m_p_grid.GridCellWidth_L(m_grid_output, j+1, 10);
			}
			else
			{
				m_p_grid.GridCellWidth_L(m_grid_output, j+1, 40);
			}
			
			m_p_grid.GridCellControlStatic(m_grid_output, i, j+1);
		}
	}
	
	m_p_grid.GridCellFont(m_grid_output, 0, 1, "MS Sans Serif", 14);
	m_p_grid.GridCellColor(m_grid_output, 0, 1, BLACK_L, YELLOW_C);
	m_p_grid.GridCellText(m_grid_output, 0, 1, "ON / OFF");
	
	m_p_grid.GridCellFont(m_grid_output, 0, 2, "MS Sans Serif", 14);
	m_p_grid.GridCellColor(m_grid_output, 0, 2, BLACK_L, YELLOW_C);
	m_p_grid.GridCellText(m_grid_output, 0, 2, "Description");
	
	for(i=0; i<m_n_out_cnt; i++)
	{
		m_p_grid.GridCellFont(m_grid_output, i+1, 1, "MS Sans Serif", 10);
		m_p_grid.GridCellColor(m_grid_output, i+1, 1, RGB(st_work.n_grid_r[1][1], st_work.n_grid_g[1][1], st_work.n_grid_b[1][1]), 
													  RGB(st_work.n_text_r[1][1], st_work.n_text_g[1][1], st_work.n_text_b[1][1]));
		m_p_grid.GridCellText(m_grid_output, i+1, 1, "OFF");
		
		m_p_grid.GridCellFont(m_grid_output, i+1, 2, "MS Sans Serif", 10);
		m_p_grid.GridCellColor(m_grid_output, i+1, 2, GREEN_L, BLACK_L);
		m_p_grid.GridCellText(m_grid_output, i+1, 2, m_str_out[i]);
	}
}


void CDialog_Part_IO::OnInputCheck()
{
	int bit;
	int i;
	m_grid_input = (TSpread*)GetDlgItem(IDC_CUSTOM_INPUT);

	for(i=0; i<m_n_in_cnt; i++)
	{
		bit = (m_n_in_port[i] * 100) + m_n_in_num[i];
		
		if(g_ioMgr.Get_In_Bit(bit) == IO_ON)
		{
			m_p_grid.GridCellText(m_grid_input, i+1, 1, "ON");
			m_p_grid.GridCellColor(m_grid_input, i+1, 1, RGB(st_work.n_grid_r[0][0], st_work.n_grid_g[0][0], st_work.n_grid_b[0][0]), 
														 RGB(st_work.n_text_r[0][0], st_work.n_text_g[0][0], st_work.n_text_b[0][0]));
		}
		else
		{
			m_p_grid.GridCellText(m_grid_input, i+1, 1, "OFF");
			m_p_grid.GridCellColor(m_grid_input, i+1, 1, RGB(st_work.n_grid_r[0][1], st_work.n_grid_g[0][1], st_work.n_grid_b[0][1]), 
													     RGB(st_work.n_text_r[0][1], st_work.n_text_g[0][1], st_work.n_text_b[0][1]));
		}
	}
}

void CDialog_Part_IO::OnOutputCheck()
{
	int bit;
	int i;

	m_grid_output = (TSpread*)GetDlgItem(IDC_CUSTOM_OUTPUT);
	
	for(i=0; i<m_n_out_cnt; i++)
	{
		bit = (m_n_out_port[i] * 100) + m_n_out_num[i];

		if(g_ioMgr.Get_Out_Bit(bit) == IO_ON)
		{
			m_p_grid.GridCellText(m_grid_output, i+1, 1, "ON");
			m_p_grid.GridCellColor(m_grid_output, i+1, 1, RGB(st_work.n_grid_r[1][0], st_work.n_grid_g[1][0], st_work.n_grid_b[1][0]), 
														  RGB(st_work.n_text_r[1][0], st_work.n_text_g[1][0], st_work.n_text_b[1][0]));
		}
		else
		{
			m_p_grid.GridCellText(m_grid_output, i+1, 1, "OFF");
			m_p_grid.GridCellColor(m_grid_output, i+1, 1, RGB(st_work.n_grid_r[1][1], st_work.n_grid_g[1][1], st_work.n_grid_b[1][1]), 
														  RGB(st_work.n_text_r[1][1], st_work.n_text_g[1][1], st_work.n_text_b[1][1]));
		}
	}
}

void CDialog_Part_IO::OnCellClick(WPARAM wParam, LPARAM lParam)
{
	LPSS_CELLCOORD lpcc = (LPSS_CELLCOORD)lParam;
	int output;
	int bit;
	CString str_tmp;
	
	if(lpcc->Col != 1) return;

	if(wParam==IDC_CUSTOM_OUTPUT)
	{
		m_grid_output = (TSpread*)GetDlgItem(IDC_CUSTOM_OUTPUT);

		bit = (m_n_out_port[lpcc->Row-1] * 100) + m_n_out_num[lpcc->Row-1];
		output = g_ioMgr.Get_Out_Bit(bit);
		
		g_ioMgr.Set_Out_Bit(bit, !output);

		if (!output == IO_ON)
		{
			str_tmp = "On";
		}
		else
		{
			str_tmp = "Off";
		}
		
		if (st_handler.cwnd_list != NULL)  // 리스트 바 화면 존재
		{
			st_other.str_normal_msg.Format("%s did  %s", m_str_out[lpcc->Row-1], str_tmp);
			sprintf(st_other.c_normal_msg, st_other.str_normal_msg);
			st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);
		}
	}
}

void CDialog_Part_IO::PostNcDestroy() 
{
	// TODO: Add your specialized code here and/or call the base class
	if(m_p_font != NULL)
	{
		delete m_p_font;
		m_p_font = NULL;
	}
	
	KillTimer(TM_IN_READ);
	KillTimer(TM_OUT_READ);
	
	delete this;
	Func.m_p_io = NULL;
	CDialog::PostNcDestroy();
}
