#pragma once

// Cef3Loader: Manage Cef3 work steps.

namespace SOUI
{
	class Cef3Loader
	{
	public:
		Cef3Loader(const char *locate);

		// get Cef3Loader pointer
		static Cef3Loader *GetInstance();

		virtual ~Cef3Loader();
		void DoMessageLoop();

	protected:
		static Cef3Loader *s_Inst;
	};
}