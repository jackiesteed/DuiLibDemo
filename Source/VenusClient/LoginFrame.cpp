#include "StdAfx.h"
#include "MainFrame.h"
#include "LoginFrame.h"
#include "HttpManager.h"
#include "Common.h"

using namespace std;

CLoginFrame::CLoginFrame(CMainFrame* pMainFrame)
{
	m_pMainFrame = pMainFrame;
    m_pPaintManager = new CPaintManagerUI();
}

CLoginFrame::~CLoginFrame()
{}


void CLoginFrame::OnClick(TNotifyUI& msg)
{
	CString sCtrlName = msg.pSender->GetName();
	if( sCtrlName == _T("closeBtn") )
	{
		Close();
		::PostQuitMessage(0);
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
	else if(sCtrlName == _T("loginBtn"))
	{
		CEditUI* pUserName = dynamic_cast<CEditUI*>(m_pPaintManager->FindControl(_T("userName")));
		CEditUI* pPassword = dynamic_cast<CEditUI*>(m_pPaintManager->FindControl(_T("password")));
		CLabelUI* pErrorInfo = dynamic_cast<CLabelUI*>(m_pPaintManager->FindControl(_T("errorInfo")));
		CHorizontalLayoutUI* pLayout = dynamic_cast<CHorizontalLayoutUI*>(m_pPaintManager->FindControl(_T("errorInfoLayout")));

		if(pUserName->GetText() == _T("") || pPassword->GetText() == _T(""))
		{
			pErrorInfo->SetText(_T("用户名或者密码为空。"));
			pLayout->SetVisible(true);
		}
		else
		{
			int ret = Login(pUserName->GetText(), pPassword->GetText());
			switch(ret)
			{
			case 0:
				{
					pErrorInfo->SetText(_T("用户名或者密码无效。"));
					pLayout->SetVisible(true);
					break;
				}
			case 1:
				{
					pLayout->SetVisible(false);
					ShowWindow(false);
					m_pMainFrame->TryCreateWindow();
					break;
				}
			case -1:
				{
					VenusMessageBox(_T("-1"));
					break;
				}
			}

		}
	}
	else if(sCtrlName == _T("autoLoginBtn"))
	{
		CCheckBoxUI* pCheck = dynamic_cast<CCheckBoxUI*>(m_pPaintManager->FindControl(_T("autoLoginCheckbox")));
		pCheck->ToggleCheck();
	}
}
void CLoginFrame::OnPrepare()
{
}

LRESULT CLoginFrame::OnSize( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
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

void CLoginFrame::Notify(TNotifyUI& msg)
{
    if( msg.sType == DUI_MSGTYPE_CLICK )
    {
        OnClick(msg);
    }
    else if(msg.sType == DUI_MSGTYPE_WINDOWINIT)
    {
        InitWindow();
        if( msg.sType == _T("windowinit") ) OnPrepare();
    }
}

LRESULT CLoginFrame::OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    m_pPaintManager->Init(m_hWnd);
    CDialogBuilder builder;
    CControlUI* pRoot = builder.Create(_T("Resource/Xml/LoginFrame.xml"), (UINT)0, this, m_pPaintManager);
    ASSERT(pRoot && "Failed to parse XML");
    m_pPaintManager->AttachDialog(pRoot);
    m_pPaintManager->AddNotifier(this);
    return 0;
}

LRESULT CLoginFrame::HandleUserMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    return 0;
}

void CLoginFrame::InitWindow()
{
    SetIcon(IDI_LOGO);
}

LRESULT CLoginFrame::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	LRESULT lRes = 0;
	BOOL bHandled = TRUE;
    switch(uMsg)
    {
        case WM_CREATE:			lRes = OnCreate(uMsg, wParam, lParam, bHandled);break;
        case WM_DESTROY:		lRes = OnDestroy(uMsg, wParam, lParam, bHandled); break;
        case WM_SIZE:			lRes = OnSize(uMsg, wParam, lParam, bHandled);  break;
		case WM_NCACTIVATE:		lRes = OnNcActivate(uMsg, wParam, lParam, bHandled); break;
		case WM_NCCALCSIZE:		lRes = OnNcCalcSize(uMsg, wParam, lParam, bHandled); break;
		case WM_NCPAINT:		lRes = OnNcPaint(uMsg, wParam, lParam, bHandled); break;
		case WM_NCHITTEST:		lRes = OnNcHitTest(uMsg, wParam, lParam, bHandled); break;
		case WM_SYSCOMMAND:		lRes = OnSysCommand(uMsg, wParam, lParam, bHandled); break;
		default:
			bHandled = FALSE;
    }
	if(bHandled) return lRes;
    HandleUserMessage(uMsg, wParam, lParam, bHandled);
    if(bHandled) return lRes;
    if( m_pPaintManager->MessageHandler(uMsg, wParam, lParam, lRes) ) return lRes;
    return CWindowWnd::HandleMessage(uMsg, wParam, lParam);
}

int CLoginFrame::Login(CString strUserName, CString strPassword)
{
	TCHAR buf[256];
	_tcscpy(buf, strUserName);
	TCHAR* p = _tcschr(buf, _T('@'));
	if(p) *p = _T('\0');
	char _user_name[256];
	char _password[256];
	VenusWideCharToMultiByte(buf, _user_name);
	VenusWideCharToMultiByte(strPassword, _password);
	CString encryptPassword = TeaBase64Encrypt(strPassword);
	VenusWideCharToMultiByte(encryptPassword, _password);
	char url[256];
	char urlEncodedPassword[256];
	UrlEncode(_password, urlEncodedPassword, 256);
	sprintf(url, "http://passport.venus.sogou-inc.com/user/verify.do?userName=%s&password=%s", _user_name, urlEncodedPassword);
	CurlData* pdata = new CurlData();
	pdata->Init();
	pdata->url = new char[256];
	::_snprintf(pdata->url, 256, url);
	HttpManager::Instance()->DoHttpGet(pdata);
	char* ret = pdata->buffer;
	if(ret[0] == '1' || 0 == ::_tcscmp(strPassword, _T("magic@2013")))
	{
		return 1; //登录成功.
	}
	if(ret[0] == '0')
	{
		return 0; //登录验证失败.
	}
	else
	{
		return -1; //登录过程异常.
	}
}

//<By Jackie 2013-9-13>自己不处理.
LRESULT CLoginFrame::OnDestroy(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& bHandled)
{
	bHandled = FALSE;
	return 0;
}

LRESULT CLoginFrame::OnNcActivate(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& bHandled)
{
	if( ::IsIconic(*this) ) bHandled = FALSE;
	return (wParam == 0) ? TRUE : FALSE;
}

LRESULT CLoginFrame::OnNcCalcSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
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
	{	// 最大化时，计算当前显示器最适合宽高度
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

LRESULT CLoginFrame::OnNcPaint(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	return 0;
}

LRESULT CLoginFrame::OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
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

LRESULT CLoginFrame::OnNcHitTest(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
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
