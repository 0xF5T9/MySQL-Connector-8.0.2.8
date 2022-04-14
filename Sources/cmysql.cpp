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

	//- 1. Kết nối máy chủ MySQL (Connect to MySQL Server)
	//- Đóng chương trình nếu kết nối thất bại 3 lần (Close the program if fails to connect 3 times)
	int SoLanThuKetNoi = 0;	//	Biến đếm (Counting variable)
	while (true) {	//	Vòng lặp vĩnh cữu (Loop untill success or not)
		try
		{
			driver = get_driver_instance();
			con = driver->connect(server, username, password);
			break;
		}
		catch (sql::SQLException e)
		{
			std::cout << "Không thể kết nối tới máy chủ ";
			//	cout << e.what() << endl;	//	DEBUG ONLY
			aniSQLObj.dotAnimation(500);
			SoLanThuKetNoi++;	//	+1 mỗi lần thử kết nối (+1 per connect attempt)
			system("cls");
		}
		if (SoLanThuKetNoi >= 3) exit(1);	//	Đóng chương trình nếu kết nối thất bại 3 lần (Close the program if fails to connect 3 times)
	}
}

void cmysql::ConnectDB() {
	/*	Kết nối đến Database (Connect to Database)	*/

	//- 1. Kết nối đến Database (Connect to Database)
	//- Đóng chương trình nếu kết nối thất bại 3 lần (Close the program if fails to connect 3 times)
	int SoLanThuKetNoi = 0;
	std::string database;
	while (true) {
		try {
			/*	Kiểu 1: Nhập thủ công (Manually)	*/
			//	std::cout << "Nhập Database: ";
			//	std::cin >> database;
			//	con->setSchema(database);
			/*--------------------------------------*/

			/*	Kiểu 2: Định sẵn giá trị (Predefined) */
			database = "test";
			con->setSchema(database);
			/*----------------------------------------*/
			break;
		}
		catch (sql::SQLException e) {
			//	cout << e.what() << endl;	//	DEBUG ONLY

			/*	Kiểu 1: Nhập thủ công (Manually)	*/
			//	std::cin.clear();
			//	std::cin.ignore(10000, '\n');
			//	system("cls");
			/*--------------------------------------*/

			/*	Kiểu 2: Định sẵn giá trị (Predefined) */
			std::cout << "Không thể kết nối tới database ";
			//	cout << e.what() << endl;	//	DEBUG ONLY
			aniSQLObj.dotAnimation(500);
			SoLanThuKetNoi++;
			system("cls");
			/*----------------------------------------*/
		}
		if (SoLanThuKetNoi >= 3) exit(1);	//	Đóng chương trình nếu kết nối thất bại 3 lần (Close the program if fails to connect 3 times)
	}
}

