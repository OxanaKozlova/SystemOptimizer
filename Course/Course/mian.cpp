#include <iostream>
#include <windows.h>
#include <stdio.h>
#include <cstdlib>
#include <conio.h>
#include "registry.h"
#include <shlwapi.h>
#include "findFunction.h"



#define STRLEN(x) (sizeof(x)/sizeof(x[0]) - 1)


using namespace std;

void getDisks();
void menu();
void cleanRegister();



int main(){
	menu();
	return 0;
}



void menu(){

	while (1){
		cout << "1 - delete temp and cookies files" << endl
			<< "2 - clean registry" << endl
			<< "3 - show information" << endl << endl
			<< "Enter option" << endl;
		int option;
		cin >> option;

		switch (option) {
		case 1:
			cout << "Delete temp files" << endl;
			findTempFile();
			break;
		case 2:
			cout << "Clean registry" << endl;
			OptimizeRegister();

			break;
		case 3:
			
			cout << "Statistic" << endl;
			findCategoryFiles();
			break;

		default:
			cout << "Error! Try again" << endl;
			break;
		}
		cout << "Continue enter 1" << endl;
		char c;

		if (getch() != '1')
			break;
		system("cls");

	}
}






