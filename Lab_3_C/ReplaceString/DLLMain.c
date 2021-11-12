#define _CRT_SECURE_NO_WARNINGS

#include <Windows.h>
#include <stdlib.h>
#include <stdio.h>

typedef struct {
    char src[255];
    char res[255];
} DataToSend;

BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved);
DataToSend GetData();
extern __declspec(dllexport) void __stdcall ReplaceString(DWORD pid, const char *srcStr, const char *resStr);

BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
    DWORD pid = GetCurrentProcessId();

    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    {
        DataToSend data = GetData();
        ReplaceString(pid, data.src, data.res);
        break;
    }
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

DataToSend GetData()
{
    TCHAR lpTempPathBuffer[MAX_PATH];

    GetTempPath(MAX_PATH, lpTempPathBuffer);

    char pathToFile[MAX_PATH];
    wcstombs(pathToFile, lpTempPathBuffer, wcslen(lpTempPathBuffer) + 1);
    FILE* fp;
    DataToSend data = { 0 };
    if ((fp = fopen(strcat(pathToFile, "dataToSend.txt"), "r+")) != NULL)
    {
        fscanf(fp, "%s %s", &data.src, &data.res);
        fclose(fp);
    }

    return data;
}