#include <iostream>
#include <fstream> 
#include <windows.h>
#include <tchar.h>
#include <stdio.h>

#define MAX_THREADS 1
#define MAX_SEM_COUNT 1

DWORD WINAPI mathFunction(LPVOID lpParameter);
HANDLE createFile();
HANDLE createFileForResult();
int * randNumbers(int n);
bool _writeLineToFile(HANDLE h, int n);
bool _writeLineToFileForResult(HANDLE h, int n);
bool _writeLineToFileForResultString(HANDLE h, std::string value);
typedef struct MyData{
	int nData;
	int * numbers;
}MYDATA, *PMYDATA;
CRITICAL_SECTION CriticalSection; 
HANDLE ghSemaphore;
HANDLE gMutex = NULL;
HANDLE file;
int main(int argc, TCHAR *argv[], TCHAR *envp[]){
	PMYDATA pDataArray[MAX_THREADS];
	HANDLE  hThreadArray[MAX_THREADS];
	DWORD   dwThreadIdArray[MAX_THREADS];
	std::string str="runSecondProcess";
	TCHAR *param=new TCHAR[str.size()+1];
	param[str.size()]=0;
	std::copy(str.begin(),str.end(),param);
	if(argv[1]==str){
		if (!InitializeCriticalSectionAndSpinCount(&CriticalSection, 
        0x00000400) ) {
        	return 0;
		}
		std::cout<<"Start of second process"<<std::endl;
		HANDLE hMutex = CreateMutex(NULL, FALSE, "MUTEXA");
		if(hMutex){
		 	std::cout<<"OpenMutex(): "<<GetLastError()<<std::endl;
		 }
		WaitForSingleObject(hMutex,INFINITE);
		HANDLE hMapFile;
		std::string str;
		std::string delimiter = " ";
		size_t pos = 0;
		std::string token;
		int nArg, repeatArg, secondsArg;
		if (argc <= 1 ) { 
			std::cout<<"Error"<<std::endl;
			}else{
			sscanf(argv[2], "%p", &hMapFile);
			nArg = atoi(argv[3]);
			repeatArg = atoi(argv[4]);
			secondsArg = atoi(argv[5]);
		}
		
		int* numbers = new int[repeatArg * nArg];
		char  *pMapFile = (char *)MapViewOfFile(hMapFile, FILE_MAP_READ, 0,0, 0);
		int len = strlen(pMapFile);
		char *array = new char[len];
		DWORD sizeF = len;
		memcpy(&array[0], pMapFile, sizeF);
		for(int i=0;i<len;i++){
			str += array[i];
		}
		int j = 0;
		while ((pos = str.find(delimiter)) != std::string::npos) {
		    token = str.substr(0, pos);
		    numbers[j]=stoi(token);
		    str.erase(0, pos + delimiter.length());
		    j++;
		}
		ghSemaphore = CreateSemaphore( NULL, 1, MAX_SEM_COUNT, "SEMAPHOREA");  
        if (ghSemaphore == NULL) 
	    {
	        std::cout<<"CreateSemaphore error: "<<GetLastError()<<std::endl;
	        return 1;
	    }
	    file = createFileForResult();
	    SYSTEM_INFO siSysInfo;
	    GetSystemInfo(&siSysInfo); 
	    auto totalNumbers = std::to_string(nArg*repeatArg);
	    auto repeat = std::to_string(repeatArg);
	    auto seconds = std::to_string(secondsArg);
	    auto processorInfo = std::to_string(siSysInfo.dwProcessorType);
	    std::string text = "Total numbers: "+ totalNumbers +" | Version: Multithreads | Machine: "+
		processorInfo+
		" | Replies: "+repeat+" | Pause: "+seconds+"\n"+"\n"+
		"Nr\tAvg\tMin\tMax \n";
	    _writeLineToFileForResultString(file, text);
	    int* tempNumbers = new int[nArg];
	    for(int i=0;i<repeatArg;i++){
	    	for(int j= i * nArg,k=0;j< (i+1) * nArg;j++,k++){
	    		tempNumbers[k] = numbers[j];
			}
	    	std::cout<<"Calculation number:"<<i+1<<std::endl;
			for( int k=0; k<MAX_THREADS; k++ ){
				pDataArray[k] = (PMYDATA) HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY,
	                	sizeof(MYDATA));
	                	
	            if( pDataArray[k] == NULL ){
	           		std::cout<<"Error with allocation"<<std::endl;
	            	ExitProcess(2);
	        	}
	        	
	        	pDataArray[k]->nData = nArg;
	        	pDataArray[k]->numbers = tempNumbers;
	        	auto s = std::to_string(i+1);
			    _writeLineToFileForResultString(file, s);
			    _writeLineToFileForResultString(file, " ");
	        	hThreadArray[k] = CreateThread( 
	            NULL,                   
	            0,                      
	            mathFunction,       
	            pDataArray[k],         
	            0,                     
	            &dwThreadIdArray[k]);
	            if (hThreadArray[k] == NULL) 
			    {
			       std::cout<<"Error with thread "+k<<std::endl;
			       ExitProcess(3);
			    }
			    WaitForSingleObject(hThreadArray[k],dwThreadIdArray[k]);
			}
			WaitForMultipleObjects(MAX_THREADS, hThreadArray, TRUE, INFINITE);
		    for(int l=0; l<MAX_THREADS; l++)
		    {
	        	CloseHandle(hThreadArray[l]);
		        if(pDataArray[l] != NULL)
		        {
		            HeapFree(GetProcessHeap(), 0, pDataArray[l]);
		            pDataArray[l] = NULL;   
		        }
		    }
		}
		CloseHandle(ghSemaphore);
		UnmapViewOfFile(pMapFile);
		CloseHandle(file);
		CloseHandle(hMapFile);
		CloseHandle(hMutex);
		DeleteCriticalSection(&CriticalSection);
		Sleep(5000);
	}
	
	else{
    std::cout<<"First process"<<std::endl;
    int arg =  atoi(argv[1]);
    int secondsArg =  atoi(argv[2]);
    int repeatArg =  atoi(argv[3]);
    HANDLE hMapFile;
    HANDLE hFile = createFile();
    SECURITY_ATTRIBUTES sa;
    sa.nLength = sizeof(sa);
    sa.lpSecurityDescriptor = NULL;
    sa.bInheritHandle = TRUE;
    DWORD sizeF = arg*repeatArg*sizeof(int);
    hMapFile = CreateFileMapping(hFile, &sa, PAGE_READWRITE,0, sizeF, "MappFileA");
    if(hMapFile==NULL){
	 	std::cout<<"CreateFileMapping error:"<<GetLastError()<<std::endl;
		return 1; 
		}
	PROCESS_INFORMATION pi1; 
	char cmd1[128];
	sprintf(cmd1, "randA.exe runSecondProcess %p %d %d %d", hMapFile,arg, repeatArg, secondsArg);
 	STARTUPINFO si1 ;
 	ZeroMemory( &si1, sizeof(si1) );
    si1.cb = sizeof(si1);
    ZeroMemory( &pi1, sizeof(pi1) );
    gMutex = CreateMutex(NULL, FALSE, "MUTEXA");
    WaitForSingleObject(gMutex,INFINITE);
    if (gMutex == NULL) 
    {
    	std::cout<<"CreateMutex error: "<<GetLastError()<<std::endl;
        return 1;
    }
	if(!CreateProcess(0, cmd1, 0,0,TRUE, CREATE_NEW_CONSOLE, 0,0, &si1, &pi1))
	{ 
	    std::cout<<("The process could not be started...")<<std::endl;
	}  
	for(int i=0; i<repeatArg;i++){
		_writeLineToFile(hFile,arg);
		Sleep(secondsArg * 1000);
	}
	ReleaseMutex(gMutex);
	CloseHandle(hMapFile);
	CloseHandle(hFile);
	WaitForSingleObject(pi1.hProcess,INFINITE);
	return 1;
	}
}


