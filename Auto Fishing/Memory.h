#include "pInc.h"

class Memory {
public:
	template <class _R> _R readMem(DWORD addr) {
		_R buff;
		ReadProcessMemory(pHandle, (LPVOID)addr, &buff, sizeof(_R), 0);
		return buff;
	}

	template <class _W> _W writeMem(DWORD addr, _W buffer) {
		WriteProcessMemory(pHandle, (LPVOID)addr, &buffer, sizeof(_W), 0);
		return 0;
	}
	DWORD getProcess(const char* procName);
	MODULEENTRY32 getModule(const char* moduleName, DWORD pid);

	HANDLE pHandle;
};