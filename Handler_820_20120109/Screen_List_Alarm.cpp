// Screen_List_Alarm.cpp : implementation file
//

#include "stdafx.h"
#include "handler.h"
#include "Screen_List_Alarm.h"

#include "io.h" 

/* ****************************************************************************** */
/* ��ȭ ���� Ŭ���� �߰�                                                          */
/* ****************************************************************************** */
#include "Dialog_Infor.h"
#include "Dialog_Message.h"
/* ****************************************************************************** */

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CScreen_List_Alarm

IMPLEMENT_DYNCREATE(CScreen_List_Alarm, CFormView)

CScreen_List_Alarm::CScreen_List_Alarm()
	: CFormView(CScreen_List_Alarm::IDD)
{
	//{{AFX_DATA_INIT(CScreen_List_Alarm)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CScreen_List_Alarm::~CScreen_List_Alarm()
{
}

void CScreen_List_Alarm::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CScreen_List_Alarm)
	DDX_Control(pDX, IDC_MSG_ALARM_FILE_NAME, m_msg_alarm_file_name);
	DDX_Control(pDX, IDC_RADIO_ALARM_MONTH, m_radio_alarm_month);
	DDX_Control(pDX, IDC_RADIO_ALARM_LOT, m_radio_alarm_lot);
	DDX_Control(pDX, IDC_RADIO_ALARM_LIST, m_radio_alarm_list);
	DDX_Control(pDX, IDC_RADIO_ALARM_DAY, m_radio_alarm_day);
	DDX_Control(pDX, IDC_MSG_ALARM_FILE, m_msg_alarm_file);
	DDX_Control(pDX, IDC_MONTHCAL_ALARM, m_monthcal_alarm);
	DDX_Control(pDX, IDC_LIST_ALARM_FILE, m_list_alarm_file);
	DDX_Control(pDX, IDC_GROUP_ALARM_TYPE, m_group_alarm_type);
	DDX_Control(pDX, IDC_GROUP_ALARM_LIST, m_group_alarm_list);
	DDX_Control(pDX, IDC_BTN_ALARM_FILE_REFRESH, m_btn_alarm_file_refresh);
	DDX_Control(pDX, IDC_BTN_ALARM_FILE_DEL, m_btn_alarm_file_del);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CScreen_List_Alarm, CFormView)
	//{{AFX_MSG_MAP(CScreen_List_Alarm)
	ON_BN_CLICKED(IDC_RADIO_ALARM_LOT, OnRadioAlarmLot)
	ON_BN_CLICKED(IDC_RADIO_ALARM_DAY, OnRadioAlarmDay)
	ON_BN_CLICKED(IDC_RADIO_ALARM_MONTH, OnRadioAlarmMonth)
	ON_BN_CLICKED(IDC_RADIO_ALARM_LIST, OnRadioAlarmList)
	ON_BN_CLICKED(IDC_BTN_ALARM_FILE_DEL, OnBtnAlarmFileDel)
	ON_BN_CLICKED(IDC_BTN_ALARM_FILE_REFRESH, OnBtnAlarmFileRefresh)
	ON_NOTIFY(MCN_SELECT, IDC_MONTHCAL_ALARM, OnSelectMonthcalAlarm)
	ON_LBN_SELCHANGE(IDC_LIST_ALARM_FILE, OnSelchangeListAlarmFile)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CScreen_List_Alarm diagnostics

#ifdef _DEBUG
void CScreen_List_Alarm::AssertValid() const
{
	CFormView::AssertValid();
}

void CScreen_List_Alarm::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CScreen_List_Alarm message handlers

