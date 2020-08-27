#include <iostream>
#include <fstream> 
#include <windows.h>
#include <tchar.h>
int main(int argc, TCHAR *argv[], TCHAR *envp[]){
	PROCESS_INFORMATION pi1; 
	std::string cmd1 = "randB.exe 10"; // argument ile liczb ma byc wylosowanych
 	STARTUPINFO si1 ;
 	ZeroMemory( &si1, sizeof(si1) );
    si1.cb = sizeof(si1);
    ZeroMemory( &pi1, sizeof(pi1) );
	if(!CreateProcess(0,(char*) cmd1.c_str(), 0,0,TRUE, CREATE_NEW_CONSOLE, 0,0, &si1, &pi1))
    { 
       std::cout<<("The process could not be started...")<<std::endl;
    }  
}
