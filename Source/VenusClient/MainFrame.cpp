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
#include "MainFrame.h"
#include "MetaDefine.h"

using namespace std;

UINT WM_TASKBARCREATED = 0;

CMainFrame::CMainFrame()
{
    m_pPaintManager = new CPaintManagerUI();
	m_pTreeViewManager = new CTreeViewManager();
	m_pListViewManager = new CListViewManager();
	m_pLoginFrame = NULL;
	m_pBrowserFrame = NULL;
	m_bCreated = false;
	m_psysTray = new CSystemTray();
	m_hIcon = NULL;
}

CMainFrame::~CMainFrame()
{
	delete m_psysTray;
	delete m_pTreeViewManager;
	delete m_pListViewManager;
	delete m_pLoginFrame;
	delete m_pBrowserFrame;
	m_bCreated = false;
	m_hIcon = NULL;
}


void CMainFrame::OnClick(TNotifyUI& msg)
{
	CString sCtrlName = msg.pSender->GetName();
	if( sCtrlName == _T("closeBtn") )
	{
		Close();
		return; 
	}
	else if( sCtrlName == _T("minBtn"))
	{ 
		SendMessage(WM_SYSCOMMAND, SC_MINIMIZE, 0); 
		return; 
	}
	else if( sCtrlName == _T("maxBtn"))
	{ 
		SendMessage(WM_SYSCOMMAND, SC_MAXIMIZE, 0); 
		return; 
	}
	else if( sCtrlName == _T("restoreBtn"))
	{ 
		SendMessage(WM_SYSCOMMAND, SC_RESTORE, 0); 
		return; 
	}
	else if(sCtrlName == _T("logoutBtn"))
	{
		ShowWindow(false);
		ShowLoginFrame();
	}
	else if(sCtrlName == _T("configBtn"))
	{
		m_pBrowserFrame->ShowPage("www.google.com");
	}
}


void CMainFrame::OnPrepare()
{
	InitWindow();
	CTreeViewUI* pTreeView = dynamic_cast<CTreeViewUI*>(m_pPaintManager->FindControl(_T("treeView")));
	m_pTreeViewManager->Init(pTreeView);
	CListUI* pListView = dynamic_cast<CListUI*>(m_pPaintManager->FindControl(_T("listView")));
	m_pListViewManager->Init(pListView);

	CVerticalLayoutUI* pLayout = dynamic_cast<CVerticalLayoutUI*>(m_pPaintManager->FindControl(_T("rightContentInner")));
	m_pBrowserFrame = new CBrowserFrame();
	RECT rc = pLayout->GetPos();
	rc.top += 2;
	rc.left += 2;
	rc.right -= 2;
	rc.bottom -= 2;
	m_pBrowserFrame->Create(m_hWnd, WS_CHILDWINDOW, 0, rc); //WS_CHILDWINDOW
	m_hIcon = ::LoadIcon(g_meta.hInst, MAKEINTRESOURCE(IDI_LOGO));
	m_psysTray->Create(m_hWnd, m_hIcon, _T("Venus"));
	
	//增加hook功能, 重启资源管理器的时候, 添加托盘.
	WM_TASKBARCREATED = ::RegisterWindowMessage(_T("TaskbarCreated"));
}

LRESULT CMainFrame::OnSize( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	if(m_pBrowserFrame && wParam != SIZE_MINIMIZED)
	{
		RECT rc;
		GetClientRect(m_hWnd, &rc);
		MoveWindow(m_pBrowserFrame->GetHwnd(), 666, 38, rc.right - rc.left  - 675, rc.bottom - rc.top - 55, true);
	}
	
	
    SIZE szRoundCorner = m_pPaintManager->GetRoundCorner();
    if( !::IsIconic(*this) && (szRoundCorner.cx != 0 || szRoundCorner.cy != 0) )
    {
        CDuiRect rcWnd;
        ::GetWindowRect(*this, &rcWnd);
        rcWnd.Offset(-rcWnd.left, -rcWnd.top);
        rcWnd.right++; rcWnd.bottom++;
        HRGN hRgn = ::CreateRoundRectRgn(rcWnd.left, rcWnd.top, rcWnd.right, rcWnd.bottom, szRoundCorner.cx, szRoundCorner.cy);
        ::SetWindowRgn(*this, hRgn, TRUE);
        ::DeleteObject(hRgn);
    }

    bHandled = FALSE;
    return 0;
}

void CMainFrame::Notify(TNotifyUI& msg)
{
    if( msg.sType == DUI_MSGTYPE_CLICK )
    {
        OnClick(msg);
    }
    else if(msg.sType == DUI_MSGTYPE_WINDOWINIT)
    {
        OnPrepare();
    }
}

