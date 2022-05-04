#include <iostream>
#include <fstream>
#include <Windows.h>

#include "mysql_connection.h"
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/prepared_statement.h>
#include "../Sources/Headers/cmysql.h"
#include "../Sources/Headers/menu.h"

sql::Driver* driver;
sql::Connection* con;
sql::Statement* stmt;
sql::PreparedStatement* pstmt;
sql::ResultSet* result;

/*	Define 'cmysql' class functions	*/

void cmysql::ConfigFile() {
	std::ifstream cFile("server.cfg");	//	Ifstream for reading
	if (cFile.is_open()) {	//	Check if the file "server.cfg" is exists
		std::string line;
		while (getline(cFile, line))
		{
			line.erase(std::remove_if(line.begin(), line.end(), isspace), line.end());
			if (line[0] == '#' || line.empty()) continue;

			auto delimiterPos = line.find("=");
			auto name = line.substr(0, delimiterPos);
			auto value = line.substr(delimiterPos + 1);

			//	Scan and read data from file
			if (name == "server") sqlserver = value;
			else if (name == "user") sqluser = value;
			else if (name == "pass") sqlpwd = value;
		}
	}
	else {
		std::ofstream gFile("server.cfg");	//	Ofstream for writing
		std::cout << "không tìm thấy file cài đặt kết nối 'server.cfg', đang tạo file";
		gFile << "# Cài đặt kết nối\nserver = tcp://localhost:3306\nuser = root\npass = 123456" << std::endl;	//	Writing default config file
		Animation->DotAnimation(300);
		std::cout << "\nĐã tạo file 'server.cfg', hãy nhập cài đặt kết nối và thử lại.\n";
		gFile.close();	// Close file
		system("pause");
		exit(1);
	}
}

void cmysql::Connect() {
	/*	Connect to MySQL Server	*/

	//- 1. Connect to MySQL Server
	//- Close the program if fails to connect 3 times
	int SoLanThuKetNoi = 0;	//	Counting variable
	bool KetNoiThanhCong = false;
	while (true) {	//	Loop untill success or not
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
			Animation->DotAnimation(500);
			SoLanThuKetNoi++;	//	+1 per connect attempt
			system("cls");
		}
		if (SoLanThuKetNoi >= 3) break;	
	}
	if (KetNoiThanhCong == false) std::cout << "Kết nối thất bại, kiểm tra cài đặt kết nối ở file 'server.cfg' và thử lại.\n",system("pause");
	if (SoLanThuKetNoi >= 3) exit(1);	//	Close the program if fails to connect 3 times
}

void cmysql::ConnectDB() {
	/*	Connect to database	*/

	//- 1. Connect to database
	//- Close the program if fails to connect 3 times
	while (true) {
		try {
			ShowDatabases();
			std::cout << "Chọn database: ";
			std::cin >> sqldb;
			con->setSchema(sqldb);
			defaultsqldb = sqldb;	//	Save the database name that have been successfully connected before
			system("cls");
			break;
		}
		catch (sql::SQLException e) {
			//	cout << e.what() << endl;	//	DEBUG ONLY
			std::cout << "Database không tồn tại hoặc có sự cố kết nối ";
			Animation->DotAnimation(500);
			std::cin.clear();
			std::cin.ignore(10000, '\n');
			system("cls");
		}
	}
}

