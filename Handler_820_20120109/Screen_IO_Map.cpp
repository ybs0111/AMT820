// Screen_IO_Map.cpp : implementation file
//

#include "stdafx.h"
#include "Handler.h"
#include "Screen_IO_Map.h"
#include "Keyboarddll.h"
#include "Variable.h"
#include "Dialog_Message.h"
#include "Dialog_Color.h"
#include "math.h"
#include "FastechPublic_IO.h"
#include "Ctlbd_Variable.h"
#include "IO_Manager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CScreen_IO_Map

IMPLEMENT_DYNCREATE(CScreen_IO_Map, CFormView)

CScreen_IO_Map::CScreen_IO_Map()
	: CFormView(CScreen_IO_Map::IDD)
{
	//{{AFX_DATA_INIT(CScreen_IO_Map)
	//}}AFX_DATA_INIT

	m_nmaster = 0;
	m_nport = 0;
	m_nslave = 0;
}

CScreen_IO_Map::~CScreen_IO_Map()
{
	
}

void CScreen_IO_Map::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CScreen_IO_Map)
	DDX_Control(pDX, IDC_BTN_COLOR, m_btn_color);
	DDX_Control(pDX, IDC_BTN_IO_E_STOP, m_btn_e_stop);
	DDX_Control(pDX, IDC_MSG_RETRY_WAIT, m_msg_retry_wait);
	DDX_Control(pDX, IDC_MSG_RETRY_CNT, m_msg_retry_cnt);
	DDX_Control(pDX, IDC_CHK_CONTINUE, m_chk_continue);
	DDX_Control(pDX, IDC_GROUP_CONTINUE, m_group_continue);
	DDX_Control(pDX, IDC_BTN_SLAVE_ENABLE, m_btn_slave_enable);
	DDX_Control(pDX, IDC_DGT_RETRY_WAIT, m_dgt_retry_wait);
	DDX_Control(pDX, IDC_DGT_RETRY_CNT, m_dgt_retry_cnt);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CScreen_IO_Map, CFormView)
	//{{AFX_MSG_MAP(CScreen_IO_Map)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BTN_SLAVE_ENABLE, OnBtnSlaveEnable)
	ON_BN_CLICKED(IDC_CHK_CONTINUE, OnChkContinue)
	ON_BN_CLICKED(IDC_BTN_IO_E_STOP, OnBtnIoEStop)
	ON_BN_CLICKED(IDC_DGT_RETRY_CNT, OnDgtRetryCnt)
	ON_BN_CLICKED(IDC_DGT_RETRY_WAIT, OnDgtRetryWait)
	ON_BN_CLICKED(IDC_BTN_COLOR, OnBtnColor)
	//}}AFX_MSG_MAP
	ON_MESSAGE(SSM_CLICK  , OnCellClick)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CScreen_IO_Map diagnostics

#ifdef _DEBUG
void CScreen_IO_Map::AssertValid() const
{
	CFormView::AssertValid();
}

void CScreen_IO_Map::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CScreen_IO_Map message handlers

void CScreen_IO_Map::OnInitialUpdate() 
{
	CFormView::OnInitialUpdate();
	
	// TODO: Add your specialized code here and/or call the base class
	mp_io_font = NULL;
	mp_io_font = new CFont;
	mp_io_font->CreateFont(14,0,0,0,900,0,0,0,0,0,0,ANTIALIASED_QUALITY,0,"MS Sans Serif");
	// **************************************************************************
	
	mn_continue = 0;
	mn_actioncnt = 10;
	mn_conti_wait = 1000;
	
	mstr_io_name = "";
	mstr_io_act = "";

	Init_Map();
	Init_List();

	OnIO_Button_Set();
	OnIO_GroupBox_Set();
	OnIO_Continue_Change();
	OnIO_Name_Label_Set();
	OnIO_Digital_Count_Set();
	OnChangeSheet(m_nport, m_nslave); //kwlee 2017.0406
	OnOutputCheck(m_nport, m_nslave); //kwlee 2017.0406

	SetTimer(TMR_IO_READ, 50, NULL);
}

BOOL CScreen_IO_Map::DestroyWindow() 
{
	// TODO: Add your specialized code here and/or call the base class
	// **************************************************************************
	// 생성한 폰트 정보 삭제한다                                                 
	// **************************************************************************
	delete mp_io_font;
	mp_io_font = NULL;

	KillTimer(TMR_IO_READ);
	KillTimer(TMR_IO_CONTINUE);
	return CFormView::DestroyWindow();
}

void CScreen_IO_Map::Init_Map()
{
	TSpread *Grid = (TSpread*)GetDlgItem(IDC_CUSTOM_IO);

	m_p_grid.GridFileOpen(Grid, st_path.mstr_io_map, "Sheet1");
	OnSearch_Data();
	
	OnDrawGrid();
	OnChangeSheet(m_nport, m_nslave);
	OnOutputCheck(m_nport, m_nslave);
}

