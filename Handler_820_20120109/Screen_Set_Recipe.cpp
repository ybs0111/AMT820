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
	DDX_Control(pDX, IDC_GROUP_DATA_SETTING, m_group_data_setting);
	DDX_Control(pDX, IDC_GROUP_EPOXY_USECNT, m_group_epoxt_limit_usecnt);
	DDX_Control(pDX, IDC_MSG_Y_RUBB_OFFSET, m_msg_HsRub_Y_Offset);
	DDX_Control(pDX, IDC_MSG_X_RUBB_OFFSET, m_msg_HsRub_X_Offset);
	DDX_Control(pDX, IDC_DGT_HS_RUB_Y_OFFSET, m_dgt_HsRub_Y_Offset);
	DDX_Control(pDX, IDC_DGT_HS_RUB_X_OFFSET, m_dgt_HsRub_X_Offset);
	DDX_Control(pDX, IDC_DGT_EPOXY_USECNT, m_dgt_Epoxy_UseCnt);
	DDX_Control(pDX, IDC_GROUP_DISPENSERVPPMA, m_group_DispenserVppmA);
	DDX_Control(pDX, IDC_DGT_DISPENSERVPPMA, m_dgt_DispenserVppmA);
	DDX_Control(pDX, IDC_BTN_RECIPE_CANCLE, m_btn_recipe_cancel);
	DDX_Control(pDX, IDC_BTN_RECIPE_APPLY, m_btn_recipe_apply);
	DDX_Control(pDX, IDC_GROUP_SIZE_SETTING, m_group_size_setting);
	DDX_Control(pDX, IDC_GROUP_PITCH_DATA, m_group_pitch_data);
	DDX_Control(pDX, IDC_GROUP_CARRIER_MOVE, m_group_Carrier_Move);
	DDX_Control(pDX, IDC_MSG_DISTANCE, m_msg_Distance);
	DDX_Control(pDX, IDC_MSG_OFFSET_30, m_msg_Offset_30);
	DDX_Control(pDX, IDC_MSG_OFFSET_20, m_msg_Offset_20);
	DDX_Control(pDX, IDC_MSG_OFFSET_10, m_msg_Offset_10);
	DDX_Control(pDX, IDC_MSG_OFFSET, m_msg_Offset);
	DDX_Control(pDX, IDC_DGT_OFFSET_DIS, m_dgt_Offset_Dis);
	DDX_Control(pDX, IDC_DGT_OFFSET_30, m_dgt_Offset_30);
	DDX_Control(pDX, IDC_DGT_OFFSET_20, m_dgt_Offset_20);
	DDX_Control(pDX, IDC_DGT_OFFSET_10, m_dgt_Offset_10);
	DDX_Control(pDX, IDC_DGT_OFFSET, m_dgt_Offset);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CScreen_Set_Recipe, CFormView)
	//{{AFX_MSG_MAP(CScreen_Set_Recipe)
	ON_WM_DESTROY()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BTN_RECIPE_APPLY, OnBtnRecipeApply)
	ON_BN_CLICKED(IDC_BTN_RECIPE_CANCLE, OnBtnRecipeCancle)
	ON_BN_CLICKED(IDC_DGT_DISPENSERVPPMA, OnDgtDispenservppma)
	ON_BN_CLICKED(IDC_DGT_OFFSET, OnDgtOffset)
	ON_BN_CLICKED(IDC_DGT_OFFSET_10, OnDgtOffset10)
	ON_BN_CLICKED(IDC_DGT_OFFSET_20, OnDgtOffset20)
	ON_BN_CLICKED(IDC_DGT_OFFSET_30, OnDgtOffset30)
	ON_BN_CLICKED(IDC_DGT_OFFSET_DIS, OnDgtOffsetDis)
	ON_BN_CLICKED(IDC_DGT_EPOXY_USECNT, OnDgtEpoxyUsecnt)
	ON_BN_CLICKED(IDC_DGT_HS_RUB_X_OFFSET, OnDgtHsRubXOffset)
	ON_BN_CLICKED(IDC_DGT_HS_RUB_Y_OFFSET, OnDgtHsRubYOffset)
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
	
	//kwlee 2017.0410
	OnSetRecipe_Data_Set();
	OnInitGrid();
	OnSetRecipe_Label(); //kwlee 2017.0416
	OnSetRecipe_GroupBox_Set();

//	OnSetRecipe_Data_BackUp();
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
	CDialog_Message msg_dlg;
	CDialog_Select	select_dlg;
	CString			str_tmp;
