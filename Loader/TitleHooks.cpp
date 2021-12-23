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

		if (Requests::PacketGetPlugin(executionId->TitleID)) {
			LOG_PRINT("Successfully loaded Sterling Cheat for 0x%x", executionId->TitleID);
		}
	}
}