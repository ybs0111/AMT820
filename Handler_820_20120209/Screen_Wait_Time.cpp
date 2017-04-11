// Screen_Wait_Time.cpp : implementation file
//

#include "stdafx.h"
#include "Handler.h"
#include "Screen_Wait_Time.h"
//#include "CXGemManager.h"
#include "AMTRegistry.h"
#include "AMTLotManager.h"
// ****************************************************************************** 
// 대화 상자 클래스 추가                                                          
// ****************************************************************************** 
#include "Dialog_Select.h"
#include "Dialog_Message.h"

#include "Dialog_KeyPad.h"
// ****************************************************************************** 

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CScreen_Wait_Time

IMPLEMENT_DYNCREATE(CScreen_Wait_Time, CFormView)

CScreen_Wait_Time::CScreen_Wait_Time()
	: CFormView(CScreen_Wait_Time::IDD)
{
	//{{AFX_DATA_INIT(CScreen_Wait_Time)
	//}}AFX_DATA_INIT
}

CScreen_Wait_Time::~CScreen_Wait_Time()
{
}

void CScreen_Wait_Time::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CScreen_Wait_Time)
	DDX_Control(pDX, IDC_GROUP_TEMP_CHECK, m_group_temp_check);
	DDX_Control(pDX, IDC_GROUP_TA_TEMP_DATA, m_group_ta_temp_data);
	DDX_Control(pDX, IDC_GROUP_TA_ETUB, m_group_ta_etub);
	DDX_Control(pDX, IDC_GROUP_TA_ETLB, m_group_ta_etlb);
	DDX_Control(pDX, IDC_GROUP_TA_DELTA_DATA, m_group_ta_delta_data);
	DDX_Control(pDX, IDC_GROUP_TA_AVG_BD, m_group_ta_avg_bd);
	DDX_Control(pDX, IDC_BTN_WAIT_APPLY, m_btn_wait_apply);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CScreen_Wait_Time, CFormView)
	//{{AFX_MSG_MAP(CScreen_Wait_Time)
	ON_WM_DESTROY()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BTN_WAIT_APPLY, OnBtnWaitApply)
	ON_BN_CLICKED(IDC_CHK_TA_FUNC_USED, OnChkTaFuncUsed)
	ON_BN_CLICKED(IDC_DGT_TA_AVG_BD, OnDgtTaAvgBd)
	ON_BN_CLICKED(IDC_DGT_TA_ETLB, OnDgtTaEtlb)
	ON_BN_CLICKED(IDC_DGT_TA_ETUB, OnDgtTaEtub)
	ON_BN_CLICKED(IDC_DGT_TEMP_CHECK, OnDgtTempCheck)
	//}}AFX_MSG_MAP
	ON_MESSAGE(SSM_CLICK, OnCellClick)
	ON_MESSAGE(WM_WAITTIME_APPLY, OnWaitTime_Info_Apply)  // DATA를 APPLY 시키기 위한 사용자 정의 메시지 선언
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CScreen_Wait_Time diagnostics

#ifdef _DEBUG
void CScreen_Wait_Time::AssertValid() const
{
	CFormView::AssertValid();
}

void CScreen_Wait_Time::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CScreen_Wait_Time message handlers
void CScreen_Wait_Time::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	
	CFormView::OnTimer(nIDEvent);
}

void CScreen_Wait_Time::OnInitialUpdate() 
{
	CFormView::OnInitialUpdate();
	
	// ************************************************************************** 
	// 컨트롤에 설정할 폰트 생성한다                                              
	// ************************************************************************** 
	mp_wait_time_font = NULL;
	mp_wait_time_font = new CFont;
	mp_wait_time_font->CreateFont(15,0,0,0,900,0,0,0,0,0,0,ANTIALIASED_QUALITY,0,"MS Sans Serif");
	// ************************************************************************** 
	OnWaitTime_Data_Display();
	OnWaitTime_GroupBox_Set();

	OnWaitTime_Init_Delta_Data();
	OnWaitTime_Init_Temp_Data();

	OnWaitTime_Display_Temp_Data();
	OnWaitTime_Display_Delta_Data();

//	CheckDlgButton( IDC_CHK_TA_FUNC_USED, st_basic.m_iTaFuncUsed );

	mn_enable = TRUE;
	st_handler.cwnd_waittime = this;  // Wait Time 화면에 대한 핸들 정보 설정 
}

void CScreen_Wait_Time::OnDestroy() 
{
	int mn_response;  // 대화 상자에 대한 리턴 값 저장 변수
	int Ret;

	//Start_www_tipssoft_com();			// 2014.12.24 - Bredmin.

	CDialog_Select select_dlg;

	//프로그램을 빠져나가면서 Save 하지 않고, Form이 Destroy되는것이라면 Setting을 포기한다는 말이다.
	//비교도 하지 말자.
	if (st_handler.b_program_exit == false)
	{
		Ret = OnWaitTime_Data_Comparison();

		if (Ret == RET_ERROR)
		{
			st_msg.mstr_confirm_msg = _T("Changed Data! Apply?");
			
			mn_response = select_dlg.DoModal();
			
			if (mn_response == IDOK)
			{
				OnWaitTime_Data_Apply();

				OnWaitTime_LogFile_Create();
			}
			else if (mn_response == IDCANCEL)
			{
				OnWaitTime_Data_Recovery();

				OnWaitTime_Data_Display();

				OnWaitTime_Data_Apply();
			}	
		}
	}

	// ************************************************************************** 
	// 생성한 폰트 정보 삭제한다                                                  
	// ************************************************************************** 
	delete mp_wait_time_font;
	mp_wait_time_font = NULL;
	// ************************************************************************** 

	st_handler.cwnd_waittime = NULL;	// Wait Time 화면에 대한 핸들 정보 저장 변수 초기화 

	//Stop_www_tipssoft_com();			// 2014.12.24 - Bredmin.
	
	CFormView::OnDestroy();
}