void CScreen_List_Alarm::OnInitialUpdate() 
{
	CFormView::OnInitialUpdate();
	
	/* ************************************************************************** */
	/* ��Ʈ�ѿ� ������ ��Ʈ �����Ѵ�                                              */
	/* ************************************************************************** */
	mp_alarm_list_font = NULL;
	mp_alarm_list_font = new CFont;
	mp_alarm_list_font->CreateFont(15,0,0,0,900,0,0,0,0,0,0,ANTIALIASED_QUALITY,0,"MS Sans Serif");
	/* ************************************************************************** */

	/* ************************************************************************** */
    /* ����� �˶� BIN ��� ���� ���� ���� ����                                 */
    /* ************************************************************************** */
	mn_type_select = 2;				// �ʱ� ���� (Lot��) ���� ���� 

	mstr_selected_Date.Empty();		// ���õ� ��¥ ���� ���� ���� �ʱ�ȭ 
	mstr_alarm_file_name.Empty();	// ���õ� ���� �̸��� �����ϱ� ���� ���� ����

	mstr_default_path = "C:\\AMT820\\";  

	OnAlarm_List_Full_Path_Set(mn_type_select);			// ���� ���� �ε��� ��ü ���� ���� ���� �Լ� */
	/* ************************************************************************** */

	OnAlarm_List_GroupBox_Set();
	OnAlarm_List_Lable_Set();
	OnAlarm_List_EditBox_Set();
	OnAlarm_List_RadioButton_Set();
	OnAlarm_List_State_Change(mn_type_select);

	OnAlarm_List_Create_ImageList();	
	m_list_alarm_file.SetImageList(&mp_alarm_image_list);

	OnAlarm_List_Init_List(LPCTSTR(mstr_full_path));	// �˶� ��� ��� ȭ�鿡 ����� ���� ��� �ʱ�ȭ �۾� �Լ�

	m_monthcal_alarm.SetMaxSelCount(2);
}

void CScreen_List_Alarm::OnAlarm_List_GroupBox_Set()
{
	CSxLogFont alarm_list_font(15,FW_SEMIBOLD,false,"MS Sans Serif");

//	m_group_alarm_list.SetXPGroupStyle(CXPGroupBox::XPGB_WINDOW);
//	m_group_alarm_list.SetBackgroundColor(RGB(201, 201, 151), RGB(255, 255, 204));
	m_group_alarm_list.SetFont(alarm_list_font);
	m_group_alarm_list.SetBorderColor(YELLOW_L);
	m_group_alarm_list.SetCatptionTextColor(BLUE_C);
	m_group_alarm_list.SetFontBold(TRUE);

//	m_group_alarm_type.SetXPGroupStyle(CXPGroupBox::XPGB_WINDOW);
//	m_group_alarm_type.SetBackgroundColor(RGB(201, 201, 151), RGB(255, 255, 204));
	m_group_alarm_type.SetFont(alarm_list_font);
	m_group_alarm_type.SetBorderColor(YELLOW_L);
	m_group_alarm_type.SetCatptionTextColor(BLUE_C);
	m_group_alarm_type.SetFontBold(TRUE);
}

void CScreen_List_Alarm::OnAlarm_List_Lable_Set()
{
	m_msg_alarm_file.SetFont(mp_alarm_list_font);
	m_msg_alarm_file.SetWindowText(_T("Select Display File"));
	m_msg_alarm_file.SetCenterText();
	m_msg_alarm_file.SetColor(RGB(0,0,255));
	m_msg_alarm_file.SetGradientColor(RGB(255,0,0));
	m_msg_alarm_file.SetTextColor(RGB(255,255,255));

	m_msg_alarm_file_name.SetFont(mp_alarm_list_font);
	m_msg_alarm_file_name.SetWindowText(_T(""));
	m_msg_alarm_file_name.SetCenterText();
	m_msg_alarm_file_name.SetColor(RGB(0,0,255));
	m_msg_alarm_file_name.SetGradientColor(RGB(0,0,0));
	m_msg_alarm_file_name.SetTextColor(RGB(255,255,255));
}

void CScreen_List_Alarm::OnAlarm_List_EditBox_Set()
{
	m_edit_alarm_content.SubclassDlgItem(IDC_EDIT_ALARM_DISPLAY, this);
	m_edit_alarm_content.bkColor( RGB(50, 100, 150) );
	m_edit_alarm_content.textColor( RGB(255, 255,255) );
	m_edit_alarm_content.setFont(-14, FW_BOLD, DEFAULT_PITCH | FF_DONTCARE, "����ü");
}

void CScreen_List_Alarm::OnAlarm_List_RadioButton_Set()
{
	//Set images
    m_radio_alarm_lot.SetImage(IDB_REDBUTTON, 15);
	m_radio_alarm_day.SetImage(IDB_REDBUTTON, 15);
    m_radio_alarm_month.SetImage(IDB_REDBUTTON, 15);
	m_radio_alarm_list.SetImage(IDB_REDBUTTON, 15);
}

