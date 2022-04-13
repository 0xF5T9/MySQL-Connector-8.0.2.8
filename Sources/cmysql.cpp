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

void cmysql::ReadTable(std::string t) {
	/*	Đọc dữ liệu từ Table (Read data from Tables)	*/
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
		//	std::cout << e.what() << std::endl;	//	DEBUG ONLY
		std::cout << "Table không tồn tại ";
		aniSQLObj.dotAnimation(1000);
	}
}

void cmysql::AddTable(std::string x) {
	/*	Thêm table vào Database (Add tables to the Database)	*/
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
		//	std::cout << std::endl;
		//	std::cout << e.what() << std::endl;	//	DEBUG ONLY
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

void cmysql::InsertData(std::string t, std::string u, std::string p) {
	/*	Thêm dữ liệu vào Table (Add data to the Tables)	*/
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
		//	std::cout << e.what() << std::endl;	//	DEBUG ONLY
		std::cout << "Table không tồn tại hoặc sai kiểu dữ liệu ";
		aniSQLObj.dotAnimation(1000);
	}
}

void cmysql::UpdateData(std::string t, std::string c, std::string u, std::string i) {
	try { //UPDATE account SET pwd = "1212" WHERE user = "nhatduy1284t"
		std::string y;
		y = "UPDATE " + t + " SET " + c + " = \"" + u + "\" WHERE id = " + i;
		pstmt = con->prepareStatement(y);
		pstmt->execute();
		std::cout << std::endl;
		std::cout << "Đang cập nhật dữ liêu cho table " << t << " ";
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