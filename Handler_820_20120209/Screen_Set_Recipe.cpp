// Screen_Set_Recipe.cpp : implementation file
//

#include "stdafx.h"
#include "handler.h"
#include "Screen_Set_Recipe.h"
#include "AMTLotManager.h"

#include "io.h"  // 파일 존재 유무 검사 함수 호출하기 위해서는 반드시 INCLUDE 필요

// ******************************************************************************
// 대화 상자 클래스 추가
// ******************************************************************************
#include "Dialog_Select.h"
#include "Dialog_Message.h"
#include "Dialog_KeyBoard.h"

#include "Dialog_KeyPad.h"
// ******************************************************************************

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CScreen_Set_Recipe

IMPLEMENT_DYNCREATE(CScreen_Set_Recipe, CFormView)

CScreen_Set_Recipe::CScreen_Set_Recipe()
	: CFormView(CScreen_Set_Recipe::IDD)
{
	//{{AFX_DATA_INIT(CScreen_Set_Recipe)
	//}}AFX_DATA_INIT
}

CScreen_Set_Recipe::~CScreen_Set_Recipe()
{
}

void CScreen_Set_Recipe::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CScreen_Set_Recipe)
	DDX_Control(pDX, IDC_BTN_RECIPE_CANCLE, m_btn_recipe_cancel);
	DDX_Control(pDX, IDC_BTN_RECIPE_APPLY, m_btn_recipe_apply);
	DDX_Control(pDX, IDC_GROUP_SIZE_SETTING, m_group_size_setting);
	DDX_Control(pDX, IDC_GROUP_PITCH_DATA, m_group_pitch_data);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CScreen_Set_Recipe, CFormView)
	//{{AFX_MSG_MAP(CScreen_Set_Recipe)
	ON_WM_DESTROY()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BTN_RECIPE_APPLY, OnBtnRecipeApply)
	ON_BN_CLICKED(IDC_BTN_RECIPE_CANCLE, OnBtnRecipeCancle)
//	ON_COMMAND(ID_USER_CONTROL, OnUserControl)
	//}}AFX_MSG_MAP
	ON_MESSAGE(SSM_CLICK  , OnCellClick)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CScreen_Set_Recipe diagnostics

#ifdef _DEBUG
void CScreen_Set_Recipe::AssertValid() const
{
	CFormView::AssertValid();
}

void CScreen_Set_Recipe::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CScreen_Set_Recipe message handlers

void CScreen_Set_Recipe::OnInitialUpdate() 
{
	CFormView::OnInitialUpdate();
	
	// **************************************************************************
	// 컨트롤에 설정할 폰트 생성한다
	// **************************************************************************
	mp_recipe_font = NULL;
	mp_recipe_font = new CFont;
	mp_recipe_font->CreateFont(15,8,0,0,900,0,0,0,0,0,0,DEFAULT_QUALITY,0,"Arial");

	mp_recipe_combo_font = NULL;
	mp_recipe_combo_font = new CFont;
	mp_recipe_combo_font->CreateFont(23, 0, 0, 0, 300, 0, 0, 0, 0, 0, 0, ANTIALIASED_QUALITY, 0, "Arial");
	// **************************************************************************

	m_iPartNo = 0;
	m_iJigNumber = 0;

	OnSetRecipe_GroupBox_Set();

	OnSetRecipe_Data_Set();
	OnSetRecipe_Data_BackUp();
	OnSetRecipe_Digital_Count_Set();

	OnSetRecipe_Button_Set();
	OnSetRecipe_EditBox_Set();
	OnSetRecipe_Init_Size_Data();
	OnSetRecipe_Init_Pitch_Data();
	OnSetRecipe_Data_Change();
	
	st_handler.cwnd_recipe = this;  // 화면 핸들 초기화
}

void CScreen_Set_Recipe::OnDestroy() 
{
	// **************************************************************************
	// 생성한 폰트 정보 삭제한다
	// **************************************************************************
	delete mp_recipe_font;
	mp_recipe_font = NULL;

	delete mp_recipe_combo_font;
	mp_recipe_combo_font = NULL;
	// **************************************************************************
	
//	st_handler.cwnd_recipe = NULL;  // 화면 핸들 초기화

	CFormView::OnDestroy();
}

