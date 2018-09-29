#include "stdafx.h"
#include ".\dittowindow.h"
#include "CP_Main.h"
#include "Options.h"


CDittoWindow::CDittoWindow(void)
{
	m_captionBorderWidth = theApp.m_metrics.ScaleX(25);

	m_lTopBorder = m_captionBorderWidth;
	m_lRightBorder = BORDER;
	m_lBottomBorder = BORDER;
	m_lLeftBorder = BORDER;
	m_bMouseOverChevron = false;
	m_bMouseDownOnChevron = false;
	m_bMouseDownOnClose = false;
	m_bMouseOverClose = false;
	m_bMouseDownOnMinimize = false;
	m_bMouseOverMinimize = false;
	m_bMouseDownOnMaximize = false;
	m_bMouseOverMaximize = false;
	m_bDrawClose = true;
	m_bDrawChevron = true;
	m_bDrawMaximize = true;
	m_bDrawMinimize = true;
	m_bMinimized = false;
	m_crCloseBT.SetRectEmpty();
	m_crChevronBT.SetRectEmpty();
	m_crMaximizeBT.SetRectEmpty();
	m_crMinimizeBT.SetRectEmpty();
	m_CaptionColorLeft = RGB(255, 255, 255);
	m_CaptionColorRight = RGB(204, 204, 204);
	m_CaptionTextColor = RGB(191, 191, 191);
	m_border = RGB(204, 204, 204);
	m_sendWMClose = true;
	m_customWindowTitle = _T("");
	m_useCustomWindowTitle = false;
	m_buttonDownOnCaption = false;
	m_crFullSizeWindow.SetRectEmpty();	
}

CDittoWindow::~CDittoWindow(void)
{
}

void CDittoWindow::DoCreate(CWnd *pWnd)
{
	m_VertFont.CreateFont(theApp.m_metrics.PointsToPixels(18), 0, -900, 0, 400, FALSE, FALSE, 0, DEFAULT_CHARSET,
							OUT_DEFAULT_PRECIS,	CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, 
							DEFAULT_PITCH|FF_SWISS, _T("Segoe UI"));

	m_HorFont.CreateFont(theApp.m_metrics.PointsToPixels(18), 0, 0, 0, 500, FALSE, FALSE, 0, DEFAULT_CHARSET,
						OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,
						DEFAULT_PITCH|FF_SWISS, _T("Segoe UI"));

	SetTitleTextHeight(pWnd);
	
	m_closeButton.LoadStdImageDPI(Close_Black_16_16, Close_Black_20_20, Close_Black_24_24, Close_Black_28, Close_Black_32_32, _T("PNG"));
	m_chevronRightButton.LoadStdImageDPI(ChevronRight_Black_16_16, ChevronRight_Black_20_20, ChevronRight_Black_24_24, ChevronRight_Black_28, ChevronRight_Black_32_32, _T("PNG"));
	m_chevronLeftButton.LoadStdImageDPI(ChevronLeft_Black_16_16, ChevronLeft_Black_20_20, ChevronLeft_Black_24_24, ChevronLeft_Black_28, ChevronLeft_Black_32_32, _T("PNG"));
	m_maximizeButton.LoadStdImageDPI(IDB_MAXIMIZE_16_16, maximize_20, maximize_24, maximize_28, maximize_32, _T("PNG"));
	m_minimizeButton.LoadStdImageDPI(minimize_16, minimize_20, minimize_24, minimize_28, minimize_32, _T("PNG"));
	//m_windowIcon.LoadStdImageDPI(NewWindowIcon_24_14, NewWindowIcon_30, NewWindowIcon_36, NewWindowIcon_48, _T("PNG"));
}

void CDittoWindow::DoNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS FAR* lpncsp)
{
	//Decrease the client area
	lpncsp->rgrc[0].left += m_lLeftBorder;
	lpncsp->rgrc[0].top += m_lTopBorder;
	lpncsp->rgrc[0].right -= m_lRightBorder;
	lpncsp->rgrc[0].bottom -= m_lBottomBorder;
}

