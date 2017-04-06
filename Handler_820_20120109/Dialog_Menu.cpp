// Dialog_Menu.cpp : implementation file
//

#include "stdafx.h"
#include "handler.h"
#include "Dialog_Menu.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDialog_Menu dialog


CDialog_Menu::CDialog_Menu(CWnd* pParent /*=NULL*/)
	: StandardDialog(CDialog_Menu::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDialog_Menu)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDialog_Menu::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialog_Menu)
	DDX_Control(pDX, IDC_GIF_MENU, m_gif_menu);
	DDX_Control(pDX, IDC_MSG_MENU, m_msg_menu);
	DDX_Control(pDX, IDC_LIST_MENU, m_list_menu);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDialog_Menu, StandardDialog)
	//{{AFX_MSG_MAP(CDialog_Menu)
	ON_LBN_SELCHANGE(IDC_LIST_MENU, OnSelchangeListMenu)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialog_Menu message handlers

BOOL CDialog_Menu::OnInitDialog() 
{
	StandardDialog::OnInitDialog();
	
	/* ************************************************************************** */
	/* ��Ʈ�ѿ� ������ ��Ʈ �����Ѵ�                                              */
	/* ************************************************************************** */
	mp_menu_font = NULL;
	mp_menu_font = new CFont;
	mp_menu_font->CreateFont(18,0,0,0,900,0,0,0,0,0,0,ANTIALIASED_QUALITY,0,"MS Sans Serif");
	/* ************************************************************************** */
	
	OnMenu_Button_Set();
	OnMenu_Lable_Set();
	OnMenu_Picture_Set();

	OnCreate_Menu_ListBox();	// ����Ʈ �ڽ� ��Ʈ�ѿ� �޴� �ؽ�Ʈ ���� �߰� �Լ�
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}



void CDialog_Menu::OnMenu_Button_Set()
{
	mp_menu_ok = CreateOkButton(IDOK, this);
}

void CDialog_Menu::OnMenu_Lable_Set()
{
	m_msg_menu.SetFont(mp_menu_font);
	m_msg_menu.SetWindowText(_T("Select Menu"));
	m_msg_menu.SetCenterText();
	m_msg_menu.SetColor(RGB(0,0,255));
	m_msg_menu.SetGradientColor(RGB(0,0,0));
	m_msg_menu.SetTextColor(RGB(255,255,255));
}

void CDialog_Menu::OnMenu_Picture_Set()
{
	if (m_gif_menu.Load(MAKEINTRESOURCE(IDR_GIF_MENU),_T("GIF")))
		m_gif_menu.Draw();
}

void CDialog_Menu::OnCreate_Menu_ListBox()
{
	OnCreate_Menu_Image_List(); /* ����Ʈ �ڽ� ��Ʈ�ѿ� ������ ���� �Լ� */

	// Associate image list to list box
	m_list_menu.SetImageList(&mp_menu_image_list);

	if (st_other.str_parent_menu == "Setting Screen")		// Setting �޴��� ���� ���� �޴� ���� 
	{
		m_list_menu.AddString(_T("Maintenance"), 0);
//		m_list_menu.AddString(_T("Interface"), 1);
		m_list_menu.AddString(_T("Measure"), 1);
	}
	else if (st_other.str_parent_menu == "Motor Screen")	// Robot ���� �޴��� ���� ���� �޴� ���� 
	{
		m_list_menu.AddString(_T("Motor Teach"), 0);
		m_list_menu.AddString(_T("Speed"), 1);
	}
	else if (st_other.str_parent_menu == "List Screen")		// ����Ʈ �޴��� ���� ���� �޴� ���� 
	{
		m_list_menu.AddString(_T("Operation"), 0);
		m_list_menu.AddString(_T("Work"), 1);
		m_list_menu.AddString(_T("Alarm"), 2);
		m_list_menu.AddString(_T("Step"), 3);
		m_list_menu.AddString(_T("WaitTime"), 4); //kwlee 2017.0406
	}
}

