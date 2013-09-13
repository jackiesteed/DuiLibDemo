#include "StdAfx.h"
#include "HttpThread.h"


CHttpThread::CHttpThread(CurlData* pdata, HWND hwnd, int iMessage)
{
	m_pdata = pdata;
	m_hUIWnd = hwnd;
	m_iMessage = iMessage;
}
CHttpThread::~CHttpThread()
{}

unsigned CHttpThread::Run()
{
	HttpManager::Instance()->DoHttpGet(m_pdata);
	::PostMessage(m_hUIWnd, m_iMessage, 0, reinterpret_cast<LPARAM>(m_pdata));
	return 0;
}