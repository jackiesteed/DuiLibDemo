#ifndef __SERVER_MANAGER_H__
#define __SERVER_MANAGER_H__

#include "MetaDefine.h"

//<By Jackie 2013-9-11> ����ҵ����http���󶼴������.
class CServerManager
{
public:
	CServerManager();
	~CServerManager();
public:
	void GetFlowList(vector<Flow>& flowList);
	int Login(CString userName, string password);

};


#endif