// 	int				n_response;
	int             nCnt;
	
	CString mstr_temp;  // 저장할 정보 임시 저장 변수 
	//	char chr_buf[20] ;
	double md_Temp;
	
	nCnt = lpcc->Row;
	
	//kwlee 2017.0403
	if (nCnt <= 0 || g_lotMgr.GetLotCount() > 0)
	{
		return;
	}
	
	if (st_handler.n_menu_lock) return;

	if (wParam == IDC_CUSTOM_DATA)
	{
		m_grid_operate = (TSpread*)GetDlgItem(IDC_CUSTOM_DATA);
		
		switch(lpcc->Row)
		{
		
		case 2:
			md_Temp = m_dLoaderTransferTrayDeviceGap[1];

			//mstr_temp = LPCTSTR(_ltoa(md_X, chr_buf, 10));
			mstr_temp.Format("%0.2f",md_Temp);
			st_msg.mstr_keypad_msg = _T("UnPressPos거리");
			
			st_msg.mstr_keypad_val = mstr_temp;
			
			mstr_temp = KeyPad.GetNumEditString_D(1, 10000, atof(st_msg.mstr_keypad_val), st_msg.mstr_keypad_msg);
			m_dLoaderTransferTrayDeviceGap[1] = atof(mstr_temp);
			//int iTemp = atoi(mstr_temp);
			//g_dataMgr.SetDataTemp( &st_basic.n_tray_x, iTemp );
			mstr_temp.Format("%0.2f",m_dLoaderTransferTrayDeviceGap[1]);
			m_p_grid.GridCellText(m_grid_operate, 2, 1, mstr_temp);
			break;

		case 4:
			md_Temp = m_dSatbleTime[1];
			
			//mstr_temp = LPCTSTR(_ltoa(md_X, chr_buf, 10));
			mstr_temp.Format("%0.2f",md_Temp);
			st_msg.mstr_keypad_msg = _T("Satble Time");
			
			st_msg.mstr_keypad_val = mstr_temp;
			
			mstr_temp = KeyPad.GetNumEditString_D(1, 10000, atof(st_msg.mstr_keypad_val), st_msg.mstr_keypad_msg);
			m_dSatbleTime[1] = atol(mstr_temp);
			//int iTemp = atoi(mstr_temp);
			//g_dataMgr.SetDataTemp( &st_basic.n_tray_x, iTemp );
			mstr_temp.Format("%0.2f",(double)m_dSatbleTime[1]);
			m_p_grid.GridCellText(m_grid_operate, 4, 1, mstr_temp);
			break;
		
		case 6:
			md_Temp = m_nEpoxyRunSpeed[1];
			
			//mstr_temp = LPCTSTR(_ltoa(md_X, chr_buf, 10));
			mstr_temp.Format("%0.2f",md_Temp);
			st_msg.mstr_keypad_msg = _T("Epoxy Run Speed");
			
			st_msg.mstr_keypad_val = mstr_temp;
			
			mstr_temp = KeyPad.GetNumEditString_D(1, 10000, atof(st_msg.mstr_keypad_val), st_msg.mstr_keypad_msg);
			m_nEpoxyRunSpeed[1] = atof(mstr_temp);
			//int iTemp = atoi(mstr_temp);
			//g_dataMgr.SetDataTemp( &st_basic.n_tray_x, iTemp );
			mstr_temp.Format("%0.2f",m_nEpoxyRunSpeed[1]);
			m_p_grid.GridCellText(m_grid_operate, 6, 1, mstr_temp);
			break;

			////
		case 8:
			md_Temp = m_nEpoxyXYRunSpeed[1][EPOXY_VEL];
			
			//mstr_temp = LPCTSTR(_ltoa(md_X, chr_buf, 10));
			mstr_temp.Format("%0.2f",md_Temp);
			st_msg.mstr_keypad_msg = _T("Epoxy Run Speed_Vel");
			
			st_msg.mstr_keypad_val = mstr_temp;
			
			mstr_temp = KeyPad.GetNumEditString_D(1, 10000, atof(st_msg.mstr_keypad_val), st_msg.mstr_keypad_msg);
			m_nEpoxyXYRunSpeed[1][EPOXY_VEL] = atof(mstr_temp);
			//int iTemp = atoi(mstr_temp);
			//g_dataMgr.SetDataTemp( &st_basic.n_tray_x, iTemp );
			mstr_temp.Format("Vel  :  %0.2f",m_nEpoxyXYRunSpeed[1][EPOXY_VEL]);
			m_p_grid.GridCellText(m_grid_operate, 8, 1, mstr_temp);
			break;

		case 9:
			md_Temp = m_nEpoxyXYRunSpeed[1][EPOXY_ACC];
			
			//mstr_temp = LPCTSTR(_ltoa(md_X, chr_buf, 10));
			mstr_temp.Format("%0.2f",md_Temp);
			st_msg.mstr_keypad_msg = _T("Epoxy Run Speed_Acc");
			
			st_msg.mstr_keypad_val = mstr_temp;
			
			mstr_temp = KeyPad.GetNumEditString_D(1, 10000, atof(st_msg.mstr_keypad_val), st_msg.mstr_keypad_msg);
			m_nEpoxyXYRunSpeed[1][EPOXY_ACC] = atof(mstr_temp);
			//int iTemp = atoi(mstr_temp);
			//g_dataMgr.SetDataTemp( &st_basic.n_tray_x, iTemp );
			mstr_temp.Format("Acc  :  %0.2f",m_nEpoxyXYRunSpeed[1][EPOXY_ACC]);
			m_p_grid.GridCellText(m_grid_operate, 9, 1, mstr_temp);
			break;

		case 10:
			md_Temp = m_nEpoxyXYRunSpeed[1][EPOXY_DEC];
			
			//mstr_temp = LPCTSTR(_ltoa(md_X, chr_buf, 10));
			mstr_temp.Format("%0.2f",md_Temp);
			st_msg.mstr_keypad_msg = _T("Epoxy Run Speed_Dec");
			
			st_msg.mstr_keypad_val = mstr_temp;
			mstr_temp = KeyPad.GetNumEditString_D(1, 10000, atof(st_msg.mstr_keypad_val), st_msg.mstr_keypad_msg);
			m_nEpoxyXYRunSpeed[1][EPOXY_DEC] = atof(mstr_temp);
			mstr_temp.Format("Dec  :  %0.2f",m_nEpoxyXYRunSpeed[1][EPOXY_DEC]);
			m_p_grid.GridCellText(m_grid_operate, 10, 1, mstr_temp);
			break;

			//////
		case 12:
			md_Temp = m_dEpoxyXOffSet[1];
			
			//mstr_temp = LPCTSTR(_ltoa(md_X, chr_buf, 10));
			mstr_temp.Format("%0.2f",md_Temp);
			st_msg.mstr_keypad_msg = _T("Epoxy X Offset");
			
			st_msg.mstr_keypad_val = mstr_temp;
			
			mstr_temp = KeyPad.GetNumEditString_D(1, 10000, atof(st_msg.mstr_keypad_val), st_msg.mstr_keypad_msg);
			m_dEpoxyXOffSet[1] = atof(mstr_temp);
			//int iTemp = atoi(mstr_temp);
			//g_dataMgr.SetDataTemp( &st_basic.n_tray_x, iTemp );
			mstr_temp.Format("%0.2f",m_dEpoxyXOffSet[1]);
			m_p_grid.GridCellText(m_grid_operate, 12, 1, mstr_temp);
			break;

		case 14:
			md_Temp = m_dEpoxyYOffSet[1];
			
			//mstr_temp = LPCTSTR(_ltoa(md_X, chr_buf, 10));
			mstr_temp.Format("%0.2f",md_Temp);
			st_msg.mstr_keypad_msg = _T("Epoxy Y Offset");
			
			st_msg.mstr_keypad_val = mstr_temp;
			
			mstr_temp = KeyPad.GetNumEditString_D(1, 10000, atof(st_msg.mstr_keypad_val), st_msg.mstr_keypad_msg);
			m_dEpoxyYOffSet[1] = atof(mstr_temp);
			//int iTemp = atoi(mstr_temp);
			//g_dataMgr.SetDataTemp( &st_basic.n_tray_x, iTemp );
			mstr_temp.Format("%0.2f",m_dEpoxyYOffSet[1]);
			m_p_grid.GridCellText(m_grid_operate, 14, 1, mstr_temp);
			break;

		case 16:
			md_Temp = m_nRubHSRunSpeed[1];
			
			//mstr_temp = LPCTSTR(_ltoa(md_X, chr_buf, 10));
			mstr_temp.Format("%0.2f",md_Temp);
			st_msg.mstr_keypad_msg = _T("Rub Hs RunSpeed");
			
			st_msg.mstr_keypad_val = mstr_temp;
			
			mstr_temp = KeyPad.GetNumEditString_D(1, 10000, atof(st_msg.mstr_keypad_val), st_msg.mstr_keypad_msg);
			m_nRubHSRunSpeed[1] = atof(mstr_temp);
			//int iTemp = atoi(mstr_temp);
			//g_dataMgr.SetDataTemp( &st_basic.n_tray_x, iTemp );
			mstr_temp.Format("%0.2f",m_nRubHSRunSpeed[1]);
			m_p_grid.GridCellText(m_grid_operate, 16, 1, mstr_temp);
			break;

			////
		case 18:
			md_Temp = m_dTrayPitch_Y[1];
			
			//mstr_temp = LPCTSTR(_ltoa(md_X, chr_buf, 10));
			mstr_temp.Format("%0.2f",md_Temp);
			st_msg.mstr_keypad_msg = _T("Pitch_Y");
			
			st_msg.mstr_keypad_val = mstr_temp;
			
			mstr_temp = KeyPad.GetNumEditString_D(1, 10000, atof(st_msg.mstr_keypad_val), st_msg.mstr_keypad_msg);
			m_dTrayPitch_Y[1] = atof(mstr_temp);
			//int iTemp = atoi(mstr_temp);
			//g_dataMgr.SetDataTemp( &st_basic.n_tray_x, iTemp );
			mstr_temp.Format("Pitch_Y :  %0.2f",m_dTrayPitch_Y[1]);
			m_p_grid.GridCellText(m_grid_operate, 18, 1, mstr_temp);
			break;


		case 19:
			md_Temp = m_dTrayPitch_X[1];
			
			//mstr_temp = LPCTSTR(_ltoa(md_X, chr_buf, 10));
			mstr_temp.Format("%0.2f",md_Temp);
			st_msg.mstr_keypad_msg = _T("Pitch_Y");
			
			st_msg.mstr_keypad_val = mstr_temp;
			
			mstr_temp = KeyPad.GetNumEditString_D(1, 10000, atof(st_msg.mstr_keypad_val), st_msg.mstr_keypad_msg);
			m_dTrayPitch_X[1] = atof(mstr_temp);
			//int iTemp = atoi(mstr_temp);
			//g_dataMgr.SetDataTemp( &st_basic.n_tray_x, iTemp );
			mstr_temp.Format("Pitch_X :  %0.2f",m_dTrayPitch_X[1]);
			m_p_grid.GridCellText(m_grid_operate, 19, 1, mstr_temp);
			break;
			//////

		case 21:
			md_Temp = m_nHsTrayY[1];
			
			//mstr_temp = LPCTSTR(_ltoa(md_X, chr_buf, 10));
			mstr_temp.Format("%d",md_Temp);
			st_msg.mstr_keypad_msg = _T("HsTray_Y");
			
			st_msg.mstr_keypad_val = mstr_temp;
			
			mstr_temp = KeyPad.GetNumEditString_I(1, 100, atof(st_msg.mstr_keypad_val), st_msg.mstr_keypad_msg);
			m_nHsTrayY[1] = atof(mstr_temp);
			//int iTemp = atoi(mstr_temp);
			//g_dataMgr.SetDataTemp( &st_basic.n_tray_x, iTemp );
			mstr_temp.Format("HsTray_Y :  %d",m_nHsTrayY[1]);
			m_p_grid.GridCellText(m_grid_operate, 21, 1, mstr_temp);
			break;
			
			
		case 22:
			md_Temp = m_nHsTrayX[1];
			
			//mstr_temp = LPCTSTR(_ltoa(md_X, chr_buf, 10));
			mstr_temp.Format("%d",md_Temp);
			st_msg.mstr_keypad_msg = _T("HsTray_X");
			
			st_msg.mstr_keypad_val = mstr_temp;
			
			mstr_temp = KeyPad.GetNumEditString_I(1, 100, atof(st_msg.mstr_keypad_val), st_msg.mstr_keypad_msg);
			m_nHsTrayX[1] = atof(mstr_temp);
			//int iTemp = atoi(mstr_temp);
			//g_dataMgr.SetDataTemp( &st_basic.n_tray_x, iTemp );
			mstr_temp.Format("HsTray_X :  %d",m_nHsTrayX[1]);
			m_p_grid.GridCellText(m_grid_operate, 22, 1, mstr_temp);
			break;

		case 24:
			md_Temp = m_nTrayY[1];
			
			//mstr_temp = LPCTSTR(_ltoa(md_X, chr_buf, 10));
			mstr_temp.Format("%d",md_Temp);
			st_msg.mstr_keypad_msg = _T("Tray_Y");
			
			st_msg.mstr_keypad_val = mstr_temp;
			
			mstr_temp = KeyPad.GetNumEditString_I(1, 100, atof(st_msg.mstr_keypad_val), st_msg.mstr_keypad_msg);
			m_nTrayY[1] = atof(mstr_temp);
			//int iTemp = atoi(mstr_temp);
			//g_dataMgr.SetDataTemp( &st_basic.n_tray_x, iTemp );
			mstr_temp.Format("Tray_Y :  %d",m_nTrayY[1]);
			m_p_grid.GridCellText(m_grid_operate, 24, 1, mstr_temp);
			break;
			
			
		case 25:
			md_Temp = m_nTrayX[1];
			
			//mstr_temp = LPCTSTR(_ltoa(md_X, chr_buf, 10));
			mstr_temp.Format("%d",md_Temp);
			st_msg.mstr_keypad_msg = _T("Tray_X");
			
			st_msg.mstr_keypad_val = mstr_temp;
			
			mstr_temp = KeyPad.GetNumEditString_I(1, 100, atof(st_msg.mstr_keypad_val), st_msg.mstr_keypad_msg);
			m_nTrayX[1] = atof(mstr_temp);
			//int iTemp = atoi(mstr_temp);
			//g_dataMgr.SetDataTemp( &st_basic.n_tray_x, iTemp );
			mstr_temp.Format("Tray_X :  %d",m_nTrayX[1]);
			m_p_grid.GridCellText(m_grid_operate, 25, 1, mstr_temp);
			break;
		
		case 27:
			md_Temp = m_nEpoxyDotScrewCount[1];
			
			//mstr_temp = LPCTSTR(_ltoa(md_X, chr_buf, 10));
			mstr_temp.Format("%d",md_Temp);
			st_msg.mstr_keypad_msg = _T("Screw Cnt");
			
			st_msg.mstr_keypad_val = mstr_temp;
			
			mstr_temp = KeyPad.GetNumEditString_I(1, 100, atof(st_msg.mstr_keypad_val), st_msg.mstr_keypad_msg);
			m_nEpoxyDotScrewCount[1] = atof(mstr_temp);
			//int iTemp = atoi(mstr_temp);
			//g_dataMgr.SetDataTemp( &st_basic.n_tray_x, iTemp );
			mstr_temp.Format("%d",m_nEpoxyDotScrewCount[1]);
			m_p_grid.GridCellText(m_grid_operate, 27, 1, mstr_temp);
			break;

		default:
			break;
		}
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

	m_group_DispenserVppmA.SetFont(recipe_font);
	m_group_DispenserVppmA.SetCatptionTextColor(RGB(145,25,0));
	m_group_DispenserVppmA.SetFontBold(TRUE);

	//kwlee 2017.0417
	m_group_Carrier_Move.SetFont(recipe_font);
	m_group_Carrier_Move.SetCatptionTextColor(RGB(145,25,0));
	m_group_Carrier_Move.SetFontBold(TRUE);


	m_group_data_setting.SetFont(recipe_font);
	m_group_data_setting.SetCatptionTextColor(RGB(145,25,0));
	m_group_data_setting.SetFontBold(TRUE);

	m_group_epoxt_limit_usecnt.SetFont(recipe_font);
	m_group_epoxt_limit_usecnt.SetCatptionTextColor(RGB(145,25,0));
	m_group_epoxt_limit_usecnt.SetFontBold(TRUE);

}

