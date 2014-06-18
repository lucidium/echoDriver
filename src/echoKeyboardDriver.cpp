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

	// Нажатие клавиши "R"
	ip.ki.wVk = 0x52; // виртуальный код клавиши "R"
	ip.ki.dwFlags = 0; // используем клавиатуру
	SendInput(1, &ip, sizeof(INPUT));
 
	// Освобождаем клавишу
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

	// Нажатие клавиши "X"
	ip.ki.wVk = 0x58; // виртуальный код клавиши "X"
	ip.ki.dwFlags = 0; // используем клавиатуру
	SendInput(1, &ip, sizeof(INPUT));
 
	// Освобождаем клавишу
	ip.ki.dwFlags = KEYEVENTF_KEYUP;
	SendInput(1, &ip, sizeof(INPUT));
}