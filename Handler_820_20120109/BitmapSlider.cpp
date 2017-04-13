// BitmapSlider.cpp : implementation file
//

#include "stdafx.h"
#include "BitmapSlider.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
//
// CBitmapSlider v1.5
//
// ���� �����Դϴ� - 2003�� 9�� 16�� - ����ȣ
//
/////////////////////////////////////////////////////////////////////////////


CBitmapSlider::CBitmapSlider()
{
	m_nPos = m_nMin = 0;
	m_nMax = 100;
	m_nPage = 20;

	m_nMarginLeft = m_nMarginRight = m_nMarginTop = m_nMarginBottom = 0;
	m_nThumbWidth = m_nThumbHeight = 0;

	m_bChannel = m_bVertical = m_bThumb = m_bLButtonDown = FALSE;

	m_bFocusRect = m_bFocus = FALSE;
	m_bDrawFocusRect = TRUE;

	m_bEnable = TRUE;

	m_nThumbBgX = m_nThumbBgY = -1;
}

CBitmapSlider::~CBitmapSlider()
{
}


BEGIN_MESSAGE_MAP(CBitmapSlider, CStatic)
	//{{AFX_MSG_MAP(CBitmapSlider)
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_WM_GETDLGCODE()
	ON_WM_KEYDOWN()
	ON_WM_KILLFOCUS()
	ON_WM_SETFOCUS()
	ON_WM_CREATE()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBitmapSlider message handlers

BOOL CBitmapSlider::OnEraseBkgnd(CDC* pDC) 
{	
	// Do not erase background for the transparency effect
	return TRUE;
}

