#include <iostream>
#include <Windows.h>
#include "..\Sources\Headers\animation.h"

/*	Define 'animation' class functions	*/
void animation::DotAnimation(int _miliseconds) {
	for (int i = 0; i < 3; i++) {
		Sleep(_miliseconds);
		std::cout << ".";
	}
}