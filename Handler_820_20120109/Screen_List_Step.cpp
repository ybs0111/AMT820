// Screen_List_Step.cpp : implementation file
//

#include "stdafx.h"
#include "handler.h"
#include "Screen_List_Step.h"

/* ****************************************************************************** */
/* ������ ���� Ŭ����                                                             */
/* ****************************************************************************** */


#include "io.h"				// ���� ���� ���� �˻� �Լ� ȣ���ϱ� ���ؼ��� �ݵ�� �߰�

#include "Dialog_Infor.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CScreen_List_Step

IMPLEMENT_DYNCREATE(CScreen_List_Step, CFormView)

CScreen_List_Step::CScreen_List_Step()
	: CFormView(CScreen_List_Step::IDD)
{
	//{{AFX_DATA_INIT(CScreen_List_Step)
	//}}AFX_DATA_INIT
}

CScreen_List_Step::~CScreen_List_Step()
{
}

void CScreen_List_Step::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CScreen_List_Step)
	DDX_Control(pDX, IDC_MSG_THREAD_11, m_msg_thread_11);
	DDX_Control(pDX, IDC_DATA_THREAD_11, m_data_thread_11);
	DDX_Control(pDX, IDC_MSG_THREAD_9, m_msg_thread_9);
	DDX_Control(pDX, IDC_MSG_THREAD_8, m_msg_thread_8);
	DDX_Control(pDX, IDC_MSG_THREAD_7, m_msg_thread_7);
	DDX_Control(pDX, IDC_MSG_THREAD_10, m_msg_thread_10);
	DDX_Control(pDX, IDC_DATA_THREAD_9, m_data_thread_9);
	DDX_Control(pDX, IDC_DATA_THREAD_8, m_data_thread_8);
	DDX_Control(pDX, IDC_DATA_THREAD_7, m_data_thread_7);
	DDX_Control(pDX, IDC_DATA_THREAD_10, m_data_thread_10);
	DDX_Control(pDX, IDC_MSG_THREAD_6, m_msg_thread_6);
	DDX_Control(pDX, IDC_MSG_THREAD_5, m_msg_thread_5);
	DDX_Control(pDX, IDC_MSG_THREAD_4, m_msg_thread_4);
	DDX_Control(pDX, IDC_MSG_THREAD_3, m_msg_thread_3);
	DDX_Control(pDX, IDC_MSG_THREAD_2, m_msg_thread_2);
	DDX_Control(pDX, IDC_MSG_THREAD_1, m_msg_thread_1);
	DDX_Control(pDX, IDC_GROUP_STEP_LIST, m_group_step_list);
	DDX_Control(pDX, IDC_DATA_THREAD_6, m_data_thread_6);
	DDX_Control(pDX, IDC_DATA_THREAD_5, m_data_thread_5);
	DDX_Control(pDX, IDC_DATA_THREAD_4, m_data_thread_4);
	DDX_Control(pDX, IDC_DATA_THREAD_3, m_data_thread_3);
	DDX_Control(pDX, IDC_DATA_THREAD_2, m_data_thread_2);
	DDX_Control(pDX, IDC_DATA_THREAD_1, m_data_thread_1);
	DDX_Control(pDX, IDC_BTN_STEP_SAVE, m_btn_step_save);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CScreen_List_Step, CFormView)
	//{{AFX_MSG_MAP(CScreen_List_Step)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BTN_STEP_SAVE, OnBtnStepSave)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CScreen_List_Step diagnostics

#ifdef _DEBUG
void CScreen_List_Step::AssertValid() const
{
	CFormView::AssertValid();
}

void CScreen_List_Step::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CScreen_List_Step message handlers

void CScreen_List_Step::OnInitialUpdate() 
{
	CFormView::OnInitialUpdate();
	
	/* ************************************************************************** */
	/* ��Ʈ�ѿ� ������ ��Ʈ �����Ѵ�                                              */
	/* ************************************************************************** */
	mp_step_list_font = NULL;
	mp_step_list_font = new CFont;
	mp_step_list_font->CreateFont(15,8,0,0,900,0,0,0,0,0,0,DEFAULT_QUALITY,0,"MS Sans Serif");
	/* ************************************************************************** */

	OnList_Step_GroupBox_Set();
	OnList_Step_Lable_Set();

	mn_timer_kill = FALSE;          // Ÿ�̸� ���� ��û �÷��� �ʱ�ȭ
	SetTimer(TMR_STEP, 250, NULL);  // ������ ���� ��� Ÿ�̸� ����
}


void CScreen_List_Step::OnTimer(UINT nIDEvent) 
{
	if (nIDEvent == TMR_STEP) 
	{
		if (mn_timer_kill != TRUE)
		{
			OnList_Step_Data_Set();			// ���� ���� Ŭ���� ������ �����ϴ� �Լ�
			OnList_Step_Data_Display();		// ���� ���� ȭ�� ��� �Լ�
		}
		else
		{
			KillTimer(TMR_STEP);			// Ÿ�̸� �� ����

			On_Step_List_File_Name_Init();  // ������ ���� ���� ���� ���� ���� �Լ�
		}
	}

	CFormView::OnTimer(nIDEvent);
}

void CScreen_List_Step::OnList_Step_GroupBox_Set()
{
	CSxLogFont step_list_font(15,FW_SEMIBOLD,false,"MS Sans Serif");

//	m_group_step_list.SetXPGroupStyle(CXPGroupBox::XPGB_WINDOW);
//	m_group_step_list.SetBackgroundColor(RGB(201, 201, 151), RGB(255, 255, 204));
	m_group_step_list.SetFont(step_list_font);
	m_group_step_list.SetCatptionTextColor(BLUE_C);
	m_group_step_list.SetBorderColor(YELLOW_L);
	m_group_step_list.SetFontBold(TRUE);
}