// ��Ʈ���� �׸���
//
void CBitmapSlider::OnPaint() 
{
	CPaintDC dcOrigin(this);

	// "Flicker Free Drawing In MFC" by Keith Rule
	CMemDC dc( &dcOrigin, &m_rect, m_bTransparentChannel );

	CDC dcMem;
	dcMem.CreateCompatibleDC( &dc );

	CBitmap *pbmTmp;

	// ��Ŀ���� ���� �������� �˸��� �簢���� �׸���.
	if( m_bFocusRect && ( m_bTransparentChannel || !m_bChannel ) ) {

		dc.DrawFocusRect( m_rect );
		m_bFocusRect = FALSE;
	}

	// ����� �׸���
	if( m_bChannel ) {

		pbmTmp = dcMem.SelectObject( &m_bmChannel );

		// Ȱ��ȭ�� ����� �׸� �ʿ䰡 ���� ��
		if( m_bChannelActive && m_bEnable ) {

			// ����
			if( m_bVertical ) {

				// �Ʒ���
				DrawBitmap( &dc, 0, Pos2Pixel(m_nPos),
					m_nWidth, m_nHeight - Pos2Pixel(m_nPos),
					&dcMem, 0, Pos2Pixel(m_nPos),
					&m_bmChannelActiveMask, m_bTransparentChannel );

				dcMem.SelectObject( &m_bmChannelActive );

				// ����
				DrawBitmap( &dc, 0, 0, m_nWidth, Pos2Pixel(m_nPos),
					&dcMem, 0, 0, &m_bmChannelMask, m_bTransparentChannel );

			// ����
			} else {

				// ������
				DrawBitmap( &dc, Pos2Pixel(m_nPos), 0,
					m_nWidth - Pos2Pixel(m_nPos), m_nHeight,
					&dcMem, Pos2Pixel(m_nPos), 0, 
					&m_bmChannelActiveMask, m_bTransparentChannel );

				dcMem.SelectObject( &m_bmChannelActive );

				// ����
				DrawBitmap( &dc, 0, 0, Pos2Pixel(m_nPos), m_nHeight,
					&dcMem, 0, 0, &m_bmChannelMask, m_bTransparentChannel );
			}

		// Ȱ��ȭ�� ����� ��Ʈ���� ���� ��
		} else {

			DrawBitmap( &dc, 0, 0, m_nWidth, m_nHeight,
				&dcMem, 0, 0, &m_bmChannelMask, m_bTransparentChannel );

		}

		dcMem.SelectObject( pbmTmp );
	}

	// �����̰� �ִ� �ڸ��� ������ ��Ʈ���� ���� ��
	if( m_nThumbBgX != -1 ) {

		RestoreBackground(
			&dc, m_nThumbBgX, m_nThumbBgY,
			m_nThumbWidth, m_nThumbHeight, &m_bmThumbBg );

		m_nThumbBgX = -1;
	}

	// �����̸� �׸���
	if( m_bThumb && m_bEnable ) {

		if( m_bThumbActive && m_bLButtonDown )
			pbmTmp = dcMem.SelectObject( &m_bmThumbActive ); // Ȱ��ȭ
		else
			pbmTmp = dcMem.SelectObject( &m_bmThumb ); // ����

		// ����
		if( m_bVertical ) {

			// �����̰� �Űܰ� �� ����� ������ �ʿ䰡 ���� ��
			if( m_bTransparentChannel || !m_bChannel ) {

				m_nThumbBgX = m_nMarginLeft;
				m_nThumbBgY = Pos2Pixel(m_nPos) - m_nThumbHeight/2;

				CopyBackground(
					&dc, m_nThumbBgX, m_nThumbBgY,
					m_nThumbWidth, m_nThumbHeight, &m_bmThumbBg );
			}

			DrawBitmap(
				&dc, m_nMarginLeft, Pos2Pixel(m_nPos) - m_nThumbHeight/2,
				m_nThumbWidth, m_nThumbHeight,
				&dcMem, 0, 0, &m_bmThumbMask, m_bTransparentThumb );

		// ����
		} else {

			// �����̰� �Űܰ� �� ����� ������ �ʿ䰡 ���� ��
			if( m_bTransparentChannel || !m_bChannel ) {

				m_nThumbBgX = Pos2Pixel(m_nPos) - m_nThumbWidth/2;
				m_nThumbBgY = m_nMarginTop;

				CopyBackground(
					&dc, m_nThumbBgX, m_nThumbBgY,
					m_nThumbWidth, m_nThumbHeight, &m_bmThumbBg );
			}

			DrawBitmap(
				&dc, Pos2Pixel(m_nPos) - m_nThumbWidth/2, m_nMarginTop,
				m_nThumbWidth, m_nThumbHeight,
				&dcMem, 0, 0, &m_bmThumbMask, m_bTransparentThumb );

		} // if ����

		dcMem.SelectObject( pbmTmp );

	} // if ������ �׸���

	// ��Ŀ�� �簢���� �׸���
	if( m_bDrawFocusRect && m_bFocus && m_bEnable ) {

		dc.DrawFocusRect( m_rect );
		m_bFocusRect = TRUE;
	}

	dcMem.DeleteDC();
}

// �����̴��� �ִ밪�� �����մϴ�.
//
// �Ű�����:
//		[IN]	nMax
//				�ִ밪.
//		[IN]	bRedraw
//				TRUE�̸� ��Ʈ���� �ٽ� �׸���
//				FALSE�̸� �ִ밪�� �����մϴ�.
//
void CBitmapSlider::SetRangeMax(int nMax, BOOL bRedraw)
{
	m_nMax = nMax;
	if( bRedraw )
		Invalidate();
}

// �����̴��� �ּҰ��� �����մϴ�.
//
// �Ű�����:
//		[IN]	nMin
//				�ּҰ�.
//		[IN]	bRedraw
//				TRUE�̸� ��Ʈ���� �ٽ� �׸���
//				FALSE�̸� �ִ밪�� �����մϴ�.
//
void CBitmapSlider::SetRangeMin(int nMin, BOOL bRedraw)
{
	m_nMin = nMin;
	if( bRedraw )
		Invalidate();
}