void CScreen_Wait_Time::OnWaitTime_Data_Set()
{
	int i;

	for(i=0; i<mn_wait_time_cnt; i++)
	{
		m_iOnWait[i][1]		= st_time.m_iWaitTime[i][1];
		m_iOffWait[i][1]		= st_time.m_iWaitTime[i][0];
		m_iLimit[i][1]			= st_time.m_iLimitTime[i];
	}
	
	OnWaitTime_Data_BackUp();

//	mn_ta_avg_bd_min[1] = st_basic.m_iTaAvgBdMin;
//	mn_ta_etlb[1] = st_basic.m_iTaETLB;
//	mn_ta_etub[1] = st_basic.m_iTaETUB;
//	mn_temp_check[1] = st_basic.m_iTempCheck;
}

void CScreen_Wait_Time::OnWaitTime_Data_Recovery()
{
	int i;
	
	for (i = 0; i < MAX_WAIT_TIME_NUM; i ++)
	{
		m_iOnWait[i][1] = m_iOnWait[i][0];
		m_iOffWait[i][1] = m_iOffWait[i][0];
		m_iLimit[i][1] = m_iLimit[i][0];
	}

//	mn_ta_avg_bd_min[1] = mn_ta_avg_bd_min[0];
//	mn_ta_etlb[1] = mn_ta_etlb[0];
//	mn_ta_etub[1] = mn_ta_etub[0];
//	mn_temp_check[1] = mn_temp_check[0];
}

void CScreen_Wait_Time::OnWaitTime_Data_Display()
{
	
	OnWaitTime_Init_Time();

	m_dgt_ta_avg_bd.SubclassDlgItem( IDC_DGT_TA_AVG_BD, this );
	m_dgt_ta_avg_bd.SetStyle( IDB_BIG3, 4 );
	m_dgt_ta_avg_bd.SetValue( mn_ta_avg_bd_min[1] );
	
	m_dgt_ta_etlb.SubclassDlgItem( IDC_DGT_TA_ETLB, this );
	m_dgt_ta_etlb.SetStyle( IDB_BIG3, 4 );
	m_dgt_ta_etlb.SetValue( mn_ta_etlb[1] );
	
	m_dgt_ta_etub.SubclassDlgItem( IDC_DGT_TA_ETUB, this );
	m_dgt_ta_etub.SetStyle( IDB_BIG3, 4 );
	m_dgt_ta_etub.SetValue( mn_ta_etub[1] );
	
	m_dgt_temp_check.SubclassDlgItem( IDC_DGT_TEMP_CHECK, this );
	m_dgt_temp_check.SetStyle( IDB_BIG3, 4 );
	m_dgt_temp_check.SetValue( mn_temp_check[1] );
}

int CScreen_Wait_Time::OnWaitTime_Data_Comparison()
{
	int Ret = CTLBD_RET_GOOD;
	int i=0;
	
	for( i = 0; i < MAX_WAIT_TIME_NUM; i ++ )
	{
		if( m_iOnWait[i][0] != m_iOnWait[i][1] )			Ret = CTLBD_RET_ERROR;
		if( m_iOffWait[i][0] != m_iOffWait[i][1] )			Ret = CTLBD_RET_ERROR;
		if( m_iLimit[i][0] != m_iLimit[i][1] )				Ret = CTLBD_RET_ERROR;
	}

	return Ret;
}

void CScreen_Wait_Time::OnWaitTime_Data_BackUp()
{
	int i=0;

	for( i = 0; i < MAX_WAIT_TIME_NUM; i ++ )
	{
		m_iOnWait[i][0] = m_iOnWait[i][1];
		m_iOffWait[i][0] = m_iOffWait[i][1];
		m_iLimit[i][0] = m_iLimit[i][1];
	}
}

void CScreen_Wait_Time::OnWaitTime_Data_Apply()
{
	int i=0;

	for( i = 0; i < MAX_WAIT_TIME_NUM; i++ )
	{
		st_time.m_iWaitTime[i][1] = m_iOnWait[i][1];
		st_time.m_iWaitTime[i][0] = m_iOffWait[i][1];
		st_time.m_iLimitTime[i] = m_iLimit[i][1];
	}
}

// ****************************************************************************** 
//  화면 정보 갱신 요청 메시지                                                    
// ****************************************************************************** 
LRESULT CScreen_Wait_Time::OnWaitTime_Info_Apply(WPARAM wParam, LPARAM lParam)  
{
	OnWaitTime_Data_Apply();		// 화면 셋팅 정보 전역 변수에 전송 함수
	
	OnWaitTime_LogFile_Create();
	
	OnWaitTime_Data_BackUp();
	
	return 0;
}

