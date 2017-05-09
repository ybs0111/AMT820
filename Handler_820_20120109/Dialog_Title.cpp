// Dialog_Title.cpp : implementation file
//

#include "stdafx.h"
#include "HANDLER.h"
#include "Dialog_Title.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDialog_Title dialog


CDialog_Title::CDialog_Title()
{
	//{{AFX_DATA_INIT(CDialog_Title)
	//}}AFX_DATA_INIT
}


void CDialog_Title::DoDataExchange(CDataExchange* pDX)
{
	CInitDialogBar::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialog_Title)
	DDX_Control(pDX, IDC_MSG_BARCODE_MODE_INFO, m_msg_Barcode_mode_info);
	DDX_Control(pDX, IDC_MSG_7387_MODE_INFO, m_msg_7387_mode_info);
	DDX_Control(pDX, IDC_MSG_3874_MODE_INFO, m_msg_3874_mode_info);
	DDX_Control(pDX, IDC_MSG_MODEL_INFO, m_msg_model_info);
	DDX_Control(pDX, IDC_MSG_FILE_INFO, m_msg_file_info);
	DDX_Control(pDX, IDC_MSG_PRESS_MODE_INFO, m_msg_press_mode_info);
	DDX_Control(pDX, IDC_MSG_INTERFACE_MODE_INFO, m_msg_interface_mode_info);
	DDX_Control(pDX, IDC_MSG_DEVICE_MODE_INFO, m_msg_device_mode_info);
	DDX_Control(pDX, IDC_MSG_STATUS_INFO, m_msg_status_info);
	DDX_Control(pDX, IDC_GIF_TITLE_LOGO, m_gif_title_logo);
	DDX_Control(pDX, IDC_MSG_FORM_NAME, m_msg_form_name);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDialog_Title, CInitDialogBar)
	//{{AFX_MSG_MAP(CDialog_Title)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_STATUS_CHANGE,OnStatusDataUpdate)  // Ÿ��Ʋ �� ȭ������ ���۵� �޽��� ��½�Ű�� ���� ����� ���� �޽��� �߰� 
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialog_Title message handlers

BOOL CDialog_Title::OnInitDialogBar() 
{
	CInitDialogBar::OnInitDialogBar();
	
	/* ************************************************************************** */
	/* ��Ʈ�ѿ� ������ ��Ʈ �����Ѵ�                                              */
	/* ************************************************************************** */
	mp_title_font = NULL;
	mp_title_font = new CFont;
	mp_title_font->CreateFont(15,0,0,0,900,0,0,0,0,0,0,ANTIALIASED_QUALITY,0,"MS Sans Serif");
	/* ************************************************************************** */
	
	OnTitle_Lable_Set();
	OnTitle_Picture_Set();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}