// �����̴��� ������ �����մϴ�.
//
// �Ű�����:
//		[IN]	nMin
//				�ּҰ�.
//		[IN]	nMax
//				�ִ밪.
//		[IN]	bRedraw
//				TRUE�̸� ��Ʈ���� �ٽ� �׸���
//				FALSE�̸� �ִ밪�� �����մϴ�.
//
void CBitmapSlider::SetRange(int nMin, int nMax, BOOL bRedraw)
{
	SetRangeMin( nMin, FALSE );
	SetRangeMax( nMax, bRedraw );
}

// �����̴��� ��ġ ���� �����մϴ�.
//
// �Ű�����:
//		[IN]	nPos
//				���ο� ��ġ ��.
//
void CBitmapSlider::SetPos(int nPos)
{
	m_nPos = nPos;

	// ������ �˻��Ѵ�.
	if( m_nPos > m_nMax )
		m_nPos = m_nMax;
	if( m_nPos < m_nMin )
		m_nPos = m_nMin;

	Invalidate();
}

// �����̴��� ������ ���� �����մϴ�.
//
// �Ű�����:
//		[IN]	nSize
//				���ο� ������ ��..
//
// ���ϰ�:
//		������ ������ ��.
//
int CBitmapSlider::SetPageSize(int nSize)
{
	int nRet = m_nPage;

	m_nPage = nSize;

	return nRet;
}

// ��Ʈ���� ������ �����մϴ�. �����̴� ���� ���ʿ��� �����Դϴ�.
//
void CBitmapSlider::SetMargin(int nLeft, int nTop, int nRight, int nBottom )
{
	SetMarginLeft( nLeft );
	SetMarginTop( nTop );
	SetMarginRight( nRight );
	SetMarginBottom( nBottom );
}

// �����̴��� ������ ������ �� �ֽ��ϴ�.
//
//		[IN]	bEnable
//				TRUE�� �����̴��� �����ϰ� �ϰ�,
//				FALSE�� ������ �����մϴ�.
//
void CBitmapSlider::Enable(BOOL bEnable)
{
	m_bEnable = bEnable;

	// �巡���ϴ� ���� ���
	if( !m_bEnable && m_bLButtonDown ) {

		m_bLButtonDown = FALSE;
		ReleaseCapture();
	}

	Invalidate();
}

// ��Ʈ���� ��Ŀ���� ����� ��, �̸� ǥ���� ������ ���θ� �����մϴ�.
//
//		[IN]	bDraw
//				TRUE�� ǥ�ø�,
//				FALSE�� ǥ������ ������ �ǹ��մϴ�.
//
//		[IN]	bRedraw
//				TRUE���� ��Ʈ���� �ٽ� �׸���,
//				FALSE�� ���°����� �ٲߴϴ�.
//
void CBitmapSlider::DrawFocusRect(BOOL bDraw, BOOL bRedraw)
{
	m_bDrawFocusRect = bDraw;

	if( bRedraw )
		Invalidate();
}

