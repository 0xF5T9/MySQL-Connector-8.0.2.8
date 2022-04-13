#pragma once
/*	Class: cMySQL - Các hàm prototype của cMySQL	*/
class cmysql {
private:
	const std::string server = "tcp://localhost:3306";
	const std::string username = "root";
	const std::string password = "1284";
public:
	void Connect();
	void ConnectDB();
	void ReadTable(std::string t);
	void ShowTables();
	void AddTable(std::string x);
	void DeleteTable(std::string x);
	void InsertData(std::string t, std::string u, std::string p);
	void UpdateData();
};