void CScreen_IO_Map::OnSearch_Data()
{
	TSpread *Grid = (TSpread*)GetDlgItem(IDC_CUSTOM_IO);
	int i, j, k;
	int row_m, col_m;
	int row_s, col_s;
	int pos;
	CString strfind;
	CString strTmp;
	char chvalue[100];

	m_nList_Count = 0;
	for(i=0; i<10; i++)
	{
		for(j=0; j<8; j++)
		{
			strfind.Format("Module No : %1d%1d", i, j);

			row_m = Grid->GetMaxRows();
			col_m = Grid->GetMaxCols();
			
			row_s = Grid->SearchCol(1, 0, row_m, strfind, SS_SEARCHFLAGS_NONE);
			col_s = Grid->SearchRow(row_s, 0, col_m, strfind, SS_SEARCHFLAGS_NONE);
			
			if(row_s > 0 && col_s > 0)
			{
// 				strTmp = m_p_grid.GridCellText(Grid, row_s, col_s);
// 				m_nIo_Module[i][j] = atoi(strTmp.Mid(strTmp.GetLength()-2, 2));
// 				
// 				strTmp = m_p_grid.GridCellText(Grid, row_s-1, col_s+4);
// 				pos = strTmp.Find(":");
// 				m_strIo_Module[i][j] = strTmp.Mid(pos+1);
// 				
// 				for(k=0; k<16; k++)
// 				{
// 					strTmp = m_p_grid.GridCellText(Grid, row_s+2+k, col_s);
// 					m_nIo_Num[i][j][k] = atoi(strTmp);
// 					
// 					strTmp = m_p_grid.GridCellText(Grid, row_s+2+k, col_s+1);
// 					m_strIo_Label[i][j][k] = strTmp;
// 
// 					strTmp = m_strIo_Label[i][j][k].Mid(0, 2);
// 
// 					if(strTmp == "PS")
// 					{
// 						m_nIo_Type[i][j][k] = IO_IN;
// 					}
// 					else
// 					{
// 						strTmp = m_strIo_Label[i][j][k].Mid(0, 1);
// 						if(strTmp == "S")
// 						{
// 							m_nIo_Type[i][j][k] = IO_OUT;
// 						}
// 						else
// 						{
// 							m_nIo_Type[i][j][k] = IO_NONE;
// 						}
// 					}
// 
// 					strTmp = m_p_grid.GridCellText(Grid, row_s+2+k, col_s+3);
// 					m_strIo_Description[i][j][k] = strTmp;
// 				}
				Grid->GetValue(col_s, row_s, chvalue);
				strTmp.Format("%s", &chvalue[12]);
				
				m_nIo_Module[i][j] = atoi(strTmp);
				
				Grid->GetValue(col_s+4, row_s-1, chvalue);
				strTmp.Format("%s", chvalue);
				pos = strTmp.Find(":");
				m_strIo_Module[i][j] = strTmp.Mid(pos+1);
				
				for (k = 0; k < 16; k++)
				{
					Grid->GetValue(col_s, row_s+2+k, chvalue);
					strTmp.Format("%s", chvalue);
					m_nIo_Num[i][j][k] = atoi(strTmp);
					
					Grid->GetValue(col_s+1, row_s+2+k, chvalue);
					strTmp.Format("%s", chvalue);
					m_strIo_Label[i][j][k] = strTmp;
					
					Grid->GetValue(col_s+1, row_s+2+k, chvalue);
					strTmp.Format("%c", chvalue[0]);
					
					if(strTmp == "S")
					{
						m_nIo_Type[i][j][k] = IO_OUT;
					}
					else if(strTmp == "P")
					{
						m_nIo_Type[i][j][k] = IO_IN;
					}
					else
					{
						m_nIo_Type[i][j][k] = IO_NONE;
					}
					
					Grid->GetValue(col_s+3, row_s+2+k, chvalue);
					strTmp.Format("%s", chvalue);
					m_strIo_Description[i][j][k] = strTmp;
				}
				m_nList_Count++;
			}	
		}
	}
}

void CScreen_IO_Map::OnDrawGrid()
{
	TSpread *Grid = (TSpread*)GetDlgItem(IDC_CUSTOM_IO);
	int i, j;
	int row;

	m_p_grid.GridReset(Grid);

	m_p_grid.GridCellSelectDisplay(Grid, TRUE);
	m_p_grid.GridRowHeader(Grid, FALSE);
	m_p_grid.GridColHeader(Grid, FALSE);
	m_p_grid.GridHorizontal(Grid, FALSE);
	m_p_grid.GridVertical(Grid, FALSE);
	m_p_grid.GridAutoSize(Grid, FALSE);
	m_p_grid.GridAutoSize(Grid, FALSE);
	m_p_grid.GridCellRows(Grid, 19);
	m_p_grid.GridCellCols(Grid, 6);

	for(i=1; i<20; i++)
	{
		for(j=1; j<7; j++)
		{
			m_p_grid.GridCellHeight_L(Grid, i, 28);

			if (j == 1)	
			{
				m_p_grid.GridCellWidth_L(Grid, j, 10);
			}
			else if (j == 2)	
			{
				m_p_grid.GridCellWidth_L(Grid, j, 20);
			}
			else if (j == 3)	
			{
				m_p_grid.GridCellWidth_L(Grid, j, 10);
			}
			else if (j == 4)	
			{
				m_p_grid.GridCellWidth_L(Grid, j, 5);
			}
			else if (j == 5)
			{
				m_p_grid.GridCellWidth_L(Grid, j, 20);
			}
			else if (j == 6)
			{
				m_p_grid.GridCellWidth_L(Grid, j, 50.5);
			}
			m_p_grid.GridCellControlStatic(Grid, i, j);
			
		}
	}

	row = 1;
	m_p_grid.GridCellMerge(Grid, row, 1, 1, 6);
	m_p_grid.GridCellFont(Grid, row, 1, "MS Sans Serif", 32);
	m_p_grid.GridCellColor(Grid, row, 1, BLUE_D, WHITE_C);
	m_p_grid.GridCellText(Grid, row, 1, "I / O Map Design Sheet");

	row = 2; 
	m_p_grid.GridCellMerge(Grid, row, 1, 1, 3);
	m_p_grid.GridCellFont(Grid, row, 1, "MS Sans Serif", 24);

	m_p_grid.GridCellMerge(Grid, row, 4, 1, 2);
	m_p_grid.GridCellFont(Grid, row, 4, "MS Sans Serif", 24);
	m_p_grid.GridCellColor(Grid, row, 4, SKY_C, BLACK_C);
	m_p_grid.GridCellText(Grid, row, 4, "ASS`Y");

	row = 3; 
	m_p_grid.GridCellFont(Grid, row, 1, "MS Sans Serif", 24);
	m_p_grid.GridCellColor(Grid, row, 1, SKY_C, BLACK_C);
	m_p_grid.GridCellText(Grid, row, 1, "No");

	m_p_grid.GridCellFont(Grid, row, 2, "MS Sans Serif", 24);
	m_p_grid.GridCellColor(Grid, row, 2, SKY_C, BLACK_C);
	m_p_grid.GridCellText(Grid, row, 2, "Label");

	m_p_grid.GridCellFont(Grid, row, 3, "MS Sans Serif", 24);
	m_p_grid.GridCellColor(Grid, row, 3, SKY_C, BLACK_C);
	m_p_grid.GridCellText(Grid, row, 3, "I/O");

	m_p_grid.GridCellMerge(Grid, row, 4, 1, 2);
	m_p_grid.GridCellFont(Grid, row, 4, "MS Sans Serif", 24);
	m_p_grid.GridCellColor(Grid, row, 4, SKY_C, BLACK_C);
	m_p_grid.GridCellText(Grid, row, 4, "Status");

	m_p_grid.GridCellFont(Grid, row, 6, "MS Sans Serif", 24);
	m_p_grid.GridCellColor(Grid, row, 6, SKY_C, BLACK_C);
	m_p_grid.GridCellText(Grid, row, 6, "Description");
}

