#ifndef __THREAD_MANAGER_H__
#define __THREAD_MANAGER_H__

#include "process.h"

#define CLASS_UNCOPYABLE(classname) \
		private: \
		classname(const classname&); \
		classname& operator=(const classname&);

class CThread 
{
	CLASS_UNCOPYABLE(CThread)
public:
	CThread();
	virtual ~CThread();
	virtual void Start();
	virtual unsigned Run();

private:
	HANDLE m_hThread;
	static unsigned __stdcall ThreadProc( void* pArgs);

};
#endif