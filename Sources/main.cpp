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

//for demonstration only. never save your password in the code!
animation aniMainObj;
cmysql cmysqlMainObj;
menu menuMainObj;


int main() {

	//	Thiết lập thông số chương trình (Initializing the program parameters)	//
	SetConsoleOutputCP(65001);
	cmysqlMainObj.Connect();
	cmysqlMainObj.ConnectDB();
	int luaChon;
	bool q = false;
	SetConsoleTitle(L"MySQL Test");

	while (q == false) {
		luaChon = menuMainObj.getInput();
		q = menuMainObj.processInput(luaChon);
	}
	exit(0);

	sql::Driver* driver;
	sql::Connection* con;
	sql::Statement* stmt;
	sql::PreparedStatement* pstmt;
	sql::ResultSet* result;
	const std::string server = "tcp://localhost:3306";
	const std::string username = "root";
	const std::string password = "1284";

	exit(0);

	/*	Read and load data from table into variables	*/
	pstmt = con->prepareStatement("SELECT * FROM account;"); //	Prepare statement to execute
	result = pstmt->executeQuery();
	string userdata[1000][3]; 
	int x = 0;
	int y;
	while (result->next()) {
		//printf("Reading from table=(%d, %s, %s)\n", result->getInt(1), result->getString(2).c_str(), result->getString(3).c_str());
		y = result->getInt(1);
		string z = to_string(y);
		userdata[x][0] = z;
		userdata[x][1] = result->getString(2);
		userdata[x][2] = result->getString(3);
		x++;
	}
	for (int i = 0; i < 3; i++) cout << userdata[0][i] << " ";
	cout << endl;
	for (int i = 0; i < 3; i++) cout << userdata[1][i] << " ";
	cout << endl;
	for (int i = 0; i < 3; i++) cout << userdata[2][i] << " ";
	cout << endl;
	delete result;
	delete pstmt;
	/*-----------------------------------------------------------------------------------------------------------------------------*/
	delete con;
	system("pause");
	return 0;
	
}