#include <iostream>
#include <windows.h>





void SearchOnAllDrives(HANDLE CONST hStdOut, LPWSTR CONST lpSearch, int offset);
void RecursiveSearch(HANDLE CONST hStdOut, LPWSTR szPath,
	LPWSTR CONST lpSearch, int offset);

void ProcessFoundFile(HANDLE CONST hStdOut, LPWSTR CONST szPath, 
	WIN32_FIND_DATA CONST * CONST fdFindData,
	LPWSTR CONST lpSearch, int offset);

void findTempFile();
void findCategoryFiles();

int countFiles(LPWSTR *list, int amountExtension, HANDLE  hStdOut);

