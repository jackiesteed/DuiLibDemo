#ifndef __LOGIN_FRAME_H__
#define __LOGIN_FRAME_H__

class CMainFrame;

class CLoginFrame : public CWindowWnd, public INotifyUI, public IDialogBuilderCallback
{
public:
	CLoginFrame(CMainFrame* pMainFrame);
	~CLoginFrame();
	LPCTSTR GetWindowClassName() const
	{
		return _T("UILoginFrame");
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
	CPaintManagerUI* GetPaintManager(){return m_pPaintManager;}
	CControlUI* CreateControl(LPCTSTR){return NULL;}

	//handlers for windows message
	LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnSize( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled );
	LRESULT OnNcActivate(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& bHandled);
	LRESULT OnNcHitTest(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnNcPaint(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnNcCalcSize(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	//handlers for notify.
	void OnClick(TNotifyUI& msg);
	

private:
	int Login(CString strUserName, CString strPassword);
	CMainFrame* m_pMainFrame;
	CPaintManagerUI* m_pPaintManager;

};



#endif