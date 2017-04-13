// APartHandler.cpp: implementation of the APartHandler class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "APartHandler.h"
#include "../IO_Manager.h"
#include "../ComizoaPublic.h"
//#include "APartHandler_COK.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
APartHandler	g_handler;

APartHandler::APartHandler()
{
	m_empos = EMPOS_FRONT;

	m_step_conv[0] = EC_NONE;
	m_step_conv[1] = EC_NONE;
	m_step_conv[2] = EC_NONE;
}

APartHandler::~APartHandler()
{

}

void APartHandler::Thread_Run()
{
	Run_Move_SMEMA();

	if( COMI.mn_run_status != dRUN )	
		return;

	/*if( g_handler_cok.GetChangingCOK() == false )
	{
		Run_Move_Main();

		Run_Move_Conv1();
	}
	else
	{
		g_handler_cok.Run_Move();
	}*/
}

void APartHandler::Run_Move_SMEMA()
{
	// Front Shuttle Request Signal
	BOOL bRequest = IO_ON;
//	if( COMI.mn_run_status != dRUN)				{ bRequest = IO_OFF; goto ioset; }
//	if( g_handler_cok.GetChangingCOK() )		{ bRequest = IO_OFF; goto ioset; }
//	if( m_step_conv[0] != EC_NONE )				{ bRequest = IO_OFF; goto ioset; }
//	if( g_ioMgr.get_in_bit( st_io.i_bin_tray1_stop_position_chk ) == IO_ON )
//												{ bRequest = IO_OFF; goto ioset; }
	// CheckSpace...

//ioset:
//	g_ioMgr.set_out_bit( st_io.o_front_shuttle_request_signal, bRequest );
}

void APartHandler::Run_Move_Conv( int nIdx )
{
	switch( m_step_conv[nIdx] )
	{
	case EC_NONE:		break;

	case EC_RECEIVE:
		{
//			Run_Bin_Stacker[nIdx].ReqRecvTray();
			m_step_conv[nIdx] = EC_RECEIVE_CHECK;
		}
		break;

	case EC_RECEIVE_CHECK:
		{
//			if( Run_Bin_Stacker[nIdx].GetRecvTrayNone() )
				m_step_conv[nIdx] = EC_NONE;
		}
		break;

	case EC_UP:
//		Run_Bin_Stacker[nIdx].ReqTrayUp();
//		m_step_conv[nIdx] = EC1_UP_CHK;
		break;

	case EC_UP_CHK:
//		if( Run_Bin_Stacker[nIdx].GetTrayUpNone() )
//		{
			m_step_conv[nIdx] = EC_NONE;
//		}
		break;

	case EC_DOWN:
//		Run_Bin_Stacker[nIdx].ReqTrayDown();
		m_step_conv[nIdx] = EC_DOWN_CHK;
		break;

	case EC_DOWN_CHK:
//		if( Run_Bin_Stacker[nIdx].GetTrayDownNone() )
		{
			m_step_conv[nIdx] = EC_NONE;
		}
		break;

	case EC_SEND:
//		Run_Bin_Stacker[nIdx].ReqTraySend();
		m_step_conv[nIdx] = EC_SEND_CHK;
		break;

	case EC_SEND_CHK:
//		if( Run_Bin_Stacker[nIdx].GetTraySendNone() )
		{
			m_step_conv[nIdx] = EC_NONE;
		}
		break;
	}
}

void APartHandler::Run_Move_Main()
{
//	if( m_step_conv[0] == EC_NONE &&
//		g_ioMgr.get_in_bit( st_io.i_front_shuttle_transfer_signal_chk ) == IO_ON &&
//		g_ioMgr.get_out_bit( st_io.o_front_shuttle_request_signal, IO_ON ) == IO_ON )
//	{
//		ControlConv(0, EC_RECEIVE );
//	}

	// test


}

void APartHandler::ControlConv( int iIdx, ESTEP_CONV ec )
{
	m_step_conv[iIdx] = ec;
}