void CScreen_Set_Recipe::OnTimer(UINT nIDEvent) 
{	
	CFormView::OnTimer(nIDEvent);
}

void CScreen_Set_Recipe::GridColor(UINT nID, int row, int col, COLORREF bk, COLORREF tk)
{
	TSpread *Grid = (TSpread*)GetDlgItem(nID);
	Grid->SetColor(col, row, bk, tk);
	Grid->SetBackColorStyle(SS_BACKCOLORSTYLE_UNDERGRID);
	
	Grid = NULL;
	delete Grid;
}

void CScreen_Set_Recipe::GridControl(UINT nID, int type, int row, int col, int pos)
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

void CScreen_Set_Recipe::GridData(UINT nID, int row, int col, CString data)
{
	TSpread *Grid = (TSpread*)GetDlgItem(nID);
	
	Grid->SetValue(col, row, data);

	Grid = NULL;
	delete Grid;
}

void CScreen_Set_Recipe::GridFont(UINT nID, int row, int col, int size)
{
	TSpread *Grid = (TSpread *)GetDlgItem(nID);
	HFONT font;
	CString strTemp;
	
	font = CreateFont(size, 0, 0, 0, FW_BOLD, 0, 0, 0, 0, 0, 0, 0, 0, "돋음체");
	Grid->SetFont(col, row, font, TRUE);

	Grid = NULL;
	delete Grid;
}

void CScreen_Set_Recipe::GridMerge(UINT nID, int srow, int scol, int nrow, int ncol)
{
	TSpread *Grid = (TSpread*)GetDlgItem(nID);
	
	Grid->AddCellSpan(scol, srow, ncol, nrow);

	Grid = NULL;
	delete Grid;
}

void CScreen_Set_Recipe::OnCellClick(WPARAM wParam, LPARAM lParam)
{
	LPSS_CELLCOORD lpcc = (LPSS_CELLCOORD)lParam;

	TSpread *Grid; 

	CString strTmp, strChange, str_file;
	CString str_tmp, str_data;
	CString strText;
	int n_size_limit=0;
	
	if (st_handler.n_menu_lock) return;

	if (wParam == IDC_CUSTOM_PITCH)
	{
		Grid = (TSpread*)GetDlgItem(IDC_CUSTOM_PITCH);
		
		if (lpcc->Col == 1)
			return;	
		
		strText = m_pGrid.GridCellText(Grid, lpcc->Row, lpcc->Col - 1);
		st_msg.mstr_keypad_msg.Format("[Pitch] %s set", strText);
		
		strTmp = m_pGrid.GridCellText(Grid, lpcc->Row, lpcc->Col);
		st_msg.mstr_keypad_val = strTmp;
		
		strTmp = KeyPad.GetNumEditString_D(0, 140, atof(st_msg.mstr_keypad_val), st_msg.mstr_keypad_msg);
		
		md_pitch_data[lpcc->Row - 1][1] = atof(strTmp);
		m_pGrid.GridCellText(Grid, lpcc->Row, lpcc->Col, strTmp);
	}
}

void CScreen_Set_Recipe::OnSetRecipe_GroupBox_Set()
{
	CSxLogFont recipe_font(15,FW_SEMIBOLD,false,"Arial");

	m_group_size_setting.SetFont(recipe_font);
	m_group_size_setting.SetCatptionTextColor(RGB(145,25,0));
	m_group_size_setting.SetFontBold(TRUE);

	m_group_pitch_data.SetFont(recipe_font);
	m_group_pitch_data.SetCatptionTextColor(RGB(145,25,0));
	m_group_pitch_data.SetFontBold(TRUE);
}

void CScreen_Set_Recipe::OnSetRecipe_Digital_Count_Set()
{

}

void CScreen_Set_Recipe::OnSetRecipe_Data_Set()
{
}

void CScreen_Set_Recipe::OnSetRecipe_Data_BackUp()
{
}

void CScreen_Set_Recipe::OnSetRecipe_Data_Change()
{
}

