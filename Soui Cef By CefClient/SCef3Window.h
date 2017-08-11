#pragma once

namespace SOUI
{

class SCef3Window : public SWindow
{
	SOUI_CLASS_NAME(SCef3Window, L"cef3")
public:
	SCef3Window();
	virtual ~SCef3Window();


	virtual BOOL OnSetCursor(const CPoint &pt) OVERRIDE;



protected:
	BOOL OnAttrUrl(SStringW strValue, BOOL bLoading);
	SOUI_ATTRS_BEGIN()
		ATTR_CUSTOM(L"url", OnAttrUrl)
	SOUI_ATTRS_END()

	int OnCreate(LPVOID);
	void OnDestroy();
	void OnSize(UINT nType, CSize size);
	void OnPaint(IRenderTarget *pRT);
	void OnSetFocus();
	void OnKillFocus();

	LRESULT OnMouseMove(UINT uMsg, WPARAM wParam, LPARAM lParam);
	LRESULT OnMouseWheel(UINT uMsg, WPARAM wParam, LPARAM lParam);
	LRESULT OnMouseEvent(UINT uMsg, WPARAM wParam, LPARAM lParam);
	LRESULT OnKeyEvent(UINT uMsg, WPARAM wParam, LPARAM lParam);
	LRESULT OnImeStartComposition(UINT uMsg, WPARAM wParam, LPARAM lParam);

	SOUI_MSG_MAP_BEGIN()
		MSG_WM_CREATE(OnCreate)
		MSG_WM_DESTROY(OnDestroy)
		MSG_WM_SIZE(OnSize)
		MSG_WM_PAINT_EX(OnPaint)
		// MSG_WM_SETFOCUS_EX(OnSetFocus)
		// MSG_WM_KILLFOCUS_EX(OnKillFocus)
		MESSAGE_HANDLER_EX(WM_MOUSEMOVE, OnMouseMove)
		MESSAGE_HANDLER_EX(WM_MOUSEWHEEL, OnMouseWheel)
		MESSAGE_RANGE_HANDLER_EX(WM_MOUSEFIRST, 0x209, OnMouseEvent)
		MESSAGE_RANGE_HANDLER_EX(WM_KEYFIRST, WM_KEYLAST, OnKeyEvent)
		MESSAGE_HANDLER_EX(WM_IME_STARTCOMPOSITION, OnImeStartComposition)
	SOUI_MSG_MAP_END()

private:
	Browser *m_pBrowser;
	SStringW m_strUrl;
};

}