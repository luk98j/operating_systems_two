#include <iostream>
#include <fstream> 
#include <windows.h>
#include <tchar.h>
#include <thread>
class Task1{
	public:
	HANDLE createFile(){

	HANDLE hFile = CreateFile(
		"log.txt",
		GENERIC_ALL,
		0,
		NULL,
		CREATE_ALWAYS,
		FILE_ATTRIBUTE_NORMAL,
		NULL
	);
	if(hFile){
		std::cout<<"File created"<<std::endl;
	}
	return hFile;
}


bool _writeLineToFile(HANDLE h){
	
	std::string text;
	getline(std::cin, text);
	std::cout<<text;
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

void createAndSave(){
	HANDLE hFile = createFile();
	_writeLineToFile(hFile);
}
};


int main(int argc, TCHAR *argv[], TCHAR *envp[]){
	Task1 task;
	PROCESS_INFORMATION pi1; 
//	char cmd1[80] = "Task1::createAndSave";
 	STARTUPINFO si1 = {
 		sizeof(si1), NULL, NULL,(LPTSTR) "createAndSave", 500, 370,
 		420, 310, // szerokosc i wysokosc okna aplikacji [piksel]
 		0, 0,
 		0x0d, // atrybuty wypelnienia okna
 		STARTF_USEFILLATTRIBUTE | STARTF_USEPOSITION | STARTF_USESIZE,
 		0, 0, NULL, NULL, NULL }; 
 
 CreateProcess(0,task.createAndSave() , NULL,NULL,0, CREATE_NEW_CONSOLE, NULL,NULL, &si1, &pi1);
	
}
