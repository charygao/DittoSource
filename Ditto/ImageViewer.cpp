// ImageViewer.cpp : implementation file
//

#include "stdafx.h"
#include "CP_Main.h"
#include "ImageViewer.h"
#include "BitmapHelper.h"


// CImageViewer

IMPLEMENT_DYNAMIC(CImageViewer, CWnd)

CImageViewer::CImageViewer()
{
	m_scrollHelper.AttachWnd(this);
	m_hoveringOverImage = false;
	m_pGdiplusBitmap = NULL;
}

CImageViewer::~CImageViewer()
{
	delete m_pGdiplusBitmap;
}


BEGIN_MESSAGE_MAP(CImageViewer, CWnd)
	ON_WM_HSCROLL()
	ON_WM_VSCROLL()
	ON_WM_MOUSEWHEEL()
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_SETCURSOR()
	ON_WM_LBUTTONUP()
END_MESSAGE_MAP()

BOOL CImageViewer::Create(CWnd* pParent)
{
	BOOL bSuccess;

	// Register window class
	CString csClassName = AfxRegisterWndClass(CS_OWNDC | CS_HREDRAW | CS_VREDRAW,
		LoadCursor(NULL, IDC_ARROW),
		CBrush(::GetSysColor(COLOR_BTNFACE)));



	// If no parent supplied then try and get a pointer to it anyway
	if (!pParent)
		pParent = AfxGetMainWnd();

	// Create popup window
	//bSuccess = CreateEx(WS_EX_DLGMODALFRAME|WS_EX_TOPMOST, // Extended style
	bSuccess = CreateEx(0,
		csClassName,                       // Classname
		_T(""),                          // Title
		WS_CHILD | WS_VISIBLE | WS_VSCROLL | WS_HSCROLL,     // style
		0, 0,                               // position - updated soon.
		390, 130,                           // Size - updated soon
		pParent->GetSafeHwnd(),            // handle to parent
		0,                                 // No menu
		NULL);
	if (!bSuccess) return FALSE;



	return TRUE;
}

void CImageViewer::UpdateBitmapSize()
{
	if (m_pGdiplusBitmap != NULL)
	{
		if (CGetSetOptions::GetScaleImagesToDescWindow())
		{
			CRect rect;
			GetClientRect(rect);

			m_scrollHelper.SetDisplaySize(0, 0);
			m_scrollHelper.DetachWnd();
			
		}
		else
		{
			m_scrollHelper.AttachWnd(this);
			m_scrollHelper.SetDisplaySize(m_pGdiplusBitmap->GetWidth(), m_pGdiplusBitmap->GetHeight());
		}
	}
}

void CImageViewer::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	CRect rect;
	GetClientRect(rect);	

	CBrush  Brush, *pOldBrush;
	Brush.CreateSolidBrush(g_Opt.m_Theme.DescriptionWindowBG());

	pOldBrush = dc.SelectObject(&Brush);

	dc.FillRect(&rect, &Brush);

	if (m_pGdiplusBitmap)
	{
		int width = m_pGdiplusBitmap->GetWidth();
		int height = m_pGdiplusBitmap->GetHeight();
		
		if (CGetSetOptions::GetScaleImagesToDescWindow())
		{
			double newWidth = rect.Width();
			double newHeight = rect.Height();

			if (width > 0 &&
				height > 0 &&
				rect.Width() > 0 &&
				rect.Height() > 0)
			{
				float origAspect = (width / (float)height);
				float newAspect = (rect.Width() / (float)rect.Height());

				if (origAspect > newAspect)
				{
					newHeight = (rect.Width() * height) / width;
				}
				else
				{
					newWidth = (rect.Height() * width) / height;
				}
			}

			Gdiplus::ImageAttributes attrs;
			Gdiplus::Rect dest(0, 0, newWidth, newHeight);

			Gdiplus::Graphics graphics(dc);
			graphics.SetInterpolationMode(Gdiplus::InterpolationModeHighQualityBicubic);
			graphics.SetPixelOffsetMode(Gdiplus::PixelOffsetModeHalf);

			graphics.DrawImage(m_pGdiplusBitmap, dest, 0, 0, width, height, Gdiplus::UnitPixel, &attrs);


		}
		else
		{
			CSize s = m_scrollHelper.GetScrollPos();
			Gdiplus::Graphics graphics(dc);
			graphics.DrawImage(m_pGdiplusBitmap, rect.left, rect.top, s.cx, s.cy, width, height, Gdiplus::UnitPixel);

			//dc.BitBlt(rect.left, rect.top, width, height, &MemDc, s.cx, s.cy, SRCCOPY);
		}
		
		rect.top += height;
	}
	
	// Cleanup
	dc.SelectObject(pOldBrush);
}


void CImageViewer::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	m_scrollHelper.OnHScroll(nSBCode, nPos, pScrollBar);
}

void CImageViewer::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	m_scrollHelper.OnVScroll(nSBCode, nPos, pScrollBar);
}

BOOL CImageViewer::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	BOOL wasScrolled = m_scrollHelper.OnMouseWheel(nFlags, zDelta, pt);
	return wasScrolled;
}

void CImageViewer::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);
	m_scrollHelper.OnSize(nType, cx, cy);
}

BOOL CImageViewer::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	if (this->m_pGdiplusBitmap &&
		pWnd->m_hWnd == this->m_hWnd &&
		nHitTest == HTCLIENT)
	{
		if (CGetSetOptions::GetScaleImagesToDescWindow())
		{
			::SetCursor(AfxGetApp()->LoadCursor(IDC_CURSOR_ZOOM_IN));
		}
		else
		{
			::SetCursor(AfxGetApp()->LoadCursor(IDC_CURSOR_ZOOM_OUT));
		}

		m_hoveringOverImage = true;

		return TRUE;
	}

	m_hoveringOverImage = false;

	return CWnd::OnSetCursor(pWnd, nHitTest, message);
}


void CImageViewer::OnLButtonUp(UINT nFlags, CPoint point)
{
	if (this->m_pGdiplusBitmap &&
		m_hoveringOverImage)
	{
		CGetSetOptions::SetScaleImagesToDescWindow(!CGetSetOptions::GetScaleImagesToDescWindow());
		this->UpdateBitmapSize();
		Invalidate();
		return;
	}

	CWnd::OnLButtonUp(nFlags, point);
}