UINT CDittoWindow::DoNcHitTest(CWnd *pWnd, CPoint point) 
{
	CRect crWindow;
	pWnd->GetWindowRect(crWindow);

	if(crWindow.PtInRect(point) == false)
	{
		return -1;
	}
	
	int x = point.x - crWindow.left;
	int y = point.y - crWindow.top;

	CPoint myLocal(x, y);

	//http://stackoverflow.com/questions/521147/the-curious-problem-of-the-missing-wm-nclbuttonup-message-when-a-window-isnt-ma
	//workaround for l button up not coming after a lbutton down
	if (m_crCloseBT.PtInRect(myLocal) ||
		m_crChevronBT.PtInRect(myLocal) ||
		m_crMinimizeBT.PtInRect(myLocal) ||
		m_crMaximizeBT.PtInRect(myLocal))
	{
		return HTBORDER;;
	}

	if(m_bMinimized == false)
	{
		if ((point.y < crWindow.top + BORDER * 4) &&
			(point.x < crWindow.left + BORDER * 4))
			return HTTOPLEFT;
		else if ((point.y < crWindow.top + BORDER * 4) &&
			(point.x > crWindow.right - BORDER * 4))
			return HTTOPRIGHT;
		else if ((point.y > crWindow.bottom - BORDER * 4) &&
			(point.x > crWindow.right - BORDER * 4))
			return HTBOTTOMRIGHT;
		else if ((point.y > crWindow.bottom - BORDER * 4) &&
			(point.x < crWindow.left + BORDER * 4))
			return HTBOTTOMLEFT;
	}

	if((((m_lTopBorder == m_captionBorderWidth) || (m_lBottomBorder == m_captionBorderWidth)) &&
		(m_bMinimized)) == false)
	{
		if (point.y < crWindow.top + BORDER * 2)
			return HTTOP;
		if (point.y > crWindow.bottom - BORDER * 2)
			return HTBOTTOM;
	}

	if((((m_lLeftBorder == m_captionBorderWidth) || (m_lRightBorder == m_captionBorderWidth)) &&
		(m_bMinimized)) == false)
	{
		if (point.x > crWindow.right - BORDER * 2)
			return HTRIGHT;
		if (point.x < crWindow.left + BORDER * 2)
			return HTLEFT;
	}

	if(m_lRightBorder == m_captionBorderWidth)
	{
		if (point.x > crWindow.right - m_lRightBorder)
			return HTCAPTION;
	}
	else if(m_lBottomBorder == m_captionBorderWidth)
	{
		if(point.y > crWindow.bottom - m_lBottomBorder)
			return HTCAPTION;
	}
	else if(m_lLeftBorder == m_captionBorderWidth)
	{
		if (point.x < crWindow.left + m_lLeftBorder)
			return HTCAPTION;
	}
	else if(m_lTopBorder == m_captionBorderWidth)
	{
		if (point.y < crWindow.top + m_lTopBorder)
			return HTCAPTION;
	}

	return -1;
}

int IndexToPos(int index, bool horizontal)
{
	switch (index)
	{
	case 0:
		if (horizontal)
		{
			return theApp.m_metrics.ScaleX(24);
		}
		else
		{
			return theApp.m_metrics.ScaleY(8);
		}
		break;
	case 1:
		if (horizontal)
		{
			return theApp.m_metrics.ScaleX(48);
		}
		else
		{
			return theApp.m_metrics.ScaleY(32);
		}
		break;
	case 2:

		if (horizontal)
		{
			return theApp.m_metrics.ScaleX(72);
		}
		else
		{
			return theApp.m_metrics.ScaleY(56);
		}
		break;
	case 3:
		if (horizontal)
		{
			return theApp.m_metrics.ScaleX(96);
		}
		else
		{
			return theApp.m_metrics.ScaleY(80);
		}
		break;
	case 4:
		if (horizontal)
		{
			return theApp.m_metrics.ScaleX(104);
		}
		else
		{
			return theApp.m_metrics.ScaleY(104);
		}
		break;
	}

	return 0;
}

