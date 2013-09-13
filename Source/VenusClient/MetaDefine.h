#ifndef __VENUS_DEFINE_H__
#define __VENUS_DEFINE_H__

const int STR_SIZE =		512;
const int CONTENT_SIZE =	256;
const int MAX_USER_COUNT = 1000;
const int MAX_FLOW_COUNT =	100;
const int BUF_SIZE =		4096;
const int MINI_SIZE =		64;

const int WM_USER_TRAY_MSG = WM_USER + (1);


typedef struct tagFlow
{
	int id;
	char name[MINI_SIZE];
	char flowCode[MINI_SIZE];
}Flow;

typedef struct tagGlobalMeta
{
	HINSTANCE hInst;
	HWND hwndMainFrame;
	HWND hwndBrowser;

}GlobalMeta;

extern GlobalMeta g_meta;
extern UINT WM_TASKBARCREATED;

#endif