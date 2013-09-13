#ifndef __HTTP_MANAGER_H__
#define __HTTP_MANAGER_H__

#include "Singleton.h"
#include "curl/curl.h"
#include "Thread.h"

//<By Jackie 2013-6-8>��������ṹ��, ��Ҫ��������url���ݸ��߳�, ���߳�ִ��, Ȼ��
typedef struct tagCurlData
{
	char* url;					/*�������Ҫʹ�õ�url��.*/
	char* data;					/*data��post������*/
	char *buffer;               /* buffer to store cached data*/
	size_t buffer_len;          /* currently allocated buffers length */
	size_t buffer_pos;          /* end of data in buffer*/

	void Init()
	{
		url = NULL;
		data = NULL;
		buffer = NULL;
		buffer_len = buffer_pos = 0;
	}

	//<By Jackie 2013-6-25>���������delete this, ���Ա����������õĺ���!!!
	void Clear()
	{
		if(url) 
		{
			delete []url;
			url = NULL;
		}
		if(data)
		{
			delete []data;
			data = NULL;
		}
		if(buffer)
		{
			delete []buffer;
			buffer = NULL;
		}
		delete this;
	}

}CurlData;


class HttpManagerProto
{
public:
	HttpManagerProto();
	~HttpManagerProto();

public:
	bool DoHttpPost(CurlData* pdata);
	bool DoHttpGet(CurlData* pdata);
	bool CurlGlobalInit();


private:
	CURL* CurlInit();
	static size_t WriteFunction( void* ptr, size_t size, size_t nmemb, void *usrptr );

	DECLARE_SINGLETON_CLASS(HttpManagerProto);
};

typedef Singleton<HttpManagerProto> HttpManager;


#endif