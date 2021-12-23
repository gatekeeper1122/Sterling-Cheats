#include "stdafx.h"

void SystemThread() {
	DbgPrint((char*)"Running...\n");

	auto mountPath = Utils::GetMountPath();
	if (!SUCCEEDED(Utils::MountPath("STERLING:", mountPath, true))) {
		FreeLibraryAndExitThread((HMODULE)Sterling.hMyModule, 0);
		*(WORD*)((DWORD)Sterling.hMyModule + 64) = 1;
		return;
	}

	delete[] mountPath;

	Utils::MountPath("HDD:", "\\Device\\Harddisk0\\Partition1\\", true);
	Utils::MountPath("USB:", "\\Device\\Mass0\\", true);

	Log::Initialize();

	if (FAILED(Hypervisor::Initialize())) {
		LOG_PRINT("Failed to initialize #3c50eb34");
		Sleep(4000);
		HalReturnToFirmware(HalFatalErrorRebootRoutine);
	}

	if (FAILED(SystemHooks::Initialize())) {
		LOG_PRINT("Failed to initialize #578ce42d");
		Sleep(4000);
		HalReturnToFirmware(HalFatalErrorRebootRoutine);
	}

	Sterling.bLoadedProperly = true;

	HANDLE hThread; DWORD hThreadID;
	ExCreateThread(&hThread, 0, &hThreadID, (PVOID)XapiThreadStartup, (LPTHREAD_START_ROUTINE)Requests::InitThread, NULL, 0x1c000427);
	XSetThreadProcessor(hThread, 4);
	ResumeThread(hThread);
	CloseHandle(hThread);
}

BOOL APIENTRY DllMain(HANDLE hModule, DWORD Reason, LPVOID Reserved) {
	switch (Reason) {
	case DLL_PROCESS_ATTACH:
		srand(time(0));
		if (!SMC::IsTrayOpen() && XboxKrnlVersion->Build == KERNEL_VERSION) {
			Sterling.hMyModule = hModule;

			XexPcToFileHeader((PVOID)0x91700000, &Sterling.pDataTableEntry);

			HANDLE hThread; DWORD hThreadID;
			ExCreateThread(&hThread, 0, &hThreadID, (PVOID)XapiThreadStartup, (LPTHREAD_START_ROUTINE)SystemThread, NULL, 0x1c000427);
			XSetThreadProcessor(hThread, 4);
			ResumeThread(hThread);
			CloseHandle(hThread);
			return TRUE;
		}
		break;
	}

	*(WORD*)((DWORD)hModule + 64) = 1;
	return FALSE;
}