void CScreen_IO_Map::OnChangeSheet(int port, int num)
{
	int row;
	int i;
	CString strTmp;
	TSpread *Grid = (TSpread*)GetDlgItem(IDC_CUSTOM_IO);

	row = 2; 

	if(m_nIo_Module[port][num]/40)
	{
		strTmp.Format("Master : [1] Port : [%d] Slave : [%d]", m_nIo_Module[port][num]/10-4, m_nIo_Module[port][num]%10);
	}
	else
	{
		strTmp.Format("Master : [0] Port : [%d] Slave : [%d]", m_nIo_Module[port][num]/10, m_nIo_Module[port][num]%10);
	}

	m_p_grid.GridCellText(Grid, row, 1, strTmp);

	m_p_grid.GridCellMerge(Grid, row, 5, 1, 2);
	m_p_grid.GridCellFont(Grid, row, 5, "MS Sans Serif", 24);
	m_p_grid.GridCellColor(Grid, row, 5, WHITE_C, BLACK_C);
	m_p_grid.GridCellText(Grid, row, 5, "I / O Map Design Sheet");

	for(i=0; i<16; i++)
	{
		m_p_grid.GridCellText(Grid, i+4, 4, "");

		if(m_strIo_Description[port][num][i] != "")
		{
			strTmp.Format("%d", i+1);

			m_p_grid.GridCellFont(Grid, i+4, 1, "MS Sans Serif", 18);
			if(m_nIo_Type[port][num][i] == IO_IN)
			{
				m_p_grid.GridCellColor(Grid, i+4, 1, YELLOW_L, BLACK_C);
				m_p_grid.GridCellColor(Grid, i+4, 2, YELLOW_L, BLACK_C);
				m_p_grid.GridCellColor(Grid, i+4, 3, YELLOW_L, BLACK_C);
				m_p_grid.GridCellControlStatic(Grid, i+4, 4);
				m_p_grid.GridCellColor(Grid, i+4, 4, BLACK_L, BLACK_C);

				m_p_grid.GridCellColor(Grid, i+4, 6, YELLOW_L, BLACK_C);
				m_p_grid.GridCellText(Grid, i+4, 3, "Input");
			}
			else if(m_nIo_Type[port][num][i] == IO_OUT)
			{
				m_p_grid.GridCellColor(Grid, i+4, 1, GREEN_L, BLACK_C);
				m_p_grid.GridCellColor(Grid, i+4, 2, GREEN_L, BLACK_C);
				m_p_grid.GridCellColor(Grid, i+4, 3, GREEN_L, BLACK_C);

				m_p_grid.GridCellFont(Grid, i+4, 4, "MS Sans Serif", 12);
				m_p_grid.GridCellText(Grid, i+4, 4, "N");
				m_p_grid.GridCellColor(Grid, i+4, 4, RGB(255, 200, 200), RGB(25, 25, 85));

				m_p_grid.GridCellColor(Grid, i+4, 6, GREEN_L, BLACK_C);
				m_p_grid.GridCellText(Grid, i+4, 3, "Output");
			}

			m_p_grid.GridCellText(Grid, i+4, 1, strTmp);
			
			m_p_grid.GridCellFont(Grid, i+4, 2, "MS Sans Serif", 18);
			m_p_grid.GridCellText(Grid, i+4, 2, m_strIo_Label[port][num][i]);
			
			m_p_grid.GridCellFont(Grid, i+4, 3, "MS Sans Serif", 18);

			m_p_grid.GridCellFont(Grid, i+4, 6, "MS Sans Serif", 18);
			m_p_grid.GridCellText(Grid, i+4, 6, m_strIo_Description[port][num][i]);
		}
		else
		{
			m_p_grid.GridCellFont(Grid, i+4, 1, "MS Sans Serif", 18);
			m_p_grid.GridCellColor(Grid, i+4, 1, BLACK_L, WHITE_C);
			m_p_grid.GridCellText(Grid, i+4, 1, "-");

			m_p_grid.GridCellFont(Grid, i+4, 2, "MS Sans Serif", 18);
			m_p_grid.GridCellColor(Grid, i+4, 2, BLACK_L, WHITE_C);
			m_p_grid.GridCellText(Grid, i+4, 2, "-");

			m_p_grid.GridCellFont(Grid, i+4, 3, "MS Sans Serif", 18);
			m_p_grid.GridCellColor(Grid, i+4, 3, BLACK_L, WHITE_C);
			m_p_grid.GridCellText(Grid, i+4, 3, "-");

			m_p_grid.GridCellControlStatic(Grid, i+4, 4);
			m_p_grid.GridCellFont(Grid, i+4, 4, "MS Sans Serif", 18);
			m_p_grid.GridCellColor(Grid, i+4, 4, BLACK_L, WHITE_C);
			m_p_grid.GridCellText(Grid, i+4, 4, "");

			m_p_grid.GridCellFont(Grid, i+4, 5, "MS Sans Serif", 18);
			m_p_grid.GridCellColor(Grid, i+4, 5, BLACK_L, WHITE_C);
			m_p_grid.GridCellText(Grid, i+4, 5, "-");

			m_p_grid.GridCellFont(Grid, i+4, 6, "MS Sans Serif", 18);
			m_p_grid.GridCellColor(Grid, i+4, 6, BLACK_L, WHITE_C);
			m_p_grid.GridCellText(Grid, i+4, 6, "-");
		}
	}
}


