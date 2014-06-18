#include "echoFilesystem.h"

BOOL WINAPI GetOpenFileName(
  _Inout_  LPOPENFILENAME lpofn
);

FileSystem::FileSystem(){}
FileSystem::~FileSystem(){}


static wchar_t* filter_w = 0;

void FileSystem::SetFilter(wchar_t* _f)
{
	size_t sz = wcslen(_f);
	filter_w = new wchar_t[sz]; 
	filter_w = _f;
}

wchar_t* FileSystem::GetFileNameW(const ParamsFileWin attrib, int mode)
{


	  static wchar_t file_name[1024] = {0};
	  static OPENFILENAMEW file_params = {0};

	  file_params.lStructSize = sizeof(OPENFILENAME);
	  file_params.hwndOwner = NULL;
	  switch(mode)
	  {
		  case 0: file_params.lpstrFilter = L"Image files (*.png)\0 *.png\0"; break;
		  case 1: file_params.lpstrFilter = L"Preset files (*.edp)\0 *.edp\0"; break;
	  }
  
	  file_params.lpstrFile = file_name;
	  file_params.nMaxFile = 1024;
	  file_params.lpstrInitialDir = 0;
	  file_params.lpstrFileTitle = 0;

	  file_params.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_NOCHANGEDIR
				| OFN_HIDEREADONLY;
	


	  switch(attrib)
	  {
			case _FOR_OPEN:
			  if(GetOpenFileNameW(&file_params))
				return file_params.lpstrFile;      
			  break;

			case _FOR_SAVE:
			  if(GetSaveFileNameW(&file_params))
				return file_params.lpstrFile;
			  break;
	  }
  
	  return 0;
}

std::string FileSystem::openFile()
{
	wchar_t* path = GetFileNameW(_FOR_OPEN,0);
	if(path)
	{
		std::wstring ws(path);
		std::string ss(ws.begin(), ws.end());
		std::string s_path = ss;

		return s_path;

	}
	else return std::string("");
}

std::string FileSystem::openXmlFile()
{
	wchar_t* path = GetFileNameW(_FOR_OPEN,1);
	if(path)
	{
		std::wstring ws(path);
		std::string ss(ws.begin(), ws.end());
		std::string s_path = ss;

		return s_path;

	}
	else return std::string("");
}

std::string FileSystem::saveFile()
{
	wchar_t* path = GetFileNameW(_FOR_SAVE,1);
	if(path)
	{
		std::wstring ws(path);
		std::string ss(ws.begin(), ws.end());
		std::string s_path = ss;

		return s_path;

	}
	else return std::string("");
}