// ��濡 ����� ��Ʈ���� �ҷ��ɴϴ�.
//
// �Ű�����:
//		[IN]	nChannelID
//				��� ��Ʈ���� ID ��.
//		[IN]	nActiveID
//				Ȱ��ȭ�� ��� ��Ʈ���� ID ��.
//		[IN]	bTransparent
//				���� ȿ���� �����Ϸ��� TRUE, �׷��� ������ FALSE�Դϴ�.
//		[IN]	clrpTransColor
//				���� ȿ���� ������ ������ RGB ��.
//		[IN]	iTransPixelX
//				������ ȿ���� �� ������ ��ġ�� ����Ʈ�� x��ǥ ��.
//		[IN]	iTransPixelY
//				������ ȿ���� �� ������ ��ġ�� ����Ʈ�� y��ǥ ��.
//
// ���ϰ�:
//		���������� TRUE �� �׷��� ������FALSE�� �����մϴ�
//
BOOL CBitmapSlider::SetBitmapChannel(
	UINT nChannelID, UINT nActiveID , BOOL bTransparent,
	COLORREF clrpTransColor, int iTransPixelX, int iTransPixelY )
{
	// ��Ʈ���� ID�� �������� �ʾ��� ��
	if( !nChannelID ) {

		m_bChannel = FALSE;

		m_bmChannel.DeleteObject();
		m_bmChannelMask.DeleteObject();
		m_bmChannelActive.DeleteObject();
		m_bmChannelActiveMask.DeleteObject();
	
		return TRUE;
	}

	// ��Ʈ���� �ҷ��´�
	m_bmChannel.DeleteObject();

	if( !m_bmChannel.LoadBitmap( nChannelID ) )
		return FALSE;

	// ������ ��Ʈ���� �׸��� ���� ����ũ�� �����.
	if( bTransparent ) {

		PrepareMask( &m_bmChannel, &m_bmChannelMask,
			clrpTransColor, iTransPixelX, iTransPixelY );
	}

	// Ȱ��ȭ ��� ��Ʈ���� �ҷ��´�.
	if( nActiveID ) {

		m_bmChannelActive.DeleteObject();

		if( !m_bmChannelActive.LoadBitmap( nActiveID ) ) {

			m_bmChannel.DeleteObject();
			if( bTransparent )
				m_bmChannelMask.DeleteObject();

			return FALSE;
		}

		if( bTransparent ) {

			PrepareMask( &m_bmChannelActive, &m_bmChannelActiveMask,
				clrpTransColor, iTransPixelX, iTransPixelY );
		}
		
		m_bChannelActive = TRUE;

	// Ȱ��ȭ ������ ��Ʈ���� �������� ���� ��.
	} else
		m_bChannelActive = FALSE;

	// ��Ʈ���� ũ�� ������ �����´�.
	BITMAP	bitmap;
	m_bmChannel.GetBitmap( &bitmap );

	m_nWidth = bitmap.bmWidth;
	m_nHeight = bitmap.bmHeight;

	// ũ�⸦ ���Ѵ�
	if( m_bChannelActive ) {

		BITMAP	bitmap;
		m_bmChannelActive.GetBitmap( &bitmap );

		ASSERT( m_nWidth == bitmap.bmWidth && m_nHeight == bitmap.bmHeight );
	}

	// ��Ʈ���� ũ�⸦ ��Ʈ���� ũ��� �ٲ۴�.
	SetWindowPos(NULL, 0, 0, m_nWidth, m_nHeight, SWP_NOZORDER | SWP_NOMOVE);

	GetClientRect( &m_rect );

	m_bTransparentChannel = bTransparent;
	m_bChannel = TRUE;

	return TRUE;
}

