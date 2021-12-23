#include "stdafx.h"

char vaBuffer[0x1000];

DWORD Utils::ResolveFunction(PCHAR Module, DWORD Ordinal) {
	HANDLE Handle = 0;
	if (NT_SUCCESS(XexGetModuleHandle(Module, &Handle))) {
		DWORD Address = 0;
		if (NT_SUCCESS(XexGetProcedureAddress(Handle, Ordinal, &Address))) return Address;
	}
	return 0;
}

HRESULT Utils::DoMountPath(const char* szDrive, const char* szDevice, const char* sysStr) {
	STRING DeviceName, LinkName;
	CHAR szDestinationDrive[MAX_PATH];
	RtlSnprintf(szDestinationDrive, MAX_PATH, sysStr, szDrive);
	RtlInitAnsiString(&DeviceName, szDevice);
	RtlInitAnsiString(&LinkName, szDestinationDrive);
	ObDeleteSymbolicLink(&LinkName);
	return (HRESULT)ObCreateSymbolicLink(&LinkName, &DeviceName);
}

const char* Utils::GetMountPath() {
	char* DEVICE_DYNAMIC = new char[MAX_PATH];
	wstring ws;
	PLDR_DATA_TABLE_ENTRY TableEntry;
	XexPcToFileHeader((PVOID)0x90e00000, &TableEntry);

	if (TableEntry) {

		ws = TableEntry->FullDllName.Buffer;
		string FullDllName(ws.begin(), ws.end());

		ws = TableEntry->BaseDllName.Buffer;
		string BaseDllName(ws.begin(), ws.end());

		string::size_type i = FullDllName.find(BaseDllName);

		if (i != string::npos)
			FullDllName.erase(i, BaseDllName.length());

		memset(DEVICE_DYNAMIC, 0x0, MAX_PATH);
		strcpy(DEVICE_DYNAMIC, FullDllName.c_str());
	}
	else {
		LOG_PRINT("Mounting failed!");
	}

	return DEVICE_DYNAMIC;
}

HRESULT Utils::MountPath(const char* szDrive, const char* szDevice, bool both) {
	HRESULT res;
	if (both) {
		res = DoMountPath(szDrive, szDevice, "\\System??\\%s");
		res = DoMountPath(szDrive, szDevice, "\\??\\%s");
	}
	else {
		if (KeGetCurrentProcessType() == 2) //SYSTEM_PROC
			res = DoMountPath(szDrive, szDevice, "\\System??\\%s");
		else
			res = DoMountPath(szDrive, szDevice, "\\??\\%s");
	}
	return res;
}

VOID PatchInJump(PDWORD Address, DWORD Destination, BOOL Linked) {
	if (Destination & 0x8000) Address[0] = (0x3D600000 + (((Destination >> 16) & 0xFFFF) + 1));
	else Address[0] = (0x3D600000 + ((Destination >> 16) & 0xFFFF));
	Address[1] = (0x396B0000 + (Destination & 0xFFFF));
	Address[2] = 0x7D6903A6;
	Address[3] = (0x4E800420 | Linked);
	doSync(Address);
}

DWORD Utils::PatchModuleImport(PLDR_DATA_TABLE_ENTRY Module, PCHAR Import, DWORD Ordinal, DWORD Destination) {
	DWORD OrdinalAddress = Utils::ResolveFunction(Import, Ordinal);
	if (OrdinalAddress == 0) return S_FALSE;

	PXEX_IMPORT_DESCRIPTOR ImportDesc = (PXEX_IMPORT_DESCRIPTOR)RtlImageXexHeaderField(Module->XexHeaderBase, XEX_HEADER_IMPORTS);
	if (ImportDesc == 0) return S_FALSE;

	PXEX_IMPORT_TABLE ImportTable = (PXEX_IMPORT_TABLE)((PBYTE)ImportDesc + sizeof(*ImportDesc) + ImportDesc->NameTableSize);
	for (DWORD i = 0; i < ImportDesc->ModuleCount; i++)
	{
		for (WORD j = 0; j < ImportTable->ImportCount; j++)
		{
			DWORD StubAddress = *((PDWORD)ImportTable->ImportStubAddr[j]);
			if (OrdinalAddress != StubAddress) continue;
			StubAddress = (DWORD)ImportTable->ImportStubAddr[j + 1];
			PatchInJump((PDWORD)StubAddress, Destination, FALSE);
			j = ImportTable->ImportCount;
		}
		ImportTable = (PXEX_IMPORT_TABLE)((PBYTE)ImportTable + ImportTable->TableSize);
	}
	return S_OK;
}

DWORD Utils::PatchModuleImport(PCHAR Module, PCHAR Import, DWORD Ordinal, DWORD Destination) {
	PLDR_DATA_TABLE_ENTRY Handle = (PLDR_DATA_TABLE_ENTRY)GetModuleHandle(Module);
	return (Handle != 0) ? PatchModuleImport(Handle, Import, Ordinal, Destination) : S_FALSE;
}

void Utils::Notify(const char* text) {
	wchar_t* buf = (wchar_t*)calloc(1024, 1);
	wsprintfW(buf, L"%S", text);
	XNotifyQueueUI(XNOTIFYUI_TYPE_PREFERRED_REVIEW, 0, 2, buf, 0);
	free(buf);
}

HANDLE Utils::ResolveFunction(const char* pModuleName, DWORD dwOrdinal) {
	HANDLE proc = NULL;
	HANDLE hModule = NULL;

	if (!NT_SUCCESS(XexGetModuleHandle((char*)pModuleName, &hModule)))
		return NULL;

	if (!NT_SUCCESS(XexGetProcedureAddress(hModule, dwOrdinal, &proc)))
		return NULL;

	return proc;
}

FARPROC Utils::ResolveFunction(HMODULE hHandle, DWORD Ordinal) {
	return (hHandle == NULL) ? NULL : GetProcAddress(hHandle, (LPCSTR)Ordinal);
}

char* Utils::va(const char* fmt, ...) {
	memset(vaBuffer, 0, 0x1000);
	va_list ap;
	va_start(ap, fmt);
	RtlVsprintf(vaBuffer, fmt, ap); // RtlVsprintf
	va_end(ap);
	return vaBuffer;
}

char* Utils::vaBuff(char* vaBuffer, int size, const char* fmt, ...) {
	memset(vaBuffer, 0, size);
	va_list ap;
	va_start(ap, fmt);
	RtlVsprintf(vaBuffer, fmt, ap);
	va_end(ap);
	return vaBuffer;
}

bool Utils::WriteFile(const CHAR* FilePath, const VOID* Data, DWORD Size) {
	// Open our file
	HANDLE fHandle = CreateFile(FilePath, GENERIC_WRITE, FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (fHandle == INVALID_HANDLE_VALUE) {
		LOG_PRINT("WriteFile - CreateFile failed");
		return FALSE;
	}

	// Write our data and close
	DWORD writeSize = Size;
	if (::WriteFile(fHandle, Data, writeSize, &writeSize, NULL) != TRUE) {
		LOG_PRINT("WriteFile - WriteFile failed");
		return FALSE;
	}

	CloseHandle(fHandle);

	// All done
	return TRUE;
}