void CScreen_Set_Recipe::OnSetRecipe_Digital_Count_Set()
{
	CString strTmp;
	strTmp.Format("%d",m_nfDispenserVppmA[1]);
	m_dgt_DispenserVppmA.SubclassDlgItem(IDC_DGT_DISPENSERVPPMA, this);
	m_dgt_DispenserVppmA.SetStyle(IDB_BIG3, 3);
	m_dgt_DispenserVppmA.SetValue(strTmp);

	//kwlee 2017.0417
	strTmp.Format("%0.2f",dHSCarrierSpreadMoveDistance[1]);
	m_dgt_Offset_Dis.SubclassDlgItem(IDC_DGT_OFFSET_DIS, this);
	m_dgt_Offset_Dis.SetStyle(IDB_BIG3, 6);
	m_dgt_Offset_Dis.SetValue(strTmp);
	
	
	strTmp.Format("%0.2f",dHeatSinkCarrierSpreadMove3Offset[1]);
	m_dgt_Offset_30.SubclassDlgItem(IDC_DGT_OFFSET_30, this);
	m_dgt_Offset_30.SetStyle(IDB_BIG3, 6);
	m_dgt_Offset_30.SetValue(strTmp);
	
	strTmp.Format("%0.2f",dHeatSinkCarrierSpreadMove2Offset[1]);
	m_dgt_Offset_20.SubclassDlgItem(IDC_DGT_OFFSET_20, this);
	m_dgt_Offset_20.SetStyle(IDB_BIG3, 6);
	m_dgt_Offset_20.SetValue(strTmp);
	
	strTmp.Format("%0.2f",dHeatSinkCarrierSpreadMove1Offset[1]);
	m_dgt_Offset_10.SubclassDlgItem(IDC_DGT_OFFSET_10, this);
	m_dgt_Offset_10.SetStyle(IDB_BIG3, 6);
	m_dgt_Offset_10.SetValue(strTmp);
	
	strTmp.Format("%0.2f",dHSCarrierSpreadMoveOffset[1]);
	m_dgt_Offset.SubclassDlgItem(IDC_DGT_OFFSET, this);
	m_dgt_Offset.SetStyle(IDB_BIG3, 6);
	m_dgt_Offset.SetValue(strTmp);

	//kwlee 2017.0416
	strTmp.Format("%0.2f",m_dHeatsinkRubXOffset[1]);
	m_dgt_HsRub_X_Offset.SubclassDlgItem(IDC_DGT_HS_RUB_X_OFFSET, this);
	m_dgt_HsRub_X_Offset.SetStyle(IDB_BIG3, 6);
	m_dgt_HsRub_X_Offset.SetValue(strTmp);
	
	strTmp.Format("%0.2f",m_dHeatsinkRubYOffset[1]);
	m_dgt_HsRub_Y_Offset.SubclassDlgItem(IDC_DGT_HS_RUB_Y_OFFSET, this);
	m_dgt_HsRub_Y_Offset.SetStyle(IDB_BIG3, 6);
	m_dgt_HsRub_Y_Offset.SetValue(strTmp);
	
	strTmp.Format("%d",m_nEpoxyLimitUseCont[1]);
	m_dgt_Epoxy_UseCnt.SubclassDlgItem(IDC_DGT_EPOXY_USECNT, this);
	m_dgt_Epoxy_UseCnt.SetStyle(IDB_BIG3, 3);
	m_dgt_Epoxy_UseCnt.SetValue(strTmp);

}
int CScreen_Set_Recipe::Data_Comparison()
{
	int Ret = RET_GOOD;
	
	//kwlee 2017.0406
	if(m_dLoaderTransferTrayDeviceGap[0]	!= m_dLoaderTransferTrayDeviceGap[1])   return RET_ERROR;
	if(m_dSatbleTime[0]						!= m_dSatbleTime[1])					return RET_ERROR;
	if(m_nEpoxyRunSpeed[0]					!= m_nEpoxyRunSpeed[1])					return RET_ERROR;
	if(m_nEpoxyXYRunSpeed[0][EPOXY_VEL]		!= m_nEpoxyXYRunSpeed[1][EPOXY_VEL])	return RET_ERROR;
	if(m_nEpoxyXYRunSpeed[0][EPOXY_ACC]		!= m_nEpoxyXYRunSpeed[1][EPOXY_ACC])	return RET_ERROR;
	if(m_nEpoxyXYRunSpeed[0][EPOXY_DEC]		!= m_nEpoxyXYRunSpeed[1][EPOXY_DEC])	return RET_ERROR;
	if(m_dEpoxyXOffSet[0]					!= m_dEpoxyXOffSet[1])					return RET_ERROR;
	if(m_dEpoxyYOffSet[0]					!= m_dEpoxyYOffSet[1])					return RET_ERROR;
	if(m_nRubHSRunSpeed[0]					!= m_nRubHSRunSpeed[1])					return RET_ERROR;
	
	if(m_dTrayPitch_Y[0]					!= m_dTrayPitch_Y[1])					return RET_ERROR;
	if(m_dTrayPitch_X[0]					!= m_dTrayPitch_X[1])					return RET_ERROR;
	if(m_nfDispenserVppmA[0]				!= m_nfDispenserVppmA[1])				return RET_ERROR;
	if(m_nEpoxyDotScrewCount[0]				!= m_nEpoxyDotScrewCount[1])			return RET_ERROR;
	if(m_nHsTrayY[0]						!= m_nHsTrayY[1])						return RET_ERROR;
	if(m_nHsTrayX[0]						!= m_nHsTrayX[1])						return RET_ERROR;
	if(m_nTrayY[0]							!= m_nTrayY[1])							return RET_ERROR;
	if(m_nTrayX[0]							!= m_nTrayX[1])							return RET_ERROR;
	//kwlee 2017.0417
	if(dHSCarrierSpreadMoveOffset[0]		!= dHSCarrierSpreadMoveOffset[1])						return RET_ERROR;
	if(dHSCarrierSpreadMoveDistance[0]			!= dHSCarrierSpreadMoveDistance[1])					return RET_ERROR;
	if(dHeatSinkCarrierSpreadMove1Offset[0]		!= dHeatSinkCarrierSpreadMove1Offset[1])			return RET_ERROR;
	if(dHeatSinkCarrierSpreadMove2Offset[0]		!= dHeatSinkCarrierSpreadMove2Offset[1])			return RET_ERROR;
	if(dHeatSinkCarrierSpreadMove3Offset[0]		!= dHeatSinkCarrierSpreadMove3Offset[1])			return RET_ERROR;
	
	if(m_dHeatsinkRubXOffset[0]		!= m_dHeatsinkRubXOffset[1])			return RET_ERROR;
	if(m_dHeatsinkRubYOffset[0]		!= m_dHeatsinkRubYOffset[1])			return RET_ERROR;
	if(m_nEpoxyLimitUseCont[0]		!= m_nEpoxyLimitUseCont[1])			return RET_ERROR;
	return Ret;
}
void CScreen_Set_Recipe::OnSetRecipe_Data_Set()
{
	m_dLoaderTransferTrayDeviceGap[1]             =  st_recipe.dLoaderTransferTrayDeviceGap;
	m_dSatbleTime[1]							  =  st_recipe.dSatbleTime;
	m_nEpoxyRunSpeed[1]							  =  st_recipe.nEpoxyRunSpeed;
	m_nEpoxyXYRunSpeed[1][EPOXY_VEL]               =  st_recipe.nEpoxyXYRunSpeed[EPOXY_VEL];
	m_nEpoxyXYRunSpeed[1][EPOXY_ACC]               =  st_recipe.nEpoxyXYRunSpeed[EPOXY_ACC];
	m_nEpoxyXYRunSpeed[1][EPOXY_DEC]               =  st_recipe.nEpoxyXYRunSpeed[EPOXY_DEC];
	m_dEpoxyXOffSet[1]                            =  st_recipe.dEpoxyXOffSet;
	m_dEpoxyYOffSet[1]                            =  st_recipe.dEpoxyYOffSet;
	m_nRubHSRunSpeed[1]                           = st_recipe.nRubHSRunSpeed;
	m_dTrayPitch_Y[1]			                  = st_recipe.dTrayPitch_Y;
	m_dTrayPitch_X[1]			                  = st_recipe.dTrayPitch_X;

	m_nfDispenserVppmA[1]                         = st_recipe.fDispenserVppmA;
	m_nEpoxyDotScrewCount[1]                      = st_recipe.nEpoxyDotScrewCount; 
	m_nHsTrayY[1]						          = st_recipe.nHsTrayY;
	m_nHsTrayX[1]						          = st_recipe.nHsTrayX;
	m_nTrayY[1]						              = st_recipe.nTrayY;
    m_nTrayX[1]                                   = st_recipe.nTrayX;

	//kwlee 2017.0417
	dHSCarrierSpreadMoveOffset[1]					= 	st_recipe.dHSCarrierSpreadMoveOffset;			
	dHSCarrierSpreadMoveDistance[1]					= 	st_recipe.dHSCarrierSpreadMoveDistance;		
	dHeatSinkCarrierSpreadMove1Offset[1]			= 	st_recipe.dHeatSinkCarrierSpreadMove1Offset;	
	dHeatSinkCarrierSpreadMove2Offset[1]			= 	st_recipe.dHeatSinkCarrierSpreadMove2Offset;	
	dHeatSinkCarrierSpreadMove3Offset[1]			= 	st_recipe.dHeatSinkCarrierSpreadMove3Offset;	

	m_dHeatsinkRubXOffset[1]			= 	st_recipe.dHeatsinkRubXOffset;	
	m_dHeatsinkRubYOffset[1]			= 	st_recipe.dHeatsinkRubYOffset;	
	m_nEpoxyLimitUseCont[1]			= 	st_recipe.nEpoxyUseLimitCont;	
}
void CScreen_Set_Recipe::OnInitGrid()
{

	int		i, j;
	int		max_row, max_col;
	CString str_tmp;
	
	
	
	//max_row = m_n_operate_cnt[0] + m_n_operate_cnt[1] + m_n_operate_cnt[2];
	//kwlee 2017.0404
	max_row = 27;
	max_col = 1;
	
	m_grid_operate = (TSpread*)GetDlgItem(IDC_CUSTOM_DATA);
	
	m_p_grid.GridReset(m_grid_operate);
	
	// 대문자 
	m_p_grid.GridCellSelectDisplay(m_grid_operate, TRUE);
	m_p_grid.GridRowHeader(m_grid_operate, FALSE);
	m_p_grid.GridColHeader(m_grid_operate, FALSE);
	m_p_grid.GridHorizontal(m_grid_operate, FALSE);
	m_p_grid.GridVertical(m_grid_operate, FALSE);
	m_p_grid.GridAutoSize(m_grid_operate, FALSE);
	
	m_p_grid.GridCellRows(m_grid_operate, max_row);
	m_p_grid.GridCellCols(m_grid_operate, max_col);
	
	
	//kwlee 2017.0404
	// 	width_p		= area_p.Width() - 15;
	// 	height_p	= area_p.Height() - 10;
	// 	
	// 	x_p			= (width_p / max_col);
	// 	y_p			= (height_p / max_row);
	// 	
	// 	// group box에서 grid 센터에 표시하기 위한 x좌표 구하기 
	// 	cen_x_p = area_p.left + (((area_p.Width() - 15) - (x_p * max_col)) / 2) + 8;
	// 	// group box에서 grid 센터에 표시하기 위한 y좌표 구하기 
	// 	cen_y_p = area_p.top + (((area_p.Height() - 30) - (y_p * max_row)) / 2) + 20;
	
	// grid size 계산하여 중심에 다시 그리기 
	//	m_grid_operate->MoveWindow(cen_x_p, cen_y_p, x_p*(max_col), y_p*(max_row), TRUE); //kwlee 2017.0404
	
	
	for(i=0; i<max_row + 1; i++)
	{
		//m_p_grid.GridCellHeight(m_grid_operate, i, y_p);
		//kwlee 2017.0404
		m_p_grid.GridCellHeight(m_grid_operate, i, 18);
		
		for(j=0; j<max_col+1; j++)
		{	
			//m_p_grid.GridCellWidth(m_grid_operate, j, x_p);
			//kwlee 2017.0404
			m_p_grid.GridCellWidth(m_grid_operate, j, 21);
			m_p_grid.GridCellControlStatic(m_grid_operate, i, j);
		}
	}

	m_p_grid.GridCellFont(m_grid_operate, 1, 1, "MS Sans Serif", 18);
	m_p_grid.GridCellColor(m_grid_operate, 1, 1, BLACK_L, YELLOW_C);
	m_p_grid.GridCellText(m_grid_operate, 1, 1, "UnPressPos거리");
	
	str_tmp.Format("%0.2f",m_dLoaderTransferTrayDeviceGap[1]);
	m_p_grid.GridCellColor(m_grid_operate, 2, 1, GREEN_C, BLACK_C);
	m_p_grid.GridCellFont(m_grid_operate, 2, 1, "MS Sans Serif", 12);
	m_p_grid.GridCellText(m_grid_operate, 2, 1, str_tmp);
	
	
	m_p_grid.GridCellFont(m_grid_operate, 3, 1, "MS Sans Serif", 18);
	m_p_grid.GridCellColor(m_grid_operate, 3, 1, BLACK_L, YELLOW_C);
	m_p_grid.GridCellText(m_grid_operate, 3, 1, "SatbleTime");
	
	str_tmp.Format("%0.2f",(double)m_dSatbleTime[1]);
	m_p_grid.GridCellColor(m_grid_operate, 4, 1, GREEN_C, BLACK_C);
	m_p_grid.GridCellFont(m_grid_operate, 4, 1, "MS Sans Serif", 12);
	m_p_grid.GridCellText(m_grid_operate, 4, 1, str_tmp);
	
	
	m_p_grid.GridCellFont(m_grid_operate, 5, 1, "MS Sans Serif", 18);
	m_p_grid.GridCellColor(m_grid_operate, 5, 1, BLACK_L, YELLOW_C);
	m_p_grid.GridCellText(m_grid_operate, 5, 1, "EpoxyRunSpeed");
	
	str_tmp.Format("%0.2f",m_nEpoxyRunSpeed[1]);
	m_p_grid.GridCellColor(m_grid_operate, 6, 1, GREEN_C, BLACK_C);
	m_p_grid.GridCellFont(m_grid_operate, 6, 1, "MS Sans Serif", 12);
	m_p_grid.GridCellText(m_grid_operate, 6, 1, str_tmp);
	
	
	m_p_grid.GridCellFont(m_grid_operate, 7, 1, "MS Sans Serif", 18);
	m_p_grid.GridCellColor(m_grid_operate, 7, 1, BLACK_L, YELLOW_C);
	m_p_grid.GridCellText(m_grid_operate, 7, 1, "EpoxyRunSpeed");
	
	str_tmp.Format("Vel  :   %0.2f",m_nEpoxyXYRunSpeed[1][EPOXY_VEL]);
	m_p_grid.GridCellColor(m_grid_operate, 8, 1, GREEN_C, BLACK_C);
	m_p_grid.GridCellFont(m_grid_operate, 8, 1, "MS Sans Serif", 12);
	m_p_grid.GridCellText(m_grid_operate, 8, 1, str_tmp);
	
	
	// 	m_p_grid.GridCellFont(m_grid_operate, 18, 1, "MS Sans Serif", 18);
	// 	m_p_grid.GridCellColor(m_grid_operate, 18, 1, BLACK_L, YELLOW_C);
	// 	m_p_grid.GridCellText(m_grid_operate, 18, 1, "EpoxyRunSpeed_Acc");
	
	str_tmp.Format("Acc  :   %0.2f",m_nEpoxyXYRunSpeed[1][EPOXY_ACC]);
	m_p_grid.GridCellColor(m_grid_operate, 9,1, GREEN_C, BLACK_C);
	m_p_grid.GridCellFont(m_grid_operate, 9 ,1, "MS Sans Serif", 12);
	m_p_grid.GridCellText(m_grid_operate, 9 ,1, str_tmp);
	
	// 	m_p_grid.GridCellFont(m_grid_operate, 20, 1, "MS Sans Serif", 18);
	// 	m_p_grid.GridCellColor(m_grid_operate, 20, 1, BLACK_L, YELLOW_C);
	// 	m_p_grid.GridCellText(m_grid_operate, 20 ,1, "EpoxyRunSpeed_Dec");
	
	str_tmp.Format("Dec   :   %0.2f",m_nEpoxyXYRunSpeed[1][EPOXY_DEC]);
	m_p_grid.GridCellColor(m_grid_operate, 10,1, GREEN_C, BLACK_C);
	m_p_grid.GridCellFont(m_grid_operate, 10,1, "MS Sans Serif", 12);
	m_p_grid.GridCellText(m_grid_operate, 10,1, str_tmp);
	
	
	m_p_grid.GridCellFont(m_grid_operate, 11, 1, "MS Sans Serif", 18);
	m_p_grid.GridCellColor(m_grid_operate, 11, 1, BLACK_L, YELLOW_C);
	m_p_grid.GridCellText(m_grid_operate, 11, 1, "EpoxyXOffSet");
	
	str_tmp.Format("%0.2f",m_dEpoxyXOffSet[1]);
	m_p_grid.GridCellColor(m_grid_operate, 12, 1, GREEN_C, BLACK_C);
	m_p_grid.GridCellFont(m_grid_operate, 12, 1, "MS Sans Serif", 12);
	m_p_grid.GridCellText(m_grid_operate, 12, 1, str_tmp);
	
	m_p_grid.GridCellFont(m_grid_operate, 13, 1, "MS Sans Serif", 18);
	m_p_grid.GridCellColor(m_grid_operate, 13, 1, BLACK_L, YELLOW_C);
	m_p_grid.GridCellText(m_grid_operate, 13, 1, "EpoxyYOffSet");
	
	str_tmp.Format("%0.2f",m_dEpoxyYOffSet[1]);
	m_p_grid.GridCellColor(m_grid_operate, 14, 1, GREEN_C, BLACK_C);
	m_p_grid.GridCellFont(m_grid_operate, 14, 1, "MS Sans Serif", 12);
	m_p_grid.GridCellText(m_grid_operate, 14, 1, str_tmp);
	
	m_p_grid.GridCellFont(m_grid_operate, 15, 1, "MS Sans Serif", 18);
	m_p_grid.GridCellColor(m_grid_operate, 15, 1, BLACK_L, YELLOW_C);
	m_p_grid.GridCellText(m_grid_operate, 15, 1, "RubHSRunSpeed");
	
	str_tmp.Format("%0.2f",m_nRubHSRunSpeed[1]);
	m_p_grid.GridCellColor(m_grid_operate, 16, 1, GREEN_C, BLACK_C);
	m_p_grid.GridCellFont(m_grid_operate, 16, 1, "MS Sans Serif", 12);
	m_p_grid.GridCellText(m_grid_operate, 16, 1, str_tmp);
	
	m_p_grid.GridCellFont(m_grid_operate, 17, 1, "MS Sans Serif", 18);
	m_p_grid.GridCellColor(m_grid_operate, 17, 1, BLACK_L, YELLOW_C);
	m_p_grid.GridCellText(m_grid_operate, 17, 1, "TrayPitch");
	
	str_tmp.Format("Pitch_Y : %0.2f",m_dTrayPitch_Y[1]);
	m_p_grid.GridCellColor(m_grid_operate, 18, 1, GREEN_C, BLACK_C);
	m_p_grid.GridCellFont(m_grid_operate, 18, 1, "MS Sans Serif", 12);
	m_p_grid.GridCellText(m_grid_operate, 18, 1, str_tmp);
	
	str_tmp.Format("Pitch_X : %0.2f",m_dTrayPitch_X[1]);
	m_p_grid.GridCellColor(m_grid_operate, 19, 1, GREEN_C, BLACK_C);
	m_p_grid.GridCellFont(m_grid_operate, 19, 1, "MS Sans Serif", 12);
	m_p_grid.GridCellText(m_grid_operate, 19, 1, str_tmp);

	//kwlee 2017.0410
	m_p_grid.GridCellFont(m_grid_operate, 20, 1, "MS Sans Serif", 18);
	m_p_grid.GridCellColor(m_grid_operate, 20, 1, BLACK_L, YELLOW_C);
	m_p_grid.GridCellText(m_grid_operate, 20, 1, "HsTray");
	
	str_tmp.Format("HsTray_Y : %d",m_nHsTrayY[1]);
	m_p_grid.GridCellColor(m_grid_operate, 21, 1, GREEN_C, BLACK_C);
	m_p_grid.GridCellFont(m_grid_operate, 21, 1, "MS Sans Serif", 12);
	m_p_grid.GridCellText(m_grid_operate, 21, 1, str_tmp);
	
	str_tmp.Format("HsTray_X : %d",m_nHsTrayX[1]);
	m_p_grid.GridCellColor(m_grid_operate, 22, 1, GREEN_C, BLACK_C);
	m_p_grid.GridCellFont(m_grid_operate, 22, 1, "MS Sans Serif", 12);
	m_p_grid.GridCellText(m_grid_operate, 22, 1, str_tmp);

	m_p_grid.GridCellFont(m_grid_operate, 23, 1, "MS Sans Serif", 18);
	m_p_grid.GridCellColor(m_grid_operate, 23, 1, BLACK_L, YELLOW_C);
	m_p_grid.GridCellText(m_grid_operate, 23, 1, "Tray Cnt");
	
	str_tmp.Format("Tray_Y : %d",m_nTrayY[1]);
	m_p_grid.GridCellColor(m_grid_operate, 24, 1, GREEN_C, BLACK_C);
	m_p_grid.GridCellFont(m_grid_operate, 24, 1, "MS Sans Serif", 12);
	m_p_grid.GridCellText(m_grid_operate, 24, 1, str_tmp);
	
	str_tmp.Format("Tray_X : %d",m_nTrayX[1]);
	m_p_grid.GridCellColor(m_grid_operate, 25, 1, GREEN_C, BLACK_C);
	m_p_grid.GridCellFont(m_grid_operate, 25, 1, "MS Sans Serif", 12);
	m_p_grid.GridCellText(m_grid_operate, 25, 1, str_tmp);

	m_p_grid.GridCellFont(m_grid_operate, 26, 1, "MS Sans Serif", 18);
	m_p_grid.GridCellColor(m_grid_operate, 26, 1, BLACK_L, YELLOW_C);
	m_p_grid.GridCellText(m_grid_operate, 26, 1, "Screw Cnt");
	
	str_tmp.Format("%d",m_nEpoxyDotScrewCount[1]);
	m_p_grid.GridCellColor(m_grid_operate, 27, 1, GREEN_C, BLACK_C);
	m_p_grid.GridCellFont(m_grid_operate, 27, 1, "MS Sans Serif", 12);
	m_p_grid.GridCellText(m_grid_operate, 27, 1, str_tmp);
	
}