void CScreen_List_Step::OnList_Step_Lable_Set()
{
	// ������� ����..
	/* ************************************************************************** */
	/* Ÿ��Ʋ ��� Į�� �ؽ�Ʈ �ڽ� �����Ѵ�                                      */
	/* ************************************************************************** */
	m_msg_thread_1.SetAlignment(CNewLabel::HORZ_CENTER | CNewLabel::VERT_CENTER);
	m_msg_thread_1.SetText(" Main Robot Step");
	m_msg_thread_1.SetTextColor(RGB(25, 25, 85));
	m_msg_thread_1.SetBkColor(RGB(180, 230, 250));
	m_msg_thread_1.SetFontBold(TRUE);
	m_msg_thread_1.SetFontName("MS Sans Serif");
	m_msg_thread_1.SetFontSize(12);

	m_msg_thread_2.SetAlignment(CNewLabel::HORZ_CENTER | CNewLabel::VERT_CENTER);
	m_msg_thread_2.SetText(" Unloader Elevator Left Step");
	m_msg_thread_2.SetTextColor(RGB(25, 25, 85));
	m_msg_thread_2.SetBkColor(RGB(180, 230, 250));
	m_msg_thread_2.SetFontBold(TRUE);
	m_msg_thread_2.SetFontName("MS Sans Serif");
	m_msg_thread_2.SetFontSize(12);

	m_msg_thread_3.SetAlignment(CNewLabel::HORZ_CENTER | CNewLabel::VERT_CENTER);
	m_msg_thread_3.SetText(" Unloader Elevator Right Step");
	m_msg_thread_3.SetTextColor(RGB(25, 25, 85));
	m_msg_thread_3.SetBkColor(RGB(180, 230, 250));
	m_msg_thread_3.SetFontBold(TRUE);
	m_msg_thread_3.SetFontName("MS Sans Serif");
	m_msg_thread_3.SetFontSize(12);
/*
	m_msg_thread_4.SetAlignment(CNewLabel::HORZ_CENTER | CNewLabel::VERT_CENTER);
	m_msg_thread_4.SetText(" Loader Site Step");
	m_msg_thread_4.SetTextColor(RGB(25, 25, 85));
	m_msg_thread_4.SetBkColor(RGB(180, 230, 250));
	m_msg_thread_4.SetFontBold(TRUE);
	m_msg_thread_4.SetFontName("MS Sans Serif");
	m_msg_thread_4.SetFontSize(12);

	m_msg_thread_5.SetAlignment(CNewLabel::HORZ_CENTER | CNewLabel::VERT_CENTER);
	m_msg_thread_5.SetText(" Unloader Site Step");
	m_msg_thread_5.SetTextColor(RGB(25, 25, 85));
	m_msg_thread_5.SetBkColor(RGB(180, 230, 250));
	m_msg_thread_5.SetFontBold(TRUE);
	m_msg_thread_5.SetFontName("MS Sans Serif");
	m_msg_thread_5.SetFontSize(12);
*/
	m_msg_thread_4.SetAlignment(CNewLabel::HORZ_CENTER | CNewLabel::VERT_CENTER);
	m_msg_thread_4.SetText(" Unloader Transfer Left Step");
	m_msg_thread_4.SetTextColor(RGB(25, 25, 85));
	m_msg_thread_4.SetBkColor(RGB(180, 230, 250));
	m_msg_thread_4.SetFontBold(TRUE);
	m_msg_thread_4.SetFontName("MS Sans Serif");
	m_msg_thread_4.SetFontSize(12);

	m_msg_thread_5.SetAlignment(CNewLabel::HORZ_CENTER | CNewLabel::VERT_CENTER);
	m_msg_thread_5.SetText(" Unloader Transfer Right Step");
	m_msg_thread_5.SetTextColor(RGB(25, 25, 85));
	m_msg_thread_5.SetBkColor(RGB(180, 230, 250));
	m_msg_thread_5.SetFontBold(TRUE);
	m_msg_thread_5.SetFontName("MS Sans Serif");
	m_msg_thread_5.SetFontSize(12);

	m_msg_thread_6.SetAlignment(CNewLabel::HORZ_CENTER | CNewLabel::VERT_CENTER);
	m_msg_thread_6.SetText(" Test Site Step");
	m_msg_thread_6.SetTextColor(RGB(25, 25, 85));
	m_msg_thread_6.SetBkColor(RGB(180, 230, 250));
	m_msg_thread_6.SetFontBold(TRUE);
	m_msg_thread_6.SetFontName("MS Sans Serif");
	m_msg_thread_6.SetFontSize(12);

	m_msg_thread_7.SetAlignment(CNewLabel::HORZ_CENTER | CNewLabel::VERT_CENTER);
	m_msg_thread_7.SetText(" Test Dump Step");
	m_msg_thread_7.SetTextColor(RGB(25, 25, 85));
	m_msg_thread_7.SetBkColor(RGB(180, 230, 250));
	m_msg_thread_7.SetFontBold(TRUE);
	m_msg_thread_7.SetFontName("MS Sans Serif");
	m_msg_thread_7.SetFontSize(12);

	m_msg_thread_8.SetAlignment(CNewLabel::HORZ_CENTER | CNewLabel::VERT_CENTER);
	m_msg_thread_8.SetText(" Rotator Table Step");
	m_msg_thread_8.SetTextColor(RGB(25, 25, 85));
	m_msg_thread_8.SetBkColor(RGB(180, 230, 250));
	m_msg_thread_8.SetFontBold(TRUE);
	m_msg_thread_8.SetFontName("MS Sans Serif");
	m_msg_thread_8.SetFontSize(12);

	m_msg_thread_9.SetAlignment(CNewLabel::HORZ_CENTER | CNewLabel::VERT_CENTER);
	m_msg_thread_9.SetText(" Rotator Dump Step");
	m_msg_thread_9.SetTextColor(RGB(25, 25, 85));
	m_msg_thread_9.SetBkColor(RGB(180, 230, 250));
	m_msg_thread_9.SetFontBold(TRUE);
	m_msg_thread_9.SetFontName("MS Sans Serif");
	m_msg_thread_9.SetFontSize(12);

	m_msg_thread_10.SetAlignment(CNewLabel::HORZ_CENTER | CNewLabel::VERT_CENTER);
	m_msg_thread_10.SetText(" Conveyor Step");
	m_msg_thread_10.SetTextColor(RGB(25, 25, 85));
	m_msg_thread_10.SetBkColor(RGB(180, 230, 250));
	m_msg_thread_10.SetFontBold(TRUE);
	m_msg_thread_10.SetFontName("MS Sans Serif");
	m_msg_thread_10.SetFontSize(12);

	m_msg_thread_11.SetAlignment(CNewLabel::HORZ_CENTER | CNewLabel::VERT_CENTER);
	m_msg_thread_11.SetText(" Module Gripper Step");
	m_msg_thread_11.SetTextColor(RGB(25, 25, 85));
	m_msg_thread_11.SetBkColor(RGB(180, 230, 250));
	m_msg_thread_11.SetFontBold(TRUE);
	m_msg_thread_11.SetFontName("MS Sans Serif");
	m_msg_thread_11.SetFontSize(12);
	/* ************************************************************************** */

	/* ************************************************************************** */
	/* ������ ���� ��� Į�� �ؽ�Ʈ �ڽ� �����Ѵ�                                 */
	/* ************************************************************************** */
	m_data_thread_1.SetFont(mp_step_list_font);
	m_data_thread_1.SetCenterText();
	m_data_thread_1.SetColor(RGB(0,0,255));
	m_data_thread_1.SetGradientColor(RGB(0,0,0));
	m_data_thread_1.SetTextColor(RGB(255,255,255));

	m_data_thread_2.SetFont(mp_step_list_font);
	m_data_thread_2.SetCenterText();
	m_data_thread_2.SetColor(RGB(0,0,255));
	m_data_thread_2.SetGradientColor(RGB(0,0,0));
	m_data_thread_2.SetTextColor(RGB(255,255,255));

	m_data_thread_3.SetFont(mp_step_list_font);
	m_data_thread_3.SetCenterText();
	m_data_thread_3.SetColor(RGB(0,0,255));
	m_data_thread_3.SetGradientColor(RGB(0,0,0));
	m_data_thread_3.SetTextColor(RGB(255,255,255));

	m_data_thread_4.SetFont(mp_step_list_font);
	m_data_thread_4.SetCenterText();
	m_data_thread_4.SetColor(RGB(0,0,255));
	m_data_thread_4.SetGradientColor(RGB(0,0,0));
	m_data_thread_4.SetTextColor(RGB(255,255,255));

	m_data_thread_5.SetFont(mp_step_list_font);
	m_data_thread_5.SetCenterText();
	m_data_thread_5.SetColor(RGB(0,0,255));
	m_data_thread_5.SetGradientColor(RGB(0,0,0));
	m_data_thread_5.SetTextColor(RGB(255,255,255));

	m_data_thread_6.SetFont(mp_step_list_font);
	m_data_thread_6.SetCenterText();
	m_data_thread_6.SetColor(RGB(0,0,255));
	m_data_thread_6.SetGradientColor(RGB(0,0,0));
	m_data_thread_6.SetTextColor(RGB(255,255,255));

	m_data_thread_7.SetFont(mp_step_list_font);
	m_data_thread_7.SetCenterText();
	m_data_thread_7.SetColor(RGB(0,0,255));
	m_data_thread_7.SetGradientColor(RGB(0,0,0));
	m_data_thread_7.SetTextColor(RGB(255,255,255));

	m_data_thread_8.SetFont(mp_step_list_font);
	m_data_thread_8.SetCenterText();
	m_data_thread_8.SetColor(RGB(0,0,255));
	m_data_thread_8.SetGradientColor(RGB(0,0,0));
	m_data_thread_8.SetTextColor(RGB(255,255,255));

	m_data_thread_9.SetFont(mp_step_list_font);
	m_data_thread_9.SetCenterText();
	m_data_thread_9.SetColor(RGB(0,0,255));
	m_data_thread_9.SetGradientColor(RGB(0,0,0));
	m_data_thread_9.SetTextColor(RGB(255,255,255));

	m_data_thread_10.SetFont(mp_step_list_font);
	m_data_thread_10.SetCenterText();
	m_data_thread_10.SetColor(RGB(0,0,255));
	m_data_thread_10.SetGradientColor(RGB(0,0,0));
	m_data_thread_10.SetTextColor(RGB(255,255,255));

	m_data_thread_11.SetFont(mp_step_list_font);
	m_data_thread_11.SetCenterText();
	m_data_thread_11.SetColor(RGB(0,0,255));
	m_data_thread_11.SetGradientColor(RGB(0,0,0));
	m_data_thread_11.SetTextColor(RGB(255,255,255));
}