void CDialog_Menu::OnCreate_Menu_Image_List()
{
	BOOL bRetValue = FALSE;
	HICON hIcon = NULL;

	/* ************************************************************************** */
	/* ����Ʈ �ڽ� ��Ʈ�ѿ� ������ �̹��� �����Ѵ�.                               */
	/* ************************************************************************** */
	bRetValue = mp_menu_image_list.Create(32, 32, ILC_COLOR32 | ILC_MASK, 5, 1);
	ASSERT(bRetValue == TRUE);
	/* ************************************************************************** */

	/* ************************************************************************** */
	/* ����Ʈ �ڽ� ��Ʈ�ѿ� ������ �������� �߰��Ѵ�.                             */
	/* ************************************************************************** */
	if (st_other.str_parent_menu == "Setting Screen")  // Setting �޴��� ���� ���� �޴� ���� 
	{
		hIcon = AfxGetApp()->LoadIcon(IDI_MAINTENANCE);
		mp_menu_image_list.Add(hIcon);

		hIcon = AfxGetApp()->LoadIcon(IDI_MAINTENANCE);
		mp_menu_image_list.Add(hIcon);

//		hIcon = AfxGetApp()->LoadIcon(IDI_MAINTENANCE);
//		mp_menu_image_list.Add(hIcon);
	}
	else if (st_other.str_parent_menu == "Motor Screen")  // Robot ���� �޴��� ���� ���� �޴� ���� 
	{
		hIcon = AfxGetApp()->LoadIcon(IDI_MOT_POS);
		mp_menu_image_list.Add(hIcon);

		hIcon = AfxGetApp()->LoadIcon(IDI_MOT_SPD);
		mp_menu_image_list.Add(hIcon);
	}
	else if (st_other.str_parent_menu == "List Screen")  // ����Ʈ �޴��� ���� ���� �޴� ���� 
	{
		hIcon = AfxGetApp()->LoadIcon(IDI_LIST_ARM);
		mp_menu_image_list.Add(hIcon);

		hIcon = AfxGetApp()->LoadIcon(IDI_LIST_WORK);
		mp_menu_image_list.Add(hIcon);

		hIcon = AfxGetApp()->LoadIcon(IDI_ONLINE);
		mp_menu_image_list.Add(hIcon);

		hIcon = AfxGetApp()->LoadIcon(IDI_OPEN_FOLDER);
		mp_menu_image_list.Add(hIcon);

		//kwlee 2017.0406
		hIcon = AfxGetApp()->LoadIcon(IDI_OPEN_FOLDER);
		mp_menu_image_list.Add(hIcon);
	}
}

void CDialog_Menu::OnMenu_ID_Sending(CString str_selected_menu)
{
	if (str_selected_menu == "Motor Teach")
	{
		::PostMessage(st_handler.hWnd, WM_FORM_CHANGE, 5, 1);
	}
	else if (str_selected_menu == "Speed")
	{
		::PostMessage(st_handler.hWnd, WM_FORM_CHANGE, 5, 2);
	}
	else if (str_selected_menu == "Operation")
	{
		::PostMessage(st_handler.hWnd, WM_FORM_CHANGE, 6, 1);
	}
	else if (str_selected_menu == "Work")
	{
		::PostMessage(st_handler.hWnd, WM_FORM_CHANGE, 6, 2);
	}
	else if (str_selected_menu == "Alarm")
	{
		::PostMessage(st_handler.hWnd, WM_FORM_CHANGE, 6, 3);
	}
	else if (str_selected_menu == "Step")
	{
		::PostMessage(st_handler.hWnd, WM_FORM_CHANGE, 6, 4);
	}
// 	else if (str_selected_menu == "DataView")
// 	{
// 		::PostMessage(st_handler.hWnd, WM_FORM_CHANGE, 6, 6);
// 	}
	//kwlee 2017.0406
	else if (str_selected_menu == "WaitTime")
	{
		::PostMessage(st_handler.hWnd, WM_FORM_CHANGE, 6, 6);
	}
	else if (str_selected_menu == "Maintenance")
	{
		::PostMessage(st_handler.hWnd, WM_FORM_CHANGE, 3, 1);
	}
	else if (str_selected_menu == "Interface")
	{
		::PostMessage(st_handler.hWnd, WM_FORM_CHANGE, 3, 2);
	}
	else if (str_selected_menu == "Measure")
	{
		::PostMessage(st_handler.hWnd, WM_FORM_CHANGE, 3, 3);
	}
	StandardDialog::OnOK();
}

void CDialog_Menu::OnSelchangeListMenu() 
{
	int mn_index = m_list_menu.GetCurSel();				// ���� ���õ� �� ������ ��´�. 

	if (mn_index != LB_ERR)
	{
		m_list_menu.GetText(mn_index, mp_menu_name);	// ���õ� ���� ���� ���ڿ� ������ ��´�. 

		OnMenu_ID_Sending(mp_menu_name);				// ����Ʈ �ڽ� ��Ʈ�ѿ��� ���õ� �޴��� ���� �޽��� ���� �Լ�
	}
}

BOOL CDialog_Menu::DestroyWindow() 
{
	// TODO: Add your specialized code here and/or call the base class
	/* ************************************************************************** */
	/* ������ ��Ʈ ���� �����Ѵ�                                                  */
	/* ************************************************************************** */
	delete mp_menu_font;
	mp_menu_font = NULL;
	/* ************************************************************************** */
	
	delete mp_menu_ok;  // OK ��ư�� ������ ���� ����
	return StandardDialog::DestroyWindow();
}
