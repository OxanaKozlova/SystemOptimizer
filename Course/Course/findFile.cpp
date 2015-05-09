#include "findFunction.h"
#include <shlwapi.h>

using namespace std;

int amountFiles = 0;
int allFiles = 0;


int ProcessFoundFiles(LPWSTR CONST szPath,
	WIN32_FIND_DATA CONST * CONST fdFindData,
	LPWSTR CONST lpSearch, int offset) {
	TCHAR szEnd[] = L"\r\n"; 
	DWORD dwTemp;
	allFiles++;
	PWSTR lastPosition = StrStrI(fdFindData->cFileName, lpSearch) + offset;
	if (NULL != (lastPosition - offset) && *lastPosition == '\0') {
		WriteConsole(GetStdHandle(STD_OUTPUT_HANDLE), szPath, lstrlen(szPath), &dwTemp, NULL);
		printf("\n");
		amountFiles++;
		return 0;
	}
	return 1;
}




void RecursiveSearch( LPWSTR szPath,
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
		WCHAR *temp = szPath;
		LPCTSTR path = temp;
		if (fdFindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
			//removeDirectory
			if (!RemoveDirectory(path)); {
				lstrcat(szPath, L"\\");
				RecursiveSearch(szPath, lpSearch, offset);
			}
		}
		else {
			if (!ProcessFoundFiles(szPath, &fdFindData, lpSearch, offset)){
				deleteFile(&fdFindData, hFind, szPath);
			}
		}
		*lpLastChar = '\0';
	} while (FindNextFile(hFind, &fdFindData));

	FindClose(hFind);
}

void SearchOnAllDrives( LPWSTR CONST lpSearch, int offset) {
	TCHAR szCurrDrive[] = L"A:\\";
	TCHAR szPath[MAX_PATH + 1];
	DWORD i, dwDisksMask = GetLogicalDrives();
	for (i = 0; i < 7; i++) {
		if (dwDisksMask & 1) {
			lstrcpy(szPath, szCurrDrive);
			RecursiveSearch( szPath, lpSearch, offset);
		}
		dwDisksMask >>= 1;
		szCurrDrive[0]++;
		
		
	}
	//RecursiveSearch(szPath, lpSearch, offset);
}

void findTempFile(){
	LPWSTR extention[] = { L"tmp", L".localstorage" };
	int offset;
	allFiles = 0;
	int amountExtention = sizeof(extention) / sizeof(extention[0]);
	for (int i = 0; i < amountExtention; i++){
		offset = lstrlen(extention[i]);
		SearchOnAllDrives(extention[i], offset);
	}
	
	printf("Found %d files\n", allFiles);
}


void findCategoryFiles(){
	LPWSTR extensionPhoto[] = { L".jpeg", L".psd", L".bmp", L".jpg", L"gif", L".png", L".tif" };
	LPWSTR extensionVideo[] = { L".mpeg", L".avi", L".wmv", L".mpg", L".mp4" };
	LPWSTR extensionMusic[] = { L".mp3", L".wav", L".wma", L".mid" };
	LPWSTR extensionDocuments[] = { L".doc", L".docx", L".pdf", L"xls", L"ppt", L".xlsx", L".odc" };
	HANDLE CONST hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);

	int amountPhoto = countFiles(extensionPhoto, (sizeof(extensionPhoto) / sizeof(extensionPhoto[0])));
	int amountVideo = countFiles(extensionVideo, (sizeof(extensionVideo) / sizeof(extensionVideo[0])));
	int amountMusic = countFiles(extensionMusic, (sizeof(extensionMusic) / sizeof(extensionMusic[0])));
	int amountDocuments = countFiles(extensionDocuments, (sizeof(extensionDocuments) / sizeof(extensionDocuments[0])));


	
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


int countFiles(LPWSTR *list, int amountExtension ){
	int i = 0;
	
	while (i < amountExtension){
		allFiles = 0;
		SearchOnAllDrives( list[i], lstrlen(list[i]));

		i++;
	}
	int amount= amountFiles;
	amountFiles = 0;
	return amount;
}


void deleteFile(WIN32_FIND_DATA CONST * CONST fdFindData, HANDLE hFind, LPWSTR szPath){
	int isDeleted = 0;	
	WCHAR *temp = szPath;
	LPCTSTR str = temp;
	HANDLE hOpen;
	if (fdFindData->dwFileAttributes != FILE_ATTRIBUTE_READONLY || fdFindData->dwFileAttributes != FILE_ATTRIBUTE_SYSTEM){
		//hOpen = CreateFile(str, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, NULL);
		if (!DeleteFile(szPath)){
			printf("Failed to delete the file. try to close\n");
		}
	}
	else printf("File is protected\n");
}