void CScreen_List_Alarm::OnAlarm_List_State_Change(int n_state)
{
	switch(n_state)
	{
	case 1 :
		m_radio_alarm_lot.Depress(true);
		m_radio_alarm_day.Depress(false);
		m_radio_alarm_month.Depress(false);
		m_radio_alarm_list.Depress(false);
		break;

	case 2 :
		m_radio_alarm_lot.Depress(false);
		m_radio_alarm_day.Depress(true);
		m_radio_alarm_month.Depress(false);
		m_radio_alarm_list.Depress(false);
		break;

	case 3 :
		m_radio_alarm_lot.Depress(false);
		m_radio_alarm_day.Depress(false);
		m_radio_alarm_month.Depress(true);
		m_radio_alarm_list.Depress(false);
		break;

	case 4 :
		m_radio_alarm_lot.Depress(false);
		m_radio_alarm_day.Depress(false);
		m_radio_alarm_month.Depress(false);
		m_radio_alarm_list.Depress(true);
		break;
	default:
		break;
	}
}

void CScreen_List_Alarm::OnAlarm_List_Create_ImageList()
{
	BOOL bRetValue = FALSE;

	// Create image list
	bRetValue = mp_alarm_image_list.Create(16, 16, ILC_COLOR32 | ILC_MASK, 5, 1);
	ASSERT(bRetValue == TRUE);
}

