#include <iostream>
#include <fstream> 
#include <windows.h>
#include <tchar.h>
#include <stdio.h>

#define MAX_THREADS 1

DWORD WINAPI mathFunction(LPVOID lpParameter);
HANDLE createFile();
int * randNumbers(int n);
bool _writeLineToFile(HANDLE h, int n);

typedef struct MyData{
	int nData;
	int * numbers;
}MYDATA, *PMYDATA;

int main(int argc, TCHAR *argv[], TCHAR *envp[]){
	PMYDATA pDataArray[MAX_THREADS];
	HANDLE  hThreadArray[MAX_THREADS];
	DWORD   dwThreadIdArray[MAX_THREADS];
	std::string str="runSecondProcess";
	TCHAR *param=new TCHAR[str.size()+1];
	param[str.size()]=0;
	std::copy(str.begin(),str.end(),param);
	if(argv[1]==str){
		HANDLE hMapFile;
		std::string str;
		std::string delimiter = " ";
		size_t pos = 0;
		std::string token;
		int nArg;
		if (argc <= 1 ) { 
			std::cout<<"Error"<<std::endl;
			}else{
			sscanf(argv[2], "%p", &hMapFile);
			nArg = atoi(argv[3]);
			}
		int* numbers = new int[nArg];
		std::cout<<"Start of second process"<<std::endl;
		Sleep(1000);
		char  *pMapFile = (char *)MapViewOfFile(hMapFile, FILE_MAP_READ, 0,0, 0);
		int len = strlen(pMapFile);
		char *array = new char[len];
		DWORD sizeF = len;
		memcpy(&array[0], pMapFile, sizeF);
		for(int i=0;i<len;i++){
			str += array[i];
		}
		int i = 0;
		while ((pos = str.find(delimiter)) != std::string::npos) {
		    token = str.substr(0, pos);
		    numbers[i]=stoi(token);
		    str.erase(0, pos + delimiter.length());
		    i++;
		}
		for( int i=0; i<MAX_THREADS; i++ ){
			pDataArray[i] = (PMYDATA) HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY,
                	sizeof(MYDATA));
                	
            if( pDataArray[i] == NULL ){
           		std::cout<<"Error with allocation"<<std::endl;
            	ExitProcess(2);
        	}
        	
        	pDataArray[i]->nData = nArg;
        	pDataArray[i]->numbers = numbers;
        	
        	hThreadArray[i] = CreateThread( 
            NULL,                   // default security attributes
            0,                      // use default stack size  
            mathFunction,       // thread function name
            pDataArray[i],          // argument to thread function 
            0,                      // use default creation flags 
            &dwThreadIdArray[i]);
            
            if (hThreadArray[i] == NULL) 
		    {
		       std::cout<<"Error with thread "+i<<std::endl;
		       ExitProcess(3);
		    }
		}
		WaitForMultipleObjects(MAX_THREADS, hThreadArray, TRUE, INFINITE);

	    for(int i=0; i<MAX_THREADS; i++)
	    {
        	CloseHandle(hThreadArray[i]);
	        if(pDataArray[i] != NULL)
	        {
	            HeapFree(GetProcessHeap(), 0, pDataArray[i]);
	            pDataArray[i] = NULL;   
	        }
	    }
		UnmapViewOfFile(pMapFile);
		CloseHandle(hMapFile);
	}
	
	else{
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
	sprintf(cmd1, "randA.exe runSecondProcess %p %d", hMapFile,arg);
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


DWORD WINAPI mathFunction(LPVOID lpParameter){
	PMYDATA pDataArray = (PMYDATA)lpParameter;
	int nArg = pDataArray->nData;
	int * numbers = pDataArray->numbers;	
	int sum;
	for(int i=0;i<nArg;i++){
		sum += numbers[i];
		std::cout<<"Liczba od :"<<i<<" "<<numbers[i]<<std::endl;
	}
	int min = numbers[0];
   	for(int i=0; i<nArg; i++) {
      if(min>numbers[i]) {
         min=numbers[i];
      }
   	}
   	int max = numbers[0];
   	for(int i=0; i<nArg; i++) {
      if(max<numbers[i]) {
         max=numbers[i];
      }
   	}
	double avg = (double) sum/nArg;
	std::cout<<"Average is: "<< avg<<std::endl;
	std::cout<<"Minimum value is: "<< min<<std::endl;
	std::cout<<"Maximum value is: "<< max<<std::endl;
	Sleep(6000);
	return 0;
}

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

