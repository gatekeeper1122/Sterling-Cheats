#include "stdafx.h"

void TitleHooks::RunOnTitleLoad(PLDR_DATA_TABLE_ENTRY moduleHandle) {
	auto mountPath = Utils::GetMountPath();
	Utils::MountPath("STERLING:", mountPath, true);
	delete[] mountPath;

	if (moduleHandle) {
		XEX_EXECUTION_ID* executionId = (XEX_EXECUTION_ID*)RtlImageXexHeaderField(moduleHandle->XexHeaderBase, XEX_HEADER_EXECUTION_ID);
		if (!executionId) {
			LOG_PRINT("Failed to get execution id!");
			return;
		}

		/* Cheat Handling */

		if (executionId->TitleID != 0xFFFE07D1) {
			DWORD dwAddress, dwSize;
			if (Requests::PacketDownloadPlugin(executionId->TitleID, &dwAddress, &dwSize)) {
				BYTE* bytes = (BYTE*)dwAddress;

				HANDLE handle;
				auto moduleLoadStatus = XexLoadImageFromMemory(bytes, dwSize, "sterling_engine.xex", XEX_MODULE_FLAG_DLL, 0, &handle);

				free(bytes);

				if (moduleLoadStatus != ERROR_SUCCESS) {
					if (moduleLoadStatus == 0xC0000018) {
						Utils::Notify("Sterling - Failed to load engine, conflicting module detected!");
					}

					Utils::Notify(Utils::va("Sterling - Error 0x%X occured when trying to load engine", moduleLoadStatus));
					return;
				}

				Utils::Notify("Sterling - Cheat engine loaded!");
			}
			else {
				Utils::Notify("Sterling - Cheats were not loaded. Please restart game to try again.");
			}
		}
	}
}