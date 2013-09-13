#include "StdAfx.h"
#include <dbghelp.h>
#include "Common.h"
#include "MetaDefine.h"
#include <Shellapi.h>
#include <Shlobj.h>
#include <Shlwapi.h>
#include "Ftp.h"
#include "tea.h"
#include "base64.h"
#include <WinUser.h>


int VenusWideCharToMultiByte(LPCWSTR lpWideCharStr, LPSTR lpMultiByteStr)
{
	int len = WideCharToMultiByte(CP_ACP, 0, lpWideCharStr, -1, NULL,  0, NULL, NULL);
	WideCharToMultiByte(CP_ACP, 0, lpWideCharStr, -1, lpMultiByteStr, len, NULL, NULL);
	return len;
}

int VenusMultiByteToWideChar(LPCSTR lpMultiByteStr, LPWSTR lpWideCharStr)
{
	int len = MultiByteToWideChar(CP_ACP, 0, lpMultiByteStr, -1, NULL, NULL);
	MultiByteToWideChar(CP_ACP, 0, lpMultiByteStr, -1, lpWideCharStr, len);
	return len;
}

void VenusMessageBox(LPCTSTR lpText)
{
	MessageBox(NULL, lpText, _T("VenusMessenger"), MB_OK);
}

int VenusMessageBoxA(LPSTR lpText)
{
	return MessageBoxA(NULL, lpText, "VenusMessenger", MB_OK);
}

bool ANSIToUTF8(char* src, char* dest)
{
	int nLen = MultiByteToWideChar(CP_ACP, 0, src, -1, NULL, NULL);
	LPWSTR lpwsz = new WCHAR[nLen];
	MultiByteToWideChar(CP_ACP, 0, src, -1, lpwsz, nLen);
	int nLen1 = WideCharToMultiByte(CP_UTF8, 0, lpwsz, nLen, NULL, NULL, NULL, NULL);
	WideCharToMultiByte(CP_UTF8, 0, lpwsz, nLen, dest, nLen1, NULL, NULL);
	return true;
}
bool UTF8ToANSI(char* src, char* dest)
{
	int nLen = MultiByteToWideChar(CP_UTF8, 0, src, -1, NULL, NULL);
	LPWSTR lpwsz = new WCHAR[nLen];
	MultiByteToWideChar(CP_UTF8, 0, src, -1, lpwsz, nLen);
	int nLen1 = WideCharToMultiByte(CP_ACP, 0, lpwsz, nLen, NULL, NULL, NULL, NULL);
	WideCharToMultiByte(CP_ACP, 0, lpwsz, nLen, dest, nLen1, NULL, NULL);
	return true;
}

bool CheckForUpdate()
{
	PROCESS_INFORMATION ProcessInfo;
	STARTUPINFO StartupInfo; //This is an [in] parameter
	::ZeroMemory(&StartupInfo, sizeof(STARTUPINFO));
	StartupInfo.cb = sizeof StartupInfo ; //Only compulsory field
	if(CreateProcess(_T("Updater.exe"), NULL,
		NULL, NULL, FALSE, 0, NULL,
		NULL, &StartupInfo, &ProcessInfo))
	{
		WaitForSingleObject(ProcessInfo.hProcess, INFINITE);
		CloseHandle(ProcessInfo.hThread);
		CloseHandle(ProcessInfo.hProcess);
	}
	return true;
}

bool InitSocket()
{
	WSADATA wsaData;
	WORD wVersionRequested  = MAKEWORD(2, 2);
	int wsaRet = ::WSAStartup(wVersionRequested, &wsaData);
	return wsaRet == 0;
}

bool ClearSocket()
{
	return 0 == ::WSACleanup();
}


CString TeaBase64Encrypt(CString plaintext)
{
	unsigned char key[] = "$VenusMessenger$";
	TEA tea(key, 32);
	char buf[BUF_SIZE];
	VenusWideCharToMultiByte(plaintext, buf);
	int len = strlen(buf);

	while(len % 8)
	{
		buf[len++] = '\0';
	}

	unsigned char crypt_tea[BUF_SIZE];
	int encrypt_len = tea.encrypt(reinterpret_cast<unsigned char*>(buf), crypt_tea, len);
	crypt_tea[encrypt_len] = '\0';
	char crypt_base64[BUF_SIZE];
	base64_encode(crypt_tea, encrypt_len, crypt_base64);

	TCHAR ret[BUF_SIZE];
	VenusMultiByteToWideChar(crypt_base64, ret);
	return CString(ret);

}
CString TeaBase64Decrypt(CString cipher)
{
	unsigned char key[] = "$VenusMessenger$";
	TEA tea(key, 32);
	char buf[BUF_SIZE];
	VenusWideCharToMultiByte(cipher, buf);
	unsigned char plain_base64[BUF_SIZE];
	int len = base64_decode(buf, plain_base64);
	char plain_tea[BUF_SIZE];
	tea.decrypt(plain_base64, reinterpret_cast<unsigned char*>(plain_tea), len);
	plain_tea[len] = '\0';

	TCHAR ret[BUF_SIZE];
	VenusMultiByteToWideChar(plain_tea, ret);
	return CString(ret);
}

