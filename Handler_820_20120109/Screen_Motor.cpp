// Screen_Motor.cpp : implementation file
//

#include "stdafx.h"
#include "handler.h"
#include "Screen_Motor.h"
#include "Dialog_Motor_Part.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CScreen_Motor
#define WM_DIALOG_EXIT WM_USER+100

IMPLEMENT_DYNCREATE(CScreen_Motor, CFormView)

CScreen_Motor::CScreen_Motor()
	: CFormView(CScreen_Motor::IDD)
{
	//{{AFX_DATA_INIT(CScreen_Motor)
	//}}AFX_DATA_INIT
	Func.m_p_motor = NULL;
}

CScreen_Motor::~CScreen_Motor()
{
}

void CScreen_Motor::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CScreen_Motor)
	DDX_Control(pDX, IDC_GROUP_PART_INFORMATION, m_group_part_information);
	DDX_Control(pDX, IDC_GROUP_MOTOR_PART, m_group_motor_part);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CScreen_Motor, CFormView)
	//{{AFX_MSG_MAP(CScreen_Motor)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
	ON_MESSAGE(SSM_CLICK  , OnCellClick)
	ON_MESSAGE(WM_WORK_END, OnMotor_Work_Message)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CScreen_Motor diagnostics

#ifdef _DEBUG
void CScreen_Motor::AssertValid() const
{
	CFormView::AssertValid();
}

void CScreen_Motor::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CScreen_Motor message handlers



BOOL CScreen_Motor::DestroyWindow() 
{
	// TODO: Add your specialized code here and/or call the base class
	if(Func.m_p_motor != NULL)
	{
		Func.m_p_motor->DestroyWindow();
	}

	if(mp_main_font != NULL)
	{
		delete mp_main_font;
		mp_main_font = NULL;
	}

	st_handler.cwnd_motor = NULL;

	return CFormView::DestroyWindow();
}

void CScreen_Motor::OnInitialUpdate() 
{
	CFormView::OnInitialUpdate();
	
	// TODO: Add your specialized code here and/or call the base class
	mp_main_font = NULL;
	mp_main_font = new CFont;
	mp_main_font->CreateFont(15,0,0,0,FW_BOLD,0,0,0,0,0,0,DEFAULT_QUALITY,0,"Arial");

	Init_Part_Grid();
	Init_Motor_Grid();
	Init_Group();

	CRect rectArea;
	
		
	GetDlgItem(IDC_MOTOR_IMG)->GetWindowRect(rectArea);

	st_handler.cwnd_motor = this;
}

