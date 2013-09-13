#ifndef __BROWSER_FRAME_H__
#define __BROWSER_FRAME_H__

#include "MetaDefine.h"

class CBrowserFrame
{
public:
	CBrowserFrame();
	~CBrowserFrame();

	void Create(HWND hwndParent,  DWORD dwStyle, DWORD dwExStyle, int x = 0, int y = 0, int cx = 0, int cy  = 0, HMENU hMenu = NULL);
	void Create(HWND hwndParent,  DWORD dwStyle, DWORD dwExStyle, const RECT rc, HMENU hMenu = NULL);
	void ShowPage(char url[]);
	HWND GetHwnd(){return m_hwnd;}
private:
	HWND m_hwnd;
};


LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);




#endif