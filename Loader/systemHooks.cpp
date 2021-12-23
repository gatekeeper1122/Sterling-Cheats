#include "stdafx.h"

HRESULT SystemHooks::XexStartExecutableHook(FARPROC TitleProcessInitThreadProc) {
	auto res = XexStartExecutable(TitleProcessInitThreadProc);
	TitleHooks::RunOnTitleLoad((PLDR_DATA_TABLE_ENTRY)*XexExecutableModuleHandle);
	return res;
}

HRESULT SystemHooks::Initialize() {
	Hooking::HookModuleImport(MODULE_XAM, MODULE_KERNEL, 416, XexStartExecutableHook);
	return S_OK;
}