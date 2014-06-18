#pragma once

#include "windows.h"
#include "stdio.h"
#include <iostream>

typedef enum _ParamsFileWin{
	_FOR_OPEN = 0,
	_FOR_SAVE = 1,    
}ParamsFileWin;

class FileSystem
{
	public:
		FileSystem();
		~FileSystem();

		void SetFilter(wchar_t* _f);
		wchar_t* GetFileNameW(const ParamsFileWin attrib, int mode);

		std::string openFile();
		std::string saveFile();
		std::string openXmlFile();

		HWND hWnd;
};