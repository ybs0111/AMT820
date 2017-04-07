// Run_Motors.cpp: implementation of the CRun_Motors class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "handler.h"
#include "Run_Motors.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CRun_Motors	g_Run_Motors;

CRun_Motors::CRun_Motors()
{
	for( int i=0; i<MOTOR_COUNT; i++ )
	{
		m_motor[i].SetMotorID( i );
	}
}

CRun_Motors::~CRun_Motors()
{

}

void CRun_Motors::Thread_Run()
{
	for( int i=0; i<M_MOTOR_COUNT; i++ )
	{
		m_motor[i].MotorMoving_repeat();
		m_motormap[i].MotorLMoving_repeat();
	}
}