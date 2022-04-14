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

/*	Tạo object từ classes để sử dụng constructor (Create the object to use constructors)	*/
cmysql SQLMenuObj;
animation aniMenuObj;

/*	Định nghĩa các hàm của Menu (Define Menu Class Functions)	*/
void menu::Menu() {
	std::cout << "1. Đọc dữ liệu\n";
	std::cout << "2. Thêm table\n";
	std::cout << "3. Xoá table\n";
	std::cout << "4. Thêm dữ liệu vào table\n";
	std::cout << "5. Cập nhật dữ liệu table\n";
	std::cout << "6. Thoát chương trình\n";
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
		if (i > 0 && i < 7) {
			hopLe = true;
		}
		std::cin.clear();
		std::cin.ignore(10000, '\n');
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
		SQLMenuObj.ShowTables();
		std::cout << std::endl;
		std::string tinput;
		std::cout << "Chọn table: ";
		std::cin >> tinput;
		SQLMenuObj.ReadTable2(tinput);
		return q;
		break;
	}
	case 2: {
		system("cls");
		Menu();
		SQLMenuObj.ShowTables();
		std::cout << std::endl;
		std::string tinput;
		int cinput;
		std::cout << "Nhập table: ";
		std::cin >> tinput;
		std::cout << "Nhập số cột: ";
		std::cin >> cinput;
		SQLMenuObj.AddTable(tinput, cinput);
		return q;
		break;
	}
	case 3: {
		system("cls");
		Menu();
		SQLMenuObj.ShowTables();
		std::cout << std::endl;
		std::string dinput;
		std::cout << "Chọn table: ";
		std::cin >> dinput;
		SQLMenuObj.DeleteTable(dinput);
		return q;
		break;
	}
	case 4: {
		system("cls");
		Menu();
		std::string tinput;
		SQLMenuObj.ShowTables();
		std::cout << std::endl;
		std::cout << "Chọn table: ";
		std::cin >> tinput;
		SQLMenuObj.InsertData(tinput);
		return q;
		break;
	}
	case 5: {
		system("cls");
		Menu();
		SQLMenuObj.UpdateData();
		return q;
		break;
	}
	case 6:
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