void CScreen_Motor::Init_Part_Grid()
{
	int   i, j;
	CString str_tmp;
	m_grid_part = (TSpread*)GetDlgItem(IDC_CUSTOM_MOTOR_PART);
	
	m_p_grid.GridFileOpen(m_grid_part, st_path.mstr_motor_part_map, "test");
	
	st_motor_info.n_part_cnt = atoi(m_p_grid.GridCellText(m_grid_part, 4, 2));
	
	if(st_motor_info.n_part_cnt == 0) return;
	
	for(i=0; i<st_motor_info.n_part_cnt; i++)
	{
		st_motor_info.n_part_axis_cnt[i]	= atoi(m_p_grid.GridCellText(m_grid_part, (i*2)+7, 2));
		st_motor_info.str_part_name[i]		= m_p_grid.GridCellText(m_grid_part, (i*2)+7, 3);
		st_motor_info.str_part_info[i]		= m_p_grid.GridCellText(m_grid_part, (i*2)+7, 4);

		if(st_motor_info.n_part_axis_cnt[i] > 0)
		{
			for(j=0; j<st_motor_info.n_part_axis_cnt[i]; j++)
			{
				st_motor_info.n_part_axis[i][j]			= atoi(m_p_grid.GridCellText(m_grid_part, (i*2)+8, j+5));
				st_motor_info.str_part_axis_name[i][j]	= m_p_grid.GridCellText(m_grid_part, (i*2)+7, j+5);
			}
		}
	}

	m_p_grid.GridReset(m_grid_part);
	// ´ë¹®ÀÚ 
	m_p_grid.GridCellSelectDisplay(m_grid_part, TRUE);
	
	m_p_grid.GridRowHeader(m_grid_part, TRUE);
	m_p_grid.GridColHeader(m_grid_part, FALSE);
	m_p_grid.GridHorizontal(m_grid_part, TRUE);
	m_p_grid.GridVertical(m_grid_part, TRUE);
	m_p_grid.GridAutoSize(m_grid_part, FALSE);

	m_p_grid.GridCellRows(m_grid_part, 12);
	m_p_grid.GridCellCols(m_grid_part, st_motor_info.n_part_cnt);

	for(i=0; i<12; i++)
	{
		m_p_grid.GridCellHeight_L(m_grid_part, i+1, 30);

		for(j=0; j<st_motor_info.n_part_cnt+1; j++)
		{
			m_p_grid.GridCellWidth_L(m_grid_part, j, 15);
			m_p_grid.GridCellControlStatic(m_grid_part, i+1, j);
		}
	}

	m_p_grid.GridCellFont(m_grid_part, 1, 0, "MS Sans Serif", 12);
	m_p_grid.GridCellColor(m_grid_part, 1, 0, BLACK_L, YELLOW_C);
	m_p_grid.GridCellText(m_grid_part, 1, 0, "No.");
	
	m_p_grid.GridCellFont(m_grid_part, 2, 0, "MS Sans Serif", 12);
	m_p_grid.GridCellColor(m_grid_part, 2, 0, BLACK_L, YELLOW_C);
	m_p_grid.GridCellText(m_grid_part, 2, 0, "Part Ass'Y");
	
	m_p_grid.GridCellFont(m_grid_part, 3, 0, "MS Sans Serif", 12);
	m_p_grid.GridCellColor(m_grid_part, 3, 0, BLACK_L, YELLOW_C);
	m_p_grid.GridCellText(m_grid_part, 3, 0, "Axis");
	
	m_p_grid.GridCellFont(m_grid_part, 4, 0, "MS Sans Serif", 12);
	m_p_grid.GridCellColor(m_grid_part, 4, 0, BLACK_L, YELLOW_C);
	m_p_grid.GridCellText(m_grid_part, 4, 0, "Axis");
	
	m_p_grid.GridCellFont(m_grid_part, 5, 0, "MS Sans Serif", 12);
	m_p_grid.GridCellColor(m_grid_part, 5, 0, BLACK_L, YELLOW_C);
	m_p_grid.GridCellText(m_grid_part, 5, 0, "Axis");
	
	m_p_grid.GridCellFont(m_grid_part, 6, 0, "MS Sans Serif", 12);
	m_p_grid.GridCellColor(m_grid_part, 6, 0, BLACK_L, YELLOW_C);
	m_p_grid.GridCellText(m_grid_part, 6, 0, "Axis");
	
	m_p_grid.GridCellFont(m_grid_part, 7, 0, "MS Sans Serif", 12);
	m_p_grid.GridCellColor(m_grid_part, 7, 0, BLACK_L, YELLOW_C);
	m_p_grid.GridCellText(m_grid_part, 7, 0, "Axis");

	m_p_grid.GridCellFont(m_grid_part, 8, 0, "MS Sans Serif", 12);
	m_p_grid.GridCellColor(m_grid_part, 8, 0, BLACK_L, YELLOW_C);
	m_p_grid.GridCellText(m_grid_part, 8, 0, "Axis");
	
	m_p_grid.GridCellFont(m_grid_part, 9, 0, "MS Sans Serif", 12);
	m_p_grid.GridCellColor(m_grid_part, 9, 0, BLACK_L, YELLOW_C);
	m_p_grid.GridCellText(m_grid_part, 9, 0, "Axis");
	
	m_p_grid.GridCellFont(m_grid_part, 10, 0, "MS Sans Serif", 12);
	m_p_grid.GridCellColor(m_grid_part, 10, 0, BLACK_L, YELLOW_C);
	m_p_grid.GridCellText(m_grid_part, 10, 0, "Axis");
	
	m_p_grid.GridCellFont(m_grid_part, 11, 0, "MS Sans Serif", 12);
	m_p_grid.GridCellColor(m_grid_part, 11, 0, BLACK_L, YELLOW_C);
	m_p_grid.GridCellText(m_grid_part, 11, 0, "Axis");
	
	m_p_grid.GridCellFont(m_grid_part, 12, 0, "MS Sans Serif", 12);
	m_p_grid.GridCellColor(m_grid_part, 12, 0, BLACK_L, YELLOW_C);
	m_p_grid.GridCellText(m_grid_part, 12, 0, "Axis");

	for(i=0; i<st_motor_info.n_part_cnt; i++)
	{
		m_p_grid.GridCellColor(m_grid_part, 1, i+1, BLACK_L, YELLOW_C);
		str_tmp.Format("%d", i+1);
		m_p_grid.GridCellText(m_grid_part, 1, i+1, str_tmp);
		
		m_p_grid.GridCellColor(m_grid_part, 2, i+1, TEXT_BC, BLACK_C);
		m_p_grid.GridCellText(m_grid_part, 2, i+1, st_motor_info.str_part_name[i]);
		
		for(j=0; j<st_motor_info.n_part_axis_cnt[i]; j++)
		{
			m_p_grid.GridCellColor(m_grid_part, j+3, i+1, TEXT_BC_1, BLACK_C);
			m_p_grid.GridCellText(m_grid_part, j+3, i+1, st_motor_info.str_part_axis_name[i][j]);
		}
	}
}

