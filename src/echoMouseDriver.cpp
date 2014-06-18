#include "echoMouseDriver.h"

echoMouseDriver::echoMouseDriver()
{
	GetCursorPos(&mousePos);

	mouseX = mousePos.x;
	mouseY = mousePos.y;
}

echoMouseDriver::~echoMouseDriver()
{

}

void echoMouseDriver::mouseMove(int dx, int dy)
{
	mouseX += dx;
	mouseY += dy;

	//SetCursorPos(mouseX, mouseY);

	//Раскомментировать для работы с DirectX полноэкранными приложениями
	INPUT inp[1];
	inp[0].type = INPUT_MOUSE;
    inp[0].mi.dwFlags = MOUSEEVENTF_MOVE;
    inp[0].mi.dx = dx;
    inp[0].mi.dy = dy;

	SendInput(1, inp, sizeof(INPUT));
}

void echoMouseDriver::mouseButton1Event(bool state)
{
	INPUT mouseIpt[2];
	ZeroMemory(mouseIpt, sizeof(INPUT) * 2);

	if(state)
	{
		mouseIpt[0].type = INPUT_MOUSE;
		mouseIpt[0].mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
	}
	else
	{
		mouseIpt[1].type = INPUT_MOUSE;
		mouseIpt[1].mi.dwFlags = MOUSEEVENTF_LEFTUP;
	}

	SendInput(2, mouseIpt, sizeof(INPUT));
}

void echoMouseDriver::mouseButton2Event(bool state)
{
	INPUT mouseIpt[2];
	ZeroMemory(mouseIpt, sizeof(INPUT) * 2);

	if(state)
	{
		mouseIpt[0].type = INPUT_MOUSE;
		mouseIpt[0].mi.dwFlags = MOUSEEVENTF_RIGHTDOWN;
	}
	else
	{
		mouseIpt[1].type = INPUT_MOUSE;
		mouseIpt[1].mi.dwFlags = MOUSEEVENTF_RIGHTUP;
	}

	SendInput(2, mouseIpt, sizeof(INPUT));
}

void echoMouseDriver::mouseButton3Event(bool state)
{
	INPUT mouseIpt[2];
	ZeroMemory(mouseIpt, sizeof(INPUT) * 2);

	if(state)
	{
		mouseIpt[0].type = INPUT_MOUSE;
		mouseIpt[0].mi.dwFlags = MOUSEEVENTF_MIDDLEDOWN;
	}
	else
	{
		mouseIpt[1].type = INPUT_MOUSE;
		mouseIpt[1].mi.dwFlags = MOUSEEVENTF_MIDDLEUP;
	}

	SendInput(2, mouseIpt, sizeof(INPUT));
}