void CDialog_Title::OnTitle_Lable_Set()
{
	m_msg_form_name.SetTextFont(mp_title_font);
	m_msg_form_name.SetColor(RGB(181, 182, 140), RGB(0, 0, 0));
	m_msg_form_name.SetTextAlign(DT_LEFT);

	m_msg_status_info.SetFont(mp_title_font);
	m_msg_status_info.SetCenterText();
	m_msg_status_info.SetColor(RGB(255,255,255));
	m_msg_status_info.SetGradientColor(RGB(0,255,0));
	m_msg_status_info.SetTextColor(RGB(255,255,255));

	m_msg_device_mode_info.SetFont(mp_title_font);
	m_msg_device_mode_info.SetCenterText();
	m_msg_device_mode_info.SetColor(RGB(0,0,255));
	m_msg_device_mode_info.SetGradientColor(RGB(0,0,0));
	m_msg_device_mode_info.SetTextColor(RGB(255,255,255));

	m_msg_interface_mode_info.SetFont(mp_title_font);
	m_msg_interface_mode_info.SetCenterText();
	m_msg_interface_mode_info.SetColor(RGB(0,0,255));
	m_msg_interface_mode_info.SetGradientColor(RGB(0,0,0));
	m_msg_interface_mode_info.SetTextColor(RGB(255,255,255));

	m_msg_press_mode_info.SetFont(mp_title_font);
	m_msg_press_mode_info.SetCenterText();
	m_msg_press_mode_info.SetColor(RGB(0,0,255));
	m_msg_press_mode_info.SetGradientColor(RGB(0,0,0));
	m_msg_press_mode_info.SetTextColor(RGB(255,255,255));

	m_msg_model_info.SetFont(mp_title_font);
	m_msg_model_info.SetCenterText();
	m_msg_model_info.SetColor(RGB(0,0,255));
	m_msg_model_info.SetGradientColor(RGB(0,0,0));
	m_msg_model_info.SetTextColor(RGB(255,255,255));

	m_msg_file_info.SetFont(mp_title_font);
	m_msg_file_info.SetCenterText();
	m_msg_file_info.SetColor(RGB(0,0,255));
	m_msg_file_info.SetGradientColor(RGB(0,0,0));
	m_msg_file_info.SetTextColor(RGB(255,255,255));

	//kwlee 2017.0406
	m_msg_7387_mode_info.SetFont(mp_title_font);
	m_msg_7387_mode_info.SetCenterText();
	m_msg_7387_mode_info.SetColor(RGB(0,0,255));
	m_msg_7387_mode_info.SetGradientColor(RGB(0,0,0));
	m_msg_7387_mode_info.SetTextColor(RGB(255,255,255));

	m_msg_3874_mode_info.SetFont(mp_title_font);
	m_msg_3874_mode_info.SetCenterText();
	m_msg_3874_mode_info.SetColor(RGB(0,0,255));
	m_msg_3874_mode_info.SetGradientColor(RGB(0,0,0));
	m_msg_3874_mode_info.SetTextColor(RGB(255,255,255));

	m_msg_Barcode_mode_info.SetFont(mp_title_font);
	m_msg_Barcode_mode_info.SetCenterText();
	m_msg_Barcode_mode_info.SetColor(RGB(0,0,255));
	m_msg_Barcode_mode_info.SetGradientColor(RGB(0,0,0));
	m_msg_Barcode_mode_info.SetTextColor(RGB(255,255,255));
}

LRESULT CDialog_Title::OnStatusDataUpdate(WPARAM wParam,LPARAM lParam)
{
	int mn_change;			// PostMessage�� ���� ���ŵ� �޽��� ID ���� ���� 
	CString str_file;
	int n_length, n_pos;

	mn_change = lParam;		// ��ɾ� ID ���� ���� 

	if(wParam == FORM_NAME)			// ȭ�� �� ��ȯ�� ��� 
	{
		OnTitle_Form_Name_Change(mn_change); // ���� ��µ� �� ȭ�� �̸� ���� ��� �Լ�
	}
	else if(wParam == MACHINE_STATUS)	// ��� ���� ���� ������ ��� 
	{
		OnTitle_Status_Change(mn_change);
	}
	else if(wParam == DEVICE_MODE)	// Device Mode
	{
		OnTitle_Device_Mode_Display(mn_change);
	}
	//kwlee 2017.0406
	else if(wParam == MODE_7387)	// 7387 Mode
	{
		OnTitle_7387_Mode_Display(mn_change);
	}
	else if(wParam == MODE_3874)	// 3874 Mode
	{
		OnTitle_3874_Mode_Display(mn_change);
	}
	else if(wParam == MODE_BCR)	// Device Mode
	{
		OnTitle_Barcode_Mode_Display(mn_change);
	}
	//////////
	else if(wParam == PRESS_MODE)	// Jig Mode
	{
		OnTitle_Press_Mode_Display(mn_change);
	}
	else if(wParam == INTERFACE_MODE)	// Heatsink Mode
	{
		OnTitle_Interface_Mode_Display(mn_change);
	}
	else if(wParam == NEXTMACHINE_MODE)	// Next Machine Mode
	{
		OnTitle_NextMachine_Mode_Display(mn_change);
	}
	else if(wParam == MODEL_MODE)
	{

	}
	else if(wParam == FILE_MODE)
	{
		n_length	= st_basic.mstr_device_name.GetLength();
		n_pos		= st_basic.mstr_device_name.Find(".", 0);
		str_file	= st_basic.mstr_device_name.Mid(0, n_pos);

		m_msg_file_info.SetWindowText(str_file);
	}

	return 0;
}

