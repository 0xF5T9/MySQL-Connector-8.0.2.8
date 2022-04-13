#include <iostream>
#include <Windows.h>
#include "..\Sources\Headers\animation.h"

void animation::dotAnimation(int x) {
	for (int i = 0; i < 3; i++) {
		Sleep(x);
		std::cout << ".";
	}
}