void CScreen_List_Step::OnList_Step_Data_Set()
{
	
	CString str_temp;		// ���� �ӽ� ���� ����

//	char chr_buf[20] ;
/*
	str_temp = LPCTSTR(_itoa((int)(Run_Main_Robot.Run_Step), chr_buf, 10)) ;
	mstr_thread_1 = str_temp;

	str_temp = LPCTSTR(_itoa((int)(Run_Unload_EL_L.Run_Step), chr_buf, 10)) ;
	mstr_thread_2 = str_temp;

	str_temp = LPCTSTR(_itoa((int)(Run_Unload_EL_R.Run_Step), chr_buf, 10)) ;
	mstr_thread_3 = str_temp;

	str_temp = LPCTSTR(_itoa((int)(Run_Unload_TF_L.Run_Step), chr_buf, 10)) ;
	mstr_thread_4 = str_temp;

	str_temp = LPCTSTR(_itoa((int)(Run_Unload_TF_R.Run_Step), chr_buf, 10)) ;
	mstr_thread_5 = str_temp;

	str_temp = LPCTSTR(_itoa((int)(Run_Test_Site.Run_Step), chr_buf, 10)) ;
	mstr_thread_6 = str_temp;

	str_temp = LPCTSTR(_itoa((int)(Run_Test_Dump.Run_Step), chr_buf, 10)) ;
	mstr_thread_7 = str_temp;

	str_temp = LPCTSTR(_itoa((int)(Run_Index_Table.Run_Step), chr_buf, 10)) ;
	mstr_thread_8 = str_temp;

	str_temp = LPCTSTR(_itoa((int)(Run_Index_Dump.Run_Step), chr_buf, 10)) ;
	mstr_thread_9 = str_temp;

	str_temp = LPCTSTR(_itoa((int)(Run_Conveyor.Run_Step), chr_buf, 10)) ;
	mstr_thread_10 = str_temp;

	str_temp = LPCTSTR(_itoa((int)(Run_Module.Run_Step), chr_buf, 10)) ;
	mstr_thread_11 = str_temp;*/
}