void CDialog_Title::OnTitle_Form_Name_Change(int nViewID)
{
	CString mstr_form_name;  // ���� ������ ȭ�� ���� ���� ���� 

	switch(nViewID)
	{
		case IDW_SCREEN_MAIN:
			mstr_form_name = _T("\n");
			mstr_form_name += _T(" Screen: Main");
			break;
		case IDW_SCREEN_LOCK:
			mstr_form_name = _T("\n");
			mstr_form_name += _T(" Screen: System Lock");
			break;
		case IDW_SCREEN_INIT:
			mstr_form_name = _T("\n");
			mstr_form_name += _T(" Screen: Initial");
			break;
		case IDW_SCREEN_BASIC :
			mstr_form_name = _T("\n");
			mstr_form_name += _T(" Screen: Basic");
			break;
		case IDW_SCREEN_SET_MAINTENANCE :
			mstr_form_name = _T(" Menu: Setting");
			mstr_form_name += _T("\n");
			mstr_form_name += _T(" Screen: Maintenance");
			break;
		case IDW_SCREEN_WAIT_TIME :
			mstr_form_name += _T("\n");
			mstr_form_name += _T(" Screen: Wait Time");
			break;
		case IDW_SCREEN_MOTOR_SPEED :
			mstr_form_name = _T(" Menu: Motor");
			mstr_form_name += _T("\n");
			mstr_form_name += _T(" Screen: Speed");
			break;
		case IDW_SCREEN_LIST_OPERATION :
			mstr_form_name = _T(" Menu: List");
			mstr_form_name += _T("\n");
			mstr_form_name += _T(" Screen: Operation");
			break;
		case IDW_SCREEN_LIST_WORK :
			mstr_form_name = _T(" Menu: List");
			mstr_form_name += _T("\n");
			mstr_form_name += _T(" Screen: Work");
			break;
		case IDW_SCREEN_LIST_ALARM :
			mstr_form_name = _T(" Menu: List");
			mstr_form_name += _T("\n");
			mstr_form_name += _T(" Screen: Alarm");
			break;
// 		case IDW_SCREEN_LIST_STEP :
// 			mstr_form_name = _T(" Menu: List");
// 			mstr_form_name += _T("\n");
// 			mstr_form_name += _T(" Screen: Step");
// 			break;
			//kwlee 2017.0421
		case IDW_SCREEN_LIST_BOATTEACING :
			mstr_form_name = _T(" Menu: List");
			mstr_form_name += _T("\n");
			mstr_form_name += _T(" Screen: Boat Teaching");
			break;
		case IDW_SCREEN_LIST_DATA_VIEW:
			mstr_form_name = _T(" Menu: List");
			mstr_form_name += _T("\n");
			mstr_form_name += _T(" Screen: Data");
			break;
		case IDW_SCREEN_IO_MAP :
			mstr_form_name = _T("\n");
			mstr_form_name += _T(" Screen: I/O");
			break;
		case IDW_SCREEN_ADMINISTRATOR :
			mstr_form_name = _T("\n");
			mstr_form_name += _T(" Screen: ADMINISTRATOR");
			break;
		case IDW_SCREEN_MOTOR:
			mstr_form_name = _T("\n");
			mstr_form_name += _T(" Screen: Motor Teach");
			break;
		case IDW_SCREEN_SET_MEASURE:
			mstr_form_name = _T(" Menu: Setting");
			mstr_form_name = _T("\n");
			mstr_form_name += _T(" Screen: Measure");
			break;
		case IDW_SCREEN_SET_INTERFACE:
			mstr_form_name += _T("\n");
			mstr_form_name += _T(" Screen: Setting Mode");
			break;
		default:
			mstr_form_name = _T(" ERROR: SCREEN VIEW ID");
			break;
	}

	m_msg_form_name.SetText(_T(mstr_form_name));
}

void CDialog_Title::OnTitle_Picture_Set()
{
	if (m_gif_title_logo.Load(MAKEINTRESOURCE(IDR_GIF_LOGO),_T("GIF")))
		m_gif_title_logo.Draw();
}	

