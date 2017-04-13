///////////////////////////////////////////////////////////////////////////////
/* ****************************************************************************** */
// FILE NAME : CCharSetCtrl.cpp
//
// Copyright (C) 2003 AMT Corporation
// All rights reserved.
//
// CREATED BY  : Lee Kyu Il
// REVISION    :
//     1> 2003. 12. 16 : Created by Lee Kyu Il
// DESCRIPTION :
//     Implementation of the CCharSetCtrl and associated classes
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
#include "charsetctrl.h"
#include "Resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define PERFORM_MOUSEMOVE

/////////////////////////////////////////////////////////////////////////////
// CCharSetCtrl

CCharSetCtrl::CCharSetCtrl()
{
	int i, j, k;

    RegisterWindowClass();  /* 클래스 레지스터에 등록하는 함수 */

	/* ************************************************************************** */
	/* 셀 그리는데 필요한 각종 데이터 초기화한다                                  */
	/* -> GetSysColor() : 현재 디스플레이 장치에 대한 칼라 정보 얻는다            */
	/*  : 이 정보는 얻어진 윈도우 클래스와 비슷한 클래스를 등록할 목적으로 사용   */
	/* -> LoadStandardCursor() : 기본 커서를 로딩한다                             */
	/* -> AfxRegisterClass() : 주어진 클래스를 레지스터에 등록한다                */
	/* -> AfxThrowResourceException() : 리소스 로딩 실패 시 에러 처리한다         */
	/* ************************************************************************** */
	m_nRows=1;  /* 초기 셀 ROW 갯수 */
	m_nCols=16;  /* 초기 셀 COL 갯수 */

	m_crLine=::GetSysColor(COLOR_GRAYTEXT);      /* 셀 라인 칼라 */

	m_crText=::GetSysColor(COLOR_WINDOWTEXT);    /* 셀에 출력할 문자 칼라 */
	m_crBkg=::GetSysColor(COLOR_WINDOW);         /* 셀 배경 칼라 */
	
	m_crHiliteText=RGB(0,0,255);                 /* 하이라이트 셀에 출력할 문자 칼라 */
	m_crHiliteBkg=::GetSysColor(COLOR_BTNFACE);  /* 하이라이트 셀 배경 칼라 */

	m_nHiliteCol=-1;  /* 하이라이트 시킬 셀 COL 위치 */
	m_nHiliteRow=-1;  /* 하이라이트 시킬 셀 ROW 위치 */

	m_strFaceName="Arial";  /* 문자 출력할 폰트 이름 */
	m_bBold=FALSE;          /* 문자 굵기 적용 플래그 */
	m_bItalic=FALSE;        /* 문자 이탤릭 적용 플래그 */

	m_nSelect=-1;  /* 선택된 셀 위치 저장 변수 */
	m_nStart=32;

	for(i=0; i<m_nRows; i++)
	{
		for(j=0; j<m_nCols; j++)
		{
			k = i*m_nCols+j;
			mc_BcrColor[k] = RGB(0,0,255);
			mc_TcrColor[k] = ::GetSysColor(COLOR_WINDOWTEXT);

			mstr_Out[k].Empty();
		}
	}
	/* ************************************************************************** */
}

CCharSetCtrl::~CCharSetCtrl()
{
}

/* ****************************************************************************** */
/* 클래스 레지스터에 등록하는 함수                                                */
/* -> 해당 클래스 레지스터에 등록되지 않은 경우 클래스 등록시킨다                 */
/* ****************************************************************************** */
BOOL CCharSetCtrl::RegisterWindowClass()
{
	/* ************************************************************************** */
	/* AfxGetInstanceHandle() : 현재 애플리케이션에 대한 인스턴스 핸들 리턴 함수  */
	/* ************************************************************************** */
    WNDCLASS wndcls;
    HINSTANCE hInst = AfxGetInstanceHandle();
	/* ************************************************************************** */

	/* ************************************************************************** */
	/* 레지스터에 새로운 클래스를 등록한다                                        */
	/* -> GetClassInfo() : 주어진 윈도우 클래스에 대한 WNDCLASS 정보를 얻는다     */
	/*  : 이 정보는 얻어진 윈도우 클래스와 비슷한 클래스를 등록할 목적으로 사용   */
	/* -> LoadStandardCursor() : 기본 커서를 로딩한다                             */
	/* -> AfxRegisterClass() : 주어진 클래스를 레지스터에 등록한다                */
	/* -> AfxThrowResourceException() : 리소스 로딩 실패 시 에러 처리한다         */
	/* ************************************************************************** */
    if (!(::GetClassInfo(hInst, CHARSETCTRL_CLASSNAME, &wndcls)))
    {
        wndcls.style            = CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW;
        wndcls.lpfnWndProc      = ::DefWindowProc;
        wndcls.cbClsExtra       = wndcls.cbWndExtra = 0;
        wndcls.hInstance        = hInst;
        wndcls.hIcon            = NULL;
        wndcls.hCursor          = AfxGetApp()->LoadStandardCursor(IDC_ARROW);
        wndcls.hbrBackground    = (HBRUSH) (COLOR_3DFACE + 1);
        wndcls.lpszMenuName     = NULL;
        wndcls.lpszClassName    = CHARSETCTRL_CLASSNAME;

        if (!AfxRegisterClass(&wndcls))
        {
            AfxThrowResourceException();
            return FALSE;
        }
    }
	/* ************************************************************************** */

    return TRUE;
}
/* ****************************************************************************** */

