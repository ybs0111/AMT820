// APartShortkey.cpp: implementation of the APartShortkey class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
//#include "handler.h"
#include "APartShortkey.h"
#include "..\SrcBase\ALocalization.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
APartShortkey	g_ShortKey;

APartShortkey::APartShortkey()
{
}

APartShortkey::~APartShortkey()
{
}

void APartShortkey::CheckKeyboard( MSG* pMsg )
{
	// Alt + ??
	if( pMsg->message == WM_SYSKEYDOWN )
	{
		switch( pMsg->wParam )
		{

		case 'K':	g_local.SetLocal( LOCAL_KOR );	break;
		case 'E':	g_local.SetLocal( LOCAL_ENG );	break;
		case 'C':	g_local.SetLocal( LOCAL_CHN );	break;

		case 'L':	g_local.SetNextLocal();			break;

		}
	}


}
