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

	//<By Jackie 2013-6-8>���߳���, ����heap�������, ����������delete��.
	// �������Ϊ�ֲ�����, ��ô�ͻ������ǰ���������.
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
	//<By Jackie 2013-6-8>�ڼ̳�������չ�������.
	return 0;
}