void CScreen_Wait_Time::OnWaitTime_LogFile_Create()
{
	CString str_msg;
	CString str_data;
	CString str_old_data;
	CString str_io_name;
	int i;
	
	for (i = 0; i < MAX_WAIT_TIME_NUM; i++)
	{
		str_io_name = mstr_wait_time_name[i];

		if (m_iOnWait[i][1] != m_iOnWait[i][0])
		{
			str_data.Format("%d", m_iOnWait[i][1]);
			str_old_data.Format("%d", m_iOnWait[i][0]);
			str_msg.Format("[Time Set] %s On Wait Time was changed by %s To %s", str_io_name, str_old_data, str_data);
			Func.On_LogFile_Add(0, str_msg);
			Func.On_LogFile_Add(99, str_msg);
		}

		if (m_iOffWait[i][1] != m_iOffWait[i][0])
		{
			str_data.Format("%d", m_iOffWait[i][1]);
			str_old_data.Format("%d", m_iOffWait[i][0]);
			str_msg.Format("[Time Set] %s Off Wait Time was changed by %s To %s", str_io_name, str_old_data, str_data);
			Func.On_LogFile_Add(0, str_msg);
			Func.On_LogFile_Add(99, str_msg);
		}

		if (m_iLimit[i][1] != m_iLimit[i][0])
		{
			str_data.Format("%d", m_iLimit[i][1]);
			str_old_data.Format("%d", m_iLimit[i][0]);
			str_msg.Format("[Time Set] %s Limit Time was changed by %s To %s", str_io_name, str_old_data, str_data);
			Func.On_LogFile_Add(0, str_msg);
			Func.On_LogFile_Add(99, str_msg);
		}
	}
}

void CScreen_Wait_Time::OnWaitTime_Init_Time()
{
	int i, j;
	CString str_tmp;
	mn_wait_time_cnt = 0;
	m_grid_time = (TSpread*)GetDlgItem(IDC_CUSTOM_TIME);
	
	mp_grid.GridFileOpen(m_grid_time, st_path.mstr_wait_time_map, "sheet1");
	
	
	mn_wait_time_cnt = atoi(mp_grid.GridCellText(m_grid_time, 1, 3));
	
	if(mn_wait_time_cnt == 0) return;
	
	for(i=0; i<mn_wait_time_cnt; i++)
	{
		mn_on_used[i]				= atoi(mp_grid.GridCellText(m_grid_time, i+4, 4));
		mn_off_used[i]				= atoi(mp_grid.GridCellText(m_grid_time, i+4, 5));
		mn_limit_used[i]			= atoi(mp_grid.GridCellText(m_grid_time, i+4, 6));
		mstr_wait_time_name[i]		= mp_grid.GridCellText(m_grid_time, i+4, 2);
	}
	
	OnWaitTime_Data_Set();
	
	mp_grid.GridReset(m_grid_time);
	// 대문자 
	mp_grid.GridCellSelectDisplay(m_grid_time, TRUE);
	mp_grid.GridRowHeader(m_grid_time, FALSE);
	mp_grid.GridColHeader(m_grid_time, TRUE);
	mp_grid.GridHorizontal(m_grid_time, FALSE);
	mp_grid.GridVertical(m_grid_time, TRUE);
	mp_grid.GridAutoSize(m_grid_time, FALSE);
	
	mp_grid.GridCellRows(m_grid_time, mn_wait_time_cnt);
	mp_grid.GridCellCols(m_grid_time, 5);


	for(i=0; i<mn_wait_time_cnt+1; i++)
	{
		mp_grid.GridCellHeight_L(m_grid_time, i, (double)30);
		
		if(i == 0)
		{
			for(j=0; j<5; j++)
			{
				switch(j)
				{
					case 0:
						mp_grid.GridCellWidth_L(m_grid_time, j+1, (double)5);

						mp_grid.GridCellFont(m_grid_time, i, j+1, "MS Sans Serif", 18);
						mp_grid.GridCellColor(m_grid_time, i, j+1, BLUE_D, WHITE_C);
						mp_grid.GridCellText(m_grid_time, i, j+1, "No.");
						break;

					case 1:
						mp_grid.GridCellWidth_L(m_grid_time, j+1, (double)54);

						mp_grid.GridCellFont(m_grid_time, i, j+1, "MS Sans Serif", 18);
						mp_grid.GridCellColor(m_grid_time, i, j+1, BLUE_D, WHITE_C);
						mp_grid.GridCellText(m_grid_time, i, j+1, "Name");
						break;

					case 2:
						mp_grid.GridCellWidth_L(m_grid_time, j+1, (double)20);

						mp_grid.GridCellFont(m_grid_time, i, j+1, "MS Sans Serif", 18);
						mp_grid.GridCellColor(m_grid_time, i, j+1, BLUE_D, WHITE_C);
						mp_grid.GridCellText(m_grid_time, i, j+1, "On Time (ms)");
						break;

					case 3:
						mp_grid.GridCellWidth_L(m_grid_time, j+1, (double)20);

						mp_grid.GridCellFont(m_grid_time, i, j+1, "MS Sans Serif", 18);
						mp_grid.GridCellColor(m_grid_time, i, j+1, BLUE_D, WHITE_C);
						mp_grid.GridCellText(m_grid_time, i, j+1, "Off Time (ms)");
						break;

					case 4:
						mp_grid.GridCellWidth_L(m_grid_time, j+1, (double)20);

						mp_grid.GridCellFont(m_grid_time, i, j+1, "MS Sans Serif", 18);
						mp_grid.GridCellColor(m_grid_time, i, j+1, BLUE_D, WHITE_C);
						mp_grid.GridCellText(m_grid_time, i, j+1, "Limit Time (ms)");
						break;
				}

				mp_grid.GridCellControlStatic(m_grid_time, i, j+1);
			}
		}
		else
		{
			for(j=0; j<5; j++)
			{
				switch(j)
				{
					case 0:
						mp_grid.GridCellWidth_L(m_grid_time, j+1, 5);

						mp_grid.GridCellFont(m_grid_time, i, j+1, "MS Sans Serif", 14);
						mp_grid.GridCellColor(m_grid_time, i, j+1, BLUE_D, WHITE_C);
						str_tmp.Format("%d", i);
						mp_grid.GridCellText(m_grid_time, i, j+1, str_tmp);
						break;

					case 1:
						mp_grid.GridCellWidth_L(m_grid_time, j+1, 54);

						mp_grid.GridCellFont(m_grid_time, i, j+1, "MS Sans Serif", 14);
						mp_grid.GridCellColor(m_grid_time, i, j+1, BLACK_L, YELLOW_C);
						mp_grid.GridCellText(m_grid_time, i, j+1, mstr_wait_time_name[i-1]);
						break;

					case 2:
						mp_grid.GridCellWidth_L(m_grid_time, j+1, 20);
						
						if(mn_on_used[i-1] == CTL_YES)
						{
							mp_grid.GridCellFont(m_grid_time, i, j+1, "MS Sans Serif", 18);
							mp_grid.GridCellColor(m_grid_time, i, j+1, GREEN_L, BLACK_C);
							str_tmp.Format("%d", m_iOnWait[i-1][1]);
							mp_grid.GridCellText(m_grid_time, i, j+1, str_tmp);
						}
						else
						{
							mp_grid.GridCellColor(m_grid_time, i, j+1, DSERBLE_BC, YELLOW_C);
							mp_grid.GridCellText(m_grid_time, i, j+1, "-");
						}
						break;

					case 3:
						mp_grid.GridCellWidth_L(m_grid_time, j+1, 20);

						if(mn_off_used[i-1] == CTL_YES)
						{
							mp_grid.GridCellFont(m_grid_time, i, j+1, "MS Sans Serif", 18);
							mp_grid.GridCellColor(m_grid_time, i, j+1, RED_L, BLACK_C);
							str_tmp.Format("%d", m_iOffWait[i-1][1]);
							mp_grid.GridCellText(m_grid_time, i, j+1, str_tmp);
						}
						else
						{
							mp_grid.GridCellColor(m_grid_time, i, j+1, DSERBLE_BC, YELLOW_C);
							mp_grid.GridCellText(m_grid_time, i, j+1, "-");
						}
						break;

					case 4:
						mp_grid.GridCellWidth_L(m_grid_time, j+1, 20);

						if(mn_limit_used[i-1] == CTL_YES)
						{
							mp_grid.GridCellFont(m_grid_time, i, j+1, "MS Sans Serif", 18);
							mp_grid.GridCellColor(m_grid_time, i, j+1, YELLOW_L, BLACK_C);
							str_tmp.Format("%d", m_iLimit[i-1][1]);
							mp_grid.GridCellText(m_grid_time, i, j+1, str_tmp);
						}
						else
						{
							mp_grid.GridCellColor(m_grid_time, i, j+1, DSERBLE_BC, YELLOW_C);
							mp_grid.GridCellText(m_grid_time, i, j+1, "-");
						}
						break;
				}
	
				mp_grid.GridCellControlStatic(m_grid_time, i, j+1);
			}
		}
	}
}

