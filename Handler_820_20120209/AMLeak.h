/*------------------------------------------------------------------------
CLASS
	AutodetectMemoryLeak

DESCRIPTION
	디버그 모드에서 memory leak을 자동으로 감지하여 STDOUT이나 디버그 창 또는 STDOUT으로 
 	뿌려준다. 
		- new로 할당된 메모리에 누수가 있을 경우 소스상의 정확한 위치(줄번호)를 덤프
			MFC AppWizard로 생성(특히 GUI)한 경우엔 VC가 알아서 해주기때문에 필요없습니다.
		- malloc으로 할당된 메모리에 누수가 있을 경우 위치를 덤프

	* 기본적으로 _CRTDBG_MAP_ALLOC만 디파인 해주면 일반적인 덤프는 발생을 합니다만
	  정확한 줄번호를 나타내주기 위해서는 재정의가 필요한 경우가 있습니다.

	EXAMPLE_CODE
		void genMemoryLeak ()
		{
			TCHAR *p1 = new TCHAR [10];
			_tcscpy ( p1, _T("p1=new") );
			TCHAR *p2 = (TCHAR*) malloc ( sizeof(TCHAR)*10 );
			_tcscpy  ( p2, L"p2=malloc" );
		}
	END

	RESULT_AFXDLL
		Detected memory leaks!
		Dumping objects ->
		F:\my\dlgtest\dlgtestDlg.cpp(186) : {163} normal block at 0x003470C8, 20 bytes long.
		 Data: <p 2 = m a l l o > 70 00 32 00 3D 00 6D 00 61 00 6C 00 6C 00 6F 00 
		F:\my\dlgtest\dlgtestDlg.cpp(183) : {162} normal block at 0x00347070, 20 bytes long.
		 Data: <p 1 = n e w     > 70 00 31 00 3D 00 6E 00 65 00 77 00 00 00 CD CD 
		Object dump complete.
	END

	RESULT_CONSOLE
		Detected memory leaks!
		Dumping objects ->
		F:\my\libTester\test.cpp(38) : {1094} normal block at 0x00341148, 10 bytes long.
		 Data: <p1=new    > 70 31 3D 6E 65 77 00 CD CD CD
		F:\my\libTester\test.cpp(35) : {1093} normal block at 0x00342570, 10 bytes long.
		 Data: <p1=malloc > 70 31 3D 6D 61 6C 6C 6F 63 00
		Object dump complete.
	END

AUTHOR
	yonghany
	contact: yonghany@orgio.net

DATE
	2000-11-18
	2000-12-05
	2001-01-30
------------------------------------------------------------------------*/
#if !defined (__yonghany_AutoDetectMemoryLeak_h__)
#define __yonghany_AutoDetectMemoryLeak_h__

#if defined(_MSC_VER) && defined (_DEBUG)

#define _CRTDBG_MAP_ALLOC // 메모리 누수를 탐지하기 위해 선언 해주어야 한다.
#include <crtdbg.h>
#if !defined (_CONSOLE)
    #include <cstdlib> // for Consol Application
#endif

class __AutoDetectMemoryLeak
{
public:
    __AutoDetectMemoryLeak ()
        {
            _CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

// Consol Application인 경우
#if defined (_CONSOLE)
            // Send all reports to STDOUT
            _CrtSetReportMode( _CRT_WARN, _CRTDBG_MODE_FILE);
            _CrtSetReportFile( _CRT_WARN, _CRTDBG_FILE_STDOUT);
            _CrtSetReportMode( _CRT_ERROR, _CRTDBG_MODE_FILE);
            _CrtSetReportFile( _CRT_ERROR, _CRTDBG_FILE_STDOUT);
            _CrtSetReportMode( _CRT_ASSERT, _CRTDBG_MODE_FILE);
            _CrtSetReportFile( _CRT_ASSERT, _CRTDBG_FILE_STDOUT);


            /* new로 할당된 메모리에 누수가 있을 경우 소스상의 정확한 위치를 덤프해준다.
             *  ※ _AFXDLL을 사용할때는 자동으로 되지만 CONSOLE 모드에서 아래처럼
             * 재정의를 해주어야만 합니다.
             *    date: 2000-12-05
             */
            #define DEBUG_NORMALBLOCK   new ( _NORMAL_BLOCK, __FILE__, __LINE__)
            #ifdef new
            #undef new
            #endif
            #define new DEBUG_NORMALBLOCK

#else

            // Send all reports to DEBUG window
            _CrtSetReportMode( _CRT_WARN, _CRTDBG_MODE_DEBUG);
            _CrtSetReportMode( _CRT_ERROR, _CRTDBG_MODE_DEBUG);
            _CrtSetReportMode( _CRT_ASSERT, _CRTDBG_MODE_DEBUG);

#endif

            #ifdef malloc
            #undef malloc
            #endif
            /*
             * malloc으로 할당된 메모리에 누수가 있을 경우 위치를 덤프
             * CONSOLE 모드일 경우 crtdbg.h에 malloc이 정의되어 있지만,
             * _AXFDLL 모드일 경우에는 약간 다른방식으로 덤프 하게된다.
             * date: 2001-01-30
             */
            #define malloc(s) (_malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ ))
        }
};

// 몇 가지 초기화를 생성자를 통해 자동으로 해주기 위해 전역으로 선언한다.
static __AutoDetectMemoryLeak __autoDetectMemoryLeak;

#endif // if defined(_MSC_VER) && defined (_DEBUG)

#endif // __yonghany_AutoDetectMemoryLeak_h__