//kwlee 2017.0416
void CScreen_Set_Recipe::OnDgtOffset() 
{
	// TODO: Add your control notification handler code here
	CString mstr_temp;  // 저장할 정보 임시 저장 변수 
	char chr_buf[20] ;
	double mdTemp;

	
	mdTemp = m_dgt_Offset.GetValue();
	mstr_temp = LPCTSTR(_ltoa(mdTemp, chr_buf, 10));
	
	st_msg.mstr_keypad_msg = _T("Carrier Move Offset");
    
	st_msg.mstr_keypad_val = mstr_temp;
	
	mstr_temp = KeyPad.GetNumEditString_D(1, 10000, atof(st_msg.mstr_keypad_val), st_msg.mstr_keypad_msg);
	dHSCarrierSpreadMoveOffset[1] = atof(mstr_temp);
	//int iTemp = atoi(mstr_temp);
	//g_dataMgr.SetDataTemp( &st_basic.n_tray_x, iTemp );
	mstr_temp.Format("%0.2f",dHSCarrierSpreadMoveOffset[1]);
	m_dgt_Offset.SetValue( mstr_temp);
}

void CScreen_Set_Recipe::OnDgtOffset10() 
{
	// TODO: Add your control notification handler code here
	CString mstr_temp;  // 저장할 정보 임시 저장 변수 
	char chr_buf[20] ;
	double mdTemp;

	
	mdTemp = m_dgt_Offset_10.GetValue();
	mstr_temp = LPCTSTR(_ltoa(mdTemp, chr_buf, 10));
	
	st_msg.mstr_keypad_msg = _T("Carrier Offset 10");
    
	st_msg.mstr_keypad_val = mstr_temp;
	
	mstr_temp = KeyPad.GetNumEditString_D(-5, 5, atof(st_msg.mstr_keypad_val), st_msg.mstr_keypad_msg);
	dHeatSinkCarrierSpreadMove1Offset[1] = atof(mstr_temp);
	//int iTemp = atoi(mstr_temp);
	//g_dataMgr.SetDataTemp( &st_basic.n_tray_x, iTemp );
	mstr_temp.Format("%0.2f",dHeatSinkCarrierSpreadMove1Offset[1]);
	m_dgt_Offset_10.SetValue( mstr_temp);
	
}