BEGIN_MESSAGE_MAP(CCharSetCtrl, CWnd)
	//{{AFX_MSG_MAP(CCharSetCtrl)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_KEYDOWN()
	ON_WM_LBUTTONDBLCLK()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCharSetCtrl message handlers

void CCharSetCtrl::OnPaint() 
{
	CPaintDC dc(this); // device context for painting

	int i, j, k;

	CPen linePen(PS_SOLID, 1, m_crLine);
	CPen textPen(PS_SOLID, 1, m_crText);
	CFont ctrlFont;

	/* ************************************************************************** */
	/* GetClientRect() : 주어진 윈도우의 클라이언트 영역 사각형을 얻는 함수       */
	/* -> 좌표는 윈도우의 클라이언트 여역으로부터 계산되는 상대 좌표이다          */
	/*  : 따라서, 좌측 상단의 좌표는 (0,0)이다                                    */
	/* -> GetClientRect() 특정 윈도우의 클라이언트 영역의 크기 알고 싶을 때       */
	/*    유용하게 사용할 수 있다                                                 */
	/* FillSolidRect() : 주어진 사각형 영역을 주어진 칼라로 채운다                */
	/* ************************************************************************** */
	GetClientRect(m_Rect);

	dc.FillSolidRect(m_Rect, GetSysColor(COLOR_BTNFACE));
	/* ************************************************************************** */
	
	/* ************************************************************************** */
	/* -> SelectObject() : 장치 컨텍스트에 현재 오브젝트(폰트) 설정               */
	/* ************************************************************************** */
	CPen* pOldPen=dc.SelectObject(&linePen);
	/* ************************************************************************** */
	
	/* ************************************************************************** */
	/* 셀을 그린다                                                                */
	/* -> MoveTo() : 지정된 장치 컨텍스트와 연관된 현재의 위치를 움직인다         */
	/*  : 현재의 위치는 LineTo()와 같은 다양한 그리기 함수에 사용된다             */
	/* -> LineTo() : 지정된 점에서 혅의 위치로 선을 그린다                        */
	/*  : 선은 현재 설정한 펜으로 그리게 된다                                     */
	/* ************************************************************************** */
	m_nRangeCol=m_Rect.Width();
	m_nStepCol=m_nRangeCol/(m_nCols);
	m_nRangeRow=m_Rect.Height();
	m_nStepRow=m_nRangeRow/(m_nRows);
	
	m_Rect.right=m_Rect.left+m_nCols*m_nStepCol;
	m_Rect.bottom=m_Rect.top+m_nRows*m_nStepRow;

	for(i=0; i<=m_nCols; i++)
	{
		dc.MoveTo(m_Rect.left + i*m_nStepCol, m_Rect.top);
		dc.LineTo(m_Rect.left + i*m_nStepCol, m_Rect.Height());
	}

	for(j=0; j<=m_nRows; j++)
	{
		dc.MoveTo(m_Rect.left, m_Rect.top + j*m_nStepRow);
		dc.LineTo(m_Rect.Width()+1, m_Rect.top + j*m_nStepRow);
	}
	/* ************************************************************************** */

	/* ************************************************************************** */
	/* 셀에 사용할 폰트를 생성한다                                                */
	/* -> CreateFont() : 주어진 특성대로 논리적 글꼴을 생성한다                   */
	/*  : 응용 프로그램은 이 글꼴을 선택하여 어떤 장치에도 사용할 수 있다         */
	/*  : 일반적으로 응용 프로그램이 오직 하나의 글꼴만을 가지고 있을 경우        */
	/*    CreateFont() 함수를 사용한다                                            */
	/*  : 이 함수는 글꼴 데이터가 LOGFONT 구조체를 지원하므로 사용하기 쉽다       */
	/* -> SetTextColor() : 지정된 색으로 장치 컨텍스트의 텍스트 색을 설정한다     */
	/*  : 기본값 텍스트 색은 검정이다                                             */
	/*  : 이 함수는 텍스트 출력에 어떠한 RGB 색도 설정할 수 있다                  */
	/*  : 색은 장치 컨텍스트가 해제되거나 다른 테스트 색이 설정될 때까지 유효한다 */
	/* -> SetBkColor() : 배경색을 설정한다                                        */
	/*  : 배경색은 텍스트를 그릴 때 비트맵의 컬러를 단색으로 변환할 때 이용되고   */
	/*    펜들에 의한 공백이 있는 색들에 이용된다                                 */
	/* -> DeleteObject() : GDI 객체를 지운다                                      */
	/*  : GetStockObject() 함수로 생성된 객체는 제외된다                          */
	/*  : GDI 객체를 생성하는 함수를 사용한 경우 반드시 생성한 객체를 지워야 한다 */
	/*  : 지워야 하는 객체로는 펜, 비트맵, 브러시, 영역, 팔레트, 폰트 등이다      */
	/*  : 현재 장치 컨텍스트에서 선택된 객체에 대해서는 DeleteObject() 함수를     */
	/*    사용하면 안된다                                                         */
	/*  : 현재 장치 컨텍스트의 선택된 객체를 지울 때 좀비 객체가 생성되기 때문    */
	/*  : 윈도우 NT의 경우 현재 장치 컨텍스트에서 선택된 객체를 이 함수가         */
	/*    호출하는 것을 허용하지 않는다                                           */
	/*  : 만일 지우려는 객체가 브러시일 경우 브러시를 만들기 위해 사용했던        */
	/*    비트맵은 이 함수로 지울 수가 없다                                       */
	/*  : 그럴 경우 직접 그 객체를 지워야 한다                                    */
	/* ************************************************************************** */
	dc.SelectObject(pOldPen);
	
	ctrlFont.CreateFont
	(
		min(m_nStepCol-10, m_nStepRow-10),
		0,
		0,
		0,
		m_bBold ? FW_BOLD : FW_NORMAL,
		m_bItalic,
		0,
		0,
		DEFAULT_CHARSET,
		OUT_CHARACTER_PRECIS,
		CLIP_CHARACTER_PRECIS,
		DEFAULT_QUALITY,
		DEFAULT_PITCH | FF_DONTCARE,
		m_strFaceName
	);

	CFont* pOldFont=dc.SelectObject(&ctrlFont);
	dc.SetTextColor(m_crText);
	dc.SetBkColor(m_crBkg);
	
	for(i=0; i<m_nRows; i++)
	{
		for(j=0; j<m_nCols; j++)
		{
			k = i*m_nCols+j;
			DrawCell(&dc, i, j, mc_BcrColor[k], mc_TcrColor[k], mstr_Out[k]);  /* 셀을 생성하는 함수 */
		}
	}

	dc.SelectObject(pOldFont);
	ctrlFont.DeleteObject();
	linePen.DeleteObject();
	textPen.DeleteObject();
	/* ************************************************************************** */
}