void CScreen_IO_Map::Init_List()
{
	TSpread *Grid = (TSpread*)GetDlgItem(IDC_CUSTOM_LIST);
	int i, j;
	int count;
	CString strTmp;

	m_p_grid.GridReset(Grid);
	m_p_grid.GridRowHeader(Grid, FALSE);
	m_p_grid.GridColHeader(Grid, TRUE);
	m_p_grid.GridHorizontal(Grid, FALSE);
	m_p_grid.GridVertical(Grid, TRUE);
	m_p_grid.GridAutoSize(Grid, FALSE);
	m_p_grid.GridAutoSize(Grid, FALSE);
	m_p_grid.GridCellRows(Grid, m_nList_Count);
	m_p_grid.GridCellCols(Grid, 2);

	m_p_grid.GridCellFont(Grid, 0, 1, "MS Sans Serif", 14);
	m_p_grid.GridCellColor(Grid, 0, 1, BLUE_D, WHITE_C);
	m_p_grid.GridCellText(Grid, 0, 1, "Module No.");

	m_p_grid.GridCellFont(Grid, 0, 2, "MS Sans Serif", 14);
	m_p_grid.GridCellColor(Grid, 0, 2, BLUE_D, WHITE_C);
	m_p_grid.GridCellText(Grid, 0, 2, "ASS`Y");

	for(i=0; i<m_nList_Count+1; i++)
	{
		for(j=0; j<3; j++)
		{
			m_p_grid.GridCellHeight_L(Grid, i, 20);

			if(j==1) m_p_grid.GridCellWidth_L(Grid, j, 10);
			else if(j==2) m_p_grid.GridCellWidth_L(Grid, j, 25.5);

			m_p_grid.GridCellControlStatic(Grid, i, j);
		}
	}

	count = 0;

	for(i=0; i<10; i++)
	{
		for(j=0; j<8; j++)
		{
			if(m_strIo_Module[i][j] != "")
			{
				strTmp.Format("%d", m_nIo_Module[i][j]);

				
				if(i == 0 && j == 0)
				{
					m_p_grid.GridCellColor(Grid, count+1, 1, SKY_C, BLACK_C);
				}
				else
				{
					m_p_grid.GridCellColor(Grid, count+1, 1, WHITE_C, BLACK_C);
				}

				m_p_grid.GridCellText(Grid, count+1, 1, strTmp);
				
				
				if(i == 0 && j == 0)
				{
					m_p_grid.GridCellColor(Grid, count+1, 2, SKY_C, BLACK_C);
				}
				else
				{
					m_p_grid.GridCellColor(Grid, count+1, 2, WHITE_C, BLACK_C);
				}

				m_p_grid.GridCellText(Grid, count+1, 2, m_strIo_Module[i][j]);
				
				count++;
			}	
		}
	}
}

