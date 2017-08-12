#include "stdafx.h"
#include "SCef3Window.h"
#include "../Cef3Loader.h"

namespace SOUI
{

SCef3Window::SCef3Window()
{
	SASSERT(Cef3Loader::bInitialized);
	m_evtSet.addEvent(EVENTID(EventWebViewNotify));
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
	m_pBrowser->RegisterMessageHandler(this);
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
	if (m_pBrowser) {
		m_pBrowser->close();
		m_pBrowser->UnRegisterMessgeHandler();
	}
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

void SCef3Window::LoadURL(SStringW strURL)
{
	if (strURL.GetLength() != 0)
		m_strUrl = strURL;
	if (m_pBrowser)
		m_pBrowser->loadURL(m_strUrl);
}

void SCef3Window::ExecuteJS(SStringW strValue)
{
	if (m_pBrowser && strValue.GetLength() != 0)
		m_pBrowser->executeJS(strValue, m_strUrl, 0);
}

bool SCef3Window::OnBrowserMessage(
	CefRefPtr<CefBrowser> browser,
	CefProcessId source_process,
	CefRefPtr<CefProcessMessage> message)
{
	EventWebViewNotify evt(this);

	evt.MessageName = message->GetName().ToWString().c_str();
	CefRefPtr<CefListValue> arg = message->GetArgumentList();

	for (size_t i = 0; i < arg->GetSize(); ++i) {
		SStringW str = arg->GetString(i).ToWString().c_str();
		evt.Arguments.Add(str);
	}
	return !!FireEvent(evt);
}

}