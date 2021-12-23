#ifndef __TOOLS_H__
#define __TOOLS_H__

enum titleIDs {
	MW2 = 0x41560817
};

class Tools {
public:
	static bool CheckGame(DWORD TitleID);
	static void Notify(const char* text);
};

class color {
public:
	color();
	color(float red, float green, float blue, float alpha);

	void alphaTransitionFade(float alphaToFadeTo, float transitionTime) {
		if (a > alphaToFadeTo)
			a -= transitionTime;

		else if (a < alphaToFadeTo)
			a += transitionTime;

		if (a == alphaToFadeTo)
			return;
	}

	float r, g, b, a;
};

extern color colorWhite;
extern color colorBlack;
extern color colorRed;
extern color colorGreen;
extern color colorBlue;
extern color colorGrey;
extern color colorGrey2;
extern color colorLightBlue;

#endif // __TOOLS_H__