void CScreen_Wait_Time::GridColor(UINT nID, int row, int col, COLORREF bk, COLORREF tk)
{
	TSpread *Grid = (TSpread*)GetDlgItem(nID);
	Grid->SetColor(col, row, bk, tk);
	Grid->SetBackColorStyle(SS_BACKCOLORSTYLE_UNDERGRID);

	Grid = NULL;
	delete Grid;
}

void CScreen_Wait_Time::GridControl(UINT nID, int type, int row, int col, int pos)
{
	TSpread *Grid = (TSpread*)GetDlgItem(nID);
	
	SS_CELLTYPE CellType;
	SS_DATEFORMAT dateFormat = { TRUE, '/', IDF_DDMMYY, FALSE };
	SS_TIMEFORMAT tmFormat;
	CString strList;
	strList = "combo1\tcombo2\tcombo3\tcombo4\tcombo5\tcombo6";
	double a = 0.5;

	switch (type)
	{
		case DATE:			
			Grid->SetTypeDate(&CellType, 0 , &dateFormat, NULL, NULL);
			break;

		case EDIT:
			Grid->SetTypeEdit(&CellType,ES_CENTER,60,SS_CHRSET_CHR,SS_CASE_NOCASE);
			break;

		case COMBO:
			// SetValue(col, row, data); data = ""0"(1선택) data = "1"(2선택) data = "2"(3선택)
			Grid->SetTypeComboBox(&CellType, 0, strList);
			break;

		case BUTTON:
			if (pos == 0)
			{
				Grid->SetTypeButton(&CellType, SBS_DEFPUSHBUTTON, "Go", NULL, SUPERBTN_PICT_NONE,
					NULL, SUPERBTN_PICT_NONE, SUPERBTN_TYPE_NORMAL, 2, NULL);
			}
			else
			{
				Grid->SetTypeButton(&CellType, SBS_DEFPUSHBUTTON, "Read", NULL, SUPERBTN_PICT_NONE,
					NULL, SUPERBTN_PICT_NONE,SUPERBTN_TYPE_NORMAL, 2, NULL);
			}
			break;

		case STATIC:
			Grid->SetTypeStaticText(&CellType, SS_TEXT_CENTER | SS_TEXT_VCENTER);
			break;

		case TIME:
			tmFormat.b24Hour=TRUE;
			tmFormat.bSpin=FALSE;
			tmFormat.bSeconds=FALSE;
			tmFormat.cSeparator=':';
			Grid->SetTypeTime(&CellType, 0, &tmFormat, NULL, NULL);
			break;

		case PERCENT:
			Grid->SetTypePercentEx(&CellType,ES_LEFTALIGN,2,.01,100,".",0,0,0,0,1.001);
			break;

		case CHECK1:
			Grid->SetTypeCheckBox(&CellType,BS_CENTER, "", 
				"CheckUp", BT_BITMAP,
				"CheckDown", BT_BITMAP,
				"CheckFocusUp", BT_BITMAP,
				"CheckFocusDown", BT_BITMAP,
				"CheckDisabledUp", BT_BITMAP,
				"CheckDisabledDown", BT_BITMAP);
			break;

		case NUMBER:
			Grid->SetTypeNumberEx(&CellType,0,pos, 0, 10000,0,".",",",0,0,1,0,1.001);
			break;
	}
	
	Grid->SetCellType(col,row,&CellType);

	Grid = NULL;
	delete Grid;
}

