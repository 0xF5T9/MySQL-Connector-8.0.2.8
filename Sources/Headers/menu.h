#pragma once
#ifndef menu_h_
#define menu_h_

#include "../Sources/Headers/animation.h"
class cmysql;

class menu {
private:

	/*	Class Properties (Private)	*/
	animation* Animation;
	cmysql* CMySQL;

public:
	menu() {	//	Constructor

	}
	menu(animation* _Animation) {
		this->Animation = _Animation;
	}
	menu(const menu& _copy) {	//	Copy Constructor

	}
	~menu() {	//	Destructor

	}

	/*	Class function prototypes	*/
	void GetObj(cmysql* _CMySQL) {
		this->CMySQL = _CMySQL;
	}
	void ShowMenu() const;
	int SelectOption();
	bool ProcessOption(int _option);
};

#endif // !menu_h_