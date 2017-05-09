// Dialog_Lot_End.cpp : implementation file
//

#include "stdafx.h"
#include "handler.h"
#include "Dialog_Lot_End.h"
#include "Dialog_Select.h"

#include "AMTLotManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDialog_Lot_End dialog


CDialog_Lot_End::CDialog_Lot_End(CWnd* pParent /*=NULL*/)
	: StandardDialog(CDialog_Lot_End::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDialog_Lot_End)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_caption_main = "LOT END";
}


void CDialog_Lot_End::DoDataExchange(CDataExchange* pDX)
{
	StandardDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialog_Lot_End)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDialog_Lot_End, StandardDialog)
	//{{AFX_MSG_MAP(CDialog_Lot_End)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BTN_LOTEND, OnBtnLotend)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialog_Lot_End message handlers
BOOL CDialog_Lot_End::Create() 
{  
	return StandardDialog::Create(IDD);
}

BOOL CDialog_Lot_End::OnInitDialog() 
{
	StandardDialog::OnInitDialog();
	
	CenterWindow();
	mp_lot_end_font = NULL;
	mp_lot_end_font = new CFont;
	mp_lot_end_font->CreateFont(15,0, 0, 0, FW_BOLD, 0, 0, 0, 0, 0, 0, 0, 0, "Arial");
	
	OnLotEnd_Button_Set();
	OnLotEnd_Init_Lot_Info();

	st_handler.cwnd_lot_end = this;		// MAIN 화면 핸들 초기화

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDialog_Lot_End::OnDestroy() 
{
	st_handler.cwnd_lot_end = NULL;		// MAIN 화면 핸들 초기화

	// 해당 LOT의 COUNT를 삭제한다. 2K13/01/29/ViboX
// 	g_BinResultMgr.ClearCountData( LOT );

	Func.OnSet_IO_Port_Sound(IO_OFF);

	delete mp_lot_end_font;
	mp_lot_end_font = NULL;

	StandardDialog::OnDestroy();
}

void CDialog_Lot_End::GridColor(UINT nID, int row, int col, COLORREF bk, COLORREF tk)
{
	TSpread *Grid = (TSpread*)GetDlgItem(nID);
	Grid->SetColor(col, row, bk, tk);
	Grid->SetBackColorStyle(SS_BACKCOLORSTYLE_UNDERGRID);
	
	Grid = NULL;
	delete Grid;
}

void CDialog_Lot_End::GridControl(UINT nID, int type, int row, int col, int pos)
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

void CDialog_Lot_End::GridData(UINT nID, int row, int col, CString data)
{
	TSpread *Grid = (TSpread*)GetDlgItem(nID);
	
	Grid->SetValue(col, row, data);

	Grid = NULL;
	delete Grid;
}

void CDialog_Lot_End::GridFont(UINT nID, int row, int col, int size)
{
	TSpread *Grid = (TSpread *)GetDlgItem(nID);
	HFONT font;
	CString strTemp;
	
	font = CreateFont(size, 0, 0, 0, FW_BOLD, 0, 0, 0, 0, 0, 0, 0, 0, "Arial");
	Grid->SetFont(col, row, font, TRUE);

	Grid = NULL;
	delete Grid;
}

void CDialog_Lot_End::GridMerge(UINT nID, int srow, int scol, int nrow, int ncol)
{
	TSpread *Grid = (TSpread*)GetDlgItem(nID);
	
	Grid->AddCellSpan(scol, srow, ncol, nrow);

	Grid = NULL;
	delete Grid;
}


void CDialog_Lot_End::OnLotEnd_Button_Set()
{
	mp_lot_end_ok = CreateOkButton(IDOK, this);
}

void CDialog_Lot_End::OnLotEnd_Init_Lot_Info()
{
	TSpread *Grid;
	CString str_pos, str_temp;
	int row = 6;
	int col = 2;
	int i, j;
	
	Grid = (TSpread*)GetDlgItem(IDC_CUSTOM_LOT_INFO);
	Grid -> SetBool(SSB_EDITMODEPERMANENT, TRUE);
	Grid -> SetBool(SSB_SHOWCOLHEADERS, FALSE);
	Grid -> SetBool(SSB_SHOWROWHEADERS, FALSE);
	Grid -> SetBool(SSB_HORZSCROLLBAR, FALSE);
	Grid -> SetBool(SSB_VERTSCROLLBAR, FALSE);
	Grid -> SetMaxRows(row);
	Grid -> SetMaxCols(col);
	
	for (i = 1; i <= row; i++)
	{
		for (j = 1; j <= col; j++)
		{
			Grid -> SetRowHeightInPixels(i, 35);
			
			if (j == 1)
			{
				Grid -> SetColWidthInPixels(j, 130);
			}
			else
			{
				Grid -> SetColWidthInPixels(j, 210);
			}
			
			if ( ( i == 1 && j == 1 ) || ( i == 2 && j == 1 ) )
			{
				GridColor(IDC_CUSTOM_LOT_INFO, i, j, ORANGE_C, WHITE_C);
			}
			else if (i == 1 && j == 2)
			{
				GridColor(IDC_CUSTOM_LOT_INFO, i, j, BLACK_C, GREEN_B);
			}
			else if (i == 1 || j == 1)
			{
				GridColor(IDC_CUSTOM_LOT_INFO, i, j, WHITE_C, BLACK_C);
			}
			else
			{
				if (j == 4)
				{
					GridColor(IDC_CUSTOM_LOT_INFO, i, j, BLACK_C, RED_C);
				}
				else
				{
					GridColor(IDC_CUSTOM_LOT_INFO, i, j, BLACK_C, GREEN_B);
				}
			}
			GridControl(IDC_CUSTOM_LOT_INFO, STATIC, i, j, 0);
			GridFont(IDC_CUSTOM_LOT_INFO, i, j, 25);

			if ( i == 2 && j == 2 )
			{
				GridFont(IDC_CUSTOM_LOT_INFO, i, j, 15);
			}
		}
	}
		
	CString strTmp[6] =  {"LOT ID", "PART ID", "INPUT CNT", "GOOD CNT", "FAIL CNT", "YIELD"};
	
	for (i = 0; i < row; i++)
	{
		GridData(IDC_CUSTOM_LOT_INFO, i + 1, 1, strTmp[i]);
	}
	
	Grid = NULL;
	delete Grid;

	CString strTemp;
	
 	strTemp.Format("%s", g_lotMgr.GetLotAt(0).GetLotID() );
 	GridData(IDC_CUSTOM_LOT_INFO, 1, 2, strTemp);
 
 	strTemp.Format("%s", g_lotMgr.GetLotAt(0).GetPartID() );
 	GridData(IDC_CUSTOM_LOT_INFO, 2, 2, strTemp);
 	
	strTemp.Format("%04d", g_lotMgr.GetLotAt(0).GetInputCnt( PRIME ) );
	GridData(IDC_CUSTOM_LOT_INFO, 3, 2, strTemp);
	
	strTemp.Format("%04d", g_lotMgr.GetLotAt(0).GetPassCnt( PRIME ) );
	GridData(IDC_CUSTOM_LOT_INFO, 4, 2, strTemp);
	
	strTemp.Format("%04d", g_lotMgr.GetLotAt(0).GetFailCnt( PRIME ) );
	GridData(IDC_CUSTOM_LOT_INFO, 5, 2, strTemp);
	
	float fYield = 0.0;
	if ( g_lotMgr.GetLotAt(0).GetPassCnt( PRIME ) == 0)
	{
		fYield = 0;
	}
	else
	{
		fYield = (double)((double)g_lotMgr.GetLotAt(0).GetPassCnt( PRIME ) / (double)((double)g_lotMgr.GetLotAt(0).GetPassCnt( PRIME ) + (double)g_lotMgr.GetLotAt(0).GetFailCnt( PRIME ))) * (double)100;
	}
	
	strTemp.Format("%3.2f%%", fYield);
	GridData(IDC_CUSTOM_LOT_INFO, 6, 2, strTemp);
}
// 
// void CDialog_Lot_End::OnOK() 
// {
// 	Func.OnSet_IO_Port_Sound(IO_OFF);
// 	
// 	StandardDialog::OnOK();
// }

void CDialog_Lot_End::OnBtnLotend() 
{
	CString strData = "";
	int n_response = 0;
	CDialog_Select	select_dlg;
	st_other.str_confirm_msg = _T("Would you like to do LotEnd?");
	n_response = select_dlg.DoModal();
	
	if (n_response == IDOK)
	{
		strData.Format("LOTID: %s InPut: %d Pass: %d Fail: %d", g_lotMgr.GetLotAt(0).GetLotID(), 
			g_lotMgr.GetLotAt(0).GetInputCnt( PRIME ),g_lotMgr.GetLotAt(0).GetPassCnt( PRIME ), g_lotMgr.GetLotAt(0).GetFailCnt( PRIME ) );
		Func.On_LogFile_Add( LOG_LOT_PRO, strData);

		g_lotMgr.DeleteLot(0);
		if( st_handler.cwnd_main )
		{
			st_handler.cwnd_main->PostMessage(WM_WORK_END, MAIN_LOTINFO, 0);
		}

		for( int i = 0; i <THREAD_MAX_SITE; i++ )
		{
			st_sync.nLotEndFlag[0][i] = CTL_NO;
		}

		::PostMessage(st_handler.hWnd, WM_MAIN_EVENT, CTL_NO, 1);

	}
}

void CDialog_Lot_End::OnOK() 
{
	::PostMessage(st_handler.hWnd, WM_MAIN_EVENT, CTL_NO, 1);
}
