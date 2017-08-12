#pragma once

#include "include/cef_app.h"

class ClientApp : public CefApp
{
public:
	ClientApp();
	virtual ~ClientApp();

	virtual void OnBeforeCommandLineProcessing(
		const CefString& process_type,
		CefRefPtr<CefCommandLine> command_line) OVERRIDE;

private:
	IMPLEMENT_REFCOUNTING(ClientApp);
};