#ifndef _EXDIGITST_H
#define _EXDIGITST_H

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#define ST_MIN_PRECISION	1   /* 출력할 데이터 최소 길이 */
#define ST_MAX_PRECISION	10  /* 출력할 데이터 최대 길이 */
#define ST_BORDER_SPACE		3   /* 테두리와 비트맵 사이의 공간 */

/////////////////////////////////////////////////////////////////////////////
// CEXDigitST window

class CEXDigitST : public CStatic
{
// Construction
public:
	CEXDigitST();
	virtual ~CEXDigitST();

    enum {	ST_LEFT			= 0x01, 
			ST_TOP			= 0x02,
			ST_RIGHT		= 0x04,
			ST_BOTTOM		= 0x08};

public:

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEXDigitST)
	//}}AFX_VIRTUAL

// Operations
public:
	void SetValue(double dValue, BOOL bRepaint = TRUE);
	BOOL m_b16Flag;  /* 출력 데이터 종류 저장 변수 */
	
	Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID = NULL) ;  /* 클래스 동적 생성 함수 */
	void SetValue (CString strValue, BOOL bRepaint = TRUE);  /* 새로운 출력 값 설정 함수 */
	BOOL SetStyle(UINT nBitmapId, int nPrecision);           /* 출력 형태 설정 함수 */

	void SetValue(int nValue, BOOL bRepaint = TRUE);         /* 새로운 출력 값 설정 함수 */
	double GetValue();         /* 출력 값 리턴 함수 */

	void SetPrecision(int nPrecision, BOOL bResize = TRUE);  /* 입력 범위 검사 함수 */
	int GetPrecision();     /* 입력 범위 리턴 함수 */

	void SetResize(DWORD dwResize, BOOL bResize = TRUE);     /* 비트맵 정렬 함수 */
	DWORD GetResize();      /* 비트맵 크기 리턴 함수 */

	void SetZeroPadding(BOOL bPad = FALSE, BOOL bRepaint = TRUE);  /* 패딩 스타일 설정 함수 */
	BOOL GetZeroPadding();  /* 패딩 스타일 리턴 함수 */

	void Inc(BOOL bRepaint = TRUE);  /* 출력 데이터 1 증가 함수 */
	void Dec(BOOL bRepaint = TRUE);  /* 출력 데이터 1 감소 함수 */

	static const char* GetVersionC();  /* 클래스 버전 리턴 함수 */
	static const short GetVersionI();  /* 클래스 버전 리턴 함수 */

protected:
	//{{AFX_MSG(CEXDigitST)
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	int m_nDisplayMode;
	double m_dValue;
	void Resize();  /* 비트맵 재 설정 함수 */
	void PrepareString(char* szDest);  /* 출력 문자열 생성 함수 */
	void DrawDigits(CDC* pDC);         /* 디지털 카운터 출력 함수 */

	CBitmap* m_pbmDigit;  /* 비트맵 저장 변수 */
	int m_nWidth;   /* 비트맵 폭 저장 변수 */
	int m_nHeight;  /* 비트맵 높이 저장 변수 */

	int m_nPrecision;    /* 출력할 비트맵 길이 저장 변수 */
	int m_nValue;        /* 출력 데이터 저장 변수 */
	CString m_strValue;  /* 출력 문자열 저장 변수 */

	BOOL m_bZeroPadding;  /* 패딩 플래그 */
	DWORD m_dwResize;     /* 비트맵 정렬 옵션 저장 변수 */
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif
