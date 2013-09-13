#ifndef FTP_H_
#define FTP_H_
#include <WinSock2.h>

class CFTP
{
public:
	CFTP(void);
	~CFTP(void);
	//连接ftp服务器
	int ftp_connect(const char *ip);
	//登录ftp服务器
	int ftp_login(char *user, char *pass);
	//显示当前目录
	int ftp_pwd(char *buff);
	//更改目录
	int ftp_cd(char *dir);
	//返回上层目录
	int ftp_cdup();
	//创建目录
	int ftp_mkdir(char *dir);
	//删除目录
	int ftp_rmdir(char *dir);
	//数据传输模式
	int ftp_setpasv();
	//上传文件
	int ftp_upload(char *localfile, char *remotepath, char *remotefilename);
	//下载文件
	int ftp_download(char *localfile, char *remotefile);
	//退出登录
	int ftp_quit();

	int ftp_setbinary();

private:
	int m_sockctrl;//控制连接socket
	int m_sockdata;//数据连接socket
	char m_cmd[256];//存放指令
	char m_resp[256];//存放返回语句
	char m_ip[64];//保存ip

	int ftp_sendcmd();//发送指令
	int ftp_checkresp(char expresp);//接收返回状态，检测是否成功
	int ftp_mkdirSingle(char *dir);
};
#endif