void CScreen_Set_Recipe::OnDgtOffset20() 
{
	// TODO: Add your control notification handler code here
	CString mstr_temp;  // 저장할 정보 임시 저장 변수 
	char chr_buf[20] ;
	double mdTemp;
	
	
	mdTemp = m_dgt_Offset_20.GetValue();
	mstr_temp = LPCTSTR(_ltoa(mdTemp, chr_buf, 10));
	
	st_msg.mstr_keypad_msg = _T("Carrier Offset 10");
    
	st_msg.mstr_keypad_val = mstr_temp;
	
	mstr_temp = KeyPad.GetNumEditString_D(-5, 5, atof(st_msg.mstr_keypad_val), st_msg.mstr_keypad_msg);
	dHeatSinkCarrierSpreadMove2Offset[1] = atof(mstr_temp);
	//int iTemp = atoi(mstr_temp);
	//g_dataMgr.SetDataTemp( &st_basic.n_tray_x, iTemp );
	mstr_temp.Format("%0.2f",dHeatSinkCarrierSpreadMove2Offset[1]);
	m_dgt_Offset_20.SetValue( mstr_temp);
}

void CScreen_Set_Recipe::OnDgtOffset30() 
{
	// TODO: Add your control notification handler code here
	CString mstr_temp;  // 저장할 정보 임시 저장 변수 
	char chr_buf[20] ;
	double mdTemp;

	
	mdTemp = m_dgt_Offset_30.GetValue();
	mstr_temp = LPCTSTR(_ltoa(mdTemp, chr_buf, 10));
	
	st_msg.mstr_keypad_msg = _T("Carrier Offset 10");
    
	st_msg.mstr_keypad_val = mstr_temp;
	
	mstr_temp = KeyPad.GetNumEditString_D(1, 10000, atof(st_msg.mstr_keypad_val), st_msg.mstr_keypad_msg);
	dHeatSinkCarrierSpreadMove3Offset[1] = atof(mstr_temp);
	//int iTemp = atoi(mstr_temp);
	//g_dataMgr.SetDataTemp( &st_basic.n_tray_x, iTemp );
	mstr_temp.Format("%0.2f",dHeatSinkCarrierSpreadMove2Offset[1]);
	m_dgt_Offset_30.SetValue( mstr_temp);
}