void CScreen_Set_Recipe::OnSetRecipe_LogFile_Create()
{
	int i=0, j=0;
	CString str_msg;

	for (i = 0; i < 5; i++)
	{
		if (mn_size_x[i][0] != mn_size_x[i][1])
		{
			str_msg.Format("[Recipe] [%d] size x was changed by %d to %d", i, mn_size_x[i][0], mn_size_x[i][1]);
			Func.On_LogFile_Add(0, str_msg);
			Func.On_LogFile_Add(99, str_msg);
		}

		if (mn_size_y[i][0] != mn_size_y[i][1])
		{
			str_msg.Format("[Recipe] [%d] size y was changed by %d to %d", i, mn_size_y[i][0], mn_size_y[i][1]);
			Func.On_LogFile_Add(0, str_msg);
			Func.On_LogFile_Add(99, str_msg);
		}
	}	
}

void CScreen_Set_Recipe::OnSetRecipe_Data_Apply()
{
	int i=0, j=0;
}

void CScreen_Set_Recipe::OnSetRecipe_Data_Recovery()
{
	int i=0, j=0;
	for (i = 0; i < 5; i++)
	{
		mn_size_x[i][1] =			mn_size_x[i][0];
		mn_size_y[i][1] =			mn_size_y[i][0];
	}
}

void CScreen_Set_Recipe::OnSetRecipe_Init_Size_Data()
{
	TSpread *Grid;
	CString str_pos;
	int row = 6;			// 세로
	int col = 3;			// 가로
	
	int i, j;
	CString strData;
	
	CString str_tmp[6] = {"Item", "PCB Tray", "Case Tray", "Jig", "Buffer", "ULD Tray"};
	CString str_col_tmp[3] = {"Item", "X", "Y"};
	
	Grid = (TSpread*)GetDlgItem(IDC_CUSTOM_SIZE);
	Grid -> SetBool(SSB_EDITMODEPERMANENT, TRUE);
	Grid -> SetBool(SSB_SHOWCOLHEADERS, FALSE);
	Grid -> SetBool(SSB_SHOWROWHEADERS, FALSE);
	Grid -> SetBool(SSB_HORZSCROLLBAR, FALSE);
	Grid -> SetBool(SSB_VERTSCROLLBAR, FALSE);
	Grid -> SetMaxRows(row);
	Grid -> SetMaxCols(col);
	
	for (i = 0; i <= row; i++)
	{
		for (j = 0; j <= col; j++)
		{
			Grid -> SetRowHeightInPixels(i, 23);
			
			if (j == 1)
			{
				Grid -> SetColWidthInPixels(j, 106);
				GridColor(IDC_CUSTOM_SIZE, i, j, YELLOW_L, BLACK_C);
				
				GridControl(IDC_CUSTOM_SIZE, STATIC, i, j, 0);
				GridFont(IDC_CUSTOM_SIZE, i, j, 15);
			}
			else
			{
				Grid -> SetColWidthInPixels(j, 120);
				GridColor(IDC_CUSTOM_SIZE, i, j, BLACK_C, WHITE_C);
				
				GridControl(IDC_CUSTOM_SIZE, STATIC, i, j, 0);
				
				GridFont(IDC_CUSTOM_SIZE, i, j, 15);
			}
		}
	}
	
	for (i = 1; i < row + 1; i++)
	{
		GridData(IDC_CUSTOM_SIZE, i, 1, str_tmp[i - 1]);
		
		if (i == row)	continue;
		
		strData.Format("%d", mn_size_x[i - 1][1]);
		GridData(IDC_CUSTOM_SIZE, i + 1, 2, strData);
		
		strData.Format("%d", mn_size_y[i - 1][1]);
		GridData(IDC_CUSTOM_SIZE, i + 1, 3, strData);
	}
	
	for (i = 1; i < col + 1; i++)
	{
		GridColor(IDC_CUSTOM_SIZE, 1, i, ORANGE_C, WHITE_C);
		GridData(IDC_CUSTOM_SIZE, 1, i, str_col_tmp[i - 1]);
	}
	
	Grid = NULL;
	delete Grid;
}

