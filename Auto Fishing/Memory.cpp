#include "Memory.h"

DWORD Memory::getProcess(const char* procName) {
	DWORD pid;
	HANDLE hProcess = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hProcess != INVALID_HANDLE_VALUE) {
		PROCESSENTRY32 entry;
		entry.dwSize = sizeof(entry);
		do
		{
			if (!strcmp(procName, entry.szExeFile)) {
				pid = entry.th32ProcessID;
				pHandle = OpenProcess(PROCESS_ALL_ACCESS, 0, pid);
				CloseHandle(hProcess);
			}
		} while (Process32Next(hProcess, &entry));
	}
	return pid;
}

MODULEENTRY32 Memory::getModule(const char* moduleName, DWORD pid) {
	MODULEENTRY32 mod;
	HANDLE hModule = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE32 || TH32CS_SNAPMODULE, pid);
	mod.dwSize = sizeof(mod);
	do
	{
		if (!_stricmp(moduleName, mod.szModule)) {
			CloseHandle(hModule);
			break;
		}
	} while (Module32Next(hModule, &mod));
	return mod;
}