void CScreen_Set_Recipe::OnDgtOffsetDis() 
{
	// TODO: Add your control notification handler code here
	CString mstr_temp;  // 저장할 정보 임시 저장 변수 
	char chr_buf[20] ;
	double mdTemp;

	
	mdTemp = m_dgt_Offset_Dis.GetValue();
	mstr_temp = LPCTSTR(_ltoa(mdTemp, chr_buf, 10));
	
	st_msg.mstr_keypad_msg = _T("Carrier Offset 10");
    
	st_msg.mstr_keypad_val = mstr_temp;
	
	mstr_temp = KeyPad.GetNumEditString_D(1, 10000, atof(st_msg.mstr_keypad_val), st_msg.mstr_keypad_msg);
	dHSCarrierSpreadMoveDistance[1] = atof(mstr_temp);
	//int iTemp = atoi(mstr_temp);
	//g_dataMgr.SetDataTemp( &st_basic.n_tray_x, iTemp );
	mstr_temp.Format("%0.2f",dHSCarrierSpreadMoveDistance[1]);
	m_dgt_Offset_Dis.SetValue( mstr_temp);
}
/////
void CScreen_Set_Recipe::OnSetRecipe_Data_BackUp()
{

	m_dLoaderTransferTrayDeviceGap[0]  = m_dLoaderTransferTrayDeviceGap[1];  
	m_dSatbleTime[0]				   = m_dSatbleTime[1];					
	m_nEpoxyRunSpeed[0]				   = m_nEpoxyRunSpeed[1];					
	m_nEpoxyXYRunSpeed[0][EPOXY_VEL]   = m_nEpoxyXYRunSpeed[1][EPOXY_VEL];
	m_nEpoxyXYRunSpeed[0][EPOXY_ACC]   = m_nEpoxyXYRunSpeed[1][EPOXY_ACC];
	m_nEpoxyXYRunSpeed[0][EPOXY_DEC]   = m_nEpoxyXYRunSpeed[1][EPOXY_DEC];
	m_dEpoxyXOffSet[0]                 = m_dEpoxyXOffSet[1];                 
	m_dEpoxyYOffSet[0]                 = m_dEpoxyYOffSet[1];                 
	m_nRubHSRunSpeed[0]                = m_nRubHSRunSpeed[1];                
	m_dTrayPitch_Y[0]			       = m_dTrayPitch_Y[1];			       
	m_dTrayPitch_X[0]			       = m_dTrayPitch_X[1];			       
	m_nfDispenserVppmA[0]              =  m_nfDispenserVppmA[1];              
	m_nEpoxyDotScrewCount[0]           =  m_nEpoxyDotScrewCount[1];           
	m_nHsTrayY[0]						= m_nHsTrayY[1];						
	m_nHsTrayX[0]						= m_nHsTrayX[1];						
	m_nTrayY[0]						   =  m_nTrayY[1];						   
	m_nTrayX[0]                        =  m_nTrayX[1];  
	//kwlee 2017.0417
	dHSCarrierSpreadMoveOffset[0]			= dHSCarrierSpreadMoveOffset[1];
	dHSCarrierSpreadMoveDistance[0]			= dHSCarrierSpreadMoveDistance[1];
	dHeatSinkCarrierSpreadMove1Offset[0]	= dHeatSinkCarrierSpreadMove1Offset[1];
	dHeatSinkCarrierSpreadMove2Offset[0]	= dHeatSinkCarrierSpreadMove2Offset[1];
	dHeatSinkCarrierSpreadMove3Offset[0]	= dHeatSinkCarrierSpreadMove3Offset[1];

	m_dHeatsinkRubXOffset[0]	= m_dHeatsinkRubXOffset[1];
	m_dHeatsinkRubYOffset[0]	= m_dHeatsinkRubYOffset[1];
	m_nEpoxyLimitUseCont[0]	= m_nEpoxyLimitUseCont[1];
}

void CScreen_Set_Recipe::OnSetRecipe_Data_Change()
{
}

