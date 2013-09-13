#ifndef __HTTP_THREAD_H__
#define __HTTP_THREAD_H__

#include "Thread.h"
#include "HttpManager.h"
class CHttpThread: public CThread
{
public:
	CHttpThread(CurlData* pdata, HWND hwnd, int iMessage);
	virtual ~CHttpThread();

private:
	virtual unsigned Run();
	CurlData* m_pdata;
	HWND m_hUIWnd;
	int m_iMessage; //<By Jackie 2013-6-9> 设定是需要把请求出来的内容发送给主窗口.
};

#endif