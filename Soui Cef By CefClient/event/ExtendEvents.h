#pragma once
#include <souistd.h>

namespace SOUI
{
#define EVT_CEFWEBVIEW_BEGIN (EVT_EXTERNAL_BEGIN + 900)
#define EVT_WEBVIEW_NOTIFY   (EVT_CEFWEBVIEW_BEGIN + 0)

class EventWebViewNotify : public TplEventArgs<EventWebViewNotify>
{
	SOUI_CLASS_NAME(EventWebViewNotify, L"on_webview_notify")
public:
	EventWebViewNotify(SObject *pSender) : TplEventArgs<EventWebViewNotify>(pSender) {}
	enum { EventID = EVT_WEBVIEW_NOTIFY };

	SStringW MessageName;
	SArray<SStringW> Arguments;
};

}