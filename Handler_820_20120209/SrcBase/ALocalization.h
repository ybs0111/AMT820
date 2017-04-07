// ALocalization.h: interface for the ALocalization class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ALOCALIZATION_H__7B3E45A3_0BF2_4AD1_B43E_DA3F515E3B3F__INCLUDED_)
#define AFX_ALOCALIZATION_H__7B3E45A3_0BF2_4AD1_B43E_DA3F515E3B3F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
//#pragma warning( disable : 4786 )
#endif // _MSC_VER > 1000

#include <string>
#include <map>
#include <vector>


enum LOCAL_TYPE	{ LOCAL_KOR, LOCAL_CHN, LOCAL_ENG, LOCAL_MAX };
class ALocalization  
{
public:
	ALocalization();
	virtual ~ALocalization();

	void SetLocal( LOCAL_TYPE lt );
	void LoadLocalFile();
	LPCSTR	Get( std::string strID );

	LOCAL_TYPE GetLocalType() { return m_lt; }
	void SetNextLocal();

protected:
	LOCAL_TYPE	m_lt;

	std::map<std::string, std::string>	m_mapLocal;
};

extern ALocalization g_local;

#endif // !defined(AFX_ALOCALIZATION_H__7B3E45A3_0BF2_4AD1_B43E_DA3F515E3B3F__INCLUDED_)
