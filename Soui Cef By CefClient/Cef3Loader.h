#pragma once

// Cef3Loader: Manage Cef3 work steps.

namespace SOUI
{
	class Cef3Loader
	{
	public:
		static void Initialize();
		static void DoMessageLoop();
		static void ShutDown();
		static bool bInitialized;
	};
}