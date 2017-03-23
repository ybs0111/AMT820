///////////////////////////////////////////////////////////////////////////////
/* ****************************************************************************** */
// FILE NAME : CEXDigitST.cpp
//
// Copyright (C) 2003 AMT Corporation
// All rights reserved.
//
// CREATED BY  : Lee Kyu Il
// REVISION    :
//     1> 2003. 12. 17 : Created by Lee Kyu Il
// DESCRIPTION :
//     Implementation of the CEXDigitST and associated classes
//	   This code may be used in compiled form in any way you desire. This
//	   file may be redistributed unmodified by any means PROVIDING it is
//	   not sold for profit without the authors written consent, and
//	   providing that this notice and the authors name and all copyright
//	   notices remains intact.
//
//	   An email letting me know how you are using it would be nice as well.
//
//
// REFERENCE   :
// 
/* ****************************************************************************** */
///////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "EXDigitST.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEXDigitST

CEXDigitST::CEXDigitST()
{
	m_nPrecision = 2;    /* 출력할 데이터 길이 초기화 */

	m_nValue = 0;        /* 출력 숫자형 데이터 초기화 */
	m_strValue.Empty();  /* 출력 문자열 데이터 초기화 */

	m_b16Flag = FALSE ;  /* 출력 데이터 종류 초기화 [FALSE:숫자형    TRUE:문자형] */

	m_bZeroPadding = FALSE;  /* 선두 공백 0으로 채울지에 대한 플래그 */

	m_dwResize = ST_RIGHT | ST_BOTTOM;  /* 컨트롤 RESIZE 옵션 [RIGHT-BOTTOM] */

	m_nWidth = 0;        /* 출력 비트맵 폭 초기화 */
	m_nHeight = 0;       /* 출력 비트맵 높이 초기화 */

	m_pbmDigit = NULL;   /* 비트맵 로딩 유무 플래그 초기화 */

	m_nDisplayMode = 0; // double 표시 초기화
}


CEXDigitST::~CEXDigitST()
{
	/* ************************************************************************** */
	/* 비트맵을 로딩한 경우 해당 리소스 해제한다                                  */
	/* -> 이 객체는 new 연산자를 사용하여 동적으로 생성하였기 때문에 반드시       */
	/*    delete해야 한다                                                         */
	/* ************************************************************************** */
	if (m_pbmDigit != NULL)
		delete m_pbmDigit;

	m_pbmDigit = NULL;
	/* ************************************************************************** */
}


BEGIN_MESSAGE_MAP(CEXDigitST, CStatic)
	//{{AFX_MSG_MAP(CEXDigitST)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/* ****************************************************************************** */
/* 클래스 동적 생성 함수                                                          */
/* -> 해당 클래스 CHILD 형태로 생성한다                                           */
/* ****************************************************************************** */
BOOL CEXDigitST::Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID) 
{
	BOOL result ;

	/* ************************************************************************** */
	/* 윈도우 레지스터에 등록 후 생성한다                                         */
	/* -> AfxRegisterWndClass() : 클래스 이름을 레지스터에 등록하는 함수          */
	/*  : 이 함수는 Create() 멤버 함수에서 사용한다                               */
	/* -> CreateEx() : 윈도우 생성 함수                                           */
	/* ************************************************************************** */
	static CString className = AfxRegisterWndClass(CS_HREDRAW | CS_VREDRAW) ;

	result = CWnd::CreateEx(/*WS_EX_CLIENTEDGE | WS_EX_STATICEDGE*/NULL, 
                          className, NULL, dwStyle, 
                          rect.left, rect.top, rect.right-rect.left, rect.bottom-rect.top,
                          pParentWnd->GetSafeHwnd(), (HMENU)nID) ;
	/* ************************************************************************** */

	if (result != 0)
		Invalidate() ;  /* 윈도우 영역 갱신 함수 */

	return result ;
} 
/* ****************************************************************************** */

/////////////////////////////////////////////////////////////////////////////
// CEXDigitST message handlers


