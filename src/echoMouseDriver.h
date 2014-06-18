#pragma once

#define _WIN32_WINNT 0x0500

#include "ofMain.h"
#include <windows.h>

//TODO:
//1) Автоперехват hwnd следующего окна

class echoMouseDriver{
	public:
		echoMouseDriver();
		~echoMouseDriver();

		//Процедура имитации движения мыши
		void mouseMove(int dx, int dy);
		//Процедура имитации нажатия левой кнопки мыши
		void mouseButton1Event(bool state);
		//Процедура имитации нажатия правой кнопки мыши
		void mouseButton2Event(bool state);
		//Процедура имитации нажатия скролла
		void mouseButton3Event(bool state);

	private:
		//Координаты курсора мыши
		POINT mousePos;
		int mouseX;
		int mouseY;
};