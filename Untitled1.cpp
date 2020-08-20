#include <iostream>
#include <fstream> 
#include <windows.h>
#include <tchar.h>

/**
 * @file Untitled1
 * @author Lukasz Jankowski
 * @date 29.06.2020
 * @brief EX 2 - Systemy operacyjne II, cwiczenie 2 - LAB
 	Program nie spelnia oczekiwan prowadzacego - tzn. nie jest w jednym pliku i nie moglem wykorzystac
 	parametru CreateFile jako mutexa. Wolalem oddac jakas swoja prace w terminie niz pozostac z niczym.
 	Jezeli ma Pan zastrzezenia i moglby Pan podpowiedziec jak to zrealizowac w jednym pliku prosilbym
 	o krotka informacje na maila. 
 	Dziekuje i pozdrawiam.
 */

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

void CreateAndSave(){
		HANDLE hFile = createFile();
	_writeLineToFile(hFile);
}

int main(int argc, TCHAR *argv[], TCHAR *envp[]){
	
	PROCESS_INFORMATION pi1; 
	std::string cmd1 = "MonitorProcess.exe file";
 	STARTUPINFO si1 ;
 	ZeroMemory( &si1, sizeof(si1) );
    si1.cb = sizeof(si1);
    ZeroMemory( &pi1, sizeof(pi1) );
	if(CreateProcess(0,(char*) cmd1.c_str(), 0,0,TRUE, CREATE_NEW_CONSOLE, 0,0, &si1, &pi1))
    { 
       
    }  
    else
    {
        std::cout<<("The process could not be started...")<<std::endl;
    }	
	
	PROCESS_INFORMATION pi2; 
	std::string cmd2 = "MonitorProcess.exe monitorProcess";
 	STARTUPINFO si2;
 	ZeroMemory( &si2, sizeof(si2) );
    si2.cb = sizeof(si2);
    ZeroMemory( &pi2, sizeof(pi2) );
	if(CreateProcess(0,(char*) cmd2.c_str(), 0,0,TRUE, CREATE_NEW_CONSOLE, 0,0, &si2, &pi2)){
	
	}
	else
    {
        std::cout<<("The process could not be started...")<<std::endl;
    }
		
 	
 	
	
}
