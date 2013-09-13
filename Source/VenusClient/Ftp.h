#ifndef FTP_H_
#define FTP_H_
#include <WinSock2.h>

class CFTP
{
public:
	CFTP(void);
	~CFTP(void);
	//����ftp������
	int ftp_connect(const char *ip);
	//��¼ftp������
	int ftp_login(char *user, char *pass);
	//��ʾ��ǰĿ¼
	int ftp_pwd(char *buff);
	//����Ŀ¼
	int ftp_cd(char *dir);
	//�����ϲ�Ŀ¼
	int ftp_cdup();
	//����Ŀ¼
	int ftp_mkdir(char *dir);
	//ɾ��Ŀ¼
	int ftp_rmdir(char *dir);
	//���ݴ���ģʽ
	int ftp_setpasv();
	//�ϴ��ļ�
	int ftp_upload(char *localfile, char *remotepath, char *remotefilename);
	//�����ļ�
	int ftp_download(char *localfile, char *remotefile);
	//�˳���¼
	int ftp_quit();

	int ftp_setbinary();

private:
	int m_sockctrl;//��������socket
	int m_sockdata;//��������socket
	char m_cmd[256];//���ָ��
	char m_resp[256];//��ŷ������
	char m_ip[64];//����ip

	int ftp_sendcmd();//����ָ��
	int ftp_checkresp(char expresp);//���շ���״̬������Ƿ�ɹ�
	int ftp_mkdirSingle(char *dir);
};
#endif
