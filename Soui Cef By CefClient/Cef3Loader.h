#pragma once

// Cef3Loader: Manage Cef3 work steps.

namespace SOUI
{
	class Cef3Loader
	{
	public:
		static Cef3Loader *GetInstance();
		
		Cef3Loader(const char *locate);
		virtual ~Cef3Loader();

		void DoMessageLoop();
		void ShutDown();

	protected:
		static Cef3Loader *s_Inst;
	};
}