LRESULT CMainFrame::OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    m_pPaintManager->Init(m_hWnd);
    CDialogBuilder builder;
    CControlUI* pRoot = builder.Create(_T("Resource/Xml/MainFrame.xml"), (UINT)0, this, m_pPaintManager);
    ASSERT(pRoot && "Failed to parse XML");
    m_pPaintManager->AttachDialog(pRoot);
    m_pPaintManager->AddNotifier(this);
    return 0;
}

LRESULT CMainFrame::HandleUserMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    return 0;
}

void CMainFrame::InitWindow()
{
    SetIcon(IDI_LOGO);
}


LRESULT CMainFrame::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	LRESULT lRes = 0;
	BOOL bHandled = TRUE;
    switch(uMsg)
    {
        case WM_CREATE:			lRes =  OnCreate(uMsg, wParam, lParam, bHandled);break;
        case WM_DESTROY:		lRes = OnDestroy(uMsg, wParam, lParam, bHandled); break;
        case WM_SIZE:			lRes = OnSize(uMsg, wParam, lParam, bHandled); break;
		case WM_NCACTIVATE:		lRes = OnNcActivate(uMsg, wParam, lParam, bHandled); break;
		case WM_NCCALCSIZE:		lRes = OnNcCalcSize(uMsg, wParam, lParam, bHandled); break;
		case WM_NCPAINT:		lRes = OnNcPaint(uMsg, wParam, lParam, bHandled); break;
		case WM_NCHITTEST:		lRes = OnNcHitTest(uMsg, wParam, lParam, bHandled); break;
		case WM_SYSCOMMAND:		lRes = OnSysCommand(uMsg, wParam, lParam, bHandled); break;
		case WM_GETMINMAXINFO:	lRes = OnGetMinMaxInfo(uMsg, wParam, lParam, bHandled); break;
		default:
			bHandled = FALSE;
    }

	if(uMsg == WM_TASKBARCREATED)
	{
		m_psysTray->Create(m_hWnd, m_hIcon, _T("Venus"));
		return 0;
	}
    HandleUserMessage(uMsg, wParam, lParam, bHandled);
    if(bHandled) return lRes;

    
    if( m_pPaintManager->MessageHandler(uMsg, wParam, lParam, lRes) ) return lRes;
    return CWindowWnd::HandleMessage(uMsg, wParam, lParam);
}

//<By Jackie 2013-9-13>自己不处理.
LRESULT CMainFrame::OnDestroy(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& bHandled)
{
	bHandled = FALSE;
	return 0;
}

LRESULT CMainFrame::OnNcActivate(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& bHandled)
{
	if( ::IsIconic(*this) ) bHandled = FALSE;
	return (wParam == 0) ? TRUE : FALSE;
}

LRESULT CMainFrame::OnNcCalcSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	LPRECT pRect=NULL;

	if ( wParam == TRUE)
	{
		LPNCCALCSIZE_PARAMS pParam = (LPNCCALCSIZE_PARAMS)lParam;
		pRect=&pParam->rgrc[0];
	}
	else
	{
		pRect=(LPRECT)lParam;
	}

	if ( ::IsZoomed(m_hWnd))
	{
		return 0;
		// 最大化时，计算当前显示器最适合宽高度
		MONITORINFO oMonitor = {};
		oMonitor.cbSize = sizeof(oMonitor);
		::GetMonitorInfo(::MonitorFromWindow(*this, MONITOR_DEFAULTTONEAREST), &oMonitor);
		CDuiRect rcWork = oMonitor.rcWork;
		CDuiRect rcMonitor = oMonitor.rcMonitor;
		rcWork.Offset(-oMonitor.rcMonitor.left, -oMonitor.rcMonitor.top);

		pRect->right = pRect->left + rcWork.GetWidth();
		pRect->bottom = pRect->top + rcWork.GetHeight();
		return WVR_REDRAW;
	}

	return 0;
}

LRESULT CMainFrame::OnNcPaint(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	return 0;
}

