#include <iostream>
#include <fstream> 
#include <windows.h>
#include <tchar.h>
int main(int argc, TCHAR *argv[], TCHAR *envp[]){
	LARGE_INTEGER freq, perf_start, perf_end;
	float freqms;
	QueryPerformanceFrequency(&freq);
	freqms = freq.QuadPart / 1000.0f;
	QueryPerformanceCounter(&perf_start);
	PROCESS_INFORMATION pi1; 
	std::string cmd1 = "randA.exe 10 3 10"; // argument ile liczb ma byc wylosowanych
 	STARTUPINFO si1 ;
 	ZeroMemory( &si1, sizeof(si1) );
    si1.cb = sizeof(si1);
    ZeroMemory( &pi1, sizeof(pi1) );
	if(!CreateProcess(0,(char*) cmd1.c_str(), 0,0,TRUE, CREATE_NEW_CONSOLE, 0,0, &si1, &pi1))
    { 
       std::cout<<("The process could not be started...")<<std::endl;
    }  
    WaitForSingleObject(pi1.hProcess,INFINITE);
    QueryPerformanceCounter(&perf_end);
    float elapsedC = (perf_end.QuadPart - perf_start.QuadPart) / freqms;
    std::cout<<"The application execution time is: "<<elapsedC<<" ms"<<std::endl;
}
