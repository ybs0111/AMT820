// DeviceEditdlg.cpp : implementation file
//

#include "stdafx.h"
#include "HANDLER.h"
#include "DeviceEditdlg.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDeviceEditdlg dialog


CDeviceEditdlg::CDeviceEditdlg(CWnd* pParent /*=NULL*/)
	: StandardDialog(CDeviceEditdlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDeviceEditdlg)
	//}}AFX_DATA_INIT
	m_caption_main = "Tray editor(Heat sink)"; 
}


void CDeviceEditdlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDeviceEditdlg)
	DDX_Control(pDX, IDC_BTN_DVC_YES, m_btn_dvc_yes);
	DDX_Control(pDX, IDC_BTN_DVC_NONE, m_btn_dvc_none);
	DDX_Control(pDX, IDC_BTN_DVC_ALL_YES, m_btn_dvc_all_Yes);
	DDX_Control(pDX, IDC_BTN_DVC_ALL_NONE, m_btn_dvc_all_None);
	DDX_Control(pDX, IDC_CBO_SELECT_STAGE, m_cboSelectStage);
	DDX_Control(pDX, IDC_TRAYMODIFYDEVICE, m_ctrlTrayModifyDevice);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDeviceEditdlg, StandardDialog)
	//{{AFX_MSG_MAP(CDeviceEditdlg)
	ON_CBN_SELCHANGE(IDC_CBO_SELECT_STAGE, OnSelchangeCboSelectStage)
	ON_BN_CLICKED(IDC_DVC_NONE, OnDvcNone)
	ON_BN_CLICKED(IDC_DVC_ALL_YES, OnDvcAllYes)
	ON_BN_CLICKED(IDC_DVC_YES, OnDvcYes)
	ON_BN_CLICKED(IDC_DVC_ALL_NONE, OnDvcAllNone)
	ON_BN_CLICKED(IDC_BTN_DVC_NONE, OnBtnDvcNone)
	ON_BN_CLICKED(IDC_BTN_DVC_YES, OnBtnDvcYes)
	ON_BN_CLICKED(IDC_BTN_DVC_ALL_NONE, OnBtnDvcAllNone)
	ON_BN_CLICKED(IDC_BTN_DVC_ALL_YES, OnBtnDvcAllYes)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDeviceEditdlg message handlers