BOOL CCharSetCtrl::OnEraseBkgnd(CDC* pDC) 
{
	return CWnd::OnEraseBkgnd(pDC);
}

void CCharSetCtrl::PreSubclassWindow() 
{
	// TODO: Add your specialized code here and/or call the base class
	// In our case this is not needed - yet - so just drop through to
    // the base class
	CWnd::PreSubclassWindow();
}

/////////////////////////////////////////////////////////////////////////////
// CCharSetCtrl methods

BOOL CCharSetCtrl::Create(CWnd* pParentWnd, const RECT& rect, UINT nID, DWORD dwStyle /*=WS_VISIBLE*/)
{
	return CWnd::Create(CHARSETCTRL_CLASSNAME, _T(""), dwStyle, rect, pParentWnd, nID);
}

void CCharSetCtrl::OnMouseMove(UINT nFlags, CPoint point) 
{
	CWnd::OnMouseMove(nFlags, point);
}

void CCharSetCtrl::OnLButtonDown(UINT nFlags, CPoint point) 
{
	CWnd::OnLButtonDown(nFlags, point);
}

void CCharSetCtrl::OnLButtonUp(UINT nFlags, CPoint point) 
{
	CWnd::OnLButtonUp(nFlags, point);
}

void CCharSetCtrl::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	CWnd::OnKeyDown(nChar, nRepCnt, nFlags);
}

