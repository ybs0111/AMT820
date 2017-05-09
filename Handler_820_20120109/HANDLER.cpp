// HANDLER.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "HANDLER.h"

#include "MainFrm.h"
#include "HANDLERDoc.h"
#include "HANDLERView.h"

#include "Screen_Main.h"  // 초기에 메인 화면을 출력하기 위하여 추가 

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/* ****************************************************************************** */
/* TCP/IP 통신 관련 정보 정의                                                     */
/* ****************************************************************************** */
#define WSA_VERSION  MAKEWORD(2,0)
#define MAX_HOSTNAME 256
#define MAX_HOSTADDR 40
/* ****************************************************************************** */

/////////////////////////////////////////////////////////////////////////////
// CHANDLERApp

BEGIN_MESSAGE_MAP(CHANDLERApp, CWinApp)
	//{{AFX_MSG_MAP(CHANDLERApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
	// Standard print setup command
//	ON_COMMAND(ID_FILE_PRINT_SETUP, CWinApp::OnFilePrintSetup)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHANDLERApp construction

CHANDLERApp::CHANDLERApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CHANDLERApp object

CHANDLERApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CHANDLERApp initialization

BOOL CHANDLERApp::InitInstance()
{
	if (!CheckSingleInstance(IDR_MAINFRAME))	// 생성할 응용 프로그램이 이미 존재하는지 검사..Need for Single Instance
		return FALSE;
	
 	if (!AfxSocketInit())
 	{
 		AfxMessageBox(IDP_SOCKETS_INIT_FAILED);
 		return FALSE;
 	}

	/* ************************************************************************** */
	/* 소켓 초기화 작업 진행한다                                                  */
	/* ************************************************************************** */
/*	WSADATA		WSAData = { 0 };
	if ( 0 != WSAStartup( WSA_VERSION, &WSAData ) )
	{
		// Tell the user that we could not find a usable
		// WinSock DLL.
		if ( LOBYTE( WSAData.wVersion ) != LOBYTE(WSA_VERSION) ||
			HIBYTE( WSAData.wVersion ) != HIBYTE(WSA_VERSION) )
			::MessageBox(NULL, _T("Incorrect version of Winsock.dll found"), _T("Error"), MB_OK);
		
		WSACleanup( );
		return FALSE;
	}*/
	/* ************************************************************************** */

	AfxEnableControlContainer();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	TAPI_LoadSystemButtonImage();

	// Change the registry key under which our settings are stored.
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization.
	SetRegistryKey(_T("Local AppWizard-Generated Applications"));

	LoadStdProfileSettings();  // Load standard INI file options (including MRU)

	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views.

	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_EMPTY,
		RUNTIME_CLASS(CHANDLERDoc),
		RUNTIME_CLASS(CMainFrame),       // main SDI frame window
//		RUNTIME_CLASS(CHANDLERView));
		RUNTIME_CLASS(CScreen_Main));
	AddDocTemplate(pDocTemplate);

	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	// Dispatch commands specified on the command line
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	// The one and only window has been initialized, so show and update it.
	m_pMainWnd->ShowWindow(SW_SHOWMAXIMIZED);
	m_pMainWnd->UpdateWindow();


	//////////////////////////////////////////////////////////////////////////
	//Func.CaseAssemblyConnect();
	WORD         wTotalBoard , wInitialCode;
	wInitialCode=PIODA_DriverInit();
	if( wInitialCode!=PIODA_NoError )
	{
		//      MessageBox(hwnd,"No PIO-DA4/DA8/DA16 card in this system !!!","PIO Card Error",MB_OK);
	}
	
	wTotalBoard = PIODA_SearchCard(&wTotalBoard,PIO_DA);
	if( wTotalBoard==0 )
	{
		////
	}
	//////////////////////////////////////////////////////////////////////////
	return TRUE;
}


/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
		// No message handlers
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// App command to run the dialog
void CHANDLERApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

/////////////////////////////////////////////////////////////////////////////
// CHANDLERApp message handlers


int CHANDLERApp::ExitInstance() 
{
	TAPI_DeleteSystemButtonImage();
	
	return CWinApp::ExitInstance();
}

void CAboutDlg::PostNcDestroy() 
{
	// TODO: Add your specialized code here and/or call the base class
	
	CDialog::PostNcDestroy();
}
