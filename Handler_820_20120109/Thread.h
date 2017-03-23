#ifndef _AFX_THREAD_H__
#define _AFX_THREAD_H__

// ******************************************************************************
// 쓰레드 및 쓰레드 핸들 저장 변수 선언                                          
// ******************************************************************************
extern CWinThread  *m_thread[40] ;
extern HANDLE		hThrHandle[40];
// ******************************************************************************

// ******************************************************************************
// 전역 쓰레드 함수 선언한다                                                     
// ******************************************************************************
extern UINT  OnThread_Handler_Check(LPVOID lParam);				// m_thread[0]
extern UINT  OnThread_Tower_Lamp_Check(LPVOID lParam);			// m_thread[1]
extern UINT  OnThread_Alarm_Display(LPVOID lParam);				// m_thread[2]
extern UINT  OnThread_IO_Check(LPVOID lParam);				// m_thread[3]

extern int All_Stop ;  // 쓰레드 상태 플래그

#endif