void CDittoWindow::DoNcPaint(CWnd *pWnd)
{
	CWindowDC dc(pWnd);

	CRect rcFrame;
	pWnd->GetWindowRect(rcFrame);
	pWnd->ScreenToClient(rcFrame);

	CRect rc;
	pWnd->GetClientRect(rc);
	pWnd->ClientToScreen(rc);

	long lWidth = rcFrame.Width();

	// Draw the window border
	CRect rcBorder(0, 0, lWidth, rcFrame.Height());

	int border = theApp.m_metrics.ScaleX(2);
	int widthHeight = theApp.m_metrics.ScaleX(16);

	for (int x = 0; x < border; x++)
	{
		dc.Draw3dRect(rcBorder, m_border, m_border);
		rcBorder.DeflateRect(1, 1, 1, 1);
	}

	int iconArea = 0;
	int index = 0;
	int closeIndex = 0;
	int chevronIndex = 0;
	int minIndex = 0;
	int maxIndex = 0;

	if (m_bDrawClose)
	{
		iconArea += theApp.m_metrics.ScaleX(32);
		closeIndex = index++;
	}
	if (m_bDrawChevron)
	{
		iconArea += theApp.m_metrics.ScaleX(32);
		chevronIndex = index++;
	}
	if (m_bDrawMaximize)
	{
		iconArea += theApp.m_metrics.ScaleX(32);
		maxIndex = index++;
	}
	if (m_bDrawMinimize)
	{
		iconArea += theApp.m_metrics.ScaleX(32);
		minIndex = index++;
	}
	
	CRect leftRect;
	CRect rightRect;
	CRect textRect;	

	BOOL bVertical = FALSE;
	if(m_lRightBorder == m_captionBorderWidth)
	{
		rightRect.SetRect(rcBorder.right - (m_captionBorderWidth - border), rcBorder.top, rcBorder.right, rcBorder.top + IndexToPos(index, false));
		leftRect.SetRect(rcBorder.right - (m_captionBorderWidth - border), rcBorder.top + IndexToPos(index, false), rcBorder.right, rcBorder.bottom);
		
		textRect.SetRect(rcBorder.right, rightRect.bottom + theApp.m_metrics.ScaleX(10), rcBorder.right - m_captionBorderWidth, rcBorder.bottom - theApp.m_metrics.ScaleX(1));

		int left = rightRect.left;
		int right = rightRect.right;

		int top = IndexToPos(closeIndex, false);
		m_crCloseBT.SetRect(left, top, right, top+ widthHeight);

		top = IndexToPos(chevronIndex, false);
		m_crChevronBT.SetRect(left, top, right, top + widthHeight);

		top = IndexToPos(maxIndex, false);
		m_crMaximizeBT.SetRect(left, top, right, top + widthHeight);

		top = IndexToPos(minIndex, false);
		m_crMinimizeBT.SetRect(left, top, right, top + widthHeight);


		m_crWindowIconBT.SetRect(rcBorder.right - theApp.m_metrics.ScaleX(24), rcBorder.bottom - theApp.m_metrics.ScaleX(28), rcBorder.right - theApp.m_metrics.ScaleX(2), rcBorder.bottom);

		bVertical = TRUE;
	}
	else if(m_lLeftBorder == m_captionBorderWidth)
	{
		rightRect.SetRect(rcBorder.left, rcBorder.top, rcBorder.left + m_captionBorderWidth - border, rcBorder.top + IndexToPos(index, false));
		leftRect.SetRect(rcBorder.left, rcBorder.top + IndexToPos(index, false), rcBorder.left + m_captionBorderWidth - border, rcBorder.bottom);

		textRect.SetRect(rcBorder.left + m_captionBorderWidth - theApp.m_metrics.ScaleX(0), rightRect.bottom + theApp.m_metrics.ScaleX(10), rcBorder.left - theApp.m_metrics.ScaleX(5), rcBorder.bottom - theApp.m_metrics.ScaleX(1));

		int left = rightRect.left;
		int right = rightRect.right;

		int top = IndexToPos(closeIndex, false);
		m_crCloseBT.SetRect(left, top, right, top + widthHeight);

		top = IndexToPos(chevronIndex, false);
		m_crChevronBT.SetRect(left, top, right, top + widthHeight);

		top = IndexToPos(maxIndex, false);
		m_crMaximizeBT.SetRect(left, top, right, top + widthHeight);

		top = IndexToPos(minIndex, false);
		m_crMinimizeBT.SetRect(left, top, right, top + widthHeight);

		m_crWindowIconBT.SetRect(rcBorder.left + theApp.m_metrics.ScaleX(0), rcBorder.bottom - theApp.m_metrics.ScaleX(28), rcBorder.left + theApp.m_metrics.ScaleX(25), rcBorder.bottom);

		bVertical = TRUE;
	}
	else if(m_lTopBorder == m_captionBorderWidth)
	{
		leftRect.SetRect(rcBorder.left, rcBorder.top, rcBorder.right - IndexToPos(index-1, true)- theApp.m_metrics.ScaleX(8), m_captionBorderWidth);
		rightRect.SetRect(leftRect.right, rcBorder.top, rcBorder.right, m_captionBorderWidth);

		textRect.SetRect(leftRect.right, leftRect.top, leftRect.right, leftRect.bottom);

		int top = rightRect.top;
		int bottom = rightRect.bottom;

		int left = rcBorder.right - IndexToPos(closeIndex, true);
		m_crCloseBT.SetRect(left, top, left + widthHeight, bottom);

		left = rcBorder.right - IndexToPos(chevronIndex, true);
		m_crChevronBT.SetRect(left, top, left + widthHeight, bottom);
		
		left = rcBorder.right - IndexToPos(maxIndex, true);
		m_crMaximizeBT.SetRect(left, top, left + widthHeight, bottom);
		
		left = rcBorder.right - IndexToPos(minIndex, true);
		m_crMinimizeBT.SetRect(left, top, left + widthHeight, bottom);
				left = rcBorder.left + theApp.m_metrics.ScaleX(10);
		m_crWindowIconBT.SetRect(left, top, left + theApp.m_metrics.ScaleX(24), bottom);
		
		bVertical = FALSE;
	}
	else if(m_lBottomBorder == m_captionBorderWidth)
	{
		leftRect.SetRect(rcBorder.left, rcBorder.bottom- m_captionBorderWidth - border, rcBorder.right - IndexToPos(index - 1, true) - theApp.m_metrics.ScaleX(8), rcBorder.bottom);
		rightRect.SetRect(leftRect.right, rcBorder.bottom - m_captionBorderWidth - border, rcBorder.right, rcBorder.bottom);

		textRect.SetRect(leftRect.right, leftRect.top, leftRect.right, leftRect.bottom);

		int top = rightRect.top;
		int bottom = rightRect.bottom;

		int left = rcBorder.right - IndexToPos(closeIndex, true);
		m_crCloseBT.SetRect(left, top, left + widthHeight, bottom);

		left = rcBorder.right - IndexToPos(chevronIndex, true);
		m_crChevronBT.SetRect(left, top, left+ widthHeight, bottom);

		left = rcBorder.right - IndexToPos(maxIndex, true);
		m_crMaximizeBT.SetRect(left, top, left + widthHeight, bottom);

		left = rcBorder.right - IndexToPos(minIndex, true);
		m_crMinimizeBT.SetRect(left, top, left + widthHeight, bottom);

		left = rcBorder.left + theApp.m_metrics.ScaleX(10);
		m_crWindowIconBT.SetRect(left, top, left + theApp.m_metrics.ScaleX(24), bottom);

		bVertical = FALSE;
	}


	HBRUSH leftColor = CreateSolidBrush(m_CaptionColorLeft);
	HBRUSH rightColor = CreateSolidBrush(m_CaptionColorRight);	

	::FillRect(dc, &leftRect, leftColor);
	::FillRect(dc, &rightRect, rightColor);

	DeleteObject(leftColor);
	DeleteObject(rightColor);


	int nOldBKMode = dc.SetBkMode(TRANSPARENT);
	COLORREF oldColor = dc.SetTextColor(m_CaptionTextColor);

	CFont *pOldFont = NULL;
	if (bVertical)
		pOldFont = dc.SelectObject(&m_VertFont);
	else
		pOldFont = dc.SelectObject(&m_HorFont);

	CString csText = m_customWindowTitle;
	if (m_useCustomWindowTitle == false)
	{
		pWnd->GetWindowText(csText);
	}

	int flags = DT_SINGLELINE;
	if (bVertical == false)
	{
		CRect size(0, 0, 0, 0);
		dc.DrawText(csText, size, DT_CALCRECT);
		textRect.left = textRect.right - size.Width() - theApp.m_metrics.ScaleX(10);

		flags |= DT_VCENTER;
	}
	else
	{
		CRect size(0, 0, 0, 0);
		dc.DrawText(csText, size, DT_CALCRECT| DT_SINGLELINE);

		int rectWidth = textRect.left - textRect.right;
		int offset = rectWidth / 2 - m_titleTextHeight / 2;
		//textRect.right += 30;
		//I don't understand where the 4 is coming from but it's always 4 pixals from the right so adjust for this
		textRect.left -= (offset - theApp.m_metrics.ScaleX(4));		

		int k = 0;
	}

	dc.DrawText(csText, textRect, flags);

	dc.SelectObject(pOldFont);
	dc.SetBkMode(nOldBKMode);

	DrawWindowIcon(dc, pWnd);
	DrawChevronBtn(dc, pWnd);
	DrawCloseBtn(dc, pWnd);
	DrawMaximizeBtn(dc, pWnd);
	DrawMinimizeBtn(dc, pWnd);
}

