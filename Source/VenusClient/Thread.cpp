#include "StdAfx.h"
#include "Thread.h"


CThread::CThread(): m_hThread(NULL)
{

}


CThread::~CThread()
{
	if(m_hThread)
	{
		::CloseHandle(m_hThread);
		m_hThread = NULL;
	}
}


unsigned CThread::ThreadProc( void* pArgs)
{

	CThread* pthread = static_cast<CThread*>(pArgs);
	unsigned ret = pthread->Run();

	//<By Jackie 2013-6-8>的线程类, 是在heap上申请的, 我们在这里delete掉.
	// 如果声明为局部变量, 那么就会出现提前析构的情况.
	delete pthread;
	return ret;
}

void CThread::Start()
{
	assert(m_hThread == NULL);
	unsigned threadId = 0;
	m_hThread = reinterpret_cast<HANDLE>(::_beginthreadex(NULL, 0, CThread::ThreadProc, this, 0, &threadId));
}

unsigned CThread::Run()
{
	//<By Jackie 2013-6-8>在继承类中扩展这个函数.
	return 0;
}