void CScreen_List_Step::OnList_Step_Data_Display()
{
	m_data_thread_1.SetWindowText(_T(mstr_thread_1));
	m_data_thread_2.SetWindowText(_T(mstr_thread_2));
	m_data_thread_3.SetWindowText(_T(mstr_thread_3));
	m_data_thread_4.SetWindowText(_T(mstr_thread_4));
	m_data_thread_5.SetWindowText(_T(mstr_thread_5));
	m_data_thread_6.SetWindowText(_T(mstr_thread_6));
	m_data_thread_7.SetWindowText(_T(mstr_thread_7));
	m_data_thread_8.SetWindowText(_T(mstr_thread_8));
	m_data_thread_9.SetWindowText(_T(mstr_thread_9));
	m_data_thread_10.SetWindowText(_T(mstr_thread_10));
	m_data_thread_11.SetWindowText(_T(mstr_thread_11));
}

void CScreen_List_Step::On_Step_List_File_Name_Init()
{
	CString str_folder_data;					// ������ ���� ���� ���� ������ ���� ���� ����
	CString str_new_file;						// ������ ���ϸ� ���� ����
	CString str_title_time;						// ���Ͽ� ����� Ÿ��Ʋ �ð� ���� ����
	int n_cur_year, n_cur_month, n_cur_day;		// ��, ��, �� ���� ����

	COleDateTime mtime_cur;						// ���� �ð� ���� ����
	CTime mp_time_cur;							// Ÿ��Ʋ �ð� ���� ����

	str_folder_data = (st_path.mstr_path_back) + "Step\\";  // ������ ���� ���� ���� ���� ���� ����

	/* ************************************************************************** */
	/* ���� �̸����� ����� ��¥ �����Ѵ�                                         */
	/* ************************************************************************** */
	mtime_cur = COleDateTime::GetCurrentTime();
	mp_time_cur = CTime::GetCurrentTime() ;

	n_cur_year = mtime_cur.GetYear();  
	n_cur_month = mtime_cur.GetMonth();  
    n_cur_day = mtime_cur.GetDay();  

	str_new_file.Format("%04d%02d%02d", n_cur_year, n_cur_month, n_cur_day);
	str_title_time = mp_time_cur.Format("%Y/%m/%d %H:%M:%S"); 
	/* ************************************************************************** */

	On_Step_List_File_Compare(str_folder_data, str_new_file, str_title_time);  // ������ ���� ���� ���� ���� ���� �˻� �� ���� ���� �Լ�
}

void CScreen_List_Step::On_Step_List_File_Compare(CString str_folder, CString str_file_name, CString str_time_info)
{
	CString str_step_file;		// ������ ���� ���� ���ϸ� ���� ���� [��¥�� ����]
	CString str_index_data;		// ���� �ε��� ���� ����
	CString str_chk_file;		// ���� ���� �˻��� ���Ͽ� ���� [����]+[���ϸ�] ���� ����
	CString str_create_file;	// ������ ���� �̸� ���� ���� [���ϸ�] +[�ε���]+[Ȯ����]
	int n_index_data;			// ���� �ε��� ���� ����
	int n_return;				// �Լ� ���� �÷���

	/* ************************************************************************** */
	/* ������ ���� ���� ���ϸ� �� �ε��� �ε��Ѵ�                                 */
	/* ************************************************************************** */
	str_step_file = mcls_file.OnStep_File_Name_Load();    // ������ ���� ���� ���ϸ� �ε� �Լ�
	str_index_data = mcls_file.OnStep_File_Index_Load();  // ������ ���� ���� ���� �ε��� �ε� �Լ�
	/* ************************************************************************** */

	/* ************************************************************************** */
	/* ������ ���ϸ�� ������ ���� ���ϸ� ���Ͽ� �ε��� ����                    */
	/* -> str_step_file : ������ ������ ���ϸ�                                    */
	/* -> str_file_name : ���� ������ ���ϸ�                                      */
	/* ************************************************************************** */
	if (str_step_file == str_file_name)  
	{
		n_index_data = atoi(str_index_data);
		n_index_data++;
		if (n_index_data > 99)  // �ε��� �ִ� ���� ���� �˻�
			n_index_data = 1;
	}
	else  
		n_index_data = 1;		// ��¥ ����Ǿ����Ƿ� �ε��� �ʱ�ȭ

	str_index_data.Format("%02d", n_index_data);  // �ε��� 2�ڸ� ���ڷ� ����
	/* ************************************************************************** */

	/* ************************************************************************** */
	/* ������ ���Ͽ� ���� ����+���ϸ� �����Ѵ�                                    */
	/* ************************************************************************** */
	str_chk_file = str_folder; 

	str_create_file = _T("AMT820_");
	str_create_file += str_file_name;
	str_create_file += str_index_data;
	str_create_file += ".TXT";

	str_chk_file += str_create_file;
	/* ************************************************************************** */

	n_return = On_Step_List_File_Search(str_chk_file);								// ���� ���� ���� �˻� �Լ�
	if (n_return == TRUE)  
		On_Step_List_File_Create(str_chk_file, str_create_file, str_time_info);		// ������ ���� ���� ���� ���� �Լ�
	else if (n_return == FALSE)
		On_Step_List_File_Create(str_chk_file, str_create_file, str_time_info);		// ������ ���� ���� ���� ���� �Լ�

	On_Step_List_File_Info_Save(str_file_name, str_index_data);						// ������ ���� ���� ���� �̸� ���� �Լ�
}