void CScreen_Wait_Time::GridData(UINT nID, int row, int col, CString data)
{
	TSpread *Grid = (TSpread*)GetDlgItem(nID);
	
	Grid->SetValue(col, row, data);

	Grid = NULL;
	delete Grid;
}

void CScreen_Wait_Time::GridFont(UINT nID, int row, int col, int size)
{
	TSpread *Grid = (TSpread *)GetDlgItem(nID);
	HFONT font;
	CString strTemp;
	
	font = CreateFont(size, 0, 0, 0, FW_BOLD, 0, 0, 0, 0, 0, 0, 0, 0, "돋음체");
	Grid->SetFont(col, row, font, TRUE);

	Grid = NULL;
	delete Grid;
}

void CScreen_Wait_Time::GridMerge(UINT nID, int srow, int scol, int nrow, int ncol)
{
	TSpread *Grid = (TSpread*)GetDlgItem(nID);
	
	Grid->AddCellSpan(scol, srow, ncol, nrow);

	Grid = NULL;
	delete Grid;
}

void CScreen_Wait_Time::OnCellClick(WPARAM wParam, LPARAM lParam)
{
	int nId = -1;
	LPSS_CELLCOORD lpcc = (LPSS_CELLCOORD)lParam;
	
	if (mn_enable != TRUE)					return;
	if (lpcc->Col == 1 || lpcc->Col == 2)	return;

	switch (wParam)
	{
	case IDC_CUSTOM_TIME:
		nId = 0;
		break;	
	}
	
	if (nId > -1)
	{
		if (lpcc->Col == 3)
		{
			if (mn_on_used[lpcc->Row-1] == CTL_NO) return;
		}
		else if (lpcc->Col == 4)
		{
			if (mn_off_used[lpcc->Row-1] == CTL_NO) return;
		}
		else if (lpcc->Col == 5)
		{
			if (mn_limit_used[lpcc->Row-1] == CTL_NO) return;
		}

		OnChangeValue(nId, lpcc->Row, lpcc->Col);
	}
}

void CScreen_Wait_Time::OnChangeValue(int nid, int nrow, int ncol)
{
	TSpread *Grid; 
	CString strTmp;
	char chvalue[100];
	char chText[100];
	CString strText;
	CString strText2;
	int nValue, i;

	CString str_temp;	// 저장할 정보 임시 저장 변수 

	if (nid == 0)
	{
		Grid = (TSpread*)GetDlgItem(IDC_CUSTOM_TIME);
		
		Grid->GetValue(ncol, nrow, chvalue);
		nValue = atoi(chvalue);
		strTmp.Format("%d", nValue);
		
		Grid->GetValue(2, nrow, chText);
		strText = chText;

		Grid->GetValue(ncol, 0, chText);
		strText2 = chText;

		if (nrow == 1)		// 전체 선택.
		{
			st_msg.mstr_keypad_msg.Format("[TIME SET] All Contents, %s set", strText2);
		}
		else
		{
			st_msg.mstr_keypad_msg.Format("[TIME SET] %s, %s set", strText, strText2);
		}
		
		st_msg.mstr_keypad_val = strTmp;
		
		strTmp = KeyPad.GetNumEditString_I(0, 99999, atoi(st_msg.mstr_keypad_val), st_msg.mstr_keypad_msg);

		if (ncol == 1)
		{
		}
		else if (ncol == 3)		// On Wait
		{
			if (nrow == 0)		// 전체 선택.
			{
				for (i = 0; i < MAX_WAIT_TIME_NUM; i++)
				{	
					if (mn_on_used[i] == CTL_NO) continue;

					m_iOnWait[i][1] = atoi(strTmp);
					strTmp.Format("%d", m_iOnWait[i][1]);

					Grid->SetValue(ncol, i + 1, strTmp);
				}
				return;
			}
			else
			{
				m_iOnWait[nrow-1][1] = atoi(strTmp);
				strTmp.Format("%d", m_iOnWait[nrow-1][1]);
			}
		}
		else if (ncol == 4)		// Off Wait
		{
			if (nrow == 0)		// 전체 선택.
			{
				for (i = 0; i < MAX_WAIT_TIME_NUM; i++)
				{	
					if (mn_off_used[i] == CTL_NO) continue;
					m_iOffWait[i][1] = atoi(strTmp);
					strTmp.Format("%d", m_iOffWait[i][1]);
					
					Grid->SetValue(ncol, i + 1, strTmp);
				}
				return;
			}
			else
			{
				m_iOffWait[nrow-1][1] = atoi(strTmp);
				strTmp.Format("%d", m_iOffWait[nrow-1][1]);
			}
		}
		else if (ncol == 5)		// Limit
		{
			if (nrow == 0)		// 전체 선택.
			{
				for (i = 0; i < MAX_WAIT_TIME_NUM; i++)
				{	
					if (mn_limit_used[i] == CTL_NO) continue;
					m_iLimit[i][1] = atoi(strTmp);
					strTmp.Format("%d", m_iLimit[i][1]);
					
					Grid->SetValue(ncol, i + 1, strTmp);
				}
				return;
			}
			else
			{
				m_iLimit[nrow-1][1] = atoi(strTmp);
				strTmp.Format("%d", m_iLimit[nrow-1][1]);
			}
		}

		Grid->SetValue(ncol, nrow, strTmp);
	}
}

