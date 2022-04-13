#include <iostream>
#include <Windows.h>
#include "..\Sources\Headers\animation.h"

/*	Định nghĩa các hàm của Animation (Define Animation Class Functions)	*/
void animation::dotAnimation(int x) {
	for (int i = 0; i < 3; i++) {
		Sleep(x);
		std::cout << ".";
	}
}