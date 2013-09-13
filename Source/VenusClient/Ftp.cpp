#include "stdafx.h"
#include <iostream>
#include <WinSock2.h>
#include "Common.h"
#include "FTP.h"

using namespace std;
#pragma comment(lib, "Ws2_32.lib")

CFTP::CFTP(void)
{
}

CFTP::~CFTP(void)
{

}

int CFTP::ftp_checkresp(char expresp)
{
	int len = recv(m_sockctrl, m_resp, 256, 0);
	if(-1 == len)return -1;
	m_resp[len] = 0;
	if(m_resp[0] != expresp)return -1;
	return 0;
}

int CFTP::ftp_sendcmd()
{
	int ret = send(m_sockctrl, m_cmd, strlen(m_cmd), 0);
	if(-1 == ret)return -1;
	return 0;
}

int CFTP::ftp_connect(const char *ip)
{
	m_sockctrl = socket(AF_INET, SOCK_STREAM, 0);
	if(0 == m_sockctrl)return -1;
	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(21);
	addr.sin_addr.s_addr = inet_addr(ip);
	int err = connect(m_sockctrl, (sockaddr *)&addr, sizeof(addr));
	if(err)return -1;
	err = ftp_checkresp('2');
	if(err)return -1;
	return 0;
}

int CFTP::ftp_login(char *user, char *pass)
{
	sprintf(m_cmd, "USER %s\r\n", user);
	int err = ftp_sendcmd();
	if(err)return -1;
	err = ftp_checkresp('3');
	if(err)return -1;
	sprintf(m_cmd, "PASS %s\r\n", pass);
	err = ftp_sendcmd();
	if(err)return -1;
	err = ftp_checkresp('2');
	if(err)return -1;
	return 0;
}

int CFTP::ftp_quit()
{
	sprintf(m_cmd, "QUIT\r\n");
	int err = ftp_sendcmd();
	if(err)return -1;
	err = ftp_checkresp('2');
	if(err)return -1;
	return 0;
}
int CFTP::ftp_cd(char *dir)
{
	sprintf(m_cmd, "CWD %s\r\n", dir);
	int err = ftp_sendcmd();
	if(err)return -1;
	err = ftp_checkresp('2');
	if(err)return -1;
	return 0;
}

int CFTP::ftp_cdup()
{
	sprintf(m_cmd, "CDUP\r\n");
	int err = ftp_sendcmd();
	if(err)return -1;
	err = ftp_checkresp('2');
	if(err)return -1;
	return 0;
}

int CFTP::ftp_pwd(char *buff)
{
	sprintf(m_cmd, "PWD\r\n");
	int err = ftp_sendcmd();
	if(err)return -1;
	err = ftp_checkresp('2');
	if(err)return -1;
	char *p = m_resp;
	while(*p)
	{
		if(*p++ == '"')
			while(*p != '"')
				*buff++ = *p++;
	}
	*buff = 0;
	printf("current work directory is : %s\n", buff);
	return 0;
}
int CFTP::ftp_mkdirSingle(char *dir)
{
	sprintf(m_cmd, "MKD %s\r\n", dir);
	int err = ftp_sendcmd();
	if(err)return -1;
	err = ftp_checkresp('2');
	if(err)return -1;
	return 0;
}
int CFTP::ftp_mkdir(char *dir)
{
	char path[300];
	int err = ftp_cd("/");
	if(err)return -1;
	int i, j;
	//	printf("strlen(dir):%d",strlen(dir));
	for(i = 1, j = 0; i < (int)strlen(dir); i++)	//第一个字节是根目录
	{
		path[j++] = dir[i];
		if(dir[i] == '/')
		{
			path[j++] = '\0';
			printf("create :%s\n", path);
			err = ftp_mkdirSingle(path);
			err = ftp_cd(path);
			if(err)return -1;
			j = 0;
		}
	}
	path[j++] = '\0';
	sprintf(m_cmd, "MKD %s\r\n", path);
	err = ftp_sendcmd();
	if(err)return -1;
	err = ftp_checkresp('2');
	if(err)return -1;
	return 0;
}