BOOL CDeviceEditdlg::OnInitDialog() 
{
	StandardDialog::OnInitDialog();
	
	RECT rect;
	char sz[100];

	mp_tray_font = NULL;
	mp_tray_font = new CFont;
	mp_tray_font->CreateFont(15,0, 0, 0, FW_BOLD, 0, 0, 0, 0, 0, 0, 0, 0, "Arial");


	OnTray_Label_Set();
	OnInitButtonSet(); //kwlee 2017.0425
	m_nCol = st_recipe.nHsTrayX;
	m_nRow = st_recipe.nHsTrayY;

	m_ctrlTrayModifyDevice.SetRows(m_nRow);
	m_ctrlTrayModifyDevice.SetCols(0,m_nCol);
	m_ctrlTrayModifyDevice.GetWindowRect(&rect);
	m_nWidth = (int)((rect.right - rect.left) / m_nCol * 15);   
	m_nHeight = (int)((rect.bottom - rect.top) / m_nRow * 14.5);
	
	nPoint[0] = (int)(m_nWidth / 15);
	nPoint[1] = (int)(m_nHeight / 14.5);


	sprintf(sz,"%d-%d   %d-%d",nPoint[0],nPoint[1],m_nWidth,m_nHeight);
	nSelStage = 0;  
	m_cboSelectStage.SetCurSel(nSelStage);

	m_ctrlTrayModifyDevice.SetColWidth(-1,0,m_nWidth);
	m_ctrlTrayModifyDevice.SetRowHeight(-1,m_nHeight);

	nSelChip = 0;   //1:None 2: Chip  3:All Blank 4:Chip 
	nSelChip = 1;
	SetCmdColor(nSelChip);
	nMouseDown = 0;

	m_msg_dvc_no.EnableWindow(TRUE);
	m_msg_dvc_yes.EnableWindow(TRUE);
	m_msg_all_yes.EnableWindow(TRUE);
	m_msg_all_none.EnableWindow(TRUE);

	LoadTray(nSelStage);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

//kwlee 2017.0425
void CDeviceEditdlg::OnInitButtonSet()
{
	short	shBtnColor = 30;

	m_btn_dvc_all_None.SetColor(CButtonST::BTNST_COLOR_BK_IN, RGB(219, 255, 210));
	m_btn_dvc_all_None.SetColor(CButtonST::BTNST_COLOR_BK_OUT, RGB(250, 255, 244));
	m_btn_dvc_all_None.SetColor(CButtonST::BTNST_COLOR_BK_FOCUS, RGB(250, 255, 244));
	m_btn_dvc_all_None.SetColor(CButtonST::BTNST_COLOR_FG_IN, BLUE_C);
	m_btn_dvc_all_None.SetColor(CButtonST::BTNST_COLOR_FG_OUT,BLACK_C );
	//m_btn_Tray1_Lock.SetColor(CButtonST::BTNST_COLOR_FG_FOCUS, BLUE_C);
	m_btn_dvc_all_None.SetFont(mp_tray_font);
	m_btn_dvc_all_None.SetTooltipText("All_None");
		
	m_btn_dvc_all_Yes.SetColor(CButtonST::BTNST_COLOR_BK_IN, RGB(219, 255, 210));
	m_btn_dvc_all_Yes.SetColor(CButtonST::BTNST_COLOR_BK_OUT, RGB(250, 255, 244));
	m_btn_dvc_all_Yes.SetColor(CButtonST::BTNST_COLOR_BK_FOCUS, RGB(250, 255, 244));
	m_btn_dvc_all_Yes.SetColor(CButtonST::BTNST_COLOR_FG_IN, BLUE_C);
	m_btn_dvc_all_Yes.SetColor(CButtonST::BTNST_COLOR_FG_OUT, BLACK_C);
	//m_btn_Tray1_Unlock.SetColor(CButtonST::BTNST_COLOR_FG_FOCUS, BLUE_C);
	m_btn_dvc_all_Yes.SetFont(mp_tray_font);
	m_btn_dvc_all_Yes.SetTooltipText("All_Yes");
	
	m_btn_dvc_none.SetColor(CButtonST::BTNST_COLOR_BK_IN, RGB(219, 255, 210));
	m_btn_dvc_none.SetColor(CButtonST::BTNST_COLOR_BK_OUT, RGB(250, 255, 244));
	m_btn_dvc_none.SetColor(CButtonST::BTNST_COLOR_BK_FOCUS, RGB(250, 255, 244));
	m_btn_dvc_none.SetColor(CButtonST::BTNST_COLOR_FG_IN, BLUE_C);
	m_btn_dvc_none.SetColor(CButtonST::BTNST_COLOR_FG_OUT,BLACK_C );
	//m_btn_Tray1_Lock.SetColor(CButtonST::BTNST_COLOR_FG_FOCUS, BLUE_C);
	m_btn_dvc_none.SetFont(mp_tray_font);
	m_btn_dvc_none.SetTooltipText("None");
		
	m_btn_dvc_yes.SetColor(CButtonST::BTNST_COLOR_BK_IN, RGB(219, 255, 210));
	m_btn_dvc_yes.SetColor(CButtonST::BTNST_COLOR_BK_OUT, RGB(250, 255, 244));
	m_btn_dvc_yes.SetColor(CButtonST::BTNST_COLOR_BK_FOCUS, RGB(250, 255, 244));
	m_btn_dvc_yes.SetColor(CButtonST::BTNST_COLOR_FG_IN, BLUE_C);
	m_btn_dvc_yes.SetColor(CButtonST::BTNST_COLOR_FG_OUT, BLACK_C);
	//m_btn_Tray1_Unlock.SetColor(CButtonST::BTNST_COLOR_FG_FOCUS, BLUE_C);
	m_btn_dvc_yes.SetFont(mp_tray_font);
	m_btn_dvc_yes.SetTooltipText("DVC_YEs");

}
void CDeviceEditdlg::OnTray_Label_Set()
{
	m_msg_dvc_no.SetFont(mp_tray_font);
	m_msg_dvc_no.SetWindowText(_T("NONE"));
	m_msg_dvc_no.SetCenterText();
	m_msg_dvc_no.SetColor(WHITE_C);
	m_msg_dvc_no.SetGradientColor(RGB(255, 255, 0));
	m_msg_dvc_no.SetTextColor(RGB(255, 255, 255));
	
	m_msg_dvc_yes.SetFont(mp_tray_font);
	m_msg_dvc_yes.SetWindowText(_T("YES"));
	m_msg_dvc_yes.SetCenterText();
	m_msg_dvc_yes.SetColor(GRAY);
	m_msg_dvc_yes.SetGradientColor(RGB(0, 0, 255));
	m_msg_dvc_yes.SetTextColor(RGB(255, 255, 255));
	
	m_msg_all_none.SetFont(mp_tray_font);
	m_msg_all_none.SetWindowText(_T("ALL NONE"));
	m_msg_all_none.SetCenterText();
	m_msg_all_none.SetColor(WHITE_C);
	m_msg_all_none.SetGradientColor(RGB(255, 255, 0));
	m_msg_all_none.SetTextColor(RGB(255, 255, 255));
	
	m_msg_all_yes.SetFont(mp_tray_font);
	m_msg_all_yes.SetWindowText(_T("ALL YES"));
	m_msg_all_yes.SetCenterText();
	m_msg_all_yes.SetColor(GRAY);
	m_msg_all_yes.SetGradientColor(RGB(0, 0, 255));
	m_msg_all_yes.SetTextColor(RGB(255, 255, 255));
	

}

void CDeviceEditdlg::OnSelchangeCboSelectStage() 
{
	nSelStage = (int)m_cboSelectStage.GetCurSel();

	m_msg_dvc_no.EnableWindow(TRUE);
	m_msg_dvc_yes.EnableWindow(TRUE);
	m_msg_all_none.EnableWindow(TRUE);
	m_msg_all_yes.EnableWindow(TRUE);

	if(nSelStage == 0 || nSelStage == 1){
	}
	if(nSelStage == 2){
	}
	LoadTray(nSelStage);

}

BEGIN_EVENTSINK_MAP(CDeviceEditdlg, CDialog)
    //{{AFX_EVENTSINK_MAP(CDeviceEditdlg)
	ON_EVENT(CDeviceEditdlg, IDC_TRAYMODIFYDEVICE, -600 /* Click */, OnClickTraymodifydevice, VTS_NONE)
	ON_EVENT(CDeviceEditdlg, IDC_TRAYMODIFYDEVICE, -605 /* MouseDown */, OnMouseDownTraymodifydevice, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CDeviceEditdlg, IDC_TRAYMODIFYDEVICE, -607 /* MouseUp */, OnMouseUpTraymodifydevice, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CDeviceEditdlg, IDC_TRAYMODIFYDEVICE, -606 /* MouseMove */, OnMouseMoveTraymodifydevice, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

void CDeviceEditdlg::OnClickTraymodifydevice() 
{
	//int nCol,nRow;
/*
	nCol = (int)m_ctrlTrayModifyDevice.GetCol();
	nRow = (int)m_ctrlTrayModifyDevice.GetRow();

	if(!nSelChip) return;
	
	switch(nSelChip) {  //1:None 2: Chip  3:All Blank 4:Chip 
		case 1:	m_ctrlTrayModifyDevice.SetCellBackColor(WHITE); break;
		case 2:	
			if((nSelStage == 0) || (nSelStage == 1) || (nSelStage == 2)){
				m_ctrlTrayModifyDevice.SetCellBackColor(DGRAY);
			}
			else if(nSelStage == 3){
				m_ctrlTrayModifyDevice.SetCellBackColor(LRED);
			}
			break;
	}*/
}

void CDeviceEditdlg::OnMouseDownTraymodifydevice(short Button, short Shift, long x, long y) 
{
	int nCol,nRow;

	nCol = (int)m_ctrlTrayModifyDevice.GetCol();
	nRow = (int)m_ctrlTrayModifyDevice.GetRow();

	if(!nSelChip) return;
	nMouseDown = 1;
	switch(nSelChip) {  //1:None 2: Chip  3:All Blank 4:Chip 5:Good 6:Error
		case 1 : 
			m_ctrlTrayModifyDevice.SetCellBackColor(WHITE_C); 
			break;
		
		case 2 :
			if((nSelStage == 0) || (nSelStage == 1) || (nSelStage == 2)){
				m_ctrlTrayModifyDevice.SetCellBackColor(GRAY);
			}
			else if(nSelStage == 3){
				m_ctrlTrayModifyDevice.SetCellBackColor(GRAY);  //LRED
			}
			break;

		case 5 :
			if((nSelStage == 0) || (nSelStage == 1) || (nSelStage == 2)){
				m_ctrlTrayModifyDevice.SetCellBackColor(GREEN_D);
			}
			break;

		case 6 :
			if((nSelStage == 2) || (nSelStage == 3)){
				m_ctrlTrayModifyDevice.SetCellBackColor(RED_L);
			}
			break;
	}	
}

void CDeviceEditdlg::OnMouseUpTraymodifydevice(short Button, short Shift, long x, long y) 
{
	nMouseDown = 0;	
	
}

void CDeviceEditdlg::OnMouseMoveTraymodifydevice(short Button, short Shift, long x, long y) 
{
	char str[100];
	sprintf(str,"Mouse Point : %d - %d", x / nPoint[0]+1, /*9 /*TrayY*/ m_nRow - y /nPoint[1] );

	SetDlgItemText(IDC_STATIC_MOUSEPOS,str);
	if(nSelChip) {
		m_ctrlTrayModifyDevice.SetCol(x /nPoint[0]);
		m_ctrlTrayModifyDevice.SetRow(y /nPoint[1]);
	}

	if(nMouseDown == 0 ) return; //if(!nMouseDown) return;
	switch(nSelChip) {
		case 1 : m_ctrlTrayModifyDevice.SetCellBackColor(WHITE_C); break;
		case 2 : 
			if(nSelStage == 0){
				m_ctrlTrayModifyDevice.SetCellBackColor(GRAY);
			}
			else if(nSelStage == 1){
				m_ctrlTrayModifyDevice.SetCellBackColor(GRAY);
			}
			else if(nSelStage == 2){
				m_ctrlTrayModifyDevice.SetCellBackColor(GRAY);
			}
			else if(nSelStage == 3){
				m_ctrlTrayModifyDevice.SetCellBackColor(GRAY);  //LRED
			}
			break;
		case 3 : m_ctrlTrayModifyDevice.SetCellBackColor(WHITE_C); break;
		case 4 : 
			if(nSelStage == 0){
				m_ctrlTrayModifyDevice.SetCellBackColor(GRAY);
			}
			else if(nSelStage == 1){
				m_ctrlTrayModifyDevice.SetCellBackColor(GRAY);
			}
			else if(nSelStage == 2){
				m_ctrlTrayModifyDevice.SetCellBackColor(GRAY);
			}
			else if(nSelStage == 3){
				m_ctrlTrayModifyDevice.SetCellBackColor(RED_L);
			}
		case 5 :
			if((nSelStage == 0) || (nSelStage == 1) || (nSelStage == 2)){
				m_ctrlTrayModifyDevice.SetCellBackColor(GREEN_D);
			}			
			break;
		case 6 :
			if((nSelStage == 2) || (nSelStage == 3)){
				m_ctrlTrayModifyDevice.SetCellBackColor(RED_L);
			}
			break;
	}	
}

void CDeviceEditdlg::OnDvcNone() 
{
	nSelChip = 1;
	SetCmdColor(nSelChip);		
}


void CDeviceEditdlg::OnDvcYes() 
{
	nSelChip = 2;
	SetCmdColor(nSelChip);	
}

void CDeviceEditdlg::OnDvcAllNone() 
{
	int i,j;// 
// 	int i,j;
// 	nSelChip = 3;
// 	SetCmdColor(nSelChip);
// 	
// 	for( i = 0; i < m_nRow/*TrayY*/; i++) {
// 		for(j = 0; j < m_nCol/*TrayX*/; j++) {
// 			m_tempTrayArray[nSelStage][i][j] = CTL_NO;
// 			m_ctrlTrayModifyDevice.SetCol(j);
// 			m_ctrlTrayModifyDevice.SetRow(i);
// 			m_ctrlTrayModifyDevice.SetCellBackColor(WHITE_C);
// 		}
// 	}	

	nSelChip = 3;
	
	SetCmdColor(nSelChip);
	
	int chipMode;
	
	if((nSelStage == 0) || (nSelStage == 1) || (nSelStage == 2)){
		chipMode = CTL_NO;
	}
	//else if(nSelStage == 3){
	//	chipMode = BhPnPChip::TRAY_CHIP_BURN_FAIL;
	//	nRow = m_nERow;
	//	nCol = m_nECol;
	//}
	
	for( i = 0; i < m_nRow/*TrayY*/; i++) {
		for(j = 0; j < m_nCol/*TrayX*/; j++) {
			m_tempTrayArray[nSelStage][i][j] = chipMode;
			m_ctrlTrayModifyDevice.SetCol(j);
			m_ctrlTrayModifyDevice.SetRow(i);
			
			if(nSelStage == 0){
				m_ctrlTrayModifyDevice.SetCellBackColor(WHITE_C);
			}
			else if(nSelStage == 1){
				m_ctrlTrayModifyDevice.SetCellBackColor(GRAY);
			}
			else if(nSelStage == 2){
				m_ctrlTrayModifyDevice.SetCellBackColor(GRAY);
			}			
			else if(nSelStage == 3){
				m_ctrlTrayModifyDevice.SetCellBackColor(RED_L);
			}			
		}
	}				
}


void CDeviceEditdlg::OnDvcAllYes() 
{
	int i,j;
	nSelChip = 4;
	
	SetCmdColor(nSelChip);
	
	int chipMode;
	
	if((nSelStage == 0) || (nSelStage == 1) || (nSelStage == 2)){
		chipMode = CTL_YES;
	}
	//else if(nSelStage == 3){
	//	chipMode = BhPnPChip::TRAY_CHIP_BURN_FAIL;
	//	nRow = m_nERow;
	//	nCol = m_nECol;
	//}
	
	for( i = 0; i < m_nRow/*TrayY*/; i++) {
		for(j = 0; j < m_nCol/*TrayX*/; j++) {
			m_tempTrayArray[nSelStage][i][j] = chipMode;
			m_ctrlTrayModifyDevice.SetCol(j);
			m_ctrlTrayModifyDevice.SetRow(i);
			
			if(nSelStage == 0){
				m_ctrlTrayModifyDevice.SetCellBackColor(GRAY);
			}
			else if(nSelStage == 1){
				m_ctrlTrayModifyDevice.SetCellBackColor(GRAY);
			}
			else if(nSelStage == 2){
				m_ctrlTrayModifyDevice.SetCellBackColor(GRAY);
			}			
			else if(nSelStage == 3){
				m_ctrlTrayModifyDevice.SetCellBackColor(RED_L);
			}			
		}
	}			
	
}


void CDeviceEditdlg::SetCmdColor(int selchip)
{
	m_msg_dvc_no.SetColor(WHITE_C);	
	m_msg_dvc_yes.SetColor(GRAY);
	m_msg_all_none.SetColor(WHITE_C);
	m_msg_all_yes.SetColor(GRAY);
	if(selchip == 1){
		m_msg_dvc_no.SetColor( YELLOW_C);
	}
	else if(selchip == 2) {
		m_msg_dvc_yes.SetColor(YELLOW_C);
	}
	else if(selchip == 3){
		m_msg_all_none.SetColor(YELLOW_C);
	}
	else if(selchip == 4) {
		m_msg_all_yes.SetColor(YELLOW_C);
	}
}

void CDeviceEditdlg::LoadTray(int sel)
{
	int i,j;
	
	InitShowErrorTray(m_nRow,m_nCol);
	for( i = 0; i < m_nRow/*TrayY*/; i++) {
		for(j = 0; j < m_nCol/*TrayX*/; j++) {
			m_bTrayArray[sel][i][j] = st_tray_info[THD_LD_HEATSINK_BUFF].st_dvc_info[sel][i][j].nYesNo;
			m_tempTrayArray[sel][i][j] = m_bTrayArray[sel][i][j];  //sel:0 UnLoad sel:1 Load Sel:2 Error Sel:3 Mark
		}
	}

	for( i = 0; i < m_nRow/*TrayY*/; i++) {
		for(j = 0; j < m_nCol/*TrayX*/; j++) {

			m_ctrlTrayModifyDevice.SetCol(j);
			m_ctrlTrayModifyDevice.SetRow(m_nRow/*TrayY*/ - i - 1);

			if (st_tray_info[THD_LD_HEATSINK_BUFF].st_dvc_info[sel][i][j].nYesNo == CTL_NO) {
				m_ctrlTrayModifyDevice.SetCellBackColor(WHITE_C);
			} 
			else if (st_tray_info[THD_LD_HEATSINK_BUFF].st_dvc_info[sel][i][j].nYesNo == CTL_YES) {
				m_ctrlTrayModifyDevice.SetCellBackColor(GRAY);
			}
		}
	}	
}

void CDeviceEditdlg::InitShowErrorTray(int Row, int Col)
{
	RECT rect;
	char sz[100];

	m_ctrlTrayModifyDevice.SetRows(Row);
	m_ctrlTrayModifyDevice.SetCols(0,Col);
	m_ctrlTrayModifyDevice.GetWindowRect(&rect);
	
	m_nWidth = (int)((rect.right - rect.left) / m_nCol * 15);   
	m_nHeight = (int)((rect.bottom - rect.top) / m_nRow * 14.5);
	
	nPoint[0] = (int)(m_nWidth / 15);
	nPoint[1] = (int)(m_nHeight / 14.5);

	if(nSelStage == 3 && Col == 2){
		m_nWidth = (int)((rect.right - rect.left) / Row * 22);  //15  22
		m_nHeight = (int)((rect.bottom - rect.top) / Row * 14.5);  //14.5

		nPoint[0] = (int)(m_nWidth / 22);  //15
		nPoint[1] = (int)(m_nHeight / 14.5);  //14.6	
	}


	sprintf(sz,"%d-%d   %d-%d",nPoint[0],nPoint[1],m_nWidth,m_nHeight);
	m_cboSelectStage.SetCurSel(nSelStage);

	m_ctrlTrayModifyDevice.SetColWidth(-1,0,m_nWidth);
	m_ctrlTrayModifyDevice.SetRowHeight(-1,m_nHeight);

	nSelChip = 1;   //1:None 2: Chip  3:All Blank 4:Chip 
	SetCmdColor(nSelChip);
	nMouseDown = 0;
}

void CDeviceEditdlg::OnOK() 
{
	int i,j;
	unsigned long lColor;
	
	for( i = 0; i < m_nRow/*TrayY*/; i++) {
		for(j = 0; j < m_nCol/*TrayX*/; j++) {
			m_ctrlTrayModifyDevice.SetCol(j);
			m_ctrlTrayModifyDevice.SetRow(m_nRow/*TrayY*/-i-1);

			lColor = m_ctrlTrayModifyDevice.GetCellBackColor();

			if(lColor == WHITE_C) {
				m_tempTrayArray[nSelStage][m_nRow - i - 1][j] = CTL_NO;
			}
			else if(((nSelStage == 0) || (nSelStage == 1) || (nSelStage == 2)) && (lColor == GRAY)){
				m_tempTrayArray[nSelStage][m_nRow - i - 1][j] = CTL_YES;
			}
			else if(((nSelStage == 0) || (nSelStage == 1) || (nSelStage == 2)) && (lColor == GREEN_D)){
				m_tempTrayArray[nSelStage][m_nRow - i - 1][j] = CTL_YES;
			}
			else if(((nSelStage == 2) || (nSelStage == 3)) && (lColor == RED_L)){
				m_tempTrayArray[nSelStage][m_nRow - i - 1][j] = CTL_YES;
			}
		}
	}
	if(nSelStage == 0) {  //PnP1
		CString tmp;
		tmp.LoadString(IDS_PNP_STAGE);
		if(MessageBox(tmp,"CDIMM Handler",MB_ICONQUESTION | MB_YESNO) == IDYES) {
			for( i = 0; i < m_nRow/*TrayY*/; i++) {
				for(j = 0; j < m_nCol/*TrayX*/; j++) {
					m_bTrayArray[0][i][j] = m_tempTrayArray[0][i][j];
					st_tray_info[THD_LD_HEATSINK_BUFF].st_dvc_info[nSelStage][i][j].nYesNo = m_bTrayArray[0][i][j];					
				}
			}
		}
	}
	else if(nSelStage == 1) {  //PnP2
		CString tmp;
		tmp.LoadString(IDS_SAVELOADERSTAGE);
		if(MessageBox(tmp,"CDIMM Handler",MB_ICONQUESTION | MB_YESNO) == IDYES) {
			for( i = 0; i < m_nRow/*TrayY*/; i++) {
				for(j = 0; j < m_nCol/*TrayX*/; j++) {
					m_bTrayArray[1][i][j] = m_tempTrayArray[1][i][j];
					st_tray_info[THD_LD_HEATSINK_BUFF].st_dvc_info[nSelStage][i][j].nYesNo = m_bTrayArray[1][i][j];					
				}
			}
		}
	}
}




BOOL CDeviceEditdlg::DestroyWindow() 
{
	if (mp_tray_font != NULL)
	{
		delete mp_tray_font;
		mp_tray_font = NULL;
	}
	
	return StandardDialog::DestroyWindow();
}

void CDeviceEditdlg::OnBtnDvcNone() 
{

	//kwlee 2017.0425
	if(!m_btn_dvc_none.GetCheck())
	{
		m_btn_dvc_none.SetCheck(TRUE);
		return;
	}
	
	if(m_btn_dvc_yes.GetCheck())
	{
		m_btn_dvc_yes.SetCheck(FALSE);
	}

	if(m_btn_dvc_all_Yes.GetCheck())
	{
		m_btn_dvc_all_Yes.SetCheck(FALSE);
	}
	
	if(m_btn_dvc_all_None.GetCheck())
	{
		m_btn_dvc_all_None.SetCheck(FALSE);	
	}
	nSelChip = 1;
	SetCmdColor(nSelChip);		
	
}

void CDeviceEditdlg::OnBtnDvcYes() 
{
	//kwlee 2017.0425
	if(!m_btn_dvc_yes.GetCheck())
	{
		m_btn_dvc_yes.SetCheck(TRUE);
		return;
	}
	
	if(m_btn_dvc_none.GetCheck())
	{
		m_btn_dvc_none.SetCheck(FALSE);	
	}

	if(m_btn_dvc_all_Yes.GetCheck())
	{
		m_btn_dvc_all_Yes.SetCheck(FALSE);
	}
	
	if(m_btn_dvc_all_None.GetCheck())
	{
		m_btn_dvc_all_None.SetCheck(FALSE);	
	}


	nSelChip = 2;
	SetCmdColor(nSelChip);	
}

void CDeviceEditdlg::OnBtnDvcAllNone() 
{
	int i,j; 
	
	//kwlee 2017.0425
	if(!m_btn_dvc_all_None.GetCheck())
	{
		m_btn_dvc_all_None.SetCheck(TRUE);
		return;
	}
	if(m_btn_dvc_all_Yes.GetCheck())
	{
		m_btn_dvc_all_Yes.SetCheck(FALSE);
	}
	
	if(m_btn_dvc_none.GetCheck())
	{
		m_btn_dvc_none.SetCheck(FALSE);
	}

	if(m_btn_dvc_yes.GetCheck())
	{
		m_btn_dvc_yes.SetCheck(FALSE);
	}
	nSelChip = 3;
	SetCmdColor(nSelChip);
	
	int chipMode;
	
	if((nSelStage == 0) || (nSelStage == 1) || (nSelStage == 2)){
		chipMode = CTL_NO;
	}
	for( i = 0; i < m_nRow/*TrayY*/; i++) {
		for(j = 0; j < m_nCol/*TrayX*/; j++) {
			m_tempTrayArray[nSelStage][i][j] = chipMode;
			m_ctrlTrayModifyDevice.SetCol(j);
			m_ctrlTrayModifyDevice.SetRow(i);
			
			if(nSelStage == 0){
				m_ctrlTrayModifyDevice.SetCellBackColor(WHITE_C);
			}
			else if(nSelStage == 1){
				m_ctrlTrayModifyDevice.SetCellBackColor(GRAY);
			}
			else if(nSelStage == 2){
				m_ctrlTrayModifyDevice.SetCellBackColor(GRAY);
			}			
			else if(nSelStage == 3){
				m_ctrlTrayModifyDevice.SetCellBackColor(RED_L);
			}			
		}
	}	
}

void CDeviceEditdlg::OnBtnDvcAllYes() 
{
	int i,j;
	//kwlee 2017.0425	
	if(!m_btn_dvc_all_Yes.GetCheck())
	{
		m_btn_dvc_all_Yes.SetCheck(TRUE);
		return;
	}
	
	if(m_btn_dvc_all_None.GetCheck())
	{
		m_btn_dvc_all_None.SetCheck(FALSE);
	}
	
	if(m_btn_dvc_none.GetCheck())
	{
		m_btn_dvc_none.SetCheck(FALSE);
	}

	if(m_btn_dvc_yes.GetCheck())
	{
		m_btn_dvc_yes.SetCheck(FALSE);
	}
	nSelChip = 4;
	
	SetCmdColor(nSelChip);
	
	int chipMode;
	
	if((nSelStage == 0) || (nSelStage == 1) || (nSelStage == 2)){
		chipMode = CTL_YES;
	}
	//else if(nSelStage == 3){
	//	chipMode = BhPnPChip::TRAY_CHIP_BURN_FAIL;
	//	nRow = m_nERow;
	//	nCol = m_nECol;
	//}
	
	for( i = 0; i < m_nRow/*TrayY*/; i++) {
		for(j = 0; j < m_nCol/*TrayX*/; j++) {
			m_tempTrayArray[nSelStage][i][j] = chipMode;
			m_ctrlTrayModifyDevice.SetCol(j);
			m_ctrlTrayModifyDevice.SetRow(i);
			
			if(nSelStage == 0){
				m_ctrlTrayModifyDevice.SetCellBackColor(GRAY);
			}
			else if(nSelStage == 1){
				m_ctrlTrayModifyDevice.SetCellBackColor(GRAY);
			}
			else if(nSelStage == 2){
				m_ctrlTrayModifyDevice.SetCellBackColor(GRAY);
			}			
			else if(nSelStage == 3){
				m_ctrlTrayModifyDevice.SetCellBackColor(RED_L);
			}			
		}
	}	
}