void CEXDigitST::OnPaint() 
{
	/* ************************************************************************** */
	/* BeginPaint() : 장치 컨텍스트를 할당하고 갱신된 영역에 관한 정보를          */
	/*                반환하는 것으로 윈도우 페인팅 프로세스를 시작한다           */
	/* -> 이 함수는 WM_PAINT 메시지 응답에 따라 쓰여진다                          */
	/* ************************************************************************** */
	PAINTSTRUCT lpPaint;
	BeginPaint(&lpPaint);
	/* ************************************************************************** */

	CWindowDC dc(this);

	/* ************************************************************************** */
	/* 비트맵이 로딩 완료된 경우 해당 비트맵을 사용하여 디지털 카운터 출력한다    */
	/* ************************************************************************** */
	if (m_pbmDigit->m_hObject)
		DrawDigits(&dc);  /* 디지털 카운터 출력 함수 */
	/* ************************************************************************** */

	/* ************************************************************************** */
	/* EndPaint() : 페인팅 작업의 종료를 나타낸다                                 */
	/* -> 모든 페인팅 작업이 종료되면 BeginPaint() 후에 꼭 써주어야 한다          */
	/* -> BeginPaint() 함수에 의해 카렛이나 커서가 숨겨졌다면 EndPaint() 함수는   */
	/*    그것을 복구한다                                                         */
	/* ************************************************************************** */
	EndPaint(&lpPaint);
	/* ************************************************************************** */
} 


BOOL CEXDigitST::SetStyle(UINT nBitmapId, int nPrecision)
{
	BITMAP bm;
	BOOL bRet;

	/* ************************************************************************** */
	/* 비트맵을 로딩한 경우 해당 리소스 해제한다                                  */
	/* -> 이 객체는 new 연산자를 사용하여 동적으로 생성하였기 때문에 반드시       */
	/*    delete해야 한다                                                         */
	/* ************************************************************************** */
	if (m_pbmDigit != NULL)
	{
		delete m_pbmDigit;

		m_pbmDigit = NULL;
	}
	/* ************************************************************************** */

	m_pbmDigit = new CBitmap;  /* 새로운 비트맵 생성 */

	/* ************************************************************************** */
	/* 새로운 비트맵 로딩한다                                                     */
	/* -> LoadBitmap() : 모듈의 리소스 파일로부터 비트맵 리소스를 읽어 들인다     */
	/*  : 응용 프로그램은 BITMAP 문을 사용해서 리소스 스크립트 파일(.RC)에        */
	/*    비트맵을 열거해야 한다                                                  */
	/*  : 비트맵의 사용이 끝나면 응용 프로그램은 DeleteObject()를 호출해서        */
	/*    메모리에서 비트맵을 해제해야 한다                                       */
	/*  : 이 함수는 장치 의존적 비트맵(DDB)이나 장치 독립적 비트맵(DIB)를 읽는다  */
	/*  : DIB를 읽어 들이면 DDB로 변환하고 색상 정보는 읽어버린다                 */
	/* -> GetBitmap() : 호출된 비트맵의 크기를 구한다                             */
	/* ************************************************************************** */
	bRet = m_pbmDigit->LoadBitmap(nBitmapId);
	if (bRet == TRUE)
	{
		m_pbmDigit->GetBitmap(&bm);

		if (nBitmapId == 239)
		{
			m_nWidth = (int)bm.bmWidth / 14;  /* 한개 비트맵 폭을 계산한다 */
		}
		else
		{
			m_nWidth = (int)bm.bmWidth / 13;  /* 한개 비트맵 폭을 계산한다 */
		}
		m_nHeight = bm.bmHeight;          /* 비트맵 높이 설정 */

		SetPrecision(nPrecision);  /* 입력 범위 검사 함수 */
	}
	/* ************************************************************************** */

	return bRet;
}

void CEXDigitST::SetValue(int nValue, BOOL bRepaint)
{
	m_nValue = nValue;   /* 새로운 출력 값 설정 */

	m_b16Flag = FALSE ;  /* 숫자형 데이터 출력 플래그 설정 */

	m_nDisplayMode = 0;
	/* ************************************************************************** */
	/* 윈도우 영역을 다시 그린다                                                  */
	/* -> InvalidateRect() : 주어진 윈도우에 현재 갱신 영역을 지정된 직사각형에   */
	/*    덧붙이는 함수                                                           */
	/*  : 갱신 영역은 다시 그려져야 하는 윈도우의 부분을 정의한다                 */
	/* ************************************************************************** */
	if (bRepaint == TRUE)
		Invalidate(FALSE);
	/* ************************************************************************** */
}