// �����̿� ����� ��Ʈ���� �ҷ��ɴϴ�.
//
// �Ű�����:
//		[IN]	nThumbID
//				������ ��Ʈ���� ID.
//		[IN]	nActiveID
//				Ȱ��ȭ�� ������ ��Ʈ���� ID.
//		[IN]	bTransparent
//				���� ȿ���� �����Ϸ��� TRUE, �׷��� ������ FALSE�Դϴ�.
//		[IN]	clrpTransColor
//				���� ȿ���� ������ ������ RGB ��.
//		[IN]	iTransPixelX
//				������ ȿ���� �� ������ ��ġ�� ����Ʈ�� x��ǥ ��.
//		[IN]	iTransPixelY
//				������ ȿ���� �� ������ ��ġ�� ����Ʈ�� y��ǥ ��.
//
// ���ϰ�:
//		���������� TRUE �� �׷��� ������FALSE�� �����մϴ�
//
BOOL CBitmapSlider::SetBitmapThumb(
	UINT nThumbID, UINT nActiveID, BOOL bTransparent,
	COLORREF clrpTransColor, int iTransPixelX, int iTransPixelY )
{
	// �������� ID���� NULL�϶�
	if( !nThumbID ) {

		m_bThumb = FALSE;

		m_bmThumb.DeleteObject();
		m_bmThumbMask.DeleteObject();
		m_bmThumbActive.DeleteObject();
		m_bmThumbActiveMask.DeleteObject();
		m_bmThumbBg.DeleteObject();

		return TRUE;
	}

	// ��Ʈ���� �ҷ��´�
	m_bmThumb.DeleteObject();

	if( !m_bmThumb.LoadBitmap( nThumbID ) )
		return FALSE;

	// ������ ��Ʈ���� �׸��� ���� ����ũ�� �����.
	if( bTransparent ) {

		PrepareMask( &m_bmThumb, &m_bmThumbMask,
			clrpTransColor, iTransPixelX, iTransPixelY );
	}

	// Ȱ��ȭ ���¸� ���� ��Ʈ���� �ҷ��´�.
	if( nActiveID ) {

		m_bmThumbActive.DeleteObject();

		if( !m_bmThumbActive.LoadBitmap( nActiveID ) ) {

			m_bmThumb.DeleteObject();
			if( bTransparent )
				m_bmThumbMask.DeleteObject();

			return FALSE;
		}

		if( bTransparent ) {

			PrepareMask( &m_bmThumbActive, &m_bmThumbActiveMask,
				clrpTransColor, iTransPixelX, iTransPixelY );
		}
		
		m_bThumbActive = TRUE;

	// Ȱ��ȭ ������ ��Ʈ���� �������� ���� ��.
	} else
		m_bThumbActive = FALSE;

	// ��Ʈ���� ũ�� ������ �����´�.
	BITMAP	bitmap;
	m_bmThumb.GetBitmap( &bitmap );

	m_nThumbWidth = bitmap.bmWidth;
	m_nThumbHeight = bitmap.bmHeight;

	// ����� ���� ��Ʈ���� ������ ��Ʈ���� ����� ����Ѵ�.
	if( !m_bChannel ) {

		GetClientRect( &m_rect );
		m_nHeight = m_rect.Height();
		m_nWidth = m_rect.Width();
	}

	ASSERT( m_nThumbWidth <= m_nWidth && m_nThumbHeight <= m_nHeight );

	// ũ�⸦ ���Ѵ�
	if( m_bThumbActive ) {

		BITMAP	bitmap;
		m_bmThumbActive.GetBitmap( &bitmap );

		ASSERT(
			m_nThumbWidth == bitmap.bmWidth &&
			m_nThumbHeight == bitmap.bmHeight );
	}

	// �Ӽ����� �����Ѵ�
	m_bTransparentThumb = bTransparent;
	m_bThumb = TRUE;

	return TRUE;
}

// OnLButtonDown
//
// �巡�װ� ���۵� ��
//
void CBitmapSlider::OnLButtonDown(UINT nFlags, CPoint point) 
{
	if( !m_bEnable )
		return;

	SetCapture();
	SetFocus();

	m_bLButtonDown = TRUE;

	// �����̸� ���콺�� Ŭ������ ��
	// ���콺�� ��ǥ�� ������ �߽��� ��ǥ�� ���Ͽ�
	// �� ������ �Ÿ��� ���صд�
	if( m_bVertical ) {

		if( abs( point.y - Pos2Pixel( m_nPos ) ) <= m_nThumbHeight / 2 )
			m_nMouseOffset = point.y - Pos2Pixel( m_nPos );
		else
			m_nMouseOffset = 0;

	} else {

		if( abs( point.x - Pos2Pixel( m_nPos ) ) <= m_nThumbWidth / 2 )
			m_nMouseOffset = point.x - Pos2Pixel( m_nPos );
		else
			m_nMouseOffset = 0;
	}

	OnMouseMove( nFlags, point );
	Invalidate();

	CStatic::OnLButtonDown(nFlags, point);
}

