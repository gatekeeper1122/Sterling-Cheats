#include "stdafx.h"


void InitializingHooking() {
	Addresses::LoadAddresses();

	//game startup here.
	Hooks::InitializeHooks();
}

BOOL WINAPI DllMain(HANDLE hInstDLL, DWORD fdwReason, LPVOID lpReserved) {
	if (fdwReason == DLL_PROCESS_ATTACH) {
		InitializingHooking();
	}
	else if (fdwReason == DLL_PROCESS_DETACH) {
		Hooks::UnloadHooks();
	}
	return TRUE;
}