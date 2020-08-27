#include <iostream>
#include <fstream> 
#include <windows.h>
#include <tchar.h>
#include <stdio.h>

HANDLE createFile(){

	HANDLE hFile = CreateFile(
		"log.txt",
		GENERIC_READ|GENERIC_WRITE,
		FILE_SHARE_READ,
		0,
		CREATE_ALWAYS,
		0,
		0
	);
	if(hFile){
		std::cout<<"File created"<<std::endl;
	}
	return hFile;
}

int * randNumbers(int n){
	int* numbers = new int[n];
	for(int i=0;i<n;i++){
		//( std::rand() % ile_liczb_w_przedziale ) + startowa_liczba;
		numbers[i] = ( std::rand() % 100 ) + 1;
	}
	return numbers;
}

bool _writeLineToFile(HANDLE h, int n){
	int* numbers = randNumbers(n);
		std::cout<<"Size of array "<<n<<std::endl;
	for(int i=0;i<n;i++){
		std::cout<<"Element "<<i<<std::endl;
		std::cout<<numbers[i]<<std::endl;
	}
	std::string str;
	for(int i=0;i<n;i++){
		str += std::to_string(numbers[i]) + " ";
	}
	const char *cstr = str.c_str();
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


int main(int argc, TCHAR *argv[], TCHAR *envp[]){
	std::string str="runSecondProcess";
	TCHAR *param=new TCHAR[str.size()+1];
	param[str.size()]=0;
	std::copy(str.begin(),str.end(),param);
	
	if(argv[1]==str){
		HANDLE hMapFile;
		int nArg;
		if (argc <= 1 ) { 
			std::cout<<"Error"<<std::endl;
			}else{
			sscanf(argv[2], "%p", &hMapFile);
			nArg = atoi(argv[3]);
			}
		std::cout<<"Start of second process"<<std::endl;
		Sleep(3000);
		char  *pMapFile = (char *)MapViewOfFile(hMapFile, FILE_MAP_READ, 0,0, 0);
		int len = strlen(pMapFile);
		char *array = new char[len];
		DWORD sizeF = len;
		memcpy(&array[0], pMapFile, sizeF);
		std::string str;
		for(int i=0;i<len;i++){
			str += array[i];
		}
		std::string delimiter = " ";
		size_t pos = 0;
		std::string token;
		int* numbers = new int[nArg];
		int i = 0;
		while ((pos = str.find(delimiter)) != std::string::npos) {
		    token = str.substr(0, pos);
		    numbers[i]=stoi(token);
		    str.erase(0, pos + delimiter.length());
		    i++;
		}
		int sum;
		for(int i=0;i<nArg;i++){
			sum += numbers[i];
			std::cout<<"Liczba od :"<<i<<" "<<numbers[i]<<std::endl;
		}
		double avg = (double) sum/nArg;
		std::cout<<"Average is: "<< avg<<std::endl;
		UnmapViewOfFile(pMapFile);
		CloseHandle(hMapFile);
		int x;
		std::cin>>x;
	}else{
    std::cout<<"First process"<<std::endl;
    int arg =  atoi(argv[1]);
    HANDLE hMapFile;
    HANDLE hFile = createFile();
    SECURITY_ATTRIBUTES sa;
    sa.nLength = sizeof(sa);
    sa.lpSecurityDescriptor = NULL;
    sa.bInheritHandle = TRUE;
    DWORD sizeF = arg*sizeof(int);
    hMapFile = CreateFileMapping(hFile, &sa, PAGE_READWRITE,0, sizeF, NULL);
    if(hMapFile==NULL){
	 	std::cout<<"CreateFileMapping error:"<<GetLastError()<<std::endl;
		return 1; 
		}
	PROCESS_INFORMATION pi1; 
	//std::string cmd1 = "rand.exe runSecondProcess " + hMapFile;
	char cmd1[128];
	sprintf(cmd1, "rand.exe runSecondProcess %p %d", hMapFile,arg);
 	STARTUPINFO si1 ;
 	ZeroMemory( &si1, sizeof(si1) );
    si1.cb = sizeof(si1);
    ZeroMemory( &pi1, sizeof(pi1) );
    
	if(!CreateProcess(0, cmd1, 0,0,TRUE, CREATE_NEW_CONSOLE, 0,0, &si1, &pi1))
	{ 
	    std::cout<<("The process could not be started...")<<std::endl;
	}  
		_writeLineToFile(hFile,arg);
		CloseHandle(hMapFile);
		WaitForSingleObject(pi1.hProcess,INFINITE);
		return 1;
	}
}
