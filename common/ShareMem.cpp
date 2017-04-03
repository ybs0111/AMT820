// ShareMem.cpp: implementation of the ShareMem class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
//#include "VirtualAPCT.h"
#include "ShareMem.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

ShareMem g_sm;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

ShareMem::ShareMem()
{
	Initialize();
}

ShareMem::~ShareMem()
{
	UnInitialize();
}

void ShareMem::Initialize()
{
	UnInitialize();

	for( int smt = 0; smt < SMT_COUNT; smt++ )
	{
		char szNUM[32];
		itoa( smt, szNUM, 10 );

		st_shareMem st;
		st.smt = (SM_TYPE)smt;
		
		std::string strSMT = "SMT_";
		strSMT.append(szNUM);

		st.hFMap = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, 1024, strSMT.c_str() );
		st.pInFile = (char *)MapViewOfFile( st.hFMap, FILE_MAP_ALL_ACCESS, 0, 0, 1024 );

		m_vecSM.push_back( st );
	}
}

void ShareMem::UnInitialize()
{
	int iSize = m_vecSM.size();
	for( int i = 0; i < iSize; i++ )
	{
		UnmapViewOfFile( m_vecSM.at(i).pInFile );
		CloseHandle( m_vecSM.at(i).hFMap );

	}

	m_vecSM.clear();
}

void ShareMem::SetData(SM_TYPE smt, std::string strData )
{
	int iSize = m_vecSM.size();
	for( int i=0; i<iSize; i++ )
	{
		if( smt == m_vecSM.at(i).smt )
		{
			if( strData != m_vecSM.at(i).pInFile )
				sprintf( m_vecSM.at(i).pInFile, "%s", strData.c_str() );
		}
	}
}

std::string ShareMem::GetData( SM_TYPE smt )
{
	std::string strRtn = "";

	int iSize = m_vecSM.size();
	for( int i=0; i<iSize; i++ )
	{
		if( smt == m_vecSM.at(i).smt )
		{
			strRtn = m_vecSM.at(i).pInFile;
		}
	}

	if( strRtn == "" )
		strRtn = "0";

	return strRtn;
}