BOOL CCharSetCtrl::PreTranslateMessage(MSG* pMsg) 
{
	return CWnd::PreTranslateMessage(pMsg);
}

void CCharSetCtrl::DrawCell(CDC* pDC, int nRow, int nCol, const COLORREF crBkColor, const COLORREF crTextColor, CString crText)
{
	CString strOut;
	strOut="0";

	strOut =crText;

	CRect rcHilite(m_Rect.left + nCol*m_nStepCol+1, 
				m_Rect.top + nRow*m_nStepRow+1, 
				m_Rect.left + (nCol+1)*m_nStepCol, 
				m_Rect.top + (nRow+1)*m_nStepRow);

	CRect rcText(m_Rect.left + nCol*m_nStepCol+2, 
				m_Rect.top + nRow*m_nStepRow+2, 
				m_Rect.left + (nCol+1)*m_nStepCol-1, 
				m_Rect.top + (nRow+1)*m_nStepRow-1);
	
	/* ************************************************************************** */
	/* 주어진 칼라를 사용하여 셀을 생성한다                                       */
	/* -> SetBkMode() : 현재의 배경색을 설정한다                                  */
	/*  : 배경 모드는 윈도우가 텍스트와 선들 내의 공백을 그리고 색의 비트맵을     */
	/*    단색으로 변환하는 방식을 지정한다                                       */
	/* -> FillSolidRect() : 지정된 사각형을 주어진 브러시로 채운다                */
	/* -> DrawText() : 원하는 출력 형식으로 텍스트를 그린다                       */
	/* ************************************************************************** */
	pDC->SetBkMode(TRANSPARENT);
	if (nCol==m_nHiliteCol && nRow==m_nHiliteRow)
	{
		/*
		pDC->SetTextColor(m_crHiliteText);
		pDC->SetBkColor(m_crHiliteBkg);

		pDC->FillSolidRect(rcHilite, m_crHiliteBkg);
		pDC->DrawText(strOut, rcText, DT_CENTER | DT_VCENTER);
		*/
		pDC->SetTextColor(crTextColor);
		pDC->SetBkColor(crBkColor);

		pDC->FillSolidRect(rcHilite, crBkColor);
		pDC->DrawText(strOut, rcText, DT_CENTER | DT_VCENTER);
	}
	else
	{
		/*
		pDC->SetTextColor(m_crText);
		pDC->SetBkColor(m_crBkg);

		pDC->FillSolidRect(rcHilite, m_crBkg);
		pDC->DrawText(strOut, rcText, DT_CENTER | DT_VCENTER);
		*/
		pDC->SetTextColor(crTextColor);
		pDC->SetBkColor(crBkColor);

		pDC->FillSolidRect(rcHilite, crBkColor);
		pDC->DrawText(strOut, rcText, DT_CENTER | DT_VCENTER);
	}
	/* ************************************************************************** */
}

void CCharSetCtrl::DrawCell(CDC* pDC, int nChar)
{
	int nCol=(nChar-m_nStart)%m_nCols;
	int nRow=(int)((nChar-m_nStart)/m_nCols);

	int k = nRow*m_nCols+nCol;
	DrawCell(pDC, nRow, nCol, mc_BcrColor[k], mc_TcrColor[k], mstr_Out[k]);  /* 셀을 그리는 함수 */
}

void CCharSetCtrl::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	CWnd::OnLButtonDblClk(nFlags, point);
}

/* ****************************************************************************** */
/* 선택된 셀 위치 설정 함수                                                       */
/* ****************************************************************************** */
void CCharSetCtrl::SetSelection(int nRow, int nCol)
{
	int nChar=m_nStart+m_nCols*nRow+nCol;
	SetSelection(nChar);  /* 선택된 셀 그리는 함수 */
}
/* ****************************************************************************** */