void CScreen_Set_Recipe::OnSetRecipe_Init_Pitch_Data()
{
	TSpread *Grid;
	CString str_pos;
	int row = 8;
	int col = 2;
	int i, j;
	CString strData;
	
	CString str_tmp[8] = {"Case Tray Pitch X", "Case Tray Pitch y", "PCB Tray Pitch X", "PCB Tray Pitch y", "Jig Pitch", "Flipper Pitch", "ULD Tray Pitch X", "ULD Tray Pitch y",};
	Grid = (TSpread*)GetDlgItem(IDC_CUSTOM_PITCH);
	Grid -> SetBool(SSB_EDITMODEPERMANENT, TRUE);
	Grid -> SetBool(SSB_SHOWCOLHEADERS, FALSE);
	Grid -> SetBool(SSB_SHOWROWHEADERS, FALSE);
	Grid -> SetBool(SSB_HORZSCROLLBAR, FALSE);
	Grid -> SetBool(SSB_VERTSCROLLBAR, FALSE);
	Grid -> SetMaxRows(row);
	Grid -> SetMaxCols(col);
	
	for (i = 0; i <= row; i++)
	{
		for (j = 0; j <= col; j++)
		{
			Grid -> SetRowHeightInPixels(i, 24);
			
			if (j == 1)
			{
				Grid -> SetColWidthInPixels(j, 190);
				GridColor(IDC_CUSTOM_PITCH, i, j, YELLOW_L, BLACK_C);
				
				GridControl(IDC_CUSTOM_PITCH, STATIC, i, j, 0);
				GridFont(IDC_CUSTOM_PITCH, i, j, 15);
			}
			else
			{
				Grid -> SetColWidthInPixels(j, 155);
				GridColor(IDC_CUSTOM_PITCH, i, j, BLACK_C, WHITE_C);
				
				GridControl(IDC_CUSTOM_PITCH, STATIC, i, j, 0);
				
				GridFont(IDC_CUSTOM_PITCH, i, j, 15);
			}
		}
	}
	
	for (i = 1; i < row + 1; i++)
	{
		strData.Format("%3.2f", md_pitch_data[i - 1][1]);
		GridData(IDC_CUSTOM_PITCH, i, 1, str_tmp[i - 1]);
		GridData(IDC_CUSTOM_PITCH, i, 2, strData);
	}
	Grid = NULL;
	delete Grid;
}

void CScreen_Set_Recipe::OnBtnRecipeApply() 
{
	int n_response;  // 대화 상자 리턴 플래그
	
	//Start_www_tipssoft_com();			// 2014.12.24 - Bredmin.

	CDialog_Select  select_dlg;
	//CDialog_Message msg_dlg;			// 2014.12.24 - Bredmin.
	
	st_msg.mstr_confirm_msg = _T("Recipe : Setting Data Apply?");
	
	n_response = select_dlg.DoModal();
	
	if (n_response == IDOK)
	{
		
	}
	else if (n_response == IDCANCEL)
	{
		
	}
	//Stop_www_tipssoft_com();			// 2014.12.24 - Bredmin.
}

void CScreen_Set_Recipe::OnBtnRecipeCancle() 
{
	int n_response;  // 대화 상자 리턴 플래그
	
	//Start_www_tipssoft_com();			// 2014.12.24 - Bredmin.

	CDialog_Select  select_dlg;
	
	st_msg.mstr_confirm_msg = _T("Basic : Previous Data Recovery?");
	
	n_response = select_dlg.DoModal();
	
	if (n_response == IDOK)
	{

	}
	else if (n_response == IDCANCEL)
	{
		
	}
	//Stop_www_tipssoft_com();			// 2014.12.24 - Bredmin.
}

void CScreen_Set_Recipe::OnSetRecipe_Button_Set()
{
	// *************************************************************
	//  PAD BUTTON		                                            
	// *************************************************************
}

void CScreen_Set_Recipe::OnSetRecipe_EditBox_Set()
{
}

void CScreen_Set_Recipe::OnUserControl() 
{
	// TODO: Add your command handler code here
	
}
