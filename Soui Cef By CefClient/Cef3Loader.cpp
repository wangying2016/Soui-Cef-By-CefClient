#include "stdafx.h"
#include "Cef3Loader.h"
#include "include/cef_app.h"

namespace SOUI
{
	bool Cef3Loader::bInitialized = false;

	void Cef3Loader::Initialize()
	{
		SASSERT(!bInitialized);
		CefMainArgs args;

		// cef settings
		CefSettings settings;
		settings.no_sandbox = true;
		settings.multi_threaded_message_loop = false;

		// cef sub process path
		SStringW strAppPath;
		strAppPath = _T("cefclient.exe");
		CefString(&settings.browser_subprocess_path) = strAppPath;

		// cef locate 
		CefString(&settings.locale) = "zh-CN";

		// cef initialize
		BOOL bOK = CefInitialize(args, settings, NULL, NULL);
		bInitialized = true;
	}

	void Cef3Loader::DoMessageLoop()
	{
		CefDoMessageLoopWork();
	}

	void Cef3Loader::ShutDown()
	{
		CefShutdown();
	}
}