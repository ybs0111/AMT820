// Thread.cpp: implementation of the Thread class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "iostream.h"
#include "afxmt.h"
#include "Thread.h"

#include "Variable.h"  // 전역 변수 정의 클래스

// ******************************************************************************
// 쓰레드 동작 관련 클래스 추가                                                  
// ******************************************************************************
#include "Run_Handler_Check.h"
#include "Run_TowerLamp.h"
#include "Run_IO_Check.h"
// ******************************************************************************

#include "MyJamData.h"  // 알람 출력 정보 로딩 및 알람 메시지 저장 클래스 

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

// ******************************************************************************
// 알람 출력 클래스 변수 선언                                                    
// ******************************************************************************
CMyJamData   mcls_alarm_disp;  // 발생 알람 출력 정보 로딩 클래스 변수
// ******************************************************************************
// ******************************************************************************
// 쓰레드 및 핸들 저장 변수 선언                                                 
// ******************************************************************************
CWinThread  *m_thread[40];
HANDLE		hThrHandle[40];
// ******************************************************************************
// ******************************************************************************
// 전역 오퍼레이션 쓰레드 함수 선언한다                                          
// ******************************************************************************
UINT  OnThread_Handler_Check(LPVOID lParam);		// m_thread[0]
UINT  OnThread_Tower_Lamp_Check(LPVOID lParam);		// m_thread[1]
UINT  OnThread_Alarm_Display(LPVOID lParam);		// m_thread[2]
UINT  OnThread_IO_Check(LPVOID lParam);

CMutex mutex ;

int All_Stop;			// 쓰레드 상태 플래그
int threadtimer[3];
int countflag = 0;
// ******************************************************************************
//  스위치 검사 동작 제어 쓰레드 함수                                            
// ******************************************************************************
UINT OnThread_Handler_Check(LPVOID lParam)  // m_Thread[10]
{
	CSingleLock sing(&mutex);

	while (!All_Stop)
	{
		sing.Lock();

		Run_Handler_Check.ButtonCheck_Start();
		Run_Handler_Check.ButtonCheck_Stop();
		Run_Handler_Check.ButtonCheck_Reset();
		Run_Handler_Check.ButtonCheck_BuzzerOff();
		Run_Handler_Check.ButtonCheck_Loader();
		Run_Handler_Check.HandlerCheck_RejectTray();

	    Sleep(0);
		sing.Unlock();
	}

	hThrHandle[0] = NULL;
	return TRUE;
}
// ******************************************************************************

// ******************************************************************************
//  타워 램프 출력 동작 제어 쓰레드 함수                                         
// ******************************************************************************
UINT OnThread_Tower_Lamp_Check(LPVOID lParam)  // m_Thread[11]
{
	CSingleLock sing(&mutex) ;

	while(!All_Stop)
	{
		sing.Lock();

 		st_handler.threadcount++;
		
 		if (countflag == 0) 
 		{
 			threadtimer[0] = GetCurrentTime();
 			countflag =1;
 			st_handler.threadcount=0;
 		}
 		else 
 		{
 			threadtimer[1] = GetCurrentTime();
 			threadtimer[2] = threadtimer[1] - threadtimer[0];
 			if (threadtimer[2] > 1000)
 			{
 				countflag = 0;
 				st_handler.threadcount = 0;
 			}
 		}

		Run_TowerLamp.Thread_Run();
	    Sleep(0);
		sing.Unlock();
	}

	hThrHandle[1] = NULL;
	return TRUE;
}
// ******************************************************************************

// ******************************************************************************
//  장비 동작 중 발생한 알람 출력하기 위한 쓰레드 함수                           
// ******************************************************************************
UINT OnThread_Alarm_Display(LPVOID lParam)  // m_Thread[12]
{
	CSingleLock sing(&mutex);

	while(!All_Stop)
	{
		sing.Lock() ;
		
		mcls_alarm_disp.On_Alarm_Display(); // 알람 발생한 경우 알람 화면 출력 함수
		
		Sleep(0) ;
		sing.Unlock() ;
	}

	hThrHandle[2] = NULL;
	return TRUE;
}

UINT OnThread_IO_Check(LPVOID lParam)  // m_Thread[12]
{
	CSingleLock sing(&mutex);
	
	while(!All_Stop)
	{
		sing.Lock() ;

		Run_IO_Check.Thread_Run();
		
		Sleep(0) ;
		sing.Unlock() ;
	}
	
	hThrHandle[3] = NULL;
	return TRUE;
}