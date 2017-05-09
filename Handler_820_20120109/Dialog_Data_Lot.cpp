// Dialog_Data_Lot.cpp : implementation file
//

#include "stdafx.h"
#include "handler.h"
#include "Dialog_Data_Lot.h"
#include "SrcPart/PartFunction.h"
#include "AMTLotManager.h"
#include "ComizoaPublic.h"
#include "SrcPart/APartNetworkManager.h"
#include "Dialog_KeyPad.h"
#include "SrcBase/ALocalization.h"
#include "Dialog_LotStart.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDialog_Data_Lot dialog


CDialog_Data_Lot::CDialog_Data_Lot(CWnd* pParent /*=NULL*/)
	: StandardDialog(CDialog_Data_Lot::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDialog_Data_Lot)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_caption_main = "DATA (LOT)"; 
}


void CDialog_Data_Lot::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialog_Data_Lot)
	DDX_Control(pDX, IDC_BTN_UPDATE, m_btn_update);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDialog_Data_Lot, StandardDialog)
	//{{AFX_MSG_MAP(CDialog_Data_Lot)
	ON_BN_CLICKED(IDC_BTN_CREATE, OnBtnCreate)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BTN_DELETE, OnBtnDelete)
	ON_BN_CLICKED(IDC_BTN_UP, OnBtnUp)
	ON_BN_CLICKED(IDC_BTN_DOWN, OnBtnDown)
	ON_BN_CLICKED(IDC_BTN_UPDATE, OnBtnUpdate)
	ON_BN_CLICKED(IDC_BTN_NEW_LOT_IN, OnBtnNewLotIn)
	ON_BN_CLICKED(IDC_BTN_NET_LOT, OnBtnNetLot)
	//}}AFX_MSG_MAP
	ON_MESSAGE(SSM_CLICK, OnCellClick)
	ON_MESSAGE( WM_DRAW_DATA_LOT, OnDraw_Data_Lot)
END_MESSAGE_MAP()




/////////////////////////////////////////////////////////////////////////////
// CDialog_Data_Lot message handlers

