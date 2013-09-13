#include "StdAfx.h"
#include "MetaDefine.h"
#include <ShellAPI.h>
#include "SystemTray.h"


//<By张方雪 2013/4/17>The system enforces minimum and maximum timeout values.
//Values specified in uTimeout that are too large are set to the maximum value.
//Values that are too small default to the minimum value. 
//The system minimum and maximum timeout values are currently set at 10 seconds and 30 seconds, respectively. 

CSystemTray::CSystemTray(void): 
	m_hParent(NULL), m_hIconTray(NULL), m_hIconBalloon(NULL)
{
	m_lLastTipTime = static_cast<long>(time(NULL));
	::ZeroMemory(&m_nidSystemTray, sizeof(NOTIFYICONDATA));
}

CSystemTray::~CSystemTray(void)
{
	Destroy();
}

void CSystemTray::Destroy()
{
	if(m_nidSystemTray.hWnd != NULL)
	{
		Shell_NotifyIcon(NIM_DELETE, &m_nidSystemTray);
		::ZeroMemory(&m_nidSystemTray, sizeof(NOTIFYICONDATA));
	}
	m_hParent = NULL;
}

bool CSystemTray::SetFocus()
{
	assert(m_nidSystemTray.hWnd != NULL);
	return MKBOOL(Shell_NotifyIcon(NIM_SETFOCUS, &m_nidSystemTray));
}

bool CSystemTray::ModifyBalloonIcon(HICON hIcon)
{
	m_nidSystemTray.dwInfoFlags = NIIF_USER;
	m_nidSystemTray.hIcon = m_hIconBalloon = hIcon;
	m_nidSystemTray.uFlags = NIF_INFO | NIF_ICON;
	BOOL bRet = FALSE;
	bRet = Shell_NotifyIcon(NIM_MODIFY, &m_nidSystemTray);
	if (bRet)
	{
		m_nidSystemTray.hIcon = m_hIconTray;

		m_nidSystemTray.uFlags = NIF_ICON;
		return MKBOOL(Shell_NotifyIcon(NIM_MODIFY, &m_nidSystemTray));
	}
	else return false;
}

bool CSystemTray::ModifyBalloonText(LPCTSTR szTitle, LPCTSTR szInfo)
{
	ASSERT(m_nidSystemTray.hWnd != NULL);
	_tcscpy(m_nidSystemTray.szInfoTitle, szTitle);
	_tcscpy(m_nidSystemTray.szInfo, szInfo);
	m_nidSystemTray.hIcon = m_hIconBalloon;
	m_nidSystemTray.uFlags = NIF_INFO | NIF_ICON;
	BOOL bRet = Shell_NotifyIcon(NIM_MODIFY, &m_nidSystemTray);
	if (bRet)
	{
		m_nidSystemTray.hIcon = m_hIconTray;
		m_nidSystemTray.uFlags = NIF_ICON;
		return MKBOOL(Shell_NotifyIcon(NIM_MODIFY, &m_nidSystemTray));
	}
	else return false;
}

bool CSystemTray::Create(HWND hParent, HICON hIcon, LPCTSTR szToolTip)
{
	m_hParent = hParent;
	m_nidSystemTray.cbSize = sizeof(NOTIFYICONDATA);
	m_nidSystemTray.hWnd = m_hParent;
	m_nidSystemTray.hIcon = m_hIconTray = hIcon;

	m_nidSystemTray.uFlags = NIF_ICON | NIF_MESSAGE;
	m_nidSystemTray.uCallbackMessage = WM_USER_TRAY_MSG;
	if (szToolTip)
	{
		m_nidSystemTray.uFlags |= NIF_TIP;
		_tcscpy(m_nidSystemTray.szTip, szToolTip);
	}
	return MKBOOL(Shell_NotifyIcon(NIM_ADD, &m_nidSystemTray));
}

bool CSystemTray::ModifyIcon(HICON hIcon)
{
	ASSERT(m_nidSystemTray.hWnd != NULL);
	m_nidSystemTray.hIcon = hIcon;
	m_nidSystemTray.uFlags = NIF_ICON;
	return MKBOOL(Shell_NotifyIcon(NIM_MODIFY, &m_nidSystemTray));
}

bool CSystemTray::ModifyToolTipText(LPCTSTR szToolTip)
{
	ASSERT(m_nidSystemTray.hWnd != NULL);
	_tcscpy(m_nidSystemTray.szTip, szToolTip);
	m_nidSystemTray.uFlags = NIF_TIP;
	return MKBOOL(Shell_NotifyIcon(NIM_MODIFY, &m_nidSystemTray));
}

void CSystemTray::ShowWebMessage(HICON hIcon)
{
	long newTime = static_cast<long>(time(NULL));
	if(newTime - m_lLastTipTime >= 60 * 5)
	{
		ModifyBalloonText(_T("VenusMessenger"), _T("打开VenusMessenger查看消息!"));
		ModifyBalloonIcon(hIcon);
	}
	m_lLastTipTime = newTime;
}