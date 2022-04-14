#include <stdlib.h>
#include <iostream>
#include <Windows.h>

#include "mysql_connection.h"
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/prepared_statement.h>
#include "..\Sources\Headers\cmysql.h"
#include "..\Sources\Headers\animation.h"

/*	Tạo object từ classes để sử dụng constructor (Create the object to use constructors)	*/
animation aniSQLObj;
sql::Driver* driver;
sql::Connection* con;
sql::Statement* stmt;
sql::PreparedStatement* pstmt;
sql::ResultSet* result;

/*	Định nghĩa các hàm của cMySQL (Define cMySQL Class Functions)	*/
void cmysql::Connect() {
	/*	Kết nối máy chủ MySQL (Connect to MySQL Server)	*/
	int attemptKetNoi = 0;
	while (true) {
		try
		{
			driver = get_driver_instance();
			con = driver->connect(server, username, password);
			break;
		}
		catch (sql::SQLException e)
		{
			std::cout << "Không thể kết nối tới máy chủ ";
			//cout << e.what() << endl;	//	DEBUG ONLY
			aniSQLObj.dotAnimation(500);
			attemptKetNoi++;
			system("cls");
		}
		if (attemptKetNoi >= 3) exit(1);	//	Đóng chương trình nếu kết nối thất bại 3 lần (Close the program if fails to connect 3 times)
	}
	/*-----------------------------------------------------------------------------*/
}

void cmysql::ConnectDB() {
	/*	Kết nối đến Database (Connect to Database)	*/
	int attemptKetNoi = 0;
	std::string input;
	while (true) {
		try {
			/*	Kiểu 1: Nhập thủ công (Manually)	*/
			//	std::cout << "Nhập Database: ";
			//	std::cin >> input;
			//	con->setSchema(input);
			/*--------------------------------------*/

			/*	Kiểu 2: Định sẵn giá trị (Predefined) */
			input = "test";
			con->setSchema(input);
			/*----------------------------------------*/
			break;
		}
		catch (sql::SQLException e) {
			//cout << e.what() << endl;	//	DEBUG ONLY

			/*	Kiểu 1: Nhập thủ công (Manually)	*/
			//	std::cin.clear();
			//	std::cin.ignore(10000, '\n');
			//	system("cls");
			/*--------------------------------------*/

			/*	Kiểu 2: Định sẵn giá trị (Predefined) */
			std::cout << "Không thể kết nối tới database ";
			//	cout << e.what() << endl;	//	DEBUG ONLY
			aniSQLObj.dotAnimation(500);
			attemptKetNoi++;
			system("cls");
			/*----------------------------------------*/
		}
		if (attemptKetNoi >= 3) exit(1);	//	Đóng chương trình nếu kết nối thất bại 3 lần (Close the program if fails to connect 3 times)
	}
	/*-------------------------------------------------------*/
}

void cmysql::ReadTable2(std::string t) {
	/*	Đọc dữ liệu từ Table (Read datas from the Table)	*/

	int tongcot = 0;	//	Đếm số cột trong Table

	try {
		std::string taolenh;
		taolenh = "SHOW COLUMNS FROM " + t;
		pstmt = con->prepareStatement(taolenh);
		result = pstmt->executeQuery();
		while (result->next()) {
			std::string z = result->getString(1);
			std::cout << z << "\t\t";
			tongcot++;	//	+1 mỗi 1 vòng lặp
		}
	}
	catch (sql::SQLException e) {
		std::cout << std::endl;
		//	std::cout << e.what() << std::endl;	//	DEBUG ONLY
		std::cout << "Table không tồn tại ";
		aniSQLObj.dotAnimation(1000);
		goto escape0;	//	Thoát khỏi vòng lặp vì Table không tồn tại (Break the loop to try again because table doesn't exists)
	}
	try {
		std::string y;
		y = "SELECT * FROM " + t + ";";
		pstmt = con->prepareStatement(y);
		result = pstmt->executeQuery();
		std::string userdata[100][100];
		int x = 0;
		int x2 = 0;
		int a = 1;
		int z;
		while (result->next()) {
			for (int i = 0; i < tongcot; i++) {
				userdata[x2][i] = result->getString(i+1);
			}
			x++;
			x2++;
		}
		std::cout << std::endl;
		for (int o = 0; o <= x; o++) {
			for (int a = 0; a < tongcot; a++) {
				std::cout << userdata[o][a] << "\t\t";
			}
			std::cout << std::endl;
		}
		delete result;
		delete pstmt;
		system("pause");
	}
	catch (sql::SQLException e) {
		std::cout << std::endl;
		//	std::cout << e.what() << std::endl;	//	DEBUG ONLY
		std::cout << "Table không tồn tại ";
		aniSQLObj.dotAnimation(1000);
	}
	escape0:
	std::cout << "";
}