void cmysql::ReadTable(std::string t) {
	/*	Read datas from Table	*/

	//-	1. Count total columns in Table
	//- Cout list of columns
	int TongCot = 0;	//	Counting variable
	try {
		std::string TaoQuery;
		TaoQuery = "SHOW COLUMNS FROM " + t;
		pstmt = con->prepareStatement(TaoQuery);
		result = pstmt->executeQuery();
		while (result->next()) {	//	Loop cout column name
			std::string i = result->getString(1);
			std::cout << i << "\t\t";
			TongCot++;	//	+1 per loop
		}
	}
	catch (sql::SQLException e) {
		std::cout << std::endl;
		//	std::cout << e.what() << std::endl;	//	DEBUG ONLY
		std::cout << "Table không tồn tại ";
		Animation->DotAnimation(1000);
		goto escape0;	//	Break the loop to try again because table doesn't exists
	}

	//- 2. Get and cout datas from table
	try {
		std::string TaoQuery;
		std::string DuLieu[100][100];	//	Create 2-dimensions variable to store datas
		TaoQuery = "SELECT * FROM " + t + ";";
		pstmt = con->prepareStatement(TaoQuery);
		result = pstmt->executeQuery();
		int x1 = 0;	//	Counting variable
		int x2 = 0;	//	Counting variable
		while (result->next()) {
			for (int i = 0; i < TongCot; i++) { 
				DuLieu[x2][i] = result->getString(i+1);
			}
			x1++;	//	Count how many row exists
			x2++;	//	Count how many row exists
		}
		std::cout << std::endl;
		for (int i = 0; i <= x1; i++) {	// The number of cout loops is equal to the number of rows counted*x1
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
		Animation->DotAnimation(1000);
	}
	escape0:
	std::cout << "";
}

void cmysql::ShowDatabases() {
	/*	Show the available tables in Database	*/

	//- 1. Show the available tables in Database
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
	/*	Show the available tables in Database	*/

	//- 1. Show the available tables in Database
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
	/*	Add tables to database	*/

	//- 1. Get datas from input
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

	//- 2. Add tables to database
	try {
		pstmt = con->prepareStatement(TaoQuery);
		pstmt->execute();
		std::cout << std::endl;
		std::cout << "Đang tạo table " << t << " ";
		delete pstmt;
		Animation->DotAnimation(1000);
	}
	catch (sql::SQLException e) {
			std::cout << std::endl;
			std::cout << "Table đã tồn tồn tại hoặc loại cột không hợp lệ ";
			Animation->DotAnimation(1000);
			//std::cout << e.what() << std::endl;	//	DEBUG ONLY
	}
}

void cmysql::DeleteTable(std::string t) {
	/*	Delete Tables from the Database	*/

	//-	1. Delete Tables from the Database
	try {
		std::string TaoQuery;
		TaoQuery = "DROP TABLE IF EXISTS " + t;
		pstmt = con->prepareStatement(TaoQuery);
		pstmt->execute();
		std::cout << std::endl;
		std::cout << "Đang xoá table " << t << " ";
		delete pstmt;
		Animation->DotAnimation(1000);
	}
	catch (sql::SQLException e) {
		//	std::cout << std::endl;
		//	std::cout << e.what() << std::endl;	//	DEBUG ONLY
	}
}

void cmysql::InsertData(std::string t) {
	/*	Add data to table	*/

	//- 2. Cout list of columns
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
		Animation->DotAnimation(1000);
		goto escape1;	//	Break the loop to try again because table doesn't exists
	}
	TaoQuery = "INSERT INTO " + t + "(";
	std::cout << std::endl;
	std::cout << "Đối với loại cột 'varchar' phải thêm \"\" (Ví dụ: \"text\")" << std::endl;
	std::cout << "Nhập tổng số cột muốn thêm dữ liệu: ";
	std::cin >> TongCot;
	
	//-	3. Add datas to table
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
		Animation->DotAnimation(1000);
	}
	catch (sql::SQLException e) {
		std::cout << std::endl;
		//	std::cout << e.what() << std::endl;	//	DEBUG ONLY
		std::cout << "Table không tồn tại hoặc dữ liệu không hợp lệ ";
		Animation->DotAnimation(1000);
	}
	escape1:
	std::cout << "";
}

void cmysql::UpdateData() {
	/*	Update datas for table	*/
	std::string c, u, d, h;	//	Column, Update, Data, Hàng

	//-	1. Count total columns in Table
	//- Cout list of columns
	int TongCot = 0;	//	Counting variable
	std::string t;
	std::cout << "Chọn Table: ";
	std::cin >> t;
	try {
		std::string TaoQuery;
		TaoQuery = "SHOW COLUMNS FROM " + t;
		pstmt = con->prepareStatement(TaoQuery);
		result = pstmt->executeQuery();
		while (result->next()) {	//	Loop cout column name
			std::string i = result->getString(1);
			std::cout << i << "\t\t";
			TongCot++;	//	+1 per loop
		}
	}
	catch (sql::SQLException e) {
		std::cout << std::endl;
		//	std::cout << e.what() << std::endl;	//	DEBUG ONLY
		std::cout << "Table không tồn tại ";
		Animation->DotAnimation(1000);
		goto escape2;	//	Break the loop to try again because table doesn't exists
	}

	//- 2. Get and cout datas from table
	try {
		std::string TaoQuery;
		std::string DuLieu[100][100];	//	Create 2-dimensions variable to store datas
		TaoQuery = "SELECT * FROM " + t + ";";
		pstmt = con->prepareStatement(TaoQuery);
		result = pstmt->executeQuery();
		int x1 = 0;	//	Counting variable
		int x2 = 0;	//	Counting variable
		while (result->next()) {
			for (int i = 0; i < TongCot; i++) {
				DuLieu[x2][i] = result->getString(i + 1);
			}
			x1++;	//	Count how many row exists
			x2++;	//	Count how many row exists
		}
		std::cout << std::endl;
		for (int i = 0; i <= x1; i++) {	// The number of cout loops is equal to the number of rows counted*x1
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
		Animation->DotAnimation(1000);
	}

	//-	2. Update datas for the table
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
		Animation->DotAnimation(1000);
	}
	catch (sql::SQLException e) {
		std::cout << std::endl;
		//std::cout << e.what() << std::endl;	//	DEBUG ONLY
		std::cout << "Table/column/id không tồn tại hoặc dữ liệu không hợp lệ ";
		Animation->DotAnimation(1000);
	}
	/*	Set the escape point to break loop if needed	*/
	escape2:
	std::cout << "";
}

void cmysql::ChangeDatabase() {
	/*	Connect to another database	*/

	//- 1. Connect to another database
	//- Abort if database doesn't exists or fails to connect
	int SoLanKetNoi = 1;
	while (true) {
		try {
			Menu->ShowMenu();
			ShowDatabases();
			std::cout << "Chọn database: ";
			std::cin >> sqldb;
			con->setSchema(sqldb);
			defaultsqldb = sqldb;
			system("cls");
			break;
		}
		catch (sql::SQLException e) {
			//	cout << e.what() << endl;	//	DEBUG ONLY
			std::cout << "Database không tồn tại hoặc có sự cố kết nối ";
			Animation->DotAnimation(500);
			std::cin.clear();
			std::cin.ignore(10000, '\n');
			system("cls");
			SoLanKetNoi++;
		}
		if (SoLanKetNoi == 1) sqldb = defaultsqldb; break;	//	Reconnect to the previous connected database
	}
}