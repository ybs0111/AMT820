// Run_Motors.h: interface for the CRun_Motors class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RUN_MOTORS_H__BB26A8C9_14F6_4B4C_A4A1_E59CCD4F1FB8__INCLUDED_)
#define AFX_RUN_MOTORS_H__BB26A8C9_14F6_4B4C_A4A1_E59CCD4F1FB8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SrcBase/AMotor.h"

#define GETMOTOR( motorID )				g_Run_Motors.GetMotor( motorID )
#define GETMOTORMAP( motormapID )		g_Run_Motors.GetMotorMap( motormapID )
// 
enum ENUM_MOTOR
{
	TRAY1_Z				= 0,
	TRAY2_Z,				
	PRESS_Y,				
	EPOXY_TRANSFER_X,		
	EPOXY_TRANSFER_Y,		
	EPOXY_TRANSFER_Z,		
	EPOXY_SCREW,
	CARRIER_X,
	
	LOADER_TRANSFER_Y,
	LOADER_TRANSFER_Z,
	HEATSINK_TRANSFER_X,
	HEATSINK_TRANSFER_Y,
	HEATSINK_TRANSFER_Z,
	UNLOADER_TRANSFER_X,
	UNLOADER_TRANSFER_Y,
	UNLOADER_TRANSFER_Z,
	
	DISPENSER_Y,	
	TRAY_REMOVE_X,			
	HEATSINK_INSPECT_Y,
	
	HEATSINK_INSPECT_Z,
	HEATSINK_PICKER_PITCH,
	
	MOTOR_COUNT,
};
class CRun_Motors  
{
public:
	CRun_Motors();
	virtual ~CRun_Motors();

public:
	void Thread_Run();
	AMotor& GetMotor( ENUM_MOTOR motorID ) { return m_motor[motorID]; }
	AMotor& GetMotorMap( int motormapID ) { return m_motormap[motormapID]; }
	
protected:
	AMotor	m_motor[MOTOR_COUNT];
	AMotor	m_motormap[MOTOR_COUNT];
};

extern CRun_Motors	g_Run_Motors;

#endif // !defined(AFX_RUN_MOTORS_H__BB26A8C9_14F6_4B4C_A4A1_E59CCD4F1FB8__INCLUDED_)
