#pragma once
#ifndef cmysql_h_
#define cmysql_h_

#include "../Sources/Headers/animation.h"
class menu;

class cmysql {
private:

	/*	Class Properties (Private)	*/
	animation* Animation;
	menu* Menu;
	std::string sqlserver;
	std::string sqluser;
	std::string sqlpwd;
	std::string sqldb;
	std::string defaultsqldb;

public:
	cmysql() {	//	Constructor
	}
	cmysql(animation* _Animation) {
		this->Animation = _Animation;
	}
	cmysql(const cmysql& _copy) {	//	Copy Constructor

	}
	~cmysql() {	//	Destructor
	}

	/*	Class function prototypes	*/
	void GetObj(menu* _Menu) {
		this->Menu = _Menu;
	}
	void ConfigFile();
	void Connect();
	void ConnectDB();
	void ReadTable(std::string t);
	void ShowDatabases();
	void ShowTables();
	void AddTable(std::string t, int c);
	void DeleteTable(std::string t);
	void InsertData(std::string t);
	void UpdateData();
	void ChangeDatabase();
};

#endif // !cmysql_h_