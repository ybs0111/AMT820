#if !defined(_CHARSETCTRL_H__INCLUDED_)
#define _CHARSETCTRL_H__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CCharSetCtrl window

#define CHARSETCTRL_CLASSNAME    _T("MFCCharSetCtrl")  /* 클래스 이름 설정 */

#define BTNST_MAX_COLORS  256	/* 버퍼 크기 */

class CCharSetCtrl : public CWnd
{
// Construction
public:
	CCharSetCtrl();

// Attributes
protected:
	int m_nRows;  /* 셀 ROW 갯수 저장 변수 */
	int m_nCols;  /* 셀 COL 갯수 저장 변수 */

	COLORREF m_crLine;        /* 셀 라인 칼라 저장 변수 */
	COLORREF m_crBkg;         /* 셀 배경 칼라 저장 변수 */
	COLORREF m_crText;        /* 텍스트 칼라 저장 변수 */
	COLORREF m_crHiliteBkg;   /* 하이라이트 배경 칼라 저장 변수 */
	COLORREF m_crHiliteText;  /* 하이라이트 출력 텍스트 칼라 저장 변수 */

	CRect m_Rect;
	int m_nRangeCol;
	int m_nStepCol;
	int m_nRangeRow;
	int m_nStepRow;

	int m_nHiliteCol;
	int m_nHiliteRow;
	int m_nSelect;
	int m_nStart;

	CString m_strFaceName;  /* 적용할 폰트명 저장 변수 */
	BOOL m_bBold;           /* 글자 굵게 적용 플래그 */
	BOOL m_bItalic;         /* 이탤릭 적용 플래그 */


// Attributes
public:
	COLORREF mc_TcrColor[BTNST_MAX_COLORS];  /* 텍스트 칼라 저장 변수 */
	COLORREF mc_BcrColor[BTNST_MAX_COLORS];	 /* 셀 배경 칼라 저장 변수 */

	CString mstr_Out[BTNST_MAX_COLORS];      /* 출력할 문자열 저장 변수 */


// Operations
public:
	BOOL Create(CWnd* pParentWnd, const RECT& rect, UINT nID, DWORD dwStyle = WS_VISIBLE);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCharSetCtrl)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

public:
	void SetBlock(UINT nBlockNumber, COLORREF BcrColor, COLORREF TcrColor, CString strText);  /* 특정 블록에 대한 칼라 정보 설정 함수 */
	void Refresh();  /* 클라이언트 영역 갱신하는 함수 */

	void SetRows(int nRows);  /* 셀 ROW 갯수 설정 함수 */
	void SetCols(int nCols);  /* 셀 COL 갯수 설정 함수 */

	void SetColorBkg(COLORREF crLine);   /* 셀 배경 칼라 설정 함수 */
	void SetColorLine(COLORREF crBkg);   /* 셀 테두리 라인 칼라 설정 함수 */
	void SetColorText(COLORREF crText);  /* 셀에 출력할 텍스트 칼라 설정 함수 */
	void SetColorHiliteBkg(COLORREF crHiliteBkg);    /* 하이라이트 셀 배경 칼라 설정 함수 */
	void SetColorHiliteText(COLORREF crHiliteText);  /* 하이라이트 텍스트 칼라 설정 함수 */

	void SetFaceName(CString strFaceName);  /* 사용할 폰트명 설정 함수 */
	void SetBold(BOOL bBold);               /* 글자 굵게 옵션 설정 함수 */
	void SetItalic(BOOL bItalic);           /* 이탤릭 옵션 설정 함수 */

	void SetSelection(int nSelect);         /* 선택된 셀 위치 설정 함수 */
	void SetSelection(int nRow, int nCol);  /* 선택된 셀 그리는 함수 */
	void SetStart(int nStart);

	int GetRows();  /* 셀의 ROW 갯수 리턴 함수 */
	int GetCols();  /* 셀의 COL 갯수 리턴 함수 */

	COLORREF GetColorLine();  /* 셀의 테두리 라인 칼라 리턴 함수 */
	COLORREF GetColorBkg();   /* 셀 배경 칼라 리턴 함수 */
	COLORREF GetColorText();  /* 셀에 출력할 텍스트 칼라 리턴 함수 */
	COLORREF GetColorHiliteBkg();   /* 하이라이트 셀 배경 칼라 리턴 함수 */
	COLORREF GetColorHiliteText();  /* 하이라이트 텍스트 칼라 리턴 함수 */

	CString GetFaceName();  /* 셀에 적용할 폰트명 리턴 함수 */
	BOOL GetBold();         /* 글자 굵게 옵션 상태 리턴 함수 */
	BOOL GetItalic();       /* 이탤릭 옵션 상태 리턴 함수 */
	
	int GetSelection();     /* 선택된 셀 위치 리턴 함수 */
	int GetStart();         /* CHAR 시작 값 리턴 함수 */

// Implementation
public:
	void SetTextColor(UINT nBlockNumber, COLORREF TcrColor);
	void SetText(UINT nBlockNumber, CString strText);
	virtual ~CCharSetCtrl();
protected:
    BOOL RegisterWindowClass();

	void DrawCell(CDC* pDC, int nRow, int nCol, const COLORREF crBkColor, const COLORREF crTextColor, CString crText);
	void DrawCell(CDC*, int);
	// Generated message map functions
protected:
	//{{AFX_MSG(CCharSetCtrl)
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(_CHARSETCTRL_H__INCLUDED_)
