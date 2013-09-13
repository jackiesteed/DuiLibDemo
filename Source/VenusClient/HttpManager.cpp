#include "StdAfx.h"
#include "HttpManager.h"


HttpManagerProto::HttpManagerProto()
{

}

HttpManagerProto::~HttpManagerProto()
{

}

//<By Jackie 2013-6-8> call before you use this class.
bool HttpManagerProto::CurlGlobalInit()
{
	CURLcode res = ::curl_global_init( CURL_GLOBAL_ALL );
	if( CURLE_OK != res ) 
	{
		//log here
		return false;
	}
	return true;
}

CURL* HttpManagerProto::CurlInit()
{
	CURL* curl = ::curl_easy_init();
	if(NULL == curl)
	{
		//log here
	}
	return curl;
}

bool HttpManagerProto::DoHttpPost(CurlData* pdata)
{
	CURL* curl = CurlInit();
	//<By Jackie 2013-6-7>这里设置要不要返回头部, 自己看着办.
	::curl_easy_setopt( curl, CURLOPT_HEADER, false );
	::curl_easy_setopt( curl, CURLOPT_URL, pdata->url );
	::curl_easy_setopt( curl, CURLOPT_POST, 1 );
	::curl_easy_setopt( curl, CURLOPT_POSTFIELDS, pdata->data );
	::curl_easy_setopt( curl, CURLOPT_TIMEOUT, 10 );
	::curl_easy_setopt( curl, CURLOPT_WRITEFUNCTION, HttpManagerProto::WriteFunction); 
	::curl_easy_setopt( curl, CURLOPT_WRITEDATA, pdata); 
	CURLcode res = ::curl_easy_perform( curl);
	return res == 0;

}

bool HttpManagerProto::DoHttpGet(CurlData* pdata)
{
	CURL* curl = CurlInit();
	::curl_easy_setopt( curl, CURLOPT_URL, pdata->url );
	::curl_easy_setopt( curl, CURLOPT_HTTPGET, 1 );
	::curl_easy_setopt( curl, CURLOPT_HEADER, false );
	::curl_easy_setopt( curl, CURLOPT_TIMEOUT, 10 );
	::curl_easy_setopt( curl, CURLOPT_WRITEFUNCTION, HttpManagerProto::WriteFunction); 
	::curl_easy_setopt( curl, CURLOPT_WRITEDATA, pdata); 

	CURLcode res = ::curl_easy_perform( curl);
	return ( 0 == res ) ;
}


size_t HttpManagerProto::WriteFunction( void* ptr, size_t size, size_t nmemb, void *usrptr )
{
	CurlData* pdata = static_cast<CurlData*>(usrptr);

	char *newbuff;
	size_t rembuff;

	size *= nmemb;

	rembuff=pdata->buffer_len - pdata->buffer_pos; /* remaining space in buffer */

	if(size > rembuff) 
	{
		/* not enough space in buffer */
		newbuff = static_cast<char*>(::realloc(pdata->buffer,pdata->buffer_len + (size - rembuff) + 1) );
		if(newbuff==NULL) 
		{
			//log here
			size=rembuff;
		}
		else 
		{
			/* realloc suceeded increase buffer size*/
			pdata->buffer_len+=size - rembuff;
			pdata->buffer=newbuff;
		}
	}

	memcpy(&pdata->buffer[pdata->buffer_pos], ptr, size);
	pdata->buffer_pos += size;
	pdata->buffer[pdata->buffer_pos] = '\0';
	return size;
}