BOOL CDialog_Data_Lot::OnInitDialog() 
{
	StandardDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_nSelectedLotIdx = -1;

	OnInit_Lot();
	OnInit_LotInfo();

	OnDisplay_Lot();

	st_handler.cwnd_data_lot = this;
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDialog_Data_Lot::OnInit_Lot()
{
	TSpread* Grid;
	int row = 5 + 1;
	int col = 1;

	Grid = (TSpread*)GetDlgItem( IDC_CUSTOM_LOT_LIST );
	Grid -> SetBool(SSB_EDITMODEPERMANENT, TRUE);
	Grid -> SetBool(SSB_SHOWCOLHEADERS, FALSE);
	Grid -> SetBool(SSB_SHOWROWHEADERS, FALSE);
	Grid -> SetBool(SSB_HORZSCROLLBAR, FALSE);
	Grid -> SetBool(SSB_VERTSCROLLBAR, FALSE);
	Grid -> SetMaxRows(row);
	Grid -> SetMaxCols(col);

	for( int iw=0; iw<row; iw++ )
	{
		Grid -> SetRowHeightInPixels(iw + 1, 20);
		Grid -> SetColWidthInPixels(1, 135 );
			
		GridFont(IDC_CUSTOM_LOT_LIST, iw + 1, 1, 15);
		GridControl(IDC_CUSTOM_LOT_LIST, STATIC, iw + 1, 1, 0);
	}

	GridColor(IDC_CUSTOM_LOT_LIST, 1, 1, BLACK, WHITE_C );
	GridData(IDC_CUSTOM_LOT_LIST, 1, 1, "LOT LIST" );
	
	Grid = NULL;
	delete Grid;
}

void CDialog_Data_Lot::OnInit_LotInfo()
{
	TSpread* Grid;
//	int row = 7 + 1;
	int row = 9 + 1;
	int col = 4;

	Grid = (TSpread*)GetDlgItem( IDC_CUSTOM_LOT_INFO );
	Grid -> SetBool(SSB_EDITMODEPERMANENT, TRUE);
	Grid -> SetBool(SSB_SHOWCOLHEADERS, FALSE);
	Grid -> SetBool(SSB_SHOWROWHEADERS, FALSE);
	Grid -> SetBool(SSB_HORZSCROLLBAR, FALSE);
	Grid -> SetBool(SSB_VERTSCROLLBAR, FALSE);
	Grid -> SetMaxRows(row);
	Grid -> SetMaxCols(col);

	Grid -> SetColWidthInPixels(1, 160 );
	Grid -> SetColWidthInPixels(2, 50 );
	Grid -> SetColWidthInPixels(3, 50 );
	Grid -> SetColWidthInPixels(4, 60 );
	for( int iw=0; iw<row; iw++ )
	{
		Grid -> SetRowHeightInPixels(iw + 1, 18);
			
		GridFont(IDC_CUSTOM_LOT_INFO, iw + 1, 1, 15);
		GridFont(IDC_CUSTOM_LOT_INFO, iw + 1, 2, 15);
		GridFont(IDC_CUSTOM_LOT_INFO, iw + 1, 3, 15);
		GridFont(IDC_CUSTOM_LOT_INFO, iw + 1, 4, 15);
		GridControl(IDC_CUSTOM_LOT_INFO, STATIC, iw + 1, 1, 0);

		GridColor( IDC_CUSTOM_LOT_INFO, iw + 1, 1, ORANGE_C, BLACK_C );
	}

	GridMerge( IDC_CUSTOM_LOT_INFO, 1, 1, 1, 4 );
	GridMerge( IDC_CUSTOM_LOT_INFO, 2, 2, 1, 4 );
	GridMerge( IDC_CUSTOM_LOT_INFO, 3, 2, 1, 4 );
	GridMerge( IDC_CUSTOM_LOT_INFO, 4, 2, 1, 4 );
	GridMerge( IDC_CUSTOM_LOT_INFO, 5, 2, 1, 4 );
	GridMerge( IDC_CUSTOM_LOT_INFO, 6, 2, 1, 4 );
	GridMerge( IDC_CUSTOM_LOT_INFO, 7, 2, 1, 4 );
	GridMerge( IDC_CUSTOM_LOT_INFO, 8, 2, 1, 4 );
	GridMerge( IDC_CUSTOM_LOT_INFO, 9, 2, 1, 4 );

	GridColor(IDC_CUSTOM_LOT_INFO, 1, 1, BLACK, WHITE_C );
	GridData(IDC_CUSTOM_LOT_INFO, 1, 1, "LOT INFO" );

	GridData( IDC_CUSTOM_LOT_INFO, 2, 1, "LOT NO" );
	GridData( IDC_CUSTOM_LOT_INFO, 3, 1, "PART NO" );
	GridData( IDC_CUSTOM_LOT_INFO, 4, 1, "NEW_LOT_IN PASS(NEXT)" );
	GridData( IDC_CUSTOM_LOT_INFO, 5, 1, "START TIME" );
	GridData( IDC_CUSTOM_LOT_INFO, 6, 1, "LAST MODULE" );
	GridData( IDC_CUSTOM_LOT_INFO, 7, 1, "LOT TOTAL" ); //2013,0910
	GridData( IDC_CUSTOM_LOT_INFO, 8, 1, "BYPASS" );
	GridData( IDC_CUSTOM_LOT_INFO, 9, 1, "DVC TYPE" );

	//2016.0112
	GridData( IDC_CUSTOM_LOT_INFO, 10, 1, "TIME OUT" );

	GridControl(IDC_CUSTOM_LOT_INFO, STATIC, 4, 2, 0);
	GridControl(IDC_CUSTOM_LOT_INFO, STATIC, 5, 2, 0);
	GridControl(IDC_CUSTOM_LOT_INFO, STATIC, 6, 2, 0);
	GridControl(IDC_CUSTOM_LOT_INFO, STATIC, 7, 2, 0);
	GridControl(IDC_CUSTOM_LOT_INFO, STATIC, 8, 2, 0);
	GridControl(IDC_CUSTOM_LOT_INFO, STATIC, 9, 2, 0);  //2013,0910
	GridControl(IDC_CUSTOM_LOT_INFO, STATIC, 10, 2, 0);  
	GridControl(IDC_CUSTOM_LOT_INFO, STATIC, 10, 3, 0);  
	GridControl(IDC_CUSTOM_LOT_INFO, STATIC, 10, 4, 0);  
	Grid = NULL;
	delete Grid;
}

void CDialog_Data_Lot::OnDisplay_Lot()
{
	GridColor( IDC_CUSTOM_LOT_LIST, 2, 1, WHITE_C, BLACK_C );	GridData( IDC_CUSTOM_LOT_LIST, 2, 1, "" );
	GridColor( IDC_CUSTOM_LOT_LIST, 3, 1, WHITE_C, BLACK_C );	GridData( IDC_CUSTOM_LOT_LIST, 3, 1, "" );
	GridColor( IDC_CUSTOM_LOT_LIST, 4, 1, WHITE_C, BLACK_C );	GridData( IDC_CUSTOM_LOT_LIST, 4, 1, "" );
	GridColor( IDC_CUSTOM_LOT_LIST, 5, 1, WHITE_C, BLACK_C );	GridData( IDC_CUSTOM_LOT_LIST, 5, 1, "" );
	GridColor( IDC_CUSTOM_LOT_LIST, 6, 1, WHITE_C, BLACK_C );	GridData( IDC_CUSTOM_LOT_LIST, 6, 1, "" );
	GridColor( IDC_CUSTOM_LOT_LIST, 7, 1, WHITE_C, BLACK_C );	GridData( IDC_CUSTOM_LOT_LIST, 7, 1, "" );
	GridColor( IDC_CUSTOM_LOT_LIST, 10, 3, WHITE_C, BLACK_C );  GridData( IDC_CUSTOM_LOT_LIST, 10, 3, "" );
	GridColor( IDC_CUSTOM_LOT_LIST, 10, 4, WHITE_C, BLACK_C );  GridData( IDC_CUSTOM_LOT_LIST, 10, 4, "" );

	int nCnt = g_lotMgr.GetLotCount();
	for( int i=0; i<nCnt; i++ )
	{
		GridColor( IDC_CUSTOM_LOT_LIST, i + 2, 1, SKY_C, BLACK_C );
		GridData( IDC_CUSTOM_LOT_LIST, i + 2, 1, g_lotMgr.GetLotIDAt(i) );
	}

	if( m_nSelectedLotIdx >= 0 )
	{
		GridColor( IDC_CUSTOM_LOT_LIST, m_nSelectedLotIdx + 2, 1, YELLOW_C, BLACK_C );
	}
}

void CDialog_Data_Lot::OnDisplay_LotInfo()
{
	CString strTimeout="", strTmp="";

	if( m_nSelectedLotIdx < 0 ||
		m_nSelectedLotIdx >= g_lotMgr.GetLotCount() )
	{
		GridData( IDC_CUSTOM_LOT_INFO, 2, 2, "" );
		GridData( IDC_CUSTOM_LOT_INFO, 3, 2, "" );
		GridData( IDC_CUSTOM_LOT_INFO, 4, 2, "" );
		GridData( IDC_CUSTOM_LOT_INFO, 5, 2, "" );
		GridData( IDC_CUSTOM_LOT_INFO, 6, 2, "" );
		GridData( IDC_CUSTOM_LOT_INFO, 7, 2, "" );
		GridData( IDC_CUSTOM_LOT_INFO, 8, 2, "" );
		GridData( IDC_CUSTOM_LOT_INFO, 9, 2, "" );//2013,0910
		GridData( IDC_CUSTOM_LOT_INFO, 10, 2, "" );
		GridData( IDC_CUSTOM_LOT_INFO, 10, 3, "" );
		GridData( IDC_CUSTOM_LOT_INFO, 10, 4, "" );
		return;
	}

	GridData( IDC_CUSTOM_LOT_INFO, 2, 2, g_lotMgr.GetLotAt(m_nSelectedLotIdx).GetLotID() );
	GridData( IDC_CUSTOM_LOT_INFO, 3, 2, g_lotMgr.GetLotAt(m_nSelectedLotIdx).GetPartID() );
	GridData( IDC_CUSTOM_LOT_INFO, 4, 2, g_lotMgr.GetLotAt(m_nSelectedLotIdx).GetStrNewLotInPass() );
	GridData( IDC_CUSTOM_LOT_INFO, 5, 2, g_lotMgr.GetLotAt(m_nSelectedLotIdx).GetTimeStart() );
	GridData( IDC_CUSTOM_LOT_INFO, 6, 2, g_lotMgr.GetLotAt(m_nSelectedLotIdx).GetStrLastModule() );
	if( g_lotMgr.GetLotAt(m_nSelectedLotIdx).GetTotLotCount() <= 0 )
		g_lotMgr.GetLotAt(m_nSelectedLotIdx).SetLotCount(0);
	strTmp.Format("%d", g_lotMgr.GetLotAt(m_nSelectedLotIdx).GetTotLotCount());
	GridData( IDC_CUSTOM_LOT_INFO, 7, 2, strTmp );
	GridData( IDC_CUSTOM_LOT_INFO, 8, 2, g_lotMgr.GetLotAt(m_nSelectedLotIdx).GetStrBypass() );
	GridData( IDC_CUSTOM_LOT_INFO, 10, 2, g_lotMgr.GetLotAt(m_nSelectedLotIdx).GetAutoTimeOut());

	strTimeout.Format("%d", g_lotMgr.GetLotAt(m_nSelectedLotIdx).GetTimeOutCnt());
	GridData( IDC_CUSTOM_LOT_INFO, 10, 3, strTimeout);
	strTimeout.Format("%d sec", g_lotMgr.GetLotAt(m_nSelectedLotIdx).GetTimeOut());
	GridData( IDC_CUSTOM_LOT_INFO, 10, 4, strTimeout);

	CString strCokType;
	strCokType.Format( "%s", g_lotMgr.GetLotAt(m_nSelectedLotIdx).GetDvcType());
	GridData( IDC_CUSTOM_LOT_INFO, 9, 2, strCokType );

	if( st_handler.cwnd_main )
		st_handler.cwnd_main->PostMessage(WM_WORK_END, MAIN_LOTINFO, 0);
}

void CDialog_Data_Lot::GridColor(UINT nID, int row, int col, COLORREF bk, COLORREF tk)
{
	TSpread *Grid = (TSpread*)GetDlgItem(nID);
	
	COLORREF bk_old, tk_old;
	Grid->GetColor(col, row, &bk_old, &tk_old);
	if( bk_old != bk || tk_old != tk )
	{
		Grid->SetColor(col, row, bk, tk);
		Grid->SetBackColorStyle(SS_BACKCOLORSTYLE_UNDERGRID);
	}

	Grid = NULL;
	delete Grid;
}

void CDialog_Data_Lot::GridControl(UINT nID, int type, int row, int col, int pos)
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

void CDialog_Data_Lot::GridData(UINT nID, int row, int col, CString data)
{
	TSpread *Grid = (TSpread*)GetDlgItem(nID);

	Grid->SetValue(col, row, data);

	Grid = NULL;
	delete Grid;
}

void CDialog_Data_Lot::GridFont(UINT nID, int row, int col, int size)
{
	TSpread *Grid = (TSpread *)GetDlgItem(nID);
	HFONT font;
	CString strTemp;
	
	font = CreateFont(size, 0, 0, 0, FW_BOLD, 0, 0, 0, 0, 0, 0, 0, 0, "돋음체");
	Grid->SetFont(col, row, font, TRUE);

	Grid = NULL;
	delete Grid;
}

void CDialog_Data_Lot::GridMerge(UINT nID, int srow, int scol, int nrow, int ncol)
{
	TSpread *Grid = (TSpread*)GetDlgItem(nID);
	
	Grid->AddCellSpan(scol, srow, ncol, nrow);

	Grid = NULL;
	delete Grid;
}

void CDialog_Data_Lot::OnCellClick( WPARAM wParam, LPARAM lParam )
{
	LPSS_CELLCOORD lpcc = (LPSS_CELLCOORD)lParam;

	if( wParam == IDC_CUSTOM_LOT_LIST )
	{
		int nCnt = g_lotMgr.GetLotCount();
		if( nCnt > lpcc->Row - 2 )
		{
			m_nSelectedLotIdx = lpcc->Row - 2;
			OnDisplay_Lot();
			OnDisplay_LotInfo();
		}
		else
		{
			m_nSelectedLotIdx = -1;
		}
	}

	else if( wParam == IDC_CUSTOM_LOT_INFO )
	{
		if( lpcc->Col == 1 )	return;

		switch( lpcc->Row )
		{
		case 7:
			{

				if( m_nSelectedLotIdx < 0 ||
					m_nSelectedLotIdx >= g_lotMgr.GetLotCount() )
					return;
				
				CString mstr_temp;  // 저장할 정보 임시 저장 변수 
				int mn_lotcount;
				
				TSpread *Grid = (TSpread*)GetDlgItem(IDC_CUSTOM_LOT_INFO);
				char strGet[256];
				sprintf(strGet, "%d", g_lotMgr.GetLotAt(m_nSelectedLotIdx).GetTotLotCount());
				Grid->GetData( 7, 10, strGet );
				
				mn_lotcount = atoi(strGet);						
				
				CRect r;
				m_btn_update.GetWindowRect(&r);
				mstr_temp = KeyPad.GetNumEditString_I(1, 1000, mn_lotcount, mstr_temp, &r);
				
				int nTemp = 0;
				nTemp = atoi(mstr_temp);
				if(  nTemp < 1 || nTemp > 1000 ) return;
				
				g_lotMgr.GetLotAt(m_nSelectedLotIdx).SetLotCount( nTemp );
				
				GridData( IDC_CUSTOM_LOT_INFO, lpcc->Row, 2, mstr_temp );
			}
			break;


			//DVCTYPE
		case 9:
			{
				lpcc->Col = 2;
				CString strData;
				TSpread *Grid = (TSpread*)GetDlgItem(IDC_CUSTOM_LOT_INFO);
				
				char strGet[16];
				Grid->GetData( lpcc->Col, lpcc->Row, strGet );
				
				strData = strGet;
				if( strData == "SFF" )
				{
					GridData( IDC_CUSTOM_LOT_INFO, lpcc->Row, lpcc->Col, "TFF" );
				}
				else
				{
					GridData( IDC_CUSTOM_LOT_INFO, lpcc->Row, lpcc->Col, "SFF" );
				}
				
				Grid = NULL;
				delete Grid;
			}
			break;
				

		case 4:
		case 6:
		case 8:
		case 10:
			{
				//2016.0112
				if(lpcc->Row == 10)
				{
					if( lpcc->Col == 3 )
					{
						if( m_nSelectedLotIdx < 0 ||
							m_nSelectedLotIdx >= g_lotMgr.GetLotCount() )
							return;
						
						CString mstr_temp;  // 저장할 정보 임시 저장 변수 
						int mn_minute;
						
						TSpread *Grid = (TSpread*)GetDlgItem(IDC_CUSTOM_LOT_INFO);
						char strGet[256];
						sprintf(strGet, "%d sec", g_lotMgr.GetLotAt(m_nSelectedLotIdx).GetTimeOutCnt());
						Grid->GetData( 3, 10, strGet );
						
						mn_minute = atoi(strGet);
						
						mstr_temp = _T("Count (회)");
						if ( g_local.GetLocalType() == LOCAL_ENG )	mstr_temp = _T("Count ");
						
						CRect r;
						m_btn_update.GetWindowRect(&r);
						mstr_temp = KeyPad.GetNumEditString_I(0, 8, mn_minute, mstr_temp, &r);
						
						int nTemp = 0;
						if(mstr_temp == "") mstr_temp = "0";
						nTemp = atoi(mstr_temp);
						if(mn_minute < nTemp)
						{
							AfxMessageBox("You can't write more than the first value.");
							return;
						}
						if(  nTemp < 0 || nTemp > 8 ) return;
						/*mstr_temp += " sec";*/
						
						g_lotMgr.GetLotAt(m_nSelectedLotIdx).SetTimeOutCnt(nTemp);
						
						GridData( IDC_CUSTOM_LOT_INFO, lpcc->Row, lpcc->Col, mstr_temp );
						
						return;
					}					
					else if( lpcc->Col == 4 )
					{
						if( m_nSelectedLotIdx < 0 ||
							m_nSelectedLotIdx >= g_lotMgr.GetLotCount() )
							return;

						CString mstr_temp;  // 저장할 정보 임시 저장 변수 
						int mn_minute;

						TSpread *Grid = (TSpread*)GetDlgItem(IDC_CUSTOM_LOT_INFO);
						char strGet[256];
						sprintf(strGet, "%d sec", g_lotMgr.GetLotAt(m_nSelectedLotIdx).GetTimeOut());
						Grid->GetData( 4, 10, strGet );
						
						mn_minute = atoi(strGet);
						
						mstr_temp = _T("Time out 시간 (초)");
						if ( g_local.GetLocalType() == LOCAL_ENG )	mstr_temp = _T("Time out(sec)");
						
						CRect r;
						m_btn_update.GetWindowRect(&r);
						mstr_temp = KeyPad.GetNumEditString_I(1, 900, mn_minute, mstr_temp, &r);

						int nTemp = 0;
						nTemp = atoi(mstr_temp);
						if(  nTemp < 1 || nTemp > 900 ) return;
						mstr_temp += " sec";

						g_lotMgr.GetLotAt(m_nSelectedLotIdx).SetTimeOut( nTemp );
						
						GridData( IDC_CUSTOM_LOT_INFO, lpcc->Row, lpcc->Col, mstr_temp );

						return;
					}

				}
				//2016.0112
				if( lpcc->Col == 3 || lpcc->Col == 4 ) lpcc->Col = 2;
				CString strData;
				TSpread *Grid = (TSpread*)GetDlgItem(IDC_CUSTOM_LOT_INFO);
				
				char strGet[16];
				Grid->GetData( lpcc->Col, lpcc->Row, strGet );

				strData = strGet;
				if( strData == "YES" )
				{
					GridData( IDC_CUSTOM_LOT_INFO, lpcc->Row, lpcc->Col, "NO" );
				}
				else
				{
					GridData( IDC_CUSTOM_LOT_INFO, lpcc->Row, lpcc->Col, "YES" );
				}
				
				Grid = NULL;
				delete Grid;
			}
			break;
		}
	}
}


void CDialog_Data_Lot::OnBtnCreate() 
{
	// TODO: Add your control notification handler code here
	if( COMI.mn_run_status != dSTOP )
		return;

	g_lotMgr.AddLot("", "");
	if( st_handler.cwnd_main )
		st_handler.cwnd_main->PostMessage(WM_WORK_END, MAIN_LOTINFO, 0);

	if (st_handler.cwnd_list != NULL)	// 리스트 바 화면 존재
	{
		sprintf(st_msg.c_normal_msg, "[LOT] LOT Create.");
		st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // 동작 실패 출력 요청
	}
	OnDisplay_Lot();
}

LRESULT CDialog_Data_Lot::OnDraw_Data_Lot( WPARAM wParam,LPARAM lParam )
{
	m_nSelectedLotIdx = -1;

	OnDisplay_Lot();
	OnDisplay_LotInfo();

	return 0;
}

void CDialog_Data_Lot::OnDestroy() 
{
	StandardDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	st_handler.cwnd_data_lot = NULL;
}

void CDialog_Data_Lot::OnBtnDelete() 
{
	if( COMI.mn_run_status != dSTOP )
		return;

	if( m_nSelectedLotIdx < 0 ||
		m_nSelectedLotIdx >= g_lotMgr.GetLotCount() )
		return;

	g_lotMgr.DeleteLot( m_nSelectedLotIdx );

	if( st_handler.cwnd_main )
		st_handler.cwnd_main->PostMessage(WM_WORK_END, MAIN_LOTINFO, 0);

	if (st_handler.cwnd_list != NULL)	// 리스트 바 화면 존재
	{
		sprintf(st_msg.c_normal_msg, "[LOT] LOT Delete.");
		st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // 동작 실패 출력 요청
	}

	OnDisplay_Lot();
	OnDisplay_LotInfo();
}

void CDialog_Data_Lot::OnBtnUp() 
{
	if( COMI.mn_run_status != dSTOP )
		return;

	if( m_nSelectedLotIdx <= 0 )
		return;

	ALot& prev = g_lotMgr.GetLotAt(m_nSelectedLotIdx - 1);
	ALot& current = g_lotMgr.GetLotAt(m_nSelectedLotIdx);
	ALot tmp = current;
	current = prev;
	prev = tmp;

	m_nSelectedLotIdx -= 1;
	OnDisplay_Lot();
	OnDisplay_LotInfo();
}

void CDialog_Data_Lot::OnBtnDown() 
{
	if( COMI.mn_run_status != dSTOP )
		return;

	if( m_nSelectedLotIdx >= g_lotMgr.GetLotCount() - 1 )
		return;

	ALot& next = g_lotMgr.GetLotAt(m_nSelectedLotIdx + 1);
	ALot& current = g_lotMgr.GetLotAt(m_nSelectedLotIdx);
	ALot tmp = current;
	current = next;
	next = tmp;

	m_nSelectedLotIdx += 1;
	OnDisplay_Lot();
	OnDisplay_LotInfo();
}

void CDialog_Data_Lot::OnBtnUpdate() 
{
	if( COMI.mn_run_status != dSTOP )
		return;

	if( m_nSelectedLotIdx < 0 ||
		m_nSelectedLotIdx >= g_lotMgr.GetLotCount() )
		return;

	TSpread *Grid = (TSpread*)GetDlgItem(IDC_CUSTOM_LOT_INFO);
				
	char strGet[256];
	Grid->GetData( 2, 2, strGet );	g_lotMgr.GetLotAt( m_nSelectedLotIdx ).SetLotID( strGet );
	Grid->GetData( 2, 3, strGet );	g_lotMgr.GetLotAt( m_nSelectedLotIdx ).SetPartID( strGet );
	Grid->GetData( 2, 4, strGet );	g_lotMgr.GetLotAt( m_nSelectedLotIdx ).SetNewLotInPass( (strcmp( strGet, "YES" ) == 0) );
	Grid->GetData( 2, 6, strGet );	g_lotMgr.GetLotAt( m_nSelectedLotIdx ).SetLastModule( strGet );
	Grid->GetData( 2, 7, strGet );	
	g_lotMgr.GetLotAt( m_nSelectedLotIdx ).SetLotCount( atoi(strGet) );
	Grid->GetData( 2, 8, strGet );	g_lotMgr.GetLotAt( m_nSelectedLotIdx ).SetByPass( strGet );
	Grid->GetData( 2, 9, strGet );	g_lotMgr.GetLotAt( m_nSelectedLotIdx ).SetDvcType( strGet );
	Grid->GetData( 2, 10, strGet );	g_lotMgr.GetLotAt( m_nSelectedLotIdx ).SetAutoTimeOut( strGet );
	//2016.0202
	if( st_basic.n_auto_timeout_mode == CTL_YES )
	{
 		if( g_lotMgr.GetLotAt( m_nSelectedLotIdx ).GetAutoTimeOut() == "YES" )
 		{
//			int nTime = g_lotMgr.GetLotAt( m_nSelectedLotIdx ).GetTimeOut();
//			if(st_handler.cwnd_title != NULL)
//			{
//				st_handler.cwnd_title->PostMessage(WM_STATUS_CHANGE, AOTO_TIMEOUT_MODE, nTime);
//			}

//  			if( g_lotMgr.GetLotAt( m_nSelectedLotIdx ).GetTimeOutCnt()>= TEST_MAX_CNT)
//  			{
//  				g_lotMgr.GetLotAt( m_nSelectedLotIdx ).SetTimeOutCnt(-1);
//  			}
 		}
	}
	Grid = NULL;
	delete Grid;

	g_lotMgr.GetLotAt( m_nSelectedLotIdx ).CalcCokType();

	if( g_lotMgr.GetLotAt(m_nSelectedLotIdx).GetDvcType() != "SFF" && 
		g_lotMgr.GetLotAt(m_nSelectedLotIdx).GetDvcType() != "TFF" )
	{
		AfxMessageBox("DVC 타입이 설정되지 않았습니다.");
	}

	OnDisplay_Lot();
	OnDisplay_LotInfo();
}

void CDialog_Data_Lot::OnBtnNewLotIn() 
{
	if( g_lotMgr.GetLotCount() > 0 )
	{
		if( m_nSelectedLotIdx >= 0 )
		{
			st_work.n_DataYes[0] = "YES";
			st_work.n_DataYes[1].Format("%d", m_nSelectedLotIdx);
		}
	}

}

void CDialog_Data_Lot::OnBtnNetLot() 
{
	CDialog_LotStart dlg;
	dlg.DoModal();	
}