LRESULT CMainFrame::OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if (wParam == SC_CLOSE)
	{
		bHandled = TRUE;
		SendMessage(WM_CLOSE);
		return 0;
	}
	BOOL bZoomed = ::IsZoomed(m_hWnd);
	LRESULT lRes = CWindowWnd::HandleMessage(uMsg, wParam, lParam);
	if(::IsZoomed(m_hWnd) != bZoomed)
	{
		if(!bZoomed)
		{
			CControlUI* pControl = static_cast<CControlUI*>(m_pPaintManager->FindControl(_T("maxBtn")));
			if(pControl) pControl->SetVisible(false);
			pControl = static_cast<CControlUI*>(m_pPaintManager->FindControl(_T("restoreBtn")));
			if(pControl) pControl->SetVisible(true);
		}
		else
		{
			CControlUI* pControl = static_cast<CControlUI*>(m_pPaintManager->FindControl(_T("maxBtn")));
			if(pControl) pControl->SetVisible(true);
			pControl = static_cast<CControlUI*>(m_pPaintManager->FindControl(_T("restoreBtn")));
			if(pControl) pControl->SetVisible(false);
		}
	}
	return lRes;
}

LRESULT CMainFrame::OnNcHitTest(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	POINT pt; pt.x = GET_X_LPARAM(lParam); pt.y = GET_Y_LPARAM(lParam);
	::ScreenToClient(*this, &pt);

	RECT rcClient;
	::GetClientRect(*this, &rcClient);

	if( !::IsZoomed(*this) )
	{
		RECT rcSizeBox = m_pPaintManager->GetSizeBox();
		if( pt.y < rcClient.top + rcSizeBox.top )
		{
			if( pt.x < rcClient.left + rcSizeBox.left ) return HTTOPLEFT;
			if( pt.x > rcClient.right - rcSizeBox.right ) return HTTOPRIGHT;
			return HTTOP;
		}
		else if( pt.y > rcClient.bottom - rcSizeBox.bottom )
		{
			if( pt.x < rcClient.left + rcSizeBox.left ) return HTBOTTOMLEFT;
			if( pt.x > rcClient.right - rcSizeBox.right ) return HTBOTTOMRIGHT;
			return HTBOTTOM;
		}

		if( pt.x < rcClient.left + rcSizeBox.left ) return HTLEFT;
		if( pt.x > rcClient.right - rcSizeBox.right ) return HTRIGHT;
	}

	RECT rcCaption = m_pPaintManager->GetCaptionRect();
	if( pt.x >= rcClient.left + rcCaption.left && pt.x < rcClient.right - rcCaption.right \
		&& pt.y >= rcCaption.top && pt.y < rcCaption.bottom ) {
			CControlUI* pControl = static_cast<CControlUI*>(m_pPaintManager->FindControl(pt));
			if( pControl && _tcsicmp(pControl->GetClass(), _T("ButtonUI")) != 0 && 
				_tcsicmp(pControl->GetClass(), _T("OptionUI")) != 0 &&
				_tcsicmp(pControl->GetClass(), _T("TextUI")) != 0 )
				return HTCAPTION;
	}
	return HTCLIENT;
}

void CMainFrame::ShowLoginFrame()
{
	if(m_pLoginFrame == NULL)
	{
		m_pLoginFrame = new CLoginFrame(this);
		m_pLoginFrame->Create(NULL, _T("Venus"), UI_WNDSTYLE_FRAME & ~WS_MAXIMIZEBOX & ~WS_THICKFRAME, WS_EX_WINDOWEDGE);
	}
	if( m_pLoginFrame == NULL ) return ;
	m_pLoginFrame->ShowWindow(true);
}

void CMainFrame::TryCreateWindow()
{
	if(m_bCreated == false)
	{
		Create(NULL, _T("Venus 张方雪"), UI_WNDSTYLE_FRAME | WS_CLIPCHILDREN, WS_EX_APPWINDOW);
		m_bCreated = true;
	}
	CenterWindow();
}

LRESULT CMainFrame::OnGetMinMaxInfo(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	LPMINMAXINFO lpMMI = (LPMINMAXINFO) lParam;

	MONITORINFO oMonitor = {};
	oMonitor.cbSize = sizeof(oMonitor);
	::GetMonitorInfo(::MonitorFromWindow(*this, MONITOR_DEFAULTTONEAREST), &oMonitor);
	CDuiRect rcWork = oMonitor.rcWork;
	CDuiRect rcMonitor = oMonitor.rcMonitor;
	rcWork.Offset(-oMonitor.rcMonitor.left, -oMonitor.rcMonitor.top);

	// 计算最大化时，正确的原点坐标
	lpMMI->ptMaxPosition.x	= rcWork.left;
	lpMMI->ptMaxPosition.y	= rcWork.top;

	lpMMI->ptMaxTrackSize.x = rcWork.GetWidth();
	lpMMI->ptMaxTrackSize.y =  rcWork.GetHeight();

	lpMMI->ptMinTrackSize.x =m_pPaintManager->GetMinInfo().cx;
	lpMMI->ptMinTrackSize.y =m_pPaintManager->GetMinInfo().cy;

	bHandled = FALSE;
	return 0;
}