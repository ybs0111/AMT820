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
#include "Run_Device_Carrier_Robot.h"
#include "Run_DvcLdBuffer.h"
#include "Run_EmptyStacker_Elvator.h"
#include "Run_EmptyTrayTransfer.h"
#include "Run_HeatSinkVision_Transfer_Robot.h"
#include "Run_UnPress_Robot.h"
#include "Run_LdPicker.h"
#include "Run_LdStacker_Elvator.h"
#include "Run_LdTrayPlate.h"
#include "Run_UldPicker.h"
#include "Run_Epoxy_Transfer_Robot.h"
#include "Run_Motors.h"
#include "SrcPart/APartHandler.h"
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
CWinThread  *m_thread[25];
HANDLE		hThrHandle[25];
// ******************************************************************************
// ******************************************************************************
// ���� ���۷��̼� ������ �Լ� �����Ѵ�                                          
// ******************************************************************************
UINT	OnThread_Handler_Check(LPVOID lParam);			// m_thread[0]
UINT	OnThread_Tower_Lamp_Check(LPVOID lParam);		// m_thread[1]
UINT	OnThread_Alarm_Display(LPVOID lParam);			// m_thread[2]
UINT	OnThread_IO_Check(LPVOID lParam);				// m_thread[3]
UINT	OnThread_Motors(LPVOID lParam);					// m_thread[4]

UINT	OnThread_LdStacker_Elivator(LPVOID lParam);		// m_thread[5]
UINT	OnThread_Load_Plate(LPVOID lParam);				// m_thread[6]
UINT	OnThread_Tray_Transfer(LPVOID lParam);			// m_thread[7]
UINT	OnThread_Load_Picker(LPVOID lParam);			// m_thread[8]
UINT	OnThread_Load_Buffer(LPVOID lParam);			// m_thread[9]
UINT	OnThread_Unload_Picker(LPVOID lParam);			// m_thread[10]
UINT	OnThread_UnPress_Robot(LPVOID lParam);			// m_thread[11]
UINT	OnThread_Epoxy_Robot(LPVOID lParam);			// m_thread[12]
UINT	OnThread_Carrier_Robot(LPVOID lParam);			// m_thread[13]
UINT	OnThread_HeatSink_Robot(LPVOID lParam);			// m_thread[14]
UINT    OnThread_EmptyStacker_Elivator(LPVOID lParam);	// m_thread[15]




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
		Run_Handler_Check.ButtonCheck_Auto();
		Run_Handler_Check.ButtonCheck_Manual();
		Run_Handler_Check.ButtonSendData();
		g_handler.Thread_Run();
// 		Run_Handler_Check.HandlerCheck_Safety();
// 		Run_Handler_Check.StackerMutingCheck();



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
	return TRUE;
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
UINT OnThread_Alarm_Display(LPVOID lParam)
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

UINT OnThread_IO_Check(LPVOID lParam)
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

UINT OnThread_Motors(LPVOID lParam)					// m_thread[4]
{
	CSingleLock sing(&mutex);
	
	while(!All_Stop)
	{
		sing.Lock() ;
		
		g_Run_Motors.Thread_Run();
		
		Sleep(0) ;
		sing.Unlock() ;
	}
	
	hThrHandle[4] = NULL;
	return TRUE;
}

UINT	OnThread_LdStacker_Elivator(LPVOID lParam)			// m_thread[5]
{
	CSingleLock sing(&mutex);
	
	while(!All_Stop)
	{
		sing.Lock() ;
		
		Run_LdStacker_Elvator.Thread_Run();
		
		Sleep(0) ;
		sing.Unlock() ;
	}
	
	hThrHandle[5] = NULL;
	return TRUE;
}

UINT	OnThread_Load_Plate(LPVOID lParam)			// m_thread[6]
{
	CSingleLock sing(&mutex);
	
	while(!All_Stop)
	{
		sing.Lock() ;
		
		Run_LdTrayPlate.Thread_Run();
		
		Sleep(0) ;
		sing.Unlock() ;
	}
	
	hThrHandle[6] = NULL;
	return TRUE;
}

