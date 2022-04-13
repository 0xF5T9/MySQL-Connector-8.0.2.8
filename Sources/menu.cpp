#include <stdlib.h>
#include <iostream>
#include <Windows.h>

#include "mysql_connection.h"
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/prepared_statement.h>
#include "..\Sources\Headers\cmysql.h"
#include "..\Sources\Headers\menu.h"
#include "..\Sources\Headers\animation.h"

cmysql SQLMenuObj;
animation aniMenuObj;

void menu::Menu() {
	std::cout << "1. Đọc dữ liệu\n";
	std::cout << "2. Thêm table\n";
	std::cout << "3. Xoá table\n";
	std::cout << "4. Thêm dữ liệu vào table\n";
	std::cout << "5. Thoát chương trình\n";
	std::cout << std::endl;
}

int menu::getInput() {
	int i;
	bool hopLe = false;
	while (hopLe == false) {
		system("cls");
		Menu();
		std::cout << "Nhập lựa chọn: ";
		std::cin >> i;
		if (i > 0 && i < 6) {
			hopLe = true;
		}
		std::cin.clear();
		std::cin.ignore(10000, '\n');
		//system("cls");
		//Menu();
	}
	return i;
}

bool menu::processInput(int x) {
	bool q;
	q = false;
	switch (x) {
	case 1: {
		system("cls");
		Menu();
		std::string tinput;
		std::cout << "Nhập tên table: ";
		std::cin >> tinput;
		SQLMenuObj.ReadTable(tinput);
		return q;
		break;
	}
	case 2: {
		system("cls");
		Menu();
		std::string ainput;
		std::cout << "Nhập tên table: ";
		std::cin >> ainput;
		SQLMenuObj.AddTable(ainput);
		return q;
		break;
	}
	case 3: {
		system("cls");
		Menu();
		std::string dinput;
		std::cout << "Nhập tên table: ";
		std::cin >> dinput;
		SQLMenuObj.DeleteTable(dinput);
		return q;
		break;
	}
	case 4: {
		system("cls");
		Menu();
		std::string tinput;
		std::string uinput;
		std::string pinput;
		std::cout << "Nhập tên table: ";
		std::cin >> tinput;
		std::cout << "Nhập dữ liệu 'user': ";
		std::cin >> uinput;
		std::cout << "Nhập dữ liệu 'pwd': ";
		std::cin >> pinput;
		SQLMenuObj.InsertData(tinput, uinput, pinput);
		return q;
		break;
	}
	case 5:
		std::cout << std::endl;
		std::cout << "Đang thoát chương trình ";
		aniMenuObj.dotAnimation(100);
		q = true;
		return q;
		break;
	default:
		return q;
		break;
	}
	return q;
}