void CScreen_List_Step::On_Step_List_File_Create(CString str_step_file, CString str_create_file, CString str_time)
{
	CString str_content;    // �� ���ο� ����� ���ڿ� ���� ����
	CString str_temp_data;  // ������ ���� �ӽ� ���� ��
//	char chr_buf[20];
//	int n_length;
	FILE  *fp ;				// ���� ������ ����

	if ((fp=fopen(str_step_file,"w+"))==NULL)  // ���� ����
	{
		AfxMessageBox("���� ���� ����!..");
		return ;
	}

	/* ************************************************************************** */
	/* ���Ͽ� Ÿ��Ʋ ���� ����Ѵ�                                                */
	/* ************************************************************************** */
	str_content = "**************************************************";
	fprintf(fp,"%s\r\n", str_content) ;

	str_content = "| Thread Step Info File                          |";
	fprintf(fp,"%s\r\n", str_content) ;

	str_content = "| DIVISION  : AMT Coporation                     |";
	fprintf(fp,"%s\n", str_content) ;
	
	str_content = "| File Name : " + str_create_file;
    str_content += "              |\r\n";
	fprintf(fp,"%s", str_content) ;

	str_content = "| Save Time : " + str_time;
    str_content += "                |\r\n";
	fprintf(fp,"%s", str_content) ;

	str_content = "**************************************************";
	fprintf(fp,"%s\r\n", str_content) ;

	str_content = "|  Thread Part                           | Step  |";
	fprintf(fp,"%s\n", str_content );
	
	str_content = "+----------------------------------------+-------+";
	fprintf(fp,"%s\r\n",str_content) ;
	/* ************************************************************************** */

	/* ************************************************************************** */
	/* ������ ���� ���Ͽ� ����Ѵ�                                                */
	/* ************************************************************************** */
/*
	str_temp_data = LPCTSTR(_itoa((int)(Run_Main_Robot.Run_Step), chr_buf, 10));
	n_length = str_temp_data.GetLength();
	str_content = "|  Main Robot Run_Step							| ";
	str_content += str_temp_data;
	str_content += FileTextLine(1, n_length);
	fprintf(fp,"%s\r\n", str_content);
	str_content = "+----------------------------------------+-------+";
	fprintf(fp,"%s\r\n",str_content);

	str_temp_data = LPCTSTR(_itoa((int)(Run_Unload_EL_L.Run_Step), chr_buf, 10));
	n_length = str_temp_data.GetLength();
	str_content = "|  Unloader Elevator Left Run_Step               | ";
	str_content += str_temp_data;
	str_content += FileTextLine(1, n_length);
	fprintf(fp,"%s\r\n", str_content);
	str_content = "+----------------------------------------+-------+";
	fprintf(fp,"%s\r\n",str_content);

	str_temp_data = LPCTSTR(_itoa((int)(Run_Unload_EL_R.Run_Step), chr_buf, 10));
	n_length = str_temp_data.GetLength();
	str_content = "|  Unloader Elevator Right Run_Step               | ";
	str_content += str_temp_data;
	str_content += FileTextLine(1, n_length);
	fprintf(fp,"%s\r\n", str_content);
	str_content = "+----------------------------------------+-------+";
	fprintf(fp,"%s\r\n",str_content);

	str_temp_data = LPCTSTR(_itoa((int)(Run_Unload_TF_L.Run_Step), chr_buf, 10));
	n_length = str_temp_data.GetLength();
	str_content = "|  Unloader Transfer Left Run_Step                | ";
	str_content += str_temp_data;
	str_content += FileTextLine(1, n_length);
	fprintf(fp,"%s\r\n", str_content);
	str_content = "+----------------------------------------+-------+";
	fprintf(fp,"%s\r\n",str_content);

	str_temp_data = LPCTSTR(_itoa((int)(Run_Unload_TF_R.Run_Step), chr_buf, 10));
	n_length = str_temp_data.GetLength();
	str_content = "|  Unloader Transfer Right Run_Step               | ";
	str_content += str_temp_data;
	str_content += FileTextLine(1, n_length);
	fprintf(fp,"%s\r\n", str_content);
	str_content = "+----------------------------------------+-------+";
	fprintf(fp,"%s\r\n",str_content);

	str_temp_data = LPCTSTR(_itoa((int)(Run_Test_Site.Run_Step), chr_buf, 10));
	n_length = str_temp_data.GetLength();
	str_content = "|  Test Site Run Step							| ";
	str_content += str_temp_data;
	str_content += FileTextLine(1, n_length);
	fprintf(fp,"%s\r\n", str_content);
	str_content = "+----------------------------------------+-------+";
	fprintf(fp,"%s\r\n",str_content);

	str_temp_data = LPCTSTR(_itoa((int)(Run_Test_Dump.Run_Step), chr_buf, 10));
	n_length = str_temp_data.GetLength();
	str_content = "|  Test Dump Run Step							| ";
	str_content += str_temp_data;
	str_content += FileTextLine(1, n_length);
	fprintf(fp,"%s\r\n", str_content);
	str_content = "+----------------------------------------+-------+";
	fprintf(fp,"%s\r\n",str_content);

	str_temp_data = LPCTSTR(_itoa((int)(Run_Index_Table.Run_Step), chr_buf, 10));
	n_length = str_temp_data.GetLength();
	str_content = "|  Rotator Table Run Step						| ";
	str_content += str_temp_data;
	str_content += FileTextLine(1, n_length);
	fprintf(fp,"%s\r\n", str_content);
	str_content = "+----------------------------------------+-------+";
	fprintf(fp,"%s\r\n",str_content);

	str_temp_data = LPCTSTR(_itoa((int)(Run_Index_Dump.Run_Step), chr_buf, 10));
	n_length = str_temp_data.GetLength();
	str_content = "|  Rotator Dump Run_Step							| ";
	str_content += str_temp_data;
	str_content += FileTextLine(1, n_length);
	fprintf(fp,"%s\r\n", str_content);
	str_content = "+----------------------------------------+-------+";
	fprintf(fp,"%s\r\n",str_content);

	str_temp_data = LPCTSTR(_itoa((int)(Run_Conveyor.Run_Step), chr_buf, 10));
	n_length = str_temp_data.GetLength();
	str_content = "|  Conveyor Run Step                             | ";
	str_content += str_temp_data;
	str_content += FileTextLine(1, n_length);
	fprintf(fp,"%s\r\n", str_content);
	str_content = "+----------------------------------------+-------+";
	fprintf(fp,"%s\r\n",str_content);

	str_temp_data = LPCTSTR(_itoa((int)(Run_Module.Run_Step), chr_buf, 10));
	n_length = str_temp_data.GetLength();
	str_content = "|  RightStep										| ";
	str_content += str_temp_data;
	str_content += FileTextLine(1, n_length);
	fprintf(fp,"%s\r\n", str_content);
	str_content = "+----------------------------------------+-------+";
	fprintf(fp,"%s\r\n",str_content);*/
/*
	str_temp_data = LPCTSTR(_itoa((int)(Run_Upper_Base.LeftElevatorStep), chr_buf, 10));
	n_length = str_temp_data.GetLength();
	str_content = "|  LeftElevatorStep                      | ";
	str_content += str_temp_data;
	str_content += FileTextLine(1, n_length);
	fprintf(fp,"%s\r\n", str_content);
	str_content = "+----------------------------------------+-------+";
	fprintf(fp,"%s\r\n",str_content);
	
	str_temp_data = LPCTSTR(_itoa((int)(Run_Upper_Base.RightElevatorStep), chr_buf, 10));
	n_length = str_temp_data.GetLength();
	str_content = "|  RightElevatorStep                     | ";
	str_content += str_temp_data;
	str_content += FileTextLine(1, n_length);
	fprintf(fp,"%s\r\n", str_content);
	str_content = "+----------------------------------------+-------+";
	fprintf(fp,"%s\r\n",str_content);

	str_temp_data = LPCTSTR(_itoa((int)(Run_Robot_HeatSink.Run_Step), chr_buf, 10));
	n_length = str_temp_data.GetLength();
	str_content = "|  Run_Robot_HeatSink.Run_Step           | ";
	str_content += str_temp_data;
	str_content += FileTextLine(1, n_length);
	fprintf(fp,"%s\r\n", str_content);
	str_content = "+----------------------------------------+-------+";
	fprintf(fp,"%s\r\n",str_content);

	str_temp_data = LPCTSTR(_itoa((int)(Run_Robot_HeatSink.mTrayPickStep), chr_buf, 10));
	n_length = str_temp_data.GetLength();
	str_content = "|  Run_Robot_HeatSink.mTrayPickStep      | ";
	str_content += str_temp_data;
	str_content += FileTextLine(1, n_length);
	fprintf(fp,"%s\r\n", str_content);
	str_content = "+----------------------------------------+-------+";
	fprintf(fp,"%s\r\n",str_content);

	str_temp_data = LPCTSTR(_itoa((int)(Run_Robot_HeatSink.mWhirlerPlaceStep), chr_buf, 10));
	n_length = str_temp_data.GetLength();
	str_content = "|  Run_Robot_HeatSink.mWhirlerPlaceStep  | ";
	str_content += str_temp_data;
	str_content += FileTextLine(1, n_length);
	fprintf(fp,"%s\r\n", str_content);
	str_content = "+----------------------------------------+-------+";
	fprintf(fp,"%s\r\n",str_content);

	str_temp_data = LPCTSTR(_itoa((int)(Run_Robot_HeatSink.mWhirlerPickStep), chr_buf, 10));
	n_length = str_temp_data.GetLength();
	str_content = "|  Run_Robot_HeatSink.mWhirlerPickStep   | ";
	str_content += str_temp_data;
	str_content += FileTextLine(1, n_length);
	fprintf(fp,"%s\r\n", str_content);
	str_content = "+----------------------------------------+-------+";
	fprintf(fp,"%s\r\n",str_content);

	str_temp_data = LPCTSTR(_itoa((int)(Run_Robot_HeatSink.mPlatePlaceStep), chr_buf, 10));
	n_length = str_temp_data.GetLength();
	str_content = "|  Run_Robot_HeatSink.mPlatePlaceStep    | ";
	str_content += str_temp_data;
	str_content += FileTextLine(1, n_length);
	fprintf(fp,"%s\r\n", str_content);
	str_content = "+----------------------------------------+-------+";
	fprintf(fp,"%s\r\n",str_content);

	str_temp_data = LPCTSTR(_itoa((int)(Run_Robot_Module.Run_Step), chr_buf, 10));
	n_length = str_temp_data.GetLength();
	str_content = "|  Run_Robot_Module.Run_Step             | ";
	str_content += str_temp_data;
	str_content += FileTextLine(1, n_length);
	fprintf(fp,"%s\r\n", str_content);
	str_content = "+----------------------------------------+-------+";
	fprintf(fp,"%s\r\n",str_content);

	str_temp_data = LPCTSTR(_itoa((int)(Run_Robot_Module.mTrayPickStep), chr_buf, 10));
	n_length = str_temp_data.GetLength();
	str_content = "|  Run_Robot_Module.mTrayPickStep        | ";
	str_content += str_temp_data;
	str_content += FileTextLine(1, n_length);
	fprintf(fp,"%s\r\n", str_content);
	str_content = "+----------------------------------------+-------+";
	fprintf(fp,"%s\r\n",str_content);

	str_temp_data = LPCTSTR(_itoa((int)(Run_Robot_Module.mLdPlaceStep), chr_buf, 10));
	n_length = str_temp_data.GetLength();
	str_content = "|  Run_Robot_Module.mLdPlaceStep         | ";
	str_content += str_temp_data;
	str_content += FileTextLine(1, n_length);
	fprintf(fp,"%s\r\n", str_content);
	str_content = "+----------------------------------------+-------+";
	fprintf(fp,"%s\r\n",str_content);

	str_temp_data = LPCTSTR(_itoa((int)(Run_Robot_Module.mPlatePickStep), chr_buf, 10));
	n_length = str_temp_data.GetLength();
	str_content = "|  Run_Robot_Module.mPlatePickStep       | ";
	str_content += str_temp_data;
	str_content += FileTextLine(1, n_length);
	fprintf(fp,"%s\r\n", str_content);
	str_content = "+----------------------------------------+-------+";
	fprintf(fp,"%s\r\n",str_content);

	str_temp_data = LPCTSTR(_itoa((int)(Run_Robot_Module.mTrayPlaceStep), chr_buf, 10));
	n_length = str_temp_data.GetLength();
	str_content = "|  Run_Robot_Module.mTrayPlaceStep       | ";
	str_content += str_temp_data;
	str_content += FileTextLine(1, n_length);
	fprintf(fp,"%s\r\n", str_content);
	str_content = "+----------------------------------------+-------+";
	fprintf(fp,"%s\r\n",str_content);

//	str_temp_data = LPCTSTR(_itoa((int)(Run_Transfer_HeatSink.Run_Step), chr_buf, 10));
//	n_length = str_temp_data.GetLength();
	str_content = "|  XYZ Robot mLdPlaceStep                | ";
	str_content += str_temp_data;
	str_content += FileTextLine(1, n_length);
	fprintf(fp,"%s\r\n", str_content);
	str_content = "+----------------------------------------+-------+";
	fprintf(fp,"%s\r\n",str_content);

	str_temp_data = LPCTSTR(_itoa((int)(Run_Transfer_Module.Run_Step), chr_buf, 10));
	n_length = str_temp_data.GetLength();
	str_content = "|  XYZ Robot mPickStep                   | ";
	str_content += str_temp_data;
	str_content += FileTextLine(1, n_length);
	fprintf(fp,"%s\r\n", str_content);
	str_content = "+----------------------------------------+-------+";
	fprintf(fp,"%s\r\n",str_content);

	str_temp_data = LPCTSTR(_itoa((int)(Run_Whirler.Run_Step), chr_buf, 10));
	n_length = str_temp_data.GetLength();
	str_content = "|  Run_Whirler.Run_Step                  | ";
	str_content += str_temp_data;
	str_content += FileTextLine(1, n_length);
	fprintf(fp,"%s\r\n", str_content);
	str_content = "+----------------------------------------+-------+";
	fprintf(fp,"%s\r\n",str_content);

	str_temp_data = LPCTSTR(_itoa((int)(Run_Whirler.WhirlerReadyStep), chr_buf, 10));
	n_length = str_temp_data.GetLength();
	str_content = "|  Run_Whirler.WhirlerReadyStep          | ";
	str_content += str_temp_data;
	str_content += FileTextLine(1, n_length);
	fprintf(fp,"%s\r\n", str_content);
	str_content = "+----------------------------------------+-------+";
	fprintf(fp,"%s\r\n",str_content);

	str_temp_data = LPCTSTR(_itoa((int)(Run_Whirler.WhirlerWorkStep), chr_buf, 10));
	n_length = str_temp_data.GetLength();
	str_content = "|  Run_Whirler.WhirlerWorkStep           | ";
	str_content += str_temp_data;
	str_content += FileTextLine(1, n_length);
	fprintf(fp,"%s\r\n", str_content);
	str_content = "+----------------------------------------+-------+";
	fprintf(fp,"%s\r\n",str_content);

	str_temp_data = LPCTSTR(_itoa((int)(Run_TowerLamp.mn_lamp_step), chr_buf, 10));
	n_length = str_temp_data.GetLength();
	str_content = "|  Tower Lamp Step                       | ";
	str_content += str_temp_data;
	str_content += FileTextLine(1, n_length);
	fprintf(fp,"%s\r\n", str_content);
	str_content = "+----------------------------------------+-------+";
	fprintf(fp,"%s\r\n",str_content);

	str_temp_data = st_handler.str_last_alarm;
	str_content = "|  Last Alarm Code                       |";
	TextAndLine(6, str_content, str_temp_data, fp);

	str_content = "+----------------------------------------+-------+";
	fprintf(fp,"%s\r\n",str_content);*/
	/* ************************************************************************** */

	if (ferror(fp)) 
	{
		AfxMessageBox("���� ���� ����!..");
		return ;
	}

	fclose(fp);  // ���� ����
}

