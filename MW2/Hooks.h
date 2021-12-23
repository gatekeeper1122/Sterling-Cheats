#ifndef __HOOKS_H__
#define __HOOKS_H__

class Hooks {
public:
	static void SND_EndFrameHook();
	static void InitializeHooks();
	static void UnloadHooks();
};

#endif // __HOOKS_H__