/* ****************************************************************************** */
/* 선택된 셀 그리는 함수                                                          */
/* ****************************************************************************** */
void CCharSetCtrl::SetSelection(int nSelect)
{
	int nOldSelect=m_nSelect;
	m_nSelect=nSelect;

	CClientDC dc (this);
	CFont ctrlFont;
	
	ctrlFont.CreateFont
	(
		min(m_nStepCol, m_nStepRow),
		0,
		0,
		0,
		m_bBold ? FW_BOLD : FW_NORMAL,
		m_bItalic,
		0,
		0,
		DEFAULT_CHARSET,
		OUT_CHARACTER_PRECIS,
		CLIP_CHARACTER_PRECIS,
		DEFAULT_QUALITY,
		DEFAULT_PITCH | FF_DONTCARE,
		m_strFaceName
	);

	CFont* pOldFont=dc.SelectObject(&ctrlFont);
	
	if (nOldSelect!=-1)
		DrawCell(&dc, nOldSelect);
	if (m_nSelect!=-1)
		DrawCell(&dc, m_nSelect);

	dc.SelectObject(pOldFont);
	ctrlFont.DeleteObject();
}
/* ****************************************************************************** */

/* ****************************************************************************** */
/* 특정 블록에 대한 칼라 설정 함수                                                */
/* ****************************************************************************** */
void CCharSetCtrl::SetBlock(UINT nBlockNumber, COLORREF BcrColor, COLORREF TcrColor, CString strText)
{
	mc_BcrColor[nBlockNumber] = BcrColor;
	mc_TcrColor[nBlockNumber] = TcrColor;

	mstr_Out[nBlockNumber] = strText;
}
/* ****************************************************************************** */

/* ****************************************************************************** */
/* 클라이언트 영역 갱신하는 함수                                                  */
/* ****************************************************************************** */
void CCharSetCtrl::Refresh()
{
	Invalidate();  /* 윈도우 영역 갱신 함수 */
}
/* ****************************************************************************** */

void CCharSetCtrl::SetRows(int nRows)
{
	m_nRows=nRows;
	Invalidate();
}
void CCharSetCtrl::SetCols(int nCols)
{
	m_nCols=nCols;
	Invalidate();
}

void CCharSetCtrl::SetColorLine(COLORREF crLine)
{
	m_crLine=crLine;
	Invalidate();
}

void CCharSetCtrl::SetColorBkg(COLORREF crBkg)
{
	m_crBkg=crBkg;
	Invalidate();
}

void CCharSetCtrl::SetColorText(COLORREF crText)
{
	m_crText=crText;
	Invalidate();
}

void CCharSetCtrl::SetColorHiliteBkg(COLORREF crHiliteBkg)
{
	m_crHiliteBkg=crHiliteBkg;
	Invalidate();
}

void CCharSetCtrl::SetColorHiliteText(COLORREF crHiliteText)
{
	m_crHiliteText=crHiliteText;
	Invalidate();
}

void CCharSetCtrl::SetFaceName(CString strFaceName)
{
	m_strFaceName=strFaceName;
	Invalidate();
}

void CCharSetCtrl::SetBold(BOOL bBold)
{
	m_bBold=bBold;
	Invalidate();
}

void CCharSetCtrl::SetItalic(BOOL bItalic)
{
	m_bItalic=bItalic;
	Invalidate();
}

void CCharSetCtrl::SetStart(int nStart)
{
	m_nStart=nStart;
	Invalidate();
}

int CCharSetCtrl::GetRows()
{
	return m_nRows;
}

int CCharSetCtrl::GetCols()
{
	return m_nCols;
}

COLORREF CCharSetCtrl::GetColorLine()
{
	return m_crLine;
}

COLORREF CCharSetCtrl::GetColorBkg()
{
	return m_crBkg;
}

COLORREF CCharSetCtrl::GetColorText()
{
	return m_crText;
}

COLORREF CCharSetCtrl::GetColorHiliteBkg()
{
	return m_crHiliteBkg;
}

COLORREF CCharSetCtrl::GetColorHiliteText()
{
	return m_crHiliteText;
}

CString CCharSetCtrl::GetFaceName()
{
	return m_strFaceName;
}

BOOL CCharSetCtrl::GetBold()
{
	return m_bBold;
}

BOOL CCharSetCtrl::GetItalic()
{
	return m_bItalic;
}

int CCharSetCtrl::GetSelection()
{
	return m_nSelect;
}

int CCharSetCtrl::GetStart()
{
	return m_nStart;
}

void CCharSetCtrl::SetText(UINT nBlockNumber, CString strText)
{
	mstr_Out[nBlockNumber] = strText;
	Invalidate();
}

void CCharSetCtrl::SetTextColor(UINT nBlockNumber, COLORREF TcrColor)
{
	mc_TcrColor[nBlockNumber] = TcrColor;
	Invalidate();
}