void CDialog_Title::OnTitle_Status_Change(int n_status)
{
	if (st_handler.n_maint_mode == YES)
	{
		m_msg_status_info.SetWindowText(_T("MAINT"));
		m_msg_status_info.SetColor(RGB(0,0,0));
		m_msg_status_info.SetGradientColor(RGB(255,0,0));
		m_msg_status_info.SetTextColor(RGB(255,255,255));
		return;
	}
	
	switch(n_status)
	{
	case dSTOP :		// STOP ����
		m_msg_status_info.SetWindowText(_T("STOP"));
		m_msg_status_info.SetTextColor(RGB(255,0,0));
		break;
	case dRUN :			// RUN ����
		m_msg_status_info.SetWindowText(_T("RUN"));
		m_msg_status_info.SetTextColor(RGB(0, 0, 255));
		break;
	case dJAM :			// ALARM ����
		m_msg_status_info.SetWindowText(_T("ALARM"));
		m_msg_status_info.SetTextColor(RGB(255,0,0));
		break;
	case dWARNING :		// ALARM ����
		m_msg_status_info.SetWindowText(_T("ALARM"));
		m_msg_status_info.SetTextColor(RGB(255,0,0));
		break;
	case dMAINT :		// MAINT ����
		m_msg_status_info.SetWindowText(_T("MAINT"));
		m_msg_status_info.SetTextColor(RGB(0,0,200));
		break;
	case dINIT :		// INIT ����
		m_msg_status_info.SetWindowText(_T("INIT"));
		m_msg_status_info.SetTextColor(RGB(50,100,150));
		break;
	case dLOTEND :		// LOT END ����
		m_msg_status_info.SetWindowText(_T("LOTEND"));
		m_msg_status_info.SetTextColor(RGB(50,100,150));
		break;
	case dLOCK :		// LOCK ����
		m_msg_status_info.SetWindowText(_T("LOCK"));
		m_msg_status_info.SetTextColor(RGB(50,100,150));
		break;
	case dSELFCHECK :	// SELFCHECK ����
		m_msg_status_info.SetWindowText(_T("SELFCHECK"));
		m_msg_status_info.SetTextColor(RGB(50,100,150));
		break;
	default :
		break;
	}
	
	m_msg_status_info.SetColor(RGB(255,255,255));
	m_msg_status_info.SetGradientColor(RGB(0,255,0));
}

void CDialog_Title::OnTitle_Device_Mode_Display(int n_device)
{
	CString str_device;   // �ӽ� ���� ����
	
	switch(n_device)
	{
		case WITHOUT_DVC:
			m_msg_device_mode_info.SetColor(RGB(255,0,0));
			str_device = _T("WITHOUT DEVICE");
			break;

		case WITH_DVC:
			m_msg_device_mode_info.SetColor(RGB(0,0,255));
			str_device = _T("WITH DEVICE");
			break;

		case WITHOUT_TRAY:
			m_msg_device_mode_info.SetColor(RGB(255,0,0));
			str_device = _T("NO C-TRAY");
			break;
		default:
			m_msg_device_mode_info.SetColor(RGB(255,0,0));
			str_device = _T("ERROR");
			break;
	}
	
	m_msg_device_mode_info.SetWindowText(_T(str_device));
}
//kwlee 2017.0406
void CDialog_Title::OnTitle_7387_Mode_Display(int n_device)
{
	CString str_device;   // �ӽ� ���� ����
	
	switch(n_device)
	{
	case NOT_USE:
		m_msg_7387_mode_info.SetColor(RGB(255,0,0));
		str_device = _T("7387����");
		break;
		
	case USE:
		m_msg_7387_mode_info.SetColor(RGB(0,0,255));
		str_device = _T("7387���");
		break;
		

	default:
		m_msg_7387_mode_info.SetColor(RGB(255,0,0));
		str_device = _T("ERROR");
		break;
	}
	
	m_msg_7387_mode_info.SetWindowText(_T(str_device));
}

void CDialog_Title::OnTitle_3874_Mode_Display(int n_device)
{
	CString str_device;   // �ӽ� ���� ����
	
	switch(n_device)
	{
	case NOT_USE:
		m_msg_3874_mode_info.SetColor(RGB(255,0,0));
		str_device = _T("3874����");
		break;
		
	case USE:
		m_msg_3874_mode_info.SetColor(RGB(0,0,255));
		str_device = _T("3874���");
		break;
		

	
	default:
		m_msg_3874_mode_info.SetColor(RGB(255,0,0));
		str_device = _T("ERROR");
		break;
	}
	
	m_msg_3874_mode_info.SetWindowText(_T(str_device));
}

