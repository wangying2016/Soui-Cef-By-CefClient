// MainDlg.cpp : implementation of the CMainDlg class
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MainDlg.h"
	
#ifdef DWMBLUR	//win7毛玻璃开关
#include <dwmapi.h>
#pragma comment(lib,"dwmapi.lib")
#endif
	
CMainDlg::CMainDlg() : SHostWnd(_T("LAYOUT:XML_MAINWND"))
{
	m_bLayoutInited = FALSE;
}

CMainDlg::~CMainDlg()
{
}

int CMainDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	#ifdef DWMBLUR	//win7毛玻璃开关
	MARGINS mar = {5,5,30,5};
	DwmExtendFrameIntoClientArea ( m_hWnd, &mar );
	#endif

	SetMsgHandled(FALSE);
	return 0;
}

BOOL CMainDlg::OnInitDialog(HWND hWnd, LPARAM lParam)
{
	m_bLayoutInited = TRUE;

	SOUI::SCef3Window *pCefBrowser = FindChildByName2<SOUI::SCef3Window>(L"cef3");
	SASSERT(pCefBrowser);
	pCefBrowser->GetEventSet()->subscribeEvent(EVT_WEBVIEW_NOTIFY, Subscriber(&CMainDlg::OnWebViewNotify, this));

	return 0;
}

void CMainDlg::OnClose()
{
	CSimpleWnd::DestroyWindow();
}

void CMainDlg::OnMaximize()
{
	SendMessage(WM_SYSCOMMAND, SC_MAXIMIZE);
}
void CMainDlg::OnRestore()
{
	SendMessage(WM_SYSCOMMAND, SC_RESTORE);
}
void CMainDlg::OnMinimize()
{
	SendMessage(WM_SYSCOMMAND, SC_MINIMIZE);
}

void CMainDlg::OnSize(UINT nType, CSize size)
{
	SetMsgHandled(FALSE);
	if (!m_bLayoutInited) return;
	
	SWindow *pBtnMax = FindChildByName(L"btn_max");
	SWindow *pBtnRestore = FindChildByName(L"btn_restore");
	if(!pBtnMax || !pBtnRestore) return;
	
	if (nType == SIZE_MAXIMIZED)
	{
		pBtnRestore->SetVisible(TRUE);
		pBtnMax->SetVisible(FALSE);
	}
	else if (nType == SIZE_RESTORED)
	{
		pBtnRestore->SetVisible(FALSE);
		pBtnMax->SetVisible(TRUE);
	}
}

void CMainDlg::OnLoadURL()
{
	SOUI::SEdit *pEdit = FindChildByName2<SOUI::SEdit>(L"edit_input_url");
	SASSERT(pEdit);
	SStringW strURL = pEdit->GetWindowText();
	SOUI::SCef3Window *pCefBrowser = FindChildByName2<SOUI::SCef3Window>(L"cef3");
	SASSERT(pCefBrowser);
	pCefBrowser->LoadURL(strURL);
}

void CMainDlg::OnExecuteJS()
{
	SOUI::SEdit *pEdit = FindChildByName2<SOUI::SEdit>(L"edit_input_url");
	SASSERT(pEdit);
	SStringW strJs = pEdit->GetWindowText();
	SOUI::SCef3Window *pCefBrowser = FindChildByName2<SOUI::SCef3Window>(L"cef3");
	SASSERT(pCefBrowser);
	pCefBrowser->ExecuteJS(strJs);
}

bool CMainDlg::OnWebViewNotify(SOUI::EventArgs *pEvt)
{
	SOUI::EventWebViewNotify *pev = SOUI::sobj_cast<SOUI::EventWebViewNotify>(pEvt);
	SASSERT(pev);
	SStringW args;
	for (size_t i = 0; i < pev->Arguments.GetCount(); ++i) {
		if (i != 0)
			args += _T(",");
		args += pev->Arguments[i];
	}
	if (pev->MessageName == L"ClickButton")
		SOUI::SMessageBox(m_hWnd, args, pev->MessageName, MB_OK);
	return true;
}