bool IsSubStr(char pat[], char text[])
{
	int len_pat = strlen(pat);
	int len_text = strlen(text);
	if(len_pat > len_text) return false;
	for(int i = 0; i < len_text; i++)
	{
		bool bingo = true;
		for(int j = 0; j < len_pat; j++)
		{
			if(text[i + j] != pat[j])
			{
				bingo = false;
				break;
			}
		}
		if(bingo) return true;
	}
	return false;
}

BOOL CALLBACK EnumWindowsProc(_In_  HWND hwnd, _In_  LPARAM lParam)
{
	return true;
}

bool CheckForExistInstance(bool bFromWeb)
{
	//By张方雪 2013年4月9日
	HANDLE hMutex = ::CreateMutex(NULL, TRUE, _T("$VenusMessenger$"));
	if(NULL == hMutex)
	{
		return false;
	}

	if(hMutex != NULL && ::GetLastError() == ERROR_ALREADY_EXISTS)
	{
		if(bFromWeb)
		{
			HWND hwnd = ::FindWindow(_T("UIMainFrame"), _T("VenusMessenger"));
			if(hwnd)
			{
				::PostMessage(hwnd, 0, 0, 0);
				return true;
			}
		}

		//By张方雪 2013年4月8日 FindWindow能够找到隐藏窗口..
		HWND hwnd = ::FindWindow(_T("UIMainFrame"), _T("VenusMessenger"));
		if(NULL == hwnd) hwnd = ::FindWindow(_T("UILoginWnd"), _T("VenusMessenger"));
		if(hwnd)
		{
			if(::IsIconic(hwnd))
			{
				::ShowWindow(hwnd, SW_RESTORE);
				::BringWindowToTop(hwnd);
				::SetForegroundWindow(hwnd);
			}
			if(!::IsWindowVisible(hwnd))
			{
				ShowWindow(hwnd, SW_HIDE);
				AnimateShowWindow(hwnd, true);
			}
			SetForegroundWindow(hwnd);
			return true;
		}
		return false;
	}
	return false;
}

//<By张方雪 2013/4/16>如果出现下载错误, 就不升级.
bool NeedUpdate()
{
	unsigned short port = 21;
	char ftpIp[100] = "10.12.220.120";
	CFTP ftpclient;
	int err = ftpclient.ftp_connect(ftpIp);
	if(err)
	{
		return false;
	}
	err = ftpclient.ftp_login("setest", "setest");
	if(err)
	{
		return 0;
	}
	err = ftpclient.ftp_cd("Venus");
	if(err)
	{
		return false;
	}
	err = ftpclient.ftp_download("VenusMessengerVersion.ini", "VenusMessengerVersion.ini");
	if(err)
	{
		return false;
	}
	TCHAR szCurrentDir[BUF_SIZE];
	::GetCurrentDirectory(BUF_SIZE, szCurrentDir);
	CString iniFile = CString(szCurrentDir) + _T("\\VenusMessengerVersion.ini");
	TCHAR bufVer[BUF_SIZE];
	::GetPrivateProfileString(_T("VERSION"), _T("CURRENT_VERSION"), NULL, bufVer, BUF_SIZE, iniFile);

	////By张方雪 2013年4月9日 只要版本不相等, 就下载并更新
	//if(0 == _tcscmp(bufVer, ConfigSingleton::Instance()->GetLocalVersion()))
	//{
	//	return false;
	//}
	return true;
}

void GetAppDataPath(LPTSTR lpszPath)
{
	if(SUCCEEDED(::SHGetFolderPath(NULL,
		CSIDL_APPDATA | CSIDL_FLAG_CREATE,
		NULL,
		NULL,
		lpszPath)))
	{
		::PathAppend(lpszPath, _T("VenusMessenger"));
	}
	else
	{
		lpszPath[0] = _T('\0');
	}
}

void GetAppDataPathA(LPSTR lpszPath)
{
	if(SUCCEEDED(::SHGetFolderPathA(NULL,
		CSIDL_APPDATA | CSIDL_FLAG_CREATE,
		NULL,
		NULL,
		lpszPath)))
	{
		::PathAppendA(lpszPath, "VenusMessenger");
	}
	else
	{
		lpszPath[0] = '\0';
	}
}

