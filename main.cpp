#include <iostream>
#include <windows.h>


BOOL FileExists(LPCTSTR szPath)
{
  DWORD dwAttrib = GetFileAttributes(szPath);

  return (dwAttrib != INVALID_FILE_ATTRIBUTES && 
         !(dwAttrib & FILE_ATTRIBUTE_DIRECTORY));
}

DWORD fileSize(HANDLE hFile){
	return GetFileSize(hFile,NULL);
}

HANDLE createFile(LPCSTR fileName){
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
	return hFile;
}


void writeLineToFile(HANDLE h){
char str[] = "Example text testing WriteFile and new text";
DWORD bytesToWrite = strlen(str);
DWORD bytesWritten = 0;
BOOL bFile;
bFile = WriteFile(h,
          str,
          bytesToWrite,
          &bytesWritten,
          NULL);

if(bFile == FALSE){
    std::cout<<"WriteFile Failed "<<GetLastError()<<std::endl;
}else{
    std::cout<<"WriteFile Succes!"<<std::endl;
}
CloseHandle(h);

}
void readFile(HANDLE file){
	DWORD bytesRead = 0;
	char fileContent[60];
	ReadFile(
	file,
	fileContent,
	fileSize(file),
	&bytesRead,
	NULL
	);
	
	for(auto i = 0; i < (signed)strlen(fileContent);i++){
		std::cout<<fileContent[i]<<std::endl;
	}
}

int main(int argc, char** argv) {
	LPCSTR fileName = "wynik.txt";
	std::cout << "Arguments number: " << argc << std::endl;

	for(auto i = 0; i<argc; i++){
		std::cout << argv[i] << std::endl;
	}
	
	HANDLE hFile = createFile(fileName);
    writeLineToFile(hFile);
    readFile(hFile);
}


