// APartErrReport.cpp: implementation of the APartErrReport class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "APartErrReport.h"
#include <DbgHelp.h>
#include "PartFunction.h"
//#include <io.h>
//#include <direct.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

LPTOP_LEVEL_EXCEPTION_FILTER PreviousExceptionFilter = NULL;

BOOL CMiniDump::Begin(VOID)
{
	PreviousExceptionFilter = SetUnhandledExceptionFilter(UnHandledExceptionFilter);
	return true;
}

BOOL CMiniDump::End(VOID)
{
	SetUnhandledExceptionFilter(PreviousExceptionFilter);
	return true;
}


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

APartErrReport::APartErrReport()
{

}

APartErrReport::~APartErrReport()
{

}