void CDittoWindow::DoSetRegion(CWnd *pWnd)
{
	return;
	//Create the region for drawing the rounded top edge

	CRect rect;
	CRgn rgnRect, rgnRect2, rgnRound, rgnFinalA, rgnFinalB;
	pWnd->GetWindowRect(rect);

	if(rect.Width() < 0)
		return;

	CRect r;
	pWnd->GetClientRect(&r);

	int seven = theApp.m_metrics.ScaleX(7);
	int fifteen = theApp.m_metrics.ScaleX(15);
	int one = theApp.m_metrics.ScaleX(1);
	
	if((m_lRightBorder == m_captionBorderWidth) ||
		(m_lTopBorder == m_captionBorderWidth))
	{		
		rgnRect.CreateRectRgn(0, 0, rect.Width() - seven, rect.Height());
		rgnRound.CreateRoundRectRgn(0, 0, rect.Width() + one, rect.Height(), fifteen, fifteen);

		rgnFinalB.CreateRectRgn(0, 0, 0, 0);
		rgnFinalB.CombineRgn(&rgnRect, &rgnRound, RGN_OR);

		rgnRect2.CreateRectRgn(0, seven, rect.Width(), rect.Height());
		rgnFinalA.CreateRectRgn(0, 0, 0, 0);
		rgnFinalA.CombineRgn(&rgnRect2, &rgnFinalB, RGN_OR);

		//Set the region
		pWnd->SetWindowRgn(rgnFinalA, TRUE);
	}
	else if(m_lLeftBorder == m_captionBorderWidth)
	{
		rgnRect.CreateRectRgn(0, seven, rect.Width(), rect.Height());
		rgnRound.CreateRoundRectRgn(0, 0, rect.Width(), rect.Height(), fifteen, fifteen);

		rgnFinalB.CreateRectRgn(0, 0, 0, 0);
		rgnFinalB.CombineRgn(&rgnRect, &rgnRound, RGN_OR);

		rgnRect2.CreateRectRgn(seven, 0, rect.Width(), rect.Height());
		rgnFinalA.CreateRectRgn(0, 0, 0, 0);
		rgnFinalA.CombineRgn(&rgnRect2, &rgnFinalB, RGN_OR);

		pWnd->SetWindowRgn(rgnFinalA, TRUE);
	}
	else if(m_lBottomBorder == m_captionBorderWidth)
	{
		rgnRect.CreateRectRgn(0, 0, rect.Width(), rect.Height() - seven);
		rgnRound.CreateRoundRectRgn(0, 0, rect.Width() + one, rect.Height() + one, fifteen, fifteen);

		rgnFinalB.CreateRectRgn(0, 0, 0, 0);
		rgnFinalB.CombineRgn(&rgnRect, &rgnRound, RGN_OR);

		rgnRect2.CreateRectRgn(0, 0, rect.Width() - fifteen, rect.Height());
		rgnFinalA.CreateRectRgn(0, 0, 0, 0);
		rgnFinalA.CombineRgn(&rgnRect2, &rgnFinalB, RGN_OR);

		pWnd->SetWindowRgn(rgnFinalA, TRUE);
	}
}

