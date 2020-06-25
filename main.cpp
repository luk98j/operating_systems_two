#include <iostream>
#include <windows.h>
#include <tchar.h> 


BOOL FileExists(LPCTSTR szPath)
{
  DWORD dwAttrib = GetFileAttributes(szPath);

  return (dwAttrib != INVALID_FILE_ATTRIBUTES && 
         !(dwAttrib & FILE_ATTRIBUTE_DIRECTORY));
}

HANDLE createFile(LPCSTR fileName){
	if(FileExists(fileName)){
		std::cout<<"File Exists"<<std::endl;
	}
	
	HANDLE hFile = CreateFile(
		fileName,
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


bool _writeLineToFile(HANDLE h, TCHAR *str[],int argc){
auto size = 0;
BOOL bFile;
DWORD bytesWritten = 0;
	 for(int i=0;i<argc;i++){
	 	size = strlen(str[i]);
        bFile = WriteFile(h,
          strcat(str[i], "\n"),
          size+1,
          &bytesWritten,
          NULL);
     }

CloseHandle(h);
return bFile;
}
/*void readFile(HANDLE hFile){
	DWORD loadedBytes = 0;
	char fileContent[BYTEStoREAD];
	
	ReadFile(
	hFile,
	fileContent,
	fileSize(hFile),
	&loadedBytes,
	NULL
	);
	
	CloseHandle(hFile);
	std::cout<< "Text from file: "<<std::endl;
	puts(fileContent);
	
	
}
*/
int main(int argc, TCHAR *argv[], TCHAR *envp[]) {
	LPCSTR fileName = "wynik.txt";	
	HANDLE hFile = createFile(fileName);
    BOOL writeCompleted = _writeLineToFile(hFile,argv,argc);
    if(writeCompleted == FALSE){
    	std::cout<<"WriteFile Failed "<<GetLastError()<<std::endl;
	}else{
    	std::cout<<"WriteFile Succes!"<<std::endl;	
	}	
    //readFile(hFile);
}