// OnMouseMove
//
// �巡�� ���� ��
//
void CBitmapSlider::OnMouseMove(UINT nFlags, CPoint point) 
{
	if( !m_bLButtonDown || !m_bEnable )
		return;

	int nPixel;

	// ������ ������� �˻��Ѵ�
	if( m_bVertical ) {

		nPixel = point.y - m_nMouseOffset;

		if( nPixel > m_nHeight - m_nMarginBottom - m_nThumbHeight/2 )
			nPixel = m_nHeight - m_nMarginBottom - m_nThumbHeight/2;

		if( nPixel < m_nMarginTop + m_nThumbHeight/2 )
			nPixel = m_nMarginTop + m_nThumbHeight/2;

	} else {

		nPixel = point.x - m_nMouseOffset;

		if( nPixel < m_nMarginLeft + m_nThumbWidth/2 )
			nPixel = m_nMarginLeft + m_nThumbWidth/2;

		if( nPixel > m_nWidth - m_nMarginRight - m_nThumbWidth/2 )
			nPixel = m_nWidth - m_nMarginRight - m_nThumbWidth/2;
	}

	// ���� ������ �����Ѵ�
	if( Pos2Pixel(m_nPos) != nPixel ) {

		SetPos( Pixel2Pos( nPixel ) );

		::PostMessage(
			GetParent()->GetSafeHwnd(), WM_BITMAPSLIDER_MOVING,
			GetDlgCtrlID(), m_nPos );
	}

	CStatic::OnMouseMove(nFlags, point);
}

// OnLButtonUp
//
// �巡�װ� ������ ��
//
void CBitmapSlider::OnLButtonUp(UINT nFlags, CPoint point) 
{
	if( !m_bEnable )
		return;

	ReleaseCapture();
	m_bLButtonDown = FALSE;

	Invalidate();

	::PostMessage(
		GetParent()->GetSafeHwnd(),	WM_BITMAPSLIDER_MOVED,
		GetDlgCtrlID(), m_nPos );

	CStatic::OnLButtonUp(nFlags, point);
}

// ��Ʈ���� ��ġ���� �ش��ϴ� ������ �߽��� ��ǥ���� ���Ѵ�
//
int CBitmapSlider::Pos2Pixel(int nPos)
{
	if( m_bVertical ) {

		return
			m_nMarginTop + m_nThumbHeight/2 +
			(int)(
			( m_nHeight - m_nMarginTop - m_nMarginBottom - m_nThumbHeight ) *
			((double) ( nPos - m_nMin ) / ( m_nMax - m_nMin ) )
			);

	} else {

		return (int)(
			( m_nWidth - m_nMarginLeft - m_nMarginRight - m_nThumbWidth ) *
			((double) ( nPos - m_nMin ) / ( m_nMax - m_nMin ) )
			) + m_nMarginLeft + m_nThumbWidth/2;
	}
}

// ���콺�� ��ǥ���� �ش��ϴ� ��Ʈ���� ��ġ���� ���Ѵ�
//
int CBitmapSlider::Pixel2Pos(int nPixel)
{
	if( m_bVertical ) {

		return (int)(
			m_nMin +
			(double)( nPixel - m_nMarginTop - m_nThumbHeight/2) /
			( m_nHeight - m_nMarginBottom - m_nMarginTop - m_nThumbHeight ) *
			( m_nMax - m_nMin )
			);

	} else {

		return (int)(
			m_nMin +
			(double)( nPixel - m_nMarginLeft - m_nThumbWidth/2 ) /
			( m_nWidth - m_nMarginLeft - m_nMarginRight - m_nThumbWidth ) *
			( m_nMax - m_nMin )
			);
	}
}

// ������ ������ ��Ʈ�� ������ �����´�
//
void CBitmapSlider::CopyBackground(
	CDC *pDC, int nXSrc, int nYSrc, int nWidth, int nHeight, CBitmap *pBmDst)
{
	pBmDst->DeleteObject();
	pBmDst->CreateCompatibleBitmap( pDC, nWidth, nHeight );

	CDC memDC;

	memDC.CreateCompatibleDC( pDC );
	CBitmap *pBmTmp = memDC.SelectObject( pBmDst );

	memDC.BitBlt( 0, 0, nWidth, nHeight, pDC, nXSrc, nYSrc, SRCCOPY );

	memDC.SelectObject( pBmTmp );
	memDC.DeleteDC();
}

// ��Ʈ���� ������ ������ �׸���
//
void CBitmapSlider::RestoreBackground(
	CDC *pDC, int nXDst, int nYDst, int nWidth, int nHeight, CBitmap *pBmSrc)
{
	CDC memDC;

	memDC.CreateCompatibleDC( pDC );
	CBitmap *pBmTmp = memDC.SelectObject( pBmSrc );

	pDC->BitBlt( nXDst, nYDst, nWidth, nHeight, &memDC, 0, 0, SRCCOPY );

	memDC.SelectObject( pBmTmp );
	memDC.DeleteDC();
}

