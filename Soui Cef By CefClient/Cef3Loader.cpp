#include "stdafx.h"
#include "Cef3Loader.h"
#include "include/cef_app.h"


namespace SOUI
{
	Cef3Loader *Cef3Loader::s_Inst = NULL;

	Cef3Loader::Cef3Loader(const char *locate)
	{
		SASSERT(s_Inst == NULL);

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
		if (!locate)
			locate = "zh-CN";
		CefString(&settings.locale) = locate;

		// cef initialize
		BOOL bOK = CefInitialize(args, settings, NULL, NULL);
		SASSERT(bOK);
		s_Inst = this;
	}

	Cef3Loader* Cef3Loader::GetInstance()
	{
		return s_Inst;
	}

	Cef3Loader::~Cef3Loader()
	{
		s_Inst = NULL;
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