void CScreen_Wait_Time::On_Controls_Enable(int nState)
{
	mn_enable = nState;
	m_btn_wait_apply.EnableWindow(nState);
}	

void CScreen_Wait_Time::OnBtnWaitApply() 
{
	int n_response;  // 대화 상자 리턴 플래그

	//Start_www_tipssoft_com();			// 2014.12.24 - Bredmin.

	CDialog_Select  select_dlg;
	
	st_other.str_confirm_msg = _T("Wait Time : Setting Data Apply?");

	n_response = select_dlg.DoModal();
	
	if (n_response == IDOK)
	{
		OnWaitTime_Data_Apply();			// 화면 셋팅 정보 전역 변수에 전송 함수
		
		OnWaitTime_LogFile_Create();
		
		OnWaitTime_Data_BackUp();

		/*
		if( st_basic.m_iXGemMode == CTL_YES )
		{
			CMyBasicData    mcls_m_basic;		// 기본 셋팅 정보 로딩 및 저장 클래스
			
			mcls_m_basic.OnPPID_Data_Save();
			
			if( g_XGemMgr.IsXGemRdy() )
			{
				g_XGemMgr.XGEMUploadRecipe(st_basic.mstr_device_name, st_path.mstr_path_dvc);
			}
		}
		*/
	}
	else if (n_response == IDCANCEL)
	{
		
	}

	//Stop_www_tipssoft_com();			// 2014.12.24 - Bredmin.
}

void CScreen_Wait_Time::OnChkTaFuncUsed() 
{
	CString mstr_temp;
	
//	st_basic.m_iTaFuncUsed = IsDlgButtonChecked( IDC_CHK_TA_FUNC_USED );
	
//	mstr_temp.Format("%d", st_basic.m_iTaFuncUsed );
//	:: WritePrivateProfileString("BasicData", "Ta_Func_Used", LPCTSTR(mstr_temp), st_path.mstr_basic);
}

void CScreen_Wait_Time::OnWaitTime_GroupBox_Set()
{
	CSxLogFont wait_font(15,FW_SEMIBOLD,false,"Arial");
	
	m_group_ta_avg_bd.SetFont(wait_font);
	m_group_ta_avg_bd.SetCatptionTextColor(RGB(145,25,0));
	m_group_ta_avg_bd.SetFontBold(TRUE);
	
	m_group_ta_temp_data.SetFont(wait_font);
	m_group_ta_temp_data.SetCatptionTextColor(RGB(145,25,0));
	m_group_ta_temp_data.SetFontBold(TRUE);
	
	m_group_ta_delta_data.SetFont(wait_font);
	m_group_ta_delta_data.SetCatptionTextColor(RGB(145,25,0));
	m_group_ta_delta_data.SetFontBold(TRUE);
	
	m_group_ta_etub.SetFont(wait_font);
	m_group_ta_etub.SetCatptionTextColor(RGB(145,25,0));
	m_group_ta_etub.SetFontBold(TRUE);
	
	m_group_ta_etlb.SetFont(wait_font);
	m_group_ta_etlb.SetCatptionTextColor(RGB(145,25,0));
	m_group_ta_etlb.SetFontBold(TRUE);
	
	m_group_temp_check.SetFont(wait_font);
	m_group_temp_check.SetCatptionTextColor(RGB(145,25,0));
	m_group_temp_check.SetFontBold(TRUE);
}

void CScreen_Wait_Time::OnWaitTime_Init_Temp_Data()
{
	TSpread *Grid; 
	
	CString strPos;
	int row = 3;
	int col = 6;
	int i, j;
	
	CString strTmp[6] = { "Lot ID", "Ta", "Ts_min", "Ts_target", "ts_max", "Delta" };
	
	Grid = (TSpread*)GetDlgItem(IDC_CUSTOM_TA_TEMP_DATA);
	
	Grid->SetBool(SSB_EDITMODEPERMANENT, TRUE); // cell선택시 박스 제거
	Grid->SetBool(SSB_SHOWCOLHEADERS, FALSE);	// col header 삭제
	Grid->SetBool(SSB_SHOWROWHEADERS, FALSE);	// row header 삭제
	Grid->SetBool(SSB_HORZSCROLLBAR, FALSE);	// 좌우 스크롤바
	Grid->SetBool(SSB_VERTSCROLLBAR, FALSE);	// 상하 스크롤바
	
	Grid->SetMaxRows(row);
	Grid->SetMaxCols(col);
	
	for (i = 1; i <= row; i++)
	{	
		for (j = 1; j <= col; j++)
		{
			Grid->SetRowHeightInPixels(i, 22);
			
			if (j == 1)
			{
				Grid->SetColWidthInPixels(j, 300);
			}
			else
			{
				Grid->SetColWidthInPixels(j, 95);
			}
			
			if ( i == 1 )
			{
				GridColor(IDC_CUSTOM_TA_TEMP_DATA, i, j, BLUE_C, WHITE_C);
			}
			else
			{
				GridColor(IDC_CUSTOM_TA_TEMP_DATA, i, j, WHITE_C, BLACK_C);
			}
			
			GridControl(IDC_CUSTOM_TA_TEMP_DATA, STATIC, i, j, 0);
			GridFont(IDC_CUSTOM_TA_TEMP_DATA, i, j, 14);
		}
	}
	
	for (i = 1; i < col + 1; i++)
	{
		GridData(IDC_CUSTOM_TA_TEMP_DATA, 1, i, strTmp[i - 1]);
	}
	
	Grid = NULL;
	delete Grid;
}