void CEXDigitST::SetValue(double dValue, BOOL bRepaint)
{
//	m_nValue = (int)dValue;   /* 새로운 출력 값 설정 */
	m_dValue = dValue;

	m_b16Flag = FALSE ;  /* 숫자형 데이터 출력 플래그 설정 */

	m_nDisplayMode = 1;
	/* ************************************************************************** */
	/* 윈도우 영역을 다시 그린다                                                  */
	/* -> InvalidateRect() : 주어진 윈도우에 현재 갱신 영역을 지정된 직사각형에   */
	/*    덧붙이는 함수                                                           */
	/*  : 갱신 영역은 다시 그려져야 하는 윈도우의 부분을 정의한다                 */
	/* ************************************************************************** */
	if (bRepaint == TRUE)
		Invalidate(FALSE);
	/* ************************************************************************** */
}

void CEXDigitST::SetValue(CString strValue, BOOL bRepaint)
{
	if (strValue == "")		strValue = "0";
   	m_strValue = strValue;			// 새로운 출력 값 설정

	m_nValue = atoi(m_strValue);	// int로 변환.

	m_b16Flag = TRUE ;				// 문자형 데이터 출력 플래그 설정
	
	m_nDisplayMode = 2;

	if (bRepaint == TRUE)
		OnPaint();
}


double CEXDigitST::GetValue()
{
	if(m_nDisplayMode==0) return (double)m_nValue;
	else if(m_nDisplayMode==1) return m_dValue;

	return 0.0;
}


void CEXDigitST::SetPrecision(int nPrecision, BOOL bResize)
{
	/* ************************************************************************** */
	/* 입력된 값이 출력 범위내에 존재하는지 검사한다                              */
	/* -> 설정된 조건에 따라 비트맵을 클라이언트 윈도우에 정렬한다                */
	/* ************************************************************************** */
	if (nPrecision >= ST_MIN_PRECISION && nPrecision <= ST_MAX_PRECISION)
		m_nPrecision = nPrecision;
	/* ************************************************************************** */

	if (bResize == TRUE)
		Resize();  /* 비트맵 정렬 함수 */
}


int CEXDigitST::GetPrecision()
{
	return m_nPrecision;
}


void CEXDigitST::SetResize(DWORD dwResize, BOOL bResize)
{
	m_dwResize = dwResize;

	if (bResize == TRUE)
		Resize();  /* 비트맵 정렬 함수 */
}


DWORD CEXDigitST::GetResize()
{
	return m_dwResize;
}


void CEXDigitST::SetZeroPadding(BOOL bPad, BOOL bRepaint)
{
	m_bZeroPadding = bPad;  /* 패딩 스타일 설정 */

	if (bRepaint == TRUE)
		Invalidate(FALSE);  /* 윈도우 영역 갱신 함수 */
}


BOOL CEXDigitST::GetZeroPadding()
{
	return m_bZeroPadding;
}


void CEXDigitST::Inc(BOOL bRepaint)
{
	SetValue(GetValue() + 1, bRepaint);  /* 1 증가된 데이터 출력 함수 */
}


void CEXDigitST::Dec(BOOL bRepaint)
{
	SetValue(GetValue() - 1, bRepaint);  /* 1 감소된 데이터 출력 함수 */
}


const char* CEXDigitST::GetVersionC()
{
	return "1.1";  /* 버전 리턴 */
}


const short CEXDigitST::GetVersionI()
{
	return 10;
} 