void CScreen_IO_Map::OnCellClick(WPARAM wParam, LPARAM lParam)
{
	TSpread *Grid_List, *Grid_Map;
	LPSS_CELLCOORD lpcc = (LPSS_CELLCOORD)lParam;
	int port, slave, io;
	int count;
	int i;
	CString strTmp;
	
	if(wParam==IDC_CUSTOM_IO)
	{
		if(st_handler.n_menu_lock == TRUE) return;
		if(lpcc->Row<4) return;
	
		Grid_Map = (TSpread*)GetDlgItem(IDC_CUSTOM_IO);
	
		if(lpcc->Col == 4)
		{
			strTmp = m_p_grid.GridCellText(Grid_Map, lpcc->Row, 1);
			
			if(strTmp == "-") return;
			
			io = atoi(strTmp) - 1;

			if(m_nIo_Type[m_nport][m_nslave][io] == IO_OUT)
			{
				strTmp = m_p_grid.GridCellText(Grid_Map, lpcc->Row, 4);

				if (strTmp == "N")
				{
					m_p_grid.GridCellColor(Grid_Map, lpcc->Row, 4, YELLOW_C, BLACK_C);  // 규이리 추가 [2011.11.25]
					m_p_grid.GridCellText(Grid_Map,  lpcc->Row, 4, "Y");  // o_ld_tr_left_dn_cyl
				}
				else
				{
					m_p_grid.GridCellColor(Grid_Map, lpcc->Row, 4, RGB(255,200,200), RGB(25,25,85));  // 규이리 추가 [2011.11.25]
					m_p_grid.GridCellText(Grid_Map,  lpcc->Row, 4, "N");
				}
			}
		}
		else if(lpcc->Col == 5)
		{
			strTmp = m_p_grid.GridCellText(Grid_Map, lpcc->Row, 1);
			
			if(strTmp == "-") return;
			
			io = atoi(strTmp) - 1;
			
			strTmp = m_p_grid.GridCellText(Grid_Map, lpcc->Row, 6);
			mstr_io_name = strTmp;
			
			strTmp.Format("[IO] Port(%d) Slave(%d) IO(%d)", m_nport, m_nslave, io);
			Func.On_LogFile_Add(99, strTmp);
			
			if (mn_continue == YES)
			{
				mn_stop_req = FALSE;  // ESTOP 요청 플래그 초기화
				mn_action_step = 0;
				mn_action_cnt = 1;
				
				count = 0;
				for(i=0; i<16; i++)
				{
					if(m_nIo_Type[m_nport][m_nslave][i] == IO_IN)
					{
						m_n_output_info[i]	= NO;
					}
					else if(m_nIo_Type[m_nport][m_nslave][i] == IO_OUT)
					{
						strTmp = m_p_grid.GridCellText(Grid_Map, i+4, 4);
						
						if(strTmp == "Y")
						{
							count++;
							m_n_output_info[i] = YES;
						}
						else
						{
							m_n_output_info[i] = NO;				
						}
					}
				}
				
				if(count == 0)
				{
					if(m_nIo_Type[m_nport][m_nslave][io] == IO_OUT)
					{
						m_n_output_info[io] = YES;
						m_p_grid.GridCellText(Grid_Map, lpcc->Row, 4, "1");
					}
					else
					{
						return;
					}
				}

				OnIO_Controls_Enable(false);			// 모든 컨트롤 화면 출력 제어 함수
				
				if(st_handler.n_menu_lock == FALSE)
				{
					st_handler.n_menu_lock = TRUE;
				}
				
				SetTimer(TMR_IO_CONTINUE, 100, NULL);	// I/O 상태 출력 타이머 생성
			}
			else
			{
				OnOutputData(m_nport, m_nslave, io);
			}
		}	
	}
	else if(wParam==IDC_CUSTOM_LIST)
	{
		Grid_List = (TSpread*)GetDlgItem(IDC_CUSTOM_LIST);

		strTmp = m_p_grid.GridCellText(Grid_List, lpcc->Row, 1);

		port = atoi(strTmp) / 10;
		slave = atoi(strTmp) % 10;

		if(port == m_nport && slave == m_nslave) return;
	
		m_nport = port;
		m_nslave = slave;

		for(i=0; i<m_nList_Count; i++)
		{
			m_p_grid.GridCellColor(Grid_List, i+1, 1, WHITE_C, BLACK_C);
			m_p_grid.GridCellColor(Grid_List, i+1, 2, WHITE_C, BLACK_C);
		}
		
		m_p_grid.GridCellColor(Grid_List, lpcc->Row, 1, SKY_C, BLACK_C);
		m_p_grid.GridCellColor(Grid_List, lpcc->Row, 2, SKY_C, BLACK_C);
		
		OnChangeSheet(m_nport, m_nslave);	
	}
}

void CScreen_IO_Map::OnOutputData(int port, int slave, int io)
{
	int bit;
	CString str_tmp;
	CDialog_Message msg_dlg;

	if(st_handler.n_menu_lock == TRUE) return;

	if(m_nIo_Type[port][slave][io] == IO_OUT)
	{
		bit = (m_nIo_Module[port][slave]*100)+io;

		if(FAS_IO.n_out[bit] == IO_ON)
		{
			FAS_IO.n_out[bit] = IO_OFF;
			mstr_io_act = "Off";
		}
		else
		{
			FAS_IO.n_out[bit] = IO_ON;
			mstr_io_act = "On";
		}

		if (st_handler.cwnd_list != NULL)  // 리스트 바 화면 존재
		{
			st_other.str_normal_msg.Format("%s did  %s", mstr_io_name, mstr_io_act);
			sprintf(st_msg.c_normal_msg, st_other.str_normal_msg);
			st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);
		}
	}
}

