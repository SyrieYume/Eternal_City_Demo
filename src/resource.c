#include <windows.h>
#include <stdio.h>
#include <io.h>


// 从文件中加载数据
BOOL Res_LoadFromFile(char *filepath, BYTE **data, DWORD *dwSize) {
    HANDLE hFile = CreateFileA(filepath, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if(hFile == INVALID_HANDLE_VALUE)
        return FALSE;

    LARGE_INTEGER fileSize;
    GetFileSizeEx(hFile, &fileSize);

    *dwSize = fileSize.LowPart;
    *data = (BYTE*)malloc(*dwSize);

    DWORD dwRead = 0;
    ReadFile(hFile, *data, *dwSize, &dwRead, NULL);

    CloseHandle(hFile);

    return TRUE;
}


// 从资源中加载数据
BOOL Res_LoadFromResource(int resourceId, BYTE **data, DWORD *dwSize) {
    HRSRC hResource = FindResourceA(NULL, MAKEINTRESOURCEA(resourceId), MAKEINTRESOURCEA(RT_RCDATA)); 
    if(!hResource)
        return FALSE;

    HGLOBAL hGlobal = LoadResource(NULL, hResource);
    *dwSize = SizeofResource(NULL, hResource);
    *data = (BYTE*)malloc(*dwSize);
    memcpy(*data, (BYTE*)LockResource(hGlobal), *dwSize);

    UnlockResource(hGlobal);
    return TRUE;
}


// 从资源中加载数据，并且写入文件
BOOL Res_LoadResourceToFile(int resourceId, char *filepath) {
    BYTE *data;
    DWORD dwSize;
    
    if(!Res_LoadFromResource(resourceId, &data, &dwSize))
        return FALSE;

    HANDLE hFile = CreateFileA(filepath, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_TEMPORARY, NULL);
    if(hFile == INVALID_HANDLE_VALUE)
        return FALSE;
    
    DWORD dwWrite = 0;
    WriteFile(hFile, data, dwSize, &dwWrite, NULL);
    
    CloseHandle(hFile);
    free(data);

    return TRUE;
}


// 从资源中加载字体
BOOL Res_LoadFontResource(int resourceId, char *fontName) {
    BYTE *data;
    DWORD dwSize;
    
    if(!Res_LoadFromResource(resourceId, &data, &dwSize))
        return FALSE;

    DWORD num = 1;
    AddFontMemResourceEx((PVOID)data, dwSize, 0, &num);

    free(data);

    return TRUE;
}


// 获取一个可用的随机临时文件名
BOOL Res_GetTempFileName(char *prefix, char *surfix, char *resultFilepath) {
    static char tempPath[MAX_PATH], tempFileName[MAX_PATH];

    GetTempPathA(MAX_PATH, tempPath);
    
    while(TRUE) {
        int randNum = rand();
        sprintf(resultFilepath, "%s%s-%d%s", tempPath, prefix, randNum, surfix);
        
        if(_access(resultFilepath, 0) != 0)
            return TRUE;
    }

    return TRUE;
}