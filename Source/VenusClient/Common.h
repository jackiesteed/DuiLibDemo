#ifndef __COMMON_H__
#define __COMMON_H__
#include "StdAfx.h"

#define MKBOOL(_a) ((_a) != 0)  

int VenusWideCharToMultiByte(LPCWSTR lpWideCharStr, LPSTR lpMultiByteStr);
int VenusMultiByteToWideChar(LPCSTR lpMultiByteStr, LPWSTR lpWideCharStr);
void VenusMessageBox(LPCTSTR lpText);
int VenusMessageBoxA(LPSTR lpText);
bool ANSIToUTF8(char* src, char* dest);
bool UTF8ToANSI(char* src, char* dest);
bool CheckForUpdate();
bool InitSocket();
bool ClearSocket();
CString TeaBase64Encrypt(CString plaintext);
CString TeaBase64Decrypt(CString cipher);
bool IsSubStr(char pat[], char text[]);
BOOL CALLBACK EnumWindowsProc(_In_  HWND hwnd, _In_  LPARAM lParam);
bool CheckForExistInstance(bool bFromWeb);
bool NeedUpdate();
void GetAppDataPath(LPTSTR lpszPath);
void GetAppDataPathA(LPSTR lpszPath);
int ReverseFind(LPCTSTR szText, TCHAR c);
void AnimateShowWindow(HWND hwnd, bool bShow);
BOOL UrlEncode(const char* szSrc, char* pBuf, int cbBufLen, BOOL bUpperCase = true);
BOOL UrlDecode(const char* szSrc, char* pBuf, int cbBufLen);

void Pin2Taskbar(HINSTANCE hInst, bool bPin = false);

#endif