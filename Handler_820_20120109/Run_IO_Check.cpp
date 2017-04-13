// Run_IO_Check.cpp: implementation of the CRun_IO_Check class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "handler.h"
#include "Run_IO_Check.h"
#include "Variable.h"
#include "FastechPublic_IO.h"
#include "math.h"
#include "IO_Manager.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CRun_IO_Check Run_IO_Check;

CRun_IO_Check::CRun_IO_Check()
{

}

CRun_IO_Check::~CRun_IO_Check()
{

}

void CRun_IO_Check::Thread_Run()
{
	if(g_ioMgr.GetIOBoardInit() == false) return;

	FAS_IO.IO_ReadWrite(0, 0, 0, 1);
	FAS_IO.IO_ReadWrite(0, 0, 1, 1);
	FAS_IO.IO_ReadWrite(0, 0, 2, 1);
	FAS_IO.IO_ReadWrite(0, 0, 3, 1);
	FAS_IO.IO_ReadWrite(0, 0, 4, 1);
	FAS_IO.IO_ReadWrite(0, 0, 5, 1);
	FAS_IO.IO_ReadWrite(0, 0, 6, 1);
	FAS_IO.IO_ReadWrite(0, 0, 7, 1);
	
	FAS_IO.IO_ReadWrite(0, 1, 0, 1);
	FAS_IO.IO_ReadWrite(0, 1, 1, 1);
	FAS_IO.IO_ReadWrite(0, 1, 2, 1);
	FAS_IO.IO_ReadWrite(0, 1, 3, 1);
	FAS_IO.IO_ReadWrite(0, 1, 4, 1);
	FAS_IO.IO_ReadWrite(0, 1, 5, 1);
	FAS_IO.IO_ReadWrite(0, 1, 6, 1);
	FAS_IO.IO_ReadWrite(0, 1, 7, 1);

	FAS_IO.IO_ReadWrite(0, 2, 0, 1);
	FAS_IO.IO_ReadWrite(0, 2, 1, 1);
	FAS_IO.IO_ReadWrite(0, 2, 2, 1);
	FAS_IO.IO_ReadWrite(0, 2, 3, 1);
	FAS_IO.IO_ReadWrite(0, 2, 4, 1);
}


