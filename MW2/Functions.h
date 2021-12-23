#ifndef __FUNCTIONS_H__
#define __FUNCTIONS_H__

class Functions {
public:
	static char* GetFont();
	static void Cbuf_AddText(int LocalClient, char* Angles);
	static int R_RegisterFont(const char* name, int imageTrack);
};

class Dvar {
public:
	static bool GetBool(const char* Dvar);
};

#endif // __FUNCTIONS_H__