void CScreen_Motor::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	CRect rect;
	// TODO: Add your message handler code here
	CDC *pDC;

	pDC = GetDlgItem(IDC_BMP_VIEW)->GetDC();
	GetDlgItem(IDC_BMP_VIEW)->GetWindowRect(rect);

	m_p_bmp_view.Show(pDC, CPoint(0, 0), CPoint(rect.Width(), rect.Height()), 0,0);

	ReleaseDC(pDC);
	// Do not call CFormView::OnPaint() for painting messages
}

void CScreen_Motor::OnCellClick(WPARAM wParam, LPARAM lParam)
{
	LPSS_CELLCOORD lpcc = (LPSS_CELLCOORD)lParam;
	CString str_tmp, str_file;

	if(lpcc->Col <1) return;

	m_grid_part = (TSpread*)GetDlgItem(IDC_CUSTOM_MOTOR_PART);

	switch(lpcc->Row)
	{
		case 0:
			break;

		case 1:
			str_file.Format("C:\\AMT820\\Bmp\\%s", st_motor_info.str_part_info[lpcc->Col-1]);
			m_p_bmp_view.Load(str_file);
			
			Invalidate(FALSE);
			break;

		case 2:
			OnMotor_Part_List(lpcc->Col);
			break;
	}
}

void CScreen_Motor::Init_Group()
{
	CSxLogFont main_font(16,FW_SEMIBOLD,false,"MS Sans Serif");
	
	m_group_motor_part.SetFont(main_font);
	m_group_motor_part.SetCatptionTextColor(BLUE_C);
	m_group_motor_part.SetBorderColor(YELLOW_L);
	m_group_motor_part.SetFontBold(TRUE);

	m_group_part_information.SetFont(main_font);
	m_group_part_information.SetCatptionTextColor(BLUE_C);
	m_group_part_information.SetBorderColor(YELLOW_L);
	m_group_part_information.SetFontBold(TRUE);
}


