#include "cefclient/cefclient.h"
#include "include/cef_app.h"
#include "include/cef_browser.h"
#include "include/cef_frame.h"
#include "include/cef_runnable.h"
#include "cefclient/cefclient_osr_widget_win.h"
#include "cefclient/client_handler.h"
#include "cefclient/client_switches.h"
#include "cefclient/resource.h"
#include "cefclient/scheme_test.h"
#include "cefclient/string_util.h"
#include "StdAfx.h"
#include "BrowserFrame.h"

#define MAX_LOADSTRING 100
#define MAX_URL_LENGTH  255
#define BUTTON_WIDTH 72
#define URLBAR_HEIGHT  24

// The global ClientHandler reference.
extern CefRefPtr<ClientHandler> g_handler;

class MainBrowserProvider : public OSRBrowserProvider
{
	virtual CefRefPtr<CefBrowser> GetBrowser()
	{
		if (g_handler.get())
			return g_handler->GetBrowser();

		return NULL;
	}
} g_main_browser_provider;


CBrowserFrame::CBrowserFrame(){}
CBrowserFrame::~CBrowserFrame(){}

void CBrowserFrame::Create(HWND hwndParent, DWORD dwStyle, DWORD dwExStyle, const RECT rc, HMENU hMenu /* = NULL */)
{
	int cx = rc.right - rc.left;
	int cy = rc.bottom - rc.top;
	Create(hwndParent, dwStyle, dwExStyle, rc.left, rc.top, cx, cy, hMenu);
}

void CBrowserFrame::Create(HWND hwndParent, DWORD dwStyle, DWORD dwExStyle, int x /* = 0 */, int y /* = 0 */, int cx /* = 0 */, int cy /* = 0 */, HMENU hMenu /* = NULL */)
{

	TCHAR szTitle[MINI_SIZE] = _T("BrowserFrame");  // The title bar text
	TCHAR szWindowClass[MINI_SIZE] = _T("BrowserFrameUI");  // the main window class name
	

	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style         = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc   = WndProc;
	wcex.cbClsExtra    = 0;
	wcex.cbWndExtra    = 0;
	wcex.hInstance     = g_meta.hInst;
	wcex.hIcon         = LoadIcon(NULL, IDI_APPLICATION);
	wcex.hCursor       = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName  = NULL;
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm       = LoadIcon(NULL, IDI_APPLICATION);

	::RegisterClassEx(&wcex);

	m_hwnd = ::CreateWindowEx(dwExStyle, szWindowClass, szTitle, dwStyle, x, y, cx, cy, hwndParent, hMenu, g_meta.hInst, NULL);

	if (!m_hwnd)
		return ;

	ShowWindow(m_hwnd, SW_SHOW);
	UpdateWindow(m_hwnd);

}

void CBrowserFrame::ShowPage(char url[])
{
	g_handler->GetBrowser()->GetMainFrame()->LoadURL(url);
}




LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam,
	LPARAM lParam)
{
	static HWND backWnd = NULL, forwardWnd = NULL, reloadWnd = NULL,
		stopWnd = NULL, editWnd = NULL;
	static WNDPROC editWndOldProc = NULL;
	TCHAR szOSRWindowClass[MINI_SIZE] = _T("OSRWindowClass");  // the OSR window class name

	// Static members used for the find dialog.
	//static FINDREPLACE fr;
	static WCHAR szFindWhat[80] = {0};
	static WCHAR szLastFindWhat[80] = {0};
	static bool findNext = false;
	static bool lastMatchCase = false;

	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;

	if (hWnd == editWnd)
	{
		// Callback for the edit window
		switch (message)
		{
		case WM_CHAR:
			if (wParam == VK_RETURN && g_handler.get())
			{
				// When the user hits the enter key load the URL
				CefRefPtr<CefBrowser> browser = g_handler->GetBrowser();
				wchar_t strPtr[MAX_URL_LENGTH + 1] = {0};
				*((LPWORD)strPtr) = MAX_URL_LENGTH;
				LRESULT strLen = SendMessage(hWnd, EM_GETLINE, 0, (LPARAM)strPtr);
				if (strLen > 0)
				{
					strPtr[strLen] = 0;
					browser->GetMainFrame()->LoadURL(strPtr);
				}

				return 0;
			}
		}

		return (LRESULT)CallWindowProc(editWndOldProc, hWnd, message, wParam,
			lParam);
	}
	else
	{
		// Callback for the main window
		switch (message)
		{
		case WM_CREATE:
			{
				// Create the single static handler class instance
				g_handler = new ClientHandler();
				g_handler->SetMainHwnd(hWnd);
				

				// Create the child windows used for navigation
				RECT rect;
				int x = 0;

				GetClientRect(hWnd, &rect);

				/*backWnd = CreateWindow(L"BUTTON", L"Back",
					WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON
					| WS_DISABLED, x, 0, 0, 0,
					hWnd, (HMENU) IDC_NAV_BACK, g_meta.hInst, 0);
				x += BUTTON_WIDTH;

				forwardWnd = CreateWindow(L"BUTTON", L"Forward",
					WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON
					| WS_DISABLED, x, 0, 0,
					0, hWnd, (HMENU) IDC_NAV_FORWARD,
					g_meta.hInst, 0);
				x += BUTTON_WIDTH;

				reloadWnd = CreateWindow(L"BUTTON", L"Reload",
					WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON
					| WS_DISABLED, x, 0, 0,
					0, hWnd, (HMENU) IDC_NAV_RELOAD,
					g_meta.hInst, 0);
				x += BUTTON_WIDTH;

				stopWnd = CreateWindow(L"BUTTON", L"Stop",
					WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON
					| WS_DISABLED, x, 0, 0, 0,
					hWnd, (HMENU) IDC_NAV_STOP, g_meta.hInst, 0);
				x += BUTTON_WIDTH;*/

				/*editWnd = CreateWindow(L"EDIT", 0,
				WS_CHILD | WS_VISIBLE | WS_BORDER | ES_LEFT |
				ES_AUTOVSCROLL | ES_AUTOHSCROLL | WS_DISABLED,
				x, 0, 0,
				0, hWnd, 0, g_meta.hInst, 0);*/

				// Assign the edit window's WNDPROC to this function so that we can
				// capture the enter key
				/*editWndOldProc =
				reinterpret_cast<WNDPROC>(GetWindowLongPtr(editWnd, GWLP_WNDPROC));
				SetWindowLongPtr(editWnd, GWLP_WNDPROC,
				reinterpret_cast<LONG_PTR>(WndProc));*/
				/*g_handler->SetEditHwnd(editWnd);
				g_handler->SetButtonHwnds(backWnd, forwardWnd, reloadWnd, stopWnd);*/

				//rect.top += URLBAR_HEIGHT;

				CefWindowInfo info;
				
				CefBrowserSettings settings;

				if (AppIsOffScreenRenderingEnabled())
				{
					CefRefPtr<CefCommandLine> cmd_line = AppGetCommandLine();
					bool transparent =
						cmd_line->HasSwitch(cefclient::kTransparentPaintingEnabled);

					CefRefPtr<OSRWindow> osr_window =
						OSRWindow::Create(&g_main_browser_provider, transparent);
					osr_window->CreateWidget(hWnd, rect, g_meta.hInst, szOSRWindowClass);
					info.SetAsOffScreen(osr_window->hwnd());
					info.SetTransparentPainting(transparent ? TRUE : FALSE);
					g_handler->SetOSRHandler(osr_window.get());
				}
				else
				{
					// Initialize window info to the defaults for a child window.
					info.SetAsChild(hWnd, rect);
				}

				// Creat the new child browser window
				CefBrowserHost::CreateBrowser(info, g_handler.get(),
					g_handler->GetStartupURL(), settings);

				return 0;
			}

		case WM_COMMAND:
			{
				CefRefPtr<CefBrowser> browser;
				if (g_handler.get())
					browser = g_handler->GetBrowser();

				wmId    = LOWORD(wParam);
				wmEvent = HIWORD(wParam);
				// Parse the menu selections:
				switch (wmId)
				{
				case IDM_EXIT:
					if (g_handler.get())
						g_handler->CloseAllBrowsers(false);
					return 0;
					/*case ID_WARN_CONSOLEMESSAGE:
					if (g_handler.get())
					{
					std::wstringstream ss;
					ss << L"Console messages will be written to "
					<< std::wstring(CefString(g_handler->GetLogFile()));
					MessageBox(hWnd, ss.str().c_str(), L"Console Messages",
					MB_OK | MB_ICONINFORMATION);
					}*/
					return 0;
				case ID_WARN_DOWNLOADCOMPLETE:
				case ID_WARN_DOWNLOADERROR:
					if (g_handler.get())
					{
						std::wstringstream ss;
						ss << L"File \"" <<
							std::wstring(CefString(g_handler->GetLastDownloadFile())) <<
							L"\" ";

						if (wmId == ID_WARN_DOWNLOADCOMPLETE)
							ss << L"downloaded successfully.";
						else
							ss << L"failed to download.";

						MessageBox(hWnd, ss.str().c_str(), L"File Download",
							MB_OK | MB_ICONINFORMATION);
					}
					return 0;
				case IDC_NAV_BACK:   // Back button
					if (browser.get())
						browser->GoBack();
					return 0;
				case IDC_NAV_FORWARD:  // Forward button
					if (browser.get())
						browser->GoForward();
					return 0;
				case IDC_NAV_RELOAD:  // Reload button
					if (browser.get())
						browser->Reload();
					return 0;
				case IDC_NAV_STOP:  // Stop button
					if (browser.get())
						browser->StopLoad();
					return 0;
				case ID_TESTS_GETSOURCE:  // Test the GetSource function
					if (browser.get())
						RunGetSourceTest(browser);
					return 0;
				case ID_TESTS_GETTEXT:  // Test the GetText function
					if (browser.get())
						RunGetTextTest(browser);
					return 0;
				case ID_TESTS_POPUP:  // Test a popup window
					if (browser.get())
						RunPopupTest(browser);
					return 0;
				case ID_TESTS_REQUEST:  // Test a request
					if (browser.get())
						RunRequestTest(browser);
					return 0;
				case ID_TESTS_PLUGIN_INFO:  // Test plugin info
					if (browser.get())
						RunPluginInfoTest(browser);
					return 0;
				//case ID_TESTS_ZOOM_IN:
				//	if (browser.get())
				//		ModifyZoom(browser, 0.5);
				//	return 0;
				//case ID_TESTS_ZOOM_OUT:
				//	if (browser.get())
				//		ModifyZoom(browser, -0.5);
				//	return 0;
				case ID_TESTS_ZOOM_RESET:
					if (browser.get())
						browser->GetHost()->SetZoomLevel(0.0);
					return 0;
				case ID_TESTS_TRACING_BEGIN:
					g_handler->BeginTracing();
					return 0;
				case ID_TESTS_TRACING_END:
					g_handler->EndTracing();
					return 0;
				case ID_TESTS_OTHER_TESTS:
					if (browser.get())
						RunOtherTests(browser);
					return 0;
				}
				break;
			}

		case WM_PAINT:
			hdc = BeginPaint(hWnd, &ps);
			EndPaint(hWnd, &ps);
			return 0;

		case WM_SETFOCUS:
			if (g_handler.get() && g_handler->GetBrowser())
			{
				// Pass focus to the browser window
				CefWindowHandle hwnd =
					g_handler->GetBrowser()->GetHost()->GetWindowHandle();
				if (hwnd)
					PostMessage(hwnd, WM_SETFOCUS, wParam, NULL);
			}
			return 0;

		case WM_SIZE:
			// Minimizing resizes the window to 0x0 which causes our layout to go all
			// screwy, so we just ignore it.
			if (wParam != SIZE_MINIMIZED && g_handler.get() &&
				g_handler->GetBrowser())
			{
				CefWindowHandle hwnd =
					g_handler->GetBrowser()->GetHost()->GetWindowHandle();
				if (hwnd)
				{
					// Resize the browser window and address bar to match the new frame
					// window size
					RECT rect;
					GetClientRect(hWnd, &rect);
					//rect.top += URLBAR_HEIGHT;

					//int urloffset = rect.left + BUTTON_WIDTH * 4;

					HDWP hdwp = BeginDeferWindowPos(1);
					/*hdwp = DeferWindowPos(hdwp, editWnd, NULL, urloffset,
					0, rect.right - urloffset, URLBAR_HEIGHT, SWP_NOZORDER);*/
					hdwp = DeferWindowPos(hdwp, hwnd, NULL,
						rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top,
						SWP_NOZORDER);
					EndDeferWindowPos(hdwp);
				}
			}
			break;

		case WM_ERASEBKGND:
			if (g_handler.get() && g_handler->GetBrowser())
			{
				CefWindowHandle hwnd =
					g_handler->GetBrowser()->GetHost()->GetWindowHandle();
				if (hwnd)
				{
					// Dont erase the background if the browser window has been loaded
					// (this avoids flashing)
					return 0;
				}
			}
			break;

		case WM_ENTERMENULOOP:
			if (!wParam)
			{
				// Entering the menu loop for the application menu.
				CefSetOSModalLoop(true);
			}
			break;

		case WM_EXITMENULOOP:
			if (!wParam)
			{
				// Exiting the menu loop for the application menu.
				CefSetOSModalLoop(false);
			}
			break;

		case WM_CLOSE:
			if (g_handler.get() && !g_handler->IsClosing())
			{
				CefRefPtr<CefBrowser> browser = g_handler->GetBrowser();
				if (browser.get())
				{
					// Notify the browser window that we would like to close it. This
					// will result in a call to ClientHandler::DoClose() if the
					// JavaScript 'onbeforeunload' event handler allows it.
					browser->GetHost()->CloseBrowser(false);

					// Cancel the close.
					return 0;
				}
			}

			// Allow the close.
			break;

		case WM_DESTROY:
			// Quitting CEF is handled in ClientHandler::OnBeforeClose().
			return 0;
		}

		return DefWindowProc(hWnd, message, wParam, lParam);
	}
}