void cmysql::ReadTable(std::string t) {
	/*	Đọc dữ liệu từ Table (Read datas from Table)	*/

	//-	1. Đếm số cột trong Table (Count total columns in Table)
	//- Cout danh sách các cột (Cout list of columns)
	int TongCot = 0;	//	Biến đếm (Counting variable)
	try {
		std::string TaoQuery;
		TaoQuery = "SHOW COLUMNS FROM " + t;
		pstmt = con->prepareStatement(TaoQuery);
		result = pstmt->executeQuery();
		while (result->next()) {	//	Vòng lặp cout tên cột (Loop cout column name)
			std::string i = result->getString(1);
			std::cout << i << "\t\t";
			TongCot++;	//	+1 mỗi 1 vòng lặp (+1 per loop)
		}
	}
	catch (sql::SQLException e) {
		std::cout << std::endl;
		//	std::cout << e.what() << std::endl;	//	DEBUG ONLY
		std::cout << "Table không tồn tại ";
		aniSQLObj.dotAnimation(1000);
		goto escape0;	//	Thoát khỏi vòng lặp vì Table không tồn tại (Break the loop to try again because table doesn't exists)
	}

	//- 2. Lấy dữ liệu từ table và cout (Get and cout datas from table)
	try {
		std::string TaoQuery;
		std::string DuLieu[100][100];	//	Tạo biến mảng 2 chiều chứa dữ liệu table (Create 2-dimensions variable to store datas)
		TaoQuery = "SELECT * FROM " + t + ";";
		pstmt = con->prepareStatement(TaoQuery);
		result = pstmt->executeQuery();
		int x1 = 0;	//	Biến đếm (Counting variable)
		int x2 = 0;	//	Biến đếm (Counting variable)
		while (result->next()) {
			for (int i = 0; i < TongCot; i++) { 
				DuLieu[x2][i] = result->getString(i+1);
			}
			x1++;	//	Đếm có tổng cộng bao nhiêu row trong table (Count how many row exists)
			x2++;	//	Đếm có tổng cộng bao nhiêu row trong table (Count how many row exists)
		}
		std::cout << std::endl;
		for (int i = 0; i <= x1; i++) {	// Số vòng lặp cout bằng với số row đã đếm (The number of cout loops is equal to the number of rows counted*x1)
			for (int a = 0; a < TongCot; a++) {
				std::cout << DuLieu[i][a] << "\t\t";
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

	//- 1. Hiện các table có sẵn trong Database (Show the available tables in Database)
	std::cout << "Danh sách table:   ";
	try {
		std::string TaoQuery;
		TaoQuery = "SHOW TABLES;";
		pstmt = con->prepareStatement(TaoQuery);
		result = pstmt->executeQuery();
		while (result->next()) {
			std::string i = result->getString(1);
			std::cout << i << " ";
		}
	}
	catch (sql::SQLException e) {
		//	std::cout << std::endl;
		//	std::cout << e.what() << std::endl;	//	DEBUG ONLY
	}
}

void cmysql::AddTable(std::string t, int c) {
	/*	Thêm table vào Database (Add tables to the Database)	*/

	//- 1. Lấy dữ liệu từ input (Get datas from input)
	std::string TenCot;
	std::string LoaiCot;
	std::string TaoQuery;
	TaoQuery = "CREATE TABLE " + t + " (";
	std::cout << "Danh sách các loại cột có sẵn: serial, serialPRIMARY, varchar, varcharPRIMARY, integer, integerPRIMARY\n";
	std::cout << std::endl;
	for (int i = 0; i < c; i++) {
		std::cout << "Nhập tên cột (" << i + 1 << "): ";
		std::cin >> TenCot;
		std::cout << "Nhập loại cột (" << i + 1 << "): ";
		std::cin >> LoaiCot;
		if (LoaiCot == "serial") LoaiCot = "serial";
		if (LoaiCot == "serialPRIMARY") LoaiCot = "serial PRIMARY KEY";
		if (LoaiCot == "varchar") LoaiCot = "VARCHAR(100)";
		if (LoaiCot == "varcharPRIMARY") LoaiCot = "VARCHAR(100) PRIMARY KEY";
		if (LoaiCot == "integer") LoaiCot = "INTEGER";
		if (LoaiCot == "integerPRIMARY") LoaiCot = "INTEGER PRIMARY KEY";
		TaoQuery.append(TenCot + " " + LoaiCot);
		if (i < c - 1) TaoQuery.append(",");
	}
	TaoQuery.append(")");

	//- 2. Thêm table vào Database (Add tables to the Database)
	try {
		pstmt = con->prepareStatement(TaoQuery);
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

	//-	1. Xoá Table khỏi Database (Delete Tables from the Database)
	try {
		std::string TaoQuery;
		TaoQuery = "DROP TABLE IF EXISTS " + x;
		pstmt = con->prepareStatement(TaoQuery);
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

	//- 2. Cout danh sách các cột (Cout list of columns)
	std::string TenCot;
	std::string DuLieuCot;
	std::string TaoQuery;
	std::string QueryPhu;
	int TongCot;
	std::cout << "Danh sách các cột:   ";
	try {
		std::string TaoQuery2;
		TaoQuery2 = "SHOW COLUMNS FROM " + t;
		pstmt = con->prepareStatement(TaoQuery2);
		result = pstmt->executeQuery();
		while (result->next()) {
			std::string i = result->getString(1);
			std::cout << i << " ";
		}
	}
	catch (sql::SQLException e) {
		std::cout << std::endl;
		//	std::cout << e.what() << std::endl;	//	DEBUG ONLY
		std::cout << "Table không tồn tại ";
		aniSQLObj.dotAnimation(1000);
		goto escape1;	//	Thoát khỏi vòng lặp vì Table không tồn tại (Break the loop to try again because table doesn't exists)
	}
	TaoQuery = "INSERT INTO " + t + "(";
	std::cout << std::endl;
	std::cout << "Đối với loại cột 'varchar' phải thêm \"\" (Ví dụ: \"text\")" << std::endl;
	std::cout << "Nhập tổng số cột muốn thêm dữ liệu: ";
	std::cin >> TongCot;
	
	//-	3. Thêm dữ liệu vào Table (Add data to the Tables)
	try {
		for (int i = 0; i < TongCot; i++) {
			std::cout << "Nhập tên cột (" << i + 1 << "): ";
			std::cin >> TenCot;
			std::cout << "Nhập dữ liệu cột (" << i + 1 << "): ";
			std::cin >> DuLieuCot;
			TaoQuery.append(TenCot);
			QueryPhu.append(DuLieuCot);
			if (i < TongCot - 1) TaoQuery.append(",");
			if (i < TongCot - 1) QueryPhu.append(",");
		}
		TaoQuery.append(") VALUES (" + QueryPhu + ")");
		pstmt = con->prepareStatement(TaoQuery);
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