void cmysql::ShowTables() {
	/*	Hiện các table có sẵn trong Database (Show the available tables in Database)	*/
	std::cout << "Danh sách table:   ";
	try {
		std::string y;
		y = "SHOW TABLES;";
		pstmt = con->prepareStatement(y);
		result = pstmt->executeQuery();
		while (result->next()) {
			std::string z = result->getString(1);
			std::cout << z << " ";
		}
	}
	catch (sql::SQLException e) {
	}
}

void cmysql::AddTable(std::string t, int c) {
	/*	Thêm table vào Database (Add tables to the Database)	*/
	std::string tencot;
	std::string loaicot;
	std::string cy;
	cy = "CREATE TABLE " + t + " (";
	std::cout << "Danh sách các loại cột có sẵn: serial, serialPRIMARY, varchar, varcharPRIMARY, integer, integerPRIMARY\n";
	std::cout << std::endl;
	for (int i = 0; i < c; i++) {
		std::cout << "Nhập tên cột (" << i + 1 << "): ";
		std::cin >> tencot;
		std::cout << "Nhập loại cột (" << i + 1 << "): ";
		std::cin >> loaicot;
		if (loaicot == "serial") loaicot = "serial";
		if (loaicot == "serialPRIMARY") loaicot = "serial PRIMARY KEY";
		if (loaicot == "varchar") loaicot = "VARCHAR(100)";
		if (loaicot == "varcharPRIMARY") loaicot = "VARCHAR(100) PRIMARY KEY";
		if (loaicot == "integer") loaicot = "INTEGER";
		if (loaicot == "integerPRIMARY") loaicot = "INTEGER PRIMARY KEY";
		cy.append(tencot + " " + loaicot);
		if (i < c - 1) cy.append(",");
	}
	cy.append(")");
	try {
		std::string y;
		pstmt = con->prepareStatement(cy);
		pstmt->execute();
		std::cout << std::endl;
		std::cout << "Đang tạo table " << t << " ";
		delete pstmt;
		aniSQLObj.dotAnimation(1000);
	}
	catch (sql::SQLException e) {
			std::cout << std::endl;
			std::cout << "Table đã tồn tồn tại hoặc loại cột không hợp lệ ";
			aniSQLObj.dotAnimation(1000);
			//std::cout << e.what() << std::endl;	//	DEBUG ONLY
	}
}

void cmysql::DeleteTable(std::string x) {
	/*	Xoá Table khỏi Database (Delete Tables from the Database)	*/
	try {
		std::string y;
		y = "DROP TABLE IF EXISTS " + x;
		pstmt = con->prepareStatement(y);
		pstmt->execute();
		std::cout << std::endl;
		std::cout << "Đang xoá table " << x << " ";
		delete pstmt;
		aniSQLObj.dotAnimation(1000);
	}
	catch (sql::SQLException e) {
		//	std::cout << std::endl;
		//	std::cout << e.what() << std::endl;	//	DEBUG ONLY
	}
}