void CDialog_Title::OnTitle_Barcode_Mode_Display(int n_device)
{
	CString str_device;   // �ӽ� ���� ����
	
	switch(n_device)
	{
	case NOT_USE:
		m_msg_Barcode_mode_info.SetColor(RGB(255,0,0));
		str_device = _T("BCR����");
		break;
		
	case USE:
		m_msg_Barcode_mode_info.SetColor(RGB(0,0,255));
		str_device = _T("BCR���");
		break;
		
	default:
		m_msg_Barcode_mode_info.SetColor(RGB(255,0,0));
		str_device = _T("ERROR");
		break;
	}
	
	m_msg_Barcode_mode_info.SetWindowText(_T(str_device));
}

////////////


void CDialog_Title::OnTitle_Press_Mode_Display(int npress)
{
/*	CString str_press;   // �ӽ� ���� ����
	
	switch(npress){
		case SINGLE_MODE:
			m_msg_press_mode_info.SetColor(RGB(255,0,0));
			str_press = _T("Single");
			break;
		case DOUBLE_MODE:
			m_msg_press_mode_info.SetColor(RGB(0,0,255));
			str_press = _T("Double");
			break;
		default:
			m_msg_press_mode_info.SetColor(RGB(255,0,0));
			str_press = _T("ERROR");
			break;
	}
	
	m_msg_press_mode_info.SetWindowText(_T(str_press));*/
}

void CDialog_Title::OnTitle_Interface_Mode_Display(int ninterface)
{
	CString str_interface;   // �ӽ� ���� ����
/*	
	switch(ninterface){
		case OFF_LINE_MODE:
			m_msg_interface_mode_info.SetColor(RGB(255,0,0));
			str_interface = _T("Off Line");
			break;	
		case ON_LINE_MODE:
			m_msg_interface_mode_info.SetColor(RGB(0,0,255));
			str_interface = _T("On Line");
			break;
	}
*/	
// 	switch(ninterface)
// 	{
// 		case 0:
// 			m_msg_interface_mode_info.SetColor(RGB(255,0,0));
// 			str_interface = _T("�ڵ� �ܺ���");
// 			break;	
// 		case 1:
// 			m_msg_interface_mode_info.SetColor(RGB(0,0,255));
// 			str_interface = _T("���� �ܺ���");
// 			break;
// 	}
// 
// 	m_msg_interface_mode_info.SetWindowText(_T(str_interface));
}

void CDialog_Title::OnTitle_NextMachine_Mode_Display(int n_device)
{
	CString str_device;   // �ӽ� ���� ����
	
	switch(n_device)
	{
	case NOT_USE:
		m_msg_interface_mode_info.SetColor(RGB(255,0,0));
		str_device = _T("ü������");
		break;
		
	case USE:
		m_msg_interface_mode_info.SetColor(RGB(0,0,255));
		str_device = _T("ü�����");
		break;
		
	default:
		m_msg_interface_mode_info.SetColor(RGB(255,0,0));
		str_device = _T("ERROR");
		break;
	}
	
	m_msg_interface_mode_info.SetWindowText(_T(str_device));
}

BOOL CDialog_Title::DestroyWindow() 
{
	// TODO: Add your specialized code here and/or call the base class
	/* ************************************************************************** */
	/* ������ ��Ʈ ���� �����Ѵ�                                                  */
	/* ************************************************************************** */
	delete mp_title_font;
	mp_title_font = NULL;
	/* ************************************************************************** */
	return CInitDialogBar::DestroyWindow();
}

void CDialog_Title::PostNcDestroy() 
{
	// TODO: Add your specialized code here and/or call the base class
	/* ************************************************************************** */
	/* ������ ��Ʈ ���� �����Ѵ�                                                  */
	/* ************************************************************************** */
	delete mp_title_font;
	mp_title_font = NULL;
	/* ************************************************************************** */
	CInitDialogBar::PostNcDestroy();
}
