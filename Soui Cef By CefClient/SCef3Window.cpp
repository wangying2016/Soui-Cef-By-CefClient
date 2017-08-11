#include "stdafx.h"
#include "SCef3Window.h"
#include "Cef3Loader.h"

namespace SOUI
{

SCef3Window::SCef3Window()
{
	SASSERT(SOUI::Cef3Loader::GetInstance());

	m_pBrowser = new Browser;
	m_pBrowser->AddRef();
	m_pBrowser->setHost(this);
}

SCef3Window::~SCef3Window()
{
	m_pBrowser->Release();
	m_pBrowser = NULL;
}

int SCef3Window::OnCreate(LPVOID)
{
	bool bOK = m_pBrowser->open();
	if (!bOK) return -1;
	if (m_strUrl.IsEmpty() == FALSE)
		m_pBrowser->loadURL(m_strUrl);
	return 0;
}

void SCef3Window::OnSize(UINT nType, CSize size)
{
	SWindow::OnSize(nType, size);
	m_pBrowser->wasResized(size.cx, size.cy);
}

BOOL SCef3Window::OnAttrUrl(SStringW strValue, BOOL bLoading)
{
	m_strUrl = strValue;
	if (!bLoading) m_pBrowser->loadURL(m_strUrl);
	return !bLoading;
}

void SCef3Window::OnDestroy()
{
	if (m_pBrowser) m_pBrowser->close();
	SWindow::OnDestroy();
}

void SCef3Window::OnPaint(IRenderTarget *pRT)
{
	if (m_pBrowser == NULL) return;
	if (m_pBrowser->pixmap_) {
		CRect rcWnd = GetClientRect();
		pRT->DrawBitmap(rcWnd, m_pBrowser->pixmap_, 0, 0, 0xFF);
	}
}

LRESULT SCef3Window::OnMouseMove(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	m_pBrowser->sendMouseMoveEvent(uMsg, wParam, lParam);
	return 0;
}

LRESULT SCef3Window::OnMouseWheel(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	m_pBrowser->sendMouseWheelEvent(uMsg, wParam, lParam);
	return 0;
}

LRESULT SCef3Window::OnMouseEvent(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (uMsg == WM_LBUTTONDOWN)
	{
		SetCapture();
	} else if (uMsg == WM_LBUTTONUP)
	{
		ReleaseCapture();
	}
	m_pBrowser->sendMouseClickEvent(uMsg, wParam, lParam);
	return 0;
}

LRESULT SCef3Window::OnKeyEvent(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	m_pBrowser->sendKeyEvent(uMsg, wParam, lParam);
	return 0;
}

void SCef3Window::OnSetFocus(SWND wndOld)
{
	m_pBrowser->sendFocusEvent(true);
}

void SCef3Window::OnKillFocus(SWND wndFocus)
{
	m_pBrowser->sendFocusEvent(false);
}

LRESULT SCef3Window::OnImeStartComposition(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	return 0;
}

BOOL SCef3Window::OnSetCursor(const CPoint &pt)
{
	return TRUE;
}

}