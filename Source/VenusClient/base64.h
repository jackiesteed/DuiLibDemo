#ifndef	__BASE64__H__
#define __BASE64__H__


int base64_encode( const unsigned char* bytes_to_encode, int in_len, char* crypt);
int base64_decode(char* encoded_string, unsigned char* plain);

#endif /*_BASE64__H_*/