UINT	OnThread_Tray_Transfer(LPVOID lParam)			// m_thread[7]
{
	CSingleLock sing(&mutex);
	
	while(!All_Stop)
	{
		sing.Lock() ;
		
		Run_EmptyTrayTransfer.Thread_Run();
		
		Sleep(0) ;
		sing.Unlock() ;
	}
	
	hThrHandle[7] = NULL;
	return TRUE;

}

UINT	OnThread_Load_Picker(LPVOID lParam)			// m_thread[7]
{
	CSingleLock sing(&mutex);
	
	while(!All_Stop)
	{
		sing.Lock() ;
		
		Run_LdPicker.Thread_Run();
		
		Sleep(0) ;
		sing.Unlock() ;
	}
	
	hThrHandle[8] = NULL;
	return TRUE;
}

UINT	OnThread_Load_Buffer(LPVOID lParam)			// m_thread[9]
{
	CSingleLock sing(&mutex);
	
	while(!All_Stop)
	{
		sing.Lock() ;
		
		Run_DvcLdBuffer.Thread_Run();
		
		Sleep(0) ;
		sing.Unlock() ;
	}
	
	hThrHandle[9] = NULL;
	return TRUE;
}

UINT	OnThread_Unload_Picker(LPVOID lParam)			// m_thread[8]
{
	CSingleLock sing(&mutex);
	
	while(!All_Stop)
	{
		sing.Lock() ;
		
		Run_UldPicker.Thread_Run();
		
		Sleep(0) ;
		sing.Unlock() ;
	}
	
	hThrHandle[10] = NULL;
	return TRUE;
}


UINT	OnThread_UnPress_Robot(LPVOID lParam)			// m_thread[9]
{
	CSingleLock sing(&mutex);
	
	while(!All_Stop)
	{
		sing.Lock() ;
		
		Run_UnPress_Robot.Thread_Run();
		
		Sleep(0) ;
		sing.Unlock() ;
	}
	
	hThrHandle[11] = NULL;
	return TRUE;
}

UINT	OnThread_Epoxy_Robot(LPVOID lParam)			// m_thread[9]
{
	CSingleLock sing(&mutex);
	
	while(!All_Stop)
	{
		sing.Lock() ;
		
		Run_Epoxy_Transfer_Robot.Thread_Run();
		
		Sleep(0) ;
		sing.Unlock() ;
	}
	
	hThrHandle[12] = NULL;
	return TRUE;
}

UINT	OnThread_Carrier_Robot(LPVOID lParam)			// m_thread[10]
{
	CSingleLock sing(&mutex);
	
	while(!All_Stop)
	{
		sing.Lock() ;
		
		Run_Device_Carrier_Robot.Thread_Run();
		
		Sleep(0) ;
		sing.Unlock() ;
	}
	
	hThrHandle[13] = NULL;
	return TRUE;

}

UINT	OnThread_HeatSink_Robot(LPVOID lParam)			// m_thread[11]
{
	CSingleLock sing(&mutex);
	
	while(!All_Stop)
	{
		sing.Lock() ;
		
		Run_HeatSinkVision_Transfer_Robot.Thread_Run();
		
		Sleep(0) ;
		sing.Unlock() ;
	}
	
	hThrHandle[14] = NULL;
	return TRUE;
		
}

UINT	OnThread_EmptyStacker_Elivator(LPVOID lParam)			// m_thread[15]
{
	CSingleLock sing(&mutex);
	
	while(!All_Stop)
	{
		sing.Lock() ;
		
		Run_EmptyStacker_Elvator.Thread_Run();
		
		Sleep(0) ;
		sing.Unlock() ;
	}
	
	hThrHandle[15] = NULL;
	return TRUE;
}
