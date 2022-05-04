#include <iostream>
#include <Windows.h>
#include "../Sources/Headers/menu.h"

void menu::ShowMenu() const {
	std::cout << "1. Đọc dữ liệu\n";
	std::cout << "2. Thêm table\n";
	std::cout << "3. Xoá table\n";
	std::cout << "4. Thêm dữ liệu vào table\n";
	std::cout << "5. Cập nhật dữ liệu table\n";
	std::cout << "6. Đổi database\n";
	std::cout << "7. Thoát chương trình\n";
	std::cout << std::endl;
}

int menu::SelectOption() {
	int iOption;
	bool ValidOption = false;
	while (ValidOption == false) {
		system("cls");
		ShowMenu();
		std::cout << "Nhập lựa chọn: ";
		std::cin >> iOption;
		if (iOption > 0 && iOption < 8) {
			ValidOption = true;
		}
		std::cin.clear();
		std::cin.ignore(10000, '\n');
	}
	return iOption;
}

bool menu::ProcessOption(int _option) {
	bool EXITPROGRAM = false;
	switch (_option) {
	case 1: {
		system("cls");
		ShowMenu();
		CMySQL->ShowTables();
		std::cout << std::endl;
		std::string tinput;
		std::cout << "Chọn table: ";
		std::cin >> tinput;
		CMySQL->ReadTable(tinput);
		break;
	}
	case 2: {
		system("cls");
		ShowMenu();
		CMySQL->ShowTables();
		std::cout << std::endl;
		std::string tinput;
		int cinput;
		std::cout << "Nhập table: ";
		std::cin >> tinput;
		std::cout << "Nhập số cột: ";
		std::cin >> cinput;
		CMySQL->AddTable(tinput, cinput);
		break;
	}
	case 3: {
		system("cls");
		ShowMenu();
		CMySQL->ShowTables();
		std::cout << std::endl;
		std::string dinput;
		std::cout << "Chọn table: ";
		std::cin >> dinput;
		CMySQL->DeleteTable(dinput);
		break;
	}
	case 4: {
		system("cls");
		ShowMenu();
		std::string tinput;
		CMySQL->ShowTables();
		std::cout << std::endl;
		std::cout << "Chọn table: ";
		std::cin >> tinput;
		CMySQL->InsertData(tinput);
		break;
	}
	case 5: {
		system("cls");
		ShowMenu();
		CMySQL->ShowTables();
		std::cout << std::endl;
		CMySQL->UpdateData();
		break;
	}
	case 6: {
		system("cls");
		CMySQL->ChangeDatabase();
		ShowMenu();
		break;
	}
	case 7:
		std::cout << std::endl;
		std::cout << "Đang thoát chương trình ";
		Animation->DotAnimation(100);
		EXITPROGRAM = true;
		break;
	default:
		break;
	}
	return EXITPROGRAM;
}