void CScreen_IO_Map::OnOutputCheck(int port, int slave)
{
	int i;
	int bit;
	TSpread *Grid = (TSpread*)GetDlgItem(IDC_CUSTOM_IO);

	for(i=0; i<16; i++)
	{
		if(m_nIo_Type[port][slave][i] == IO_OUT && m_strIo_Description[port][slave][i] != "")
		{
			bit = (m_nIo_Module[port][slave]*100)+i;

			if(FAS_IO.n_out[bit] == IO_ON)
			{
				//m_p_grid.GridCellColor(Grid, i+4, 5, RGB(st_work.n_grid_r[1][0], st_work.n_grid_g[1][0], st_work.n_grid_b[1][0]), RGB(st_work.n_text_r[1][0], st_work.n_text_g[1][0], st_work.n_text_b[1][0]));
				//kwlee 2017.0406
				m_p_grid.GridCellColor(Grid, i+4, 5, RGB(255,128,64), WHITE_C);
				m_p_grid.GridCellFont(Grid, i+4, 5, "MS Sans Serif", 14);
				m_p_grid.GridCellText(Grid, i+4, 5, "ON");
			}
			else
			{
				//m_p_grid.GridCellColor(Grid, i+4, 5, RGB(st_work.n_grid_r[1][1], st_work.n_grid_g[1][1], st_work.n_grid_b[1][1]), RGB(st_work.n_text_r[1][1], st_work.n_text_g[1][1], st_work.n_text_b[1][1]));
				//kwlee 2017.0406
				m_p_grid.GridCellColor(Grid, i+4, 5, RGB(128,64,0), BLACK_C);
				m_p_grid.GridCellFont(Grid, i+4, 5, "MS Sans Serif", 14);
				m_p_grid.GridCellText(Grid, i+4, 5, "OFF");
			}
		}
	}
}


void CScreen_IO_Map::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	int n_check;

	switch(nIDEvent)
	{
		case TMR_IO_READ:
			OnOutputCheck(m_nport, m_nslave);
			OnInputCheck(m_nport, m_nslave);
			break;
		case TMR_IO_CONTINUE:
			n_check = OnIO_Cylinder_Excution();
			
			if(n_check == RET_GOOD)
			{
				if(st_handler.n_menu_lock != FALSE)
				{
					st_handler.n_menu_lock = FALSE;
				}
				
				mn_continue = 0;
				m_chk_continue.SetCheck(mn_continue);

				KillTimer(TMR_IO_CONTINUE);		// 타이머 삭제
				OnIO_Controls_Enable(true);		// 모든 컨트롤 화면 출력 제어 함수
			}
			else if(n_check == RET_ERROR)
			{
				if(st_handler.n_menu_lock != FALSE)
				{
					st_handler.n_menu_lock = FALSE;
				}
				
				mn_continue = 0;
				m_chk_continue.SetCheck(mn_continue);

				KillTimer(TMR_IO_CONTINUE);		// 타이머 삭제
				OnIO_Controls_Enable(true);		// 모든 컨트롤 화면 출력 제어 함수
			}
			break;
	}

	CFormView::OnTimer(nIDEvent);
}

void CScreen_IO_Map::OnInputCheck(int port, int slave)
{
	int i;
	int bit;
	TSpread *Grid = (TSpread*)GetDlgItem(IDC_CUSTOM_IO);

	for(i=0; i<16; i++)
	{
		if(m_nIo_Type[port][slave][i] == IO_IN && m_strIo_Description[port][slave][i] != "")
		{
			bit = (m_nIo_Module[port][slave]*100)+i;
			if(FAS_IO.n_in[bit] == IO_ON)
			{
				//m_p_grid.GridCellColor(Grid, i+4, 5, RGB(st_work.n_grid_r[0][0], st_work.n_grid_g[0][0], st_work.n_grid_b[0][0]), RGB(st_work.n_text_r[0][0], st_work.n_text_g[0][0], st_work.n_text_b[0][0]));
				//kwlee 2017.0406
				m_p_grid.GridCellColor(Grid, i+4, 5, RGB(0,128,0), WHITE_C);
				m_p_grid.GridCellFont(Grid, i+4, 5, "MS Sans Serif", 14);
				m_p_grid.GridCellText(Grid, i+4, 5, "ON");
			}
			else
			{
				//m_p_grid.GridCellColor(Grid, i+4, 5, RGB(st_work.n_grid_r[0][1], st_work.n_grid_g[0][1], st_work.n_grid_b[0][1]), RGB(st_work.n_text_r[0][1], st_work.n_text_g[0][1], st_work.n_text_b[0][1]));
				//kwlee 2017.0406
				m_p_grid.GridCellColor(Grid, i+4, 5, RGB(128,255,128), BLACK_L);
				m_p_grid.GridCellFont(Grid, i+4, 5, "MS Sans Serif", 14);
				m_p_grid.GridCellText(Grid, i+4, 5, "OFF");
			}
		}
	}
}

void CScreen_IO_Map::OnBtnSlaveEnable() 
{
	// TODO: Add your control notification handler code here
	if(st_handler.n_menu_lock != FALSE)	return;

	g_ioMgr.Set_IO_SlaveEnable(0, m_nport, m_nslave, !n_check_slave);
	OnIO_Slave_Change();
}

void CScreen_IO_Map::OnIO_Slave_Change()
{
	n_check_slave = FAS_IO.Check_IO_Slave(0, m_nport, m_nslave);
	if (n_check_slave == TRUE)		// 살아 있다면... 죽일수 있는 버튼.
	{
		SetDlgItemText(IDC_BTN_SLAVE_ENABLE, "Disable");
	}
	else							// 죽어 있다면... 살릴수 있는 버튼.
	{
		SetDlgItemText(IDC_BTN_SLAVE_ENABLE, "Enable");
	}
}

void CScreen_IO_Map::OnIO_GroupBox_Set()
{
	CSxLogFont io_font(15,FW_SEMIBOLD,false,"MS Sans Serif");
	
	m_group_continue.SetFont(io_font);
	m_group_continue.SetBorderColor(YELLOW_L);
	m_group_continue.SetCatptionTextColor(BLUE_C);
	m_group_continue.SetFontBold(TRUE);
}

void CScreen_IO_Map::OnIO_Continue_Change()
{
	if (mn_continue < 0 || mn_continue > 1)		mn_continue = 0;
	
	m_chk_continue.SetCheck(mn_continue);
	
	if (mn_continue == 1)		SetDlgItemText(IDC_CHK_CONTINUE, "Continue");
	else						SetDlgItemText(IDC_CHK_CONTINUE, "Normal");
}

void CScreen_IO_Map::OnChkContinue() 
{
	// TODO: Add your control notification handler code here
	if(st_handler.n_menu_lock != FALSE)	return;

	if (m_chk_continue.GetCheck() == 1)		mn_continue = 1;
	else									mn_continue = 0;
	
	OnIO_Continue_Change();	
}

void CScreen_IO_Map::OnIO_Controls_Enable(bool b_state)
{
	TSpread *Grid_Map;
	int i;

	m_chk_continue.EnableWindow(b_state);

	if(b_state)
	{
		Grid_Map = (TSpread*)GetDlgItem(IDC_CUSTOM_IO);

		for(i=0; i<16; i++)
		{
			if(m_n_output_info[i] == YES)
			{
				m_n_output_info[i]	= NO;
				m_p_grid.GridCellText(Grid_Map,  i+4, 4, "N");
				m_p_grid.GridCellColor(Grid_Map, i+4, 4, RGB(255,200,200), RGB(25,25,85));
			}
		}
	}
}

void CScreen_IO_Map::OnIO_Button_Set()
{
	short	shBtnColor = 30;
	
	m_chk_continue.SetFont(mp_io_font);
	m_chk_continue.SetIcon(IDI_SAFETY, IDI_NONSAFETY);
	m_chk_continue.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);
	m_chk_continue.SetButtonColor(1);
}

void CScreen_IO_Map::OnIO_Name_Label_Set()
{
	m_msg_retry_cnt.SetTextFont(mp_io_font);
	m_msg_retry_cnt.SetColor(RGB(0, 0, 0), RGB(255, 255, 255), 3, 2);
	m_msg_retry_cnt.SetTextAlign(CFloatST::STA_CENTER, 10);
	
	m_msg_retry_wait.SetTextFont(mp_io_font);
	m_msg_retry_wait.SetColor(RGB(0, 0, 0), RGB(255, 255, 255), 3, 2);
	m_msg_retry_wait.SetTextAlign(CFloatST::STA_CENTER, 10);
}

void CScreen_IO_Map::OnBtnIoEStop() 
{
	// TODO: Add your control notification handler code here
	mn_stop_req = TRUE;  // ESTOP 요청 플래그 설정	
}

void CScreen_IO_Map::OnIO_Digital_Count_Set()
{
	m_dgt_retry_cnt.SetStyle(CDigit::DS_INT, 4, CDigit::DC_BGREEN);
	m_dgt_retry_wait.SetStyle(CDigit::DS_INT, 4, CDigit::DC_BGREEN);
	
	m_dgt_retry_cnt.SetVal(mn_actioncnt);
	m_dgt_retry_wait.SetVal(mn_conti_wait);
}

