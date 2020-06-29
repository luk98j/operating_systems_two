#include <iostream>
#include <fstream> 
#include <windows.h>
#include <tchar.h>
#include <thread>

HANDLE createFile(){

	HANDLE hFile = CreateFile(
		"log.txt",
		FILE_APPEND_DATA,
		0,
		NULL,
		OPEN_ALWAYS,
		FILE_ATTRIBUTE_NORMAL,
		NULL
	);
	if(hFile){
		std::cout<<"File created"<<std::endl;
	}
	return hFile;
}


bool _writeLineToFile(HANDLE h){
	std::cout<<"Enter text: "<<std::endl;
	std::string text;
	getline(std::cin, text);
	text = text + "\n";
	const char *cstr = text.c_str();	
	BOOL bFile;
	DWORD bytesWritten = 0;
	DWORD bytesToWrite = strlen(cstr);
        bFile = WriteFile(
		h,
        cstr,
        bytesToWrite,
        &bytesWritten,
        NULL);
 

CloseHandle(h);
return bFile;
}

int main(int argc, TCHAR *argv[], TCHAR *envp[]) {
	HANDLE hFile = createFile();
	_writeLineToFile(hFile);
}

