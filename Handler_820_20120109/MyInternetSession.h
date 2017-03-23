// MyInternetSession.h: interface for the CMyInternetSession class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MYINTERNETSESSION_H__254FC5C5_3B0E_11D6_AB38_00D0B70C3D79__INCLUDED_)
#define AFX_MYINTERNETSESSION_H__254FC5C5_3B0E_11D6_AB38_00D0B70C3D79__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "xSkinButton.h"
#include "xpbutton.h"		// XP Į�� ��ư Ŭ����
#include "ssdllmfc.h"
#include "xShadeButton.h"
#include "XPGroupBox.h"		// Į�� �׷� �ڽ� ���� Ŭ���� �߰� 
#include "sxlogfont.h"
#include "BtnST.h"          // Į�� ��ư ���� Ŭ���� �߰�  
#include "NewLabel.h"		// Į�� �ؽ�Ʈ �ڽ� ���� Ŭ���� �߰� 
#include "EditEx.h"			// Į�� ����Ʈ �ڽ� ���� Ŭ���� �߰� 
#include "EXDigitST.h"		// ������ ī���� Ŭ����
#include "LedButton.h"      // LED ��ư Ŭ����
#include "XPGroupBox.h"		// Į�� �׷� �ڽ� ���� Ŭ���� �߰� 
#include "MacButtons.h"
#include "GradientStatic.h" // �׶��̼� Į�� �ؽ�Ʈ �ڽ� ���� Ŭ���� �߰� 
#include "XpButton.h"		// XP Į�� ��ư Ŭ����

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
