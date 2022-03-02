#include "Functions.h"

Functions fFun;

void ConsoleSettings()
{
	SetConsoleTitleA("Auto Fishing v1.30 | Author: CoderDuc");
	HANDLE hHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hHandle, 14);
}

int main() {
	ConsoleSettings();
	int rod_num;
	cout << "Enter your rod number: " << endl;
	cin >> rod_num;
	system("cls");
	fFun.auto_fishing(rod_num);
	system("pause");
	return 0;
}