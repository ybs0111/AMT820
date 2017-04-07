// ALocalization.cpp: implementation of the ALocalization class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
//#include "handler.h"
#include "ALocalization.h"
#include "..\Variable.h"

#include <fstream.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

ALocalization g_local;

ALocalization::ALocalization() :
m_mapLocal()
{
	m_lt = LOCAL_KOR;
}

ALocalization::~ALocalization()
{

}

void ALocalization::SetLocal( LOCAL_TYPE lt )
{
	m_lt = lt;

	m_mapLocal.clear();
	LoadLocalFile();

	if( st_handler.cwnd_main )
	{
//		st_handler.cwnd_main->PostMessage( WM_DRAW_UI_MAIN );
	}

}

void ALocalization::SetNextLocal()
{
	LOCAL_TYPE lt = (LOCAL_TYPE)(m_lt + 1);
	if( lt >= LOCAL_MAX )
		lt = LOCAL_KOR;

	SetLocal( lt );
}

void ALocalization::LoadLocalFile()
{
	char szBuffer[256];
	char szID[32];
	char szData[256];

	ifstream fLocal;

	char szCurrentPath[ MAX_PATH ];
	GetCurrentDirectory( MAX_PATH, szCurrentPath );

	std::string strLocalFile = szCurrentPath;
	strLocalFile.append("\\..\\Local\\" );

	switch( m_lt )
	{
	case LOCAL_KOR : strLocalFile.append( "Local_Kor.txt" );	break;
	case LOCAL_ENG : strLocalFile.append( "Local_Eng.txt" );	break;
	case LOCAL_CHN : strLocalFile.append( "Local_Chn.txt" );	break;
	}

	fLocal.open( strLocalFile.c_str() );
	

	while( fLocal.getline( szBuffer, 256) )
	{
		if( strcmp( szBuffer, "" ) == 0 )
			continue;

		if( strncmp( szBuffer, "//", 2 ) == 0 )
			continue;

		sscanf( szBuffer, "%s	%s", szID, szData );

		CString strTmp = szBuffer;
		CString strData = strTmp.Mid( 9 );

		m_mapLocal[szID] = (LPCSTR)strData;
	}

	fLocal.close();
}

LPCSTR	ALocalization::Get( std::string strID )
{
	std::map<std::string,std::string>::iterator it;
	it = m_mapLocal.find( strID );

	if( it == m_mapLocal.end() )
		return "";

	return it->second.c_str();
}