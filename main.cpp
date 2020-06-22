#include <iostream>
#include <windows.h>


BOOL FileExists(LPCTSTR szPath)
{
  DWORD dwAttrib = GetFileAttributes(szPath);

  return (dwAttrib != INVALID_FILE_ATTRIBUTES && 
         !(dwAttrib & FILE_ATTRIBUTE_DIRECTORY));
}

void _writeLineToFile(HANDLE h){
char str[] = "Example text testing WriteFile";
DWORD bytesToWrite = strlen(str);
DWORD bytesWritten = 0;
BOOL bfile;
bfile = WriteFile(h,
          str,
          bytesToWrite,
          &bytesWritten,
          NULL);

if(bfile == FALSE){
    std::cout<<"WriteFile Failed "<<GetLastError()<<std::endl;
}else{
    std::cout<<"WriteFile Succes!"<<std::endl;
}
CloseHandle(h);

}

int main(int argc, char** argv) {
	LPCSTR fileName = "wynik.txt";
	std::cout << "Arguments number: " << argc << std::endl;

	for(auto i = 0; i<argc; i++){
		std::cout << argv[i] << std::endl;
	}
	
	if(FileExists(fileName)){
		std::cout<<"File Exists"<<std::endl;
		if(DeleteFileA(fileName)){
		std::cout<<"File delete"<<std::endl;
		}
	}
	
	HANDLE hFile = CreateFile(
		fileName,
		GENERIC_WRITE,
		0,
		NULL,
		CREATE_NEW,
		FILE_ATTRIBUTE_NORMAL,
		NULL
	);
	if(hFile){
		std::cout<<"File created"<<std::endl;
	}
    _writeLineToFile(hFile);
    
}