void CEXDigitST::DrawDigits(CDC* pDC)
{
	char szValue[ST_MAX_PRECISION+1];

	int nLoop;
	int destX;
	CRect rectCtrl;
	int nAsciiChar;
	CBitmap* pOldBitmap;

	/* ************************************************************************** */
	/* 메모리 DC 생성한다                                                         */
	/* -> CreateCompatibleDC() : 주어진 장치를 위한 메모리 DC를 생성한다          */
	/*  : 메모리 DC를 그리기 작업을 위해 사용하기 전에 응용 프로그램은 반드시     */
	/*    적당한 폭과 높이의 비트맵을 선택하여 DC로 설정해야 한다                 */
	/*  : 비트맵이 선택된 후로 DC는 화면에 복사하거나 출력할 이미지 준비 가능     */
	/*  : DC가 더 이상 필요하지 않으면 DeleteDC() 함수를 사용하여 DC 제거해야 함  */
	/*  : 이 함수는 리스터(raster) 기능을 지원하는 장치에만 사용할 수 있다        */
	/*  : 이 기능을 지원하는지의 여부는 GetDeviceCaps() 함수를 사용하면 된다      */
	/* ************************************************************************** */
    CDC memDC;
	memDC.CreateCompatibleDC(pDC);
	/* ************************************************************************** */

	/* ************************************************************************** */
	/* 새로운 비트맵 설정한다                                                     */
	/* -> SelectObject() : 장치 컨텍스트에 현재 오브젝트(폰트) 설정한다           */
	/* -> GetClientRect() : 주어진 윈도우의 클라이언트 영역 사각형을 얻는 함수    */
	/*  : 좌표는 윈도우의 클라이언트 영역으로부터 계산되는 상태 좌표이다          */
	/*  : 좌측 상단의 좌표는 (0,0)이다                                            */
	/* ************************************************************************** */
	pOldBitmap = memDC.SelectObject(m_pbmDigit);
	
	GetClientRect(rectCtrl);
	/* ************************************************************************** */

	destX = ST_BORDER_SPACE;  /* 테두리로부터의 OFFSET 설정 */

	PrepareString(szValue);  /* 출력할 문자 생성 함수 */

	/* ************************************************************************** */
	/* 입력된 데이터에 해당하는 비트맵을 출력한다                                 */
	/* -> BitBlt() : 비트맵을 나타내는데 사용되는 표준 함수로 비트맵을            */
	/*    메모리 장치 컨텍스트에서 윈도우 장치 컨텍스트 같은 다른 장치 컨텍스트로 */
	/*    복사하는 역할을 한다                                                    */
	/*  : 이 함수를 사용하려면 먼저 비트맵을 메모리 장치 컨텍스트로 읽어 들인 후  */
	/*    CreateCompatibleDC() 함수로 비트맵을 생성해야 한다                      */
	/*  : 비트맵을 래스터 연산 코드를 사용해서 현재 배경과 조합할 수 있다         */
	/*  : 모든 장치가 이 함수를 지원하지는 않기 때문에 GetDeviceCaps() 함수로부터 */
	/*    반환되는 RASTERCAPS 값을 확인해서 지원 여부 확인해야 한다               */
	/* ************************************************************************** */
	for(nLoop=0; nLoop<m_nPrecision; nLoop++)
	{
		if (m_bZeroPadding == TRUE && m_nValue >= 0)
			nAsciiChar = 0;
		else
			nAsciiChar = 10;

		if (szValue[nLoop] >= '0' && szValue[nLoop] <= '9')  /* 0-9까지의 숫자인 경우 */
			nAsciiChar = szValue[nLoop] - 48;

		if (szValue[nLoop] == ':')  /* 소수점인 경우 */
			nAsciiChar = 13;

		if (szValue[nLoop] == '.')  /* 소수점인 경우 */
			nAsciiChar = 12;

		if (szValue[nLoop] == '-')  /* - 부호인 경우 */
			nAsciiChar = 11;

		pDC->BitBlt(destX, ST_BORDER_SPACE, m_nWidth, m_nHeight, &memDC, 0+(m_nWidth*nAsciiChar), 0, SRCCOPY);
		destX += m_nWidth;
	}
	/* ************************************************************************** */

	memDC.SelectObject(pOldBitmap);
}