int ReverseFind(LPCTSTR szText, TCHAR c)
{
	LPCTSTR p = _tcsrchr(szText, c);
	if(p == NULL) return -1;
	return (int)(p - szText);
}

void AnimateShowWindow(HWND hwnd, bool bShow)
{
	if(bShow)
	{
		::AnimateWindow(hwnd, 200, AW_BLEND | AW_ACTIVATE);
		::SetForegroundWindow(hwnd);
	}
	else 
	{
		::AnimateWindow(hwnd, 200, AW_BLEND | AW_HIDE);
	}
}

BOOL UrlEncode(const char* szSrc, char* pBuf, int cbBufLen, BOOL bUpperCase)
{
	if(szSrc == NULL || pBuf == NULL || cbBufLen <= 0)
		return FALSE;

	size_t len_ascii = strlen(szSrc);
	if(len_ascii == 0)
	{
		pBuf[0] = 0;
		return TRUE;
	}

	//先转换到UTF-8
	char baseChar = bUpperCase ? 'A' : 'a';
	int cchWideChar = MultiByteToWideChar(CP_ACP, 0, szSrc, len_ascii, NULL, 0);
	LPWSTR pUnicode = (LPWSTR)malloc((cchWideChar + 1) * sizeof(WCHAR));
	if(pUnicode == NULL)
		return FALSE;
	MultiByteToWideChar(CP_ACP, 0, szSrc, len_ascii, pUnicode, cchWideChar + 1);

	int cbUTF8 = WideCharToMultiByte(CP_UTF8, 0, pUnicode, cchWideChar, NULL, 0, NULL, NULL);
	LPSTR pUTF8 = (LPSTR)malloc((cbUTF8 + 1) * sizeof(CHAR));
	if(pUTF8 == NULL)
	{
		free(pUnicode);
		return FALSE;
	}
	WideCharToMultiByte(CP_UTF8, 0, pUnicode, cchWideChar, pUTF8, cbUTF8 + 1, NULL, NULL);
	pUTF8[cbUTF8] = '\0';

	unsigned char c;
	int cbDest = 0; //累加
	unsigned char *pSrc = (unsigned char*)pUTF8;
	unsigned char *pDest = (unsigned char*)pBuf;
	while(*pSrc && cbDest < cbBufLen - 1)
	{
		c = *pSrc;
		if(isalpha(c) || isdigit(c) || c == '-' || c == '.' || c == '~')
		{
			*pDest = c;
			++pDest;
			++cbDest;
		}
		else if(c == ' ')
		{
			*pDest = '+';
			++pDest;
			++cbDest;
		}
		else
		{
			//检查缓冲区大小是否够用？
			if(cbDest + 3 > cbBufLen - 1)
				break;
			pDest[0] = '%';
			pDest[1] = (c >= 0xA0) ? ((c >> 4) - 10 + baseChar) : ((c >> 4) + '0');
			pDest[2] = ((c & 0xF) >= 0xA)? ((c & 0xF) - 10 + baseChar) : ((c & 0xF) + '0');
			pDest += 3;
			cbDest += 3;
		}
		++pSrc;
	}
	//null-terminator
	*pDest = '\0';
	free(pUnicode);
	free(pUTF8);
	return TRUE;
}

//解码后是utf-8编码
BOOL UrlDecode(const char* szSrc, char* pBuf, int cbBufLen)
{
	if(szSrc == NULL || pBuf == NULL || cbBufLen <= 0)
		return FALSE;

	size_t len_ascii = strlen(szSrc);
	if(len_ascii == 0)
	{
		pBuf[0] = 0;
		return TRUE;
	}

	char *pUTF8 = (char*)malloc(len_ascii + 1);
	if(pUTF8 == NULL)
		return FALSE;

	int cbDest = 0; //累加
	unsigned char *pSrc = (unsigned char*)szSrc;
	unsigned char *pDest = (unsigned char*)pUTF8;
	while(*pSrc)
	{
		if(*pSrc == '%')
		{
			*pDest = 0;
			//高位
			if(pSrc[1] >= 'A' && pSrc[1] <= 'F')
				*pDest += (pSrc[1] - 'A' + 10) * 0x10;
			else if(pSrc[1] >= 'a' && pSrc[1] <= 'f')
				*pDest += (pSrc[1] - 'a' + 10) * 0x10;
			else
				*pDest += (pSrc[1] - '0') * 0x10;

			//低位
			if(pSrc[2] >= 'A' && pSrc[2] <= 'F')
				*pDest += (pSrc[2] - 'A' + 10);
			else if(pSrc[2] >= 'a' && pSrc[2] <= 'f')
				*pDest += (pSrc[2] - 'a' + 10);
			else
				*pDest += (pSrc[2] - '0');

			pSrc += 3;
		}
		else if(*pSrc == '+')
		{
			*pDest = ' ';
			++pSrc;
		}
		else
		{
			*pDest = *pSrc;
			++pSrc;
		}
		++pDest;
		++cbDest;
	}
	//null-terminator
	*pDest = '\0';
	++cbDest;

	int cchWideChar = MultiByteToWideChar(CP_UTF8, 0, (LPCSTR)pUTF8, cbDest, NULL, 0);
	LPWSTR pUnicode = (LPWSTR)malloc(cchWideChar * sizeof(WCHAR));
	if(pUnicode == NULL)
	{
		free(pUTF8);
		return FALSE;
	}
	MultiByteToWideChar(CP_UTF8, 0, (LPCSTR)pUTF8, cbDest, pUnicode, cchWideChar);
	WideCharToMultiByte(CP_ACP, 0, pUnicode, cchWideChar, pBuf, cbBufLen, NULL, NULL);
	free(pUTF8);
	free(pUnicode);
	return TRUE;
}

