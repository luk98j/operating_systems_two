/**
 * @file main.cpp
 * @author Lukasz Jankowski 224816
 * @date 25.06.2020
 * @brief EX 1 - Systemy operacyjne II, æwiczenie 1 - LAB
 */


#include <iostream>
#include <windows.h>
#include <tchar.h> 


BOOL environmentalVariableIsTrue(LPCTSTR environmentalVariable)
{
	
		const DWORD buffSize = 65535;
    	char buffer[buffSize];
    	GetEnvironmentVariableA(environmentalVariable, buffer, buffSize);
    	if(buffer[0]=='T' && buffer[3]=='E')
    	{
        std::cout<<buffer<<std::endl;
        return true;
    	}
    	else
    	{
        return false;
    	}
		
}

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
bool _openFileInNotepad(std::string name)
{
	environmentalVariableIsTrue("NOTEPAD");
	STARTUPINFO StartInfo;
	PROCESS_INFORMATION ProcessInfo;

  	memset(&StartInfo, 0, sizeof(StartInfo));
  	memset(&StartInfo, 0, sizeof(ProcessInfo));

  	StartInfo.cb      = sizeof(StartInfo);

  	StartInfo.wShowWindow = SW_NORMAL;
  	StartInfo.dwFlags    |= STARTF_USESHOWWINDOW;
  	
  	std::string notepad= "notepad ";
  	char *program= new char[notepad.length()+1];
  	strcpy(program,notepad.c_str());
  	const char *carray=  name.c_str();
  	TCHAR filename[255];		
  	_tcscpy(filename, _T(strcat(program,carray)));
  	if(!CreateProcess(0,
	  filename,
	  NULL,
	  NULL,
	  TRUE,
	  CREATE_NEW_PROCESS_GROUP,
	  NULL,
	  NULL,
	  &StartInfo,
	  &ProcessInfo)){
	  	 std::cout << "Something went wrong" << std::endl;
	  	 return false;
	  }else{
	  	return true;
	  }
  	
}
int main(int argc, TCHAR *argv[], TCHAR *envp[]) {
	LPCSTR fileName = "wynik.txt";	
	HANDLE hFile = createFile(fileName);
    BOOL writeCompleted = _writeLineToFile(hFile,argv,argc);
    if(writeCompleted == FALSE){
    	std::cout<<"WriteFile Failed "<<GetLastError()<<std::endl;
	}else{
    	std::cout<<"WriteFile Succes!"<<std::endl;	
	}	
    BOOL openInNotepad = _openFileInNotepad(fileName);
    if(openInNotepad == FALSE){
    	std::cout<<"Open in Notepad - fail: "<<GetLastError()<<std::endl;
	}else{
    	std::cout<<"Open in Notepad - success"<<std::endl;
	}
    
}