// DrawBitmap
//
// OnPaint�� �ڵ带 �б� ���� �ϱ����� �Լ�
//
void CBitmapSlider::DrawBitmap(
	CDC *pDC, int xStart, int yStart, int wWidth, int wHeight,
	CDC *pTmpDC, int xSource, int ySource, CBitmap *bmMask, BOOL bTransparent )
{
	if( bTransparent ) {

		DrawTransparentBitmap(
			pDC, xStart, yStart,
			wWidth, wHeight,
			pTmpDC, xSource, ySource, bmMask );

	} else {

		pDC->BitBlt( xStart, yStart,
			wWidth, wHeight,
			pTmpDC, xSource, ySource, SRCCOPY );
	}
}

// PrepareMask
//
// "Drawing Transparent Bitmap with ease with on the fly masks in MFC"
// By Raja Segar
//
// clrpTransColor�� �⺻���� NULL(black)����
// 0xFF000000(RGB ���� �ƴ� ��)���� �ٲ���ϴ�.
//
void CBitmapSlider::PrepareMask(
	CBitmap *pBmpSource, CBitmap *pBmpMask,
	COLORREF clrpTransColor, int iTransPixelX, int iTransPixelY)
{
	BITMAP bm;

	// Get the dimensions of the source bitmap
	pBmpSource->GetObject(sizeof(BITMAP), &bm);

	// Create the mask bitmap
	pBmpMask->DeleteObject();
	pBmpMask->CreateBitmap( bm.bmWidth, bm.bmHeight, 1, 1, NULL);

	// We will need two DCs to work with. One to hold the Image
	// (the source), and one to hold the mask (destination).
	// When blitting onto a monochrome bitmap from a color, pixels
	// in the source color bitmap that are equal to the background
	// color are blitted as white. All the remaining pixels are
	// blitted as black.

	CDC hdcSrc, hdcDst;

	hdcSrc.CreateCompatibleDC(NULL);
	hdcDst.CreateCompatibleDC(NULL);

	// Load the bitmaps into memory DC
	CBitmap* hbmSrcT = (CBitmap*) hdcSrc.SelectObject(pBmpSource);
	CBitmap* hbmDstT = (CBitmap*) hdcDst.SelectObject(pBmpMask);

	// Dynamically get the transparent color
	COLORREF clrTrans;
	if (clrpTransColor == 0xFF000000)
	{
		// User did not specify trans color so get it from bmp
		clrTrans = hdcSrc.GetPixel(iTransPixelX, iTransPixelY);
	}
	else
	{
		clrTrans = clrpTransColor;
	}


	// Change the background to trans color
	COLORREF clrSaveBk  = hdcSrc.SetBkColor(clrTrans);

	// This call sets up the mask bitmap.
	hdcDst.BitBlt(0,0,bm.bmWidth, bm.bmHeight, &hdcSrc,0,0,SRCCOPY);

	// Now, we need to paint onto the original image, making
	// sure that the "transparent" area is set to black. What
	// we do is AND the monochrome image onto the color Image
	// first. When blitting from mono to color, the monochrome
	// pixel is first transformed as follows:
	// if  1 (black) it is mapped to the color set by SetTextColor().
	// if  0 (white) is is mapped to the color set by SetBkColor().
	// Only then is the raster operation performed.

	COLORREF clrSaveDstText = hdcSrc.SetTextColor(RGB(255,255,255));
	hdcSrc.SetBkColor(RGB(0,0,0));

	hdcSrc.BitBlt(0,0,bm.bmWidth, bm.bmHeight, &hdcDst,0,0,SRCAND);

	// Clean up by deselecting any objects, and delete the
	// DC's.
	hdcDst.SetTextColor(clrSaveDstText);

	hdcSrc.SetBkColor(clrSaveBk);
	hdcSrc.SelectObject(hbmSrcT);
	hdcDst.SelectObject(hbmDstT);

	hdcSrc.DeleteDC();
	hdcDst.DeleteDC();
}

