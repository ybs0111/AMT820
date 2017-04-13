/*------------------------------------------------------------------------
CLASS
	AutodetectMemoryLeak

DESCRIPTION
	����� ��忡�� memory leak�� �ڵ����� �����Ͽ� STDOUT�̳� ����� â �Ǵ� STDOUT���� 
 	�ѷ��ش�. 
		- new�� �Ҵ�� �޸𸮿� ������ ���� ��� �ҽ����� ��Ȯ�� ��ġ(�ٹ�ȣ)�� ����
			MFC AppWizard�� ����(Ư�� GUI)�� ��쿣 VC�� �˾Ƽ� ���ֱ⶧���� �ʿ�����ϴ�.
		- malloc���� �Ҵ�� �޸𸮿� ������ ���� ��� ��ġ�� ����

	* �⺻������ _CRTDBG_MAP_ALLOC�� ������ ���ָ� �Ϲ����� ������ �߻��� �մϴٸ�
	  ��Ȯ�� �ٹ�ȣ�� ��Ÿ���ֱ� ���ؼ��� �����ǰ� �ʿ��� ��찡 �ֽ��ϴ�.

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

#define _CRTDBG_MAP_ALLOC // �޸� ������ Ž���ϱ� ���� ���� ���־�� �Ѵ�.
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

// Consol Application�� ���
#if defined (_CONSOLE)
            // Send all reports to STDOUT
            _CrtSetReportMode( _CRT_WARN, _CRTDBG_MODE_FILE);
            _CrtSetReportFile( _CRT_WARN, _CRTDBG_FILE_STDOUT);
            _CrtSetReportMode( _CRT_ERROR, _CRTDBG_MODE_FILE);
            _CrtSetReportFile( _CRT_ERROR, _CRTDBG_FILE_STDOUT);
            _CrtSetReportMode( _CRT_ASSERT, _CRTDBG_MODE_FILE);
            _CrtSetReportFile( _CRT_ASSERT, _CRTDBG_FILE_STDOUT);


            /* new�� �Ҵ�� �޸𸮿� ������ ���� ��� �ҽ����� ��Ȯ�� ��ġ�� �������ش�.
             *  �� _AFXDLL�� ����Ҷ��� �ڵ����� ������ CONSOLE ��忡�� �Ʒ�ó��
             * �����Ǹ� ���־�߸� �մϴ�.
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
             * malloc���� �Ҵ�� �޸𸮿� ������ ���� ��� ��ġ�� ����
             * CONSOLE ����� ��� crtdbg.h�� malloc�� ���ǵǾ� ������,
             * _AXFDLL ����� ��쿡�� �ణ �ٸ�������� ���� �ϰԵȴ�.
             * date: 2001-01-30
             */
            #define malloc(s) (_malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ ))
        }
};

// �� ���� �ʱ�ȭ�� �����ڸ� ���� �ڵ����� ���ֱ� ���� �������� �����Ѵ�.
static __AutoDetectMemoryLeak __autoDetectMemoryLeak;

#endif // if defined(_MSC_VER) && defined (_DEBUG)

#endif // __yonghany_AutoDetectMemoryLeak_h__