void CScreen_Set_Recipe::OnSetRecipe_LogFile_Create()
{
	CString str_msg;

	if(m_dLoaderTransferTrayDeviceGap[0]	!=  m_dLoaderTransferTrayDeviceGap[1])
	{
		str_msg.Format( "[RECIPE] LoaderTransferTrayDeviceGap Change %0.2f -> 0.2f", m_dLoaderTransferTrayDeviceGap[0], m_dLoaderTransferTrayDeviceGap[1]);
		Func.On_LogFile_Add(0, str_msg);
	}
	
	if(m_dSatbleTime[0]	!=  m_dSatbleTime[1])
	{
		str_msg.Format( "[RECIPE] SatbleTime Change %0.2f -> 0.2f", m_dSatbleTime[0], m_dSatbleTime[1]);
		Func.On_LogFile_Add(0, str_msg);
	}
	
	if(m_nEpoxyRunSpeed[0]	!=  m_nEpoxyRunSpeed[1])
	{
		str_msg.Format( "[RECIPE] EpoxyRunSpeed Change %d -> %d", m_nEpoxyRunSpeed[0], m_nEpoxyRunSpeed[1]);
		Func.On_LogFile_Add(0, str_msg);
	}
	
	if(m_nEpoxyXYRunSpeed[0][EPOXY_VEL]	!=  m_nEpoxyXYRunSpeed[1][EPOXY_VEL])
	{
		str_msg.Format( "[RECIPE] EpoxyXYRunSpeed_vel Change %d -> %d", m_nEpoxyXYRunSpeed[0][EPOXY_VEL], m_nEpoxyXYRunSpeed[1][EPOXY_VEL]);
		Func.On_LogFile_Add(0, str_msg);
	}

	if(m_nEpoxyXYRunSpeed[0][EPOXY_ACC]	!=  m_nEpoxyXYRunSpeed[1][EPOXY_ACC])
	{
		str_msg.Format( "[RECIPE] EpoxyXYRunSpeed_Acc Change %d -> %d", m_nEpoxyXYRunSpeed[0][EPOXY_ACC], m_nEpoxyXYRunSpeed[1][EPOXY_ACC]);
		Func.On_LogFile_Add(0, str_msg);
	}
	
	if(m_nEpoxyXYRunSpeed[0][EPOXY_DEC]	!=  m_nEpoxyXYRunSpeed[1][EPOXY_DEC])
	{
		str_msg.Format( "[RECIPE] EpoxyXYRunSpeed_Dec Change %d -> %d", m_nEpoxyXYRunSpeed[0][EPOXY_DEC], m_nEpoxyXYRunSpeed[1][EPOXY_DEC]);
		Func.On_LogFile_Add(0, str_msg);
	}
	
	if(m_dEpoxyXOffSet[0]	!=  m_dEpoxyXOffSet[1])
	{
		str_msg.Format( "[RECIPE] EpoxyXOffSet Change %0.2f -> 0.2f", m_dEpoxyXOffSet[0], m_dEpoxyXOffSet[1]);
		Func.On_LogFile_Add(0, str_msg);
	}
	
	if(m_dEpoxyYOffSet[0]	!=  m_dEpoxyYOffSet[1])
	{
		str_msg.Format( "[RECIPE] EpoxyYOffSet Change %0.2f -> 0.2f", m_dEpoxyYOffSet[0], m_dEpoxyYOffSet[1]);
		Func.On_LogFile_Add(0, str_msg);
	}
	
	if(m_nRubHSRunSpeed[0]	!=  m_nRubHSRunSpeed[1])
	{
		str_msg.Format( "[RECIPE] RubHSRunSpeed Change %d -> %d", m_nRubHSRunSpeed[0], m_nRubHSRunSpeed[1]);
		Func.On_LogFile_Add(0, str_msg);
	}

	if(m_dTrayPitch_Y[0]	!=  m_dTrayPitch_Y[1])
	{
		str_msg.Format( "[RECIPE] TrayPitch_YLoaderTransferTrayDeviceGap Change %0.2f -> 0.2f", m_dTrayPitch_Y[0], m_dTrayPitch_Y[1]);
		Func.On_LogFile_Add(0, str_msg);
	}
	
	if(m_dTrayPitch_X[0]	!=  m_dTrayPitch_X[1])
	{
		str_msg.Format( "[RECIPE] TrayPitch_X Change %0.2f -> 0.2f", m_dTrayPitch_X[0], m_dTrayPitch_X[1]);
		Func.On_LogFile_Add(0, str_msg);
	}
	
	if(m_nfDispenserVppmA[0]	!=  m_nfDispenserVppmA[1])
	{
		str_msg.Format( "[RECIPE] fDispenserVppmA Change %d -> %d", m_nfDispenserVppmA[0], m_nfDispenserVppmA[1]);
		Func.On_LogFile_Add(0, str_msg);
	}
	
	if(m_nEpoxyDotScrewCount[0]	!=  m_nEpoxyDotScrewCount[1])
	{
		str_msg.Format( "[RECIPE] EpoxyDotScrewCount Change %d -> %d", m_nEpoxyDotScrewCount[0], m_nEpoxyDotScrewCount[1]);
		Func.On_LogFile_Add(0, str_msg);
	}

	if(m_nHsTrayY[0]	!=  m_nHsTrayY[1])
	{
		str_msg.Format( "[RECIPE] HsTrayY Change %d -> %d", m_nHsTrayY[0], m_nHsTrayY[1]);
		Func.On_LogFile_Add(0, str_msg);
	}
	
	if(m_nHsTrayX[0]	!=  m_nHsTrayX[1])
	{
		str_msg.Format( "[RECIPE] HsTrayX Change %d -> %d", m_nHsTrayX[0], m_nHsTrayX[1]);
		Func.On_LogFile_Add(0, str_msg);
	}
	
	if(m_nTrayY[0]	!=  m_nTrayY[1])
	{
		str_msg.Format( "[RECIPE] TrayY Change %d -> %d", m_nTrayY[0], m_nTrayY[1]);
		Func.On_LogFile_Add(0, str_msg);
	}
	
	if(m_nTrayX[0]	!=  m_nTrayX[1])
	{
		str_msg.Format( "[RECIPE] TrayX Change %d -> %d", m_nTrayX[0], m_nTrayX[1]);
		Func.On_LogFile_Add(0, str_msg);
	}

	//kwlee 2017.0417
	if(dHSCarrierSpreadMoveOffset[0] != dHSCarrierSpreadMoveOffset[1])
	{
		str_msg.Format( "[RECIPE] HSCarrierSpreadMoveOffset Change %0.2f -> 0.2f", dHSCarrierSpreadMoveOffset[0], dHSCarrierSpreadMoveOffset[1]);
		Func.On_LogFile_Add(0, str_msg);
	}
	if(dHSCarrierSpreadMoveDistance[0] != dHSCarrierSpreadMoveDistance[1])
	{
		str_msg.Format( "[RECIPE] HSCarrierSpreadMoveDistance Change %0.2f -> 0.2f", dHSCarrierSpreadMoveDistance[0], dHSCarrierSpreadMoveDistance[1]);
		Func.On_LogFile_Add(0, str_msg);
	}
	
	if(dHeatSinkCarrierSpreadMove1Offset[0] != dHeatSinkCarrierSpreadMove1Offset[1])
	{
		str_msg.Format( "[RECIPE] HeatSinkCarrierSpreadMove1Offset Change %0.2f -> 0.2f", dHeatSinkCarrierSpreadMove1Offset[0], dHeatSinkCarrierSpreadMove1Offset[1]);
		Func.On_LogFile_Add(0, str_msg);
	}
	
	if(dHeatSinkCarrierSpreadMove2Offset[0] != dHeatSinkCarrierSpreadMove2Offset[1])
	{
		str_msg.Format( "[RECIPE] HeatSinkCarrierSpreadMove2Offset Change%0.2f -> 0.2f", dHeatSinkCarrierSpreadMove2Offset[0], dHeatSinkCarrierSpreadMove2Offset[1]);
		Func.On_LogFile_Add(0, str_msg);
	}
	
	if(dHeatSinkCarrierSpreadMove3Offset[0] != dHeatSinkCarrierSpreadMove3Offset[1])
	{
		str_msg.Format( "[RECIPE] HeatSinkCarrierSpreadMove3Offset Change %0.2f -> 0.2f", dHeatSinkCarrierSpreadMove3Offset[0], dHeatSinkCarrierSpreadMove3Offset[1]);
		Func.On_LogFile_Add(0, str_msg);
	}

	
	
	if(m_dHeatsinkRubXOffset[0] != m_dHeatsinkRubXOffset[1])
	{
		str_msg.Format( "[RECIPE] m_dHeatsinkRubXOffset Change %0.2f -> 0.2f", m_dHeatsinkRubXOffset[0], m_dHeatsinkRubXOffset[1]);
		Func.On_LogFile_Add(0, str_msg);
	}
	
	if(m_dHeatsinkRubYOffset[0] != m_dHeatsinkRubYOffset[1])
	{
		str_msg.Format( "[RECIPE] m_dHeatsinkRubYOffset Change%0.2f -> 0.2f", m_dHeatsinkRubYOffset[0], m_dHeatsinkRubYOffset[1]);
		Func.On_LogFile_Add(0, str_msg);
	}
	
	if(m_nEpoxyLimitUseCont[0] != m_nEpoxyLimitUseCont[1])
	{
		str_msg.Format( "[RECIPE] m_nEpoxyUseLimitCont Change %0.2f -> 0.2f", m_nEpoxyLimitUseCont[0], m_nEpoxyLimitUseCont[1]);
		Func.On_LogFile_Add(0, str_msg);
	}
}

void CScreen_Set_Recipe::OnSetRecipe_Data_Apply()
{
	st_recipe.dLoaderTransferTrayDeviceGap   = m_dLoaderTransferTrayDeviceGap[1];
	st_recipe.dSatbleTime                    = m_dSatbleTime[1];
	st_recipe.nEpoxyRunSpeed                 = m_nEpoxyRunSpeed[1];             
	st_recipe.nEpoxyXYRunSpeed[EPOXY_VEL]    = m_nEpoxyXYRunSpeed[1][EPOXY_VEL];           ;//[0]//vel [1]:acc [2] dec
	st_recipe.nEpoxyXYRunSpeed[EPOXY_ACC]    = m_nEpoxyXYRunSpeed[1][EPOXY_ACC];
	st_recipe.nEpoxyXYRunSpeed[EPOXY_DEC]    = m_nEpoxyXYRunSpeed[1][EPOXY_DEC];
	
	st_recipe.dEpoxyXOffSet                  = m_dEpoxyXOffSet[1];
	st_recipe.dEpoxyYOffSet                  = m_dEpoxyYOffSet[1];
	st_recipe.nRubHSRunSpeed                 = m_nRubHSRunSpeed[1];
	
	st_recipe.dTrayPitch_Y                   = m_dTrayPitch_Y[1];			//트레이 피치간격 
	st_recipe.dTrayPitch_X                   = m_dTrayPitch_X[1];			//트레이 피치간격 	
	
	st_recipe.fDispenserVppmA                = m_nfDispenserVppmA[1];
	st_recipe.nEpoxyDotScrewCount            = m_nEpoxyDotScrewCount[1];
	st_recipe.nHsTrayY                       = m_nHsTrayY[1];						// HeatSink tray Y 정보.......
	st_recipe.nHsTrayX                       = m_nHsTrayX[1];						// HeatSink tray X 정보.......
	
	st_recipe.nTrayY                         = m_nTrayY[1];						// tray Y 정보.......
	st_recipe.nTrayX						 = m_nTrayX[1];	
	
	//kwlee 2017.0417
	st_recipe.dHSCarrierSpreadMoveOffset		= dHSCarrierSpreadMoveOffset[1];
	st_recipe.dHSCarrierSpreadMoveDistance		= dHSCarrierSpreadMoveDistance[1];
	st_recipe.dHeatSinkCarrierSpreadMove1Offset	= dHeatSinkCarrierSpreadMove1Offset[1];
	st_recipe.dHeatSinkCarrierSpreadMove2Offset	= dHeatSinkCarrierSpreadMove2Offset[1];
	st_recipe.dHeatSinkCarrierSpreadMove3Offset	= dHeatSinkCarrierSpreadMove3Offset[1];

	st_recipe.dHeatsinkRubXOffset	= m_dHeatsinkRubXOffset[1];
	st_recipe.dHeatsinkRubYOffset	= m_dHeatsinkRubYOffset[1];
	st_recipe.nEpoxyUseLimitCont	= m_nEpoxyLimitUseCont[1];
}

