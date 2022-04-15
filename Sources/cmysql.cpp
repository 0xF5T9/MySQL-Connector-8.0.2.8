#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <Windows.h>

#include "mysql_connection.h"
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/prepared_statement.h>
#include "..\Sources\Headers\cmysql.h"
#include "..\Sources\Headers\animation.h"

/*	Tạo object từ classes (Create objects from classes)	*/
animation aniSQLObj;
sql::Driver* driver;
sql::Connection* con;
sql::Statement* stmt;
sql::PreparedStatement* pstmt;
sql::ResultSet* result;

/*	Định nghĩa các hàm của cMySQL (Define cMySQL Class Functions)	*/
void cmysql::ConfigFile() {
	std::ifstream cFile("server.cfg");	//	Tạo biến input-stream xử lý đầu vào (Ifstream for reading)
	if (cFile.is_open()) {	//	Kiểm tra nếu file server.cfg tồn tại (Check if the file "server.cfg" is exists)
		std::string line;
		while (getline(cFile, line))
		{
			line.erase(std::remove_if(line.begin(), line.end(), isspace), line.end());
			if (line[0] == '#' || line.empty()) continue;

			auto delimiterPos = line.find("=");
			auto name = line.substr(0, delimiterPos);
			auto value = line.substr(delimiterPos + 1);

			//	Truy và đọc dữ liệu từ file (Scan and read data from file)
			if (name == "server") sqlserver = value;
			else if (name == "user") sqluser = value;
			else if (name == "pass") sqlpwd = value;
		}
	}
	else {
		std::ofstream gFile("server.cfg");	//	Tạo biến output-stream xử lý đầu ra (Ofstream for writing)
		std::cout << "không tìm thấy file cài đặt kết nối 'server.cfg', đang tạo file";
		gFile << "# Cài đặt kết nối\nserver = tcp://localhost:3306\nuser = root\npass = 123456" << std::endl;	//	Viết cài đặt kết nối mặc định vào file (Writing default config file)
		aniSQLObj.dotAnimation(300);
		std::cout << "\nĐã tạo file 'server.cfg', hãy nhập cài đặt kết nối và thử lại.\n";
		gFile.close();	// Đóng file (Close file)
		system("pause");
		exit(1);
	}
}

void cmysql::Connect() {
	/*	Kết nối máy chủ MySQL (Connect to MySQL Server)	*/

	//- 1. Kết nối máy chủ MySQL (Connect to MySQL Server)
	//- Đóng chương trình nếu kết nối thất bại 3 lần (Close the program if fails to connect 3 times)
	int SoLanThuKetNoi = 0;	//	Biến đếm (Counting variable)
	bool KetNoiThanhCong = false;
	while (true) {	//	Vòng lặp vĩnh cữu (Loop untill success or not)
		try
		{
			driver = get_driver_instance();
			con = driver->connect(sqlserver, sqluser, sqlpwd);
			KetNoiThanhCong = true;
			break;
		}
		catch (sql::SQLException e)
		{
			std::cout << "Đang kết nối tới máy chủ MySQL ";
			//	cout << e.what() << endl;	//	DEBUG ONLY
			aniSQLObj.dotAnimation(500);
			SoLanThuKetNoi++;	//	+1 mỗi lần thử kết nối (+1 per connect attempt)
			system("cls");
		}
		if (SoLanThuKetNoi >= 3) break;	
	}
	if (KetNoiThanhCong == false) std::cout << "Kết nối thất bại, kiểm tra cài đặt kết nối ở file 'server.cfg' và thử lại.\n",system("pause");
	if (SoLanThuKetNoi >= 3) exit(1);	//	Đóng chương trình nếu kết nối thất bại 3 lần (Close the program if fails to connect 3 times)
}

void cmysql::ConnectDB() {
	/*	Kết nối đến Database (Connect to Database)	*/

	//- 1. Kết nối đến Database (Connect to Database)
	//- Đóng chương trình nếu kết nối thất bại 3 lần (Close the program if fails to connect 3 times)
	std::string database;
	while (true) {
		try {
			ShowDatabases();
			std::cout << "Chọn database: ";
			std::cin >> database;
			con->setSchema(database);
			system("cls");
			break;
		}
		catch (sql::SQLException e) {
			//	cout << e.what() << endl;	//	DEBUG ONLY
			std::cout << "Database không tồn tại hoặc có sự cố kết nối ";
			aniSQLObj.dotAnimation(500);
			std::cin.clear();
			std::cin.ignore(10000, '\n');
			system("cls");
		}
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

void cmysql::ShowDatabases() {
	/*	Hiện các table có sẵn trong Database (Show the available tables in Database)	*/

	//- 1. Hiện các table có sẵn trong Database (Show the available tables in Database)
	std::cout << "Danh sách database:   ";
	try {
		std::string TaoQuery;
		TaoQuery = "SHOW DATABASES;";
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
	std::cout << std::endl;
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
	/*	Thêm table vào Database (Add tables to database)	*/

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

	//- 2. Thêm table vào Database (Add tables to database)
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

void cmysql::DeleteTable(std::string t) {
	/*	Xoá Table khỏi Database (Delete Tables from the Database)	*/

	//-	1. Xoá Table khỏi Database (Delete Tables from the Database)
	try {
		std::string TaoQuery;
		TaoQuery = "DROP TABLE IF EXISTS " + t;
		pstmt = con->prepareStatement(TaoQuery);
		pstmt->execute();
		std::cout << std::endl;
		std::cout << "Đang xoá table " << t << " ";
		delete pstmt;
		aniSQLObj.dotAnimation(1000);
	}
	catch (sql::SQLException e) {
		//	std::cout << std::endl;
		//	std::cout << e.what() << std::endl;	//	DEBUG ONLY
	}
}

void cmysql::InsertData(std::string t) {
	/*	Thêm dữ liệu vào Table (Add data sto table)	*/

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
	
	//-	3. Thêm dữ liệu vào table (Add datas to table)
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
	/*	Cập nhật dữ liệu cho table (Update datas for table)	*/
	std::string c, u, d, h;	//	Column, Update, Data, Hàng

	//-	1. Đếm số cột trong Table (Count total columns in Table)
	//- Cout danh sách các cột (Cout list of columns)
	int TongCot = 0;	//	Biến đếm (Counting variable)
	std::string t;
	std::cout << "Chọn Table: ";
	std::cin >> t;
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
		goto escape2;	//	Thoát khỏi vòng lặp vì Table không tồn tại (Break the loop to try again because table doesn't exists)
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
				DuLieu[x2][i] = result->getString(i + 1);
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
	}
	catch (sql::SQLException e) {
		std::cout << std::endl;
		//	std::cout << e.what() << std::endl;	//	DEBUG ONLY
		std::cout << "Table không tồn tại ";
		aniSQLObj.dotAnimation(1000);
	}

	//-	2. Cập nhật dữ liệu cho Table (Update datas for the table)
	std::cout << std::endl;
	std::cout << "Đối với loại cột 'varchar' phải thêm \"\" (Ví dụ: \"text\")" << std::endl;
	std::cout << "Chọn cột để chọn hàng muốn update: ";
	std::cin >> h;
	std::cout << "Chọn hàng muốn update: ";
	std::cin >> d;
	std::cout << "Chọn cột muốn update: ";
	std::cin >> c;
	std::cout << "Nhập dữ liệu update: ";
	std::cin >> u;
	try {
		std::string TaoQuery;
		TaoQuery = "UPDATE " + t + " SET " + c + " = " + u + " WHERE " + h + " = " + d;
		pstmt = con->prepareStatement(TaoQuery);
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