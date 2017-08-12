#include "stdafx.h"
#include "Browser.h"

namespace SOUI
{
#define CHECK_CEF_BROWSER_HOST					\
	if (!browser_)								\
	return;										\
	CefRefPtr<CefBrowserHost> host =			\
	browser_->GetHost();						\
	if (!host)									\
	return										\

Browser::Browser() : skip_cursor_(true), 
                     web_view_(NULL),
	                 m_pMsgHandler(NULL)
{

}

Browser::~Browser()
{

}

void Browser::setHost(SOUI::SWindow *wv)
{
	web_view_ = wv;
}

bool Browser::open()
{
	CefWindowInfo info;
	info.SetAsWindowless(NULL);

	info.x = 0;
	info.y = 0;
	CRect rcView;
	if (web_view_)
		rcView = web_view_->GetClientRect();
	info.width = rcView.Width();
	info.height = rcView.Height();
	browser_ = CefBrowserHost::CreateBrowserSync(info,
												 this,
									             "",
		                                         CefBrowserSettings(),
		                                         NULL);
	return !!browser_;
}

void Browser::close()
{
	setHost(NULL);
	if (browser_)
		browser_->GetHost()->CloseBrowser(true);
	browser_ = NULL;
}

void Browser::loadURL(const wchar_t *url)
{
	if (!url)
		return;
	CefRefPtr<CefFrame> frame = mainFrame();
	if (!frame)
		return;
	frame->LoadURL(url);
}

void Browser::executeJS(const wchar_t *code, const wchar_t *url, int start_line)
{
	if (!code)
		return;
	CefRefPtr<CefFrame> frame = mainFrame();
	if (!frame)
		return;
	frame->ExecuteJavaScript(code, url, start_line);
}

void Browser::wasResized(int width, int height)
{
	if (!browser_)
		return;
	if (!pixmap_ || (pixmap_->Width() != width || pixmap_->Height() != height))
		browser_->GetHost()->WasResized();
}

uint32_t Browser::GetCefModifiers(UINT uMsg)
{
	BYTE keyState[256];
	GetKeyboardState(keyState);
	uint32_t modifiers = EVENTFLAG_NONE;
	if (keyState[VK_CAPITAL] & 0x80)
		modifiers |= EVENTFLAG_CAPS_LOCK_ON;
	if (keyState[VK_SHIFT]   & 0x80)
		modifiers |= EVENTFLAG_SHIFT_DOWN;
	if (keyState[VK_CONTROL] & 0x80)
		modifiers |= EVENTFLAG_CONTROL_DOWN;
	if (keyState[VK_MENU]    & 0x80)
		modifiers |= EVENTFLAG_ALT_DOWN;
	if (keyState[VK_NUMLOCK] & 0x80)
		modifiers |= EVENTFLAG_NUM_LOCK_ON;
	switch (uMsg)
	{
	case WM_LBUTTONDOWN:
		modifiers |= EVENTFLAG_LEFT_MOUSE_BUTTON;
		break;
	case WM_MBUTTONDOWN:
		modifiers |= EVENTFLAG_MIDDLE_MOUSE_BUTTON;
		break;
	case WM_RBUTTONDOWN:
		modifiers |= EVENTFLAG_RIGHT_MOUSE_BUTTON;
		break;
	}
	return modifiers;
}

CefKeyEvent Browser::ToCefKeyEvent(UINT message, WPARAM wParam, LPARAM lParam)
{
	CefKeyEvent event;
	event.windows_key_code = wParam;
	event.native_key_code = lParam;
	event.is_system_key = message == WM_SYSCHAR    ||	
						  message == WM_SYSKEYDOWN ||
		                  message == WM_SYSKEYUP;

	if (message == WM_KEYDOWN || message == WM_SYSKEYDOWN)
		event.type = KEYEVENT_RAWKEYDOWN;
	else if (message == WM_KEYUP || message == WM_SYSKEYUP)
		event.type = KEYEVENT_KEYUP;
	else
		event.type = KEYEVENT_CHAR;
	event.modifiers = GetCefKeyboardModifiers(wParam, lParam);

	return event;
}

bool Browser::isKeyDown(WPARAM wParam)
{
	return (GetKeyState(wParam) & 0x8000) != 0;
}

int Browser::GetCefMouseModifiers(WPARAM wParam)
{
	int modifiers = 0;
	if (wParam & MK_CONTROL)
		modifiers |= EVENTFLAG_CONTROL_DOWN;
	if (wParam & MK_SHIFT)
		modifiers |= EVENTFLAG_SHIFT_DOWN;
	if (isKeyDown(VK_MENU))
		modifiers |= EVENTFLAG_ALT_DOWN;
	if (wParam & MK_LBUTTON)
		modifiers |= EVENTFLAG_LEFT_MOUSE_BUTTON;
	if (wParam & MK_MBUTTON)
		modifiers |= EVENTFLAG_MIDDLE_MOUSE_BUTTON;
	if (wParam & MK_RBUTTON)
		modifiers |= EVENTFLAG_RIGHT_MOUSE_BUTTON;

	if (::GetKeyState(VK_NUMLOCK) & 1)
		modifiers |= EVENTFLAG_NUM_LOCK_ON;
	if (::GetKeyState(VK_CAPITAL) & 1)
		modifiers |= EVENTFLAG_CAPS_LOCK_ON;
	return modifiers;
}

int Browser::GetCefKeyboardModifiers(WPARAM wParam, LPARAM lParam)
{
	int modifiers = 0;
	if (isKeyDown(VK_SHIFT))
		modifiers |= EVENTFLAG_SHIFT_DOWN;
	if (isKeyDown(VK_CONTROL))
		modifiers |= EVENTFLAG_CONTROL_DOWN;
	if (isKeyDown(VK_MENU))
		modifiers |= EVENTFLAG_ALT_DOWN;

	if (::GetKeyState(VK_NUMLOCK) & 1)
		modifiers |= EVENTFLAG_NUM_LOCK_ON;
	if (::GetKeyState(VK_CAPITAL) & 1)
		modifiers |= EVENTFLAG_CAPS_LOCK_ON;

	switch (wParam)
	{
	case VK_RETURN:
		if ((lParam >> 16) & KF_EXTENDED)
			modifiers |= EVENTFLAG_IS_KEY_PAD;
		break;
	case VK_INSERT:
	case VK_DELETE:
	case VK_HOME:
	case VK_END:
	case VK_PRIOR:
	case VK_NEXT:
	case VK_UP:
	case VK_DOWN:
	case VK_LEFT:
	case VK_RIGHT:
		if (!((lParam >> 16) & KF_EXTENDED))
			modifiers |= EVENTFLAG_IS_KEY_PAD;
		break;
	case VK_NUMLOCK:
	case VK_NUMPAD0:
	case VK_NUMPAD1:
	case VK_NUMPAD2:
	case VK_NUMPAD3:
	case VK_NUMPAD4:
	case VK_NUMPAD5:
	case VK_NUMPAD6:
	case VK_NUMPAD7:
	case VK_NUMPAD8:
	case VK_NUMPAD9:
	case VK_DIVIDE:
	case VK_MULTIPLY:
	case VK_SUBTRACT:
	case VK_ADD:
	case VK_DECIMAL:
	case VK_CLEAR:
		modifiers |= EVENTFLAG_IS_KEY_PAD;
		break;
	case VK_SHIFT:
		if (isKeyDown(VK_LSHIFT))
			modifiers |= EVENTFLAG_IS_LEFT;
		else if (isKeyDown(VK_RSHIFT))
			modifiers |= EVENTFLAG_IS_RIGHT;
		break;
	case VK_CONTROL:
		if (isKeyDown(VK_LCONTROL))
			modifiers |= EVENTFLAG_IS_LEFT;
		else if (isKeyDown(VK_RCONTROL))
			modifiers |= EVENTFLAG_IS_RIGHT;
		break;
	case VK_MENU:
		if (isKeyDown(VK_LMENU))
			modifiers |= EVENTFLAG_IS_LEFT;
		else if (isKeyDown(VK_RMENU))
			modifiers |= EVENTFLAG_IS_RIGHT;
		break;
	case VK_LWIN:
		modifiers |= EVENTFLAG_IS_LEFT;
		break;
	case VK_RWIN:
		modifiers |= EVENTFLAG_IS_RIGHT;
		break;
	}
	return modifiers;
}

void Browser::sendMouseMoveEvent(UINT uMsg, WPARAM wp, LPARAM lp)
{
	// cef cursor is asynchronously, maybe our cursor leave from webivew,
	// event webview send mouse event, this event should be ignored.
	CHECK_CEF_BROWSER_HOST;
	CPoint pt(GET_X_LPARAM(lp), GET_Y_LPARAM(lp));
	CRect rcView = web_view_->GetClientRect();
	bool leave = !rcView.PtInRect(pt) && web_view_->GetCapture() != web_view_->GetSwnd();
	if (leave)
		skip_cursor_ = true;
	else
		skip_cursor_ = false;

	CefMouseEvent evt;
	evt.x = pt.x - rcView.left;
	evt.y = pt.y - rcView.top;
	evt.modifiers = GetCefMouseModifiers(wp);
	host->SendMouseMoveEvent(evt, leave);
}

void Browser::sendMouseClickEvent(UINT uMsg, WPARAM wp, LPARAM lp)
{
	CHECK_CEF_BROWSER_HOST;
	CefMouseEvent evt;
	CefBrowserHost::MouseButtonType mt = MBT_LEFT;
	bool bMouseUp = false;
	int  nClickCount = 1;
	switch (uMsg)
	{
	case WM_LBUTTONUP:
		bMouseUp = true;
	case WM_LBUTTONDOWN:
		mt = MBT_LEFT;
		break;
	case WM_LBUTTONDBLCLK:
		nClickCount = 2;
		break;
	case WM_MBUTTONUP:
		bMouseUp = true;
	case WM_MBUTTONDOWN:
		mt = MBT_MIDDLE;
		break;
	case WM_RBUTTONUP:
		bMouseUp = true;
	case WM_RBUTTONDOWN:
		mt = MBT_RIGHT;
		break;
	}
	CRect rcClient = web_view_->GetClientRect();
	CPoint pt(GET_X_LPARAM(lp), GET_Y_LPARAM(lp));
	evt.x = ptMouse_.x = pt.x - rcClient.left;
	evt.y = ptMouse_.y = pt.y - rcClient.top;
	evt.modifiers = GetCefMouseModifiers(wp);
	host->SendMouseClickEvent(evt, mt, bMouseUp, nClickCount);
}

void Browser::sendFocusEvent(bool focus)
{
	CHECK_CEF_BROWSER_HOST;
	host->SendFocusEvent(focus);
}

void Browser::sendMouseWheelEvent(UINT uMsg, WPARAM wp, LPARAM lp)
{
	CHECK_CEF_BROWSER_HOST;

	POINT pt;
	pt.x = GET_X_LPARAM(lp);
	pt.y = GET_Y_LPARAM(lp);

	int delta = GET_WHEEL_DELTA_WPARAM(wp);

	CefMouseEvent evt;
	CRect rcView = web_view_->GetClientRect();
	pt.x -= rcView.left;
	pt.y -= rcView.top;
	evt.x = pt.x;
	evt.y = pt.y;
	evt.modifiers = 0;
	host->SendMouseWheelEvent(evt, isKeyDown(VK_SHIFT) ? delta : 0, isKeyDown(VK_SHIFT) ? 0 : delta);
}

void Browser::sendKeyEvent(UINT uMsg, WPARAM wp, LPARAM lp)
{
	CHECK_CEF_BROWSER_HOST;
	CefKeyEvent evt = ToCefKeyEvent(uMsg, wp, lp);
	host->SendKeyEvent(evt);
}

CRect Browser::getCaret()
{
	CRect caret;
	CefRefPtr<CefFrame> focusFrame = browser_->GetFocusedFrame();
	if (focusFrame) {
		focusFrame->SelectAll();
	}
	return caret;
}

void Browser::OnBeforeClose(CefRefPtr<CefBrowser> browser)
{
	
}

void Browser::RegisterMessageHandler(MessageHandler * handler)
{
	m_pMsgHandler = handler;
}

void Browser::UnRegisterMessgeHandler()
{
	m_pMsgHandler = NULL;
}

bool Browser::OnSetFocus(CefRefPtr<CefBrowser> browser, FocusSource source)
{
	web_view_ ? web_view_->SetFocus() : 0;
	return true;
}

bool Browser::GetViewRect(CefRefPtr<CefBrowser> browser, CefRect& rect)
{
	rect.x = rect.y = 0;
	CRect rcView;
	if (web_view_)
		rcView = web_view_->GetClientRect();
	rect.width = rcView.Width();
	rect.height = rcView.Height();
	return true;
}

void Browser::OnPaint(CefRefPtr<CefBrowser> browser, 
	                  PaintElementType type, 
	                  const RectList& dirtyRects, 
	                  const void* buffer, int width, 
	                  int height) {
	if (type != PET_VIEW) return;
	adjustPixmap(width, height);
	LPBYTE pDst = (LPBYTE)pixmap_->LockPixelBits();
	if (pDst) {
		memcpy(pDst, buffer, width * height * 4);
		pixmap_->UnlockPixelBits(pDst);
		web_view_ ? web_view_->Invalidate() : 0;
	}
}

void Browser::OnCursorChange(CefRefPtr<CefBrowser> browser, 
	                         CefCursorHandle cursor,
	                         CursorType type, 
	                         const CefCursorInfo& custom_cursor_info) {
	if (!skip_cursor_) ::SetCursor(cursor);
}

void Browser::OnTitleChange(CefRefPtr<CefBrowser> browser,
	                        const CefString& title)
{
	// TODO: Set Browser Title
}

void Browser::adjustPixmap(int width, int height)
{
	if (!pixmap_)
		GETRENDERFACTORY->CreateBitmap(&pixmap_);
	if (pixmap_->Width() != width || pixmap_->Height() != height)
		pixmap_->Init(width, height);
}

}