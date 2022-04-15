﻿#pragma once
/*	Class: cMySQL - Các hàm prototype của cMySQL	*/
class cmysql {
private:
	std::string sqlserver;
	std::string sqluser;
	std::string sqlpwd;
public:
	cmysql() {	//	Constructor
	}
	~cmysql() {	//	Destructor
	}

	void ConfigFile();
	void Connect();
	void ConnectDB();
	void ReadTable(std::string t);
	void ShowTables();
	void AddTable(std::string t, int c);
	void DeleteTable(std::string t);
	void InsertData(std::string t);
	void UpdateData();
};

