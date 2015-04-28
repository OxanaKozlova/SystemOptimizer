#include "findFunction.h"
#include <shlwapi.h>

using namespace std;

int amountFiles = 0;
int allFiles = 0;


void ProcessFoundFile(HANDLE CONST hStdOut, LPWSTR CONST szPath,
	WIN32_FIND_DATA CONST * CONST fdFindData,
	LPWSTR CONST lpSearch, int offset) {
	TCHAR szEnd[] = L"\r\n"; 
	DWORD dwTemp;
	allFiles++;
	PWSTR lastPosition = StrStrI(fdFindData->cFileName, lpSearch) + offset;
	if (NULL != (lastPosition - offset) && *lastPosition == '\0') {
		WriteConsole(hStdOut, szPath, lstrlen(szPath), &dwTemp, NULL);
		printf("\n");
		amountFiles++;


	}
}




void RecursiveSearch(HANDLE CONST hStdOut, LPWSTR szPath,
	LPWSTR CONST lpSearch, int offset) {
	WIN32_FIND_DATA fdFindData;
	HANDLE hFind;
	TCHAR * CONST lpLastChar = szPath + lstrlen(szPath);

	lstrcat(szPath, L"*");
	hFind = FindFirstFile(szPath, &fdFindData);
	*lpLastChar = '\0';

	if (INVALID_HANDLE_VALUE == hFind) {
		return;
	}

	do {
		if ((0 == lstrcmp(fdFindData.cFileName, L".")) ||
			(0 == lstrcmp(fdFindData.cFileName, L".."))) {
			continue;
		}
		lstrcat(szPath, fdFindData.cFileName);
		if (fdFindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
			lstrcat(szPath, L"\\");
			RecursiveSearch(hStdOut, szPath, lpSearch, offset);
		}
		else {
			ProcessFoundFile(hStdOut, szPath, &fdFindData, lpSearch, offset);
		}
		*lpLastChar = '\0';
	} while (FindNextFile(hFind, &fdFindData));

	FindClose(hFind);
}

void SearchOnAllDrives(HANDLE CONST hStdOut, LPWSTR CONST lpSearch, int offset) {
	TCHAR szCurrDrive[] = L"A:\\";
	TCHAR szPath[MAX_PATH + 1];
	DWORD i, dwDisksMask = GetLogicalDrives();

	for (i = 0; i < 3; i++) {
		if (dwDisksMask & 1) {
			lstrcpy(szPath, szCurrDrive);
			RecursiveSearch(hStdOut, szPath, lpSearch, offset);
		}
		dwDisksMask >>= 1;
		szCurrDrive[0]++;
	}
}

void findTempFile(){

	HANDLE CONST hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
	LPWSTR extention = L".tmp";
	int offset = lstrlen(extention);
	allFiles = 0;
	
	
	SearchOnAllDrives(hStdOut, extention, offset);
	LPWSTR  lpSearch = L".temp";
	
	printf("Found %d files/n", allFiles);
}


void findCategoryFiles(){
	LPWSTR extensionPhoto[] = { L".jpeg", L".psd", L".bmp", L".jpg", L"gif", L".png", L".tif" };
	LPWSTR extensionVideo[] = { L".mpeg", L".avi", L".wmv", L".mpg", L".mp4" };
	LPWSTR extensionMusic[] = { L".mp3", L".wav", L".wma", L".mid" };
	LPWSTR extensionDocuments[] = { L".doc", L".docx", L".pdf", L"xls", L"ppt", L".xlsx", L".odc" };
	HANDLE CONST hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);

	int amountPhoto = countFiles(extensionPhoto, (sizeof(extensionPhoto) / sizeof(extensionPhoto[0])), hStdOut);
	int amountVideo = countFiles(extensionVideo, (sizeof(extensionVideo) / sizeof(extensionVideo[0])), hStdOut);
	int amountMusic = countFiles(extensionMusic, (sizeof(extensionMusic) / sizeof(extensionMusic[0])), hStdOut);
	int amountDocuments = countFiles(extensionDocuments, (sizeof(extensionDocuments) / sizeof(extensionDocuments[0])), hStdOut);


	
	float percentPhoto = (float)amountPhoto /(float) allFiles *100.0;
	float percentVideo = (float) amountVideo /(float) allFiles * 100.0;
	float percentMusic = (float)amountMusic / (float)allFiles * 100.0;
	float percentDocuments = (float) amountDocuments /(float) allFiles * 100.0;
	float otherFiles = 100.0 - (float)percentPhoto - (float)percentVideo - (float)percentMusic - (float)percentDocuments;
	
	printf("PHOTO %.2f\n", percentPhoto);
	printf("VIDEO %.2f%\n", percentVideo);
	printf("MUSIC %.2f\n", percentMusic);
	printf("DOCUMENTS %.2f\n", percentDocuments);
	printf("OTHER FILES %.2f\n", otherFiles);

}


int countFiles(LPWSTR *list, int amountExtension, HANDLE  hStdOut ){
	int i = 0;
	
	while (i < amountExtension){
		allFiles = 0;
		SearchOnAllDrives(hStdOut, list[i], lstrlen(list[i]));

		i++;
	}
	int amount= amountFiles;
	amountFiles = 0;
	return amount;
}