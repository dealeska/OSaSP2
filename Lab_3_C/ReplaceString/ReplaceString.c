#include <Windows.h>
#include <stdlib.h>

__declspec(dllexport) void __stdcall ReplaceString(DWORD pid, const char* srcStr, const char* resStr)
{
	HANDLE hProcess = OpenProcess(PROCESS_VM_READ | PROCESS_VM_WRITE | PROCESS_QUERY_INFORMATION, FALSE, pid);

	if (hProcess)
	{
		SYSTEM_INFO systemInfo;
		GetSystemInfo(&systemInfo);

		MEMORY_BASIC_INFORMATION mbInfo;
		char* buf = 0;
		char* p = 0;

		while (p < systemInfo.lpMaximumApplicationAddress)
		{
			if (VirtualQueryEx(hProcess, p, &mbInfo, sizeof(mbInfo)) == sizeof(mbInfo))
			{
				if (mbInfo.State == MEM_COMMIT && mbInfo.AllocationProtect == PAGE_READWRITE)
				{
					p = (char*)mbInfo.BaseAddress;

					buf = 0;
					while (!buf)
					{
						buf = (char*)calloc(mbInfo.RegionSize, sizeof(char));
					}
					SIZE_T bytesRead;
					
					if (ReadProcessMemory(hProcess, p, buf, mbInfo.RegionSize, &bytesRead))
					{
						for (size_t i = 0; i <= (bytesRead - strlen(srcStr)); ++i)
						{
							char* ref = (char*)p + i;

							if (memcmp(srcStr, ref, strlen(srcStr)) == 0)
							{																
								memcpy(ref, resStr, strlen(srcStr) + 1);
								free(buf);
								return;
							}
						}
					}
					free(buf);
				}
			}
			p += mbInfo.RegionSize;
		}
	}
}