void CScreen_Motor::OnMotor_Part_List(int n_col)
{
	CRect rectArea;

	if(Func.m_p_motor == NULL)
	{
		Func.m_p_motor					= new CDialog_Motor_Part;
		Func.m_p_motor->m_n_part		= n_col-1;
		
		GetDlgItem(IDC_MOTOR_IMG)->GetWindowRect(rectArea);
		Func.m_p_motor->m_cp_coord		= CPoint(rectArea.left, rectArea.top);
		Func.m_p_motor->Create();
		Func.m_p_motor->ShowWindow(SW_SHOW);
	}
	else
	{
		Func.m_p_motor->SetFocus();
		Func.m_p_motor->ShowWindow(SW_SHOW);
	}
}

LRESULT CScreen_Motor::OnMotor_Work_Message(WPARAM wParam,LPARAM lParam)
{
	switch(wParam)
	{
		case WM_DIALOG_EXIT:
			break;
	}

	return 0;
}

void CScreen_Motor::Init_Motor_Grid()
{
	int   i, j, k;
	CString str_tmp;
	m_grid_motor = (TSpread*)GetDlgItem(IDC_CUSTOM_MOTOR);
	
	m_p_grid.GridFileOpen(m_grid_motor, st_path.mstr_motor_axis_map, "test");

	memset(st_motor_info.n_axis_pos_num,	-1, sizeof(st_motor_info.n_axis_pos_num));
	memset(st_motor_info.n_axis_minus_el,	-1, sizeof(st_motor_info.n_axis_minus_el));
	memset(st_motor_info.n_axis_plus_el,	-1, sizeof(st_motor_info.n_axis_plus_el));
	memset(st_motor_info.n_axis_home,		-1, sizeof(st_motor_info.n_axis_home));
	memset(st_motor_info.n_axis_direction,  -1, sizeof(st_motor_info.n_axis_direction));
	memset(st_motor_info.n_axis_account,	-1, sizeof(st_motor_info.n_axis_account));

	for(i=0; i<st_motor_info.n_part_cnt; i++)
	{
		for(j=0; j<st_motor_info.n_part_axis_cnt[i]; j++)
		{
			st_motor_info.n_axis_pos_num[i][j]		= atoi(m_p_grid.GridCellText(m_grid_motor, 16, (st_motor_info.n_part_axis[i][j] * 2) + 2));
			st_motor_info.n_axis_minus_el[i][j]		= atoi(m_p_grid.GridCellText(m_grid_motor, 8, (st_motor_info.n_part_axis[i][j] * 2) + 2));
			st_motor_info.n_axis_plus_el[i][j]		= atoi(m_p_grid.GridCellText(m_grid_motor, 9, (st_motor_info.n_part_axis[i][j] * 2) + 2));
			st_motor_info.n_axis_home[i][j]			= atoi(m_p_grid.GridCellText(m_grid_motor, 7, (st_motor_info.n_part_axis[i][j] * 2) + 2));
			st_motor_info.n_axis_direction[i][j]	= atoi(m_p_grid.GridCellText(m_grid_motor, 11, (st_motor_info.n_part_axis[i][j] * 2) + 2));
			st_motor_info.n_axis_account[i][j]		= atoi(m_p_grid.GridCellText(m_grid_motor, 12, (st_motor_info.n_part_axis[i][j] * 2) + 2));

			for(k=0; k<st_motor_info.n_axis_pos_num[i][j]; k++)
			{
				st_motor_info.str_axis_pos_name[i][j][k]	= m_p_grid.GridCellText(m_grid_motor, k+17, (st_motor_info.n_part_axis[i][j] * 2) + 2);
				st_motor_info.str_axis_pos_info[i][j][k]	= m_p_grid.GridCellText(m_grid_motor, k+17, ((st_motor_info.n_part_axis[i][j] * 2) + 2)+1);
			}
		}
	}
}