int CScreen_List_Alarm::OnAlarm_List_Init_List(LPCTSTR pszPath)
{
	int mn_chk;					// �Լ� ���� �÷��� ���� ���� 
    CString str_path = pszPath; // ���� ���� ���� 

	HANDLE hFind;
    WIN32_FIND_DATA fd;
	int mn_count = 0;			// ����Ʈ �ڽ� ��Ʈ�ѿ� �߰��� ���� ���� ���� ���� ���� 

	if (str_path.Right (1) != "\\")
        str_path += "\\";

	str_path += "*.*";
    
    if ((hFind = ::FindFirstFile ((LPCTSTR) str_path, &fd)) != INVALID_HANDLE_VALUE) 
	{
        if (!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) 
		{
			mn_chk = AddItem (mn_count, &fd);
			if (mn_chk == TRUE)  mn_count++;
        }

        while (::FindNextFile (hFind, &fd)) 
		{
            if (!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
			{
				mn_chk = AddItem (mn_count, &fd);
				if (mn_chk == FALSE) 
				{
					break;
				}
				else if (mn_chk == RET_ERROR)  mn_count--;
			}
            mn_count++;
        }
		::FindClose(hFind);
    }
	
	return mn_count;
}

void CScreen_List_Alarm::OnAlarm_List_Full_Path_Set(int n_selected)
{
	switch(n_selected)
	{
	case 1 :  // Lot�� ���� ��� ���� 
		mstr_full_path = mstr_default_path + "Alarm\\Lot\\";
		break;
	case 2 :  // Day�� ���� ��� ���� 
		mstr_full_path = mstr_default_path + "Alarm\\Daily\\";
		break;
	case 3 :  // Month�� ���� ��� ���� 
		mstr_full_path = mstr_default_path + "Alarm\\Monthly\\";
		break;
	case 4 :  // List�� ��� ���� 
		mstr_full_path = mstr_default_path + "Alarm\\List\\";
		break;
	default :
		break;
	}
}

BOOL CScreen_List_Alarm::AddItem(int n_index, WIN32_FIND_DATA *pfd)
{
	int mn_pos;

	HICON	hIcon = NULL;

	/* ************************************************************************** */
    /* �ش� ���� ����Ʈ ��Ʈ�ѿ� �߰����� ���� �˻��Ѵ�                           */
    /* ************************************************************************** */
	if (!mstr_selected_Date.IsEmpty()) 
	{
		mstr_temp_file = (pfd->cFileName);
		mn_pos = mstr_temp_file.Find(mstr_selected_Date);  // �޷¿��� ������ ��¥ ������ ���� �̸��� �����ϴ��� �˻� 
		if (mn_pos == -1)  // ������ ��¥ ������ ���� �̸��� �������� �ʴ� ��� 
		{
			return RET_ERROR;
		}
	}
	/* ************************************************************************** */

	hIcon = AfxGetApp()->LoadIcon(IDI_OPEN_FOLDER);
	mp_alarm_image_list.Add(hIcon);

    if (m_list_alarm_file.AddString(_T(pfd->cFileName), n_index-1) == -1)  return FALSE;

    return TRUE;
}

void CScreen_List_Alarm::OnAlarm_List_Selected_File_Load(CString str_folder, CString str_file)
{
	int ndx;					// ����Ʈ �ڽ��� ��µ� ���ڿ� ��ü ���� ���� ���� ���� 
	int mn_existence;			// ���� ���� ���� ���� ���� 
	CString mstr_chk_file;		// ����� ���Ͽ� ���� [����]+[���ϸ�] ���� ���� 
	CString mstr_description;	// ���õ� ���� ���� �ӽ� ���� ���� 
	CString mstr_temp_file;		// ���� ���õ� ���ϸ� �ӽ� ���� ���� 
	char fileName[256];			// ����� ���� ���� ���� ���� 
	char buffer[MAX_BUF_LEN+1];	// ���� ���� �� �� ���� ���� �ӽ� ���� ���� 

	m_msg_alarm_file.SetWindowText(_T("Select Display File"));

	/* ************************************************************************** */
    /* ���� ���õ� ������ �̹� ��µ� �������� �˻��Ѵ�                           */
	/* -> ���� �̸� ��� ����Ʈ �ڽ����� �о ���ؾ� �Ѵ�                     */
    /* ************************************************************************** */
	mstr_temp_file = m_msg_alarm_file_name.GetWindowText();
	mstr_temp_file.MakeUpper();
	mstr_temp_file.TrimLeft(' ');               
	mstr_temp_file.TrimRight(' ');

	if (mstr_alarm_file_name == mstr_temp_file)  return ;  // ���õ� ������ ���� ��µǾ� �ִ� ���ϰ� ������ ��� ���� 
	/* ************************************************************************** */

	mstr_chk_file = str_folder + str_file;  // ���õ� ���Ͽ� ���� [����]+[���ϸ�] ���� 

	sprintf(fileName, "%s", mstr_chk_file);
	mn_existence = access(fileName,0) ;
	if(mn_existence != -1)  
	{
		FILE *fp = fopen ( fileName, "r" );

		if ( fp == (FILE *)NULL ) 
		{
			m_msg_alarm_file.SetWindowText(_T("File Open Error~!!"));
			return ;
		}

		mstr_description.Empty();  // ���� ���� ���� ���� �ʱ�ȭ 

		while ( !feof(fp) )
		{
			if ( fgets ( buffer, MAX_BUF_LEN, fp ) == NULL )  break;

			if ( strlen(buffer) )   buffer[strlen(buffer)-1] = 0;
			strcat ( buffer, "\r\n" );

			mstr_description += buffer;
		}
		fclose(fp);

		((CEdit*)GetDlgItem(IDC_EDIT_ALARM_DISPLAY))->SetWindowText(_T("")); 
		((CEdit*)GetDlgItem(IDC_EDIT_ALARM_DISPLAY))->SetFocus();

		ndx = ((CEdit*)GetDlgItem(IDC_EDIT_ALARM_DISPLAY))->GetWindowTextLength();

		((CEdit*)GetDlgItem(IDC_EDIT_ALARM_DISPLAY))->SendMessage(EM_SETSEL, ndx, ndx);
//		((CEdit*)GetDlgItem(IDC_EDIT_ALARM_DISPLAY))->SendMessage(EM_REPLACESEL, 0, (LPARAM)(LPCSTR)mstr_description );
		((CEdit*)GetDlgItem(IDC_EDIT_ALARM_DISPLAY))->SetWindowText(_T(mstr_description));
	}
	else   
	{
		m_msg_alarm_file.SetWindowText(_T("Not Exist File~!"));
		return ;
	}
}

int CScreen_List_Alarm::OnAlarm_List_Selected_File_Name_Check()
{
	int mn_pos;  // ��ġ ���� ���� ���� 
	CString mstr_chk_file;  // ���õ� ���Ͽ� ���� [����]+[���ϸ�] ���� ���� 
	CString mstr_chk_ext;  // ���� ���� �� Ȯ���� �������� �����ϱ� ���� ���� 

	m_msg_alarm_file.SetWindowText(_T("Select Display File"));

	mstr_alarm_file_name.MakeUpper();
	mstr_alarm_file_name.TrimLeft(' ');
	mstr_alarm_file_name.TrimRight(' ');

	if (mstr_alarm_file_name.IsEmpty()) 
	{
		m_msg_alarm_file.SetWindowText(_T("File name error~!"));
		return FALSE;
	}

	mn_pos = mstr_alarm_file_name.Find(" ");		// ���� ���� ���� �˻� 
	if (mn_pos != -1)
	{
		m_msg_alarm_file.SetWindowText(_T("File name in space error~!"));
		return FALSE;
	}

	mstr_chk_file = mstr_full_path + mstr_alarm_file_name;  // ���Ͽ� ���� [����]+[���ϸ�] ���� ���� 
	mn_pos = mstr_chk_file.Find(".");						// Ȯ���� ��ġ �˻� 
	if (mn_pos == -1) mstr_chk_file += ".TXT";
	else 
	{
		mstr_chk_ext = mstr_chk_file.Mid(mn_pos);	// ���õ� ���� ���� �� ���� Ȯ���� �������� ���� 
		if (mstr_chk_ext != ".TXT")					// .TXT �̿ܿ��� ������ ó��
		{
			m_msg_alarm_file.SetWindowText(_T("Ȯ���� �Է� ���� �߻�"));
			return FALSE;
		}
	}

	return TRUE;
}

void CScreen_List_Alarm::OnAlarm_List_Reset_AllItem()
{
	mstr_selected_Date.Empty();						// ���õ� ��¥ ���� ���� ���� �ʱ�ȭ 
	mstr_alarm_file_name.Empty();					// ���õ� ���� �̸� ���� ���� ���� �ʱ�ȭ 

	OnAlarm_List_Full_Path_Set(mn_type_select);		// ���� ���� �ε��� ��ü ���� ���� ���� �Լ�
	OnAlarm_List_State_Change(mn_type_select);		// ���� ���� ���� ���� ��ư ���� ���� �Լ�

	/* ************************************************************************** */
	/* �ش� ���� �ȿ� �����ϴ� ���� ���� ���Ӱ� ����Ʈ �ڽ� ��Ʈ�ѿ� �߰�         */
	/* ************************************************************************** */
	m_list_alarm_file.ResetContent();				// ����Ʈ �ڽ� ��Ʈ�ѿ� �߰��� ��� ���� ���� 
	mp_alarm_image_list.DeleteImageList();			// �̹��� ����Ʈ�� ������ ��� ���� ���� ���� 

	OnAlarm_List_Create_ImageList();				// �׽�Ʈ ��� ��� ȭ�鿡 ����� �̹��� ���� �Լ�

	// Associate image list to list box
	m_list_alarm_file.SetImageList(&mp_alarm_image_list);

	OnAlarm_List_Init_List(LPCTSTR(mstr_full_path)); // �׽�Ʈ ��� ��� ȭ�鿡 ����� ���� ��� �ʱ�ȭ �۾� �Լ�

	m_msg_alarm_file.SetWindowText(_T("Select Display File"));
	/* ************************************************************************** */
}

void CScreen_List_Alarm::OnAlarm_List_Display_Selected_Day()
{
	COleDateTime ctMinRange, ctMaxRange;
	CString mstr_date_msg, mstr_chk_file;

	if (m_monthcal_alarm.GetSelRange(ctMinRange, ctMaxRange))
	{
		/* ********************************************************************** */
		/* ����� ���� ���� �����Ѵ�                                              */
		/* ********************************************************************** */
		switch(mn_type_select)
		{
		case 1 :  // LOT�� ��� ���� �̸� ���� ���� 
			mstr_date_msg = ctMinRange.Format(_T("%Y%m%d"));
			mstr_chk_file = "LT" + mstr_date_msg;
			break;
		case 2 :  // �Ϻ� ��� ���� �̸� ���� ���� 
			mstr_date_msg = ctMinRange.Format(_T("%Y%m%d"));
			mstr_chk_file = "DY" + mstr_date_msg;
			break;
		case 3 :  // ���� ��� ���� �̸� ���� ���� 
			mstr_date_msg = ctMinRange.Format(_T("%Y%m"));
			mstr_chk_file = "MT" + mstr_date_msg;
			break;
		case 4 :  // ����Ʈ ��� ���� �̸� ���� ���� 
			mstr_date_msg = ctMinRange.Format(_T("%Y%m%d"));
			mstr_chk_file = "DL" + mstr_date_msg;
			break;
		default :
			break;
		}
		/* ********************************************************************** */

		if (mstr_date_msg.IsEmpty()) return;		// ���õ� ��¥ ������ ������ ��� ���� 

		if (mstr_selected_Date != mstr_chk_file)	// ������ ��¥�� ���� ���� ������� �˻� 
		{
			mstr_selected_Date = mstr_chk_file;		// ���� ������ ��¥�� ���� ���� ���� ���� 

			/* ****************************************************************** */
			/* �ش� �����ȿ� �����ϴ� ���� ���� ���Ӱ� ����Ʈ �ڽ��� �߰�         */
			/* ****************************************************************** */
			m_list_alarm_file.ResetContent();		// ����Ʈ �ڽ� ��Ʈ�ѿ� ������ ��� ���� ���� 
			mp_alarm_image_list.DeleteImageList();  // �̹��� ����Ʈ�� ������ ��� ���� ���� 

			OnAlarm_List_Create_ImageList();		// �׽�Ʈ ��� ��� ȭ�鿡 ����� �̹��� ���� �Լ�

			// Associate image list to list box
			m_list_alarm_file.SetImageList(&mp_alarm_image_list);

			OnAlarm_List_Init_List(LPCTSTR(mstr_full_path)); /* �׽�Ʈ ��� ��� ȭ�鿡 ����� ���� ��� �ʱ�ȭ �۾� �Լ� */
			/* ****************************************************************** */
		}
	}
}

void CScreen_List_Alarm::OnRadioAlarmLot() 
{
	if (mn_type_select != 1)  
	{
		mn_type_select = 1;							// Lot ���� ���� 
		OnAlarm_List_State_Change(mn_type_select);	// ���� ���� ���� ���� ��ư ���� ���� �Լ�

		OnAlarm_List_Reset_AllItem();				// ����Ʈ ��Ʈ�ѿ� ������ ��� ������ ���� ���� �� �� ���� �Լ�
	}
}

void CScreen_List_Alarm::OnRadioAlarmDay() 
{
	if (mn_type_select != 2)  
	{
		mn_type_select = 2;							// Lot ���� ���� 
		OnAlarm_List_State_Change(mn_type_select);	// ���� ���� ���� ���� ��ư ���� ���� �Լ�

		OnAlarm_List_Reset_AllItem();				// ����Ʈ ��Ʈ�ѿ� ������ ��� ������ ���� ���� �� �� ���� �Լ�
	}
}

void CScreen_List_Alarm::OnRadioAlarmMonth() 
{
	if (mn_type_select != 3)  
	{
		mn_type_select = 3;							// Lot ���� ���� 
		OnAlarm_List_State_Change(mn_type_select);	// ���� ���� ���� ���� ��ư ���� ���� �Լ�

		OnAlarm_List_Reset_AllItem();				// ����Ʈ ��Ʈ�ѿ� ������ ��� ������ ���� ���� �� �� ���� �Լ�
	}
}

void CScreen_List_Alarm::OnRadioAlarmList() 
{
	if (mn_type_select != 4)  
	{
		mn_type_select = 4;							// Lot ���� ���� 
		OnAlarm_List_State_Change(mn_type_select);	// ���� ���� ���� ���� ��ư ���� ���� �Լ�

		OnAlarm_List_Reset_AllItem();				// ����Ʈ ��Ʈ�ѿ� ������ ��� ������ ���� ���� �� �� ���� �Լ�
	}
}

void CScreen_List_Alarm::OnBtnAlarmFileDel() 
{
	CString mstr_del_file;  // ������ ���Ͽ� ���� [����]+[���ϸ�] ���� ���� 
	int mn_response;		// ��ȭ ���ڿ� ���� ���� �� ���� ����
	int mn_existence;		// ���� ���� ���� ���� �÷��� 

	char fileName[256];		// ������ ���� ���� ���� ���� 

	CDialog_Infor info_dlg;

	if (OnAlarm_List_Selected_File_Name_Check() == FALSE)  return;  // ���õ� ���� �̸� ���� ���� ���� �˻� �Լ�

	st_other.str_confirm_msg = _T("Do you want delete file?");
	
	mn_response = info_dlg.DoModal();
	
	if (mn_response == IDOK)
	{
		mstr_del_file = mstr_full_path + mstr_alarm_file_name;  // ������ ���Ͽ� ���� [����]+[���ϸ�] ���� ���� 

		sprintf(fileName, "%s", mstr_del_file);					// ������ ���Ͽ� ���� [����]+[���ϸ�] ���� ���� 
		mn_existence = access(fileName,0) ;
		if (mn_existence != -1)
		{
			DeleteFile(fileName);  // ���õ� ���� ����

			/* ****************************************************************** */
			/* �ش� �����ȿ� �����ϴ� ���� ���� ���Ӱ� ����Ʈ �ڽ��� �߰�         */
			/* ****************************************************************** */
			m_list_alarm_file.ResetContent();		// ����Ʈ �ڽ� ��Ʈ�ѿ� ������ ��� ���� ���� 
			mp_alarm_image_list.DeleteImageList();  // �̹��� ����Ʈ�� ������ ��� ���� ���� 

			OnAlarm_List_Create_ImageList();		// �˶� ����Ʈ ��� ȭ�鿡 ����� �̹��� ���� �Լ�

			// Associate image list to list box
			m_list_alarm_file.SetImageList(&mp_alarm_image_list);

			OnAlarm_List_Init_List(LPCTSTR(mstr_full_path)); // �˶� ����Ʈ ��� ȭ�鿡 ����� ���� ��� �ʱ�ȭ �۾� �Լ�
			/* ****************************************************************** */

			m_msg_alarm_file_name.SetWindowText(_T(""));			// ���õ� ������ �̸��� ��� ��Ų��.
			
			((CEdit*)GetDlgItem(IDC_EDIT_ALARM_DISPLAY))->SetWindowText(_T("")); 
			((CEdit*)GetDlgItem(IDC_EDIT_ALARM_DISPLAY))->SetFocus();
		}
		else  
		{
			m_msg_alarm_file.SetWindowText(_T("Not Exist File~!"));
			return ;
		}
	}
}

void CScreen_List_Alarm::OnBtnAlarmFileRefresh() 
{
	if (mn_type_select != 1)  
	{
		mn_type_select = 1;							// �Ϻ� �˶� ����Ʈ ���� ���� 
		OnAlarm_List_State_Change(mn_type_select);	// ���� ���� ���� ���� ��ư ���� ���� �Լ�
	}

	OnAlarm_List_Reset_AllItem();					// ����Ʈ ��Ʈ�ѿ� ������ ��� ������ ���� ���� �� �� ���� �Լ�
}

void CScreen_List_Alarm::OnSelectMonthcalAlarm(NMHDR* pNMHDR, LRESULT* pResult) 
{
	OnAlarm_List_Display_Selected_Day();			// ���� ���õ� ��¥ ���� ������� ����� ���� ���� ���� �Լ�
	
	*pResult = 0;
}

void CScreen_List_Alarm::OnSelchangeListAlarmFile() 
{
	int mn_index = m_list_alarm_file.GetCurSel();  // ���� ���õ� ����Ʈ ��Ʈ�� ��ġ ���� ���� 

	if (mn_index != LB_ERR)  // ������ ���ÿ� ���� �߻��ߴ��� �˻� 
	{
		m_list_alarm_file.GetText(mn_index, mstr_alarm_file_name);				// ���� ���õ� ���� ���� ��´� 

		OnAlarm_List_Selected_File_Load(mstr_full_path, mstr_alarm_file_name);	// ���õ� ���� ���� ��� �Լ�

		m_msg_alarm_file_name.SetWindowText(_T(mstr_alarm_file_name));			// ���õ� ������ �̸��� ��� ��Ų��.		
	}
}

BOOL CScreen_List_Alarm::DestroyWindow() 
{
	// TODO: Add your specialized code here and/or call the base class
	/* ************************************************************************** */
	/* ������ ��Ʈ ���� �����Ѵ�                                                  */
	/* ************************************************************************** */
	delete mp_alarm_list_font;
	mp_alarm_list_font = NULL;
	/* ************************************************************************** */
	return CFormView::DestroyWindow();
}