void CScreen_Wait_Time::OnWaitTime_Init_Delta_Data()
{
	TSpread *Grid; 
	
	CString strPos;
	int row = 2;
	int col = 32;
	int i, j;
	CString strTemp;
	
	CString strTmp[32] = { "1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12", "13", "14", "15", "16", "17", "18", "19", "20", "21", "22", "23", "24", "25", "26", "27", "28", "29", "30", "31", "32" };
	
	Grid = (TSpread*)GetDlgItem(IDC_CUSTOM_TA_DELTA_DATA);
	
	Grid->SetBool(SSB_EDITMODEPERMANENT, TRUE); // cell선택시 박스 제거
	Grid->SetBool(SSB_SHOWCOLHEADERS, FALSE);	// col header 삭제
	Grid->SetBool(SSB_SHOWROWHEADERS, FALSE);	// row header 삭제
	Grid->SetBool(SSB_HORZSCROLLBAR, FALSE);	// 좌우 스크롤바
	Grid->SetBool(SSB_VERTSCROLLBAR, FALSE);	// 상하 스크롤바
	
	Grid->SetMaxRows(row);
	Grid->SetMaxCols(col);
	
	for (i = 1; i <= row; i++)
	{	
		for (j = 1; j <= col; j++)
		{
			Grid->SetRowHeightInPixels(i, 22);
			Grid->SetColWidthInPixels(j, 50);
			
			if ( i == 1 )
			{
				GridColor(IDC_CUSTOM_TA_DELTA_DATA, i, j, ORANGE_C, WHITE_C);
			}
			else
			{
				GridColor(IDC_CUSTOM_TA_DELTA_DATA, i, j, WHITE_C, BLACK_C);
			}
			
			GridControl(IDC_CUSTOM_TA_DELTA_DATA, STATIC, i, j, 0);
			GridFont(IDC_CUSTOM_TA_DELTA_DATA, i, j, 14);
		}
	}
	
	for (i = 1; i < col + 1; i++)
	{
		GridData(IDC_CUSTOM_TA_DELTA_DATA, 1, i, strTmp[i - 1]);
	}
	
	Grid = NULL;
	delete Grid;
}

void CScreen_Wait_Time::OnWaitTime_Display_Delta_Data()
{
	int i = 0;
	CString strTemp = "";

//	for (i = 1; i < 33; i++)
//	{
//		strTemp.Format( "%d", st_tsspd[ i - 1 ].m_iDeltaData );
//		GridData(IDC_CUSTOM_TA_DELTA_DATA, 2, i, strTemp);
//	}
}

void CScreen_Wait_Time::OnWaitTime_Display_Temp_Data()
{
	/*
	CString strTemp = "";

	GridData( IDC_CUSTOM_TA_TEMP_DATA, 2, 1, st_handler.mstr_lot_name[0] );

	strTemp.Format( "%d", g_AMTLotMgr.FindLotByName( st_handler.mstr_lot_name[0] ).m_iTa );
	GridData( IDC_CUSTOM_TA_TEMP_DATA, 2, 2, strTemp );

	strTemp.Format( "%d", g_AMTLotMgr.FindLotByName( st_handler.mstr_lot_name[0] ).m_iTsMin );
	GridData( IDC_CUSTOM_TA_TEMP_DATA, 2, 3, strTemp );

	strTemp.Format( "%d", g_AMTLotMgr.FindLotByName( st_handler.mstr_lot_name[0] ).m_iTsTarget );
	GridData( IDC_CUSTOM_TA_TEMP_DATA, 2, 4, strTemp );

	strTemp.Format( "%d", g_AMTLotMgr.FindLotByName( st_handler.mstr_lot_name[0] ).m_iTsMax );
	GridData( IDC_CUSTOM_TA_TEMP_DATA, 2, 5, strTemp );

	strTemp.Format( "%d", g_AMTLotMgr.FindLotByName( st_handler.mstr_lot_name[0] ).m_iTsDelta );
	GridData( IDC_CUSTOM_TA_TEMP_DATA, 2, 6, strTemp );


	GridData( IDC_CUSTOM_TA_TEMP_DATA, 3, 1, st_handler.mstr_lot_name[1] );
	
	strTemp.Format( "%d", g_AMTLotMgr.FindLotByName( st_handler.mstr_lot_name[1] ).m_iTa );
	GridData( IDC_CUSTOM_TA_TEMP_DATA, 3, 2, strTemp );
	
	strTemp.Format( "%d", g_AMTLotMgr.FindLotByName( st_handler.mstr_lot_name[1] ).m_iTsMin );
	GridData( IDC_CUSTOM_TA_TEMP_DATA, 3, 3, strTemp );
	
	strTemp.Format( "%d", g_AMTLotMgr.FindLotByName( st_handler.mstr_lot_name[1] ).m_iTsTarget );
	GridData( IDC_CUSTOM_TA_TEMP_DATA, 3, 4, strTemp );
	
	strTemp.Format( "%d", g_AMTLotMgr.FindLotByName( st_handler.mstr_lot_name[1] ).m_iTsMax );
	GridData( IDC_CUSTOM_TA_TEMP_DATA, 3, 5, strTemp );

	strTemp.Format( "%d", g_AMTLotMgr.FindLotByName( st_handler.mstr_lot_name[1] ).m_iTsDelta );
	GridData( IDC_CUSTOM_TA_TEMP_DATA, 3, 6, strTemp );
	*/
}