int CScreen_List_Step::On_Step_List_File_Search(CString str_chk_file)
{
	int n_existence;		// ���� ���� ���� �÷���

	char fileName[256];

	sprintf(fileName, "%s", str_chk_file);
	n_existence = access(fileName,0) ;
	if (n_existence == -1)  // �ش� ������ ������
		return FALSE;
	
	DeleteFile(fileName);	// �����ϴ� ������ ���� ���� ���� ����

	return TRUE;
}

void CScreen_List_Step::On_Step_List_File_Info_Save(CString str_step_file, CString str_file_index)
{
	mcls_file.OnStep_File_Name_Save(str_step_file);		// ������ ���� ���� ���ϸ� ���Ͽ� �����ϴ� �Լ�
	mcls_file.OnStep_File_Index_Save(str_file_index);	// ������ ���� ���� ���� �ε��� ���Ͽ� �����ϴ� �Լ�
}

CString CScreen_List_Step::FileTextLine(int dir, int length)
{
	CString line;

	switch(dir)
	{
	case 0 :
		if (length == 1 || length == 0 )
		{
			line.Format("%14c", '|');
		}
		else if (length == 2)
		{
			line.Format("%13c", '|');
		}
		else if (length == 3)
		{
			line.Format("%12c", '|');
		}
		else if (length == 4)
		{
			line.Format("%11c", '|');
		}
		else if (length == 5)
		{
			line.Format("%10c", '|');
		}
		else if (length == 6)
		{
			line.Format("%9c", '|');
		}
		else if (length == 7)
		{
			line.Format("%8c", '|');
		}
		else if (length == 8)
		{
			line.Format("%7c", '|');
		}
		else
		{
			line.Format("%8c", '|');
		}
		break;
	case 1 :  
		if (length == 1 || length == 0 )
		{
			line.Format("%6c", '|');
		}
		else if (length == 2)
		{
			line.Format("%5c", '|');
		}
		else if (length == 3)
		{
			line.Format("%4c", '|');
		}
		else if (length == 4)
		{
			line.Format("%3c", '|');
		}
		else if (length == 5)
		{
			line.Format("%2c", '|');
		}
		else
		{
			line.Format("%1c", '|');
		}
		break;
	case 2 :
		if (length == 1 || length == 0 )
		{
			line.Format("%5c", '|');
		}
		else if (length == 2)
		{
			line.Format("%4c", '|');
		}
		else if (length == 3)
		{
			line.Format("%3c", '|');
		}
		else if (length == 4)
		{
			line.Format("%2c", '|');
		}
		else
		{
			line.Format("%1c", '|');
		}
		break;
	case 3 : 
		if (length == 1 || length == 0 )
		{
			//line.Format("%16c", '|');
			line.Format("%17c", '|');
		}
		else if (length == 2)
		{
			line.Format("%15c", '|');
		}
		else if (length == 3)
		{
			line.Format("%14c", '|');
		}
		else if (length == 4)
		{
			line.Format("%13c", '|');
		}
		else if (length == 5)
		{
			line.Format("%12c", '|');
		}
		else if (length == 6)
		{
			line.Format("%11c", '|');
		}
		else if (length == 7)
		{
			line.Format("%10c", '|');
		}
		else if (length == 8)
		{
			line.Format("%9c", '|');
		}
		else if (length == 9)
		{
			line.Format("%8c", '|');
		}
		else if (length == 10)
		{
			line.Format("%7c", '|');
		}
		else if (length == 11)
		{
			line.Format("%6c", '|');
		}
		else
		{
			line.Format("%5c", '|');
		}
		break;
	default :
		break;
	}

	return line;
}

