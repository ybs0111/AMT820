// MyInternetSession.h: interface for the CMyInternetSession class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MYINTERNETSESSION_H__254FC5C5_3B0E_11D6_AB38_00D0B70C3D79__INCLUDED_)
#define AFX_MYINTERNETSESSION_H__254FC5C5_3B0E_11D6_AB38_00D0B70C3D79__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "xSkinButton.h"
#include "xpbutton.h"		// XP 칼라 버튼 클래스
#include "ssdllmfc.h"
#include "xShadeButton.h"
#include "XPGroupBox.h"		// 칼라 그룹 박스 생성 클래스 추가 
#include "sxlogfont.h"
#include "BtnST.h"          // 칼라 버튼 생성 클래스 추가  
#include "NewLabel.h"		// 칼라 텍스트 박스 생성 클래스 추가 
#include "EditEx.h"			// 칼라 에디트 박스 생성 클래스 추가 
#include "EXDigitST.h"		// 디지털 카운터 클래스
#include "LedButton.h"      // LED 버튼 클래스
#include "XPGroupBox.h"		// 칼라 그룹 박스 생성 클래스 추가 
#include "MacButtons.h"
#include "GradientStatic.h" // 그라데이션 칼라 텍스트 박스 생성 클래스 추가 
#include "XpButton.h"		// XP 칼라 버튼 클래스

class CMyInternetSession : public CInternetSession
{
public:
	CMyInternetSession(LPCTSTR pstrAgent = NULL, DWORD dwContext = 1, DWORD dwAccessType = INTERNET_OPEN_TYPE_PRECONFIG, LPCTSTR pstrProxyName = NULL, LPCTSTR pstrProxyBypass = NULL, DWORD dwFlags = 0 ) 
		: CInternetSession(pstrAgent, dwContext, dwAccessType, pstrProxyName, pstrProxyBypass, dwFlags) 
		{ 	
//			m_pMainWnd = NULL;	
		};

// Attributes
public:
//	HWND m_pMainWnd;  // pointer to parent window
// Operations
public:
	CString GetErrorString(DWORD dwErrorCode);

	virtual void OnStatusCallback(DWORD dwContext, DWORD dwInternetStatus, LPVOID lpvStatusInfomration, DWORD dwStatusInformationLen);
};



#endif // !defined(AFX_MYINTERNETSESSION_H__254FC5C5_3B0E_11D6_AB38_00D0B70C3D79__INCLUDED_)
