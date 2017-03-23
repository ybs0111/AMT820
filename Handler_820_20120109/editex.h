// EditEx.h : header file
//
/////////////////////////////////////////////////////////////////////////////
// Copyright (C) 1998-1999 Mirae Corporation
// All rights reserved.
//
// DIVISION    : MIRAE Coperation
// MODIFIED BY : Lee Kyu Il
// REVISION    :
//     1> 2000. 02. 24 : Modified by Lee Kyu Il
// 


#ifndef LOGFONT_H_E7F7D1A0_AE36_11d1_9257_006097CAB461
#define LOGFONT_H_E7F7D1A0_AE36_11d1_9257_006097CAB461

#ifndef __cplusplus
#   error C++ compilation is required.
#endif

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

struct CLogFont : public LOGFONT
{
    CLogFont( LONG fontHeight      = -8, 
              LONG fontWeight      = FW_NORMAL,
              UCHAR pitchAndFamily = DEFAULT_PITCH | FF_DONTCARE,
              LPCSTR faceName      = _T("MS Sans Serif" )
            )
    {
        const int size = sizeof(*this);

        memset( this, 0, size );

        lfHeight         = fontHeight;
        lfWeight         = fontWeight;
        lfPitchAndFamily = pitchAndFamily;

        _ASSERT( LF_FACESIZE > lstrlen( faceName ) );

        lstrcpy( lfFaceName, faceName );
    }

    // Note: No need for CLogFont& operator =(const CLogFont& lf) {...}
    //       We let the compiler handle it...

    void lfFaceNameSet( LPCSTR faceName )
    {
        _ASSERT( faceName );

        if ( !faceName ) return;

        _ASSERT( LF_FACESIZE > lstrlen( faceName ) );

        lstrcpy( lfFaceName, faceName );        
    }

};

#endif // LOGFONT_H_E7F7D1A0_AE36_11d1_9257_006097CAB461

#if !defined(AFX_EDITEX_H__B3788DB1_4FA1_11D1_9FB8_006097B531B3__INCLUDED_)
#define AFX_EDITEX_H__B3788DB1_4FA1_11D1_9FB8_006097B531B3__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

/////////////////////////////////////////////////////////////////////////////
// CEditEx window

class CEditMask
{
public:
    virtual BOOL AddChar( UINT nChar ) = 0;
};


class CEditEx : public CEdit
{
// Construction
public:
	CEditEx();

    DECLARE_DYNCREATE( CEditEx )

// Copy Semantics

   // Block copy construction...
   //
   // No m_hWnd defined for object. 
   //
private:
   CEditEx( const CEditEx& ); 

public:
   // Allow basics to be copied...
   CEditEx& operator = ( const CEditEx& );

// Attributes
public:

    void setCustomMask( CEditMask* editMask )
    {
        if ( m_pEditMask ) delete m_pEditMask;
        m_pEditMask = editMask;
    } 

    COLORREF bkColor  ( COLORREF );    
    COLORREF textColor( COLORREF );

    COLORREF bkColor()   const { return m_crBkColor;   }   
    COLORREF textColor() const { return m_crTextColor; }

    void setFont( const LOGFONT* lpLogFont );

    void setFont( LONG fontHeight      = -8, 
                  LONG fontWeight      = FW_NORMAL,
                  UCHAR pitchAndFamily = DEFAULT_PITCH | FF_DONTCARE,
                  LPCSTR faceName      = _T("MS Sans Serif" ) );
    
    void definePopupMenu( UINT uResourceID = 0 /* 0 uses default */ )
    {
        m_MenuResourceID = uResourceID;
    }

// Operations:
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEditEx)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CEditEx();

	// Generated message map functions
protected:
	//{{AFX_MSG(CEditEx)
    afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG
    afx_msg HBRUSH CtlColor(CDC* pDC, UINT nCtlColor);

	DECLARE_MESSAGE_MAP()

    // Customize your brush
    //
    virtual BOOL CreateBrushType();

private:
    CBrush   m_brBkGround;
    COLORREF m_crBkColor;
    COLORREF m_crTextColor;

    CEditMask* m_pEditMask;

    CFont* m_pCFont;

    UINT m_MenuResourceID;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EDITEX_H__B3788DB1_4FA1_11D1_9FB8_006097B531B3__INCLUDED_)