void CScreen_Wait_Time::OnDgtTaEtlb() 
{
	int mn_response;	// 대화 상자에 대한 리턴 값 저장 변수
	CString mstr_temp;  // 저장할 정보 임시 저장 변수 
	char chr_buf[20] ;
	int mn_min;
	
	CDialog_KeyPad pad_dlg;
	
	mn_min = m_dgt_ta_etlb.GetValue();
	mstr_temp = LPCTSTR(_itoa(mn_min, chr_buf, 10));
	
	st_msg.mstr_keypad_msg = "Ta Effective Temperature Lower Bound";
    
	st_msg.mstr_keypad_val = mstr_temp;
	
	st_msg.mstr_pad_high_limit = "10";
	st_msg.mstr_pad_low_limit = "0";
	
	st_msg.mn_dot_use = FALSE;
	
	CRect r;
	
	m_dgt_ta_etlb.GetWindowRect(&r);
	
	pad_dlg.m_ptRef = CPoint(r.right, r.top);
	
	mn_response = pad_dlg.DoModal();
	
	if (mn_response == IDOK)
	{
		mn_ta_etlb[1] = atoi(st_msg.mstr_keypad_val);
		m_dgt_ta_etlb.SetValue(mn_ta_etlb[1]);
	}
	else if (mn_response == IDCANCEL)
	{
		
	}
}

void CScreen_Wait_Time::OnDgtTaEtub() 
{
	int mn_response;	// 대화 상자에 대한 리턴 값 저장 변수
	CString mstr_temp;  // 저장할 정보 임시 저장 변수 
	char chr_buf[20] ;
	int mn_min;
	
	CDialog_KeyPad pad_dlg;
	
	mn_min = m_dgt_ta_etub.GetValue();
	mstr_temp = LPCTSTR(_itoa(mn_min, chr_buf, 10));
	
	st_msg.mstr_keypad_msg = "Ta Effective Temperature Upper Bound";
    
	st_msg.mstr_keypad_val = mstr_temp;
	
	st_msg.mstr_pad_high_limit = "200";
	st_msg.mstr_pad_low_limit = "50";
	
	st_msg.mn_dot_use = FALSE;
	
	CRect r;
	
	m_dgt_ta_etub.GetWindowRect(&r);
	
	pad_dlg.m_ptRef = CPoint(r.right, r.top);
	
	mn_response = pad_dlg.DoModal();
	
	if (mn_response == IDOK)
	{
		mn_ta_etub[1] = atoi(st_msg.mstr_keypad_val);
		m_dgt_ta_etub.SetValue(mn_ta_etub[1]);
	}
	else if (mn_response == IDCANCEL)
	{
		
	}
}

void CScreen_Wait_Time::OnDgtTempCheck() 
{
	int mn_response;	// 대화 상자에 대한 리턴 값 저장 변수
	CString mstr_temp;  // 저장할 정보 임시 저장 변수 
	char chr_buf[20] ;
	int mn_min;
	
	CDialog_KeyPad pad_dlg;
	
	mn_min = m_dgt_temp_check.GetValue();
	mstr_temp = LPCTSTR(_itoa(mn_min, chr_buf, 10));
	
	st_msg.mstr_keypad_msg = "Effective Temperature Check";
    
	st_msg.mstr_keypad_val = mstr_temp;
	
	st_msg.mstr_pad_high_limit = "200";
	st_msg.mstr_pad_low_limit = "1";
	
	st_msg.mn_dot_use = FALSE;
	
	CRect r;
	
	m_dgt_temp_check.GetWindowRect(&r);
	
	pad_dlg.m_ptRef = CPoint(r.right, r.top);
	
	mn_response = pad_dlg.DoModal();
	
	if (mn_response == IDOK)
	{
		mn_temp_check[1] = atoi(st_msg.mstr_keypad_val);
		m_dgt_temp_check.SetValue(mn_temp_check[1]);
	}
	else if (mn_response == IDCANCEL)
	{
		
	}
}

void CScreen_Wait_Time::OnDgtTaAvgBd() 
{
	int mn_response;	// 대화 상자에 대한 리턴 값 저장 변수
	CString mstr_temp;  // 저장할 정보 임시 저장 변수 
	char chr_buf[20] ;
	int mn_min;
	
	CDialog_KeyPad pad_dlg;
	
	mn_min = m_dgt_ta_avg_bd.GetValue();
	mstr_temp = LPCTSTR(_itoa(mn_min, chr_buf, 10));
	
	st_msg.mstr_keypad_msg = "Ta평균산출 모듈최소수량";
    
	st_msg.mstr_keypad_val = mstr_temp;
	
	st_msg.mstr_pad_high_limit = "20";
	st_msg.mstr_pad_low_limit = "1";
	
	st_msg.mn_dot_use = FALSE;
	
	CRect r;
	
	m_dgt_ta_avg_bd.GetWindowRect(&r);
	
	pad_dlg.m_ptRef = CPoint(r.right, r.top);
	
	mn_response = pad_dlg.DoModal();
	
	if (mn_response == IDOK)
	{
		mn_ta_avg_bd_min[1] = atoi(st_msg.mstr_keypad_val);
		m_dgt_ta_avg_bd.SetValue(mn_ta_avg_bd_min[1]);
	}
	else if (mn_response == IDCANCEL)
	{
		
	}
}
