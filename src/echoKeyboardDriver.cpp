#include "echoKeyboardDriver.h"

echoKeyboardDriver::echoKeyboardDriver()
{

}

echoKeyboardDriver::~echoKeyboardDriver()
{

}

void echoKeyboardDriver::pressReloadKey()
{
	INPUT ip;

	ip.type = INPUT_KEYBOARD;
	ip.ki.wScan = 0;
	ip.ki.time = 0;
	ip.ki.dwExtraInfo = 0;

	// ������� ������� "R"
	ip.ki.wVk = 0x52; // ����������� ��� ������� "R"
	ip.ki.dwFlags = 0; // ���������� ����������
	SendInput(1, &ip, sizeof(INPUT));
 
	// ����������� �������
	ip.ki.dwFlags = KEYEVENTF_KEYUP;
	SendInput(1, &ip, sizeof(INPUT));
}

void echoKeyboardDriver::pressFireModeKey()
{
	INPUT ip;

	ip.type = INPUT_KEYBOARD;
	ip.ki.wScan = 0;
	ip.ki.time = 0;
	ip.ki.dwExtraInfo = 0;

	// ������� ������� "X"
	ip.ki.wVk = 0x58; // ����������� ��� ������� "X"
	ip.ki.dwFlags = 0; // ���������� ����������
	SendInput(1, &ip, sizeof(INPUT));
 
	// ����������� �������
	ip.ki.dwFlags = KEYEVENTF_KEYUP;
	SendInput(1, &ip, sizeof(INPUT));
}