int CScreen_IO_Map::OnIO_Cylinder_Excution()
{
	int mn_action_flag = RET_PROCEED;
	int i;
	int bit;
	CString str_temp;
	char chr_buf[20];

	// **************************************************************************
	// 모터 동작 중 ESTOP 요청 발생했는지 검사한다                               
	// -> ESTOP 요청 발생한 경우 긴급 정지로 모터 멈춘다                         
	// **************************************************************************
	if(mn_stop_req == TRUE)
	{
		if (mn_action_step < 1000)
			mn_action_step = 1000;
	}
	// **************************************************************************


	switch(mn_action_step)
	{
		case 0:
			for(i=0; i<16; i++)
			{
				if(m_n_output_info[i] == YES)
				{
					bit = (m_nIo_Module[m_nport][m_nslave]*100)+i;

					if(FAS_IO.n_out[bit] == IO_ON)
					{
						FAS_IO.n_out[bit] = IO_OFF;
					}
					else
					{
						FAS_IO.n_out[bit] = IO_ON;
					}
				}
			}

			if (st_handler.cwnd_list != NULL)  // 리스트 바 화면 존재
			{
				st_other.str_normal_msg.Format("[%d][%d][%d][%d][%d][%d][%d][%d][%d][%d][%d][%d][%d][%d][%d][%d] Word Output", 
					                           m_n_output_info[0], m_n_output_info[1], m_n_output_info[2], m_n_output_info[3],
											   m_n_output_info[4], m_n_output_info[5], m_n_output_info[6], m_n_output_info[7],
											   m_n_output_info[8], m_n_output_info[9], m_n_output_info[10], m_n_output_info[11],
											   m_n_output_info[12], m_n_output_info[13], m_n_output_info[14], m_n_output_info[15]);

				sprintf(st_msg.c_normal_msg, st_other.str_normal_msg);
				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);
			}

			mn_conti_io_flag = !mn_conti_io_flag;
			n_conti_wait_time[0] = GetCurrentTime();
			mn_action_step = 100;
			break;

		case 100:
			// *************************************************************
			//  잠시 대기후 Retry 진행                                      
			// *************************************************************
			n_conti_wait_time[1] = GetCurrentTime();

			n_conti_wait_time[2] = n_conti_wait_time[1] - n_conti_wait_time[0];

			if (n_conti_wait_time[2] > mn_conti_wait)
			{
				mn_action_step = 200;
			}
			break;

		case 200:
			for(i=0; i<16; i++)
			{
				if(m_n_output_info[i] == YES)
				{
					bit = (m_nIo_Module[m_nport][m_nslave]*100)+i;
					
					if(FAS_IO.n_out[bit] == IO_ON)
					{
						FAS_IO.n_out[bit] = IO_OFF;
					}
					else
					{
						FAS_IO.n_out[bit] = IO_ON;
					}
				}
			}

			if (st_handler.cwnd_list != NULL)  // 리스트 바 화면 존재
			{
				st_other.str_normal_msg.Format("[%d][%d][%d][%d][%d][%d][%d][%d][%d][%d][%d][%d][%d][%d][%d][%d] Word Output", 
												m_n_output_info[0], m_n_output_info[1], m_n_output_info[2], m_n_output_info[3],
												m_n_output_info[4], m_n_output_info[5], m_n_output_info[6], m_n_output_info[7],
												m_n_output_info[8], m_n_output_info[9], m_n_output_info[10], m_n_output_info[11],
												m_n_output_info[12], m_n_output_info[13], m_n_output_info[14], m_n_output_info[15]);

				sprintf(st_msg.c_normal_msg, st_other.str_normal_msg);
				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);
			}
			n_conti_wait_time[0] = GetCurrentTime();
			mn_action_step = 300;
			break;

		case 300:
			// *************************************************************
			//  잠시 대기후 Retry 진행                                      
			// *************************************************************
			n_conti_wait_time[1] = GetCurrentTime();

			n_conti_wait_time[2] = n_conti_wait_time[1] - n_conti_wait_time[0];

			if (n_conti_wait_time[2] > mn_conti_wait)
			{
				mn_action_step = 400;
			}
			break;

		case 400:
			str_temp = LPCTSTR(_itoa(mn_action_cnt, chr_buf, 10));

			if (st_handler.cwnd_list != NULL)  // 리스트 바 화면 존재
			{
				st_other.str_normal_msg = _T(str_temp + "th repeat action.");
				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG); 
			}
			
			mn_action_cnt++;		

			if(mn_action_cnt > mn_actioncnt)
			{
				mn_action_step = 500;
			}
			else	mn_action_step = 0;

			break;

		case 500:
// 			if (st_handler.cwnd_list != NULL)  // 리스트 바 화면 존재
// 			{
// 				st_msgr.str_normal_msg = _T("[Repeat action] The action was completed.!");
// 				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG); 
// 			}
			mn_action_flag = RET_GOOD;
			break;

		case 1000:
			if (st_handler.cwnd_list != NULL)  // 리스트 바 화면 존재
			{
				st_other.str_abnormal_msg = _T("A Repeat action became the cancel.");
				sprintf(st_msg.c_abnormal_msg, st_other.str_abnormal_msg);
				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG); 
			}
			mn_action_flag = RET_GOOD;

			if(mn_stop_req != FALSE)
			{
				mn_stop_req = FALSE;  // ESTOP 요청 플래그 초기화
			}
			break;
	}
	return mn_action_flag;
}





void CScreen_IO_Map::OnDgtRetryCnt() 
{
	// TODO: Add your control notification handler code here
	CString mstr_temp;  // 저장할 정보 임시 저장 변수 

	if(st_handler.n_menu_lock != FALSE)	return;

	mstr_temp = GetNumEditString_I(0, 100, "10");
	mn_actioncnt = atoi(mstr_temp);
	m_dgt_retry_cnt.SetVal(mn_actioncnt);

/*	
	CDialog_KeyPad pad_dlg;
	
	mn_actioncnt = (int)m_dgt_retry_cnt.GetVal();
	mstr_temp = LPCTSTR(_itoa(mn_actioncnt, chr_buf, 10));
	
	st_other.str_keypad_msg = _T("Retry Count Setting");
    
	st_other.str_pad_val = mstr_temp;
	
	st_other.str_pad_high_limit = "100";
	st_other.str_pad_low_limit = "1";
	
	CRect r;
	
	m_dgt_retry_cnt.GetWindowRect(&r);
	
	pad_dlg.m_ptRef = CPoint(r.right, r.top);
	
	mn_response = pad_dlg.DoModal();
	
	if (mn_response == IDOK)
	{
		mstr_temp = st_other.str_pad_val;
		mn_actioncnt = atoi(mstr_temp);
		
		m_dgt_retry_cnt.SetVal(mn_actioncnt);
	}
	else if (mn_response == IDCANCEL)
	{
		
	}
*/
}

void CScreen_IO_Map::OnDgtRetryWait() 
{
	// TODO: Add your control notification handler code here
	CString mstr_temp;  // 저장할 정보 임시 저장 변수 

	
	if(st_handler.n_menu_lock != FALSE)	return;

	mstr_temp = GetNumEditString_I(100, 5000, "100");
	mn_conti_wait = atoi(mstr_temp);
	m_dgt_retry_wait.SetVal(mn_conti_wait);
}

void CScreen_IO_Map::OnBtnColor() 
{
	// TODO: Add your control notification handler code here
	CDialog_Color dlg;

	if(dlg.DoModal() == IDOK)
	{
		OnChangeSheet(m_nport, m_nslave);
	}
}


int CScreen_IO_Map::OnIO_Safety(int n_io)
{
	int FuncRet = CTLBD_RET_GOOD;

	return FuncRet;
}

