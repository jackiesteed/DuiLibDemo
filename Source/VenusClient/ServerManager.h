#ifndef __SERVER_MANAGER_H__
#define __SERVER_MANAGER_H__

#include "MetaDefine.h"

//<By Jackie 2013-9-11> 所有业务层的http请求都从这里出.
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