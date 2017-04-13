// Dialog_LotStart.cpp : implementation file
//

#include "stdafx.h"
#include "handler.h"
#include "Dialog_LotStart.h"
#include "AMTLotManager.h"
#include "Dialog_KeyPad.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDialog_LotStart dialog


CDialog_LotStart::CDialog_LotStart(CWnd* pParent /*=NULL*/)
	: StandardDialog(CDialog_LotStart::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDialog_LotStart)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDialog_LotStart::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialog_LotStart)
	DDX_Control(pDX, IDC_LOT_CANCEL, m_btn_lotcancel);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDialog_LotStart, StandardDialog)
	//{{AFX_MSG_MAP(CDialog_LotStart)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_LOT_DISPLAY, OnLotDisplay)
	ON_BN_CLICKED(IDC_LOT_START, OnLotStart)
	//}}AFX_MSG_MAP
	ON_MESSAGE(SSM_CLICK, OnCellClick)
	ON_MESSAGE( WM_DRAW_DATA_LOT, OnDraw_Data_LotStart)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialog_LotStart message handlers

BOOL CDialog_LotStart::OnInitDialog() 
{
	StandardDialog::OnInitDialog();
	
	// TODO: Add extra initialization here

	OnInitGridLotInfo();
	OnDisplay_Lot();

	st_handler.cwnd_lotstart = this;
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDialog_LotStart::OnInitGridLotInfo()
{
	TSpread *Grid;
	int row = 6;
	int col = 2;

	Grid = (TSpread*)GetDlgItem( IDC_CUSTOM_LOT_INFO );
	Grid->SetBool(SSB_EDITMODEPERMANENT, TRUE);
	Grid->SetBool(SSB_SHOWCOLHEADERS, FALSE);
	Grid->SetBool(SSB_SHOWROWHEADERS, FALSE);
	Grid->SetBool(SSB_HORZSCROLLBAR, FALSE);
	Grid->SetBool(SSB_VERTSCROLLBAR, FALSE);
	Grid->SetMaxRows(row);
	Grid->SetMaxCols(col);


	Grid -> SetColWidthInPixels(1, 180 );
	Grid -> SetColWidthInPixels(2, 580 );

	for( int iw=0; iw < row; iw++ )
	{
		Grid -> SetRowHeightInPixels(iw + 1, 70);
		GridFont(IDC_CUSTOM_LOT_INFO, iw + 1, 1, 25);
		GridFont(IDC_CUSTOM_LOT_INFO, iw + 1, 2, 25);
		GridControl(IDC_CUSTOM_LOT_INFO, STATIC, iw + 1, 1, 0);

		GridColor( IDC_CUSTOM_LOT_INFO, iw + 1, 1, ORANGE_C, BLACK_C );
	}

	GridMerge( IDC_CUSTOM_LOT_INFO, 1, 1, 1, 4 );
	GridMerge( IDC_CUSTOM_LOT_INFO, 2, 2, 1, 4 );
	GridMerge( IDC_CUSTOM_LOT_INFO, 3, 2, 1, 4 );
	GridMerge( IDC_CUSTOM_LOT_INFO, 4, 2, 1, 4 );
	GridMerge( IDC_CUSTOM_LOT_INFO, 5, 2, 1, 4 );
	GridMerge( IDC_CUSTOM_LOT_INFO, 6, 2, 1, 4 );



	GridColor(IDC_CUSTOM_LOT_INFO, 1, 1, BLACK, WHITE_C );
	GridData(IDC_CUSTOM_LOT_INFO, 1, 1, "LOT INFO" );

	GridData( IDC_CUSTOM_LOT_INFO, 2, 1, "LOT NO" );
	GridData( IDC_CUSTOM_LOT_INFO, 3, 1, "PART NO" );
	GridData( IDC_CUSTOM_LOT_INFO, 4, 1, "Device Type" );
	GridData( IDC_CUSTOM_LOT_INFO, 5, 1, "Device density" );
	GridData( IDC_CUSTOM_LOT_INFO, 6, 1, "Device Count" );

	m_strCombo = "SFF";
	m_strCombo += "\tTFF";
	GridControl(IDC_CUSTOM_LOT_INFO, COMBO, 4, 2, 0);

	m_strCombo = "4GB";
	m_strCombo += "\t8G/16G\t32G\t64G";
	GridControl(IDC_CUSTOM_LOT_INFO, COMBO, 5, 2, 0);
	//GridControl(IDC_CUSTOM_LOT_INFO, STATIC, 6, 2, 0);
	//GridControl(IDC_CUSTOM_LOT_INFO, STATIC, 7, 2, 0);
	//GridControl(IDC_CUSTOM_LOT_INFO, STATIC, 8, 2, 0);

	Grid = NULL;
	delete Grid;
}

void CDialog_LotStart::OnDisplay_Lot()
{
	/*GridColor( IDC_CUSTOM_LOT_LIST, 2, 1, WHITE_C, BLACK_C );	GridData( IDC_CUSTOM_LOT_LIST, 2, 1, "" );
	GridColor( IDC_CUSTOM_LOT_LIST, 3, 1, WHITE_C, BLACK_C );	GridData( IDC_CUSTOM_LOT_LIST, 3, 1, "" );
	GridColor( IDC_CUSTOM_LOT_LIST, 4, 1, WHITE_C, BLACK_C );	GridData( IDC_CUSTOM_LOT_LIST, 4, 1, "" );
	GridColor( IDC_CUSTOM_LOT_LIST, 5, 1, WHITE_C, BLACK_C );	GridData( IDC_CUSTOM_LOT_LIST, 5, 1, "" );
	GridColor( IDC_CUSTOM_LOT_LIST, 6, 1, WHITE_C, BLACK_C );	GridData( IDC_CUSTOM_LOT_LIST, 6, 1, "" );

	int nCnt = g_lotMgr.GetLotCount();
	for( int i=0; i<nCnt; i++ )
	{
		GridColor( IDC_CUSTOM_LOT_LIST, i + 2, 1, SKY_C, BLACK_C );
		GridData( IDC_CUSTOM_LOT_LIST, i + 2, 1, g_lotMgr.GetLotIDAt(i) );
	}

	if( m_nSelectedLotIdx >= 0 )
	{
		GridColor( IDC_CUSTOM_LOT_LIST, m_nSelectedLotIdx + 2, 1, YELLOW_C, BLACK_C );
	}*/
}

LRESULT CDialog_LotStart::OnDraw_Data_LotStart( WPARAM wParam,LPARAM lParam )
{
	m_nSelectedLotIdx = -1;

	OnDisplay_Lot();
	//OnDisplay_LotInfo();

	return 0;
}


void CDialog_LotStart::OnCellClick( WPARAM wParam, LPARAM lParam )
{
	LPSS_CELLCOORD lpcc = (LPSS_CELLCOORD)lParam;
/*
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
*/

	GridControl(IDC_CUSTOM_LOT_INFO, EDIT, 2, 2, 0);
	GridControl(IDC_CUSTOM_LOT_INFO, EDIT, 3, 2, 0);

	if( wParam == IDC_CUSTOM_LOT_INFO )
	{
		if( lpcc->Col == 1 )	return;

		switch( lpcc->Row )
		{
		case 6:
	//	case 8:
			{
				//2016.0112
				if(lpcc->Row == 6)
				{
					if( lpcc->Col == 2 )
					{


						CString mstr_temp;  // 저장할 정보 임시 저장 변수 
						int mn_minute;


						TSpread *Grid = (TSpread*)GetDlgItem(IDC_CUSTOM_LOT_INFO);
						char strGet[100];
						//sprintf(strGet, "%d sec", g_lotMgr.GetLotAt(m_nSelectedLotIdx).GetTimeOut());
						Grid->GetData( 2, 6, strGet );
						
						mn_minute = atoi(strGet);
						
						mstr_temp = _T("개수(ea)");
						
						CRect r;
						m_btn_lotcancel.GetWindowRect(&r);
						mstr_temp = KeyPad.GetNumEditString_I(1, 900, mn_minute, mstr_temp, &r);

						int nTemp = 0;
						nTemp = atoi(mstr_temp);
						if(  nTemp < 1 || nTemp > 900 ) return;
						mstr_temp += " 개(ea)";

						//g_lotMgr.GetLotAt(m_nSelectedLotIdx).SetTimeOut( nTemp );
						
						GridData( IDC_CUSTOM_LOT_INFO, lpcc->Row, lpcc->Col, mstr_temp );
						Grid = NULL;
						delete Grid;

						return;
					}
				}
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
			
				
		
		
		}
	}
}


void CDialog_LotStart::GridColor(UINT nID, int row, int col, COLORREF bk, COLORREF tk)
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

void CDialog_LotStart::GridControl(UINT nID, int type, int row, int col, int pos)
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
			Grid->SetTypeComboBox(&CellType, 0, m_strCombo);
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

void CDialog_LotStart::GridData(UINT nID, int row, int col, CString data)
{
	TSpread *Grid = (TSpread*)GetDlgItem(nID);

	Grid->SetValue(col, row, data);

	Grid = NULL;
	delete Grid;
}

void CDialog_LotStart::GridFont(UINT nID, int row, int col, int size)
{
	TSpread *Grid = (TSpread *)GetDlgItem(nID);
	HFONT font;
	CString strTemp;
	
	font = CreateFont(size, 0, 0, 0, FW_BOLD, 0, 0, 0, 0, 0, 0, 0, 0, "돋음체");
	Grid->SetFont(col, row, font, TRUE);

	Grid = NULL;
	delete Grid;
}

void CDialog_LotStart::GridMerge(UINT nID, int srow, int scol, int nrow, int ncol)
{
	TSpread *Grid = (TSpread*)GetDlgItem(nID);
	
	Grid->AddCellSpan(scol, srow, ncol, nrow);

	Grid = NULL;
	delete Grid;
}



void CDialog_LotStart::OnDestroy() 
{
	StandardDialog::OnDestroy();
	
	// TODO: Add your message handler code here

	st_handler.cwnd_lotstart = NULL;
	
}


void CDialog_LotStart::OnLotDisplay() 
{
	TSpread *Grid = (TSpread*)GetDlgItem(IDC_CUSTOM_LOT_INFO);
	char strLotId[100],strPartId[100],strType[100],strDen[100];
	Grid->GetData( 2, 2, strLotId );
	GridData( IDC_CUSTOM_LOT_INFO, 2, 2, strLotId );
	Grid->GetData( 2, 3, strPartId );
	GridData( IDC_CUSTOM_LOT_INFO, 3, 2, strPartId );
	Grid->GetData( 2, 4, strType );
	GridData( IDC_CUSTOM_LOT_INFO, 4, 2, strType );
	Grid->GetData( 2, 5, strDen );
	GridData( IDC_CUSTOM_LOT_INFO, 5, 2, strDen );

	GridControl(IDC_CUSTOM_LOT_INFO, STATIC, 2, 2, 0);
	GridControl(IDC_CUSTOM_LOT_INFO, STATIC, 3, 2, 0);

	//m_strCombo = "NONE";
	//m_strCombo += "\t4GB\t8G/16G\t32G\t64G";
	//GridControl(IDC_CUSTOM_LOT_INFO, COMBO, 4, 2, 0);
	//GridControl(IDC_CUSTOM_LOT_INFO, STATIC, 5, 2, 0);

	/*

	int n_response, nDvc;
	CString strLot, strMsg, strPart, strDvc;				
	CDialog_Infor infor_dlg;
	CDialog_Message	msg_dlg;
	
	m_edit_lot_id.GetWindowText(strLot);
	m_edit_part_id.GetWindowText(strPart);
	GetDlgItemText(IDC_EDIT_DEVICE_COUNT,strDvc);	
	
	if(st_work.b_lot_start_flag == TRUE && st_var.nMlctSelect == 0)
	{
		st_other.str_fallacy_msg = _T("작업중인 Lot 정보는 수정이 불가능합니다.");
		n_response = msg_dlg.DoModal();

		if(n_response == IDOK)
		{
			PublicFunction.MessageDisplay("작업중인 Lot 정보는 수정이 불가능합니다.");
		}
		return;
	}
	else
	{
		if(st_work.nNameCount == 6)
		{
			st_other.str_fallacy_msg = _T("연속 Lot 작업은 최대 5번 입니다.");
			n_response = msg_dlg.DoModal();
			
			if(n_response == IDOK)
			{
				PublicFunction.MessageDisplay("연속 Lot 작업은 최대 5번 입니다.");
			}
			return;
		}
		else if(strLot == "" || strPart == "" || strDvc == "")
		{
			st_other.str_fallacy_msg = _T("LotID, PartID, Device Count를 입력해주세요");
			n_response = msg_dlg.DoModal();
			
			if(n_response == IDOK)
			{
				PublicFunction.MessageDisplay("LotID, PartID, Device Count를 입력해주세요");
			}
			return;
		}
		else
		{
			strMsg.Format("작업파일 " + strLot + " 를 새로 생성하시겠습니까??");	
			st_other.str_confirm_msg = _T(strMsg);	
			n_response = infor_dlg.DoModal();
			
			if(n_response == IDOK)
			{
				//st_work.nDeviceType_B = st_work.nDeviceType;
					
				nDvc = atoi((const char *)strDvc);

				g_Vector vec_info(strPart, strLot, nDvc, st_work.nDeviceType_B, st_work.strSelect, st_work.bLotEndInfo); // 20130924 kks
				vec.push_back(vec_info);
				
				st_work.nNameCount++;

				st_work.bLotInfoSend = FALSE;

				m_edit_lot_id.SetWindowText("");
				m_edit_part_id.SetWindowText("");
				SetDlgItemText(IDC_EDIT_DEVICE_COUNT,"");
				//////////
			}
			else
			{
			
			}	
		}
	}*/
}

void CDialog_LotStart::OnLotStart() 
{
	// TODO: Add your control notification handler code here
	
}
