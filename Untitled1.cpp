#include <iostream>
#include <fstream> 
#include <windows.h>


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
	char cmd1[80] = "Untitled1 -mode CreateAndSave";
	
 	STARTUPINFO si1 = {
 		sizeof(si1), NULL, NULL,(LPTSTR) "createAndSave", 500, 370,
 		420, 310, 
 		0, 0,
 		0x0d, 
 		STARTF_USEFILLATTRIBUTE | STARTF_USEPOSITION | STARTF_USESIZE,
 		0, 0, NULL, NULL, NULL }; 
 		
 		
 		CreateProcess(0,cmd1, NULL,NULL,0, CREATE_NEW_CONSOLE, NULL,NULL, &si1, &pi1);
 	
 		std::string cas= "CreateAndSave";
 		for(int i=0;i<argc;i++){
 			if(argv[i]==(TCHAR*)cas.c_str()){
 			CreateAndSave();
		 	}
	 	}
 		
 	
	
}
