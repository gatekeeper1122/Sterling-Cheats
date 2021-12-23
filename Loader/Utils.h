#pragma once
class Utils {
public:
	static DWORD ResolveFunction(PCHAR Module, DWORD Ordinal);
	static HRESULT DoMountPath(const char* szDrive, const char* szDevice, const char* sysStr);
	static HRESULT MountPath(const char* szDrive, const char* szDevice, bool both);
	static const char* GetMountPath();

	static DWORD PatchModuleImport(PCHAR Module, PCHAR Import, DWORD Ordinal, DWORD Destination);
	static DWORD PatchModuleImport(PLDR_DATA_TABLE_ENTRY Module, PCHAR Import, DWORD Ordinal, DWORD Destination);
	static void Notify(const char* text);
	static HANDLE ResolveFunction(const char* pModuleName, DWORD dwOrdinal);
	static FARPROC ResolveFunction(HMODULE hHandle, DWORD Ordinal);
	static char* va(const char* fmt, ...);
	static char* vaBuff(char* vaBuffer, int size, const char* fmt, ...);
	static bool WriteFile(const CHAR* FilePath, const VOID* Data, DWORD Size);
};

