#include <iostream>
#include <fstream> 
#include <windows.h>

bool FileExists(const TCHAR *fileName)
{
    DWORD fileAttr;
    fileAttr = GetFileAttributes(fileName);
    if (0xFFFFFFFF == fileAttr)
        return false;
    return true;
}


int main(int argc, TCHAR *argv[], TCHAR *envp[]){
	TCHAR *fileName = "log.txt";
	std::cout<<FileExists(fileName)<<std::endl;
}