// DrawTransparentBitmap
//
// "Drawing Transparent Bitmap with ease with on the fly masks in MFC"
// By Raja Segar
//
void CBitmapSlider::DrawTransparentBitmap(
	CDC *pDC, int xStart, int yStart, int wWidth, int wHeight,
	CDC *pTmpDC, int xSource, int ySource, CBitmap *bmMask )
{
	// We are going to paint the two DDB's in sequence to the destination.
	// 1st the monochrome bitmap will be blitted using an AND operation to
	// cut a hole in the destination. The color image will then be ORed
	// with the destination, filling it into the hole, but leaving the
	// surrounding area untouched.

	CDC hdcMem;
	hdcMem.CreateCompatibleDC(NULL);

	CBitmap* hbmT = hdcMem.SelectObject(bmMask);

	pDC->BitBlt( xStart, yStart, wWidth, wHeight, &hdcMem,
		xSource, ySource, SRCAND);

	// Also note the use of SRCPAINT rather than SRCCOPY.

	pDC->BitBlt(xStart, yStart, wWidth, wHeight, pTmpDC,
		xSource, ySource,SRCPAINT);

	// Now, clean up.
	hdcMem.SelectObject(hbmT);
	hdcMem.DeleteDC();
}

// Ű���� �Է��� �ޱ� ����
//
UINT CBitmapSlider::OnGetDlgCode() 
{
	if( GetKeyState(VK_TAB) >= 0 ) {

		return  DLGC_WANTALLKEYS;
	}
	
	return CStatic::OnGetDlgCode();
}

// Ű������ �Է��� ó���Ѵ�
//
void CBitmapSlider::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	if( !m_bEnable )
		return;

	switch( nChar ) {

	// ����, ����
	case VK_LEFT :
	case VK_UP :

		SetPos( m_nPos-1 );
		break;

	// ������, �Ʒ���
	case VK_RIGHT :
	case VK_DOWN :

		SetPos( m_nPos+1 );
		break;

	// Home
	case VK_HOME :

		SetPos( m_nMin );
		break;

	// End
	case VK_END :

		SetPos( m_nMax );
		break;

	// Page up
	case VK_PRIOR :

		SetPos( m_nPos - m_nPage );
		break;

	// Page down
	case VK_NEXT :

		SetPos( m_nPos + m_nPage );
		break;

	default :

		CStatic::OnKeyDown(nChar, nRepCnt, nFlags);
		return;
	}
	
	::PostMessage(
		GetParent()->GetSafeHwnd(),	WM_BITMAPSLIDER_MOVED,
		GetDlgCtrlID(), m_nPos );

	CStatic::OnKeyDown(nChar, nRepCnt, nFlags);
}

// ��Ʈ���� ��Ŀ���� �Ҿ��� ��
//
void CBitmapSlider::OnKillFocus(CWnd* pNewWnd) 
{
	CStatic::OnKillFocus(pNewWnd);

	m_bFocus = FALSE;
	Invalidate();
}

// ��Ʈ���� ��Ŀ���� ����� ��
//
void CBitmapSlider::OnSetFocus(CWnd* pOldWnd) 
{
	CStatic::OnSetFocus(pOldWnd);

	m_bFocus = TRUE;
	Invalidate();
}

// ��Ʈ���� �����Ѵ�
//
void CBitmapSlider::OnDestroy() 
{
	CStatic::OnDestroy();
	
	m_bmThumb.DeleteObject();
	m_bmThumbMask.DeleteObject();

	m_bmThumbActive.DeleteObject();
	m_bmThumbActiveMask.DeleteObject();

	m_bmThumbBg.DeleteObject();

	m_bmChannel.DeleteObject();
	m_bmChannelMask.DeleteObject();

	m_bmChannelActive.DeleteObject();
	m_bmChannelActiveMask.DeleteObject();
}
