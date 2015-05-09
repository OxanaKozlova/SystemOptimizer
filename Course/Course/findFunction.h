#include <iostream>
#include <windows.h>





void SearchOnAllDrives(LPWSTR CONST lpSearch, int offset);
void RecursiveSearch( LPWSTR szPath,
	LPWSTR CONST lpSearch, int offset);
int ProcessFoundFiles(LPWSTR CONST szPath,
	WIN32_FIND_DATA CONST * CONST fdFindData,
	LPWSTR CONST lpSearch, int offset);

void findTempFile();
void findCategoryFiles();

int countFiles(LPWSTR *list, int amountExtensio);
void deleteFile(WIN32_FIND_DATA CONST * CONST fdFindData, HANDLE hFind, LPWSTR szPath);

