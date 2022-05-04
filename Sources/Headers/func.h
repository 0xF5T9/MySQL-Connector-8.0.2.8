#pragma once
#include "animation.h"
#include "cmysql.h"
#include "menu.h"

/*	Define create object functions	*/
animation* CreateObjectAnimation() {
	animation* CreateObject = new animation();
	return CreateObject;
}

cmysql* CreateObjectCMySQL(animation* _Animation) {
	cmysql* CreateObject = new cmysql(_Animation);
	return CreateObject;
}

menu* CreateObjectMenu(animation* _Animation) {
	menu* CreateObject = new menu(_Animation);
	return CreateObject;
}

/*	Define free object memory functions	*/
void FreeMemory(animation* _Ptr) {
	delete _Ptr;
}

void FreeMemory(cmysql* _Ptr) {
	delete _Ptr;
}

void FreeMemory(menu* _Ptr) {
	delete _Ptr;
}