void CScreen_Set_Recipe::OnSetRecipe_Data_Recovery()
{
	m_dLoaderTransferTrayDeviceGap[1]   =   m_dLoaderTransferTrayDeviceGap[0];
	m_dSatbleTime[1]				    =	 m_dSatbleTime[0];
	m_nEpoxyRunSpeed[1]			 	    =	 m_nEpoxyRunSpeed[0];	
	m_nEpoxyXYRunSpeed[1][EPOXY_VEL]    =   m_nEpoxyXYRunSpeed[0][EPOXY_VEL];
	m_nEpoxyXYRunSpeed[1][EPOXY_ACC]    =   m_nEpoxyXYRunSpeed[0][EPOXY_ACC];
	m_nEpoxyXYRunSpeed[1][EPOXY_DEC]    =   m_nEpoxyXYRunSpeed[0][EPOXY_DEC];
	m_dEpoxyXOffSet[1]                  =   m_dEpoxyXOffSet[0];
	m_dEpoxyYOffSet[1]                  =   m_dEpoxyYOffSet[0];
	m_nRubHSRunSpeed[1]                 =   m_nRubHSRunSpeed[0];
	m_dTrayPitch_Y[1]			        =  	 m_dTrayPitch_Y[0];   
	m_dTrayPitch_X[1]			        =  	 m_dTrayPitch_X[0];   
	m_nfDispenserVppmA[1]               =    m_nfDispenserVppmA[0];
	m_nEpoxyDotScrewCount[1]            =    m_nEpoxyDotScrewCount[0];
	m_nHsTrayY[1]				 	    =	 m_nHsTrayY[0];
	m_nHsTrayX[1]					    =	 m_nHsTrayX[0];
	m_nTrayY[1]					        =	 m_nTrayY[0];   
    m_nTrayX[1]                         =    m_nTrayX[0];  

	//kwlee 2017.0417
	dHSCarrierSpreadMoveOffset[1]			= dHSCarrierSpreadMoveOffset[0];
	dHSCarrierSpreadMoveDistance[1]			= dHSCarrierSpreadMoveDistance[0];
	dHeatSinkCarrierSpreadMove1Offset[1]	= dHeatSinkCarrierSpreadMove1Offset[0];
	dHeatSinkCarrierSpreadMove2Offset[1]	= dHeatSinkCarrierSpreadMove2Offset[0];
	dHeatSinkCarrierSpreadMove3Offset[1]	= dHeatSinkCarrierSpreadMove3Offset[0];

	m_dHeatsinkRubXOffset[1]	= m_dHeatsinkRubXOffset[0];
	m_dHeatsinkRubXOffset[1]	= m_dHeatsinkRubXOffset[0];
	m_dHeatsinkRubXOffset[1]	= m_dHeatsinkRubXOffset[0];
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
	CString m_strTemp;
	//Start_www_tipssoft_com();			// 2014.12.24 - Bredmin.

	CDialog_Select  select_dlg;
	//CDialog_Message msg_dlg;			// 2014.12.24 - Bredmin.
	st_other.str_confirm_msg = _T("Recipe : Setting Data Apply?");
	
	n_response = select_dlg.DoModal();
	
	if (n_response == IDOK)
	{
		
		if( g_lotMgr.GetLotCount() > 0 )
		{
			m_strTemp = "진행중인 랏이 있습니다.";
			if ( g_local.GetLocalType() == LOCAL_ENG ) m_strTemp = "There is a Running LOT.";
			DoModal_Msg( m_strTemp );
			//OnBasic_Device_Focus_Set(); //kwlee 2017.0403
			return;
		}
		
		///Data_Apply(); //kwlee 2017.0403
		if (Data_Comparison() == RET_ERROR)
		{	
			OnSetRecipe_Data_Apply();
			OnSetRecipe_LogFile_Create();
			OnSetRecipe_Data_BackUp();
		}
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
void CScreen_Set_Recipe::OnSetRecipe_Label()
{
	m_msg_Offset_30.SetFont(mp_recipe_font);
	m_msg_Offset_30.SetWindowText(_T("Offset 30"));
	m_msg_Offset_30.SetCenterText();
	m_msg_Offset_30.SetColor(WHITE_C);
	m_msg_Offset_30.SetGradientColor(GREEN_C);
	m_msg_Offset_30.SetTextColor(BLACK_C);
	
	m_msg_Offset_20.SetFont(mp_recipe_font);
	m_msg_Offset_20.SetWindowText(_T("Offset 20"));
	m_msg_Offset_20.SetCenterText();
	m_msg_Offset_20.SetColor(WHITE_C);
	m_msg_Offset_20.SetGradientColor(GREEN_C);
	m_msg_Offset_20.SetTextColor(BLACK_C);
	
	
	m_msg_Offset_10.SetFont(mp_recipe_font);
	m_msg_Offset_10.SetWindowText(_T("Offset 10"));
	m_msg_Offset_10.SetCenterText();
	m_msg_Offset_10.SetColor(WHITE_C);
	m_msg_Offset_10.SetGradientColor(GREEN_C);
	m_msg_Offset_10.SetTextColor(BLACK_C);
	
	m_msg_Offset.SetFont(mp_recipe_font);
	m_msg_Offset.SetWindowText(_T("Offset"));
	m_msg_Offset.SetCenterText();
	m_msg_Offset.SetColor(WHITE_C);
	m_msg_Offset.SetGradientColor(GREEN_C);
	m_msg_Offset.SetTextColor(BLACK_C);
	
	
	m_msg_Distance.SetFont(mp_recipe_font);
	m_msg_Distance.SetWindowText(_T("Distance"));
	m_msg_Distance.SetCenterText();
	m_msg_Distance.SetColor(WHITE_C);
	m_msg_Distance.SetGradientColor(GREEN_C);
	m_msg_Distance.SetTextColor(BLACK_C);

	//kwlee 2017.0416
	m_msg_HsRub_X_Offset.SetFont(mp_recipe_font);
	m_msg_HsRub_X_Offset.SetWindowText(_T("HsRubX_Offset"));
	m_msg_HsRub_X_Offset.SetCenterText();
	m_msg_HsRub_X_Offset.SetColor(WHITE_C);
	m_msg_HsRub_X_Offset.SetGradientColor(GREEN_C);
	m_msg_HsRub_X_Offset.SetTextColor(BLACK_C);

	m_msg_HsRub_Y_Offset.SetFont(mp_recipe_font);
	m_msg_HsRub_Y_Offset.SetWindowText(_T("HsRubY_Offset"));
	m_msg_HsRub_Y_Offset.SetCenterText();
	m_msg_HsRub_Y_Offset.SetColor(WHITE_C);
	m_msg_HsRub_Y_Offset.SetGradientColor(GREEN_C);
	m_msg_HsRub_Y_Offset.SetTextColor(BLACK_C);
}
void CScreen_Set_Recipe::OnSetRecipe_EditBox_Set()
{
}

void CScreen_Set_Recipe::OnUserControl() 
{
	// TODO: Add your command handler code here
	
}

void CScreen_Set_Recipe::OnDgtDispenservppma() 
{
	// TODO: Add your control notification handler code here
	CString mstr_temp;  // 저장할 정보 임시 저장 변수 
	char chr_buf[20] ;
	int mn_Cnt;
	
	mn_Cnt = m_dgt_DispenserVppmA.GetValue();
	mstr_temp = LPCTSTR(_itoa(mn_Cnt, chr_buf, 10));
	
	st_msg.mstr_keypad_msg = _T("EPoxy Screw Cnt");
    
	st_msg.mstr_keypad_val = mstr_temp;
	mstr_temp = KeyPad.GetNumEditString_I(1, 500, atoi(st_msg.mstr_keypad_val), st_msg.mstr_keypad_msg);
	m_nfDispenserVppmA[1] = atoi(mstr_temp);
	m_dgt_DispenserVppmA.SetValue( m_nfDispenserVppmA[1]);
}

void CScreen_Set_Recipe::OnDgtEpoxyUsecnt() 
{
	// TODO: Add your control notification handler code here
	CString mstr_temp;  // 저장할 정보 임시 저장 변수 
	char chr_buf[20] ;
	int mn_Cnt;
	
	mn_Cnt = m_dgt_Epoxy_UseCnt.GetValue();
	mstr_temp = LPCTSTR(_itoa(mn_Cnt, chr_buf, 10));
	
	st_msg.mstr_keypad_msg = _T("EPoxy Use Limit Cnt");
    
	st_msg.mstr_keypad_val = mstr_temp;
	mstr_temp = KeyPad.GetNumEditString_I(1, 100, atoi(st_msg.mstr_keypad_val), st_msg.mstr_keypad_msg);
	m_nEpoxyLimitUseCont[1] = atoi(mstr_temp);
	m_dgt_Epoxy_UseCnt.SetValue( m_nEpoxyLimitUseCont[1]);
	
}

void CScreen_Set_Recipe::OnDgtHsRubXOffset() 
{
	// TODO: Add your control notification handler code here
	CString mstr_temp;  // 저장할 정보 임시 저장 변수 
	char chr_buf[20] ;
	double mdTemp;
	
	
	mdTemp = m_dgt_HsRub_X_Offset.GetValue();
	mstr_temp = LPCTSTR(_ltoa(mdTemp, chr_buf, 10));
	
	st_msg.mstr_keypad_msg = _T("Hs Rub X Offset");
    
	st_msg.mstr_keypad_val = mstr_temp;
	
	mstr_temp = KeyPad.GetNumEditString_D(1, 10000, atof(st_msg.mstr_keypad_val), st_msg.mstr_keypad_msg);
	m_dHeatsinkRubXOffset[1] = atof(mstr_temp);
	//int iTemp = atoi(mstr_temp);
	//g_dataMgr.SetDataTemp( &st_basic.n_tray_x, iTemp );
	mstr_temp.Format("%0.2f",m_dHeatsinkRubXOffset[1]);
	m_dgt_HsRub_X_Offset.SetValue( mstr_temp);
}

void CScreen_Set_Recipe::OnDgtHsRubYOffset() 
{
	CString mstr_temp;  // 저장할 정보 임시 저장 변수 
	char chr_buf[20] ;
	double mdTemp;
	
	
	mdTemp = m_dgt_HsRub_Y_Offset.GetValue();
	mstr_temp = LPCTSTR(_ltoa(mdTemp, chr_buf, 10));
	
	st_msg.mstr_keypad_msg = _T("Hs Rub Y Offset");
    
	st_msg.mstr_keypad_val = mstr_temp;
	
	mstr_temp = KeyPad.GetNumEditString_D(1, 10000, atof(st_msg.mstr_keypad_val), st_msg.mstr_keypad_msg);
	m_dHeatsinkRubYOffset[1] = atof(mstr_temp);
	//int iTemp = atoi(mstr_temp);
	//g_dataMgr.SetDataTemp( &st_basic.n_tray_x, iTemp );
	mstr_temp.Format("%0.2f",m_dHeatsinkRubYOffset[1]);
	m_dgt_HsRub_Y_Offset.SetValue( mstr_temp);		
}
