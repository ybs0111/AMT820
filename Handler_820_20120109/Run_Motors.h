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

enum ENUM_MOTOR
{
	MOTOR_FRONT_SHIFTER_Z = 				0,
	MOTOR_FRONT_SHIFTER_Y,
	MOTOR_FRONT_SHIFTER_GRIPPER,
	MOTOR_FRONT_SHIFTER_X,
	
	MOTOR_REAR_SHIFTER_Z,
	MOTOR_REAR_SHIFTER_Y,
	MOTOR_REAR_SHIFTER_GRIPPER,
	MOTOR_REAR_SHIFTER_X,
	
	MOTOR_FRONT_BUFFER_1,
	MOTOR_FRONT_BUFFER_2,
	MOTOR_REAR_BUFFER_1,
	MOTOR_REAR_BUFFER_2,
	
	MOTOR_RETEST_ROBOT_Y,
	MOTOR_RETEST_ROBOT_Z,
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
