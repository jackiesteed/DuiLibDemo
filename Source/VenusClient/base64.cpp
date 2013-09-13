/*

Base64是一种使用64基的位置计数法。
它使用2的最大次方来代表仅可打印的ASCII 字符。
这使它可用来作为电子邮件的传输编码。
在Base64中的变量使用字符A-Z、a-z和0-9 ，这样共有62个字符，用来作为开始的64个数字，最后两个用来作为数字的符号在不同的系统中而不同。
一些如uuencode的其他编码方法，和之后binhex的版本使用不同的64字符集来代表6个二进制数字，但是它们不叫Base64。

在MIME格式的电子邮件中，base64可以用来将binary的字节序列数据编码成ASCII字符序列构成的文本。
使用时，在传输编码方式中指定base64。
使用的字符包括大小写字母各26个，加上10个数字，和加号“+”，斜杠“/”，一共64个字符，等号“=”用来作为后缀用途。

完整的base64定义可见 RFC 1421和 RFC 2045。
编码后的数据比原始数据略长，为原来的4/3。
在电子邮件中，根据RFC 822规定，每76个字符，还需要加上一个回车换行。
可以估算编码后数据长度大约为原长的135.1%。

转换的时候，将三个byte的数据，先后放入一个24bit的缓冲区中，先来的byte占高位。
数据不足3byte的话，于缓冲区中剩下的bit用0补足。
然后，每次取出6（因为26 = 64）个bit，按照其值选择ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/中的字符作为编码后的输出。
不断进行，直到全部输入数据转换完成。

如果最后剩下两个输入数据，在编码结果后加1个“=”；如果最后剩下一个输入数据，编码结果后加2个“=”；如果没有剩下任何数据，就什么都不要加，这样才可以保证资料还原的正确性。

举例来说，一段引用自Thomas Hobbes's Leviathan的文句：

Man is distinguished, not only by his reason, but by this singular passion from other animals, which is a lust of the mind, that by a perseverance of delight in the continued and indefatigable generation of knowledge, exceeds the short vehemence of any carnal pleasure.

经过base64编码之后变成：

TWFuIGlzIGRpc3Rpbmd1aXNoZWQsIG5vdCBvbmx5IGJ5IGhpcyByZWFzb24sIGJ1dCBieSB0aGlz
IHNpbmd1bGFyIHBhc3Npb24gZnJvbSBvdGhlciBhbmltYWxzLCB3aGljaCBpcyBhIGx1c3Qgb2Yg
dGhlIG1pbmQsIHRoYXQgYnkgYSBwZXJzZXZlcmFuY2Ugb2YgZGVsaWdodCBpbiB0aGUgY29udGlu
dWVkIGFuZCBpbmRlZmF0aWdhYmxlIGdlbmVyYXRpb24gb2Yga25vd2xlZGdlLCBleGNlZWRzIHRo
ZSBzaG9ydCB2ZWhlbWVuY2Ugb2YgYW55IGNhcm5hbCBwbGVhc3VyZS4=


例子

编码“Man”
「M」的ASCII碼 =  77 = 01001101
「a」的 =  97 = 01100001
「n」的 = 110 = 01101110


将这三个字节拼合，得出一个24位的资料：

010011010110000101101110


现在六个一组的分开，这样便得到六个数。将这些数转为：

010011 = 19 = T （T是第20個英文字母，但从0开始计数）
010110 = 22 = W （W是第23個英文字母，但从0开始计数）
000101 =  5 = F （F是第6個英文字母，但从0开始计数）
101110 = 46 = u （U是第21個英文字母，但从0开始计数）


base64编码是：
00010011 00010110 00000101 00101110

即是每3个未编码字节，编码后会得到4个字节。

加密M：M=01001101，变成加密010011010000，六个一组分开是010011 010000，结果是TQ，然后在后面加两个“=”，结果就是“TQ==”。

*/
#include "StdAfx.h"
#include "base64.h"
#include <string>

static const std::string base64_chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

static inline bool is_base64(unsigned char c)
{
	return (isalnum(c) || (c == '+') || (c == '/'));
}

int base64_encode( const unsigned char *bytes_to_encode, int in_len, char *crypt)
{
	int i = 0;
	int j = 0;
	unsigned char char_array_3[3];
	unsigned char char_array_4[4];

	int crypt_len = 0;

	while (in_len--)
	{
		char_array_3[i++] = *(bytes_to_encode++);
		if (i == 3)
		{
			char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
			char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
			char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
			char_array_4[3] = char_array_3[2] & 0x3f;

			for (i = 0; (i < 4) ; i++)
				crypt[crypt_len++] = base64_chars[char_array_4[i]];
			i = 0;
		}
	}

	if (i)
	{
		for (j = i; j < 3; j++)
			char_array_3[j] = '\0';

		char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
		char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
		char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
		char_array_4[3] = char_array_3[2] & 0x3f;

		for (j = 0; (j < i + 1); j++)
			crypt[crypt_len++] = base64_chars[char_array_4[j]];

		while ((i++ < 3))
			crypt[crypt_len++] = '=';

	}
	crypt[crypt_len] = '\0';

	return crypt_len;

}

int base64_decode(char *encoded_string, unsigned char *plain)
{
	int in_len = strlen(encoded_string);
	int i = 0;
	int j = 0;
	int in_ = 0;
	unsigned char char_array_4[4], char_array_3[3];

	int plain_len = 0;

	while (in_len-- && ( encoded_string[in_] != '=') && is_base64(encoded_string[in_]))
	{
		char_array_4[i++] = encoded_string[in_];
		in_++;
		if (i == 4)
		{
			for (i = 0; i < 4; i++)
				char_array_4[i] = (unsigned char)base64_chars.find(char_array_4[i]);

			char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
			char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
			char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

			for (i = 0; (i < 3); i++)
				plain[plain_len++] = char_array_3[i];
			i = 0;
		}
	}

	if (i)
	{
		for (j = i; j < 4; j++)
			char_array_4[j] = 0;

		for (j = 0; j < 4; j++)
			char_array_4[j] = (unsigned char)base64_chars.find(char_array_4[j]);

		char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
		char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
		char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];


		//<By张方雪>2013-3-27 , 这里保证了是一一对应的.
		for (j = 0; (j < i - 1); j++) plain[plain_len++] = char_array_3[j];
	}
	plain[plain_len] = '\0';

	return plain_len;
}


