#include "stdafx.h"
#include "Cef3Loader.h"
#include "include/cef_app.h"

namespace SOUI
{
	bool Cef3Loader::bInitialized = false;

	void Cef3Loader::Initialize()
	{
		SASSERT(!bInitialized);
		CefMainArgs args(GetModuleHandle(NULL));

		// cef settings
		CefSettings settings;
		settings.no_sandbox = true;
		settings.multi_threaded_message_loop = false;

		// cef sub process path
		SStringW strAppPath;
		strAppPath = _T("cefclient.exe");
		CefString(&settings.browser_subprocess_path) = strAppPath;
		settings.windowless_rendering_enabled = true;
		settings.single_process = false;

		// cef locate 
		CefString(&settings.locale) = "zh-CN";

		// cef app
		/*CefRefPtr<ClientApp> app(new ClientApp);*/

		// cef run multi processes
		/*int exit_code = CefExecuteProcess(args, app.get(), NULL);
		if (exit_code >= 0)
			return;*/

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