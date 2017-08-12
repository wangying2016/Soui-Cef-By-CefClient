#pragma once
#include "stdafx.h"
#include "include/cef_app.h"
#include "include/cef_client.h"

namespace SOUI {

// Post javascript messsage to ui
class MessageHandler {
public:
	virtual bool OnBrowserMessage(CefRefPtr<CefBrowser> browser,
			                      CefProcessId source_process,
			                      CefRefPtr<CefProcessMessage> message) = 0;
};

class Browser : public CefClient,
				public CefLifeSpanHandler,
				public CefRenderHandler,
				public CefFocusHandler,
				public CefDisplayHandler
{
public:
	Browser();
	virtual ~Browser();

	// user methods
	void setHost(SOUI::SWindow *wv);
	bool open();
	void close();
	void loadURL(const wchar_t *url);
	void executeJS(const wchar_t *code, const wchar_t *url, int start_line);

	// browser resized
	void wasResized(int width, int height);

	// mouse event
	uint32_t GetCefModifiers(UINT uMsg);
	CefKeyEvent ToCefKeyEvent(UINT message, WPARAM wParam, LPARAM lParam);
	bool isKeyDown(WPARAM wParam);
	int GetCefMouseModifiers(WPARAM wParam);
	int GetCefKeyboardModifiers(WPARAM wParam, LPARAM lParam);
	void sendMouseMoveEvent(UINT uMsg, WPARAM wp, LPARAM lp);
	void sendMouseClickEvent(UINT uMsg, WPARAM wp, LPARAM lp);
	void sendMouseWheelEvent(UINT uMsg, WPARAM wp, LPARAM lp);
	void sendFocusEvent(bool focus);
	void sendKeyEvent(UINT uMsg, WPARAM wp, LPARAM lp);
	CRect getCaret();

	// client methods
	virtual CefRefPtr<CefLifeSpanHandler> GetLifeSpanHandler() OVERRIDE { return this; }
	virtual CefRefPtr<CefRenderHandler> GetRenderHandler()     OVERRIDE { return this; }
	virtual CefRefPtr<CefFocusHandler> GetFocusHandler()       OVERRIDE { return this; }
	virtual CefRefPtr<CefDisplayHandler> GetDisplayHandler()   OVERRIDE { return this; }
	virtual bool OnProcessMessageReceived(CefRefPtr<CefBrowser> browser,
		                                  CefProcessId source_process,
		                                  CefRefPtr<CefProcessMessage> message) {
		if (m_pMsgHandler)
			return m_pMsgHandler->OnBrowserMessage(browser, source_process, message);
	}

	// Post javascript message to ui
	void RegisterMessageHandler(MessageHandler * handler);
	void UnRegisterMessgeHandler();

	// CefFocusHandler methods
	virtual bool OnSetFocus(CefRefPtr<CefBrowser> browser, FocusSource source) OVERRIDE;


	// CefLifeSpanHandler methods:
	virtual void OnAfterCreated(CefRefPtr<CefBrowser> browser) OVERRIDE {
		browser_ = browser;
	}
	virtual bool DoClose(CefRefPtr<CefBrowser> browser) OVERRIDE {
		return false;
	}
	virtual void OnBeforeClose(CefRefPtr<CefBrowser> browser) OVERRIDE;
	virtual bool OnBeforePopup(CefRefPtr<CefBrowser> browser,
							   CefRefPtr<CefFrame> frame,
		                       const CefString& target_url,
		                       const CefString& target_frame_name,
		                       WindowOpenDisposition target_disposition,
		                       bool user_gesture,
		                       const CefPopupFeatures& popupFeatures,
		                       CefWindowInfo& windowInfo,
		                       CefRefPtr<CefClient>& client,
		                       CefBrowserSettings& settings,
							   bool* no_javascript_access) OVERRIDE {
		return false;
	}

	// CefRenderHandler methods
	virtual bool GetViewRect(CefRefPtr<CefBrowser> browser, CefRect& rect) OVERRIDE;
	virtual void OnPaint(CefRefPtr<CefBrowser> browser,
			             PaintElementType type,
		                 const RectList& dirtyRects,
		                 const void* buffer,
		                 int width,
		                 int height) OVERRIDE;
	virtual void OnCursorChange(CefRefPtr<CefBrowser> browser,
		                        CefCursorHandle cursor,
		                        CursorType type,
		                        const CefCursorInfo& custom_cursor_info) OVERRIDE;
	// CefDisplayHandler methods
	virtual void OnTitleChange(CefRefPtr<CefBrowser> browser,
		                       const CefString& title) OVERRIDE; 

protected:
	// get main frame
	CefRefPtr<CefFrame> mainFrame() {
		if (!browser_) return NULL;
		return browser_->GetMainFrame();
	}

private:
	// adjust pix map
	void adjustPixmap(int width, int height);

private:
	CefRefPtr<CefBrowser> browser_;
	SOUI::SWindow *web_view_;
	CAutoRefPtr<IBitmap> pixmap_;
	CPoint ptMouse_;
	bool skip_cursor_;
	MessageHandler *m_pMsgHandler;
	IMPLEMENT_REFCOUNTING(Browser);
	friend class SCef3Window;
};

}