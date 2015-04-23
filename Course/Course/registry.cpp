#include "registry.h"

using namespace std;


#define MAX_LOADSTRING 1024
TCHAR szBuff[MAX_LOADSTRING];

int countWrongValue = 0;




#ifndef STATUS_SUCCESS
#define STATUS_SUCCESS ((NTSTATUS)0x00000000L)
#endif

#ifndef STATUS_BUFFER_TOO_SMALL
#define STATUS_BUFFER_TOO_SMALL ((NTSTATUS)0xC0000023L)
#endif

TCHAR **szListBuff;
wstring GetKeyPathFromKKEY(HKEY key);

void OptimizeRegister(){
	szListBuff = new TCHAR*[2048];
	for (int i = 0; i < 2048; i++)
		szListBuff[i] = new TCHAR[2048];
	HKEY hKey;
	RegOpenKeyEx(HKEY_CURRENT_USER, L"Software", NULL, KEY_ALL_ACCESS, &hKey);
	Find(hKey);
	cout << "Find " << countWrongValue << " wrong value" << endl;
	RegCloseKey(hKey);
	return;
}



int Find(HKEY hKey) {
	if (hKey == NULL) {
		return 0;
	}
	LONG ErrorKey, ErrorValue;
	int dwIndexKey = 0, dwIndexValue = 0;
	TCHAR lpValueNameKey[MAX_PATH], lpValueNameValue[MAX_PATH];
	TCHAR lpDataValue[MAX_PATH];
	DWORD lpcbValueNameKey, lpcbValueNameValue, lpTypeValue, lpcbDataValue;
	HKEY hKeyI;
	int i = 0;
	for (;;) {
		lpcbValueNameKey = sizeof(lpValueNameKey);
		ErrorKey = RegEnumKeyEx(hKey, dwIndexKey, lpValueNameKey, &lpcbValueNameKey, NULL, NULL, NULL, NULL);

		if (ErrorKey == ERROR_NO_MORE_ITEMS) {
			if (dwIndexKey == 0) {

				for (;;)
				{
					lpcbValueNameValue = MAX_PATH;
					lpcbDataValue = sizeof(lpDataValue);
					ErrorValue = RegEnumValue(hKey, dwIndexValue++, lpValueNameValue, &lpcbValueNameValue, NULL, &lpTypeValue, (LPBYTE)lpDataValue, &lpcbDataValue);
					if (ErrorValue == ERROR_NO_MORE_ITEMS) {
						break;
					}

					if (lpTypeValue == REG_SZ && IsFileName(lpDataValue) && !IsFileExist(lpDataValue)) {
						Log(hKey, lpValueNameValue, lpDataValue, i);
						i++;
						countWrongValue++;
					}
				}
			}
			break;
		};

		RegOpenKeyEx(hKey, lpValueNameKey, NULL, KEY_READ, &hKeyI);
		Find(hKeyI);
		RegCloseKey(hKeyI);

		++dwIndexKey;
	}

	return dwIndexValue;
}





int IsFileName(const TCHAR *fileName) {
	int i = 0;
	while (fileName[i] != '\0') {
		if (i == 0 && iswalpha(fileName[i])) { //представляет ли целое число алфавитный символ
		}
		else if (i == 1 && fileName[i] == ':') {
		}
		else if (i == 2 && fileName[i] == '\\') {
		}
		else if (i > 2 && fileName[i] != '\\' && fileName[i] != '/' && fileName[i] != ':'
			&& fileName[i] != '?' && fileName[i] != '\"' && fileName[i] != '<'
			&& fileName[i] != '>' && fileName[i] != '|') {
		}
		else if (i > 3 && fileName[i] == '\\' && fileName[i - 1] != '\\') {
		}
		else return 0;
		++i;
	}
	if (i > 2) {
		return 1;
	}
	return 0;
}


void Log(HKEY hKey, const TCHAR *info1, const TCHAR *info2, int i) {


	wsprintf(szBuff, L"%s, \"%s\" : \"%s\"", GetKeyPathFromKKEY(hKey).c_str(), info1, info2);

	szListBuff[i] = szBuff;



}


wstring GetKeyPathFromKKEY(HKEY key)
{
	std::wstring keyPath;
	if (key != NULL)
	{
		HMODULE dll = LoadLibrary(L"ntdll.dll");
		if (dll != NULL) {
			typedef DWORD(__stdcall *NtQueryKeyType)(
				HANDLE  KeyHandle,
				int KeyInformationClass,
				PVOID  KeyInformation,
				ULONG  Length,
				PULONG  ResultLength);

			NtQueryKeyType func = reinterpret_cast<NtQueryKeyType>(::GetProcAddress(dll, "NtQueryKey"));

			if (func != NULL) {
				DWORD size = 0;
				DWORD result = 0;
				result = func(key, 3, 0, 0, &size);
				if (result == STATUS_BUFFER_TOO_SMALL)
				{
					size = size + 2;
					wchar_t* buffer = new (std::nothrow) wchar_t[size / sizeof(wchar_t)];
					if (buffer != NULL)
					{
						result = func(key, 3, buffer, size, &size);
						if (result == STATUS_SUCCESS)
						{
							buffer[size / sizeof(wchar_t)] = L'\0';
							keyPath = std::wstring(buffer + 2);
						}
						delete[] buffer;
					}
				}
			}
			FreeLibrary(dll);
		}
	}
	return keyPath;
}

int IsFileExist(const TCHAR *filename)
{
	struct _stat buffer;
	TCHAR str[MAX_PATH];
	wcscpy_s(str, filename);
	int result = 0;

	int lasti = wcslen(str) - 1;
	if (lasti > 2 && str[lasti] == '\\') {
		str[lasti] = '\0';
		result = (_wstat(str, &buffer) == 0);
	}
	else if (!(result = (_wstat(str, &buffer) == 0))) {
		for (int i = lasti; i > 0; --i) {
			if (str[i] == ',') {
				str[i] = '\0';
				result = (_wstat(str, &buffer) == 0);// 0 - сведения о файле получены
				break;
			}
		}
	}
	return result;;
}

