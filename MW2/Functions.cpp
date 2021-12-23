#include "stdafx.h"

char* Functions::GetFont() {
	char* defaultFont = "";

	if (Tools::CheckGame(MW2))
		defaultFont = "fonts/smallFont";

	return defaultFont;
}

int Functions::R_RegisterFont(const char* name, int imageTrack) {
	return ((int(*)(...))addrs.R_RegisterFont)(name, imageTrack);
}

void Functions::Cbuf_AddText(int LocalClient, char* Angles) {
	return ((void(*)(...))addrs.Cbuf_AddText)(LocalClient, Angles);
}

bool Dvar::GetBool(const char* Dvar) {
	return ((bool(*)(...))addrs.Dvar_GetBool)(Dvar);
}