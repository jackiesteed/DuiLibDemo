#include <direct.h>
#include <sstream>
#include <string>
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
#include "cefclient/cefclient.h"
#include "StdAfx.h"
#include <exdisp.h>
#include <comdef.h>
#include "MainFrame.h"
#include <stdio.h>
#include <cstdlib>
#include <sstream>
#include <string>
#include "MetaDefine.h"

char szWorkingDir[MAX_PATH];  // The current working directory
HWND hMessageWnd = NULL;
// Global functions

GlobalMeta g_meta;

std::string AppGetWorkingDirectory()
{
	return szWorkingDir;
}

void AppQuitMessageLoop()
{
	CefRefPtr<CefCommandLine> command_line = AppGetCommandLine();
	if (command_line->HasSwitch(cefclient::kMultiThreadedMessageLoop) || true)
	{
		// Running in multi-threaded message loop mode. Need to execute
		// PostQuitMessage on the main application thread.
		ASSERT(hMessageWnd);
		PostMessage(hMessageWnd, WM_COMMAND, ID_QUIT, 0);
	}
	else
	{
		CefQuitMessageLoop();
	}
}

LRESULT CALLBACK MessageWndProc(HWND hWnd, UINT message, WPARAM wParam,
	LPARAM lParam)
{
	switch (message)
	{
	case WM_COMMAND:
		{
			int wmId = LOWORD(wParam);
			switch (wmId)
			{
			case ID_QUIT:
				PostQuitMessage(0);
				return 0;
			}
		}
	}
	return DefWindowProc(hWnd, message, wParam, lParam);
}

HWND CreateMessageWindow(HINSTANCE hInstance)
{
	static const wchar_t kWndClass[] = L"ClientMessageWindow";

	WNDCLASSEX wc = {0};
	wc.cbSize = sizeof(wc);
	wc.lpfnWndProc = MessageWndProc;
	wc.hInstance = hInstance;
	wc.lpszClassName = kWndClass;
	RegisterClassEx(&wc);

	return CreateWindow(kWndClass, 0, 0, 0, 0, 0, 0, HWND_MESSAGE, 0,
		hInstance, 0);
}

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{

	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);
	CefMainArgs main_args(hInstance);
	CefRefPtr<ClientApp> app(new ClientApp);

	g_meta.hInst = hInstance;

	// Execute the secondary process, if any.
	int exit_code = CefExecuteProcess(main_args, app.get());
	if (exit_code >= 0)
		return exit_code;

	// Retrieve the current working directory.
	if (_getcwd(szWorkingDir, MAX_PATH) == NULL)
		szWorkingDir[0] = 0;

	// Parse command line arguments. The passed in values are ignored on Windows.
	AppInitCommandLine(0, NULL);

	CefSettings settings;
	

	// Populate the settings based on command line arguments.
	AppGetSettings(settings);
	settings.multi_threaded_message_loop = true;
	// Initialize CEF.
	CefInitialize(main_args, settings, app.get());

	// Register the scheme handler.
	scheme_test::InitTest();

	CPaintManagerUI::SetInstance(hInstance);
	CPaintManagerUI::SetResourcePath(CPaintManagerUI::GetInstancePath());

	HRESULT Hr = ::CoInitialize(NULL);
	if( FAILED(Hr) ) return 0;

	CMainFrame* pFrame = new CMainFrame();
	if( pFrame == NULL ) return 0;
	pFrame->TryCreateWindow();
	//pFrame->ShowLoginFrame();
	if (!settings.multi_threaded_message_loop)
	{
		// Run the CEF message loop. This function will block until the application
		// recieves a WM_QUIT message.
		CefRunMessageLoop();
	}
	else
	{
		// Create a hidden window for message processing.
		hMessageWnd = CreateMessageWindow(hInstance);
		ASSERT(hMessageWnd);
		
		CPaintManagerUI::MessageLoop();
	}

	// Shut down CEF.
	CefShutdown();
	::CoUninitialize();


	return 0;
}