void CScreen_List_Step::OnBtnStepSave() 
{
	int n_response;				// ��ȭ ���� ���� �÷���

	CDialog_Infor   info_dlg;

	st_other.str_confirm_msg = _T("Do you want Save Step Information?");

	n_response = info_dlg.DoModal();
	if (n_response == IDOK)
		mn_timer_kill = TRUE;	// Ÿ�̸� ���� ��û �÷��� ���� 
}

void CScreen_List_Step::TextAndLine(int dir, CString str_sdata, CString str_data, FILE *fp)
{
	CString line, str_temp_data_temp, str_content;
	
	line = "";
	
	fprintf(fp,"%s", str_sdata);
	
	switch(dir)
	{
	case 6 :
		str_temp_data_temp.Format("%6s", str_data);
		line += str_temp_data_temp;
		line += " |";
		fprintf(fp,"%s\r\n", line);
		str_content = "+----------------------------------------+-------+";
		break;
		
	case 14:
		str_temp_data_temp.Format("%14s", str_data);
		line += str_temp_data_temp;
		line += " |";
		fprintf(fp,"%s\r\n", line);
		str_content = "+----------------------------------------+---------------+";
		break;
		
	case 140:
		str_temp_data_temp.Format("%14s", str_data);
		line += str_temp_data_temp;
		line += " |";
		fprintf(fp,"%s", line);
		break;
		
	case 30:
		str_temp_data_temp.Format("%30s", str_data);
		line += str_temp_data_temp;
		line += " |";
		fprintf(fp,"%s\r\n", line);
		str_content = "+----------------------------------------+-------------------------------+";
		break;
		
	case 300:
		str_temp_data_temp.Format("%30s", str_data);
		line += str_temp_data_temp;
		line += " |";
		fprintf(fp,"%s", line);
		break;
	}
	fprintf(fp,"%s\r\n",str_content);
}

BOOL CScreen_List_Step::DestroyWindow() 
{
	// TODO: Add your specialized code here and/or call the base class
	/* ************************************************************************** */
	/* ������ ��Ʈ ���� �����Ѵ�                                                  */
	/* ************************************************************************** */
	delete mp_step_list_font;
	mp_step_list_font = NULL;
	/* ************************************************************************** */

	KillTimer(TMR_STEP);			// Ÿ�̸� �� ����
	return CFormView::DestroyWindow();
}