void CDittoWindow::DrawChevronBtn(CWindowDC &dc, CWnd *pWnd)
{
	if(m_bDrawChevron == false)
	{
		return;
	}
		
	if(this->m_bMinimized)
	{
		m_chevronLeftButton.Draw(&dc, pWnd, m_crChevronBT, m_bMouseOverChevron, m_bMouseDownOnChevron);
	}
	else
	{
		m_chevronRightButton.Draw(&dc, pWnd, m_crChevronBT, m_bMouseOverChevron, m_bMouseDownOnChevron);
	}
}

void CDittoWindow::DrawWindowIcon(CWindowDC &dc, CWnd *pWnd)
{
	//m_windowIcon.Draw(&dc, pWnd, m_crWindowIconBT.left, m_crWindowIconBT.top, false, false);
}

void CDittoWindow::DrawCloseBtn(CWindowDC &dc, CWnd *pWnd)
{
	if(m_bDrawClose == false)
	{
		return;
	}
	
	m_closeButton.Draw(&dc, pWnd, m_crCloseBT, m_bMouseOverClose, m_bMouseDownOnClose);
}

void CDittoWindow::DrawMinimizeBtn(CWindowDC &dc, CWnd *pWnd)
{
	if(m_bDrawMinimize == false)
	{
		return;
	}

	m_minimizeButton.Draw(&dc, pWnd, m_crMinimizeBT, m_bMouseOverMinimize, m_bMouseDownOnMinimize);
}

void CDittoWindow::DrawMaximizeBtn(CWindowDC &dc, CWnd *pWnd)
{
	if(m_bDrawMaximize == false)
	{
		return;
	}

	m_maximizeButton.Draw(&dc, pWnd, m_crMaximizeBT, m_bMouseOverMaximize, m_bMouseDownOnMaximize);
}

int CDittoWindow::DoNcLButtonDown(CWnd *pWnd, UINT nHitTest, CPoint point) 
{
	switch (nHitTest)
	{
	case HTCAPTION:
		m_buttonDownOnCaption = true;
		break;
	default:
		m_buttonDownOnCaption = false;
	}

	int buttonPressed = 0;
	//ReleaseCapture();
	CPoint clPoint(point);
	pWnd->ScreenToClient(&clPoint);

	clPoint.x += m_lLeftBorder;
	clPoint.y += m_lTopBorder;

	if(m_crCloseBT.PtInRect(clPoint))
	{
		m_bMouseDownOnClose = true;
		//InvalidateRect(pWnd->m_hWnd, m_crCloseBT, TRUE);
		//pWnd->InvalidateRect(m_crCloseBT);
		//pWnd->UpdateWindow();
		//DoNcPaint(pWnd);
		RedrawWindow(pWnd->m_hWnd, NULL, NULL, RDW_FRAME | RDW_INVALIDATE);
		buttonPressed = BUTTON_CLOSE;
	}
	else if(m_crChevronBT.PtInRect(clPoint))
	{
		m_bMouseDownOnChevron = true;
		RedrawWindow(pWnd->m_hWnd, NULL, NULL, RDW_FRAME | RDW_INVALIDATE);
		buttonPressed = BUTTON_CHEVRON;
	}
	else if(m_crMinimizeBT.PtInRect(clPoint))
	{
		m_bMouseDownOnMinimize = true;
		RedrawWindow(pWnd->m_hWnd, NULL, NULL, RDW_FRAME | RDW_INVALIDATE);
		buttonPressed = BUTTON_MINIMIZE;
	}
	else if(m_crMaximizeBT.PtInRect(clPoint))
	{
		m_bMouseDownOnMaximize = true;
		RedrawWindow(pWnd->m_hWnd, NULL, NULL, RDW_FRAME | RDW_INVALIDATE);
		buttonPressed = BUTTON_MAXIMIZE;
	}
	else if(m_bMinimized)
	{
		//MinMaxWindow(FORCE_MAX);
	}

	return buttonPressed;
}

