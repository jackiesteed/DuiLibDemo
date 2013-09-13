#ifndef __SYSTEM_TRAY_H__
#define __SYSTEM_TRAY_H__
#include <ShellAPI.h>
#include <ctime>
#include "Common.h"

using namespace std;

class CSystemTray
{
public:
	CSystemTray();
	~CSystemTray();
	void Destroy();
	bool Create(HWND hParent, HICON hIcon, LPCTSTR szToolTip);  //hParent 是接收WM_TRAY_MSG消息的窗口
	bool ModifyIcon(HICON hIcon);
	bool ModifyToolTipText(LPCTSTR szToolTip);
	bool ModifyBalloonText(LPCTSTR szTitle, LPCTSTR szInfo);
	bool ModifyBalloonIcon(HICON hIcon);
	bool SetFocus();
	void ShowWebMessage(HICON hIcon);

private:
	void Wait(int nSeconds);
	HWND m_hParent;
	HICON m_hIconTray, m_hIconBalloon;
	NOTIFYICONDATA m_nidSystemTray;
	long m_lLastTipTime; //<By张方雪 2013-6-4>这个专门用于来自web的启动消息.
};
#endif