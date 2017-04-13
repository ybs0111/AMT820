#ifndef _AFX_THREAD_H__
#define _AFX_THREAD_H__

// ******************************************************************************
// 쓰레드 및 쓰레드 핸들 저장 변수 선언                                          
// ******************************************************************************

enum 
{
	THREAD_HANDLER_CHK = 0,
	THREAD_TOWER_LAMP_CHK,
	THREAD_ALARM_DISPLAY,
	THREAD_IO,
	THREAD_MOTORS,

	THREAD_LDSTACKER_ELIVATOR,
	THREAD_EMPTYSTACKER_ELIVATOR,
	THREAD_EMPTYTRAY_TRANSFER,
	THREAD_LDOAD_PLATE,
	THREAD_TRAY_TRANSFER,
	THREAD_LOAD_PICKER,
	THREAD_LOAD_BUFFER,
	THREAD_UNLAOAD_PICKER,

	THREAD_UNPRESS_ROBOT,
	THREAD_EPOXY_ROBOT,
	THREAD_CARRIER_ROBOT,
	THREAD_HEATSINK_ROBOT, //THREAD_DISPENSOR_ROBOT THREAD_VISION_ROBOT,

	THREAD_COUNT,
};

extern CWinThread  *m_thread[25] ;
extern HANDLE		hThrHandle[25];
// ******************************************************************************

// ******************************************************************************
// 전역 쓰레드 함수 선언한다                                                     
// ******************************************************************************
extern UINT OnThread_Handler_Check(LPVOID lParam);				// m_thread[0]
extern UINT OnThread_Tower_Lamp_Check(LPVOID lParam);			// m_thread[1]
extern UINT OnThread_Alarm_Display(LPVOID lParam);				// m_thread[2]
extern UINT OnThread_IO_Check(LPVOID lParam);					// m_thread[3]
extern UINT	OnThread_Motors(LPVOID lParam);						// m_thread[4]


extern UINT	OnThread_LdStacker_Elivator(LPVOID lParam);			// m_thread[5]
extern UINT	OnThread_Load_Plate(LPVOID lParam);					// m_thread[6]
extern UINT	OnThread_Tray_Transfer(LPVOID lParam);				// m_thread[7]
extern UINT	OnThread_Load_Picker(LPVOID lParam);				// m_thread[8]
extern UINT	OnThread_Load_Buffer(LPVOID lParam);				// m_thread[9]
extern UINT	OnThread_Unload_Picker(LPVOID lParam);				// m_thread[10]
extern UINT	OnThread_UnPress_Robot(LPVOID lParam);				// m_thread[11]
extern UINT	OnThread_Epoxy_Robot(LPVOID lParam);				// m_thread[12]
extern UINT	OnThread_Carrier_Robot(LPVOID lParam);				// m_thread[13]
extern UINT	OnThread_HeatSink_Robot(LPVOID lParam);				// m_thread[14]
extern UINT	OnThread_EmptyStacker_Elivator(LPVOID lParam);	    // m_thread[15]




extern int All_Stop ;  // 쓰레드 상태 플래그

#endif