long CDittoWindow::DoNcLButtonUp(CWnd *pWnd, UINT nHitTest, CPoint point) 
{
	m_buttonDownOnCaption = false;

	CRect crWindow;
	pWnd->GetWindowRect(crWindow);

	CPoint localPoint(point.x - crWindow.left, point.y - crWindow.top);

	long lRet = 0;
	if(m_bMouseDownOnClose)
	{
		m_bMouseDownOnClose = false;
		m_bMouseOverClose = false;

		RedrawWindow(pWnd->m_hWnd, NULL, NULL, RDW_FRAME | RDW_INVALIDATE);
		
		if(m_crCloseBT.PtInRect(localPoint))
		{
			if(m_sendWMClose)
			{
				pWnd->SendMessage(WM_CLOSE, 0, 0);
			}
			lRet = BUTTON_CLOSE;
		}
	}
	else if(m_bMouseDownOnChevron)
	{
		m_bMouseDownOnChevron = false;
		m_bMouseOverChevron = false;

		RedrawWindow(pWnd->m_hWnd, NULL, NULL, RDW_FRAME | RDW_INVALIDATE);

		if(m_crChevronBT.PtInRect(localPoint))
		{
			lRet = BUTTON_CHEVRON;
		}
	}
	else if(m_bMouseDownOnMinimize)
	{
		m_bMouseDownOnMinimize = false;
		m_bMouseOverMinimize = false;

		RedrawWindow(pWnd->m_hWnd, NULL, NULL, RDW_FRAME | RDW_INVALIDATE);

		if(m_crMinimizeBT.PtInRect(localPoint))
		{
			pWnd->ShowWindow(SW_MINIMIZE);
			lRet = BUTTON_MINIMIZE;
		}
	}
	else if(m_bMouseDownOnMaximize)
	{
		m_bMouseDownOnMaximize = false;
		m_bMouseOverMaximize = false;

		RedrawWindow(pWnd->m_hWnd, NULL, NULL, RDW_FRAME | RDW_INVALIDATE);

		if(m_crMaximizeBT.PtInRect(localPoint))
		{
			if(pWnd->GetStyle() & WS_MAXIMIZE)
				pWnd->ShowWindow(SW_RESTORE);
			else
				pWnd->ShowWindow(SW_SHOWMAXIMIZED);

			lRet = BUTTON_MAXIMIZE;
		}
	}

	return lRet;
}

void CDittoWindow::DoNcMouseMove(CWnd *pWnd, UINT nHitTest, CPoint point) 
{
	return;
	CRect crWindow;
	pWnd->GetWindowRect(crWindow);

	CPoint localPoint(point.x - crWindow.left, point.y - crWindow.top);

	if(m_crCloseBT.PtInRect(localPoint))
	{
		m_bMouseOverClose = true;
		RedrawWindow(pWnd->m_hWnd, NULL, NULL, RDW_FRAME | RDW_INVALIDATE);
	}
	else if(m_bMouseOverClose)
	{
		m_bMouseOverClose = false;
		RedrawWindow(pWnd->m_hWnd, NULL, NULL, RDW_FRAME | RDW_INVALIDATE);
	}

	if(m_crChevronBT.PtInRect(localPoint))
	{
		m_bMouseOverChevron = true;
		RedrawWindow(pWnd->m_hWnd, NULL, NULL, RDW_FRAME | RDW_INVALIDATE);
	}
	else if(m_bMouseOverChevron)
	{
		m_bMouseOverChevron = false;
		RedrawWindow(pWnd->m_hWnd, NULL, NULL, RDW_FRAME | RDW_INVALIDATE);
	}

	if(m_crMinimizeBT.PtInRect(localPoint))
	{
		m_bMouseOverMinimize = true;
		RedrawWindow(pWnd->m_hWnd, NULL, NULL, RDW_FRAME | RDW_INVALIDATE);
	}
	else if(m_bMouseOverMinimize)
	{
		m_bMouseOverMinimize = false;
		RedrawWindow(pWnd->m_hWnd, NULL, NULL, RDW_FRAME | RDW_INVALIDATE);
	}

	if(m_crMaximizeBT.PtInRect(localPoint))
	{
		m_bMouseOverMaximize = true;
		RedrawWindow(pWnd->m_hWnd, NULL, NULL, RDW_FRAME | RDW_INVALIDATE);
	}
	else if(m_bMouseOverMaximize)
	{
		m_bMouseOverMaximize = false;
		RedrawWindow(pWnd->m_hWnd, NULL, NULL, RDW_FRAME | RDW_INVALIDATE);
	}
}

