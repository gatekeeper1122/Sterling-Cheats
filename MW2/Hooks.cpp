#include "stdafx.h"

detour<void>* SND_EndFrameDetour;

void Hooks::SND_EndFrameHook() {
	Menu::UpdateMenu();

	SND_EndFrameDetour->callOriginal();
}

void Hooks::InitializeHooks() {
	SND_EndFrameDetour = new detour<void>;
	SND_EndFrameDetour->setupDetour(addrs.SND_EndFrame, (PVOID)SND_EndFrameHook);
}

void Hooks::UnloadHooks() {
	SND_EndFrameDetour->takeDownDetour();
}