LONG WINAPI MyUnhandledExceptionFilter(struct _EXCEPTION_POINTERS *pExceptionPointers)
{
	//先创建dump文件
	HANDLE lhDumpFile = CreateFile(_T("DumpFile.dmp"), GENERIC_WRITE, 0, NULL,                                    
		CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL ,NULL); 

	MINIDUMP_EXCEPTION_INFORMATION loExceptionInfo;
	loExceptionInfo.ExceptionPointers = pExceptionPointers;
	loExceptionInfo.ThreadId = GetCurrentThreadId();
	loExceptionInfo.ClientPointers = TRUE;

	int nDumpType = MiniDumpNormal | MiniDumpWithDataSegs | MiniDumpWithFullMemory | 
		MiniDumpWithThreadInfo | MiniDumpWithHandleData | MiniDumpWithUnloadedModules ;
	//写dump文件
	MiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(),lhDumpFile, (MINIDUMP_TYPE)nDumpType, &loExceptionInfo, NULL, NULL);

	CloseHandle(lhDumpFile);

	return EXCEPTION_EXECUTE_HANDLER;
} 

//<By张方雪 2013-5-28>这个函数能够把程序pin到任务栏.
void Pin2Taskbar(HINSTANCE hInst, bool bPin)
{
	TCHAR szInstancePath[MAX_PATH + 1] = {0};
	GetModuleFileName(hInst, szInstancePath, MAX_PATH);
	int pos = ::ReverseFind(szInstancePath, _T('\\'));
	if( pos >= 0 ) szInstancePath[pos + 1] = _T('\0');

	int iResIndex = bPin ? 5386 : 5387; //<By张方雪 2013-5-24>不晓得这个index是什么, 有文档吗?
	IShellDispatch* pShell = NULL;
	Folder *pFolder = NULL;
	HMODULE hShell32 = ::LoadLibrary(L"shell32.dll");
	if (!hShell32)
		return;

	TCHAR szCmd[512];
	int nLen = ::LoadString(hShell32, iResIndex, szCmd, _countof(szCmd));
	HRESULT hr = S_OK;
	VARIANT vPath;
	::VariantInit(&vPath);
	vPath.vt = VT_BSTR;

	vPath.bstrVal = ::SysAllocString(szInstancePath);

	if (FAILED(::CoCreateInstance(CLSID_Shell, NULL, CLSCTX_SERVER, IID_IDispatch, (LPVOID*)&pShell)) || !pShell)
		return;

	if (FAILED(pShell->NameSpace(vPath, &pFolder)) || !pFolder)
		return ;

	FolderItem* pItem = NULL;
	VARIANT vPathName;
	::VariantInit(&vPathName);
	vPathName.vt = VT_BSTR;
	BSTR bstrPathName = ::SysAllocString(_T("VenusMessenger.exe"));
	if (FAILED(pFolder->ParseName(bstrPathName, &pItem)) || !pItem)
		return;

	FolderItemVerbs* pVerbs = NULL;

	if (FAILED(pItem->Verbs(&pVerbs)) || !pVerbs)
		return ;

	long lCount = 0;
	pVerbs->get_Count(&lCount);

	BSTR bstrCmd = ::SysAllocString(szCmd);
	for(long i = 0; i < lCount; i++)
	{
		FolderItemVerb* pVerb = NULL;
		VARIANT var;
		::VariantInit(&var);
		var.vt = VT_I4;
		var.lVal = i;
		pVerbs->Item(var, &pVerb);
		if(pVerb)
		{
			BSTR bstrName;
			pVerb->get_Name(&bstrName);
			if(_tcscmp(bstrCmd, bstrName) == 0)
			{
				pVerb->DoIt();
				return;
			}
		}
		::VariantClear(&var);
	}
}

