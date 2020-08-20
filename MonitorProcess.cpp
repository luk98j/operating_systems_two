#include <iostream>
#include <fstream> 
#include <windows.h>

DWORD readLettersFromFile(HANDLE hFile, DWORD dwFileSize){
	TCHAR data[100];
    DWORD dwRead;
	ReadFile(hFile, &data, dwFileSize, &dwRead, NULL);
   	
    CloseHandle(hFile);
    return dwRead;
}

HANDLE create(){

HANDLE hFile = CreateFile("log.txt", GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL,
        OPEN_EXISTING, 0, NULL);
	CreateFileMapping(hFile, 
                    NULL, 
                    PAGE_READONLY,
                    0, 
                    1,
                    NULL);
return hFile;                
}

DWORD FileSize(HANDLE hFile){
	
	DWORD dwFileSize = GetFileSize(hFile, NULL);
	std::cout<<dwFileSize<<" bytes"<<std::endl;
	return dwFileSize;
}

bool FileExists(const TCHAR *fileName)
{
    DWORD fileAttr;
    fileAttr = GetFileAttributes(fileName);
    if (0xFFFFFFFF == fileAttr)
        return false;
    return true;
}


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


int main(int argc, TCHAR *argv[], TCHAR *envp[]){
	//std::string strCreateAndSave = "file";
	//TCHAR* tcharCreateAndSave = (TCHAR*)strCreateAndSave.c_str();
	std::string str="file";
	TCHAR *param=new TCHAR[str.size()+1];
	param[str.size()]=0;
	std::copy(str.begin(),str.end(),param);
	if(str==argv[1]){
		HANDLE hFile = createFile();
		_writeLineToFile(hFile);
	}else{
		std::cout<<"Monitor process"<<std::endl;
		std::string path = "log.txt";
		TCHAR *fileName = (TCHAR*)path.c_str();
		int i = 0 ;
		DWORD temp[100];
		while(true){
		
		if(FileExists(fileName)){
			std::cout<<"File exists"<<std::endl;
			HANDLE hFile = create();
			DWORD dwFileSize = FileSize(hFile);
			temp[i] = readLettersFromFile(hFile,dwFileSize);
			if(i!=0&&temp[i-1]!=temp[i]){
				std::cout<<"Char diffrence: "<<temp[i]-temp[i-1]<<std::endl;
			}
		
		}else{
			std::cout<<"File doesn't exists"<<std::endl;
		}
		i++;
		Sleep(7000);
		}
		
	}

}
