#pragma once

#define _WIN32_WINNT 0x0500

#include "ofMain.h"
#include <windows.h>

class echoKeyboardDriver{
	public:

		echoKeyboardDriver();
		~echoKeyboardDriver();

		//��������� �������� ������� ������� �����������
		void pressReloadKey();
		//��������� �������� ������� ������� ����� ������ ����
		void pressFireModeKey();
		void allowKeyStrokes();

	private:

};