DWORD WINAPI mathFunction(LPVOID lpParameter){
	HANDLE hSemaphore = OpenSemaphore(SEMAPHORE_ALL_ACCESS | SYNCHRONIZE,FALSE,"SEMAPHOREA");
	WaitForSingleObject(hSemaphore,INFINITE);
	PMYDATA pDataArray = (PMYDATA)lpParameter;
	int nArg = pDataArray->nData;
	int * numbers = pDataArray->numbers;	
	int sum;
	for(int i=0;i<nArg;i++){
		sum += numbers[i];
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
	_writeLineToFileForResultString(file, "\t");
	BOOLEAN isWritten =_writeLineToFileForResult(file,avg);
	if(!isWritten){
   		std::cout<<"Error: "<<GetLastError()<<std::endl;
	}
	_writeLineToFileForResultString(file, "\t");
	isWritten =_writeLineToFileForResult(file,min);
   	if(!isWritten){
   		std::cout<<"Error: "<<GetLastError()<<std::endl;
	}
	_writeLineToFileForResultString(file, "\t");
	isWritten = _writeLineToFileForResult(file,max);
   	if(!isWritten){
   		std::cout<<"Error: "<<GetLastError()<<std::endl;
	}
	_writeLineToFileForResultString(file, "\n");
	std::cout<<"Average is: "<< avg<<std::endl;
	std::cout<<"Minimum value is: "<< min<<std::endl;
	std::cout<<"Maximum value is: "<< max<<std::endl;
	
	BOOL bSemaphore = ReleaseSemaphore(hSemaphore, 1, NULL);
	if (!bSemaphore)
		{
			std::cout<<"Error mathFunction: "<<GetLastError()<<std::endl;
			Sleep(5000);
			return -1;
		}
	return 0;
}

HANDLE createFile(){
	HANDLE hFile = CreateFile(
		TEXT("rand.txt"),
		GENERIC_READ | GENERIC_WRITE,
		FILE_SHARE_WRITE | FILE_SHARE_READ,
		NULL,
		OPEN_ALWAYS,
		FILE_ATTRIBUTE_NORMAL,
		0
	);
	if(hFile){
		std::cout<<"File created"<<std::endl;
	}
	return hFile;
}
HANDLE createFileForResult(){

	HANDLE hFile = CreateFile(
		TEXT("wynik.txt"),
		GENERIC_READ | GENERIC_WRITE,
		FILE_SHARE_WRITE | FILE_SHARE_READ,
		NULL,
		CREATE_ALWAYS,
		FILE_ATTRIBUTE_NORMAL,
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
	return bFile;
}
bool _writeLineToFileForResult(HANDLE h, int numbers){
	std::string str;
	str += std::to_string(numbers) + " ";
	const char *cstr = str.c_str();
	DWORD bytesWritten = 0;
	DWORD bytesToWrite = strlen(cstr);
    BOOL bFile = WriteFile(
		h,
        cstr,
        bytesToWrite,
        &bytesWritten,
        NULL);
    if(bFile){
    	return true;
	}else{
		return false;
	}
	
}
bool _writeLineToFileForResultString(HANDLE h, std::string value){
	std::string str;
	str = value;
	const char *cstr = str.c_str();
	DWORD bytesWritten = 0;
	DWORD bytesToWrite = strlen(cstr);
    BOOL bFile = WriteFile(
		h,
        cstr,
        bytesToWrite,
        &bytesWritten,
        NULL);
	 if(bFile){
    	return true;
	}else{
		return false;
	}
}