bool CDittoWindow::DoPreTranslateMessage(MSG* pMsg) 
{
	return true;
}

void CDittoWindow::SetCaptionOn(CWnd *pWnd, int nPos, bool bOnstartup, int captionSize, int captionFontSize)
{
	m_VertFont.Detach();
	m_VertFont.CreateFont(theApp.m_metrics.PointsToPixels(captionFontSize), 0, -900, 0, 400, FALSE, FALSE, 0, DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
		DEFAULT_PITCH | FF_SWISS, _T("Segoe UI"));

	m_HorFont.Detach();
	m_HorFont.CreateFont(theApp.m_metrics.PointsToPixels(captionFontSize), 0, 0, 0, 500, FALSE, FALSE, 0, DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
		DEFAULT_PITCH | FF_SWISS, _T("Segoe UI"));

	SetTitleTextHeight(pWnd);

	m_lTopBorder = BORDER;
	m_lRightBorder = BORDER;
	m_lBottomBorder = BORDER;
	m_lLeftBorder = BORDER;

	int oldWidth = m_captionBorderWidth;
	m_captionBorderWidth = theApp.m_metrics.ScaleX(captionSize);	

	if(nPos == CAPTION_RIGHT)
		m_lRightBorder = m_captionBorderWidth;
	if(nPos == CAPTION_BOTTOM)
		m_lBottomBorder = m_captionBorderWidth;
	if(nPos == CAPTION_LEFT)
		m_lLeftBorder = m_captionBorderWidth;
	if(nPos == CAPTION_TOP)
		m_lTopBorder = m_captionBorderWidth;

	DoSetRegion(pWnd);

	if(!bOnstartup)
	{
		pWnd->SetWindowPos(NULL, 0, 0, 0, 0, SWP_FRAMECHANGED|SWP_NOMOVE|SWP_NOSIZE|SWP_NOZORDER);
	}

	pWnd->Invalidate();
	pWnd->RedrawWindow();

	if (oldWidth != m_captionBorderWidth)
	{
		::SetWindowPos(pWnd->m_hWnd, NULL, 0, 0, 0, 0, SWP_DRAWFRAME | SWP_NOSIZE | SWP_NOMOVE | SWP_NOACTIVATE);
	}
}

void CDittoWindow::SetTitleTextHeight(CWnd *pWnd)
{
	CWindowDC dc(pWnd);
	CFont *pOldFont = dc.SelectObject(&m_HorFont);
	CRect size(0, 0, 0, 0);
	dc.DrawText(_T("W"), size, DT_CALCRECT);
	m_titleTextHeight = size.Height();
	dc.SelectObject(pOldFont);
}

bool CDittoWindow::SetCaptionColors(COLORREF left, COLORREF right, COLORREF border)
{
	m_CaptionColorLeft = left;
	m_CaptionColorRight = right;
	m_border = border;

	return true;
}

void CDittoWindow::SetCaptionTextColor(COLORREF color)
{
	m_CaptionTextColor = color;
}

void CDittoWindow::SnapToEdge(CWnd *pWnd, WINDOWPOS* lpwndpos)
{
	if (lpwndpos->cx == 0 &&
		lpwndpos->cy == 0)
	{
		return;
	}

	const char threshold = 12;
	RECT rect = { 0 };
	HMONITOR hMonitor;
	MONITORINFO mi;

	// Grab information about our monitors
	// For multi-monitor support, we use this instead of SystemParametersInfo(SPI_GETWORKAREA, 0, &rect, 0);
	hMonitor = MonitorFromWindow(pWnd->m_hWnd, MONITOR_DEFAULTTONEAREST);
	mi.cbSize = sizeof(mi);
	GetMonitorInfo(hMonitor, &mi);
	rect = mi.rcWork;

	bool edgeMove = true;
	bool captionMove = false;

	if (m_buttonDownOnCaption)
	{
		edgeMove = false;
		captionMove = true;
	}

	// Snap to left
	if (lpwndpos->x >= (rect.left - threshold) &&
		lpwndpos->x <= (rect.left + threshold))
	{
		if (edgeMove)
		{
			int diff = lpwndpos->x - rect.left;
			lpwndpos->cx += diff;
		}
		if (edgeMove || captionMove)
		{
			lpwndpos->x = rect.left;
		}
	}

	// Snap to right
	if ((lpwndpos->x + lpwndpos->cx) >= (rect.right - threshold) &&
		(lpwndpos->x + lpwndpos->cx) <= (rect.right + threshold))
	{
		if (edgeMove)
		{
			int diff = rect.right - (lpwndpos->x + lpwndpos->cx);
			lpwndpos->cx += diff;
		}
		if (captionMove)
		{
			lpwndpos->x = (rect.right - lpwndpos->cx);
		}
	}

	// Snap to top
	if (lpwndpos->y >= (rect.top - threshold) &&
		lpwndpos->y <= (rect.top + threshold))
	{
		if (edgeMove)
		{
			int diff = lpwndpos->y - rect.top;
			lpwndpos->cy += diff;
		}
		if (edgeMove || captionMove)
		{
			lpwndpos->y = rect.top;
		}
	}

	// Snap to bottom
	if ((lpwndpos->y + lpwndpos->cy) >= (rect.bottom - threshold) &&
		(lpwndpos->y + lpwndpos->cy) <= (rect.bottom + threshold))
	{
		if (edgeMove)
		{
			int diff = rect.bottom - (lpwndpos->y + lpwndpos->cy);
			lpwndpos->cy += diff;
		}
		if (captionMove)
		{
			lpwndpos->y = (rect.bottom - lpwndpos->cy);
		}
	}
}

