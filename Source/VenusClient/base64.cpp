/*

Base64��һ��ʹ��64����λ�ü�������
��ʹ��2�����η���������ɴ�ӡ��ASCII �ַ���
��ʹ����������Ϊ�����ʼ��Ĵ�����롣
��Base64�еı���ʹ���ַ�A-Z��a-z��0-9 ����������62���ַ���������Ϊ��ʼ��64�����֣��������������Ϊ���ֵķ����ڲ�ͬ��ϵͳ�ж���ͬ��
һЩ��uuencode���������뷽������֮��binhex�İ汾ʹ�ò�ͬ��64�ַ���������6�����������֣��������ǲ���Base64��

��MIME��ʽ�ĵ����ʼ��У�base64����������binary���ֽ��������ݱ����ASCII�ַ����й��ɵ��ı���
ʹ��ʱ���ڴ�����뷽ʽ��ָ��base64��
ʹ�õ��ַ�������Сд��ĸ��26��������10�����֣��ͼӺš�+����б�ܡ�/����һ��64���ַ����Ⱥš�=��������Ϊ��׺��;��

������base64����ɼ� RFC 1421�� RFC 2045��
���������ݱ�ԭʼ�����Գ���Ϊԭ����4/3��
�ڵ����ʼ��У�����RFC 822�涨��ÿ76���ַ�������Ҫ����һ���س����С�
���Թ����������ݳ��ȴ�ԼΪԭ����135.1%��

ת����ʱ�򣬽�����byte�����ݣ��Ⱥ����һ��24bit�Ļ������У�������byteռ��λ��
���ݲ���3byte�Ļ����ڻ�������ʣ�µ�bit��0���㡣
Ȼ��ÿ��ȡ��6����Ϊ26 = 64����bit��������ֵѡ��ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/�е��ַ���Ϊ�����������
���Ͻ��У�ֱ��ȫ����������ת����ɡ�

������ʣ�������������ݣ��ڱ��������1����=����������ʣ��һ���������ݣ����������2����=�������û��ʣ���κ����ݣ���ʲô����Ҫ�ӣ������ſ��Ա�֤���ϻ�ԭ����ȷ�ԡ�

������˵��һ��������Thomas Hobbes's Leviathan���ľ䣺

Man is distinguished, not only by his reason, but by this singular passion from other animals, which is a lust of the mind, that by a perseverance of delight in the continued and indefatigable generation of knowledge, exceeds the short vehemence of any carnal pleasure.

����base64����֮���ɣ�

TWFuIGlzIGRpc3Rpbmd1aXNoZWQsIG5vdCBvbmx5IGJ5IGhpcyByZWFzb24sIGJ1dCBieSB0aGlz
IHNpbmd1bGFyIHBhc3Npb24gZnJvbSBvdGhlciBhbmltYWxzLCB3aGljaCBpcyBhIGx1c3Qgb2Yg
dGhlIG1pbmQsIHRoYXQgYnkgYSBwZXJzZXZlcmFuY2Ugb2YgZGVsaWdodCBpbiB0aGUgY29udGlu
dWVkIGFuZCBpbmRlZmF0aWdhYmxlIGdlbmVyYXRpb24gb2Yga25vd2xlZGdlLCBleGNlZWRzIHRo
ZSBzaG9ydCB2ZWhlbWVuY2Ugb2YgYW55IGNhcm5hbCBwbGVhc3VyZS4=


����

���롰Man��
��M����ASCII�a =  77 = 01001101
��a���� =  97 = 01100001
��n���� = 110 = 01101110


���������ֽ�ƴ�ϣ��ó�һ��24λ�����ϣ�

010011010110000101101110


��������һ��ķֿ���������õ�������������Щ��תΪ��

010011 = 19 = T ��T�ǵ�20��Ӣ����ĸ������0��ʼ������
010110 = 22 = W ��W�ǵ�23��Ӣ����ĸ������0��ʼ������
000101 =  5 = F ��F�ǵ�6��Ӣ����ĸ������0��ʼ������
101110 = 46 = u ��U�ǵ�21��Ӣ����ĸ������0��ʼ������


base64�����ǣ�
00010011 00010110 00000101 00101110

����ÿ3��δ�����ֽڣ�������õ�4���ֽڡ�

����M��M=01001101����ɼ���010011010000������һ��ֿ���010011 010000�������TQ��Ȼ���ں����������=����������ǡ�TQ==����

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


		//<By�ŷ�ѩ>2013-3-27 , ���ﱣ֤����һһ��Ӧ��.
		for (j = 0; (j < i - 1); j++) plain[plain_len++] = char_array_3[j];
	}
	plain[plain_len] = '\0';

	return plain_len;
}


