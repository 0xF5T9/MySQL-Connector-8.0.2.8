#include <iostream>
#include <Windows.h>
#include "../Sources/Headers/animation.h"
#include "../Sources/Headers/cmysql.h"
#include "../Sources/Headers/menu.h"
#include "../Sources/Headers/func.h"

int main() {
	
	/*	Initialize program parameters	*/
	animation* Animation = CreateObjectAnimation();
	cmysql* CMySQL = CreateObjectCMySQL(Animation);
	menu* Menu = CreateObjectMenu(Animation);
	CMySQL->GetObj(Menu);
	Menu->GetObj(CMySQL);
	SetConsoleOutputCP(65001);

	SetConsoleTitle(L"Loading ...");
	CMySQL->ConfigFile();
	CMySQL->Connect();
	CMySQL->ConnectDB();
	SetConsoleTitle(L"MySQL Connector with Demo Functions");

	/*	Start the program	*/
	while (true) {
		bool EXITPROGRAM = Menu->ProcessOption(Menu->SelectOption());
		if (EXITPROGRAM == true) break;
	}
	FreeMemory(Animation);
	FreeMemory(CMySQL);
	FreeMemory(Menu);
	return 0;
}