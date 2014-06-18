#pragma once

#define _WIN32_WINNT 0x0500

#include "ofMain.h"
#include <windows.h>

class echoKeyboardDriver{
	public:

		echoKeyboardDriver();
		~echoKeyboardDriver();

		//Процедура имитации нажатия клавиши перезарядки
		void pressReloadKey();
		//Процедура имитации нажатия клавиши смены режима огня
		void pressFireModeKey();
		void allowKeyStrokes();

	private:

};