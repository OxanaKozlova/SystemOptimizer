#include <iostream>
#include <windows.h>
#include <shlwapi.h>






int Find(HKEY hKey);
int IsFileName(const TCHAR *fileName);
void Log(HKEY hKey, const TCHAR *info1, const TCHAR *info2, int i);
int IsFileExist(const TCHAR *filename);
void deleteKey(HKEY hKey, TCHAR *fileName);
LONG DeleteKey(HKEY hParentKey, LPCTSTR szKey);
void OptimizeRegister();

