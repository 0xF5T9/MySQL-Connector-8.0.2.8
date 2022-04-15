#include <stdlib.h>
#include <iostream>
#include <Windows.h>

#include "mysql_connection.h"
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/prepared_statement.h>
#include "..\Sources\Headers\animation.h"
#include "..\Sources\Headers\cmysql.h"
#include "..\Sources\Headers\menu.h"

using namespace std;

/*	Tạo object từ classes (Create objects from classes)	*/
animation aniMainObj;
cmysql cmysqlMainObj;
menu menuMainObj;


int main() {
	/*	Thiết lập thông số chương trình (Initializing the program parameters)	*/
	int luaChon;
	bool q = false;
	SetConsoleOutputCP(65001);	//	Set codepage UTF-8 - hỗ trợ tiếng việt
	SetConsoleTitle(L"Loading ...");
	cmysqlMainObj.ConfigFile();	//	Lấy dữ liệu kết nối máy chủ MySQL từ file server.cfg
	cmysqlMainObj.Connect();	//	Kết nối máy chủ MySQL (Connect to the MySQL Server)
	cmysqlMainObj.ConnectDB();	//	Kết nối Database (Connect to the Database)
	SetConsoleTitle(L"MySQL Connector with Demo Functions");

	/*	Khởi chạy chương trình (Start the program)	*/
	while (q == false) {
		luaChon = menuMainObj.getInput();
		q = menuMainObj.processInput(luaChon);
	}
	/*	Kết thúc chương trình (Ending the program)	*/
	return 0;
}