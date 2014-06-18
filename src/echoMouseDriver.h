#pragma once

#define _WIN32_WINNT 0x0500

#include "ofMain.h"
#include <windows.h>

//TODO:
//1) ������������ hwnd ���������� ����

class echoMouseDriver{
	public:
		echoMouseDriver();
		~echoMouseDriver();

		//��������� �������� �������� ����
		void mouseMove(int dx, int dy);
		//��������� �������� ������� ����� ������ ����
		void mouseButton1Event(bool state);
		//��������� �������� ������� ������ ������ ����
		void mouseButton2Event(bool state);
		//��������� �������� ������� �������
		void mouseButton3Event(bool state);

	private:
		//���������� ������� ����
		POINT mousePos;
		int mouseX;
		int mouseY;
};