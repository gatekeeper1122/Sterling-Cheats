#pragma once
class SystemHooks {
public:
	static HRESULT Initialize();

	static HRESULT XexStartExecutableHook(FARPROC TitleProcessInitThreadProc);
};

