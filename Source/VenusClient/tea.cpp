#include "StdAfx.h"
#include "tea.h"
#include <cstring> //for memcpy,memset
#include <cmath>
using namespace std;

inline double logbase(double base, double x)
{
    return log(x) / log(base);
}

TEA::TEA(const byte* key, int round /*= 32*/)
    :
    _round(round)
{
    if (key != 0)
        memcpy(_key, key, 16);
    else
        memset(_key, 0, 16);

}

TEA::TEA(const TEA& rhs)
    :
    _round(rhs._round)
{
    memcpy(_key, rhs._key, 16);
}

TEA& TEA::operator=(const TEA& rhs)
{
    if (&rhs != this)
    {
        _round = rhs._round;
        memcpy(_key, rhs._key, 16);

    }
    return *this;

}

int TEA::encrypt(const byte* in, byte* out, int in_len)
{
    byte* p = const_cast<byte*>(in);
    while(p - in < in_len)
    {
        encrypt((const int*)p, (int*)out);
        p += 8;
        out += 8;
    }
    return p - in;
}

int TEA::decrypt(const byte* in, byte* out, int in_len)
{
    byte* p = const_cast<byte*>(in);
    while(p - in < in_len)
    {
        decrypt((const int*)p, (int*)out);
        p += 8;
        out += 8;
    }
    return p - in;
}

void TEA::encrypt(const int* in, int* out)
{
    int* k = (int*)_key;
    register int y = ntoh(in[0]);
    register int z = ntoh(in[1]);
    register int a = ntoh(k[0]);
    register int b = ntoh(k[1]);
    register int c = ntoh(k[2]);
    register int d = ntoh(k[3]);
    register int delta = 0x9E3779B9; /* (sqrt(5)-1)/2*2^32 */
    register int round = _round;
    register int sum = 0;


    while (round--)      /* basic cycle start */
    {
        sum += delta;
        y += ((z << 4) + a) ^ (z + sum) ^ ((z >> 5) + b);
        z += ((y << 4) + c) ^ (y + sum) ^ ((y >> 5) + d);

    }    /* end cycle */
    out[0] = ntoh(y);
    out[1] = ntoh(z);

}

void TEA::decrypt(const int* in, int* out)
{
    int* k = (int*)_key;
    register int y = ntoh(in[0]);
    register int z = ntoh(in[1]);
    register int a = ntoh(k[0]);
    register int b = ntoh(k[1]);
    register int c = ntoh(k[2]);
    register int d = ntoh(k[3]);
    register int delta = 0x9E3779B9; /* (sqrt(5)-1)/2*2^32 */
    register int round = _round;
    register int sum = 0;

    if (round == 32)
        sum = 0xC6EF3720; /* delta << 5*/
    else if (round == 16)
        sum = 0xE3779B90; /* delta << 4*/
    else
        sum = delta << static_cast<int>(logbase(2, round));

    while (round--)      /* basic cycle start */
    {
        z -= ((y << 4) + c) ^ (y + sum) ^ ((y >> 5) + d);
        y -= ((z << 4) + a) ^ (z + sum) ^ ((z >> 5) + b);
        sum -= delta;
    }    /* end cycle */
    out[0] = ntoh(y);
    out[1] = ntoh(z);

}