void CDittoWindow::MinMaxWindow(CWnd *pWnd, long lOption)
{
	if ((m_bMinimized) && (lOption == FORCE_MIN))
		return;

	if ((m_bMinimized == false) && (lOption == FORCE_MAX))
		return;

	if (m_lRightBorder == m_captionBorderWidth)
	{
		if (m_bMinimized == false)
		{
			pWnd->GetWindowRect(m_crFullSizeWindow);
			pWnd->MoveWindow(m_crFullSizeWindow.right - m_captionBorderWidth,
				m_crFullSizeWindow.top, m_captionBorderWidth,
				m_crFullSizeWindow.Height());
			m_bMinimized = true;
			m_TimeMinimized = COleDateTime::GetCurrentTime();
		}
		else
		{
			CRect cr;
			pWnd->GetWindowRect(cr);
			pWnd->MoveWindow(cr.right - m_crFullSizeWindow.Width(),
				cr.top, m_crFullSizeWindow.Width(), cr.Height());

			m_crFullSizeWindow.SetRectEmpty();
			m_bMinimized = false;
			m_TimeMaximized = COleDateTime::GetCurrentTime();
			::SetForegroundWindow(pWnd->GetSafeHwnd());
		}
	}
	if (m_lLeftBorder == m_captionBorderWidth)
	{
		if (m_bMinimized == false)
		{
			pWnd->GetWindowRect(m_crFullSizeWindow);
			pWnd->MoveWindow(m_crFullSizeWindow.left,
				m_crFullSizeWindow.top, m_captionBorderWidth,
				m_crFullSizeWindow.Height());
			m_bMinimized = true;
			m_TimeMinimized = COleDateTime::GetCurrentTime();
		}
		else
		{
			CRect cr;
			pWnd->GetWindowRect(cr);
			pWnd->MoveWindow(cr.left, cr.top,
				m_crFullSizeWindow.Width(), cr.Height());

			m_crFullSizeWindow.SetRectEmpty();
			m_bMinimized = false;
			m_TimeMaximized = COleDateTime::GetCurrentTime();
			::SetForegroundWindow(pWnd->GetSafeHwnd());
		}
	}
	else if (m_lTopBorder == m_captionBorderWidth)
	{
		if (m_bMinimized == false)
		{
			pWnd->GetWindowRect(m_crFullSizeWindow);
			pWnd->MoveWindow(m_crFullSizeWindow.left,
				m_crFullSizeWindow.top,
				m_crFullSizeWindow.Width(),
				m_captionBorderWidth);
			m_bMinimized = true;
			m_TimeMinimized = COleDateTime::GetCurrentTime();
		}
		else
		{
			CRect cr;
			pWnd->GetWindowRect(cr);
			pWnd->MoveWindow(cr.left, cr.top,
				cr.Width(), m_crFullSizeWindow.Height());

			m_crFullSizeWindow.SetRectEmpty();
			m_bMinimized = false;
			m_TimeMaximized = COleDateTime::GetCurrentTime();
			::SetForegroundWindow(pWnd->GetSafeHwnd());
		}
	}
	else if (m_lBottomBorder == m_captionBorderWidth)
	{
		if (m_bMinimized == false)
		{
			pWnd->GetWindowRect(m_crFullSizeWindow);
			pWnd->MoveWindow(m_crFullSizeWindow.left,
				m_crFullSizeWindow.bottom - m_captionBorderWidth,
				m_crFullSizeWindow.Width(),
				m_captionBorderWidth);
			m_bMinimized = true;
			m_TimeMinimized = COleDateTime::GetCurrentTime();
		}
		else
		{
			CRect cr;
			pWnd->GetWindowRect(cr);
			pWnd->MoveWindow(cr.left,
				cr.bottom - m_crFullSizeWindow.Height(),
				cr.Width(), m_crFullSizeWindow.Height());

			m_crFullSizeWindow.SetRectEmpty();
			m_bMinimized = false;
			m_TimeMaximized = COleDateTime::GetCurrentTime();
			::SetForegroundWindow(pWnd->GetSafeHwnd());
		}
	}
}