void CEXDigitST::Resize()
{
	CRect rectCtrl;

	Invalidate();  /* 윈도우 영역 갱신 함수 */

	/* ************************************************************************** */
	/* 모든 비트맵을 출력할 영역을 재설정한다                                     */
	/* -> GetWindowRect() : 윈도우를 둘러싸는 사각형의 크기를 구해 lprc가         */
	/*    가리키는 RECT 구조체에 그 값을 저장한다                                 */
	/*  : 얻어진 좌표는 픽셀 단위이며 화면의 좌측 상단을 기준으로 계산한다        */
	/* -> GetParent() : 주어진 윈도우를 소유하고 있는 윈도우의 핸들을 얻는다      */
	/*  : 자식 윈도우가 메시지를 부모 윈도우로 보내 서로간에 통신하기 위해        */
	/*    어떤 자식 윈도우의 부모 윈도우 핸들을 얻는데 사용된다                   */
	/*  : 한 윈도우의 부모 윈도우에 대한 정보를 얻기 위해 사용될 수 있다          */
	/* -> ScreenToClient() : 화면 좌표를 클라이언트 좌표로 변환한다               */
	/*  : 마우스 커서의 위치를 윈도우 클라이언트 영역의 X/Y로 변환하기 위해       */
	/*    자주 GetCursorPos() 함수와 결합하여 사용된다                            */
	/*  : 처리되기 전에 움직일 수 있다                                            */
	/*  : 선을 그리거나 텍스트 위치시키는 동안 WM_MOUSEMOVE 메시지 처리될 때 사용 */
	/* ************************************************************************** */
	GetWindowRect(rectCtrl);
	GetParent()->ScreenToClient(rectCtrl);
	/* ************************************************************************** */

	/* ************************************************************************** */
	/* 입력된 데이터에 해당하는 비트맵을 출력한다                                 */
	/* -> MoveWindow() : 윈도우의 위치와 크기를 변경한다                          */
	/*  : 최상위 레벨 윈도우에 있어서 좌표체계는 화면의 좌측 상단이 기준이며,     */
	/*    자식 윈도우에 있어서는 부모 윈도우의 클라이언트 영역이 기준이다         */
	/* ************************************************************************** */
	if (m_dwResize & ST_RIGHT)   /* RIGHT로 재설정 */
		rectCtrl.right = rectCtrl.left + (m_nWidth * m_nPrecision)+(ST_BORDER_SPACE*2);
	else                         /* LEFT로 재설정 */
		rectCtrl.left = rectCtrl.right - (m_nWidth * m_nPrecision)-(ST_BORDER_SPACE*2);

	if (m_dwResize & ST_BOTTOM)  /* BOTTOM으로 재설정 */
		rectCtrl.bottom = rectCtrl.top + m_nHeight+(ST_BORDER_SPACE*2);
	else                         /* TOP으로 재설정 */
		rectCtrl.top = rectCtrl.bottom - m_nHeight-(ST_BORDER_SPACE*2);

	MoveWindow(rectCtrl);
	/* ************************************************************************** */
}


void CEXDigitST::PrepareString(char* szDest)
{
	char szStr[ST_MAX_PRECISION+1];	

	switch(m_nDisplayMode){
		case 0: // int
			sprintf(szStr, "%*d", m_nPrecision,m_nValue);
			sprintf(szDest, "%*s", m_nPrecision, &szStr[strlen(szStr)-m_nPrecision]);
			break;
		case 1: // double
			sprintf(szStr, "%*.3f", m_nPrecision,m_dValue);
			sprintf(szDest, "%*s", m_nPrecision, &szStr[strlen(szStr)-m_nPrecision]);
			break;
		case 2: // string
			sprintf(szStr, "%*s", m_nPrecision,(LPSTR)(LPCSTR)m_strValue);
			sprintf(szDest, "%*s", m_nPrecision, &szStr[strlen(szStr)-m_nPrecision]);
			break;
	}
/*	else if (m_b16Flag == FALSE){
		sprintf(szStr, "%*d", m_nPrecision,m_nValue);
		sprintf(szDest, "%*s", m_nPrecision, &szStr[strlen(szStr)-m_nPrecision]);
	}
	else{
		sprintf(szStr, "%*s", m_nPrecision,(LPSTR)(LPCSTR)m_strValue);
		sprintf(szDest, "%*s", m_nPrecision, &szStr[strlen(szStr)-m_nPrecision]);
	}*/

}

#undef ST_MIN_PRECISION
#undef ST_MAX_PRECISION
#undef ST_BORDER_SPACE