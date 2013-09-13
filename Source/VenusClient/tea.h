#ifndef __TEA_H__
#define __TEA_H__

#include <winsock2.h>
typedef unsigned long ulong;
class TEA
{
public:
	TEA(const byte *key, int round = 32);
	TEA(const TEA &rhs);
	TEA &operator=(const TEA &rhs);
	int encrypt(const byte *in, byte *out, int in_len);
	int decrypt(const byte *in, byte *out, int in_len);
private:
	void encrypt(const int *in, int *out);
	void decrypt(const int *in, int *out);
	ulong ntoh(ulong netlong)
	{
		return ::ntohl(netlong);
		//return netlong;
	}
	ulong hton(ulong hostlong)
	{
		return ::htonl(hostlong);
		//return hostlong;
	}
private:
	int _round; //iteration round to encrypt or decrypt
	byte _key[16]; //encrypt or decrypt key
};

#endif/*TEA_H*/