int CFTP::ftp_rmdir(char *dir)
{
	sprintf(m_cmd, "RMD %s\r\n", dir);
	int err = ftp_sendcmd();
	if(err)return -1;
	err = ftp_checkresp('2');
	if(err)return -1;
	return 0;
}

int CFTP::ftp_upload(char *localfile, char *remotepath, char *remotefilename)
{
	int err  = ftp_setbinary();
	if(err) return -1;
	ftp_mkdir(remotepath);
	err = ftp_cd(remotepath);
	if(err)return -1;
	ftp_setpasv();
	sprintf(m_cmd, "STOR %s\r\n", remotefilename);
	err = ftp_sendcmd();
	if(err)return -1;
	err = ftp_checkresp('1');
	if(err)return -1;
	FILE *pf = fopen(localfile, "r");
	if(NULL == pf)return -1;
	char sendbuf[256];
	size_t len = 0;
	while((len = fread(sendbuf, 1, 255, pf)) > 0)
	{
		err = send(m_sockdata, sendbuf, len, 0);
		if(err < 0)return -1;
	}
	closesocket(m_sockdata);
	fclose(pf);
	err = ftp_checkresp('2');
	if(err)return -1;
	return 0;
}

int CFTP::ftp_setpasv()
{
	sprintf(m_cmd, "PASV\r\n");
	int err = ftp_sendcmd();
	if(err)return -1;
	err = ftp_checkresp('2');
	if(err)return -1;
	m_sockdata = socket(AF_INET, SOCK_STREAM, 0);
	unsigned int v[6];
	union
	{
		struct sockaddr sa;
		struct sockaddr_in in;
	} sin;
	sscanf(m_resp, "%*[^(](%u,%u,%u,%u,%u,%u", &v[2], &v[3], &v[4], &v[5], &v[0], &v[1]);
	sin.sa.sa_family = AF_INET;
	sin.sa.sa_data[2] = v[2];
	sin.sa.sa_data[3] = v[3];
	sin.sa.sa_data[4] = v[4];
	sin.sa.sa_data[5] = v[5];
	sin.sa.sa_data[0] = v[0];
	sin.sa.sa_data[1] = v[1];

	int on = 1;
	if (setsockopt(m_sockdata, SOL_SOCKET, SO_REUSEADDR,
		(const char *) &on, sizeof(on)) == -1)
	{
		perror("setsockopt");
		closesocket(m_sockdata);
		return -1;
	}
	struct linger lng = { 0, 0 };

	if (setsockopt(m_sockdata, SOL_SOCKET, SO_LINGER,
		(const char *) &lng, sizeof(lng)) == -1)
	{
		perror("setsockopt");
		closesocket(m_sockdata);
		return -1;
	}

	err = connect(m_sockdata, (sockaddr *)&sin, sizeof(sin));
	if(err)return -1;
	return 0;
}
int CFTP::ftp_download(char *localfile, char *remotefile)
{
	int err;
	err = ftp_setbinary();
	if(err)
	{
		//VenusMessageBox(_T("Jackiesteed"));
		return -1;
	}
	ftp_setpasv();
	sprintf(m_cmd, "RETR %s\r\n", remotefile);
	err = ftp_sendcmd();
	if(err)return -1;
	err = ftp_checkresp('1');
	if(err)return -1;
	FILE *pf = fopen(localfile, "wb");
	if(NULL == pf)return -1;
	char *recvbuf = new char[1024];
	int len = 0;
	while((len = recv(m_sockdata, recvbuf, 1024, 0)) > 0)
	{
		err = fwrite(recvbuf, len, 1, pf);
		if(len < 0)break;
	}
	closesocket(m_sockdata);
	fclose(pf);
	err = ftp_checkresp('2');
	delete [] recvbuf;
	if(err)return -1;
	return 0;
}

int CFTP::ftp_setbinary()
{
	sprintf(m_cmd, "TYPE I\r\n");
	int err = ftp_sendcmd();
	if(err) return -1;
	err = ftp_checkresp('2');
	if(err)return -1;
	return 0;
}
