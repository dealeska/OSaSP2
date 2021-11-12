#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <Windows.h>

#define true 1
#define false 0

__declspec(dllimport) void __stdcall ReplaceString(DWORD pid, const char* srcString, const char* resString);
typedef void __stdcall TReplaceString(DWORD, const char*, const char*);


void Injection(DWORD PID, const char* srcStr, const char* resStr)
{
	HANDLE hProc = OpenProcess(PROCESS_VM_READ | PROCESS_VM_OPERATION | PROCESS_VM_WRITE |
		PROCESS_CREATE_THREAD | PROCESS_CREATE_PROCESS,
		FALSE, PID);

	if (hProc)
	{
		TCHAR lpTempPathBuffer[MAX_PATH];

		GetTempPath(MAX_PATH, lpTempPathBuffer);

		char pathToFile[MAX_PATH];
		wcstombs(pathToFile, lpTempPathBuffer, wcslen(lpTempPathBuffer) + 1);
		FILE* fp;
		if ((fp = fopen(strcat(pathToFile, "dataToSend.txt"), "w")) != NULL)
		{
			fprintf(fp, "%s %s", srcStr, resStr);
			fclose(fp);
		}

		LPVOID baseAddress = VirtualAllocEx(hProc, NULL, strlen("ReplaceString.dll") + 1, MEM_COMMIT, PAGE_READWRITE);
		int i = WriteProcessMemory(hProc, baseAddress, "ReplaceString.dll", strlen("ReplaceString.dll") + 1, NULL);
		LPVOID addr = (LPVOID)GetProcAddress(GetModuleHandle(L"kernel32.dll"), "LoadLibraryA");
		HANDLE threadID = CreateRemoteThread(hProc, NULL, 0, (LPTHREAD_START_ROUTINE)addr, baseAddress, NULL, NULL);

		if (threadID != NULL) {
			WaitForSingleObject(hProc, INFINITE);

			DWORD hLibrary = 0;
			GetExitCodeThread(hProc, &hLibrary);
			CloseHandle(hProc);

			FreeLibrary((HMODULE)hLibrary);
		}
	}
}

void ReplaceStringDynamic(DWORD PID, const char* srcStr, const char* resStr)
{
	HMODULE hDll = LoadLibrary(L"ReplaceString.dll");

	if (hDll)
	{
		TReplaceString* lpReplaceString = (TReplaceString*)GetProcAddress(hDll, "_ReplaceString@12");

		if (lpReplaceString != NULL)
		{
			lpReplaceString(PID, srcStr, resStr);
		}

		FreeLibrary(hDll);
	}
}

int main()
{
	byte isExit = false;
	char sourceString[255];
	char destString[255];
	char action;
	char flush[1];
	DWORD attachedPId;
	DWORD pId = GetCurrentProcessId();

	while (!isExit)
	{
		printf_s("\nChoose:\n");
		printf_s("1 - Static import.\n2 - Dynamic import.\n3 - DllInjection.\n4 - Exit.\n");

		scanf_s("%c", &action, 1);
		gets(flush);

		switch (action)
		{
		case '1':
			printf_s("Enter string to find: ");			
			gets(sourceString);
			printf_s("\nEnter string to change: ");
			gets(destString);
			ReplaceString(pId, sourceString, destString);
			printf("Changed string: %s", sourceString);
			break;
		case '2':
			printf_s("Enter string to find: ");
			gets(sourceString);
			printf_s("\nEnter string to change: ");
			gets(destString);
			ReplaceStringDynamic(pId, sourceString, destString);
			printf("Changed string: %s", sourceString);
			break;
		case '3':
			printf_s("Enter a pid: ");
			scanf_s("%d", &attachedPId);
			printf_s("Enter string to find: ");
			gets(sourceString);
			printf_s("\nEnter string to change: ");
			gets(destString);
			Injection(attachedPId, sourceString, destString);
			break;
		case '4':
			isExit = true;
			break;
		}
	}	
}