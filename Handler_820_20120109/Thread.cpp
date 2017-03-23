// Thread.cpp: implementation of the Thread class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "iostream.h"
#include "afxmt.h"
#include "Thread.h"

#include "Variable.h"  // ���� ���� ���� Ŭ����

// ******************************************************************************
// ������ ���� ���� Ŭ���� �߰�                                                  
// ******************************************************************************
#include "Run_Handler_Check.h"
#include "Run_TowerLamp.h"
#include "Run_IO_Check.h"
// ******************************************************************************

#include "MyJamData.h"  // �˶� ��� ���� �ε� �� �˶� �޽��� ���� Ŭ���� 

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

// ******************************************************************************
// �˶� ��� Ŭ���� ���� ����                                                    
// ******************************************************************************
CMyJamData   mcls_alarm_disp;  // �߻� �˶� ��� ���� �ε� Ŭ���� ����
// ******************************************************************************
// ******************************************************************************
// ������ �� �ڵ� ���� ���� ����                                                 
// ******************************************************************************
CWinThread  *m_thread[40];
HANDLE		hThrHandle[40];
// ******************************************************************************
// ******************************************************************************
// ���� ���۷��̼� ������ �Լ� �����Ѵ�                                          
// ******************************************************************************
UINT  OnThread_Handler_Check(LPVOID lParam);		// m_thread[0]
UINT  OnThread_Tower_Lamp_Check(LPVOID lParam);		// m_thread[1]
UINT  OnThread_Alarm_Display(LPVOID lParam);		// m_thread[2]
UINT  OnThread_IO_Check(LPVOID lParam);

CMutex mutex ;

int All_Stop;			// ������ ���� �÷���
int threadtimer[3];
int countflag = 0;
// ******************************************************************************
//  ����ġ �˻� ���� ���� ������ �Լ�                                            
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
//  Ÿ�� ���� ��� ���� ���� ������ �Լ�                                         
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
//  ��� ���� �� �߻��� �˶� ����ϱ� ���� ������ �Լ�                           
// ******************************************************************************
UINT OnThread_Alarm_Display(LPVOID lParam)  // m_Thread[12]
{
	CSingleLock sing(&mutex);

	while(!All_Stop)
	{
		sing.Lock() ;
		
		mcls_alarm_disp.On_Alarm_Display(); // �˶� �߻��� ��� �˶� ȭ�� ��� �Լ�
		
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