void cmysql::InsertData(std::string t) {
	/*	Thêm dữ liệu vào Table (Add data to the Tables)	*/
	std::string tencot;
	std::string dulieucot;
	std::string cy;
	std::string cy2;
	int c;
	std::cout << "Danh sách các cột:   ";
	try {
		std::string y;
		y = "SHOW COLUMNS FROM " + t;
		pstmt = con->prepareStatement(y);
		result = pstmt->executeQuery();
		while (result->next()) {
			std::string z = result->getString(1);
			std::cout << z << " ";
		}
	}
	catch (sql::SQLException e) {
		std::cout << std::endl;
		//	std::cout << e.what() << std::endl;	//	DEBUG ONLY
		std::cout << "Table không tồn tại ";
		aniSQLObj.dotAnimation(1000);
		goto escape1;	//	Thoát khỏi vòng lặp vì Table không tồn tại (Break the loop to try again because table doesn't exists)
	}
	cy = "INSERT INTO " + t + "(";
	std::cout << std::endl;
	std::cout << "Đối với loại cột 'varchar' phải thêm \"\" (Ví dụ: \"text\")" << std::endl;
	std::cout << "Nhập tổng số cột muốn thêm dữ liệu: ";
	std::cin >> c;
	try {
		std::string y; // INSERT INTO table(coum, colum) VALUES(value,value)
		for (int i = 0; i < c; i++) {
			std::cout << "Nhập tên cột (" << i + 1 << "): ";
			std::cin >> tencot;
			std::cout << "Nhập dữ liệu cột (" << i + 1 << "): ";
			std::cin >> dulieucot;
			cy.append(tencot);
			cy2.append(dulieucot);
			if (i < c - 1) cy.append(",");
			if (i < c - 1) cy2.append(",");
		}
		cy.append(") VALUES (" + cy2 + ")");
		pstmt = con->prepareStatement(cy);
		pstmt->execute();
		std::cout << std::endl;
		std::cout << "Đang thêm dữ liệu table " << t << " ";
		delete pstmt;
		aniSQLObj.dotAnimation(1000);
	}
	catch (sql::SQLException e) {
		std::cout << std::endl;
		//	std::cout << e.what() << std::endl;	//	DEBUG ONLY
		std::cout << "Table không tồn tại hoặc dữ liệu không hợp lệ ";
		aniSQLObj.dotAnimation(1000);
	}
	escape1:
	std::cout << "";
}

void cmysql::UpdateData() {
	/*	Hiện các table có sẵn trong Database (Show the available tables in Database)	*/
	std::string t, c, u, i;
	std::cout << "Danh sách table:   ";
	try {
		std::string y;
		y = "SHOW TABLES;";
		pstmt = con->prepareStatement(y);
		result = pstmt->executeQuery();
		while (result->next()) {
			std::string z = result->getString(1);
			std::cout << z << " ";
		}
	}
	catch (sql::SQLException e) {
	}
	/*	Chọn Table để chỉnh sửa (Select the Table to update)	*/
	try {
		std::cout << std::endl;
		std::cout << "Chọn table: ";
		std::cin >> t;
		std::string y;
		y = "SELECT * FROM " + t + ";";
		pstmt = con->prepareStatement(y);
		result = pstmt->executeQuery();
		std::string userdata[1000][3];
		int x = 0;
		int z;
		std::cout << "id\t\tuser\t\t\tpwd\t\t";
		while (result->next()) {
			//printf("Reading from table=(%d, %s, %s)\n", result->getInt(1), result->getString(2).c_str(), result->getString(3).c_str());
			z = result->getInt(1);
			std::string z2 = std::to_string(z);
			userdata[x][0] = z2;
			userdata[x][1] = result->getString(2);
			userdata[x][2] = result->getString(3);
			x++;
		}
		std::cout << std::endl;
		for (int o = 0; o <= x; o++) {
			for (int a = 0; a < 3; a++) {
				std::cout << userdata[o][a] << "\t\t";
			}
			std::cout << std::endl;
		}
		delete result;
		delete pstmt;
	}
	catch (sql::SQLException e) {
		std::cout << std::endl;
		//	std::cout << e.what() << std::endl;	//	DEBUG ONLY
		std::cout << "Table không tồn tại ";
		aniSQLObj.dotAnimation(1000);
		goto escape2;	//	Thoát khỏi vòng lặp vì Table không tồn tại (Break the loop to try again because table doesn't exists)
	}
	/*	Cập nhật dữ liệu cho Table (Update datas for the table)	*/
	std::cout << "Chọn id: ";
	std::cin >> i;
	std::cout << "Chọn cột: ";
	std::cin >> c;
	std::cout << "Nhập dữ liệu update: ";
	std::cin >> u;
	try {
		std::string y;
		y = "UPDATE " + t + " SET " + c + " = \"" + u + "\" WHERE id = " + i;
		pstmt = con->prepareStatement(y);
		pstmt->execute();
		std::cout << std::endl;
		std::cout << "Đang cập nhật dữ liệu table " << t << " ";
		delete pstmt;
		aniSQLObj.dotAnimation(1000);
	}
	catch (sql::SQLException e) {
		std::cout << std::endl;
		//std::cout << e.what() << std::endl;	//	DEBUG ONLY
		std::cout << "Table/column/id không tồn tại hoặc dữ liệu không hợp lệ ";
		aniSQLObj.dotAnimation(1000);
	}
	/*	Đặt điểm thoát nhanh khỏi vòng lặp (Set the escape point to break loop if needed)	*/
	escape2:
	std::cout << "";
}