#ifndef __MAIN_FRAME_H__
#define __MAIN_FRAME_H__

#include "TreeViewManager.h"
#include "ListViewManager.h"
#include "LoginFrame.h"
#include "BrowserFrame.h"
#include "SystemTray.h"

// 窗口实例及消息响应部分
class CMainFrame : public CWindowWnd, public INotifyUI, public IDialogBuilderCallback
{
public:
	CMainFrame();
	~CMainFrame();
	LPCTSTR GetWindowClassName() const
	{
		return _T("UIMainFrame");
	};
	UINT GetClassStyle() const
	{
		return UI_CLASSSTYLE_FRAME | CS_DBLCLKS;
	};
	void OnFinalMessage(HWND /*hWnd*/)
	{
		delete this;
	};
	void InitWindow();
	void OnPrepare();
	void Notify(TNotifyUI& msg);
	LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
	LRESULT HandleUserMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	CControlUI* CreateControl(LPCTSTR){return NULL;}
	CPaintManagerUI* GetPaintManager(){return m_pPaintManager;}
	void ShowLoginFrame();
	void TryCreateWindow();

	//handlers for windows message
	LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnSize( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled );
	LRESULT OnNcActivate(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& bHandled);
	LRESULT OnNcHitTest(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnNcPaint(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnNcCalcSize(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnGetMinMaxInfo(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);


	//handlers for notify.
	void OnClick(TNotifyUI& msg);

private:
	CPaintManagerUI* m_pPaintManager;
	CTreeViewManager* m_pTreeViewManager;
	CListViewManager* m_pListViewManager;
	CLoginFrame* m_pLoginFrame;
	CBrowserFrame* m_pBrowserFrame;
	bool m_bCreated;
	CSystemTray* m_psysTray;
	HICON m_hIcon;
};




#endif