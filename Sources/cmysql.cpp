#include <stdlib.h>
#include <iostream>
#include <Windows.h>

#include "mysql_connection.h"
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/prepared_statement.h>
#include "..\Sources\Headers\cmysql.h"
#include "..\Sources\Headers\animation.h"

animation aniSQLObj;

sql::Driver* driver;
sql::Connection* con;
sql::Statement* stmt;
sql::PreparedStatement* pstmt;
sql::ResultSet* result;

void cmysql::ReadTable(std::string t) {
	try {
		std::string y;
		y = "SELECT * FROM " + t + ";";
		pstmt = con->prepareStatement(y);
		result = pstmt->executeQuery();
		std::string userdata[1000][3];
		int x = 0;
		int z;
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
				std::cout << userdata[o][a] << "\t";
			}
			std::cout << std::endl;
		}
		delete result;
		delete pstmt;
		system("pause");
	}
	catch (sql::SQLException e) {
		std::cout << std::endl;
		//std::cout << e.what() << std::endl;	//	DEBUG ONLY
		std::cout << "Table không tồn tại ";
		aniSQLObj.dotAnimation(1000);
	}
}

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
		if (attemptKetNoi >= 3) exit(1);	//	Đóng chương trình nếu kết nối thất bại 3 lần
	}
	/*-----------------------------------------------------------------------------*/
}

void cmysql::ConnectDB() {
	/*	Kết nối đến Database (Connect to Database)	*/
	int attemptKetNoi = 0;
	std::string input;
	while (true) {
		try {
			/*	Kiểu 1: Nhập thủ công	*/
			//std::cout << "Nhập Database: ";
			//std::cin >> input;
			//con->setSchema(input);
			/*--------------------------*/

			/*	Kiểu 2: Định sẵn giá trị*/
			input = "quickstartdb";
			con->setSchema(input);
			/*--------------------------*/
			break;
		}
		catch (sql::SQLException e) {
			//cout << e.what() << endl;	//	DEBUG ONLY

			/*	Kiểu 1: Nhập thủ công	*/
			//std::cin.clear();
			//std::cin.ignore(10000, '\n');
			//system("cls");
			/*--------------------------*/

			/*	Kiểu 2: Định sẵn giá trị*/
			std::cout << "Không thể kết nối tới database ";
			//cout << e.what() << endl;	//	DEBUG ONLY
			aniSQLObj.dotAnimation(500);
			attemptKetNoi++;
			system("cls");
			/*--------------------------*/
		}
		if (attemptKetNoi >= 3) exit(1);	//	Đóng chương trình nếu kết nối thất bại 3 lần
	}
	/*-------------------------------------------------------*/
}

void cmysql::AddTable(std::string x) {
	try {
		std::string y;
		y = "CREATE TABLE " + x + " (id serial PRIMARY KEY, user VARCHAR(100), pwd VARCHAR(100))";
		pstmt = con->prepareStatement(y);
		pstmt->execute();
		std::cout << std::endl;
		std::cout << "Đang tạo table " << x << " ";
		delete pstmt;
		aniSQLObj.dotAnimation(1000);
	}
	catch (sql::SQLException e) {
		//std::cout << std::endl;
		//std::cout << e.what() << std::endl;	//	DEBUG ONLY
	}
}

void cmysql::DeleteTable(std::string x) {
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
		//std::cout << std::endl;
		//std::cout << e.what() << std::endl;	//	DEBUG ONLY
	}
}

void cmysql::InsertData(std::string t, std::string u, std::string p) {
	try {
		std::string y;
		y = "INSERT INTO " + t + "(user, pwd) VALUES(\"" + u + "\",\"" + p + "\")";
		pstmt = con->prepareStatement(y);
		pstmt->execute();
		std::cout << std::endl;
		std::cout << "Đang thêm dữ liệu vào table " << t << " ";
		delete pstmt;
		aniSQLObj.dotAnimation(1000);
	}
	catch (sql::SQLException e) {
		std::cout << std::endl;
		//std::cout << e.what() << std::endl;	//	DEBUG ONLY
		std::cout << "Table không tồn tại hoặc sai kiểu dữ liệu ";
		aniSQLObj.dotAnimation(1000);
	}
}